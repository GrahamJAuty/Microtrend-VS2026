#pragma once
/**********************************************************************/
#ifdef POSTRAY_UTILITY
/**********************************************************************/
// CTrayIcon Copyright 1996 Microsoft Systems Journal.
//
// If this code works, it was written by Paul DiLascia.
// If not, I don't know who wrote it.
/**********************************************************************/

class CTrayIcon
{
public:
	CTrayIcon ( UINT uID );
	~CTrayIcon();

	void SetNotificationWnd ( CWnd* pNotifyWnd, UINT uCbMsg );	// Call this to receive tray notifications

	// SetIcon functions. To remove icon, call SetIcon(0)

	BOOL SetIcon ( UINT uID );		// main variant you want to use
	BOOL SetIcon ( HICON hicon, LPCSTR lpTip );

	virtual LRESULT OnTrayNotification ( WPARAM uID, LPARAM lEvent );

protected:
	NOTIFYICONDATA m_nid;			// struct for Shell_NotifyIcon args
};

/**********************************************************************/
#endif
/**********************************************************************/
