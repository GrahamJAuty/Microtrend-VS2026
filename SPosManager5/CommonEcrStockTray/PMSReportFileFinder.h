/**********************************************************************/
#pragma once
/**********************************************************************/
#ifdef COMPILE_PMS
/**********************************************************************/
#include "Consolidation.h"
#include "EposSelectArray.h"
#include "PMSDefines.h"
#include "ReportConsolidationArray.h"
#include "TermFileListInfo.h"
/**********************************************************************/

class CPMSReportFileFinder
{
public:
	CPMSReportFileFinder();
	void SetIncludeActiveFlag( bool b ){ m_bIncludeActive = b; }
	void SetIncludePendingFlag( bool b ){ m_bIncludePending = b; }
	void SetIncludeCancelledFlag( bool b ){ m_bIncludeCancelled = b; }
	void SetDefaultListInfo( CTermFileListInfo& info );

public:
	void GetPMSFileListComplete( CEposSelectArray& arraySelect, CReportConsolidationArray<CSortedIntItem>& arrayChecked, int nPMSLocIdx, int nPMSSelArrayPos, CReportConsolidationArray< CTermFileListInfo>& arrayFileInfo, int& nTotalFileSize );
	void GetPMSFileListIncomplete( CEposSelectArray& arraySelect, CReportConsolidationArray<CSortedIntItem>& arrayChecked, int nPMSLocIdx, int nPMSSelArrayPos, CReportConsolidationArray<CTermFileListInfo>& arrayFileInfo, int& nTotalFileSize );
	
public:
	void SetPMSDateMode( int n, const char*szFrom, const char* szTo, bool bDeposits );
	bool CheckPMSAccountDateRange( CSSFile& fileSales, int nPMSDayFrom, int nPMSDayTo );
	void ProcessPMSTimeLine( CString& strBuffer, int& nPMSDayCurrent, COleDateTime& oleDatePMS, CString& strPMSDate, CString& strPMSTime );

private:
	int m_nPMSDateMode;
	CString m_strCheckDateFrom;
	CString m_strCheckDateTo;
	bool m_bCheckDeposits;
	bool m_bIncludePending;
	bool m_bIncludeActive;
	bool m_bIncludeCancelled;

private:
	CTermFileListInfo m_infoDefault;
};

/**********************************************************************/
#endif
/**********************************************************************/
