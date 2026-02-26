#pragma once
/**********************************************************************/
#include "BaseReportHelpers.h"
#include "CCSVReportLine.h"
#include "RepCSV.h"
#include "ReportConsolidationArray.h"
#include "ServerSelectionList.h"
#include "EposSelectArray.h"
#include "SessionCSVArray.h"
/**********************************************************************/
#define TAREPORT_DETAIL 0
#define TAREPORT_DETAIL_ESTPAY 1
#define TAREPORT_ESTPAY 2
#define TAREPORT_EXPORT 3
#define TAREPORT_EXPORT_BREAKS 4
#define TAREPORT_SESSION 5
/**********************************************************************/
#define WORKING_REPORT_SYS 0
#define WORKING_REPORT_DB 1
#define WORKING_REPORT_LOC 2
/**********************************************************************/
#define WORKING_STATUS_SIGNOUT 0
#define WORKING_STATUS_SIGNIN 1
#define WORKING_STATUS_BREAKIN 2
/**********************************************************************/
#define WORKING_ACTION_SIGNIN 1
#define WORKING_ACTION_SIGNOUT 2
#define WORKING_ACTION_BREAKIN 3
#define WORKING_ACTION_BREAKOUT 4
/**********************************************************************/

struct CEposReportConsolTimeAndAttendance
{
public:
	CEposReportConsolTimeAndAttendance();
	void Reset();

public:
	void Clear();

public:
	int Compare(CEposReportConsolTimeAndAttendance& source, int nHint = 0);
	void Add(CEposReportConsolTimeAndAttendance& source);

public:
	bool CompareServer(CEposReportConsolTimeAndAttendance& source);

public:
	int m_nServerNo;
	int m_nDbNo;
	int m_nNwkNo;
	int m_nLocNo;
	CString m_strDate;
	CString m_strTime;
	__int64 m_nEventID;

public:
	int m_nHighestEdit;
	bool m_bSingleEvent;
	CString m_strData;
};

/**********************************************************************/

struct CEposReportConsolTAWorkHours
{
public:
	CEposReportConsolTAWorkHours();
	void Reset();

public:
	int Compare(CEposReportConsolTAWorkHours& source, int nHint = 0);
	void Add(CEposReportConsolTAWorkHours& source);

public:
	CString m_strDate;
	
public:
	int m_nMinutesWorked;
	double m_dEstPay;
	double m_dCapPay;
};

/**********************************************************************/

struct CTimeAndAttendanceExportDetail
{
public:
	void LoadFromCSV(CCSV& csv);

public:
	bool m_bBreak = FALSE;
	CString m_strStartDate = "";
	CString m_strStartTime = "";
	CString m_strEndDate = "";
	CString m_strEndTime = "";
	int m_nWeekday = 0;
	int m_nEventMinutes = 0;
	CString m_strTimeSpan = "";
	bool m_bOpenSession = FALSE;
	bool m_bBadShift = FALSE;
	int m_nBreakMinutes = 0;
};

/**********************************************************************/

class CReportTimeAndAttendance
{
public:
	CReportTimeAndAttendance( CEposSelectArray& SelectArray, CServerSelectionList& list, bool bGroupByServer );
	
	void SetServerPickType( int n );
	void SetServerRange( int nStart, int nEnd );
	void SetTAReportType( int n ){ m_nTAReportType = n; }
	void SetTAReportTitleInfo(const char* sz) { m_strTAReportTitleInfo = sz; }
	void SetServerConsolidationType(int n) { m_nServerConsolidationType = n; }
	void SetDates ( COleDateTime oleDateFrom, COleDateTime oleDateTo, CSessionCSVRecord& ReportSession );
	void SetShowEditDetailType( int n ){ m_nShowEditType = n; }
	void SetClosingTimeFlag( bool b ){ m_bCheckClosingTime = b; }
	void SetEventsBySecondFlag( bool b ){ m_bEventsBySecond = b; }
	void SetOpenShiftFlag(bool b) { m_bOpenShift = b; }

public:
	void ClearConsolidateArray();	
	void ConsolidateServerData ( CCSVEposTimeAttendData& csv, int nNwkNo, int nDbNo, int nLocNo, CEposSelectArray& SelectArray, int nSelArrayIdx );
	void SortByEventDateAndTime();
	
private:
	bool ConsolidateServerDataInternal ( CCSVEposTimeAttendData& csv, int nServerNo, int nNwkNo, int nDbNo, int nLocNo );
	bool CheckTimeStage1( CCSVEposTimeAttendData& csv, CString& strDateUTC, CString& strTimeUTC, CString& strDateReport, CString& strTimeReport );
	bool CheckTimeStage2( CString& strDateReport, CString& strTimeReport );

public:
	bool CreateReport();
	CString GetTAReportTitleFull() { return m_strTAReportTitleName + m_strTAReportTitleInfo;  }

public:
	static void GetFileList(CEposSelectArray& EposSelectArray, CStringArray& FileList, CWordArray& DbIdxList, CWordArray& LocIdxList, CWordArray& SelIdxList, __int64& nTotalFileSize);

private:
	bool CreateFullReport();
	bool CreateFullReportInternal();
	bool CreateSessionSummaryReport();
	bool CreateSessionSummaryReportInternal();
	void WriteHeader( CEposReportConsolTimeAndAttendance& ConsolInfo );
	void WriteReportLines( CEposReportConsolTimeAndAttendance& ConsolInfo, bool bTidyUp );
	void AssignWorkHoursToDate(COleDateTime& timeSignOutUTC);

private:
	bool CreateSimpleExport( bool bBreaks );
	bool CreateSimpleExportInternal( bool bBreaks );
	void ProcessExportLine( CEposReportConsolTimeAndAttendance& ConsolInfo, bool bBreaks, bool bTidyUp );
		
private:
	void GetServerInfo( CServerSelectionInfo& info, int nDbNo, int nNwkNo, int nLocNo, int nServerNo);
	void ResetTotals();
	void WriteTimeSpans(bool bSessionSummary, bool bGotOpenSession);
	void WriteTimeSpans( COleDateTimeSpan spanWork, COleDateTimeSpan spanBreak );
	void AppendSubtotalLabel(CCSVReportLine& csv, CString strLabel, bool bSessionSummary);
	void TidyTimeSpans();
	void WriteWorkHours();
	void WriteWorkHoursTotal();

	void UTCToOleDate(const char* szUTC, COleDateTime& oleUTC, COleDateTime& oleReport);
	void UTCToOleDate(COleDateTime& oleUTC, COleDateTime& oleReport);
	void LocalTimeToUTC(COleDateTime& oleLocal, COleDateTime& oleUTC);
	int GetTotalMinutesFromOleDateTimeSpan(COleDateTimeSpan& span);
	void SetAutoSignOutTime(COleDateTime& oleSignInTimeUTC);

	const char* GetTimeSpanString( const COleDateTimeSpan span );

private:
	CReportFile m_ReportFile;
	bool m_bHasData = FALSE;

private:
	CStringArray m_arrayListedNames;

private:
	CEposSelectArray& m_SelectArray;
	CServerSelectionList& m_ServerSelectionList;

	int m_nServerPickType = 0;
	int m_nServerStart = 0;
	int m_nServerEnd = 0;

	CString m_strNewDateReport = "";
	CString m_strLastDateReport = "";
	int m_nLastSelArrayIdx = 0;

	int m_nServerStatus = 0;
	CServerSelectionInfo m_currentServerInfo;
	
	COleDateTime m_oleTimeAutoSignOutUTC;
	COleDateTime m_oleTimeSignInUTC;
	COleDateTime m_oleTimeBreakInUTC;
	COleDateTime m_oleTimeLastLineUTC;
	COleDateTime m_oleDateTimeTidyUp;
	COleDateTime m_oleDateTimeNow;
	COleDateTimeSpan m_oleSpanWorkUTC;
	COleDateTimeSpan m_oleSpanBreakUTC;
	COleDateTimeSpan m_oleSpanCurrentShiftBreakUTC;
	COleDateTimeSpan m_oleSpanBadShiftBreakUTC;
	COleDateTimeSpan m_oleSpanBadShiftWorkUTC;

	CString m_strExportSignInDate = "";
	CString m_strExportSignInTime = "";
	CString m_strExportBreakInDate = "";
	CString m_strExportBreakInTime = "";
	CString m_strExportSignInCSVDate = "";
	CString m_strExportBreakInCSVDate = "";
	int m_nExportSignInWeekday = 0;
	
	int m_nTAReportType = 0;
	CString m_strTAReportTitleName = "";
	CString m_strTAReportTitleInfo = "";
	int m_nServerConsolidationType = 0;
	int m_nShowEditType = 0;
	bool m_bCheckClosingTime = FALSE;
	bool m_bEventsBySecond = FALSE;
	bool m_bOpenShift = FALSE;
	
	bool m_bGotBadShift = FALSE;

	CString m_strTimeSpan = "";

	CReportConsolidationArray<CEposReportConsolTimeAndAttendance> m_ConsolArrayStage1;
	CReportConsolidationArray<CEposReportConsolTimeAndAttendance> m_ConsolArrayStage2;
	CReportConsolidationArray<CEposReportConsolTAWorkHours> m_ConsolArrayWorkHours;
	CReportConsolidationArray<CEposReportConsolTAWorkHours> m_ConsolArrayWorkTotal;
};

/**********************************************************************/
