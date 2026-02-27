/**********************************************************************/
#include "SQLBuildSchema.h"
#include "SQLNamespaces.h"
/**********************************************************************/
#include "SQLBuilderCreateTable.h"
/**********************************************************************/

CSQLCreateFieldInfo::CSQLCreateFieldInfo()
{
	m_strFieldName = "";
	m_nDataType = SQLSRVR_INT32;
	m_nFieldFlags = 0;
	m_nFieldLen1 = 0;
	m_nFieldLen2 = 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CSQLBuilderCreateTable::CSQLBuilderCreateTable()
{ 
	m_bAutoID = FALSE;
	m_nPKFieldCount = 1;
}

/**********************************************************************/

void CSQLBuilderCreateTable::AddFieldInternal(CString strFieldName, int nDataType, int nFlags, int nFieldLen1, int nFieldLen2)
{
	CSQLCreateFieldInfo FieldInfo;
	FieldInfo.m_strFieldName = strFieldName;
	FieldInfo.m_nDataType = nDataType;
	FieldInfo.m_nFieldFlags = nFlags;
	FieldInfo.m_nFieldLen1 = nFieldLen1;
	FieldInfo.m_nFieldLen2 = nFieldLen2;
	m_arrayFieldInfo.Add(FieldInfo);
}

/**********************************************************************/

CString CSQLBuilderCreateTable::GetDataTypeString(CSQLCreateFieldInfo& FieldInfo)
{
	CString strResult = "";

	switch (FieldInfo.m_nDataType)
	{
	case SQLSRVR_BIT:
		strResult = "BIT";
		break;

	case SQLSRVR_INT32:
		strResult = "INT";
		break;

	case SQLSRVR_INT64:
		strResult = "BIGINT";
		break;

	case SQLSRVR_DECIMAL:
		strResult.Format("DECIMAL(%d,%d)", FieldInfo.m_nFieldLen1, FieldInfo.m_nFieldLen2);
		break;

	case SQLSRVR_ROWVERSION:
		strResult = "ROWVERSION";
		break;

	case SQLSRVR_COMPUTED:
		strResult = "";
		break;

	case SQLSRVR_FIXLENTEXT:
		strResult.Format("CHAR(%d)", FieldInfo.m_nFieldLen1);
		break;

	case SQLSRVR_TEXT:
	default:
		strResult.Format("VARCHAR(%d)", FieldInfo.m_nFieldLen1);
		break;
	}

	return strResult;
}

/**********************************************************************/

void CSQLBuilderCreateTable::SetupAccountFields(int nTableType)
{
	m_bAutoID = FALSE;
	m_arrayFieldInfo.RemoveAll();

	switch (nTableType)
	{
	case ACCOUNTS_TABLE_LEAVERS:
		AddInt32(Account::LeaverYear.Label);
		m_nPKFieldCount = 2;
		break;

	case ACCOUNTS_TABLE_CURRENT:
	default:
		m_nPKFieldCount = 1;
		break;
	}

	AddInt64(Account::UserID.Label);
	AddText(Account::Username);
	AddText(Account::Forename);
	AddText(Account::Surname);
	AddText(Account::DOB);
	AddText(Account::ExpiryDate);
	AddText(Account::Gender);
	AddInt32(Account::GroupNo);
	AddText(Account::DayGroups);
	AddInt32(Account::GroupShiftStatus);
	AddText(Account::GroupShiftStartDate);
	AddText(Account::GroupShiftEndDate);
	AddInt32(Account::GroupShiftFutureGroup);
	AddText(Account::Email);
	AddText(Account::Info1, SQLFIELD_FLAG_INDEX);
	AddText(Account::Info2, SQLFIELD_FLAG_INDEX);
	AddText(Account::Info3, SQLFIELD_FLAG_INDEX);
	AddText(Account::Info4, SQLFIELD_FLAG_INDEX);
	AddText(Account::Info5, SQLFIELD_FLAG_INDEX);
	AddText(Account::Info6, SQLFIELD_FLAG_INDEX);
	AddText(Account::Info7, SQLFIELD_FLAG_INDEX);
	AddText(Account::Info8, SQLFIELD_FLAG_INDEX);
	AddBit(Account::Tick1);
	AddBit(Account::Tick2);
	AddBit(Account::Tick3);
	AddBit(Account::Tick4);
	AddInt32(Account::AllergyCodes);
	AddText(Account::Dietary);
	AddBit(Account::Inactive);
	/*****/
	AddText(Account::MemberID, SQLFIELD_FLAG_INDEX);
	AddText(Account::ExternalRef1, SQLFIELD_FLAG_INDEX);
	AddText(Account::ExternalRef2, SQLFIELD_FLAG_INDEX);
	AddInt64(Account::NextUserID);
	AddInt64(Account::PreviousUserID);
	AddText(Account::PebblePurseID);
	/*****/
	AddInt32(Account::Purse1Allow);
	AddDecimal(Account::Purse1Liability);
	AddDecimal(Account::Purse1Credit);
	AddText(Account::Purse1LastSpendDate);
	AddText(Account::Purse1LastSpendTime);
	AddDecimal(Account::Purse1LastSpend);
	AddDecimal(Account::Purse1SpendToDate);
	/*****/
	AddInt32(Account::Purse2Allow);
	AddDecimal(Account::Purse2Balance);
	AddText(Account::Purse2LastSpendDate);
	AddText(Account::Purse2LastSpendTime);
	AddDecimal(Account::Purse2LastSpend);
	AddDecimal(Account::Purse2SpendToDate);
	AddInt32(Account::Purse2RefreshFlags);
	AddText(Account::Purse2RefreshedDate);
	AddText(Account::Purse2RefreshedTime);
	/*****/
	AddInt32(Account::Purse3Allow);
	AddDecimal(Account::Purse3Liability);
	AddDecimal(Account::Purse3Credit);
	AddText(Account::Purse3LastSpendDate);
	AddText(Account::Purse3LastSpendTime);
	AddDecimal(Account::Purse3LastSpend);
	AddDecimal(Account::Purse3SpendToDate);
	/*****/
	AddDecimal(Account::CashSpendToDate);
	/*****/
	AddInt32(Account::Points);
	AddInt32(Account::PointsToDate);
	/*****/
	AddBit(Account::OwnMaxSpend);
	/*****/
	AddDecimal(Account::MaxSpendPurse1Period1);
	AddDecimal(Account::MaxSpendPurse1Period2);
	AddDecimal(Account::MaxSpendPurse1Period3);
	AddDecimal(Account::MaxSpendPurse1Period4);
	AddDecimal(Account::MaxSpendPurse1Period5);
	AddDecimal(Account::MaxSpendPurse1Period6);
	/*****/
	AddDecimal(Account::MaxSpendPurse3Period1);
	AddDecimal(Account::MaxSpendPurse3Period2);
	AddDecimal(Account::MaxSpendPurse3Period3);
	AddDecimal(Account::MaxSpendPurse3Period4);
	AddDecimal(Account::MaxSpendPurse3Period5);
	AddDecimal(Account::MaxSpendPurse3Period6);
	/*****/
	AddDecimal(Account::MaxOverdraftPurse1Period1);
	AddDecimal(Account::MaxOverdraftPurse1Period2);
	AddDecimal(Account::MaxOverdraftPurse1Period3);
	AddDecimal(Account::MaxOverdraftPurse1Period4);
	AddDecimal(Account::MaxOverdraftPurse1Period5);
	AddDecimal(Account::MaxOverdraftPurse1Period6);
	/*****/
	AddDecimal(Account::MaxOverdraftPurse3Period1);
	AddDecimal(Account::MaxOverdraftPurse3Period2);
	AddDecimal(Account::MaxOverdraftPurse3Period3);
	AddDecimal(Account::MaxOverdraftPurse3Period4);
	AddDecimal(Account::MaxOverdraftPurse3Period5);
	AddDecimal(Account::MaxOverdraftPurse3Period6);
	/*****/
	AddInt32(Account::AlertCode);
	AddText(Account::AlertDate);
	AddText(Account::AlertTime);
	/*****/
	AddRowVersion(Account::RowVersion);
	/*****/

	{
		CString strField = "";
		strField.Format("%s AS ([%s] + [%s])",
			Account::Purse1Balance.Label,
			Account::Purse1Liability.Label,
			Account::Purse1Credit.Label);

		AddComputed(strField);
	}

	{
		CString strField = "";
		strField.Format("%s AS ([%s] + [%s])",
			Account::Purse3Balance.Label,
			Account::Purse3Liability.Label,
			Account::Purse3Credit.Label);

		AddComputed(strField);
	}

	{
		CString strField = "";
		strField.Format("%s AS ([%s] + [%s] + [%s] + [%s])",
			Account::TotalSpendToDate.Label,
			Account::Purse1SpendToDate.Label,
			Account::Purse2SpendToDate.Label,
			Account::Purse3SpendToDate.Label,
			Account::CashSpendToDate.Label);

		AddComputed(strField);
	}
}

/**********************************************************************/

CString CSQLBuilderCreateTable::GetCommandCreateAccountTable()
{
	SetupAccountFields(ACCOUNTS_TABLE_CURRENT);
	return BuildCommandString(SQLTableNames::Accounts);
}

/**********************************************************************/

CString CSQLBuilderCreateTable::GetCommandCreateLeaversTable()
{
	SetupAccountFields(ACCOUNTS_TABLE_LEAVERS);
	return BuildCommandString(SQLTableNames::Leavers);
}

/**********************************************************************/

CString CSQLBuilderCreateTable::GetCommandCreateGroupTable()
{
	//VAT SHIFT AND REFRESH APPEND SHOULD BE BITS

	m_nPKFieldCount = 1;
	m_bAutoID = FALSE;

	m_arrayFieldInfo.RemoveAll();

	AddInt32(Group::GroupNo);
	AddText(Group::GroupName);
	AddInt32(Group::UserType);
	AddInt32(Group::SetNo);
	AddText(Group::ShiftGroups);
	AddInt32(Group::PriceLevel);
	AddInt32(Group::VATShift);
	AddBit(Group::PurchaseControlFlag);
	AddBit(Group::AllowRevalKioskFlag);
	/*****/
	AddInt32(Group::RefreshType);
	AddInt32(Group::RefreshFlags);
	AddDecimal(Group::RefreshValue);
	AddInt32(Group::RefreshAppend);
	AddInt32(Group::RefreshTypePeriod1);
	AddInt32(Group::RefreshTypePeriod2);
	AddInt32(Group::RefreshTypePeriod3);
	AddInt32(Group::RefreshTypePeriod4);
	AddInt32(Group::RefreshTypePeriod5);
	AddInt32(Group::RefreshTypePeriod6);
	AddDecimal(Group::RefreshValuePeriod1);
	AddDecimal(Group::RefreshValuePeriod2);
	AddDecimal(Group::RefreshValuePeriod3);
	AddDecimal(Group::RefreshValuePeriod4);
	AddDecimal(Group::RefreshValuePeriod5);
	AddDecimal(Group::RefreshValuePeriod6);
	/*****/
	AddInt32(Group::Purse1Allowed);
	AddInt32(Group::Purse2Allowed);
	AddInt32(Group::Purse3Allowed);
	/*****/
	AddInt32(Group::MaxSpendType);
	/*****/
	AddDecimal(Group::MaxSpendPurse1Period1);
	AddDecimal(Group::MaxSpendPurse1Period2);
	AddDecimal(Group::MaxSpendPurse1Period3);
	AddDecimal(Group::MaxSpendPurse1Period4);
	AddDecimal(Group::MaxSpendPurse1Period5);
	AddDecimal(Group::MaxSpendPurse1Period6);
	/*****/
	AddDecimal(Group::MaxSpendPurse3Period1);
	AddDecimal(Group::MaxSpendPurse3Period2);
	AddDecimal(Group::MaxSpendPurse3Period3);
	AddDecimal(Group::MaxSpendPurse3Period4);
	AddDecimal(Group::MaxSpendPurse3Period5);
	AddDecimal(Group::MaxSpendPurse3Period6);
	/*****/
	AddDecimal(Group::MaxOverdraftPurse1Period1);
	AddDecimal(Group::MaxOverdraftPurse1Period2);
	AddDecimal(Group::MaxOverdraftPurse1Period3);
	AddDecimal(Group::MaxOverdraftPurse1Period4);
	AddDecimal(Group::MaxOverdraftPurse1Period5);
	AddDecimal(Group::MaxOverdraftPurse1Period6);
	/*****/
	AddDecimal(Group::MaxOverdraftPurse3Period1);
	AddDecimal(Group::MaxOverdraftPurse3Period2);
	AddDecimal(Group::MaxOverdraftPurse3Period3);
	AddDecimal(Group::MaxOverdraftPurse3Period4);
	AddDecimal(Group::MaxOverdraftPurse3Period5);
	AddDecimal(Group::MaxOverdraftPurse3Period6);
	/*****/
	AddText(Group::DeptInhibitPurse1);
	AddText(Group::DeptInhibitPurse2);
	AddText(Group::DeptInhibitPurse3);
	AddText(Group::DeptInhibitCash);
	/*****/
	AddDecimal(Group::LowBalancePurse1);
	AddDecimal(Group::LowBalancePurse2);
	AddDecimal(Group::LowBalancePurse3);
	/*****/
	AddInt32(Group::PointsAchievement);
	/*****/
	AddBit(Group::RevalueFlag);
	AddBit(Group::RevalueDisable);
	AddInt32(Group::RevalueType);
	AddDecimal(Group::RevalueTrip1);
	AddDecimal(Group::RevalueTrip2);
	AddInt32(Group::RevaluePoints);
	AddDecimal(Group::RevalueBonus);
	/*****/
	AddBit(Group::RewardFlag);
	AddBit(Group::RewardDisable);
	AddInt32(Group::RewardType);
	AddDecimal(Group::RewardTrip);
	AddInt32(Group::RewardPoints);
	AddDecimal(Group::RewardBonus);
	AddDecimal(Group::RewardLimit);
	AddDecimal(Group::RewardTrigger);
	/*****/
	AddBit(Group::RedeemFlag);
	AddBit(Group::RedeemDisable);
	AddDecimal(Group::RedeemTripAsDouble);
	AddDecimal(Group::RedeemValue);
	AddInt32(Group::RedeemPurse);

	return BuildCommandString(SQLTableNames::AccountGroups);
}

/**********************************************************************/

CString CSQLBuilderCreateTable::GetCommandCreateVersionTable()
{
	m_nPKFieldCount = 1;
	m_bAutoID = FALSE;

	m_arrayFieldInfo.RemoveAll();

	AddInt32(DbVersion::LineID);
	AddInt32(DbVersion::Version);
	AddInt32(DbVersion::ExternalVersion1);

	return BuildCommandString(SQLTableNames::DbVersion);
}

/**********************************************************************/

CString CSQLBuilderCreateTable::GetCommandCreateExternalAccountTable()
{
	m_nPKFieldCount = 1;
	m_bAutoID = FALSE;

	m_arrayFieldInfo.RemoveAll();

	AddText(ExternalAccount::Username);	//EXTERNAL USER NAME
	AddText(ExternalAccount::Password);	//PASSWORD
	AddText(ExternalAccount::UserID);	//USER ID

	return BuildCommandString(SQLTableNames::ExternalAccounts);
}

/**********************************************************************/

CString CSQLBuilderCreateTable::GetCommandCreatePeriodRefreshTable()
{
	m_nPKFieldCount = 1;
	m_bAutoID = FALSE;

	m_arrayFieldInfo.RemoveAll();

	AddInt64(PeriodRefresh::UserID);	
	AddText(PeriodRefresh::Period1Date);	
	AddDecimal(PeriodRefresh::Period1Balance);
	AddText(PeriodRefresh::Period2Date);
	AddDecimal(PeriodRefresh::Period2Balance);
	AddText(PeriodRefresh::Period3Date);
	AddDecimal(PeriodRefresh::Period3Balance);
	AddText(PeriodRefresh::Period4Date);
	AddDecimal(PeriodRefresh::Period4Balance);
	AddText(PeriodRefresh::Period5Date);
	AddDecimal(PeriodRefresh::Period5Balance);
	AddText(PeriodRefresh::Period6Date);
	AddDecimal(PeriodRefresh::Period6Balance);

	return BuildCommandString(SQLTableNames::PeriodRefresh);
}

/**********************************************************************/

CString CSQLBuilderCreateTable::GetCommandCreateBioRegisterTable()
{
	m_nPKFieldCount = 1;
	m_bAutoID = FALSE;

	m_arrayFieldInfo.RemoveAll();

	AddInt64(BioRegister::UserID);
	AddText(BioRegister::Username);
	AddInt32(BioRegister::Counter);
	AddText(BioRegister::RegDate);
	AddText(BioRegister::RegTime);
	AddText(BioRegister::UnRegDate);
	AddText(BioRegister::UnRegTime);
	AddBit(BioRegister::UnRegFlag);
	AddText(BioRegister::FirstDate);
	AddText(BioRegister::FirstTime);
	AddText(BioRegister::LastDate);
	AddText(BioRegister::LastTime);

	return BuildCommandString(SQLTableNames::BiometricRegister);
}

/**********************************************************************/

CString CSQLBuilderCreateTable::GetCommandCreatePluInfoTable()
{
	m_nPKFieldCount = 1;
	m_bAutoID = FALSE;

	m_arrayFieldInfo.RemoveAll();

	AddInt64(PluInfo::PluNo);
	AddText(PluInfo::Description);
	AddInt32(PluInfo::DeptNo);
	AddText(PluInfo::TaxCode);
	AddInt32(PluInfo::ModType);
	AddInt32(PluInfo::Points);
	AddInt32(PluInfo::PCtrlRuleID);
	AddInt32(PluInfo::StampWeight);

	return BuildCommandString(SQLTableNames::PluInfo);
}

/**********************************************************************/

CString CSQLBuilderCreateTable::GetCommandCreateAuditPeriodTable()
{
	m_nPKFieldCount = 1;
	m_bAutoID = FALSE;

	m_arrayFieldInfo.RemoveAll();

	AddInt32(AuditPeriod::PeriodID);
	AddText(AuditPeriod::PeriodName);
	AddInt32(AuditPeriod::StartTime);
	
	return BuildCommandString(SQLTableNames::AuditPeriods);
}

/**********************************************************************/

CString CSQLBuilderCreateTable::GetCommandCreateUserTextTable()
{
	m_nPKFieldCount = 1;
	m_bAutoID = FALSE;

	m_arrayFieldInfo.RemoveAll();

	AddText(UserText::FieldLabel);
	AddText(UserText::UserText);
	AddBit(UserText::UseFlag);

	return BuildCommandString(SQLTableNames::UserTexts);
}

/**********************************************************************/

CString CSQLBuilderCreateTable::GetCommandCreateClosingBalanceTable()
{
	m_nPKFieldCount = 1;
	m_bAutoID = FALSE;

	m_arrayFieldInfo.RemoveAll();

	AddText(ClosingBalance::KeyDate);
	AddText(ClosingBalance::Date);
	AddText(ClosingBalance::Time);
	AddText(ClosingBalance::ArchiveDate);
	AddText(ClosingBalance::ArchiveTime);
	AddDecimal(ClosingBalance::Purse1LiabilityAudit);
	AddDecimal(ClosingBalance::Purse1CreditAudit);
	AddDecimal(ClosingBalance::Purse2BalanceAudit);
	AddDecimal(ClosingBalance::Purse3LiabilityAudit);
	AddDecimal(ClosingBalance::Purse3CreditAudit);
	AddBit(ClosingBalance::HaveAdjust);
	AddDecimal(ClosingBalance::Purse1LiabilityAdjust);
	AddDecimal(ClosingBalance::Purse1CreditAdjust);
	AddDecimal(ClosingBalance::Purse3LiabilityAdjust);
	AddDecimal(ClosingBalance::Purse3CreditAdjust);

	return BuildCommandString(SQLTableNames::ClosingBalances);
}

/**********************************************************************/

CString CSQLBuilderCreateTable::GetCommandCreateBromComPendingOrderTable()
{
	m_nPKFieldCount = 1;
	m_bAutoID = FALSE;

	m_arrayFieldInfo.RemoveAll();
	AddInt32(BromComPendingOrder::OrderID);
	AddText(BromComPendingOrder::Status);
	AddText(BromComPendingOrder::LastModified);
	AddBit(BromComPendingOrder::NewOrder);

	return BuildCommandString(SQLTableNames::BromComPendingOrders);
}

/**********************************************************************/

CString CSQLBuilderCreateTable::GetCommandCreateBromComPendingItemTable()
{
	m_nPKFieldCount = 2;
	m_bAutoID = FALSE;

	m_arrayFieldInfo.RemoveAll();
	AddInt32(BromComPendingItem::OrderID);
	AddInt32(BromComPendingItem::PersonID);
	AddDecimal(BromComPendingItem::Amount);

	return BuildCommandString(SQLTableNames::BromComPendingItems);
}

/**********************************************************************/

CString CSQLBuilderCreateTable::GetCommandCreateBromComItemTable()
{
	m_nPKFieldCount = 2;
	m_bAutoID = FALSE;

	m_arrayFieldInfo.RemoveAll();
	AddInt32(BromComItem::OrderID);
	AddInt32(BromComItem::PersonID);
	AddDecimal(BromComItem::Amount);
	AddText(BromComItem::Status);

	return BuildCommandString(SQLTableNames::BromComItems);
}

/**********************************************************************/

CString CSQLBuilderCreateTable::GetCommandCreateBromComBalanceChangesTable()
{
	m_nPKFieldCount = 1;
	m_bAutoID = FALSE;

	m_arrayFieldInfo.RemoveAll();
	AddInt32(BromComBalanceChanges::PersonID);
	AddInt64(BromComBalanceChanges::AccountID);

	return BuildCommandString(SQLTableNames::BromComBalanceChanges);
}

/**********************************************************************/

CString CSQLBuilderCreateTable::GetCommandCreateBromComTransactionTable()
{
	m_nPKFieldCount = 1;
	m_bAutoID = TRUE;

	m_arrayFieldInfo.RemoveAll();
	AddInt64(BromComTransaction::LineID);
	AddInt32(BromComTransaction::PersonID);
	AddText(BromComTransaction::Details);
	AddDecimal(BromComTransaction::Amount);
	AddDecimal(BromComTransaction::Balance);
	AddText(BromComTransaction::DateTime);
	AddText(BromComTransaction::Type);
	AddInt32(BromComTransaction::TransactionID);
	AddText(BromComTransaction::WebComment);

	return BuildCommandString(SQLTableNames::BromComTransactions);
}

/**********************************************************************/

CString CSQLBuilderCreateTable::GetCommandCreatePurchaseControlRuleTable()
{
	m_nPKFieldCount = 1;
	m_bAutoID = FALSE;

	m_arrayFieldInfo.RemoveAll();
	AddInt32(PurchaseControlRule::RuleID.Label);
	AddBit(PurchaseControlRule::Enabled.Label);
	AddInt32(PurchaseControlRule::Threshold.Label);
	AddInt32(PurchaseControlRule::Expiry.Label);
	AddText(PurchaseControlRule::Description.Label, PurchaseControlRule::Description.Max);

	return BuildCommandString(SQLTableNames::PurchaseControlRules);
}

/**********************************************************************/

CString CSQLBuilderCreateTable::GetCommandCreatePurchaseControlStampTable()
{
	m_nPKFieldCount = 4;
	m_bAutoID = FALSE;

	m_arrayFieldInfo.RemoveAll();
	AddInt64(PurchaseControlStamp::UserID.Label);
	AddInt32(PurchaseControlStamp::RuleID.Label);
	AddText(PurchaseControlStamp::AwardDate.Label, PurchaseControlStamp::AwardDate.Max);
	AddText(PurchaseControlStamp::ExpireDate.Label, PurchaseControlStamp::ExpireDate.Max);
	AddInt32(PurchaseControlStamp::StampCount.Label);

	return BuildCommandString(SQLTableNames::PurchaseControlStamps);
}

/**********************************************************************/

CString CSQLBuilderCreateTable::GetCommandCreateImportFilenameCacheTable()
{
	m_nPKFieldCount = 1;
	m_bAutoID = TRUE;

	m_arrayFieldInfo.RemoveAll();
	AddInt64(ImportFilenameCache::LineID.Label);
	AddInt32(ImportFilenameCache::Type.Label);
	AddText(ImportFilenameCache::Filename.Label, ImportFilenameCache::Filename.Max);
	AddText(ImportFilenameCache::DateTime.Label, ImportFilenameCache::DateTime.Max);
	
	return BuildCommandString(SQLTableNames::ImportFilenameCache);
}

/**********************************************************************/

CString CSQLBuilderCreateTable::GetCommandCreatePebbleConfigTable(bool bOriginal)
{
	m_nPKFieldCount = 1;
	m_bAutoID = FALSE;

	m_arrayFieldInfo.RemoveAll();
	AddInt32(PebbleConfig::LineID.Label);
	AddText(PebbleConfig::InstallationID.Label, PebbleConfig::InstallationID.Max);
	AddText(PebbleConfig::OrganisationID.Label, PebbleConfig::OrganisationID.Max);
	AddInt32(PebbleConfig::MISInfoIndex.Label);
	AddInt32(PebbleConfig::UPNInfoIndex.Label);
	AddText(PebbleConfig::LastImportDateTopup.Label, PebbleConfig::LastImportDateTopup.Max);
	AddText(PebbleConfig::LastImportTimeTopup.Label, PebbleConfig::LastImportTimeTopup.Max);
	AddText(PebbleConfig::LastImportDateSale.Label, PebbleConfig::LastImportDateSale.Max);
	AddText(PebbleConfig::LastImportTimeSale.Label, PebbleConfig::LastImportTimeSale.Max);
	AddBit(PebbleConfig::FirstImportTopup.Label);
	AddBit(PebbleConfig::FirstImportSale.Label);
	AddInt32(PebbleConfig::RecentCacheDays.Label);
	AddInt32(PebbleConfig::PaymentCacheDays.Label);
	AddDecimal(PebbleConfig::VAT_A.Label);
	AddDecimal(PebbleConfig::VAT_B.Label);
	AddDecimal(PebbleConfig::VAT_C.Label);
	AddDecimal(PebbleConfig::VAT_D.Label);
	AddDecimal(PebbleConfig::VAT_E.Label);
	AddDecimal(PebbleConfig::VAT_F.Label);
	AddDecimal(PebbleConfig::VAT_G.Label);
	AddDecimal(PebbleConfig::VAT_H.Label);
	AddDecimal(PebbleConfig::VAT_I.Label);
	AddDecimal(PebbleConfig::VAT_J.Label);
	AddDecimal(PebbleConfig::VAT_K.Label);
	AddDecimal(PebbleConfig::VAT_L.Label);

	if (FALSE == bOriginal)
	{
		AddInt32(PebbleConfig::VATCODE_A.Label);
		AddInt32(PebbleConfig::VATCODE_B.Label);
		AddInt32(PebbleConfig::VATCODE_C.Label);
		AddInt32(PebbleConfig::VATCODE_D.Label);
		AddInt32(PebbleConfig::VATCODE_E.Label);
		AddInt32(PebbleConfig::VATCODE_F.Label);
		AddInt32(PebbleConfig::VATCODE_G.Label);
		AddInt32(PebbleConfig::VATCODE_H.Label);
		AddInt32(PebbleConfig::VATCODE_I.Label);
		AddInt32(PebbleConfig::VATCODE_J.Label);
		AddInt32(PebbleConfig::VATCODE_K.Label);
		AddInt32(PebbleConfig::VATCODE_L.Label);
	}

	AddInt64(PebbleConfig::OfficeMealPluNo.Label);
	AddInt32(PebbleConfig::OfficeMealVATBand.Label);

	return BuildCommandString(SQLTableNames::PebbleConfig);
}

/**********************************************************************/

CString CSQLBuilderCreateTable::GetCommandCreatePebblePendingTransactionTable(bool bUnmatched)
{
	m_nPKFieldCount = 1;
	m_bAutoID = FALSE;

	m_arrayFieldInfo.RemoveAll();
	AddText(PebblePendingTransaction::TransactionID.Label, PebblePendingTransaction::TransactionID.Max);
	AddText(PebblePendingTransaction::MemberID.Label, PebblePendingTransaction::MemberID.Max);
	AddText(PebblePendingTransaction::DateTime.Label, PebblePendingTransaction::DateTime.Max);
	AddText(PebblePendingTransaction::Name.Label, PebblePendingTransaction::Name.Max);
	AddInt32(PebblePendingTransaction::Amount.Label);
	AddBit(PebblePendingTransaction::IsSale.Label);

	if (FALSE == bUnmatched)
	{
		return BuildCommandString(SQLTableNames::PebblePendingTransactions);
	}
	else
	{
		return BuildCommandString(SQLTableNames::PebbleUnmatchedTransactions);
	}
}

/**********************************************************************/

CString CSQLBuilderCreateTable::GetCommandCreatePebbleRecentTransactionTable()
{
	m_nPKFieldCount = 1;
	m_bAutoID = FALSE;

	m_arrayFieldInfo.RemoveAll();
	AddText(PebbleRecentTransaction::TransactionID.Label, PebbleRecentTransaction::TransactionID.Max);
	AddText(PebbleRecentTransaction::DateTime.Label, PebbleRecentTransaction::DateTime.Max);
	
	return BuildCommandString(SQLTableNames::PebbleRecentTransactions);
}

/**********************************************************************/

CString CSQLBuilderCreateTable::GetCommandCreatePebbleExportTransactionTable(bool bOriginal)
{
	m_nPKFieldCount = 1;
	m_bAutoID = TRUE;

	m_arrayFieldInfo.RemoveAll();
	AddInt64(PebbleExportTransaction::LineID.Label);
	AddInt64(PebbleExportTransaction::UserID.Label);

	if (FALSE == bOriginal)
	{
		AddInt64(PebbleExportTransaction::XFerUserID.Label);
	}

	AddText(PebbleExportTransaction::MemberID.Label, PebbleExportTransaction::MemberID.Max);
	AddText(PebbleExportTransaction::Date.Label, PebbleExportTransaction::Date.Max);
	AddText(PebbleExportTransaction::Time.Label, PebbleExportTransaction::Time.Max);
	AddInt32(PebbleExportTransaction::CCNo.Label);
	AddInt32(PebbleExportTransaction::SourceType.Label);
	AddInt32(PebbleExportTransaction::AppNo.Label);
	AddDecimal(PebbleExportTransaction::Amount.Label);
	AddDecimal(PebbleExportTransaction::Purse1.Label);
	AddDecimal(PebbleExportTransaction::Purse2.Label);
	AddDecimal(PebbleExportTransaction::Cash.Label);
	AddText(PebbleExportTransaction::WebComment.Label, PebbleExportTransaction::WebComment.Max);

	return BuildCommandString(SQLTableNames::PebbleExportTransactions);
}

/**********************************************************************/

CString CSQLBuilderCreateTable::GetCommandCreatePebbleExportTransactionSentTable()
{
	m_nPKFieldCount = 1;
	m_bAutoID = TRUE;

	m_arrayFieldInfo.RemoveAll();
	AddInt64(PebbleExportTransactionSent::LineID.Label);
	AddInt64(PebbleExportTransactionSent::TranLineID.Label);

	return BuildCommandString(SQLTableNames::PebbleExportTransactionsSent);
}

/**********************************************************************/

CString CSQLBuilderCreateTable::GetCommandCreatePebbleExportPaymentTable()
{
	m_nPKFieldCount = 4;
	m_bAutoID = FALSE;

	m_arrayFieldInfo.RemoveAll();
	AddInt64(PebbleExportPayment::UserID.Label);
	AddText(PebbleExportPayment::Date.Label, PebbleExportPayment::Date.Max);
	AddText(PebbleExportPayment::Time.Label, PebbleExportPayment::Time.Max);
	AddText(PebbleExportPayment::CreateDateTime.Label, PebbleExportPayment::CreateDateTime.Max);
	AddInt32(PebbleExportPayment::CCNo.Label);
	AddInt32(PebbleExportPayment::PaymentType1.Label);
	AddDecimal(PebbleExportPayment::PaymentAmount1.Label);
	AddInt32(PebbleExportPayment::PaymentType2.Label);
	AddDecimal(PebbleExportPayment::PaymentAmount2.Label);
	AddInt32(PebbleExportPayment::PaymentType3.Label);
	AddDecimal(PebbleExportPayment::PaymentAmount3.Label);
	AddInt32(PebbleExportPayment::PaymentType4.Label);
	AddDecimal(PebbleExportPayment::PaymentAmount4.Label);
	AddInt32(PebbleExportPayment::PaymentType5.Label);
	AddDecimal(PebbleExportPayment::PaymentAmount5.Label);

	return BuildCommandString(SQLTableNames::PebbleExportPayments);
}

/**********************************************************************/

CString CSQLBuilderCreateTable::GetCommandCreateChartwellExportPaymentTable()
{
	m_nPKFieldCount = 4;
	m_bAutoID = FALSE;

	m_arrayFieldInfo.RemoveAll();
	AddInt64(ChartwellExportPayment::UserID.Label);
	AddInt64(ChartwellExportPayment::TransactionID.Label);
	AddInt32(ChartwellExportPayment::TerminalNo.Label);
	AddText(ChartwellExportPayment::Date.Label, ChartwellExportPayment::Date.Max);
	AddText(ChartwellExportPayment::CreateDateTime.Label, ChartwellExportPayment::CreateDateTime.Max);
	AddInt32(ChartwellExportPayment::GroupNo.Label);
	AddDecimal(ChartwellExportPayment::PaymentPurse1.Label);
	AddDecimal(ChartwellExportPayment::PaymentPurse2.Label);
	AddDecimal(ChartwellExportPayment::PaymentPurse3.Label);
	
	return BuildCommandString(SQLTableNames::ChartwellExportPayments);
}

/**********************************************************************/

CString CSQLBuilderCreateTable::GetCommandCreatePebblePaymentNameTable()
{
	m_nPKFieldCount = 1;
	m_bAutoID = FALSE;

	m_arrayFieldInfo.RemoveAll();
	AddInt32(PebblePaymentType::SPOSPaymentType.Label);
	AddText(PebblePaymentType::PebblePaymentName.Label, PebblePaymentType::PebblePaymentName.Max);
	AddInt32(PebblePaymentType::PebbleTranType.Label);

	return BuildCommandString(SQLTableNames::PebblePaymentTypes);
}

/**********************************************************************/

void CSQLBuilderCreateTable::AddPebblePaymentNames(CStringArray& arrayCommands)
{
	CString strRoot = "";
	strRoot.Format("INSERT INTO %s (%s, %s, %s) VALUES(",
		SQLTableNames::PebblePaymentTypes,
		PebblePaymentType::SPOSPaymentType.Label,
		PebblePaymentType::PebblePaymentName.Label,
		PebblePaymentType::PebbleTranType.Label);

	arrayCommands.Add(strRoot + "0, 'CASH', 0)");
	arrayCommands.Add(strRoot + "1, 'CHEQUE', 1)");
	arrayCommands.Add(strRoot + "2, 'CARD', 2)");
	arrayCommands.Add(strRoot + "3, 'CARD', 2)");
	arrayCommands.Add(strRoot + "6, 'ACCOUNT', 2)");
	arrayCommands.Add(strRoot + "9, 'CARD', 2)");
}

/**********************************************************************/

CString CSQLBuilderCreateTable::GetCommandCreatePebbleLinkTimeTable()
{
	m_nPKFieldCount = 1;
	m_bAutoID = TRUE;

	m_arrayFieldInfo.RemoveAll();
	AddInt64(PebbleLinkTime::LineID.Label);
	AddText(PebbleLinkTime::Date.Label, PebbleLinkTime::Date.Max);          // New date column
	AddText(PebbleLinkTime::Time.Label, PebbleLinkTime::Time.Max);          // New time column
	AddInt64(PebbleLinkTime::UserID.Label);
	AddText(PebbleLinkTime::Username.Label, PebbleLinkTime::Username.Max);
	AddText(PebbleLinkTime::Forename.Label, PebbleLinkTime::Forename.Max);
	AddText(PebbleLinkTime::Surname.Label, PebbleLinkTime::Surname.Max);
	AddInt32(PebbleLinkTime::GroupNo.Label);
	AddText(PebbleLinkTime::PebbleFirstName.Label, PebbleLinkTime::PebbleFirstName.Max);
	AddText(PebbleLinkTime::PebbleLastName.Label, PebbleLinkTime::PebbleLastName.Max);
	AddInt32(PebbleLinkTime::ProcessType.Label);
	AddInt32(PebbleLinkTime::MatchType.Label);
	AddText(PebbleLinkTime::MIS.Label, PebbleLinkTime::MIS.Max);
	AddText(PebbleLinkTime::UPN.Label, PebbleLinkTime::UPN.Max);
			
	return BuildCommandString(SQLTableNames::PebbleLinkTimes);
}

/**********************************************************************/

/*
CString CSQLBuilderCreateTable::GetCommandCreateOperatorNameTable()
{
	m_nPKFieldCount = 1;
	m_bAutoID = FALSE;

	m_arrayFieldInfo.RemoveAll();
	AddInt32(OperatorName::OperatorNo.Label);
	AddText(OperatorName::OperatorName.Label, OperatorName::OperatorName.Max);
	
	return BuildCommandString(SQLTableNames::OperatorNames);
}
*/

/**********************************************************************/

CString CSQLBuilderCreateTable::GetCommandCreateEposTerminalTable()
{
	m_nPKFieldCount = 1;
	m_bAutoID = FALSE;

	m_arrayFieldInfo.RemoveAll();
	AddInt32(EposTerminal::TerminalNo.Label);
	AddInt32(EposTerminal::TerminalType.Label);
	AddText(EposTerminal::TerminalName.Label, EposTerminal::TerminalName.Max);
	AddText(EposTerminal::LastUsedDate.Label, EposTerminal::LastUsedDate.Max);
	AddText(EposTerminal::LastUsedTime.Label, EposTerminal::LastUsedTime.Max);
	AddInt32(EposTerminal::CCNo.Label);
	AddInt64(EposTerminal::UserID.Label);
	AddDecimal(EposTerminal::CardValue.Label);
	AddInt32(EposTerminal::TotalCounter.Label);
	AddDecimal(EposTerminal::TotalInDrawer.Label);
	AddBit(EposTerminal::Disabled.Label);
	AddBit(EposTerminal::TerminalOn.Label);
	AddBit(EposTerminal::InService.Label);
	AddBit(EposTerminal::DoorOpen.Label);

	return BuildCommandString(SQLTableNames::EposTerminals);
}

/**********************************************************************/

CString CSQLBuilderCreateTable::GetCommandCreateWebPaymentOptionTable()
{
	m_nPKFieldCount = 1;
	m_bAutoID = FALSE;

	m_arrayFieldInfo.RemoveAll();
	AddInt32(WebPaymentOption::LineID.Label);
	AddInt32(WebPaymentOption::AccountLinkDays.Label);
	AddText(WebPaymentOption::AccountLinkStart.Label, WebPaymentOption::AccountLinkStart.Max);
	AddText(WebPaymentOption::AccountLinkEnd.Label, WebPaymentOption::AccountLinkEnd.Max);
	AddText(WebPaymentOption::AccountLinkLastDate.Label, WebPaymentOption::AccountLinkLastDate.Max);
	AddText(WebPaymentOption::AccountLinkLastTime.Label, WebPaymentOption::AccountLinkLastTime.Max);
	AddInt32(WebPaymentOption::PaymentDays.Label);
	AddText(WebPaymentOption::PaymentStart.Label, WebPaymentOption::PaymentStart.Max);
	AddText(WebPaymentOption::PaymentEnd.Label, WebPaymentOption::PaymentEnd.Max);
	AddText(WebPaymentOption::PaymentLastDate.Label, WebPaymentOption::PaymentLastDate.Max);
	AddText(WebPaymentOption::PaymentLastTime.Label, WebPaymentOption::PaymentLastTime.Max);
	AddText(WebPaymentOption::BalanceLastDate.Label, WebPaymentOption::BalanceLastDate.Max);
	AddText(WebPaymentOption::BalanceLastTime.Label, WebPaymentOption::BalanceLastTime.Max);
	AddText(WebPaymentOption::TransactionLastDate.Label, WebPaymentOption::TransactionLastDate.Max);
	AddText(WebPaymentOption::TransactionLastTime.Label, WebPaymentOption::TransactionLastTime.Max);
	AddBit(WebPaymentOption::Exclude1Flag.Label);
	AddText(WebPaymentOption::Exclude1Start.Label, WebPaymentOption::Exclude1Start.Max);
	AddText(WebPaymentOption::Exclude1End.Label, WebPaymentOption::Exclude1End.Max);
	AddBit(WebPaymentOption::Exclude2Flag.Label);
	AddText(WebPaymentOption::Exclude2Start.Label, WebPaymentOption::Exclude2Start.Max);
	AddText(WebPaymentOption::Exclude2End.Label, WebPaymentOption::Exclude2End.Max);
	AddBit(WebPaymentOption::Exclude3Flag.Label);
	AddText(WebPaymentOption::Exclude3Start.Label, WebPaymentOption::Exclude3Start.Max);
	AddText(WebPaymentOption::Exclude3End.Label, WebPaymentOption::Exclude3End.Max);
	AddBit(WebPaymentOption::Exclude4Flag.Label);
	AddText(WebPaymentOption::Exclude4Start.Label, WebPaymentOption::Exclude4Start.Max);
	AddText(WebPaymentOption::Exclude4End.Label, WebPaymentOption::Exclude4End.Max);
	AddText(WebPaymentOption::ExcludeUploadStart.Label, WebPaymentOption::ExcludeUploadStart.Max);
	AddText(WebPaymentOption::ExcludeUploadEnd.Label, WebPaymentOption::ExcludeUploadEnd.Max);
	AddBit(WebPaymentOption::EODAfterPaymentFlag.Label);
	AddText(WebPaymentOption::EODAfterTime.Label, WebPaymentOption::EODAfterTime.Max);
	AddBit(WebPaymentOption::PromptBeforeAccessFlag.Label);
	AddBit(WebPaymentOption::ImportAccountsFlag.Label);
	AddRowVersion(WebPaymentOption::RowVersion);

	return BuildCommandString(SQLTableNames::WebPaymentOptions);
}

/**********************************************************************/

CString CSQLBuilderCreateTable::GetCommandCreatePurchaseHistoryHeaderTable()
{
	m_nPKFieldCount = 1;
	m_bAutoID = TRUE;

	m_arrayFieldInfo.RemoveAll();
	AddInt64(PurchaseHistoryHeader::LineID.Label);
	AddText(PurchaseHistoryHeader::Date.Label, PurchaseHistoryHeader::Date.Max);          // New date column
	AddText(PurchaseHistoryHeader::Time.Label, PurchaseHistoryHeader::Time.Max);          // New time column

	return BuildCommandString(SQLTableNames::PurchaseHistoryHeader);
}

/**********************************************************************/

CString CSQLBuilderCreateTable::GetCommandCreatePINNumberTable()
{
	m_nPKFieldCount = 1;
	m_bAutoID = FALSE;

	m_arrayFieldInfo.RemoveAll();
	AddFixLenText(PINNumber::PINNumber.Label, PINNumber::PINNumber.Max);
	AddText(PINNumber::UserID.Label,PINNumber::UserID.Max);
	AddText(PINNumber::Username.Label, PINNumber::Username.Max);
	AddText(PINNumber::FirstDate.Label, PINNumber::FirstDate.Max);
	AddText(PINNumber::FirstTime.Label, PINNumber::FirstTime.Max);
	AddText(PINNumber::LastDate.Label, PINNumber::LastDate.Max);
	AddText(PINNumber::LastTime.Label, PINNumber::LastTime.Max);
	AddInt32(PINNumber::UseCounter.Label);
	return BuildCommandString(SQLTableNames::PINNumber);
}

/**********************************************************************/

CString CSQLBuilderCreateTable::GetCommandCreatePINNumberIndex()
{
	CString strIndex = "";
	strIndex.Format("CREATE UNIQUE INDEX idx_%s_%s ON PINNumber(%s) WHERE %s IS NOT NULL",
		SQLTableNames::PINNumber,
		PINNumber::UserID.Label,
		PINNumber::UserID.Label,
		PINNumber::UserID.Label);

	return strIndex;
}

/**********************************************************************/

CString CSQLBuilderCreateTable::GetCommandCreatePINNumberConstraint()
{
	CString strCheck = ""; 
	strCheck.Format("ALTER TABLE %s ADD CONSTRAINT CK_%s_PIN_Exactly4Digits CHECK (LEN(%s) = 4 AND %s NOT LIKE '%%[^0-9]%%')", 
		SQLTableNames::PINNumber, 
		SQLTableNames::PINNumber, 
		PINNumber::PINNumber.Label, 
		PINNumber::PINNumber.Label);

	return strCheck;
}

/**********************************************************************/

CString CSQLBuilderCreateTable::GetCommandCreateCCNoTable()
{
	m_nPKFieldCount = 1;
	m_bAutoID = FALSE;

	m_arrayFieldInfo.RemoveAll();
	AddText(CCNo::Type);
	AddInt32(CCNo::Number);

	return BuildCommandString(SQLTableNames::CCNo);
}

/**********************************************************************/

CString CSQLBuilderCreateTable::BuildCommandString(CString strTableName, bool bIfNotExists)
{
	CString strCommand = "";
	strCommand += "CREATE TABLE ";

	/*
	if (TRUE == bIfNotExists)
	{
		strCommand += "IF NOT EXISTS ";
	}
	*/

	strCommand += strTableName;
	strCommand += " (";

	CArray<int, int> arrayIndexColumns;

	for (int n = 0; n < m_arrayFieldInfo.GetSize(); n++)
	{
		if (n != 0)
		{
			strCommand += ",";
		}

		strCommand += m_arrayFieldInfo[n].m_strFieldName;
		strCommand += " ";
		strCommand += GetDataTypeString(m_arrayFieldInfo[n]);

		if (0 == n)
		{
			if (1 == m_nPKFieldCount)
			{
				strCommand += " PRIMARY KEY";

				if (TRUE == m_bAutoID)
				{
					strCommand += " IDENTITY";
				}
			}
		}
		else if ((m_arrayFieldInfo[n].m_nFieldFlags & SQLFIELD_FLAG_INDEX) != 0)
		{
			arrayIndexColumns.Add(n);
		}

		bool bNotNull = TRUE;
		switch (m_arrayFieldInfo[n].m_nDataType)
		{
		case SQLSRVR_TEXT:
		case SQLSRVR_FIXLENTEXT:
		case SQLSRVR_COMPUTED:
			bNotNull = (0 == n);
			break;

		case SQLSRVR_ROWVERSION:
			bNotNull = FALSE;
			break;

		case SQLSRVR_INT32:
		case SQLSRVR_INT64:
			bNotNull = ((m_arrayFieldInfo[n].m_nFieldFlags & SQLFIELD_FLAG_NULLABLEINT) == 0);
			break;
		}

		if (TRUE == bNotNull)
		{
			strCommand += " NOT NULL";
		}
	}

	if (m_nPKFieldCount > 1)
	{
		strCommand += ", CONSTRAINT PK_";
		strCommand += strTableName;
		strCommand += " PRIMARY KEY(";

		CCSV csv{};
		for (int n = 0; n < m_nPKFieldCount; n++)
		{
			csv.Add(m_arrayFieldInfo[n].m_strFieldName);
		}

		strCommand += csv.GetLine();
		strCommand += ")";
	}

	for (int n = 0; n < arrayIndexColumns.GetSize(); n++)
	{
		int nPos = arrayIndexColumns.GetAt(n);

		strCommand += ",INDEX IX_";
		strCommand += m_arrayFieldInfo[nPos].m_strFieldName;
		strCommand += " NONCLUSTERED (";
		strCommand += m_arrayFieldInfo[nPos].m_strFieldName;
		strCommand += ")";
	}

	strCommand += ");";

	return strCommand;
}

/**********************************************************************/

