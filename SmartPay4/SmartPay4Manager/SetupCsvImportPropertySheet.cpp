//**********************************************************************
#include "SetupCsvImportPropertySheet.h"
//**********************************************************************

CSetupCsvImportPropertySheet::CSetupCsvImportPropertySheet( CWnd* pParentWnd, UINT iSelectPage)
	:CSSAutoShutdownPropertySheet(AutoShutdownHelper, "Setup Data Importing", pParentWnd, iSelectPage)
{
	m_psh.dwFlags |= PSH_NOAPPLYNOW;
	m_psh.dwFlags &= ~PSH_HASHELP;

	m_data.Read();

	AddPage(&m_Page1);										// CSV records
	m_Page1.m_psp.dwFlags &= ~PSP_HASHELP;
	m_Page1.SetPointer ( &m_data );

	AddPage(&m_Page2);										// CSV topups
	m_Page2.m_psp.dwFlags &= ~PSP_HASHELP;
	m_Page2.SetPointer ( &m_data );

	AddPage(&m_Page3);										// CSV custom
	m_Page3.m_psp.dwFlags &= ~PSP_HASHELP;
	m_Page3.SetPointer ( &m_data );

	AddPage(&m_Page4);										// Audit Balances
	m_Page4.m_psp.dwFlags &= ~PSP_HASHELP;
	m_Page4.SetPointer ( &m_data );
}

//**********************************************************************

CSetupCsvImportPropertySheet::~CSetupCsvImportPropertySheet()
{
}

//**********************************************************************
BEGIN_MESSAGE_MAP(CSetupCsvImportPropertySheet, CSSAutoShutdownPropertySheet)
END_MESSAGE_MAP()
//**********************************************************************

void CSetupCsvImportPropertySheet::Save()
{
	m_data.Save();
}

//**********************************************************************
