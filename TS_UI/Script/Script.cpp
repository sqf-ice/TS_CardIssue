
#include "Script.h"
#include "Chip.h"
#include "tool.h"
#include <algorithm>
#include <iomanip>

CScript::CScript( int COS, CInformation *cInfo ):cInfo(cInfo)
{	
	//参数类实例化
	cParameter = new CParameter( cInfo );
	//命令类实例化
	cCmd = new CCommand( cInfo );
	//指令类实例化
	switch ( COS )
	{
	case COSID_T514:
		cInst = new COS_T514( cInfo, cParameter );
		break;
	default:
		cInst = NULL;
		cInfo->Append_Error("无法识别的COS标志");
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
	cParameter->GetLoadKeyInfo( m_struLoadKeyInfo );//获取装载密钥相关信息
	cParameter->GetCardStruct( m_vecMFStruct );//获取卡片结构
	cParameter->GetMFInfo( m_struMFInfo );//获取MF信息

	LvOne_Annotations( "设置读写器卡座并上电复位" );
	cCmd->SETNAD( "0", m_bRefreshFlag );//设置读写器卡座
	cCmd->RESET( m_bRefreshFlag );//复位

	LvOne_Annotations( "卡片SN" );
	cCmd->ATR( 9, 8, _CARD_SN, m_bRefreshFlag );//作为导出密钥分散因子

	//华虹、同方则仅支持明文方式装载密钥
	if ( strCOS == "3" || strCOS == "4" )
	{
		if ( m_struLoadKeyInfo.m_strType == "0" )//密文
			m_strLoadKeyMode = "1";//改成明文
		else if ( m_struLoadKeyInfo.m_strType == "10" )//密文更新
			m_strLoadKeyMode = "11";//改成明文更新
	}
	else
		m_strLoadKeyMode = m_struLoadKeyInfo.m_strType;

	if ( "1" == m_struLoadKeyInfo.m_strMode )//加密机方式装载密钥
	{
		LvOne_Annotations( "连接加密机并设置装载密钥的参数" );
		cCmd->CONNECTJMJ( CON_Name("CON2"), CON_Name("CON0"), CON_Name("CON1"), m_bRefreshFlag );//连加密机
		cCmd->VAR( strCOS, _COSMANU, m_bRefreshFlag);//COS厂商 0－华大，1－德生，3－华虹，4－同方，5－大唐 （可扩充）'暂不理 6: 天津卡, 7 华虹
		cCmd->VAR( m_strLoadKeyMode, _STOWAGE, m_bRefreshFlag );//装载方式 0－密文，1－明文（华大、德生、大唐支持密文直接装载而华虹、同方则仅支持明文方式）  10密文更新  11: 明文更新,    '暂不理
		cCmd->VAR( m_struLoadKeyInfo.m_strPKeyNo, _PKEYNO, m_bRefreshFlag );//16进制格式保护密钥ID
		cCmd->VAR( "1", _KEYFSJS, m_bRefreshFlag );//16进制格式导出密钥分散级数
		cCmd->VAR( m_struLoadKeyInfo.m_strPKeyfsjs, _PKEYFSJS, m_bRefreshFlag );//16进制格式保护密钥（次主密钥）级数
		cCmd->ATR( 9, 8, _KEYFSYZ, m_bRefreshFlag );//导出密钥分散因子,值为ATR应答中的最后八字节内容
		cCmd->VAR( m_struLoadKeyInfo.m_strPKeyfsyz, _PKEYFSYZ, m_bRefreshFlag );//保护密钥（次主密钥）分散因子
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
	//获取项目相关信息
	PROJINFO struProjInfo;
	cParameter->GetProjInfo( struProjInfo );
	m_sstrTemp.str("");
	m_sstrTemp<<";项目:"<<struProjInfo.m_strName<<endl;
	m_sstrTemp<<";说明:"<<struProjInfo.m_strDesc<<endl;
	vector<PROJMOD>::const_iterator iterProj =struProjInfo.m_vecProjMod.begin();
	while(iterProj!=struProjInfo.m_vecProjMod.end())
	{
		m_sstrTemp<<";作 者："<<(*iterProj).m_strAuthor<<endl;
		m_sstrTemp<<";修 改："<<(*iterProj).m_strModify<<endl;
		m_sstrTemp<<";日 期："<<(*iterProj).m_strDate<<endl;
		m_sstrTemp<<";版 本："<<(*iterProj).m_strVer<<endl<<endl;
		++iterProj;
	}
	m_sstrTemp<<";1. 数据库对应字段列表"<<endl<<endl;
	cInfo->Append_Script(m_sstrTemp.str());
}

void CScript::CON_Statement()
{
	m_sstrTemp.str("");

	//获取脚本说明的CON常量说明部分内容
	vector<CONDECLAR> vecCONDeclar;
	string strTemp;
	int nLen;
	cParameter->GetCONStatement( vecCONDeclar );
	m_sstrTemp<<";2. 对应关系"<<endl<<endl;
	m_sstrTemp<<";常 量"<<"      用户字段"<<"      中文名称"<<"                                              所属文件"<<"            类型"<<"      长度"<<endl;
	vector<CONDECLAR>::const_iterator iterCON = vecCONDeclar.begin();
	while ( iterCON!=vecCONDeclar.end() )
	{
		strTemp = ";"+(*iterCON).m_strCON;
		nLen = 12-strTemp.length();//用于调整空格间距
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

	m_sstrTemp<<";3. 变量说明"<<endl<<endl;
	m_sstrTemp<<";VAR"<<_KEY<<"   保存密钥"<<endl;
	m_sstrTemp<<";VAR"<<_RECNO<<"   记录号"<<endl;
	m_sstrTemp<<";VAR"<<_CARD_SN<<"   卡片SN"<<endl;
	m_sstrTemp<<";VAR"<<_COSMANU<<"   COS类型"<<endl;
	m_sstrTemp<<";VAR"<<_TRANS_KEY<<"   认证传输密钥"<<endl;
	m_sstrTemp<<";VAR"<<_STOWAGE<<"   LOADKEY装载方式"<<endl;
	m_sstrTemp<<";VAR"<<_PKEYNO<<"   LOADKEY保护密钥ID"<<endl;
	m_sstrTemp<<";VAR"<<_KEYFSJS<<"   LOADKEY导出密钥分散级数"<<endl;
	m_sstrTemp<<";VAR"<<_PKEYFSJS<<"   LOADKEY保护密钥（次主密钥）分散级数"<<endl;
	m_sstrTemp<<";VAR"<<_KEYFSYZ<<"   LOADKEY导出密钥分散因子"<<endl;
	m_sstrTemp<<";VAR"<<_VAR10<<"  认证密钥时保存随机数"<<endl;
	m_sstrTemp<<";VAR"<<_VAR11<<"  认证密钥时保存过程密钥"<<endl;
	m_sstrTemp<<";VAR"<<_VAR12<<"  认证密钥时保存过程密钥"<<endl;
	m_sstrTemp<<";VAR"<<_PKEYFSYZ<<"  LOADKEY保护密钥（次主密钥）分散因子"<<endl;
	m_sstrTemp<<";VAR"<<_ATR<<"  用于存储复位信息"<<endl;
	m_sstrTemp<<";VAR"<<_MAC<<"  用于存储计算的mac值"<<endl;
	m_sstrTemp<<";VAR"<<_CARDID<<"  用于存储个人识别码"<<endl;
	m_sstrTemp<<";VAR"<<_TEMP<<"  用于存储临时变量"<<endl;
	m_sstrTemp<<";VAR30-VAR40预留给MAC和密文等计算的临时变量"<<endl;
	m_sstrTemp<<endl;
	m_sstrTemp<<";CON0:加密机 IP地址:"<<endl;
	m_sstrTemp<<";CON1:加密机 端口:6666"<<endl;
	m_sstrTemp<<";CON2:加密机 类型"<<endl;
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
	cCmd->SETNAD( "0", m_bRefreshFlag );//设置读写器卡座
	cCmd->RESET(m_bRefreshFlag);
	
	//清二级目录
	vector<CLEARCARDPARAM>::const_iterator iter = m_vecCLCParam.begin();
	while ( iter != m_vecCLCParam.end() )
	{
		if ( (*iter).m_strDirLv == "二级目录" )
		{
			strTmp = "清除"+(*iter).m_strFID;
			LvOne_Annotations( strTmp, m_bRefreshFlag );
			cInst->Select_File( "MF", "", "", "", m_bRefreshFlag );
			cInst->Select_File( true, false, (*iter).m_strAID.length()/2, (*iter).m_strAID, m_bRefreshFlag );
			cCmd->VAR( (*iter).m_strMK, _KEY, m_bRefreshFlag );
			cInst->Get_Challenge( 8, m_bRefreshFlag );
			m_sstrTemp.str("");
			m_sstrTemp << "返回值,保存到 VAR" << _VAR10;
			LvOne_Annotations( m_sstrTemp.str(), m_bRefreshFlag );
			cCmd->RESP( 0, 8, _VAR10, m_bRefreshFlag );
			if ( (*iter).m_strAE ==  "社保应用" )
			{
				m_sstrTemp.str("");
				m_sstrTemp << "过程密钥保存到VAR" << _VAR11 << ",VAR" << _VAR12;
				LvOne_Annotations( m_sstrTemp.str(), m_bRefreshFlag );
				cCmd->TDES( VAR_Name(_VAR10), VAR_Name(_KEY), _VAR11, m_bRefreshFlag );
				cCmd->DES( VAR_Name(_VAR10), VAR_Name(_VAR11), _VAR12, m_bRefreshFlag );
				cInst->External_Authenticate( "00", VAR_Name(_VAR12), VAR_Name(_VAR10), 0, m_bRefreshFlag );
			}
			else if ( (*iter).m_strAE ==  "非社保应用" )
			{
				m_sstrTemp.str("");
				m_sstrTemp << "过程密钥保存到VAR" << _VAR11;
				LvOne_Annotations( m_sstrTemp.str(), m_bRefreshFlag );
				cCmd->TDES( VAR_Name(_VAR10), VAR_Name(_KEY), _VAR11, m_bRefreshFlag );
				cInst->External_Authenticate( "00", VAR_Name(_VAR11), "", 0, m_bRefreshFlag );
			}
			cInst->Delete_File( 1, "DDF", (*iter).m_strFID, m_bRefreshFlag );
			LineFeed(1);
		}
		++iter;
	}

	//清一级目录 即MF
	vector<CLEARCARDPARAM>::const_iterator iter1 = m_vecCLCParam.begin();
	while ( iter1 != m_vecCLCParam.end() )
	{
		if ( (*iter1).m_strDirLv == "一级目录" )
		{
			//选择MF，外部认证密钥，清除MF
			LvOne_Annotations( "清除MF", m_bRefreshFlag );
			cCmd->VAR( (*iter1).m_strMK, _KEY, m_bRefreshFlag );
			cInst->Select_File( "MF", "", "", "", m_bRefreshFlag );
			cInst->Get_Challenge( 8, m_bRefreshFlag );
			m_sstrTemp.str("");
			m_sstrTemp << "返回值,保存到 VAR" << _VAR10;
			LvOne_Annotations( m_sstrTemp.str(), m_bRefreshFlag );
			cCmd->RESP( 0, 8, _VAR10, m_bRefreshFlag );
			m_sstrTemp.str("");
			m_sstrTemp << "过程密钥保存到VAR" << _VAR11 << ",VAR" << _VAR12;
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
		m_sstrTemp << "文件初始化 InitFile(" << DDF << ", " << ADF << ", " << EF << ")，找不到指定路径。" << endl;
		cInfo->Append_Error(m_sstrTemp.str());
		return false;
	}

	strDDFTmp = DDF;
	strEFTmp = EF;
	transform( DDF.begin(), DDF.end(), DDF.begin(), toupper );
	transform( EF.begin(), EF.end(), EF.begin(), toupper );
	if ( strDDFTmp == "SSSE" && ADF == "" && strEFTmp == "EF05" )//当在建SSSE下的EF05文件时候需要用到个人识别码
		GetCardID();

	//内部文件
	if (efInfo.m_strEFType == "5")
	{
		m_sstrTemp.clear();
		m_sstrTemp.str("");
		m_sstrTemp << "文件初始化 InitFile(" << DDF << ", " << ADF << ", " << EF << ")，指定文件为内部文件。" << endl;
		cInfo->Append_Error(m_sstrTemp.str());
		return false;
	}
	//循环文件
	if (efInfo.m_strEFType == "4")
	{	}
	//记录文件
	else if (efInfo.m_strEFType == "1" || efInfo.m_strEFType == "2")
	{
		for (int i = 1; i <= efInfo.m_nRecordCount; ++i)
		{
			if (!cParameter->GetEFData(DDF, ADF, EF, i, efData))
			{
				m_sstrTemp.clear();
				m_sstrTemp.str("");
				m_sstrTemp << "文件初始化 InitFile(" << DDF << ", " << ADF << ", " << EF << ")，找不到指定记录" << i << "。" << endl;
				cInfo->Append_Error(m_sstrTemp.str());
				return false;				
			}
			if ( efData.m_strRemark != "" )//备注如果不为空的话，添加注释
			{
				m_strTemp = "写"+efData.m_strRemark;
				LvOne_Annotations(m_strTemp);
			}
			cInst->Update_Record("MF", DDF, ADF, EF, i, true);//初始化数据
			if ( strDDFTmp == "SSSE" && ADF == "" && strEFTmp == "EF05" && efData.m_strTag == "01" )//如果是写SSSE下的EF05的"卡的识别码"这条记录的话需要特殊处理
			{
				strP1 = DecToHex( i );
				m_strTemp = "0110" + VAR_Name(_CARDID);//个人识别码的记录数据
				cInst->Update_Record( strP1, "04", 18, m_strTemp, m_bRefreshFlag);//个人识别码固定数据长度
				continue;
			}
			if (((efData.m_strrType == "an" || efData.m_strrType == "b") && !StrTrim(efData.m_strVal, "0").empty()) || (efData.m_strrType == "cn" && !StrTrim(efData.m_strVal, "F").empty()))
			{	
				cInst->Update_Record("MF", DDF, ADF, EF, i);//写配置文件中的数据
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
				cInst->Update_Record("MF", DDF, ADF, EF, i);//写配置文件中的数据
			}
		}
	}
	//透明文件 
	else if (efInfo.m_strEFType == "3")
	{
		if (!cParameter->GetEFData(DDF, ADF, EF, 1, efData))//该文件没有记录可写
			return true;				
		string str;
		int len = HexToDec(efInfo.m_strEFLen);
		bool flag = StrTrim(efData.m_strVal, "0").empty();//当该值为true时表示使用初始化数据进行填充
		if (!flag && StrDel_ConVar(efData.m_strVal, str))
		{	
			m_sstrTemp.clear();
			m_sstrTemp.str("");
			m_sstrTemp << "文件初始化 InitFile(" << DDF << ", " << ADF << ", " << EF << ")，二进制文件的值不应该是常量。" << "。" << endl;
			cInfo->Append_Warning(m_sstrTemp.str());
			flag = true;
		}
		if (!JudgeHex(StrTrim(efData.m_strVal)))
		{
			m_sstrTemp.clear();
			m_sstrTemp.str("");
			m_sstrTemp << "文件初始化 InitFile(" << DDF << ", " << ADF << ", " << EF << ")，二进制文件的值含有非法字符。" << "。" << endl;
			cInfo->Append_Warning(m_sstrTemp.str());
			flag = true;
		}
		if (StrTrim(efData.m_strVal).length() > (unsigned long)len * 2)
		{
			m_sstrTemp.clear();
			m_sstrTemp.str("");
			m_sstrTemp << "文件初始化 InitFile(" << DDF << ", " << ADF << ", " << EF << ")，二进制文件的值长度过大。" << "。" << endl;
			cInfo->Append_Warning(m_sstrTemp.str());
			flag = true;
		}
		if (flag)
			str = "";
		else
			str = StrTrim(efData.m_strVal);
		
		if ( efData.m_strRemark != "" )//备注如果不为空的话，添加注释
		{
			m_strTemp = "写"+efData.m_strRemark;
			LvOne_Annotations(m_strTemp);
		}

		for (int i = 0; i < len; i += 0x80)
		{
			m_sstrTemp.clear();
			m_sstrTemp.str("");
			if (i + 0x80 <= len)//将长度以0x80长度为单位划分数据写记录
			{
				//透明文件的数据类型为b
				if ((unsigned long)i + 0x80 <= str.length()/2)//str截取数据写0x80长度的记录数据
					cInst->Update_Binary(i, 0x80, str.substr(i * 2, 0x100));
				else if ((unsigned long)i < str.length()/2)//str剩余数据补足0x80长度的记录数据并写数据
				{
					m_sstrTemp << setfill('0') << setw(0x100) << left << str.substr(i * 2, str.length() - i * 2);
					cInst->Update_Binary(i, 0x80, m_sstrTemp.str());
				}
				else//写填充00的记录数据
				{
					m_sstrTemp << setfill('0') << setw(0x100) << left << "";
					cInst->Update_Binary(i, 0x80, m_sstrTemp.str());
				}
			}
			else//将划分最后剩余不足0x80长度的数据写记录
			{
				if ((unsigned long)i < str.length()/2)//str剩余数据写记录
				{
					m_sstrTemp << setfill('0') << setw((len - i) * 2) << left << str.substr(i * 2, str.length() - i * 2);
					cInst->Update_Binary(i, len - i, m_sstrTemp.str() );
				}
				else//写最后填充00的记录数据
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
		m_sstrTemp << "文件初始化 InitFile(" << DDF << ", " << ADF << ", " << EF << ")，无法识别的文件类型（EFType = " << efInfo.m_strEFType << ")。" << endl;
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
	while ( iterSDDF != m_vecMFStruct.end() )//遍历DDF
	{
		if ( "" != (*iterSDDF).m_strFID )//非DDF0
		{
			//添加MF/DDF目录文件记录
			strInst = cInst->DIR_Record( nP1, "MF", (*iterSDDF).m_strFID, "", (*iterSDDF).m_strDDFName, bRefreshFlag );
			strsInsts << strInst << endl;
			nLen += HexToDec( strInst.substr(8,2) );//取指令头表长度的部分
			++nP1;
		}
		else//DDF0的话需要添加MF下的所有ADF的记录信息
		{
			vector<S_ADFINFO>::const_iterator iterSADF = (*iterSDDF).m_vecADFInfo.begin();
			while ( iterSADF!=(*iterSDDF).m_vecADFInfo.end() )//遍历当前DDF下的ADF
			{
				if ( "" != (*iterSADF).m_strFID )//非DF00
				{
					//添加MF/ADF目录文件记录
					strInst = cInst->DIR_Record( nP1, "MF", "", (*iterSADF).m_strFID, (*iterSADF).m_strADFName, bRefreshFlag );
					strsInsts << strInst << endl;
					nLen += HexToDec( strInst.substr(8,2) );//取指令头表长度的部分
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
	//添加MF的FCI记录
	strInst = cInst->FCI_Record( nP1, "MF", "", "", bRefreshFlag );
	strsInsts << strInst << endl;
	nLen += HexToDec( strInst.substr(8,2) );//取指令头表长度的部分
	++nP1;	

	vector<S_DDFINFO>::const_iterator iterSDDF = m_vecMFStruct.begin();
	while ( iterSDDF != m_vecMFStruct.end() )//遍历DDF
	{
		if ( strDDF == "" )//添加MF下所有DDF、ADF的信息记录
		{
			if ( "" != (*iterSDDF).m_strFID )//非DDF0添加DDF的记录信息
			{
				//添加MF/DDF的FCI记录
				strInst = cInst->FCI_Record( nP1, "MF", (*iterSDDF).m_strFID, "", bRefreshFlag );
				strsInsts << strInst << endl;
				nLen += HexToDec( strInst.substr(8,2) );//取指令头表长度的部分
				++nP1;	
			}
			else//DDF0添加MF下直属ADF的记录信息
			{
				vector<S_ADFINFO>::const_iterator iterSADF = (*iterSDDF).m_vecADFInfo.begin();
				while ( iterSADF!=(*iterSDDF).m_vecADFInfo.end() )//遍历当前DDF下的ADF
				{
					if ( "" != (*iterSADF).m_strFID )//非DF00
					{
						//添加MF/ADF的FCI记录
						strInst = cInst->FCI_Record( nP1, "MF", "", (*iterSADF).m_strFID, bRefreshFlag );
						strsInsts << strInst << endl;
						nLen += HexToDec( strInst.substr(8,2) );//取指令头表长度的部分
						++nP1;	
					}

					++iterSADF;
				}	
			}

			++iterSDDF;
		}
		else//添加当前DDF和当前DDF下所有ADF的相关记录信息
		{
			m_strTemp = (*iterSDDF).m_strDDFName;
			strDDFFID = (*iterSDDF).m_strFID;
			transform(  m_strTemp.begin(), m_strTemp.end(), m_strTemp.begin(), toupper );
			transform(  strDDFFID.begin(), strDDFFID.end(), strDDFFID.begin(), toupper );
			if ( !(m_strTemp == strDDF || strDDFFID == strDDF) )//不为DDF文件标识符或不为应用环境简称时继续遍历(读取的数据中DDF0的FID为"")
			{
				++iterSDDF;
				continue;
			}

			//添加MF下 DDF=strDDF 的FCI记录
			strInst = cInst->FCI_Record( nP1, "MF", (*iterSDDF).m_strFID, "", bRefreshFlag );
			strsInsts << strInst << endl;
			nLen += HexToDec( strInst.substr(8,2) );//取指令头表长度的部分
			++nP1;	

			vector<S_ADFINFO>::const_iterator iterSADF = (*iterSDDF).m_vecADFInfo.begin();
			while ( iterSADF!=(*iterSDDF).m_vecADFInfo.end() )//遍历当前DDF下的ADF
			{
				if ( "" != (*iterSADF).m_strFID )//非DF00
				{
					//添加MF下 DDF=strDDF 下所有ADF的FCI记录
					strInst = cInst->FCI_Record( nP1, "MF", (*iterSDDF).m_strFID, (*iterSADF).m_strFID, bRefreshFlag );
					strsInsts << strInst << endl;
					nLen += HexToDec( strInst.substr(8,2) );//取指令头表长度的部分
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
	//指令数据 安全级别 密钥命令头 密钥的密钥头 导出密钥的字节长度 保护密钥的字节长度 密钥地址 配置读取的密钥地址 密钥版本 密钥组数(可确认密钥版本数)
	string strInst,strSafeLv,strInstHead,strInstData,strOpKLen,strPKLen,strKeyAddr,strAddrs,strVer,strGroup;
	string strTmp,strTmp1,strBLOCK,strMK;
	bool bRefreshFlag = false;
	bool bEncrypt = false;//true 密文方式装载或更新密钥 false 明文方式装载或更新密钥
	bool bMac = false;//true MAC方式装载密钥
	vector<KEYDATAS> vecKeyDatas;//存储获取指定DF下的所有密钥信息
	S_DDFINFO struDDFInfo;//存储获取的DDF信息
	S_ADFINFO struADFInfo;//获取ADF信息

	strsInsts.str("");
	if ( strDDF == "" )
		strDDF = "DDF0";
	if ( strADF == "" )
		strADF = "DF00";

	/*先装载主控密钥 版本默认为00*/
	if ( strADF != "DF00" )//存在ADF的时候,获取ADF信息,装载ADF主控密钥
	{
		m_strTemp = "添加" + strADF + "下的主控密钥";
		m_strTemp = LvOne_Annotations(m_strTemp,bRefreshFlag);
		strInst = cInst->Write_Key( "MF", strDDF, strADF, "MK", "00", bRefreshFlag );
		cParameter->GetADFInfo( strDDF, strADF, struADFInfo );
		strMK = struADFInfo.m_strMKvalue;
	}
	else if ( strDDF != "DDF0" )//存在DDF的时候,获取DDF信息,装载DDF主控密钥
	{
		m_strTemp = "添加" + strDDF + "下的主控密钥";
		m_strTemp = LvOne_Annotations(m_strTemp,bRefreshFlag);
		strInst = cInst->Write_Key( "MF", strDDF, "", "MK", "00", bRefreshFlag );
		cParameter->GetDDFInfo( strDDF, struDDFInfo );
		strMK = struDDFInfo.m_strMKvalue;
	}
	else//获取MF信息,装载MF主控密钥
	{
		m_strTemp = LvOne_Annotations("添加MF下的主控密钥",bRefreshFlag);
		strInst = cInst->Write_Key( "MF", "", "", "MK", "00", bRefreshFlag );
		strMK = m_struMFInfo.m_strMKvalue;
	}
	strsInsts << m_strTemp << endl << strInst << endl;//注释加指令
	nLen = HexToDec( strInst.substr(8,2) );//取指令头表长度的部分
	strsInsts << endl;

	/*再装载其他密钥,以下为大唐的装载流程和方式(注意：根据脚本说明文档,加密机装载,华大、德生等可以支持明文和密文方式，而华虹、同方则仅支持明文方式)*/
	if ( cParameter->GetADFKeys( strDDF, strADF, vecKeyDatas ) )
	{
		if ( "0" == m_strLoadKeyMode || "10" == m_strLoadKeyMode )//密文添加或者密文更新
			bEncrypt = true;
		if ( bEncrypt && "2"==m_struLoadKeyInfo.m_strMode && vecKeyDatas.size()>0 )//固定密钥、密文方式 利用当前目录的MK计算密文然后装载密钥
		{
			strsInsts << LvOne_Annotations("利用当前目录文件的MK计算密文然后装载密钥", bRefreshFlag) << endl;
			strsInsts << cCmd->VAR( strMK, _KEY, bRefreshFlag ) << endl;
		}

		vector<KEYDATAS>::const_iterator iterKeyDatas = vecKeyDatas.begin();
		while ( iterKeyDatas!=vecKeyDatas.end() )
		{ 
			strTmp = (*iterKeyDatas).m_strName;
			transform( strTmp.begin(), strTmp.end(), strTmp.begin(), toupper );
			if ( strTmp == "PIN" )//跳过PIN，PIN在装载完其他密钥后再安装
			{
				++iterKeyDatas;
				continue;
			}

			strVer = (*iterKeyDatas).m_strVer;//取密钥的初始版本
			/*装载密钥*/
			if ( "1" == m_struLoadKeyInfo.m_strMode )//加密机装载
			{
				strAddrs = (*iterKeyDatas).m_strAddr;//取加密机的地址
				nPos = strAddrs.find("|");
				while ( nPos != string::npos )//从竖线分隔符的密钥地址中分离出地址
				{
					strKeyAddr = strAddrs.substr( 0, nPos );//获取当前版本的密钥地址
					strSafeLv = DecToHex((*iterKeyDatas).m_nSeq).substr(1);//获取密钥的安全级别
					strSafeLv += strSafeLv;

					m_strTemp = (*iterKeyDatas).m_strName + " 地址: 0x" + strKeyAddr + " 版本:" + strVer + " 安全级别:" + strSafeLv + " 长度（字节）:" + (*iterKeyDatas).m_strLen;
					m_strTemp = LvOne_Annotations(m_strTemp,bRefreshFlag);
					//获取装载密钥的指令头和密钥头
					if ( strADF == "DF00" )
						cInst->Write_Key( "MF", strDDF, "", (*iterKeyDatas).m_strName, strVer, bEncrypt, bMac );
					else
						cInst->Write_Key( "MF", strDDF, strADF, (*iterKeyDatas).m_strName, strVer, bEncrypt, bMac );
					strInstHead = cInst->Get_Instruction_Head();//命令头
					strInstData = cInst->Get_Instruction_Data();//密钥头
					//导出密钥的字节长度：0-8字节，1-10字节
					if ( (*iterKeyDatas).m_strLen == "08" )
					{
						strOpKLen = "0";
						//如果是8字节的话，密文或者密文+mac安装的时候指令头长度需要改变
						if ( bEncrypt && !bMac )//密文
							strInstHead = strInstHead.substr(0,8)+"18";
						else if ( bEncrypt && bMac )//密文+MAC
							strInstHead = strInstHead.substr(0,8)+"1C";
					}
					else if( (*iterKeyDatas).m_strLen == "10" )
						strOpKLen = "1";
					//保护密钥的字节长度：0-8字节，1-10字节
					if ( m_struLoadKeyInfo.m_strPKeyLen == "8" )
						strPKLen = "0";
					else if( m_struLoadKeyInfo.m_strPKeyLen == "10" )
						strPKLen = "1";
					strInst = cCmd->LOADKEY( VAR_Name(_COSMANU), VAR_Name(_STOWAGE), strKeyAddr, VAR_Name(_PKEYNO), VAR_Name(_KEYFSJS), VAR_Name(_PKEYFSJS),
								   VAR_Name(_KEYFSYZ), VAR_Name(_PKEYFSYZ), strInstHead, strInstData, strOpKLen, strPKLen, bRefreshFlag );

					strsInsts << m_strTemp << endl << strInst << endl;//注释加指令
					nLen += strInstData.length()/2 + 16;//累加密钥记录长度（密钥头长度+密钥长度），安全文件是定长文件所以密钥长度都为16字节
					//装载完成之后不同芯片的操作有区别,此处为大唐芯片的处理方式
					if ( !bEncrypt && !bMac )//明文
						strsInsts << cCmd->RESP( 0, 29, -1, bRefreshFlag ) <<endl;
					else if ( bEncrypt && !bMac )//密文
					{
						if ( (*iterKeyDatas).m_strLen == "08" )//STK
							strsInsts << cCmd->RESP( 0, 29, -1, bRefreshFlag ) <<endl;
						else
							strsInsts << cCmd->RESP( 0, 37, -1, bRefreshFlag ) <<endl;
					}
					else if ( bEncrypt && bMac )//密文+MAC
					{
						if ( (*iterKeyDatas).m_strLen == "08" )//STK
							strsInsts << cCmd->RESP( 0, 33, -1, bRefreshFlag ) <<endl;
						else
							strsInsts << cCmd->RESP( 0, 41, -1, bRefreshFlag ) <<endl;
					}

					strVer = DecToHex( HexToDec(strVer)+1 );//密钥版本号+1
					strAddrs = strAddrs.substr(nPos+1);//剩余的密钥地址
					nPos = strAddrs.find("|");
				}

				strsInsts << endl;
			}
			else if ( "2" == m_struLoadKeyInfo.m_strMode )//固定密钥装载
			{
				strGroup = (*iterKeyDatas).m_strGroup;
				nPos = strGroup.find("|");
				while ( nPos != string::npos )//以密钥组数的竖线分隔符确认版本数
				{
					strSafeLv = DecToHex((*iterKeyDatas).m_nSeq).substr(1);//获取密钥的安全级别
					strSafeLv += strSafeLv;

					m_strTemp = (*iterKeyDatas).m_strName + " 版本:" + strVer + " 安全级别:" + strSafeLv + " 长度（字节）:" + (*iterKeyDatas).m_strLen;
					strsInsts << LvOne_Annotations(m_strTemp,bRefreshFlag) << endl;
					//获取装载密钥的指令头和密钥头
					if ( strADF == "DF00" )
						cInst->Write_Key( "MF", strDDF, "", (*iterKeyDatas).m_strName, strVer, bEncrypt, bMac );
					else
						cInst->Write_Key( "MF", strDDF, strADF, (*iterKeyDatas).m_strName, strVer, bEncrypt, bMac );
					strInstHead = cInst->Get_Instruction_Head();//命令头
					strInstData = cInst->Get_Instruction_Data();//密钥头

					/*不同方式装载密钥生成不同指令*/
					if ( !bEncrypt && !bMac )//明文
					{
						strInst = strInstHead + " " + strInstData + " " + (*iterKeyDatas).m_strValue;
						strInst.append( 32-(*iterKeyDatas).m_strValue.length(), 'F' );//所有的密钥都后补F补足16字节数据
						strsInsts << strInst << endl;
					}
					else if ( !bEncrypt && bMac )//明文+MAC
					{

					}
					else if ( bEncrypt && !bMac )//密文 利用当前目录的MK计算密文然后装载密钥
					{
						strTmp = strInstData + (*iterKeyDatas).m_strValue;
						strTmp.append( 32-(*iterKeyDatas).m_strValue.length(), 'F' );//所有的密钥都后补F补足16字节数据
						//加密数据=密钥头和密钥体的总长度 + 密钥头 + 密钥体
						strTmp1 = DecToHex( strTmp.length()/2 ) + strTmp;
						strsInsts << cCmd->VAR( strTmp1, 30, bRefreshFlag ) << endl;//加密数据保存到VAR30
						i = 0;
						nVARSeq = 33;//用于存储每个数据块3DES运算的结果，最后所有的计算结果拼接到一起就是密文
						nBlockLen = strTmp.length()/2 + 1;//得到加密数据长度(字节)
						while ( nBlockLen >= 8 )//nBlockLen以字节为单位
						{
							strsInsts << cCmd->SUBSTR( i*8, 8, VAR_Name(30), 31, bRefreshFlag ) << endl;//获得8字节的分块数据保存到VAR31
							strsInsts << cCmd->TDES( VAR_Name(31), VAR_Name(_KEY), nVARSeq+i, bRefreshFlag ) << endl;//分块数据的3DES计算结果保存到VAR【nVARSeq+i】
							nBlockLen = nBlockLen - 8;
							++i;
						}
						if ( nBlockLen != 0 )//最后如果还存在数据块的话，需要在数据块之后加80，然后用00填充到8字节的数据块，进行3DES运算
						{
							strsInsts << cCmd->SUBSTR( i*8, nBlockLen, VAR_Name(30), 32, bRefreshFlag ) << endl;//最后的分块数据VAR32
							strBLOCK = "80";//补一个字节的80
							strBLOCK.append( (8-nBlockLen-1)*2, '0' );//继续补8-nLen-1个字节的00
							strBLOCK = VAR_Name(32) + strBLOCK;
							strsInsts << cCmd->VAR( strBLOCK, 31, bRefreshFlag ) << endl;//8字节的分块数据保存到VAR31
							strsInsts << cCmd->TDES( VAR_Name(31), VAR_Name(_KEY), nVARSeq+i, bRefreshFlag ) << endl;//分块数据的3DES计算结果保存到VAR【nVARSeq+i】
							++i;
						}
						//指令 = 指令头 + 加密数据
						strInst = strInstHead + " ";
						for ( int j=0; j<i; ++j )
							strInst += VAR_Name(nVARSeq+j);
						strsInsts << strInst << endl;
							
					}
					else if ( bEncrypt && bMac )//密文+MAC
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

					nLen += strInstData.length()/2 + 16;//累加密钥记录长度（密钥头长度+密钥长度），安全文件是定长文件所以密钥长度都为16字节
					strVer = DecToHex( HexToDec(strVer)+1 );//密钥版本号+1
					strGroup=strGroup.substr(nPos+1);
					nPos = strGroup.find("|");
				}

				strsInsts << endl;
			}

			++iterKeyDatas;
		}
	}
	
	//安装 DDF/DF00 下的PIN
	if ( strADF == "DF00" && cParameter->GetADFKeys( strDDF, "", vecKeyDatas ) )
	{
		vector<KEYDATAS>::const_iterator iterKeyDatas = vecKeyDatas.begin();
		while ( iterKeyDatas!=vecKeyDatas.end() )
		{ 
			strTmp = (*iterKeyDatas).m_strName;
			transform( strTmp.begin(), strTmp.end(), strTmp.begin(), toupper );
			if ( strTmp == "PIN" )//安装PIN,版本默认为00
			{
				m_strTemp = LvOne_Annotations("安装PIN",bRefreshFlag);
				strsInsts << m_strTemp << endl;
				cInst->Write_Key( "MF", strDDF, "", "PIN", "00", bEncrypt, bMac );
				strInstHead = cInst->Get_Instruction_Head();//命令头
				strInstData = cInst->Get_Instruction_Data();//密钥头

				/*不同方式装载密钥生成不同指令*/
				if ( !bEncrypt && !bMac )//明文
				{
					strInst = strInstHead + " " + strInstData + (*iterKeyDatas).m_strValue;
					strInst.append( 32-(*iterKeyDatas).m_strValue.length(), 'F' );
					strsInsts << strInst << endl;
				}
				else if ( !bEncrypt && bMac )//明文+MAC
				{

				}
				else if ( bEncrypt && !bMac )//密文
				{
					strsInsts << LvOne_Annotations("利用当前目录的MK计算密文然后安装PIN", bRefreshFlag) << endl;
					strsInsts << cCmd->VAR( strMK, _KEY, bRefreshFlag ) << endl;
					strTmp = strInstData + (*iterKeyDatas).m_strValue;
					strTmp.append( 32-(*iterKeyDatas).m_strValue.length(), 'F' );
					//加密数据=密钥头和密钥体的总长度 + 密钥头 + 密钥体
					strTmp1 = DecToHex( strTmp.length()/2 ) + strTmp;
					strsInsts << cCmd->VAR( strTmp1, 30, bRefreshFlag ) << endl;//加密数据保存到VAR30
					i = 0;
					nVARSeq = 33;//用于存储每个数据块3DES运算的结果，最后所有的计算结果拼接到一起就是密文
					nBlockLen = strTmp.length()/2 + 1;//得到加密数据长度(字节)
					while ( nBlockLen >= 8 )//nBlockLen以字节为单位
					{
						strsInsts << cCmd->SUBSTR( i*8, 8, VAR_Name(30), 31, bRefreshFlag ) << endl;//获得8字节的分块数据保存到VAR31
						strsInsts << cCmd->TDES( VAR_Name(31), VAR_Name(_KEY), nVARSeq+i, bRefreshFlag ) << endl;//分块数据的3DES计算结果保存到VAR【nVARSeq+i】
						nBlockLen = nBlockLen - 8;
						++i;
					}
					if ( nBlockLen != 0 )//最后如果还存在数据块的话，需要在数据块之后加80，然后用00填充到8字节的数据块，进行3DES运算
					{
						strsInsts << cCmd->SUBSTR( i*8, nBlockLen, VAR_Name(30), 32, bRefreshFlag ) << endl;//最后的分块数据VAR32
						strBLOCK = "80";//补一个字节的80
						strBLOCK.append( (8-nBlockLen-1)*2, '0' );//继续补8-nLen-1个字节的00
						strBLOCK = VAR_Name(32) + strBLOCK;
						strsInsts << cCmd->VAR( strBLOCK, 31, bRefreshFlag ) << endl;//8字节的分块数据保存到VAR31
						strsInsts << cCmd->TDES( VAR_Name(31), VAR_Name(_KEY), nVARSeq+i, bRefreshFlag ) << endl;//分块数据的3DES计算结果保存到VAR【nVARSeq+i】
						++i;
					}
					//指令 = 指令头 + 加密数据
					strInst = strInstHead + " ";
					for ( int j=0; j<i; ++j )
						strInst += VAR_Name(nVARSeq+j);
					strsInsts << strInst << endl;
							
				}
				else if ( bEncrypt && bMac )//密文+MAC
				{
				}

				nLen += strInstData.length()/2 + 16;//累加密钥记录长度，PIN密钥必须后补F补足16字节
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

	//获取城市代码
	cParameter->GetCityCode(  struCityCode );
	
	LvOne_Annotations("用IRK密钥计算个人识别码", bRefreshFlag);
	cCmd->SUBSTR( 4, 4, VAR_Name(_CARD_SN), 30, bRefreshFlag );//卡的流水号保存到VAR30
	//应用城市代码+社会保障卡应用标识符+卡的流水号 保存到VAR31
	m_strTemp = struCityCode.m_strValue+"D156000005"+VAR_Name(30);
	cCmd->VAR( m_strTemp, 31, bRefreshFlag );
	//获取SSSE下IRK密钥信息
	cParameter->GetKeyInfo( "SSSE", "", "IRK", struKeyDatas );

	if ( "1" == m_struLoadKeyInfo.m_strMode )//加密机装载
	{
		strAddr = struKeyDatas.m_strAddr;
		nPos = strAddr.find("|");
		strAddr = strAddr.substr( 0, nPos );

		cCmd->GETMAC( "1", "1", VAR_Name(_CARD_SN), "1", strAddr, "0000000000000000", VAR_Name(31), VAR_Name(_PKEYNO), "1", "00000000", 32, bRefreshFlag );//算mac的结果保存到VAR32
		cCmd->SUBSTR( 0, 4, VAR_Name(32), _MAC, bRefreshFlag );
	}
	else if ( "2" == m_struLoadKeyInfo.m_strMode )//固定密钥装载 需要算MAC值
	{
		//取卡随机数后补“00000000”作初始值
		cInst->Get_Challenge( 8, bRefreshFlag );
		LvOne_Annotations( "取随机数前4字节数据后补“00000000”作初始值,保存到 VAR32", bRefreshFlag );
		cCmd->VAR( cCmd->RESP(0,4,-1,false)+"00000000", 32);

		strKey = struKeyDatas.m_strValue;//IRK 的密钥值
		LvOne_Annotations("IRK密钥", bRefreshFlag);
		cCmd->VAR( strKey, _KEY, bRefreshFlag );
		LvOne_Annotations("IRK密钥的KEY_L保存到VAR38", bRefreshFlag);
		cCmd->VAR( strKey.substr(0,16), 38, bRefreshFlag );
		i = 0;
		nLen = struCityCode.m_strValue.length()/2 + 5 + 4;//得到需要计算MAC的数据长度(字节)
		while ( nLen > 8 )//nLen以字节为单位
		{
			cCmd->SUBSTR( i*8, 8, VAR_Name(31), 33, bRefreshFlag );//获得8字节的分块数据保存到VAR33
			cCmd->XOR( VAR_Name(32), VAR_Name(33), 34, bRefreshFlag );//初始值VAR32异或分块数据VAR33得到需要加密的数据保存到VAR34
			cCmd->DES( VAR_Name(34), VAR_Name(38), 32, m_bRefreshFlag );//DES计算的结果作为下一个块运算的初始值
			nLen = nLen - 8;
			++i;
		}
		if ( nLen == 8 )//如果最后的数据块的长度为8字节的话，还需要在数据块之后加一个8字节的80 00 00 00 00 00 00 00进行3DES运算得到mac
		{
			cCmd->SUBSTR( i*8, 8, VAR_Name(31), 33, bRefreshFlag );
			cCmd->XOR( VAR_Name(32), VAR_Name(33), 34, bRefreshFlag );
			cCmd->DES( VAR_Name(34), VAR_Name(38), 32, m_bRefreshFlag );
			
			//得到最后进行运算的数据块
			strBLOCK = "8000000000000000";
			cCmd->VAR( strBLOCK, 33, bRefreshFlag );
			cCmd->XOR( VAR_Name(32), VAR_Name(33), 34, bRefreshFlag );
		}
		else if ( nLen < 8 )//如果最后的数据块的小于8字节的话，还需要在数据块之后加80，然后用00填充到8字节的数据块，进行3DES运算获取mac
		{
			cCmd->SUBSTR( i*8, nLen, VAR_Name(31), 33, bRefreshFlag );//最后的分块数据保存到VAR33
			strBLOCK = "80";//补一个字节的80
			strBLOCK.append( (8-nLen-1)*2, '0' );//继续补8-nLen-1个字节的00
			strBLOCK = VAR_Name(33) + strBLOCK;
			cCmd->VAR( strBLOCK, 36, bRefreshFlag );
			cCmd->XOR( VAR_Name(32), VAR_Name(36), 34, bRefreshFlag );
		}
		//取高四字节作为MAC值
		cCmd->TDES( VAR_Name(34), VAR_Name(_KEY), 35, m_bRefreshFlag );//3DES计算的结果保存到VAR35
		cCmd->SUBSTR( 0, 4, VAR_Name(35), _MAC, bRefreshFlag );

	}

	m_strTemp = VAR_Name(31)+VAR_Name(_MAC);
	cCmd->VAR( m_strTemp, _CARDID, bRefreshFlag );//个人识别码
}

void CScript::CreateMF()
{
	stringstream strsRecords;
	string strLen,strDirFID;

	cCmd->RESET(m_bRefreshFlag);

	//先认证传输密钥
	LvOne_Annotations( "认证传输密钥" );
	cCmd->ATR( 11, 6, _ATR, m_bRefreshFlag );
	m_strTemp = "0000" + VAR_Name(_ATR);
	cCmd->VAR(  m_strTemp, _TEMP, m_bRefreshFlag );
	cCmd->TDES( VAR_Name(_TEMP), m_strTranKey, _TRANS_KEY, m_bRefreshFlag );
	cInst->Verify_TranCode( 8, VAR_Name(_TRANS_KEY), m_bRefreshFlag );
	LineFeed(1);

	//创建MF
	LvOne_Annotations( "创建MF" );
	cInst->Create_File( "MF", "", "", "", m_bRefreshFlag );
	LineFeed(1);

	//创建安全文件
	LvOne_Annotations( "创建安全文件" );
	strLen = LoadKeys( "", "", strsRecords );//获取装载密钥的记录和密钥文件空间
	cInst->Create_Internal_EF( "SF", strLen, m_bRefreshFlag  );
	cInfo->Append_Script( strsRecords.str() );//添加所有的装载密钥脚本数据

	//创建目录文件
	LvOne_Annotations( "创建目录文件" );
	strLen = Dir_Records( strsRecords );//获取目录文件的记录和文件空间
	cInst->Create_Internal_EF( "DIR", strLen, m_bRefreshFlag  );
	strDirFID = cInst->Get_Internal_FID("DIR");
	cInst->Select_File( false, true, 2, strDirFID, m_bRefreshFlag );//选择目录文件
	cInfo->Append_Script( strsRecords.str() );//添加目录文件的记录

	//创建FCI文件
	LvOne_Annotations( "创建FCI文件" );
	strLen = FCI_Records( "", strsRecords );//获取目录文件的记录和文件空间
	cInst->Create_Internal_EF( "FCI", strLen, m_bRefreshFlag  );
	cInfo->Append_Script( strsRecords.str() );//添加目录文件的记录

	//外部认证MF的主控密钥
	LvOne_Annotations( "外部认证MF的主控密钥", m_bRefreshFlag );
	cCmd->VAR( m_struMFInfo.m_strMKvalue, _KEY, m_bRefreshFlag );//将MF的MK保存到_KEY
	cInst->Get_Challenge( 8, m_bRefreshFlag );
	cCmd->RESP( 0, 8, _VAR10, m_bRefreshFlag );
	cCmd->TDES( VAR_Name(_VAR10), VAR_Name(_KEY), _VAR11, m_bRefreshFlag );
	cCmd->DES( VAR_Name(_VAR10), VAR_Name(_VAR11), _VAR12, m_bRefreshFlag );
	cInst->External_Authenticate( "00", VAR_Name(_VAR12), VAR_Name(_VAR10), 0, m_bRefreshFlag );//MK的密钥标识为00
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

	//创建DDF
	m_strTemp = "创建"+strDDF;
	LvTwo_Annotations( m_strTemp );
	cInst->Select_File( "MF", "", "", "", m_bRefreshFlag );
	cInst->Create_File( "MF", strTmp, "", "", m_bRefreshFlag );
	LineFeed(1);

	//创建安全文件
	cInst->Select_File( "MF", strDDF, "", "", m_bRefreshFlag );
	LvOne_Annotations( "创建安全文件" );
	strLen = LoadKeys( strTmp, "", strsRecords );//获取装载密钥的记录和密钥文件空间
	cInst->Create_Internal_EF( "SF", strLen, m_bRefreshFlag  );
	cInfo->Append_Script( strsRecords.str() );//添加所有的装载密钥脚本数据

	//创建FCI文件
	LvOne_Annotations( "创建FCI文件" );
	strLen = FCI_Records( strTmp, strsRecords );//获取目录文件的记录和文件空间
	cInst->Create_Internal_EF( "FCI", strLen, m_bRefreshFlag  );
	cInfo->Append_Script( strsRecords.str() );//添加目录文件的记录

	//外部认证DDF的主控密钥
	m_strTemp = "外部认证"+strDDF+"的主控密钥";
	LvOne_Annotations( m_strTemp, m_bRefreshFlag );
	cParameter->GetDDFInfo( strDDF, struDDFInfo );//获取DDF信息
	cCmd->VAR( struDDFInfo.m_strMKvalue, _KEY, m_bRefreshFlag );//将DDF的MK保存到_KEY
	cInst->Get_Challenge( 8, m_bRefreshFlag );
	cCmd->RESP( 0, 8, _VAR10, m_bRefreshFlag );
	cCmd->TDES( VAR_Name(_VAR10), VAR_Name(_KEY), _VAR11, m_bRefreshFlag );
	cCmd->DES( VAR_Name(_VAR10), VAR_Name(_VAR11), _VAR12, m_bRefreshFlag );
	cInst->External_Authenticate( "00", VAR_Name(_VAR12), VAR_Name(_VAR10), 0, m_bRefreshFlag );//MK的密钥标识为00
	LineFeed(1);

}

void CScript::CreateADFS( string strDDF /*= ""*/ )
{
	string strTmp,strDDFFID;

	transform( strDDF.begin(), strDDF.end(), strDDF.begin(), toupper );

	vector<S_DDFINFO>::const_iterator iterSDDF = m_vecMFStruct.begin();
	while ( iterSDDF != m_vecMFStruct.end() )//遍历DDF
	{
		strTmp = (*iterSDDF).m_strDDFName;//应用环境简称
		strDDFFID = (*iterSDDF).m_strFID;//DDF的文件标识符
		transform(  strTmp.begin(), strTmp.end(), strTmp.begin(), toupper );
		transform(  strDDFFID.begin(), strDDFFID.end(), strDDFFID.begin(), toupper );
		if ( !(strTmp == strDDF || strDDFFID == strDDF) )//不为DDF文件标识符或不为应用环境简称时继续遍历(读取的数据中DDF0的FID为"")
		{
			++iterSDDF;
			continue;
		}
		//当前DDF下没有ADF(除了DF00)，不用建ADF目录文件，跳出循环
		if ( (*iterSDDF).m_vecADFInfo.size()==0 || ( (*iterSDDF).m_vecADFInfo.size()==1 && (*iterSDDF).m_vecADFInfo.front().m_strADFName=="DF00" ) )
			break;
		if ( strTmp == "DDF0" )
			m_strTemp = "创建MF下的所有ADF";
		else
			m_strTemp = "创建"+strTmp+"下的所有ADF";
		LvTwo_Annotations( m_strTemp );
		cInst->Select_File( "MF", (*iterSDDF).m_strFID, "", "", m_bRefreshFlag );
		//找到对应的DDF后，遍历ADF，建该DDF下的所有ADF
		vector<S_ADFINFO>::const_iterator iterSADF = (*iterSDDF).m_vecADFInfo.begin();
		while ( iterSADF!=(*iterSDDF).m_vecADFInfo.end() )//遍历ADF
		{
			strTmp = (*iterSADF).m_strFID;
			transform(  strTmp.begin(), strTmp.end(), strTmp.begin(), toupper );
			if ( strTmp == "" )//DF00的话继续遍历
			{
				++iterSADF;
				continue;
			}
			m_strTemp = "创建"+(*iterSADF).m_strADFName;
			LvOne_Annotations( m_strTemp );
			//非DF00创建ADF文件
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
	while ( iterSDDF != m_vecMFStruct.end() )//遍历DDF
	{
		strTmp = (*iterSDDF).m_strDDFName;//应用环境简称
		transform(  strTmp.begin(), strTmp.end(), strTmp.begin(), toupper );
		if ( strTmp != strDDF )//不为应用环境简称时继续遍历
		{
			++iterSDDF;
			continue;
		}

		vector<S_ADFINFO>::const_iterator iterSADF = (*iterSDDF).m_vecADFInfo.begin();
		while ( iterSADF!=(*iterSDDF).m_vecADFInfo.end() )//遍历ADF
		{
			strTmp = (*iterSADF).m_strFID;
			transform(  strTmp.begin(), strTmp.end(), strTmp.begin(), toupper );
			if ( strTmp != strADF )//不为ADF文件标识符的话继续遍历(读取的数据中DF00的FID为"")
			{
				++iterSADF;
				continue;
			}
			if ( (*iterSADF).m_vecEFInfo.size() == 0 )//说明ADF下没有EF文件,直接跳出循环
				break;
			if ( strDDF == "DDF0" && strADF == "" )//表示建MF下的所有直属EF文件
			{
				LvTwo_Annotations( "创建MF下的EF文件" );
				LvOne_Annotations( "选择MF" );
				cInst->Select_File( "MF", (*iterSDDF).m_strFID, (*iterSADF).m_strFID, "", m_bRefreshFlag );
			}
			else if ( strDDF != "DDF0" && strADF == "" )//表示建MF/DDF下的所有直属EF文件
			{
				m_strTemp = "创建 MF-"+(*iterSDDF).m_strDDFName+" 下的EF文件";
				LvTwo_Annotations( m_strTemp );
				m_strTemp = "选择"+(*iterSDDF).m_strDDFName;
				LvOne_Annotations( m_strTemp );
				cInst->Select_File( "MF", (*iterSDDF).m_strFID, (*iterSADF).m_strFID, "", m_bRefreshFlag );
			}
			else//表示建MF/ADF下的所有直属EF文件 或者 建MF/DDF/ADF下的所有直属EF文件
			{
				if ( (*iterSDDF).m_strFID == "" )
					m_strTemp = "创建 MF-"+(*iterSADF).m_strFID+" 下的EF文件";
				else
					m_strTemp = "创建 MF-"+(*iterSDDF).m_strDDFName+"-"+(*iterSADF).m_strFID+" 下的EF文件";
				LvTwo_Annotations( m_strTemp );
				m_strTemp = "选择"+(*iterSADF).m_strFID;
				LvOne_Annotations( m_strTemp );
				cInst->Select_File( "MF", (*iterSDDF).m_strFID, (*iterSADF).m_strFID, "", m_bRefreshFlag );

				//创建安全文件
				LvOne_Annotations( "创建安全文件" );
				strLen = LoadKeys( (*iterSDDF).m_strDDFName, (*iterSADF).m_strFID, strsRecords );//获取装载密钥的记录和密钥文件空间
				cInst->Create_Internal_EF( "SF", strLen, m_bRefreshFlag  );
				cInfo->Append_Script( strsRecords.str() );//添加所有的装载密钥脚本数据

				//外部认证ADF的主控密钥
				m_strTemp = "外部认证"+(*iterSADF).m_strFID+"的主控密钥";
				LvOne_Annotations( m_strTemp, m_bRefreshFlag );
				cParameter->GetADFInfo( (*iterSDDF).m_strFID, (*iterSADF).m_strFID,  struADFInfo );//获取ADF信息
				cCmd->VAR( struADFInfo.m_strMKvalue, _KEY, m_bRefreshFlag );//将ADF的MK保存到_KEY
				cInst->Get_Challenge( 8, m_bRefreshFlag );
				cCmd->RESP( 0, 8, _VAR10, m_bRefreshFlag );
				cCmd->TDES( VAR_Name(_VAR10), VAR_Name(_KEY), _VAR11, m_bRefreshFlag );
				cCmd->DES( VAR_Name(_VAR10), VAR_Name(_VAR11), _VAR12, m_bRefreshFlag );
				cInst->External_Authenticate( "00", VAR_Name(_VAR12), VAR_Name(_VAR10), 0, m_bRefreshFlag );//MK的密钥标识为00
				LineFeed(1);
			}

			strTmp = (*iterSADF).m_strAID;
			transform(  strTmp.begin(), strTmp.end(), strTmp.begin(), toupper );
			if ( strDDF == "SSSE" && strTmp == "D15600000503" )
			{
				//社会保险数据区2目录文件下需要建TF内部文件才能使用医疗消费等功能
				LvOne_Annotations( "建立基本文件(医疗保险交易内部文件) 读：FF(禁止) 写: DSK", m_bRefreshFlag );
				cInst->Create_Internal_EF( "TF", "1F", m_bRefreshFlag  );
				LineFeed(1);
			}
			vector<S_EFINFO>::const_iterator iterEF = (*iterSADF).m_vecEFInfo.begin();
			while ( iterEF!=(*iterSADF).m_vecEFInfo.end() )//遍历EF，建EF文件并初始化文件
			{
				//文件类型
				if ( "1" == (*iterEF).m_strEFType )
					strEFType = "定长文件";
				if ( "2" == (*iterEF).m_strEFType )
					strEFType = "变长文件";
				if ( "3" == (*iterEF).m_strEFType )
					strEFType = "透明文件";
				if ( "4" == (*iterEF).m_strEFType )
					strEFType = "循环文件";
				if ( "5" == (*iterEF).m_strEFType )
					strEFType = "内部文件";
				m_strTemp = "建立基本文件"+(*iterEF).m_strSFI+" 读："+(*iterEF).m_strReadControl+" 写："+(*iterEF).m_strWriteControl+" 文件类型："+strEFType;
				LvOne_Annotations( m_strTemp, m_bRefreshFlag );
				cInst->Create_File( "MF", (*iterSDDF).m_strDDFName, (*iterSADF).m_strFID, (*iterEF).m_strSFI, m_bRefreshFlag );//建EF文件
				cInst->Select_File( "MF", (*iterSDDF).m_strFID, (*iterSADF).m_strFID, (*iterEF).m_strSFI, m_bRefreshFlag );//选当前建的EF文件
				//调用初始化文件接口写记录
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
	LvTwo_Annotations("预个人化结束");
	LvOne_Annotations("选择MF,预个人化结束");
	cInst->Select_File( "MF", "", "", "", m_bRefreshFlag );
	cInst->End_PreIssue( m_bRefreshFlag );
	LineFeed(1);

	vector<S_DDFINFO>::const_iterator iterSDDF = m_vecMFStruct.begin();
	while ( iterSDDF != m_vecMFStruct.end() )
	{
		if ( (*iterSDDF).m_strFID != "" )//非DDF0
		{
			m_strTemp = "选择MF-"+(*iterSDDF).m_strDDFName+",预个人化结束";
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
			if ( (*iterSADF).m_strFID != "" )//非DF00
			{
				if ( (*iterSDDF).m_strFID != "" )//非DDF0
					m_strTemp = "选择MF-"+(*iterSDDF).m_strDDFName+"-"+(*iterSADF).m_strFID+",预个人化结束";
				else
					m_strTemp = "选择MF-"+(*iterSADF).m_strFID+",预个人化结束";
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