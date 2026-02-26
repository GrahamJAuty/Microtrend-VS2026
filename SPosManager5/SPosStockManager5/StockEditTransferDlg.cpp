/**********************************************************************/
#include "LocalPluExcludeList.h"
#include "LocationCSVArray.h"
#include "RepCSV.h"
#include "..\CommonEcrStock\SelectMultiplePluDlg.h"
#include "StockAuditBaseReport.h"
/**********************************************************************/
#include "StockEditTransferDlg.h"
/**********************************************************************/

CStockEditTransferDlg::CStockEditTransferDlg( CSSListCtrlVirtual& StockList, CStockFilterArray* pFilterArray, CWnd* pParent)
	: CSSDialog(CStockEditTransferDlg::IDD, pParent), m_StockList ( StockList )
{
	//{{AFX_DATA_INIT(CStockEditTransferDlg)
	//}}AFX_DATA_INIT
	m_pFilterArray = pFilterArray;
	dbStockpoint.FindStockpointByNumber ( m_pFilterArray -> GetDbNo(), m_pFilterArray -> GetSpNo(), m_nSpIdx );
	dbStockpoint.FindStockpointByNumber ( m_pFilterArray -> GetDbNo(), m_pFilterArray -> GetSpNo2(), m_nSpIdx2 );
	m_dQuantity = 0.0;
	m_nTotalSubUnits = 0;
	m_nSelectedDelUnit = -1;
}

/**********************************************************************/

void CStockEditTransferDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStockEditTransferDlg)
	DDX_Control(pDX, IDC_EDIT_TO, m_editTo);
	DDX_Control(pDX, IDC_EDIT_FROM, m_editFrom);
	DDX_Control(pDX, IDC_COMBO_DELUNIT, m_comboDelUnit);
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

BEGIN_MESSAGE_MAP(CStockEditTransferDlg, CDialog)
	//{{AFX_MSG_MAP(CStockEditTransferDlg)
	ON_BN_CLICKED(IDC_DEFAULT_BUTTON, OnDefaultButton)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN, OnSpinButton)
	ON_CBN_SELCHANGE(IDC_COMBO_DELUNIT, OnSelectDeliveryUnit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/
CEdit* CStockEditTransferDlg::GetEditStockQty() { return GetEdit( IDC_EDIT_STOCK_QTY ); }
/**********************************************************************/

BOOL CStockEditTransferDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	SubclassEdit ( IDC_EDIT_STOCK_QTY, SS_NUM_SDP, 8, "%.5f" );

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

void CStockEditTransferDlg::OnDefaultButton() 
{
	SpinNext();
}

/**********************************************************************/

void CStockEditTransferDlg::OnSpinButton(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	
	if ( pNMUpDown -> iDelta > 0 )
		SpinPrevious();
	else
		SpinNext();
	
	*pResult = 0;
}

/**********************************************************************/

void CStockEditTransferDlg::SpinNext() 
{
	if ( UpdateData ( TRUE ) == FALSE )
		return;
	
	if ( m_pFilterArray -> HasNext() == FALSE )
		return;
		
	ProcessLevels();
	m_pFilterArray -> Next();
	SelectStockRecord();
	m_StockList.SetCurSel ( m_pFilterArray -> GetArrayIdx() );
}

/**********************************************************************/

void CStockEditTransferDlg::SpinPrevious() 
{
	if ( UpdateData ( TRUE ) == FALSE )
		return;
	
	if ( m_pFilterArray -> HasPrevious() == FALSE )
		return;
		
	ProcessLevels();
	m_pFilterArray -> Previous();
	SelectStockRecord();
	m_StockList.SetCurSel ( m_pFilterArray -> GetArrayIdx() );
}

/**********************************************************************/

void CStockEditTransferDlg::SelectStockRecord()
{
	CStockCSVRecord StockRecord;
	int nStockIdx = m_pFilterArray -> GetStockItemIndex(); 
	DataManager.Stock.GetAt ( nStockIdx, StockRecord );

	//PREPARE THE DELIVERY UNIT SELECTOR
	m_DelUnitSelector.FillComboBox( &m_comboDelUnit, nStockIdx );
	m_nSelectedDelUnit = 0;

	m_dQuantity = 0.0;
	m_nTotalSubUnits = 0;

	CString strText; 
	strText.Format ( "Transfer Stock %s %s",
		StockRecord.GetStockCode(),
		StockRecord.GetDescription() );
	
	SetWindowText(strText);

	CStockLevelsCSVRecord StockLevels, StockLevels2;
	DataManager.Stock.GetStockpointTotals ( nStockIdx, StockRecord, m_pFilterArray -> GetSpNo(), StockLevels );
	DataManager.Stock.GetStockpointTotals ( nStockIdx, StockRecord, m_pFilterArray -> GetSpNo2(), StockLevels2 );

	m_editFrom.SetWindowText ( DataManager.Stock.GetStockLevelString ( StockRecord, StockLevels.GetApparentQty() ) );
	m_editTo.SetWindowText ( DataManager.Stock.GetStockLevelString ( StockRecord, StockLevels2.GetApparentQty() ) );

	//PREPARE THE EDIT BOXES
	PrepareEditBoxes( StockRecord, 0 );
	UpdateData( FALSE );
	SetEditFocus();
}

/**********************************************************************/

void CStockEditTransferDlg::OnSelectDeliveryUnit() 
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

void CStockEditTransferDlg::PrepareEditBoxes( CStockCSVRecord& StockRecord, int nUnitPos )
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

void CStockEditTransferDlg::SetEditFocus()
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

void CStockEditTransferDlg::ProcessLevels()
{
	CStockCSVRecord StockRecord;
	int nStockIdx = m_pFilterArray -> GetStockItemIndex();
	DataManager.Stock.GetAt ( nStockIdx, StockRecord );
	
	double dInputStockQty;
	if ( ( DataManager.Stock.GetSubUnits( StockRecord ) >= 2 ) && ( m_nSelectedDelUnit == 0 ) )
		StockRecord.ConvertTotalSubUnitsToStockLevel( m_nTotalSubUnits, dInputStockQty );
	else 
	{
		CDelUnitInfo DelUnitInfo;
		m_DelUnitSelector.GetDelUnitInfo( DelUnitInfo );
		dInputStockQty = m_dQuantity * DelUnitInfo.m_dDUYield;
	}

	if ( dInputStockQty != 0.0 )
	{		
		DataManager.Stock.UpdateTransferDetail( m_nSpIdx, m_nSpIdx2, nStockIdx, StockRecord, dInputStockQty );

		CCSVStockAudit csv;
		csv.Add ( "D" );
		csv.Add ( AUDIT_TRANSFER );
		csv.Add ( 2 );
		csv.Add ( StockRecord.GetStockCode() );
		csv.Add ( StockRecord.GetDescription() );
		csv.Add ( DataManager.Category.GetText ( StockRecord.GetCategory() ) );
		csv.Add ( DataManager.Stock.GetSubUnits( StockRecord ) );
		csv.AddStockAuditQty ( ( m_nSelectedDelUnit != 0 ) ? m_dQuantity : 0.0 );
		csv.AddStockAuditQty ( dInputStockQty );
		
		CSSFile fileAudit;
		COleDateTime date = COleDateTime::GetCurrentTime();
		CString strAuditFile = dbStockpoint.GetFilePathAudit( m_nSpIdx, date, AUDIT_FAMILY_TEMP );
		
		if ( fileAudit.Open ( strAuditFile, "ab" ) == TRUE )
			fileAudit.WriteLine ( csv.GetLine() );

		if ( EcrmanOptions.GetPluPriceSetsFlag() == FALSE )
			return;

		int nStockPluIdx; 
		if ( DataManager.StockPlu.FindStockCode ( StockRecord.GetStockCode(), nStockPluIdx ) == FALSE )
			return;

		CArray<CSelectMultipleEntityInfo,CSelectMultipleEntityInfo> arrayPluNo;
		for ( int nIndex = 0; nIndex < DataManager.StockPlu.GetPluNoCount ( nStockPluIdx ); nIndex++ )
		{
			CSelectMultipleEntityInfo infoPlu;
			infoPlu.m_nPluNo = DataManager.StockPlu.GetPluNo ( nStockPluIdx, nIndex ); 
			arrayPluNo.Add( infoPlu );
		}
		
		CWordArray arrayLocIdx;

		int nDbNo = m_pFilterArray -> GetDbNo();
		for ( int nLocIdx = 0; nLocIdx < dbLocation.GetSize(); nLocIdx++ )
		{
			if ( dbLocation.GetDbNo( nLocIdx ) != nDbNo )
				continue;

			for ( int nEcrIdx = 0; nEcrIdx < dbLocation.GetTerminalCount( nLocIdx ); nEcrIdx++ )
			{
				if ( dbLocation.GetSpNo( nLocIdx, nEcrIdx ) == m_pFilterArray -> GetSpNo2() )
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

		if ( ( arrayPluNo.GetSize() != 0 ) && ( arrayLocIdx.GetSize() != 0 ) )
		{	
			for ( int nIndex = 0; nIndex < arrayLocIdx.GetSize(); nIndex++ )
			{
				int nLocIdx = arrayLocIdx.GetAt ( nIndex );
				
				CString strFilename;
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
						IncludeExcludeList.AddPluNo( arrayPluNo[nIndex].m_nPluNo );
					else
						IncludeExcludeList.RemovePluNo( arrayPluNo[nIndex].m_nPluNo );
				}

				IncludeExcludeList.Write ( strFilename );		
			}
		}
	}
}

/**********************************************************************/

void CStockEditTransferDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == FALSE )
		return;
	
	ProcessLevels();
	EndDialog ( IDOK );
}

/**********************************************************************/

