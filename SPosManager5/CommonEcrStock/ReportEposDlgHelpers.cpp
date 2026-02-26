/**********************************************************************/
#include "..\CommonEcrStock\DefEditDlg.h"
#include "..\CommonEcrStock\PresetReportBase.h"
/**********************************************************************/
#include "CommsHandshaker.h"
#include "DateRangeHelpers.h"
#include "EposReportCustomDepositDlg.h"
#include "EposReportCustomDiscountDlg.h"
#include "EposReportCustomGeneralDlg.h"
#include "EposReportCustomHourlyDlg.h"
#include "EposReportCustomLoyaltyReconDlg.h"
#include "EposReportCustomMixMatchDlg.h"
#include "EposReportCustomPaymentDetailDlg.h"
#include "EposReportCustomPluPriceBandDlg.h"
#include "EposReportCustomTransactionDlg.h"
#include "EposReportSelect.h"
#include "ListDataPresetDlg.h"
#include "MaxLengths.h"
#include "NodeTypes.h"
#include "OfferGroupsCSVArray.h"
#include "Password.h"
#include "PMSOptions.h"
#include "PresetReportManager.h"
#include "ReportEposConsolDlg.h"
#include "ReportEposDlg.h"
#include "ReportEposPMSDlg.h"
#include "ReportHelpers.h"
#include "ReportTypes.h"
#include "RepSpawn.h"
#include "SelectPresetActionDlg.h"
#include "SessionEditDlg.h"
#include "SysInfo.h"
/**********************************************************************/
#include "ReportEposDlgHelpers.h"
/**********************************************************************/

CReportEposDlgHelpers::CReportEposDlgHelpers()
{
	m_pParentWnd = NULL;
	m_nPMSConsolidationMode = 0;

	m_EposReportSelect.LoadReportSelection();

	bool bModifier = (SysInfo.GetMaxBasePluLen() != 0);
	bool bReason = (EcrmanOptions.GetReportsEODReasonsFlag() == TRUE);
	bool bVIP = (EcrmanOptions.GetReportsVIPType() != 0);
	bool bEOD = (EcrmanOptions.GetReportsEODSessionFlag() != 0);

	for (int n = 0; n < m_EposReportSelect.GetSelectedReportCount(); n++)
	{
		int nReportType = m_EposReportSelect.GetSelectedReportType(n);

		bool bAcceptType = TRUE;

		if (SysInfo.IsEcrLiteSystem() == TRUE)
		{
			bAcceptType = FALSE;

			switch (nReportType)
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

			if (nReportType >= EPOS_CUSTOM_FIRST_REPORT)
			{
				CEposReportSelectInfo infoReport;
				infoReport.SetReportType(nReportType);
				m_EposReportSelect.GetReportInfo(infoReport);

				switch (infoReport.GetCustomReportFamily())
				{
				case EPOS_CUSTOM_FAMILY_HOURLY:
				case EPOS_CUSTOM_FAMILY_TRANSACTION:
					bAcceptType = TRUE;
					break;
				}
			}

		}

		if (nReportType >= EPOS_CUSTOM_FIRST_REPORT)
		{
			if (TRUE == bAcceptType)
			{
				CEposReportSelectInfo infoReport;
				infoReport.SetReportType(nReportType);
				m_EposReportSelect.GetReportInfo(infoReport);

				switch (infoReport.GetCustomReportFamily())
				{
				case EPOS_CUSTOM_FAMILY_LOYALTYRECON:

#ifdef POSTRAY_UTILITY
					bAcceptType = FALSE;
#else
					bAcceptType = DealerFlags.GetLoyaltyReconReportFlag();
#endif
					break;
				}
			}
		}

		if (FALSE == bAcceptType)
		{
			continue;
		}

		switch (nReportType)
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
		case REPORT_TYPE_COVERS_GROUP:
		case REPORT_TYPE_COVERS_DEPT:
		case REPORT_TYPE_COVERS_HOURLY:
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
			m_arrayReportTypesMaster.Add(nReportType);
			break;

		case REPORT_TYPE_SPTBOOK_CASCADE:
			if (DealerFlags.GetCascadeReportFlag() == TRUE)
				m_arrayReportTypesMaster.Add(nReportType);
			break;

		case REPORT_TYPE_CUSTOMER_BALANCE:
			if (EcrmanOptions.GetFeaturesCustomerBalanceFlag() == TRUE)
				m_arrayReportTypesMaster.Add(nReportType);
			break;

		case REPORT_TYPE_GENERAL_GROUP:
		case REPORT_TYPE_GENERAL_DEPT:
		case REPORT_TYPE_GENERAL_GROUP_QTY:
		case REPORT_TYPE_GENERAL_DEPT_QTY:
		case REPORT_TYPE_WEEKLY_GROUP:
		case REPORT_TYPE_WEEKLY_DEPT:
		case REPORT_TYPE_WEEKLY_GROUP_QTY:
		case REPORT_TYPE_WEEKLY_DEPT_QTY:
			if (EcrmanOptions.GetReportsShowCashChangeFlag() == TRUE)
				m_arrayReportTypesMaster.Add(nReportType);
			break;

		case REPORT_TYPE_PLUSALES_MODIFIER:
		case REPORT_TYPE_PLUWASTE_MODIFIER:
			if (TRUE == bModifier)
				m_arrayReportTypesMaster.Add(nReportType);
			break;

		case REPORT_TYPE_VIPSALES:
		case REPORT_TYPE_VIPSALES_DEPT_ITEM:
		case REPORT_TYPE_VIPSALES_TRANSERVER:
		case REPORT_TYPE_VIPSALES_DEPT_SUMMARY:
			if (TRUE == bVIP)
				m_arrayReportTypesMaster.Add(nReportType);
			break;

		case REPORT_TYPE_VIPSALES_MODIFIER:
			if ((TRUE == bModifier) && (TRUE == bVIP))
				m_arrayReportTypesMaster.Add(nReportType);
			break;

		case REPORT_TYPE_REASON_VOID:
		case REPORT_TYPE_REASON_REFUND:
		case REPORT_TYPE_REASON_PROMOTION:
		case REPORT_TYPE_REASON_WASTAGE:
			if (TRUE == bReason)
				m_arrayReportTypesMaster.Add(nReportType);
			break;

		case REPORT_TYPE_REASON_VIP:
			if ((TRUE == bReason) && (TRUE == bVIP))
				m_arrayReportTypesMaster.Add(nReportType);
			break;

		case REPORT_TYPE_EODSTATUS:
			if (TRUE == bEOD)
				m_arrayReportTypesMaster.Add(nReportType);
			break;

		default:
			if (nReportType >= EPOS_CUSTOM_FIRST_REPORT)
			{
				m_arrayReportTypesMaster.Add(nReportType);
			}
			break;
		}
	}

	if (m_arrayReportTypesMaster.GetSize() == 0)
	{
		m_arrayReportTypesMaster.Add(REPORT_TYPE_PLUSALES);
	}

	CPresetReportInfo Dummy;
	SetReportsTypeBatch(Dummy, TRUE);
}

/**********************************************************************/

void CReportEposDlgHelpers::SetReportsTypeBatch( CPresetReportInfo& BatchReport, bool bAll )
{
	m_arrayReportTypesBatch.RemoveAll();

	CPresetReportInfoEposBatch Preset;
	{
		CString str = BatchReport.GetOtherFields();
		Preset.FromString( str );
	}

	for ( int n = 0; n < m_arrayReportTypesMaster.GetSize(); n++ )
	{
		WORD wReportType = m_arrayReportTypesMaster.GetAt(n);

		if ( ( TRUE == bAll ) || ( Preset.GetReportFlag( wReportType ) == TRUE ) )
			m_arrayReportTypesBatch.Add( wReportType );
	}

	if ( m_arrayReportTypesBatch.GetSize() == 0 )
		m_arrayReportTypesBatch.Add ( REPORT_TYPE_PLUSALES );
}

/**********************************************************************/

void CReportEposDlgHelpers::SetControlPointers( void* pDlg )
{
	m_pParentWnd = ( CWnd* ) pDlg;

	if ( SysInfo.IsPMSSystem() == FALSE )
	{
		CReportEposDlg* pDlg2 = ( CReportEposDlg*) pDlg;

		m_pDateSelectorRange = &(pDlg2 -> m_DateSelectorRange);
		m_pLocationSelector = &(pDlg2 -> m_LocationSelector);
		m_pDepartmentSelector = &(pDlg2 -> m_DepartmentSelector);
		
		m_pComboBatch = &(pDlg2 -> m_comboBatch);
		m_pListReport = &(pDlg2 -> m_listReport);
		m_pComboDates = &(pDlg2 -> m_comboDates);
		m_pDatePickerFrom = &(pDlg2 -> m_DatePickerFrom);
		m_pDatePickerTo = &(pDlg2 -> m_DatePickerTo);
		m_pComboSession = &(pDlg2 -> m_comboSession);
		m_pRadioConsolDefault = &(pDlg2 -> m_radioConsolDefault );
		m_pRadioConsolSaved = &(pDlg2 -> m_radioConsolSaved );
		m_pRadioConsolAdhoc = &(pDlg2 -> m_radioConsolAdhoc );
		m_pRadioConsolSystem = &(pDlg2 -> m_radioConsolSystem );
		m_pCheckReportAllLocations = &(pDlg2 -> m_checkReportAllLocations);
		m_pCheckReportLocations = &(pDlg2 -> m_checkReportLocations);
		m_pCheckReportEpos = &(pDlg2 -> m_checkReportEpos);
		m_pButtonPrintQuick = &(pDlg2 -> m_buttonPrintQuick);
		m_pComboPreset = &(pDlg2 -> m_comboPreset);
		m_pComboLocation = &(pDlg2 -> m_comboLocation);
		m_pComboTerminal = &(pDlg2 -> m_comboTerminal);
		m_pComboTermDate = &(pDlg2->m_comboTermDate);
		m_pButtonDisplaySingle = &(pDlg2 -> m_buttonDisplaySingle);
		m_pButtonPrintMultiple = &(pDlg2 -> m_buttonPrintMultiple);
		m_pButtonGraphSingle = &(pDlg2 -> m_buttonGraphSingle);
		m_pCheckItemServer = &(pDlg2 -> m_checkItemServer);

		m_pEditDeptNo = &(pDlg2 -> m_editDeptNo );
		m_pComboPluFilter = &(pDlg2 -> m_comboPluFilter );
		m_pComboPluTime = &(pDlg2 -> m_comboPluTime );
	}
	
#ifdef COMPILE_PMS

	if ( SysInfo.IsPMSSystem() == TRUE )
	{
		CReportEposPMSDlg* pDlg2 = ( CReportEposPMSDlg*) pDlg;

		m_pDateSelectorRange = &(pDlg2 -> m_DateSelectorRange);
		m_pLocationSelector = &(pDlg2 -> m_LocationSelector);
		m_pDepartmentSelector = &(pDlg2 -> m_DepartmentSelector);

		m_pComboBatch = &(pDlg2 -> m_comboBatch);
		m_pListReport = &(pDlg2 -> m_listReport);
		m_pComboDates = &(pDlg2 -> m_comboDates);
		m_pDatePickerFrom = &(pDlg2 -> m_DatePickerFrom);
		m_pDatePickerTo = &(pDlg2 -> m_DatePickerTo);
		m_pComboSession = &(pDlg2 -> m_comboSession);
		m_pCheckPending = &(pDlg2 -> m_checkPending);
		m_pCheckActive = &(pDlg2 -> m_checkActive);
		m_pCheckComplete = &(pDlg2 -> m_checkComplete);
		m_pCheckCancelled = &(pDlg2 -> m_checkCancelled);
		m_pRadioPMSConsolidate1 = &(pDlg2 -> m_radioPMSConsolidate1);
		m_pRadioPMSConsolidate2 = &(pDlg2 -> m_radioPMSConsolidate2);
		m_pRadioPMSConsolidate3 = &(pDlg2 -> m_radioPMSConsolidate3);
		m_pRadioConsolDefault = &(pDlg2 -> m_radioConsolDefault );
		m_pRadioConsolSaved = &(pDlg2 -> m_radioConsolSaved );
		m_pRadioConsolAdhoc = &(pDlg2 -> m_radioConsolAdhoc );
		m_pRadioConsolSystem = &(pDlg2 -> m_radioConsolSystem );
		m_pCheckReportAllLocations = &(pDlg2 -> m_checkReportAllLocations);
		m_pCheckReportLocations = &(pDlg2 -> m_checkReportLocations);
		m_pCheckReportEpos = &(pDlg2 -> m_checkReportEpos);
		m_pButtonPrintQuick = &(pDlg2 -> m_buttonPrintQuick);
		m_pComboLocation = &(pDlg2 -> m_comboLocation);
		m_pComboPreset = &(pDlg2 -> m_comboPreset);
		m_pComboTerminal = &(pDlg2 -> m_comboTerminal);
		m_pComboTermDate = &(pDlg2->m_comboTermDate);
		m_pButtonDisplaySingle = &(pDlg2 -> m_buttonDisplaySingle);
		m_pButtonPrintMultiple = &(pDlg2 -> m_buttonPrintMultiple);
		m_pButtonGraphSingle = &(pDlg2 -> m_buttonGraphSingle);
		m_pCheckItemServer = &(pDlg2 -> m_checkItemServer);

		m_pEditDeptNo = &(pDlg2 -> m_editDeptNo );
		m_pComboPluFilter = &(pDlg2 -> m_comboPluFilter );
		m_pComboPluTime = &(pDlg2 -> m_comboPluTime );
	}

#endif

	m_DeptAndPluComboHelpers.SetControlPointers( m_pComboPluFilter, m_pLocationSelector, m_pDepartmentSelector );
}

/**********************************************************************/

bool CReportEposDlgHelpers::CheckGraphableJobs()
{
	if ( SysInfo.IsEcrLiteSystem() == TRUE )
		return FALSE;

	int nReportType = GetSelectedReportType(); 

	switch ( nReportType )
	{
	case REPORT_TYPE_HOURLY:
	case REPORT_TYPE_COVERS_HOURLY:
	case REPORT_TYPE_COVERS_MONTHLY:
	case REPORT_TYPE_TRANSERVER_SUMMARY:	
	case REPORT_TYPE_PLUSALES_DEPT_SUMMARY:
	case REPORT_TYPE_PLUWASTE_DEPT_SUMMARY:
	case REPORT_TYPE_PAYMENT_SUMMARY:	
		return TRUE;

	case REPORT_TYPE_VIPSALES_DEPT_SUMMARY:
		return ( EcrmanOptions.GetReportsVIPType() == 1 );

	default:
		if ( nReportType >= EPOS_CUSTOM_FIRST_REPORT )
		{
			CEposReportSelectInfo infoReport;
			infoReport.SetReportType( nReportType );
			m_EposReportSelect.GetReportInfo( infoReport );
			
			switch( infoReport.GetCustomReportFamily() )
			{
			case EPOS_CUSTOM_FAMILY_HOURLY:
			case EPOS_CUSTOM_FAMILY_COVERS_HOURLY:
				return TRUE;

			default:
				return FALSE;
			}
		}
		return FALSE;
	}
}

/**********************************************************************/

bool CReportEposDlgHelpers::CheckPrintableJobs()
{
	for ( int nIndex = 0; nIndex < m_pListReport -> GetItemCount(); nIndex++ )
	{
		if ( m_pListReport -> GetItemTagState( nIndex ) == 1 )
			return TRUE;
	}
	return FALSE;
}

/**********************************************************************/

bool CReportEposDlgHelpers::CheckQuickPrintOptions()
{
	return ( ( m_pCheckReportAllLocations -> GetCheck() != 0 ) || ( m_pCheckReportEpos -> GetCheck() != 0 ) || ( m_pCheckReportLocations -> GetCheck() != 0 ) );
}

/**********************************************************************/

bool CReportEposDlgHelpers::CheckTrainingOption()
{
	if ( SysInfo.IsPMSSystem() == FALSE )
	{
		switch ( GetSelectedReportType() )
		{
		case REPORT_TYPE_REASON_VOID:
		case REPORT_TYPE_REASON_REFUND:
		case REPORT_TYPE_REASON_PROMOTION:
		case REPORT_TYPE_REASON_WASTAGE:
		case REPORT_TYPE_REASON_VIP:		return FALSE;
		default:							return TRUE;
		}
	}
	else
		return TRUE;
}

/**********************************************************************/

int CReportEposDlgHelpers::GetSelectedReportType()
{
	int nSel = m_pListReport -> GetCurSel();

	if ( nSel < 0 || nSel >= m_arrayReportTypesBatch.GetSize() )
		return 0;
	else
		return m_arrayReportTypesBatch.GetAt( nSel );
}

/**********************************************************************/

void CReportEposDlgHelpers::SelectReportType( int nType )
{
	int nSel = 0;
	for ( int n = 0; n < m_arrayReportTypesBatch.GetSize(); n++ )
	{
		if ( m_arrayReportTypesBatch.GetAt(n) == nType )
		{
			nSel = n;
			break;
		}
	}

	m_pListReport -> SetCurSel( nSel );
}

/**********************************************************************/

void CReportEposDlgHelpers::GetReportDates()
{
	m_pDatePickerFrom -> GetTime( m_oleDateFrom );
	m_pDatePickerTo -> GetTime( m_oleDateTo );

	{
		CString strDateRange;
		m_pComboDates -> GetWindowText( strDateRange );
	
		CDateRangeHelpers DateRangeHelpers;
		DateRangeHelpers.GetFutureDateTo( strDateRange, m_oleDateFrom, m_oleDateTo, m_oleFutureDateTo );
	}

	bool bSpecialSession = FALSE;
	switch( m_pComboSession -> GetCount() - m_pComboSession -> GetCurSel() )
	{
	case 2:
		if ( EcrmanOptions.GetReportsEODSessionFlag() == TRUE )
		{
			m_ReportSession.ClearRecord();
			m_ReportSession.SetEODFlag( TRUE );
			bSpecialSession = TRUE;
		}
		break;

	case 1:
		m_ReportSession.ClearRecord();
		m_ReportSession.SetEODFlag( FALSE );
		bSpecialSession = TRUE;
		break;
	}

	if ( FALSE == bSpecialSession )
	{
		int n = m_pComboSession -> GetCurSel();
		if ( ( n >= 0 ) && ( n < DataManagerNonDb.ReportSession.GetSize() ) )
			DataManagerNonDb.ReportSession.GetAt ( n, m_ReportSession );
		else
			m_ReportSession.ClearRecord();

		m_ReportSession.SetEODFlag( FALSE );
	}
}

/**********************************************************************/

bool CReportEposDlgHelpers::GetAdhocReportSession()
{
	CSessionEditDlg dlg( m_ReportSessionAdhoc, TRUE, m_pParentWnd );

	if ( dlg.DoModal() != IDOK )
		return FALSE;
	
	CCSV csv;
	m_ReportSessionAdhoc.ConvertToCSV( csv );
	m_ReportSession.ConvertFromCSV( csv );
	m_ReportSessionAdhoc.SetEODFlag( FALSE );
	m_ReportSession.SetEODFlag( FALSE );
	return TRUE;
}

/**********************************************************************/

void CReportEposDlgHelpers::CreateReportList()
{
	m_pListReport -> ClearList();
	for ( int n = 0; n < m_arrayReportTypesBatch.GetSize(); n++ )
	{
		int nReportType = m_arrayReportTypesBatch.GetAt(n);

		CSSListTaggedSelectItem item;
		item.m_nData = 0;
		item.SetTagState(0);

		if ( nReportType < EPOS_CUSTOM_FIRST_REPORT )
			item.m_strText = CEposReport::GetReportNameExternal ( nReportType );
		else
			item.m_strText = m_EposReportSelect.GetCustomReportName( nReportType );

		m_pListReport -> AddItemToList( item );
	}
}

/**********************************************************************/

void CReportEposDlgHelpers::HandleInitDialog( int nPresetNo, bool bPresetPrint, bool bAutoRunPreset, int& nPresetError )
{
	//BATCH REPORT COMBO
	m_pComboBatch -> ResetContent();
	m_pComboBatch -> AddString ( "None" );
	for ( int nBatchIdx = 0; nBatchIdx < PresetReportManager.EposBatch.GetSize(); nBatchIdx++ )
	{
		CPresetReportInfo BatchRecord;
		PresetReportManager.EposBatch.GetAt( nBatchIdx, BatchRecord );
		m_pComboBatch -> AddString ( BatchRecord.GetName() );
	}
	m_pComboBatch -> SetCurSel(0);
	m_pComboBatch -> EnableWindow ( m_pComboBatch -> GetCount() >= 2 );

	//REPORT LIST
	m_pListReport -> SubclassDlgItem ( IDC_LIST, m_pParentWnd );
	m_pListReport -> SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_pListReport -> InsertColumn ( 0, "", LVCFMT_LEFT, 260);
	m_pListReport -> SetLockItemZeroFlag( FALSE );
	CreateReportList();

	//DATE AND REPORT SESSION SELECTION
	m_pDateSelectorRange -> InitialiseDateControls();
	m_pDateSelectorRange -> AddStandardReportRanges();
	m_pDateSelectorRange -> SetComboSelection( 0, szRANGE_TODAY );

	{
		int nResult = m_pComboDates -> FindStringExact( 0, EcrmanOptions.GetReportsDefaultDateRange() );
			
		if (nResult != CB_ERR)
		{
			m_pComboDates->SetCurSel(nResult);
		}
		else
		{
			m_pComboDates->SetCurSel(0);
		}

		HandleSelectDates();
	}

	for (int nIndex = 0; nIndex < DataManagerNonDb.ReportSession.GetSize(); nIndex++)
	{
		m_pComboSession->AddString(DataManagerNonDb.ReportSession.GetName(nIndex));
	}

	if (EcrmanOptions.GetReportsEODSessionFlag() == TRUE)
	{
		m_pComboSession->AddString("EOD Business Day");
	}

	m_pComboSession -> AddString( "Adhoc Session" );

	if (m_pComboSession->GetCount() < 2)
	{
		m_pComboSession->InsertString(0, "00:00 to 23:59");
	}

	m_pComboSession -> SetCurSel(0);
	m_pComboSession -> EnableWindow( m_pComboSession -> GetCount() > 1 );
	
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
		m_pLocationSelector -> SetSingleDbIdx( PasswordArray.GetLocationAccessDbIdx() );
		break;

	case LOCATION_ACCESS_LOC:
		arrayTypes.Add ( NODE_LOCATION );
		m_pLocationSelector -> SetSingleLocIdx( PasswordArray.GetLocationAccessLocIdx() );
		break;
	}
	
	m_pLocationSelector -> SetDatabaseMode ( NODE_LOCATION );
	m_pLocationSelector -> BuildList( arrayTypes );
	m_pLocationSelector -> FillLocationCombo ( m_pComboLocation, m_pComboTerminal );
	m_pComboLocation -> EnableWindow ( m_pComboLocation -> GetCount() >= 2 );

	{
		CArray<int, int> arrayStops;
		arrayStops.Add(5);
		arrayStops.Add(35);
		m_pComboTermDate->SetTabStops(arrayStops);
	}

	m_pComboTermDate->AddItem(",Date info by terminal");
	m_pComboTermDate->AddItem(",Date info by location");
	m_pComboTermDate->AddItem(",Date info summary");
	m_pComboTermDate->SetCurSel(0);

	//LOCATION CONSOLIDATION
	m_pRadioConsolDefault -> SetCheck( FALSE );
	m_pRadioConsolSaved -> SetCheck( TRUE );
	m_pRadioConsolAdhoc -> SetCheck( FALSE );
	m_pRadioConsolSystem -> SetCheck( FALSE );

	//QUICK REPORTS
	bool bEnableLocationButtons = ( m_pComboLocation -> GetCount() >= 1 );
	m_pCheckReportAllLocations -> EnableWindow ( bEnableLocationButtons );
	m_pCheckReportLocations -> EnableWindow ( bEnableLocationButtons );
	m_pCheckReportEpos -> EnableWindow ( bEnableLocationButtons );
	m_pButtonPrintQuick -> EnableWindow( FALSE );
	
	//SELECTIVE REPORTS DEPARTMENT / PLU
	m_pEditDeptNo -> LimitText( MAX_LENGTH_DEPTNO );
	m_DeptAndPluComboHelpers.UpdateCombos();

	m_pComboPluTime -> AddString( "Transaction completion time" );
	m_pComboPluTime -> AddString( "Action time (where available)" );
	m_pComboPluTime -> SetCurSel( ( EcrmanOptions.GetReportsSalesTimeType() == 1 ) ? 1 : 0 );

	//SELECTIVE REPORTS ACTION
	m_pButtonDisplaySingle -> EnableWindow( FALSE );
	m_pButtonPrintMultiple -> EnableWindow( FALSE );
	
	//ECR LITE RESTRICTIONS
	if ( SysInfo.IsEcrLiteSystem() == TRUE )
	{
		m_pButtonGraphSingle -> EnableWindow( FALSE );
		m_pButtonGraphSingle -> ShowWindow( SW_HIDE );
	}

	//INITIAL REPORT SELECTION
	m_pListReport -> SetCurSel(0);
	HandleSelectReport();
	HandleSelectLocation();

#ifdef COMPILE_PMS

	if ( SysInfo.IsPMSSystem() == TRUE )
	{
		m_pCheckPending -> SetCheck( TRUE );
		m_pCheckActive -> SetCheck( TRUE );
		m_pCheckComplete -> SetCheck( TRUE );
		m_pCheckCancelled -> SetCheck( TRUE );
		HandlePMSRadioConsolidate(0);

		if ( PMSOptions.GetFixedTimeReportEnableFlag() == FALSE )
		{
			m_pRadioPMSConsolidate2 -> SetWindowText( "Report on EPOS sales by originating terminal" );
			m_pRadioPMSConsolidate3 -> EnableWindow( FALSE );
			m_pRadioPMSConsolidate3 -> ShowWindow( SW_HIDE );
		}
		else
		{
			m_pRadioPMSConsolidate2 -> SetWindowText( "Report on completed bookings by fixed time" );
			m_pRadioPMSConsolidate3 -> SetWindowText( "Report on EPOS sales by originating terminal" );
		}
	}

#endif

	PresetReportManager.Epos.SetPasswordAccess();
	FillPresetCombo(-1);

	nPresetError = 0;
	if ( nPresetNo != 0 )
	{
		nPresetError = PresetReportManager.SelectFromCombo( NODE_PRESET_EPOS, *(m_pComboPreset), nPresetNo );

		if ( 0 == nPresetError )
		{
			HandleSelectPreset();

			if ( TRUE == bAutoRunPreset )
			{
				if ( FALSE == bPresetPrint )
					HandleDisplaySingle();
				else
					HandlePrintMultiple();
			}
		}
	}
}

/**********************************************************************/

void CReportEposDlgHelpers::HandleSelectBatch()
{
	int nBatchIdx = m_pComboBatch -> GetCurSel();
	
	if ( ( nBatchIdx >= 0 ) && ( nBatchIdx < m_pComboBatch -> GetCount() ) )
	{
		if ( nBatchIdx == 0 )
		{
			int nReportType = GetSelectedReportType();
			
			CPresetReportInfo BatchRecord;
			SetReportsTypeBatch( BatchRecord, TRUE );
			CreateReportList();
			SelectReportType( nReportType );

			m_pListReport -> SelectNone();
		}
		else
		{
			nBatchIdx--;
	
			if ( ( nBatchIdx >= 0 ) && ( nBatchIdx < PresetReportManager.EposBatch.GetSize() ) )
			{
				CPresetReportInfo BatchRecord;
				PresetReportManager.EposBatch.GetAt ( nBatchIdx, BatchRecord );
				
				CPresetReportInfoEposBatch Preset;
				{
					CString str = BatchRecord.GetOtherFields();
					Preset.FromString( str );
				}
				
				m_pCheckReportEpos -> SetCheck ( Preset.GetReportByTerminalFlag() );
				m_pCheckReportLocations -> SetCheck ( Preset.GetReportByLocationFlag() );
				m_pCheckReportAllLocations -> SetCheck ( Preset.GetReportAllLocationsFlag() );

				if ( Preset.GetActionType() != 0 )
				{
					int nReportType = GetSelectedReportType();
					SetReportsTypeBatch( BatchRecord, FALSE );
					CreateReportList();
					SelectReportType( nReportType );
				}

				if ( Preset.GetActionType() != 1 )
				{
					for ( int n = 0; n < m_pListReport -> GetItemCount(); n++ )
					{
						int nReportType = m_arrayReportTypesBatch.GetAt(n);
						m_pListReport -> SetItemTagState( n, Preset.GetReportFlag( nReportType ) ? 1 : 0 );
					}
				}
			}
		}
	}

	m_pButtonPrintMultiple -> EnableWindow ( CheckPrintableJobs() );
	m_pButtonPrintQuick -> EnableWindow ( CheckPrintableJobs() & CheckQuickPrintOptions() );
}

/**********************************************************************/

void CReportEposDlgHelpers::HandleSelectReport()
{
	m_pButtonDisplaySingle -> EnableWindow ( TRUE );
	m_pButtonGraphSingle -> EnableWindow( CheckGraphableJobs() );
	m_pButtonPrintMultiple -> EnableWindow ( CheckPrintableJobs() );
	m_pButtonPrintQuick -> EnableWindow ( CheckPrintableJobs() && CheckQuickPrintOptions() );
}

/**********************************************************************/

void CReportEposDlgHelpers::HandleSelectDates() 
{
	m_pDateSelectorRange -> UpdateDateControls();	
}

/**********************************************************************/

void CReportEposDlgHelpers::HandleChangeDateFrom() 
{
	m_pDateSelectorRange -> OnChangeFromDate();
	m_pDateSelectorRange -> CheckForChangedDate();
}

/**********************************************************************/

void CReportEposDlgHelpers::HandlePMSRadioConsolidate( int n )
{
#ifdef COMPILE_PMS
	int nMax = ( PMSOptions.GetFixedTimeReportEnableFlag() ? 2 : 1 );

	if ( ( n >= 0 ) && ( n <= nMax ) )
	{
		m_nPMSConsolidationMode = n;
		m_pRadioPMSConsolidate1 -> SetCheck( 0 == m_nPMSConsolidationMode );
		m_pRadioPMSConsolidate2 -> SetCheck( 1 == m_nPMSConsolidationMode );
		m_pRadioPMSConsolidate3 -> SetCheck( 2 == m_nPMSConsolidationMode );
		m_pCheckPending -> EnableWindow( 0 == m_nPMSConsolidationMode );
		m_pCheckActive -> EnableWindow( 0 == m_nPMSConsolidationMode );
		m_pCheckComplete -> EnableWindow( 0 == m_nPMSConsolidationMode );
		m_pCheckCancelled -> EnableWindow( 0 == m_nPMSConsolidationMode );
	}
#endif
}

/**********************************************************************/

void CReportEposDlgHelpers::HandleChangeDateTo() 
{
	m_pDateSelectorRange -> CheckForChangedDate();
}

/**********************************************************************/

void CReportEposDlgHelpers::HandleCheckQuickPrint() 
{
	m_pButtonPrintQuick -> EnableWindow ( CheckPrintableJobs() && CheckQuickPrintOptions() );
}

/**********************************************************************/

void CReportEposDlgHelpers::HandleSelectLocation()
{
	CLocationSelectorEntity LocSelEntity;
	m_pLocationSelector -> GetSelectedEntity( LocSelEntity );	
	m_pLocationSelector -> FillEposCombo();
	m_DeptAndPluComboHelpers.UpdateCombos();
}

/**********************************************************************/

void CReportEposDlgHelpers::HandleKillFocusDeptNo() 
{
	m_pDepartmentSelector -> SelectDepartmentFromEditBox();
}

/**********************************************************************/

void CReportEposDlgHelpers::HandleSelectDeptName() 
{
	m_pDepartmentSelector -> SelectDepartmentFromCombo();
}

/**********************************************************************/

void CReportEposDlgHelpers::HandleDisplaySingle()
{
	CWaitCursor wait;
	CommsHandshaker.ProcessSales();

	HandleKillFocusDeptNo();

	int nReportType = GetSelectedReportType();

	if (nReportType < EPOS_CUSTOM_FIRST_REPORT)
	{
		if ((nReportType < 1) || (nReportType > REPORT_TYPE_COUNT))
		{
			return;
		}
	}

	GetReportDates();

	if (m_pComboSession->GetCurSel() == m_pComboSession->GetCount() - 1)
	{
		if (GetAdhocReportSession() == FALSE)
		{
			return;
		}
	}

	CLocationSelectorEntity LocSelEntity;
	m_pLocationSelector->GetSelectedEntity(LocSelEntity);
	LocSelEntity.GetEPOSSelectArray(m_SelectArray);

	if (REPORT_TYPE_CUSTOMER_BALANCE == nReportType)
	{
		switch (LocSelEntity.GetConType())
		{
		case NODE_LOCATION_TERMINAL:
			Prompter.Error("The Customer Balance report cannot be\ncreated for individual terminals.");
			return;

		case NODE_REPORTPOINT:
			Prompter.Error("The Customer Balance report cannot be\ncreated for report points.");
			return;
		}
	}

	CDataManagerInfo info;
	DataManager.OpenDatabaseReadOnly(LocSelEntity.GetDbIdx(), info, FALSE);

	CString strTitle = LocSelEntity.GetConsolidationTitle();
	CreateReport(nReportType, LocSelEntity.GetConType(), strTitle, FALSE, FALSE);
}

/**********************************************************************/

void CReportEposDlgHelpers::HandlePrintMultiple()
{
	CWaitCursor wait;
	CommsHandshaker.ProcessSales();
	
	HandleKillFocusDeptNo();
	
	GetReportDates();

	if (m_pComboSession->GetCurSel() == m_pComboSession->GetCount() - 1)
	{
		if (GetAdhocReportSession() == FALSE)
		{
			return;
		}
	}
	
	CLocationSelectorEntity LocSelEntity;
	m_pLocationSelector -> GetSelectedEntity( LocSelEntity );
	LocSelEntity.GetEPOSSelectArray ( m_SelectArray );

	CDataManagerInfo info;
	DataManager.OpenDatabaseReadOnly( LocSelEntity.GetDbIdx(), info, FALSE );
	
	bool bOkay = TRUE;
	for ( int n = 0; n < m_pListReport -> GetItemCount(); n++ )
	{
		if ( m_pListReport -> GetItemTagState( n ) == 0 )
			continue;

		CString strTitle = LocSelEntity.GetConsolidationTitle();
		
		int nReportType = m_arrayReportTypesBatch.GetAt(n);

		bool bCanPrint = TRUE;
		if ( REPORT_TYPE_CUSTOMER_BALANCE == nReportType )
		{
			switch( LocSelEntity.GetConType() )
			{
			case NODE_LOCATION_TERMINAL:
			case NODE_REPORTPOINT:
				bCanPrint = FALSE;
				break;
			}
		}
	
		if ( TRUE == bCanPrint )
			bOkay &= CreateReport ( nReportType, LocSelEntity.GetConType(), strTitle, TRUE, FALSE );
	}

	if ( bOkay == FALSE )
		Prompter.Error ( "One or more reports could not be printed." );
}

/**********************************************************************/

void CReportEposDlgHelpers::HandlePrintQuick()
{
	CWaitCursor wait;
	CommsHandshaker.ProcessSales();

	HandleKillFocusDeptNo();

	GetReportDates();

	if (m_pComboSession->GetCurSel() == m_pComboSession->GetCount() - 1)
	{
		if (GetAdhocReportSession() == FALSE)
		{
			return;
		}
	}

	CLocationSelectorEntity LocSelEntity;
	m_pLocationSelector->GetSelectedEntity(LocSelEntity);
	int nConType = LocSelEntity.GetConType();

	int nFirstDbIdx, nLastDbIdx;
	if (NODE_SYSTEM == nConType)
	{
		nFirstDbIdx = 0;
		nLastDbIdx = dbDatabase.GetSize() - 1;
	}
	else
	{
		nFirstDbIdx = LocSelEntity.GetDbIdx();
		nLastDbIdx = nFirstDbIdx;
	}

	CStringArray arrayDatabases;

	{
		CEposSelectArray LocalSelectArray;
		LocSelEntity.GetEPOSSelectArray(LocalSelectArray);

		for (int nDbIdx = nFirstDbIdx; nDbIdx <= nLastDbIdx; nDbIdx++)
		{
			CString strLocations = "";

			CArray<int, int> Set;
			dbLocation.GetDatabaseLocIdxSet(nDbIdx, Set);

			CCSV csvDatabase;
			for (int n = 0; n < Set.GetSize(); n++)
			{
				int nLocIdx = Set.GetAt(n);
				CString strTerminals = LocalSelectArray.GetLocationString(nLocIdx);

				if (strTerminals != "")
				{
					csvDatabase.Add(nLocIdx);
					csvDatabase.Add(strTerminals);
				}
			}

			arrayDatabases.Add(csvDatabase.GetLine());
		}
	}

	bool bTerminal = (m_pCheckReportEpos->GetCheck() != 0);
	bool bLocation = (m_pCheckReportLocations->GetCheck() != 0);
	bool bDatabase = (m_pCheckReportAllLocations->GetCheck() != 0);

	bool bGotError = FALSE;
	for (int n = 0; n < m_pListReport->GetItemCount(); n++)
	{
		if (m_pListReport->GetItemTagState(n) == 0)
		{
			continue;
		}

		int nReportType = m_arrayReportTypesBatch.GetAt(n);

		for (int x = 0; x < arrayDatabases.GetSize(); x++)
		{
			CString strLocations = arrayDatabases.GetAt(x);

			if (strLocations != "")
			{
				int nDbIdx = nFirstDbIdx + x;

				CCSV csvLocations(strLocations);
				int nLocCount = csvLocations.GetSize() / 2;

				for (int y = 0; y < nLocCount * 2; y += 2)
				{
					int nLocIdx = csvLocations.GetInt(y);
					CString strTerminals = csvLocations.GetString(y + 1);

					//REPORTS BY TERMINAL
					if ((TRUE == bTerminal) && (REPORT_TYPE_CUSTOMER_BALANCE != nReportType))
					{
						CArray<int, int> arrayTermIdx;
						for (int z = 0; z < strTerminals.GetLength(); z++)
						{
							if (strTerminals.GetAt(z) == '1')
								arrayTermIdx.Add(z);
						}

						bool bShowTerminals = TRUE;
						if (arrayTermIdx.GetSize() < 2)
						{
							if (TRUE == bLocation)
								bShowTerminals = FALSE;
							else if ((TRUE == bDatabase) && (1 == nLocCount))
								bShowTerminals = FALSE;
						}

						if (TRUE == bShowTerminals)
						{
							for (int t = 0; t < arrayTermIdx.GetSize(); t++)
							{
								int nTermIdx = arrayTermIdx.GetAt(t);

								m_SelectArray.RemoveAll();
								m_SelectArray.SelectTerminal(nLocIdx, nTermIdx);

								CString strTitle;
								strTitle.Format("%s, %s", dbLocation.GetName(nLocIdx), dbLocation.GetTerminalName(nLocIdx, nTermIdx));
								bGotError |= (CreateReport(nReportType, nConType, strTitle, TRUE, TRUE) == FALSE);
							}
						}
					}

					if (TRUE == bLocation)
					{
						if ((FALSE == bDatabase) || (nLocCount >= 2))
						{
							m_SelectArray.RemoveAll();
							m_SelectArray.SetLocationString(nLocIdx, strTerminals);

							CString strTitle;
							strTitle.Format("%s", dbLocation.GetName(nLocIdx));
							bGotError |= (CreateReport(nReportType, nConType, strTitle, TRUE, TRUE) == FALSE);
						}
					}
				}

				if (TRUE == bDatabase)
				{
					m_SelectArray.RemoveAll();

					for (int y = 0; y < nLocCount * 2; y += 2)
					{
						m_SelectArray.SetLocationString(csvLocations.GetInt(y), csvLocations.GetString(y + 1));
					}

					CString strTitle;
					strTitle.Format("%s", dbDatabase.GetName(nDbIdx));
					bGotError |= (CreateReport(nReportType, nConType, strTitle, TRUE, TRUE) == FALSE);
				}
			}
		}
	}

	if (TRUE == bGotError)
	{
		Prompter.Error("One or more reports could not be printed");
	}
}

/**********************************************************************/

bool CReportEposDlgHelpers::CreateReport( int nReportType, int nConLevel, CString& strTitle, bool bPrint, bool bQuick )
{
	CString strSessionTitle = "";

	switch( m_pComboSession -> GetCount() - m_pComboSession -> GetCurSel() )
	{
	case 1:
		strSessionTitle = m_ReportSessionAdhoc.GetName();
		break;

	default:
		m_pComboSession -> GetWindowText( strSessionTitle );
		break;
	}

	CEposReportSelectInfo infoReport;
	infoReport.SetReportType( nReportType );
	bool bResult = m_EposReportSelect.GetReportInfo( infoReport );

	CEposReport* pReport = NULL;
	bool bFixStartDate = FALSE;
	
	if ( TRUE == bResult )
	{
		if (infoReport.LoadConsolPrefs() == TRUE)
		{
			m_EposReportSelect.SetReportInfo(infoReport);
		}

		if ( nReportType >= EPOS_CUSTOM_FIRST_REPORT )
		{
			int nReportFamily = infoReport.GetCustomReportFamily(); 
			switch( nReportFamily )
			{
			case EPOS_CUSTOM_FAMILY_HOURLY:
			case EPOS_CUSTOM_FAMILY_HOURLYGRP:
			case EPOS_CUSTOM_FAMILY_HOURLYDPT:
			case EPOS_CUSTOM_FAMILY_COVERS_HOURLY:
			case EPOS_CUSTOM_FAMILY_COVERS_HOURLYGRP:
			case EPOS_CUSTOM_FAMILY_COVERS_HOURLYDPT:

				if ( ( FALSE == bPrint ) && ( FALSE == bQuick ) )
				{
					CEposReportCustomSettingsHourly Settings;
					CString strParams = infoReport.GetCustomReportParams();
					Settings.SetSettingsCSVLine( strParams );

					if (Settings.GetAllowAdhocFlag() == TRUE)
					{
						CEposReportCustomHourlyDlg dlg(nReportFamily, m_EposReportSelect, infoReport, TRUE, m_pParentWnd);
						if (dlg.DoModal() != IDOK)
						{
							bResult = FALSE;
						}
					}
				}

				if ( TRUE == bResult )
				{
					switch ( nReportFamily )
					{
					case EPOS_CUSTOM_FAMILY_HOURLY:
						pReport = new CEposReportHourly( m_SelectArray, infoReport.GetCustomReportParams() );
						break;

					case EPOS_CUSTOM_FAMILY_HOURLYGRP:
						pReport = new CEposReportHourlyGroup( m_SelectArray, infoReport.GetCustomReportParams(), FALSE );
						break;

					case EPOS_CUSTOM_FAMILY_HOURLYDPT:
						pReport = new CEposReportHourlyGroup( m_SelectArray, infoReport.GetCustomReportParams(), TRUE );
						break;

					case EPOS_CUSTOM_FAMILY_COVERS_HOURLY:
						pReport = new CEposReportCoversHourly( m_SelectArray, infoReport.GetCustomReportParams() );
						break;

					case EPOS_CUSTOM_FAMILY_COVERS_HOURLYGRP:
						pReport = new CEposReportCoversHourlyGroup( m_SelectArray, infoReport.GetCustomReportParams(), FALSE );
						break;

					case EPOS_CUSTOM_FAMILY_COVERS_HOURLYDPT:
						pReport = new CEposReportCoversHourlyGroup( m_SelectArray, infoReport.GetCustomReportParams(), TRUE );
						break;
					}
				}
				break;

			case EPOS_CUSTOM_FAMILY_LOYALTYRECON:	
			{
				if (TRUE == bQuick)
				{
					bResult = FALSE;
				}
				else if (FALSE == bPrint)
				{
					CEposReportCustomSettingsLoyaltyRecon Settings;
					CString strParams = infoReport.GetCustomReportParams();
					Settings.SetSettingsCSVLine(strParams);

					if (Settings.GetAllowAdhocFlag() == TRUE)
					{
						CEposReportCustomLoyaltyReconDlg dlg(m_EposReportSelect, infoReport, TRUE, m_pParentWnd);
						if (dlg.DoModal() != IDOK)
						{
							bResult = FALSE;
						}
					}
				}

				if (TRUE == bResult)
				{
					pReport = new CEposReportLoyaltyRecon(m_SelectArray, infoReport.GetCustomReportParams());
				}
			}
			break;

			case EPOS_CUSTOM_FAMILY_PAYMENTDETAIL:
			{
				if (TRUE == bQuick)
				{
					bResult = FALSE;
				}
				else if (FALSE == bPrint)
				{
					CEposReportCustomSettingsPaymentDetail Settings;
					CString strParams = infoReport.GetCustomReportParams();
					Settings.SetSettingsCSVLine(strParams);

					if (Settings.GetAllowAdhocFlag() == TRUE)
					{
						CEposReportCustomPaymentDetailDlg dlg(m_EposReportSelect, infoReport, TRUE, m_pParentWnd);
						if (dlg.DoModal() != IDOK)
						{
							bResult = FALSE;
						}
					}
				}

				if (TRUE == bResult)
				{
					pReport = new CEposReportPaymentDetail(m_SelectArray, infoReport.GetCustomReportParams());
				}
			}
			break;

			case EPOS_CUSTOM_FAMILY_GENERAL:
				if ( ( FALSE == bPrint ) && ( FALSE == bQuick ) )
				{
					CEposReportCustomSettingsGeneral Settings;
					CString strParams = infoReport.GetCustomReportParams();
					Settings.SetSettingsCSVLine( strParams );

					if ( Settings.GetAllowAdhocFlag() == TRUE )
					{
						CEposReportCustomGeneralDlg dlg( m_EposReportSelect, infoReport, TRUE, m_pParentWnd );
						if (dlg.DoModal() != IDOK)
						{
							bResult = FALSE;
						}
					}
				}

				if (TRUE == bResult)
				{
					pReport = new CEposReportGeneral(m_SelectArray, nReportType, infoReport.GetCustomReportParams());
				}

				break;

			case EPOS_CUSTOM_FAMILY_TRANSACTION:
				if ( ( FALSE == bPrint ) && ( FALSE == bQuick ) )
				{
					CEposReportCustomSettingsTransaction Settings;
					CString strParams = infoReport.GetCustomReportParams();
					Settings.SetSettingsCSVLine( strParams );

					if (Settings.GetAllowAdhocFlag() == TRUE)
					{
						CEposReportCustomTransactionDlg dlg(m_EposReportSelect, infoReport, TRUE, m_pParentWnd);
						if (dlg.DoModal() != IDOK)
						{
							bResult = FALSE;
						}
					}
				}

				if (TRUE == bResult)
				{
					pReport = new CEposReportTransactionSorted(m_SelectArray, nReportType, infoReport.GetCustomReportParams());
				}

				break;

			case EPOS_CUSTOM_FAMILY_PLU_PRICEBAND:
				if ((FALSE == bPrint) && (FALSE == bQuick))
				{
					CEposReportCustomSettingsPluPriceBand Settings;
					CString strParams = infoReport.GetCustomReportParams();
					Settings.SetSettingsCSVLine(strParams);

					if (Settings.GetAllowAdhocFlag() == TRUE)
					{
						CDataManagerInfo info;
						DataManager.OpenDatabaseQuickNoFiles(0, TRUE);
						DataManager.OpenPriceText(DB_READONLY, info);

						CEposReportCustomPluPriceBandDlg dlg(m_EposReportSelect, infoReport, TRUE, m_pParentWnd);
						if (dlg.DoModal() != IDOK)
						{
							bResult = FALSE;
						}

						DataManager.ClosePriceText(info);
					}
				}

				if (TRUE == bResult)
				{
					CString strParams = infoReport.GetCustomReportParams();
					CEposReportCustomSettingsPluPriceBand Settings;
					Settings.SetSettingsCSVLine(strParams);

					if (Settings.GetSortByServerFlag() == FALSE)
					{
						pReport = new CEposReportPluByPriceBandSummary(m_SelectArray, infoReport.GetCustomReportParams());
					}
					else
					{
						pReport = new CEposReportPluByPriceBandServer(m_SelectArray, infoReport.GetCustomReportParams());
					}
				}

				break;

			case EPOS_CUSTOM_FAMILY_DEPOSIT:
				if ( ( FALSE == bPrint ) && ( FALSE == bQuick ) )
				{
					CEposReportCustomSettingsDeposit Settings;
					CString strParams = infoReport.GetCustomReportParams();
					Settings.SetSettingsCSVLine( strParams );

					if ( Settings.GetAllowAdhocFlag() == TRUE )
					{
						CEposReportCustomDepositDlg dlg( m_EposReportSelect, infoReport, TRUE, m_pParentWnd );
						if (dlg.DoModal() != IDOK)
						{
							bResult = FALSE;
						}
					}
				}

				if ( TRUE == bResult )
				{
					CString strParams = infoReport.GetCustomReportParams();

					CEposReportCustomSettingsDeposit Settings;
					Settings.SetSettingsCSVLine( strParams );
					
					switch( Settings.GetReportType() )
					{
					case 1:
						pReport = new CEposReportAccountBookingPurchase( m_SelectArray, strParams );
						bFixStartDate = FALSE;
						break;

					case 2:
						pReport = new CEposReportAccountBookingPurchase( m_SelectArray, strParams );
						bFixStartDate = TRUE;
						break;

					case 3:
						pReport = new CEposReportAccountBookingEvent( m_SelectArray, strParams );
						bFixStartDate = FALSE;
						break;

					case 0:
					default:
						pReport = new CEposReportAccountReceiveDeposit( m_SelectArray, strParams );
						bFixStartDate = FALSE;
						break;
					}
				}
				break;

			case EPOS_CUSTOM_FAMILY_MIXMATCH:
				if ( ( FALSE == bPrint ) && ( FALSE == bQuick ) )
				{
					CEposReportCustomSettingsMixMatch Settings;
					CString strParams = infoReport.GetCustomReportParams();
					Settings.SetSettingsCSVLine( strParams );

					if (Settings.GetAllowAdhocFlag() == TRUE)
					{
						CEposReportCustomMixMatchDlg dlg(m_EposReportSelect, infoReport, TRUE, m_pParentWnd);
						if (dlg.DoModal() != IDOK)
						{
							bResult = FALSE;
						}
					}
				}

				{
					CString strParams = infoReport.GetCustomReportParams();

					CEposReportCustomSettingsMixMatch Settings;
					Settings.SetSettingsCSVLine( strParams );
					
					int nReportType = 0;

					if ( FALSE == Settings.GetCoversFlag() ) 
					{
						switch( Settings.GetReportType() )
						{
						case 1:		
							nReportType = REPORT_TYPE_MIXMATCH_DEPT;
							break;

						case 2:		
							nReportType = REPORT_TYPE_MIXMATCH_ITEM;
							break;

						case 0:		
						default:	
							nReportType = REPORT_TYPE_MIXMATCH_GROUP;	
							break;
						}
					}
					else
					{
						switch( Settings.GetReportType() )
						{
						case 1:		
							nReportType = REPORT_TYPE_MIXMATCH_DEPT_COVERS;	
							break;

						case 2:		
							nReportType = REPORT_TYPE_MIXMATCH_ITEM_COVERS;	
							break;

						case 0:		
						default:	
							nReportType = REPORT_TYPE_MIXMATCH_GROUP_COVERS;
							break;
						}
					}

					pReport = new CEposReportMixMatchItem( m_SelectArray, nReportType, strParams );
				}
				break;

			case EPOS_CUSTOM_FAMILY_DISCOUNT:
				if ( ( FALSE == bPrint ) && ( FALSE == bQuick ) )
				{
					CEposReportCustomSettingsDiscount Settings;
					CString strParams = infoReport.GetCustomReportParams();
					Settings.SetSettingsCSVLine( strParams );

					if ( Settings.GetAllowAdhocFlag() == TRUE )
					{
						CEposReportCustomDiscountDlg dlg( m_EposReportSelect, infoReport, TRUE, m_pParentWnd );
						if (dlg.DoModal() != IDOK)
						{
							bResult = FALSE;
						}
					}
				}
				
				{
					CString strParams = infoReport.GetCustomReportParams();

					CEposReportCustomSettingsDiscount Settings;
					Settings.SetSettingsCSVLine( strParams );
					
					if (Settings.GetByServerFlag() == TRUE)
					{
						pReport = new CEposReportDiscountServer(m_SelectArray, strParams);
					}
					else
					{
						pReport = new CEposReportDiscountSummary(m_SelectArray, strParams);
					}
				}
				bResult = TRUE;
				break;

			default:
				bResult = FALSE;
				break;
			}	
		}
		else
		{
			switch ( nReportType )
			{
			case REPORT_TYPE_PLUSALES:				
			case REPORT_TYPE_PLUSALES_NOZEROSKIP:
			case REPORT_TYPE_PLUWASTE:
			case REPORT_TYPE_VIPSALES:					
				pReport = new CEposReportPluVipSalesSummary( m_SelectArray, nReportType );		
				break;

			case REPORT_TYPE_PLUSALES_TRANSERVER:
			case REPORT_TYPE_PLUWASTE_TRANSERVER:
			case REPORT_TYPE_COVERS_PLU_TRANSERVER:
			case REPORT_TYPE_VIPSALES_TRANSERVER:
				pReport = new CEposReportPluVipSalesServer( m_SelectArray, nReportType );	
				break;

			case REPORT_TYPE_PLUSALES_DEPT_ITEM:		
				pReport = new CEposReportPluVipSalesDept( m_SelectArray, REPORT_TYPE_PLUSALES_DEPT_ITEM );	
				break;

			case REPORT_TYPE_PLUSALES_TAX_ITEM:		
				pReport = new CEposReportPluSalesTax( m_SelectArray );	
				break;

			case REPORT_TYPE_PLUSALES_CUSTOMER:			
				pReport = new CEposReportPluSalesCustomer( m_SelectArray );		
				break;

			case REPORT_TYPE_PLUSALES_MODIFIER:			
				pReport = new CEposReportPluVipSalesModifier( m_SelectArray, REPORT_TYPE_PLUSALES_MODIFIER );		
				break;

			case REPORT_TYPE_PLUSALES_DEPT_SUMMARY:		
				pReport = new CEposReportPluVipDeptSummary( m_SelectArray, REPORT_TYPE_PLUSALES_DEPT_SUMMARY );	
				break;

			case REPORT_TYPE_TAX_SUMMARY_TAX:		
				pReport = new CEposReportTaxSummaryTax( m_SelectArray );	
				break;

			case REPORT_TYPE_TAX_SUMMARY_PRICE:		
				pReport = new CEposReportTaxSummaryPrice( m_SelectArray );	
				break;

			case REPORT_TYPE_VIPSALES_DEPT_ITEM:		
				pReport = new CEposReportPluVipSalesDept( m_SelectArray, REPORT_TYPE_VIPSALES_DEPT_ITEM );			
				break;

			case REPORT_TYPE_VIPSALES_MODIFIER:			
				pReport = new CEposReportPluVipSalesModifier( m_SelectArray, REPORT_TYPE_VIPSALES_MODIFIER );			
				break;

			case REPORT_TYPE_VIPSALES_DEPT_SUMMARY:		
				pReport = new CEposReportPluVipDeptSummary( m_SelectArray, REPORT_TYPE_VIPSALES_DEPT_SUMMARY );			
				break;

			case REPORT_TYPE_PLUWASTE_DEPT_ITEM:		
				pReport = new CEposReportPluVipSalesDept( m_SelectArray, REPORT_TYPE_PLUWASTE_DEPT_ITEM );	
				break;

			case REPORT_TYPE_PLUWASTE_MODIFIER:			
				pReport = new CEposReportPluVipSalesModifier( m_SelectArray, REPORT_TYPE_PLUWASTE_MODIFIER );	
				break;

			case REPORT_TYPE_PLUWASTE_DEPT_SUMMARY:		
				pReport = new CEposReportPluVipDeptSummary( m_SelectArray, REPORT_TYPE_PLUWASTE_DEPT_SUMMARY  );	
				break;

			case REPORT_TYPE_HOURLY:
				pReport = new CEposReportHourly( m_SelectArray );
				break;

			case REPORT_TYPE_HOURLY_GROUP:
				pReport = new CEposReportHourlyGroup( m_SelectArray, FALSE );
				break;

			case REPORT_TYPE_HOURLY_DEPT:
				pReport = new CEposReportHourlyGroup( m_SelectArray, TRUE );
				break;

			case REPORT_TYPE_DAILY:
				pReport = new CEposReportDaily( m_SelectArray );
				break;

			case REPORT_TYPE_DAILY_GROUP:
				pReport = new CEposReportDailyGroup( m_SelectArray, FALSE );
				break;

			case REPORT_TYPE_DAILY_DEPT:
				pReport = new CEposReportDailyGroup( m_SelectArray, TRUE );
				break;

			case REPORT_TYPE_SPTBOOK_CASCADE:					
				pReport = new CEposReportSptBookCascade( m_SelectArray );				
				break;

			case REPORT_TYPE_COVERS_GROUP:				
				pReport = new CEposReportCoversGroup( m_SelectArray, FALSE );			
				break;

			case REPORT_TYPE_COVERS_DEPT:				
				pReport = new CEposReportCoversGroup( m_SelectArray, TRUE );			
				break;

			case REPORT_TYPE_COVERS_HOURLY:			
				pReport = new CEposReportCoversHourly( m_SelectArray );				
				break;

			case REPORT_TYPE_COVERS_HOURLY_GROUP:				
				pReport = new CEposReportCoversHourlyGroup( m_SelectArray, FALSE );				
				break;

			case REPORT_TYPE_COVERS_HOURLY_DEPT:				
				pReport = new CEposReportCoversHourlyGroup( m_SelectArray, TRUE );				
				break;

			case REPORT_TYPE_COVERS_DAILY:			
				pReport = new CEposReportCoversDaily( m_SelectArray );				
				break;

			case REPORT_TYPE_COVERS_DAILY_GROUP:			
				pReport = new CEposReportCoversDailyGroup( m_SelectArray, FALSE );				
				break;

			case REPORT_TYPE_COVERS_DAILY_DEPT:			
				pReport = new CEposReportCoversDailyGroup( m_SelectArray, TRUE );				
				break;

			case REPORT_TYPE_COVERS_MONTHLY:				
				pReport = new CEposReportCoversMonthly( m_SelectArray );				
				break;

			case REPORT_TYPE_COVERS_PRICELEVEL:				
				pReport = new CEposReportCoversPriceLevel( m_SelectArray );			
				break;

			case REPORT_TYPE_COVERS_PAYMENT:				
				pReport = new CEposReportCoversPayment( m_SelectArray );			
				break;

			case REPORT_TYPE_TRANSERVER_SUMMARY:					
				pReport = new CEposReportServer( m_SelectArray );				
				break;

			case REPORT_TYPE_GENERAL_GROUP:				
			case REPORT_TYPE_GENERAL_DEPT:
			case REPORT_TYPE_GENERAL_GROUP_QTY:				
			case REPORT_TYPE_GENERAL_DEPT_QTY:	
			case REPORT_TYPE_WEEKLY_GROUP:				
			case REPORT_TYPE_WEEKLY_DEPT:
			case REPORT_TYPE_WEEKLY_GROUP_QTY:				
			case REPORT_TYPE_WEEKLY_DEPT_QTY:	
				pReport = new CEposReportGeneral( m_SelectArray, nReportType );	
				break;

			case REPORT_TYPE_PAYMENT_SUMMARY:			
				pReport = new CEposReportPaymentSummary( m_SelectArray );	
				break;

			case REPORT_TYPE_PAYMENT_SERVER:			
				pReport = new CEposReportPaymentServer( m_SelectArray );			
				break;

			case REPORT_TYPE_PAYMENT_GROUP_SUMMARY:			
				pReport = new CEposReportPaymentReportGroup( m_SelectArray );			
				break;

			case REPORT_TYPE_PAYMENT_ESTIMATE_GROUP:				
			case REPORT_TYPE_PAYMENT_ESTIMATE_DEPT:				
			case REPORT_TYPE_PAYMENT_ESTIMATE_ITEM:
				pReport = new CEposReportPaymentByItem( m_SelectArray, nReportType ); 
				break;

			case REPORT_TYPE_PAYMENT_ESTIMATE_TAX:
				pReport = new CEposReportPaymentByTaxBand( m_SelectArray ); 
				break;

			case REPORT_TYPE_PAYMENT_DETAIL:				
				pReport = new CEposReportPaymentDetail( m_SelectArray ); 
				break;

			case REPORT_TYPE_DEPOSIT_PURCHASED:			
				pReport = new CEposReportAccountReceiveDeposit( m_SelectArray ); 
				break;

			case REPORT_TYPE_DEPOSIT_BOOKING_PURCHASE:			
				pReport = new CEposReportAccountBookingPurchase( m_SelectArray ); 
				break;

			case REPORT_TYPE_DEPOSIT_BOOKING_EVENT:			
				pReport = new CEposReportAccountBookingEvent( m_SelectArray ); 
				break;

			case REPORT_TYPE_DEPOSIT_REDEEMED:			
				pReport = new CEposReportDepositRedeemed( CASHRSP_ITEMPAYTYPE_DEPOSIT, m_SelectArray );		
				break;

			case REPORT_TYPE_DEPOSIT_UNREDEEMED:		
				pReport = new CEposReportDepositUnredeemed( m_SelectArray );	
				break;

			case REPORT_TYPE_CUSTOMER_RA:				
				pReport = new CEposReportAccountReceiveCustomer( m_SelectArray );		
				break;

			case REPORT_TYPE_CUSTOMER_SPEND:			
				pReport = new CEposReportDepositRedeemed( CASHRSP_ITEMPAYTYPE_CUSTOMER, m_SelectArray );		
				break;

			case REPORT_TYPE_CUSTOMER_ACTIVITY:			
				pReport = new CEposReportCustomerActivity( m_SelectArray );		
				break;

			case REPORT_TYPE_CUSTOMER_BALANCE:			
				pReport = new CEposReportCustomerBalance( m_SelectArray );		
				break;

			case REPORT_TYPE_PROMOTION_SUMMARY:				
				pReport = new CEposReportPromotionSummary( m_SelectArray );			
				break;

			case REPORT_TYPE_MIXMATCH_GROUP:
			case REPORT_TYPE_MIXMATCH_DEPT:
			case REPORT_TYPE_MIXMATCH_ITEM:
			case REPORT_TYPE_MIXMATCH_GROUP_COVERS:
			case REPORT_TYPE_MIXMATCH_DEPT_COVERS:
			case REPORT_TYPE_MIXMATCH_ITEM_COVERS:
				pReport = new CEposReportMixMatchItem( m_SelectArray, nReportType );			
				break;

			case REPORT_TYPE_CHEAPESTONEFREE_SUMMARY:				
				pReport = new CEposReportCheapestOneFreeSummary( m_SelectArray );			
				break;

			case REPORT_TYPE_CHEAPESTONEFREE_DETAIL:				
				pReport = new CEposReportCheapestOneFreeDetail( m_SelectArray );			
				break;
			
			case REPORT_TYPE_TRANSERVER_VOID:						
				pReport = new CEposReportVoid( m_SelectArray );					
				break;

			case REPORT_TYPE_TRANSERVER_VOID_REASON:						
				pReport = new CEposReportVoidReason( m_SelectArray );					
				break;

			case REPORT_TYPE_TRANSERVER_REFUND:					
				pReport = new CEposReportRefund( m_SelectArray );				
				break;

			case REPORT_TYPE_TRANSERVER_REFUND_REASON:
			case REPORT_TYPE_TRANSERVER_VIP_REASON:
			case REPORT_TYPE_TRANSERVER_WASTAGE_REASON:		
				pReport = new CEposReportMiscReason( nReportType, m_SelectArray );				
				break;

			case REPORT_TYPE_TRANSERVER_DISCOUNT:					
				pReport = new CEposReportDiscountServer( m_SelectArray );				
				break;
		
			case REPORT_TYPE_TRANSERVER_DISCOUNT_REASON:					
				pReport = new CEposReportDiscountReason( m_SelectArray );				
				break;

			case REPORT_TYPE_NOSALE:					
				pReport = new CEposReportNoSale( m_SelectArray );				
				break;

			case REPORT_TYPE_AGEVERIFY:					
				pReport = new CEposReportAgeVerify( m_SelectArray );			
				break;

			case REPORT_TYPE_EODSTATUS:					
				pReport = new CEposReportEODStatus( m_SelectArray );				
				break;

			case REPORT_TYPE_TRANSACTION_NORMAL:
			case REPORT_TYPE_TRANSACTION_DISCREPANCY:	
				pReport = new CEposReportTransactionSorted( m_SelectArray, nReportType );	
				break;

			case REPORT_TYPE_REASON_VOID:				
			case REPORT_TYPE_REASON_REFUND:	
			case REPORT_TYPE_REASON_PROMOTION:
			case REPORT_TYPE_REASON_WASTAGE:
			case REPORT_TYPE_REASON_VIP:				
				pReport = new CEposReportReason( m_SelectArray, nReportType );	
				break;

			default: 
				bResult = FALSE;
				break;
			}
		}
	}
	
	if ( TRUE == bResult )
	{
		int nConsolType = EPOSREPORT_CONSOL_DEFAULT;

		if (m_pRadioConsolSaved->GetCheck() != 0)
		{
			nConsolType = EPOSREPORT_CONSOL_SAVED;
		}
		else if (m_pRadioConsolAdhoc->GetCheck() != 0)
		{
			nConsolType = EPOSREPORT_CONSOL_ADHOC;
		}
		else if (m_pRadioConsolSystem->GetCheck() != 0)
		{
			nConsolType = EPOSREPORT_CONSOL_SYSTEM;
		}

		if ( EPOSREPORT_CONSOL_ADHOC == nConsolType )
		{
			if (TRUE == bPrint)
			{
				nConsolType = EPOSREPORT_CONSOL_SAVED;
			}
			else
			{
				if ( CReportEposConsolDlg::CanDoAdhocReport( infoReport ) == TRUE )
				{
					CReportEposConsolDlg dlg( infoReport, m_pParentWnd );
					bResult = ( dlg.DoModal() == IDOK );
					m_EposReportSelect.SetReportInfo( infoReport );
				}
			}
		}

		pReport -> SetPreferConsolType( nConsolType );	
	}

	if ( TRUE == bResult )
	{
		pReport -> SetEposReportSelectInfo( infoReport );
		bResult = CreateReportInternal ( pReport, nConLevel, strTitle, strSessionTitle, bFixStartDate, bPrint, bQuick );
	}

	if (pReport != NULL)
	{
		delete pReport;
	}
	
	return bResult;
}

/**********************************************************************/

bool CReportEposDlgHelpers::CreateReportInternal(CEposReport* pReport, int nConLevel, CString& strTitle, CString& strSessionTitle, bool bFixStartDate, bool bPrint, bool bQuick)
{
	COleDateTime dateTo = m_oleDateTo;
	COleDateTime dateFrom = m_oleDateFrom;

	if (TRUE == bFixStartDate)
	{
		COleDateTime timeNow = COleDateTime::GetCurrentTime();
		dateFrom = COleDateTime(2016, 1, 1, 0, 0, 0);
		dateTo = COleDateTime(timeNow.GetYear(), timeNow.GetMonth(), timeNow.GetDay(), 0, 0, 0);
	}
	else if (pReport->GetReportType() == REPORT_TYPE_DEPOSIT_BOOKING_EVENT)
	{
		dateTo = m_oleFutureDateTo;
	}

	if ((pReport->GetReportType() == REPORT_TYPE_EODSTATUS) && (m_ReportSession.GetEODFlag() == TRUE))
	{
		CSessionCSVRecord DefaultSession;
		pReport->SetDates(dateFrom, dateTo, DefaultSession);
		pReport->SetSessionTitle("00:00 to 23:59");
	}
	else
	{
		pReport->SetDates(dateFrom, dateTo, m_ReportSession);
		pReport->SetSessionTitle(strSessionTitle);
	}

	pReport->SetConLevelTitle(strTitle);
	pReport->SetItemServerFlag(m_pCheckItemServer->GetCheck() != 0);
	pReport->SetConLevel(nConLevel);

	if (SysInfo.IsPMSSystem() == FALSE)
	{
		pReport->SetPMSPendingFlag(FALSE);
		pReport->SetPMSActiveFlag(FALSE);
		pReport->SetPMSCompleteFlag(FALSE);
		pReport->SetPMSCancelledFlag(FALSE);
		pReport->SetPMSEposFlag(TRUE);
		pReport->SetPMSDateMode(PMS_DATEMODE_ACTUAL);
	}

#ifdef COMPILE_PMS

	if (SysInfo.IsPMSSystem() == TRUE)
	{
		pReport->SetPMSPendingFlag(m_pCheckPending->GetCheck() != 0);
		pReport->SetPMSActiveFlag(m_pCheckActive->GetCheck() != 0);
		pReport->SetPMSCompleteFlag(m_pCheckComplete->GetCheck() != 0);
		pReport->SetPMSCancelledFlag(m_pCheckCancelled->GetCheck() != 0);

		if (PMSOptions.GetFixedTimeReportEnableFlag() == TRUE)
		{
			switch (m_nPMSConsolidationMode)
			{
			case 1:
				pReport->SetPMSEposFlag(FALSE);
				pReport->SetPMSDateMode(1);
				pReport->SetPMSPendingFlag(FALSE);
				pReport->SetPMSActiveFlag(FALSE);
				pReport->SetPMSCompleteFlag(TRUE);
				pReport->SetPMSCancelledFlag(FALSE);
				break;

			case 2:
				pReport->SetPMSEposFlag(TRUE);
				pReport->SetPMSDateMode(0);
				break;

			case 0:
			default:
				pReport->SetPMSEposFlag(FALSE);
				pReport->SetPMSDateMode(0);
				break;
			}
		}
		else
		{
			pReport->SetPMSEposFlag(1 == m_nPMSConsolidationMode);
			pReport->SetPMSDateMode(0);
		}
	}

#endif

	{
		CLocationSelectorEntity LocSelEntity;
		m_pLocationSelector->GetSelectedEntity(LocSelEntity);
		int nConType = LocSelEntity.GetConType();

		int nPluFilterNo = m_DeptAndPluComboHelpers.GetPluFilterNo();
		pReport->m_ReportFilters.PluFilter.SetConsolidationType(nConType);
		pReport->m_ReportFilters.PluFilter.SetMasterFilterNo(nPluFilterNo);

		int nDeptFilterNo = m_pDepartmentSelector->GetDepartmentFilter();
		pReport->m_ReportFilters.DeptFilter.SetConsolidationType(nConType);
		pReport->m_ReportFilters.DeptFilter.SetDeptFilterNo(nDeptFilterNo);
	}

	pReport->SetSaleTimeFlag(m_pComboPluTime->GetCurSel() == 1);
	pReport->SetTermDateType(m_pComboTermDate->GetCurSel());

	if (pReport->GetReportType() == REPORT_TYPE_LOYALTYRECON)
	{
		if (pReport->SpecialPrepare() == FALSE)
		{
			return FALSE;
		}
	}

	pReport->Consolidate();

	bool bResult = pReport->CreateReport();

	if (pReport->GetCashRSPVersionCheckerFailFlag() == TRUE)
	{
		if (FALSE == bPrint)
		{
			pReport->ShowCashRSPVersionCheckerFailMessage();
		}

		return FALSE;
	}

	if (FALSE == bResult)
	{
		if (FALSE == bPrint)
		{
			Prompter.Error("Unable to create report.");
		}

		return FALSE;
	}

	CReportManager ReportManager;

	if (bPrint == TRUE)
	{
		return ReportManager.PrintSilent(Super.ReportFilename(), pReport->GetParamsFilename(), "", m_pParentWnd);
	}
	else
	{
		return ReportManager.DisplayReport(Super.ReportFilename(), pReport->GetParamsFilename(), "", m_pParentWnd);
	}
}

/**********************************************************************/

void CReportEposDlgHelpers::HandleGraphSingle()
{
	if ( CheckGraphableJobs() == FALSE )
		return;

	CWaitCursor wait;
	CommsHandshaker.ProcessSales();
	
	HandleKillFocusDeptNo();
	
	int nReportType = GetSelectedReportType();
	
	GetReportDates();

	if ( m_pComboSession -> GetCurSel() == m_pComboSession -> GetCount() - 1 )
		if ( GetAdhocReportSession() == FALSE )
			return;
	
	CLocationSelectorEntity LocSelEntity;
	m_pLocationSelector -> GetSelectedEntity( LocSelEntity );
	LocSelEntity.GetEPOSSelectArray ( m_SelectArray );
	
	CDataManagerInfo info;
	DataManager.OpenDatabaseReadOnly( LocSelEntity.GetDbIdx(), info, FALSE );
			
	CString strTitle = LocSelEntity.GetConsolidationTitle();
	
	CreateGraph ( nReportType, LocSelEntity.GetConType(), strTitle );
}

/**********************************************************************/

void CReportEposDlgHelpers::CreateGraph( int nReportType, int nConLevel, CString& strTitle )
{
	CString strSessionTitle;

	switch( m_pComboSession -> GetCount() - m_pComboSession -> GetCurSel() )
	{
	case 1:
		strSessionTitle = m_ReportSessionAdhoc.GetName();
		break;

	default:
		m_pComboSession -> GetWindowText( strSessionTitle );
		break;
	}

	CEposReportSelectInfo infoReport;
	infoReport.SetReportType( nReportType );
	if ( m_EposReportSelect.GetReportInfo( infoReport ) == TRUE )
	{
		CEposReport* pReport = NULL;

		switch ( nReportType )
		{
		case REPORT_TYPE_PLUSALES_DEPT_SUMMARY:		
			pReport = new CEposReportPluVipDeptSummary( m_SelectArray, REPORT_TYPE_PLUSALES_DEPT_SUMMARY );
			break;
		
		case REPORT_TYPE_PLUWASTE_DEPT_SUMMARY:		
			pReport = new CEposReportPluVipDeptSummary( m_SelectArray, REPORT_TYPE_PLUWASTE_DEPT_SUMMARY );	
			break;

		case REPORT_TYPE_VIPSALES_DEPT_SUMMARY:		
			pReport = new CEposReportPluVipDeptSummary( m_SelectArray, REPORT_TYPE_VIPSALES_DEPT_SUMMARY );		
			break;

		case REPORT_TYPE_TAX_SUMMARY_TAX:			pReport = new CEposReportTaxSummaryTax( m_SelectArray );		break;
		case REPORT_TYPE_TAX_SUMMARY_PRICE:			pReport = new CEposReportTaxSummaryPrice( m_SelectArray );		break;
		case REPORT_TYPE_HOURLY:					pReport = new CEposReportHourly( m_SelectArray );			break;
		case REPORT_TYPE_COVERS_HOURLY:				pReport = new CEposReportCoversHourly( m_SelectArray );			break;
		case REPORT_TYPE_COVERS_MONTHLY:			pReport = new CEposReportCoversMonthly( m_SelectArray );		break;
		case REPORT_TYPE_TRANSERVER_SUMMARY:		pReport = new CEposReportServer( m_SelectArray );				break;
		case REPORT_TYPE_PAYMENT_SUMMARY:			pReport = new CEposReportPaymentSummary( m_SelectArray );		break;
		}

		if ( ( nReportType >= EPOS_CUSTOM_FIRST_REPORT ) && ( NULL == pReport ) )
		{
			CEposReportSelectInfo infoReport;
			infoReport.SetReportType( nReportType );
			m_EposReportSelect.GetReportInfo( infoReport );
			
			int nReportFamily = infoReport.GetCustomReportFamily(); 
			switch( nReportFamily )
			{
			case EPOS_CUSTOM_FAMILY_HOURLY:
			case EPOS_CUSTOM_FAMILY_HOURLYGRP:
			case EPOS_CUSTOM_FAMILY_HOURLYDPT:
			case EPOS_CUSTOM_FAMILY_COVERS_HOURLY:
				{
					bool bContinue = TRUE;

					CEposReportCustomSettingsHourly Settings;
					CString strParams = infoReport.GetCustomReportParams();
					Settings.SetSettingsCSVLine( strParams );

					if ( Settings.GetAllowAdhocFlag() == TRUE )
					{
						CEposReportCustomHourlyDlg dlg( nReportFamily, m_EposReportSelect, infoReport, TRUE, m_pParentWnd );
						if ( dlg.DoModal() != IDOK )
							bContinue = FALSE;
					}

					if ( TRUE == bContinue )
					{
						switch( nReportFamily )
						{
						case EPOS_CUSTOM_FAMILY_HOURLY:
							pReport = new CEposReportHourly( m_SelectArray, infoReport.GetCustomReportParams() );
							break;

						case EPOS_CUSTOM_FAMILY_HOURLYGRP:
							pReport = new CEposReportHourlyGroup( m_SelectArray, infoReport.GetCustomReportParams(), FALSE );
							break;

						case EPOS_CUSTOM_FAMILY_HOURLYDPT:
							pReport = new CEposReportHourlyGroup( m_SelectArray, infoReport.GetCustomReportParams(), TRUE );
							break;

						case EPOS_CUSTOM_FAMILY_COVERS_HOURLY:
							pReport = new CEposReportCoversHourly( m_SelectArray, infoReport.GetCustomReportParams() );
							break;
						}
					}
				}
				break;
			}
		}
		
		if ( NULL != pReport )
		{
			pReport -> SetEposReportSelectInfo( infoReport );
			CreateGraphInternal( pReport, nConLevel, strTitle, strSessionTitle );
			delete pReport;
		}
	}
}

/**********************************************************************/

void CReportEposDlgHelpers::CreateGraphInternal( CEposReport* pReport, int nConLevel, CString& strTitle, CString& strSessionTitle )
{
	pReport -> SetDates ( m_oleDateFrom, m_oleDateTo, m_ReportSession );
	pReport -> SetConLevelTitle ( strTitle );
	pReport -> SetSessionTitle ( strSessionTitle );

	{
		CLocationSelectorEntity LocSelEntity;
		m_pLocationSelector -> GetSelectedEntity( LocSelEntity );
		int nConType = LocSelEntity.GetConType();
		
		int nPluFilterNo = m_DeptAndPluComboHelpers.GetPluFilterNo();
		pReport -> m_ReportFilters.PluFilter.SetConsolidationType(nConType );
		pReport -> m_ReportFilters.PluFilter.SetMasterFilterNo( nPluFilterNo );

		int nDeptFilterNo = m_pDepartmentSelector -> GetDepartmentFilter();
		pReport -> m_ReportFilters.DeptFilter.SetConsolidationType( nConType );
		pReport -> m_ReportFilters.DeptFilter.SetDeptFilterNo( nDeptFilterNo );
	}
	
	pReport -> SetSaleTimeFlag( m_pComboPluTime -> GetCurSel() == 1 );
	pReport -> SetConLevel( nConLevel );
	pReport -> SetPreferConsolType( EPOSREPORT_CONSOL_SYSTEM );


	if ( SysInfo.IsPMSSystem() == FALSE )
	{
		pReport -> SetPMSPendingFlag( FALSE );
		pReport -> SetPMSActiveFlag( FALSE );
		pReport -> SetPMSCompleteFlag( FALSE );
		pReport -> SetPMSCancelledFlag( FALSE );
		pReport -> SetPMSEposFlag( TRUE );
		pReport -> SetPMSDateMode( PMS_DATEMODE_ACTUAL );
	}

#ifdef COMPILE_PMS

	if ( SysInfo.IsPMSSystem() == TRUE )
	{
		pReport -> SetPMSPendingFlag( m_pCheckPending -> GetCheck() != 0 );
		pReport -> SetPMSActiveFlag( m_pCheckActive -> GetCheck() != 0 );
		pReport -> SetPMSCompleteFlag( m_pCheckComplete -> GetCheck() != 0 );
		pReport -> SetPMSCancelledFlag( m_pCheckCancelled -> GetCheck() != 0 );
		
		if ( PMSOptions.GetFixedTimeReportEnableFlag() == TRUE )
		{
			switch( m_nPMSConsolidationMode )
			{
			case 1:
				pReport -> SetPMSEposFlag( FALSE );
				pReport -> SetPMSDateMode( 1 );
				pReport -> SetPMSPendingFlag( FALSE );
				pReport -> SetPMSActiveFlag( FALSE );
				pReport -> SetPMSCompleteFlag( TRUE );
				pReport -> SetPMSCancelledFlag( FALSE );
				break;

			case 2:
				pReport -> SetPMSEposFlag( TRUE );
				pReport -> SetPMSDateMode( 0 );
				break;

			case 0:
			default:
				pReport -> SetPMSEposFlag( FALSE );
				pReport -> SetPMSDateMode( 0 );
				break;
			}
		}
		else
		{
			pReport -> SetPMSEposFlag( 1 == m_nPMSConsolidationMode );
			pReport -> SetPMSDateMode( 0 );
		}
	}

#endif

	if ( pReport -> CheckGraph() == TRUE )
	{
		pReport -> SetGraphModeFlag( TRUE );
		pReport -> Consolidate();

		if ( pReport -> GetCashRSPVersionCheckerFailFlag() == TRUE )
			pReport -> ShowCashRSPVersionCheckerFailMessage();
		else if ( pReport -> CreateGraph() )
		{
			CReportManager ReportManager;
			ReportManager.DisplayGraph( Super.ReportFilename() );
		}
		else
			Prompter.Error ( "Unable to create graph" );
	}
}

/**********************************************************************/

void CReportEposDlgHelpers::HandleButtonPreset()
{
	CPresetReportInfo Preset;
	CreatePreset( Preset );
	PresetReportManager.PresetButton( NODE_PRESET_EPOS, *(m_pComboPreset ), Preset, m_pParentWnd );
}

/**********************************************************************/

void CReportEposDlgHelpers::HandleSelectPreset()
{
	int nSel = m_pComboPreset -> GetCurSel() - 1;
	if ( ( nSel < 0 ) || ( nSel >= PresetReportManager.Epos.GetSize() ) )
		return;

	CPresetReportInfo Base;
	PresetReportManager.Epos.GetAt( nSel, Base );

	CPresetReportInfoEpos Preset;
	{
		CString str = Base.GetOtherFields();
		Preset.FromString( str );
	}

	{
		int nSel = 0;
		int nBatchNoPreset = Preset.GetBatchNo();

		if ( nBatchNoPreset != 0 )
		{
			for ( int nBatchIdx = 0; nBatchIdx < PresetReportManager.EposBatch.GetSize(); nBatchIdx++ )
			{
				CString strDummy;
				int nBatchNoArray;
				PresetReportManager.EposBatch.GetEntityNoAndListName( nBatchIdx, nBatchNoArray, strDummy );

				if ( nBatchNoArray == nBatchNoPreset )
				{
					nSel = nBatchIdx + 1;
					break;
				}
			}
		}

		m_pComboBatch -> SetCurSel( nSel );
		HandleSelectBatch();
	}

	{
		int nSel = 0;
		int nPresetReportType = Preset.GetReportType();
			
		for ( int n = 1; n < m_arrayReportTypesBatch.GetSize(); n++ )
		{
			if ( m_arrayReportTypesBatch.GetAt(n) == nPresetReportType )
			{
				nSel = n;
				break;
			}
		}

		m_pListReport -> SetCurSel( nSel );
		HandleSelectReport();
	}

	{
		int nResult = m_pComboDates -> FindStringExact( 0, Preset.GetDateType() );
		
		if ( nResult != CB_ERR )
			m_pComboDates -> SetCurSel( nResult );
	}

	{
		COleDateTime dateFrom;
		CString strDateFrom = Preset.GetDateFrom();
		if ( ::GetOleDateFromString( strDateFrom, dateFrom ) == TRUE )
			m_pDatePickerFrom -> SetTime( dateFrom );
	}

	{
		COleDateTime dateTo;
		CString strDateTo = Preset.GetDateTo();
		if ( ::GetOleDateFromString( strDateTo, dateTo ) == TRUE )
			m_pDatePickerTo -> SetTime( dateTo );
	}

	HandleSelectDates();

	{
		int nResult = m_pComboSession -> FindStringExact( 0, Preset.GetSessionName() );
		
		if ( nResult != CB_ERR )
			m_pComboSession -> SetCurSel( nResult );
	}

	m_pComboPluTime -> SetCurSel( Preset.GetPluTimeType() == 1 ? 1 : 0 );

#ifdef COMPILE_PMS

	if ( SysInfo.IsPMSSystem() == TRUE )
	{
		int nType = 0;
		
		if ( PMSOptions.GetFixedTimeReportEnableFlag() == FALSE )
			nType = ( Preset.GetPMSConsolidateType() == 2 ) ? 1 : 0; 
		else
			nType = Preset.GetPMSConsolidateType();
		
		if ( ( nType < 0 ) || ( nType > 2 ) )
			nType = 0;

		HandlePMSRadioConsolidate( nType );

		m_pCheckPending -> SetCheck( Preset.GetPMSPendingFlag() );
		m_pCheckActive -> SetCheck( Preset.GetPMSActiveFlag() );
		m_pCheckComplete -> SetCheck( Preset.GetPMSCompleteFlag() );
		m_pCheckCancelled -> SetCheck( Preset.GetPMSCancelledFlag() );
	}

#endif

	{
		int nType = Preset.GetBlockType();
		if ( ( nType < 0 ) || ( nType > 3 ) )
			nType = 0;

		m_pRadioConsolDefault -> SetCheck( 0 == nType );
		m_pRadioConsolSaved -> SetCheck( 1 == nType );
		m_pRadioConsolAdhoc -> SetCheck( 2 == nType );
		m_pRadioConsolSystem -> SetCheck( 3 == nType );
	}
	
	{
		int nConLevel = Base.GetPasswordConLevel();
		int nEntity1 = Base.GetPasswordEntityNo1();
		int nEntity2 = Base.GetPasswordEntityNo2();
		int nTerminalIdx = Base.GetTerminalIdx();

		int nEntityIdx = 0;
		CPresetReportArray::GetEntityIdx( nConLevel, nEntity1, nEntity2, nEntityIdx );

		if ( nConLevel == NODE_LOCATION_TERMINAL )
		{
			if ( ( nTerminalIdx >= 0 ) && ( nTerminalIdx < m_pComboTerminal -> GetCount() ) )
				m_pComboTerminal -> SetCurSel(nTerminalIdx);
		}
		
		m_pLocationSelector -> ForceSelection( nConLevel, nEntityIdx );
	}

	{
		int nType = Preset.GetTermDateType();
		if ((nType < 0) || (nType > 2))
		{
			nType = 0;
		}
		m_pComboTermDate->SetCurSel(nType);
	}

	m_pDepartmentSelector -> UpdateDepartmentCombo( Preset.GetDepartmentFilter() );
	m_pDepartmentSelector -> UpdateDepartmentEditBox();
	m_DeptAndPluComboHelpers.SetPluFilterNo( Preset.GetPluFilterNo() );
	m_pCheckItemServer -> SetCheck( Preset.GetItemServerFlag() );
}

/**********************************************************************/

void CReportEposDlgHelpers::CreatePreset( CPresetReportInfo& Base )
{
	CPresetReportInfoEpos Preset;

	{
		int nBatchNo = 0;
		
		int nSel = m_pComboBatch -> GetCurSel();
		if ( ( nSel >= 1 ) && ( nSel <= PresetReportManager.EposBatch.GetSize() ) )
		{
			CString strDummy;
			PresetReportManager.EposBatch.GetEntityNoAndListName( nSel - 1, nBatchNo, strDummy );
		}

		Preset.SetBatchNo( nBatchNo );
	}

	Preset.SetReportType( GetSelectedReportType() );

	{
		CString strDateType;
		m_pComboDates -> GetWindowText( strDateType );
		Preset.SetDateType( strDateType );
	}

	{
		CString strDateFrom;
		COleDateTime dateFrom;
		m_pDatePickerFrom -> GetTime( dateFrom );
		::GetStringFromOleDate( dateFrom, strDateFrom );
		Preset.SetDateFrom( strDateFrom );
	}

	{
		CString strDateTo;
		COleDateTime dateTo;
		m_pDatePickerTo -> GetTime( dateTo );
		::GetStringFromOleDate( dateTo, strDateTo );
		Preset.SetDateTo( strDateTo );
	}

	{
		CString strSession;
		m_pComboSession -> GetWindowText( strSession );
		Preset.SetSessionName( strSession );
	}

	Preset.SetPluTimeType( m_pComboPluTime -> GetCurSel() );

#ifdef COMPILE_PMS

	if ( SysInfo.IsPMSSystem() == TRUE )
	{
		{
			int nType = 0;

			if ( PMSOptions.GetFixedTimeReportEnableFlag() == FALSE )
			{
				if ( m_pRadioPMSConsolidate2 -> GetCheck() != 0 ) nType = 2;
			}
			else
			{
				if ( m_pRadioPMSConsolidate2 -> GetCheck() != 0 ) nType = 1;
				if ( m_pRadioPMSConsolidate3 -> GetCheck() != 0 ) nType = 2;
			}

			Preset.SetPMSConsolidateType( nType );
		}

		Preset.SetPMSPendingFlag( m_pCheckPending -> GetCheck() != 0 );
		Preset.SetPMSActiveFlag( m_pCheckActive -> GetCheck() != 0 );
		Preset.SetPMSCompleteFlag( m_pCheckComplete -> GetCheck() != 0 );
		Preset.SetPMSCancelledFlag( m_pCheckCancelled -> GetCheck() != 0 );
	}
	else
#endif
	{
		Preset.SetPMSConsolidateType(0);
		Preset.SetPMSPendingFlag( FALSE );
		Preset.SetPMSActiveFlag( FALSE );
		Preset.SetPMSCompleteFlag( FALSE );
		Preset.SetPMSCancelledFlag( FALSE );
	}

	{
		CLocationSelectorEntity LocSelEntity;
		m_pLocationSelector -> GetSelectedEntity( LocSelEntity );
		Base.SetPresetConLevel( LocSelEntity.GetConType() );

		int nEntityNo1, nEntityNo2;
		CPresetReportArray::GetLocSelEntityNo( LocSelEntity, nEntityNo1, nEntityNo2 );
		Base.SetPresetEntityNo1( nEntityNo1 );
		Base.SetPresetEntityNo2( nEntityNo2 );
		Base.SetTerminalIdx( LocSelEntity.GetTermIdx() );

		Preset.SetTermDateType(m_pComboTermDate -> GetCurSel());
	}

	{
		int nType = 0;
		if ( m_pRadioConsolSaved -> GetCheck() != 0 ) nType = 1;
		if ( m_pRadioConsolAdhoc -> GetCheck() != 0 ) nType = 2;
		if ( m_pRadioConsolSystem -> GetCheck() != 0 ) nType = 3;
		Preset.SetBlockType( nType );
	}

	Preset.SetDepartmentFilter( m_pDepartmentSelector -> GetDepartmentFilter() );
	Preset.SetPluFilterNo( m_DeptAndPluComboHelpers.GetPluFilterNo() );
	Preset.SetItemServerFlag( m_pCheckItemServer -> GetCheck() != 0 );

	CString str = "";
	Preset.ToString( str );
	Base.SetOtherFields( str );
}

/**********************************************************************/

void CReportEposDlgHelpers::FillPresetCombo( int nPresetIdxSel )
{
	PresetReportManager.FillPresetCombo( NODE_PRESET_EPOS, *(m_pComboPreset), nPresetIdxSel );
}

/**********************************************************************/
