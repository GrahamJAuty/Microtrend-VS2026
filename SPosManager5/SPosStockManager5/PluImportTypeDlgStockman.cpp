/**********************************************************************/
 
/**********************************************************************/
#include "PluImportTypeDlgStockman.h"
/**********************************************************************/

CPluImportTypeDlgStockman::CPluImportTypeDlgStockman( CWnd* pParent )
	: CDialog(CPluImportTypeDlgStockman::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPluImportTypeDlgStockman)
	m_bCreateStock = FALSE;
	//}}AFX_DATA_INIT
	m_nImportMethod = 0;
}

/**********************************************************************/

void CPluImportTypeDlgStockman::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPluImportTypeDlgStockman)
	DDX_Check(pDX, IDC_CHECK_CREATESTOCK, m_bCreateStock);
	DDX_Radio(pDX, IDC_RADIO_PREFIMP_ALL, m_nImportMethod);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPluImportTypeDlgStockman, CDialog)
	//{{AFX_MSG_MAP(CPluImportTypeDlgStockman)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/
