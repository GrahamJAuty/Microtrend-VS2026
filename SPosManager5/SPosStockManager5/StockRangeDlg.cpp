/**********************************************************************/
 
/**********************************************************************/
#include "BarcodePluNoTable.h"
 
#include "StockRangeDlg.h"
/**********************************************************************/

CStockRangeDlg::CStockRangeDlg( const char* szTitle, CWnd* pParent )
	: CSSDialog(CStockRangeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStockRangeDlg)
	//}}AFX_DATA_INIT
	m_strTitle = szTitle;
}

/**********************************************************************/

void CStockRangeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStockRangeDlg)
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_STOCKCODE_FROM, m_strStockCodeFrom);
	DDV_MaxChars(pDX, m_strStockCodeFrom, Stock::StockCode.Max );
	DDX_Text(pDX, IDC_EDIT_STOCKCODE_TO, m_strStockCodeTo);
	DDV_MaxChars(pDX, m_strStockCodeTo, Stock::StockCode.Max );
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CStockRangeDlg, CDialog)
	//{{AFX_MSG_MAP(CStockRangeDlg)
	ON_EN_KILLFOCUS(IDC_EDIT_STOCKCODE_FROM, OnKillFocusStockCode)
	ON_EN_KILLFOCUS(IDC_EDIT_STOCKCODE_TO, OnKillFocusStockCode)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CStockRangeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	SetWindowText ( m_strTitle );	
	return TRUE;  
}

/**********************************************************************/

void CStockRangeDlg::OnKillFocusStockCode() 
{
	UpdateData( TRUE, FALSE );
	m_strStockCodeFrom = BarcodePluNoTable.ConvertStockCode ( m_strStockCodeFrom );
	m_strStockCodeTo = BarcodePluNoTable.ConvertStockCode ( m_strStockCodeTo );
	UpdateData ( FALSE );
}

/**********************************************************************/

void CStockRangeDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		OnKillFocusStockCode();

		if ( m_strStockCodeFrom == "" || m_strStockCodeTo == "" )
		{
			Prompter.Error ( "Please enter values for both\nends of the range" );
			return;
		}

		EndDialog ( IDOK );
	}
}

/**********************************************************************/

