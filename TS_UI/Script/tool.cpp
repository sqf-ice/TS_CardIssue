//#####################################
//
//	���ߺ���
//
//	��Ӧ�õ���һЩ����Ƭ�Σ���ű���COS�޹�
//	
//	����޸����ڣ�2015-11-25 17:26
//#####################################

#include "tool.h"
#include "ErrCode.h"
#include <sstream>
#include <algorithm>
#include <time.h>
#include <Windows.h>
#include <direct.h>
#include <io.h>

bool IsHex( char HexCh )//���һ���ַ��Ƿ�Ϊʮ�������ַ�
{
	if( (HexCh>='0' && HexCh<='9') || (HexCh>='a' && HexCh<='f') || (HexCh>='A' && HexCh<='F') )
		return true;
	return false;
}

bool IsHex( string HexStr )//����ַ����Ƿ�Ϊʮ�����ƴ�
{
	if (HexStr.empty())
		return false;
	for( size_t i=0; i<HexStr.length(); ++i )
	{
		if( !IsHex(HexStr.at(i)) )
			return false;
	}

	return true;
}


long HexToDec(string strSource)
{
	int t;
	long sum=0;
	for( size_t i=0; i<strSource.length(); ++i )
	{
		if(strSource.at(i)<='9' && strSource.at(i)>='0')
			t=strSource.at(i)-'0';
		else if(strSource.at(i)<='f' && strSource.at(i)>='a')
			t=strSource.at(i)-'a'+10;
		else if(strSource.at(i)<='F' && strSource.at(i)>='A')
			t=strSource.at(i)-'A'+10;
		else
			return 0;
		sum=sum*16+t;
	}
	return sum;
}	

string DecToHex(long lSource)
{
	char szRes[256];
	memset(szRes,'\0',sizeof(szRes));
	string strRes;
	_ltoa_s(lSource,szRes,16);
	strRes = szRes;
	if ( strRes.length()%2 != 0 )
	{
		strRes = "0"+strRes;
	}
	transform( strRes.begin(), strRes.end(), strRes.begin(), toupper );
	return strRes;
}	

bool JudgeHex(string strSource)
{
	if ( strSource.empty() )
		return true;
	for( size_t i=0; i<strSource.length(); ++i )
	{
		if( (strSource.at(i)<='9' && strSource.at(i)>='0') || (strSource.at(i)<='F' && strSource.at(i)>='A') || (strSource.at(i)<='f' && strSource.at(i)>='a') )
			continue;
		else
			return false;
	}
	return true;
}

bool JudgeDec(string strSource)
{
	if ( strSource.empty() )
		return true;
	for( size_t i=0; i<strSource.length(); ++i )
	{
		if( strSource.at(i)<='9' && strSource.at(i)>='0' )
			continue;
		else
			return false;
	}
	return true;
}

string StrDelBlank( string strDatas )
{
	string strDes;
	int s=strDatas.find_first_not_of(" \t");
	int e=strDatas.find_last_not_of(" \t");
	if ( s == string::npos )//δ�ҵ����,���ؿ�
		return "";
	strDes=strDatas.substr(s,e-s+1);
	return strDes;
}

string StrTrim(string str, string delStr/* = " "*/ )
{
	string::size_type pos;
	pos = str.find(delStr);
	while (pos != string::npos)
	{
		str.erase(pos, delStr.length());
		pos = str.find(delStr);
	}
	return str;
}

bool StrDel_ConVar(const string &InStr, string &OutStr)//������������ַ�
{
	string::size_type pos, s, e;
	string str;
	bool flag = false;
	OutStr = StrTrim(InStr);
	pos = 0;
	while (1)
	{
		s = OutStr.find("[", pos);
		if (s == string::npos)
			return flag;
		if ((OutStr.compare(s + 1, 3, "CON") != 0) && (OutStr.compare(s + 1, 3, "VAR") != 0))
		{
			pos = s + 1;
			continue;
		}
		e = OutStr.find("]", s + 4);
		str = OutStr.substr(s + 4, e - (s + 4));//ȡ���м�����֣�ʾ���е����Ų��֣���[VAR��12��]
		if ((str.length() == 0) || (JudgeDec(str) == false))
			pos = s + 4;
		else
		{
			flag = true;
			OutStr.erase(s, e - s + 1);
			pos = s;
		}
	}
}

string VAR_Name(int VarNo)//�����������
{
	stringstream sstr;
	sstr.clear();
	sstr.str("");
	sstr << "[VAR" << VarNo << "]";
	return sstr.str();
}

int VAR_No(string VarName)//����������
{
	int ret;
	string::size_type pos;
	stringstream sstr;
	string str = StrTrim(VarName);
	if (str.compare(0, 4, "[VAR") == 0)
	{
		pos = str.find_last_of("]");
		if (pos == string::npos)
			return ERROR_PARAM_ERR;
		if ((pos + 1) != str.length())
			return ERROR_PARAM_ERR;
		str.erase(pos, 1);
		if (JudgeDec(str) == false)
			return ERROR_PARAM_ERR;
	}
	sstr.clear();
	sstr.str("");
	sstr << str;
	sstr >> ret;
	return ret;
}

string ToUpper(string Str)//ת���ɴ�д�ַ�
{
	for (string::size_type i = 0; i < Str.length(); ++i)
	{
		if (Str.at(i) >= 'a' && Str.at(i) <= 'z')
			Str.at(i) -= 32;
	}
	return Str;
}

string CON_Name(string strCON)//�����������
{
	stringstream sstr;
	sstr.clear();
	sstr.str("");
	sstr << "[" << strCON << "]";
	return sstr.str();
}

string GbkOrAscToHex(string strSource)
{
	char *pszSource = (char *)strSource.c_str();
	char szDes[512];
	string strDes;
	int i;

	memset(szDes, '\0', sizeof(szDes));
	int len = strlen(pszSource);
	for (i = 0; i < len; i++)
	{
		sprintf(szDes + 2*i, "%02X ", (unsigned char)pszSource[i]);
	}
	szDes[2*i] = '\0';  
	strDes = szDes;
	return strDes;
}

bool SaveFile( string strFilePath, string strData )
{
		FILE *fp = NULL;
		//ֻд��ʽ���ļ�
		fp = fopen( strFilePath.c_str(), "w" );
		if( NULL == fp )
		{
			return false;
		}
		//д�ļ�����
		fputs( strData.c_str(), fp );
	
		fclose(fp);
		return true;
}

int WriteLog( string strData, int nMode )
{
	FILE *fp = NULL;
	SYSTEMTIME systime;
	string strFilePath="";
	string strTemp="";
	int nTemp;
	string strMonth,strDay;
	stringstream m_strsTemp;

	//��ȡϵͳʱ��
	memset( &systime, 0, sizeof(SYSTEMTIME) );
	::GetLocalTime( &systime );

	nTemp = systime.wMonth;
	m_strsTemp.str("");
	m_strsTemp<<nTemp;
	strMonth = m_strsTemp.str();
	nTemp = systime.wDay;
	m_strsTemp.str("");
	m_strsTemp<<nTemp;
	strDay = m_strsTemp.str();
	if ( strMonth.length() == 1 )
	{
		strMonth = "0"+strMonth;
	}
	if ( strDay.length() == 1 )
	{
		strDay = "0"+strDay;
	}

	//�ļ���
	m_strsTemp.str("");
	strFilePath.clear();
	m_strsTemp<<"C:\\TS_Logs\\TS_Script_"<<strMonth<<strDay<<".log";
	strFilePath = m_strsTemp.str();

	//��ǰĿ¼�����û���ļ���log�Ļ��������ļ���
	_mkdir("C:\\TS_Logs");
	//׷�ӷ�ʽ���ļ�
	fp = fopen( strFilePath.c_str(), "a" );
	if( NULL == fp )
	{
		return 1;//Ҫ���ش������
	}

	if ( nMode == 1 )//��¼ʱ��
	{
		//д��־ʱ��
		strTemp.clear();
		m_strsTemp.str("");
		m_strsTemp<<"*"<<systime.wYear<<"-"<<strMonth<<"-"<<strDay<<" "<<systime.wHour<<":"<<systime.wMinute<<":"<<systime.wSecond<<":"<<systime.wMilliseconds<<"* ";
		strTemp = m_strsTemp.str();
		fputs( "\x0d\x0a", fp );
		fputs( strTemp.c_str(), fp );
		fputs( "\x0d\x0a", fp );
		//д��־����
		fputs( strData.c_str(), fp );
		fputs( "\x0d\x0a", fp );
	}
	else if ( nMode == 2 )
	{
		//д��־����
		fputs( strData.c_str(), fp );
		fputs( "\x0d\x0a", fp );
	}

	fclose(fp);
	return 0;
}

void RemoveLOG(char *pszFile, int nDays)
{
	string strPath;
	strPath = pszFile;

	strcat(pszFile,"TS_Script*.*");
	struct _finddata_t fileinfo;
	long handle;
	handle=_findfirst(pszFile,&fileinfo);
	if (-1==handle)
		return;
	while ( 1 )
	{
		string strFilePath = strPath + fileinfo.name;
		HANDLE hFile;
		hFile = CreateFile (strFilePath.c_str(),
			GENERIC_READ,  //������GENERIC_READ���Բ��ܵõ�ʱ��
			FILE_SHARE_READ|FILE_SHARE_DELETE,
			NULL,
			OPEN_EXISTING,	// ���ִ��Ŀ¼
			FILE_FLAG_BACKUP_SEMANTICS,
			NULL);
		if( hFile == INVALID_HANDLE_VALUE ){
			if ( !_findnext(handle,&fileinfo) )
				continue;
			else
				break;
		}

		FILETIME lpCreationTime;
		FILETIME lpLastAccessTime;
		FILETIME lpLastWriteTime;
		SYSTEMTIME stUTC3,stLocal3;
		BOOL retval = GetFileTime( hFile, &lpCreationTime, &lpLastAccessTime, &lpLastWriteTime);
		if( retval )
		{
			// ת����ϵͳʱ���ʽ
			FileTimeToSystemTime( &lpLastWriteTime, &stUTC3 );
			SystemTimeToTzSpecificLocalTime(NULL, &stUTC3, &stLocal3);

			time_t timeStart;
			time_t timeEnd;
			time_t timeSpan;
			struct tm stm;
			stm.tm_year=stLocal3.wYear-1900; /*���ֵ��ȥ1900���õ�tm�ṹ�б�����������*/
			stm.tm_mon=stLocal3.wMonth-1;    /*�·�ֵ��ȥ1���õ�tm�ṹ�б�����·�����*/
			stm.tm_mday=stLocal3.wDay;
			stm.tm_hour=stLocal3.wHour;
			stm.tm_min=stLocal3.wMinute;
			stm.tm_sec=stLocal3.wSecond;
			timeStart = mktime(&stm);//�õ�stLocal3��Ӧ������
			timeEnd=time(NULL);//�õ���ǰϵͳUTCʱ������

			timeSpan = timeEnd-timeStart;//����ʱ���
			CloseHandle( hFile );
			if( timeSpan >= nDays * 3600 * 24 )
				DeleteFile( strFilePath.c_str() );
		}
		else
			CloseHandle( hFile );
		if ( !_findnext(handle,&fileinfo) )
			continue;
		else
			break;
	}
	_findclose(handle);
}

string HexToBinary( string strSource )
{
	int i;
	char *szBinary = new char[strSource.length()*4+1];
	string strBinary;

	sscanf(strSource.c_str(),"%x",&i);
	ltoa(i,szBinary,2);
	strBinary = szBinary;
	//����ת����ǰ��δת����δ��ʾ��0
	if( strBinary.length()%4 != 0 )
	{
		string srFillData((4-strBinary.length()%4),'0');
		strBinary = srFillData+strBinary;
	}
	for ( int i = 0;i<strSource.length();++i)
	{
		if (strSource.at(i) == '0')
		{
			strBinary = "0000"+strBinary;
		}
		else
			break;
	}
	delete []szBinary;
	return strBinary;
}

string BinaryToHex( string strSource )
{
	int i;
	char *szHex;
	char *e;
	string strHex;
	if ( strSource.length()%4 != 0 )
		szHex=new char[strSource.length()/4+2];
	else
		szHex=new char[strSource.length()/4+1];

	i=strtol(strSource.c_str(),&e,2);
	ltoa(i,szHex,16);
	strHex = szHex;

	//����ת����ǰ�涪ʧ��δת����0
	if ( strSource.length()%4 != 0 )
	{
		string strTempData(strSource.length()/4+1-strHex.length(),'0');
		strHex = strTempData+strHex;
	}
	else
	{
		string strTempData(strSource.length()/4-strHex.length(),'0');
		strHex = strTempData+strHex;
	}
	delete []szHex;
	return strHex;
}