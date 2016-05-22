/************************************************************************/
/*
/*	class CParameter
/*
/*	�����ࣨ���Ի�ȡ������������Ҫ�Ĳ�����
/*	����޸����ڣ�20151113 17:30
/************************************************************************/
#pragma once
#include "Information.h"


//EF�ļ���Ϣ
struct S_EFINFO{
	string m_strSFI;//���ļ���ʶ��,��EF05��EF06��
	string m_strReadControl;//�����ƣ�ȡֵ���ա�PIN��KEY����RKSSSE����PIN|KEY��PIN+KEY����ֹ��FF����
	string m_strWriteControl;//д����
	string m_strEFType;//�����ļ����� 1�������ļ�	2���䳤�ļ�	3��͸���ļ�	4��ѭ���ļ�	5���ڲ��ļ�
	string m_strbInit;//�Ƿ��ʼ����0����	1���ǣ�
	string m_strnCount;//ѭ���ļ���¼��
	string m_strResSpace;//Ԥ���ռ�
	string m_strEFLen;//�ļ���������������ܳ���
	int m_nRecordCount;//��EF�ļ��µ�����������
};
//ADF�ļ���Ϣ
struct S_ADFINFO{
	string m_strADFName;//ADF������DF00��DF01��DF02�ȣ�DF00��ʾ�ò�ṹ�����ڣ�����ADF�µĵ�EFֱ����һ���DDF��
	string m_strMKvalue;//������Կֵ
	string m_strCount;//�������
	string m_strUpdateMode1;//������Կ����ʹ�����أ�0����ǰMK	1����Ŀ¼MK��
	string m_strUpdateMode2;//��ǰMK����ʹ�����أ�0����ǰMK	1����Ŀ¼MK��
	string m_strFID;//�ļ���ʶ��
	string m_strAID;//Ӧ�ñ�ʶ��
	vector<S_EFINFO> m_vecEFInfo;
};

//DDF�ļ���Ϣ
struct S_DDFINFO{
	string m_strDDFName;//DDF������DDF0��SSSE��PSE�ȣ�DDF0��ʾ�ò�ṹ�����ڣ�����DDF�µ�ADFֱ����һ���MF��
	string m_strMKvalue;//������Կֵ
	string m_strCount;//�������
	string m_strSpace;//�ռ�
	string m_strUpdateMode1;//������Կ����ʹ�����أ�0����ǰMK	1����Ŀ¼MK��
	string m_strUpdateMode2;//��ǰMK����ʹ�����أ�0����ǰMK	1����Ŀ¼MK��
	string m_strFID;//�ļ���ʶ��
	string m_strAID;//Ӧ�ñ�ʶ��
	vector<S_ADFINFO> m_vecADFInfo;
};

//MF�ļ���Ϣ
struct S_MFINFO{
	string m_strMFName;//MF����
	string m_strMKvalue;//������Կֵ
	string m_strCount;//�������
	string m_strFID;//�ļ���ʶ��
	string m_strAID;//Ӧ�ñ�ʶ��
};

//���ڴ洢�ű�˵������ĳ���˵������
struct CONDECLAR
{
	string m_strCON;//����
	string m_strUserFields;//�û��ֶ�
	string m_strName;//��������
	string m_strParentDir;//�����ļ�
	string m_strType;//����
	string m_strLen;//����
};

class CParameter
{
public:
	CParameter( CInformation *cInf );
	~CParameter(void);

	/************************************************************************/
	/*��    ��:	GetCardStruct
	/*��    ��:	��ȡMF������Ŀ¼�ṹ��DDF��ADF��EF�ļ��ṹ����Ϣ��
	/*�������:	null	
	/*�������:	vecMFStruct: �洢MF��Ŀ¼�ṹ������
	/*�� �� ֵ:	null
	/*ע    �⣺��ȡ������������,��m_strDDFName
	/************************************************************************/
	void GetCardStruct( vector<S_DDFINFO> &vecMFStruct );
	/************************************************************************/
	/*��    ��:	GetEFInfo
	/*��    ��:	����DDF\ADF\EF��ȡEF�ļ���Ϣ
	/*�������:	strDDF: Ӧ��Ŀ¼,����ȡֵΪӦ�û�����ƣ���"SSSE"��"PSE"�ȣ���FID�ļ���־������"DDF1"��"DDF2"�ȣ�����Ϊ��"" (�����ִ�Сд)
	/*			strADF���ļ�Ŀ¼,����ȡֵΪFID�ļ���ʶ������"DF01"��"DF02"�ȣ�����Ϊ��"" (�����ִ�Сд)	
	/*			strEF: �����ļ�,����ȡֵΪFID�ļ���ʶ������"EF05"��"EF06"�ȣ� (�����ִ�Сд)
	/*�������:	struEFInfo: �洢��ǰDDF\ADF\EF�ļ���Ϣ�Ľṹ��
	/*�� �� ֵ:	true	�ɹ�	false	δ�ҵ�ָ��MF\DDF\ADF\EF����ļ���Ϣ
	/*ע    �⣺��DDF������,������Ҫ��ȡMF\ADF\EF05ʱ,���÷�ʽΪGetEFInfo( "", "ADF", "EF05", struEFInfo )	
	/*			��ADF������,������Ҫ��ȡMF\DDF\EF05ʱ,���÷�ʽΪGetEFInfo( "DDF", "", "EF05", struEFInfo )
	/*			��DDF��ADF��������,������Ҫ��ȡMF\EF05ʱ,���÷�ʽΪGetEFInfo( "", "", "EF05", struEFInfo )
	/************************************************************************/
	bool GetEFInfo( string strDDF, string strADF, string strEF, S_EFINFO &struEFInfo );
	/************************************************************************/
	/*��    ��:	GetADFInfo
	/*��    ��:	����DDF\ADF��ȡADF�ļ���Ϣ
	/*�������:	strDDF: Ӧ��Ŀ¼,����ȡֵΪӦ�û�����ƣ���"SSSE"��"PSE"�ȣ���FID�ļ���־������"DDF1"��"DDF2"�ȣ�����Ϊ��"" (�����ִ�Сд)
	/*			strADF���ļ�Ŀ¼,����ȡֵΪFID�ļ���ʶ������"DF01"��"DF02"�ȣ�(�����ִ�Сд)	
	/*�������:	struADFInfo: �洢��ǰDDF\ADF�ļ���Ϣ�Ľṹ��,����ṹ���еĳ�Աm_vecEFInfoΪ��,����Ҫȡ�ó�Ա����
	/*�� �� ֵ:	true	�ɹ�	false	δ�ҵ�ָ��MF\DDF\ADF����ļ���Ϣ
	/*ע    �⣺��DDF������,������Ҫ��ȡMF\ADFʱ,���÷�ʽΪGetADFInfo( "", "ADF", struADFInfo )
	/************************************************************************/
	bool GetADFInfo( string strDDF, string strADF, S_ADFINFO &struADFInfo );
	/************************************************************************/
	/*��    ��:	GetDDFInfo
	/*��    ��:	����DDF���ƻ�ȡMF\DDF�ļ���Ϣ
	/*�������:	strDDF: Ӧ��Ŀ¼,����ȡֵΪӦ�û�����ƣ���"SSSE"��"PSE"�ȣ���FID�ļ���־������"DDF1"��"DDF2"�ȣ�(�����ִ�Сд)	
	/*�������:	struDDFInfo: �洢��ǰDDF�ļ���Ϣ�Ľṹ��,����ṹ���еĳ�Աm_vecADFInfoΪ��,����Ҫȡ�ó�Ա����
	/*�� �� ֵ:	true	�ɹ�	false	δ�ҵ�ָ��DDF����ļ���Ϣ
	/************************************************************************/
	bool GetDDFInfo( string strDDF, S_DDFINFO &struDDFInfo );
	/************************************************************************/
	/*��    ��:	GetMFInfo
	/*��    ��:	��ȡMF�ļ���Ϣ
	/*�������:	null	
	/*�������:	struMFInfo: �洢MF�ļ���Ϣ�Ľṹ��
	/*�� �� ֵ:	null
	/************************************************************************/
	void GetMFInfo( S_MFINFO &struMFInfo );
	/************************************************************************/
	/*��    ��:	GetKeyInfo
	/*��    ��:	����ָ��DDF\ADF�µ���Կ���ƻ�ȡ��Կ��Ϣ(PIN�洢��SSSE��)
	/*�������:	strDDF: Ӧ��Ŀ¼,����ȡֵΪӦ�û�����ƣ���"SSSE"��"PSE"��,��DDFName������Ϊ��"" (�����ִ�Сд)
	/*			strADF���ļ�Ŀ¼,����ȡֵΪFID�ļ���ʶ������"DF01"��"DF02"�ȣ�����Ϊ��"" (�����ִ�Сд)����Ϊ�������ʱ��ADF��FID�ͱ�ǩ������һ��,���Ը���FIDҲ���ҵ�KEYS�����Ӧ�ı�ǩ����
	/*			strKeyName: ָ������Կ����(�����ִ�Сд)
	/*�������:	struKeyDatas: �洢ָ����Կ��Ϣ�Ľṹ��
	/*�� �� ֵ:	true	��ȡ��Կ��Ϣ�ɹ�	false	ָ����DDF/ADF�²�����ָ������Կ
	/*ע    �⣺��DDF������,������Ҫ��ȡMF\ADF�µ���ԿUKMFʱ,���÷�ʽΪGetKeyInfo( "", "ADF", "UKMF", struKeyDatas )
	/*			��ADF������,������Ҫ��ȡMF\DDF�µ���ԿUKMFʱ,���÷�ʽΪGetKeyInfo( "DDF", "", "UKMF", struKeyDatas )
	/*			��DDF��ADF��������,������Ҫ��ȡMF�µ���ԿUKMFʱ,���÷�ʽΪGetEFInfo( "", "", "UKMF", struEFInfo )
	/************************************************************************/
	bool GetKeyInfo( string strDDF, string strADF, string strKeyName, KEYDATAS &struKeyDatas );
	/************************************************************************/
	/*��    ��:	GetADFKeys
	/*��    ��:	��ȡָ��DDF\ADF�µ�������Կ��Ϣ
	/*�������:	strDDF: Ӧ��Ŀ¼,����ȡֵΪӦ�û�����ƣ���"SSSE"��"PSE"��,��DDFName������Ϊ��"" (�����ִ�Сд)
	/*			strADF���ļ�Ŀ¼,����ȡֵΪFID�ļ���ʶ������"DF01"��"DF02"�ȣ�����Ϊ��"" (�����ִ�Сд)����Ϊ�������ʱ��ADF��FID�ͱ�ǩ������һ��,���Ը���FIDҲ���ҵ�KEYS�����Ӧ�ı�ǩ����	
	/*�������:	vecKeyDatas: �洢ָ��ADF��������Կ��Ϣ������
	/*�� �� ֵ:	true	��ȡ��Կ��Ϣ�ɹ�	false	ָ����DDF\ADF������
	/*ע    �⣺��DDF������,������Ҫ��ȡMF\ADF�µ�������Կ��Ϣʱ,���÷�ʽΪGetADFKeys( "", "ADF", vecKeyDatas )
	/*			��ADF������,������Ҫ��ȡMF\DDF�µ�������Կ��Ϣʱ,���÷�ʽΪGetADFKeys( "DDF", "", vecKeyDatas )
	/*			��DDF��ADF��������,������Ҫ��ȡMF�µ�������Կ��Ϣʱ,���÷�ʽΪGetADFKeys( "", "", vecKeyDatas )
	/************************************************************************/
	bool GetADFKeys( string strDDF, string strADF, vector<KEYDATAS> &vecKeyDatas );
	/************************************************************************/
	/*��    ��:	GetKEYS
	/*��    ��:	��ȡĳЩDDF����Կ
	/*�������:	nMode: ģʽ 1����ȡȫ����Կ����SSSE�ͷ�SSSE��������Կ  2����ȡSSSE����Կ  3����ȡ��SSSE��������Կ
	/*�������:	vecKeys: �洢��ȡ����Կ��Ϣ������
	/*�� �� ֵ:	true	��ȡ��Կ��Ϣ�ɹ�	false	��Ҫ��ȡ����Կ������
	/************************************************************************/
	bool GetKEYS( int nMode, vector<KEYS> &vecKeys );
	/************************************************************************/
	/*��    ��:	ModeKeySeq
	/*��    ��:	�޸�ĳ��ָ����Կ�İ�ȫ����
	/*�������:	strDDF: Ӧ��Ŀ¼,����ȡֵΪӦ�û�����ƣ���"SSSE"��"PSE"��,��DDFName������Ϊ��"" (�����ִ�Сд)
	/*			strADF���ļ�Ŀ¼,����ȡֵΪFID�ļ���ʶ������"DF01"��"DF02"�ȣ�����Ϊ��"" (�����ִ�Сд)����Ϊ�������ʱ��ADF��FID�ͱ�ǩ������һ��,���Ը���FIDҲ���ҵ�KEYS�����Ӧ�ı�ǩ����	
	/*			strKeyName: ָ������Կ����(�����ִ�Сд)
	/*			nSeq����Կ�İ�ȫ״̬(ȡֵ��ΧΪ0-15)
	/*�������:	null
	/*�� �� ֵ:	true	�ɹ�	false	ʧ��
	/************************************************************************/
	bool ModeKeySeq( string strDDF, string strADF, string strKeyName, int nSeq );
	/************************************************************************/
	/*��    ��:	GetEFDatas
	/*��    ��:	��ȡָ��DDF\ADF\EF�����м�¼����(���ص�������vecEFDatas��EFDATAS��P1��������EFDATAS�е�m_vecEFExDatas��P1��������)
	/*�������:	strDDF: Ӧ��Ŀ¼,����ȡֵΪӦ�û�����ƣ���"SSSE"��"PSE"�ȣ���FID�ļ���־������"DDF1"��"DDF2"�ȣ�����Ϊ��"" (�����ִ�Сд)
	/*			strADF���ļ�Ŀ¼,����ȡֵΪFID�ļ���ʶ������"DF01"��"DF02"�ȣ�����Ϊ��"" (�����ִ�Сд)	
	/*			strEF: �����ļ�,����ȡֵΪFID�ļ���ʶ������"EF05"��"EF06"�ȣ� (�����ִ�Сд)	
	/*�������:	vecEFDatas: �洢ָ��DDF\ADF\EF���м�¼���ݵ�����
	/*�� �� ֵ:	true	��ȡDDF\ADF\EF��¼���ݳɹ�	false	��Ҫ��ȡ�����ݲ�����
	/*ע    �⣺��DDF������,������Ҫ��ȡMF\ADF\EF05�µ����м�¼ʱ,���÷�ʽΪGetEFDatas( "", "ADF", "EF05", struADFInfo )
	/*			��ADF������,������Ҫ��ȡMF\DDF\EF05�µ����м�¼ʱ,���÷�ʽΪGetEFDatas( "DDF", "", "EF05", struADFInfo )
	/*			��DDF��ADF��������,������Ҫ��ȡMF\EF05�µ����м�¼ʱ,���÷�ʽΪGetEFDatas( "", "", "EF05", struADFInfo )
	/************************************************************************/
	bool GetEFDatas( string strDDF, string strADF, string strEF, vector<EFDATAS> &vecEFDatas );
	/************************************************************************/
	/*��    ��:	GetEFData
	/*��    ��:	��ȡMF��ָ��DDF\ADF\EF��ĳ��¼��P1��Ӧ�ļ�¼����(��ΪB-TLV����ѭ������ʱ�����ص�������EFDATAS�е�m_vecEFExDatas��P1��������)
	/*�������:	strDDF: Ӧ��Ŀ¼,����ȡֵΪӦ�û�����ƣ���"SSSE"��"PSE"�ȣ���FID�ļ���־������"DDF1"��"DDF2"�ȣ�����Ϊ��"" (�����ִ�Сд)
	/*			strADF���ļ�Ŀ¼,����ȡֵΪFID�ļ���ʶ������"DF01"��"DF02"�ȣ�����Ϊ��"" (�����ִ�Сд)	
	/*			strEF: �����ļ�,����ȡֵΪFID�ļ���ʶ������"EF05"��"EF06"�ȣ� (�����ִ�Сд)	
	/*			nP1����Ҫ��ȡ�ĵڼ�����¼�����(�����ѭ���ļ�����͸���ļ�ʱ,��Ϊֻ��һ������,����nP1��ȡ����ֵ,��Ĭ��ȡ��һ������)
	/*�������:	struEFDatas: �洢ָ��DDF\ADF\EF��ĳ��¼��P1��Ӧ��¼���ݵĽṹ��
	/*�� �� ֵ:	true	��ȡ��¼���ݳɹ�	false	��Ҫ��ȡ�����ݲ�����
	/*ע    �⣺��DDF������,������Ҫ��ȡMF\ADF\EF05��P1=1�ļ�¼ʱ,���÷�ʽΪGetEFData( "", "ADF", "EF05", 1, struADFInfo )
	/*			��ADF������,������Ҫ��ȡMF\DDF\EF05��P1=1�ļ�¼ʱ,���÷�ʽΪGetEFData( "DDF", "", "EF05", 1, struADFInfo )
	/*			��DDF��ADF��������,������Ҫ��ȡMF\EF05��P1=1�ļ�¼ʱ,���÷�ʽΪGetEFData( "", "", "EF05", 1, struADFInfo )	
	/************************************************************************/
	bool GetEFData( string strDDF, string strADF, string strEF, int nP1, EFDATAS &struEFDatas );
	/************************************************************************/
	/*��    ��:	GetProjInfo
	/*��    ��:	��ȡ��Ŀ�����Ϣ�������ߡ��ű��޸����ڵ�
	/*�������:	null	
	/*�������:	struProjInfo: �洢PROJ��Ϣ�Ľṹ��
	/*�� �� ֵ:	null
	/************************************************************************/
	void GetProjInfo( PROJINFO &struProjInfo );
	/************************************************************************/
	/*��    ��:	GetCityCode
	/*��    ��:	��ȡ���д���
	/*�������:	null	
	/*�������:	struCityCode: �洢���д�����Ϣ�Ľṹ��
	/*�� �� ֵ:	null
	/************************************************************************/
	void GetCityCode( CITYCODE &struCityCode );
	/************************************************************************/
	/*��    ��:	GetLoadKeyInfo
	/*��    ��:	��ȡװ����Կ�����Ϣ
	/*�������:	null	
	/*�������:	struLoadKeyInfo: �洢װ����Կ�����Ϣ�Ľṹ��
	/*�� �� ֵ:	null
	/************************************************************************/
	void GetLoadKeyInfo( LOADKEYINFO &struLoadKeyInfo );
	/************************************************************************/
	/*��    ��:	GetCONStatement
	/*��    ��:	��ȡ�ű�˵������ĳ���˵������
	/*�������:	null
	/*�������:	vecCONDeclar: �洢����˵�����ݵ�����
	/*�� �� ֵ:	null
	/************************************************************************/
	void GetCONStatement( vector<CONDECLAR> &vecCONDeclar );
	/************************************************************************/
	/*��    ��:	Internal_FID
	/*��    ��:	ȷ����ȫ�ļ���������Ϣ�ļ�������ļ���FID,�������xml���������ظ���FID,��FID����,ֱ��ȷ����Ψһ��FIDΪֹ
	/*�������:	null	
	/*�������:	strSF_SFI����ȫ�ļ����ļ���ʶ������Կ�ļ�����Ϊ�յ�ʱ�򲻴�������ݣ������Ϊ���Ȳ�����2λ��ʮ���������Ļ��������Ĭ�ϸ�ֵΪ�գ�
	/*			strFCI_SFI��������Ϣ�ļ����ļ���ʶ����Ϊ�յ�ʱ�򲻴�������ݣ������Ϊ���Ȳ�����2λ��ʮ���������Ļ��������Ĭ�ϸ�ֵΪ�գ�
	/*			strDIR_SFI��Ŀ¼�ļ����ļ���ʶ����Ϊ�յ�ʱ�򲻴�������ݣ������Ϊ���Ȳ�����2λ��ʮ���������Ļ��������Ĭ�ϸ�ֵΪ�գ�
	/*			strTF_SFI���ڲ������ļ����ļ���ʶ����Ϊ�յ�ʱ�򲻴�������ݣ������Ϊ���Ȳ�����2λ��ʮ���������Ļ��������Ĭ�ϸ�ֵΪ�գ�
	/*			strATR_SFI����λӦ���ļ����ļ���ʶ����Ϊ�յ�ʱ�򲻴�������ݣ������Ϊ���Ȳ�����2λ��ʮ���������Ļ�������ḳĬ��ֵΪ�գ�
	/*�� �� ֵ:	null
	/************************************************************************/
	void Internal_SFI(string &strSF_SFI, string &strFCI_SFI, string &strDIR_SFI, string &strTF_SFI, string &strATR_SFI);

private:
	/************************************************************************/
	/*��    ��:	CheckEF_Exist
	/*��    ��:	����һ��MF,�������е�FID�Ƿ���XML�д����ظ��������ظ��Ļ�����FIDֵ����
	/*�������:	null
	/*�������:	strFID��EF�ļ���FID
	/*�� �� ֵ:	true	FIDֵû�з����ı�	false	FIDֵ�����˸ı�
	/************************************************************************/
	bool CheckEF_Exist( string &strFID );

	CInformation *const m_cInfo;
	string m_strTemp;
};
