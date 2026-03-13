#pragma once
/**********************************************************************/

class CSocketListenerThread
{
public:
	static UINT DoWork(LPVOID pParam);

private:
	static void LogThread(LPVOID pParam, CString strMsg);
};

/**********************************************************************/