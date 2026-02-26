#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/

struct CSageExportInfo
{
public:
	int m_nDbIdx;
	CString m_strNetworkList;
	bool m_bGotDate;
	bool m_bBadDate;
	COleDateTime m_dateLastExport;
	COleDateTime m_timeRetryExport;
	long m_lLastDatabaseError;
	int m_nLastDatabaseErrorCount;
};

/**********************************************************************/
#define SAGE_EXPORT_STATUS_OK 0
#define SAGE_EXPORT_STATUS_NOT_REALTIME 1
#define SAGE_EXPORT_STATUS_NO_LOCATIONS 2
#define SAGE_EXPORT_STATUS_BAD_DATE 3
#define SAGE_EXPORT_STATUS_WAIT_DATE 4
/**********************************************************************/

struct CSageExportStatus
{
public:
	int m_nStatus;
	COleDateTime m_dateLastExport;
};

/**********************************************************************/

class CSageExportManager
{
public:
	CSageExportManager();
	
	void BuildDatabaseMap();
	void NextDatabaseMapIdx();
	int GetDatabaseMapIdx(){ return m_nDatabaseMapIdx; }

public:
	int GetDatabaseIdx();
	const char* GetNetworkList();
	bool GetBadDateFlag();
	void GetLastExportDate( COleDateTime& date );	
	void SetLastExportDate( COleDateTime& date );
	void GetExportRetryTime( COleDateTime& time );
	void SetExportRetryTime();
	void ResetExportRetryTime();
	bool SetDatabaseError( long l );

public:
	bool CheckExportTime();
	COleDateTime GetDateExportFrom(){ return m_dateExportFrom; }
	COleDateTime GetDateExportTo(){ return m_dateExportTo; }

public:
	void HandleSetupOptions( CWnd* pParent );
	long HandleSageExport( CWnd* pParent );
	void LogResult( long lResult );
	void SetExtraErrorInfo( const char* sz ){ m_strExtraErrorInfo = sz; }

public:
	void BuildDatabaseStatusArray( CArray<CSageExportStatus,CSageExportStatus>& arrayStatus );

public:
	void SetInitialExportDate( COleDateTime& date );
	void SetDummyFutureDate( COleDateTime& date );
	void ResetExportDatesForTesting();

private:
	int m_nDatabaseMapIdx;
	COleDateTime m_dateExportFrom;
	COleDateTime m_dateExportTo;
	COleDateTime m_dateLastExportAttempt;
	CString m_strExtraErrorInfo;

private:
	CArray<CSageExportInfo,CSageExportInfo> m_DatabaseMap;
	CArray<CSageExportStatus,CSageExportStatus> m_arrayStatus;
	CString m_strNetworkList;
	bool m_bReadyToGo;
};

/**********************************************************************/
extern CSageExportManager SageExportManager;
/**********************************************************************/
#endif
/**********************************************************************/
