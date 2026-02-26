/**********************************************************************/
#include <process.h>
/**********************************************************************/
#include "..\CommonEcrStockTray\PCOptions.h"
/**********************************************************************/
#include "..\CommonEcrStock\ClearHistoryEposSales.h"
#include "..\CommonEcrStock\DatabaseValidator.h"
#include "..\CommonEcrStock\IDraughtOptions.h"
#include "..\CommonEcrStock\PropertySheetSystemDataList.h"
#include "..\CommonEcrStock\PropertySheetSystemTextsList.h"
#include "..\CommonEcrStock\SageOptions.h"
/**********************************************************************/
#include "CashRSPImporter.h"
#include "ClearHistoryDlgStockman.h"
#include "DatabaseCSVArray.h"
#include "FileRemover.h"
#include "LocationCSVArray.h"
#include "LocationSetCSVArray.h"
#include "NetworkCSVArray.h"
#include "NodeTypes.h"
#include "Password.h"
#include "PMSOptions.h"
#include "PosTrayOptions.h"
#include "ReportpointCSVArray.h"
#include "SharedResources.h"
#include "SiteCSVArray.h"
#include "StockOptionsIni.h"
#include "StockpointCSVArray.h"
#include "SysInfo.h"
#include "SystemMap.h"
#include "V2ConvertErrors.h"
/**********************************************************************/
#include "MainFrmStockman.h"
/**********************************************************************/

void CMainFrameStockman::Housekeeping()
{
	CreateSyssetSubdirectory ( "SchedLog" );
	CreateSyssetSubdirectory ( "SchedOld" );
	CreateSyssetSubdirectory ( "Exports" );
	CreateSyssetSubdirectory ( "SysFiles" );
	CreateDataSubdirectory ( "SysFiles" );

	if ( SysInfo.IsIDraughtExportSystem() == TRUE )
		IDraughtOptions.CreateIDraughtFolders();

	if ( SysInfo.IsSageExportSystem() == TRUE )
		SageOptions.CreateSageFolders();
	
	for ( int nDbIdx = 0; nDbIdx < dbDatabase.GetSize(); nDbIdx++ )
	{	
		CString strFolder = dbDatabase.GetFolderPathData ( nDbIdx );
		if ( ::ExistSubdirectory ( strFolder ) == FALSE )	
			::MakeSubdirectory ( strFolder );

		strFolder = dbDatabase.GetFolderPathSysset ( nDbIdx );
		if ( ::ExistSubdirectory ( strFolder ) == FALSE )	
			::MakeSubdirectory ( strFolder );
	}

	for ( int nNetworkIdx = 0; nNetworkIdx < dbNetwork.GetSize(); nNetworkIdx++ )
	{	
		CString strFolder = dbNetwork.GetFolderPathData ( nNetworkIdx );
		if ( ::ExistSubdirectory ( strFolder ) == FALSE )	
			::MakeSubdirectory ( strFolder );

		strFolder = dbNetwork.GetFolderPathSysset ( nNetworkIdx );
		if ( ::ExistSubdirectory ( strFolder ) == FALSE )	
			::MakeSubdirectory ( strFolder );

		strFolder = dbNetwork.GetFolderPathSyssetCashRSP ( nNetworkIdx );
		if ( ::ExistSubdirectory ( strFolder ) == FALSE )	
			::MakeSubdirectory ( strFolder );

		strFolder = dbNetwork.GetFolderPathSyssetStock ( nNetworkIdx );
		if ( ::ExistSubdirectory ( strFolder ) == FALSE )	
			::MakeSubdirectory ( strFolder );
	}

	for ( int nLocIdx = 0; nLocIdx < dbLocation.GetSize(); nLocIdx++ )
	{		
		CString strFolder = dbLocation.GetFolderPathData( nLocIdx );
		if ( ::ExistSubdirectory ( strFolder ) == FALSE )	
			::MakeSubdirectory ( strFolder );

		strFolder = dbLocation.GetFolderPathSysset( nLocIdx );
		if ( ::ExistSubdirectory ( strFolder ) == FALSE )	
			::MakeSubdirectory ( strFolder );
	}

	for ( int nSpIdx = 0; nSpIdx < dbStockpoint.GetSize(); nSpIdx++ )
	{
		CString strFolder = dbStockpoint.GetFolderPath( nSpIdx );
		if ( ::ExistSubdirectory ( strFolder ) == FALSE )
			::MakeSubdirectory ( strFolder );
	}

#ifdef COMPILE_PMS

	if ( SysInfo.IsPMSSystem() == TRUE )
		PMSOptions.CreatePMSFolders();

#endif
}

/**********************************************************************/

bool CMainFrameStockman::CheckSystem()
{
	if ( dbDatabase.GetSize() > 0 )
		return TRUE;

	if ( DataManager.LockAllUsers() == FALSE )
		return FALSE;

	DealerFlags.SetSPosBarcodeLength(0);
	DealerFlags.WriteToFile();

	dbDatabase.AddDatabase();

	if ( dbNetwork.GetSize() == 0 )
		dbNetwork.AddNetwork( FALSE );
	
	dbStockpoint.AddStockpoint(1);

	EcrmanOptions.SetDefaults();
	StockOptions.SetDefaults();
			
	Housekeeping();
	SystemMapFull.WriteSystem( TRUE );
	SystemMapFull.CreateSiteMap();
	EcrmanOptions.Write();
	StockOptions.Write();

	DataManager.UnlockAllUsers();

	return TRUE;
}

/**********************************************************************/

void CMainFrameStockman::CheckPMSSystem()
{
#ifdef COMPILE_PMS

	if ( SysInfo.IsPMSSystem() == FALSE )
		return;

	if ( DataManager.LockAllUsers() == FALSE )
		return;
	
	CWordArray arrayReadWrite;
	arrayReadWrite.Add( NODE_PLU );
	arrayReadWrite.Add( NODE_DEPARTMENT );
	arrayReadWrite.Add( NODE_DEPT_GROUP_EPOS );

	CDataManagerInfo info;
	DataManager.OpenDatabaseReadWrite( 0, arrayReadWrite, info, FALSE );

	bool bAddDefaults = TRUE;

	if ( PMSOptions.GetEposReportRoomOnlyPlu() != 0 )
		bAddDefaults = FALSE;
	else if ( PMSOptions.GetEposReportRoomBBPlu() != 0 )
		bAddDefaults = FALSE;
	else if ( PMSOptions.GetEposReportRoomDBBPlu() != 0 )
		bAddDefaults = FALSE;
	else if ( PMSOptions.GetEposReportBreakfastPlu() != 0 )
		bAddDefaults = FALSE;
	else if ( PMSOptions.GetEposReportDinnerPlu() != 0 )
		bAddDefaults = FALSE;

	if ( TRUE == bAddDefaults )
	{
		int nPluIdx1;
		DataManager.Plu.FindPluByNumber( 99999999999901, nPluIdx1 );

		int nPluIdx2;
		DataManager.Plu.FindPluByNumber( 99999999999906, nPluIdx2 );

		if ( nPluIdx1 != nPluIdx2 )
			bAddDefaults = FALSE;
	}
	
	if ( TRUE == bAddDefaults )
	{
		if ( DataManager.Department.GetSize() > 0 )
		{
			CDepartmentCSVRecord Dept;
			DataManager.Department.GetAt( DataManager.Department.GetSize() - 1, Dept );
			if ( Dept.GetDeptNo() >= 95 )
				bAddDefaults = FALSE;
		}
	}

	if ( TRUE == bAddDefaults )
	{
		if ( DataManager.EposGroup.GetSize() > 0 )
		{
			CGroupCSVRecordEpos Group;
			DataManager.EposGroup.GetAt( DataManager.EposGroup.GetSize() - 1, Group );
			if ( Group.GetGroupNo() >= 95 )
				bAddDefaults = FALSE;
		}
	}

	if ( TRUE == bAddDefaults )
	{
		::AddPMSPluItem ( 1, "Room Charge", "", 999 );
		::AddPMSPluItem ( 2, "B & B Room Charge", "", 998 );
		::AddPMSPluItem ( 3, "DB & B Room Charge", "", 997 );
		::AddPMSPluItem ( 4, "Breakfast", "", 996 );
		::AddPMSPluItem ( 5, "Dinner Supplement", "Dinner Credit", 995 );
		::AddPMSPluItem ( 6, "Extra Bed", "", 994 );
		::AddPMSPluItem ( 7, "Extra Cot", "", 993 );
			
		DataManager.Department.AddPMSDepartment( 999, "Room Charge" );
		DataManager.Department.AddPMSDepartment( 998, "B & B Room Charge" );
		DataManager.Department.AddPMSDepartment( 997, "DB & B Room Charge" );
		DataManager.Department.AddPMSDepartment( 996, "Breakfast" );
		DataManager.Department.AddPMSDepartment( 995, "Dinner Credit" );
		DataManager.Department.AddPMSDepartment( 994, "Extra Bed" );
		DataManager.Department.AddPMSDepartment( 993, "Extra Cot" );

		DataManager.EposGroup.AddPMSGroup( 99, "Room Charge" );
		DataManager.EposGroup.AddPMSGroup( 98, "B & B Room Charge" );
		DataManager.EposGroup.AddPMSGroup( 97, "DB & B Room Charge" );
		DataManager.EposGroup.AddPMSGroup( 96, "Breakfast" );
		DataManager.EposGroup.AddPMSGroup( 95, "Dinner Credit" );
		DataManager.EposGroup.AddPMSGroup( 94, "Extra Bed" );
		DataManager.EposGroup.AddPMSGroup( 93, "Extra Cot" );
			
		PMSOptions.SetEposReportRoomOnlyPlu( 99999999999901 );
		PMSOptions.SetEposReportRoomBBPlu( 99999999999902 );
		PMSOptions.SetEposReportRoomDBBPlu( 99999999999903 );
		PMSOptions.SetEposReportExtraBedPlu( 99999999999906 );
		PMSOptions.SetEposReportExtraCotPlu( 99999999999907 );
		PMSOptions.SetEposReportBreakfastPlu( 99999999999904 );
		PMSOptions.SetEposReportDinnerPlu( 99999999999905 );

		DataManager.WritePlu( info );
		DataManager.WriteDepartment( info );
		DataManager.WriteEposGroup( info );

		PMSOptions.Write();
	}

	DataManager.CloseDatabase();
	DataManager.UnlockAllUsers();

#endif
}

/**********************************************************************/

void CMainFrameStockman::DisableMenus()
{
	m_MenuChanger.EnableAll( FALSE );
	m_MenuChanger.BuildMenu( GetMenu() );
	DrawMenuBar();
}

/***********************************************************************/

void CMainFrameStockman::SetPasswordMenuAccess()
{	
	m_MenuChanger.EnableAll( TRUE );

	if ( PasswordArray.GetLocationAccessType() != LOCATION_ACCESS_SYSTEM )
	{
		m_MenuChanger.KillParent( IDM_FILE_RESTORE_NORMAL );
		m_MenuChanger.KillItem( IDM_FILE_BACKUP_LOG );
		m_MenuChanger.KillItem( IDM_FILE_BACKUP_STATUS );
		m_MenuChanger.KillParent( IDM_FILE_COMMSLOG_SYSTEM );
		m_MenuChanger.KillItem( IDM_IMPORT_SQL_MAIN );
		m_MenuChanger.KillItem( IDM_SETUP_SYSTEM );
		m_MenuChanger.KillItem( IDM_CALENDAR );
		m_MenuChanger.KillItem( IDM_SETUP_PMS );
		m_MenuChanger.KillItem( IDM_SETUP_POSTRAY );
		m_MenuChanger.KillItem( IDM_FILE_EMAILLOG );
		m_MenuChanger.KillParent( IDM_SETUP_COMMS_SETTINGS );
		m_MenuChanger.KillItem( IDM_BARCODE_NUMBERS );
		m_MenuChanger.KillItem( IDM_SETUP_BARCODE );
		m_MenuChanger.KillItem( IDM_SETUP_FOLDER_PASSWORDS );
		m_MenuChanger.KillItem( IDM_SETUP_OPTIONS );
		m_MenuChanger.KillItem( IDM_ADMIN_DELETE_SUPPLIERS );
	}

	if ( PasswordArray.GetLocationAccessType() == LOCATION_ACCESS_LOC )
	{
		m_MenuChanger.KillParent( IDM_ADMIN_DELETE_ORDERS );
		m_MenuChanger.KillItem( IDM_DATABASE_ORDER_PROCESS );
		m_MenuChanger.KillItem( IDM_DATABASE_ORDER_PROCESS_COST );
	
		if ( StockOptions.GetOrderSeparationType() != ORDER_DUSEPARATE_ORDER )
		{
			m_MenuChanger.KillParent( IDM_DATABASE_ORDER_PROCESS );
			m_MenuChanger.KillParent( IDM_DATABASE_ORDER_PROCESS_COST );
		}

		m_MenuChanger.KillParent( IDM_ADMIN_DELETE_PLU_UNSOLD );
		m_MenuChanger.KillParent( IDM_ADMIN_DELETE_STOCK_UNSOLD );
		m_MenuChanger.KillItem( IDM_ADMIN_CHANGE_STOCK_CODES );
		m_MenuChanger.KillItem( IDM_ADMIN_DELETE_BARCODES );
	}

	if ( PasswordArray.GetLocationAccessType() == LOCATION_ACCESS_NONE )
	{
		m_MenuChanger.EnableAll( FALSE );
		m_MenuChanger.EnableParent( ID_APP_EXIT, TRUE );
		m_MenuChanger.EnableItem( ID_APP_EXIT, TRUE );
		m_MenuChanger.EnableParent( ID_APP_ABOUT, TRUE );
		m_MenuChanger.EnableItem( ID_APP_ABOUT, TRUE );
		m_MenuChanger.EnableItem( IDM_HELP_LICENCEDETAILS, TRUE );
	}
	else if ( DatabaseValidator.ValidAtStartup() == FALSE )
	{
		m_MenuChanger.EnableAll( FALSE );
		m_MenuChanger.EnableParent( IDM_FILE_BACKUP, TRUE );
		m_MenuChanger.EnableParent( IDM_FILE_COMMSLOG_SYSTEM, TRUE );
		m_MenuChanger.EnableItem( IDM_FILE_COMMSLOG_SYSTEM, TRUE );
		m_MenuChanger.EnableItem( IDM_FILE_COMMSLOG_NETWORK, TRUE );
		m_MenuChanger.EnableItem( IDM_FILE_BACKUP, TRUE );
		m_MenuChanger.EnableItem( IDM_FILE_BACKUP_LOG, TRUE );
		m_MenuChanger.EnableItem( IDM_FILE_BACKUP_STATUS, TRUE );
		m_MenuChanger.EnableParent( IDM_FILE_RESTORE_NORMAL, TRUE );
		m_MenuChanger.EnableItem( ID_APP_EXIT, TRUE );
		m_MenuChanger.EnableParent( ID_APP_ABOUT, TRUE );
		m_MenuChanger.EnableItem( ID_APP_ABOUT, TRUE );
	}
	else
	{
		//FILE MENU
		if ( PasswordArray.GetEnable( PasswordTicks::FileCommsLog ) == FALSE )
			m_MenuChanger.EnableParent ( IDM_FILE_COMMSLOG_SYSTEM, FALSE );
	
		if ( PasswordArray.GetEnable( PasswordTicks::FileImportWizard ) == FALSE )
			m_MenuChanger.EnableItem ( IDM_IMPORT_SQL_MAIN, FALSE );

		if ( PasswordArray.IsMasterPassword() == FALSE )
			m_MenuChanger.KillItem ( IDM_FILE_WRITE_LOG );

		if ( PasswordArray.GetEnable( PasswordTicks::FileBackup ) == FALSE )
			m_MenuChanger.EnableItem ( IDM_FILE_BACKUP, FALSE );

		if ( PasswordArray.GetEnable( PasswordTicks::FileRestore ) == FALSE )	
			m_MenuChanger.EnableParent ( IDM_FILE_RESTORE_NORMAL, FALSE );

		if ( PasswordArray.GetEnable( PasswordTicks::FileBackupLog ) == FALSE )
			m_MenuChanger.EnableItem ( IDM_FILE_BACKUP_LOG, FALSE );

		if ( PasswordArray.GetEnable( PasswordTicks::FileBackupStatus ) == FALSE )
			m_MenuChanger.EnableItem ( IDM_FILE_BACKUP_STATUS, FALSE );

		{
			int nAccessType = PCOptionsHost.GetBackupAccessType();
			m_MenuChanger.ShowItem(IDM_FILE_BACKUP, nAccessType < 2);
			m_MenuChanger.ShowParent(IDM_FILE_RESTORE_NORMAL, nAccessType < 3);
		}

		//COMMS MENU
		{
			bool bPosTrayLog = FALSE;
			bool bPosTrayFNB = FALSE;
			bool bPosTrayFTP = FALSE;
			bool bPosTrayS4 = FALSE;
			bool bPosTrayChartwells = FALSE;
			
			if ( SysInfo.IsPosTraySystem() == TRUE )
			{
				bPosTrayLog = TRUE;
				bPosTrayFNB = PosTrayOptions.GetFNBTaskFlag();
				bPosTrayS4 = PosTrayOptions.GetS4LabourTaskFlag();
				bPosTrayFTP = PosTrayOptions.GotFTPSettings();
				bPosTrayChartwells = PosTrayOptions.GetChartwellsTaskFlag();
			}
			
			m_MenuChanger.ShowItem( IDM_FILE_POSTRAY_ACTION, bPosTrayLog );
			m_MenuChanger.ShowItem( IDM_FILE_POSTRAY_EXPORT, bPosTrayLog );
			m_MenuChanger.ShowItem( IDM_FILE_FNBLOG, bPosTrayFNB );
			m_MenuChanger.ShowItem( IDM_FILE_S4LABOURLOG, bPosTrayS4 );
			m_MenuChanger.ShowItem( IDM_FILE_FTPLOG, bPosTrayFTP );
		}
	
		//SYSTEM MENU
		if ( PasswordArray.GetEnable( PasswordTicks::DatabaseOrderProcess ) == FALSE )
			m_MenuChanger.EnableParent ( IDM_DATABASE_EDITDATA, FALSE );
	
		if ( PasswordArray.GetEnable( PasswordTicks::DatabaseOrderProcess ) == FALSE )
			m_MenuChanger.EnableParent ( IDM_DATABASE_ORDER_GENERATE, FALSE );
	
		if ( DealerFlags.GetTransitionalOrderFlag() == FALSE )
		{
			if ( StockOptions.GetOrderSeparationType() != ORDER_DUSEPARATE_ORDER )
			{
				m_MenuChanger.ShowItem( IDM_DATABASE_ORDER_PROCESS, TRUE );
				m_MenuChanger.ShowItem( IDM_DATABASE_ORDER_PROCESS_COST, TRUE );
				m_MenuChanger.ShowItem( IDM_DATABASE_ORDER_PROCESS_SP, FALSE );
				m_MenuChanger.ShowItem( IDM_DATABASE_ORDER_PROCESS_COST_SP, FALSE );
				m_MenuChanger.SetItemText( IDM_DATABASE_ORDER_PROCESS, "&Process Orders" );
				m_MenuChanger.SetItemText( IDM_DATABASE_ORDER_PROCESS_COST, "&Adjust Cost Price" );
			}
			else
			{
				m_MenuChanger.ShowItem( IDM_DATABASE_ORDER_PROCESS, FALSE );
				m_MenuChanger.ShowItem( IDM_DATABASE_ORDER_PROCESS_COST, FALSE );
				m_MenuChanger.ShowItem( IDM_DATABASE_ORDER_PROCESS_SP, TRUE );
				m_MenuChanger.ShowItem( IDM_DATABASE_ORDER_PROCESS_COST_SP, TRUE );
				m_MenuChanger.SetItemText( IDM_DATABASE_ORDER_PROCESS_SP, "&Process Orders" );
				m_MenuChanger.SetItemText( IDM_DATABASE_ORDER_PROCESS_COST_SP, "&Adjust Cost Price" );
			}
		}
		else
		{
			m_MenuChanger.ShowItem( IDM_DATABASE_ORDER_PROCESS, TRUE );
			m_MenuChanger.ShowItem( IDM_DATABASE_ORDER_PROCESS_COST, TRUE );
			m_MenuChanger.ShowItem( IDM_DATABASE_ORDER_PROCESS_SP, TRUE );
			m_MenuChanger.ShowItem( IDM_DATABASE_ORDER_PROCESS_COST_SP, TRUE );
			m_MenuChanger.SetItemText( IDM_DATABASE_ORDER_PROCESS, "Process Orders (&Database)" );
			m_MenuChanger.SetItemText( IDM_DATABASE_ORDER_PROCESS_SP, "Process Orders (&Stockpoint)" );
			m_MenuChanger.SetItemText( IDM_DATABASE_ORDER_PROCESS_COST, "Adjust Cost Price (Database)" );
			m_MenuChanger.SetItemText( IDM_DATABASE_ORDER_PROCESS_COST_SP, "Adjust Cost Price (Stockpoint)" );
		}

		m_MenuChanger.ShowItem( IDM_DATABASE_ORDER_PREVIOUS, StockOptions.GetOrderTemplateFlag() );

		if ( PasswordArray.GetEnable( PasswordTicks::Stocktaking ) == FALSE )
			m_MenuChanger.EnableItem ( IDM_DATABASE_STOCKTAKE, FALSE );

		m_MenuChanger.EnableItem ( IDM_DATABASE_EDITDATA, CPropertySheetSystemDataList::IsRelevant() );

		//TASKS MENU
		if ( PasswordArray.GetEnable( PasswordTicks::TasksMenu ) == FALSE )
			m_MenuChanger.EnableParent ( IDM_TASKS_ADHOC, FALSE );

		//REPORTS MENU
		if ( PasswordArray.GetEnable( PasswordTicks::ReportsMenu ) == FALSE )
			m_MenuChanger.EnableParent ( IDM_REPORTS_EPOS, FALSE );

		if ( PasswordArray.GetEnable( PasswordTicks::ReportsEpos ) == FALSE )
			m_MenuChanger.EnableItem ( IDM_REPORTS_EPOS, FALSE );

		if ( PasswordArray.GetEnable( PasswordTicks::ReportsPluHistory ) == FALSE )
		{
			m_MenuChanger.EnableItem ( IDM_REPORTS_SALESHISTORY_PLU, FALSE );
			m_MenuChanger.EnableItem ( IDM_REPORTS_SALESHISTORY_PLU_REGION, FALSE );
			m_MenuChanger.EnableItem ( IDM_REPORTS_SALESHISTORY_PLU_DATE, FALSE );
		}

		bool bWorking = ( EcrmanOptions.GetFeaturesServerSettingsType() != SERVER_NONE );
		m_MenuChanger.ShowItem( IDM_REPORTS_WORKING, bWorking );
		bWorking &= PasswordArray.GetEnable( PasswordTicks::ReportsWorking );
		m_MenuChanger.EnableItem( IDM_REPORTS_WORKING, bWorking );

		{
			bool bCustomer = ( EcrmanOptions.GetFeaturesCustomerBalanceFlag() == TRUE );
			bool bTransaction = ( EcrmanOptions.GetReportsCustomerTransactionFlag() == TRUE );
			bool bStatement = ( EcrmanOptions.GetReportsCustomerStatementFlag() == TRUE );
			m_MenuChanger.ShowItem( IDM_REPORTS_CUSTOMER, bCustomer & bTransaction );
			m_MenuChanger.ShowItem( IDM_REPORTS_CUSTOMER_STATEMENT, bCustomer & bStatement );
		
			bTransaction &= PasswordArray.GetEnable( PasswordTicks::ReportsCustomer );
			bStatement &= PasswordArray.GetEnable( PasswordTicks::ReportsStatement );
			m_MenuChanger.EnableItem( IDM_REPORTS_CUSTOMER, bCustomer & bTransaction );
			m_MenuChanger.EnableItem( IDM_REPORTS_CUSTOMER_STATEMENT, bCustomer & bStatement );
		}

		bool bRegion = EcrmanOptions.GetReportsRegionDateRangeFlag();
		m_MenuChanger.ShowItem ( IDM_REPORTS_SALESHISTORY_PLU_REGION, bRegion );
		m_MenuChanger.ShowItem ( IDM_REPORTS_SALESHISTORY_PLU_DATE, bRegion );

		if ( PasswordArray.GetEnable( PasswordTicks::ReportsStock ) == FALSE )
		{
			m_MenuChanger.EnableItem ( IDM_REPORTS_STOCK_LEVELS, FALSE );
			m_MenuChanger.EnableItem ( IDM_REPORTS_SALESHISTORY_STOCK, FALSE );
		}

		if ( PasswordArray.GetEnable( PasswordTicks::ReportsAudit ) == FALSE )
			m_MenuChanger.EnableItem ( IDM_REPORTS_STOCK_AUDIT, FALSE );
	
		m_MenuChanger.ShowItem( IDM_REPORTS_BRITANNIA, DealerFlags.GetBritanniaHotelsExportFlag() );
		m_MenuChanger.ShowItem( IDM_REPORTS_DORBIERE, DealerFlags.GetDorbiereImportFlag() );

		if ( SysInfo.GotLoyaltyLink() == FALSE )
		{
			if (SysInfo.GetLoyaltyOrSmartPayType() == LOYALTY_TYPE_UNKNOWN)
			{
				m_MenuChanger.KillItem(IDM_LOYALTY);
			}
			else
			{
				m_MenuChanger.KillParent(IDM_LOYALTY);
			}
		}

		if ( SysInfo.GotSmartPayLink() == FALSE )
		{
			m_MenuChanger.KillItem ( IDM_SMARTPAY );
		}

		if ( PasswordArray.GetEnable( PasswordTicks::Loyalty ) == FALSE )
			m_MenuChanger.EnableParent( IDM_LOYALTY, FALSE );

		if ( PasswordArray.GetEnable( PasswordTicks::Smartpay ) == FALSE )
			m_MenuChanger.EnableItem( IDM_SMARTPAY, FALSE );

		//SETUP MENU	
		if ( PasswordArray.GetEnable( PasswordTicks::SetupMenu ) == FALSE )
			m_MenuChanger.EnableParent ( IDM_SETUP_OPTIONS, FALSE );

		if ( PasswordArray.GetEnable( PasswordTicks::SetupMenuSystem ) == FALSE )
			m_MenuChanger.EnableItem ( IDM_SETUP_SYSTEM, FALSE );

		if ( PasswordArray.GetEnable( PasswordTicks::SetupMenuComms ) == FALSE )
			m_MenuChanger.EnableParent ( IDM_SETUP_COMMS_SETTINGS, FALSE );

		if ( PasswordArray.GetEnable( PasswordTicks::SetupMenuBarcodeEntry ) == FALSE )
			m_MenuChanger.EnableItem ( IDM_BARCODE_NUMBERS, FALSE );

		if ( PasswordArray.GetEnable( PasswordTicks::SetupMenuLabels ) == FALSE )
			m_MenuChanger.EnableItem ( IDM_SETUP_BARCODE, FALSE );

		bool bEnableTexts = PasswordArray.GetEnable( PasswordTicks::SetupMenu );
		bEnableTexts &= PasswordArray.GetEnable( PasswordTicks::SetupMenuTexts );
		bEnableTexts &= CPropertySheetSystemTextsList::IsRelevant();

		m_MenuChanger.EnableItem ( IDM_SETUP_TEXTS, bEnableTexts );
		if ( PasswordArray.GetLocationAccessType() != LOCATION_ACCESS_SYSTEM )
			m_MenuChanger.EnableParent ( IDM_SETUP_TEXTS, bEnableTexts );

		if ( PasswordArray.GetEnable( PasswordTicks::SetupMenuNetworkResources ) == FALSE )
			m_MenuChanger.EnableItem ( IDM_SETUP_FOLDER_PASSWORDS, FALSE );

		if ( PasswordArray.GetEnable( PasswordTicks::SetupMenuOptions ) == FALSE )
			m_MenuChanger.EnableItem ( IDM_SETUP_OPTIONS, FALSE );

		if ( PasswordArray.GetEnable( PasswordTicks::UnitechSetup ) == FALSE )
			m_MenuChanger.EnableItem ( IDM_SETUP_UNITECH, FALSE );
	
		//ADMIN MENU
		m_MenuChanger.ShowItem ( IDM_ADMIN_PASSWORDS, PasswordArray.IsMasterPassword() );
		m_MenuChanger.EnableParent ( IDM_ADMIN_PASSWORDS, PasswordArray.GetEnable( PasswordTicks::AdminMenu ) );
	
		bool bSalesXXX = PasswordArray.GetEnable( PasswordTicks::AdminSalesXXX );
		bool bOrders = PasswordArray.GetEnable( PasswordTicks::AdminOrders );
		bool bAudits = PasswordArray.GetEnable( PasswordTicks::AdminOrderAudits );
		bool bSupplier = PasswordArray.GetEnable( PasswordTicks::AdminSuppliers );
		
		m_MenuChanger.EnableItem ( IDM_ADMIN_SALESXXX, bSalesXXX );
		m_MenuChanger.EnableItem ( IDM_ADMIN_DELETE_SUPPLIERS, bSupplier );
		m_MenuChanger.EnableItem ( IDM_ADMIN_DELETE_ORDERS, bOrders );
		m_MenuChanger.EnableItem ( IDM_ADMIN_DELETE_AUDITS, bAudits );
		m_MenuChanger.EnableParent ( IDM_ADMIN_DELETE_AUDITS, ( bOrders | bAudits ) );

		m_MenuChanger.EnableParent ( IDM_ADMIN_DELETE_STOCK_UNLINKED, PasswordArray.GetEnable( PasswordTicks::AdminPluStock ) );
		m_MenuChanger.EnableParent ( IDM_ADMIN_DELETE_PLU_UNLINKED, PasswordArray.GetEnable( PasswordTicks::AdminPluStock ) );
	
		bool bDeleteBarcode = PasswordArray.GetEnable( PasswordTicks::AdminBarcode );
		m_MenuChanger.EnableItem( IDM_ADMIN_DELETE_BARCODES, bDeleteBarcode );

		m_MenuChanger.EnableItem ( IDM_ADMIN_CHANGE_STOCK_CODES, PasswordArray.GetEnable( PasswordTicks::AdminStockChange ) );
		
		m_MenuChanger.EnableItem ( IDM_ADMIN_DELETE_REPORTS, CClearHistoryDlgStockman::IsAvailable() );

		//PMS
		if ( SysInfo.IsPMSSystem() == FALSE )
		{
			m_MenuChanger.ShowItem( IDM_SETUP_PMS, FALSE );
			m_MenuChanger.ShowItem( IDM_CALENDAR, FALSE );
		}
		else
		{
			bool bEnablePMS = PasswordArray.GetEnable( PasswordTicks::PMS );
			m_MenuChanger.EnableItem( IDM_CALENDAR, bEnablePMS );
			bEnablePMS &= PasswordArray.GetEnable( PasswordTicks::PMSOptions );
			m_MenuChanger.EnableItem( IDM_SETUP_PMS, bEnablePMS );
		}

		if ( SysInfo.IsPosTraySystem() == FALSE )
		{
			m_MenuChanger.ShowItem( IDM_SETUP_POSTRAY, FALSE );
			m_MenuChanger.ShowItem( IDM_FILE_EMAILLOG, FALSE );
		}
	}

	m_MenuChanger.BuildMenu ( GetMenu() );
	DrawMenuBar();
}

/**********************************************************************/

bool CMainFrameStockman::CheckForLegacySystem()
{
	DWORD dwResult = 0;

	bool bAttempted = FALSE;	
	bool bLockWorked = FALSE;

	if ( DataManager.LockAllUsers() == TRUE )
	{	
		bLockWorked = TRUE;
		CheckForLegacySystemInternal( bAttempted, dwResult );
		DataManager.UnlockAllUsers();
	}

	if ( FALSE == bLockWorked )
		return TRUE;

	if ( TRUE == bAttempted )
	{
		switch( dwResult )
		{
		case CONVERT_ERROR_NONE:
			{
				CSSFile fileToken;
				CFilenameUpdater FnUp( SysFiles::RebuildDailyTran );
				if ( fileToken.Open( FnUp.GetFilenameToUse(), "wb" ) == TRUE )
				{
					fileToken.WriteLine( "Microtrend" );
					fileToken.Close();
				}

				Prompter.Info( "The SPOS Upgrade is complete.\n\nPlease restart Stock Manager after closing this message." );
			}
			return FALSE;

		case CONVERT_ERROR_NONE_NOREBUILD:
			Prompter.Info( "The SPOS Upgrade is complete.\n\nPlease restart Stock Manager after closing this message." );
			return FALSE;
		
		case CONVERT_ERROR_PARAMS:
			Prompter.Error( "The SPOS Upgrade module is not compatible with this software." );
			return TRUE;

		default:
			return TRUE;
		}
	}

	CFilenameUpdater FnUp( SysFiles::RebuildDailyTran );
		
	if ( ::FileExists( FnUp.GetFilenameToUse() ) == TRUE )
	{
		CashRSPImporter.ClearCashRSPVersionCheckerFailFlag();
				
		if ( DataManager.LockAllUsers() == TRUE )
		{
			CFileRemover FileRemover( FnUp.GetFilenameToUse() );

			if ( ::FileExists( FnUp.GetFilenameToUse() ) == FALSE )
			{
				CEposSelectArray SelectArray;
				for ( int nLocIdx = 0; nLocIdx < dbLocation.GetSize(); nLocIdx++ )
					SelectArray.SelectLocation( nLocIdx );

				//AVOID USING NODE_DATABASE / NODE_SYSTEM SO THAT STOCK SALES STAY IN PLACE
				//PASS FALSE AS FIRST TWO PARAMS SO THAT WE ONLY DELETE TRANSACTIONS
				CLocationSelectorEntity LocSelEntity;
				LocSelEntity.SetLocationNode(0);

				COleDateTime dateTo = COleDateTime( 9999, 1, 1, 0, 0, 0 );
				CClearHistoryEposSales Clear ( dateTo, LocSelEntity, SelectArray, FALSE, FALSE, TRUE, FALSE );
	
				/*FIDDLE
				for ( int nNwkIdx = 0; ( nNwkIdx < dbNetwork.GetSize() ) && ( CashRSPImporter.GetCashRSPFormatFailFlag() == 0 ); nNwkIdx++ )
				{
					CString strFrom = "00000000";
					CString strTo = "99999999";
					CashRSPImporter.SetRebuildRemoveDuplicatesFlag( FALSE );
					CashRSPImporter.RebuildNetworkSales( nNwkIdx, TRUE, strFrom, strTo );
				}
				*/
			}

			DataManager.UnlockAllUsers();
		}

		if ( CashRSPImporter.GetCashRSPVersionCheckerFailFlag() == TRUE )
		{
			CashRSPImporter.ShowCashRSPVersionCheckerFailMessage();
			CashRSPImporter.ClearCashRSPVersionCheckerFailFlag();
		}
	}

	return TRUE;
}

/**********************************************************************/

void CMainFrameStockman::CheckForLegacySystemInternal( bool& bAttempted, DWORD& dwResult )
{
	bAttempted = FALSE;
	dwResult = 0;

	CString strExe = ".\\SPOSConv4.pgm";	

	if ( SSspawnl( GetSafeOwner(this), P_WAIT, strExe, strExe, "5", NULL ) != CONVERT_ERROR_NONE )
		return;

	bool bEmptySystem = FALSE;

	CDataManagerInfo info;
	if ( DataManager.OpenDatabaseReadOnly( 0, info, FALSE ) == TRUE )
	{
		bEmptySystem = TRUE;

		bEmptySystem &= ( dbDatabase.GetSize() <= 1 );
		bEmptySystem &= ( dbLocation.GetSize() <= 1 );
		bEmptySystem &= ( dbReportpoint.GetSize() == 0 );
		bEmptySystem &= ( dbLocationSet.GetSize() == 0 );
		bEmptySystem &= ( dbStockpoint.GetSize() <= 1 );
		bEmptySystem &= ( dbSite.GetSize() == 0 );
		bEmptySystem &= ( DataManager.Plu.GetSize() == 0 );
		bEmptySystem &= ( DataManager.Stock.GetSize() == 0 );
		bEmptySystem &= ( DataManager.Supplier.GetSize() == 0 );
		bEmptySystem &= ( DataManager.Department.GetSize() <= 1 );
		bEmptySystem &= ( DataManager.EposGroup.GetSize() <= 1 );
		bEmptySystem &= ( DataManager.MixMatch.GetSize() == 0 );
		bEmptySystem &= ( DataManager.OfferGroup.GetSize() == 0 );
		bEmptySystem &= ( DataManager.OfferGroupEntry.GetSize() == 0 );
		bEmptySystem &= ( DataManager.Promotion.GetSize() == 0 );
		bEmptySystem &= ( DataManager.Barcode.GetSize() == 0 );
		bEmptySystem &= ( DataManager.CategorySet.GetSize() == 0 );

		DataManager.CloseDatabase();
	}

	if( bEmptySystem == FALSE )
		return;
	
	CString strDestPath = Sysset.GetDataPath();
	strDestPath.TrimRight('\\');
	
	CString strDestProg = Sysset.GetProgramFolder( SYSSET_STKMANSPOSV4 );
	
	CString strSyssetPath = Sysset.GetDataPath();
	strSyssetPath += "Sysset";
	
	CString strPCId;
	strPCId.Format( "%10.10d", Sysset.GetPCIdHost() );
	
	bAttempted = TRUE;

	DatabaseValidator.SetDoingUpdateFlag( TRUE );
	dwResult = SSspawnl( GetSafeOwner(this), P_WAIT, strExe, strExe, "5", strDestPath, strSyssetPath, strDestProg, strPCId, "1", NULL );
	DatabaseValidator.SetDoingUpdateFlag( FALSE );		
}

/**********************************************************************/

void CMainFrameStockman::ShowBackupResult( CWnd* pParent, bool bDestroyOld )
{
	if ( TRUE == bDestroyOld )
		if ( m_pBackupResultDlg != NULL )
			m_pBackupResultDlg -> DestroyWindow();

	if ( m_pBackupResultDlg == NULL )
	{
		m_pBackupResultDlg = new CBackupResultDlg( &m_pBackupResultDlg, pParent );
		m_pBackupResultDlg -> Create( IDD_BACKUP_RESULT, pParent );
	}
	
	m_pBackupResultDlg -> SetFocus();
	m_pBackupResultDlg -> UpdateDisplay();
}

/***********************************************************************/
