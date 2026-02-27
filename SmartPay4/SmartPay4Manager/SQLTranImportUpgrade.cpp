/**********************************************************************/
#include "..\SmartPay4Shared\GlobalState.h"
#include "..\SmartPay4Shared\SmartPayHeaderHelpers.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SmartPay4Shared\SQLHelpers.h"
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
#include "..\SmartPay4Shared\WorkingDlg.h"
/**********************************************************************/
#include "SQLTranImportUpgrade.h"
/**********************************************************************/
static const char* szCLEAR_FIELD = "CLEAR";
/**********************************************************************/

CSQLTranImportUpgrade::CSQLTranImportUpgrade() : CSQLTranBase(),
m_ImportHelpers(m_atc, m_ImportCounters)
{
}

/**********************************************************************/

void CSQLTranImportUpgrade::DoWork(int nImportAppNo, CSSFile& ImportFile, CArray<int, int>& arrayHeader, int nIndexUserID)
{
	if (SQLTRAN_STATE_ACTIVE == m_nState)
	{
		m_bCanCommit = FALSE;
		m_nState = SQLTRAN_STATE_WAITEND;

		m_nImportMethod = nIMPORT_METHOD_ALL;
		m_nImportAppNo = nImportAppNo;
		m_nIndexUserID = nIndexUserID;

		m_ImportHelpers.SetDatabase(m_pDatabase);
		m_ImportHelpers.SetImportAppNo(m_nImportAppNo);

		CWorkingDlg dlgWorking("Importing Accounts", FALSE);
		dlgWorking.Create();

		CFileRecycler::SendToRecycleBin(Filenames.GetDBImportExceptionsFilename());

		{
			CString strImportLine = "";
			while (ImportFile.ReadString(strImportLine) == TRUE)
			{
				m_ImportCounters.IncRead();

				dlgWorking.SetCaption2PercentDone(ImportFile.GetPercentPosition(), FALSE);
				
				if (ImportLine(arrayHeader, strImportLine) == FALSE)
				{
					return;
				}
			}
		}

		m_bCanCommit = TRUE;
	}
}

/**********************************************************************/

//RETURN FALSE ONLY IF THERE IS AN ERROR THAT REQUIRES TRANSACTION TO STOP
bool CSQLTranImportUpgrade::ImportLine(CArray<int, int>& arrayHeader, const char* szImportLine)
{
	CCSV csvData(szImportLine);

	//get UserID from csv line
	CString strCardNo = "";
	CString strCsvUserID = csvData.GetString(m_nIndexUserID);
	if (CSQLHelpers::IsValidUserID(strCsvUserID, strCardNo, System.GetUserIDLength(), TRUE) == TRUE)
	{
		CSQLRowAccountFull RowAccount;
		RowAccount.SetUserIDString(strCardNo);
		RowAccount.SetLeaverYear(g_GlobalState.GetLeaverYear());

		CSQLRepositoryAccount RepoAccount;
		if (RepoAccount.SelectRow(RowAccount, m_pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
		{
			if (m_ImportHelpers.ProcessNewRecord(RowAccount, arrayHeader, &csvData) == FALSE)
			{
				return FALSE;
			}
		}
		else
		{
			if (m_ImportHelpers.ProcessExistingRecord(RowAccount, arrayHeader, &csvData) == FALSE)
			{
				return FALSE;
			}
		}
	}
	else
	{
		if (csvData.GetSize() == 0)
		{
			m_ImportCounters.IncIgnored();
		}
		else
		{
			m_ImportCounters.IncInvalid();
		}
	}

	return TRUE;
}

/**********************************************************************/
