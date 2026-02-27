//*********************************************************************
#include "..\SmartPay4Shared\GlobalState.h"
#include "..\SmartPay4Shared\SQLConnectionOptions.h"
#include "..\SmartPay4Shared\SQLDb.h"
//*********************************************************************
#include "ServerRegistration.h"
//*********************************************************************
extern const char* szVERSION_SMARTPAY_ABOUT;
extern const char* szUPDATE_TOKEN_DATE;
//*********************************************************************

CServerRegistration::CServerRegistration()
{
	m_bIsRegistered = FALSE;
	m_bDoneChangeDir = FALSE;
	m_strDatapath = "";
	m_strError1 = "";
	m_strError2 = "";
	m_nRetryTime = 0;
	m_nFirstTryTime = 0;
	m_nAttemptCount1 = 0;
	m_nAttemptCount2 = 0;
}

//*******************************************************************

void CServerRegistration::CheckRegistration()
{
	if (FALSE == m_bIsRegistered)
	{
		ULONGLONG timeNow = GetTickCount64();

		if ((timeNow > m_nRetryTime + 2000) || (timeNow < m_nRetryTime) || (0 == m_nRetryTime))
		{
			if (0 == m_nFirstTryTime)
			{
				m_nFirstTryTime = timeNow;
			}

			m_nRetryTime = timeNow;
			m_nAttemptCount1++;

			if (FALSE == m_bDoneChangeDir)
			{
				if (_chdir(m_strDatapath) != 0)
				{
					m_strError1 = "Unable to access specified directory";
					return;
				}
				else
				{
					m_bDoneChangeDir = TRUE;
				}
			}

			bool bCreatePCId = FALSE;
			m_nAttemptCount2++;
			m_bIsRegistered = Sysset.TestRegistrationModule(SYSSET_SMARTPAY4, NULL, bCreatePCId, TRUE, 0);

			if (FALSE == m_bIsRegistered)
			{
				m_strError1 = "Unable to find registered installation of SmartPay Manager in specified directory";
			}
			else
			{
				System.SetGotPCIdFlag(FALSE);
			}
		}
	}
}

//*******************************************************************

bool CServerRegistration::CheckDbAccess()
{
	SQLConnectionOptions.Read();
	g_GlobalState.DetermineODBCDriverVersion(1);

	int nError = 0;
	CSQLDb SQLDb;
	SQLDb.Connect(nError, FALSE);

	if (nError != 0)
	{
		m_strError1 = "Unable to access SmartPay database";
		m_strError2.Format("==> Error code (%d:%d)",
			SQLSRVR_ERRCONTEXT_CONNECT,
			nError);

		return FALSE;
	}

	SQLDb.GetDatabaseVersion(nError);
	if (nError != 0)
	{
		m_strError1 = "Incompatible SmartPay database version";
		m_strError2.Format("==> Error code (%d:%d)",
			SQLSRVR_ERRCONTEXT_CHECKVERSION,
			nError);

		return FALSE;
	}

	return TRUE;
}

//*******************************************************************

bool CServerRegistration::CanGiveUp()
{
	ULONGLONG timeNow = GetTickCount64();

	if (timeNow < m_nFirstTryTime)
	{
		m_nFirstTryTime = timeNow;
	}

	return (timeNow > m_nFirstTryTime + (120000));
}

//*******************************************************************
