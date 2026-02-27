/**********************************************************************/
#include "..\SmartPay4Shared\SmartPayHeaderHelpers.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SmartPay4Shared\SQLTable_ImportFilenameCache\SQLRepositoryImportFilenameCache.h"
#include "..\SmartPay4Shared\SQLHelpers.h"
#include "..\SmartPay4Shared\WorkingDlg.h"
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
/**********************************************************************/
#include "SQLTranImportDatabase.h"
/**********************************************************************/
static const char* szCLEAR_FIELD = "CLEAR";
/**********************************************************************/

CSQLTranImportDatabase::CSQLTranImportDatabase( int nCacheType, CString strCacheFilename ) : CSQLTranBase(),
	m_ImportHelpers(m_atc, m_ImportCounters)
{
	m_nCacheType = nCacheType;
	m_strCacheFilename = strCacheFilename;
	m_strCacheFilename.MakeUpper();
}

/**********************************************************************/

void CSQLTranImportDatabase::DoWork(int nImportMethod, int nImportAppNo, CSSFile& ImportFile, CArray<int, int>& arrayHeader, int nIndexUserID)
{
	if (SQLTRAN_STATE_ACTIVE == m_nState)
	{
		m_bCanCommit = FALSE;
		m_nState = SQLTRAN_STATE_WAITEND;

		m_nImportMethod = nImportMethod;
		m_nImportAppNo = nImportAppNo;
		m_nIndexUserID = nIndexUserID;

		m_ImportHelpers.SetDatabase(m_pDatabase);
		m_ImportHelpers.SetImportAppNo(m_nImportAppNo);

		CWorkingDlg dlgWorking("Importing CSV Data");
		dlgWorking.Create();

		CFileRecycler::SendToRecycleBin(Filenames.GetDBImportExceptionsFilename());

		{
			CString strImportLine;
			ImportFile.ReadString(strImportLine);								// skip header

			while (ImportFile.ReadString(strImportLine) == TRUE)
			{
				m_ImportCounters.IncRead();

				if (dlgWorking.SetCaption2PercentDone(ImportFile.GetPercentPosition(), TRUE) == TRUE)
				{
					return;
				}

				if (ImportLine(arrayHeader, strImportLine) == FALSE)
				{
					return;
				}
			}
		}

		if ( (m_nCacheType != -1) && ( m_strCacheFilename != "" ) )
		{
			CSQLRowImportFilenameCache RowCache;
			RowCache.SetType(m_nCacheType);
			RowCache.SetFilename(m_strCacheFilename);
			RowCache.SetDateTime(GetSimpleDateTimeNowString());
			
			CSQLRepositoryImportFilenameCache RepoCache;
			if (RepoCache.InsertRow(RowCache, m_pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
			{
				return;
			}
		}

		m_bCanCommit = TRUE;
	}
}

/**********************************************************************/

//RETURN FALSE ONLY IF THERE IS AN ERROR THAT REQUIRES TRANSACTION TO STOP
bool CSQLTranImportDatabase::ImportLine(CArray<int, int>& arrayHeader, const char* szImportLine)
{
	CCSV csvData(szImportLine);
	m_strExceptionPrefix = "";

	CString strCsvUserID = csvData.GetString(m_nIndexUserID);			// get UserID from csv line

	CString strCardNo;
	if (CSQLHelpers::IsValidUserID(strCsvUserID, strCardNo, System.GetUserIDLength()) == TRUE)
	{
		CSQLRowAccountFull RowAccount;
		RowAccount.SetUserIDString(strCardNo);

		CSQLRepositoryAccount RepoAccount;
		bool bExists = (RepoAccount.SelectRow(RowAccount, m_pDatabase).GetSQLError() == SQLCRUD_ERR_NONE);

		if ((FALSE == bExists) && (nIMPORT_METHOD_EXISTONLY == m_nImportMethod))	
		{
			m_ImportHelpers.LogNoRecord(strCardNo, FALSE);								
			return TRUE;
		}

		if ((TRUE == bExists) && (nIMPORT_METHOD_NEWONLY == m_nImportMethod))
		{
			m_ImportHelpers.LogNoRecord(strCardNo, TRUE);
			return TRUE;
		}

		if (FALSE == bExists)
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
			m_strExceptionPrefix = "Skipped";
		}
		else
		{
			m_ImportCounters.IncInvalid();
			m_strExceptionPrefix = "Invalid";
		}
	}

	if (m_strExceptionPrefix != "")
	{
		m_ImportHelpers.LogException(m_strExceptionPrefix, csvData.GetLine());
	}

	return TRUE;
}

//*******************************************************************

