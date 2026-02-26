/**********************************************************************/
 
/**********************************************************************/
#include "DefaultCategoryDlg.h"
/**********************************************************************/

CDefaultCategoryDlg::CDefaultCategoryDlg(CWnd* pParent)
	: CDialog(CDefaultCategoryDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDefaultCategoryDlg)
	//}}AFX_DATA_INIT
	m_nDefType = 0;
}

/**********************************************************************/

void CDefaultCategoryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDefaultCategoryDlg)
	DDX_Radio(pDX, IDC_RADIO_DEFTYPE, m_nDefType);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CDefaultCategoryDlg, CDialog)
	//{{AFX_MSG_MAP(CDefaultCategoryDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

