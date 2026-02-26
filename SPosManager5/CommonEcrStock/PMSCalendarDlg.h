#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#ifdef COMPILE_PMS 
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "..\SPosEcrManager5\MainFrmEcrman.h"
#include "..\SPosStockManager5\MainFrmStockman.h"
#include "PMSBookingCSVArray.h"
#include "PMSCalendarHelpers.h"
#include "PMSClosedRoomCSVArray.h"
#include "PMSStaticCalendar.h"
#include "PMSStaticDate.h"
#include "PMSStructs.h"
#include "WmNumbers.h"
/**********************************************************************/
static const UINT BALANCE_TIMER_CALENDAR = 1;
/**********************************************************************/

class CPMSCalendarDlg : public CSSDialog
{
	DECLARE_DYNAMIC(CPMSCalendarDlg)

public:

#ifndef STOCKMAN_SYSTEM
	CPMSCalendarDlg( CMainFrameEcrman* pParent = NULL);   
#else
	CPMSCalendarDlg( CMainFrameStockman* pParent = NULL);
#endif

	virtual ~CPMSCalendarDlg();

private:
	CScrollBar m_VScrollBar;
	CScrollBar m_HScrollBar;
	CPMSStaticDate m_PMSStaticDate;
	CPMSStaticCalendar m_PMSStaticCalendar;

	enum { IDD = IDD_PMS_CALENDAR };

public:
	virtual BOOL OnInitDialog();
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg long OnClickedDate( WPARAM wDummy, LPARAM lPoint );
	afx_msg long OnLClickedRoom( WPARAM wDummy, LPARAM lPoint );
	afx_msg long OnRClickedRoom( WPARAM wDummy, LPARAM lPoint );
	afx_msg long OnClickDeadArea( WPARAM wDummy, LPARAM lPoint );
	afx_msg long OnMouseMoveCalendar( WPARAM wDummy, LPARAM lPoint );
	afx_msg void OnMenuBackupSystem();
	afx_msg void OnMenuBackupStatus();
	afx_msg void OnMenuUpdateEpos();
	afx_msg void OnMenuBookRoom();
	afx_msg void OnMenuCheckIn();
	afx_msg void OnMenuCheckOutActive();
	afx_msg void OnMenuCheckOutUnpaidDebit();
	afx_msg void OnMenuCheckOutUnpaidCredit();
	afx_msg void OnMenuBatchInvoices();
	afx_msg void OnMenuHighlightAccounts();
	afx_msg void OnMenuGuestAccounts();
	afx_msg void OnMenuFindBooking();
	afx_msg void OnMenuCancelledBooking();
	afx_msg void OnMenuReportsRoomList();
	afx_msg void OnMenuReportsUnpaid();
	afx_msg void OnMenuReportsPayments();
	afx_msg void OnMenuRoomGrid();
	afx_msg void OnMenuExit();
	afx_msg void OnContextMenuBookRoom();
	afx_msg void OnContextMenuCloseRoom();
	afx_msg void OnContextMenuCloseEdit();
	afx_msg void OnContextMenuCheckIn();
	afx_msg void OnContextMenuBookingPending();
	afx_msg void OnContextMenuBookingActive();
	afx_msg void OnContextMenuBookingComplete();
	afx_msg void OnContextMenuSwitchRoomPending();
	afx_msg void OnContextMenuSwitchRoomActive();
	afx_msg void OnContextMenuExtendBookingToday();
	afx_msg void OnContextMenuExtendBookingTomorrow();
	afx_msg void OnContextMenuExtendBookingComplete();
	afx_msg void OnContextMenuSalesActive();
	afx_msg void OnContextMenuSalesComplete();
	afx_msg void OnContextMenuPaymentsActive();
	afx_msg void OnContextMenuPaymentsComplete();
	afx_msg void OnContextMenuCheckOut();
	afx_msg void OnContextMenuInvoicePending();
	afx_msg void OnContextMenuInvoiceActive();
	afx_msg void OnContextMenuInvoiceComplete();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg LRESULT OnKickIdle( WPARAM, LPARAM );
	BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnToolTipNotify( UINT id, NMHDR* pNMHDR, LRESULT* pResult );

	afx_msg void OnSpecialKeyA();
	afx_msg void OnSpecialKeyB();
	afx_msg void OnSpecialKeyC();
	afx_msg void OnSpecialKeyD();
	afx_msg void OnSpecialKeyE();
	afx_msg void OnSpecialKeyF();
	afx_msg void OnSpecialKeyG();
	afx_msg void OnSpecialKeyH();
	afx_msg void OnSpecialKeyI();
	afx_msg void OnSpecialKeyJ();
	afx_msg void OnSpecialKeyK();
	afx_msg void OnSpecialKeyL();
	afx_msg void OnSpecialKeyM();
	afx_msg void OnSpecialKeyN();
	afx_msg void OnSpecialKeyO();
	afx_msg void OnSpecialKeyP();
	afx_msg void OnSpecialKeyQ();
	afx_msg void OnSpecialKeyR();
	afx_msg void OnSpecialKeyS();
	afx_msg void OnSpecialKeyT();
	afx_msg void OnSpecialKeyU();
	afx_msg void OnSpecialKeyV();
	afx_msg void OnSpecialKeyW();
	afx_msg void OnSpecialKeyX();
	afx_msg void OnSpecialKeyY();
	afx_msg void OnSpecialKeyZ();
	afx_msg void OnSpecialKey0();
	afx_msg void OnSpecialKey1();
	afx_msg void OnSpecialKey2();
	afx_msg void OnSpecialKey3();
	afx_msg void OnSpecialKey4();
	afx_msg void OnSpecialKey5();
	afx_msg void OnSpecialKey6();
	afx_msg void OnSpecialKey7();
	afx_msg void OnSpecialKey8();
	afx_msg void OnSpecialKey9();
	
private:
	void ProcessLeftClickRoom();
	void BookRoomFromCalendar( int nRoomIdx, CPMSBookingResponseInfo& Response );
	void CloseRoomFromCalendar( int nRoomIdx, CPMSBookingResponseInfo& Response );

private:
	void ProcessPrebookedCheckInByRoomIdx( int nRoomIdx );
	bool FinaliseCheckIn( int nRoomIdx, CPMSBookingCSVRecord& BookingRecord, bool bHighlight );
	static void CancelCheckIn( int nRoomIdx, CPMSBookingCSVRecord& BookingRecord );

private:
	void MenuBarOption( const char* szText, int nOption, bool bForceUpdate = FALSE, bool bStopTimer = TRUE );
	void MenuBarOptionBackupSystem();
	void MenuBarOptionUpdateEpos();
	void MenuBarOptionBookRoom();
	void MenuBarOptionCheckIn();
	void MenuBarOptionCheckOutActive();
	void MenuBarOptionCheckOutUnpaidDebit();
	void MenuBarOptionCheckOutUnpaidCredit();
	void MenuBarOptionBatchInvoices();
	void MenuBarOptionHighlightAccounts();
	void MenuBarOptionFindBooking();
	void MenuBarOptionCancelledBooking();
	void MenuBarOptionGuestAccounts();
	void MenuBarOptionRoomList();
	void MenuBarOptionUnpaid();
	void MenuBarOptionPayments();
	void MenuBarOptionRoomGrid();
	void MenuBarOptionRebuildHistory( bool bPrompt );
	void MenuBarOptionRebuildTax();

private:
	void ContextMenuOption( const char* szText, int nOption );
	void ContextMenuOptionBookingPending();
	void ContextMenuOptionBookingActive();
	void ContextMenuOptionBookingComplete();
	void ContextMenuOptionBookRoom();
	void ContextMenuOptionCheckIn();
	void ContextMenuOptionCheckOut();
	void ContextMenuOptionCloseRoom();
	void ContextMenuOptionCloseEdit();
	void ContextMenuOptionInvoicePending();
	void ContextMenuOptionInvoiceActive();
	void ContextMenuOptionInvoiceComplete();
	void ContextMenuOptionPaymentsActive();
	void ContextMenuOptionPaymentsComplete();
	void ContextMenuOptionSalesActive();
	void ContextMenuOptionSalesComplete();
	void ContextMenuOptionSwitchRoomPending();
	void ContextMenuOptionSwitchRoomActive();
	void ContextMenuOptionExtendBookingToday();
	void ContextMenuOptionExtendBookingTomorrow();
	void ContextMenuOptionExtendBookingComplete();

private:
	void EditBooking( int nStatus );
	void EditClosure( int nRoomIdx, int nClosureId );
	void MergeClosure( CPMSClosedRoomCSVRecord& ClosureRecord );
	void MergeClosures( CPMSClosedRoomCSVRecord& Closure1, CPMSClosedRoomCSVRecord& Closure2 );
	void FindAndSelectBooking( int nBookingId );

private:
	void UpdateReports();
	
public:
	static void EditBookingPending( int nBookingId, CWnd* pParentWnd );
	static void EditBookingCancelled( int nBookingId, CWnd* pParentWnd );
	static void EditBookingActive( int nRoomIdx, int nBookingId, CWnd* pParentWnd, CPMSCalendarDlg* pCalendarDlg );
	static void EditBookingComplete( int nBookingId, CWnd* pParentWnd, CPMSCalendarDlg* pCalendarDlg );

public:
	bool GetForceQuitFlag(){ return m_bForceQuit; }
	void UpdateDisplay();

private:
	void ProcessSalesActive( int nRoomIdx );
	void ProcessPaymentsActive( int nRoomIdx );

public:
	static void ProcessInvoicePending( int nRoomIdx, int nBookingId, CWnd* pParentWnd );
	static void ProcessInvoiceActive( int nRoomIdx, bool bCheckout, CWnd* pParentWnd );

public:
	static bool ProcessQuickPluSales( int nRoomIdx, int nAccountIdx, bool bTraining, CWnd* pParentWnd );

public:
	static bool ProcessSalesComplete( int nBookingId, CWnd* pParentWnd );
	static bool ProcessPaymentsComplete( int nBookingId, CWnd* pParentWnd );
	static bool ProcessInvoiceComplete( int nBookingId, CWnd* pParentWnd );	
	static void UpdateCompleteAccounts( int nBookingId, bool& bClearedDebit, bool& bClearedCredit );

private:
	void ProcessPrebookedCheckOut( int nRoomNo );
	void ProcessCheckOut( int nRoomIdx, bool bHighlight );
	void ProcessSpecialKey( const char* sz );

private:
	void ResizeControls();
	void ScrollToToday();
	void ScrollToDate( int nDay );
	void ScrollToRoom( int nRoomIdx );
	void ScrollToBooking( CPMSBookingCSVRecord& BookingRecord );
	const char* GetSwitchError( int nCheckinDay, int nCheckoutDay, int nGuests );
	
private:
	void ResetToolTipText( bool bResetAll );
	void UpdateToolTipText();

private:
	void BlockTimer( bool bBlock );
	
private:
	COleDateTime m_CheckBalanceTime;
	UINT m_hTimer;

private:
	HICON m_hIcon;
	HACCEL m_hAccelTable;

private:
	CToolTipCtrl m_ToolTipCtrl;
	CPMSCalendarHelpers m_CalendarHelpers;
	CPoint m_ClickPointDate;
	CPoint m_ClickPointRoom;

private:
	bool m_bInitDialog;
	int m_nMenuRoomIdx;
	int m_nMenuDay;
	int m_nMenuBookingId;
	int m_nMenuClosureId;
	CString m_strSwitchError;
	bool m_bForceQuit;
	bool m_bInTimer;
	bool m_bBlockTimer;
	
private:
	TOOLINFO m_ToolInfo;
	CString m_strToolTipText;
	int m_nToolTipRoomIdx;
	int m_nToolTipDayNo;

#ifndef STOCKMAN_SYSTEM
	CMainFrameEcrman* m_pMainFrm;   
#else
	CMainFrameStockman* m_pMainFrm;
#endif
};

/**********************************************************************/
#endif
/**********************************************************************/
#endif
/**********************************************************************/
