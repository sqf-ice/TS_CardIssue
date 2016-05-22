/************************************************************************/
/*
/*	class CInformation
/*
/*	��Ϣ��¼�ࣨ�����ü�����ã���¼���ɽű����ݡ�������Ϣ��������Ϣ����־�ȣ�
/*  ����޸����ڣ�20151203 17:30
/************************************************************************/
#pragma once
#include <string>
#include <vector>
#include <sstream>
#include <set>

using namespace std;
#define S_SETTING CInformation::s_struSetting
#define V_EKEYS CInformation::s_vecKEYS
#define S_CARDNFO CInformation::s_struCardInfo

//��Ŀ�޸ļ�¼��Ϣ
struct PROJMOD{
	string m_strAuthor;//����
	string m_strModify;//�޸ı�ע��Ϣ
	string m_strDate;//�޸�����
	string m_strVer;//�汾
};
//��Ŀ��Ϣ
struct PROJINFO{
	string m_strName;//��Ŀ����
	string m_strDesc;//����
	vector<PROJMOD> m_vecProjMod;//��Ŀ�޸ļ�¼
};
//����ʶ����
struct PIN{
	string m_strValue;//����ʶ����
	string m_strCount;//�������
};
//���д���
struct CITYCODE{
	string m_strValue;//���д���
	string m_strProvince;//ʡ
	string m_strCity;//��
};
//װ����Կ�����Ϣ
struct LOADKEYINFO{
	string m_strMode;//װ����Կ�ķ�ʽ 1�����ܻ�װ�� 2���̶���Կװ��
	string m_strType;//װ����Կ���� 0������	1������	10�����ĸ���	11�����ĸ���
	string m_strScale;//����	2��ȫʡͨ����ģʽ	1���ؼ��м���ģʽ
	string m_strPKeyNo;//������ԿID
	string m_strPKeyfsjs;//������Կ��������Կ����ɢ����
	string m_strPKeyfsyz;//������Կ��������Կ����ɢ����
	string m_strPKeyLen;//������Կ��������Կ���ĳ���
};
//<SETTING>����
struct SETTING{
	PROJINFO m_struProjInfo;
	PIN m_struPIN;
	CITYCODE m_struCityCode;
	LOADKEYINFO m_struLoadKeyInfo;
};


//��Կ����
struct KEYDATAS{
	string m_strName;//��Կ����
	string m_strKeyIndex;//��Կ��ʶ
	string m_strVer;//��Կ�汾��ʼ�汾��
	string m_strPSAMKeyType;//psam����Կ��;
	string m_strPSAMKeyID;//psam����Կ��ʶ
	string m_strAddr;//���ܻ��д洢��Կ�ĵ�ַ
	string m_strType;//��Կ����
	string m_strLen;//��Կ����(ʮ���Ƶ�λ�ֽ�)
	string m_strGroup;//��Կ����(��Addr����һһ��Ӧ)
	string m_strCount;//�������
	string m_strValue;//��Կֵ
	int m_nSeq;//����װ����Կʱ��������Կ�İ�ȫ����(ע��STK��IRK����Щû�а�ȫ��������00)
};
//<DDF>��ǩ��Կ��Ϣ
struct ADFKEY{
	string m_strADFName;//ADF�ļ���
	vector<KEYDATAS> m_vecKeyDatas;//��Կ����
};
//<SSSEKEYS>��<OTHERKEYS>��ǩ��Կ��Ϣ
struct KEYS{
	string m_strDDFName;//DDF�ļ���
	vector<ADFKEY> m_vecADFKEY;//ADF��Կ��Ϣ
};


//EF��������,ѭ���ļ�����B-TLV����
struct EFEXTRADATAS{
	//ѭ���ļ����ݺ�B-TLV���ݹ�ͬ����
	string m_strP1;//��¼��
	string m_strTag;//��ʶ
	string m_strLen;//����
	string m_strVal;//ֵ
	string m_strrType;//����(an,cn,b)

	//ѭ���ļ����е�����
	string m_strRemark;//��ע
};
//EF����
struct EFDATAS{
	string m_strP1;//��¼��
	string m_strTag;//��ʶ
	string m_strLen;//����
	string m_strVal;//ֵ
	string m_strrType;//����(an,cn,b,B-TLV,-ѭ����¼)
	string m_strRemark;//��ע
	vector<EFEXTRADATAS> m_vecEFExDatas;
};
//EF�ļ���Ϣ
struct EFINFO{
	string m_strSFI;//���ļ���ʶ��,��EF05��EF06��
	string m_strReadControl;//�����ƣ�ȡֵ���ա�PIN��KEY����RKSSSE����PIN|KEY��PIN+KEY����ֹ��FF����
	string m_strWriteControl;//д����
	string m_strEFType;//�����ļ����� 1�������ļ�	2���䳤�ļ�	3��͸���ļ�	4��ѭ���ļ�	5���ڲ��ļ�
	string m_strbInit;//�Ƿ��ʼ����0����	1���ǣ�
	string m_strnCount;//ѭ���ļ���¼��
	string m_strResSpace;//Ԥ���ռ�
	vector<EFDATAS> m_vecEFDatas;
};
//ADF�ļ���Ϣ
struct ADFINFO{
	string m_strADFName;//ADF������DF00��DF01��DF02�ȣ�DF00��ʾ�ò�ṹ�����ڣ�����ADF�µĵ�EFֱ����һ���DDF��
	string m_strMKvalue;//������Կֵ
	string m_strCount;//�������
	string m_strUpdateMode1;//������Կ����ʹ�����أ�0����ǰMK	1����Ŀ¼MK��
	string m_strUpdateMode2;//��ǰMK����ʹ�����أ�0����ǰMK	1����Ŀ¼MK��
	string m_strFID;//�ļ���ʶ��
	string m_strAID;//Ӧ�ñ�ʶ��
	vector<EFINFO> m_vecEFInfo;
};
//DDF�ļ���Ϣ
struct DDFINFO{
	string m_strDDFName;//DDF������DDF0��SSSE��PSE�ȣ�DDF0��ʾ�ò�ṹ�����ڣ�����DDF�µ�ADFֱ����һ���MF��
	string m_strMKvalue;//������Կֵ
	string m_strCount;//�������
	string m_strSpace;//�ռ�
	string m_strUpdateMode1;//������Կ����ʹ�����أ�0����ǰMK	1����Ŀ¼MK��
	string m_strUpdateMode2;//��ǰMK����ʹ�����أ�0����ǰMK	1����Ŀ¼MK��
	string m_strFID;//�ļ���ʶ��
	string m_strAID;//Ӧ�ñ�ʶ��
	vector<ADFINFO> m_vecADFInfo;
};
//<MF>��Ƭ�ṹ����
struct MFINFO{
	string m_strMFName;//MF����
	string m_strMKvalue;//������Կֵ
	string m_strCount;//�������
	string m_strFID;//�ļ���ʶ��
	string m_strAID;//Ӧ�ñ�ʶ��
	vector<DDFINFO> m_vecDDFInfo;
};

class CInformation
{
public:
	CInformation(void);
	~CInformation(void);

	/************************************************************************/
	/*��    ��:	ReadXMLDatas_YK
	/*��    ��:	��ȡ�鿨xml�е����ݲ��洢
	/*�������:	strXmlPath��XML·��  nCheckLog:Ϊ1ʱ��ʾ������־
	/*�������:	strLoadKeyStyle��װ����Կ�ķ�ʽ 1�����ܻ�װ�� 2���̶���Կװ��	
	/*�� �� ֵ:	true	�ǽű������ļ�	false	���ǽű������ļ�(ȱʧ�ű������ļ��б���ı�ǩ)
	/************************************************************************/
	bool ReadXMLDatas_YK( string strXmlPath, string &strLoadKeyStyle, int nCheckLog=1 );
	/************************************************************************/
	/*��    ��:	ReadXMLDatas
	/*��    ��:	��ȡ����xml�е����ݲ��洢
	/*�������:	strXmlPath��XML·��  nCheckLog:Ϊ1ʱ��ʾ������־
	/*�������:	null
	/*�� �� ֵ:	true	�ǽű������ļ�	false	���ǽű������ļ�(ȱʧ�ű������ļ��б���ı�ǩ)
	/************************************************************************/
	bool ReadXMLDatas( string strXmlPath, int nCheckLog=1 );
	/************************************************************************/
	/*��    ��:	CheckXmlDatas
	/*��    ��:	��ⷢ��XML��SETTING��SSSEKEYS��MF��DDF��ADF��EF���ݵĺϷ���
	/*�������:	null
	/*�������:	null
	/*�� �� ֵ:	null
	/************************************************************************/
	void CheckXmlDatas();
	/************************************************************************/
	/*��    ��:	Errors
	/*��    ��:	������ִ�й������������Ĵ�������
	/*�������:	null
	/*�������:	null
	/*�� �� ֵ:	0��֮ǰ��ִ���в�û�з����κδ���	>0���������
	/************************************************************************/
	int Errors();
	/************************************************************************/
	/*��    ��:	Warnings
	/*��    ��:	������ִ�й������������ľ�������
	/*�������:	null
	/*�������:	null
	/*�� �� ֵ:	0��֮ǰ��ִ���в�û�з����κξ���	>0���������
	/************************************************************************/
	int Warnings();
	/************************************************************************/
	/*��    ��:	Append_Error(string)
	/*��    ��:	��Ӵ�����Ϣ,�ú����ڲ�����Ӵ�����Ϣʱ�����Զ�����ʾ�ִ��ʹ����������ϵ���Ϣ����
	/*�������:	strErrMsg��������Ϣ
	/*�������:	null
	/*�� �� ֵ:	null
	/************************************************************************/
	void Append_Error( string strErrMsg );
	/************************************************************************/
	/*��    ��:	Append_Warning
	/*��    ��:	��Ӿ�����Ϣ
	/*�������:	Warning: ����ӵľ�����Ϣ
	/*�������:	null
	/*�� �� ֵ:	null
	/************************************************************************/
	void Append_Warning( string strWarning );
	void Append_Script( string strDatas );//��ӽű���Ϣ
	void Clear_Script();//��սű���Ϣ

	static SETTING s_struSetting;//��ǩSETTING�ڵ�����
	static vector<KEYS> s_vecKEYS;//������Կ����
	static MFINFO s_struCardInfo;//��Ƭ�Ľṹ����
	vector<string> m_vecErrData;	//������Ϣ
	vector<string> m_vecWrnData;	//������Ϣ
	vector<string> m_vecScriptData;	//�ű���������

private:
	/************************************************************************/
	/*��    ��:	CheckXMLFile
	/*��    ��:	���xml�ļ��Ƿ�Ϊ�ű������ļ������ǵĻ���ȷʵ�����ü�¼��������Ϣ��
	/*�������:	strXmlPath��XML·��
	/*�������:	null
	/*�� �� ֵ:	true	�ǽű������ļ�	false	���ǽű������ļ�(ȱʧ�ű������ļ��б���ı�ǩ)
	/************************************************************************/
	bool CheckXMLFile( string strXmlPath );
	/************************************************************************/
	/*��    ��:	ReadSettingDatas
	/*��    ��:	��ȡxml�ű���ǩsetting�е����ݲ��洢
	/*�������:	strXmlPath��XML·��
	/*�������:	null
	/*�� �� ֵ:	null
	/************************************************************************/
	void ReadSettingDatas( string strXmlPath );
	/************************************************************************/
	/*��    ��:	ReadKeysDatas
	/*��    ��:	��ȡxml�ű���ǩKEYS�е�����
	/*�������:	strXmlPath��XML·��
	/*�������:	null
	/*�� �� ֵ:	null
	/************************************************************************/
	void ReadKeysDatas( string strXmlPath );
	/************************************************************************/
	/*��    ��:	ReadCardInfo
	/*��    ��:	��ȡxml�ű���Ŀ�Ƭ�ṹ������Ϣ���洢����ȡ��������ȥ���ַ���ǰ��ո���
	/*�������:	strXmlPath��XML·��
	/*�������:	null
	/*�� �� ֵ:	null
	/************************************************************************/
	void ReadCardInfo( string strXmlPath );
	/************************************************************************/
	/*��    ��:	AddKeysDatas
	/*��    ��:	����ǩKEYS�ж�ȡ�����ݱ��浽���ݽṹ��,����strTagName��strName�������
	/*�������:	strTagName: ��ǩ��		strName����Կ����	strKeyIndex����Կ��ʶ	strVer����Կ�汾��ʼ�汾��	
	/*			strPSAMKeyType��psam����Կ��;		strPSAMKeyID:psam����Կ��ʶ		strAddr�����ܻ��д洢��Կ�ĵ�ַ	
	/*			strType����Կ����	strLen����Կ����	strGroup����Կ����	strCount���������	strValue����Կֵ
	/*�������:	null
	/*�� �� ֵ:	null
	/************************************************************************/
	void AddKeysDatas(string strTagName, string strName, string strKeyIndex, string strVer, string strPSAMKeyType, string strPSAMKeyID, string strAddr, string strType, string strLen, string strGroup, string strCount, string strValue);
	/************************************************************************/
	/*��    ��:	ModeKeysDatasSeq
	/*��    ��:	��PIN������ӵ�KEYS�Ľṹ����
	/*�������:	null
	/*�������:	null
	/*�� �� ֵ: null
	/************************************************************************/
	void ModeKeysDatasSeq();
	/************************************************************************/
	/*��    ��:	CheckMFDatas
	/*��    ��:	���MF������
	/*�������:	strTgNames��������������ǩ
	/*�������:	null
	/*�� �� ֵ:	true	���ݶ��Ϸ�	false	���ڲ��Ϸ�����
	/************************************************************************/
	bool CheckMFDatas( string strTgNames );
	/************************************************************************/
	/*��    ��:	CheckDDFDatas
	/*��    ��:	���DDF������
	/*�������:	strTgNames��������������ǩ	struDDFInfo������DDF������Ϣ
	/*�������:	null
	/*�� �� ֵ:	true	���ݶ��Ϸ�	false	���ڲ��Ϸ�����
	/************************************************************************/
	bool CheckDDFDatas( string strTgNames, const DDFINFO &struDDFInfo );
	/************************************************************************/
	/*��    ��:	CheckADFDatas
	/*��    ��:	���ADF������
	/*�������:	strTgNames��������������ǩ	struADFInfo������ADF������Ϣ
	/*�������:	null
	/*�� �� ֵ:	true	���ݶ��Ϸ�	false	���ڲ��Ϸ�����
	/************************************************************************/
	bool CheckADFDatas( string strTgNames, const ADFINFO &struADFInfo );
	/************************************************************************/
	/*��    ��:	CheckEFDatas
	/*��    ��:	���EF������
	/*�������:	strDDFName: ��ǰ��DDF��	 strTgNames��������������ǩ	struEFInfo������EF������Ϣ	strADFName��ADF����
	/*�������:	null
	/*�� �� ֵ:	true	���ݶ��Ϸ�	false	���ڲ��Ϸ�����
	/************************************************************************/
	bool CheckEFInfor( string strDDFName, string strTgNames, const EFINFO &struEFInfo, string strADFName );
	/************************************************************************/
	/*��    ��:	CheckEFControlKey
	/*��    ��:	���EF�Ŀ�����Կ�Ƿ����
	/*�������:	strDDFName: ��ǰ��DDF��	 strProperty����������	strKey��EF�Ŀ�����Կֵ	strADFName��ADF����
	/*�������:	null
	/*�� �� ֵ:	true	��Կ����	false	��Կ������
	/************************************************************************/
	bool CheckEFControlKey( string strDDFName, string strProperty, string strKey, string strADFName );
	/************************************************************************/
	/*��    ��:	CheckControlKeyExis
	/*��    ��:	�����Կ�Ƿ����
	/*�������:	strDDF: Ӧ��Ŀ¼,����ȡֵΪӦ�û�����ƣ���"SSSE"��"PSE"��,��DDFName��(�����ִ�Сд)��DDF0��ʾĿ¼������
	/*			strADF���ļ�Ŀ¼,����ȡֵΪADF���ƣ���"DF01"��"DF02"��,��ADFName��(�����ִ�Сд)�� DF00��ʾĿ¼������
	/*			strKey: ָ������Կ����(�����ִ�Сд)
	/*�������:	null
	/*�� �� ֵ:	true	��Կ����	false	��Կ������
	/************************************************************************/
	bool CheckControlKeyExis( string strDDF, string strADF, string strKey ); 
	/************************************************************************/
	/*��    ��:	CheckEFDatas
	/*��    ��:	���<EF05>�ȱ�ǩ�����ݵĺϷ���
	/*�������:	strEFType��EF������	struEFDatas������EF���ݽṹ��
	/*�������:	null
	/*�� �� ֵ:	true	���ݶ��Ϸ�	false	���ڲ��Ϸ�����
	/************************************************************************/
	bool CheckEFDatas( string strEFType, const EFDATAS &struEFDatas );
	/************************************************************************/
	/*��    ��:	CheckEFExDatas
	/*��    ��:	���B-TLV��ѭ���ļ����ݵĺϷ���
	/*�������:	strSFI�����ļ���	strTag��B-TLV��Tag��ǩ��	
	/*			strEFType��EF������	struEFExDatas������B-TLV��ѭ���ļ������ݽṹ��
	/*�������:	null
	/*�� �� ֵ:	true	���ݶ��Ϸ�	false	���ڲ��Ϸ�����
	/************************************************************************/
	bool CheckEFExDatas( string strSFI, string strTag, string strEFType, const EFEXTRADATAS &struEFExDatas );
	/************************************************************************/
	/*��    ��:	CheckSETTINGDatas
	/*��    ��:	���<SETTING>��ǩ�����ݵĺϷ���
	/*�������:	null
	/*�������:	null
	/*�� �� ֵ:	true	���ݶ��Ϸ�	false	���ڲ��Ϸ�����
	/************************************************************************/
	bool CheckSETTINGDatas();
	/************************************************************************/
	/*��    ��:	CheckKEYSSDatas
	/*��    ��:	����ȡ��������Կ���ݵĺϷ���
	/*�������:	null
	/*�������:	null
	/*�� �� ֵ:	true	���ݶ��Ϸ�	false	���ڲ��Ϸ�����
	/************************************************************************/
	bool CheckKEYSSDatas();
	/************************************************************************/
	/*��    ��:	CheckNecessaryKeys
	/*��    ��:	�����Կ������ĳЩ��Կ�������,����SSSE�µ�IRK���������Ҫ�������ʶ�����
	/*�������:	null
	/*�������:	null
	/*�� �� ֵ:	true	��Ҫ�������ݶ�����	false	������ĳЩ��Ҫ����Կ������
	/************************************************************************/
	bool CheckNecessaryKeys();
	/************************************************************************/
	/*��    ��:	WriteXmlDatasLog
	/*��    ��:	����ȡ��xml����д����־
	/*�������:	null
	/*�������:	null
	/*�� �� ֵ:	null
	/************************************************************************/
	void WriteXmlDatasLog();
	/************************************************************************/
	/*��    ��:	CheckDF04
	/*��    ��:	����籣Ӧ����AID=D15600000503��Ӧ����ᱣ��2Ŀ¼�ļ��µ�EF�ļ������ļ��漰ҽ�����ѵ���ع��ܣ����ȱ��ĳЩ�ļ���ʱ���������  
	/*�������:	null
	/*�������:	null
	/*�� �� ֵ:	null
	/************************************************************************/
	void CheckDF04();
	/************************************************************************/
	/*��    ��:	CheckCardNo
	/*��    ��:	��ᱣ�Ͽ����кţ���ӦSSSE��EF05����Ŀ����ֶ����ݣ��ɰ�λ����(���д������ĸ)��������һλ����(���д������ĸ)У������ɣ�������ݺϷ���
	/*�������:	null
	/*�������:	null
	/*�� �� ֵ:	null
	/************************************************************************/
	void CheckCardNo();
	/************************************************************************/
	/*��    ��:	CheckPSAMType
	/*��    ��:	У��PSAM����Կ��;�����Ƿ�Ϸ��������籣PSAM��Ӧ��ָ���ṩ�����жϣ�
	/*�������:	strPSAMType: PSAM����Կ��;��ȷ������Ϊ2λ��ʮ�������ַ�����
	/*�������:	null
	/*�� �� ֵ:	0	���ݺϷ�	1	��ɢ�������Ϸ�	2	��Կ���Ͳ��Ϸ�
	/************************************************************************/
	int CheckPSAMType( string strPSAMType );

	stringstream m_strsTemp; 
	string m_strTemp;
	set<string> m_setTag;//���ڱ���B-TLV�����ظ����
	int m_nErrorCnt;		//���������
	int m_nWarningCnt;		//���������
};
