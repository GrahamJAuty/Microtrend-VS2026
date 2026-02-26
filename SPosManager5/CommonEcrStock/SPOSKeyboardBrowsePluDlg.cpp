/**********************************************************************/
#include "SPOSKeyboardBrowsePluDlg.h"
/**********************************************************************/

CSPOSKeyboardBrowsePluDlg::CSPOSKeyboardBrowsePluDlg( CSPOSKeyboardPluFilterArray& PluFilterArray, CWnd* pParent) : CSSDialog(CSPOSKeyboardBrowsePluDlg::IDD, pParent), m_PluFilterArray( PluFilterArray )
{
	//{{AFX_DATA_INIT(CSPOSKeyboardBrowsePluDlg)
	//}}AFX_DATA_INIT
	m_nPluNo = 0;
	m_strFilter = m_PluFilterArray.GetDescription();
	m_bShowModifiers = m_PluFilterArray.GetShowModifiersFlag();
}

/**********************************************************************/

void CSPOSKeyboardBrowsePluDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSPOSKeyboardBrowsePluDlg)
	DDX_Text(pDX, IDC_EDIT_FILTER, m_strFilter);
	DDV_MaxChars(pDX, m_strFilter, 30); //CHECKED 28-07-03
	DDX_Check(pDX, IDC_CHECK_MODIFIERS, m_bShowModifiers); 
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CSPOSKeyboardBrowsePluDlg, CDialog)
	//{{AFX_MSG_MAP(CSPOSKeyboardBrowsePluDlg)
	ON_EN_KILLFOCUS(IDC_EDIT_FILTER, OnKillFocusFilter)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDoubleClickList)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST, OnGetDispInfoList)
	ON_BN_CLICKED(IDC_CHECK_MODIFIERS, OnToggleShowModifiers)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CSPOSKeyboardBrowsePluDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_listPlu.SubclassDlgItem( IDC_LIST, this );
	m_listPlu.InsertColumn ( 0, "Plu No", LVCFMT_LEFT, 100 );
	m_listPlu.InsertColumn ( 1, "Ecr Text", LVCFMT_LEFT, 160 );
	m_listPlu.InsertColumn ( 2, "Rep Text", LVCFMT_LEFT, 160 );

	int nSize = m_PluFilterArray.GetSize(); 
	m_listPlu.SetItemCountEx( nSize );
	m_listPlu.SetCurSel( ( nSize > 0 ) ? 0 : -1 );

	return TRUE;  
}

/**********************************************************************/

void CSPOSKeyboardBrowsePluDlg::OnKillFocusFilter() 
{
	CString strOldFilter = m_strFilter;
	
	UpdateData( TRUE, FALSE );
	::TrimSpaces( m_strFilter, FALSE );
	UpdateData( FALSE );
	
	if ( m_strFilter != strOldFilter )
	{
		m_PluFilterArray.SetDescriptionFilter( m_strFilter );
		m_PluFilterArray.BuildRecordList();

		int nSize = m_PluFilterArray.GetSize();
		m_listPlu.SetItemCountEx( nSize );
		m_listPlu.SetCurSel( ( nSize > 0 ) ? 0 : -1 );
		m_listPlu.Invalidate();	
	}
}

/**********************************************************************/

void CSPOSKeyboardBrowsePluDlg::OnToggleShowModifiers() 
{
	UpdateData( TRUE, FALSE );
	m_PluFilterArray.SetShowModifiersFlag( m_bShowModifiers != 0 );
	m_PluFilterArray.BuildRecordList();

	int nSize = m_PluFilterArray.GetSize();
	m_listPlu.SetItemCountEx( nSize );
	m_listPlu.SetCurSel( ( nSize > 0 ) ? 0 : -1 );
	m_listPlu.Invalidate();	
}

/**********************************************************************/

void CSPOSKeyboardBrowsePluDlg::OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;
	
	int nSel = m_listPlu.GetCurSel();
	if ( nSel < 0 || nSel >= m_PluFilterArray.GetSize() )
		return;

	CSPOSKeyboardImportPluCSVRecord PluRecord;
	SPOSKeyboardManager.GetPluDatabase() -> GetAt( m_PluFilterArray.GetRecordIndexAt( nSel ), PluRecord );
	m_nPluNo = PluRecord.GetPluNumber();
	
	EndDialog ( IDOK );
}

/**********************************************************************/

void CSPOSKeyboardBrowsePluDlg::OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	if (( pDispInfo->item.mask & LVIF_TEXT ) != 0 )
	{
		int nIndex = pDispInfo->item.iItem;
		CCSV csv ( m_PluFilterArray.GetDisplayLine ( nIndex ), '\t' );
		lstrcpy ( pDispInfo->item.pszText, csv.GetString ( pDispInfo->item.iSubItem ) );
	}
	*pResult = 0;
}

/**********************************************************************/

void CSPOSKeyboardBrowsePluDlg::OnOK() 
{
	int nSel = m_listPlu.GetCurSel();
	if ( nSel < 0 || nSel >= m_PluFilterArray.GetSize() )	
	{
		EndDialog ( IDCANCEL );
		return;
	}

	CSPOSKeyboardImportPluCSVRecord PluRecord;
	SPOSKeyboardManager.GetPluDatabase() -> GetAt( m_PluFilterArray.GetRecordIndexAt( nSel ), PluRecord );
	m_nPluNo = PluRecord.GetPluNumber();
	EndDialog ( IDOK );
}

/**********************************************************************/





