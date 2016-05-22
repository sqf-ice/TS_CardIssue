#include "Script_YK.h"
#include "Chip.h"
#include "tool.h"
#include <iomanip>
#include <algorithm>

CScript_YK::CScript_YK(CInformation *cInfo):cInfo(cInfo)
{
	//������ʵ����
	cParameter = new CParameter( cInfo );
	//������ʵ����
	cCmd = new CCommand( cInfo );
	//ָ����ʵ����,��Ϊ���ɸ��鿨�ű�û���õ��������麯��,���Բ��漰�õ��̳���Ľӿڣ����ô���ָ������ʵ����
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
		LvOne_Annotations( "���ö�д��SAM1�������ϵ縴λ", m_bRefreshFlag );
		cCmd->SETNAD( "2", m_bRefreshFlag );//���ö�д������
		cCmd->RESET( m_bRefreshFlag );//��λ
	}

	LvOne_Annotations( "���ö�д�����������ϵ縴λ", m_bRefreshFlag );
	cCmd->SETNAD( "0", m_bRefreshFlag );//���ö�д������
	cCmd->RESET( m_bRefreshFlag );//��λ
	LineFeed(1);
	LvOne_Annotations( "�����ⲿ��֤�Ĳ���", m_bRefreshFlag );
	LvOne_Annotations( "��ƬSN", m_bRefreshFlag );
	cCmd->ATR( 9, 8, _CARD_SN, m_bRefreshFlag );
	if ( nMode == 0 )//�̶���Կ 
	{}
	else if ( nMode == 1 )//���ܻ� 
	{
		LvOne_Annotations( "��ɢ����ȡATRӦ�������ֽ�����", m_bRefreshFlag );
		cCmd->ATR( 9, 8, _DIVFACTOR, m_bRefreshFlag );
	}
	else if ( nMode == 2 )//PSAM�� 
	{
		//ȡ���õĳ��д���
		cParameter->GetCityCode(struCity);
		strCityCode = struCity.m_strValue;
		LvOne_Annotations( "һ����ɢ����", m_bRefreshFlag );
		m_strTemp = GbkOrAscToHex(strCityCode.substr(0,2))+"303030307368";
		cCmd->VAR(m_strTemp,20,m_bRefreshFlag);
		LvOne_Annotations( "������ɢ����", m_bRefreshFlag );
		m_strTemp = GbkOrAscToHex(strCityCode)+"7378";
		cCmd->VAR(m_strTemp,21,m_bRefreshFlag);
		LvOne_Annotations( "������ɢ����", m_bRefreshFlag );
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
	//��ȡ��Ŀ�����Ϣ
	PROJINFO struProjInfo;
	cParameter->GetProjInfo( struProjInfo );
	m_sstrTemp.str("");
	m_sstrTemp<<";��Ŀ:"<<struProjInfo.m_strName<<endl;
	m_sstrTemp<<";˵��:"<<struProjInfo.m_strDesc<<endl;
	vector<PROJMOD>::const_iterator iterProj =struProjInfo.m_vecProjMod.begin();
	while(iterProj!=struProjInfo.m_vecProjMod.end())
	{
		m_sstrTemp<<";�� �ߣ�"<<(*iterProj).m_strAuthor<<endl;
		m_sstrTemp<<";�� �ģ�"<<(*iterProj).m_strModify<<endl;
		m_sstrTemp<<";�� �ڣ�"<<(*iterProj).m_strDate<<endl;
		m_sstrTemp<<";�� ����"<<(*iterProj).m_strVer<<endl<<endl;
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

	if ( 1 == nMode )//���ܻ���ʽ
	{
		LvOne_Annotations( "���Ӽ��ܻ�", m_bRefreshFlag );
		cCmd->CONNECTJMJ( CON_Name("CON2"), CON_Name("CON0"), CON_Name("CON1"), m_bRefreshFlag );//�����ܻ�
		LineFeed(1);
	}

	//��ȡ��Կ����Ȼ���ⲿ��֤��Կ
	vector<KEYS> vecKeys;
	cParameter->GetKEYS(2, vecKeys);//��ȡSSSE��������Կ
	vector<ADFKEY>::const_iterator iterKey = vecKeys.back().m_vecADFKEY.begin();
	while ( iterKey!=vecKeys.back().m_vecADFKEY.end() )
	{
		if ( (*iterKey).m_strADFName == "DF00" )
		{
			m_strTemp = "�ⲿ��֤ SSSE �µ���Կ";
			//��ȡSSSE��FID
			cParameter->GetDDFInfo("SSSE", struDDFInfo);
			strDF = struDDFInfo.m_strFID;
		}
		else
		{
			m_strTemp = "�ⲿ��֤ SSSE "+(*iterKey).m_strADFName+" �µ���Կ";
			strDF = (*iterKey).m_strADFName;//��Ϊ�ڼ��xmlʱ��ADF��FID�ͱ�ǩ������һ��,����KEYS�����Ӧ��ADF�ı�ǩ����ΪFID
		}
		LvTwo_Annotations( m_strTemp );
		vector<KEYDATAS>::const_iterator iterKeyDatas = (*iterKey).m_vecKeyDatas.begin();
		while ( iterKeyDatas!=(*iterKey).m_vecKeyDatas.end() )
		{
			//�ж���Կ���ƣ�STK��IRK��DLK��DPK��DTK��Щ��Կ�����ⲿ��֤����Կ
			strKeyName = (*iterKeyDatas).m_strName;
			transform(strKeyName.begin(),strKeyName.end(),strKeyName.begin(),toupper);
			if (strKeyName.find("STK")!=string::npos || strKeyName.find("IRK")!=string::npos || strKeyName.find("DLK")!=string::npos || 
				strKeyName.find("DPK")!=string::npos || strKeyName.find("DTK")!=string::npos)
			{
				++iterKeyDatas;
				continue;
			}

			//������Կ������ȡ�汾��֤��Կ
			strGroup = (*iterKeyDatas).m_strGroup;//�����ݹ̶������߷ָ�������������������һ�����߷ָ�������
			strKeyIndex = (*iterKeyDatas).m_strKeyIndex;
			strKeyVer = (*iterKeyDatas).m_strVer;
			strAddr = (*iterKeyDatas).m_strAddr;
			strPSAMID = (*iterKeyDatas).m_strPSAMKeyID;
			strPSAMType = (*iterKeyDatas).m_strPSAMKeyType;
			nPos = strGroup.find("|");
			nPos1 = strKeyIndex.find("|");
			nPos2 = strAddr.find("|");
			nPos3 = strPSAMID.find("|");
			while ( nPos != string::npos )//�ָ�����һ�����ڵģ���Ϊ�ڼ��XML��ʱ��ͼ����
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

				if ( nMode == 0 )//�̶���Կ 
				{
					m_strTemp = (*iterKeyDatas).m_strName + " ��Կ��ʶ:" + strKeyIndexTmp + " ��Կ�汾:" + strKeyVer;
					LvOne_Annotations( m_strTemp, m_bRefreshFlag );
					cCmd->GERNERALEXTERNALAUTH(strsMode.str(),strDF,"EF05",strKeyIndexTmp,strKeyVer,"00","00","00",(*iterKeyDatas).m_strValue,"",m_bRefreshFlag);
				}
				else if ( nMode == 1 )//���ܻ� 
				{
					m_strTemp = (*iterKeyDatas).m_strName + " ��Կ��ʶ:" + strKeyIndexTmp + " ��Կ�汾:" + strKeyVer + " ���ܻ���Կ��ַ:" + strAddrTmp;
					LvOne_Annotations( m_strTemp, m_bRefreshFlag );
					cCmd->GERNERALEXTERNALAUTH(strsMode.str(),strDF,"EF05",strKeyIndexTmp,strKeyVer,VAR_Name(_DIVFACTOR),"00",strAddrTmp,"00","",m_bRefreshFlag);
				}
				else if ( nMode == 2 )//PSAM�� 
				{
					m_strTemp = (*iterKeyDatas).m_strName + " ��Կ��ʶ:" + strKeyIndexTmp + " ��Կ�汾:" + strKeyVer + " PSAM����Կ��ʶ:" + strPSAMIDTmp + " PSAM����Կ��;:" + strPSAMType;
					LvOne_Annotations( m_strTemp, m_bRefreshFlag );
					//ȡPSAM����Կ��;��b8-b6�жϸ���Կ�ķ�ɢ���������Ծ���ʹ�÷�ɢ����
					nPSAMType = atoi(strPSAMType.substr(0,1).c_str())/2;
					if ( nPSAMType == 0 )
						cCmd->GERNERALEXTERNALAUTH(strsMode.str(),strDF,"EF05",strKeyIndexTmp,strKeyVer,"",strPSAMType,strPSAMIDTmp,"00","",m_bRefreshFlag);
					else if ( nPSAMType == 1 )
						cCmd->GERNERALEXTERNALAUTH(strsMode.str(),strDF,"EF05",strKeyIndexTmp,strKeyVer,VAR_Name(_DIVFACTOR1),strPSAMType,strPSAMIDTmp,"00","",m_bRefreshFlag);
					else if ( nPSAMType == 2 )
						cCmd->GERNERALEXTERNALAUTH(strsMode.str(),strDF,"EF05",strKeyIndexTmp,strKeyVer,VAR_Name(_DIVFACTOR2),strPSAMType,strPSAMIDTmp,"00","",m_bRefreshFlag);
					else if ( nPSAMType == 3 )
						cCmd->GERNERALEXTERNALAUTH(strsMode.str(),strDF,"EF05",strKeyIndexTmp,strKeyVer,VAR_Name(_DIVFACTOR3),strPSAMType,strPSAMIDTmp,"00","",m_bRefreshFlag);
					
					break;//��ΪPSAM����һ��ֻ�洢һ���汾����Կ������ֻ��ҪУ���һ���汾	
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
		LvOne_Annotations( "�رռ��ܻ�", m_bRefreshFlag );
		cCmd->CLOSEJMJ(m_bRefreshFlag);
	}
}

void CScript_YK::PIN_Certify()
{
	LvOne_Annotations( "У��PIN", m_bRefreshFlag );
	//ѡ��MF
	cInst->Select_File( "MF", "", "", "", m_bRefreshFlag );
	//ѡ��SSSE
	cInst->Select_File( "MF", "SSSE", "", "", m_bRefreshFlag );
	//У��PIN
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