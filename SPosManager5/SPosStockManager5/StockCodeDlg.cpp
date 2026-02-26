/**********************************************************************/
 
/**********************************************************************/
#include "BarcodePluNoTable.h"
#include "StockOptionsIni.h"
/**********************************************************************/
#include "StockCodeDlg.h"
/**********************************************************************/

CStockCodeDlg::CStockCodeDlg( const char* szTitle, CStockCodeSearch& StockCodeSearch, CWnd* pParent )
	: CDialog(CStockCodeDlg::IDD, pParent), m_StockCodeSearch ( StockCodeSearch )
{
	//{{AFX_DATA_INIT(CStockCodeDlg)
	//}}AFX_DATA_INIT
	m_strTitle = szTitle;
	m_bSearchFromStart = m_StockCodeSearch.m_bSearchFromStart;
	m_strStockCodeEntered = m_StockCodeSearch.m_strStockCodeEntered;
	m_strStockCodeToUse = "";
}

/**********************************************************************/

void CStockCodeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStockCodeDlg)
	DDX_Control(pDX, IDC_CHECK_SEARCH_START, m_checkSearchFromStart);
	//}}AFX_DATA_MAP
	DDX_Check(pDX, IDC_CHECK_SEARCH_START, m_bSearchFromStart);
	DDX_Text(pDX, IDC_EDIT_STOCK_CODE, m_strStockCodeEntered);
	DDV_MaxChars(pDX, m_strStockCodeEntered, Stock::StockCode.Max);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CStockCodeDlg, CDialog)
	//{{AFX_MSG_MAP(CStockCodeDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CStockCodeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	SetWindowText ( m_strTitle );	

	if ( m_StockCodeSearch.m_bSearchFromStartTick == FALSE ) 
		m_checkSearchFromStart.ShowWindow ( SW_HIDE );
	
	return TRUE;  
}

/**********************************************************************/

void CStockCodeDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		if ( m_strStockCodeEntered == "" )
			EndDialog ( IDCANCEL );
		else
		{
			m_strStockCodeToUse = m_strStockCodeEntered;
			m_strStockCodeToUse = BarcodePluNoTable.ConvertStockCode ( m_strStockCodeToUse );
			m_StockCodeSearch.m_strStockCodeEntered = m_strStockCodeEntered;
			m_StockCodeSearch.m_strStockCodeToUse = m_strStockCodeToUse;
			m_StockCodeSearch.m_bSearchFromStart = ( m_bSearchFromStart != 0 );
			EndDialog ( IDOK );
		}
	}
}

/**********************************************************************/

