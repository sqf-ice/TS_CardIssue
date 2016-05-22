/************************************************************************/
/*							大唐芯片T514								*/
/************************************************************************/
#include "Chip_Script.h"
#include "tool.h"
#include <algorithm>

#include <iostream>
using namespace std;
/*个人化脚本公共变量说明的宏定义*/

CScript_T514::CScript_T514( int COS,  CInformation *cInfo ):CScript( COS, cInfo )
{
}

CScript_T514::~CScript_T514()
{
}


bool CScript_T514::IssueCardProcess()
{
	//设置指令类和命令类的某些状态
	cCmd->Error_Check(true);//打开参数检查
	cCmd->Style_Set(STYLE_ASSIGNMENT);//设置生成脚本命令的样式
	cInst->Error_Check(true);//打开参数检查
	cInst->Style_Set(STYLE_SEPARATOR);//设置生成的脚本指令的样式
	
	ScriptModeStatement();//脚本注释
	CON_Statement();//常量注释
	VAR_Statement();//变量注释
	PersonnalVAR_Statement();//个人化变量注释
	if ( m_nCheckKeys == 1 )
	{
		LvTwo_Annotations( "清卡", m_bRefreshFlag );
		ClearCarad();
	}
	if ( m_nCheckATR == 1 )
	{
		LvTwo_Annotations( "更新ATR", m_bRefreshFlag );
		UpdateATR();
	}

	GetAndSetParama( "5" );//获取参数类数据并设置脚本通用数据
	LvTwo_Annotations( "建MF", m_bRefreshFlag );
	CreateMF();//建MF
	CreateEFS();//建MF下的所有直属EF文件并写记录
	CreateADFS();//建MF下的所有直属ADF文件
	//建MF/ADF下的EF文件并写记录(也就是MF/DDF0/ADF下的EF文件)
	vector<S_DDFINFO>::const_iterator iterSDDF = m_vecMFStruct.begin();
	while ( iterSDDF != m_vecMFStruct.end() )
	{
		if ( (*iterSDDF).m_strFID != "" )//DDF0的FID为空,没找到DDF0的话继续遍历
		{
			++iterSDDF;
			continue;
		}
		vector<S_ADFINFO>::const_iterator iterSADF = (*iterSDDF).m_vecADFInfo.begin();
		while ( iterSADF!=(*iterSDDF).m_vecADFInfo.end() )
		{
			if ( (*iterSADF).m_strFID == "" )//DF00的FID为空,DF00不是ADF目录，跳过DF00继续遍历
			{
				++iterSADF;
				continue;
			}
			CreateEFS( "", (*iterSADF).m_strFID );
			break;
		}
		break;
	}

	//遍历卡结构建DDF、ADF、EF
	vector<S_DDFINFO>::const_iterator iterDDF = m_vecMFStruct.begin();
	while ( iterDDF != m_vecMFStruct.end() )
	{
		if ( (*iterDDF).m_strFID == "" )//DDF0的FID为空,DDF0不是DDF目录，跳过DDF继续遍历
		{
			++iterDDF;
			continue;
		}
		CreateDDF( (*iterDDF).m_strDDFName );//建MF下的DDF
		CreateEFS( (*iterDDF).m_strDDFName );//建DDF下的所有直属EF文件并写记录
		CreateADFS( (*iterDDF).m_strDDFName );//建DDF下的所有直属ADF文件
		vector<S_ADFINFO>::const_iterator iterADF = (*iterDDF).m_vecADFInfo.begin();
		while ( iterADF!=(*iterDDF).m_vecADFInfo.end() )
		{
			if ( (*iterADF).m_strFID == "" )//DF00的FID为空,DF00不是ADF目录，跳过DF00继续遍历
			{
				++iterADF;
				continue;
			}
			CreateEFS( (*iterDDF).m_strDDFName, (*iterADF).m_strFID );
			++iterADF;
		}

		++iterDDF;
	}
	LineFeed(1);

	End_AllPreIssue();//预个人化结束
	if ( "1" == m_struLoadKeyInfo.m_strMode )//加密机方式装载密钥
		cCmd->CLOSEJMJ(m_bRefreshFlag);//关闭加密机
	cCmd->RESET(m_bRefreshFlag);//复位
	cCmd->ATR( 0, 17, _TEMP, m_bRefreshFlag );
	cCmd->RETURN_Value_Add( "CardATR", _TEMP, 0 );
	cCmd->RETURN_Value_Add( "CardSCICode", _CARDID, 0 );
	if ( "1" == m_struLoadKeyInfo.m_strMode )//加密机装载
		cCmd->RETURN( 1, m_bRefreshFlag );
	else if ( "2" == m_struLoadKeyInfo.m_strMode )//固定密钥装载
		cCmd->RETURN( 0, m_bRefreshFlag );
	
	return ( cInfo->Errors() == 0 ? false : true );
}


void CScript_T514::UpdateATR()
{
	string strATRHead,strCardSN;
	CITYCODE strCity;

	cCmd->SETNAD( "0", m_bRefreshFlag );//设置读写器卡座
	cCmd->RESET(m_bRefreshFlag);
	//先认证传输密钥
	LvOne_Annotations( "认证传输密钥" );
	cCmd->ATR( 11, 6, _ATR, m_bRefreshFlag );
	m_strTemp = "0000" + VAR_Name(_ATR);
	cCmd->VAR(  m_strTemp, _TEMP, m_bRefreshFlag );
	cCmd->TDES( VAR_Name(_TEMP), m_strTranKey, _TRANS_KEY, m_bRefreshFlag );
	cInst->Verify_TranCode( 8, VAR_Name(_TRANS_KEY), m_bRefreshFlag );

	//更新ATR
	strATRHead = "3B6D00000087BF10028649";
	cParameter->GetCityCode(strCity);
	strCardSN = strCity.m_strValue.substr(0,4)+m_strSearial;//城市代码+流水号
	cInst->Update_ATR(strATRHead,strCardSN,m_bRefreshFlag);

	LineFeed(1);
}
