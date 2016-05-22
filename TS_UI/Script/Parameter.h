/************************************************************************/
/*
/*	class CParameter
/*
/*	参数类（用以获取程序中所有需要的参数）
/*	最后修改日期：20151113 17:30
/************************************************************************/
#pragma once
#include "Information.h"


//EF文件信息
struct S_EFINFO{
	string m_strSFI;//短文件标识符,如EF05、EF06等
	string m_strReadControl;//读控制（取值：空、PIN、KEY（如RKSSSE）、PIN|KEY、PIN+KEY、禁止（FF））
	string m_strWriteControl;//写控制
	string m_strEFType;//基本文件类型 1：定长文件	2：变长文件	3：透明文件	4：循环文件	5：内部文件
	string m_strbInit;//是否初始化（0：否	1：是）
	string m_strnCount;//循环文件记录数
	string m_strResSpace;//预留空间
	string m_strEFLen;//文件内所有数据项的总长度
	int m_nRecordCount;//该EF文件下的数据项总数
};
//ADF文件信息
struct S_ADFINFO{
	string m_strADFName;//ADF名称如DF00、DF01、DF02等（DF00表示该层结构不存在，即该ADF下的的EF直属上一层的DDF）
	string m_strMKvalue;//主控密钥值
	string m_strCount;//错误计数
	string m_strUpdateMode1;//工作密钥更新使用主控（0：当前MK	1：父目录MK）
	string m_strUpdateMode2;//当前MK更新使用主控（0：当前MK	1：父目录MK）
	string m_strFID;//文件标识符
	string m_strAID;//应用标识符
	vector<S_EFINFO> m_vecEFInfo;
};

//DDF文件信息
struct S_DDFINFO{
	string m_strDDFName;//DDF名称如DDF0、SSSE、PSE等（DDF0表示该层结构不存在，即该DDF下的ADF直属上一层的MF）
	string m_strMKvalue;//主控密钥值
	string m_strCount;//错误计数
	string m_strSpace;//空间
	string m_strUpdateMode1;//工作密钥更新使用主控（0：当前MK	1：父目录MK）
	string m_strUpdateMode2;//当前MK更新使用主控（0：当前MK	1：父目录MK）
	string m_strFID;//文件标识符
	string m_strAID;//应用标识符
	vector<S_ADFINFO> m_vecADFInfo;
};

//MF文件信息
struct S_MFINFO{
	string m_strMFName;//MF名称
	string m_strMKvalue;//主控密钥值
	string m_strCount;//错误计数
	string m_strFID;//文件标识符
	string m_strAID;//应用标识符
};

//用于存储脚本说明里面的常量说明数据
struct CONDECLAR
{
	string m_strCON;//常量
	string m_strUserFields;//用户字段
	string m_strName;//中文名称
	string m_strParentDir;//所属文件
	string m_strType;//类型
	string m_strLen;//长度
};

class CParameter
{
public:
	CParameter( CInformation *cInf );
	~CParameter(void);

	/************************************************************************/
	/*名    称:	GetCardStruct
	/*功    能:	获取MF建卡的目录结构（DDF、ADF、EF文件结构和信息）
	/*输入参数:	null	
	/*输出参数:	vecMFStruct: 存储MF下目录结构的容器
	/*返 回 值:	null
	/*注    意：读取出来的数据中,若m_strDDFName
	/************************************************************************/
	void GetCardStruct( vector<S_DDFINFO> &vecMFStruct );
	/************************************************************************/
	/*名    称:	GetEFInfo
	/*功    能:	根据DDF\ADF\EF获取EF文件信息
	/*输入参数:	strDDF: 应用目录,可以取值为应用环境简称（如"SSSE"、"PSE"等）或FID文件标志符（如"DDF1"、"DDF2"等）或保留为空"" (不区分大小写)
	/*			strADF：文件目录,可以取值为FID文件标识符（如"DF01"、"DF02"等）或保留为空"" (不区分大小写)	
	/*			strEF: 基本文件,可以取值为FID文件标识符（如"EF05"、"EF06"等） (不区分大小写)
	/*输出参数:	struEFInfo: 存储当前DDF\ADF\EF文件信息的结构体
	/*返 回 值:	true	成功	false	未找到指定MF\DDF\ADF\EF相关文件信息
	/*注    意：若DDF不存在,即当需要获取MF\ADF\EF05时,调用方式为GetEFInfo( "", "ADF", "EF05", struEFInfo )	
	/*			若ADF不存在,即当需要获取MF\DDF\EF05时,调用方式为GetEFInfo( "DDF", "", "EF05", struEFInfo )
	/*			若DDF、ADF都不存在,即当需要获取MF\EF05时,调用方式为GetEFInfo( "", "", "EF05", struEFInfo )
	/************************************************************************/
	bool GetEFInfo( string strDDF, string strADF, string strEF, S_EFINFO &struEFInfo );
	/************************************************************************/
	/*名    称:	GetADFInfo
	/*功    能:	根据DDF\ADF获取ADF文件信息
	/*输入参数:	strDDF: 应用目录,可以取值为应用环境简称（如"SSSE"、"PSE"等）或FID文件标志符（如"DDF1"、"DDF2"等）或保留为空"" (不区分大小写)
	/*			strADF：文件目录,可以取值为FID文件标识符（如"DF01"、"DF02"等）(不区分大小写)	
	/*输出参数:	struADFInfo: 存储当前DDF\ADF文件信息的结构体,输出结构体中的成员m_vecEFInfo为空,不需要取该成员数据
	/*返 回 值:	true	成功	false	未找到指定MF\DDF\ADF相关文件信息
	/*注    意：若DDF不存在,即当需要获取MF\ADF时,调用方式为GetADFInfo( "", "ADF", struADFInfo )
	/************************************************************************/
	bool GetADFInfo( string strDDF, string strADF, S_ADFINFO &struADFInfo );
	/************************************************************************/
	/*名    称:	GetDDFInfo
	/*功    能:	根据DDF名称获取MF\DDF文件信息
	/*输入参数:	strDDF: 应用目录,可以取值为应用环境简称（如"SSSE"、"PSE"等）或FID文件标志符（如"DDF1"、"DDF2"等）(不区分大小写)	
	/*输出参数:	struDDFInfo: 存储当前DDF文件信息的结构体,输出结构体中的成员m_vecADFInfo为空,不需要取该成员数据
	/*返 回 值:	true	成功	false	未找到指定DDF相关文件信息
	/************************************************************************/
	bool GetDDFInfo( string strDDF, S_DDFINFO &struDDFInfo );
	/************************************************************************/
	/*名    称:	GetMFInfo
	/*功    能:	获取MF文件信息
	/*输入参数:	null	
	/*输出参数:	struMFInfo: 存储MF文件信息的结构体
	/*返 回 值:	null
	/************************************************************************/
	void GetMFInfo( S_MFINFO &struMFInfo );
	/************************************************************************/
	/*名    称:	GetKeyInfo
	/*功    能:	根据指定DDF\ADF下的密钥名称获取密钥信息(PIN存储在SSSE下)
	/*输入参数:	strDDF: 应用目录,可以取值为应用环境简称（如"SSSE"、"PSE"等,即DDFName）或保留为空"" (不区分大小写)
	/*			strADF：文件目录,可以取值为FID文件标识符（如"DF01"、"DF02"等）或保留为空"" (不区分大小写)（因为参数检测时候ADF的FID和标签名必须一致,所以根据FID也能找到KEYS里面对应的标签名）
	/*			strKeyName: 指定的密钥名称(不区分大小写)
	/*输出参数:	struKeyDatas: 存储指定密钥信息的结构体
	/*返 回 值:	true	获取密钥信息成功	false	指定的DDF/ADF下不存在指定的密钥
	/*注    意：若DDF不存在,即当需要获取MF\ADF下的密钥UKMF时,调用方式为GetKeyInfo( "", "ADF", "UKMF", struKeyDatas )
	/*			若ADF不存在,即当需要获取MF\DDF下的密钥UKMF时,调用方式为GetKeyInfo( "DDF", "", "UKMF", struKeyDatas )
	/*			若DDF、ADF都不存在,即当需要获取MF下的密钥UKMF时,调用方式为GetEFInfo( "", "", "UKMF", struEFInfo )
	/************************************************************************/
	bool GetKeyInfo( string strDDF, string strADF, string strKeyName, KEYDATAS &struKeyDatas );
	/************************************************************************/
	/*名    称:	GetADFKeys
	/*功    能:	获取指定DDF\ADF下的所有密钥信息
	/*输入参数:	strDDF: 应用目录,可以取值为应用环境简称（如"SSSE"、"PSE"等,即DDFName）或保留为空"" (不区分大小写)
	/*			strADF：文件目录,可以取值为FID文件标识符（如"DF01"、"DF02"等）或保留为空"" (不区分大小写)（因为参数检测时候ADF的FID和标签名必须一致,所以根据FID也能找到KEYS里面对应的标签名）	
	/*输出参数:	vecKeyDatas: 存储指定ADF下所有密钥信息的容器
	/*返 回 值:	true	获取密钥信息成功	false	指定的DDF\ADF不存在
	/*注    意：若DDF不存在,即当需要获取MF\ADF下的所有密钥信息时,调用方式为GetADFKeys( "", "ADF", vecKeyDatas )
	/*			若ADF不存在,即当需要获取MF\DDF下的所有密钥信息时,调用方式为GetADFKeys( "DDF", "", vecKeyDatas )
	/*			若DDF、ADF都不存在,即当需要获取MF下的所有密钥信息时,调用方式为GetADFKeys( "", "", vecKeyDatas )
	/************************************************************************/
	bool GetADFKeys( string strDDF, string strADF, vector<KEYDATAS> &vecKeyDatas );
	/************************************************************************/
	/*名    称:	GetKEYS
	/*功    能:	获取某些DDF的密钥
	/*输入参数:	nMode: 模式 1：获取全部密钥包括SSSE和非SSSE的所有密钥  2：获取SSSE的密钥  3：获取非SSSE的所有密钥
	/*输出参数:	vecKeys: 存储获取的密钥信息的容器
	/*返 回 值:	true	获取密钥信息成功	false	需要获取的密钥不存在
	/************************************************************************/
	bool GetKEYS( int nMode, vector<KEYS> &vecKeys );
	/************************************************************************/
	/*名    称:	ModeKeySeq
	/*功    能:	修改某条指定密钥的安全级别
	/*输入参数:	strDDF: 应用目录,可以取值为应用环境简称（如"SSSE"、"PSE"等,即DDFName）或保留为空"" (不区分大小写)
	/*			strADF：文件目录,可以取值为FID文件标识符（如"DF01"、"DF02"等）或保留为空"" (不区分大小写)（因为参数检测时候ADF的FID和标签名必须一致,所以根据FID也能找到KEYS里面对应的标签名）	
	/*			strKeyName: 指定的密钥名称(不区分大小写)
	/*			nSeq：密钥的安全状态(取值范围为0-15)
	/*输出参数:	null
	/*返 回 值:	true	成功	false	失败
	/************************************************************************/
	bool ModeKeySeq( string strDDF, string strADF, string strKeyName, int nSeq );
	/************************************************************************/
	/*名    称:	GetEFDatas
	/*功    能:	获取指定DDF\ADF\EF的所有记录数据(返回的数据中vecEFDatas按EFDATAS的P1升序排序，EFDATAS中的m_vecEFExDatas按P1升序排序)
	/*输入参数:	strDDF: 应用目录,可以取值为应用环境简称（如"SSSE"、"PSE"等）或FID文件标志符（如"DDF1"、"DDF2"等）或保留为空"" (不区分大小写)
	/*			strADF：文件目录,可以取值为FID文件标识符（如"DF01"、"DF02"等）或保留为空"" (不区分大小写)	
	/*			strEF: 基本文件,可以取值为FID文件标识符（如"EF05"、"EF06"等） (不区分大小写)	
	/*输出参数:	vecEFDatas: 存储指定DDF\ADF\EF所有记录数据的容器
	/*返 回 值:	true	获取DDF\ADF\EF记录数据成功	false	需要获取的数据不存在
	/*注    意：若DDF不存在,即当需要获取MF\ADF\EF05下的所有记录时,调用方式为GetEFDatas( "", "ADF", "EF05", struADFInfo )
	/*			若ADF不存在,即当需要获取MF\DDF\EF05下的所有记录时,调用方式为GetEFDatas( "DDF", "", "EF05", struADFInfo )
	/*			若DDF、ADF都不存在,即当需要获取MF\EF05下的所有记录时,调用方式为GetEFDatas( "", "", "EF05", struADFInfo )
	/************************************************************************/
	bool GetEFDatas( string strDDF, string strADF, string strEF, vector<EFDATAS> &vecEFDatas );
	/************************************************************************/
	/*名    称:	GetEFData
	/*功    能:	获取MF下指定DDF\ADF\EF下某记录号P1对应的记录数据(若为B-TLV或者循环数据时，返回的数据中EFDATAS中的m_vecEFExDatas按P1升序排序)
	/*输入参数:	strDDF: 应用目录,可以取值为应用环境简称（如"SSSE"、"PSE"等）或FID文件标志符（如"DDF1"、"DDF2"等）或保留为空"" (不区分大小写)
	/*			strADF：文件目录,可以取值为FID文件标识符（如"DF01"、"DF02"等）或保留为空"" (不区分大小写)	
	/*			strEF: 基本文件,可以取值为FID文件标识符（如"EF05"、"EF06"等） (不区分大小写)	
	/*			nP1：需要获取的第几条记录的序号(如果是循环文件或者透明文件时,因为只有一条数据,所以nP1可取任意值,都默认取第一条数据)
	/*输出参数:	struEFDatas: 存储指定DDF\ADF\EF下某记录号P1对应记录数据的结构体
	/*返 回 值:	true	获取记录数据成功	false	需要获取的数据不存在
	/*注    意：若DDF不存在,即当需要获取MF\ADF\EF05下P1=1的记录时,调用方式为GetEFData( "", "ADF", "EF05", 1, struADFInfo )
	/*			若ADF不存在,即当需要获取MF\DDF\EF05下P1=1的记录时,调用方式为GetEFData( "DDF", "", "EF05", 1, struADFInfo )
	/*			若DDF、ADF都不存在,即当需要获取MF\EF05下P1=1的记录时,调用方式为GetEFData( "", "", "EF05", 1, struADFInfo )	
	/************************************************************************/
	bool GetEFData( string strDDF, string strADF, string strEF, int nP1, EFDATAS &struEFDatas );
	/************************************************************************/
	/*名    称:	GetProjInfo
	/*功    能:	获取项目相关信息包括作者、脚本修改日期等
	/*输入参数:	null	
	/*输出参数:	struProjInfo: 存储PROJ信息的结构体
	/*返 回 值:	null
	/************************************************************************/
	void GetProjInfo( PROJINFO &struProjInfo );
	/************************************************************************/
	/*名    称:	GetCityCode
	/*功    能:	获取城市代码
	/*输入参数:	null	
	/*输出参数:	struCityCode: 存储城市代码信息的结构体
	/*返 回 值:	null
	/************************************************************************/
	void GetCityCode( CITYCODE &struCityCode );
	/************************************************************************/
	/*名    称:	GetLoadKeyInfo
	/*功    能:	获取装载密钥相关信息
	/*输入参数:	null	
	/*输出参数:	struLoadKeyInfo: 存储装载密钥相关信息的结构体
	/*返 回 值:	null
	/************************************************************************/
	void GetLoadKeyInfo( LOADKEYINFO &struLoadKeyInfo );
	/************************************************************************/
	/*名    称:	GetCONStatement
	/*功    能:	获取脚本说明里面的常量说明数据
	/*输入参数:	null
	/*输出参数:	vecCONDeclar: 存储常量说明数据的容器
	/*返 回 值:	null
	/************************************************************************/
	void GetCONStatement( vector<CONDECLAR> &vecCONDeclar );
	/************************************************************************/
	/*名    称:	Internal_FID
	/*功    能:	确定安全文件、控制信息文件等五个文件的FID,如果遇到xml配置中有重复的FID,则FID递增,直到确定下唯一的FID为止
	/*输入参数:	null	
	/*输出参数:	strSF_SFI：安全文件短文件标识符（密钥文件）（为空的时候不处理该数据，如果不为长度不大于2位的十六进制数的话，程序会默认赋值为空）
	/*			strFCI_SFI：控制信息文件短文件标识符（为空的时候不处理该数据，如果不为长度不大于2位的十六进制数的话，程序会默认赋值为空）
	/*			strDIR_SFI：目录文件短文件标识符（为空的时候不处理该数据，如果不为长度不大于2位的十六进制数的话，程序会默认赋值为空）
	/*			strTF_SFI：内部交易文件短文件标识符（为空的时候不处理该数据，如果不为长度不大于2位的十六进制数的话，程序会默认赋值为空）
	/*			strATR_SFI：复位应答文件短文件标识符（为空的时候不处理该数据，如果不为长度不大于2位的十六进制数的话，程序会赋默认值为空）
	/*返 回 值:	null
	/************************************************************************/
	void Internal_SFI(string &strSF_SFI, string &strFCI_SFI, string &strDIR_SFI, string &strTF_SFI, string &strATR_SFI);

private:
	/************************************************************************/
	/*名    称:	CheckEF_Exist
	/*功    能:	遍历一遍MF,检测参数中的FID是否在XML中存在重复，遇到重复的话自身FID值递增
	/*输入参数:	null
	/*输出参数:	strFID：EF文件的FID
	/*返 回 值:	true	FID值没有发生改变	false	FID值发生了改变
	/************************************************************************/
	bool CheckEF_Exist( string &strFID );

	CInformation *const m_cInfo;
	string m_strTemp;
};
