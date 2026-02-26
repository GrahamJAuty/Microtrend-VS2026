/**********************************************************************/
 
/**********************************************************************/
#include "StockPrefsDlg.h"
/**********************************************************************/

CStockPrefsDlg::CStockPrefsDlg( CWnd* pParent )
	: CDialog(CStockPrefsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStockPrefsDlg)
	//}}AFX_DATA_INIT
	m_nImportMethod = 0;
}

/**********************************************************************/

void CStockPrefsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStockPrefsDlg)
	DDX_Radio(pDX, IDC_RADIO_PREFIMP_ALL, m_nImportMethod);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CStockPrefsDlg, CDialog)
	//{{AFX_MSG_MAP(CStockPrefsDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/
