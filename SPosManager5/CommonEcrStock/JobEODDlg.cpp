/**********************************************************************/
 
/**********************************************************************/
 
/**********************************************************************/
//#include "EcrmanOptionsIni.h"
/**********************************************************************/
#include "JobEODDlg.h"
/**********************************************************************/

CJobEODDlg::CJobEODDlg( CWnd* pParent )
	: CDialog(CJobEODDlg::IDD, pParent )
{
	//{{AFX_DATA_INIT(CJobEODDlg)
	//}}AFX_DATA_INIT
	m_strDate = "";
	m_nType = 0;
	m_bReason = FALSE;
	m_bPlu = FALSE;
	m_bCustomer = FALSE;
}

/**********************************************************************/

void CJobEODDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CJobEODDlg)
	DDX_Control(pDX, IDC_STATIC_REASON, m_staticReason);
	DDX_Control(pDX, IDC_COMBO_DATES, m_comboDates);
	DDX_Control(pDX, IDC_CHECK_REASON, m_checkReason);
	DDX_Control(pDX, IDC_CHECK_PLU, m_checkPlu);
	DDX_Control(pDX, IDC_CHECK_CUSTOMER, m_checkCustomer);
	DDX_Check(pDX, IDC_CHECK_REASON, m_bReason);
	DDX_Check(pDX, IDC_CHECK_PLU, m_bPlu);
	DDX_Check(pDX, IDC_CHECK_CUSTOMER, m_bCustomer);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CJobEODDlg, CDialog)
	//{{AFX_MSG_MAP(CJobEODDlg)
	ON_CBN_SELCHANGE(IDC_COMBO_DATES, OnSelectDate)
	ON_BN_CLICKED(IDC_CHECK_REASON, OnCheckReason)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CJobEODDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_comboDates.AddString ( "Today" );
	m_comboDates.AddString ( "Yesterday" );
	m_comboDates.AddString ( "2 days ago" );
	m_comboDates.AddString ( "3 days ago" );
	m_comboDates.AddString ( "4 days ago" );
	m_comboDates.AddString ( "5 days ago" );
	m_comboDates.AddString ( "6 days ago" );
	m_comboDates.AddString ( "7 days ago" );
	m_comboDates.AddString ( "Check all dates" );
	
	m_comboDates.SetCurSel(0);
	m_comboDates.EnableWindow( FALSE );

	SetDates();

	if ( EcrmanOptions.GetReportsEODReasonsFlag() == FALSE )
	{
		m_staticReason.ShowWindow( SW_HIDE );
		m_comboDates.ShowWindow( SW_HIDE );
		m_checkReason.ShowWindow( SW_HIDE );
		m_comboDates.EnableWindow( FALSE );
		m_checkReason.EnableWindow( FALSE );
	}

	if ( EcrmanOptions.GetPluProdCSVImportType() == 0 )
	{
		m_checkPlu.SetCheck( FALSE );
		m_checkPlu.ShowWindow( SW_HIDE );
		m_checkPlu.EnableWindow( FALSE );
	}

	if ( EcrmanOptions.GetFeaturesCustomerNameImportType() == 0 )
	{
		m_checkCustomer.SetCheck( FALSE );
		m_checkCustomer.ShowWindow( SW_HIDE );
		m_checkCustomer.EnableWindow( FALSE );
	}

	return TRUE;  
}

/**********************************************************************/

void CJobEODDlg::OnSelectDate() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		SetDates();
		UpdateData ( FALSE );
	}
}

/**********************************************************************/

void CJobEODDlg::SetDates()
{
}

/**********************************************************************/

void CJobEODDlg::OnCheckReason()
{
	m_comboDates.EnableWindow( m_checkReason.GetCheck() != 0 );
}

/**********************************************************************/

void CJobEODDlg::OnOK() 
{
	if ( UpdateData( TRUE ) == TRUE )
	{
		m_nType = m_comboDates.GetCurSel();
		EndDialog( IDOK );
	}
}

/**********************************************************************/

