/**********************************************************************/
 
/**********************************************************************/
//#include "EcrmanOptionsIni.h"
/**********************************************************************/
#include "KeyboardNumberDlg.h"
/**********************************************************************/

CKeyboardNumberDlg::CKeyboardNumberDlg( CWnd* pParent )
	: CSSDialog(CKeyboardNumberDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CKeyboardNumberDlg)
	//}}AFX_DATA_INIT
	m_nKbNo = 1;
}

/**********************************************************************/

void CKeyboardNumberDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKeyboardNumberDlg)
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_TABLENO, m_nKbNo);
	DDV_MinMaxInt(pDX, m_nKbNo, 1, EcrmanOptions.GetFeaturesKeyboardsPerLocation());
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CKeyboardNumberDlg, CDialog)
	//{{AFX_MSG_MAP(CKeyboardNumberDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CKeyboardNumberDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	SubclassEdit ( IDC_EDIT_TABLENO, SS_NUM, 2, "%d" );
	return TRUE;  
}

/**********************************************************************/


