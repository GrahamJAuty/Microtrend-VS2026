#pragma once
/**********************************************************************/
#include "..\SPosLoyalty4Shared\ServerData.h"
/**********************************************************************/
#include "LoyReqThread.h"
/**********************************************************************/

class CLoyReqThreadManager
{
public:
	CLoyReqThreadManager();
	~CLoyReqThreadManager();

public:
	void ShutdownThreads();

public:
	CServerDataFolderSetIndex GetThreadIndex() { return m_ThreadIndex; }
	CString GetREQFilepath() { return m_strREQFilename; }
	CString GetREQFilename() { return m_strREQFilename; }
	CString GetRSPFilepath() { return m_strRSPFilepath; }
	CString GetRSPFilepathTemp() { return m_strRSPFilepathTemp; }
	bool IsReconRequest() { return m_bIsReconRequest; }
	bool IsJSONRequest() { return m_bIsJSONRequest; }
	int GetRequestLineCount() { return m_arrayRequest.GetSize(); }
	CString GetRequestLine(int n);

public:
	void ClearResponse() { m_arrayResponse.RemoveAll(); }
	int GetResponseLineCount() { return m_arrayResponse.GetSize(); }
	void AddResponseLine(CString str) { m_arrayResponse.Add(str); }
	void SetResponseLine(int n, CString str);
	CString GetResponseLine(int n);

public:
	void CheckThread(CServerDataFolderSetIndex& ThreadIndex);
	LONGLONG GetThreadTime() { return m_nThreadTime; }

private:
	CLoyReqThreadInfo m_arrayThreadInfo[PLU_DBNO_COUNT + 1][EPOSREQPATH_COUNT + 1];
	CStringArray m_RequestBuffer[PLU_DBNO_COUNT + 1][EPOSREQPATH_COUNT + 1];
	CWinThread* m_pThread[PLU_DBNO_COUNT + 1][EPOSREQPATH_COUNT + 1];

private:
	CServerDataFolderSetIndex m_ThreadIndex;
	CStringArray m_arrayRequest;
	CStringArray m_arrayResponse;
	CString m_strREQFilepath;
	CString m_strREQFilename;
	CString m_strRSPFilepath;
	CString m_strRSPFilepathTemp;
	bool m_bIsReconRequest;
	bool m_bIsJSONRequest;
	LONGLONG m_nThreadTime;
};

/**********************************************************************/
