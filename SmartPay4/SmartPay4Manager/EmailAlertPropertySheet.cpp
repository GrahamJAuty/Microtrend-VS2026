//$$******************************************************************
#include "EmailAlertPropertySheet.h"
//$$******************************************************************

CEmailAlertPropertySheet::CEmailAlertPropertySheet( CWnd* pParentWnd )
	: CSSAutoShutdownPropertySheet(AutoShutdownHelper,"Email Functions", pParentWnd), 
	m_Page1( m_data ),
	m_Page2( m_data ),
	m_Page3( m_data ),
	m_Page4( m_data )
{
	m_data.Read();

	m_psh.dwFlags |= PSH_NOAPPLYNOW;
	m_psh.dwFlags &= ~PSH_HASHELP;

	AddPage(&m_Page1 );									
	m_Page1.m_psp.dwFlags &= ~PSP_HASHELP;

	AddPage(&m_Page2 );									
	m_Page2.m_psp.dwFlags &= ~PSP_HASHELP;

	AddPage(&m_Page3 );									
	m_Page3.m_psp.dwFlags &= ~PSP_HASHELP;

	AddPage(&m_Page4 );									
	m_Page4.m_psp.dwFlags &= ~PSP_HASHELP;
}

//$$******************************************************************
BEGIN_MESSAGE_MAP(CEmailAlertPropertySheet, CSSAutoShutdownPropertySheet)
END_MESSAGE_MAP()
//$$******************************************************************

BOOL CEmailAlertPropertySheet::CSSAutoShutdownPostInitDialog()
{
	SetIcon ( AfxGetApp()->LoadIcon(IDR_MAINFRAME), TRUE );
	return TRUE;
}

//$$******************************************************************

void CEmailAlertPropertySheet::SaveData()
{
	m_data.Save();
}

//$$******************************************************************
