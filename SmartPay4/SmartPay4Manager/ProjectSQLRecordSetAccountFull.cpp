//**********************************************************************
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "..\SmartPay4Shared\GlobalState.h"
//**********************************************************************
#include "ProjectSQLRecordSetAccountFull.h"
//**********************************************************************

CProjectSQLRecordSetAccountFull::CProjectSQLRecordSetAccountFull() : CSQLRecordSetAccountFull(NULL, RSParams_AccountFull_Backup{})
{
}

//**********************************************************************

void CProjectSQLRecordSetAccountFull::ExportToScript(CSSFile& fileOut, CWorkingDlg& WorkingDlg, int nTableType)
{
	CStringArray arrayHeader;

	switch (nTableType)
	{
	case ACCOUNTS_TABLE_LEAVERS:
		//set leaver year so that recordset will access leaver table
		arrayHeader.Add(Account::LeaverYear.Label);
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

	CString strTableName = "";

	switch (nTableType)
	{
	case ACCOUNTS_TABLE_LEAVERS:
		strTableName = SQLTableNames::Leavers;
		break;

	case ACCOUNTS_TABLE_CURRENT:
	default:
		strTableName = SQLTableNames::Accounts;
		break;
	}

	CSQLOut SQLOut(fileOut, strTableName, arrayHeader);
	m_strSort = "";

	SQLOut.WriteDelete();

	CSQLRowAccountFull RowAccount;
	while (StepSelectAll(RowAccount) == TRUE)
	{
		switch (nTableType)
		{
		case ACCOUNTS_TABLE_LEAVERS:
			SQLOut.Add(RowAccount.GetLeaverYear());
			break;
		}

		SQLOut.Add(RowAccount.GetUserID());
		SQLOut.Add(RowAccount.GetUsername());
		SQLOut.Add(RowAccount.GetForename());
		SQLOut.Add(RowAccount.GetSurname());
		SQLOut.Add(RowAccount.GetDOB());
		SQLOut.Add(RowAccount.GetExpiryDate());
		SQLOut.Add(RowAccount.GetGender());
		SQLOut.Add(RowAccount.GetGroupNo());
		SQLOut.Add(RowAccount.GetDayGroups());
		SQLOut.Add(RowAccount.GetEmail());
		SQLOut.Add(RowAccount.GetInfo1());
		SQLOut.Add(RowAccount.GetInfo2());
		SQLOut.Add(RowAccount.GetInfo3());
		SQLOut.Add(RowAccount.GetInfo4());
		SQLOut.Add(RowAccount.GetInfo5());
		SQLOut.Add(RowAccount.GetInfo6());
		SQLOut.Add(RowAccount.GetInfo7());
		SQLOut.Add(RowAccount.GetInfo8());
		SQLOut.Add(RowAccount.GetTick1());
		SQLOut.Add(RowAccount.GetTick2());
		SQLOut.Add(RowAccount.GetTick3());
		SQLOut.Add(RowAccount.GetTick4());
		SQLOut.Add(RowAccount.GetAllergyCodes());
		SQLOut.Add(RowAccount.GetDietary());
		SQLOut.Add(RowAccount.GetInactive());
		/*****/
		SQLOut.Add(RowAccount.GetMemberID());
		SQLOut.Add(RowAccount.GetExternalRef1());
		SQLOut.Add(RowAccount.GetExternalRef2());
		SQLOut.Add(RowAccount.GetNextUserID());
		SQLOut.Add(RowAccount.GetPreviousUserID());
		SQLOut.Add(RowAccount.GetLegacyPebblePurseID());
		/*****/
		SQLOut.Add(RowAccount.GetPurse1Allow());
		SQLOut.Add(RowAccount.GetPurse1Liability(), 2);
		SQLOut.Add(RowAccount.GetPurse1Credit(), 2);
		SQLOut.Add(RowAccount.GetPurse1LastSpendDate());
		SQLOut.Add(RowAccount.GetPurse1LastSpendTime());
		SQLOut.Add(RowAccount.GetPurse1LastSpend(), 2);
		SQLOut.Add(RowAccount.GetPurse1SpendToDate(), 2);
		/*****/
		SQLOut.Add(RowAccount.GetPurse2Allow());
		SQLOut.Add(RowAccount.GetPurse2Balance(), 2);
		SQLOut.Add(RowAccount.GetPurse2LastSpendDate());
		SQLOut.Add(RowAccount.GetPurse2LastSpendTime());
		SQLOut.Add(RowAccount.GetPurse2LastSpend(), 2);
		SQLOut.Add(RowAccount.GetPurse2SpendToDate(), 2);
		SQLOut.Add(RowAccount.GetPurse2RefreshFlags());
		SQLOut.Add(RowAccount.GetPurse2RefreshedDate());
		SQLOut.Add(RowAccount.GetPurse2RefreshedTime());
		/*****/
		SQLOut.Add(RowAccount.GetPurse3Allow());
		SQLOut.Add(RowAccount.GetPurse3Liability(), 2);
		SQLOut.Add(RowAccount.GetPurse3Credit(), 2);
		SQLOut.Add(RowAccount.GetPurse3LastSpendDate());
		SQLOut.Add(RowAccount.GetPurse3LastSpendTime());
		SQLOut.Add(RowAccount.GetPurse3LastSpend(), 2);
		SQLOut.Add(RowAccount.GetPurse3SpendToDate(), 2);
		/*****/
		SQLOut.Add(RowAccount.GetCashSpendToDate(), 2);
		/*****/
		SQLOut.Add(RowAccount.GetPoints());
		SQLOut.Add(RowAccount.GetPointsToDate());
		/*****/
		SQLOut.Add(RowAccount.GetOwnMaxSpend());
		/*****/
		SQLOut.Add(RowAccount.GetMaxSpendPurse1Period1(), 2);
		SQLOut.Add(RowAccount.GetMaxSpendPurse1Period2(), 2);
		SQLOut.Add(RowAccount.GetMaxSpendPurse1Period3(), 2);
		SQLOut.Add(RowAccount.GetMaxSpendPurse1Period4(), 2);
		SQLOut.Add(RowAccount.GetMaxSpendPurse1Period5(), 2);
		SQLOut.Add(RowAccount.GetMaxSpendPurse1Period6(), 2);
		/*****/
		SQLOut.Add(RowAccount.GetMaxSpendPurse3Period1(), 2);
		SQLOut.Add(RowAccount.GetMaxSpendPurse3Period2(), 2);
		SQLOut.Add(RowAccount.GetMaxSpendPurse3Period3(), 2);
		SQLOut.Add(RowAccount.GetMaxSpendPurse3Period4(), 2);
		SQLOut.Add(RowAccount.GetMaxSpendPurse3Period5(), 2);
		SQLOut.Add(RowAccount.GetMaxSpendPurse3Period6(), 2);
		/*****/
		SQLOut.Add(RowAccount.GetMaxOverdraftPurse1Period1(), 2);
		SQLOut.Add(RowAccount.GetMaxOverdraftPurse1Period2(), 2);
		SQLOut.Add(RowAccount.GetMaxOverdraftPurse1Period3(), 2);
		SQLOut.Add(RowAccount.GetMaxOverdraftPurse1Period4(), 2);
		SQLOut.Add(RowAccount.GetMaxOverdraftPurse1Period5(), 2);
		SQLOut.Add(RowAccount.GetMaxOverdraftPurse1Period6(), 2);
		/*****/
		SQLOut.Add(RowAccount.GetMaxOverdraftPurse3Period1(), 2);
		SQLOut.Add(RowAccount.GetMaxOverdraftPurse3Period2(), 2);
		SQLOut.Add(RowAccount.GetMaxOverdraftPurse3Period3(), 2);
		SQLOut.Add(RowAccount.GetMaxOverdraftPurse3Period4(), 2);
		SQLOut.Add(RowAccount.GetMaxOverdraftPurse3Period5(), 2);
		SQLOut.Add(RowAccount.GetMaxOverdraftPurse3Period6(), 2);
		/*****/
		SQLOut.Add(RowAccount.GetAlertCode());
		SQLOut.Add(RowAccount.GetAlertDate());
		SQLOut.Add(RowAccount.GetAlertTime());

		SQLOut.WriteInsert();
		fileOut.WriteLine("");

		WorkingDlg.IncrementRecordsChecked(10, TRUE);
	}
}

//**********************************************************************
