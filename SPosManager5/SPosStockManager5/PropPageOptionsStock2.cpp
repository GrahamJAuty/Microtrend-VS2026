/**********************************************************************/
#include "MaxLengths.h"
#include "StockOptionsIni.h"
#include "PropPageOptionsStock2.h"
/**********************************************************************/

CPropPageOptionsStock2::CPropPageOptionsStock2() : CSSPropertyPage(CPropPageOptionsStock2::IDD)
{
	//{{AFX_DATA_INIT(CPropPageOptionsStock2)
	//}}AFX_DATA_INIT
}

/**********************************************************************/

CPropPageOptionsStock2::~CPropPageOptionsStock2()
{
}

/**********************************************************************/

void CPropPageOptionsStock2::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPageOptionsStock2)
	
	DDX_Control( pDX, IDC_TAB_ACTIONS, m_TabActions );
	
	DDX_Control( pDX, IDC_CHECK_REF, m_checkRef );
	DDX_Control( pDX, IDC_STATIC_REF_PREFIX, m_staticRefPrefix );
	DDX_Control( pDX, IDC_EDIT_REF_PREFIX, m_editRefPrefix );
	DDX_Control( pDX, IDC_STATIC_REF_NUMBER, m_staticRefNumber );

	DDX_Control( pDX, IDC_STATIC_IMPORT, m_staticImport );
	DDX_Control( pDX, IDC_STATIC_IMPORT_CSV, m_staticImportCSV );
	DDX_Control( pDX, IDC_CHECK_IMPORTPLU, m_checkImportPLU );
	DDX_Control( pDX, IDC_CHECK_IMPORTWRAPPER, m_checkImportWrapper );
	DDX_Control( pDX, IDC_CHECK_IMPORTSHEET, m_checkImportSheet );
	DDX_Control( pDX, IDC_STATIC_IMPORT_PLU, m_staticImportPLU );
	DDX_Control( pDX, IDC_EDIT_IMPORT_PLU, m_editImportPLU );
	DDX_Control( pDX, IDC_STATIC_IMPORT_WRAPPER, m_staticImportWRP );
	DDX_Control( pDX, IDC_EDIT_IMPORT_WRAPPER, m_editImportWRP );
	DDX_Control( pDX, IDC_CHECK_IMPORT_DELVALUE_USE, m_checkImportDelValueUse );
	DDX_Control( pDX, IDC_CHECK_IMPORT_DELVALUE_SAVE, m_checkImportDelValueSave );
	DDX_Control( pDX, IDC_CHECK_IMPORT_ADJUSTVALUE_USE, m_checkImportAdjustValueUse );
	DDX_Control( pDX, IDC_CHECK_IMPORT_RETURNVALUE_USE, m_checkImportReturnValueUse );
	DDX_Control( pDX, IDC_CHECK_IMPORT_SALES_EXCEPTION, m_checkImportSalesException );

	DDX_Control( pDX, IDC_STATIC_DELIVER, m_staticDeliver );
	DDX_Control( pDX, IDC_STATIC_DELIVER_COST, m_staticDeliverCost );
	DDX_Control( pDX, IDC_COMBO_DELIVER_COST, m_comboDeliverCost );
	DDX_Control( pDX, IDC_STATIC_DELIVER_SUPPREF, m_staticDeliverSuppRef );
	DDX_Control( pDX, IDC_COMBO_DELIVER_SUPPREF, m_comboDeliverSuppRef );

	DDX_Control( pDX, IDC_STATIC_SALE_ENTRY, m_staticSaleEntry );
	DDX_Control( pDX, IDC_COMBO_SALE_ENTRY, m_comboSaleEntry );
	DDX_Control( pDX, IDC_STATIC_ADJUST_ENTRY, m_staticAdjustEntry );
	DDX_Control( pDX, IDC_COMBO_ADJUST_ENTRY, m_comboAdjustEntry );
	
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPageOptionsStock2, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPageOptionsStock2)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_ACTIONS, OnTcnSelchangeTabs)
	ON_BN_CLICKED(IDC_CHECK_REF, OnToggleEnableRef)
	ON_EN_KILLFOCUS(IDC_EDIT_REF_NUMBER, OnKillFocusNumber)
	ON_BN_CLICKED(IDC_CHECK_IMPORT_DELVALUE_USE, OnToggleImportDelValue)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/
CEdit* CPropPageOptionsStock2::GetEditRefNumber() { return GetEdit ( IDC_EDIT_REF_NUMBER ); }
/**********************************************************************/

BOOL CPropPageOptionsStock2::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	m_editRefPrefix.LimitText( MAX_LENGTH_ADHOC_PREFIX );
	SubclassEdit ( IDC_EDIT_REF_NUMBER, SS_NUM, 6, "%s" );

	m_TabActions.InsertItem( 0, "Opening" );
	m_TabActions.InsertItem( 1, "Deliveries" );
	m_TabActions.InsertItem( 2, "Returns" );
	m_TabActions.InsertItem( 3, "Adjustments" );
	m_TabActions.InsertItem( 4, "Transfers" );
	m_TabActions.InsertItem( 5, "Sales" );
	m_TabActions.InsertItem( 6, "Closing" );
	m_TabActions.InsertItem( 7, "Reorder" );
	m_TabActions.SetCurSel(0);

	m_comboDeliverCost.AddString( "Only apply new cost price to current delivery" );
	m_comboDeliverCost.AddString( "Automatically update supplier cost price" );
	m_comboDeliverCost.AddString( "Prompt to update supplier cost price" );

	m_comboDeliverSuppRef.AddString( "Retain the currently preferred supplier" );
	m_comboDeliverSuppRef.AddString( "Change the preferred supplier for future deliveries" );
	m_comboDeliverSuppRef.AddString( "Prompt to change the preferred supplier" );

	m_comboAdjustEntry.AddString( "Plu" );
	m_comboAdjustEntry.AddString( "Stock" );
	m_comboAdjustEntry.AddString( "Plu or Stock" );

	m_comboSaleEntry.AddString( "Plu" );
	m_comboSaleEntry.AddString( "Stock" );
	m_comboSaleEntry.AddString( "Plu or Stock" );

	MoveControl( &m_checkImportReturnValueUse, 244, 143 );
	MoveControl( &m_checkImportAdjustValueUse, 244, 143 );
	MoveControl( &m_checkImportSalesException, 244, 73 );
	MoveControl( &m_staticAdjustEntry, 19, 165 );
	MoveControl( &m_comboAdjustEntry, 96, 164 );
	MoveControl( &m_staticSaleEntry, 19, 131 );
	MoveControl( &m_comboSaleEntry, 96, 130 );
		
	m_nActiveTab = 0;
	SetTabControls();
	LoadTabSettings();

	return TRUE;  
}

/**********************************************************************/

BOOL CPropPageOptionsStock2::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	SaveTabSettings();
	return bResult;
}

/**********************************************************************/

BOOL CPropPageOptionsStock2::OnSetActive() 
{
	SetTabControls();
	LoadTabSettings();
	bool bResult = ( CPropertyPage::OnSetActive() != 0 );
	return bResult;
}

/**********************************************************************/

void CPropPageOptionsStock2::OnTcnSelchangeTabs(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;

	SaveTabSettings();
	
	m_nActiveTab = m_TabActions.GetCurSel();
	if ( ( m_nActiveTab < 0 ) || ( m_nActiveTab > 7 ) )
		m_nActiveTab = 0;

	SetTabControls();
	LoadTabSettings();
}

/**********************************************************************/

void CPropPageOptionsStock2::LoadTabSettings()
{
	bool bImportPlu = FALSE;
	bool bImportWrapper = FALSE;
	bool bImportSheet = FALSE;
	bool bEnable = FALSE;
	int nRefNumber = 0;
	CString strRefNumber = "";
	CString strRefPrefix = "";
	CString strPluFile = "";
	CString strWrpFile = "";

	switch( m_nActiveTab )
	{
	case STOCK3_TAB_ADHOC:
		bEnable = StockOptions.GetAutoNumAdhocFlag();
		nRefNumber = StockOptions.GetAutoNumAdhoc() + 1;
		if ( nRefNumber >= 1000000 ) nRefNumber = 1;
		strRefPrefix = StockOptions.GetAutoNumAdhocPrefix();
		bImportPlu = StockOptions.GetImportAdhocPluFlag();
		bImportWrapper = StockOptions.GetImportAdhocWrapperFlag();
		strPluFile = StockOptions.GetImportAdhocPluName();
		strWrpFile = StockOptions.GetImportAdhocWrpName();
		break;

	case STOCK3_TAB_RETURN:
		bEnable = StockOptions.GetAutoNumReturnFlag();	
		nRefNumber = StockOptions.GetAutoNumReturn() + 1;
		if ( nRefNumber >= 1000000 ) nRefNumber = 1;
		strRefPrefix = StockOptions.GetAutoNumReturnPrefix();
		bImportPlu = StockOptions.GetImportReturnPluFlag();
		bImportWrapper = StockOptions.GetImportReturnWrapperFlag();
		strPluFile = StockOptions.GetImportReturnPluName();
		strWrpFile = StockOptions.GetImportReturnWrpName();
		break;

	case STOCK3_TAB_ADJUST:
		bEnable = StockOptions.GetAutoNumAdjustFlag();
		nRefNumber = StockOptions.GetAutoNumAdjust() + 1;
		if ( nRefNumber >= 1000000 ) nRefNumber = 1;
		bImportPlu = StockOptions.GetImportAdjustPluFlag();
		strRefPrefix = StockOptions.GetAutoNumAdjustPrefix();
		strPluFile = StockOptions.GetImportAdjustPluName();
		break;

	case STOCK3_TAB_TRANSFER:
		bEnable = StockOptions.GetAutoNumXferFlag();
		nRefNumber = StockOptions.GetAutoNumXfer() + 1;
		if ( nRefNumber >= 1000000 ) nRefNumber = 1;
		strRefPrefix = StockOptions.GetAutoNumXferPrefix();
		bImportPlu = StockOptions.GetImportXferPluFlag();
		bImportWrapper = StockOptions.GetImportXferWrapperFlag();
		strPluFile = StockOptions.GetImportXferPluName();
		strWrpFile = StockOptions.GetImportXferWrpName();
		break;

	case STOCK3_TAB_OPENING:
		bImportPlu = StockOptions.GetImportOpeningPluFlag();
		bImportWrapper = StockOptions.GetImportOpeningWrapperFlag();
		bImportSheet = StockOptions.GetImportOpeningSheetFlag();
		strPluFile = StockOptions.GetImportOpeningPluName();
		strWrpFile = StockOptions.GetImportOpeningWrpName();
		break;

	case STOCK3_TAB_CLOSING:
		bImportPlu = StockOptions.GetImportClosingPluFlag();
		bImportWrapper = StockOptions.GetImportClosingWrapperFlag();
		bImportSheet = StockOptions.GetImportClosingSheetFlag();
		strPluFile = StockOptions.GetImportClosingPluName();
		strWrpFile = StockOptions.GetImportClosingWrpName();
		break;

	case STOCK3_TAB_SALES:
		bImportPlu = StockOptions.GetImportSalesPluFlag();
		bImportWrapper = StockOptions.GetImportSalesWrapperFlag();
		strPluFile = StockOptions.GetImportSalesPluName();
		strWrpFile = StockOptions.GetImportSalesWrpName();
		break;

	case STOCK3_TAB_REORDER:
		bImportPlu = StockOptions.GetImportReorderPluFlag();
		bImportWrapper = StockOptions.GetImportReorderWrapperFlag();
		strPluFile = StockOptions.GetImportReorderPluName();
		strWrpFile = StockOptions.GetImportReorderWrpName();
		break;
	}

	switch ( m_nActiveTab )
	{
	case STOCK3_TAB_ADHOC:
	case STOCK3_TAB_RETURN:
	case STOCK3_TAB_ADJUST:
	case STOCK3_TAB_TRANSFER:
		m_checkRef.SetCheck( bEnable );
		strRefNumber.Format ( "%6.6d", nRefNumber );
		GetEditRefNumber() -> SetWindowText( strRefNumber );
		GetEditRefNumber() -> EnableWindow( bEnable );
		m_editRefPrefix.SetWindowText( strRefPrefix );
		m_editRefPrefix.EnableWindow( bEnable );
		break;
	}

	m_checkImportPLU.SetCheck( bImportPlu );
	m_checkImportWrapper.SetCheck( bImportWrapper );
	m_checkImportSheet.SetCheck( bImportSheet );
	m_editImportPLU.SetWindowText( strPluFile );
	m_editImportWRP.SetWindowText( strWrpFile );

	switch ( m_nActiveTab )
	{
	case STOCK3_TAB_ADHOC:
		{
			int nComboType1 = StockOptions.GetStockAdhocPriceChangeType();
			if ( ( nComboType1 < 0 ) || ( nComboType1 > 2 ) )
				nComboType1 = 0;

			m_comboDeliverCost.SetCurSel( nComboType1 );

			int nComboType2 = StockOptions.GetStockAdhocSupplierChangeType();
			if ( ( nComboType2 < 0 ) || ( nComboType2 > 2 ) )
				nComboType2 = 0;

			m_comboDeliverSuppRef.SetCurSel( nComboType2 );

			m_checkImportDelValueUse.SetCheck( StockOptions.GetImportAdhocValueFlag() );
			m_checkImportDelValueSave.SetCheck( StockOptions.GetImportAdhocCostFlag() );
			m_checkImportDelValueSave.EnableWindow( StockOptions.GetImportAdhocValueFlag() );

		}
		break;

	case STOCK3_TAB_RETURN:
		m_checkImportReturnValueUse.SetCheck( StockOptions.GetImportReturnValueFlag() );
		break;

	case STOCK3_TAB_ADJUST:
		m_checkImportAdjustValueUse.SetCheck( StockOptions.GetImportAdjustValueFlag() );

		switch( StockOptions.GetStockAdjustmentsEntryType() )
		{
		case 0:		m_comboAdjustEntry.SetCurSel(1);	break;
		case 2:		m_comboAdjustEntry.SetCurSel(2);	break;
		default:	m_comboAdjustEntry.SetCurSel(0);	break;
		}

		break;

	case STOCK3_TAB_SALES:
		m_checkImportSalesException.SetCheck( StockOptions.GetImportSalesExceptionFlag() );

		switch( StockOptions.GetStockSalesEntryType() )
		{
		case 1:		m_comboSaleEntry.SetCurSel(1);	break;
		case 2:		m_comboSaleEntry.SetCurSel(2);	break;
		default:	m_comboSaleEntry.SetCurSel(0);	break;
		}

		break;
	}
}

/**********************************************************************/

void CPropPageOptionsStock2::SaveTabSettings()
{
	bool bImportPLU = FALSE;
	bool bImportWrapper = FALSE;
	bool bImportSheet = FALSE;
	bool bRefEnable = FALSE;
	int nRefNumber = 0;
	CString strRefNumber = "";
	CString strRefPrefix = "";

	CString strPluFile, strWrpFile;
	m_editImportPLU.GetWindowText( strPluFile );
	m_editImportWRP.GetWindowText( strWrpFile );

	switch( m_nActiveTab )
	{
	case STOCK3_TAB_ADHOC:
	case STOCK3_TAB_RETURN:
	case STOCK3_TAB_ADJUST:
	case STOCK3_TAB_TRANSFER:
		bRefEnable = ( m_checkRef.GetCheck() != 0 );
		GetEditRefNumber() -> GetWindowText( strRefNumber );
		::TrimSpaces( strRefNumber, FALSE );
		nRefNumber = atoi( strRefNumber );
		if ( 0 == nRefNumber ) nRefNumber = 1;
		m_editRefPrefix.GetWindowText( strRefPrefix );
		break;
	}

	bImportPLU = ( m_checkImportPLU.GetCheck() != 0 );
	bImportWrapper = ( m_checkImportWrapper.GetCheck() != 0 );
		bImportSheet = ( m_checkImportSheet.GetCheck() != 0 );
	
	switch( m_nActiveTab )
	{
	case STOCK3_TAB_ADHOC:
		StockOptions.SetAutoNumAdhocFlag( bRefEnable );
		StockOptions.SetAutoNumAdhocPrefix( strRefPrefix );
		StockOptions.SetAutoNumAdhoc( nRefNumber - 1 );
		StockOptions.SetStockAdhocPriceChangeType( m_comboDeliverCost.GetCurSel() );
		StockOptions.SetStockAdhocSupplierChangeType( m_comboDeliverSuppRef.GetCurSel() );
		StockOptions.SetImportAdhocPluFlag( bImportPLU );
		StockOptions.SetImportAdhocWrapperFlag( bImportWrapper );
		StockOptions.SetImportAdhocValueFlag( m_checkImportDelValueUse.GetCheck() != 0 );
		StockOptions.SetImportAdhocCostFlag( m_checkImportDelValueSave.GetCheck() != 0 );
		StockOptions.SetImportAdhocPluName( strPluFile );
		StockOptions.SetImportAdhocWrpName( strWrpFile );
		break;

	case STOCK3_TAB_RETURN:
		StockOptions.SetAutoNumReturnFlag( bRefEnable );
		StockOptions.SetAutoNumReturnPrefix( strRefPrefix );
		StockOptions.SetAutoNumReturn( nRefNumber - 1);
		StockOptions.SetImportReturnPluFlag( bImportPLU );
		StockOptions.SetImportReturnWrapperFlag( bImportWrapper );
		StockOptions.SetImportReturnValueFlag( m_checkImportReturnValueUse.GetCheck() != 0 );
		StockOptions.SetImportReturnPluName( strPluFile );
		StockOptions.SetImportReturnWrpName( strWrpFile );
		break;

	case STOCK3_TAB_ADJUST:
		StockOptions.SetAutoNumAdjustFlag( bRefEnable );
		StockOptions.SetAutoNumAdjustPrefix( strRefPrefix );
		StockOptions.SetAutoNumAdjust( nRefNumber - 1 );
		StockOptions.SetImportAdjustPluFlag( bImportPLU );
		StockOptions.SetImportAdjustValueFlag( m_checkImportAdjustValueUse.GetCheck() != 0 );
		StockOptions.SetImportAdjustPluName( strPluFile );

		switch( m_comboAdjustEntry.GetCurSel() )
		{
		case 1:		StockOptions.SetStockAdjustmentsEntryType(0);	break;
		case 2:		StockOptions.SetStockAdjustmentsEntryType(2);	break;
		default:	StockOptions.SetStockAdjustmentsEntryType(1);	break;
		}

		break;

	case STOCK3_TAB_TRANSFER:
		StockOptions.SetAutoNumXferFlag( bRefEnable );
		StockOptions.SetAutoNumXferPrefix( strRefPrefix );
		StockOptions.SetAutoNumXfer( nRefNumber - 1 );
		StockOptions.SetImportXferPluFlag( bImportPLU );
		StockOptions.SetImportXferWrapperFlag( bImportWrapper );
		StockOptions.SetImportXferPluName( strPluFile );
		StockOptions.SetImportXferWrpName( strWrpFile );
		break;

	case STOCK3_TAB_OPENING:
		StockOptions.SetImportOpeningPluFlag( bImportPLU );
		StockOptions.SetImportOpeningWrapperFlag( bImportWrapper );
		StockOptions.SetImportOpeningSheetFlag( bImportSheet );
		StockOptions.SetImportOpeningPluName( strPluFile );
		StockOptions.SetImportOpeningWrpName( strWrpFile );
		break;

	case STOCK3_TAB_CLOSING:
		StockOptions.SetImportClosingPluFlag( bImportPLU );
		StockOptions.SetImportClosingWrapperFlag( bImportWrapper );
		StockOptions.SetImportClosingSheetFlag( bImportSheet );
		StockOptions.SetImportClosingPluName( strPluFile );
		StockOptions.SetImportClosingWrpName( strWrpFile );
		break;

	case STOCK3_TAB_SALES:
		StockOptions.SetImportSalesPluFlag( bImportPLU );
		StockOptions.SetImportSalesWrapperFlag( bImportWrapper );
		StockOptions.SetImportSalesExceptionFlag( m_checkImportSalesException.GetCheck() != 0 );
		StockOptions.SetImportSalesPluName( strPluFile );
		StockOptions.SetImportSalesWrpName( strWrpFile );

		switch( m_comboSaleEntry.GetCurSel() )
		{
		case 1:		StockOptions.SetStockSalesEntryType(1);	break;
		case 2:		StockOptions.SetStockSalesEntryType(2);	break;
		default:	StockOptions.SetStockSalesEntryType(0);	break;
		}

		break;

	case STOCK3_TAB_REORDER:
		StockOptions.SetImportReorderPluFlag( bImportPLU );
		StockOptions.SetImportReorderWrapperFlag( bImportWrapper );
		StockOptions.SetImportReorderPluName( strPluFile );
		StockOptions.SetImportReorderWrpName( strWrpFile );
		break;
	}
}

/**********************************************************************/

void CPropPageOptionsStock2::SetTabControls()
{
	//SPECIAL OPTIONS FOR DELIVERY
	bool bDelivery = ( STOCK3_TAB_ADHOC == m_nActiveTab );
	ShowAndEnableWindow( &m_staticDeliver,				bDelivery );
	ShowAndEnableWindow( &m_staticDeliverCost,			bDelivery );
	ShowAndEnableWindow( &m_comboDeliverCost,			bDelivery );
	ShowAndEnableWindow( &m_staticDeliverSuppRef,		bDelivery );
	ShowAndEnableWindow( &m_comboDeliverSuppRef,		bDelivery );
	ShowAndEnableWindow( &m_checkImportDelValueUse,		bDelivery );
	ShowAndEnableWindow( &m_checkImportDelValueSave,	bDelivery );

	//SPECIAL IMPORT OPTIONS FOR RETURNS, ADJUSTMENTS, SALES
	ShowAndEnableWindow( &m_checkImportReturnValueUse,	STOCK3_TAB_RETURN == m_nActiveTab );
	ShowAndEnableWindow( &m_checkImportAdjustValueUse,	STOCK3_TAB_ADJUST == m_nActiveTab );
	ShowAndEnableWindow( &m_staticAdjustEntry,			STOCK3_TAB_ADJUST == m_nActiveTab );
	ShowAndEnableWindow( &m_comboAdjustEntry,			STOCK3_TAB_ADJUST == m_nActiveTab );
	ShowAndEnableWindow( &m_checkImportSalesException,	STOCK3_TAB_SALES == m_nActiveTab );
	ShowAndEnableWindow( &m_staticSaleEntry,			STOCK3_TAB_SALES == m_nActiveTab );
	ShowAndEnableWindow( &m_comboSaleEntry,				STOCK3_TAB_SALES == m_nActiveTab );
	
	//AUTO NUM OPTIONS ARE NOT AVAILABLE FOR OPENING, CLOSING, SALES & REORDER
	bool bAutoNum = FALSE;
	switch( m_nActiveTab )
	{
	case STOCK3_TAB_ADHOC:
	case STOCK3_TAB_RETURN:
	case STOCK3_TAB_ADJUST:
	case STOCK3_TAB_TRANSFER:
		bAutoNum = TRUE;
		break;
	}

	ShowAndEnableWindow( &m_checkRef,			bAutoNum );
	ShowAndEnableWindow( &m_staticRefPrefix,	bAutoNum );
	ShowAndEnableWindow( &m_editRefPrefix,		bAutoNum );
	ShowAndEnableWindow( &m_staticRefNumber,	bAutoNum );
	ShowAndEnableWindow( GetEditRefNumber(),	bAutoNum );

	bool bImportWrapper = TRUE;
	bool bImportSheet = FALSE;

	switch( m_nActiveTab )
	{
	case STOCK3_TAB_ADJUST:
		bImportWrapper = FALSE;
		break;

	case STOCK3_TAB_OPENING:
	case STOCK3_TAB_CLOSING:
		bImportSheet = TRUE;
		break;
	}

	//WRAPPER OPTIONS ARE NOT AVAILABLE FOR ADJUSTMENTS
	ShowAndEnableWindow( &m_checkImportWrapper,	bImportWrapper );
	ShowAndEnableWindow( &m_checkImportSheet,	bImportSheet );
	ShowAndEnableWindow( &m_staticImportWRP,	bImportWrapper );
	ShowAndEnableWindow( &m_editImportWRP,		bImportWrapper );

	//POSITION OF IMPORT OPTIONS
	switch( m_nActiveTab )
	{
	case STOCK3_TAB_ADHOC:
	case STOCK3_TAB_RETURN:
	case STOCK3_TAB_TRANSFER:
	case STOCK3_TAB_ADJUST:
		MoveControl( &m_staticImport, 97, 111 );
		MoveControl( &m_staticImportCSV, 19, 125 );
		MoveControl( &m_staticImportPLU, 19, 143 );
		MoveControl( &m_editImportPLU, 96, 142 );
		MoveControl( &m_staticImportWRP, 19, 161 );
		MoveControl( &m_editImportWRP, 96, 160 );
		MoveControl( &m_checkImportPLU, 96, 125 );
		MoveControl( &m_checkImportWrapper, 126, 125 );
		MoveControl( &m_checkImportSheet, 171, 125 );
		break;

	case STOCK3_TAB_OPENING:
	case STOCK3_TAB_CLOSING:
	case STOCK3_TAB_SALES:
	case STOCK3_TAB_REORDER:
	default:
		MoveControl( &m_staticImport, 97, 41 );
		MoveControl( &m_staticImportCSV, 19, 55 );
		MoveControl( &m_staticImportPLU, 19, 73 );
		MoveControl( &m_editImportPLU, 96, 72 );
		MoveControl( &m_staticImportWRP, 19, 91 );
		MoveControl( &m_editImportWRP, 96, 90 );
		MoveControl( &m_checkImportPLU, 96, 55 );
		MoveControl( &m_checkImportWrapper, 126, 55 );
		MoveControl( &m_checkImportSheet, 171, 55 );
		break;
	}
}

/**********************************************************************/

void CPropPageOptionsStock2::OnToggleEnableRef()
{
	switch ( m_nActiveTab )
	{
	case STOCK3_TAB_ADHOC:
	case STOCK3_TAB_RETURN:
	case STOCK3_TAB_ADJUST:
	case STOCK3_TAB_TRANSFER:
		{
			bool bEnable = ( m_checkRef.GetCheck() != 0 );
			GetEditRefNumber() -> EnableWindow( bEnable );
			m_editRefPrefix.EnableWindow( bEnable );
		}
		break;
	}
}

/**********************************************************************/

void CPropPageOptionsStock2::OnToggleImportDelValue()
{
	switch ( m_nActiveTab )
	{
	case STOCK3_TAB_ADHOC:
		m_checkImportDelValueSave.EnableWindow( m_checkImportDelValueUse.GetCheck() != 0 );
		break;
	}
}

/**********************************************************************/

void CPropPageOptionsStock2::OnKillFocusNumber() 
{
	switch ( m_nActiveTab )
	{
	case STOCK3_TAB_ADHOC:
	case STOCK3_TAB_RETURN:
	case STOCK3_TAB_ADJUST:
	case STOCK3_TAB_TRANSFER:
		{
			CString str;
			GetEditRefNumber() -> GetWindowText( str );
			::TrimSpaces( str, FALSE );
			::AddLeading( str, 6, '0' );
			GetEditRefNumber() -> SetWindowText( str );
		}
		break;
	}
}

/**********************************************************************/
