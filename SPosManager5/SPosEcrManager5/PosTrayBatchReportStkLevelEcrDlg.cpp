/**********************************************************************/
#include <process.h>
/**********************************************************************/
#include "BarcodePluNoTable.h"
#include "DateSelectorRange.h"
#include "FilterTypes.h"
#include "MaxLengths.h"
#include "Password.h"
#include "PosTrayOptions.h"
#include "..\CommonEcrTray\ReportPluStockLevel.h"
/**********************************************************************/
#include "PosTrayBatchReportStkLevelEcrDlg.h"
/**********************************************************************/

CPosTrayBatchReportStkLevelEcrDlg::CPosTrayBatchReportStkLevelEcrDlg( CPosTrayTask& BatchEmail, CWnd* pParent /*=NULL*/)
	: CSSDialog(CPosTrayBatchReportStkLevelEcrDlg::IDD, pParent), 
		m_FilterArray ( FILTERTYPE_BROWSE_PLU ),
		m_DepartmentSelector ( m_editDeptNo, m_comboDeptName ),
		m_EcrmanSupplierSelector( m_editSupplier, m_comboSupplier, TRUE ),
		m_BatchEmail( BatchEmail )
{
	//{{AFX_DATA_INIT(CPosTrayBatchReportStkLevelEcrDlg)
	//}}AFX_DATA_INIT
	CDataManagerInfo info;
	DataManagerNonDb.OpenEcrmanSupplier( DB_READONLY, info );
}

/**********************************************************************/

CPosTrayBatchReportStkLevelEcrDlg::~CPosTrayBatchReportStkLevelEcrDlg()
{
	CDataManagerInfo info;
	DataManagerNonDb.CloseEcrmanSupplier( info );
}

/**********************************************************************/

void CPosTrayBatchReportStkLevelEcrDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPosTrayBatchReportStkLevelEcrDlg)
	
	DDX_Control(pDX, IDC_TAB_FILTER, m_TabFilter );

	DDX_Control(pDX, IDC_STATIC_NAME, m_staticName);
	DDX_Control(pDX, IDC_EDIT_NAME, m_editName);

	DDX_Control(pDX, IDC_STATIC_DATE, m_staticDate);
	DDX_Control(pDX, IDC_STATIC_DATE1, m_staticDate1);
	DDX_Control(pDX, IDC_COMBO_DATES, m_comboDates);
	
	DDX_Control(pDX, IDC_STATIC_DATABASE, m_staticDatabase);
	DDX_Control(pDX, IDC_STATIC_DATABASE1, m_staticDatabase1);
	DDX_Control(pDX, IDC_COMBO_LOCATION, m_comboLocation);
	
	DDX_Control(pDX, IDC_STATIC_FILTER, m_staticFilter);
	DDX_Control(pDX, IDC_STATIC_FILTER1, m_staticFilter1);
	DDX_Control(pDX, IDC_CHECK_SPECIFY, m_checkSpecify);
	DDX_Control(pDX, IDC_BUTTON_BROWSE_FROM, m_buttonBrowseFrom);
	DDX_Control(pDX, IDC_BUTTON_BROWSE_TO, m_buttonBrowseTo);
	DDX_IntegerString(pDX, IDC_EDIT_PLU_FROM, m_strPluNoFrom);
	DDX_IntegerString(pDX, IDC_EDIT_PLU_TO, m_strPluNoTo);
	DDX_Control(pDX, IDC_EDIT_DEPTNO, m_editDeptNo);
	DDX_Control(pDX, IDC_COMBO_DEPTNAME, m_comboDeptName);
	DDX_Control(pDX, IDC_STATIC_FILTER2, m_staticFilter2);
	DDX_Control(pDX, IDC_EDIT_SUPPLIER, m_editSupplier);
	DDX_Control(pDX, IDC_COMBO_SUPPLIER, m_comboSupplier);
	DDX_Control(pDX, IDC_CHECK_ZEROSKIP, m_checkZeroSkip);
	
	DDX_Control(pDX, IDC_STATIC_SORT, m_staticSort);
	DDX_Control(pDX, IDC_CHECK_DEPTSORT, m_checkDeptSort);
	DDX_Control(pDX, IDC_CHECK_SUPPLIER, m_checkSupplierSort);
	
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPosTrayBatchReportStkLevelEcrDlg, CDialog)
	//{{AFX_MSG_MAP(CPosTrayBatchReportStkLevelEcrDlg)

	ON_WM_MEASUREITEM()

	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_FILTER, OnSelectTabFilter)

	ON_CBN_SELCHANGE(IDC_COMBO_LOCATION, OnSelectLocation)

	ON_BN_CLICKED(IDC_CHECK_SPECIFY, OnTogglePluRange)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE_FROM, OnButtonBrowseFrom)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE_TO, OnButtonBrowseTo)
	ON_EN_KILLFOCUS(IDC_EDIT_PLU_FROM, OnKillFocusPluFrom)
	ON_EN_KILLFOCUS(IDC_EDIT_PLU_TO, OnKillFocusPluTo)
	ON_CBN_SELCHANGE(IDC_COMBO_DEPTNAME, OnSelectDepartment)
	ON_EN_KILLFOCUS(IDC_EDIT_DEPTNO, OnKillFocusDeptNo)
	ON_EN_KILLFOCUS(IDC_EDIT_SUPPLIER, OnKillFocusSupplier)
	ON_CBN_SELCHANGE(IDC_COMBO_SUPPLIER, OnSelectSupplier)
	
	//}}AFX_MSG_MAP
	
END_MESSAGE_MAP()

/**********************************************************************/
CEdit* CPosTrayBatchReportStkLevelEcrDlg::GetEditPluFrom() { return GetEdit( IDC_EDIT_PLU_FROM ); }
CEdit* CPosTrayBatchReportStkLevelEcrDlg::GetEditPluTo() { return GetEdit( IDC_EDIT_PLU_TO ); }
/**********************************************************************/

BOOL CPosTrayBatchReportStkLevelEcrDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_DeptAndPluComboHelpers.SetControlPointers( NULL, &m_LocationSelector, &m_DepartmentSelector );

	m_listReport.SubclassDlgItem ( IDC_LIST, this );
	m_listReport.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_listReport.InsertColumn ( 0, "", LVCFMT_LEFT, 160 );
	m_listReport.SetLockItemZeroFlag( FALSE );
	
	SubclassEdit ( IDC_EDIT_PLU_FROM, SS_NUM, Super.MaxPluLen(), "%s" );
	SubclassEdit ( IDC_EDIT_PLU_TO, SS_NUM, Super.MaxPluLen(), "%s" );

	FillReportList();
	
	CWordArray arrayTypes;
	arrayTypes.Add( NODE_DATABASE );

	if ( PasswordArray.GetLocationAccessType() != LOCATION_ACCESS_SYSTEM )
		m_LocationSelector.SetSingleDbIdx( PasswordArray.GetLocationAccessDbIdx() );
	
	m_LocationSelector.BuildList( arrayTypes );
	m_LocationSelector.FillLocationCombo ( &m_comboLocation );
	
	m_editDeptNo.LimitText( MAX_LENGTH_DEPTNO );

	m_editSupplier.LimitText(3);
	m_EcrmanSupplierSelector.FillEcrmanSupplierCombo( ECRMANSUPPLIER_ALL );

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

	LoadEmailBatchSettings();

	m_TabFilter.InsertItem( 0, "General" );
	m_TabFilter.InsertItem( 1, "Filter/Sort" );
	m_nTabPageFilter = 0;
	SelectTabFilter();
	
	MoveControl( &m_staticFilter, 7, 28 );
	MoveControl( &m_checkSpecify, 21, 47 );
	MoveControl( GetEditPluFrom(), 77, 47 );
	MoveControl( &m_buttonBrowseFrom, 173, 47 );
	MoveControl( GetEditPluTo(), 77, 66 );
	MoveControl( &m_buttonBrowseTo, 173, 66 );
	MoveControl( &m_staticFilter1, 21, 93 );
	MoveControl( &m_editDeptNo, 77, 93 );
	MoveControl( &m_comboDeptName, 105, 93 );
	MoveControl( &m_staticFilter2, 21, 113 );
	MoveControl( &m_editSupplier, 77, 113 );
	MoveControl( &m_comboSupplier, 105, 113 );
	MoveControl( &m_checkZeroSkip, 21, 138 );
	/****/
	MoveControl( &m_staticSort, 7, 164 );
	MoveControl( &m_checkDeptSort, 21, 184 );
	MoveControl( &m_checkSupplierSort, 21, 200 );
	/****/
	ResizeDialog( 443, 231 );

	return TRUE;  
}

/**********************************************************************/

void CPosTrayBatchReportStkLevelEcrDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
   if ( nIDCtl == IDC_LIST )  
   {
	   m_listReport.AdjustRowHeight( lpMeasureItemStruct, 10 ); 
   }
}

/**********************************************************************/

void CPosTrayBatchReportStkLevelEcrDlg::OnSelectTabFilter(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;

	m_nTabPageFilter = m_TabFilter.GetCurSel();
	
	if ( ( m_nTabPageFilter < 0 ) || ( m_nTabPageFilter > 1 ) )
		m_nTabPageFilter = 0;

	SelectTabFilter();
}

/**********************************************************************/

void CPosTrayBatchReportStkLevelEcrDlg::SelectTabFilter()
{
	bool bPluRange = ( m_checkSpecify.GetCheck() != 0 );

	ShowAndEnableWindow( &m_staticName, 0 == m_nTabPageFilter );
	ShowAndEnableWindow( &m_editName, 0 == m_nTabPageFilter );
	ShowAndEnableWindow( &m_staticDate, 0 == m_nTabPageFilter );
	ShowAndEnableWindow( &m_staticDate1, 0 == m_nTabPageFilter );
	ShowAndEnableWindow( &m_comboDates, 0 == m_nTabPageFilter );
	ShowAndEnableWindow( &m_staticDatabase, 0 == m_nTabPageFilter );
	ShowAndEnableWindow( &m_staticDatabase1, 0 == m_nTabPageFilter );
	ShowAndEnableWindow( &m_comboLocation, 0 == m_nTabPageFilter );
	/****/
	ShowAndEnableWindow( &m_staticFilter, 1 == m_nTabPageFilter );
	ShowAndEnableWindow( &m_staticFilter1, 1 == m_nTabPageFilter );
	ShowAndEnableWindow( &m_checkSpecify, 1 == m_nTabPageFilter );
	ShowAndEnableWindow( GetEditPluFrom(), 1 == m_nTabPageFilter, bPluRange );
	ShowAndEnableWindow( &m_buttonBrowseFrom, 1 == m_nTabPageFilter, bPluRange );
	ShowAndEnableWindow( GetEditPluTo(), 1 == m_nTabPageFilter, bPluRange );
	ShowAndEnableWindow( &m_buttonBrowseTo, 1 == m_nTabPageFilter, bPluRange );
	ShowAndEnableWindow( &m_staticFilter1, 1 == m_nTabPageFilter );
	ShowAndEnableWindow( &m_editDeptNo, 1 == m_nTabPageFilter );
	ShowAndEnableWindow( &m_comboDeptName, 1 == m_nTabPageFilter );
	ShowAndEnableWindow( &m_staticFilter2, 1 == m_nTabPageFilter );
	ShowAndEnableWindow( &m_editSupplier, 1 == m_nTabPageFilter );
	ShowAndEnableWindow( &m_comboSupplier, 1 == m_nTabPageFilter );
	ShowAndEnableWindow( &m_checkZeroSkip, 1 == m_nTabPageFilter );
	/****/
	ShowAndEnableWindow( &m_staticSort, 1 == m_nTabPageFilter );
	ShowAndEnableWindow( &m_checkDeptSort, 1 == m_nTabPageFilter );
	ShowAndEnableWindow( &m_checkSupplierSort, 1 == m_nTabPageFilter );
	/****/
}

/**********************************************************************/

void CPosTrayBatchReportStkLevelEcrDlg::LoadEmailBatchSettings()
{
	//LOCATION SELECT
	{
		CLocationSelectorLookup LocSelLookup ( m_BatchEmail );
		CLocationSelectorEntity LocSelEntity( LocSelLookup );
		m_LocationSelector.ForceSelection( LocSelEntity );
	}

	{
		CLocationSelectorEntity LocSelEntity;
		m_LocationSelector.GetSelectedEntity( LocSelEntity );
		m_DeptAndPluComboHelpers.UpdateCombos();
	}

	//BATCH NAME
	m_editName.SetWindowText( m_BatchEmail.GetBatchName() );

	//DATE TYPE
	int nResult = m_comboDates.FindStringExact( 0, m_BatchEmail.GetBatchDateType() );
	if ( nResult != CB_ERR )
		m_comboDates.SetCurSel( nResult );
	
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

	//SUPPLIER FILTER
	{
		int nSuppFilter = m_BatchEmail.GetBatchSupplierFilter();

		if ( nSuppFilter < 0 )
			nSuppFilter = ECRMANSUPPLIER_ALL;

		m_EcrmanSupplierSelector.UpdateEcrmanSupplierCombo( nSuppFilter );
		m_EcrmanSupplierSelector.UpdateEcrmanSupplierEditBox();
	}

	//MISC FLAGS
	m_checkDeptSort.SetCheck( m_BatchEmail.GetBatchDeptSortFlag() );
	m_checkSupplierSort.SetCheck( m_BatchEmail.GetBatchSupplierSortFlag() );
	m_checkZeroSkip.SetCheck( m_BatchEmail.GetBatchStockZeroSkipFlag() );

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

	for ( int n = 0; n < m_listReport.GetItemCount(); n++ )
	{
		CSortedStringItem item;
		item.m_strItem = m_arrayReportEntityIDs.GetAt(n);
		
		int nPos;
		m_listReport.SetItemTagState( n, arrayTemp.Find( item, nPos ) ? 1 : 0 );
	}
}

/**********************************************************************/

void CPosTrayBatchReportStkLevelEcrDlg::SaveEmailBatchSettings()
{
	//LOCATION SELECT
	{
		CLocationSelectorEntity LocSelEntity;
		m_LocationSelector.GetSelectedEntity( LocSelEntity );
		m_BatchEmail.SetBatchConType( LocSelEntity.GetConType() );
		m_BatchEmail.SetBatchConDbNo( LocSelEntity.GetDbNo() );
		m_BatchEmail.SetBatchConEntityNo( LocSelEntity.GetEntityNo() );
	}

	//BATCH NAME
	CString strName;
	m_editName.GetWindowText( strName );
	m_BatchEmail.SetBatchName( strName );

	//DATE TYPE
	CString strDateType;
	m_comboDates.GetWindowText( strDateType );
	m_BatchEmail.SetBatchDateType( strDateType );

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
	
	//SUPPLIER FILTER
	{
		int nSuppFilter = m_EcrmanSupplierSelector.GetEcrmanSupplierNo();

		if ( ECRMANSUPPLIER_ALL == nSuppFilter )
			nSuppFilter = -1;

		m_BatchEmail.SetBatchSupplierFilter( nSuppFilter );
	}

	//MISC FLAGS
	m_BatchEmail.SetBatchDeptSortFlag( m_checkDeptSort.GetCheck() != 0 );
	m_BatchEmail.SetBatchSupplierSortFlag( m_checkSupplierSort.GetCheck() != 0 );
	m_BatchEmail.SetBatchStockZeroSkipFlag( m_checkZeroSkip.GetCheck() != 0 );

	//REPORT SELECTION
	CCSV csvReports;
	for ( int n = 0; n < m_listReport.GetItemCount(); n++ )
	{
		if ( m_listReport.GetItemTagState(n) == 1 )
		{
			csvReports.Add( m_arrayReportEntityIDs.GetAt(n) );
		}
	}

	m_BatchEmail.SetBatchReportList( csvReports.GetLine() );
}

/**********************************************************************/

void CPosTrayBatchReportStkLevelEcrDlg::FillReportList()
{	
	m_listReport.ClearList();
	m_arrayReportEntityIDs.RemoveAll();
	
	AddReportToList( "Apparent Stock", REPORT_PLUSTOCK_APPARENT );
	AddReportToList( "Apparent Valuation", REPORT_PLUSTOCK_VALUATION );
	AddReportToList( "Reorder Report", REPORT_PLUSTOCK_REORDER );
	
	if ( EcrmanOptions.AllowPluDeliveryAudit() == TRUE )
	{
		AddReportToList( "Delivery Audit", REPORT_PLUSTOCK_DEL_AUDIT );
		AddReportToList( "Delivery Totals", REPORT_PLUSTOCK_DEL_TOTAL );
		AddReportToList( "Delivery Valuation", REPORT_PLUSTOCK_DEL_VALUE );
	}

	m_listReport.SetCurSel( 0 );
}

/**********************************************************************/

void CPosTrayBatchReportStkLevelEcrDlg::AddReportToList( const char* szName, int nEntityID )
{
	CString str;
	str.Format( "%4.4d", nEntityID );
	m_arrayReportEntityIDs.Add( str );

	CSSListTaggedSelectItem item;
	item.m_nData = 0;
	item.SetTagState(0);
	item.m_strText = szName;
	
	m_listReport.AddItemToList( item );
}

/**********************************************************************/

void CPosTrayBatchReportStkLevelEcrDlg::OnSelectDepartment() 
{
	m_DepartmentSelector.SelectDepartmentFromCombo();
}

/**********************************************************************/

void CPosTrayBatchReportStkLevelEcrDlg::OnKillFocusDeptNo() 
{
	m_DepartmentSelector.SelectDepartmentFromEditBox();
}

/**********************************************************************/

void CPosTrayBatchReportStkLevelEcrDlg::OnSelectLocation() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		m_LocationSelector.FillEposCombo();	

		CLocationSelectorEntity LocSelEntity;
		m_LocationSelector.GetSelectedEntity( LocSelEntity );

		m_DeptAndPluComboHelpers.UpdateCombos();
		
		CDataManagerInfo info;
		DataManager.OpenDatabaseReadOnly( LocSelEntity.GetDbIdx(), info, FALSE );
	}
}

/**********************************************************************/

void CPosTrayBatchReportStkLevelEcrDlg::OnTogglePluRange() 
{
	bool bEnable = ( m_checkSpecify.GetCheck() != 0 );
	GetEditPluFrom() -> EnableWindow ( bEnable );
	GetEditPluTo() -> EnableWindow ( bEnable );
	m_buttonBrowseFrom.EnableWindow ( bEnable );
	m_buttonBrowseTo.EnableWindow ( bEnable );	
}

/**********************************************************************/

void CPosTrayBatchReportStkLevelEcrDlg::OnKillFocusPluFrom() 
{
	UpdateData( TRUE, FALSE );
	m_strPluNoFrom = BarcodePluNoTable.ConvertPlu( m_strPluNoFrom );
	UpdateData ( FALSE );
}

/**********************************************************************/

void CPosTrayBatchReportStkLevelEcrDlg::OnKillFocusPluTo() 
{
	UpdateData( TRUE, FALSE );
	m_strPluNoTo = BarcodePluNoTable.ConvertPlu( m_strPluNoTo );
	UpdateData ( FALSE );
}

/**********************************************************************/

void CPosTrayBatchReportStkLevelEcrDlg::OnButtonBrowseFrom() 
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

void CPosTrayBatchReportStkLevelEcrDlg::OnButtonBrowseTo() 
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

void CPosTrayBatchReportStkLevelEcrDlg::OnKillFocusSupplier() 
{
	m_EcrmanSupplierSelector.SelectEcrmanSupplierFromEditBox();
}

/**********************************************************************/

void CPosTrayBatchReportStkLevelEcrDlg::OnSelectSupplier() 
{
	m_EcrmanSupplierSelector.SelectEcrmanSupplierFromCombo();
}

/**********************************************************************/

void CPosTrayBatchReportStkLevelEcrDlg::OnOK()
{
	SaveEmailBatchSettings();
	EndDialog( IDOK );
}

/**********************************************************************/
