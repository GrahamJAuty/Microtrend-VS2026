/**********************************************************************/
#include "..\CommonEcrStock\ColourEditDlg.h"
#include "StockOptionsIni.h"
#include "WMNumbers.h"
/**********************************************************************/
#include "PropPageOptionsStock.h"
/**********************************************************************/

CPropPageOptionsStock::CPropPageOptionsStock() : CSSPropertyPage(CPropPageOptionsStock::IDD)
{
	//{{AFX_DATA_INIT(CPropPageOptionsStock)
	//}}AFX_DATA_INIT
	m_nColourMode = 0;
}

/**********************************************************************/

CPropPageOptionsStock::~CPropPageOptionsStock()
{
}

/**********************************************************************/

void CPropPageOptionsStock::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPageOptionsStock)

	DDX_Control( pDX, IDC_TAB_ACTIONS, m_TabActions );

	DDX_Control(pDX, IDC_CHECK_IMPORTSTOCK_MANUAL, m_checkImportStockManual);
	DDX_Control(pDX, IDC_CHECK_EXPORTSTOCK_MANUAL, m_checkExportStockManual);
	DDX_Control(pDX, IDC_STATIC_ITEMS1, m_staticItems1);
	DDX_Control(pDX, IDC_COMBO_SUPPLIER_COUNT, m_comboSupplierCount);
	DDX_Control(pDX, IDC_CHECK_SUBUNITS, m_checkSubUnits);
	
	DDX_Control(pDX, IDC_CHECK_SIMPLEEDIT, m_checkSimpleEdit);
	DDX_Control(pDX, IDC_CHECK_DELETESTOCK, m_checkDeleteStock);
	DDX_Control(pDX, IDC_CHECK_BROWSESTOCK, m_checkBrowseStock);
	DDX_Control(pDX, IDC_CHECK_REDIRECT, m_checkRedirect);

	DDX_Control(pDX, IDC_STATIC_ORDERS1, m_staticOrders1);
	DDX_Control(pDX, IDC_COMBO_ORDER_GENERATE, m_comboOrderGenerate);
	DDX_Control(pDX, IDC_COMBO_ORDER_UNITS, m_comboOrderUnits);
	DDX_Control(pDX, IDC_STATIC_ORDERS2, m_staticOrders2);
	DDX_Control(pDX, IDC_COMBO_ORDER_SUGGEST, m_comboOrderSuggest);
	DDX_Control(pDX, IDC_CHECK_ORDER_TEMPLATE, m_checkOrderTemplates);
	DDX_Control(pDX, IDC_CHECK_RESTRICTPREVIEW, m_checkRestrictPreview);

	DDX_Control(pDX, IDC_CHECK_AUTOSTOCK, m_checkAutoStock);
	DDX_Control(pDX, IDC_CHECK_PLUENTRY, m_checkPluEntry);
	DDX_Control(pDX, IDC_CHECK_FINALISE, m_checkFinalise);
	DDX_Control(pDX, IDC_CHECK_BELOWMIN, m_checkBelowMin);
	DDX_Control(pDX, IDC_CHECK_STOCKMATE, m_checkStockmate);
	DDX_Control(pDX, IDC_COLOURBUTTON_BELOWMIN, m_buttonColourBelowMin);
	DDX_Control(pDX, IDC_CHECK_ABOVEMAX, m_checkAboveMax);
	DDX_Control(pDX, IDC_COLOURBUTTON_ABOVEMAX, m_buttonColourAboveMax);
	
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPageOptionsStock, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPageOptionsStock)
	//}}AFX_MSG_MAP
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_ACTIONS, OnTcnSelchangeTabs)
	ON_BN_CLICKED( IDC_COLOURBUTTON_BELOWMIN, OnButtonBelowMin )
	ON_BN_CLICKED( IDC_COLOURBUTTON_ABOVEMAX, OnButtonAboveMax )
	ON_MESSAGE ( WM_COLOUR_CHANGE, OnColourButtonMessage )
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPropPageOptionsStock::OnInitDialog() 
{
	GetRecordData();
	CPropertyPage::OnInitDialog();

	m_TabActions.InsertItem( 0, "Stock Items" );
	m_TabActions.InsertItem( 1, "Plu Item Links" );
	m_TabActions.InsertItem( 2, "Processed Orders" );
	m_TabActions.InsertItem( 3, "Miscellaneous" );
	m_TabActions.SetCurSel(0);

	m_comboSupplierCount.AddString( "1" );
	m_comboSupplierCount.AddString( "2" );
	m_comboSupplierCount.AddString( "3" );
	m_comboSupplierCount.AddString( "4" );
	m_comboSupplierCount.AddString( "5" );

	m_comboOrderGenerate.AddString( "Raise apparent stock to fixed level if below minimum" );
	m_comboOrderGenerate.AddString( "Order fixed amount if below minimum" );
	m_comboOrderGenerate.AddString( "Order to replace sales for specified date range" );
	
	m_comboOrderUnits.AddString( "Split delivery units between stockpoints" );
	m_comboOrderUnits.AddString( "Order separate delivery units for each stockpoint" );
	m_comboOrderUnits.AddString( "Generate separate orders for each stockpoint" );

	m_comboOrderSuggest.AddString( "Order generation method only" );
	m_comboOrderSuggest.AddString( "Order generation method, zero level orders for other items" );
	m_comboOrderSuggest.AddString( "Zero level orders for all items" );
	
	SetRecordControls();

	MoveControl( &m_checkImportStockManual, 19, 40 );
	MoveControl( &m_checkExportStockManual, 19, 54 );
	MoveControl( &m_staticItems1, 19, 84 );
	MoveControl( &m_comboSupplierCount, 71, 83 );
	MoveControl( &m_checkSubUnits, 19, 109 );
	/*****/
	MoveControl( &m_checkSimpleEdit, 19, 40 );
	MoveControl( &m_checkDeleteStock, 19, 54 );
	MoveControl( &m_checkBrowseStock, 19, 68 );
	MoveControl( &m_checkRedirect, 19, 82 );
	/*****/
	MoveControl( &m_staticOrders1, 19, 40 );
	MoveControl( &m_comboOrderGenerate, 19, 54 );
	MoveControl( &m_comboOrderUnits, 19, 74 );
	MoveControl( &m_staticOrders2, 19, 104 );
	MoveControl( &m_comboOrderSuggest, 19, 118 );
	MoveControl( &m_checkOrderTemplates, 19, 148 );
	MoveControl( &m_checkRestrictPreview, 19, 162 );
	/*****/
	MoveControl( &m_checkAutoStock, 19, 40 );
	MoveControl( &m_checkPluEntry, 19, 54 );
	MoveControl( &m_checkFinalise, 19, 68 );
	MoveControl( &m_checkStockmate, 19, 82);
	MoveControl( &m_checkBelowMin, 19, 107 );
	MoveControl( &m_buttonColourBelowMin, 162, 106 );
	MoveControl( &m_checkAboveMax, 19, 127 );
	MoveControl( &m_buttonColourAboveMax, 162, 126 );
	
	m_nActiveTab = 0;
	SelectTab();

	return TRUE;  
}

/**********************************************************************/

BOOL CPropPageOptionsStock::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	SaveRecord();
	return bResult;
}

/**********************************************************************/

BOOL CPropPageOptionsStock::OnSetActive() 
{
	GetRecordData();
	bool bResult = ( CPropertyPage::OnSetActive() != 0 );
	SetRecordControls();
	return bResult;
}

/**********************************************************************/

void CPropPageOptionsStock::OnTcnSelchangeTabs(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
	
	m_nActiveTab = m_TabActions.GetCurSel();
	if ( ( m_nActiveTab < 0 ) || ( m_nActiveTab > 3 ) )
		m_nActiveTab = 0;

	SelectTab();
}

/**********************************************************************/

void CPropPageOptionsStock::SelectTab()
{
	ShowAndEnableWindow( &m_checkImportStockManual, 0 == m_nActiveTab );
	ShowAndEnableWindow( &m_checkExportStockManual, 0 == m_nActiveTab );
	ShowAndEnableWindow( &m_staticItems1, 0 == m_nActiveTab );
	ShowAndEnableWindow( &m_comboSupplierCount, 0 == m_nActiveTab );
	ShowAndEnableWindow( &m_checkSubUnits, 0 == m_nActiveTab );
	/*****/
	ShowAndEnableWindow( &m_checkSimpleEdit, 1 == m_nActiveTab );
	ShowAndEnableWindow( &m_checkDeleteStock, 1 == m_nActiveTab );
	ShowAndEnableWindow( &m_checkBrowseStock, 1 == m_nActiveTab );
	ShowAndEnableWindow( &m_checkRedirect, 1 == m_nActiveTab );
	/*****/
	ShowAndEnableWindow( &m_staticOrders1, 2 == m_nActiveTab );
	ShowAndEnableWindow( &m_comboOrderGenerate, 2 == m_nActiveTab );
	ShowAndEnableWindow( &m_comboOrderUnits, 2 == m_nActiveTab );
	ShowAndEnableWindow( &m_staticOrders2, 2 == m_nActiveTab );
	ShowAndEnableWindow( &m_comboOrderSuggest, 2 == m_nActiveTab );
	ShowAndEnableWindow( &m_checkOrderTemplates, 2 == m_nActiveTab );
	ShowAndEnableWindow( &m_checkRestrictPreview, 2 == m_nActiveTab );
	/*****/
	ShowAndEnableWindow( &m_checkAutoStock, 3 == m_nActiveTab );
	ShowAndEnableWindow( &m_checkPluEntry, 3 == m_nActiveTab );
	ShowAndEnableWindow( &m_checkFinalise, 3 == m_nActiveTab );
	ShowAndEnableWindow( &m_checkStockmate, 3 == m_nActiveTab);
	ShowAndEnableWindow( &m_checkBelowMin, 3 == m_nActiveTab );
	ShowAndEnableWindow( &m_buttonColourBelowMin, 3 == m_nActiveTab );
	ShowAndEnableWindow( &m_checkAboveMax, 3 == m_nActiveTab );
	ShowAndEnableWindow( &m_buttonColourAboveMax, 3 == m_nActiveTab );
}

/**********************************************************************/

void CPropPageOptionsStock::GetRecordData()
{
}

/**********************************************************************/

void CPropPageOptionsStock::SaveRecord()
{
	StockOptions.SetPluCSVManualImportStockFlag( m_checkImportStockManual.GetCheck() != 0 );
	StockOptions.SetPluCSVManualExportStockFlag( m_checkExportStockManual.GetCheck() != 0);
	StockOptions.SetStockMaxSuppliersPerItem( m_comboSupplierCount.GetCurSel() + 1 );
	StockOptions.SetStockSubUnitsFlag( m_checkSubUnits.GetCheck() != 0 );

	StockOptions.SetPluSimplifiedEditingFlag ( m_checkSimpleEdit.GetCheck() != 0 );
	StockOptions.SetPluDeleteUnlinkedStockFlag( m_checkDeleteStock.GetCheck() != 0 );
	StockOptions.SetPluBrowseStockCodesFlag( m_checkBrowseStock.GetCheck() != 0 );
	StockOptions.SetPluRedirectToStockpointFlag( m_checkRedirect.GetCheck() != 0 );
	
	switch ( m_comboOrderGenerate.GetCurSel() )
	{
	case 0:		StockOptions.SetOrderGenerationMethod(0);		break;
	case 1:		StockOptions.SetOrderGenerationMethod(2);		break;
	case 2:		StockOptions.SetOrderGenerationMethod(1);		break;
	default:	StockOptions.SetOrderGenerationMethod(0);		break;
	}

	{
		int nSel = m_comboOrderUnits.GetCurSel();

		if ( ( nSel < 0 ) || ( nSel > 2 ) )
			nSel = 0;

		StockOptions.SetOrderSeparationType( nSel );
	}

	{
		int nSel = m_comboOrderSuggest.GetCurSel();

		if ( ( nSel < 0 ) || ( nSel > 2 ) )
			nSel = 0;

		StockOptions.SetOrderSuggestionMethod( nSel );
	}

	StockOptions.SetOrderTemplateFlag( m_checkOrderTemplates.GetCheck() != 0 );
	StockOptions.SetOrderRestrictPreviewFlag( m_checkRestrictPreview.GetCheck() != 0 );	

	StockOptions.SetStockAutoCreateFlag( m_checkAutoStock.GetCheck() != 0 );
	StockOptions.SetStockAddLeadingZerosFlag( m_checkPluEntry.GetCheck() != 0 );
	StockOptions.SetStockFinaliseMovementsFlag( m_checkFinalise.GetCheck() != 0 );
	StockOptions.SetStockmateLinkFlag(m_checkStockmate.GetCheck() != 0);
	StockOptions.SetStockBelowMinimumFlag( m_checkBelowMin.GetCheck() != 0 );
	StockOptions.SetStockAboveMaximumFlag( m_checkAboveMax.GetCheck() != 0 );
}

/**********************************************************************/

void CPropPageOptionsStock::SetRecordControls()
{
	int nSuppCount = StockOptions.GetStockMaxSuppliersPerItem();
	if ( ( nSuppCount < 1 ) || ( nSuppCount > 5 ) )
		nSuppCount = 1;

	m_checkImportStockManual.SetCheck( StockOptions.GetPluCSVManualImportStockFlag() );
	m_checkExportStockManual.SetCheck( StockOptions.GetPluCSVManualExportStockFlag() );
	m_comboSupplierCount.SetCurSel( nSuppCount - 1 );	
	m_checkSubUnits.SetCheck( StockOptions.GetStockSubUnitsFlag() );
	
	m_checkSimpleEdit.SetCheck( StockOptions.GetPluSimplifiedEditingFlag() );
	m_checkDeleteStock.SetCheck( StockOptions.GetPluDeleteUnlinkedStockFlag() );
	m_checkBrowseStock.SetCheck( StockOptions.GetPluBrowseStockCodesFlag() );
	m_checkRedirect.SetCheck( StockOptions.GetPluRedirectToStockpointFlag() );

	{
		int nSel = 0;

		switch ( StockOptions.GetOrderGenerationMethod() )
		{
		case 0:		nSel = 0;	break;
		case 1:		nSel = 2;	break;
		case 2:		nSel = 1;	break;
		default:	nSel = 0;	break;
		}
		
		m_comboOrderGenerate.SetCurSel( nSel );
	}

	{
		int nSel = StockOptions.GetOrderSeparationType();
		
		if ( ( nSel < 0 ) || ( nSel > 2 ) )
			nSel = 0;
		
		m_comboOrderUnits.SetCurSel( nSel );
	}

	{
		int nSel = StockOptions.GetOrderSuggestionMethod();
		
		if ( ( nSel < 0 ) || ( nSel > 2 ) )
			nSel = 0;
		
		m_comboOrderSuggest.SetCurSel( nSel );
	}

	m_checkOrderTemplates.SetCheck( StockOptions.GetOrderTemplateFlag() );
	m_checkRestrictPreview.SetCheck( StockOptions.GetOrderRestrictPreviewFlag() );

	m_checkAutoStock.SetCheck( StockOptions.GetStockAutoCreateFlag() );
	m_checkPluEntry.SetCheck( StockOptions.GetStockAddLeadingZerosFlag() );
	m_checkFinalise.SetCheck( StockOptions.GetStockFinaliseMovementsFlag() );
	m_checkStockmate.SetCheck(StockOptions.GetStockmateLinkFlag());
	m_checkBelowMin.SetCheck( StockOptions.GetStockBelowMinimumFlag() );
	m_buttonColourBelowMin.m_ColourRefBack = COLORREF( StockOptions.GetStockBelowMinimumColour() );
	m_buttonColourBelowMin.Invalidate();
	m_checkAboveMax.SetCheck( StockOptions.GetStockAboveMaximumFlag() );
	m_buttonColourAboveMax.m_ColourRefBack = COLORREF( StockOptions.GetStockAboveMaximumColour() );
	m_buttonColourAboveMax.Invalidate();
}

/**********************************************************************/

long CPropPageOptionsStock::OnColourButtonMessage( WPARAM w, LPARAM l )
{
	switch( m_nColourMode )
	{
	case 1:
		m_buttonColourBelowMin.m_ColourRefBack = l;
		m_buttonColourBelowMin.Invalidate();
		break;

	case 2:
		m_buttonColourAboveMax.m_ColourRefBack = l;
		m_buttonColourAboveMax.Invalidate();
		break;
	}
	
	return 0l;
}

/**********************************************************************/

void CPropPageOptionsStock::OnButtonBelowMin()
{
	if ( m_checkBelowMin.GetCheck() != 0 )
	{
		m_nColourMode = 1;

		int nNewColour = StockOptions.GetStockBelowMinimumColour();
		CColourEditDlg dlg( nNewColour, "Set Colour for Items Below Minimum Level", this, FALSE, this );
		if ( dlg.DoModal() == IDOK )
			nNewColour = dlg.GetColourRef();

		StockOptions.SetStockBelowMinimumColour( nNewColour );
		m_buttonColourBelowMin.m_ColourRefBack = nNewColour;
		m_buttonColourBelowMin.Invalidate();
	}
}

/**********************************************************************/

void CPropPageOptionsStock::OnButtonAboveMax()
{
	if ( m_checkAboveMax.GetCheck() != 0 )
	{
		m_nColourMode = 2;

		int nNewColour = StockOptions.GetStockAboveMaximumColour();
		CColourEditDlg dlg( nNewColour, "Set Colour for Items Above Maximum Level", this, FALSE, this );
		if ( dlg.DoModal() == IDOK )
			nNewColour = dlg.GetColourRef();
				
		StockOptions.SetStockAboveMaximumColour( nNewColour );
		m_buttonColourAboveMax.m_ColourRefBack = nNewColour;
		m_buttonColourAboveMax.Invalidate();
	}
}

/**********************************************************************/