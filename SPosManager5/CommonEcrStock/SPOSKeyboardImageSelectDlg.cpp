/**********************************************************************/
#include "SPOSKeyboardImageLibrary.h"
/**********************************************************************/
#include "SPOSKeyboardImageSelectDlg.h"
/**********************************************************************/

CSPOSKeyboardImageSelectDlg::CSPOSKeyboardImageSelectDlg( const char* szOld, CWnd* pParent)
	: CDialog(CSPOSKeyboardImageSelectDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSPOSKeyboardImageSelectDlg)
	//}}AFX_DATA_INIT
	m_strOldSelectedImage = szOld;
	m_strNewSelectedImage = "";
	m_strFilter = "";
}

/**********************************************************************/

void CSPOSKeyboardImageSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSPOSKeyboardImageSelectDlg)
	DDX_Control(pDX, IDC_EDIT_FILTER, m_editFilter);
	DDX_Control(pDX, IDC_EDIT_WIDTH, m_editWidth);
	DDX_Control(pDX, IDC_EDIT_HEIGHT, m_editHeight);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CSPOSKeyboardImageSelectDlg, CDialog)
	//{{AFX_MSG_MAP(CSPOSKeyboardImageSelectDlg)
	ON_WM_MEASUREITEM()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDoubleClickList)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST, OnGetDispInfoList)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST, OnSelectListItem)
	ON_EN_KILLFOCUS( IDC_EDIT_FILTER, OnKillFocusFilter )
	ON_BN_CLICKED( IDC_BUTTON_REFRESH, OnButtonRefresh )
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CSPOSKeyboardImageSelectDlg::OnInitDialog() 
{
	m_staticImage.SubclassDlgItem(IDC_STATIC_IMAGE,this);
	CDialog::OnInitDialog();

	m_editFilter.LimitText(20);

	SPOSKeyboardManager.GetImageLibrary() -> Refresh( FALSE );

	if ( m_strOldSelectedImage != "" )
		SPOSKeyboardManager.GetImageLibrary() -> AddFilename( m_strOldSelectedImage );
	
	m_listFiles.SubclassDlgItem ( IDC_LIST, this );
	m_listFiles.SetItemCountEx(0);
	m_listFiles.ModifyStyle( 0, LVS_SINGLESEL, 0 );
	m_listFiles.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_listFiles.InsertColumn ( 0, "Filename", LVCFMT_LEFT, 190 );

	m_editWidth.SetWindowText( "" );
	m_editHeight.SetWindowText( "" );
	
	CreateList();

	int nIdx;
	if ( SPOSKeyboardManager.GetImageLibrary() -> FindFilename( m_strOldSelectedImage, nIdx ) == TRUE )
		SelectLine(nIdx);
	else
		SelectLine(0);

	return TRUE;  
}

/**********************************************************************/

void CSPOSKeyboardImageSelectDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	if ( nIDCtl == IDC_LIST )	
	{
		m_listFiles.AdjustRowHeight( lpMeasureItemStruct, 10 );
	}
}

/**********************************************************************/

void CSPOSKeyboardImageSelectDlg::CreateList() 
{
	m_arrayImageIdx.RemoveAll();

	if ( m_strFilter == "" )
	{
		for ( int n = 0; n < SPOSKeyboardManager.GetImageLibrary() -> GetImageCount(); n++ )
			m_arrayImageIdx.Add(n);
	}
	else
	{
		CString strTest = m_strFilter;
		strTest.MakeUpper();

		for ( int n = 0; n < SPOSKeyboardManager.GetImageLibrary() -> GetImageCount(); n++ )
		{
			CString strImage = SPOSKeyboardManager.GetImageLibrary() -> GetFilename(n);	
			strImage.MakeUpper();

			if ( strImage.GetLength() > 4 )
			{
				if ( strImage.Mid( strImage.GetLength() - 4, 1 ) == "." )
					strImage = strImage.Left( strImage.GetLength() - 4 );
			}

			if ( strImage.Find( strTest ) != -1 )
				m_arrayImageIdx.Add(n);
		}
	}

	m_listFiles.SetItemCountEx( m_arrayImageIdx.GetSize() );
	SelectLine(0);
	DisplaySelectedImage();
}

/**********************************************************************/

void CSPOSKeyboardImageSelectDlg::SelectLine( int nIndex )
{
	m_listFiles.Invalidate();

	for ( int n = 0; n < m_listFiles.GetItemCount(); n++ )
	{
		if ( n != nIndex )
			m_listFiles.SetItemState(n, 0, LVIS_SELECTED | LVIS_FOCUSED );
		else
		{
			m_listFiles.SetItemState(n, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED );
			m_listFiles.SetSelectionMark(n);
		}
	}
}

/**********************************************************************/

void CSPOSKeyboardImageSelectDlg::DisplaySelectedImage() 
{
	CString strFilename = "";
	if ( GetSelectedFilename( strFilename ) == TRUE )
	{
		CString strFilepath = "";
		strFilepath += SPOSKeyboardManager.GetLayoutOptions() -> GetImageFolder();
		strFilepath += "\\";
		strFilepath += strFilename;

		if ( m_staticImage.LoadImage( strFilepath ) == TRUE )
		{
			m_staticImage.SetFailedFlag( FALSE );
			m_staticImage.SetBlankFlag( FALSE );
			m_staticImage.Invalidate();
			m_editWidth.SetWindowText( m_staticImage.GetWidthString() );
			m_editHeight.SetWindowText( m_staticImage.GetHeightString() );
		}
		else
		{
			m_staticImage.SetFailedFlag( TRUE );
			m_staticImage.SetBlankFlag( TRUE );
			m_staticImage.Invalidate();
			m_editWidth.SetWindowText( "" );
			m_editHeight.SetWindowText( "" );
		}
	}
	else
	{
		m_staticImage.SetFailedFlag( FALSE );
		m_staticImage.SetBlankFlag( TRUE );
		m_staticImage.Invalidate();
		m_editWidth.SetWindowText( "" );
		m_editHeight.SetWindowText( "" );
	}
}

/**********************************************************************/

void CSPOSKeyboardImageSelectDlg::OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	if (( pDispInfo->item.mask & LVIF_TEXT ) != 0 )
	{
		int nIndex = pDispInfo->item.iItem;
		
		CString strFilename = "";

		if ( ( nIndex >= 0 ) && ( nIndex < m_arrayImageIdx.GetSize() ) )
			strFilename = SPOSKeyboardManager.GetImageLibrary() -> GetFilename( m_arrayImageIdx.GetAt(nIndex));
		
		lstrcpy ( pDispInfo->item.pszText, strFilename );
	}
	*pResult = 0;
}

/**********************************************************************/

void CSPOSKeyboardImageSelectDlg::OnSelectListItem(NMHDR* pNMHDR, LRESULT* pResult) 
{
    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

    if ((pNMListView->uChanged & LVIF_STATE) 
        && (pNMListView->uNewState & LVNI_SELECTED))
    {
		DisplaySelectedImage();
    }
	*pResult = 0;
}

/**********************************************************************/

void CSPOSKeyboardImageSelectDlg::OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnOK();
	*pResult = 0;
}

/**********************************************************************/

void CSPOSKeyboardImageSelectDlg::OnKillFocusFilter()
{
	CString strFilter;
	m_editFilter.GetWindowText( strFilter );
	::TrimSpaces( strFilter, FALSE );

	if ( strFilter != m_strFilter )
	{
		m_strFilter = strFilter;
		CreateList();
	}
}

/**********************************************************************/

void CSPOSKeyboardImageSelectDlg::OnButtonRefresh()
{
	CString strCurrentFilename;
	GetSelectedFilename( strCurrentFilename );
	
	m_editFilter.SetWindowText( "" );
	m_strFilter = "";

	SPOSKeyboardManager.GetImageLibrary() -> Refresh( TRUE );
	SPOSKeyboardManager.GetImageLibrary() -> AddFilename( strCurrentFilename );

	CreateList();

	int nIdx;
	if ( ( strCurrentFilename != "" ) && ( SPOSKeyboardManager.GetImageLibrary() -> FindFilename( strCurrentFilename, nIdx ) == TRUE ) )
		SelectLine(nIdx);
	else
		SelectLine(0);
}

/**********************************************************************/

bool CSPOSKeyboardImageSelectDlg::GetSelectedFilename( CString& strFilename )
{
	strFilename = "";
	bool bResult = FALSE;

	int nSel = m_listFiles.GetCurSel();
	if ( ( nSel >= 0 ) && ( nSel < m_arrayImageIdx.GetSize() ) )
	{
		strFilename = SPOSKeyboardManager.GetImageLibrary() -> GetFilename( m_arrayImageIdx.GetAt(nSel) );
		bResult = TRUE;
	}

	return bResult;
}

/**********************************************************************/

void CSPOSKeyboardImageSelectDlg::OnOK()
{
	GetSelectedFilename ( m_strNewSelectedImage );
	EndDialog(IDOK);
}

/**********************************************************************/
