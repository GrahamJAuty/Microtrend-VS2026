#pragma once
/**********************************************************************/
#include "Consolidation.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/

struct CLocationMasterInfo
{
public:
	CLocationMasterInfo();
	void Reset();

public:
	int Compare ( CLocationMasterInfo& source, int nHint = 0 );
	void Add	( CLocationMasterInfo& source );

public:
	void ValidateSettings();

public:
	int GetNwkNo(){ return m_nNwkNo; }
	int GetLocNo(){ return m_nLocNo; }
	int GetTerminalNo(){ return m_nTerminalNo; }
	const char* GetTerminalName(){ return m_strTerminalName; }
	bool GetMasterFlag(){ return m_bMaster; }
	const char* GetMasterName(){ return m_strMasterName; }
	const char* GetDeferredFolder(){ return m_strDeferredFolder; }
	const char* GetImmediateFolder(){ return m_strImmediateFolder; }
	bool GetCashRSPFlag(){ return m_bCashRSP; }
	const char* GetCashRSPFolder(){ return m_strCashRSPFolder; }

public:
	void SetNwkNo( int n ){ m_nNwkNo = n; }
	void SetLocNo( int n ){ m_nLocNo = n; }
	void SetTerminalNo( int n ){ m_nTerminalNo = n; }
	void SetTerminalName( const char* sz ){ m_strTerminalName = sz; }
	void SetMasterFlag( bool b ){ m_bMaster = b; }
	void SetMasterName( const char* sz ){ m_strMasterName = sz; }
	void SetDeferredFolder( const char* sz ){ m_strDeferredFolder = sz; }
	void SetImmediateFolder( const char* sz ){ m_strImmediateFolder = sz; }
	void SetCashRSPFlag( bool b ){ m_bCashRSP = b; }
	void SetCashRSPFolder( const char* sz ){ m_strCashRSPFolder = sz; }
	
public:
	const char* GetFolder( bool bImmediate ){ return bImmediate ? m_strImmediateFolder : m_strDeferredFolder; }
	
private:
	int m_nNwkNo;
	int m_nLocNo;
	int m_nTerminalNo;
	CString m_strTerminalName;
	bool m_bMaster;
	CString m_strMasterName;
	CString m_strDeferredFolder;
	CString m_strImmediateFolder;
	bool m_bCashRSP;
	CString m_strCashRSPFolder;
};

/**********************************************************************/

class CLocationMasterMap
{
public:
	CLocationMasterMap();
	void Reset();
	
public:
	bool GetMasterInfo( int nLocIdx, int nTerminalNo, CLocationMasterInfo& infoMaster );
	void AddMaster( int nLocIdx, CLocationMasterInfo& infoMaster, bool bCheckTNo  );
	void RemoveMaster( CLocationMasterInfo& infoMaster );
	
public:
	void GetLocationMasterList( int nLocIdx, CArray<CLocationMasterInfo,CLocationMasterInfo>& arrayFolders, bool bMasterOnly, bool bCashRSPOnly );
	void SetLocationMasterList( int nLocIdx, CArray<CLocationMasterInfo,CLocationMasterInfo>& arrayFolders );
	void DeleteLocation( int nLocIdx );

public:
	const char* GetMasterName( int nLocIdx, int nTerminalNo );
	const char* GetDownloadFolder( int nLocIdx, int nTerminalNo, bool bImmediate );
	const char* GetTerminalName( int nLocIdx, int nTerminalNo );
	const char* GetCashRSPFolder( int nLocIdx, int nTerminalNo );
	
public:
	void WriteLocationMasterFile( int nLocIdx );
	
private:
	void GetLocationIdxRange( int nLocIdx, int& nStartIdx, int& nEndIdx );

private:
	bool IsLocationLoaded( int nLocIdx );
	void LoadLocation( int nLocIdx );
	
private:
	CReportConsolidationArray<CLocationMasterInfo> m_arrayMasters;
	CReportConsolidationArray<CSortedIntAndInt> m_arrayLocFlags;

private:
	CString m_strMasterName;
	CString m_strDownloadFolder;
	CString m_strTerminalName;
	CString m_strCashRSPFolder;
};

/**********************************************************************/
extern CLocationMasterMap LocationMasterMap;
/**********************************************************************/

