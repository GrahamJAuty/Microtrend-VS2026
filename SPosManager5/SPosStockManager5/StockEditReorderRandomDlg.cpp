/**********************************************************************/
 
/**********************************************************************/
#include "BarcodePluNoTable.h"
//#include "DataManager.h"
#include "FilterTypes.h"
//#include "globalfunctions.h"
#include "PluFilterDlgStockman.h"
 
#include "StockFilterDlg.h"
#include "StockOptionsIni.h"
/**********************************************************************/
#include "StockEditReorderRandomDlg.h"
/**********************************************************************/

CStockEditReorderRandomDlg::CStockEditReorderRandomDlg( CStockFilterArray* pStockFilterArray, CWnd* pParent)
	: CSSDialog(CStockEditReorderRandomDlg::IDD, pParent), m_StockFilterArray ( FILTERTYPE_BROWSE_STOCK ), m_PluFilterArray ( FILTERTYPE_BROWSE_PLU )
{
	//{{AFX_DATA_INIT(CStockEditReorderRandomDlg)
	//}}AFX_DATA_INIT
	m_bModified = FALSE;
	m_strStockCode = "";
	m_nStockIdx = -1;
	m_dQuantityMinimum = 0.0;
	m_dQuantityReorder = 0.0;
	m_dQuantityMaximum = 0.0;
	m_nTotalSubUnitsMinimum = 0;
	m_nTotalSubUnitsReorder = 0;
	m_nTotalSubUnitsMaximum = 0;
	m_StockFilterArray.SetFilterType ( FILTERTYPE_BROWSE_TRANSFER );
	m_StockFilterArray.SetCategoryFilter ( pStockFilterArray -> GetCategoryFilter() );
	m_StockFilterArray.SetSupplierFilter ( pStockFilterArray -> GetSupplierFilter() );
	m_StockFilterArray.SetDescriptionFilter ( pStockFilterArray -> GetDescription(), pStockFilterArray -> GetSearchType(), pStockFilterArray -> GetMatchCase() );
	m_StockFilterArray.SetSpIdx ( pStockFilterArray -> GetSpIdx(), pStockFilterArray -> GetSpIdx2() );
	m_nSpNo = m_StockFilterArray.GetSpNo();
	m_nDbNo = m_StockFilterArray.GetDbNo();
	m_bAllowReorder = ( StockOptions.GetOrderGenerationMethod() != ORDER_TYPE_SALES );
}

/**********************************************************************/

void CStockEditReorderRandomDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStockEditReorderRandomDlg)
	DDX_Control(pDX, IDC_STATIC_LEVELS, m_staticLevels );
	DDX_Control(pDX, IDC_STATIC_REORDER, m_staticReorder );
	DDX_Control(pDX, IDC_STATIC_MAXIMUM, m_staticMaximum );
	DDX_Control(pDX, IDOK, m_buttonOK );
	DDX_Control(pDX, IDCANCEL, m_buttonCancel );
	DDX_Control(pDX, IDC_RADIO_STOCKCODE, m_radioStockCode);
	DDX_Control(pDX, IDC_RADIO_PLUNUMBER, m_radioPluNumber);
	DDX_Control(pDX, IDC_BUTTON_UNDO, m_buttonUndo);
	DDX_Control(pDX, IDC_EDIT_DESCRIPTION, m_editDescription);
	DDX_Control(pDX, IDC_BUTTON_ADD, m_buttonAdd);
	DDX_Control(pDX, IDC_EDIT_STOCKCODE, m_editStockCode);
	//}}AFX_DATA_MAP

	DDX_Text ( pDX, IDC_EDIT_MINIMUM_QTY, m_dQuantityMinimum );
	DDX_Text ( pDX, IDC_EDIT_REORDER_QTY, m_dQuantityReorder );
	DDX_Text ( pDX, IDC_EDIT_MAXIMUM_QTY, m_dQuantityMaximum );
	DDX_Text(pDX, IDC_EDIT_DESCRIPTION, m_strDescription );
	DDX_Text(pDX, IDC_EDIT_STOCKCODE, m_strStockCode);
	DDV_MaxChars(pDX, m_strStockCode, Stock::StockCode.Max);
	DDX_IntegerString(pDX, IDC_EDIT_PLUNO, m_strPluNo);

	if ( pDX -> m_bSaveAndValidate )
	{
		CString strText;
		DDX_Text(pDX, IDC_EDIT_MINIMUM_STOCKSUB, strText );
		m_editStockSubMinimum.GetSubUnitQuantity( strText, m_nTotalSubUnitsMinimum );

		if ( TRUE == m_bAllowReorder )
		{
			DDX_Text(pDX, IDC_EDIT_REORDER_STOCKSUB, strText );
			m_editStockSubReorder.GetSubUnitQuantity( strText, m_nTotalSubUnitsReorder );
		}

		DDX_Text(pDX, IDC_EDIT_MAXIMUM_STOCKSUB, strText );
		m_editStockSubMaximum.GetSubUnitQuantity( strText, m_nTotalSubUnitsMaximum );
	}
	else
	{
		CString strText;
		m_editStockSubMinimum.GetSubUnitString( m_nTotalSubUnitsMinimum, strText );
		DDX_Text(pDX, IDC_EDIT_MINIMUM_STOCKSUB, strText );

		if ( TRUE == m_bAllowReorder )
		{
			m_editStockSubReorder.GetSubUnitString( m_nTotalSubUnitsReorder, strText );
			DDX_Text(pDX, IDC_EDIT_REORDER_STOCKSUB, strText );
		}

		m_editStockSubMaximum.GetSubUnitString( m_nTotalSubUnitsMaximum, strText );
		DDX_Text(pDX, IDC_EDIT_MAXIMUM_STOCKSUB, strText );
	}
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CStockEditReorderRandomDlg, CDialog)
	//{{AFX_MSG_MAP(CStockEditReorderRandomDlg)
	ON_EN_KILLFOCUS(IDC_EDIT_STOCKCODE, OnKillFocusStockCode)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_EN_KILLFOCUS(IDC_EDIT_PLUNO, OnKillFocusPluNo)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE_PLU, OnButtonBrowsePlu)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE_STOCK, OnButtonBrowseStock)
	ON_BN_CLICKED(IDC_BUTTON_UNDO, OnButtonUndo)
	ON_BN_CLICKED(IDC_RADIO_PLUNUMBER, OnRadioPluNumber)
	ON_BN_CLICKED(IDC_RADIO_STOCKCODE, OnRadioStockCode)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/
CEdit* CStockEditReorderRandomDlg::GetEditStockQtyMinimum() { return GetEdit( IDC_EDIT_MINIMUM_QTY ); }
CEdit* CStockEditReorderRandomDlg::GetEditStockQtyReorder() { return GetEdit( IDC_EDIT_REORDER_QTY ); }
CEdit* CStockEditReorderRandomDlg::GetEditStockQtyMaximum() { return GetEdit( IDC_EDIT_MAXIMUM_QTY ); }
CEdit* CStockEditReorderRandomDlg::GetEditPluNo() { return GetEdit( IDC_EDIT_PLUNO ); }
/**********************************************************************/

BOOL CStockEditReorderRandomDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	int nSpIdx;
	dbStockpoint.FindStockpointByNumber ( m_nDbNo, m_nSpNo, nSpIdx );
	CString strName = dbStockpoint.GetName( nSpIdx );
			
	SubclassEdit ( IDC_EDIT_MINIMUM_QTY, SS_NUM_SDP, 8, "%.5f" );
	SubclassEdit ( IDC_EDIT_REORDER_QTY, SS_NUM_SDP, 8, "%.5f" );
	SubclassEdit ( IDC_EDIT_MAXIMUM_QTY, SS_NUM_SDP, 8, "%.5f" );
	SubclassEdit ( IDC_EDIT_PLUNO, SS_NUM, Super.MaxPluLen(), "%s" );
	m_editStockSubMinimum.SubclassDlgItem ( IDC_EDIT_MINIMUM_STOCKSUB, this, 11, 1, FALSE );
	m_editStockSubReorder.SubclassDlgItem ( IDC_EDIT_REORDER_STOCKSUB, this, 11, 1, FALSE );
	m_editStockSubMaximum.SubclassDlgItem ( IDC_EDIT_MAXIMUM_STOCKSUB, this, 11, 1, FALSE );
	
	m_listReorder.SubclassDlgItem ( IDC_LIST, this );
	m_listReorder.InsertColumn ( 0, "Stock Code", LVCFMT_LEFT, 120 );
	m_listReorder.InsertColumn ( 1, "Description", LVCFMT_LEFT, 120 );

	if ( TRUE == m_bAllowReorder )
	{
		m_listReorder.InsertColumn ( 2, "Minimum", LVCFMT_LEFT, 70 );
		m_listReorder.InsertColumn ( 3, "Reorder", LVCFMT_LEFT, 70 );
		m_listReorder.InsertColumn ( 4, "Maximum", LVCFMT_LEFT, 70 );
	}
	else
	{
		m_listReorder.InsertColumn ( 2, "Minimum", LVCFMT_LEFT, 105 );
		m_listReorder.InsertColumn ( 3, "Maximum", LVCFMT_LEFT, 105 );
	}
	
	{
		CString strTitle = "";
		strTitle += "Edit ";
		
		if ( TRUE == m_bAllowReorder )
			strTitle += "Min / Reorder / Max";
		else
			strTitle += "Min / Reorder";

		strTitle += " Levels (";
		strTitle += strName;
		strTitle += ")";
		
		SetWindowText ( strTitle );
	}

	MoveControl( &m_editStockSubMinimum, 85, 200 );
	MoveControl( &m_editStockSubReorder, 85, 220 );
	MoveControl( &m_editStockSubMaximum, 85, 240 );
	
	m_editStockSubMinimum.ShowWindow ( FALSE );
	m_editStockSubReorder.ShowWindow ( FALSE );
	m_editStockSubMaximum.ShowWindow ( FALSE );

	if ( FALSE == m_bAllowReorder )
	{
		m_staticReorder.ShowWindow( SW_HIDE );
		MoveControl( &m_staticMaximum, 19, 220 );
		MoveControl( GetEditStockQtyMaximum(), 85, 220 );
		MoveControl( &m_editStockSubMaximum, 146, 220 );
		MoveControl( &m_buttonAdd, 263, 223 );
		ResizeControl( &m_staticLevels, 316, 131 );
		MoveControl( &m_buttonOK, 331, 215 );
		MoveControl( &m_buttonCancel, 331, 231 );
		ResizeDialog( 388, 252 );	
	}

	m_buttonUndo.EnableWindow( FALSE );
	OnRadioPluNumber();

	return TRUE;  
}

/**********************************************************************/

void CStockEditReorderRandomDlg::OnKillFocusStockCode() 
{
	UpdateData( TRUE, FALSE );
	m_strStockCode = BarcodePluNoTable.ConvertStockCode ( m_strStockCode );
	FindPluFromStockCode();
	CheckStockCode();
	UpdateData ( FALSE );
}

/**********************************************************************/

void CStockEditReorderRandomDlg::CheckStockCode()
{
	int nStockIdx;
	if ( DataManager.Stock.FindStockItemByCode ( m_strStockCode, nStockIdx ) == FALSE )
		nStockIdx = -1;

	m_nStockIdx = nStockIdx;
	m_dQuantityMinimum = 0.0;
	m_dQuantityReorder = 0.0;
	m_dQuantityMaximum = 0.0;
	m_nTotalSubUnitsMinimum = 0;
	m_nTotalSubUnitsReorder = 0;
	m_nTotalSubUnitsMaximum = 0;

	CStockCSVRecord StockRecord;
	StockRecord.SetSubUnits(1);

	if ( m_nStockIdx == -1 )
	{
		m_strDescription = "Unknown";
		PrepareEditBoxes( StockRecord, 0 );	
	}
	else
	{
		DataManager.Stock.GetAt ( m_nStockIdx, StockRecord );
		m_strDescription = StockRecord.GetDescription();

		CStockLevelsCSVRecord StockLevels;
		DataManager.Stock.GetStockpointTotals ( nStockIdx, StockRecord, m_nSpNo, StockLevels );
		m_dQuantityMinimum = StockLevels.GetMinimumLevelQty();
		m_dQuantityReorder = StockLevels.GetReorderLevelQty();
		m_dQuantityMaximum = StockLevels.GetMaximumLevelQty();
		StockRecord.ConvertStockLevelToTotalSubUnits ( m_dQuantityMinimum, m_nTotalSubUnitsMinimum );
		StockRecord.ConvertStockLevelToTotalSubUnits ( m_dQuantityReorder, m_nTotalSubUnitsReorder );
		StockRecord.ConvertStockLevelToTotalSubUnits ( m_dQuantityMaximum, m_nTotalSubUnitsMaximum );

		PrepareEditBoxes( StockRecord, 0 );
	}
}

/**********************************************************************/

void CStockEditReorderRandomDlg::PrepareEditBoxes( CStockCSVRecord& StockRecord, int nUnitPos )
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

void CStockEditReorderRandomDlg::OnButtonAdd() 
{
	m_buttonAdd.SetFocus();

	if ( UpdateData ( TRUE ) == FALSE )
		return;
	
	int nResult = HandleAdd();
	
	CEdit* pNewFocus = NULL;

	switch( nResult )
	{
	case 1:
		Prompter.Error( "The selected stock item does not exist in your database" );
		break;
	}

	if ( pNewFocus == NULL )
	{
		switch ( m_nEntryMode )
		{
		case ENTRY_MODE_PLUNO:		pNewFocus = GetEditPluNo();	break;
		case ENTRY_MODE_STOCKCODE:	
		default:					pNewFocus = &m_editStockCode;	break;
		}
	}

	pNewFocus -> SetFocus();
	pNewFocus -> SetSel( 0, -1 );
}

/**********************************************************************/

int CStockEditReorderRandomDlg::HandleAdd()
{
	//CHECK THAT THE STOCK ITEM IS IN RANGE
	if ( m_nStockIdx < 0 || m_nStockIdx >= DataManager.Stock.GetSize() )
		return 1;

	//READ THE STOCK RECORD
	CStockCSVRecord StockRecord;
	DataManager.Stock.GetAt ( m_nStockIdx, StockRecord );

	//WORK OUT THE TOTAL STOCK UNITS REQUIRED
	double dInputStockQtyMinimum;
	if ( GetEditStockQtyMinimum() ->IsWindowEnabled() )
		dInputStockQtyMinimum = m_dQuantityMinimum;
	else
		StockRecord.ConvertTotalSubUnitsToStockLevel( m_nTotalSubUnitsMinimum, dInputStockQtyMinimum );
	
	double dInputStockQtyReorder;
	if ( GetEditStockQtyReorder() ->IsWindowEnabled() )
		dInputStockQtyReorder = m_dQuantityReorder;
	else
		StockRecord.ConvertTotalSubUnitsToStockLevel( m_nTotalSubUnitsReorder, dInputStockQtyReorder );

	double dInputStockQtyMaximum;
	if ( GetEditStockQtyMaximum() ->IsWindowEnabled() )
		dInputStockQtyMaximum = m_dQuantityMaximum;
	else
		StockRecord.ConvertTotalSubUnitsToStockLevel( m_nTotalSubUnitsMaximum, dInputStockQtyMaximum );
	
	CStockLevelsCSVRecord StockLevels;
	DataManager.Stock.GetStockpointTotals( m_nStockIdx, StockRecord, m_nSpNo, StockLevels );
	
	CCSVStockAudit csv;
	csv.Add ( m_nStockIdx );
	csv.AddStockAuditQty ( StockLevels.GetMinimumLevelQty() );
	csv.AddStockAuditQty ( StockLevels.GetReorderLevelQty() );
	csv.AddStockAuditQty ( StockLevels.GetMaximumLevelQty() );
	m_ReorderArray.Add ( csv.GetLine() );
	
	StockLevels.SetMinimumLevelQty( dInputStockQtyMinimum );
	StockLevels.SetMaximumLevelQty( dInputStockQtyMaximum );
	
	if ( TRUE == m_bAllowReorder )
		StockLevels.SetReorderLevelQty( dInputStockQtyReorder );
	
	DataManager.Stock.SetStockpointTotals( m_nStockIdx, StockRecord, m_nSpNo, StockLevels ); 
			
	CString strMinimum = DataManager.Stock.GetStockLevelString ( StockRecord, StockLevels.GetMinimumLevelQty() );
	CString strReorder = DataManager.Stock.GetStockLevelString ( StockRecord, StockLevels.GetReorderLevelQty() );
	CString strMaximum = DataManager.Stock.GetStockLevelString ( StockRecord, StockLevels.GetMaximumLevelQty() );
	
	CCSV csvOut ( '\t' );
	csvOut.Add ( StockRecord.GetStockCode() );
	csvOut.Add ( StockRecord.GetDescription() );
	csvOut.Add ( strMinimum );

	if ( TRUE == m_bAllowReorder )
		csvOut.Add ( strReorder );

	csvOut.Add ( strMaximum );
	m_listReorder.AddString ( csvOut.GetLine() );		
	m_listReorder.SetCurSel ( m_listReorder.GetItemCount() - 1 );
	m_buttonUndo.EnableWindow ( TRUE );

	return 0;
}

/**********************************************************************/

void CStockEditReorderRandomDlg::FindStockCodeFromPlu()
{
	int nPluIdx;
	if ( DataManager.Plu.FindPluByNumber ( _atoi64(m_strPluNo), nPluIdx ) == FALSE )
		m_strStockCode = "";
	else
	{
		CPluCSVRecord PluRecord;
		DataManager.Plu.GetAt ( nPluIdx, PluRecord );
		DataManager.Plu.GetPluStockCode ( PluRecord.GetPluNoInt(), m_strStockCode, FALSE );
	}
}

/**********************************************************************/

void CStockEditReorderRandomDlg::FindPluFromStockCode()
{
	int nStockPluIdx;
	if ( DataManager.StockPlu.FindStockCode ( m_strStockCode, nStockPluIdx ) == FALSE )
	{
		m_strPluNo = "";
		return;
	}

	int nSize = DataManager.StockPlu.GetPluNoCount( nStockPluIdx );

	for ( int nIdx = 0; nIdx < nSize; nIdx++ )
	{
		CString strPluNo;
		__int64 nPluNo = DataManager.StockPlu.GetPluNo ( nStockPluIdx, nIdx );
		::FormatPluNo( nPluNo, strPluNo );

		if ( m_strPluNo == strPluNo )
			return;
	}

	if ( nSize > 0 )
	{
		__int64 nPluNo = DataManager.StockPlu.GetPluNo( nStockPluIdx, 0 );
		::FormatPluNo( nPluNo, m_strPluNo );
	}
	else
		m_strPluNo = "";
}

/**********************************************************************/

void CStockEditReorderRandomDlg::OnKillFocusPluNo() 
{
	UpdateData( TRUE, FALSE );

	m_strPluNo = BarcodePluNoTable.ConvertPlu ( m_strPluNo );
		
	if ( m_strPluNo != "" )
	{
		FindStockCodeFromPlu();
		CheckStockCode();
	}
		
	UpdateData ( FALSE );	
}

/**********************************************************************/

void CStockEditReorderRandomDlg::OnButtonBrowsePlu() 
{
	UpdateData( TRUE, FALSE );

	int nDlgResult, nPluIdx;
	::ShowPluFilterDlg( m_PluFilterArray, this, nDlgResult, nPluIdx );
		
	if ( IDOK != nDlgResult )
		return;

	if ( nPluIdx < 0 )
		return;

	CPluCSVRecord PluRecord;
	DataManager.Plu.GetAt ( nPluIdx, PluRecord );
	m_strPluNo = PluRecord.GetPluNoString();	
	FindStockCodeFromPlu();
	CheckStockCode();
	UpdateData ( FALSE );

	switch ( m_nEntryMode )
	{
	case ENTRY_MODE_STOCKCODE:
		if ( GetEditStockQtyMinimum() -> IsWindowEnabled() == TRUE )
		{
			GetEditStockQtyMinimum() -> SetFocus();
			GetEditStockQtyMinimum() -> SetSel(0,-1);
		}
		else
		{
			m_editStockSubMinimum.SetFocus();
			m_editStockSubMinimum.SetSel(0,-1);
		}
		break;

	case ENTRY_MODE_PLUNO:
	default:
		GetEditPluNo() -> SetFocus();
		GetEditPluNo() -> SetSel ( 0, -1 );
		break;
	}	
}

/**********************************************************************/

void CStockEditReorderRandomDlg::OnButtonBrowseStock() 
{
	UpdateData( TRUE, FALSE );

	CWaitCursor wait;
	CStockFilterDlg dlgBrowse ( m_StockFilterArray, this );	
	if ( dlgBrowse.DoModal() != IDOK )
		return;

	int nStockIdx = dlgBrowse.GetStockIdx();
	if ( nStockIdx < 0 )
		return;

	CStockCSVRecord StockRecord;
	DataManager.Stock.GetAt ( nStockIdx, StockRecord );
	m_strStockCode = StockRecord.GetStockCode ();	
	m_strDescription = StockRecord.GetDescription();
	FindPluFromStockCode();
	UpdateData ( FALSE );
	
	switch ( m_nEntryMode )
	{
	case ENTRY_MODE_PLUNO:
		if ( GetEditStockQtyMinimum() -> IsWindowEnabled() == TRUE )
		{
			GetEditStockQtyMinimum() -> SetFocus();
			GetEditStockQtyMinimum() -> SetSel(0,-1);
		}
		else
		{
			m_editStockSubMinimum.SetFocus();
			m_editStockSubMinimum.SetSel(0,-1);
		}
		break;

	case ENTRY_MODE_STOCKCODE:
	default:
		m_editStockCode.SetFocus();
		m_editStockCode.SetSel ( 0, -1 );
		break;
	}
}

/**********************************************************************/

void CStockEditReorderRandomDlg::OnRadioPluNumber() 
{
	m_radioPluNumber.SetCheck( TRUE );
	m_radioStockCode.SetCheck( FALSE );

	GetEditPluNo() -> EnableWindow( TRUE );
	m_editStockCode.EnableWindow( FALSE );

	m_nEntryMode = ENTRY_MODE_PLUNO;

	GetEditPluNo() -> SetFocus();
	GetEditPluNo() -> SetSel( 0, -1 );
}

/**********************************************************************/
	
void CStockEditReorderRandomDlg::OnRadioStockCode() 
{
	m_radioPluNumber.SetCheck( FALSE );
	m_radioStockCode.SetCheck( TRUE );
	
	GetEditPluNo() -> EnableWindow( FALSE );
	m_editStockCode.EnableWindow( TRUE );
	
	m_nEntryMode = ENTRY_MODE_STOCKCODE;

	m_editStockCode.SetFocus();
	m_editStockCode.SetSel( 0, -1 );
}

/**********************************************************************/

void CStockEditReorderRandomDlg::OnCancel() 
{
	if ( m_ReorderArray.GetSize() == 0 )
	{
		EndDialog ( IDCANCEL );
		return;
	}

	if ( Prompter.YesNo ( "Are you sure you wish to cancel these changes" ) == IDNO )
		return;

	while ( m_ReorderArray.GetSize() > 0 )
		Undo ( FALSE );

	EndDialog ( IDCANCEL );
}

/**********************************************************************/

void CStockEditReorderRandomDlg::OnButtonUndo() 
{
	Undo( TRUE );	
}

/**********************************************************************/

void CStockEditReorderRandomDlg::Undo( bool bUpdateList ) 
{
	int nToGo = m_ReorderArray.GetSize() - 1;

	if ( nToGo < 0 )
		return;

	CCSV csv ( m_ReorderArray.GetAt ( nToGo ) );

	int nStockIdx = csv.GetInt(0);
	double dMinimum = csv.GetDouble(1);
	double dReorder = csv.GetDouble(2);
	double dMaximum = csv.GetDouble(3);

	if ( nStockIdx < 0 || nStockIdx >= DataManager.Stock.GetSize() )
		return;

	CStockCSVRecord StockRecord;
	DataManager.Stock.GetAt ( nStockIdx, StockRecord );
	
	CStockLevelsCSVRecord StockLevels;
	DataManager.Stock.GetStockpointTotals( nStockIdx, StockRecord, m_nSpNo, StockLevels );
	StockLevels.SetMinimumLevelQty( dMinimum );
	StockLevels.SetMaximumLevelQty( dMaximum );
	
	if ( TRUE == m_bAllowReorder )
		StockLevels.SetReorderLevelQty( dReorder );
	
	DataManager.Stock.SetStockpointTotals( nStockIdx, StockRecord, m_nSpNo, StockLevels );
	
	m_ReorderArray.RemoveAt ( nToGo );
	
	if ( bUpdateList == TRUE )
	{
		if ( m_listReorder.GetCurSel() >= nToGo )
			m_listReorder.SetCurSel ( nToGo - 1 );

		m_listReorder.DeleteString ( nToGo );
	}

	if ( nToGo == 0 )
		m_buttonUndo.EnableWindow ( FALSE );
}

/**********************************************************************/

void CStockEditReorderRandomDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == FALSE )
		return;
	
	if ( m_ReorderArray.GetSize() == 0 )
	{
		EndDialog ( IDOK );
		return;
	}

	if ( Prompter.YesNo ( "Are you sure you wish to process these changes", TRUE ) == IDNO )
		return;
	
	EndDialog ( IDOK );
}

/**********************************************************************/

