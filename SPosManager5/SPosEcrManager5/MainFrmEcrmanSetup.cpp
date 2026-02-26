/**********************************************************************/
#include "BarcodePluNoTable.h"
#include "..\CommonEcrStock\BarcodeTableListDlg.h"
#include "..\CommonEcrStock\CommsSettingsDlg.h"
#include "..\CommonEcrStock\IDraughtExportManager.h"
#include "..\CommonEcrStock\ListDataNetworkResourcesDlg.h"
#include "LocationCSVArray.h"
#include "NetworkCSVArray.h"
#include "NetworkResources.h"
#include "PCOptions.h"
#include "PMSCalendarTable.h"
#include "PMSZeroNightStayArray.h"
#include "PosTrayOptions.h"
#include "PosTrayTokenFiles.h"
#include "..\CommonEcrStock\PropertySheetPMSOptions.h"
#include "..\CommonEcrStock\PropertySheetPosTrayOptions.h"
#include "..\CommonEcrStock\PropertySheetSystemTextsList.h"
#include "RepSpawn.h"
#include "..\CommonEcrStock\SageExportManager.h"
#include "SysInfo.h"
/**********************************************************************/
#include "MainFrmEcrman.h"
/**********************************************************************/

void CMainFrameEcrman::OnSetupSystem() 
{	
	CDataAccessHelpersSet AccessHelpers;
	AccessHelpers.SetParentWnd(this);
	AccessHelpers.m_Setup.SetupSystem();
}

/**********************************************************************/

void CMainFrameEcrman::OnSetupTasks() 
{
	CDataAccessHelpersTask Helper;
	Helper.SetParentWnd( this );
	
	if ( Helper.SetupTasks() == TRUE )
		BuildJobMenu();
}

/**********************************************************************/

void CMainFrameEcrman::OnSetupBarcodeNumbers() 
{
	if ( DataManager.LockAllUsers() == FALSE )
		Prompter.LockError( "change these settings" );
	else
	{
		BarcodePluNoTable.RememberSettings();

		CBarcodeTableListDlg dlg ( this );
		
		if ( dlg.DoModal() == IDOK )
			BarcodePluNoTable.Write();
		else
			BarcodePluNoTable.RestoreSettings();
		
		DataManager.UnlockAllUsers();
	}
}

/**********************************************************************/

void CMainFrameEcrman::OnSetupBarcode() 
{
	if ( ::FileExists ( "SPosBarcodeManager.pgm" ) == FALSE )
	{
		Prompter.ContactDealer ( "You must install Barcode Manager before you can setup label formats." );
		return;
	}
	
	if ( DataManager.LockAllUsers() == FALSE )
		Prompter.LockError( "setup labels" );
	else
	{
		SetupBarcodeInternal();
		DataManager.UnlockAllUsers();
	}
}

/**********************************************************************/

void CMainFrameEcrman::SetupBarcodeInternal()
{
	CString strProgramPath = "";
	GetDataProgramPath( strProgramPath );

	CString strDevmodePath = "PRINTA_";
	GetPCPathClient( strDevmodePath );

	CProtect1 protect;
	SSspawnl ( GetSafeOwner(this), P_WAIT, "SPOSBARCODEMANAGER.PGM", "SPOSBARCODEMANAGER.PGM", "101", protect.GetCode(), strProgramPath, PCOptionsClient.GetDefaultLabelPrinter(), strDevmodePath, NULL );
}

/**********************************************************************/

void CMainFrameEcrman::OnSetupTexts() 
{
	CPropertySheetSystemTextsList propSheet( this );
	propSheet.DoModal();
}

/**********************************************************************/

void CMainFrameEcrman::OnSetupPMSOptions()
{
#ifdef COMPILE_PMS
	if ( SysInfo.IsPMSSystem() == TRUE )
	{
		PMSOptions.CreatePMSFolders();

		CString strError = "";

		CSSFile filePMSLock;
		CFilenameUpdater FnUp( SysFiles::PMSLock );
		if ( filePMSLock.Open( FnUp.GetFilenameToUse(), "wb" ) == FALSE )
			Prompter.Error( "The Guest Account system is being modified by another user.\n\nPlease try again later." );
		else
		{
			CFilenameUpdater FnUp1( SysFiles::PMSRooms );
			DataManagerNonDb.PMSRoom.Read( FnUp1.GetFilenameToUse() );

			CFilenameUpdater FnUp2( SysFiles::PMSBooking );
			DataManagerNonDb.PMSBooking.Read( FnUp2.GetFilenameToUse() );

			CFilenameUpdater FnUp3( SysFiles::PMSClosure );
			DataManagerNonDb.PMSClosure.Read( FnUp3.GetFilenameToUse() );

			CFilenameUpdater FnUp4( SysFiles::PMSRoomType );
			DataManagerNonDb.PMSRoomType.Read( FnUp4.GetFilenameToUse() );

			CFilenameUpdater FnUp5( SysFiles::PMSRoomGroup );
			DataManagerNonDb.PMSRoomGroup.Read( FnUp5.GetFilenameToUse() );
		
			CDataManagerInfo info;

			CWordArray arrayReadWrite;
			arrayReadWrite.Add( NODE_PLU );
			arrayReadWrite.Add( NODE_DEPARTMENT );
			arrayReadWrite.Add( NODE_DEPT_GROUP_EPOS );

			bool bCanWriteDatabase = FALSE;
			if ( DataManager.OpenDatabaseReadWrite( 0, arrayReadWrite, info, FALSE ) == TRUE )
				bCanWriteDatabase = TRUE;
			else
				DataManager.OpenDatabaseReadOnly( 0, info, FALSE );

			DataManagerNonDb.PMSCustomer.ReadAndUpdateCustomerDatabase();

			PMSCalendarTable.RemoveAll();
			PMSZeroNightStayArray.RemoveAll();
			DataManagerNonDb.PMSClosure.BuildInternalTables();
			DataManagerNonDb.PMSBooking.BuildInternalTables();

			DataManagerNonDb.PMSBooking.ValidateBookingDatabase();
			DataManagerNonDb.PMSCustomer.ValidateCustomerDatabase();

			bool bFinished = FALSE;
		
			while ( FALSE == bFinished )
			{
				CPMSOptions PMSOptionsOld;
				PMSOptionsOld.CopyFrom( PMSOptions );

				CPMSEmailSignatures EmailSignatures;
				EmailSignatures.ReadAll();
				
				CPropertySheetPMSOptions propSheet ( bCanWriteDatabase, EmailSignatures, this );

				if ( propSheet.DoModal() == IDOK )
				{
					PMSOptions.Write();
					EmailSignatures.WriteAllIfChanged();

					if ( propSheet.HasChangedDatabase() == TRUE )
					{
						CDataManagerInfo infoWrite;
						DataManager.WritePlu( infoWrite );
						DataManager.WriteDepartment( infoWrite );
						DataManager.WriteEposGroup( infoWrite );
					}

					CFilenameUpdater FnUp( SysFiles::PMSRooms );
					if ( DataManagerNonDb.PMSRoom.Write( FnUp.GetFilenameToUse() ) == FALSE )
					{
						Prompter.Error( "Unable to write Guest Account room file" );
						bFinished = TRUE;
					}
					else
					{	
						switch( DataManagerNonDb.PMSRoom.CheckPOSFolders() )
						{
						case PMS_ERROR_NONE:
						case PMS_ERROR_ABANDON:
							bFinished = TRUE;
							break;
						}
					}
				}
				else
				{
					PMSOptions.CopyFrom( PMSOptionsOld );
					bFinished = TRUE;
				}
			}

			DataManager.CloseDatabase();
		}
	}
#endif
}

/***********************************************************************/

void CMainFrameEcrman::OnSetupNetworkResources() 
{
	CLockManagerInfo infoLock;
	infoLock.m_bLockAllUsers = TRUE;
	infoLock.m_strLockAllUsersPrompt = "change these settings";
	infoLock.m_nCommsAction = COMMS_ACTION_PAUSE;
	infoLock.m_bStopComms = TRUE;
	infoLock.m_bStopCommsPMSExemption = FALSE;
	infoLock.m_strStopCommsPrompt = "Setup folder passwords";
	infoLock.m_bStopPosTray = TRUE;
	infoLock.m_pParentWnd = this;

	CLockManager LockManager( infoLock );

	if ( LockManager.GetLock() == TRUE )
	{
		CListDataNetworkResourcesDlg dlg( this );

		CNetworkResources oldNetworkResources;
		oldNetworkResources.CopyFrom( NetworkResources );

		if ( dlg.DoModal() != IDOK )
			NetworkResources.CopyFrom( oldNetworkResources );
		else
		{
			NetworkResources.Write();
			NetworkResources.RunBatchFile();
		}
	}
}

/***********************************************************************/

void CMainFrameEcrman::OnSetupOptions() 
{
	CDataAccessHelpersSet AccessHelpers;
	AccessHelpers.SetParentWnd( this );
	AccessHelpers.m_Setup.EditOptions();
}

/**********************************************************************/

void CMainFrameEcrman::OnSetupPosTrayOptions()
{
	bool bLockAllUsers = ( DealerFlags.GetPosTraySetupNoLockAllUsersFlag() == FALSE );

	CLockManagerInfo infoLock;
	infoLock.m_bLockAllUsers = bLockAllUsers;
	infoLock.m_strLockAllUsersPrompt = "change these settings";
	infoLock.m_nCommsAction = COMMS_ACTION_PAUSE;
	infoLock.m_bStopComms = TRUE;
	infoLock.m_bStopCommsPMSExemption = FALSE;
	infoLock.m_strStopCommsPrompt = "Setup options";
	infoLock.m_bStopPosTray = TRUE;
	infoLock.m_pParentWnd = this;

	CLockManager LockManager( infoLock );

	if (LockManager.GetLock() == TRUE)
	{
		if (TRUE == bLockAllUsers)
		{
			EditPosTrayOptions();
		}
		else
		{
			CMyCSSLockFile fileLock;
			CFilenameUpdater FnUp(SysFiles::PosTraySettingsLock);

			if (fileLock.OpenLock(FnUp.GetFilenameToUse()) == TRUE)
			{
				EditPosTrayOptions();
			}
			else
			{
				CDataManagerInfo info;
				info.m_nNodeType = NODE_POSTRAY_SETTINGS;
				info.m_nErrorType = DB_ERR_SHARE;
				Prompter.ShareError(info);
			}
		}
	}
}

/***********************************************************************/

void CMainFrameEcrman::EditPosTrayOptions()
{
	DataManagerNonDb.PosTrayExportFieldNames.FixTaxNames();

	CDataManagerInfo info;
	DataManagerNonDb.OpenEmailAddress( DB_READONLY, info );

	CPropertySheetPosTrayOptions propSheet ( this );

	CPosTrayOptions oldOptions;
	oldOptions.CopyFrom( PosTrayOptions );
	PosTrayOptions.ResetRunNowList();

	if ( propSheet.DoModal() != IDOK )
	{
		PosTrayOptions.ResetRunNowList();
		PosTrayOptions.CopyFrom( oldOptions );
	}
	else
	{		
		propSheet.SaveTaskChanges();
		PosTrayOptions.Write();
		PosTrayOptions.ProcessRunNowList();

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

		m_MenuChanger.BuildMenu( GetMenu() );
		DrawMenuBar();

		CPosTrayTokenFiles::CreateSystemUpdateFile();
		CPosTrayTokenFiles::CreateRetryEmailFile();

		if ( PosTrayOptions.GetTemporaryStartupFlag() == TRUE )
		{
			if ( propSheet.RunPosTrayAtStartup() == FALSE )
			{
				CString strError = AfxGetAppName();
				strError += " was unable to start PosTray.";
				Prompter.Error( strError );
			}
		}
		else
			propSheet.RemovePosTrayAtStartup();
	}	

	DataManagerNonDb.CloseEmailAddress( info );
}

/***********************************************************************/

void CMainFrameEcrman::OnSetupIDraughtOptions() 
{
	if ( DataManager.LockAllUsers() == FALSE )
		Prompter.LockError( "change these settings" );
	else
	{
		IDraughtExportManager.HandleSetupOptions( this );
		DataManager.UnlockAllUsers();
	}
}

/**********************************************************************/

void CMainFrameEcrman::OnSetupSageOptions() 
{
	if ( DataManager.LockAllUsers() == FALSE )
		Prompter.LockError( "change these settings" );
	else
	{
		SageExportManager.HandleSetupOptions( this );
		DataManager.UnlockAllUsers();
	}
}

/**********************************************************************/

void CMainFrameEcrman::OnSetupCommsSettings() 
{
	if ( DataManager.LockAllUsers() == FALSE )
		Prompter.LockError( "change these settings" );
	else
	{
		EditCommsSettings();
		DataManager.UnlockAllUsers();
	}
}

/**********************************************************************/

void CMainFrameEcrman::EditCommsSettings()
{
	CCommsSettingsDlg dlg ( this );
	dlg.DoModal();
}

/**********************************************************************/

void CMainFrameEcrman::CheckNetworks()
{
	bool bDuplicateTerminal = FALSE;

	if ( dbNetwork.ValidateNetworks( TRUE, TRUE, TRUE, TRUE ) == FALSE )
	{
		bDuplicateTerminal = TRUE;

		CString strMsg = "You have used the same terminal numbers for two or\n";
		strMsg += "more locations on the same network.\n\n";
		strMsg += "Communications have been disabled for all networks\n";	   
		strMsg += "with duplicated terminal numbers.\n\n";	   
		strMsg += "Please refer to the following report for a list of the\n";	 
		strMsg += "duplicated numbers on each network."; 
		Prompter.Error ( strMsg );

		CReportManager ReportManager;
		CFilenameUpdater FnUp ( SysFiles::NetCheckPrm );
		ReportManager.DisplayReport ( Super.ReportFilename(), FnUp.GetFilenameToUse(), "", this );
	}

	if ( SysInfo.IsIDraughtExportSystem() == TRUE )
	{	
		dbLocation.UpdateIDraughtStatus();
		IDraughtExportManager.BuildNetworkMap();
	}

	if ( SysInfo.IsSageExportSystem() == TRUE )
		SageExportManager.BuildDatabaseMap();
}

/**********************************************************************/
