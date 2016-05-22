#include "Script_YK.h"
#include "Chip.h"
#include "tool.h"
#include <iomanip>
#include <algorithm>

CScript_YK::CScript_YK(CInformation *cInfo):cInfo(cInfo)
{
	//参数类实例化
	cParameter = new CParameter( cInfo );
	//命令类实例化
	cCmd = new CCommand( cInfo );
	//指令类实例化,因为构成该验卡脚本没有用到其他纯虚函数,所以不涉及用到继承类的接口，故用大唐指令类来实例化
	cInst = new COS_T514( cInfo, cParameter );
	m_bRefreshFlag = true;
}


CScript_YK::~CScript_YK(void)
{
	delete cCmd;
	delete cParameter;
	delete cInst;
}

void CScript_YK::LineFeed( int nLineNum )
{
	for ( int i=0; i<nLineNum; ++i )
		cInfo->Append_Script("");
}

string CScript_YK::LvOne_Annotations( string strDatas, bool RefreshFlag /*= true */ )
{
	stringstream strsTemp;
	strsTemp.str("");
	strsTemp << ";------" << strDatas;
	if ( RefreshFlag )
		cInfo->Append_Script( strsTemp.str() );
	return strsTemp.str();
}

string CScript_YK::LvTwo_Annotations( string strDatas, bool RefreshFlag /*= true */ )
{
	stringstream strsTemp;
	int nWidth = (89-strDatas.length())/2;
	strsTemp.str("");
	strsTemp << ";" << setfill('=') << setw(nWidth) << right << " " << strDatas << " " <<  setfill('=') << setw(nWidth) << left << " " << endl;
	if ( RefreshFlag )
		cInfo->Append_Script( strsTemp.str() );
	return strsTemp.str();
}

void CScript_YK::GetAndSetParama(int nMode)
{	
	CITYCODE struCity;
	string strCityCode;

	if ( nMode == 2 )
	{
		LvOne_Annotations( "设置读写器SAM1卡座并上电复位", m_bRefreshFlag );
		cCmd->SETNAD( "2", m_bRefreshFlag );//设置读写器卡座
		cCmd->RESET( m_bRefreshFlag );//复位
	}

	LvOne_Annotations( "设置读写器主卡座并上电复位", m_bRefreshFlag );
	cCmd->SETNAD( "0", m_bRefreshFlag );//设置读写器卡座
	cCmd->RESET( m_bRefreshFlag );//复位
	LineFeed(1);
	LvOne_Annotations( "设置外部认证的参数", m_bRefreshFlag );
	LvOne_Annotations( "卡片SN", m_bRefreshFlag );
	cCmd->ATR( 9, 8, _CARD_SN, m_bRefreshFlag );
	if ( nMode == 0 )//固定密钥 
	{}
	else if ( nMode == 1 )//加密机 
	{
		LvOne_Annotations( "分散因子取ATR应答最后八字节内容", m_bRefreshFlag );
		cCmd->ATR( 9, 8, _DIVFACTOR, m_bRefreshFlag );
	}
	else if ( nMode == 2 )//PSAM卡 
	{
		//取配置的城市代码
		cParameter->GetCityCode(struCity);
		strCityCode = struCity.m_strValue;
		LvOne_Annotations( "一级分散因子", m_bRefreshFlag );
		m_strTemp = GbkOrAscToHex(strCityCode.substr(0,2))+"303030307368";
		cCmd->VAR(m_strTemp,20,m_bRefreshFlag);
		LvOne_Annotations( "二级分散因子", m_bRefreshFlag );
		m_strTemp = GbkOrAscToHex(strCityCode)+"7378";
		cCmd->VAR(m_strTemp,21,m_bRefreshFlag);
		LvOne_Annotations( "三级分散因子", m_bRefreshFlag );
		cCmd->ATR( 9, 8, _DIVFACTOR1, m_bRefreshFlag );
		m_strTemp = VAR_Name(_DIVFACTOR1) + VAR_Name(21);
		cCmd->VAR(m_strTemp,_DIVFACTOR2,m_bRefreshFlag);
		m_strTemp = VAR_Name(_DIVFACTOR1) + VAR_Name(21) + VAR_Name(20);
		cCmd->VAR(m_strTemp,_DIVFACTOR3,m_bRefreshFlag);
	}

	LineFeed(1);

}

void CScript_YK::ScriptModeStatement()
{
	//获取项目相关信息
	PROJINFO struProjInfo;
	cParameter->GetProjInfo( struProjInfo );
	m_sstrTemp.str("");
	m_sstrTemp<<";项目:"<<struProjInfo.m_strName<<endl;
	m_sstrTemp<<";说明:"<<struProjInfo.m_strDesc<<endl;
	vector<PROJMOD>::const_iterator iterProj =struProjInfo.m_vecProjMod.begin();
	while(iterProj!=struProjInfo.m_vecProjMod.end())
	{
		m_sstrTemp<<";作 者："<<(*iterProj).m_strAuthor<<endl;
		m_sstrTemp<<";修 改："<<(*iterProj).m_strModify<<endl;
		m_sstrTemp<<";日 期："<<(*iterProj).m_strDate<<endl;
		m_sstrTemp<<";版 本："<<(*iterProj).m_strVer<<endl<<endl;
		++iterProj;
	}
	cInfo->Append_Script(m_sstrTemp.str());
}

void CScript_YK::ExternalAuthentication(int nMode)
{
	int nPos,nPos1,nPos2,nPos3,nPSAMType;
	string strDF,strKeyName,strGroup,strKeyIndex,strKeyVer,strAddr,strPSAMID,strPSAMType;
	string strKeyIndexTmp,strPSAMIDTmp,strAddrTmp;
	stringstream strsMode;
	strsMode.str("");
	strsMode<<nMode;
	S_DDFINFO struDDFInfo;

	if ( 1 == nMode )//加密机方式
	{
		LvOne_Annotations( "连接加密机", m_bRefreshFlag );
		cCmd->CONNECTJMJ( CON_Name("CON2"), CON_Name("CON0"), CON_Name("CON1"), m_bRefreshFlag );//连加密机
		LineFeed(1);
	}

	//获取密钥数据然后外部认证密钥
	vector<KEYS> vecKeys;
	cParameter->GetKEYS(2, vecKeys);//获取SSSE的所有密钥
	vector<ADFKEY>::const_iterator iterKey = vecKeys.back().m_vecADFKEY.begin();
	while ( iterKey!=vecKeys.back().m_vecADFKEY.end() )
	{
		if ( (*iterKey).m_strADFName == "DF00" )
		{
			m_strTemp = "外部认证 SSSE 下的密钥";
			//获取SSSE的FID
			cParameter->GetDDFInfo("SSSE", struDDFInfo);
			strDF = struDDFInfo.m_strFID;
		}
		else
		{
			m_strTemp = "外部认证 SSSE "+(*iterKey).m_strADFName+" 下的密钥";
			strDF = (*iterKey).m_strADFName;//因为在检测xml时候ADF的FID和标签名必须一致,所以KEYS里面对应的ADF的标签名即为FID
		}
		LvTwo_Annotations( m_strTemp );
		vector<KEYDATAS>::const_iterator iterKeyDatas = (*iterKey).m_vecKeyDatas.begin();
		while ( iterKeyDatas!=(*iterKey).m_vecKeyDatas.end() )
		{
			//判断密钥名称，STK、IRK、DLK、DPK、DTK这些密钥不是外部认证的密钥
			strKeyName = (*iterKeyDatas).m_strName;
			transform(strKeyName.begin(),strKeyName.end(),strKeyName.begin(),toupper);
			if (strKeyName.find("STK")!=string::npos || strKeyName.find("IRK")!=string::npos || strKeyName.find("DLK")!=string::npos || 
				strKeyName.find("DPK")!=string::npos || strKeyName.find("DTK")!=string::npos)
			{
				++iterKeyDatas;
				continue;
			}

			//根据密钥组数获取版本认证密钥
			strGroup = (*iterKeyDatas).m_strGroup;//该数据固定以竖线分隔符结束，所以至少有一个竖线分隔符存在
			strKeyIndex = (*iterKeyDatas).m_strKeyIndex;
			strKeyVer = (*iterKeyDatas).m_strVer;
			strAddr = (*iterKeyDatas).m_strAddr;
			strPSAMID = (*iterKeyDatas).m_strPSAMKeyID;
			strPSAMType = (*iterKeyDatas).m_strPSAMKeyType;
			nPos = strGroup.find("|");
			nPos1 = strKeyIndex.find("|");
			nPos2 = strAddr.find("|");
			nPos3 = strPSAMID.find("|");
			while ( nPos != string::npos )//分隔符是一定存在的，因为在检测XML的时候就检测了
			{
				if ( nPos1 == string::npos )
					strKeyIndexTmp = strKeyIndex;
				else
				{
					strKeyIndexTmp = strKeyIndex.substr( 0, nPos1 );
					strKeyIndex=strKeyIndex.substr(nPos1+1);
					nPos1 = strKeyIndex.find("|");
				}
				if ( nPos2 == string::npos )
					strAddrTmp = strAddr;
				else
				{
					strAddrTmp = strAddr.substr(0,nPos2);
					strAddr=strAddr.substr(nPos2+1);
					nPos2 = strAddr.find("|");
				}
				if ( nPos3 == string::npos )
					strPSAMIDTmp = strPSAMID;
				else
				{
					strPSAMIDTmp = strPSAMID.substr( 0, nPos3 );
					strPSAMID=strPSAMID.substr(nPos3+1);
					nPos3 = strPSAMID.find("|");
				}

				if ( nMode == 0 )//固定密钥 
				{
					m_strTemp = (*iterKeyDatas).m_strName + " 密钥标识:" + strKeyIndexTmp + " 密钥版本:" + strKeyVer;
					LvOne_Annotations( m_strTemp, m_bRefreshFlag );
					cCmd->GERNERALEXTERNALAUTH(strsMode.str(),strDF,"EF05",strKeyIndexTmp,strKeyVer,"00","00","00",(*iterKeyDatas).m_strValue,"",m_bRefreshFlag);
				}
				else if ( nMode == 1 )//加密机 
				{
					m_strTemp = (*iterKeyDatas).m_strName + " 密钥标识:" + strKeyIndexTmp + " 密钥版本:" + strKeyVer + " 加密机密钥地址:" + strAddrTmp;
					LvOne_Annotations( m_strTemp, m_bRefreshFlag );
					cCmd->GERNERALEXTERNALAUTH(strsMode.str(),strDF,"EF05",strKeyIndexTmp,strKeyVer,VAR_Name(_DIVFACTOR),"00",strAddrTmp,"00","",m_bRefreshFlag);
				}
				else if ( nMode == 2 )//PSAM卡 
				{
					m_strTemp = (*iterKeyDatas).m_strName + " 密钥标识:" + strKeyIndexTmp + " 密钥版本:" + strKeyVer + " PSAM卡密钥标识:" + strPSAMIDTmp + " PSAM卡密钥用途:" + strPSAMType;
					LvOne_Annotations( m_strTemp, m_bRefreshFlag );
					//取PSAM卡密钥用途的b8-b6判断该密钥的分散级数，用以决定使用分散因子
					nPSAMType = atoi(strPSAMType.substr(0,1).c_str())/2;
					if ( nPSAMType == 0 )
						cCmd->GERNERALEXTERNALAUTH(strsMode.str(),strDF,"EF05",strKeyIndexTmp,strKeyVer,"",strPSAMType,strPSAMIDTmp,"00","",m_bRefreshFlag);
					else if ( nPSAMType == 1 )
						cCmd->GERNERALEXTERNALAUTH(strsMode.str(),strDF,"EF05",strKeyIndexTmp,strKeyVer,VAR_Name(_DIVFACTOR1),strPSAMType,strPSAMIDTmp,"00","",m_bRefreshFlag);
					else if ( nPSAMType == 2 )
						cCmd->GERNERALEXTERNALAUTH(strsMode.str(),strDF,"EF05",strKeyIndexTmp,strKeyVer,VAR_Name(_DIVFACTOR2),strPSAMType,strPSAMIDTmp,"00","",m_bRefreshFlag);
					else if ( nPSAMType == 3 )
						cCmd->GERNERALEXTERNALAUTH(strsMode.str(),strDF,"EF05",strKeyIndexTmp,strKeyVer,VAR_Name(_DIVFACTOR3),strPSAMType,strPSAMIDTmp,"00","",m_bRefreshFlag);
					
					break;//因为PSAM卡内一般只存储一个版本的密钥，所以只需要校验第一个版本	
				}

				strKeyVer = DecToHex( HexToDec(strKeyVer)+1 );
				strGroup=strGroup.substr(nPos+1);
				nPos = strGroup.find("|");
			}

			++iterKeyDatas;
			LineFeed(1);
		}
		LineFeed(1);
		++iterKey;
	}

	if ( 1 == nMode )
	{
		LvOne_Annotations( "关闭加密机", m_bRefreshFlag );
		cCmd->CLOSEJMJ(m_bRefreshFlag);
	}
}

void CScript_YK::PIN_Certify()
{
	LvOne_Annotations( "校验PIN", m_bRefreshFlag );
	//选择MF
	cInst->Select_File( "MF", "", "", "", m_bRefreshFlag );
	//选择SSSE
	cInst->Select_File( "MF", "SSSE", "", "", m_bRefreshFlag );
	//校验PIN
	cInst->PIN_Certify( m_bRefreshFlag );
	LineFeed(1);
}

bool CScript_YK::IssueCardProcess(int nMode)
{
	ScriptModeStatement();
	GetAndSetParama(nMode);
	PIN_Certify();
	ExternalAuthentication(nMode);

	return ( cInfo->Errors() == 0 ? false : true );
}