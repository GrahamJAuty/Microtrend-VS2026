#pragma once
/**********************************************************************/
#include "..\SPOSLoyalty4Shared\ServerData.h"
/**********************************************************************/
#include "EmptyLogFileCache.h"
/**********************************************************************/
#include "LoyLogThread.h"
/**********************************************************************/

class CLoyLogThreadManager
{
public:
	CLoyLogThreadManager();
	~CLoyLogThreadManager();

public:
	void ShutdownThreads();
	void CreateLocalLogFile();

public:
	CServerDataFolderSetIndex GetThreadIndex() { return m_ThreadIndex; }
	CString GetLOGFilepath() { return m_strLOGFilename; }
	CString GetLOGFilename() { return m_strLOGFilename; }
	CString GetRSPFilepath() { return m_strRSPFilepath; }
	CString GetRSPFilepathTemp() { return m_strRSPFilepathTemp; }
	int GetLOGLineCount() { return m_arrayLOG.GetSize(); }
	CString GetLOGLine(int n);
	
public:
	void CheckThread(CServerDataFolderSetIndex& ThreadIndex );
	LONGLONG GetThreadTime() { return m_nThreadTime; }

private:
	CLoyLogThreadInfo m_arrayThreadInfo[PLU_DBNO_COUNT + 1][EPOSREQPATH_COUNT + 1];
	CStringArray m_LOGBuffer[PLU_DBNO_COUNT + 1][EPOSREQPATH_COUNT + 1];
	CStringArray m_EmptyFileBuffer[PLU_DBNO_COUNT + 1][EPOSREQPATH_COUNT + 1];
	CWinThread* m_pThread[PLU_DBNO_COUNT + 1][EPOSREQPATH_COUNT + 1];

private:
	CServerDataFolderSetIndex m_ThreadIndex;
	CStringArray m_arrayLOG;
	CString m_strLOGFilepath;
	CString m_strLOGFilename;
	CString m_strRSPFilepath;
	CString m_strRSPFilepathTemp;
	LONGLONG m_nThreadTime;

private:
	CEmptyLogFileCache m_EmptyLogFileCache;
};

/**********************************************************************/
