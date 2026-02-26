/**********************************************************************/
 
/**********************************************************************/
#include "SelectReportSessionTypeDlg.h"
/**********************************************************************/

CSelectReportSessionTypeDlg::CSelectReportSessionTypeDlg( CWnd* pParent /*=NULL*/)
	: CSSDialog(CSelectReportSessionTypeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(SelectReportSessionTypeDlg)
	//}}AFX_DATA_INIT
}

/**********************************************************************/

void CSelectReportSessionTypeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelectPresetActionDlg)
	DDX_Control( pDX, IDOK, m_buttonOK );
	DDX_Control( pDX, IDCANCEL, m_buttonCancel );
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CSelectReportSessionTypeDlg, CDialog)
	//{{AFX_MSG_MAP(SelectReportSessionTypeDlg)
	ON_WM_MEASUREITEM()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDoubleClickList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CSelectReportSessionTypeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_listActions.SubclassDlgItem ( IDC_LIST, this );
	m_listActions.InsertColumn ( 0, "", LVCFMT_LEFT, 200 );
	m_listActions.ModifyStyle( LVS_OWNERDRAWFIXED, 0, 0 );
	m_listActions.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	FillListBox();

	m_listActions.SetCurSel(0);
	m_listActions.SetFocus();
	m_listActions.Invalidate();

	SetWindowText( "Select Report Session Type" );

	MoveControl( &m_buttonOK, 188,53 );
	MoveControl( &m_buttonCancel, 188,69 );
	ResizeControl( &m_listActions, 158, 76 );
	ResizeDialog( 245, 90 );

	return FALSE;  
}

/**********************************************************************/

void CSelectReportSessionTypeDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	if ( nIDCtl == IDC_LIST )	
	{
		m_listActions.AdjustRowHeight( lpMeasureItemStruct, 10 );
	}
}

/**********************************************************************/

void CSelectReportSessionTypeDlg::OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnOK();
	*pResult = 0;
}

/**********************************************************************/

void CSelectReportSessionTypeDlg::FillListBox()
{
	m_listActions.DeleteAllItems();

	CStringArray arrayItems;
	arrayItems.Add( "Normal Report Session" );
	arrayItems.Add( "Combined Report Session" );

	for ( int n = 0; n < arrayItems.GetSize(); n++ )
	{
		m_listActions.InsertItem ( n, "" );
		m_listActions.SetItem ( n, 0, LVIF_TEXT, arrayItems.GetAt(n), 0, 0, 0, NULL );
	}
}

/**********************************************************************/

void CSelectReportSessionTypeDlg::OnOK() 
{
	m_nAction = m_listActions.GetCurSel();

	if ( ( m_nAction < 0 ) || ( m_nAction > 1 ) )
		m_nAction = 0;

	EndDialog ( IDOK );
}

/**********************************************************************/

void CSelectReportSessionTypeDlg::OnCancel() 
{
	EndDialog ( IDCANCEL );
}

/**********************************************************************/
