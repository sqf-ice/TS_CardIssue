/************************************************************************/
/*公共头文件，定义一些公共的结构体
/************************************************************************/
#pragma once
#include <vector>

using namespace std;

//清卡参数配置
struct CLEARCARDPARAM{
	string m_strDirLv;//目录级别(数据字典：一级目录、二级目录)
	string m_strAE;//应用环境(数据字典：MF、社保应用、非社保应用)
	string m_strFID;//FID
	string m_strAID;//AID
	string m_strMK;//32位的密钥值
};