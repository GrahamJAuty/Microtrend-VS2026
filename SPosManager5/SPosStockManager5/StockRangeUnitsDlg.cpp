/**********************************************************************/
 
/**********************************************************************/
//#include "DataManager.h"
//#include "DataManagerNonDb.h"
//#include "EcrmanOptionsIni.h"
#include "MessageLogger.h"
#include "BarcodePluNoTable.h"
 
#include "StatusProgress.h"
#include "StockOptionsIni.h"
/**********************************************************************/
#include "StockRangeUnitsDlg.h"
/**********************************************************************/

CStockRangeUnitsDlg::CStockRangeUnitsDlg( CStockFilterArray& FilterArray, CWnd* pParent )
	: CSSDialog(CStockRangeUnitsDlg::IDD, pParent), m_FilterArray ( FilterArray )
{
	//{{AFX_DATA_INIT(CStockRangeUnitsDlg)
	m_strStockCodeFrom = _T("");
	m_strStockCodeTo = _T("");
	m_strStockUnit = _T("");
	m_strSubUnit = _T("");
	m_dWeight = 0.0;
	m_bChangeStockUnit = FALSE;
	m_bChangeSubUnit = FALSE;
	m_bChangeWastage = FALSE;
	m_bChangeWeight = FALSE;
	m_bChangeWeightUnit = FALSE;
	m_bChangeYield = FALSE;
	//}}AFX_DATA_INIT
	m_bModifiedStock = FALSE;
	m_dWastePercent = 0.0;
	m_dWeight = 0.0;
	m_nSubUnits = 1;

	CDataManagerInfo info;
	DataManagerNonDb.OpenStockUnit( DB_READONLY, info );
	DataManagerNonDb.OpenSubUnit( DB_READONLY, info );
}

/**********************************************************************/

CStockRangeUnitsDlg::~CStockRangeUnitsDlg()
{
	CDataManagerInfo info;
	DataManagerNonDb.CloseStockUnit( info );
	DataManagerNonDb.CloseSubUnit( info );
}

/**********************************************************************/

void CStockRangeUnitsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStockRangeUnitsDlg)
	DDX_Control(pDX, IDC_COMBO_WEIGHT_SELL, m_comboSaleWeight);
	DDX_Control(pDX, IDC_COMBO_WEIGHT_UNIT, m_comboUnitWeight);
	DDX_Control(pDX, IDC_COMBO_CATEGORY, m_comboCategory);
	DDX_Control(pDX, IDC_COMBO_SUBUNIT, m_comboSubUnit);
	DDX_Control(pDX, IDC_COMBO_STOCKUNIT, m_comboStockUnit);
	DDX_Check(pDX, IDC_CHECK_CHANGE_STOCKUNIT, m_bChangeStockUnit);
	DDX_Check(pDX, IDC_CHECK_CHANGE_SUBUNIT, m_bChangeSubUnit);
	DDX_Check(pDX, IDC_CHECK_CHANGE_WASTAGE, m_bChangeWastage);
	DDX_Check(pDX, IDC_CHECK_CHANGE_WEIGHT, m_bChangeWeight);
	DDX_Check(pDX, IDC_CHECK_CHANGE_WEIGHT_UNIT, m_bChangeWeightUnit);
	DDX_Check(pDX, IDC_CHECK_CHANGE_YIELD, m_bChangeYield);
	//}}AFX_DATA_MAP
	DDX_CBString(pDX, IDC_COMBO_STOCKUNIT, m_strStockUnit);
	DDV_MaxChars(pDX, m_strStockUnit, Stock::StockUnit.Max);
	DDX_CBString(pDX, IDC_COMBO_SUBUNIT, m_strSubUnit);
	DDV_MaxChars(pDX, m_strSubUnit, Stock::SubUnit.Max);
	DDX_Text(pDX, IDC_EDIT_STOCKCODE_FROM, m_strStockCodeFrom);
	DDV_MaxChars(pDX, m_strStockCodeFrom, Stock::StockCode.Max );
	DDX_Text(pDX, IDC_EDIT_STOCKCODE_TO, m_strStockCodeTo);
	DDV_MaxChars(pDX, m_strStockCodeTo, Stock::StockCode.Max );
	DDX_Text ( pDX, IDC_EDIT_WASTAGE, m_dWastePercent );
	DDV_MinMaxDouble ( pDX, m_dWastePercent, Stock::WastagePercent.Min, Stock::WastagePercent.Max );
	DDX_Text ( pDX, IDC_EDIT_YIELD, m_nSubUnits );
	DDV_MinMaxInt ( pDX, m_nSubUnits, Stock::SUYield.Min, Stock::SUYield.Max );
	DDX_Text(pDX, IDC_EDIT_WEIGHT, m_dWeight );
	DDV_MinMaxDouble(pDX, m_dWeight, Stock::SUWeight.Min, Stock::SUWeight.Max );
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CStockRangeUnitsDlg, CDialog)
	//{{AFX_MSG_MAP(CStockRangeUnitsDlg)
	ON_EN_KILLFOCUS(IDC_EDIT_STOCKCODE_FROM, OnKillFocusStockCode)
	ON_EN_KILLFOCUS(IDC_EDIT_STOCKCODE_TO, OnKillFocusStockCode)
	ON_BN_CLICKED(IDC_CHECK_CHANGE_STOCKUNIT, OnCheckToggle)
	ON_BN_CLICKED(IDC_CHECK_CHANGE_SUBUNIT, OnCheckToggle)
	ON_BN_CLICKED(IDC_CHECK_CHANGE_WASTAGE, OnCheckToggle)
	ON_BN_CLICKED(IDC_CHECK_CHANGE_YIELD, OnCheckToggle)
	ON_BN_CLICKED(IDC_CHECK_CHANGE_WEIGHT, OnCheckToggle)
	ON_BN_CLICKED(IDC_CHECK_CHANGE_WEIGHT_UNIT, OnCheckToggle)
	ON_CBN_SELCHANGE(IDC_COMBO_WEIGHT_UNIT, OnSelectUnitWeight)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CStockRangeUnitsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	SubclassEdit( IDC_EDIT_WASTAGE, SS_NUM_DP, 5, "%.2f" );
	SubclassEdit( IDC_EDIT_YIELD, SS_NUM, 4, "%d" );
	SubclassEdit( IDC_EDIT_WEIGHT, SS_NUM_DP, 11, "%.5f" );
	
	m_comboStockUnit.LimitText ( Stock::StockUnit.Max );
	m_comboSubUnit.LimitText ( Stock::SubUnit.Max );
	
	FillCategoryCombo();	
	FillStockUnitCombo();
	FillUnitWeightCombo();
	UpdateWeightCombos( WEIGHTTYPE_GALLON, WEIGHTTYPE_GALLON );
	SetEnables();
	return TRUE;  
}

/**********************************************************************/

void CStockRangeUnitsDlg::FillCategoryCombo()
{
	m_CategorySelector.SelectCategorySet ( m_FilterArray.GetCategoryFilter() );
	m_CategorySelector.IncludeAll();
	m_CategorySelector.IncludeCategorySets();
	m_CategorySelector.FillCategoryCombo ( &m_comboCategory );
}

/**********************************************************************/

void CStockRangeUnitsDlg::FillUnitWeightCombo()
{
	m_comboUnitWeight.AddString( "Gallon" );
	m_comboUnitWeight.AddString( "Pint" );
	m_comboUnitWeight.AddString( "Fluid Ounce" );
	m_comboUnitWeight.AddString( "Litre" );
	m_comboUnitWeight.AddString( "Centilitre" );
	m_comboUnitWeight.AddString( "Millilitre" );
	m_comboUnitWeight.AddString( "Kilogram" );
	m_comboUnitWeight.AddString( "Gram" );
	m_comboUnitWeight.SetItemData( 0, WEIGHTTYPE_GALLON );
	m_comboUnitWeight.SetItemData( 1, WEIGHTTYPE_PINT );
	m_comboUnitWeight.SetItemData( 2, WEIGHTTYPE_FLOUNCE );
	m_comboUnitWeight.SetItemData( 3, WEIGHTTYPE_LITRE );
	m_comboUnitWeight.SetItemData( 4, WEIGHTTYPE_CLITRE );
	m_comboUnitWeight.SetItemData( 5, WEIGHTTYPE_MLITRE );
	m_comboUnitWeight.SetItemData( 6, WEIGHTTYPE_KILO );
	m_comboUnitWeight.SetItemData( 7, WEIGHTTYPE_GRAM );
}

/**********************************************************************/

void CStockRangeUnitsDlg::GetWeightTypes( int& nUnitWeightType, int& nSaleWeightType ) 
{
	int nSel = m_comboUnitWeight.GetCurSel();
	if ( nSel >= 0 && nSel < m_comboUnitWeight.GetCount() )
		nUnitWeightType = m_comboUnitWeight.GetItemData( nSel );
	else
		nUnitWeightType = WEIGHTTYPE_GALLON;

	nSel = m_comboSaleWeight.GetCurSel();
	if ( nSel >= 0 && nSel < m_comboSaleWeight.GetCount() )
		nSaleWeightType = m_comboSaleWeight.GetItemData( nSel );
	else
		nSaleWeightType = WEIGHTTYPE_GALLON;
}

/**********************************************************************/

void CStockRangeUnitsDlg::OnSelectUnitWeight() 
{
	int nUnitWeightType, nSaleWeightType;
	GetWeightTypes( nUnitWeightType, nSaleWeightType );
	UpdateWeightCombos( nUnitWeightType, nSaleWeightType );
}

/**********************************************************************/

void CStockRangeUnitsDlg::UpdateWeightCombos( int nUnitWeightType, int nSaleWeightType )
{
	CWordArray arrayTypes;
	CStockCSVRecord::GetValidSaleWeights( nUnitWeightType, arrayTypes );

	for ( int i = 0; i < m_comboUnitWeight.GetCount(); i++ )
	{
		if ( nUnitWeightType == (int) m_comboUnitWeight.GetItemData(i) )
		{
			m_comboUnitWeight.SetCurSel(i);
			break;
		}
	}

	m_comboSaleWeight.ResetContent();
	for ( int i = 0; i < arrayTypes.GetSize(); i++ )
	{
		bool bAdd = TRUE;
		int nType = arrayTypes.GetAt(i);
		switch ( nType )
		{
		case WEIGHTTYPE_GALLON:	m_comboSaleWeight.AddString ( "Gallon" );		break;
		case WEIGHTTYPE_PINT:	m_comboSaleWeight.AddString ( "Pint" );			break;
		case WEIGHTTYPE_FLOUNCE:m_comboSaleWeight.AddString ( "Fluid Ounce" );	break;
		case WEIGHTTYPE_LITRE:	m_comboSaleWeight.AddString ( "Litre" );		break;
		case WEIGHTTYPE_CLITRE:	m_comboSaleWeight.AddString ( "Centilitre" );	break;
		case WEIGHTTYPE_MLITRE:	m_comboSaleWeight.AddString ( "Millilitre" );	break;
		case WEIGHTTYPE_KILO:	m_comboSaleWeight.AddString ( "Kilogram" );		break;
		case WEIGHTTYPE_GRAM:	m_comboSaleWeight.AddString ( "Gram" );			break;
		default:				bAdd = FALSE;									break;
		}

		if ( bAdd == TRUE )
			m_comboSaleWeight.SetItemData( m_comboSaleWeight.GetCount() - 1, nType );
	}

	m_comboSaleWeight.SetCurSel(0);
	for ( int i = 0; i < m_comboUnitWeight.GetCount(); i++ )
	{
		if ( nSaleWeightType == (int) m_comboSaleWeight.GetItemData(i) )
		{
			m_comboSaleWeight.SetCurSel(i);
			break;
		}
	}

	SetEnables();
}

/**********************************************************************/

void CStockRangeUnitsDlg::OnKillFocusStockCode() 
{
	UpdateData( TRUE, FALSE );
	m_strStockCodeFrom = BarcodePluNoTable.ConvertStockCode( m_strStockCodeFrom );
	m_strStockCodeTo = BarcodePluNoTable.ConvertStockCode( m_strStockCodeTo );
	UpdateData( FALSE );
}

/**********************************************************************/

void CStockRangeUnitsDlg::OnCheckToggle() 
{	
	UpdateData( TRUE, FALSE );
	SetEnables();
}

/**********************************************************************/
CEdit* CStockRangeUnitsDlg::GetEditYield() { return GetEdit( IDC_EDIT_YIELD ); }
CEdit* CStockRangeUnitsDlg::GetEditWeight() { return GetEdit( IDC_EDIT_WEIGHT ); }
CEdit* CStockRangeUnitsDlg::GetEditWastage() { return GetEdit( IDC_EDIT_WASTAGE ); }
/**********************************************************************/

void CStockRangeUnitsDlg::SetEnables()
{	
	m_comboStockUnit.EnableWindow ( m_bChangeStockUnit );
	m_comboSubUnit.EnableWindow ( m_bChangeSubUnit );
	
	GetEditYield() -> EnableWindow( m_bChangeYield );
	GetEditWeight() -> EnableWindow( m_bChangeWeight );
	GetEditWastage() -> EnableWindow( m_bChangeWastage );

	m_comboUnitWeight.EnableWindow( m_bChangeWeightUnit );
	m_comboSaleWeight.EnableWindow( ( m_bChangeWeightUnit ) && ( m_comboSaleWeight.GetCount() > 1 ) );
}

/**********************************************************************/

void CStockRangeUnitsDlg::FillStockUnitCombo()
{
	m_comboStockUnit.ResetContent();
	m_comboSubUnit.ResetContent();

	for ( int nIndex = 0; nIndex < DataManagerNonDb.StockUnit.GetSize(); nIndex++ )
		m_comboStockUnit.AddString ( DataManagerNonDb.StockUnit.GetText ( nIndex ) );

	for ( int nIndex = 0; nIndex < DataManagerNonDb.SubUnit.GetSize(); nIndex++ )
		m_comboSubUnit.AddString ( DataManagerNonDb.SubUnit.GetText ( nIndex ) );
}

/**********************************************************************/

int CStockRangeUnitsDlg::UpdateProgram()
{
	CCategorySetCSVRecord CategorySet;
	CategorySet.CreateCategorySet ( m_CategorySelector.GetSelection() );

	int nStartIdx, nRange;
	m_FilterArray.GetArrayIdxRange( m_strStockCodeFrom, m_strStockCodeTo, nStartIdx, nRange );

	int nUnitWeight, nSaleWeight;
	GetWeightTypes( nUnitWeight, nSaleWeight );

	int nCount = 0;

	if ( m_bChangeStockUnit || m_bChangeSubUnit || m_bChangeYield  
		|| m_bChangeWeight || m_bChangeWeightUnit || m_bChangeWastage )
	{
		for ( int nOffset = 0; nOffset < nRange; nOffset++ )
		{
			StatusProgress.SetPos ( nOffset, nRange ); 

			CStockCSVRecord StockRecord;
			int nStockIdx = m_FilterArray.GetStockItemIndexAt ( nStartIdx + nOffset );
			DataManager.Stock.GetAt ( nStockIdx, StockRecord );

			CString strStockCode = StockRecord.GetStockCode();
			if ( ( strStockCode < m_strStockCodeFrom ) || ( strStockCode > m_strStockCodeTo ) )
				continue;

			if ( CategorySet.GetCategoryStatus ( StockRecord.GetCategory() ) == FALSE )
				continue;
				
			nCount++;
				
			if ( m_bChangeStockUnit == TRUE )
				StockRecord.SetStockUnitName ( m_strStockUnit );

			if ( m_bChangeSubUnit == TRUE )
				StockRecord.SetSubUnitName ( m_strSubUnit );

			if ( m_bChangeYield == TRUE )
				StockRecord.SetSubUnits ( m_nSubUnits );

			if ( m_bChangeWeight == TRUE )
				StockRecord.SetSUWeight( m_dWeight );

			if ( m_bChangeWeightUnit == TRUE )
				StockRecord.SetSUWeightType( nUnitWeight, nSaleWeight );

			if ( m_bChangeWastage == TRUE )
				StockRecord.SetWastePercent ( m_dWastePercent );

			DataManager.Stock.SetAt ( nStockIdx, StockRecord );
			m_bModifiedStock = TRUE;
		}
	}
	return nCount;
}

/**********************************************************************/

void CStockRangeUnitsDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		OnKillFocusStockCode();

		int nCount = UpdateProgram();

		switch ( nCount )
		{
		case 0:
			Prompter.Info ( "No records were changed" );
			break;

		case 1:
			Prompter.Info ( "1 record was changed" );
			break;

		default:
			{
				CString strBuf;
				strBuf.Format ( "%d records were changed", nCount );
				Prompter.Info ( strBuf );
			}
			break;
		}

		MessageLogger.LogSystem( 3, 4, FALSE, nCount );

		EndDialog ( IDOK );
	}
}

/**********************************************************************/
