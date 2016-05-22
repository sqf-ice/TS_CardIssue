//#####################################
//
//	��оƬCOS���ඨ��
//
//	����޸����ڣ�2015-11-09 14:34
//#####################################

#ifndef CHIP_H
#define CHIP_H

#include "Instruction.h"
#include <string>
#include <sstream>
using namespace std;

#define	COSID_T514	1


//#####################################
//
//	class COS_T514������T514��
//
//	����޸����ڣ�2015-11-09 13:52
//#####################################
class COS_T514:public CInstruction
{
public:
	COS_T514(CInformation *inf, CParameter *para);
	string Delete_File(int WorkFlag, string FType = "", string FID = "", bool RefreshFlag = true);
	string Delete_File(string MF, string DDF, string ADF, string EF, bool RefreshFlag = true);
	string Verify_TranCode(int Len, string Data, bool RefreshFlag = true);
	string End_PreIssue(bool RefreshFlag = true);
	string Create_File(string MF = "", string DDF = "", string ADF = "", string EF = "", bool RefreshFlag = true);
	string Write_Key(string MF, string DDF, string ADF, string KeyName, string KeyVer, bool RefreshFlag = true);
	bool Write_Key(string MF, string DDF, string ADF, string KeyName, string KeyVer, bool Encrypt, bool Mac);
	string Create_Internal_EF(string FType, string DataLen, bool RefreshFlag = true);
	string FCI_Record(int RecordNo, string MF, string DDF, string ADF, bool RefreshFlag = true);
	string Update_ATR(string strATRHead, string strCardSN, bool RefreshFlag = true);
private:
	string Key_Security_Level(KEYDATAS KEYInfo);//��ȡ��Կ�ĺ�����ȫ״̬
	static const string MK_Level;//������Կ�İ�ȫ����
	static const string Key_Forbidden;//��ֹȨ��
	static const string Key_Free;//��������
};


//#####################################
//
//	class COS_FM��������
//
//	����޸����ڣ�2015-11-09 09:08
//#####################################
class COS_FM :public CInstruction
{
public:
	COS_FM(CInformation *inf, CParameter *para);
	string Delete_File(int WorkFlag, string FType = "", string FID = "", bool RefreshFlag = true);
	string Delete_File(string MF, string DDF, string ADF, string EF, bool RefreshFlag = true);
	string Verify_TranCode(int Len, string Data, bool RefreshFlag = true);
	string End_PreIssue(bool RefreshFlag = true);
	string Create_File(string MF = "", string DDF = "", string ADF = "", string EF = "", bool RefreshFlag = true);
	string Write_Key(string MF, string DDF, string ADF, string KeyName, string KeyVer, bool RefreshFlag = true);
	bool Write_Key(string MF, string DDF, string ADF, string KeyName, string KeyVer, bool Encrypt, bool Mac);
	string Create_Internal_EF(string FType, string DataLen, bool RefreshFlag = true);
	string FCI_Record(int RecordNo, string MF, string DDF, string ADF, bool RefreshFlag = true);
	string Update_ATR(string strATRHead, string strCardSN, bool RefreshFlag = true);
private:
	void Init_Security_Level(string KeyNameIgnore);//����COS�Ժ�����ȫ״̬���ر�Ҫ������д
	string Key_Security_Level(KEYDATAS KEYInfo);//��ȡ��Կ�ĺ�����ȫ״̬
	bool SSSE_Flag;//�籣Ӧ�ñ�־��������COS�����籣����籣���׸��˻�ָ�����ֵΪtrueʱ��ʾ��ǰ��ִ���籣��ָ�Ŀǰ��ʼֵΪfalse����һ�δ���DDF��͸�дΪtrue
	static const string MK_Level;//���籣Ӧ�ã�������Կ�İ�ȫ����
	static const string Key_Forbidden;//���籣Ӧ�ã���ֹȨ��
	static const string Key_Free;//���籣Ӧ�ã���������
	static const string SSSE_MK_Level;//�籣Ӧ�ã�������Կ�İ�ȫ����
	static const string SSSE_Key_Forbidden;//�籣Ӧ�ã���ֹȨ��
	static const string SSSE_Key_Free;//�籣Ӧ�ã���������
	static const int FileExtSpace;//EF�ļ�����չ�ռ䣬����ÿ��EF�ļ�ʱ�ķ���ռ����ʵ�ʿռ������չ�ռ�
};
#endif//CHIP_H
