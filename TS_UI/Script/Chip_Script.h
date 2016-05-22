//#####################################
//
//	各芯片构建脚本类定义
//  
//#####################################
#pragma once
#include "Chip.h"
#include "Script.h"


//#####################################
//
//	class CScript_T514（大唐T514）
//  最后修改日期：20151112 17:30
//#####################################
class CScript_T514:public CScript
{
public:
	CScript_T514( int COS, CInformation *cInfo );
	~CScript_T514();

	//重写接口 构建发卡脚本流程
	bool IssueCardProcess();
	//重写接口 构建更新ATR流程
	void UpdateATR();

protected:

private:
	
};


//#####################################
//
//	class CScript_FM1232（复旦FM1232）
//  最后修改日期：20151109 16:50
//#####################################
class CScript_FM1232:public CScript
{
public:
	CScript_FM1232( int COS, CInformation *cInfo );
	~CScript_FM1232();

	//重写接口 构建发卡脚本流程
	bool IssueCardProcess();
	//重写接口 构建更新ATR流程
	void UpdateATR();

protected:
	/************************************************************************/
	/*功    能:	重写接口 建MF/DDF/ADF下直属的所有EF文件并写记录
	/*流程区别：若需要建MF/DDF/ADF下的所有EF文件时,调用方式为CeateEF( "DDF", "ADF" ),操作流程为: 选择ADF文件、建安全文件并添加记录、当DDF为SSSE时装载完ADF的密钥后需要引用全局PIN、外部认证ADF的主控、建EF文件、调用接口写文件记录
	/************************************************************************/
	void CreateEFS( string strDDF = "", string strADF = "" );

private:

};
