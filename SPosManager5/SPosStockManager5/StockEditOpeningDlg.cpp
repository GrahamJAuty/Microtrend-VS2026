/**********************************************************************/
 
/**********************************************************************/
//#include "DataManager.h"
#include "StockAuditBaseReport.h"
/**********************************************************************/
#include "StockEditOpeningDlg.h"
/**********************************************************************/

CStockEditOpeningDlg::CStockEditOpeningDlg( CSSListCtrlVirtual& StockList, CStockFilterArray* pFilterArray, bool bStocktake, CWnd* pParent)
	: CSSDialog(CStockEditOpeningDlg::IDD, pParent), m_StockList( StockList )
{
	//{{AFX_DATA_INIT(CStockEditOpeningDlg)
	//}}AFX_DATA_INIT
	m_pFilterArray = pFilterArray;
	m_bStocktake = bStocktake;
	m_nAction = 0;
	m_nSelectedDelUnit = -1;
	dbStockpoint.FindStockpointByNumber ( m_pFilterArray -> GetDbNo(), m_pFilterArray -> GetSpNo(), m_nSpIdx );
}

/**********************************************************************/

void CStockEditOpeningDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStockEditOpeningDlg)
	DDX_Control(pDX, IDC_COMBO_DELUNIT, m_comboDelUnit);
	DDX_Radio(pDX, IDC_RADIO_ADD, m_nAction);
	//}}AFX_DATA_MAP
	
	DDX_Text ( pDX, IDC_EDIT_STOCK_QTY, m_dQuantity );	
	
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

BEGIN_MESSAGE_MAP(CStockEditOpeningDlg, CDialog)
	//{{AFX_MSG_MAP(CStockEditOpeningDlg)
	ON_BN_CLICKED(IDC_DEFAULT_BUTTON, OnDefaultButton)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN, OnSpinButton)
	ON_CBN_SELCHANGE(IDC_COMBO_DELUNIT, OnSelectDeliveryUnit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/
CEdit* CStockEditOpeningDlg::GetEditStockQty() { return GetEdit( IDC_EDIT_STOCK_QTY ); }
/**********************************************************************/

BOOL CStockEditOpeningDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	SubclassEdit( IDC_EDIT_STOCK_QTY, SS_NUM_SDP, 8, "%.5f" );
	
	m_editStockSub.SubclassDlgItem ( IDC_EDIT_STOCKSUB, this, 11, 1, TRUE );
	
	SelectStockRecord();
	UpdateData( FALSE );

	CRect rect;
	GetEditStockQty() -> GetWindowRect ( &rect );
	ScreenToClient( &rect );
	m_editStockSub.SetWindowPos( NULL, rect.left, rect.top , 0, 0, SWP_NOSIZE | SWP_NOZORDER );

	return TRUE;  
}

/**********************************************************************/

void CStockEditOpeningDlg::OnDefaultButton() 
{
	SpinNext();
}

/**********************************************************************/

void CStockEditOpeningDlg::OnSpinButton(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	
	if ( pNMUpDown -> iDelta > 0 )
		SpinPrevious();
	else
		SpinNext();
	
	*pResult = 0;
}

/**********************************************************************/

void CStockEditOpeningDlg::SpinNext() 
{
	if ( UpdateData ( TRUE, TRUE ) == TRUE )
	{
		if ( m_pFilterArray -> HasNext() == TRUE )
		{
			ProcessLevels();
			m_pFilterArray -> Next();
			SelectStockRecord();
			m_StockList.SetCurSel( m_pFilterArray -> GetArrayIdx() );
		}
	}
}

/**********************************************************************/

void CStockEditOpeningDlg::SpinPrevious() 
{
	if ( UpdateData ( TRUE, TRUE ) == TRUE )
	{
		if ( m_pFilterArray -> HasPrevious() == TRUE )
		{
			ProcessLevels();
			m_pFilterArray -> Previous();
			SelectStockRecord();
			m_StockList.SetCurSel( m_pFilterArray -> GetArrayIdx() );
		}
	}
}

/**********************************************************************/

void CStockEditOpeningDlg::SelectStockRecord()
{
	CStockCSVRecord StockRecord;
	int nStockIdx = m_pFilterArray -> GetStockItemIndex(); 
	DataManager.Stock.GetAt ( nStockIdx, StockRecord );

	//PREPARE THE DELIVERY UNIT SELECTOR
	m_DelUnitSelector.FillComboBox( &m_comboDelUnit, nStockIdx );
	m_nSelectedDelUnit = 0;

	//SET THE WINDOW TITLE
	CString strType = ( m_bStocktake == TRUE ) ? "Closing Stock" : "Opening Stock";
	
	CString strText; 
	strText.Format ( "%s %s %s",
		( const char* ) strType,
		StockRecord.GetStockCode(),
		StockRecord.GetDescription() );
	
	SetWindowText(strText);

	//PREPARE THE INITIAL VALUES
	m_dQuantity = 0.0;
	m_nTotalSubUnits = 0;

	if ( m_nAction == OVERWRITE_STOCK_LEVELS )
	{
		CStockLevelsCSVRecord StockLevels;
		DataManager.Stock.GetStockpointTotals( nStockIdx, StockRecord, m_pFilterArray -> GetSpNo(), StockLevels );

		if ( m_bStocktake == FALSE )
			m_dQuantity = StockLevels.GetOpeningQty();
		else	
			m_dQuantity = StockLevels.GetStocktakeClosingQty();

		if ( DataManager.Stock.GetSubUnits( StockRecord ) >= 2 )
			StockRecord.ConvertStockLevelToTotalSubUnits( m_dQuantity, m_nTotalSubUnits );
	}

	//PREPARE THE EDIT BOXES
	PrepareEditBoxes( StockRecord, 0 );
	UpdateData( FALSE );
	SetEditFocus();
}

/**********************************************************************/

void CStockEditOpeningDlg::OnSelectDeliveryUnit() 
{
	int nNewUnit = m_comboDelUnit.GetCurSel();

	if ( nNewUnit != m_nSelectedDelUnit && nNewUnit >= 0 )
	{
		m_nSelectedDelUnit = nNewUnit;

		m_dQuantity = 0.0;
		m_nTotalSubUnits = 0;
		
		CStockCSVRecord StockRecord;
		int nStockIdx = m_pFilterArray -> GetStockItemIndex();
		DataManager.Stock.GetAt( nStockIdx, StockRecord );

		PrepareEditBoxes ( StockRecord, nNewUnit );	
		UpdateData( FALSE );
	}
}

/**********************************************************************/

void CStockEditOpeningDlg::PrepareEditBoxes( CStockCSVRecord& StockRecord, int nUnitPos )
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

void CStockEditOpeningDlg::SetEditFocus()
{
	if ( GetEditStockQty() -> IsWindowEnabled() )
	{
		GetEditStockQty() -> SetSel( 0, -1 );
		GetEditStockQty() -> SetFocus();
	}
	else
	{
		m_editStockSub.SetSel( 0, -1 );
		m_editStockSub.SetFocus();
	}
}

/**********************************************************************/

void CStockEditOpeningDlg::ProcessLevels()
{
	CStockCSVRecord StockRecord;
	int nStockIdx = m_pFilterArray -> GetStockItemIndex();
	DataManager.Stock.GetAt ( nStockIdx, StockRecord );
	
	double dCurrentStockQty = 0.0;
	double dOldStockQty = 0.0;
	
	double dInputStockQty;
	if ( ( DataManager.Stock.GetSubUnits( StockRecord ) >= 2 ) && ( m_nSelectedDelUnit == 0 ) )
		StockRecord.ConvertTotalSubUnitsToStockLevel( m_nTotalSubUnits, dInputStockQty );
	else 
	{
		CDelUnitInfo DelUnitInfo;
		m_DelUnitSelector.GetDelUnitInfo( DelUnitInfo );
		dInputStockQty = m_dQuantity * DelUnitInfo.m_dDUYield;
	}

	CStockLevelsCSVRecord StockLevels;
	DataManager.Stock.GetStockpointTotals( nStockIdx, StockRecord, m_pFilterArray -> GetSpNo(), StockLevels );
	
	if ( m_bStocktake == TRUE )
		dOldStockQty = StockLevels.GetStocktakeClosingQty();
	else	
		dOldStockQty = StockLevels.GetOpeningQty();
		
	switch ( m_nAction )
	{
	case OVERWRITE_STOCK_LEVELS:
		dCurrentStockQty = dInputStockQty;
		break;

	case SUBTRACT_STOCK_LEVELS:
		dCurrentStockQty = dOldStockQty - dInputStockQty;
		break;

	case ADD_STOCK_LEVELS:
	default:
		dCurrentStockQty = dOldStockQty + dInputStockQty;
		break;
	}

	if ( dCurrentStockQty != dOldStockQty )
	{
		if ( m_bStocktake == TRUE )
			StockLevels.SetStocktakeClosingQty ( dCurrentStockQty );
		else
			StockLevels.SetOpeningQty ( dCurrentStockQty );

		DataManager.Stock.SetStockpointTotals( nStockIdx, StockRecord, m_pFilterArray -> GetSpNo(), StockLevels );
	
		CCSVStockAudit csv;
		csv.Add ( "D" );
		csv.Add ( ( m_bStocktake ) ? AUDIT_CLOSING : AUDIT_OPENING );
		csv.Add ( 2 );
		csv.Add ( StockRecord.GetStockCode() );
		csv.Add ( StockRecord.GetDescription() );
		csv.Add ( DataManager.Category.GetText ( StockRecord.GetCategory() ) );
		csv.Add ( DataManager.Stock.GetSubUnits( StockRecord ) );
		csv.Add ( m_nAction );
		csv.AddStockAuditQty ( ( m_nSelectedDelUnit != 0 ) ? m_dQuantity : 0.0 );
		csv.AddStockAuditQty ( dInputStockQty );
		csv.AddStockAuditQty ( dCurrentStockQty );
		
		CSSFile fileAudit;
		COleDateTime date = COleDateTime::GetCurrentTime();
		CString strAuditFile = dbStockpoint.GetFilePathAudit( m_nSpIdx, date, AUDIT_FAMILY_TEMP );
		
		if ( fileAudit.Open ( strAuditFile, "ab" ) == TRUE )
			fileAudit.WriteLine ( csv.GetLine() );
	}
}

/**********************************************************************/

void CStockEditOpeningDlg::OnOK() 
{
	if ( UpdateData ( TRUE, TRUE ) == TRUE )
	{
		ProcessLevels();
		EndDialog ( IDOK );
	}
}

/**********************************************************************/


