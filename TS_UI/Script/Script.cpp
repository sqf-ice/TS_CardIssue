
#include "Script.h"
#include "Chip.h"
#include "tool.h"
#include <algorithm>
#include <iomanip>

CScript::CScript( int COS, CInformation *cInfo ):cInfo(cInfo)
{	
	//������ʵ����
	cParameter = new CParameter( cInfo );
	//������ʵ����
	cCmd = new CCommand( cInfo );
	//ָ����ʵ����
	switch ( COS )
	{
	case COSID_T514:
		cInst = new COS_T514( cInfo, cParameter );
		break;
	default:
		cInst = NULL;
		cInfo->Append_Error("�޷�ʶ���COS��־");
		break;
	}
	m_bRefreshFlag = true;
}

CScript::~CScript()
{
	delete cCmd;
	delete cParameter;
	delete cInst;
}

void CScript::LineFeed( int nLineNum )
{
	for ( int i=0; i<nLineNum; ++i )
		cInfo->Append_Script("");
}

string CScript::LvOne_Annotations( string strDatas, bool RefreshFlag /*= true */ )
{
	stringstream strsTemp;
	strsTemp.str("");
	strsTemp << ";------" << strDatas;
	if ( RefreshFlag )
		cInfo->Append_Script( strsTemp.str() );
	return strsTemp.str();
}

string CScript::LvTwo_Annotations( string strDatas, bool RefreshFlag /*= true */ )
{
	stringstream strsTemp;
	int nWidth = (89-strDatas.length())/2;
	strsTemp.str("");
	strsTemp << ";" << setfill('=') << setw(nWidth) << right << " " << strDatas << " " <<  setfill('=') << setw(nWidth) << left << " " << endl;
	if ( RefreshFlag )
		cInfo->Append_Script( strsTemp.str() );
	return strsTemp.str();
}

void CScript::GetAndSetParama( string strCOS )
{
	cParameter->GetLoadKeyInfo( m_struLoadKeyInfo );//��ȡװ����Կ�����Ϣ
	cParameter->GetCardStruct( m_vecMFStruct );//��ȡ��Ƭ�ṹ
	cParameter->GetMFInfo( m_struMFInfo );//��ȡMF��Ϣ

	LvOne_Annotations( "���ö�д���������ϵ縴λ" );
	cCmd->SETNAD( "0", m_bRefreshFlag );//���ö�д������
	cCmd->RESET( m_bRefreshFlag );//��λ

	LvOne_Annotations( "��ƬSN" );
	cCmd->ATR( 9, 8, _CARD_SN, m_bRefreshFlag );//��Ϊ������Կ��ɢ����

	//���硢ͬ�����֧�����ķ�ʽװ����Կ
	if ( strCOS == "3" || strCOS == "4" )
	{
		if ( m_struLoadKeyInfo.m_strType == "0" )//����
			m_strLoadKeyMode = "1";//�ĳ�����
		else if ( m_struLoadKeyInfo.m_strType == "10" )//���ĸ���
			m_strLoadKeyMode = "11";//�ĳ����ĸ���
	}
	else
		m_strLoadKeyMode = m_struLoadKeyInfo.m_strType;

	if ( "1" == m_struLoadKeyInfo.m_strMode )//���ܻ���ʽװ����Կ
	{
		LvOne_Annotations( "���Ӽ��ܻ�������װ����Կ�Ĳ���" );
		cCmd->CONNECTJMJ( CON_Name("CON2"), CON_Name("CON0"), CON_Name("CON1"), m_bRefreshFlag );//�����ܻ�
		cCmd->VAR( strCOS, _COSMANU, m_bRefreshFlag);//COS���� 0������1��������3�����磬4��ͬ����5������ �������䣩'�ݲ��� 6: ���, 7 ����
		cCmd->VAR( m_strLoadKeyMode, _STOWAGE, m_bRefreshFlag );//װ�ط�ʽ 0�����ģ�1�����ģ����󡢵���������֧������ֱ��װ�ض����硢ͬ�����֧�����ķ�ʽ��  10���ĸ���  11: ���ĸ���,    '�ݲ���
		cCmd->VAR( m_struLoadKeyInfo.m_strPKeyNo, _PKEYNO, m_bRefreshFlag );//16���Ƹ�ʽ������ԿID
		cCmd->VAR( "1", _KEYFSJS, m_bRefreshFlag );//16���Ƹ�ʽ������Կ��ɢ����
		cCmd->VAR( m_struLoadKeyInfo.m_strPKeyfsjs, _PKEYFSJS, m_bRefreshFlag );//16���Ƹ�ʽ������Կ��������Կ������
		cCmd->ATR( 9, 8, _KEYFSYZ, m_bRefreshFlag );//������Կ��ɢ����,ֵΪATRӦ���е������ֽ�����
		cCmd->VAR( m_struLoadKeyInfo.m_strPKeyfsyz, _PKEYFSYZ, m_bRefreshFlag );//������Կ��������Կ����ɢ����
	}
	LineFeed(1);

}

void CScript::ExchangeDatas( int nCheckKeys, int nCheckATR, string strSearial, string strTranKey, vector<CLEARCARDPARAM> vecQCParam )
{
	m_nCheckKeys = nCheckKeys;
	m_nCheckATR = nCheckATR;
	m_strSearial = strSearial;
	m_strTranKey = strTranKey;
	m_vecCLCParam = vecQCParam;
}

void CScript::ScriptModeStatement()
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
	m_sstrTemp<<";1. ���ݿ��Ӧ�ֶ��б�"<<endl<<endl;
	cInfo->Append_Script(m_sstrTemp.str());
}

void CScript::CON_Statement()
{
	m_sstrTemp.str("");

	//��ȡ�ű�˵����CON����˵����������
	vector<CONDECLAR> vecCONDeclar;
	string strTemp;
	int nLen;
	cParameter->GetCONStatement( vecCONDeclar );
	m_sstrTemp<<";2. ��Ӧ��ϵ"<<endl<<endl;
	m_sstrTemp<<";�� ��"<<"      �û��ֶ�"<<"      ��������"<<"                                              �����ļ�"<<"            ����"<<"      ����"<<endl;
	vector<CONDECLAR>::const_iterator iterCON = vecCONDeclar.begin();
	while ( iterCON!=vecCONDeclar.end() )
	{
		strTemp = ";"+(*iterCON).m_strCON;
		nLen = 12-strTemp.length();//���ڵ����ո���
		nLen>0 ? strTemp.append(nLen, ' ') : strTemp.append(1, ' ');
		strTemp += (*iterCON).m_strUserFields;
		nLen = 26-strTemp.length();
		nLen>0 ? strTemp.append(nLen, ' ') : strTemp.append(1, ' ');
		strTemp += (*iterCON).m_strName;
		nLen = 80-strTemp.length();
		nLen>0 ? strTemp.append(nLen, ' ') : strTemp.append(1, ' ');
		strTemp += (*iterCON).m_strParentDir;
		nLen = 100-strTemp.length();
		nLen>0 ? strTemp.append(nLen, ' ') : strTemp.append(1, ' ');
		strTemp += (*iterCON).m_strType;
		nLen = 110-strTemp.length();
		nLen>0 ? strTemp.append(nLen, ' ') : strTemp.append(1, ' ');
		m_sstrTemp<<strTemp<<(*iterCON).m_strLen<<endl;
		
		++iterCON;
	}
	m_sstrTemp<<endl;
	cInfo->Append_Script(m_sstrTemp.str());
}

void CScript::VAR_Statement()
{
	m_sstrTemp.str("");

	m_sstrTemp<<";3. ����˵��"<<endl<<endl;
	m_sstrTemp<<";VAR"<<_KEY<<"   ������Կ"<<endl;
	m_sstrTemp<<";VAR"<<_RECNO<<"   ��¼��"<<endl;
	m_sstrTemp<<";VAR"<<_CARD_SN<<"   ��ƬSN"<<endl;
	m_sstrTemp<<";VAR"<<_COSMANU<<"   COS����"<<endl;
	m_sstrTemp<<";VAR"<<_TRANS_KEY<<"   ��֤������Կ"<<endl;
	m_sstrTemp<<";VAR"<<_STOWAGE<<"   LOADKEYװ�ط�ʽ"<<endl;
	m_sstrTemp<<";VAR"<<_PKEYNO<<"   LOADKEY������ԿID"<<endl;
	m_sstrTemp<<";VAR"<<_KEYFSJS<<"   LOADKEY������Կ��ɢ����"<<endl;
	m_sstrTemp<<";VAR"<<_PKEYFSJS<<"   LOADKEY������Կ��������Կ����ɢ����"<<endl;
	m_sstrTemp<<";VAR"<<_KEYFSYZ<<"   LOADKEY������Կ��ɢ����"<<endl;
	m_sstrTemp<<";VAR"<<_VAR10<<"  ��֤��Կʱ���������"<<endl;
	m_sstrTemp<<";VAR"<<_VAR11<<"  ��֤��Կʱ���������Կ"<<endl;
	m_sstrTemp<<";VAR"<<_VAR12<<"  ��֤��Կʱ���������Կ"<<endl;
	m_sstrTemp<<";VAR"<<_PKEYFSYZ<<"  LOADKEY������Կ��������Կ����ɢ����"<<endl;
	m_sstrTemp<<";VAR"<<_ATR<<"  ���ڴ洢��λ��Ϣ"<<endl;
	m_sstrTemp<<";VAR"<<_MAC<<"  ���ڴ洢�����macֵ"<<endl;
	m_sstrTemp<<";VAR"<<_CARDID<<"  ���ڴ洢����ʶ����"<<endl;
	m_sstrTemp<<";VAR"<<_TEMP<<"  ���ڴ洢��ʱ����"<<endl;
	m_sstrTemp<<";VAR30-VAR40Ԥ����MAC�����ĵȼ������ʱ����"<<endl;
	m_sstrTemp<<endl;
	m_sstrTemp<<";CON0:���ܻ� IP��ַ:"<<endl;
	m_sstrTemp<<";CON1:���ܻ� �˿�:6666"<<endl;
	m_sstrTemp<<";CON2:���ܻ� ����"<<endl;
	cInfo->Append_Script(m_sstrTemp.str());
}

void CScript::PersonnalVAR_Statement()
{
	LineFeed(1);
}

void CScript::ClearCarad()
{
	string strTmp;
	cCmd->BEGINGO(m_bRefreshFlag);
	cCmd->SETNAD( "0", m_bRefreshFlag );//���ö�д������
	cCmd->RESET(m_bRefreshFlag);
	
	//�����Ŀ¼
	vector<CLEARCARDPARAM>::const_iterator iter = m_vecCLCParam.begin();
	while ( iter != m_vecCLCParam.end() )
	{
		if ( (*iter).m_strDirLv == "����Ŀ¼" )
		{
			strTmp = "���"+(*iter).m_strFID;
			LvOne_Annotations( strTmp, m_bRefreshFlag );
			cInst->Select_File( "MF", "", "", "", m_bRefreshFlag );
			cInst->Select_File( true, false, (*iter).m_strAID.length()/2, (*iter).m_strAID, m_bRefreshFlag );
			cCmd->VAR( (*iter).m_strMK, _KEY, m_bRefreshFlag );
			cInst->Get_Challenge( 8, m_bRefreshFlag );
			m_sstrTemp.str("");
			m_sstrTemp << "����ֵ,���浽 VAR" << _VAR10;
			LvOne_Annotations( m_sstrTemp.str(), m_bRefreshFlag );
			cCmd->RESP( 0, 8, _VAR10, m_bRefreshFlag );
			if ( (*iter).m_strAE ==  "�籣Ӧ��" )
			{
				m_sstrTemp.str("");
				m_sstrTemp << "������Կ���浽VAR" << _VAR11 << ",VAR" << _VAR12;
				LvOne_Annotations( m_sstrTemp.str(), m_bRefreshFlag );
				cCmd->TDES( VAR_Name(_VAR10), VAR_Name(_KEY), _VAR11, m_bRefreshFlag );
				cCmd->DES( VAR_Name(_VAR10), VAR_Name(_VAR11), _VAR12, m_bRefreshFlag );
				cInst->External_Authenticate( "00", VAR_Name(_VAR12), VAR_Name(_VAR10), 0, m_bRefreshFlag );
			}
			else if ( (*iter).m_strAE ==  "���籣Ӧ��" )
			{
				m_sstrTemp.str("");
				m_sstrTemp << "������Կ���浽VAR" << _VAR11;
				LvOne_Annotations( m_sstrTemp.str(), m_bRefreshFlag );
				cCmd->TDES( VAR_Name(_VAR10), VAR_Name(_KEY), _VAR11, m_bRefreshFlag );
				cInst->External_Authenticate( "00", VAR_Name(_VAR11), "", 0, m_bRefreshFlag );
			}
			cInst->Delete_File( 1, "DDF", (*iter).m_strFID, m_bRefreshFlag );
			LineFeed(1);
		}
		++iter;
	}

	//��һ��Ŀ¼ ��MF
	vector<CLEARCARDPARAM>::const_iterator iter1 = m_vecCLCParam.begin();
	while ( iter1 != m_vecCLCParam.end() )
	{
		if ( (*iter1).m_strDirLv == "һ��Ŀ¼" )
		{
			//ѡ��MF���ⲿ��֤��Կ�����MF
			LvOne_Annotations( "���MF", m_bRefreshFlag );
			cCmd->VAR( (*iter1).m_strMK, _KEY, m_bRefreshFlag );
			cInst->Select_File( "MF", "", "", "", m_bRefreshFlag );
			cInst->Get_Challenge( 8, m_bRefreshFlag );
			m_sstrTemp.str("");
			m_sstrTemp << "����ֵ,���浽 VAR" << _VAR10;
			LvOne_Annotations( m_sstrTemp.str(), m_bRefreshFlag );
			cCmd->RESP( 0, 8, _VAR10, m_bRefreshFlag );
			m_sstrTemp.str("");
			m_sstrTemp << "������Կ���浽VAR" << _VAR11 << ",VAR" << _VAR12;
			LvOne_Annotations( m_sstrTemp.str(), m_bRefreshFlag );
			cCmd->TDES( VAR_Name(_VAR10), VAR_Name(_KEY), _VAR11, m_bRefreshFlag );
			cCmd->DES( VAR_Name(_VAR10), VAR_Name(_VAR11), _VAR12, m_bRefreshFlag );
			cInst->External_Authenticate( "00", VAR_Name(_VAR12), VAR_Name(_VAR10), 0, m_bRefreshFlag );
			cInst->Delete_File( 1, "MF", (*iter1).m_strFID, m_bRefreshFlag );
			break;
		}
		++iter1;
	}

	cCmd->FINALGO(m_bRefreshFlag);
	LineFeed(1);
}

bool CScript::InitFile(string DDF, string ADF, string EF)
{
	S_EFINFO efInfo;
	EFDATAS efData;
	string strDDFTmp,strEFTmp,strP1;

	cInst->ReSetErrFlag();
	if (!cParameter->GetEFInfo(DDF, ADF, EF, efInfo))
	{
		m_sstrTemp.clear();
		m_sstrTemp.str("");
		m_sstrTemp << "�ļ���ʼ�� InitFile(" << DDF << ", " << ADF << ", " << EF << ")���Ҳ���ָ��·����" << endl;
		cInfo->Append_Error(m_sstrTemp.str());
		return false;
	}

	strDDFTmp = DDF;
	strEFTmp = EF;
	transform( DDF.begin(), DDF.end(), DDF.begin(), toupper );
	transform( EF.begin(), EF.end(), EF.begin(), toupper );
	if ( strDDFTmp == "SSSE" && ADF == "" && strEFTmp == "EF05" )//���ڽ�SSSE�µ�EF05�ļ�ʱ����Ҫ�õ�����ʶ����
		GetCardID();

	//�ڲ��ļ�
	if (efInfo.m_strEFType == "5")
	{
		m_sstrTemp.clear();
		m_sstrTemp.str("");
		m_sstrTemp << "�ļ���ʼ�� InitFile(" << DDF << ", " << ADF << ", " << EF << ")��ָ���ļ�Ϊ�ڲ��ļ���" << endl;
		cInfo->Append_Error(m_sstrTemp.str());
		return false;
	}
	//ѭ���ļ�
	if (efInfo.m_strEFType == "4")
	{	}
	//��¼�ļ�
	else if (efInfo.m_strEFType == "1" || efInfo.m_strEFType == "2")
	{
		for (int i = 1; i <= efInfo.m_nRecordCount; ++i)
		{
			if (!cParameter->GetEFData(DDF, ADF, EF, i, efData))
			{
				m_sstrTemp.clear();
				m_sstrTemp.str("");
				m_sstrTemp << "�ļ���ʼ�� InitFile(" << DDF << ", " << ADF << ", " << EF << ")���Ҳ���ָ����¼" << i << "��" << endl;
				cInfo->Append_Error(m_sstrTemp.str());
				return false;				
			}
			if ( efData.m_strRemark != "" )//��ע�����Ϊ�յĻ������ע��
			{
				m_strTemp = "д"+efData.m_strRemark;
				LvOne_Annotations(m_strTemp);
			}
			cInst->Update_Record("MF", DDF, ADF, EF, i, true);//��ʼ������
			if ( strDDFTmp == "SSSE" && ADF == "" && strEFTmp == "EF05" && efData.m_strTag == "01" )//�����дSSSE�µ�EF05��"����ʶ����"������¼�Ļ���Ҫ���⴦��
			{
				strP1 = DecToHex( i );
				m_strTemp = "0110" + VAR_Name(_CARDID);//����ʶ����ļ�¼����
				cInst->Update_Record( strP1, "04", 18, m_strTemp, m_bRefreshFlag);//����ʶ����̶����ݳ���
				continue;
			}
			if (((efData.m_strrType == "an" || efData.m_strrType == "b") && !StrTrim(efData.m_strVal, "0").empty()) || (efData.m_strrType == "cn" && !StrTrim(efData.m_strVal, "F").empty()))
			{	
				cInst->Update_Record("MF", DDF, ADF, EF, i);//д�����ļ��е�����
			}
			if (efData.m_strrType == "B-TLV" || efData.m_strrType == "-")
			{
				bool flag = false;
				for (string::size_type j = 0; j < efData.m_vecEFExDatas.size(); ++j)
				{
					if ( ( (efData.m_vecEFExDatas.at(j).m_strrType == "an" || efData.m_vecEFExDatas.at(j).m_strrType == "b") && !StrTrim(efData.m_vecEFExDatas.at(j).m_strVal, "0").empty() ) ||
						 ( efData.m_vecEFExDatas.at(j).m_strrType == "cn" && !StrTrim(efData.m_vecEFExDatas.at(j).m_strVal, "F").empty() ) )
					{
						flag = true;
						break;
					}
				}
				if (!flag)
					continue;
				cInst->Update_Record("MF", DDF, ADF, EF, i);//д�����ļ��е�����
			}
		}
	}
	//͸���ļ� 
	else if (efInfo.m_strEFType == "3")
	{
		if (!cParameter->GetEFData(DDF, ADF, EF, 1, efData))//���ļ�û�м�¼��д
			return true;				
		string str;
		int len = HexToDec(efInfo.m_strEFLen);
		bool flag = StrTrim(efData.m_strVal, "0").empty();//����ֵΪtrueʱ��ʾʹ�ó�ʼ�����ݽ������
		if (!flag && StrDel_ConVar(efData.m_strVal, str))
		{	
			m_sstrTemp.clear();
			m_sstrTemp.str("");
			m_sstrTemp << "�ļ���ʼ�� InitFile(" << DDF << ", " << ADF << ", " << EF << ")���������ļ���ֵ��Ӧ���ǳ�����" << "��" << endl;
			cInfo->Append_Warning(m_sstrTemp.str());
			flag = true;
		}
		if (!JudgeHex(StrTrim(efData.m_strVal)))
		{
			m_sstrTemp.clear();
			m_sstrTemp.str("");
			m_sstrTemp << "�ļ���ʼ�� InitFile(" << DDF << ", " << ADF << ", " << EF << ")���������ļ���ֵ���зǷ��ַ���" << "��" << endl;
			cInfo->Append_Warning(m_sstrTemp.str());
			flag = true;
		}
		if (StrTrim(efData.m_strVal).length() > (unsigned long)len * 2)
		{
			m_sstrTemp.clear();
			m_sstrTemp.str("");
			m_sstrTemp << "�ļ���ʼ�� InitFile(" << DDF << ", " << ADF << ", " << EF << ")���������ļ���ֵ���ȹ���" << "��" << endl;
			cInfo->Append_Warning(m_sstrTemp.str());
			flag = true;
		}
		if (flag)
			str = "";
		else
			str = StrTrim(efData.m_strVal);
		
		if ( efData.m_strRemark != "" )//��ע�����Ϊ�յĻ������ע��
		{
			m_strTemp = "д"+efData.m_strRemark;
			LvOne_Annotations(m_strTemp);
		}

		for (int i = 0; i < len; i += 0x80)
		{
			m_sstrTemp.clear();
			m_sstrTemp.str("");
			if (i + 0x80 <= len)//��������0x80����Ϊ��λ��������д��¼
			{
				//͸���ļ�����������Ϊb
				if ((unsigned long)i + 0x80 <= str.length()/2)//str��ȡ����д0x80���ȵļ�¼����
					cInst->Update_Binary(i, 0x80, str.substr(i * 2, 0x100));
				else if ((unsigned long)i < str.length()/2)//strʣ�����ݲ���0x80���ȵļ�¼���ݲ�д����
				{
					m_sstrTemp << setfill('0') << setw(0x100) << left << str.substr(i * 2, str.length() - i * 2);
					cInst->Update_Binary(i, 0x80, m_sstrTemp.str());
				}
				else//д���00�ļ�¼����
				{
					m_sstrTemp << setfill('0') << setw(0x100) << left << "";
					cInst->Update_Binary(i, 0x80, m_sstrTemp.str());
				}
			}
			else//���������ʣ�಻��0x80���ȵ�����д��¼
			{
				if ((unsigned long)i < str.length()/2)//strʣ������д��¼
				{
					m_sstrTemp << setfill('0') << setw((len - i) * 2) << left << str.substr(i * 2, str.length() - i * 2);
					cInst->Update_Binary(i, len - i, m_sstrTemp.str() );
				}
				else//д������00�ļ�¼����
				{
					m_sstrTemp << setfill('0') << setw((len - i) * 2) << left << "";
					cInst->Update_Binary(i, len - i, m_sstrTemp.str());
				}
			}
		}
	}
	else
	{
		m_sstrTemp.clear();
		m_sstrTemp.str("");
		m_sstrTemp << "�ļ���ʼ�� InitFile(" << DDF << ", " << ADF << ", " << EF << ")���޷�ʶ����ļ����ͣ�EFType = " << efInfo.m_strEFType << ")��" << endl;
		cInfo->Append_Error(m_sstrTemp.str());
		return false;
	}
	return cInst->Error();
}

string CScript::Dir_Records( stringstream &strsInsts )
{
	int nP1=1,nLen=0;
	string strInst;
	strsInsts.str("");
	bool bRefreshFlag = false;

	vector<S_DDFINFO>::const_iterator iterSDDF = m_vecMFStruct.begin();
	while ( iterSDDF != m_vecMFStruct.end() )//����DDF
	{
		if ( "" != (*iterSDDF).m_strFID )//��DDF0
		{
			//���MF/DDFĿ¼�ļ���¼
			strInst = cInst->DIR_Record( nP1, "MF", (*iterSDDF).m_strFID, "", (*iterSDDF).m_strDDFName, bRefreshFlag );
			strsInsts << strInst << endl;
			nLen += HexToDec( strInst.substr(8,2) );//ȡָ��ͷ���ȵĲ���
			++nP1;
		}
		else//DDF0�Ļ���Ҫ���MF�µ�����ADF�ļ�¼��Ϣ
		{
			vector<S_ADFINFO>::const_iterator iterSADF = (*iterSDDF).m_vecADFInfo.begin();
			while ( iterSADF!=(*iterSDDF).m_vecADFInfo.end() )//������ǰDDF�µ�ADF
			{
				if ( "" != (*iterSADF).m_strFID )//��DF00
				{
					//���MF/ADFĿ¼�ļ���¼
					strInst = cInst->DIR_Record( nP1, "MF", "", (*iterSADF).m_strFID, (*iterSADF).m_strADFName, bRefreshFlag );
					strsInsts << strInst << endl;
					nLen += HexToDec( strInst.substr(8,2) );//ȡָ��ͷ���ȵĲ���
					++nP1;
				}

				++iterSADF;
			}
		}

		++iterSDDF;
	}

	return DecToHex(nLen);
}

string CScript::FCI_Records( string strDDF, stringstream &strsInsts )
{
	int nP1=1,nLen=0;
	string strInst,strDDFFID;
	strsInsts.str("");
	bool bRefreshFlag = false;

	transform(  strDDF.begin(), strDDF.end(), strDDF.begin(), toupper );
	//���MF��FCI��¼
	strInst = cInst->FCI_Record( nP1, "MF", "", "", bRefreshFlag );
	strsInsts << strInst << endl;
	nLen += HexToDec( strInst.substr(8,2) );//ȡָ��ͷ���ȵĲ���
	++nP1;	

	vector<S_DDFINFO>::const_iterator iterSDDF = m_vecMFStruct.begin();
	while ( iterSDDF != m_vecMFStruct.end() )//����DDF
	{
		if ( strDDF == "" )//���MF������DDF��ADF����Ϣ��¼
		{
			if ( "" != (*iterSDDF).m_strFID )//��DDF0���DDF�ļ�¼��Ϣ
			{
				//���MF/DDF��FCI��¼
				strInst = cInst->FCI_Record( nP1, "MF", (*iterSDDF).m_strFID, "", bRefreshFlag );
				strsInsts << strInst << endl;
				nLen += HexToDec( strInst.substr(8,2) );//ȡָ��ͷ���ȵĲ���
				++nP1;	
			}
			else//DDF0���MF��ֱ��ADF�ļ�¼��Ϣ
			{
				vector<S_ADFINFO>::const_iterator iterSADF = (*iterSDDF).m_vecADFInfo.begin();
				while ( iterSADF!=(*iterSDDF).m_vecADFInfo.end() )//������ǰDDF�µ�ADF
				{
					if ( "" != (*iterSADF).m_strFID )//��DF00
					{
						//���MF/ADF��FCI��¼
						strInst = cInst->FCI_Record( nP1, "MF", "", (*iterSADF).m_strFID, bRefreshFlag );
						strsInsts << strInst << endl;
						nLen += HexToDec( strInst.substr(8,2) );//ȡָ��ͷ���ȵĲ���
						++nP1;	
					}

					++iterSADF;
				}	
			}

			++iterSDDF;
		}
		else//��ӵ�ǰDDF�͵�ǰDDF������ADF����ؼ�¼��Ϣ
		{
			m_strTemp = (*iterSDDF).m_strDDFName;
			strDDFFID = (*iterSDDF).m_strFID;
			transform(  m_strTemp.begin(), m_strTemp.end(), m_strTemp.begin(), toupper );
			transform(  strDDFFID.begin(), strDDFFID.end(), strDDFFID.begin(), toupper );
			if ( !(m_strTemp == strDDF || strDDFFID == strDDF) )//��ΪDDF�ļ���ʶ����ΪӦ�û������ʱ��������(��ȡ��������DDF0��FIDΪ"")
			{
				++iterSDDF;
				continue;
			}

			//���MF�� DDF=strDDF ��FCI��¼
			strInst = cInst->FCI_Record( nP1, "MF", (*iterSDDF).m_strFID, "", bRefreshFlag );
			strsInsts << strInst << endl;
			nLen += HexToDec( strInst.substr(8,2) );//ȡָ��ͷ���ȵĲ���
			++nP1;	

			vector<S_ADFINFO>::const_iterator iterSADF = (*iterSDDF).m_vecADFInfo.begin();
			while ( iterSADF!=(*iterSDDF).m_vecADFInfo.end() )//������ǰDDF�µ�ADF
			{
				if ( "" != (*iterSADF).m_strFID )//��DF00
				{
					//���MF�� DDF=strDDF ������ADF��FCI��¼
					strInst = cInst->FCI_Record( nP1, "MF", (*iterSDDF).m_strFID, (*iterSADF).m_strFID, bRefreshFlag );
					strsInsts << strInst << endl;
					nLen += HexToDec( strInst.substr(8,2) );//ȡָ��ͷ���ȵĲ���
					++nP1;	
				}

				++iterSADF;
			}	

			break;
		}

	}

	return DecToHex(nLen);
}

string CScript::LoadKeys( string strDDF, string strADF, stringstream &strsInsts )
{
	int nLen=0,nPos,nBlockLen,i,nVARSeq;
	//ָ������ ��ȫ���� ��Կ����ͷ ��Կ����Կͷ ������Կ���ֽڳ��� ������Կ���ֽڳ��� ��Կ��ַ ���ö�ȡ����Կ��ַ ��Կ�汾 ��Կ����(��ȷ����Կ�汾��)
	string strInst,strSafeLv,strInstHead,strInstData,strOpKLen,strPKLen,strKeyAddr,strAddrs,strVer,strGroup;
	string strTmp,strTmp1,strBLOCK,strMK;
	bool bRefreshFlag = false;
	bool bEncrypt = false;//true ���ķ�ʽװ�ػ������Կ false ���ķ�ʽװ�ػ������Կ
	bool bMac = false;//true MAC��ʽװ����Կ
	vector<KEYDATAS> vecKeyDatas;//�洢��ȡָ��DF�µ�������Կ��Ϣ
	S_DDFINFO struDDFInfo;//�洢��ȡ��DDF��Ϣ
	S_ADFINFO struADFInfo;//��ȡADF��Ϣ

	strsInsts.str("");
	if ( strDDF == "" )
		strDDF = "DDF0";
	if ( strADF == "" )
		strADF = "DF00";

	/*��װ��������Կ �汾Ĭ��Ϊ00*/
	if ( strADF != "DF00" )//����ADF��ʱ��,��ȡADF��Ϣ,װ��ADF������Կ
	{
		m_strTemp = "���" + strADF + "�µ�������Կ";
		m_strTemp = LvOne_Annotations(m_strTemp,bRefreshFlag);
		strInst = cInst->Write_Key( "MF", strDDF, strADF, "MK", "00", bRefreshFlag );
		cParameter->GetADFInfo( strDDF, strADF, struADFInfo );
		strMK = struADFInfo.m_strMKvalue;
	}
	else if ( strDDF != "DDF0" )//����DDF��ʱ��,��ȡDDF��Ϣ,װ��DDF������Կ
	{
		m_strTemp = "���" + strDDF + "�µ�������Կ";
		m_strTemp = LvOne_Annotations(m_strTemp,bRefreshFlag);
		strInst = cInst->Write_Key( "MF", strDDF, "", "MK", "00", bRefreshFlag );
		cParameter->GetDDFInfo( strDDF, struDDFInfo );
		strMK = struDDFInfo.m_strMKvalue;
	}
	else//��ȡMF��Ϣ,װ��MF������Կ
	{
		m_strTemp = LvOne_Annotations("���MF�µ�������Կ",bRefreshFlag);
		strInst = cInst->Write_Key( "MF", "", "", "MK", "00", bRefreshFlag );
		strMK = m_struMFInfo.m_strMKvalue;
	}
	strsInsts << m_strTemp << endl << strInst << endl;//ע�ͼ�ָ��
	nLen = HexToDec( strInst.substr(8,2) );//ȡָ��ͷ���ȵĲ���
	strsInsts << endl;

	/*��װ��������Կ,����Ϊ���Ƶ�װ�����̺ͷ�ʽ(ע�⣺���ݽű�˵���ĵ�,���ܻ�װ��,���󡢵����ȿ���֧�����ĺ����ķ�ʽ�������硢ͬ�����֧�����ķ�ʽ)*/
	if ( cParameter->GetADFKeys( strDDF, strADF, vecKeyDatas ) )
	{
		if ( "0" == m_strLoadKeyMode || "10" == m_strLoadKeyMode )//������ӻ������ĸ���
			bEncrypt = true;
		if ( bEncrypt && "2"==m_struLoadKeyInfo.m_strMode && vecKeyDatas.size()>0 )//�̶���Կ�����ķ�ʽ ���õ�ǰĿ¼��MK��������Ȼ��װ����Կ
		{
			strsInsts << LvOne_Annotations("���õ�ǰĿ¼�ļ���MK��������Ȼ��װ����Կ", bRefreshFlag) << endl;
			strsInsts << cCmd->VAR( strMK, _KEY, bRefreshFlag ) << endl;
		}

		vector<KEYDATAS>::const_iterator iterKeyDatas = vecKeyDatas.begin();
		while ( iterKeyDatas!=vecKeyDatas.end() )
		{ 
			strTmp = (*iterKeyDatas).m_strName;
			transform( strTmp.begin(), strTmp.end(), strTmp.begin(), toupper );
			if ( strTmp == "PIN" )//����PIN��PIN��װ����������Կ���ٰ�װ
			{
				++iterKeyDatas;
				continue;
			}

			strVer = (*iterKeyDatas).m_strVer;//ȡ��Կ�ĳ�ʼ�汾
			/*װ����Կ*/
			if ( "1" == m_struLoadKeyInfo.m_strMode )//���ܻ�װ��
			{
				strAddrs = (*iterKeyDatas).m_strAddr;//ȡ���ܻ��ĵ�ַ
				nPos = strAddrs.find("|");
				while ( nPos != string::npos )//�����߷ָ�������Կ��ַ�з������ַ
				{
					strKeyAddr = strAddrs.substr( 0, nPos );//��ȡ��ǰ�汾����Կ��ַ
					strSafeLv = DecToHex((*iterKeyDatas).m_nSeq).substr(1);//��ȡ��Կ�İ�ȫ����
					strSafeLv += strSafeLv;

					m_strTemp = (*iterKeyDatas).m_strName + " ��ַ: 0x" + strKeyAddr + " �汾:" + strVer + " ��ȫ����:" + strSafeLv + " ���ȣ��ֽڣ�:" + (*iterKeyDatas).m_strLen;
					m_strTemp = LvOne_Annotations(m_strTemp,bRefreshFlag);
					//��ȡװ����Կ��ָ��ͷ����Կͷ
					if ( strADF == "DF00" )
						cInst->Write_Key( "MF", strDDF, "", (*iterKeyDatas).m_strName, strVer, bEncrypt, bMac );
					else
						cInst->Write_Key( "MF", strDDF, strADF, (*iterKeyDatas).m_strName, strVer, bEncrypt, bMac );
					strInstHead = cInst->Get_Instruction_Head();//����ͷ
					strInstData = cInst->Get_Instruction_Data();//��Կͷ
					//������Կ���ֽڳ��ȣ�0-8�ֽڣ�1-10�ֽ�
					if ( (*iterKeyDatas).m_strLen == "08" )
					{
						strOpKLen = "0";
						//�����8�ֽڵĻ������Ļ�������+mac��װ��ʱ��ָ��ͷ������Ҫ�ı�
						if ( bEncrypt && !bMac )//����
							strInstHead = strInstHead.substr(0,8)+"18";
						else if ( bEncrypt && bMac )//����+MAC
							strInstHead = strInstHead.substr(0,8)+"1C";
					}
					else if( (*iterKeyDatas).m_strLen == "10" )
						strOpKLen = "1";
					//������Կ���ֽڳ��ȣ�0-8�ֽڣ�1-10�ֽ�
					if ( m_struLoadKeyInfo.m_strPKeyLen == "8" )
						strPKLen = "0";
					else if( m_struLoadKeyInfo.m_strPKeyLen == "10" )
						strPKLen = "1";
					strInst = cCmd->LOADKEY( VAR_Name(_COSMANU), VAR_Name(_STOWAGE), strKeyAddr, VAR_Name(_PKEYNO), VAR_Name(_KEYFSJS), VAR_Name(_PKEYFSJS),
								   VAR_Name(_KEYFSYZ), VAR_Name(_PKEYFSYZ), strInstHead, strInstData, strOpKLen, strPKLen, bRefreshFlag );

					strsInsts << m_strTemp << endl << strInst << endl;//ע�ͼ�ָ��
					nLen += strInstData.length()/2 + 16;//�ۼ���Կ��¼���ȣ���Կͷ����+��Կ���ȣ�����ȫ�ļ��Ƕ����ļ�������Կ���ȶ�Ϊ16�ֽ�
					//װ�����֮��ͬоƬ�Ĳ���������,�˴�Ϊ����оƬ�Ĵ���ʽ
					if ( !bEncrypt && !bMac )//����
						strsInsts << cCmd->RESP( 0, 29, -1, bRefreshFlag ) <<endl;
					else if ( bEncrypt && !bMac )//����
					{
						if ( (*iterKeyDatas).m_strLen == "08" )//STK
							strsInsts << cCmd->RESP( 0, 29, -1, bRefreshFlag ) <<endl;
						else
							strsInsts << cCmd->RESP( 0, 37, -1, bRefreshFlag ) <<endl;
					}
					else if ( bEncrypt && bMac )//����+MAC
					{
						if ( (*iterKeyDatas).m_strLen == "08" )//STK
							strsInsts << cCmd->RESP( 0, 33, -1, bRefreshFlag ) <<endl;
						else
							strsInsts << cCmd->RESP( 0, 41, -1, bRefreshFlag ) <<endl;
					}

					strVer = DecToHex( HexToDec(strVer)+1 );//��Կ�汾��+1
					strAddrs = strAddrs.substr(nPos+1);//ʣ�����Կ��ַ
					nPos = strAddrs.find("|");
				}

				strsInsts << endl;
			}
			else if ( "2" == m_struLoadKeyInfo.m_strMode )//�̶���Կװ��
			{
				strGroup = (*iterKeyDatas).m_strGroup;
				nPos = strGroup.find("|");
				while ( nPos != string::npos )//����Կ���������߷ָ���ȷ�ϰ汾��
				{
					strSafeLv = DecToHex((*iterKeyDatas).m_nSeq).substr(1);//��ȡ��Կ�İ�ȫ����
					strSafeLv += strSafeLv;

					m_strTemp = (*iterKeyDatas).m_strName + " �汾:" + strVer + " ��ȫ����:" + strSafeLv + " ���ȣ��ֽڣ�:" + (*iterKeyDatas).m_strLen;
					strsInsts << LvOne_Annotations(m_strTemp,bRefreshFlag) << endl;
					//��ȡװ����Կ��ָ��ͷ����Կͷ
					if ( strADF == "DF00" )
						cInst->Write_Key( "MF", strDDF, "", (*iterKeyDatas).m_strName, strVer, bEncrypt, bMac );
					else
						cInst->Write_Key( "MF", strDDF, strADF, (*iterKeyDatas).m_strName, strVer, bEncrypt, bMac );
					strInstHead = cInst->Get_Instruction_Head();//����ͷ
					strInstData = cInst->Get_Instruction_Data();//��Կͷ

					/*��ͬ��ʽװ����Կ���ɲ�ָͬ��*/
					if ( !bEncrypt && !bMac )//����
					{
						strInst = strInstHead + " " + strInstData + " " + (*iterKeyDatas).m_strValue;
						strInst.append( 32-(*iterKeyDatas).m_strValue.length(), 'F' );//���е���Կ����F����16�ֽ�����
						strsInsts << strInst << endl;
					}
					else if ( !bEncrypt && bMac )//����+MAC
					{

					}
					else if ( bEncrypt && !bMac )//���� ���õ�ǰĿ¼��MK��������Ȼ��װ����Կ
					{
						strTmp = strInstData + (*iterKeyDatas).m_strValue;
						strTmp.append( 32-(*iterKeyDatas).m_strValue.length(), 'F' );//���е���Կ����F����16�ֽ�����
						//��������=��Կͷ����Կ����ܳ��� + ��Կͷ + ��Կ��
						strTmp1 = DecToHex( strTmp.length()/2 ) + strTmp;
						strsInsts << cCmd->VAR( strTmp1, 30, bRefreshFlag ) << endl;//�������ݱ��浽VAR30
						i = 0;
						nVARSeq = 33;//���ڴ洢ÿ�����ݿ�3DES����Ľ����������еļ�����ƴ�ӵ�һ���������
						nBlockLen = strTmp.length()/2 + 1;//�õ��������ݳ���(�ֽ�)
						while ( nBlockLen >= 8 )//nBlockLen���ֽ�Ϊ��λ
						{
							strsInsts << cCmd->SUBSTR( i*8, 8, VAR_Name(30), 31, bRefreshFlag ) << endl;//���8�ֽڵķֿ����ݱ��浽VAR31
							strsInsts << cCmd->TDES( VAR_Name(31), VAR_Name(_KEY), nVARSeq+i, bRefreshFlag ) << endl;//�ֿ����ݵ�3DES���������浽VAR��nVARSeq+i��
							nBlockLen = nBlockLen - 8;
							++i;
						}
						if ( nBlockLen != 0 )//���������������ݿ�Ļ�����Ҫ�����ݿ�֮���80��Ȼ����00��䵽8�ֽڵ����ݿ飬����3DES����
						{
							strsInsts << cCmd->SUBSTR( i*8, nBlockLen, VAR_Name(30), 32, bRefreshFlag ) << endl;//���ķֿ�����VAR32
							strBLOCK = "80";//��һ���ֽڵ�80
							strBLOCK.append( (8-nBlockLen-1)*2, '0' );//������8-nLen-1���ֽڵ�00
							strBLOCK = VAR_Name(32) + strBLOCK;
							strsInsts << cCmd->VAR( strBLOCK, 31, bRefreshFlag ) << endl;//8�ֽڵķֿ����ݱ��浽VAR31
							strsInsts << cCmd->TDES( VAR_Name(31), VAR_Name(_KEY), nVARSeq+i, bRefreshFlag ) << endl;//�ֿ����ݵ�3DES���������浽VAR��nVARSeq+i��
							++i;
						}
						//ָ�� = ָ��ͷ + ��������
						strInst = strInstHead + " ";
						for ( int j=0; j<i; ++j )
							strInst += VAR_Name(nVARSeq+j);
						strsInsts << strInst << endl;
							
					}
					else if ( bEncrypt && bMac )//����+MAC
					{
						/*18 0D05003433002210 [VAR1] 80000000000000>21
						[SUBSTR(0,8,[VAR21])]>22
						[3DES([VAR22],[VAR0])]>30
						[SUBSTR(8,8,[VAR21])]>23
						[3DES([VAR23],[VAR0])]>31
						[SUBSTR(16,8,[VAR21])]>24
						[3DES([VAR24],[VAR0])]>32
						[SUBSTR(24,8,[VAR21])]>25
						[3DES([VAR25],[VAR0])]>33
						84D4000024 [VAR30][VAR31][VAR32][VAR33] [CMAC(0,37,[VAR0])]*/
					}

					nLen += strInstData.length()/2 + 16;//�ۼ���Կ��¼���ȣ���Կͷ����+��Կ���ȣ�����ȫ�ļ��Ƕ����ļ�������Կ���ȶ�Ϊ16�ֽ�
					strVer = DecToHex( HexToDec(strVer)+1 );//��Կ�汾��+1
					strGroup=strGroup.substr(nPos+1);
					nPos = strGroup.find("|");
				}

				strsInsts << endl;
			}

			++iterKeyDatas;
		}
	}
	
	//��װ DDF/DF00 �µ�PIN
	if ( strADF == "DF00" && cParameter->GetADFKeys( strDDF, "", vecKeyDatas ) )
	{
		vector<KEYDATAS>::const_iterator iterKeyDatas = vecKeyDatas.begin();
		while ( iterKeyDatas!=vecKeyDatas.end() )
		{ 
			strTmp = (*iterKeyDatas).m_strName;
			transform( strTmp.begin(), strTmp.end(), strTmp.begin(), toupper );
			if ( strTmp == "PIN" )//��װPIN,�汾Ĭ��Ϊ00
			{
				m_strTemp = LvOne_Annotations("��װPIN",bRefreshFlag);
				strsInsts << m_strTemp << endl;
				cInst->Write_Key( "MF", strDDF, "", "PIN", "00", bEncrypt, bMac );
				strInstHead = cInst->Get_Instruction_Head();//����ͷ
				strInstData = cInst->Get_Instruction_Data();//��Կͷ

				/*��ͬ��ʽװ����Կ���ɲ�ָͬ��*/
				if ( !bEncrypt && !bMac )//����
				{
					strInst = strInstHead + " " + strInstData + (*iterKeyDatas).m_strValue;
					strInst.append( 32-(*iterKeyDatas).m_strValue.length(), 'F' );
					strsInsts << strInst << endl;
				}
				else if ( !bEncrypt && bMac )//����+MAC
				{

				}
				else if ( bEncrypt && !bMac )//����
				{
					strsInsts << LvOne_Annotations("���õ�ǰĿ¼��MK��������Ȼ��װPIN", bRefreshFlag) << endl;
					strsInsts << cCmd->VAR( strMK, _KEY, bRefreshFlag ) << endl;
					strTmp = strInstData + (*iterKeyDatas).m_strValue;
					strTmp.append( 32-(*iterKeyDatas).m_strValue.length(), 'F' );
					//��������=��Կͷ����Կ����ܳ��� + ��Կͷ + ��Կ��
					strTmp1 = DecToHex( strTmp.length()/2 ) + strTmp;
					strsInsts << cCmd->VAR( strTmp1, 30, bRefreshFlag ) << endl;//�������ݱ��浽VAR30
					i = 0;
					nVARSeq = 33;//���ڴ洢ÿ�����ݿ�3DES����Ľ����������еļ�����ƴ�ӵ�һ���������
					nBlockLen = strTmp.length()/2 + 1;//�õ��������ݳ���(�ֽ�)
					while ( nBlockLen >= 8 )//nBlockLen���ֽ�Ϊ��λ
					{
						strsInsts << cCmd->SUBSTR( i*8, 8, VAR_Name(30), 31, bRefreshFlag ) << endl;//���8�ֽڵķֿ����ݱ��浽VAR31
						strsInsts << cCmd->TDES( VAR_Name(31), VAR_Name(_KEY), nVARSeq+i, bRefreshFlag ) << endl;//�ֿ����ݵ�3DES���������浽VAR��nVARSeq+i��
						nBlockLen = nBlockLen - 8;
						++i;
					}
					if ( nBlockLen != 0 )//���������������ݿ�Ļ�����Ҫ�����ݿ�֮���80��Ȼ����00��䵽8�ֽڵ����ݿ飬����3DES����
					{
						strsInsts << cCmd->SUBSTR( i*8, nBlockLen, VAR_Name(30), 32, bRefreshFlag ) << endl;//���ķֿ�����VAR32
						strBLOCK = "80";//��һ���ֽڵ�80
						strBLOCK.append( (8-nBlockLen-1)*2, '0' );//������8-nLen-1���ֽڵ�00
						strBLOCK = VAR_Name(32) + strBLOCK;
						strsInsts << cCmd->VAR( strBLOCK, 31, bRefreshFlag ) << endl;//8�ֽڵķֿ����ݱ��浽VAR31
						strsInsts << cCmd->TDES( VAR_Name(31), VAR_Name(_KEY), nVARSeq+i, bRefreshFlag ) << endl;//�ֿ����ݵ�3DES���������浽VAR��nVARSeq+i��
						++i;
					}
					//ָ�� = ָ��ͷ + ��������
					strInst = strInstHead + " ";
					for ( int j=0; j<i; ++j )
						strInst += VAR_Name(nVARSeq+j);
					strsInsts << strInst << endl;
							
				}
				else if ( bEncrypt && bMac )//����+MAC
				{
				}

				nLen += strInstData.length()/2 + 16;//�ۼ���Կ��¼���ȣ�PIN��Կ�����F����16�ֽ�
				break;
			}
			++iterKeyDatas;
		}
	}

	return DecToHex(nLen);
}

void CScript::GetCardID()
{
	KEYDATAS struKeyDatas;
	CITYCODE struCityCode;
	string strInst,strAddr,strKey,strBLOCK;
	bool bRefreshFlag = m_bRefreshFlag;
	int nPos,nLen,i;

	//��ȡ���д���
	cParameter->GetCityCode(  struCityCode );
	
	LvOne_Annotations("��IRK��Կ�������ʶ����", bRefreshFlag);
	cCmd->SUBSTR( 4, 4, VAR_Name(_CARD_SN), 30, bRefreshFlag );//������ˮ�ű��浽VAR30
	//Ӧ�ó��д���+��ᱣ�Ͽ�Ӧ�ñ�ʶ��+������ˮ�� ���浽VAR31
	m_strTemp = struCityCode.m_strValue+"D156000005"+VAR_Name(30);
	cCmd->VAR( m_strTemp, 31, bRefreshFlag );
	//��ȡSSSE��IRK��Կ��Ϣ
	cParameter->GetKeyInfo( "SSSE", "", "IRK", struKeyDatas );

	if ( "1" == m_struLoadKeyInfo.m_strMode )//���ܻ�װ��
	{
		strAddr = struKeyDatas.m_strAddr;
		nPos = strAddr.find("|");
		strAddr = strAddr.substr( 0, nPos );

		cCmd->GETMAC( "1", "1", VAR_Name(_CARD_SN), "1", strAddr, "0000000000000000", VAR_Name(31), VAR_Name(_PKEYNO), "1", "00000000", 32, bRefreshFlag );//��mac�Ľ�����浽VAR32
		cCmd->SUBSTR( 0, 4, VAR_Name(32), _MAC, bRefreshFlag );
	}
	else if ( "2" == m_struLoadKeyInfo.m_strMode )//�̶���Կװ�� ��Ҫ��MACֵ
	{
		//ȡ��������󲹡�00000000������ʼֵ
		cInst->Get_Challenge( 8, bRefreshFlag );
		LvOne_Annotations( "ȡ�����ǰ4�ֽ����ݺ󲹡�00000000������ʼֵ,���浽 VAR32", bRefreshFlag );
		cCmd->VAR( cCmd->RESP(0,4,-1,false)+"00000000", 32);

		strKey = struKeyDatas.m_strValue;//IRK ����Կֵ
		LvOne_Annotations("IRK��Կ", bRefreshFlag);
		cCmd->VAR( strKey, _KEY, bRefreshFlag );
		LvOne_Annotations("IRK��Կ��KEY_L���浽VAR38", bRefreshFlag);
		cCmd->VAR( strKey.substr(0,16), 38, bRefreshFlag );
		i = 0;
		nLen = struCityCode.m_strValue.length()/2 + 5 + 4;//�õ���Ҫ����MAC�����ݳ���(�ֽ�)
		while ( nLen > 8 )//nLen���ֽ�Ϊ��λ
		{
			cCmd->SUBSTR( i*8, 8, VAR_Name(31), 33, bRefreshFlag );//���8�ֽڵķֿ����ݱ��浽VAR33
			cCmd->XOR( VAR_Name(32), VAR_Name(33), 34, bRefreshFlag );//��ʼֵVAR32���ֿ�����VAR33�õ���Ҫ���ܵ����ݱ��浽VAR34
			cCmd->DES( VAR_Name(34), VAR_Name(38), 32, m_bRefreshFlag );//DES����Ľ����Ϊ��һ��������ĳ�ʼֵ
			nLen = nLen - 8;
			++i;
		}
		if ( nLen == 8 )//����������ݿ�ĳ���Ϊ8�ֽڵĻ�������Ҫ�����ݿ�֮���һ��8�ֽڵ�80 00 00 00 00 00 00 00����3DES����õ�mac
		{
			cCmd->SUBSTR( i*8, 8, VAR_Name(31), 33, bRefreshFlag );
			cCmd->XOR( VAR_Name(32), VAR_Name(33), 34, bRefreshFlag );
			cCmd->DES( VAR_Name(34), VAR_Name(38), 32, m_bRefreshFlag );
			
			//�õ���������������ݿ�
			strBLOCK = "8000000000000000";
			cCmd->VAR( strBLOCK, 33, bRefreshFlag );
			cCmd->XOR( VAR_Name(32), VAR_Name(33), 34, bRefreshFlag );
		}
		else if ( nLen < 8 )//����������ݿ��С��8�ֽڵĻ�������Ҫ�����ݿ�֮���80��Ȼ����00��䵽8�ֽڵ����ݿ飬����3DES�����ȡmac
		{
			cCmd->SUBSTR( i*8, nLen, VAR_Name(31), 33, bRefreshFlag );//���ķֿ����ݱ��浽VAR33
			strBLOCK = "80";//��һ���ֽڵ�80
			strBLOCK.append( (8-nLen-1)*2, '0' );//������8-nLen-1���ֽڵ�00
			strBLOCK = VAR_Name(33) + strBLOCK;
			cCmd->VAR( strBLOCK, 36, bRefreshFlag );
			cCmd->XOR( VAR_Name(32), VAR_Name(36), 34, bRefreshFlag );
		}
		//ȡ�����ֽ���ΪMACֵ
		cCmd->TDES( VAR_Name(34), VAR_Name(_KEY), 35, m_bRefreshFlag );//3DES����Ľ�����浽VAR35
		cCmd->SUBSTR( 0, 4, VAR_Name(35), _MAC, bRefreshFlag );

	}

	m_strTemp = VAR_Name(31)+VAR_Name(_MAC);
	cCmd->VAR( m_strTemp, _CARDID, bRefreshFlag );//����ʶ����
}

void CScript::CreateMF()
{
	stringstream strsRecords;
	string strLen,strDirFID;

	cCmd->RESET(m_bRefreshFlag);

	//����֤������Կ
	LvOne_Annotations( "��֤������Կ" );
	cCmd->ATR( 11, 6, _ATR, m_bRefreshFlag );
	m_strTemp = "0000" + VAR_Name(_ATR);
	cCmd->VAR(  m_strTemp, _TEMP, m_bRefreshFlag );
	cCmd->TDES( VAR_Name(_TEMP), m_strTranKey, _TRANS_KEY, m_bRefreshFlag );
	cInst->Verify_TranCode( 8, VAR_Name(_TRANS_KEY), m_bRefreshFlag );
	LineFeed(1);

	//����MF
	LvOne_Annotations( "����MF" );
	cInst->Create_File( "MF", "", "", "", m_bRefreshFlag );
	LineFeed(1);

	//������ȫ�ļ�
	LvOne_Annotations( "������ȫ�ļ�" );
	strLen = LoadKeys( "", "", strsRecords );//��ȡװ����Կ�ļ�¼����Կ�ļ��ռ�
	cInst->Create_Internal_EF( "SF", strLen, m_bRefreshFlag  );
	cInfo->Append_Script( strsRecords.str() );//������е�װ����Կ�ű�����

	//����Ŀ¼�ļ�
	LvOne_Annotations( "����Ŀ¼�ļ�" );
	strLen = Dir_Records( strsRecords );//��ȡĿ¼�ļ��ļ�¼���ļ��ռ�
	cInst->Create_Internal_EF( "DIR", strLen, m_bRefreshFlag  );
	strDirFID = cInst->Get_Internal_FID("DIR");
	cInst->Select_File( false, true, 2, strDirFID, m_bRefreshFlag );//ѡ��Ŀ¼�ļ�
	cInfo->Append_Script( strsRecords.str() );//���Ŀ¼�ļ��ļ�¼

	//����FCI�ļ�
	LvOne_Annotations( "����FCI�ļ�" );
	strLen = FCI_Records( "", strsRecords );//��ȡĿ¼�ļ��ļ�¼���ļ��ռ�
	cInst->Create_Internal_EF( "FCI", strLen, m_bRefreshFlag  );
	cInfo->Append_Script( strsRecords.str() );//���Ŀ¼�ļ��ļ�¼

	//�ⲿ��֤MF��������Կ
	LvOne_Annotations( "�ⲿ��֤MF��������Կ", m_bRefreshFlag );
	cCmd->VAR( m_struMFInfo.m_strMKvalue, _KEY, m_bRefreshFlag );//��MF��MK���浽_KEY
	cInst->Get_Challenge( 8, m_bRefreshFlag );
	cCmd->RESP( 0, 8, _VAR10, m_bRefreshFlag );
	cCmd->TDES( VAR_Name(_VAR10), VAR_Name(_KEY), _VAR11, m_bRefreshFlag );
	cCmd->DES( VAR_Name(_VAR10), VAR_Name(_VAR11), _VAR12, m_bRefreshFlag );
	cInst->External_Authenticate( "00", VAR_Name(_VAR12), VAR_Name(_VAR10), 0, m_bRefreshFlag );//MK����Կ��ʶΪ00
	LineFeed(1);

}

void CScript::CreateDDF( string strDDF )
{
	stringstream strsRecords;
	string strLen,strTmp;
	strTmp = strDDF;
	S_DDFINFO struDDFInfo;

	transform( strDDF.begin(), strDDF.end(), strDDF.begin(), toupper );
	if ( strDDF == "DDF0" || strDDF == "" )
		return;

	//����DDF
	m_strTemp = "����"+strDDF;
	LvTwo_Annotations( m_strTemp );
	cInst->Select_File( "MF", "", "", "", m_bRefreshFlag );
	cInst->Create_File( "MF", strTmp, "", "", m_bRefreshFlag );
	LineFeed(1);

	//������ȫ�ļ�
	cInst->Select_File( "MF", strDDF, "", "", m_bRefreshFlag );
	LvOne_Annotations( "������ȫ�ļ�" );
	strLen = LoadKeys( strTmp, "", strsRecords );//��ȡװ����Կ�ļ�¼����Կ�ļ��ռ�
	cInst->Create_Internal_EF( "SF", strLen, m_bRefreshFlag  );
	cInfo->Append_Script( strsRecords.str() );//������е�װ����Կ�ű�����

	//����FCI�ļ�
	LvOne_Annotations( "����FCI�ļ�" );
	strLen = FCI_Records( strTmp, strsRecords );//��ȡĿ¼�ļ��ļ�¼���ļ��ռ�
	cInst->Create_Internal_EF( "FCI", strLen, m_bRefreshFlag  );
	cInfo->Append_Script( strsRecords.str() );//���Ŀ¼�ļ��ļ�¼

	//�ⲿ��֤DDF��������Կ
	m_strTemp = "�ⲿ��֤"+strDDF+"��������Կ";
	LvOne_Annotations( m_strTemp, m_bRefreshFlag );
	cParameter->GetDDFInfo( strDDF, struDDFInfo );//��ȡDDF��Ϣ
	cCmd->VAR( struDDFInfo.m_strMKvalue, _KEY, m_bRefreshFlag );//��DDF��MK���浽_KEY
	cInst->Get_Challenge( 8, m_bRefreshFlag );
	cCmd->RESP( 0, 8, _VAR10, m_bRefreshFlag );
	cCmd->TDES( VAR_Name(_VAR10), VAR_Name(_KEY), _VAR11, m_bRefreshFlag );
	cCmd->DES( VAR_Name(_VAR10), VAR_Name(_VAR11), _VAR12, m_bRefreshFlag );
	cInst->External_Authenticate( "00", VAR_Name(_VAR12), VAR_Name(_VAR10), 0, m_bRefreshFlag );//MK����Կ��ʶΪ00
	LineFeed(1);

}

void CScript::CreateADFS( string strDDF /*= ""*/ )
{
	string strTmp,strDDFFID;

	transform( strDDF.begin(), strDDF.end(), strDDF.begin(), toupper );

	vector<S_DDFINFO>::const_iterator iterSDDF = m_vecMFStruct.begin();
	while ( iterSDDF != m_vecMFStruct.end() )//����DDF
	{
		strTmp = (*iterSDDF).m_strDDFName;//Ӧ�û������
		strDDFFID = (*iterSDDF).m_strFID;//DDF���ļ���ʶ��
		transform(  strTmp.begin(), strTmp.end(), strTmp.begin(), toupper );
		transform(  strDDFFID.begin(), strDDFFID.end(), strDDFFID.begin(), toupper );
		if ( !(strTmp == strDDF || strDDFFID == strDDF) )//��ΪDDF�ļ���ʶ����ΪӦ�û������ʱ��������(��ȡ��������DDF0��FIDΪ"")
		{
			++iterSDDF;
			continue;
		}
		//��ǰDDF��û��ADF(����DF00)�����ý�ADFĿ¼�ļ�������ѭ��
		if ( (*iterSDDF).m_vecADFInfo.size()==0 || ( (*iterSDDF).m_vecADFInfo.size()==1 && (*iterSDDF).m_vecADFInfo.front().m_strADFName=="DF00" ) )
			break;
		if ( strTmp == "DDF0" )
			m_strTemp = "����MF�µ�����ADF";
		else
			m_strTemp = "����"+strTmp+"�µ�����ADF";
		LvTwo_Annotations( m_strTemp );
		cInst->Select_File( "MF", (*iterSDDF).m_strFID, "", "", m_bRefreshFlag );
		//�ҵ���Ӧ��DDF�󣬱���ADF������DDF�µ�����ADF
		vector<S_ADFINFO>::const_iterator iterSADF = (*iterSDDF).m_vecADFInfo.begin();
		while ( iterSADF!=(*iterSDDF).m_vecADFInfo.end() )//����ADF
		{
			strTmp = (*iterSADF).m_strFID;
			transform(  strTmp.begin(), strTmp.end(), strTmp.begin(), toupper );
			if ( strTmp == "" )//DF00�Ļ���������
			{
				++iterSADF;
				continue;
			}
			m_strTemp = "����"+(*iterSADF).m_strADFName;
			LvOne_Annotations( m_strTemp );
			//��DF00����ADF�ļ�
			cInst->Create_File( "MF", (*iterSDDF).m_strDDFName, (*iterSADF).m_strFID, "", m_bRefreshFlag );

			++iterSADF;
		}	

		LineFeed(1);
		break;
	}
}

void CScript::CreateEFS( string strDDF /*= ""*/, string strADF /*= "" */ )
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
			if ( (*iterSADF).m_vecEFInfo.size() == 0 )//˵��ADF��û��EF�ļ�,ֱ������ѭ��
				break;
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
				LineFeed(1);
			}

			strTmp = (*iterSADF).m_strAID;
			transform(  strTmp.begin(), strTmp.end(), strTmp.begin(), toupper );
			if ( strDDF == "SSSE" && strTmp == "D15600000503" )
			{
				//��ᱣ��������2Ŀ¼�ļ�����Ҫ��TF�ڲ��ļ�����ʹ��ҽ�����ѵȹ���
				LvOne_Annotations( "���������ļ�(ҽ�Ʊ��ս����ڲ��ļ�) ����FF(��ֹ) д: DSK", m_bRefreshFlag );
				cInst->Create_Internal_EF( "TF", "1F", m_bRefreshFlag  );
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
				InitFile( (*iterSDDF).m_strDDFName, (*iterSADF).m_strFID, (*iterEF).m_strSFI );
				LineFeed(1);

				++iterEF;
			}

			break;
		}	

		break;
	}
}

void CScript::End_AllPreIssue()
{
	LvTwo_Annotations("Ԥ���˻�����");
	LvOne_Annotations("ѡ��MF,Ԥ���˻�����");
	cInst->Select_File( "MF", "", "", "", m_bRefreshFlag );
	cInst->End_PreIssue( m_bRefreshFlag );
	LineFeed(1);

	vector<S_DDFINFO>::const_iterator iterSDDF = m_vecMFStruct.begin();
	while ( iterSDDF != m_vecMFStruct.end() )
	{
		if ( (*iterSDDF).m_strFID != "" )//��DDF0
		{
			m_strTemp = "ѡ��MF-"+(*iterSDDF).m_strDDFName+",Ԥ���˻�����";
			LvOne_Annotations( m_strTemp );
			cInst->Select_File( "MF", "", "", "", m_bRefreshFlag );
			cInst->Select_File( "MF", (*iterSDDF).m_strFID, "", "", m_bRefreshFlag );
			cInst->End_PreIssue( m_bRefreshFlag );
			LineFeed(1);
		}
		else//DDF0
			cInst->Select_File( "MF", "", "", "", m_bRefreshFlag );
		
		vector<S_ADFINFO>::const_iterator iterSADF = (*iterSDDF).m_vecADFInfo.begin();
		while ( iterSADF!=(*iterSDDF).m_vecADFInfo.end() )
		{
			if ( (*iterSADF).m_strFID != "" )//��DF00
			{
				if ( (*iterSDDF).m_strFID != "" )//��DDF0
					m_strTemp = "ѡ��MF-"+(*iterSDDF).m_strDDFName+"-"+(*iterSADF).m_strFID+",Ԥ���˻�����";
				else
					m_strTemp = "ѡ��MF-"+(*iterSADF).m_strFID+",Ԥ���˻�����";
				LvOne_Annotations( m_strTemp );
				cInst->Select_File( "MF", (*iterSDDF).m_strFID, (*iterSADF).m_strFID, "", m_bRefreshFlag );
				cInst->End_PreIssue( m_bRefreshFlag );
				LineFeed(1);
			}
			++iterSADF;
		}
		++iterSDDF;
	}

}