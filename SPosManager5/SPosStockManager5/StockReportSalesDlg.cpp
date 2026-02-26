/**********************************************************************/
//#include "DataManager.h"
//#include "EcrmanOptionsIni.h"
#include "RepCSV.h"
#include "StockReportNames.h"
/**********************************************************************/
#include "StockReportSalesDlg.h"
/**********************************************************************/

CStockReportSalesDlg::CStockReportSalesDlg( CWnd* pParent)
	: CDialog(CStockReportSalesDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStockReportSalesDlg)
	m_bManual = FALSE;
	m_bImport = FALSE;
	m_bRecipe = FALSE;
	m_bZeroSkip = FALSE;
	m_bAverage = FALSE;
	m_bTax = FALSE;
	//}}AFX_DATA_INIT

	m_checkEposID[0] = IDC_CHECK_EPOS1;
	m_checkEposID[1] = IDC_CHECK_EPOS2;
	m_checkEposID[2] = IDC_CHECK_EPOS3;
	m_checkEposID[3] = IDC_CHECK_EPOS4;
	m_checkEposID[4] = IDC_CHECK_EPOS5;
	m_checkEposID[5] = IDC_CHECK_EPOS6;
	m_checkEposID[6] = IDC_CHECK_EPOS7;
	m_checkEposID[7] = IDC_CHECK_EPOS8;
	m_checkEposID[8] = IDC_CHECK_EPOS9;
	m_checkEposID[9] = IDC_CHECK_EPOS10;

	for ( int n = 0; n < 10; n++ )
		m_bEpos[n] = FALSE;

	CString strBuffer;
	bool bGotFile = FALSE;

	CSSFile filePrefs;
	CFilenameUpdater FnUp ( SysFiles::StockSalesDef );
	if ( filePrefs.Open ( FnUp.GetFilenameToUse(), "rb" ) == TRUE )
	{
		if ( filePrefs.ReadString ( strBuffer ) == TRUE )
		{
			bGotFile = TRUE;
			CCSVStockSalesKey csv ( strBuffer );
			m_bEpos[0] = csv.PrintEpos1();
			m_bEpos[1] = csv.PrintEpos2();
			m_bEpos[2] = csv.PrintEpos3();
			m_bEpos[3] = csv.PrintEpos4();
			m_bEpos[4] = csv.PrintEpos5();
			m_bEpos[5] = csv.PrintEpos6();
			m_bEpos[6] = csv.PrintEpos7();
			m_bEpos[7] = csv.PrintEpos8();
			m_bEpos[8] = csv.PrintEpos9();
			m_bEpos[9] = csv.PrintEpos10();
			m_bManual = csv.PrintManual();
			m_bImport = csv.PrintImport();
			m_bRecipe = csv.PrintRecipe();
			m_bAverage = csv.PrintAverage();
			m_bZeroSkip = csv.ZeroSkip();
			m_bTax = csv.PrintTax();
			m_nSalesReportType = csv.GetSalesReportType();
		}
	}

	if ( bGotFile == FALSE )
		RestoreDefaults();
}

/**********************************************************************/

void CStockReportSalesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStockReportSalesDlg)
	for ( int n = 0; n < 10; n++ )
	{
		DDX_Control(pDX, m_checkEposID[n], m_checkEpos[n] );
		DDX_Check(pDX, m_checkEposID[n], m_bEpos[n] );
	}

	DDX_Control(pDX, IDC_CHECK_MANUAL, m_checkManual);
	DDX_Control(pDX, IDC_CHECK_IMPORT, m_checkImport);
	DDX_Control(pDX, IDC_CHECK_RECIPE, m_checkRecipe);
	DDX_Control(pDX, IDC_CHECK_ZEROSKIP, m_checkZeroSkip);
	DDX_Control(pDX, IDC_CHECK_AVERAGE, m_checkAverage);

	DDX_Check(pDX, IDC_CHECK_MANUAL, m_bManual);
	DDX_Check(pDX, IDC_CHECK_IMPORT, m_bImport);
	DDX_Check(pDX, IDC_CHECK_RECIPE, m_bRecipe);
	DDX_Check(pDX, IDC_CHECK_ZEROSKIP, m_bZeroSkip);
	DDX_Check(pDX, IDC_CHECK_AVERAGE, m_bAverage);

	DDX_Radio(pDX, IDC_RADIO_REPTYPE, m_nSalesReportType);
	DDX_Control(pDX, IDC_CHECK_TAX, m_checkTax);
	DDX_Check(pDX, IDC_CHECK_TAX, m_bTax);
	
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CStockReportSalesDlg, CDialog)
	//{{AFX_MSG_MAP(CStockReportSalesDlg)
	ON_BN_CLICKED(IDC_BUTTON_DEFAULT, OnButtonDefault)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	ON_BN_CLICKED(IDC_RADIO_REPTYPE, OnRadioReportType)
	ON_BN_CLICKED(IDC_RADIO2, OnRadioReportType)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CStockReportSalesDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_checkTax.SetWindowText( EcrmanOptions.GetTaxString( "Estimated %T" ) );

	for ( int n = 0; n < 10; n++ )
	{
		CString str;
		str.Format( "%2.2d  %s", n + 1, DataManager.PriceText.GetReportText(n) );
		m_checkEpos[n].SetWindowText( str );
	}

	UpdateEnables();
	return TRUE;  
}

/**********************************************************************/

void CStockReportSalesDlg::OnButtonDefault() 
{
	GetParent() -> PostMessage ( WM_APP, 0, 0 );
	RestoreDefaults();
	UpdateEnables();
	UpdateData ( FALSE );	
}

/**********************************************************************/

void CStockReportSalesDlg::OnButtonSave() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		CSSFile filePrefs;
		CFilenameUpdater FnUp ( SysFiles::StockSalesDef );
		if ( filePrefs.Open ( FnUp.GetFilenameToUse(), "wb" ) == TRUE )
			filePrefs.WriteLine ( GetCSVLine() );
	}
}

/**********************************************************************/

void CStockReportSalesDlg::OnRadioReportType() 
{
	if ( UpdateData ( TRUE ) == TRUE )
		UpdateEnables();	
}

/**********************************************************************/

void CStockReportSalesDlg::UpdateEnables()
{
	bool bEnable = ( m_nSalesReportType == 0 ) ? FALSE : TRUE;

	for( int n = 0; n < 10; n++ )
		m_checkEpos[n].EnableWindow( bEnable );

	m_checkImport.EnableWindow ( bEnable );
	m_checkManual.EnableWindow ( bEnable );
	m_checkRecipe.EnableWindow ( bEnable );
}

/**********************************************************************/

const char* CStockReportSalesDlg::GetCSVLine()
{
	CCSV csv;	

	csv.Add ( (int) m_bEpos[0] );
	csv.Add ( (int) m_bEpos[1] );
	csv.Add ( (int) m_bEpos[2] );
	csv.Add ( (int) m_bEpos[3] );
	csv.Add ( (int) m_bImport );
	csv.Add ( (int) m_bManual );
	csv.Add ( (int) m_bRecipe );
	csv.Add ( (int) m_bAverage );
	csv.Add ( (int) m_bZeroSkip );
	csv.Add ( m_nSalesReportType );
	csv.Add ( 0 );						//EPOS SUMMARY NOT USED
	csv.Add ( 0 );						//DATE SELECT TYPE NOT USED
	csv.Add ( 0 );						//DATE FROM TYPE NOT USED
	csv.Add ( 0 );						//DATE TO TYPE NOT USED
	csv.Add ( 0 );						//SUPPLIER FLAG USED TO GO HERE
	csv.Add ( (int) m_bTax );
	csv.Add ( 0 );						//CUSTOMER USED TO GO HERE
	csv.Add ( ( int) m_bEpos[4] );
	csv.Add ( ( int) m_bEpos[5] );
	csv.Add ( ( int) m_bEpos[6] );
	csv.Add ( ( int) m_bEpos[7] );
	csv.Add ( ( int) m_bEpos[8] );
	csv.Add ( ( int) m_bEpos[9] );

	m_strFieldSelect = csv.GetLine();
	return m_strFieldSelect;
}

/**********************************************************************/

const char* CStockReportSalesDlg::GetReportFields()
{
	CCSV csv;

	if ( m_nSalesReportType == 1 )
	{
		for ( int n = 0; n < 10; n++ )
			csv.Add ( (int) m_bEpos[n] );
		
		csv.Add ( (int) m_bImport );
		csv.Add ( (int) m_bManual );
		csv.Add ( (int) m_bRecipe );
		csv.Add ( 0 );					//EPOS SUMMARY NOT USED
	}
	csv.Add ( ( int ) m_bAverage );
	csv.Add ( 0 );						//SUPPLIER FLAG USED TO GO HERE
	csv.Add ( ( int ) m_bTax );
	m_strFieldSelect = csv.GetLine();
	return m_strFieldSelect;
}

/**********************************************************************/

void CStockReportSalesDlg::RestoreDefaults()
{
	for ( int n = 0; n < 10; n++ )
		m_bEpos[n] = FALSE;

	m_bManual = FALSE;
	m_bImport = FALSE;
	m_bRecipe = FALSE;
	m_bZeroSkip = TRUE;
	m_bAverage = TRUE;
	m_bTax = FALSE;
	m_nSalesReportType = 0;
}

/**********************************************************************/

void CStockReportSalesDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
		GetParent() -> PostMessage ( WM_APP, STOCK_SALES, long (this) );
}

/**********************************************************************/
