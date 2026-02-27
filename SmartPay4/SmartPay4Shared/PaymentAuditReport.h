#pragma once
//**********************************************************************
#ifdef SYSTEMTYPE_MANAGER
//**********************************************************************
#include "..\SmartPay4Manager\PaymentAuditDlg.h"
//**********************************************************************
#endif
//**********************************************************************
#include "Consolidation.h"
#include "EposPaymentDatabase.h"
#include "EposPaymentGroupDatabase.h"
#include "PaymentAuditFile.h"
#include "RepmanHandler.h"
#include "ReportConsolidationArray.h"
#include "WorkingDlg.h"
//**********************************************************************
#define nMAX_PAYMENTGROUPS	10
//**********************************************************************
#define nTOTAL_CARD		0
#define nTOTAL_REPORT	1
//**********************************************************************
#define nORDERBY_DATE	0
#define nORDERBY_TYPE	1		
#define nORDERBY_USERID	2		
//**********************************************************************

class CPaymentAuditReport
{
public:
	CPaymentAuditReport( const char* szAuditFile, CWnd* pParent );
	
//**********************************************************************
#ifndef SYSTEMTYPE_MANAGER
//**********************************************************************

public:
	void SetSummaryRequiredFlag( bool b ){}

private:
	bool CheckSaveLine(  CCSV& csv ){ return TRUE; }
	

//**********************************************************************
#else
//**********************************************************************

public:
	void SetSummaryRequiredFlag( bool b ){ m_bSummaryReqd = b; }
	void Show();
	
private:
	int CreateReport();
	void AddColumnText();

	CString SkipColumns ( CCSV *pCsv );

	void SaveTotals ( int n, const char* szText );

	void SaveGroupTotals();
	void SaveGroupTotal ( const char* szTitle, double dValue1, double dValue2, double dValue3, double dOther );

	bool CheckSaveLine( CCSV& csv );

	int ReportByDate ( CEposPaymentAuditFile* pAudit, CWorkingDlg* pDlgWorking );
	int ReportByCardNo ( CEposPaymentAuditFile* pAudit, CWorkingDlg* pDlgWorking);
	int ReportByPayment ( CEposPaymentAuditFile* pAudit, CWorkingDlg* pDlgWorking);
	
private:
	CString m_strReportLabel;
	CPaymentAuditDlg m_dlgReport;
	CRepmanHandler m_Repman;
	CReportFileWithEndText m_Report;
	bool m_bSummaryReqd;

//**********************************************************************
#endif
//**********************************************************************

public:
	bool EODPayments ( const char* szDatefrom, const char* szTimeFrom, const char* szDateTo = "", const char* szTimeTo = "" );

private:
	void Reset( int n );
	void ResetPaymentGroups();
	bool SaveLine ( CPaymentAuditRecord* pAtc );
	
public:
	CEposPaymentGroupDatabase m_eposPaymentGroups;
	CReportConsolidationArray<CEPosPaymentGroupSale> m_GroupSale;

private:
	CString m_strAuditFilename;
	CWnd* m_pParent;

	CEposPaymentDatabase m_eposPayments;
	CEPosPaymentGroupSale m_PaymentSale[2];
};

//**********************************************************************
