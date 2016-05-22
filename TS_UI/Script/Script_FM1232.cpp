/************************************************************************/
/*							����оƬFM1232								*/
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
	while ( iterSDDF != m_vecMFStruct.end() )//����DDF
	{
		strTmp = (*iterSDDF).m_strDDFName;//Ӧ�û������
		transform(  strTmp.begin(), strTmp.end(), strTmp.begin(), toupper );
		if ( strTmp != strDDF )//��ΪӦ�û������ʱ��������
		{
			++iterSDDF;
			continue;
		}

		vector<S_ADFINFO>::const_iterator iterSADF = (*iterSDDF).m_vecADFInfo.begin();
		while ( iterSADF!=(*iterSDDF).m_vecADFInfo.end() )//����ADF
		{
			strTmp = (*iterSADF).m_strFID;
			transform(  strTmp.begin(), strTmp.end(), strTmp.begin(), toupper );
			if ( strTmp != strADF )//��ΪADF�ļ���ʶ���Ļ���������(��ȡ��������DF00��FIDΪ"")
			{
				++iterSADF;
				continue;
			}

			if ( strDDF == "DDF0" && strADF == "" )//��ʾ��MF�µ�����ֱ��EF�ļ�
			{
				LvTwo_Annotations( "����MF�µ�EF�ļ�" );
				LvOne_Annotations( "ѡ��MF" );
				cInst->Select_File( "MF", (*iterSDDF).m_strFID, (*iterSADF).m_strFID, "", m_bRefreshFlag );
			}
			else if ( strDDF != "DDF0" && strADF == "" )//��ʾ��MF/DDF�µ�����ֱ��EF�ļ�
			{
				m_strTemp = "���� MF-"+(*iterSDDF).m_strDDFName+" �µ�EF�ļ�";
				LvTwo_Annotations( m_strTemp );
				m_strTemp = "ѡ��"+(*iterSDDF).m_strDDFName;
				LvOne_Annotations( m_strTemp );
				cInst->Select_File( "MF", (*iterSDDF).m_strFID, (*iterSADF).m_strFID, "", m_bRefreshFlag );
			}
			else//��ʾ��MF/ADF�µ�����ֱ��EF�ļ� ���� ��MF/DDF/ADF�µ�����ֱ��EF�ļ�
			{
				if ( (*iterSDDF).m_strFID == "" )
					m_strTemp = "���� MF-"+(*iterSADF).m_strFID+" �µ�EF�ļ�";
				else
					m_strTemp = "���� MF-"+(*iterSDDF).m_strDDFName+"-"+(*iterSADF).m_strFID+" �µ�EF�ļ�";
				LvTwo_Annotations( m_strTemp );
				m_strTemp = "ѡ��"+(*iterSADF).m_strFID;
				LvOne_Annotations( m_strTemp );
				cInst->Select_File( "MF", (*iterSDDF).m_strFID, (*iterSADF).m_strFID, "", m_bRefreshFlag );

				//������ȫ�ļ�
				LvOne_Annotations( "������ȫ�ļ�" );
				strLen = LoadKeys( (*iterSDDF).m_strDDFName, (*iterSADF).m_strFID, strsRecords );//��ȡװ����Կ�ļ�¼����Կ�ļ��ռ�
				cInst->Create_Internal_EF( "SF", strLen, m_bRefreshFlag  );
				cInfo->Append_Script( strsRecords.str() );//������е�װ����Կ�ű�����

				//�ⲿ��֤ADF��������Կ
				m_strTemp = "�ⲿ��֤"+(*iterSADF).m_strFID+"��������Կ";
				LvOne_Annotations( m_strTemp, m_bRefreshFlag );
				cParameter->GetADFInfo( (*iterSDDF).m_strFID, (*iterSADF).m_strFID,  struADFInfo );//��ȡADF��Ϣ
				cCmd->VAR( struADFInfo.m_strMKvalue, _KEY, m_bRefreshFlag );//��ADF��MK���浽_KEY
				cInst->Get_Challenge( 8, m_bRefreshFlag );
				cCmd->RESP( 0, 8, _VAR10, m_bRefreshFlag );
				cCmd->TDES( VAR_Name(_VAR10), VAR_Name(_KEY), _VAR11, m_bRefreshFlag );
				cCmd->DES( VAR_Name(_VAR10), VAR_Name(_VAR11), _VAR12, m_bRefreshFlag );
				cInst->External_Authenticate( "00", VAR_Name(_VAR12), VAR_Name(_VAR10), 0, m_bRefreshFlag );//MK����Կ��ʶΪ00
				//����ȫ��PIN

				LineFeed(1);
			}

			vector<S_EFINFO>::const_iterator iterEF = (*iterSADF).m_vecEFInfo.begin();
			while ( iterEF!=(*iterSADF).m_vecEFInfo.end() )//����EF����EF�ļ�����ʼ���ļ�
			{
				//�ļ�����
				if ( "1" == (*iterEF).m_strEFType )
					strEFType = "�����ļ�";
				if ( "2" == (*iterEF).m_strEFType )
					strEFType = "�䳤�ļ�";
				if ( "3" == (*iterEF).m_strEFType )
					strEFType = "͸���ļ�";
				if ( "4" == (*iterEF).m_strEFType )
					strEFType = "ѭ���ļ�";
				if ( "5" == (*iterEF).m_strEFType )
					strEFType = "�ڲ��ļ�";
				m_strTemp = "���������ļ�"+(*iterEF).m_strSFI+" ����"+(*iterEF).m_strReadControl+" д��"+(*iterEF).m_strWriteControl+" �ļ����ͣ�"+strEFType;
				LvOne_Annotations( m_strTemp, m_bRefreshFlag );
				cInst->Create_File( "MF", (*iterSDDF).m_strDDFName, (*iterSADF).m_strFID, (*iterEF).m_strSFI, m_bRefreshFlag );//��EF�ļ�
				cInst->Select_File( "MF", (*iterSDDF).m_strFID, (*iterSADF).m_strFID, (*iterEF).m_strSFI, m_bRefreshFlag );//ѡ��ǰ����EF�ļ�
				//���ó�ʼ���ļ��ӿ�д��¼
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