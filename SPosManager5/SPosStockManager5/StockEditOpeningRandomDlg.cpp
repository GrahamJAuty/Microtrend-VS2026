/**********************************************************************/
#include "FilterTypes.h"
#include "PluFilterDlgStockman.h"
#include "BarcodePluNoTable.h"
#include "StockAuditBaseReport.h"
#include "StockFilterDlg.h"
/**********************************************************************/
#include "StockEditOpeningRandomDlg.h"
/**********************************************************************/

CStockEditOpeningRandomDlg::CStockEditOpeningRandomDlg( CStockFilterArray* pStockFilterArray, bool bStocktake, int nStocktakeNo, CWnd* pParent)
	: CSSDialog(CStockEditOpeningRandomDlg::IDD, pParent), m_StockFilterArray ( FILTERTYPE_BROWSE_STOCK ), m_PluFilterArray ( FILTERTYPE_BROWSE_PLU )
{
	//{{AFX_DATA_INIT(CStockEditOpeningRandomDlg)
	m_nAction = -1;
	m_strPluNo = _T("");
	//}}AFX_DATA_INIT
	m_bStocktake = bStocktake;
	m_nStocktakeNo = nStocktakeNo;
	m_bModified = FALSE;
	m_strStockCode = "";
	m_nStockIdx = -1;
	m_nAction = 0;

	m_nSelectedDelUnit = -1;
	m_dQuantity = 0.0;
	m_nTotalSubUnits = 0;
	
	if ( m_bStocktake == TRUE )
		m_StockFilterArray.SetFilterType ( FILTERTYPE_BROWSE_STOCKTAKE );
	else
		m_StockFilterArray.SetFilterType ( FILTERTYPE_BROWSE_OPENING );

	m_StockFilterArray.SetCategoryFilter ( pStockFilterArray -> GetCategoryFilter() );
	m_StockFilterArray.SetSupplierFilter ( pStockFilterArray -> GetSupplierFilter() );
	m_StockFilterArray.SetDescriptionFilter ( pStockFilterArray -> GetDescription(), pStockFilterArray -> GetSearchType(), pStockFilterArray -> GetMatchCase() );
	m_StockFilterArray.SetSpIdx ( *pStockFilterArray );

	m_nDbNo = m_StockFilterArray.GetDbNo();
	m_nSpNo = m_StockFilterArray.GetSpNo();
}

/**********************************************************************/

void CStockEditOpeningRandomDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStockEditOpeningRandomDlg)
	DDX_Control(pDX, IDC_RADIO_STOCKCODE, m_radioStockCode);
	DDX_Control(pDX, IDC_RADIO_PLUNUMBER, m_radioPluNumber);
	DDX_Control(pDX, IDC_BUTTON_UNDO, m_buttonUndo);
	DDX_Control(pDX, IDC_COMBO_ACTION, m_comboAction);
	DDX_Control(pDX, IDC_STATIC_CONTROLS, m_staticControls);
	DDX_Control(pDX, IDC_COMBO_DELUNIT, m_comboDeliveryUnit);
	DDX_Control(pDX, IDC_EDIT_DESCRIPTION, m_editDescription);
	DDX_Control(pDX, IDC_BUTTON_ADD, m_buttonAdd);
	DDX_Control(pDX, IDC_EDIT_STOCKCODE, m_editStockCode);
	DDX_CBIndex(pDX, IDC_COMBO_ACTION, m_nAction);
	//}}AFX_DATA_MAP

	DDX_IntegerString(pDX, IDC_EDIT_PLUNO, m_strPluNo);

	DDX_Text ( pDX, IDC_EDIT_STOCK_QTY, m_dQuantity );	
	DDX_Text(pDX, IDC_EDIT_DESCRIPTION, m_strDescription );
	DDX_Text(pDX, IDC_EDIT_STOCKCODE, m_strStockCode); 
	DDV_MaxChars(pDX, m_strStockCode, Stock::StockCode.Max);

	if ( pDX -> m_bSaveAndValidate )
	{
		CString strText;
		DDX_Text(pDX, IDC_EDIT_STOCKSUB, strText );
		m_editStockSub.GetSubUnitQuantity( strText, m_nTotalSubUnits );
	}
	else
	{
		CString strText;
		m_editStockSub.GetSubUnitString( m_nTotalSubUnits, strText );
		DDX_Text(pDX, IDC_EDIT_STOCKSUB, strText );
	}
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CStockEditOpeningRandomDlg, CDialog)
	//{{AFX_MSG_MAP(CStockEditOpeningRandomDlg)
	ON_EN_KILLFOCUS(IDC_EDIT_STOCKCODE, OnKillFocusStockCode)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_EN_KILLFOCUS(IDC_EDIT_PLUNO, OnKillFocusPluNo)
	ON_CBN_SELCHANGE(IDC_COMBO_DELUNIT, OnSelectDeliveryUnit)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE_PLU, OnButtonBrowsePlu)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE_STOCK, OnButtonBrowseStock)
	ON_BN_CLICKED(IDC_BUTTON_UNDO, OnButtonUndo)
	ON_BN_CLICKED(IDC_RADIO_PLUNUMBER, OnRadioPluNumber)
	ON_BN_CLICKED(IDC_RADIO_STOCKCODE, OnRadioStockCode)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/
CEdit* CStockEditOpeningRandomDlg::GetEditStockQty() { return GetEdit( IDC_EDIT_STOCK_QTY ); }
CEdit* CStockEditOpeningRandomDlg::GetEditPluNo() { return GetEdit( IDC_EDIT_PLUNO ); }
/**********************************************************************/

BOOL CStockEditOpeningRandomDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	SubclassEdit ( IDC_EDIT_STOCK_QTY, SS_NUM_SDP, 8, "%.5f" );
	SubclassEdit ( IDC_EDIT_PLUNO, SS_NUM, Super.MaxPluLen(), "%s" );
	m_editStockSub.SubclassDlgItem ( IDC_EDIT_STOCKSUB, this, 11, 1, TRUE );
	
	m_listOpening.SubclassDlgItem ( IDC_LIST, this );
	m_listOpening.InsertColumn ( 0, "Stock Code", LVCFMT_LEFT, 120 );
	m_listOpening.InsertColumn ( 1, "Description", LVCFMT_LEFT, 120 );
	m_listOpening.InsertColumn ( 2, "Action", LVCFMT_LEFT, 75 );
	m_listOpening.InsertColumn ( 3, "Amount", LVCFMT_RIGHT, 75 );
	
	if ( m_bStocktake == TRUE )
		m_listOpening.InsertColumn ( 4, "Closing", LVCFMT_RIGHT, 75 );
	else
		m_listOpening.InsertColumn ( 4, "Opening", LVCFMT_RIGHT, 75 );

	int nSpIdx;
	dbStockpoint.FindStockpointByNumber ( m_nDbNo, m_nSpNo, nSpIdx );
	CString strTitle = "";

	if ( m_bStocktake == TRUE )
	{
		strTitle.Format ( "Closing Stock (%s)", dbStockpoint.GetName( nSpIdx ) );
		SetWindowText ( strTitle );	
		m_staticControls.SetWindowText ( "Enter Closing Stock" );
	}
	else
	{
		strTitle.Format ( "Opening Stock (%s)", dbStockpoint.GetName( nSpIdx ) );
		SetWindowText ( strTitle );
		m_staticControls.SetWindowText ( "Enter Opening Stock" );
	}

		
	m_DelUnitSelector.FillComboBox( &m_comboDeliveryUnit, -1, -1 );

	m_comboAction.AddString( "Add" );
	m_comboAction.AddString( "Subtract" );
	m_comboAction.AddString( "Set" ); 
	m_comboAction.SetCurSel( 0 );

	CRect rect;	
	GetEditStockQty() ->  GetWindowRect ( &rect );
	ScreenToClient( &rect );
	m_editStockSub.SetWindowPos( NULL, rect.left, rect.top , 0, 0, SWP_NOSIZE | SWP_NOZORDER );

	m_editStockSub.ShowWindow ( FALSE );
	m_buttonUndo.EnableWindow ( FALSE );

	OnRadioPluNumber();

	return TRUE;  
}

/**********************************************************************/

void CStockEditOpeningRandomDlg::OnKillFocusStockCode() 
{
	UpdateData ( TRUE, FALSE );
	m_strStockCode = BarcodePluNoTable.ConvertStockCode( m_strStockCode );
	FindPluFromStockCode();
	CheckStockCode();
	UpdateData ( FALSE );
}

/**********************************************************************/

void CStockEditOpeningRandomDlg::CheckStockCode()
{
	int nStockIdx;
	if ( DataManager.Stock.FindStockItemByCode ( m_strStockCode, nStockIdx ) == FALSE )
		nStockIdx = -1;

	m_nStockIdx = nStockIdx;
	m_dQuantity = 0.0;
	m_nTotalSubUnits = 0;

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

		if ( m_comboAction.GetCurSel() == 2 )
		{
			if ( m_bStocktake == FALSE )
				m_dQuantity = StockLevels.GetOpeningQty();
			else
				m_dQuantity = StockLevels.GetStocktakeClosingQty();

			StockRecord.ConvertStockLevelToTotalSubUnits ( m_dQuantity, m_nTotalSubUnits );
		}

		PrepareEditBoxes( StockRecord, 0 );
	}

	m_DelUnitSelector.FillComboBox( &m_comboDeliveryUnit, m_nStockIdx, -2 );
	m_nSelectedDelUnit = 0;
}

/**********************************************************************/

void CStockEditOpeningRandomDlg::PrepareEditBoxes( CStockCSVRecord& StockRecord, int nUnitPos )
{
	int nSubUnits = 0;
	if ( nUnitPos == 0 )
		nSubUnits = DataManager.Stock.GetSubUnits( StockRecord );
		
	if ( nSubUnits <= 1 )
	{
		m_editStockSub.ShowWindow( SW_HIDE );
		m_editStockSub.EnableWindow( FALSE );
		GetEditStockQty() -> ShowWindow( SW_SHOW );
		GetEditStockQty() -> EnableWindow( TRUE );
	}
	else
	{
		m_editStockSub.ShowWindow( SW_SHOW );
		m_editStockSub.EnableWindow( TRUE );
		m_editStockSub.SetMode( 11, nSubUnits, TRUE );
		GetEditStockQty() -> ShowWindow( SW_HIDE );
		GetEditStockQty() -> EnableWindow( FALSE );
	}
}

/**********************************************************************/

void CStockEditOpeningRandomDlg::OnButtonAdd() 
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

	case 2:
		Prompter.Error( "Please enter a non-zero quantity" );

		if ( GetEditStockQty() -> IsWindowEnabled() == TRUE )
			pNewFocus = GetEditStockQty();
		else
			pNewFocus = &m_editStockSub;

		break;

	case 3:
		Prompter.Error( "The selected stock item is included in a pending stocktake" );
		break;

	case 4:
		Prompter.Error( "The selected stock item is not included in this stocktake" );
		break;

	case 5:
		Prompter.Error( "The selected stock item is not active at this stockpoint" );
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

int CStockEditOpeningRandomDlg::HandleAdd() 
{	
	//CHECK THAT THE STOCK ITEM IS IN RANGE
	if ( m_nStockIdx < 0 || m_nStockIdx >= DataManager.Stock.GetSize() )
		return 1;

	//READ THE STOCK RECORD
	CStockCSVRecord StockRecord;
	CStockLevelsCSVRecord StockLevels;
	switch( DataManager.Stock.CheckStockLevelsForImport( m_nStockIdx, StockRecord, StockLevels, m_StockFilterArray.GetSpNo(), m_nStocktakeNo ) )
	{
	case STOCKLEVEL_IMPORT_REJECT_STOCKTAKE:

		if ( m_bStocktake == FALSE )
			return 3;
		else
			return 4;

		break;

	case STOCKLEVEL_IMPORT_REJECT_INACTIVE:
		return 5;
	}

	//CALCULATE THE TOTAL STOCK QUANTITY
	double dInputStockQty;
	if ( ( DataManager.Stock.GetSubUnits( StockRecord ) >= 2 ) && ( m_nSelectedDelUnit == 0 ) )
		StockRecord.ConvertTotalSubUnitsToStockLevel( m_nTotalSubUnits, dInputStockQty );
	else 
	{
		CDelUnitInfo DelUnitInfo;
		m_DelUnitSelector.GetDelUnitInfo( DelUnitInfo );
		dInputStockQty = m_dQuantity * DelUnitInfo.m_dDUYield;
	}

	//ZERO CAN ONLY BE USED WITH THE "SET" ACTION
	if ( dInputStockQty == 0.0 && ( m_nAction != 2 ) )
		return 2;
	
	double dOldStockQty = ( m_bStocktake ) ? StockLevels.GetStocktakeClosingQty() : StockLevels.GetOpeningQty();	
	double dNewStockQty = dOldStockQty;
	
	//WORK OUT THE NEW STOCK LEVEL
	switch ( m_nAction )
	{
	case OVERWRITE_STOCK_LEVELS:
		dNewStockQty = dInputStockQty;
		break;

	case SUBTRACT_STOCK_LEVELS:
		dNewStockQty = dOldStockQty - dInputStockQty;
		break;

	case ADD_STOCK_LEVELS:
	default:
		dNewStockQty = dOldStockQty + dInputStockQty;
		break;
	}
		
	if ( m_bStocktake == FALSE )
		StockLevels.SetOpeningQty ( dNewStockQty );
	else
		StockLevels.SetStocktakeClosingQty ( dNewStockQty );

	DataManager.Stock.SetStockpointTotals( m_nStockIdx, StockRecord, m_StockFilterArray.GetSpNo(), StockLevels );
	
	CString strTotalQty = DataManager.Stock.GetStockLevelString ( StockRecord, dInputStockQty );
	CString strNewQty = DataManager.Stock.GetStockLevelString ( StockRecord, dNewStockQty );

	CCSVStockAudit csv;
	csv.Add ( m_nStockIdx );
	csv.Add ( m_nAction );					
	csv.AddStockAuditQty ( dOldStockQty );
	csv.AddStockAuditQty ( ( m_nSelectedDelUnit != 0 ) ? m_dQuantity : 0.0 );
	csv.AddStockAuditQty ( dInputStockQty );
	csv.AddStockAuditQty ( dNewStockQty );

	m_OpeningArray.Add ( csv.GetLine() );
			
	CCSV csvOut ( '\t' );
	csvOut.Add ( StockRecord.GetStockCode() );
	csvOut.Add ( StockRecord.GetDescription() );

	switch ( m_nAction )
	{
	case 0:	csvOut.Add ( "Add" );		break;
	case 1:	csvOut.Add ( "Subtract" );	break;
	case 2:	csvOut.Add ( "Set" );		break;
	default: csvOut.Add ( "Unknown" );	break;
	}

	csvOut.Add ( strTotalQty );					
	csvOut.Add ( strNewQty );					
	m_listOpening.AddString ( csvOut.GetLine() );		
	m_listOpening.SetCurSel ( m_listOpening.GetItemCount() - 1 );
	m_buttonUndo.EnableWindow ( TRUE );

	return 0;
}

/**********************************************************************/

void CStockEditOpeningRandomDlg::FindStockCodeFromPlu()
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

void CStockEditOpeningRandomDlg::FindPluFromStockCode()
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
		__int64 nPluNo = DataManager.StockPlu.GetPluNo ( nStockPluIdx, nIdx );

		CString strPluNo;
		::FormatPluNo( nPluNo, strPluNo );

		if ( m_strPluNo == strPluNo )
			return;
	}

	if ( nSize > 0 )
	{
		__int64 nPluNo = DataManager.StockPlu.GetPluNo ( nStockPluIdx, 0 );
		::FormatPluNo( nPluNo, m_strPluNo ); 
		::AddLeading( m_strPluNo, Super.MaxPluLen(), '0' );
	}
	else
		m_strPluNo = "";
}

/**********************************************************************/

void CStockEditOpeningRandomDlg::OnKillFocusPluNo() 
{
	UpdateData ( TRUE, FALSE );
		
	::TrimSpaces ( m_strPluNo, FALSE );
	m_strPluNo = BarcodePluNoTable.ConvertPlu ( m_strPluNo );
		
	if ( m_strPluNo != "" )
	{
		FindStockCodeFromPlu();
		CheckStockCode();
	}
		
	UpdateData( FALSE );
	GetEditPluNo() -> SetWindowText( m_strPluNo );
}

/**********************************************************************/

void CStockEditOpeningRandomDlg::OnSelectDeliveryUnit() 
{
	int nNewUnit = m_comboDeliveryUnit.GetCurSel();

	if ( nNewUnit != m_nSelectedDelUnit && nNewUnit >= 0 )
	{
		m_nSelectedDelUnit = nNewUnit;

		m_dQuantity = 0.0;
		m_nTotalSubUnits = 0;
		
		CStockCSVRecord StockRecord;
		StockRecord.SetSubUnits(1);
		if ( m_nStockIdx >= 0 && m_nStockIdx < DataManager.Stock.GetSize() )
			DataManager.Stock.GetAt( m_nStockIdx, StockRecord );

		PrepareEditBoxes ( StockRecord, nNewUnit );	
		UpdateData( FALSE );
	}
}

/**********************************************************************/

void CStockEditOpeningRandomDlg::OnButtonBrowsePlu() 
{
	UpdateData ( TRUE, FALSE );

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
		if ( GetEditStockQty() -> IsWindowEnabled() == TRUE )
		{
			GetEditStockQty() -> SetFocus();
			GetEditStockQty() -> SetSel(0,-1);
		}
		else
		{
			m_editStockSub.SetFocus();
			m_editStockSub.SetSel(0,-1);
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

void CStockEditOpeningRandomDlg::OnButtonBrowseStock() 
{
	UpdateData ( TRUE, FALSE );

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
		if ( GetEditStockQty() -> IsWindowEnabled() == TRUE )
		{
			GetEditStockQty() -> SetFocus();
			GetEditStockQty() -> SetSel(0,-1);
		}
		else
		{
			m_editStockSub.SetFocus();
			m_editStockSub.SetSel(0,-1);
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

void CStockEditOpeningRandomDlg::OnButtonUndo() 
{
	Undo( TRUE );
}

/**********************************************************************/

void CStockEditOpeningRandomDlg::Undo( bool bUpdateList )
{
	int nToGo = m_OpeningArray.GetSize() - 1;

	if ( nToGo < 0 )
		return;

	CCSV csv ( m_OpeningArray.GetAt ( nToGo ) );

	int nStockIdx = csv.GetInt(0);
	double dOldQty = csv.GetDouble(2);

	if ( nStockIdx < 0 || nStockIdx >= DataManager.Stock.GetSize() )
		return;

	CStockCSVRecord StockRecord;
	DataManager.Stock.GetAt ( nStockIdx, StockRecord );
	
	CStockLevelsCSVRecord StockLevels;
	DataManager.Stock.GetStockpointTotals( nStockIdx, StockRecord, m_nSpNo, StockLevels );
	
	if ( m_bStocktake == TRUE )
		StockLevels.SetStocktakeClosingQty ( dOldQty );
	else
		StockLevels.SetOpeningQty ( dOldQty );

	DataManager.Stock.SetStockpointTotals( nStockIdx, StockRecord, m_nSpNo, StockLevels );
	
	m_OpeningArray.RemoveAt ( nToGo );
	
	if ( bUpdateList == TRUE )
	{
		if ( m_listOpening.GetCurSel() >= nToGo )
			m_listOpening.SetCurSel ( nToGo - 1 );

		m_listOpening.DeleteString ( nToGo );
	}

	if ( nToGo == 0 )
		m_buttonUndo.EnableWindow ( FALSE );
}

/**********************************************************************/

void CStockEditOpeningRandomDlg::OnRadioPluNumber() 
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
	
void CStockEditOpeningRandomDlg::OnRadioStockCode() 
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

void CStockEditOpeningRandomDlg::OnCancel() 
{
	if ( m_OpeningArray.GetSize() == 0 )
	{
		EndDialog ( IDCANCEL );
		return;
	}

	if ( Prompter.YesNo ( "Are you sure you wish to cancel these changes" ) == IDNO )
		return;

	while ( m_OpeningArray.GetSize() > 0 )
		Undo ( FALSE );

	EndDialog ( IDCANCEL );
}

/**********************************************************************/

void CStockEditOpeningRandomDlg::OnOK() 
{
	if ( UpdateData ( TRUE, TRUE ) == FALSE )
		return;

	if ( m_OpeningArray.GetSize() == 0 )
	{
		EndDialog ( IDOK );
		return;
	}

	if ( Prompter.YesNo ( "Are you sure you wish to process these changes", TRUE ) == IDNO )
		return;
		
	int nSpIdx;
	dbStockpoint.FindStockpointByNumber ( m_nDbNo, m_nSpNo, nSpIdx );

	CSSFile fileAudit;
	COleDateTime date = COleDateTime::GetCurrentTime();
	CString strAuditFile = dbStockpoint.GetFilePathAudit( nSpIdx, date, AUDIT_FAMILY_TEMP );
	if ( fileAudit.Open ( strAuditFile, "ab" ) == TRUE )
	{
		for ( int nOpenIndex = 0; nOpenIndex < m_OpeningArray.GetSize(); nOpenIndex++ )
		{
			CCSV csvIn ( m_OpeningArray.GetAt ( nOpenIndex ) );

			int nStockIdx = csvIn.GetInt(0);
			if ( nStockIdx < 0 || nStockIdx >= DataManager.Stock.GetSize() )
				continue;

			CStockCSVRecord StockRecord;
			DataManager.Stock.GetAt ( nStockIdx, StockRecord );

			CCSVStockAudit csvOut;
			csvOut.Add ( "D" );
			csvOut.Add ( m_bStocktake ? AUDIT_CLOSING : AUDIT_OPENING );
			csvOut.Add ( 2 );
			csvOut.Add ( StockRecord.GetStockCode() );
			csvOut.Add ( StockRecord.GetDescription() );
			csvOut.Add ( DataManager.Category.GetText ( StockRecord.GetCategory() ) );
			csvOut.Add ( DataManager.Stock.GetSubUnits( StockRecord ) );
			csvOut.Add ( csvIn.GetInt(1) );
			csvOut.AddStockAuditQty ( csvIn.GetDouble(3) );
			csvOut.AddStockAuditQty ( csvIn.GetDouble(4) );
			csvOut.AddStockAuditQty ( csvIn.GetDouble(5) );
			fileAudit.WriteLine ( csvOut.GetLine() );
		}
	}
	EndDialog ( IDOK );
}

/**********************************************************************/
