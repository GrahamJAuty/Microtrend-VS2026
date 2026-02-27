//***************************************************************
#include "PurchaseBreakdownPropertySheet.h"
//***************************************************************

CPurchaseBreakdownPropertySheet::CPurchaseBreakdownPropertySheet(CReportPurchaseHelpers& ReportPurchaseHelpers, CWnd* pParentWnd, UINT iSelectPage)
	:CSSAutoShutdownPropertySheet(AutoShutdownHelper,"", pParentWnd, iSelectPage),
	m_ReportPurchaseHelpers(ReportPurchaseHelpers),
	m_Page1(ReportPurchaseHelpers) 
{
	m_psh.dwFlags |= PSH_NOAPPLYNOW;
	m_psh.dwFlags &= ~PSH_HASHELP;

	AddPage(&m_Page1);											// Report on
	m_Page1.m_psp.dwFlags &= ~PSP_HASHELP;
	
	m_Page2.LoadReportPurchaseHelpers(ReportPurchaseHelpers);

	AddPage(&m_Page2);											// Products
	m_Page2.m_psp.dwFlags &= ~PSP_HASHELP;
}

//***************************************************************

CPurchaseBreakdownPropertySheet::~CPurchaseBreakdownPropertySheet()
{
}

//***************************************************************
BEGIN_MESSAGE_MAP(CPurchaseBreakdownPropertySheet, CSSAutoShutdownPropertySheet)
END_MESSAGE_MAP()
//***************************************************************
// PurchasePropertySheet message handlers

BOOL CPurchaseBreakdownPropertySheet::CSSAutoShutdownPostInitDialog()
{
	SetWindowText ( m_ReportPurchaseHelpers.GetWindowTitle() );
	return TRUE;
}

//*********************************************************************

