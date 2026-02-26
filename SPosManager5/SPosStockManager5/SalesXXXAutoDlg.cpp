/**********************************************************************/
 
/**********************************************************************/
//#include "EcrmanOptionsIni.h"
#include "SalesXXXDetector.h"
/**********************************************************************/
#include "SalesXXXAutoDlg.h"
/**********************************************************************/

CSalesXXXAutoDlg::CSalesXXXAutoDlg(CWnd* pParent)
	: CDialog(CSalesXXXAutoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSalesXXXAutoDlg)
	//}}AFX_DATA_INIT
	m_bAgain = ( SalesXXXDetector.GetDelayType() != 0 );
}

/**********************************************************************/

void CSalesXXXAutoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSalesXXXAutoDlg)
	DDX_Control(pDX, IDC_COMBO_WAIT, m_comboWait);
	DDX_Check(pDX, IDC_CHECK_AGAIN, m_bAgain);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CSalesXXXAutoDlg, CDialog)
	//{{AFX_MSG_MAP(CSalesXXXAutoDlg)
	ON_BN_CLICKED(IDC_CHECK_AGAIN, OnToggleAgain)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CSalesXXXAutoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_comboWait.AddString( "10 minutes" );
	m_comboWait.AddString( "20 minutes" );
	m_comboWait.AddString( "30 minutes" );
	m_comboWait.AddString( "1 hour" );
	m_comboWait.AddString( "2 hours" );
	m_comboWait.AddString( "3 hours" );
	
	int nSel = SalesXXXDetector.GetDelayType();
	
	if ( nSel >= 1 && nSel <= m_comboWait.GetCount() )
		m_comboWait.SetCurSel( nSel - 1 );
	else
		m_comboWait.SetCurSel( 0 );
	
	m_comboWait.EnableWindow( m_bAgain != 0 );

	return TRUE;  
}

/**********************************************************************/

void CSalesXXXAutoDlg::OnToggleAgain() 
{
	if ( UpdateData( TRUE ) == TRUE )
		m_comboWait.EnableWindow( m_bAgain != 0 );
}

/**********************************************************************/

void CSalesXXXAutoDlg::OnOK() 
{
	if ( UpdateData( TRUE ) == TRUE )
	{
		if ( m_bAgain != 0 )
			SalesXXXDetector.SetDelayType( m_comboWait.GetCurSel() + 1 );
		else
			SalesXXXDetector.SetDelayType( SALESXXX_DELAY_FOREVER );
	
		EndDialog( IDOK );
	}
}

/**********************************************************************/

void CSalesXXXAutoDlg::OnCancel()
{
	OnOK();
}

/**********************************************************************/

