//#####################################
//
//	class CCommand
//
//	命令类（脚本规约中的命令）
//
//	仅构建脚本规约中的相应命令，与指令无关
//
//	注意：
//	所有构建命令的方法中都带有参数 RefreshFlag ，当该参数值为true时，所构建的指令将会自动添加至生成的脚本缓存中；
//	该参数一般默认为true，即默认为自动添加，但有个别方法的默认值为false，使用时请多加留意。
//
//	最后修改日期：2015-09-01 18:52
//#####################################


#include "Command.h"
#include "tool.h"

CCommand::CCommand(CInformation *inf):Info(inf)
{
	ErrFlag = false;
	ErrCheckFlag = true;
	Style_Assignment = "";
	Style_FuntionName = "";
	Style_Comma = "";
	RetValue.clear();
}

CCommand::~CCommand( )
{
}

void CCommand::ReSetErrFlag()//重设错误标志
{
	ErrFlag = false;
}

void CCommand::Set_Error_Flag()//设置错误标志
{
	ErrFlag = true;
	serr.clear();
	serr.str("");
}

bool CCommand::Error()//错误检查
{
	return ErrFlag;
}

void CCommand::Error_Check(bool Swf)//开关错误检查
{
	ErrCheckFlag = Swf;
}

void CCommand::Style_Set(unsigned char Sty)//设置脚本样式
{
	Style_Assignment = "";
	Style_FuntionName = "";
	Style_Comma = "";
	if (Sty & STYLE_ASSIGNMENT)
		Style_Assignment = " ";
	if (Sty & STYLE_FUNTION_NAME)
		Style_FuntionName = " ";
	if (Sty & STYLE_COMMA)
		Style_Comma = " ";
}

bool CCommand::CheckData(string Data, int Len)//数据检查
{
	string str;
	if (StrDel_ConVar(Data, str) == true)//含有常量或变量
	{
		if (JudgeHex(str) == false)//含有非法字符
			return false;
		else
			return true;
	}
	else//不含常量或变量
	{
		if (JudgeHex(str) == false)//含有非法字符
			return false;
		else//将判断Len长度
		{
			if (Len < 0)
				return true;
			else if (Len == 0)
			{
				if (str.length() % 2 != 0)
					return false;
				else
					return true;
			}
			else
			{
				if (str.length() == Len)
					return true;
				else
					return false;
			}
		}
	}
}



/***********************************************
命令方法
***********************************************/

string CCommand::VAR(string Data, int No, bool RefreshFlag /*= true */)//变量赋值
{
	bool flag = false;//该函数内的错误标志
	stringstream sstr;
	//错误检查
	if (ErrCheckFlag)
	{
		if (No < 0)
		{
			Set_Error_Flag();
			serr << "变量赋值 VAR（" << Data << ", " << No << "），变量序号不能小于0。";
			Info->Append_Error(serr.str());
			flag = true;
		}
	}
	if (flag)
		return "";
	//命令构建
	sstr.clear();
	sstr.str("");
	sstr << Data << Style_Assignment << ">" << Style_Assignment << No;
	//收尾
	if (RefreshFlag)
		Info->Append_Script(sstr.str());
	return sstr.str();
}

string CCommand::RESET(bool RefreshFlag /*= true */)//复位
{
	if (RefreshFlag)
		Info->Append_Script("RESET");
	return "RESET";
}

string CCommand::ATR(int Pos, int Len, int VarNo /*= -1*/, bool RefreshFlag /*= true */)//截取ATR
{
	bool flag = false;//该函数内的错误标志
	//错误检查
	if (ErrCheckFlag)
	{
		if (Pos < 0 || Len < 0)
		{
			Set_Error_Flag();
			serr << "复位应答 ATR（" << Pos << ", " << Len << ", " << VarNo << "），起始偏移“" << Pos << "”或截取长度“" << Len << "”小于0。";
			Info->Append_Error(serr.str());
			flag = true;
		}
	}
	if (flag)
		return "";
	//命令构建
	sstr.clear();
	sstr.str("");
	sstr << "[ATR" << Style_FuntionName << "(";
	sstr << Pos << "," << Style_Comma;
	sstr << Len << ")]";
	//收尾
	if (VarNo >= 0)
		sstr << Style_Assignment << ">" << Style_Assignment << VarNo;
	if (RefreshFlag)
		Info->Append_Script(sstr.str());
	return sstr.str();
}

string CCommand::DES(string Data, string Key, int VarNo /*= -1*/, bool RefreshFlag /*= true */)//DES算法
{
	bool flag = false;//该函数内的错误标志
	string str;
	//错误检查
	if (ErrCheckFlag)
	{
		if (CheckData(Data, 16) == false)
		{
			Set_Error_Flag();
			serr << "DES算法 DES（" << Data << ", " << Key << ", " << VarNo << "），待加密数据“" << Data << "”的必须由16个十六进制字符组成。";
			Info->Append_Error(serr.str());
			flag = true;
		}
		if (CheckData(Key, 16) == false)
		{
			Set_Error_Flag();
			serr << "DES算法 DES（" << Data << ", " << Key << ", " << VarNo << "），使用密钥“" << Key << "”的必须由16个十六进制字符组成。";
			Info->Append_Error(serr.str());
			flag = true;
		}
	}
	if (flag)
		return "";
	//命令构建
	sstr.clear();
	sstr.str("");
	sstr << "[DES" << Style_FuntionName << "(";
	sstr << Data << "," << Style_Comma;
	sstr << Key << ")]";
	//收尾
	if (VarNo >= 0)
		sstr << Style_Assignment << ">" << Style_Assignment << VarNo;
	if (RefreshFlag)
		Info->Append_Script(sstr.str());
	return sstr.str();
}

string CCommand::TDES(string Data, string Key, int VarNo /*= -1*/, bool RefreshFlag /*= true */)//3DES算法
{
	bool flag = false;//该函数内的错误标志
	string str;
	//错误检查
	if (ErrCheckFlag)
	{
		if (CheckData(Data, 16) == false)
		{
			Set_Error_Flag();
			serr << "3DES算法 TDES（" << Data << ", " << Key << ", " << VarNo << "），待加密数据“" << Data << "”的必须由16个十六进制字符组成。";
			Info->Append_Error(serr.str());
			flag = true;
		}
		if (CheckData(Key, 32) == false)
		{
			Set_Error_Flag();
			serr << "3DES算法 TDES（" << Data << ", " << Key << ", " << VarNo << "），使用密钥“" << Key << "”的必须由32个十六进制字符组成。";
			Info->Append_Error(serr.str());
			flag = true;
		}
	}
	if (flag)
		return "";
	//命令构建
	sstr.clear();
	sstr.str("");
	sstr << "[3DES" << Style_FuntionName << "(";
	sstr << Data << "," << Style_Comma;
	sstr << Key << ")]";
	//收尾
	if (VarNo >= 0)
		sstr << Style_Assignment << ">" << Style_Assignment << VarNo;
	if (RefreshFlag)
		Info->Append_Script(sstr.str());
	return sstr.str();
}

std::string CCommand::XOR(string Par1, string Par2, int VarNo /*= -1*/, bool RefreshFlag /*= true */)//异或运算
{
	bool flag = false;//该函数内的错误标志
	string str;
	//错误检查
	if (ErrCheckFlag)
	{
		if (Par1.empty() || CheckData(Par1, 0) == false)
		{
			Set_Error_Flag();
			serr << "异或运算 XOR（" << Par1 << ", " << Par2 << "），参数1“" << Par1 << "”含非法字符或长度不正确。";
			Info->Append_Error(serr.str());
			flag = true;
		}
		if (Par2.empty() || CheckData(Par2, 0) == false)
		{
			Set_Error_Flag();
			serr << "异或运算 XOR（" << Par1 << ", " << Par2 << "），参数2“" << Par2 << "”含非法字符或长度不正确。";
			Info->Append_Error(serr.str());
			flag = true;
		}
	}
	if (flag)
		return "";
	//命令构建
	sstr.clear();
	sstr.str("");
	sstr << "[XOR" << Style_FuntionName << "(";
	sstr << Par1 << "," << Style_Comma;
	sstr << Par2 << ")]";
	//收尾
	if (VarNo >= 0)
		sstr << Style_Assignment << ">" << Style_Assignment << VarNo;
	if (RefreshFlag)
		Info->Append_Script(sstr.str());
	return sstr.str();
}

string CCommand::CMAC(string Data, int Pos, int Len, string Key, int VarNo /*= -1*/, bool RefreshFlag /*= true */)//计算MAC
{
	bool flag = false;//该函数内的错误标志
	string str;
	//错误检查
	if (ErrCheckFlag)
	{
		if (StrDel_ConVar(Data, str) == false)
		{
			if (str.empty() || str.length() % 2 != 0)
			{
				Set_Error_Flag();
				serr << "计算安全鉴定报文 MAC（" << Data << ", " << Pos << ", " << Len << ", " << Key << "），源数据“" << Data << "”含非法字符或长度不正确。";
				Info->Append_Error(serr.str());
				flag = true;
			}
			if ((int)str.length() < Pos + Len)
			{
				Set_Error_Flag();
				serr << "计算安全鉴定报文 MAC（" << Data << ", " << Pos << ", " << Len << ", " << Key << "），指定的起始偏移“" << Pos << "”及运算长度“" << Len << "”超出了原字符串的范围。";
				Info->Append_Error(serr.str());
				flag = true;
			}
		}
		if ((Pos < 0) || (Len < 0))
		{
			Set_Error_Flag();
			serr << "计算安全鉴定报文 MAC（" << Data << ", " << Pos << ", " << Len << ", " << Key << "），起始偏移“" << Pos << "”或运算长度“" << Len << "”小于0。";
			Info->Append_Error(serr.str());
			flag = true;
		}
		if (CheckData(Key, 32) == false)
		{
			Set_Error_Flag();
			serr << "计算安全鉴定报文 MAC（" << Data << ", " << Pos << ", " << Len << ", " << Key << "），使用密钥“" << Key << "”必须由32个十六进制字符组成。";
			Info->Append_Error(serr.str());
			flag = true;
		}
	}
	if (flag)
		return "";
	//命令构建
	sstr.clear();
	sstr.str("");
	sstr << Data << " [CMAC" << Style_FuntionName << "(";
	sstr << Pos << "," << Style_Comma;
	sstr << Len << "," << Style_Comma;
	sstr << Key << ")]";
	//收尾
	if (VarNo >= 0)
		sstr << Style_Assignment << ">" << Style_Assignment << VarNo;
	if (RefreshFlag)
		Info->Append_Script(sstr.str());
	return sstr.str();
}

string CCommand::SUBSTR(int Pos, int Len, string Data, int VarNo /*= -1*/, bool RefreshFlag /*= true */)//字符串截取
{
	bool flag = false;//该函数内的错误标志
	string str;
	//错误检查
	if (ErrCheckFlag)
	{
		if ((Pos < 0) || (Len < 0))
		{
			Set_Error_Flag();
			serr << "字符串截取 SUBSTR（" << Pos << ", " << Len << ", " << Data << "），起始偏移“" << Pos << "”或运算长度“" << Len << "”小于0。";
			Info->Append_Error(serr.str());
			flag = true;
		}
		if (StrDel_ConVar(Data, str) == false)
		{
			if (str.empty() || str.length() % 2 != 0)
			{
				Set_Error_Flag();
				serr << "字符串截取 SUBSTR（" << Pos << ", " << Len << ", " << Data << "），源数据“" << Data << "”含非法字符或长度不正确。";
				Info->Append_Error(serr.str());
				flag = true;
			}
			if ((int)str.length() < Pos + Len)
			{
				Set_Error_Flag();
				serr << "字符串截取 SUBSTR（" << Pos << ", " << Len << ", " << Data << "），指定的起始偏移“" << Pos << "”及运算长度“" << Len << "”超出了原字符串的范围。";
				Info->Append_Error(serr.str());
				flag = true;
			}
		}
	}
	if (flag)
		return "";
	//命令构建
	sstr.clear();
	sstr.str("");
	sstr << "[SUBSTR" << Style_FuntionName << "(";
	sstr << Pos << "," << Style_Comma;
	sstr << Len << "," << Style_Comma;
	sstr << Data << ")]";
	//收尾
	if (VarNo >= 0)
		sstr << Style_Assignment << ">" << Style_Assignment << VarNo;
	if (RefreshFlag)
		Info->Append_Script(sstr.str());
	return sstr.str();
}

string CCommand::RESP(int Pos, int Len, int VarNo /*= -1*/, bool RefreshFlag /*= true */)//取卡片应答数据
{
	bool flag = false;//该函数内的错误标志
	//错误检查
	if (ErrCheckFlag)
	{
		if ((Pos < 0) || (Len < 0))
		{
			Set_Error_Flag();
			serr << "取应答 RESP（" << Pos << ", " << Len << "），起始偏移“" << Pos << "”或运算长度“" << Len << "”小于0。";
			Info->Append_Error(serr.str());
			flag = true;
		}
	}
	if (flag)
		return "";
	//命令构建
	sstr.clear();
	sstr.str("");
	sstr << "[RESP" << Style_FuntionName << "(";
	sstr << Pos << "," << Style_Comma;
	sstr << Len << ")]";
	//收尾
	if (VarNo >= 0)
		sstr << Style_Assignment << ">" << Style_Assignment << VarNo;
	if (RefreshFlag)
		Info->Append_Script(sstr.str());
	return sstr.str();
}

string CCommand::CONNECTJMJ(string DevType, string IP, string Port, bool RefreshFlag /*= true */)//连接加密机
{
	bool flag = false;//该函数内的错误标志
	string str;
	string::size_type pos, offset;
	string stemp;
	//错误检查
	if (ErrCheckFlag)
	{
		if (StrDel_ConVar(IP, str) == false)
		{
			offset = 0;
			for (int i = 0; i < 4; ++i)
			{
				pos = str.find(".", offset);
				if (pos != string::npos)
					stemp = str.substr(offset, pos-offset);
				if (pos == string::npos || (stemp.length() == 0) || (JudgeDec(stemp) == false) || (atoi(stemp.c_str()) > 255))
				{
					Set_Error_Flag();
					serr << "连接加密机 CONNECTJMJ（" << DevType << ", " << IP << ", " << Port << "），指定加密机的IP值“" << IP << "”非法。";
					Info->Append_Error(serr.str());
					flag = true;
				}
				offset = pos + 1;
			}
		}
		if (StrDel_ConVar(Port, str) == false)
		{
			if ((str.length() == 0) || (JudgeDec(str) == false) || (atoi(str.c_str()) > 65535))
			{
				Set_Error_Flag();
				serr << "连接加密机 CONNECTJMJ（" << DevType << ", " << IP << ", " << Port << "），指定加密机的端口值“" << Port << "”非法。";
				Info->Append_Error(serr.str());
				flag = true;
			}
		}
	}
	if (flag)
		return "";
	//命令构建
	sstr.clear();
	sstr.str("");
	sstr << "[CONNECTJMJ" << Style_FuntionName << "(";
	sstr << DevType << "," << Style_Comma;
	sstr << IP << "," << Style_Comma;
	sstr << Port << ")]";
	//收尾
	if (RefreshFlag)
		Info->Append_Script(sstr.str());
	return sstr.str();
}

string CCommand::CLOSEJMJ(bool RefreshFlag /*= true*/)
{
	//命令构建
	sstr.clear();
	sstr.str("");
	sstr << "[CLOSEJMJ]";
	//收尾
	if (RefreshFlag)
		Info->Append_Script(sstr.str());
	return sstr.str();
}

string CCommand::LOADKEY(string CosManu, string Stowage, string KeyNo, string PKeyNo, string Keyfsjs, string PKeyfsjs, string Keyfsyz, string PKeyfsyz, string Comm, string KeyHead, string KeyLen, string PKeyLen, bool RefreshFlag /*= true */)//装载密钥
{
	bool flag = false;//该函数内的错误标志
	string str;
	//错误检查
	if (ErrCheckFlag)
	{
		if (StrDel_ConVar(CosManu, str) == false)
		{
			if (str != "0" && str != "1" && str != "3" && str != "4" && str != "5" && str != "6" && str != "7" && str != "8" && str != "20")
			{
				serr.clear();
				serr.str("");
				serr << "装载密钥 LOADKEY（" << CosManu << ", " << Stowage << ", " << KeyNo << ", " << PKeyNo << ", " << Keyfsjs << ", " << PKeyfsjs << ", " << Keyfsyz << ", " << PKeyfsyz << ", " << Comm << ", " << KeyHead << ", " << KeyLen << ", " << PKeyLen << "），无法识别指定的COS厂商“" << CosManu << "”。";
				Info->Append_Warning(serr.str());
			}
		}
		if (StrDel_ConVar(Stowage, str) == false)
		{
			if (str != "0" && str != "1" && str != "10" && str != "11")
			{
				serr.clear();
				serr.str("");
				serr << "装载密钥 LOADKEY（" << CosManu << ", " << Stowage << ", " << KeyNo << ", " << PKeyNo << ", " << Keyfsjs << ", " << PKeyfsjs << ", " << Keyfsyz << ", " << PKeyfsyz << ", " << Comm << ", " << KeyHead << ", " << KeyLen << ", " << PKeyLen << "），无法识别指定的密钥装载方式“" << Stowage << "”。";
				Info->Append_Warning(serr.str());
			}
		}
		if (StrTrim(KeyNo).empty() || CheckData(KeyNo, -1) == false)
		{
			Set_Error_Flag();
			serr << "装载密钥 LOADKEY（" << CosManu << ", " << Stowage << ", " << KeyNo << ", " << PKeyNo << ", " << Keyfsjs << ", " << PKeyfsjs << ", " << Keyfsyz << ", " << PKeyfsyz << ", " << Comm << ", " << KeyHead << ", " << KeyLen << ", " << PKeyLen << "），指定的密钥地址“" << KeyNo << "”有误。";
			Info->Append_Error(serr.str());
			flag = true;
		}
		if (StrTrim(PKeyNo).empty() || CheckData(PKeyNo, -1) == false)
		{
			Set_Error_Flag();
			serr << "装载密钥 LOADKEY（" << CosManu << ", " << Stowage << ", " << KeyNo << ", " << PKeyNo << ", " << Keyfsjs << ", " << PKeyfsjs << ", " << Keyfsyz << ", " << PKeyfsyz << ", " << Comm << ", " << KeyHead << ", " << KeyLen << ", " << PKeyLen << "），指定的保护密钥地址“" << PKeyNo << "”有误。";
			Info->Append_Error(serr.str());
			flag = true;
		}
		if (StrDel_ConVar(Keyfsjs, str) == false)
		{
			if (str != "1" && str != "2" && str != "3")
			{
				serr.clear();
				serr.str("");
				serr << "装载密钥 LOADKEY（" << CosManu << ", " << Stowage << ", " << KeyNo << ", " << PKeyNo << ", " << Keyfsjs << ", " << PKeyfsjs << ", " << Keyfsyz << ", " << PKeyfsyz << ", " << Comm << ", " << KeyHead << ", " << KeyLen << ", " << PKeyLen << "），无法识别指定的分散级数“" << Keyfsjs << "”。";
				Info->Append_Warning(serr.str());
			}
		}
		if (StrDel_ConVar(PKeyfsjs, str) == false)
		{
			if (str != "0" && str != "1" && str != "2" && str != "3")
			{
				serr.clear();
				serr.str("");
				serr << "装载密钥 LOADKEY（" << CosManu << ", " << Stowage << ", " << KeyNo << ", " << PKeyNo << ", " << Keyfsjs << ", " << PKeyfsjs << ", " << Keyfsyz << ", " << PKeyfsyz << ", " << Comm << ", " << KeyHead << ", " << KeyLen << ", " << PKeyLen << "），无法识别指定的保护密钥分散级数“" << PKeyfsjs << "”。";
				Info->Append_Warning(serr.str());
			}
		}
		if (CheckData(Keyfsyz, 16) == false)
		{
			Set_Error_Flag();
			serr << "装载密钥 LOADKEY（" << CosManu << ", " << Stowage << ", " << KeyNo << ", " << PKeyNo << ", " << Keyfsjs << ", " << PKeyfsjs << ", " << Keyfsyz << ", " << PKeyfsyz << ", " << Comm << ", " << KeyHead << ", " << KeyLen << ", " << PKeyLen << "），分散因子“" << Keyfsyz << "”必须由16个十六进制字符组成。";
			Info->Append_Error(serr.str());
			flag = true;
		}
		if (StrTrim(PKeyfsyz).empty() || CheckData(PKeyfsyz, -1) == false)
		{
			Set_Error_Flag();
			serr << "装载密钥 LOADKEY（" << CosManu << ", " << Stowage << ", " << KeyNo << ", " << PKeyNo << ", " << Keyfsjs << ", " << PKeyfsjs << ", " << Keyfsyz << ", " << PKeyfsyz << ", " << Comm << ", " << KeyHead << ", " << KeyLen << ", " << PKeyLen << "），保护密钥分散因子“" << Keyfsyz << "”含有非法字符或长度有误。";
			Info->Append_Error(serr.str());
			flag = true;
		}
		if (CheckData(Comm, 10) == false)
		{
			serr.clear();
			serr.str("");
			serr << "装载密钥 LOADKEY（" << CosManu << ", " << Stowage << ", " << KeyNo << ", " << PKeyNo << ", " << Keyfsjs << ", " << PKeyfsjs << ", " << Keyfsyz << ", " << PKeyfsyz << ", " << Comm << ", " << KeyHead << ", " << KeyLen << ", " << PKeyLen << "），指令头部“" << Comm << "”一般要求由10个十六进制字符组成。";
			Info->Append_Warning(serr.str());
		}
		if (StrTrim(KeyHead).empty() || CheckData(KeyHead, 0) == false)
		{
			Set_Error_Flag();
			serr << "装载密钥 LOADKEY（" << CosManu << ", " << Stowage << ", " << KeyNo << ", " << PKeyNo << ", " << Keyfsjs << ", " << PKeyfsjs << ", " << Keyfsyz << ", " << PKeyfsyz << ", " << Comm << ", " << KeyHead << ", " << KeyLen << ", " << PKeyLen << "），密钥头部“" << KeyHead << "”含有非法字符或者长度有误。";
			Info->Append_Error(serr.str());
			flag = true;
		}
		if (StrDel_ConVar(KeyLen, str) == false)
		{
			if (str != "0" && str != "1")
			{
				serr.clear();
				serr.str("");
				serr << "装载密钥 LOADKEY（" << CosManu << ", " << Stowage << ", " << KeyNo << ", " << PKeyNo << ", " << Keyfsjs << ", " << PKeyfsjs << ", " << Keyfsyz << ", " << PKeyfsyz << ", " << Comm << ", " << KeyHead << ", " << KeyLen << ", " << PKeyLen << "），导出密钥的字节长度只能指定为0（表示8字节）及1（表示16字节），而当前是“" << KeyLen << "”。";
				Info->Append_Warning(serr.str());
			}
		}
		if (StrDel_ConVar(PKeyLen, str) == false)
		{
			if (str != "0" && str != "1")
			{
				serr.clear();
				serr.str("");
				serr << "装载密钥 LOADKEY（" << CosManu << ", " << Stowage << ", " << KeyNo << ", " << PKeyNo << ", " << Keyfsjs << ", " << PKeyfsjs << ", " << Keyfsyz << ", " << PKeyfsyz << ", " << Comm << ", " << KeyHead << ", " << KeyLen << ", " << PKeyLen << "），保护密钥的字节长度只能指定为0（表示8字节）及1（表示16字节），而当前是“" << PKeyLen << "”。";
				Info->Append_Warning(serr.str());
			}
		}
	}
	if (flag)
		return "";
	//命令构建
	sstr.clear();
	sstr.str("");
	sstr << "[LOADKEY" << Style_FuntionName << "(";
	sstr << CosManu << "," << Style_Comma;
	sstr << Stowage << "," << Style_Comma;
	sstr << KeyNo << "," << Style_Comma;
	sstr << PKeyNo << "," << Style_Comma;
	sstr << Keyfsjs << "," << Style_Comma;
	sstr << PKeyfsjs << "," << Style_Comma;
	sstr << Keyfsyz << "," << Style_Comma;
	sstr << PKeyfsyz << "," << Style_Comma;
	sstr << Comm << "," << Style_Comma;
	sstr << KeyHead << "," << Style_Comma;
	sstr << KeyLen << "," << Style_Comma;
	sstr << PKeyLen << ")]";
	//收尾
	if (RefreshFlag)
		Info->Append_Script(sstr.str());
	return sstr.str();
}

string CCommand::SETNAD(string CardNad, bool RefreshFlag /*= true */)//设置卡座
{
	bool flag = false;//该函数内的错误标志
	string str;
	//错误检查
	if (ErrCheckFlag)
	{
		if (StrDel_ConVar(CardNad, str) == false)
		{
			if (str != "0" && str != "1" && str != "2")
			{
				serr.clear();
				serr.str("");
				serr << "设置卡座 SETNAD（" << CardNad << "），无法识别指定的卡座编号“" << CardNad << "”。";
				Info->Append_Warning(serr.str());
			}
		}
	}
	if (flag)
		return "";
	//命令构建
	sstr.clear();
	sstr.str("");
	sstr << "[SETNAD" << Style_FuntionName << "(";
	sstr << CardNad << ")]";
	//收尾
	if (RefreshFlag)
		Info->Append_Script(sstr.str());
	return sstr.str();
}

string CCommand::GERNERALEXTERNALAUTH(string AuthType, string DF, string EF, string KeyIndex, string KeyVer, string DivFactor, string PSAMKeyType, string PSAMorJMJKeyID, string ConstKey, string Flag, bool RefreshFlag /*= true */)//外部认证
{
	bool flag = false;//该函数内的错误标志
	string str;
	int auth = -1;
	//错误检查
	if (ErrCheckFlag)
	{
		if (StrDel_ConVar(AuthType, str) == false)
		{
			if (str == "0")
				auth = 0;
			else if (str == "1")
				auth = 1;
			else if (str == "2")
				auth = 2;
			else
			{
				serr.clear();
				serr.str("");
				serr << "外部认证 GERNERALEXTERNALAUTH（" << AuthType << ", " << DF << ", " << EF << ", " << KeyIndex << ", " << KeyVer << ", " << DivFactor << ", " << PSAMKeyType << ", " << PSAMorJMJKeyID << ", " << ConstKey << ", " << Flag << "），认证方式只能指定为0（固定密钥）、1（加密机）、2（PSAM卡），而当前是“" << AuthType << "”。";
				Info->Append_Warning(serr.str());
			}
		}
		if (CheckData(DF, 4) == false)
		{
			Set_Error_Flag();
			serr << "外部认证 GERNERALEXTERNALAUTH（" << AuthType << ", " << DF << ", " << EF << ", " << KeyIndex << ", " << KeyVer << ", " << DivFactor << ", " << PSAMKeyType << ", " << PSAMorJMJKeyID << ", " << ConstKey << ", " << Flag << "），指定的认证密钥所在目录“" << DF << "”必须由4个十六进制字符组成。";
			Info->Append_Error(serr.str());
			flag = true;
		}
		if (CheckData(EF, 4) == false)
		{
			Set_Error_Flag();
			serr << "外部认证 GERNERALEXTERNALAUTH（" << AuthType << ", " << DF << ", " << EF << ", " << KeyIndex << ", " << KeyVer << ", " << DivFactor << ", " << PSAMKeyType << ", " << PSAMorJMJKeyID << ", " << ConstKey << ", " << Flag << "），指定的EF文件“" << EF << "”必须由4个十六进制字符组成。";
			Info->Append_Error(serr.str());
			flag = true;
		}
		if (CheckData(KeyIndex, 2) == false)
		{
			Set_Error_Flag();
			serr << "外部认证 GERNERALEXTERNALAUTH（" << AuthType << ", " << DF << ", " << EF << ", " << KeyIndex << ", " << KeyVer << ", " << DivFactor << ", " << PSAMKeyType << ", " << PSAMorJMJKeyID << ", " << ConstKey << ", " << Flag << "），指定的密钥标志“" << KeyIndex << "”必须由2个十六进制字符组成。";
			Info->Append_Error(serr.str());
			flag = true;
		}
		if (CheckData(KeyVer, 2) == false)
		{
			Set_Error_Flag();
			serr << "外部认证 GERNERALEXTERNALAUTH（" << AuthType << ", " << DF << ", " << EF << ", " << KeyIndex << ", " << KeyVer << ", " << DivFactor << ", " << PSAMKeyType << ", " << PSAMorJMJKeyID << ", " << ConstKey << ", " << Flag << "），指定的密钥版本“" << KeyVer << "”必须由2个十六进制字符组成。";
			Info->Append_Error(serr.str());
			flag = true;
		}
		switch (auth)
		{
		case 0:
			if (StrTrim(DivFactor).empty())
			{
				Set_Error_Flag();
				serr << "外部认证 GERNERALEXTERNALAUTH（" << AuthType << ", " << DF << ", " << EF << ", " << KeyIndex << ", " << KeyVer << ", " << DivFactor << ", " << PSAMKeyType << ", " << PSAMorJMJKeyID << ", " << ConstKey << ", " << Flag << "），即使是使用固定密钥的认证方式，分散因子“" << DivFactor << "”也不应该为空字符串，可以传入“00”。";
				Info->Append_Error(serr.str());
				flag = true;
			}
			break;
		case 1:
			if (CheckData(DivFactor, 16) == false)
			{
				Set_Error_Flag();
				serr << "外部认证 GERNERALEXTERNALAUTH（" << AuthType << ", " << DF << ", " << EF << ", " << KeyIndex << ", " << KeyVer << ", " << DivFactor << ", " << PSAMKeyType << ", " << PSAMorJMJKeyID << ", " << ConstKey << ", " << Flag << "），当使用加密机的认证方式时，分散因子“" << DivFactor << "”必须由16个十六进制字符组成。";
				Info->Append_Error(serr.str());
				flag = true;
			}
			break;
		case 2:
			if (CheckData(DivFactor, 48) == false)
			{
				Set_Error_Flag();
				serr << "外部认证 GERNERALEXTERNALAUTH（" << AuthType << ", " << DF << ", " << EF << ", " << KeyIndex << ", " << KeyVer << ", " << DivFactor << ", " << PSAMKeyType << ", " << PSAMorJMJKeyID << ", " << ConstKey << ", " << Flag << "），当使用加密机的认证方式时，分散因子“" << DivFactor << "”必须由48个十六进制字符组成。";
				Info->Append_Error(serr.str());
				flag = true;
			}
			break;
		default:
			if (StrTrim(DivFactor).empty() || CheckData(DivFactor, 0) == false)
			{
				Set_Error_Flag();
				serr << "外部认证 GERNERALEXTERNALAUTH（" << AuthType << ", " << DF << ", " << EF << ", " << KeyIndex << ", " << KeyVer << ", " << DivFactor << ", " << PSAMKeyType << ", " << PSAMorJMJKeyID << ", " << ConstKey << ", " << Flag << "），分散因子“" << KeyVer << "”含有非法字符或长度有误。";
				Info->Append_Error(serr.str());
				flag = true;
			}
		}
		if (auth != 2 && StrTrim(PSAMKeyType).empty())
		{
			Set_Error_Flag();
			serr << "外部认证 GERNERALEXTERNALAUTH（" << AuthType << ", " << DF << ", " << EF << ", " << KeyIndex << ", " << KeyVer << ", " << DivFactor << ", " << PSAMKeyType << ", " << PSAMorJMJKeyID << ", " << ConstKey << ", " << Flag << "），哪怕不使用PSAM卡的认证方式，PSAM卡密钥用途“" << PSAMKeyType << "”也不应该为空字符串，可以传入“00”。";
			Info->Append_Error(serr.str());
			flag = true;
		}
		if (auth == 2 && CheckData(PSAMKeyType, 2) == false)
		{
			Set_Error_Flag();
			serr << "外部认证 GERNERALEXTERNALAUTH（" << AuthType << ", " << DF << ", " << EF << ", " << KeyIndex << ", " << KeyVer << ", " << DivFactor << ", " << PSAMKeyType << ", " << PSAMorJMJKeyID << ", " << ConstKey << ", " << Flag << "），使用PSAM卡认证时，PSAM卡密钥用途“" << PSAMKeyType << "”必须由2个十六进制字符组成。";
			Info->Append_Error(serr.str());
			flag = true;
		}
		switch (auth)
		{
		case 0:
			if (StrTrim(PSAMorJMJKeyID).empty())
			{
				Set_Error_Flag();
				serr << "外部认证 GERNERALEXTERNALAUTH（" << AuthType << ", " << DF << ", " << EF << ", " << KeyIndex << ", " << KeyVer << ", " << DivFactor << ", " << PSAMKeyType << ", " << PSAMorJMJKeyID << ", " << ConstKey << ", " << Flag << "），即使是使用固定密钥的认证方式，PSAM密钥标志“" << PSAMorJMJKeyID << "”也不应该为空字符串，可以传入“00”。";
				Info->Append_Error(serr.str());
				flag = true;
			}
			break;
		case 1:
			if (StrTrim(PSAMorJMJKeyID).empty() || CheckData(PSAMorJMJKeyID, -1) == false)
			{
				Set_Error_Flag();
				serr << "外部认证 GERNERALEXTERNALAUTH（" << AuthType << ", " << DF << ", " << EF << ", " << KeyIndex << ", " << KeyVer << ", " << DivFactor << ", " << PSAMKeyType << ", " << PSAMorJMJKeyID << ", " << ConstKey << ", " << Flag << "），使用加密机认证方式时，密钥地址“" << PSAMorJMJKeyID << "”含非法字符或长度有误。";
				Info->Append_Error(serr.str());
				flag = true;
			}
			break;
		case 2:
			if (CheckData(PSAMorJMJKeyID, 2) == false)
			{
				Set_Error_Flag();
				serr << "外部认证 GERNERALEXTERNALAUTH（" << AuthType << ", " << DF << ", " << EF << ", " << KeyIndex << ", " << KeyVer << ", " << DivFactor << ", " << PSAMKeyType << ", " << PSAMorJMJKeyID << ", " << ConstKey << ", " << Flag << "），使用PSAM卡认证方式时，PSAM卡密钥标志“" << PSAMorJMJKeyID << "”必须由2个十六进制字符组成。";
				Info->Append_Error(serr.str());
				flag = true;
			}
			break;
		default:
			if (StrTrim(PSAMorJMJKeyID).empty() || CheckData(PSAMorJMJKeyID, -1) == false)
			{
				Set_Error_Flag();
				serr << "外部认证 GERNERALEXTERNALAUTH（" << AuthType << ", " << DF << ", " << EF << ", " << KeyIndex << ", " << KeyVer << ", " << DivFactor << ", " << PSAMKeyType << ", " << PSAMorJMJKeyID << ", " << ConstKey << ", " << Flag << "），PSAM卡密钥标志或加密机密钥地址“" << PSAMorJMJKeyID << "”含非法字符或长度有误。";
				Info->Append_Error(serr.str());
				flag = true;
			}
		}
		if (auth != 0 && StrTrim(ConstKey).empty())
		{
			Set_Error_Flag();
			serr << "外部认证 GERNERALEXTERNALAUTH（" << AuthType << ", " << DF << ", " << EF << ", " << KeyIndex << ", " << KeyVer << ", " << DivFactor << ", " << PSAMKeyType << ", " << PSAMorJMJKeyID << ", " << ConstKey << ", " << Flag << "），哪怕不使用固定密钥的认证方式，固定密钥值“" << PSAMKeyType << "”也不应该为空字符串，可以传入“00”。";
			Info->Append_Error(serr.str());
			flag = true;
		}
		if (auth == 0 && CheckData(ConstKey, 32) == false)
		{
			Set_Error_Flag();
			serr << "外部认证 GERNERALEXTERNALAUTH（" << AuthType << ", " << DF << ", " << EF << ", " << KeyIndex << ", " << KeyVer << ", " << DivFactor << ", " << PSAMKeyType << ", " << PSAMorJMJKeyID << ", " << ConstKey << ", " << Flag << "），使用固定密钥的认证方式时，固定密钥值“" << PSAMKeyType << "”必须由32个十六进制字符组成。";
			Info->Append_Error(serr.str());
			flag = true;
		}
	}
	if (flag)
		return "";
	//命令构建
	sstr.clear();
	sstr.str("");
	sstr << "[GERNERALEXTERNALAUTH" << Style_FuntionName << "(";
	sstr << AuthType << "," << Style_Comma;
	sstr << DF << "," << Style_Comma;
	sstr << EF << "," << Style_Comma;
	sstr << KeyIndex << "," << Style_Comma;
	sstr << KeyVer << "," << Style_Comma;
	sstr << DivFactor << "," << Style_Comma;
	sstr << PSAMKeyType << "," << Style_Comma;
	sstr << PSAMorJMJKeyID << "," << Style_Comma;
	sstr << ConstKey << "," << Style_Comma;
	sstr << Flag << ")]";
	//收尾
	if (RefreshFlag)
		Info->Append_Script(sstr.str());
	return sstr.str();
}

string CCommand::BEGINGO(bool RefreshFlag /*= true */)
{
	if (RefreshFlag)
		Info->Append_Script("BEGINGO");
	return "BEGINGO";
}

std::string CCommand::FINALGO(bool RefreshFlag /*= true */)
{
	if (RefreshFlag)
		Info->Append_Script("FINALGO");
	return "FINALGO";
}

string CCommand::GETMAC(string Type, string Serial, string DivFactor, string DivType, string KeyID, string Radom, string InitData, string ProtectKey, string GetRandom, string Pad, int VarNo /*= -1*/, bool RefreshFlag /*= true */)//求MAC
{
	bool flag = false;//该函数内的错误标志
	string str;
	int auth = -1;//认证方式，1-加密机，2-PSAM
	int rad = -1;//是否取随机数
	//错误检查
	if (ErrCheckFlag)
	{
		if (StrDel_ConVar(Type, str) == false)
		{
			if (str == "1" || str == "3")
				auth = 1;
			else if (str == "2" || str == "4")
				auth = 2;
			else
			{
				serr.clear();
				serr.str("");
				serr << "求安全鉴定报文 GETMAC（" << Type << ", " << Serial << ", " << DivFactor << ", " << DivType << ", " << KeyID << ", " << Radom << ", " << InitData << ", " << ProtectKey << ", " << GetRandom << ", " << Pad << "），工作类型：1-加密机计算认证码，2-PSAM卡算认证码，3-加密机算主控密钥，4-PSAM卡算主控密钥，而当前是“" << Type << "”。";
				Info->Append_Warning(serr.str());
			}
		}
		if (StrTrim(Serial).empty())
		{
			Set_Error_Flag();
			serr << "求安全鉴定报文 GETMAC（" << Type << ", " << Serial << ", " << DivFactor << ", " << DivType << ", " << KeyID << ", " << Radom << ", " << InitData << ", " << ProtectKey << ", " << GetRandom << ", " << Pad << "），第二个参数不应该为空串，建议传参“1”。";
			Info->Append_Error(serr.str());
			flag = true;
		}
		switch (auth)
		{
		case 1:
			if (CheckData(DivFactor, 16) == false)
			{
				Set_Error_Flag();
				serr << "求安全鉴定报文 GETMAC（" << Type << ", " << Serial << ", " << DivFactor << ", " << DivType << ", " << KeyID << ", " << Radom << ", " << InitData << ", " << ProtectKey << ", " << GetRandom << ", " << Pad << "），当使用加密机时，分散因子“" << DivFactor << "”必须由16个十六进制字符组成。";
				Info->Append_Error(serr.str());
				flag = true;
			}
			break;
		case 2:
			if (CheckData(DivFactor, 48) == false)
			{
				Set_Error_Flag();
				serr << "求安全鉴定报文 GETMAC（" << Type << ", " << Serial << ", " << DivFactor << ", " << DivType << ", " << KeyID << ", " << Radom << ", " << InitData << ", " << ProtectKey << ", " << GetRandom << ", " << Pad << "），当使用PSAM卡时，分散因子“" << DivFactor << "”必须由48个十六进制字符组成。";
				Info->Append_Error(serr.str());
				flag = true;
			}
			break;
		default:
			if (CheckData(DivFactor, 0) == false)
			{
				Set_Error_Flag();
				serr << "求安全鉴定报文 GETMAC（" << Type << ", " << Serial << ", " << DivFactor << ", " << DivType << ", " << KeyID << ", " << Radom << ", " << InitData << ", " << ProtectKey << ", " << GetRandom << ", " << Pad << "），分散因子“" << DivFactor << "”含非法字符或长度有误。";
				Info->Append_Error(serr.str());
				flag = true;
			}
			break;
		}
		if (auth != 2 && StrTrim(DivType).empty())
		{
			Set_Error_Flag();
			serr << "求安全鉴定报文 GETMAC（" << Type << ", " << Serial << ", " << DivFactor << ", " << DivType << ", " << KeyID << ", " << Radom << ", " << InitData << ", " << ProtectKey << ", " << GetRandom << ", " << Pad << "），哪怕不使用PSAM卡，PSAM卡用途“" << DivType << "”也不应该为空字符串，可以传入“00”。";
			Info->Append_Error(serr.str());
			flag = true;
		}
		if (auth == 2 && StrTrim(DivType).empty())
		{
			Set_Error_Flag();
			serr << "求安全鉴定报文 GETMAC（" << Type << ", " << Serial << ", " << DivFactor << ", " << DivType << ", " << KeyID << ", " << Radom << ", " << InitData << ", " << ProtectKey << ", " << GetRandom << ", " << Pad << "），当使用PSAM卡时，PSAM卡用途“" << DivType << "”必须由2个十六进制字符组成。";
			Info->Append_Error(serr.str());
			flag = true;
		}
		switch (auth)
		{
		case 1:
			if (StrTrim(KeyID).empty() || CheckData(KeyID, -1) == false)
			{
				Set_Error_Flag();
				serr << "求安全鉴定报文 GETMAC（" << Type << ", " << Serial << ", " << DivFactor << ", " << DivType << ", " << KeyID << ", " << Radom << ", " << InitData << ", " << ProtectKey << ", " << GetRandom << ", " << Pad << "），当使用加密机时，密钥地址“" << DivType << "”含非法字符或长度有误。";
				Info->Append_Error(serr.str());
				flag = true;
			}
			break;
		case 2:
			if (CheckData(KeyID, 2) == false)
			{
				Set_Error_Flag();
				serr << "求安全鉴定报文 GETMAC（" << Type << ", " << Serial << ", " << DivFactor << ", " << DivType << ", " << KeyID << ", " << Radom << ", " << InitData << ", " << ProtectKey << ", " << GetRandom << ", " << Pad << "），当使用PSAM卡时，PSAM卡密钥标志“" << DivType << "”必须由2个十六进制字符组成。";
				Info->Append_Error(serr.str());
				flag = true;
			}
			break;
		default:
			if (StrTrim(KeyID).empty() || CheckData(KeyID, -1) == false)
			{
				Set_Error_Flag();
				serr << "求安全鉴定报文 GETMAC（" << Type << ", " << Serial << ", " << DivFactor << ", " << DivType << ", " << KeyID << ", " << Radom << ", " << InitData << ", " << ProtectKey << ", " << GetRandom << ", " << Pad << "），密钥地址“" << DivType << "”含非法字符或长度有误。";
				Info->Append_Error(serr.str());
				flag = true;
			}
		}
		if (StrDel_ConVar(GetRandom, str) == false)
		{
			if (str.empty())
			{
				Set_Error_Flag();
				serr << "求安全鉴定报文 GETMAC（" << Type << ", " << Serial << ", " << DivFactor << ", " << DivType << ", " << KeyID << ", " << Radom << ", " << InitData << ", " << ProtectKey << ", " << GetRandom << ", " << Pad << "），取随机数标志GetRandom不应该为空，“0”表示不自动取随机数，“1”表示自动取随机数。";
				Info->Append_Error(serr.str());
				flag = true;
			}
			if (str == "0")
				rad = 0;
			else
				rad = 1;
		}
		if (StrDel_ConVar(Radom, str) == false)
		{
			if (rad == 0 && str.length() != 16)
			{
				Set_Error_Flag();
				serr << "求安全鉴定报文 GETMAC（" << Type << ", " << Serial << ", " << DivFactor << ", " << DivType << ", " << KeyID << ", " << Radom << ", " << InitData << ", " << ProtectKey << ", " << GetRandom << ", " << Pad << "），取随机数标志GetRandom指示着随机数从外部传入时，传入的随机数“" << Radom << "”必须由16个十六进制字符组成。";
				Info->Append_Error(serr.str());
				flag = true;
			}
			else if (rad == 1 && str.empty())
			{
				Set_Error_Flag();
				serr << "求安全鉴定报文 GETMAC（" << Type << ", " << Serial << ", " << DivFactor << ", " << DivType << ", " << KeyID << ", " << Radom << ", " << InitData << ", " << ProtectKey << ", " << GetRandom << ", " << Pad << "），哪怕取随机数标志GetRandom指示着将自动获取随机数，传入的随机数“" << Radom << "”也不应该为空串，可以传入“00”。";
				Info->Append_Error(serr.str());
				flag = true;
			}
		}
		if (StrTrim(InitData).empty() || CheckData(InitData, 0) == false)
		{
			Set_Error_Flag();
			serr << "求安全鉴定报文 GETMAC（" << Type << ", " << Serial << ", " << DivFactor << ", " << DivType << ", " << KeyID << ", " << Radom << ", " << InitData << ", " << ProtectKey << ", " << GetRandom << ", " << Pad << "），待计算MAC的原始数据“" << InitData << "”含非法字符或长度有误。";
			Info->Append_Error(serr.str());
			flag = true;
		}
		if (StrTrim(ProtectKey).empty())
		{
			Set_Error_Flag();
			serr << "求安全鉴定报文 GETMAC（" << Type << ", " << Serial << ", " << DivFactor << ", " << DivType << ", " << KeyID << ", " << Radom << ", " << InitData << ", " << ProtectKey << ", " << GetRandom << ", " << Pad << "），不管是否使用加密机，保护密钥都不应为空，建议传入“1”。";
			Info->Append_Error(serr.str());
			flag = true;
		}
	}
	if (flag)
		return "";
	//命令构建
	sstr.clear();
	sstr.str("");
	sstr << "[GETMAC" << Style_FuntionName << "(";
	sstr << Type << "," << Style_Comma;
	sstr << Serial << "," << Style_Comma;
	sstr << DivFactor << "," << Style_Comma;
	sstr << DivType << "," << Style_Comma;
	sstr << KeyID << "," << Style_Comma;
	sstr << Radom << "," << Style_Comma;
	sstr << InitData << "," << Style_Comma;
	sstr << ProtectKey << "," << Style_Comma;
	sstr << GetRandom << "," << Style_Comma;
	sstr << Pad << ")]";
	//收尾
	if (VarNo >= 0)
		sstr << Style_Assignment << ">" << Style_Assignment << VarNo;
	if (RefreshFlag)
		Info->Append_Script(sstr.str());
	return sstr.str();
}

string CCommand::PRODUCT(string Par1, string Par2, int VarNo /*= -1*/, bool RefreshFlag /*= true */)//乘法运算
{
	bool flag = false;//该函数内的错误标志
	string str;
	//错误检查
	if (ErrCheckFlag)
	{
		StrDel_ConVar(Par1, str);
		if (StrTrim(Par1).empty() || JudgeDec(str) == false)
		{
			Set_Error_Flag();
			serr << "乘法运算 PRODUCT（" << Par1 << ", " << Par2 << "），参数1“" << Par1 << "”只能由十进制数字组成。";
			Info->Append_Error(serr.str());
			flag = true;
		}
		StrDel_ConVar(Par2, str);
		if (StrTrim(Par2).empty() || JudgeDec(str) == false)
		{
			Set_Error_Flag();
			serr << "乘法运算 PRODUCT（" << Par1 << ", " << Par2 << "），参数2“" << Par2 << "”只能由十进制数字组成。";
			Info->Append_Error(serr.str());
			flag = true;
		}
	}
	if (flag)
		return "";
	//命令构建
	sstr.clear();
	sstr.str("");
	sstr << "[PRODUCT" << Style_FuntionName << "(";
	sstr << Par1 << "," << Style_Comma;
	sstr << Par2 << ")]";
	//收尾
	if (VarNo >= 0)
		sstr << Style_Assignment << ">" << Style_Assignment << VarNo;
	if (RefreshFlag)
		Info->Append_Script(sstr.str());
	return sstr.str();
}

string CCommand::HEX(string Decimal, int VarNo /*= -1*/, bool RefreshFlag /*= true */)//十进制串转换成十六进制串
{
	bool flag = false;//该函数内的错误标志
	string str;
	//错误检查
	if (ErrCheckFlag)
	{
		StrDel_ConVar(Decimal, str);
		if (StrTrim(Decimal).empty() || JudgeDec(str) == false)
		{
			Set_Error_Flag();
			serr << "十进制串转换成十六进制串 HEX（" << Decimal << "），待转换的十进制数据“" << Decimal << "”含非法字符或长度有误。";
			Info->Append_Error(serr.str());
			flag = true;
		}
	}
	if (flag)
		return "";
	//命令构建
	sstr.clear();
	sstr.str("");
	sstr << "[HEX" << Style_FuntionName << "(";
	sstr << Decimal << ")]";
	//收尾
	if (VarNo >= 0)
		sstr << Style_Assignment << ">" << Style_Assignment << VarNo;
	if (RefreshFlag)
		Info->Append_Script(sstr.str());
	return sstr.str();
}

string CCommand::DHEX(string Hexadecimal, int VarNo /*= -1*/, bool RefreshFlag /*= true */)//十六进制串转换成十进制串
{
	bool flag = false;//该函数内的错误标志
	string str;
	//错误检查
	if (ErrCheckFlag)
	{
		StrDel_ConVar(Hexadecimal, str);
		if (StrTrim(Hexadecimal).empty() || JudgeHex(str) == false)
		{
			Set_Error_Flag();
			serr << "十六进制串转换成十进制串 DHEX（" << Hexadecimal << "），待转换的十六进制数据“" << Hexadecimal << "”含非法字符或长度有误。";
			Info->Append_Error(serr.str());
			flag = true;
		}
	}
	if (flag)
		return "";
	//命令构建
	sstr.clear();
	sstr.str("");
	sstr << "[DHEX" << Style_FuntionName << "(";
	sstr << Hexadecimal << ")]";
	//收尾
	if (VarNo >= 0)
		sstr << Style_Assignment << ">" << Style_Assignment << VarNo;
	if (RefreshFlag)
		Info->Append_Script(sstr.str());
	return sstr.str();
}

string CCommand::EX_HEX(string Decimal, string Len, int VarNo /*= -1*/, bool RefreshFlag /*= true */)//十进制串转换为十六进制串
{
	bool flag = false;//该函数内的错误标志
	string str;
	//错误检查
	if (ErrCheckFlag)
	{
		StrDel_ConVar(Decimal, str);
		if (StrTrim(Decimal).empty() || JudgeDec(str) == false)
		{
			Set_Error_Flag();
			serr << "十进制串转换为十六进制串 EX_HEX（" << Decimal << ", " << Len << "），待转换的十进制数据“" << Decimal << "”含非法字符或长度有误。";
			Info->Append_Error(serr.str());
			flag = true;
		}
		StrDel_ConVar(Len, str);
		if (StrTrim(Len).empty() || JudgeDec(str) == false)
		{
			Set_Error_Flag();
			serr << "十进制串转换为十六进制串 EX_HEX（" << Decimal << ", " << Len << "），指定的转换后长度“" << Len << "”只能由十进制数字组成。";
			Info->Append_Error(serr.str());
			flag = true;
		}
	}
	if (flag)
		return "";
	//命令构建
	sstr.clear();
	sstr.str("");
	sstr << "[EX_HEX" << Style_FuntionName << "(";
	sstr << Decimal << "," << Style_Comma;
	sstr << Len << ")]";
	//收尾
	if (VarNo >= 0)
		sstr << Style_Assignment << ">" << Style_Assignment << VarNo;
	if (RefreshFlag)
		Info->Append_Script(sstr.str());
	return sstr.str();
}

string CCommand::LEN(string Data, int VarNo /*= -1*/, bool RefreshFlag /*= true */)//返回十六进制表示的字符串字节数
{
	//命令构建
	sstr.clear();
	sstr.str("");
	sstr << "[LEN" << Style_FuntionName << "(";
	sstr << Data << ")]";
	//收尾
	if (VarNo >= 0)
		sstr << Style_Assignment << ">" << Style_Assignment << VarNo;
	if (RefreshFlag)
		Info->Append_Script(sstr.str());
	return sstr.str();
}

string CCommand::GETTIME(string Format, string RFU, int VarNo /*= -1*/, bool RefreshFlag /*= true */)//获取系统时间
{
	bool flag = false;//该函数内的错误标志
	string str;
	//错误检查
	if (ErrCheckFlag)
	{
		if (StrDel_ConVar(Format, str) == false)
		{
			if (str != "1" && str != "2" && str != "3" && str != "4")
			{
				Set_Error_Flag();
				serr << "获取系统时间 GETTIME（" << Format << ", " << RFU << "），未能识别指定的时间格式“" << Format << "”，有效取值为：1-“yyyy-MM-dd hh:mm:ss”，2-“yyyyMMddhhmmss”，3-“yyyy-MM-dd”，4-“yyyyMMdd”。";
				Info->Append_Error(serr.str());
				flag = true;
			}
		}
	}
	if (flag)
		return "";
	//命令构建
	sstr.clear();
	sstr.str("");
	sstr << "[GETTIME" << Style_FuntionName << "(";
	sstr << Format << "," << Style_Comma;
	sstr << RFU << ")]";
	//收尾
	if (VarNo >= 0)
		sstr << Style_Assignment << ">" << Style_Assignment << VarNo;
	if (RefreshFlag)
		Info->Append_Script(sstr.str());
	return sstr.str();
}

string CCommand::SUBTRACT(string Minuend, string Subtrahend, int VarNo /*= -1*/, bool RefreshFlag /*= true */)//减法运算
{
	bool flag = false;//该函数内的错误标志
	string str;
	//错误检查
	if (ErrCheckFlag)
	{
		StrDel_ConVar(Minuend, str);
		if (StrTrim(Minuend).empty() || JudgeDec(str) == false)
		{
			Set_Error_Flag();
			serr << "减法运算 SUBTRACT（" << Minuend << ", " << Subtrahend << "），十进制被减数“" << Minuend << "”含非法字符或长度有误。";
			Info->Append_Error(serr.str());
			flag = true;
		}
		StrDel_ConVar(Subtrahend, str);
		if (StrTrim(Subtrahend).empty() || JudgeDec(str) == false)
		{
			Set_Error_Flag();
			serr << "减法运算 SUBTRACT（" << Minuend << ", " << Subtrahend << "），十进制减数“" << Subtrahend << "”含非法字符或长度有误。";
			Info->Append_Error(serr.str());
			flag = true;
		}
	}
	if (flag)
		return "";
	//命令构建
	sstr.clear();
	sstr.str("");
	sstr << "[SUBTRACT" << Style_FuntionName << "(";
	sstr << Minuend << "," << Style_Comma;
	sstr << Subtrahend << ")]";
	//收尾
	if (VarNo >= 0)
		sstr << Style_Assignment << ">" << Style_Assignment << VarNo;
	if (RefreshFlag)
		Info->Append_Script(sstr.str());
	return sstr.str();
}

void CCommand::RETURN_Value_Add(string Tag, int VarNo, int Format)//添加输出信息
{
	bool flag = false;//该函数内的错误标志
	CReturnValue rValue;
	if (ErrCheckFlag)
	{
		if (StrTrim(Tag).empty())
		{
			serr.clear();
			serr.str("");
			serr << "添加输出信息 RETURN_Value_Add（" << Tag << ", " << VarNo << ", " << Format << "），标签值不应该为空。";
			Info->Append_Warning(serr.str());
		}
		if (VarNo < 0)
		{
			Set_Error_Flag();
			serr << "添加输出信息 RETURN_Value_Add（" << Tag << ", " << VarNo << ", " << Format << "），变量序号“" << VarNo <<"”不应该小于0。";
			Info->Append_Error(serr.str());
			flag = true;
		}
		if (Format != 0 && Format != 1 && Format != 2 && Format != 3 && Format != 4 && Format != 5 && Format != 6)
		{
			Set_Error_Flag();
			serr << "添加输出信息 RETURN_Value_Add（" << Tag << ", " << VarNo << ", " << Format << "），无法识别指定的处理格式Format“" << Format << "”，有效取值范围：0-不处理，1-an格式，2-cn格式，3-前补0的数值（若为0则返回0），4-后补F的数值（若为0则返回0），5--前补0的数值（若为0则返回空串），6-后补F的数值（若为0则返回空串）。";
			Info->Append_Error(serr.str());
			flag = true;
		}
	}
	if (flag)
		return;
	rValue.Tag = Tag;
	rValue.VarNo = VarNo;
	rValue.ForMat = Format;
	RetValue.push_back(rValue);
}

string CCommand::RETURN(int KeyType, bool RefreshFlag /*= true */)//返回输出信息
{
	bool flag = false;//该函数内的错误标志
	//错误检查
	if (ErrCheckFlag)
	{
		if (KeyType != 0 && KeyType != 1 && KeyType != 2)
		{
			Set_Error_Flag();
			serr << "返回输出信息 RETURN（" << KeyType << "），无法识别指定的密钥类型“" << KeyType << "”，有效取值范围：0-固定密钥，1-加密机，2-PSAM卡。";
			Info->Append_Error(serr.str());
			flag = true;
		}
	}
	if (flag)
		return "";
	//命令构建
	sstr.clear();
	sstr.str("");
	sstr << "[RETURN" << Style_FuntionName << "(";
	sstr << "Key|" << KeyType << "|0|$";
	for (unsigned int i = 0; i < RetValue.size(); ++i)
		sstr << Style_Comma << RetValue.at(i).Tag << "|VAR" << RetValue.at(i).VarNo << "|" << RetValue.at(i).ForMat << "|$";
	sstr << ")]";
	//收尾
	RetValue.clear();
	if (RefreshFlag)
		Info->Append_Script(sstr.str());
	return sstr.str();
}



