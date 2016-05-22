/************************************************************************/
/*
/*	class CInformation
/*
/*	信息记录类（读配置检查配置，记录生成脚本数据、错误信息、警告信息、日志等）
/*  最后修改日期：20151203 17:30
/************************************************************************/
#pragma once
#include <string>
#include <vector>
#include <sstream>
#include <set>

using namespace std;
#define S_SETTING CInformation::s_struSetting
#define V_EKEYS CInformation::s_vecKEYS
#define S_CARDNFO CInformation::s_struCardInfo

//项目修改记录信息
struct PROJMOD{
	string m_strAuthor;//作者
	string m_strModify;//修改备注信息
	string m_strDate;//修改日期
	string m_strVer;//版本
};
//项目信息
struct PROJINFO{
	string m_strName;//项目名称
	string m_strDesc;//描述
	vector<PROJMOD> m_vecProjMod;//项目修改记录
};
//个人识别码
struct PIN{
	string m_strValue;//个人识别码
	string m_strCount;//错误计数
};
//城市代码
struct CITYCODE{
	string m_strValue;//城市代码
	string m_strProvince;//省
	string m_strCity;//市
};
//装载密钥相关信息
struct LOADKEYINFO{
	string m_strMode;//装载密钥的方式 1：加密机装载 2：固定密钥装载
	string m_strType;//装载密钥类型 0：密文	1：明文	10：密文更新	11：明文更新
	string m_strScale;//级别	2：全省通集中模式	1：地级市集中模式
	string m_strPKeyNo;//保护密钥ID
	string m_strPKeyfsjs;//保护密钥（次主密钥）分散级数
	string m_strPKeyfsyz;//保护密钥（次主密钥）分散因子
	string m_strPKeyLen;//保护密钥（次主密钥）的长度
};
//<SETTING>数据
struct SETTING{
	PROJINFO m_struProjInfo;
	PIN m_struPIN;
	CITYCODE m_struCityCode;
	LOADKEYINFO m_struLoadKeyInfo;
};


//密钥数据
struct KEYDATAS{
	string m_strName;//密钥名称
	string m_strKeyIndex;//密钥标识
	string m_strVer;//密钥版本开始版本号
	string m_strPSAMKeyType;//psam卡密钥用途
	string m_strPSAMKeyID;//psam卡密钥标识
	string m_strAddr;//加密机中存储密钥的地址
	string m_strType;//密钥类型
	string m_strLen;//密钥长度(十进制单位字节)
	string m_strGroup;//密钥组数(与Addr组数一一对应)
	string m_strCount;//错误计数
	string m_strValue;//密钥值
	int m_nSeq;//用于装载密钥时候设置密钥的安全级别(注意STK和IRK等这些没有安全级别的填充00)
};
//<DDF>标签密钥信息
struct ADFKEY{
	string m_strADFName;//ADF文件名
	vector<KEYDATAS> m_vecKeyDatas;//密钥数据
};
//<SSSEKEYS>和<OTHERKEYS>标签密钥信息
struct KEYS{
	string m_strDDFName;//DDF文件名
	vector<ADFKEY> m_vecADFKEY;//ADF密钥信息
};


//EF额外数据,循环文件或者B-TLV数据
struct EFEXTRADATAS{
	//循环文件数据和B-TLV数据共同属性
	string m_strP1;//记录号
	string m_strTag;//标识
	string m_strLen;//长度
	string m_strVal;//值
	string m_strrType;//类型(an,cn,b)

	//循环文件特有的属性
	string m_strRemark;//备注
};
//EF数据
struct EFDATAS{
	string m_strP1;//记录号
	string m_strTag;//标识
	string m_strLen;//长度
	string m_strVal;//值
	string m_strrType;//属性(an,cn,b,B-TLV,-循环记录)
	string m_strRemark;//备注
	vector<EFEXTRADATAS> m_vecEFExDatas;
};
//EF文件信息
struct EFINFO{
	string m_strSFI;//短文件标识符,如EF05、EF06等
	string m_strReadControl;//读控制（取值：空、PIN、KEY（如RKSSSE）、PIN|KEY、PIN+KEY、禁止（FF））
	string m_strWriteControl;//写控制
	string m_strEFType;//基本文件类型 1：定长文件	2：变长文件	3：透明文件	4：循环文件	5：内部文件
	string m_strbInit;//是否初始化（0：否	1：是）
	string m_strnCount;//循环文件记录数
	string m_strResSpace;//预留空间
	vector<EFDATAS> m_vecEFDatas;
};
//ADF文件信息
struct ADFINFO{
	string m_strADFName;//ADF名称如DF00、DF01、DF02等（DF00表示该层结构不存在，即该ADF下的的EF直属上一层的DDF）
	string m_strMKvalue;//主控密钥值
	string m_strCount;//错误计数
	string m_strUpdateMode1;//工作密钥更新使用主控（0：当前MK	1：父目录MK）
	string m_strUpdateMode2;//当前MK更新使用主控（0：当前MK	1：父目录MK）
	string m_strFID;//文件标识符
	string m_strAID;//应用标识符
	vector<EFINFO> m_vecEFInfo;
};
//DDF文件信息
struct DDFINFO{
	string m_strDDFName;//DDF名称如DDF0、SSSE、PSE等（DDF0表示该层结构不存在，即该DDF下的ADF直属上一层的MF）
	string m_strMKvalue;//主控密钥值
	string m_strCount;//错误计数
	string m_strSpace;//空间
	string m_strUpdateMode1;//工作密钥更新使用主控（0：当前MK	1：父目录MK）
	string m_strUpdateMode2;//当前MK更新使用主控（0：当前MK	1：父目录MK）
	string m_strFID;//文件标识符
	string m_strAID;//应用标识符
	vector<ADFINFO> m_vecADFInfo;
};
//<MF>卡片结构数据
struct MFINFO{
	string m_strMFName;//MF名称
	string m_strMKvalue;//主控密钥值
	string m_strCount;//错误计数
	string m_strFID;//文件标识符
	string m_strAID;//应用标识符
	vector<DDFINFO> m_vecDDFInfo;
};

class CInformation
{
public:
	CInformation(void);
	~CInformation(void);

	/************************************************************************/
	/*名    称:	ReadXMLDatas_YK
	/*功    能:	读取验卡xml中的数据并存储
	/*输入参数:	strXmlPath：XML路径  nCheckLog:为1时表示生成日志
	/*输出参数:	strLoadKeyStyle：装载密钥的方式 1：加密机装载 2：固定密钥装载	
	/*返 回 值:	true	是脚本配置文件	false	不是脚本配置文件(缺失脚本配置文件中必须的标签)
	/************************************************************************/
	bool ReadXMLDatas_YK( string strXmlPath, string &strLoadKeyStyle, int nCheckLog=1 );
	/************************************************************************/
	/*名    称:	ReadXMLDatas
	/*功    能:	读取发卡xml中的数据并存储
	/*输入参数:	strXmlPath：XML路径  nCheckLog:为1时表示生成日志
	/*输出参数:	null
	/*返 回 值:	true	是脚本配置文件	false	不是脚本配置文件(缺失脚本配置文件中必须的标签)
	/************************************************************************/
	bool ReadXMLDatas( string strXmlPath, int nCheckLog=1 );
	/************************************************************************/
	/*名    称:	CheckXmlDatas
	/*功    能:	检测发卡XML内SETTING、SSSEKEYS、MF、DDF、ADF、EF数据的合法性
	/*输入参数:	null
	/*输出参数:	null
	/*返 回 值:	null
	/************************************************************************/
	void CheckXmlDatas();
	/************************************************************************/
	/*名    称:	Errors
	/*功    能:	返回在执行过程中所发生的错误总数
	/*输入参数:	null
	/*输出参数:	null
	/*返 回 值:	0：之前的执行中并没有发生任何错误	>0：错误个数
	/************************************************************************/
	int Errors();
	/************************************************************************/
	/*名    称:	Warnings
	/*功    能:	返回在执行过程中所发生的警告总数
	/*输入参数:	null
	/*输出参数:	null
	/*返 回 值:	0：之前的执行中并没有发生任何警告	>0：警告个数
	/************************************************************************/
	int Warnings();
	/************************************************************************/
	/*名    称:	Append_Error(string)
	/*功    能:	添加错误信息,该函数内部在添加错误信息时，将自动将提示字串和错误类型整合到信息里面
	/*输入参数:	strErrMsg：错误信息
	/*输出参数:	null
	/*返 回 值:	null
	/************************************************************************/
	void Append_Error( string strErrMsg );
	/************************************************************************/
	/*名    称:	Append_Warning
	/*功    能:	添加警告信息
	/*输入参数:	Warning: 需添加的警告信息
	/*输出参数:	null
	/*返 回 值:	null
	/************************************************************************/
	void Append_Warning( string strWarning );
	void Append_Script( string strDatas );//添加脚本信息
	void Clear_Script();//清空脚本信息

	static SETTING s_struSetting;//标签SETTING内的数据
	static vector<KEYS> s_vecKEYS;//所有密钥数据
	static MFINFO s_struCardInfo;//卡片的结构数据
	vector<string> m_vecErrData;	//错误信息
	vector<string> m_vecWrnData;	//警告信息
	vector<string> m_vecScriptData;	//脚本生成数据

private:
	/************************************************************************/
	/*名    称:	CheckXMLFile
	/*功    能:	检测xml文件是否为脚本配置文件，不是的话将确实的配置记录到错误信息里
	/*输入参数:	strXmlPath：XML路径
	/*输出参数:	null
	/*返 回 值:	true	是脚本配置文件	false	不是脚本配置文件(缺失脚本配置文件中必须的标签)
	/************************************************************************/
	bool CheckXMLFile( string strXmlPath );
	/************************************************************************/
	/*名    称:	ReadSettingDatas
	/*功    能:	读取xml脚本标签setting中的数据并存储
	/*输入参数:	strXmlPath：XML路径
	/*输出参数:	null
	/*返 回 值:	null
	/************************************************************************/
	void ReadSettingDatas( string strXmlPath );
	/************************************************************************/
	/*名    称:	ReadKeysDatas
	/*功    能:	读取xml脚本标签KEYS中的数据
	/*输入参数:	strXmlPath：XML路径
	/*输出参数:	null
	/*返 回 值:	null
	/************************************************************************/
	void ReadKeysDatas( string strXmlPath );
	/************************************************************************/
	/*名    称:	ReadCardInfo
	/*功    能:	读取xml脚本里的卡片结构数据信息并存储（读取的数据作去除字符串前后空格处理）
	/*输入参数:	strXmlPath：XML路径
	/*输出参数:	null
	/*返 回 值:	null
	/************************************************************************/
	void ReadCardInfo( string strXmlPath );
	/************************************************************************/
	/*名    称:	AddKeysDatas
	/*功    能:	将标签KEYS中读取的数据保存到数据结构中,根据strTagName和strName添加数据
	/*输入参数:	strTagName: 标签名		strName：密钥名称	strKeyIndex：密钥标识	strVer：密钥版本开始版本号	
	/*			strPSAMKeyType：psam卡密钥用途		strPSAMKeyID:psam卡密钥标识		strAddr：加密机中存储密钥的地址	
	/*			strType：密钥类型	strLen：密钥长度	strGroup：密钥组数	strCount：错误计数	strValue：密钥值
	/*输出参数:	null
	/*返 回 值:	null
	/************************************************************************/
	void AddKeysDatas(string strTagName, string strName, string strKeyIndex, string strVer, string strPSAMKeyType, string strPSAMKeyID, string strAddr, string strType, string strLen, string strGroup, string strCount, string strValue);
	/************************************************************************/
	/*名    称:	ModeKeysDatasSeq
	/*功    能:	将PIN数据添加到KEYS的结构体中
	/*输入参数:	null
	/*输出参数:	null
	/*返 回 值: null
	/************************************************************************/
	void ModeKeysDatasSeq();
	/************************************************************************/
	/*名    称:	CheckMFDatas
	/*功    能:	检测MF的数据
	/*输入参数:	strTgNames：该数据所属标签
	/*输出参数:	null
	/*返 回 值:	true	数据都合法	false	存在不合法数据
	/************************************************************************/
	bool CheckMFDatas( string strTgNames );
	/************************************************************************/
	/*名    称:	CheckDDFDatas
	/*功    能:	检测DDF的数据
	/*输入参数:	strTgNames：该数据所属标签	struDDFInfo：检测的DDF数据信息
	/*输出参数:	null
	/*返 回 值:	true	数据都合法	false	存在不合法数据
	/************************************************************************/
	bool CheckDDFDatas( string strTgNames, const DDFINFO &struDDFInfo );
	/************************************************************************/
	/*名    称:	CheckADFDatas
	/*功    能:	检测ADF的数据
	/*输入参数:	strTgNames：该数据所属标签	struADFInfo：检测的ADF数据信息
	/*输出参数:	null
	/*返 回 值:	true	数据都合法	false	存在不合法数据
	/************************************************************************/
	bool CheckADFDatas( string strTgNames, const ADFINFO &struADFInfo );
	/************************************************************************/
	/*名    称:	CheckEFDatas
	/*功    能:	检测EF的数据
	/*输入参数:	strDDFName: 当前的DDF名	 strTgNames：该数据所属标签	struEFInfo：检测的EF数据信息	strADFName：ADF名称
	/*输出参数:	null
	/*返 回 值:	true	数据都合法	false	存在不合法数据
	/************************************************************************/
	bool CheckEFInfor( string strDDFName, string strTgNames, const EFINFO &struEFInfo, string strADFName );
	/************************************************************************/
	/*名    称:	CheckEFControlKey
	/*功    能:	检测EF的控制密钥是否存在
	/*输入参数:	strDDFName: 当前的DDF名	 strProperty：属性名称	strKey：EF的控制密钥值	strADFName：ADF名称
	/*输出参数:	null
	/*返 回 值:	true	密钥存在	false	密钥不存在
	/************************************************************************/
	bool CheckEFControlKey( string strDDFName, string strProperty, string strKey, string strADFName );
	/************************************************************************/
	/*名    称:	CheckControlKeyExis
	/*功    能:	检测密钥是否存在
	/*输入参数:	strDDF: 应用目录,可以取值为应用环境简称（如"SSSE"、"PSE"等,即DDFName）(不区分大小写)，DDF0表示目录不存在
	/*			strADF：文件目录,可以取值为ADF名称（如"DF01"、"DF02"等,即ADFName）(不区分大小写)， DF00表示目录不存在
	/*			strKey: 指定的密钥名称(不区分大小写)
	/*输出参数:	null
	/*返 回 值:	true	密钥存在	false	密钥不存在
	/************************************************************************/
	bool CheckControlKeyExis( string strDDF, string strADF, string strKey ); 
	/************************************************************************/
	/*名    称:	CheckEFDatas
	/*功    能:	检测<EF05>等标签内数据的合法性
	/*输入参数:	strEFType：EF的类型	struEFDatas：检测的EF数据结构体
	/*输出参数:	null
	/*返 回 值:	true	数据都合法	false	存在不合法数据
	/************************************************************************/
	bool CheckEFDatas( string strEFType, const EFDATAS &struEFDatas );
	/************************************************************************/
	/*名    称:	CheckEFExDatas
	/*功    能:	检测B-TLV和循环文件数据的合法性
	/*输入参数:	strSFI：短文件名	strTag：B-TLV的Tag标签名	
	/*			strEFType：EF的类型	struEFExDatas：保存B-TLV和循环文件的数据结构体
	/*输出参数:	null
	/*返 回 值:	true	数据都合法	false	存在不合法数据
	/************************************************************************/
	bool CheckEFExDatas( string strSFI, string strTag, string strEFType, const EFEXTRADATAS &struEFExDatas );
	/************************************************************************/
	/*名    称:	CheckSETTINGDatas
	/*功    能:	检测<SETTING>标签内数据的合法性
	/*输入参数:	null
	/*输出参数:	null
	/*返 回 值:	true	数据都合法	false	存在不合法数据
	/************************************************************************/
	bool CheckSETTINGDatas();
	/************************************************************************/
	/*名    称:	CheckKEYSSDatas
	/*功    能:	检测读取的所有密钥数据的合法性
	/*输入参数:	null
	/*输出参数:	null
	/*返 回 值:	true	数据都合法	false	存在不合法数据
	/************************************************************************/
	bool CheckKEYSSDatas();
	/************************************************************************/
	/*名    称:	CheckNecessaryKeys
	/*功    能:	检测密钥数据中某些密钥必须存在,例如SSSE下的IRK必须存在需要计算个人识别码等
	/*输入参数:	null
	/*输出参数:	null
	/*返 回 值:	true	需要检测的数据都存在	false	其中有某些必要的密钥不存在
	/************************************************************************/
	bool CheckNecessaryKeys();
	/************************************************************************/
	/*名    称:	WriteXmlDatasLog
	/*功    能:	将读取的xml数据写到日志
	/*输入参数:	null
	/*输出参数:	null
	/*返 回 值:	null
	/************************************************************************/
	void WriteXmlDatasLog();
	/************************************************************************/
	/*名    称:	CheckDF04
	/*功    能:	检测社保应用下AID=D15600000503对应的社会保险2目录文件下的EF文件，该文件涉及医疗消费的相关功能，如果缺少某些文件的时候给出警告  
	/*输入参数:	null
	/*输出参数:	null
	/*返 回 值:	null
	/************************************************************************/
	void CheckDF04();
	/************************************************************************/
	/*名    称:	CheckCardNo
	/*功    能:	社会保障卡序列号（对应SSSE下EF05里面的卡号字段数据）由八位数字(或大写拉丁字母)本体代码和一位数字(或大写拉丁字母)校验码组成，检测数据合法性
	/*输入参数:	null
	/*输出参数:	null
	/*返 回 值:	null
	/************************************************************************/
	void CheckCardNo();
	/************************************************************************/
	/*名    称:	CheckPSAMType
	/*功    能:	校验PSAM卡密钥用途构成是否合法（根据社保PSAM卡应用指南提供资料判断）
	/*输入参数:	strPSAMType: PSAM卡密钥用途（确保数据为2位的十六进制字符串）
	/*输出参数:	null
	/*返 回 值:	0	数据合法	1	分散级数不合法	2	密钥类型不合法
	/************************************************************************/
	int CheckPSAMType( string strPSAMType );

	stringstream m_strsTemp; 
	string m_strTemp;
	set<string> m_setTag;//用于避免B-TLV数据重复检测
	int m_nErrorCnt;		//错误计数器
	int m_nWarningCnt;		//警告计数器
};
