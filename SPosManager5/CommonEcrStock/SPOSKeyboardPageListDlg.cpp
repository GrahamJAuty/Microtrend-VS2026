/**********************************************************************/
#include "SPOSKeyboardKeyHelpers.h"
#include "SPOSKeyboardPageAddDlg.h"
/**********************************************************************/
#include "SPOSKeyboardPageListDlg.h"
/**********************************************************************/

CSPOSKeyboardPageListDlg::CSPOSKeyboardPageListDlg( int nListIdx, int nPageNo, CWordArray& arrayPageTypes, CWnd* pParent)
	: CDialog(CSPOSKeyboardPageListDlg::IDD, pParent), m_arrayPageTypes( arrayPageTypes )
{
	//{{AFX_DATA_INIT(CSPOSKeyboardPageListDlg)
	//}}AFX_DATA_INIT
	m_pView = NULL;
	
	m_nListIdx = nListIdx;
	SPOSKeyboardManager.GetListManager() -> GetListRecord( m_nListIdx, m_ListRecord );

	m_arrayPageTypes.RemoveAll();
	for ( int n = 1; n <= m_ListRecord.GetPageCount(); n++ )
		m_arrayPageTypes.Add(n);

	m_nInitialPageNo = nPageNo;
}

/**********************************************************************/

void CSPOSKeyboardPageListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSPOSKeyboardPageListDlg)
	DDX_Control(pDX, IDC_EDIT_PAGES, m_editPages);
	DDX_Control(pDX, IDC_EDIT_KEYS, m_editKeys);
	DDX_Control(pDX, IDC_EDIT_FORMAT, m_editFormat);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CSPOSKeyboardPageListDlg, CDialog)
	//{{AFX_MSG_MAP(CSPOSKeyboardPageListDlg)
	ON_WM_MEASUREITEM()
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_MOVEDOWN, OnButtonMoveDown)
	ON_BN_CLICKED(IDC_BUTTON_MOVEUP, OnButtonMoveUp)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST, OnSelectPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CSPOSKeyboardPageListDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();	

	m_listPages.SubclassDlgItem ( IDC_LIST, this );
	m_listPages.InsertColumn ( 0, "", LVCFMT_LEFT, 40 );
	m_listPages.InsertColumn ( 1, "", LVCFMT_LEFT, 220 );
	m_listPages.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );

	CSPOSKeyboardKeyHelpers KeyHelpers;
	CString strCombiText = KeyHelpers.GetCombiText( KEY_TYPE_LIST, m_ListRecord.GetListNo() );

	CString strTitle;
	strTitle.Format ( "Move List Pages (%s)", (const char*) strCombiText );
	
	SetWindowText( strTitle );

	AddList();
	UpdateCounters();

	int nSel = m_nInitialPageNo - 1;
	if ( nSel >= 0 && nSel < m_listPages.GetItemCount() )
		m_listPages.SetCurSel( nSel );

	return TRUE;  
}

/**********************************************************************/

void CSPOSKeyboardPageListDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
   if ( nIDCtl == IDC_LIST ) 
   {
	   m_listPages.AdjustRowHeight( lpMeasureItemStruct, 10 );
   }
}

/**********************************************************************/

void CSPOSKeyboardPageListDlg::OnSelectPage(NMHDR* pNMHDR, LRESULT* pResult)  
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	
	if ( ( pNMListView -> uNewState & LVIS_SELECTED ) != 0 )
	{
		HandleSelectPage();
	}
	*pResult = 0;
}

/**********************************************************************/

void CSPOSKeyboardPageListDlg::HandleSelectPage()
{
	int nSel = m_listPages.GetCurSel();
	
	if ( m_pView != NULL )
	{
		if ( nSel >= 0 && nSel < m_arrayPageTypes.GetSize() )
		{
			int nPageType = m_arrayPageTypes.GetAt( nSel );
		
			if ( nPageType >= 1001 && nPageType <= 1000 + MAX_LIST_PAGES )
				nPageType -= 1000;

			if ( nPageType >= 1 && nPageType <= MAX_LIST_PAGES )
				m_pView -> SelectPage( nPageType );
		}
	}
}

/**********************************************************************/

void CSPOSKeyboardPageListDlg::OnButtonAdd() 
{
	int nPageCount = m_arrayPageTypes.GetSize();
	int nMaxNewPages = m_ListRecord.GetMaxNewPages( nPageCount );
	
	if ( nMaxNewPages == -1 )
	{
		Prompter.Error( m_ListRecord.GetPageError(1, nPageCount ) );
		return;
	}

	if ( nMaxNewPages == -2 )
	{
		Prompter.Error( m_ListRecord.GetPageError(2, nPageCount ) );
		return;
	}
	
	int nPageNo = m_listPages.GetCurSel() + 1;
	if ( nPageNo < 1 || nPageNo > MAX_LIST_PAGES )
		return;

	CSPOSKeyboardPageAddDlg dlg( nPageNo, nMaxNewPages, this );
	if ( dlg.DoModal() != IDOK )
		return;

	int nNewPageToInsert = 0;
	if ( dlg.m_bCopyVisible == TRUE )  
		nNewPageToInsert = m_arrayPageTypes.GetAt(nPageNo - 1);

	if ( nNewPageToInsert >= 1 && nNewPageToInsert <= MAX_LIST_PAGES )
		nNewPageToInsert += 1000;

	int nInsertPos;
	switch( dlg.m_nMethod )
	{
	case ADD_PAGES_END:		
		nInsertPos = nPageCount;	
		break;
	
	case ADD_PAGES_START:	
		nInsertPos = 0;		
		break;
	
	case ADD_PAGES_MIDDLE:	
		nInsertPos = nPageNo;	
		break;
	}

	m_arrayPageTypes.InsertAt( nInsertPos, nNewPageToInsert, dlg.m_nPagesToAdd );
	
	if ( nInsertPos < nPageNo )
		nPageNo += dlg.m_nPagesToAdd;

	AddList();
	UpdateCounters();
	m_listPages.SetCurSel( nPageNo - 1 );

	HandleSelectPage();
}

/**********************************************************************/

void CSPOSKeyboardPageListDlg::OnButtonDelete() 
{
	int nPageCount = m_arrayPageTypes.GetSize();

	if ( nPageCount < 2 )
	{
		Prompter.Error ( "You must leave at least one page in this list." );
		return;
	}

	int nSel = m_listPages.GetCurSel();
	if ( nSel < 0 || nSel >= m_arrayPageTypes.GetSize() )
		return;

	m_arrayPageTypes.RemoveAt( nSel );

	AddList();

	if ( nSel < m_listPages.GetItemCount() )
		m_listPages.SetCurSel(nSel);
	else
		m_listPages.SetCurSel(nSel - 1 );

	HandleSelectPage();
	UpdateCounters();
}

/**********************************************************************/

void CSPOSKeyboardPageListDlg::OnButtonMoveDown() 
{
	int nSel = m_listPages.GetCurSel();
	if ( nSel < 0 || nSel >= m_arrayPageTypes.GetSize() - 1 )
		return;

	WORD w = m_arrayPageTypes.GetAt(nSel);
	m_arrayPageTypes.SetAt( nSel, m_arrayPageTypes.GetAt( nSel + 1 ) );
	m_arrayPageTypes.SetAt( nSel + 1, w );

	AddList();
	m_listPages.SetCurSel( nSel + 1 );
}

/**********************************************************************/

void CSPOSKeyboardPageListDlg::OnButtonMoveUp() 
{
	int nSel = m_listPages.GetCurSel();
	if ( nSel < 1 || nSel >= m_arrayPageTypes.GetSize() )
		return;

	WORD w = m_arrayPageTypes.GetAt(nSel);
	m_arrayPageTypes.SetAt( nSel, m_arrayPageTypes.GetAt( nSel - 1 ) );
	m_arrayPageTypes.SetAt( nSel - 1, w );

	AddList();
	m_listPages.SetCurSel( nSel - 1 );
}

/**********************************************************************/

void CSPOSKeyboardPageListDlg::AddList()
{
	m_listPages.DeleteAllItems();
	for ( int n = 0; n < m_arrayPageTypes.GetSize(); n++ )
	{
		CString strText;
		strText.Format( "%d\t", n + 1 );

		int nPageType = m_arrayPageTypes.GetAt( n );
			
		if ( nPageType >= 1 && nPageType <= MAX_LIST_PAGES )
		{
			CString strName;
			strName.Format ( "Current page %d", nPageType );
			strText += strName;
		}
		else if ( nPageType >= 1001 && nPageType <= 1000 + MAX_LIST_PAGES )
		{
			CString strName;
			strName.Format ( "Copy of Current page %d", nPageType - 1000 );
			strText += strName;	
		}
		else if ( nPageType == 0 )
			strText += "New blank page";
		else
			strText += "Unknown page type";

		m_listPages.AddString( strText );
	}
}

/**********************************************************************/

void CSPOSKeyboardPageListDlg::UpdateCounters()
{
	CString strPages;
	strPages.Format( "%d", m_listPages.GetItemCount() );
	m_editPages.SetWindowText( strPages );

	CString strFormat;
	strFormat.Format( "%d x %d", m_ListRecord.GetRowCount(), m_ListRecord.GetColumnCount() );
	m_editFormat.SetWindowText( strFormat );

	CString strKeyCount;
	strKeyCount.Format( "%d", m_listPages.GetItemCount() * m_ListRecord.GetKeysPerPage() );
	m_editKeys.SetWindowText( strKeyCount );
}

/**********************************************************************/

void CSPOSKeyboardPageListDlg::OnOK() 
{
	for ( int n = 0; n < m_arrayPageTypes.GetSize(); n++ )
	{
		int x = m_arrayPageTypes.GetAt(n);
		if ( x >= 1001 && n <= 1000 + MAX_LIST_PAGES ) 
			m_arrayPageTypes.SetAt( n, x - 1000 );
	}
	EndDialog( IDOK );
}

/**********************************************************************/

