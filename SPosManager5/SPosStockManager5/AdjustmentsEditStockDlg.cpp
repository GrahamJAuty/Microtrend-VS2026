/**********************************************************************/
#include "FilterTypes.h"
#include "..\CommonEcrStock\ListDataUnitDlg.h"
#include "PluFilterDlgStockman.h"
#include "BarcodePluNoTable.h"
#include "PriceHelpers.h"
#include "RepCSV.h"
#include "StockFilterDlg.h"
#include "SysInfo.h"
/**********************************************************************/
#include "AdjustmentsEditStockDlg.h"
/**********************************************************************/

CAdjustmentsEditStockDlg::CAdjustmentsEditStockDlg( int nSpIdx, CString& strLastAdjustment, CAdjustmentsEditStockDlg** pParentToThis, int nStkTakeNo, CWnd* pParent)
	: CSSDialog("IDD_ADJUSTMENT_EDIT_STOCK", pParent),
	m_FilterArray ( FILTERTYPE_BROWSE_PLU ), 
	m_StockFilterArray ( FILTERTYPE_BROWSE_STOCK ),
	m_strLastAdjustment ( strLastAdjustment )
{
	m_nStkTakeNo = nStkTakeNo;

	DataInit ( nSpIdx );
	m_bAdd = TRUE;
	m_strAction = "Add";
	
	m_nEntryMode = ENTRY_MODE_PLUNO;

	m_pParentToThis = pParentToThis;
	*m_pParentToThis = this;

	m_dQuantity = 0.0;
	m_nTotalSubUnits = 0;
}

/**********************************************************************/

CAdjustmentsEditStockDlg::CAdjustmentsEditStockDlg( int nSpIdx, CString& strDummy, int nStkTakeNo, CWnd* pParent)
	: CSSDialog("IDD_ADJUSTMENT_EDIT_STOCK", pParent),
	m_FilterArray ( FILTERTYPE_BROWSE_PLU ), 
	m_StockFilterArray ( FILTERTYPE_BROWSE_STOCK ), 
	m_strLastAdjustment( strDummy )
{
	m_nStkTakeNo = nStkTakeNo;

	DataInit ( nSpIdx );
	m_bAdd = FALSE;
	m_strAction = "Edit";
}

/**********************************************************************/

void CAdjustmentsEditStockDlg::DataInit( int nSpIdx )
{
	//{{AFX_DATA_INIT(CAdjustmentsEditStockDlg)
	m_strDescription = _T("");
	//}}AFX_DATA_INIT

	m_dQuantity = 0.0;
	m_nTotalSubUnits = 0;
	m_dCost = 0.0;
	m_strStockCode = "";
	m_nSpIdx = nSpIdx;

	if ( m_nStkTakeNo >= 1 )
	{
		m_StockFilterArray.SetSpIdx( m_nSpIdx, -1 );
		m_StockFilterArray.SetStocktakeFilter( m_nStkTakeNo );
	}
}

/**********************************************************************/

void CAdjustmentsEditStockDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAdjustmentsEditStockDlg)
	DDX_Control(pDX, IDC2_COMBO_REASON, m_comboReason);
	DDX_Control(pDX, IDC2_BUTTON_BROWSE_PLU, m_buttonBrowsePlu);
	DDX_Control(pDX, IDC2_BUTTON_BROWSE, m_buttonBrowseStock);
	DDX_Control(pDX, IDC2_RADIO_STOCKCODE, m_radioStockCode);
	DDX_Control(pDX, IDC2_RADIO_PLUNUMBER, m_radioPluNumber);
	DDX_Control(pDX, IDC2_EDIT_STOCKCODE, m_editStockCode);
	DDX_Control(pDX, IDC2_STATIC_UNITTEXT, m_staticUnitText);
	DDX_Control(pDX, IDCANCEL, m_buttonCancel);
	DDX_Control(pDX, IDOK, m_buttonOK);
	DDX_Text(pDX, IDC2_EDIT_DESCRIPTION, m_strDescription );
	
	//}}AFX_DATA_MAP
	DDX_Text ( pDX, IDC2_EDIT_STOCK_QTY, m_dQuantity );	
	
	if ( pDX -> m_bSaveAndValidate )
	{
		CString strText;
		DDX_Text(pDX, IDC2_EDIT_STOCKSUB, strText );
		m_editStockSub.GetSubUnitQuantity( strText, m_nTotalSubUnits );
	}
	else
	{
		CString strText;
		m_editStockSub.GetSubUnitString( m_nTotalSubUnits, strText );
		DDX_Text(pDX, IDC2_EDIT_STOCKSUB, strText );
	}
	
	DDX_Text(pDX, IDC2_COMBO_REASON, m_strLastAdjustment);
	DDX_Text(pDX, IDC2_EDIT_COST, m_dCost );
	DDX_Text(pDX, IDC2_EDIT_STOCKCODE, m_strStockCode);
	DDV_MaxChars(pDX, m_strStockCode, Stock::StockCode.Max); 
	DDX_IntegerString(pDX, IDC2_EDIT_PLUNO, m_strPluNo);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CAdjustmentsEditStockDlg, CDialog)
	//{{AFX_MSG_MAP(CAdjustmentsEditStockDlg)
	ON_EN_KILLFOCUS(IDC2_EDIT_STOCKCODE, OnKillFocusStockCode)
	ON_BN_CLICKED(IDC2_BUTTON_BROWSE, OnButtonBrowseStock)
	ON_EN_KILLFOCUS(IDC2_EDIT_PLUNO, OnKillFocusPluNo)
	ON_BN_CLICKED(IDC2_BUTTON_BROWSE_PLU, OnButtonBrowsePlu)
	ON_EN_KILLFOCUS(IDC2_EDIT_STOCK_QTY, OnKillFocusStockQty)
	ON_EN_KILLFOCUS(IDC2_EDIT_STOCKSUB, OnKillFocusStockSub)
	ON_BN_CLICKED(IDC2_RADIO_PLUNUMBER, OnRadioPluNumber)
	ON_BN_CLICKED(IDC2_RADIO_STOCKCODE, OnRadioStockCode)
	ON_BN_CLICKED(IDC2_BUTTON_SET, OnButtonSet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/
CEdit* CAdjustmentsEditStockDlg::GetEditStockQty() { return GetEdit( IDC2_EDIT_STOCK_QTY ); }
CEdit* CAdjustmentsEditStockDlg::GetEditCost() { return GetEdit( IDC2_EDIT_COST ); }
CEdit* CAdjustmentsEditStockDlg::GetEditPluNo() { return GetEdit( IDC2_EDIT_PLUNO ); }
/**********************************************************************/

BOOL CAdjustmentsEditStockDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	SubclassEdit ( IDC2_EDIT_STOCK_QTY, SS_NUM_SDP, 8, "%.5f" );
	SubclassEdit ( IDC2_EDIT_COST, SS_NUM_DP, 8, "%.*f", SysInfo.GetDPValue() );
	SubclassEdit ( IDC2_EDIT_PLUNO, SS_NUM, Super.MaxPluLen(), "%s" );

	m_editStockSub.SubclassDlgItem ( IDC2_EDIT_STOCKSUB, this, 11, 1, TRUE );

	m_staticUnitText.SetWindowText( "" );

	CString strTitle;
	strTitle.Format ( "%s Adjustments Line (%s)", 
		(const char*) m_strAction,
		dbStockpoint.GetName( m_nSpIdx ) );
	
	SetWindowText ( strTitle );

	for ( int nIndex = 0; nIndex < DataManagerNonDb.AdjustText.GetSize(); nIndex++ )
		m_comboReason.AddString ( DataManagerNonDb.AdjustText.GetText( nIndex ) );

	CenterWindow();

	CRect rect;
	GetEditStockQty() -> GetWindowRect ( &rect );
	ScreenToClient( &rect );
	m_editStockSub.SetWindowPos( NULL, rect.left, rect.top , 0, 0, SWP_NOSIZE | SWP_NOZORDER );

	CStockCSVRecord StockRecord;
	StockRecord.SetSubUnits(1);

	if ( m_bAdd == TRUE )
	{
		m_comboReason.SetWindowText( m_strLastAdjustment );
		PrepareEditBoxes( StockRecord );
		OnRadioPluNumber();
	}
	else
	{
		CCSVAdjustmentInfo csvIn( m_strInputLine );
			
		m_nEntryMode = csvIn.GetItemLookupMethod();
		m_radioPluNumber.SetCheck( m_nEntryMode == ENTRY_MODE_PLUNO );
		m_radioStockCode.SetCheck( m_nEntryMode == ENTRY_MODE_STOCKCODE );

		GetEditPluNo() -> EnableWindow( m_nEntryMode == ENTRY_MODE_PLUNO );
		m_editStockCode.EnableWindow( m_nEntryMode == ENTRY_MODE_STOCKCODE );
	
		m_strStockCode = csvIn.GetStockCode();
		m_strDescription = csvIn.GetDescription();
		::FormatPluNo ( csvIn.GetPluNo(), m_strPluNo );
		m_dCost = csvIn.GetValue();
		
		m_dQuantity = csvIn.GetQuantity();
		m_nTotalSubUnits = 0;
		
		int nStockIdx;
		if ( DataManager.Stock.FindStockItemByCode( m_strStockCode, nStockIdx ) == TRUE )
		{
			DataManager.Stock.GetAt( nStockIdx, StockRecord );
			StockRecord.ConvertStockLevelToTotalSubUnits( m_dQuantity, m_nTotalSubUnits );

			m_staticUnitText.SetWindowText( DataManager.Stock.GetStockSubUnitNameForPluEdit( StockRecord ) );
		}
		else
		{
			m_dQuantity = csvIn.GetQuantity();
			m_nTotalSubUnits = (int) m_dQuantity;

			m_staticUnitText.SetWindowText( "" );
		}

		PrepareEditBoxes( StockRecord );
		UpdateData( FALSE );
			
		m_buttonOK.SetWindowText ( "OK" );
		m_buttonCancel.SetWindowText ("Cancel" );

		m_comboReason.SetWindowText( csvIn.GetReason() );			
		SetEditFocus();
	}
	
	//WE HAVE SET FOCUS TO A CONTROL
	return FALSE;  
}

/**********************************************************************/

void CAdjustmentsEditStockDlg::PrepareEditBoxes( CStockCSVRecord& StockRecord )
{
	int nSubUnits = DataManager.Stock.GetSubUnits( StockRecord );
		
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

void CAdjustmentsEditStockDlg::SetEditFocus()
{
	if ( GetEditStockQty() -> IsWindowEnabled() )
	{
		GetEditStockQty() -> SetFocus();
		GetEditStockQty() -> SetSel( 0, -1 );
	}
	else
	{
		m_editStockSub.SetFocus();
		m_editStockSub.SetSel( 0, -1 );
	}
}

/**********************************************************************/

bool CAdjustmentsEditStockDlg::LookUpPluNo( __int64 nPluNo, CString& strStockCode, int& nStockIdx )
{
	strStockCode = "";
	nStockIdx = -1;

	bool bFound = FALSE;

	if ( nPluNo != 0 )
	{
		int nPluIdx;
		if ( DataManager.Plu.FindPluByNumber( nPluNo, nPluIdx ) == TRUE )
		{
			bFound = TRUE;
			DataManager.Plu.GetPluStockCode ( nPluNo, strStockCode, FALSE );
		
			if ( strStockCode != "" )
			{
				if ( DataManager.Stock.FindStockItemByCode( strStockCode, nStockIdx ) == FALSE )
				{	
					strStockCode = "";
					nStockIdx = -1;
				}
			}
		}
	}

	return bFound;
}

/**********************************************************************/

bool CAdjustmentsEditStockDlg::LookUpStockCode( __int64& nPluNo, CString& strStockCode, int& nStockIdx )
{
	nPluNo = 0;
	nStockIdx = -1;

	bool bFound = FALSE;

	if ( DataManager.Stock.FindStockItemByCode( strStockCode, nStockIdx ) == FALSE )
		nStockIdx = -1;
	else
	{
		bFound = TRUE;

		int nStockPluIdx;
		if ( DataManager.StockPlu.FindStockCode ( strStockCode, nStockPluIdx ) == TRUE )
			if ( DataManager.StockPlu.GetPluNoCount( nStockPluIdx ) > 0 )
				nPluNo = DataManager.StockPlu.GetPluNo( nStockPluIdx, 0 );
	}
	
	return bFound;
}

/**********************************************************************/

void CAdjustmentsEditStockDlg::OnKillFocusPluNo() 
{
	UpdateData ( TRUE, FALSE );
	m_strPluNo = BarcodePluNoTable.ConvertPlu( m_strPluNo );
	int nStockIdx;

	__int64 nPluNo = _atoi64( m_strPluNo );
	LookUpPluNo( nPluNo, m_strStockCode, nStockIdx );
	UpdateStockCode( nStockIdx );
	UpdateData(FALSE);
}

/**********************************************************************/

void CAdjustmentsEditStockDlg::OnKillFocusStockCode() 
{
	UpdateData ( TRUE, FALSE );
	m_strStockCode = BarcodePluNoTable.ConvertStockCode( m_strStockCode );
	
	int nStockIdx;
	__int64 nPluNo = 0;
	LookUpStockCode( nPluNo, m_strStockCode, nStockIdx );
	::FormatPluNo( nPluNo, m_strPluNo );
	UpdateStockCode( nStockIdx );
	UpdateData(FALSE);
	
	//WE **MUST** DO THIS HERE BECAUSE WE ARE TABBING FROM A STANDARD (NON-CSS)
	//EDIT THAT DOES NOT IMPLEMENT PAUL'S SETSEL FIX
	SetEditFocus();
}

/**********************************************************************/

void CAdjustmentsEditStockDlg::UpdateStockCode( int nStockIdx )
{		
	if ( nStockIdx != -1 )
	{
		CStockCSVRecord StockRecord;
		DataManager.Stock.GetAt( nStockIdx, StockRecord );
		m_strDescription = StockRecord.GetDescription();
		m_dCost = DataManager.Stock.CalculateRetailValue( StockRecord, 1.0 );
		m_dQuantity = 1.0;
		StockRecord.ConvertStockLevelToTotalSubUnits( 1.0, m_nTotalSubUnits );
		PrepareEditBoxes( StockRecord );

		m_staticUnitText.SetWindowText( DataManager.Stock.GetStockSubUnitNameForPluEdit( StockRecord ) );
	}
	else
	{
		m_strDescription = "Unknown";
		m_dCost = 0.0;
		m_dQuantity = 1.0;
		m_nTotalSubUnits = 1;

		CStockCSVRecord StockRecord;
		StockRecord.SetSubUnits(1);
		PrepareEditBoxes( StockRecord );

		m_staticUnitText.SetWindowText( "" );
	}
}

/**********************************************************************/

void CAdjustmentsEditStockDlg::OnKillFocusStockQty() 
{
	double dOldStockQty = m_dQuantity;
	UpdateData( TRUE, FALSE );

	//WHEN EDITING AN EXISTING LINE, WE DO NOT WANT THE VALUE TO BE
	//RECALCULATED IF THE QUANTITY REMAINS THE SAME
	if ( CPriceHelpers::CompareDoubles( dOldStockQty, m_dQuantity, 5 ) != 0 )
	{
		int nStockIdx;
		CString strStockCode = m_strStockCode;
		if ( DataManager.Stock.FindStockItemByCode( strStockCode, nStockIdx ) == TRUE )
		{
			CStockCSVRecord StockRecord;
			DataManager.Stock.GetAt( nStockIdx, StockRecord );
			m_dCost = DataManager.Stock.CalculateRetailValue( StockRecord, m_dQuantity );
		}
		else
			m_dCost = 0.0;

		UpdateData( FALSE );
	}
}

/**********************************************************************/

void CAdjustmentsEditStockDlg::OnKillFocusStockSub() 
{
	int nOldStockSub = (int)m_nTotalSubUnits; //FIDDLE - AVOID WARNING
	UpdateData( TRUE, FALSE );

	//WHEN EDITING AN EXISTING LINE, WE DO NOT WANT THE VALUE TO BE
	//RECALCULATED IF THE QUANTITY REMAINS THE SAME
	if ( nOldStockSub != m_nTotalSubUnits )
	{
		int nStockIdx;
		CString strStockCode = m_strStockCode;
		if ( DataManager.Stock.FindStockItemByCode( strStockCode, nStockIdx ) == TRUE )
		{
			CStockCSVRecord StockRecord;
			DataManager.Stock.GetAt( nStockIdx, StockRecord );
			
			double dQuantity;
			StockRecord.ConvertTotalSubUnitsToStockLevel( m_nTotalSubUnits, dQuantity );
			m_dCost = DataManager.Stock.CalculateRetailValue( StockRecord, dQuantity );
		}
		else
			m_dCost = 0.0;

		UpdateData( FALSE );
	}
}

/**********************************************************************/

void CAdjustmentsEditStockDlg::OnButtonBrowsePlu() 
{
	m_pParentWnd -> EnableWindow( FALSE );
	BrowsePluInternal();
	m_pParentWnd -> EnableWindow( TRUE );
}

/**********************************************************************/

void CAdjustmentsEditStockDlg::BrowsePluInternal()
{
	UpdateData( TRUE, FALSE );

	int nDlgResult, nPluIdx;
	::ShowPluFilterDlg( m_FilterArray, this, nDlgResult, nPluIdx );
	
	if ( IDOK != nDlgResult )
		return;

	if ( nPluIdx < 0 )
		return;
		
	CPluCSVRecord PluRecord;
	DataManager.Plu.GetAt ( nPluIdx, PluRecord );
	m_strPluNo = PluRecord.GetPluNoString();	

	int nStockIdx;
	__int64 nPluNo = _atoi64( m_strPluNo );
	LookUpPluNo( nPluNo, m_strStockCode, nStockIdx );
	UpdateStockCode( nStockIdx );
	
	UpdateData ( FALSE );

	switch ( m_nEntryMode )
	{
	case ENTRY_MODE_STOCKCODE:
		SetEditFocus();
		break;

	case ENTRY_MODE_PLUNO:
	default:
		GetEditPluNo() -> SetFocus();
		GetEditPluNo() -> SetSel ( 0, -1 );
		break;
	}
}

/**********************************************************************/

void CAdjustmentsEditStockDlg::OnButtonBrowseStock() 
{
	m_pParentWnd -> EnableWindow( FALSE );
	BrowseStockInternal();
	m_pParentWnd -> EnableWindow( TRUE );
}

/**********************************************************************/

void CAdjustmentsEditStockDlg::BrowseStockInternal()
{
	UpdateData( TRUE, FALSE );

	CStockFilterDlg dlgBrowse ( m_StockFilterArray, this );
	dlgBrowse.SetSimpleSupplierMode();
	
	if ( dlgBrowse.DoModal() != IDOK )
		return;

	int nStockIdx = dlgBrowse.GetStockIdx();
	if ( nStockIdx < 0 )
		return;

	CStockCSVRecord StockRecord;
	DataManager.Stock.GetAt ( nStockIdx, StockRecord );
	m_strStockCode = StockRecord.GetStockCode ();	
	
	__int64 nPluNo = 0;
	LookUpStockCode( nPluNo, m_strStockCode, nStockIdx );
	::FormatPluNo( nPluNo, m_strPluNo );
	UpdateStockCode( nStockIdx );

	UpdateData ( FALSE );

	switch ( m_nEntryMode )
	{
	case ENTRY_MODE_PLUNO:
		SetEditFocus();
		break;

	case ENTRY_MODE_STOCKCODE:
	default:
		m_editStockCode.SetFocus();
		m_editStockCode.SetSel ( 0, -1 );
		break;
	}
}

/**********************************************************************/

void CAdjustmentsEditStockDlg::OnRadioPluNumber() 
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

void CAdjustmentsEditStockDlg::OnRadioStockCode() 
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

void CAdjustmentsEditStockDlg::OnButtonSet() 
{
	CDataManagerInfo info;
	if ( DataManagerNonDb.OpenAdjustTextBuffer( info ) == FALSE )
		Prompter.ShareError( info );
	else
	{
		CString strText;
		m_comboReason.GetWindowText( strText );

		CListDataUnitDlg dlg ( DataManagerNonDb.AdjustText, strText, this );
		if ( dlg.DoModal() == IDOK )
		{
			if ( DataManagerNonDb.WriteAdjustTextBuffer( info ) == FALSE )
				Prompter.WriteError( info );
			else
			{
				m_comboReason.ResetContent();
				for ( int nIndex = 0; nIndex < DataManagerNonDb.AdjustText.GetSize(); nIndex++ )
					m_comboReason.AddString ( DataManagerNonDb.AdjustText.GetText( nIndex ) );
		
				m_comboReason.SetWindowText( dlg.GetSelectedText() );
			}
		}

		DataManagerNonDb.CloseAdjustTextBuffer( info );
	}
}

/**********************************************************************/

void CAdjustmentsEditStockDlg::OnOK() 
{	
	m_buttonOK.SetFocus();

	if ( UpdateData( TRUE ) == FALSE )
		return;

	int nResult = HandleAdd();
	
	switch ( nResult )
	{
	case 1:
		m_pParentWnd -> EnableWindow( FALSE );
		Prompter.Error ( "The selected stock code does not exist in your database" );
		m_pParentWnd -> EnableWindow( TRUE );
		break;

	case 2:
		m_pParentWnd -> EnableWindow( FALSE );
		Prompter.Error ( "Please enter a non-zero quantity" );
		m_pParentWnd -> EnableWindow( TRUE );
		SetEditFocus();
		return;

	case 3:
		m_pParentWnd -> EnableWindow( FALSE );
		Prompter.Error ( "You must enter a reason for each adjustment" );
		m_pParentWnd -> EnableWindow( TRUE );
		m_comboReason.SetFocus();
		return;

	case 4:
		m_pParentWnd -> EnableWindow( FALSE );
		Prompter.Error ( "The selected stock code is not in this stocktake" );
		m_pParentWnd -> EnableWindow( TRUE );
		break;

	case 5:
		m_pParentWnd -> EnableWindow( FALSE );
		Prompter.Error ( "The selected stock item is not active at this stockpoint" );
		m_pParentWnd -> EnableWindow( TRUE );
		break;

	default:
		if ( m_bAdd == TRUE )
			GetParent() -> PostMessage ( WM_APP, 0, (long) this );
		else
		{
			EndDialog( IDOK );
			return;
		}
		break;
	}

	CEdit* pNewFocus;

	switch ( m_nEntryMode )
	{
	case ENTRY_MODE_PLUNO:		pNewFocus = GetEditPluNo();	break;
	case ENTRY_MODE_STOCKCODE:	
	default:					pNewFocus = &m_editStockCode;	break;
	}
	
	pNewFocus -> SetFocus();
	pNewFocus -> SetSel( 0, -1 );
}

/**********************************************************************/

int CAdjustmentsEditStockDlg::HandleAdd() 
{
	m_buttonOK.SetFocus();

	int nStockIdx;
	if ( DataManager.Stock.FindStockItemByCode( m_strStockCode, nStockIdx ) == FALSE )
		return 1;

	double dQuantity = 0.0;
	
	CStockCSVRecord StockRecord;
	CStockLevelsCSVRecord StockLevels;
	switch ( DataManager.Stock.CheckStockLevelsForImport( nStockIdx, StockRecord, StockLevels, dbStockpoint.GetSpNo( m_nSpIdx ), m_nStkTakeNo ) )
	{
	case STOCKLEVEL_IMPORT_REJECT_STOCKTAKE:
		return 4;

	case STOCKLEVEL_IMPORT_REJECT_INACTIVE:
		return 5;
	}

	if ( GetEditStockQty() -> IsWindowEnabled() )
		dQuantity = m_dQuantity;
	else
		StockRecord.ConvertTotalSubUnitsToStockLevel( m_nTotalSubUnits, dQuantity );
	
	if ( dQuantity == 0.0 )
		return 2;

	if ( m_strLastAdjustment == "" )
		return 3;
					
	CCSVStockAudit csvOut;
	csvOut.Add ( m_strPluNo );
	csvOut.Add ( DataManager.Stock.GetStockCode( nStockIdx ) );
	csvOut.Add ( DataManager.Stock.GetDescription( nStockIdx ) );
	csvOut.AddStockAuditQty ( dQuantity );
	csvOut.AddStockAuditVal ( m_dCost );
	csvOut.Add ( m_nEntryMode );
	csvOut.Add ( DataManager.Stock.GetSubUnits( StockRecord ) );
	csvOut.Add ( m_strLastAdjustment );

	m_strOutputLine = csvOut.GetLine();

	return 0;
}

/**********************************************************************/

void CAdjustmentsEditStockDlg::OnCancel() 
{
	if ( m_bAdd == TRUE )
		DestroyWindow();
	else
		EndDialog( IDCANCEL );
}

/**********************************************************************/

void CAdjustmentsEditStockDlg::PostNcDestroy() 
{
	if ( m_bAdd == TRUE )
	{
		*m_pParentToThis = NULL;
		delete this;
	}
}

/**********************************************************************/

