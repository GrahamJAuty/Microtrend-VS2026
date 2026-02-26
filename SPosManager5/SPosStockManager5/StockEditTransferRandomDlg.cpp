/**********************************************************************/
#include "FilterTypes.h"
#include "LocationCSVArray.h"
#include "LocalPluExcludeList.h"
#include "PluFilterDlgStockman.h"
#include "BarcodePluNoTable.h"
#include "..\CommonEcrStock\SelectMultiplePluDlg.h"
#include "StockAuditBaseReport.h"
#include "StockFilterDlg.h"
/**********************************************************************/
#include "StockEditTransferRandomDlg.h"
/**********************************************************************/

CStockEditTransferRandomDlg::CStockEditTransferRandomDlg( CStockFilterArray* pStockFilterArray, CWnd* pParent)
	: CSSDialog(CStockEditTransferRandomDlg::IDD, pParent), m_StockFilterArray ( FILTERTYPE_BROWSE_STOCK ), m_PluFilterArray ( FILTERTYPE_BROWSE_PLU )
{
	//{{AFX_DATA_INIT(CStockEditTransferRandomDlg)
	//}}AFX_DATA_INIT
	m_bModified = FALSE;
	m_strStockCode = "";
	m_nStockIdx = -1;
	m_dQuantity = 0.0;
	m_nTotalSubUnits = 0;
	m_nSelectedDelUnit = -1;
	m_StockFilterArray.SetFilterType ( FILTERTYPE_BROWSE_TRANSFER );
	m_StockFilterArray.SetCategoryFilter ( pStockFilterArray -> GetCategoryFilter() );
	m_StockFilterArray.SetSupplierFilter ( pStockFilterArray -> GetSupplierFilter() );
	m_StockFilterArray.SetDescriptionFilter ( pStockFilterArray -> GetDescription(), pStockFilterArray -> GetSearchType(), pStockFilterArray -> GetMatchCase() );
	m_StockFilterArray.SetSpIdx ( pStockFilterArray -> GetSpIdx(), pStockFilterArray -> GetSpIdx2() );
	m_nSpIdx = m_StockFilterArray.GetSpIdx();
	m_nSpIdx2 = m_StockFilterArray.GetSpIdx2();
	m_nSpNo = m_StockFilterArray.GetSpNo();
	m_nSpNo2 = m_StockFilterArray.GetSpNo2();
	m_nDbNo = m_StockFilterArray.GetDbNo();
}

/**********************************************************************/

void CStockEditTransferRandomDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStockEditTransferRandomDlg)
	DDX_Control(pDX, IDC_RADIO_STOCKCODE, m_radioStockCode);
	DDX_Control(pDX, IDC_RADIO_PLUNUMBER, m_radioPluNumber);
	DDX_Control(pDX, IDC_EDIT_TO, m_editTo);
	DDX_Control(pDX, IDC_EDIT_FROM, m_editFrom);
	DDX_Control(pDX, IDC_BUTTON_UNDO, m_buttonUndo);
	DDX_Control(pDX, IDC_EDIT_DESCRIPTION, m_editDescription);
	DDX_Control(pDX, IDC_BUTTON_ADD, m_buttonAdd);
	DDX_Control(pDX, IDC_EDIT_STOCKCODE, m_editStockCode);
	DDX_Control(pDX, IDC_COMBO_DELUNIT, m_comboDeliveryUnit);
	//}}AFX_DATA_MAP

	DDX_Text ( pDX, IDC_EDIT_STOCK_QTY, m_dQuantity );	
	DDX_Text(pDX, IDC_EDIT_DESCRIPTION, m_strDescription );
	DDX_Text(pDX, IDC_EDIT_STOCKCODE, m_strStockCode);
	DDV_MaxChars(pDX, m_strStockCode, Stock::StockCode.Max);
	DDX_IntegerString(pDX, IDC_EDIT_PLUNO, m_strPluNo);

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

BEGIN_MESSAGE_MAP(CStockEditTransferRandomDlg, CDialog)
	//{{AFX_MSG_MAP(CStockEditTransferRandomDlg)
	ON_EN_KILLFOCUS(IDC_EDIT_STOCKCODE, OnKillFocusStockCode)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_EN_KILLFOCUS(IDC_EDIT_PLUNO, OnKillFocusPluNo)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE_PLU, OnButtonBrowsePlu)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE_STOCK, OnButtonBrowseStock)
	ON_BN_CLICKED(IDC_BUTTON_UNDO, OnButtonUndo)
	ON_BN_CLICKED(IDC_RADIO_PLUNUMBER, OnRadioPluNumber)
	ON_BN_CLICKED(IDC_RADIO_STOCKCODE, OnRadioStockCode)
	ON_CBN_SELCHANGE(IDC_COMBO_DELUNIT, OnSelectDeliveryUnit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/
CEdit* CStockEditTransferRandomDlg::GetEditStockQty() { return GetEdit( IDC_EDIT_STOCK_QTY ); }
CEdit* CStockEditTransferRandomDlg::GetEditPluNo() { return GetEdit( IDC_EDIT_PLUNO ); }
/**********************************************************************/

BOOL CStockEditTransferRandomDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CString strName1 = dbStockpoint.GetName( m_nSpIdx );
	CString strName2 = dbStockpoint.GetName( m_nSpIdx2 );
			
	SubclassEdit ( IDC_EDIT_STOCK_QTY, SS_NUM_SDP, 8, "%.5f" );
	SubclassEdit ( IDC_EDIT_PLUNO, SS_NUM, Super.MaxPluLen(), "%s" );
	m_editStockSub.SubclassDlgItem ( IDC_EDIT_STOCKSUB, this, 11, 1, FALSE );
	
	m_listTransfer.SubclassDlgItem ( IDC_LIST, this );
	m_listTransfer.InsertColumn ( 0, "Stock Code", LVCFMT_LEFT, 120 );
	m_listTransfer.InsertColumn ( 1, "Description", LVCFMT_LEFT, 120 );
	m_listTransfer.InsertColumn ( 2, "Qty", LVCFMT_LEFT, 50 );
	m_listTransfer.InsertColumn ( 3, "Apparent From", LVCFMT_RIGHT, 85 );
	m_listTransfer.InsertColumn ( 4, "Apparent To", LVCFMT_RIGHT, 75 );
	
	CString strTitle = "";
	strTitle.Format ( "Transfer Stock (%s to %s)", 
		(const char*) strName1,
		(const char*) strName2);
	
	SetWindowText ( strTitle );

	m_DelUnitSelector.FillComboBox( &m_comboDeliveryUnit, -1, -1 );
	
	CRect rect;
	GetEditStockQty() ->GetWindowRect ( &rect );
	ScreenToClient( &rect );
	m_editStockSub.SetWindowPos( NULL, rect.left, rect.top , 0, 0, SWP_NOSIZE | SWP_NOZORDER );
	m_editStockSub.ShowWindow ( FALSE );

	OnRadioPluNumber();

	return TRUE;  
}

/**********************************************************************/

void CStockEditTransferRandomDlg::OnKillFocusStockCode() 
{
	UpdateData( TRUE, FALSE );
	m_strStockCode = BarcodePluNoTable.ConvertStockCode ( m_strStockCode );
	FindPluFromStockCode();
	CheckStockCode();
	UpdateData ( FALSE );
}

/**********************************************************************/

void CStockEditTransferRandomDlg::CheckStockCode()
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
		m_editFrom.SetWindowText ( "" );
		m_editTo.SetWindowText ( "" );
	}
	else
	{
		DataManager.Stock.GetAt ( m_nStockIdx, StockRecord );
		m_strDescription = StockRecord.GetDescription();
		PrepareEditBoxes( StockRecord, 0 );

		CStockLevelsCSVRecord StockLevels, StockLevels2;
		DataManager.Stock.GetStockpointTotals ( nStockIdx, StockRecord, m_nSpNo, StockLevels );
		DataManager.Stock.GetStockpointTotals ( nStockIdx, StockRecord, m_nSpNo2, StockLevels2 );

		m_editFrom.SetWindowText ( DataManager.Stock.GetStockLevelString ( StockRecord, StockLevels.GetApparentQty() ) );
		m_editTo.SetWindowText ( DataManager.Stock.GetStockLevelString ( StockRecord, StockLevels2.GetApparentQty() ) );
	}

	m_DelUnitSelector.FillComboBox( &m_comboDeliveryUnit, m_nStockIdx, -2 );
	m_nSelectedDelUnit = 0;
}

/**********************************************************************/

void CStockEditTransferRandomDlg::PrepareEditBoxes( CStockCSVRecord& StockRecord, int nUnitPos )
{
	int nSubUnits = 0;
	if ( nUnitPos == 0 )
		nSubUnits = DataManager.Stock.GetSubUnits( StockRecord );
		
	if ( nSubUnits <= 1 )
	{
		m_editStockSub.ShowWindow( SW_HIDE );
		m_editStockSub.EnableWindow( FALSE );
		GetEditStockQty() ->ShowWindow( SW_SHOW );
		GetEditStockQty() ->EnableWindow( TRUE );
	}
	else
	{
		m_editStockSub.ShowWindow( SW_SHOW );
		m_editStockSub.EnableWindow( TRUE );
		m_editStockSub.SetMode( 11, nSubUnits, TRUE );
		GetEditStockQty() ->ShowWindow( SW_HIDE );
		GetEditStockQty() ->EnableWindow( FALSE );
	}
}

/**********************************************************************/

void CStockEditTransferRandomDlg::OnButtonAdd() 
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
		Prompter.Error( "The selected stock item is not active at the source stockpoint" );
		break;

	case 4:
		Prompter.Error( "The selected stock item is not active at the target stockpoint" );
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

int CStockEditTransferRandomDlg::HandleAdd() 
{
	//CHECK THAT THE STOCK ITEM IS IN RANGE
	if ( m_nStockIdx < 0 || m_nStockIdx >= DataManager.Stock.GetSize() )
		return 1;

	//READ THE STOCK RECORD AND CHECK FOR INACTIVE STATUS AT SOURCE OR TARGET
	CStockCSVRecord StockRecord;
	switch( DataManager.Stock.CheckStockLevelsForTransfer( m_nStockIdx, StockRecord, m_nSpNo, m_nSpNo2 ) )
	{
	case STOCKLEVEL_IMPORT_REJECT_INACTIVE_SOURCE:
		return 3;

	case STOCKLEVEL_IMPORT_REJECT_INACTIVE_TARGET:
		return 4;
	}
	
	//WORK OUT THE TOTAL STOCK UNITS REQUIRED
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
		
	if ( dInputStockQty == 0.0 )
		return 2;	

	
	DataManager.Stock.UpdateTransferDetail( m_nSpIdx, m_nSpIdx2, m_nStockIdx, StockRecord, dInputStockQty );

	CStockLevelsCSVRecord StockLevelsFrom, StockLevelsTo;
	DataManager.Stock.GetStockpointTotals( m_nStockIdx, StockRecord, m_nSpNo, StockLevelsFrom );
	DataManager.Stock.GetStockpointTotals( m_nStockIdx, StockRecord, m_nSpNo2, StockLevelsTo );

	CString strTransferQty = DataManager.Stock.GetStockLevelString ( StockRecord, dInputStockQty );
	CString strApparentFrom = DataManager.Stock.GetStockLevelString ( StockRecord, StockLevelsFrom.GetApparentQty() );
	CString strApparentTo = DataManager.Stock.GetStockLevelString ( StockRecord, StockLevelsTo.GetApparentQty() );
		
	CCSVStockAudit csv;
	csv.Add ( m_nStockIdx );
	csv.AddStockAuditQty ( dInputStockQty );
	csv.AddStockAuditQty ( ( m_nSelectedDelUnit != 0 ) ? m_dQuantity : 0.0 );
	m_TransferArray.Add ( csv.GetLine() );
			
	CCSV csvOut ( '\t' );
	csvOut.Add ( StockRecord.GetStockCode() );
	csvOut.Add ( StockRecord.GetDescription() );
	csvOut.Add ( strTransferQty );
	csvOut.Add ( strApparentFrom );
	csvOut.Add ( strApparentTo );
	m_listTransfer.AddString ( csvOut.GetLine() );		
	m_listTransfer.SetCurSel ( m_listTransfer.GetItemCount() - 1 );
	m_buttonUndo.EnableWindow ( TRUE );

	if ( EcrmanOptions.GetPluPriceSetsFlag() == FALSE )
		return 0;

	int nStockPluIdx; 
	if ( DataManager.StockPlu.FindStockCode ( StockRecord.GetStockCode(), nStockPluIdx ) == FALSE )
		return 0;

	CArray<CSelectMultipleEntityInfo,CSelectMultipleEntityInfo> arrayPluNo;
	for ( int nIndex = 0; nIndex < DataManager.StockPlu.GetPluNoCount ( nStockPluIdx ); nIndex++ )
	{
		CSelectMultipleEntityInfo infoPlu;
		infoPlu.m_nPluNo = DataManager.StockPlu.GetPluNo ( nStockPluIdx, nIndex ); 
		arrayPluNo.Add( infoPlu );
	}

	CWordArray arrayLocIdx;
	for ( int nLocIdx = 0; nLocIdx < dbLocation.GetSize(); nLocIdx++ )
	{	
		for ( int nEcrIdx = 0; nEcrIdx < dbLocation.GetTerminalCount( nLocIdx ); nEcrIdx++ )
		{
			if ( dbLocation.GetSpNo ( nLocIdx, nEcrIdx ) == m_nSpNo2 )
			{
				arrayLocIdx.Add ( nLocIdx );
				break;
			}
		}
	}

	if ( ( arrayPluNo.GetSize() >= 2 ) && ( arrayLocIdx.GetSize() != 0 ) )
	{
		CSelectMultiplePluDlg dlg ( arrayPluNo, StockRecord.GetStockCode(), this );
		dlg.DoModal();
	}

	for ( int n = arrayPluNo.GetSize() - 1; n >= 0; n-- )
		if ( FALSE == arrayPluNo[n].m_bSelected )
			arrayPluNo.RemoveAt(n);

	if ( ( arrayPluNo.GetSize() != 0 ) && ( arrayLocIdx.GetSize() != 0  ) )
	{	
		for ( int nIndex = 0; nIndex < arrayLocIdx.GetSize(); nIndex++ )
		{
			int nLocIdx = arrayLocIdx.GetAt ( nIndex );
		
			CString strFilename = "";
			bool bExcludeByDefault = DealerFlags.GetPriceSetExcludeByDefaultFlag();

			if ( bExcludeByDefault == TRUE )
				strFilename = dbLocation.GetFilePathDataLocalPluIncludeTemp( nLocIdx );
			else
				strFilename = dbLocation.GetFilePathDataLocalPluExcludeTemp( nLocIdx );
			
			CLocalPluIncludeExcludeList IncludeExcludeList;
			IncludeExcludeList.Read ( strFilename );
				
			for ( int nIndex = 0; nIndex < arrayPluNo.GetSize(); nIndex++ )
			{
				if ( bExcludeByDefault == TRUE )
					IncludeExcludeList.AddPluNo ( arrayPluNo[nIndex].m_nPluNo );
				else
					IncludeExcludeList.RemovePluNo ( arrayPluNo[nIndex].m_nPluNo );
			}			
		
			IncludeExcludeList.Write ( strFilename );		
		}
	} 
	
	return 0;
}

/**********************************************************************/

void CStockEditTransferRandomDlg::FindStockCodeFromPlu()
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

void CStockEditTransferRandomDlg::FindPluFromStockCode()
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
		__int64 nPluNo = DataManager.StockPlu.GetPluNo ( nStockPluIdx, 0 );
		::FormatPluNo( nPluNo, m_strPluNo ); 
	}
	else
		m_strPluNo = "";
}

/**********************************************************************/

void CStockEditTransferRandomDlg::OnKillFocusPluNo() 
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

void CStockEditTransferRandomDlg::OnSelectDeliveryUnit() 
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

void CStockEditTransferRandomDlg::OnButtonBrowsePlu() 
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

void CStockEditTransferRandomDlg::OnButtonBrowseStock() 
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

void CStockEditTransferRandomDlg::OnButtonUndo() 
{
	Undo( TRUE );
}

/**********************************************************************/

void CStockEditTransferRandomDlg::Undo( bool bUpdateList )
{
	int nToGo = m_TransferArray.GetSize() - 1;

	if ( nToGo < 0 )
		return;

	CCSV csv ( m_TransferArray.GetAt ( nToGo ) );

	int nStockIdx = csv.GetInt(0);
	double dQty = csv.GetDouble(1);

	if ( nStockIdx < 0 || nStockIdx >= DataManager.Stock.GetSize() )
		return;

	CStockCSVRecord StockRecord;
	DataManager.Stock.GetAt ( nStockIdx, StockRecord );

	DataManager.Stock.UpdateTransferDetail( m_nSpIdx, m_nSpIdx2, nStockIdx, StockRecord, -dQty );

	m_TransferArray.RemoveAt ( nToGo );
	
	if ( bUpdateList == TRUE )
	{
		if ( m_listTransfer.GetCurSel() >= nToGo )
			m_listTransfer.SetCurSel ( nToGo - 1 );

		m_listTransfer.DeleteString ( nToGo );
	}

	if ( nToGo == 0 )
		m_buttonUndo.EnableWindow ( FALSE );
}

/**********************************************************************/

void CStockEditTransferRandomDlg::OnRadioPluNumber() 
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
	
void CStockEditTransferRandomDlg::OnRadioStockCode() 
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

void CStockEditTransferRandomDlg::OnCancel() 
{
	if ( m_TransferArray.GetSize() == 0 )
	{
		EndDialog ( IDCANCEL );
		return;
	}

	if ( Prompter.YesNo ( "Are you sure you wish to cancel these changes" ) == IDNO )
		return;

	while ( m_TransferArray.GetSize() > 0 )
		Undo ( FALSE );

	EndDialog ( IDCANCEL );
}

/**********************************************************************/

void CStockEditTransferRandomDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == FALSE )
		return;
	
	if ( m_TransferArray.GetSize() == 0 )
	{
		EndDialog ( IDOK );
		return;
	}

	if ( Prompter.YesNo ( "Are you sure you wish to process these changes", TRUE ) == IDNO )
		return;

	CSSTime timeNow;
	CSSDate dateNow;
	timeNow.SetCurrentTime();
	dateNow.SetCurrentDate();
	
	int nSpIdx;
	dbStockpoint.FindStockpointByNumber ( m_nDbNo, m_nSpNo, nSpIdx );
	
	COleDateTime date = COleDateTime::GetCurrentTime();
	CString strAuditFile = dbStockpoint.GetFilePathAudit( nSpIdx, date, AUDIT_FAMILY_TEMP );
	
	CSSFile fileAudit;
	if ( fileAudit.Open ( strAuditFile, "ab" ) == TRUE )
	{
		for ( int nIndex = 0; nIndex < m_TransferArray.GetSize(); nIndex++ )
		{
			CCSV csvIn ( m_TransferArray.GetAt ( nIndex ) );
			int nStockIdx = csvIn.GetInt(0);
			double dQty = csvIn.GetDouble(1);

			if ( nStockIdx < 0 || nStockIdx >= DataManager.Stock.GetSize() )
				continue;

			CStockCSVRecord StockRecord;
			DataManager.Stock.GetAt ( nStockIdx, StockRecord );

			CCSVStockAudit csvOut;
			csvOut.Add ( "D" );
			csvOut.Add ( AUDIT_TRANSFER );
			csvOut.Add ( 2 );
			csvOut.Add ( StockRecord.GetStockCode() );
			csvOut.Add ( StockRecord.GetDescription() );
			csvOut.Add ( DataManager.Category.GetText ( StockRecord.GetCategory() ) );
			csvOut.Add ( DataManager.Stock.GetSubUnits( StockRecord ) );
			csvOut.AddStockAuditQty ( csvIn.GetDouble(2) );
			csvOut.AddStockAuditQty ( dQty );	
			fileAudit.WriteLine ( csvOut.GetLine() );
		}
	}
	EndDialog ( IDOK );
}

/**********************************************************************/



