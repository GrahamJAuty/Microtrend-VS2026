/**********************************************************************/
 
/**********************************************************************/
#include "FilterTypes.h"
#include "BarcodePluNoTable.h"
#include "PriceHelpers.h"
#include "StockFilterDlg.h"
#include "StockOptionsIni.h"
/**********************************************************************/
#include "PluStockItemDlg.h"
/**********************************************************************/

CPluStockItemDlg::CPluStockItemDlg( CRecipeCSVRecord& RecipeRecord, const char* szTitle, bool bEditStockCode, bool bEditSpecialFlags, bool bApparentStock, CWnd* pParent )
	: CSSDialog(CPluStockItemDlg::IDD, pParent), m_RecipeRecord( RecipeRecord ), m_StockFilterArray ( FILTERTYPE_BROWSE_STOCK )
{
	//{{AFX_DATA_INIT(CPluStockItemDlg)
	//}}AFX_DATA_INIT

	m_strTitle = szTitle;

	m_dQtyToWeightFactor = 1.0;
	m_dStockUnitWeight = 0.0;
	m_nSubUnitsPerStockItem = 1;

	m_bEditStockCode = bEditStockCode;
	m_bEditSpecialFlags = bEditSpecialFlags;
	m_StockFilterArray.SetCheckApparentFlag( bApparentStock );
}

/**********************************************************************/

void CPluStockItemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPluStockItemDlg)
	DDX_Control(pDX, IDC_EDIT_CODE, m_editStockCode);
	DDX_Control(pDX, IDC_BUTTON_BROWSE, m_buttonBrowse);
	DDX_Control(pDX, IDC_EDIT_DESCRIPTION, m_editDescription);
	DDX_Control(pDX, IDC_STATIC_WEIGHT, m_staticWeight);
	DDX_Control(pDX, IDC_STATIC_STOCKPOINT, m_staticStockpoint);
	DDX_Control(pDX, IDC_COMBO_STOCKPOINT, m_comboStockpoint);
	DDX_Control(pDX, IDC_STATIC_SALES, m_staticSalesAction);
	DDX_Control(pDX, IDC_COMBO_SALES, m_comboSalesAction);
	DDX_Control(pDX, IDOK, m_buttonOK);
	DDX_Control(pDX, IDCANCEL, m_buttonCancel);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPluStockItemDlg, CDialog)
	//{{AFX_MSG_MAP(CPluStockItemDlg)
	ON_EN_KILLFOCUS(IDC_EDIT_CODE, OnKillFocusStockCode)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, OnButtonBrowse)
	ON_EN_KILLFOCUS(IDC_EDIT_STOCK_QTY, OnKillFocusStockQty)
	ON_EN_KILLFOCUS(IDC_EDIT_STOCKSUB, OnKillFocusStockSub)
	ON_EN_KILLFOCUS(IDC_EDIT_WEIGHT, OnKillFocusWeight)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/
CEdit* CPluStockItemDlg::GetEditStockQty() { return GetEdit( IDC_EDIT_STOCK_QTY ); }
CEdit* CPluStockItemDlg::GetEditWeight() { return GetEdit( IDC_EDIT_WEIGHT ); }
/**********************************************************************/

BOOL CPluStockItemDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	SubclassEdit ( IDC_EDIT_STOCK_QTY, SS_NUM_DP, 8, "%.5f" );
	SubclassEdit ( IDC_EDIT_WEIGHT, SS_NUM_DP, 8, "%.5f" );
	m_editStockSub.SubclassDlgItem ( IDC_EDIT_STOCKSUB, this, 15, 1, FALSE );
	m_editStockCode.LimitText( Stock::StockCode.Max );
	SetWindowText ( m_strTitle );

	CArray<int, int> arrayStops;
	arrayStops.Add(5);
	arrayStops.Add(60);
	m_comboSalesAction.SetTabStops( arrayStops );
			
	UpdateStockRecordInfo();

	CString strStockCode = "";
	double dStockQty = 1.0;

	if ( m_RecipeRecord.GetSize() != 0 )
	{
		strStockCode = m_RecipeRecord.GetStockCode(0);
		dStockQty = m_RecipeRecord.GetStockQty(0);
	}
		
	PutStockQty( dStockQty );
	PutWeight( dStockQty * m_dQtyToWeightFactor );
	PutSubUnits( dStockQty );

	m_editStockCode.SetWindowText( strStockCode );

	if ( ( m_bEditStockCode == TRUE ) && ( strStockCode == "" ) )
		m_editStockCode.SetFocus();
	else
		SetDefaultQuantityFocus();

	FillStockpointCombo();
	FillSalesActionCombo();

	ShowAndEnableWindow( &m_staticStockpoint, m_bEditSpecialFlags );
	ShowAndEnableWindow( &m_comboStockpoint, m_bEditSpecialFlags );
	ShowAndEnableWindow( &m_staticSalesAction, m_bEditSpecialFlags );
	ShowAndEnableWindow( &m_comboSalesAction, m_bEditSpecialFlags );

	if ( FALSE == m_bEditSpecialFlags )
	{
		MoveControl( &m_buttonOK, 259, 84 );
		MoveControl( &m_buttonCancel, 259, 100 );
		ResizeDialog( 316, 121 );
	}
	else if ( StockOptions.GetPluRedirectToStockpointFlag() == FALSE )
	{
		m_comboStockpoint.SetCurSel(0);
		m_comboStockpoint.EnableWindow( FALSE );
	}

	return FALSE;  
}

/**********************************************************************/

void CPluStockItemDlg::FillStockpointCombo()
{
	m_comboStockpoint.AddString( "As per EPOS link" );
	m_comboStockpoint.SetItemData( 0, 0 );

	int nStartSpIdx, nEndSpIdx;
	dbStockpoint.GetSpIdxRange( DataManager.GetActiveDbIdx(), nStartSpIdx, nEndSpIdx );

	for ( int n = nStartSpIdx; n <= nEndSpIdx; n++ )
	{
		int nPos = m_comboStockpoint.AddString( dbStockpoint.GetName(n) );
		m_comboStockpoint.SetItemData( nPos, dbStockpoint.GetSpNo(n) );
	}
	
	m_comboStockpoint.EnableWindow( m_comboStockpoint.GetCount() > 1 );

	m_comboStockpoint.SetCurSel(0);
	for ( int n = 1; n < m_comboStockpoint.GetCount(); n++ )
	{
		if ( m_comboStockpoint.GetItemData(n) == m_RecipeRecord.GetSpNo() )
		{
			m_comboStockpoint.SetCurSel(n);
			break;
		}
	}
}

/**********************************************************************/

void CPluStockItemDlg::FillSalesActionCombo()
{
	m_comboSalesAction.AddItem( "Normal,(reduce apparent stock)" );
	m_comboSalesAction.AddItem( "Reverse,(increase apparent stock)" );
	m_comboSalesAction.SetCurSel( m_RecipeRecord.GetReverseSaleFlag() ? 1 : 0 );
}

/**********************************************************************/

void CPluStockItemDlg::EnableSubUnitEntry( bool bEnable )
{
	ShowAndEnableWindow( &m_editStockSub, bEnable );
}	

/**********************************************************************/

void CPluStockItemDlg::EnableWeightEntry( bool bEnable )
{
	ShowAndEnableWindow( &m_staticWeight, bEnable );
	ShowAndEnableWindow( GetEditWeight(), bEnable );
}

/**********************************************************************/

void CPluStockItemDlg::PutStockQty( double dValue )
{
	CString strValue;
	strValue.Format ( "%.5f", dValue );
	GetEditStockQty() -> SetWindowText( strValue );
}

/**********************************************************************/

void CPluStockItemDlg::PutWeight( double dValue )
{
	CString strValue;
	strValue.Format ( "%.5f", dValue );
	GetEditWeight() -> SetWindowText( strValue );
}

/**********************************************************************/

double CPluStockItemDlg::GetStockQty()
{
	CString strValue;
	GetEditStockQty() -> GetWindowText( strValue );
	return atof ( strValue );
}

/**********************************************************************/

double CPluStockItemDlg::GetWeight()
{
	CString strValue;
	GetEditWeight() -> GetWindowText( strValue );
	return atof ( strValue );
}

/**********************************************************************/

void CPluStockItemDlg::PutSubUnits( double dQuantity )
{
	CString strText;
	m_editStockSub.GetSubUnitString( dQuantity, strText );
	m_editStockSub.SetWindowText( strText );	
}

/**********************************************************************/

double CPluStockItemDlg::GetSubUnits()
{	
	double dQuantity;
	CString strText;
	m_editStockSub.GetWindowText( strText );
	m_editStockSub.GetSubUnitQuantity( strText, dQuantity );
	return dQuantity;
}

/**********************************************************************/

void CPluStockItemDlg::UpdateStockRecordInfo()
{
	CString strStockCode = "";
	if ( m_RecipeRecord.GetSize() != 0 )
		strStockCode = m_RecipeRecord.GetStockCode(0);

	CString strDescription = "";
	
	int nIndex;
	if ( DataManager.Stock.FindStockItemByCode ( strStockCode, nIndex ) == TRUE )
	{	
		CStockCSVRecord StockRecord;
		DataManager.Stock.GetAt ( nIndex, StockRecord );
		m_editStockCode.SetWindowText( strStockCode );

		CString strWeight = "";
		switch( StockRecord.GetSUWeightType() % 100 )
		{
		case WEIGHTTYPE_GALLON:		strWeight = "Gallons";		break;
		case WEIGHTTYPE_PINT:		strWeight = "Pints";		break;
		case WEIGHTTYPE_FLOUNCE:	strWeight = "Fluid Ounces";	break;
		case WEIGHTTYPE_LITRE:		strWeight = "Litres";		break;
		case WEIGHTTYPE_CLITRE:		strWeight = "Centilitres";	break;
		case WEIGHTTYPE_MLITRE:		strWeight = "Millilitres";	break;
		case WEIGHTTYPE_KILO:		strWeight = "Kilograms";	break;
		case WEIGHTTYPE_GRAM:		strWeight = "Grams";		break;
		}
		m_staticWeight.SetWindowText( strWeight );

		bool bSubUnits = StockOptions.GetStockSubUnitsFlag();
		m_dQtyToWeightFactor = StockRecord.GetQtyToWeightFactor();
		m_dStockUnitWeight = StockRecord.GetSUWeight( bSubUnits );
		m_nSubUnitsPerStockItem = StockRecord.GetSubUnits( bSubUnits );
		strDescription = StockRecord.GetDescription();
	}
	else
	{
		m_dQtyToWeightFactor = 0.0;
		m_dStockUnitWeight = 0.0;
		strDescription = "Unknown";
		m_nSubUnitsPerStockItem = 1;
	}

	m_editStockCode.EnableWindow( m_bEditStockCode );
	ShowAndEnableWindow( &m_buttonBrowse, m_bEditStockCode & ( DataManager.Stock.GetSize() > 0 ) );

	EnableSubUnitEntry( m_nSubUnitsPerStockItem >= 2 );
	EnableWeightEntry( CPriceHelpers::CompareDoubles( m_dQtyToWeightFactor, 0.0, 3 ) != 0 );

	m_editStockSub.SetMode( 15, m_nSubUnitsPerStockItem, FALSE );
	m_editDescription.SetWindowText( strDescription );
}

/**********************************************************************/

void CPluStockItemDlg::OnButtonBrowse() 
{
	CString strOldStockCode = "";
	if ( m_RecipeRecord.GetSize() != 0 )
		strOldStockCode = m_RecipeRecord.GetStockCode(0);
	
	CWaitCursor wait;
	CStockFilterDlg dlgBrowse ( m_StockFilterArray, this );

	if ( dlgBrowse.DoModal() != IDOK )
		return;

	int nStockIdx = dlgBrowse.GetStockIdx();

	if ( nStockIdx < 0 )
		return;

	CString strStockCode = DataManager.Stock.GetStockCode ( nStockIdx );
			
	if ( strStockCode != strOldStockCode )
	{
		m_RecipeRecord.RemoveAll();
		m_RecipeRecord.AddStockCode( strStockCode, 1.0 );
			
		UpdateStockRecordInfo();
		
		PutStockQty( 1.0 );
		PutWeight( m_dQtyToWeightFactor );
		PutSubUnits( 1.0 );

		m_editStockCode.SetFocus();
		m_editStockCode.SetSel ( 0, -1 );
	}
}

/**********************************************************************/

void CPluStockItemDlg::OnKillFocusStockCode() 
{
	CString strOldStockCode = "";
	if ( m_RecipeRecord.GetSize() != 0 )
		strOldStockCode = m_RecipeRecord.GetStockCode(0);
	
	CString strNewStockCode;
	m_editStockCode.GetWindowText( strNewStockCode );
	strNewStockCode = BarcodePluNoTable.ConvertStockCode ( strNewStockCode );
	
	if ( strNewStockCode != strOldStockCode )
	{
		m_RecipeRecord.RemoveAll();
		m_RecipeRecord.AddStockCode( strNewStockCode, 1.0 );

		UpdateStockRecordInfo();
		
		PutStockQty( 1.0 );
		PutWeight( m_dQtyToWeightFactor );
		PutSubUnits( 1.0 );	
		SetDefaultQuantityFocus();
	}	
}

/**********************************************************************/

void CPluStockItemDlg::UpdateFocus()
{
	CWnd* pWnd = CWnd::GetFocus();
	if ( pWnd == GetEditStockQty() )		GetEditStockQty() -> SetSel( 0, -1 );
	else if ( pWnd == &m_editStockSub )		m_editStockSub.SetSel( 0, -1 );
	else if ( pWnd == GetEditWeight() )		GetEditWeight() -> SetSel( 0, -1 );
}

/**********************************************************************/

void CPluStockItemDlg::OnKillFocusStockQty() 
{
	double dQuantity = GetStockQty();
	PutWeight( dQuantity * m_dQtyToWeightFactor );
	PutSubUnits( dQuantity );
	UpdateFocus();
}

/**********************************************************************/

void CPluStockItemDlg::OnKillFocusStockSub() 
{
	double dQuantity = GetSubUnits();
	PutStockQty( dQuantity );
	PutWeight( dQuantity * m_dQtyToWeightFactor );
	UpdateFocus();
}

/**********************************************************************/

void CPluStockItemDlg::OnKillFocusWeight() 
{
	double dQuantity = GetWeight() / m_dQtyToWeightFactor;
	PutStockQty( dQuantity );
	dQuantity = GetStockQty();
	PutWeight( dQuantity * m_dQtyToWeightFactor );
	PutSubUnits( dQuantity );
	UpdateFocus();
}

/**********************************************************************/

void CPluStockItemDlg::SetDefaultQuantityFocus()
{
	if ( m_nSubUnitsPerStockItem > 1 )
	{
		m_editStockSub.SetFocus();
		m_editStockSub.SetSel( 0, -1 );
	}
	else if ( CPriceHelpers::CompareDoubles ( m_dStockUnitWeight, 0.0, 3 ) != 0 )
	{
		GetEditWeight() -> SetFocus();
		GetEditWeight() -> SetSel( 0, -1 );
	}
	else
	{
		GetEditStockQty() -> SetFocus();
		GetEditStockQty() -> SetSel( 0, -1 );
	}
}

/**********************************************************************/

void CPluStockItemDlg::OnOK() 
{	
	m_buttonOK.SetFocus();

	CString strStockCode = "";
	if ( m_RecipeRecord.GetSize() != 0 )
	{
		strStockCode = m_RecipeRecord.GetStockCode(0);
		::TrimSpaces ( strStockCode, FALSE );
	}
		
	if ( strStockCode == "" )
	{
		Prompter.Error ( "Please enter a stock code" );
		m_editStockCode.SetFocus();
		return;
	}

	double dStockQty = GetStockQty();
	if ( dStockQty <= 0.0 )
	{
		Prompter.Error( "Please enter a non-zero stock level" );
		SetDefaultQuantityFocus();
		return;
	}

	m_RecipeRecord.RemoveAll();
	m_RecipeRecord.AddStockCode( strStockCode, dStockQty );

	if ( StockOptions.GetPluRedirectToStockpointFlag() == TRUE )
	{
		int nSel = m_comboStockpoint.GetCurSel();
		if ( ( nSel >= 0 ) && ( nSel < m_comboStockpoint.GetCount() ) )
			m_RecipeRecord.SetSpNo( m_comboStockpoint.GetItemData(nSel) );
	}

	m_RecipeRecord.SetReverseSaleFlag( m_comboSalesAction.GetCurSel() == 1 );

	EndDialog( IDOK );
}

/**********************************************************************/
