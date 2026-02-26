#pragma once
/**********************************************************************/
#include "..\SPosLoyalty4Shared\ServerData.h"
/**********************************************************************/
#include "LoyReqTidyUpThread.h"
/**********************************************************************/

class CLoyReqTidyUpThreadManager
{
public:
	CLoyReqTidyUpThreadManager();
	~CLoyReqTidyUpThreadManager();

public:
	void ShutdownThreads();

public:
	CServerDataFolderSetIndex GetThreadIndex() { return m_ThreadIndex; }
	
public:
	void CheckThread(CServerDataFolderSetIndex& ThreadIndex);
	
private:
	CLoyReqTidyUpThreadInfo m_arrayThreadInfo[PLU_DBNO_COUNT + 1][EPOSREQPATH_COUNT + 1];
	CReportConsolidationArray<CTidyUpFileInfo> m_TidyUpFileBuffer[PLU_DBNO_COUNT + 1][EPOSREQPATH_COUNT + 1];
	CWinThread* m_pThread[PLU_DBNO_COUNT + 1][EPOSREQPATH_COUNT + 1];

private:
	CServerDataFolderSetIndex m_ThreadIndex;
};

/**********************************************************************/
