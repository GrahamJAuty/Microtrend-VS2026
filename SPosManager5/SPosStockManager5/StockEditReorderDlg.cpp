/**********************************************************************/
 
/**********************************************************************/
//#include "DataManager.h"
#include "StockOptionsIni.h"
/**********************************************************************/
#include "StockEditReorderDlg.h"
/**********************************************************************/

CStockEditReorderDlg::CStockEditReorderDlg( CSSListCtrlVirtual& StockList, CStockFilterArray* pFilterArray, CWnd* pParent)
	: CSSDialog(CStockEditReorderDlg::IDD, pParent), m_StockList ( StockList )
{
	//{{AFX_DATA_INIT(CStockEditReorderDlg)
	//}}AFX_DATA_INIT
	m_pFilterArray = pFilterArray;
	dbStockpoint.FindStockpointByNumber ( m_pFilterArray -> GetDbNo(), m_pFilterArray -> GetSpNo(), m_nSpIdx );
	m_dQuantityMinimum = 0.0;
	m_nTotalSubUnitsMinimum = 0;
	m_dQuantityReorder = 0.0;
	m_nTotalSubUnitsReorder = 0;
	m_dQuantityMaximum = 0.0;
	m_nTotalSubUnitsMaximum = 0;
	m_bAllowReorder = ( StockOptions.GetOrderGenerationMethod() != ORDER_TYPE_SALES );
}

/**********************************************************************/

void CStockEditReorderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStockEditReorderDlg)
	//}}AFX_DATA_MAP
	DDX_Control( pDX, IDC_STATIC_LEVELS, m_staticLevels );
	DDX_Control( pDX, IDC_STATIC_MINIMUM, m_staticMinimum );
	DDX_Control( pDX, IDC_STATIC_REORDER, m_staticReorder );
	DDX_Control( pDX, IDC_STATIC_MAXIMUM, m_staticMaximum );
	DDX_Control( pDX, IDC_SPIN, m_SpinButton );
	DDX_Control( pDX, IDC_DEFAULT_BUTTON, m_buttonDefault );
	DDX_Control( pDX, IDOK, m_buttonOK );
	DDX_Control( pDX, IDCANCEL, m_buttonCancel );

	DDX_Text ( pDX, IDC_EDIT_QTY_MINIMUM, m_dQuantityMinimum );	
	DDX_Text ( pDX, IDC_EDIT_QTY_REORDER, m_dQuantityReorder );
	DDX_Text ( pDX, IDC_EDIT_QTY_MAXIMUM, m_dQuantityMaximum );
	
	if ( pDX -> m_bSaveAndValidate )
	{
		CString strText;
		DDX_Text(pDX, IDC_EDIT_STOCKSUB_MINIMUM, strText );
		m_editStockSubMinimum.GetSubUnitQuantity( strText, m_nTotalSubUnitsMinimum );

		if ( TRUE == m_bAllowReorder )
		{
			DDX_Text(pDX, IDC_EDIT_STOCKSUB_REORDER, strText );
			m_editStockSubReorder.GetSubUnitQuantity( strText, m_nTotalSubUnitsReorder );
		}

		DDX_Text(pDX, IDC_EDIT_STOCKSUB_MAXIMUM, strText );
		m_editStockSubMaximum.GetSubUnitQuantity( strText, m_nTotalSubUnitsMaximum );
	}
	else
	{
		CString strText;
		m_editStockSubMinimum.GetSubUnitString( m_nTotalSubUnitsMinimum, strText );
		DDX_Text(pDX, IDC_EDIT_STOCKSUB_MINIMUM, strText );
		
		if ( TRUE == m_bAllowReorder )
		{
			m_editStockSubReorder.GetSubUnitString( m_nTotalSubUnitsReorder, strText );
			DDX_Text(pDX, IDC_EDIT_STOCKSUB_REORDER, strText );
		}

		m_editStockSubMaximum.GetSubUnitString( m_nTotalSubUnitsMaximum, strText );
		DDX_Text(pDX, IDC_EDIT_STOCKSUB_MAXIMUM, strText );
	}
}
/**********************************************************************/

BEGIN_MESSAGE_MAP(CStockEditReorderDlg, CDialog)
	//{{AFX_MSG_MAP(CStockEditReorderDlg)
	ON_BN_CLICKED(IDC_DEFAULT_BUTTON, OnDefaultButton)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN, OnSpinButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/
CEdit* CStockEditReorderDlg::GetEditStockQtyMinimum() { return GetEdit( IDC_EDIT_QTY_MINIMUM ); }
CEdit* CStockEditReorderDlg::GetEditStockQtyReorder() { return GetEdit( IDC_EDIT_QTY_REORDER ); }
CEdit* CStockEditReorderDlg::GetEditStockQtyMaximum() { return GetEdit( IDC_EDIT_QTY_MAXIMUM ); }
/**********************************************************************/

BOOL CStockEditReorderDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	SubclassEdit ( IDC_EDIT_QTY_MINIMUM, SS_NUM_SDP, 8, "%.5f" );
	SubclassEdit ( IDC_EDIT_QTY_REORDER, SS_NUM_SDP, 8, "%.5f" );
	SubclassEdit ( IDC_EDIT_QTY_MAXIMUM, SS_NUM_SDP, 8, "%.5f" );

	m_editStockSubMinimum.SubclassDlgItem ( IDC_EDIT_STOCKSUB_MINIMUM, this, 11, 1, TRUE );
	m_editStockSubReorder.SubclassDlgItem ( IDC_EDIT_STOCKSUB_REORDER, this, 11, 1, TRUE );
	m_editStockSubMaximum.SubclassDlgItem ( IDC_EDIT_STOCKSUB_MAXIMUM, this, 11, 1, TRUE );
	
	SelectStockRecord();
	UpdateData( FALSE );

	MoveControl( &m_editStockSubMinimum, 60, 22 );
	MoveControl( &m_editStockSubReorder, 60, 42 );
	MoveControl( &m_editStockSubMaximum, 60, 62 );

	if ( FALSE == m_bAllowReorder )
	{
		ResizeControl( &m_staticLevels, 258, 60 );
		MoveControl( &m_staticMaximum, 20, 44 );
		MoveControl( GetEditStockQtyMaximum(), 60, 42 );
		MoveControl( &m_editStockSubMaximum, 60, 42 );
		MoveControl( &m_SpinButton, 7, 75 );
		MoveControl( &m_buttonDefault, 67, 75 );
		MoveControl( &m_buttonOK, 162, 75 );
		MoveControl( &m_buttonCancel, 215, 75 );
		m_staticReorder.ShowWindow( SW_HIDE );
		ResizeDialog( 272, 96 );
	}

	return TRUE;  
}

/**********************************************************************/

void CStockEditReorderDlg::OnDefaultButton() 
{
	SpinNext();
}

/**********************************************************************/

void CStockEditReorderDlg::OnSpinButton(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	
	if ( pNMUpDown -> iDelta > 0 )
		SpinPrevious();
	else
		SpinNext();
	
	*pResult = 0;
}

/**********************************************************************/

void CStockEditReorderDlg::SpinNext() 
{
	if ( UpdateData ( TRUE ) == FALSE )
		return;
	
	if ( m_pFilterArray -> HasNext() == FALSE )
		return;
		
	ProcessLevels();
	m_pFilterArray -> Next();
	SelectStockRecord();
	m_StockList.SetCurSel( m_pFilterArray -> GetArrayIdx() );
}

/**********************************************************************/

void CStockEditReorderDlg::SpinPrevious() 
{
	if ( UpdateData ( TRUE ) == FALSE )
		return;
	
	if ( m_pFilterArray -> HasPrevious() == FALSE )
		return;
		
	ProcessLevels();
	m_pFilterArray -> Previous();
	SelectStockRecord();
	m_StockList.SetCurSel( m_pFilterArray -> GetArrayIdx() );
}

/**********************************************************************/

void CStockEditReorderDlg::SelectStockRecord()
{
	CStockCSVRecord StockRecord;
	int nStockIdx = m_pFilterArray -> GetStockItemIndex(); 
	DataManager.Stock.GetAt ( nStockIdx, StockRecord );

	m_dQuantityMinimum = 0.0;
	m_nTotalSubUnitsMinimum = 0;
	m_dQuantityReorder = 0.0;
	m_nTotalSubUnitsReorder = 0;
	m_dQuantityMaximum = 0.0;
	m_nTotalSubUnitsMaximum = 0;

	CStockLevelsCSVRecord StockLevels;
	DataManager.Stock.GetStockpointTotals( nStockIdx, StockRecord, m_pFilterArray -> GetSpNo(), StockLevels );

	if ( DataManager.Stock.GetSubUnits( StockRecord ) < 2 )
	{
		m_dQuantityMinimum = StockLevels.GetMinimumLevelQty();
		m_dQuantityReorder = StockLevels.GetReorderLevelQty();
		m_dQuantityMaximum = StockLevels.GetMaximumLevelQty();
	}
	else
	{
		StockRecord.ConvertStockLevelToTotalSubUnits ( StockLevels.GetMinimumLevelQty(), m_nTotalSubUnitsMinimum );
		StockRecord.ConvertStockLevelToTotalSubUnits ( StockLevels.GetReorderLevelQty(), m_nTotalSubUnitsReorder );
		StockRecord.ConvertStockLevelToTotalSubUnits ( StockLevels.GetMaximumLevelQty(), m_nTotalSubUnitsMaximum );
	}

	{
		CString strTitle = "";
		
		if ( TRUE == m_bAllowReorder )
			strTitle += "Min / Reorder / Max";
		else
			strTitle += "Min / Max";

		strTitle += " Levels ";
		strTitle += StockRecord.GetStockCode();
		strTitle += " ";
		strTitle += StockRecord.GetDescription();
		
		SetWindowText ( strTitle );
	}

	//PREPARE THE EDIT BOXES
	PrepareEditBoxes( StockRecord, 0 );
	UpdateData( FALSE );
	SetEditFocus();
}

/**********************************************************************/

void CStockEditReorderDlg::PrepareEditBoxes( CStockCSVRecord& StockRecord, int nUnitPos )
{
	int nSubUnits = 0;
	if ( nUnitPos == 0 )
		nSubUnits = DataManager.Stock.GetSubUnits( StockRecord );
		
	if ( nSubUnits <= 1 )
	{
		ShowAndEnableWindow( &m_editStockSubMinimum, FALSE );
		ShowAndEnableWindow( &m_editStockSubReorder, FALSE );
		ShowAndEnableWindow( &m_editStockSubMaximum, FALSE );
		ShowAndEnableWindow( GetEditStockQtyMinimum(), TRUE );
		ShowAndEnableWindow( GetEditStockQtyReorder(), m_bAllowReorder );
		ShowAndEnableWindow( GetEditStockQtyMaximum(), TRUE );
	}
	else
	{
		ShowAndEnableWindow( &m_editStockSubMinimum, TRUE );
		m_editStockSubMinimum.SetMode( 11, nSubUnits, TRUE );
		ShowAndEnableWindow( &m_editStockSubReorder, m_bAllowReorder );
		m_editStockSubReorder.SetMode( 11, nSubUnits, TRUE );
		ShowAndEnableWindow( &m_editStockSubMaximum, TRUE );
		m_editStockSubMaximum.SetMode( 11, nSubUnits, TRUE );
		ShowAndEnableWindow( GetEditStockQtyMinimum(), FALSE );
		ShowAndEnableWindow( GetEditStockQtyReorder(), FALSE );
		ShowAndEnableWindow( GetEditStockQtyMaximum(), FALSE );
	}
}

/**********************************************************************/

void CStockEditReorderDlg::SetEditFocus()
{
	if ( GetEditStockQtyMinimum() -> IsWindowEnabled() )
	{
		GetEditStockQtyMinimum() -> SetSel( 0, -1 );
		GetEditStockQtyMinimum() -> SetFocus();
	}
	else
	{
		m_editStockSubMinimum.SetSel( 0, -1 );
		m_editStockSubMinimum.SetFocus();
	}
}

/**********************************************************************/

void CStockEditReorderDlg::ProcessLevels()
{
	CStockCSVRecord StockRecord;
	int nStockIdx = m_pFilterArray -> GetStockItemIndex();
	DataManager.Stock.GetAt ( nStockIdx, StockRecord );
	
	double dOldMinimumQty = 0.0;
	double dOldReorderQty = 0.0;
	double dOldMaximumQty = 0.0;

	CStockLevelsCSVRecord StockLevels;
	DataManager.Stock.GetStockpointTotals( nStockIdx, StockRecord, m_pFilterArray -> GetSpNo(), StockLevels );
	dOldMinimumQty = StockLevels.GetMinimumLevelQty();
	dOldReorderQty = StockLevels.GetReorderLevelQty();
	dOldMaximumQty = StockLevels.GetMaximumLevelQty();

	double dNewMinimumQty = 0.0;
	double dNewReorderQty = 0.0;
	double dNewMaximumQty = 0.0;

	if ( GetEditStockQtyMaximum() -> IsWindowEnabled() )
	{
		dNewMinimumQty = m_dQuantityMinimum;
		dNewReorderQty = m_dQuantityReorder;
		dNewMaximumQty = m_dQuantityMaximum;
	}
	else
	{
		StockRecord.ConvertTotalSubUnitsToStockLevel( m_nTotalSubUnitsMinimum, dNewMinimumQty );
		StockRecord.ConvertTotalSubUnitsToStockLevel( m_nTotalSubUnitsReorder, dNewReorderQty );
		StockRecord.ConvertTotalSubUnitsToStockLevel( m_nTotalSubUnitsMaximum, dNewMaximumQty );
	}

	if ( dOldMinimumQty != dNewMinimumQty || dOldReorderQty != dNewReorderQty || dOldMaximumQty != dNewMaximumQty )
	{
		if ( TRUE == m_bAllowReorder )	
			StockLevels.SetReorderLevelQty( dNewReorderQty );
		
		StockLevels.SetMinimumLevelQty( dNewMinimumQty );
		StockLevels.SetMaximumLevelQty( dNewMaximumQty );
		DataManager.Stock.SetStockpointTotals( nStockIdx, StockRecord, m_pFilterArray -> GetSpNo(), StockLevels );
	}
}

/**********************************************************************/

void CStockEditReorderDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == FALSE )
		return;
	
	ProcessLevels();
	EndDialog ( IDOK );
}

/**********************************************************************/
