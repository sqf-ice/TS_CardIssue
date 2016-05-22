/************************************************************************/
/*							����оƬT514								*/
/************************************************************************/
#include "Chip_Script.h"
#include "tool.h"
#include <algorithm>

#include <iostream>
using namespace std;
/*���˻��ű���������˵���ĺ궨��*/

CScript_T514::CScript_T514( int COS,  CInformation *cInfo ):CScript( COS, cInfo )
{
}

CScript_T514::~CScript_T514()
{
}


bool CScript_T514::IssueCardProcess()
{
	//����ָ������������ĳЩ״̬
	cCmd->Error_Check(true);//�򿪲������
	cCmd->Style_Set(STYLE_ASSIGNMENT);//�������ɽű��������ʽ
	cInst->Error_Check(true);//�򿪲������
	cInst->Style_Set(STYLE_SEPARATOR);//�������ɵĽű�ָ�����ʽ
	
	ScriptModeStatement();//�ű�ע��
	CON_Statement();//����ע��
	VAR_Statement();//����ע��
	PersonnalVAR_Statement();//���˻�����ע��
	if ( m_nCheckKeys == 1 )
	{
		LvTwo_Annotations( "�忨", m_bRefreshFlag );
		ClearCarad();
	}
	if ( m_nCheckATR == 1 )
	{
		LvTwo_Annotations( "����ATR", m_bRefreshFlag );
		UpdateATR();
	}

	GetAndSetParama( "5" );//��ȡ���������ݲ����ýű�ͨ������
	LvTwo_Annotations( "��MF", m_bRefreshFlag );
	CreateMF();//��MF
	CreateEFS();//��MF�µ�����ֱ��EF�ļ���д��¼
	CreateADFS();//��MF�µ�����ֱ��ADF�ļ�
	//��MF/ADF�µ�EF�ļ���д��¼(Ҳ����MF/DDF0/ADF�µ�EF�ļ�)
	vector<S_DDFINFO>::const_iterator iterSDDF = m_vecMFStruct.begin();
	while ( iterSDDF != m_vecMFStruct.end() )
	{
		if ( (*iterSDDF).m_strFID != "" )//DDF0��FIDΪ��,û�ҵ�DDF0�Ļ���������
		{
			++iterSDDF;
			continue;
		}
		vector<S_ADFINFO>::const_iterator iterSADF = (*iterSDDF).m_vecADFInfo.begin();
		while ( iterSADF!=(*iterSDDF).m_vecADFInfo.end() )
		{
			if ( (*iterSADF).m_strFID == "" )//DF00��FIDΪ��,DF00����ADFĿ¼������DF00��������
			{
				++iterSADF;
				continue;
			}
			CreateEFS( "", (*iterSADF).m_strFID );
			break;
		}
		break;
	}

	//�������ṹ��DDF��ADF��EF
	vector<S_DDFINFO>::const_iterator iterDDF = m_vecMFStruct.begin();
	while ( iterDDF != m_vecMFStruct.end() )
	{
		if ( (*iterDDF).m_strFID == "" )//DDF0��FIDΪ��,DDF0����DDFĿ¼������DDF��������
		{
			++iterDDF;
			continue;
		}
		CreateDDF( (*iterDDF).m_strDDFName );//��MF�µ�DDF
		CreateEFS( (*iterDDF).m_strDDFName );//��DDF�µ�����ֱ��EF�ļ���д��¼
		CreateADFS( (*iterDDF).m_strDDFName );//��DDF�µ�����ֱ��ADF�ļ�
		vector<S_ADFINFO>::const_iterator iterADF = (*iterDDF).m_vecADFInfo.begin();
		while ( iterADF!=(*iterDDF).m_vecADFInfo.end() )
		{
			if ( (*iterADF).m_strFID == "" )//DF00��FIDΪ��,DF00����ADFĿ¼������DF00��������
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

	End_AllPreIssue();//Ԥ���˻�����
	if ( "1" == m_struLoadKeyInfo.m_strMode )//���ܻ���ʽװ����Կ
		cCmd->CLOSEJMJ(m_bRefreshFlag);//�رռ��ܻ�
	cCmd->RESET(m_bRefreshFlag);//��λ
	cCmd->ATR( 0, 17, _TEMP, m_bRefreshFlag );
	cCmd->RETURN_Value_Add( "CardATR", _TEMP, 0 );
	cCmd->RETURN_Value_Add( "CardSCICode", _CARDID, 0 );
	if ( "1" == m_struLoadKeyInfo.m_strMode )//���ܻ�װ��
		cCmd->RETURN( 1, m_bRefreshFlag );
	else if ( "2" == m_struLoadKeyInfo.m_strMode )//�̶���Կװ��
		cCmd->RETURN( 0, m_bRefreshFlag );
	
	return ( cInfo->Errors() == 0 ? false : true );
}


void CScript_T514::UpdateATR()
{
	string strATRHead,strCardSN;
	CITYCODE strCity;

	cCmd->SETNAD( "0", m_bRefreshFlag );//���ö�д������
	cCmd->RESET(m_bRefreshFlag);
	//����֤������Կ
	LvOne_Annotations( "��֤������Կ" );
	cCmd->ATR( 11, 6, _ATR, m_bRefreshFlag );
	m_strTemp = "0000" + VAR_Name(_ATR);
	cCmd->VAR(  m_strTemp, _TEMP, m_bRefreshFlag );
	cCmd->TDES( VAR_Name(_TEMP), m_strTranKey, _TRANS_KEY, m_bRefreshFlag );
	cInst->Verify_TranCode( 8, VAR_Name(_TRANS_KEY), m_bRefreshFlag );

	//����ATR
	strATRHead = "3B6D00000087BF10028649";
	cParameter->GetCityCode(strCity);
	strCardSN = strCity.m_strValue.substr(0,4)+m_strSearial;//���д���+��ˮ��
	cInst->Update_ATR(strATRHead,strCardSN,m_bRefreshFlag);

	LineFeed(1);
}
