/**********************************************************************/
#ifndef LOYSERVER
/**********************************************************************/
#include "..\SPosLoyalty4Manager\WorkingDlg.h"
/**********************************************************************/
#endif
/**********************************************************************/
#include "AccountFieldValidator.h"
#include "AuditRecord_base.h"
#include "LoyaltyAuditAppend.h"
#include "LoyaltyHeaderHelpers.h"
#include "SQLNamespaces.h"
#include "SQLTable_Account\SQLRepositoryAccount.h"
#include "SQLTable_Group\SQLRepositoryGroup.h"
#include "SQLTable_GroupTopupBonus\SQLRepositoryGroupTopupBonus.h"
/**********************************************************************/
#include "SQLTranImportDatabase.h"
/**********************************************************************/

void CSQLTranImportDatabase::DoWorkManagerImport(bool bBackgroundMode, bool bForUpgrade, int nImportMethod, CSSFile& ImportFile, CArray<int, int>& arrayHeader, int nIndexUserID )
{
	if (SQLTRAN_STATE_ACTIVE == m_nState)
	{
		m_bCanCommit = FALSE;
		m_nState = SQLTRAN_STATE_WAITEND;

		m_nRead = 0;
		m_nInvalid = 0;
		m_nIgnored = 0;
		m_nAdded = 0;
		m_nUsed = 0;

#ifndef LOYSERVER

		m_nImportMethod = nImportMethod;
		m_nIndexUserID = nIndexUserID;
		m_nSourceType = AUDIT_PC;

		{
			CWorkingDlg WorkingDlg("Import CSV Account Data", ( FALSE == bForUpgrade));

			if (FALSE == bBackgroundMode)
			{
				WorkingDlg.Create();
			}

			::SendToRecycleBin(Filenames.GetDBImportExceptionsFilename());

			int nCount = 0;
			CString strImportLine = "";
			while (ImportFile.ReadString(strImportLine) == TRUE)
			{
				++m_nRead;

				if (FALSE == bBackgroundMode)
				{
					if (++nCount == 10)
					{
						if (WorkingDlg.SetCaption2PercentDone(ImportFile.GetPercentPosition(), (FALSE == bForUpgrade)) == TRUE)
						{
							return;
						}

						nCount = 0;
					}
				}

				if (FALSE == bForUpgrade)
				{
					if (ImportLine(arrayHeader, strImportLine) == FALSE)
					{
						return;
					}
				}
				else
				{
					if (ImportLineForUpgrade(arrayHeader, strImportLine) == FALSE)
					{
						return;
					}
				}
			}
		}
		
		m_bCanCommit = TRUE;

#endif
	}
}

/**********************************************************************/

void CSQLTranImportDatabase::DoWorkServerImport(int nImportMethod, CSSFile& ImportFile, CArray<int, int>& arrayHeader, int nIndexUserID)
{
	if (SQLTRAN_STATE_ACTIVE == m_nState)
	{
		m_bCanCommit = FALSE;
		m_nState = SQLTRAN_STATE_WAITEND;

		m_nRead = 0;
		m_nInvalid = 0;
		m_nIgnored = 0;
		m_nAdded = 0;
		m_nUsed = 0;

#ifdef LOYSERVER

		m_nImportMethod = nImportMethod;
		m_nIndexUserID = nIndexUserID;
		m_nSourceType = AUDIT_SERVER;

		{
			CString strImportLine = "";
			while (ImportFile.ReadString(strImportLine) == TRUE)
			{
				++m_nRead;
				if (ImportLine(arrayHeader, strImportLine) == FALSE)
				{
					return;
				}
			}
		}

		m_bCanCommit = TRUE;

#endif
	}
}

/**********************************************************************/

//ONLY RETURN FALSE IF WE HAVE AN SQL ERROR AND NEED TO STOP THE TRANSACTION
bool CSQLTranImportDatabase::ImportLine(CArray<int, int>& arrayHeader, const char* szImportLine)
{
	CCSV csvData(szImportLine);

	__int64 nUserID = 0;
	bool bValidUserID = FALSE;

	{
		CString strUserID = csvData.GetString(m_nIndexUserID);

		if (::IsStringNumeric(strUserID, 1, System.GetUserIDLength()))
		{
			nUserID = _atoi64(strUserID);
			bValidUserID = ((nUserID >= Account::UserID.Min) && (nUserID <= Account::UserID.Max));
		}
	}

	if (FALSE == bValidUserID)
	{
		CSSFile file;
		if (file.Open(Filenames.GetDBImportExceptionsFilename(), "ab") == TRUE)
		{
			CCSV csv;
			csv.Add(m_nRead + 1);					// Line number of error ( allow for header line in csv file )
			csv.Add(csvData.GetLine());
			file.Write(csv);
			file.Close();
		}
		++m_nInvalid;
	}
	else
	{
		int nAppNo = APPNO_IMPORTUPDATE;

		CSQLRowAccountFull RowAccount;
		RowAccount.SetUserID(nUserID);

		bool bExists = FALSE;
		CSQLRepositoryAccount repoAccount;
		switch (repoAccount.SelectRow(RowAccount, m_pDatabase).GetSQLError())
		{
		case SQLCRUD_ERR_NONE:
			bExists = TRUE;
			break;

		case SQLCRUD_ERR_NOMATCH:
			break;

		default:
			return FALSE;
		}

		// see if must exist
		if ((FALSE == bExists) && (IMPORT_METHOD_EXISTONLY == m_nImportMethod))
		{
			++m_nIgnored;
			return TRUE;
		}

		// only new record allowed
		if ((TRUE == bExists) && (IMPORT_METHOD_NEWONLY == m_nImportMethod))
		{
			++m_nIgnored;
			return TRUE;
		}

		if (FALSE == bExists)
		{
			++m_nAdded;
			nAppNo = APPNO_IMPORTINSERT;
		}

		++m_nUsed;
		if (ImportFields(arrayHeader, &csvData, RowAccount) == FALSE)
		{
			return FALSE;
		}

		CAuditRecord_base atc;
		atc.SetSQL(RowAccount);
		atc.SetSourceType(m_nSourceType);
		atc.SetApplicationNo(nAppNo);
		atc.SetPoints(RowAccount.GetPoints());
		atc.SetPointsTD(RowAccount.GetPointsToDate());
		atc.SetPurse1(RowAccount.GetPurse1Balance());
		atc.SetPurse2(RowAccount.GetPurse2Balance());
		atc.SetPurse1SpendTD(RowAccount.GetPurse1SpendToDate());
		atc.SetPurse2SpendTD(RowAccount.GetPurse2SpendToDate());
		atc.SetCashSpendTD(RowAccount.GetCashSpendToDate());
		atc.SaveLineToArray(m_arrayAuditBuffer);
	}

	return TRUE;
}

//*****************************************************************************************

//ONLY RETURN FALSE IF WE HAVE AN SQL ERROR AND NEED TO STOP THE TRANSACTION
bool CSQLTranImportDatabase::ImportLineForUpgrade(CArray<int, int>& arrayHeader, const char* szImportLine)
{
	CCSV csvData(szImportLine);

	__int64 nUserID = 0;
	bool bValidUserID = FALSE;

	{
		CString strCardNo = csvData.GetString(m_nIndexUserID);

		if (::IsStringNumeric(strCardNo, 1, System.GetUserIDLength()))
		{
			nUserID = _atoi64(strCardNo);
			bValidUserID = ((nUserID >= Account::UserID.Min) && (nUserID <= Account::UserID.Max));
		}
	}

	if (FALSE == bValidUserID)
	{
		m_nInvalid++;
	}
	else
	{
		CSQLRowAccountFull RowAccount;
		RowAccount.SetUserID(nUserID);

		bool bExists = FALSE;
		CSQLRepositoryAccount repoAccount;
		switch (repoAccount.SelectRow(RowAccount, m_pDatabase).GetSQLError())
		{
		case SQLCRUD_ERR_NONE:
			bExists = TRUE;
			break;

		case SQLCRUD_ERR_NOMATCH:
			break;

		default:
			return FALSE;
		}

		if (FALSE == bExists)
		{
			++m_nAdded;
		}

		++m_nUsed;
		if (ImportFields(arrayHeader, &csvData, RowAccount) == FALSE)
		{
			return FALSE;
		}
	}
	
	return TRUE;
}

//*****************************************************************************************

//ONLY RETURN FALSE IF WE HAVE AN SQL ERROR AND NEED TO STOP THE TRANSACTION
bool CSQLTranImportDatabase::ImportFields(CArray<int, int>& arrayHeader, CCSV* pCsvData, CSQLRowAccountFull& RowAccount)
{
	bool bGotQRInfo = FALSE;
	CString strQRInfo = "";

	for (int i = 0; i < arrayHeader.GetSize(); i++)
	{
		CString strField = pCsvData->GetString(i);
		if (strField == "")
		{
			continue;
		}

		switch (arrayHeader.GetAt(i))
		{
			// already have cardno
		case LOYALTYFIELD_USERID:
			break;

			// v2.06a - 07/01/2016 - Database Import SetExtRef saved the label & not the data
		case LOYALTYFIELD_EXTERNALREF:
			RowAccount.SetExternalRef(strField);
			break;

		case LOYALTYFIELD_QRINFO:
			strQRInfo = strField;
			bGotQRInfo = TRUE;
			break;

		case LOYALTYFIELD_FULLNAME:
			RowAccount.SetFullname(strField);
			break;

		case LOYALTYFIELD_FORENAME:
			RowAccount.SetForename(strField);
			break;

		case LOYALTYFIELD_SURNAME:
			RowAccount.SetSurname(strField);
			break;

		case LOYALTYFIELD_GROUPNO:
			RowAccount.SetGroupNo(pCsvData->GetInt(i));
			break;

		case LOYALTYFIELD_SCHEME:
			RowAccount.SetSchemeNo(pCsvData->GetInt(i));
			break;

		case LOYALTYFIELD_DOB:
			RowAccount.SetDOB(strField);
			break;

		case LOYALTYFIELD_GENDER:
			RowAccount.SetGender(strField);
			break;

		case LOYALTYFIELD_EXPIRYDATE:
			RowAccount.SetExpiryDate(strField);
			break;

		case LOYALTYFIELD_MAXSPEND:
			RowAccount.SetMaxSpend(pCsvData->GetDouble(i));
			break;

		case LOYALTYFIELD_MAXOVERDRAFT:
			RowAccount.SetMaxOverdraft(pCsvData->GetDouble(i));
			break;

			// v2.06a - 07/01/2016 - Fix : Database Import checking was wrong ( code was "if ( strLabel = Account::Inactive.Label )"
			// Convert 1 or T or Y or X to TRUE else FALSE
		case LOYALTYFIELD_INACTIVE:
			RowAccount.SetInactive(RowAccount.GetBoolData(strField));
			break;

		case LOYALTYFIELD_PREVUSERID:
			RowAccount.SetPreviousUserID(pCsvData->GetInt64(i));
			break;

		case LOYALTYFIELD_NEXTUSERID:
			RowAccount.SetNextUserID(pCsvData->GetInt64(i));
			break;

		case LOYALTYFIELD_HOTLISTCODE:
			RowAccount.SetHotlistCode(pCsvData->GetInt(i));
			break;

		case LOYALTYFIELD_HOTLISTDATE:
			RowAccount.SetHotlistDate(strField);
			break;

		case LOYALTYFIELD_HOTLISTTIME:
			RowAccount.SetHotlistTime(strField);
			break;

		case LOYALTYFIELD_POINTS:
			RowAccount.SetPoints(pCsvData->GetInt(i));
			break;

		case LOYALTYFIELD_POINTSTODATE:
			RowAccount.SetPointsToDate(pCsvData->GetInt(i));
			break;

		case LOYALTYFIELD_PURSE1_BALANCE:
		{
			double dValue = pCsvData->GetDouble(i);

			if (dValue != RowAccount.GetPurse1Balance())
			{
				RowAccount.SetPurse1Balance(dValue);

				CAuditRecord_base atc;
				atc.SetSQL(RowAccount);
				atc.SetSourceType(m_nSourceType);
				atc.SetApplicationNo(APPNO_SETPURSE1);			// audit cash added
				atc.SetPurse1Transaction(0.0);
				atc.SetPurse2Transaction(0.0);
				atc.SetCashTransaction(0.0);
				atc.SetPointsTransaction(0);
				atc.SaveLineToArray(m_arrayAuditBuffer);
			}
		}
		break;

		case LOYALTYFIELD_PURSE1_TOPUP:
		{
			double dValue = pCsvData->GetDouble(i);

			if (dValue != 0.0)
			{
				RowAccount.AddToPurse1(dValue);

				CAuditRecord_base atc;
				atc.SetSQL(RowAccount);
				atc.SetSourceType(m_nSourceType);
				atc.SetApplicationNo(APPNO_PURSE1_IMPTOPUP);			// audit cash added
				atc.SetPurse1Transaction(dValue);
				atc.SetPurse2Transaction(0.0);
				atc.SetCashTransaction(0.0);
				atc.SetPointsTransaction(0);
				atc.SaveLineToArray(m_arrayAuditBuffer);

				ApplyRevaluationBonus(RowAccount, dValue,1);
			}
		}
		break;

		case LOYALTYFIELD_PURSE2_TOPUP:
		{
			double dValue = pCsvData->GetDouble(i);

			if (dValue != 0.0)
			{
				RowAccount.AddToPurse2(dValue);

				CAuditRecord_base atc;
				atc.SetSQL(RowAccount);
				atc.SetSourceType(m_nSourceType);
				atc.SetApplicationNo(APPNO_PURSE2_IMPTOPUP);			// audit cash added
				atc.SetPurse1Transaction(0.0);
				atc.SetPurse2Transaction(dValue);
				atc.SetCashTransaction(0.0);
				atc.SetPointsTransaction(0);
				atc.SaveLineToArray(m_arrayAuditBuffer);

				ApplyRevaluationBonus(RowAccount, dValue, 2);
			}
		}
		break;

		case LOYALTYFIELD_PURSE1_LASTSPENDDATE:
			RowAccount.SetPurse1LastSpendDate(strField);
			break;

		case LOYALTYFIELD_PURSE1_LASTSPENDTIME:
			RowAccount.SetPurse1LastSpendTime(strField);
			break;

		case LOYALTYFIELD_PURSE1_LASTSPEND:
		{
			// last spend should be a +ve value ( // v2.07x - 06/03/2017 )
			// will be negative if importing from a fiddle audit line ( re EBM\MeanTime)

			double dValue = pCsvData->GetDouble(i);
			if (dValue < 0)
			{
				dValue = -(dValue);
			}

			RowAccount.SetPurse1LastSpend(dValue);
		}
		break;

		case LOYALTYFIELD_PURSE1_SPENDTODATE:
			RowAccount.SetPurse1SpendToDate(pCsvData->GetDouble(i));
			break;

		case LOYALTYFIELD_CASH_LASTSPENDDATE:
			RowAccount.SetCashLastSpendDate(strField);
			break;

		case LOYALTYFIELD_CASH_LASTSPENDTIME:
			RowAccount.SetCashLastSpendTime(strField);
			break;

		case LOYALTYFIELD_CASH_LASTSPEND:
			RowAccount.SetCashLastSpend(pCsvData->GetDouble(i));
			break;

		case LOYALTYFIELD_CASH_SPENDTODATE:
			RowAccount.SetCashSpendToDate(pCsvData->GetDouble(i));
			break;

		case LOYALTYFIELD_ADDRESS1:
			RowAccount.SetAddress1(strField);
			break;

		case LOYALTYFIELD_ADDRESS2:
			RowAccount.SetAddress2(strField);
			break;

		case LOYALTYFIELD_ADDRESS3:
			RowAccount.SetAddress3(strField);
			break;

		case LOYALTYFIELD_ADDRESS4:
			RowAccount.SetAddress4(strField);
			break;

		case LOYALTYFIELD_ADDRESS5:
			RowAccount.SetAddress5(strField);
			break;

		case LOYALTYFIELD_INFO1:
			RowAccount.SetInfo1(strField);
			break;

		case LOYALTYFIELD_INFO2:
			RowAccount.SetInfo2(strField);
			break;

		case LOYALTYFIELD_INFO3:
			RowAccount.SetInfo3(strField);
			break;

		case LOYALTYFIELD_INFO4:
			RowAccount.SetInfo4(strField);
			break;

		case LOYALTYFIELD_PHONE1:
			RowAccount.SetPhone1(strField);
			break;

		case LOYALTYFIELD_PHONE2:
			RowAccount.SetPhone2(strField);
			break;
		}
	}

	CSQLRepositoryAccount repoAccount;

	if ((TRUE == bGotQRInfo) && (strQRInfo != ""))
	{
		if (repoAccount.RemoveQRInfo(strQRInfo, m_pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
		{
			return FALSE;
		}

		RowAccount.SetQRInfo(strQRInfo);
	}

	if (repoAccount.UpsertRow(RowAccount, m_pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
	{
		return FALSE;
	}

	return TRUE;
}

//******************************************************************

void CSQLTranImportDatabase::ApplyRevaluationBonus(CSQLRowAccountFull& RowAccount, double dRevalueAmount, int nPurse)
{
	int nTripType = (2 == nPurse) ? Server.GetTopupPCBonusTypePurse2() : Server.GetTopupPCBonusTypePurse1();

	if ((nTripType < 1) || (nTripType > REVALUE_PLUNO_PERPURSE_COUNT))
	{
		return;
	}

	bool bUseGroupSettings = FALSE;

	double dRevaluationTrip = 0.0;
	int nRevaluationType = 0;
	int nRevaluationPoints = 0;
	double dRevaluationValue = 0.0;
	int nRevaluationPercent = 0;
	int nRevaluationPurse = (2 == nPurse) ? 2 : 1;

	CSQLRowGroup RowGroup;
	RowGroup.SetGroupNo(RowAccount.GetGroupNo());

	CSQLRepositoryGroup repoGroup;
	CSQLRepositoryGroupTopupBonus repoBonus;

	if (repoGroup.SelectRow(RowGroup, m_pDatabase).GetSQLError() == SQLCRUD_ERR_NONE)
	{
		CSQLRowGroupTopupBonus RowGroupTopupBonus;
		RowGroupTopupBonus.SetGroupNo(RowAccount.GetGroupNo());
		RowGroupTopupBonus.SetTopupNo(nTripType);

		if (repoBonus.SelectRow(RowGroupTopupBonus, m_pDatabase).GetSQLError() == SQLCRUD_ERR_NONE)
		{
			if (RowGroupTopupBonus.GetBonusFlag() == TRUE)
			{
				bUseGroupSettings = TRUE;

				if (RowGroupTopupBonus.GetBonusDisable() == FALSE)			// see if disabled for this group
				{
					dRevaluationTrip = RowGroupTopupBonus.GetBonusTrip();
					nRevaluationType = RowGroupTopupBonus.GetBonusType();
					nRevaluationPoints = RowGroupTopupBonus.GetBonusPoints();
					dRevaluationValue = RowGroupTopupBonus.GetBonusValue();
					nRevaluationPercent = RowGroupTopupBonus.GetBonusPercent();
				}
			}
		}
	}

	if (FALSE == bUseGroupSettings)
	{
		dRevaluationTrip = Server.GetTopupBonusTrip(nPurse, nTripType);
		nRevaluationType = Server.GetTopupBonusType(nPurse, nTripType);
		nRevaluationPoints = Server.GetTopupBonusPoints(nPurse, nTripType);
		dRevaluationValue = Server.GetTopupBonusValue(nPurse, nTripType);
		nRevaluationPercent = Server.GetTopupBonusPercent(nPurse, nTripType);
	}

	if (dRevaluationTrip != 0.0)								// see if any revaluation bonus is required
	{
		switch (nRevaluationType)
		{
		case 0:
		{
			int nBonusPoints = Server.CalculateRevaluationBonusPoints(dRevalueAmount, dRevaluationTrip, nRevaluationPoints);
			if (nBonusPoints != 0)
			{
				RowAccount.UpdatePoints(nBonusPoints);

				CAuditRecord_base atc;
				atc.SetSQL(RowAccount);
				atc.SetSourceType(m_nSourceType);
				atc.SetApplicationNo(APPNO_REVALUEBONUSPOINTS);	// audit bonus points

				atc.SetPointsTransaction(nBonusPoints);
				atc.SetPurse1Transaction(0.0);
				atc.SetPurse2Transaction(0.0);
				atc.SetCashTransaction(dRevalueAmount);
				atc.SetComment((2 == nPurse) ? Server.GetTopupCommentPurse2() : Server.GetTopupCommentPurse1());
				atc.SaveLineToArray(m_arrayAuditBuffer);
			}
		}
		break;

		case 1:
		{
			double dBonusValue = Server.CalculateRevaluationBonusValue(dRevalueAmount, dRevaluationTrip, dRevaluationValue);
			if (dBonusValue != 0.0)
			{
				if (nRevaluationPurse != 2)
				{
					RowAccount.AddToPurse1(dBonusValue);
				}
				else
				{
					RowAccount.AddToPurse2(dBonusValue);
				}

				CAuditRecord_base atc;
				atc.SetSQL(RowAccount);
				atc.SetSourceType(m_nSourceType);
				atc.SetApplicationNo(APPNO_REVALUEBONUSCASH);	// audit bonus cash

				if (nRevaluationPurse != 2)
				{
					atc.SetPurse1Transaction(dBonusValue);
					atc.SetPurse2Transaction(0.0);
				}
				else
				{
					atc.SetPurse1Transaction(0.0);
					atc.SetPurse2Transaction(dBonusValue);
				}

				atc.SetCashTransaction(dRevalueAmount);
				atc.SetPointsTransaction(0);
				atc.SetComment((2 == nPurse) ? Server.GetTopupCommentPurse2() : Server.GetTopupCommentPurse1());
				atc.SaveLineToArray(m_arrayAuditBuffer);
			}
		}
		break;

		case 2:
		{
			double dBonusValue = Server.CalculateRevaluationBonusPercent(dRevalueAmount, dRevaluationTrip, nRevaluationPercent);
			if (dBonusValue != 0.0)
			{
				if (nRevaluationPurse != 2)
				{
					RowAccount.AddToPurse1(dBonusValue);
				}
				else
				{
					RowAccount.AddToPurse2(dBonusValue);
				}

				CAuditRecord_base atc;
				atc.SetSQL(RowAccount);
				atc.SetSourceType(m_nSourceType);
				atc.SetApplicationNo(APPNO_REVALUEBONUSCASH);	// audit bonus cash

				if (nRevaluationPurse != 2)
				{
					atc.SetPurse1Transaction(dBonusValue);
					atc.SetPurse2Transaction(0.0);
				}
				else
				{
					atc.SetPurse1Transaction(0.0);
					atc.SetPurse2Transaction(dBonusValue);
				}

				atc.SetCashTransaction(dRevalueAmount);
				atc.SetPointsTransaction(0);
				atc.SetComment((2 == nPurse) ? Server.GetTopupCommentPurse2() : Server.GetTopupCommentPurse1());
				atc.SaveLineToArray(m_arrayAuditBuffer);
			}
		}
		break;
		}
	}
}

//******************************************************************

