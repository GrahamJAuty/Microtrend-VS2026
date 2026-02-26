/**********************************************************************/
#include "..\CommonEcrStock\AutoExpireHistory.h"
#include "..\CommonEcrStock\CustomerStatementDlg.h"
#include "..\CommonEcrStock\DataAccessHelpersReports.h"
#include "..\CommonEcrStock\IDraughtReportDlg.h"
#include "Password.h"
#include "..\CommonEcrStock\PMSCalendarHelpers.h"
#include "PMSCheckoutDateArray.h"
#include "..\SPosEcrManager5\ReportEcrmanStockDlg.h"
#include "..\CommonEcrStock\SalesHistoryDateRangePluDlg.h"
#include "..\CommonEcrStock\SalesHistoryRegionPluDlg.h"
#include "..\CommonEcrStock\SalesHistoryReportCustomerDlg.h"
#include "SysInfo.h"
#include "SystemMap.h"
/**********************************************************************/
#include "MainFrmEcrman.h"
/**********************************************************************/

void CMainFrameEcrman::OnReportsEpos()
{
	CDataAccessHelpersReports Helper;
	Helper.SetParentWnd( this );
	Helper.EposReports(0,FALSE,FALSE);
}

/**********************************************************************/

void CMainFrameEcrman::OnReportsWorking()
{
	CDataAccessHelpersReports Helper;
	Helper.SetParentWnd( this );
	Helper.TimeAttendance(0,FALSE);
}

/**********************************************************************/

void CMainFrameEcrman::OnReportsPluStock() 
{
	CDataAccessHelpersReports Helper;
	Helper.SetParentWnd( this );
	Helper.EcrmanStockLevel(0,FALSE);
}

/**********************************************************************/

void CMainFrameEcrman::OnReportsSalesHistoryPlu() 
{
	CDataAccessHelpersReports Helper;
	Helper.SetParentWnd( this );
	Helper.SalesHistoryPlu(0,FALSE);
}

/**********************************************************************/

void CMainFrameEcrman::OnReportsSalesHistoryPluRegion() 
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

void CMainFrameEcrman::OnReportsSalesHistoryPluDateRange() 
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

void CMainFrameEcrman::OnReportsCustomer()
{
	CDataAccessHelpersReports Helper;
	Helper.SetParentWnd( this );
	Helper.CustomerTransactions(0,FALSE);
}

/**********************************************************************/

void CMainFrameEcrman::OnReportsCustomerStatement()
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

void CMainFrameEcrman::OnReportsBritannia()
{
	/*
	CDataManagerInfo info;
	DataManagerNonDb.OpenReportSession( DB_READONLY, info );

	CSalesHistoryReportBritanniaDlg dlgBritannia( this );
	dlgBritannia.DoModal();

	DataManagerNonDb.CloseReportSession( info );
	*/
}

/**********************************************************************/

void CMainFrameEcrman::OnReportsIDraught()
{
	if ( dbLocation.GetIDraughtStatus() == IDRAUGHT_STATUS_UNKNOWN )
		dbLocation.UpdateIDraughtStatus();

	CIDraughtReportDlg dlg( this );
	dlg.DoModal();
}

/**********************************************************************/
