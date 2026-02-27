/**********************************************************************/
#include "..\SmartPay4Shared\FileRemover.h"
//#include "SysInfo.h"
/**********************************************************************/
#include "LockManager.h"
/**********************************************************************/

CLockManagerInfo::CLockManagerInfo()
{
	m_bLockAllUsers = FALSE;
	m_strLockAllUsersPrompt = "";
	m_strStopCommsPrompt = "";
	m_bStopSmartPay = FALSE;
	m_pParentWnd = NULL;
}

/**********************************************************************/

CLockManager::CLockManager( CLockManagerInfo& infoLock ) : m_infoLock( infoLock ), 
	m_WaitSmartPayServiceDlg( m_infoLock.m_strStopCommsPrompt, m_SmartPayLockManager, m_infoLock.m_pParentWnd )
{
	m_nLockResult = LOCK_RESULT_INACTIVE;
}

/**********************************************************************/

bool CLockManager::GetLock()
{
	if ( LOCK_RESULT_INACTIVE == m_nLockResult )
	{
		bool bLockFailed = FALSE;

		//LOCK ALL USERS
		if ( TRUE == m_infoLock.m_bLockAllUsers )
		{
			if ( System.LockAllUsers() == FALSE )
			{
				if (m_infoLock.m_strLockAllUsersPrompt != "")
				{
					Prompter.LockError(m_infoLock.m_strLockAllUsersPrompt);
				}

				bLockFailed = TRUE;
				m_nLockResult = LOCK_RESULT_FAIL_USERS;
			}
		}

		//SMARTPAY SERVER
		if (FALSE == bLockFailed)
		{
			if (TRUE == m_infoLock.m_bStopSmartPay)
			{
				if (m_WaitSmartPayServiceDlg.LockSmartPay() == FALSE)
				{
					SmartPayFail(bLockFailed);
				}
			}
		}

		if (FALSE == bLockFailed)
		{
			m_nLockResult = LOCK_RESULT_OK;
		}
	}

	return ( m_nLockResult == LOCK_RESULT_OK );
}

/**********************************************************************/

void CLockManager::SmartPayFail( bool& bFlag )
{						
	if ( TRUE == m_infoLock.m_bLockAllUsers )
	{
		System.UnlockAllUsers();
	}

	bFlag = TRUE;
	m_nLockResult = LOCK_RESULT_FAIL_SMARTPAY;
}

/**********************************************************************/

void CLockManager::ReleaseLock()
{
	if ( LOCK_RESULT_OK == m_nLockResult )
	{
		if (TRUE == m_infoLock.m_bLockAllUsers)
		{
			System.UnlockAllUsers();
		}

		m_nLockResult = LOCK_RESULT_RELEASED;
	}
}

/**********************************************************************/

CLockManager::~CLockManager(void)
{
	ReleaseLock();
}

/**********************************************************************/
