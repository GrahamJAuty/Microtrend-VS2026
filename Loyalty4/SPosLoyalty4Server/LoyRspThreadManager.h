#pragma once
/**********************************************************************/
#include "..\SPosLoyalty4Shared\ServerData.h"
/**********************************************************************/
#include "LoyRspThread.h"
/**********************************************************************/

class CLoyRspThreadManager
{
public:
	CLoyRspThreadManager();
	~CLoyRspThreadManager();

public:
	void ShutdownThreads();

public:
	void QueueResponseHeader(CServerDataFolderSetIndex& ThreadIndex);
	void QueueResponseLine(CServerDataFolderSetIndex& ThreadIndex, CString strLine);
	void CheckThread(CServerDataFolderSetIndex& ThreadIndex);

private:
	CString GetResponseHeader();

private:
	CLoyRspThreadInfo m_arrayThreadInfo[PLU_DBNO_COUNT + 1][EPOSREQPATH_COUNT + 1];
	CStringArray m_ResponseQueue[PLU_DBNO_COUNT + 1][EPOSREQPATH_COUNT + 1];
	CStringArray m_ResponseCurrent[PLU_DBNO_COUNT + 1][EPOSREQPATH_COUNT + 1];
	CWinThread* m_pThread[PLU_DBNO_COUNT + 1][EPOSREQPATH_COUNT + 1];
};

/**********************************************************************/
