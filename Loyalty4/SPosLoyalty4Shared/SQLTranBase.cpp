/**********************************************************************/
#ifndef LOYSERVER
/**********************************************************************/
#include "..\SPosLoyalty4Manager\Prompter.h"
#include "..\SPosLoyalty4Manager\WorkingDlg.h"
/**********************************************************************/
#endif
/**********************************************************************/
#include "LoyaltyAuditAppend.h"
#include "SQLResultInfo.h"
#include "SQLConnectionPool.h"
#include "SQLDefines.h"
#include "SQLTable_Account\SQLRepositoryAccount.h"
#include "SQLTable_ExternalAccount\SQLRepositoryExternalAccount.h"
/**********************************************************************/
#include "SQLTranBase.h"
/**********************************************************************/

CSQLTranBase::CSQLTranBase()
{
	m_nState = SQLTRAN_STATE_NOTSTARTED;
	m_nConnection = -1;
	m_pDatabase = NULL;
	m_bCanCommit = FALSE;
	m_strProcessError = "";
}

/**********************************************************************/

CSQLTranBase::~CSQLTranBase()
{
	EndTrans();
}

/**********************************************************************/

int CSQLTranBase::BeginTrans()
{
	if (SQLTRAN_STATE_NOTSTARTED == m_nState)
	{
		m_bCanCommit = FALSE;
		m_arrayAuditBuffer.RemoveAll();

		try
		{
			m_pDatabase = SQLConnectionPool.GetPooledConnection(m_nConnection);

			if (m_nConnection >= 0)
			{
				if (m_pDatabase->BeginTrans() == TRUE)
				{
					m_nState = SQLTRAN_STATE_ACTIVE;
				}
			}
		}
		catch (...)
		{
			if (m_nConnection >= 0)
			{
				SQLConnectionPool.FreeConnection(m_nConnection);
				m_nConnection = -1;
			}
		}
	}

	return m_nState;
}

/**********************************************************************/

int CSQLTranBase::EndTrans()
{
	switch (m_nState)
	{
	case SQLTRAN_STATE_ACTIVE:
	case SQLTRAN_STATE_WAITEND:

		try
		{
			if (TRUE == m_bCanCommit)
			{
				if (m_pDatabase->CommitTrans())
				{
					m_nState = SQLTRAN_STATE_COMMITOK;
				}
				else
				{
					m_nState = SQLTRAN_STATE_COMMITFAIL;
				}
			}
			else
			{
				if (m_pDatabase->Rollback())
				{
					m_nState = SQLTRAN_STATE_ROLLBACKOK;
				}
				else
				{
					m_nState = SQLTRAN_STATE_ROLLBACKFAIL;
				}
			}
		}
		catch (...)
		{
			m_nState = SQLTRAN_STATE_ROLLBACKFAIL;
		}

		break;
	}

	if (m_nConnection >= 0)
	{
		SQLConnectionPool.FreeConnection(m_nConnection);
		m_nConnection = -1;
	}

	return m_nState;
}

/**********************************************************************/

void CSQLTranBase::AfterTransaction()
{
	if (m_arrayAuditBuffer.GetSize() > 0)
	{
		CLoyaltyAuditAppend auditFile;
		auditFile.AppendArray(m_arrayAuditBuffer);
	}
}

/**********************************************************************/

void CSQLTranBase::ShowErrorMessage(CString strAction)
{
#ifndef LOYSERVER

	CString strError = "";
	strError += "Database Transaction Error (";
	strError += strAction;
	strError += ")\n\n";

	switch (m_nState)
	{
	case SQLTRAN_STATE_COMMITFAIL:
		strError += "Unable to commit transaction.";
		break;

	case SQLTRAN_STATE_NOTSTARTED:
		strError += "Unable to create transaction.";
		break;

	default:
		strError += "Unable to process transaction.";

		if (m_strProcessError != "")
		{
			strError += "\n\n";
			strError += m_strProcessError;
		}

		break;
	}

	Prompter.Error(strError);

#endif
}

/**********************************************************************/

