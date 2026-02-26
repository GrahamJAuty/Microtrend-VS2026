//$$******************************************************************
#include "LoyaltyManager.h"
//$$******************************************************************
#include "SystemPropertySheet.h"
//$$******************************************************************

CSystemPropertySheet::CSystemPropertySheet( CWnd* pParentWnd, UINT iSelectPage)
	: CSSPropertySheet("Setup System", pParentWnd, iSelectPage)
{
	m_psh.dwFlags |= PSH_NOAPPLYNOW;
	m_psh.dwFlags &= ~PSH_HASHELP;

	AddPage(&m_Page1);									// General
	m_Page1.m_psp.dwFlags &= ~PSP_HASHELP;
	
	AddPage(&m_Page2);									// Editing
	m_Page2.m_psp.dwFlags &= ~PSP_HASHELP;
	
	AddPage(&m_Page4);									// Reporting
	m_Page4.m_psp.dwFlags &= ~PSP_HASHELP;
	
	AddPage(&m_Page3);									// Export
	m_Page3.m_psp.dwFlags &= ~PSP_HASHELP;
	
	AddPage(&m_Page5);									// Message Boxes
	m_Page5.m_psp.dwFlags &= ~PSP_HASHELP;

	AddPage(&m_Page7);									// Backup Options
	m_Page7.m_psp.dwFlags &= ~PSP_HASHELP;

	AddPage(&m_Page6);									// Email
	m_Page6.m_psp.dwFlags &= ~PSP_HASHELP;
}

//$$******************************************************************
BEGIN_MESSAGE_MAP(CSystemPropertySheet, CPropertySheet)
END_MESSAGE_MAP()
//$$******************************************************************

BOOL CSystemPropertySheet::OnInitDialog()
{
	BOOL bResult = CPropertySheet::OnInitDialog();

	SetIcon ( AfxGetApp()->LoadIcon(IDR_MAINFRAME), TRUE );

	return bResult;
}

//$$******************************************************************

int CSystemPropertySheet::GetChangedPaxtonMode()
{
	return m_Page1.GetChangedPaxtonMode();
}

//$$******************************************************************

bool CSystemPropertySheet::ChangedServerSettings()
{
	return m_Page1.ChangedServerSettings();
}

//$$******************************************************************
