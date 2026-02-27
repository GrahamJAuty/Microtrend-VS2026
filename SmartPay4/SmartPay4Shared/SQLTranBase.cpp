/**********************************************************************/
#include "Prompter.h"
/**********************************************************************/
#include "SQLTable_Account\SQLRepositoryAccount.h"
#include "SQLTable_ExternalAccount\SQLRepositoryExternalAccount.h"
/**********************************************************************/
#include "SQLResultInfo.h"
#include "SQLConnectionPool.h"
#include "SQLDefines.h"
/**********************************************************************/
#include "SQLTranBase.h"
/**********************************************************************/

CSQLTranBase::CSQLTranBase()
{
	m_nState = SQLTRAN_STATE_NOTSTARTED;
	m_nConnection = -1;
	m_pDatabase = NULL;
	m_bCanCommit = FALSE;
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
		break;
	}

	Prompter.Error(strError);

#endif
}

/**********************************************************************/

void CSQLTranBase::AfterTransaction()
{
	m_atc.WritePendingLines();
}

/**********************************************************************/

void CSQLTranBase::AddAlterTableAddColumnCommand(CStringArray& arrayCommand, CString strTable, CString strField, CString strType, bool bNotNull, CString strDefault)
{
	CString strCommand = "";
	strCommand += "ALTER TABLE ";
	strCommand += strTable;
	strCommand += " ADD ";
	strCommand += strField;
	strCommand += " ";
	strCommand += strType;

	if (FALSE == bNotNull)
	{
		arrayCommand.Add(strCommand);
	}
	else
	{
		strCommand += " NOT NULL CONSTRAINT TEMP_";
		strCommand += strField;
		strCommand += " DEFAULT ";
		strCommand += strDefault;
		arrayCommand.Add(strCommand);

		strCommand = "";
		strCommand += "ALTER TABLE ";
		strCommand += strTable;
		strCommand += " DROP CONSTRAINT TEMP_";
		strCommand += strField;
		arrayCommand.Add(strCommand);
	}
}

/**********************************************************************/

void CSQLTranBase::AddAlterTableNotNullCommand(CStringArray& arrayCommand, CString strTable, CString strField, CString strType, CString strDefault)
{
	CString strCommand = "";
	strCommand += "UPDATE ";
	strCommand += strTable;
	strCommand += " SET ";
	strCommand += strField;
	strCommand += " = ";
	strCommand += strDefault;
	strCommand += " WHERE ";
	strCommand += strField;
	strCommand += " IS NULL";
	arrayCommand.Add(strCommand);

	strCommand = "";
	strCommand += "ALTER TABLE ";
	strCommand += strTable;
	strCommand += " ALTER COLUMN ";
	strCommand += strField;
	strCommand += " ";
	strCommand += strType;
	strCommand += " ";
	strCommand += "NOT NULL";
	arrayCommand.Add(strCommand);
}

/**********************************************************************/
