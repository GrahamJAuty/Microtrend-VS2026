#pragma once
/**********************************************************************/
#include "CashRSPVersionChecker.h"
#include "DepartmentSetCSVArray.h"
#include "EODDateTimeFilter.h"
#include "EposReportCustomSettings.h"
#include "EposReportFile.h"
#include "EposReportFileListMaker.h"
#include "EposReportPMSModes.h"
#include "EposReportSelect.h"
#include "EposSelectArray.h"
#include "PluSalesLine.h"
#include "PMSReportFileFinder.h"
#include "ReasonInfo.h"
#include "RepCSV.h"
#include "ReportConsolidationArray.h"
#include "ReportFilters.h"
#include "SessionCSVArray.h"
#include "TermFileListInfo.h"
#include "TimeMap.h"
#include "UnderlineMaker.h"
/**********************************************************************/
#define SERVER_REPORT_CODE 0
#define SERVER_REPORT_NAME 1
#define SERVER_REPORT_SYS 2
#define SERVER_REPORT_DB 3
#define SERVER_REPORT_LOC 4
/**********************************************************************/
#define EPOSREPORT_CONSOL_DEFAULT 0
#define EPOSREPORT_CONSOL_SAVED 1
#define EPOSREPORT_CONSOL_ADHOC 2
#define EPOSREPORT_CONSOL_SYSTEM 3
/**********************************************************************/

struct CEposReportConsolTermFile
{
public:
	CEposReportConsolTermFile();
	void Reset();

public:
	int Compare ( CEposReportConsolTermFile& source, int nHint = 0 );
	void Add	( CEposReportConsolTermFile& source );
	
public:
	int m_nSelArrayIdx;
	int m_nTNo;
	int m_nFileType;
	CString m_strDateFolder;
};

/**********************************************************************/

class CEposReport  
{
public:
	CEposReport( CEposSelectArray& SelectArray );
	virtual ~CEposReport();

	//BASE CLASS FUNCTIONS
	virtual const char* GetReportTitle( bool bForGraph );
	const char* GetParamsFilename();
	void ShowEposDetails();
	int GetReportType(){ return m_nReportType; }
	bool GotDataLine() { return m_ReportFile.GotDataLine(); }
	void SetCreateMD5HashFlag(bool b) { m_ReportFile.SetCreateMD5HashFlag(b); }
	CString GetMD5Hash() { return m_ReportFile.GetMD5Hash(); }

public:
	void SetEposReportSelectInfo( CEposReportSelectInfo& info );
	
	void SetDates ( COleDateTime oleDateFrom, COleDateTime oleDateTo, CSessionCSVRecord& ReportSession );
	void SetConLevel( int nConLevel ){ m_nConLevel = nConLevel; }
	void SetConLevelTitle ( const char* szTitle ) { m_strConLevelTitle = szTitle; }
	void SetSessionTitle( const char* szTitle ) { m_strSessionTitle = szTitle; }
	void SetShowCreationTimeFlag( bool b ){ m_bShowCreationTime = b; }
	
	void SetItemServerFlag( bool bFlag ){ m_bItemServer = bFlag; }
	void SetSaleTimeFlag( bool bFlag ){ m_bSaleTime = bFlag; }
	void SetGraphModeFlag( bool b ){ m_bGraphMode = TRUE; }

	void SetPMSPendingFlag( bool b ){ m_PMSModes.SetWantPendingFlag(b); }
	void SetPMSActiveFlag( bool b ){ m_PMSModes.SetWantActiveFlag(b); }
	void SetPMSCompleteFlag( bool b ){ m_PMSModes.SetWantCompleteFlag(b); }
	void SetPMSCancelledFlag( bool b ){ m_PMSModes.SetWantCancelledFlag(b); }
	void SetPMSEposFlag( bool b ){ m_PMSModes.SetEposFlag(b); }
	void SetPMSDepositsFlag( bool b ){ m_PMSModes.SetWantDepositsFlag(b); }
	void SetPMSAllowMode( int n ){ m_PMSModes.SetAllowMode(n); }
	void SetPMSDateMode( int n ){ m_PMSModes.SetDateMode(n); }

	void SetTermDateType(int n) { m_nTermDateType = n; }
	
	void SetPreferConsolType( int n );
	
	bool SimpleTimeCheck( int nLocIdx, CString& strDate, CString& strTime );
	bool ValidateAccountDate( int nLocIdx, CString& strDate, CString& strTime, COleDateTime& timeTransaction, COleDateTime& dateTransaction );	
	
	bool IsDepartmentPaymentReport();
	bool IsWastageReport();

	//VIRTUAL FUNCTIONS
	virtual void Consolidate() = 0;
	virtual bool CreateReport() = 0;
	virtual bool CreateGraph(){ return FALSE; }
	virtual bool CheckGraph(){ return FALSE; }
	virtual bool SpecialPrepare() { return FALSE; }
	
	//TIME SLICE FUNCTIONS
	void LoadTimeSliceSettings( const char* szSettings, CEposReportCustomSettingsHourly& Settings, int& nTimeSliceType, CTimeMap& TimeMap );
	const char* GetTimeSliceString( int nTimeSliceType, CTimeMap& TimeMap, const char* szTime );
	void GetTimeSliceTimeCode( CString& strSaleTime, CString& strSODLabel, int nTimeSliceType, CTimeMap& TimeMap, CString& strTimeCode, int& nDayShift );
	
	//HELPER FUNCTIONS
	void FormatCSVTime ( const char* szTime, CString& strTime );
	static __int64 GetSeparatedPromoPluNo( __int64 nPromoNo );
	static __int64 GetSeparatedMixMatchPluNo( __int64 nMixMatchNo );
	
	static const char* GetReportNameExternal( int nReportType );
	virtual const char* GetReportNameInternal( int nReportType );

	bool CheckPluSaleDateTime( bool bIsPMSLocation, CTermFileListInfo& infoFile, CPluSalesLine& csv, COleDateTime& oleDatePMS, CString strThisSaleDate, CString& strThisSaleTime );
	bool CheckReasonInfoDateTime( CTermFileListInfo& infoFile, CReasonInfo& ReasonInfo, CString strThisSaleDate, CString& strThisSaleTime );

	bool GetCashRSPVersionCheckerFailFlag(){ return m_CashRSPVersionChecker.GetCashRSPFormatFailFlag(); }
	void ShowCashRSPVersionCheckerFailMessage(){ m_CashRSPVersionChecker.ShowCashRSPFormatFailMessage(FALSE); }

	void AddReportColumn( const char* szName, UINT nAlign, int nWidth, bool bTotal );

protected:
	const char* GetServerHeader( int nServerLoc, int nServerNo, int nForceDbIdx = -1, bool bFullLine = TRUE );
	const char* GetServerColumnName( int nServerLoc, int nServerNo );
	const char* GetTotalTypeName( bool bFullLine, int nEntityType, bool bSystemToGrand = FALSE, bool bTotal = FALSE  );

protected:
	int GetDatabaseTaxBand( CPluSalesLine& csv, int nLineType );

protected:
	void UpdateCreateReportProgress();

public:
	CReportFilters m_ReportFilters;
	
protected:
	CEposReportFile m_ReportFile;
	CEposSelectArray& m_SelectArray;
	bool m_bShowCreationTime;
	int m_nReportType;
	CString m_strTitle;
	CString m_strSessionTitle;
	int m_nConLevel;
	CString m_strConLevelTitle;
	CString m_strParamsFilename;
	CString m_strReportName;
	CString m_strTimeSliceString;

	static CString m_strReportNameTemp;
	
	CEposReportPMSModes m_PMSModes;

	bool m_bEODMode;
	bool m_bItemServer;
	bool m_bSaleTime;
	bool m_bGraphMode;
	
	int m_nPreferConsolType;

	int m_nTermDateType;
	
protected:
	CCashRSPVersionChecker m_CashRSPVersionChecker;
	
protected:
	int m_nServerReportType;
	CString m_strServerHeader;
	CString m_strTotalTypeName;
	int m_nBlankColumnsBeforeDate;
	
protected:
	__int64 m_nCreateReportTarget;
	__int64 m_nCreateReportCount;
	__int64 m_nCreateReportMiniTarget;
	__int64 m_nCreateReportMiniCount;

protected:
	CEposReportSelectInfo m_EposReportSelectInfo;

protected:
	CUnderlineMaker m_UnderlineMaker;
};

/**********************************************************************/
