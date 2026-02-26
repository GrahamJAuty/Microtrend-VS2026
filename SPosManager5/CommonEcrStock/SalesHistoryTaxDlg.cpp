/**********************************************************************/
#include "SysInfo.h"
/**********************************************************************/
#include "SalesHistoryTaxDlg.h"
/**********************************************************************/

CSalesHistoryTaxDlg::CSalesHistoryTaxDlg(CWnd* pParent)
	: CSSDialog(CSalesHistoryTaxDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSalesHistoryTaxDlg)
	//}}AFX_DATA_INIT

	m_IDTaxStatic[0] = IDC_STATIC_TAXBAND0;
	m_IDTaxStatic[1] = IDC_STATIC_TAXBAND1;
	m_IDTaxStatic[2] = IDC_STATIC_TAXBAND2;
	m_IDTaxStatic[3] = IDC_STATIC_TAXBAND3;
	m_IDTaxStatic[4] = IDC_STATIC_TAXBAND4;
	m_IDTaxStatic[5] = IDC_STATIC_TAXBAND5;
	m_IDTaxStatic[6] = IDC_STATIC_TAXBAND6;
	m_IDTaxStatic[7] = IDC_STATIC_TAXBAND7;
	m_IDTaxStatic[8] = IDC_STATIC_TAXBAND8;
	m_IDTaxStatic[9] = IDC_STATIC_TAXBAND9;
	m_IDTaxStatic[10] = IDC_STATIC_TAXBAND10;
	m_IDTaxStatic[11] = IDC_STATIC_TAXBAND11;
	m_IDTaxStatic[12] = IDC_STATIC_TAXBAND12;

	m_IDTaxCheck[0] = IDC_CHECK_TAXBAND0;
	m_IDTaxCheck[1] = IDC_CHECK_TAXBAND1;
	m_IDTaxCheck[2] = IDC_CHECK_TAXBAND2;
	m_IDTaxCheck[3] = IDC_CHECK_TAXBAND3;
	m_IDTaxCheck[4] = IDC_CHECK_TAXBAND4;
	m_IDTaxCheck[5] = IDC_CHECK_TAXBAND5;
	m_IDTaxCheck[6] = IDC_CHECK_TAXBAND6;
	m_IDTaxCheck[7] = IDC_CHECK_TAXBAND7;
	m_IDTaxCheck[8] = IDC_CHECK_TAXBAND8;
	m_IDTaxCheck[9] = IDC_CHECK_TAXBAND9;
	m_IDTaxCheck[10] = IDC_CHECK_TAXBAND10;
	m_IDTaxCheck[11] = IDC_CHECK_TAXBAND11;
	m_IDTaxCheck[12] = IDC_CHECK_TAXBAND12;
}

/**********************************************************************/

void CSalesHistoryTaxDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSalesHistoryTaxDlg)

	for (int n = 0; n <= MAX_TAX_BANDS; n++)
	{
		DDX_Control(pDX, m_IDTaxStatic[n], m_staticTaxBand[n]);
		DDX_Control(pDX, m_IDTaxCheck[n], m_checkTaxBand[n]);
	}
	DDX_Control(pDX, IDC_BUTTON_ALL, m_buttonAll);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CSalesHistoryTaxDlg, CDialog)
	//{{AFX_MSG_MAP(CSalesHistoryTaxDlg)
 	   ON_BN_CLICKED(IDC_BUTTON_ALL, OnButtonAll)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CSalesHistoryTaxDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	SetWindowText( EcrmanOptions.GetTaxString( "Filter by %T Band" ) );

	m_staticTaxBand[0].SetWindowText ( "0.0000" );

	for (int n = 1; n <= MAX_TAX_BANDS; n++)
	{
		CString strTaxBand;
		CTaxRateInfo TaxRateInfo;
		DataManager.TaxRates.GetTaxRateInfo(n, TaxRateInfo);
		strTaxBand.Format("%.4lf", TaxRateInfo.m_dTaxRate);
		m_staticTaxBand[n].SetWindowText(strTaxBand);
	}

	int nFilter = SysInfo.GetCustomTaxFilter();
	int nMask = 1;

	for (int n = 0; n <= MAX_TAX_BANDS; n++)
	{
		m_checkTaxBand[n].SetCheck((nFilter & nMask) != 0);
		nMask *= 2;
	}

	if (TAXBAND_FILTER_ALL == nFilter)
	{
		m_bAllButton = FALSE;
		m_buttonAll.SetWindowText("None");
	}
	else
	{
		m_bAllButton = TRUE;
		m_buttonAll.SetWindowText("All");
	}
	
	return TRUE;  
}

/**********************************************************************/

void CSalesHistoryTaxDlg::OnButtonAll()
{
	for (int n = 0; n <= MAX_TAX_BANDS; n++)
	{
		m_checkTaxBand[n].SetCheck(m_bAllButton);
	}

	if (TRUE == m_bAllButton)
	{
		m_bAllButton = FALSE;
		m_buttonAll.SetWindowText("None");
	}
	else
	{
		m_bAllButton = TRUE;
		m_buttonAll.SetWindowText("All");
	}
}

/**********************************************************************/

void CSalesHistoryTaxDlg::OnOK() 
{
	int nFilter = 0;
	int nMask = 1;

	for (int n = 0; n <= MAX_TAX_BANDS; n++)
	{
		if (IsTicked(m_checkTaxBand[n]))
		{
			nFilter += nMask;
		}
		nMask *= 2;
	}

	SysInfo.SetCustomTaxFilter( nFilter );

	EndDialog ( IDOK );
}

/**********************************************************************/
