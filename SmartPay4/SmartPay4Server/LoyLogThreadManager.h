#pragma once
/**********************************************************************/
#include "..\SmartPay4Shared\ServerData.h"
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
	int GetCurrentThread() { return m_nCurrentThread; }
	CString GetLOGFilepath() { return m_strLOGFilename; }
	CString GetLOGFilename() { return m_strLOGFilename; }
	CString GetRSPFilepath() { return m_strRSPFilepath; }
	CString GetRSPFilepathTemp() { return m_strRSPFilepathTemp; }
	int GetLOGLineCount() { return m_arrayLOG.GetSize(); }
	CString GetLOGLine(int n);
	
public:
	void CheckThread(int nThread);
	LONGLONG GetThreadTime() { return m_nThreadTime; }

private:
	CLoyLogThreadInfo m_arrayThreadInfo[EPOSREQPATH_COUNT + 1];
	CStringArray m_LOGBuffer[EPOSREQPATH_COUNT + 1];
	CStringArray m_EmptyFileBuffer[EPOSREQPATH_COUNT + 1];
	CWinThread* m_pThread[EPOSREQPATH_COUNT + 1];

private:
	int m_nCurrentThread;
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
