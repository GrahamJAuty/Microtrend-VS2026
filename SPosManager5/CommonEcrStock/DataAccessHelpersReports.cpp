/**********************************************************************/
#include "AutoExpireHistory.h"
#include "ButtonActionHandler.h"
#include "LocationCSVArray.h"
#include "Password.h"
#include "PMSCalendarHelpers.h"
#include "PMSCheckoutDateArray.h"
#include "PresetReportManager.h"
#include "..\SPosEcrManager5\ReportEcrmanStockDlg.h"
#include "ReportEposDlg.h"
#include "ReportEposPMSDlg.h"
#include "ReportTimeAndAttendanceDlg.h"
#include "SalesHistoryReportCustomerDlg.h"
#include "SalesHistoryReportPluDlg.h"
#include "..\SPosStockManager5\SalesHistoryReportStockNewDlg.h"
#include "SysInfo.h"
/**********************************************************************/
#include "DataAccessHelpersReports.h"
/**********************************************************************/

CDataAccessHelpersReports::CDataAccessHelpersReports()
{
}

/**********************************************************************/

void CDataAccessHelpersReports::SalesHistoryPlu( int nPresetNo, bool bAutoRunPreset )
{
	AutoExpireHistory.ExpireReportsNow();

	if ( dbLocation.GetSize() == 0 )
		ShowLocationError();
	else if ( PasswordArray.CheckLocationAccessLoc() == FALSE )
		Prompter.LocationAccessError();
	else
	{
		CDataManagerInfo info;
		DataManagerNonDb.OpenReportSession( DB_READONLY, info );
		DataManagerNonDb.OpenTimeSliceList( DB_READONLY, info );
		DataManagerNonDb.OpenSalesHistoryPlu( DB_READONLY, info );
		DataManagerNonDb.OpenSystemPluFilterNames( DB_READONLY, info );
		DataManagerNonDb.OpenSystemPluFilterItems( DB_READONLY, info );
		DataManagerNonDb.OpenSystemDepartment( DB_READONLY, info );
		DataManagerNonDb.OpenSystemDepartmentSet( DB_READONLY, info );
		PresetReportManager.Open( NODE_PRESET_PLU_HIST, DB_READONLY, info );
		
#ifndef STOCKMAN_SYSTEM
		DataManagerNonDb.OpenEcrmanSupplier( DB_READONLY, info );
#endif

#ifdef COMPILE_PMS
		if ( SysInfo.IsPMSSystem() == TRUE )
		{
			CFilenameUpdater FnUp( SysFiles::PMSBooking );
			DataManagerNonDb.PMSBooking.Read( FnUp.GetFilenameToUse() );
			PMSCheckoutDateArray.InvalidateCheckoutDateArray();
			
			if ( CPMSCalendarHelpers::CheckReports() == TRUE )
				dbLocation.CreatePMSPseudoLocation();
		}
#endif

		CSalesHistoryReportPluDlg dlg ( nPresetNo, bAutoRunPreset, m_pParentWnd );
		dlg.DoModal();

		switch( dlg.GetPresetError() )
		{
		case 1:
			{
				CString strMsg;
				strMsg.Format( "Unknown preset report #%3.3d", nPresetNo );
				Prompter.Error( strMsg );
			}
			break;

		case 2:
			ButtonActionHandler.ShowAuthorisationError(6);
			break;
		}

#ifdef COMPILE_PMS
		if ( SysInfo.IsPMSSystem() == TRUE )
		{
			PMSCheckoutDateArray.InvalidateCheckoutDateArray();
			dbLocation.RemovePMSPseudoLocation();
		}
#endif

		DataManagerNonDb.CloseReportSession( info );
		DataManagerNonDb.CloseTimeSliceList( info );
		DataManagerNonDb.CloseSalesHistoryPlu( info );
		DataManagerNonDb.CloseSystemPluFilterNames( info );
		DataManagerNonDb.CloseSystemPluFilterItems( info );
		DataManagerNonDb.CloseSystemDepartment( info );
		DataManagerNonDb.CloseSystemDepartmentSet( info );
		PresetReportManager.Close( NODE_PRESET_PLU_HIST, info );

#ifndef STOCKMAN_SYSTEM
		DataManagerNonDb.CloseEcrmanSupplier( info );
#endif
	}
}

/**********************************************************************/

void CDataAccessHelpersReports::EposReports( int nPresetNo, bool bPresetPrint, bool bAutoRunPreset )
{
	AutoExpireHistory.ExpireReportsNow();
	
	if ( dbLocation.CanDoReports() == FALSE )
		Prompter.Error ( "You have not set up any locations that\nare linked to a system database" );
	else if ( PasswordArray.CheckLocationAccessLoc() == FALSE )
		Prompter.LocationAccessError();
	else
	{
		CDataManagerInfo info;
		PresetReportManager.Open( NODE_PRESET_EPOS_BATCH, DB_READONLY, info );
		DataManagerNonDb.OpenReportSession( DB_READONLY, info );
		DataManagerNonDb.OpenTimeSliceList( DB_READONLY, info );
		DataManagerNonDb.OpenSystemPluFilterNames( DB_READONLY, info );
		DataManagerNonDb.OpenSystemPluFilterItems( DB_READONLY, info );
		DataManagerNonDb.OpenSystemDepartment( DB_READONLY, info );
		DataManagerNonDb.OpenSystemDepartmentSet( DB_READONLY, info );
		PresetReportManager.Open( NODE_PRESET_EPOS, DB_READONLY, info );
		
		if ( SysInfo.IsPMSSystem() == FALSE )
		{
			CReportEposDlg dlg ( nPresetNo, bPresetPrint, bAutoRunPreset, m_pParentWnd );
			dlg.DoModal();

			switch( dlg.GetPresetError() )
			{
			case 1:
				{
					CString strMsg;
					strMsg.Format( "Unknown preset report #%3.3d", nPresetNo );
					Prompter.Error( strMsg );
				}
				break;

			case 2:
				ButtonActionHandler.ShowAuthorisationError(6);
				break;
			}
		}

#ifdef COMPILE_PMS

		if ( SysInfo.IsPMSSystem() == TRUE )
		{
			if ( CPMSCalendarHelpers::CheckReports() == TRUE )
				dbLocation.CreatePMSPseudoLocation();
			
			CFilenameUpdater FnUp( SysFiles::PMSBooking );
			DataManagerNonDb.PMSBooking.Read( FnUp.GetFilenameToUse() );
			PMSCheckoutDateArray.InvalidateCheckoutDateArray();

			DataManagerNonDb.PMSCustomer.ReadAndUpdateCustomerDatabase();
		
			CReportEposPMSDlg dlg ( nPresetNo, bPresetPrint, bAutoRunPreset, m_pParentWnd );
			dlg.DoModal();

			switch( dlg.GetPresetError() )
			{
			case 1:
				{
					CString strMsg;
					strMsg.Format( "Unknown preset report #%3.3d", nPresetNo );
					Prompter.Error( strMsg );
				}
				break;

			case 2:
				ButtonActionHandler.ShowAuthorisationError(6);
				break;
			}

			dbLocation.RemovePMSPseudoLocation();
			PMSCheckoutDateArray.InvalidateCheckoutDateArray();
		}

#endif

		PresetReportManager.Close( NODE_PRESET_EPOS_BATCH, info );
		DataManagerNonDb.CloseReportSession( info );
		DataManagerNonDb.CloseTimeSliceList( info );
		DataManagerNonDb.CloseSystemPluFilterNames( info );
		DataManagerNonDb.CloseSystemPluFilterItems( info );
		DataManagerNonDb.CloseSystemDepartment( info );
		DataManagerNonDb.CloseSystemDepartmentSet( info );
		PresetReportManager.Close( NODE_PRESET_EPOS, info );
	}
}

/**********************************************************************/

void CDataAccessHelpersReports::CustomerTransactions( int nPresetNo, bool bAutoRunPreset )
{
	AutoExpireHistory.ExpireReportsNow();

	if ( dbLocation.GetSize() == 0 )
		ShowLocationError();
	else if ( PasswordArray.CheckLocationAccessLoc() == FALSE )
		Prompter.LocationAccessError();
	else
	{
		CDataManagerInfo info;
		DataManagerNonDb.OpenReportSession( DB_READONLY, info );
		DataManagerNonDb.OpenTimeSliceList( DB_READONLY, info );
		DataManagerNonDb.OpenSalesHistoryCustomer( DB_READONLY, info );
		DataManagerNonDb.OpenSystemDepartment( DB_READONLY, info );
		DataManagerNonDb.OpenSystemDepartmentSet( DB_READONLY, info );
		PresetReportManager.Open( NODE_PRESET_CUST_HIST, DB_READONLY, info );

		CSalesHistoryReportCustomerDlg dlg ( nPresetNo, bAutoRunPreset, m_pParentWnd );
		dlg.DoModal();

		switch( dlg.GetPresetError() )
		{
		case 1:
			{
				CString strMsg;
				strMsg.Format( "Unknown preset report #%3.3d", nPresetNo );
				Prompter.Error( strMsg );
			}
			break;

		case 2:
			ButtonActionHandler.ShowAuthorisationError(6);
			break;
		}

		DataManagerNonDb.CloseReportSession( info );
		DataManagerNonDb.CloseTimeSliceList( info );
		DataManagerNonDb.CloseSalesHistoryCustomer( info );
		DataManagerNonDb.CloseSystemDepartment( info );
		DataManagerNonDb.CloseSystemDepartmentSet( info );
		PresetReportManager.Close( NODE_PRESET_CUST_HIST, info );
	}
}

/**********************************************************************/

void CDataAccessHelpersReports::TimeAttendance( int nPresetNo, bool bAutoRunPreset )
{
	AutoExpireHistory.ExpireReportsNow();

	if ( dbLocation.CanDoReports() == FALSE )
		Prompter.Error ( "You have not set up any locations that\nare linked to a system database" );
	else if ( PasswordArray.CheckLocationAccessLoc() == FALSE )
		Prompter.LocationAccessError();
	else
	{
		CDataManagerInfo info;
		DataManagerNonDb.OpenReportSession( DB_READONLY, info );
		PresetReportManager.Open( NODE_PRESET_TIMEATTEND, DB_READONLY, info );
		
		CReportTimeAndAttendanceDlg dlg( nPresetNo, bAutoRunPreset );
		dlg.DoModal();

		switch( dlg.GetPresetError() )
		{
		case 1:
			{
				CString strMsg;
				strMsg.Format( "Unknown preset report #%3.3d", nPresetNo );
				Prompter.Error( strMsg );
			}
			break;

		case 2:
			ButtonActionHandler.ShowAuthorisationError(6);
			break;
		}

		DataManagerNonDb.CloseReportSession( info );
		PresetReportManager.Close( NODE_PRESET_TIMEATTEND, info );
	}

	DataManager.CloseDatabase();
}

/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/

void CDataAccessHelpersReports::SalesHistoryStock( int nPresetNo, bool bAutoRunPreset )
{
	AutoExpireHistory.ExpireReportsNow();

	if ( dbStockpoint.GetSize() == 0 )
		ShowStockpointError();
	else if ( PasswordArray.CheckLocationAccessSp() == FALSE )
		Prompter.StockpointAccessError();
	else
	{
		CDataManagerInfo info;
		DataManagerNonDb.OpenSystemCategory( DB_READONLY, info );
		DataManagerNonDb.OpenSystemCategorySet( DB_READONLY, info );

		for ( int nFieldNo = 1; nFieldNo <= MAX_ITEM_CUSTOM_FIELDS; nFieldNo++ )
		{
			DataManagerNonDb.OpenSystemCustomFieldSet( nFieldNo, DB_READONLY, info );
		}

		CSalesHistoryReportStockNewDlg dlg ( nPresetNo, bAutoRunPreset, m_pParentWnd );
		dlg.DoModal();

		DataManagerNonDb.CloseSystemCategory( info );
		DataManagerNonDb.CloseSystemCategorySet( info );

		for ( int nFieldNo = 1; nFieldNo <= MAX_ITEM_CUSTOM_FIELDS; nFieldNo++ )
		{
			DataManagerNonDb.CloseSystemCustomFieldSet( nFieldNo, info );
		}

		switch( dlg.GetPresetError() )
		{
		case 1:
			{
				CString strMsg;
				strMsg.Format( "Unknown preset report #%3.3d", nPresetNo );
				Prompter.Error( strMsg );
			}
			break;

		case 2:
			ButtonActionHandler.ShowAuthorisationError(6);
			break;
		}
	}
}

/**********************************************************************/
#else
/**********************************************************************/

void CDataAccessHelpersReports::EcrmanStockLevel( int nPresetNo, bool bAutoRunPreset )
{
	CDataManagerInfo info;
	PresetReportManager.Open( NODE_PRESET_ECR_STOCK, DB_READONLY, info );
	
	AutoExpireHistory.ExpireReportsNow();
	CReportEcrmanStockDlg dlg ( nPresetNo, bAutoRunPreset, m_pParentWnd );
	dlg.DoModal();

	PresetReportManager.Close( NODE_PRESET_ECR_STOCK, info );
}

/**********************************************************************/
#endif
/**********************************************************************/
