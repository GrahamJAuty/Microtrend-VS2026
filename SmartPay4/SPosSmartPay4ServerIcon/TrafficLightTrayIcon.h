#pragma once
//$$******************************************************************
// Modified Microsoft Tray Icon, original message below
//$$******************************************************************
// CTrayIcon Copyright 1996 Microsoft Systems Journal.
//
// If this code works, it was written by Paul DiLascia.
// If not, I don't know who wrote it.
//$$******************************************************************

class CTrafficLightTrayIcon
{
public:
	CTrafficLightTrayIcon ( UINT uID );
	~CTrafficLightTrayIcon();

	bool CreateIconCache();
	void SetNotificationWnd ( CWnd* pNotifyWnd, UINT uCbMsg );	
	virtual LRESULT OnTrayNotification(WPARAM uID, LPARAM lEvent);

public:
	BOOL SetRedIcon();
	BOOL SetYellowIcon();
	BOOL SetGreenIcon();
	
private:
	BOOL SetIcon(int nIconWanted, CString strText);		
	BOOL SetColourIcon(int nIconWanted, CString strStatus, CString strColour);
	bool IsWindowsInDarkMode();

protected:
	NOTIFYICONDATA m_nid;			

private:
	int m_nLastIcon;
	HICON m_IconCache[6];
	bool m_bCacheCreated;
};

//$$******************************************************************

