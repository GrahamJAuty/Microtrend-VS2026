/**********************************************************************/
#include "SelectMultipleEntityDlg.h"
/**********************************************************************/

CSelectMultipleEntityDlg::CSelectMultipleEntityDlg( const char* szTitle, CArray<CSelectMultipleEntityInfo,CSelectMultipleEntityInfo>& arrayEntityInfo, bool bWantAllNone, bool bAutoSelectSingle, CWnd* pParent )
	: CSSAutoShutdownDialog(AutoShutdownHelper,CSelectMultipleEntityDlg::IDD, pParent), m_arrayEntityInfo( arrayEntityInfo )
{
	//{{AFX_DATA_INIT(CSelectMultipleEntityDlg)
	//}}AFX_DATA_INIT
	m_bWantAllNone = bWantAllNone;
	m_strTitle = szTitle;
	m_nColumnCount = 0;
	m_bEndAsCancel = FALSE;
	m_bAutoSelectSingle = bAutoSelectSingle;
	m_nInitialSel = -1;
	m_bDoubleClickExit = FALSE;
}

/**********************************************************************/

void CSelectMultipleEntityDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelectMultipleEntityDlg)
	DDX_Control( pDX, IDC_BUTTON_ALL, m_buttonAll );
	DDX_Control( pDX, IDC_BUTTON_NONE, m_buttonNone );
	DDX_Control( pDX, IDOK, m_buttonOK );
	DDX_Control( pDX, IDCANCEL, m_buttonCancel );
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CSelectMultipleEntityDlg, CSSAutoShutdownDialog)
	//{{AFX_MSG_MAP(CSelectMultipleEntityDlg)
	ON_WM_MEASUREITEM()
	ON_BN_CLICKED( IDC_BUTTON_ALL, OnButtonAll )
	ON_BN_CLICKED( IDC_BUTTON_NONE, OnButtonNone )
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDoubleClickList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CSelectMultipleEntityDlg::CSSAutoShutdownPostInitDialog()
{
	SetWindowText ( m_strTitle );

	if ( FALSE == m_bWantAllNone )
	{
		ShowAndEnableWindow( &m_buttonAll, FALSE );
		ShowAndEnableWindow( &m_buttonNone, FALSE );
	}
	
	m_listEntities.SubclassDlgItem ( IDC_LIST, this );
	m_listEntities.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	
	SetListColumnns();
	GetListData();

	RedrawList();

	if ( m_nInitialSel != -1 )
		m_listEntities.EnsureVisible( m_nInitialSel, FALSE );

	return TRUE;  
}

/**********************************************************************/

void CSelectMultipleEntityDlg::RedrawList()
{
	m_listEntities.DeleteAllItems();

	int nSize = m_arrayEntityInfo.GetSize();
	bool bAutoSelectSingle = ( 1 == nSize ) && ( TRUE == m_bAutoSelectSingle );
	
	for ( int nIndex = 0; nIndex < nSize; nIndex++ )
	{		
		m_listEntities.InsertItem ( nIndex, "" );

		CSelectMultipleEntityInfo item = m_arrayEntityInfo.GetAt(nIndex);
		
		for ( int n = 0; n < m_nColumnCount; n++ )
			m_listEntities.SetItem ( nIndex, n, LVIF_TEXT, item.m_strText[n], 0, 0, 0, NULL );
		
		if ( ( TRUE == item.m_bSelected ) || ( TRUE == bAutoSelectSingle ) )
		{
			m_listEntities.SetItemState(nIndex, LVIS_SELECTED, LVIS_SELECTED);
			m_listEntities.SetSelectionMark(nIndex);	
		}
	}
}

/**********************************************************************/

void CSelectMultipleEntityDlg::GetListData()
{
	m_nColumnCount = 0;
}

/**********************************************************************/

void CSelectMultipleEntityDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	if ( nIDCtl == IDC_LIST )	
	{
		m_listEntities.AdjustRowHeight( lpMeasureItemStruct, 10 );
	}
}

/**********************************************************************/

void CSelectMultipleEntityDlg::OnButtonAll()
{
	HandleAll();
}

/**********************************************************************/

void CSelectMultipleEntityDlg::HandleAll()
{
	if ( TRUE == m_bWantAllNone )
	{
		for ( int nIndex = 0; nIndex < m_listEntities.GetItemCount(); nIndex++ )
		{
			m_listEntities.SetItemState(nIndex, LVIS_SELECTED, LVIS_SELECTED);
			m_listEntities.SetSelectionMark(nIndex);	
		}
	}
}

/**********************************************************************/

void CSelectMultipleEntityDlg::OnButtonNone()
{
	if ( TRUE == m_bWantAllNone )
	{
		for ( int nIndex = 0; nIndex < m_listEntities.GetItemCount(); nIndex++ )
			m_listEntities.SetItemState(nIndex, 0, LVIS_SELECTED);
	}
}

/**********************************************************************/

void CSelectMultipleEntityDlg::OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{	
	if ( TRUE == m_bDoubleClickExit )
	{
		OnOK();
		*pResult = 0;
	}
}

/**********************************************************************/

void CSelectMultipleEntityDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		if ( HandleOK() == TRUE )
		{
			EndDialog ( m_bEndAsCancel ? IDCANCEL : IDOK );
		}
	}
}

/**********************************************************************/

bool CSelectMultipleEntityDlg::HandleOK() 
{
	for ( int nIndex = 0; nIndex < m_listEntities.GetItemCount(); nIndex++ )
		m_arrayEntityInfo[nIndex].m_bSelected = ( m_listEntities.GetItemState( nIndex, LVIS_SELECTED ) != 0 );

	return TRUE;
}

/**********************************************************************/

void CSelectMultipleEntityDlg::OnCancel() 
{
	m_bEndAsCancel = TRUE;
	HandleCancel();
	EndDialog ( m_bEndAsCancel ? IDCANCEL : IDOK );
}

/**********************************************************************/

void CSelectMultipleEntityDlg::HandleCancel() 
{
}

/**********************************************************************/
