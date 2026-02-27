/**********************************************************************/
#include "..\SmartPay4Shared\SmartPayHeaderHelpers.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SmartPay4Shared\SQLTable_ImportFilenameCache\SQLRepositoryImportFilenameCache.h"
#include "..\SmartPay4Shared\SQLHelpers.h"
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
#include "..\SmartPay4Shared\WorkingDlg.h"
/**********************************************************************/
#include "SQLTranImportTopup.h"
/**********************************************************************/
static const char* szCLEAR_FIELD = "CLEAR";
/**********************************************************************/

CSQLTranImportTopup::CSQLTranImportTopup(int nCacheType, CString strCacheFilename) : CSQLTranBase(),
m_ImportHelpers(m_atc, m_ImportCounters)
{
	m_nImportAppNo = 0;
	m_nIndexUserID = 0;
	m_nIndexLocateOn = 0;
	m_nImportMethod = nIMPORT_METHOD_ALL;
	m_nCacheType = nCacheType;
	m_strCacheFilename = strCacheFilename;
	m_strCacheFilename.MakeUpper();
}

/**********************************************************************/

void CSQLTranImportTopup::DoWork(int nImportAppNo, CSSFile& ImportFile, CArray<int, int>& arrayHeader, int nLocateOnIndex, CString strLocateOnLabel, int nIndexUserID)
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

		m_nIndexLocateOn = nLocateOnIndex;
		m_strLocateOnLabel = strLocateOnLabel;

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

		if ((m_nCacheType != -1) && (m_strCacheFilename != ""))
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
bool CSQLTranImportTopup::ImportLine(CArray<int, int>& arrayHeader, const char* szImportLine)
{
	CString strImportLine = szImportLine;

	m_strExceptionPrefix = "";

	CCSV csvData(strImportLine);

	CString strCsvUserID = csvData.GetString(m_nIndexUserID);			// get UserID from csv line

	CString strCardNo;
	bool bNewRecord = FALSE;

	if (CSQLHelpers::IsValidUserID(strCsvUserID, strCardNo, System.GetUserIDLength()) == TRUE)
	{
		CSQLRowAccountFull RowAccount;
		RowAccount.SetUserIDString(strCardNo);

		CSQLRepositoryAccount RepoAccount;
		__int64 nSQLError = RepoAccount.SelectRow(RowAccount, m_pDatabase).GetSQLError();
		
		if (SQLCRUD_ERR_NOMATCH == nSQLError)
		{
			if (m_nImportMethod == nIMPORT_METHOD_ALL)
			{
				if (m_ImportHelpers.ProcessNewRecord(RowAccount, arrayHeader, NULL) == FALSE)
				{
					return FALSE;
				}

				bNewRecord = TRUE;

				CString strLocateOnData = csvData.GetString(m_nIndexLocateOn);

				if (m_strLocateOnLabel == Account::Info1.Label)
				{
					RowAccount.SetInfo1(strLocateOnData);
				}
				else if (m_strLocateOnLabel == Account::Info2.Label)
				{
					RowAccount.SetInfo2(strLocateOnData);
				}
				else if (m_strLocateOnLabel == Account::Info3.Label)
				{
					RowAccount.SetInfo3(strLocateOnData);
				}
				else if (m_strLocateOnLabel == Account::Info4.Label)
				{
					RowAccount.SetInfo4(strLocateOnData);
				}
				else if (m_strLocateOnLabel == Account::Info5.Label)
				{
					RowAccount.SetInfo5(strLocateOnData);
				}
				else if (m_strLocateOnLabel == Account::Info6.Label)
				{
					RowAccount.SetInfo6(strLocateOnData);
				}
				else if (m_strLocateOnLabel == Account::Info7.Label)
				{
					RowAccount.SetInfo7(strLocateOnData);
				}
				else if (m_strLocateOnLabel == Account::Info8.Label)
				{
					RowAccount.SetInfo8(strLocateOnData);
				}
				else if (m_strLocateOnLabel == Account::MemberID.Label)
				{
					RowAccount.SetMemberID(strLocateOnData);
				}
				else if (m_strLocateOnLabel == Account::ExternalRef1.Label)
				{
					RowAccount.SetExternalRef1(strLocateOnData);
				}
				else if (m_strLocateOnLabel == Account::ExternalRef2.Label)
				{
					RowAccount.SetExternalRef2(strLocateOnData);
				}

				if (RepoAccount.UpdateRow(RowAccount, m_pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
				{
					return FALSE;
				}
			}
			else
			{
				m_ImportHelpers.LogNoRecord(strCardNo, FALSE);	
				return TRUE;
			}
		}

		switch (nSQLError)
		{
		case SQLCRUD_ERR_NONE:
		case SQLCRUD_ERR_NOMATCH:
		{
			m_atc.Set(&RowAccount);										// set audit is for a pc line
			m_atc.SetApplicationNo(APPNO_IMPORTTOPUP);

			ImportTopupFields(RowAccount, arrayHeader, &csvData);
			if (RepoAccount.UpdateRow(RowAccount, m_pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
			{
				return FALSE;
			}

			if (m_atc.AuditChanges(&RowAccount) == TRUE)			// audit line
			{
				if (bNewRecord == FALSE)								// used counter already updated when record added
				{
					m_ImportCounters.IncUsed();
				}

				m_atc.BufferNewLine();										// save audit line
			}
			else
			{
				m_ImportCounters.IncIgnored();
				m_strExceptionPrefix = "No change";
			}
		}
		break;
		}
	}
	else
	{
		if (strImportLine == "")
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
		m_ImportHelpers.LogException(m_strExceptionPrefix, strImportLine);
	}

	return TRUE;
}

//**********************************************************************

void CSQLTranImportTopup::ImportTopupFields(CSQLRowAccountFull& RowAccount, CArray<int, int>& arrayHeader, CCSV* pCsvData)
{
	for (int i = 0; i < arrayHeader.GetSize(); i++)
	{
		switch (arrayHeader.GetAt(i))
		{
		case SMARTPAYFIELD_USERID:
			break;

		case SMARTPAYFIELD_PURSE1_LIABILITY:
			RowAccount.AddToPurse1Liability(pCsvData->GetDouble(i));
			break;

		case SMARTPAYFIELD_PURSE1_CREDIT:
			if (System.GetCreditOptionsFlag() == TRUE)
			{
				bool bAllow = TRUE;

				double dNewCredit = pCsvData->GetDouble(i);

				if (dNewCredit < 0.0)
				{
					bAllow = (RowAccount.GetPurse1Credit() + dNewCredit) >= 0.0;
				}

				if (TRUE == bAllow)
				{
					RowAccount.AddToPurse1Credit(dNewCredit);
				}
			}
			break;

		case SMARTPAYFIELD_PURSE2_BALANCE:
			RowAccount.AddToPurse2Balance(pCsvData->GetDouble(i));
			break;

		case SMARTPAYFIELD_PURSE3_LIABILITY:
			RowAccount.AddToPurse3Liability(pCsvData->GetDouble(i));
			break;

		case SMARTPAYFIELD_PURSE3_CREDIT:
			if (System.GetCreditOptionsFlag() == TRUE)
			{
				bool bAllow = TRUE;

				double dNewCredit = pCsvData->GetDouble(i);

				if (dNewCredit < 0.0)
				{
					bAllow = (RowAccount.GetPurse3Credit() + dNewCredit) >= 0.0;
				}

				if (TRUE == bAllow)
				{
					RowAccount.AddToPurse3Credit(dNewCredit);
				}
			}
			break;

		case SMARTPAYFIELD_POINTS:
			RowAccount.AddToPoints(pCsvData->GetInt(i));
			break;

		case SMARTPAYFIELD_POINTS_TODATE:
			RowAccount.AddToPointsToDate(pCsvData->GetInt(i));
			break;
		}
	}
}

//**********************************************************************

