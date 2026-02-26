/**********************************************************************/
#include "StockImportFilterPrintDlg.h"
/**********************************************************************/

CStockImportFilterPrintDlg::CStockImportFilterPrintDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStockImportFilterPrintDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStockImportFilterPrintDlg)
	//}}AFX_DATA_INIT
	m_bLines = TRUE;
	m_bMarked = TRUE;
}

/**********************************************************************/

void CStockImportFilterPrintDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStockImportFilterPrintDlg)
	DDX_Check(pDX,IDC_CHECK_LINES, m_bLines);
	DDX_Check(pDX,IDC_CHECK_MARKED, m_bMarked);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CStockImportFilterPrintDlg, CDialog)
	//{{AFX_MSG_MAP(CStockImportFilterPrintDlg)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CStockImportFilterPrintDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	return TRUE;  
}

/**********************************************************************/

void CStockImportFilterPrintDlg::OnButtonSave()
{
	UpdateData( TRUE );

	CSSFile fileOptions;
	CFilenameUpdater FnUp( SysFiles::StockImportDef );
	if ( fileOptions.Open( FnUp.GetFilenameToUse(), "wb" ) == TRUE )
	{
		CCSV csv;
		csv.Add( 1 );
		csv.Add( m_bLines );
		csv.Add( m_bMarked );
		fileOptions.WriteLine( csv.GetLine() );
	}
}

/**********************************************************************/

void CStockImportFilterPrintDlg::OnOK() 
{
	UpdateData( TRUE );
	EndDialog( IDOK );
}

/**********************************************************************/


