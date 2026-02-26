/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "..\SmartPaySQL\SQLConnectionOptions.h"
#include "..\SmartPaySQL\SQLDb.h"
/**********************************************************************/
#include "..\CommonEcrStockTray\AutoBackupOptions.h"
#include "..\CommonEcrStockTray\AutoBackupProcessor.h"
#include "..\CommonEcrStockTray\ChartwellsOptions.h"
/**********************************************************************/
#include "AboutDlgPosTray.h"
#include "CommsHandshaker.h"
#include "Consolidation.h"
#include "DateRangeHelpers.h"
#include "EmailOptions.h"
#include "FileRemover.h"
#include "FNBDatabaseOptions.h"
#include "LIPAExportDate.h"
#include "LocationCSVArray.h"
#include "LocationMasterMap.h"
#include "MessageLogger.h"
#include "NetworkCSVArray.h"
#include "NetworkResources.h"
#include "PCOptions.h"
#include "PMSCheckoutDateArray.h"
#include "PosTrayApp.h"
#include "PosTrayLIPAExportCreator.h"
#include "PosTrayTask.h"
#include "PosTrayExportCreatorDiscount.h"
#include "PosTrayExportCreatorPayment.h"
#include "PosTrayExportCreatorS4Labour.h"
#include "PosTrayExportCreatorSales.h"
#include "PosTrayExportCreatorVoid.h"
#include "PosTrayReportCreatorEpos.h"
#include "PosTrayReportCreatorHistory.h"
#include "PosTrayReportCreatorTimeAttend.h"
#include "PosTrayReportCreatorStkLevelEcr.h"
#include "PosTrayChartwellsExportCreator.h"
#include "PosTrayFNBExportCreator.h"
#include "PosTrayFTPSender.h"
#include "PosTrayLock.h"
#include "PosTrayOptions.h"
#include "PosTrayStatus.h"
#include "PosTrayTokenFiles.h"
#include "ReportConsolidationArray.h"
#include "RepSpawn.h"
#include "SysInfo.h"
#include "SystemMap.h"
/**********************************************************************/
#include "MainFrmPosTray.h"
/**********************************************************************/
#define WM_MY_TRAY_NOTIFICATION WM_USER+0
/**********************************************************************/
static const char* szRunFilename = "SPosTray.run";
/**********************************************************************/
static const int TASK_CHECK_PRODDER = 1;
/**********************************************************************/

IMPLEMENT_DYNAMIC(CMainFramePosTray, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFramePosTray, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFramePosTray)
	ON_WM_CREATE()
	ON_COMMAND(IDM_TRAY_CLOSE, OnTrayClose)
	ON_COMMAND(IDM_TRAY_HELP, OnTrayHelp)
	ON_WM_ENDSESSION()
	ON_WM_TIMER()
	ON_COMMAND(IDM_TRAY_STATUS, OnTrayStatus)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MY_TRAY_NOTIFICATION, OnTrayNotification)
	ON_MESSAGE(WM_APP, OnStartPosTray)
END_MESSAGE_MAP()

/**********************************************************************/

#ifdef POSTRAY_SOCKET
CMainFramePosTray::CMainFramePosTray() : CFrameWnd(), m_trayIcon ( IDR_MAINFRAME ), m_SocketServer( 10, 1000 )
#else
CMainFramePosTray::CMainFramePosTray() : CFrameWnd(), m_trayIcon(IDR_MAINFRAME)
#endif
{
	m_pStatusDlg = NULL;

	m_strMiscFolder = "SysFiles";
	GetSyssetProgramPath(m_strMiscFolder);
	::CreateSubdirectory(m_strMiscFolder);

	m_strFilePathRunOnce.Format("%s\\%d.run",
		(const char*)m_strMiscFolder,
		Sysset.GetPCIdHost());

	m_strFilePathReport.Format("%s\\%d.rep",
		(const char*)m_strMiscFolder,
		Sysset.GetPCIdHost());

	m_strFilePathParams.Format("%s\\%d.prm",
		(const char*)m_strMiscFolder,
		Sysset.GetPCIdHost());

	m_bUIBusy = FALSE;

	m_bReadyToAct = FALSE;
	m_bReadyToActSalesImporter = FALSE;
	m_bReadyToActEmail = FALSE;
	m_bReadyToActPrint = FALSE;
	m_bReadyToActExport = FALSE;
	m_bReadyToActFNBExport = FALSE;
	m_bReadyToActChartwellsExport = FALSE;
	m_bReadyToActS4LabourExport = FALSE;
	m_bReadyToActFTPExport = FALSE;
	m_bReadyToActBackup = FALSE;
	m_bReadyToActStatus = FALSE;

	m_bWantQuit = FALSE;
	m_bBlockHibernation = FALSE;
	m_bTryFNBExportFirst = FALSE;

	SetStatus(POSUTILITY_STATUS_NOT_CHECKED);
	UpdateTime(m_NextCheckTime, 0, 0, FALSE);
	UpdateTime(m_NextEmailTime, 0, 0, FALSE);
	UpdateTime(m_NextPrintTime, 0, 0, FALSE);
	UpdateTime(m_NextExportTime, 0, 0, FALSE);
	UpdateTime(m_NextStatusTime, 0, 0, TRUE);
	UpdateTime(m_CurrentStatusTime, 0, 0, TRUE);
	UpdateTime(m_NextFNBExportTime, 0, 0, TRUE);
	UpdateTime(m_NextChartwellsExportTime, 0, 0, TRUE);
	UpdateTime(m_NextS4LabourExportTime, 0, 0, TRUE);
	UpdateTime(m_NextBackupTime, 0, 0, TRUE);
	UpdateTime(m_NextFTPExportTime, 0, 0, TRUE);

	m_nEmailTaskToCheck = 0;
	m_nEmailTaskToPerform = 0;

	m_nFNBExportToCheck = 0;
	m_nFNBExportToPerform = 0;
	m_nChartwellsExportToPerform = 0;
	m_nS4LabourExportToCheck = 0;
	m_nS4LabourExportToPerform = 0;

	if (SysInfo.IsStockSystem())
	{
		m_strProgramName = "Stock Manager";
	}
	else
	{
		m_strProgramName = "Ecr Manager";
	}

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

/**********************************************************************/

CMainFramePosTray::~CMainFramePosTray()
{
	if (m_pStatusDlg != NULL)
	{
		delete m_pStatusDlg;
	}
}

/**********************************************************************/

int CMainFramePosTray::OnCreate ( LPCREATESTRUCT lpCreateStruct )
{
	if ( fileRunOnce.Open ( m_strFilePathRunOnce, "wb", _SH_DENYRW ) == TRUE ) 
	{
		if ( CFrameWnd::OnCreate(lpCreateStruct) != -1 )
		{
			m_trayIcon.SetNotificationWnd ( this, WM_MY_TRAY_NOTIFICATION );
			m_trayIcon.SetIcon ( IDR_MAINFRAME );
			
			InitialiseSystem();
			
			m_hTimer = SetTimer( TASK_CHECK_PRODDER, 5000, NULL );
			PostMessage ( WM_APP, 0, 0 );

#ifdef POSTRAY_SOCKET
			if ( Super.GetPosTraySocketFlag() == TRUE )
			{
				m_SocketServer.EnableLogging( ".\\PosTraySocketLog.txt", TRUE );
				m_SocketServer.StartListening(4001);
			}
#endif

			return 0;
		}
	}
	return -1;
}

/**********************************************************************/

void CMainFramePosTray::OnEndSession(BOOL bEnding)
{
	CFrameWnd::OnEndSession(bEnding);
	if (bEnding == TRUE)
	{
		CloseTray(FALSE);
	}
}

/**********************************************************************/

LRESULT CMainFramePosTray::OnTrayNotification(WPARAM uID, LPARAM lEvent)
{
	return m_trayIcon.OnTrayNotification ( uID, lEvent );
}

/**********************************************************************/

void CMainFramePosTray::OnTrayHelp() 
{
	if ( m_bUIBusy == FALSE )
	{
		m_bUIBusy = TRUE;
		CAboutDlgPosTray dlg ( this );
		dlg.DoModal();
		m_bUIBusy = FALSE;
	}
}

/**********************************************************************/

long CMainFramePosTray::OnStartPosTray( WPARAM wIndex, LPARAM lParam )
{
	while( m_bWantQuit == FALSE )
	{
		m_bReadyToAct = FALSE;
		m_bReadyToActSalesImporter = FALSE;
		m_bReadyToActEmail = FALSE;
		m_bReadyToActPrint = FALSE;
		m_bReadyToActExport = FALSE;
		m_bReadyToActFNBExport = FALSE;
		m_bReadyToActChartwellsExport = FALSE;
		m_bReadyToActS4LabourExport = FALSE;
		m_bReadyToActFTPExport = FALSE;
		m_bReadyToActBackup = FALSE;
		m_bReadyToActStatus = FALSE;

		WaitExit( m_bReadyToAct, 10 );

		CFilenameUpdater FnUp( SysFiles::SchedulerQuit );
		if (::FileExists(FnUp.GetFilenameToUse()))
		{
			m_bWantQuit = TRUE;
		}

		if ( m_bWantQuit == FALSE )
		{
			if ( TRUE == m_bReadyToActSalesImporter )
			{
				switch( m_nPosTrayStatus )
				{
				case POSUTILITY_STATUS_NO_SYSTEM:
				case POSUTILITY_STATUS_NOT_CHECKED:
					CommsHandshaker.PauseComms();
					SetStatus( POSUTILITY_STATUS_WAIT_INITSYSTEM );
					UpdateTime( m_NextCheckTime, 0, 5, FALSE );
					break;

				case POSUTILITY_STATUS_WAIT_INITSYSTEM:
					if ( CommsHandshaker.CheckStoppedForPosTray() == TRUE )
					{
						UpdateStatus();
						UpdateTime( m_NextCheckTime, 0, 5, FALSE );
					}	
					break;

				case POSUTILITY_STATUS_WAIT_TASK:
				case POSUTILITY_STATUS_WAIT_FNB:
				case POSUTILITY_STATUS_WAIT_S4LABOUR:
				case POSUTILITY_STATUS_WAIT_LIPA:
				case POSUTILITY_STATUS_WAIT_BACKUP:
				case POSUTILITY_STATUS_WAIT_CHARTWELLS:
					if ( CommsHandshaker.CheckStoppedForPosTray() == TRUE )
					{
						UpdateStatus();
						UpdateTime( m_NextCheckTime, 0, 5, FALSE );
					}	
					break;

				case POSUTILITY_STATUS_GOT_SYSTEM:
					UpdateStatus();
					UpdateTime( m_NextCheckTime, 0, 5, FALSE );
					break;

				case POSUTILITY_STATUS_CHECKING:
				case POSUTILITY_STATUS_UPDATING:
					UpdateTime( m_NextCheckTime, 0, 5, FALSE );
					break;
				}
			}

			if ( TRUE == m_bReadyToActEmail )
			{
				if (SysInfo.IsPosTrayNoTaskSystem() == FALSE)
				{
					CString strExe = ".\\SPosTray.Exe";
					CString strStockmanFlag = (SysInfo.IsStockSystem()) ? "1" : "0";
					CString strSMTPFlag = (EmailOptions.GetEmailMethodToUse() != 0) ? "1" : "0";

					CString strDataPathNo;
					strDataPathNo.Format("%d", SysInfo.GetDataPathNo());

					SSspawnl(GetSafeOwner(this), P_NOWAIT, strExe, strExe,
						"302",
						strStockmanFlag,
						strSMTPFlag,
						strDataPathNo,
						NULL);
				}
				
				UpdateTime( m_NextEmailTime, 0, 30, FALSE );
			}

			if ( ( TRUE == m_bReadyToActFNBExport ) && ( PosTrayOptions.GetFNBTaskFlag() == TRUE ) )
			{	
				if (SysInfo.IsPosTrayNoTaskSystem() == FALSE)
				{
					CString strExe = ".\\SPosTray.Exe";
					CString strStockmanFlag = (SysInfo.IsStockSystem()) ? "1" : "0";

					CString strDataPathNo;
					strDataPathNo.Format("%d", SysInfo.GetDataPathNo());

					SSspawnl(GetSafeOwner(this), P_NOWAIT, strExe, strExe,
						"401",
						strStockmanFlag,
						strDataPathNo,
						NULL);
				}

				UpdateTime( m_NextFNBExportTime, 0, 30, FALSE );
			}

			if ((TRUE == m_bReadyToActChartwellsExport) && (PosTrayOptions.GetChartwellsTaskFlag() == TRUE))
			{
				if (SysInfo.IsPosTrayNoTaskSystem() == FALSE)
				{
					CString strExe = ".\\SPosTray.Exe";
					CString strStockmanFlag = (SysInfo.IsStockSystem()) ? "1" : "0";

					CString strDataPathNo;
					strDataPathNo.Format("%d", SysInfo.GetDataPathNo());

					SSspawnl(GetSafeOwner(this), P_NOWAIT, strExe, strExe,
						"902",
						strStockmanFlag,
						strDataPathNo,
						NULL);
				}

				UpdateTime(m_NextChartwellsExportTime, 0, 30, FALSE);
			}

			if ((TRUE == m_bReadyToActBackup) && (PosTrayOptions.GetManageBackupFlag() == TRUE))
			{
				if (SysInfo.IsPosTrayNoTaskSystem() == FALSE)
				{
					bool bCanTryBackup = TRUE;
					
					if (PCOptionsHost.Read() == FALSE)
					{
						AutoBackupProcessor.LogPosTrayBackupMessage("Unable to check PC host options");
						bCanTryBackup = FALSE;
					}
					else if (PCOptionsHost.GetBackupAccessType() != 0)
					{
						AutoBackupProcessor.LogPosTrayBackupMessage("Auto backup not enabled at this PC");
						bCanTryBackup = FALSE;
					}
					if (AutoBackupOptions.Read() == FALSE)
					{
						AutoBackupProcessor.LogPosTrayBackupMessage("Unable to check backup schedule");
						bCanTryBackup = FALSE;
					}

					AutoBackupProcessor.ResetNextAttemptTime();
					AutoBackupProcessor.SetReadyToGoFlag(TRUE);
					
					if ( AutoBackupProcessor.CheckNextAttemptTime() == FALSE )
					{
						COleDateTime oleNextAttempt = AutoBackupProcessor.GetNextAttemptTime();
						
						CString strMsg = "";
						strMsg.Format("Next backup attempt due %2.2d:%2.2d:%2.2d on %2.2d/%2.2d/%4.4d",
							oleNextAttempt.GetHour(),
							oleNextAttempt.GetMinute(),
							oleNextAttempt.GetSecond(),
							oleNextAttempt.GetDay(),
							oleNextAttempt.GetMonth(),
							oleNextAttempt.GetYear());

						AutoBackupProcessor.LogPosTrayBackupMessage(strMsg);
						bCanTryBackup = FALSE;
					}

					if (TRUE == bCanTryBackup)
					{
						CFilenameUpdater FnUp(SysFiles::PosTrayBackupToken);
						CSharedStringArray arrayDummy;

						//INTENTIONAL QUICK CHECK FOR BACKUP TOKEN FILE LOCK
						//IF THE FIE IS LOCKED THEN ANOTHER BACKUP IS IN PROGRESS
						if (arrayDummy.Open(FnUp.GetFilenameToUse(), DB_READWRITE) == DB_ERR_NONE)
						{
							arrayDummy.Close();

							AutoBackupProcessor.LogPosTrayBackupMessage("Requesting Backup Attempt");

							CString strExe = ".\\SPosTray.Exe";
							CString strStockmanFlag = (SysInfo.IsStockSystem()) ? "1" : "0";

							CString strDataPathNo;
							strDataPathNo.Format("%d", SysInfo.GetDataPathNo());

							SSspawnl(GetSafeOwner(this), P_NOWAIT, strExe, strExe,
								"1001",
								strStockmanFlag,
								strDataPathNo,
								NULL);
						}
					}
				}

				UpdateTime(m_NextBackupTime, 0, 30, FALSE);
			}

			if ( ( TRUE == m_bReadyToActS4LabourExport ) && ( PosTrayOptions.GetS4LabourTaskFlag() == TRUE ) )
			{	
				if (SysInfo.IsPosTrayNoTaskSystem() == FALSE)
				{
					CString strExe = ".\\SPosTray.Exe";
					CString strStockmanFlag = (SysInfo.IsStockSystem()) ? "1" : "0";

					CString strDataPathNo = "";
					strDataPathNo.Format("%d", SysInfo.GetDataPathNo());

					SSspawnl(GetSafeOwner(this), P_NOWAIT, strExe, strExe,
						"701",
						strStockmanFlag,
						strDataPathNo,
						NULL);
				}

				UpdateTime( m_NextS4LabourExportTime, 0, 30, FALSE );
			}

			if ( ( TRUE == m_bReadyToActFTPExport ) && ( PosTrayOptions.GotFTPSettings() == TRUE ) )
			{	
				if (SysInfo.IsPosTrayNoTaskSystem() == FALSE)
				{
					CString strExe = ".\\SPosTray.Exe";
					CString strStockmanFlag = (SysInfo.IsStockSystem()) ? "1" : "0";

					CString strDataPathNo;
					strDataPathNo.Format("%d", SysInfo.GetDataPathNo());

					SSspawnl(GetSafeOwner(this), P_NOWAIT, strExe, strExe,
						"601",
						strStockmanFlag,
						strDataPathNo,
						NULL);
				}

				UpdateTime( m_NextFTPExportTime, 0, 30, FALSE );
			}

			if ( TRUE == m_bReadyToActPrint )
			{	
				if (SysInfo.IsPosTrayNoTaskSystem() == FALSE)
				{
					CString strExe = ".\\SPosTray.Exe";
					CString strStockmanFlag = (SysInfo.IsStockSystem()) ? "1" : "0";

					CString strDataPathNo;
					strDataPathNo.Format("%d", SysInfo.GetDataPathNo());

					SSspawnl(GetSafeOwner(this), P_NOWAIT, strExe, strExe,
						"501",
						strStockmanFlag,
						strDataPathNo,
						NULL);
				}

				UpdateTime( m_NextPrintTime, 0, 30, FALSE );
			}

			if ( TRUE == m_bReadyToActExport )
			{	
				if (SysInfo.IsPosTrayNoTaskSystem() == FALSE)
				{
					CString strExe = ".\\SPosTray.Exe";
					CString strStockmanFlag = (SysInfo.IsStockSystem()) ? "1" : "0";

					CString strDataPathNo;
					strDataPathNo.Format("%d", SysInfo.GetDataPathNo());

					SSspawnl(GetSafeOwner(this), P_NOWAIT, strExe, strExe,
						"801",
						strStockmanFlag,
						strDataPathNo,
						NULL);
				}

				UpdateTime( m_NextExportTime, 0, 30, FALSE );
			}
		
			if ( TRUE == m_bReadyToActStatus )
			{
				if ( m_pStatusDlg != NULL )
				{	
					UpdateNetworkList();	
					//UpdateTaskList();		
					UpdateGeneralList();
				}

				UpdateTime( m_NextStatusTime, 0, 5, TRUE );
			}
		}
	}

	if ( m_hTimer > 0 )
	{
		KillTimer( m_hTimer );
		m_hTimer = 0;
	}
	
	fileRunOnce.Close();

	CFileRemover FileRemover;
	FileRemover.RemoveFile( m_strFilePathRunOnce );

	CFrameWnd::OnClose();
	return 0L;
}

/**********************************************************************/

void CMainFramePosTray::SetStatus( int nNewStatus )
{
	m_nPosTrayStatus = nNewStatus;

#ifdef _DEBUG

	switch( m_nPosTrayStatus )
	{
	case POSUTILITY_STATUS_NOT_CHECKED:
		TRACE ( "PosTray Status = Not Checked \n" );
		break;
			
	case POSUTILITY_STATUS_CHECKING:
		TRACE ( "PosTray Status = Checking \n" );
		break;

	case POSUTILITY_STATUS_NO_SYSTEM:
		TRACE ( "PosTray Status = No System \n" );
		break;

	case POSUTILITY_STATUS_GOT_SYSTEM:
		TRACE ( "PosTray Status = Got System \n" );
		break;

	case POSUTILITY_STATUS_UPDATING:
		TRACE ( "PosTray Status = Updating \n" );
		break;

	case POSUTILITY_STATUS_NOT_REGISTERED:
		TRACE ( "PosTray Status = Not Registered \n" );
		break;

	case POSUTILITY_STATUS_WAIT_INITSYSTEM:
		TRACE ( "PosTray Status = Wait Init System \n" );
		break;

	case POSUTILITY_STATUS_WAIT_TASK:
		TRACE ( "PosTray Status = Wait Task \n" );
		break;

	case POSUTILITY_STATUS_WAIT_FNB:
		TRACE ( "PosTray Status = Wait FNB \n" );
		break;

	case POSUTILITY_STATUS_WAIT_CHARTWELLS:
		TRACE("PosTray Status = Wait Chartwells \n");
		break;

	case POSUTILITY_STATUS_WAIT_S4LABOUR:
		TRACE ( "PosTray Status = Wait S4 Labour \n" );
		break;

	case POSUTILITY_STATUS_WAIT_LIPA:
		TRACE ( "PosTray Status = Wait LIPA \n" );
		break;

	case POSUTILITY_STATUS_WAIT_BACKUP:
		TRACE("PosTray Status = Wait Backup \n");
		break;

	default:
		{
			CString str;
			str.Format( "PosTray Status = Unknown (%d) \n", m_nPosTrayStatus );
			TRACE( str );
		}
		break;
	}

#endif
}

/**********************************************************************/

bool CMainFramePosTray::UpdateStatusEmailTask()
{
	bool bGotWork = FALSE;

	if (m_arrayEmailTasks.GetSize() == 0)
	{
		return FALSE;
	}

	if ((m_nEmailTaskToCheck < 0) || (m_nEmailTaskToCheck >= m_arrayEmailTasks.GetSize()))
	{
		m_nEmailTaskToCheck = 0;
	}

	int nStartTask = m_nEmailTaskToCheck;

	do
	{
		CPosTrayEmailScheduleInfo infoTask = m_arrayEmailTasks[m_nEmailTaskToCheck];

		CPosTrayTask task(infoTask.GetFileType());

		task.SetTaskNo(infoTask.m_nTaskNo);
		CString strFilenameTask = task.GetFilename(FALSE);

		switch (infoTask.m_nTaskStatus)
		{
		case EMAILTASK_STATUS_UNCHECKED:
		case EMAILTASK_STATUS_NOFILE:
		case EMAILTASK_STATUS_WAIT_UPDATE:
			if (::FileExists(strFilenameTask) == FALSE)
			{
				infoTask.m_nTaskStatus = EMAILTASK_STATUS_NOFILE;
			}
			else if (task.Read(FALSE) == TRUE)
			{
				CFileStatus FileStatus;
				CFile::GetStatus(strFilenameTask, FileStatus);

				if (task.GetMergeDeleteFlag() == TRUE)
				{
					infoTask.m_nTaskStatus = EMAILTASK_STATUS_NOTASK;
					infoTask.m_nTaskError = 0;
					infoTask.m_timeTaskFile = FileStatus.m_mtime;
				}
				else
				{
					int nResult = task.ValidateTask();

					COleDateTime oleTimeNow = COleDateTime::GetCurrentTime();

					if (0 != nResult)
					{
						infoTask.m_strTaskName = task.GetTaskName();
						infoTask.m_bWantEmail = FALSE;
						infoTask.m_bWantExport = FALSE;
						infoTask.m_bWantPrint = (task.GetTaskAutoPrintFlag() == TRUE);
						infoTask.m_nTaskStatus = EMAILTASK_STATUS_INVALID;
						infoTask.m_nTaskError = nResult;
						infoTask.m_timeTaskFile = FileStatus.m_mtime;
						infoTask.m_timeTaskChecked = oleTimeNow;

						MessageLogger.LogPosTraySystem(infoTask, task, "Check Task", "INVALID");
					}
					else
					{
						infoTask.m_strTaskName = task.GetTaskName();
						infoTask.m_bWantEmail = (task.GetTaskFormatEmail() != -1);
						infoTask.m_bWantExport = (task.GetTaskFormatExport() != -1);
						infoTask.m_bWantPrint = (task.GetTaskAutoPrintFlag() == TRUE);
						infoTask.m_nTaskStatus = EMAILTASK_STATUS_WAIT_TASK;
						infoTask.m_nTaskError = 0;
						infoTask.m_timeTaskFile = FileStatus.m_mtime;
						infoTask.m_timeTaskChecked = oleTimeNow;
						CString strLastRunTime = task.GetLastRunTime(FALSE);
						task.GetNextRunTime(infoTask.m_timeTaskDue, strLastRunTime, oleTimeNow);

						MessageLogger.LogPosTraySystem(infoTask, task, "Check Task", "VALID");
					}
				}
			}
			break;

		case EMAILTASK_STATUS_INVALID:
		case EMAILTASK_STATUS_NOTASK:
		case EMAILTASK_STATUS_WAIT_TASK:
		case EMAILTASK_STATUS_WAIT_CASHRSP:
		case EMAILTASK_STATUS_WAIT_RETRYFOLDER:
			if (::FileExists(strFilenameTask) == FALSE)
			{
				infoTask.m_nTaskStatus = EMAILTASK_STATUS_NOFILE;
			}
			else
			{
				CFileStatus FileStatus;
				CFile::GetStatus(strFilenameTask, FileStatus);

				if (FileStatus.m_mtime != infoTask.m_timeTaskFile)
				{
					MessageLogger.LogPosTraySystem(infoTask, task, "Task File Changed", "(main loop)");
					infoTask.m_nTaskStatus = EMAILTASK_STATUS_WAIT_UPDATE;
				}
				else if (EMAILTASK_STATUS_WAIT_RETRYFOLDER == infoTask.m_nTaskStatus)
				{
					if (COleDateTime::GetCurrentTime() >= infoTask.m_timeTaskRetryFolder)
					{
						infoTask.m_nTaskStatus = EMAILTASK_STATUS_WAIT_TASK;
					}
				}
			}
			break;
		}

		switch (infoTask.m_nTaskStatus)
		{
		case EMAILTASK_STATUS_WAIT_TASK:
		case EMAILTASK_STATUS_WAIT_CASHRSP:
			if (CPosTrayTokenFiles::StopFileExists() == FALSE)
			{
				COleDateTime timeNow = COleDateTime::GetCurrentTime();

				{
					if (1 == infoTask.m_nRunNowStatus)
					{
						infoTask.m_timeTaskDue = timeNow;
						infoTask.m_nRunNowStatus = 2;
					}
				}

				if (timeNow >= infoTask.m_timeTaskDue)
				{
					if (m_CashRSPBuffer.IsDateTimeCleared(infoTask.m_timeTaskDue) == TRUE)
					{
						m_nEmailTaskToPerform = m_nEmailTaskToCheck;
						SetStatus(POSUTILITY_STATUS_WAIT_TASK);
						CommsHandshaker.PauseComms();
						bGotWork = TRUE;
					}
					else
					{
						infoTask.m_nTaskStatus = EMAILTASK_STATUS_WAIT_CASHRSP;
						m_CashRSPBuffer.SetPendingDateTime(infoTask.m_timeTaskDue);
					}
				}
			}
			break;
		}

		m_arrayEmailTasks[m_nEmailTaskToCheck] = infoTask;
		UpdateTaskList();

		m_nEmailTaskToCheck++;
		if (m_nEmailTaskToCheck >= m_arrayEmailTasks.GetSize())
		{
			m_nEmailTaskToCheck = 0;
		}

	} while ((m_nEmailTaskToCheck != nStartTask) && (FALSE == bGotWork));

	return bGotWork;
}

/**********************************************************************/

bool CMainFramePosTray::UpdateStatusFNBExport()
{
	bool bGotWork = FALSE;

	if (PosTrayOptions.GetFNBTaskFlag() == FALSE)
	{
		return FALSE;
	}

	if (m_arrayFNBExports.GetSize() == 0)
	{
		return FALSE;
	}

	if ((m_nFNBExportToCheck < 0) || (m_nFNBExportToCheck >= m_arrayFNBExports.GetSize()))
	{
		m_nFNBExportToCheck = 0;
	}

	int nStartExport = m_nFNBExportToCheck;
	
	do
	{
		CPosTrayFNBScheduleInfo infoExport = m_arrayFNBExports[ m_nFNBExportToCheck ];
		
		CFNBDatabaseOptions FNBOptions;
		CString strFilenameOptions = FNBOptions.GetFilename( infoExport.m_nDbIdx, FALSE );

		switch( infoExport.m_nExportStatus )
		{
		case BESPOKE_EXPORT_STATUS_UNCHECKED:
		case BESPOKE_EXPORT_STATUS_NOFILE:
		case BESPOKE_EXPORT_STATUS_WAIT_UPDATE:
			if (::FileExists(strFilenameOptions) == FALSE)
			{
				infoExport.m_nExportStatus = BESPOKE_EXPORT_STATUS_NOFILE;
			}
			else if ( FNBOptions.Read( infoExport.m_nDbIdx, FALSE ) == TRUE )
			{
				CFileStatus FileStatus;
				CFile::GetStatus( strFilenameOptions, FileStatus );

				if ( FNBOptions.GetEnable() == FALSE )
				{
					infoExport.m_nExportStatus = BESPOKE_EXPORT_STATUS_DISABLED;
					infoExport.m_nExportError = 0;
					infoExport.m_timeExportFile = FileStatus.m_mtime;
				}
				else
				{
					int nResult = FNBOptions.ValidateSettings();

					if ( 0 != nResult )
					{
						infoExport.m_strDbName = dbDatabase.GetName( infoExport.m_nDbIdx );
						infoExport.m_nExportStatus = BESPOKE_EXPORT_STATUS_INVALID;
						infoExport.m_nExportError = nResult;
						infoExport.m_timeExportFile = FileStatus.m_mtime;
					}
					else
					{
						infoExport.m_strDbName = dbDatabase.GetName( infoExport.m_nDbIdx );
						infoExport.m_nExportStatus = BESPOKE_EXPORT_STATUS_WAIT_EXPORT;
						infoExport.m_nExportError = 0;
						infoExport.m_timeExportFile = FileStatus.m_mtime;

						COleDateTime oleDummy;
						CString strLastRunDate = FNBOptions.GetLastExportDate( infoExport.m_nDbIdx, FALSE );
						FNBOptions.GetNextExportTime( oleDummy, infoExport.m_timeExportDue, strLastRunDate );
					}
				}
			}
			break;
		
		case BESPOKE_EXPORT_STATUS_INVALID:
		case BESPOKE_EXPORT_STATUS_DISABLED:
		case BESPOKE_EXPORT_STATUS_WAIT_EXPORT:
		case BESPOKE_EXPORT_STATUS_WAIT_CASHRSP:
			if (::FileExists(strFilenameOptions) == FALSE)
			{
				infoExport.m_nExportStatus = BESPOKE_EXPORT_STATUS_NOFILE;
			}
			else
			{
				CFileStatus FileStatus;
				CFile::GetStatus(strFilenameOptions, FileStatus);

				if (FileStatus.m_mtime != infoExport.m_timeExportFile)
				{
					infoExport.m_nExportStatus = BESPOKE_EXPORT_STATUS_WAIT_UPDATE;
				}
			}
			break;
		}

		switch ( infoExport.m_nExportStatus )
		{
		case BESPOKE_EXPORT_STATUS_WAIT_EXPORT:
		case BESPOKE_EXPORT_STATUS_WAIT_CASHRSP:
			if ( CPosTrayTokenFiles::StopFileExists() == FALSE )
			{
				COleDateTime timeNow = COleDateTime::GetCurrentTime();

				if ( timeNow >= infoExport.m_timeExportDue )
				{
					if ( m_CashRSPBuffer.IsDateTimeCleared( infoExport.m_timeExportDue ) == TRUE )
					{
						m_nFNBExportToPerform = m_nFNBExportToCheck;
						SetStatus( POSUTILITY_STATUS_WAIT_FNB );
						CommsHandshaker.PauseComms();
						bGotWork = TRUE;
					}
					else 
					{
						infoExport.m_nExportStatus = BESPOKE_EXPORT_STATUS_WAIT_CASHRSP;
						m_CashRSPBuffer.SetPendingDateTime( infoExport.m_timeExportDue );
					}
				}
			}
			break;
		}

		m_arrayFNBExports[ m_nFNBExportToCheck ] = infoExport;
		UpdateFNBList();

		m_nFNBExportToCheck++;
		if (m_nFNBExportToCheck >= m_arrayFNBExports.GetSize())
		{
			m_nFNBExportToCheck = 0;
		}
	}
	while ( ( m_nFNBExportToCheck != nStartExport ) && ( FALSE == bGotWork ) );

	return bGotWork;
}

/**********************************************************************/

bool CMainFramePosTray::UpdateStatusChartwellsExport()
{
	bool bGotWork = FALSE;

	if (PosTrayOptions.GetChartwellsTaskFlag() == FALSE)
	{
		return FALSE;
	}

	if (m_arrayChartwellsExports.GetSize() == 0 )
	{
		return FALSE;
	}

	{
		CPosTrayChartwellsScheduleInfo infoExport = m_arrayChartwellsExports[0];
		COleDateTime timeNow = COleDateTime::GetCurrentTime();

		if (timeNow.GetMinute() != infoExport.m_nLastDbCheckMinute)
		{
			if (SysInfo.IsPosTrayNoTaskSystem() == FALSE)
			{
				CString strExe = ".\\SPosTray.Exe";
				CString strStockmanFlag = (SysInfo.IsStockSystem()) ? "1" : "0";

				CString strDataPathNo = "";
				strDataPathNo.Format("%d", SysInfo.GetDataPathNo());

				SSspawnl(GetSafeOwner(this), P_NOWAIT, strExe, strExe,
					"903",
					strStockmanFlag,
					strDataPathNo,
					NULL);
			}

			infoExport.m_nLastDbCheckMinute = timeNow.GetMinute();
			m_arrayChartwellsExports[0] = infoExport;
		}
	}

	{
		CPosTrayChartwellsScheduleInfo infoExport = m_arrayChartwellsExports[0];

		CFilenameUpdater FnUp(SysFiles::ChartwellsDbCheck);
		CString strDbCheckFilename = FnUp.GetFilenameToUse();
		
		if (::FileExists(strDbCheckFilename) == TRUE)
		{
			CSSFile fileDbCheck;
			if (fileDbCheck.Open(strDbCheckFilename, "rb") == TRUE)
			{
				CString strBuffer = "";
				fileDbCheck.ReadString(strBuffer);
				fileDbCheck.Close();

				CCSV csv(strBuffer);
				if (csv.GetSize() == 2)
				{
					infoExport.m_strLastDbCheckTime = csv.GetString(0);	
					infoExport.m_nDatabaseError = csv.GetInt(1);
					m_arrayChartwellsExports[0] = infoExport;
				}

				fileDbCheck.Close();
				CFileRemover FileRemover(strDbCheckFilename);
			}
		}


		CChartwellsOptions ChartwellsOptions;
		CString strFilenameOptions = ChartwellsOptions.GetFilename(FALSE);

		switch (infoExport.m_nExportStatus)
		{
		case BESPOKE_EXPORT_STATUS_UNCHECKED:
		case BESPOKE_EXPORT_STATUS_NOFILE:
		case BESPOKE_EXPORT_STATUS_WAIT_UPDATE:
			if (::FileExists(strFilenameOptions) == FALSE)
			{
				infoExport.m_nExportStatus = BESPOKE_EXPORT_STATUS_NOFILE;
			}
			else if (ChartwellsOptions.Read(FALSE) == TRUE)
			{
				CFileStatus FileStatus;
				CFile::GetStatus(strFilenameOptions, FileStatus);

				if (ChartwellsOptions.GetEnable() == FALSE)
				{
					infoExport.m_nExportStatus = BESPOKE_EXPORT_STATUS_DISABLED;
					infoExport.m_nExportError = 0;
					infoExport.m_timeExportFile = FileStatus.m_mtime;
				}
				else
				{
					int nResult = ChartwellsOptions.ValidateSettings();

					if (0 != nResult)
					{
						infoExport.m_nExportStatus = BESPOKE_EXPORT_STATUS_INVALID;
						infoExport.m_nExportError = nResult;
						infoExport.m_timeExportFile = FileStatus.m_mtime;
					}
					else
					{
						infoExport.m_nExportStatus = BESPOKE_EXPORT_STATUS_WAIT_EXPORT;
						infoExport.m_nExportError = 0;
						infoExport.m_timeExportFile = FileStatus.m_mtime;

						COleDateTime oleDummy;
						CString strLastRunDate = ChartwellsOptions.GetLastExportDate(FALSE);
						ChartwellsOptions.GetNextExportTime(oleDummy, infoExport.m_timeExportDue, strLastRunDate);
					}
				}
			}
			break;

		case BESPOKE_EXPORT_STATUS_INVALID:
		case BESPOKE_EXPORT_STATUS_DISABLED:
		case BESPOKE_EXPORT_STATUS_WAIT_EXPORT:
		case BESPOKE_EXPORT_STATUS_WAIT_CASHRSP:
			if (::FileExists(strFilenameOptions) == FALSE)
			{
				infoExport.m_nExportStatus = BESPOKE_EXPORT_STATUS_NOFILE;
			}
			else
			{
				CFileStatus FileStatus;
				CFile::GetStatus(strFilenameOptions, FileStatus);

				if (FileStatus.m_mtime != infoExport.m_timeExportFile)
				{
					infoExport.m_nExportStatus = BESPOKE_EXPORT_STATUS_WAIT_UPDATE;
				}
			}
			break;
		}

		switch (infoExport.m_nExportStatus)
		{
		case BESPOKE_EXPORT_STATUS_WAIT_EXPORT:
		case BESPOKE_EXPORT_STATUS_WAIT_CASHRSP:
			if (CPosTrayTokenFiles::StopFileExists() == FALSE)
			{
				COleDateTime timeNow = COleDateTime::GetCurrentTime();

				if (timeNow >= infoExport.m_timeExportDue)
				{
					if (m_CashRSPBuffer.IsDateTimeCleared(infoExport.m_timeExportDue) == TRUE)
					{
						m_nChartwellsExportToPerform = 0;
						SetStatus(POSUTILITY_STATUS_WAIT_CHARTWELLS);
						CommsHandshaker.PauseComms();
						bGotWork = TRUE;
					}
					else
					{
						infoExport.m_nExportStatus = BESPOKE_EXPORT_STATUS_WAIT_CASHRSP;
						m_CashRSPBuffer.SetPendingDateTime(infoExport.m_timeExportDue);
					}
				}
			}
			break;
		}

		m_arrayChartwellsExports[0] = infoExport;
		UpdateChartwellsList();
	} 
	
	return bGotWork;
}

/**********************************************************************/

bool CMainFramePosTray::UpdateStatusS4LabourExport()
{
	bool bGotWork = FALSE;

	if (PosTrayOptions.GetS4LabourTaskFlag() == FALSE)
	{
		return FALSE;
	}

	if (m_arrayS4LabourExports.GetSize() == 0)
	{
		return FALSE;
	}

	if ((m_nS4LabourExportToCheck < 0) || (m_nS4LabourExportToCheck >= m_arrayS4LabourExports.GetSize()))
	{
		m_nS4LabourExportToCheck = 0;
	}

	int nStartExport = m_nS4LabourExportToCheck;

	do
	{
		CPosTrayS4LabourScheduleInfo infoExport = m_arrayS4LabourExports[m_nS4LabourExportToCheck];

		CS4LabourLocationOptions S4LabourOptions;
		CString strFilenameOptions = S4LabourOptions.GetFilename(infoExport.m_nLocIdx, FALSE);

		switch (infoExport.m_nExportStatus)
		{
		case BESPOKE_EXPORT_STATUS_UNCHECKED:
		case BESPOKE_EXPORT_STATUS_NOFILE:
		case BESPOKE_EXPORT_STATUS_WAIT_UPDATE:
			if (::FileExists(strFilenameOptions) == FALSE)
			{
				infoExport.m_nExportStatus = BESPOKE_EXPORT_STATUS_NOFILE;
			}
			else if (S4LabourOptions.Read(infoExport.m_nLocIdx, FALSE) == TRUE)
			{
				CFileStatus FileStatus;
				CFile::GetStatus(strFilenameOptions, FileStatus);

				if (S4LabourOptions.GetEnable() == FALSE)
				{
					infoExport.m_nExportStatus = BESPOKE_EXPORT_STATUS_DISABLED;
					infoExport.m_nExportError = 0;
					infoExport.m_timeExportFile = FileStatus.m_mtime;
					infoExport.m_strLocID = "";
				}
				else
				{
					int nResult = S4LabourOptions.ValidateSettings(infoExport.m_nLocIdx);

					if (0 != nResult)
					{
						infoExport.m_strLocName = dbLocation.GetName(infoExport.m_nLocIdx);
						infoExport.m_nExportStatus = BESPOKE_EXPORT_STATUS_INVALID;
						infoExport.m_nExportError = nResult;
						infoExport.m_timeExportFile = FileStatus.m_mtime;
						infoExport.m_strLocID = S4LabourOptions.GetLocID();
					}
					else
					{
						infoExport.m_strLocName = dbLocation.GetName(infoExport.m_nLocIdx);
						infoExport.m_nExportStatus = BESPOKE_EXPORT_STATUS_WAIT_EXPORT;
						infoExport.m_nExportError = 0;
						infoExport.m_timeExportFile = FileStatus.m_mtime;
						infoExport.m_strLocID = S4LabourOptions.GetLocID();

						COleDateTime oleDummy;
						CString strLastRunDate = S4LabourOptions.GetLastExportDate(infoExport.m_nLocIdx, FALSE);
						S4LabourOptions.GetNextExportTime(oleDummy, infoExport.m_timeExportDue, strLastRunDate);
					}
				}
			}
			break;

		case BESPOKE_EXPORT_STATUS_INVALID:
		case BESPOKE_EXPORT_STATUS_DISABLED:
		case BESPOKE_EXPORT_STATUS_WAIT_EXPORT:
		case BESPOKE_EXPORT_STATUS_WAIT_CASHRSP:
			if (::FileExists(strFilenameOptions) == FALSE)
			{
				infoExport.m_nExportStatus = BESPOKE_EXPORT_STATUS_NOFILE;
			}
			else
			{
				CFileStatus FileStatus;
				CFile::GetStatus(strFilenameOptions, FileStatus);

				if (FileStatus.m_mtime != infoExport.m_timeExportFile)
				{
					infoExport.m_nExportStatus = BESPOKE_EXPORT_STATUS_WAIT_UPDATE;
				}
			}
			break;
		}

		switch (infoExport.m_nExportStatus)
		{
		case BESPOKE_EXPORT_STATUS_WAIT_EXPORT:
		case BESPOKE_EXPORT_STATUS_WAIT_CASHRSP:
			if (CPosTrayTokenFiles::StopFileExists() == FALSE)
			{
				COleDateTime timeNow = COleDateTime::GetCurrentTime();

				if (timeNow >= infoExport.m_timeExportDue)
				{
					if (m_CashRSPBuffer.IsDateTimeCleared(infoExport.m_timeExportDue) == TRUE)
					{
						m_nS4LabourExportToPerform = m_nS4LabourExportToCheck;
						SetStatus(POSUTILITY_STATUS_WAIT_S4LABOUR);
						CommsHandshaker.PauseComms();
						bGotWork = TRUE;
					}
					else
					{
						infoExport.m_nExportStatus = BESPOKE_EXPORT_STATUS_WAIT_CASHRSP;
						m_CashRSPBuffer.SetPendingDateTime(infoExport.m_timeExportDue);
					}
				}
			}
			break;
		}

		m_arrayS4LabourExports[m_nS4LabourExportToCheck] = infoExport;
		UpdateS4LabourList();

		m_nS4LabourExportToCheck++;
		if (m_nS4LabourExportToCheck >= m_arrayS4LabourExports.GetSize())
		{
			m_nS4LabourExportToCheck = 0;
		}
	}
	while ((m_nS4LabourExportToCheck != nStartExport) && (FALSE == bGotWork));

	return bGotWork;
}

/**********************************************************************/

bool CMainFramePosTray::UpdateStatusBackupTask()
{
	if (PosTrayOptions.GetManageBackupFlag() == TRUE)
	{
		UpdateBackupList();
	}

	return FALSE;
}

/**********************************************************************/

bool CMainFramePosTray::UpdateStatusLIPAExport()
{
	bool bGotWork = FALSE;

	if (DealerFlags.Get_LIPA_LJMU_ExportType() == 0)
	{
		return FALSE;
	}

	CLIPAExportDate LIPAExportDate;
	
	switch( m_LIPAExport.m_nExportStatus )
	{
	case BESPOKE_EXPORT_STATUS_UNCHECKED:
	case BESPOKE_EXPORT_STATUS_WAIT_UPDATE:
		{
			m_LIPAExport.m_nExportStatus = BESPOKE_EXPORT_STATUS_WAIT_EXPORT;
			m_LIPAExport.m_nExportError = 0;
							
			COleDateTime oleDummy;
			CString strLastRunDate = LIPAExportDate.GetLastExportDate( FALSE );
			LIPAExportDate.GetNextExportTime( oleDummy, m_LIPAExport.m_timeExportDue, strLastRunDate );
		}
		break;
	
	case BESPOKE_EXPORT_STATUS_WAIT_EXPORT:
	case BESPOKE_EXPORT_STATUS_WAIT_CASHRSP:
		break;
	}

	switch ( m_LIPAExport.m_nExportStatus )
	{
	case BESPOKE_EXPORT_STATUS_WAIT_EXPORT:
	case BESPOKE_EXPORT_STATUS_WAIT_CASHRSP:
		if ( CPosTrayTokenFiles::StopFileExists() == FALSE )
		{
			COleDateTime timeNow = COleDateTime::GetCurrentTime();

			if ( timeNow >= m_LIPAExport.m_timeExportDue )
			{
				if ( m_CashRSPBuffer.IsDateTimeCleared( m_LIPAExport.m_timeExportDue ) == TRUE )
				{
					SetStatus( POSUTILITY_STATUS_WAIT_LIPA );
					CommsHandshaker.PauseComms();
					bGotWork = TRUE;
				}
				else 
				{
					m_LIPAExport.m_nExportStatus = BESPOKE_EXPORT_STATUS_WAIT_CASHRSP;
					m_CashRSPBuffer.SetPendingDateTime( m_LIPAExport.m_timeExportDue );
				}
			}
		}
		break;
	}

	UpdateLIPAList();
	return bGotWork;
}

/**********************************************************************/

void CMainFramePosTray::UpdateStatus()
{
	//IF WE DO HAVE A SYSTEM THEN DETECT CHANGES IN LOCCSV
	switch (m_nPosTrayStatus)
	{
	case POSUTILITY_STATUS_GOT_SYSTEM:
	{
		COleDateTime timePosUtilityToken;
		CPosTrayTokenFiles::GetSystemUpdateFileTime(timePosUtilityToken);

		if (timePosUtilityToken != SystemMapFull.GetOleTimePosUtilityToken())
		{
			SetStatus(POSUTILITY_STATUS_WAIT_INITSYSTEM);
			CommsHandshaker.PauseComms();
			return;
		}
	}
	break;
	}

	//IF WE DO HAVE A SYSTEM THEN SEE IF THE NEXT TASK IS DUE
	bool bGotWork = FALSE;

	switch (m_nPosTrayStatus)
	{
	case POSUTILITY_STATUS_GOT_SYSTEM:
		if (TRUE == m_bTryFNBExportFirst)
		{
			bGotWork = UpdateStatusFNBExport();

			if (FALSE == bGotWork)
			{
				bGotWork = UpdateStatusChartwellsExport();
			}

			if (FALSE == bGotWork)
			{
				bGotWork = UpdateStatusS4LabourExport();
			}

			if (FALSE == bGotWork)
			{
				bGotWork = UpdateStatusLIPAExport();
			}

			if (FALSE == bGotWork)
			{
				bGotWork = UpdateStatusEmailTask();
			}

			if (PosTrayOptions.AreTasksEnabled() == TRUE)
			{
				m_bTryFNBExportFirst = FALSE;
			}
		}
		else
		{
			bGotWork = UpdateStatusEmailTask();

			if (FALSE == bGotWork)
			{
				bGotWork = UpdateStatusChartwellsExport();
			}

			if (FALSE == bGotWork)
			{
				bGotWork = UpdateStatusFNBExport();
			}

			if (FALSE == bGotWork)
			{
				bGotWork = UpdateStatusS4LabourExport();
			}

			if (FALSE == bGotWork)
			{
				bGotWork = UpdateStatusLIPAExport();
			}

			if (PosTrayOptions.GetFNBTaskFlag() == TRUE)
			{
				m_bTryFNBExportFirst = TRUE;
			}
		}
		break;
	}

	UpdateStatusBackupTask();

	m_CashRSPBuffer.Pulse();

	if (TRUE == bGotWork)
	{
		return;
	}

	switch (m_nPosTrayStatus)
	{
	case POSUTILITY_STATUS_WAIT_TASK:
	{
		__int64 nLockTag = GetTickCount64();
		if (PosTrayLock.LockFile(1, nLockTag) == TRUE)
		{
			CCashRSPImporterStatusDisplayPosTray StatusDisplay(this);
			if (CommsHandshaker.ProcessSalesForPosTray(StatusDisplay) == TRUE)
			{
				PerformEmailTask();
			}

			SetStatus(POSUTILITY_STATUS_GOT_SYSTEM);
			CommsHandshaker.ResumeComms();

			PosTrayLock.UnlockFile(1, nLockTag);
			DisplayCashRSPStatus("");
		}
	}
	break;

	case POSUTILITY_STATUS_WAIT_FNB:
	{
		__int64 nLockTag = GetTickCount64();
		if (PosTrayLock.LockFile(2, nLockTag) == TRUE)
		{
			CCashRSPImporterStatusDisplayPosTray StatusDisplay(this);
			if (CommsHandshaker.ProcessSalesForPosTray(StatusDisplay) == TRUE)
			{
				PerformFNBExport();
			}

			SetStatus(POSUTILITY_STATUS_GOT_SYSTEM);
			CommsHandshaker.ResumeComms();

			PosTrayLock.UnlockFile(2, nLockTag);
			DisplayCashRSPStatus("");
		}
	}
	break;

	case POSUTILITY_STATUS_WAIT_CHARTWELLS:
	{
		__int64 nLockTag = GetTickCount64();
		if (PosTrayLock.LockFile(2, nLockTag) == TRUE)
		{
			CCashRSPImporterStatusDisplayPosTray StatusDisplay(this);
			if (CommsHandshaker.ProcessSalesForPosTray(StatusDisplay) == TRUE)
			{
				PerformChartwellsExport();
			}

			SetStatus(POSUTILITY_STATUS_GOT_SYSTEM);
			CommsHandshaker.ResumeComms();

			PosTrayLock.UnlockFile(2, nLockTag);
			DisplayCashRSPStatus("");
		}
	}
	break;

	case POSUTILITY_STATUS_WAIT_S4LABOUR:
	{
		__int64 nLockTag = GetTickCount64();
		if (PosTrayLock.LockFile(3, nLockTag) == TRUE)
		{
			CCashRSPImporterStatusDisplayPosTray StatusDisplay(this);
			if (CommsHandshaker.ProcessSalesForPosTray(StatusDisplay) == TRUE)
			{
				PerformS4LabourExport();
			}

			SetStatus(POSUTILITY_STATUS_GOT_SYSTEM);
			CommsHandshaker.ResumeComms();

			PosTrayLock.UnlockFile(3, nLockTag);
			DisplayCashRSPStatus("");
		}
	}
	break;

	case POSUTILITY_STATUS_WAIT_LIPA:
	{
		__int64 nLockTag = GetTickCount64();
		if (PosTrayLock.LockFile(4, nLockTag) == TRUE)
		{
			CCashRSPImporterStatusDisplayPosTray StatusDisplay(this);
			if (CommsHandshaker.ProcessSalesForPosTray(StatusDisplay) == TRUE)
			{
				PerformLIPAExport();
			}

			SetStatus(POSUTILITY_STATUS_GOT_SYSTEM);
			CommsHandshaker.ResumeComms();

			PosTrayLock.UnlockFile(4, nLockTag);
			DisplayCashRSPStatus("");
		}
	}
	break;
	}

	//IF WE DO NOT HAVE A SYSTEM THEN RECHECK THE DATA FOLDER 
	switch (m_nPosTrayStatus)
	{
	case POSUTILITY_STATUS_WAIT_INITSYSTEM:
		InitialiseSystem();
		break;
	}
}

/**********************************************************************/

void CMainFramePosTray::PerformEmailTask()
{
	int nTaskIdx = m_nEmailTaskToPerform;

	if ( ( nTaskIdx >= 0 ) && ( nTaskIdx < m_arrayEmailTasks.GetSize() ) )
	{
		CPosTrayEmailScheduleInfo infoTask = m_arrayEmailTasks[ nTaskIdx ];

		COleDateTime timeNow = COleDateTime::GetCurrentTime();

		bool bStatusCheck = FALSE;
		switch( infoTask.m_nTaskStatus )
		{
		case EMAILTASK_STATUS_WAIT_TASK:
		case EMAILTASK_STATUS_WAIT_CASHRSP:
			bStatusCheck = TRUE;
			break;
		}

		if ( (timeNow >= infoTask.m_timeTaskDue) && ( TRUE == bStatusCheck ) )
		{
			CPosTrayTask Task( infoTask.GetFileType() );
			Task.SetTaskNo( infoTask.m_nTaskNo );

			if (Task.Read(FALSE) == TRUE)
			{
				CString strTestTime1 = "";
				CString strTestTime2 = "";
				COleDateTime timeDue = infoTask.m_timeTaskDue;

				if ( infoTask.m_nRunNowStatus != 2)
				{
					CString strLastRunTime = Task.GetLastRunTime(FALSE);
					Task.GetNextRunTime(timeDue, strLastRunTime, infoTask.m_timeTaskChecked);
					strTestTime1 = GetFormattedTimeString(timeDue);
					strTestTime2 = GetFormattedTimeString(infoTask.m_timeTaskDue);
				}
				
				if ( strTestTime1 != strTestTime2 )
				{
					MessageLogger.LogPosTraySystem( infoTask, Task, "Task File Changed", "(pre-run check)" );
					infoTask.m_nTaskStatus = EMAILTASK_STATUS_WAIT_UPDATE;
				}
				else
				{
					infoTask.m_nRunNowStatus = 0;

#ifdef COMPILE_PMS
					if ( SysInfo.IsPMSSystem() == TRUE )
					{
						dbLocation.CreatePMSPseudoLocation();
			
						CFilenameUpdater FnUp( SysFiles::PMSBooking );
						DataManagerNonDb.PMSBooking.Read( FnUp.GetFilenameToUse() );
						PMSCheckoutDateArray.InvalidateCheckoutDateArray();
					}
#endif

					dbLocation.BuildSortIdxTables();
					dbDatabase.BuildSortIdxTables();
			
					bool bResult = FALSE;
					bool bBadExportFolder = FALSE;
					
					switch( infoTask.m_nTaskType )
					{
					case EMAILTASK_TYPE_HIST:
						{
							CPosTrayReportCreatorHistory TaskProcessor( Task );
							bResult = TaskProcessor.ProcessTask( timeDue );

							{
								int nSkipped = TaskProcessor.GetSkippedReportCountEmpty();
								if (nSkipped > 0)
								{
									CString strPlural = (nSkipped > 1) ? "s" : "";

									CString strSkipped = "";
									strSkipped.Format("Skipped %d empty report%s",
										nSkipped,
										(const char*)strPlural);

									MessageLogger.LogPosTraySystem(infoTask, Task, strSkipped, "OK");
								}
							}

							{
								int nSkipped = TaskProcessor.GetSkippedReportCountNoChange();
								if (nSkipped > 0)
								{
									CString strPlural = (nSkipped > 1) ? "s" : "";

									CString strSkipped = "";
									strSkipped.Format("Skipped %d unchanged report%s",
										nSkipped,
										(const char*)strPlural);

									MessageLogger.LogPosTraySystem(infoTask, Task, strSkipped, "OK");
								}
							}
						}
						break;

					case EMAILTASK_TYPE_TIMEATTEND:
						if ( SysInfo.IsPosTrayTimeAndAttendanceSystem() == TRUE )
						{
							CPosTrayReportCreatorTimeAttend TaskProcessor(timeDue, Task);
							bResult = TaskProcessor.ProcessTask();
						}
						break;

					case EMAILTASK_TYPE_STKLEVEL:
						if ( SysInfo.IsStockSystem() == FALSE )
						{
							CPosTrayReportCreatorStkLevelEcr TaskProcessor( Task );
							bResult = TaskProcessor.ProcessTask( timeDue );
						}
						break;

					case EMAILTASK_TYPE_EXPORT_SALES:
						{
							bool bDummy = FALSE;
							CPosTrayExportCreatorSales TaskProcessor( Task, bDummy );
							
							__int64 nResult = TaskProcessor.ProcessTask( timeDue );
							bResult = ( nResult >= 1 );
							bBadExportFolder = ( -1 == nResult );
						}
						break;

					case EMAILTASK_TYPE_EXPORT_PAYMENT:
						{
							bool bDummy = FALSE;
							CPosTrayExportCreatorPayment TaskProcessor( Task, bDummy );
							
							__int64 nResult = TaskProcessor.ProcessTask( timeDue );
							bResult = ( nResult >= 1 );
							bBadExportFolder = ( -1 == nResult );
						}
						break;

					case EMAILTASK_TYPE_EXPORT_VOID:
						{
							bool bDummy = FALSE;
							CPosTrayExportCreatorVoid TaskProcessor( Task, bDummy );
							
							__int64 nResult = TaskProcessor.ProcessTask( timeDue );
							bResult = ( nResult >= 1 );
							bBadExportFolder = ( -1 == nResult );
						}
						break;

					case EMAILTASK_TYPE_EXPORT_DISCOUNT:
						{
							bool bDummy = FALSE;
							CPosTrayExportCreatorDiscount TaskProcessor( Task, bDummy );
							
							__int64 nResult = TaskProcessor.ProcessTask( timeDue );
							bResult = ( nResult >= 1 );
							bBadExportFolder = ( -1 == nResult );
						}
						break;

					case EMAILTASK_TYPE_PROCESSTRAN:
						bResult = TRUE;
						break;

					case EMAILTASK_TYPE_EPOS:
					default:
						{
							CPosTrayReportCreatorEpos TaskProcessor( timeDue, Task );
							bResult = TaskProcessor.ProcessTask();

							{
								int nSkipped = TaskProcessor.GetSkippedReportCountEmpty();
								if (nSkipped > 0)
								{
									CString strPlural = (nSkipped > 1) ? "s" : "";

									CString strSkipped = "";
									strSkipped.Format("Skipped %d empty report%s",
										nSkipped,
										(const char*)strPlural);

									MessageLogger.LogPosTraySystem(infoTask, Task, strSkipped, "OK");
								}
							}

							{
								int nSkipped = TaskProcessor.GetSkippedReportCountNoChange();
								if (nSkipped > 0)
								{
									CString strPlural = (nSkipped > 1) ? "s" : "";

									CString strSkipped = "";
									strSkipped.Format("Skipped %d unchanged report%s",
										nSkipped,
										(const char*)strPlural);

									MessageLogger.LogPosTraySystem(infoTask, Task, strSkipped, "OK");
								}
							}
						}
						break;
					}

#ifdef COMPILE_PMS
					if ( SysInfo.IsPMSSystem() == TRUE )
					{
						dbLocation.RemovePMSPseudoLocation();
						PMSCheckoutDateArray.InvalidateCheckoutDateArray();
					}
#endif

					if ( TRUE == bResult )
					{
						MessageLogger.LogPosTraySystem( infoTask, Task, "Run Task", "OK" );

						CString strTime;
						strTime.Format( "%4.4d%2.2d%2.2d%2.2d%2.2d", 
							timeDue.GetYear(),
							timeDue.GetMonth(),
							timeDue.GetDay(),
							timeDue.GetHour(),
							timeDue.GetMinute() );

						Task.SetLastRunTime( strTime, FALSE );

						COleDateTime timeNow = COleDateTime::GetCurrentTime();
	
						COleDateTime timeNextRun;
						if ( Task.GetNextRunTime( timeNextRun, strTime, timeNow ) == TRUE )
						{
							infoTask.m_timeTaskChecked = timeNow;
							infoTask.m_timeTaskDue = timeNextRun;
							infoTask.m_nTaskStatus = EMAILTASK_STATUS_WAIT_TASK;
							MessageLogger.LogPosTraySystem( infoTask, Task, "Update Run Time", "OK" );
						}
						else
						{
							infoTask.m_timeTaskChecked = timeNow;
							infoTask.m_nTaskStatus = EMAILTASK_STATUS_WAIT_UPDATE;
							MessageLogger.LogPosTraySystem( infoTask, Task, "Update Run Time", "FAIL" );
						}
					}
					else 
					{
						MessageLogger.LogPosTraySystem( infoTask, Task, "Run Task", "FAIL" );
						
						if ( TRUE == bBadExportFolder )
						{
							infoTask.m_nTaskStatus = EMAILTASK_STATUS_WAIT_RETRYFOLDER;
							infoTask.m_timeTaskRetryFolder = timeNow + COleDateTimeSpan( 0, 0, 2, 0 );
						}
					}
				}
					
				m_arrayEmailTasks[ nTaskIdx ] = infoTask;
				UpdateTaskList();
			}
		}
	}
}

/**********************************************************************/

void CMainFramePosTray::PerformFNBExport()
{
	int nExportIdx = m_nFNBExportToPerform;

	if ( ( nExportIdx >= 0 ) && ( nExportIdx < m_arrayFNBExports.GetSize() ) )
	{
		CPosTrayFNBScheduleInfo infoExport = m_arrayFNBExports[ nExportIdx ];

		COleDateTime timeNow = COleDateTime::GetCurrentTime();

		bool bStatusCheck = FALSE;
		switch( infoExport.m_nExportStatus )
		{
		case BESPOKE_EXPORT_STATUS_WAIT_EXPORT:
		case BESPOKE_EXPORT_STATUS_WAIT_CASHRSP:
			bStatusCheck = TRUE;
			break;
		}

		if ( ( timeNow >= infoExport.m_timeExportDue ) && ( TRUE == bStatusCheck ) )
		{
			CFNBDatabaseOptions Options;
			if ( Options.Read( infoExport.m_nDbIdx, FALSE ) == TRUE )
			{
				COleDateTime dateReport, timeDue;
				CString strLastExportDate = Options.GetLastExportDate( infoExport.m_nDbIdx, FALSE );
				Options.GetNextExportTime( dateReport, timeDue, strLastExportDate );

				CString strTestTime1 = GetFormattedTimeString( timeDue );
				CString strTestTime2 = GetFormattedTimeString( infoExport.m_timeExportDue );

				if (strTestTime1 != strTestTime2)
				{
					infoExport.m_nExportStatus = BESPOKE_EXPORT_STATUS_WAIT_UPDATE;
				}
				else
				{
					dbLocation.BuildSortIdxTables();
					dbDatabase.BuildSortIdxTables();
					CPosTrayFNBExportCreator ExportCreator( infoExport, Options );

					if (ExportCreator.ProcessExport(dateReport) == TRUE)
					{
						CString strDate;
						strDate.Format("%4.4d%2.2d%2.2d",
							dateReport.GetYear(),
							dateReport.GetMonth(),
							dateReport.GetDay());

						Options.SetLastExportDate(infoExport.m_nDbIdx, strDate, FALSE);

						COleDateTime dateDummy, timeNextExport;
						if (Options.GetNextExportTime(dateDummy, timeNextExport, strDate) == TRUE)
						{
							infoExport.m_timeExportDue = timeNextExport;
							infoExport.m_nExportStatus = BESPOKE_EXPORT_STATUS_WAIT_EXPORT;
						}
						else
						{
							infoExport.m_nExportStatus = BESPOKE_EXPORT_STATUS_WAIT_UPDATE;
						}
					}
				}
					
				m_arrayFNBExports[ nExportIdx ] = infoExport;
				UpdateFNBList();
			}
		}
	}
}

/**********************************************************************/

void CMainFramePosTray::PerformChartwellsExport()
{
	int nExportIdx = m_nChartwellsExportToPerform;

	if ((nExportIdx >= 0) && (nExportIdx < m_arrayChartwellsExports.GetSize()))
	{
		CPosTrayChartwellsScheduleInfo infoExport = m_arrayChartwellsExports[nExportIdx];

		COleDateTime timeNow = COleDateTime::GetCurrentTime();

		bool bStatusCheck = FALSE;
		switch (infoExport.m_nExportStatus)
		{
		case BESPOKE_EXPORT_STATUS_WAIT_EXPORT:
		case BESPOKE_EXPORT_STATUS_WAIT_CASHRSP:
			bStatusCheck = TRUE;
			break;
		}

		if ((timeNow >= infoExport.m_timeExportDue) && (TRUE == bStatusCheck))
		{
			if (ChartwellsOptions.Read(FALSE) == TRUE)
			{
				COleDateTime dateReport, timeDue;
				CString strLastExportDate = ChartwellsOptions.GetLastExportDate(FALSE);
				ChartwellsOptions.GetNextExportTime(dateReport, timeDue, strLastExportDate);

				CString strTestTime1 = GetFormattedTimeString(timeDue);
				CString strTestTime2 = GetFormattedTimeString(infoExport.m_timeExportDue);

				if (strTestTime1 != strTestTime2)
				{
					infoExport.m_nExportStatus = BESPOKE_EXPORT_STATUS_WAIT_UPDATE;
				}
				else
				{
					dbLocation.BuildSortIdxTables();
					dbDatabase.BuildSortIdxTables();
					CPosTrayChartwellsExportCreator ExportCreator(infoExport);

					if (ExportCreator.ProcessExport(dateReport) == TRUE)
					{
						CString strDate;
						strDate.Format("%4.4d%2.2d%2.2d",
							dateReport.GetYear(),
							dateReport.GetMonth(),
							dateReport.GetDay());

						ChartwellsOptions.SetLastExportDate(strDate, FALSE);

						COleDateTime dateDummy, timeNextExport;
						if (ChartwellsOptions.GetNextExportTime(dateDummy, timeNextExport, strDate) == TRUE)
						{
							infoExport.m_timeExportDue = timeNextExport;
							infoExport.m_nExportStatus = BESPOKE_EXPORT_STATUS_WAIT_EXPORT;
						}
						else
						{
							infoExport.m_nExportStatus = BESPOKE_EXPORT_STATUS_WAIT_UPDATE;
						}
					}
				}

				m_arrayChartwellsExports[nExportIdx] = infoExport;
				UpdateChartwellsList();
			}
		}
	}
}

/**********************************************************************/

void CMainFramePosTray::PerformS4LabourExport()
{
	int nExportIdx = m_nS4LabourExportToPerform;

	if ( ( nExportIdx >= 0 ) && ( nExportIdx < m_arrayS4LabourExports.GetSize() ) )
	{
		CPosTrayS4LabourScheduleInfo infoExport = m_arrayS4LabourExports[ nExportIdx ];

		COleDateTime timeNow = COleDateTime::GetCurrentTime();

		bool bStatusCheck = FALSE;
		switch( infoExport.m_nExportStatus )
		{
		case BESPOKE_EXPORT_STATUS_WAIT_EXPORT:
		case BESPOKE_EXPORT_STATUS_WAIT_CASHRSP:
			bStatusCheck = TRUE;
			break;
		}

		if ( ( timeNow >= infoExport.m_timeExportDue ) && ( TRUE == bStatusCheck ) )
		{
			CS4LabourLocationOptions Options;
			if ( Options.Read( infoExport.m_nLocIdx, FALSE ) == TRUE )
			{
				COleDateTime dateNextExport, timeDue;
				CString strLastExportDate = Options.GetLastExportDate( infoExport.m_nLocIdx, FALSE );
				Options.GetNextExportTime( dateNextExport, timeDue, strLastExportDate );

				CString strTestTime1 = GetFormattedTimeString( timeDue );
				CString strTestTime2 = GetFormattedTimeString( infoExport.m_timeExportDue );

				if (strTestTime1 != strTestTime2)
				{
					infoExport.m_nExportStatus = BESPOKE_EXPORT_STATUS_WAIT_UPDATE;
				}
				else
				{
					MarkS4LabourLocIDDuplicates();
					
					if ( FALSE == m_arrayS4LabourExports[nExportIdx].m_bDuplicateID )
					{
						bool bQuitNow = FALSE;

						COleDateTime dateBackStop = dateNextExport;
						COleDateTime dateLatest = Options.GetLatestExportableDate();
						COleDateTime dateToExport = dateLatest;

						{
							int nCount = 0;
							bool bFail = FALSE;

							do
							{
								CPosTrayExportCreatorS4Labour ExportCreator( infoExport, Options, bQuitNow );
								ExportCreator.SetExportDate( dateToExport );

								if ( ExportCreator.ProcessTask() < 0 )
								{
									bFail = TRUE;
									break;
								}
								else
								{
									CDateRangeHelpers DateRangeHelpers;
									DateRangeHelpers.AddDaysToTime( dateToExport, -1 );
									nCount++;
								}
							}
							while ( ( nCount < 7 ) || ( dateToExport >= dateBackStop ) );

							if ( FALSE == bFail )
							{
								CString strDate;
								strDate.Format( "%4.4d%2.2d%2.2d", 
									dateLatest.GetYear(),
									dateLatest.GetMonth(),
									dateLatest.GetDay() );

								Options.SetLastExportDate( infoExport.m_nLocIdx, strDate, FALSE );
						
								COleDateTime dateDummy, timeNextExport;
								if ( Options.GetNextExportTime( dateDummy, timeNextExport, strDate ) == TRUE )
								{
									infoExport.m_timeExportDue = timeNextExport;
									infoExport.m_nExportStatus = BESPOKE_EXPORT_STATUS_WAIT_EXPORT;
								}
								else
									infoExport.m_nExportStatus = BESPOKE_EXPORT_STATUS_WAIT_UPDATE;
							}
						}
					}
				}
					
				m_arrayS4LabourExports[ nExportIdx ] = infoExport;
				UpdateS4LabourList();
			}
		}
	}
}

/**********************************************************************/

void CMainFramePosTray::PerformLIPAExport()
{
	COleDateTime timeNow = COleDateTime::GetCurrentTime();

	bool bStatusCheck = FALSE;
	switch( m_LIPAExport.m_nExportStatus )
	{
	case BESPOKE_EXPORT_STATUS_WAIT_EXPORT:
	case BESPOKE_EXPORT_STATUS_WAIT_CASHRSP:
		bStatusCheck = TRUE;
		break;
	}

	if ( ( timeNow >= m_LIPAExport.m_timeExportDue ) && ( TRUE == bStatusCheck ) )
	{
		CLIPAExportDate LIPAExportDate;

		COleDateTime dateReport, timeDue;
		CString strLastExportDate = LIPAExportDate.GetLastExportDate( FALSE );
		LIPAExportDate.GetNextExportTime( dateReport, timeDue, strLastExportDate );

		CString strTestTime1 = GetFormattedTimeString( timeDue );
		CString strTestTime2 = GetFormattedTimeString( m_LIPAExport.m_timeExportDue );

		if (strTestTime1 != strTestTime2)
		{
			m_LIPAExport.m_nExportStatus = BESPOKE_EXPORT_STATUS_WAIT_UPDATE;
		}
		else
		{
			CPosTrayLIPAExportCreator ExportCreator( m_LIPAExport );

			if ( ExportCreator.ProcessExport( dateReport ) == TRUE )
			{
				CString strDate;
				strDate.Format( "%4.4d%2.2d%2.2d", 
				dateReport.GetYear(),
				dateReport.GetMonth(),
				dateReport.GetDay() );

				CString str = "LIPA EXPORT ";
				str.Format( strDate );

				LIPAExportDate.SetLastExportDate( strDate, FALSE );
					
				COleDateTime dateDummy, timeNextExport;
				if ( LIPAExportDate.GetNextExportTime( dateDummy, timeNextExport, strDate ) == TRUE )
				{
					m_LIPAExport.m_timeExportDue = timeNextExport;
					m_LIPAExport.m_nExportStatus = BESPOKE_EXPORT_STATUS_WAIT_EXPORT;
				}
				else
				{
					m_LIPAExport.m_nExportStatus = BESPOKE_EXPORT_STATUS_WAIT_UPDATE;
				}
			}
					
			UpdateLIPAList();
		}
	}
}

/**********************************************************************/

void CMainFramePosTray::SetNextCheckNotAfter(COleDateTime timeNow, int nMinutes, int nSeconds, COleDateTime& timeNextCheck)
{
	if ( timeNow + COleDateTimeSpan( 0, 0, nMinutes, nSeconds ) < timeNextCheck )
	{
		timeNextCheck = timeNow + COleDateTimeSpan( 0, 0, nMinutes, nSeconds );
	}
}

/**********************************************************************/

void CMainFramePosTray::OnTimer(UINT nIDEvent) 
{
	if ( nIDEvent == TASK_CHECK_PRODDER )
	{
		COleDateTime timeNow = COleDateTime::GetCurrentTime();

		if ( timeNow >= m_NextCheckTime )
		{
			m_bReadyToActSalesImporter = TRUE;
			m_bReadyToAct = TRUE;
		}

		if ( timeNow >= m_NextStatusTime )
		{
			m_CurrentStatusTime = m_NextStatusTime;
			m_bReadyToActStatus = TRUE;
			m_bReadyToAct = TRUE;
		}

		if ( ( timeNow >= m_NextEmailTime ) && ( PosTrayOptions.AreTasksEnabled() == TRUE ) )
		{
			m_bReadyToActEmail = TRUE;
			m_bReadyToAct = TRUE;
		}

		if ( ( timeNow >= m_NextPrintTime ) && ( PosTrayOptions.AreTasksEnabled() == TRUE ) )
		{
			m_bReadyToActPrint = TRUE;
			m_bReadyToAct = TRUE;
		}

		if ( ( timeNow >= m_NextExportTime ) && ( PosTrayOptions.AreTasksEnabled() == TRUE ) )
		{
			m_bReadyToActExport = TRUE;
			m_bReadyToAct = TRUE;
		}

		if ( ( timeNow >= m_NextFNBExportTime ) && ( PosTrayOptions.GetFNBTaskFlag() == TRUE ) )
		{
			m_bReadyToActFNBExport = TRUE;
			m_bReadyToAct = TRUE;
		}

		if ((timeNow >= m_NextChartwellsExportTime) && (PosTrayOptions.GetChartwellsTaskFlag() == TRUE))
		{
			m_bReadyToActChartwellsExport = TRUE;
			m_bReadyToAct = TRUE;
		}

		if ((timeNow >= m_NextBackupTime) && (PosTrayOptions.GetManageBackupFlag() == TRUE))
		{
			m_bReadyToActBackup = TRUE;
			m_bReadyToAct = TRUE;
		}

		if ( ( timeNow >= m_NextS4LabourExportTime ) && ( PosTrayOptions.GetS4LabourTaskFlag() == TRUE ) )
		{
			m_bReadyToActS4LabourExport = TRUE;
			m_bReadyToAct = TRUE;
		}

		if ( ( timeNow >= m_NextFTPExportTime ) && ( PosTrayOptions.GotFTPSettings() == TRUE ) )
		{
			m_bReadyToActFTPExport = TRUE;
			m_bReadyToAct = TRUE;
		}

		SetNextCheckNotAfter(timeNow, 2, 0, m_NextCheckTime);
		SetNextCheckNotAfter(timeNow, 0, 30, m_NextEmailTime);
		SetNextCheckNotAfter(timeNow, 0, 30, m_NextPrintTime);
		SetNextCheckNotAfter(timeNow, 0, 30, m_NextExportTime);
		SetNextCheckNotAfter(timeNow, 0, 30, m_NextFNBExportTime);
		SetNextCheckNotAfter(timeNow, 0, 30, m_NextChartwellsExportTime);
		SetNextCheckNotAfter(timeNow, 0, 5, m_NextBackupTime);
		SetNextCheckNotAfter(timeNow, 0, 30, m_NextS4LabourExportTime);
		SetNextCheckNotAfter(timeNow, 0, 30, m_NextFTPExportTime);
		SetNextCheckNotAfter(timeNow, 0, 10, m_NextStatusTime);
	}
	
	CFrameWnd::OnTimer(nIDEvent);
}

/**********************************************************************/

void CMainFramePosTray::InitialiseSystem()
{
	InitialiseSystemInternal();
	MessageLogger.LogPosTrayInitialiseSystem( POSUTILITY_STATUS_GOT_SYSTEM == m_nPosTrayStatus );
}

/**********************************************************************/

void CMainFramePosTray::InitialiseSystemInternal()
{
	SetStatus( POSUTILITY_STATUS_CHECKING );

	if ( PosTrayOptions.Read() == FALSE )
	{
		if ( PosTrayOptions.GetFatalReadErrorFlag() )
		{
			SetStatus( POSUTILITY_STATUS_NO_SYSTEM );
			return;
		}
	}

	m_arrayEmailTasks.RemoveAll();
	
	if (PosTrayOptions.GetPosTrayProcessTranFlag() == TRUE)
	{
		for (int nTaskNo = 1; nTaskNo <= POSTRAY_MAX_TASK; nTaskNo++)
		{
			CPosTrayEmailScheduleInfo info;
			info.m_nTaskType = EMAILTASK_TYPE_PROCESSTRAN;
			info.m_nTaskNo = nTaskNo;
			info.m_nTaskStatus = EMAILTASK_STATUS_UNCHECKED;
			m_arrayEmailTasks.Add(info);
		}
	}

	if ( PosTrayOptions.GetReportTaskEposFlag() == TRUE )
	{
		for ( int nTaskNo = 1; nTaskNo <= POSTRAY_MAX_TASK; nTaskNo++ )
		{
			CPosTrayEmailScheduleInfo info;
			info.m_nTaskType = EMAILTASK_TYPE_EPOS;
			info.m_nTaskNo = nTaskNo;
			info.m_nTaskStatus = EMAILTASK_STATUS_UNCHECKED;
			info.InitRunNowFlag();
			m_arrayEmailTasks.Add( info );
		}
	}

	if ( PosTrayOptions.GetReportTaskHistoryFlag() == TRUE )
	{
		for ( int nTaskNo = 1; nTaskNo <= POSTRAY_MAX_TASK; nTaskNo++ )
		{
			CPosTrayEmailScheduleInfo info;
			info.m_nTaskType = EMAILTASK_TYPE_HIST;
			info.m_nTaskNo = nTaskNo;
			info.m_nTaskStatus = EMAILTASK_STATUS_UNCHECKED;
			info.InitRunNowFlag();
			m_arrayEmailTasks.Add( info );
		}
	}

	if ( PosTrayOptions.GetReportTaskStockLevelFlag() == TRUE )
	{
		for ( int nTaskNo = 1; nTaskNo <= POSTRAY_MAX_TASK; nTaskNo++ )
		{
			CPosTrayEmailScheduleInfo info;
			info.m_nTaskType = EMAILTASK_TYPE_STKLEVEL;
			info.m_nTaskNo = nTaskNo;
			info.m_nTaskStatus = EMAILTASK_STATUS_UNCHECKED;
			info.InitRunNowFlag();
			m_arrayEmailTasks.Add( info );
		}
	}

	if (SysInfo.IsPosTrayTimeAndAttendanceSystem() == TRUE)
	{
		if (PosTrayOptions.GetReportTaskTimeAttendFlag() == TRUE)
		{
			for (int nTaskNo = 1; nTaskNo <= POSTRAY_MAX_TASK; nTaskNo++)
			{
				CPosTrayEmailScheduleInfo info;
				info.m_nTaskType = EMAILTASK_TYPE_TIMEATTEND;
				info.m_nTaskNo = nTaskNo;
				info.m_nTaskStatus = EMAILTASK_STATUS_UNCHECKED;
				info.InitRunNowFlag();
				m_arrayEmailTasks.Add(info);
			}
		}
	}

	if ( PosTrayOptions.GetExportTaskSalesFlag() == TRUE )
	{
		for ( int nTaskNo = 1; nTaskNo <= POSTRAY_MAX_TASK; nTaskNo++ )
		{
			CPosTrayEmailScheduleInfo info;
			info.m_nTaskType = EMAILTASK_TYPE_EXPORT_SALES;
			info.m_nTaskNo = nTaskNo;
			info.m_nTaskStatus = EMAILTASK_STATUS_UNCHECKED;
			m_arrayEmailTasks.Add( info );
		}
	}

	if ( PosTrayOptions.GetExportTaskPaymentFlag() == TRUE )
	{
		for ( int nTaskNo = 1; nTaskNo <= POSTRAY_MAX_TASK; nTaskNo++ )
		{
			CPosTrayEmailScheduleInfo info;
			info.m_nTaskType = EMAILTASK_TYPE_EXPORT_PAYMENT;
			info.m_nTaskNo = nTaskNo;
			info.m_nTaskStatus = EMAILTASK_STATUS_UNCHECKED;
			m_arrayEmailTasks.Add( info );
		}
	}

	if ( PosTrayOptions.GetExportTaskVoidFlag() == TRUE )
	{
		for ( int nTaskNo = 1; nTaskNo <= POSTRAY_MAX_TASK; nTaskNo++ )
		{
			CPosTrayEmailScheduleInfo info;
			info.m_nTaskType = EMAILTASK_TYPE_EXPORT_VOID;
			info.m_nTaskNo = nTaskNo;
			info.m_nTaskStatus = EMAILTASK_STATUS_UNCHECKED;
			m_arrayEmailTasks.Add( info );
		}
	}

	if ( PosTrayOptions.GetExportTaskDiscountFlag() == TRUE )
	{
		for ( int nTaskNo = 1; nTaskNo <= POSTRAY_MAX_TASK; nTaskNo++ )
		{
			CPosTrayEmailScheduleInfo info;
			info.m_nTaskType = EMAILTASK_TYPE_EXPORT_DISCOUNT;
			info.m_nTaskNo = nTaskNo;
			info.m_nTaskStatus = EMAILTASK_STATUS_UNCHECKED;
			m_arrayEmailTasks.Add( info );
		}
	}

	UpdateTaskList();

	{
		CString strPathHost = "";
		GetPCPathHost( strPathHost );
		CreateSubdirectory( strPathHost );

		CString strPathClient = "";
		GetPCPathClient(strPathClient);

		if ( ( strPathClient != "" ) && ( strPathHost != strPathClient ) )
		{
			CreateSubdirectory( strPathClient );
		}
	}

	SysInfo.ClearFatalReadErrorFlag();
	DealerFlags.ClearFatalReadErrorFlag();
	EcrmanOptions.ClearFatalReadErrorFlag();

	if ( SysInfo.Read() == FALSE )
	{
		if ( SysInfo.GetFatalReadErrorFlag() )
		{
			SetStatus( POSUTILITY_STATUS_NO_SYSTEM );
			return;
		}
	}
		
	if ( DealerFlags.ReadFromFile() == FALSE )
	{
		if ( DealerFlags.GetFatalReadErrorFlag() )
		{
			SetStatus( POSUTILITY_STATUS_NO_SYSTEM );
			return;
		}
	}

	if ( EcrmanOptions.Read() == FALSE )
	{
		if ( EcrmanOptions.GetFatalReadErrorFlag() )
		{
			SetStatus( POSUTILITY_STATUS_NO_SYSTEM );
			return;
		}
	}

	if ( EmailOptions.Read() == FALSE )
	{
		if ( EmailOptions.GetFatalReadErrorFlag() )
		{
			Prompter.FatalReadError ( SysFiles::EmailOptions );
			return;
		}
	}

	if ( PCOptionsHost.Read() == FALSE )
	{
		if ( PCOptionsHost.GetFatalReadErrorFlag() )
		{
			Prompter.FatalReadError ( SysFiles::PCOptionsHost );
			return;
		}
	}

	if (PCOptionsClient.Read() == FALSE)
	{
		if (PCOptionsClient.GetFatalReadErrorFlag())
		{
			Prompter.FatalReadError(SysFiles::PCOptionsClient);
			return;
		}
	}

	if ( NetworkResources.Read() == FALSE )
	{
		if ( NetworkResources.GetFatalReadErrorFlag() )
		{
			Prompter.FatalReadError ( SysFiles::NetworkResources );
			return;
		}
	}
	NetworkResources.RunBatchFile();

	//OPENING AND CLOSING THIS FILE WILL CAUSE HEADER NAMES TO BE LOADED
	{
		DataManagerNonDb.PosTrayExportFieldNames.FixTaxNames();

		CDataManagerInfo info;
		DataManagerNonDb.OpenPosTrayExportFieldNames( DB_READONLY, info );

		if ( info.m_nErrorType == DB_ERR_SHARE )
		{
			Prompter.FatalReadError( SysFiles::PosTrayExportFieldNames );
			return;
		}

		DataManagerNonDb.ClosePosTrayExportFieldNames( info );
	}

	if ( PosTrayOptions.GetDisableHibernateFlag() == TRUE )
	{
		SetThreadExecutionState( ES_CONTINUOUS | ES_SYSTEM_REQUIRED );
		m_bBlockHibernation = TRUE;
	}
	else
	{
		SetThreadExecutionState( ES_CONTINUOUS );
		m_bBlockHibernation = FALSE;
	}

	SystemMapFull.CreateSystem();
	SystemMapFull.FixWebSalesNetworks();

	m_arrayNetworkInfo.RemoveAll();
	LocationMasterMap.Reset();
	for ( int nNetWkIdx = 0; nNetWkIdx < dbNetwork.GetSize(); nNetWkIdx++ )
	{
		if ( dbNetwork.GetConnectionType( nNetWkIdx ) == CONNECTION_TYPE_STANDARD_REALTIME )
		{
			CString strSPOSVersion = "";
			dbNetwork.GetSPOSVersionTreeName( nNetWkIdx, strSPOSVersion, FALSE );
			
			CPosTrayNetworkInfo info;
			info.m_nNetworkIdx = nNetWkIdx;
			info.m_strNetworkName = dbNetwork.GetName( nNetWkIdx );
			info.m_strSPOSVersion = strSPOSVersion;
			dbNetwork.GetSafeFileNetworkPath( nNetWkIdx, info.m_strNetworkPath );
			m_arrayNetworkInfo.Add( info );

			int nLocIdxStart, nLocIdxEnd;
			dbLocation.GetNetworkLocIdxRange( nNetWkIdx, nLocIdxStart, nLocIdxEnd );

			//CASHRSP FOLDERS BY TERMINAL
			for ( int nLocIdx = nLocIdxStart; nLocIdx <= nLocIdxEnd; nLocIdx++ )
			{
				CArray<CLocationMasterInfo,CLocationMasterInfo> arrayFolders;
				LocationMasterMap.GetLocationMasterList( nLocIdx, arrayFolders, FALSE, TRUE );

				for ( int n = 0; n < arrayFolders.GetSize(); n++ )
				{
					CLocationMasterInfo infoMaster = arrayFolders.GetAt( n );
				
					CPosTrayNetworkInfo info;
					info.m_nNetworkIdx = nNetWkIdx;
					
					info.m_strNetworkName = "==> ";
					info.m_strNetworkName += dbLocation.GetName( nLocIdx );

					CString strTerminalName = infoMaster.GetTerminalName();
					::TrimSpaces( strTerminalName, FALSE );
					if ( strTerminalName == "" )
						strTerminalName.Format( "Terminal %d", infoMaster.GetTerminalNo() );

					info.m_strSPOSVersion.Format( "TNo %3.3d (%s)", 
						infoMaster.GetTerminalNo(), 
						(const char*) strTerminalName );
					
					info.m_strNetworkPath = infoMaster.GetCashRSPFolder();
					m_arrayNetworkInfo.Add( info );
				}
			}
		}
	}
	UpdateNetworkList();

	m_arrayFNBExports.RemoveAll();
	if ( PosTrayOptions.GetFNBTaskFlag() == TRUE )
	{
		for ( int nDbIdx = 0; nDbIdx < dbDatabase.GetSize(); nDbIdx++ )
		{
			CPosTrayFNBScheduleInfo info;
			info.m_nDbIdx = nDbIdx;
			info.m_strDbName = dbDatabase.GetName( nDbIdx );
			info.m_nExportStatus = BESPOKE_EXPORT_STATUS_UNCHECKED;
			m_arrayFNBExports.Add( info );
		}
	}
	UpdateFNBList();

	m_arrayChartwellsExports.RemoveAll();
	if (PosTrayOptions.GetChartwellsTaskFlag() == TRUE)
	{
		CPosTrayChartwellsScheduleInfo info;
		info.m_nExportStatus = BESPOKE_EXPORT_STATUS_UNCHECKED;
		m_arrayChartwellsExports.Add(info);
	}
	UpdateChartwellsList();

	m_arrayS4LabourExports.RemoveAll();
	if ( PosTrayOptions.GetS4LabourTaskFlag() == TRUE )
	{
		for ( int nLocIdx = 0; nLocIdx < dbLocation.GetSize(); nLocIdx++ )
		{
			CPosTrayS4LabourScheduleInfo info;
			info.m_nLocIdx = nLocIdx;
			info.m_strLocName = dbLocation.GetName( nLocIdx );
			info.m_nExportStatus = BESPOKE_EXPORT_STATUS_UNCHECKED;
			m_arrayS4LabourExports.Add( info );
		}
	}
	UpdateS4LabourList();

	m_LIPAExport.m_nExportStatus = BESPOKE_EXPORT_STATUS_UNCHECKED;

	m_CashRSPBuffer.SetPendingNetworkInfo( m_arrayNetworkInfo );
	
	CreateSyssetSubdirectory ( "SchedLog" );
	CreateSyssetSubdirectory ( "SchedOld" );

	CommsHandshaker.BeginComms();
	CommsHandshaker.ResumeComms();

	SetStatus( POSUTILITY_STATUS_GOT_SYSTEM );
}

/**********************************************************************/

void CMainFramePosTray::OnTrayStatus() 
{	
	if ( m_pStatusDlg == NULL )
	{
		m_pStatusDlg = new CPosTrayStatusDlg( &m_pStatusDlg, this );
		m_pStatusDlg -> Create( IDD_POSTRAY_STATUS, this );
		m_pStatusDlg -> ShowWindow( SW_SHOW );
	}
	
	m_pStatusDlg -> SetFocus();
	UpdateNetworkList();
	UpdateTaskList();
	UpdateGeneralList();
}

/**********************************************************************/

void CMainFramePosTray::UpdateNetworkList()
{
	CStringArray arrayList;

	{
		for (int n = 0; n < m_arrayNetworkInfo.GetSize(); n++)
		{
			CPosTrayNetworkInfo info = m_arrayNetworkInfo.GetAt(n);

			CCSV csvLine('\t');
			csvLine.Add(info.m_strNetworkName);
			csvLine.Add(info.m_strSPOSVersion);
			csvLine.Add(info.m_strNetworkPath);
			csvLine.Add(CommsHandshaker.GetTranCountForPosTray(info.m_strNetworkPath));
			arrayList.Add(csvLine.GetLine());
		}
	}

	CString strStatus;
	GetStatusLine(strStatus);

	if (m_pStatusDlg != NULL)
	{
		m_pStatusDlg->UpdateNetworkList(strStatus, arrayList, m_CurrentStatusTime);
	}
}

/**********************************************************************/

void CMainFramePosTray::UpdateTaskList()
{
	CStringArray arrayListProcessTran;
	CStringArray arrayListReportEpos;
	CStringArray arrayListReportHist;
	CStringArray arrayListReportStkLevel;
	CStringArray arrayListReportTimeAttend;
	CStringArray arrayListExportSales;
	CStringArray arrayListExportPayment;
	CStringArray arrayListExportVoid;
	CStringArray arrayListExportDiscount;

	bool bGotUnchecked = FALSE;
	for ( int n = 0; ( n < m_arrayEmailTasks.GetSize() ) && ( FALSE == bGotUnchecked ); n++ )
	{
		CPosTrayEmailScheduleInfo infoTask = m_arrayEmailTasks.GetAt( n );
		
		CString strTaskAction, strTaskType;
		infoTask.GetTaskTypeStrings( strTaskAction, strTaskType );

		CString strLine = "";

		switch( infoTask.m_nTaskStatus )
		{
		case EMAILTASK_STATUS_WAIT_TASK:
			{
				CCSV csvActions;
				if ( infoTask.m_bWantEmail )	csvActions.Add( "Email" );
				if ( infoTask.m_bWantExport )	csvActions.Add( "Export" );
				if ( infoTask.m_bWantPrint )	csvActions.Add( "Print" );

				CString strStatus = "";

				if (infoTask.m_nTaskType != EMAILTASK_TYPE_PROCESSTRAN)
				{
					strStatus.Format("Task due at %s (%s)",
						GetFormattedTimeString(infoTask.m_timeTaskDue),
						(const char*) csvActions.GetLine());
				}
				else
				{
					strStatus.Format("Task due at %s",
						GetFormattedTimeString(infoTask.m_timeTaskDue));
				}

				CCSV csvLine ( '\t' );
				csvLine.Add( infoTask.m_strTaskName );
				csvLine.Add( strStatus );
				strLine = csvLine.GetLine();
			}
			break;

		case EMAILTASK_STATUS_WAIT_CASHRSP:
			{
				CString strStatus = "Task due at ";
				strStatus += GetFormattedTimeString( infoTask.m_timeTaskDue );
				strStatus += " (Processing Sales";
				strStatus += m_CashRSPBuffer.GetPendingWorkString();
				strStatus += ")";

				CCSV csvLine ( '\t' );
				csvLine.Add( infoTask.m_strTaskName );
				csvLine.Add( strStatus );
				strLine = csvLine.GetLine();
			}
			break;

		case EMAILTASK_STATUS_WAIT_RETRYFOLDER:
			{
				CCSV csvLine ( '\t' );
				csvLine.Add( infoTask.m_strTaskName );
				
				CString strRetry;
				strRetry.Format( "(Unable to access export folder, retry %2.2d:%2.2d:%2.2d)",
					infoTask.m_timeTaskRetryFolder.GetHour(),
					infoTask.m_timeTaskRetryFolder.GetMinute(),
					infoTask.m_timeTaskRetryFolder.GetSecond() );
				csvLine.Add( strRetry );

				strLine = csvLine.GetLine();
			}
			break;

		case EMAILTASK_STATUS_INVALID:
			{
				CCSV csvLine ( '\t' );
				csvLine.Add( infoTask.m_strTaskName );
				
				switch( infoTask.m_nTaskError )
				{
				case EMAILTASK_VALIDATE_NOBATCH:		csvLine.Add( "(No batch of jobs linked to task)" );				break;
				case EMAILTASK_VALIDATE_NOJOBS:			csvLine.Add( "(No reports selected for task)" );				break;
				case EMAILTASK_VALIDATE_NODAYS:			csvLine.Add( "(No weekdays selected for task)" );				break;
				case EMAILTASK_VALIDATE_NOCONTYPE:		csvLine.Add( "(No consolidation types selected)" );				break;
				case EMAILTASK_VALIDATE_NOEMAIL:		csvLine.Add( "(No email address specified)" );					break;
				case EMAILTASK_VALIDATE_NOACTION:		csvLine.Add( "(No email, export or print action selected)" );	break;
				case EMAILTASK_VALIDATE_WRONGTYPE:		csvLine.Add( "(Unknown task type)" );							break;	
				default:								csvLine.Add( "(Unknown task validation error)" );				break;
				}
				
				strLine = csvLine.GetLine();
			}
			break;

		case EMAILTASK_STATUS_WAIT_UPDATE:
			{
				CCSV csvLine ( '\t' );
				csvLine.Add( infoTask.m_strTaskName );
				csvLine.Add( "(Waiting to update task)" );
				strLine = csvLine.GetLine();
			}
			break;

		case EMAILTASK_STATUS_UNCHECKED:
			bGotUnchecked = TRUE;
			break;
		}

		if ( strLine != "" )
		{
			switch( infoTask.m_nTaskType )
			{
			case EMAILTASK_TYPE_PROCESSTRAN:	arrayListProcessTran.Add(strLine);			break;
			case EMAILTASK_TYPE_HIST:			arrayListReportHist.Add( strLine );			break;
			case EMAILTASK_TYPE_EPOS:			arrayListReportEpos.Add( strLine );			break;
			case EMAILTASK_TYPE_STKLEVEL:		arrayListReportStkLevel.Add( strLine );		break;
			case EMAILTASK_TYPE_TIMEATTEND:		arrayListReportTimeAttend.Add(strLine);		break;
			case EMAILTASK_TYPE_EXPORT_SALES:	arrayListExportSales.Add( strLine );		break;
			case EMAILTASK_TYPE_EXPORT_PAYMENT:	arrayListExportPayment.Add( strLine );		break;
			case EMAILTASK_TYPE_EXPORT_VOID:	arrayListExportVoid.Add( strLine );			break;
			case EMAILTASK_TYPE_EXPORT_DISCOUNT:arrayListExportDiscount.Add( strLine );		break;
			}
		}
	}

	if ( FALSE == bGotUnchecked )
	{
		CString strStatus;
		GetStatusLine( strStatus );

		if ( m_pStatusDlg != NULL )
		{
			m_pStatusDlg -> UpdateStatusLine( strStatus, m_CurrentStatusTime );
			m_pStatusDlg -> UpdateTaskListProcessTran(arrayListProcessTran);
			m_pStatusDlg -> UpdateTaskListReportEpos( arrayListReportEpos );
			m_pStatusDlg -> UpdateTaskListReportHist( arrayListReportHist );
			m_pStatusDlg -> UpdateTaskListReportStkLevel( arrayListReportStkLevel );
			m_pStatusDlg -> UpdateTaskListReportTimeAttend(arrayListReportTimeAttend );
			m_pStatusDlg -> UpdateTaskListExportSales( arrayListExportSales );
			m_pStatusDlg -> UpdateTaskListExportPayment( arrayListExportPayment );
			m_pStatusDlg -> UpdateTaskListExportVoid( arrayListExportVoid );
			m_pStatusDlg -> UpdateTaskListExportDiscount( arrayListExportDiscount );
		}
	}
}

/**********************************************************************/

void CMainFramePosTray::UpdateFNBList()
{
	CStringArray arrayList;

	bool bGotUnchecked = FALSE;
	for ( int n = 0; ( n < m_arrayFNBExports.GetSize() ) && ( FALSE == bGotUnchecked ); n++ )
	{
		CPosTrayFNBScheduleInfo infoExport = m_arrayFNBExports.GetAt( n );
		
		switch( infoExport.m_nExportStatus )
		{
		case BESPOKE_EXPORT_STATUS_WAIT_EXPORT:
			{
				CString strStatus = "Export due at ";
				strStatus += GetFormattedTimeString( infoExport.m_timeExportDue );

				CCSV csvLine ( '\t' );
				csvLine.Add( dbDatabase.GetDbNo( infoExport.m_nDbIdx ) );
				csvLine.Add( infoExport.m_strDbName );
				csvLine.Add( strStatus );
				arrayList.Add( csvLine.GetLine() );
			}
			break;

		case BESPOKE_EXPORT_STATUS_WAIT_CASHRSP:
			{
				CString strStatus = "Export due at ";
				strStatus += GetFormattedTimeString( infoExport.m_timeExportDue );
				strStatus += " (Processing Sales)";

				CCSV csvLine ( '\t' );
				csvLine.Add( dbDatabase.GetDbNo( infoExport.m_nDbIdx ) );
				csvLine.Add( infoExport.m_strDbName );
				csvLine.Add( strStatus );
				arrayList.Add( csvLine.GetLine() );
			}
			break;

		case BESPOKE_EXPORT_STATUS_INVALID:
			{
				CCSV csvLine ( '\t' );
				csvLine.Add( dbDatabase.GetDbNo( infoExport.m_nDbIdx ) );
				csvLine.Add( infoExport.m_strDbName );
				
				arrayList.Add( csvLine.GetLine() );
			}
			break;

		case BESPOKE_EXPORT_STATUS_WAIT_UPDATE:
			{
				CCSV csvLine ( '\t' );
				csvLine.Add( dbDatabase.GetDbNo( infoExport.m_nDbIdx ) );
				csvLine.Add( infoExport.m_strDbName );
				csvLine.Add( "(Waiting to update export status)" );
				arrayList.Add( csvLine.GetLine() );
			}
			break;

		case BESPOKE_EXPORT_STATUS_DISABLED:
			{
				CCSV csvLine ( '\t' );
				csvLine.Add( dbDatabase.GetDbNo( infoExport.m_nDbIdx ) );
				csvLine.Add( infoExport.m_strDbName );
				csvLine.Add( "(FNB Export Disabled)" );
				arrayList.Add( csvLine.GetLine() );
			}
			break;

		case BESPOKE_EXPORT_STATUS_UNCHECKED:
			bGotUnchecked = TRUE;
			break;
		}
	}

	if (FALSE == bGotUnchecked)
	{
		CString strStatus;
		GetStatusLine(strStatus);

		if (m_pStatusDlg != NULL)
		{
			m_pStatusDlg->UpdateFNBList(strStatus, arrayList, m_CurrentStatusTime);
		}
	}
}

/**********************************************************************/

void CMainFramePosTray::UpdateChartwellsList()
{
	CStringArray arrayList;

	bool bGotUnchecked = FALSE;
	for (int n = 0; (n < m_arrayChartwellsExports.GetSize()) && (FALSE == bGotUnchecked); n++)
	{
		CPosTrayChartwellsScheduleInfo infoExport = m_arrayChartwellsExports.GetAt(n);

		CString strDbStatus = "Not Checked";

		switch (infoExport.m_nDatabaseError)
		{
		case 0:
			strDbStatus = infoExport.m_strLastDbCheckTime + "   OK";
			break;

		case 1:
			strDbStatus = infoExport.m_strLastDbCheckTime + "   Cannot Access Database";
			break;

		case 2:
			strDbStatus = infoExport.m_strLastDbCheckTime + "   No Version Info";
			break;

		case 3:
			strDbStatus = infoExport.m_strLastDbCheckTime + "   Incompatible Database Version";
			break;

		case 4:
			strDbStatus = infoExport.m_strLastDbCheckTime + "   Incompatible External Version";
			break;
		}

		switch (infoExport.m_nExportStatus)
		{
		case BESPOKE_EXPORT_STATUS_WAIT_EXPORT:
		{
			CString strStatus = "Export due at ";
			strStatus += GetFormattedTimeString(infoExport.m_timeExportDue);

			CCSV csvLine('\t');
			csvLine.Add(strDbStatus);
			csvLine.Add(strStatus);
			arrayList.Add(csvLine.GetLine());
		}
		break;

		case BESPOKE_EXPORT_STATUS_WAIT_CASHRSP:
		{
			CString strStatus = "Export due at ";
			strStatus += GetFormattedTimeString(infoExport.m_timeExportDue);
			strStatus += " (Processing Sales)";

			CCSV csvLine('\t');
			csvLine.Add(strDbStatus);
			csvLine.Add(strStatus);
			arrayList.Add(csvLine.GetLine());
		}
		break;

		case BESPOKE_EXPORT_STATUS_INVALID:
		{
			CCSV csvLine('\t');
			csvLine.Add(strDbStatus);
			csvLine.Add("Invalid Task");
			arrayList.Add(csvLine.GetLine());
		}
		break;

		case BESPOKE_EXPORT_STATUS_WAIT_UPDATE:
		{
			CCSV csvLine('\t');
			csvLine.Add(strDbStatus);
			csvLine.Add("(Waiting to update export status)");
			arrayList.Add(csvLine.GetLine());
		}
		break;

		case BESPOKE_EXPORT_STATUS_DISABLED:
		{
			CCSV csvLine('\t');
			csvLine.Add(strDbStatus);
			csvLine.Add("(Chartwells Export Disabled)");
			arrayList.Add(csvLine.GetLine());
		}
		break;

		case BESPOKE_EXPORT_STATUS_UNCHECKED:
			bGotUnchecked = TRUE;
			break;
		}
	}

	if (FALSE == bGotUnchecked)
	{
		CString strStatus = "";
		GetStatusLine(strStatus);

		if (m_pStatusDlg != NULL)
		{
			m_pStatusDlg->UpdateChartwellsList(strStatus, arrayList, m_CurrentStatusTime);
		}
	}
}

/**********************************************************************/

void CMainFramePosTray::UpdateS4LabourList()
{
	MarkS4LabourLocIDDuplicates();

	CStringArray arrayList;

	bool bGotUnchecked = FALSE;
	for ( int n = 0; ( n < m_arrayS4LabourExports.GetSize() ) && ( FALSE == bGotUnchecked ); n++ )
	{
		CPosTrayS4LabourScheduleInfo infoExport = m_arrayS4LabourExports.GetAt( n );

		CString strLocNo;
		strLocNo.Format( "%d.%d", dbLocation.GetNetworkNo( infoExport.m_nLocIdx ), dbLocation.GetLocNo( infoExport.m_nLocIdx ) );
		
		switch( infoExport.m_nExportStatus )
		{
		case BESPOKE_EXPORT_STATUS_WAIT_EXPORT:
			{
				CString strStatus = "";
				
				if ( infoExport.m_bDuplicateID )
					strStatus = "(Duplicated Location ID)";
				else
				{
					strStatus = "Export due at ";
					strStatus += GetFormattedTimeString( infoExport.m_timeExportDue );
				}

				CCSV csvLine ( '\t' );
				csvLine.Add( strLocNo );
				csvLine.Add( infoExport.m_strLocName );
				csvLine.Add( strStatus );
				arrayList.Add( csvLine.GetLine() );
			}
			break;

		case BESPOKE_EXPORT_STATUS_WAIT_CASHRSP:
			{
				CString strStatus = "";
				
				if ( infoExport.m_bDuplicateID )
					strStatus = "(Duplicated Location ID)";
				else
				{
					strStatus = "Export due at ";
					strStatus += GetFormattedTimeString( infoExport.m_timeExportDue );
					strStatus += " (Processing Sales)";
				}
				
				CCSV csvLine ( '\t' );
				csvLine.Add( strLocNo );
				csvLine.Add( infoExport.m_strLocName );
				csvLine.Add( strStatus );
				arrayList.Add( csvLine.GetLine() );
			}
			break;

		case BESPOKE_EXPORT_STATUS_INVALID:
			{
				CCSV csvLine ( '\t' );
				csvLine.Add( strLocNo );
				csvLine.Add( infoExport.m_strLocName );

				switch( infoExport.m_nExportError )
				{
				case S4LABOUR_VALIDATE_NOID:		csvLine.Add( "(No ID defined for location)" );				break;
				case S4LABOUR_VALIDATE_NODB:		csvLine.Add( "(Location not linked to a database)" );		break;
				default:							csvLine.Add( "(Unknown task validation error)" );			break;
				}
				
				arrayList.Add( csvLine.GetLine() );
			}
			break;

		case BESPOKE_EXPORT_STATUS_WAIT_UPDATE:
			{
				CCSV csvLine ( '\t' );
				csvLine.Add( strLocNo );
				csvLine.Add( infoExport.m_strLocName );
				csvLine.Add( "(Waiting to update export status)" );
				arrayList.Add( csvLine.GetLine() );
			}
			break;

		case BESPOKE_EXPORT_STATUS_DISABLED:
			{
				CCSV csvLine ( '\t' );
				csvLine.Add( strLocNo );
				csvLine.Add( infoExport.m_strLocName );
				csvLine.Add( "(S4 Labour Export Disabled)" );
				arrayList.Add( csvLine.GetLine() );
			}
			break;

		case BESPOKE_EXPORT_STATUS_UNCHECKED:
			bGotUnchecked = TRUE;
			break;
		}
	}

	if (FALSE == bGotUnchecked)
	{
		CString strStatus;
		GetStatusLine(strStatus);

		if (m_pStatusDlg != NULL)
		{
			m_pStatusDlg->UpdateS4LabourList(strStatus, arrayList, m_CurrentStatusTime);
		}
	}
}

/**********************************************************************/

void CMainFramePosTray::UpdateLIPAList()
{
	CStringArray arrayList;
	bool bGotUnchecked = FALSE;
	
	switch( m_LIPAExport.m_nExportStatus )
	{
	case BESPOKE_EXPORT_STATUS_WAIT_EXPORT:
		{
			CString strStatus = "";		
			strStatus = "Export due at ";
			strStatus += GetFormattedTimeString( m_LIPAExport.m_timeExportDue );
		
			CCSV csvLine ( '\t' );
			csvLine.Add( "System" );
			csvLine.Add( strStatus );
			arrayList.Add( csvLine.GetLine() );
		}
		break;

	case BESPOKE_EXPORT_STATUS_WAIT_CASHRSP:
		{
			CString strStatus = "";
				
			strStatus = "Export due at ";
			strStatus += GetFormattedTimeString( m_LIPAExport.m_timeExportDue );
			strStatus += " (Processing Sales)";
				
			CCSV csvLine ( '\t' );
			csvLine.Add( "System" );
			csvLine.Add( strStatus );
			arrayList.Add( csvLine.GetLine() );
		}
		break;

	case BESPOKE_EXPORT_STATUS_WAIT_UPDATE:
		{
			CCSV csvLine ( '\t' );
			csvLine.Add( "System" );
			csvLine.Add( "(Waiting to update export status)" );
			arrayList.Add( csvLine.GetLine() );
		}
		break;

	case BESPOKE_EXPORT_STATUS_UNCHECKED:
		bGotUnchecked = TRUE;
		break;
	}

	if (FALSE == bGotUnchecked)
	{
		CString strStatus;
		GetStatusLine(strStatus);

		if (m_pStatusDlg != NULL)
		{
			m_pStatusDlg->UpdateLIPAList(strStatus, arrayList, m_CurrentStatusTime);
		}
	}
}

/**********************************************************************/

void CMainFramePosTray::UpdateBackupList()
{
	CStringArray arrayList;

	CString strTime = "";
	CString strMessage = "";

	CSSFile fileMessage;

	{
		CFilenameUpdater FnUp(SysFiles::PosTrayBackupMessage);
		if (fileMessage.Open(FnUp.GetFilenameToUse(), "rb") == TRUE)
		{
			CString strLine = "";
			if (fileMessage.ReadLine(strLine) == TRUE)
			{
				CCSV csv(strLine);
				if (csv.GetSize() == 2)
				{
					strTime = csv.GetString(0);
					strMessage = csv.GetString(1);
				}
			}
			fileMessage.Close();
		}
	}

	CString strResultDate = "";
	CString strResultTime = "";
	CString strResultMessage = "";

	{
		CFilenameUpdater FnUp(SysFiles::PosTrayBackupResult);
		if (fileMessage.Open(FnUp.GetFilenameToUse(), "rb") == TRUE)
		{
			CString strLine = "";
			if (fileMessage.ReadLine(strLine) == TRUE)
			{
				CCSV csv(strLine);
				if (csv.GetSize() == 4)
				{
					strResultDate = csv.GetString(0);
					strResultTime = csv.GetString(1);
					strResultMessage = csv.GetString(3);
				}
			}
			fileMessage.Close();
		}
	}

	arrayList.Add("");
	arrayList.Add("");
	arrayList.Add("");

	if ( ( strTime != "" ) && ( strMessage != "" ) )
	{
		arrayList.SetAt(0, strTime + CString("\t") + strMessage);
	}

	if ((strResultTime != "") && (strResultDate != "") && ( strResultMessage != "" ) )
	{
		arrayList.SetAt(2, strResultTime + CString(" on ") + strResultDate + CString("\t") + strResultMessage);
	}

	CString strStatus = "";
	GetStatusLine(strStatus);

	if (m_pStatusDlg != NULL)
	{
		m_pStatusDlg->UpdateBackupList(strStatus, arrayList, m_CurrentStatusTime);
	}
}

/**********************************************************************/

void CMainFramePosTray::UpdateGeneralList()
{
	CStringArray arrayList;
	arrayList.Add("");

	{
		CString strLine;
		strLine.Format("\t%s data path : %s",
			(const char*)m_strProgramName,
			(const char*)Sysset.GetProgramPath());

		arrayList.Add(strLine);
		arrayList.Add("");
	}

	if (PosTrayOptions.GetDisableHibernateFlag() == TRUE)
	{
		arrayList.Add("\tAutomatic hibernation is blocked on this PC.");
		arrayList.Add("");
	}

	CString strLine;
	if (PosTrayOptions.GetPosTrayRealTimeFlag() == FALSE)
	{
		strLine.Format("\tReal time communications are performed by %s.",
			(const char*)m_strProgramName);

		arrayList.Add(strLine);
	}
	else
	{
		strLine.Format("\tReal time communications speed is set to %d.",
			PosTrayOptions.GetRealTimeCommsSpeed());

		arrayList.Add(strLine);
	}

	if (PosTrayOptions.AreTasksEnabled() == FALSE)
	{
		arrayList.Add("");
		arrayList.Add("\tAutomatic email is disabled for this system.");
	}

	if (PosTrayOptions.GetFNBTaskFlag() == FALSE)
	{
		arrayList.Add("");
		arrayList.Add("\tFNB export is disabled for this system.");
	}

	CString strStatus;
	GetStatusLine(strStatus);

	if (m_pStatusDlg != NULL)
	{
		m_pStatusDlg->UpdateGeneralList(strStatus, arrayList, m_CurrentStatusTime);
	}
}

/**********************************************************************/

void CMainFramePosTray::GetStatusLine( CString& strLine )
{
	if ( PosTrayLock.LockedByEcrman() == TRUE )
	{
		strLine.Format ( "PosTray has been paused by %s (Instance %d)",
			(const char*) m_strProgramName,
			PosTrayLock.GetLockedInstanceNo() );
	}
	else
	{
		switch( m_nPosTrayStatus )
		{
		case POSUTILITY_STATUS_NOT_CHECKED:
			strLine.Format ( "Waiting to check %s system files.",
				(const char*) m_strProgramName );
			break;

		case POSUTILITY_STATUS_CHECKING:
			strLine.Format ( "Checking %s system files.",
				(const char*) m_strProgramName );
			break;

		case POSUTILITY_STATUS_NO_SYSTEM:
			strLine.Format ( "No %s system files detected.",
				(const char*) m_strProgramName );
			break;

		case POSUTILITY_STATUS_UPDATING:
			strLine = "Checking updated settings for PosTray.";
			break;

		case POSUTILITY_STATUS_WAIT_INITSYSTEM:
			strLine = "Waiting to pause comms before checking updated settings for PosTray.";
			break;

		case POSUTILITY_STATUS_WAIT_TASK:
			strLine = "Waiting to pause comms before attempting task.";
			break;

		default:
			strLine = "";
			break;
		}
	}
}

/**********************************************************************/

void CMainFramePosTray::OnTrayClose() 
{
	CloseTray( TRUE );
}

/**********************************************************************/

void CMainFramePosTray::CloseTray( bool bWarn )
{
	if ( ( m_bUIBusy == TRUE ) && ( bWarn == TRUE ) )
		return;

	if ( bWarn == TRUE )
	{
		m_bUIBusy = TRUE;

		CString strMsg = "";
		strMsg += "Are you sure you want to close PosTray\n\n";
		strMsg += "Closing this utility will delay the processing of sales reports\n";
		strMsg += "and emails for the ";
		strMsg += m_strProgramName;
		strMsg += " system";

		if ( Prompter.YesNo( strMsg ) != IDYES )
		{
			MessageLogger.LogPosTrayClose( 0 );
			m_bUIBusy = FALSE;
			return;
		}
	}

	MessageLogger.LogPosTrayClose( bWarn ? 1 : 2 );
	CommsHandshaker.PauseComms();
	CommsHandshaker.TerminateComms();

#ifdef POSTRAY_SOCKET
	if ( Super.GetPosTraySocketFlag() == TRUE )
	{
		m_SocketServer.StopListening();
	}
#endif

	m_bWantQuit = TRUE;
	m_bReadyToAct = TRUE;
}

/**********************************************************************/

void CMainFramePosTray::UpdateTime( COleDateTime& time, int nMinute, int nSecond, bool bMod5 )
{
	time = COleDateTime::GetCurrentTime() + COleDateTimeSpan( 0, 0, nMinute, nSecond );

	if ( TRUE == bMod5 )
	{
		int nSecond = ( time.GetSecond() % 5 );
		if ( nSecond != 0 )
			time -= COleDateTimeSpan( 0, 0, 0, nSecond );
	}
}

/**********************************************************************/

const char* CMainFramePosTray::GetFormattedTimeString( COleDateTime& timeDue )
{
	m_strFormattedTime.Format ( "%2.2d:%2.2d:%2.2d on %2.2d/%2.2d/%4.4d",
		timeDue.GetHour(),
		timeDue.GetMinute(),
		timeDue.GetSecond(),
		timeDue.GetDay(),
		timeDue.GetMonth(),
		timeDue.GetYear() );

	return m_strFormattedTime;
}

/**********************************************************************/

void CMainFramePosTray::DisplayCashRSPStatus( const char* sz )
{
	if ( m_pStatusDlg != NULL )
		m_pStatusDlg -> DisplayCashRSPStatus( sz );
}

/**********************************************************************/

void CMainFramePosTray::MarkS4LabourLocIDDuplicates()
{
	CReportConsolidationArray<CSortedStringAndIntItem> arrayIDs;

	for ( int n = 0; n < m_arrayS4LabourExports.GetSize(); n++ )
	{
		m_arrayS4LabourExports[n].m_bDuplicateID = FALSE;

		CString strLocID = m_arrayS4LabourExports[n].m_strLocID;
		::TrimSpaces( strLocID, FALSE );

		if ( strLocID != "" )
		{
			strLocID.MakeUpper();
			
			CSortedStringAndIntItem item;
			item.m_strItem = strLocID;
			item.m_nItem = n;
			arrayIDs.Consolidate(item);
		}
	}

	CSortedStringAndIntItem itemCurr;
	CSortedStringAndIntItem itemLast;
	for ( int n = 0; n < arrayIDs.GetSize(); n++ )
	{
		arrayIDs.GetAt(n,itemCurr);

		if ( n != 0 )
		{
			if ( itemCurr.m_strItem == itemLast.m_strItem )
			{
				m_arrayS4LabourExports[itemLast.m_nItem].m_bDuplicateID = TRUE;
				m_arrayS4LabourExports[itemCurr.m_nItem].m_bDuplicateID = TRUE;
			}
		}

		itemLast = itemCurr;
	}
}

/**********************************************************************/
