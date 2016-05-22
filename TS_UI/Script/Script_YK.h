//#####################################
//
//	class CScript_YK
//
//	验卡脚本类（通过参数及指令而组成一个个小块的验卡脚本功能）,目前仅支持验证SSSE的脚本生成
//  最后修改日期：20151109 17:30
//#####################################
#pragma once

#include "Information.h"
#include "Parameter.h"
#include "Instruction.h"
#include "Command.h"

#define _CARD_SN 2		//VAR2   卡片SN
#define _DIVFACTOR 92	//VAR92	 分散因子
#define _DIVFACTOR1 93	//VAR93  三级分散因子
#define _DIVFACTOR2 94	//VAR94	 三级分散因子+二级分散因子
#define _DIVFACTOR3 95	//VAR95	 三级分散因子+二级分散因子+一级分散因子

class CScript_YK
{
public:
	CScript_YK(CInformation *cInfo);
	~CScript_YK(void);
	
	/************************************************************************/
	/*名    称:	IssueCardProcess
	/*功    能:	构建发卡脚本流程
	/*输入参数:	nMode：外部认证方式  0：固定密钥  1：加密机  2：PSAM卡
	/*输出参数:	null
	/*返 回 值:	false	成功	true	构建脚本过程发生错误
	/************************************************************************/
	bool IssueCardProcess(int nMode);

protected:
	CInstruction *cInst;
	CInformation *const cInfo;
	CParameter *cParameter;
	CCommand *cCmd;
	bool m_bRefreshFlag;//刷新标志，true:所构建的指令将会自动添加至生成的脚本缓存中
	stringstream m_sstrTemp;//临时存储变量
	string m_strTemp;

	/************************************************************************/
	/*名    称:	LineFeed
	/*功    能:	脚本中添加换行
	/*输入参数:	nLineNum: 表示换行数
	/*输出参数:	null
	/*返 回 值:	null
	/************************************************************************/
	void LineFeed( int nLineNum );
	/************************************************************************/
	/*名    称:	LvOne_Annotations
	/*功    能:	一级注释,用于一般注释,注释方式如 ;------注释内容
	/*输入参数:	strDatas: 注释内容	
	/*			RefreshFlag：刷新标志，当该值为true时，所构建的指令将会自动添加至生成的脚本缓存中，该参数不填写时候默认为true
	/*输出参数:	null
	/*返 回 值:	null
	/************************************************************************/
	string LvOne_Annotations( string strDatas, bool RefreshFlag = true );
	/************************************************************************/
	/*名    称:	LvTwo_Annotations
	/*功    能:	二级注释,用于分割注释,注释方式如 ;==============注释内容==============
	/*输入参数:	strDatas: 注释内容	
	/*			RefreshFlag：刷新标志，当该值为true时，所构建的指令将会自动添加至生成的脚本缓存中，该参数不填写时候默认为true
	/*输出参数:	null
	/*返 回 值:	null
	/************************************************************************/
	string LvTwo_Annotations( string strDatas, bool RefreshFlag = true  );
	/************************************************************************/
	/*名    称:	GetAndSetParama
	/*功    能:	设置脚本参数,包括获取参数类的数据、设置连接加密机、设置认证参数等脚本通用数据
	/*输入参数:	nMode：外部认证方式  0：固定密钥  1：加密机  2：PSAM卡
	/*输出参数:	null
	/*返 回 值:	null
	/************************************************************************/
	void GetAndSetParama(int nMode);
	/************************************************************************/
	/*名    称:	ScriptModeStatement
	/*功    能:	脚本作者和修改日期等声明
	/*输入参数:	null
	/*输出参数:	null
	/*返 回 值:	null
	/************************************************************************/
	void ScriptModeStatement();
	/************************************************************************/
	/*名    称:	ExternalAuthentication
	/*功    能:	生成外部认证的流程，流程包括：读取xml配置的密钥数据，生成认证SSSE下密钥的脚本数据
	/*输入参数:	nMode：外部认证方式  0：固定密钥  1：加密机  2：PSAM卡
	/*输出参数:	null
	/*返 回 值:	null
	/************************************************************************/
	void ExternalAuthentication(int nMode);
	/************************************************************************/
	/*名    称:	PIN_Certify
	/*功    能:	生成认证PIN的流程，流程包括：读取XML配置的PIN，然后会生成校验SSSE下的PIN的脚本数据
	/*输入参数:	null
	/*输出参数:	null
	/*返 回 值:	null
	/************************************************************************/
	void PIN_Certify();
};

