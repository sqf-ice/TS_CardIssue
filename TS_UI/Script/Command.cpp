//#####################################
//
//	class CCommand
//
//	�����ࣨ�ű���Լ�е����
//
//	�������ű���Լ�е���Ӧ�����ָ���޹�
//
//	ע�⣺
//	���й�������ķ����ж����в��� RefreshFlag �����ò���ֵΪtrueʱ����������ָ����Զ���������ɵĽű������У�
//	�ò���һ��Ĭ��Ϊtrue����Ĭ��Ϊ�Զ���ӣ����и��𷽷���Ĭ��ֵΪfalse��ʹ��ʱ�������⡣
//
//	����޸����ڣ�2015-09-01 18:52
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

void CCommand::ReSetErrFlag()//��������־
{
	ErrFlag = false;
}

void CCommand::Set_Error_Flag()//���ô����־
{
	ErrFlag = true;
	serr.clear();
	serr.str("");
}

bool CCommand::Error()//������
{
	return ErrFlag;
}

void CCommand::Error_Check(bool Swf)//���ش�����
{
	ErrCheckFlag = Swf;
}

void CCommand::Style_Set(unsigned char Sty)//���ýű���ʽ
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

bool CCommand::CheckData(string Data, int Len)//���ݼ��
{
	string str;
	if (StrDel_ConVar(Data, str) == true)//���г��������
	{
		if (JudgeHex(str) == false)//���зǷ��ַ�
			return false;
		else
			return true;
	}
	else//�������������
	{
		if (JudgeHex(str) == false)//���зǷ��ַ�
			return false;
		else//���ж�Len����
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
�����
***********************************************/

string CCommand::VAR(string Data, int No, bool RefreshFlag /*= true */)//������ֵ
{
	bool flag = false;//�ú����ڵĴ����־
	stringstream sstr;
	//������
	if (ErrCheckFlag)
	{
		if (No < 0)
		{
			Set_Error_Flag();
			serr << "������ֵ VAR��" << Data << ", " << No << "����������Ų���С��0��";
			Info->Append_Error(serr.str());
			flag = true;
		}
	}
	if (flag)
		return "";
	//�����
	sstr.clear();
	sstr.str("");
	sstr << Data << Style_Assignment << ">" << Style_Assignment << No;
	//��β
	if (RefreshFlag)
		Info->Append_Script(sstr.str());
	return sstr.str();
}

string CCommand::RESET(bool RefreshFlag /*= true */)//��λ
{
	if (RefreshFlag)
		Info->Append_Script("RESET");
	return "RESET";
}

string CCommand::ATR(int Pos, int Len, int VarNo /*= -1*/, bool RefreshFlag /*= true */)//��ȡATR
{
	bool flag = false;//�ú����ڵĴ����־
	//������
	if (ErrCheckFlag)
	{
		if (Pos < 0 || Len < 0)
		{
			Set_Error_Flag();
			serr << "��λӦ�� ATR��" << Pos << ", " << Len << ", " << VarNo << "������ʼƫ�ơ�" << Pos << "�����ȡ���ȡ�" << Len << "��С��0��";
			Info->Append_Error(serr.str());
			flag = true;
		}
	}
	if (flag)
		return "";
	//�����
	sstr.clear();
	sstr.str("");
	sstr << "[ATR" << Style_FuntionName << "(";
	sstr << Pos << "," << Style_Comma;
	sstr << Len << ")]";
	//��β
	if (VarNo >= 0)
		sstr << Style_Assignment << ">" << Style_Assignment << VarNo;
	if (RefreshFlag)
		Info->Append_Script(sstr.str());
	return sstr.str();
}

string CCommand::DES(string Data, string Key, int VarNo /*= -1*/, bool RefreshFlag /*= true */)//DES�㷨
{
	bool flag = false;//�ú����ڵĴ����־
	string str;
	//������
	if (ErrCheckFlag)
	{
		if (CheckData(Data, 16) == false)
		{
			Set_Error_Flag();
			serr << "DES�㷨 DES��" << Data << ", " << Key << ", " << VarNo << "�������������ݡ�" << Data << "���ı�����16��ʮ�������ַ���ɡ�";
			Info->Append_Error(serr.str());
			flag = true;
		}
		if (CheckData(Key, 16) == false)
		{
			Set_Error_Flag();
			serr << "DES�㷨 DES��" << Data << ", " << Key << ", " << VarNo << "����ʹ����Կ��" << Key << "���ı�����16��ʮ�������ַ���ɡ�";
			Info->Append_Error(serr.str());
			flag = true;
		}
	}
	if (flag)
		return "";
	//�����
	sstr.clear();
	sstr.str("");
	sstr << "[DES" << Style_FuntionName << "(";
	sstr << Data << "," << Style_Comma;
	sstr << Key << ")]";
	//��β
	if (VarNo >= 0)
		sstr << Style_Assignment << ">" << Style_Assignment << VarNo;
	if (RefreshFlag)
		Info->Append_Script(sstr.str());
	return sstr.str();
}

string CCommand::TDES(string Data, string Key, int VarNo /*= -1*/, bool RefreshFlag /*= true */)//3DES�㷨
{
	bool flag = false;//�ú����ڵĴ����־
	string str;
	//������
	if (ErrCheckFlag)
	{
		if (CheckData(Data, 16) == false)
		{
			Set_Error_Flag();
			serr << "3DES�㷨 TDES��" << Data << ", " << Key << ", " << VarNo << "�������������ݡ�" << Data << "���ı�����16��ʮ�������ַ���ɡ�";
			Info->Append_Error(serr.str());
			flag = true;
		}
		if (CheckData(Key, 32) == false)
		{
			Set_Error_Flag();
			serr << "3DES�㷨 TDES��" << Data << ", " << Key << ", " << VarNo << "����ʹ����Կ��" << Key << "���ı�����32��ʮ�������ַ���ɡ�";
			Info->Append_Error(serr.str());
			flag = true;
		}
	}
	if (flag)
		return "";
	//�����
	sstr.clear();
	sstr.str("");
	sstr << "[3DES" << Style_FuntionName << "(";
	sstr << Data << "," << Style_Comma;
	sstr << Key << ")]";
	//��β
	if (VarNo >= 0)
		sstr << Style_Assignment << ">" << Style_Assignment << VarNo;
	if (RefreshFlag)
		Info->Append_Script(sstr.str());
	return sstr.str();
}

std::string CCommand::XOR(string Par1, string Par2, int VarNo /*= -1*/, bool RefreshFlag /*= true */)//�������
{
	bool flag = false;//�ú����ڵĴ����־
	string str;
	//������
	if (ErrCheckFlag)
	{
		if (Par1.empty() || CheckData(Par1, 0) == false)
		{
			Set_Error_Flag();
			serr << "������� XOR��" << Par1 << ", " << Par2 << "��������1��" << Par1 << "�����Ƿ��ַ��򳤶Ȳ���ȷ��";
			Info->Append_Error(serr.str());
			flag = true;
		}
		if (Par2.empty() || CheckData(Par2, 0) == false)
		{
			Set_Error_Flag();
			serr << "������� XOR��" << Par1 << ", " << Par2 << "��������2��" << Par2 << "�����Ƿ��ַ��򳤶Ȳ���ȷ��";
			Info->Append_Error(serr.str());
			flag = true;
		}
	}
	if (flag)
		return "";
	//�����
	sstr.clear();
	sstr.str("");
	sstr << "[XOR" << Style_FuntionName << "(";
	sstr << Par1 << "," << Style_Comma;
	sstr << Par2 << ")]";
	//��β
	if (VarNo >= 0)
		sstr << Style_Assignment << ">" << Style_Assignment << VarNo;
	if (RefreshFlag)
		Info->Append_Script(sstr.str());
	return sstr.str();
}

string CCommand::CMAC(string Data, int Pos, int Len, string Key, int VarNo /*= -1*/, bool RefreshFlag /*= true */)//����MAC
{
	bool flag = false;//�ú����ڵĴ����־
	string str;
	//������
	if (ErrCheckFlag)
	{
		if (StrDel_ConVar(Data, str) == false)
		{
			if (str.empty() || str.length() % 2 != 0)
			{
				Set_Error_Flag();
				serr << "���㰲ȫ�������� MAC��" << Data << ", " << Pos << ", " << Len << ", " << Key << "����Դ���ݡ�" << Data << "�����Ƿ��ַ��򳤶Ȳ���ȷ��";
				Info->Append_Error(serr.str());
				flag = true;
			}
			if ((int)str.length() < Pos + Len)
			{
				Set_Error_Flag();
				serr << "���㰲ȫ�������� MAC��" << Data << ", " << Pos << ", " << Len << ", " << Key << "����ָ������ʼƫ�ơ�" << Pos << "�������㳤�ȡ�" << Len << "��������ԭ�ַ����ķ�Χ��";
				Info->Append_Error(serr.str());
				flag = true;
			}
		}
		if ((Pos < 0) || (Len < 0))
		{
			Set_Error_Flag();
			serr << "���㰲ȫ�������� MAC��" << Data << ", " << Pos << ", " << Len << ", " << Key << "������ʼƫ�ơ�" << Pos << "�������㳤�ȡ�" << Len << "��С��0��";
			Info->Append_Error(serr.str());
			flag = true;
		}
		if (CheckData(Key, 32) == false)
		{
			Set_Error_Flag();
			serr << "���㰲ȫ�������� MAC��" << Data << ", " << Pos << ", " << Len << ", " << Key << "����ʹ����Կ��" << Key << "��������32��ʮ�������ַ���ɡ�";
			Info->Append_Error(serr.str());
			flag = true;
		}
	}
	if (flag)
		return "";
	//�����
	sstr.clear();
	sstr.str("");
	sstr << Data << " [CMAC" << Style_FuntionName << "(";
	sstr << Pos << "," << Style_Comma;
	sstr << Len << "," << Style_Comma;
	sstr << Key << ")]";
	//��β
	if (VarNo >= 0)
		sstr << Style_Assignment << ">" << Style_Assignment << VarNo;
	if (RefreshFlag)
		Info->Append_Script(sstr.str());
	return sstr.str();
}

string CCommand::SUBSTR(int Pos, int Len, string Data, int VarNo /*= -1*/, bool RefreshFlag /*= true */)//�ַ�����ȡ
{
	bool flag = false;//�ú����ڵĴ����־
	string str;
	//������
	if (ErrCheckFlag)
	{
		if ((Pos < 0) || (Len < 0))
		{
			Set_Error_Flag();
			serr << "�ַ�����ȡ SUBSTR��" << Pos << ", " << Len << ", " << Data << "������ʼƫ�ơ�" << Pos << "�������㳤�ȡ�" << Len << "��С��0��";
			Info->Append_Error(serr.str());
			flag = true;
		}
		if (StrDel_ConVar(Data, str) == false)
		{
			if (str.empty() || str.length() % 2 != 0)
			{
				Set_Error_Flag();
				serr << "�ַ�����ȡ SUBSTR��" << Pos << ", " << Len << ", " << Data << "����Դ���ݡ�" << Data << "�����Ƿ��ַ��򳤶Ȳ���ȷ��";
				Info->Append_Error(serr.str());
				flag = true;
			}
			if ((int)str.length() < Pos + Len)
			{
				Set_Error_Flag();
				serr << "�ַ�����ȡ SUBSTR��" << Pos << ", " << Len << ", " << Data << "����ָ������ʼƫ�ơ�" << Pos << "�������㳤�ȡ�" << Len << "��������ԭ�ַ����ķ�Χ��";
				Info->Append_Error(serr.str());
				flag = true;
			}
		}
	}
	if (flag)
		return "";
	//�����
	sstr.clear();
	sstr.str("");
	sstr << "[SUBSTR" << Style_FuntionName << "(";
	sstr << Pos << "," << Style_Comma;
	sstr << Len << "," << Style_Comma;
	sstr << Data << ")]";
	//��β
	if (VarNo >= 0)
		sstr << Style_Assignment << ">" << Style_Assignment << VarNo;
	if (RefreshFlag)
		Info->Append_Script(sstr.str());
	return sstr.str();
}

string CCommand::RESP(int Pos, int Len, int VarNo /*= -1*/, bool RefreshFlag /*= true */)//ȡ��ƬӦ������
{
	bool flag = false;//�ú����ڵĴ����־
	//������
	if (ErrCheckFlag)
	{
		if ((Pos < 0) || (Len < 0))
		{
			Set_Error_Flag();
			serr << "ȡӦ�� RESP��" << Pos << ", " << Len << "������ʼƫ�ơ�" << Pos << "�������㳤�ȡ�" << Len << "��С��0��";
			Info->Append_Error(serr.str());
			flag = true;
		}
	}
	if (flag)
		return "";
	//�����
	sstr.clear();
	sstr.str("");
	sstr << "[RESP" << Style_FuntionName << "(";
	sstr << Pos << "," << Style_Comma;
	sstr << Len << ")]";
	//��β
	if (VarNo >= 0)
		sstr << Style_Assignment << ">" << Style_Assignment << VarNo;
	if (RefreshFlag)
		Info->Append_Script(sstr.str());
	return sstr.str();
}

string CCommand::CONNECTJMJ(string DevType, string IP, string Port, bool RefreshFlag /*= true */)//���Ӽ��ܻ�
{
	bool flag = false;//�ú����ڵĴ����־
	string str;
	string::size_type pos, offset;
	string stemp;
	//������
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
					serr << "���Ӽ��ܻ� CONNECTJMJ��" << DevType << ", " << IP << ", " << Port << "����ָ�����ܻ���IPֵ��" << IP << "���Ƿ���";
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
				serr << "���Ӽ��ܻ� CONNECTJMJ��" << DevType << ", " << IP << ", " << Port << "����ָ�����ܻ��Ķ˿�ֵ��" << Port << "���Ƿ���";
				Info->Append_Error(serr.str());
				flag = true;
			}
		}
	}
	if (flag)
		return "";
	//�����
	sstr.clear();
	sstr.str("");
	sstr << "[CONNECTJMJ" << Style_FuntionName << "(";
	sstr << DevType << "," << Style_Comma;
	sstr << IP << "," << Style_Comma;
	sstr << Port << ")]";
	//��β
	if (RefreshFlag)
		Info->Append_Script(sstr.str());
	return sstr.str();
}

string CCommand::CLOSEJMJ(bool RefreshFlag /*= true*/)
{
	//�����
	sstr.clear();
	sstr.str("");
	sstr << "[CLOSEJMJ]";
	//��β
	if (RefreshFlag)
		Info->Append_Script(sstr.str());
	return sstr.str();
}

string CCommand::LOADKEY(string CosManu, string Stowage, string KeyNo, string PKeyNo, string Keyfsjs, string PKeyfsjs, string Keyfsyz, string PKeyfsyz, string Comm, string KeyHead, string KeyLen, string PKeyLen, bool RefreshFlag /*= true */)//װ����Կ
{
	bool flag = false;//�ú����ڵĴ����־
	string str;
	//������
	if (ErrCheckFlag)
	{
		if (StrDel_ConVar(CosManu, str) == false)
		{
			if (str != "0" && str != "1" && str != "3" && str != "4" && str != "5" && str != "6" && str != "7" && str != "8" && str != "20")
			{
				serr.clear();
				serr.str("");
				serr << "װ����Կ LOADKEY��" << CosManu << ", " << Stowage << ", " << KeyNo << ", " << PKeyNo << ", " << Keyfsjs << ", " << PKeyfsjs << ", " << Keyfsyz << ", " << PKeyfsyz << ", " << Comm << ", " << KeyHead << ", " << KeyLen << ", " << PKeyLen << "�����޷�ʶ��ָ����COS���̡�" << CosManu << "����";
				Info->Append_Warning(serr.str());
			}
		}
		if (StrDel_ConVar(Stowage, str) == false)
		{
			if (str != "0" && str != "1" && str != "10" && str != "11")
			{
				serr.clear();
				serr.str("");
				serr << "װ����Կ LOADKEY��" << CosManu << ", " << Stowage << ", " << KeyNo << ", " << PKeyNo << ", " << Keyfsjs << ", " << PKeyfsjs << ", " << Keyfsyz << ", " << PKeyfsyz << ", " << Comm << ", " << KeyHead << ", " << KeyLen << ", " << PKeyLen << "�����޷�ʶ��ָ������Կװ�ط�ʽ��" << Stowage << "����";
				Info->Append_Warning(serr.str());
			}
		}
		if (StrTrim(KeyNo).empty() || CheckData(KeyNo, -1) == false)
		{
			Set_Error_Flag();
			serr << "װ����Կ LOADKEY��" << CosManu << ", " << Stowage << ", " << KeyNo << ", " << PKeyNo << ", " << Keyfsjs << ", " << PKeyfsjs << ", " << Keyfsyz << ", " << PKeyfsyz << ", " << Comm << ", " << KeyHead << ", " << KeyLen << ", " << PKeyLen << "����ָ������Կ��ַ��" << KeyNo << "������";
			Info->Append_Error(serr.str());
			flag = true;
		}
		if (StrTrim(PKeyNo).empty() || CheckData(PKeyNo, -1) == false)
		{
			Set_Error_Flag();
			serr << "װ����Կ LOADKEY��" << CosManu << ", " << Stowage << ", " << KeyNo << ", " << PKeyNo << ", " << Keyfsjs << ", " << PKeyfsjs << ", " << Keyfsyz << ", " << PKeyfsyz << ", " << Comm << ", " << KeyHead << ", " << KeyLen << ", " << PKeyLen << "����ָ���ı�����Կ��ַ��" << PKeyNo << "������";
			Info->Append_Error(serr.str());
			flag = true;
		}
		if (StrDel_ConVar(Keyfsjs, str) == false)
		{
			if (str != "1" && str != "2" && str != "3")
			{
				serr.clear();
				serr.str("");
				serr << "װ����Կ LOADKEY��" << CosManu << ", " << Stowage << ", " << KeyNo << ", " << PKeyNo << ", " << Keyfsjs << ", " << PKeyfsjs << ", " << Keyfsyz << ", " << PKeyfsyz << ", " << Comm << ", " << KeyHead << ", " << KeyLen << ", " << PKeyLen << "�����޷�ʶ��ָ���ķ�ɢ������" << Keyfsjs << "����";
				Info->Append_Warning(serr.str());
			}
		}
		if (StrDel_ConVar(PKeyfsjs, str) == false)
		{
			if (str != "0" && str != "1" && str != "2" && str != "3")
			{
				serr.clear();
				serr.str("");
				serr << "װ����Կ LOADKEY��" << CosManu << ", " << Stowage << ", " << KeyNo << ", " << PKeyNo << ", " << Keyfsjs << ", " << PKeyfsjs << ", " << Keyfsyz << ", " << PKeyfsyz << ", " << Comm << ", " << KeyHead << ", " << KeyLen << ", " << PKeyLen << "�����޷�ʶ��ָ���ı�����Կ��ɢ������" << PKeyfsjs << "����";
				Info->Append_Warning(serr.str());
			}
		}
		if (CheckData(Keyfsyz, 16) == false)
		{
			Set_Error_Flag();
			serr << "װ����Կ LOADKEY��" << CosManu << ", " << Stowage << ", " << KeyNo << ", " << PKeyNo << ", " << Keyfsjs << ", " << PKeyfsjs << ", " << Keyfsyz << ", " << PKeyfsyz << ", " << Comm << ", " << KeyHead << ", " << KeyLen << ", " << PKeyLen << "������ɢ���ӡ�" << Keyfsyz << "��������16��ʮ�������ַ���ɡ�";
			Info->Append_Error(serr.str());
			flag = true;
		}
		if (StrTrim(PKeyfsyz).empty() || CheckData(PKeyfsyz, -1) == false)
		{
			Set_Error_Flag();
			serr << "װ����Կ LOADKEY��" << CosManu << ", " << Stowage << ", " << KeyNo << ", " << PKeyNo << ", " << Keyfsjs << ", " << PKeyfsjs << ", " << Keyfsyz << ", " << PKeyfsyz << ", " << Comm << ", " << KeyHead << ", " << KeyLen << ", " << PKeyLen << "����������Կ��ɢ���ӡ�" << Keyfsyz << "�����зǷ��ַ��򳤶�����";
			Info->Append_Error(serr.str());
			flag = true;
		}
		if (CheckData(Comm, 10) == false)
		{
			serr.clear();
			serr.str("");
			serr << "װ����Կ LOADKEY��" << CosManu << ", " << Stowage << ", " << KeyNo << ", " << PKeyNo << ", " << Keyfsjs << ", " << PKeyfsjs << ", " << Keyfsyz << ", " << PKeyfsyz << ", " << Comm << ", " << KeyHead << ", " << KeyLen << ", " << PKeyLen << "����ָ��ͷ����" << Comm << "��һ��Ҫ����10��ʮ�������ַ���ɡ�";
			Info->Append_Warning(serr.str());
		}
		if (StrTrim(KeyHead).empty() || CheckData(KeyHead, 0) == false)
		{
			Set_Error_Flag();
			serr << "װ����Կ LOADKEY��" << CosManu << ", " << Stowage << ", " << KeyNo << ", " << PKeyNo << ", " << Keyfsjs << ", " << PKeyfsjs << ", " << Keyfsyz << ", " << PKeyfsyz << ", " << Comm << ", " << KeyHead << ", " << KeyLen << ", " << PKeyLen << "������Կͷ����" << KeyHead << "�����зǷ��ַ����߳�������";
			Info->Append_Error(serr.str());
			flag = true;
		}
		if (StrDel_ConVar(KeyLen, str) == false)
		{
			if (str != "0" && str != "1")
			{
				serr.clear();
				serr.str("");
				serr << "װ����Կ LOADKEY��" << CosManu << ", " << Stowage << ", " << KeyNo << ", " << PKeyNo << ", " << Keyfsjs << ", " << PKeyfsjs << ", " << Keyfsyz << ", " << PKeyfsyz << ", " << Comm << ", " << KeyHead << ", " << KeyLen << ", " << PKeyLen << "����������Կ���ֽڳ���ֻ��ָ��Ϊ0����ʾ8�ֽڣ���1����ʾ16�ֽڣ�������ǰ�ǡ�" << KeyLen << "����";
				Info->Append_Warning(serr.str());
			}
		}
		if (StrDel_ConVar(PKeyLen, str) == false)
		{
			if (str != "0" && str != "1")
			{
				serr.clear();
				serr.str("");
				serr << "װ����Կ LOADKEY��" << CosManu << ", " << Stowage << ", " << KeyNo << ", " << PKeyNo << ", " << Keyfsjs << ", " << PKeyfsjs << ", " << Keyfsyz << ", " << PKeyfsyz << ", " << Comm << ", " << KeyHead << ", " << KeyLen << ", " << PKeyLen << "����������Կ���ֽڳ���ֻ��ָ��Ϊ0����ʾ8�ֽڣ���1����ʾ16�ֽڣ�������ǰ�ǡ�" << PKeyLen << "����";
				Info->Append_Warning(serr.str());
			}
		}
	}
	if (flag)
		return "";
	//�����
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
	//��β
	if (RefreshFlag)
		Info->Append_Script(sstr.str());
	return sstr.str();
}

string CCommand::SETNAD(string CardNad, bool RefreshFlag /*= true */)//���ÿ���
{
	bool flag = false;//�ú����ڵĴ����־
	string str;
	//������
	if (ErrCheckFlag)
	{
		if (StrDel_ConVar(CardNad, str) == false)
		{
			if (str != "0" && str != "1" && str != "2")
			{
				serr.clear();
				serr.str("");
				serr << "���ÿ��� SETNAD��" << CardNad << "�����޷�ʶ��ָ���Ŀ�����š�" << CardNad << "����";
				Info->Append_Warning(serr.str());
			}
		}
	}
	if (flag)
		return "";
	//�����
	sstr.clear();
	sstr.str("");
	sstr << "[SETNAD" << Style_FuntionName << "(";
	sstr << CardNad << ")]";
	//��β
	if (RefreshFlag)
		Info->Append_Script(sstr.str());
	return sstr.str();
}

string CCommand::GERNERALEXTERNALAUTH(string AuthType, string DF, string EF, string KeyIndex, string KeyVer, string DivFactor, string PSAMKeyType, string PSAMorJMJKeyID, string ConstKey, string Flag, bool RefreshFlag /*= true */)//�ⲿ��֤
{
	bool flag = false;//�ú����ڵĴ����־
	string str;
	int auth = -1;
	//������
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
				serr << "�ⲿ��֤ GERNERALEXTERNALAUTH��" << AuthType << ", " << DF << ", " << EF << ", " << KeyIndex << ", " << KeyVer << ", " << DivFactor << ", " << PSAMKeyType << ", " << PSAMorJMJKeyID << ", " << ConstKey << ", " << Flag << "������֤��ʽֻ��ָ��Ϊ0���̶���Կ����1�����ܻ�����2��PSAM����������ǰ�ǡ�" << AuthType << "����";
				Info->Append_Warning(serr.str());
			}
		}
		if (CheckData(DF, 4) == false)
		{
			Set_Error_Flag();
			serr << "�ⲿ��֤ GERNERALEXTERNALAUTH��" << AuthType << ", " << DF << ", " << EF << ", " << KeyIndex << ", " << KeyVer << ", " << DivFactor << ", " << PSAMKeyType << ", " << PSAMorJMJKeyID << ", " << ConstKey << ", " << Flag << "����ָ������֤��Կ����Ŀ¼��" << DF << "��������4��ʮ�������ַ���ɡ�";
			Info->Append_Error(serr.str());
			flag = true;
		}
		if (CheckData(EF, 4) == false)
		{
			Set_Error_Flag();
			serr << "�ⲿ��֤ GERNERALEXTERNALAUTH��" << AuthType << ", " << DF << ", " << EF << ", " << KeyIndex << ", " << KeyVer << ", " << DivFactor << ", " << PSAMKeyType << ", " << PSAMorJMJKeyID << ", " << ConstKey << ", " << Flag << "����ָ����EF�ļ���" << EF << "��������4��ʮ�������ַ���ɡ�";
			Info->Append_Error(serr.str());
			flag = true;
		}
		if (CheckData(KeyIndex, 2) == false)
		{
			Set_Error_Flag();
			serr << "�ⲿ��֤ GERNERALEXTERNALAUTH��" << AuthType << ", " << DF << ", " << EF << ", " << KeyIndex << ", " << KeyVer << ", " << DivFactor << ", " << PSAMKeyType << ", " << PSAMorJMJKeyID << ", " << ConstKey << ", " << Flag << "����ָ������Կ��־��" << KeyIndex << "��������2��ʮ�������ַ���ɡ�";
			Info->Append_Error(serr.str());
			flag = true;
		}
		if (CheckData(KeyVer, 2) == false)
		{
			Set_Error_Flag();
			serr << "�ⲿ��֤ GERNERALEXTERNALAUTH��" << AuthType << ", " << DF << ", " << EF << ", " << KeyIndex << ", " << KeyVer << ", " << DivFactor << ", " << PSAMKeyType << ", " << PSAMorJMJKeyID << ", " << ConstKey << ", " << Flag << "����ָ������Կ�汾��" << KeyVer << "��������2��ʮ�������ַ���ɡ�";
			Info->Append_Error(serr.str());
			flag = true;
		}
		switch (auth)
		{
		case 0:
			if (StrTrim(DivFactor).empty())
			{
				Set_Error_Flag();
				serr << "�ⲿ��֤ GERNERALEXTERNALAUTH��" << AuthType << ", " << DF << ", " << EF << ", " << KeyIndex << ", " << KeyVer << ", " << DivFactor << ", " << PSAMKeyType << ", " << PSAMorJMJKeyID << ", " << ConstKey << ", " << Flag << "������ʹ��ʹ�ù̶���Կ����֤��ʽ����ɢ���ӡ�" << DivFactor << "��Ҳ��Ӧ��Ϊ���ַ��������Դ��롰00����";
				Info->Append_Error(serr.str());
				flag = true;
			}
			break;
		case 1:
			if (CheckData(DivFactor, 16) == false)
			{
				Set_Error_Flag();
				serr << "�ⲿ��֤ GERNERALEXTERNALAUTH��" << AuthType << ", " << DF << ", " << EF << ", " << KeyIndex << ", " << KeyVer << ", " << DivFactor << ", " << PSAMKeyType << ", " << PSAMorJMJKeyID << ", " << ConstKey << ", " << Flag << "������ʹ�ü��ܻ�����֤��ʽʱ����ɢ���ӡ�" << DivFactor << "��������16��ʮ�������ַ���ɡ�";
				Info->Append_Error(serr.str());
				flag = true;
			}
			break;
		case 2:
			if (CheckData(DivFactor, 48) == false)
			{
				Set_Error_Flag();
				serr << "�ⲿ��֤ GERNERALEXTERNALAUTH��" << AuthType << ", " << DF << ", " << EF << ", " << KeyIndex << ", " << KeyVer << ", " << DivFactor << ", " << PSAMKeyType << ", " << PSAMorJMJKeyID << ", " << ConstKey << ", " << Flag << "������ʹ�ü��ܻ�����֤��ʽʱ����ɢ���ӡ�" << DivFactor << "��������48��ʮ�������ַ���ɡ�";
				Info->Append_Error(serr.str());
				flag = true;
			}
			break;
		default:
			if (StrTrim(DivFactor).empty() || CheckData(DivFactor, 0) == false)
			{
				Set_Error_Flag();
				serr << "�ⲿ��֤ GERNERALEXTERNALAUTH��" << AuthType << ", " << DF << ", " << EF << ", " << KeyIndex << ", " << KeyVer << ", " << DivFactor << ", " << PSAMKeyType << ", " << PSAMorJMJKeyID << ", " << ConstKey << ", " << Flag << "������ɢ���ӡ�" << KeyVer << "�����зǷ��ַ��򳤶�����";
				Info->Append_Error(serr.str());
				flag = true;
			}
		}
		if (auth != 2 && StrTrim(PSAMKeyType).empty())
		{
			Set_Error_Flag();
			serr << "�ⲿ��֤ GERNERALEXTERNALAUTH��" << AuthType << ", " << DF << ", " << EF << ", " << KeyIndex << ", " << KeyVer << ", " << DivFactor << ", " << PSAMKeyType << ", " << PSAMorJMJKeyID << ", " << ConstKey << ", " << Flag << "�������²�ʹ��PSAM������֤��ʽ��PSAM����Կ��;��" << PSAMKeyType << "��Ҳ��Ӧ��Ϊ���ַ��������Դ��롰00����";
			Info->Append_Error(serr.str());
			flag = true;
		}
		if (auth == 2 && CheckData(PSAMKeyType, 2) == false)
		{
			Set_Error_Flag();
			serr << "�ⲿ��֤ GERNERALEXTERNALAUTH��" << AuthType << ", " << DF << ", " << EF << ", " << KeyIndex << ", " << KeyVer << ", " << DivFactor << ", " << PSAMKeyType << ", " << PSAMorJMJKeyID << ", " << ConstKey << ", " << Flag << "����ʹ��PSAM����֤ʱ��PSAM����Կ��;��" << PSAMKeyType << "��������2��ʮ�������ַ���ɡ�";
			Info->Append_Error(serr.str());
			flag = true;
		}
		switch (auth)
		{
		case 0:
			if (StrTrim(PSAMorJMJKeyID).empty())
			{
				Set_Error_Flag();
				serr << "�ⲿ��֤ GERNERALEXTERNALAUTH��" << AuthType << ", " << DF << ", " << EF << ", " << KeyIndex << ", " << KeyVer << ", " << DivFactor << ", " << PSAMKeyType << ", " << PSAMorJMJKeyID << ", " << ConstKey << ", " << Flag << "������ʹ��ʹ�ù̶���Կ����֤��ʽ��PSAM��Կ��־��" << PSAMorJMJKeyID << "��Ҳ��Ӧ��Ϊ���ַ��������Դ��롰00����";
				Info->Append_Error(serr.str());
				flag = true;
			}
			break;
		case 1:
			if (StrTrim(PSAMorJMJKeyID).empty() || CheckData(PSAMorJMJKeyID, -1) == false)
			{
				Set_Error_Flag();
				serr << "�ⲿ��֤ GERNERALEXTERNALAUTH��" << AuthType << ", " << DF << ", " << EF << ", " << KeyIndex << ", " << KeyVer << ", " << DivFactor << ", " << PSAMKeyType << ", " << PSAMorJMJKeyID << ", " << ConstKey << ", " << Flag << "����ʹ�ü��ܻ���֤��ʽʱ����Կ��ַ��" << PSAMorJMJKeyID << "�����Ƿ��ַ��򳤶�����";
				Info->Append_Error(serr.str());
				flag = true;
			}
			break;
		case 2:
			if (CheckData(PSAMorJMJKeyID, 2) == false)
			{
				Set_Error_Flag();
				serr << "�ⲿ��֤ GERNERALEXTERNALAUTH��" << AuthType << ", " << DF << ", " << EF << ", " << KeyIndex << ", " << KeyVer << ", " << DivFactor << ", " << PSAMKeyType << ", " << PSAMorJMJKeyID << ", " << ConstKey << ", " << Flag << "����ʹ��PSAM����֤��ʽʱ��PSAM����Կ��־��" << PSAMorJMJKeyID << "��������2��ʮ�������ַ���ɡ�";
				Info->Append_Error(serr.str());
				flag = true;
			}
			break;
		default:
			if (StrTrim(PSAMorJMJKeyID).empty() || CheckData(PSAMorJMJKeyID, -1) == false)
			{
				Set_Error_Flag();
				serr << "�ⲿ��֤ GERNERALEXTERNALAUTH��" << AuthType << ", " << DF << ", " << EF << ", " << KeyIndex << ", " << KeyVer << ", " << DivFactor << ", " << PSAMKeyType << ", " << PSAMorJMJKeyID << ", " << ConstKey << ", " << Flag << "����PSAM����Կ��־����ܻ���Կ��ַ��" << PSAMorJMJKeyID << "�����Ƿ��ַ��򳤶�����";
				Info->Append_Error(serr.str());
				flag = true;
			}
		}
		if (auth != 0 && StrTrim(ConstKey).empty())
		{
			Set_Error_Flag();
			serr << "�ⲿ��֤ GERNERALEXTERNALAUTH��" << AuthType << ", " << DF << ", " << EF << ", " << KeyIndex << ", " << KeyVer << ", " << DivFactor << ", " << PSAMKeyType << ", " << PSAMorJMJKeyID << ", " << ConstKey << ", " << Flag << "�������²�ʹ�ù̶���Կ����֤��ʽ���̶���Կֵ��" << PSAMKeyType << "��Ҳ��Ӧ��Ϊ���ַ��������Դ��롰00����";
			Info->Append_Error(serr.str());
			flag = true;
		}
		if (auth == 0 && CheckData(ConstKey, 32) == false)
		{
			Set_Error_Flag();
			serr << "�ⲿ��֤ GERNERALEXTERNALAUTH��" << AuthType << ", " << DF << ", " << EF << ", " << KeyIndex << ", " << KeyVer << ", " << DivFactor << ", " << PSAMKeyType << ", " << PSAMorJMJKeyID << ", " << ConstKey << ", " << Flag << "����ʹ�ù̶���Կ����֤��ʽʱ���̶���Կֵ��" << PSAMKeyType << "��������32��ʮ�������ַ���ɡ�";
			Info->Append_Error(serr.str());
			flag = true;
		}
	}
	if (flag)
		return "";
	//�����
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
	//��β
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

string CCommand::GETMAC(string Type, string Serial, string DivFactor, string DivType, string KeyID, string Radom, string InitData, string ProtectKey, string GetRandom, string Pad, int VarNo /*= -1*/, bool RefreshFlag /*= true */)//��MAC
{
	bool flag = false;//�ú����ڵĴ����־
	string str;
	int auth = -1;//��֤��ʽ��1-���ܻ���2-PSAM
	int rad = -1;//�Ƿ�ȡ�����
	//������
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
				serr << "��ȫ�������� GETMAC��" << Type << ", " << Serial << ", " << DivFactor << ", " << DivType << ", " << KeyID << ", " << Radom << ", " << InitData << ", " << ProtectKey << ", " << GetRandom << ", " << Pad << "�����������ͣ�1-���ܻ�������֤�룬2-PSAM������֤�룬3-���ܻ���������Կ��4-PSAM����������Կ������ǰ�ǡ�" << Type << "����";
				Info->Append_Warning(serr.str());
			}
		}
		if (StrTrim(Serial).empty())
		{
			Set_Error_Flag();
			serr << "��ȫ�������� GETMAC��" << Type << ", " << Serial << ", " << DivFactor << ", " << DivType << ", " << KeyID << ", " << Radom << ", " << InitData << ", " << ProtectKey << ", " << GetRandom << ", " << Pad << "�����ڶ���������Ӧ��Ϊ�մ������鴫�Ρ�1����";
			Info->Append_Error(serr.str());
			flag = true;
		}
		switch (auth)
		{
		case 1:
			if (CheckData(DivFactor, 16) == false)
			{
				Set_Error_Flag();
				serr << "��ȫ�������� GETMAC��" << Type << ", " << Serial << ", " << DivFactor << ", " << DivType << ", " << KeyID << ", " << Radom << ", " << InitData << ", " << ProtectKey << ", " << GetRandom << ", " << Pad << "������ʹ�ü��ܻ�ʱ����ɢ���ӡ�" << DivFactor << "��������16��ʮ�������ַ���ɡ�";
				Info->Append_Error(serr.str());
				flag = true;
			}
			break;
		case 2:
			if (CheckData(DivFactor, 48) == false)
			{
				Set_Error_Flag();
				serr << "��ȫ�������� GETMAC��" << Type << ", " << Serial << ", " << DivFactor << ", " << DivType << ", " << KeyID << ", " << Radom << ", " << InitData << ", " << ProtectKey << ", " << GetRandom << ", " << Pad << "������ʹ��PSAM��ʱ����ɢ���ӡ�" << DivFactor << "��������48��ʮ�������ַ���ɡ�";
				Info->Append_Error(serr.str());
				flag = true;
			}
			break;
		default:
			if (CheckData(DivFactor, 0) == false)
			{
				Set_Error_Flag();
				serr << "��ȫ�������� GETMAC��" << Type << ", " << Serial << ", " << DivFactor << ", " << DivType << ", " << KeyID << ", " << Radom << ", " << InitData << ", " << ProtectKey << ", " << GetRandom << ", " << Pad << "������ɢ���ӡ�" << DivFactor << "�����Ƿ��ַ��򳤶�����";
				Info->Append_Error(serr.str());
				flag = true;
			}
			break;
		}
		if (auth != 2 && StrTrim(DivType).empty())
		{
			Set_Error_Flag();
			serr << "��ȫ�������� GETMAC��" << Type << ", " << Serial << ", " << DivFactor << ", " << DivType << ", " << KeyID << ", " << Radom << ", " << InitData << ", " << ProtectKey << ", " << GetRandom << ", " << Pad << "�������²�ʹ��PSAM����PSAM����;��" << DivType << "��Ҳ��Ӧ��Ϊ���ַ��������Դ��롰00����";
			Info->Append_Error(serr.str());
			flag = true;
		}
		if (auth == 2 && StrTrim(DivType).empty())
		{
			Set_Error_Flag();
			serr << "��ȫ�������� GETMAC��" << Type << ", " << Serial << ", " << DivFactor << ", " << DivType << ", " << KeyID << ", " << Radom << ", " << InitData << ", " << ProtectKey << ", " << GetRandom << ", " << Pad << "������ʹ��PSAM��ʱ��PSAM����;��" << DivType << "��������2��ʮ�������ַ���ɡ�";
			Info->Append_Error(serr.str());
			flag = true;
		}
		switch (auth)
		{
		case 1:
			if (StrTrim(KeyID).empty() || CheckData(KeyID, -1) == false)
			{
				Set_Error_Flag();
				serr << "��ȫ�������� GETMAC��" << Type << ", " << Serial << ", " << DivFactor << ", " << DivType << ", " << KeyID << ", " << Radom << ", " << InitData << ", " << ProtectKey << ", " << GetRandom << ", " << Pad << "������ʹ�ü��ܻ�ʱ����Կ��ַ��" << DivType << "�����Ƿ��ַ��򳤶�����";
				Info->Append_Error(serr.str());
				flag = true;
			}
			break;
		case 2:
			if (CheckData(KeyID, 2) == false)
			{
				Set_Error_Flag();
				serr << "��ȫ�������� GETMAC��" << Type << ", " << Serial << ", " << DivFactor << ", " << DivType << ", " << KeyID << ", " << Radom << ", " << InitData << ", " << ProtectKey << ", " << GetRandom << ", " << Pad << "������ʹ��PSAM��ʱ��PSAM����Կ��־��" << DivType << "��������2��ʮ�������ַ���ɡ�";
				Info->Append_Error(serr.str());
				flag = true;
			}
			break;
		default:
			if (StrTrim(KeyID).empty() || CheckData(KeyID, -1) == false)
			{
				Set_Error_Flag();
				serr << "��ȫ�������� GETMAC��" << Type << ", " << Serial << ", " << DivFactor << ", " << DivType << ", " << KeyID << ", " << Radom << ", " << InitData << ", " << ProtectKey << ", " << GetRandom << ", " << Pad << "������Կ��ַ��" << DivType << "�����Ƿ��ַ��򳤶�����";
				Info->Append_Error(serr.str());
				flag = true;
			}
		}
		if (StrDel_ConVar(GetRandom, str) == false)
		{
			if (str.empty())
			{
				Set_Error_Flag();
				serr << "��ȫ�������� GETMAC��" << Type << ", " << Serial << ", " << DivFactor << ", " << DivType << ", " << KeyID << ", " << Radom << ", " << InitData << ", " << ProtectKey << ", " << GetRandom << ", " << Pad << "����ȡ�������־GetRandom��Ӧ��Ϊ�գ���0����ʾ���Զ�ȡ���������1����ʾ�Զ�ȡ�������";
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
				serr << "��ȫ�������� GETMAC��" << Type << ", " << Serial << ", " << DivFactor << ", " << DivType << ", " << KeyID << ", " << Radom << ", " << InitData << ", " << ProtectKey << ", " << GetRandom << ", " << Pad << "����ȡ�������־GetRandomָʾ����������ⲿ����ʱ��������������" << Radom << "��������16��ʮ�������ַ���ɡ�";
				Info->Append_Error(serr.str());
				flag = true;
			}
			else if (rad == 1 && str.empty())
			{
				Set_Error_Flag();
				serr << "��ȫ�������� GETMAC��" << Type << ", " << Serial << ", " << DivFactor << ", " << DivType << ", " << KeyID << ", " << Radom << ", " << InitData << ", " << ProtectKey << ", " << GetRandom << ", " << Pad << "��������ȡ�������־GetRandomָʾ�Ž��Զ���ȡ�������������������" << Radom << "��Ҳ��Ӧ��Ϊ�մ������Դ��롰00����";
				Info->Append_Error(serr.str());
				flag = true;
			}
		}
		if (StrTrim(InitData).empty() || CheckData(InitData, 0) == false)
		{
			Set_Error_Flag();
			serr << "��ȫ�������� GETMAC��" << Type << ", " << Serial << ", " << DivFactor << ", " << DivType << ", " << KeyID << ", " << Radom << ", " << InitData << ", " << ProtectKey << ", " << GetRandom << ", " << Pad << "����������MAC��ԭʼ���ݡ�" << InitData << "�����Ƿ��ַ��򳤶�����";
			Info->Append_Error(serr.str());
			flag = true;
		}
		if (StrTrim(ProtectKey).empty())
		{
			Set_Error_Flag();
			serr << "��ȫ�������� GETMAC��" << Type << ", " << Serial << ", " << DivFactor << ", " << DivType << ", " << KeyID << ", " << Radom << ", " << InitData << ", " << ProtectKey << ", " << GetRandom << ", " << Pad << "���������Ƿ�ʹ�ü��ܻ���������Կ����ӦΪ�գ����鴫�롰1����";
			Info->Append_Error(serr.str());
			flag = true;
		}
	}
	if (flag)
		return "";
	//�����
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
	//��β
	if (VarNo >= 0)
		sstr << Style_Assignment << ">" << Style_Assignment << VarNo;
	if (RefreshFlag)
		Info->Append_Script(sstr.str());
	return sstr.str();
}

string CCommand::PRODUCT(string Par1, string Par2, int VarNo /*= -1*/, bool RefreshFlag /*= true */)//�˷�����
{
	bool flag = false;//�ú����ڵĴ����־
	string str;
	//������
	if (ErrCheckFlag)
	{
		StrDel_ConVar(Par1, str);
		if (StrTrim(Par1).empty() || JudgeDec(str) == false)
		{
			Set_Error_Flag();
			serr << "�˷����� PRODUCT��" << Par1 << ", " << Par2 << "��������1��" << Par1 << "��ֻ����ʮ����������ɡ�";
			Info->Append_Error(serr.str());
			flag = true;
		}
		StrDel_ConVar(Par2, str);
		if (StrTrim(Par2).empty() || JudgeDec(str) == false)
		{
			Set_Error_Flag();
			serr << "�˷����� PRODUCT��" << Par1 << ", " << Par2 << "��������2��" << Par2 << "��ֻ����ʮ����������ɡ�";
			Info->Append_Error(serr.str());
			flag = true;
		}
	}
	if (flag)
		return "";
	//�����
	sstr.clear();
	sstr.str("");
	sstr << "[PRODUCT" << Style_FuntionName << "(";
	sstr << Par1 << "," << Style_Comma;
	sstr << Par2 << ")]";
	//��β
	if (VarNo >= 0)
		sstr << Style_Assignment << ">" << Style_Assignment << VarNo;
	if (RefreshFlag)
		Info->Append_Script(sstr.str());
	return sstr.str();
}

string CCommand::HEX(string Decimal, int VarNo /*= -1*/, bool RefreshFlag /*= true */)//ʮ���ƴ�ת����ʮ�����ƴ�
{
	bool flag = false;//�ú����ڵĴ����־
	string str;
	//������
	if (ErrCheckFlag)
	{
		StrDel_ConVar(Decimal, str);
		if (StrTrim(Decimal).empty() || JudgeDec(str) == false)
		{
			Set_Error_Flag();
			serr << "ʮ���ƴ�ת����ʮ�����ƴ� HEX��" << Decimal << "������ת����ʮ�������ݡ�" << Decimal << "�����Ƿ��ַ��򳤶�����";
			Info->Append_Error(serr.str());
			flag = true;
		}
	}
	if (flag)
		return "";
	//�����
	sstr.clear();
	sstr.str("");
	sstr << "[HEX" << Style_FuntionName << "(";
	sstr << Decimal << ")]";
	//��β
	if (VarNo >= 0)
		sstr << Style_Assignment << ">" << Style_Assignment << VarNo;
	if (RefreshFlag)
		Info->Append_Script(sstr.str());
	return sstr.str();
}

string CCommand::DHEX(string Hexadecimal, int VarNo /*= -1*/, bool RefreshFlag /*= true */)//ʮ�����ƴ�ת����ʮ���ƴ�
{
	bool flag = false;//�ú����ڵĴ����־
	string str;
	//������
	if (ErrCheckFlag)
	{
		StrDel_ConVar(Hexadecimal, str);
		if (StrTrim(Hexadecimal).empty() || JudgeHex(str) == false)
		{
			Set_Error_Flag();
			serr << "ʮ�����ƴ�ת����ʮ���ƴ� DHEX��" << Hexadecimal << "������ת����ʮ���������ݡ�" << Hexadecimal << "�����Ƿ��ַ��򳤶�����";
			Info->Append_Error(serr.str());
			flag = true;
		}
	}
	if (flag)
		return "";
	//�����
	sstr.clear();
	sstr.str("");
	sstr << "[DHEX" << Style_FuntionName << "(";
	sstr << Hexadecimal << ")]";
	//��β
	if (VarNo >= 0)
		sstr << Style_Assignment << ">" << Style_Assignment << VarNo;
	if (RefreshFlag)
		Info->Append_Script(sstr.str());
	return sstr.str();
}

string CCommand::EX_HEX(string Decimal, string Len, int VarNo /*= -1*/, bool RefreshFlag /*= true */)//ʮ���ƴ�ת��Ϊʮ�����ƴ�
{
	bool flag = false;//�ú����ڵĴ����־
	string str;
	//������
	if (ErrCheckFlag)
	{
		StrDel_ConVar(Decimal, str);
		if (StrTrim(Decimal).empty() || JudgeDec(str) == false)
		{
			Set_Error_Flag();
			serr << "ʮ���ƴ�ת��Ϊʮ�����ƴ� EX_HEX��" << Decimal << ", " << Len << "������ת����ʮ�������ݡ�" << Decimal << "�����Ƿ��ַ��򳤶�����";
			Info->Append_Error(serr.str());
			flag = true;
		}
		StrDel_ConVar(Len, str);
		if (StrTrim(Len).empty() || JudgeDec(str) == false)
		{
			Set_Error_Flag();
			serr << "ʮ���ƴ�ת��Ϊʮ�����ƴ� EX_HEX��" << Decimal << ", " << Len << "����ָ����ת���󳤶ȡ�" << Len << "��ֻ����ʮ����������ɡ�";
			Info->Append_Error(serr.str());
			flag = true;
		}
	}
	if (flag)
		return "";
	//�����
	sstr.clear();
	sstr.str("");
	sstr << "[EX_HEX" << Style_FuntionName << "(";
	sstr << Decimal << "," << Style_Comma;
	sstr << Len << ")]";
	//��β
	if (VarNo >= 0)
		sstr << Style_Assignment << ">" << Style_Assignment << VarNo;
	if (RefreshFlag)
		Info->Append_Script(sstr.str());
	return sstr.str();
}

string CCommand::LEN(string Data, int VarNo /*= -1*/, bool RefreshFlag /*= true */)//����ʮ�����Ʊ�ʾ���ַ����ֽ���
{
	//�����
	sstr.clear();
	sstr.str("");
	sstr << "[LEN" << Style_FuntionName << "(";
	sstr << Data << ")]";
	//��β
	if (VarNo >= 0)
		sstr << Style_Assignment << ">" << Style_Assignment << VarNo;
	if (RefreshFlag)
		Info->Append_Script(sstr.str());
	return sstr.str();
}

string CCommand::GETTIME(string Format, string RFU, int VarNo /*= -1*/, bool RefreshFlag /*= true */)//��ȡϵͳʱ��
{
	bool flag = false;//�ú����ڵĴ����־
	string str;
	//������
	if (ErrCheckFlag)
	{
		if (StrDel_ConVar(Format, str) == false)
		{
			if (str != "1" && str != "2" && str != "3" && str != "4")
			{
				Set_Error_Flag();
				serr << "��ȡϵͳʱ�� GETTIME��" << Format << ", " << RFU << "����δ��ʶ��ָ����ʱ���ʽ��" << Format << "������ЧȡֵΪ��1-��yyyy-MM-dd hh:mm:ss����2-��yyyyMMddhhmmss����3-��yyyy-MM-dd����4-��yyyyMMdd����";
				Info->Append_Error(serr.str());
				flag = true;
			}
		}
	}
	if (flag)
		return "";
	//�����
	sstr.clear();
	sstr.str("");
	sstr << "[GETTIME" << Style_FuntionName << "(";
	sstr << Format << "," << Style_Comma;
	sstr << RFU << ")]";
	//��β
	if (VarNo >= 0)
		sstr << Style_Assignment << ">" << Style_Assignment << VarNo;
	if (RefreshFlag)
		Info->Append_Script(sstr.str());
	return sstr.str();
}

string CCommand::SUBTRACT(string Minuend, string Subtrahend, int VarNo /*= -1*/, bool RefreshFlag /*= true */)//��������
{
	bool flag = false;//�ú����ڵĴ����־
	string str;
	//������
	if (ErrCheckFlag)
	{
		StrDel_ConVar(Minuend, str);
		if (StrTrim(Minuend).empty() || JudgeDec(str) == false)
		{
			Set_Error_Flag();
			serr << "�������� SUBTRACT��" << Minuend << ", " << Subtrahend << "����ʮ���Ʊ�������" << Minuend << "�����Ƿ��ַ��򳤶�����";
			Info->Append_Error(serr.str());
			flag = true;
		}
		StrDel_ConVar(Subtrahend, str);
		if (StrTrim(Subtrahend).empty() || JudgeDec(str) == false)
		{
			Set_Error_Flag();
			serr << "�������� SUBTRACT��" << Minuend << ", " << Subtrahend << "����ʮ���Ƽ�����" << Subtrahend << "�����Ƿ��ַ��򳤶�����";
			Info->Append_Error(serr.str());
			flag = true;
		}
	}
	if (flag)
		return "";
	//�����
	sstr.clear();
	sstr.str("");
	sstr << "[SUBTRACT" << Style_FuntionName << "(";
	sstr << Minuend << "," << Style_Comma;
	sstr << Subtrahend << ")]";
	//��β
	if (VarNo >= 0)
		sstr << Style_Assignment << ">" << Style_Assignment << VarNo;
	if (RefreshFlag)
		Info->Append_Script(sstr.str());
	return sstr.str();
}

void CCommand::RETURN_Value_Add(string Tag, int VarNo, int Format)//��������Ϣ
{
	bool flag = false;//�ú����ڵĴ����־
	CReturnValue rValue;
	if (ErrCheckFlag)
	{
		if (StrTrim(Tag).empty())
		{
			serr.clear();
			serr.str("");
			serr << "��������Ϣ RETURN_Value_Add��" << Tag << ", " << VarNo << ", " << Format << "������ǩֵ��Ӧ��Ϊ�ա�";
			Info->Append_Warning(serr.str());
		}
		if (VarNo < 0)
		{
			Set_Error_Flag();
			serr << "��������Ϣ RETURN_Value_Add��" << Tag << ", " << VarNo << ", " << Format << "����������š�" << VarNo <<"����Ӧ��С��0��";
			Info->Append_Error(serr.str());
			flag = true;
		}
		if (Format != 0 && Format != 1 && Format != 2 && Format != 3 && Format != 4 && Format != 5 && Format != 6)
		{
			Set_Error_Flag();
			serr << "��������Ϣ RETURN_Value_Add��" << Tag << ", " << VarNo << ", " << Format << "�����޷�ʶ��ָ���Ĵ����ʽFormat��" << Format << "������Чȡֵ��Χ��0-������1-an��ʽ��2-cn��ʽ��3-ǰ��0����ֵ����Ϊ0�򷵻�0����4-��F����ֵ����Ϊ0�򷵻�0����5--ǰ��0����ֵ����Ϊ0�򷵻ؿմ�����6-��F����ֵ����Ϊ0�򷵻ؿմ�����";
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

string CCommand::RETURN(int KeyType, bool RefreshFlag /*= true */)//���������Ϣ
{
	bool flag = false;//�ú����ڵĴ����־
	//������
	if (ErrCheckFlag)
	{
		if (KeyType != 0 && KeyType != 1 && KeyType != 2)
		{
			Set_Error_Flag();
			serr << "���������Ϣ RETURN��" << KeyType << "�����޷�ʶ��ָ������Կ���͡�" << KeyType << "������Чȡֵ��Χ��0-�̶���Կ��1-���ܻ���2-PSAM����";
			Info->Append_Error(serr.str());
			flag = true;
		}
	}
	if (flag)
		return "";
	//�����
	sstr.clear();
	sstr.str("");
	sstr << "[RETURN" << Style_FuntionName << "(";
	sstr << "Key|" << KeyType << "|0|$";
	for (unsigned int i = 0; i < RetValue.size(); ++i)
		sstr << Style_Comma << RetValue.at(i).Tag << "|VAR" << RetValue.at(i).VarNo << "|" << RetValue.at(i).ForMat << "|$";
	sstr << ")]";
	//��β
	RetValue.clear();
	if (RefreshFlag)
		Info->Append_Script(sstr.str());
	return sstr.str();
}



