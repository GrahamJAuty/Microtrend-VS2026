#pragma once
/**********************************************************************/
#ifdef COMPILE_PMS 
/**********************************************************************/
#include "SessionCSVArray.h"
/**********************************************************************/

struct CEposReportConsolPMSPayments
{
public:
	CEposReportConsolPMSPayments();

public:
	void Reset();
	int Compare ( CEposReportConsolPMSPayments& source, int nHint = 0 );
	void Add	( CEposReportConsolPMSPayments& source );
	
public:
	int m_nPaymentNo;

public:
	double m_dAccount;
	double m_dDeposit;
};

/**********************************************************************/

class CPMSReportPayments
{
public:
	CPMSReportPayments();
	void CreateReport();
	void SetReportDates( COleDateTime& dateFrom, COleDateTime& dateTo, CSessionCSVRecord& ReportSession );
	void SetSessionTitle( const char* sz ){ m_strSessionTitle = sz; }

public:
	void SetIncludePendingFlag( bool b ){ m_bIncludePending = b; }
	void SetIncludeActiveFlag( bool b ){ m_bIncludeActive = b; }
	void SetIncludeCompleteFlag( bool b ){ m_bIncludeComplete = b; }
	void SetIncludeCancelledFlag( bool b ){ m_bIncludeCancelled = b; }
	void SetIncludeDepositFlag( bool b ){ m_bIncludeDeposit = b; }
	void SetIncludePaymentFlag( bool b ){ m_bIncludePayment = b; }
	void SetIncludeTrainingFlag( bool b ){ m_bIncludeTraining = b; }

private:
	bool CheckTime( int nPMSDay, COleDateTime& oleTime );
	bool CreateReportInternal();
	
private:
	CReportFile m_ReportFile;
	CString m_strParamsFilename;
	bool m_bIncludePending;
	bool m_bIncludeActive;
	bool m_bIncludeComplete;
	bool m_bIncludeCancelled;
	bool m_bIncludeDeposit;
	bool m_bIncludePayment;
	bool m_bIncludeTraining;

private:
	CString m_strSessionTitle;
};

/**********************************************************************/
#endif
/**********************************************************************/
