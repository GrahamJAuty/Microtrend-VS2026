/**********************************************************************/
#include "DateSelectorRange.h"
#include "MaxLengths.h"
#include "Password.h"
#include "PMSOptions.h"
#include "PosTrayEposConsolDlg.h"
#include "PosTrayOptions.h"
#include "ReportEposConsolDlg.h"
#include "ReportTypes.h"
#include "SysInfo.h"
/**********************************************************************/
#include "PosTrayBatchReportEposDlg.h"
/**********************************************************************/

CPosTrayBatchReportEposDlg::CPosTrayBatchReportEposDlg( CPosTrayTask& BatchEmail, bool bModifiedBatch, CWnd* pParent)
	: CSSDialog(CPosTrayBatchReportEposDlg::IDD, pParent), 
	m_BatchEmail( BatchEmail ),
	m_DepartmentSelector ( m_editDeptNo, m_comboDeptName )
{
	//{{AFX_DATA_INIT(CPosTrayBatchReportEposDlg)
	//}}AFX_DATA_INIT
	m_nTabPageFilter = 0;
	m_bModifiedBatch = bModifiedBatch;
	DataManagerNonDb.ServerNameTable.BuildList();
	DataManagerNonDb.CustomerNameTable.BuildList();

	m_nPMSConsolidationMode = 0;

	m_EposReportSelect.LoadReportSelection();

	bool bModifier = ( SysInfo.GetMaxBasePluLen() != 0 );
	bool bReason = ( EcrmanOptions.GetReportsEODReasonsFlag() == TRUE );
	bool bVIP = ( EcrmanOptions.GetReportsVIPType() != 0 );
	bool bEOD = ( EcrmanOptions.GetReportsEODSessionFlag() );
	
	for ( int n = 0; n < m_EposReportSelect.GetSelectedReportCount(); n++ )
	{
		int nReportType = m_EposReportSelect.GetSelectedReportType(n);

		bool bAcceptType = TRUE;

		if ( SysInfo.IsEcrLiteSystem() == TRUE )
		{
			switch( nReportType )
			{
			case REPORT_TYPE_PLUSALES:
			case REPORT_TYPE_PLUSALES_DEPT_ITEM:
			case REPORT_TYPE_PLUSALES_MODIFIER:
			case REPORT_TYPE_PLUSALES_DEPT_SUMMARY:
			case REPORT_TYPE_COVERS_DAILY:
			case REPORT_TYPE_HOURLY:
			case REPORT_TYPE_PAYMENT_SUMMARY:
			case REPORT_TYPE_TRANSACTION_NORMAL:
			case REPORT_TYPE_TRANSACTION_DISCREPANCY:
				bAcceptType = TRUE;
				break;
			}

			if ( nReportType >= EPOS_CUSTOM_FIRST_REPORT )
			{
				CEposReportSelectInfo infoReport;
				infoReport.SetReportType( nReportType );
				m_EposReportSelect.GetReportInfo( infoReport );
		
				switch( infoReport.GetCustomReportFamily() )
				{
				case EPOS_CUSTOM_FAMILY_HOURLY:
				case EPOS_CUSTOM_FAMILY_TRANSACTION:
					bAcceptType = TRUE;
					break;
				}
			}
		}

		if (TRUE == bAcceptType)
		{
			if (nReportType >= EPOS_CUSTOM_FIRST_REPORT)
			{
				CEposReportSelectInfo infoReport;
				infoReport.SetReportType(nReportType);
				m_EposReportSelect.GetReportInfo(infoReport);

				switch (infoReport.GetCustomReportFamily())
				{
				case EPOS_CUSTOM_FAMILY_LOYALTYRECON:
					bAcceptType = FALSE;
					break;
				}
			}
		}

		if (FALSE == bAcceptType)
		{
			continue;
		}

		switch( nReportType )
		{
		case REPORT_TYPE_PLUSALES:
		case REPORT_TYPE_PLUSALES_NOZEROSKIP:
		case REPORT_TYPE_PLUSALES_DEPT_ITEM:
		case REPORT_TYPE_PLUSALES_TAX_ITEM:
		case REPORT_TYPE_PLUSALES_TRANSERVER:
		case REPORT_TYPE_COVERS_PLU_TRANSERVER:
		case REPORT_TYPE_PLUSALES_CUSTOMER:
		case REPORT_TYPE_PLUSALES_DEPT_SUMMARY:
		case REPORT_TYPE_TAX_SUMMARY_TAX:
		case REPORT_TYPE_TAX_SUMMARY_PRICE:
		case REPORT_TYPE_PLUWASTE:
		case REPORT_TYPE_PLUWASTE_DEPT_ITEM:
		case REPORT_TYPE_PLUWASTE_TRANSERVER:
		case REPORT_TYPE_PLUWASTE_DEPT_SUMMARY:
		case REPORT_TYPE_HOURLY:
		case REPORT_TYPE_HOURLY_GROUP:
		case REPORT_TYPE_HOURLY_DEPT:
		case REPORT_TYPE_DAILY:
		case REPORT_TYPE_DAILY_GROUP:
		case REPORT_TYPE_DAILY_DEPT:
		case REPORT_TYPE_TRANSERVER_SUMMARY:
		case REPORT_TYPE_COVERS_HOURLY:
		case REPORT_TYPE_COVERS_GROUP:
		case REPORT_TYPE_COVERS_DEPT:
		case REPORT_TYPE_COVERS_HOURLY_GROUP:
		case REPORT_TYPE_COVERS_HOURLY_DEPT:
		case REPORT_TYPE_COVERS_DAILY:
		case REPORT_TYPE_COVERS_DAILY_GROUP:
		case REPORT_TYPE_COVERS_DAILY_DEPT:
		case REPORT_TYPE_COVERS_MONTHLY:
		case REPORT_TYPE_COVERS_PRICELEVEL:
		case REPORT_TYPE_COVERS_PAYMENT:
		case REPORT_TYPE_PAYMENT_SUMMARY:
		case REPORT_TYPE_PAYMENT_SERVER:
		case REPORT_TYPE_PAYMENT_GROUP_SUMMARY:
		case REPORT_TYPE_PAYMENT_ESTIMATE_GROUP:
		case REPORT_TYPE_PAYMENT_ESTIMATE_DEPT:
		case REPORT_TYPE_PAYMENT_ESTIMATE_ITEM:
		case REPORT_TYPE_PAYMENT_ESTIMATE_TAX:
		case REPORT_TYPE_PAYMENT_DETAIL:
		case REPORT_TYPE_DEPOSIT_PURCHASED:
		case REPORT_TYPE_DEPOSIT_REDEEMED:
		case REPORT_TYPE_DEPOSIT_UNREDEEMED:
		case REPORT_TYPE_DEPOSIT_BOOKING_PURCHASE:
		case REPORT_TYPE_DEPOSIT_BOOKING_EVENT:
		case REPORT_TYPE_CUSTOMER_RA:
		case REPORT_TYPE_CUSTOMER_SPEND:
		case REPORT_TYPE_CUSTOMER_ACTIVITY:
		case REPORT_TYPE_PROMOTION_SUMMARY:
		case REPORT_TYPE_MIXMATCH_GROUP:
		case REPORT_TYPE_MIXMATCH_DEPT:
		case REPORT_TYPE_MIXMATCH_ITEM:
		case REPORT_TYPE_MIXMATCH_GROUP_COVERS:
		case REPORT_TYPE_MIXMATCH_DEPT_COVERS:
		case REPORT_TYPE_MIXMATCH_ITEM_COVERS:
		case REPORT_TYPE_CHEAPESTONEFREE_SUMMARY:
		case REPORT_TYPE_CHEAPESTONEFREE_DETAIL:
		case REPORT_TYPE_TRANSERVER_VOID:
		case REPORT_TYPE_TRANSERVER_VOID_REASON:
		case REPORT_TYPE_TRANSERVER_REFUND:
		case REPORT_TYPE_TRANSERVER_REFUND_REASON:
		case REPORT_TYPE_TRANSERVER_DISCOUNT:
		case REPORT_TYPE_TRANSERVER_DISCOUNT_REASON:
		case REPORT_TYPE_TRANSERVER_WASTAGE_REASON:
		case REPORT_TYPE_TRANSERVER_VIP_REASON:
		case REPORT_TYPE_NOSALE:
		case REPORT_TYPE_AGEVERIFY:
		case REPORT_TYPE_TRANSACTION_NORMAL:
		case REPORT_TYPE_TRANSACTION_DISCREPANCY:
			{
				m_arrayReportTypes.Add(nReportType);
			}
			break;

		case REPORT_TYPE_SPTBOOK_CASCADE:
			if (DealerFlags.GetCascadeReportFlag() == TRUE)
			{
				m_arrayReportTypes.Add(nReportType);
			}
			break;

		case REPORT_TYPE_CUSTOMER_BALANCE:
			if (EcrmanOptions.GetFeaturesCustomerBalanceFlag() == TRUE)
			{
				m_arrayReportTypes.Add(nReportType);
			}
			break;

		case REPORT_TYPE_GENERAL_GROUP:
		case REPORT_TYPE_GENERAL_DEPT:
		case REPORT_TYPE_WEEKLY_GROUP:
		case REPORT_TYPE_WEEKLY_DEPT:
		case REPORT_TYPE_GENERAL_GROUP_QTY:
		case REPORT_TYPE_GENERAL_DEPT_QTY:
		case REPORT_TYPE_WEEKLY_GROUP_QTY:
		case REPORT_TYPE_WEEKLY_DEPT_QTY:
			if (EcrmanOptions.GetReportsShowCashChangeFlag() == TRUE)
			{
				m_arrayReportTypes.Add(nReportType);
			}
			break;
		
		case REPORT_TYPE_PLUSALES_MODIFIER:
		case REPORT_TYPE_PLUWASTE_MODIFIER:
			if (TRUE == bModifier)
			{
				m_arrayReportTypes.Add(nReportType);
			}
			break;

		case REPORT_TYPE_VIPSALES:
		case REPORT_TYPE_VIPSALES_DEPT_ITEM:
		case REPORT_TYPE_VIPSALES_TRANSERVER:
		case REPORT_TYPE_VIPSALES_DEPT_SUMMARY:
			if (TRUE == bVIP)
			{
				m_arrayReportTypes.Add(nReportType);
			}
			break;

		case REPORT_TYPE_VIPSALES_MODIFIER:
			if ((TRUE == bModifier) && (TRUE == bVIP))
			{
				m_arrayReportTypes.Add(nReportType);
			}
			break;

		case REPORT_TYPE_REASON_VOID:
		case REPORT_TYPE_REASON_REFUND:
		case REPORT_TYPE_REASON_PROMOTION:
		case REPORT_TYPE_REASON_WASTAGE:
			if (TRUE == bReason)
			{
				m_arrayReportTypes.Add(nReportType);
			}
			break;

		case REPORT_TYPE_REASON_VIP:
			if ((TRUE == bReason) && (TRUE == bVIP))
			{
				m_arrayReportTypes.Add(nReportType);
			}
			break;

		case REPORT_TYPE_EODSTATUS:
			if (TRUE == bEOD)
			{
				m_arrayReportTypes.Add(nReportType);
			}
			break;

		default:
			if (nReportType >= EPOS_CUSTOM_FIRST_REPORT)
			{
				m_arrayReportTypes.Add(nReportType);
			}
			break;
		}
	}
	
	if (m_arrayReportTypes.GetSize() == 0)
	{
		m_arrayReportTypes.Add(REPORT_TYPE_PLUSALES);
	}
}

/**********************************************************************/

CPosTrayBatchReportEposDlg::~CPosTrayBatchReportEposDlg()
{
	DataManagerNonDb.ServerNameTable.ClearList();
	DataManagerNonDb.CustomerNameTable.ClearList();
}

/**********************************************************************/

void CPosTrayBatchReportEposDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPosTrayBatchReportEposDlg)

	DDX_Control(pDX, IDC_TAB_FILTER, m_TabFilter );

	DDX_Control(pDX, IDC_STATIC_NAME, m_staticName);
	DDX_Control(pDX, IDC_EDIT_NAME, m_editName);

	DDX_Control(pDX, IDC_STATIC_DATE, m_staticDate);
	DDX_Control(pDX, IDC_STATIC_DATE1, m_staticDate1);
	DDX_Control(pDX, IDC_STATIC_DATE2, m_staticDate2);
	DDX_Control(pDX, IDC_STATIC_DATE3, m_staticDate3);
	DDX_Control(pDX, IDC_COMBO_DATES, m_comboDates);
	DDX_Control(pDX, IDC_COMBO_SESSION, m_comboSession);
	DDX_Control(pDX, IDC_COMBO_PLUTIME, m_comboPluTime);

	DDX_Control(pDX, IDC_STATIC_DATABASE, m_staticDatabase);
	DDX_Control(pDX, IDC_STATIC_DATABASE1, m_staticDatabase1);
	DDX_Control(pDX, IDC_STATIC_DATABASE2, m_staticDatabase2);
	DDX_Control(pDX, IDC_COMBO_LOCATION, m_comboLocation);
	DDX_Control(pDX, IDC_COMBO_TERMINAL, m_comboTerminal);
	DDX_Control(pDX, IDC_COMBO_TERMDATE, m_comboTermDate);
	DDX_Control(pDX, IDC_CHECK_REPORT_COMBINED, m_checkReportCombined);
	DDX_Control(pDX, IDC_CHECK_REPORT_TERMINAL, m_checkReportTerminal);
	DDX_Control(pDX, IDC_CHECK_REPORT_LOCATION, m_checkReportLocation);
	DDX_Control(pDX, IDC_CHECK_REPORT_DATABASE, m_checkReportDatabase);

	DDX_Control(pDX, IDC_STATIC_LOCATION, m_staticLocation);
	DDX_Control(pDX, IDC_RADIO_CONSOL_DEFAULT, m_radioConsolDefault);
	DDX_Control(pDX, IDC_RADIO_CONSOL_SAVED, m_radioConsolSaved);
	DDX_Control(pDX, IDC_RADIO_CONSOL_SYSTEM, m_radioConsolSystem);
	DDX_Control(pDX, IDC_CHECK_OVERRIDE, m_checkOverride);

	DDX_Control(pDX, IDC_STATIC_FILTER, m_staticFilter);
	DDX_Control(pDX, IDC_STATIC_FILTER1, m_staticFilter1);
	DDX_Control(pDX, IDC_STATIC_FILTER2, m_staticFilter2);
	DDX_Control(pDX, IDC_EDIT_DEPTNO, m_editDeptNo);
	DDX_Control(pDX, IDC_COMBO_DEPTNAME, m_comboDeptName);
	DDX_Control(pDX, IDC_COMBO_PLUFILTER, m_comboPluFilter);

	DDX_Control(pDX, IDC_STATIC_MISC, m_staticMisc);
	DDX_Control(pDX, IDC_CHECK_ITEMSERVER, m_checkItemServer);
	DDX_Control(pDX, IDC_CHECK_REPORTTIME, m_checkReportTime);

	DDX_Control(pDX, IDC_STATIC_PMS, m_staticPMS);
	DDX_Control(pDX, IDC_RADIO_PMS_CONSOLIDATE1, m_radioPMSConsolidate1);
	DDX_Control(pDX, IDC_RADIO_PMS_CONSOLIDATE2, m_radioPMSConsolidate2);
	DDX_Control(pDX, IDC_RADIO_PMS_CONSOLIDATE3, m_radioPMSConsolidate3);
	DDX_Control(pDX, IDC_CHECK_PENDING, m_checkPending);
	DDX_Control(pDX, IDC_CHECK_ACTIVE, m_checkActive);
	DDX_Control(pDX, IDC_CHECK_COMPLETE, m_checkComplete);
	DDX_Control(pDX, IDC_CHECK_CANCELLED, m_checkCancelled);

	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPosTrayBatchReportEposDlg, CDialog)
	//{{AFX_MSG_MAP(CPosTrayBatchReportEposDlg)

	ON_WM_MEASUREITEM()

	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_FILTER, OnSelectTabFilter)

	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDoubleClickList)
	
	ON_BN_CLICKED(IDC_RADIO_PMS_CONSOLIDATE1, OnRadioPMSConsolidate1)
	ON_BN_CLICKED(IDC_RADIO_PMS_CONSOLIDATE2, OnRadioPMSConsolidate2)
	ON_BN_CLICKED(IDC_RADIO_PMS_CONSOLIDATE3, OnRadioPMSConsolidate3)
	
	ON_CBN_SELCHANGE(IDC_COMBO_LOCATION, OnSelectLocation)
	ON_EN_KILLFOCUS(IDC_EDIT_DEPTNO, OnKillFocusDeptNo)
	ON_CBN_SELCHANGE(IDC_COMBO_DEPTNAME, OnSelectDeptName)
	
	//}}AFX_MSG_MAP

END_MESSAGE_MAP()

/**********************************************************************/

int CPosTrayBatchReportEposDlg::GetSelectedReportType()
{
	int nSel = m_listReport.GetCurSel();

	if ( nSel < 0 || nSel >= m_arrayReportTypes.GetSize() )
		return 0;
	else
		return m_arrayReportTypes.GetAt( nSel );
}

/**********************************************************************/

void CPosTrayBatchReportEposDlg::LoadEmailBatchSettings( CPosTrayTask& batch, bool bModifiedBatch )
{
	//BATCH NAME
	m_editName.SetWindowText( batch.GetBatchName() );

	//REPORT TYPES
	CCSV csvReports( batch.GetBatchReportList() );
	CReportConsolidationArray<CSortedIntItem> arrayTemp;
	for ( int n = 0; n < csvReports.GetSize(); n++ )
	{
		CSortedIntItem item;
		item.m_nItem = csvReports.GetInt(n);
		arrayTemp.Consolidate( item );
	}

	for ( int n = 0; ( n < m_listReport.GetItemCount() ) && ( n < m_arrayReportTypes.GetSize() ); n++ )
	{
		CSortedIntItem item;
		item.m_nItem = m_arrayReportTypes.GetAt(n);
		
		int nPos;
		m_listReport.SetItemTagState( n, arrayTemp.Find( item, nPos ) ? 1 : 0 );
	}

	//DATE TYPE
	int nResult = m_comboDates.FindStringExact( 0, batch.GetBatchDateType() );
	if ( nResult != CB_ERR )
		m_comboDates.SetCurSel( nResult );

	//REPORT SESSION
	{
		int nSel = 0;
		CString strSession = batch.GetBatchSessionEntity();

		if ( strSession == "EOD" )
		{
			if ( EcrmanOptions.GetReportsEODSessionFlag() == TRUE )
				nSel = DataManagerNonDb.ReportSession.GetSize();
		}
		else if ( strSession == "" )
		{
			strSession = batch.GetBatchSessionLegacy();

			nResult = m_comboSession.FindStringExact( 0, strSession );
			if ( nResult != CB_ERR )
				nSel = nResult;
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

	//MISC
	m_checkItemServer.SetCheck( batch.GetBatchItemServerFlag() );
	m_checkReportTime.SetCheck( batch.GetBatchReportTimeFlag() );
	
	//PMS
#ifdef COMPILE_PMS
	if ( SysInfo.IsPMSSystem() == TRUE )
	{
		if ( PMSOptions.GetFixedTimeReportEnableFlag() == FALSE )
		{
			int nPMSType = batch.GetBatchPMSType();
			if  ( ( nPMSType != 0 ) && ( nPMSType != 2 ) )
				nPMSType = 0;

			m_radioPMSConsolidate1.SetCheck( 0 == nPMSType );
			m_radioPMSConsolidate2.SetCheck( 2 == nPMSType );	
		}
		else
		{
			int nPMSType = batch.GetBatchPMSType();
			if  ( ( nPMSType < 0 ) || ( nPMSType > 2 ) )
				nPMSType = 0;

			m_radioPMSConsolidate1.SetCheck( 0 == nPMSType );
			m_radioPMSConsolidate2.SetCheck( 1 == nPMSType );
			m_radioPMSConsolidate3.SetCheck( 2 == nPMSType );	
		}

		m_checkPending.SetCheck( batch.GetBatchPMSPendingFlag() );
		m_checkActive.SetCheck( batch.GetBatchPMSActiveFlag() );
		m_checkComplete.SetCheck( batch.GetBatchPMSCompleteFlag() );
		m_checkCancelled.SetCheck( batch.GetBatchPMSCancelledFlag() );
	}
#endif

	//CONSOLIDATION
	int nPreferConsolType = batch.GetBatchPreferConsolType();
	switch ( nPreferConsolType )
	{
	case POSTRAY_EPOS_CONSOL_DEFAULT:
	case POSTRAY_EPOS_CONSOL_SAVED:
	case POSTRAY_EPOS_CONSOL_SYSTEM:
		break;

	default:
		nPreferConsolType = POSTRAY_EPOS_CONSOL_DEFAULT;
		break;
	}

	m_radioConsolDefault.SetCheck( POSTRAY_EPOS_CONSOL_DEFAULT == nPreferConsolType );
	m_radioConsolSaved.SetCheck( POSTRAY_EPOS_CONSOL_SAVED == nPreferConsolType );
	m_radioConsolSystem.SetCheck( POSTRAY_EPOS_CONSOL_SYSTEM == nPreferConsolType );
	m_checkOverride.SetCheck( batch.GetBatchJobSetConsolType() );

	//QUICK REPORTS
	m_checkReportTerminal.SetCheck( batch.GetBatchReportEcrFlag() );
	m_checkReportLocation.SetCheck( batch.GetBatchReportLocFlag() );
	m_checkReportDatabase.SetCheck( batch.GetBatchReportAllFlag() );

	//SELECTIVE REPORTS
	m_checkReportCombined.SetCheck( batch.GetBatchReportConFlag() );

	//LOCATION SELECT
	CLocationSelectorLookup LocSelLookup( batch );

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

	{
		int nTermDateType = batch.GetBatchTermDateType();
		if ((nTermDateType < 0) || (nTermDateType > 2))
		{
			nTermDateType = 0;
		}
		m_comboTermDate.SetCurSel(nTermDateType);
	}

	m_DeptAndPluComboHelpers.UpdateCombos();
	
	//DEPARTMENT FILTER
	m_DepartmentSelector.UpdateDepartmentCombo( batch.GetBatchDeptFilter() );
	m_DepartmentSelector.UpdateDepartmentEditBox();

	m_ParamsArray.SetBatchNo( batch.GetBatchNo() );
	m_ParamsArray.Read( bModifiedBatch );

	//PLU FILTER
	m_DeptAndPluComboHelpers.SetPluFilterNo( batch.GetBatchPluFilter() );

	//PLU TIME
	m_comboPluTime.SetCurSel( ( batch.GetBatchSaleTimeFlag() == 1 ) ? 1 : 0 );
}

/**********************************************************************/

BOOL CPosTrayBatchReportEposDlg::OnInitDialog() 
{
	m_DeptAndPluComboHelpers.SetControlPointers( &m_comboPluFilter, &m_LocationSelector, &m_DepartmentSelector );
	
	CDialog::OnInitDialog();
	
	m_editName.LimitText( POSTRAY_MAXLENGTH_BATCH_NAME );

	//REPORT LIST
	m_listReport.SubclassDlgItem ( IDC_LIST, this );
	m_listReport.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_listReport.InsertColumn ( 0, "", LVCFMT_LEFT, 240 );
	m_listReport.SetLockItemZeroFlag( FALSE );
	
	for ( int n = 0; n < m_arrayReportTypes.GetSize(); n++ )
	{
		int nReportType = m_arrayReportTypes.GetAt(n);

		CSSListTaggedSelectItem item;
		item.m_nData = 0;
		item.SetTagState(0);

		if ( nReportType < EPOS_CUSTOM_FIRST_REPORT )
			item.m_strText = CEposReport::GetReportNameExternal ( nReportType );
		else
			item.m_strText = m_EposReportSelect.GetCustomReportName( nReportType );

		m_listReport.AddItemToList( item );
	}

	//DATE AND REPORT SESSION SELECTION
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
	m_comboDates.SetCurSel( 0 );
	
	for (int nIndex = 0; nIndex < DataManagerNonDb.ReportSession.GetSize(); nIndex++)
	{
		m_comboSession.AddString(DataManagerNonDb.ReportSession.GetName(nIndex));
	}

	if (EcrmanOptions.GetReportsEODSessionFlag() == TRUE)
	{
		m_comboSession.AddString("EOD Business Day");
	}

	if (m_comboSession.GetCount() == 0)
	{
		m_comboSession.InsertString(0, "00:00 to 23:59");
	}

	m_comboSession.SetCurSel(0);
	m_comboSession.EnableWindow( m_comboSession.GetCount() > 1 );
	
	//SELECTIVE REPORTS LOCATION
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
	
	m_LocationSelector.SetDatabaseMode ( NODE_LOCATION );
	m_LocationSelector.BuildList( arrayTypes );
	m_LocationSelector.FillLocationCombo ( &m_comboLocation, &m_comboTerminal );
	
	//TERM DATE
	{
		CArray<int, int> arrayStops;
		arrayStops.Add(5);
		arrayStops.Add(35);
		m_comboTermDate.SetTabStops(arrayStops);
	}

	m_comboTermDate.AddItem(",Date info by terminal");
	m_comboTermDate.AddItem(",Date info by location");
	m_comboTermDate.AddItem(",Date info summary");
	m_comboTermDate.SetCurSel(0);

	//PLU SALE TIME
	m_comboPluTime.AddString( "Transaction completion time" );
	m_comboPluTime.AddString( "Action time (where available)" );
	
	//SELECTIVE REPORTS DEPARTMENT
	m_editDeptNo.LimitText( MAX_LENGTH_DEPTNO );
	
	//INITIAL REPORT SELECTION
	m_listReport.SetCurSel(0);
	
#ifdef COMPILE_PMS
	if ( SysInfo.IsPMSSystem() == TRUE )
	{
		m_checkPending.SetCheck( TRUE );
		m_checkActive.SetCheck( TRUE );
		m_checkComplete.SetCheck( TRUE );
		m_checkCancelled.SetCheck( TRUE );
		HandlePMSRadioConsolidate(0);

		if ( PMSOptions.GetFixedTimeReportEnableFlag() == FALSE )
		{
			m_radioPMSConsolidate2.SetWindowText( "Report on EPOS sales by originating terminal" );
		}
		else
		{
			m_radioPMSConsolidate2.SetWindowText( "Report on completed bookings by fixed time" );
			m_radioPMSConsolidate3.SetWindowText( "Report on EPOS sales by originating terminal" );
		}
	}
#endif

	LoadEmailBatchSettings( m_BatchEmail, m_bModifiedBatch );

	m_TabFilter.InsertItem( 0, "General" );
	m_TabFilter.InsertItem( 1, "Advanced" );
	if ( SysInfo.IsPMSSystem() == TRUE ) m_TabFilter.InsertItem( 2, "Guest Accounts" );
	m_nTabPageFilter = 0;
	SelectTabFilter();

	MoveControl( &m_staticLocation, 7, 28 );
	MoveControl( &m_radioConsolDefault, 21, 47 );
	MoveControl( &m_radioConsolSaved, 21, 63 );
	MoveControl( &m_radioConsolSystem, 21, 79 );
	MoveControl( &m_checkOverride, 21, 99 );
	MoveControl( &m_staticFilter, 7, 146 );
	MoveControl( &m_staticFilter1, 21, 170 );
	MoveControl( &m_editDeptNo, 76, 169 );
	MoveControl( &m_comboDeptName, 103, 170 );
	MoveControl( &m_staticFilter2, 21, 190 );
	MoveControl( &m_comboPluFilter, 76, 190 );	
	MoveControl( &m_staticMisc, 7, 237 );
	MoveControl( &m_checkItemServer, 21, 261 );
	MoveControl( &m_checkReportTime, 21, 278 );
	/*****/
	MoveControl( &m_staticPMS, 7, 28 );
	MoveControl( &m_radioPMSConsolidate1, 21, 47 );
	MoveControl( &m_radioPMSConsolidate2, 21, 63 );
	MoveControl( &m_radioPMSConsolidate3, 21, 79 );
	MoveControl( &m_checkPending, 21, 109 );
	MoveControl( &m_checkActive, 21, 125 );
	MoveControl( &m_checkComplete, 21, 141 );
	MoveControl( &m_checkCancelled, 21, 157 );
	/*****/
	ResizeDialog( 520, 350 );

	return TRUE;  
}

/**********************************************************************/

void CPosTrayBatchReportEposDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
   if ( nIDCtl == IDC_LIST )  
   {
	   m_listReport.AdjustRowHeight( lpMeasureItemStruct, 10 ); 
   }
}

/**********************************************************************/

void CPosTrayBatchReportEposDlg::OnSelectTabFilter(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;

	m_nTabPageFilter = m_TabFilter.GetCurSel();
	
	int nMaxPageFilter = ( SysInfo.IsPMSSystem() ? 2 : 1 );
	if ( ( m_nTabPageFilter < 0 ) || ( m_nTabPageFilter > nMaxPageFilter ) )
		m_nTabPageFilter = 0;

	SelectTabFilter();
}

/**********************************************************************/

void CPosTrayBatchReportEposDlg::SelectTabFilter()
{
#ifdef COMPILE_PMS
	bool bAllowPMSConsol3 = (PMSOptions.GetFixedTimeReportEnableFlag());
#else
	bool bAllowPMSConsol3 = FALSE;
#endif

	ShowAndEnableWindow(&m_staticName, 0 == m_nTabPageFilter);
	ShowAndEnableWindow(&m_editName, 0 == m_nTabPageFilter);
	ShowAndEnableWindow(&m_staticDate, 0 == m_nTabPageFilter);
	ShowAndEnableWindow(&m_staticDate1, 0 == m_nTabPageFilter);
	ShowAndEnableWindow(&m_staticDate2, 0 == m_nTabPageFilter);
	ShowAndEnableWindow(&m_staticDate3, 0 == m_nTabPageFilter);
	ShowAndEnableWindow(&m_comboDates, 0 == m_nTabPageFilter);
	ShowAndEnableWindow(&m_comboSession, 0 == m_nTabPageFilter);
	ShowAndEnableWindow(&m_comboPluTime, 0 == m_nTabPageFilter);
	ShowAndEnableWindow(&m_staticDatabase, 0 == m_nTabPageFilter);
	ShowAndEnableWindow(&m_staticDatabase1, 0 == m_nTabPageFilter);
	ShowAndEnableWindow(&m_staticDatabase2, 0 == m_nTabPageFilter);
	ShowAndEnableWindow(&m_comboLocation, 0 == m_nTabPageFilter);
	ShowAndEnableWindow(&m_comboTerminal, 0 == m_nTabPageFilter);
	ShowAndEnableWindow(&m_comboTermDate, 0 == m_nTabPageFilter);
	ShowAndEnableWindow(&m_checkReportCombined, 0 == m_nTabPageFilter);
	ShowAndEnableWindow(&m_checkReportTerminal, 0 == m_nTabPageFilter);
	ShowAndEnableWindow(&m_checkReportLocation, 0 == m_nTabPageFilter);
	ShowAndEnableWindow(&m_checkReportDatabase, 0 == m_nTabPageFilter);
	/****/
	ShowAndEnableWindow(&m_staticLocation, 1 == m_nTabPageFilter);
	ShowAndEnableWindow(&m_radioConsolDefault, 1 == m_nTabPageFilter);
	ShowAndEnableWindow(&m_radioConsolSaved, 1 == m_nTabPageFilter);
	ShowAndEnableWindow(&m_radioConsolSystem, 1 == m_nTabPageFilter);
	ShowAndEnableWindow(&m_checkOverride, 1 == m_nTabPageFilter);
	ShowAndEnableWindow(&m_staticFilter, 1 == m_nTabPageFilter);
	ShowAndEnableWindow(&m_staticFilter1, 1 == m_nTabPageFilter);
	ShowAndEnableWindow(&m_staticFilter2, 1 == m_nTabPageFilter);
	ShowAndEnableWindow(&m_editDeptNo, 1 == m_nTabPageFilter);
	ShowAndEnableWindow(&m_comboDeptName, 1 == m_nTabPageFilter);
	ShowAndEnableWindow(&m_comboPluFilter, 1 == m_nTabPageFilter);
	ShowAndEnableWindow(&m_staticMisc, 1 == m_nTabPageFilter);
	ShowAndEnableWindow(&m_checkItemServer, 1 == m_nTabPageFilter);
	ShowAndEnableWindow(&m_checkReportTime, 1 == m_nTabPageFilter);
	/****/
	ShowAndEnableWindow(&m_staticPMS, 2 == m_nTabPageFilter);
	ShowAndEnableWindow(&m_radioPMSConsolidate1, 2 == m_nTabPageFilter);
	ShowAndEnableWindow(&m_radioPMSConsolidate2, 2 == m_nTabPageFilter);
	ShowAndEnableWindow(&m_radioPMSConsolidate3, 2 == m_nTabPageFilter && (bAllowPMSConsol3));
	ShowAndEnableWindow(&m_checkPending, 2 == m_nTabPageFilter);
	ShowAndEnableWindow(&m_checkActive, 2 == m_nTabPageFilter);
	ShowAndEnableWindow(&m_checkComplete, 2 == m_nTabPageFilter);
	ShowAndEnableWindow(&m_checkCancelled, 2 == m_nTabPageFilter);

	bool bEnableSession = (m_comboSession.GetCount() > 1);
	bool bEnableLocation = (m_comboLocation.GetCount() > 1);
	bool bEnableQuick = (m_comboLocation.GetCount() >= 1);

	if (FALSE == bEnableSession)
	{
		m_comboSession.EnableWindow(FALSE);
	}

	if (FALSE == bEnableLocation)
	{
		m_comboLocation.EnableWindow(FALSE);
	}

	if (FALSE == bEnableQuick)
	{
		m_checkReportDatabase.EnableWindow(FALSE);
		m_checkReportLocation.EnableWindow(FALSE);
		m_checkReportTerminal.EnableWindow(FALSE);
	}
}

/**********************************************************************/

void CPosTrayBatchReportEposDlg::HandlePMSRadioConsolidate( int n )
{
#ifdef COMPILE_PMS
	int nMax = ( PMSOptions.GetFixedTimeReportEnableFlag() ? 2 : 1 );

	if ( ( n >= 0 ) && ( n <= nMax ) )
	{
		m_nPMSConsolidationMode = n;
		m_radioPMSConsolidate1.SetCheck( 0 == m_nPMSConsolidationMode );
		m_radioPMSConsolidate2.SetCheck( 1 == m_nPMSConsolidationMode );
		m_radioPMSConsolidate3.SetCheck( 2 == m_nPMSConsolidationMode );
		m_checkPending.EnableWindow( 0 == m_nPMSConsolidationMode );
		m_checkActive.EnableWindow( 0 == m_nPMSConsolidationMode );
		m_checkComplete.EnableWindow( 0 == m_nPMSConsolidationMode );
		m_checkCancelled.EnableWindow( 0 == m_nPMSConsolidationMode );
	}
#endif
}

/**********************************************************************/

void CPosTrayBatchReportEposDlg::OnRadioPMSConsolidate1()
{
	HandlePMSRadioConsolidate(0);
}

/**********************************************************************/

void CPosTrayBatchReportEposDlg::OnRadioPMSConsolidate2()
{
	HandlePMSRadioConsolidate(1);
}

/**********************************************************************/

void CPosTrayBatchReportEposDlg::OnRadioPMSConsolidate3()
{
	HandlePMSRadioConsolidate(2);
}

/**********************************************************************/

void CPosTrayBatchReportEposDlg::OnSelectLocation() 
{
	CLocationSelectorEntity LocSelEntity;
	m_LocationSelector.GetSelectedEntity( LocSelEntity );	
	m_LocationSelector.FillEposCombo();
	m_DeptAndPluComboHelpers.UpdateCombos();
}

/**********************************************************************/

void CPosTrayBatchReportEposDlg::OnKillFocusDeptNo() 
{
	m_DepartmentSelector.SelectDepartmentFromEditBox();
}

/**********************************************************************/

void CPosTrayBatchReportEposDlg::OnSelectDeptName() 
{
	m_DepartmentSelector.SelectDepartmentFromCombo();
}

/**********************************************************************/

void CPosTrayBatchReportEposDlg::OnDoubleClickList( NMHDR* pNMHDR, LRESULT* pResult ) 
{
	if ( m_checkOverride.GetCheck() != 0 )
	{
		int nReportType = GetSelectedReportType();

		CEposReportSelectInfo infoReport;
		infoReport.SetReportType( nReportType );
		m_EposReportSelect.GetReportInfo( infoReport );

		//LOAD SAVED PREFENCES FOR MANUAL EPOS REPORTS
		if ( infoReport.LoadConsolPrefs() == TRUE )
			m_EposReportSelect.SetReportInfo( infoReport );

		if ( CReportEposConsolDlg::CanDoAdhocReport( infoReport ) == TRUE )
		{
			CPosTrayEposConsolParamsInfo infoParams;
			m_ParamsArray.GetParams( infoReport, infoParams );

			int nTaskType = POSTRAY_EPOS_CONSOL_DEFAULT;
			if ( m_radioConsolSaved.GetCheck() != 0 )
				nTaskType = POSTRAY_EPOS_CONSOL_SAVED;
			else if ( m_radioConsolSystem.GetCheck() != 0 )
				nTaskType = POSTRAY_EPOS_CONSOL_SYSTEM;

			CPosTrayEposConsolDlg dlg( infoReport, infoParams, nTaskType, this );
			if ( dlg.DoModal() == IDOK )
			{
				m_ParamsArray.SetParams( infoReport, infoParams );
				m_ParamsArray.Write();
			}
		}
	}	

	*pResult = 0;
}

/**********************************************************************/

void CPosTrayBatchReportEposDlg::SaveEmailBatchSettings( CPosTrayTask& batch )
{
	//BATCH NAME
	CString strName;
	m_editName.GetWindowText( strName );
	batch.SetBatchName( strName );

	//REPORT TYPES
	CCSV csv;
	for ( int n = 0; ( n < m_listReport.GetItemCount() ) && ( n < m_arrayReportTypes.GetSize() ); n++ )
	{
		if ( m_listReport.GetItemTagState(n) == 1 )
			csv.Add( m_arrayReportTypes.GetAt(n) );
	}

	batch.SetBatchReportList( csv.GetLine() );

	//DATE TYPE
	CString strDateType;
	m_comboDates.GetWindowText( strDateType );
	batch.SetBatchDateType( strDateType );

	//REPORT SESSION
	{
		int nSel = m_comboSession.GetCurSel();
		
		if ( ( nSel >= 0 ) && ( nSel < DataManagerNonDb.ReportSession.GetSize() ) )
		{
			CSessionCSVRecord Session;
			DataManagerNonDb.ReportSession.GetAt( nSel, Session );
			batch.SetBatchSessionEntity( Session.GetEntityID() );
		}
		else if ( ( nSel == DataManagerNonDb.ReportSession.GetSize() ) && ( EcrmanOptions.GetReportsEODSessionFlag() == TRUE ) )
			batch.SetBatchSessionEntity( "EOD" );
		else
			batch.SetBatchSessionEntity( "" );

		batch.SetBatchSessionLegacy( "" );
	}

	//MISC
	batch.SetBatchItemServerFlag( m_checkItemServer.GetCheck() != 0 );
	batch.SetBatchTrainServerFlag( FALSE );
	batch.SetBatchReportTimeFlag( m_checkReportTime.GetCheck() != 0 );
	
	//PMS
#ifdef COMPILE_PMS
	if ( SysInfo.IsPMSSystem() == TRUE )
	{
		if ( PMSOptions.GetFixedTimeReportEnableFlag() == FALSE )
		{
			int nPMSType = 0;
			if ( m_radioPMSConsolidate2.GetCheck() != 0 ) nPMSType = 2;
			batch.SetBatchPMSType( nPMSType );
		}
		else
		{
			int nPMSType = 0;
			if ( m_radioPMSConsolidate2.GetCheck() != 0 ) nPMSType = 1;
			if ( m_radioPMSConsolidate3.GetCheck() != 0 ) nPMSType = 2;
			batch.SetBatchPMSType( nPMSType );
		}

		batch.SetBatchPMSPendingFlag( m_checkPending.GetCheck() != 0 );
		batch.SetBatchPMSActiveFlag( m_checkActive.GetCheck() != 0 );
		batch.SetBatchPMSCompleteFlag( m_checkComplete.GetCheck() != 0 );
		batch.SetBatchPMSCancelledFlag( m_checkCancelled.GetCheck() != 0 );
	}
#endif

	if ( SysInfo.IsPMSSystem() == FALSE )
	{
		batch.SetBatchPMSType(0);
		batch.SetBatchPMSPendingFlag( FALSE );
		batch.SetBatchPMSActiveFlag( FALSE );
		batch.SetBatchPMSCompleteFlag( FALSE );
		batch.SetBatchPMSCancelledFlag( FALSE );
	}

	//CONSOLIDATION
	int nType = POSTRAY_EPOS_CONSOL_DEFAULT;
	if ( m_radioConsolSaved.GetCheck() != 0 )
		nType = POSTRAY_EPOS_CONSOL_SAVED;
	else if ( m_radioConsolSystem.GetCheck() != 0 )
		nType = POSTRAY_EPOS_CONSOL_SYSTEM;

	batch.SetBatchPreferConsolType( nType );
	batch.SetBatchJobSetConsolType( m_checkOverride.GetCheck() != 0 );

	//QUICK REPORTS
	batch.SetBatchReportEcrFlag( m_checkReportTerminal.GetCheck() != 0 );
	batch.SetBatchReportLocFlag( m_checkReportLocation.GetCheck() != 0 );
	batch.SetBatchReportAllFlag( m_checkReportDatabase.GetCheck() != 0 );

	//SELECTIVE REPORTS
	batch.SetBatchReportConFlag( m_checkReportCombined.GetCheck() != 0 );

	//LOCATION SELECT
	CLocationSelectorEntity LocSelEntity;
	m_LocationSelector.GetSelectedEntity( LocSelEntity );
	batch.SetBatchConType( LocSelEntity.GetConType() );
	batch.SetBatchConDbNo( LocSelEntity.GetDbNo() );
	batch.SetBatchConNwkNo( LocSelEntity.GetNwkNo() );
	batch.SetBatchConEntityNo( LocSelEntity.GetEntityNo() );
	batch.SetBatchConTermIdx( LocSelEntity.GetTermIdx() );
	
	batch.SetBatchTermDateType(m_comboTermDate.GetCurSel());

	//DEPARTMENT FILTER
	batch.SetBatchDeptFilter( m_DepartmentSelector.GetDepartmentFilter() );

	//PLU FILTER
	batch.SetBatchPluFilter( m_DeptAndPluComboHelpers.GetPluFilterNo() );

	//PLU TIME
	batch.SetBatchSaleTimeFlag( m_comboPluTime.GetCurSel() == 1 );
}

/**********************************************************************/

void CPosTrayBatchReportEposDlg::OnOK()
{
	SaveEmailBatchSettings( m_BatchEmail );
	EndDialog( IDOK );
}

/**********************************************************************/
