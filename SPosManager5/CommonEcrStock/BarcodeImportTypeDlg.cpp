/**********************************************************************/
 
/**********************************************************************/
#include "BarcodeImportTypeDlg.h"
/**********************************************************************/

CBarcodeImportTypeDlg::CBarcodeImportTypeDlg( CWnd* pParent )
	: CDialog(CBarcodeImportTypeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBarcodeImportTypeDlg)
	//}}AFX_DATA_INIT
	m_nImportMethod = 0;
}

/**********************************************************************/

void CBarcodeImportTypeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBarcodeImportTypeDlg)
	DDX_Radio(pDX, IDC_RADIO_PREFIMP_ALL, m_nImportMethod);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CBarcodeImportTypeDlg, CDialog)
	//{{AFX_MSG_MAP(CBarcodeImportTypeDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/
