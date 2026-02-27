#pragma once
/**********************************************************************/

struct CBusyFileUpdateThreadInfo
{
public:
	CBusyFileUpdateThreadInfo();

public:
	CString m_strBusyFilename = "";
	bool m_bCanBeBusy = FALSE;
	bool m_bWantQuit = FALSE;
};

/**********************************************************************/

class CBusyFileUpdateThread
{
public:
	static UINT DoWork( LPVOID pParam );

private:
	static void DoWorkInternal(CBusyFileUpdateThreadInfo* pInfo );
};

/**********************************************************************/
