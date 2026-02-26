#pragma once
/**********************************************************************/

struct CLoyLogThreadInfo
{
public:
	CLoyLogThreadInfo();

public:
	CString m_strLOGFolder;
	CString m_strLOGFilepath;
	CString m_strLOGFilename;
	CString m_strRSPFilepath; 
	CString m_strRSPFilepathTemp;
	CStringArray* m_pLOGBuffer;
	CStringArray* m_pEmptyFileBuffer;
	bool m_bCopyLogFile;
	LONGLONG m_StartTime;
	LONGLONG m_EndTime;
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
