/**********************************************************************/
#include "SysInfo.h"
/**********************************************************************/
#include "PropertySheetPassword.h"
/**********************************************************************/
IMPLEMENT_DYNAMIC(CPropertySheetPassword, CSpinPropertySheet)
/**********************************************************************/

CPropertySheetPassword::CPropertySheetPassword(WORD wSpinID, int nPasswordIdx, CWnd* pWndParent)
	: CSpinPropertySheet(TRUE, wSpinID, "", pWndParent)
{
	m_psh.dwFlags |= PSH_NOAPPLYNOW;
	m_Page1.SetPasswordRecord(&m_PasswordRecord);
	AddPage(&m_Page1);
	m_PageArray.Add(1);

	m_psh.dwFlags |= PSH_NOAPPLYNOW;
	m_Page2.SetPasswordRecord(&m_PasswordRecord);
	AddPage(&m_Page2);
	m_PageArray.Add(2);

	m_Page3.SetPasswordRecord(&m_PasswordRecord);
	m_Page3.SetPageType(PASSWORDTICKS_PAGE_FILE);
	m_Page3.m_psp.dwFlags |= PSP_USETITLE;
	m_Page3.m_psp.pszTitle = "File";
	AddPage(&m_Page3);
	m_PageArray.Add(3);

	m_Page4.SetPasswordRecord(&m_PasswordRecord);
	m_Page4.SetPageType(PASSWORDTICKS_PAGE_SYSTEM);
	m_Page4.m_psp.dwFlags |= PSP_USETITLE;
	m_Page4.m_psp.pszTitle = "System";
	AddPage(&m_Page4);
	m_PageArray.Add(4);

#ifdef STOCKMAN_SYSTEM
	m_Page9.SetPasswordRecord(&m_PasswordRecord);
	m_Page9.SetPageType(PASSWORDTICKS_PAGE_STOCKTAKE);
	m_Page9.m_psp.dwFlags |= PSP_USETITLE;
	m_Page9.m_psp.pszTitle = "Stocktake";
	AddPage(&m_Page9);
	m_PageArray.Add(9);
#endif

	m_Page5.SetPasswordRecord(&m_PasswordRecord);
	m_Page5.SetPageType(PASSWORDTICKS_PAGE_TASKS);
	m_Page5.m_psp.dwFlags |= PSP_USETITLE;
	m_Page5.m_psp.pszTitle = "Tasks";
	AddPage(&m_Page5);
	m_PageArray.Add(5);

	m_Page6.SetPasswordRecord(&m_PasswordRecord);
	m_Page6.SetPageType(PASSWORDTICKS_PAGE_REPORTS);
	m_Page6.m_psp.dwFlags |= PSP_USETITLE;
	m_Page6.m_psp.pszTitle = "Reports";
	AddPage(&m_Page6);
	m_PageArray.Add(6);

	if (SysInfo.GotLoyaltyOrSmartPayLink())
	{
		m_Page10.SetPasswordRecord(&m_PasswordRecord);
		m_Page10.SetPageType(PASSWORDTICKS_PAGE_SMARTCARD);
		m_Page10.m_psp.dwFlags |= PSP_USETITLE;

		if (SysInfo.GotSmartPayLink())
			m_Page10.m_psp.pszTitle = "Smartpay";
		else
			m_Page10.m_psp.pszTitle = "Loyalty";

		AddPage(&m_Page10);
		m_PageArray.Add(10);
	}

	if (SysInfo.IsPMSSystem() == TRUE)
	{
		m_Page11.SetPasswordRecord(&m_PasswordRecord);
		m_Page11.SetPageType(PASSWORDTICKS_PAGE_PMS);
		m_Page11.m_psp.dwFlags |= PSP_USETITLE;
		m_Page11.m_psp.pszTitle = "Guest Accounts";
		AddPage(&m_Page11);
		m_PageArray.Add(11);
	}

	m_Page7.SetPasswordRecord(&m_PasswordRecord);
	m_Page7.SetPageType(PASSWORDTICKS_PAGE_ADMIN);
	m_Page7.m_psp.dwFlags |= PSP_USETITLE;
	m_Page7.m_psp.pszTitle = "Admin";
	AddPage(&m_Page7);
	m_PageArray.Add(7);

	m_Page8.SetPasswordRecord(&m_PasswordRecord);
	m_Page8.SetPageType(PASSWORDTICKS_PAGE_SETUP);
	m_Page8.m_psp.dwFlags |= PSP_USETITLE;
	m_Page8.m_psp.pszTitle = "Setup";
	AddPage(&m_Page8);
	m_PageArray.Add(8);

	m_nPasswordIdx = nPasswordIdx;
	PasswordArray.GetAt(m_nPasswordIdx, m_PasswordRecord);
	UpdateTitle();
}

/**********************************************************************/

CPropertySheetPassword::~CPropertySheetPassword()
{
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropertySheetPassword, CSpinPropertySheet)
	//{{AFX_MSG_MAP(CPropertySheetPassword)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

void CPropertySheetPassword::SpinNext()
{
	if ((m_nPasswordIdx < PasswordArray.GetSize() - 1) && (m_Page2.GetBlockSpinButtonFlag() == FALSE))
	{
		int nPage = m_PageArray.GetAt(GetActiveIndex());

		bool bUpdate = FALSE;

		switch (nPage)
		{
		case 1:	bUpdate = m_Page1.UpdateRecord();	break;
		case 2:	bUpdate = m_Page2.UpdateRecord();	break;
		case 3:	bUpdate = m_Page3.UpdateRecord();	break;
		case 4:	bUpdate = m_Page4.UpdateRecord();	break;
		case 5:	bUpdate = m_Page5.UpdateRecord();	break;
		case 6:	bUpdate = m_Page6.UpdateRecord();	break;
		case 7:	bUpdate = m_Page7.UpdateRecord();	break;
		case 8:	bUpdate = m_Page8.UpdateRecord();	break;
		case 9:	bUpdate = m_Page9.UpdateRecord();	break;
		case 10: bUpdate = m_Page10.UpdateRecord();	break;
		case 11: bUpdate = m_Page11.UpdateRecord();	break;
		}

		if (bUpdate == FALSE)
			return;

		SaveRecord();
		m_nPasswordIdx++;
		PasswordArray.GetAt(m_nPasswordIdx, m_PasswordRecord);
		UpdateTitle();

		switch (nPage)
		{
		case 1:	m_Page1.Refresh();	break;
		case 2:	m_Page2.Refresh();	break;
		case 3:	m_Page3.Refresh();	break;
		case 4:	m_Page4.Refresh();	break;
		case 5:	m_Page5.Refresh();	break;
		case 6:	m_Page6.Refresh();	break;
		case 7:	m_Page7.Refresh();	break;
		case 8:	m_Page8.Refresh();	break;
		case 9:	m_Page9.Refresh();	break;
		case 10: m_Page10.Refresh();	break;
		case 11: m_Page11.Refresh();	break;
		}
	}
}

/**********************************************************************/

void CPropertySheetPassword::SpinPrevious()
{
	if ((m_nPasswordIdx > 1) && (m_Page2.GetBlockSpinButtonFlag() == FALSE))
	{
		int nPage = m_PageArray.GetAt(GetActiveIndex());

		bool bUpdate = FALSE;

		switch (nPage)
		{
		case 1:	bUpdate = m_Page1.UpdateRecord();	break;
		case 2:	bUpdate = m_Page2.UpdateRecord();	break;
		case 3:	bUpdate = m_Page3.UpdateRecord();	break;
		case 4:	bUpdate = m_Page4.UpdateRecord();	break;
		case 5:	bUpdate = m_Page5.UpdateRecord();	break;
		case 6:	bUpdate = m_Page6.UpdateRecord();	break;
		case 7:	bUpdate = m_Page7.UpdateRecord();	break;
		case 8:	bUpdate = m_Page8.UpdateRecord();	break;
		case 9:	bUpdate = m_Page9.UpdateRecord();	break;
		case 10: bUpdate = m_Page10.UpdateRecord();	break;
		case 11: bUpdate = m_Page11.UpdateRecord();	break;
		}

		if (bUpdate == FALSE)
			return;

		SaveRecord();
		m_nPasswordIdx--;
		PasswordArray.GetAt(m_nPasswordIdx, m_PasswordRecord);
		UpdateTitle();

		switch (nPage)
		{
		case 1:	m_Page1.Refresh();	break;
		case 2:	m_Page2.Refresh();	break;
		case 3:	m_Page3.Refresh();	break;
		case 4:	m_Page4.Refresh();	break;
		case 5:	m_Page5.Refresh();	break;
		case 6:	m_Page6.Refresh();	break;
		case 7:	m_Page7.Refresh();	break;
		case 8:	m_Page8.Refresh();	break;
		case 9:	m_Page9.Refresh();	break;
		case 10: m_Page10.Refresh();	break;
		case 11: m_Page11.Refresh();	break;
		}
	}
}

/**********************************************************************/

void CPropertySheetPassword::SaveRecord()
{
	PasswordArray.SetAt ( m_nPasswordIdx, m_PasswordRecord );
}

/**********************************************************************/

void CPropertySheetPassword::UpdateTitle()
{
	CString strUserName = m_PasswordRecord.GetUserName();

	if ( strUserName == "" )
		strUserName = "No Username";

	CString strText; 
	strText.Format ( "Password Settings (%2.2d, %s)",
		m_nPasswordIdx,
		( const char* ) strUserName );

	SetTitle ( strText, 0 );
}

/**********************************************************************/
