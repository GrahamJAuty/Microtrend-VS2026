/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "NodeTypes.h"
#include "Password.h"
#include "StockEditInitialDlg.h"
#include "StockOptionsIni.h"
/**********************************************************************/
#include "PropPageStockApparent.h"
/**********************************************************************/

CPropPageStockApparent::CPropPageStockApparent() : CSSPropertyPage(CPropPageStockApparent::IDD)
{
	//{{AFX_DATA_INIT(CPropPageStockApparent)
	//}}AFX_DATA_INIT
	m_nApparentComboPos = 0;
	m_nDefaultSpIdx = 0;
	m_bSingleSpIdx = FALSE;
	m_bAllowEdit = FALSE;
	m_bNeedSave = FALSE;
	m_bReadOnly = FALSE;
}

/**********************************************************************/

CPropPageStockApparent::~CPropPageStockApparent()
{
}

/**********************************************************************/

void CPropPageStockApparent::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPageStockApparent)
	DDX_Control(pDX, IDC_COMBO_STOCKPOINT, m_comboStockpoint);
	DDX_Control(pDX, IDC_STATIC_OPENING_TEXT, m_staticOpeningText);
	DDX_Control(pDX, IDC_STATIC_MINIMUM_TEXT, m_staticMinimumText);
	DDX_Control(pDX, IDC_STATIC_MAXIMUM_TEXT, m_staticMaximumText);
	DDX_Control(pDX, IDC_STATIC_REORDER_TEXT, m_staticReorderText);
	DDX_Control(pDX, IDC_STATIC_OPENING, m_staticOpening);
	DDX_Control(pDX, IDC_STATIC_MINIMUM, m_staticMinimum);
	DDX_Control(pDX, IDC_STATIC_MAXIMUM, m_staticMaximum);
	DDX_Control(pDX, IDC_STATIC_REORDER, m_staticReorder);
	DDX_Control(pDX, IDC_STATIC_OPENING_UNIT, m_staticOpeningUnit);
	DDX_Control(pDX, IDC_STATIC_MINIMUM_UNIT, m_staticMinimumUnit);
	DDX_Control(pDX, IDC_STATIC_REORDER_UNIT, m_staticReorderUnit);
	DDX_Control(pDX, IDC_STATIC_MAXIMUM_UNIT, m_staticMaximumUnit);
	DDX_Control(pDX, IDC_BUTTON_EDIT, m_buttonEdit);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPageStockApparent, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPageStockApparent)
	ON_WM_MEASUREITEM()
	ON_CBN_SELCHANGE(IDC_COMBO_STOCKPOINT, OnSelectStockpoint)
	ON_BN_CLICKED(IDC_BUTTON_EDIT, OnButtonEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPropPageStockApparent::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	int nDbIdx = DataManager.GetActiveDbIdx();

	CWordArray arrayTypes;

	switch( PasswordArray.GetLocationAccessType() )
	{
	case LOCATION_ACCESS_SYSTEM:	
	case LOCATION_ACCESS_DB:
		arrayTypes.Add ( NODE_DATABASE);
		arrayTypes.Add ( NODE_SITE );
		arrayTypes.Add ( NODE_STOCKPOINT );
		break;

	case LOCATION_ACCESS_LOC:
		arrayTypes.Add ( NODE_STOCKPOINT );
		m_LocationSelector.SetSingleSpIdx( PasswordArray.GetLocationAccessSpIdx() );
		break;
	}

	m_LocationSelector.SetDatabaseMode ( NODE_STOCKPOINT );
	m_LocationSelector.SetSingleDbIdx ( DataManager.GetActiveDbIdx() );
	m_LocationSelector.BuildList( arrayTypes );
	m_LocationSelector.FillLocationCombo ( &m_comboStockpoint );

	m_nApparentComboPos = 0;
	for ( int n = 0; n < m_LocationSelector.GetListSize(); n++ )
	{
		CLocationSelectorEntity LocSelEntity;
		m_LocationSelector.GetListEntry( n, LocSelEntity );

		if ( LocSelEntity.GetConType() == NODE_STOCKPOINT )
		{
			if ( LocSelEntity.GetEntityIdx() == m_nDefaultSpIdx )
			{
				m_nApparentComboPos = n;
				break;
			}
		}
	}

	if ( m_bSingleSpIdx == TRUE )
		m_comboStockpoint.EnableWindow( FALSE );

	if ( StockOptions.GetOrderGenerationMethod() == ORDER_TYPE_SALES )
	{
		m_staticMinimum.ShowWindow( SW_HIDE );
		m_staticReorder.ShowWindow( SW_HIDE );
		m_staticMinimumText.ShowWindow( SW_HIDE );
		m_staticReorderText.ShowWindow( SW_HIDE );
		m_staticMinimumUnit.ShowWindow( SW_HIDE );
		m_staticReorderUnit.ShowWindow( SW_HIDE );
		MoveControl( &m_staticMaximumText, 7, 360 );
		MoveControl( &m_staticMaximum, 73, 359 );
		MoveControl( &m_staticMaximumUnit, 149, 361 );
		MoveControl( &m_buttonEdit, 150, 384 );
	}

	if ( FALSE == m_bAllowEdit )
	{
		m_staticOpeningText.ShowWindow( SW_HIDE );
		m_staticMinimumText.ShowWindow( SW_HIDE );
		m_staticReorderText.ShowWindow( SW_HIDE );
		m_staticMaximumText.ShowWindow( SW_HIDE );

		m_staticOpening.ShowWindow( SW_HIDE );
		m_staticMinimum.ShowWindow( SW_HIDE );
		m_staticReorder.ShowWindow( SW_HIDE );
		m_staticMaximum.ShowWindow( SW_HIDE );

		m_staticOpeningUnit.ShowWindow( SW_HIDE );
		m_staticMinimumUnit.ShowWindow( SW_HIDE );
		m_staticReorderUnit.ShowWindow( SW_HIDE );
		m_staticMaximumUnit.ShowWindow( SW_HIDE );

		ShowAndEnableWindow( &m_buttonEdit, FALSE );
	}
	
	m_listLevels.SubclassDlgItem ( IDC_LIST, this );
	m_listLevels.EnableColour();
	m_listLevels.InsertColumn ( 0, "", LVCFMT_RIGHT, 50 );
	m_listLevels.InsertColumn ( 1, "Type", LVCFMT_LEFT, 180 );
	m_listLevels.InsertColumn ( 2, "Qty", LVCFMT_RIGHT, 90 );
	m_listLevels.InsertColumn ( 3, "", LVCFMT_RIGHT, 50 );
	m_listLevels.InsertColumn ( 4, "Type", LVCFMT_LEFT, 180 );
	m_listLevels.InsertColumn ( 5, "Qty", LVCFMT_RIGHT, 90 );
	m_listLevels.InsertColumn ( 6, "", LVCFMT_RIGHT, 50 );
	m_listLevels.InsertColumn ( 7, "Type", LVCFMT_LEFT, 180 );
	m_listLevels.InsertColumn ( 8, "Qty", LVCFMT_RIGHT, 90 );
	m_listLevels.SetExtendedStyle ( LVS_EX_FULLROWSELECT );
	
	SetRecordControls();
	return TRUE;  
}

/**********************************************************************/

void CPropPageStockApparent::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	if ( nIDCtl == IDC_LIST )	
	{
		m_listLevels.AdjustRowHeight( lpMeasureItemStruct, 8 );
	}
}

/**********************************************************************/

BOOL CPropPageStockApparent::OnSetActive() 
{
	bool bResult = ( CPropertyPage::OnSetActive() != 0 );
	SetRecordControls();
	return bResult;
}

/**********************************************************************/

void CPropPageStockApparent::OnSelectStockpoint() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		m_nApparentComboPos = m_comboStockpoint.GetCurSel();
		SelectStockpoint();
		UpdateData ( FALSE );
	}
}

/**********************************************************************/

void CPropPageStockApparent::SelectStockpoint()
{
	CStockLevelsCSVRecord StockLevels;

	CLocationSelectorEntity LocSelEntity;
	m_LocationSelector.GetSelectedEntity( LocSelEntity );
	
	int nTableNo = LocSelEntity.GetEntityNo();
	int nConType = LocSelEntity.GetConType();
	switch ( nConType )
	{
	case NODE_DATABASE:		DataManager.Stock.GetDatabaseTotals( m_nStockIdx, m_StockRecord, StockLevels );	break;
	case NODE_SITE:			DataManager.Stock.GetSiteTotals( m_nStockIdx, m_StockRecord, nTableNo, StockLevels );	break;
	case NODE_STOCKPOINT:	DataManager.Stock.GetStockpointTotals( m_nStockIdx, m_StockRecord, nTableNo, StockLevels ); break;	
	}

	DisplayLevels ( StockLevels );

	m_staticOpening.SetWindowText( DataManager.Stock.GetStockLevelString( m_StockRecord, StockLevels.GetOpeningQty() ) );
	m_staticMinimum.SetWindowText( DataManager.Stock.GetStockLevelString( m_StockRecord, StockLevels.GetMinimumLevelQty() ) );
	m_staticReorder.SetWindowText( DataManager.Stock.GetStockLevelString( m_StockRecord, StockLevels.GetReorderLevelQty() ) );
	m_staticMaximum.SetWindowText( DataManager.Stock.GetStockLevelString( m_StockRecord, StockLevels.GetMaximumLevelQty() ) );

	if ( ( NODE_STOCKPOINT != nConType ) || ( StockLevels.GetActiveFlag() == TRUE ) ) 
		m_buttonEdit.SetWindowText( "Edit\nLevels" );
	else
		m_buttonEdit.SetWindowText( "Inactive\nItem" );

	m_buttonEdit.EnableWindow( ( NODE_STOCKPOINT == nConType ) && ( m_bAllowEdit ) && ( StockLevels.GetActiveFlag() == TRUE ) );
}

/**********************************************************************/

void CPropPageStockApparent::DisplayLevels( CStockLevelsCSVRecord& StockLevels )
{
	m_listLevels.DeleteAllItems();
	CStringArray arrayStockIn, arrayStockOut, arrayStockYTD;
	CWordArray arrayColours;

	//ADD THE STOCK INPUTS
	AddStockLevelString( arrayStockIn, "Opening Stock", StockLevels.GetOpeningQty() );
	AddStockLevelString( arrayStockIn, "Deliveries", StockLevels.GetDeliveryQty() );
	AddStockLevelString( arrayStockIn, "Free Stock", StockLevels.GetFreeQty() );
	if ( Sysset.GetMaxStockPoints() >= 2 ) 
		AddStockLevelString( arrayStockIn, "Transfer In", StockLevels.GetTransferInQty() );
		
	//ADD THE STOCK OUTPUTS
	AddStockLevelString( arrayStockOut, "Epos Sales", StockLevels.GetEposSalesQty() );
	AddStockLevelString( arrayStockOut, "Manual Sales", StockLevels.GetManualSalesQty() );
	AddStockLevelString( arrayStockOut, "Import Sales", StockLevels.GetImportSalesQty() );
	AddStockLevelString( arrayStockOut, "Recipe Sales", StockLevels.GetRecipeSalesQty() );
	AddStockLevelString( arrayStockOut, "Returns", StockLevels.GetReturnsQty() );
	AddStockLevelString( arrayStockOut, "Adjustments", StockLevels.GetAdjustmentQty() );
	AddStockLevelString( arrayStockOut, "Epos Wastage", StockLevels.GetEcrWastageQty() );
	
	if ( StockOptions.GetStockSubUnitsFlag() ) 
		AddStockLevelString( arrayStockOut, "Auto Wastage", StockLevels.GetAutoWastageQty() );
	
	if ( Sysset.GetMaxStockPoints() >= 2 ) 
		AddStockLevelString( arrayStockOut, "Transfer Out", StockLevels.GetTransferOutQty() );

	if ( ( StockLevels.GetPendingStocktakeQty() != 0.0 ) || ( StockLevels.GetStocktakeNo() != 0 ) )
		AddStockLevelString( arrayStockOut, "Pending Stocktake", StockLevels.GetPendingStocktakeQty() );
	
	//ADD THE YTD LEVELS
	AddStockLevelString ( arrayStockYTD, "YTD Sales", StockLevels.GetYTDSalesQty() );
	AddStockLevelString ( arrayStockYTD, "YTD Adjustments", StockLevels.GetYTDAdjustmentQty() );
	AddStockLevelString ( arrayStockYTD, "YTD Epos Wastage", StockLevels.GetYTDEcrWastageQty() );
		
	if ( StockOptions.GetStockSubUnitsFlag() ) 
		AddStockLevelString( arrayStockYTD, "YTD Auto Wastage", StockLevels.GetYTDAutoWastageQty() );

	int nSize1 = arrayStockIn.GetSize();
	int nSize2 = arrayStockOut.GetSize();
	int nSize3 = arrayStockYTD.GetSize();
	int nSize = nSize1;
	if ( nSize2 > nSize ) nSize = nSize2;
	if ( nSize3 > nSize ) nSize = nSize3;

	//FILL OUT THE SMALLER LISTS WITH BLANKS
	while ( arrayStockIn.GetSize() < nSize )	arrayStockIn.Add ( "" );
	while ( arrayStockOut.GetSize() < nSize )	arrayStockOut.Add ( "" );
	while ( arrayStockYTD.GetSize() < nSize )	arrayStockYTD.Add ( "" );
	while ( arrayColours.GetSize() < nSize )	arrayColours.Add(0);

	//ADD APPARENT AND EXPECTED STOCK TO CENTRE COLUMN
	arrayStockOut.Add ( "" );
	AddStockLevelString ( arrayStockOut, "Apparent Stock", StockLevels.GetApparentQty() );
	arrayStockOut.Add ( "" );
	AddStockLevelString ( arrayStockOut, "Qty on Order", StockLevels.GetQtyOnOrder() );
	AddStockLevelString ( arrayStockOut, "Total Expected", StockLevels.GetApparentQty() + StockLevels.GetQtyOnOrder() );

	//FILL OUT OTHER LISTS WITH BLANKS
	while ( arrayStockIn.GetSize() < arrayStockOut.GetSize() )
	{
		arrayStockIn.Add ( "" );
		arrayStockYTD.Add( "" );
	}
	
	//APPARENT STOCK LINE IS RED IF NEGATIVE
	arrayColours.Add(0);
	arrayColours.Add ( (StockLevels.GetApparentQty() >= 0.0) ? 0 : 12 ); 
	arrayColours.Add(0);
	arrayColours.Add(0);
	arrayColours.Add ( (StockLevels.GetApparentQty() + StockLevels.GetQtyOnOrder() >= 0.0) ? 0 : 12 ); 
	
	//DISPLAY THE LIST
	for ( int nIndex = 0; nIndex < arrayStockIn.GetSize(); nIndex++ )
	{
		CString strLine = "\t";
		
		CString strStockIn = arrayStockIn.GetAt( nIndex );
		CString strStockOut = arrayStockOut.GetAt( nIndex );
		CString strStockYTD = arrayStockYTD.GetAt( nIndex );

		if ( strStockIn != "" )
			strLine += strStockIn + "\t\t";
		else
			strLine += "\t\t\t";

		if ( strStockOut != "" )
			strLine += strStockOut + "\t\t";
		else
			strLine += "\t\t\t";

		strLine += strStockYTD;
	
		if ( strLine == "" ) strLine = " ";
		m_listLevels.InsertString( -1, strLine, arrayColours.GetAt( nIndex ) );
	}
}

/**********************************************************************/

void CPropPageStockApparent::AddStockLevelString( CStringArray& array, const char* szType, double dLevel )
{
	CCSV csvOut ( '\t' );
	csvOut.Add( szType );
	csvOut.Add( DataManager.Stock.GetStockLevelString( m_StockRecord, dLevel ) );
	array.Add ( csvOut.GetLine() );
}

/**********************************************************************/

void CPropPageStockApparent::SetRecordControls()
{
	if ( m_nStockIdx >= 0 && m_nStockIdx < DataManager.Stock.GetSize() )
		DataManager.Stock.GetAt ( m_nStockIdx, m_StockRecord );

	CString strUnit;
	strUnit.Format ( "(%s)", DataManager.Stock.GetStockSubUnitName( m_StockRecord, TRUE ) );
	m_staticOpeningUnit.SetWindowText( strUnit );
	m_staticMinimumUnit.SetWindowText( strUnit );
	m_staticReorderUnit.SetWindowText( strUnit );
	m_staticMaximumUnit.SetWindowText( strUnit );

	m_comboStockpoint.SetCurSel ( m_nApparentComboPos );
	SelectStockpoint();
}

/**********************************************************************/

void CPropPageStockApparent::Refresh()
{
	SetRecordControls();
}

/**********************************************************************/

void CPropPageStockApparent::SetStockIdx ( int nStockIdx )
{
	if ( nStockIdx >= 0 && nStockIdx < DataManager.Stock.GetSize() )
		m_nStockIdx = nStockIdx;
	else
		m_nStockIdx = -1;
}

/**********************************************************************/

void CPropPageStockApparent::OnButtonEdit() 
{
	CStockLevelsCSVRecord StockLevels;

	CLocationSelectorEntity LocSelEntity;
	m_LocationSelector.GetSelectedEntity( LocSelEntity );

	int nSpNo = LocSelEntity.GetEntityNo();

	DataManager.Stock.GetStockpointTotals( m_nStockIdx, m_StockRecord, nSpNo, StockLevels ); 	
	StockLevels.SetReferenceNo( nSpNo );

	CStockEditInitialDlg dlg ( m_StockRecord, StockLevels, this );
	
	if ( dlg.DoModal() == IDOK )
	{
		DataManager.Stock.SetStockpointTotals( m_nStockIdx, m_StockRecord, nSpNo, StockLevels );
		SelectStockpoint();
		m_bNeedSave = TRUE;
	}
}

/**********************************************************************/


