/**********************************************************************/
#include "../CommonEcrStockTray/AutoBackupProcessor.h"
/**********************************************************************/
#include "../CommonEcrStock/BackgroundLayoutInfo.h"
#include "../CommonEcrStock/ButtonManager.h"
#include "../CommonEcrStock/ButtonOptions.h"
#include "../CommonEcrStock/DatabaseValidator.h"
/**********************************************************************/
#include "CommsHandshaker.h"
#include "StatusProgress.h"
#include "SysInfo.h"
#include "TimerNumbers.h"
#include "WMNumbers.h"
/**********************************************************************/
#include "MainFrmEcrman.h"
/**********************************************************************/

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_PROGRESS,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/**********************************************************************/

IMPLEMENT_DYNCREATE(CMainFrameEcrman, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrameEcrman, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrameEcrman)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_COMMAND(ID_APP_EXIT, OnAppExit)
	ON_WM_ENDSESSION()
	ON_WM_QUERYENDSESSION()
	ON_WM_GETMINMAXINFO()
	ON_COMMAND(IDM_ADMIN_PASSWORDS, OnAdminPasswords)
	ON_COMMAND(IDM_SETUP_SYSTEM, OnSetupSystem)
	ON_COMMAND(IDM_SETUP_PMS, OnSetupPMSOptions)
	ON_COMMAND(IDM_SETUP_POSTRAY, OnSetupPosTrayOptions)
	ON_COMMAND(IDM_SETUP_IDRAUGHT, OnSetupIDraughtOptions)
	ON_COMMAND(IDM_SETUP_SAGE, OnSetupSageOptions)
	ON_COMMAND(IDM_SETUP_FOLDER_PASSWORDS, OnSetupNetworkResources)
	ON_COMMAND(IDM_SETUP_OPTIONS, OnSetupOptions)
	ON_COMMAND(IDM_DATABASE_EDITDATA, OnDatabaseEditData)
	ON_COMMAND(IDM_BARCODE_NUMBERS, OnSetupBarcodeNumbers)
	ON_COMMAND(IDM_SETUP_TEXTS, OnSetupTexts)
	ON_COMMAND(IDM_FILE_BACKUP, OnFileBackup)
	ON_COMMAND(IDM_FILE_BACKUP_LOG, OnFileBackupLog)
	ON_COMMAND(IDM_FILE_BACKUP_STATUS, OnFileBackupStatus)
	ON_COMMAND(IDM_FILE_RESTORE_NORMAL, OnFileRestoreNormal)
	ON_COMMAND(IDM_FILE_RESTORE_FOLDER, OnFileRestoreFolder)
	ON_COMMAND(IDM_FILE_SAGE_LOG, OnFileSageLog)
	ON_COMMAND(IDM_FILE_WRITE_LOG, OnFileWriteLog)
	ON_COMMAND(IDM_REPORTS_EPOS, OnReportsEpos)
	ON_COMMAND(IDM_REPORTS_WORKING, OnReportsWorking)
	ON_COMMAND(IDM_REPORTS_SALESHISTORY_PLU, OnReportsSalesHistoryPlu)
	ON_COMMAND(IDM_REPORTS_SALESHISTORY_PLU_REGION, OnReportsSalesHistoryPluRegion)
	ON_COMMAND(IDM_REPORTS_SALESHISTORY_PLU_DATE, OnReportsSalesHistoryPluDateRange)
	ON_COMMAND(IDM_REPORTS_CUSTOMER, OnReportsCustomer)
	ON_COMMAND(IDM_REPORTS_CUSTOMER_STATEMENT, OnReportsCustomerStatement)
	ON_COMMAND(IDM_REPORTS_PLU_STOCK, OnReportsPluStock)
	ON_COMMAND(IDM_REPORTS_BRITANNIA, OnReportsBritannia)
	ON_COMMAND(IDM_REPORTS_IDRAUGHT, OnReportsIDraught)
	ON_COMMAND(IDM_ADMIN_DELETE_REPORTS, OnAdminDeleteReports)
	ON_COMMAND(IDM_ADMIN_DELETE_PLU_UNSOLD, OnAdminDeletePluUnsold)
	ON_COMMAND(IDM_ADMIN_DELETE_BARCODES, OnAdminDeleteBarcodes)
	ON_COMMAND(IDM_SETUP_BARCODE, OnSetupBarcode)
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_COMMAND(IDM_FILE_COMMSLOG_SYSTEM, OnFileCommsLogSystem)
	ON_COMMAND(IDM_FILE_EMAILLOG, OnFileEmailLog)
	ON_COMMAND(IDM_FILE_POSTRAY_ACTION, OnFilePosTrayLogAction)
	ON_COMMAND(IDM_FILE_POSTRAY_EXPORT, OnFilePosTrayLogExport)
	ON_COMMAND(IDM_FILE_FNBLOG, OnFileFNBLog)
	ON_COMMAND(IDM_FILE_S4LABOURLOG, OnFileS4LabourLog)
	ON_COMMAND(IDM_FILE_FTPLOG, OnFileFTPLog)
	ON_COMMAND(IDM_SETUP_TASKS, OnSetupTasks)
	ON_COMMAND(IDM_TASKS_ADHOC, OnTasksAdhoc)

	ON_COMMAND(IDM_JOB_PRESET1, OnJobPreset1)
	ON_UPDATE_COMMAND_UI(IDM_JOB_PRESET1, OnUpdateJobPreset1)
	ON_COMMAND(IDM_JOB_PRESET2, OnJobPreset2)
	ON_UPDATE_COMMAND_UI(IDM_JOB_PRESET2, OnUpdateJobPreset2)
	ON_COMMAND(IDM_JOB_PRESET3, OnJobPreset3)
	ON_UPDATE_COMMAND_UI(IDM_JOB_PRESET3, OnUpdateJobPreset3)
	ON_COMMAND(IDM_JOB_PRESET4, OnJobPreset4)
	ON_UPDATE_COMMAND_UI(IDM_JOB_PRESET4, OnUpdateJobPreset4)
	ON_COMMAND(IDM_JOB_PRESET5, OnJobPreset5)
	ON_UPDATE_COMMAND_UI(IDM_JOB_PRESET5, OnUpdateJobPreset5)

	ON_COMMAND(IDM_JOB_PRESET6, OnJobPreset6)
	ON_UPDATE_COMMAND_UI(IDM_JOB_PRESET6, OnUpdateJobPreset6)
	ON_COMMAND(IDM_JOB_PRESET7, OnJobPreset7)
	ON_UPDATE_COMMAND_UI(IDM_JOB_PRESET7, OnUpdateJobPreset7)
	ON_COMMAND(IDM_JOB_PRESET8, OnJobPreset8)
	ON_UPDATE_COMMAND_UI(IDM_JOB_PRESET8, OnUpdateJobPreset8)
	ON_COMMAND(IDM_JOB_PRESET9, OnJobPreset9)
	ON_UPDATE_COMMAND_UI(IDM_JOB_PRESET9, OnUpdateJobPreset9)
	ON_COMMAND(IDM_JOB_PRESET10, OnJobPreset10)
	ON_UPDATE_COMMAND_UI(IDM_JOB_PRESET10, OnUpdateJobPreset10)

	ON_COMMAND(IDM_JOB_PRESET11, OnJobPreset11)
	ON_UPDATE_COMMAND_UI(IDM_JOB_PRESET11, OnUpdateJobPreset11)
	ON_COMMAND(IDM_JOB_PRESET12, OnJobPreset12)
	ON_UPDATE_COMMAND_UI(IDM_JOB_PRESET12, OnUpdateJobPreset12)
	ON_COMMAND(IDM_JOB_PRESET13, OnJobPreset13)
	ON_UPDATE_COMMAND_UI(IDM_JOB_PRESET13, OnUpdateJobPreset13)
	ON_COMMAND(IDM_JOB_PRESET14, OnJobPreset14)
	ON_UPDATE_COMMAND_UI(IDM_JOB_PRESET14, OnUpdateJobPreset14)
	ON_COMMAND(IDM_JOB_PRESET15, OnJobPreset15)
	ON_UPDATE_COMMAND_UI(IDM_JOB_PRESET15, OnUpdateJobPreset15)

	ON_COMMAND(IDM_JOB_PRESET16, OnJobPreset16)
	ON_UPDATE_COMMAND_UI(IDM_JOB_PRESET16, OnUpdateJobPreset16)
	ON_COMMAND(IDM_JOB_PRESET17, OnJobPreset17)
	ON_UPDATE_COMMAND_UI(IDM_JOB_PRESET17, OnUpdateJobPreset17)
	ON_COMMAND(IDM_JOB_PRESET18, OnJobPreset18)
	ON_UPDATE_COMMAND_UI(IDM_JOB_PRESET18, OnUpdateJobPreset18)
	ON_COMMAND(IDM_JOB_PRESET19, OnJobPreset19)
	ON_UPDATE_COMMAND_UI(IDM_JOB_PRESET19, OnUpdateJobPreset19)
	ON_COMMAND(IDM_JOB_PRESET20, OnJobPreset20)
	ON_UPDATE_COMMAND_UI(IDM_JOB_PRESET20, OnUpdateJobPreset20)

	ON_UPDATE_COMMAND_UI(IDM_TASKS_ADHOC, OnUpdateTasksAdhoc)
	ON_COMMAND(IDM_SETUP_COMMS_SETTINGS, OnSetupCommsSettings)
	ON_COMMAND(IDM_FILE_COMMSLOG_NETWORK, OnFileCommsLogNetwork)
	//}}AFX_MSG_MAP
	ON_COMMAND(IDM_IMPORT_SQL_MAIN, OnImportSQL)
	ON_COMMAND(IDM_HELP_LICENCEDETAILS, OnHelpLicenceDetails)
	ON_COMMAND(IDM_HELP_ABOUT, OnHelpAbout)
	ON_COMMAND(IDM_LOYALTY, OnLoyalty)
	ON_COMMAND(IDM_SMARTPAY, OnSmartPay)
	ON_COMMAND(IDM_CALENDAR, OnPMSCalendar)
	ON_MESSAGE(WM_IDRAUGHT_EXPORT, OnIDraughtExport)
	ON_MESSAGE(WM_IDRAUGHT_TRANSFER, OnIDraughtTransfer)
	ON_MESSAGE(WM_SAGE_EXPORT, OnSageExport)
	ON_MESSAGE(WM_AUTOBACKUP, OnAutoBackup)
	ON_MESSAGE(WM_STARTUPMSG, OnStartupMsg)

	ON_COMMAND(IDM_SPECIALKEY_A, OnSpecialKeyA)
	ON_COMMAND(IDM_SPECIALKEY_B, OnSpecialKeyB)
	ON_COMMAND(IDM_SPECIALKEY_C, OnSpecialKeyC)
	ON_COMMAND(IDM_SPECIALKEY_D, OnSpecialKeyD)
	ON_COMMAND(IDM_SPECIALKEY_E, OnSpecialKeyE)
	ON_COMMAND(IDM_SPECIALKEY_F, OnSpecialKeyF)
	ON_COMMAND(IDM_SPECIALKEY_G, OnSpecialKeyG)
	ON_COMMAND(IDM_SPECIALKEY_H, OnSpecialKeyH)
	ON_COMMAND(IDM_SPECIALKEY_I, OnSpecialKeyI)
	ON_COMMAND(IDM_SPECIALKEY_J, OnSpecialKeyJ)
	ON_COMMAND(IDM_SPECIALKEY_K, OnSpecialKeyK)
	ON_COMMAND(IDM_SPECIALKEY_L, OnSpecialKeyL)
	ON_COMMAND(IDM_SPECIALKEY_M, OnSpecialKeyM)
	ON_COMMAND(IDM_SPECIALKEY_N, OnSpecialKeyN)
	ON_COMMAND(IDM_SPECIALKEY_O, OnSpecialKeyO)
	ON_COMMAND(IDM_SPECIALKEY_P, OnSpecialKeyP)
	ON_COMMAND(IDM_SPECIALKEY_Q, OnSpecialKeyQ)
	ON_COMMAND(IDM_SPECIALKEY_R, OnSpecialKeyR)
	ON_COMMAND(IDM_SPECIALKEY_S, OnSpecialKeyS)
	ON_COMMAND(IDM_SPECIALKEY_T, OnSpecialKeyT)
	ON_COMMAND(IDM_SPECIALKEY_U, OnSpecialKeyU)
	ON_COMMAND(IDM_SPECIALKEY_V, OnSpecialKeyV)
	ON_COMMAND(IDM_SPECIALKEY_W, OnSpecialKeyW)
	ON_COMMAND(IDM_SPECIALKEY_X, OnSpecialKeyX)
	ON_COMMAND(IDM_SPECIALKEY_Y, OnSpecialKeyY)
	ON_COMMAND(IDM_SPECIALKEY_Z, OnSpecialKeyZ)
	ON_COMMAND(IDM_SPECIALKEY_0, OnSpecialKey0)
	ON_COMMAND(IDM_SPECIALKEY_1, OnSpecialKey1)
	ON_COMMAND(IDM_SPECIALKEY_2, OnSpecialKey2)
	ON_COMMAND(IDM_SPECIALKEY_3, OnSpecialKey3)
	ON_COMMAND(IDM_SPECIALKEY_4, OnSpecialKey4)
	ON_COMMAND(IDM_SPECIALKEY_5, OnSpecialKey5)
	ON_COMMAND(IDM_SPECIALKEY_6, OnSpecialKey6)
	ON_COMMAND(IDM_SPECIALKEY_7, OnSpecialKey7)
	ON_COMMAND(IDM_SPECIALKEY_8, OnSpecialKey8)
	ON_COMMAND(IDM_SPECIALKEY_9, OnSpecialKey9)
	
END_MESSAGE_MAP()

/**********************************************************************/
extern CBitmap GlobalBitmap;
extern CBitmap GlobalMask;
/**********************************************************************/

CMainFrameEcrman::CMainFrameEcrman()
{
	for (int n = 0; n < 10; n++)
	{
		m_arrayPresetTaskIdx[n] = -1;
	}

	DatabaseValidator.SetErrorFlag( FALSE );
	DatabaseValidator.SetBusyFlag( FALSE );
	DatabaseValidator.SetDoingUpdateFlag( FALSE );
	
	m_hTimerIdle = NULL;
	m_pBackupResultDlg = NULL;
	m_pStartupMsgDlg = NULL;
	
	m_bAutoMenuEnable = FALSE;
	Housekeeping();
}

/**********************************************************************/

CMainFrameEcrman::~CMainFrameEcrman()
{
	if (m_pBackupResultDlg != NULL)
	{
		delete m_pBackupResultDlg;
	}

	if (m_pStartupMsgDlg != NULL)
	{
		delete m_pStartupMsgDlg;
	}
}

/**********************************************************************/

int CMainFrameEcrman::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      	
	}

	StatusProgress.SetParent ( &m_wndStatusBar );
	StatusProgress.SetPaneWidth ( 200 );

	BackgroundLayoutInfo.SetStatusBarHeight( m_wndStatusBar.CalcFixedLayout( TRUE, TRUE ).cy );

	CDWordArray ColourRefs;
	ColourRefs.Add ( 0x000000 );		// BLACK
	ColourRefs.Add ( 0x800000 );		// BLUE		
	ColourRefs.Add ( 0x008000 );		// GREEN
	ColourRefs.Add ( 0x808000 );		// CYAN
	ColourRefs.Add ( 0x000080 );		// RED
	ColourRefs.Add ( 0x800080 );		// MAGENTA
	ColourRefs.Add ( 0x008080 );		// YELLOW
	ColourRefs.Add ( 0xC0C0C0 );		// LIGHT GREY
	ColourRefs.Add ( 0x808080 );		// GREY
	ColourRefs.Add ( 0xFF0000 );		// LIGHT BLUE
	ColourRefs.Add ( 0x00FF00 );		// LIGHT GREEN
	ColourRefs.Add ( 0xFFFF00 );		// LIGHT CYAN
	ColourRefs.Add ( 0x0000FF );		// LIGHT RED
	ColourRefs.Add ( 0xFF00FF );		// LIGHT MAGENTA
	ColourRefs.Add ( 0x00FFFF );		// LIGHT YELLOW
	ColourRefs.Add ( 0xF0F0F0 );		// WHITE
	ColourRefs.Add ( 0xFFC0C0 );		// PASTEL BLUE
	ColourRefs.Add ( 0xC0FFC0 );		// PASTEL GREEN
	ColourRefs.Add ( 0xFFFFC0 );		// PASTEL CYAN
	ColourRefs.Add ( 0xC0C0FF );		// PASTEL RED
	ColourRefs.Add ( 0xFFC0FF );		// PASTEL MAGENTA
	ColourRefs.Add ( 0xC0FFFF );		// PASTEL YELLOW
		
	CClientDC dcScreen( this );
	
	GlobalBitmap.CreateCompatibleBitmap ( &dcScreen, 682, 16 );
	GlobalMask.CreateBitmap( 682, 16, 1, 1, NULL );

	CDC dcMem;
	dcMem.CreateCompatibleDC( &dcScreen );
	CBitmap* pOldBitmap = dcMem.SelectObject( &GlobalBitmap );

	dcMem.FillSolidRect ( CRect (   0, 0, 682, 16 ), COLORREF ( 0xFFFFFF ) );
	
	for ( int nPos = 0; nPos < 22; nPos++ )
	{
		CRect rect ( 2 + ( nPos * 31 ), 1, 27 + ( nPos * 31 ), 14 );
		dcMem.FillSolidRect ( rect, COLORREF ( ColourRefs.GetAt( nPos ) ) );
	}
	
	dcMem.SelectObject ( &GlobalMask );
	dcMem.FillSolidRect ( CRect (   0, 0, 682, 16 ), COLORREF ( 0xFFFFFF ) );
	
	for ( int nPos = 0; nPos <= 22; nPos++ )
		dcMem.FillSolidRect ( CRect ( ( nPos * 31 ) + 2, 1, ( nPos * 31 ) + 27, 14 ), COLORREF ( 0x000000 ) );
	
	dcMem.SelectObject ( pOldBitmap );

	m_MenuChanger.EatMenuTopLevel ( GetMenu() );

	if ( Sysset.IsBarmanSystem() == FALSE )
		m_MenuChanger.KillItem ( IDM_SETUP_BARCODE );

	SysInfo.SetLoyaltyOrSmartPayType( LOYALTY_TYPE_NONE );
	if ( Sysset.IsSmartcardSystem() == TRUE )
	{
		if (::FileExists(".\\SPOSLOYALTY4MANAGER.EXE") == TRUE)
			SysInfo.SetLoyaltyOrSmartPayType(LOYALTY_TYPE_LOY_V4);
		else if ( ::FileExists( ".\\SPOSLOYALTY3MANAGER.EXE" ) == TRUE )
			SysInfo.SetLoyaltyOrSmartPayType( LOYALTY_TYPE_LOY_V3 );
		else if ( ::FileExists( ".\\SPOSLOYALTYMANAGER.EXE" ) == TRUE )
			SysInfo.SetLoyaltyOrSmartPayType( LOYALTY_TYPE_LOY_V2 );
		else if (::FileExists(".\\SPOSSMARTPAY4MANAGER.EXE") == TRUE)
			SysInfo.SetLoyaltyOrSmartPayType(LOYALTY_TYPE_SMP_V4);
		else if ( ::FileExists( ".\\SPOSSMARTPAY2MANAGER.EXE" ) == TRUE )
			SysInfo.SetLoyaltyOrSmartPayType( LOYALTY_TYPE_SMP_V2 );
		else if ( ::FileExists( ".\\SPOSSMARTPAYMANAGER.EXE" ) == TRUE )
			SysInfo.SetLoyaltyOrSmartPayType( LOYALTY_TYPE_SMP_V1 );
		else
			SysInfo.SetLoyaltyOrSmartPayType( LOYALTY_TYPE_UNKNOWN );
	}

	if ( SysInfo.GotSmartPayLink() == FALSE )
	{
		m_MenuChanger.KillItem ( IDM_SMARTPAY );
	}
	
	if ( SysInfo.GotLoyaltyLink() == FALSE )
	{
		m_MenuChanger.KillParent(IDM_LOYALTY);

		/*
		if (SysInfo.GetLoyaltyOrSmartPayType() == LOYALTY_TYPE_UNKNOWN)
		{
			m_MenuChanger.KillItem(IDM_LOYALTY);
		}
		else
		{
			m_MenuChanger.KillParent(IDM_LOYALTY);
		}
		*/
	}
	
	if ( SysInfo.IsPMSSystem() == FALSE )
	{
		m_MenuChanger.KillItem( IDM_CALENDAR );
		m_MenuChanger.KillItem( IDM_SETUP_PMS );
	}

	if ( SysInfo.IsPosTraySystem() == FALSE )
	{
		m_MenuChanger.KillItem( IDM_SETUP_POSTRAY );
		m_MenuChanger.KillItem( IDM_FILE_EMAILLOG );
	}

	if ( SysInfo.IsIDraughtExportSystem() == FALSE )
	{
		m_MenuChanger.KillItem( IDM_SETUP_IDRAUGHT );
		m_MenuChanger.KillItem( IDM_REPORTS_IDRAUGHT );
	}

	if ( SysInfo.IsSageExportSystem() == FALSE )
	{
		m_MenuChanger.KillItem( IDM_SETUP_SAGE );
		m_MenuChanger.KillItem( IDM_FILE_SAGE_LOG );
	}

	m_MenuChanger.ShowItem( IDM_FILE_WRITE_LOG, DealerFlags.GetLogFileWritesFlag() );

	if ( SysInfo.IsEcrLiteSystem() == TRUE )
	{
		m_MenuChanger.KillItem( IDM_JOB_PRESET3 );
		m_MenuChanger.KillItem( IDM_JOB_PRESET4 );
		m_MenuChanger.KillItem( IDM_JOB_PRESET5 );
		m_MenuChanger.KillItem( IDM_JOB_PRESET6 );
		m_MenuChanger.KillItem( IDM_JOB_PRESET7 );
		m_MenuChanger.KillItem( IDM_JOB_PRESET8 );
		m_MenuChanger.KillItem( IDM_JOB_PRESET9 );
		m_MenuChanger.KillItem( IDM_JOB_PRESET10 );
		m_MenuChanger.KillItem( IDM_JOB_PRESET11 );
		m_MenuChanger.KillItem( IDM_JOB_PRESET12 );
		m_MenuChanger.KillItem( IDM_JOB_PRESET13 );
		m_MenuChanger.KillItem( IDM_JOB_PRESET14 );
		m_MenuChanger.KillItem( IDM_JOB_PRESET15 );
		m_MenuChanger.KillItem( IDM_JOB_PRESET16 );
		m_MenuChanger.KillItem( IDM_JOB_PRESET17 );
		m_MenuChanger.KillItem( IDM_JOB_PRESET18 );
		m_MenuChanger.KillItem( IDM_JOB_PRESET19 );
		m_MenuChanger.KillItem( IDM_JOB_PRESET20 );
		m_MenuChanger.KillItem( IDM_REPORTS_WORKING );
		m_MenuChanger.KillItem( IDM_REPORTS_SALESHISTORY_PLU_DATE );
		m_MenuChanger.KillItem( IDM_REPORTS_SALESHISTORY_PLU_REGION );
		m_MenuChanger.KillItem( IDM_REPORTS_CUSTOMER );
		m_MenuChanger.KillItem( IDM_REPORTS_CUSTOMER_STATEMENT );
		m_MenuChanger.KillItem( IDM_SETUP_SAGE );
		m_MenuChanger.KillItem( IDM_FILE_SAGE_LOG );
	}

	SetTitleBarText();
		
	m_MenuChanger.BuildMenu ( GetMenu() );
	DrawMenuBar();
	
	BuildJobMenu();

	if (SysInfo.GetScriptModeFlag() == FALSE)
	{
		m_hTimerIdle = SetTimer(TIMER_MAINFRAME, 1000, NULL);
	}

	return 0;
}

/**********************************************************************/

BOOL CMainFrameEcrman::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style &= ~FWS_ADDTOTITLE;

	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}

/**********************************************************************/

#ifdef _DEBUG
void CMainFrameEcrman::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrameEcrman::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/**********************************************************************/

void CMainFrameEcrman::ActivateFrame(int nCmdShow) 
{
	if ( BackgroundLayoutInfo.GotValidLayout() == TRUE )
		CFrameWnd::ActivateFrame(SW_MAXIMIZE);

	if ( m_hAccelTable != NULL )
	{
		DestroyAcceleratorTable( m_hAccelTable );
		m_hAccelTable = NULL;
	}

	LoadAccelTable( MAKEINTRESOURCE(IDR_SHARED_SPECIALKEYS));
}

/**********************************************************************/

void CMainFrameEcrman::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	if ( ( BackgroundLayoutInfo.GotValidLayout() == TRUE ) && ( ButtonOptions.GetLayoutEnable() == TRUE ) )
	{
		CRect rcMin(0, 0, 500, 200 );
		CRect rcMax(0, 0, BackgroundLayoutInfo.GetWindowSizeRequestX(), BackgroundLayoutInfo.GetWindowSizeRequestY() );
		lpMMI->ptMaxPosition.x = 1;
		lpMMI->ptMaxPosition.y = 1;
		lpMMI->ptMinTrackSize.x = rcMin.Width();
		lpMMI->ptMinTrackSize.y = rcMin.Height();
		lpMMI->ptMaxTrackSize.x = rcMax.Width();
		lpMMI->ptMaxTrackSize.y = rcMax.Height();
		lpMMI->ptMaxSize.x = rcMax.Width();
		lpMMI->ptMaxSize.y = rcMax.Height();
	}
}

/**********************************************************************/

void CMainFrameEcrman::RefreshLayout()
{
	if ( ButtonOptions.GetLayoutEnable() == TRUE )
	{
		switch( BackgroundLayoutInfo.LoadUpperImage() )
		{
		case BACKIMAGE_ERROR_LOAD:
			{
				CString strError = "";
				strError.Format( "Unable to load upper image file (UPPER.%s)", ButtonOptions.GetUpperImageType() );
				BackgroundLayoutInfo.AddError( strError );
			}
			break;
			
		case BACKIMAGE_ERROR_NODC:
			BackgroundLayoutInfo.AddError( "Unable to create upper image device context" );
			break;
		}

		switch( BackgroundLayoutInfo.LoadLowerImage() )
		{
		case BACKIMAGE_ERROR_LOAD:
			{
				CString strError = "";
				strError.Format( "Unable to load lower image file (LOWER.%s)", ButtonOptions.GetLowerImageType() );
				BackgroundLayoutInfo.AddError( strError );
			}
			break;
			
		case BACKIMAGE_ERROR_NODC:
			BackgroundLayoutInfo.AddError( "Unable to create lower image device context" );
			break;
		}

		if ( BackgroundLayoutInfo.ValidateLayout() == TRUE )
		{
			ButtonManager.CalculateButtonRects( TRUE );
			CRect rcMax(0, 0, BackgroundLayoutInfo.GetWindowSizeRequestX(), BackgroundLayoutInfo.GetWindowSizeRequestY() );
			ShowWindow( SW_RESTORE );
			SetWindowPos( NULL, 0, 0, rcMax.Width(), rcMax.Height(), SWP_NOZORDER | SWP_NOACTIVATE );
		}
	}
	GetActiveView() -> Invalidate();
}

/**********************************************************************/

void CMainFrameEcrman::SetTitleBarText()
{
	CString strText;
	if ( SysInfo.IsEcrLiteSystem() == TRUE )
		strText = "SPOS Ecr Manager Lite";
	else
		strText = "SPOS Ecr Manager";

	if ( TRUE == SysInfo.GetScriptModeFlag() )
		strText += " (Script Mode : No Background Processing)";

	SetWindowText( strText );
}

/**********************************************************************/

void CMainFrameEcrman::OnSize(UINT nType, int cx, int cy) 
{
	CFrameWnd::OnSize(nType, cx, cy);
	StatusProgress.Reposition();
}

/**********************************************************************/

void CMainFrameEcrman::OnTimer(UINT nIDEvent) 
{	
	CFrameWnd::OnTimer(nIDEvent);
}

/**********************************************************************/

long CMainFrameEcrman::OnAutoBackup( WPARAM, LPARAM )
{
	AutoBackupProcessor.CheckAutoBackup( this, FALSE );

	if ( AutoBackupProcessor.GetWantQuitFlag() == TRUE )
		OnClose();

	else if ( AutoBackupProcessor.GetLoggedResultFlag() == TRUE )
		ShowBackupResult( this );

	return 0;
}

/**********************************************************************/

long CMainFrameEcrman::OnStartupMsg(WPARAM, LPARAM)
{
	if (m_pStartupMsgDlg == NULL)
	{
		Prompter.ShowStartupMessage(&m_pStartupMsgDlg,this);
	}

	return 0;
}

/**********************************************************************/

BOOL CMainFrameEcrman::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	if ( ( DatabaseValidator.GetDoingUpdateFlag() == TRUE ) || ( DatabaseValidator.GetBusyFlag() == TRUE ) )
		return 0;

	if ( DatabaseValidator.ValidAtStartup() == TRUE )
	{
		CFilenameUpdater FnUp( SysFiles::DataValidationQuit );
		if ( ::FileExists( FnUp.GetFilenameToUse() ) == TRUE )
		{
			DatabaseValidator.SetErrorFlag( TRUE );
			PostMessage( WM_CLOSE );
			return 0;
		}
	}

	if ( ButtonManager.GetButtonEditDlg() != NULL )
	{
		delete( ButtonManager.GetButtonEditDlg() );
		ButtonManager.SetButtonEditDlg( NULL );
	}

	CommsHandshaker.ProcessSales();
	
	return CFrameWnd::OnCommand(wParam, lParam);
}

/**********************************************************************/

void CMainFrameEcrman::BuildJobMenu()
{
	CEposTaskDatabase TaskDatabase;
	TaskDatabase.Read();

	m_MenuChanger.SetItemText ( IDM_JOB_PRESET1, TaskDatabase.GetLibraryTaskName ( 0x81 ) );
	m_MenuChanger.SetItemText ( IDM_JOB_PRESET2, TaskDatabase.GetLibraryTaskName ( 0x82 ) );
	m_MenuChanger.SetItemText ( IDM_JOB_PRESET3, TaskDatabase.GetLibraryTaskName ( 0x83 ) );
	m_MenuChanger.SetItemText ( IDM_JOB_PRESET4, TaskDatabase.GetLibraryTaskName ( 0x84 ) );
	m_MenuChanger.SetItemText ( IDM_JOB_PRESET5, TaskDatabase.GetLibraryTaskName ( 0x85 ) );

	m_MenuChanger.SetItemText ( IDM_JOB_PRESET6, TaskDatabase.GetLibraryTaskName ( 0x86 ) );
	m_MenuChanger.SetItemText ( IDM_JOB_PRESET7, TaskDatabase.GetLibraryTaskName ( 0x87 ) );
	m_MenuChanger.SetItemText ( IDM_JOB_PRESET8, TaskDatabase.GetLibraryTaskName ( 0x88 ) );
	m_MenuChanger.SetItemText ( IDM_JOB_PRESET9, TaskDatabase.GetLibraryTaskName ( 0x89 ) );
	m_MenuChanger.SetItemText ( IDM_JOB_PRESET10, TaskDatabase.GetLibraryTaskName ( 0x8A ) );

	m_MenuChanger.SetItemText ( IDM_JOB_PRESET11, TaskDatabase.GetLibraryTaskName ( 0x8B ) );
	m_MenuChanger.SetItemText ( IDM_JOB_PRESET12, TaskDatabase.GetLibraryTaskName ( 0x8C ) );
	m_MenuChanger.SetItemText ( IDM_JOB_PRESET13, TaskDatabase.GetLibraryTaskName ( 0x8D ) );
	m_MenuChanger.SetItemText ( IDM_JOB_PRESET14, TaskDatabase.GetLibraryTaskName ( 0x8E ) );
	m_MenuChanger.SetItemText ( IDM_JOB_PRESET15, TaskDatabase.GetLibraryTaskName ( 0x8F ) );

	m_MenuChanger.SetItemText ( IDM_JOB_PRESET16, TaskDatabase.GetLibraryTaskName ( 0x90 ) );
	m_MenuChanger.SetItemText ( IDM_JOB_PRESET17, TaskDatabase.GetLibraryTaskName ( 0x91 ) );
	m_MenuChanger.SetItemText ( IDM_JOB_PRESET18, TaskDatabase.GetLibraryTaskName ( 0x92 ) );
	m_MenuChanger.SetItemText ( IDM_JOB_PRESET19, TaskDatabase.GetLibraryTaskName ( 0x93 ) );
	m_MenuChanger.SetItemText ( IDM_JOB_PRESET20, TaskDatabase.GetLibraryTaskName ( 0x94 ) );

	
	m_MenuChanger.ShowItem ( IDM_JOB_PRESET1, TaskDatabase.GetLibraryTaskJobCount( 0x81 ) > 0 );
	m_MenuChanger.ShowItem ( IDM_JOB_PRESET2, TaskDatabase.GetLibraryTaskJobCount( 0x82 ) > 0 );
	m_MenuChanger.ShowItem ( IDM_JOB_PRESET3, TaskDatabase.GetLibraryTaskJobCount( 0x83 ) > 0 );
	m_MenuChanger.ShowItem ( IDM_JOB_PRESET4, TaskDatabase.GetLibraryTaskJobCount( 0x84 ) > 0 );
	m_MenuChanger.ShowItem ( IDM_JOB_PRESET5, TaskDatabase.GetLibraryTaskJobCount( 0x85 ) > 0 );
	
	m_MenuChanger.ShowItem ( IDM_JOB_PRESET6, TaskDatabase.GetLibraryTaskJobCount( 0x86 ) > 0 );
	m_MenuChanger.ShowItem ( IDM_JOB_PRESET7, TaskDatabase.GetLibraryTaskJobCount( 0x87 ) > 0 );
	m_MenuChanger.ShowItem ( IDM_JOB_PRESET8, TaskDatabase.GetLibraryTaskJobCount( 0x88 ) > 0 );
	m_MenuChanger.ShowItem ( IDM_JOB_PRESET9, TaskDatabase.GetLibraryTaskJobCount( 0x89 ) > 0 );
	m_MenuChanger.ShowItem ( IDM_JOB_PRESET10, TaskDatabase.GetLibraryTaskJobCount( 0x8A ) > 0 );
	
	m_MenuChanger.ShowItem ( IDM_JOB_PRESET11, TaskDatabase.GetLibraryTaskJobCount( 0x8B ) > 0 );
	m_MenuChanger.ShowItem ( IDM_JOB_PRESET12, TaskDatabase.GetLibraryTaskJobCount( 0x8C ) > 0 );
	m_MenuChanger.ShowItem ( IDM_JOB_PRESET13, TaskDatabase.GetLibraryTaskJobCount( 0x8D ) > 0 );
	m_MenuChanger.ShowItem ( IDM_JOB_PRESET14, TaskDatabase.GetLibraryTaskJobCount( 0x8E ) > 0 );
	m_MenuChanger.ShowItem ( IDM_JOB_PRESET15, TaskDatabase.GetLibraryTaskJobCount( 0x8F ) > 0 );
	
	m_MenuChanger.ShowItem ( IDM_JOB_PRESET16, TaskDatabase.GetLibraryTaskJobCount( 0x90 ) > 0 );
	m_MenuChanger.ShowItem ( IDM_JOB_PRESET17, TaskDatabase.GetLibraryTaskJobCount( 0x91 ) > 0 );
	m_MenuChanger.ShowItem ( IDM_JOB_PRESET18, TaskDatabase.GetLibraryTaskJobCount( 0x92 ) > 0 );
	m_MenuChanger.ShowItem ( IDM_JOB_PRESET19, TaskDatabase.GetLibraryTaskJobCount( 0x93 ) > 0 );
	m_MenuChanger.ShowItem ( IDM_JOB_PRESET20, TaskDatabase.GetLibraryTaskJobCount( 0x94 ) > 0 );
	
	m_MenuChanger.BuildMenu ( GetMenu() );
	DrawMenuBar();
}

/**********************************************************************/

BOOL CMainFrameEcrman::DestroyWindow()
{
	if ( m_hTimerIdle > 0 )
		KillTimer( m_hTimerIdle );
	
    Sysset.SaveWindowSize ( this );
    return CFrameWnd::DestroyWindow();
}

/**********************************************************************/
