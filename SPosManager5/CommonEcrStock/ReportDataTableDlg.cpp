/**********************************************************************/
#include "ReportDataTableDlg.h"
/**********************************************************************/

CReportDataTableDlg::CReportDataTableDlg(CWnd* pParent /*=NULL*/)
	: CSSDialog(CReportDataTableDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CReportDataTableDlg)
	//}}AFX_DATA_INIT
	m_bKeepOpen = FALSE;
}

/**********************************************************************/

void CReportDataTableDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CReportDataTableDlg)
	//}}AFX_DATA_MAP
	DDX_Control( pDX, IDOK, m_buttonOK );
	DDX_Control( pDX, IDCANCEL, m_buttonCancel );
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CReportDataTableDlg, CDialog)
	//{{AFX_MSG_MAP(CReportDataTableDlg)
	ON_WM_MEASUREITEM()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDoubleClickList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CReportDataTableDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	SetWindowText( m_strTitle );

	m_listReports.SubclassDlgItem ( IDC_LIST, this );
	m_listReports.InsertColumn ( 0, "", LVCFMT_LEFT, 200 );
	m_listReports.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	FillListBox();

	m_listReports.SetCurSel(0);
	m_listReports.SetFocus();
	m_listReports.Invalidate();

	HandleInitDialog();
	
	return FALSE;  
}

/**********************************************************************/

void CReportDataTableDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
   if ( nIDCtl == IDC_LIST )	
	{
		m_listReports.AdjustRowHeight( lpMeasureItemStruct, 10 );
	}
}

/**********************************************************************/

void CReportDataTableDlg::OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnOK();
	*pResult = 0;
}

/**********************************************************************/

void CReportDataTableDlg::FillListBox()
{
	m_listReports.DeleteAllItems();

	for ( int n = 0; n < m_arrayReports.GetSize(); n++ )
	{
		m_listReports.InsertItem ( n, "" );
		m_listReports.SetItem ( n, 0, LVIF_TEXT, m_arrayReports.GetAt(n), 0, 0, 0, NULL );
	}
}

/**********************************************************************/

void CReportDataTableDlg::OnOK() 
{
	switch( m_listReports.GetCurSel() )
	{
	case 0:	HandleReport1();	break;
	case 1: HandleReport2();	break;
	case 2: HandleReport3();	break;
	case 3: HandleReport4();	break;
	}

	if ( FALSE == m_bKeepOpen )
	{
		EndDialog ( IDOK );
	}
}

/**********************************************************************/

void CReportDataTableDlg::OnCancel() 
{
	EndDialog ( IDCANCEL );
}

/**********************************************************************/
