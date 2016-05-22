//#####################################
//
//	���� COSָ��
//
//	����COS�����籣����籣���׸��˻�ָ�Ŀǰ���壺��ʼ����ʱ���÷��籣��ʽ����һ�δ���DDF�͸�ΪдΪtrue
//
//	����޸����ڣ�2015-11-09 14:34
//#####################################

#include "chip.h"
#include "tool.h"
#include <iomanip>

const string COS_FM::MK_Level = "FF";//���籣Ӧ�ã�������Կ�İ�ȫ����
const string COS_FM::Key_Forbidden = "EF";//���籣Ӧ�ã���ֹȨ��
const string COS_FM::Key_Free = "00";//���籣Ӧ�ã���������
const string COS_FM::SSSE_MK_Level = "40";//�籣Ӧ�ã�������Կ�İ�ȫ����
const string COS_FM::SSSE_Key_Forbidden = "08";//�籣Ӧ�ã���ֹȨ��
const string COS_FM::SSSE_Key_Free = "00";//�籣Ӧ�ã���������
const int COS_FM::FileExtSpace = 0x08;//EF�ļ�����չ�ռ�




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
	//bool flag = false;//�ú����ڵĴ����־
	//bool extflag = false;//��־�Ŵ���EF�ļ�ʱ��дȨ���Ƿ�ʹ������չ������ȫ״̬�ֽ�
	//bool efFlag = true;//����������¼�ļ���Ч����ֵΪtrueʱ��ʾ������¼�ļ�����Tag��Length�ֽ�
	//S_MFINFO mfInfo;
	//S_DDFINFO ddfInfo;
	//S_ADFINFO adfInfo;
	//S_EFINFO efInfo;
	//KEYDATAS keyData1;
	//KEYDATAS keyData2;
	//EFDATAS efData;//����������¼�ļ�ʱ������¼���ȼ��ж������¼��ʶ��
	//string efSpace;//EF�ļ��ռ�
	//string efType;//EF�ļ�����
	//string efByte4 = "";//��4�ֽڣ�һ���Ƕ�Ȩ��
	//string efByte5 = "";//��5�ֽڣ�һ����дȨ��
	//string efByte6 = "";//��6�ֽڣ���չ��ȫ״̬�ֽ�
	//string efByte7 = "";//��7�ֽڣ���·����
	//int i;//��������
	//string str;//��ʱ����
	//Initialization();
	////�������
	//if (ErrCheckFlag)
	//{
	//	if (MF != "MF" && MF != "3F00" && MF != "")
	//	{
	//		Set_Error_Flag();
	//		serr << "�����ļ�ָ�� Create_File��" << MF << ", " << DDF << ", " << ADF << ", " << EF << "����MFֻ��ȡֵΪ��MF������3F00����մ�����������ǰ�ǡ�" << MF << "����";
	//		Info->Append_Error(serr.str());
	//		flag = true;
	//	}
	//	if (!ADF.empty() && (!IsHex(ADF) || ADF.length() != 4))
	//	{
	//		Set_Error_Flag();
	//		serr << "�����ļ�ָ�� Create_File��" << MF << ", " << DDF << ", " << ADF << ", " << EF << "��������ADF������4��ʮ�������ַ���ɣ�����ǰΪ��" << ADF << "����";
	//		Info->Append_Error(serr.str());
	//		flag = true;
	//	}
	//	if (!EF.empty() && (!IsHex(EF) || EF.length() != 4))
	//	{
	//		Set_Error_Flag();
	//		serr << "�����ļ�ָ�� Create_File��" << MF << ", " << DDF << ", " << ADF << ", " << EF << "��������EF������4��ʮ�������ַ���ɣ�����ǰΪ��" << EF << "����";
	//		Info->Append_Error(serr.str());
	//		flag = true;
	//	}
	//}
	////����ָ��ͷ��
	//sstr << "80" << Style_InstHead;
	//sstr << "E0" << Style_InstHead;
	//sstr << "01" << Style_InstHead;
	//sstr << "00" << Style_InstHead;
	////��ȡ���� �� ����ָ��
	//if (EF.empty() == false)//EF
	//{
	//	if (Param->GetEFInfo(DDF, ADF, EF, efInfo) == false)
	//	{
	//		Set_Error_Flag();
	//		serr << "�����ļ�ָ�� Create_File��" << MF << ", " << DDF << ", " << ADF << ", " << EF << "�����Ҳ���ָ��·����EF�ļ���";
	//		Info->Append_Error(serr.str());
	//		flag = true;
	//	}
	//	//EF - ���ü��
	//	if (ErrCheckFlag)
	//	{
	//		if (efInfo.m_strSFI.length() % 2 != 0 || !IsHex(efInfo.m_strSFI))
	//		{
	//			Set_Error_Flag();
	//			serr << "�����ļ�ָ�� Create_File��" << MF << ", " << DDF << ", " << ADF << ", " << EF << "����ָ��EF�ļ���FID��" << efInfo.m_strSFI << "������";
	//			Info->Append_Error(serr.str());
	//			flag = true;
	//		}
	//	}
	//	//EF - �ļ�����
	//	if (efInfo.m_strEFType == EFTYPE_FIXED_LENGTH)//������¼
	//	{
	//		if (ErrCheckFlag)
	//		{
	//			if (efInfo.m_nRecordCount == 0)
	//			{
	//				Set_Error_Flag();
	//				serr << "�����ļ�ָ�� Create_File��" << MF << ", " << DDF << ", " << ADF << ", " << EF << "��������������¼�ļ�ʱ��Ҫ��������һ����¼��ָ�������ļ��ļ�¼���ȣ�����ǰ���贴�����ļ��в�û���κμ�¼��Ϣ��";
	//				Info->Append_Error(serr.str());
	//				flag = true;
	//			}
	//			if (efInfo.m_nRecordCount > 0 && HexToDec(efInfo.m_strEFLen) % efInfo.m_nRecordCount != 0)
	//			{
	//				Set_Error_Flag();
	//				serr << "�����ļ�ָ�� Create_File��" << MF << ", " << DDF << ", " << ADF << ", " << EF << "����Ҫ�󶨳���¼�ļ��ļ�¼����֮�͡�0x" << efInfo.m_strEFLen << "�������Ǽ�¼��Ŀ��" << efInfo.m_nRecordCount << "���ı�����";
	//				Info->Append_Error(serr.str());
	//				flag = true;
	//			}
	//			if (efInfo.m_nRecordCount > 0 && HexToDec(efInfo.m_strEFLen) / efInfo.m_nRecordCount > 0xFF)
	//			{
	//				Set_Error_Flag();
	//				serr << "�����ļ�ָ�� Create_File��" << MF << ", " << DDF << ", " << ADF << ", " << EF << "����������¼���ȳ�����0xFF�Ĵ�С��";
	//				Info->Append_Error(serr.str());
	//				flag = true;
	//			}
	//			for (i = 1; i <= efInfo.m_nRecordCount; ++i)
	//			{
	//				if (Param->GetEFData(DDF, ADF, EF, i, efData) == false)
	//				{
	//					serr.clear();
	//					serr.str("");
	//					serr << "�����ļ�ָ�� Create_File��" << MF << ", " << DDF << ", " << ADF << ", " << EF << "������EF�ļ�ָ���ú��� " << efInfo.m_nRecordCount << "����¼����ȴ�Ҳ����� " << i << "����¼��";
	//					Info->Append_Warning(serr.str());
	//					break;
	//				}
	//				if ((efInfo.m_nRecordCount > 0) && (HexToDec(efInfo.m_strEFLen) / efInfo.m_nRecordCount != HexToDec(efData.m_strLen)))
	//				{
	//					Set_Error_Flag();
	//					serr << "�����ļ�ָ�� Create_File��" << MF << ", " << DDF << ", " << ADF << ", " << EF << "����ָ���Ķ�����¼�ļ�����¼���Ȳ���ȡ�";
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
	//						serr << "�����ļ�ָ�� Create_File��" << MF << ", " << DDF << ", " << ADF << ", " << EF << "����ָ���Ķ�����¼�ļ��У���Щ��¼���м�¼��ʶ������Щȴû�С�";
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
	//	else if (efInfo.m_strEFType == EFTYPE_VARIABLE_LENGTH)//�䳤��¼
	//	{
	//		efSpace = DecToHex(HexToDec(efInfo.m_strEFLen) + HexToDec(efInfo.m_strResSpace) + efInfo.m_nRecordCount * 2 + FileExtSpace);
	//		efType = "2C";
	//		if (ErrCheckFlag)
	//		{
	//			if (efSpace.length() > 4)
	//			{
	//				Set_Error_Flag();
	//				serr << "�����ļ�ָ�� Create_File��" << MF << ", " << DDF << ", " << ADF << ", " << EF << "�����ļ��ռ䳬�����ļ����ݡ�0x" << efInfo.m_strEFLen << "�� + TagLength�ռ䡰" << efInfo.m_nRecordCount << "*2�� + Ԥ���ռ䡰0x" << efInfo.m_strResSpace << "�� + 2����";
	//				Info->Append_Error(serr.str());
	//				flag = true;
	//			}
	//		}
	//	}
	//	else if (efInfo.m_strEFType == EFTYPE_BINARY)//͸���ļ�
	//	{
	//		efSpace = DecToHex(HexToDec(efInfo.m_strEFLen) + HexToDec(efInfo.m_strResSpace));//͸���ļ�������չ�ռ�
	//		efType = "28";
	//		if (ErrCheckFlag)
	//		{
	//			if (efSpace.length() > 4)
	//			{
	//				Set_Error_Flag();
	//				serr << "�����ļ�ָ�� Create_File��" << MF << ", " << DDF << ", " << ADF << ", " << EF << "�����ļ��ռ䳬�����ļ����ݡ�0x" << efInfo.m_strEFLen << "�� + Ԥ���ռ䡰0x" << efInfo.m_strResSpace << "������";
	//				Info->Append_Error(serr.str());
	//				flag = true;
	//			}
	//		}
	//	}
	//	else if (efInfo.m_strEFType == EFTYPE_LOOP)//ѭ���ļ�
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
	//				serr << "�����ļ�ָ�� Create_File��" << MF << ", " << DDF << ", " << ADF << ", " << EF << "��������ѭ���ļ���û��ָ�����ٵļ�¼������";
	//				Info->Append_Error(serr.str());
	//				flag = true;
	//			}
	//			if (HexToDec(efInfo.m_strEFLen) == 0)
	//			{
	//				Set_Error_Flag();
	//				serr << "�����ļ�ָ�� Create_File��" << MF << ", " << DDF << ", " << ADF << ", " << EF << "����ָ���贴����ѭ���ļ��в�û�м�¼��Ϣ��";
	//				Info->Append_Error(serr.str());
	//				flag = true;
	//			}
	//		}
	//	}
	//	else//����
	//	{
	//		Set_Error_Flag();
	//		serr << "�����ļ�ָ�� Create_File��" << MF << ", " << DDF << ", " << ADF << ", " << EF << "����EF�ļ������͡�" << efInfo.m_strEFType << "������ȷ��";
	//		Info->Append_Error(serr.str());
	//		flag = true;
	//	}
	//	//EF - ��������Կ
	//	switch (EF_Control(DDF, ADF, efInfo.m_strReadControl, keyData1, keyData2, str))
	//	{
	//	case 1://����
	//		efByte4 = SSSE_Flag ? SSSE_Key_Free : Key_Free;
	//		break;
	//	case 2://��ֹ
	//		efByte4 = SSSE_Flag ? SSSE_Key_Forbidden : Key_Forbidden;
	//		break;
	//	case 3://�ϼ�Ŀ¼PIN
	//		efByte4 = SSSE_Flag ? "88" : Key_Security_Level(keyData1);
	//		break;
	//	case 4://��ǰĿ¼��Կ
	//		if (SSSE_Flag)
	//		{
	//			str = Key_Security_Level(keyData1);
	//			if (HexToDec(str) > 0x80)//ʹ����չ��ȫ״̬�ֽ�
	//			{
	//				Set_Error_Flag();
	//				serr << "�����ļ�ָ�� Create_File��" << MF << ", " << DDF << ", " << ADF << ", " << EF << "�����ļ���Ȩ����Կ�Ķ�Ӧ������ȫ״̬��Ӧ��ʹ����չ��ȫ״̬�ֽڡ�";
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
	//	case 5://�ϼ�����ǰĿ¼��Կ���롱�߼�
	//		if (SSSE_Flag)
	//		{
	//			if (keyData1.m_strName != "PIN")
	//			{
	//				Set_Error_Flag();
	//				serr << "�����ļ�ָ�� Create_File��" << MF << ", " << DDF << ", " << ADF << ", " << EF << "������Ȩ�޵��ϼ�Ŀ¼�ܿ���Կֻ��ΪPIN������ǰ�ǡ�" << keyData1.m_strName << "����";
	//				Info->Append_Error(serr.str());
	//				flag = true;
	//			}

	//		}
	//		
	//		break;
	//	case 6://�ϼ�����ǰĿ¼��Կ�����߼�
	//		efSCW = "0";
	//		readCtrl = Key_Security_Level(keyData1) + Key_Security_Level(keyData2);
	//		break;
	//	default:
	//		Set_Error_Flag();
	//		serr << "�����ļ�ָ�� Create_File��" << MF << ", " << DDF << ", " << ADF << ", " << EF << "������Ȩ��״̬" << str;
	//		Info->Append_Error(serr.str());
	//		flag = true;
	//		break;
	//	}
	//	//EF - д������Կ
	//	switch (EF_Control(DDF, ADF, efInfo.m_strWriteControl, keyData1, keyData2, str))
	//	{
	//	case 1://����
	//		efSCW += "0";
	//		writeCtrl = Key_Free + Key_Free;
	//		break;
	//	case 2://��ֹ
	//		efSCW += "0";
	//		writeCtrl = Key_Forbidden + Key_Forbidden;
	//		break;
	//	case 3://�ϼ�Ŀ¼PIN
	//		efSCW = "0";
	//		readCtrl = Key_Security_Level(keyData1) + Key_Free;
	//		break;
	//	case 4://��ǰĿ¼��Կ
	//		efSCW += "0";
	//		writeCtrl = Key_Free + Key_Security_Level(keyData1);
	//		break;
	//	case 5://�ϼ�����ǰĿ¼��Կ���롱�߼�
	//		efSCW += "4";
	//		writeCtrl = Key_Security_Level(keyData1) + Key_Security_Level(keyData2);
	//		break;
	//	case 6://�ϼ�����ǰĿ¼��Կ�����߼�
	//		efSCW += "0";
	//		writeCtrl = Key_Security_Level(keyData1) + Key_Security_Level(keyData2);
	//		break;
	//	default:
	//		Set_Error_Flag();
	//		serr << "�����ļ�ָ�� Create_File��" << MF << ", " << DDF << ", " << ADF << ", " << EF << "����дȨ��״̬" << str;
	//		Info->Append_Error(serr.str());
	//		flag = true;
	//		break;
	//	}
	//	if (flag)
	//		return "";
	//	//EF - ����ָ��
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
	//		serr << "�����ļ�ָ�� Create_File��" << MF << ", " << DDF << ", " << ADF << ", " << EF << "�����Ҳ���ָ��·����ADF�ļ���";
	//		Info->Append_Error(serr.str());
	//		flag = true;
	//	}
	//	//ADF - ���ü��
	//	if (ErrCheckFlag)
	//	{
	//		if (adfInfo.m_strAID.length() % 2 != 0 || !IsHex(adfInfo.m_strAID))
	//		{
	//			Set_Error_Flag();
	//			serr << "�����ļ�ָ�� Create_File��" << MF << ", " << DDF << ", " << ADF << ", " << EF << "����ָ��ADF�ļ���AID��" << adfInfo.m_strAID << "������";
	//			Info->Append_Error(serr.str());
	//			flag = true;
	//		}
	//		if (adfInfo.m_strFID.length() != 4 || !IsHex(adfInfo.m_strFID))
	//		{
	//			Set_Error_Flag();
	//			serr << "�����ļ�ָ�� Create_File��" << MF << ", " << DDF << ", " << ADF << ", " << EF << "����ָ��ADF�ļ���FID��" << adfInfo.m_strFID << "������";
	//			Info->Append_Error(serr.str());
	//			flag = true;
	//		}
	//	}
	//	if (flag)
	//		return "";
	//	//ADF - ����ָ��
	//	sstr << setfill('0') << setw(2) << hex << uppercase << right << 0x10 + adfInfo.m_strAID.length() / 2 << Style_Separator;
	//	sstr << "00";
	//	if (Param->GetKeyInfo(DDF, ADF, "LK" + ADF, keyData1))//Ӧ������Կ�ĺ�����ȫ״̬
	//		sstr << Key_Security_Level(keyData1);
	//	else
	//		sstr << "00";
	//	sstr << Key_Free << MK_Level;//����ɾ��Ȩ��
	//	sstr << "01" << Style_InstData;//Ԥ���˻���Ӧ�ý׶ζ�ʹ��DES�㷨
	//	sstr << ToUpper(adfInfo.m_strFID) << Style_InstData;
	//	sstr << "02";//ADF���ļ����ͣ��̶�Ϊ"02"
	//	sstr << "00" << "00";
	//	sstr << "00";//FCI_SFI
	//	sstr << "00";//��ȫ�����ֽ�
	//	sstr << "00";//��������
	//	sstr << "00";//������
	//	sstr << "00";//������
	//	sstr << "00" << Style_InstData;//������
	//	sstr << ToUpper(adfInfo.m_strAID);
	//}
	//else if (DDF.empty() == false)//DDF
	//{
	//	if (Param->GetDDFInfo(DDF, ddfInfo) == false)
	//	{
	//		Set_Error_Flag();
	//		serr << "�����ļ�ָ�� Create_File��" << MF << ", " << DDF << ", " << ADF << ", " << EF << "�����Ҳ���ָ��·����DDF�ļ���";
	//		Info->Append_Error(serr.str());
	//		flag = true;
	//	}
	//	//DDF - ���ü��
	//	if (ErrCheckFlag)
	//	{
	//		if (ddfInfo.m_strAID.length() % 2 != 0 || !IsHex(ddfInfo.m_strAID))
	//		{
	//			Set_Error_Flag();
	//			serr << "�����ļ�ָ�� Create_File��" << MF << ", " << DDF << ", " << ADF << ", " << EF << "����ָ��DDF�ļ���AID��" << ddfInfo.m_strAID << "������";
	//			Info->Append_Error(serr.str());
	//			flag = true;
	//		}
	//		if (ddfInfo.m_strFID.length() != 4 || !IsHex(ddfInfo.m_strFID))
	//		{
	//			Set_Error_Flag();
	//			serr << "�����ļ�ָ�� Create_File��" << MF << ", " << DDF << ", " << ADF << ", " << EF << "����ָ��DDF�ļ���FID��" << ddfInfo.m_strFID << "������";
	//			Info->Append_Error(serr.str());
	//			flag = true;
	//		}
	//	}
	//	if (flag)
	//		return "";
	//	//DDF - ����ָ��
	//	sstr << setfill('0') << setw(2) << hex << uppercase << right << 0x10 + ddfInfo.m_strAID.length() / 2 << Style_Separator;
	//	sstr << "00";
	//	if (Param->GetKeyInfo(DDF, ADF, "BK", keyData1) == false)//������Կ
	//	{
	//		if (DDF == "SSSE")
	//		{
	//			Set_Error_Flag();
	//			serr << "�����ļ�ָ�� Create_File��" << MF << ", " << DDF << ", " << ADF << ", " << EF << "��������SSSEʱ�Ҳ�����ԿBK��";
	//			Info->Append_Error(serr.str());
	//			flag = true;
	//		}
	//		else
	//			sstr << "00";
	//	}
	//	else
	//		sstr << Key_Security_Level(keyData1);
	//	sstr << Key_Free << MK_Level;//����ɾ��Ȩ��
	//	sstr << "01" << Style_InstData;//Ԥ���˻���Ӧ�ý׶ζ�ʹ��DES�㷨
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
	//	//MF - ���ü��
	//	if (ErrCheckFlag)
	//	{
	//		if (mfInfo.m_strAID.length() % 2 != 0 || !JudgeHex(mfInfo.m_strAID))//����Ϊ��
	//		{
	//			Set_Error_Flag();
	//			serr << "�����ļ�ָ�� Create_File��" << MF << ", " << DDF << ", " << ADF << ", " << EF << "����ָ��MF�ļ���AID��" << mfInfo.m_strAID << "������";
	//			Info->Append_Error(serr.str());
	//			flag = true;
	//		}
	//		if (mfInfo.m_strFID != "3F00")
	//		{
	//			Set_Error_Flag();
	//			serr << "�����ļ�ָ�� Create_File��" << MF << ", " << DDF << ", " << ADF << ", " << EF << "����MF�ļ���FIDҪ������ǡ�3F00��������ǰȴ�ǡ�" << mfInfo.m_strFID << "����";
	//			Info->Append_Error(serr.str());
	//			flag = true;
	//		}
	//	}
	//	if (flag)
	//		return "";
	//	//MF - ����ָ��
	//	sstr << setfill('0') << setw(2) << hex << uppercase << right << 0x10 + mfInfo.m_strAID.length() / 2 << Style_Separator;
	//	sstr << "00";
	//	sstr << "00";//������Կ
	//	sstr << "00" << MK_Level;//����ɾ��Ȩ����
	//	sstr << "01" << Style_InstData;
	//	sstr << ToUpper(mfInfo.m_strFID) << Style_InstData;
	//	sstr << "01";
	//	sstr << "00";
	//	sstr << "01";//�籣DDFִ�п�����ͬʱ����MF
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
	////��β
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
	bool flag = false;//�ú����ڵĴ����־
	bool extflag = false;//��־�Ŵ���EF�ļ�ʱ��дȨ���Ƿ�ʹ������չ������ȫ״̬�ֽ�
	string SFI = "";//���ļ���־��
	string efType = "";//�ļ�����
	string efSpase = "";//�ļ��ռ�
	string efByte4 = "";//��4�ֽڣ�һ���Ƕ�Ȩ��
	string efByte5 = "";//��5�ֽڣ�һ����дȨ��
	string efByte6 = "";//��6�ֽڣ���չ��ȫ״̬�ֽ�
	string efByte7 = "";//��7�ֽڣ���·����
	KEYDATAS keyData;
	string str;//��ʱ����
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
		SFI = SF_SFI;
		efType = "3F";
		efSpase = DecToHex(HexToDec(DataLen) + FileExtSpace);
		efByte4 = "FF";//DF���ļ���ʶ������֪�������
		efByte5 = SSSE_Flag ? SSSE_MK_Level : MK_Level;//����Ȩ��
		efByte6 = "FF";
		efByte7 = "FF";
	}
	else if (FType == "FCI")
	{
		SFI = FCI_SFI;
		efType = "2C";
		efSpase = DecToHex(HexToDec(DataLen) + FileExtSpace);
		efByte4 = SSSE_Flag ? SSSE_Key_Free : Key_Free;//������
		efByte5 = SSSE_Flag ? SSSE_MK_Level : MK_Level;//д���ƣ���ǰĿ¼����
		efByte6 = "FF";
		efByte7 = "FF";
	}
	else if (FType == "ATR")
	{
		Set_Error_Flag();
		serr << "�����ڲ��ļ�ָ�� Create_Internal_EF(" << FType << ", " << DataLen << ")������оƬ��û����ν��ATR�ļ���";
		Info->Append_Error(serr.str());
		flag = true;
	}
	else if (FType == "DIR")
	{
		SFI = FCI_SFI;
		efType = "2C";
		efSpase = DecToHex(HexToDec(DataLen) + FileExtSpace);
		efByte4 = SSSE_Flag ? SSSE_Key_Free : Key_Free;//������
		efByte5 = SSSE_Flag ? SSSE_MK_Level : MK_Level;//д���ƣ���ǰĿ¼����
		efByte6 = "FF";
		efByte7 = "FF";
	}
	else if (FType == "TF")
	{
		if (SSSE_Flag == false)
		{
			Set_Error_Flag();
			serr << "�����ڲ��ļ�ָ�� Create_Internal_EF(" << FType << ", " << DataLen << ")�������籣Ǯ���ļ�ʱ�����쳣���籣Ǯ���ļ�ֻ�����籣Ŀ¼�´�������������� class CScript �еĽ������̡�";
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
			serr << "�����ڲ��ļ�ָ�� Create_Internal_EF(" << FType << ", " << DataLen << ")����SSSE��DF04���Ҳ�����ԿDSK��";
			Info->Append_Error(serr.str());
			flag = true;
		}
		else
		{
			str = Key_Security_Level(keyData);
			if (HexToDec(str) > 0x80)//ʹ����չ��ȫ״̬�ֽ�
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
	//ָ���
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
	//��β
	if (RefreshFlag)
		Info->Append_Script(sstr.str());
	return sstr.str();
}

string COS_FM::FCI_Record(int RecordNo, string MF, string DDF, string ADF, bool RefreshFlag /*= true*/)
{
	return "";
}

/**************************************
���ܣ�����ָ����Կ�İ�ȫ����
���������÷��������Զ���ȡ�������ò�����
IN��DDF��Ӧ��Ŀ¼������ȡֵΪӦ�û�����ƣ���"SSSE"��"PSE"�ȣ����ļ���־������"DDF1"��"DDF2"�ȣ�����Ϊ��""����Ϊ��ʱ����ʾӦ��Ŀ¼������
IN��ADF���ļ�Ŀ¼������ȡֵΪ�ļ���ʶ������"DF01"��"DF02"�ȣ�����Ϊ��""����Ϊ��ʱ����ʾ�ļ�Ŀ¼������
IN��KeyName������㰲ȫ�������Կ���ƣ���"IRK"��
���أ�
�����2���ַ���ɵİ�ȫ�����������������մ�
ע�⣺��ͬ��COS�в�һ���İ�ȫ������뷽ʽ
������
����COS�����籣����籣������Կ��ȫ���𣬴˴�����Ϊ����DDF�����籣��ʽ������ֻ��MF�µ�ADF��EF�Ų��÷��籣��ʽ
���籣��ʽ�У�������ȫ״̬���ֽ����λΪ��1�����ʾ��������չ��ȫ״̬�ֽ��ϣ��硰81����ʾ����չ��ȫ״̬�ֽ��ϻ�01������C0����ʾ����չ��ȫ״̬�ֽ��ϻ�40��
**************************************/
string COS_FM::Key_Security_Level(KEYDATAS KEYInfo)
{
	bool flag = true;//�ú����ڵĴ����־

	serr.clear();
	serr.str("");
	if (SSSE_Flag)//�籣��ʽ
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
	else//���籣��ʽ
	{
		if (KEYInfo.m_nSeq < 0 || KEYInfo.m_nSeq > 0xF)
		{
			serr.clear();
			serr.str("");
			serr << "������Կ��ȫ���� Key_Security_Level��ָ����Կ" << KEYInfo.m_strName << "��˳��ţ�" << KEYInfo.m_nSeq << "����������Χ0-F�ڡ������Ŀ¼λ���µ���Կ�����Ƿ���ࡣ�������ֽ��Զ�ʹ��һ�µİ�ȫ����";
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
		string KeyName;//���޳�����Կ����
		int IgnoreType;//ƥ�䷽ʽ��1-�Ӵ�ƥ�䡰XXX����2-����ƥ�䡰[XXX����3-�Ҷ�ƥ�䡰XXX]����4-ȫƥ�䡰[XXX]��
	}keyIgnore;
	vector<S_KEYIGNORE> vecKeyIgnore;//�Ӳ��� KeyNameIgnore �ֽ��������Ϣ
	vector<S_DDFINFO> vecMFStruct;//��DDF��Ϣ
	vector<KEYDATAS> vecKeyDatas;//ADFĿ¼�µ���Կ
	string::size_type s = 0, e = 0;
	bool matchFlag;//ƥ���־������ֵΪtrueʱ������ǰ��Կ��KeyNameIgnore��Ϣ��
	int cnt;//��Կ��ţ�ÿ��ADF�¸���Ŷ����� 1 ��ʼ������COS�з�д����Կ��UK��ֻ����1-6����ȫ����
	int rcnt;//��Կ��ţ�ÿ��ADF�¸���Ŷ����� 13 ��ʼ���˴���Ӧ�ġ���չ����״̬�ֽڡ���ֻ�С�UK����ͷ����Կ�Ż�Ӵ˴���ʼ��������εݼ�
	

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
			rcnt = 13;
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
						if (ToUpper(vecKeyDatas.at(m).m_strName).find(vecKeyIgnore.at(n).KeyName, 0) == 0)
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
				{
					if (cnt == 7)
					{
						serr.clear();
						serr.str("");
						serr << "<MF><" << vecMFStruct.at(i).m_strDDFName << "><" << vecMFStruct.at(i).m_vecADFInfo.at(j).m_strADFName << ">�µ���Կ��Ŀ���࣬���ⲿ��֤�ҷǡ�UK����ͷ��д����Կ���Ӧ�ò�����6���������أ����������ֽ���ʹ��һ�µİ�ȫ����";
						Info->Append_Warning(serr.str());
					}
					if (cnt > rcnt)
					{
						serr.clear();
						serr.str("");
						serr << "<MF><" << vecMFStruct.at(i).m_strDDFName << "><" << vecMFStruct.at(i).m_vecADFInfo.at(j).m_strADFName << ">�µ���Կ��Ŀ���࣬���ⲿ��֤����Կ��Ӧ�ó���13���������أ����������ֽ���ʹ��һ�µİ�ȫ����";
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
