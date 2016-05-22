
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

	doc.load_file(strXmlPath.c_str());//若读取不了中文路径则换位load
	//检测一级标签中<INFO>标签是否存在
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
		m_strTemp = "XML文件“"+strXmlPath+"”中缺失<INFO>标签，该配置不是脚本配置文件，请重新选择文件!";
		Append_Error(m_strTemp);
		return false;
	}

	//检测二级标签中<SETTING>、<SSSEKEYS>、<MF>标签是否存在
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
		m_strTemp = "XML文件“"+strXmlPath+"”中<INFO>标签下缺失标签<SETTING>，该脚本配置文件不完整将无法生成脚本，请重新选择文件!";
		Append_Error(m_strTemp);
	}
	if ( !bFlag2 )
	{
		m_strTemp = "XML文件“"+strXmlPath+"”中<INFO>标签下缺失标签<SSSEKEYS>，该脚本配置文件不完整将无法生成脚本，请重新选择文件!";
		Append_Error(m_strTemp);
	}
	if ( !bFlag3 )
	{
		m_strTemp = "XML文件“"+strXmlPath+"”中<INFO>标签下缺失标签<MF>，该脚本配置文件不完整将无法生成脚本，请重新选择文件!";
		Append_Error(m_strTemp);
	}

	return bFlag1 & bFlag2 & bFlag3;
}

void CInformation::ReadSettingDatas( string strXmlPath )
{
	PROJMOD struProjMod;
	xml_document doc;

	doc.load_file(strXmlPath.c_str());//若读取不了中文路径则换位load
	xml_node setting = doc.child("INFO").child("SETTING");

	//项目信息
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

	//个人识别码
	xml_node pin = setting.child("PIN");
	S_SETTING.m_struPIN.m_strValue = StrDelBlank(pin.attribute("Value").value());
	S_SETTING.m_struPIN.m_strCount = StrDelBlank(pin.attribute("Count").value());

	//城市代码
	xml_node citycode = setting.child("CITYCODE");
	S_SETTING.m_struCityCode.m_strValue = StrDelBlank(citycode.attribute("Value").value());
	S_SETTING.m_struCityCode.m_strProvince = StrDelBlank(citycode.attribute("province").value());
	S_SETTING.m_struCityCode.m_strCity = StrDelBlank(citycode.attribute("city").value());

	//装载密钥相关信息
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
	//兼容0-8字节，1-10字节的代码形式
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

	//添加SSSE的密钥数据
	struKeys.m_strDDFName = "SSSE";
	struKeys.m_vecADFKEY.clear();
	V_EKEYS.push_back(struKeys);

	//遍历标签<SSSEKEYS>里面的密钥数据
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

	//遍历标签<OTHERKEYS>里面的密钥数据
	xml_node keys = doc.child("INFO").child("OTHERKEYS"); 
	for ( xml_node key = keys.first_child(); key; key = key.next_sibling() )
	{
		//遍历KEYS结构体，看看添加的m_strDDFName是否有重复(不区分大小写)，重复的话不添加数据，做出警告提示
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
					m_strsTemp<<"<OTHERKEYS>下只能存在除社保应用<SSSE>之外的其他标签数据,因为已经存在<SSSEKEYS>标签数据,所以<OTHERKEYS>下的<SSSE>标签数据无效";
				else
					m_strsTemp<<"<OTHERKEYS>下有重复标签<"<<strTagDDFName<<">,相同标签情况只以最前面的标签数据为准，其他数据无效";
				Append_Warning(m_strsTemp.str());
				break;
			}
			++iterKeys;
		}
		if ( iterKeys == V_EKEYS.end() )
		{
			//添加新的KEYS数据
			struKeys.m_strDDFName = key.name();
			struKeys.m_vecADFKEY.clear();
			V_EKEYS.push_back(struKeys);//在末尾添加元素

			//遍历标签DDF里的数据
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

	vector<ADFKEY>::iterator iterKey = V_EKEYS.back().m_vecADFKEY.begin();//返回末尾元素的vec的迭代器
	//遍历ADFKEY中的数据，看看当前的标签strTagName是否存在于现有的m_strADFName中
	while ( iterKey != V_EKEYS.back().m_vecADFKEY.end() )
	{
		//密钥ADFKEY中的m_strADFName已存在
		string strVecADFName = StrDelBlank((*iterKey).m_strADFName);
		string strXMLTagName = StrDelBlank(strTagName);
		transform(strVecADFName.begin(), strVecADFName.end(), strVecADFName.begin(), toupper);
		transform(strXMLTagName.begin(), strXMLTagName.end(), strXMLTagName.begin(), toupper);
		if ( strVecADFName == strXMLTagName )
			break;
		++iterKey;
	}
	if( iterKey == V_EKEYS.back().m_vecADFKEY.end() )//向ADFKEY中添加ADF名
	{
		struKey.m_strADFName = StrDelBlank(strTagName);
		struKey.m_vecKeyDatas.clear();
		V_EKEYS.back().m_vecADFKEY.push_back(struKey);
		iterKey = V_EKEYS.back().m_vecADFKEY.end()-1;//迭代器指向刚添加的ADF
	}

	//遍历当前ADF中的KEYDATAS数据，若密钥名称不存在则添加密钥信息
	vector<KEYDATAS>::const_iterator iterKeyData= (*iterKey).m_vecKeyDatas.begin();
	while ( iterKeyData != (*iterKey).m_vecKeyDatas.end() )
	{
		//密钥名称已存在,说明重复密钥信息,不录入数据
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
					m_strsTemp<<"<SSSEKEYS><SSSE>下存在重复密钥<SSSE Name=\""<<strName<<"\" />";
				else
					m_strsTemp<<"<SSSEKEYS><"<<(*iterKey).m_strADFName<<">中存在重复密钥<"<<strTagName<<" Name=\""<<strName<<"\" />";
			}
			else
				m_strsTemp<<"<OTHERKEYS><"<<V_EKEYS.back().m_strDDFName<<"><"<<(*iterKey).m_strADFName<<">中存在重复密钥<"<<strTagName<<" Name=\""<<strName<<"\" />";
			Append_Warning(m_strsTemp.str());
			break;
		}
		++iterKeyData;
	}
	if ( iterKeyData == (*iterKey).m_vecKeyDatas.end() )//添加密钥数据
	{
		struKeyDatas.m_strName = StrDelBlank(strName);
		struKeyDatas.m_strKeyIndex = StrDelBlank(strKeyIndex);
		struKeyDatas.m_strVer = StrDelBlank(strVer);
		struKeyDatas.m_strPSAMKeyType = StrDelBlank(strPSAMKeyType);
		struKeyDatas.m_strPSAMKeyID = StrDelBlank(strPSAMKeyID);
		struKeyDatas.m_strAddr = StrDelBlank(strAddr);
		struKeyDatas.m_strType = StrDelBlank(strType);
		struKeyDatas.m_strLen = StrDelBlank(strLen);
		//兼容用十进制数16表示长度的形式
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

	//遍历KEYS的数据
	vector<KEYS>::iterator iterKeys = V_EKEYS.begin();
	while ( iterKeys!=V_EKEYS.end() )
	{
		//遍历<DDF>数据
		vector<ADFKEY>::iterator iterKey = (*iterKeys).m_vecADFKEY.begin();
		while ( iterKey!=(*iterKeys).m_vecADFKEY.end() )
		{
			//nSeq = 1;
			strDDF = (*iterKeys).m_strDDFName;
			strADF = (*iterKey).m_strADFName;
			transform( strDDF.begin(), strDDF.end(), strDDF.begin(), toupper );
			transform( strADF.begin(), strADF.end(), strADF.begin(), toupper );
			if ( "SSSE" == strDDF && "DF00" == strADF )//SSSE下添加PIN
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
			//添加安全级别
			//vector<KEYDATAS>::iterator iterKeyDatas = (*iterKey).m_vecKeyDatas.begin();
			//while ( iterKeyDatas!=(*iterKey).m_vecKeyDatas.end() )
			//{
			//	if ( nSeq > 15 )//安全状态超出范围11-FF
			//	{
			//		strsTmp.str("");
			//		strsTmp << "<KESYS><" <<(*iterKeys).m_strDDFName << ">下的<" << (*iterKey).m_strADFName << ">密钥数量超过16个导致无法分配到密钥的安全状态";
			//		Append_Error( strsTmp.str() );
			//		break;
			//	}
			//	(*iterKeyDatas).m_nSeq = nSeq;

			//	++nSeq;
			//	++iterKeyDatas;
			//}
			//if ( "SSSE" == strDDF && "DF00" == strADF )//SSSE下添加PIN的密钥数据
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
	long lLen;//用于计算文件的总长度
	int nP;//用于记录TAG的P1

	doc.load_file(strXmlPath.c_str());
	xml_node mf = doc.child("INFO").child("MF");
	S_CARDNFO.m_strMFName =  StrDelBlank(mf.name());//标签名
	S_CARDNFO.m_strMKvalue = StrDelBlank(mf.attribute("MKvalue").value());//属性值
	S_CARDNFO.m_strCount = StrDelBlank(mf.attribute("Count").value());
	S_CARDNFO.m_strFID = StrDelBlank(mf.attribute("FID").value());
	S_CARDNFO.m_strAID = StrDelBlank(mf.attribute("AID").value());

	S_CARDNFO.m_vecDDFInfo.clear();
	//遍历MF下的DDF如SSSE、PSE等
	for ( xml_node ddf = mf.first_child(); ddf; ddf = ddf.next_sibling() )
	{
		/*遍历检测DDF标签数据是否存在于vector，存在的话说明xml脚本中DDF标签有重复*/
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
				m_strsTemp<<"<MF> 下有重复的DDF <"<<ddf.name()<<">,相同标签情况只以最前面的标签数据为准，其他数据无效";
				Append_Warning(m_strsTemp.str());
				break;
			}
			++iterJudgeDDF;
		}
		if ( bFlag )
		{
			//有重复的DDF，不插入数据，继续循环
			continue;
		}

		//插入新数据到数据结构中
		struDDFInfo.m_strDDFName = StrDelBlank(ddf.name());
		struDDFInfo.m_strMKvalue = StrDelBlank(ddf.attribute("MKvalue").value());
		struDDFInfo.m_strCount = StrDelBlank(ddf.attribute("Count").value());
		struDDFInfo.m_strSpace = StrDelBlank(ddf.attribute("Space").value());
		struDDFInfo.m_strUpdateMode1 = StrDelBlank(ddf.attribute("UpdateMode1").value());
		struDDFInfo.m_strUpdateMode2 = StrDelBlank(ddf.attribute("UpdateMode2").value());
		struDDFInfo.m_strFID = StrDelBlank(ddf.attribute("FID").value());
		struDDFInfo.m_strAID = StrDelBlank(ddf.attribute("AID").value());

		struDDFInfo.m_vecADFInfo.clear();
		//遍历当前DDF下的ADF
		for ( xml_node adf = ddf.first_child(); adf; adf = adf.next_sibling() )
		{
			/*检测当前DDF下的ADF标签数据在evctor中是否已存在,存在则表示XML中有重复的标签数据*/
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
					m_strsTemp<<"<MF><"<<ddf.name()<<"> 下有重复的ADF <"<<adf.name()<<">,相同标签情况只以最前面的标签数据为准，其他数据无效";
					Append_Warning(m_strsTemp.str());
					break;
				}
				++iterJudgeADF;
			}
			if ( bFlag )
			{
				//有重复的ADF，不插入数据，继续循环
				continue;
			}

			//插入新数据到数据结构中
			struADFInfo.m_strADFName = StrDelBlank(adf.name());
			struADFInfo.m_strMKvalue = StrDelBlank(adf.attribute("MKvalue").value());
			struADFInfo.m_strCount = StrDelBlank(adf.attribute("Count").value());
			struADFInfo.m_strUpdateMode1 = StrDelBlank(adf.attribute("UpdateMode1").value());
			struADFInfo.m_strUpdateMode2 = StrDelBlank(adf.attribute("UpdateMode2").value());
			struADFInfo.m_strFID = StrDelBlank(adf.attribute("FID").value());
			struADFInfo.m_strAID = StrDelBlank(adf.attribute("AID").value());

			struADFInfo.m_vecEFInfo.clear();
			//遍历当前EF下的EFINFO
			xml_named_node_iterator efinfo = adf.children("EF").begin();
			while ( efinfo != adf.children("EF").end() )
			{
				if ( (*efinfo).attribute("EFType").value() == "5" )
				{
					++efinfo;
					continue;
				}
				/*检测当前ADF下的EF标签数据SFI在evctor中是否已存在,存在则表示XML中有重复的标签数据*/
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
						m_strsTemp<<"<MF><"<<ddf.name()<<"><"<<adf.name()<<"> 下有重复的 <EF SFI=\""<<(*efinfo).attribute("SFI").value()<<"\">,相同标签情况只以最前面的标签数据为准，其他数据无效";
						Append_Warning(m_strsTemp.str());
						break;
					}
					++iterJudgeEF;
				}
				if ( bFlag )
				{
					//EF中有重复的SFI，不插入数据，继续循环
					++efinfo;
					continue;
				}

				//插入新数据到数据结构中
				struEFInfo.m_strSFI = StrDelBlank((*efinfo).attribute("SFI").value());
				struEFInfo.m_strReadControl = StrDelBlank((*efinfo).attribute("ReadControl").value());
				struEFInfo.m_strWriteControl = StrDelBlank((*efinfo).attribute("WriteControl").value());
				struEFInfo.m_strEFType = StrDelBlank((*efinfo).attribute("EFType").value());
				struEFInfo.m_strbInit = StrDelBlank((*efinfo).attribute("bInit").value());
				struEFInfo.m_strnCount = StrDelBlank((*efinfo).attribute("nCount").value());
				struEFInfo.m_strResSpace = StrDelBlank((*efinfo).attribute("ResSpace").value());

				struEFInfo.m_vecEFDatas.clear();
				//遍历m_strSFI标签对应的数据
				string strSFI = StrDelBlank((*efinfo).attribute("SFI").value());
				xml_named_node_iterator efdatas = adf.children(strSFI.c_str()).begin();
				while( efdatas!=adf.children(strSFI.c_str()).end() )
				{
					string rType = StrDelBlank((*efdatas).attribute("rType").value());
					transform(rType.begin(),rType.end(),rType.begin(),toupper);
					string strEFType = StrDelBlank((*efinfo).attribute("EFType").value());
					if( "4" == strEFType )//循环文件
					{
						struEFDatas.m_strP1 = "";
						struEFDatas.m_strTag = "-";
						struEFDatas.m_strVal = "";
						struEFDatas.m_strrType = "-";
						struEFDatas.m_strRemark = "";
						struEFDatas.m_vecEFExDatas.clear();
						lLen=0;
						//循环遍历该EF的所有循环数据并保存到额外数据结构中
						while( efdatas!=adf.children(strSFI.c_str()).end() )
						{
							struEFExtra.m_strP1 = StrDelBlank((*efdatas).attribute("P1").value());
							struEFExtra.m_strTag = StrDelBlank((*efdatas).attribute("Tag").value());
							struEFExtra.m_strLen = StrDelBlank((*efdatas).attribute("Len").value());
							struEFExtra.m_strVal = StrDelBlank((*efdatas).attribute("Val").value());
							struEFExtra.m_strrType = StrDelBlank((*efdatas).attribute("rType").value());
							struEFExtra.m_strRemark = StrDelBlank((*efdatas).attribute("remark").value());
							lLen += HexToDec(struEFExtra.m_strLen);
							//保存EF额外数据结构
							struEFDatas.m_vecEFExDatas.push_back(struEFExtra);
							++efdatas;
						}
						struEFDatas.m_strLen = DecToHex(lLen);//循环文件的总长度

						//保存EF数据结构
						struEFInfo.m_vecEFDatas.push_back(struEFDatas);
						break;
					}
					else if( "B-TLV" == rType )//B-TLV数据还需要存储到EFEXTRADATAS中
					{
						struEFDatas.m_strP1 = StrDelBlank((*efdatas).attribute("P1").value());
						struEFDatas.m_strTag = StrDelBlank((*efdatas).attribute("Tag").value());
						struEFDatas.m_strVal = "";
						struEFDatas.m_strrType = "B-TLV";
						struEFDatas.m_strRemark = StrDelBlank((*efdatas).attribute("remark").value());
						struEFDatas.m_vecEFExDatas.clear();
						//遍历标签Tag+(*efdatas).attribute("Tag").value()的标签数据
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
							//保存EF额外数据结构
							struEFDatas.m_vecEFExDatas.push_back(struEFExtra);
							++nP;
							++efextra;
						}
						struEFDatas.m_strLen = DecToHex(lLen);//B-TLV文件的长度
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
					//保存EF数据结构
					struEFInfo.m_vecEFDatas.push_back(struEFDatas);

					++efdatas;
				}
				//保存ADF数据结构
				struADFInfo.m_vecEFInfo.push_back(struEFInfo);

				++efinfo;
			}
			//保存DDF数据结构
			struDDFInfo.m_vecADFInfo.push_back(struADFInfo);
		}
		//保存MF数据结构
		S_CARDNFO.m_vecDDFInfo.push_back(struDDFInfo);
	}
}

void CInformation::CheckXmlDatas()
{
	string strTgNames="",strDDFName,strTemp1,strTemp2,strAID1,strAID2,strTmp;
	vector<pair<string,string>> vecAID;//<DDF或ADF的名称, AID>

	/*检测<SETTING>标签内数据合法性*/
	CheckSETTINGDatas();
	/*检测<SSSEKEYS>和<OTHERKEYS>标签内数据合法性*/
	CheckKEYSSDatas();
	/*检测<SSSEKEYS>和<OTHERKEYS>标签内某些必须的密钥是否都存在*/
	CheckNecessaryKeys();

	/*检测<MF>标签内数据合法性*/
	strTgNames = "<"+S_CARDNFO.m_strMFName+">";
	if ( S_CARDNFO.m_vecDDFInfo.size() == 0 )
	{
		m_strsTemp.str("");
		m_strsTemp<<"<INFO><MF>标签下的数据为空,建脚本数据不完整,请补全XML数据";
		Append_Error(m_strsTemp.str());
		return;
	}
	else
	{
		vector<DDFINFO>::const_iterator iterDDFTemp = S_CARDNFO.m_vecDDFInfo.begin();
		while( iterDDFTemp!=S_CARDNFO.m_vecDDFInfo.end() )//检测是否存在社保应用<SSSE>
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
			m_strsTemp<<"<INFO><MF>标签下缺少社保应用<SSSE>数据,无法建社保应用,请补全XML数据";
			Append_Error(m_strsTemp.str());
			return;
		}
	}

	if( !CheckMFDatas( strTgNames ) )
	{
		Append_Error(m_strsTemp.str());
	}
	//遍历从xml读取出来<MF>标签内的数据
	vector<DDFINFO>::const_iterator iterDDF = S_CARDNFO.m_vecDDFInfo.begin();
	while ( iterDDF!=S_CARDNFO.m_vecDDFInfo.end() )//遍历DDF
	{
		//遍历DDF,检测DDF的FID是否有重复(不区分大小写)
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
				m_strsTemp<<"<INFO><"+S_CARDNFO.m_strMFName+">下的 <"+(*iterDDF).m_strDDFName+"> 和 <"+(*iterDDF1).m_strDDFName+"> 存在相同的FID,不合法";
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
			m_strsTemp<<"<INFO>"<<strTgNames<<"标签下的数据为空,生成建卡脚本会导致缺失该DDF下的文件数据";
			Append_Warning(m_strsTemp.str());
		}
		if ( !CheckDDFDatas( strTgNames, *iterDDF ) )
		{
			Append_Error(m_strsTemp.str());
		}

		vector<ADFINFO>::const_iterator iterADF = (*iterDDF).m_vecADFInfo.begin();
		while ( iterADF!=(*iterDDF).m_vecADFInfo.end() )//遍历ADF
		{
			//遍历ADF,检测当前DDF下的ADF的FID是否有重复(不区分大小写)
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
					m_strsTemp<<"<INFO><"+S_CARDNFO.m_strMFName+"><"+(*iterDDF).m_strDDFName+">下的 <"+(*iterADF).m_strADFName+"> 和 <"+(*iterADF1).m_strADFName+"> 存在相同的FID,不合法";
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
				m_strsTemp<<"<INFO>"<<strTgNames<<"标签下的数据为空,生成建卡脚本会导致缺失该ADF下的EF文件数据";
				Append_Warning(m_strsTemp.str());
			}
			if ( !CheckADFDatas( strTgNames, *iterADF ) )
			{
				Append_Error(m_strsTemp.str());
			}

			m_setTag.clear();//同一ADF下的B-TLV公用数据只检测一遍
			vector<EFINFO>::const_iterator iterEFInfo = (*iterADF).m_vecEFInfo.begin();
			while ( iterEFInfo!=(*iterADF).m_vecEFInfo.end() )//遍历EFInfor
			{
				strTgNames = "<"+S_CARDNFO.m_strMFName+"><"+(*iterDDF).m_strDDFName+"><"+(*iterADF).m_strADFName+">";
				if ( (*iterEFInfo).m_vecEFDatas.size() == 0 && (*iterEFInfo).m_strSFI != "" && (*iterEFInfo).m_strEFType != "3" )//透明文件可允许不存在记录所以不检测
				{
					m_strsTemp.str("");
					m_strsTemp<<"<INFO>"<<strTgNames<<"下缺少<"<<(*iterEFInfo).m_strSFI<<">标签数据,生成建卡脚本会导致缺失该EF文件的记录";
					Append_Warning(m_strsTemp.str());
				}
				//如果EF不存在记录的话，如果没有预留空间的话警告
				if ( (*iterEFInfo).m_vecEFDatas.size() == 0 && (*iterEFInfo).m_strSFI != "" && HexToDec((*iterEFInfo).m_strResSpace)<=0 )
				{
					m_strsTemp.str("");
					m_strsTemp<<"<INFO>"<<strTgNames<<"<EF SFI=\""<<(*iterEFInfo).m_strSFI<<"\">因为该文件缺失文件记录，所以预留空间Space字段值应该大于零才能建有效的文件";
					Append_Warning(m_strsTemp.str());
				}
				CheckEFInfor( (*iterDDF).m_strDDFName, strTgNames, *iterEFInfo, (*iterADF).m_strADFName );

				vector<EFDATAS>::const_iterator iterEFDatas = (*iterEFInfo).m_vecEFDatas.begin();
				while ( iterEFDatas != (*iterEFInfo).m_vecEFDatas.end() )//EFDatas
				{
					if ( "4" == (*iterEFInfo).m_strEFType )//检测是否存在循环文件的记录数据
					{
						if ( (*iterEFDatas).m_vecEFExDatas.size() == 0 )
						{
							m_strsTemp.str("");
							m_strsTemp<<"<INFO>"<<strTgNames<<"下缺少<"<<(*iterEFInfo).m_strSFI<<">标签数据,生成建卡脚本会导致缺失该EF文件的记录";
							Append_Warning(m_strsTemp.str());
						}
						//如果EF不存在记录的话，如果没有预留空间的话警告
						if ( (*iterEFDatas).m_vecEFExDatas.size() == 0 && HexToDec((*iterEFInfo).m_strResSpace)<=0 )
						{
							m_strsTemp.str("");
							m_strsTemp<<"<INFO>"<<strTgNames<<"<EF SFI=\""<<(*iterEFInfo).m_strSFI<<"\">因为该文件缺失文件记录，所以预留空间应该大于零才能建有效的文件";
							Append_Warning(m_strsTemp.str());
						}
					}
					else if ( "B-TLV" == (*iterEFDatas).m_strrType )//检测是否存在B-TLV类型的记录数据
					{
						if ( (*iterEFDatas).m_vecEFExDatas.size() == 0 )
						{
							m_strsTemp.str("");
							m_strsTemp<<"<INFO>"<<strTgNames<<"下缺少<Tag"<<(*iterEFDatas).m_strTag<<">标签数据,生成建卡脚本会导致该ADF下文件<"<<(*iterEFInfo).m_strSFI<<" P1=\""<<(*iterEFDatas).m_strP1<<"\">的记录为空";
							Append_Warning(m_strsTemp.str());
						}
						//如果EF不存在记录的话，如果没有预留空间的话警告
						if ( (*iterEFDatas).m_vecEFExDatas.size() == 0 && HexToDec((*iterEFInfo).m_strResSpace)<=0 )
						{
							m_strsTemp.str("");
							m_strsTemp<<"<INFO>"<<strTgNames<<"<EF SFI=\""<<(*iterEFInfo).m_strSFI<<"\">因为该文件缺失文件记录，所以预留空间应该大于零才能建有效的文件";
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

	sort( vecAID.begin(), vecAID.end(), strHEX );//升序排序
	//遍历vecAID,检测当前vecAID下的AID是否有重复(不区分大小写)(排序后的数据只需要前后数据对比就知道有没有重复了)
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
			m_strsTemp<<"<INFO><MF>下的 "+strTemp1+" 和 "+strTemp2+" 存在相同的AID,不合法,请确保所有的AID必须是唯一的";
			Append_Error(m_strsTemp.str());
		}
	}
	/*检测SSSE下EF05的卡号构成是否正确*/
	CheckCardNo();
	/*检测社保应用中的医疗保险2文件是否完整*/
	CheckDF04();

	return ;
}

bool CInformation::CheckMFDatas( string strTgNames )
{
	bool bFlag=true;

	m_strsTemp.str("");
	m_strsTemp<<"<INFO>"<<strTgNames<<" 标签内数据不合法：";
	if ( S_CARDNFO.m_strMKvalue.length()!=32 || !JudgeHex(S_CARDNFO.m_strMKvalue) )
	{
		m_strsTemp<<"MKvalue必须为长度等于32位的十六进制字符串；";
		bFlag=false;
	}
	if ( S_CARDNFO.m_strCount.length()!=2 || !JudgeHex(S_CARDNFO.m_strCount) )
	{
		m_strsTemp<<"Count必须为长度等于2位的十六进制字符串；";
		bFlag=false;
	}
	string strFID = S_CARDNFO.m_strFID;
	transform( strFID.begin(), strFID.end(), strFID.begin(), toupper );
	if ( strFID!="3F00" )
	{
		m_strsTemp<<"FID必须为3F00；";
		bFlag=false;
	}
	string strAID = S_CARDNFO.m_strAID;
	if ( strAID.empty() || !JudgeHex(strAID) )
	{
		m_strsTemp<<"AID必须为非空十六进制字符串；";
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
	m_strsTemp<<"<INFO>"<<strTgNames<<" 标签内数据不合法：";
	if ( struDDFInfo.m_strMKvalue.length()!=32 || !JudgeHex(struDDFInfo.m_strMKvalue) )
	{
		m_strsTemp<<"MKvalue必须为长度等于32位的十六进制字符串；";
		bFlag=false;
	}
	if ( struDDFInfo.m_strCount.length()!=2 || !JudgeHex(struDDFInfo.m_strCount) )
	{
		m_strsTemp<<"Count必须为长度等于2位的十六进制字符串；";
		bFlag=false;
	}
	if ( struDDFInfo.m_strUpdateMode1!="0" && struDDFInfo.m_strUpdateMode1!="1" )
	{
		m_strsTemp<<"UpdateMode1必须为0或1；";
		bFlag=false;
	}
	if ( struDDFInfo.m_strUpdateMode2!="0" && struDDFInfo.m_strUpdateMode2!="1" )
	{
		m_strsTemp<<"UpdateMode2必须为0或1；";
		bFlag=false;
	}
	if ( struDDFInfo.m_strFID.length()!=4 || !JudgeHex(struDDFInfo.m_strFID) )
	{
		m_strsTemp<<"FID必须为长度等于4位的十六进制字符串；";
		bFlag=false;
	}

	string strDDFAID = struDDFInfo.m_strAID;
	transform( strDDFAID.begin(), strDDFAID.end(), strDDFAID.begin(), toupper );
	if ( strTagName == "SSSE" && strDDFAID != "7378312E73682EC9E7BBE1B1A3D5CF" )//SSSE的AID为固定值7378312E73682EC9E7BBE1B1A3D5CF
	{
		m_strsTemp<<"SSSE的AID必须为7378312E73682EC9E7BBE1B1A3D5CF；";
		bFlag=false;
	}
	else 
	{
		if ( strDDFAID.empty() || !JudgeHex(strDDFAID) )
		{
			m_strsTemp<<"AID必须为非空十六进制字符串；";
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
	m_strsTemp<<"<INFO>"<<strTgNames<<" 标签内数据不合法：";
	if ( struADFInfo.m_strMKvalue.length()!=32 || !JudgeHex(struADFInfo.m_strMKvalue) )
	{
		m_strsTemp<<"MKvalue必须为长度等于32位的十六进制字符串；";
		bFlag=false;
	}
	if ( struADFInfo.m_strCount.length()!=2 || !JudgeHex(struADFInfo.m_strCount) )
	{
		m_strsTemp<<"Count必须为长度等于2位的十六进制字符串；";
		bFlag=false;
	}
	if ( struADFInfo.m_strUpdateMode1!="0" && struADFInfo.m_strUpdateMode1!="1" )
	{
		m_strsTemp<<"UpdateMode1必须为0或1；";
		bFlag=false;
	}
	if ( struADFInfo.m_strUpdateMode2!="0" && struADFInfo.m_strUpdateMode2!="1" )
	{
		m_strsTemp<<"UpdateMode2必须为0或1；";
		bFlag=false;
	}
	if ( strFID.length()!=4 || !JudgeHex(strFID) )
	{
		m_strsTemp<<"FID必须为长度等于4位的十六进制字符串；";
		bFlag=false;
	}
	if ( strFID != strTagName )
	{
		m_strsTemp<<"FID必须和标签名一致；";
		bFlag=false;
	}
	
	return bFlag;
}

bool CInformation::CheckEFInfor( string strDDFName, string strTgNames, const EFINFO &struEFInfo, string strADFName )
{
	bool bFlag=true,bFlag1,bFlag2;
	bool bFlagEF=false;//标记透明文件是否存在P1=1的记录的标志 false不存在 true存在
	int nEFType,nCount,nP1,nExP1;
	string strLen="";
	string strTagLable;//B-TLV公共数据的标签名

	m_strsTemp.str("");
	m_strsTemp<<"<INFO>"<<strTgNames<<"<EF SFI=\""<<struEFInfo.m_strSFI<<"\"> 标签内数据不合法：";
	if ( struEFInfo.m_strSFI.empty() || !JudgeHex(struEFInfo.m_strSFI) )
	{
		m_strsTemp<<"SFI必须为非空十六进制字符串；";
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
	//EFType:（1：定长文件	2：变长文件	3：透明文件	4：循环文件）
	nEFType = atoi(struEFInfo.m_strEFType.c_str());
	if ( 1 != struEFInfo.m_strEFType.length() || (nEFType<1 || nEFType>4) )
	{
		m_strsTemp<<"EFType取值范围必须为1～4整数；";
		bFlag=false;
	}
	if ( "0"!=struEFInfo.m_strbInit && "1"!=struEFInfo.m_strbInit )
	{
		m_strsTemp<<"bInit取值只能为0或1；";
		bFlag=false;
	}
	//循环文件判断nCount循环次数
	nCount = atoi(struEFInfo.m_strnCount.c_str());
	if ( "4" == struEFInfo.m_strEFType && nCount<=0 )
	{
		m_strsTemp<<"该循环文件nCount取值必须为大于0的整数；";
		bFlag=false;
	}
	//结束<EF>标签的内容检测,输出<EF>检测到的不合法数据信息
	if ( !bFlag )
		Append_Error(m_strsTemp.str());

	//检测透明文件记录
	if ( "3" == struEFInfo.m_strEFType )
	{
		if ( struEFInfo.m_vecEFDatas.size() == 0 )//没有记录数据
		{}
		else if ( struEFInfo.m_vecEFDatas.size() == 1 )//只有一条记录
		{
			m_strsTemp.str("");
			m_strsTemp<<"<INFO>"<<strTgNames<<"<"<<struEFInfo.m_strSFI<<" P1=\""<<struEFInfo.m_vecEFDatas.front().m_strP1<<"\" Tag=\""<<struEFInfo.m_vecEFDatas.front().m_strTag<<"\"> 标签内数据不合法：";
			if ( !CheckEFDatas( struEFInfo.m_strEFType, struEFInfo.m_vecEFDatas.front() ) )
			{
				Append_Error(m_strsTemp.str());
				bFlag=false;
			}
		}
		else if ( struEFInfo.m_vecEFDatas.size() > 1 )//存在多条记录的时候
		{
			//透明文件只能存在一条记录数据,冗错处理数据
			m_strsTemp.str("");
			//透明文件若存在记录则必须存在P1=1的数据
			vector<EFDATAS>::const_iterator iterEFDatas1 = struEFInfo.m_vecEFDatas.begin();
			while ( iterEFDatas1 != struEFInfo.m_vecEFDatas.end() )
			{
				if ( 1 == HexToDec( (*iterEFDatas1).m_strP1 ) )//存在P1=1的记录
				{
					bFlagEF = true;
					m_strsTemp<<"<INFO>"<<strTgNames<<"<EF SFI=\""<<struEFInfo.m_strSFI<<"\">该透明文件存在多条记录<"<<struEFInfo.m_strSFI<<">,默认取P1=1的记录为有效记录,其他无效；";
					Append_Warning(m_strsTemp.str());
					//检测P1=1的记录
					m_strsTemp.str("");
					m_strsTemp<<"<INFO>"<<strTgNames<<"<"<<struEFInfo.m_strSFI<<" P1=\""<<(*iterEFDatas1).m_strP1<<"\" Tag=\""<<(*iterEFDatas1).m_strTag<<"\"> 标签内数据不合法：";
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
				m_strsTemp<<"<INFO>"<<strTgNames<<"<EF SFI=\""<<struEFInfo.m_strSFI<<"\">该透明文件存在多条记录<"<<struEFInfo.m_strSFI<<">,默认取第一条记录为有效记录,其他无效；";
				Append_Warning(m_strsTemp.str());
				//检测透明文件的第一条记录
				m_strsTemp.str("");
				m_strsTemp<<"<INFO>"<<strTgNames<<"<"<<struEFInfo.m_strSFI<<" P1=\""<<struEFInfo.m_vecEFDatas.front().m_strP1<<"\" Tag=\""<<struEFInfo.m_vecEFDatas.front().m_strTag<<"\"> 标签内数据不合法：";
				if ( !CheckEFDatas( struEFInfo.m_strEFType, struEFInfo.m_vecEFDatas.front() ) )
				{
					Append_Error(m_strsTemp.str());
					bFlag=false;
				}
			}
		}
	}
	else//检测其他类型的文件
	{
		//遍历<EF>对应SFI的标签<SFI>数据
		vector<EFDATAS>::const_iterator iterEFDatas = struEFInfo.m_vecEFDatas.begin();
		bFlag2 = true;
		bFlag1 = true;//用于检测B-TLV数据是否已经检测过
		nP1=0;//用于检测定长和变长文件的P1是否存在从1开始递增数
		while ( iterEFDatas != struEFInfo.m_vecEFDatas.end() )
		{
			//定长和变长文件检测P1是否存在从1开始递增数和P1是否有重复
			if ( "1"==struEFInfo.m_strEFType || "2"==struEFInfo.m_strEFType )
			{
				//检测P1是否从1开始递增
				++nP1;
				vector<EFDATAS>::const_iterator iterEFDatas2 = struEFInfo.m_vecEFDatas.begin();
				while ( iterEFDatas2 != struEFInfo.m_vecEFDatas.end() )
				{
					if ( nP1 == HexToDec( (*iterEFDatas2).m_strP1 ) )//存在P1=1	
						break;
					++iterEFDatas2;
				}
				if ( iterEFDatas2 == struEFInfo.m_vecEFDatas.end() )
				{
					m_strsTemp.str("");
					m_strsTemp<<"<INFO>"<<strTgNames<<"下缺少 <"<<struEFInfo.m_strSFI<<" P1=\""<<DecToHex(nP1)<<"\"> 的标签数据,请确认该EF文件所有数据项的P1属性是否从1开始递增；";
					Append_Error(m_strsTemp.str());
					bFlag=false;
				}

				//检测P1是否有重复
				vector<EFDATAS>::const_iterator iterEFDatas1 = iterEFDatas+1;
				while ( iterEFDatas1 != struEFInfo.m_vecEFDatas.end() )
				{
					if ( (*iterEFDatas).m_strP1 == (*iterEFDatas1).m_strP1 )
					{	
						m_strsTemp.str("");
						m_strsTemp<<"<INFO>"<<strTgNames<<"下存在重复的P1标签数据 <"<<struEFInfo.m_strSFI<<" P1=\""<<(*iterEFDatas1).m_strP1<<"\" Tag=\""<<(*iterEFDatas1).m_strTag<<"\">；";
						Append_Error(m_strsTemp.str());
						bFlag=false;
						break;
					}
					++iterEFDatas1;
				}
			}

			//检测定长文件的所有记录的Len是否都一致
			if (  iterEFDatas == struEFInfo.m_vecEFDatas.begin() )
				strLen = (*iterEFDatas).m_strLen;
			else
			{
				if ( bFlag2 && "1"==struEFInfo.m_strEFType && strLen!=(*iterEFDatas).m_strLen )
				{
					m_strsTemp.str("");
					m_strsTemp<<"<INFO>"<<strTgNames<<"定长文件<"<<struEFInfo.m_strSFI<<">所有记录的Len必须一致；";
					Append_Error(m_strsTemp.str());
					bFlag=false;
					bFlag2=false;
				}
			}

			//检测EF记录的数据
			m_strsTemp.str("");
			m_strsTemp<<"<INFO>"<<strTgNames<<"<"<<struEFInfo.m_strSFI<<" P1=\""<<(*iterEFDatas).m_strP1<<"\" Tag=\""<<(*iterEFDatas).m_strTag<<"\"> 标签内数据不合法：";
			if ( !CheckEFDatas( struEFInfo.m_strEFType, *iterEFDatas ) )
			{
				Append_Error(m_strsTemp.str());
				bFlag=false;
			}

			//遍历存储B-TLV或者循环文件数据的结构体EFEXTRADATAS
			nExP1=0;//用于检测循环文件的P1是否存在从1开始递增数
			vector<EFEXTRADATAS>::const_iterator iterEFExDatas = (*iterEFDatas).m_vecEFExDatas.begin();
			while ( iterEFExDatas != (*iterEFDatas).m_vecEFExDatas.end() )
			{
				//循环文件检测P1是否存在从1开始递增数和P1是否有重复
				if ( "4"==struEFInfo.m_strEFType )
				{
					//检测P1是否从1开始递增
					++nExP1;
					vector<EFEXTRADATAS>::const_iterator iterEFExDatas2 = (*iterEFDatas).m_vecEFExDatas.begin();
					while ( iterEFExDatas2 != (*iterEFDatas).m_vecEFExDatas.end() )
					{
						if ( nExP1 == HexToDec( (*iterEFExDatas2).m_strP1 ) )//存在P1	
							break;
						++iterEFExDatas2;
					}
					if ( iterEFExDatas2 == (*iterEFDatas).m_vecEFExDatas.end() )
					{
						m_strsTemp.str("");
						m_strsTemp<<"<INFO>"<<strTgNames<<"下缺少 <"<<struEFInfo.m_strSFI<<" P1=\""<<DecToHex(nExP1)<<"\"> 的标签数据,请确认该EF文件所有数据项的P1属性是否从1开始递增；";
						Append_Error(m_strsTemp.str());
						bFlag=false;
					}

					//检测P1是否有重复
					vector<EFEXTRADATAS>::const_iterator iterEFExDatas1 = iterEFExDatas+1;
					while ( iterEFExDatas1 != (*iterEFDatas).m_vecEFExDatas.end() )
					{
						if ( (*iterEFExDatas).m_strP1 == (*iterEFExDatas1).m_strP1 )
						{	
							m_strsTemp.str("");
							m_strsTemp<<"<INFO>"<<strTgNames<<"下存在重复的P1标签数据 <"<<struEFInfo.m_strSFI<<" P1=\""<<(*iterEFExDatas1).m_strP1<<"\" Tag=\""<<(*iterEFExDatas1).m_strTag<<"\">；";
							Append_Error(m_strsTemp.str());
							bFlag=false;
							break;
						}
						++iterEFExDatas1;
					}
				}

				//B-TLV数据检测Tag是否有重复
				if ( "B-TLV"==(*iterEFExDatas).m_strrType )
				{
					vector<EFEXTRADATAS>::const_iterator iterEFExDatas1 = iterEFExDatas+1;
					while ( iterEFExDatas1 != (*iterEFDatas).m_vecEFExDatas.end() )
					{
						if ( (*iterEFExDatas).m_strTag == (*iterEFExDatas1).m_strTag )
						{	
							m_strsTemp.str("");
							m_strsTemp<<"<INFO>"<<strTgNames<<"下存在重复的Tag标签数据 <Tag"<<(*iterEFExDatas1).m_strTag<<" Tag=\""<<(*iterEFExDatas1).m_strTag<<"\">；";
							Append_Error(m_strsTemp.str());
							bFlag=false;
							break;
						}
						++iterEFExDatas1;
					}
				}

				//检测EFEXTRADATAS数据合法性
				m_strsTemp.str("");
				m_strsTemp<<"<INFO>"<<strTgNames;
				strTagLable = "Tag"+(*iterEFDatas).m_strTag;
				if ( "4"==struEFInfo.m_strEFType )//循环文件
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
					if ( iterTag==m_setTag.end() )//未检测过的B-TLV公共数据,即<TagXX>标签组数据只检测一次
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
	int nFlag=0;//标记KEY的样式 值为0：KEY|KEY、KEY+KEY的样式	值为1：KEY的样式
	transform( strKey.begin(), strKey.end(), strKey.begin(), toupper );
	transform( strADFName.begin(), strADFName.end(), strADFName.begin(), toupper );
	transform( strDDFName.begin(), strDDFName.end(), strDDFName.begin(), toupper );

	//控制密钥（取值：空、PIN、KEY（如RKSSSE）、PIN|KEY、PIN+KEY、禁止（FF））
	if ( strKey.empty() || strKey == "FF" )//空表示自由读写，FF表示进制读写
		return true;
	
	//分割KEY|KEY、KEY+KEY(PIN是KEY的一种)
	nPos=strKey.find('|',0);
	if ( nPos==-1 )//不存在分隔符|，说明是KEY或KEY+KEY
	{
		nPos=strKey.find('+',0);
		if ( nPos==-1 )//也不存在分隔符+，说明是KEY
		{
			strKey1 = strKey;//存储KEY
			nFlag = 0;
		}
		else
		{
			strKeyL = strKey.substr(0,nPos);//strKeyL 受上级目录控制的密钥
			strKeyR = strKey.substr(nPos+1);//strKeyL 受当前目录控制的密钥
			nFlag = 1;
		}
	}
	else
	{
		strKeyL = strKey.substr(0,nPos);
		strKeyR = strKey.substr(nPos+1);
		nFlag = 1;
	}
	
	if ( strDDFName == "DDF0" && strADFName == "DF00" )//MF下EF的控制密钥检测
	{
		//不能为KEY|KEY、KEY+KEY、PIN（PIN表示为上级目录的控制密钥）(因为MF没有上级目录，所以不存在受上级目录控制的密钥)
		if ( nFlag == 1 || strKey1 == "PIN" )
		{
			m_strsTemp<<strProperty<<"密钥值只能为（自由）空、KEY、禁止（FF）；";
			return false;
		}
		else if ( nFlag == 0 )
		{
			if ( !CheckControlKeyExis( strDDFName, strADFName, strKey1 ) )
			{
				m_strsTemp<<strProperty<<"密钥值"<<strKey1<<"在<OTHERKEYS><"<<strDDFName<<"><"<<strADFName<<">中不存在；";
				return false;
			}
		}
	}
	else// MF/ADF 或 MF/DDF 或 MF/DDF/ADF 下EF的控制密钥检测
	{
		if ( nFlag == 0 )
		{
			if ( strKey1 == "PIN" )//检测受上级目录控制的密钥
			{
				if ( strDDFName != "DDF0" && strADFName != "DF00" )// MF/DDF/ADF 要检测DDF下是否存在PIN 
				{
					strTmpDDF = strDDFName;
					strTmpADF = "DF00";
				}
				else// MF/ADF 或 MF/DDF 要检测MF下是否存在PIN
				{
					strTmpDDF = "DDF0";
					strTmpADF = "DF00";
				}

				if ( !CheckControlKeyExis( strTmpDDF, strTmpADF, strKey1 ) )
				{
					if ( strTmpDDF == "SSSE" )
					{
						if ( strTmpADF == "DF00" )
							m_strsTemp<<strProperty<<"密钥值"<<strKey1<<"在<SSSEKEYS><SSSE>中不存在；";
						else
							m_strsTemp<<strProperty<<"密钥值"<<strKey1<<"在<SSSEKEYS><"<<strTmpADF<<">中不存在；";
					}
					else
						m_strsTemp<<strProperty<<"密钥值"<<strKey1<<"在<OTHERKEYS><"<<strTmpDDF<<"><"<<strTmpADF<<">中不存在；";
					return false;
				}
			}
			else// 检测受当前目录控制的密钥
			{
				if ( !CheckControlKeyExis( strDDFName, strADFName, strKey1 ) )
				{
					if ( strDDFName == "SSSE" )
					{
						if ( strADFName == "DF00" )
							m_strsTemp<<strProperty<<"密钥值"<<strKey1<<"在<SSSEKEYS><SSSE>中不存在；";
						else
							m_strsTemp<<strProperty<<"密钥值"<<strKey1<<"在<SSSEKEYS><"<<strADFName<<">中不存在；";
					}
					else
						m_strsTemp<<strProperty<<"密钥值"<<strKey1<<"在<OTHERKEYS><"<<strDDFName<<"><"<<strADFName<<">中不存在；";
					return false;
				}
			}
		}
		else if ( nFlag == 1 )
		{
			//KEY|KEY、KEY+KEY中右边的KEY不能为PIN，因为PIN表示为上级目录的控制密钥，只能存在于左边的值中
			if ( strKeyR == "PIN" )
			{
				m_strsTemp<<strProperty<<"密钥值"<<strKey<<"不能为 KEY+PIN、KEY|PIN 这两种形式；";
				return false;
			}

			if ( strDDFName != "DDF0" && strADFName != "DF00" )// MF/DDF/ADF 要检测DDF下是否存在 受上级目录控制的密钥
			{
				strTmpDDF = strDDFName;
				strTmpADF = "DF00";
			}
			else// MF/ADF 或 MF/DDF 要检测MF下是否存在 受上级目录控制的密钥
			{
				strTmpDDF = "DDF0";
				strTmpADF = "DF00";
			}

			if ( !CheckControlKeyExis( strTmpDDF, strTmpADF, strKeyL ) )//检测受上级目录控制的密钥
			{
				if ( strTmpDDF == "SSSE" )
				{
					if ( strTmpADF == "DF00" )
						m_strsTemp<<strProperty<<"密钥值"<<strKey<<"中的"<<strKeyL<<"在<SSSEKEYS><SSSE>中不存在；";
					else
						m_strsTemp<<strProperty<<"密钥值"<<strKey<<"中的"<<strKeyL<<"在<SSSEKEYS><"<<strTmpADF<<">中不存在；";
				}
				else
					m_strsTemp<<strProperty<<"密钥值"<<strKey<<"中的"<<strKeyL<<"在<OTHERKEYS><"<<strTmpDDF<<"><"<<strTmpADF<<">中不存在；";
				return false;
			}
			if ( !CheckControlKeyExis( strDDFName, strADFName, strKeyR ) )//检测受当前目录控制的密钥
			{
				if ( strDDFName == "SSSE" )
				{
					if ( strADFName == "DF00" )
						m_strsTemp<<strProperty<<"密钥值"<<strKey<<"中的"<<strKeyR<<"在<SSSEKEYS><SSSE>中不存在；";
					else
						m_strsTemp<<strProperty<<"密钥值"<<strKey<<"中的"<<strKeyR<<"在<SSSEKEYS><"<<strADFName<<">中不存在；";
				}
				else
					m_strsTemp<<strProperty<<"密钥值"<<strKey<<"中的"<<strKeyR<<"在<OTHERKEYS><"<<strDDFName<<"><"<<strADFName<<">中不存在；";
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

	//遍历结构体KEYS的数据
	vector<KEYS>::const_iterator iterKeys = V_EKEYS.begin();
	while ( iterKeys!=V_EKEYS.end() )
	{
		//先寻找密钥数据下是否存在该DDF
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
			//找到后继续寻找ADFKEY密钥中是否存在ADF,存在的话在当前ADF下继续寻找是否存在该控制密钥
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
				if ( strTemp == strKey )//找到密钥，匹配成功
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
	
	//定长和变长文件的P1、Tag、长度、值和类型
	if ( "1" == strEFType || "2" == strEFType )
	{
		if ( strP1 == "" || strP1.length()>2 || !JudgeHex(strP1) )
		{
			m_strsTemp<<"P1必须为长度不超过2位的非空十六进制字符串；";
			bFlag = false;
		}

		if ( strTag == "" || strTag.length()>2 || !JudgeHex(strTag) )
		{
			m_strsTemp<<"Tag必须为长度不超过2位的非空十六进制字符串；";
			bFlag = false;
		}

		if ( strLen == "" || !JudgeHex(strLen) )
		{
			m_strsTemp<<"Len必须为非空十六进制字符串；";
			bFlag = false;
		}

		if ( strVal.compare(0, 3, "CON")==0 )
		{
			strTemp = strVal.substr(3);
			if ( strTemp == "" || !JudgeDec(strTemp) )
			{
				m_strsTemp<<"Val的常量格式必须为“CON+数字”；";
				bFlag = false;
			}
		}
		else if ( strVal != "00" && strVal != "FF" )//00和FF表示该条记录没有实际数据只有默认填充数据,此时不做检测
		{
			if ( "AN"!=strType )//非an类型检测数据Val的合法性
			{
				if ( !JudgeHex(strVal) || nValLen>HexToDec(strLen)*2 )
				{
					m_strsTemp<<"Val应该为空或CON常量或长度不大于Len*2的十六进制字符串,否则生成脚本写卡后会导致该项记录数据异常；";
					bFlag = false;
				}
			}
			else//an类型检测数据Val的合法性
			{
				if ( nValLen>HexToDec(strLen) )
				{
					m_strsTemp<<"Val应该为空或00或CON常量或长度不大于Len的字符串,否则生成脚本写卡后会导致该项记录数据异常；";
					bFlag = false;
				}
			}
		}

		//类型(an,cn,b,B-TLV)
		if ( "AN"!=strType && "CN"!=strType && "B"!=strType && "B-TLV"!=strType )
		{
			m_strsTemp<<"rType必须为类型(an,cn,b,B-TLV)中的一种；";
			bFlag = false;
		}
	}
	//透明文件，值不能为CON常量,只能为空或十六进制字符串,值类型只能是b，检测数据长度、值
	else if ( "3" == strEFType )
	{
		if ( strLen == "" || !JudgeHex(strLen) )
		{
			m_strsTemp<<"Len必须为非空十六进制字符串；";
			bFlag = false;
		}

		//类型只能是b
		if ( "B"!=strType )
		{
			m_strsTemp<<"rType必须为类型b；";
			bFlag = false;
		}

		if ( strVal != "00" && strVal != "FF" )//00和FF表示该条记录没有实际数据只有默认填充数据,此时不做检测
		{
			if ( !JudgeHex(strVal) || nValLen>HexToDec(strLen)*2 )
			{
				m_strsTemp<<"Val应该为空或长度不大于Len*2的十六进制字符串,否则生成脚本写卡后会导致该项记录数据异常；";
				bFlag = false;
			}
		}

	}
	else if ( "4" == strEFType )//循环文件的EF不作检测因为数据都保存在Ex_EF内
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

	//循环文件数据检测
	if( "4"==strEFType )
	{
		m_strsTemp<<"<"<<strSFI<<" P1=\""<<struEFExDatas.m_strP1<<"\" Tag=\""<<struEFExDatas.m_strTag<<"\"> 标签内数据不合法：";
		if ( strP1 == "" || strP1.length()>2 || !JudgeHex(strP1) )
		{
			m_strsTemp<<"P1必须为长度不超过2位的非空十六进制字符串；";
			bFlag = false;
		}
	}
	else//B-TLV数据检测
	{
		m_strsTemp<<"<"<<strTag<<" Tag=\""<<struEFExDatas.m_strTag<<"\" Len=\""<<struEFExDatas.m_strLen<<"\"> 标签内数据不合法：";
		if ( strExTag == "" || strExTag.length()>2 || !JudgeHex(strExTag) )
		{
			m_strsTemp<<"Tag必须为长度不超过2位的非空十六进制字符串；";
			bFlag = false;
		}
	}

	/*共同数据检测*/
	if ( strLen == "" || !JudgeHex(strLen) )
	{
		m_strsTemp<<"Len必须为非空十六进制字符串；";
		bFlag = false;
	}

	if ( strVal != "00" && strVal != "FF" )//00和FF表示该条记录没有实际数据只有默认填充数据,此时不做检测
	{
		if (  "AN"!=strType )//非an类型检测数据Val的合法性
		{
			if ( !JudgeHex(strVal) || nValLen>HexToDec(strLen)*2 )
			{
				m_strsTemp<<"Val应该为空或长度不大于Len*2的十六进制字符串,否则生成脚本写卡后会导致该项记录数据异常；";
				bFlag = false;
			}
		}
		else//an类型检测数据Val的合法性
		{
			if ( nValLen>HexToDec(strLen) )
			{
				m_strsTemp<<"Val应该为空或00或长度不大于Len的字符串,否则生成脚本写卡后会导致该项记录数据异常；";
				bFlag = false;
			}
		}
	}

	//类型(an,cn,b)
	if ( "AN"!=strType && "CN"!=strType && "B"!=strType )
	{
		m_strsTemp<<"rType必须为类型(an,cn,b)中的一种；";
		bFlag = false;
	}

	return bFlag;
}

bool CInformation::CheckSETTINGDatas()
{
	bool bFlag=true,bTemp;
	string strPin,strCityCode,strCount,strMode,strType,strScale,strPKeyNo,strPKeyfsjs,strPKeyfsyz,strPKeyLen;

	/*检查PIN*/
	m_strsTemp.str("");
	m_strsTemp<<"<INFO><SETTING><PIN> 标签内数据不合法：";
	bTemp=true;
	strPin = S_SETTING.m_struPIN.m_strValue;
	strCount = S_SETTING.m_struPIN.m_strCount;
	if ( strPin.length()<4 || strPin.length()>12 || !JudgeHex(strPin) )
	{
		m_strsTemp<<"Value必须为长度2-6字节十六进制字符串；";
		bTemp = false;
	}
	if ( strCount.length()!=2 || !JudgeHex(strCount) )
	{
		m_strsTemp<<"Count必须为长度等于2位的十六进制字符串；";
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
		m_strsTemp<<"<INFO><SETTING><PIN>标签内Value值的长度最好为3个字节；";
		Append_Warning(m_strsTemp.str());
	}

	/*检查CITYCODE*/
	m_strsTemp.str("");
	m_strsTemp<<"<INFO><SETTING><CITYCODE> 标签内数据不合法：";
	strCityCode = S_SETTING.m_struCityCode.m_strValue;
	if ( strCityCode.length()/2 != 3 )
	{
		m_strsTemp<<"Value的数据长度必须为3字节；";
		Append_Error(m_strsTemp.str());
		bFlag = false;
	}

	/*检查LOADKEYMODE Value：装载密钥的方式 1：加密机装载 2：固定密钥装载*/
	m_strsTemp.str("");
	m_strsTemp<<"<INFO><SETTING><LOADKEYMODE> 标签内数据不合法：";
	strMode = S_SETTING.m_struLoadKeyInfo.m_strMode;
	if ( strMode!="1" && strMode!="2" )
	{
		m_strsTemp<<"Value取值必须为1或者2；";
		Append_Error(m_strsTemp.str());
		bFlag = false;
	}

	/*检查STORGEMODE Value	0：密文	1：明文	10：密文更新	11：明文更新*/
	m_strsTemp.str("");
	m_strsTemp<<"<INFO><SETTING><STORGEMODE> 标签内数据不合法：";
	strType = S_SETTING.m_struLoadKeyInfo.m_strType;
	if ( strType!="0" && strType!="1" && strType!="10" && strType!="11" )
	{
		m_strsTemp<<"Value取值必须为(0、1、10、11)中的一种；";
		Append_Error(m_strsTemp.str());
		bFlag = false;
	}

	/*检查ENCRYPTDEV Scale：级别	2：全省通集中模式	1：地级市集中模式*/
	m_strsTemp.str("");
	m_strsTemp<<"<INFO><SETTING><ENCRYPTDEV> 标签内数据不合法：";
	strScale = S_SETTING.m_struLoadKeyInfo.m_strScale;
	if ( strScale!="1" && strScale!="2" )
	{
		m_strsTemp<<"Value取值必须为1或者2；";
		Append_Error(m_strsTemp.str());
		bFlag = false;
	}

	/*当LOADKEYMODE为1加密机装载的时候检查LOADSET*/
	if ( strMode == "1" )
	{
		m_strsTemp.str("");
		m_strsTemp<<"<INFO><SETTING><LOADSET> 标签内数据不合法：";
		bTemp=true;
		strPKeyNo = S_SETTING.m_struLoadKeyInfo.m_strPKeyNo;
		strPKeyfsjs = S_SETTING.m_struLoadKeyInfo.m_strPKeyfsjs;
		strPKeyfsyz = S_SETTING.m_struLoadKeyInfo.m_strPKeyfsyz;
		strPKeyLen = S_SETTING.m_struLoadKeyInfo.m_strPKeyLen;
		if ( strPKeyNo.length()>8 || !JudgeHex(strPKeyNo) )
		{
			m_strsTemp<<"PKeyNo必须为长度不大于8位的十六进制字符串；";
			bTemp = false;
		}
		if ( strPKeyfsjs.length()>8 || !JudgeHex(strPKeyfsjs) )
		{
			m_strsTemp<<"PKeyfsjs必须为长度不大于8位的十六进制字符串；";
			bTemp = false;
		}
		if ( strPKeyfsyz.length()>8 || !JudgeHex(strPKeyfsyz) )
		{
			m_strsTemp<<"PKeyfsyz必须为长度不大于8位的十六进制字符串；";
			bTemp = false;
		}
		if ( strPKeyLen!="08" && strPKeyLen!="10" )
		{
			m_strsTemp<<"PKeyLen必须为“08”或者“10”(单位字节)；";
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
	
	//遍历KEYS的数据
	vector<KEYS>::const_iterator iterKeys = V_EKEYS.begin();
	while ( iterKeys!=V_EKEYS.end() )
	{
		//遍历<DDF>数据
		vector<ADFKEY>::const_iterator iterKey = (*iterKeys).m_vecADFKEY.begin();
		while ( iterKey!=(*iterKeys).m_vecADFKEY.end() )
		{
			vector<KEYDATAS>::const_iterator iterKeyDatas = (*iterKey).m_vecKeyDatas.begin();
			while ( iterKeyDatas!=(*iterKey).m_vecKeyDatas.end() )
			{
				if ( (*iterKeyDatas).m_strName == "PIN" )//PIN不检测数据的合法性
				{
					++iterKeyDatas;
					continue;
				}

				bFlag=true;
				m_strsTemp.str("");
				if ( (*iterKeys).m_strDDFName == "SSSE" )
				{
					if ( (*iterKey).m_strADFName == "DF00" )
						m_strsTemp<<"<INFO><SSSEKEYS><SSSE Name=\""<<(*iterKeyDatas).m_strName<<"\" KeyIndex=\""<<(*iterKeyDatas).m_strKeyIndex<<"\">标签内数据不合法：";
					else
						m_strsTemp<<"<INFO><SSSEKEYS><"<<(*iterKey).m_strADFName<<" Name=\""<<(*iterKeyDatas).m_strName<<"\" KeyIndex=\""<<(*iterKeyDatas).m_strKeyIndex<<"\">标签内数据不合法：";
				}
				else
					m_strsTemp<<"<INFO><OTHERKEYS><"<<(*iterKeys).m_strDDFName<<"><"<<(*iterKey).m_strADFName<<" Name=\""<<(*iterKeyDatas).m_strName<<"\" KeyIndex=\""<<(*iterKeyDatas).m_strKeyIndex<<"\">标签内数据不合法：";
			
				/*检测密钥标识*/
				nIndex = 0;
				strKeyIndex = (*iterKeyDatas).m_strKeyIndex;
				nPos = strKeyIndex.find("|");
				if ( nPos == string::npos )
				{
					if ( strKeyIndex.length()!=2 || !JudgeHex(strKeyIndex) )
					{
						m_strsTemp<<"KeyIndex必须为2位十六进制字符串；";
						bFlag = false;
					}
				}
				while ( nPos != string::npos )
				{
					m_strTemp = strKeyIndex.substr( 0, nPos );
					if ( m_strTemp.length()!=2 || !JudgeHex(m_strTemp) )
					{
						m_strsTemp<<"KeyIndex竖线分隔符内的数据必须为2位十六进制字符串；";
						bFlag = false;
						break;
					}
					strKeyIndex=strKeyIndex.substr(nPos+1);
					nPos = strKeyIndex.find("|");
					if ( nPos == string::npos && !strKeyIndex.empty() )
					{
						m_strsTemp<<"KeyIndex必须以分隔符\"|\"作为结束符；";
						bFlag = false;
					}
					nIndex++;
				}
			
				/*检测密钥版本*/
				if ( (*iterKeyDatas).m_strVer.length()!=2 || !JudgeHex((*iterKeyDatas).m_strVer) )
				{
					m_strsTemp<<"Ver必须为长度等于2位的十六进制字符串；";
					bFlag = false;
				}

				//加密机方式装载的时候才检测PSAM卡标识和用途
				if ( "1"==S_SETTING.m_struLoadKeyInfo.m_strMode )
				{
					/*检测psam卡密钥用途*/
					strPSAMKeyType = (*iterKeyDatas).m_strPSAMKeyType;
					if ( strPSAMKeyType!="-" && (strPSAMKeyType.length()!=2 || !JudgeHex(strPSAMKeyType)) )
					{
						m_strsTemp<<"PSAMKeyType必须为2位十六进制字符串或字符\"-\"；";
						bFlag = false;
					}
					else
					{
						if ( strPSAMKeyType!="-" )
						{
							if ( CheckPSAMType(strPSAMKeyType) == 1 )
							{
								m_strsTemp<<"PSAMKeyType当前值中b8-b6所表示的密钥分散级数不合法；";
								bFlag = false;
							}
							else if ( CheckPSAMType(strPSAMKeyType) == 2 )
							{
								m_strsTemp<<"PSAMKeyType当前值中b5-b1所表示的密钥类型不合法；";
								bFlag = false;
							}
						}
					}
					/*检测psam卡密钥标识*/
					strPSAMKeyID = (*iterKeyDatas).m_strPSAMKeyID;
					nPos = strPSAMKeyID.find("|");
					if ( nPos == string::npos )
					{
						if ( strPSAMKeyID!="-" && (strPSAMKeyID.length()!=2 || !JudgeHex(strPSAMKeyID)) )
						{
							m_strsTemp<<"PSAMKeyID必须为2位十六进制字符串或字符\"-\"；";
							bFlag = false;
						}
					}
					while ( nPos != string::npos )
					{
						m_strTemp = strPSAMKeyID.substr( 0, nPos );
						if ( m_strTemp.length()!=2 || !JudgeHex(m_strTemp) )
						{
							m_strsTemp<<"PSAMKeyID竖线分隔符内的数据必须为2位十六进制字符串；";
							bFlag = false;
							break;
						}
						strPSAMKeyID=strPSAMKeyID.substr(nPos+1);
						nPos = strPSAMKeyID.find("|");
						if ( nPos == string::npos && !strPSAMKeyID.empty() )
						{
							m_strsTemp<<"PSAMKeyID必须以分隔符\"|\"作为结束符；";
							bFlag = false;
						}
					}
				}

				/*检测Len*/
				strLen = (*iterKeyDatas).m_strLen;
				if (strLen != "08" && strLen != "10")
				{
					m_strsTemp<<"Len的值必须为“08”或者“10”(单位字节)；";
					bFlag = false;
				}
				else
				{
					/*固定密钥装载方式的时候检测密钥值长度必须等于Len字节的十六进制字符串*/
					if ( "2"==S_SETTING.m_struLoadKeyInfo.m_strMode )
					{
						steKeyVal = (*iterKeyDatas).m_strValue;
						nLen=HexToDec( strLen.c_str() )*2;
						nValLen=steKeyVal.length();
						if ( nValLen!=nLen || !JudgeHex(steKeyVal) )
						{
							m_strsTemp<<"固定密钥装载时Value必须为长度等于"<<strLen<<"字节的十六进制字符串；";
							bFlag = false;
						}
					}
				}

				/*检测密钥组数*/
				strGroup = (*iterKeyDatas).m_strGroup;
				nPos = strGroup.find("|");
				nGroup=0;
				if ( nPos==string::npos )
				{
					m_strsTemp<<"Group数据中缺少竖线分隔符\"|\"；";
					bFlag = false;
				}
				while ( nPos != string::npos )
				{
					m_strTemp = strGroup.substr( 0, nPos );
					if ( m_strTemp.length()!=2 || !JudgeHex(m_strTemp) )
					{
						m_strsTemp<<"Group竖线分隔符内的数据必须为2位十六进制字符串；";
						bFlag = false;
						break;
					}
					strGroup=strGroup.substr(nPos+1);
					nPos = strGroup.find("|");
					if ( nPos == string::npos && !strGroup.empty() )
					{
						m_strsTemp<<"Group必须以分隔符\"|\"作为结束符；";
						bFlag = false;
					}
					nGroup++;
				}
				if ( nGroup>4 )
				{
					m_strsTemp<<"Group以竖线为分隔符的数据组数不能超过4组；";
					bFlag = false;
				}
				/*加密机装载密钥方式的时候才检测加密机存储密钥的地址*/
				if ( "1"==S_SETTING.m_struLoadKeyInfo.m_strMode )
				{
					strAddr = (*iterKeyDatas).m_strAddr;
					nPos = strAddr.find("|");
					nAddr=0;
					if ( nPos==string::npos )
					{
						m_strsTemp<<"Addr数据中缺少竖线分隔符\"|\"；";
						bFlag = false;
					}
					while ( nPos != string::npos )
					{
						m_strTemp = strAddr.substr( 0, nPos );
						if ( m_strTemp=="" || !JudgeHex(m_strTemp) )
						{
							m_strsTemp<<"Addr竖线分隔符内的数据必须为非空十六进制字符串；";
							bFlag = false;
							break;
						}
						strAddr=strAddr.substr(nPos+1);
						nPos = strAddr.find("|");
						if ( nPos == string::npos && !strAddr.empty() )
						{
							m_strsTemp<<"Addr必须以分隔符\"|\"作为结束符；";
							bFlag = false;
						}
						nAddr++;
					}
					if ( nGroup != nAddr )
					{
						m_strsTemp<<"(以竖线为分隔符划分数据组数)Addr数据组数必须和Group数据组数相同；";
						bFlag = false;
					}
				}

				/*检测错误计数器*/
				if ( (*iterKeyDatas).m_strCount.length()!=2 || !JudgeHex((*iterKeyDatas).m_strCount) )
				{
					m_strsTemp<<"Count必须为长度等于2位的十六进制字符串；";
					bFlag = false;
				}
				/*检测必要标识组数是否正确*/
				if ( nIndex>0 && nIndex!=nGroup )//当有几组密钥标识的时候需要和Group数据组数相同
				{
					m_strsTemp<<"(以竖线为分隔符划分数据组数)KeyIndex数据组数必须和Group数据组数相同；";
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
	int nDivLv,nKeyType;//分散级数，密钥类型
	string strTmp;

	//取b8-b6的值判断分散级数
	nDivLv = atoi( strPSAMType.substr(0,1).c_str() )/2;
	if ( nDivLv<0 || nDivLv>3 )
		return 1;
	
	//取b5-b1的值判断密钥类型
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

	//遍历KEYS的数据
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

		//遍历<DDF>数据
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
		Append_Error( "<SSSEKEYS><SSSE>下缺少密钥IRK，该密钥为必须存在的密钥！" );

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
	strsTmp1.str("");//存储SSSEKEYS密钥数据
	strsTmp2.str("");//存储OTHERKEYS密钥数据

	//输出读取的xml数据到log文件
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

					if ( "4" == (*iterEFInfo).m_strEFType )//循环文件的额外数据
					{
						vector<EFEXTRADATAS>::const_iterator iterEFExDatas = (*iterEFDatas).m_vecEFExDatas.begin();
						while( iterEFExDatas!=(*iterEFDatas).m_vecEFExDatas.end() )
						{
							m_strsTemp<<"              <"<<(*iterEFInfo).m_strSFI<<" "<<(*iterEFExDatas).m_strP1<<" "<<(*iterEFExDatas).m_strTag<<" "<<(*iterEFExDatas).m_strLen
								<<" "<<(*iterEFExDatas).m_strVal<<" "<<(*iterEFExDatas).m_strrType<<" "<<(*iterEFExDatas).m_strRemark<<" />"<<endl;
							++iterEFExDatas;
						}
					}
					else if ( "B-TLV" == (*iterEFDatas).m_strrType )//B-TLV类型的额外数据
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
			//检测社保应用下的社会保险数据区2目录文件下的文件是否存在，不存在的给出警告信息
			bflag1 = true;//标记存在该目录文件
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
						if ( "EF05" == m_strTemp )//医疗保险基本信息文件
							bflag = true;
						break;
					case 1:
						if ( "EF06" == m_strTemp )//医疗保险临时脱网结算信息文件
							bflag = true;
						break;
					case 2:
						if ( "EF08" == m_strTemp )//医疗交易明细文件
							bflag = true;
						break;
					case 3:
						if ( "EF15" == m_strTemp )//特殊医疗结算记录文件
							bflag = true;
						break;
					default:
						break;
					}
					if ( bflag )//找到对应的文件跳出循环
						break;
					++iterEFInfo;
				}
				if ( !bflag )//不存在文件的话给出警告
				{
					strWarn = "<MF><"+(*iterDDF).m_strDDFName+"><"+(*iterADF).m_strADFName+">";
					switch(i)
					{
					case 0:
						strWarn += "下缺少基本文件EF05(医疗保险基本信息文件),有可能会影响到医疗消费功能的使用！";
						Append_Warning(strWarn);
						break;
					case 1:
						strWarn += "下缺少基本文件EF06(医疗保险临时脱网结算信息文件),有可能会影响到医疗消费功能的使用！";
						Append_Warning(strWarn);
						break;
					case 2:
						strWarn += "下缺少基本文件EF08(医疗交易明细文件),有可能会影响到医疗消费功能的使用！";
						Append_Warning(strWarn);
						break;
					case 3:
						strWarn += "下缺少基本文件EF15(特殊医疗结算记录文件),有可能会影响到医疗消费功能的使用！";
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
		Append_Warning("社保应用<MF><SSSE>下缺少对应AID=\"D15600000503\"的ADF 社会保险数据区2 文件，将会导致医疗消费功能无法使用！");

}


void CInformation::CheckCardNo()
{
	string strCardNo="";
	
	vector<DDFINFO>::const_iterator iterDDF = S_CARDNFO.m_vecDDFInfo.begin();
	while ( iterDDF!=S_CARDNFO.m_vecDDFInfo.end() )//DDF
	{
		m_strTemp = (*iterDDF).m_strAID;
		transform(m_strTemp.begin(), m_strTemp.end(), m_strTemp.begin(), toupper); 
		if ( "7378312E73682EC9E7BBE1B1A3D5CF" != m_strTemp )//非SSSE
		{
			++iterDDF;
			continue;
		}
		vector<ADFINFO>::const_iterator iterADF = (*iterDDF).m_vecADFInfo.begin();
		while ( iterADF!=(*iterDDF).m_vecADFInfo.end() )//ADF
		{
			m_strTemp = (*iterADF).m_strAID;
			if ( "" != m_strTemp )//非DF00
			{
				++iterADF;
				continue;
			}
			
			vector<EFINFO>::const_iterator iterEFInfo = (*iterADF).m_vecEFInfo.begin();
			while ( iterEFInfo!=(*iterADF).m_vecEFInfo.end() )//EFInfor
			{
				m_strTemp = (*iterEFInfo).m_strSFI;
				transform(m_strTemp.begin(), m_strTemp.end(), m_strTemp.begin(), toupper); 
				if ( "EF05" != m_strTemp )//非EF05
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
						//取卡号数据
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
	if ( strCardNo.empty() )//不存在该项数据的时候不检测
		return;

	string strTemp;
	stringstream strsTmp;
	if ( strCardNo.compare(0, 3, "CON")==0 )//不检测
		return;
	if ( strCardNo.length() != 9 )//an属性长度9
	{
		Append_Error("<INFO><MF><SSSE><DF00>下<EF05 Tag=\"07\"/>的Val卡号长度必须为9位");
		return;
	}
	for( size_t i=0; i<strCardNo.length(); ++i )
	{
		if ( (strCardNo.at(i)<='9' && strCardNo.at(i)>='0') || (strCardNo.at(i)<='Z' && strCardNo.at(i)>='A') )
		{}
		else
		{
			Append_Error("<INFO><MF><SSSE><DF00>下<EF05 Tag=\"07\"/>的Val卡号值必须由八位数字(或大写拉丁字母)和一位数字(或大写拉丁字母)校验码组成");
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
		else//大写拉丁字母
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
		strsTmp<<"<INFO><MF><SSSE><DF00>下<EF05 Tag=\"07\"/>的Val卡号值最后一位校验码错误,正确的卡号为:"<<strCardNo.substr(0,8)<<strRes;
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
	strLoadKeyStyle = S_SETTING.m_struLoadKeyInfo.m_strMode;//返回装载密钥的方式
	
	return true;
}
