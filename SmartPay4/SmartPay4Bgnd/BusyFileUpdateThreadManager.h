#pragma once
/**********************************************************************/
#include "BusyFileUpdateThread.h"
/**********************************************************************/

class CBusyFileUpdateThreadManager
{
public:
	CBusyFileUpdateThreadManager();
	~CBusyFileUpdateThreadManager();

public:
	void SetCanBeBusy(bool bCanBeBusy) { m_ThreadInfo.m_bCanBeBusy = bCanBeBusy; }
	void ShutdownThread();	
	void CheckThread();
	
private:
	CBusyFileUpdateThreadInfo m_ThreadInfo;
	CWinThread* m_pThread;
};

/**********************************************************************/
