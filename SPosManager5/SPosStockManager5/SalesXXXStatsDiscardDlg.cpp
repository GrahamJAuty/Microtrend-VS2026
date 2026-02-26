/**********************************************************************/
 
/**********************************************************************/
#include "SalesXXXStatsDiscardDlg.h"
/**********************************************************************/

CSalesXXXStatsDiscardDlg::CSalesXXXStatsDiscardDlg(CWnd* pParent)
	: CDialog(CSalesXXXStatsDiscardDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSalesXXXStatsDiscardDlg)
	//}}AFX_DATA_INIT
}

/**********************************************************************/

void CSalesXXXStatsDiscardDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSalesXXXStatsDiscardDlg)
	DDX_Control(pDX, IDC_STATIC_NUMBER2, m_staticNumber2);
	DDX_Control(pDX, IDC_STATIC_NUMBER1, m_staticNumber1);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CSalesXXXStatsDiscardDlg, CDialog)
	//{{AFX_MSG_MAP(CSalesXXXStatsDiscardDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CSalesXXXStatsDiscardDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CString strNumber;
	
	strNumber.Format( "%d", m_nNumber1 );
	m_staticNumber1.SetWindowText( strNumber );

	strNumber.Format( "%d", m_nNumber2 );
	m_staticNumber2.SetWindowText( strNumber );

	return TRUE;  
}

/**********************************************************************/
