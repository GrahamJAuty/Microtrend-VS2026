#pragma once
/**********************************************************************/

struct CLoyLogThreadInfo
{
public:
	CLoyLogThreadInfo();

public:
	CString m_strLOGFolder = "";
	CString m_strLOGFilepath = "";
	CString m_strLOGFilename = "";
	CString m_strRSPFilepath = "";
	CString m_strRSPFilepathTemp = "";
	CStringArray* m_pLOGBuffer = nullptr;
	CStringArray* m_pEmptyFileBuffer = nullptr;
	bool m_bCopyLogFile = FALSE;
	LONGLONG m_StartTime = 0;
	LONGLONG m_EndTime = 0;
};

/**********************************************************************/

class CLoyLogThread
{
public:
	static UINT DoWork( LPVOID pParam );

private:
	static void DoWorkInternal( CLoyLogThreadInfo* pInfo );
};

/**********************************************************************/
