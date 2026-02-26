/**********************************************************************/
#include "..\CommonEcrStock\AutoExpireHistory.h"
#include "..\CommonEcrStock\CustomerStatementDlg.h"
#include "..\CommonEcrStock\DataAccessHelpersReports.h"
#include "DataAccessHelpersStockData.h"
#include "DorbiereExportDlg.h"
#include "..\CommonEcrStock\IDraughtReportDlg.h"
#include "..\CommonEcrStock\PMSCalendarHelpers.h"
#include "Password.h"
#include "PMSCheckoutDateArray.h"
#include "ReportAuditDlg.h"
#include "ReportStockNewDlg.h"
#include "..\CommonEcrStock\SalesHistoryDateRangePluDlg.h"
#include "..\CommonEcrStock\SalesHistoryRegionPluDlg.h"
#include "SalesHistoryReportBritanniaDlg.h"
#include "..\CommonEcrStock\SalesHistoryReportCustomerDlg.h"
#include "..\CommonEcrStock\SalesHistoryReportPluDlg.h"
#include "SalesHistoryReportStockNewDlg.h"
#include "StockpointCSVArray.h"
#include "SysInfo.h"
/**********************************************************************/
#include "MainFrmStockman.h"
/**********************************************************************/

void CMainFrameStockman::OnReportsEpos()
{
	CDataAccessHelpersReports Helper;
	Helper.SetParentWnd( this );
	Helper.EposReports(0,FALSE,FALSE);
}

/**********************************************************************/

void CMainFrameStockman::OnReportsWorking()
{
	CDataAccessHelpersReports Helper;
	Helper.SetParentWnd( this );
	Helper.TimeAttendance(0,FALSE);
}

/**********************************************************************/

void CMainFrameStockman::OnReportsStockLevels() 
{
	CLocationSelectorEntity LocSelEntity;
	LocSelEntity.SetSystemNode();

	CDataAccessHelpersStockData Helper;
	Helper.SetParentWnd( this );
	Helper.HandleStockReports( 0, FALSE, LocSelEntity );
}

/**********************************************************************/

void CMainFrameStockman::OnReportsStockAudit() 
{
	CLocationSelectorEntity LocSelEntity;
	LocSelEntity.SetSystemNode();

	CDataAccessHelpersStockData Helper;
	Helper.SetParentWnd( this );
	Helper.HandleStockAudits( 0, FALSE, LocSelEntity );
}

/**********************************************************************/

void CMainFrameStockman::OnReportsSalesHistoryStock() 
{
	CDataAccessHelpersReports Helper;
	Helper.SetParentWnd( this );
	Helper.SalesHistoryStock(0,FALSE);
}

/**********************************************************************/

void CMainFrameStockman::OnReportsSalesHistoryPlu() 
{
	CDataAccessHelpersReports Helper;
	Helper.SetParentWnd( this );
	Helper.SalesHistoryPlu(0,FALSE);
}

/**********************************************************************/

void CMainFrameStockman::OnReportsSalesHistoryPluRegion() 
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
		
#ifdef COMPILE_PMS

		if ( SysInfo.IsPMSSystem() == TRUE )
		{
			if ( CPMSCalendarHelpers::CheckReports() == TRUE )
				dbLocation.CreatePMSPseudoLocation();
		
			CFilenameUpdater FnUp( SysFiles::PMSBooking );
			DataManagerNonDb.PMSBooking.Read( FnUp.GetFilenameToUse() );
			PMSCheckoutDateArray.InvalidateCheckoutDateArray();
		}

#endif

		CSalesHistoryRegionPluDlg dlg ( this );
		dlg.DoModal();

#ifdef COMPILE_PMS

		if ( SysInfo.IsPMSSystem() == TRUE )
		{
			dbLocation.RemovePMSPseudoLocation();
			PMSCheckoutDateArray.InvalidateCheckoutDateArray();
		}

#endif

		DataManagerNonDb.CloseReportSession( info );
	}
}

/**********************************************************************/

void CMainFrameStockman::OnReportsSalesHistoryPluDateRange() 
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
		
#ifdef COMPILE_PMS

		if ( SysInfo.IsPMSSystem() == TRUE )
		{
			if ( CPMSCalendarHelpers::CheckReports() == TRUE )
				dbLocation.CreatePMSPseudoLocation();
		
			CFilenameUpdater FnUp( SysFiles::PMSBooking );
			DataManagerNonDb.PMSBooking.Read( FnUp.GetFilenameToUse() );
			PMSCheckoutDateArray.InvalidateCheckoutDateArray();
		}

#endif

		CSalesHistoryDateRangePluDlg dlg ( this );
		dlg.DoModal();

#ifdef COMPILE_PMS

		if ( SysInfo.IsPMSSystem() == TRUE )
		{
			dbLocation.RemovePMSPseudoLocation();
			PMSCheckoutDateArray.InvalidateCheckoutDateArray();
		}

#endif

		DataManagerNonDb.CloseReportSession( info );
	}
}

/**********************************************************************/

void CMainFrameStockman::OnReportsCustomer()
{
	CDataAccessHelpersReports Helper;
	Helper.SetParentWnd( this );
	Helper.CustomerTransactions(0,FALSE);
}

/**********************************************************************/

void CMainFrameStockman::OnReportsCustomerStatement()
{
	AutoExpireHistory.ExpireReportsNow();

	if ( dbLocation.GetSize() == 0 )
		ShowLocationError();
	else if ( PasswordArray.CheckLocationAccessLoc() == FALSE )
		Prompter.LocationAccessError();
	else
	{
		//CDataManagerInfo info;
		//DataManagerNonDb.OpenReportSession( DB_READONLY, info );
		//DataManagerNonDb.OpenTimeSliceList( DB_READONLY, info );
		//DataManagerNonDb.OpenSalesHistoryCustomer( DB_READONLY, info );
		//DataManagerNonDb.OpenSalesHistoryPresetCustomer( DB_READONLY, info );

		CCustomerStatementDlg dlg ( this );
		dlg.DoModal();

		//DataManagerNonDb.CloseReportSession( info );
		//DataManagerNonDb.CloseTimeSliceList( info );
		//DataManagerNonDb.CloseSalesHistoryCustomer( info );
		//DataManagerNonDb.CloseSalesHistoryPresetCustomer( info );
	}
}

/**********************************************************************/

void CMainFrameStockman::OnReportsBritannia()
{
	CDataManagerInfo info;
	DataManagerNonDb.OpenReportSession(DB_READONLY, info);
	DataManagerNonDb.OpenSystemDepartment(DB_READONLY, info);
	DataManagerNonDb.OpenSystemDepartmentSet(DB_READONLY, info);

	CSalesHistoryReportBritanniaDlg dlgBritannia(this);
	dlgBritannia.DoModal();

	DataManagerNonDb.CloseReportSession(info);
	DataManagerNonDb.CloseSystemDepartment(info);
	DataManagerNonDb.CloseSystemDepartmentSet(info);
}

/**********************************************************************/

void CMainFrameStockman::OnReportsDorbiere()
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
		
		CDorbiereExportDlg dlg( this );
		dlg.DoModal();

		DataManagerNonDb.CloseReportSession( info );
	}
}

/**********************************************************************/

void CMainFrameStockman::OnReportsIDraught()
{
	if ( dbLocation.GetIDraughtStatus() == IDRAUGHT_STATUS_UNKNOWN )
		dbLocation.UpdateIDraughtStatus();

	CIDraughtReportDlg dlg( this );
	dlg.DoModal();
}

/**********************************************************************/
