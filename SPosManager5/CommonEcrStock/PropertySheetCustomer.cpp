/**********************************************************************/
#include "PropertySheetCustomer.h"
/**********************************************************************/

CPropertySheetCustomer::CPropertySheetCustomer( WORD wSpinID, bool bOneOff, int nIndex, CListDataDlgChangeFlag& WarnCancelAfterChange, CWnd* pWndParent)
	 : CSpinPropertySheet( TRUE, wSpinID, "", pWndParent ), 
	 m_PageGeneral( m_bAdminEnabled, m_nIndex, bOneOff ),
	 m_PageContact( m_nIndex, bOneOff ),
	 m_PageAutoDelete( m_nIndex, bOneOff ),
	 m_WarnCancelAfterChange( WarnCancelAfterChange )
{	
	m_pListDataDlg = NULL;
	m_psh.dwFlags |= PSH_NOAPPLYNOW;

	m_bOneOff = bOneOff;
	m_bAdminEnabled = FALSE;

	AddPage( &m_PageGeneral );
	m_PageArray.Add(1);

	AddPage( &m_PageContact );
	m_PageArray.Add(2);
	
	bool bAutoDelete = ( m_bOneOff ) ? EcrmanOptions.GetFeaturesCustomerOneOffAutoDeleteFlag() : EcrmanOptions.GetFeaturesCustomerAccountAutoDeleteFlag();

	m_PageAutoDelete.m_psp.dwFlags |= PSP_USETITLE;
	m_PageAutoDelete.m_psp.pszTitle = bAutoDelete ? "Auto Delete" : "Info";
	AddPage( &m_PageAutoDelete );
	m_PageArray.Add(3);
	
	m_nIndex = nIndex;
	
	UpdateTitle();
}

/**********************************************************************/

CPropertySheetCustomer::~CPropertySheetCustomer()
{
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropertySheetCustomer, CSpinPropertySheet)
	//{{AFX_MSG_MAP(CPropertySheetCustomer)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

void CPropertySheetCustomer::SpinNext()
{
	int nMaxIndex = ( m_bOneOff ) ? DataManagerNonDb.CustomerNameTable.OneOff.GetSize() : DataManagerNonDb.CustomerNameTable.Account.GetSize(); 

	if ( m_nIndex < nMaxIndex - 1 )
	{
		int nPage = m_PageArray.GetAt ( GetActiveIndex() );

		bool bUpdate = FALSE;

		switch ( nPage )
		{
		case 1:	bUpdate = m_PageGeneral.UpdateRecord();		break;
		case 2:	bUpdate = m_PageContact.UpdateRecord();		break;
		case 3:	bUpdate = m_PageAutoDelete.UpdateRecord();	break;
		}
		
		if ( bUpdate == FALSE )
			return;

		SaveRecord();
		m_nIndex++;
		UpdateTitle();

		switch ( nPage )
		{
		case 1:	m_PageGeneral.Refresh();	break;
		case 2:	m_PageContact.Refresh();	break;
		case 3:	m_PageAutoDelete.Refresh();	break;
		}

		if ( m_pListDataDlg != NULL )
			m_pListDataDlg -> SelectLine ( m_nIndex );
	}	
}

/**********************************************************************/

void CPropertySheetCustomer::SpinPrevious()
{
	if ( m_nIndex >= 1 )
	{
		int nPage = m_PageArray.GetAt ( GetActiveIndex() );

		bool bUpdate = FALSE;

		switch ( nPage )
		{
		case 1:	bUpdate = m_PageGeneral.UpdateRecord();		break;
		case 2:	bUpdate = m_PageContact.UpdateRecord();		break;
		case 3:	bUpdate = m_PageAutoDelete.UpdateRecord();	break;
		}
		
		if ( bUpdate == FALSE )
			return;

		SaveRecord();
		m_nIndex--;
		UpdateTitle();

		switch ( nPage )
		{
		case 1:	m_PageGeneral.Refresh();	break;
		case 2:	m_PageContact.Refresh();	break;
		case 3:	m_PageAutoDelete.Refresh();	break;
		}

		if ( m_pListDataDlg != NULL )
			m_pListDataDlg -> SelectLine ( m_nIndex );
	}
}

/**********************************************************************/

void CPropertySheetCustomer::SaveRecord()
{
	m_WarnCancelAfterChange.SetFlag();
}

/**********************************************************************/

void CPropertySheetCustomer::UndoChanges()
{
}

/**********************************************************************/

void CPropertySheetCustomer::UpdateTitle()
{
	CCustomerCSVRecord CustomerRecord;

	if ( FALSE == m_bOneOff )
	{
		int nCustomerIdx = DataManagerNonDb.CustomerNameTable.Account.GetSortOrderItem( m_nIndex );
		DataManagerNonDb.CustomerAccount.GetAt ( nCustomerIdx, CustomerRecord );
	}
	else
	{
		int nCustomerIdx = DataManagerNonDb.CustomerNameTable.OneOff.GetSortOrderItem( m_nIndex );
		DataManagerNonDb.CustomerOneOff.GetAt ( nCustomerIdx, CustomerRecord );
	}

	CString strTitle = "";
	strTitle.Format ( "C%8.8d %s", 
		CustomerRecord.GetCustomerID(), 
		CustomerRecord.GetDisplayNameFull() );

	SetTitle( strTitle, 0 );
}

/**********************************************************************/

BOOL CPropertySheetCustomer::OnInitDialog()
{  
	CPropertySheet::OnInitDialog();

	if ( TRUE == m_bOneOff )
	{
		RECT rc;
	
		// resize the sheet
		GetWindowRect (&rc);
		ScreenToClient (&rc);
		rc.bottom -= 50;      
		MoveWindow (&rc);
     
		// resize the CTabCtrl
		CTabCtrl* pTab = GetTabControl ();    
		ASSERT (pTab); 
		pTab->GetWindowRect (&rc);
		ScreenToClient (&rc);
		rc.bottom -= 50;      
		pTab->MoveWindow (&rc);

		// resize the page
		CPropertyPage* pPage = GetActivePage ();      
		ASSERT (pPage);     
		// store page size in m_PageRect
		pPage->GetWindowRect (&m_PageRect);
		ScreenToClient (&m_PageRect);      
		m_PageRect.bottom -= 50;      
		pPage->MoveWindow (&m_PageRect);
 
		// move the OK, Cancel, and Apply buttons
		CWnd* pWnd = GetDlgItem(IDOK);    
		pWnd->GetWindowRect(&rc);
		rc.top -= 50;
		rc.bottom -= 50;
		ScreenToClient(&rc);
		pWnd->MoveWindow(&rc);

		pWnd = GetDlgItem(IDCANCEL);    
		pWnd->GetWindowRect(&rc);    
		rc.top -= 50;
		rc.bottom -= 50;
		ScreenToClient(&rc);
		pWnd->MoveWindow(&rc);
	}

	CenterWindow();
	return TRUE;
}

/**********************************************************************/
