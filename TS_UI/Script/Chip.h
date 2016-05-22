//#####################################
//
//	各芯片COS的类定义
//
//	最后修改日期：2015-11-09 14:34
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
//	class COS_T514（大唐T514）
//
//	最后修改日期：2015-11-09 13:52
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
	string Key_Security_Level(KEYDATAS KEYInfo);//获取密钥的后续安全状态
	static const string MK_Level;//主控密钥的安全级别
	static const string Key_Forbidden;//禁止权限
	static const string Key_Free;//访问自由
};


//#####################################
//
//	class COS_FM（复旦）
//
//	最后修改日期：2015-11-09 09:08
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
	void Init_Security_Level(string KeyNameIgnore);//复旦COS对后续安全状态有特别要求，需重写
	string Key_Security_Level(KEYDATAS KEYInfo);//获取密钥的后续安全状态
	bool SSSE_Flag;//社保应用标志，复旦的COS含有社保与非社保两套个人化指令，当该值为true时表示当前需执行社保的指令，目前初始值为false，第一次创建DDF后就改写为true
	static const string MK_Level;//非社保应用，主控密钥的安全级别
	static const string Key_Forbidden;//非社保应用，禁止权限
	static const string Key_Free;//非社保应用，访问自由
	static const string SSSE_MK_Level;//社保应用，主控密钥的安全级别
	static const string SSSE_Key_Forbidden;//社保应用，禁止权限
	static const string SSSE_Key_Free;//社保应用，访问自由
	static const int FileExtSpace;//EF文件的扩展空间，创建每个EF文件时的分配空间等于实际空间加上扩展空间
};
#endif//CHIP_H
