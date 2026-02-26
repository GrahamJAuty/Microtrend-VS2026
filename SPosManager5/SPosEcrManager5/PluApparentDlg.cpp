/**********************************************************************/
 
/**********************************************************************/
#include "SysInfo.h"
/**********************************************************************/
#include "PluApparentDlg.h"
/**********************************************************************/

CPluApparentDlg::CPluApparentDlg(CWnd* pParent)
	: CSSDialog(CPluApparentDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPluApparentDlg)
	m_dApparent = 0.0;
	//}}AFX_DATA_INIT
}

/**********************************************************************/

void CPluApparentDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPluApparentDlg)
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_APPARENT, m_dApparent );
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPluApparentDlg, CDialog)
	//{{AFX_MSG_MAP(CPluApparentDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPluApparentDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	SubclassEdit( IDC_EDIT_APPARENT, SS_NUM_SDP, 8, "%.*f", SysInfo.GetDPQty() ); 
	return TRUE;  
}

/***********************************************************************/

