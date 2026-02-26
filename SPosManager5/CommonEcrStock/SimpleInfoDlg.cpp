/**********************************************************************/
 
/**********************************************************************/
#include "SimpleInfoDlg.h"
/**********************************************************************/

CSimpleInfoDlg::CSimpleInfoDlg( const char* szTitle, const char* szLabel1, const char* szLabel2, int nNumber1, int nNumber2, CWnd* pParent /*=NULL*/)
	: CDialog(CSimpleInfoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSimpleInfoDlg)
	//}}AFX_DATA_INIT
	m_nNumber1 = nNumber1;
	m_nNumber2 = nNumber2;
	m_strLabel1 = szLabel1;
	m_strLabel2 = szLabel2;
	m_strTitle = szTitle;
}

/**********************************************************************/

void CSimpleInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSimpleInfoDlg)
	DDX_Control(pDX, IDC_STATIC_LABEL1, m_staticLabel1);
	DDX_Control(pDX, IDC_STATIC_LABEL2, m_staticLabel2);
	DDX_Control(pDX, IDC_STATIC_NUMBER1, m_staticNumber1);
	DDX_Control(pDX, IDC_STATIC_NUMBER2, m_staticNumber2);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CSimpleInfoDlg, CDialog)
	//{{AFX_MSG_MAP(CSimpleInfoDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CSimpleInfoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();	
	SetWindowText ( m_strTitle );
	m_staticLabel1.SetWindowText ( m_strLabel1 );
	m_staticLabel2.SetWindowText ( m_strLabel2 );
	CString strNum;
	strNum.Format ( "%d", m_nNumber1 );
	m_staticNumber1.SetWindowText ( strNum );
	strNum.Format ( "%d", m_nNumber2 );
	m_staticNumber2.SetWindowText ( strNum );
	return TRUE;  
}

/**********************************************************************/
