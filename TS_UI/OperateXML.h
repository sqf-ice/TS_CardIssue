/************************************************************************/
/*COperateXML	���ڲ��������ļ�ScriptConfig.xml
/************************************************************************/
#pragma once
#include <string>
#include "Common.h"
#include <sstream>
#include "Script/pugixml.hpp"
using namespace pugi;

using namespace std;

class COperateXML
{
public:
	COperateXML(void);
	~COperateXML(void);

	//��ȡ�������ò��� srtXMLPath�����ļ�·�� ���nCosоƬ����  nCheckKeys�Ƿ��忨  nCheckATR�Ƿ����ATR  strSerialNum��ˮ��  strTranKey������Կ  nCheckLog�Ƿ�д��־
	void ReadOptionDatas(string srtXMLPath, int &nCos, int &nCheckKeys, int &nCheckATR, string &strSerialNum, string &strTranKey, int &nCheckLog);
	//��ȡ�忨���ò��� srtXMLPath�����ļ�·�� ���vecClearCardParam�忨�������
	void ReadKeysDatas(string srtXMLPath, vector<CLEARCARDPARAM> &vecClearCardParam);
	//����������ò��� srtXMLPath�����ļ�·�� ����nCosоƬ����  nCheckATR�Ƿ����ATR  strSerialNum��ˮ��  strTranKey������Կ  nCheckLog�Ƿ�д��־  nCheckKeys�Ƿ��忨  vecClearCardParam�忨�������
	void SaveConfDatas(string srtXMLPath, int nCos, int nCheckATR, string strSerialNum, string strTranKey, int nCheckKeys, int nCheckLog, vector<CLEARCARDPARAM> &vecClearCardParam);

};

