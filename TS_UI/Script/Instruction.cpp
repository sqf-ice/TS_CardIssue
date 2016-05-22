//#####################################
//
//	class Instruction
//
//	指令类（各芯片类的基类，形成统一对外接口）
//
//	最后修改日期：2015-11-13 11:46
//#####################################

#include "Instruction.h"
#include "tool.h"
#include <iomanip>


CInstruction::CInstruction( CInformation *inf, CParameter *para ):Info(inf),Param(para)
{
	ErrFlag = false;
	ErrCheckFlag = true;
	Style_Separator = "";
	Style_InstHead = "";
	Style_InstData = "";
	sstr.clear();
	sstr.str("");
	serr.clear();
	serr.str("");
	memset(InstHead, 0, sizeof(InstHead));
	memset(InstData, 0, sizeof(InstData));
	Extends = "";
	//获取各内部文件的短文件标识符
	SF_SFI = "80";
	FCI_SFI = "23";
	DIR_SFI = "1E";
	TF_SFI = "81";
	ATR_SFI = "";
	Param->Internal_SFI(SF_SFI, FCI_SFI, DIR_SFI, TF_SFI, ATR_SFI);
	Init_Security_Level("[IRK]|[STK|[DLK]|[DPK]|[DTK]|");
}

CInstruction::~CInstruction()
{ }

void CInstruction::ReSetErrFlag()//重置错误标志
{
	ErrFlag = false;
}

bool CInstruction::Error()//判断错误
{
	return ErrFlag;
}

void CInstruction::Error_Check(bool Swf)//是否打开错误检查
{
	ErrCheckFlag = Swf;
}

void CInstruction::Style_Set(unsigned char Sty)//设置样式
{
	Style_Separator = "";
	Style_InstHead = "";
	Style_InstData = "";
	if (Sty & STYLE_SEPARATOR)
		Style_Separator = " ";
	if (Sty & STYLE_INSTHEAD)
	{
		Style_InstHead = " ";
		Style_Separator += " ";
	}
	if (Sty & STYLE_INSTDATA)
		Style_InstData = " ";
}

string CInstruction::Get_Instruction_Head()//取出指令头部
{
	return StrTrim(InstHead);
}

string CInstruction::Get_Instruction_Data()//取出指令附加信息
{
	return StrTrim(InstData);
}

void CInstruction::Extend_Param( string Ext )//设置扩展参数
{
	Extends = Ext;
}

void CInstruction::Initialization()//清除缓存记录，进行初始化
{
	sstr.clear();
	sstr.str("");
	serr.clear();
	serr.str("");
	memset(InstHead, 0, sizeof(InstHead));
	memset(InstData, 0, sizeof(InstData));
}

void CInstruction::Set_Error_Flag()//设置错误标志
{
	ErrFlag = true;
	serr.clear();
	serr.str("");
}

bool CInstruction::CheckData(const string &Data, int Len, string &ErrMsg)//检查数据
{
	int cnt;//记录长度
	string str;
	string::size_type pos;
	string::size_type s, e;
	pos = 0;
	cnt = 0;
	while (pos < Data.length())
	{
		s = Data.find("\"", pos);
		//收尾
		if (s == string::npos)
		{
			str = Data.substr(pos, Data.length() - pos);
			if (IsHex(str) == false)
			{
				ErrMsg = "含有非法字符";
				return false;
			}
			cnt += str.length();
			break;
		}
		//CN部分
		str = Data.substr(pos, s - pos);
		if (JudgeHex(str) == false)
		{
			ErrMsg = "含有非法字符";
			return false;
		}
		cnt += str.length();
		//AN部分
		e = Data.find("\"", pos + s + 1);
		if (e == string::npos)
		{
			ErrMsg = "双引号不配对。";
			return false;
		}
		cnt += (e - s - 1) * 2;
		pos = e + 1;
	}
	//长度判断
	if (((Len > 0) && (Len * 2 != cnt)) || ((Len < 0) && (((-Len) * 2 < cnt)) || (cnt % 2 != 0)))
	{
		ErrMsg = "长度不符合要求";
		return false;
	}
	return true;
}

void CInstruction::CON_Extend(string &ConData)//常量处理
{
	string str = ConData;
	if (str.compare(0, 3, "CON") != 0)
		return;
	str.erase(0, 3);
	if (str.empty() || !JudgeDec(str))
		return;
	ConData.insert(0, "[");
	ConData.append("]");
	return;
}

string CInstruction::Get_Internal_FID(string FType)//获取内部文件FID
{
	//参数检查
	if (FType != "SF" && FType != "FCI" && FType != "DIR" && FType != "TF" && FType != "ATR")
	{
		serr.clear();
		serr.str("");
		serr << "获取内部文件FID Get_Internal_FID（" << FType << "），无法识别指定的FID。";
		Info->Append_Error(serr.str());
	}
	if (FType == "SF")
		return "EF" + SF_SFI;
	else if (FType == "FCI")
		return "EF" + FCI_SFI;
	else if (FType == "DIR")
		return "EF" + DIR_SFI;
	else if (FType == "TF")
		return "EF" + TF_SFI;
	else if (FType == "ATR")
		return "EF" + ATR_SFI;
	else
		return "";
}



/***********************************************
	指令方法
***********************************************/

string CInstruction::Get_Challenge(int Len, bool RefreshFlag /*= true*/)//取随机数
{
	Initialization();
	//参数检查
	if (ErrCheckFlag)
	{
		if (Len != 4 && Len != 8 && Len != 16)
		{
			serr.clear();
			serr.str("");
			serr << "取随机数指令 Get_Challenge（" << Len << "），指定生成随机数的长度要求为“0x04”、“0x08”或“0x10”，而当前为“0x" << setfill('0') << setw(2) << uppercase << right << Len << "”。";
			Info->Append_Warning(serr.str());
		}
	}
	//构建指令
	sstr << "00" << Style_InstHead;
	sstr << "84" << Style_InstHead;
	sstr << "00" << Style_InstHead;
	sstr << "00" << Style_InstHead;
	sstr << setfill('0') << setw(2) << hex << uppercase << right << Len;
	strcpy(InstHead, StrTrim(sstr.str()).c_str());
	//收尾
	if (RefreshFlag)
		Info->Append_Script(sstr.str());
	return sstr.str();
}

string CInstruction::Select_File(bool Dir, bool FID, int Len, string Data, bool RefreshFlag /*= true */)//选择文件
{
	bool flag = false;//该函数内的错误标志
	string str;
	string estr;
	Initialization();
	//参数检查
	if (ErrCheckFlag)
	{
		if (FID == true && Len != 2)
		{
			Set_Error_Flag();
			serr << "选择文件指令 Select_File（" << (Dir ? "Dir" : "!Dir") << ", " << (FID ? "Fid" : "!Fid") << ", " << Len << ", " << Data << "），当使用文件标识符时（FID为true），指令数据域长度必须为2（参数Len），而当前为“" << Len << "”。";
			Info->Append_Error(serr.str());
			flag = true;
		}
		if (Len<0 || Len>255)
		{
			Set_Error_Flag();
			serr << "选择文件指令 Select_File（" << (Dir ? "Dir" : "!Dir") << ", " << (FID ? "Fid" : "!Fid") << ", " << Len << ", " << Data << "），指定数据域长度参数的取值范围为0-255，而当前是“" << Len << "”。";
			Info->Append_Error(serr.str());
			flag = true;
		}
		if (Dir == false && FID == false)
		{
			Set_Error_Flag();
			serr << "选择文件指令 Select_File（" << (Dir ? "Dir" : "!Dir") << ", " << (FID ? "Fid" : "!Fid") << ", " << Len << ", " << Data << "），当选择EF时必须将参数FID置为true（文件标识符）。";
			Info->Append_Error(serr.str());
			flag = true;
		}
	}
	//数据域检查
	if (StrDel_ConVar(Data, str) == false)
	{
		if (ErrCheckFlag && !CheckData(str, Len, estr))
		{
			Set_Error_Flag();
			serr << "选择文件指令 Select_File（" << (Dir ? "Dir" : "!Dir") << ", " << (FID ? "Fid" : "!Fid") << ", " << Len << ", " << Data << "），参数Data（" << Data << "）有误，" << estr;
			Info->Append_Error(serr.str());
			flag = true;
		}
	}
	if ( flag )
		return "";
	//构建指令
	sstr << "00" << Style_InstHead;
	sstr << "A4" << Style_InstHead;
	if( Dir )
	{
		if (FID == true)
			sstr << "00" << Style_InstHead;
		else
			sstr << "04" << Style_InstHead;
	}
	else
		sstr << "02" << Style_InstHead;
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

string CInstruction::Select_File(string MF /*= ""*/, string DDF /*= ""*/, string ADF /*= ""*/, string EF /*= ""*/, bool RefreshFlag /*= true */)//选择文件
{
	bool flag = false;//该函数内的错误标志
	string str;
	string estr;
	string Data;//AID或FID
	bool dir;//目录标志
	bool fid;//FID标志
	S_MFINFO mfInfo;
	S_DDFINFO ddfInfo;
	S_ADFINFO adfInfo;
	S_EFINFO efInfo;
	Initialization();
	//参数检查
	if (ErrCheckFlag)
	{
		if (MF != "MF" && MF != "3F00" && MF != "")
		{
			serr.clear();
			serr.str("");
			serr << "选择文件指令 Select_File（" << MF << ", " << DDF << ", " << ADF << ", " << EF << "），MF只可取值为“MF”、“3F00”或空串“”，而当前是“" << MF << "”。";
			Info->Append_Warning(serr.str());
		}
		if (!ADF.empty() && (!IsHex(ADF) || ADF.length() != 4))
		{
			Set_Error_Flag();
			serr << "选择文件指令 Select_File（" << MF << ", " << DDF << ", " << ADF << ", " << EF << "），参数ADF必须由4个十六进制字符组成，而当前为“" << ADF << "”。";
			Info->Append_Error(serr.str());
			flag = true;
		}
		if (!EF.empty() && (!IsHex(EF) || EF.length() != 4))
		{
			Set_Error_Flag();
			serr << "选择文件指令 Select_File（" << MF << ", " << DDF << ", " << ADF << ", " << EF << "），参数EF必须由4个十六进制字符组成，而当前为“" << EF << "”。";
			Info->Append_Error(serr.str());
			flag = true;
		}
	}
	//获取配置
	if (EF.empty() == false)
	{
		dir = false;
		fid = true;
		if (Param->GetEFInfo(DDF, ADF, EF, efInfo) == false)
		{
			Set_Error_Flag();
			serr << "选择文件指令 Select_File（" << MF << ", " << DDF << ", " << ADF << ", " << EF << "），在配置文件内找不到指定路径的EF文件（“" << MF << "”，“" << DDF << "”，“" << ADF << "”，“" << EF << "”）。";
			Info->Append_Error(serr.str());
			flag = true;
		}
		Data = efInfo.m_strSFI;
	}
	else if (ADF.empty() == false)
	{
		dir = true;
		fid = false;
		if (Param->GetADFInfo(DDF, ADF, adfInfo) == false)
		{
			Set_Error_Flag();
			serr << "选择文件指令 Select_File（" << MF << ", " << DDF << ", " << ADF << ", " << EF << "），在配置文件内找不到指定路径的ADF文件（“" << MF << "”，“" << DDF << "”，“" << ADF << "”）。";
			Info->Append_Error(serr.str());
			flag = true;
		}
		Data = adfInfo.m_strAID;
	}
	else if (DDF.empty() == false)
	{
		dir = true;
		fid = false;
		if (Param->GetDDFInfo(DDF, ddfInfo) == false)
		{
			if (DDF == "SSSE")
			{
				Data = "7378312E73682EC9E7BBE1B1A3D5CF";
				serr.clear();
				serr.str("");
				serr << "选择文件指令 Select_File（" << MF << ", " << DDF << ", " << ADF << ", " << EF << "），在配置里面找不到SSSE目录，此处将自动使用默认的AID“7378312E73682EC9E7BBE1B1A3D5CF”。";
				Info->Append_Warning(serr.str());
			}
			else if (DDF == "PSE")
				Data = "315041592E5359532E4444463031";
			else
			{
				Set_Error_Flag();
				serr << "选择文件指令 Select_File（" << MF << ", " << DDF << ", " << ADF << ", " << EF << "），在配置文件内找不到指定路径的DDF文件（“" << MF << "”，“" << DDF << "”）。";
				Info->Append_Error(serr.str());
				flag = true;
			}
		}
		else
			Data = ddfInfo.m_strAID;
	}
	else
	{
		dir = true;
		fid = true;
		Param->GetMFInfo(mfInfo);
		Data = mfInfo.m_strFID;
	}
	//配置检查
	if (StrDel_ConVar(Data, str) == true)
	{
		Set_Error_Flag();
		serr << "选择文件指令 Select_File（" << MF << ", " << DDF << ", " << ADF << ", " << EF << "），指定文件的AID或FID不应该由[CON]常量来指定。";
		Info->Append_Error(serr.str());
		flag = true;
	}
	if (flag)
		return "";
	//指令构建
	return Select_File(dir, fid, Data.length() / 2, Data, RefreshFlag);
}

string CInstruction::External_Authenticate(string KeyID, string Cipher, string Scr, int KeyVer /*= 0*/, bool RefreshFlag /*= true */)//外部认证
{
	bool flag = false;//该函数内的错误标志
	string str;
	string estr;
	Initialization();
	//参数检查
	if (ErrCheckFlag)
	{
		if (!IsHex(KeyID) || KeyID.length() != 2)
		{
			Set_Error_Flag();
			serr << "外部认证指令 External_Authenticate（" << KeyID << ", " << Cipher << ", " << Scr << ", " << KeyVer << "），密钥标识必须由2个十六进制字符组成，而当前为“" << KeyID << "”。";
			Info->Append_Error(serr.str());
			flag = true;
		}
		if (KeyVer<0 || KeyVer>255)
		{
			Set_Error_Flag();
			serr << "外部认证指令 External_Authenticate（" << KeyID << ", " << Cipher << ", " << Scr << ", " << KeyVer << "），密钥版本取值超出了有效范围，当前是“" << KeyVer << "”。";
			Info->Append_Error(serr.str());
			flag = true;
		}
		else if (KeyVer<0 || KeyVer>3)
		{
			serr.clear();
			serr.str("");
			serr << "外部认证指令 External_Authenticate（" << KeyID << ", " << Cipher << ", " << Scr << ", " << KeyVer << "），密钥版本取值范围必须是‘0’-‘3’，而当前是“" << KeyVer << "”。";
			Info->Append_Warning(serr.str());
		}
	}
	//数据域检查及调整
	if (StrDel_ConVar(Cipher, str) == false)
	{
		if (ErrCheckFlag && !CheckData(str, 8, estr))
		{
			Set_Error_Flag();
			serr << "外部认证指令 External_Authenticate（" << KeyID << ", " << Cipher << ", " << Scr << ", " << KeyVer << "），参数Cipher（" << Cipher << "）有误，" << estr;
			Info->Append_Error(serr.str());
			flag = true;
		}
	}
	if (!Scr.empty() && !StrDel_ConVar(Scr, str))
	{
		if (ErrCheckFlag && !CheckData(str, 8, estr))
		{
			Set_Error_Flag();
			serr << "外部认证指令 External_Authenticate（" << KeyID << ", " << Cipher << ", " << Scr << ", " << KeyVer << "），参数Scr（" << Scr << "）有误，" << estr;
			Info->Append_Error(serr.str());
			flag = true;
		}
	}
	if (flag)
		return "";
	//构建指令
	sstr << "00" << Style_InstHead;
	sstr << "82" << Style_InstHead;
	sstr << "00" << Style_InstHead;
	sstr << ToUpper(KeyID) << Style_InstHead;
	sstr << setfill('0') << setw(2) << hex << uppercase << right << 8 + (Scr.empty() ? 0 : 8) + (KeyVer ? 1 : 0) << Style_Separator;
	sstr << ToUpper(Cipher) << Style_InstData;
	sstr << ToUpper(Scr);
	if (KeyVer != 0)
		sstr << Style_InstData << setfill('0') << setw(2) << hex << uppercase << right << KeyVer;
	strcpy(InstHead, StrTrim(sstr.str()).substr(0, 10).c_str());
	strcpy(InstData, StrTrim(sstr.str()).erase(0, 10).c_str());
	//收尾
	if (RefreshFlag)
		Info->Append_Script(sstr.str());
	return sstr.str();
}

string CInstruction::Update_Record(string P1, string P2, int Len, string Data, bool RefreshFlag /*= true */)//修改记录
{
	bool flag = false;//该函数内的错误标志
	string str;
	string estr;
	Initialization();
	//参数检查
	if (ErrCheckFlag)
	{
		if (!IsHex(P1) || P1.length() != 2)
		{
			Set_Error_Flag();
			serr << "修改记录指令 Update_Record（" << P1 << ", " << P2 << ", " << Len << ", " << Data << "），P1必须由2个十六进制字符组成，而当前是“" << P1 << "”。";
			Info->Append_Error(serr.str());
			flag = true;
		}
		if (!IsHex(P2) || P2.length() != 2)
		{
			Set_Error_Flag();
			serr << "修改记录指令 Update_Record（" << P1 << ", " << P2 << ", " << Len << ", " << Data << "），P2必须由2个十六进制字符组成，而当前是“" << P2 << "”。";
			Info->Append_Error(serr.str());
			flag = true;
		}
		if (Len<0 || Len>255)
		{
			Set_Error_Flag();
			serr << "修改记录指令 Update_Record（" << P1 << ", " << P2 << ", " << Len << ", " << Data << "），指定数据域长度参数的取值范围为0-255，而当前是“" << Len << "”。";
			Info->Append_Error(serr.str());
			flag = true;
		}
	}
	//数据域检查及调整
	if (StrDel_ConVar(Data, str) == false)
	{
		if (ErrCheckFlag && !CheckData(str, -Len, estr))
		{
			Set_Error_Flag();
			serr << "修改记录指令 Update_Record（" << P1 << ", " << P2 << ", " << Len << ", " << Data << "），参数Data（" << Data << ")有误，" << estr;
			Info->Append_Error(serr.str());
			flag = true;
		}
	}
	if (flag)
		return "";
	//构建指令
	sstr << "00" << Style_InstHead;
	sstr << "DC" << Style_InstHead;
	sstr << ToUpper(P1) << Style_InstHead;
	sstr << ToUpper(P2) << Style_InstHead;
	sstr << setfill('0') << setw(2) << hex << uppercase << right << Len << Style_Separator;
	sstr << ToUpper(Data);
	strcpy(InstHead, StrTrim(sstr.str()).substr(0, 10).c_str());
	strcpy(InstData, StrTrim(sstr.str()).erase(0, 10).c_str());
	//收尾
	if (RefreshFlag)
		Info->Append_Script(sstr.str());
	return sstr.str();
}

string CInstruction::Update_Record(string MF, string DDF, string ADF, string EF, int RecordNo, bool InitFlag /*= false*/, bool RefreshFlag /*= true */)
{
	bool flag = false;//该函数内的错误标志
	string str;
	string estr;
	S_EFINFO efInfo;
	EFDATAS efData;
	Initialization();
	//参数检查
	if (ErrCheckFlag)
	{
		if (MF != "MF" && MF != "3F00" && MF != "")
		{
			Set_Error_Flag();
			serr << "修改记录指令 Update_Record（" << MF << ", " << DDF << ", " << ADF << ", " << EF << ", " << RecordNo << "），MF只可取值为“MF”、“3F00”或空串“”，而当前是“" << MF << "”。";
			Info->Append_Error(serr.str());
			flag = true;
		}
		if (!ADF.empty() && (!IsHex(ADF) || ADF.length() != 4))
		{
			Set_Error_Flag();
			serr << "修改记录指令 Update_Record（" << MF << ", " << DDF << ", " << ADF << ", " << EF << ", " << RecordNo << "），参数ADF必须由4个十六进制字符组成，而当前为“" << ADF << "”。";
			Info->Append_Error(serr.str());
			flag = true;
		}
		if (!EF.empty() && (!IsHex(EF) || EF.length() != 4))
		{
			Set_Error_Flag();
			serr << "修改记录指令 Update_Record（" << MF << ", " << DDF << ", " << ADF << ", " << EF << ", " << RecordNo << "），参数EF必须由4个十六进制字符组成，而当前为“" << EF << "”。";
			Info->Append_Error(serr.str());
			flag = true;
		}
		if (RecordNo <= 0 || RecordNo > 255)
		{
			Set_Error_Flag();
			serr << "修改记录指令 Update_Record（" << MF << ", " << DDF << ", " << ADF << ", " << EF << ", " << RecordNo << "），记录号参数的取值范围必须为1-255，而当前为“" << RecordNo << "”。";
			Info->Append_Error(serr.str());
			flag = true;
		}
	}
	//获取配置
	if (Param->GetEFInfo(DDF, ADF, EF, efInfo) == false)
	{
		Set_Error_Flag();
		serr << "修改记录指令 Update_Record（" << MF << ", " << DDF << ", " << ADF << ", " << EF << ", " << RecordNo << "），在配置文件内找不到指定路径的EF文件（“" << MF << "”，“" << DDF << "”，“" << ADF << "”，“" << EF << "”）。";
		Info->Append_Error(serr.str());
		flag = true;
	}
	if (efInfo.m_strEFType == EFTYPE_BINARY)
	{
		Set_Error_Flag();
		serr << "修改记录指令 Update_Record（" << MF << ", " << DDF << ", " << ADF << ", " << EF << ", " << RecordNo << "），请使用 Update_Binary 函数来构建写二进制文件。";
		Info->Append_Error(serr.str());
		flag = true;
	}
	if (Param->GetEFData(DDF, ADF, EF, RecordNo, efData) == false)
	{
		Set_Error_Flag();
		serr << "修改记录指令 Update_Record（" << MF << ", " << DDF << ", " << ADF << ", " << EF << ", " << RecordNo << "），在配置文件内找不到指定路径的文件记录。";
		Info->Append_Error(serr.str());
		flag = true;
	}
	CON_Extend(efData.m_strVal);
	//检查从配置中获取到的参数
	if (ErrCheckFlag)
	{
		if (efData.m_strrType != RECORD_AN && efData.m_strrType != RECORD_CN && efData.m_strrType != RECORD_B && efData.m_strrType != RECORD_TLV && efData.m_strrType != RECORD_LOOP)
		{
			Set_Error_Flag();
			serr << "修改记录指令 Update_Record（" << MF << ", " << DDF << ", " << ADF << ", " << EF << ", " << RecordNo << "），从配置文件中获取到的记录类型不在预期之中（" << efData.m_strrType << "）。";
			Info->Append_Error(serr.str());
			flag = true;
		}
		if (efData.m_strrType != RECORD_LOOP && (!IsHex(efData.m_strTag) || efData.m_strTag.length() > 2))
		{
			Set_Error_Flag();
			serr << "修改记录指令 Update_Record（" << MF << ", " << DDF << ", " << ADF << ", " << EF << ", " << RecordNo << "），记录标志必须由2个十六进制字符组成，而当前是“" << efData.m_strTag << "”。";
			Info->Append_Error(serr.str());
			flag = true;
		}
		if (!IsHex(efData.m_strLen) || efData.m_strLen.length() > 2)
		{
			Set_Error_Flag();
			serr << "修改记录指令 Update_Record（" << MF << ", " << DDF << ", " << ADF << ", " << EF << ", " << RecordNo << "），记录长度必须由2个十六进制字符组成，而当前是“" << efData.m_strLen << "”。";
			Info->Append_Error(serr.str());
			flag = true;
		}
		if (StrDel_ConVar(efData.m_strVal, str) == false)
		{
			if ((efData.m_strrType == RECORD_CN || efData.m_strrType == RECORD_B) && !CheckData(str, -HexToDec(efData.m_strLen), estr))
			{
				Set_Error_Flag();
				serr << "修改记录指令 Update_Record（" << MF << ", " << DDF << ", " << ADF << ", " << EF << ", " << RecordNo << "），记录的值有误。。";
				Info->Append_Error(serr.str());
				flag = true;
			}
			if (efData.m_strrType == RECORD_AN && !StrTrim(efData.m_strVal, "0").empty() && efData.m_strVal.length() > (unsigned long)HexToDec(efData.m_strLen))
			{
				Set_Error_Flag();
				serr << "修改记录指令 Update_Record（" << MF << ", " << DDF << ", " << ADF << ", " << EF << ", " << RecordNo << "），记录长度“" << efData.m_strLen << "”有误。";
				Info->Append_Error(serr.str());
				flag = true;
			}
		}	
	}
	if (flag)
		return "";
	//指令构建
		//B-TLV
	if (efData.m_strrType == RECORD_TLV)
	{
		sstr << "00" << Style_InstHead;
		sstr << "DC" << Style_InstHead;
		sstr << setfill('0') << setw(2) << right << efData.m_strP1 << Style_InstHead;
		sstr << "04" << Style_InstHead;
		sstr << setfill('0') << setw(2) << right << hex << uppercase << HexToDec(efData.m_strLen) + 2 << Style_Separator;
		sstr << setfill('0') << setw(2) << right << ToUpper(efData.m_strTag);
		sstr << setfill('0') << setw(2) << right << ToUpper(efData.m_strLen);
		for (string::size_type i = 0; i < efData.m_vecEFExDatas.size(); ++i)
		{
			sstr << Style_InstData;
			CON_Extend(efData.m_vecEFExDatas.at(i).m_strVal);
			//配置检查
			if (ErrCheckFlag)
			{
				if (!IsHex(efData.m_vecEFExDatas.at(i).m_strTag) || efData.m_vecEFExDatas.at(i).m_strTag.length() > 2)
				{
					Set_Error_Flag();
					serr << "修改记录指令 Update_Record（" << MF << ", " << DDF << ", " << ADF << ", " << EF << ", " << RecordNo << "），B-TLV子记录标志必须由2个十六进制字符组成，而当前是“" << efData.m_strTag << "”。";
					Info->Append_Error(serr.str());
					flag = true;
				}
				if (!IsHex(efData.m_vecEFExDatas.at(i).m_strLen) || efData.m_vecEFExDatas.at(i).m_strLen.length() > 2)
				{
					Set_Error_Flag();
					serr << "修改记录指令 Update_Record（" << MF << ", " << DDF << ", " << ADF << ", " << EF << ", " << RecordNo << "），B-TLV子记录长度必须由2个十六进制字符组成，而当前是“" << efData.m_strTag << "”。";
					Info->Append_Error(serr.str());
					flag = true;
				}
				if (efData.m_vecEFExDatas.at(i).m_strrType != RECORD_AN && efData.m_vecEFExDatas.at(i).m_strrType != RECORD_CN && efData.m_vecEFExDatas.at(i).m_strrType != RECORD_B)
				{
					Set_Error_Flag();
					serr << "修改记录指令 Update_Record（" << MF << ", " << DDF << ", " << ADF << ", " << EF << ", " << RecordNo << "），配置中标签为“" << efData.m_vecEFExDatas.at(i).m_strTag << "”的类型（" << efData.m_vecEFExDatas.at(i).m_strrType << "）有误。。";
					Info->Append_Error(serr.str());
					flag = true;

				}
				if ((efData.m_vecEFExDatas.at(i).m_strrType == RECORD_CN || efData.m_vecEFExDatas.at(i).m_strrType == RECORD_B) && StrDel_ConVar(efData.m_vecEFExDatas.at(i).m_strVal, str) == false)
				{
					if (CheckData(str, -HexToDec(efData.m_vecEFExDatas.at(i).m_strLen), estr) == false)
					{
						Set_Error_Flag();
						serr << "修改记录指令 Update_Record（" << MF << ", " << DDF << ", " << ADF << ", " << EF << ", " << RecordNo << "），配置中标签为“" << efData.m_vecEFExDatas.at(i).m_strTag << "”的值有误。。";
						Info->Append_Error(serr.str());
						flag = true;
					}
				}
				if (StrDel_ConVar(efData.m_vecEFExDatas.at(i).m_strVal, str) == false)
				{
					if ((efData.m_vecEFExDatas.at(i).m_strrType == RECORD_CN || efData.m_vecEFExDatas.at(i).m_strrType == RECORD_B) && !CheckData(str, -HexToDec(efData.m_vecEFExDatas.at(i).m_strLen), estr))
					{
						Set_Error_Flag();
						serr << "修改记录指令 Update_Record（" << MF << ", " << DDF << ", " << ADF << ", " << EF << ", " << RecordNo << "），配置中标签为“" << efData.m_vecEFExDatas.at(i).m_strTag << "”的记录内容有误。";
						Info->Append_Error(serr.str());
						flag = true;
					}
					if (efData.m_vecEFExDatas.at(i).m_strrType == RECORD_AN && !StrTrim(efData.m_vecEFExDatas.at(i).m_strVal, "0").empty() && efData.m_vecEFExDatas.at(i).m_strVal.length() > (unsigned long)HexToDec(efData.m_vecEFExDatas.at(i).m_strLen))
					{
						Set_Error_Flag();
						serr << "修改记录指令 Update_Record（" << MF << ", " << DDF << ", " << ADF << ", " << EF << ", " << RecordNo << "），配置中标签为“" << efData.m_vecEFExDatas.at(i).m_strTag << "”的记录内容的长度有误。";
						Info->Append_Error(serr.str());
						flag = true;
					}
				}
			}
			if (flag)
				return "";
			//指令
			sstr << setfill('0') << setw(2) << right << efData.m_vecEFExDatas.at(i).m_strTag;
			sstr << setfill('0') << setw(2) << right << efData.m_vecEFExDatas.at(i).m_strLen;
			if (efData.m_vecEFExDatas.at(i).m_strrType == RECORD_CN)//cn
			{
				if (InitFlag || StrTrim(efData.m_vecEFExDatas.at(i).m_strVal, "F").empty())
					sstr << setfill('F') << setw(HexToDec(efData.m_vecEFExDatas.at(i).m_strLen) * 2) << "";
				else
					sstr << setfill('F') << setw(HexToDec(efData.m_vecEFExDatas.at(i).m_strLen) * 2) << left << ToUpper(efData.m_vecEFExDatas.at(i).m_strVal);
			}
			else if (efData.m_vecEFExDatas.at(i).m_strrType == RECORD_AN)//an
			{
				if (InitFlag || StrTrim(efData.m_vecEFExDatas.at(i).m_strVal, "0").empty())
					sstr << setfill('0') << setw(HexToDec(efData.m_vecEFExDatas.at(i).m_strLen) * 2) << "";
				else
					sstr << "\"" << ToUpper(efData.m_vecEFExDatas.at(i).m_strVal) << "\"" << setfill('0') << setw((HexToDec(efData.m_vecEFExDatas.at(i).m_strLen) - efData.m_vecEFExDatas.at(i).m_strVal.length()) * 2) << "";
			}
			else//b
			{
				if (InitFlag || StrTrim(efData.m_vecEFExDatas.at(i).m_strVal, "0").empty())
					sstr << setfill('0') << setw(HexToDec(efData.m_vecEFExDatas.at(i).m_strLen) * 2) << "";
				else
					sstr << setfill('0') << setw(HexToDec(efData.m_vecEFExDatas.at(i).m_strLen) * 2) << left << ToUpper(efData.m_vecEFExDatas.at(i).m_strVal);
			}
		}
	}
	//循环记录
	else if (efData.m_strrType == RECORD_LOOP)
	{
		sstr << "00" << Style_InstHead;
		sstr << "DC" << Style_InstHead;
		sstr << "00" << Style_InstHead;
		sstr << "03" << Style_InstHead;
		sstr << setfill('0') << setw(2) << right << ToUpper(efData.m_strLen) << Style_Separator;
		for (string::size_type i = 0; i < efData.m_vecEFExDatas.size(); ++i)
		{
			if (i)
				sstr << Style_InstData;
			CON_Extend(efData.m_vecEFExDatas.at(i).m_strVal);
			//配置检查
			if (ErrCheckFlag)
			{
				if (!IsHex(efData.m_vecEFExDatas.at(i).m_strLen) || efData.m_vecEFExDatas.at(i).m_strLen.length() > 2)
				{
					Set_Error_Flag();
					serr << "修改记录指令 Update_Record（" << MF << ", " << DDF << ", " << ADF << ", " << EF << ", " << RecordNo << "），循环记录的子记录（P1：“" << efData.m_vecEFExDatas.at(i).m_strP1 << "”）长度必须由2个十六进制字符组成，而当前是“" << efData.m_strLen << "”。";
					Info->Append_Error(serr.str());
					flag = true;
				}
				if (efData.m_vecEFExDatas.at(i).m_strrType != RECORD_AN && efData.m_vecEFExDatas.at(i).m_strrType != RECORD_CN && efData.m_vecEFExDatas.at(i).m_strrType != RECORD_B)
				{
					Set_Error_Flag();
					serr << "修改记录指令 Update_Record（" << MF << ", " << DDF << ", " << ADF << ", " << EF << ", " << RecordNo << "），循环子记录（P1：“" << efData.m_vecEFExDatas.at(i).m_strP1 << "”）的类型（" << efData.m_vecEFExDatas.at(i).m_strrType << "）有误。。";
					Info->Append_Error(serr.str());
					flag = true;
				}
				if (StrDel_ConVar(efData.m_vecEFExDatas.at(i).m_strVal, str) == false)
				{
					if ((efData.m_vecEFExDatas.at(i).m_strrType == RECORD_CN || efData.m_vecEFExDatas.at(i).m_strrType == RECORD_B) && !CheckData(str, -HexToDec(efData.m_vecEFExDatas.at(i).m_strLen), estr))
					{
						Set_Error_Flag();
						serr << "修改记录指令 Update_Record（" << MF << ", " << DDF << ", " << ADF << ", " << EF << ", " << RecordNo << "），循环子记录（P1：“" << efData.m_vecEFExDatas.at(i).m_strP1 << "”）的记录内容有误。";
						Info->Append_Error(serr.str());
						flag = true;
					}
					if (efData.m_vecEFExDatas.at(i).m_strrType == RECORD_AN && !StrTrim(efData.m_vecEFExDatas.at(i).m_strVal, "0").empty() && efData.m_vecEFExDatas.at(i).m_strVal.length() > (unsigned long)HexToDec(efData.m_vecEFExDatas.at(i).m_strLen))
					{
						Set_Error_Flag();
						serr << "修改记录指令 Update_Record（" << MF << ", " << DDF << ", " << ADF << ", " << EF << ", " << RecordNo << "），循环子记录（P1：“" << efData.m_vecEFExDatas.at(i).m_strP1 << "”）记录内容的长度有误。";
						Info->Append_Error(serr.str());
						flag = true;
					}
				}
			}
			if (flag)
				return "";
			//指令
			if (efData.m_vecEFExDatas.at(i).m_strrType == RECORD_CN)//cn
			{
				if (InitFlag || StrTrim(efData.m_vecEFExDatas.at(i).m_strVal, "F").empty())
					sstr << setfill('F') << setw(HexToDec(efData.m_vecEFExDatas.at(i).m_strLen) * 2) << "";
				else
					sstr << setfill('F') << setw(HexToDec(efData.m_vecEFExDatas.at(i).m_strLen) * 2) << left << ToUpper(efData.m_vecEFExDatas.at(i).m_strVal);
			}
			else if (efData.m_vecEFExDatas.at(i).m_strrType == RECORD_AN)//an
			{
				if (InitFlag || StrTrim(efData.m_vecEFExDatas.at(i).m_strVal, "0").empty())
					sstr << setfill('0') << setw(HexToDec(efData.m_vecEFExDatas.at(i).m_strLen) * 2) << "";
				else
					sstr << "\"" << ToUpper(efData.m_vecEFExDatas.at(i).m_strVal) << "\"" << setfill('0') << setw((HexToDec(efData.m_vecEFExDatas.at(i).m_strLen) - efData.m_vecEFExDatas.at(i).m_strVal.length()) * 2) << "";
			}
			else//b
			{
				if (InitFlag || StrTrim(efData.m_vecEFExDatas.at(i).m_strVal, "0").empty())
					sstr << setfill('0') << setw(HexToDec(efData.m_vecEFExDatas.at(i).m_strLen) * 2) << "";
				else
					sstr << setfill('0') << setw(HexToDec(efData.m_vecEFExDatas.at(i).m_strLen) * 2) << left << ToUpper(efData.m_vecEFExDatas.at(i).m_strVal);
			}
		}
	}
	//an、cn、b
	else
	{
		sstr << "00" << Style_InstHead;
		sstr << "DC" << Style_InstHead;
		sstr << setfill('0') << setw(2) << right << ToUpper(efData.m_strP1) << Style_InstHead;
		sstr << "04" << Style_InstHead;
		sstr << setfill('0') << setw(2) << right << hex << uppercase << HexToDec(efData.m_strLen) + 2 << Style_Separator;
		sstr << setfill('0') << setw(2) << right << ToUpper(efData.m_strTag);
		sstr << setfill('0') << setw(2) << right << ToUpper(efData.m_strLen) << Style_InstData;
		if (efData.m_strrType == RECORD_CN)//cn
		{
			if (InitFlag || StrTrim(efData.m_strVal, "F").empty())
				sstr << setfill('F') << setw(HexToDec(efData.m_strLen) * 2) << "";
			else
				sstr << ToUpper(efData.m_strVal);
		}
		else if (efData.m_strrType == RECORD_AN)//an
		{
			if (InitFlag || StrTrim(efData.m_strVal, "0").empty())
				sstr << setfill('0') << setw(HexToDec(efData.m_strLen) * 2) << "";
			else
				sstr << "\"" << ToUpper(efData.m_strVal) << "\"";
		}
		else//b
		{
			if (InitFlag || StrTrim(efData.m_strVal, "0").empty())
				sstr << setfill('0') << setw(HexToDec(efData.m_strLen) * 2) << "";
			else
				sstr << ToUpper(efData.m_strVal);
		}
	}
	strcpy(InstHead, StrTrim(sstr.str()).substr(0, 10).c_str());
	strcpy(InstData, StrTrim(sstr.str()).erase(0, 10).c_str());
	//收尾
	if (RefreshFlag)
		Info->Append_Script(sstr.str());
	return sstr.str();
}

string CInstruction::Update_Binary(int Offset, int Len, string Data, bool RefreshFlag /*= true */)//修改二进制
{
	bool flag = false;//该函数内的错误标志
	string str;
	string estr;
	Initialization();
	//参数检查
	if (ErrCheckFlag)
	{
		if (Offset<0 || Offset>0x7FFF)
		{
			Set_Error_Flag();
			serr << "修改二进制指令 Update_Binary（" << Offset << ", " << Len << ", " << Data << "），偏移量取值范围为0-0x7FFF，而当前为“0x" << hex << setfill('0') << setw(4) << uppercase << right << Offset << "”。";
			Info->Append_Error(serr.str());
			flag = true;
		}
		if (Len<0 || Len>255)
		{
			Set_Error_Flag();
			serr << "修改二进制指令 Update_Binary（" << Offset << ", " << Len << ", " << Data << "），指定数据域长度参数的取值范围为0-255，而当前是“" << Len << "”。";
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
			serr << "修改二进制指令 Update_Binary（" << Offset << ", " << Len << ", " << Data << "），参数Data（" << Data << ")有误，" << estr;
			Info->Append_Error(serr.str());
			flag = true;
		}
		if (!Style_InstData.empty())
		{
			Data = StrTrim(Data);
			for (int pos = 0; pos < Len; pos += 8)
				if (pos + 8 < Len)
					Data.insert(pos * 2, Style_InstData);
		}
	}
	if (flag)
		return "";
	//指令构建
	sstr << "00" << Style_InstHead;
	sstr << "D6" << Style_InstHead;
	sstr << setfill('0') << setw(2) << right << hex << uppercase << Offset / 0x100 << Style_InstHead;
	sstr << setfill('0') << setw(2) << right << hex << uppercase << Offset % 0x100 << Style_InstHead;
	sstr << setfill('0') << setw(2) << right << hex << uppercase << Len << Style_Separator;
	sstr << ToUpper(Data);
	strcpy(InstHead, StrTrim(sstr.str()).substr(0, 10).c_str());
	strcpy(InstData, StrTrim(sstr.str()).erase(0, 10).c_str());
	//收尾
	if (RefreshFlag)
		Info->Append_Script(sstr.str());
	return sstr.str();
}

string CInstruction::DIR_Record(int RecordNo, string MF, string DDF, string ADF, string Label, bool RefreshFlag /*= true*/)
{
	bool flag = false;//该函数内的错误标志
	S_MFINFO mfInfo;
	S_DDFINFO ddfInfo;
	S_ADFINFO adfInfo;
	string aid;
	stringstream stemp;
	Initialization();
	//参数检查
	if (ErrCheckFlag)
	{
		if (MF != "MF" && MF != "3F00" && MF != "")
		{
			Set_Error_Flag();
			serr << "添加DIR记录 DIR_Record(" << RecordNo << ", " << MF << ", " << DDF << ", " << ADF << ", " << Label << ")，MF只可取值为“MF”、“3F00”或空串“”，而当前是“" << MF << "”。";
			Info->Append_Error(serr.str());
			flag = true;
		}
		if (!ADF.empty() && (!IsHex(ADF) || ADF.length() != 4))
		{
			Set_Error_Flag();
			serr << "添加DIR记录 DIR_Record(" << RecordNo << ", " << MF << ", " << DDF << ", " << ADF << ", " << Label << ")，参数ADF必须由4个十六进制字符组成，而当前为“" << ADF << "”。";
			Info->Append_Error(serr.str());
			flag = true;
		}
		if (RecordNo <= 0)
		{
			serr.clear();
			serr.str("");
			serr << "添加DIR记录 DIR_Record(" << RecordNo << ", " << MF << ", " << DDF << ", " << ADF << ", " << Label << ")，记录号必须大于0。";
			Info->Append_Warning(serr.str());
		}
		if (Label.length() > 200)
		{
			Set_Error_Flag();
			serr << "添加DIR记录 DIR_Record(" << RecordNo << ", " << MF << ", " << DDF << ", " << ADF << ", " << Label << ")，应用标签数据内容过多。";
			Info->Append_Error(serr.str());
			flag = true;
		}
	}
	//获取配置
	if (ADF.empty() == false)
	{
		if (Param->GetADFInfo(DDF, ADF, adfInfo) == false)
		{
			Set_Error_Flag();
			serr << "添加DIR记录 DIR_Record(" << RecordNo << ", " << MF << ", " << DDF << ", " << ADF << ", " << Label << ")，在配置文件内找不到指定路径的ADF文件。";
			Info->Append_Error(serr.str());
			flag = true;
		}
		aid = adfInfo.m_strAID;
	}
	else if (DDF.empty() == false)
	{
		if (Param->GetDDFInfo(DDF, ddfInfo) == false)
		{
			Set_Error_Flag();
			serr << "添加DIR记录 DIR_Record(" << RecordNo << ", " << MF << ", " << DDF << ", " << ADF << ", " << Label << ")，在配置文件内找不到指定路径的DDF文件。";
			Info->Append_Error(serr.str());
			flag = true;
		}
		aid = ddfInfo.m_strAID;
	}
	else
	{
		serr.clear();
		serr.str("");
		serr << "添加DIR记录 DIR_Record(" << RecordNo << ", " << MF << ", " << DDF << ", " << ADF << ", " << Label << ")，在配置文件内找不到指定路径的DDF文件。";
		Info->Append_Warning(serr.str());
		Param->GetMFInfo(mfInfo);
		aid = mfInfo.m_strAID;
	}
	//配置检查
	if (ErrCheckFlag)
	{
		if (aid.length() % 2 != 0 || !IsHex(aid))
		{
			Set_Error_Flag();
			serr << "添加DIR记录 DIR_Record(" << RecordNo << ", " << MF << ", " << DDF << ", " << ADF << ", " << Label << ")，指定目录文件的AID(" << aid << ")长度有误或含非法字符。";
			Info->Append_Error(serr.str());
			flag = true;
		}
	}
	if (flag)
		return "";
	//构建数据域内容
	sstr.clear();
	sstr.str("");
	sstr << "70";
	sstr << setfill('0') << setw(2) << hex << uppercase << right << aid.length() / 2 + 6 + Label.length();
	sstr << "61";
	sstr << setfill('0') << setw(2) << hex << uppercase << right << aid.length() / 2 + 4 + Label.length();
	sstr << "4F";
	sstr << setfill('0') << setw(2) << hex << uppercase << right << aid.length() / 2 << Style_InstData;
	sstr << aid << Style_InstData;
	sstr << "50";
	sstr << setfill('0') << setw(2) << hex << uppercase << right << Label.length() << Style_InstData;
	sstr << "\"" << Label << "\"";
	//完整指令
	stemp << setfill('0') << setw(2) << hex << uppercase << right << RecordNo;
	if (HexToDec(DIR_SFI) >= 0x05 && HexToDec(DIR_SFI) <= 0x1E)//在SFI寻址范围之类，自动使用SFI寻址
		return Update_Record(stemp.str(), DecToHex(HexToDec(DIR_SFI) * 8 + 4), (StrTrim(StrTrim(sstr.str()), "\"").length() + Label.length()) / 2, sstr.str(), RefreshFlag);
	else
		return Update_Record(stemp.str(), "04", (StrTrim(StrTrim(sstr.str()), "\"").length() + Label.length()) / 2, sstr.str(), RefreshFlag);
}

int CInstruction::EF_Control(string DDF, string ADF, string CtrlStr, KEYDATAS &keyData1, KEYDATAS &keyData2, string &ErrMsg)
{
	string::size_type n;
	if (CtrlStr == "" || CtrlStr == "00")//读自由
		return 1;
	else if (CtrlStr == "FF")//读禁止
		return 2;
	else if ((n = CtrlStr.find_first_of("+|")) != string::npos)//Key+Key 或 Key|Key
	{
		if (n != CtrlStr.find_last_of("+|"))
		{
			ErrMsg = "EF文件的读写受控密钥最多只能由上级目录及当前目录两个密钥来组成。";
			return -1;
		}
		if (Param->GetKeyInfo(DDF, "", StrDelBlank(CtrlStr.substr(0, n)), keyData1) == false)
		{
			ErrMsg = "找不到密钥" + StrDelBlank(CtrlStr.substr(0, n));
			return -1;
		}
		if (Param->GetKeyInfo(DDF, ADF, StrDelBlank(CtrlStr.substr(n + 1, CtrlStr.length() - n - 1)), keyData2) == false)
		{
			ErrMsg = "找不到密钥" + StrDelBlank(CtrlStr.substr(n + 1, CtrlStr.length() - n - 1));
			return -1;
		}
		if (CtrlStr.find_first_of('+') != string::npos)
			return 5;
		else
			return 6;
	}
	else if (CtrlStr == "PIN")
	{
		if (Param->GetKeyInfo(DDF, "", StrDelBlank(CtrlStr), keyData1) == false)
		{
			ErrMsg = "找不到密钥" + StrDelBlank(CtrlStr);
			return -1;
		}
		return 3;
	}
	else
	{
		if (Param->GetKeyInfo(DDF, ADF, StrDelBlank(CtrlStr), keyData1) == false)
		{
			ErrMsg = "找不到密钥" + StrDelBlank(CtrlStr);
			return -1;
		}
		return 4;
	}
}

void CInstruction::Init_Security_Level(string KeyNameIgnore)
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
	int cnt;//密钥序号，每个ADF下该序号都将从 1 开始

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
						if (ToUpper(vecKeyDatas.at(m).m_strName).find(vecKeyIgnore.at(n).KeyName) == 0)
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
					Param->ModeKeySeq(vecMFStruct.at(i).m_strDDFName, vecMFStruct.at(i).m_vecADFInfo.at(j).m_strADFName, vecKeyDatas.at(m).m_strName, cnt++);
			}
		}
	}
}

string CInstruction::PIN_Certify( bool RefreshFlag /*= true*/ )
{
	Initialization();
	KEYDATAS struKey;
	Param->GetKeyInfo("SSSE","","PIN",struKey);//获取SSSE下的PIN

	//指令构建
	sstr << "00" << Style_InstHead;
	sstr << "20" << Style_InstHead;
	sstr << "00" << Style_InstHead;
	sstr << "00" << Style_InstHead;
	sstr << setfill('0') << setw(2) << left << DecToHex(struKey.m_strValue.length()/2) << Style_Separator;
	sstr << struKey.m_strValue;
	strcpy(InstHead, StrTrim(sstr.str()).substr(0, 10).c_str());
	strcpy(InstData, StrTrim(sstr.str()).erase(0, 10).c_str());
	//收尾
	if (RefreshFlag)
		Info->Append_Script(sstr.str());
	return sstr.str();
}
