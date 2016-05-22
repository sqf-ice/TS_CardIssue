//#####################################
//
//	��оƬ�����ű��ඨ��
//  
//#####################################
#pragma once
#include "Chip.h"
#include "Script.h"


//#####################################
//
//	class CScript_T514������T514��
//  ����޸����ڣ�20151112 17:30
//#####################################
class CScript_T514:public CScript
{
public:
	CScript_T514( int COS, CInformation *cInfo );
	~CScript_T514();

	//��д�ӿ� ���������ű�����
	bool IssueCardProcess();
	//��д�ӿ� ��������ATR����
	void UpdateATR();

protected:

private:
	
};


//#####################################
//
//	class CScript_FM1232������FM1232��
//  ����޸����ڣ�20151109 16:50
//#####################################
class CScript_FM1232:public CScript
{
public:
	CScript_FM1232( int COS, CInformation *cInfo );
	~CScript_FM1232();

	//��д�ӿ� ���������ű�����
	bool IssueCardProcess();
	//��д�ӿ� ��������ATR����
	void UpdateATR();

protected:
	/************************************************************************/
	/*��    ��:	��д�ӿ� ��MF/DDF/ADF��ֱ��������EF�ļ���д��¼
	/*������������Ҫ��MF/DDF/ADF�µ�����EF�ļ�ʱ,���÷�ʽΪCeateEF( "DDF", "ADF" ),��������Ϊ: ѡ��ADF�ļ�������ȫ�ļ�����Ӽ�¼����DDFΪSSSEʱװ����ADF����Կ����Ҫ����ȫ��PIN���ⲿ��֤ADF�����ء���EF�ļ������ýӿ�д�ļ���¼
	/************************************************************************/
	void CreateEFS( string strDDF = "", string strADF = "" );

private:

};
