/**********************************************************************/
#include "..\..\SPosLoyalty4Shared\SQLNamespaces.h"
/**********************************************************************/
#include "SQLBuildSchema.h"
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
	m_nPKFieldCount = 1;
	m_bAutoID = FALSE;
}

/**********************************************************************/

void CSQLBuilderCreateTable::AddFieldInternal(CString strFieldName, int nDataType, int nFlags, int nFieldLen1, int nFieldLen2 )
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

	case SQLSRVR_LEGACY_FLOAT:
		return "FLOAT";

	case SQLSRVR_ROWVERSION:
		strResult = "ROWVERSION";
		break;

	case SQLSRVR_COMPUTED:
		strResult = "";
		break;

	case SQLSRVR_TEXT:
	default:
		strResult.Format("VARCHAR(%d)", FieldInfo.m_nFieldLen1);
		break;
	}

	return strResult;
}

/**********************************************************************/

void CSQLBuilderCreateTable::SetupAccountFields()
{
	m_nPKFieldCount = 1;
	m_bAutoID = FALSE;

	m_arrayFieldInfo.RemoveAll();
	AddInt64(Account::UserID);
	AddText(Account::Fullname);
	AddText(Account::Forename);
	AddText(Account::Surname);
	AddText(Account::DOB);
	AddText(Account::ExpiryDate);
	AddText(Account::Gender);
	AddInt32(Account::GroupNo);
	AddInt32(Account::SchemeNo);
	AddText(Account::Info1);
	AddText(Account::Info2, SQLFIELD_FLAG_INDEX);
	AddText(Account::Info3);
	AddText(Account::Info4);
	AddText(Account::Address1);
	AddText(Account::Address2);
	AddText(Account::Address3);
	AddText(Account::Address4);
	AddText(Account::Address5);
	AddText(Account::Phone1);
	AddText(Account::Phone2);
	AddBit(Account::Inactive);
	/*****/
	AddText(Account::ExternalRef, SQLFIELD_FLAG_INDEX);
	AddText(Account::QRInfo, SQLFIELD_FLAG_INDEX);
	AddInt64(Account::NextUserID);
	AddInt64(Account::PreviousUserID);
	/*****/
	AddDecimal (Account::Purse1Balance);
	AddText(Account::Purse1LastSpendDate);
	AddText(Account::Purse1LastSpendTime);
	AddDecimal(Account::Purse1LastSpend);
	AddDecimal(Account::Purse1SpendToDate);
	/*****/
	AddDecimal(Account::Purse2Balance);
	AddText(Account::Purse2LastSpendDate);
	AddText(Account::Purse2LastSpendTime);
	AddDecimal(Account::Purse2LastSpend);
	AddDecimal(Account::Purse2SpendToDate);
	AddText(Account::Purse2RefreshedDate);
	AddText(Account::Purse2RefreshedTime);
	/*****/
	AddText(Account::CashLastSpendDate);
	AddText(Account::CashLastSpendTime);
	AddDecimal(Account::CashLastSpend);
	AddDecimal(Account::CashSpendToDate);
	/*****/
	AddInt32(Account::Points);
	AddInt32(Account::PointsToDate);
	/*****/
	AddInt32(Account::HotlistCode);
	AddText(Account::HotlistDate);
	AddText(Account::HotlistTime);
	/*****/
	AddDecimal(Account::MaxSpend);
	AddDecimal(Account::MaxOverdraft);
	/*****/
	AddText(Account::AlertCodes);
	/*****/
	AddRowVersion(Account::RowVersion);
	/*****/
	AddComputed("TotalSpendToDate AS ([Purse1SpendToDate] + [Purse2SpendToDate] + [CashSpendToDate])");
}

/**********************************************************************/

CString CSQLBuilderCreateTable::GetCommandCreateAccountTable()
{
	SetupAccountFields();
	return BuildCommandString(SQLTableNames::Accounts);
}

/**********************************************************************/

CString CSQLBuilderCreateTable::GetCommandCreateGroupTable()
{
	//REDEEM TRIP SHOULD HAVE BEEN AN INTEGER
	//RETAIN AS A DECIMAL FOR NOW FOR BACKWARD COMPATIBILITY
	//MAYBE MIGRATE TO INTEGER IN FUTURE

	m_nPKFieldCount = 1;
	m_bAutoID = FALSE;

	m_arrayFieldInfo.RemoveAll();

	AddInt32(Group::GroupNo);
	AddText(Group::GroupName);
	AddInt32(Group::SchemeNo);
	AddBit(Group::StampOfferFlag);
	/*****/
	AddInt32(Group::RefreshType);
	AddInt32(Group::RefreshFlags);
	AddDecimal(Group::RefreshValue);
	AddInt32(Group::RefreshAppend);
	/*****/
	AddInt32(Group::Purse1Allowed);
	AddInt32(Group::Purse2Allowed);
	/*****/
	AddBit(Group::NoPluBonus);
	AddBit(Group::NoDeptBonus);
	/*****/
	AddInt32(Group::MaxSpendType);
	/*****/
	AddDecimal(Group::MaxSpendPeriod1);
	AddDecimal(Group::MaxSpendPeriod2);
	AddDecimal(Group::MaxSpendPeriod3);
	AddDecimal(Group::MaxSpendPeriod4);
	AddDecimal(Group::MaxSpendPeriod5);
	AddDecimal(Group::MaxSpendPeriod6);
	/*****/
	AddDecimal(Group::MaxOverdraftPeriod1);
	AddDecimal(Group::MaxOverdraftPeriod2);
	AddDecimal(Group::MaxOverdraftPeriod3);
	AddDecimal(Group::MaxOverdraftPeriod4);
	AddDecimal(Group::MaxOverdraftPeriod5);
	AddDecimal(Group::MaxOverdraftPeriod6);	
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
	AddInt32(Group::RedeemType);
	AddDecimal(Group::RedeemTripAsDouble.Label);
	AddDecimal(Group::RedeemValue);
	/*****/
	return BuildCommandString(SQLTableNames::AccountGroups);
}

/**********************************************************************/

CString CSQLBuilderCreateTable::GetCommandCreateGroupTopupBonusTable()
{
	m_nPKFieldCount = 3;
	m_bAutoID = FALSE;

	m_arrayFieldInfo.RemoveAll();

	AddInt32(GroupTopupBonus::GroupNo);
	AddInt32(GroupTopupBonus::PurseNo);
	AddInt32(GroupTopupBonus::TopupNo);
	/*****/
	AddBit(GroupTopupBonus::BonusFlag);
	AddBit(GroupTopupBonus::BonusDisable);
	AddInt32(GroupTopupBonus::BonusType);
	AddDecimal(GroupTopupBonus::BonusTrip);
	AddInt32(GroupTopupBonus::BonusPoints);
	AddDecimal(GroupTopupBonus::BonusValue);
	AddInt32(GroupTopupBonus::BonusPercent);
	/*****/
	return BuildCommandString(SQLTableNames::GroupTopupBonuses);
}

/**********************************************************************/

CString CSQLBuilderCreateTable::GetCommandCreateVersionTable()
{
	m_nPKFieldCount = 1;
	m_bAutoID = FALSE;
	
	m_arrayFieldInfo.RemoveAll();
	AddInt32(DbVersion::LineID);
	AddInt32(DbVersion::Version);

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

CString CSQLBuilderCreateTable::GetCommandCreatePluInfoTable()
{
	m_nPKFieldCount = 2;
	m_bAutoID = FALSE;

	m_arrayFieldInfo.RemoveAll();
	AddInt32(PluInfo::DbNo);
	AddInt64(PluInfo::PluNo);
	AddText(PluInfo::Description);
	AddInt32(PluInfo::DeptNo);
	AddText(PluInfo::TaxCode);
	AddInt32(PluInfo::Points);
	AddInt32(PluInfo::ModType);
	AddInt32(PluInfo::StampOfferID);

	return BuildCommandString(SQLTableNames::PluInfo);
}

/**********************************************************************/

CString CSQLBuilderCreateTable::GetCommandCreateDeptInfoTable(bool bOriginalVersion)
{
	m_bAutoID = FALSE;
	m_arrayFieldInfo.RemoveAll();

	if (TRUE == bOriginalVersion)
	{
		m_nPKFieldCount = 1;
		AddInt32(DeptInfo::DeptNo);
		AddText(DeptInfo::Name);
		AddDecimal(DeptInfo::LoyaltyBonusTrip);
		AddInt32(DeptInfo::LoyaltyBonusType);
		AddInt32(DeptInfo::LoyaltyBonusPoints);
		AddDecimal(DeptInfo::LoyaltyBonusValue);
		AddInt32(DeptInfo::LoyaltyBonusPercent);
	}
	else
	{
		m_nPKFieldCount = 2;
		AddInt32(DeptInfo::DbNo);
		AddInt32(DeptInfo::DeptNo);
		AddText(DeptInfo::Name);
		AddDecimal(DeptInfo::LoyaltyBonusTrip);
		AddInt32(DeptInfo::LoyaltyBonusType);
		AddInt32(DeptInfo::LoyaltyBonusPoints);
		AddDecimal(DeptInfo::LoyaltyBonusValue);
		AddInt32(DeptInfo::LoyaltyBonusPercent);
		AddBit(DeptInfo::GeneralSpendBonusFlag);
	}

	return BuildCommandString(SQLTableNames::DeptInfo);
}

/**********************************************************************/

CString CSQLBuilderCreateTable::GetCommandCreateStaffCardTable(bool bOriginalVersion)
{
	m_bAutoID = FALSE;
	m_arrayFieldInfo.RemoveAll();

	if (TRUE == bOriginalVersion)
	{
		m_nPKFieldCount = 2;
		AddInt32(StaffCards::FolderIndexSetNo);
		AddInt32(StaffCards::ServerNo);
		AddInt64(StaffCards::UserID);
	}
	else
	{
		m_nPKFieldCount = 3;
		AddInt32(StaffCards::FolderIndexDbNo);
		AddInt32(StaffCards::FolderIndexSetNo);
		AddInt32(StaffCards::ServerNo);
		AddInt64(StaffCards::UserID);
	}
	
	return BuildCommandString(SQLTableNames::StaffCards);
}

/**********************************************************************/

CString CSQLBuilderCreateTable::GetCommandCreateStaffGiftExceptionTable(bool bOriginalVersion)
{
	m_nPKFieldCount = 1;
	m_bAutoID = TRUE;
	m_arrayFieldInfo.RemoveAll();

	if (TRUE == bOriginalVersion)
	{
		AddInt64(StaffGiftExceptions::LineID);
		AddInt64(StaffGiftExceptions::CustomerID);
		AddInt32(StaffGiftExceptions::FolderIndexSetNo);
		AddInt32(StaffGiftExceptions::ServerNo);
		AddInt64(StaffGiftExceptions::ServerID);
		AddText(StaffGiftExceptions::Date);
		AddText(StaffGiftExceptions::Time);
		AddLegacyFloat(StaffGiftExceptions::Value);
		AddInt32(StaffGiftExceptions::ErrorCode);
	}
	else
	{
		AddInt64(StaffGiftExceptions::LineID);
		AddInt64(StaffGiftExceptions::CustomerID);
		AddInt32(StaffGiftExceptions::FolderIndexDbNo);
		AddInt32(StaffGiftExceptions::FolderIndexSetNo);
		AddInt32(StaffGiftExceptions::ServerNo);
		AddInt64(StaffGiftExceptions::ServerID);
		AddText(StaffGiftExceptions::Date);
		AddText(StaffGiftExceptions::Time);
		AddDecimal(StaffGiftExceptions::Value);
		AddInt32(StaffGiftExceptions::ErrorCode);
	}

	return BuildCommandString(SQLTableNames::StaffGiftExceptions);
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

CString CSQLBuilderCreateTable::GetCommandCreateOfferStampTable()
{
	m_nPKFieldCount = 5;
	m_bAutoID = FALSE;

	m_arrayFieldInfo.RemoveAll();
	AddInt64(OfferStamp::UserID);
	AddInt32(OfferStamp::StampOfferID);
	AddInt32(OfferStamp::Edition);
	AddText(OfferStamp::AwardDate);
	AddText(OfferStamp::ExpireDate);
	AddInt32(OfferStamp::StampCount);

	return BuildCommandString(SQLTableNames::OfferStamps);
}

/**********************************************************************/

CString CSQLBuilderCreateTable::GetCommandCreateStampOfferInfoTable()
{
	m_nPKFieldCount = 1;
	m_bAutoID = FALSE;

	m_arrayFieldInfo.RemoveAll();
	AddInt32(StampOfferInfo::StampOfferID);
	AddInt32(StampOfferInfo::Edition);
	AddBit(StampOfferInfo::Enabled);
	AddInt32(StampOfferInfo::Threshold);
	AddInt32(StampOfferInfo::Expiry);
	AddText(StampOfferInfo::Description);

	return BuildCommandString(SQLTableNames::StampOfferInfo);
}

/**********************************************************************/

CString CSQLBuilderCreateTable::GetCommandCreateEcrmanDatabaseTable()
{
	m_nPKFieldCount = 1;
	m_bAutoID = FALSE;

	m_arrayFieldInfo.RemoveAll();
	AddInt32(EcrmanDatabase::DbNo.Label);
	AddText(EcrmanDatabase::DbName);

	return BuildCommandString(SQLTableNames::EcrmanDatabases);
}

/**********************************************************************/

CString CSQLBuilderCreateTable::GetCommandCreateSchemeTable()
{
	m_nPKFieldCount = 1;
	m_bAutoID = FALSE;

	m_arrayFieldInfo.RemoveAll();
	AddInt32(Scheme::SchemeNo);
	AddText(Scheme::SchemeName);

	return BuildCommandString(SQLTableNames::Schemes);
}

/**********************************************************************/

CString CSQLBuilderCreateTable::BuildCommandString(CString strTableName, bool bIfNotExists )
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
		case SQLSRVR_COMPUTED:
			bNotNull = (0 == n);
			break;

		case SQLSRVR_ROWVERSION:
			bNotNull = FALSE;
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
