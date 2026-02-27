#pragma once
/**********************************************************************/
#include "Defines.h"
#include "UserIDRecord.h"
/**********************************************************************/

struct CGetUserThreadInfo
{
public:
	CGetUserThreadInfo();

public:
	CString m_strFilename = "";
	CUserIDRecord m_UserIDRecord;
	CString m_strLoyReqFilename = "";
	CString m_strLoyReqTempFilename = "";
	CString m_strLoyReqReplyFilename = "";
	CStringArray m_arrayUserLines;
	int m_nMessageNo = nMESSAGE_NONE;
	bool m_bBoolResult = FALSE;
	bool m_bServerValidate = FALSE;
};

/**********************************************************************/

class CGetUserThread
{
public:
	static UINT DoWork( LPVOID pParam );

private:
	static void DoWorkInternal(CGetUserThreadInfo* pInfo );
	static bool Extract(CGetUserThreadInfo* pInfo);

private:
	static bool AddToLoyReqFile(CGetUserThreadInfo* pInfo, CString strPrefix, CString strUserID);
	static bool SendLoyReqFile(CGetUserThreadInfo* pInfo);
	static bool WaitReply(CGetUserThreadInfo* pInfo);
};

/**********************************************************************/
