/**********************************************************************/
#include "..\SmartPay4Shared\SmartPayHeaderHelpers.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
/**********************************************************************/
#include "SQLTranImportHelpers.h"
/**********************************************************************/
static const char* szCLEAR_FIELD = "CLEAR";
/**********************************************************************/

CSQLImportCounters::CSQLImportCounters()
{
	m_nIgnored = 0;
	m_nRead = 0;
	m_nUsed = 0;
	m_nInvalid = 0;
	m_nAdded = 0;
	m_nDeleted = 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CSQLTranImportHelpers::CSQLTranImportHelpers(CSQLAuditRecord_base& atc, CSQLImportCounters& ImportCounters) :
	m_atc(atc),
	m_ImportCounters(ImportCounters)
{
	m_pDatabase = NULL;
	m_nImportAppNo = 0;
	m_bAllergyRead = FALSE;
}

/**********************************************************************/

void CSQLTranImportHelpers::ImportFieldsStage1(CSQLRowAccountFull& RowAccount, CArray<int, int>& arrayHeader, CCSV* pCsvData, bool& bGotMaxSpendField, bool& bChangedGroup)
{
	bGotMaxSpendField = FALSE;
	bChangedGroup = FALSE;

	int nOldGroupNo = RowAccount.GetGroupNo();

	for (int i = 0; i < arrayHeader.GetSize(); i++)
	{
		CString strField = pCsvData->GetString(i);							// 07/01/2016 - v1.10b - Database import checks have valid data in csv field
		if (strField == "")												// otherwise field would be cleared
		{
			continue;
		}

		switch (arrayHeader.GetAt(i))
		{
		case SMARTPAYFIELD_USERID:
			break;

		case SMARTPAYFIELD_MEMBERID:
			if (strField.CompareNoCase(szCLEAR_FIELD) == 0)
			{
				RowAccount.SetMemberID("");			//23/09/2017
			}
			else
			{
				RowAccount.SetMemberID(strField);
			}
			break;

		case SMARTPAYFIELD_EXTERNALREF1:
			if (strField.CompareNoCase(szCLEAR_FIELD) == 0)
			{
				RowAccount.SetExternalRef1("");				//23/09/2017
			}
			else
			{
				if (System.GetImportExtRef1TrimZeroFlag() == TRUE)
				{
					strField.TrimLeft('0');

					if (strField == "")
					{
						continue;
					}
				}

				RowAccount.SetExternalRef1(strField);
			}
			break;

		case SMARTPAYFIELD_EXTERNALREF2:
			if (strField.CompareNoCase(szCLEAR_FIELD) == 0)
			{
				RowAccount.SetExternalRef2("");			//23/09/2017
			}
			else
			{
				RowAccount.SetExternalRef2(strField);
			}
			break;

		case SMARTPAYFIELD_USERNAME:
			RowAccount.SetUsername(strField);
			break;

		case SMARTPAYFIELD_FORENAME:
			RowAccount.SetForename(strField);
			break;

		case SMARTPAYFIELD_SURNAME:
			RowAccount.SetSurname(strField);
			break;

		case SMARTPAYFIELD_EMAIL:
			RowAccount.SetEmail(strField);
			break;

		case SMARTPAYFIELD_GROUPNO:
			RowAccount.SetGroupNo(pCsvData->GetInt(i));
			break;

		case SMARTPAYFIELD_GENDER:
			RowAccount.SetGender(strField);
			break;

		case SMARTPAYFIELD_PURSE1_LIABILITY:
			RowAccount.SetPurse1Liability(pCsvData->GetDouble(i));
			break;

		case SMARTPAYFIELD_PURSE1_CREDIT:
		{
			if (System.GetCreditOptionsFlag() == TRUE)
			{
				double dCredit = pCsvData->GetDouble(i);

				if (dCredit >= 0.0)
				{
					RowAccount.SetPurse1Credit(dCredit);
				}
			}
		}
		break;

		case SMARTPAYFIELD_PURSE1_LASTSPENDDATE:
			RowAccount.SetPurse1LastSpendDate(strField);
			break;

		case SMARTPAYFIELD_PURSE1_LASTSPENDTIME:
			RowAccount.SetPurse1LastSpendTime(strField);
			break;

		case SMARTPAYFIELD_PURSE1_LASTSPEND:
			RowAccount.SetPurse1LastSpend(pCsvData->GetDouble(i));
			break;

		case SMARTPAYFIELD_PURSE1_SPENDTODATE:
			RowAccount.SetPurse1SpendToDate(pCsvData->GetDouble(i));
			break;

		case SMARTPAYFIELD_PURSE3_LIABILITY:
			RowAccount.SetPurse3Liability(pCsvData->GetDouble(i));
			break;

		case SMARTPAYFIELD_PURSE3_CREDIT:
		{
			if (System.GetCreditOptionsFlag() == TRUE)
			{
				double dCredit = pCsvData->GetDouble(i);

				if (dCredit >= 0.0)
				{
					RowAccount.SetPurse3Credit(dCredit);
				}
			}
		}
		break;

		case SMARTPAYFIELD_PURSE3_LASTSPENDDATE:
			RowAccount.SetPurse3LastSpendDate(strField);
			break;

		case SMARTPAYFIELD_PURSE3_LASTSPENDTIME:
			RowAccount.SetPurse3LastSpendTime(strField);
			break;

		case SMARTPAYFIELD_PURSE3_LASTSPEND:
			RowAccount.SetPurse3LastSpend(pCsvData->GetDouble(i));
			break;

		case SMARTPAYFIELD_PURSE3_SPENDTODATE:
			RowAccount.SetPurse3SpendToDate(pCsvData->GetDouble(i));
			break;

		case SMARTPAYFIELD_PURSE2_BALANCE:
			RowAccount.SetPurse2Balance(pCsvData->GetDouble(i));
			break;

		case SMARTPAYFIELD_PURSE2_LASTSPENDDATE:
			RowAccount.SetPurse2LastSpendDate(strField);
			break;

		case SMARTPAYFIELD_PURSE2_LASTSPENDTIME:
			RowAccount.SetPurse2LastSpendTime(strField);
			break;

		case SMARTPAYFIELD_PURSE2_LASTSPEND:
			RowAccount.SetPurse2LastSpend(pCsvData->GetDouble(i));
			break;

		case SMARTPAYFIELD_PURSE2_SPENDTODATE:
			RowAccount.SetPurse2SpendToDate(pCsvData->GetDouble(i));
			break;

		case SMARTPAYFIELD_PURSE2_REFRESHED_DATE:
			RowAccount.SetPurse2RefreshedDate(strField);
			break;

		case SMARTPAYFIELD_PURSE2_REFRESHED_TIME:
			RowAccount.SetPurse2RefreshedTime(strField);
			break;

		case SMARTPAYFIELD_PURSE2_REFRESHFLAGS:
			RowAccount.SetRefreshRef(strField);
			break;

		case SMARTPAYFIELD_POINTS:
			RowAccount.SetPoints(pCsvData->GetInt(i));
			break;

		case SMARTPAYFIELD_POINTS_TODATE:
			RowAccount.SetPointsToDate(pCsvData->GetInt(i));
			break;

		case SMARTPAYFIELD_CASH_SPENDTODATE:
			RowAccount.SetCashSpendToDate(pCsvData->GetDouble(i));
			break;

		case SMARTPAYFIELD_OWNMAXSPEND:
			RowAccount.SetOwnMaxSpend(pCsvData->GetBool(i));
			break;

		case SMARTPAYFIELD_PURSE1_ALLOW:
		case SMARTPAYFIELD_PURSE2_ALLOW:
		case SMARTPAYFIELD_PURSE3_ALLOW:
		case SMARTPAYFIELD_MAXSPEND_PURSE1_PERIOD1:
		case SMARTPAYFIELD_MAXSPEND_PURSE1_PERIOD2:
		case SMARTPAYFIELD_MAXSPEND_PURSE1_PERIOD3:
		case SMARTPAYFIELD_MAXSPEND_PURSE1_PERIOD4:
		case SMARTPAYFIELD_MAXSPEND_PURSE1_PERIOD5:
		case SMARTPAYFIELD_MAXSPEND_PURSE1_PERIOD6:
		case SMARTPAYFIELD_MAXSPEND_PURSE3_PERIOD1:
		case SMARTPAYFIELD_MAXSPEND_PURSE3_PERIOD2:
		case SMARTPAYFIELD_MAXSPEND_PURSE3_PERIOD3:
		case SMARTPAYFIELD_MAXSPEND_PURSE3_PERIOD4:
		case SMARTPAYFIELD_MAXSPEND_PURSE3_PERIOD5:
		case SMARTPAYFIELD_MAXSPEND_PURSE3_PERIOD6:
		case SMARTPAYFIELD_MAXOVERDRAFT_PURSE1_PERIOD1:
		case SMARTPAYFIELD_MAXOVERDRAFT_PURSE1_PERIOD2:
		case SMARTPAYFIELD_MAXOVERDRAFT_PURSE1_PERIOD3:
		case SMARTPAYFIELD_MAXOVERDRAFT_PURSE1_PERIOD4:
		case SMARTPAYFIELD_MAXOVERDRAFT_PURSE1_PERIOD5:
		case SMARTPAYFIELD_MAXOVERDRAFT_PURSE1_PERIOD6:
		case SMARTPAYFIELD_MAXOVERDRAFT_PURSE3_PERIOD1:
		case SMARTPAYFIELD_MAXOVERDRAFT_PURSE3_PERIOD2:
		case SMARTPAYFIELD_MAXOVERDRAFT_PURSE3_PERIOD3:
		case SMARTPAYFIELD_MAXOVERDRAFT_PURSE3_PERIOD4:
		case SMARTPAYFIELD_MAXOVERDRAFT_PURSE3_PERIOD5:
		case SMARTPAYFIELD_MAXOVERDRAFT_PURSE3_PERIOD6:
			bGotMaxSpendField = TRUE;
			break;

		case SMARTPAYFIELD_ALERTCODE:
			RowAccount.SetAlertCode(pCsvData->GetInt(i));
			break;

		case SMARTPAYFIELD_ALERTDATE:
			RowAccount.SetAlertDate(strField);
			break;

		case SMARTPAYFIELD_ALERTTIME:
			RowAccount.SetAlertTime(strField);
			break;

		case SMARTPAYFIELD_PREVIOUS_USERID:
			RowAccount.SetPreviousUserID(pCsvData->GetInt64(i));
			break;

		case SMARTPAYFIELD_NEXT_USERID:
			RowAccount.SetNextUserID(pCsvData->GetInt64(i));
			break;

		case SMARTPAYFIELD_INFO1:
			RowAccount.SetInfo1(strField);
			break;

		case SMARTPAYFIELD_INFO2:
			RowAccount.SetInfo2(strField);
			break;

		case SMARTPAYFIELD_INFO3:
			RowAccount.SetInfo3(strField);
			break;

		case SMARTPAYFIELD_INFO4:
			RowAccount.SetInfo4(strField);
			break;

		case SMARTPAYFIELD_INFO5:
			RowAccount.SetInfo5(strField);
			break;

		case SMARTPAYFIELD_INFO6:
			RowAccount.SetInfo6(strField);
			break;

		case SMARTPAYFIELD_INFO7:
			RowAccount.SetInfo7(strField);
			break;

		case SMARTPAYFIELD_INFO8:
			RowAccount.SetInfo8(strField);
			break;

		case SMARTPAYFIELD_TICK1:
			RowAccount.SetTick1(GetBoolData(strField));
			break;

		case SMARTPAYFIELD_TICK2:
			RowAccount.SetTick2(GetBoolData(strField));
			break;

		case SMARTPAYFIELD_TICK3:
			RowAccount.SetTick3(GetBoolData(strField));
			break;

		case SMARTPAYFIELD_TICK4:
			RowAccount.SetTick4(GetBoolData(strField));
			break;

		case SMARTPAYFIELD_EXPIRYDATE:
			RowAccount.SetExpiryDate(strField);
			break;

		case SMARTPAYFIELD_DOB:
			RowAccount.SetDOB(strField);
			break;

		case SMARTPAYFIELD_INACTIVE:
			RowAccount.SetInactive(GetBoolData(strField));
			break;

		case SMARTPAYFIELD_ALLERGYCODES:
			if (::IsStringNumeric(strField) == TRUE)
			{
				RowAccount.SetAllergyCodes(pCsvData->GetInt(i));	// see if import a code
			}
			else
			{
				SetAllergies(RowAccount, pCsvData->GetString(i));	// importing text
			}
			break;

		case SMARTPAYFIELD_DIETARY:
			RowAccount.SetDietary(strField);
			break;

		case SMARTPAYFIELD_DAYGROUPS:
			RowAccount.SetDailyGroups(strField);
			break;
		}
	}

	bChangedGroup = (nOldGroupNo != RowAccount.GetGroupNo());
}

/**********************************************************************/

void CSQLTranImportHelpers::ImportFieldsStage2(CSQLRowAccountFull& RowAccount, CArray<int, int>& arrayHeader, CCSV* pCsvData)
{
	for (int i = 0; i < arrayHeader.GetSize(); i++)
	{
		CString strField = pCsvData->GetString(i);							// 07/01/2016 - v1.10b - Database import checks have valid data in csv field
		if (strField == "")												// otherwise field would be cleared
		{
			continue;
		}

		switch (arrayHeader.GetAt(i))
		{
		case SMARTPAYFIELD_PURSE1_ALLOW:
			RowAccount.SetPurse1Allow(pCsvData->GetInt(i));
			break;

		case SMARTPAYFIELD_PURSE2_ALLOW:
			RowAccount.SetPurse2Allow(pCsvData->GetInt(i));
			break;

		case SMARTPAYFIELD_PURSE3_ALLOW:
			RowAccount.SetPurse3Allow(pCsvData->GetInt(i));
			break;

		case SMARTPAYFIELD_MAXSPEND_PURSE1_PERIOD1:
			RowAccount.SetMaxSpendPurse1Period1(pCsvData->GetDouble(i));
			break;

		case SMARTPAYFIELD_MAXSPEND_PURSE1_PERIOD2:
			RowAccount.SetMaxSpendPurse1Period2(pCsvData->GetDouble(i));
			break;

		case SMARTPAYFIELD_MAXSPEND_PURSE1_PERIOD3:
			RowAccount.SetMaxSpendPurse1Period3(pCsvData->GetDouble(i));
			break;

		case SMARTPAYFIELD_MAXSPEND_PURSE1_PERIOD4:
			RowAccount.SetMaxSpendPurse1Period4(pCsvData->GetDouble(i));
			break;

		case SMARTPAYFIELD_MAXSPEND_PURSE1_PERIOD5:
			RowAccount.SetMaxSpendPurse1Period5(pCsvData->GetDouble(i));
			break;

		case SMARTPAYFIELD_MAXSPEND_PURSE1_PERIOD6:
			RowAccount.SetMaxSpendPurse1Period6(pCsvData->GetDouble(i));
			break;

		case SMARTPAYFIELD_MAXSPEND_PURSE3_PERIOD1:
			RowAccount.SetMaxSpendPurse3Period1(pCsvData->GetDouble(i));
			break;

		case SMARTPAYFIELD_MAXSPEND_PURSE3_PERIOD2:
			RowAccount.SetMaxSpendPurse3Period2(pCsvData->GetDouble(i));
			break;

		case SMARTPAYFIELD_MAXSPEND_PURSE3_PERIOD3:
			RowAccount.SetMaxSpendPurse3Period3(pCsvData->GetDouble(i));
			break;

		case SMARTPAYFIELD_MAXSPEND_PURSE3_PERIOD4:
			RowAccount.SetMaxSpendPurse3Period4(pCsvData->GetDouble(i));
			break;

		case SMARTPAYFIELD_MAXSPEND_PURSE3_PERIOD5:
			RowAccount.SetMaxSpendPurse3Period5(pCsvData->GetDouble(i));
			break;

		case SMARTPAYFIELD_MAXSPEND_PURSE3_PERIOD6:
			RowAccount.SetMaxSpendPurse3Period6(pCsvData->GetDouble(i));
			break;

		case SMARTPAYFIELD_MAXOVERDRAFT_PURSE1_PERIOD1:
			RowAccount.SetMaxOverdraftPurse1Period1(pCsvData->GetDouble(i));
			break;

		case SMARTPAYFIELD_MAXOVERDRAFT_PURSE1_PERIOD2:
			RowAccount.SetMaxOverdraftPurse1Period2(pCsvData->GetDouble(i));
			break;

		case SMARTPAYFIELD_MAXOVERDRAFT_PURSE1_PERIOD3:
			RowAccount.SetMaxOverdraftPurse1Period3(pCsvData->GetDouble(i));
			break;

		case SMARTPAYFIELD_MAXOVERDRAFT_PURSE1_PERIOD4:
			RowAccount.SetMaxOverdraftPurse1Period4(pCsvData->GetDouble(i));
			break;

		case SMARTPAYFIELD_MAXOVERDRAFT_PURSE1_PERIOD5:
			RowAccount.SetMaxOverdraftPurse1Period5(pCsvData->GetDouble(i));
			break;

		case SMARTPAYFIELD_MAXOVERDRAFT_PURSE1_PERIOD6:
			RowAccount.SetMaxOverdraftPurse1Period6(pCsvData->GetDouble(i));
			break;

		case SMARTPAYFIELD_MAXOVERDRAFT_PURSE3_PERIOD1:
			RowAccount.SetMaxOverdraftPurse3Period1(pCsvData->GetDouble(i));
			break;

		case SMARTPAYFIELD_MAXOVERDRAFT_PURSE3_PERIOD2:
			RowAccount.SetMaxOverdraftPurse3Period2(pCsvData->GetDouble(i));
			break;

		case SMARTPAYFIELD_MAXOVERDRAFT_PURSE3_PERIOD3:
			RowAccount.SetMaxOverdraftPurse3Period3(pCsvData->GetDouble(i));
			break;

		case SMARTPAYFIELD_MAXOVERDRAFT_PURSE3_PERIOD4:
			RowAccount.SetMaxOverdraftPurse3Period4(pCsvData->GetDouble(i));
			break;

		case SMARTPAYFIELD_MAXOVERDRAFT_PURSE3_PERIOD5:
			RowAccount.SetMaxOverdraftPurse3Period5(pCsvData->GetDouble(i));
			break;

		case SMARTPAYFIELD_MAXOVERDRAFT_PURSE3_PERIOD6:
			RowAccount.SetMaxOverdraftPurse3Period6(pCsvData->GetDouble(i));
			break;
		}
	}
}

/**********************************************************************/

bool CSQLTranImportHelpers::GetBoolData(const char* szData)
{
	CString strData = szData;
	strData.MakeUpper();

	// '1' = bool yes
	if (strData == "1")
	{
		return TRUE;
	}

	// 'Y'es
	if (strData.Left(1) == "Y")
	{
		return TRUE;
	}

	// 'T'rue
	if (strData.Left(1) == "T")
	{
		return TRUE;
	}

	// 'X' marks the spot ;-)
	if (strData == "X")
	{
		return TRUE;
	}

	return FALSE;
}

//***********************************************************************

void CSQLTranImportHelpers::SetAllergies(CSQLRowAccountFull& RowAccount, const char* szText)
{
	if (m_bAllergyRead == FALSE)
	{
		m_allergy.Read();												// only read if required
		m_bAllergyRead = TRUE;
	}

	m_allergy.SetAllergyCodes(RowAccount.GetAllergyCodes());		// current account allergies

	CCSV csv(szText);												// allergy string
	for (int n = 0; n < csv.GetSize(); n++)
	{
		m_allergy.SetAllergy(csv.GetString(n));						// set allergy if in allergy list
	}

	RowAccount.SetAllergyCodes(m_allergy.GetAllergyCodes());		// new account allergy setting
}

//***********************************************************************

bool CSQLTranImportHelpers::ProcessNewRecord(CSQLRowAccountFull& RowAccount, CArray<int, int>& arrayHeader, CCSV* pCsvData)
{
	CSQLRepositoryAccount RepoAccount;
	if (RepoAccount.InsertRow(RowAccount, m_pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
	{
		return FALSE;
	}

	m_ImportCounters.IncUsed();
	m_ImportCounters.IncAdded();

	if (pCsvData != NULL)
	{
		bool bGotMaxSpendField = FALSE;
		bool bChangedGroup = FALSE;
		ImportFieldsStage1(RowAccount, arrayHeader, pCsvData, bGotMaxSpendField, bChangedGroup);
		RowAccount.CopyGroupMaxSpendSettings();

		if (TRUE == bGotMaxSpendField)
		{
			ImportFieldsStage2(RowAccount, arrayHeader, pCsvData);
		}

		if (RepoAccount.UpdateRow(RowAccount, m_pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
		{
			return FALSE;
		}
	}

	// save new audit line
	m_atc.Set(&RowAccount);
	m_atc.SetApplicationNo(APPNO_IMPORTINSERT);
	m_atc.SetTransactionPurse1Credit(m_atc.GetCurrentPurse1Credit());
	m_atc.SetTransactionPurse1Liability(m_atc.GetCurrentPurse1Liability());	
	m_atc.BufferNewLine();											
	
	return TRUE;
}

//*******************************************************************

bool CSQLTranImportHelpers::ProcessExistingRecord(CSQLRowAccountFull& RowAccount, CArray<int, int>& arrayHeader, CCSV* pCsvData)
{
	double dInitialPurse1Liability = RowAccount.GetPurse1Liability();
	double dInitialPurse1Credit = RowAccount.GetPurse1Credit();
	double dInitialPurse2Balance = RowAccount.GetPurse2Balance();	
	double dInitialPurse3Liability = RowAccount.GetPurse3Liability();
	double dInitialPurse3Credit = RowAccount.GetPurse3Credit();
	int nInitialPoints = RowAccount.GetPoints();
	int nInitialGroupNo = RowAccount.GetGroupNo();
	CString strInitialUsername = RowAccount.GetUsername();
	double dInitialPurse1SpendToDate = RowAccount.GetPurse1SpendToDate();
	double dInitialPurse2SpendToDate = RowAccount.GetPurse2SpendToDate();
	double dInitialPointsToDate = RowAccount.GetPointsToDate();
	double dInitialCashSpendToDate = RowAccount.GetCashSpendToDate();

	bool bGotMaxSpendField = FALSE;
	bool bChangedGroup = FALSE;
	ImportFieldsStage1(RowAccount, arrayHeader, pCsvData, bGotMaxSpendField, bChangedGroup);

	if (TRUE == bChangedGroup)
	{
		RowAccount.CopyGroupMaxSpendSettings();
	}

	if (TRUE == bGotMaxSpendField)
	{
		ImportFieldsStage2(RowAccount, arrayHeader, pCsvData);
	}

	CSQLRepositoryAccount RepoAccount;
	if (RepoAccount.UpdateRow(RowAccount, m_pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
	{
		return FALSE;
	}

	bool bAuditRequired = TRUE;

	if (System.GetAuditImportFilterFlag() == TRUE)
	{
		bAuditRequired = FALSE;
		if (!bAuditRequired) bAuditRequired = (CompareDoubles(dInitialPurse1Liability, RowAccount.GetPurse1Liability(), 3 ) != 0);
		if (!bAuditRequired) bAuditRequired = (CompareDoubles(dInitialPurse1Credit, RowAccount.GetPurse1Credit(), 3) != 0);
		if (!bAuditRequired) bAuditRequired = (CompareDoubles(dInitialPurse2Balance, RowAccount.GetPurse2Balance(), 3) != 0);
		if (!bAuditRequired) bAuditRequired = (CompareDoubles(dInitialPurse3Liability, RowAccount.GetPurse3Liability(), 3) != 0);
		if (!bAuditRequired) bAuditRequired = (CompareDoubles(dInitialPurse3Credit, RowAccount.GetPurse3Credit(), 3) != 0);
		if (!bAuditRequired) bAuditRequired = (nInitialPoints != RowAccount.GetPoints());
		if (!bAuditRequired) bAuditRequired = (nInitialGroupNo != RowAccount.GetGroupNo());
		if (!bAuditRequired) bAuditRequired = (strInitialUsername.CompareNoCase(RowAccount.GetUsername()) != 0);
		if (!bAuditRequired) bAuditRequired = (CompareDoubles(dInitialPurse1SpendToDate, RowAccount.GetPurse1SpendToDate(), 3) != 0);
		if (!bAuditRequired) bAuditRequired = (CompareDoubles(dInitialPurse2SpendToDate, RowAccount.GetPurse2SpendToDate(), 3) != 0);
		if (!bAuditRequired) bAuditRequired = (CompareDoubles(dInitialPointsToDate, RowAccount.GetPointsToDate(), 3) != 0);
		if (!bAuditRequired) bAuditRequired = (CompareDoubles(dInitialCashSpendToDate, RowAccount.GetCashSpendToDate(), 3) != 0);
	}

	m_ImportCounters.IncUsed();

	if (TRUE == bAuditRequired)
	{
		m_atc.Set(&RowAccount);
		m_atc.SetApplicationNo(m_nImportAppNo);
		m_atc.SetTransactionPurse1Credit(m_atc.GetCurrentPurse1Credit() - dInitialPurse1Credit);
		m_atc.SetTransactionPurse1Liability(m_atc.GetCurrentPurse1Liability() - dInitialPurse1Liability);
		m_atc.BufferNewLine();
	}
	
	return TRUE;
}

//*******************************************************************

void CSQLTranImportHelpers::LogNoRecord(const char* szCardNo, bool bRecordExists)
{
	m_ImportCounters.IncIgnored();

	CString strPrefix = (bRecordExists == FALSE) ? "No record" : "Record exists";

	CString strText = "";
	strText.Format("Account %s", 
		(const char*) System.FormatCardNo(szCardNo));

	LogException(strPrefix, strText);
}

//***********************************************************************

void CSQLTranImportHelpers::LogException(const char* szPrefix, const char* szText)
{
	CSSFile file;
	if (file.Open(Filenames.GetDBImportExceptionsFilename(), "ab") == TRUE)
	{
		CString strText = szText;
		CString strPrefix = szPrefix;

		if (strText == "")
		{
			strText = "{blank line}";
		}

		CString strLine = "";
		strLine.Format("%s: %s", 
			(const char*) strPrefix,
			(const char*) strText);

		CCSV csv;
		// Line number of error ( allow for header line in csv file )
		csv.Add(m_ImportCounters.GetRead() + 1);	
		csv.Add(strLine);
		file.Write(csv);
		file.Close();
	}
}

//***********************************************************************

