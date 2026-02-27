#pragma once
/**********************************************************************/

struct CPostTransactionThreadInfo	
{
public:
	CPostTransactionThreadInfo();

public:
	int m_nAction = nTRANPOST_ACTION_SENDNORMAL;
	CString m_strOfflineFilename = "";
	CString m_strLoyLogTransFilename = "";
	CString m_strPayLinkTTLFilename = "";
	bool m_bBoolResult = FALSE;
	int m_nIntResult = 0;
};

/**********************************************************************/

class CPostTransactionThread
{
public:
	static UINT DoWork( LPVOID pParam );

private:
	static void DoWorkInternal(CPostTransactionThreadInfo* pInfo );
	static void SendNormal(CPostTransactionThreadInfo* pInfo);
};

/**********************************************************************/
