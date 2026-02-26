#pragma once
/**********************************************************************/
#include "ReportConsolidationArray.h"
/**********************************************************************/
#define FUNCTIONFILE_SYSTEM 0
#define FUNCTIONFILE_DATABASE 1
#define FUNCTIONFILE_LOCATION 2
/**********************************************************************/

struct CFunctionAuthMiniInfo
{
public:
	int m_nType;
	int m_nFuncNo;
	CString m_strName;
	int m_nAuthLevel;
	bool m_bPassword;
	bool m_bAudit;
	bool m_bLog;
};

/**********************************************************************/

struct CFunctionAuthInfo
{
public:
	CFunctionAuthInfo();
	void Reset();

public:
	int GetFuncNo(){ return m_nFuncNo; }
	const char* GetName(){ return m_strName; }
	//SYSTEM
	int GetAuthLevelSystem(){ return m_nAuthLevelSystem; }
	bool GetPasswordFlagSystem(){ return m_bPasswordSystem; }
	bool GetAuditFlagSystem(){ return m_bAuditSystem; }
	bool GetLogFlagSystem(){ return m_bLogSystem; }
	//DATABASE
	int GetAuthLevelDatabase(){ return m_nAuthLevelDatabase; }
	bool GetPasswordFlagDatabase(){ return m_bPasswordDatabase; }
	bool GetAuditFlagDatabase(){ return m_bAuditDatabase; }
	bool GetLogFlagDatabase(){ return m_bLogDatabase; }
	//LOCATION
	int GetAuthLevelLocation(){ return m_nAuthLevelLocation; }
	bool GetPasswordFlagLocation(){ return m_bPasswordLocation; }
	bool GetAuditFlagLocation(){ return m_bAuditLocation; }
	bool GetLogFlagLocation(){ return m_bLogLocation; }

public:
	void SetFuncNo( int n );
	void SetName( const char* sz );
	//SYSTEM
	void SetAuthLevelSystem( int n );
	void SetPasswordFlagSystem( bool b ){ m_bPasswordSystem = b; }
	void SetAuditFlagSystem( bool b ){ m_bAuditSystem = b; }
	void SetLogFlagSystem( bool b ){ m_bLogSystem = b; }
	//DATABASE
	void SetAuthLevelDatabase( int n );
	void SetPasswordFlagDatabase( bool b ){ m_bPasswordDatabase = b; }
	void SetAuditFlagDatabase( bool b ){ m_bAuditDatabase = b; }
	void SetLogFlagDatabase( bool b ){ m_bLogDatabase = b; }
	//LOCATION
	void SetAuthLevelLocation( int n );
	void SetPasswordFlagLocation( bool b ){ m_bPasswordLocation = b; }
	void SetAuditFlagLocation( bool b ){ m_bAuditLocation = b; }
	void SetLogFlagLocation( bool b ){ m_bLogLocation = b; }

public:
	int Compare ( CFunctionAuthInfo& source, int nHint = 0 );
	void Add	( CFunctionAuthInfo& source );

private:
	int m_nFuncNo;
	CString m_strName;
	//SYSTEM
	int m_nAuthLevelSystem;
	bool m_bPasswordSystem;
	bool m_bAuditSystem;
	bool m_bLogSystem;
	//DATABASE
	int m_nAuthLevelDatabase;
	bool m_bPasswordDatabase;
	bool m_bAuditDatabase;
	bool m_bLogDatabase;
	//LOCATION
	int m_nAuthLevelLocation;
	bool m_bPasswordLocation;
	bool m_bAuditLocation;
	bool m_bLogLocation;
};

/**********************************************************************/

class CFunctionAuthTable
{
public:
	CFunctionAuthTable(void);
	void ResetFunctionList( int nFunctionMode );

public:
	int GetSize(){ return m_arrayFunctions.GetSize(); }
	int GetMacroFunctionCount() { return m_nMacroFunctionCount; }
	bool GetFunction( int nFuncIdx, CFunctionAuthMiniInfo& infoMini );
	bool GetFunction( CFunctionAuthMiniInfo& infoMini );

public:
	void UpdateFunction( CFunctionAuthMiniInfo& infoMini );
	void UpdateFunction( int nFuncIdx, CFunctionAuthMiniInfo& infoMini );

private:
	void AddFunction( int nFuncNo, const char* szName );

public:
	void FixTaxFunction();

private:
	CReportConsolidationArray<CFunctionAuthInfo> m_arrayFunctions;
	int m_nMacroFunctionCount;
};

/**********************************************************************/

class CFunctionAuthArray : public CSharedStringArray
{
public:
	CFunctionAuthArray( int nFunctionMode, CFunctionAuthTable& FunctionTable );

public:
	int Open( const char* szFilename, int nMode );
	void PrepareForWrite();

public:
	int GetFunctionCount(){ return m_FunctionTable.GetSize(); }
	void GetFunction( int nFuncIdx, CFunctionAuthMiniInfo& infoMini );
	void GetFunction( CFunctionAuthMiniInfo& infoMini );
	void UpdateFunction( int nFuncIdx, CFunctionAuthMiniInfo& infoMini );
	void UpdateFunction( CFunctionAuthMiniInfo& infoMini );
	void ResetFunctionList();

private:
	int m_nFunctionMode;
	CFunctionAuthTable& m_FunctionTable;
};

/**********************************************************************/
