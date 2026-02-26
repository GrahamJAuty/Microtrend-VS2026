/**********************************************************************/
#include "StockDeletePrintDlg.h"
/**********************************************************************/

CStockDeletePrintDlg::CStockDeletePrintDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStockDeletePrintDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStockDeletePrintDlg)
	//}}AFX_DATA_INIT
	m_bLines = TRUE;
	m_bMarked = TRUE;
	m_bOnOrder = TRUE;
	m_bApparent = TRUE;
	m_bSales = TRUE;
	m_bValue = TRUE;
	m_bEmpty = TRUE;
}

/**********************************************************************/

void CStockDeletePrintDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStockDeletePrintDlg)
	DDX_Check(pDX,IDC_CHECK_LINES, m_bLines);
	DDX_Check(pDX,IDC_CHECK_MARKED, m_bMarked);
	DDX_Check(pDX,IDC_CHECK_ONORDER, m_bOnOrder);
	DDX_Check(pDX,IDC_CHECK_APPARENT, m_bApparent);
	DDX_Check(pDX,IDC_CHECK_SALES, m_bSales);
	DDX_Check(pDX,IDC_CHECK_VALUE, m_bValue);
	DDX_Check(pDX,IDC_CHECK_EMPTY, m_bEmpty);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CStockDeletePrintDlg, CDialog)
	//{{AFX_MSG_MAP(CStockDeletePrintDlg)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CStockDeletePrintDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	return TRUE;  
}

/**********************************************************************/

void CStockDeletePrintDlg::OnButtonSave()
{
	UpdateData( TRUE );

	CSSFile fileOptions;
	CFilenameUpdater FnUp( SysFiles::StockDeleteDef );
	if ( fileOptions.Open( FnUp.GetFilenameToUse(), "wb" ) == TRUE )
	{
		CCSV csv;
		csv.Add( 1 );
		csv.Add( m_bLines );
		csv.Add( m_bMarked );
		csv.Add( m_bOnOrder );
		csv.Add( m_bApparent );
		csv.Add( m_bSales );
		csv.Add( m_bValue );
		csv.Add( m_bEmpty );
		fileOptions.WriteLine( csv.GetLine() );
	}
}

/**********************************************************************/

void CStockDeletePrintDlg::OnOK() 
{
	UpdateData( TRUE );
	EndDialog( IDOK );
}

/**********************************************************************/


