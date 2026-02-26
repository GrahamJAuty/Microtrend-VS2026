/**********************************************************************/
 
/**********************************************************************/
#include "ProcessedOrderGenerateInfoDlg.h"
/**********************************************************************/

CProcessedOrderGenerateInfoDlg::CProcessedOrderGenerateInfoDlg( int nMatched, int nReordered, CWnd* pParent)
	: CDialog(CProcessedOrderGenerateInfoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProcessedOrderGenerateInfoDlg)
	//}}AFX_DATA_INIT
	m_nMatched = nMatched;
	m_nReordered = nReordered;
}

/**********************************************************************/

void CProcessedOrderGenerateInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProcessedOrderGenerateInfoDlg)
	DDX_Control(pDX, IDC_STATIC_REORDERED, m_staticReordered);
	DDX_Control(pDX, IDC_STATIC_MATCHED, m_staticMatched);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CProcessedOrderGenerateInfoDlg, CDialog)
	//{{AFX_MSG_MAP(CProcessedOrderGenerateInfoDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CProcessedOrderGenerateInfoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CString strTemp;
	strTemp.Format ( "%d", m_nMatched );
	m_staticMatched.SetWindowText ( strTemp );
	strTemp.Format ( "%d", m_nReordered );
	m_staticReordered.SetWindowText ( strTemp );	
	return TRUE;  
}

/**********************************************************************/
