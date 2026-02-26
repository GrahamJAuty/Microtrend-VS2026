/**********************************************************************/
#include "CommsHandshaker.h"
#include "FileRemover.h"
#include "SysInfo.h"
/**********************************************************************/
#include "LockManager.h"
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/

CLockManagerInfo::CLockManagerInfo()
{
	m_bLockAllUsers = FALSE;
	m_strLockAllUsersPrompt = "";
	m_nCommsAction = COMMS_ACTION_NONE;
	m_bStopComms = FALSE;
	m_bStopCommsPMSExemption = FALSE;
	m_strStopCommsPrompt = "";
	m_bStopPosTray = FALSE;
	m_bStopLoyalty = FALSE;
	m_bLoyaltyWasRunning = FALSE;
	m_bSmartPayWasRunning = FALSE;
	m_bSmartBackWasRunning = FALSE;
	m_pParentWnd = NULL;
}

/**********************************************************************/

CLockManager::CLockManager( CLockManagerInfo& infoLock ) : m_infoLock( infoLock ), 
	m_WaitLoyaltyDlg( m_infoLock.m_strStopCommsPrompt, m_infoLock.m_pParentWnd ),
	m_WaitPosTrayDlg( m_infoLock.m_strStopCommsPrompt, m_infoLock.m_pParentWnd ),
	m_WaitSmartPayServerDlg( m_infoLock.m_strStopCommsPrompt, m_infoLock.m_pParentWnd ),
	m_WaitSmartPayBgndDlg( m_infoLock.m_strStopCommsPrompt, m_infoLock.m_pParentWnd ),
	m_WaitSmartPayServiceDlg( m_SmartPaySemaphore, m_infoLock.m_strStopCommsPrompt, m_infoLock.m_pParentWnd ),
	m_WaitLoyaltyServiceDlg( m_LoyaltySemaphore, m_infoLock.m_strStopCommsPrompt, m_infoLock.m_pParentWnd )
{
	m_nLockResult = LOCK_RESULT_INACTIVE;
	
	{
		CSmartPayLockRequest Request;
		Request.m_bLockServerOuter = TRUE;
		Request.m_bLockBackground = TRUE;
		m_SmartPaySemaphore.SetRequest( Request );
	}

	{
		CLoyaltyLockRequest Request;
		Request.m_bLockServerOuter = TRUE;
		m_LoyaltySemaphore.SetRequest( Request );
	}
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
			if ( DataManager.LockAllUsers() == FALSE )
			{
				if ( m_infoLock.m_strLockAllUsersPrompt != "" )
					Prompter.LockError( m_infoLock.m_strLockAllUsersPrompt );

				bLockFailed = TRUE;
				m_nLockResult = LOCK_RESULT_FAIL_USERS;
			}
		}

		//COMMS HANDSHAKER
		if ( FALSE == bLockFailed )
		{
			switch( m_infoLock.m_nCommsAction )
			{
			case COMMS_ACTION_DISABLE:	
				CommsHandshaker.DisableComms();		
				break;

			case COMMS_ACTION_PAUSE:	
				CommsHandshaker.PauseComms();		
				break;
			}
		}

		//POSTRAY
		if ( ( FALSE == bLockFailed ) && ( SysInfo.IsPosTraySystem() == TRUE ) )
		{
			if ( TRUE == m_infoLock.m_bStopPosTray )
			{
				if ( m_WaitPosTrayDlg.LockPosTray() == FALSE )
				{
					UnlockComms();

					if ( TRUE == m_infoLock.m_bLockAllUsers )
						DataManager.UnlockAllUsers();

					bLockFailed = TRUE;
					m_nLockResult = LOCK_RESULT_FAIL_POSTRAY;
				}
			}
		}

		//LOYALTY SERVER
		if ( FALSE == bLockFailed )
		{
			if ( TRUE == m_infoLock.m_bStopLoyalty )
			{
				switch( SysInfo.GetLoyaltyOrSmartPayType() )
				{
				case LOYALTY_TYPE_LOY_V2:

					if ( m_WaitLoyaltyDlg.LockLoyalty() == FALSE )
					{
						LoyaltyFail( bLockFailed );
					}
					m_infoLock.m_bLoyaltyWasRunning = m_WaitLoyaltyDlg.WasRunning();

					break;

				case LOYALTY_TYPE_LOY_V3:
					
					if ( m_WaitLoyaltyServiceDlg.LockLoyalty() == FALSE )
					{
						LoyaltyFail( bLockFailed );
					}
					
					break;

				case LOYALTY_TYPE_SMP_V1:

					if ( m_WaitSmartPayServerDlg.LockSmartPay() == FALSE )
					{
						LoyaltyFail( bLockFailed );
					}
					m_infoLock.m_bSmartPayWasRunning = m_WaitSmartPayServerDlg.WasRunning();
					
					if ( FALSE == bLockFailed )
					{
						if ( m_WaitSmartPayBgndDlg.LockSmartBack() == FALSE )
						{
							LoyaltyFail( bLockFailed );
						}
						m_infoLock.m_bSmartBackWasRunning = m_WaitSmartPayBgndDlg.WasRunning();
					}

					break;

				case LOYALTY_TYPE_SMP_V2:

					if ( m_WaitSmartPayServiceDlg.LockSmartPay() == FALSE )
					{
						LoyaltyFail( bLockFailed );
					}
					
					break;
				}
			}
		}

		if ( FALSE == bLockFailed )
			m_nLockResult = LOCK_RESULT_OK;
	}

	return ( m_nLockResult == LOCK_RESULT_OK );
}

/**********************************************************************/

void CLockManager::LoyaltyFail( bool& bFlag )
{
	UnlockComms();
							
	if ( TRUE == m_infoLock.m_bLockAllUsers )
	{
		DataManager.UnlockAllUsers();
	}

	bFlag = TRUE;
	m_nLockResult = LOCK_RESULT_FAIL_LOYALTY;
}

/**********************************************************************/

void CLockManager::ReleaseLock()
{
	if ( LOCK_RESULT_OK == m_nLockResult )
	{
		UnlockComms();

		if ( TRUE == m_infoLock.m_bLockAllUsers )
			DataManager.UnlockAllUsers();

		m_nLockResult = LOCK_RESULT_RELEASED;
	}
}

/**********************************************************************/

void CLockManager::UnlockComms()
{
	switch( m_infoLock.m_nCommsAction )
	{
	case COMMS_ACTION_DISABLE:	
		CommsHandshaker.EnableComms();		
		break;

	case COMMS_ACTION_PAUSE:	
		CommsHandshaker.BeginComms();
		CommsHandshaker.ResumeComms();
		break;
	}
}

/**********************************************************************/

CLockManager::~CLockManager(void)
{
	ReleaseLock();
}

/**********************************************************************/
#else
/**********************************************************************/

bool CLockManager::GetLock()
{
	if ( LOCK_RESULT_INACTIVE == m_nLockResult )
		m_nLockResult = LOCK_RESULT_OK;
	
	return ( m_nLockResult == LOCK_RESULT_OK );
}

/**********************************************************************/

void CLockManager::ReleaseLock()
{
	if ( LOCK_RESULT_OK == m_nLockResult )
		m_nLockResult = LOCK_RESULT_RELEASED;
}

/**********************************************************************/
#endif
/**********************************************************************/

