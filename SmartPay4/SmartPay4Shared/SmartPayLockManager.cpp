//*********************************************************************
#include "MessageLogger.h"
#include "Prompter.h"
#include "ServerData.h"
#include "SQLConnectionPool.h"
#include "SQLHelpers.h"
#include "WaitDlg.h"
//*********************************************************************
#include "SmartPayLockManager.h"
//*********************************************************************
int CSmartPayLockManager::m_nLastError = LOCKMANAGER_OK;
long long CSmartPayLockManager::m_nLastErrorTime = 0;
//*********************************************************************

CSmartPayLockManager::CSmartPayLockManager()
{
	m_pDb = SQLConnectionPool.GetPooledConnection(m_nConnection);
}

//*********************************************************************

CSmartPayLockManager::~CSmartPayLockManager()
{
	if (m_nConnection >= 0)
	{
		SQLConnectionPool.DeleteConnection(m_nConnection);
	}
}

//*********************************************************************

int CSmartPayLockManager::AcquireLocks(CSmartPayLockRequest& LockRequest, CWnd* pParentWnd)
{
	m_LockRequest.m_bWantLockServerInner |= LockRequest.m_bWantLockServerInner;
	m_LockRequest.m_bWantLockServerOuter |= LockRequest.m_bWantLockServerOuter;
	m_LockRequest.m_bWantLockBgnd |= LockRequest.m_bWantLockBgnd;
	m_LockRequest.m_bWantLockPebble |= LockRequest.m_bWantLockPebble;
	m_LockRequest.m_bOneShot = LockRequest.m_bOneShot;

	if ((FALSE == m_LockRequest.m_bWantLockServerInner) && 
		(FALSE == m_LockRequest.m_bWantLockServerOuter) && 
		(FALSE == m_LockRequest.m_bWantLockBgnd) && 
		(FALSE == m_LockRequest.m_bWantLockPebble))
	{
		return LOCKMANAGER_ERROR_NOLOCKS;
	}

	if (m_nConnection < 0)
	{
		return LOCKMANAGER_ERROR_NOCONNECT;
	}

	if ( (TRUE == m_LockRequest.m_bWantLockServerInner) && ( FALSE == m_LockRequest.m_bGotLockServerInner) )
	{
		int nResult = AcquireSingleLock("ServerInnerLock", "SmartPay Server", 0, pParentWnd);

		if (LOCKMANAGER_OK != nResult)
		{
			return nResult;
		}

		m_LockRequest.m_bGotLockServerInner = TRUE;
	}

	if ((TRUE == m_LockRequest.m_bWantLockServerOuter) && (FALSE == m_LockRequest.m_bGotLockServerOuter))
	{
		int nResult = AcquireSingleLock("ServerOuterLock", "SmartPay Server", 5, pParentWnd);

		if (LOCKMANAGER_OK != nResult)
		{
			return nResult;
		}

		m_LockRequest.m_bGotLockServerOuter = TRUE;
	}

	if ((TRUE == m_LockRequest.m_bWantLockBgnd) && (FALSE == m_LockRequest.m_bGotLockBgnd))
	{
		int nResult = AcquireSingleLock("BgndLock", "Background Processor", 10, pParentWnd);

		if (LOCKMANAGER_OK != nResult)
		{
			return nResult;
		}

		m_LockRequest.m_bGotLockBgnd = TRUE;
	}

	if ((TRUE == m_LockRequest.m_bWantLockPebble) && (FALSE == m_LockRequest.m_bGotLockPebble))
	{
		int nResult = AcquireSingleLock("PebbleLock1", "Pebble Member Import", 15, pParentWnd);

		if (LOCKMANAGER_OK != nResult)
		{
			return nResult;
		}

		m_LockRequest.m_bGotLockPebble = TRUE;
	}
	
	return LOCKMANAGER_OK; 
}

//*********************************************************************

int CSmartPayLockManager::AcquireSingleLock(CString strLockName, CString strPromptText, int nErrorOffset, CWnd* pParentWnd)
{
	int nResult = AcquireLockSQL(strLockName);

	if ( (LOCKMANAGER_OK != nResult) && ( FALSE == m_LockRequest.m_bOneShot ) )
	{
		CString strWaitText = "";
		strWaitText += "Pausing ";
		strWaitText += strPromptText;

		CWaitDlg dlgWait(pParentWnd);
		dlgWait.SetMessageText(strWaitText);

		bool bKeepTrying = TRUE;

		while ((LOCKMANAGER_OK != nResult) && (TRUE == bKeepTrying))
		{
			int nRetries = 0;
			int nMaxRetries = Server.GetLockRetries();
			while ((LOCKMANAGER_OK != nResult) && (nRetries < nMaxRetries) && ( nRetries < 10 ) )
			{
				Sleep(100);
				nResult = AcquireLockSQL(strLockName);
				nRetries++;
			}

			if (LOCKMANAGER_OK != nResult)
			{
				CString strPauseText = "";
				strPauseText.Format("Unable to pause %s : keep trying", (const char*)strPromptText);
				bKeepTrying = (Prompter.YesNo(strPauseText, "", TRUE, FALSE) == IDYES);
			}
		}
	}

	switch (nResult)
	{
	case 0: // Lock acquired
	case 1: // Lock already held
		return 0;
		
	case -1: // Timeout
		return LOCKMANAGER_ERROR_INNERLOCK_TIMEOUT + nErrorOffset;

	case -2: // Cancelled
		return LOCKMANAGER_ERROR_INNERLOCK_CANCELLED + nErrorOffset;

	case -3: // Deadlock
		return LOCKMANAGER_ERROR_INNERLOCK_DEADLOCK + nErrorOffset;

	case 500: // SQL error
		return LOCKMANAGER_ERROR_INNERLOCK_SQL + nErrorOffset;

	case -999: // Parameter error
	default:
		return LOCKMANAGER_ERROR_INNERLOCK_UNKNOWN + nErrorOffset;
	}
}

//*********************************************************************

int CSmartPayLockManager::AcquireLockSQL(CString strLockName)
{
	LONG nReturnValue = -999;
	HSTMT hStmt = SQL_NULL_HSTMT;

	try
	{
		RETCODE retcode;
		CString strSQL = "{? = CALL sp_getapplock(?, 'Exclusive', 'Session', 500)}";

		// Allocate statement handle
		retcode = ::SQLAllocStmt(m_pDb->m_hdbc, &hStmt);
		if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
		{
			return 500;
		}

		// Bind the return value parameter
		retcode = ::SQLBindParameter(hStmt, 1, SQL_PARAM_OUTPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &nReturnValue, 0, NULL);
		if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
		{
			::SQLFreeStmt(hStmt, SQL_DROP);
			return 500;
		}

		// Bind the resource name parameter
		retcode = ::SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_TCHAR, SQL_VARCHAR, strLockName.GetLength(), 0, (SQLPOINTER)(LPCTSTR)strLockName, 0, NULL);
		if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
		{
			::SQLFreeStmt(hStmt, SQL_DROP);
			return 500;
		}

		// Execute the statement
		retcode = ::SQLExecDirect(hStmt, (SQLTCHAR*)(LPCTSTR)strSQL, SQL_NTS);
		if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
		{
			::SQLFreeStmt(hStmt, SQL_DROP);
			return 500;
		}

		::SQLFreeStmt(hStmt, SQL_DROP);
	}
	catch (...)
	{
		if (hStmt != SQL_NULL_HSTMT)
		{
			::SQLFreeStmt(hStmt, SQL_DROP);
		}

		return 500;
	}

	return nReturnValue;
}

//*********************************************************************

void CSmartPayLockManager::LogError(int nError)
{
	LONGLONG dwTimeNow = GetTickCount64();

	bool bShowError = FALSE;

	if ((m_nLastError == 0) && (nError != 0))
	{
		bShowError = TRUE;
	}
	else if ((m_nLastError != 0) && (nError == 0))
	{
		bShowError = TRUE;
	}
	else if (nError != 0 ) 
	{
		if ((nError != m_nLastError) || (dwTimeNow - m_nLastErrorTime > 30000)) 
		{
			bShowError = TRUE;
		}
	}

	if (FALSE == bShowError)
	{
		return;
	}

	m_nLastError = nError;
	m_nLastErrorTime = dwTimeNow;

	if (0 == nError)
	{
		MessageLogger.LogSystemMessage("Locks acquired successfully");
		return;
	}

	MessageLogger.LogSystemMessage("Unable to acquire locks");

	CString strError = "==> ";
	
	switch (nError)
	{
	case LOCKMANAGER_ERROR_INNERLOCK_SQL:
	case LOCKMANAGER_ERROR_INNERLOCK_TIMEOUT:
	case LOCKMANAGER_ERROR_INNERLOCK_CANCELLED:
	case LOCKMANAGER_ERROR_INNERLOCK_DEADLOCK:
	case LOCKMANAGER_ERROR_INNERLOCK_UNKNOWN:
	case LOCKMANAGER_ERROR_OUTERLOCK_SQL:
	case LOCKMANAGER_ERROR_OUTERLOCK_TIMEOUT:
	case LOCKMANAGER_ERROR_OUTERLOCK_CANCELLED:
	case LOCKMANAGER_ERROR_OUTERLOCK_DEADLOCK:
	case LOCKMANAGER_ERROR_OUTERLOCK_UNKNOWN:
		strError += "Server ";
		break;

	case LOCKMANAGER_ERROR_BGNDLOCK_SQL:
	case LOCKMANAGER_ERROR_BGNDLOCK_TIMEOUT:
	case LOCKMANAGER_ERROR_BGNDLOCK_CANCELLED:
	case LOCKMANAGER_ERROR_BGNDLOCK_DEADLOCK:
	case LOCKMANAGER_ERROR_BGNDLOCK_UNKNOWN:	
		strError += "Bgnd ";
		break;

	case LOCKMANAGER_ERROR_PEBBLE1_SQL:
	case LOCKMANAGER_ERROR_PEBBLE1_TIMEOUT:
	case LOCKMANAGER_ERROR_PEBBLE1_CANCELLED:
	case LOCKMANAGER_ERROR_PEBBLE1_DEADLOCK:
	case LOCKMANAGER_ERROR_PEBBLE1_UNKNOWN:
		strError += "Pebble ";
		break;
	}
	
	switch (nError)
	{
	case LOCKMANAGER_ERROR_NOLOCKS:
		strError += "No locks requested";
		break;

	case LOCKMANAGER_ERROR_NOCONNECT:
		strError += "No database connection";
		break;

	case LOCKMANAGER_ERROR_INNERLOCK_SQL:
		strError += "Inner lock SQL error";
		break;

	case LOCKMANAGER_ERROR_INNERLOCK_TIMEOUT:
		strError += "Inner lock timeout";
		break;

	case LOCKMANAGER_ERROR_INNERLOCK_CANCELLED:
		strError += "Inner lock cancelled";
		break;

	case LOCKMANAGER_ERROR_INNERLOCK_DEADLOCK:
		strError += "Inner lock deadlock";
		break;

	case LOCKMANAGER_ERROR_INNERLOCK_UNKNOWN:
		strError += "Unknown inner lock error";
		break;

	case LOCKMANAGER_ERROR_OUTERLOCK_SQL:
		strError += "Outer lock SQL error";
		break;

	case LOCKMANAGER_ERROR_OUTERLOCK_TIMEOUT:
		strError += "Outer lock timeout";
		break;

	case LOCKMANAGER_ERROR_OUTERLOCK_CANCELLED:
		strError += "Outer lock cancelled";
		break;

	case LOCKMANAGER_ERROR_OUTERLOCK_DEADLOCK:
		strError += "Outer lock deadlock";
		break;

	case LOCKMANAGER_ERROR_OUTERLOCK_UNKNOWN:
		strError += "Unknown outer lock error";
		break;

	case LOCKMANAGER_ERROR_BGNDLOCK_SQL:
	case LOCKMANAGER_ERROR_PEBBLE1_SQL:
		strError += "lock SQL error";
		break;

	case LOCKMANAGER_ERROR_BGNDLOCK_TIMEOUT:
	case LOCKMANAGER_ERROR_PEBBLE1_TIMEOUT:
		strError += "lock timeout";
		break;

	case LOCKMANAGER_ERROR_BGNDLOCK_CANCELLED:
	case LOCKMANAGER_ERROR_PEBBLE1_CANCELLED:
		strError += "lock cancelled";
		break;

	case LOCKMANAGER_ERROR_BGNDLOCK_DEADLOCK:
	case LOCKMANAGER_ERROR_PEBBLE1_DEADLOCK:
		strError += "lock deadlock";
		break;

	case LOCKMANAGER_ERROR_BGNDLOCK_UNKNOWN:
	case LOCKMANAGER_ERROR_PEBBLE1_UNKNOWN:
		strError += "Unknown lock error";
		break;

	default:
		strError += "Unknown error code: ";
		strError.AppendFormat("%d", nError);
		break;
	}
	
	MessageLogger.LogSystemMessage(strError);
}

//*********************************************************************
