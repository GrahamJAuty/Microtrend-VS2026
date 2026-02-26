//**********************************************************************
#include "..\SPosLoyalty4Shared\SQLNamespaces.h"
//**********************************************************************
#include "ProjectSQLRecordSetAccountFull.h"
//**********************************************************************

CProjectSQLRecordSetAccountFull::CProjectSQLRecordSetAccountFull() : CSQLRecordSetAccountFull(NULL, RSParams_AccountFull_NormalNoParams{})
{
}

//**********************************************************************

void CProjectSQLRecordSetAccountFull::ExportToScript(CSSFile& fileOut, CWorkingDlg& WorkingDlg)
{
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

    CSQLOut SQLOut(fileOut, SQLTableNames::Accounts, arrayHeader);

    SQLOut.WriteDelete();

    CSQLRowAccountFull RowAccount;
    while (StepSelectAll(RowAccount) == TRUE)
    {
        SQLOut.Add(RowAccount.GetUserID());
        SQLOut.Add(RowAccount.GetFullname());
        SQLOut.Add(RowAccount.GetForename());
        SQLOut.Add(RowAccount.GetSurname());
        SQLOut.Add(RowAccount.GetDOB());
        SQLOut.Add(RowAccount.GetExpiryDate());
        SQLOut.Add(RowAccount.GetGender());
        SQLOut.Add(RowAccount.GetGroupNo());
        SQLOut.Add(RowAccount.GetSchemeNo());
        SQLOut.Add(RowAccount.GetInfo1());
        SQLOut.Add(RowAccount.GetInfo2());
        SQLOut.Add(RowAccount.GetInfo3());
        SQLOut.Add(RowAccount.GetInfo4());
        SQLOut.Add(RowAccount.GetAddress1());
        SQLOut.Add(RowAccount.GetAddress2());
        SQLOut.Add(RowAccount.GetAddress3());
        SQLOut.Add(RowAccount.GetAddress4());
        SQLOut.Add(RowAccount.GetAddress5());
        SQLOut.Add(RowAccount.GetPhone1());
        SQLOut.Add(RowAccount.GetPhone2());
        SQLOut.Add(RowAccount.GetInactive());
        /*****/
        SQLOut.Add(RowAccount.GetExternalRef());
        SQLOut.Add(RowAccount.GetQRInfo());
        SQLOut.Add(RowAccount.GetNextUserID());
        SQLOut.Add(RowAccount.GetPreviousUserID());
        /*****/
        SQLOut.Add(RowAccount.GetPurse1Balance(), 2);
        SQLOut.Add(RowAccount.GetPurse1LastSpendDate());
        SQLOut.Add(RowAccount.GetPurse1LastSpendTime());
        SQLOut.Add(RowAccount.GetPurse1LastSpend(), 2);
        SQLOut.Add(RowAccount.GetPurse1SpendToDate(), 2);
        /*****/
        SQLOut.Add(RowAccount.GetPurse2Balance(), 2);
        SQLOut.Add(RowAccount.GetPurse2LastSpendDate());
        SQLOut.Add(RowAccount.GetPurse2LastSpendTime());
        SQLOut.Add(RowAccount.GetPurse2LastSpend(), 2);
        SQLOut.Add(RowAccount.GetPurse2SpendToDate(), 2);
        SQLOut.Add(RowAccount.GetPurse2RefreshedDate());
        SQLOut.Add(RowAccount.GetPurse2RefreshedTime());
        /*****/
        SQLOut.Add(RowAccount.GetCashLastSpendDate());
        SQLOut.Add(RowAccount.GetCashLastSpendTime());
        SQLOut.Add(RowAccount.GetCashLastSpend(), 2);
        SQLOut.Add(RowAccount.GetCashSpendToDate(), 2);
        /*****/
        SQLOut.Add(RowAccount.GetPoints());
        SQLOut.Add(RowAccount.GetPointsToDate());
        /*****/
        SQLOut.Add(RowAccount.GetHotlistCode());
        SQLOut.Add(RowAccount.GetHotlistDate());
        SQLOut.Add(RowAccount.GetHotlistTime());
        /*****/
        SQLOut.Add(RowAccount.GetMaxSpend(), 2);
        SQLOut.Add(RowAccount.GetMaxOverdraft(), 2);
        /*****/
        SQLOut.Add(RowAccount.GetAlertCodes());

        SQLOut.WriteInsert();
        WorkingDlg.IncrementRecordsChecked(10, FALSE);
    }
}

//**********************************************************************
