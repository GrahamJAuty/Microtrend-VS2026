/**********************************************************************/
 
/**********************************************************************/
//#include "EcrmanOptionsIni.h"
/**********************************************************************/
#include "SalesXXXStatsDlg.h"
/**********************************************************************/

CSalesXXXStatsDlg::CSalesXXXStatsDlg(CWnd* pParent)
	: CDialog(CSalesXXXStatsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSalesXXXStatsDlg)
	//}}AFX_DATA_INIT
}

/**********************************************************************/

void CSalesXXXStatsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSalesXXXStatsDlg)
	DDX_Control(pDX, IDC_STATIC_NUMBER7, m_staticNumber7);
	DDX_Control(pDX, IDC_STATIC_NUMBER6, m_staticNumber6);
	DDX_Control(pDX, IDC_STATIC_NUMBER5, m_staticNumber5);
	DDX_Control(pDX, IDC_STATIC_NUMBER4, m_staticNumber4);
	DDX_Control(pDX, IDC_STATIC_NUMBER3, m_staticNumber3);
	DDX_Control(pDX, IDC_STATIC_NUMBER2, m_staticNumber2);
	DDX_Control(pDX, IDC_STATIC_NUMBER1, m_staticNumber1);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CSalesXXXStatsDlg, CDialog)
	//{{AFX_MSG_MAP(CSalesXXXStatsDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CSalesXXXStatsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CString strNumber;
	
	strNumber.Format( "%d", m_nNumber1 );
	m_staticNumber1.SetWindowText( strNumber );

	strNumber.Format( "%d", m_nNumber2 );
	m_staticNumber2.SetWindowText( strNumber );

	strNumber.Format( "%d", m_nNumber3 );
	m_staticNumber3.SetWindowText( strNumber );

	strNumber.Format( "%d", m_nNumber4 );
	m_staticNumber4.SetWindowText( strNumber );

	strNumber.Format( "%d", m_nNumber5 );
	m_staticNumber5.SetWindowText( strNumber );

	strNumber.Format( "%d", m_nNumber6 );
	m_staticNumber6.SetWindowText( strNumber );

	strNumber.Format( "%d", m_nNumber7 );
	m_staticNumber7.SetWindowText( strNumber );

	return TRUE;  
}

/**********************************************************************/
