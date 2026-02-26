/**********************************************************************/
#include "PropertySheetCustomerNoSpin.h"
/**********************************************************************/

CPropertySheetCustomerNoSpin::CPropertySheetCustomerNoSpin( int nIndex, CListDataDlgChangeFlag& WarnCancelAfterChange, CWnd* pWndParent)
	 : CSSPropertySheet( "", pWndParent ), 
	 m_PageGeneral( m_bAdminEnabledDummy, m_nIndex, TRUE ),
	 m_PageContact( m_nIndex, TRUE ),
	 m_PageAutoDelete( m_nIndex, TRUE ),
	 m_WarnCancelAfterChange( WarnCancelAfterChange )
{	
	m_psh.dwFlags |= PSH_NOAPPLYNOW;

	m_bAdminEnabledDummy = TRUE;
	
	AddPage( &m_PageGeneral );
	m_PageArray.Add(1);

	AddPage( &m_PageContact );
	m_PageArray.Add(2);
	
	m_PageAutoDelete.m_psp.dwFlags |= PSP_USETITLE;
	m_PageAutoDelete.m_psp.pszTitle = EcrmanOptions.GetFeaturesCustomerOneOffAutoDeleteFlag() ? "Auto Delete" : "Info";
	AddPage( &m_PageAutoDelete );
	m_PageArray.Add(3);
	
	m_nIndex = nIndex;
	
	UpdateTitle();
}

/**********************************************************************/

CPropertySheetCustomerNoSpin::~CPropertySheetCustomerNoSpin()
{
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropertySheetCustomerNoSpin, CPropertySheet)
	//{{AFX_MSG_MAP(CPropertySheetCustomerNoSpin)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

void CPropertySheetCustomerNoSpin::SaveRecord()
{
	m_WarnCancelAfterChange.SetFlag();
}

/**********************************************************************/

void CPropertySheetCustomerNoSpin::UndoChanges()
{
}

/**********************************************************************/

void CPropertySheetCustomerNoSpin::UpdateTitle()
{
	CCustomerCSVRecord CustomerRecord;

	int nCustomerIdx = DataManagerNonDb.CustomerNameTable.OneOff.GetSortOrderItem( m_nIndex );
	DataManagerNonDb.CustomerOneOff.GetAt ( nCustomerIdx, CustomerRecord );

	CString strTitle = "";
	strTitle.Format ( "C%8.8d %s", 
		CustomerRecord.GetCustomerID(), 
		CustomerRecord.GetDisplayNameFull() );

	SetTitle( strTitle, 0 );
}

/**********************************************************************/

BOOL CPropertySheetCustomerNoSpin::OnInitDialog()
{  
	CPropertySheet::OnInitDialog();

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
