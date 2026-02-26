/**********************************************************************/
//#include "DataManager.h"
#include "Password.h"
#include "StockOptionsIni.h"
/**********************************************************************/
#include "StockEditInitialDlg.h"
/**********************************************************************/

CStockEditInitialDlg::CStockEditInitialDlg( CStockCSVRecord& StockRecord, CStockLevelsCSVRecord& StockLevels, CWnd* pParent)
	: CSSDialog(CStockEditInitialDlg::IDD, pParent), m_StockRecord( StockRecord ), m_StockLevels( StockLevels )
{
	//{{AFX_DATA_INIT(CStockEditInitialDlg)
	//}}AFX_DATA_INIT
	m_dOldQuantityOpening = 0.0;
	m_dQuantityOpening = 0.0;
	m_nTotalSubUnitsOpening = 0;
	m_dQuantityMinimum = 0.0;
	m_nTotalSubUnitsMinimum = 0;
	m_dQuantityReorder = 0.0;
	m_nTotalSubUnitsReorder = 0;
	m_dQuantityMaximum = 0.0;
	m_nTotalSubUnitsMaximum = 0;
	m_bEditOpening = TRUE;
	m_bEditMinimum = TRUE;
	m_bEditMaximum = TRUE;
	m_nSubUnits = 1;
}

/**********************************************************************/

void CStockEditInitialDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStockEditInitialDlg)
	DDX_Control(pDX, IDC_STATIC_OPENING, m_staticOpening);
	DDX_Control(pDX, IDC_STATIC_OPENING_UNIT, m_staticOpeningUnit);
	DDX_Control(pDX, IDC_STATIC_MINIMUM, m_staticMinimum);
	DDX_Control(pDX, IDC_STATIC_MINIMUM_UNIT, m_staticMinimumUnit);
	DDX_Control(pDX, IDC_STATIC_REORDER, m_staticReorder);
	DDX_Control(pDX, IDC_STATIC_REORDER_UNIT, m_staticReorderUnit);
	DDX_Control(pDX, IDC_STATIC_MAXIMUM, m_staticMaximum);
	DDX_Control(pDX, IDC_STATIC_MAXIMUM_UNIT, m_staticMaximumUnit);
	//}}AFX_DATA_MAP
	DDX_Text ( pDX, IDC_EDIT_QTY_OPENING, m_dQuantityOpening );	
	DDX_Text ( pDX, IDC_EDIT_QTY_MINIMUM, m_dQuantityMinimum );	
	DDX_Text ( pDX, IDC_EDIT_QTY_REORDER, m_dQuantityReorder );
	DDX_Text ( pDX, IDC_EDIT_QTY_MAXIMUM, m_dQuantityMaximum );	
	
	if ( pDX -> m_bSaveAndValidate )
	{
		CString strText;
		DDX_Text(pDX, IDC_EDIT_STOCKSUB_OPENING, strText );
		m_editStockSubOpening.GetSubUnitQuantity( strText, m_nTotalSubUnitsOpening );
		
		DDX_Text(pDX, IDC_EDIT_STOCKSUB_MINIMUM, strText );
		m_editStockSubMinimum.GetSubUnitQuantity( strText, m_nTotalSubUnitsMinimum );
		
		DDX_Text(pDX, IDC_EDIT_STOCKSUB_REORDER, strText );
		m_editStockSubReorder.GetSubUnitQuantity( strText, m_nTotalSubUnitsReorder );

		DDX_Text(pDX, IDC_EDIT_STOCKSUB_MAXIMUM, strText );
		m_editStockSubMaximum.GetSubUnitQuantity( strText, m_nTotalSubUnitsMaximum );
	}
	else
	{
		CString strText;
		m_editStockSubOpening.GetSubUnitString( m_nTotalSubUnitsOpening, strText );
		DDX_Text(pDX, IDC_EDIT_STOCKSUB_OPENING, strText );
		
		m_editStockSubMinimum.GetSubUnitString( m_nTotalSubUnitsMinimum, strText );
		DDX_Text(pDX, IDC_EDIT_STOCKSUB_MINIMUM, strText );
		
		m_editStockSubReorder.GetSubUnitString( m_nTotalSubUnitsReorder, strText );
		DDX_Text(pDX, IDC_EDIT_STOCKSUB_REORDER, strText );

		m_editStockSubMaximum.GetSubUnitString( m_nTotalSubUnitsMaximum, strText );
		DDX_Text(pDX, IDC_EDIT_STOCKSUB_MAXIMUM, strText );
	}
}
/**********************************************************************/

BEGIN_MESSAGE_MAP(CStockEditInitialDlg, CDialog)
	//{{AFX_MSG_MAP(CStockEditInitialDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/
CEdit* CStockEditInitialDlg::GetEditQtyOpening() { return GetEdit( IDC_EDIT_QTY_OPENING ); }
CEdit* CStockEditInitialDlg::GetEditQtyMinimum() { return GetEdit( IDC_EDIT_QTY_MINIMUM ); }
CEdit* CStockEditInitialDlg::GetEditQtyReorder() { return GetEdit( IDC_EDIT_QTY_REORDER ); }
CEdit* CStockEditInitialDlg::GetEditQtyMaximum() { return GetEdit( IDC_EDIT_QTY_MAXIMUM ); }
/**********************************************************************/

BOOL CStockEditInitialDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	SubclassEdit ( IDC_EDIT_QTY_OPENING, SS_NUM_SDP, 8, "%.5f" );
	SubclassEdit ( IDC_EDIT_QTY_MINIMUM, SS_NUM_SDP, 8, "%.5f" );
	SubclassEdit ( IDC_EDIT_QTY_REORDER, SS_NUM_SDP, 8, "%.5f" );
	SubclassEdit ( IDC_EDIT_QTY_MAXIMUM, SS_NUM_SDP, 8, "%.5f" );

	m_editStockSubOpening.SubclassDlgItem ( IDC_EDIT_STOCKSUB_OPENING, this, 11, 1, TRUE );
	m_editStockSubMinimum.SubclassDlgItem ( IDC_EDIT_STOCKSUB_MINIMUM, this, 11, 1, TRUE );
	m_editStockSubReorder.SubclassDlgItem ( IDC_EDIT_STOCKSUB_REORDER, this, 11, 1, TRUE );
	m_editStockSubMaximum.SubclassDlgItem ( IDC_EDIT_STOCKSUB_MAXIMUM, this, 11, 1, TRUE );
	
	CString strUnit;
	strUnit.Format ( "(%s)", DataManager.Stock.GetStockSubUnitName( m_StockRecord, TRUE ) );
	m_staticOpeningUnit.SetWindowText( strUnit );
	m_staticMinimumUnit.SetWindowText( strUnit );
	m_staticReorderUnit.SetWindowText( strUnit );
	m_staticMaximumUnit.SetWindowText( strUnit );

	m_nSubUnits = DataManager.Stock.GetSubUnits( m_StockRecord );
	if ( m_nSubUnits < 2 )
	{
		m_dQuantityOpening = m_StockLevels.GetOpeningQty();
		m_dOldQuantityOpening = m_dQuantityOpening;
		m_dQuantityMinimum = m_StockLevels.GetMinimumLevelQty();
		m_dQuantityReorder = m_StockLevels.GetReorderLevelQty();
		m_dQuantityMaximum = m_StockLevels.GetMaximumLevelQty();
		
		ShowAndEnableWindow( &m_editStockSubOpening, FALSE );
		ShowAndEnableWindow( &m_editStockSubMinimum, FALSE );
		ShowAndEnableWindow( &m_editStockSubReorder, FALSE );
		ShowAndEnableWindow( &m_editStockSubMaximum, FALSE );

		ShowAndEnableWindow( GetEditQtyOpening(), TRUE );
		ShowAndEnableWindow( GetEditQtyMinimum(), TRUE );
		ShowAndEnableWindow( GetEditQtyReorder(), TRUE );
		ShowAndEnableWindow( GetEditQtyMaximum(), TRUE );
	}
	else
	{
		m_StockRecord.ConvertStockLevelToTotalSubUnits ( m_StockLevels.GetOpeningQty(), m_nTotalSubUnitsOpening );
		m_StockRecord.ConvertStockLevelToTotalSubUnits ( m_StockLevels.GetMinimumLevelQty(), m_nTotalSubUnitsMinimum );
		m_StockRecord.ConvertStockLevelToTotalSubUnits ( m_StockLevels.GetReorderLevelQty(), m_nTotalSubUnitsReorder );
		m_StockRecord.ConvertStockLevelToTotalSubUnits ( m_StockLevels.GetMaximumLevelQty(), m_nTotalSubUnitsMaximum );
		
		ShowAndEnableWindow( &m_editStockSubOpening, TRUE );
		m_editStockSubOpening.SetMode( 11, m_nSubUnits, TRUE );
		ShowAndEnableWindow( &m_editStockSubMinimum, TRUE );
		m_editStockSubMinimum.SetMode( 11, m_nSubUnits, TRUE );
		ShowAndEnableWindow( &m_editStockSubReorder, TRUE );
		m_editStockSubReorder.SetMode( 11, m_nSubUnits, TRUE );
		ShowAndEnableWindow( &m_editStockSubMaximum, TRUE );
		m_editStockSubMaximum.SetMode( 11, m_nSubUnits, TRUE );

		ShowAndEnableWindow( GetEditQtyOpening(), FALSE );
		ShowAndEnableWindow( GetEditQtyMinimum(), FALSE );
		ShowAndEnableWindow( GetEditQtyReorder(), FALSE );
		ShowAndEnableWindow( GetEditQtyMaximum(), FALSE );
	}

	if ( m_StockLevels.GetStocktakeNo() != 0 )
	{
		GetEditQtyOpening() -> EnableWindow( FALSE );
		m_editStockSubOpening.EnableWindow( FALSE );
		m_staticOpeningUnit.SetWindowText( "(item is in pending stocktake)" );
		m_bEditOpening = FALSE;
	}

	if ( PasswordArray.GetEnable( PasswordTicks::EditOpening ) == FALSE )
	{
		GetEditQtyOpening() -> EnableWindow( FALSE );
		m_editStockSubOpening.EnableWindow( FALSE );
		m_bEditOpening = FALSE;
	}

	if ( StockOptions.GetOrderGenerationMethod() == ORDER_TYPE_SALES )
	{
		m_staticMinimum.ShowWindow( SW_HIDE );
		m_staticMinimumUnit.ShowWindow( SW_HIDE );
		m_staticReorder.ShowWindow( SW_HIDE );
		m_staticReorderUnit.ShowWindow( SW_HIDE );
		GetEditQtyMinimum() -> ShowWindow( SW_HIDE );
		GetEditQtyReorder() -> ShowWindow( SW_HIDE );
		m_editStockSubMinimum.ShowWindow( SW_HIDE );
		m_editStockSubReorder.ShowWindow( SW_HIDE );
		m_bEditMinimum = FALSE;
	}

	if ( PasswordArray.GetEnable( PasswordTicks::EditReorder ) == FALSE ) 
	{
		GetEditQtyMinimum() -> EnableWindow( FALSE );
		GetEditQtyReorder() -> EnableWindow( FALSE );
		GetEditQtyMaximum() -> EnableWindow( FALSE );
		m_editStockSubMinimum.EnableWindow( FALSE );
		m_editStockSubReorder.EnableWindow( FALSE );
		m_editStockSubMaximum.EnableWindow( FALSE );
		m_bEditMinimum = FALSE;
	}
	
	CString strText; 
	strText.Format ( "Edit Stock Levels %s %s",
		m_StockRecord.GetStockCode(),
		m_StockRecord.GetDescription() );
	
	SetWindowText(strText);

	UpdateData( FALSE );

	MoveControl( &m_editStockSubOpening, 73, 7 );
	MoveControl( &m_editStockSubMinimum, 73, 25 );
	MoveControl( &m_editStockSubReorder, 73, 43 );
	MoveControl( &m_editStockSubMaximum, 73, 61 );

	if ( StockOptions.GetOrderGenerationMethod() == ORDER_TYPE_SALES )
	{
		MoveControl( &m_staticMaximum, 8, 25 );
		MoveControl( GetEditQtyMaximum(), 73, 25 );
		MoveControl( &m_editStockSubMaximum, 73, 25 );
		MoveControl( &m_staticMaximumUnit, 126, 26 );
	}

	return TRUE;  
}

/**********************************************************************/

void CStockEditInitialDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == FALSE )
		return;

	double dNewQuantityOpening;
	double dNewQuantityMinimum;
	double dNewQuantityReorder;
	double dNewQuantityMaximum;

	if ( m_nSubUnits < 2 )
	{
		dNewQuantityOpening = m_dQuantityOpening;
		dNewQuantityMinimum = m_dQuantityMinimum;
		dNewQuantityReorder = m_dQuantityReorder;
		dNewQuantityMaximum = m_dQuantityMaximum;
	}
	else
	{
		m_StockRecord.ConvertTotalSubUnitsToStockLevel( m_nTotalSubUnitsOpening, dNewQuantityOpening );
		m_StockRecord.ConvertTotalSubUnitsToStockLevel( m_nTotalSubUnitsMinimum, dNewQuantityMinimum );
		m_StockRecord.ConvertTotalSubUnitsToStockLevel( m_nTotalSubUnitsReorder, dNewQuantityReorder );
		m_StockRecord.ConvertTotalSubUnitsToStockLevel( m_nTotalSubUnitsMaximum, dNewQuantityMaximum );
	}
	
	if ( m_bEditOpening == TRUE ) 
	{	
		if ( dNewQuantityOpening != m_dOldQuantityOpening )
		{
			m_StockLevels.SetOpeningQty( dNewQuantityOpening );

			CSSFile fileOpeningTemp;
			CFilenameUpdater FnUp ( SysFiles::OpeningTemp, DataManager.GetActiveDbIdx() );
			if ( fileOpeningTemp.Open( FnUp.GetFilenameToUse(), "ab" ) == TRUE )
			{
				CCSV csv;
				csv.Add( m_StockLevels.GetReferenceNo() );
				csv.Add( m_StockRecord.GetStockCode() );
				csv.Add( m_StockRecord.GetDescription() );
				csv.Add( DataManager.Category.GetText ( m_StockRecord.GetCategory() ) );
				csv.Add( DataManager.Stock.GetSubUnits( m_StockRecord ) );
				csv.Add( dNewQuantityOpening, 5 );
				fileOpeningTemp.WriteLine( csv.GetLine() );
			}
		}
	}

	if ( m_bEditMinimum == TRUE )	m_StockLevels.SetMinimumLevelQty( dNewQuantityMinimum );
	if ( m_bEditMinimum == TRUE )	m_StockLevels.SetReorderLevelQty( dNewQuantityReorder );
	if ( m_bEditMaximum == TRUE )	m_StockLevels.SetMaximumLevelQty( dNewQuantityMaximum );
	
	EndDialog ( IDOK );
}

/**********************************************************************/

