/**********************************************************************/
 
/**********************************************************************/
//#include "DataManager.h"
//#include "EcrmanOptionsIni.h"
#include "SysInfo.h"
/**********************************************************************/
#include "SalesHistoryPriceLevelDlg.h"
/**********************************************************************/

CSalesHistoryPriceLevelDlg::CSalesHistoryPriceLevelDlg(CWnd* pParent)
	: CDialog(CSalesHistoryPriceLevelDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSalesHistoryPriceLevelDlg)
	//}}AFX_DATA_INIT
}

/**********************************************************************/

void CSalesHistoryPriceLevelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSalesHistoryPriceLevelDlg)
	DDX_Control(pDX, IDC_CHECK_PRICELEVEL1, m_checkPriceLevel1);
	DDX_Control(pDX, IDC_CHECK_PRICELEVEL2, m_checkPriceLevel2);
	DDX_Control(pDX, IDC_CHECK_PRICELEVEL3, m_checkPriceLevel3);
	DDX_Control(pDX, IDC_CHECK_PRICELEVEL4, m_checkPriceLevel4);
	DDX_Control(pDX, IDC_CHECK_PRICELEVEL5, m_checkPriceLevel5);
	DDX_Control(pDX, IDC_CHECK_PRICELEVEL6, m_checkPriceLevel6);
	DDX_Control(pDX, IDC_CHECK_PRICELEVEL7, m_checkPriceLevel7);
	DDX_Control(pDX, IDC_CHECK_PRICELEVEL8, m_checkPriceLevel8);
	DDX_Control(pDX, IDC_CHECK_PRICELEVEL9, m_checkPriceLevel9);
	DDX_Control(pDX, IDC_CHECK_PRICELEVEL10, m_checkPriceLevel10);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CSalesHistoryPriceLevelDlg, CDialog)
	//{{AFX_MSG_MAP(CSalesHistoryPriceLevelDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CSalesHistoryPriceLevelDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();	

	int nMask = SysInfo.GetCustomPriceLevelFilter();
	m_checkPriceLevel1.SetCheck( ( nMask & 1 ) != 0 );
	m_checkPriceLevel2.SetCheck( ( nMask & 2 ) != 0 );
	m_checkPriceLevel3.SetCheck( ( nMask & 4 ) != 0 );
	m_checkPriceLevel4.SetCheck( ( nMask & 8 ) != 0 );
	m_checkPriceLevel5.SetCheck( ( nMask & 16 ) != 0 );
	m_checkPriceLevel6.SetCheck( ( nMask & 32 ) != 0 );
	m_checkPriceLevel7.SetCheck( ( nMask & 64 ) != 0 );
	m_checkPriceLevel8.SetCheck( ( nMask & 128 ) != 0 );
	m_checkPriceLevel9.SetCheck( ( nMask & 256 ) != 0 );
	m_checkPriceLevel10.SetCheck( ( nMask & 512 ) != 0 );
	
	return TRUE;  
}

/**********************************************************************/

void CSalesHistoryPriceLevelDlg::OnOK() 
{
	int nMask = 0;
	if ( m_checkPriceLevel1.GetCheck() != 0 )	nMask += 1;
	if ( m_checkPriceLevel2.GetCheck() != 0 )	nMask += 2;
	if ( m_checkPriceLevel3.GetCheck() != 0 )	nMask += 4;
	if ( m_checkPriceLevel4.GetCheck() != 0 )	nMask += 8;
	if ( m_checkPriceLevel5.GetCheck() != 0 )	nMask += 16;
	if ( m_checkPriceLevel6.GetCheck() != 0 )	nMask += 32;
	if ( m_checkPriceLevel7.GetCheck() != 0 )	nMask += 64;
	if ( m_checkPriceLevel8.GetCheck() != 0 )	nMask += 128;
	if ( m_checkPriceLevel9.GetCheck() != 0 )	nMask += 256;
	if ( m_checkPriceLevel10.GetCheck() != 0 )	nMask += 512;
	SysInfo.SetCustomPriceLevelFilter( nMask );

	EndDialog ( IDOK );
}

/**********************************************************************/
