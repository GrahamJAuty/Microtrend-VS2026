/**********************************************************************/
 
/**********************************************************************/
//#include "DataManager.h"
//#include "EcrmanOptionsIni.h"
#include "StockReportNames.h"
#include "SysInfo.h"
/**********************************************************************/
#include "StockReportAllowanceDlg.h"
/**********************************************************************/

CStockReportAllowanceDlg::CStockReportAllowanceDlg(CWnd* pParent /*=NULL*/)
	: CSSDialog(CStockReportAllowanceDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStockReportAllowanceDlg)
	//}}AFX_DATA_INIT
	m_nAllowanceID[0] = IDC_EDIT_ALLOWANCE1;
	m_nAllowanceID[1] = IDC_EDIT_ALLOWANCE2;
	m_nAllowanceID[2] = IDC_EDIT_ALLOWANCE3;
	m_nAllowanceID[3] = IDC_EDIT_ALLOWANCE4;
	m_nAllowanceID[4] = IDC_EDIT_ALLOWANCE5;
	m_nAllowanceID[5] = IDC_EDIT_ALLOWANCE6;
	m_nAllowanceID[6] = IDC_EDIT_ALLOWANCE7;
	m_nAllowanceID[7] = IDC_EDIT_ALLOWANCE8;
	m_nAllowanceID[8] = IDC_EDIT_ALLOWANCE9;
	m_nAllowanceNameID[0] = IDC_STATIC_ALLOWANCE1;
	m_nAllowanceNameID[1] = IDC_STATIC_ALLOWANCE2;
	m_nAllowanceNameID[2] = IDC_STATIC_ALLOWANCE3;
	m_nAllowanceNameID[3] = IDC_STATIC_ALLOWANCE4;
	m_nAllowanceNameID[4] = IDC_STATIC_ALLOWANCE5;
	m_nAllowanceNameID[5] = IDC_STATIC_ALLOWANCE6;
	m_nAllowanceNameID[6] = IDC_STATIC_ALLOWANCE7;
	m_nAllowanceNameID[7] = IDC_STATIC_ALLOWANCE8;
	m_nAllowanceNameID[8] = IDC_STATIC_ALLOWANCE9;

	for ( int n = 0; n < 9; n++ )
	{
		m_strAllowanceName[n] = DataManager.Allowance.GetText(n+1);
		m_dAllowance[n] = 0.0;
	}

	m_dAllowanceTotal = 0.0;
	m_dAllowanceTax = 0.0;
	m_dCash = 0.0;
	m_dCashTax = 0.0;
}

/**********************************************************************/

void CStockReportAllowanceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStockReportAllowanceDlg)
	//}}AFX_DATA_MAP
	DDX_Control( pDX, IDC_STATIC_TAX, m_staticTax );
	DDX_Control( pDX, IDC_STATIC_TAX2, m_staticTax2 );

	for ( int n = 0; n < 9; n++ )
	{
		DDX_Text(pDX, m_nAllowanceNameID[n], m_strAllowanceName[n] );
		DDX_Text(pDX, m_nAllowanceID[n], m_dAllowance[n] );
	}

	DDX_Text ( pDX, IDC_EDIT_ALLOWANCETOTAL, m_dAllowanceTotal );
	DDX_Text ( pDX, IDC_EDIT_ALLOWANCETAX, m_dAllowanceTax );
	DDX_Text ( pDX, IDC_EDIT_CASH, m_dCash );
	DDX_Text ( pDX, IDC_EDIT_CASHTAX, m_dCashTax );
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CStockReportAllowanceDlg, CDialog)
	//{{AFX_MSG_MAP(CStockReportAllowanceDlg)
	ON_EN_KILLFOCUS(IDC_EDIT_ALLOWANCE1, OnKillFocusAllowance)
	ON_EN_KILLFOCUS(IDC_EDIT_CASH, OnKillFocusCash)
	ON_EN_KILLFOCUS(IDC_EDIT_ALLOWANCE2, OnKillFocusAllowance)
	ON_EN_KILLFOCUS(IDC_EDIT_ALLOWANCE3, OnKillFocusAllowance)
	ON_EN_KILLFOCUS(IDC_EDIT_ALLOWANCE4, OnKillFocusAllowance)
	ON_EN_KILLFOCUS(IDC_EDIT_ALLOWANCE5, OnKillFocusAllowance)
	ON_EN_KILLFOCUS(IDC_EDIT_ALLOWANCE6, OnKillFocusAllowance)
	ON_EN_KILLFOCUS(IDC_EDIT_ALLOWANCE7, OnKillFocusAllowance)
	ON_EN_KILLFOCUS(IDC_EDIT_ALLOWANCE8, OnKillFocusAllowance)
	ON_EN_KILLFOCUS(IDC_EDIT_ALLOWANCE9, OnKillFocusAllowance)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/
CEdit* CStockReportAllowanceDlg::GetEditAllowance( int n ) { return GetEdit( m_nAllowanceID[n] ); }
CEdit* CStockReportAllowanceDlg::GetEditAllowanceTotal() { return GetEdit( IDC_EDIT_ALLOWANCETOTAL ); }
CEdit* CStockReportAllowanceDlg::GetEditAllowanceTax() { return GetEdit( IDC_EDIT_ALLOWANCETAX ); }
CEdit* CStockReportAllowanceDlg::GetEditCash() { return GetEdit( IDC_EDIT_CASH ); }
CEdit* CStockReportAllowanceDlg::GetEditCashTax() { return GetEdit( IDC_EDIT_CASHTAX ); }
/**********************************************************************/

BOOL CStockReportAllowanceDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_staticTax.SetWindowText( EcrmanOptions.GetTaxString( "%T content" ) );
	m_staticTax2.SetWindowText( EcrmanOptions.GetTaxString( "%T content" ) );

	for ( int n = 0; n < 9; n++ )
	{
		SubclassEdit ( m_nAllowanceID[n], SS_NUM_DP, 10, "%.*f", SysInfo.GetDPValue() );
		GetEditAllowance(n) -> EnableWindow ( DataManager.Allowance.GetReportEnable(n+1) );
	}

	SubclassEdit ( IDC_EDIT_ALLOWANCETOTAL, SS_NUM_DP, 10, "%.*f", SysInfo.GetDPValue() );
	SubclassEdit ( IDC_EDIT_ALLOWANCETAX, SS_NUM_DP, 10, "%.*f", SysInfo.GetDPValue() );
	SubclassEdit ( IDC_EDIT_CASH, SS_NUM_DP, 10, "%.*f", SysInfo.GetDPValue() );
	SubclassEdit ( IDC_EDIT_CASHTAX, SS_NUM_DP, 10, "%.*f", SysInfo.GetDPValue() );

	return TRUE;  
}

/**********************************************************************/

void CStockReportAllowanceDlg::OnKillFocusAllowance() 
{
	UpdateData( TRUE, FALSE );
		
	m_dAllowanceTotal = 0.0;
	m_dAllowanceTax = 0.0;

	for ( int n = 0; n < 9; n++ )
	{
		CTaxRateInfo TaxRateInfo;
		DataManager.TaxRates.GetTaxRateInfo( DataManager.Allowance.GetTaxBand( n + 1 ), TaxRateInfo );

		m_dAllowanceTotal += m_dAllowance[n];
		m_dAllowanceTax += m_dAllowance[n] - ( m_dAllowance[n] / ( 1 + ( TaxRateInfo.m_dTaxRate / 100.0 ) ) );	
	}

	UpdateData ( FALSE );
}

/**********************************************************************/

void CStockReportAllowanceDlg::OnKillFocusCash() 
{
	UpdateData( TRUE, FALSE );

	CTaxRateInfo TaxRateInfo;
	DataManager.TaxRates.GetTaxRateInfo( 1, TaxRateInfo );
	m_dCashTax = m_dCash - ( m_dCash / ( 1 + ( TaxRateInfo.m_dTaxRate / 100.0 ) ) );
	UpdateData ( FALSE );	
}

/**********************************************************************/

void CStockReportAllowanceDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
		GetParent() -> PostMessage ( WM_APP, STOCK_RECONCILE_CASH, (long) this );
}

/**********************************************************************/
