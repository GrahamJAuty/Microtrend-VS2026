/**********************************************************************/
#include "BarcodePluNoTable.h"
#include "CommsHandshaker.h"
#include "..\CommonEcrStock\DefEditDlg.h"
#include "FilterTypes.h"
#include "..\CommonEcrStock\ListDataPresetDlg.h"
#include "MaxLengths.h"
#include "Password.h"
#include "..\CommonEcrStock\PresetReportManager.h"
#include "ProcessedOrderReportDelivery.h"
#include "RepSpawn.h"
#include "..\CommonEcrStock\SelectPresetActionDlg.h"
#include "StatusProgress.h"
#include "StockAuditAdjustmentStock.h"
#include "StockAuditAdjustmentPlu.h"
#include "StockAuditDeliveryDetail.h"
#include "StockAuditDeliverySummary.h"
#include "StockAuditExceptionsSales.h"
#include "StockAuditItemActivity.h"
#include "StockAuditOpeningClosing.h"
#include "StockAuditReturns.h"
#include "StockAuditSales.h"
#include "StockAuditStocktake.h"
#include "StockAuditTransfer.h"
#include "StockFilterDlg.h"
#include "StockOptionsIni.h"
#include "StockReportExceptionsDlg.h"
#include "StockReportNames.h"
#include "StockReportSalesAuditDlg.h"
#include "StockReportSupplierDlg.h"
#include "SysInfo.h"
/**********************************************************************/
#include "ReportAuditDlg.h"
/**********************************************************************/
extern CSupplierCSVArray SupplierArray;
/**********************************************************************/

CReportAuditDlg::CReportAuditDlg( int nPresetNo, bool bAutoRunPreset, CWnd* pParent)
	: CSSDialog("IDD_REPORT_AUDIT", pParent),
	m_DateSelectorRange( m_comboDates, m_DatePickerFrom, m_DatePickerTo )
{
	//{{AFX_DATA_INIT(CReportAuditDlg)
	//}}AFX_DATA_INIT
	m_nReportType = AUDIT_DELIVERY;
	m_nDefSelectIdx = 1;
	m_nDefSelectType = NODE_PLACEHOLDER;
	m_nSpIdx = 0;
	m_nPresetNo = nPresetNo;
	m_nPresetError = 0;
	m_bAutoRunPreset = bAutoRunPreset;
	m_bSelectHeader = FALSE;
	m_bSelectLine = FALSE;
	m_bSelectPageBreak = FALSE;
}

/**********************************************************************/

void CReportAuditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CReportAuditDlg)
	
	DDX_Control(pDX, IDC2_STATIC_PRESET, m_staticPreset);
	DDX_Control(pDX, IDC2_BUTTON_PRESET, m_buttonPreset);
	DDX_Control(pDX, IDC2_COMBO_PRESET, m_comboPreset);

	DDX_Control(pDX, IDC2_COMBO_CONLEVEL, m_comboConLevel);
	
	DDX_Control(pDX, IDC2_COMBO_DATES, m_comboDates);
	DDX_Control(pDX, IDC2_DATEPICKER_TO, m_DatePickerTo);
	DDX_Control(pDX, IDC2_DATEPICKER_FROM, m_DatePickerFrom);
	
	DDX_Control(pDX, IDC2_EDIT_HEADER, m_editHeader);
	DDX_Control(pDX, IDC2_EDIT_LINE, m_editLine);
	DDX_Control(pDX, IDC2_CHECK_PAGEBREAK, m_checkPageBreak);

	DDX_Control(pDX, IDC2_CHECK_STOCK, m_checkStock);
	DDX_Control(pDX, IDC2_EDIT_STOCKCODE_TO, m_editStockCodeTo);
	DDX_Control(pDX, IDC2_BUTTON_BROWSETO, m_buttonBrowseTo);
	DDX_Control(pDX, IDC2_EDIT_STOCKCODE_FROM, m_editStockCodeFrom);
	DDX_Control(pDX, IDC2_BUTTON_BROWSEFROM, m_buttonBrowseFrom);
	
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CReportAuditDlg, CDialog)
	//{{AFX_MSG_MAP(CReportAuditDlg)

	ON_WM_MEASUREITEM()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC2_LIST, OnSelectReport)
	ON_NOTIFY(NM_DBLCLK, IDC2_LIST, OnDoubleClickList)
	ON_LBN_DBLCLK(IDC2_LIST, OnButtonPrint)
	
	ON_BN_CLICKED(IDC2_BUTTON_PRESET, OnButtonPreset)
	ON_CBN_SELCHANGE(IDC2_COMBO_PRESET, OnSelectPreset)
	ON_CBN_SELCHANGE(IDC2_COMBO_CONLEVEL, OnSelectConLevel)
	
	ON_CBN_SELCHANGE(IDC2_COMBO_DATES, OnSetDates)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC2_DATEPICKER_FROM, OnChangeDateFrom)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC2_DATEPICKER_TO, OnChangeDateTo)
	
	ON_BN_CLICKED(IDC2_CHECK_STOCK, OnToggleStock)
	ON_EN_KILLFOCUS(IDC2_EDIT_STOCKCODE_FROM, OnKillFocusStockCode)
	ON_BN_CLICKED(IDC2_BUTTON_BROWSEFROM, OnButtonBrowseFrom)
	ON_EN_KILLFOCUS(IDC2_EDIT_STOCKCODE_TO, OnKillFocusStockCode)
	ON_BN_CLICKED(IDC2_BUTTON_BROWSETO, OnButtonBrowseTo)

	ON_BN_CLICKED(IDOK, OnButtonPrint)

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

void CReportAuditDlg::SetDefaultSelection( int nDefSelectIdx, int nDefSelectType )
{
	m_nDefSelectIdx = nDefSelectIdx;
	m_nDefSelectType = nDefSelectType;
}

/**********************************************************************/

BOOL CReportAuditDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_listReports.SubclassDlgItem ( IDC2_LIST, this );
	m_listReports.InsertColumn ( 0, "", LVCFMT_LEFT, 220 );
	m_listReports.ModifyStyle( LVS_OWNERDRAWFIXED, 0, 0 );
	m_listReports.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );

	m_editHeader.LimitText( MAX_LENGTH_GENERAL_REFERENCE );
	m_editLine.LimitText( MAX_LENGTH_GENERAL_REFERENCE );
	
	CWordArray arrayTypes;

	switch( PasswordArray.GetLocationAccessType() )
	{
	case LOCATION_ACCESS_SYSTEM:
		arrayTypes.Add ( NODE_STOCKPOINT );
		arrayTypes.Add ( NODE_SITE );
		arrayTypes.Add ( NODE_DATABASE );
		arrayTypes.Add ( NODE_SYSTEM );
		break;

	case LOCATION_ACCESS_DB:
		arrayTypes.Add ( NODE_STOCKPOINT );
		arrayTypes.Add ( NODE_SITE );
		arrayTypes.Add ( NODE_DATABASE );
		m_LocationSelector.SetSingleDbIdx( PasswordArray.GetLocationAccessDbIdx() );
		break;

	case LOCATION_ACCESS_LOC:
		arrayTypes.Add ( NODE_STOCKPOINT );
		m_LocationSelector.SetSingleSpIdx( PasswordArray.GetLocationAccessSpIdx() );
		break;
	}

	m_LocationSelector.SetDatabaseMode ( NODE_STOCKPOINT );

	m_LocationSelector.BuildList( arrayTypes );
	m_LocationSelector.FillLocationCombo ( &m_comboConLevel );
	m_LocationSelector.ForceSelection ( m_nDefSelectType, m_nDefSelectIdx );
	GetConDetails();

	CWordArray arrayTemp;
	arrayTemp.Add( AUDIT_OPENING );
	arrayTemp.Add( AUDIT_DELIVERY );
	arrayTemp.Add( AUDIT_DELIVERY_SUMMARY );
	arrayTemp.Add( AUDIT_RETURNS );
	arrayTemp.Add( AUDIT_RETURNS_SUMMARY );
	arrayTemp.Add( AUDIT_SALES);

	if ( StockOptions.GetStockAdjustmentsEntryType() == 0 )
		arrayTemp.Add( AUDIT_ADJUSTMENT_STOCK );
	else
		arrayTemp.Add( AUDIT_ADJUSTMENT_PLU );

	arrayTemp.Add( AUDIT_TRANSFER );
	arrayTemp.Add( AUDIT_STOCKTAKE );
	arrayTemp.Add( AUDIT_ITEM_ACTIVITY );
	arrayTemp.Add( AUDIT_EX_IMPORT );
	arrayTemp.Add( AUDIT_EX_SALES );
	
	int nPos = 0;
	for ( int nIndex = 0; nIndex < arrayTemp.GetSize(); nIndex++ )
	{
		bool bAdd;
		int nRepType = arrayTemp.GetAt( nIndex );

		switch ( nRepType )
		{
		case AUDIT_TRANSFER:
			bAdd =  ( Sysset.GetMaxStockPoints() > 1 ) ? TRUE : FALSE;
			break;
				
		default:
			bAdd = TRUE;
			break;
		}

		if ( bAdd == TRUE )
		{
			CStockAuditBaseReport Audit;
			Audit.SetReportType( nRepType );

			m_listReports.InsertItem ( nPos, "" );
			m_listReports.SetItem ( nPos, 0, LVIF_TEXT, Audit.GetReportName(), 0, 0, 0, NULL );	
			m_listReports.SetItemData ( nPos, nRepType );
			
			nPos++;
		}
	}

	m_listReports.InsertItem ( 2, "" );
	m_listReports.SetItem ( 2, 0, LVIF_TEXT, "Processed Deliveries", 0, 0, 0, NULL );
	m_listReports.SetItemData( 2, ORDER_DELIVERY );

	m_nReportType = -1;
	m_listReports.SetCurSel(1);
	SelectReport();
	
	m_editHeader.EnableWindow ( m_bSelectHeader );
	m_editLine.EnableWindow ( m_bSelectLine );

	m_DateSelectorRange.InitialiseDateControls();
	m_DateSelectorRange.AddReportRange( "Stock Period" );
	m_DateSelectorRange.AddStandardReportRanges();
	m_DateSelectorRange.SetComboSelection( 0, szRANGE_TODAY );

	{
		int nResult = m_comboDates.FindStringExact( 0, EcrmanOptions.GetReportsDefaultDateRange() );
			
		if ( nResult != CB_ERR )
			m_comboDates.SetCurSel( nResult );
		else
			m_comboDates.SetCurSel( 0 );

		OnSetDates();
	}

	m_editStockCodeFrom.EnableWindow ( FALSE );
	m_editStockCodeTo.EnableWindow ( FALSE );
	m_buttonBrowseFrom.EnableWindow ( FALSE );
	m_buttonBrowseTo.EnableWindow ( FALSE );

	PresetReportManager.StockAudit.SetPasswordAccess();
	FillPresetCombo(-1);

	if ( m_nPresetNo != 0 )
	{
		m_nPresetError = PresetReportManager.SelectFromCombo( NODE_PRESET_STOCK_AUDIT, m_comboPreset, m_nPresetNo );

		if ( 0 == m_nPresetError )
		{
			OnSelectPreset();

			if ( TRUE == m_bAutoRunPreset )
				OnButtonPrint();
		}

		if ( TRUE == m_bAutoRunPreset )
		{
			EndDialog( IDCANCEL );
			return FALSE;
		}			
	}

	bool bPreset = PasswordArray.CheckLocationAccessSystem();
	ShowAndEnableWindow( &m_buttonPreset, bPreset );
	ShowAndEnableWindow( &m_staticPreset, FALSE == bPreset );

	return TRUE;
}

/**********************************************************************/

void CReportAuditDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	if ( nIDCtl == IDC2_LIST )	
	{
		m_listReports.AdjustRowHeight( lpMeasureItemStruct, 8 );
	}
}

/**********************************************************************/

void CReportAuditDlg::GetConDetails()
{
	CLocationSelectorEntity LocSelEntity;
	m_LocationSelector.GetSelectedEntity( LocSelEntity );

	if ( LocSelEntity.GetConType() == NODE_STOCKPOINT )
		m_nSpIdx = LocSelEntity.GetEntityIdx();
}

/**********************************************************************/

void CReportAuditDlg::FillPresetCombo( int nPresetIdxSel )
{
	PresetReportManager.FillPresetCombo( NODE_PRESET_STOCK_AUDIT, m_comboPreset, nPresetIdxSel );
}

/**********************************************************************/

void CReportAuditDlg::OnSelectReport(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

    if ((pNMListView->uChanged & LVIF_STATE) 
        && (pNMListView->uNewState & LVNI_SELECTED))
    {
		SelectReport();
	}
}

/**********************************************************************/

void CReportAuditDlg::SelectReport() 
{
	UpdateData( TRUE, FALSE );

	int nSelection = m_listReports.GetCurSel();

	if ( nSelection >= 0 && nSelection < m_listReports.GetItemCount() )
	{
		int nOldReportType = m_nReportType; 
		m_nReportType = m_listReports.GetItemData ( nSelection );
		
		if ( m_nReportType != nOldReportType )
		{
			switch ( m_nReportType )
			{
			case ORDER_DELIVERY: 
			case AUDIT_DELIVERY:
			case AUDIT_DELIVERY_SUMMARY:
			case AUDIT_ADJUSTMENT_STOCK:
			case AUDIT_ADJUSTMENT_PLU:
			case AUDIT_RETURNS:
			case AUDIT_RETURNS_SUMMARY:
			case AUDIT_TRANSFER:
			case AUDIT_STOCKTAKE:
			case AUDIT_EX_IMPORT:
				m_bSelectHeader = TRUE;
				break;

			default:
				m_bSelectHeader = FALSE;
				break;
			}

			switch( m_nReportType )
			{
			case AUDIT_ADJUSTMENT_STOCK:
			case AUDIT_ADJUSTMENT_PLU:
			case AUDIT_DELIVERY:
			case AUDIT_RETURNS:
				m_bSelectLine = TRUE;
				break;

			default:
				m_bSelectLine = FALSE;
				break;
			}

			switch ( m_nReportType )
			{
			case ORDER_DELIVERY:
			case AUDIT_DELIVERY:
			case AUDIT_RETURNS:
			case AUDIT_ADJUSTMENT_STOCK:
			case AUDIT_ADJUSTMENT_PLU:
			case AUDIT_TRANSFER:
			case AUDIT_EX_IMPORT:
				m_bSelectPageBreak = TRUE;
				break;

			default:
				m_bSelectPageBreak = FALSE;
				break;
			}

			bool bSelectStockCodes = TRUE;
			switch( m_nReportType )
			{
			case AUDIT_EX_IMPORT:
			case AUDIT_EX_SALES:
				m_checkStock.EnableWindow( FALSE );
				m_editStockCodeFrom.EnableWindow( FALSE );
				m_editStockCodeTo.EnableWindow( FALSE );
				m_buttonBrowseFrom.EnableWindow( FALSE );
				m_buttonBrowseTo.EnableWindow( FALSE );
				break;

			default:
				m_checkStock.EnableWindow( TRUE );
				OnToggleStock();
				break;
			}

			m_editHeader.EnableWindow ( m_bSelectHeader );
			m_editLine.EnableWindow( m_bSelectLine );
			m_checkPageBreak.EnableWindow ( m_bSelectPageBreak );
				
			m_comboDates.EnableWindow ( TRUE );
			m_DateSelectorRange.UpdateDateControls();
		}
	}
}

/**********************************************************************/

void CReportAuditDlg::OnSelectConLevel() 
{
	UpdateData( TRUE, FALSE );

	GetConDetails();

	if ( m_comboDates.GetCurSel() == 0 )
		SetStartDate();
		
	UpdateData ( FALSE );
}

/**********************************************************************/

void CReportAuditDlg::SetStartDate()
{
	CString strDate = dbStockpoint.GetPeriodStart( m_nSpIdx );
	
	int nYear = atoi ( strDate.Right(4) );
	int nMonth = atoi ( strDate.Mid(2,2) );
	int nMDay = atoi ( strDate.Left(2) );
	
	CTime timeFrom, timeTo;
	CSSDateRange DateRange;
	DateRange.GetRange ( szRANGE_TODAY, timeFrom, timeTo );
	timeFrom = CTime ( nYear, nMonth, nMDay, 0, 0, 0, -1 );
	
	m_DatePickerFrom.SetTime ( &timeFrom );
	m_DatePickerTo.SetTime ( &timeTo );

	m_DateSelectorRange.SetCurrentDatesFromControls();
}

/**********************************************************************/

void CReportAuditDlg::OnSetDates() 
{
	m_DateSelectorRange.UpdateDateControls();

	int nSelection = m_comboDates.GetCurSel();
	if ( nSelection >= 0 && nSelection < m_comboDates.GetCount() )
	{
		CString strComboText;
		m_comboDates.GetLBText ( nSelection, strComboText );

		if ( strComboText == "Stock Period" )
			SetStartDate();
	}
}

/**********************************************************************/

void CReportAuditDlg::OnChangeDateFrom(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_DateSelectorRange.OnChangeFromDate();
	m_DateSelectorRange.CheckForChangedDate();
	*pResult = 0;
}

/**********************************************************************/

void CReportAuditDlg::OnChangeDateTo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_DateSelectorRange.CheckForChangedDate();
	*pResult = 0;
}

/**********************************************************************/

void CReportAuditDlg::OnKillFocusStockCode() 
{
	CString strStockCodeFrom, strStockCodeTo;
	m_editStockCodeFrom.GetWindowText( strStockCodeFrom );
	m_editStockCodeTo.GetWindowText( strStockCodeTo );
	strStockCodeFrom = BarcodePluNoTable.ConvertStockCode ( strStockCodeFrom );
	strStockCodeTo = BarcodePluNoTable.ConvertStockCode ( strStockCodeTo );
	m_editStockCodeFrom.SetWindowText( strStockCodeFrom );
	m_editStockCodeTo.SetWindowText( strStockCodeTo );
}

/**********************************************************************/

void CReportAuditDlg::OnToggleStock() 
{
	bool bEnable = ( m_checkStock.GetCheck() != 0 ); 
	m_editStockCodeFrom.EnableWindow ( bEnable );
	m_editStockCodeTo.EnableWindow ( bEnable );
	m_buttonBrowseFrom.EnableWindow ( bEnable );
	m_buttonBrowseTo.EnableWindow ( bEnable );
}

/**********************************************************************/

void CReportAuditDlg::OnButtonBrowseFrom() 
{
	CLocationSelectorEntity LocSetEntity;
	m_LocationSelector.GetSelectedEntity( LocSetEntity );

	{
		CWaitCursor wait;
		CDataManagerInfo info;
		DataManager.OpenDatabaseReadOnly( LocSetEntity.GetDbIdx(), info, FALSE );
	}

	CStockFilterArray StockFilterArray ( FILTERTYPE_BROWSE_STOCK );
	StockFilterArray.SetSupplierFilter ( 0 );
	StockFilterArray.SetCategoryFilter ( 0 );
	StockFilterArray.SetCheckApparentFlag( FALSE );
		
	CStockFilterDlg dlgBrowse ( StockFilterArray, this );	
	if ( dlgBrowse.DoModal() != IDOK )
		return;

	int nStockIdx = dlgBrowse.GetStockIdx();
	if ( nStockIdx < 0 )
		return;

	CStockCSVRecord StockRecord;
	DataManager.Stock.GetAt ( nStockIdx, StockRecord );
	m_editStockCodeFrom.SetWindowText( StockRecord.GetStockCode() );	
	UpdateData ( FALSE );
	m_editStockCodeFrom.SetFocus();
	m_editStockCodeFrom.SetSel ( 0, -1 );	
}

/**********************************************************************/

void CReportAuditDlg::OnButtonBrowseTo() 
{
	CLocationSelectorEntity LocSetEntity;
	m_LocationSelector.GetSelectedEntity( LocSetEntity );

	{
		CWaitCursor wait;
		CDataManagerInfo info;
		DataManager.OpenDatabaseReadOnly( LocSetEntity.GetDbIdx(), info, FALSE );
	}
	
	CWaitCursor wait;
	CStockFilterArray StockFilterArray ( FILTERTYPE_BROWSE_STOCK );
	StockFilterArray.SetSupplierFilter ( 0 );
	StockFilterArray.SetCategoryFilter ( 0 );
	StockFilterArray.SetCheckApparentFlag( FALSE );
	
	CStockFilterDlg dlgBrowse ( StockFilterArray, this );
	if ( dlgBrowse.DoModal() != IDOK )
		return;

	int nStockIdx = dlgBrowse.GetStockIdx();
	if ( nStockIdx < 0 )
		return;

	CStockCSVRecord StockRecord;
	DataManager.Stock.GetAt ( nStockIdx, StockRecord );
	m_editStockCodeTo.SetWindowText( StockRecord.GetStockCode() );	
	UpdateData ( FALSE );
	m_editStockCodeTo.SetFocus();
	m_editStockCodeTo.SetSel ( 0, -1 );	
}

/**********************************************************************/

void CReportAuditDlg::OnButtonPreset() 
{
	CPresetReportInfo Preset;
	CreatePreset( Preset );
	PresetReportManager.PresetButton( NODE_PRESET_STOCK_AUDIT, m_comboPreset, Preset, m_pParentWnd );
}

/**********************************************************************/

void CReportAuditDlg::CreatePreset( CPresetReportInfo& Base ) 
{
	CPresetReportInfoStockAudit Preset;

	int nSel = m_listReports.GetCurSel();
	if ( ( nSel >= 0 ) && ( nSel < m_listReports.GetItemCount() ) )
		Preset.SetReportType( m_listReports.GetItemData( nSel ) );

	CString strDateType;
	m_comboDates.GetWindowText( strDateType );
	Preset.SetDateType( strDateType );

	CString strDateFrom;
	COleDateTime dateFrom;
	m_DatePickerFrom.GetTime( dateFrom );
	::GetStringFromOleDate( dateFrom, strDateFrom );
	Preset.SetDateFrom( strDateFrom );
	
	CString strDateTo;
	COleDateTime dateTo;
	m_DatePickerTo.GetTime( dateTo );
	::GetStringFromOleDate( dateTo, strDateTo );
	Preset.SetDateTo( strDateTo );

	CLocationSelectorEntity LocSelEntity;
	m_LocationSelector.GetSelectedEntity( LocSelEntity );
	Base.SetPresetConLevel( LocSelEntity.GetConType() );

	int nEntityNo1, nEntityNo2;
	CPresetReportArray::GetLocSelEntityNo( LocSelEntity, nEntityNo1, nEntityNo2 );
	Base.SetPresetEntityNo1( nEntityNo1 );
	Base.SetPresetEntityNo2( nEntityNo2 );

	Preset.SetStockRangeFlag( m_checkStock.GetCheck() != 0 );

	CString strStockFrom;
	m_editStockCodeFrom.GetWindowText( strStockFrom );
	Preset.SetStockFrom( strStockFrom );

	CString strStockTo;
	m_editStockCodeTo.GetWindowText( strStockTo );
	Preset.SetStockTo( strStockTo );

	Preset.SetOnePerPageFlag( m_checkPageBreak.GetCheck() != 0 );
	
	CString strHeader;
	m_editHeader.GetWindowText( strHeader );
	Preset.SetHeaderReference( strHeader );

	CString strLine;
	m_editLine.GetWindowText( strLine );
	Preset.SetLineReference( strLine );

	CString str;
	Preset.ToString( str );
	Base.SetOtherFields( str );
}

/**********************************************************************/

void CReportAuditDlg::OnSelectPreset() 
{
	int nSel = m_comboPreset.GetCurSel() - 1;
	if ( nSel < 0 || nSel >= PresetReportManager.StockAudit.GetSize() )
		return;

	CPresetReportInfo Base;
	PresetReportManager.StockAudit.GetAt( nSel, Base );

	CPresetReportInfoStockAudit Preset;
	{
		CString str = Base.GetOtherFields();
		Preset.FromString( str );
	}

	int nReportType = Preset.GetReportType();
	for ( int n = 0; n < m_listReports.GetItemCount(); n++ )
	{
		if ( m_listReports.GetItemData(n) == nReportType )
		{
			m_listReports.SetCurSel(n);
			break;
		}
	}

	SelectReport();

	//DATE GROUP
	int nResult = m_comboDates.FindStringExact( 0, Preset.GetDateType() );
		
	if ( nResult != CB_ERR )
		m_comboDates.SetCurSel( nResult );
	
	CString strDateFrom = Preset.GetDateFrom();
	CString strDateTo = Preset.GetDateTo();

	COleDateTime dateFrom, dateTo;
	
	if ( ::GetOleDateFromString( strDateFrom, dateFrom ) == TRUE )
		m_DatePickerFrom.SetTime( dateFrom );

	if ( ::GetOleDateFromString( strDateTo, dateTo ) == TRUE )
		m_DatePickerTo.SetTime( dateTo );

	OnSetDates();

	int nConLevel = Base.GetPasswordConLevel();
	int nEntity1 = Base.GetPasswordEntityNo1();
	int nEntity2 = Base.GetPasswordEntityNo2();
	
	int nEntityIdx = 0;
	CPresetReportArray::GetEntityIdx( nConLevel, nEntity1, nEntity2, nEntityIdx );	
	m_LocationSelector.ForceSelection( nConLevel, nEntityIdx );

	m_checkStock.SetCheck( Preset.GetStockRangeFlag() );
	OnToggleStock();

	CString strStockFrom = Preset.GetStockFrom();
	if ( strStockFrom.GetLength() <= Stock::StockCode.Max )
		m_editStockCodeFrom.SetWindowText( strStockFrom );

	CString strStockTo = Preset.GetStockTo();
	if ( strStockTo.GetLength() <= Stock::StockCode.Max )
		m_editStockCodeTo.SetWindowText( strStockTo );

	m_checkPageBreak.SetCheck( Preset.GetOnePerPageFlag() );
	m_editHeader.SetWindowText( Preset.GetHeaderReference() );
	m_editLine.SetWindowText( Preset.GetLineReference() );
}

/**********************************************************************/

void CReportAuditDlg::OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{	
	OnButtonPrint();
	*pResult = 0;
}

/**********************************************************************/

void CReportAuditDlg::OnButtonPrint() 
{
	UpdateData( TRUE, FALSE );
	CommsHandshaker.ProcessSales();
	OnKillFocusStockCode();
	
	switch ( m_nReportType )
	{
	case ORDER_DELIVERY:				PrintOrderDeliveryAudit();	break;
	case AUDIT_DELIVERY:				PrintDeliveryAudit( DealerFlags.GetDeliveryDetailFlag() );	break;
	case AUDIT_RETURNS:					PrintReturnsAudit();			break;
	case AUDIT_DELIVERY_SUMMARY:		PrintDeliverySummary();			break;
	case AUDIT_RETURNS_SUMMARY:			PrintDeliverySummary();			break;
	case AUDIT_ADJUSTMENT_STOCK:		PrintAdjustmentStockAudit();	break;
	case AUDIT_ADJUSTMENT_PLU:			PrintAdjustmentPluAudit();		break;
	case AUDIT_OPENING:					PrintOpeningAudit();			break;
	case AUDIT_STOCKTAKE:				PrintStocktakingAudit();		break;		
	case AUDIT_SALES:					PrintSalesAudit();				break;
	case AUDIT_TRANSFER:				PrintTransferAudit();			break;
	case AUDIT_ITEM_ACTIVITY:			PrintItemActivityAudit();		break;
	case AUDIT_EX_IMPORT:				PrintExceptionsAudit();			break;
	case AUDIT_EX_SALES:				PrintSalesExceptionsAudit();	break;
}
}

/**********************************************************************/

void CReportAuditDlg::PrintOrderDeliveryAudit()
{		
	CLocationSelectorEntity LocSetEntity;
	m_LocationSelector.GetSelectedEntity( LocSetEntity );

	if ( LocSetEntity.GetConType() == NODE_SYSTEM )
	{
		Prompter.Error ( "This report cannot be\ncreated at system level" );
		return;
	}

	CString strFrom = "";
	CString strTo = "";
	if ( m_checkStock.GetCheck() != 0 )
	{
		m_editStockCodeFrom.GetWindowText( strFrom );
		m_editStockCodeTo.GetWindowText( strTo );
	}

	CString strHeader = "";
	if ( m_editHeader.IsWindowEnabled() == TRUE )
	{
		m_editHeader.GetWindowText( strHeader );
		::TrimSpaces( strHeader, FALSE );
	}

	CDataManagerInfo info;
	DataManager.OpenDatabaseReadOnly( 0, info, FALSE );

	CStockReportSupplierDlg dlg ( m_wSuppSelectArray, this );
	if ( dlg.DoModal() == IDCANCEL )
		return;

	if ( m_wSuppSelectArray.GetSize() < 1 )
	{
		Prompter.Error ( "You must select at least one supplier\nbefore you can view this report." );
		return;
	}
				
	CProcessedOrderReportDelivery* pReport = new CProcessedOrderReportDelivery ( m_wSuppSelectArray );
	
	if ( pReport != NULL )
	{
		pReport -> SetLatestFlag ( FALSE );
		pReport -> SetPageBreakFlag ( m_checkPageBreak.GetCheck() != 0 );
		
		COleDateTime dateFrom, dateTo;
		m_DatePickerFrom.GetTime( dateFrom );
		m_DatePickerTo.GetTime( dateTo );

		CString strDateFrom, strDateTo;
		strDateFrom.Format ( "%2.2d/%2.2d/%4.4d", dateFrom.GetDay(), dateFrom.GetMonth(), dateFrom.GetYear() );
		strDateTo.Format ( "%2.2d/%2.2d/%4.4d", dateTo.GetDay(), dateTo.GetMonth(), dateTo.GetYear() );

		pReport -> SetReportType ( m_nReportType );
		pReport -> SetConLevel ( LocSetEntity.GetConType() );
		pReport -> SetTableNo (LocSetEntity.GetEntityNo() );
		pReport -> SetConLevelText ( LocSetEntity.GetEntityName() );
		pReport -> SetDateFrom ( strDateFrom );
		pReport -> SetDateTo ( strDateTo );
		pReport -> SetReference( strHeader );
		pReport -> SetStockCodeFrom( strFrom );
		pReport -> SetStockCodeTo( strTo );
		
		StatusProgress.Lock( TRUE, "Creating report" );
		bool bResult = ( pReport -> CreateReport( LocSetEntity.GetDbIdx() ) );
		StatusProgress.Unlock();

		if ( bResult == TRUE )
		{
			CReportManager ReportManager;
			ReportManager.DisplayReport ( Super.ReportFilename(), pReport -> GetReportParamsFilename( ORDER_DELIVERY ), "", this );
		}
		
		delete pReport;
	}
}

/**********************************************************************/

void CReportAuditDlg::PrintDeliveryAudit( bool bDetail )
{
	CStockAuditDeliveryDetail* pReport = new CStockAuditDeliveryDetail( bDetail );
			
	if ( pReport != NULL )
	{
		pReport -> SetPageBreakFlag ( m_checkPageBreak.GetCheck() != 0 );
		PrintAudit ( pReport );
		delete pReport;
	}
}

/**********************************************************************/

void CReportAuditDlg::PrintDeliverySummary()
{
	CStockAuditDeliverySummary* pReport = new CStockAuditDeliverySummary;

	if ( pReport != NULL )
	{
		PrintAudit ( pReport );
		delete pReport;
	}
}	

/**********************************************************************/

void CReportAuditDlg::PrintReturnsAudit()
{
	CStockAuditReturns* pReport = new CStockAuditReturns();
			
	if ( pReport != NULL )
	{
		pReport -> SetPageBreakFlag ( m_checkPageBreak.GetCheck() != 0 );
		PrintAudit ( pReport );
		delete pReport;
	}
}

/**********************************************************************/

void CReportAuditDlg::PrintAdjustmentStockAudit()
{
	CStockAuditAdjustmentStock* pReport = new CStockAuditAdjustmentStock;

	if ( pReport != NULL )
	{
		pReport -> SetPageBreakFlag ( m_checkPageBreak.GetCheck() != 0 );
		pReport -> SetWaitForDataFlag();
		PrintAudit ( pReport );
		delete pReport;
	}
}	

/**********************************************************************/

void CReportAuditDlg::PrintAdjustmentPluAudit()
{
	CStockAuditAdjustmentPlu* pReport = new CStockAuditAdjustmentPlu;

	if ( pReport != NULL )
	{
		pReport -> SetPageBreakFlag ( m_checkPageBreak.GetCheck() != 0 );
		pReport -> SetWaitForDataFlag();
		PrintAudit ( pReport );
		delete pReport;
	}
}	

/**********************************************************************/

void CReportAuditDlg::PrintOpeningAudit()
{
	CStockAuditOpeningClosing* pReport = new CStockAuditOpeningClosing;

	if ( pReport != NULL )
	{
		pReport -> SetAuditMode ( AUDIT_MODE_NORMAL );
		PrintAudit ( pReport );
		delete pReport;
	}
}	

/**********************************************************************/

void CReportAuditDlg::PrintStocktakingAudit()
{
	CStockAuditStocktake* pReport = new CStockAuditStocktake;

	if ( pReport != NULL )
	{
		pReport -> SetAuditMode ( AUDIT_MODE_NORMAL );
		PrintAudit ( pReport );
		delete pReport;
	}
}	

/**********************************************************************/
	
void CReportAuditDlg::PrintSalesAudit()
{
	CStockReportSalesAuditDlg dlg ( FALSE, this );

	if ( dlg.DoModal() == IDCANCEL )
		return;

	CStockAuditSales* pReport = new CStockAuditSales;

	if ( pReport != NULL )
	{
		pReport -> SetAuditMode ( AUDIT_MODE_NORMAL );
		pReport -> SetE1SalesFlag ( dlg.m_bE1Sales != 0 );
		pReport -> SetImportSalesFlag ( dlg.m_bImportSales != 0 );
		pReport -> SetManualSalesFlag ( dlg.m_bManualSales != 0 );
		PrintAudit ( pReport );
		delete pReport;
	}
}	

/**********************************************************************/
	
void CReportAuditDlg::PrintTransferAudit()
{
	CStockAuditTransfer* pReport = new CStockAuditTransfer;

	if ( pReport != NULL )
	{
		pReport -> SetPageBreakFlag ( m_checkPageBreak.GetCheck() != 0 );
		PrintAudit ( pReport );
		delete pReport;
	}
}	

/**********************************************************************/

void CReportAuditDlg::PrintItemActivityAudit()
{
	CStockAuditItemActivity* pReport = new CStockAuditItemActivity;

	if ( pReport != NULL )
	{
		pReport -> SetPageBreakFlag ( m_checkPageBreak.GetCheck() != 0 );
		PrintAudit ( pReport );
		delete pReport;
	}
}	

/**********************************************************************/

void CReportAuditDlg::PrintExceptionsAudit()
{
	CStockReportExceptionsDlg dlg ( m_ExceptionTypes, this );
	if ( dlg.DoModal() == IDCANCEL )
		return;

	CStockAuditExceptions* pReport = new CStockAuditExceptions ( m_ExceptionTypes );
	pReport -> SetWaitForDataFlag();
	pReport -> SetPageBreakFlag ( m_checkPageBreak.GetCheck() != 0 );
	
	if ( pReport != NULL )
	{
		pReport -> SetAuditMode ( AUDIT_MODE_NORMAL );
		PrintAudit ( pReport );
		delete pReport;
	}
}	

/**********************************************************************/

void CReportAuditDlg::PrintSalesExceptionsAudit()
{
	CStockReportSalesAuditDlg dlg ( TRUE, this );

	if ( dlg.DoModal() == IDCANCEL )
		return;

	CStockAuditExceptionsSales* pReport = new CStockAuditExceptionsSales;
				
	if ( pReport != NULL )
	{
		pReport -> SetAuditMode ( AUDIT_MODE_NORMAL );
		pReport -> SetE1SalesFlag ( dlg.m_bE1Sales != 0 );
		pReport -> SetImportSalesFlag ( dlg.m_bImportSales != 0 );

		if ( SysInfo.IsPMSSystem() == TRUE )
			pReport -> SetManualSalesFlag ( dlg.m_bManualSales != 0 );
		else
			pReport -> SetManualSalesFlag ( FALSE );

		PrintAudit ( pReport );
		delete pReport;
	}
}	

/**********************************************************************/

void CReportAuditDlg::PrintAudit ( CStockAuditBaseReport* pReport )
{
	CLocationSelectorEntity LocSetEntity;
	m_LocationSelector.GetSelectedEntity( LocSetEntity );

	COleDateTime dateFrom, dateTo;
	m_DatePickerFrom.GetTime( dateFrom );
	m_DatePickerTo.GetTime( dateTo );

	CString strFrom = "";
	CString strTo = "";
	if ( m_checkStock.GetCheck() != 0 )
	{
		m_editStockCodeFrom.GetWindowText( strFrom );
		m_editStockCodeTo.GetWindowText( strTo );
	}
	
	pReport -> SetReportType ( m_nReportType );
	pReport -> SetConLevel ( LocSetEntity.GetConType() );
	pReport -> SetTableNo ( LocSetEntity.GetEntityNo() );
	pReport -> SetConLevelText ( LocSetEntity.GetEntityName() );
	pReport -> SetAuditDateFrom ( dateFrom );
	pReport -> SetAuditDateTo ( dateTo );
	pReport -> SetStockCodeFrom( strFrom );
	pReport -> SetStockCodeTo( strTo );

	CString strHeader = "";
	if ( m_editHeader.IsWindowEnabled() == TRUE )
	{
		m_editHeader.GetWindowText( strHeader );
		::TrimSpaces ( strHeader, FALSE );
	}

	CString strLine = "";
	if ( m_editLine.IsWindowEnabled() == TRUE )
	{
		m_editLine.GetWindowText( strLine );
		::TrimSpaces ( strLine, FALSE );
	}

	pReport -> SetHeaderReferenceFilter( strHeader );		
	pReport -> SetLineReferenceFilter( strLine );
		
	StatusProgress.Lock( TRUE, "Creating report" );
	bool bResult = ( pReport -> CreateReport( LocSetEntity.GetDbIdx() ) );
	StatusProgress.Unlock();

	if ( bResult == TRUE )
	{
		CReportManager ReportManager;
		ReportManager.DisplayReport ( Super.ReportFilename(), pReport -> GetReportParamsFilename(), "", this );
	}
}

/**********************************************************************/
