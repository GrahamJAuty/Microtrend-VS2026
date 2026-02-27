#pragma once
/**********************************************************************/

struct CLoyReqThreadInfo
{
public:
	CLoyReqThreadInfo();

public:
	CString m_strREQFolder = "";
	CString m_strREQFilepath = "";
	CString m_strREQFilename = "";
	CString m_strRSPFilepath = "";
	CString m_strRSPFilepathTemp = "";
	CStringArray* m_pRequestBuffer = nullptr;
	bool m_bIsReconRequest = FALSE;
	bool m_bIsJSONRequest = FALSE;
	LONGLONG m_StartTime = 0;
	LONGLONG m_EndTime = 0;
	int m_nDbNo = 0;
	int m_nSetNo = 0;
	bool m_bLogFileReads = 0;
};

/**********************************************************************/

class CLoyReqThread
{
public:
	static UINT DoWork( LPVOID pParam );

private:
	static void DoWorkInternal( CLoyReqThreadInfo* pInfo );
	static void LogThread(CLoyReqThreadInfo* pInfo, CString strMsg);
};

/**********************************************************************/
