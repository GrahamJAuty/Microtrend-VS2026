//$$******************************************************************
#include "SystemPropertySheet.h"
//$$******************************************************************

CSystemPropertySheet::CSystemPropertySheet( CWnd* pParentWnd, UINT iSelectPage)
	: CSSAutoShutdownPropertySheet( AutoShutdownHelper, "Setup System", pParentWnd, iSelectPage)
{
	m_bHasDisplayTab = FALSE;

	m_psh.dwFlags |= PSH_NOAPPLYNOW;
	m_psh.dwFlags &= ~PSH_HASHELP;

	AddPage(&m_Page1);									// General
	m_Page1.m_psp.dwFlags &= ~PSP_HASHELP;
	
	AddPage(&m_Page3);									// Editing
	m_Page3.m_psp.dwFlags &= ~PSP_HASHELP;
	
	AddPage(&m_Page2);									// Reporting
	m_Page2.m_psp.dwFlags &= ~PSP_HASHELP;
	
	AddPage(&m_Page8);									// Epos
	m_Page8.m_psp.dwFlags &= ~PSP_HASHELP;

	AddPage(&m_Page7);									// Interfaces + Web Payments
	m_Page7.m_psp.dwFlags &= ~PSP_HASHELP;

	AddPage(&m_Page5);									// Export
	m_Page5.m_psp.dwFlags &= ~PSP_HASHELP;

	m_bHasDisplayTab = TRUE;
	AddPage(&m_Page9);									// Message Boxes
	m_Page9.m_psp.dwFlags &= ~PSP_HASHELP;

	AddPage(&m_Page10);									// BACKUP OPTIONS
	m_Page10.m_psp.dwFlags &= ~PSP_HASHELP;

	AddPage(&m_Page6);									// EMAIL
	m_Page6.m_psp.dwFlags &= ~PSP_HASHELP;
	
}

//$$******************************************************************

bool CSystemPropertySheet::RestartServerReqd()
{
	if ( m_Page1.RestartServerReqd() == TRUE )
		return TRUE;

	if ( m_Page2.RestartServerReqd() == TRUE )
		return TRUE;

	if ( m_Page7.RestartServerReqd() == TRUE )
		return TRUE;

	return FALSE;
}

//$$******************************************************************

BOOL CSystemPropertySheet::CSSAutoShutdownPostInitDialog()
{
	SetIcon ( AfxGetApp()->LoadIcon(IDR_MAINFRAME), TRUE );
	return TRUE;
}

//$$******************************************************************
