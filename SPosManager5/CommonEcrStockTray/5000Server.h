#pragma once
/**********************************************************************/
#include "5000SiteArray.h"
#include "Consolidation.h"
#include "NetServer.h"
#include "RealTimeCommsThread.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/

class C5000Server : public CNetCommsServer  
{
public:
	C5000Server();
	~C5000Server();

	bool BeginComms ( CStringArray& strSiteArraySales, CStringArray& strSiteArrayProducts, CStringArray& strSiteArrayCustomer, bool bEnableLocal );
	void EnableComms();
	void DisableComms();
	void PauseComms();
	void ResumeComms();
	void TerminateThread();
	void PMSPulse();

	bool QuickCheckNoComms();

	bool GetConfirmStoppedFlag(){ return m_bConfirmStopped; }

#ifndef POSTRAY_UTILITY
	long FetchAllRealTimeSales( int nNetworkIdx, int nType );
#else
	int GetTranCountForPosTray( const char* sz );
#endif

private:
	virtual void Timer();

private:
	static void LogMessageDateTime ( const char* szText, bool bHide=FALSE );
	void LogTerminate ( const char* szText );

private:
	C5000SiteArray m_SiteArraySales;
	C5000SiteArray m_SiteArrayProducts;
	C5000SiteArray m_SiteArrayCustomer;
	CString m_strNetworkPath;
	CString m_strRemoteDestPath;
	bool m_bEnableLocal;

private:
	int m_nCommsEnable;
	CString m_strError;

	bool m_bPaused;
	bool m_bWaitPause;

	int m_nEnableCount;
	int m_nDisableCount;

	bool m_bPMSPulse;

private:
	CWinThread* m_pRealTimeCommsThread;
	CRealTimeCommsThreadInfo m_RealTimeCommsThreadInfo;

public:
	bool m_bConfirmStopped;

#ifdef POSTRAY_UTILITY
	CReportConsolidationArray<CSortedStringCounter> m_arrayTranCountForPosTray;
#endif
};

/**********************************************************************/

