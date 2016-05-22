/************************************************************************/
/*COperateXML	用于操作配置文件ScriptConfig.xml
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

	//读取基本配置参数 srtXMLPath配置文件路径 输出nCos芯片类型  nCheckKeys是否清卡  nCheckATR是否更新ATR  strSerialNum流水号  strTranKey传输密钥  nCheckLog是否写日志
	void ReadOptionDatas(string srtXMLPath, int &nCos, int &nCheckKeys, int &nCheckATR, string &strSerialNum, string &strTranKey, int &nCheckLog);
	//读取清卡配置参数 srtXMLPath配置文件路径 输出vecClearCardParam清卡相关数据
	void ReadKeysDatas(string srtXMLPath, vector<CLEARCARDPARAM> &vecClearCardParam);
	//保存基本配置参数 srtXMLPath配置文件路径 输入nCos芯片类型  nCheckATR是否更新ATR  strSerialNum流水号  strTranKey传输密钥  nCheckLog是否写日志  nCheckKeys是否清卡  vecClearCardParam清卡相关数据
	void SaveConfDatas(string srtXMLPath, int nCos, int nCheckATR, string strSerialNum, string strTranKey, int nCheckKeys, int nCheckLog, vector<CLEARCARDPARAM> &vecClearCardParam);

};

