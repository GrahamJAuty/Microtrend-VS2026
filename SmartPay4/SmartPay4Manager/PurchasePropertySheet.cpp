/**********************************************************************/
// R104 - Purchase history reporting
//		- R104a - Purchase history reporting Selective plu
// R105 - Single account purchase history
//		- R105b - Single account purchase history selective plu
// R121 - Account statement reporting
// R122 - Single account statement// CRecordXferRecordPropertySheet
/**********************************************************************/
#include "PurchasePropertySheet.h"
/**********************************************************************/

CPurchasePropertySheet::CPurchasePropertySheet( CReportPurchaseHelpers& ReportPurchaseHelpers, CWnd* pParentWnd, UINT iSelectPage)
	:CSSAutoShutdownPropertySheet(AutoShutdownHelper, "", pParentWnd, iSelectPage), 
	m_ReportPurchaseHelpers(ReportPurchaseHelpers),
	m_Page1(ReportPurchaseHelpers)
{
	m_psh.dwFlags |= PSH_NOAPPLYNOW;
	m_psh.dwFlags &= ~PSH_HASHELP;

	AddPage(&m_Page1);												// Report on
	m_Page1.m_psp.dwFlags &= ~PSP_HASHELP;

	// only used by purchase histories - not statement report
	if ( m_ReportPurchaseHelpers.IsPurchaseHistoryReport() || m_ReportPurchaseHelpers.IsPurchaseBreakdownReport() ) 
	{
		AddPage(&m_Page2);				
		m_Page2.LoadReportPurchaseHelpers(ReportPurchaseHelpers);
		m_Page2.m_psp.dwFlags &= ~PSP_HASHELP;
	}
}

/**********************************************************************/

CPurchasePropertySheet::~CPurchasePropertySheet()
{
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPurchasePropertySheet, CSSAutoShutdownPropertySheet)
END_MESSAGE_MAP()

/**********************************************************************/
// PurchasePropertySheet message handlers

BOOL CPurchasePropertySheet::CSSAutoShutdownPostInitDialog()
{
	SetWindowText(m_ReportPurchaseHelpers.GetWindowTitle());
	return TRUE;
}

/**********************************************************************/
