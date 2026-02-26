//**********************************************************************
#include "..\SPosLoyalty4Shared\SQLHelpers.h"
#include "..\SPosLoyalty4Shared\SQLNamespaces.h"
//**********************************************************************
#include "SQLIn.h"
//**********************************************************************
#include "ProjectSQLRepositoryAccount.h"
//**********************************************************************

CProjectSQLRepositoryAccount::CProjectSQLRepositoryAccount() : CSQLRepositoryAccount()
{
	m_strImportTableName = SQLTableNames::Accounts;
}

//**********************************************************************

bool CProjectSQLRepositoryAccount::ImportFromScript(CSSFile& fileIn, CDatabase* pDatabase, CWorkingDlg& WorkingDlg)
{
	WorkingDlg.SetCaption1("Preparing Accounts Table");
    if ( CSQLHelpers::DeleteTableRows(m_strImportTableName, pDatabase) == FALSE )
    {
        return FALSE;
	}
	WorkingDlg.SetCaption1("Restoring Accounts Table");

	fileIn.Seek(0, CFile::begin);

	CStringArray arrayHeader;
	arrayHeader.Add(Account::UserID.Label);
	arrayHeader.Add(Account::Fullname.Label);
	arrayHeader.Add(Account::Forename.Label);
	arrayHeader.Add(Account::Surname.Label);
	arrayHeader.Add(Account::DOB.Label);
	arrayHeader.Add(Account::ExpiryDate.Label);
	arrayHeader.Add(Account::Gender.Label);
	arrayHeader.Add(Account::GroupNo.Label);
	arrayHeader.Add(Account::SchemeNo.Label);
	arrayHeader.Add(Account::Info1.Label);
	arrayHeader.Add(Account::Info2.Label);
	arrayHeader.Add(Account::Info3.Label);
	arrayHeader.Add(Account::Info4.Label);
	arrayHeader.Add(Account::Address1.Label);
	arrayHeader.Add(Account::Address2.Label);
	arrayHeader.Add(Account::Address3.Label);
	arrayHeader.Add(Account::Address4.Label);
	arrayHeader.Add(Account::Address5.Label);
	arrayHeader.Add(Account::Phone1.Label);
	arrayHeader.Add(Account::Phone2.Label);
	arrayHeader.Add(Account::Inactive.Label);
	/*****/
	arrayHeader.Add(Account::ExternalRef.Label);
	arrayHeader.Add(Account::QRInfo.Label);
	arrayHeader.Add(Account::NextUserID.Label);
	arrayHeader.Add(Account::PreviousUserID.Label);
	/*****/
	arrayHeader.Add(Account::Purse1Balance.Label);
	arrayHeader.Add(Account::Purse1LastSpendDate.Label);
	arrayHeader.Add(Account::Purse1LastSpendTime.Label);
	arrayHeader.Add(Account::Purse1LastSpend.Label);
	arrayHeader.Add(Account::Purse1SpendToDate.Label);
	/*****/
	arrayHeader.Add(Account::Purse2Balance.Label);
	arrayHeader.Add(Account::Purse2LastSpendDate.Label);
	arrayHeader.Add(Account::Purse2LastSpendTime.Label);
	arrayHeader.Add(Account::Purse2LastSpend.Label);
	arrayHeader.Add(Account::Purse2SpendToDate.Label);
	arrayHeader.Add(Account::Purse2RefreshedDate.Label);
	arrayHeader.Add(Account::Purse2RefreshedTime.Label);
	/*****/
	arrayHeader.Add(Account::CashLastSpendDate.Label);
	arrayHeader.Add(Account::CashLastSpendTime.Label);
	arrayHeader.Add(Account::CashLastSpend.Label);
	arrayHeader.Add(Account::CashSpendToDate.Label);
	/*****/
	arrayHeader.Add(Account::Points.Label);
	arrayHeader.Add(Account::PointsToDate.Label);
	/*****/
	arrayHeader.Add(Account::HotlistCode.Label);
	arrayHeader.Add(Account::HotlistDate.Label);
	arrayHeader.Add(Account::HotlistTime.Label);
	/*****/
	arrayHeader.Add(Account::MaxSpend.Label);
	arrayHeader.Add(Account::MaxOverdraft.Label);
	/*****/
	arrayHeader.Add(Account::AlertCodes.Label);

	CSQLIn SQLIn(fileIn, m_strImportTableName, arrayHeader);

	CString strBuffer = "";
	while (SQLIn.ReadLine(strBuffer))
	{
		CCSV csvIn(strBuffer);

		CSQLRowAccountFull RowAccount;
		RowAccount.SetUserID(csvIn.GetInt64(0));
		RowAccount.SetFullname(csvIn.GetString(1));
		RowAccount.SetForename(csvIn.GetString(2));
		RowAccount.SetSurname(csvIn.GetString(3));
		RowAccount.SetDOB(csvIn.GetString(4));
		RowAccount.SetExpiryDate(csvIn.GetString(5));
		RowAccount.SetGender(csvIn.GetString(6));
		RowAccount.SetGroupNo(csvIn.GetInt(7));
		RowAccount.SetSchemeNo(csvIn.GetInt(8));
		RowAccount.SetInfo1(csvIn.GetString(9));
		RowAccount.SetInfo2(csvIn.GetString(10));
		RowAccount.SetInfo3(csvIn.GetString(11));
		RowAccount.SetInfo4(csvIn.GetString(12));
		RowAccount.SetAddress1(csvIn.GetString(13));
		RowAccount.SetAddress2(csvIn.GetString(14));
		RowAccount.SetAddress3(csvIn.GetString(15));
		RowAccount.SetAddress4(csvIn.GetString(16));
		RowAccount.SetAddress5(csvIn.GetString(17));
		RowAccount.SetPhone1(csvIn.GetString(18));
		RowAccount.SetPhone2(csvIn.GetString(19));
		RowAccount.SetInactive(csvIn.GetBool(20));
		/*****/
		RowAccount.SetExternalRef(csvIn.GetString(21));
		RowAccount.SetQRInfo(csvIn.GetString(22));
		RowAccount.SetNextUserID(csvIn.GetInt64(23));
		RowAccount.SetPreviousUserID(csvIn.GetInt64(24));
		/*****/
		RowAccount.SetPurse1Balance(csvIn.GetDouble(25));
		RowAccount.SetPurse1LastSpendDate(csvIn.GetString(26));
		RowAccount.SetPurse1LastSpendTime(csvIn.GetString(27));
		RowAccount.SetPurse1LastSpend(csvIn.GetDouble(28));
		RowAccount.SetPurse1SpendToDate(csvIn.GetDouble(29));
		/*****/
		RowAccount.SetPurse2Balance(csvIn.GetDouble(30));
		RowAccount.SetPurse2LastSpendDate(csvIn.GetString(31));
		RowAccount.SetPurse2LastSpendTime(csvIn.GetString(32));
		RowAccount.SetPurse2LastSpend(csvIn.GetDouble(33));
		RowAccount.SetPurse2SpendToDate(csvIn.GetDouble(34));
		RowAccount.SetPurse2RefreshedDate(csvIn.GetString(35));
		RowAccount.SetPurse2RefreshedTime(csvIn.GetString(36));
		/*****/
		RowAccount.SetCashLastSpendDate(csvIn.GetString(37));
		RowAccount.SetCashLastSpendTime(csvIn.GetString(38));
		RowAccount.SetCashLastSpend(csvIn.GetDouble(39));
		RowAccount.SetCashSpendToDate(csvIn.GetDouble(40));
		/*****/
		RowAccount.SetPoints(csvIn.GetInt(41));
		RowAccount.SetPointsToDate(csvIn.GetInt(42));
		/*****/
		RowAccount.SetHotlistCode(csvIn.GetInt(43));
		RowAccount.SetHotlistDate(csvIn.GetString(44));
		RowAccount.SetHotlistTime(csvIn.GetString(45));
		/*****/
		RowAccount.SetMaxSpend(csvIn.GetDouble(46));
		RowAccount.SetMaxOverdraft(csvIn.GetDouble(47));
		/*****/
		RowAccount.SetAlertCodes(csvIn.GetString(48));

		if (InsertRow(RowAccount, pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
		{
			return FALSE;
		}

		WorkingDlg.IncrementRecordsChecked(10, FALSE);
	}

	return TRUE;
}

//**********************************************************************
