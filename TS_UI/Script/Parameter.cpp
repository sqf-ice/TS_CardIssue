
#include "Parameter.h"
#include <algorithm>
#include "tool.h"
#include <iomanip>
#include <vector>

using namespace std;

CParameter::CParameter( CInformation *cInf ):m_cInfo(cInf)
{
}

CParameter::~CParameter(void)
{
}

/************************************************************************/
/*名    称:	EFDATASCmpP1
/*功    能:	用于sort函数比较EFDATAS中的P1按升序排序数据
/*输入参数:	struEFDatas1、struEFDatas2: 结构体EFDATAS引用	
/*输出参数:	null
/*返 回 值:	true	成功	false	失败
/************************************************************************/
bool EFDATASCmpP1( const EFDATAS &struEFDatas1, const EFDATAS &struEFDatas2 )
{
	return HexToDec( struEFDatas1.m_strP1 ) < HexToDec( struEFDatas2.m_strP1 );
}

/************************************************************************/
/*名    称:	EFEXTRADATASCmpP1
/*功    能:	用于sort函数比较EFEXTRADATAS中的P1按升序排序数据
/*输入参数:	struEFExDatas1、struEFExDatas2: 结构体EFEXTRADATAS引用	
/*输出参数:	null
/*返 回 值:	true	成功	false	失败
/************************************************************************/
bool EFEXTRADATASCmpP1( const EFEXTRADATAS &struEFExDatas1, const EFEXTRADATAS &struEFExDatas2 )
{
	return HexToDec( struEFExDatas1.m_strP1 ) < HexToDec( struEFExDatas2.m_strP1 );
}

/************************************************************************/
/*名    称:	CONDECLARCmpCON
/*功    能:	用于sort函数比较CONDECLAR中的CON按升序排序数据
/*输入参数:	struCONDeclar1、struCONDeclar2: 结构体CONDECLAR引用	
/*输出参数:	null
/*返 回 值:	true	成功	false	失败
/************************************************************************/
bool CONDECLARCmpCON( const CONDECLAR &struCONDeclar1, const CONDECLAR &struCONDeclar2 )
{
	return struCONDeclar1.m_strCON < struCONDeclar2.m_strCON;
}

/************************************************************************/
/*名    称:	PROJMODCmpVer
/*功    能:	用于sort函数比较PROJMOD中的Ver按升序排序数据
/*输入参数:	struCONDeclar1、struCONDeclar2: 结构体CONDECLAR引用	
/*输出参数:	null
/*返 回 值:	true	成功	false	失败
/************************************************************************/
bool PROJMODCmpVer( const PROJMOD &struProjMod1, const PROJMOD &struProjMod2 )
{
	return struProjMod1.m_strVer < struProjMod2.m_strVer; 
}

/************************************************************************/
/*名    称:	strHEX
/*功    能:	用于sort函数比较FID按升序排序数据
/*输入参数:	str1、str2: 比较的数据结构
/*输出参数:	null
/*返 回 值:	true	成功	false	失败
/************************************************************************/
bool strHEX( const pair<string,string> &str1, const pair<string,string> &str2 )
{
	return HexToDec(str1.second) < HexToDec(str2.second);
}

void CParameter::GetCardStruct( vector<S_DDFINFO> &vecMFStruct )
{
	int nTotalLen,nTotalCount;
	S_DDFINFO s_DDFInfo;
	S_ADFINFO s_ADFInfo;
	S_EFINFO s_EFInfo;
	vecMFStruct.clear();

	//遍历从xml读取出来<MF>标签内的数据
	vector<DDFINFO>::const_iterator iterDDF = S_CARDNFO.m_vecDDFInfo.begin();
	while ( iterDDF!=S_CARDNFO.m_vecDDFInfo.end() )//遍历DDF
	{
		//获取DDF信息
		s_DDFInfo.m_strAID = (*iterDDF).m_strAID;
		s_DDFInfo.m_strCount = (*iterDDF).m_strCount;
		s_DDFInfo.m_strDDFName = (*iterDDF).m_strDDFName;
		s_DDFInfo.m_strFID = (*iterDDF).m_strFID;
		s_DDFInfo.m_strMKvalue = (*iterDDF).m_strMKvalue;
		s_DDFInfo.m_strSpace = (*iterDDF).m_strSpace;
		s_DDFInfo.m_strUpdateMode1 = (*iterDDF).m_strUpdateMode1;
		s_DDFInfo.m_strUpdateMode2 = (*iterDDF).m_strUpdateMode2;
		s_DDFInfo.m_vecADFInfo.clear();

		vector<ADFINFO>::const_iterator iterADF = (*iterDDF).m_vecADFInfo.begin();
		while ( iterADF!=(*iterDDF).m_vecADFInfo.end() )//遍历ADF
		{
			//获取ADF信息
			s_ADFInfo.m_strADFName = (*iterADF).m_strADFName;
			s_ADFInfo.m_strAID = (*iterADF).m_strAID;
			s_ADFInfo.m_strCount = (*iterADF).m_strCount;
			s_ADFInfo.m_strFID = (*iterADF).m_strFID;
			s_ADFInfo.m_strMKvalue = (*iterADF).m_strMKvalue;
			s_ADFInfo.m_strUpdateMode1 = (*iterADF).m_strUpdateMode1;
			s_ADFInfo.m_strUpdateMode2 = (*iterADF).m_strUpdateMode2;
			s_ADFInfo.m_vecEFInfo.clear();

			vector<EFINFO>::const_iterator iterEFInfo = (*iterADF).m_vecEFInfo.begin();
			while ( iterEFInfo!=(*iterADF).m_vecEFInfo.end() )//遍历EFInfor
			{
				//获取EF信息
				s_EFInfo.m_strbInit = (*iterEFInfo).m_strbInit;
				s_EFInfo.m_strEFType = (*iterEFInfo).m_strEFType;
				s_EFInfo.m_strnCount = (*iterEFInfo).m_strnCount;
				s_EFInfo.m_strReadControl = (*iterEFInfo).m_strReadControl;
				s_EFInfo.m_strResSpace = (*iterEFInfo).m_strResSpace;
				s_EFInfo.m_strSFI = (*iterEFInfo).m_strSFI;
				s_EFInfo.m_strWriteControl = (*iterEFInfo).m_strWriteControl;

				nTotalLen = 0;
				nTotalCount = 0;
				//遍历<EF>对应SFI的标签<SFI>数据
				vector<EFDATAS>::const_iterator iterEFDatas = (*iterEFInfo).m_vecEFDatas.begin();
				while ( iterEFDatas != (*iterEFInfo).m_vecEFDatas.end() )
				{
					nTotalLen += HexToDec((*iterEFDatas).m_strLen);
					++nTotalCount;
					if ( (*iterEFInfo).m_strEFType == "3" )
						break;//透明文件只取一条p1=1的记录为有效数据
					++iterEFDatas;
				}
				s_EFInfo.m_strEFLen = DecToHex(nTotalLen);
				s_EFInfo.m_nRecordCount = nTotalCount;

				s_ADFInfo.m_vecEFInfo.push_back(s_EFInfo);
				++iterEFInfo;
			}

			s_DDFInfo.m_vecADFInfo.push_back(s_ADFInfo);
			++iterADF;
		}

		vecMFStruct.push_back(s_DDFInfo);
		++iterDDF;
	}

}

bool CParameter::GetEFInfo( string strDDF, string strADF, string strEF, S_EFINFO &struEFInfo )
{
	int nTotalLen,nTotalCount;
	string strDDFFID;
	transform(  strDDF.begin(), strDDF.end(), strDDF.begin(), toupper );
	transform(  strADF.begin(), strADF.end(), strADF.begin(), toupper );
	transform(  strEF.begin(), strEF.end(), strEF.begin(), toupper );

	//遍历从xml读取出来<MF>标签内的数据
	vector<DDFINFO>::const_iterator iterDDF = S_CARDNFO.m_vecDDFInfo.begin();
	while ( iterDDF!=S_CARDNFO.m_vecDDFInfo.end() )//遍历DDF
	{
		m_strTemp = (*iterDDF).m_strDDFName;
		strDDFFID = (*iterDDF).m_strFID;
		transform(  m_strTemp.begin(), m_strTemp.end(), m_strTemp.begin(), toupper );
		transform(  strDDFFID.begin(), strDDFFID.end(), strDDFFID.begin(), toupper );
		if ( !(m_strTemp == strDDF || strDDFFID == strDDF) )//不为DDF文件标识符或不为应用环境简称时继续遍历(读取的数据中DDF0的FID为"")
		{
			++iterDDF;
			continue;
		}

		vector<ADFINFO>::const_iterator iterADF = (*iterDDF).m_vecADFInfo.begin();
		while ( iterADF!=(*iterDDF).m_vecADFInfo.end() )//遍历ADF
		{
			m_strTemp = (*iterADF).m_strFID;
			transform(  m_strTemp.begin(), m_strTemp.end(), m_strTemp.begin(), toupper );
			if ( m_strTemp != strADF )//不为ADF文件标识符的话继续遍历(读取的数据中DF00的FID为"")
			{
				++iterADF;
				continue;
			}

			vector<EFINFO>::const_iterator iterEFInfo = (*iterADF).m_vecEFInfo.begin();
			while ( iterEFInfo!=(*iterADF).m_vecEFInfo.end() )//遍历EFInfor
			{
				m_strTemp = (*iterEFInfo).m_strSFI;
				transform(  m_strTemp.begin(), m_strTemp.end(), m_strTemp.begin(), toupper );
				if ( m_strTemp != strEF )
				{
					++iterEFInfo;
					continue;
				}
				else
				{
					//获取EF信息
					struEFInfo.m_strbInit = (*iterEFInfo).m_strbInit;
					struEFInfo.m_strEFType = (*iterEFInfo).m_strEFType;
					struEFInfo.m_strnCount = (*iterEFInfo).m_strnCount;
					struEFInfo.m_strReadControl = (*iterEFInfo).m_strReadControl;
					struEFInfo.m_strResSpace = (*iterEFInfo).m_strResSpace;
					struEFInfo.m_strSFI = (*iterEFInfo).m_strSFI;
					struEFInfo.m_strWriteControl = (*iterEFInfo).m_strWriteControl;
					
					nTotalLen = 0;
					nTotalCount = 0;
					//遍历<EF>对应SFI的标签<SFI>数据
					vector<EFDATAS>::const_iterator iterEFDatas = (*iterEFInfo).m_vecEFDatas.begin();
					while ( iterEFDatas != (*iterEFInfo).m_vecEFDatas.end() )
					{
						nTotalLen += HexToDec((*iterEFDatas).m_strLen);
						++nTotalCount;
						if ( (*iterEFInfo).m_strEFType == "3" )
							break;//透明文件只取第一条记录为有效数据
						++iterEFDatas;
					}
					struEFInfo.m_strEFLen = DecToHex(nTotalLen);
					struEFInfo.m_nRecordCount = nTotalCount;

					return true;
				}

			}

			break;
		}

		break;
	}

	return false;
}

bool CParameter::GetADFInfo( string strDDF, string strADF, S_ADFINFO &struADFInfo )
{
	string strDDFFID;
	transform(  strDDF.begin(), strDDF.end(), strDDF.begin(), toupper );
	transform(  strADF.begin(), strADF.end(), strADF.begin(), toupper );

	if ( strADF == "" )//为空表示的是该ADF的FID为DF00,即表示该层ADF不存在,所以获取DF00的信息是没有意义的
		return false;

	//遍历从xml读取出来<MF>标签内的数据
	vector<DDFINFO>::const_iterator iterDDF = S_CARDNFO.m_vecDDFInfo.begin();
	while ( iterDDF!=S_CARDNFO.m_vecDDFInfo.end() )//遍历DDF
	{
		m_strTemp = (*iterDDF).m_strDDFName;
		strDDFFID = (*iterDDF).m_strFID;
		transform(  m_strTemp.begin(), m_strTemp.end(), m_strTemp.begin(), toupper );
		transform(  strDDFFID.begin(), strDDFFID.end(), strDDFFID.begin(), toupper );
		if ( !(m_strTemp == strDDF || strDDFFID == strDDF) )//不为DDF文件标识符或不为应用环境简称时继续遍历(读取的数据中DDF0的FID为"")
		{
			++iterDDF;
			continue;
		}

		vector<ADFINFO>::const_iterator iterADF = (*iterDDF).m_vecADFInfo.begin();
		while ( iterADF!=(*iterDDF).m_vecADFInfo.end() )//遍历ADF
		{
			m_strTemp = (*iterADF).m_strFID;
			transform(  m_strTemp.begin(), m_strTemp.end(), m_strTemp.begin(), toupper );
			if ( m_strTemp != strADF )//不为ADF文件标识符时继续遍历
			{
				++iterADF;
				continue;
			}
			else
			{
				//获取ADF信息
				struADFInfo.m_strADFName = (*iterADF).m_strADFName;
				struADFInfo.m_strAID = (*iterADF).m_strAID;
				struADFInfo.m_strCount = (*iterADF).m_strCount;
				struADFInfo.m_strFID = (*iterADF).m_strFID;
				struADFInfo.m_strMKvalue = (*iterADF).m_strMKvalue;
				struADFInfo.m_strUpdateMode1 = (*iterADF).m_strUpdateMode1;
				struADFInfo.m_strUpdateMode2 = (*iterADF).m_strUpdateMode2;
				struADFInfo.m_vecEFInfo.clear();
				return true;
			}
			
			break;
		}

		break;
	}

	return false;
}

bool CParameter::GetDDFInfo( string strDDF, S_DDFINFO &struDDFInfo )
{
	string strDDFFID;
	transform(  strDDF.begin(), strDDF.end(), strDDF.begin(), toupper );

	if ( strDDF == "" )//为空表示的是该DDF的FID为DDF0,即表示该层DDF不存在,所以获取DDF0的信息是没有意义的
		return false;

	//遍历从xml读取出来<MF>标签内的数据
	vector<DDFINFO>::const_iterator iterDDF = S_CARDNFO.m_vecDDFInfo.begin();
	while ( iterDDF!=S_CARDNFO.m_vecDDFInfo.end() )//遍历DDF
	{
		m_strTemp = (*iterDDF).m_strDDFName;
		strDDFFID = (*iterDDF).m_strFID;
		transform(  m_strTemp.begin(), m_strTemp.end(), m_strTemp.begin(), toupper );
		transform(  strDDFFID.begin(), strDDFFID.end(), strDDFFID.begin(), toupper );
		if ( m_strTemp == strDDF || strDDFFID == strDDF )
		{
			struDDFInfo.m_strAID = (*iterDDF).m_strAID;
			struDDFInfo.m_strCount = (*iterDDF).m_strCount;
			struDDFInfo.m_strDDFName = (*iterDDF).m_strDDFName;
			struDDFInfo.m_strFID = (*iterDDF).m_strFID;
			struDDFInfo.m_strMKvalue = (*iterDDF).m_strMKvalue;
			struDDFInfo.m_strSpace = (*iterDDF).m_strSpace;
			struDDFInfo.m_strUpdateMode1 = (*iterDDF).m_strUpdateMode1;
			struDDFInfo.m_strUpdateMode2 = (*iterDDF).m_strUpdateMode2;
			struDDFInfo.m_vecADFInfo.clear();
			return true;
		}

		++iterDDF;
	}

	return false;
}

void CParameter::GetMFInfo( S_MFINFO &struMFInfo )
{
	struMFInfo.m_strAID = S_CARDNFO.m_strAID;
	struMFInfo.m_strCount = S_CARDNFO.m_strCount;
	struMFInfo.m_strFID = S_CARDNFO.m_strFID;
	struMFInfo.m_strMFName = S_CARDNFO.m_strMFName;
	struMFInfo.m_strMKvalue = S_CARDNFO.m_strMKvalue;
}

bool CParameter::GetKeyInfo( string strDDF, string strADF, string strKeyName, KEYDATAS &struKeyDatas )
{
	transform(  strDDF.begin(), strDDF.end(), strDDF.begin(), toupper );
	transform(  strADF.begin(), strADF.end(), strADF.begin(), toupper );
	transform(  strKeyName.begin(), strKeyName.end(), strKeyName.begin(), toupper );
	if ( strDDF == "" )
		strDDF = "DDF0";
	if ( strADF == "" )
		strADF = "DF00";

	//遍历KEYS的数据
	vector<KEYS>::const_iterator iterKeys = V_EKEYS.begin();
	while ( iterKeys!=V_EKEYS.end() )
	{
		m_strTemp = (*iterKeys).m_strDDFName;
		transform(  m_strTemp.begin(), m_strTemp.end(), m_strTemp.begin(), toupper );
		if ( strDDF!=m_strTemp )//没找到对应的DDF,继续遍历
		{
			++iterKeys;
			continue;
		}

		vector<ADFKEY>::const_iterator iterKey = (*iterKeys).m_vecADFKEY.begin();
		while ( iterKey!=(*iterKeys).m_vecADFKEY.end() )
		{
			m_strTemp = (*iterKey).m_strADFName;
			transform(  m_strTemp.begin(), m_strTemp.end(), m_strTemp.begin(), toupper );
			if ( strADF!=m_strTemp )//没找到对应的ADF,继续遍历
			{
				++iterKey;
				continue;
			}

			vector<KEYDATAS>::const_iterator iterKeyDatas = (*iterKey).m_vecKeyDatas.begin();
			while ( iterKeyDatas!=(*iterKey).m_vecKeyDatas.end() )
			{
				m_strTemp = (*iterKeyDatas).m_strName;
				transform(  m_strTemp.begin(), m_strTemp.end(), m_strTemp.begin(), toupper );
				if ( m_strTemp == strKeyName )//找到指定密钥并获取密钥信息
				{
					struKeyDatas = (*iterKeyDatas);
					return true;
				}

				++iterKeyDatas;
			}
			break;
		}
		break;
	}

	return false;
}

bool CParameter::GetADFKeys( string strDDF, string strADF, vector<KEYDATAS> &vecKeyDatas )
{
	transform(  strDDF.begin(), strDDF.end(), strDDF.begin(), toupper );
	transform(  strADF.begin(), strADF.end(), strADF.begin(), toupper );
	if ( strDDF == "" )
		strDDF = "DDF0";
	if ( strADF == "" )
		strADF = "DF00";
	vecKeyDatas.clear();

	//遍历KEYS的数据
	vector<KEYS>::const_iterator iterKeys = V_EKEYS.begin();
	while ( iterKeys!=V_EKEYS.end() )
	{
		m_strTemp = (*iterKeys).m_strDDFName;
		transform(  m_strTemp.begin(), m_strTemp.end(), m_strTemp.begin(), toupper );
		if ( strDDF!=m_strTemp )//没找到对应的DDF,继续遍历
		{
			++iterKeys;
			continue;
		}

		vector<ADFKEY>::const_iterator iterKey = (*iterKeys).m_vecADFKEY.begin();
		while ( iterKey!=(*iterKeys).m_vecADFKEY.end() )
		{
			m_strTemp = (*iterKey).m_strADFName;
			transform(  m_strTemp.begin(), m_strTemp.end(), m_strTemp.begin(), toupper );
			if ( m_strTemp == strADF )
			{
				vecKeyDatas = (*iterKey).m_vecKeyDatas;
				return true;
			}

			++iterKey;
		}
		break;
	}

	return false;
}

bool CParameter::GetEFDatas( string strDDF, string strADF, string strEF, vector<EFDATAS> &vecEFDatas )
{
	bool bFlag = false;
	string strDDFFID,strTmp,strVal;
	transform(  strDDF.begin(), strDDF.end(), strDDF.begin(), toupper );
	transform(  strADF.begin(), strADF.end(), strADF.begin(), toupper );
	transform(  strEF.begin(), strEF.end(), strEF.begin(), toupper );
	vecEFDatas.clear();

	//遍历从xml读取出来<MF>标签内的数据
	vector<DDFINFO>::const_iterator iterDDF = S_CARDNFO.m_vecDDFInfo.begin();
	while ( iterDDF!=S_CARDNFO.m_vecDDFInfo.end() )//遍历DDF
	{
		m_strTemp = (*iterDDF).m_strDDFName;
		strDDFFID = (*iterDDF).m_strFID;
		transform(  m_strTemp.begin(), m_strTemp.end(), m_strTemp.begin(), toupper );
		transform(  strDDFFID.begin(), strDDFFID.end(), strDDFFID.begin(), toupper );
		if ( !(m_strTemp == strDDF || strDDFFID == strDDF) )//不为DDF文件标识符或不为应用环境简称时继续遍历(读取的数据中DDF0的FID为"")
		{
			++iterDDF;
			continue;
		}

		vector<ADFINFO>::const_iterator iterADF = (*iterDDF).m_vecADFInfo.begin();
		while ( iterADF!=(*iterDDF).m_vecADFInfo.end() )//遍历ADF
		{
			m_strTemp = (*iterADF).m_strFID;
			transform(  m_strTemp.begin(), m_strTemp.end(), m_strTemp.begin(), toupper );
			if ( m_strTemp != strADF )//不为ADF文件标识符的话继续遍历(读取的数据中DF00的FID为"")
			{
				++iterADF;
				continue;
			}

			vector<EFINFO>::const_iterator iterEFInfo = (*iterADF).m_vecEFInfo.begin();
			while ( iterEFInfo!=(*iterADF).m_vecEFInfo.end() )//遍历EFInfor
			{
				m_strTemp = (*iterEFInfo).m_strSFI;
				transform(  m_strTemp.begin(), m_strTemp.end(), m_strTemp.begin(), toupper );
				if ( m_strTemp != strEF )//未找到指定EF相匹配的数据则继续遍历EFInfor
				{
					++iterEFInfo;
					continue;
				}
				else//获取指定EF对应的记录数据
				{
					//透明文件只取一条记录
					if ( "3" == (*iterEFInfo).m_strEFType )
					{
						//遍历<EF>对应SFI的标签<SFI>数据
						vector<EFDATAS>::const_iterator iterEFDatas = (*iterEFInfo).m_vecEFDatas.begin();
						while ( iterEFDatas != (*iterEFInfo).m_vecEFDatas.end() )
						{
							if ( 1 == HexToDec( (*iterEFDatas).m_strP1 ) )//如果存在P1=1的记录的话取该记录
							{
								vecEFDatas.push_back((*iterEFDatas));
								bFlag = true;
								break;
							}
							++iterEFDatas;
						}
						if ( (*iterEFInfo).m_vecEFDatas.size()!=0 && iterEFDatas == (*iterEFInfo).m_vecEFDatas.end() )//不存在P1=1的记录的话取第一条记录
						{
							vecEFDatas.push_back((*iterEFInfo).m_vecEFDatas.front());
							bFlag = true;
						}
					}
					else
					{
						//其他类型返回的数据按P1升序排序
						vecEFDatas = (*iterEFInfo).m_vecEFDatas;
						sort( vecEFDatas.begin(), vecEFDatas.end(), EFDATASCmpP1 );
						//结构体EFEXTRADATAS的数据按P1升序排序
						vector<EFDATAS>::iterator iterEFDatas = vecEFDatas.begin();
						while ( iterEFDatas != vecEFDatas.end() )
						{
							///B-TLV数据、循环文件数据按P1升序排序
							if ( "4" == (*iterEFInfo).m_strEFType || "B-TLV" == (*iterEFDatas).m_strrType )
							{
								sort( (*iterEFDatas).m_vecEFExDatas.begin(), (*iterEFDatas).m_vecEFExDatas.end(), EFEXTRADATASCmpP1 );
							}
							++iterEFDatas;
						}
						bFlag = true;
					}
					break;
				}
	
				break;
			}

			break;
		}

		break;
	}

	//将数据长度不足2的倍数的cn类型数据后补F，b类型数据后补0
	vector<EFDATAS>::iterator iterEFDatas = vecEFDatas.begin();
	while ( iterEFDatas != vecEFDatas.end() )//EFDatas
	{
		strTmp = (*iterEFDatas).m_strrType;
		transform(  strTmp.begin(), strTmp.end(), strTmp.begin(), toupper );
		strVal = (*iterEFDatas).m_strVal;
		if ( strVal.compare(0, 3, "CON")==0 )//CON的话不需要后补数据
		{}
		else if ( strTmp == "CN" && (*iterEFDatas).m_strVal.length()%2!=0 )
			(*iterEFDatas).m_strVal =  (*iterEFDatas).m_strVal + "F";
		else if ( strTmp == "B" && (*iterEFDatas).m_strVal.length()%2!=0  )
			(*iterEFDatas).m_strVal =  (*iterEFDatas).m_strVal + "0";
		vector<EFEXTRADATAS>::iterator iterEFExDatas = (*iterEFDatas).m_vecEFExDatas.begin();
		while( iterEFExDatas!=(*iterEFDatas).m_vecEFExDatas.end() )
		{
			strTmp = (*iterEFExDatas).m_strrType;
			transform(  strTmp.begin(), strTmp.end(), strTmp.begin(), toupper );
			strVal = (*iterEFExDatas).m_strVal;
			if ( strVal.compare(0, 3, "CON")==0 )//CON的话不需要后补数据
			{}
			else if ( strTmp == "CN" && (*iterEFExDatas).m_strVal.length()%2!=0 )
				(*iterEFExDatas).m_strVal =  (*iterEFExDatas).m_strVal + "F";
			else if ( strTmp == "B" && (*iterEFExDatas).m_strVal.length()%2!=0  )
				(*iterEFExDatas).m_strVal =  (*iterEFExDatas).m_strVal + "0";
			++iterEFExDatas;
		}
		++iterEFDatas;
	}

	return bFlag;
}

bool CParameter::GetEFData( string strDDF, string strADF, string strEF, int nP1, EFDATAS &struEFDatas )
{
	bool bFlag = false;
	string strDDFFID,strTmp,strVal;
	transform(  strDDF.begin(), strDDF.end(), strDDF.begin(), toupper );
	transform(  strADF.begin(), strADF.end(), strADF.begin(), toupper );
	transform(  strEF.begin(), strEF.end(), strEF.begin(), toupper );

	//遍历从xml读取出来<MF>标签内的数据
	vector<DDFINFO>::const_iterator iterDDF = S_CARDNFO.m_vecDDFInfo.begin();
	while ( iterDDF!=S_CARDNFO.m_vecDDFInfo.end() )//遍历DDF
	{
		m_strTemp = (*iterDDF).m_strDDFName;
		strDDFFID = (*iterDDF).m_strFID;
		transform(  m_strTemp.begin(), m_strTemp.end(), m_strTemp.begin(), toupper );
		transform(  strDDFFID.begin(), strDDFFID.end(), strDDFFID.begin(), toupper );
		if ( !(m_strTemp == strDDF || strDDFFID == strDDF) )//不为DDF文件标识符或不为应用环境简称时继续遍历(读取的数据中DDF0的FID为"")
		{
			++iterDDF;
			continue;
		}

		vector<ADFINFO>::const_iterator iterADF = (*iterDDF).m_vecADFInfo.begin();
		while ( iterADF!=(*iterDDF).m_vecADFInfo.end() )//遍历ADF
		{
			m_strTemp = (*iterADF).m_strFID;
			transform(  m_strTemp.begin(), m_strTemp.end(), m_strTemp.begin(), toupper );
			if ( m_strTemp != strADF )//不为ADF文件标识符的话继续遍历(读取的数据中DF00的FID为"")
			{
				++iterADF;
				continue;
			}

			vector<EFINFO>::const_iterator iterEFInfo = (*iterADF).m_vecEFInfo.begin();
			while ( iterEFInfo!=(*iterADF).m_vecEFInfo.end() )//遍历EFInfor
			{
				m_strTemp = (*iterEFInfo).m_strSFI;
				transform(  m_strTemp.begin(), m_strTemp.end(), m_strTemp.begin(), toupper );
				if ( m_strTemp != strEF )
				{
					++iterEFInfo;
					continue;
				}

				//循环文件若存在记录的话只能有一条记录,取第一条数据
				if ( (*iterEFInfo).m_strEFType == "4" && (*iterEFInfo).m_vecEFDatas.size() != 0 )
				{
					struEFDatas = (*iterEFInfo).m_vecEFDatas.front();
					//循环文件的EFEXTRADATAS数据按P1升序排序
					sort( struEFDatas.m_vecEFExDatas.begin(), struEFDatas.m_vecEFExDatas.end(), EFEXTRADATASCmpP1 );
					bFlag = true;
					break;
				}
				else if ( (*iterEFInfo).m_strEFType == "3" )//透明文件取P1=1的记录或第一条记录
				{	
					//遍历<EF>对应SFI的标签<SFI>数据
					vector<EFDATAS>::const_iterator iterEFDatas = (*iterEFInfo).m_vecEFDatas.begin();
					while ( iterEFDatas != (*iterEFInfo).m_vecEFDatas.end() )
					{
						if ( 1 == HexToDec( (*iterEFDatas).m_strP1 ) )//如果存在P1=1的记录的话取该记录
						{
							struEFDatas = (*iterEFDatas);
							bFlag = true;
							break;
						}
						++iterEFDatas;
					}
					if ( (*iterEFInfo).m_vecEFDatas.size()!=0 && iterEFDatas == (*iterEFInfo).m_vecEFDatas.end() )//不存在P1=1的记录的话取第一条记录
					{
						struEFDatas = (*iterEFInfo).m_vecEFDatas.front();
						bFlag = true;
					}
				}
				else//其他文件
				{
					//遍历<EF>对应SFI的标签<SFI>数据
					vector<EFDATAS>::const_iterator iterEFDatas = (*iterEFInfo).m_vecEFDatas.begin();
					while ( iterEFDatas != (*iterEFInfo).m_vecEFDatas.end() )
					{
						m_strTemp = (*iterEFDatas).m_strP1;
						if ( HexToDec(m_strTemp) != nP1 )
						{
							++iterEFDatas;
							continue;
						}
						else
						{
							struEFDatas = (*iterEFDatas);
							if ( "B-TLV" == (*iterEFDatas).m_strrType )//B-TLV文件的EFEXTRADATAS数据按P1升序排序
								sort( struEFDatas.m_vecEFExDatas.begin(), struEFDatas.m_vecEFExDatas.end(), EFEXTRADATASCmpP1 );
							bFlag = true;
							break;
						}

						++iterEFDatas;
					}
				}

				break;
			}

			break;
		}

		break;
	}

	//将数据长度不足2的倍数的cn类型数据后补F，b类型数据后补0
	strTmp = struEFDatas.m_strrType;
	transform(  strTmp.begin(), strTmp.end(), strTmp.begin(), toupper );
	strVal = struEFDatas.m_strVal;
	if ( strVal.compare(0, 3, "CON")==0 )//CON的话不需要后补数据
	{}
	else if ( strTmp == "CN" && struEFDatas.m_strVal.length()%2!=0 )
		struEFDatas.m_strVal =  struEFDatas.m_strVal + "F";
	else if ( strTmp == "B" && struEFDatas.m_strVal.length()%2!=0  )
		struEFDatas.m_strVal =  struEFDatas.m_strVal + "0";
	vector<EFEXTRADATAS>::iterator iterEFExDatas = struEFDatas.m_vecEFExDatas.begin();
	while( iterEFExDatas!=struEFDatas.m_vecEFExDatas.end() )
	{
		strTmp = (*iterEFExDatas).m_strrType;
		transform(  strTmp.begin(), strTmp.end(), strTmp.begin(), toupper );
		strVal = (*iterEFExDatas).m_strVal;
		if ( strVal.compare(0, 3, "CON")==0 )//CON的话不需要后补数据
		{}
		else if ( strTmp == "CN" && (*iterEFExDatas).m_strVal.length()%2!=0 )
			(*iterEFExDatas).m_strVal =  (*iterEFExDatas).m_strVal + "F";
		else if ( strTmp == "B" && (*iterEFExDatas).m_strVal.length()%2!=0  )
			(*iterEFExDatas).m_strVal =  (*iterEFExDatas).m_strVal + "0";
		++iterEFExDatas;
	}

	return bFlag;
}

void CParameter::GetProjInfo( PROJINFO &struProjInfo )
{
	struProjInfo = S_SETTING.m_struProjInfo;
	//将m_vecProjMod按m_strVer升序排序
	sort( struProjInfo.m_vecProjMod.begin(), struProjInfo.m_vecProjMod.end(), PROJMODCmpVer );
}

void CParameter::GetCityCode( CITYCODE &struCityCode )
{
	struCityCode = S_SETTING.m_struCityCode;
}

void CParameter::GetLoadKeyInfo( LOADKEYINFO &struLoadKeyInfo )
{
	struLoadKeyInfo = S_SETTING.m_struLoadKeyInfo;
}

void CParameter::GetCONStatement( vector<CONDECLAR> &vecCONDeclar )
{
	string strDir="",strTemp;
	vecCONDeclar.clear();
	CONDECLAR struCON;

	//遍历从xml读取出来<MF>标签内的数据
	vector<DDFINFO>::const_iterator iterDDF = S_CARDNFO.m_vecDDFInfo.begin();
	while ( iterDDF!=S_CARDNFO.m_vecDDFInfo.end() )//遍历DDF
	{
		vector<ADFINFO>::const_iterator iterADF = (*iterDDF).m_vecADFInfo.begin();
		while ( iterADF!=(*iterDDF).m_vecADFInfo.end() )//遍历ADF
		{
			strTemp = (*iterADF).m_strADFName;
			transform( strTemp.begin(), strTemp.end(), strTemp.begin(), toupper );
			if ( strTemp != "DF00" )
				strDir = (*iterDDF).m_strDDFName+" "+strTemp+" ";
			else
				strDir = (*iterDDF).m_strDDFName+" ";
			vector<EFINFO>::const_iterator iterEFInfo = (*iterADF).m_vecEFInfo.begin();
			while ( iterEFInfo!=(*iterADF).m_vecEFInfo.end() )//遍历EFInfor
			{
				//遍历<EF>对应SFI的标签<SFI>数据
				vector<EFDATAS>::const_iterator iterEFDatas = (*iterEFInfo).m_vecEFDatas.begin();
				while ( iterEFDatas != (*iterEFInfo).m_vecEFDatas.end() )
				{
					if ( (*iterEFDatas).m_strVal.compare(0, 3, "CON")==0 )
					{
						struCON.m_strCON = (*iterEFDatas).m_strVal;
						struCON.m_strLen = (*iterEFDatas).m_strLen;
						if ( StrDelBlank((*iterEFDatas).m_strRemark) != "" )
							struCON.m_strName = (*iterEFDatas).m_strRemark;
						else
							struCON.m_strName = "null";
						struCON.m_strParentDir = strDir+(*iterEFInfo).m_strSFI;
						struCON.m_strType = (*iterEFDatas).m_strrType;
						struCON.m_strUserFields = "null";
						vecCONDeclar.push_back(struCON);
					}

					++iterEFDatas;
				}
				
				++iterEFInfo;
			}

			++iterADF;
		}

		++iterDDF;
	}

	//将vecCONDeclar按struCON升序排序
	sort( vecCONDeclar.begin(), vecCONDeclar.end(), CONDECLARCmpCON );
}

bool CParameter::CheckEF_Exist( string &strFID )
{
	bool bFlag = true;//标志位，当FID发生变化的时候值变为false
	transform(  strFID.begin(), strFID.end(), strFID.begin(), toupper );

	//遍历从xml读取出来<MF>标签内的数据
	vector<DDFINFO>::const_iterator iterDDF = S_CARDNFO.m_vecDDFInfo.begin();
	while ( iterDDF!=S_CARDNFO.m_vecDDFInfo.end() )//遍历DDF
	{

		vector<ADFINFO>::const_iterator iterADF = (*iterDDF).m_vecADFInfo.begin();
		while ( iterADF!=(*iterDDF).m_vecADFInfo.end() )//遍历ADF
		{

			vector<EFINFO>::const_iterator iterEFInfo = (*iterADF).m_vecEFInfo.begin();
			while ( iterEFInfo!=(*iterADF).m_vecEFInfo.end() )//遍历EFInfor
			{
				m_strTemp = (*iterEFInfo).m_strSFI;
				transform(  m_strTemp.begin(), m_strTemp.end(), m_strTemp.begin(), toupper );
				//遇到重复的FID，FID递增
				if ( m_strTemp == strFID )
				{
					strFID = DecToHex( (HexToDec(m_strTemp)+1) );
					bFlag = false;
				}
				
				++iterEFInfo;
			}

			++iterADF;
		}

		++iterDDF;
	}

	return bFlag;
}

void CParameter::Internal_SFI(string &strSF_SFI, string &strFCI_SFI, string &strDIR_SFI, string &strTF_SFI, string &strATR_SFI)
{
	string strTmp,strTmp1;
	string strDIR_FID,strFCI_FID,strSF_FID,strTF_FID,strATR_FID;
	stringstream strsTmp;

	vector< pair<string, string> > vecFID;

	if ( strDIR_SFI == "" || strDIR_SFI.length() > 2 || !JudgeHex(strDIR_SFI) )//为空或长度大于2或不是十六进制数的时候，strDIR_SFI赋值一个默认值
		strDIR_SFI = "";
	else
	{
		strsTmp.str("");
		strsTmp << setfill('0') << setw(2) << right << strDIR_SFI;
		strDIR_FID = "EF"+strsTmp.str();
		vecFID.push_back( make_pair( "DIR", strDIR_FID ) );
	}
	if ( strFCI_SFI == "" || strFCI_SFI.length() > 2 || !JudgeHex(strFCI_SFI) )//为空或长度大于2或不是十六进制数的时候，strFCI_SFI赋值一个默认值
		strFCI_SFI = "";
	else
	{
		strsTmp.str("");
		strsTmp << setfill('0') << setw(2) << right << strFCI_SFI;
		strFCI_FID = "EF"+strsTmp.str();
		vecFID.push_back( make_pair( "FCI", strFCI_FID ) );
	}
	if ( strSF_SFI == "" || strSF_SFI.length() > 2 || !JudgeHex(strSF_SFI) )//为空或长度大于2或不是十六进制数的时候，strSF_SFI赋值一个默认值
		strSF_SFI = "";
	else
	{
		strsTmp.str("");
		strsTmp << setfill('0') << setw(2) << right << strSF_SFI;
		strSF_FID = "EF"+strsTmp.str();
		vecFID.push_back( make_pair( "SF", strSF_FID ) );
	}
	if ( strTF_SFI == "" || strTF_SFI.length() > 2 || !JudgeHex(strTF_SFI) )//为空或长度大于2或不是十六进制数的时候，strTF_SFI赋值一个默认值
		strTF_SFI = "";
	else
	{
		strsTmp.str("");
		strsTmp << setfill('0') << setw(2) << right << strTF_SFI;
		strTF_FID = "EF"+strsTmp.str();
		vecFID.push_back( make_pair( "TF", strTF_FID ) );
	}
	if ( strATR_SFI == "" || strATR_SFI.length() > 2 || !JudgeHex(strATR_SFI) )//为空或长度大于2或不是十六进制数的时候，strATR_SFI赋值一个默认值
		strATR_SFI = "";
	else
	{
		strsTmp.str("");
		strsTmp << setfill('0') << setw(2) << right << strATR_SFI;
		strATR_FID = "EF"+strsTmp.str();
		vecFID.push_back( make_pair( "ATR", strATR_FID ) );
	}

	sort( vecFID.begin(), vecFID.end(), strHEX );//升序排序

	vector< pair<string, string> >::iterator iterFID = vecFID.begin();
	while ( iterFID != vecFID.end() )
	{
		while ( !CheckEF_Exist( (*iterFID).second ) );//循环遍历MF，直到FID都没有和XML的数据重复为止
		//用临时变量保存循环结束后得到的FID，例如：原先的FID为EF05，循环结束后的FID为EF10，则表示EF05-EF10这区间的值在xml文件中都存在
		strTmp = (*iterFID).second;

		if ( iterFID+1 != vecFID.end() )//不是最后一个元素的时候
		{
			//用strTmp1保存后一个FID
			strTmp1 = (*(iterFID+1)).second;
			//因为vecFID之前做过升序排序，所以当前FID的值查找重复发生值改变的时候，若后一个FID的值有出现小于或等于strTmp的情况的话，后一个FID=strTmp+1，然后再继续遍历检测循环
			if ( HexToDec(strTmp1)<= HexToDec(strTmp) )
				(*(iterFID+1)).second = DecToHex( HexToDec( strTmp )+1 );
		}

		//将当前的FID赋值给对应的输出变量
		if ( (*iterFID).first == "DIR" )
			strDIR_FID = (*iterFID).second;
		else if ( (*iterFID).first == "FCI" )
			strFCI_FID = (*iterFID).second;
		else if ( (*iterFID).first == "SF" )
			strSF_FID = (*iterFID).second;
		else if ( (*iterFID).first == "TF" )
			strTF_FID = (*iterFID).second;
		else if ( (*iterFID).first == "ATR" )
			strATR_FID = (*iterFID).second;
			
		++iterFID;
	}
	
	iterFID = vecFID.begin();
	while ( iterFID != vecFID.end() )
	{
		if ( (*iterFID).first == "DIR" )
			strDIR_SFI = (*iterFID).second.substr(2);
		else if ( (*iterFID).first == "FCI" )
			strFCI_SFI = (*iterFID).second.substr(2);
		else if ( (*iterFID).first == "SF" )
			strSF_SFI = (*iterFID).second.substr(2);
		else if ( (*iterFID).first == "TF" )
			strTF_SFI = (*iterFID).second.substr(2);
		else if ( (*iterFID).first == "ATR" )
			strATR_SFI = (*iterFID).second.substr(2);
		++iterFID;
	}

}

bool CParameter::ModeKeySeq( string strDDF, string strADF, string strKeyName, int nSeq )
{
	if ( nSeq<0 || nSeq>15 )
		return false;
	transform(  strDDF.begin(), strDDF.end(), strDDF.begin(), toupper );
	transform(  strADF.begin(), strADF.end(), strADF.begin(), toupper );
	transform(  strKeyName.begin(), strKeyName.end(), strKeyName.begin(), toupper );
	if ( strDDF == "" )
		strDDF = "DDF0";
	if ( strADF == "" )
		strADF = "DF00";

	//遍历KEYS的数据
	vector<KEYS>::iterator iterKeys = V_EKEYS.begin();
	while ( iterKeys!=V_EKEYS.end() )
	{
		m_strTemp = (*iterKeys).m_strDDFName;
		transform(  m_strTemp.begin(), m_strTemp.end(), m_strTemp.begin(), toupper );
		if ( strDDF!=m_strTemp )//没找到对应的DDF,继续遍历
		{
			++iterKeys;
			continue;
		}

		vector<ADFKEY>::iterator iterKey = (*iterKeys).m_vecADFKEY.begin();
		while ( iterKey!=(*iterKeys).m_vecADFKEY.end() )
		{
			m_strTemp = (*iterKey).m_strADFName;
			transform(  m_strTemp.begin(), m_strTemp.end(), m_strTemp.begin(), toupper );
			if ( strADF!=m_strTemp )//没找到对应的ADF,继续遍历
			{
				++iterKey;
				continue;
			}

			vector<KEYDATAS>::iterator iterKeyDatas = (*iterKey).m_vecKeyDatas.begin();
			while ( iterKeyDatas!=(*iterKey).m_vecKeyDatas.end() )
			{
				m_strTemp = (*iterKeyDatas).m_strName;
				transform(  m_strTemp.begin(), m_strTemp.end(), m_strTemp.begin(), toupper );
				if ( m_strTemp == strKeyName )//找到指定密钥并修改密钥的安全级别
				{
					(*iterKeyDatas).m_nSeq = nSeq;
					return true;
				}

				++iterKeyDatas;
			}
			break;
		}
		break;
	}

	return false;
}

bool CParameter::GetKEYS( int nMode, vector<KEYS> &vecKeys )
{
	vecKeys.clear();
	KEYS keys;
	bool bFlag=false;

	if ( nMode == 1 )
	{
		vecKeys = V_EKEYS;
		bFlag = true;
	}
	else if ( nMode == 2 )
	{
		vector<KEYS>::const_iterator iterKeys = V_EKEYS.begin();
		while ( iterKeys!=V_EKEYS.end() )
		{
			if ( (*iterKeys).m_strDDFName == "SSSE" )
			{
				keys = *iterKeys;
				vecKeys.push_back(keys);
				bFlag = true;
				break;
			}
			++iterKeys;
		}
	}
	else if ( nMode == 3 )
	{
		vector<KEYS>::const_iterator iterKeys = V_EKEYS.begin();
		while ( iterKeys!=V_EKEYS.end() )
		{
			if ( (*iterKeys).m_strDDFName != "SSSE" )
			{
				keys = *iterKeys;
				vecKeys.push_back(keys);
				bFlag = true;
			}
			++iterKeys;
		}
	}

	return bFlag;
}