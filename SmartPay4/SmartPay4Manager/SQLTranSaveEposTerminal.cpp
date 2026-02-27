/**********************************************************************/
#include "..\SmartPay4Shared\SQLHelpers.h"
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
#include "..\SmartPay4Shared\SQLResultInfo.h"
#include "..\SmartPay4Shared\SQLTable_EposTerminal\SQLRepositoryEposTerminal.h"
/**********************************************************************/
#include "SQLTranSaveEposTerminal.h"
/**********************************************************************/

void CSQLTranSaveEposTerminal::DoWork(CSQLRowSetEposTerminal& RowSetEP, CArray<int, int>& arrayDeletedEP)
{
	if (SQLTRAN_STATE_ACTIVE == m_nState)
	{
		m_bCanCommit = FALSE;
		m_nState = SQLTRAN_STATE_WAITEND;

		for (int n = 0; n < arrayDeletedEP.GetSize(); n++)
		{
			CSQLRowEposTerminal RowEP;
			RowEP.SetTerminalNo(arrayDeletedEP[n]);

			CSQLRepositoryEposTerminal RepoTerm;
			switch (RepoTerm.DeleteRow(RowEP, m_pDatabase).GetSQLError())
			{
			case SQLCRUD_ERR_NONE:
			case SQLCRUD_ERR_NOMATCH:
				break;

			default:
				return;
			}	
		}

		for (int n = 0; n < RowSetEP.GetSize(); n++)
		{
			CSQLRowEposTerminal RowEP;
			RowSetEP.GetRow(n, RowEP);
			bool dDisableFlag = RowEP.GetDisableFlag();	

			CSQLRepositoryEposTerminal RepoTerm;
			if ( RepoTerm.SelectRow(RowEP,m_pDatabase).GetSQLError() == SQLCRUD_ERR_NONE)
			{
				RowEP.SetDisableFlag(dDisableFlag);

				if (RepoTerm.UpdateRow(RowEP, m_pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
				{
					return;
				}
			}
		}

		m_bCanCommit = TRUE;
	}
}

/**********************************************************************/
