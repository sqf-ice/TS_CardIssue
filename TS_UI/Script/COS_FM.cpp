//#####################################
//
//	复旦 COS指令
//
//	复旦COS含有社保与非社保两套个人化指令，目前定义：开始建卡时采用非社保形式，第一次创建DDF就改为写为true
//
//	最后修改日期：2015-11-09 14:34
//#####################################

#include "chip.h"
#include "tool.h"
#include <iomanip>

const string COS_FM::MK_Level = "FF";//非社保应用，主控密钥的安全级别
const string COS_FM::Key_Forbidden = "EF";//非社保应用，禁止权限
const string COS_FM::Key_Free = "00";//非社保应用，访问自由
const string COS_FM::SSSE_MK_Level = "40";//社保应用，主控密钥的安全级别
const string COS_FM::SSSE_Key_Forbidden = "08";//社保应用，禁止权限
const string COS_FM::SSSE_Key_Free = "00";//社保应用，访问自由
const int COS_FM::FileExtSpace = 0x08;//EF文件的扩展空间




COS_FM::COS_FM(CInformation *inf, CParameter *para):CInstruction(inf, para)
{
	SSSE_Flag = false;
}

string COS_FM::Delete_File(int WorkFlag, string FType /*= ""*/, string FID /*= ""*/, bool RefreshFlag /*= true*/)
{
	return "";
}

string COS_FM::Delete_File(string MF, string DDF, string ADF, string EF, bool RefreshFlag /*= true*/)
{
	return "";
}


string COS_FM::Verify_TranCode(int Len, string Data, bool RefreshFlag /*= true*/)
{
	return "";
}

string COS_FM::End_PreIssue(bool RefreshFlag /*= true*/)
{
	return "";
}

string COS_FM::Create_File(string MF /*= ""*/, string DDF /*= ""*/, string ADF /*= ""*/, string EF /*= ""*/, bool RefreshFlag /*= true*/)
{
	//bool flag = false;//该函数内的错误标志
	//bool extflag = false;//标志着创建EF文件时的写权限是否使用了扩展后续安全状态字节
	//bool efFlag = true;//创建定长记录文件有效，该值为true时表示定长记录文件含有Tag、Length字节
	//S_MFINFO mfInfo;
	//S_DDFINFO ddfInfo;
	//S_ADFINFO adfInfo;
	//S_EFINFO efInfo;
	//KEYDATAS keyData1;
	//KEYDATAS keyData2;
	//EFDATAS efData;//创建定长记录文件时检查各记录长度及判断有误记录标识符
	//string efSpace;//EF文件空间
	//string efType;//EF文件类型
	//string efByte4 = "";//第4字节，一般是读权限
	//string efByte5 = "";//第5字节，一般是写权限
	//string efByte6 = "";//第6字节，扩展安全状态字节
	//string efByte7 = "";//第7字节，线路保护
	//int i;//计数变量
	//string str;//临时变量
	//Initialization();
	////参数检查
	//if (ErrCheckFlag)
	//{
	//	if (MF != "MF" && MF != "3F00" && MF != "")
	//	{
	//		Set_Error_Flag();
	//		serr << "创建文件指令 Create_File（" << MF << ", " << DDF << ", " << ADF << ", " << EF << "），MF只可取值为“MF”、“3F00”或空串“”，而当前是“" << MF << "”。";
	//		Info->Append_Error(serr.str());
	//		flag = true;
	//	}
	//	if (!ADF.empty() && (!IsHex(ADF) || ADF.length() != 4))
	//	{
	//		Set_Error_Flag();
	//		serr << "创建文件指令 Create_File（" << MF << ", " << DDF << ", " << ADF << ", " << EF << "），参数ADF必须由4个十六进制字符组成，而当前为“" << ADF << "”。";
	//		Info->Append_Error(serr.str());
	//		flag = true;
	//	}
	//	if (!EF.empty() && (!IsHex(EF) || EF.length() != 4))
	//	{
	//		Set_Error_Flag();
	//		serr << "创建文件指令 Create_File（" << MF << ", " << DDF << ", " << ADF << ", " << EF << "），参数EF必须由4个十六进制字符组成，而当前为“" << EF << "”。";
	//		Info->Append_Error(serr.str());
	//		flag = true;
	//	}
	//}
	////构建指令头部
	//sstr << "80" << Style_InstHead;
	//sstr << "E0" << Style_InstHead;
	//sstr << "01" << Style_InstHead;
	//sstr << "00" << Style_InstHead;
	////获取配置 并 构建指令
	//if (EF.empty() == false)//EF
	//{
	//	if (Param->GetEFInfo(DDF, ADF, EF, efInfo) == false)
	//	{
	//		Set_Error_Flag();
	//		serr << "创建文件指令 Create_File（" << MF << ", " << DDF << ", " << ADF << ", " << EF << "），找不到指定路径的EF文件。";
	//		Info->Append_Error(serr.str());
	//		flag = true;
	//	}
	//	//EF - 配置检查
	//	if (ErrCheckFlag)
	//	{
	//		if (efInfo.m_strSFI.length() % 2 != 0 || !IsHex(efInfo.m_strSFI))
	//		{
	//			Set_Error_Flag();
	//			serr << "创建文件指令 Create_File（" << MF << ", " << DDF << ", " << ADF << ", " << EF << "），指定EF文件的FID“" << efInfo.m_strSFI << "”有误。";
	//			Info->Append_Error(serr.str());
	//			flag = true;
	//		}
	//	}
	//	//EF - 文件类型
	//	if (efInfo.m_strEFType == EFTYPE_FIXED_LENGTH)//定长记录
	//	{
	//		if (ErrCheckFlag)
	//		{
	//			if (efInfo.m_nRecordCount == 0)
	//			{
	//				Set_Error_Flag();
	//				serr << "创建文件指令 Create_File（" << MF << ", " << DDF << ", " << ADF << ", " << EF << "），创建定长记录文件时，要求至少有一条记录来指定定长文件的记录长度，而当前所需创建的文件中并没有任何记录信息。";
	//				Info->Append_Error(serr.str());
	//				flag = true;
	//			}
	//			if (efInfo.m_nRecordCount > 0 && HexToDec(efInfo.m_strEFLen) % efInfo.m_nRecordCount != 0)
	//			{
	//				Set_Error_Flag();
	//				serr << "创建文件指令 Create_File（" << MF << ", " << DDF << ", " << ADF << ", " << EF << "），要求定长记录文件的记录长度之和“0x" << efInfo.m_strEFLen << "”必须是记录数目“" << efInfo.m_nRecordCount << "”的倍数。";
	//				Info->Append_Error(serr.str());
	//				flag = true;
	//			}
	//			if (efInfo.m_nRecordCount > 0 && HexToDec(efInfo.m_strEFLen) / efInfo.m_nRecordCount > 0xFF)
	//			{
	//				Set_Error_Flag();
	//				serr << "创建文件指令 Create_File（" << MF << ", " << DDF << ", " << ADF << ", " << EF << "），单条记录长度超出了0xFF的大小。";
	//				Info->Append_Error(serr.str());
	//				flag = true;
	//			}
	//			for (i = 1; i <= efInfo.m_nRecordCount; ++i)
	//			{
	//				if (Param->GetEFData(DDF, ADF, EF, i, efData) == false)
	//				{
	//					serr.clear();
	//					serr.str("");
	//					serr << "创建文件指令 Create_File（" << MF << ", " << DDF << ", " << ADF << ", " << EF << "），该EF文件指定该含有 " << efInfo.m_nRecordCount << "条记录，但却找不到第 " << i << "条记录。";
	//					Info->Append_Warning(serr.str());
	//					break;
	//				}
	//				if ((efInfo.m_nRecordCount > 0) && (HexToDec(efInfo.m_strEFLen) / efInfo.m_nRecordCount != HexToDec(efData.m_strLen)))
	//				{
	//					Set_Error_Flag();
	//					serr << "创建文件指令 Create_File（" << MF << ", " << DDF << ", " << ADF << ", " << EF << "），指定的定长记录文件各记录长度不相等。";
	//					Info->Append_Error(serr.str());
	//					flag = true;
	//					break;
	//				}
	//				if (i == 1)
	//				{
	//					if (efData.m_strTag == "-" || efData.m_strTag.empty())
	//						efFlag = false;
	//					else
	//						efFlag = true;
	//				}
	//				else
	//				{
	//					if (((efData.m_strTag == "-" || efData.m_strTag.empty()) && efFlag) || (!(efData.m_strTag == "-" || efData.m_strTag.empty()) && !efFlag))
	//					{
	//						Set_Error_Flag();
	//						serr << "创建文件指令 Create_File（" << MF << ", " << DDF << ", " << ADF << ", " << EF << "），指定的定长记录文件中，有些记录含有记录标识符，有些却没有。";
	//						Info->Append_Error(serr.str());
	//						flag = true;
	//						break;
	//					}
	//				}
	//			}
	//		}
	//		efType = "2A";
	//		if (efInfo.m_nRecordCount > 0)
	//		{
	//			serr.clear();
	//			serr.str("");
	//			serr << setfill('0') << setw(2) << right << efInfo.m_nRecordCount;
	//			serr << setfill('0') << setw(2) << right << DecToHex(HexToDec(efInfo.m_strEFLen) / efInfo.m_nRecordCount + (efFlag ? 2 : 0));
	//			efSpace = serr.str();
	//		}
	//		else
	//			efSpace = "0000";
	//	}
	//	else if (efInfo.m_strEFType == EFTYPE_VARIABLE_LENGTH)//变长记录
	//	{
	//		efSpace = DecToHex(HexToDec(efInfo.m_strEFLen) + HexToDec(efInfo.m_strResSpace) + efInfo.m_nRecordCount * 2 + FileExtSpace);
	//		efType = "2C";
	//		if (ErrCheckFlag)
	//		{
	//			if (efSpace.length() > 4)
	//			{
	//				Set_Error_Flag();
	//				serr << "创建文件指令 Create_File（" << MF << ", " << DDF << ", " << ADF << ", " << EF << "），文件空间超长（文件内容“0x" << efInfo.m_strEFLen << "” + TagLength空间“" << efInfo.m_nRecordCount << "*2” + 预留空间“0x" << efInfo.m_strResSpace << "” + 2）。";
	//				Info->Append_Error(serr.str());
	//				flag = true;
	//			}
	//		}
	//	}
	//	else if (efInfo.m_strEFType == EFTYPE_BINARY)//透明文件
	//	{
	//		efSpace = DecToHex(HexToDec(efInfo.m_strEFLen) + HexToDec(efInfo.m_strResSpace));//透明文件不加扩展空间
	//		efType = "28";
	//		if (ErrCheckFlag)
	//		{
	//			if (efSpace.length() > 4)
	//			{
	//				Set_Error_Flag();
	//				serr << "创建文件指令 Create_File（" << MF << ", " << DDF << ", " << ADF << ", " << EF << "），文件空间超长（文件内容“0x" << efInfo.m_strEFLen << "” + 预留空间“0x" << efInfo.m_strResSpace << "”）。";
	//				Info->Append_Error(serr.str());
	//				flag = true;
	//			}
	//		}
	//	}
	//	else if (efInfo.m_strEFType == EFTYPE_LOOP)//循环文件
	//	{
	//		serr.clear();
	//		serr.str("");
	//		serr << setfill('0') << setw(2) << right << efInfo.m_strnCount;
	//		serr << setfill('0') << setw(2) << right << efInfo.m_strEFLen;
	//		efSpace = serr.str();
	//		efType = "2E";
	//		if (ErrCheckFlag)
	//		{
	//			if (HexToDec(efInfo.m_strnCount) == 0)
	//			{
	//				Set_Error_Flag();
	//				serr << "创建文件指令 Create_File（" << MF << ", " << DDF << ", " << ADF << ", " << EF << "），对于循环文件并没有指定最少的记录条数。";
	//				Info->Append_Error(serr.str());
	//				flag = true;
	//			}
	//			if (HexToDec(efInfo.m_strEFLen) == 0)
	//			{
	//				Set_Error_Flag();
	//				serr << "创建文件指令 Create_File（" << MF << ", " << DDF << ", " << ADF << ", " << EF << "），指定需创建的循环文件中并没有记录信息。";
	//				Info->Append_Error(serr.str());
	//				flag = true;
	//			}
	//		}
	//	}
	//	else//其他
	//	{
	//		Set_Error_Flag();
	//		serr << "创建文件指令 Create_File（" << MF << ", " << DDF << ", " << ADF << ", " << EF << "），EF文件的类型“" << efInfo.m_strEFType << "”不正确。";
	//		Info->Append_Error(serr.str());
	//		flag = true;
	//	}
	//	//EF - 读控制密钥
	//	switch (EF_Control(DDF, ADF, efInfo.m_strReadControl, keyData1, keyData2, str))
	//	{
	//	case 1://自由
	//		efByte4 = SSSE_Flag ? SSSE_Key_Free : Key_Free;
	//		break;
	//	case 2://禁止
	//		efByte4 = SSSE_Flag ? SSSE_Key_Forbidden : Key_Forbidden;
	//		break;
	//	case 3://上级目录PIN
	//		efByte4 = SSSE_Flag ? "88" : Key_Security_Level(keyData1);
	//		break;
	//	case 4://当前目录密钥
	//		if (SSSE_Flag)
	//		{
	//			str = Key_Security_Level(keyData1);
	//			if (HexToDec(str) > 0x80)//使用扩展安全状态字节
	//			{
	//				Set_Error_Flag();
	//				serr << "创建文件指令 Create_File（" << MF << ", " << DDF << ", " << ADF << ", " << EF << "），文件读权限密钥的对应后续安全状态不应该使用扩展安全状态字节。";
	//				Info->Append_Error(serr.str());
	//				flag = true;
	//				efByte4 = SSSE_Key_Free;
	//			}
	//			else
	//				efByte4 = str;
	//		}
	//		else
	//			efByte4 = Key_Security_Level(keyData1);
	//		break;
	//	case 5://上级及当前目录密钥“与”逻辑
	//		if (SSSE_Flag)
	//		{
	//			if (keyData1.m_strName != "PIN")
	//			{
	//				Set_Error_Flag();
	//				serr << "创建文件指令 Create_File（" << MF << ", " << DDF << ", " << ADF << ", " << EF << "），读权限的上级目录受控密钥只能为PIN，而当前是“" << keyData1.m_strName << "”。";
	//				Info->Append_Error(serr.str());
	//				flag = true;
	//			}

	//		}
	//		
	//		break;
	//	case 6://上级及当前目录密钥“或”逻辑
	//		efSCW = "0";
	//		readCtrl = Key_Security_Level(keyData1) + Key_Security_Level(keyData2);
	//		break;
	//	default:
	//		Set_Error_Flag();
	//		serr << "创建文件指令 Create_File（" << MF << ", " << DDF << ", " << ADF << ", " << EF << "），读权限状态" << str;
	//		Info->Append_Error(serr.str());
	//		flag = true;
	//		break;
	//	}
	//	//EF - 写控制密钥
	//	switch (EF_Control(DDF, ADF, efInfo.m_strWriteControl, keyData1, keyData2, str))
	//	{
	//	case 1://自由
	//		efSCW += "0";
	//		writeCtrl = Key_Free + Key_Free;
	//		break;
	//	case 2://禁止
	//		efSCW += "0";
	//		writeCtrl = Key_Forbidden + Key_Forbidden;
	//		break;
	//	case 3://上级目录PIN
	//		efSCW = "0";
	//		readCtrl = Key_Security_Level(keyData1) + Key_Free;
	//		break;
	//	case 4://当前目录密钥
	//		efSCW += "0";
	//		writeCtrl = Key_Free + Key_Security_Level(keyData1);
	//		break;
	//	case 5://上级及当前目录密钥“与”逻辑
	//		efSCW += "4";
	//		writeCtrl = Key_Security_Level(keyData1) + Key_Security_Level(keyData2);
	//		break;
	//	case 6://上级及当前目录密钥“或”逻辑
	//		efSCW += "0";
	//		writeCtrl = Key_Security_Level(keyData1) + Key_Security_Level(keyData2);
	//		break;
	//	default:
	//		Set_Error_Flag();
	//		serr << "创建文件指令 Create_File（" << MF << ", " << DDF << ", " << ADF << ", " << EF << "），写权限状态" << str;
	//		Info->Append_Error(serr.str());
	//		flag = true;
	//		break;
	//	}
	//	if (flag)
	//		return "";
	//	//EF - 构建指令
	//	sstr << "10" << Style_Separator;
	//	sstr << "00";
	//	sstr << "00" << "00";
	//	sstr << setfill('0') << setw(4) << right << ToUpper(efSpace) << Style_InstData;
	//	sstr << ToUpper(efInfo.m_strSFI) << Style_InstData;
	//	sstr << efType;
	//	sstr << "00";
	//	sstr << ToUpper(efInfo.m_strSFI).substr(2, 2);
	//	sstr << efSCW;
	//	sstr << setfill('0') << setw(2) << right << efRecordLen << Style_InstData;
	//	sstr << readCtrl << Style_InstData;
	//	sstr << writeCtrl;
	//}
	//else if (ADF.empty() == false)//ADF
	//{
	//	if (Param->GetADFInfo(DDF, ADF, adfInfo) == false)
	//	{
	//		Set_Error_Flag();
	//		serr << "创建文件指令 Create_File（" << MF << ", " << DDF << ", " << ADF << ", " << EF << "），找不到指定路径的ADF文件。";
	//		Info->Append_Error(serr.str());
	//		flag = true;
	//	}
	//	//ADF - 配置检查
	//	if (ErrCheckFlag)
	//	{
	//		if (adfInfo.m_strAID.length() % 2 != 0 || !IsHex(adfInfo.m_strAID))
	//		{
	//			Set_Error_Flag();
	//			serr << "创建文件指令 Create_File（" << MF << ", " << DDF << ", " << ADF << ", " << EF << "），指定ADF文件的AID“" << adfInfo.m_strAID << "”有误。";
	//			Info->Append_Error(serr.str());
	//			flag = true;
	//		}
	//		if (adfInfo.m_strFID.length() != 4 || !IsHex(adfInfo.m_strFID))
	//		{
	//			Set_Error_Flag();
	//			serr << "创建文件指令 Create_File（" << MF << ", " << DDF << ", " << ADF << ", " << EF << "），指定ADF文件的FID“" << adfInfo.m_strFID << "”有误。";
	//			Info->Append_Error(serr.str());
	//			flag = true;
	//		}
	//	}
	//	if (flag)
	//		return "";
	//	//ADF - 构建指令
	//	sstr << setfill('0') << setw(2) << hex << uppercase << right << 0x10 + adfInfo.m_strAID.length() / 2 << Style_Separator;
	//	sstr << "00";
	//	if (Param->GetKeyInfo(DDF, ADF, "LK" + ADF, keyData1))//应用锁密钥的后续安全状态
	//		sstr << Key_Security_Level(keyData1);
	//	else
	//		sstr << "00";
	//	sstr << Key_Free << MK_Level;//创建删除权限
	//	sstr << "01" << Style_InstData;//预个人化和应用阶段都使用DES算法
	//	sstr << ToUpper(adfInfo.m_strFID) << Style_InstData;
	//	sstr << "02";//ADF的文件类型，固定为"02"
	//	sstr << "00" << "00";
	//	sstr << "00";//FCI_SFI
	//	sstr << "00";//安全控制字节
	//	sstr << "00";//声明周期
	//	sstr << "00";//保留项
	//	sstr << "00";//保留项
	//	sstr << "00" << Style_InstData;//保留项
	//	sstr << ToUpper(adfInfo.m_strAID);
	//}
	//else if (DDF.empty() == false)//DDF
	//{
	//	if (Param->GetDDFInfo(DDF, ddfInfo) == false)
	//	{
	//		Set_Error_Flag();
	//		serr << "创建文件指令 Create_File（" << MF << ", " << DDF << ", " << ADF << ", " << EF << "），找不到指定路径的DDF文件。";
	//		Info->Append_Error(serr.str());
	//		flag = true;
	//	}
	//	//DDF - 配置检查
	//	if (ErrCheckFlag)
	//	{
	//		if (ddfInfo.m_strAID.length() % 2 != 0 || !IsHex(ddfInfo.m_strAID))
	//		{
	//			Set_Error_Flag();
	//			serr << "创建文件指令 Create_File（" << MF << ", " << DDF << ", " << ADF << ", " << EF << "），指定DDF文件的AID“" << ddfInfo.m_strAID << "”有误。";
	//			Info->Append_Error(serr.str());
	//			flag = true;
	//		}
	//		if (ddfInfo.m_strFID.length() != 4 || !IsHex(ddfInfo.m_strFID))
	//		{
	//			Set_Error_Flag();
	//			serr << "创建文件指令 Create_File（" << MF << ", " << DDF << ", " << ADF << ", " << EF << "），指定DDF文件的FID“" << ddfInfo.m_strFID << "”有误。";
	//			Info->Append_Error(serr.str());
	//			flag = true;
	//		}
	//	}
	//	if (flag)
	//		return "";
	//	//DDF - 构建指令
	//	sstr << setfill('0') << setw(2) << hex << uppercase << right << 0x10 + ddfInfo.m_strAID.length() / 2 << Style_Separator;
	//	sstr << "00";
	//	if (Param->GetKeyInfo(DDF, ADF, "BK", keyData1) == false)//卡锁密钥
	//	{
	//		if (DDF == "SSSE")
	//		{
	//			Set_Error_Flag();
	//			serr << "创建文件指令 Create_File（" << MF << ", " << DDF << ", " << ADF << ", " << EF << "），创建SSSE时找不到密钥BK。";
	//			Info->Append_Error(serr.str());
	//			flag = true;
	//		}
	//		else
	//			sstr << "00";
	//	}
	//	else
	//		sstr << Key_Security_Level(keyData1);
	//	sstr << Key_Free << MK_Level;//创建删除权限
	//	sstr << "01" << Style_InstData;//预个人化和应用阶段都使用DES算法
	//	sstr << ToUpper(ddfInfo.m_strFID) << Style_InstData;
	//	sstr << "01";
	//	sstr << "00";
	//	sstr << "00";
	//	sstr << FCI_SFI;
	//	sstr << "00";
	//	sstr << "00";
	//	sstr << SF_SFI;
	//	sstr << "00";
	//	sstr << TF_SFI;
	//	sstr << ToUpper(ddfInfo.m_strAID);
	//}
	//else//MF
	//{
	//	Param->GetMFInfo(mfInfo);
	//	//MF - 配置检查
	//	if (ErrCheckFlag)
	//	{
	//		if (mfInfo.m_strAID.length() % 2 != 0 || !JudgeHex(mfInfo.m_strAID))//允许为空
	//		{
	//			Set_Error_Flag();
	//			serr << "创建文件指令 Create_File（" << MF << ", " << DDF << ", " << ADF << ", " << EF << "），指定MF文件的AID“" << mfInfo.m_strAID << "”有误。";
	//			Info->Append_Error(serr.str());
	//			flag = true;
	//		}
	//		if (mfInfo.m_strFID != "3F00")
	//		{
	//			Set_Error_Flag();
	//			serr << "创建文件指令 Create_File（" << MF << ", " << DDF << ", " << ADF << ", " << EF << "），MF文件的FID要求必须是“3F00”，而当前却是“" << mfInfo.m_strFID << "”。";
	//			Info->Append_Error(serr.str());
	//			flag = true;
	//		}
	//	}
	//	if (flag)
	//		return "";
	//	//MF - 构建指令
	//	sstr << setfill('0') << setw(2) << hex << uppercase << right << 0x10 + mfInfo.m_strAID.length() / 2 << Style_Separator;
	//	sstr << "00";
	//	sstr << "00";//卡锁密钥
	//	sstr << "00" << MK_Level;//创建删除权限字
	//	sstr << "01" << Style_InstData;
	//	sstr << ToUpper(mfInfo.m_strFID) << Style_InstData;
	//	sstr << "01";
	//	sstr << "00";
	//	sstr << "01";//社保DDF执行卡锁，同时锁定MF
	//	sstr << FCI_SFI;
	//	sstr << "00";
	//	sstr << "00";
	//	sstr << SF_SFI;
	//	sstr << "00";
	//	sstr << "00" << Style_InstData;
	//	sstr << ToUpper(mfInfo.m_strAID);
	//}
	//strcpy(InstHead, StrTrim(sstr.str()).substr(0, 10).c_str());
	//strcpy(InstData, StrTrim(sstr.str()).erase(0, 10).c_str());
	////收尾
	//if (RefreshFlag)
	//	Info->Append_Script(sstr.str());
	//return sstr.str();
return "";
}

string COS_FM::Write_Key(string MF, string DDF, string ADF, string KeyName, string KeyVer, bool RefreshFlag /*= true*/)
{
	return "";
}

bool COS_FM::Write_Key(string MF, string DDF, string ADF, string KeyName, string KeyVer, bool Encrypt, bool Mac)
{
	return false;
}

string COS_FM::Create_Internal_EF(string FType, string DataLen, bool RefreshFlag /*= true*/)
{
	bool flag = false;//该函数内的错误标志
	bool extflag = false;//标志着创建EF文件时的写权限是否使用了扩展后续安全状态字节
	string SFI = "";//短文件标志符
	string efType = "";//文件类型
	string efSpase = "";//文件空间
	string efByte4 = "";//第4字节，一般是读权限
	string efByte5 = "";//第5字节，一般是写权限
	string efByte6 = "";//第6字节，扩展安全状态字节
	string efByte7 = "";//第7字节，线路保护
	KEYDATAS keyData;
	string str;//临时变量
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
		SFI = SF_SFI;
		efType = "3F";
		efSpase = DecToHex(HexToDec(DataLen) + FileExtSpace);
		efByte4 = "FF";//DF短文件标识符，不知道干嘛的
		efByte5 = SSSE_Flag ? SSSE_MK_Level : MK_Level;//增加权限
		efByte6 = "FF";
		efByte7 = "FF";
	}
	else if (FType == "FCI")
	{
		SFI = FCI_SFI;
		efType = "2C";
		efSpase = DecToHex(HexToDec(DataLen) + FileExtSpace);
		efByte4 = SSSE_Flag ? SSSE_Key_Free : Key_Free;//读自由
		efByte5 = SSSE_Flag ? SSSE_MK_Level : MK_Level;//写控制：当前目录主控
		efByte6 = "FF";
		efByte7 = "FF";
	}
	else if (FType == "ATR")
	{
		Set_Error_Flag();
		serr << "创建内部文件指令 Create_Internal_EF(" << FType << ", " << DataLen << ")，复旦芯片并没有所谓的ATR文件。";
		Info->Append_Error(serr.str());
		flag = true;
	}
	else if (FType == "DIR")
	{
		SFI = FCI_SFI;
		efType = "2C";
		efSpase = DecToHex(HexToDec(DataLen) + FileExtSpace);
		efByte4 = SSSE_Flag ? SSSE_Key_Free : Key_Free;//读自由
		efByte5 = SSSE_Flag ? SSSE_MK_Level : MK_Level;//写控制：当前目录主控
		efByte6 = "FF";
		efByte7 = "FF";
	}
	else if (FType == "TF")
	{
		if (SSSE_Flag == false)
		{
			Set_Error_Flag();
			serr << "创建内部文件指令 Create_Internal_EF(" << FType << ", " << DataLen << ")，创建社保钱包文件时遇到异常，社保钱包文件只能在社保目录下创建，请检查代码中 class CScript 中的建卡流程。";
			Info->Append_Error(serr.str());
			flag = false;
		}
		SFI = TF_SFI;
		efType = "2F";
		efSpase = "0210";
		efByte4 = SSSE_MK_Level;
		if (Param->GetKeyInfo("SSSE", "DF04", "DSK", keyData) == false)
		{
			Set_Error_Flag();
			serr << "创建内部文件指令 Create_Internal_EF(" << FType << ", " << DataLen << ")，在SSSE的DF04下找不到密钥DSK。";
			Info->Append_Error(serr.str());
			flag = true;
		}
		else
		{
			str = Key_Security_Level(keyData);
			if (HexToDec(str) > 0x80)//使用扩展安全状态字节
			{
				efByte5 = "00";
				efByte6 = DecToHex(HexToDec(str) - 0x80);
				extflag = true;
			}
			else
			{
				efByte5 = str;
				efByte6 = "FF";
			}
		}
		efByte7 = "FF";
	}
	if (flag)
		return  "";
	//指令构建
	sstr << "80" << Style_InstHead;
	sstr << "E0" << Style_InstHead;
	sstr << "EF" << Style_InstHead;
	sstr << setfill('0') << setw(2) << right << SFI << Style_InstHead;
	sstr << "07" << Style_Separator;
	sstr << efType << Style_InstData;
	sstr << setfill('0') << setw(4) << right << efSpase << Style_InstData;
	if (extflag)
	{
		sstr << efByte4 << Style_InstData;
		sstr << efByte5 << efByte6 << Style_InstData;
		sstr << efByte7;
	}
	else
	{
		sstr << efByte4 << efByte5 << Style_InstData;
		sstr << efByte6 << efByte7;
	}
	strcpy(InstHead, StrTrim(sstr.str()).substr(0, 10).c_str());
	strcpy(InstData, StrTrim(sstr.str()).erase(0, 10).c_str());
	//收尾
	if (RefreshFlag)
		Info->Append_Script(sstr.str());
	return sstr.str();
}

string COS_FM::FCI_Record(int RecordNo, string MF, string DDF, string ADF, bool RefreshFlag /*= true*/)
{
	return "";
}

/**************************************
功能：计算指定密钥的安全级别
参数：（该方法将会自动获取所需配置参数）
IN：DDF：应用目录，可以取值为应用环境简称（如"SSSE"、"PSE"等）、文件标志符（如"DDF1"、"DDF2"等）或保留为空""，当为空时将表示应用目录不存在
IN：ADF：文件目录，可以取值为文件标识符（如"DF01"、"DF02"等）或保留为空""，当为空时将表示文件目录不存在
IN：KeyName：需计算安全级别的密钥名称，如"IRK"等
返回：
输出由2个字符组成的安全级别，如果错误则输出空串
注意：不同的COS有不一样的安全级别编码方式
描述：
复旦COS含有社保与非社保两套密钥安全级别，此处定义为所有DDF都以社保形式创建，只有MF下的ADF及EF才采用非社保形式
在社保形式中，后续安全状态首字节最高位为“1”则表示作用在扩展安全状态字节上，如“81”表示将扩展安全状态字节上或“01”、“C0”表示在扩展安全状态字节上或“40”
**************************************/
string COS_FM::Key_Security_Level(KEYDATAS KEYInfo)
{
	bool flag = true;//该函数内的错误标志

	serr.clear();
	serr.str("");
	if (SSSE_Flag)//社保形式
	{
		switch (KEYInfo.m_nSeq)
		{
		case 1:
			serr << "01";
			break;
		case 2:
			serr << "02";
			break;
		case 3:
			serr << "04";
			break;
		case 4:
			serr << "10";
			break;
		case 5:
			serr << "20";
			break;
		case 6:
			serr << "40";
			break;
		case 7:
			serr << "81";
			break;
		case 8:
			serr << "82";
			break;
		case 9:
			serr << "84";
			break;
		case 10:
			serr << "88";
			break;
		case 11:
			serr << "90";
			break;
		case 12:
			serr << "A0";
			break;
		case 13:
			serr << "C0";
			break;
		default:
			serr << "00";
			break;
		}
	}
	else//非社保形式
	{
		if (KEYInfo.m_nSeq < 0 || KEYInfo.m_nSeq > 0xF)
		{
			serr.clear();
			serr.str("");
			serr << "计算密钥安全级别 Key_Security_Level，指定密钥" << KEYInfo.m_strName << "的顺序号（" << KEYInfo.m_nSeq << "）不在允许范围0-F内。请检查该目录位置下的密钥条数是否过多。超出部分将自动使用一致的安全级别。";
			Info->Append_Warning(serr.str());
			flag = true;
		}
		if (flag)
			serr << "EE";
		else
			serr << hex << uppercase << KEYInfo.m_nSeq << hex << uppercase << KEYInfo.m_nSeq;
	}
	return serr.str();
}


void COS_FM::Init_Security_Level(string KeyNameIgnore)
{
	struct S_KEYIGNORE
	{
		string KeyName;//需剔除的密钥名称
		int IgnoreType;//匹配方式：1-子串匹配“XXX”、2-左起匹配“[XXX”、3-右端匹配“XXX]”、4-全匹配“[XXX]”
	}keyIgnore;
	vector<S_KEYIGNORE> vecKeyIgnore;//从参数 KeyNameIgnore 分解出来的信息
	vector<S_DDFINFO> vecMFStruct;//各DDF信息
	vector<KEYDATAS> vecKeyDatas;//ADF目录下的密钥
	string::size_type s = 0, e = 0;
	bool matchFlag;//匹配标志，当该值为true时表明当前密钥在KeyNameIgnore信息中
	int cnt;//密钥序号，每个ADF下该序号都将从 1 开始，复旦COS中非写卡密钥“UK”只能有1-6个安全级别
	int rcnt;//密钥序号，每个ADF下该序号都将从 13 开始，此处对应的“扩展后续状态字节”，只有“UK”开头的密钥才会从此处开始，序号依次递减
	

	//分解 KeyNameIgnore
	while (true)
	{
		e = KeyNameIgnore.find('|', s);
		if (e == string::npos)
			break;
		keyIgnore.KeyName = ToUpper(KeyNameIgnore.substr(s, e - s));
		keyIgnore.IgnoreType = 1;
		if (keyIgnore.KeyName.at(0) == '[')
		{
			keyIgnore.KeyName.erase(0, 1);
			keyIgnore.IgnoreType += 1;
		}
		if (keyIgnore.KeyName.at(keyIgnore.KeyName.length() - 1) == ']')
		{
			keyIgnore.KeyName.erase(keyIgnore.KeyName.length() - 1, 1);
			keyIgnore.IgnoreType += 2;
		}
		vecKeyIgnore.push_back(keyIgnore);
		s = e + 1;
	}

	//初始化密钥序号
	Param->GetCardStruct(vecMFStruct);
	for (unsigned int i = 0; i < vecMFStruct.size(); ++i)//遍历DDF
	{
		for (unsigned int j = 0; j < vecMFStruct.at(i).m_vecADFInfo.size(); ++j)//遍历ADF
		{
			vecKeyDatas.clear();
			Param->GetADFKeys(vecMFStruct.at(i).m_strDDFName, vecMFStruct.at(i).m_vecADFInfo.at(j).m_strADFName, vecKeyDatas);
			cnt = 1;
			rcnt = 13;
			for (unsigned int m = 0; m < vecKeyDatas.size(); ++m)//遍历密钥
			{
				for (unsigned int n = 0; n < vecKeyIgnore.size(); ++n)//遍历需剔除的密钥
				{
					matchFlag = false;
					switch (vecKeyIgnore.at(n).IgnoreType)//需剔除的密钥则将序号置为 0
					{
					case 1://子串匹配“XXX”
						if (ToUpper(vecKeyDatas.at(m).m_strName).find(vecKeyIgnore.at(n).KeyName) != string::npos)
							matchFlag = true;
						break;
					case 2://左起匹配“[XXX”
						if (ToUpper(vecKeyDatas.at(m).m_strName).find(vecKeyIgnore.at(n).KeyName, 0) == 0)
							matchFlag = true;
						break;
					case 3://右端匹配“XXX]”
						if (ToUpper(vecKeyDatas.at(m).m_strName).compare(vecKeyDatas.at(m).m_strName.length() - vecKeyIgnore.at(n).KeyName.length(), vecKeyIgnore.at(n).KeyName.length(), vecKeyIgnore.at(n).KeyName) == 0)
							matchFlag = true;
						break;
					case 4://全匹配“[XXX]”
						if (ToUpper(vecKeyDatas.at(m).m_strName) == vecKeyIgnore.at(n).KeyName)
							matchFlag = true;
						break;
					default:
						break;
					}
					if (matchFlag)//匹配成功，跳出循环
						break;
				}
				if (matchFlag)//被剔除的密钥都将置序号为 0
					Param->ModeKeySeq(vecMFStruct.at(i).m_strDDFName, vecMFStruct.at(i).m_vecADFInfo.at(j).m_strADFName, vecKeyDatas.at(m).m_strName, 0);
				else
				{
					if (cnt == 7)
					{
						serr.clear();
						serr.str("");
						serr << "<MF><" << vecMFStruct.at(i).m_strDDFName << "><" << vecMFStruct.at(i).m_vecADFInfo.at(j).m_strADFName << ">下的密钥数目过多，需外部认证且非“UK”开头的写卡密钥最多应该不超过6条（含主控），超出部分将会使用一致的安全级别。";
						Info->Append_Warning(serr.str());
					}
					if (cnt > rcnt)
					{
						serr.clear();
						serr.str("");
						serr << "<MF><" << vecMFStruct.at(i).m_strDDFName << "><" << vecMFStruct.at(i).m_vecADFInfo.at(j).m_strADFName << ">下的密钥数目过多，需外部认证的密钥不应该超过13条（含主控），超出部分将会使用一致的安全级别。";
						Info->Append_Warning(serr.str());
					}
					if (vecKeyDatas.at(m).m_strName.compare(0, 2, "UK") == 0 || vecKeyDatas.at(m).m_strName == "DSK")
						Param->ModeKeySeq(vecMFStruct.at(i).m_strDDFName, vecMFStruct.at(i).m_vecADFInfo.at(j).m_strADFName, vecKeyDatas.at(m).m_strName, (rcnt <= 0) ? 1 : rcnt--);
					else
						Param->ModeKeySeq(vecMFStruct.at(i).m_strDDFName, vecMFStruct.at(i).m_vecADFInfo.at(j).m_strADFName, vecKeyDatas.at(m).m_strName, (cnt > 6) ? 6 : cnt++);
				}
			}
		}
	}
}

string COS_FM::Update_ATR( string strATRHead, string strCardSN, bool RefreshFlag /*= true*/ )
{
	return "";
}
