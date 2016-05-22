/************************************************************************/
/*							复旦芯片FM1232								*/
/************************************************************************/
#include "Chip_Script.h"
#include <algorithm>
#include "tool.h"

CScript_FM1232::CScript_FM1232( int COS,  CInformation *cInfo ):CScript( COS, cInfo )
{
}

CScript_FM1232::~CScript_FM1232()
{
}


bool CScript_FM1232::IssueCardProcess()
{
	return true;
}

void CScript_FM1232::CreateEFS( string strDDF /*= ""*/, string strADF /*= "" */ )
{
	stringstream strsRecords;
	string strLen,strTmp,strDDFFID,strEFType;
	S_ADFINFO struADFInfo;

	transform( strDDF.begin(), strDDF.end(), strDDF.begin(), toupper );
	transform( strADF.begin(), strADF.end(), strADF.begin(), toupper );
	if ( strDDF == "" )
		strDDF = "DDF0";

	vector<S_DDFINFO>::const_iterator iterSDDF = m_vecMFStruct.begin();
	while ( iterSDDF != m_vecMFStruct.end() )//遍历DDF
	{
		strTmp = (*iterSDDF).m_strDDFName;//应用环境简称
		transform(  strTmp.begin(), strTmp.end(), strTmp.begin(), toupper );
		if ( strTmp != strDDF )//不为应用环境简称时继续遍历
		{
			++iterSDDF;
			continue;
		}

		vector<S_ADFINFO>::const_iterator iterSADF = (*iterSDDF).m_vecADFInfo.begin();
		while ( iterSADF!=(*iterSDDF).m_vecADFInfo.end() )//遍历ADF
		{
			strTmp = (*iterSADF).m_strFID;
			transform(  strTmp.begin(), strTmp.end(), strTmp.begin(), toupper );
			if ( strTmp != strADF )//不为ADF文件标识符的话继续遍历(读取的数据中DF00的FID为"")
			{
				++iterSADF;
				continue;
			}

			if ( strDDF == "DDF0" && strADF == "" )//表示建MF下的所有直属EF文件
			{
				LvTwo_Annotations( "创建MF下的EF文件" );
				LvOne_Annotations( "选择MF" );
				cInst->Select_File( "MF", (*iterSDDF).m_strFID, (*iterSADF).m_strFID, "", m_bRefreshFlag );
			}
			else if ( strDDF != "DDF0" && strADF == "" )//表示建MF/DDF下的所有直属EF文件
			{
				m_strTemp = "创建 MF-"+(*iterSDDF).m_strDDFName+" 下的EF文件";
				LvTwo_Annotations( m_strTemp );
				m_strTemp = "选择"+(*iterSDDF).m_strDDFName;
				LvOne_Annotations( m_strTemp );
				cInst->Select_File( "MF", (*iterSDDF).m_strFID, (*iterSADF).m_strFID, "", m_bRefreshFlag );
			}
			else//表示建MF/ADF下的所有直属EF文件 或者 建MF/DDF/ADF下的所有直属EF文件
			{
				if ( (*iterSDDF).m_strFID == "" )
					m_strTemp = "创建 MF-"+(*iterSADF).m_strFID+" 下的EF文件";
				else
					m_strTemp = "创建 MF-"+(*iterSDDF).m_strDDFName+"-"+(*iterSADF).m_strFID+" 下的EF文件";
				LvTwo_Annotations( m_strTemp );
				m_strTemp = "选择"+(*iterSADF).m_strFID;
				LvOne_Annotations( m_strTemp );
				cInst->Select_File( "MF", (*iterSDDF).m_strFID, (*iterSADF).m_strFID, "", m_bRefreshFlag );

				//创建安全文件
				LvOne_Annotations( "创建安全文件" );
				strLen = LoadKeys( (*iterSDDF).m_strDDFName, (*iterSADF).m_strFID, strsRecords );//获取装载密钥的记录和密钥文件空间
				cInst->Create_Internal_EF( "SF", strLen, m_bRefreshFlag  );
				cInfo->Append_Script( strsRecords.str() );//添加所有的装载密钥脚本数据

				//外部认证ADF的主控密钥
				m_strTemp = "外部认证"+(*iterSADF).m_strFID+"的主控密钥";
				LvOne_Annotations( m_strTemp, m_bRefreshFlag );
				cParameter->GetADFInfo( (*iterSDDF).m_strFID, (*iterSADF).m_strFID,  struADFInfo );//获取ADF信息
				cCmd->VAR( struADFInfo.m_strMKvalue, _KEY, m_bRefreshFlag );//将ADF的MK保存到_KEY
				cInst->Get_Challenge( 8, m_bRefreshFlag );
				cCmd->RESP( 0, 8, _VAR10, m_bRefreshFlag );
				cCmd->TDES( VAR_Name(_VAR10), VAR_Name(_KEY), _VAR11, m_bRefreshFlag );
				cCmd->DES( VAR_Name(_VAR10), VAR_Name(_VAR11), _VAR12, m_bRefreshFlag );
				cInst->External_Authenticate( "00", VAR_Name(_VAR12), VAR_Name(_VAR10), 0, m_bRefreshFlag );//MK的密钥标识为00
				//引用全局PIN

				LineFeed(1);
			}

			vector<S_EFINFO>::const_iterator iterEF = (*iterSADF).m_vecEFInfo.begin();
			while ( iterEF!=(*iterSADF).m_vecEFInfo.end() )//遍历EF，建EF文件并初始化文件
			{
				//文件类型
				if ( "1" == (*iterEF).m_strEFType )
					strEFType = "定长文件";
				if ( "2" == (*iterEF).m_strEFType )
					strEFType = "变长文件";
				if ( "3" == (*iterEF).m_strEFType )
					strEFType = "透明文件";
				if ( "4" == (*iterEF).m_strEFType )
					strEFType = "循环文件";
				if ( "5" == (*iterEF).m_strEFType )
					strEFType = "内部文件";
				m_strTemp = "建立基本文件"+(*iterEF).m_strSFI+" 读："+(*iterEF).m_strReadControl+" 写："+(*iterEF).m_strWriteControl+" 文件类型："+strEFType;
				LvOne_Annotations( m_strTemp, m_bRefreshFlag );
				cInst->Create_File( "MF", (*iterSDDF).m_strDDFName, (*iterSADF).m_strFID, (*iterEF).m_strSFI, m_bRefreshFlag );//建EF文件
				cInst->Select_File( "MF", (*iterSDDF).m_strFID, (*iterSADF).m_strFID, (*iterEF).m_strSFI, m_bRefreshFlag );//选当前建的EF文件
				//调用初始化文件接口写记录
				InitFile( (*iterSDDF).m_strFID, (*iterSADF).m_strFID, (*iterEF).m_strSFI );
				LineFeed(1);

				++iterEF;
			}

			break;
		}	

		break;
	}
}


void CScript_FM1232::UpdateATR()
{

}