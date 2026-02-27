#pragma once
/**********************************************************************/
#include "Defines.h"
/**********************************************************************/

struct CStatusCheckThreadInfo
{
public:
	CStatusCheckThreadInfo();

public:
	CString m_strImportFile = "";
	CString m_strStatusReply = "";
	CString m_strLoyReqFilename = "";
	CString m_strLoyReqTempFilename = "";
	CString m_strLoyReqReplyFilename = "";
	int m_nStatusErrorNo = 0;
	int m_nTerminalNo = 0;
	int m_nAction = nWAITSTATUS_ACTION_CHECKSERVERONLINE;
	bool m_bBoolResult = FALSE;
	int m_nIntResult = 0;
};

/**********************************************************************/

class CStatusCheckThread
{
public:
	static UINT DoWork( LPVOID pParam );

private:
	static void DoWorkInternal(CStatusCheckThreadInfo* pInfo );
	static bool Send(CStatusCheckThreadInfo* pInfo, int nStatus);
	static void CheckServerOnline(CStatusCheckThreadInfo* pInfo);
};

/**********************************************************************/
