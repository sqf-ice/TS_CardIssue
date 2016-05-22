//#####################################
//
//	����T514 COSָ��
//
//	����޸����ڣ�2015-11-09 17:34
//#####################################

#include "chip.h"
#include "tool.h"
#include <iomanip>

const string COS_T514::MK_Level = "FF";//������Կ�İ�ȫ����
const string COS_T514::Key_Forbidden = "EF";//��ֹȨ��
const string COS_T514::Key_Free = "00";//��������


COS_T514::COS_T514(CInformation *inf, CParameter *para) :CInstruction(inf, para)
{	}

string COS_T514::Delete_File(int WorkFlag, string FType /*= ""*/, string FID /*= ""*/, bool RefreshFlag /*= true*/ )
{
	bool flag = false;//�ú����ڵĴ����־
	Initialization();
	//�������
	if (ErrCheckFlag)
	{
		if (!FID.empty() && (!IsHex(FID) || FID.length() != 4))
		{
			Set_Error_Flag();
			serr << "ɾ���ļ�ָ�� Delete_File������FID������4��ʮ�������ַ���ɣ�����ǰΪ��" << FID << "����";
			Info->Append_Error(serr.str());
			flag = true;
		}
		if ( FType!="MF" && FType!="DDF" && FType!="ADF" && FType!="EF" )
		{
			Set_Error_Flag();
			serr << "ɾ���ļ�ָ�� Delete_File������FType����Ϊ\"MF\"��\"DDF\"��\"ADF\"��\"EF\"��һ�֣�����ǰΪ��" << FType << "����";
			Info->Append_Error(serr.str());
			flag = true;
		}
	}
	if (flag)
		return "";
	//ָ���
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
	//��β
	if (RefreshFlag)
		Info->Append_Script(sstr.str());
	return sstr.str();
}

string COS_T514::Delete_File(string MF, string DDF, string ADF, string EF, bool RefreshFlag /*= true*/ )
{
	return "�����Ⱑ��";
}

string COS_T514::Verify_TranCode( int Len, string Data, bool RefreshFlag /*= true*/ )
{
	bool flag = false;//�ú����ڵĴ����־
	string str;
	string estr;
	Initialization();
	//�������
	if (ErrCheckFlag)
	{
		if (Len<0 || Len>255)
		{
			Set_Error_Flag();
			serr << "������Կ��֤ Verify_TranCode(" << Len << ", " << Data << ")��ָ����������";
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
			serr << "������Կ��֤ Verify_TranCode(" << Len << ", " << Data << ")���������������" << estr;
			Info->Append_Error(serr.str());
			flag = true;
		}
		if (!Style_InstData.empty())
			Data = StrTrim(Data);
	}
	if (flag)
		return "";
	//ָ���
	sstr << "80" << Style_InstHead;
	sstr << "E0" << Style_InstHead;
	sstr << "03" << Style_InstHead;
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

string COS_T514::End_PreIssue( bool RefreshFlag /*= true*/ )
{
	Initialization();
	//ָ���
	sstr << "80" << Style_InstHead;
	sstr << "E0" << Style_InstHead;
	sstr << "02" << Style_InstHead;
	sstr << "00" << Style_InstHead;
	sstr << "00";
	strcpy(InstHead, sstr.str().c_str());
	//��β
	if (RefreshFlag)
		Info->Append_Script(sstr.str());
	return sstr.str();
}

string COS_T514::Create_Internal_EF(string FType, string DataLen, bool RefreshFlag /*= true*/)
{
	bool flag = false;//�ú����ڵĴ����־
	string efSFI = "";//���ļ���ʶ��
	string readCtrl = "";//������
	string writeCtrl = "";//д����
	string efType = "";//�ļ�����
	string recordLen = "";//��¼����
	string str = "";
	string estr = "";
	KEYDATAS keyData;
	Initialization();
	//�������
	if (ErrCheckFlag)
	{
		if (FType != "SF" && FType != "FCI" && FType != "DIR" && FType != "TF" && FType != "ATR")
		{
			Set_Error_Flag();
			serr << "�����ڲ��ļ�ָ�� Create_Internal_EF(" << FType << ", " << DataLen << ")��ָ�����ڲ��ļ����Ͳ���ȷ��";
			Info->Append_Error(serr.str());
			flag = true;
		}
		if (DataLen.length() > 4 || !IsHex(DataLen))
		{
			Set_Error_Flag();
			serr << "�����ڲ��ļ�ָ�� Create_Internal_EF(" << FType << ", " << DataLen << ")���ļ��ռ䲻��ȷ��";
			Info->Append_Error(serr.str());
			flag = true;
		}
	}
	//�ļ�����
	if (FType == "SF")
	{
		efSFI = SF_SFI;
		efType = "C3";
		recordLen = "18";
		readCtrl = Key_Forbidden + Key_Forbidden;//����ֹ
		writeCtrl = Key_Free + MK_Level;//д�ܵ�ǰĿ¼���ؿ���
	}
	else if (FType == "FCI")
	{
		efSFI = FCI_SFI;
		efType = "C3";
		recordLen = "FF";
		readCtrl = Key_Forbidden + Key_Forbidden;//����ֹ
		writeCtrl = Key_Free + MK_Level;//д�ܵ�ǰĿ¼���ؿ���
	}
	else if (FType == "ATR")
	{
		Set_Error_Flag();
		serr << "�����ڲ��ļ�ָ�� Create_Internal_EF(" << FType << ", " << DataLen << ")������T514оƬ��û����ν��ATR�ļ���";
		Info->Append_Error(serr.str());
		flag = true;
	}
	else if (FType == "DIR")
	{
		efSFI = DIR_SFI;
		efType = "D6";
		recordLen = "FF";
		readCtrl = Key_Free + Key_Free;//������
		writeCtrl = Key_Free + MK_Level;//д�ܵ�ǰĿ¼���ؿ���
	}
	else if (FType == "TF")
	{
		efSFI = TF_SFI;
		efType = "C3";
		recordLen = "00";
		readCtrl = Key_Forbidden + Key_Forbidden;//����ֹ
		if (Param->GetKeyInfo("SSSE", "DF04", "DSK", keyData) == false)
		{
			Set_Error_Flag();
			serr << "�����ڲ��ļ�ָ�� Create_Internal_EF(" << FType << ", " << DataLen << ")����SSSE��DF04���Ҳ�����ԿDSK��";
			Info->Append_Error(serr.str());
			flag = true;
		}
		else
			writeCtrl = Key_Free + Key_Security_Level(keyData);
	}
	if (flag)
		return  "";
	//ָ���
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
	//��β
	if (RefreshFlag)
		Info->Append_Script(sstr.str());
	return sstr.str();
}

string COS_T514::Create_File( string MF /*= ""*/, string DDF /*= ""*/, string ADF /*= ""*/, string EF /*= ""*/, bool RefreshFlag /*= true*/ )
{
	bool flag = false;//�ú����ڵĴ����־
	S_MFINFO mfInfo;
	S_DDFINFO ddfInfo;
	S_ADFINFO adfInfo;
	S_EFINFO efInfo;
	KEYDATAS keyData1;
	KEYDATAS keyData2;
	EFDATAS efData;//����������¼�ļ�ʱ������¼���ȼ��ж������¼��ʶ��
	bool efFlag = true;//����������¼�ļ���Ч����ֵΪtrueʱ��ʾ������¼�ļ�����Tag��Length�ֽ�
	string str;//��ʱ����
	string efSpace;//EF�ļ��ռ�
	string efType;//EF�ļ�����
	string efRecordLen;//EF��¼����
	string efSCW;//���ڼ���EF��SCW����Կ�߼���ϵ��
	string readCtrl = "";//������
	string writeCtrl = "";//д����
	int i;//��������
	Initialization();
	//�������
	if (ErrCheckFlag)
	{
		if (MF != "MF" && MF != "3F00" && MF != "")
		{
			Set_Error_Flag();
			serr << "�����ļ�ָ�� Create_File��" << MF << ", " << DDF << ", " << ADF << ", " << EF << "����MFֻ��ȡֵΪ��MF������3F00����մ�����������ǰ�ǡ�" << MF << "����";
			Info->Append_Error(serr.str());
			flag = true;
		}
		if (!ADF.empty() && (!IsHex(ADF) || ADF.length() != 4))
		{
			Set_Error_Flag();
			serr << "�����ļ�ָ�� Create_File��" << MF << ", " << DDF << ", " << ADF << ", " << EF << "��������ADF������4��ʮ�������ַ���ɣ�����ǰΪ��" << ADF << "����";
			Info->Append_Error(serr.str());
			flag = true;
		}
		if (!EF.empty() && (!IsHex(EF) || EF.length() != 4))
		{
			Set_Error_Flag();
			serr << "�����ļ�ָ�� Create_File��" << MF << ", " << DDF << ", " << ADF << ", " << EF << "��������EF������4��ʮ�������ַ���ɣ�����ǰΪ��" << EF << "����";
			Info->Append_Error(serr.str());
			flag = true;
		}
	}
	//����ָ��ͷ��
	sstr << "80" << Style_InstHead;
	sstr << "E0" << Style_InstHead;
	sstr << "01" << Style_InstHead;
	sstr << "00" << Style_InstHead;
	//��ȡ���� �� ����ָ��
	if (EF.empty() == false)//EF
	{
		if (Param->GetEFInfo(DDF, ADF, EF, efInfo) == false)
		{
			Set_Error_Flag();
			serr << "�����ļ�ָ�� Create_File��" << MF << ", " << DDF << ", " << ADF << ", " << EF << "�����Ҳ���ָ��·����EF�ļ���";
			Info->Append_Error(serr.str());
			flag = true;
		}
		//EF - ���ü��
		if (ErrCheckFlag)
		{
			if (efInfo.m_strSFI.length() % 2 != 0 || !IsHex(efInfo.m_strSFI))
			{
				Set_Error_Flag();
				serr << "�����ļ�ָ�� Create_File��" << MF << ", " << DDF << ", " << ADF << ", " << EF << "����ָ��EF�ļ���FID��" << efInfo.m_strSFI << "������";
				Info->Append_Error(serr.str());
				flag = true;
			}
		}
		//EF - �ļ�����
		if (efInfo.m_strEFType == EFTYPE_FIXED_LENGTH)//������¼
		{
			efSpace = DecToHex(HexToDec(efInfo.m_strEFLen) + HexToDec(efInfo.m_strResSpace) + efInfo.m_nRecordCount * 2 + 2);
			efType = "C5";
			if (ErrCheckFlag)
			{
				if (efInfo.m_nRecordCount == 0)
				{
					Set_Error_Flag();
					serr << "�����ļ�ָ�� Create_File��" << MF << ", " << DDF << ", " << ADF << ", " << EF << "��������������¼�ļ�ʱ��Ҫ��������һ����¼��ָ�������ļ��ļ�¼���ȣ�����ǰ���贴�����ļ��в�û���κμ�¼��Ϣ��";
					Info->Append_Error(serr.str());
					flag = true;
				}
				if (efInfo.m_nRecordCount > 0 && HexToDec(efInfo.m_strEFLen) % efInfo.m_nRecordCount != 0)
				{
					Set_Error_Flag();
					serr << "�����ļ�ָ�� Create_File��" << MF << ", " << DDF << ", " << ADF << ", " << EF << "����Ҫ�󶨳���¼�ļ��ļ�¼����֮�͡�0x" << efInfo.m_strEFLen << "�������Ǽ�¼��Ŀ��" << efInfo.m_nRecordCount << "���ı�����";
					Info->Append_Error(serr.str());
					flag = true;
				}
				if (efSpace.length() > 4)
				{
					Set_Error_Flag();
					serr << "�����ļ�ָ�� Create_File��" << MF << ", " << DDF << ", " << ADF << ", " << EF << "�����ļ��ռ䳬�����ļ����ݡ�0x" << efInfo.m_strEFLen << "�� + TagLength�ռ䡰" << efInfo.m_nRecordCount << "*2�� + Ԥ���ռ䡰0x" << efInfo.m_strResSpace << "�� + 2����";
					Info->Append_Error(serr.str());
					flag = true;
				}
				for (i = 1; i <= efInfo.m_nRecordCount; ++i)
				{
					if (Param->GetEFData(DDF, ADF, EF, i, efData) == false)
					{
						serr.clear();
						serr.str("");
						serr << "�����ļ�ָ�� Create_File��" << MF << ", " << DDF << ", " << ADF << ", " << EF << "������EF�ļ�ָ���ú��� " << efInfo.m_nRecordCount << "����¼����ȴ�Ҳ����� " << i << "����¼��";
						Info->Append_Warning(serr.str());
						break;
					}
					if ((efInfo.m_nRecordCount > 0) && (HexToDec(efInfo.m_strEFLen) / efInfo.m_nRecordCount != HexToDec(efData.m_strLen)))
					{
						Set_Error_Flag();
						serr << "�����ļ�ָ�� Create_File��" << MF << ", " << DDF << ", " << ADF << ", " << EF << "����ָ���Ķ�����¼�ļ�����¼���Ȳ���ȡ�";
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
							serr << "�����ļ�ָ�� Create_File��" << MF << ", " << DDF << ", " << ADF << ", " << EF << "����ָ���Ķ�����¼�ļ��У���Щ��¼���м�¼��ʶ������Щȴû�С�";
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
		else if (efInfo.m_strEFType == EFTYPE_VARIABLE_LENGTH)//�䳤��¼
		{
			efSpace = DecToHex(HexToDec(efInfo.m_strEFLen) + HexToDec(efInfo.m_strResSpace) + efInfo.m_nRecordCount * 2 + 2);
			efType = "C6";
			if (ErrCheckFlag)
			{
				if (efSpace.length() > 4)
				{
					Set_Error_Flag();
					serr << "�����ļ�ָ�� Create_File��" << MF << ", " << DDF << ", " << ADF << ", " << EF << "�����ļ��ռ䳬�����ļ����ݡ�0x" << efInfo.m_strEFLen << "�� + TagLength�ռ䡰" << efInfo.m_nRecordCount << "*2�� + Ԥ���ռ䡰0x" << efInfo.m_strResSpace << "�� + 2����";
					Info->Append_Error(serr.str());
					flag = true;
				}
			}
			efRecordLen = "FF";
		}
		else if (efInfo.m_strEFType == EFTYPE_BINARY)//͸���ļ�
		{
			efSpace = DecToHex(HexToDec(efInfo.m_strEFLen) + HexToDec(efInfo.m_strResSpace));
			efType = "C4";
			if (ErrCheckFlag)
			{
				if (efSpace.length() > 4)
				{
					Set_Error_Flag();
					serr << "�����ļ�ָ�� Create_File��" << MF << ", " << DDF << ", " << ADF << ", " << EF << "�����ļ��ռ䳬�����ļ����ݡ�0x" << efInfo.m_strEFLen << "�� + Ԥ���ռ䡰0x" << efInfo.m_strResSpace << "������";
					Info->Append_Error(serr.str());
					flag = true;
				}
			}
			efRecordLen = "00";
		}
		else if (efInfo.m_strEFType == EFTYPE_LOOP)//ѭ���ļ�
		{
			efSpace = DecToHex(HexToDec(efInfo.m_strEFLen) * atoi(efInfo.m_strnCount.c_str()) + HexToDec(efInfo.m_strResSpace) + 2);
			efType = "C7";
			if (ErrCheckFlag)
			{
				if (efSpace.length() > 4)
				{
					Set_Error_Flag();
					serr << "�����ļ�ָ�� Create_File��" << MF << ", " << DDF << ", " << ADF << ", " << EF << "�����ļ��ռ䳬�����ļ����ݡ�0x" << efInfo.m_strEFLen << "�� + Ԥ���ռ䡰0x" << efInfo.m_strResSpace << "�� + 2����";
					Info->Append_Error(serr.str());
					flag = true;
				}
				if (HexToDec(efInfo.m_strEFLen) == 0)
				{
					Set_Error_Flag();
					serr << "�����ļ�ָ�� Create_File��" << MF << ", " << DDF << ", " << ADF << ", " << EF << "����ָ���贴����ѭ���ļ��в�û�м�¼��Ϣ��";
					Info->Append_Error(serr.str());
					flag = true;
				}
			}
			efRecordLen = efInfo.m_strEFLen;
		}
		else//����
		{
			Set_Error_Flag();
			serr << "�����ļ�ָ�� Create_File��" << MF << ", " << DDF << ", " << ADF << ", " << EF << "����EF�ļ������͡�" << efInfo.m_strEFType << "������ȷ��";
			Info->Append_Error(serr.str());
			flag = true;
		}
		//EF - ��������Կ
		switch (EF_Control(DDF, ADF, efInfo.m_strReadControl, keyData1, keyData2, str))
		{
		case 1://����
			efSCW = "0";
			readCtrl = Key_Free + Key_Free;
			break;
		case 2://��ֹ
			efSCW = "0";
			readCtrl = Key_Forbidden + Key_Forbidden;
			break;
		case 3://�ϼ�Ŀ¼PIN
			efSCW = "0";
			readCtrl = Key_Security_Level(keyData1) + Key_Free;
			break;
		case 4://��ǰĿ¼��Կ
			efSCW = "0";
			readCtrl = Key_Free + Key_Security_Level(keyData1);
			break;
		case 5://�ϼ�����ǰĿ¼��Կ���롱�߼�
			efSCW = "4";
			readCtrl = Key_Security_Level(keyData1) + Key_Security_Level(keyData2);
			break;
		case 6://�ϼ�����ǰĿ¼��Կ�����߼�
			efSCW = "0";
			readCtrl = Key_Security_Level(keyData1) + Key_Security_Level(keyData2);
			break;
		default:
			Set_Error_Flag();
			serr << "�����ļ�ָ�� Create_File��" << MF << ", " << DDF << ", " << ADF << ", " << EF << "������Ȩ��״̬" << str;
			Info->Append_Error(serr.str());
			flag = true;
			break;
		}
		//EF - д������Կ
		switch (EF_Control(DDF, ADF, efInfo.m_strWriteControl, keyData1, keyData2, str))
		{
		case 1://����
			efSCW += "0";
			writeCtrl = Key_Free + Key_Free;
			break;
		case 2://��ֹ
			efSCW += "0";
			writeCtrl = Key_Forbidden + Key_Forbidden;
			break;
		case 3://�ϼ�Ŀ¼PIN
			efSCW = "0";
			readCtrl = Key_Security_Level(keyData1) + Key_Free;
			break;
		case 4://��ǰĿ¼��Կ
			efSCW += "0";
			writeCtrl = Key_Free + Key_Security_Level(keyData1);
			break;
		case 5://�ϼ�����ǰĿ¼��Կ���롱�߼�
			efSCW += "4";
			writeCtrl = Key_Security_Level(keyData1) + Key_Security_Level(keyData2);
			break;
		case 6://�ϼ�����ǰĿ¼��Կ�����߼�
			efSCW += "0";
			writeCtrl = Key_Security_Level(keyData1) + Key_Security_Level(keyData2);
			break;
		default:
			Set_Error_Flag();
			serr << "�����ļ�ָ�� Create_File��" << MF << ", " << DDF << ", " << ADF << ", " << EF << "����дȨ��״̬" << str;
			Info->Append_Error(serr.str());
			flag = true;
			break;
		}
		if (flag)
			return "";
		//EF - ����ָ��
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
			serr << "�����ļ�ָ�� Create_File��" << MF << ", " << DDF << ", " << ADF << ", " << EF << "�����Ҳ���ָ��·����ADF�ļ���";
			Info->Append_Error(serr.str());
			flag = true;
		}
		//ADF - ���ü��
		if (ErrCheckFlag)
		{
			if (adfInfo.m_strAID.length() % 2 != 0 || !IsHex(adfInfo.m_strAID))
			{
				Set_Error_Flag();
				serr << "�����ļ�ָ�� Create_File��" << MF << ", " << DDF << ", " << ADF << ", " << EF << "����ָ��ADF�ļ���AID��" << adfInfo.m_strAID << "������";
				Info->Append_Error(serr.str());
				flag = true;
			}
			if (adfInfo.m_strFID.length() != 4 || !IsHex(adfInfo.m_strFID))
			{
				Set_Error_Flag();
				serr << "�����ļ�ָ�� Create_File��" << MF << ", " << DDF << ", " << ADF << ", " << EF << "����ָ��ADF�ļ���FID��" << adfInfo.m_strFID << "������";
				Info->Append_Error(serr.str());
				flag = true;
			}
		}
		if (flag)
			return "";
		//ADF - ����ָ��
		sstr << setfill('0') << setw(2) << hex << uppercase << right << 0x10 + adfInfo.m_strAID.length() / 2 << Style_Separator;
		sstr << "00";
		if (Param->GetKeyInfo(DDF, ADF, "LK" + ADF, keyData1))//Ӧ������Կ�ĺ�����ȫ״̬
			sstr << Key_Security_Level(keyData1);
		else
			sstr << "00";
		sstr << Key_Free << MK_Level;//����ɾ��Ȩ��
		sstr << "01" << Style_InstData;//Ԥ���˻���Ӧ�ý׶ζ�ʹ��DES�㷨
		sstr << ToUpper(adfInfo.m_strFID) << Style_InstData;
		sstr << "02";//ADF���ļ����ͣ��̶�Ϊ"02"
		sstr << "00" << "00";
		sstr << "00";//FCI_SFI
		sstr << "00";//��ȫ�����ֽ�
		sstr << "00";//��������
		sstr << "00";//������
		sstr << "00";//������
		sstr << "00" << Style_InstData;//������
		sstr << ToUpper(adfInfo.m_strAID);
	}
	else if (DDF.empty() == false)//DDF
	{
		if (Param->GetDDFInfo(DDF, ddfInfo) == false)
		{
			Set_Error_Flag();
			serr << "�����ļ�ָ�� Create_File��" << MF << ", " << DDF << ", " << ADF << ", " << EF << "�����Ҳ���ָ��·����DDF�ļ���";
			Info->Append_Error(serr.str());
			flag = true;
		}
		//DDF - ���ü��
		if (ErrCheckFlag)
		{
			if (ddfInfo.m_strAID.length() % 2 != 0 || !IsHex(ddfInfo.m_strAID))
			{
				Set_Error_Flag();
				serr << "�����ļ�ָ�� Create_File��" << MF << ", " << DDF << ", " << ADF << ", " << EF << "����ָ��DDF�ļ���AID��" << ddfInfo.m_strAID << "������";
				Info->Append_Error(serr.str());
				flag = true;
			}
			if (ddfInfo.m_strFID.length() != 4 || !IsHex(ddfInfo.m_strFID))
			{
				Set_Error_Flag();
				serr << "�����ļ�ָ�� Create_File��" << MF << ", " << DDF << ", " << ADF << ", " << EF << "����ָ��DDF�ļ���FID��" << ddfInfo.m_strFID << "������";
				Info->Append_Error(serr.str());
				flag = true;
			}
		}
		if (flag)
			return "";
		//DDF - ����ָ��
		sstr << setfill('0') << setw(2) << hex << uppercase << right << 0x10 + ddfInfo.m_strAID.length() / 2 << Style_Separator;
		sstr << "00";
		if (Param->GetKeyInfo(DDF, ADF, "BK", keyData1) == false)//������Կ
		{
			if (DDF == "SSSE")
			{
				Set_Error_Flag();
				serr << "�����ļ�ָ�� Create_File��" << MF << ", " << DDF << ", " << ADF << ", " << EF << "��������SSSEʱ�Ҳ�����ԿBK��";
				Info->Append_Error(serr.str());
				flag = true;
			}
			else
				sstr << "00";
		}
		else
			sstr << Key_Security_Level(keyData1);
		sstr << Key_Free << MK_Level;//����ɾ��Ȩ��
		sstr << "01" << Style_InstData;//Ԥ���˻���Ӧ�ý׶ζ�ʹ��DES�㷨
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
		//MF - ���ü��
		if (ErrCheckFlag)
		{
			if (mfInfo.m_strAID.length() % 2 != 0 || !JudgeHex(mfInfo.m_strAID))//����Ϊ��
			{
				Set_Error_Flag();
				serr << "�����ļ�ָ�� Create_File��" << MF << ", " << DDF << ", " << ADF << ", " << EF << "����ָ��MF�ļ���AID��" << mfInfo.m_strAID << "������";
				Info->Append_Error(serr.str());
				flag = true;
			}
			if (mfInfo.m_strFID != "3F00")
			{
				Set_Error_Flag();
				serr << "�����ļ�ָ�� Create_File��" << MF << ", " << DDF << ", " << ADF << ", " << EF << "����MF�ļ���FIDҪ������ǡ�3F00��������ǰȴ�ǡ�" << mfInfo.m_strFID << "����";
				Info->Append_Error(serr.str());
				flag = true;
			}
		}
		if (flag)
			return "";
		//MF - ����ָ��
		sstr << setfill('0') << setw(2) << hex << uppercase << right << 0x10 + mfInfo.m_strAID.length() / 2 << Style_Separator;
		sstr << "00";
		sstr << "00";//������Կ
		sstr << "00" << MK_Level;//����ɾ��Ȩ����
		sstr << "01" << Style_InstData;
		sstr << ToUpper(mfInfo.m_strFID) << Style_InstData;
		sstr << "01";
		sstr << "00";
		sstr << "01";//�籣DDFִ�п�����ͬʱ����MF
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
	//��β
	if (RefreshFlag)
		Info->Append_Script(sstr.str());
	return sstr.str();
}

string COS_T514::Write_Key(string MF, string DDF, string ADF, string KeyName, string KeyVer, bool RefreshFlag /*= true*/ )
{
	bool flag = false;//�ú����ڵĴ����־
	S_MFINFO mfInfo;
	S_DDFINFO ddfInfo;
	S_ADFINFO adfInfo;
	KEYDATAS keyData;
	string keyLen;//��Կ����
	string keyValue;//��Կֵ
	Initialization();
	//��ȡָ��ͷ������ͷ
	if (Write_Key(MF, DDF, ADF, KeyName, KeyVer, false, false) == false)
		return "";
	//ȡ��Կֵ������һ������ɴ����鹤�����˴������ټ���
	if (KeyName.compare(0, 2, "MK") == 0)//��װ����
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
			serr << "װ�ع̶���Կ Write_MKey��" << MF << ", " << DDF << ", " << ADF << ", " << KeyName << ", " << KeyVer << "����ָ����Կ��ֵ����";
			Info->Append_Error(serr.str());
			flag = true;
		}
		if (keyValue.length() != HexToDec(keyLen) * 2)
		{
			Set_Error_Flag();
			serr << "װ�ع̶���Կ Write_MKey��" << MF << ", " << DDF << ", " << ADF << ", " << KeyName << ", " << KeyVer << "����ָ����Կ����Կֵ����Կ���Ȳ������";
			Info->Append_Error(serr.str());
			flag = true;
		}
	}
	if (flag)
		return "";
	//ָ���
	sstr.clear();
	sstr.str("");
	sstr << Get_Instruction_Head().substr(0, 2) << Style_InstHead;
	sstr << Get_Instruction_Head().substr(2, 2) << Style_InstHead;
	sstr << Get_Instruction_Head().substr(4, 2) << Style_InstHead;
	sstr << Get_Instruction_Head().substr(6, 2) << Style_InstHead;
	sstr << Get_Instruction_Head().substr(8, 2) << Style_Separator;
	sstr << Get_Instruction_Data() << Style_InstData;
	sstr << setfill('F') << setw(32) << left << ToUpper(keyValue);
	//��β
	if (RefreshFlag)
		Info->Append_Script(sstr.str());
	return sstr.str();
}

bool COS_T514::Write_Key(string MF, string DDF, string ADF, string KeyName, string KeyVer, bool Encrypt, bool Mac)
{
	bool flag = false;//�ú����ڵĴ����־
	S_MFINFO mfInfo;
	S_DDFINFO ddfInfo;
	S_ADFINFO adfInfo;
	KEYDATAS keyData;
	string keyType;//��Կ����
	string keyID;//��Կ��־
	string keyCnt;//���������
	string keyByte6;//��Կͷ�ĵ����ֽڣ�PIN����ӦPUK�İ�ȫ����������Կ��ʾ�㷨��־
	string keyLevel;//������ȫ����
	string keyLen;//��Կ����
	string keyValue;//��Կֵ�����ڰ�װ����ʱ��ʹ�õ�
	Initialization();
	//�������
	if (ErrCheckFlag)
	{
		if (MF != "MF" && MF != "3F00" && MF != "")
		{
			Set_Error_Flag();
			serr << "װ����Կ Write_Key��" << MF << ", " << DDF << ", " << ADF << ", " << KeyName << ", " << KeyVer << "����MFֻ��ȡֵΪ��MF������3F00����մ�����������ǰ�ǡ�" << MF << "����";
			Info->Append_Error(serr.str());
			flag = true;
		}
		if (!ADF.empty() && (!IsHex(ADF) || ADF.length() != 4))
		{
			Set_Error_Flag();
			serr << "װ����Կ Write_Key��" << MF << ", " << DDF << ", " << ADF << ", " << KeyName << ", " << KeyVer << "��������ADF������4��ʮ�������ַ���ɣ�����ǰΪ��" << ADF << "����";
			Info->Append_Error(serr.str());
			flag = true;
		}
		if (KeyVer.length() != 2 || IsHex(KeyVer) == false)
		{
			Set_Error_Flag();
			serr << "װ����Կ Write_Key��" << MF << ", " << DDF << ", " << ADF << ", " << KeyName << ", " << KeyVer << "������Կ�汾��������λʮ�������ַ���ɣ�����ǰΪ��" << KeyVer << "����";
			Info->Append_Error(serr.str());
			flag = true;
		}
	}
	//��ȡ����
	if (KeyName.compare(0, 2, "MK") == 0)//��װ����
	{
		keyType = "00";
		keyID = "00";
		if (ErrCheckFlag && KeyVer != "00")
		{
			serr.clear();
			serr.str("");
			serr << "װ��������Կ Write_MKey��" << MF << ", " << DDF << ", " << ADF << ", " << KeyName << ", " << KeyVer << "����װ��������Կʱ����Կ�汾Ӧ��Ϊ��00��������ǰ�ǡ�" << KeyVer << "����";
			Info->Append_Warning(serr.str());
		}
		keyByte6 = "00";
		keyLevel = MK_Level;
		if (ADF.empty() == false)//ADF
		{
			if (Param->GetADFInfo(DDF, ADF, adfInfo) == false)
			{
				Set_Error_Flag();
				serr << "װ��������Կ Write_MKey��" << MF << ", " << DDF << ", " << ADF << ", " << KeyName << ", " << KeyVer << "�����Ҳ���ָ����ADF�ļ���";
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
				serr << "װ��������Կ Write_MKey��" << MF << ", " << DDF << ", " << ADF << ", " << KeyName << ", " << KeyVer << "�����Ҳ���ָ����DDF�ļ���";
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
				serr << "װ��������Կ Write_MKey��" << MF << ", " << DDF << ", " << ADF << ", " << KeyName << ", " << KeyVer << "����ָ��Ŀ¼��������Կֵ����";
				Info->Append_Error(serr.str());
				flag = true;
			}
			if (keyValue.length() != 32)
			{
				serr.clear();
				serr.str("");
				serr << "װ��������Կ Write_MKey��" << MF << ", " << DDF << ", " << ADF << ", " << KeyName << ", " << KeyVer << "����ָ��Ŀ¼��������Կֵ���Ȳ���16���ֽڡ�";
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
			serr << "װ����Կ Write_Key��" << MF << ", " << DDF << ", " << ADF << ", " << KeyName << ", " << KeyVer << "�����Ҳ���ָ������Կ��";
			Info->Append_Error(serr.str());
			flag = true;
		}
		keyCnt = keyData.m_strCount;
		switch (HexToDec(keyData.m_strLen))
		{
		case 8:
			keyByte6 = "01";//DES�㷨
			break;
		case 0x10:
			keyByte6 = "00";//3DES
			break;
		default:
			if (keyData.m_strName == "PIN")
				break;
			Set_Error_Flag();
			serr << "װ����Կ Write_Key��" << MF << ", " << DDF << ", " << ADF << ", " << KeyName << ", " << KeyVer << "������Կ����ֻ����0x08���ֽڻ�0x10���ֽڣ�����ǰ�ǡ�" << keyData.m_strLen << "����";
			Info->Append_Error(serr.str());
			flag = true;
			break;
		}
		keyLevel = Key_Security_Level(keyData);
		keyLen = keyData.m_strLen;
		//��Կ��־��������DLK��DPK����Կ�����⴦��
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
					serr << "װ����Կ Write_Key��" << MF << ", " << DDF << ", " << ADF << ", " << KeyName << ", " << KeyVer << "��������Կ��־��" << keyData.m_strKeyIndex << "�������У��Ҳ�����Կ�汾��" << KeyVer << "����ָ���ĵ� " << HexToDec(KeyVer) << " ����־(ע�⣺����Կ��־�����У��������ԡ�|����β)��";
					Info->Append_Error(serr.str());
					flag = true;
					break;
				}
				pos = e + 1;
			}
			if (HexToDec(KeyVer) == 0)
			{
				Set_Error_Flag();
				serr << "װ����Կ Write_Key��" << MF << ", " << DDF << ", " << ADF << ", " << KeyName << ", " << KeyVer << "��������Կ��־���ж��ʱ��" << keyData.m_strKeyIndex << "������Կ�汾��" << KeyVer << "������Ϊ00��";
				Info->Append_Error(serr.str());
				flag = true;
				keyID == "00";
			}
			else if (e == string::npos)
				keyID == "00";
			else
				keyID = keyData.m_strKeyIndex.substr(s, e - s);
		}
		//��Կ����
		if (keyData.m_strName == "IRK")
			keyType = "0E";
		else if (keyData.m_strName == "PIN")
		{
			keyType = "01";
			keyID = "00";
			if (Param->GetKeyInfo(DDF, ADF, "PUK", keyData) == false)
			{
				Set_Error_Flag();
				serr << "װ����Կ Write_Key��" << MF << ", " << DDF << ", " << ADF << ", " << KeyName << ", " << KeyVer << "������װPINʱ��ͬһĿ¼·�����Ҳ�����ԿPUK��";
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
		else//������Կ���������ⲿ��֤��Կ
			keyType = "0D";
	}
	//���ü��
	if (ErrCheckFlag)
	{
		if (keyID.length() != 2 || IsHex(keyID) == false)
		{
			Set_Error_Flag();
			serr << "װ����Կ Write_Key��" << MF << ", " << DDF << ", " << ADF << ", " << KeyName << ", " << KeyVer << "������Կ��־��������λʮ�������ַ���ɣ�����ǰΪ��" << keyID << "����";
			Info->Append_Error(serr.str());
			flag = true;
		}
		if (keyCnt.length() != 2 || IsHex(keyCnt) == false)
		{
			Set_Error_Flag();
			serr << "װ����Կ Write_Key��" << MF << ", " << DDF << ", " << ADF << ", " << KeyName << ", " << KeyVer << "���������������������λʮ�������ַ���ɣ�����ǰΪ��" << keyCnt << "����";
			Info->Append_Error(serr.str());
			flag = true;
		}
		if (KeyName != "PIN" && keyLen != "08" && keyLen != "10")
		{
			Set_Error_Flag();
			serr << "װ����Կ Write_Key��" << MF << ", " << DDF << ", " << ADF << ", " << KeyName << ", " << KeyVer << "������Կ���ȱ���Ϊ��08�����ߡ�10��������ǰΪ��" << keyLen << "����";
			Info->Append_Error(serr.str());
			flag = true;
		}
	}
	if (flag)
		return false;
	//����ָ��ͷ��
	sstr << (Encrypt ? "8" : "0");
	sstr << (Mac ? "4" : "0") << Style_InstHead;
	sstr << "D4" << Style_InstHead;
	sstr << ((KeyName.compare(0, 2, "MK")) ? "00" : "01") << Style_InstHead;
	sstr << "00" << Style_InstHead;
	sstr << setfill('0') << setw(2) << hex << uppercase << right << (Encrypt ? 0x20 : 0x18) + (Mac ? 4 : 0) << Style_Separator;
	strcpy(InstHead, StrTrim(sstr.str()).c_str());
	//������Կͷ��
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
	bool flag = false;//�ú����ڵĴ����־
	S_MFINFO mfInfo;
	S_DDFINFO ddfInfo;
	S_ADFINFO adfInfo;
	string fid;
	string aid;
	Initialization();
	//�������
	if (ErrCheckFlag)
	{
		if (MF != "MF" && MF != "3F00" && MF != "")
		{
			Set_Error_Flag();
			serr << "���FCI��¼ FCI_Record(" << RecordNo << ", " << MF << ", " << DDF << ", " << ADF << ")��MFֻ��ȡֵΪ��MF������3F00����մ�����������ǰ�ǡ�" << MF << "����";
			Info->Append_Error(serr.str());
			flag = true;
		}
		if (!ADF.empty() && (!IsHex(ADF) || ADF.length() != 4))
		{
			Set_Error_Flag();
			serr << "���FCI��¼ FCI_Record(" << RecordNo << ", " << MF << ", " << DDF << ", " << ADF << ")������ADF������4��ʮ�������ַ���ɣ�����ǰΪ��" << ADF << "����";
			Info->Append_Error(serr.str());
			flag = true;
		}
		if (RecordNo < 0)
		{
			serr.clear();
			serr.str("");
			serr << "���FCI��¼ FCI_Record(" << RecordNo << ", " << MF << ", " << DDF << ", " << ADF << ")����¼��С��0��";
			Info->Append_Warning(serr.str());
		}
		if (flag)
			return "";
	}
	//��ȡ����
	if (ADF.empty() == false)
	{
		if (Param->GetADFInfo(DDF, ADF, adfInfo) == false)
		{
			Set_Error_Flag();
			serr << "���FCI��¼ FCI_Record(" << RecordNo << ", " << MF << ", " << DDF << ", " << ADF << ")���������ļ����Ҳ���ָ��·����ADF�ļ���";
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
			serr << "���FCI��¼ FCI_Record(" << RecordNo << ", " << MF << ", " << DDF << ", " << ADF << ")���������ļ����Ҳ���ָ��·����DDF�ļ���";
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
	//���ü��
	if (ErrCheckFlag)
	{
		if (fid.length() != 4 || !IsHex(fid))
		{
			Set_Error_Flag();
			serr << "���FCI��¼ FCI_Record(" << RecordNo << ", " << MF << ", " << DDF << ", " << ADF << ")��ָ��Ŀ¼�ļ���FID(" << fid << ")��ʽ����";
			Info->Append_Error(serr.str());
			flag = true;
		}
		if (aid.length() % 2 != 0 || !IsHex(aid))
		{
			Set_Error_Flag();
			serr << "���FCI��¼ FCI_Record(" << RecordNo << ", " << MF << ", " << DDF << ", " << ADF << ")��ָ��Ŀ¼�ļ���AID(" << fid << ")��������򺬷Ƿ��ַ���";
			Info->Append_Error(serr.str());
			flag = true;
		}
	}
	if (flag)
		return "";
	//ָ���
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
	if (ADF.empty())//����ADFĿ¼����Ҫ���Ŀ¼�����Ϣ
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
	//��β
	if (RefreshFlag)
		Info->Append_Script(sstr.str());
	return sstr.str();
}

/**************************************
���ܣ�����ָ����Կ�İ�ȫ����
������
IN��KEYInfo���ѻ�ȡ������Կ��Ϣ�ṹ��
���أ�
�����2���ַ���ɵİ�ȫ�����������������մ�
ע�⣺��ͬ��COS�в�һ���İ�ȫ������뷽ʽ
**************************************/
string COS_T514::Key_Security_Level(KEYDATAS KEYInfo)
{
	bool flag = false;//�ú����ڵĴ����־
	//�������
	if (KEYInfo.m_nSeq < 0 || KEYInfo.m_nSeq > 0xF)
	{
		serr.clear();
		serr.str("");
		serr << "������Կ��ȫ���� Key_Security_Level��ָ����Կ" << KEYInfo.m_strName << "��˳��ţ�" << KEYInfo.m_nSeq << "����������Χ0-F�ڡ������Ŀ¼λ���µ���Կ�����Ƿ���ࡣ�������ֽ��Զ�ʹ��һ�µİ�ȫ����";
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
	bool flag = false;//�ú����ڵĴ����־
	Initialization();
	//�������
	if (ErrCheckFlag)
	{
		if (!IsHex(strATRHead) || strATRHead.length() != 22)
		{
			Set_Error_Flag();
			serr << "����ATRָ�� Update_ATR������strATRHead������22��ʮ�������ַ���ɣ�����ǰΪ��" << strATRHead << "����";
			Info->Append_Error(serr.str());
			flag = true;
		}
		if (!IsHex(strCardSN) || strCardSN.length() != 12)
		{
			Set_Error_Flag();
			serr << "����ATRָ�� Update_ATR������strCardSN������12��ʮ�������ַ���ɣ�����ǰΪ��" << strCardSN << "����";
			Info->Append_Error(serr.str());
			flag = true;
		}
	}
	if (flag)
		return "";
	//ָ���
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
	//��β
	if (RefreshFlag)
		Info->Append_Script(sstr.str());
	return sstr.str();
}