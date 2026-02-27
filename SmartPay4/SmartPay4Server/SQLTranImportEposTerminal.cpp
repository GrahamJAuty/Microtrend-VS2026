/**********************************************************************/
#include "..\SmartPay4Shared\FileRemover.h"
#include "..\SmartPay4Shared\ServerData.h"
/**********************************************************************/
#include "..\SmartPay4Shared\SQLTable_EposTerminal\SQLRepositoryEposTerminal.h"
/**********************************************************************/
#include "SQLTranImportEposTerminal.h"
/**********************************************************************/

void CSQLTranImportEposTerminal::DoWork()
{
	if (SQLTRAN_STATE_ACTIVE == m_nState)
	{
		m_bCanCommit = FALSE;
		m_nState = SQLTRAN_STATE_WAITEND;

		bool bDeleteFile = FALSE;
		CSQLRepositoryEposTerminal RepoTerm;
		RepoTerm.ImportTerminalInfo(bDeleteFile, m_bCanCommit, m_pDatabase);

		if (TRUE == bDeleteFile)
		{
			CFileRemover FileRemover(Server.GetEcrManagerImportEposTerminalFilename());
		}
	}
}

/**********************************************************************/
