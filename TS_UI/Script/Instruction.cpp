//#####################################
//
//	class Instruction
//
//	ָ���ࣨ��оƬ��Ļ��࣬�γ�ͳһ����ӿڣ�
//
//	����޸����ڣ�2015-11-13 11:46
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
	//��ȡ���ڲ��ļ��Ķ��ļ���ʶ��
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

void CInstruction::ReSetErrFlag()//���ô����־
{
	ErrFlag = false;
}

bool CInstruction::Error()//�жϴ���
{
	return ErrFlag;
}

void CInstruction::Error_Check(bool Swf)//�Ƿ�򿪴�����
{
	ErrCheckFlag = Swf;
}

void CInstruction::Style_Set(unsigned char Sty)//������ʽ
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

string CInstruction::Get_Instruction_Head()//ȡ��ָ��ͷ��
{
	return StrTrim(InstHead);
}

string CInstruction::Get_Instruction_Data()//ȡ��ָ�����Ϣ
{
	return StrTrim(InstData);
}

void CInstruction::Extend_Param( string Ext )//������չ����
{
	Extends = Ext;
}

void CInstruction::Initialization()//��������¼�����г�ʼ��
{
	sstr.clear();
	sstr.str("");
	serr.clear();
	serr.str("");
	memset(InstHead, 0, sizeof(InstHead));
	memset(InstData, 0, sizeof(InstData));
}

void CInstruction::Set_Error_Flag()//���ô����־
{
	ErrFlag = true;
	serr.clear();
	serr.str("");
}

bool CInstruction::CheckData(const string &Data, int Len, string &ErrMsg)//�������
{
	int cnt;//��¼����
	string str;
	string::size_type pos;
	string::size_type s, e;
	pos = 0;
	cnt = 0;
	while (pos < Data.length())
	{
		s = Data.find("\"", pos);
		//��β
		if (s == string::npos)
		{
			str = Data.substr(pos, Data.length() - pos);
			if (IsHex(str) == false)
			{
				ErrMsg = "���зǷ��ַ�";
				return false;
			}
			cnt += str.length();
			break;
		}
		//CN����
		str = Data.substr(pos, s - pos);
		if (JudgeHex(str) == false)
		{
			ErrMsg = "���зǷ��ַ�";
			return false;
		}
		cnt += str.length();
		//AN����
		e = Data.find("\"", pos + s + 1);
		if (e == string::npos)
		{
			ErrMsg = "˫���Ų���ԡ�";
			return false;
		}
		cnt += (e - s - 1) * 2;
		pos = e + 1;
	}
	//�����ж�
	if (((Len > 0) && (Len * 2 != cnt)) || ((Len < 0) && (((-Len) * 2 < cnt)) || (cnt % 2 != 0)))
	{
		ErrMsg = "���Ȳ�����Ҫ��";
		return false;
	}
	return true;
}

void CInstruction::CON_Extend(string &ConData)//��������
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

string CInstruction::Get_Internal_FID(string FType)//��ȡ�ڲ��ļ�FID
{
	//�������
	if (FType != "SF" && FType != "FCI" && FType != "DIR" && FType != "TF" && FType != "ATR")
	{
		serr.clear();
		serr.str("");
		serr << "��ȡ�ڲ��ļ�FID Get_Internal_FID��" << FType << "�����޷�ʶ��ָ����FID��";
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
	ָ���
***********************************************/

string CInstruction::Get_Challenge(int Len, bool RefreshFlag /*= true*/)//ȡ�����
{
	Initialization();
	//�������
	if (ErrCheckFlag)
	{
		if (Len != 4 && Len != 8 && Len != 16)
		{
			serr.clear();
			serr.str("");
			serr << "ȡ�����ָ�� Get_Challenge��" << Len << "����ָ������������ĳ���Ҫ��Ϊ��0x04������0x08����0x10��������ǰΪ��0x" << setfill('0') << setw(2) << uppercase << right << Len << "����";
			Info->Append_Warning(serr.str());
		}
	}
	//����ָ��
	sstr << "00" << Style_InstHead;
	sstr << "84" << Style_InstHead;
	sstr << "00" << Style_InstHead;
	sstr << "00" << Style_InstHead;
	sstr << setfill('0') << setw(2) << hex << uppercase << right << Len;
	strcpy(InstHead, StrTrim(sstr.str()).c_str());
	//��β
	if (RefreshFlag)
		Info->Append_Script(sstr.str());
	return sstr.str();
}

string CInstruction::Select_File(bool Dir, bool FID, int Len, string Data, bool RefreshFlag /*= true */)//ѡ���ļ�
{
	bool flag = false;//�ú����ڵĴ����־
	string str;
	string estr;
	Initialization();
	//�������
	if (ErrCheckFlag)
	{
		if (FID == true && Len != 2)
		{
			Set_Error_Flag();
			serr << "ѡ���ļ�ָ�� Select_File��" << (Dir ? "Dir" : "!Dir") << ", " << (FID ? "Fid" : "!Fid") << ", " << Len << ", " << Data << "������ʹ���ļ���ʶ��ʱ��FIDΪtrue����ָ�������򳤶ȱ���Ϊ2������Len��������ǰΪ��" << Len << "����";
			Info->Append_Error(serr.str());
			flag = true;
		}
		if (Len<0 || Len>255)
		{
			Set_Error_Flag();
			serr << "ѡ���ļ�ָ�� Select_File��" << (Dir ? "Dir" : "!Dir") << ", " << (FID ? "Fid" : "!Fid") << ", " << Len << ", " << Data << "����ָ�������򳤶Ȳ�����ȡֵ��ΧΪ0-255������ǰ�ǡ�" << Len << "����";
			Info->Append_Error(serr.str());
			flag = true;
		}
		if (Dir == false && FID == false)
		{
			Set_Error_Flag();
			serr << "ѡ���ļ�ָ�� Select_File��" << (Dir ? "Dir" : "!Dir") << ", " << (FID ? "Fid" : "!Fid") << ", " << Len << ", " << Data << "������ѡ��EFʱ���뽫����FID��Ϊtrue���ļ���ʶ������";
			Info->Append_Error(serr.str());
			flag = true;
		}
	}
	//��������
	if (StrDel_ConVar(Data, str) == false)
	{
		if (ErrCheckFlag && !CheckData(str, Len, estr))
		{
			Set_Error_Flag();
			serr << "ѡ���ļ�ָ�� Select_File��" << (Dir ? "Dir" : "!Dir") << ", " << (FID ? "Fid" : "!Fid") << ", " << Len << ", " << Data << "��������Data��" << Data << "������" << estr;
			Info->Append_Error(serr.str());
			flag = true;
		}
	}
	if ( flag )
		return "";
	//����ָ��
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
	//��β
	if (RefreshFlag)
		Info->Append_Script(sstr.str());
	return sstr.str();
}

string CInstruction::Select_File(string MF /*= ""*/, string DDF /*= ""*/, string ADF /*= ""*/, string EF /*= ""*/, bool RefreshFlag /*= true */)//ѡ���ļ�
{
	bool flag = false;//�ú����ڵĴ����־
	string str;
	string estr;
	string Data;//AID��FID
	bool dir;//Ŀ¼��־
	bool fid;//FID��־
	S_MFINFO mfInfo;
	S_DDFINFO ddfInfo;
	S_ADFINFO adfInfo;
	S_EFINFO efInfo;
	Initialization();
	//�������
	if (ErrCheckFlag)
	{
		if (MF != "MF" && MF != "3F00" && MF != "")
		{
			serr.clear();
			serr.str("");
			serr << "ѡ���ļ�ָ�� Select_File��" << MF << ", " << DDF << ", " << ADF << ", " << EF << "����MFֻ��ȡֵΪ��MF������3F00����մ�����������ǰ�ǡ�" << MF << "����";
			Info->Append_Warning(serr.str());
		}
		if (!ADF.empty() && (!IsHex(ADF) || ADF.length() != 4))
		{
			Set_Error_Flag();
			serr << "ѡ���ļ�ָ�� Select_File��" << MF << ", " << DDF << ", " << ADF << ", " << EF << "��������ADF������4��ʮ�������ַ���ɣ�����ǰΪ��" << ADF << "����";
			Info->Append_Error(serr.str());
			flag = true;
		}
		if (!EF.empty() && (!IsHex(EF) || EF.length() != 4))
		{
			Set_Error_Flag();
			serr << "ѡ���ļ�ָ�� Select_File��" << MF << ", " << DDF << ", " << ADF << ", " << EF << "��������EF������4��ʮ�������ַ���ɣ�����ǰΪ��" << EF << "����";
			Info->Append_Error(serr.str());
			flag = true;
		}
	}
	//��ȡ����
	if (EF.empty() == false)
	{
		dir = false;
		fid = true;
		if (Param->GetEFInfo(DDF, ADF, EF, efInfo) == false)
		{
			Set_Error_Flag();
			serr << "ѡ���ļ�ָ�� Select_File��" << MF << ", " << DDF << ", " << ADF << ", " << EF << "�����������ļ����Ҳ���ָ��·����EF�ļ�����" << MF << "������" << DDF << "������" << ADF << "������" << EF << "������";
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
			serr << "ѡ���ļ�ָ�� Select_File��" << MF << ", " << DDF << ", " << ADF << ", " << EF << "�����������ļ����Ҳ���ָ��·����ADF�ļ�����" << MF << "������" << DDF << "������" << ADF << "������";
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
				serr << "ѡ���ļ�ָ�� Select_File��" << MF << ", " << DDF << ", " << ADF << ", " << EF << "���������������Ҳ���SSSEĿ¼���˴����Զ�ʹ��Ĭ�ϵ�AID��7378312E73682EC9E7BBE1B1A3D5CF����";
				Info->Append_Warning(serr.str());
			}
			else if (DDF == "PSE")
				Data = "315041592E5359532E4444463031";
			else
			{
				Set_Error_Flag();
				serr << "ѡ���ļ�ָ�� Select_File��" << MF << ", " << DDF << ", " << ADF << ", " << EF << "�����������ļ����Ҳ���ָ��·����DDF�ļ�����" << MF << "������" << DDF << "������";
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
	//���ü��
	if (StrDel_ConVar(Data, str) == true)
	{
		Set_Error_Flag();
		serr << "ѡ���ļ�ָ�� Select_File��" << MF << ", " << DDF << ", " << ADF << ", " << EF << "����ָ���ļ���AID��FID��Ӧ����[CON]������ָ����";
		Info->Append_Error(serr.str());
		flag = true;
	}
	if (flag)
		return "";
	//ָ���
	return Select_File(dir, fid, Data.length() / 2, Data, RefreshFlag);
}

string CInstruction::External_Authenticate(string KeyID, string Cipher, string Scr, int KeyVer /*= 0*/, bool RefreshFlag /*= true */)//�ⲿ��֤
{
	bool flag = false;//�ú����ڵĴ����־
	string str;
	string estr;
	Initialization();
	//�������
	if (ErrCheckFlag)
	{
		if (!IsHex(KeyID) || KeyID.length() != 2)
		{
			Set_Error_Flag();
			serr << "�ⲿ��ָ֤�� External_Authenticate��" << KeyID << ", " << Cipher << ", " << Scr << ", " << KeyVer << "������Կ��ʶ������2��ʮ�������ַ���ɣ�����ǰΪ��" << KeyID << "����";
			Info->Append_Error(serr.str());
			flag = true;
		}
		if (KeyVer<0 || KeyVer>255)
		{
			Set_Error_Flag();
			serr << "�ⲿ��ָ֤�� External_Authenticate��" << KeyID << ", " << Cipher << ", " << Scr << ", " << KeyVer << "������Կ�汾ȡֵ��������Ч��Χ����ǰ�ǡ�" << KeyVer << "����";
			Info->Append_Error(serr.str());
			flag = true;
		}
		else if (KeyVer<0 || KeyVer>3)
		{
			serr.clear();
			serr.str("");
			serr << "�ⲿ��ָ֤�� External_Authenticate��" << KeyID << ", " << Cipher << ", " << Scr << ", " << KeyVer << "������Կ�汾ȡֵ��Χ�����ǡ�0��-��3��������ǰ�ǡ�" << KeyVer << "����";
			Info->Append_Warning(serr.str());
		}
	}
	//�������鼰����
	if (StrDel_ConVar(Cipher, str) == false)
	{
		if (ErrCheckFlag && !CheckData(str, 8, estr))
		{
			Set_Error_Flag();
			serr << "�ⲿ��ָ֤�� External_Authenticate��" << KeyID << ", " << Cipher << ", " << Scr << ", " << KeyVer << "��������Cipher��" << Cipher << "������" << estr;
			Info->Append_Error(serr.str());
			flag = true;
		}
	}
	if (!Scr.empty() && !StrDel_ConVar(Scr, str))
	{
		if (ErrCheckFlag && !CheckData(str, 8, estr))
		{
			Set_Error_Flag();
			serr << "�ⲿ��ָ֤�� External_Authenticate��" << KeyID << ", " << Cipher << ", " << Scr << ", " << KeyVer << "��������Scr��" << Scr << "������" << estr;
			Info->Append_Error(serr.str());
			flag = true;
		}
	}
	if (flag)
		return "";
	//����ָ��
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
	//��β
	if (RefreshFlag)
		Info->Append_Script(sstr.str());
	return sstr.str();
}

string CInstruction::Update_Record(string P1, string P2, int Len, string Data, bool RefreshFlag /*= true */)//�޸ļ�¼
{
	bool flag = false;//�ú����ڵĴ����־
	string str;
	string estr;
	Initialization();
	//�������
	if (ErrCheckFlag)
	{
		if (!IsHex(P1) || P1.length() != 2)
		{
			Set_Error_Flag();
			serr << "�޸ļ�¼ָ�� Update_Record��" << P1 << ", " << P2 << ", " << Len << ", " << Data << "����P1������2��ʮ�������ַ���ɣ�����ǰ�ǡ�" << P1 << "����";
			Info->Append_Error(serr.str());
			flag = true;
		}
		if (!IsHex(P2) || P2.length() != 2)
		{
			Set_Error_Flag();
			serr << "�޸ļ�¼ָ�� Update_Record��" << P1 << ", " << P2 << ", " << Len << ", " << Data << "����P2������2��ʮ�������ַ���ɣ�����ǰ�ǡ�" << P2 << "����";
			Info->Append_Error(serr.str());
			flag = true;
		}
		if (Len<0 || Len>255)
		{
			Set_Error_Flag();
			serr << "�޸ļ�¼ָ�� Update_Record��" << P1 << ", " << P2 << ", " << Len << ", " << Data << "����ָ�������򳤶Ȳ�����ȡֵ��ΧΪ0-255������ǰ�ǡ�" << Len << "����";
			Info->Append_Error(serr.str());
			flag = true;
		}
	}
	//�������鼰����
	if (StrDel_ConVar(Data, str) == false)
	{
		if (ErrCheckFlag && !CheckData(str, -Len, estr))
		{
			Set_Error_Flag();
			serr << "�޸ļ�¼ָ�� Update_Record��" << P1 << ", " << P2 << ", " << Len << ", " << Data << "��������Data��" << Data << ")����" << estr;
			Info->Append_Error(serr.str());
			flag = true;
		}
	}
	if (flag)
		return "";
	//����ָ��
	sstr << "00" << Style_InstHead;
	sstr << "DC" << Style_InstHead;
	sstr << ToUpper(P1) << Style_InstHead;
	sstr << ToUpper(P2) << Style_InstHead;
	sstr << setfill('0') << setw(2) << hex << uppercase << right << Len << Style_Separator;
	sstr << ToUpper(Data);
	strcpy(InstHead, StrTrim(sstr.str()).substr(0, 10).c_str());
	strcpy(InstData, StrTrim(sstr.str()).erase(0, 10).c_str());
	//��β
	if (RefreshFlag)
		Info->Append_Script(sstr.str());
	return sstr.str();
}

string CInstruction::Update_Record(string MF, string DDF, string ADF, string EF, int RecordNo, bool InitFlag /*= false*/, bool RefreshFlag /*= true */)
{
	bool flag = false;//�ú����ڵĴ����־
	string str;
	string estr;
	S_EFINFO efInfo;
	EFDATAS efData;
	Initialization();
	//�������
	if (ErrCheckFlag)
	{
		if (MF != "MF" && MF != "3F00" && MF != "")
		{
			Set_Error_Flag();
			serr << "�޸ļ�¼ָ�� Update_Record��" << MF << ", " << DDF << ", " << ADF << ", " << EF << ", " << RecordNo << "����MFֻ��ȡֵΪ��MF������3F00����մ�����������ǰ�ǡ�" << MF << "����";
			Info->Append_Error(serr.str());
			flag = true;
		}
		if (!ADF.empty() && (!IsHex(ADF) || ADF.length() != 4))
		{
			Set_Error_Flag();
			serr << "�޸ļ�¼ָ�� Update_Record��" << MF << ", " << DDF << ", " << ADF << ", " << EF << ", " << RecordNo << "��������ADF������4��ʮ�������ַ���ɣ�����ǰΪ��" << ADF << "����";
			Info->Append_Error(serr.str());
			flag = true;
		}
		if (!EF.empty() && (!IsHex(EF) || EF.length() != 4))
		{
			Set_Error_Flag();
			serr << "�޸ļ�¼ָ�� Update_Record��" << MF << ", " << DDF << ", " << ADF << ", " << EF << ", " << RecordNo << "��������EF������4��ʮ�������ַ���ɣ�����ǰΪ��" << EF << "����";
			Info->Append_Error(serr.str());
			flag = true;
		}
		if (RecordNo <= 0 || RecordNo > 255)
		{
			Set_Error_Flag();
			serr << "�޸ļ�¼ָ�� Update_Record��" << MF << ", " << DDF << ", " << ADF << ", " << EF << ", " << RecordNo << "������¼�Ų�����ȡֵ��Χ����Ϊ1-255������ǰΪ��" << RecordNo << "����";
			Info->Append_Error(serr.str());
			flag = true;
		}
	}
	//��ȡ����
	if (Param->GetEFInfo(DDF, ADF, EF, efInfo) == false)
	{
		Set_Error_Flag();
		serr << "�޸ļ�¼ָ�� Update_Record��" << MF << ", " << DDF << ", " << ADF << ", " << EF << ", " << RecordNo << "�����������ļ����Ҳ���ָ��·����EF�ļ�����" << MF << "������" << DDF << "������" << ADF << "������" << EF << "������";
		Info->Append_Error(serr.str());
		flag = true;
	}
	if (efInfo.m_strEFType == EFTYPE_BINARY)
	{
		Set_Error_Flag();
		serr << "�޸ļ�¼ָ�� Update_Record��" << MF << ", " << DDF << ", " << ADF << ", " << EF << ", " << RecordNo << "������ʹ�� Update_Binary ����������д�������ļ���";
		Info->Append_Error(serr.str());
		flag = true;
	}
	if (Param->GetEFData(DDF, ADF, EF, RecordNo, efData) == false)
	{
		Set_Error_Flag();
		serr << "�޸ļ�¼ָ�� Update_Record��" << MF << ", " << DDF << ", " << ADF << ", " << EF << ", " << RecordNo << "�����������ļ����Ҳ���ָ��·�����ļ���¼��";
		Info->Append_Error(serr.str());
		flag = true;
	}
	CON_Extend(efData.m_strVal);
	//���������л�ȡ���Ĳ���
	if (ErrCheckFlag)
	{
		if (efData.m_strrType != RECORD_AN && efData.m_strrType != RECORD_CN && efData.m_strrType != RECORD_B && efData.m_strrType != RECORD_TLV && efData.m_strrType != RECORD_LOOP)
		{
			Set_Error_Flag();
			serr << "�޸ļ�¼ָ�� Update_Record��" << MF << ", " << DDF << ", " << ADF << ", " << EF << ", " << RecordNo << "�����������ļ��л�ȡ���ļ�¼���Ͳ���Ԥ��֮�У�" << efData.m_strrType << "����";
			Info->Append_Error(serr.str());
			flag = true;
		}
		if (efData.m_strrType != RECORD_LOOP && (!IsHex(efData.m_strTag) || efData.m_strTag.length() > 2))
		{
			Set_Error_Flag();
			serr << "�޸ļ�¼ָ�� Update_Record��" << MF << ", " << DDF << ", " << ADF << ", " << EF << ", " << RecordNo << "������¼��־������2��ʮ�������ַ���ɣ�����ǰ�ǡ�" << efData.m_strTag << "����";
			Info->Append_Error(serr.str());
			flag = true;
		}
		if (!IsHex(efData.m_strLen) || efData.m_strLen.length() > 2)
		{
			Set_Error_Flag();
			serr << "�޸ļ�¼ָ�� Update_Record��" << MF << ", " << DDF << ", " << ADF << ", " << EF << ", " << RecordNo << "������¼���ȱ�����2��ʮ�������ַ���ɣ�����ǰ�ǡ�" << efData.m_strLen << "����";
			Info->Append_Error(serr.str());
			flag = true;
		}
		if (StrDel_ConVar(efData.m_strVal, str) == false)
		{
			if ((efData.m_strrType == RECORD_CN || efData.m_strrType == RECORD_B) && !CheckData(str, -HexToDec(efData.m_strLen), estr))
			{
				Set_Error_Flag();
				serr << "�޸ļ�¼ָ�� Update_Record��" << MF << ", " << DDF << ", " << ADF << ", " << EF << ", " << RecordNo << "������¼��ֵ���󡣡�";
				Info->Append_Error(serr.str());
				flag = true;
			}
			if (efData.m_strrType == RECORD_AN && !StrTrim(efData.m_strVal, "0").empty() && efData.m_strVal.length() > (unsigned long)HexToDec(efData.m_strLen))
			{
				Set_Error_Flag();
				serr << "�޸ļ�¼ָ�� Update_Record��" << MF << ", " << DDF << ", " << ADF << ", " << EF << ", " << RecordNo << "������¼���ȡ�" << efData.m_strLen << "������";
				Info->Append_Error(serr.str());
				flag = true;
			}
		}	
	}
	if (flag)
		return "";
	//ָ���
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
			//���ü��
			if (ErrCheckFlag)
			{
				if (!IsHex(efData.m_vecEFExDatas.at(i).m_strTag) || efData.m_vecEFExDatas.at(i).m_strTag.length() > 2)
				{
					Set_Error_Flag();
					serr << "�޸ļ�¼ָ�� Update_Record��" << MF << ", " << DDF << ", " << ADF << ", " << EF << ", " << RecordNo << "����B-TLV�Ӽ�¼��־������2��ʮ�������ַ���ɣ�����ǰ�ǡ�" << efData.m_strTag << "����";
					Info->Append_Error(serr.str());
					flag = true;
				}
				if (!IsHex(efData.m_vecEFExDatas.at(i).m_strLen) || efData.m_vecEFExDatas.at(i).m_strLen.length() > 2)
				{
					Set_Error_Flag();
					serr << "�޸ļ�¼ָ�� Update_Record��" << MF << ", " << DDF << ", " << ADF << ", " << EF << ", " << RecordNo << "����B-TLV�Ӽ�¼���ȱ�����2��ʮ�������ַ���ɣ�����ǰ�ǡ�" << efData.m_strTag << "����";
					Info->Append_Error(serr.str());
					flag = true;
				}
				if (efData.m_vecEFExDatas.at(i).m_strrType != RECORD_AN && efData.m_vecEFExDatas.at(i).m_strrType != RECORD_CN && efData.m_vecEFExDatas.at(i).m_strrType != RECORD_B)
				{
					Set_Error_Flag();
					serr << "�޸ļ�¼ָ�� Update_Record��" << MF << ", " << DDF << ", " << ADF << ", " << EF << ", " << RecordNo << "���������б�ǩΪ��" << efData.m_vecEFExDatas.at(i).m_strTag << "�������ͣ�" << efData.m_vecEFExDatas.at(i).m_strrType << "�����󡣡�";
					Info->Append_Error(serr.str());
					flag = true;

				}
				if ((efData.m_vecEFExDatas.at(i).m_strrType == RECORD_CN || efData.m_vecEFExDatas.at(i).m_strrType == RECORD_B) && StrDel_ConVar(efData.m_vecEFExDatas.at(i).m_strVal, str) == false)
				{
					if (CheckData(str, -HexToDec(efData.m_vecEFExDatas.at(i).m_strLen), estr) == false)
					{
						Set_Error_Flag();
						serr << "�޸ļ�¼ָ�� Update_Record��" << MF << ", " << DDF << ", " << ADF << ", " << EF << ", " << RecordNo << "���������б�ǩΪ��" << efData.m_vecEFExDatas.at(i).m_strTag << "����ֵ���󡣡�";
						Info->Append_Error(serr.str());
						flag = true;
					}
				}
				if (StrDel_ConVar(efData.m_vecEFExDatas.at(i).m_strVal, str) == false)
				{
					if ((efData.m_vecEFExDatas.at(i).m_strrType == RECORD_CN || efData.m_vecEFExDatas.at(i).m_strrType == RECORD_B) && !CheckData(str, -HexToDec(efData.m_vecEFExDatas.at(i).m_strLen), estr))
					{
						Set_Error_Flag();
						serr << "�޸ļ�¼ָ�� Update_Record��" << MF << ", " << DDF << ", " << ADF << ", " << EF << ", " << RecordNo << "���������б�ǩΪ��" << efData.m_vecEFExDatas.at(i).m_strTag << "���ļ�¼��������";
						Info->Append_Error(serr.str());
						flag = true;
					}
					if (efData.m_vecEFExDatas.at(i).m_strrType == RECORD_AN && !StrTrim(efData.m_vecEFExDatas.at(i).m_strVal, "0").empty() && efData.m_vecEFExDatas.at(i).m_strVal.length() > (unsigned long)HexToDec(efData.m_vecEFExDatas.at(i).m_strLen))
					{
						Set_Error_Flag();
						serr << "�޸ļ�¼ָ�� Update_Record��" << MF << ", " << DDF << ", " << ADF << ", " << EF << ", " << RecordNo << "���������б�ǩΪ��" << efData.m_vecEFExDatas.at(i).m_strTag << "���ļ�¼���ݵĳ�������";
						Info->Append_Error(serr.str());
						flag = true;
					}
				}
			}
			if (flag)
				return "";
			//ָ��
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
	//ѭ����¼
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
			//���ü��
			if (ErrCheckFlag)
			{
				if (!IsHex(efData.m_vecEFExDatas.at(i).m_strLen) || efData.m_vecEFExDatas.at(i).m_strLen.length() > 2)
				{
					Set_Error_Flag();
					serr << "�޸ļ�¼ָ�� Update_Record��" << MF << ", " << DDF << ", " << ADF << ", " << EF << ", " << RecordNo << "����ѭ����¼���Ӽ�¼��P1����" << efData.m_vecEFExDatas.at(i).m_strP1 << "�������ȱ�����2��ʮ�������ַ���ɣ�����ǰ�ǡ�" << efData.m_strLen << "����";
					Info->Append_Error(serr.str());
					flag = true;
				}
				if (efData.m_vecEFExDatas.at(i).m_strrType != RECORD_AN && efData.m_vecEFExDatas.at(i).m_strrType != RECORD_CN && efData.m_vecEFExDatas.at(i).m_strrType != RECORD_B)
				{
					Set_Error_Flag();
					serr << "�޸ļ�¼ָ�� Update_Record��" << MF << ", " << DDF << ", " << ADF << ", " << EF << ", " << RecordNo << "����ѭ���Ӽ�¼��P1����" << efData.m_vecEFExDatas.at(i).m_strP1 << "���������ͣ�" << efData.m_vecEFExDatas.at(i).m_strrType << "�����󡣡�";
					Info->Append_Error(serr.str());
					flag = true;
				}
				if (StrDel_ConVar(efData.m_vecEFExDatas.at(i).m_strVal, str) == false)
				{
					if ((efData.m_vecEFExDatas.at(i).m_strrType == RECORD_CN || efData.m_vecEFExDatas.at(i).m_strrType == RECORD_B) && !CheckData(str, -HexToDec(efData.m_vecEFExDatas.at(i).m_strLen), estr))
					{
						Set_Error_Flag();
						serr << "�޸ļ�¼ָ�� Update_Record��" << MF << ", " << DDF << ", " << ADF << ", " << EF << ", " << RecordNo << "����ѭ���Ӽ�¼��P1����" << efData.m_vecEFExDatas.at(i).m_strP1 << "�����ļ�¼��������";
						Info->Append_Error(serr.str());
						flag = true;
					}
					if (efData.m_vecEFExDatas.at(i).m_strrType == RECORD_AN && !StrTrim(efData.m_vecEFExDatas.at(i).m_strVal, "0").empty() && efData.m_vecEFExDatas.at(i).m_strVal.length() > (unsigned long)HexToDec(efData.m_vecEFExDatas.at(i).m_strLen))
					{
						Set_Error_Flag();
						serr << "�޸ļ�¼ָ�� Update_Record��" << MF << ", " << DDF << ", " << ADF << ", " << EF << ", " << RecordNo << "����ѭ���Ӽ�¼��P1����" << efData.m_vecEFExDatas.at(i).m_strP1 << "������¼���ݵĳ�������";
						Info->Append_Error(serr.str());
						flag = true;
					}
				}
			}
			if (flag)
				return "";
			//ָ��
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
	//an��cn��b
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
	//��β
	if (RefreshFlag)
		Info->Append_Script(sstr.str());
	return sstr.str();
}

string CInstruction::Update_Binary(int Offset, int Len, string Data, bool RefreshFlag /*= true */)//�޸Ķ�����
{
	bool flag = false;//�ú����ڵĴ����־
	string str;
	string estr;
	Initialization();
	//�������
	if (ErrCheckFlag)
	{
		if (Offset<0 || Offset>0x7FFF)
		{
			Set_Error_Flag();
			serr << "�޸Ķ�����ָ�� Update_Binary��" << Offset << ", " << Len << ", " << Data << "����ƫ����ȡֵ��ΧΪ0-0x7FFF������ǰΪ��0x" << hex << setfill('0') << setw(4) << uppercase << right << Offset << "����";
			Info->Append_Error(serr.str());
			flag = true;
		}
		if (Len<0 || Len>255)
		{
			Set_Error_Flag();
			serr << "�޸Ķ�����ָ�� Update_Binary��" << Offset << ", " << Len << ", " << Data << "����ָ�������򳤶Ȳ�����ȡֵ��ΧΪ0-255������ǰ�ǡ�" << Len << "����";
			Info->Append_Error(serr.str());
			flag = true;
		}
	}
	//�������鼰����
	if (StrDel_ConVar(Data, str) == false)
	{
		if (ErrCheckFlag && !CheckData(str, Len, estr))
		{
			Set_Error_Flag();
			serr << "�޸Ķ�����ָ�� Update_Binary��" << Offset << ", " << Len << ", " << Data << "��������Data��" << Data << ")����" << estr;
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
	//ָ���
	sstr << "00" << Style_InstHead;
	sstr << "D6" << Style_InstHead;
	sstr << setfill('0') << setw(2) << right << hex << uppercase << Offset / 0x100 << Style_InstHead;
	sstr << setfill('0') << setw(2) << right << hex << uppercase << Offset % 0x100 << Style_InstHead;
	sstr << setfill('0') << setw(2) << right << hex << uppercase << Len << Style_Separator;
	sstr << ToUpper(Data);
	strcpy(InstHead, StrTrim(sstr.str()).substr(0, 10).c_str());
	strcpy(InstData, StrTrim(sstr.str()).erase(0, 10).c_str());
	//��β
	if (RefreshFlag)
		Info->Append_Script(sstr.str());
	return sstr.str();
}

string CInstruction::DIR_Record(int RecordNo, string MF, string DDF, string ADF, string Label, bool RefreshFlag /*= true*/)
{
	bool flag = false;//�ú����ڵĴ����־
	S_MFINFO mfInfo;
	S_DDFINFO ddfInfo;
	S_ADFINFO adfInfo;
	string aid;
	stringstream stemp;
	Initialization();
	//�������
	if (ErrCheckFlag)
	{
		if (MF != "MF" && MF != "3F00" && MF != "")
		{
			Set_Error_Flag();
			serr << "���DIR��¼ DIR_Record(" << RecordNo << ", " << MF << ", " << DDF << ", " << ADF << ", " << Label << ")��MFֻ��ȡֵΪ��MF������3F00����մ�����������ǰ�ǡ�" << MF << "����";
			Info->Append_Error(serr.str());
			flag = true;
		}
		if (!ADF.empty() && (!IsHex(ADF) || ADF.length() != 4))
		{
			Set_Error_Flag();
			serr << "���DIR��¼ DIR_Record(" << RecordNo << ", " << MF << ", " << DDF << ", " << ADF << ", " << Label << ")������ADF������4��ʮ�������ַ���ɣ�����ǰΪ��" << ADF << "����";
			Info->Append_Error(serr.str());
			flag = true;
		}
		if (RecordNo <= 0)
		{
			serr.clear();
			serr.str("");
			serr << "���DIR��¼ DIR_Record(" << RecordNo << ", " << MF << ", " << DDF << ", " << ADF << ", " << Label << ")����¼�ű������0��";
			Info->Append_Warning(serr.str());
		}
		if (Label.length() > 200)
		{
			Set_Error_Flag();
			serr << "���DIR��¼ DIR_Record(" << RecordNo << ", " << MF << ", " << DDF << ", " << ADF << ", " << Label << ")��Ӧ�ñ�ǩ�������ݹ��ࡣ";
			Info->Append_Error(serr.str());
			flag = true;
		}
	}
	//��ȡ����
	if (ADF.empty() == false)
	{
		if (Param->GetADFInfo(DDF, ADF, adfInfo) == false)
		{
			Set_Error_Flag();
			serr << "���DIR��¼ DIR_Record(" << RecordNo << ", " << MF << ", " << DDF << ", " << ADF << ", " << Label << ")���������ļ����Ҳ���ָ��·����ADF�ļ���";
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
			serr << "���DIR��¼ DIR_Record(" << RecordNo << ", " << MF << ", " << DDF << ", " << ADF << ", " << Label << ")���������ļ����Ҳ���ָ��·����DDF�ļ���";
			Info->Append_Error(serr.str());
			flag = true;
		}
		aid = ddfInfo.m_strAID;
	}
	else
	{
		serr.clear();
		serr.str("");
		serr << "���DIR��¼ DIR_Record(" << RecordNo << ", " << MF << ", " << DDF << ", " << ADF << ", " << Label << ")���������ļ����Ҳ���ָ��·����DDF�ļ���";
		Info->Append_Warning(serr.str());
		Param->GetMFInfo(mfInfo);
		aid = mfInfo.m_strAID;
	}
	//���ü��
	if (ErrCheckFlag)
	{
		if (aid.length() % 2 != 0 || !IsHex(aid))
		{
			Set_Error_Flag();
			serr << "���DIR��¼ DIR_Record(" << RecordNo << ", " << MF << ", " << DDF << ", " << ADF << ", " << Label << ")��ָ��Ŀ¼�ļ���AID(" << aid << ")��������򺬷Ƿ��ַ���";
			Info->Append_Error(serr.str());
			flag = true;
		}
	}
	if (flag)
		return "";
	//��������������
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
	//����ָ��
	stemp << setfill('0') << setw(2) << hex << uppercase << right << RecordNo;
	if (HexToDec(DIR_SFI) >= 0x05 && HexToDec(DIR_SFI) <= 0x1E)//��SFIѰַ��Χ֮�࣬�Զ�ʹ��SFIѰַ
		return Update_Record(stemp.str(), DecToHex(HexToDec(DIR_SFI) * 8 + 4), (StrTrim(StrTrim(sstr.str()), "\"").length() + Label.length()) / 2, sstr.str(), RefreshFlag);
	else
		return Update_Record(stemp.str(), "04", (StrTrim(StrTrim(sstr.str()), "\"").length() + Label.length()) / 2, sstr.str(), RefreshFlag);
}

int CInstruction::EF_Control(string DDF, string ADF, string CtrlStr, KEYDATAS &keyData1, KEYDATAS &keyData2, string &ErrMsg)
{
	string::size_type n;
	if (CtrlStr == "" || CtrlStr == "00")//������
		return 1;
	else if (CtrlStr == "FF")//����ֹ
		return 2;
	else if ((n = CtrlStr.find_first_of("+|")) != string::npos)//Key+Key �� Key|Key
	{
		if (n != CtrlStr.find_last_of("+|"))
		{
			ErrMsg = "EF�ļ��Ķ�д�ܿ���Կ���ֻ�����ϼ�Ŀ¼����ǰĿ¼������Կ����ɡ�";
			return -1;
		}
		if (Param->GetKeyInfo(DDF, "", StrDelBlank(CtrlStr.substr(0, n)), keyData1) == false)
		{
			ErrMsg = "�Ҳ�����Կ" + StrDelBlank(CtrlStr.substr(0, n));
			return -1;
		}
		if (Param->GetKeyInfo(DDF, ADF, StrDelBlank(CtrlStr.substr(n + 1, CtrlStr.length() - n - 1)), keyData2) == false)
		{
			ErrMsg = "�Ҳ�����Կ" + StrDelBlank(CtrlStr.substr(n + 1, CtrlStr.length() - n - 1));
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
			ErrMsg = "�Ҳ�����Կ" + StrDelBlank(CtrlStr);
			return -1;
		}
		return 3;
	}
	else
	{
		if (Param->GetKeyInfo(DDF, ADF, StrDelBlank(CtrlStr), keyData1) == false)
		{
			ErrMsg = "�Ҳ�����Կ" + StrDelBlank(CtrlStr);
			return -1;
		}
		return 4;
	}
}

void CInstruction::Init_Security_Level(string KeyNameIgnore)
{
	struct S_KEYIGNORE
	{
		string KeyName;//���޳�����Կ����
		int IgnoreType;//ƥ�䷽ʽ��1-�Ӵ�ƥ�䡰XXX����2-����ƥ�䡰[XXX����3-�Ҷ�ƥ�䡰XXX]����4-ȫƥ�䡰[XXX]��
	}keyIgnore;
	vector<S_KEYIGNORE> vecKeyIgnore;//�Ӳ��� KeyNameIgnore �ֽ��������Ϣ
	vector<S_DDFINFO> vecMFStruct;//��DDF��Ϣ
	vector<KEYDATAS> vecKeyDatas;//ADFĿ¼�µ���Կ
	string::size_type s = 0, e = 0;
	bool matchFlag;//ƥ���־������ֵΪtrueʱ������ǰ��Կ��KeyNameIgnore��Ϣ��
	int cnt;//��Կ��ţ�ÿ��ADF�¸���Ŷ����� 1 ��ʼ

	//�ֽ� KeyNameIgnore
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

	//��ʼ����Կ���
	Param->GetCardStruct(vecMFStruct);
	for (unsigned int i = 0; i < vecMFStruct.size(); ++i)//����DDF
	{
		for (unsigned int j = 0; j < vecMFStruct.at(i).m_vecADFInfo.size(); ++j)//����ADF
		{
			vecKeyDatas.clear();
			Param->GetADFKeys(vecMFStruct.at(i).m_strDDFName, vecMFStruct.at(i).m_vecADFInfo.at(j).m_strADFName, vecKeyDatas);
			cnt = 1;
			for (unsigned int m = 0; m < vecKeyDatas.size(); ++m)//������Կ
			{
				for (unsigned int n = 0; n < vecKeyIgnore.size(); ++n)//�������޳�����Կ
				{
					matchFlag = false;
					switch (vecKeyIgnore.at(n).IgnoreType)//���޳�����Կ�������Ϊ 0
					{
					case 1://�Ӵ�ƥ�䡰XXX��
						if (ToUpper(vecKeyDatas.at(m).m_strName).find(vecKeyIgnore.at(n).KeyName) != string::npos)
							matchFlag = true;
						break;
					case 2://����ƥ�䡰[XXX��
						if (ToUpper(vecKeyDatas.at(m).m_strName).find(vecKeyIgnore.at(n).KeyName) == 0)
							matchFlag = true;
						break;
					case 3://�Ҷ�ƥ�䡰XXX]��
						if (ToUpper(vecKeyDatas.at(m).m_strName).compare(vecKeyDatas.at(m).m_strName.length() - vecKeyIgnore.at(n).KeyName.length(), vecKeyIgnore.at(n).KeyName.length(), vecKeyIgnore.at(n).KeyName) == 0)
							matchFlag = true;
						break;
					case 4://ȫƥ�䡰[XXX]��
						if (ToUpper(vecKeyDatas.at(m).m_strName) == vecKeyIgnore.at(n).KeyName)
							matchFlag = true;
						break;
					default:
						break;
					}
					if (matchFlag)//ƥ��ɹ�������ѭ��
						break;
				}
				if (matchFlag)//���޳�����Կ���������Ϊ 0
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
	Param->GetKeyInfo("SSSE","","PIN",struKey);//��ȡSSSE�µ�PIN

	//ָ���
	sstr << "00" << Style_InstHead;
	sstr << "20" << Style_InstHead;
	sstr << "00" << Style_InstHead;
	sstr << "00" << Style_InstHead;
	sstr << setfill('0') << setw(2) << left << DecToHex(struKey.m_strValue.length()/2) << Style_Separator;
	sstr << struKey.m_strValue;
	strcpy(InstHead, StrTrim(sstr.str()).substr(0, 10).c_str());
	strcpy(InstData, StrTrim(sstr.str()).erase(0, 10).c_str());
	//��β
	if (RefreshFlag)
		Info->Append_Script(sstr.str());
	return sstr.str();
}
