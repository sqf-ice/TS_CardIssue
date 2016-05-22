
#include "Information.h"
#include "pugixml.hpp"
using namespace pugi;
#include <algorithm>
#include "tool.h"

extern bool strHEX( const pair<string,string> &str1, const pair<string,string> &str2 );

SETTING CInformation::s_struSetting;
vector<KEYS> CInformation::s_vecKEYS;
MFINFO CInformation::s_struCardInfo;

CInformation::CInformation(void):m_nErrorCnt(0),m_nWarningCnt(0)
{
	m_vecErrData.clear();
	m_vecWrnData.clear();
	m_vecScriptData.clear();
}


CInformation::~CInformation(void)
{
}

bool CInformation::ReadXMLDatas( string strXmlPath, int nCheckLog/*=1 */ )
{
	if ( !CheckXMLFile(strXmlPath) )
		return false;
	ReadSettingDatas(strXmlPath);
	ReadKeysDatas(strXmlPath);
	ModeKeysDatasSeq();
	ReadCardInfo(strXmlPath);
	if ( nCheckLog == 1 )
		WriteXmlDatasLog();
	return true;
}

bool CInformation::CheckXMLFile( string strXmlPath )
{
	bool bCheckFlag=false,bFlag1=false,bFlag2=false,bFlag3=false;
	xml_document doc;
	string strTmp;

	doc.load_file(strXmlPath.c_str());//����ȡ��������·����λload
	//���һ����ǩ��<INFO>��ǩ�Ƿ����
	for ( xml_node xml = doc.first_child(); xml; xml = xml.next_sibling() )
	{
		strTmp = xml.name();
		if ( strTmp == "INFO" )
		{
			bCheckFlag = true;
			break;
		}
	}
	if ( !bCheckFlag )
	{
		m_strTemp = "XML�ļ���"+strXmlPath+"����ȱʧ<INFO>��ǩ�������ò��ǽű������ļ���������ѡ���ļ�!";
		Append_Error(m_strTemp);
		return false;
	}

	//��������ǩ��<SETTING>��<SSSEKEYS>��<MF>��ǩ�Ƿ����
	xml_node info = doc.child("INFO");
	for ( xml_node datas = info.first_child(); datas; datas = datas.next_sibling() )
	{
		strTmp = datas.name();
		if ( strTmp == "SETTING" )
			bFlag1 = true;
		else if ( strTmp == "SSSEKEYS" )
			bFlag2 = true;
		else if ( strTmp == "MF" )
			bFlag3 = true;
	}
	if ( !bFlag1 )
	{
		m_strTemp = "XML�ļ���"+strXmlPath+"����<INFO>��ǩ��ȱʧ��ǩ<SETTING>���ýű������ļ����������޷����ɽű���������ѡ���ļ�!";
		Append_Error(m_strTemp);
	}
	if ( !bFlag2 )
	{
		m_strTemp = "XML�ļ���"+strXmlPath+"����<INFO>��ǩ��ȱʧ��ǩ<SSSEKEYS>���ýű������ļ����������޷����ɽű���������ѡ���ļ�!";
		Append_Error(m_strTemp);
	}
	if ( !bFlag3 )
	{
		m_strTemp = "XML�ļ���"+strXmlPath+"����<INFO>��ǩ��ȱʧ��ǩ<MF>���ýű������ļ����������޷����ɽű���������ѡ���ļ�!";
		Append_Error(m_strTemp);
	}

	return bFlag1 & bFlag2 & bFlag3;
}

void CInformation::ReadSettingDatas( string strXmlPath )
{
	PROJMOD struProjMod;
	xml_document doc;

	doc.load_file(strXmlPath.c_str());//����ȡ��������·����λload
	xml_node setting = doc.child("INFO").child("SETTING");

	//��Ŀ��Ϣ
	S_SETTING.m_struProjInfo.m_vecProjMod.clear();
	xml_node proj = setting.child("PROJ");
	S_SETTING.m_struProjInfo.m_strName = StrDelBlank(proj.attribute("Name").value());
	S_SETTING.m_struProjInfo.m_strDesc = StrDelBlank(proj.attribute("Desc").value());
	struProjMod.m_strAuthor = StrDelBlank(proj.attribute("Author").value());
	struProjMod.m_strModify = StrDelBlank(proj.attribute("Modify").value());
	struProjMod.m_strDate = StrDelBlank(proj.attribute("Date").value());
	struProjMod.m_strVer = StrDelBlank(proj.attribute("Ver").value());
	S_SETTING.m_struProjInfo.m_vecProjMod.push_back(struProjMod);
	xml_named_node_iterator mode = proj.children("MOD").begin();
	while (mode!=proj.children("MOD").end())
	{
		struProjMod.m_strAuthor = StrDelBlank(mode->attribute("Author").value());
		struProjMod.m_strModify = StrDelBlank(mode->attribute("Modify").value());
		struProjMod.m_strDate = StrDelBlank(mode->attribute("Date").value());
		struProjMod.m_strVer = StrDelBlank(mode->attribute("Ver").value());
		S_SETTING.m_struProjInfo.m_vecProjMod.push_back(struProjMod);
		++mode;
	}

	//����ʶ����
	xml_node pin = setting.child("PIN");
	S_SETTING.m_struPIN.m_strValue = StrDelBlank(pin.attribute("Value").value());
	S_SETTING.m_struPIN.m_strCount = StrDelBlank(pin.attribute("Count").value());

	//���д���
	xml_node citycode = setting.child("CITYCODE");
	S_SETTING.m_struCityCode.m_strValue = StrDelBlank(citycode.attribute("Value").value());
	S_SETTING.m_struCityCode.m_strProvince = StrDelBlank(citycode.attribute("province").value());
	S_SETTING.m_struCityCode.m_strCity = StrDelBlank(citycode.attribute("city").value());

	//װ����Կ�����Ϣ
	xml_node loadkeymode = setting.child("LOADKEYMODE");
	xml_node loadkeytype = setting.child("STORGEMODE");
	xml_node encryptdev = setting.child("ENCRYPTDEV");
	xml_node loadset = encryptdev.child("LOADSET");
	S_SETTING.m_struLoadKeyInfo.m_strMode = StrDelBlank(loadkeymode.attribute("Value").value());
	S_SETTING.m_struLoadKeyInfo.m_strType = StrDelBlank(loadkeytype.attribute("Value").value());
	S_SETTING.m_struLoadKeyInfo.m_strScale = StrDelBlank(encryptdev.attribute("Scale").value());
	S_SETTING.m_struLoadKeyInfo.m_strPKeyNo = StrDelBlank(loadset.attribute("PKeyNo").value());
	S_SETTING.m_struLoadKeyInfo.m_strPKeyfsjs = StrDelBlank(loadset.attribute("PKeyfsjs").value());
	S_SETTING.m_struLoadKeyInfo.m_strPKeyfsyz = StrDelBlank(loadset.attribute("PKeyfsyz").value());
	S_SETTING.m_struLoadKeyInfo.m_strPKeyLen = StrDelBlank(loadset.attribute("PKeyLen").value());
	//����0-8�ֽڣ�1-10�ֽڵĴ�����ʽ
	if ( S_SETTING.m_struLoadKeyInfo.m_strPKeyLen == "0" )
		S_SETTING.m_struLoadKeyInfo.m_strPKeyLen = "8";
	else if ( S_SETTING.m_struLoadKeyInfo.m_strPKeyLen == "1" )
		S_SETTING.m_struLoadKeyInfo.m_strPKeyLen = "10";
}

void CInformation::ReadKeysDatas( string strXmlPath )
{
	xml_document doc;
	KEYS struKeys;

	V_EKEYS.clear();
	doc.load_file(strXmlPath.c_str());

	//���SSSE����Կ����
	struKeys.m_strDDFName = "SSSE";
	struKeys.m_vecADFKEY.clear();
	V_EKEYS.push_back(struKeys);

	//������ǩ<SSSEKEYS>�������Կ����
	xml_node ssseKeys = doc.child("INFO").child("SSSEKEYS"); 
	for ( xml_node adfKey = ssseKeys.first_child(); adfKey; adfKey = adfKey.next_sibling() )
	{
		string strTagADFName = adfKey.name();
		transform(strTagADFName.begin(), strTagADFName.end(), strTagADFName.begin(), toupper);
		if ( strTagADFName == "SSSE" )
			strTagADFName = "DF00";
		AddKeysDatas( strTagADFName, adfKey.attribute("Name").value(), adfKey.attribute("KeyIndex").value(), 
			adfKey.attribute("Ver").value(), adfKey.attribute("PSAMKeyType").value(), adfKey.attribute("PSAMKeyID").value(), 
			adfKey.attribute("Addr").value(), adfKey.attribute("Type").value(), adfKey.attribute("Len").value(), 
			adfKey.attribute("Group").value(), adfKey.attribute("Count").value(), adfKey.attribute("Value").value() );
	}

	//������ǩ<OTHERKEYS>�������Կ����
	xml_node keys = doc.child("INFO").child("OTHERKEYS"); 
	for ( xml_node key = keys.first_child(); key; key = key.next_sibling() )
	{
		//����KEYS�ṹ�壬������ӵ�m_strDDFName�Ƿ����ظ�(�����ִ�Сд)���ظ��Ļ���������ݣ�����������ʾ
		vector<KEYS>::const_iterator iterKeys = V_EKEYS.begin();
		while ( iterKeys != V_EKEYS.end() )
		{
			string strTagDDFName = key.name();
			string strVecDDFName = (*iterKeys).m_strDDFName;
			transform(strTagDDFName.begin(), strTagDDFName.end(), strTagDDFName.begin(), toupper);
			transform(strVecDDFName.begin(), strVecDDFName.end(), strVecDDFName.begin(), toupper);
			if ( strTagDDFName == strVecDDFName )
			{
				m_strsTemp.str("");
				if ( strTagDDFName == "SSSE" )
					m_strsTemp<<"<OTHERKEYS>��ֻ�ܴ��ڳ��籣Ӧ��<SSSE>֮���������ǩ����,��Ϊ�Ѿ�����<SSSEKEYS>��ǩ����,����<OTHERKEYS>�µ�<SSSE>��ǩ������Ч";
				else
					m_strsTemp<<"<OTHERKEYS>�����ظ���ǩ<"<<strTagDDFName<<">,��ͬ��ǩ���ֻ����ǰ��ı�ǩ����Ϊ׼������������Ч";
				Append_Warning(m_strsTemp.str());
				break;
			}
			++iterKeys;
		}
		if ( iterKeys == V_EKEYS.end() )
		{
			//����µ�KEYS����
			struKeys.m_strDDFName = key.name();
			struKeys.m_vecADFKEY.clear();
			V_EKEYS.push_back(struKeys);//��ĩβ���Ԫ��

			//������ǩDDF�������
			xml_node ddfKey = keys.child( key.name() );
			for ( xml_node adfKey = ddfKey.first_child(); adfKey; adfKey = adfKey.next_sibling() )
			{
				AddKeysDatas( adfKey.name(), adfKey.attribute("Name").value(), adfKey.attribute("KeyIndex").value(), 
					adfKey.attribute("Ver").value(), adfKey.attribute("PSAMKeyType").value(), adfKey.attribute("PSAMKeyID").value(), 
					adfKey.attribute("Addr").value(), adfKey.attribute("Type").value(), adfKey.attribute("Len").value(), 
					adfKey.attribute("Group").value(), adfKey.attribute("Count").value(), adfKey.attribute("Value").value() );
			}
		}
	}

}

void CInformation::AddKeysDatas( string strTagName, string strName, string strKeyIndex, string strVer, string strPSAMKeyType, string strPSAMKeyID, string strAddr, string strType, string strLen, string strGroup, string strCount, string strValue )
{
	ADFKEY struKey;
	KEYDATAS struKeyDatas;

	vector<ADFKEY>::iterator iterKey = V_EKEYS.back().m_vecADFKEY.begin();//����ĩβԪ�ص�vec�ĵ�����
	//����ADFKEY�е����ݣ�������ǰ�ı�ǩstrTagName�Ƿ���������е�m_strADFName��
	while ( iterKey != V_EKEYS.back().m_vecADFKEY.end() )
	{
		//��ԿADFKEY�е�m_strADFName�Ѵ���
		string strVecADFName = StrDelBlank((*iterKey).m_strADFName);
		string strXMLTagName = StrDelBlank(strTagName);
		transform(strVecADFName.begin(), strVecADFName.end(), strVecADFName.begin(), toupper);
		transform(strXMLTagName.begin(), strXMLTagName.end(), strXMLTagName.begin(), toupper);
		if ( strVecADFName == strXMLTagName )
			break;
		++iterKey;
	}
	if( iterKey == V_EKEYS.back().m_vecADFKEY.end() )//��ADFKEY�����ADF��
	{
		struKey.m_strADFName = StrDelBlank(strTagName);
		struKey.m_vecKeyDatas.clear();
		V_EKEYS.back().m_vecADFKEY.push_back(struKey);
		iterKey = V_EKEYS.back().m_vecADFKEY.end()-1;//������ָ�����ӵ�ADF
	}

	//������ǰADF�е�KEYDATAS���ݣ�����Կ���Ʋ������������Կ��Ϣ
	vector<KEYDATAS>::const_iterator iterKeyData= (*iterKey).m_vecKeyDatas.begin();
	while ( iterKeyData != (*iterKey).m_vecKeyDatas.end() )
	{
		//��Կ�����Ѵ���,˵���ظ���Կ��Ϣ,��¼������
		string strVecKeyName = StrDelBlank((*iterKeyData).m_strName);
		string strXMLKeyName = StrDelBlank(strName);
		transform(strVecKeyName.begin(), strVecKeyName.end(), strVecKeyName.begin(), toupper);
		transform(strXMLKeyName.begin(), strXMLKeyName.end(), strXMLKeyName.begin(), toupper);
		if ( strVecKeyName == strXMLKeyName )
		{
			m_strsTemp.str("");
			if ( V_EKEYS.back().m_strDDFName == "SSSE" )
			{
				if ( (*iterKey).m_strADFName == "DF00" )
					m_strsTemp<<"<SSSEKEYS><SSSE>�´����ظ���Կ<SSSE Name=\""<<strName<<"\" />";
				else
					m_strsTemp<<"<SSSEKEYS><"<<(*iterKey).m_strADFName<<">�д����ظ���Կ<"<<strTagName<<" Name=\""<<strName<<"\" />";
			}
			else
				m_strsTemp<<"<OTHERKEYS><"<<V_EKEYS.back().m_strDDFName<<"><"<<(*iterKey).m_strADFName<<">�д����ظ���Կ<"<<strTagName<<" Name=\""<<strName<<"\" />";
			Append_Warning(m_strsTemp.str());
			break;
		}
		++iterKeyData;
	}
	if ( iterKeyData == (*iterKey).m_vecKeyDatas.end() )//�����Կ����
	{
		struKeyDatas.m_strName = StrDelBlank(strName);
		struKeyDatas.m_strKeyIndex = StrDelBlank(strKeyIndex);
		struKeyDatas.m_strVer = StrDelBlank(strVer);
		struKeyDatas.m_strPSAMKeyType = StrDelBlank(strPSAMKeyType);
		struKeyDatas.m_strPSAMKeyID = StrDelBlank(strPSAMKeyID);
		struKeyDatas.m_strAddr = StrDelBlank(strAddr);
		struKeyDatas.m_strType = StrDelBlank(strType);
		struKeyDatas.m_strLen = StrDelBlank(strLen);
		//������ʮ������16��ʾ���ȵ���ʽ
		if ( struKeyDatas.m_strLen == "16" )
			struKeyDatas.m_strLen = "10";
		struKeyDatas.m_strGroup = StrDelBlank(strGroup);
		struKeyDatas.m_strCount = StrDelBlank(strCount);
		struKeyDatas.m_strValue = StrDelBlank(strValue);
		struKeyDatas.m_nSeq = 0;
		(*iterKey).m_vecKeyDatas.push_back(struKeyDatas);
	}
}

void CInformation::ModeKeysDatasSeq()
{
	string strDDF,strADF;
	//int nSeq;
	KEYDATAS struKeyDatas;
	stringstream strsTmp;

	//����KEYS������
	vector<KEYS>::iterator iterKeys = V_EKEYS.begin();
	while ( iterKeys!=V_EKEYS.end() )
	{
		//����<DDF>����
		vector<ADFKEY>::iterator iterKey = (*iterKeys).m_vecADFKEY.begin();
		while ( iterKey!=(*iterKeys).m_vecADFKEY.end() )
		{
			//nSeq = 1;
			strDDF = (*iterKeys).m_strDDFName;
			strADF = (*iterKey).m_strADFName;
			transform( strDDF.begin(), strDDF.end(), strDDF.begin(), toupper );
			transform( strADF.begin(), strADF.end(), strADF.begin(), toupper );
			if ( "SSSE" == strDDF && "DF00" == strADF )//SSSE�����PIN
			{
				struKeyDatas.m_nSeq = 0;
				struKeyDatas.m_strAddr = "";
				struKeyDatas.m_strCount = S_SETTING.m_struPIN.m_strCount;
				struKeyDatas.m_strGroup = "";
				struKeyDatas.m_strKeyIndex = "00";
				struKeyDatas.m_strLen = DecToHex(S_SETTING.m_struPIN.m_strValue.length()/2);
				struKeyDatas.m_strName = "PIN";
				struKeyDatas.m_strPSAMKeyID = "";
				struKeyDatas.m_strPSAMKeyType = "";
				struKeyDatas.m_strType = "";
				struKeyDatas.m_strValue = S_SETTING.m_struPIN.m_strValue;
				struKeyDatas.m_strVer = "00";
				//++nSeq;
				(*iterKey).m_vecKeyDatas.push_back(struKeyDatas);
			}
			//��Ӱ�ȫ����
			//vector<KEYDATAS>::iterator iterKeyDatas = (*iterKey).m_vecKeyDatas.begin();
			//while ( iterKeyDatas!=(*iterKey).m_vecKeyDatas.end() )
			//{
			//	if ( nSeq > 15 )//��ȫ״̬������Χ11-FF
			//	{
			//		strsTmp.str("");
			//		strsTmp << "<KESYS><" <<(*iterKeys).m_strDDFName << ">�µ�<" << (*iterKey).m_strADFName << ">��Կ��������16�������޷����䵽��Կ�İ�ȫ״̬";
			//		Append_Error( strsTmp.str() );
			//		break;
			//	}
			//	(*iterKeyDatas).m_nSeq = nSeq;

			//	++nSeq;
			//	++iterKeyDatas;
			//}
			//if ( "SSSE" == strDDF && "DF00" == strADF )//SSSE�����PIN����Կ����
			//	(*iterKey).m_vecKeyDatas.push_back(struKeyDatas);

			++iterKey;
		}

		++iterKeys;
	}
}

void CInformation::ReadCardInfo( string strXmlPath )
{
	xml_document doc;
	DDFINFO struDDFInfo;
	ADFINFO struADFInfo;
	EFINFO struEFInfo;
	EFDATAS struEFDatas;
	EFEXTRADATAS struEFExtra;
	bool bFlag;
	string strTemp;
	long lLen;//���ڼ����ļ����ܳ���
	int nP;//���ڼ�¼TAG��P1

	doc.load_file(strXmlPath.c_str());
	xml_node mf = doc.child("INFO").child("MF");
	S_CARDNFO.m_strMFName =  StrDelBlank(mf.name());//��ǩ��
	S_CARDNFO.m_strMKvalue = StrDelBlank(mf.attribute("MKvalue").value());//����ֵ
	S_CARDNFO.m_strCount = StrDelBlank(mf.attribute("Count").value());
	S_CARDNFO.m_strFID = StrDelBlank(mf.attribute("FID").value());
	S_CARDNFO.m_strAID = StrDelBlank(mf.attribute("AID").value());

	S_CARDNFO.m_vecDDFInfo.clear();
	//����MF�µ�DDF��SSSE��PSE��
	for ( xml_node ddf = mf.first_child(); ddf; ddf = ddf.next_sibling() )
	{
		/*�������DDF��ǩ�����Ƿ������vector�����ڵĻ�˵��xml�ű���DDF��ǩ���ظ�*/
		vector<DDFINFO>::const_iterator iterJudgeDDF = S_CARDNFO.m_vecDDFInfo.begin();
		bFlag = false;
		while ( iterJudgeDDF != S_CARDNFO.m_vecDDFInfo.end() )
		{
			string strVecDDFName = (*iterJudgeDDF).m_strDDFName;
			string strXMLDDFName = StrDelBlank(ddf.name());
			transform(strVecDDFName.begin(), strVecDDFName.end(), strVecDDFName.begin(), toupper);
			transform(strXMLDDFName.begin(), strXMLDDFName.end(), strXMLDDFName.begin(), toupper);
			if ( strVecDDFName == strXMLDDFName )
			{
				bFlag = true;
				m_strsTemp.str("");
				m_strsTemp<<"<MF> �����ظ���DDF <"<<ddf.name()<<">,��ͬ��ǩ���ֻ����ǰ��ı�ǩ����Ϊ׼������������Ч";
				Append_Warning(m_strsTemp.str());
				break;
			}
			++iterJudgeDDF;
		}
		if ( bFlag )
		{
			//���ظ���DDF�����������ݣ�����ѭ��
			continue;
		}

		//���������ݵ����ݽṹ��
		struDDFInfo.m_strDDFName = StrDelBlank(ddf.name());
		struDDFInfo.m_strMKvalue = StrDelBlank(ddf.attribute("MKvalue").value());
		struDDFInfo.m_strCount = StrDelBlank(ddf.attribute("Count").value());
		struDDFInfo.m_strSpace = StrDelBlank(ddf.attribute("Space").value());
		struDDFInfo.m_strUpdateMode1 = StrDelBlank(ddf.attribute("UpdateMode1").value());
		struDDFInfo.m_strUpdateMode2 = StrDelBlank(ddf.attribute("UpdateMode2").value());
		struDDFInfo.m_strFID = StrDelBlank(ddf.attribute("FID").value());
		struDDFInfo.m_strAID = StrDelBlank(ddf.attribute("AID").value());

		struDDFInfo.m_vecADFInfo.clear();
		//������ǰDDF�µ�ADF
		for ( xml_node adf = ddf.first_child(); adf; adf = adf.next_sibling() )
		{
			/*��⵱ǰDDF�µ�ADF��ǩ������evctor���Ƿ��Ѵ���,�������ʾXML�����ظ��ı�ǩ����*/
			bFlag = false;
			vector<ADFINFO>::const_iterator iterJudgeADF = struDDFInfo.m_vecADFInfo.begin();
			while ( iterJudgeADF!=struDDFInfo.m_vecADFInfo.end() )
			{
				string strVecADFName = (*iterJudgeADF).m_strADFName;
				string strXMLADFName = StrDelBlank(adf.name());
				transform(strVecADFName.begin(), strVecADFName.end(), strVecADFName.begin(), toupper);
				transform(strXMLADFName.begin(), strXMLADFName.end(), strXMLADFName.begin(), toupper);
				if ( strVecADFName == strXMLADFName )
				{
					bFlag = true;
					m_strsTemp.str("");
					m_strsTemp<<"<MF><"<<ddf.name()<<"> �����ظ���ADF <"<<adf.name()<<">,��ͬ��ǩ���ֻ����ǰ��ı�ǩ����Ϊ׼������������Ч";
					Append_Warning(m_strsTemp.str());
					break;
				}
				++iterJudgeADF;
			}
			if ( bFlag )
			{
				//���ظ���ADF�����������ݣ�����ѭ��
				continue;
			}

			//���������ݵ����ݽṹ��
			struADFInfo.m_strADFName = StrDelBlank(adf.name());
			struADFInfo.m_strMKvalue = StrDelBlank(adf.attribute("MKvalue").value());
			struADFInfo.m_strCount = StrDelBlank(adf.attribute("Count").value());
			struADFInfo.m_strUpdateMode1 = StrDelBlank(adf.attribute("UpdateMode1").value());
			struADFInfo.m_strUpdateMode2 = StrDelBlank(adf.attribute("UpdateMode2").value());
			struADFInfo.m_strFID = StrDelBlank(adf.attribute("FID").value());
			struADFInfo.m_strAID = StrDelBlank(adf.attribute("AID").value());

			struADFInfo.m_vecEFInfo.clear();
			//������ǰEF�µ�EFINFO
			xml_named_node_iterator efinfo = adf.children("EF").begin();
			while ( efinfo != adf.children("EF").end() )
			{
				if ( (*efinfo).attribute("EFType").value() == "5" )
				{
					++efinfo;
					continue;
				}
				/*��⵱ǰADF�µ�EF��ǩ����SFI��evctor���Ƿ��Ѵ���,�������ʾXML�����ظ��ı�ǩ����*/
				bFlag = false;
				vector<EFINFO>::const_iterator iterJudgeEF = struADFInfo.m_vecEFInfo.begin();
				while ( iterJudgeEF!=struADFInfo.m_vecEFInfo.end() )
				{
					string strVecEFSFI = (*iterJudgeEF).m_strSFI;
					string strXMLEFSFI = StrDelBlank((*efinfo).attribute("SFI").value());
					transform(strVecEFSFI.begin(), strVecEFSFI.end(), strVecEFSFI.begin(), toupper);
					transform(strXMLEFSFI.begin(), strXMLEFSFI.end(), strXMLEFSFI.begin(), toupper);
					if ( strVecEFSFI == strXMLEFSFI )
					{
						bFlag = true;
						m_strsTemp.str("");
						m_strsTemp<<"<MF><"<<ddf.name()<<"><"<<adf.name()<<"> �����ظ��� <EF SFI=\""<<(*efinfo).attribute("SFI").value()<<"\">,��ͬ��ǩ���ֻ����ǰ��ı�ǩ����Ϊ׼������������Ч";
						Append_Warning(m_strsTemp.str());
						break;
					}
					++iterJudgeEF;
				}
				if ( bFlag )
				{
					//EF�����ظ���SFI�����������ݣ�����ѭ��
					++efinfo;
					continue;
				}

				//���������ݵ����ݽṹ��
				struEFInfo.m_strSFI = StrDelBlank((*efinfo).attribute("SFI").value());
				struEFInfo.m_strReadControl = StrDelBlank((*efinfo).attribute("ReadControl").value());
				struEFInfo.m_strWriteControl = StrDelBlank((*efinfo).attribute("WriteControl").value());
				struEFInfo.m_strEFType = StrDelBlank((*efinfo).attribute("EFType").value());
				struEFInfo.m_strbInit = StrDelBlank((*efinfo).attribute("bInit").value());
				struEFInfo.m_strnCount = StrDelBlank((*efinfo).attribute("nCount").value());
				struEFInfo.m_strResSpace = StrDelBlank((*efinfo).attribute("ResSpace").value());

				struEFInfo.m_vecEFDatas.clear();
				//����m_strSFI��ǩ��Ӧ������
				string strSFI = StrDelBlank((*efinfo).attribute("SFI").value());
				xml_named_node_iterator efdatas = adf.children(strSFI.c_str()).begin();
				while( efdatas!=adf.children(strSFI.c_str()).end() )
				{
					string rType = StrDelBlank((*efdatas).attribute("rType").value());
					transform(rType.begin(),rType.end(),rType.begin(),toupper);
					string strEFType = StrDelBlank((*efinfo).attribute("EFType").value());
					if( "4" == strEFType )//ѭ���ļ�
					{
						struEFDatas.m_strP1 = "";
						struEFDatas.m_strTag = "-";
						struEFDatas.m_strVal = "";
						struEFDatas.m_strrType = "-";
						struEFDatas.m_strRemark = "";
						struEFDatas.m_vecEFExDatas.clear();
						lLen=0;
						//ѭ��������EF������ѭ�����ݲ����浽�������ݽṹ��
						while( efdatas!=adf.children(strSFI.c_str()).end() )
						{
							struEFExtra.m_strP1 = StrDelBlank((*efdatas).attribute("P1").value());
							struEFExtra.m_strTag = StrDelBlank((*efdatas).attribute("Tag").value());
							struEFExtra.m_strLen = StrDelBlank((*efdatas).attribute("Len").value());
							struEFExtra.m_strVal = StrDelBlank((*efdatas).attribute("Val").value());
							struEFExtra.m_strrType = StrDelBlank((*efdatas).attribute("rType").value());
							struEFExtra.m_strRemark = StrDelBlank((*efdatas).attribute("remark").value());
							lLen += HexToDec(struEFExtra.m_strLen);
							//����EF�������ݽṹ
							struEFDatas.m_vecEFExDatas.push_back(struEFExtra);
							++efdatas;
						}
						struEFDatas.m_strLen = DecToHex(lLen);//ѭ���ļ����ܳ���

						//����EF���ݽṹ
						struEFInfo.m_vecEFDatas.push_back(struEFDatas);
						break;
					}
					else if( "B-TLV" == rType )//B-TLV���ݻ���Ҫ�洢��EFEXTRADATAS��
					{
						struEFDatas.m_strP1 = StrDelBlank((*efdatas).attribute("P1").value());
						struEFDatas.m_strTag = StrDelBlank((*efdatas).attribute("Tag").value());
						struEFDatas.m_strVal = "";
						struEFDatas.m_strrType = "B-TLV";
						struEFDatas.m_strRemark = StrDelBlank((*efdatas).attribute("remark").value());
						struEFDatas.m_vecEFExDatas.clear();
						//������ǩTag+(*efdatas).attribute("Tag").value()�ı�ǩ����
						lLen=0;
						nP=0;
						string strTag="Tag"+struEFDatas.m_strTag;
						xml_named_node_iterator efextra = adf.children(strTag.c_str()).begin();
						while( efextra!=adf.children(strTag.c_str()).end() )
						{
							m_strsTemp.str("");
							m_strsTemp<<nP+1;
							struEFExtra.m_strP1 = m_strsTemp.str();
							struEFExtra.m_strTag = StrDelBlank((*efextra).attribute("Tag").value());
							struEFExtra.m_strLen = StrDelBlank((*efextra).attribute("Len").value());
							struEFExtra.m_strVal = StrDelBlank((*efextra).attribute("Val").value());
							struEFExtra.m_strrType = StrDelBlank((*efextra).attribute("rType").value());
							lLen += HexToDec(struEFExtra.m_strLen)+2;
							//����EF�������ݽṹ
							struEFDatas.m_vecEFExDatas.push_back(struEFExtra);
							++nP;
							++efextra;
						}
						struEFDatas.m_strLen = DecToHex(lLen);//B-TLV�ļ��ĳ���
					} 
					else
					{
						struEFDatas.m_strP1 = StrDelBlank((*efdatas).attribute("P1").value());
						struEFDatas.m_strTag = StrDelBlank((*efdatas).attribute("Tag").value());
						struEFDatas.m_strLen = StrDelBlank((*efdatas).attribute("Len").value());
						struEFDatas.m_strVal = StrDelBlank((*efdatas).attribute("Val").value());
						struEFDatas.m_strrType = StrDelBlank((*efdatas).attribute("rType").value());
						struEFDatas.m_strRemark = StrDelBlank((*efdatas).attribute("remark").value());
						struEFDatas.m_vecEFExDatas.clear();
					}
					//����EF���ݽṹ
					struEFInfo.m_vecEFDatas.push_back(struEFDatas);

					++efdatas;
				}
				//����ADF���ݽṹ
				struADFInfo.m_vecEFInfo.push_back(struEFInfo);

				++efinfo;
			}
			//����DDF���ݽṹ
			struDDFInfo.m_vecADFInfo.push_back(struADFInfo);
		}
		//����MF���ݽṹ
		S_CARDNFO.m_vecDDFInfo.push_back(struDDFInfo);
	}
}

void CInformation::CheckXmlDatas()
{
	string strTgNames="",strDDFName,strTemp1,strTemp2,strAID1,strAID2,strTmp;
	vector<pair<string,string>> vecAID;//<DDF��ADF������, AID>

	/*���<SETTING>��ǩ�����ݺϷ���*/
	CheckSETTINGDatas();
	/*���<SSSEKEYS>��<OTHERKEYS>��ǩ�����ݺϷ���*/
	CheckKEYSSDatas();
	/*���<SSSEKEYS>��<OTHERKEYS>��ǩ��ĳЩ�������Կ�Ƿ񶼴���*/
	CheckNecessaryKeys();

	/*���<MF>��ǩ�����ݺϷ���*/
	strTgNames = "<"+S_CARDNFO.m_strMFName+">";
	if ( S_CARDNFO.m_vecDDFInfo.size() == 0 )
	{
		m_strsTemp.str("");
		m_strsTemp<<"<INFO><MF>��ǩ�µ�����Ϊ��,���ű����ݲ�����,�벹ȫXML����";
		Append_Error(m_strsTemp.str());
		return;
	}
	else
	{
		vector<DDFINFO>::const_iterator iterDDFTemp = S_CARDNFO.m_vecDDFInfo.begin();
		while( iterDDFTemp!=S_CARDNFO.m_vecDDFInfo.end() )//����Ƿ�����籣Ӧ��<SSSE>
		{
			strDDFName = (*iterDDFTemp).m_strDDFName;
			transform( strDDFName.begin(), strDDFName.end(), strDDFName.begin(), toupper );
			if ( strDDFName=="SSSE" )
				break;
			++iterDDFTemp;
		}
		if ( iterDDFTemp==S_CARDNFO.m_vecDDFInfo.end() )
		{
			m_strsTemp.str("");
			m_strsTemp<<"<INFO><MF>��ǩ��ȱ���籣Ӧ��<SSSE>����,�޷����籣Ӧ��,�벹ȫXML����";
			Append_Error(m_strsTemp.str());
			return;
		}
	}

	if( !CheckMFDatas( strTgNames ) )
	{
		Append_Error(m_strsTemp.str());
	}
	//������xml��ȡ����<MF>��ǩ�ڵ�����
	vector<DDFINFO>::const_iterator iterDDF = S_CARDNFO.m_vecDDFInfo.begin();
	while ( iterDDF!=S_CARDNFO.m_vecDDFInfo.end() )//����DDF
	{
		//����DDF,���DDF��FID�Ƿ����ظ�(�����ִ�Сд)
		strTemp1 = (*iterDDF).m_strFID;
		transform( strTemp1.begin(), strTemp1.end(), strTemp1.begin(), toupper );
		vector<DDFINFO>::const_iterator iterDDF1 = iterDDF+1;
		while ( iterDDF1 != S_CARDNFO.m_vecDDFInfo.end() )
		{
			strTemp2 = (*iterDDF1).m_strFID;
			transform( strTemp2.begin(), strTemp2.end(), strTemp2.begin(), toupper );
			if ( strTemp1 == strTemp2 )
			{
				m_strsTemp.str("");
				m_strsTemp<<"<INFO><"+S_CARDNFO.m_strMFName+">�µ� <"+(*iterDDF).m_strDDFName+"> �� <"+(*iterDDF1).m_strDDFName+"> ������ͬ��FID,���Ϸ�";
				Append_Error(m_strsTemp.str());
				break;
			}
			++iterDDF1;
		}
		if ( (*iterDDF).m_strFID != "" )
		{
			strTmp = "<"+(*iterDDF).m_strDDFName+">";
			vecAID.push_back( make_pair( strTmp, (*iterDDF).m_strAID ) );
		}

		strTgNames = "<"+S_CARDNFO.m_strMFName+"><"+(*iterDDF).m_strDDFName+">";
		if ( (*iterDDF).m_vecADFInfo.size() == 0 )
		{
			m_strsTemp.str("");
			m_strsTemp<<"<INFO>"<<strTgNames<<"��ǩ�µ�����Ϊ��,���ɽ����ű��ᵼ��ȱʧ��DDF�µ��ļ�����";
			Append_Warning(m_strsTemp.str());
		}
		if ( !CheckDDFDatas( strTgNames, *iterDDF ) )
		{
			Append_Error(m_strsTemp.str());
		}

		vector<ADFINFO>::const_iterator iterADF = (*iterDDF).m_vecADFInfo.begin();
		while ( iterADF!=(*iterDDF).m_vecADFInfo.end() )//����ADF
		{
			//����ADF,��⵱ǰDDF�µ�ADF��FID�Ƿ����ظ�(�����ִ�Сд)
			strTemp1 = (*iterADF).m_strFID;
			transform( strTemp1.begin(), strTemp1.end(), strTemp1.begin(), toupper );
			vector<ADFINFO>::const_iterator iterADF1 = iterADF+1;
			while ( iterADF1 != (*iterDDF).m_vecADFInfo.end() )
			{
				strTemp2 = (*iterADF1).m_strFID;
				transform( strTemp2.begin(), strTemp2.end(), strTemp2.begin(), toupper );
				if ( strTemp1 == strTemp2 )
				{
					m_strsTemp.str("");
					m_strsTemp<<"<INFO><"+S_CARDNFO.m_strMFName+"><"+(*iterDDF).m_strDDFName+">�µ� <"+(*iterADF).m_strADFName+"> �� <"+(*iterADF1).m_strADFName+"> ������ͬ��FID,���Ϸ�";
					Append_Error(m_strsTemp.str());
					break;
				}
				++iterADF1;
			}
			if ( (*iterADF).m_strFID != "" )
			{
				strTmp = "<"+(*iterDDF).m_strDDFName+"><"+(*iterADF).m_strADFName+">";
				vecAID.push_back( make_pair( strTmp, (*iterADF).m_strAID ) );
			}

			strTgNames = "<"+S_CARDNFO.m_strMFName+"><"+(*iterDDF).m_strDDFName+"><"+(*iterADF).m_strADFName+">";
			if ( (*iterADF).m_vecEFInfo.size() == 0 )
			{
				m_strsTemp.str("");
				m_strsTemp<<"<INFO>"<<strTgNames<<"��ǩ�µ�����Ϊ��,���ɽ����ű��ᵼ��ȱʧ��ADF�µ�EF�ļ�����";
				Append_Warning(m_strsTemp.str());
			}
			if ( !CheckADFDatas( strTgNames, *iterADF ) )
			{
				Append_Error(m_strsTemp.str());
			}

			m_setTag.clear();//ͬһADF�µ�B-TLV��������ֻ���һ��
			vector<EFINFO>::const_iterator iterEFInfo = (*iterADF).m_vecEFInfo.begin();
			while ( iterEFInfo!=(*iterADF).m_vecEFInfo.end() )//����EFInfor
			{
				strTgNames = "<"+S_CARDNFO.m_strMFName+"><"+(*iterDDF).m_strDDFName+"><"+(*iterADF).m_strADFName+">";
				if ( (*iterEFInfo).m_vecEFDatas.size() == 0 && (*iterEFInfo).m_strSFI != "" && (*iterEFInfo).m_strEFType != "3" )//͸���ļ����������ڼ�¼���Բ����
				{
					m_strsTemp.str("");
					m_strsTemp<<"<INFO>"<<strTgNames<<"��ȱ��<"<<(*iterEFInfo).m_strSFI<<">��ǩ����,���ɽ����ű��ᵼ��ȱʧ��EF�ļ��ļ�¼";
					Append_Warning(m_strsTemp.str());
				}
				//���EF�����ڼ�¼�Ļ������û��Ԥ���ռ�Ļ�����
				if ( (*iterEFInfo).m_vecEFDatas.size() == 0 && (*iterEFInfo).m_strSFI != "" && HexToDec((*iterEFInfo).m_strResSpace)<=0 )
				{
					m_strsTemp.str("");
					m_strsTemp<<"<INFO>"<<strTgNames<<"<EF SFI=\""<<(*iterEFInfo).m_strSFI<<"\">��Ϊ���ļ�ȱʧ�ļ���¼������Ԥ���ռ�Space�ֶ�ֵӦ�ô�������ܽ���Ч���ļ�";
					Append_Warning(m_strsTemp.str());
				}
				CheckEFInfor( (*iterDDF).m_strDDFName, strTgNames, *iterEFInfo, (*iterADF).m_strADFName );

				vector<EFDATAS>::const_iterator iterEFDatas = (*iterEFInfo).m_vecEFDatas.begin();
				while ( iterEFDatas != (*iterEFInfo).m_vecEFDatas.end() )//EFDatas
				{
					if ( "4" == (*iterEFInfo).m_strEFType )//����Ƿ����ѭ���ļ��ļ�¼����
					{
						if ( (*iterEFDatas).m_vecEFExDatas.size() == 0 )
						{
							m_strsTemp.str("");
							m_strsTemp<<"<INFO>"<<strTgNames<<"��ȱ��<"<<(*iterEFInfo).m_strSFI<<">��ǩ����,���ɽ����ű��ᵼ��ȱʧ��EF�ļ��ļ�¼";
							Append_Warning(m_strsTemp.str());
						}
						//���EF�����ڼ�¼�Ļ������û��Ԥ���ռ�Ļ�����
						if ( (*iterEFDatas).m_vecEFExDatas.size() == 0 && HexToDec((*iterEFInfo).m_strResSpace)<=0 )
						{
							m_strsTemp.str("");
							m_strsTemp<<"<INFO>"<<strTgNames<<"<EF SFI=\""<<(*iterEFInfo).m_strSFI<<"\">��Ϊ���ļ�ȱʧ�ļ���¼������Ԥ���ռ�Ӧ�ô�������ܽ���Ч���ļ�";
							Append_Warning(m_strsTemp.str());
						}
					}
					else if ( "B-TLV" == (*iterEFDatas).m_strrType )//����Ƿ����B-TLV���͵ļ�¼����
					{
						if ( (*iterEFDatas).m_vecEFExDatas.size() == 0 )
						{
							m_strsTemp.str("");
							m_strsTemp<<"<INFO>"<<strTgNames<<"��ȱ��<Tag"<<(*iterEFDatas).m_strTag<<">��ǩ����,���ɽ����ű��ᵼ�¸�ADF���ļ�<"<<(*iterEFInfo).m_strSFI<<" P1=\""<<(*iterEFDatas).m_strP1<<"\">�ļ�¼Ϊ��";
							Append_Warning(m_strsTemp.str());
						}
						//���EF�����ڼ�¼�Ļ������û��Ԥ���ռ�Ļ�����
						if ( (*iterEFDatas).m_vecEFExDatas.size() == 0 && HexToDec((*iterEFInfo).m_strResSpace)<=0 )
						{
							m_strsTemp.str("");
							m_strsTemp<<"<INFO>"<<strTgNames<<"<EF SFI=\""<<(*iterEFInfo).m_strSFI<<"\">��Ϊ���ļ�ȱʧ�ļ���¼������Ԥ���ռ�Ӧ�ô�������ܽ���Ч���ļ�";
							Append_Warning(m_strsTemp.str());
						}
					}

					++iterEFDatas;
				}

				++iterEFInfo;
			}

			++iterADF;
		}

		++iterDDF;
	}

	sort( vecAID.begin(), vecAID.end(), strHEX );//��������
	//����vecAID,��⵱ǰvecAID�µ�AID�Ƿ����ظ�(�����ִ�Сд)(����������ֻ��Ҫǰ�����ݶԱȾ�֪����û���ظ���)
	vector<pair<string, string>>::const_iterator iterAID = vecAID.begin();
	while ( iterAID != vecAID.end()-1 )
	{
		strAID1 = (*iterAID).second;
		strTemp1 = (*iterAID).first;
		iterAID++;
		strAID2 = (*iterAID).second;
		strTemp2 = (*iterAID).first;
		transform( strAID1.begin(), strAID1.end(), strAID1.begin(), toupper );
		transform( strAID2.begin(), strAID2.end(), strAID2.begin(), toupper );
		if ( strAID1 == strAID2 )
		{
			m_strsTemp.str("");
			m_strsTemp<<"<INFO><MF>�µ� "+strTemp1+" �� "+strTemp2+" ������ͬ��AID,���Ϸ�,��ȷ�����е�AID������Ψһ��";
			Append_Error(m_strsTemp.str());
		}
	}
	/*���SSSE��EF05�Ŀ��Ź����Ƿ���ȷ*/
	CheckCardNo();
	/*����籣Ӧ���е�ҽ�Ʊ���2�ļ��Ƿ�����*/
	CheckDF04();

	return ;
}

bool CInformation::CheckMFDatas( string strTgNames )
{
	bool bFlag=true;

	m_strsTemp.str("");
	m_strsTemp<<"<INFO>"<<strTgNames<<" ��ǩ�����ݲ��Ϸ���";
	if ( S_CARDNFO.m_strMKvalue.length()!=32 || !JudgeHex(S_CARDNFO.m_strMKvalue) )
	{
		m_strsTemp<<"MKvalue����Ϊ���ȵ���32λ��ʮ�������ַ�����";
		bFlag=false;
	}
	if ( S_CARDNFO.m_strCount.length()!=2 || !JudgeHex(S_CARDNFO.m_strCount) )
	{
		m_strsTemp<<"Count����Ϊ���ȵ���2λ��ʮ�������ַ�����";
		bFlag=false;
	}
	string strFID = S_CARDNFO.m_strFID;
	transform( strFID.begin(), strFID.end(), strFID.begin(), toupper );
	if ( strFID!="3F00" )
	{
		m_strsTemp<<"FID����Ϊ3F00��";
		bFlag=false;
	}
	string strAID = S_CARDNFO.m_strAID;
	if ( strAID.empty() || !JudgeHex(strAID) )
	{
		m_strsTemp<<"AID����Ϊ�ǿ�ʮ�������ַ�����";
		bFlag=false;
	}
	
	return bFlag;
}

bool CInformation::CheckDDFDatas( string strTgNames, const DDFINFO &struDDFInfo )
{
	bool bFlag=true;

	string strTagName = struDDFInfo.m_strDDFName;
	transform( strTagName.begin(), strTagName.end(), strTagName.begin(), toupper );
	if ( strTagName == "DDF0" )
		return true;

	m_strsTemp.str("");
	m_strsTemp<<"<INFO>"<<strTgNames<<" ��ǩ�����ݲ��Ϸ���";
	if ( struDDFInfo.m_strMKvalue.length()!=32 || !JudgeHex(struDDFInfo.m_strMKvalue) )
	{
		m_strsTemp<<"MKvalue����Ϊ���ȵ���32λ��ʮ�������ַ�����";
		bFlag=false;
	}
	if ( struDDFInfo.m_strCount.length()!=2 || !JudgeHex(struDDFInfo.m_strCount) )
	{
		m_strsTemp<<"Count����Ϊ���ȵ���2λ��ʮ�������ַ�����";
		bFlag=false;
	}
	if ( struDDFInfo.m_strUpdateMode1!="0" && struDDFInfo.m_strUpdateMode1!="1" )
	{
		m_strsTemp<<"UpdateMode1����Ϊ0��1��";
		bFlag=false;
	}
	if ( struDDFInfo.m_strUpdateMode2!="0" && struDDFInfo.m_strUpdateMode2!="1" )
	{
		m_strsTemp<<"UpdateMode2����Ϊ0��1��";
		bFlag=false;
	}
	if ( struDDFInfo.m_strFID.length()!=4 || !JudgeHex(struDDFInfo.m_strFID) )
	{
		m_strsTemp<<"FID����Ϊ���ȵ���4λ��ʮ�������ַ�����";
		bFlag=false;
	}

	string strDDFAID = struDDFInfo.m_strAID;
	transform( strDDFAID.begin(), strDDFAID.end(), strDDFAID.begin(), toupper );
	if ( strTagName == "SSSE" && strDDFAID != "7378312E73682EC9E7BBE1B1A3D5CF" )//SSSE��AIDΪ�̶�ֵ7378312E73682EC9E7BBE1B1A3D5CF
	{
		m_strsTemp<<"SSSE��AID����Ϊ7378312E73682EC9E7BBE1B1A3D5CF��";
		bFlag=false;
	}
	else 
	{
		if ( strDDFAID.empty() || !JudgeHex(strDDFAID) )
		{
			m_strsTemp<<"AID����Ϊ�ǿ�ʮ�������ַ�����";
			bFlag=false;
		}
	}
	
	return bFlag;
}

bool CInformation::CheckADFDatas( string strTgNames, const ADFINFO &struADFInfo )
{
	bool bFlag=true;
	string strFID = struADFInfo.m_strFID;
	string strTagName = struADFInfo.m_strADFName;
	transform( strFID.begin(), strFID.end(), strFID.begin(), toupper );
	transform( strTagName.begin(), strTagName.end(), strTagName.begin(), toupper );
	if ( strTagName == "DF00" )
		return true;

	m_strsTemp.str("");
	m_strsTemp<<"<INFO>"<<strTgNames<<" ��ǩ�����ݲ��Ϸ���";
	if ( struADFInfo.m_strMKvalue.length()!=32 || !JudgeHex(struADFInfo.m_strMKvalue) )
	{
		m_strsTemp<<"MKvalue����Ϊ���ȵ���32λ��ʮ�������ַ�����";
		bFlag=false;
	}
	if ( struADFInfo.m_strCount.length()!=2 || !JudgeHex(struADFInfo.m_strCount) )
	{
		m_strsTemp<<"Count����Ϊ���ȵ���2λ��ʮ�������ַ�����";
		bFlag=false;
	}
	if ( struADFInfo.m_strUpdateMode1!="0" && struADFInfo.m_strUpdateMode1!="1" )
	{
		m_strsTemp<<"UpdateMode1����Ϊ0��1��";
		bFlag=false;
	}
	if ( struADFInfo.m_strUpdateMode2!="0" && struADFInfo.m_strUpdateMode2!="1" )
	{
		m_strsTemp<<"UpdateMode2����Ϊ0��1��";
		bFlag=false;
	}
	if ( strFID.length()!=4 || !JudgeHex(strFID) )
	{
		m_strsTemp<<"FID����Ϊ���ȵ���4λ��ʮ�������ַ�����";
		bFlag=false;
	}
	if ( strFID != strTagName )
	{
		m_strsTemp<<"FID����ͱ�ǩ��һ�£�";
		bFlag=false;
	}
	
	return bFlag;
}

bool CInformation::CheckEFInfor( string strDDFName, string strTgNames, const EFINFO &struEFInfo, string strADFName )
{
	bool bFlag=true,bFlag1,bFlag2;
	bool bFlagEF=false;//���͸���ļ��Ƿ����P1=1�ļ�¼�ı�־ false������ true����
	int nEFType,nCount,nP1,nExP1;
	string strLen="";
	string strTagLable;//B-TLV�������ݵı�ǩ��

	m_strsTemp.str("");
	m_strsTemp<<"<INFO>"<<strTgNames<<"<EF SFI=\""<<struEFInfo.m_strSFI<<"\"> ��ǩ�����ݲ��Ϸ���";
	if ( struEFInfo.m_strSFI.empty() || !JudgeHex(struEFInfo.m_strSFI) )
	{
		m_strsTemp<<"SFI����Ϊ�ǿ�ʮ�������ַ�����";
		bFlag=false;
	}
	if ( !CheckEFControlKey(strDDFName, "ReadControl", struEFInfo.m_strReadControl, strADFName) )
	{
		bFlag=false;
	}
	if ( !CheckEFControlKey(strDDFName, "WriteControl", struEFInfo.m_strWriteControl, strADFName) )
	{
		bFlag=false;
	}
	//EFType:��1�������ļ�	2���䳤�ļ�	3��͸���ļ�	4��ѭ���ļ���
	nEFType = atoi(struEFInfo.m_strEFType.c_str());
	if ( 1 != struEFInfo.m_strEFType.length() || (nEFType<1 || nEFType>4) )
	{
		m_strsTemp<<"EFTypeȡֵ��Χ����Ϊ1��4������";
		bFlag=false;
	}
	if ( "0"!=struEFInfo.m_strbInit && "1"!=struEFInfo.m_strbInit )
	{
		m_strsTemp<<"bInitȡֵֻ��Ϊ0��1��";
		bFlag=false;
	}
	//ѭ���ļ��ж�nCountѭ������
	nCount = atoi(struEFInfo.m_strnCount.c_str());
	if ( "4" == struEFInfo.m_strEFType && nCount<=0 )
	{
		m_strsTemp<<"��ѭ���ļ�nCountȡֵ����Ϊ����0��������";
		bFlag=false;
	}
	//����<EF>��ǩ�����ݼ��,���<EF>��⵽�Ĳ��Ϸ�������Ϣ
	if ( !bFlag )
		Append_Error(m_strsTemp.str());

	//���͸���ļ���¼
	if ( "3" == struEFInfo.m_strEFType )
	{
		if ( struEFInfo.m_vecEFDatas.size() == 0 )//û�м�¼����
		{}
		else if ( struEFInfo.m_vecEFDatas.size() == 1 )//ֻ��һ����¼
		{
			m_strsTemp.str("");
			m_strsTemp<<"<INFO>"<<strTgNames<<"<"<<struEFInfo.m_strSFI<<" P1=\""<<struEFInfo.m_vecEFDatas.front().m_strP1<<"\" Tag=\""<<struEFInfo.m_vecEFDatas.front().m_strTag<<"\"> ��ǩ�����ݲ��Ϸ���";
			if ( !CheckEFDatas( struEFInfo.m_strEFType, struEFInfo.m_vecEFDatas.front() ) )
			{
				Append_Error(m_strsTemp.str());
				bFlag=false;
			}
		}
		else if ( struEFInfo.m_vecEFDatas.size() > 1 )//���ڶ�����¼��ʱ��
		{
			//͸���ļ�ֻ�ܴ���һ����¼����,�ߴ�������
			m_strsTemp.str("");
			//͸���ļ������ڼ�¼��������P1=1������
			vector<EFDATAS>::const_iterator iterEFDatas1 = struEFInfo.m_vecEFDatas.begin();
			while ( iterEFDatas1 != struEFInfo.m_vecEFDatas.end() )
			{
				if ( 1 == HexToDec( (*iterEFDatas1).m_strP1 ) )//����P1=1�ļ�¼
				{
					bFlagEF = true;
					m_strsTemp<<"<INFO>"<<strTgNames<<"<EF SFI=\""<<struEFInfo.m_strSFI<<"\">��͸���ļ����ڶ�����¼<"<<struEFInfo.m_strSFI<<">,Ĭ��ȡP1=1�ļ�¼Ϊ��Ч��¼,������Ч��";
					Append_Warning(m_strsTemp.str());
					//���P1=1�ļ�¼
					m_strsTemp.str("");
					m_strsTemp<<"<INFO>"<<strTgNames<<"<"<<struEFInfo.m_strSFI<<" P1=\""<<(*iterEFDatas1).m_strP1<<"\" Tag=\""<<(*iterEFDatas1).m_strTag<<"\"> ��ǩ�����ݲ��Ϸ���";
					if ( !CheckEFDatas( struEFInfo.m_strEFType, *iterEFDatas1 ) )
					{
						Append_Error(m_strsTemp.str());
						bFlag=false;
					}
					break;
				}
				++iterEFDatas1;
			}
			if ( iterEFDatas1 == struEFInfo.m_vecEFDatas.end() )
			{
				m_strsTemp<<"<INFO>"<<strTgNames<<"<EF SFI=\""<<struEFInfo.m_strSFI<<"\">��͸���ļ����ڶ�����¼<"<<struEFInfo.m_strSFI<<">,Ĭ��ȡ��һ����¼Ϊ��Ч��¼,������Ч��";
				Append_Warning(m_strsTemp.str());
				//���͸���ļ��ĵ�һ����¼
				m_strsTemp.str("");
				m_strsTemp<<"<INFO>"<<strTgNames<<"<"<<struEFInfo.m_strSFI<<" P1=\""<<struEFInfo.m_vecEFDatas.front().m_strP1<<"\" Tag=\""<<struEFInfo.m_vecEFDatas.front().m_strTag<<"\"> ��ǩ�����ݲ��Ϸ���";
				if ( !CheckEFDatas( struEFInfo.m_strEFType, struEFInfo.m_vecEFDatas.front() ) )
				{
					Append_Error(m_strsTemp.str());
					bFlag=false;
				}
			}
		}
	}
	else//����������͵��ļ�
	{
		//����<EF>��ӦSFI�ı�ǩ<SFI>����
		vector<EFDATAS>::const_iterator iterEFDatas = struEFInfo.m_vecEFDatas.begin();
		bFlag2 = true;
		bFlag1 = true;//���ڼ��B-TLV�����Ƿ��Ѿ�����
		nP1=0;//���ڼ�ⶨ���ͱ䳤�ļ���P1�Ƿ���ڴ�1��ʼ������
		while ( iterEFDatas != struEFInfo.m_vecEFDatas.end() )
		{
			//�����ͱ䳤�ļ����P1�Ƿ���ڴ�1��ʼ��������P1�Ƿ����ظ�
			if ( "1"==struEFInfo.m_strEFType || "2"==struEFInfo.m_strEFType )
			{
				//���P1�Ƿ��1��ʼ����
				++nP1;
				vector<EFDATAS>::const_iterator iterEFDatas2 = struEFInfo.m_vecEFDatas.begin();
				while ( iterEFDatas2 != struEFInfo.m_vecEFDatas.end() )
				{
					if ( nP1 == HexToDec( (*iterEFDatas2).m_strP1 ) )//����P1=1	
						break;
					++iterEFDatas2;
				}
				if ( iterEFDatas2 == struEFInfo.m_vecEFDatas.end() )
				{
					m_strsTemp.str("");
					m_strsTemp<<"<INFO>"<<strTgNames<<"��ȱ�� <"<<struEFInfo.m_strSFI<<" P1=\""<<DecToHex(nP1)<<"\"> �ı�ǩ����,��ȷ�ϸ�EF�ļ������������P1�����Ƿ��1��ʼ������";
					Append_Error(m_strsTemp.str());
					bFlag=false;
				}

				//���P1�Ƿ����ظ�
				vector<EFDATAS>::const_iterator iterEFDatas1 = iterEFDatas+1;
				while ( iterEFDatas1 != struEFInfo.m_vecEFDatas.end() )
				{
					if ( (*iterEFDatas).m_strP1 == (*iterEFDatas1).m_strP1 )
					{	
						m_strsTemp.str("");
						m_strsTemp<<"<INFO>"<<strTgNames<<"�´����ظ���P1��ǩ���� <"<<struEFInfo.m_strSFI<<" P1=\""<<(*iterEFDatas1).m_strP1<<"\" Tag=\""<<(*iterEFDatas1).m_strTag<<"\">��";
						Append_Error(m_strsTemp.str());
						bFlag=false;
						break;
					}
					++iterEFDatas1;
				}
			}

			//��ⶨ���ļ������м�¼��Len�Ƿ�һ��
			if (  iterEFDatas == struEFInfo.m_vecEFDatas.begin() )
				strLen = (*iterEFDatas).m_strLen;
			else
			{
				if ( bFlag2 && "1"==struEFInfo.m_strEFType && strLen!=(*iterEFDatas).m_strLen )
				{
					m_strsTemp.str("");
					m_strsTemp<<"<INFO>"<<strTgNames<<"�����ļ�<"<<struEFInfo.m_strSFI<<">���м�¼��Len����һ�£�";
					Append_Error(m_strsTemp.str());
					bFlag=false;
					bFlag2=false;
				}
			}

			//���EF��¼������
			m_strsTemp.str("");
			m_strsTemp<<"<INFO>"<<strTgNames<<"<"<<struEFInfo.m_strSFI<<" P1=\""<<(*iterEFDatas).m_strP1<<"\" Tag=\""<<(*iterEFDatas).m_strTag<<"\"> ��ǩ�����ݲ��Ϸ���";
			if ( !CheckEFDatas( struEFInfo.m_strEFType, *iterEFDatas ) )
			{
				Append_Error(m_strsTemp.str());
				bFlag=false;
			}

			//�����洢B-TLV����ѭ���ļ����ݵĽṹ��EFEXTRADATAS
			nExP1=0;//���ڼ��ѭ���ļ���P1�Ƿ���ڴ�1��ʼ������
			vector<EFEXTRADATAS>::const_iterator iterEFExDatas = (*iterEFDatas).m_vecEFExDatas.begin();
			while ( iterEFExDatas != (*iterEFDatas).m_vecEFExDatas.end() )
			{
				//ѭ���ļ����P1�Ƿ���ڴ�1��ʼ��������P1�Ƿ����ظ�
				if ( "4"==struEFInfo.m_strEFType )
				{
					//���P1�Ƿ��1��ʼ����
					++nExP1;
					vector<EFEXTRADATAS>::const_iterator iterEFExDatas2 = (*iterEFDatas).m_vecEFExDatas.begin();
					while ( iterEFExDatas2 != (*iterEFDatas).m_vecEFExDatas.end() )
					{
						if ( nExP1 == HexToDec( (*iterEFExDatas2).m_strP1 ) )//����P1	
							break;
						++iterEFExDatas2;
					}
					if ( iterEFExDatas2 == (*iterEFDatas).m_vecEFExDatas.end() )
					{
						m_strsTemp.str("");
						m_strsTemp<<"<INFO>"<<strTgNames<<"��ȱ�� <"<<struEFInfo.m_strSFI<<" P1=\""<<DecToHex(nExP1)<<"\"> �ı�ǩ����,��ȷ�ϸ�EF�ļ������������P1�����Ƿ��1��ʼ������";
						Append_Error(m_strsTemp.str());
						bFlag=false;
					}

					//���P1�Ƿ����ظ�
					vector<EFEXTRADATAS>::const_iterator iterEFExDatas1 = iterEFExDatas+1;
					while ( iterEFExDatas1 != (*iterEFDatas).m_vecEFExDatas.end() )
					{
						if ( (*iterEFExDatas).m_strP1 == (*iterEFExDatas1).m_strP1 )
						{	
							m_strsTemp.str("");
							m_strsTemp<<"<INFO>"<<strTgNames<<"�´����ظ���P1��ǩ���� <"<<struEFInfo.m_strSFI<<" P1=\""<<(*iterEFExDatas1).m_strP1<<"\" Tag=\""<<(*iterEFExDatas1).m_strTag<<"\">��";
							Append_Error(m_strsTemp.str());
							bFlag=false;
							break;
						}
						++iterEFExDatas1;
					}
				}

				//B-TLV���ݼ��Tag�Ƿ����ظ�
				if ( "B-TLV"==(*iterEFExDatas).m_strrType )
				{
					vector<EFEXTRADATAS>::const_iterator iterEFExDatas1 = iterEFExDatas+1;
					while ( iterEFExDatas1 != (*iterEFDatas).m_vecEFExDatas.end() )
					{
						if ( (*iterEFExDatas).m_strTag == (*iterEFExDatas1).m_strTag )
						{	
							m_strsTemp.str("");
							m_strsTemp<<"<INFO>"<<strTgNames<<"�´����ظ���Tag��ǩ���� <Tag"<<(*iterEFExDatas1).m_strTag<<" Tag=\""<<(*iterEFExDatas1).m_strTag<<"\">��";
							Append_Error(m_strsTemp.str());
							bFlag=false;
							break;
						}
						++iterEFExDatas1;
					}
				}

				//���EFEXTRADATAS���ݺϷ���
				m_strsTemp.str("");
				m_strsTemp<<"<INFO>"<<strTgNames;
				strTagLable = "Tag"+(*iterEFDatas).m_strTag;
				if ( "4"==struEFInfo.m_strEFType )//ѭ���ļ�
				{
					if ( !CheckEFExDatas( struEFInfo.m_strSFI, strTagLable, struEFInfo.m_strEFType, *iterEFExDatas ) )
					{
						Append_Error(m_strsTemp.str());
						bFlag=false;
					}
				}
				else
				{
					set<string>::const_iterator iterTag = m_setTag.find(strTagLable);
					if ( iterTag==m_setTag.end() )//δ������B-TLV��������,��<TagXX>��ǩ������ֻ���һ��
					{
						bFlag1 = false;
						if ( !CheckEFExDatas( struEFInfo.m_strSFI, strTagLable, struEFInfo.m_strEFType, *iterEFExDatas ) )
						{
							Append_Error(m_strsTemp.str());
							bFlag=false;
						}
					}
				}

				iterEFExDatas++;
			}

			if ( !bFlag1 )
				m_setTag.insert(strTagLable);
			++iterEFDatas;
		}

	}

	return bFlag;
}

bool CInformation::CheckEFControlKey( string strDDFName, string strProperty, string strKey, string strADFName )
{
	string strKey1="",strKey2="",strKeyL="",strKeyR="",strTemp="",strTmpDDF,strTmpADF;
	int nPos;
	int nFlag=0;//���KEY����ʽ ֵΪ0��KEY|KEY��KEY+KEY����ʽ	ֵΪ1��KEY����ʽ
	transform( strKey.begin(), strKey.end(), strKey.begin(), toupper );
	transform( strADFName.begin(), strADFName.end(), strADFName.begin(), toupper );
	transform( strDDFName.begin(), strDDFName.end(), strDDFName.begin(), toupper );

	//������Կ��ȡֵ���ա�PIN��KEY����RKSSSE����PIN|KEY��PIN+KEY����ֹ��FF����
	if ( strKey.empty() || strKey == "FF" )//�ձ�ʾ���ɶ�д��FF��ʾ���ƶ�д
		return true;
	
	//�ָ�KEY|KEY��KEY+KEY(PIN��KEY��һ��)
	nPos=strKey.find('|',0);
	if ( nPos==-1 )//�����ڷָ���|��˵����KEY��KEY+KEY
	{
		nPos=strKey.find('+',0);
		if ( nPos==-1 )//Ҳ�����ڷָ���+��˵����KEY
		{
			strKey1 = strKey;//�洢KEY
			nFlag = 0;
		}
		else
		{
			strKeyL = strKey.substr(0,nPos);//strKeyL ���ϼ�Ŀ¼���Ƶ���Կ
			strKeyR = strKey.substr(nPos+1);//strKeyL �ܵ�ǰĿ¼���Ƶ���Կ
			nFlag = 1;
		}
	}
	else
	{
		strKeyL = strKey.substr(0,nPos);
		strKeyR = strKey.substr(nPos+1);
		nFlag = 1;
	}
	
	if ( strDDFName == "DDF0" && strADFName == "DF00" )//MF��EF�Ŀ�����Կ���
	{
		//����ΪKEY|KEY��KEY+KEY��PIN��PIN��ʾΪ�ϼ�Ŀ¼�Ŀ�����Կ��(��ΪMFû���ϼ�Ŀ¼�����Բ��������ϼ�Ŀ¼���Ƶ���Կ)
		if ( nFlag == 1 || strKey1 == "PIN" )
		{
			m_strsTemp<<strProperty<<"��Կֵֻ��Ϊ�����ɣ��ա�KEY����ֹ��FF����";
			return false;
		}
		else if ( nFlag == 0 )
		{
			if ( !CheckControlKeyExis( strDDFName, strADFName, strKey1 ) )
			{
				m_strsTemp<<strProperty<<"��Կֵ"<<strKey1<<"��<OTHERKEYS><"<<strDDFName<<"><"<<strADFName<<">�в����ڣ�";
				return false;
			}
		}
	}
	else// MF/ADF �� MF/DDF �� MF/DDF/ADF ��EF�Ŀ�����Կ���
	{
		if ( nFlag == 0 )
		{
			if ( strKey1 == "PIN" )//������ϼ�Ŀ¼���Ƶ���Կ
			{
				if ( strDDFName != "DDF0" && strADFName != "DF00" )// MF/DDF/ADF Ҫ���DDF���Ƿ����PIN 
				{
					strTmpDDF = strDDFName;
					strTmpADF = "DF00";
				}
				else// MF/ADF �� MF/DDF Ҫ���MF���Ƿ����PIN
				{
					strTmpDDF = "DDF0";
					strTmpADF = "DF00";
				}

				if ( !CheckControlKeyExis( strTmpDDF, strTmpADF, strKey1 ) )
				{
					if ( strTmpDDF == "SSSE" )
					{
						if ( strTmpADF == "DF00" )
							m_strsTemp<<strProperty<<"��Կֵ"<<strKey1<<"��<SSSEKEYS><SSSE>�в����ڣ�";
						else
							m_strsTemp<<strProperty<<"��Կֵ"<<strKey1<<"��<SSSEKEYS><"<<strTmpADF<<">�в����ڣ�";
					}
					else
						m_strsTemp<<strProperty<<"��Կֵ"<<strKey1<<"��<OTHERKEYS><"<<strTmpDDF<<"><"<<strTmpADF<<">�в����ڣ�";
					return false;
				}
			}
			else// ����ܵ�ǰĿ¼���Ƶ���Կ
			{
				if ( !CheckControlKeyExis( strDDFName, strADFName, strKey1 ) )
				{
					if ( strDDFName == "SSSE" )
					{
						if ( strADFName == "DF00" )
							m_strsTemp<<strProperty<<"��Կֵ"<<strKey1<<"��<SSSEKEYS><SSSE>�в����ڣ�";
						else
							m_strsTemp<<strProperty<<"��Կֵ"<<strKey1<<"��<SSSEKEYS><"<<strADFName<<">�в����ڣ�";
					}
					else
						m_strsTemp<<strProperty<<"��Կֵ"<<strKey1<<"��<OTHERKEYS><"<<strDDFName<<"><"<<strADFName<<">�в����ڣ�";
					return false;
				}
			}
		}
		else if ( nFlag == 1 )
		{
			//KEY|KEY��KEY+KEY���ұߵ�KEY����ΪPIN����ΪPIN��ʾΪ�ϼ�Ŀ¼�Ŀ�����Կ��ֻ�ܴ�������ߵ�ֵ��
			if ( strKeyR == "PIN" )
			{
				m_strsTemp<<strProperty<<"��Կֵ"<<strKey<<"����Ϊ KEY+PIN��KEY|PIN ��������ʽ��";
				return false;
			}

			if ( strDDFName != "DDF0" && strADFName != "DF00" )// MF/DDF/ADF Ҫ���DDF���Ƿ���� ���ϼ�Ŀ¼���Ƶ���Կ
			{
				strTmpDDF = strDDFName;
				strTmpADF = "DF00";
			}
			else// MF/ADF �� MF/DDF Ҫ���MF���Ƿ���� ���ϼ�Ŀ¼���Ƶ���Կ
			{
				strTmpDDF = "DDF0";
				strTmpADF = "DF00";
			}

			if ( !CheckControlKeyExis( strTmpDDF, strTmpADF, strKeyL ) )//������ϼ�Ŀ¼���Ƶ���Կ
			{
				if ( strTmpDDF == "SSSE" )
				{
					if ( strTmpADF == "DF00" )
						m_strsTemp<<strProperty<<"��Կֵ"<<strKey<<"�е�"<<strKeyL<<"��<SSSEKEYS><SSSE>�в����ڣ�";
					else
						m_strsTemp<<strProperty<<"��Կֵ"<<strKey<<"�е�"<<strKeyL<<"��<SSSEKEYS><"<<strTmpADF<<">�в����ڣ�";
				}
				else
					m_strsTemp<<strProperty<<"��Կֵ"<<strKey<<"�е�"<<strKeyL<<"��<OTHERKEYS><"<<strTmpDDF<<"><"<<strTmpADF<<">�в����ڣ�";
				return false;
			}
			if ( !CheckControlKeyExis( strDDFName, strADFName, strKeyR ) )//����ܵ�ǰĿ¼���Ƶ���Կ
			{
				if ( strDDFName == "SSSE" )
				{
					if ( strADFName == "DF00" )
						m_strsTemp<<strProperty<<"��Կֵ"<<strKey<<"�е�"<<strKeyR<<"��<SSSEKEYS><SSSE>�в����ڣ�";
					else
						m_strsTemp<<strProperty<<"��Կֵ"<<strKey<<"�е�"<<strKeyR<<"��<SSSEKEYS><"<<strADFName<<">�в����ڣ�";
				}
				else
					m_strsTemp<<strProperty<<"��Կֵ"<<strKey<<"�е�"<<strKeyR<<"��<OTHERKEYS><"<<strDDFName<<"><"<<strADFName<<">�в����ڣ�";
				return false;
			}
		}

	}

	return true;
}

bool CInformation::CheckControlKeyExis( string strDDF, string strADF, string strKey )
{
	string strTemp;
	transform( strKey.begin(), strKey.end(), strKey.begin(), toupper );
	transform( strADF.begin(), strADF.end(), strADF.begin(), toupper );
	transform( strDDF.begin(), strDDF.end(), strDDF.begin(), toupper );

	//�����ṹ��KEYS������
	vector<KEYS>::const_iterator iterKeys = V_EKEYS.begin();
	while ( iterKeys!=V_EKEYS.end() )
	{
		//��Ѱ����Կ�������Ƿ���ڸ�DDF
		strTemp = (*iterKeys).m_strDDFName;
		transform( strTemp.begin(), strTemp.end(), strTemp.begin(), toupper );
		if ( strTemp != strDDF )
		{
			++iterKeys;
			continue;
		}

		vector<ADFKEY>::const_iterator iterKey = (*iterKeys).m_vecADFKEY.begin();
		while ( iterKey != (*iterKeys).m_vecADFKEY.end() )
		{
			//�ҵ������Ѱ��ADFKEY��Կ���Ƿ����ADF,���ڵĻ��ڵ�ǰADF�¼���Ѱ���Ƿ���ڸÿ�����Կ
			strTemp = (*iterKey).m_strADFName;
			transform( strTemp.begin(), strTemp.end(), strTemp.begin(), toupper );
			if ( strTemp!=strADF )
			{
				++iterKey;
				continue;
			}

			vector<KEYDATAS>::const_iterator iterKeyDatas = (*iterKey).m_vecKeyDatas.begin();
			while ( iterKeyDatas!=(*iterKey).m_vecKeyDatas.end() )
			{
				strTemp = (*iterKeyDatas).m_strName;
				transform( strTemp.begin(), strTemp.end(), strTemp.begin(), toupper );
				if ( strTemp == strKey )//�ҵ���Կ��ƥ��ɹ�
					return true;
				++iterKeyDatas;
			}
			break;
		}
		break;
	}
	return false;
}

bool CInformation::CheckEFDatas( string strEFType, const EFDATAS &struEFDatas )
{
	bool bFlag=true;
	string strTemp = "";
	string strP1 = struEFDatas.m_strP1;
	string strTag = struEFDatas.m_strTag;
	string strLen = struEFDatas.m_strLen;
	string strVal = struEFDatas.m_strVal;
	int nValLen = strVal.length();
	string strType = struEFDatas.m_strrType;
	transform( strType.begin(), strType.end(), strType.begin(), toupper );
	
	//�����ͱ䳤�ļ���P1��Tag�����ȡ�ֵ������
	if ( "1" == strEFType || "2" == strEFType )
	{
		if ( strP1 == "" || strP1.length()>2 || !JudgeHex(strP1) )
		{
			m_strsTemp<<"P1����Ϊ���Ȳ�����2λ�ķǿ�ʮ�������ַ�����";
			bFlag = false;
		}

		if ( strTag == "" || strTag.length()>2 || !JudgeHex(strTag) )
		{
			m_strsTemp<<"Tag����Ϊ���Ȳ�����2λ�ķǿ�ʮ�������ַ�����";
			bFlag = false;
		}

		if ( strLen == "" || !JudgeHex(strLen) )
		{
			m_strsTemp<<"Len����Ϊ�ǿ�ʮ�������ַ�����";
			bFlag = false;
		}

		if ( strVal.compare(0, 3, "CON")==0 )
		{
			strTemp = strVal.substr(3);
			if ( strTemp == "" || !JudgeDec(strTemp) )
			{
				m_strsTemp<<"Val�ĳ�����ʽ����Ϊ��CON+���֡���";
				bFlag = false;
			}
		}
		else if ( strVal != "00" && strVal != "FF" )//00��FF��ʾ������¼û��ʵ������ֻ��Ĭ���������,��ʱ�������
		{
			if ( "AN"!=strType )//��an���ͼ������Val�ĺϷ���
			{
				if ( !JudgeHex(strVal) || nValLen>HexToDec(strLen)*2 )
				{
					m_strsTemp<<"ValӦ��Ϊ�ջ�CON�����򳤶Ȳ�����Len*2��ʮ�������ַ���,�������ɽű�д����ᵼ�¸����¼�����쳣��";
					bFlag = false;
				}
			}
			else//an���ͼ������Val�ĺϷ���
			{
				if ( nValLen>HexToDec(strLen) )
				{
					m_strsTemp<<"ValӦ��Ϊ�ջ�00��CON�����򳤶Ȳ�����Len���ַ���,�������ɽű�д����ᵼ�¸����¼�����쳣��";
					bFlag = false;
				}
			}
		}

		//����(an,cn,b,B-TLV)
		if ( "AN"!=strType && "CN"!=strType && "B"!=strType && "B-TLV"!=strType )
		{
			m_strsTemp<<"rType����Ϊ����(an,cn,b,B-TLV)�е�һ�֣�";
			bFlag = false;
		}
	}
	//͸���ļ���ֵ����ΪCON����,ֻ��Ϊ�ջ�ʮ�������ַ���,ֵ����ֻ����b��������ݳ��ȡ�ֵ
	else if ( "3" == strEFType )
	{
		if ( strLen == "" || !JudgeHex(strLen) )
		{
			m_strsTemp<<"Len����Ϊ�ǿ�ʮ�������ַ�����";
			bFlag = false;
		}

		//����ֻ����b
		if ( "B"!=strType )
		{
			m_strsTemp<<"rType����Ϊ����b��";
			bFlag = false;
		}

		if ( strVal != "00" && strVal != "FF" )//00��FF��ʾ������¼û��ʵ������ֻ��Ĭ���������,��ʱ�������
		{
			if ( !JudgeHex(strVal) || nValLen>HexToDec(strLen)*2 )
			{
				m_strsTemp<<"ValӦ��Ϊ�ջ򳤶Ȳ�����Len*2��ʮ�������ַ���,�������ɽű�д����ᵼ�¸����¼�����쳣��";
				bFlag = false;
			}
		}

	}
	else if ( "4" == strEFType )//ѭ���ļ���EF���������Ϊ���ݶ�������Ex_EF��
		return true;

	return bFlag;
}

bool CInformation::CheckEFExDatas( string strSFI, string strTag, string strEFType, const EFEXTRADATAS &struEFExDatas )
{
	bool bFlag=true;
	string strP1 = struEFExDatas.m_strP1;
	string strExTag = struEFExDatas.m_strTag;
	string strLen = struEFExDatas.m_strLen;
	string strVal = struEFExDatas.m_strVal;
	int nValLen = strVal.length();
	string strType = struEFExDatas.m_strrType;
	transform( strType.begin(), strType.end(), strType.begin(), toupper );

	//ѭ���ļ����ݼ��
	if( "4"==strEFType )
	{
		m_strsTemp<<"<"<<strSFI<<" P1=\""<<struEFExDatas.m_strP1<<"\" Tag=\""<<struEFExDatas.m_strTag<<"\"> ��ǩ�����ݲ��Ϸ���";
		if ( strP1 == "" || strP1.length()>2 || !JudgeHex(strP1) )
		{
			m_strsTemp<<"P1����Ϊ���Ȳ�����2λ�ķǿ�ʮ�������ַ�����";
			bFlag = false;
		}
	}
	else//B-TLV���ݼ��
	{
		m_strsTemp<<"<"<<strTag<<" Tag=\""<<struEFExDatas.m_strTag<<"\" Len=\""<<struEFExDatas.m_strLen<<"\"> ��ǩ�����ݲ��Ϸ���";
		if ( strExTag == "" || strExTag.length()>2 || !JudgeHex(strExTag) )
		{
			m_strsTemp<<"Tag����Ϊ���Ȳ�����2λ�ķǿ�ʮ�������ַ�����";
			bFlag = false;
		}
	}

	/*��ͬ���ݼ��*/
	if ( strLen == "" || !JudgeHex(strLen) )
	{
		m_strsTemp<<"Len����Ϊ�ǿ�ʮ�������ַ�����";
		bFlag = false;
	}

	if ( strVal != "00" && strVal != "FF" )//00��FF��ʾ������¼û��ʵ������ֻ��Ĭ���������,��ʱ�������
	{
		if (  "AN"!=strType )//��an���ͼ������Val�ĺϷ���
		{
			if ( !JudgeHex(strVal) || nValLen>HexToDec(strLen)*2 )
			{
				m_strsTemp<<"ValӦ��Ϊ�ջ򳤶Ȳ�����Len*2��ʮ�������ַ���,�������ɽű�д����ᵼ�¸����¼�����쳣��";
				bFlag = false;
			}
		}
		else//an���ͼ������Val�ĺϷ���
		{
			if ( nValLen>HexToDec(strLen) )
			{
				m_strsTemp<<"ValӦ��Ϊ�ջ�00�򳤶Ȳ�����Len���ַ���,�������ɽű�д����ᵼ�¸����¼�����쳣��";
				bFlag = false;
			}
		}
	}

	//����(an,cn,b)
	if ( "AN"!=strType && "CN"!=strType && "B"!=strType )
	{
		m_strsTemp<<"rType����Ϊ����(an,cn,b)�е�һ�֣�";
		bFlag = false;
	}

	return bFlag;
}

bool CInformation::CheckSETTINGDatas()
{
	bool bFlag=true,bTemp;
	string strPin,strCityCode,strCount,strMode,strType,strScale,strPKeyNo,strPKeyfsjs,strPKeyfsyz,strPKeyLen;

	/*���PIN*/
	m_strsTemp.str("");
	m_strsTemp<<"<INFO><SETTING><PIN> ��ǩ�����ݲ��Ϸ���";
	bTemp=true;
	strPin = S_SETTING.m_struPIN.m_strValue;
	strCount = S_SETTING.m_struPIN.m_strCount;
	if ( strPin.length()<4 || strPin.length()>12 || !JudgeHex(strPin) )
	{
		m_strsTemp<<"Value����Ϊ����2-6�ֽ�ʮ�������ַ�����";
		bTemp = false;
	}
	if ( strCount.length()!=2 || !JudgeHex(strCount) )
	{
		m_strsTemp<<"Count����Ϊ���ȵ���2λ��ʮ�������ַ�����";
		bTemp = false;
	}
	if ( !bTemp )
	{
		Append_Error(m_strsTemp.str());
		bFlag = bTemp;
	}
	if ( strPin.length() != 6 )
	{
		m_strsTemp.str("");
		m_strsTemp<<"<INFO><SETTING><PIN>��ǩ��Valueֵ�ĳ������Ϊ3���ֽڣ�";
		Append_Warning(m_strsTemp.str());
	}

	/*���CITYCODE*/
	m_strsTemp.str("");
	m_strsTemp<<"<INFO><SETTING><CITYCODE> ��ǩ�����ݲ��Ϸ���";
	strCityCode = S_SETTING.m_struCityCode.m_strValue;
	if ( strCityCode.length()/2 != 3 )
	{
		m_strsTemp<<"Value�����ݳ��ȱ���Ϊ3�ֽڣ�";
		Append_Error(m_strsTemp.str());
		bFlag = false;
	}

	/*���LOADKEYMODE Value��װ����Կ�ķ�ʽ 1�����ܻ�װ�� 2���̶���Կװ��*/
	m_strsTemp.str("");
	m_strsTemp<<"<INFO><SETTING><LOADKEYMODE> ��ǩ�����ݲ��Ϸ���";
	strMode = S_SETTING.m_struLoadKeyInfo.m_strMode;
	if ( strMode!="1" && strMode!="2" )
	{
		m_strsTemp<<"Valueȡֵ����Ϊ1����2��";
		Append_Error(m_strsTemp.str());
		bFlag = false;
	}

	/*���STORGEMODE Value	0������	1������	10�����ĸ���	11�����ĸ���*/
	m_strsTemp.str("");
	m_strsTemp<<"<INFO><SETTING><STORGEMODE> ��ǩ�����ݲ��Ϸ���";
	strType = S_SETTING.m_struLoadKeyInfo.m_strType;
	if ( strType!="0" && strType!="1" && strType!="10" && strType!="11" )
	{
		m_strsTemp<<"Valueȡֵ����Ϊ(0��1��10��11)�е�һ�֣�";
		Append_Error(m_strsTemp.str());
		bFlag = false;
	}

	/*���ENCRYPTDEV Scale������	2��ȫʡͨ����ģʽ	1���ؼ��м���ģʽ*/
	m_strsTemp.str("");
	m_strsTemp<<"<INFO><SETTING><ENCRYPTDEV> ��ǩ�����ݲ��Ϸ���";
	strScale = S_SETTING.m_struLoadKeyInfo.m_strScale;
	if ( strScale!="1" && strScale!="2" )
	{
		m_strsTemp<<"Valueȡֵ����Ϊ1����2��";
		Append_Error(m_strsTemp.str());
		bFlag = false;
	}

	/*��LOADKEYMODEΪ1���ܻ�װ�ص�ʱ����LOADSET*/
	if ( strMode == "1" )
	{
		m_strsTemp.str("");
		m_strsTemp<<"<INFO><SETTING><LOADSET> ��ǩ�����ݲ��Ϸ���";
		bTemp=true;
		strPKeyNo = S_SETTING.m_struLoadKeyInfo.m_strPKeyNo;
		strPKeyfsjs = S_SETTING.m_struLoadKeyInfo.m_strPKeyfsjs;
		strPKeyfsyz = S_SETTING.m_struLoadKeyInfo.m_strPKeyfsyz;
		strPKeyLen = S_SETTING.m_struLoadKeyInfo.m_strPKeyLen;
		if ( strPKeyNo.length()>8 || !JudgeHex(strPKeyNo) )
		{
			m_strsTemp<<"PKeyNo����Ϊ���Ȳ�����8λ��ʮ�������ַ�����";
			bTemp = false;
		}
		if ( strPKeyfsjs.length()>8 || !JudgeHex(strPKeyfsjs) )
		{
			m_strsTemp<<"PKeyfsjs����Ϊ���Ȳ�����8λ��ʮ�������ַ�����";
			bTemp = false;
		}
		if ( strPKeyfsyz.length()>8 || !JudgeHex(strPKeyfsyz) )
		{
			m_strsTemp<<"PKeyfsyz����Ϊ���Ȳ�����8λ��ʮ�������ַ�����";
			bTemp = false;
		}
		if ( strPKeyLen!="08" && strPKeyLen!="10" )
		{
			m_strsTemp<<"PKeyLen����Ϊ��08�����ߡ�10��(��λ�ֽ�)��";
			bTemp = false;
		}

		if ( !bTemp )
		{
			Append_Error(m_strsTemp.str());
			bFlag = bTemp;
		}
	}

	return bFlag;
}

bool CInformation::CheckKEYSSDatas()
{
	bool bFlag,bFlag1=true;
	int nPos,nIndex,nGroup,nAddr,nLen,nValLen;
	string strKeyIndex,strPSAMKeyType,strPSAMKeyID,strLen,strGroup,strAddr,steKeyVal;
	
	//����KEYS������
	vector<KEYS>::const_iterator iterKeys = V_EKEYS.begin();
	while ( iterKeys!=V_EKEYS.end() )
	{
		//����<DDF>����
		vector<ADFKEY>::const_iterator iterKey = (*iterKeys).m_vecADFKEY.begin();
		while ( iterKey!=(*iterKeys).m_vecADFKEY.end() )
		{
			vector<KEYDATAS>::const_iterator iterKeyDatas = (*iterKey).m_vecKeyDatas.begin();
			while ( iterKeyDatas!=(*iterKey).m_vecKeyDatas.end() )
			{
				if ( (*iterKeyDatas).m_strName == "PIN" )//PIN��������ݵĺϷ���
				{
					++iterKeyDatas;
					continue;
				}

				bFlag=true;
				m_strsTemp.str("");
				if ( (*iterKeys).m_strDDFName == "SSSE" )
				{
					if ( (*iterKey).m_strADFName == "DF00" )
						m_strsTemp<<"<INFO><SSSEKEYS><SSSE Name=\""<<(*iterKeyDatas).m_strName<<"\" KeyIndex=\""<<(*iterKeyDatas).m_strKeyIndex<<"\">��ǩ�����ݲ��Ϸ���";
					else
						m_strsTemp<<"<INFO><SSSEKEYS><"<<(*iterKey).m_strADFName<<" Name=\""<<(*iterKeyDatas).m_strName<<"\" KeyIndex=\""<<(*iterKeyDatas).m_strKeyIndex<<"\">��ǩ�����ݲ��Ϸ���";
				}
				else
					m_strsTemp<<"<INFO><OTHERKEYS><"<<(*iterKeys).m_strDDFName<<"><"<<(*iterKey).m_strADFName<<" Name=\""<<(*iterKeyDatas).m_strName<<"\" KeyIndex=\""<<(*iterKeyDatas).m_strKeyIndex<<"\">��ǩ�����ݲ��Ϸ���";
			
				/*�����Կ��ʶ*/
				nIndex = 0;
				strKeyIndex = (*iterKeyDatas).m_strKeyIndex;
				nPos = strKeyIndex.find("|");
				if ( nPos == string::npos )
				{
					if ( strKeyIndex.length()!=2 || !JudgeHex(strKeyIndex) )
					{
						m_strsTemp<<"KeyIndex����Ϊ2λʮ�������ַ�����";
						bFlag = false;
					}
				}
				while ( nPos != string::npos )
				{
					m_strTemp = strKeyIndex.substr( 0, nPos );
					if ( m_strTemp.length()!=2 || !JudgeHex(m_strTemp) )
					{
						m_strsTemp<<"KeyIndex���߷ָ����ڵ����ݱ���Ϊ2λʮ�������ַ�����";
						bFlag = false;
						break;
					}
					strKeyIndex=strKeyIndex.substr(nPos+1);
					nPos = strKeyIndex.find("|");
					if ( nPos == string::npos && !strKeyIndex.empty() )
					{
						m_strsTemp<<"KeyIndex�����Էָ���\"|\"��Ϊ��������";
						bFlag = false;
					}
					nIndex++;
				}
			
				/*�����Կ�汾*/
				if ( (*iterKeyDatas).m_strVer.length()!=2 || !JudgeHex((*iterKeyDatas).m_strVer) )
				{
					m_strsTemp<<"Ver����Ϊ���ȵ���2λ��ʮ�������ַ�����";
					bFlag = false;
				}

				//���ܻ���ʽװ�ص�ʱ��ż��PSAM����ʶ����;
				if ( "1"==S_SETTING.m_struLoadKeyInfo.m_strMode )
				{
					/*���psam����Կ��;*/
					strPSAMKeyType = (*iterKeyDatas).m_strPSAMKeyType;
					if ( strPSAMKeyType!="-" && (strPSAMKeyType.length()!=2 || !JudgeHex(strPSAMKeyType)) )
					{
						m_strsTemp<<"PSAMKeyType����Ϊ2λʮ�������ַ������ַ�\"-\"��";
						bFlag = false;
					}
					else
					{
						if ( strPSAMKeyType!="-" )
						{
							if ( CheckPSAMType(strPSAMKeyType) == 1 )
							{
								m_strsTemp<<"PSAMKeyType��ǰֵ��b8-b6����ʾ����Կ��ɢ�������Ϸ���";
								bFlag = false;
							}
							else if ( CheckPSAMType(strPSAMKeyType) == 2 )
							{
								m_strsTemp<<"PSAMKeyType��ǰֵ��b5-b1����ʾ����Կ���Ͳ��Ϸ���";
								bFlag = false;
							}
						}
					}
					/*���psam����Կ��ʶ*/
					strPSAMKeyID = (*iterKeyDatas).m_strPSAMKeyID;
					nPos = strPSAMKeyID.find("|");
					if ( nPos == string::npos )
					{
						if ( strPSAMKeyID!="-" && (strPSAMKeyID.length()!=2 || !JudgeHex(strPSAMKeyID)) )
						{
							m_strsTemp<<"PSAMKeyID����Ϊ2λʮ�������ַ������ַ�\"-\"��";
							bFlag = false;
						}
					}
					while ( nPos != string::npos )
					{
						m_strTemp = strPSAMKeyID.substr( 0, nPos );
						if ( m_strTemp.length()!=2 || !JudgeHex(m_strTemp) )
						{
							m_strsTemp<<"PSAMKeyID���߷ָ����ڵ����ݱ���Ϊ2λʮ�������ַ�����";
							bFlag = false;
							break;
						}
						strPSAMKeyID=strPSAMKeyID.substr(nPos+1);
						nPos = strPSAMKeyID.find("|");
						if ( nPos == string::npos && !strPSAMKeyID.empty() )
						{
							m_strsTemp<<"PSAMKeyID�����Էָ���\"|\"��Ϊ��������";
							bFlag = false;
						}
					}
				}

				/*���Len*/
				strLen = (*iterKeyDatas).m_strLen;
				if (strLen != "08" && strLen != "10")
				{
					m_strsTemp<<"Len��ֵ����Ϊ��08�����ߡ�10��(��λ�ֽ�)��";
					bFlag = false;
				}
				else
				{
					/*�̶���Կװ�ط�ʽ��ʱ������Կֵ���ȱ������Len�ֽڵ�ʮ�������ַ���*/
					if ( "2"==S_SETTING.m_struLoadKeyInfo.m_strMode )
					{
						steKeyVal = (*iterKeyDatas).m_strValue;
						nLen=HexToDec( strLen.c_str() )*2;
						nValLen=steKeyVal.length();
						if ( nValLen!=nLen || !JudgeHex(steKeyVal) )
						{
							m_strsTemp<<"�̶���Կװ��ʱValue����Ϊ���ȵ���"<<strLen<<"�ֽڵ�ʮ�������ַ�����";
							bFlag = false;
						}
					}
				}

				/*�����Կ����*/
				strGroup = (*iterKeyDatas).m_strGroup;
				nPos = strGroup.find("|");
				nGroup=0;
				if ( nPos==string::npos )
				{
					m_strsTemp<<"Group������ȱ�����߷ָ���\"|\"��";
					bFlag = false;
				}
				while ( nPos != string::npos )
				{
					m_strTemp = strGroup.substr( 0, nPos );
					if ( m_strTemp.length()!=2 || !JudgeHex(m_strTemp) )
					{
						m_strsTemp<<"Group���߷ָ����ڵ����ݱ���Ϊ2λʮ�������ַ�����";
						bFlag = false;
						break;
					}
					strGroup=strGroup.substr(nPos+1);
					nPos = strGroup.find("|");
					if ( nPos == string::npos && !strGroup.empty() )
					{
						m_strsTemp<<"Group�����Էָ���\"|\"��Ϊ��������";
						bFlag = false;
					}
					nGroup++;
				}
				if ( nGroup>4 )
				{
					m_strsTemp<<"Group������Ϊ�ָ����������������ܳ���4�飻";
					bFlag = false;
				}
				/*���ܻ�װ����Կ��ʽ��ʱ��ż����ܻ��洢��Կ�ĵ�ַ*/
				if ( "1"==S_SETTING.m_struLoadKeyInfo.m_strMode )
				{
					strAddr = (*iterKeyDatas).m_strAddr;
					nPos = strAddr.find("|");
					nAddr=0;
					if ( nPos==string::npos )
					{
						m_strsTemp<<"Addr������ȱ�����߷ָ���\"|\"��";
						bFlag = false;
					}
					while ( nPos != string::npos )
					{
						m_strTemp = strAddr.substr( 0, nPos );
						if ( m_strTemp=="" || !JudgeHex(m_strTemp) )
						{
							m_strsTemp<<"Addr���߷ָ����ڵ����ݱ���Ϊ�ǿ�ʮ�������ַ�����";
							bFlag = false;
							break;
						}
						strAddr=strAddr.substr(nPos+1);
						nPos = strAddr.find("|");
						if ( nPos == string::npos && !strAddr.empty() )
						{
							m_strsTemp<<"Addr�����Էָ���\"|\"��Ϊ��������";
							bFlag = false;
						}
						nAddr++;
					}
					if ( nGroup != nAddr )
					{
						m_strsTemp<<"(������Ϊ�ָ���������������)Addr�������������Group����������ͬ��";
						bFlag = false;
					}
				}

				/*�����������*/
				if ( (*iterKeyDatas).m_strCount.length()!=2 || !JudgeHex((*iterKeyDatas).m_strCount) )
				{
					m_strsTemp<<"Count����Ϊ���ȵ���2λ��ʮ�������ַ�����";
					bFlag = false;
				}
				/*����Ҫ��ʶ�����Ƿ���ȷ*/
				if ( nIndex>0 && nIndex!=nGroup )//���м�����Կ��ʶ��ʱ����Ҫ��Group����������ͬ
				{
					m_strsTemp<<"(������Ϊ�ָ���������������)KeyIndex�������������Group����������ͬ��";
					bFlag = false;
				}

				if ( !bFlag )
				{
					Append_Error(m_strsTemp.str());
					bFlag1 = bFlag;
				}

				++iterKeyDatas;
			}

			++iterKey;
		}

		++iterKeys;
	}

	return bFlag1;
}

int CInformation::CheckPSAMType( string strPSAMType )
{
	int nDivLv,nKeyType;//��ɢ��������Կ����
	string strTmp;

	//ȡb8-b6��ֵ�жϷ�ɢ����
	nDivLv = atoi( strPSAMType.substr(0,1).c_str() )/2;
	if ( nDivLv<0 || nDivLv>3 )
		return 1;
	
	//ȡb5-b1��ֵ�ж���Կ����
	strTmp = HexToBinary(strPSAMType);
	nKeyType = HexToDec( BinaryToHex(strTmp.substr(3)) );
	if ( nKeyType!=2 && nKeyType!=6 && nKeyType!=7 && nKeyType!=8 && nKeyType!=9 && nKeyType!=12 )
		return 2;

	return 0;
}

bool CInformation::CheckNecessaryKeys()
{
	bool bFlag = false;
	string strKeyName,strDDF,strADF;

	//����KEYS������
	vector<KEYS>::const_iterator iterKeys = V_EKEYS.begin();
	while ( iterKeys!=V_EKEYS.end() )
	{
		strDDF = (*iterKeys).m_strDDFName;
		transform( strDDF.begin(), strDDF.end(), strDDF.begin(), toupper );
		if ( strDDF != "SSSE" )
		{
			++iterKeys;
			continue;
		}

		//����<DDF>����
		vector<ADFKEY>::const_iterator iterKey = (*iterKeys).m_vecADFKEY.begin();
		while ( iterKey!=(*iterKeys).m_vecADFKEY.end() )
		{
			strADF = (*iterKey).m_strADFName;
			transform( strADF.begin(), strADF.end(), strADF.begin(), toupper );
			if ( strADF != "DF00" )
			{
				++iterKey;
				continue;
			}

			vector<KEYDATAS>::const_iterator iterKeyDatas = (*iterKey).m_vecKeyDatas.begin();
			while ( iterKeyDatas!=(*iterKey).m_vecKeyDatas.end() )
			{
				strKeyName = (*iterKeyDatas).m_strName;
				transform( strKeyName.begin(), strKeyName.end(), strKeyName.begin(), toupper );
				if ( strDDF == "SSSE" && strADF == "DF00" && strKeyName == "IRK" )
					bFlag = true;

				++iterKeyDatas;
			}

			++iterKey;
		}

		++iterKeys;
	}

	if ( !bFlag )
		Append_Error( "<SSSEKEYS><SSSE>��ȱ����ԿIRK������ԿΪ������ڵ���Կ��" );

	return bFlag;
}

void CInformation::Append_Error( string strErrMsg )
{
	m_vecErrData.push_back(strErrMsg);
	++ m_nErrorCnt;
}

void CInformation::Append_Warning( string strWarning )
{
	m_vecWrnData.push_back(strWarning);
	++ m_nWarningCnt;
}

int CInformation::Errors()
{
	return m_nErrorCnt;
}

int CInformation::Warnings()
{
	return m_nWarningCnt;
}

void CInformation::WriteXmlDatasLog()
{
	m_strsTemp.str("");
	stringstream strsTmp1,strsTmp2;
	strsTmp1.str("");//�洢SSSEKEYS��Կ����
	strsTmp2.str("");//�洢OTHERKEYS��Կ����

	//�����ȡ��xml���ݵ�log�ļ�
	m_strsTemp<<"<SETTING>"<<endl;
	m_strsTemp<<"	<PROJ "<<S_SETTING.m_struProjInfo.m_strName<<" "<<S_SETTING.m_struProjInfo.m_strDesc<<endl;
	vector<PROJMOD>::const_iterator iterProj =S_SETTING.m_struProjInfo.m_vecProjMod.begin();
	while(iterProj!=S_SETTING.m_struProjInfo.m_vecProjMod.end())
	{
		m_strsTemp<<"		<MOD "<<(*iterProj).m_strAuthor<<" "<<(*iterProj).m_strModify<<" "<<(*iterProj).m_strDate<<" "<<(*iterProj).m_strVer<<"/>"<<endl;
		++iterProj;
	}
	m_strsTemp<<"	<PIN "<<S_SETTING.m_struPIN.m_strValue<<" "<<S_SETTING.m_struPIN.m_strCount<<"/>"<<endl;
	m_strsTemp<<"	<CITYCODE "<<S_SETTING.m_struCityCode.m_strValue<<" "<<S_SETTING.m_struCityCode.m_strProvince<<" "<<S_SETTING.m_struCityCode.m_strCity<<"/>"<<endl;
	m_strsTemp<<"	<LOADKEYMODE "<<S_SETTING.m_struLoadKeyInfo.m_strMode<<"/>"<<endl;
	m_strsTemp<<"	<STORGEMODE "<<S_SETTING.m_struLoadKeyInfo.m_strType<<"/>"<<endl;
	m_strsTemp<<"	<ENCRYPTDEV "<<S_SETTING.m_struLoadKeyInfo.m_strScale<<"/>"<<endl;
	m_strsTemp<<"		<LOADSET "<<S_SETTING.m_struLoadKeyInfo.m_strPKeyNo<<" "<<S_SETTING.m_struLoadKeyInfo.m_strPKeyfsjs<<" "<<S_SETTING.m_struLoadKeyInfo.m_strPKeyfsyz<<" "<<S_SETTING.m_struLoadKeyInfo.m_strPKeyLen<<"/>"<<endl;
	m_strsTemp<<"	</ENCRYPTDEV>"<<endl;
	m_strsTemp<<"</SETTING>"<<endl;

	strsTmp1<<"<SSSEKEYS>"<<endl;
	strsTmp2<<"<OTHERKEYS>"<<endl;
	vector<KEYS>::const_iterator iterKeys = V_EKEYS.begin();
	while ( iterKeys!=V_EKEYS.end() )
	{
		if ( (*iterKeys).m_strDDFName != "SSSE" )
			strsTmp2<<"	<"<<(*iterKeys).m_strDDFName<<">"<<endl;

		vector<ADFKEY>::const_iterator iterKey = (*iterKeys).m_vecADFKEY.begin();
		while ( iterKey!=(*iterKeys).m_vecADFKEY.end() )
		{
			vector<KEYDATAS>::const_iterator iterKeyDatas = (*iterKey).m_vecKeyDatas.begin();
			while ( iterKeyDatas!=(*iterKey).m_vecKeyDatas.end() )
			{
				if ( (*iterKeys).m_strDDFName == "SSSE" )
				{
					if ( (*iterKey).m_strADFName == "DF00" )
						m_strTemp = "SSSE";
					else
						m_strTemp = (*iterKey).m_strADFName;
					strsTmp1<<"		<"<<m_strTemp<<" "<<(*iterKeyDatas).m_strName<<" "<<(*iterKeyDatas).m_strKeyIndex<<" "<<(*iterKeyDatas).m_strVer<<" "<<
						(*iterKeyDatas).m_strPSAMKeyType<<" "<<(*iterKeyDatas).m_strPSAMKeyID<<" "<<(*iterKeyDatas).m_strAddr<<" "<<(*iterKeyDatas).m_strType<<" "<<
						(*iterKeyDatas).m_strLen<<" "<<(*iterKeyDatas).m_strGroup<<" "<<(*iterKeyDatas).m_strCount<<" "<<(*iterKeyDatas).m_strValue<<" "<<(*iterKeyDatas).m_nSeq<<" />"<<endl;
				}
				else
					strsTmp2<<"		<"<<(*iterKey).m_strADFName<<" "<<(*iterKeyDatas).m_strName<<" "<<(*iterKeyDatas).m_strKeyIndex<<" "<<(*iterKeyDatas).m_strVer<<" "<<
					(*iterKeyDatas).m_strPSAMKeyType<<" "<<(*iterKeyDatas).m_strPSAMKeyID<<" "<<(*iterKeyDatas).m_strAddr<<" "<<(*iterKeyDatas).m_strType<<" "<<
					(*iterKeyDatas).m_strLen<<" "<<(*iterKeyDatas).m_strGroup<<" "<<(*iterKeyDatas).m_strCount<<" "<<(*iterKeyDatas).m_strValue<<" "<<(*iterKeyDatas).m_nSeq<<" />"<<endl;
				++iterKeyDatas;
			}
			++iterKey;
		}
		if ( (*iterKeys).m_strDDFName != "SSSE" )
			strsTmp2<<"	<"<<(*iterKeys).m_strDDFName<<"/>"<<endl;
		++iterKeys;
	}
	strsTmp1<<"</SSSEKEYS>"<<endl;
	strsTmp2<<"</OTHERKEYS>"<<endl;
	m_strsTemp<<strsTmp1.str()<<endl<<strsTmp2.str()<<endl;

	m_strsTemp<<"<"<<S_CARDNFO.m_strMFName<<" "<<S_CARDNFO.m_strMKvalue<<" "<<S_CARDNFO.m_strCount<<" "<<S_CARDNFO.m_strFID<<" "<<S_CARDNFO.m_strAID<<" >"<<endl;
	vector<DDFINFO>::const_iterator iterDDF = S_CARDNFO.m_vecDDFInfo.begin();
	while ( iterDDF!=S_CARDNFO.m_vecDDFInfo.end() )//DDF
	{
		m_strsTemp<<"  <"<<(*iterDDF).m_strDDFName<<" "<<(*iterDDF).m_strMKvalue<<" "<<(*iterDDF).m_strCount<<" "<<(*iterDDF).m_strSpace<<" "
			<<(*iterDDF).m_strUpdateMode1<<" "<<(*iterDDF).m_strUpdateMode2<<" "<<(*iterDDF).m_strFID<<" "<<(*iterDDF).m_strAID<<" >"<<endl;

		vector<ADFINFO>::const_iterator iterADF = (*iterDDF).m_vecADFInfo.begin();
		while ( iterADF!=(*iterDDF).m_vecADFInfo.end() )//ADF
		{
			m_strsTemp<<"     <"<<(*iterADF).m_strADFName<<" "<<(*iterADF).m_strMKvalue<<" "<<(*iterADF).m_strCount<<" "<<(*iterADF).m_strUpdateMode1
				<<" "<<(*iterADF).m_strUpdateMode2<<" "<<(*iterADF).m_strFID<<" "<<(*iterADF).m_strAID<<" >"<<endl;

			vector<EFINFO>::const_iterator iterEFInfo = (*iterADF).m_vecEFInfo.begin();
			while ( iterEFInfo!=(*iterADF).m_vecEFInfo.end() )//EFInfor
			{
				m_strsTemp<<"        <EF "<<(*iterEFInfo).m_strSFI<<" "<<(*iterEFInfo).m_strReadControl<<" "<<(*iterEFInfo).m_strWriteControl<<" "<<(*iterEFInfo).m_strEFType
					<<" "<<(*iterEFInfo).m_strbInit<<" "<<(*iterEFInfo).m_strnCount<<" "<<(*iterEFInfo).m_strResSpace<<" />"<<endl;

				vector<EFDATAS>::const_iterator iterEFDatas = (*iterEFInfo).m_vecEFDatas.begin();
				while ( iterEFDatas != (*iterEFInfo).m_vecEFDatas.end() )//EFDatas
				{
					m_strsTemp<<"           <"<<(*iterEFInfo).m_strSFI<<" "<<(*iterEFDatas).m_strP1<<" "<<(*iterEFDatas).m_strTag<<" "<<(*iterEFDatas).m_strLen
						<<" "<<(*iterEFDatas).m_strVal<<" "<<(*iterEFDatas).m_strrType<<" "<<(*iterEFDatas).m_strRemark<<" />"<<endl;

					if ( "4" == (*iterEFInfo).m_strEFType )//ѭ���ļ��Ķ�������
					{
						vector<EFEXTRADATAS>::const_iterator iterEFExDatas = (*iterEFDatas).m_vecEFExDatas.begin();
						while( iterEFExDatas!=(*iterEFDatas).m_vecEFExDatas.end() )
						{
							m_strsTemp<<"              <"<<(*iterEFInfo).m_strSFI<<" "<<(*iterEFExDatas).m_strP1<<" "<<(*iterEFExDatas).m_strTag<<" "<<(*iterEFExDatas).m_strLen
								<<" "<<(*iterEFExDatas).m_strVal<<" "<<(*iterEFExDatas).m_strrType<<" "<<(*iterEFExDatas).m_strRemark<<" />"<<endl;
							++iterEFExDatas;
						}
					}
					else if ( "B-TLV" == (*iterEFDatas).m_strrType )//B-TLV���͵Ķ�������
					{
						vector<EFEXTRADATAS>::const_iterator iterEFExDatas = (*iterEFDatas).m_vecEFExDatas.begin();
						while( iterEFExDatas!=(*iterEFDatas).m_vecEFExDatas.end() )
						{
							m_strsTemp<<"              <Tag"<<(*iterEFDatas).m_strTag<<" "<<(*iterEFExDatas).m_strP1<<" "<<(*iterEFExDatas).m_strTag<<" "<<(*iterEFExDatas).m_strLen<<" "
								<<(*iterEFExDatas).m_strVal<<" "<<(*iterEFExDatas).m_strrType<<" />"<<endl;
							++iterEFExDatas;
						}
					}
					++iterEFDatas;
				}


				++iterEFInfo;
			}

			m_strsTemp<<"     </"<<(*iterADF).m_strADFName<<">"<<endl;
			++iterADF;
		}

		m_strsTemp<<" </"<<(*iterDDF).m_strDDFName<<">"<<endl;
		++iterDDF;
	}

	WriteLog(m_strsTemp.str() ,1);
}

void CInformation::Append_Script( string strDatas )
{
	m_vecScriptData.push_back(strDatas);
}

void CInformation::Clear_Script()
{
	m_vecScriptData.clear();
}

void CInformation::CheckDF04()
{
	string strWarn;
	bool bflag = false,bflag1 = false;
	vector<DDFINFO>::const_iterator iterDDF = S_CARDNFO.m_vecDDFInfo.begin();
	while ( iterDDF!=S_CARDNFO.m_vecDDFInfo.end() )//DDF
	{
		m_strTemp = (*iterDDF).m_strAID;
		transform(m_strTemp.begin(), m_strTemp.end(), m_strTemp.begin(), toupper); 
		if ( "7378312E73682EC9E7BBE1B1A3D5CF" != m_strTemp )
		{
			++iterDDF;
			continue;
		}
		vector<ADFINFO>::const_iterator iterADF = (*iterDDF).m_vecADFInfo.begin();
		while ( iterADF!=(*iterDDF).m_vecADFInfo.end() )//ADF
		{
			m_strTemp = (*iterADF).m_strAID;
			transform(m_strTemp.begin(), m_strTemp.end(), m_strTemp.begin(), toupper); 
			if ( "D15600000503" != m_strTemp )
			{
				++iterADF;
				continue;
			}
			//����籣Ӧ���µ���ᱣ��������2Ŀ¼�ļ��µ��ļ��Ƿ���ڣ������ڵĸ���������Ϣ
			bflag1 = true;//��Ǵ��ڸ�Ŀ¼�ļ�
			for ( int i=0; i<4; ++i )
			{
				bflag = false;
				vector<EFINFO>::const_iterator iterEFInfo = (*iterADF).m_vecEFInfo.begin();
				while ( iterEFInfo!=(*iterADF).m_vecEFInfo.end() )//EFInfor
				{
					m_strTemp = (*iterEFInfo).m_strSFI;
					transform(m_strTemp.begin(), m_strTemp.end(), m_strTemp.begin(), toupper); 
					switch(i)
					{
					case 0:
						if ( "EF05" == m_strTemp )//ҽ�Ʊ��ջ�����Ϣ�ļ�
							bflag = true;
						break;
					case 1:
						if ( "EF06" == m_strTemp )//ҽ�Ʊ�����ʱ����������Ϣ�ļ�
							bflag = true;
						break;
					case 2:
						if ( "EF08" == m_strTemp )//ҽ�ƽ�����ϸ�ļ�
							bflag = true;
						break;
					case 3:
						if ( "EF15" == m_strTemp )//����ҽ�ƽ����¼�ļ�
							bflag = true;
						break;
					default:
						break;
					}
					if ( bflag )//�ҵ���Ӧ���ļ�����ѭ��
						break;
					++iterEFInfo;
				}
				if ( !bflag )//�������ļ��Ļ���������
				{
					strWarn = "<MF><"+(*iterDDF).m_strDDFName+"><"+(*iterADF).m_strADFName+">";
					switch(i)
					{
					case 0:
						strWarn += "��ȱ�ٻ����ļ�EF05(ҽ�Ʊ��ջ�����Ϣ�ļ�),�п��ܻ�Ӱ�쵽ҽ�����ѹ��ܵ�ʹ�ã�";
						Append_Warning(strWarn);
						break;
					case 1:
						strWarn += "��ȱ�ٻ����ļ�EF06(ҽ�Ʊ�����ʱ����������Ϣ�ļ�),�п��ܻ�Ӱ�쵽ҽ�����ѹ��ܵ�ʹ�ã�";
						Append_Warning(strWarn);
						break;
					case 2:
						strWarn += "��ȱ�ٻ����ļ�EF08(ҽ�ƽ�����ϸ�ļ�),�п��ܻ�Ӱ�쵽ҽ�����ѹ��ܵ�ʹ�ã�";
						Append_Warning(strWarn);
						break;
					case 3:
						strWarn += "��ȱ�ٻ����ļ�EF15(����ҽ�ƽ����¼�ļ�),�п��ܻ�Ӱ�쵽ҽ�����ѹ��ܵ�ʹ�ã�";
						Append_Warning(strWarn);
					default:
						break;
					}
				}
			}

			break;
		}

		break;
	}

	if ( !bflag1 )
		Append_Warning("�籣Ӧ��<MF><SSSE>��ȱ�ٶ�ӦAID=\"D15600000503\"��ADF ��ᱣ��������2 �ļ������ᵼ��ҽ�����ѹ����޷�ʹ�ã�");

}


void CInformation::CheckCardNo()
{
	string strCardNo="";
	
	vector<DDFINFO>::const_iterator iterDDF = S_CARDNFO.m_vecDDFInfo.begin();
	while ( iterDDF!=S_CARDNFO.m_vecDDFInfo.end() )//DDF
	{
		m_strTemp = (*iterDDF).m_strAID;
		transform(m_strTemp.begin(), m_strTemp.end(), m_strTemp.begin(), toupper); 
		if ( "7378312E73682EC9E7BBE1B1A3D5CF" != m_strTemp )//��SSSE
		{
			++iterDDF;
			continue;
		}
		vector<ADFINFO>::const_iterator iterADF = (*iterDDF).m_vecADFInfo.begin();
		while ( iterADF!=(*iterDDF).m_vecADFInfo.end() )//ADF
		{
			m_strTemp = (*iterADF).m_strAID;
			if ( "" != m_strTemp )//��DF00
			{
				++iterADF;
				continue;
			}
			
			vector<EFINFO>::const_iterator iterEFInfo = (*iterADF).m_vecEFInfo.begin();
			while ( iterEFInfo!=(*iterADF).m_vecEFInfo.end() )//EFInfor
			{
				m_strTemp = (*iterEFInfo).m_strSFI;
				transform(m_strTemp.begin(), m_strTemp.end(), m_strTemp.begin(), toupper); 
				if ( "EF05" != m_strTemp )//��EF05
				{
					++iterEFInfo;
					continue;
				}

				vector<EFDATAS>::const_iterator iterEFData = (*iterEFInfo).m_vecEFDatas.begin();
				while ( iterEFData!=(*iterEFInfo).m_vecEFDatas.end() )
				{
					m_strTemp = (*iterEFData).m_strTag;
					if ( "07" == m_strTemp )
					{
						//ȡ��������
						strCardNo = (*iterEFData).m_strVal;
						break;
					}
					++iterEFData;
				}
				break;
			}
			break;
		}
		break;
	}
	if ( strCardNo.empty() )//�����ڸ������ݵ�ʱ�򲻼��
		return;

	string strTemp;
	stringstream strsTmp;
	if ( strCardNo.compare(0, 3, "CON")==0 )//�����
		return;
	if ( strCardNo.length() != 9 )//an���Գ���9
	{
		Append_Error("<INFO><MF><SSSE><DF00>��<EF05 Tag=\"07\"/>��Val���ų��ȱ���Ϊ9λ");
		return;
	}
	for( size_t i=0; i<strCardNo.length(); ++i )
	{
		if ( (strCardNo.at(i)<='9' && strCardNo.at(i)>='0') || (strCardNo.at(i)<='Z' && strCardNo.at(i)>='A') )
		{}
		else
		{
			Append_Error("<INFO><MF><SSSE><DF00>��<EF05 Tag=\"07\"/>��Val����ֵ�����ɰ�λ����(���д������ĸ)��һλ����(���д������ĸ)У�������");
			return;
		}
	}

	int nW[9]={3,7,9,10,5,8,4,2};
	int nC=0,nSum=0,nRes;
	char strC,strRes;
	for ( int i=0; i<8; ++i )
	{
		strC = strCardNo.at(i);
		if ( strC<='9' && strC>='0' )
			nC = strC-'0';
		else//��д������ĸ
			nC = strC-'A'+10;
		nSum += nC * nW[i];
	}
	nRes = 11-nSum%11;
	if ( nRes == 10 )
		strRes='X';
	else if ( nRes == 11 )
		strRes = '0';
	else
		strRes = '0'+nRes;

	if (strCardNo.at(8) == strRes)
		return;
	else
	{
		strsTmp.str("");
		strsTmp<<"<INFO><MF><SSSE><DF00>��<EF05 Tag=\"07\"/>��Val����ֵ���һλУ�������,��ȷ�Ŀ���Ϊ:"<<strCardNo.substr(0,8)<<strRes;
		Append_Warning(strsTmp.str());
		return;
	}
}

bool CInformation::ReadXMLDatas_YK( string strXmlPath, string &strLoadKeyStyle, int nCheckLog/*=1 */ )
{
	if ( !CheckXMLFile(strXmlPath) )
		return false;
	ReadSettingDatas(strXmlPath);
	ReadKeysDatas(strXmlPath);
	ModeKeysDatasSeq();
	ReadCardInfo(strXmlPath);
	if ( nCheckLog == 1 )
		WriteXmlDatasLog();
	strLoadKeyStyle = S_SETTING.m_struLoadKeyInfo.m_strMode;//����װ����Կ�ķ�ʽ
	
	return true;
}
