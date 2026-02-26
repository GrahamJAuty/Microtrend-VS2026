/**********************************************************************/
#ifdef COMPILE_PMS
/**********************************************************************/
#include <afxpriv.h>
/**********************************************************************/
#include "AutoBackupProcessor.h"
#include "BackupErrors.h"
#include "CommsHandshaker.h"
#include "IdleTimeManager.h"
#include "LockManager.h"
#include "Password.h"
#include "PMSAccountFindDlg.h"
#include "PMSBookEditDlg.h"
#include "PMSBookRoomDlg.h"
#include "PMSBookSearchDlg.h"
#include "PMSCalendarTable.h"
#include "PMSCloseRoomDlg.h"
#include "PMSDownloadMonitorDlg.h"
#include "PMSInvoiceArray.h"
#include "PMSOptions.h"
#include "PMSPrintPaymentsDlg.h"
#include "PMSPrintRoomsDlg.h"
#include "PMSPrintUnpaidDlg.h"
#include "PMSReportUnpaid.h"
#include "PMSStatusHelpers.h"
#include "PosTrayTokenFiles.h"
#include "PriceHelpers.h"
#include "PropertySheetPMSRoomMonitor.h"
#include "SpecialKeys.h"
#include "StatusProgress.h"
#include "SysInfo.h"
#include "SystemBackup.h"
#include "TimerNumbers.h"
#include "WaitLockHelpers.h"
/**********************************************************************/
#include "PMSCalendarDlg.h"
/**********************************************************************/
IMPLEMENT_DYNAMIC(CPMSCalendarDlg, CDialog)
/**********************************************************************/
#define PMS_REBUILD_HISTORY 99998
#define PMS_REBUILD_TAX 99999
/**********************************************************************/

#ifndef STOCKMAN_SYSTEM
CPMSCalendarDlg::CPMSCalendarDlg( CMainFrameEcrman* pParent) : CSSDialog(CPMSCalendarDlg::IDD, pParent),
		m_PMSStaticDate( m_CalendarHelpers ),
		m_PMSStaticCalendar( m_CalendarHelpers )
#else
CPMSCalendarDlg::CPMSCalendarDlg( CMainFrameStockman* pParent) : CSSDialog(CPMSCalendarDlg::IDD, pParent),
		m_PMSStaticDate( m_CalendarHelpers ),
		m_PMSStaticCalendar( m_CalendarHelpers )
#endif
{
	m_pMainFrm = pParent;

	m_hTimer = 0;
	m_CheckBalanceTime = COleDateTime( 1980, 1, 1, 0, 0, 0 );

	m_hIcon = AfxGetApp()->LoadIcon ( IDR_MAINFRAME );

	m_CalendarHelpers.SetCalendarStartRoom(0);

	m_nMenuRoomIdx = 0;
	m_nMenuBookingId = 0;
	m_nMenuClosureId = 0;
	m_nMenuDay = 0;

	m_bForceQuit = FALSE;
	m_bInTimer = FALSE;
	m_bBlockTimer = FALSE;

	ResetToolTipText( TRUE );
}

/**********************************************************************/

CPMSCalendarDlg::~CPMSCalendarDlg()
{
}

/**********************************************************************/

void CPMSCalendarDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_VSCROLL, m_VScrollBar);
	DDX_Control(pDX, IDC_HSCROLL, m_HScrollBar);
	DDX_Control(pDX, IDC_STATIC_CALENDAR, m_PMSStaticCalendar);
	DDX_Control(pDX, IDC_STATIC_DATE, m_PMSStaticDate);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPMSCalendarDlg, CDialog)
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_COMMAND( IDM_PMSMENUBAR_BACKUP_SYSTEM, OnMenuBackupSystem )
	ON_COMMAND( IDM_PMSMENUBAR_BACKUP_STATUS, OnMenuBackupStatus )
	ON_COMMAND( IDM_PMSMENUBAR_UPDATE_EPOS, OnMenuUpdateEpos )
	ON_COMMAND( IDM_PMSMENUBAR_BOOK_ROOM, OnMenuBookRoom )
	ON_COMMAND( IDM_PMSMENUBAR_GUEST_ACCOUNTS, OnMenuGuestAccounts )
	ON_COMMAND( IDM_PMSMENUBAR_FIND_BOOKING, OnMenuFindBooking )
	ON_COMMAND( IDM_PMSMENUBAR_CANCELLED_BOOKING, OnMenuCancelledBooking )
	ON_COMMAND( IDM_PMSMENUBAR_CHECK_IN, OnMenuCheckIn )
	ON_COMMAND( IDM_PMSMENUBAR_CHECK_OUT_ACTIVE, OnMenuCheckOutActive )
	ON_COMMAND( IDM_PMSMENUBAR_CHECK_OUT_UNPAID_DEBIT, OnMenuCheckOutUnpaidDebit )
	ON_COMMAND( IDM_PMSMENUBAR_CHECK_OUT_UNPAID_CREDIT, OnMenuCheckOutUnpaidCredit )
	ON_COMMAND( IDM_PMSMENUBAR_BATCH_INVOICE, OnMenuBatchInvoices )
	ON_COMMAND( IDM_PMSMENUBAR_HIGHLIGHT_ACCOUNTS, OnMenuHighlightAccounts )
	ON_COMMAND( IDM_PMSMENUBAR_REPORTS_ROOMLIST, OnMenuReportsRoomList )
	ON_COMMAND( IDM_PMSMENUBAR_REPORTS_UNPAID, OnMenuReportsUnpaid )
	ON_COMMAND( IDM_PMSMENUBAR_REPORTS_PAYMENTS, OnMenuReportsPayments )
	ON_COMMAND( IDM_PMSMENUBAR_ROOM_GRID, OnMenuRoomGrid )
	ON_COMMAND( IDM_FILE_EXIT, OnMenuExit )
	ON_MESSAGE( WM_APP + 1, OnClickedDate )
	ON_MESSAGE( WM_APP + 2, OnLClickedRoom )
	ON_MESSAGE( WM_APP + 3, OnRClickedRoom )
	ON_MESSAGE( WM_APP + 4, OnClickDeadArea )
	ON_MESSAGE( WM_APP + 5, OnMouseMoveCalendar )
	ON_COMMAND( IDM_PMSCONTEXT_BOOK_ROOM, OnContextMenuBookRoom )
	ON_COMMAND( IDM_PMSCONTEXT_CLOSE_ROOM, OnContextMenuCloseRoom )
	ON_COMMAND( IDM_PMSCONTEXT_CLOSE_EDIT, OnContextMenuCloseEdit )
	ON_COMMAND( IDM_PMSCONTEXT_CHECKIN,OnContextMenuCheckIn )
	ON_COMMAND( IDM_PMSCONTEXT_BOOKING_PENDING,OnContextMenuBookingPending )
	ON_COMMAND( IDM_PMSCONTEXT_BOOKING_ACTIVE,OnContextMenuBookingActive )
	ON_COMMAND( IDM_PMSCONTEXT_BOOKING_COMPLETE,OnContextMenuBookingComplete )
	ON_COMMAND( IDM_PMSCONTEXT_SWITCHROOM_PENDING,OnContextMenuSwitchRoomPending )
	ON_COMMAND( IDM_PMSCONTEXT_SWITCHROOM_ACTIVE,OnContextMenuSwitchRoomActive )
	ON_COMMAND( IDM_PMSCONTEXT_EXTEND_BOOKING_TODAY,OnContextMenuExtendBookingToday )
	ON_COMMAND( IDM_PMSCONTEXT_EXTEND_BOOKING_TOMORROW,OnContextMenuExtendBookingTomorrow )
	ON_COMMAND( IDM_PMSCONTEXT_EXTEND_BOOKING_COMPLETE,OnContextMenuExtendBookingComplete )
	ON_COMMAND( IDM_PMSCONTEXT_SALES_ACTIVE,OnContextMenuSalesActive )
	ON_COMMAND( IDM_PMSCONTEXT_SALES_COMPLETE,OnContextMenuSalesComplete )
	ON_COMMAND( IDM_PMSCONTEXT_PAYMENTS_ACTIVE,OnContextMenuPaymentsActive )
	ON_COMMAND( IDM_PMSCONTEXT_PAYMENTS_COMPLETE,OnContextMenuPaymentsComplete )
	ON_COMMAND( IDM_PMSCONTEXT_INVOICE_PENDING,OnContextMenuInvoicePending )
	ON_COMMAND( IDM_PMSCONTEXT_INVOICE_ACTIVE,OnContextMenuInvoiceActive )
	ON_COMMAND( IDM_PMSCONTEXT_INVOICE_COMPLETE,OnContextMenuInvoiceComplete )
	ON_COMMAND( IDM_PMSCONTEXT_CHECKOUT,OnContextMenuCheckOut )
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXT, 0, 0xFFFF, OnToolTipNotify)
	ON_MESSAGE( WM_KICKIDLE, OnKickIdle)
	ON_WM_DESTROY()

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

BOOL CPMSCalendarDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetIcon ( m_hIcon, TRUE );

	m_ToolTipCtrl.Create(this,TTS_ALWAYSTIP | TTF_TRACK | TTF_ABSOLUTE);
	CWnd* pWnd = GetDlgItem( IDC_STATIC_CALENDAR );

	m_ToolInfo.cbSize = sizeof (TOOLINFO);
	m_ToolInfo.uFlags = TTF_IDISHWND | TTF_TRACK | TTF_ABSOLUTE;
	m_ToolInfo.rect.left = m_ToolInfo.rect.top = m_ToolInfo.rect.bottom = m_ToolInfo.rect.right = 0;
	m_ToolInfo.hwnd = pWnd -> GetParent() -> GetSafeHwnd ();
	m_ToolInfo.uId = (UINT) pWnd->GetSafeHwnd ();
	m_ToolInfo.hinst = AfxGetInstanceHandle ();
	m_ToolInfo.lpszText = LPSTR_TEXTCALLBACK;

	m_ToolTipCtrl.SendMessage (TTM_ADDTOOL, 0, (LPARAM) &m_ToolInfo);
	m_ToolTipCtrl.SendMessage (TTM_TRACKACTIVATE, TRUE, (LPARAM)&m_ToolInfo);

	GetDlgItem(IDC_STATIC_CALENDAR)->EnableTrackingToolTips(TRUE);

	m_hAccelTable = LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_SHARED_SPECIALKEYS));
	
	m_bInitDialog = TRUE;
	CenterWindow();
	ResizeControls();

	m_PMSStaticDate.SetWindowText( "" );
	m_PMSStaticCalendar.SetWindowText( "" );

	m_HScrollBar.SetScrollRange( 0, MAX_PMS_DAY );

	ScrollToToday();

	m_PMSStaticDate.SetClickPoint( &m_ClickPointDate );
	m_PMSStaticCalendar.SetClickPoint( &m_ClickPointRoom );

	CMenuChanger MenuChanger;
	MenuChanger.EatMenuTopLevel( GetMenu() );

	if ( PasswordArray.GetEnable( PasswordTicks::PMSCheckIn ) == FALSE )
		MenuChanger.EnableItem( IDM_PMSMENUBAR_CHECK_IN, FALSE );

	if ( PasswordArray.GetEnable( PasswordTicks::PMSCheckOut ) == FALSE )
		MenuChanger.EnableItem( IDM_PMSMENUBAR_CHECK_OUT_ACTIVE, FALSE );

	if ( PasswordArray.GetEnable( PasswordTicks::PMSManualSales ) == FALSE )
	{
		if ( PasswordArray.GetEnable( PasswordTicks::PMSPayments ) == FALSE )
			if ( PasswordArray.GetEnable( PasswordTicks::PMSInvoices ) == FALSE )
			{
				MenuChanger.EnableItem( IDM_PMSMENUBAR_GUEST_ACCOUNTS, FALSE );
				MenuChanger.EnableItem( IDM_PMSMENUBAR_CHECK_OUT_UNPAID_DEBIT, FALSE );
				MenuChanger.EnableItem( IDM_PMSMENUBAR_CHECK_OUT_UNPAID_CREDIT, FALSE );
			}
	}

	if ( PasswordArray.GetEnable( PasswordTicks::FileBackup ) == FALSE )
		MenuChanger.EnableItem( IDM_PMSMENUBAR_BACKUP_SYSTEM, FALSE );

	if ( PasswordArray.GetEnable( PasswordTicks::FileBackupStatus ) == FALSE )
		MenuChanger.EnableItem( IDM_PMSMENUBAR_BACKUP_STATUS, FALSE );
		
	MenuChanger.BuildMenu( GetMenu() );
	DrawMenuBar();

	ShowWindow( SW_MAXIMIZE );

	UpdateReports();

	if ( m_hTimer == 0 )
		m_hTimer = SetTimer ( TIMER_CALENDAR, 200, NULL );

	return TRUE;  
}

/**********************************************************************/
void CPMSCalendarDlg::OnSpecialKeyA(){ ProcessSpecialKey("A"); }
void CPMSCalendarDlg::OnSpecialKeyB(){ ProcessSpecialKey("B"); }
void CPMSCalendarDlg::OnSpecialKeyC(){ ProcessSpecialKey("C"); }
void CPMSCalendarDlg::OnSpecialKeyD(){ ProcessSpecialKey("D"); }
void CPMSCalendarDlg::OnSpecialKeyE(){ ProcessSpecialKey("E"); }
void CPMSCalendarDlg::OnSpecialKeyF(){ ProcessSpecialKey("F"); }
void CPMSCalendarDlg::OnSpecialKeyG(){ ProcessSpecialKey("G"); }
void CPMSCalendarDlg::OnSpecialKeyH(){ ProcessSpecialKey("H"); }
void CPMSCalendarDlg::OnSpecialKeyI(){ ProcessSpecialKey("I"); }
void CPMSCalendarDlg::OnSpecialKeyJ(){ ProcessSpecialKey("J"); }
void CPMSCalendarDlg::OnSpecialKeyK(){ ProcessSpecialKey("K"); }
void CPMSCalendarDlg::OnSpecialKeyL(){ ProcessSpecialKey("L"); }
void CPMSCalendarDlg::OnSpecialKeyM(){ ProcessSpecialKey("M"); }
void CPMSCalendarDlg::OnSpecialKeyN(){ ProcessSpecialKey("N"); }
void CPMSCalendarDlg::OnSpecialKeyO(){ ProcessSpecialKey("O"); }
void CPMSCalendarDlg::OnSpecialKeyP(){ ProcessSpecialKey("P"); }
void CPMSCalendarDlg::OnSpecialKeyQ(){ ProcessSpecialKey("Q"); }
void CPMSCalendarDlg::OnSpecialKeyR(){ ProcessSpecialKey("R"); }
void CPMSCalendarDlg::OnSpecialKeyS(){ ProcessSpecialKey("S"); }
void CPMSCalendarDlg::OnSpecialKeyT(){ ProcessSpecialKey("T"); }
void CPMSCalendarDlg::OnSpecialKeyU(){ ProcessSpecialKey("U"); }
void CPMSCalendarDlg::OnSpecialKeyV(){ ProcessSpecialKey("V"); }
void CPMSCalendarDlg::OnSpecialKeyW(){ ProcessSpecialKey("W"); }
void CPMSCalendarDlg::OnSpecialKeyX(){ ProcessSpecialKey("X"); }
void CPMSCalendarDlg::OnSpecialKeyY(){ ProcessSpecialKey("Y"); }
void CPMSCalendarDlg::OnSpecialKeyZ(){ ProcessSpecialKey("Z"); }
void CPMSCalendarDlg::OnSpecialKey0(){ ProcessSpecialKey("0"); }
void CPMSCalendarDlg::OnSpecialKey1(){ ProcessSpecialKey("1"); }
void CPMSCalendarDlg::OnSpecialKey2(){ ProcessSpecialKey("2"); }
void CPMSCalendarDlg::OnSpecialKey3(){ ProcessSpecialKey("3"); }
void CPMSCalendarDlg::OnSpecialKey4(){ ProcessSpecialKey("4"); }
void CPMSCalendarDlg::OnSpecialKey5(){ ProcessSpecialKey("5"); }
void CPMSCalendarDlg::OnSpecialKey6(){ ProcessSpecialKey("6"); }
void CPMSCalendarDlg::OnSpecialKey7(){ ProcessSpecialKey("7"); }
void CPMSCalendarDlg::OnSpecialKey8(){ ProcessSpecialKey("8"); }
void CPMSCalendarDlg::OnSpecialKey9(){ ProcessSpecialKey("9"); }
/**********************************************************************/

void CPMSCalendarDlg::ProcessSpecialKey( const char* sz )
{
	CString strKey = sz;
	strKey.MakeUpper();

	{
		CString strTest = SpecialKeys.GetPMSRebuildSalesKey();
		strTest.MakeUpper();
		
		if ( strTest == strKey )
		{
			MenuBarOption( "Rebuild Epos Sales", PMS_REBUILD_HISTORY );
			return;
		}
	}

	{
		CString strTest = SpecialKeys.GetPMSRebuildTaxKey();
		strTest.MakeUpper();
		
		if ( strTest == strKey )
		{
			if (DealerFlags.GetUseDatabaseVATBandFlag() == FALSE)
			{
				MenuBarOption( EcrmanOptions.GetTaxString( "Rebuild %T Band History" ), PMS_REBUILD_TAX );
			}

			return;
		}
	}
}

/**********************************************************************/

void CPMSCalendarDlg::UpdateReports()
{
	CString strOldReportPath = "PMS2\\Reports";
	GetDataProgramPath( strOldReportPath );

	CFileFind fileFinder;

	BOOL bWorking = fileFinder.FindFile ( strOldReportPath + "\\*.*" );

	bool bGotOldFiles = FALSE;
	
	while ( bWorking == TRUE )
	{
		bWorking = fileFinder.FindNextFile();

		if ( fileFinder.IsDots() == TRUE )
			continue;

		if ( fileFinder.IsDirectory() == TRUE )
			continue;

		bGotOldFiles = TRUE;
		break;
	}

	if ( TRUE == bGotOldFiles )
	{
		CDataManagerInfo infoDb;
		DataManager.CloseDatabase();
		DataManager.OpenDatabaseReadOnly( 0, infoDb );

		MenuBarOptionRebuildHistory(FALSE);

		CFileRemover FileRemover;
		FileRemover.RemoveFolder( strOldReportPath );
	}
}

/**********************************************************************/

BOOL CPMSCalendarDlg::PreTranslateMessage(MSG* pMsg) 
{
   if (m_hAccelTable) 
   {
      if (::TranslateAccelerator(m_hWnd, m_hAccelTable, pMsg)) {
         return(TRUE);
      }
   }

   return CDialog::PreTranslateMessage(pMsg);

}

/**********************************************************************/

void CPMSCalendarDlg::OnSize(UINT nType, int cx, int cy) 
{
	if ( m_bInitDialog == TRUE )
	{
		CDialog::OnSize(nType, cx, cy);
		ResizeControls();
	}
}

/**********************************************************************/

void CPMSCalendarDlg::ResizeControls()
{
	CRect rect;
	GetClientRect( rect );

	int nOldPos = m_CalendarHelpers.GetCalendarStartRoom();
	int nOldRows = m_CalendarHelpers.GetDisplayRows();
	
	if ( ( rect.right >= 400 ) && ( rect.bottom >= 300 ) )
	{
		m_CalendarHelpers.SetMetrics( rect );

		m_HScrollBar.SetWindowPos(&m_HScrollBar, 
			m_CalendarHelpers.GetHScrollBarX(),
			m_CalendarHelpers.GetHScrollBarY(),
			m_CalendarHelpers.GetHScrollBarCX(),
			m_CalendarHelpers.GetHScrollBarCY(), 
			SWP_NOZORDER | SWP_SHOWWINDOW);

		m_VScrollBar.SetWindowPos(&m_VScrollBar, 
			m_CalendarHelpers.GetVScrollBarX(),
			m_CalendarHelpers.GetVScrollBarY(),
			m_CalendarHelpers.GetVScrollBarCX(),
			m_CalendarHelpers.GetVScrollBarCY(), 
			SWP_NOZORDER | SWP_SHOWWINDOW);

		m_PMSStaticDate.SetWindowPos(&m_PMSStaticDate, 
			m_CalendarHelpers.GetDateX(),
			m_CalendarHelpers.GetDateY(),
			m_CalendarHelpers.GetDateCX(),
			m_CalendarHelpers.GetDateCY(), 
			SWP_NOZORDER | SWP_SHOWWINDOW);

		m_PMSStaticCalendar.SetWindowPos(&m_PMSStaticCalendar, 
			m_CalendarHelpers.GetCalendarX(),
			m_CalendarHelpers.GetCalendarY(),
			m_CalendarHelpers.GetCalendarCX(),
			m_CalendarHelpers.GetCalendarCY(), 
			SWP_NOZORDER | SWP_SHOWWINDOW);

		if ( m_CalendarHelpers.GetVScrollBarMax() == 0 )
		{
			m_VScrollBar.EnableWindow( FALSE );
			m_VScrollBar.SetScrollRange(0,0);
			m_CalendarHelpers.SetCalendarStartRoom( 0 );
		}
		else
		{
			m_VScrollBar.EnableWindow( TRUE );
			m_VScrollBar.SetScrollRange( m_CalendarHelpers.GetVScrollBarMin(), m_CalendarHelpers.GetVScrollBarMax(), TRUE );
		
			int nNewPos = nOldPos;
			int nNewRows = m_CalendarHelpers.GetDisplayRows();
			int nTotalRooms = DataManagerNonDb.PMSRoom.GetSize();

			while ( ( nNewPos + nNewRows > nTotalRooms + 1 ) && ( nNewPos >= 0 ) )
				nNewPos--;
			
			m_VScrollBar.SetScrollPos( nNewPos );
			m_CalendarHelpers.SetCalendarStartRoom( nNewPos );
		}
		
		m_PMSStaticDate.RedrawWindow();
		m_PMSStaticCalendar.RedrawWindow();
	}
}

/**********************************************************************/

void CPMSCalendarDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	lpMMI->ptMinTrackSize.x = 500;
	lpMMI->ptMinTrackSize.y = 400;

	CSSDialog::OnGetMinMaxInfo(lpMMI);
}

/**********************************************************************/

void CPMSCalendarDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if ( pScrollBar == &m_VScrollBar )
	{
		int nMin, nMax;
		pScrollBar -> GetScrollRange( &nMin, &nMax );

		int nCurrPos = pScrollBar -> GetScrollPos();

		int nThumbPos = (int) nPos;

		switch( nSBCode )
		{
		case SB_BOTTOM:
			pScrollBar -> SetScrollPos( nMax );
			break;

		case SB_TOP:
			pScrollBar -> SetScrollPos( 0 );
			break;

		case SB_LINEDOWN:
			if ( nCurrPos < nMax )
				pScrollBar -> SetScrollPos( nCurrPos + 1 );
			break;

		case SB_LINEUP:
			if ( nCurrPos > nMin )
				pScrollBar -> SetScrollPos( nCurrPos - 1 );
			break;

		case SB_THUMBPOSITION:
		case SB_THUMBTRACK:
			if ( ( nThumbPos >= nMin ) && ( nThumbPos <= nMax ) )
				pScrollBar -> SetScrollPos( nThumbPos );
			break;

		case SB_PAGEUP:
			{
				if ( nCurrPos > nMin )
					pScrollBar -> SetScrollPos ( max( nMin, nCurrPos - 1 ) );
			}
			break;

		case SB_PAGEDOWN:
			{
				if ( nCurrPos < nMax )
					pScrollBar -> SetScrollPos ( min( nMax, nCurrPos + 1 ) );
			}
			break;
		}

		m_CalendarHelpers.SetCalendarStartRoom( pScrollBar -> GetScrollPos() );
		m_PMSStaticCalendar.RedrawWindow();
	}
}

/**********************************************************************/

void CPMSCalendarDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if ( pScrollBar == &m_HScrollBar )
	{
		int nMin, nMax;
		pScrollBar -> GetScrollRange( &nMin, &nMax );

		int nCurrPos = pScrollBar -> GetScrollPos();

		int nThumbPos = (int) nPos;

		switch( nSBCode )
		{
		case SB_RIGHT:
			pScrollBar -> SetScrollPos( nMax );
			break;

		case SB_LEFT:
			pScrollBar -> SetScrollPos( 0 );
			break;

		case SB_LINERIGHT:
			if ( nCurrPos < nMax )
				pScrollBar -> SetScrollPos( nCurrPos + 1 );
			break;

		case SB_LINELEFT:
			if ( nCurrPos > nMin )
				pScrollBar -> SetScrollPos( nCurrPos - 1 );
			break;

		case SB_THUMBPOSITION:
		case SB_THUMBTRACK:
			if ( ( nThumbPos >= nMin ) && ( nThumbPos <= nMax ) )
				pScrollBar -> SetScrollPos( nThumbPos );
			break;

		case SB_PAGELEFT:
			{
				if ( nCurrPos > nMin )
					pScrollBar -> SetScrollPos ( max( nMin, nCurrPos - 7 ) );
			}
			break;

		case SB_PAGERIGHT:
			{
				if ( nCurrPos < nMax )
					pScrollBar -> SetScrollPos ( min( nMax, nCurrPos + 7 ) );
			}
			break;
		}

		m_CalendarHelpers.SetCalendarStartCol( pScrollBar -> GetScrollPos() );
		m_PMSStaticDate.RedrawWindow();
		m_PMSStaticCalendar.RedrawWindow();
	}
}

/**********************************************************************/

long CPMSCalendarDlg::OnMouseMoveCalendar(WPARAM wParam, LPARAM lParam )
{
	CPoint p;
	p.y = ( ( lParam & 0xFFFF0000 ) >> 16 ) + 20;
	p.x = ( lParam & 0xFFFF ) + 20;

	m_PMSStaticCalendar.ClientToScreen( &p );

	lParam = ( p.y << 16 ) + p.x;

	UpdateToolTipText();

	if ( m_strToolTipText != " " )
	{
		m_ToolTipCtrl.SendMessage( TTM_TRACKPOSITION,0,lParam);
		m_ToolTipCtrl.SendMessage (TTM_TRACKACTIVATE, TRUE, (LPARAM)&m_ToolInfo);
	}
	else 		
		m_ToolTipCtrl.SendMessage (TTM_TRACKACTIVATE, FALSE, (LPARAM)&m_ToolInfo);
	
	return 0l;
}

/**********************************************************************/

long CPMSCalendarDlg::OnClickedDate( WPARAM wDummy, LPARAM lParam ) 
{
	if ( m_PMSStaticDate.GetClickDateFlag() == TRUE )
		ScrollToToday();

	return 0l;
}

/**********************************************************************/

long CPMSCalendarDlg::OnLClickedRoom( WPARAM wDummy, LPARAM lParam ) 
{
	m_ToolTipCtrl.SendMessage (TTM_TRACKACTIVATE, FALSE, (LPARAM)&m_ToolInfo);
	ProcessLeftClickRoom();
	return 0l;
}

/**********************************************************************/

void CPMSCalendarDlg::ProcessLeftClickRoom()
{
	int nRoomIdx = m_CalendarHelpers.GetCalendarStartRoom() + m_PMSStaticCalendar.GetCalendarClickRow();
	m_nMenuDay = m_CalendarHelpers.GetCalendarClickDay( m_PMSStaticCalendar.GetCalendarClickCol() );
	
	CPMSRoomCSVRecord Room;
	DataManagerNonDb.PMSRoom.GetAt( nRoomIdx, Room );

	CPMSBookingQueryInfo Query;
	Query.m_nRoomNo = Room.GetRoomNo();
	Query.m_nStartDay = m_nMenuDay;
	Query.m_nMinNights = 1;
	Query.m_nMaxNights = 1;
	
	CPMSBookingResponseInfo Response;
	PMSCalendarTable.CheckRoomAvailability( Query, Response );

	if ( 0 == Response.m_nBookingClashId )
	{
		m_nMenuRoomIdx = nRoomIdx;
		m_nMenuBookingId = 0;
		m_nMenuClosureId = 0;
		OnContextMenuBookRoom();
	}
	else 
	{
		CPMSCalendarBooking CalendarBooking;
		PMSCalendarTable.GetAt( Response.m_nBookingClashArrayIdx, CalendarBooking );

		if ( PMS_BOOKING_STATUS_CLOSED == CalendarBooking.m_nStatus )
		{
			m_nMenuRoomIdx = nRoomIdx;
			m_nMenuBookingId = 0;
			m_nMenuClosureId = Response.m_nBookingClashId;
			OnContextMenuCloseEdit();
		}
		else
		{
			int nBookingIdx;
			if ( DataManagerNonDb.PMSBooking.FindBookingById( Response.m_nBookingClashId, nBookingIdx ) == FALSE )
				return;

			CPMSBookingCSVRecord BookingRecord;
			DataManagerNonDb.PMSBooking.GetAt( nBookingIdx, BookingRecord );

			if ( BookingRecord.GetPartCount() == 0 )
				return;

			CPMSBookingPartInfo infoPart;
			BookingRecord.GetFinalPartInfo( infoPart );
			if ( DataManagerNonDb.PMSRoom.FindRoomByNumber( infoPart.GetRoomNo(), m_nMenuRoomIdx ) == FALSE )
				return;

			m_nMenuBookingId = BookingRecord.GetBookingId();
			m_nMenuClosureId = 0;

			switch( BookingRecord.GetBookingStatus() )
			{
			case PMS_BOOKING_STATUS_PENDING:	OnContextMenuBookingPending();	break;
			case PMS_BOOKING_STATUS_ACTIVE:		OnContextMenuBookingActive();	break;
			case PMS_BOOKING_STATUS_COMPLETE:	OnContextMenuBookingComplete();	break;
			}
		}
	}
}

/**********************************************************************/

void CPMSCalendarDlg::BookRoomFromCalendar( int nRoomIdx, CPMSBookingResponseInfo& Response )
{
	CPMSBookingCSVRecord BookingRecord;
	CPMSBookRoomDlg dlgBook( nRoomIdx, Response, BookingRecord, this );
		
	if ( dlgBook.DoModal() != IDOK )
		return;

	CPMSRoomCSVRecord Room;
	DataManagerNonDb.PMSRoom.GetAt( nRoomIdx, Room );
		
	int nBookingId = DataManagerNonDb.PMSBooking.FindFirstFreeId();
	int nCustomerId = DataManagerNonDb.PMSCustomer.FindFirstFreeId();

	BookingRecord.SetBookingId( nBookingId );
	BookingRecord.SetCustomerId( nCustomerId );
	BookingRecord.SetGuests( dlgBook.m_nSearchGuests );

	if ( 1 == dlgBook.m_nAction )
	{
		if ( FinaliseCheckIn( nRoomIdx, BookingRecord, FALSE ) == FALSE )
			return;
	}
	else
	{	
		CPMSCustomerCSVRecord CustomerRecord;
		PMSRoomStatusTable.LoadRoomAccountSettingsBuffer( BookingRecord, FALSE );
		CPMSBookEditDlg dlgEdit( PMS_EDITBOOKING_PENDING_SINGLE_NEW, MAX_PMS_ROOMS, 0, BookingRecord, CustomerRecord, this );

		if ( dlgEdit.DoModal() != IDOK )
			return;

		DataManagerNonDb.PMSCustomer.InsertAndSaveCustomer( CustomerRecord );

		CPMSBookingPartInfo infoPart;
		BookingRecord.GetFinalPartInfo( infoPart );
		if ( infoPart.GetStartDay() + infoPart.GetNights() <= PMSOptions.GetPMSDayNumber() )
			BookingRecord.SetBookingStatus( PMS_BOOKING_STATUS_COMPLETE );
		else
			BookingRecord.SetBookingStatus( PMS_BOOKING_STATUS_PENDING );

		CPMSBookingCSVArray::InsertAndSaveBooking( BookingRecord );

		PMSCalendarTable.ApplyBooking( BookingRecord );

		m_CalendarHelpers.SetSelectedBookingId( BookingRecord.GetBookingId() );
		m_CalendarHelpers.SetSelectedClosureId( 0 );
	}
}

/**********************************************************************/
#define PMS_CONTEXTMENU_TYPE_NONE 0
#define PMS_CONTEXTMENU_TYPE_COMPLETE 1
#define PMS_CONTEXTMENU_TYPE_PENDING 2
#define PMS_CONTEXTMENU_TYPE_PENDING_NOCHECKIN 3
#define PMS_CONTEXTMENU_TYPE_ACTIVE 4
#define PMS_CONTEXTMENU_TYPE_VACANT 5
#define PMS_CONTEXTMENU_TYPE_CLOSED 6
/**********************************************************************/

long CPMSCalendarDlg::OnRClickedRoom( WPARAM wDummy, LPARAM lParam ) 
{
	m_ToolTipCtrl.SendMessage (TTM_TRACKACTIVATE, FALSE, (LPARAM)&m_ToolInfo);

	m_nMenuRoomIdx = m_CalendarHelpers.GetCalendarStartRoom() + m_PMSStaticCalendar.GetCalendarClickRow();
	m_nMenuDay = m_CalendarHelpers.GetCalendarClickDay( m_PMSStaticCalendar.GetCalendarClickCol() );
		
	m_nMenuBookingId = 0;
	m_nMenuClosureId = 0;

	PMSOptions.SetPMSTime();
	PMSRoomStatusTable.InvalidateRoomStatus();

	CPMSRoomCSVRecord Room;
	DataManagerNonDb.PMSRoom.GetAt( m_nMenuRoomIdx, Room );

	CPMSRoomStatusInfo infoRoom;
	PMSRoomStatusTable.GetRoomInfo( m_nMenuRoomIdx, infoRoom );

	CPMSBookingQueryInfo Query;
	Query.m_nRoomNo = Room.GetRoomNo();
	Query.m_nStartDay = m_nMenuDay;
	Query.m_nMinNights = 1;
	Query.m_nMaxNights = 1;

	CPMSBookingResponseInfo Response;

	PMSCalendarTable.CheckRoomAvailability( Query, Response );

	int nId = Response.m_nBookingClashId;
	int nPart = Response.m_nBookingClashPart;
	int nMenuType = PMS_CONTEXTMENU_TYPE_NONE;

	CPMSBookingPartInfo infoPart;

	int nPMSDay = PMSOptions.GetPMSDayNumber();
	int nPartCheckOutDay = 0;

	if ( nId == 0 )
		nMenuType = PMS_CONTEXTMENU_TYPE_VACANT;
	else
	{
		CPMSCalendarBooking CalendarBooking;
		PMSCalendarTable.GetAt( Response.m_nBookingClashArrayIdx, CalendarBooking );

		if ( PMS_BOOKING_STATUS_CLOSED == CalendarBooking.m_nStatus )
		{
			int nClosureIdx;
			if ( DataManagerNonDb.PMSClosure.FindClosureById( nId, nClosureIdx ) == TRUE )
				nMenuType = PMS_CONTEXTMENU_TYPE_CLOSED;
		}
		else
		{
			int nBookingIdx;
			if ( DataManagerNonDb.PMSBooking.FindBookingById( nId, nBookingIdx ) == TRUE )
			{
				CPMSBookingCSVRecord BookingRecord;
				DataManagerNonDb.PMSBooking.GetAt( nBookingIdx, BookingRecord );
				BookingRecord.GetFinalPartInfo( infoPart );
				nPartCheckOutDay = infoPart.GetStartDay() + infoPart.GetNights();
						
				switch( BookingRecord.GetBookingStatus() )
				{
				case PMS_BOOKING_STATUS_ACTIVE:
					{
						if ( DataManagerNonDb.PMSRoom.FindRoomByNumber( infoPart.GetRoomNo(), m_nMenuRoomIdx ) == TRUE )
							nMenuType = PMS_CONTEXTMENU_TYPE_ACTIVE;
					}
					break;

				case PMS_BOOKING_STATUS_PENDING:
					if ( nPMSDay >= nPartCheckOutDay )
						nMenuType = PMS_CONTEXTMENU_TYPE_PENDING_NOCHECKIN;
					else if ( PMSRoomStatusTable.GetExpectedBookingId( m_nMenuRoomIdx ) != nId )
						nMenuType = PMS_CONTEXTMENU_TYPE_PENDING_NOCHECKIN;
					else
						nMenuType = PMS_CONTEXTMENU_TYPE_PENDING;
					break;

				case PMS_BOOKING_STATUS_COMPLETE:
					nMenuType = PMS_CONTEXTMENU_TYPE_COMPLETE;
					break;
				}
			}
		}
	}

	if ( nMenuType != PMS_CONTEXTMENU_TYPE_NONE )
	{
		if ( nMenuType != PMS_CONTEXTMENU_TYPE_CLOSED )
		{
			m_nMenuBookingId = nId;
			m_CalendarHelpers.SetSelectedBookingId( nId );
			m_CalendarHelpers.SetSelectedClosureId( 0 );
		}
		else
		{
			m_nMenuClosureId = nId;
			m_CalendarHelpers.SetSelectedBookingId( 0 );
			m_CalendarHelpers.SetSelectedClosureId( nId );
		}

		m_PMSStaticCalendar.RedrawWindow();

		CPoint pt;
		GetCursorPos(&pt);

		CMenu menu;
		VERIFY(menu.LoadMenu(IDR_POPUP_PMS));

		CMenu* pPopup = menu.GetSubMenu(0);
		ASSERT(pPopup != NULL);
		CWnd* pWndPopupOwner = this;

		CMenuChanger MenuChanger;
		MenuChanger.EatMenuPopup( pPopup );
		
		MenuChanger.ShowItem( IDM_PMSCONTEXT_BOOK_ROOM,					( PMS_CONTEXTMENU_TYPE_VACANT == nMenuType ) );
		MenuChanger.ShowItem( IDM_PMSCONTEXT_CLOSE_ROOM,				( PMS_CONTEXTMENU_TYPE_VACANT == nMenuType ) );
		MenuChanger.ShowItem( IDM_PMSCONTEXT_CLOSE_EDIT,				( PMS_CONTEXTMENU_TYPE_CLOSED == nMenuType ) );
		MenuChanger.ShowItem( IDM_PMSCONTEXT_CHECKIN,					( PMS_CONTEXTMENU_TYPE_PENDING == nMenuType ) );
		MenuChanger.ShowItem( IDM_PMSCONTEXT_BOOKING_PENDING,			( PMS_CONTEXTMENU_TYPE_PENDING == nMenuType ) || ( PMS_CONTEXTMENU_TYPE_PENDING_NOCHECKIN == nMenuType ) );
		MenuChanger.ShowItem( IDM_PMSCONTEXT_BOOKING_ACTIVE,			( PMS_CONTEXTMENU_TYPE_ACTIVE == nMenuType ) );
		MenuChanger.ShowItem( IDM_PMSCONTEXT_BOOKING_COMPLETE,			( PMS_CONTEXTMENU_TYPE_COMPLETE == nMenuType ) );
		MenuChanger.ShowItem( IDM_PMSCONTEXT_SWITCHROOM_PENDING,		( PMS_CONTEXTMENU_TYPE_PENDING == nMenuType ) || ( PMS_CONTEXTMENU_TYPE_PENDING_NOCHECKIN == nMenuType ) );
		MenuChanger.ShowItem( IDM_PMSCONTEXT_SWITCHROOM_ACTIVE,			( PMS_CONTEXTMENU_TYPE_ACTIVE == nMenuType ) );
		MenuChanger.ShowItem( IDM_PMSCONTEXT_EXTEND_BOOKING_TODAY,		( PMS_CONTEXTMENU_TYPE_ACTIVE == nMenuType ) );
		MenuChanger.ShowItem( IDM_PMSCONTEXT_EXTEND_BOOKING_TOMORROW,	( PMS_CONTEXTMENU_TYPE_ACTIVE == nMenuType ) );
		MenuChanger.ShowItem( IDM_PMSCONTEXT_EXTEND_BOOKING_COMPLETE,	( PMS_CONTEXTMENU_TYPE_COMPLETE == nMenuType ) );
		MenuChanger.ShowItem( IDM_PMSCONTEXT_SALES_ACTIVE,				( PMS_CONTEXTMENU_TYPE_ACTIVE == nMenuType ) );
		MenuChanger.ShowItem( IDM_PMSCONTEXT_SALES_COMPLETE,			( PMS_CONTEXTMENU_TYPE_COMPLETE == nMenuType ) );
		MenuChanger.ShowItem( IDM_PMSCONTEXT_PAYMENTS_ACTIVE,			( PMS_CONTEXTMENU_TYPE_ACTIVE == nMenuType ) );
		MenuChanger.ShowItem( IDM_PMSCONTEXT_PAYMENTS_COMPLETE,			( PMS_CONTEXTMENU_TYPE_COMPLETE == nMenuType ) );
		MenuChanger.ShowItem( IDM_PMSCONTEXT_INVOICE_PENDING,			( PMS_CONTEXTMENU_TYPE_PENDING == nMenuType ) || ( PMS_CONTEXTMENU_TYPE_PENDING_NOCHECKIN == nMenuType ) );
		MenuChanger.ShowItem( IDM_PMSCONTEXT_INVOICE_ACTIVE,			( PMS_CONTEXTMENU_TYPE_ACTIVE == nMenuType ) );
		MenuChanger.ShowItem( IDM_PMSCONTEXT_INVOICE_COMPLETE,			( PMS_CONTEXTMENU_TYPE_COMPLETE == nMenuType ) );
		MenuChanger.ShowItem( IDM_PMSCONTEXT_CHECKOUT,					( PMS_CONTEXTMENU_TYPE_ACTIVE == nMenuType ) );

		if ( nPMSDay >= nPartCheckOutDay )
			MenuChanger.KillItem( IDM_PMSCONTEXT_SWITCHROOM_ACTIVE );
		
		if ( nPMSDay != nPartCheckOutDay )
			MenuChanger.KillItem( IDM_PMSCONTEXT_EXTEND_BOOKING_TODAY );

		if ( nPMSDay != nPartCheckOutDay - 1 )
			MenuChanger.KillItem( IDM_PMSCONTEXT_EXTEND_BOOKING_TOMORROW );

		if ( ( nPMSDay != nPartCheckOutDay ) && ( nPMSDay != nPartCheckOutDay + 1 ) )
			MenuChanger.KillItem( IDM_PMSCONTEXT_EXTEND_BOOKING_COMPLETE );

		if ( PasswordArray.GetEnable( PasswordTicks::PMSManualSales ) == FALSE )
		{
			MenuChanger.KillItem( IDM_PMSCONTEXT_SALES_ACTIVE );
			MenuChanger.KillItem( IDM_PMSCONTEXT_SALES_COMPLETE );
		}

		if ( PasswordArray.GetEnable( PasswordTicks::PMSPayments ) == FALSE )
		{
			MenuChanger.KillItem( IDM_PMSCONTEXT_PAYMENTS_ACTIVE );
			MenuChanger.KillItem( IDM_PMSCONTEXT_PAYMENTS_COMPLETE );
		}

		if ( PasswordArray.GetEnable( PasswordTicks::PMSInvoices ) == FALSE )
		{
			MenuChanger.KillItem( IDM_PMSCONTEXT_INVOICE_PENDING );
			MenuChanger.KillItem( IDM_PMSCONTEXT_INVOICE_ACTIVE );
			MenuChanger.KillItem( IDM_PMSCONTEXT_INVOICE_COMPLETE );
		}
		
		if ( PasswordArray.GetEnable( PasswordTicks::PMSCheckIn ) == FALSE )
			MenuChanger.KillItem( IDM_PMSCONTEXT_CHECKIN );

		if ( PasswordArray.GetEnable( PasswordTicks::PMSCheckOut ) == FALSE )
			MenuChanger.KillItem( IDM_PMSCONTEXT_CHECKOUT );

		MenuChanger.BuildMenu( pPopup );

		while (pWndPopupOwner->GetStyle() & WS_CHILD)
			pWndPopupOwner = pWndPopupOwner->GetParent();

		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y,
			pWndPopupOwner);

		m_CalendarHelpers.SetSelectedBookingId( 0 );
		m_CalendarHelpers.SetSelectedClosureId( 0 );
		m_PMSStaticCalendar.RedrawWindow();
	}

	return 0l;
}

/**********************************************************************/

long CPMSCalendarDlg::OnClickDeadArea( WPARAM wDummy, LPARAM lParam ) 
{
	m_CalendarHelpers.SetSelectedBookingId( 0 );
	m_CalendarHelpers.SetSelectedClosureId( 0 );
	m_PMSStaticCalendar.RedrawWindow();
	return 0l;
}

/**********************************************************************/

void CPMSCalendarDlg::ScrollToToday()
{
	ScrollToDate( DataManagerNonDb.PMSBooking.GetCurrentDayNumber() );
}

/**********************************************************************/

void CPMSCalendarDlg::ScrollToDate( int nDay )
{
	int nColLeft = m_CalendarHelpers.GetColumnFromDayNumber( nDay - 3, CPMSBookingCSVArray::GetCurrentDayNumber() );
	if ( nColLeft < 0 )
		nColLeft = 0;

	m_HScrollBar.SetScrollPos( nColLeft );

	m_CalendarHelpers.SetCalendarStartCol( nColLeft );
	m_PMSStaticDate.RedrawWindow();
	m_PMSStaticCalendar.RedrawWindow();
}

/**********************************************************************/

void CPMSCalendarDlg::ScrollToRoom( int nRoomIdx )
{
	int nRowTop = nRoomIdx - 3;
	
	int nMin, nMax;
	m_VScrollBar.GetScrollRange( &nMin, &nMax );

	if ( nRowTop > nMax )
		nRowTop = nMax;

	if ( nRowTop < 0 )
		nRowTop = 0;

	m_VScrollBar.SetScrollPos( nRowTop );
	m_CalendarHelpers.SetCalendarStartRoom( nRowTop );

	m_PMSStaticDate.RedrawWindow();
	m_PMSStaticCalendar.RedrawWindow();
}

/**********************************************************************/

void CPMSCalendarDlg::BlockTimer( bool b )
{
	m_bBlockTimer = b;
}

/**********************************************************************/

void CPMSCalendarDlg::OnTimer(UINT nIDEvent) 
{
	if ( TIMER_CALENDAR == nIDEvent )
	{
		if ( ( FALSE == m_bInTimer ) && ( FALSE == m_bBlockTimer ) )
		{
			m_bInTimer = TRUE;
	
			if ( CommsHandshaker.GetConfirmStoppedFlag() == TRUE )
				CommsHandshaker.PMSPulse();

			m_bInTimer = FALSE;
		}
	}

	CDialog::OnTimer(nIDEvent);
}

/**********************************************************************/

LRESULT CPMSCalendarDlg::OnKickIdle( WPARAM, LPARAM )
{
	if ( SysInfo.GetScriptModeFlag() == FALSE )
	{
		BlockTimer( TRUE );

		AutoBackupProcessor.CheckAutoBackup( this, TRUE );

		if ( TRUE == AutoBackupProcessor.GetWantQuitFlag() )
		{
			m_bForceQuit = TRUE;
			EndDialog( IDOK );
		}

		else if ( TRUE == AutoBackupProcessor.GetLoggedResultFlag() )
			m_pMainFrm -> ShowBackupResult( this, TRUE );

		BlockTimer( FALSE );
	}

	return FALSE;
}

/**********************************************************************/

void CPMSCalendarDlg::OnMenuExit()
{
	EndDialog( IDCANCEL );
}

/**********************************************************************/

void CPMSCalendarDlg::UpdateDisplay()
{
	m_PMSStaticCalendar.RedrawWindow();
	m_PMSStaticDate.RedrawWindow();
}

/**********************************************************************/

void CPMSCalendarDlg::OnMenuBackupSystem()
{
	MenuBarOption( "Backup System", IDM_PMSMENUBAR_BACKUP_SYSTEM );
}

/**********************************************************************/

void CPMSCalendarDlg::OnMenuBackupStatus()
{
	m_pMainFrm -> ShowBackupResult( this, TRUE );
}

/**********************************************************************/

void CPMSCalendarDlg::OnMenuUpdateEpos()
{
	MenuBarOption( "Update Epos", IDM_PMSMENUBAR_UPDATE_EPOS, FALSE, FALSE );
}

/**********************************************************************/

void CPMSCalendarDlg::OnMenuBookRoom()
{
	MenuBarOption( "Book Room", IDM_PMSMENUBAR_BOOK_ROOM );
}

/**********************************************************************/

void CPMSCalendarDlg::OnMenuCheckIn()
{
	MenuBarOption( "Check In", IDM_PMSMENUBAR_CHECK_IN );
}

/**********************************************************************/

void CPMSCalendarDlg::OnMenuCheckOutActive()
{
	MenuBarOption( "Check Out", IDM_PMSMENUBAR_CHECK_OUT_ACTIVE );
}

/**********************************************************************/

void CPMSCalendarDlg::OnMenuCheckOutUnpaidDebit()
{
	MenuBarOption( "Process Unpaid Accounts", IDM_PMSMENUBAR_CHECK_OUT_UNPAID_DEBIT );
}

/**********************************************************************/

void CPMSCalendarDlg::OnMenuCheckOutUnpaidCredit()
{
	MenuBarOption( "Process Unpaid Accounts (CR)", IDM_PMSMENUBAR_CHECK_OUT_UNPAID_CREDIT );
}

/**********************************************************************/

void CPMSCalendarDlg::OnMenuBatchInvoices()
{
	MenuBarOption( "Print Batch Invoices", IDM_PMSMENUBAR_BATCH_INVOICE );
}

/**********************************************************************/

void CPMSCalendarDlg::OnMenuHighlightAccounts()
{
	MenuBarOption( "Highlight Unpaid Accounts", IDM_PMSMENUBAR_HIGHLIGHT_ACCOUNTS );
}

/**********************************************************************/

void CPMSCalendarDlg::OnMenuFindBooking()
{
	MenuBarOption( "Find Bookings", IDM_PMSMENUBAR_FIND_BOOKING );
}

/**********************************************************************/

void CPMSCalendarDlg::OnMenuCancelledBooking()
{
	MenuBarOption( "Find Cancelled Bookings", IDM_PMSMENUBAR_CANCELLED_BOOKING );
}

/**********************************************************************/

void CPMSCalendarDlg::OnMenuGuestAccounts()
{
	MenuBarOption( "Process Guest Accounts", IDM_PMSMENUBAR_GUEST_ACCOUNTS );
}

/**********************************************************************/

void CPMSCalendarDlg::OnMenuRoomGrid()
{
	MenuBarOption( "Display Room Monitor", IDM_PMSMENUBAR_ROOM_GRID, TRUE );
}

/**********************************************************************/

void CPMSCalendarDlg::OnMenuReportsRoomList()
{
	MenuBarOption( "Print Room List", IDM_PMSMENUBAR_REPORTS_ROOMLIST );
}

/**********************************************************************/

void CPMSCalendarDlg::OnMenuReportsUnpaid()
{
	MenuBarOption( "Print Unpaid Accounts List", IDM_PMSMENUBAR_REPORTS_UNPAID );
}

/**********************************************************************/

void CPMSCalendarDlg::OnMenuReportsPayments()
{
	MenuBarOption( "Print Payment Summary Report", IDM_PMSMENUBAR_REPORTS_PAYMENTS );
}

/**********************************************************************/

void CPMSCalendarDlg::MenuBarOption ( const char* szText, int nOption, bool bForceUpdate, bool bStopTimer )
{
	m_ToolTipCtrl.SendMessage (TTM_TRACKACTIVATE, FALSE, (LPARAM)&m_ToolInfo);

	PMSOptions.SetPMSTime();
	PMSRoomStatusTable.InvalidateRoomStatus();

	bool bProcessOption = TRUE;

	CLockManagerInfo infoLock;
	infoLock.m_bLockAllUsers = FALSE;
	infoLock.m_nCommsAction = COMMS_ACTION_NONE;
	infoLock.m_bStopComms = TRUE;
	infoLock.m_bStopCommsPMSExemption = FALSE;
	infoLock.m_strStopCommsPrompt = szText;
	infoLock.m_bStopPosTray = FALSE;
	infoLock.m_pParentWnd = this;

	CLockManager LockManager( infoLock );

	if ( TRUE == bStopTimer )
	{
		BlockTimer( TRUE );	
		bProcessOption = LockManager.GetLock(); 
	}

	if ( TRUE == bProcessOption )
	{
		CDataManagerInfo infoDb;
		DataManager.CloseDatabase();
		DataManager.OpenDatabaseReadOnly( 0, infoDb );

		if ( TRUE == bStopTimer )
		{
			CPMSStatusHelpers PMSStatusHelpers;
			PMSStatusHelpers.ImportPMSSales();
		}

		for ( int nRoomIdx = 0; nRoomIdx < DataManagerNonDb.PMSRoom.GetSize(); nRoomIdx++ )
			PMSRoomStatusTable.UpdateAccountBalance( nRoomIdx, bForceUpdate );
	
		switch( nOption )
		{
		case PMS_REBUILD_HISTORY:						MenuBarOptionRebuildHistory(TRUE);	break;
		case PMS_REBUILD_TAX:							MenuBarOptionRebuildTax();			break;
		case IDM_PMSMENUBAR_BACKUP_SYSTEM:				MenuBarOptionBackupSystem();		break;
		case IDM_PMSMENUBAR_UPDATE_EPOS:				MenuBarOptionUpdateEpos();			break;
		case IDM_PMSMENUBAR_BOOK_ROOM:					MenuBarOptionBookRoom();			break;
		case IDM_PMSMENUBAR_CHECK_IN:					MenuBarOptionCheckIn();				break;
		case IDM_PMSMENUBAR_CHECK_OUT_ACTIVE:			MenuBarOptionCheckOutActive();		break;
		case IDM_PMSMENUBAR_CHECK_OUT_UNPAID_DEBIT:		MenuBarOptionCheckOutUnpaidDebit();	break;
		case IDM_PMSMENUBAR_CHECK_OUT_UNPAID_CREDIT:	MenuBarOptionCheckOutUnpaidCredit();break;
		case IDM_PMSMENUBAR_BATCH_INVOICE:				MenuBarOptionBatchInvoices();		break;
		case IDM_PMSMENUBAR_HIGHLIGHT_ACCOUNTS:			MenuBarOptionHighlightAccounts();	break;
		case IDM_PMSMENUBAR_FIND_BOOKING:				MenuBarOptionFindBooking();			break;
		case IDM_PMSMENUBAR_CANCELLED_BOOKING:			MenuBarOptionCancelledBooking();	break;
		case IDM_PMSMENUBAR_GUEST_ACCOUNTS:				MenuBarOptionGuestAccounts();		break;
		case IDM_PMSMENUBAR_ROOM_GRID:					MenuBarOptionRoomGrid();			break;
		case IDM_PMSMENUBAR_REPORTS_ROOMLIST:			MenuBarOptionRoomList();			break;
		case IDM_PMSMENUBAR_REPORTS_UNPAID:				MenuBarOptionUnpaid();				break;
		case IDM_PMSMENUBAR_REPORTS_PAYMENTS:			MenuBarOptionPayments();			break;
		}

		DataManager.CloseDatabase();
	}

	if ( PMSOptions.GetMiscRetainSelectionFlag() == FALSE )
	{
		m_CalendarHelpers.SetSelectedBookingId(0);
		m_CalendarHelpers.SetSelectedClosureId(0);
	}

	m_PMSStaticDate.RedrawWindow();
	m_PMSStaticCalendar.RedrawWindow();

	if ( TRUE == bStopTimer )
		BlockTimer( FALSE );
}

/**********************************************************************/

void CPMSCalendarDlg::MenuBarOptionRebuildHistory( bool bPrompt )
{
	bool bRebuild = TRUE;

	if ( TRUE == bPrompt )
		bRebuild = ( Prompter.YesNo( "Do you wish to rebuild the EPOS sales history\nfor checked out guest accounts.", FALSE ) == IDYES );
	
	if ( TRUE == bRebuild )
	{
		StatusProgress.Lock( TRUE, "Rebuild EPOS Sales History" );

		int nFileNo = 0;
		CFileRemover FileRemover;
		FileRemover.RemoveFolderContents( PMSOptions.GetPMSFolderReports(), nFileNo );

		int nSize = DataManagerNonDb.PMSBooking.GetSize();
		for ( int nBookingIdx = 0; nBookingIdx < nSize ; nBookingIdx++ )
		{
			StatusProgress.SetPos( nBookingIdx, nSize );

			CPMSBookingCSVRecord BookingRecord;
			DataManagerNonDb.PMSBooking.GetAt( nBookingIdx, BookingRecord );

			if ( BookingRecord.GetBookingStatus() == PMS_BOOKING_STATUS_COMPLETE )
			{
				PMSRoomStatusTable.LoadRoomAccountSettingsBuffer( BookingRecord, TRUE );

				bool bDebit = FALSE;
				bool bCredit = FALSE;

				CPMSRoomStatusInfo infoRoom;
				PMSRoomStatusTable.GetRoomInfo( MAX_PMS_ROOMS, infoRoom );

				for ( int nAccountIdx = 0; nAccountIdx < infoRoom.GetSafeAccountCount(); nAccountIdx++ )
				{
					//MUST UPDATE ACCOUNT BALANCE BEFORE CREATING EPOS REPORT FILE
					PMSRoomStatusTable.UpdateAccountBalance( MAX_PMS_ROOMS, nAccountIdx, TRUE );
					
					if ( BookingRecord.GetTrainingFlag() == FALSE )
					{
						bool bDBBCheck = ( BookingRecord.GetRoomRateType() == PMS_BOOKING_RATE_DBB ) && ( 0 == nAccountIdx );
						 
						CPMSInvoiceArray arrayInvoice;
						arrayInvoice.BuildInvoiceArray( BookingRecord, nAccountIdx + 1, PMS_INVOICETYPE_EPOS_REPORT, bDBBCheck );
						arrayInvoice.CreateEPOSReportFile( MAX_PMS_ROOMS, nAccountIdx );
					}

					CPMSRoomAccountInfo infoAccount;
					PMSRoomStatusTable.GetAccountInfo( MAX_PMS_ROOMS, nAccountIdx, infoAccount );

					int nBalanceType = CPriceHelpers::CompareDoubles( infoAccount.CalculateBalance(), 0.0, 3 );
					bDebit |= ( nBalanceType == 1 );
					bCredit |= ( nBalanceType == -1 );
				}

				int nNewBalanceType;
				if ( TRUE == bDebit )
					nNewBalanceType = ( TRUE == bCredit ) ? PMS_CHECKOUT_BALANCE_MIXED : PMS_CHECKOUT_BALANCE_DEBIT;
				else
					nNewBalanceType = ( TRUE == bCredit ) ? PMS_CHECKOUT_BALANCE_CREDIT : PMS_CHECKOUT_BALANCE_ZERO;

				if ( nNewBalanceType != BookingRecord.GetCheckoutBalanceType() )
				{
					BookingRecord.SetCheckoutBalanceType( nNewBalanceType );
					DataManagerNonDb.PMSBooking.InsertAndSaveBooking( BookingRecord );
				}
			}
		}

		StatusProgress.Unlock();
	}

	if ( ( TRUE == bPrompt ) && ( TRUE == bRebuild ) )
		Prompter.Info( "Sales History has been rebuilt as requested." );
}

/**********************************************************************/

void CPMSCalendarDlg::MenuBarOptionRebuildTax()
{
	if ( Prompter.YesNo( EcrmanOptions.GetTaxString( "Do you wish to rebuild the %T band history\nfor previous guest account sales.\n\n" ) ) == IDYES )
	{
		StatusProgress.Lock( TRUE, EcrmanOptions.GetTaxString( "Rebuild %T band history" ) );

		int nSize = DataManagerNonDb.PMSBooking.GetSize();
		for ( int nBookingIdx = 0; nBookingIdx < nSize ; nBookingIdx++ )
		{
			StatusProgress.SetPos( nBookingIdx, nSize );

			CPMSBookingCSVRecord BookingRecord;
			DataManagerNonDb.PMSBooking.GetAt( nBookingIdx, BookingRecord );

			PMSRoomStatusTable.LoadRoomAccountSettingsBuffer( BookingRecord, TRUE );

			CPMSRoomStatusInfo infoRoom;
			PMSRoomStatusTable.GetRoomInfo( MAX_PMS_ROOMS, infoRoom );

			for ( int nAccountIdx = 0; nAccountIdx < infoRoom.GetSafeAccountCount(); nAccountIdx++ )
				CPMSInvoiceArray::RebuildTaxBands( BookingRecord.GetBookingId(), nAccountIdx + 1 );
		}
			
		StatusProgress.Unlock();
		MenuBarOptionRebuildHistory( FALSE );

		CString strMsg = EcrmanOptions.GetSalesTaxName();
		strMsg += " band history has been rebuilt as requested.";
		Prompter.Info( strMsg );
	}
}

/**********************************************************************/

void CPMSCalendarDlg::MenuBarOptionBackupSystem() 
{
	AutoBackupProcessor.ManualBackup( this, TRUE );

	if ( TRUE == AutoBackupProcessor.GetWantQuitFlag() )
	{
		m_bForceQuit = TRUE;
		EndDialog( IDOK );
	}

	else if ( TRUE == AutoBackupProcessor.GetLoggedResultFlag() )
		m_pMainFrm -> ShowBackupResult( this, TRUE );
}

/**********************************************************************/

void CPMSCalendarDlg::MenuBarOptionUpdateEpos()
{
	CPMSDownloadMonitorDlg dlg( this );
	dlg.DoModal();
}

/**********************************************************************/

void CPMSCalendarDlg::MenuBarOptionBookRoom()
{
	CPMSBookingCSVRecord BookingRecord;
	BookingRecord.SetRoomRateType( PMSOptions.GetMiscDefaultRoomRateType() );
	CPMSBookSearchDlg dlgSearch( BookingRecord, this );
	
	if ( ( dlgSearch.DoModal() == IDOK ) && ( BookingRecord.GetPartCount() != 0 ) )
	{
		int nBookingId = DataManagerNonDb.PMSBooking.FindFirstFreeId();
		int nCustomerId = DataManagerNonDb.PMSCustomer.FindFirstFreeId();

		BookingRecord.SetBookingId( nBookingId );
		BookingRecord.SetCustomerId( nCustomerId );
		BookingRecord.SetBookingStatus( PMS_BOOKING_STATUS_PENDING );

		CPMSBookingPartInfo infoPart;
		BookingRecord.GetPartInfo( 0, infoPart );

		int nRoomIdx;
		if ( DataManagerNonDb.PMSRoom.FindRoomByNumber( infoPart.GetRoomNo(), nRoomIdx ) == TRUE )
		{
			CPMSRoomCSVRecord Room;
			DataManagerNonDb.PMSRoom.GetAt( nRoomIdx, Room );

			switch( BookingRecord.GetRoomRateType() )
			{
			case PMS_BOOKING_RATE_BB:
				BookingRecord.SetRoomRate( Room.GetRoomBBRate() );
				break;

			case PMS_BOOKING_RATE_DBB:
				BookingRecord.SetRoomRate( Room.GetRoomDBBRate() );
				break;

			case PMS_BOOKING_RATE_FREE:
				BookingRecord.SetRoomRate( 0.0 );
				break;

			case PMS_BOOKING_RATE_ROOM:
			default:
				BookingRecord.SetRoomRate( Room.GetRoomOnlyRate() );
				break;
			}

			if ( Room.GetSleeps() == BookingRecord.GetGuests() - 1 )
			{
				if ( Room.GetRoomAllowExtraBedFlag() == TRUE )
				{
					BookingRecord.SetTariffType( PMS_TARIFFTYPE_EXTRABED );
					BookingRecord.SetExtraBedRate( Room.GetRoomExtraBedRate() );
				}
				else if ( Room.GetRoomAllowExtraCotFlag() == TRUE )
				{
					BookingRecord.SetTariffType( PMS_TARIFFTYPE_EXTRACOT );
					BookingRecord.SetExtraBedRate( Room.GetRoomExtraCotRate() );
				}
			}
		}

		CPMSCustomerCSVRecord CustomerRecord;
		PMSRoomStatusTable.LoadRoomAccountSettingsBuffer( BookingRecord, FALSE );
		CPMSBookEditDlg dlgEdit( PMS_EDITBOOKING_PENDING_SINGLE_NEW, MAX_PMS_ROOMS, 0, BookingRecord, CustomerRecord, this );

		if ( dlgEdit.DoModal() == IDOK )
		{
			DataManagerNonDb.PMSCustomer.InsertAndSaveCustomer( CustomerRecord );

			CPMSBookingCSVArray::InsertAndSaveBooking( BookingRecord );
			PMSCalendarTable.ApplyBooking( BookingRecord );

			ScrollToBooking( BookingRecord );
		}
	}
}

/**********************************************************************/

void CPMSCalendarDlg::MenuBarOptionFindBooking()
{
	CPMSAccountFindDlg dlg( IDM_PMSMENUBAR_FIND_BOOKING, this );
	dlg.SetCalendarDlg( this );
	dlg.DoModal();
	FindAndSelectBooking( dlg.GetLastEditedBookingId() );
}

/**********************************************************************/

void CPMSCalendarDlg::MenuBarOptionCancelledBooking()
{
	CPMSAccountFindDlg dlg( IDM_PMSMENUBAR_CANCELLED_BOOKING, this );
	dlg.SetCalendarDlg( this );
	dlg.DoModal();
	FindAndSelectBooking( dlg.GetLastEditedBookingId() );
}

/**********************************************************************/

void CPMSCalendarDlg::MenuBarOptionGuestAccounts()
{
	CPMSAccountFindDlg dlg( IDM_PMSMENUBAR_GUEST_ACCOUNTS, this );
	dlg.DoModal();
	FindAndSelectBooking( dlg.GetLastEditedBookingId() );
}

/**********************************************************************/

void CPMSCalendarDlg::MenuBarOptionRoomList()
{
	CPMSPrintRoomsDlg dlg( this );
	dlg.DoModal();
}

/**********************************************************************/

void CPMSCalendarDlg::MenuBarOptionUnpaid()
{
	CPMSPrintUnpaidDlg dlg( this );
	dlg.DoModal();
}

/**********************************************************************/

void CPMSCalendarDlg::MenuBarOptionPayments()
{
	CPMSPrintPaymentsDlg dlg( this );
	dlg.DoModal();
}

/**********************************************************************/

void CPMSCalendarDlg::MenuBarOptionRoomGrid()
{
	CPropertySheetPMSRoomMonitor propSheet( this );
	propSheet.DoModal();
}

/**********************************************************************/

void CPMSCalendarDlg::OnContextMenuBookingPending()
{ 
	ContextMenuOption( "Edit Booking", IDM_PMSCONTEXT_BOOKING_PENDING );
}

/**********************************************************************/

void CPMSCalendarDlg::OnContextMenuBookingActive()
{ 
	ContextMenuOption( "Edit Booking", IDM_PMSCONTEXT_BOOKING_ACTIVE );
}

/**********************************************************************/

void CPMSCalendarDlg::OnContextMenuBookingComplete()
{ 
	ContextMenuOption( "Edit Booking", IDM_PMSCONTEXT_BOOKING_COMPLETE );
}

/**********************************************************************/

void CPMSCalendarDlg::OnContextMenuBookRoom()
{ 
	ContextMenuOption( "Book Room", IDM_PMSCONTEXT_BOOK_ROOM );
}

/**********************************************************************/

void CPMSCalendarDlg::OnContextMenuCheckIn()
{ 
	ContextMenuOption( "Check In", IDM_PMSCONTEXT_CHECKIN );
}

/**********************************************************************/

void CPMSCalendarDlg::OnContextMenuCheckOut()
{ 
	ContextMenuOption( "Check Out", IDM_PMSCONTEXT_CHECKOUT );
}

/**********************************************************************/

void CPMSCalendarDlg::OnContextMenuCloseRoom()
{ 
	ContextMenuOption( "Close Room", IDM_PMSCONTEXT_CLOSE_ROOM );
}

/**********************************************************************/

void CPMSCalendarDlg::OnContextMenuCloseEdit()
{ 
	ContextMenuOption( "Edit Room Closure", IDM_PMSCONTEXT_CLOSE_EDIT );
}

/**********************************************************************/

void CPMSCalendarDlg::OnContextMenuInvoicePending()
{ 
	ContextMenuOption( "Print Invoices", IDM_PMSCONTEXT_INVOICE_PENDING );
}

/**********************************************************************/

void CPMSCalendarDlg::OnContextMenuInvoiceActive()
{ 
	ContextMenuOption( "Print Invoices", IDM_PMSCONTEXT_INVOICE_ACTIVE );
}

/**********************************************************************/

void CPMSCalendarDlg::OnContextMenuInvoiceComplete()
{ 
	ContextMenuOption( "Print Invoices", IDM_PMSCONTEXT_INVOICE_COMPLETE );
}

/**********************************************************************/

void CPMSCalendarDlg::OnContextMenuPaymentsActive()
{ 
	ContextMenuOption( "Manual Payments", IDM_PMSCONTEXT_PAYMENTS_ACTIVE );
}

/**********************************************************************/

void CPMSCalendarDlg::OnContextMenuPaymentsComplete()
{ 
	ContextMenuOption( "Manual Payments", IDM_PMSCONTEXT_PAYMENTS_COMPLETE );
}

/**********************************************************************/

void CPMSCalendarDlg::OnContextMenuSalesActive()
{ 
	ContextMenuOption( "Manual Sales", IDM_PMSCONTEXT_SALES_ACTIVE );
}

/**********************************************************************/

void CPMSCalendarDlg::OnContextMenuSalesComplete()
{ 
	ContextMenuOption( "Manual Sales", IDM_PMSCONTEXT_SALES_COMPLETE );
}

/**********************************************************************/

void CPMSCalendarDlg::OnContextMenuSwitchRoomPending()
{
	ContextMenuOption( "Switch Room", IDM_PMSCONTEXT_SWITCHROOM_PENDING );
}

/**********************************************************************/

void CPMSCalendarDlg::OnContextMenuSwitchRoomActive()
{
	ContextMenuOption( "Switch Room", IDM_PMSCONTEXT_SWITCHROOM_ACTIVE );
}

/**********************************************************************/

void CPMSCalendarDlg::OnContextMenuExtendBookingToday()
{
	ContextMenuOption( "Extend Booking", IDM_PMSCONTEXT_EXTEND_BOOKING_TODAY );
}

/**********************************************************************/

void CPMSCalendarDlg::OnContextMenuExtendBookingTomorrow()
{
	ContextMenuOption( "Extend Booking", IDM_PMSCONTEXT_EXTEND_BOOKING_TOMORROW );
}

/**********************************************************************/

void CPMSCalendarDlg::OnContextMenuExtendBookingComplete()
{
	ContextMenuOption( "Extend Booking", IDM_PMSCONTEXT_EXTEND_BOOKING_COMPLETE );
}

/**********************************************************************/

void CPMSCalendarDlg::ContextMenuOption ( const char* szText, int nOption )
{
	m_ToolTipCtrl.SendMessage (TTM_TRACKACTIVATE, FALSE, (LPARAM)&m_ToolInfo);

	PMSOptions.SetPMSTime();
	PMSRoomStatusTable.InvalidateRoomStatus();

	m_CalendarHelpers.SetSelectedBookingId( m_nMenuBookingId );
	m_CalendarHelpers.SetSelectedClosureId( m_nMenuClosureId );
	m_PMSStaticDate.RedrawWindow();
	m_PMSStaticCalendar.RedrawWindow();

	BlockTimer( TRUE );

	CLockManagerInfo infoLock;
	infoLock.m_bLockAllUsers = FALSE;
	infoLock.m_nCommsAction = COMMS_ACTION_NONE;
	infoLock.m_bStopComms = TRUE;
	infoLock.m_bStopCommsPMSExemption = FALSE;
	infoLock.m_strStopCommsPrompt = szText;
	infoLock.m_bStopPosTray = FALSE;
	infoLock.m_pParentWnd = this;

	CLockManager LockManager( infoLock );

	if ( LockManager.GetLock() == TRUE )
	{
		CDataManagerInfo infoDb;
		DataManager.CloseDatabase();
		DataManager.OpenDatabaseReadOnly( 0, infoDb );

		CPMSStatusHelpers PMSStatusHelpers;
		PMSStatusHelpers.ImportPMSSales();
		
		for ( int nRoomIdx = 0; nRoomIdx < DataManagerNonDb.PMSRoom.GetSize(); nRoomIdx++ )
			PMSRoomStatusTable.UpdateAccountBalance( nRoomIdx, nRoomIdx == m_nMenuRoomIdx );

		switch( nOption )
		{
		case IDM_PMSCONTEXT_BOOKING_PENDING:			ContextMenuOptionBookingPending();			break;
		case IDM_PMSCONTEXT_BOOKING_ACTIVE:				ContextMenuOptionBookingActive();			break;
		case IDM_PMSCONTEXT_BOOKING_COMPLETE:			ContextMenuOptionBookingComplete();			break;
		case IDM_PMSCONTEXT_BOOK_ROOM:					ContextMenuOptionBookRoom();				break;
		case IDM_PMSCONTEXT_CHECKIN:					ContextMenuOptionCheckIn();					break;
		case IDM_PMSCONTEXT_CHECKOUT:					ContextMenuOptionCheckOut();				break;
		case IDM_PMSCONTEXT_CLOSE_ROOM:					ContextMenuOptionCloseRoom();				break;
		case IDM_PMSCONTEXT_CLOSE_EDIT:					ContextMenuOptionCloseEdit();				break;
		case IDM_PMSCONTEXT_INVOICE_PENDING:			ContextMenuOptionInvoicePending();			break;
		case IDM_PMSCONTEXT_INVOICE_ACTIVE:				ContextMenuOptionInvoiceActive();			break;
		case IDM_PMSCONTEXT_INVOICE_COMPLETE:			ContextMenuOptionInvoiceComplete();			break;
		case IDM_PMSCONTEXT_PAYMENTS_ACTIVE:			ContextMenuOptionPaymentsActive();			break;
		case IDM_PMSCONTEXT_PAYMENTS_COMPLETE:			ContextMenuOptionPaymentsComplete();		break;
		case IDM_PMSCONTEXT_SALES_ACTIVE:				ContextMenuOptionSalesActive();				break;
		case IDM_PMSCONTEXT_SALES_COMPLETE:				ContextMenuOptionSalesComplete();			break;
		case IDM_PMSCONTEXT_SWITCHROOM_PENDING:			ContextMenuOptionSwitchRoomPending();		break;
		case IDM_PMSCONTEXT_SWITCHROOM_ACTIVE:			ContextMenuOptionSwitchRoomActive();		break;
		case IDM_PMSCONTEXT_EXTEND_BOOKING_TODAY:		ContextMenuOptionExtendBookingToday();		break;
		case IDM_PMSCONTEXT_EXTEND_BOOKING_TOMORROW:	ContextMenuOptionExtendBookingTomorrow();	break;
		case IDM_PMSCONTEXT_EXTEND_BOOKING_COMPLETE:	ContextMenuOptionExtendBookingComplete();	break;
		}

		DataManager.CloseDatabase();
	}

	if ( PMSOptions.GetMiscRetainSelectionFlag() == FALSE )
	{
		m_CalendarHelpers.SetSelectedBookingId(0);
		m_CalendarHelpers.SetSelectedClosureId(0);
	}

	m_PMSStaticDate.RedrawWindow();
	m_PMSStaticCalendar.RedrawWindow();

	BlockTimer( FALSE );
}

/**********************************************************************/

void CPMSCalendarDlg::ContextMenuOptionBookRoom()
{	
	CPMSRoomCSVRecord Room;
	DataManagerNonDb.PMSRoom.GetAt( m_nMenuRoomIdx, Room );

	CPMSBookingQueryInfo Query;
	Query.m_nRoomNo = Room.GetRoomNo();
	Query.m_nStartDay = m_nMenuDay;
	Query.m_nMinNights = 1;
	Query.m_nMaxNights = 1;
	
	CPMSBookingResponseInfo Response;
	PMSCalendarTable.CheckRoomAvailability( Query, Response );

	if ( 0 == Response.m_nBookingClashId )
		BookRoomFromCalendar( m_nMenuRoomIdx, Response );
}

/**********************************************************************/

void CPMSCalendarDlg::UpdateCompleteAccounts( int nBookingId, bool& bClearedDebit, bool& bClearedCredit )
{
	bClearedDebit = FALSE;
	bClearedCredit = FALSE;
	bool bAddedDebit = FALSE;
	bool bAddedCredit = FALSE;
	
	int nBookingIdx;
	CPMSBookingCSVRecord BookingRecord;
	if ( DataManagerNonDb.PMSBooking.FindBookingById( nBookingId, nBookingIdx ) == FALSE )
		return;

	DataManagerNonDb.PMSBooking.GetAt( nBookingIdx, BookingRecord );
	
	CPMSRoomStatusInfo infoRoom;
	PMSRoomStatusTable.GetRoomInfo( MAX_PMS_ROOMS, infoRoom );

	bool bDebit = FALSE;
	bool bCredit = FALSE;
	
	for ( int nAccountIdx = 0; nAccountIdx < infoRoom.GetSafeAccountCount(); nAccountIdx++ )
	{
		//MUST UPDATE BALANCE BEFORE CREATING EPOS REPORT
		PMSRoomStatusTable.UpdateAccountBalance( MAX_PMS_ROOMS, nAccountIdx, TRUE );

		CPMSInvoiceArray arrayInvoice;
			
		if ( BookingRecord.GetTrainingFlag() == FALSE )
		{
			bool bDBBCredit = ( BookingRecord.GetRoomRateType() == PMS_BOOKING_RATE_DBB ) && ( 0 == nAccountIdx );
			
			arrayInvoice.BuildInvoiceArray( BookingRecord, nAccountIdx + 1, PMS_INVOICETYPE_EPOS_REPORT, bDBBCredit );
			arrayInvoice.CreateEPOSReportFile( MAX_PMS_ROOMS, nAccountIdx );
		}
		else
			arrayInvoice.DeleteEPOSReportFile( MAX_PMS_ROOMS, nAccountIdx );

		CPMSRoomAccountInfo infoAccount;
		PMSRoomStatusTable.GetAccountInfo( MAX_PMS_ROOMS, nAccountIdx, infoAccount );

		int nBalanceType = CPriceHelpers::CompareDoubles( infoAccount.CalculateBalance(), 0.0, 3 );
		bDebit |= ( nBalanceType == 1 );
		bCredit |= ( nBalanceType == -1 );
	}

	int nNewBalanceType;
	if ( TRUE == bDebit )
		nNewBalanceType = ( TRUE == bCredit ) ? PMS_CHECKOUT_BALANCE_MIXED : PMS_CHECKOUT_BALANCE_DEBIT;
	else
		nNewBalanceType = ( TRUE == bCredit ) ? PMS_CHECKOUT_BALANCE_CREDIT : PMS_CHECKOUT_BALANCE_ZERO;

	int nOldBalanceType = BookingRecord.GetCheckoutBalanceType();
	
	switch( nOldBalanceType )
	{
	case PMS_CHECKOUT_BALANCE_DEBIT:
		bClearedDebit = ( FALSE == bDebit );
		bAddedCredit = ( TRUE == bCredit );
		break;

	case PMS_CHECKOUT_BALANCE_CREDIT:
		bAddedDebit = ( TRUE == bDebit );
		bClearedCredit = ( FALSE == bCredit );
		break;

	case PMS_CHECKOUT_BALANCE_MIXED:
		bClearedDebit = ( FALSE == bDebit );
		bClearedCredit = ( FALSE == bCredit );
		break;

	case PMS_CHECKOUT_BALANCE_ZERO:
		bAddedDebit = ( TRUE == bDebit );
		bAddedCredit = ( TRUE == bCredit );
		break;
	}

	if ( nNewBalanceType != nOldBalanceType )
	{
		BookingRecord.SetCheckoutBalanceType( nNewBalanceType );
		CPMSBookingCSVArray::InsertAndSaveBooking( BookingRecord );
		
		CStringArray arrayMsg;

		if ( TRUE == bClearedDebit )
		{
			arrayMsg.Add( "All outstanding debits for this booking have now been cleared." );
			arrayMsg.Add( "This booking will be removed from the unpaid accounts debit list." );
			arrayMsg.Add( "" );
		}
		else if ( TRUE == bAddedDebit )
		{
			arrayMsg.Add( "One or more accounts on this booking has a new debit balance." );
			arrayMsg.Add( "This booking will be added to the unpaid accounts debit list." );
			arrayMsg.Add( "" );
		}

		if ( TRUE == bClearedCredit )
		{
			arrayMsg.Add( "All outstanding credits for this booking have now been cleared." );
			arrayMsg.Add( "This booking will be removed from the unpaid accounts credit list." );
			arrayMsg.Add( "" );
		}
		else if ( TRUE == bAddedCredit )
		{
			arrayMsg.Add( "One or more accounts on this booking has a new credit balance." );
			arrayMsg.Add( "This booking will be added to the unpaid accounts credit list." );
			arrayMsg.Add( "" );
		}

		if ( arrayMsg.GetSize() > 0 )
		{
			CString strMsg;
			for ( int n = 0; n < arrayMsg.GetSize() - 1; n++ )
			{
				strMsg += arrayMsg.GetAt(n);
				strMsg += "\n";
			}

			Prompter.Info( strMsg );
		}
	}
}

/**********************************************************************/

void CPMSCalendarDlg::FindAndSelectBooking( int nBookingId )
{
	int nBookingIdx;
	if ( DataManagerNonDb.PMSBooking.FindBookingById( nBookingId, nBookingIdx ) == TRUE )
	{
		CPMSBookingCSVRecord BookingRecord;
		DataManagerNonDb.PMSBooking.GetAt( nBookingIdx, BookingRecord );
		ScrollToBooking( BookingRecord );
	}
}

/**********************************************************************/

void CPMSCalendarDlg::ScrollToBooking( CPMSBookingCSVRecord& BookingRecord )
{
	CPMSBookingPartInfo infoPart;
	BookingRecord.GetPartInfo(0, infoPart);
	ScrollToDate( infoPart.GetStartDay() );

	int nRoomIdx;
	if ( DataManagerNonDb.PMSRoom.FindRoomByNumber( infoPart.GetRoomNo(), nRoomIdx ) == TRUE )
		ScrollToRoom( nRoomIdx );

	m_CalendarHelpers.SetSelectedClosureId( 0 );
	m_CalendarHelpers.SetSelectedBookingId( BookingRecord.GetBookingId() );
}

/**********************************************************************/

BOOL CPMSCalendarDlg::OnToolTipNotify( UINT id, NMHDR * pNMHDR, LRESULT * pResult )
{
    // Get the tooltip structure.
    TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pNMHDR;

    // Actually the idFrom holds Control's handle.
    UINT CtrlHandle = pNMHDR->idFrom;

    // Check once again that the idFrom holds handle itself.
    if (pTTT->uFlags & TTF_IDISHWND)
    {
        // Get the control's ID.
        UINT nID = ::GetDlgCtrlID( HWND( CtrlHandle ));

        // Now you have the ID. depends on control,
        // set your tooltip message.
        switch( nID )
        {
        case IDC_STATIC_CALENDAR:
			{
				CString strOld = m_strToolTipText;

				UpdateToolTipText();
				pTTT->lpszText = m_strToolTipText.GetBuffer(0);
				m_strToolTipText.ReleaseBuffer();
			}
            break;  
		}

        return TRUE;
    }

    // Not handled.
    return FALSE;
}

/**********************************************************************/

void CPMSCalendarDlg::ResetToolTipText( bool bResetAll )
{
	m_strToolTipText = " ";

	if ( TRUE == bResetAll )
	{
		m_nToolTipRoomIdx = -1;
		m_nToolTipDayNo = -1;
	}
}

/**********************************************************************/

void CPMSCalendarDlg::UpdateToolTipText()
{
	int nCalendarRow = m_PMSStaticCalendar.GetCalendarMoveRow();
	int nCalendarCol = m_PMSStaticCalendar.GetCalendarMoveCol();
	
	if ( ( -1 == nCalendarCol ) || ( -1 == nCalendarRow ) )
	{
		ResetToolTipText( TRUE );
		return;
	}

	int nMouseRoomIdx = m_CalendarHelpers.GetCalendarStartRoom() + nCalendarRow;
	int nMouseDayNo = m_CalendarHelpers.GetCalendarClickDay( nCalendarCol );

	if ( ( nMouseRoomIdx == m_nToolTipRoomIdx ) && ( nMouseDayNo == m_nToolTipDayNo ) )
		return;

	m_nToolTipRoomIdx = nMouseRoomIdx;
	m_nToolTipDayNo = nMouseDayNo;
	ResetToolTipText( FALSE );

	CPMSRoomCSVRecord Room;
	DataManagerNonDb.PMSRoom.GetAt( m_nToolTipRoomIdx, Room );

	CPMSBookingQueryInfo Query;
	Query.m_nRoomNo = Room.GetRoomNo();
	Query.m_nStartDay = m_nToolTipDayNo;
	Query.m_nMinNights = 1;
	Query.m_nMaxNights = 1;
	
	CPMSBookingResponseInfo Response;
	PMSCalendarTable.CheckRoomAvailability( Query, Response );

	if ( 0 == Response.m_nBookingClashId )
		return;

	CPMSCalendarBooking CalendarBooking;
	PMSCalendarTable.GetAt( Response.m_nBookingClashArrayIdx, CalendarBooking );

	if ( PMS_BOOKING_STATUS_CLOSED == CalendarBooking.m_nStatus )
	{
		m_strToolTipText = "Closed Room";

		int nClosureIdx;
		if ( DataManagerNonDb.PMSClosure.FindClosureById( Response.m_nBookingClashId, nClosureIdx ) == TRUE )
		{
			CPMSClosedRoomCSVRecord Closure;
			DataManagerNonDb.PMSClosure.GetAt( nClosureIdx, Closure );

			CString strReason = Closure.GetReason();
			::TrimSpaces( strReason, FALSE );

			if ( strReason != "" )
			{
				m_strToolTipText += ", ";
				m_strToolTipText += strReason;
			}
		}

		return;
	}
		
	int nBookingIdx;
	if ( DataManagerNonDb.PMSBooking.FindBookingById( Response.m_nBookingClashId, nBookingIdx ) == FALSE )
		return;

	CPMSBookingCSVRecord BookingRecord;
	DataManagerNonDb.PMSBooking.GetAt( nBookingIdx, BookingRecord );

	if ( BookingRecord.GetPartCount() == 0 )
		return;

	m_strToolTipText = "";

	switch( BookingRecord.GetBookingStatus() )
	{
	case PMS_BOOKING_STATUS_PENDING:	m_strToolTipText = "Pending Booking";	break;
	case PMS_BOOKING_STATUS_ACTIVE:		m_strToolTipText = "Active Booking";	break;
	case PMS_BOOKING_STATUS_COMPLETE:	m_strToolTipText = "Completed Booking";	break;
	}

	CString strName = "";

	int nCustomerIdx;
	int nCustomerId = BookingRecord.GetCustomerId();
	if ( DataManagerNonDb.PMSCustomer.FindCustomerById( nCustomerId, nCustomerIdx ) == TRUE )
	{
		CPMSCustomerCSVRecord Customer;
		DataManagerNonDb.PMSCustomer.GetAt( nCustomerIdx, Customer );

		CString strGuestName = Customer.GetGuestName();
		::TrimSpaces( strGuestName, FALSE );

		if ( strGuestName != "" )
		{
			m_strToolTipText += ", ";
			m_strToolTipText += strGuestName;
		}
	}

	CString strGuests = "";

	int nGuests = BookingRecord.GetGuests();
	if ( 1 == nGuests )
		strGuests = ", 1 Guest";
	else
		strGuests.Format( ", %d Guests", nGuests );

	m_strToolTipText += strGuests;

	CString strTariff;
	switch ( BookingRecord.GetRoomRateType() )
	{
	case PMS_BOOKING_RATE_ROOM:		m_strToolTipText += ", Room Only";	break;
	case PMS_BOOKING_RATE_BB:		m_strToolTipText += ", B && B";		break;
	case PMS_BOOKING_RATE_DBB:		m_strToolTipText += ", DB && B";	break;
	case PMS_BOOKING_RATE_FREE:		m_strToolTipText += ", Free Room";	break;
	default:						m_strToolTipText += "";				break;
	}

	CString strRate;
	strRate.Format( ", %.2f per night", BookingRecord.GetRoomRate() + BookingRecord.GetExtraBedRate() );
	m_strToolTipText += strRate;
}

/**********************************************************************/

void CPMSCalendarDlg::OnDestroy()
{	
	if ( m_hTimer != NULL )
		KillTimer( m_hTimer );

	CDialog::OnDestroy();
}

/**********************************************************************/
#endif
/**********************************************************************/
