/**********************************************************************/
#include <process.h>
/**********************************************************************/
#include "BarcodePluNoTable.h"
#include "DateSelectorRange.h"
#include "FilterTypes.h"
#include "MaxLengths.h"
#include "Password.h"
#include "PosTrayOptions.h"
#include "SalesHistoryModifierDlg.h"
#include "SalesHistoryPriceLevelDlg.h"
#include "SalesHistoryTaxDlg.h"
#include "SysInfo.h"
/**********************************************************************/
#include "PosTrayBatchExportSalesDlg.h"
/**********************************************************************/

CPosTrayBatchExportSalesDlg::CPosTrayBatchExportSalesDlg( CPosTrayTask& BatchEmail, CWnd* pParent /*=NULL*/)
	: CSSDialog(CPosTrayBatchExportSalesDlg::IDD, pParent), 
		m_FilterArray ( FILTERTYPE_BROWSE_PLU ),
		m_DepartmentSelector ( m_editDeptNo, m_comboDeptName ),
		m_BatchEmail( BatchEmail ),
		m_EditHelpers( m_comboPriceLevel, m_comboTaxBand, m_comboModifier )
{
	//{{AFX_DATA_INIT(CPosTrayBatchExportSalesDlg)
	//}}AFX_DATA_INIT
	m_nTabPageFilter = 0;
	m_pFieldsDlg = NULL;
	m_bShowVIPTick = ( EcrmanOptions.GetReportsVIPType() != 0 );
}

/**********************************************************************/

CPosTrayBatchExportSalesDlg::~CPosTrayBatchExportSalesDlg()
{
	if ( m_pFieldsDlg != NULL )
		delete m_pFieldsDlg;
}

/**********************************************************************/

void CPosTrayBatchExportSalesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPosTrayBatchExportSalesDlg)
	
	DDX_Control(pDX, IDC_TAB_FILTER, m_TabFilter );

	DDX_Control(pDX, IDC_STATIC_NAME, m_staticName);
	DDX_Control(pDX, IDC_EDIT_NAME, m_editName);

	DDX_Control(pDX, IDC_STATIC_DATE, m_staticDate);
	DDX_Control(pDX, IDC_STATIC_DATE1, m_staticDate1);
	DDX_Control(pDX, IDC_STATIC_DATE2, m_staticDate2);
	DDX_Control(pDX, IDC_STATIC_DATE3, m_staticDate3);
	DDX_Control(pDX, IDC_STATIC_DATE4, m_staticDate4);
	DDX_Control(pDX, IDC_STATIC_DATE5, m_staticDate5);
	DDX_Control(pDX, IDC_COMBO_DATES, m_comboDates);
	DDX_Control(pDX, IDC_COMBO_ROUND, m_comboRound);
	DDX_Control(pDX, IDC_COMBO_SESSION, m_comboSession);
	DDX_Control(pDX, IDC_COMBO_TIMESLICE, m_comboTimeSlice);
	DDX_Control(pDX, IDC_COMBO_PLUTIME, m_comboPluTime);
	
	DDX_Control(pDX, IDC_STATIC_DATABASE, m_staticDatabase);
	DDX_Control(pDX, IDC_STATIC_DATABASE1, m_staticDatabase1);
	DDX_Control(pDX, IDC_STATIC_DATABASE2, m_staticDatabase2);
	DDX_Control(pDX, IDC_COMBO_LOCATION, m_comboLocation);
	DDX_Control(pDX, IDC_COMBO_TERMINAL, m_comboTerminal);
	DDX_Control(pDX, IDC_STATIC_TABLE, m_staticTable);
	DDX_Control(pDX, IDC_COMBO_TABLE, m_comboTable);
	DDX_Control(pDX, IDC_CHECK_EPOSSALES, m_checkEposSales);
	DDX_Control(pDX, IDC_CHECK_MANUALSALES, m_checkManualSales);
	DDX_Control(pDX, IDC_CHECK_IMPORTSALES, m_checkImportSales);

	DDX_Control(pDX, IDC_STATIC_FILTER, m_staticFilter);
	DDX_Control(pDX, IDC_STATIC_FILTER1, m_staticFilter1);
	DDX_Control(pDX, IDC_STATIC_FILTER2, m_staticFilter2);
	DDX_Control(pDX, IDC_CHECK_SPECIFY, m_checkSpecify);
	DDX_Control(pDX, IDC_BUTTON_BROWSE_FROM, m_buttonBrowseFrom);
	DDX_Control(pDX, IDC_BUTTON_BROWSE_TO, m_buttonBrowseTo);
	DDX_IntegerString(pDX, IDC_EDIT_PLU_FROM, m_strPluNoFrom);
	DDX_IntegerString(pDX, IDC_EDIT_PLU_TO, m_strPluNoTo);
	DDX_Control(pDX, IDC_EDIT_DEPTNO, m_editDeptNo);
	DDX_Control(pDX, IDC_COMBO_DEPTNAME, m_comboDeptName);
	DDX_Control(pDX, IDC_COMBO_PLUFILTER, m_comboPluFilter);
	DDX_Control(pDX, IDC_BUTTON_TAXBAND, m_buttonTaxBand);
	DDX_Control(pDX, IDC_COMBO_TAXBAND, m_comboTaxBand);
	DDX_Control(pDX, IDC_BUTTON_PRICELEVEL, m_buttonPriceLevel);
	DDX_Control(pDX, IDC_COMBO_PRICELEVEL, m_comboPriceLevel);
	DDX_Control(pDX, IDC_BUTTON_MODIFIER, m_buttonModifier);
	DDX_Control(pDX, IDC_COMBO_MODIFIER, m_comboModifier);

	DDX_Control(pDX, IDC_STATIC_MISC, m_staticMisc);
	DDX_Control(pDX, IDC_CHECK_GUEST, m_checkGuest);
	DDX_Control(pDX, IDC_CHECK_CUSTOMER, m_checkCustomer);
	DDX_Control(pDX, IDC_CHECK_VIPSALES, m_checkVIPSales);
	DDX_Control(pDX, IDC_STATIC_MISC1, m_staticMisc1);
	DDX_Control(pDX, IDC_COMBO_TAXFIELD, m_comboTaxField);

	DDX_Control(pDX, IDOK, m_buttonOK);
	DDX_Control(pDX, IDCANCEL, m_buttonCancel);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPosTrayBatchExportSalesDlg, CDialog)
	//{{AFX_MSG_MAP(CPosTrayBatchExportSalesDlg)

	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_FILTER, OnSelectTabFilter)

	ON_BN_CLICKED(IDC_CHECK_SPECIFY, OnTogglePluRange)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE_FROM, OnButtonBrowseFrom)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE_TO, OnButtonBrowseTo)
	ON_EN_KILLFOCUS(IDC_EDIT_PLU_FROM, OnKillFocusPluFrom)
	ON_EN_KILLFOCUS(IDC_EDIT_PLU_TO, OnKillFocusPluTo)
	
	ON_CBN_SELCHANGE(IDC_COMBO_DEPTNAME, OnSelectDepartment)
	ON_EN_KILLFOCUS(IDC_EDIT_DEPTNO, OnKillFocusDeptNo)
	
	ON_BN_CLICKED(IDC_BUTTON_TAXBAND, OnButtonTaxBand)
	ON_BN_CLICKED(IDC_BUTTON_PRICELEVEL, OnButtonPriceLevel)
	ON_BN_CLICKED(IDC_BUTTON_MODIFIER, OnButtonModifier)
	
	ON_CBN_SELCHANGE(IDC_COMBO_LOCATION, OnSelectLocation)

	ON_WM_MEASUREITEM()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDoubleClickList)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST, OnGetDispInfoList)
	ON_BN_CLICKED(IDC_BUTTON_FIELD_ADD, OnButtonFieldAdd)
	ON_BN_CLICKED(IDC_BUTTON_FIELD_CHANGE, OnButtonFieldChange)
	ON_BN_CLICKED(IDC_BUTTON_FIELD_REMOVE, OnButtonFieldRemove)
	ON_BN_CLICKED(IDC_BUTTON_FIELD_TIDY, OnButtonFieldTidy)
	ON_BN_CLICKED(IDC_BUTTON_FIELD_UP, OnButtonFieldMoveUp)
	ON_BN_CLICKED(IDC_BUTTON_FIELD_DOWN, OnButtonFieldMoveDown)

	ON_MESSAGE( WM_APP, OnAddField)

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/
CEdit* CPosTrayBatchExportSalesDlg::GetEditPluFrom() { return GetEdit( IDC_EDIT_PLU_FROM ); }
CEdit* CPosTrayBatchExportSalesDlg::GetEditPluTo() { return GetEdit( IDC_EDIT_PLU_TO ); }
/**********************************************************************/

BOOL CPosTrayBatchExportSalesDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_buttonTaxBand.SetWindowText( EcrmanOptions.GetTaxBandString() );

	m_EditHelpers.InitTabbedCombos();

	m_BatchEmail.GetExportFieldListSales( m_arrayFieldTypes );

	m_listFields.SubclassDlgItem ( IDC_LIST, this );
	m_listFields.InsertColumn ( 1, "No", LVCFMT_LEFT, 40 );
	m_listFields.InsertColumn ( 2, "Type", LVCFMT_LEFT, 210 );
	m_listFields.InsertColumn ( 3, "Sub Type", LVCFMT_LEFT, 110 );
	m_listFields.SetItemCountEx( m_arrayFieldTypes.GetSize() );
	m_listFields.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_listFields.SetCurSel(0);

	m_DeptAndPluComboHelpers.SetControlPointers( &m_comboPluFilter, &m_LocationSelector, &m_DepartmentSelector );
	
	SubclassEdit ( IDC_EDIT_PLU_FROM, SS_NUM, Super.MaxPluLen(), "%s" );
	SubclassEdit ( IDC_EDIT_PLU_TO, SS_NUM, Super.MaxPluLen(), "%s" );

	FillTimeSliceCombo();
	
	CWordArray arrayTypes;

	switch( PasswordArray.GetLocationAccessType() )
	{
	case LOCATION_ACCESS_SYSTEM:
		arrayTypes.Add ( NODE_LOCATION );
		arrayTypes.Add ( NODE_DATABASE );
		arrayTypes.Add ( NODE_LOCATIONSET );
		arrayTypes.Add ( NODE_REPORTPOINT );
		arrayTypes.Add ( NODE_SYSTEM );
		break;

	case LOCATION_ACCESS_DB:
		arrayTypes.Add ( NODE_LOCATION );
		arrayTypes.Add ( NODE_DATABASE );
		arrayTypes.Add ( NODE_LOCATIONSET );
		arrayTypes.Add ( NODE_REPORTPOINT );
		m_LocationSelector.SetSingleDbIdx( PasswordArray.GetLocationAccessDbIdx() );
		break;

	case LOCATION_ACCESS_LOC:
		arrayTypes.Add ( NODE_LOCATION );
		m_LocationSelector.SetSingleLocIdx( PasswordArray.GetLocationAccessLocIdx() );
		break;
	}

	m_LocationSelector.BuildList( arrayTypes );
	
	if ( SysInfo.IsEcrLiteSystem() == FALSE )
	{
		m_LocationSelector.FillLocationCombo ( &m_comboLocation, &m_comboTerminal, &m_comboTable, &m_TableReportHelpers );
	}
	else
	{
		m_LocationSelector.FillLocationCombo ( &m_comboLocation, &m_comboTerminal );
		ShowAndEnableWindow( &m_staticTable, FALSE );
		ShowAndEnableWindow( &m_comboTable, FALSE );
		MoveControl( &m_checkEposSales, 21, 285 );
		MoveControl( &m_checkManualSales, 21, 300 );
		MoveControl( &m_checkImportSales, 21, 315 );
	}

	//PLU SALE TIME
	m_comboPluTime.AddString( "Transaction completion time" );
	m_comboPluTime.AddString( "Item sale time" );
	
	m_editDeptNo.LimitText( MAX_LENGTH_DEPTNO );

	if ( PosTrayOptions.GetBusinessDateFlag() == FALSE )
	{
		m_comboDates.AddString ( szRANGE_TODAY );
		m_comboDates.AddString ( szRANGE_YESTERDAY );
	}
	else
	{
		m_comboDates.AddString ( szRANGE_THIS_BUSDAY );
		m_comboDates.AddString ( szRANGE_LAST_BUSDAY );
	}
	CDateSelectorRange::AddSingleDayRanges ( m_comboDates );
	CDateSelectorRange::AddPreviousDayRanges( m_comboDates );
	m_comboDates.AddString ( szRANGE_THIS_WEEK );
	m_comboDates.AddString ( szRANGE_THIS_MONTH );
	CDateSelectorRange::AddThisTaxQuarterRange( m_comboDates );
	m_comboDates.AddString ( szRANGE_THIS_YEAR );
	m_comboDates.AddString ( szRANGE_THIS_FINANCIAL_YEAR );
	m_comboDates.AddString ( szRANGE_LAST_WEEK );
	m_comboDates.AddString ( szRANGE_LAST_MONTH );
	CDateSelectorRange::AddLastTaxQuarterRange( m_comboDates );
	m_comboDates.AddString ( szRANGE_LAST_YEAR );
	m_comboDates.AddString ( szRANGE_LAST_FINANCIAL_YEAR );
	m_comboDates.AddString ( szRANGE_LAST_12_MONTHS );
	m_comboDates.SetCurSel ( 0 );

	for ( int nIndex = 0; nIndex < DataManagerNonDb.ReportSession.GetSize(); nIndex++ )
		m_comboSession.AddString( DataManagerNonDb.ReportSession.GetName( nIndex ) );

	if ( EcrmanOptions.GetReportsEODSessionFlag() == TRUE )
		m_comboSession.AddString( "EOD Business Day" );

	if ( m_comboSession.GetCount() == 0 )
		m_comboSession.InsertString ( 0, "00:00 to 23:59" );
	
	m_comboSession.EnableWindow( m_comboSession.GetCount() > 1 );
	m_comboSession.SetCurSel(0);

#ifndef STOCKMAN_SYSTEM
	m_checkEposSales.SetCheck( TRUE );
	m_checkManualSales.SetCheck( FALSE );
	m_checkImportSales.SetCheck( FALSE );
	m_checkEposSales.ShowWindow ( SW_HIDE );
	m_checkManualSales.ShowWindow ( SW_HIDE );
	m_checkImportSales.ShowWindow ( SW_HIDE );
#endif

	m_comboRound.AddString ( "Dates as selected" );
	m_comboRound.AddString ( "Nearest full week" );
	m_comboRound.AddString ( "Nearest full month" );
	m_comboRound.SetCurSel(0);

	if ( SysInfo.IsPMSSystem() == FALSE )
	{
		m_checkGuest.SetCheck( FALSE );
		m_checkGuest.EnableWindow( FALSE );
		m_checkGuest.ShowWindow( SW_HIDE );
	}

	{
		m_comboTaxField.AddString( "Same as sales value" );
		m_comboTaxField.AddString( "No decimal places" );
		m_comboTaxField.AddString( "1 decimal place" );
		for ( int n = 2; n <= 8; n++ )
		{
			CString str;
			str.Format( "%d decimal places", n );
			m_comboTaxField.AddString( str );
		}
	}

	LoadEmailBatchSettings();

	m_TabFilter.InsertItem( 0, "General" );
	m_TabFilter.InsertItem( 1, "Advanced" );
	m_nTabPageFilter = 0;
	SelectTabFilter();

	{
		CString strTaxField = EcrmanOptions.GetSalesTaxName();
		strTaxField += " field width";
		m_staticMisc1.SetWindowTextA( strTaxField );
	}

	MoveControl( &m_staticFilter, 7, 28 );
	MoveControl( &m_staticFilter1, 21, 148 );
	MoveControl( &m_staticFilter2, 21, 85);
	MoveControl( &m_checkSpecify, 21, 46 );
	MoveControl( GetEditPluFrom(), 77, 46 );
	MoveControl( GetEditPluTo(), 77, 65 );
	MoveControl( &m_buttonBrowseFrom, 173, 46 );
	MoveControl( &m_buttonBrowseTo, 173, 65 );
	MoveControl( &m_comboPluFilter, 77, 85 );
	MoveControl( &m_buttonModifier, 21, 114 );
	MoveControl( &m_comboModifier, 77, 114 );
	MoveControl( &m_editDeptNo, 77, 148 );
	MoveControl( &m_comboDeptName, 105, 148 );
	MoveControl( &m_buttonTaxBand, 21, 182 );
	MoveControl( &m_comboTaxBand, 77, 182 );
	MoveControl( &m_buttonPriceLevel, 21, 202 );
	MoveControl( &m_comboPriceLevel, 77, 202 );
	MoveControl( &m_staticMisc, 7, 248 );
	MoveControl( &m_checkGuest, 21, 267 );

	int nYPos = ( SysInfo.IsPMSSystem() ? 281 : 267 );
	MoveControl( &m_checkCustomer, 21, nYPos );

	if ( TRUE == m_bShowVIPTick )
	{
		nYPos += 14;
		MoveControl( &m_checkVIPSales, 21, nYPos );
	}

	nYPos += 20;
	MoveControl( &m_staticMisc1, 21, nYPos );
	MoveControl( &m_comboTaxField, 77, nYPos );

	MoveControl( &m_buttonOK, 540, 313 );
	MoveControl( &m_buttonCancel, 540, 329 );
	ResizeDialog( 597, 350 );

	return TRUE;  
}

/**********************************************************************/

void CPosTrayBatchExportSalesDlg::OnSelectTabFilter(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;

	m_nTabPageFilter = m_TabFilter.GetCurSel();
	if ( ( m_nTabPageFilter < 0 ) || ( m_nTabPageFilter > 1 ) )
		m_nTabPageFilter = 0;

	SelectTabFilter();
}

/**********************************************************************/

void CPosTrayBatchExportSalesDlg::SelectTabFilter()
{
	bool bEnableSalesType = FALSE;
	{
		CLocationSelectorEntity LocSelEntity;
		m_LocationSelector.GetSelectedEntity( LocSelEntity );

		switch( LocSelEntity.GetConType() )
		{
		case NODE_DATABASE:
		case NODE_SYSTEM:
			bEnableSalesType = TRUE;
			break;
		}
	}

	ShowAndEnableWindow( &m_staticName, 0 == m_nTabPageFilter );
	ShowAndEnableWindow( &m_editName, 0 == m_nTabPageFilter );
	ShowAndEnableWindow( &m_staticDate, 0 == m_nTabPageFilter );
	ShowAndEnableWindow( &m_staticDate1, 0 == m_nTabPageFilter );
	ShowAndEnableWindow( &m_staticDate2, 0 == m_nTabPageFilter );
	ShowAndEnableWindow( &m_staticDate3, 0 == m_nTabPageFilter );
	ShowAndEnableWindow( &m_staticDate4, 0 == m_nTabPageFilter );
	ShowAndEnableWindow( &m_staticDate5, 0 == m_nTabPageFilter );
	ShowAndEnableWindow( &m_comboDates, 0 == m_nTabPageFilter );
	ShowAndEnableWindow( &m_comboRound, 0 == m_nTabPageFilter );
	ShowAndEnableWindow( &m_comboSession, 0 == m_nTabPageFilter );
	ShowAndEnableWindow( &m_comboTimeSlice, 0 == m_nTabPageFilter );

	bool bEcrLite = SysInfo.IsEcrLiteSystem();

	bool bPluRange = ( m_checkSpecify.GetCheck() != 0 );
	ShowAndEnableWindow( &m_staticDatabase, 0 == m_nTabPageFilter );
	ShowAndEnableWindow( &m_staticDatabase1, 0 == m_nTabPageFilter );
	ShowAndEnableWindow( &m_staticDatabase2, 0 == m_nTabPageFilter );
	ShowAndEnableWindow( &m_comboLocation, 0 == m_nTabPageFilter );
	ShowAndEnableWindow( &m_comboTerminal, 0 == m_nTabPageFilter );
	ShowAndEnableWindow( &m_staticTable, ( 0 == m_nTabPageFilter ) && ( FALSE == bEcrLite ) );
	ShowAndEnableWindow( &m_comboTable, ( 0 == m_nTabPageFilter ) && ( FALSE == bEcrLite ) );
	ShowAndEnableWindow( &m_comboPluTime, 0 == m_nTabPageFilter );
	ShowAndEnableWindow( &m_checkEposSales, 0 == m_nTabPageFilter , TRUE == bEnableSalesType );
	ShowAndEnableWindow( &m_checkManualSales, 0 == m_nTabPageFilter, TRUE == bEnableSalesType );
	ShowAndEnableWindow( &m_checkImportSales, 0 == m_nTabPageFilter, TRUE == bEnableSalesType );
	ShowAndEnableWindow( &m_staticFilter, 1 == m_nTabPageFilter );
	ShowAndEnableWindow( &m_staticFilter1, 1 == m_nTabPageFilter );
	ShowAndEnableWindow( &m_staticFilter2, 1 == m_nTabPageFilter );
	ShowAndEnableWindow( &m_checkSpecify, 1 == m_nTabPageFilter );
	ShowAndEnableWindow( GetEditPluFrom(), 1 == m_nTabPageFilter, bPluRange );
	ShowAndEnableWindow( GetEditPluTo(), 1 == m_nTabPageFilter, bPluRange );
	ShowAndEnableWindow( &m_buttonBrowseTo, 1 == m_nTabPageFilter, bPluRange );
	ShowAndEnableWindow( &m_buttonBrowseFrom, 1 == m_nTabPageFilter, bPluRange );
	ShowAndEnableWindow( &m_editDeptNo, 1 == m_nTabPageFilter );
	ShowAndEnableWindow( &m_comboDeptName, 1 == m_nTabPageFilter );
	ShowAndEnableWindow( &m_comboPluFilter, 1 == m_nTabPageFilter );
	ShowAndEnableWindow( &m_buttonModifier, 1 == m_nTabPageFilter );
	ShowAndEnableWindow( &m_comboModifier, 1 == m_nTabPageFilter );
	ShowAndEnableWindow( &m_buttonTaxBand, 1 == m_nTabPageFilter );
	ShowAndEnableWindow( &m_comboTaxBand, 1 == m_nTabPageFilter );
	ShowAndEnableWindow( &m_buttonPriceLevel, 1 == m_nTabPageFilter );
	ShowAndEnableWindow( &m_comboPriceLevel, 1 == m_nTabPageFilter );
	ShowAndEnableWindow( &m_staticMisc, 1 == m_nTabPageFilter );
	ShowAndEnableWindow( &m_checkGuest, 1 == m_nTabPageFilter );
	ShowAndEnableWindow( &m_checkCustomer, 1 == m_nTabPageFilter );
	ShowAndEnableWindow( &m_checkVIPSales, 1 == m_nTabPageFilter );
	ShowAndEnableWindow( &m_staticMisc1, 1 == m_nTabPageFilter );
	ShowAndEnableWindow( &m_comboTaxField, 1 == m_nTabPageFilter );

#ifndef STOCKMAN_SYSTEM
	m_checkEposSales.SetCheck( TRUE );
	m_checkManualSales.SetCheck( FALSE );
	m_checkImportSales.SetCheck( FALSE );
	m_checkEposSales.ShowWindow ( SW_HIDE );
	m_checkManualSales.ShowWindow ( SW_HIDE );
	m_checkImportSales.ShowWindow ( SW_HIDE );
#endif

	if ( FALSE == m_bShowVIPTick )
	{
		m_checkVIPSales.SetCheck( FALSE );
		m_checkVIPSales.EnableWindow( FALSE );
		m_checkVIPSales.ShowWindow( SW_HIDE );
	}

	if ( SysInfo.IsPMSSystem() == FALSE )
	{
		m_checkGuest.SetCheck( FALSE );
		m_checkGuest.EnableWindow( FALSE );
		m_checkGuest.ShowWindow( SW_HIDE );
	}
}

/**********************************************************************/

void CPosTrayBatchExportSalesDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
  if ( nIDCtl == IDC_LIST )  
   {
	   m_listFields.AdjustRowHeight( lpMeasureItemStruct, 10 ); 
   }
}

/**********************************************************************/

void CPosTrayBatchExportSalesDlg::OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	if (( pDispInfo->item.mask & LVIF_TEXT ) != 0 )
	{
		int nIndex = pDispInfo->item.iItem;
		PrepareLine( nIndex );
		CCSV csv ( m_strDisplayLine, '\t' );
		lstrcpy ( pDispInfo->item.pszText, csv.GetString ( pDispInfo->item.iSubItem ) );
	}
	*pResult = 0;
}

/**********************************************************************/

bool CPosTrayBatchExportSalesDlg::PrepareLine( int nIndex )
{
	bool bResult = FALSE;
	m_strDisplayLine = "";

	if ( ( nIndex >= 0 ) && ( nIndex < m_arrayFieldTypes.GetSize() ) )
	{
		CPosTrayExportFieldName Name;
		Name.m_nFieldType = m_arrayFieldTypes.GetAt( nIndex );
		DataManagerNonDb.PosTrayExportFieldNames.GetNameInfo( Name );
		CreateFieldDisplayLine( nIndex, Name );
		bResult = TRUE;
	}
		
	return bResult;
}

/**********************************************************************/

void CPosTrayBatchExportSalesDlg::CreateFieldDisplayLine( int nIndex, CPosTrayExportFieldName& Name )
{
	CCSV csv( '\t' );
	csv.Add( nIndex + 1 );
	csv.Add( Name.m_strDisplayName1 );
	csv.Add( Name.m_strDisplayName2 );
	m_strDisplayLine = csv.GetLine();
}

/**********************************************************************/

void CPosTrayBatchExportSalesDlg::OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{	
	OnButtonFieldChange();
	*pResult = 0;
}

/**********************************************************************/

bool CPosTrayBatchExportSalesDlg::SelectExistingItem( int nFieldType )
{
	bool bResult = FALSE;

	for ( int nFieldIdx = 0; nFieldIdx < m_arrayFieldTypes.GetSize(); nFieldIdx++ )
	{
		if ( m_arrayFieldTypes.GetAt( nFieldIdx ) == nFieldType )
		{
			m_listFields.SetCurSel( nFieldIdx );
			m_listFields.Invalidate();
			bResult = TRUE;
			break;
		}
	}

	return bResult;
}

/**********************************************************************/

void CPosTrayBatchExportSalesDlg::OnButtonFieldAdd()
{
	if ( m_arrayFieldTypes.GetSize() >= POSTRAY_MAX_FIELDS_PLU )
	{
		Prompter.Error( "You have already added the maximum\nnumber of fields." );
	}
	else
	{
		if ( m_pFieldsDlg == NULL )
		{
			m_pFieldsDlg = new CPosTrayExportFieldDlg( POSTRAY_TASKFILE_EXPORT_SALES_TASK, &m_pFieldsDlg, this );
			m_pFieldsDlg -> Create( IDD_POSTRAY_EXPORT_FIELDS, this );
		}
	
		m_pFieldsDlg -> SetFocus();
	}
}

/**********************************************************************/

long CPosTrayBatchExportSalesDlg::OnAddField( WPARAM wIndex, LPARAM lParam )
{
	int nListSize = m_arrayFieldTypes.GetSize();
	
	if ( nListSize < POSTRAY_MAX_FIELDS_PLU )
	{
		int nInsertPos = 0;

		if ( nListSize != 0 )
		{
			nInsertPos = m_listFields.GetCurSel() + 1;
			if ( ( nInsertPos < 1 ) || ( nInsertPos > nListSize ) )
				nInsertPos = nListSize;
		}
				
		if ( SelectExistingItem( lParam ) == FALSE )
		{
			m_arrayFieldTypes.InsertAt( nInsertPos, lParam );
			m_listFields.SetItemCountEx( m_arrayFieldTypes.GetSize() );
			m_listFields.SetCurSel( nInsertPos );
			m_listFields.Invalidate();
		}
	}	
	
	return 0l;
}

/**********************************************************************/

void CPosTrayBatchExportSalesDlg::OnButtonFieldChange()
{
	int nSel = m_listFields.GetCurSel();
	if ( ( nSel >= 0 ) && ( nSel < m_arrayFieldTypes.GetSize() ) )
	{
		if ( m_pFieldsDlg != NULL )
		{
			 delete m_pFieldsDlg;
			 m_pFieldsDlg = NULL;
		}
		  
		CPosTrayExportFieldDlg dlg( POSTRAY_TASKFILE_EXPORT_SALES_TASK, m_arrayFieldTypes.GetAt( nSel ), this );

		if ( dlg.DoModal() == IDOK )
		{
			if ( SelectExistingItem( dlg.m_nFieldType ) == FALSE )
			{
				m_arrayFieldTypes.SetAt( nSel, dlg.m_nFieldType );
				m_listFields.Invalidate();
			}
		}
	}
}

/**********************************************************************/

void CPosTrayBatchExportSalesDlg::OnButtonFieldRemove()
{
	int nSel = m_listFields.GetCurSel();

	if ( ( nSel >= 0 ) && ( nSel < m_arrayFieldTypes.GetSize() ) )
	{
		m_arrayFieldTypes.RemoveAt( nSel );

		if ( nSel >= m_arrayFieldTypes.GetSize() )
			nSel--;

		m_listFields.SetItemCountEx( m_arrayFieldTypes.GetSize() );
		m_listFields.SetCurSel( nSel );
		m_listFields.Invalidate();
	}
}

/**********************************************************************/

void CPosTrayBatchExportSalesDlg::OnButtonFieldTidy()
{
	int nSel = m_listFields.GetCurSel();
	
	int nFieldType = 0;
	if ( ( nSel >= 0 ) && ( nSel < m_arrayFieldTypes.GetSize() ) )
		nFieldType = m_arrayFieldTypes.GetAt( nSel );

	m_BatchEmail.TidyExportFieldListSales( m_arrayFieldTypes );
	m_listFields.SetItemCountEx( m_arrayFieldTypes.GetSize() );
	
	if ( SelectExistingItem( nFieldType ) == FALSE )
	{
		m_listFields.SetCurSel(0);
		m_listFields.Invalidate();
	}
}

/**********************************************************************/

void CPosTrayBatchExportSalesDlg::OnButtonFieldMoveUp()
{
	int nSel = m_listFields.GetCurSel();
	
	if ( ( nSel >= 1 ) && ( nSel < m_arrayFieldTypes.GetSize() ) )
	{
		int nTemp = m_arrayFieldTypes.GetAt( nSel );
		m_arrayFieldTypes.SetAt( nSel, m_arrayFieldTypes.GetAt( nSel - 1 ) );
		m_arrayFieldTypes.SetAt( nSel - 1, nTemp );
		m_listFields.SetCurSel( nSel - 1 );
		m_listFields.Invalidate();
	}
}

/**********************************************************************/

void CPosTrayBatchExportSalesDlg::OnButtonFieldMoveDown()
{
	int nSel = m_listFields.GetCurSel();
	
	if ( ( nSel >= 0 ) && ( nSel < m_arrayFieldTypes.GetSize() - 1 ) )
	{
		int nTemp = m_arrayFieldTypes.GetAt( nSel );
		m_arrayFieldTypes.SetAt( nSel, m_arrayFieldTypes.GetAt( nSel + 1 ) );
		m_arrayFieldTypes.SetAt( nSel + 1, nTemp );
		m_listFields.SetCurSel( nSel + 1 );
		m_listFields.Invalidate();
	}
}

/**********************************************************************/

void CPosTrayBatchExportSalesDlg::LoadEmailBatchSettings()
{
	//LOCATION SELECT
	{
		CLocationSelectorLookup LocSelLookup ( m_BatchEmail );
		
		if ( NODE_LOCATION_TERMINAL == LocSelLookup.GetConType() )
		{
			LocSelLookup.SetConTypeToLocation();

			CLocationSelectorEntity LocSelEntity( LocSelLookup );
			m_LocationSelector.ForceSelection( LocSelEntity );
		
			int nTermIdx = LocSelEntity.GetTermIdx();
			if ( ( nTermIdx >= 0 ) && ( nTermIdx < m_comboTerminal.GetCount() ) )
				m_comboTerminal.SetCurSel( nTermIdx );
		}
		else	
		{
			CLocationSelectorEntity LocSelEntity( LocSelLookup );
			m_LocationSelector.ForceSelection( LocSelEntity );
		}
	}

	{
		CLocationSelectorEntity LocSelEntity;
		m_LocationSelector.GetSelectedEntity( LocSelEntity );

		m_DeptAndPluComboHelpers.UpdateCombos();
		m_EditHelpers.FillTabbedCombos( LocSelEntity.GetDbIdx() );

		switch( LocSelEntity.GetConType() )
		{
		case NODE_DATABASE:
		case NODE_SYSTEM:
			break;

		default:
			m_checkEposSales.EnableWindow ( FALSE );
			m_checkManualSales.EnableWindow ( FALSE );
			m_checkImportSales.EnableWindow ( FALSE );
			break;
		}
	}

	//TABLE
	if ( SysInfo.IsEcrLiteSystem() == FALSE )
	{
		CTableFilter TableFilter;
		TableFilter.m_nType = m_BatchEmail.GetBatchTableFilterType();
		TableFilter.m_nLevel = m_BatchEmail.GetBatchTableFilterLevel();
		TableFilter.m_nEntityNo = m_BatchEmail.GetBatchTableFilterEntityNo();
		m_LocationSelector.SelectTableComboFilter( TableFilter );
	}

	//BATCH NAME
	m_editName.SetWindowText( m_BatchEmail.GetBatchName() );

	//DATE TYPE
	int nResult = m_comboDates.FindStringExact( 0, m_BatchEmail.GetBatchDateType() );
	if ( nResult != CB_ERR )
		m_comboDates.SetCurSel( nResult );

	//DATE ROUNDING
	int nRoundTo = m_BatchEmail.GetBatchDateRoundTo();
	if ( ( nRoundTo < 0 ) || ( nRoundTo > 2 ) ) nRoundTo = 0;
	m_comboRound.SetCurSel( nRoundTo );

	//REPORT SESSION
	{
		int nSel = 0;
		CString strSession = m_BatchEmail.GetBatchSessionEntity();
		
		if ( strSession == "" )
			strSession = m_BatchEmail.GetBatchSessionLegacy();

		if ( strSession == "EOD" )
		{
			if ( EcrmanOptions.GetReportsEODSessionFlag() == TRUE )
				nSel = DataManagerNonDb.ReportSession.GetSize();
		}
		else
		{
			for ( int n = 0; n < DataManagerNonDb.ReportSession.GetSize(); n++ )
			{
				CSessionCSVRecord Session;
				DataManagerNonDb.ReportSession.GetAt( n, Session );

				if ( strSession == Session.GetEntityID() )
				{
					nSel = n;
					break;
				}
			}
		}

		if ( nSel >= m_comboSession.GetCount() )
			nSel = 0;

		m_comboSession.SetCurSel( nSel );
	}
	
	//TIME SLICE
	{
		int nSel = 0;
		CString strTimeSlice = m_BatchEmail.GetBatchTimeSlice();

		for ( int n = 0; n < DataManagerNonDb.TimeSliceList.GetSize(); n++ )
		{
			CTimeSliceMapCSVRecord TimeSlice;
			DataManagerNonDb.TimeSliceList.GetAt( n, TimeSlice );

			if ( strTimeSlice == TimeSlice.GetEntityID() )
			{
				nSel = n;
				break;
			}
		}

		if ( nSel >= m_comboTimeSlice.GetCount() )
			nSel = 0;

		m_comboTimeSlice.SetCurSel( nSel );
	}

	nResult = m_comboTimeSlice.FindStringExact( 0, m_BatchEmail.GetBatchTimeSlice() );
	if ( nResult != CB_ERR )
		m_comboTimeSlice.SetCurSel( nResult );

	//PLU RANGE
	bool bPluRange = m_BatchEmail.GetBatchPluRangeFlag();
	m_checkSpecify.SetCheck( bPluRange );
	GetEditPluFrom() -> EnableWindow( bPluRange );
	GetEditPluTo() -> EnableWindow( bPluRange );
	m_buttonBrowseFrom.EnableWindow( bPluRange );
	m_buttonBrowseTo.EnableWindow( bPluRange );
	
	CString strPluFrom = "";
	
	if ( m_BatchEmail.GetBatchPluNoFrom() != 0 )
		strPluFrom.Format( "%*.*I64d", 
		Super.MaxPluLen(),
		Super.MaxPluLen(),
		m_BatchEmail.GetBatchPluNoFrom() );
	
	if ( strPluFrom.GetLength() <= Super.MaxPluLen() )
		if ( ::TestNumeric( strPluFrom ) == TRUE )
			GetEditPluFrom() -> SetWindowText( strPluFrom );

	CString strPluTo;
	
	if ( m_BatchEmail.GetBatchPluNoTo() != 0 )
		strPluTo.Format( "%*.*I64d", 
		Super.MaxPluLen(),
		Super.MaxPluLen(),
		m_BatchEmail.GetBatchPluNoTo() );
	
	if ( strPluTo.GetLength() <= Super.MaxPluLen() )
		if ( ::TestNumeric( strPluTo ) == TRUE )
			GetEditPluTo() -> SetWindowText( strPluTo );

	//DEPARTMENT FILTER
	m_DepartmentSelector.UpdateDepartmentCombo(m_BatchEmail.GetBatchDeptFilter() );
	m_DepartmentSelector.UpdateDepartmentEditBox();

	//PLU FILTER
	m_DeptAndPluComboHelpers.SetPluFilterNo( m_BatchEmail.GetBatchPluFilter() );

	//PLU TIME
	m_comboPluTime.SetCurSel( ( m_BatchEmail.GetBatchSaleTimeFlag() == 1 ) ? 1 : 0 );

	//MODIFIER, PRICE LEVEL, TAX BAND
	m_EditHelpers.LoadTaskSettings( m_BatchEmail );

	m_checkEposSales.SetCheck( m_BatchEmail.GetBatchEposSalesFlag() );
	m_checkManualSales.SetCheck( m_BatchEmail.GetBatchManualSalesFlag() );
	m_checkImportSales.SetCheck( m_BatchEmail.GetBatchImportSalesFlag() );
	m_checkGuest.SetCheck( m_BatchEmail.GetBatchPMSTerminalFlag() );
	m_checkCustomer.SetCheck( m_BatchEmail.GetBatchCustomerOnlyFlag() );
	m_checkVIPSales.SetCheck( m_BatchEmail.GetBatchVIPOnlyFlag() );

	//TAX FIELD WIDTH
	{
		int nSel = m_BatchEmail.GetBatchTaxDecimalPlaces() + 1;
		if ( ( nSel >= 0 ) && ( nSel < m_comboTaxField.GetCount() ) )
			m_comboTaxField.SetCurSel( nSel );
		else
			m_comboTaxField.SetCurSel( 0 );
	}

	//REPORT SELECTION
	//REPORT TYPES
	CCSV csvReports( m_BatchEmail.GetBatchReportList() );
	CReportConsolidationArray<CSortedStringItem> arrayTemp;
	for ( int n = 0; n < csvReports.GetSize(); n++ )
	{
		CSortedStringItem item;
		item.m_strItem = csvReports.GetString(n);
		arrayTemp.Consolidate( item );
	}
}

/**********************************************************************/

void CPosTrayBatchExportSalesDlg::SaveEmailBatchSettings()
{
	//LOCATION SELECT
	{
		CLocationSelectorEntity LocSelEntity;
		m_LocationSelector.GetSelectedEntity( LocSelEntity );
		m_BatchEmail.SetBatchConType( LocSelEntity.GetConType() );
		m_BatchEmail.SetBatchConDbNo( LocSelEntity.GetDbNo() );
		m_BatchEmail.SetBatchConNwkNo( LocSelEntity.GetNwkNo() );
		m_BatchEmail.SetBatchConEntityNo( LocSelEntity.GetEntityNo() );
		m_BatchEmail.SetBatchConTermIdx( LocSelEntity.GetTermIdx() );
	}

	//TABLE FILTER
	if ( SysInfo.IsEcrLiteSystem() == FALSE )
	{
		CTableFilter TableFilter;
		m_TableReportHelpers.GetTableFilter( m_comboTable.GetCurSel(), TableFilter );
		m_BatchEmail.SetBatchTableFilterType( TableFilter.m_nType );
		m_BatchEmail.SetBatchTableFilterLevel( TableFilter.m_nLevel );
		m_BatchEmail.SetBatchTableFilterEntityNo( TableFilter.m_nEntityNo );
	}

	//BATCH NAME
	CString strName;
	m_editName.GetWindowText( strName );
	m_BatchEmail.SetBatchName( strName );

	//DATE TYPE
	CString strDateType;
	m_comboDates.GetWindowText( strDateType );
	m_BatchEmail.SetBatchDateType( strDateType );

	//DATE ROUND
	m_BatchEmail.SetBatchDateRoundTo( m_comboRound.GetCurSel() );

	//REPORT SESSION
	{
		int nSel = m_comboSession.GetCurSel();
		
		if ( ( nSel >= 0 ) && ( nSel < DataManagerNonDb.ReportSession.GetSize() ) )
		{
			CSessionCSVRecord Session;
			DataManagerNonDb.ReportSession.GetAt( nSel, Session );
			m_BatchEmail.SetBatchSessionEntity( Session.GetEntityID() );
		}
		else if ( ( nSel == DataManagerNonDb.ReportSession.GetSize() ) && ( EcrmanOptions.GetReportsEODSessionFlag() == TRUE ) )
			m_BatchEmail.SetBatchSessionEntity( "EOD" );
		else
			m_BatchEmail.SetBatchSessionEntity( "" );

		m_BatchEmail.SetBatchSessionLegacy( "" );
	}

	//TIME SLICE
	{
		int nSel = m_comboTimeSlice.GetCurSel();
		
		if ( ( nSel >= 0 ) && ( nSel < DataManagerNonDb.TimeSliceList.GetSize() ) )
		{
			CTimeSliceMapCSVRecord TimeSlice;
			DataManagerNonDb.TimeSliceList.GetAt( nSel, TimeSlice );
			m_BatchEmail.SetBatchTimeSlice( TimeSlice.GetEntityID() );
		}
		else
			m_BatchEmail.SetBatchTimeSlice( "" );
	}

	//PLU NO
	bool bPluRange = ( m_checkSpecify.GetCheck() != 0 );
	m_BatchEmail.SetBatchPluRangeFlag( bPluRange );

	if ( TRUE == bPluRange )
	{
		CString str;
		GetEditPluFrom() -> GetWindowText( str );
		m_BatchEmail.SetBatchPluNoFrom( _atoi64(str) );
		GetEditPluTo() -> GetWindowText( str );
		m_BatchEmail.SetBatchPluNoTo( _atoi64(str) );
	}
	else
	{
		m_BatchEmail.SetBatchPluNoFrom( 0 );
		m_BatchEmail.SetBatchPluNoTo( 0 );
	}

	//DEPARTMENT FILTER
	m_BatchEmail.SetBatchDeptFilter( m_DepartmentSelector.GetDepartmentFilter() );

	//PLU FILTER
	m_BatchEmail.SetBatchPluFilter( m_DeptAndPluComboHelpers.GetPluFilterNo() );

	//PLU TIME
	m_BatchEmail.SetBatchSaleTimeFlag( m_comboPluTime.GetCurSel() == 1 );

	//MODIFIER,PRICE LEVEL,TAX NAND
	m_EditHelpers.SaveTaskSettings( m_BatchEmail );

	//TAX FIELD
	m_BatchEmail.SetBatchTaxDecimalPlaces( m_comboTaxField.GetCurSel() - 1 );

	//MISC FLAGS
	m_BatchEmail.SetBatchEposSalesFlag( m_checkEposSales.GetCheck() != 0 );
	m_BatchEmail.SetBatchManualSalesFlag( m_checkManualSales.GetCheck() != 0 );
	m_BatchEmail.SetBatchImportSalesFlag( m_checkImportSales.GetCheck() != 0 );
	m_BatchEmail.SetBatchPMSTerminalFlag( m_checkGuest.GetCheck() != 0 );
	m_BatchEmail.SetBatchCustomerOnlyFlag( m_checkCustomer.GetCheck() != 0 );
	m_BatchEmail.SetBatchVIPOnlyFlag( m_checkVIPSales.GetCheck() != 0 );
}

/**********************************************************************/

void CPosTrayBatchExportSalesDlg::FillTimeSliceCombo()
{
	m_comboTimeSlice.ResetContent();

	for ( int nPos = 0; nPos < DataManagerNonDb.TimeSliceList.GetSize(); nPos++ )
	{
		CTimeSliceMapCSVRecord ReportRecord;
		DataManagerNonDb.TimeSliceList.GetAt( nPos, ReportRecord );

		switch ( ReportRecord.GetReportType() )
		{
		case SH_CUSTOM:
			m_comboTimeSlice.AddString ( ReportRecord.GetListName() );
			break;

		case SH_TIMESLICE_SECOND:
		case SH_TIMESLICE_MINUTE:
		case SH_TIMESLICE_5MIN:
		case SH_TIMESLICE_10MIN:
		case SH_TIMESLICE_15MIN:
		case SH_TIMESLICE_30MIN:
		case SH_TIMESLICE_60MIN:
			m_comboTimeSlice.AddString ( ReportRecord.GetListName() );
			break;
		}
	}

	m_comboTimeSlice.SetCurSel( 0 );
	if ( m_comboTimeSlice.GetCount() <= 1 )
		m_comboTimeSlice.EnableWindow( FALSE );
}

/**********************************************************************/

void CPosTrayBatchExportSalesDlg::OnSelectDepartment() 
{
	m_DepartmentSelector.SelectDepartmentFromCombo();
}

/**********************************************************************/

void CPosTrayBatchExportSalesDlg::OnKillFocusDeptNo() 
{
	m_DepartmentSelector.SelectDepartmentFromEditBox();
}

/**********************************************************************/

void CPosTrayBatchExportSalesDlg::OnSelectLocation() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		m_LocationSelector.FillEposCombo();
		m_LocationSelector.FillTableCombo();	

		CLocationSelectorEntity LocSelEntity;
		m_LocationSelector.GetSelectedEntity( LocSelEntity );

		m_DeptAndPluComboHelpers.UpdateCombos();
		m_EditHelpers.FillTabbedCombos( LocSelEntity.GetDbIdx() ); 
		
		switch( LocSelEntity.GetConType() )
		{
		case NODE_DATABASE:
		case NODE_SYSTEM:
			m_checkEposSales.EnableWindow ( TRUE );
			m_checkManualSales.EnableWindow ( TRUE );
			m_checkImportSales.EnableWindow ( TRUE );
			break;

		default:
			m_checkEposSales.SetCheck ( TRUE );
			m_checkManualSales.SetCheck ( FALSE );
			m_checkImportSales.SetCheck ( FALSE );
			m_checkEposSales.EnableWindow ( FALSE );
			m_checkManualSales.EnableWindow ( FALSE );
			m_checkImportSales.EnableWindow ( FALSE );
			break;
		}

		CDataManagerInfo info;
		DataManager.OpenDatabaseReadOnly( LocSelEntity.GetDbIdx(), info, FALSE );
	}
}

/**********************************************************************/

void CPosTrayBatchExportSalesDlg::OnTogglePluRange() 
{
	bool bEnable = ( m_checkSpecify.GetCheck() != 0 );
	GetEditPluFrom() -> EnableWindow ( bEnable );
	GetEditPluTo() -> EnableWindow ( bEnable );
	m_buttonBrowseFrom.EnableWindow ( bEnable );
	m_buttonBrowseTo.EnableWindow ( bEnable );	
}

/**********************************************************************/

void CPosTrayBatchExportSalesDlg::OnKillFocusPluFrom() 
{
	UpdateData( TRUE, FALSE );
	m_strPluNoFrom = BarcodePluNoTable.ConvertPlu( m_strPluNoFrom );
	UpdateData ( FALSE );
}

/**********************************************************************/

void CPosTrayBatchExportSalesDlg::OnKillFocusPluTo() 
{
	UpdateData( TRUE, FALSE );
	m_strPluNoTo = BarcodePluNoTable.ConvertPlu( m_strPluNoTo );
	UpdateData ( FALSE );
}

/**********************************************************************/

void CPosTrayBatchExportSalesDlg::OnButtonBrowseFrom() 
{
	UpdateData( TRUE, FALSE );

	int nDlgResult, nPluIdx;
	::ShowPluFilterDlg( m_FilterArray, this, nDlgResult, nPluIdx );

	if ( IDOK == nDlgResult )
	{
		if ( nPluIdx >= 0 && nPluIdx < DataManager.Plu.GetSize() )
		{
			m_strPluNoFrom = DataManager.Plu.GetPluNoString ( nPluIdx );
			UpdateData ( FALSE );
		}
	}	
}

/**********************************************************************/

void CPosTrayBatchExportSalesDlg::OnButtonBrowseTo() 
{
	UpdateData( TRUE, FALSE );

	int nDlgResult, nPluIdx;
	::ShowPluFilterDlg( m_FilterArray, this, nDlgResult, nPluIdx );

	if ( IDOK == nDlgResult )
	{
		if ( nPluIdx >= 0 && nPluIdx < DataManager.Plu.GetSize() )
		{
			m_strPluNoTo = DataManager.Plu.GetPluNoString ( nPluIdx );
			UpdateData ( FALSE );
		}
	}
}

/**********************************************************************/

void CPosTrayBatchExportSalesDlg::OnButtonTaxBand() 
{
	m_EditHelpers.HandleButtonTaxBand( this );
}

/**********************************************************************/

void CPosTrayBatchExportSalesDlg::OnButtonModifier() 
{
	m_EditHelpers.HandleButtonModifier( this );
}

/**********************************************************************/

void CPosTrayBatchExportSalesDlg::OnButtonPriceLevel() 
{
	m_EditHelpers.HandleButtonPriceLevel( this );
}

/**********************************************************************/

void CPosTrayBatchExportSalesDlg::OnOK()
{
	SaveEmailBatchSettings();
	m_BatchEmail.SetExportFieldListSales( m_arrayFieldTypes );
	EndDialog( IDOK );
}

/**********************************************************************/
