//#####################################
//
//	大唐T514 COS指令
//
//	最后修改日期：2015-11-09 17:34
//#####################################

#include "chip.h"
#include "tool.h"
#include <iomanip>

const string COS_T514::MK_Level = "FF";//主控密钥的安全级别
const string COS_T514::Key_Forbidden = "EF";//禁止权限
const string COS_T514::Key_Free = "00";//访问自由


COS_T514::COS_T514(CInformation *inf, CParameter *para) :CInstruction(inf, para)
{	}

string COS_T514::Delete_File(int WorkFlag, string FType /*= ""*/, string FID /*= ""*/, bool RefreshFlag /*= true*/ )
{
	bool flag = false;//该函数内的错误标志
	Initialization();
	//参数检查
	if (ErrCheckFlag)
	{
		if (!FID.empty() && (!IsHex(FID) || FID.length() != 4))
		{
			Set_Error_Flag();
			serr << "删除文件指令 Delete_File，参数FID必须由4个十六进制字符组成，而当前为“" << FID << "”。";
			Info->Append_Error(serr.str());
			flag = true;
		}
		if ( FType!="MF" && FType!="DDF" && FType!="ADF" && FType!="EF" )
		{
			Set_Error_Flag();
			serr << "删除文件指令 Delete_File，参数FType必须为\"MF\"、\"DDF\"、\"ADF\"、\"EF\"的一种，而当前为“" << FType << "”。";
			Info->Append_Error(serr.str());
			flag = true;
		}
	}
	if (flag)
		return "";
	//指令构建
	sstr << "80" << Style_InstHead;
	sstr << "E1" << Style_InstHead;
	sstr << "00" << Style_InstHead;
	if ( FType=="MF" || FType=="DDF" )
		sstr << "01" << Style_InstHead;
	else if ( FType=="ADF" )
		sstr << "02" << Style_InstHead;
	else if ( FType=="EF" )
		sstr << "03" << Style_InstHead;
	sstr << setfill('0') << setw(2) << left << DecToHex(FID.length()/2) << Style_Separator;
	sstr << FID;
	strcpy(InstHead, StrTrim(sstr.str()).substr(0, 10).c_str());
	strcpy(InstData, StrTrim(sstr.str()).erase(0, 10).c_str());
	//收尾
	if (RefreshFlag)
		Info->Append_Script(sstr.str());
	return sstr.str();
}

string COS_T514::Delete_File(string MF, string DDF, string ADF, string EF, bool RefreshFlag /*= true*/ )
{
	return "有问题啊！";
}

string COS_T514::Verify_TranCode( int Len, string Data, bool RefreshFlag /*= true*/ )
{
	bool flag = false;//该函数内的错误标志
	string str;
	string estr;
	Initialization();
	//参数检查
	if (ErrCheckFlag)
	{
		if (Len<0 || Len>255)
		{
			Set_Error_Flag();
			serr << "传输密钥验证 Verify_TranCode(" << Len << ", " << Data << ")，指定长度有误。";
			Info->Append_Error(serr.str());
			flag = true;
		}
	}
	//数据域检查及调整
	if (StrDel_ConVar(Data, str) == false)
	{
		if (ErrCheckFlag && !CheckData(str, Len, estr))
		{
			Set_Error_Flag();
			serr << "传输密钥验证 Verify_TranCode(" << Len << ", " << Data << ")，数据域参数有误，" << estr;
			Info->Append_Error(serr.str());
			flag = true;
		}
		if (!Style_InstData.empty())
			Data = StrTrim(Data);
	}
	if (flag)
		return "";
	//指令构建
	sstr << "80" << Style_InstHead;
	sstr << "E0" << Style_InstHead;
	sstr << "03" << Style_InstHead;
	sstr << "00" << Style_InstHead;
	sstr << setfill('0') << setw(2) << hex << uppercase << right << Len << Style_Separator;
	sstr << ToUpper(Data);
	strcpy(InstHead, StrTrim(sstr.str()).substr(0, 10).c_str());
	strcpy(InstData, StrTrim(sstr.str()).erase(0, 10).c_str());
	//收尾
	if (RefreshFlag)
		Info->Append_Script(sstr.str());
	return sstr.str();
}

string COS_T514::End_PreIssue( bool RefreshFlag /*= true*/ )
{
	Initialization();
	//指令构建
	sstr << "80" << Style_InstHead;
	sstr << "E0" << Style_InstHead;
	sstr << "02" << Style_InstHead;
	sstr << "00" << Style_InstHead;
	sstr << "00";
	strcpy(InstHead, sstr.str().c_str());
	//收尾
	if (RefreshFlag)
		Info->Append_Script(sstr.str());
	return sstr.str();
}

string COS_T514::Create_Internal_EF(string FType, string DataLen, bool RefreshFlag /*= true*/)
{
	bool flag = false;//该函数内的错误标志
	string efSFI = "";//短文件标识符
	string readCtrl = "";//读控制
	string writeCtrl = "";//写控制
	string efType = "";//文件类型
	string recordLen = "";//记录长度
	string str = "";
	string estr = "";
	KEYDATAS keyData;
	Initialization();
	//参数检查
	if (ErrCheckFlag)
	{
		if (FType != "SF" && FType != "FCI" && FType != "DIR" && FType != "TF" && FType != "ATR")
		{
			Set_Error_Flag();
			serr << "创建内部文件指令 Create_Internal_EF(" << FType << ", " << DataLen << ")，指定的内部文件类型不正确。";
			Info->Append_Error(serr.str());
			flag = true;
		}
		if (DataLen.length() > 4 || !IsHex(DataLen))
		{
			Set_Error_Flag();
			serr << "创建内部文件指令 Create_Internal_EF(" << FType << ", " << DataLen << ")，文件空间不正确。";
			Info->Append_Error(serr.str());
			flag = true;
		}
	}
	//文件属性
	if (FType == "SF")
	{
		efSFI = SF_SFI;
		efType = "C3";
		recordLen = "18";
		readCtrl = Key_Forbidden + Key_Forbidden;//读禁止
		writeCtrl = Key_Free + MK_Level;//写受当前目录主控控制
	}
	else if (FType == "FCI")
	{
		efSFI = FCI_SFI;
		efType = "C3";
		recordLen = "FF";
		readCtrl = Key_Forbidden + Key_Forbidden;//读禁止
		writeCtrl = Key_Free + MK_Level;//写受当前目录主控控制
	}
	else if (FType == "ATR")
	{
		Set_Error_Flag();
		serr << "创建内部文件指令 Create_Internal_EF(" << FType << ", " << DataLen << ")，大唐T514芯片并没有所谓的ATR文件。";
		Info->Append_Error(serr.str());
		flag = true;
	}
	else if (FType == "DIR")
	{
		efSFI = DIR_SFI;
		efType = "D6";
		recordLen = "FF";
		readCtrl = Key_Free + Key_Free;//读自由
		writeCtrl = Key_Free + MK_Level;//写受当前目录主控控制
	}
	else if (FType == "TF")
	{
		efSFI = TF_SFI;
		efType = "C3";
		recordLen = "00";
		readCtrl = Key_Forbidden + Key_Forbidden;//读禁止
		if (Param->GetKeyInfo("SSSE", "DF04", "DSK", keyData) == false)
		{
			Set_Error_Flag();
			serr << "创建内部文件指令 Create_Internal_EF(" << FType << ", " << DataLen << ")，在SSSE的DF04下找不到密钥DSK。";
			Info->Append_Error(serr.str());
			flag = true;
		}
		else
			writeCtrl = Key_Free + Key_Security_Level(keyData);
	}
	if (flag)
		return  "";
	//指令构建
	sstr << "80" << Style_InstHead;
	sstr << "E0" << Style_InstHead;
	sstr << "01" << Style_InstHead;
	sstr << "00" << Style_InstHead;
	sstr << "10" << Style_Separator;
	sstr << "000000";
	sstr << setfill('0') << setw(4) << hex << uppercase << right << HexToDec(DataLen) + 2 << Style_InstData;
	sstr << "EF" << efSFI << Style_InstData;
	sstr << efType;
	sstr << "00";
	sstr << efSFI;
	sstr << "00";
	sstr << recordLen << Style_InstData;
	sstr << readCtrl << Style_InstData;
	sstr << writeCtrl;
	strcpy(InstHead, StrTrim(sstr.str()).substr(0, 10).c_str());
	strcpy(InstData, StrTrim(sstr.str()).erase(0, 10).c_str());
	//收尾
	if (RefreshFlag)
		Info->Append_Script(sstr.str());
	return sstr.str();
}

string COS_T514::Create_File( string MF /*= ""*/, string DDF /*= ""*/, string ADF /*= ""*/, string EF /*= ""*/, bool RefreshFlag /*= true*/ )
{
	bool flag = false;//该函数内的错误标志
	S_MFINFO mfInfo;
	S_DDFINFO ddfInfo;
	S_ADFINFO adfInfo;
	S_EFINFO efInfo;
	KEYDATAS keyData1;
	KEYDATAS keyData2;
	EFDATAS efData;//创建定长记录文件时检查各记录长度及判断有误记录标识符
	bool efFlag = true;//创建定长记录文件有效，该值为true时表示定长记录文件含有Tag、Length字节
	string str;//临时变量
	string efSpace;//EF文件空间
	string efType;//EF文件类型
	string efRecordLen;//EF记录长度
	string efSCW;//用于计算EF的SCW（密钥逻辑关系）
	string readCtrl = "";//读控制
	string writeCtrl = "";//写控制
	int i;//计数变量
	Initialization();
	//参数检查
	if (ErrCheckFlag)
	{
		if (MF != "MF" && MF != "3F00" && MF != "")
		{
			Set_Error_Flag();
			serr << "创建文件指令 Create_File（" << MF << ", " << DDF << ", " << ADF << ", " << EF << "），MF只可取值为“MF”、“3F00”或空串“”，而当前是“" << MF << "”。";
			Info->Append_Error(serr.str());
			flag = true;
		}
		if (!ADF.empty() && (!IsHex(ADF) || ADF.length() != 4))
		{
			Set_Error_Flag();
			serr << "创建文件指令 Create_File（" << MF << ", " << DDF << ", " << ADF << ", " << EF << "），参数ADF必须由4个十六进制字符组成，而当前为“" << ADF << "”。";
			Info->Append_Error(serr.str());
			flag = true;
		}
		if (!EF.empty() && (!IsHex(EF) || EF.length() != 4))
		{
			Set_Error_Flag();
			serr << "创建文件指令 Create_File（" << MF << ", " << DDF << ", " << ADF << ", " << EF << "），参数EF必须由4个十六进制字符组成，而当前为“" << EF << "”。";
			Info->Append_Error(serr.str());
			flag = true;
		}
	}
	//构建指令头部
	sstr << "80" << Style_InstHead;
	sstr << "E0" << Style_InstHead;
	sstr << "01" << Style_InstHead;
	sstr << "00" << Style_InstHead;
	//获取配置 并 构建指令
	if (EF.empty() == false)//EF
	{
		if (Param->GetEFInfo(DDF, ADF, EF, efInfo) == false)
		{
			Set_Error_Flag();
			serr << "创建文件指令 Create_File（" << MF << ", " << DDF << ", " << ADF << ", " << EF << "），找不到指定路径的EF文件。";
			Info->Append_Error(serr.str());
			flag = true;
		}
		//EF - 配置检查
		if (ErrCheckFlag)
		{
			if (efInfo.m_strSFI.length() % 2 != 0 || !IsHex(efInfo.m_strSFI))
			{
				Set_Error_Flag();
				serr << "创建文件指令 Create_File（" << MF << ", " << DDF << ", " << ADF << ", " << EF << "），指定EF文件的FID“" << efInfo.m_strSFI << "”有误。";
				Info->Append_Error(serr.str());
				flag = true;
			}
		}
		//EF - 文件类型
		if (efInfo.m_strEFType == EFTYPE_FIXED_LENGTH)//定长记录
		{
			efSpace = DecToHex(HexToDec(efInfo.m_strEFLen) + HexToDec(efInfo.m_strResSpace) + efInfo.m_nRecordCount * 2 + 2);
			efType = "C5";
			if (ErrCheckFlag)
			{
				if (efInfo.m_nRecordCount == 0)
				{
					Set_Error_Flag();
					serr << "创建文件指令 Create_File（" << MF << ", " << DDF << ", " << ADF << ", " << EF << "），创建定长记录文件时，要求至少有一条记录来指定定长文件的记录长度，而当前所需创建的文件中并没有任何记录信息。";
					Info->Append_Error(serr.str());
					flag = true;
				}
				if (efInfo.m_nRecordCount > 0 && HexToDec(efInfo.m_strEFLen) % efInfo.m_nRecordCount != 0)
				{
					Set_Error_Flag();
					serr << "创建文件指令 Create_File（" << MF << ", " << DDF << ", " << ADF << ", " << EF << "），要求定长记录文件的记录长度之和“0x" << efInfo.m_strEFLen << "”必须是记录数目“" << efInfo.m_nRecordCount << "”的倍数。";
					Info->Append_Error(serr.str());
					flag = true;
				}
				if (efSpace.length() > 4)
				{
					Set_Error_Flag();
					serr << "创建文件指令 Create_File（" << MF << ", " << DDF << ", " << ADF << ", " << EF << "），文件空间超长（文件内容“0x" << efInfo.m_strEFLen << "” + TagLength空间“" << efInfo.m_nRecordCount << "*2” + 预留空间“0x" << efInfo.m_strResSpace << "” + 2）。";
					Info->Append_Error(serr.str());
					flag = true;
				}
				for (i = 1; i <= efInfo.m_nRecordCount; ++i)
				{
					if (Param->GetEFData(DDF, ADF, EF, i, efData) == false)
					{
						serr.clear();
						serr.str("");
						serr << "创建文件指令 Create_File（" << MF << ", " << DDF << ", " << ADF << ", " << EF << "），该EF文件指定该含有 " << efInfo.m_nRecordCount << "条记录，但却找不到第 " << i << "条记录。";
						Info->Append_Warning(serr.str());
						break;
					}
					if ((efInfo.m_nRecordCount > 0) && (HexToDec(efInfo.m_strEFLen) / efInfo.m_nRecordCount != HexToDec(efData.m_strLen)))
					{
						Set_Error_Flag();
						serr << "创建文件指令 Create_File（" << MF << ", " << DDF << ", " << ADF << ", " << EF << "），指定的定长记录文件各记录长度不相等。";
						Info->Append_Error(serr.str());
						flag = true;
						break;
					}
					if (i == 1)
					{
						if (efData.m_strTag == "-" || efData.m_strTag.empty())
							efFlag = false;
						else
							efFlag = true;
					}
					else
					{
						if (((efData.m_strTag == "-" || efData.m_strTag.empty()) && efFlag) || (!(efData.m_strTag == "-" || efData.m_strTag.empty()) && !efFlag))
						{
							Set_Error_Flag();
							serr << "创建文件指令 Create_File（" << MF << ", " << DDF << ", " << ADF << ", " << EF << "），指定的定长记录文件中，有些记录含有记录标识符，有些却没有。";
							Info->Append_Error(serr.str());
							flag = true;
							break;
						}
					}
				}
			}
			if (efInfo.m_nRecordCount > 0)
				efRecordLen = DecToHex(HexToDec(efInfo.m_strEFLen) / efInfo.m_nRecordCount + (efFlag ? 2 : 0));
			else
				efRecordLen = "0";
		}
		else if (efInfo.m_strEFType == EFTYPE_VARIABLE_LENGTH)//变长记录
		{
			efSpace = DecToHex(HexToDec(efInfo.m_strEFLen) + HexToDec(efInfo.m_strResSpace) + efInfo.m_nRecordCount * 2 + 2);
			efType = "C6";
			if (ErrCheckFlag)
			{
				if (efSpace.length() > 4)
				{
					Set_Error_Flag();
					serr << "创建文件指令 Create_File（" << MF << ", " << DDF << ", " << ADF << ", " << EF << "），文件空间超长（文件内容“0x" << efInfo.m_strEFLen << "” + TagLength空间“" << efInfo.m_nRecordCount << "*2” + 预留空间“0x" << efInfo.m_strResSpace << "” + 2）。";
					Info->Append_Error(serr.str());
					flag = true;
				}
			}
			efRecordLen = "FF";
		}
		else if (efInfo.m_strEFType == EFTYPE_BINARY)//透明文件
		{
			efSpace = DecToHex(HexToDec(efInfo.m_strEFLen) + HexToDec(efInfo.m_strResSpace));
			efType = "C4";
			if (ErrCheckFlag)
			{
				if (efSpace.length() > 4)
				{
					Set_Error_Flag();
					serr << "创建文件指令 Create_File（" << MF << ", " << DDF << ", " << ADF << ", " << EF << "），文件空间超长（文件内容“0x" << efInfo.m_strEFLen << "” + 预留空间“0x" << efInfo.m_strResSpace << "”）。";
					Info->Append_Error(serr.str());
					flag = true;
				}
			}
			efRecordLen = "00";
		}
		else if (efInfo.m_strEFType == EFTYPE_LOOP)//循环文件
		{
			efSpace = DecToHex(HexToDec(efInfo.m_strEFLen) * atoi(efInfo.m_strnCount.c_str()) + HexToDec(efInfo.m_strResSpace) + 2);
			efType = "C7";
			if (ErrCheckFlag)
			{
				if (efSpace.length() > 4)
				{
					Set_Error_Flag();
					serr << "创建文件指令 Create_File（" << MF << ", " << DDF << ", " << ADF << ", " << EF << "），文件空间超长（文件内容“0x" << efInfo.m_strEFLen << "” + 预留空间“0x" << efInfo.m_strResSpace << "” + 2）。";
					Info->Append_Error(serr.str());
					flag = true;
				}
				if (HexToDec(efInfo.m_strEFLen) == 0)
				{
					Set_Error_Flag();
					serr << "创建文件指令 Create_File（" << MF << ", " << DDF << ", " << ADF << ", " << EF << "），指定需创建的循环文件中并没有记录信息。";
					Info->Append_Error(serr.str());
					flag = true;
				}
			}
			efRecordLen = efInfo.m_strEFLen;
		}
		else//其他
		{
			Set_Error_Flag();
			serr << "创建文件指令 Create_File（" << MF << ", " << DDF << ", " << ADF << ", " << EF << "），EF文件的类型“" << efInfo.m_strEFType << "”不正确。";
			Info->Append_Error(serr.str());
			flag = true;
		}
		//EF - 读控制密钥
		switch (EF_Control(DDF, ADF, efInfo.m_strReadControl, keyData1, keyData2, str))
		{
		case 1://自由
			efSCW = "0";
			readCtrl = Key_Free + Key_Free;
			break;
		case 2://禁止
			efSCW = "0";
			readCtrl = Key_Forbidden + Key_Forbidden;
			break;
		case 3://上级目录PIN
			efSCW = "0";
			readCtrl = Key_Security_Level(keyData1) + Key_Free;
			break;
		case 4://当前目录密钥
			efSCW = "0";
			readCtrl = Key_Free + Key_Security_Level(keyData1);
			break;
		case 5://上级及当前目录密钥“与”逻辑
			efSCW = "4";
			readCtrl = Key_Security_Level(keyData1) + Key_Security_Level(keyData2);
			break;
		case 6://上级及当前目录密钥“或”逻辑
			efSCW = "0";
			readCtrl = Key_Security_Level(keyData1) + Key_Security_Level(keyData2);
			break;
		default:
			Set_Error_Flag();
			serr << "创建文件指令 Create_File（" << MF << ", " << DDF << ", " << ADF << ", " << EF << "），读权限状态" << str;
			Info->Append_Error(serr.str());
			flag = true;
			break;
		}
		//EF - 写控制密钥
		switch (EF_Control(DDF, ADF, efInfo.m_strWriteControl, keyData1, keyData2, str))
		{
		case 1://自由
			efSCW += "0";
			writeCtrl = Key_Free + Key_Free;
			break;
		case 2://禁止
			efSCW += "0";
			writeCtrl = Key_Forbidden + Key_Forbidden;
			break;
		case 3://上级目录PIN
			efSCW = "0";
			readCtrl = Key_Security_Level(keyData1) + Key_Free;
			break;
		case 4://当前目录密钥
			efSCW += "0";
			writeCtrl = Key_Free + Key_Security_Level(keyData1);
			break;
		case 5://上级及当前目录密钥“与”逻辑
			efSCW += "4";
			writeCtrl = Key_Security_Level(keyData1) + Key_Security_Level(keyData2);
			break;
		case 6://上级及当前目录密钥“或”逻辑
			efSCW += "0";
			writeCtrl = Key_Security_Level(keyData1) + Key_Security_Level(keyData2);
			break;
		default:
			Set_Error_Flag();
			serr << "创建文件指令 Create_File（" << MF << ", " << DDF << ", " << ADF << ", " << EF << "），写权限状态" << str;
			Info->Append_Error(serr.str());
			flag = true;
			break;
		}
		if (flag)
			return "";
		//EF - 构建指令
		sstr << "10" << Style_Separator;
		sstr << "00";
		sstr << "00" << "00";
		sstr << setfill('0') << setw(4) << right << ToUpper(efSpace) << Style_InstData;
		sstr << ToUpper(efInfo.m_strSFI) << Style_InstData;
		sstr << efType;
		sstr << "00";
		sstr << ToUpper(efInfo.m_strSFI).substr(2, 2);
		sstr << efSCW;
		sstr << setfill('0') << setw(2) << right << efRecordLen << Style_InstData;
		sstr << readCtrl << Style_InstData;
		sstr << writeCtrl;
	}
	else if (ADF.empty() == false)//ADF
	{
		if (Param->GetADFInfo(DDF, ADF, adfInfo) == false)
		{
			Set_Error_Flag();
			serr << "创建文件指令 Create_File（" << MF << ", " << DDF << ", " << ADF << ", " << EF << "），找不到指定路径的ADF文件。";
			Info->Append_Error(serr.str());
			flag = true;
		}
		//ADF - 配置检查
		if (ErrCheckFlag)
		{
			if (adfInfo.m_strAID.length() % 2 != 0 || !IsHex(adfInfo.m_strAID))
			{
				Set_Error_Flag();
				serr << "创建文件指令 Create_File（" << MF << ", " << DDF << ", " << ADF << ", " << EF << "），指定ADF文件的AID“" << adfInfo.m_strAID << "”有误。";
				Info->Append_Error(serr.str());
				flag = true;
			}
			if (adfInfo.m_strFID.length() != 4 || !IsHex(adfInfo.m_strFID))
			{
				Set_Error_Flag();
				serr << "创建文件指令 Create_File（" << MF << ", " << DDF << ", " << ADF << ", " << EF << "），指定ADF文件的FID“" << adfInfo.m_strFID << "”有误。";
				Info->Append_Error(serr.str());
				flag = true;
			}
		}
		if (flag)
			return "";
		//ADF - 构建指令
		sstr << setfill('0') << setw(2) << hex << uppercase << right << 0x10 + adfInfo.m_strAID.length() / 2 << Style_Separator;
		sstr << "00";
		if (Param->GetKeyInfo(DDF, ADF, "LK" + ADF, keyData1))//应用锁密钥的后续安全状态
			sstr << Key_Security_Level(keyData1);
		else
			sstr << "00";
		sstr << Key_Free << MK_Level;//创建删除权限
		sstr << "01" << Style_InstData;//预个人化和应用阶段都使用DES算法
		sstr << ToUpper(adfInfo.m_strFID) << Style_InstData;
		sstr << "02";//ADF的文件类型，固定为"02"
		sstr << "00" << "00";
		sstr << "00";//FCI_SFI
		sstr << "00";//安全控制字节
		sstr << "00";//声明周期
		sstr << "00";//保留项
		sstr << "00";//保留项
		sstr << "00" << Style_InstData;//保留项
		sstr << ToUpper(adfInfo.m_strAID);
	}
	else if (DDF.empty() == false)//DDF
	{
		if (Param->GetDDFInfo(DDF, ddfInfo) == false)
		{
			Set_Error_Flag();
			serr << "创建文件指令 Create_File（" << MF << ", " << DDF << ", " << ADF << ", " << EF << "），找不到指定路径的DDF文件。";
			Info->Append_Error(serr.str());
			flag = true;
		}
		//DDF - 配置检查
		if (ErrCheckFlag)
		{
			if (ddfInfo.m_strAID.length() % 2 != 0 || !IsHex(ddfInfo.m_strAID))
			{
				Set_Error_Flag();
				serr << "创建文件指令 Create_File（" << MF << ", " << DDF << ", " << ADF << ", " << EF << "），指定DDF文件的AID“" << ddfInfo.m_strAID << "”有误。";
				Info->Append_Error(serr.str());
				flag = true;
			}
			if (ddfInfo.m_strFID.length() != 4 || !IsHex(ddfInfo.m_strFID))
			{
				Set_Error_Flag();
				serr << "创建文件指令 Create_File（" << MF << ", " << DDF << ", " << ADF << ", " << EF << "），指定DDF文件的FID“" << ddfInfo.m_strFID << "”有误。";
				Info->Append_Error(serr.str());
				flag = true;
			}
		}
		if (flag)
			return "";
		//DDF - 构建指令
		sstr << setfill('0') << setw(2) << hex << uppercase << right << 0x10 + ddfInfo.m_strAID.length() / 2 << Style_Separator;
		sstr << "00";
		if (Param->GetKeyInfo(DDF, ADF, "BK", keyData1) == false)//卡锁密钥
		{
			if (DDF == "SSSE")
			{
				Set_Error_Flag();
				serr << "创建文件指令 Create_File（" << MF << ", " << DDF << ", " << ADF << ", " << EF << "），创建SSSE时找不到密钥BK。";
				Info->Append_Error(serr.str());
				flag = true;
			}
			else
				sstr << "00";
		}
		else
			sstr << Key_Security_Level(keyData1);
		sstr << Key_Free << MK_Level;//创建删除权限
		sstr << "01" << Style_InstData;//预个人化和应用阶段都使用DES算法
		sstr << ToUpper(ddfInfo.m_strFID) << Style_InstData;
		sstr << "01";
		sstr << "00";
		sstr << "00";
		sstr << FCI_SFI;
		sstr << "00";
		sstr << "00";
		sstr << SF_SFI;
		sstr << "00";
		sstr << TF_SFI;
		sstr << ToUpper(ddfInfo.m_strAID);
	}
	else//MF
	{
		Param->GetMFInfo(mfInfo);
		//MF - 配置检查
		if (ErrCheckFlag)
		{
			if (mfInfo.m_strAID.length() % 2 != 0 || !JudgeHex(mfInfo.m_strAID))//允许为空
			{
				Set_Error_Flag();
				serr << "创建文件指令 Create_File（" << MF << ", " << DDF << ", " << ADF << ", " << EF << "），指定MF文件的AID“" << mfInfo.m_strAID << "”有误。";
				Info->Append_Error(serr.str());
				flag = true;
			}
			if (mfInfo.m_strFID != "3F00")
			{
				Set_Error_Flag();
				serr << "创建文件指令 Create_File（" << MF << ", " << DDF << ", " << ADF << ", " << EF << "），MF文件的FID要求必须是“3F00”，而当前却是“" << mfInfo.m_strFID << "”。";
				Info->Append_Error(serr.str());
				flag = true;
			}
		}
		if (flag)
			return "";
		//MF - 构建指令
		sstr << setfill('0') << setw(2) << hex << uppercase << right << 0x10 + mfInfo.m_strAID.length() / 2 << Style_Separator;
		sstr << "00";
		sstr << "00";//卡锁密钥
		sstr << "00" << MK_Level;//创建删除权限字
		sstr << "01" << Style_InstData;
		sstr << ToUpper(mfInfo.m_strFID) << Style_InstData;
		sstr << "01";
		sstr << "00";
		sstr << "01";//社保DDF执行卡锁，同时锁定MF
		sstr << FCI_SFI;
		sstr << "00";
		sstr << "00";
		sstr << SF_SFI;
		sstr << "00";
		sstr << "00" << Style_InstData;
		sstr << ToUpper(mfInfo.m_strAID);
	}
	strcpy(InstHead, StrTrim(sstr.str()).substr(0, 10).c_str());
	strcpy(InstData, StrTrim(sstr.str()).erase(0, 10).c_str());
	//收尾
	if (RefreshFlag)
		Info->Append_Script(sstr.str());
	return sstr.str();
}

string COS_T514::Write_Key(string MF, string DDF, string ADF, string KeyName, string KeyVer, bool RefreshFlag /*= true*/ )
{
	bool flag = false;//该函数内的错误标志
	S_MFINFO mfInfo;
	S_DDFINFO ddfInfo;
	S_ADFINFO adfInfo;
	KEYDATAS keyData;
	string keyLen;//密钥长度
	string keyValue;//密钥值
	Initialization();
	//获取指令头与命令头
	if (Write_Key(MF, DDF, ADF, KeyName, KeyVer, false, false) == false)
		return "";
	//取密钥值，在上一步已完成错误检查工作，此处无需再继续
	if (KeyName.compare(0, 2, "MK") == 0)//安装主控
	{
		if (ADF.empty() == false)//ADF
		{
			Param->GetADFInfo(DDF, ADF, adfInfo);
			keyValue = adfInfo.m_strMKvalue;
		}
		else if (DDF.empty() == false)//DDF
		{
			Param->GetDDFInfo(DDF, ddfInfo);
			keyValue = ddfInfo.m_strMKvalue;
		}
		else//MF
		{
			Param->GetMFInfo(mfInfo);
			keyValue = mfInfo.m_strMKvalue;
		}
		keyLen = DecToHex(keyValue.length() / 2);
	}
	else
	{
		Param->GetKeyInfo(DDF, ADF, KeyName, keyData);
		keyLen = keyData.m_strLen;
		keyValue = keyData.m_strValue;
	}
	if (ErrCheckFlag)
	{
		if (keyValue.length() % 2 != 0 || IsHex(keyValue) == false)
		{
			Set_Error_Flag();
			serr << "装载固定密钥 Write_MKey（" << MF << ", " << DDF << ", " << ADF << ", " << KeyName << ", " << KeyVer << "），指定密钥的值有误。";
			Info->Append_Error(serr.str());
			flag = true;
		}
		if (keyValue.length() != HexToDec(keyLen) * 2)
		{
			Set_Error_Flag();
			serr << "装载固定密钥 Write_MKey（" << MF << ", " << DDF << ", " << ADF << ", " << KeyName << ", " << KeyVer << "），指定密钥的密钥值与密钥长度不相符。";
			Info->Append_Error(serr.str());
			flag = true;
		}
	}
	if (flag)
		return "";
	//指令构建
	sstr.clear();
	sstr.str("");
	sstr << Get_Instruction_Head().substr(0, 2) << Style_InstHead;
	sstr << Get_Instruction_Head().substr(2, 2) << Style_InstHead;
	sstr << Get_Instruction_Head().substr(4, 2) << Style_InstHead;
	sstr << Get_Instruction_Head().substr(6, 2) << Style_InstHead;
	sstr << Get_Instruction_Head().substr(8, 2) << Style_Separator;
	sstr << Get_Instruction_Data() << Style_InstData;
	sstr << setfill('F') << setw(32) << left << ToUpper(keyValue);
	//收尾
	if (RefreshFlag)
		Info->Append_Script(sstr.str());
	return sstr.str();
}

bool COS_T514::Write_Key(string MF, string DDF, string ADF, string KeyName, string KeyVer, bool Encrypt, bool Mac)
{
	bool flag = false;//该函数内的错误标志
	S_MFINFO mfInfo;
	S_DDFINFO ddfInfo;
	S_ADFINFO adfInfo;
	KEYDATAS keyData;
	string keyType;//密钥类型
	string keyID;//密钥标志
	string keyCnt;//错误计数器
	string keyByte6;//密钥头的第六字节，PIN：对应PUK的安全级别，其他密钥表示算法标志
	string keyLevel;//后续安全级别
	string keyLen;//密钥长度
	string keyValue;//密钥值，仅在安装主控时会使用到
	Initialization();
	//参数检查
	if (ErrCheckFlag)
	{
		if (MF != "MF" && MF != "3F00" && MF != "")
		{
			Set_Error_Flag();
			serr << "装载密钥 Write_Key（" << MF << ", " << DDF << ", " << ADF << ", " << KeyName << ", " << KeyVer << "），MF只可取值为“MF”、“3F00”或空串“”，而当前是“" << MF << "”。";
			Info->Append_Error(serr.str());
			flag = true;
		}
		if (!ADF.empty() && (!IsHex(ADF) || ADF.length() != 4))
		{
			Set_Error_Flag();
			serr << "装载密钥 Write_Key（" << MF << ", " << DDF << ", " << ADF << ", " << KeyName << ", " << KeyVer << "），参数ADF必须由4个十六进制字符组成，而当前为“" << ADF << "”。";
			Info->Append_Error(serr.str());
			flag = true;
		}
		if (KeyVer.length() != 2 || IsHex(KeyVer) == false)
		{
			Set_Error_Flag();
			serr << "装载密钥 Write_Key（" << MF << ", " << DDF << ", " << ADF << ", " << KeyName << ", " << KeyVer << "），密钥版本必须由两位十六进制字符组成，而当前为“" << KeyVer << "”。";
			Info->Append_Error(serr.str());
			flag = true;
		}
	}
	//获取配置
	if (KeyName.compare(0, 2, "MK") == 0)//安装主控
	{
		keyType = "00";
		keyID = "00";
		if (ErrCheckFlag && KeyVer != "00")
		{
			serr.clear();
			serr.str("");
			serr << "装载主控密钥 Write_MKey（" << MF << ", " << DDF << ", " << ADF << ", " << KeyName << ", " << KeyVer << "），装载主控密钥时，密钥版本应该为“00”，而当前是“" << KeyVer << "”。";
			Info->Append_Warning(serr.str());
		}
		keyByte6 = "00";
		keyLevel = MK_Level;
		if (ADF.empty() == false)//ADF
		{
			if (Param->GetADFInfo(DDF, ADF, adfInfo) == false)
			{
				Set_Error_Flag();
				serr << "装载主控密钥 Write_MKey（" << MF << ", " << DDF << ", " << ADF << ", " << KeyName << ", " << KeyVer << "），找不到指定的ADF文件。";
				Info->Append_Error(serr.str());
				flag = true;
			}
			keyCnt = adfInfo.m_strCount;
			keyValue = adfInfo.m_strMKvalue;
		}
		else if (DDF.empty() == false)//DDF
		{
			if (Param->GetDDFInfo(DDF, ddfInfo) == false)
			{
				Set_Error_Flag();
				serr << "装载主控密钥 Write_MKey（" << MF << ", " << DDF << ", " << ADF << ", " << KeyName << ", " << KeyVer << "），找不到指定的DDF文件。";
				Info->Append_Error(serr.str());
				flag = true;
			}
			keyCnt = ddfInfo.m_strCount;
			keyValue = ddfInfo.m_strMKvalue;
		}
		else//MF
		{
			Param->GetMFInfo(mfInfo);
			keyCnt = mfInfo.m_strCount;
			keyValue = mfInfo.m_strMKvalue;
		}
		if (ErrCheckFlag)
		{
			if (keyValue.length() % 2 != 0 || IsHex(keyValue) == false)
			{
				Set_Error_Flag();
				serr << "装载主控密钥 Write_MKey（" << MF << ", " << DDF << ", " << ADF << ", " << KeyName << ", " << KeyVer << "），指定目录的主控密钥值有误。";
				Info->Append_Error(serr.str());
				flag = true;
			}
			if (keyValue.length() != 32)
			{
				serr.clear();
				serr.str("");
				serr << "装载主控密钥 Write_MKey（" << MF << ", " << DDF << ", " << ADF << ", " << KeyName << ", " << KeyVer << "），指定目录的主控密钥值长度不是16个字节。";
				Info->Append_Warning(serr.str());
			}
		}
		keyLen = DecToHex(keyValue.length() / 2);
	}
	else
	{
		if (Param->GetKeyInfo(DDF, ADF, KeyName, keyData) == false)
		{
			Set_Error_Flag();
			serr << "装载密钥 Write_Key（" << MF << ", " << DDF << ", " << ADF << ", " << KeyName << ", " << KeyVer << "），找不到指定的密钥。";
			Info->Append_Error(serr.str());
			flag = true;
		}
		keyCnt = keyData.m_strCount;
		switch (HexToDec(keyData.m_strLen))
		{
		case 8:
			keyByte6 = "01";//DES算法
			break;
		case 0x10:
			keyByte6 = "00";//3DES
			break;
		default:
			if (keyData.m_strName == "PIN")
				break;
			Set_Error_Flag();
			serr << "装载密钥 Write_Key（" << MF << ", " << DDF << ", " << ADF << ", " << KeyName << ", " << KeyVer << "），密钥长度只能是0x08个字节或0x10个字节，而当前是“" << keyData.m_strLen << "”。";
			Info->Append_Error(serr.str());
			flag = true;
			break;
		}
		keyLevel = Key_Security_Level(keyData);
		keyLen = keyData.m_strLen;
		//密钥标志，对形如DLK、DPK的密钥做特殊处理
		if (keyData.m_strKeyIndex.find('|') == string::npos)
			keyID = keyData.m_strKeyIndex;
		else
		{
			string::size_type pos = 0, s = 0, e;
			for (int i = HexToDec(KeyVer); i > 0; --i)
			{
				s = pos;
				e = keyData.m_strKeyIndex.find('|', pos);
				if (e == string::npos)
				{
					Set_Error_Flag();
					serr << "装载密钥 Write_Key（" << MF << ", " << DDF << ", " << ADF << ", " << KeyName << ", " << KeyVer << "），在密钥标志“" << keyData.m_strKeyIndex << "”集合中，找不到密钥版本“" << KeyVer << "”所指定的第 " << HexToDec(KeyVer) << " 个标志(注意：在密钥标志集合中，最后必须以“|”结尾)。";
					Info->Append_Error(serr.str());
					flag = true;
					break;
				}
				pos = e + 1;
			}
			if (HexToDec(KeyVer) == 0)
			{
				Set_Error_Flag();
				serr << "装载密钥 Write_Key（" << MF << ", " << DDF << ", " << ADF << ", " << KeyName << ", " << KeyVer << "），当密钥标志含有多个时“" << keyData.m_strKeyIndex << "”，密钥版本“" << KeyVer << "”不能为00。";
				Info->Append_Error(serr.str());
				flag = true;
				keyID == "00";
			}
			else if (e == string::npos)
				keyID == "00";
			else
				keyID = keyData.m_strKeyIndex.substr(s, e - s);
		}
		//密钥类型
		if (keyData.m_strName == "IRK")
			keyType = "0E";
		else if (keyData.m_strName == "PIN")
		{
			keyType = "01";
			keyID = "00";
			if (Param->GetKeyInfo(DDF, ADF, "PUK", keyData) == false)
			{
				Set_Error_Flag();
				serr << "装载密钥 Write_Key（" << MF << ", " << DDF << ", " << ADF << ", " << KeyName << ", " << KeyVer << "），安装PIN时在同一目录路径下找不到密钥PUK。";
				Info->Append_Error(serr.str());
				flag = true;
			}
			keyByte6 = Key_Security_Level(keyData);
		}
		else if (keyData.m_strName == "DLK")
			keyType = "08";
		else if (keyData.m_strName == "DPK")
			keyType = "09";
		else if (keyData.m_strName == "DTK")
			keyType = "0A";
		else if (keyData.m_strName.compare(0, 3, "STK") == 0)
			keyType = "07";
		else//其他密钥都当作是外部认证密钥
			keyType = "0D";
	}
	//配置检查
	if (ErrCheckFlag)
	{
		if (keyID.length() != 2 || IsHex(keyID) == false)
		{
			Set_Error_Flag();
			serr << "装载密钥 Write_Key（" << MF << ", " << DDF << ", " << ADF << ", " << KeyName << ", " << KeyVer << "），密钥标志必须由两位十六进制字符组成，而当前为“" << keyID << "”。";
			Info->Append_Error(serr.str());
			flag = true;
		}
		if (keyCnt.length() != 2 || IsHex(keyCnt) == false)
		{
			Set_Error_Flag();
			serr << "装载密钥 Write_Key（" << MF << ", " << DDF << ", " << ADF << ", " << KeyName << ", " << KeyVer << "），错误计数器必须由两位十六进制字符组成，而当前为“" << keyCnt << "”。";
			Info->Append_Error(serr.str());
			flag = true;
		}
		if (KeyName != "PIN" && keyLen != "08" && keyLen != "10")
		{
			Set_Error_Flag();
			serr << "装载密钥 Write_Key（" << MF << ", " << DDF << ", " << ADF << ", " << KeyName << ", " << KeyVer << "），密钥长度必须为“08”或者“10”，而当前为“" << keyLen << "”。";
			Info->Append_Error(serr.str());
			flag = true;
		}
	}
	if (flag)
		return false;
	//构建指令头部
	sstr << (Encrypt ? "8" : "0");
	sstr << (Mac ? "4" : "0") << Style_InstHead;
	sstr << "D4" << Style_InstHead;
	sstr << ((KeyName.compare(0, 2, "MK")) ? "00" : "01") << Style_InstHead;
	sstr << "00" << Style_InstHead;
	sstr << setfill('0') << setw(2) << hex << uppercase << right << (Encrypt ? 0x20 : 0x18) + (Mac ? 4 : 0) << Style_Separator;
	strcpy(InstHead, StrTrim(sstr.str()).c_str());
	//构建密钥头部
	sstr.clear();
	sstr.str("");
	sstr << keyType;
	sstr << keyID;
	sstr << KeyVer;
	sstr << "EF";
	sstr << keyCnt;
	sstr << keyByte6;
	sstr << keyLevel;
	sstr << setfill('0') << setw(2) << right << keyLen;
	strcpy(InstData, StrTrim(sstr.str()).c_str());
	return true;
}

string COS_T514::FCI_Record(int RecordNo, string MF, string DDF, string ADF, bool RefreshFlag /*= true*/ )
{
	bool flag = false;//该函数内的错误标志
	S_MFINFO mfInfo;
	S_DDFINFO ddfInfo;
	S_ADFINFO adfInfo;
	string fid;
	string aid;
	Initialization();
	//参数检查
	if (ErrCheckFlag)
	{
		if (MF != "MF" && MF != "3F00" && MF != "")
		{
			Set_Error_Flag();
			serr << "添加FCI记录 FCI_Record(" << RecordNo << ", " << MF << ", " << DDF << ", " << ADF << ")，MF只可取值为“MF”、“3F00”或空串“”，而当前是“" << MF << "”。";
			Info->Append_Error(serr.str());
			flag = true;
		}
		if (!ADF.empty() && (!IsHex(ADF) || ADF.length() != 4))
		{
			Set_Error_Flag();
			serr << "添加FCI记录 FCI_Record(" << RecordNo << ", " << MF << ", " << DDF << ", " << ADF << ")，参数ADF必须由4个十六进制字符组成，而当前为“" << ADF << "”。";
			Info->Append_Error(serr.str());
			flag = true;
		}
		if (RecordNo < 0)
		{
			serr.clear();
			serr.str("");
			serr << "添加FCI记录 FCI_Record(" << RecordNo << ", " << MF << ", " << DDF << ", " << ADF << ")，记录号小于0。";
			Info->Append_Warning(serr.str());
		}
		if (flag)
			return "";
	}
	//获取配置
	if (ADF.empty() == false)
	{
		if (Param->GetADFInfo(DDF, ADF, adfInfo) == false)
		{
			Set_Error_Flag();
			serr << "添加FCI记录 FCI_Record(" << RecordNo << ", " << MF << ", " << DDF << ", " << ADF << ")，在配置文件内找不到指定路径的ADF文件。";
			Info->Append_Error(serr.str());
			flag = true;
		}
		fid = adfInfo.m_strFID;
		aid = adfInfo.m_strAID;
	}
	else if (DDF.empty() == false)
	{
		if (Param->GetDDFInfo(DDF, ddfInfo) == false)
		{
			Set_Error_Flag();
			serr << "添加FCI记录 FCI_Record(" << RecordNo << ", " << MF << ", " << DDF << ", " << ADF << ")，在配置文件内找不到指定路径的DDF文件。";
			Info->Append_Error(serr.str());
			flag = true;
		}
		fid = ddfInfo.m_strFID;
		aid = ddfInfo.m_strAID;
	}
	else
	{
		Param->GetMFInfo(mfInfo);
		fid = mfInfo.m_strFID;
		aid = mfInfo.m_strAID;
	}
	//配置检查
	if (ErrCheckFlag)
	{
		if (fid.length() != 4 || !IsHex(fid))
		{
			Set_Error_Flag();
			serr << "添加FCI记录 FCI_Record(" << RecordNo << ", " << MF << ", " << DDF << ", " << ADF << ")，指定目录文件的FID(" << fid << ")格式有误。";
			Info->Append_Error(serr.str());
			flag = true;
		}
		if (aid.length() % 2 != 0 || !IsHex(aid))
		{
			Set_Error_Flag();
			serr << "添加FCI记录 FCI_Record(" << RecordNo << ", " << MF << ", " << DDF << ", " << ADF << ")，指定目录文件的AID(" << fid << ")长度有误或含非法字符。";
			Info->Append_Error(serr.str());
			flag = true;
		}
	}
	if (flag)
		return "";
	//指令构建
	sstr << "00" << Style_InstHead;
	sstr << "D2" << Style_InstHead;
	if (RecordNo <= 0)
		sstr << "00" << Style_InstHead;
	else
		sstr << setfill('0') << setw(2) << hex << uppercase << right << RecordNo;
	sstr << FCI_SFI << Style_InstHead;
	sstr << setfill('0') << setw(2) << hex << uppercase << right << (ADF.empty() ? 14 : 9) + aid.length() / 2 << Style_Separator;
	sstr << "70";
	sstr << setfill('0') << setw(2) << hex << uppercase << right << (ADF.empty() ? 12 : 7) + aid.length() / 2 << Style_InstData;
	sstr << ToUpper(fid) << Style_InstData;
	sstr << setfill('0') << setw(2) << hex << uppercase << right << (ADF.empty() ? 9 : 4) + aid.length() / 2;
	sstr << "6F";
	sstr << setfill('0') << setw(2) << hex << uppercase << right << (ADF.empty() ? 7 : 2) + aid.length() / 2;
	sstr << "84";
	sstr << setfill('0') << setw(2) << hex << uppercase << right << aid.length() / 2 << Style_InstData;
	sstr << ToUpper(aid);
	if (ADF.empty())//不是ADF目录，则要添加目录入口信息
	{
		sstr << Style_InstData;
		sstr << "A5";
		sstr << "03";
		sstr << "88";
		sstr << "01";
		sstr << DIR_SFI;
	}
	strcpy(InstHead, StrTrim(sstr.str()).substr(0, 10).c_str());
	strcpy(InstData, StrTrim(sstr.str()).erase(0, 10).c_str());
	//收尾
	if (RefreshFlag)
		Info->Append_Script(sstr.str());
	return sstr.str();
}

/**************************************
功能：计算指定密钥的安全级别
参数：
IN：KEYInfo：已获取到的密钥信息结构体
返回：
输出由2个字符组成的安全级别，如果错误则输出空串
注意：不同的COS有不一样的安全级别编码方式
**************************************/
string COS_T514::Key_Security_Level(KEYDATAS KEYInfo)
{
	bool flag = false;//该函数内的错误标志
	//检查配置
	if (KEYInfo.m_nSeq < 0 || KEYInfo.m_nSeq > 0xF)
	{
		serr.clear();
		serr.str("");
		serr << "计算密钥安全级别 Key_Security_Level，指定密钥" << KEYInfo.m_strName << "的顺序号（" << KEYInfo.m_nSeq << "）不在允许范围0-F内。请检查该目录位置下的密钥条数是否过多。超出部分将自动使用一致的安全级别。";
		Info->Append_Warning(serr.str());
		flag = true;
	}
	serr.clear();
	serr.str("");
	if (flag)
		serr << "EE";
	else
		serr << hex << uppercase << KEYInfo.m_nSeq << hex << uppercase << KEYInfo.m_nSeq;
	return serr.str();
}


string COS_T514::Update_ATR( string strATRHead, string strCardSN, bool RefreshFlag /*= true*/ )
{
	bool flag = false;//该函数内的错误标志
	Initialization();
	//参数检查
	if (ErrCheckFlag)
	{
		if (!IsHex(strATRHead) || strATRHead.length() != 22)
		{
			Set_Error_Flag();
			serr << "更新ATR指令 Update_ATR，参数strATRHead必须由22个十六进制字符组成，而当前为“" << strATRHead << "”。";
			Info->Append_Error(serr.str());
			flag = true;
		}
		if (!IsHex(strCardSN) || strCardSN.length() != 12)
		{
			Set_Error_Flag();
			serr << "更新ATR指令 Update_ATR，参数strCardSN必须由12个十六进制字符组成，而当前为“" << strCardSN << "”。";
			Info->Append_Error(serr.str());
			flag = true;
		}
	}
	if (flag)
		return "";
	//指令构建
	sstr << "80" << Style_InstHead;
	sstr << "E0" << Style_InstHead;
	sstr << "05" << Style_InstHead;
	sstr << "00" << Style_InstHead;
	sstr << setfill('0') << setw(2) << left << DecToHex(2+strCardSN.length()/2) << Style_Separator;
	sstr << "0000" << strCardSN << endl;

	sstr << "00" << Style_InstHead;
	sstr << "14" << Style_InstHead;
	sstr << "00" << Style_InstHead;
	sstr << "00" << Style_InstHead;
	sstr << setfill('0') << setw(2) << left << DecToHex(strATRHead.length()/2) << Style_Separator;
	sstr << strATRHead;
	//收尾
	if (RefreshFlag)
		Info->Append_Script(sstr.str());
	return sstr.str();
}