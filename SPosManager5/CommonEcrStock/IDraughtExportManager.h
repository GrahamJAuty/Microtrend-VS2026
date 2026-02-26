#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/

struct CIDraughtExportInfo
{
public:
	int m_nNetworkIdx;
	CString m_strLocationList;
	bool m_bGotDate;
	bool m_bBadDate;
	COleDateTime m_dateLastExport;
	COleDateTime m_timeRetryExport;
	long m_lLastNetworkError;
	int m_nLastNetworkErrorCount;
};

/**********************************************************************/
#define IDRAUGHT_EXPORT_STATUS_OK 0
#define IDRAUGHT_EXPORT_STATUS_NOT_REALTIME 1
#define IDRAUGHT_EXPORT_STATUS_NO_DBLINK 2
#define IDRAUGHT_EXPORT_STATUS_NO_LOCATIONS 3
#define IDRAUGHT_EXPORT_STATUS_BAD_DATE 4
#define IDRAUGHT_EXPORT_STATUS_WAIT_DATE 5
/**********************************************************************/

struct CIDraughtExportStatus
{
public:
	int m_nStatus;
	COleDateTime m_dateLastExport;
};

/**********************************************************************/

class CIDraughtExportManager
{
public:
	CIDraughtExportManager();
	
	void BuildNetworkMap();
	void NextNetworkMapIdx();
	int GetNetworkMapIdx(){ return m_nNetworkMapIdx; }

public:
	int GetNetworkIdx();
	const char* GetLocationList();
	bool GetBadDateFlag();
	void GetLastExportDate( COleDateTime& date );	
	void SetLastExportDate( COleDateTime& date );
	void GetExportRetryTime( COleDateTime& time );
	void SetExportRetryTime();
	bool SetNetworkError( long l );

public:
	bool CheckExportTime();
	COleDateTime GetDateExportFrom(){ return m_dateExportFrom; }
	COleDateTime GetDateExportTo(){ return m_dateExportTo; }

public:
	bool CheckConnectTime();
	void SetConnectTime();

public:
	void HandleSetupOptions( CWnd* pParent );
	long HandleIDraughtExport( CWnd* pParent );
	long HandleIDraughtTransfer( CWnd* pParent );
	void LogResult( long lResult );
	void SetExtraErrorInfo( const char* sz ){ m_strExtraErrorInfo = sz; }

public:
	void BuildNetworkStatusArray( CArray<CIDraughtExportStatus,CIDraughtExportStatus>& arrayStatus );

public:
	void SetInitialExportDate( COleDateTime& date );
	void SetDummyFutureDate( COleDateTime& date );
	void ResetExportDatesForTesting();

private:
	int m_nNetworkMapIdx;
	COleDateTime m_dateExportFrom;
	COleDateTime m_dateExportTo;
	COleDateTime m_dateNextConnect;
	CString m_strExtraErrorInfo;

private:
	CArray<CIDraughtExportInfo,CIDraughtExportInfo> m_NetworkMap;
	CString m_strLocationList;
	bool m_bReadyToGo;
};

/**********************************************************************/
extern CIDraughtExportManager IDraughtExportManager;
/**********************************************************************/
#endif
/**********************************************************************/
