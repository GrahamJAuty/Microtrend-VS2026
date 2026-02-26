/**********************************************************************/
 
/**********************************************************************/
#include "StockLevelsAccessDlg.h"
/**********************************************************************/

IMPLEMENT_DYNAMIC(CStockLevelsAccessDlg, CDialog)

CStockLevelsAccessDlg::CStockLevelsAccessDlg( bool bStock, CWnd* pParent /*=NULL*/)
	: CDialog(CStockLevelsAccessDlg::IDD, pParent)
	, m_nType(0)
{
	m_bStock = bStock;
}

/**********************************************************************/

CStockLevelsAccessDlg::~CStockLevelsAccessDlg()
{
}

/**********************************************************************/

void CStockLevelsAccessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPageDownloadPlu)
	DDX_Radio(pDX, IDC_RADIO1, m_nType);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_RADIO_READWRITE, m_radioReadWrite);
}

/**********************************************************************/
BEGIN_MESSAGE_MAP(CStockLevelsAccessDlg, CDialog)
END_MESSAGE_MAP()
/**********************************************************************/

BOOL CStockLevelsAccessDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	if ( m_bStock == TRUE )
	{
		SetWindowText( "Open Stock Database" );
		m_radioReadWrite.SetWindowText( "Full access to stock levels (delete stock items)" );
	}
	else
	{
		SetWindowText( "Open Plu Database" );
		m_radioReadWrite.SetWindowText( "Full access to stock levels (edit stock levels)" );
	}
	return TRUE;  
}

/**********************************************************************/

void CStockLevelsAccessDlg::OnOK()
{
	if ( UpdateData( TRUE ) == TRUE )
	{
		m_nType = 2 - m_nType;
		EndDialog( IDOK );
	}
}

/**********************************************************************/
