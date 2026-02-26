/**********************************************************************/
#include "ListDataPrinterSelectDlg.h"
#include "MaxLengths.h"
#include "PCOptions.h"
#include "PropPageOptionsReports3.h"
/**********************************************************************/
#include "PropPageOptionsLabels.h"
/**********************************************************************/

CPropPageOptionsLabels::CPropPageOptionsLabels() : CSSPropertyPage(CPropPageOptionsLabels::IDD)
{
	//{{AFX_DATA_INIT(CPropPageOptionsLabels)
	
	m_strShelfEdgeCurrency1 = _T("");
	m_strShelfEdgeCurrency2 = _T("");
	
	m_strProductCurrency1 = _T("");
	m_strProductCurrency2 = _T("");
	
	//}}AFX_DATA_INIT

	m_strSystemDefaultPrinter = "<Use System Default>";
}

/**********************************************************************/

CPropPageOptionsLabels::~CPropPageOptionsLabels()
{
}

/**********************************************************************/

void CPropPageOptionsLabels::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPageOptionsLabels)

	DDX_Control(pDX, IDC_CHECK_SHELF_OPTION1, m_checkShelfOption1);
	DDX_Control(pDX, IDC_CHECK_SHELF_OPTION2, m_checkShelfOption2);
	DDX_Control(pDX, IDC_CHECK_SHELF_OPTION3, m_checkShelfOption3);
	DDX_Control(pDX, IDC_CHECK_SHELF_OPTION4, m_checkShelfOption4);

	DDX_Control(pDX, IDC_COMBO_SHELF_PRICE1, m_comboShelfEdgePrice1);
	DDX_Control(pDX, IDC_COMBO_SHELF_TAX1, m_comboShelfEdgeTax1);
	DDX_Control(pDX, IDC_EDIT_SHELF_CURRENCY1, m_editShelfEdgeCurrency1);
	DDX_Control(pDX, IDC_COMBO_SHELF_PRICE2, m_comboShelfEdgePrice2);
	DDX_Control(pDX, IDC_COMBO_SHELF_TAX2, m_comboShelfEdgeTax2);
	DDX_Control(pDX, IDC_EDIT_SHELF_CURRENCY2, m_editShelfEdgeCurrency2);

	DDX_Control(pDX, IDC_STATIC_SHELF1, m_staticShelf1);
	DDX_Control(pDX, IDC_STATIC_SHELF2, m_staticShelf2);
	DDX_Control(pDX, IDC_STATIC_SHELF3, m_staticShelf3);
	DDX_Control(pDX, IDC_STATIC_SHELF4, m_staticShelf4);

	DDX_Control(pDX, IDC_CHECK_PRODUCT_OPTION1, m_checkProductOption1);
	DDX_Control(pDX, IDC_CHECK_PRODUCT_OPTION2, m_checkProductOption2);
	DDX_Control(pDX, IDC_CHECK_PRODUCT_OPTION3, m_checkProductOption3);
	DDX_Control(pDX, IDC_CHECK_PRODUCT_OPTION4, m_checkProductOption4);

	DDX_Control(pDX, IDC_COMBO_PRODUCT_PRICE1, m_comboProductPrice1);
	DDX_Control(pDX, IDC_COMBO_PRODUCT_TAX1, m_comboProductTax1);
	DDX_Control(pDX, IDC_EDIT_PRODUCT_CURRENCY1, m_editProductCurrency1);
	DDX_Control(pDX, IDC_COMBO_PRODUCT_PRICE2, m_comboProductPrice2);
	DDX_Control(pDX, IDC_COMBO_PRODUCT_TAX2, m_comboProductTax2);
	DDX_Control(pDX, IDC_EDIT_PRODUCT_CURRENCY2, m_editProductCurrency2);

	DDX_Control(pDX, IDC_STATIC_PRODUCT1, m_staticProduct1);
	DDX_Control(pDX, IDC_STATIC_PRODUCT2, m_staticProduct2);
	DDX_Control(pDX, IDC_STATIC_PRODUCT3, m_staticProduct3);
	DDX_Control(pDX, IDC_STATIC_PRODUCT4, m_staticProduct4);

	DDX_Control(pDX, IDC_EDIT_PRINTER, m_editPrinter);

	//}}AFX_DATA_MAP
	
	DDX_Text(pDX, IDC_EDIT_SHELF_CURRENCY1, m_strShelfEdgeCurrency1);
	DDV_MaxChars(pDX, m_strShelfEdgeCurrency1, MAX_LENGTH_CURRENCY_SYMBOL);
	DDX_Text(pDX, IDC_EDIT_SHELF_CURRENCY2, m_strShelfEdgeCurrency2);
	DDV_MaxChars(pDX, m_strShelfEdgeCurrency2, MAX_LENGTH_CURRENCY_SYMBOL);
	
	DDX_Text(pDX, IDC_EDIT_PRODUCT_CURRENCY1, m_strProductCurrency1);
	DDV_MaxChars(pDX, m_strProductCurrency1, MAX_LENGTH_CURRENCY_SYMBOL);
	DDX_Text(pDX, IDC_EDIT_PRODUCT_CURRENCY2, m_strProductCurrency2);
	DDV_MaxChars(pDX, m_strProductCurrency2, MAX_LENGTH_CURRENCY_SYMBOL);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPageOptionsLabels, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPageOptionsLabels)
	ON_CBN_SELCHANGE(IDC_COMBO_PRODUCT_PRICE2, OnSelectPriceLevel2)
	ON_CBN_SELCHANGE(IDC_COMBO_SHELF_PRICE2, OnSelectPriceLevel2)
	ON_BN_CLICKED(IDC_BUTTON_PRINTER, OnButtonPrinter)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPropPageOptionsLabels::OnInitDialog() 
{
	GetRecordData();
	CPropertyPage::OnInitDialog();

	AddComboPriceLevels( m_comboShelfEdgePrice1, FALSE );
	AddComboPriceLevels( m_comboShelfEdgePrice2, TRUE );
	AddComboPriceLevels( m_comboProductPrice1, FALSE );
	AddComboPriceLevels( m_comboProductPrice2, TRUE );

	AddComboTaxType( m_comboShelfEdgeTax1 );
	AddComboTaxType( m_comboShelfEdgeTax2 );
	AddComboTaxType( m_comboProductTax1 );
	AddComboTaxType( m_comboProductTax2 );

#ifdef STOCKMAN_SYSTEM
	m_checkShelfOption1.SetWindowText( "Show Plu number" );
	m_checkShelfOption2.SetWindowText( "Show stock code" );
	m_checkShelfOption3.SetWindowText( "Show supplier reference" );
	m_checkShelfOption4.SetWindowText( "Remove leading zeros from Plu number" );
	m_checkProductOption1.SetWindowText( "Show Plu number" );
	m_checkProductOption2.SetWindowText( "Show stock code" );
	m_checkProductOption3.SetWindowText( "Show supplier reference" );
	m_checkProductOption4.SetWindowText( "Remove leading zeros from Plu number" );
#else
	m_checkShelfOption1.SetWindowText( "Show supplier reference" );
	m_checkShelfOption2.SetWindowText( "Remove leading zeros from Plu number" );
	m_checkShelfOption3.ShowWindow( SW_HIDE );
	m_checkShelfOption4.ShowWindow( SW_HIDE );
	m_checkProductOption1.SetWindowText( "Show supplier reference" );
	m_checkProductOption2.SetWindowText( "Remove leading zeros from Plu number" );
	m_checkProductOption3.ShowWindow( SW_HIDE );
	m_checkProductOption4.ShowWindow( SW_HIDE );
#endif

	SetRecordControls();
	return TRUE;  
}

/**********************************************************************/

void CPropPageOptionsLabels::AddComboPriceLevels( CSSComboBox& combo, bool bNone )
{
	if ( TRUE == bNone )
		combo.AddString ( "None" );

	combo.AddString ( "Price 1" );
	combo.AddString ( "Price 2" );
	combo.AddString ( "Price 3" );
	combo.AddString ( "Price 4" );
}

/**********************************************************************/

void CPropPageOptionsLabels::AddComboTaxType( CSSComboBox& combo )
{
	combo.AddString ( EcrmanOptions.GetTaxString( "Inc %T" ) );
	combo.AddString ( EcrmanOptions.GetTaxString( "Ex %T" ) );
	combo.AddString ( EcrmanOptions.GetTaxString( "%T Amt" ) );
}

/**********************************************************************/

BOOL CPropPageOptionsLabels::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	SaveRecord();
	return bResult;
}

/**********************************************************************/

BOOL CPropPageOptionsLabels::OnSetActive() 
{
	GetRecordData();
	SetRecordControls();
	bool bResult = ( CPropertyPage::OnSetActive() != 0 );
	return bResult;
}

/**********************************************************************/

void CPropPageOptionsLabels::GetRecordData()
{
	m_strShelfEdgeCurrency1 = EcrmanOptions.GetLabelsShelfPriceSymbol1();
	m_strShelfEdgeCurrency2 = EcrmanOptions.GetLabelsShelfPriceSymbol2();
	m_strProductCurrency1 = EcrmanOptions.GetLabelsProductPriceSymbol1();
	m_strProductCurrency2 = EcrmanOptions.GetLabelsProductPriceSymbol2();
}

/**********************************************************************/

void CPropPageOptionsLabels::SaveRecord()
{

#ifdef STOCKMAN_SYSTEM
	EcrmanOptions.SetLabelsShelfShowPluFlag( m_checkShelfOption1.GetCheck() != 0 );
	EcrmanOptions.SetLabelsShelfShowStockCodeFlag( m_checkShelfOption2.GetCheck() != 0 );
	EcrmanOptions.SetLabelsShelfShowSuppRefFlag( m_checkShelfOption3.GetCheck() != 0 );
	EcrmanOptions.SetLabelsShelfTrimPluZerosFlag( m_checkShelfOption4.GetCheck() != 0 );
	EcrmanOptions.SetLabelsProductShowPluFlag( m_checkProductOption1.GetCheck() != 0 );
	EcrmanOptions.SetLabelsProductShowStockCodeFlag( m_checkProductOption2.GetCheck() != 0 );
	EcrmanOptions.SetLabelsProductShowSuppRefFlag( m_checkProductOption3.GetCheck() != 0 );
	EcrmanOptions.SetLabelsProductTrimPluZerosFlag( m_checkProductOption4.GetCheck() != 0 );
#else
	EcrmanOptions.SetLabelsShelfShowSuppRefFlag( m_checkShelfOption1.GetCheck() != 0 );
	EcrmanOptions.SetLabelsShelfTrimPluZerosFlag( m_checkShelfOption2.GetCheck() != 0 );
	EcrmanOptions.SetLabelsProductShowSuppRefFlag( m_checkProductOption1.GetCheck() != 0 );
	EcrmanOptions.SetLabelsProductTrimPluZerosFlag( m_checkProductOption2.GetCheck() != 0 );
#endif

	int nLevel = m_comboShelfEdgePrice1.GetCurSel();
	if ( nLevel < 0 || nLevel > 3) nLevel = 0;
	EcrmanOptions.SetLabelsShelfPriceLevel1 ( nLevel + 1 );

	int nType = m_comboShelfEdgeTax1.GetCurSel();
	if ( nType < 0 || nType > 2 ) nType = 0;
	EcrmanOptions.SetLabelsShelfTaxType1( nType );

	EcrmanOptions.SetLabelsShelfPriceSymbol1( m_strShelfEdgeCurrency1 );
	
	nLevel = m_comboShelfEdgePrice2.GetCurSel();
	if ( nLevel < 0 || nLevel > 4 ) nLevel = 0;
	EcrmanOptions.SetLabelsShelfPriceLevel2 ( nLevel );

	nType = m_comboShelfEdgeTax2.GetCurSel();
	if ( nType < 0 || nType > 2 ) nType = 0;
	EcrmanOptions.SetLabelsShelfTaxType2( nType );

	EcrmanOptions.SetLabelsShelfPriceSymbol2( m_strShelfEdgeCurrency2 );
	
	nLevel = m_comboProductPrice1.GetCurSel();
	if ( nLevel < 0 || nLevel > 3 ) nLevel = 0;
	EcrmanOptions.SetLabelsProductPriceLevel1 ( nLevel + 1 );

	nType = m_comboProductTax1.GetCurSel();
	if ( nType < 0 || nType > 2 ) nType = 0;
	EcrmanOptions.SetLabelsProductTaxType1( nType );

	EcrmanOptions.SetLabelsProductPriceSymbol1( m_strProductCurrency1 );

	nLevel = m_comboProductPrice2.GetCurSel();
	if ( nLevel < 0 || nLevel > 4 ) nLevel = 0;
	EcrmanOptions.SetLabelsProductPriceLevel2 ( nLevel );

	nType = m_comboProductTax2.GetCurSel();
	if ( nType < 0 || nType > 2 ) nType = 0;
	EcrmanOptions.SetLabelsProductTaxType2( nType );

	EcrmanOptions.SetLabelsProductPriceSymbol2( m_strProductCurrency2 );

	CString strPrinter = GetEditBoxText( m_editPrinter );

	if ( strPrinter == m_strSystemDefaultPrinter )
	{
		PCOptionsClient.SetDefaultLabelPrinter( "" );
	}
	else
	{
		PCOptionsClient.SetDefaultLabelPrinter( strPrinter );
	}
}

/**********************************************************************/

void CPropPageOptionsLabels::SetRecordControls()
{
#ifdef STOCKMAN_SYSTEM
	m_checkShelfOption1.SetCheck( EcrmanOptions.GetLabelsShelfShowPluFlag() );
	m_checkShelfOption2.SetCheck( EcrmanOptions.GetLabelsShelfShowStockCodeFlag() );
	m_checkShelfOption3.SetCheck( EcrmanOptions.GetLabelsShelfShowSuppRefFlag() );
	m_checkShelfOption4.SetCheck( EcrmanOptions.GetLabelsShelfTrimPluZerosFlag() );
	m_checkProductOption1.SetCheck( EcrmanOptions.GetLabelsProductShowPluFlag() );
	m_checkProductOption2.SetCheck( EcrmanOptions.GetLabelsProductShowStockCodeFlag() );
	m_checkProductOption3.SetCheck( EcrmanOptions.GetLabelsProductShowSuppRefFlag() );
	m_checkProductOption4.SetCheck( EcrmanOptions.GetLabelsProductTrimPluZerosFlag() );
#else
	m_checkShelfOption1.SetCheck( EcrmanOptions.GetLabelsShelfShowSuppRefFlag() );
	m_checkShelfOption2.SetCheck( EcrmanOptions.GetLabelsShelfTrimPluZerosFlag() );
	m_checkProductOption1.SetCheck( EcrmanOptions.GetLabelsProductShowSuppRefFlag() );
	m_checkProductOption2.SetCheck( EcrmanOptions.GetLabelsProductTrimPluZerosFlag() );
#endif

	int nLevel = EcrmanOptions.GetLabelsShelfPriceLevel1();
	if ( nLevel < 1 || nLevel > 4 ) nLevel = 1;
	m_comboShelfEdgePrice1.SetCurSel ( nLevel - 1 );

	int nType = EcrmanOptions.GetLabelsShelfTaxType1();
	if ( nType < 0 || nType > 2 ) nType = 0;
	m_comboShelfEdgeTax1.SetCurSel( nType );

	nLevel = EcrmanOptions.GetLabelsShelfPriceLevel2();
	if ( nLevel < 0 || nLevel > 4 ) nLevel = 0;
	m_comboShelfEdgePrice2.SetCurSel ( nLevel );
	
	if ( nLevel == 0 ) 
	{
		m_editShelfEdgeCurrency2.EnableWindow ( FALSE );
		m_comboShelfEdgeTax2.EnableWindow ( FALSE );
	}

	nType = EcrmanOptions.GetLabelsShelfTaxType2();
	if ( nType < 0 || nType > 2 ) nType = 0;
	m_comboShelfEdgeTax2.SetCurSel( nType );

	nLevel = EcrmanOptions.GetLabelsProductPriceLevel1();
	if ( nLevel < 1 || nLevel > 4 ) nLevel = 1;
	m_comboProductPrice1.SetCurSel ( nLevel - 1 );

	nType = EcrmanOptions.GetLabelsProductTaxType1();
	if ( nType < 0 || nType > 2 ) nType = 0;
	m_comboProductTax1.SetCurSel( nType );

	nLevel = EcrmanOptions.GetLabelsProductPriceLevel2();
	if ( nLevel < 0 || nLevel > 4 ) nLevel = 0;
	m_comboProductPrice2.SetCurSel ( nLevel );

	if ( nLevel == 0 ) 
	{
		m_editProductCurrency2.EnableWindow ( FALSE );
		m_comboProductTax2.EnableWindow ( FALSE );
	}
	
	nType = EcrmanOptions.GetLabelsProductTaxType2();
	if ( nType < 0 || nType > 2 ) nType = 0;
	m_comboProductTax2.SetCurSel( nType );

	CString strPrinter = PCOptionsClient.GetDefaultLabelPrinter();

	if ( strPrinter != "" )
	{
		m_editPrinter.SetWindowText( strPrinter );
	}
	else
	{
		m_editPrinter.SetWindowText( m_strSystemDefaultPrinter );
	}
}

/**********************************************************************/

void CPropPageOptionsLabels::OnSelectPriceLevel2() 
{
	bool bEnableShelfEdgePrice2 = ( m_comboShelfEdgePrice2.GetCurSel() != 0 );
	bool bEnableProductPrice2 = ( m_comboProductPrice2.GetCurSel() != 0 );
	m_editShelfEdgeCurrency2.EnableWindow ( bEnableShelfEdgePrice2 );
	m_comboShelfEdgeTax2.EnableWindow ( bEnableShelfEdgePrice2 );
	m_editProductCurrency2.EnableWindow ( bEnableProductPrice2 );
	m_comboProductTax2.EnableWindow ( bEnableProductPrice2 );
}

/**********************************************************************/

void CPropPageOptionsLabels::OnButtonPrinter() 
{
	CPropPageOptionsReports3::ShowPrinterList(this, m_editPrinter,m_strSystemDefaultPrinter, "Barcode Labels" );
}

/**********************************************************************/