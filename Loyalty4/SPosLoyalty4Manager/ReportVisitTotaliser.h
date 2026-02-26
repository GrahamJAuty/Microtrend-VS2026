#pragma once
//*******************************************************************
#include "resource.h"
//*******************************************************************
#include "..\SPosLoyalty4Shared\LoyaltyAuditFile.h"
//*******************************************************************

class CReportVisitTotaliser
{
public:
	CReportVisitTotaliser ( CWnd* pParent );
	void Show();

private:
	void Reset();
	int Create ( const char* szReportFile );
	void AddColumnText();
	void SaveTotals();
	void AddCount( CCSV* pCsv, int nCount );

private:
	CWnd* m_pParent;

	CReportFile m_Report;
	CString m_strReportLabel;
	CString m_strReportKey;
	CString m_strCaption;

	CLoyaltyAuditFile m_auditFile;			// main audit file
	int m_nMonthTotals[13];
};
