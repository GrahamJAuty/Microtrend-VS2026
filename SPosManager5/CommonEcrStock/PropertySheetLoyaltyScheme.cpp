/**********************************************************************/
#include "PropertySheetLoyaltyScheme.h"
/**********************************************************************/
IMPLEMENT_DYNAMIC(CPropertySheetLoyaltyScheme, CSpinPropertySheet)
/**********************************************************************/

CPropertySheetLoyaltyScheme::CPropertySheetLoyaltyScheme( WORD wSpinID, int nSchemeIdx, CListDataDlgChangeFlag& WarnCancelAfterChange, CWnd* pWndParent)
	 : CSpinPropertySheet( TRUE, wSpinID, "", pWndParent ), m_WarnCancelAfterChange( WarnCancelAfterChange )
{	
	m_pListDataDlg = NULL;
	m_psh.dwFlags |= PSH_NOAPPLYNOW;

	m_nSchemeIdx = nSchemeIdx;
	DataManager.LoyaltyScheme.GetAt ( m_nSchemeIdx, m_SchemeRecord );
	
	m_Page1.SetSchemeRecord ( &m_SchemeRecord );
	AddPage( &m_Page1 );
	m_PageArray.Add ( 1 );

	m_Page2.SetSchemeRecord ( &m_SchemeRecord );
	AddPage( &m_Page2 );
	m_PageArray.Add ( 2 );

	m_Page3.SetSchemeRecord ( &m_SchemeRecord );
	AddPage( &m_Page3 );
	m_PageArray.Add ( 3 );

	m_Page4.SetSchemeRecord ( &m_SchemeRecord );
	AddPage( &m_Page4 );
	m_PageArray.Add ( 4 );
	
	UpdateTitle();
}

/**********************************************************************/

CPropertySheetLoyaltyScheme::~CPropertySheetLoyaltyScheme()
{
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropertySheetLoyaltyScheme, CSpinPropertySheet)
	//{{AFX_MSG_MAP(CPropertySheetLoyaltyScheme)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

void CPropertySheetLoyaltyScheme::SpinNext()
{
	if (m_nSchemeIdx < DataManager.LoyaltyScheme.GetSize() - 1)
	{
		int nPage = m_PageArray.GetAt(GetActiveIndex());

		bool bUpdate = FALSE;

		switch (nPage)
		{
		case 1:	bUpdate = m_Page1.UpdateRecord();	break;
		case 2:	bUpdate = m_Page2.UpdateRecord();	break;
		case 3:	bUpdate = m_Page3.UpdateRecord();	break;
		case 4:	bUpdate = m_Page4.UpdateRecord();	break;
		}

		if (bUpdate == FALSE)
		{
			return;
		}

		SaveRecord();
		m_nSchemeIdx++;
		DataManager.LoyaltyScheme.GetAt(m_nSchemeIdx, m_SchemeRecord);
		UpdateTitle();

		switch (nPage)
		{
		case 1:	m_Page1.Refresh();	break;
		case 2:	m_Page2.Refresh();	break;
		case 3:	m_Page3.Refresh();	break;
		case 4:	m_Page4.Refresh();	break;
		}

		if (m_pListDataDlg != NULL)
		{
			m_pListDataDlg->SelectLine(m_nSchemeIdx);
		}
	}
}

/**********************************************************************/

void CPropertySheetLoyaltyScheme::SpinPrevious()
{
	if (m_nSchemeIdx >= 1)
	{
		int nPage = m_PageArray.GetAt(GetActiveIndex());

		bool bUpdate = FALSE;

		switch (nPage)
		{
		case 1:	bUpdate = m_Page1.UpdateRecord();	break;
		case 2:	bUpdate = m_Page2.UpdateRecord();	break;
		case 3:	bUpdate = m_Page3.UpdateRecord();	break;
		case 4:	bUpdate = m_Page4.UpdateRecord();	break;
		}

		if (bUpdate == FALSE)
		{
			return;
		}

		SaveRecord();
		m_nSchemeIdx--;
		DataManager.LoyaltyScheme.GetAt(m_nSchemeIdx, m_SchemeRecord);
		UpdateTitle();

		switch (nPage)
		{
		case 1:	m_Page1.Refresh();	break;
		case 2:	m_Page2.Refresh();	break;
		case 3:	m_Page3.Refresh();	break;
		case 4:	m_Page4.Refresh();	break;
		}

		if (m_pListDataDlg != NULL)
		{
			m_pListDataDlg->SelectLine(m_nSchemeIdx);
		}
	}
}

/**********************************************************************/

void CPropertySheetLoyaltyScheme::SaveRecord()
{
	DataManager.LoyaltyScheme.SetAt ( m_nSchemeIdx, m_SchemeRecord );
	m_WarnCancelAfterChange.SetFlag();
}

/**********************************************************************/

void CPropertySheetLoyaltyScheme::UpdateTitle()
{
	CString strTitle;
	strTitle.Format ( "P%2.2d %s", m_SchemeRecord.GetSchemeNo(), m_SchemeRecord.GetDisplayName() );
	SetTitle ( strTitle, 0 );
}

/**********************************************************************/
