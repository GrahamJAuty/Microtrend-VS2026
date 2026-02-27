//**********************************************************************
#include "..\SmartPay4Shared\SQLHelpers.h"
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "SQLIn.h"
//**********************************************************************
#include "ProjectSQLRepositoryAccount.h"
//**********************************************************************

CProjectSQLRepositoryAccount::CProjectSQLRepositoryAccount(int nTableType) : CSQLRepositoryAccount()
{
	m_nTableType = nTableType;
	
	switch (m_nTableType)
	{
	case ACCOUNTS_TABLE_LEAVERS:
		m_strImportTableName = SQLTableNames::Leavers;
		break;

	case ACCOUNTS_TABLE_CURRENT:
	default:
		m_strImportTableName = SQLTableNames::Accounts;
		break;
	}
}

//**********************************************************************

bool CProjectSQLRepositoryAccount::ImportFromScript(CSSFile& fileIn, CDatabase* pDatabase, CWorkingDlg& WorkingDlg)
{
	switch (m_nTableType)
	{
	case ACCOUNTS_TABLE_LEAVERS:
		WorkingDlg.SetCaption1("Preparing Leavers Table");
		break;

	case ACCOUNTS_TABLE_CURRENT:
	default:
		WorkingDlg.SetCaption1("Preparing Accounts Table");
		break;
	}

	if (CSQLHelpers::DeleteTableRows(m_strImportTableName, pDatabase) == FALSE)
	{
		return FALSE;
	}

	fileIn.Seek(0, CFile::begin);

	CStringArray arrayHeader;

	switch (m_nTableType)
	{
	case ACCOUNTS_TABLE_LEAVERS:
		arrayHeader.Add(Account::LeaverYear.Label);
		WorkingDlg.SetCaption1("Restoring Leavers Table");
		break;

	case ACCOUNTS_TABLE_CURRENT:
		default:
		WorkingDlg.SetCaption1("Restoring Accounts Table");
		break;
	}

	arrayHeader.Add(Account::UserID.Label);
	arrayHeader.Add(Account::Username.Label);
	arrayHeader.Add(Account::Forename.Label);
	arrayHeader.Add(Account::Surname.Label);
	arrayHeader.Add(Account::DOB.Label);
	arrayHeader.Add(Account::ExpiryDate.Label);
	arrayHeader.Add(Account::Gender.Label);
	arrayHeader.Add(Account::GroupNo.Label);
	arrayHeader.Add(Account::DayGroups.Label);
	arrayHeader.Add(Account::Email.Label);
	arrayHeader.Add(Account::Info1.Label);
	arrayHeader.Add(Account::Info2.Label);
	arrayHeader.Add(Account::Info3.Label);
	arrayHeader.Add(Account::Info4.Label);
	arrayHeader.Add(Account::Info5.Label);
	arrayHeader.Add(Account::Info6.Label);
	arrayHeader.Add(Account::Info7.Label);
	arrayHeader.Add(Account::Info8.Label);
	arrayHeader.Add(Account::Tick1.Label);
	arrayHeader.Add(Account::Tick2.Label);
	arrayHeader.Add(Account::Tick3.Label);
	arrayHeader.Add(Account::Tick4.Label);
	arrayHeader.Add(Account::AllergyCodes.Label);
	arrayHeader.Add(Account::Dietary.Label);
	arrayHeader.Add(Account::Inactive.Label);
	/*****/
	arrayHeader.Add(Account::MemberID.Label);
	arrayHeader.Add(Account::ExternalRef1.Label);
	arrayHeader.Add(Account::ExternalRef2.Label);
	arrayHeader.Add(Account::NextUserID.Label);
	arrayHeader.Add(Account::PreviousUserID.Label);
	arrayHeader.Add(Account::PebblePurseID.Label);
	/*****/
	arrayHeader.Add(Account::Purse1Allow.Label);
	arrayHeader.Add(Account::Purse1Liability.Label);
	arrayHeader.Add(Account::Purse1Credit.Label);
	arrayHeader.Add(Account::Purse1LastSpendDate.Label);
	arrayHeader.Add(Account::Purse1LastSpendTime.Label);
	arrayHeader.Add(Account::Purse1LastSpend.Label);
	arrayHeader.Add(Account::Purse1SpendToDate.Label);
	/*****/
	arrayHeader.Add(Account::Purse2Allow.Label);
	arrayHeader.Add(Account::Purse2Balance.Label);
	arrayHeader.Add(Account::Purse2LastSpendDate.Label);
	arrayHeader.Add(Account::Purse2LastSpendTime.Label);
	arrayHeader.Add(Account::Purse2LastSpend.Label);
	arrayHeader.Add(Account::Purse2SpendToDate.Label);
	arrayHeader.Add(Account::Purse2RefreshFlags.Label);
	arrayHeader.Add(Account::Purse2RefreshedDate.Label);
	arrayHeader.Add(Account::Purse2RefreshedTime.Label);
	/*****/
	arrayHeader.Add(Account::Purse3Allow.Label);
	arrayHeader.Add(Account::Purse3Liability.Label);
	arrayHeader.Add(Account::Purse3Credit.Label);
	arrayHeader.Add(Account::Purse3LastSpendDate.Label);
	arrayHeader.Add(Account::Purse3LastSpendTime.Label);
	arrayHeader.Add(Account::Purse3LastSpend.Label);
	arrayHeader.Add(Account::Purse3SpendToDate.Label);
	/*****/
	arrayHeader.Add(Account::CashSpendToDate.Label);
	/*****/
	arrayHeader.Add(Account::Points.Label);
	arrayHeader.Add(Account::PointsToDate.Label);
	/*****/
	arrayHeader.Add(Account::OwnMaxSpend.Label);
	/*****/
	arrayHeader.Add(Account::MaxSpendPurse1Period1.Label);
	arrayHeader.Add(Account::MaxSpendPurse1Period2.Label);
	arrayHeader.Add(Account::MaxSpendPurse1Period3.Label);
	arrayHeader.Add(Account::MaxSpendPurse1Period4.Label);
	arrayHeader.Add(Account::MaxSpendPurse1Period5.Label);
	arrayHeader.Add(Account::MaxSpendPurse1Period6.Label);
	/*****/
	arrayHeader.Add(Account::MaxSpendPurse3Period1.Label);
	arrayHeader.Add(Account::MaxSpendPurse3Period2.Label);
	arrayHeader.Add(Account::MaxSpendPurse3Period3.Label);
	arrayHeader.Add(Account::MaxSpendPurse3Period4.Label);
	arrayHeader.Add(Account::MaxSpendPurse3Period5.Label);
	arrayHeader.Add(Account::MaxSpendPurse3Period6.Label);
	/*****/
	arrayHeader.Add(Account::MaxOverdraftPurse1Period1.Label);
	arrayHeader.Add(Account::MaxOverdraftPurse1Period2.Label);
	arrayHeader.Add(Account::MaxOverdraftPurse1Period3.Label);
	arrayHeader.Add(Account::MaxOverdraftPurse1Period4.Label);
	arrayHeader.Add(Account::MaxOverdraftPurse1Period5.Label);
	arrayHeader.Add(Account::MaxOverdraftPurse1Period6.Label);
	/*****/
	arrayHeader.Add(Account::MaxOverdraftPurse3Period1.Label);
	arrayHeader.Add(Account::MaxOverdraftPurse3Period2.Label);
	arrayHeader.Add(Account::MaxOverdraftPurse3Period3.Label);
	arrayHeader.Add(Account::MaxOverdraftPurse3Period4.Label);
	arrayHeader.Add(Account::MaxOverdraftPurse3Period5.Label);
	arrayHeader.Add(Account::MaxOverdraftPurse3Period6.Label);
	/*****/
	arrayHeader.Add(Account::AlertCode.Label);
	arrayHeader.Add(Account::AlertDate.Label);
	arrayHeader.Add(Account::AlertTime.Label);

	CSQLIn SQLIn(fileIn, m_strImportTableName, arrayHeader);

	CString strBuffer = "";
	while (SQLIn.ReadLine(strBuffer))
	{
		CCSV csvIn(strBuffer);

		CSQLRowAccountFull RowAccount;

		int nOffset = 0;

		switch (m_nTableType)
		{
		case ACCOUNTS_TABLE_LEAVERS:
			RowAccount.SetLeaverYear(csvIn.GetInt(nOffset++));
			break;
		}

		RowAccount.SetUserID(csvIn.GetInt64(nOffset++));
		RowAccount.SetUsername(csvIn.GetString(nOffset++));
		RowAccount.SetForename(csvIn.GetString(nOffset++));
		RowAccount.SetSurname(csvIn.GetString(nOffset++));
		RowAccount.SetDOB(csvIn.GetString(nOffset++));
		RowAccount.SetExpiryDate(csvIn.GetString(nOffset++));
		RowAccount.SetGender(csvIn.GetString(nOffset++));
		RowAccount.SetGroupNo(csvIn.GetInt(nOffset++));
		RowAccount.SetDayGroups(csvIn.GetString(nOffset++));
		RowAccount.SetEmail(csvIn.GetString(nOffset++));
		RowAccount.SetInfo1(csvIn.GetString(nOffset++));
		RowAccount.SetInfo2(csvIn.GetString(nOffset++));
		RowAccount.SetInfo3(csvIn.GetString(nOffset++));
		RowAccount.SetInfo4(csvIn.GetString(nOffset++));
		RowAccount.SetInfo5(csvIn.GetString(nOffset++));
		RowAccount.SetInfo6(csvIn.GetString(nOffset++));
		RowAccount.SetInfo7(csvIn.GetString(nOffset++));
		RowAccount.SetInfo8(csvIn.GetString(nOffset++));
		RowAccount.SetTick1(csvIn.GetBool(nOffset++));
		RowAccount.SetTick2(csvIn.GetBool(nOffset++));
		RowAccount.SetTick3(csvIn.GetBool(nOffset++));
		RowAccount.SetTick4(csvIn.GetBool(nOffset++));
		RowAccount.SetAllergyCodes(csvIn.GetInt(nOffset++));
		RowAccount.SetDietary(csvIn.GetString(nOffset++));
		RowAccount.SetInactive(csvIn.GetBool(nOffset++));
		/*****/
		RowAccount.SetMemberID(csvIn.GetString(nOffset++));
		RowAccount.SetExternalRef1(csvIn.GetString(nOffset++));
		RowAccount.SetExternalRef2(csvIn.GetString(nOffset++));
		RowAccount.SetNextUserID(csvIn.GetInt64(nOffset++));
		RowAccount.SetPreviousUserID(csvIn.GetInt64(nOffset++));
		RowAccount.SetLegacyPebblePurseID(csvIn.GetString(nOffset++));
		/*****/
		RowAccount.SetPurse1Allow(csvIn.GetInt(nOffset++));
		RowAccount.SetPurse1Liability(csvIn.GetDouble(nOffset++));
		RowAccount.SetPurse1Credit(csvIn.GetDouble(nOffset++));
		RowAccount.SetPurse1LastSpendDate(csvIn.GetString(nOffset++));
		RowAccount.SetPurse1LastSpendTime(csvIn.GetString(nOffset++));
		RowAccount.SetPurse1LastSpend(csvIn.GetDouble(nOffset++));
		RowAccount.SetPurse1SpendToDate(csvIn.GetDouble(nOffset++));
		/*****/
		RowAccount.SetPurse2Allow(csvIn.GetInt(nOffset++));
		RowAccount.SetPurse2Balance(csvIn.GetDouble(nOffset++));
		RowAccount.SetPurse2LastSpendDate(csvIn.GetString(nOffset++));
		RowAccount.SetPurse2LastSpendTime(csvIn.GetString(nOffset++));
		RowAccount.SetPurse2LastSpend(csvIn.GetDouble(nOffset++));
		RowAccount.SetPurse2SpendToDate(csvIn.GetDouble(nOffset++));
		RowAccount.SetPurse2RefreshFlags(csvIn.GetInt(nOffset++));
		RowAccount.SetPurse2RefreshedDate(csvIn.GetString(nOffset++));
		RowAccount.SetPurse2RefreshedTime(csvIn.GetString(nOffset++));
		/*****/
		RowAccount.SetPurse3Allow(csvIn.GetInt(nOffset++));
		RowAccount.SetPurse3Liability(csvIn.GetDouble(nOffset++));
		RowAccount.SetPurse3Credit(csvIn.GetDouble(nOffset++));
		RowAccount.SetPurse3LastSpendDate(csvIn.GetString(nOffset++));
		RowAccount.SetPurse3LastSpendTime(csvIn.GetString(nOffset++));
		RowAccount.SetPurse3LastSpend(csvIn.GetDouble(nOffset++));
		RowAccount.SetPurse3SpendToDate(csvIn.GetDouble(nOffset++));
		/*****/
		RowAccount.SetCashSpendToDate(csvIn.GetDouble(nOffset++));
		/*****/
		RowAccount.SetPoints(csvIn.GetInt(nOffset++));
		RowAccount.SetPointsToDate(csvIn.GetInt(nOffset++));
		/*****/
		RowAccount.SetOwnMaxSpend(csvIn.GetBool(nOffset++));
		/*****/
		RowAccount.SetMaxSpendPurse1Period1(csvIn.GetDouble(nOffset++));
		RowAccount.SetMaxSpendPurse1Period2(csvIn.GetDouble(nOffset++));
		RowAccount.SetMaxSpendPurse1Period3(csvIn.GetDouble(nOffset++));
		RowAccount.SetMaxSpendPurse1Period4(csvIn.GetDouble(nOffset++));
		RowAccount.SetMaxSpendPurse1Period5(csvIn.GetDouble(nOffset++));
		RowAccount.SetMaxSpendPurse1Period6(csvIn.GetDouble(nOffset++));
		/*****/
		RowAccount.SetMaxSpendPurse3Period1(csvIn.GetDouble(nOffset++));
		RowAccount.SetMaxSpendPurse3Period2(csvIn.GetDouble(nOffset++));
		RowAccount.SetMaxSpendPurse3Period3(csvIn.GetDouble(nOffset++));
		RowAccount.SetMaxSpendPurse3Period4(csvIn.GetDouble(nOffset++));
		RowAccount.SetMaxSpendPurse3Period5(csvIn.GetDouble(nOffset++));
		RowAccount.SetMaxSpendPurse3Period6(csvIn.GetDouble(nOffset++));
		/*****/
		RowAccount.SetMaxOverdraftPurse1Period1(csvIn.GetDouble(nOffset++));
		RowAccount.SetMaxOverdraftPurse1Period2(csvIn.GetDouble(nOffset++));
		RowAccount.SetMaxOverdraftPurse1Period3(csvIn.GetDouble(nOffset++));
		RowAccount.SetMaxOverdraftPurse1Period4(csvIn.GetDouble(nOffset++));
		RowAccount.SetMaxOverdraftPurse1Period5(csvIn.GetDouble(nOffset++));
		RowAccount.SetMaxOverdraftPurse1Period6(csvIn.GetDouble(nOffset++));
		/*****/
		RowAccount.SetMaxOverdraftPurse3Period1(csvIn.GetDouble(nOffset++));
		RowAccount.SetMaxOverdraftPurse3Period2(csvIn.GetDouble(nOffset++));
		RowAccount.SetMaxOverdraftPurse3Period3(csvIn.GetDouble(nOffset++));
		RowAccount.SetMaxOverdraftPurse3Period4(csvIn.GetDouble(nOffset++));
		RowAccount.SetMaxOverdraftPurse3Period5(csvIn.GetDouble(nOffset++));
		RowAccount.SetMaxOverdraftPurse3Period6(csvIn.GetDouble(nOffset++));
		/*****/
		RowAccount.SetAlertCode(csvIn.GetInt(nOffset++));
		RowAccount.SetAlertDate(csvIn.GetString(nOffset++));
		RowAccount.SetAlertTime(csvIn.GetString(nOffset++));

		switch (m_nTableType)
		{
		case ACCOUNTS_TABLE_LEAVERS:
			if (InsertLeaverDirect(RowAccount, pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
			{
				return FALSE;
			}
			break;

		case ACCOUNTS_TABLE_CURRENT:
		default:
			if (InsertRow(RowAccount, pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
			{
				return FALSE;
			}
			break;
		}

		WorkingDlg.IncrementRecordsChecked(10, FALSE);
	}

	return TRUE;

}

//**********************************************************************
