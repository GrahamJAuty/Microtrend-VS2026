/**********************************************************************/
#include <direct.h>
/**********************************************************************/
#include "..\CommonEcrStockTray\AutoBackupProcessor.h"
#include "..\CommonEcrStockTray\SystemBackup.h"
/**********************************************************************/
#include "..\CommonEcrStock\ButtonDefines.h"
#include "..\CommonEcrStock\ButtonManager.h"
#include "..\CommonEcrStock\DatabaseValidator.h"
#include "..\CommonEcrStock\FileWriteLog.h"
#include "..\CommonEcrStock\IDraughtExportCreator.h"
#include "..\CommonEcrStock\IDraughtExportManager.h"
#include "..\CommonEcrStock\PropertySheetWizardImport.h"
#include "..\CommonEcrStock\SageExportManager.h"
#include "..\CommonEcrStock\SelectSingleNetworkDlg.h"
/**********************************************************************/
#include "CommsHandshaker.h"
#include "MessageLogger.h"
#include "NetworkCSVArray.h"
#include "RepSpawn.h"
#include "StatusProgress.h"
#include "SysInfo.h"
/**********************************************************************/
#include "MainFrmEcrman.h"
/**********************************************************************/

void CMainFrameEcrman::OnFileCommsLogSystem() 
{	
	CFilenameUpdater FnUp ( SysFiles::SchedLogPrm );

	if ( ::FileExists ( Super.SchedLogAll() ) == TRUE )
	{
		CReportManager ReportManager;
		ReportManager.DisplayReport ( Super.SchedLogAll(), FnUp.GetFilenameToUse(), "", this );
	}
	else
		Prompter.Error ( "The Comms Log file was not found." );
}

/**********************************************************************/

void CMainFrameEcrman::OnFileCommsLogNetwork() 
{
	switch( dbNetwork.GetSize() )
	{
	case 0:
		Prompter.Error( "You must create a network before you can view this report" );
		break;

	case 1:
		ShowNetworkCommsLog(0);
		break;

	default:
		{
			int nNetworkIdx = -1;
			
			do
			{
				CSelectSingleNetworkDlg dlg ( "View Network Comms Log", this );

				if ( dlg.DoModal() != IDOK )
					return;
	
				nNetworkIdx = dlg.GetNetworkIdx();
				ShowNetworkCommsLog(nNetworkIdx);

			}while( TRUE );
		}
		break;
	}
}

/**********************************************************************/

void CMainFrameEcrman::ShowNetworkCommsLog( int nNetworkIdx )
{
	CFilenameUpdater FnUp ( SysFiles::SchedLogPrm );

	CString strFilename = dbNetwork.GetFilePathSyssetSchedLogAll( nNetworkIdx );

	if ( ::FileExists ( strFilename ) == TRUE )
	{
		CReportManager ReportManager;
		ReportManager.DisplayReport ( strFilename, FnUp.GetFilenameToUse(), "", this );
	}
	else
		Prompter.Error ( "The Comms Log file was not found." );	
}

/**********************************************************************/

void CMainFrameEcrman::OnFileEmailLog() 
{
	CFilenameUpdater FnUp ( SysFiles::EmailLogPrm );

	if ( ::FileExists ( Super.EmailLogAll() ) == TRUE )
	{
		CReportManager ReportManager;
		ReportManager.DisplayReport ( Super.EmailLogAll(), FnUp.GetFilenameToUse(), "", this );
	}
	else
		Prompter.Error ( "The Email Log file was not found." );
}

/**********************************************************************/

void CMainFrameEcrman::OnFilePosTrayLogAction() 
{
	CFilenameUpdater FnUp ( SysFiles::PosTrayLogActionPrm );

	if ( ::FileExists ( Super.PosTrayLogActionAll() ) == TRUE )
	{
		CReportManager ReportManager;
		ReportManager.DisplayReport ( Super.PosTrayLogActionAll(), FnUp.GetFilenameToUse(), "", this );
	}
	else
		Prompter.Error ( "The PosTray Action Log file was not found." );
}

/**********************************************************************/

void CMainFrameEcrman::OnFilePosTrayLogExport() 
{
	CFilenameUpdater FnUp ( SysFiles::PosTrayLogExportPrm );

	if ( ::FileExists ( Super.PosTrayLogExportAll() ) == TRUE )
	{
		CReportManager ReportManager;
		ReportManager.DisplayReport ( Super.PosTrayLogExportAll(), FnUp.GetFilenameToUse(), "", this );
	}
	else
		Prompter.Error ( "The PosTray Export Log file was not found." );
}

/**********************************************************************/

void CMainFrameEcrman::OnFileFNBLog() 
{
	CFilenameUpdater FnUp ( SysFiles::FNBLogPrm );

	if ( ::FileExists ( Super.FNBLogAll() ) == TRUE )
	{
		CReportManager ReportManager;
		ReportManager.DisplayReport ( Super.FNBLogAll(), FnUp.GetFilenameToUse(), "", this );
	}
	else
		Prompter.Error ( "The FNB Export Log file was not found." );
}

/**********************************************************************/

void CMainFrameEcrman::OnFileS4LabourLog() 
{
	CFilenameUpdater FnUp ( SysFiles::S4LabourLogPrm );

	if ( ::FileExists ( Super.S4LabourLogAll() ) == TRUE )
	{
		CReportManager ReportManager;
		ReportManager.DisplayReport ( Super.S4LabourLogAll(), FnUp.GetFilenameToUse(), "", this );
	}
	else
		Prompter.Error ( "The S4 Labour Export Log file was not found." );
}

/**********************************************************************/

void CMainFrameEcrman::OnFileFTPLog() 
{
	CFilenameUpdater FnUp ( SysFiles::FTPLogPrm );

	if ( ::FileExists ( Super.FTPLogAll() ) == TRUE )
	{
		CReportManager ReportManager;
		ReportManager.DisplayReport ( Super.FTPLogAll(), FnUp.GetFilenameToUse(), "", this );
	}
	else
		Prompter.Error ( "The FTP Export Log file was not found." );
}

/**********************************************************************/

void CMainFrameEcrman::OnFileBackup() 
{
	AutoBackupProcessor.ManualBackup( this, FALSE );

	if ( TRUE == AutoBackupProcessor.GetWantQuitFlag() )
		OnClose();

	else if ( TRUE == AutoBackupProcessor.GetLoggedResultFlag() )
		ShowBackupResult( this );
}

/**********************************************************************/

void CMainFrameEcrman::OnFileBackupLog() 
{
	if ( ::FileExists ( Super.BackupLogToRead() ) == TRUE )
	{
		CReportManager ReportManager;
		ReportManager.DisplayReport ( Super.BackupLogToRead(), Super.BackupLogParams(), "", this );
	}
	else
		Prompter.Error ( "The Backup Log file was not found." );
}

/**********************************************************************/

void CMainFrameEcrman::OnFileBackupStatus()
{
	if ( m_pBackupResultDlg == NULL )
	{
		m_pBackupResultDlg = new CBackupResultDlg( &m_pBackupResultDlg, this );
		m_pBackupResultDlg -> Create( IDD_BACKUP_RESULT, this );
	}
	
	m_pBackupResultDlg -> SetFocus();
	m_pBackupResultDlg -> UpdateDisplay();
}

/**********************************************************************/

void CMainFrameEcrman::OnFileRestoreNormal()
{
	HandleFileRestore( FALSE );
}

/**********************************************************************/

void CMainFrameEcrman::OnFileRestoreFolder()
{
	HandleFileRestore( TRUE );
}

/**********************************************************************/

void CMainFrameEcrman::HandleFileRestore( bool bFolder ) 
{
	CLockManagerInfo infoLock;
	infoLock.m_bLockAllUsers = TRUE;
	infoLock.m_strLockAllUsersPrompt = "restore the system";
	infoLock.m_nCommsAction = COMMS_ACTION_PAUSE;
	infoLock.m_bStopComms = TRUE;
	infoLock.m_bStopCommsPMSExemption = FALSE;
	infoLock.m_strStopCommsPrompt = "Restore system";
	infoLock.m_bStopPosTray = TRUE;
	infoLock.m_bStopLoyalty = TRUE;
	infoLock.m_pParentWnd = this;

	CString strError = "";
	bool bAttempted = FALSE;
	bool bComplete = FALSE;
	bool bTerminatedLoyalty = FALSE;
	int nResult = 0;

	{
		CLockManager LockManager( infoLock );

		if ( LockManager.GetLock() == TRUE )
		{
			nResult = RestoreInternal( strError, bComplete, bTerminatedLoyalty, infoLock, bFolder );
			bAttempted = TRUE;
		}
	}

	if ( FALSE == bTerminatedLoyalty )
	{
		if ( infoLock.m_bLoyaltyWasRunning )
		{
			CWaitLockHelpersLoyalty LoyaltyPause;
			LoyaltyPause.CreateResumeRequest();
		}

		if ( infoLock.m_bSmartPayWasRunning )
		{
			CWaitLockHelpersSmartPay SmartPayPause;
			SmartPayPause.CreateResumeRequest();
		}

		if ( infoLock.m_bSmartBackWasRunning )
		{
			CWaitLockHelpersSmartBack SmartBackPause;
			SmartBackPause.CreateResumeRequest();
		}
	}

	if ( TRUE == bAttempted )
	{
		if ( TRUE == bComplete )
			CommsHandshaker.PauseComms();

		switch( nResult )
		{
		case 0:
			{
				for ( int n = 1; n <= POSTRAY_MAX_TASK; n++ )
				{
					CFileRemover FileRemover;

					{
						CPosTrayTask task(POSTRAY_TASKFILE_PROCESSTRAN_ALL);
						task.SetTaskNo(n);

						FileRemover.RemoveFile(task.GetFilename(TRUE));
						FileRemover.RemoveFile(task.GetFilename(FALSE));

						if (::FileExists(task.GetFilename(FALSE)) == FALSE)
						{
							FileRemover.RemoveFile(task.GetDateFilename(TRUE));
							FileRemover.RemoveFile(task.GetDateFilename(FALSE));
						}
					}

					{
						CPosTrayTask task(POSTRAY_TASKFILE_TIMEATTEND_ALL);
						task.SetTaskNo(n);

						FileRemover.RemoveFile(task.GetFilename(TRUE));
						FileRemover.RemoveFile(task.GetFilename(FALSE));

						if (::FileExists(task.GetFilename(FALSE)) == FALSE)
						{
							FileRemover.RemoveFile(task.GetDateFilename(TRUE));
							FileRemover.RemoveFile(task.GetDateFilename(FALSE));
						}
					}

					{
						CPosTrayTask task( POSTRAY_TASKFILE_EPOS_ALL );
						task.SetTaskNo(n);
						
						FileRemover.RemoveFile( task.GetFilename( TRUE ) );
						FileRemover.RemoveFile( task.GetFilename( FALSE ) );

						if ( ::FileExists( task.GetFilename( FALSE ) ) == FALSE )
						{
							FileRemover.RemoveFile( task.GetDateFilename( TRUE ) );
							FileRemover.RemoveFile( task.GetDateFilename( FALSE ) );
						}
					}

					{
						CPosTrayTask task( POSTRAY_TASKFILE_HIST_ALL );
						task.SetTaskNo(n);
						
						FileRemover.RemoveFile( task.GetFilename( TRUE ) );
						FileRemover.RemoveFile( task.GetFilename( FALSE ) );

						if ( ::FileExists( task.GetFilename( FALSE ) ) == FALSE )
						{
							FileRemover.RemoveFile( task.GetDateFilename( TRUE ) );
							FileRemover.RemoveFile( task.GetDateFilename( FALSE ) );
						}
					}

					{
						CPosTrayTask task( POSTRAY_TASKFILE_STKLEVEL_ALL );
						task.SetTaskNo(n);
						
						FileRemover.RemoveFile( task.GetFilename( TRUE ) );
						FileRemover.RemoveFile( task.GetFilename( FALSE ) );

						if ( ::FileExists( task.GetFilename( FALSE ) ) == FALSE )
						{
							FileRemover.RemoveFile( task.GetDateFilename( TRUE ) );
							FileRemover.RemoveFile( task.GetDateFilename( FALSE ) );
						}
					}
				}

				CString strMsg = "";
				strMsg += "The restore was completed successfully.";
				strMsg += "\n\nPlease restart Ecr Manager after closing this message.";
				Prompter.Info ( strMsg );
			}
			break;

		case 1:
			Prompter.Error( "Unable to find current working directory" );
			break;

		case 2:
			Prompter.Error( "Unable to change the working directory" );
			break;

		case 3:
			Prompter.Error ( strError );
			break;

		case 4:
			Prompter.Error( "The restore folder already exists" );
			break;
		}
	
		if ( TRUE == bComplete )
		{
			SysInfo.Read();
			PostMessage ( WM_CLOSE );
		}
	}
}

/**********************************************************************/

int CMainFrameEcrman::RestoreInternal( CString& strError, bool& bComplete, bool& bTerminatedLoyalty, CLockManagerInfo& infoLock, bool bRestoreToFolder )
{
	strError = "";
	bComplete = FALSE;
	bTerminatedLoyalty = FALSE;

	CString strCwd = "";
	CString strDataPath = Sysset.GetDataPath();

	if ( TRUE == bRestoreToFolder )
	{
		CString strDateTime;
		COleDateTime timeNow = COleDateTime::GetCurrentTime();

		strDateTime.Format( "RESTORED_%4.4d%2.2d%2.2d_%2.2d%2.2d%2.2d",
			timeNow.GetYear(),
			timeNow.GetMonth(),
			timeNow.GetDay(),
			timeNow.GetHour(),
			timeNow.GetMinute(),
			timeNow.GetSecond() );

		strDataPath += strDateTime;

		if ( ::ExistSubdirectory( strDataPath ) == TRUE )
			return 4;

		::CreateSubdirectory( strDataPath );
	}

	if( strDataPath != "" )
	{
		char cBuffer[ _MAX_PATH ];
		if ( _getcwd( cBuffer, _MAX_PATH ) == NULL )
			return 1;

		strCwd = cBuffer;
	
		if ( _chdir( strDataPath ) != 0 )
			return 2;
	}
	
	StatusProgress.Lock();
	Prompter.SetPreventParentEnableFlag( TRUE );
	CSystemBackup MyBackup ( FALSE, bRestoreToFolder, infoLock );
	Prompter.SetPreventParentEnableFlag( FALSE );
	StatusProgress.Unlock();
	strError = MyBackup.GetOnScreenError();
	bComplete = MyBackup.IsComplete();
	bTerminatedLoyalty = MyBackup.TerminatedLoyalty();
		
	if ( strDataPath != "" )
		_chdir( strCwd );
	
	if ( strError != "" )
		return 3;

	return 0;	
}

/**********************************************************************/

BOOL CMainFrameEcrman::OnQueryEndSession() 
{
	if (!CFrameWnd::OnQueryEndSession())
		return FALSE;

	CMessageLoggerInfo info;
	info.m_strMessage = "Displayed Log off prompt";
	info.m_nHideLevel = 2;

	MessageLogger.LogSchedulerMessage( info, FALSE, TRUE, FALSE );

	CString strMsg = "Ecr Manager has detected a request to log off or shut down Windows.\n\n";
	strMsg += "You will lose any unsaved data if you log off or shut down Windows at this time.\n\n";
	strMsg += "Do you wish to log off or shut down Windows and lose any unsaved data.";

	if ( Prompter.YesNo( strMsg, FALSE ) != IDYES )
	{
		info.m_strMessage = "Selected NO at Log off prompt";
		MessageLogger.LogSchedulerMessage( info, FALSE, TRUE, FALSE );
		return FALSE;
	}

	info.m_strMessage = "Selected YES at Log off prompt";
	MessageLogger.LogSchedulerMessage( info, FALSE, TRUE, FALSE );
	
	return TRUE;
}

/**********************************************************************/

void CMainFrameEcrman::OnEndSession(BOOL bEnding) 
{
	CMessageLoggerInfo info;
	info.m_nHideLevel = 2;

	if ( bEnding == FALSE )
		info.m_strMessage = "Log off was cancelled";
	else
		info.m_strMessage = "Log off was confirmed";

	MessageLogger.LogSchedulerMessage( info, FALSE, TRUE, FALSE );
	
	CFrameWnd::OnEndSession(bEnding);
}

/**********************************************************************/

void CMainFrameEcrman::OnAppExit() 
{
	OnClose();
}

/**********************************************************************/

void CMainFrameEcrman::OnImportSQL()
{
	if ( DataManager.LockAllUsers() == FALSE )
		Prompter.LockError( "import SQL data" );
	else
	{
		int nResult = ImportSQLInternal();
		DataManager.UnlockAllUsers();

		switch( nResult )
		{
		case 1:
			Prompter.Error ( "You must set up at least one database\nbefore you can import Wizard data." );
			break;

		case 2:
			Prompter.Error( "You must set up at least one network\nbefore you can import Wizard data." );
			break;

		case 3:
			Prompter.Error( "Unable to open import file." );
			break;

		case 4:
			Prompter.Error( "No Wizard data found in file." );
			break;

		case 5:
			Prompter.Error( "Wizard data was imported as requested." );
			break;
		}
	}
}

/**********************************************************************/

int CMainFrameEcrman::ImportSQLInternal()
{
	if (dbDatabase.GetSize() == 0)
	{
		return 1;
	}

	if (dbNetwork.GetSize() == 0)
	{
		return 2;
	}

	CFileDialog dlgFile(TRUE, "sql", "database.sql", OFN_NOCHANGEDIR | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, "SQL Import Files (*.sql)|*.sql||", NULL);
	dlgFile.m_ofn.lpstrTitle = "Import Data Transfer Wizard File";
	dlgFile.m_ofn.lpstrInitialDir = ".\\";

	if (dlgFile.DoModal() == IDCANCEL)
	{
		return 0;
	}

	CSQLImporter SQLImporter( dlgFile.GetPathName() );

	bool bMixMatch = EcrmanOptions.GetFeaturesMixMatchFlag();
	bool bWalletStampRule = EcrmanOptions.GetFeaturesWalletStampRuleFlag();
	bool bTimePlan = EcrmanOptions.GetFeaturesTimePlanFlag();

	SQLImporter.SetWantMixMatchFlag( bMixMatch );
	SQLImporter.SetWantWalletStampRuleFlag(bWalletStampRule);
	SQLImporter.SetWantTimePlanFlag( bMixMatch && bTimePlan );

	SQLImporter.ImportFile( FALSE );

	if (SQLImporter.GotFile() == FALSE)
	{
		return 3;
	}
	
	if (SQLImporter.GotData() == FALSE)
	{
		return 4;
	}
	
	if (SQLImporter.LegacyWarning() == FALSE)
	{
		return 0;
	}

	CPropertySheetWizardImport propSheet( SQLImporter, this );
	if (propSheet.DoModal() == IDOK)
	{
		propSheet.ImportWizardData();
	}
	
	if (propSheet.GetImportedDataFlag() == TRUE)
	{
		return 5;
	}

	return 0;
}

/**********************************************************************/

void CMainFrameEcrman::OnClose() 
{
	if ( DatabaseValidator.GetDoingUpdateFlag() == TRUE )
		return;

	CommsHandshaker.PauseComms();
	
	if ( DatabaseValidator.GetErrorFlag() == TRUE )
		Prompter.ContactDealer( "Ecr Manager has detected an invalid database and needs to shut down.", TRUE );
	else
	{
		if ( BUTMGR_MODE_EDIT_LIST == ButtonManager.GetButtonManagerMode() )
		{
			if ( ButtonManager.GetButtonEditDlg() != NULL )
			{
				delete( ButtonManager.GetButtonEditDlg() );
				ButtonManager.SetButtonEditDlg( NULL );
			}

			if ( ButtonManager.CheckButtonLibraryChanges() == TRUE )
				if ( Prompter.YesNo( "Button lists have been modified - save changes", TRUE ) == IDYES )
				{
					ButtonManager.TempImageModeOff( TRUE );
					ButtonManager.WriteButtonActions();
				}
		}
	}

	StatusProgress.DestroyWindow();
	CommsHandshaker.PauseComms();
	CommsHandshaker.TerminateComms();
	CFrameWnd::OnClose();
}

/**********************************************************************/

long CMainFrameEcrman::OnIDraughtExport( WPARAM wIndex, LPARAM lParam )
{
	return IDraughtExportManager.HandleIDraughtExport( this );
}

/**********************************************************************/

long CMainFrameEcrman::OnIDraughtTransfer( WPARAM wIndex, LPARAM lParam )
{
	return IDraughtExportManager.HandleIDraughtTransfer( this );
}

/**********************************************************************/

long CMainFrameEcrman::OnSageExport( WPARAM wIndex, LPARAM lParam )
{
	return SageExportManager.HandleSageExport( this );
}

/**********************************************************************/

void CMainFrameEcrman::OnFileSageLog() 
{
	CFilenameUpdater FnUp ( SysFiles::SageLogPrm );

	if ( ::FileExists ( Super.SageLogAll() ) == TRUE )
	{
		CReportManager ReportManager;
		ReportManager.DisplayReport ( Super.SageLogAll(), FnUp.GetFilenameToUse(), "", this );
	}
	else
		Prompter.Error ( "The Sage Export Log file was not found." );
}

/**********************************************************************/

void CMainFrameEcrman::OnFileWriteLog() 
{
	if ( ::FileExists ( Super.FileWriteLogNew() ) == TRUE )
	{
		CFileWriteLog FileWriteLog;

		CReportManager ReportManager;
		CFilenameUpdater FnUp ( SysFiles::FileChangesLogPrm );
		ReportManager.DisplayReport ( Super.ReportFilename(), FnUp.GetFilenameToUse(), "", this );
	}
	else
		Prompter.Error ( "The File Changes Log was not found." );
}

/**********************************************************************/
