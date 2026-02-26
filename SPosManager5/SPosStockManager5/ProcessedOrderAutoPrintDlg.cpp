/**********************************************************************/
 
/**********************************************************************/
#include "StockOptionsIni.h"
/**********************************************************************/
#include "ProcessedOrderAutoPrintDlg.h"
/**********************************************************************/

CProcessedOrderAutoPrintDlg::CProcessedOrderAutoPrintDlg( const char* szPreviousOrderFilename, const char* szPreviousOrderReference, CWnd* pParent )
	: CDialog(CProcessedOrderAutoPrintDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProcessedOrderAutoPrintDlg)
	m_bGenerate = TRUE;
	m_bPurchase = TRUE;
	m_bReports = TRUE;
	m_bEmail = FALSE;
	//}}AFX_DATA_INIT
	m_strPreviousOrderFilename = szPreviousOrderFilename;
	m_strReference = szPreviousOrderReference;
	m_nAction = 0;
}

/**********************************************************************/

void CProcessedOrderAutoPrintDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProcessedOrderAutoPrintDlg)
	DDX_Control(pDX, IDC_CHECK_GENERATE, m_checkGenerate);
	DDX_Control(pDX, IDC_CHECK_PURCHASE, m_checkPurchase);
	DDX_Control(pDX, IDC_CHECK_REPORTS, m_checkReports);
	DDX_Control(pDX, IDC_CHECK_EMAIL, m_checkEmail);
	DDX_Check(pDX, IDC_CHECK_GENERATE, m_bGenerate);
	DDX_Check(pDX, IDC_CHECK_PURCHASE, m_bPurchase);
	DDX_Check(pDX, IDC_CHECK_REPORTS, m_bReports);
	DDX_Check(pDX, IDC_CHECK_EMAIL, m_bEmail);
	DDX_Control(pDX, IDC_RADIO_ONE, m_radioOne );
	DDX_Control(pDX, IDC_RADIO_TWO, m_radioTwo );
	DDX_Control(pDX, IDC_RADIO_THREE, m_radioThree );
	DDX_Control(pDX, IDC_EDIT_REFERENCE, m_editReference );
	DDX_Text(pDX, IDC_EDIT_REFERENCE, m_strReference );
	DDV_MaxChars(pDX, m_strReference, 20);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CProcessedOrderAutoPrintDlg, CDialog)
	//{{AFX_MSG_MAP(CProcessedOrderAutoPrintDlg)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CHECK_GENERATE, &CProcessedOrderAutoPrintDlg::OnBnClickedGenerate)
	ON_BN_CLICKED(IDC_RADIO_ONE, &CProcessedOrderAutoPrintDlg::OnBnClickedRadioOne)
	ON_BN_CLICKED(IDC_RADIO_TWO, &CProcessedOrderAutoPrintDlg::OnBnClickedRadioTwo)
	ON_BN_CLICKED(IDC_RADIO_THREE, &CProcessedOrderAutoPrintDlg::OnBnClickedRadioThree)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CProcessedOrderAutoPrintDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	if ( "" == m_strPreviousOrderFilename )
	{
		m_radioOne.SetWindowText( "Remember this order for future re-use" );
		m_radioTwo.SetWindowText( "Do not remember this order for future re-use" );
		m_radioThree.ShowWindow( SW_HIDE );
	}
	else
	{
		m_radioOne.SetWindowText( "Remember this order for future re-use" );
		m_radioTwo.SetWindowText( "Remember this order for future re-use and keep the original saved order" );
		m_radioThree.SetWindowText( "Do not remember this order but keep the original saved order" );
	}

	m_radioOne.SetCheck( TRUE );
	m_radioTwo.SetCheck( FALSE );
	m_radioThree.SetCheck( FALSE );

	if ( StockOptions.GetOrderTemplateFlag() == FALSE )
	{
		m_radioOne.SetCheck( FALSE );
		m_radioTwo.SetCheck( FALSE );
		m_radioThree.SetCheck( FALSE );

		m_radioOne.EnableWindow( FALSE );
		m_radioTwo.EnableWindow( FALSE );
		m_radioThree.EnableWindow( FALSE );
		m_editReference.EnableWindow( FALSE );

		m_checkGenerate.ShowWindow( SW_HIDE );
		m_checkGenerate.EnableWindow( FALSE );
	}

	return TRUE;  
}

/**********************************************************************/

void CProcessedOrderAutoPrintDlg::OnBnClickedGenerate()
{
	bool bEnable = ( m_checkGenerate.GetCheck() != 0 );
	m_checkPurchase.EnableWindow( bEnable );
	m_checkEmail.EnableWindow( bEnable );
	m_checkReports.EnableWindow( bEnable );
}

/**********************************************************************/

void CProcessedOrderAutoPrintDlg::OnBnClickedRadioOne()
{
	m_editReference.EnableWindow( TRUE );
	m_nAction = 0;
}

/**********************************************************************/

void CProcessedOrderAutoPrintDlg::OnBnClickedRadioTwo()
{
	m_editReference.EnableWindow(  m_strPreviousOrderFilename != "" );
	m_nAction = 1;
}

/**********************************************************************/

void CProcessedOrderAutoPrintDlg::OnBnClickedRadioThree()
{
	m_editReference.EnableWindow( FALSE );
	m_nAction = 2;
}

/**********************************************************************/
