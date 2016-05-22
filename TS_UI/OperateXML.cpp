
#include "stdafx.h"
#include "OperateXML.h"

COperateXML::COperateXML(void)
{
}


COperateXML::~COperateXML(void)
{
}

void COperateXML::ReadOptionDatas( string srtXMLPath, int &nCos, int &nCheckKeys, int &nCheckATR, string &strSerialNum, string &strTranKey, int &nCheckLog )
{
	xml_document doc;
	doc.load_file(srtXMLPath.c_str());
	xml_node srcConf = doc.child("ScriptConf");

	xml_node cos = srcConf.child("COS");
	string strCos = cos.attribute("Type").value();
	nCos = atoi(strCos.c_str());

	xml_node keys = srcConf.child("Keys");
	string strCheckKeys = keys.attribute("Value").value();
	nCheckKeys = atoi(strCheckKeys.c_str());

	xml_node tran = srcConf.child("TranKey");
	strTranKey = tran.attribute("Value").value();

	xml_node atr = srcConf.child("ATR");
	string strCheckATR = atr.attribute("Value").value();
	nCheckATR = atoi(strCheckATR.c_str());
	strSerialNum = atr.attribute("SerialNum").value();

	xml_node log = srcConf.child("LOG");
	string strCheckLog = log.attribute("Value").value();
	nCheckLog = atoi(strCheckLog.c_str());
}

void COperateXML::ReadKeysDatas( string srtXMLPath, vector<CLEARCARDPARAM> &vecClearCardParam )
{
	CLEARCARDPARAM struClCard;
	vecClearCardParam.clear();
	xml_document doc;
	doc.load_file(srtXMLPath.c_str());
	xml_node srcConf = doc.child("ScriptConf");

	xml_node keys = srcConf.child("Keys");
	xml_named_node_iterator key = keys.children("Key").begin();
	while ( key != keys.children("Key").end() )
	{
		struClCard.m_strDirLv = (*key).attribute("DirLv").value();
		struClCard.m_strAE = (*key).attribute("AE").value();
		struClCard.m_strFID = (*key).attribute("FID").value();
		struClCard.m_strAID = (*key).attribute("AID").value();
		struClCard.m_strMK = (*key).attribute("MK").value();
		vecClearCardParam.push_back(struClCard);
		++key;
	}
}

void COperateXML::SaveConfDatas( string srtXMLPath, int nCos, int nCheckATR, string strSerialNum, string strTranKey, int nCheckKeys, int nCheckLog, vector<CLEARCARDPARAM> &vecClearCardParam )
{
	int i=0;
	stringstream strsTmp;
	xml_document doc;
	doc.load_file(srtXMLPath.c_str());
	xml_node srcConf = doc.child("ScriptConf");

	strsTmp.str("");
	strsTmp<<nCos;
	xml_node cosType = srcConf.child("COS");
	cosType.attribute("Type").set_value(strsTmp.str().c_str());

	xml_node keys = srcConf.child("Keys");
	strsTmp.str("");
	strsTmp<<nCheckKeys;
	keys.attribute("Value").set_value(strsTmp.str().c_str());
	if ( nCheckKeys == 1 )
	{
		for ( xml_node key = keys.first_child(); key; key = key.next_sibling() )
			++i;
		while ( i!=0 )
		{
			keys.remove_child("Key");
			--i;
		}

		vector<CLEARCARDPARAM>::const_iterator iter = vecClearCardParam.begin();
		while ( iter!=vecClearCardParam.end() )
		{
			xml_node key = keys.append_child("Key");
			if ( (*iter).m_strDirLv == "一级目录" )
				key.append_attribute("DirLv") = "1";
			else if ( (*iter).m_strDirLv == "二级目录" )
				key.append_attribute("DirLv") = "2";
			if ( (*iter).m_strAE == "MF" )
				key.append_attribute("AE") = "1";
			else if ( (*iter).m_strAE == "社保应用" )
				key.append_attribute("AE") = "2";
			else if ( (*iter).m_strAE == "非社保应用" )
				key.append_attribute("AE") = "3";
			key.append_attribute("FID") = (*iter).m_strFID.c_str();
			key.append_attribute("AID") = (*iter).m_strAID.c_str();
			key.append_attribute("MK") = (*iter).m_strMK.c_str();
			++iter;
		}
	}

	xml_node tran = srcConf.child("TranKey");
	tran.attribute("Value").set_value(strTranKey.c_str());

	strsTmp.str("");
	strsTmp<<nCheckATR;
	xml_node atr = srcConf.child("ATR");
	atr.attribute("Value").set_value(strsTmp.str().c_str());
	if ( nCheckATR == 1 )
		atr.attribute("SerialNum").set_value(strSerialNum.c_str());

	strsTmp.str("");
	strsTmp<<nCheckLog;
	xml_node log = srcConf.child("LOG");
	log.attribute("Value").set_value(strsTmp.str().c_str());

	doc.save_file(srtXMLPath.c_str());

}