//$$******************************************************************
#include "..\SPosLoyalty4Shared\MessageLogger.h"
#include "..\SPosLoyalty4Shared\ServiceOptions.h"
//$$******************************************************************
#include "SPosLoyalty4ServerIcon.h"
#include "TrafficLightTrayIcon.h"
//$$******************************************************************
#include "..\SPosLoyalty4ServerIcon\Resource.h"
//$$******************************************************************

CTrafficLightTrayIcon::CTrafficLightTrayIcon( UINT uID )
{
	memset(&m_nid, 0, sizeof(m_nid));	// Initialize NOTIFYICONDATA
	m_nid.cbSize = sizeof(m_nid);
	m_nid.uID = uID;					// never changes after construction
	m_nLastIcon = -1;
	m_bCacheCreated = FALSE;

	for (int i = 0; i < 6; i++)
	{
		m_IconCache[i] = NULL;
	}
}

//$$******************************************************************

CTrafficLightTrayIcon::~CTrafficLightTrayIcon()
{
	if (m_nid.hWnd != NULL)
	{
		Shell_NotifyIcon(NIM_DELETE, &m_nid);
	}

	for (int i = 0; i < 6; i++)
	{
		if (m_IconCache[i] != NULL)
		{
			DestroyIcon(m_IconCache[i]);
			m_IconCache[i] = NULL;
		}
	}
}

//$$******************************************************************

void CTrafficLightTrayIcon::SetNotificationWnd ( CWnd* pNotifyWnd, UINT uCbMsg )
{
	// If the following assert fails, you're probably
	// calling me before you created your window. Oops.

	ASSERT ( pNotifyWnd == NULL || ::IsWindow(pNotifyWnd->GetSafeHwnd()) );
	m_nid.hWnd = pNotifyWnd->GetSafeHwnd();

	ASSERT ( uCbMsg == 0 || uCbMsg >= WM_USER );
	m_nid.uCallbackMessage = uCbMsg;
}

//$$******************************************************************

bool CTrafficLightTrayIcon::CreateIconCache()
{
	if (FALSE == m_bCacheCreated)
	{
		UINT iconsToCreate[6];
		iconsToCreate[0] = IDI_ICON_RED;
		iconsToCreate[1] = IDI_ICON_YELLOW;
		iconsToCreate[2] = IDI_ICON_GREEN;
		iconsToCreate[3] = IDI_ICON_DARKRED;
		iconsToCreate[4] = IDI_ICON_DARKYELLOW;
		iconsToCreate[5] = IDI_ICON_DARKGREEN;

		for (int i = 0; i < 6; i++)
		{
			m_IconCache[i] = (HICON)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(iconsToCreate[i]), IMAGE_ICON, 16, 16, 0);

			if (m_IconCache[i] == NULL)
			{
				return FALSE;
			}
		}

		m_bCacheCreated = TRUE;
	}

	return TRUE;
}

//$$******************************************************************

bool CTrafficLightTrayIcon::IsWindowsInDarkMode()
{
	switch (ServerServiceOptions.GetServerIconType())
	{
	case 1:
		return TRUE;

	case 2:
		return FALSE;
	}

	bool bDarkMode = false;
	DWORD value = 0;
	DWORD dataSize = sizeof(value);
	HKEY hKey;

	if (RegOpenKeyEx(HKEY_CURRENT_USER,
		"Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize",
		0, KEY_READ, &hKey) == ERROR_SUCCESS)
	{
		if (RegQueryValueEx(hKey, "AppsUseLightTheme", nullptr, nullptr,
			reinterpret_cast<LPBYTE>(&value), &dataSize) == ERROR_SUCCESS)
		{
			bDarkMode = (0 == value);
		}
		RegCloseKey(hKey);
	}

	return bDarkMode;
}

//$$******************************************************************

BOOL CTrafficLightTrayIcon::SetRedIcon()
{
	return SetColourIcon(IsWindowsInDarkMode() ? 3 : 0, "Not Running", "(Red)");
}

//$$******************************************************************

BOOL CTrafficLightTrayIcon::SetYellowIcon()
{
	return SetColourIcon(IsWindowsInDarkMode() ? 4 : 1, "Status Unknown", "(Yellow)");
}

//$$******************************************************************

BOOL CTrafficLightTrayIcon::SetGreenIcon()
{
	return SetColourIcon(IsWindowsInDarkMode() ? 5 : 2, "Running", "(Green)");
}

//$$******************************************************************

BOOL CTrafficLightTrayIcon::SetColourIcon(int nIconWanted, CString strStatus, CString strColour)
{
	bool bResult = TRUE;

	if (m_nLastIcon != nIconWanted)
	{
		CString strResult = "";
		strResult += "Loyalty Server ";
		strResult += strStatus;

		bResult = SetIcon(nIconWanted, strResult);

		if (TRUE == bResult)
		{
			m_nLastIcon = nIconWanted;
		}
		else
		{
			CString strMessage = "";
			strMessage += "Server Tray Icon Colour Change Fail ";
			strMessage += strColour;

			MessageLogger.LogSystemMessage(strMessage);
			theApp.m_bFailIconChange = TRUE;
		}
	}

	return bResult;
}

//$$******************************************************************

BOOL CTrafficLightTrayIcon::SetIcon(int nIconWanted, CString strText)
{
	UINT msg = (m_nid.hIcon != NULL) ? NIM_MODIFY : NIM_ADD;;

	if (strText.GetLength() > 0)								// Use the tip, if any
	{
		strncpy_s(m_nid.szTip, strText, strText.GetLength());
	}

	if (m_nid.szTip[0] != 0)
	{
		m_nid.uFlags |= NIF_TIP;
	}

	if (m_nid.uCallbackMessage != 0 && m_nid.hWnd != NULL)	// Use callback if any
	{
		m_nid.uFlags |= NIF_MESSAGE;
	}

	m_nid.uFlags |= NIF_ICON;

	m_nid.hIcon = m_IconCache[nIconWanted];

	return Shell_NotifyIcon(msg, &m_nid);
}

//$$******************************************************************

LRESULT CTrafficLightTrayIcon::OnTrayNotification ( WPARAM wID, LPARAM lEvent )
{
	if (wID != m_nid.uID || (lEvent != WM_RBUTTONUP && lEvent != WM_LBUTTONDBLCLK))
	{
		return 0;
	}

	// If there's a resource menu with the same ID as the icon, use it as 
	// the right-button popup menu. CTrayIcon will interprets the first
	// item in the menu as the default command for WM_LBUTTONDBLCLK

	CMenu menu;
	if (menu.LoadMenu(m_nid.uID) == FALSE)
	{
		return 0;
	}

	CMenu* pSubMenu = menu.GetSubMenu(0);
	if (pSubMenu == NULL)
	{
		return 0;
	}

	if ( lEvent == WM_RBUTTONUP )
	{
		::SetMenuDefaultItem ( pSubMenu->m_hMenu, 0, TRUE );	// Make first menu item the default (bold font)

		CPoint mouse;
		GetCursorPos ( &mouse );

		::SetForegroundWindow ( m_nid.hWnd );	
		::TrackPopupMenu ( pSubMenu->m_hMenu, 0, mouse.x, mouse.y, 0, m_nid.hWnd, NULL );
	}
	else			// double click: execute first menu item
	{
		::SetForegroundWindow ( m_nid.hWnd );		// added by PMW
		::SendMessage ( m_nid.hWnd, WM_COMMAND, pSubMenu->GetMenuItemID(0), 0 );
	}

	return 1;		// handled
}

//$$******************************************************************
