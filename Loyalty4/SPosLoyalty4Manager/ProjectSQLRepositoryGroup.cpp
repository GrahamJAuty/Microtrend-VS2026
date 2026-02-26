//**********************************************************************
#include "..\SPosLoyalty4Shared\SQLHelpers.h"
#include "..\SPosLoyalty4Shared\SQLNamespaces.h"
//**********************************************************************
#include "..\SPosLoyalty4Shared\SQLTable_GroupTopupBonus\SQLRepositoryGroupTopupBonus.h"
//**********************************************************************
#include "SQLIn.h"
//**********************************************************************
#include "ProjectSQLRepositoryGroup.h"
//**********************************************************************

CProjectSQLRepositoryGroup::CProjectSQLRepositoryGroup() : CSQLRepositoryGroup()
{
	m_strImportTableName = SQLTableNames::AccountGroups;
}

//**********************************************************************

bool CProjectSQLRepositoryGroup::ImportFromScript(CSSFile& fileIn, CDatabase* pDatabase, bool bGotTopupBonuses, CWorkingDlg& WorkingDlg)
{
	bGotTopupBonuses = FALSE;

	WorkingDlg.SetCaption1("Preparing Account Group Table");
	if (CSQLHelpers::DeleteTableRows(m_strImportTableName, pDatabase) == FALSE)
	{
		return FALSE;
	}
	WorkingDlg.SetCaption2("Importing Account Group Table");

	fileIn.Seek(0, CFile::begin);

	CStringArray arrayHeader;
	arrayHeader.Add(Group::GroupNo.Label);
	arrayHeader.Add(Group::GroupName.Label);
	arrayHeader.Add(Group::SchemeNo.Label);
	arrayHeader.Add(Group::StampOfferFlag.Label);
	/*****/
	arrayHeader.Add(Group::RefreshType.Label);
	arrayHeader.Add(Group::RefreshFlags.Label);
	arrayHeader.Add(Group::RefreshValue.Label);
	arrayHeader.Add(Group::RefreshAppend.Label);
	/*****/
	arrayHeader.Add(Group::Purse1Allowed.Label);
	arrayHeader.Add(Group::Purse2Allowed.Label);
	/*****/
	arrayHeader.Add(Group::NoPluBonus.Label);
	arrayHeader.Add(Group::NoDeptBonus.Label);
	/*****/
	arrayHeader.Add(Group::MaxSpendType.Label);
	/*****/
	arrayHeader.Add(Group::MaxSpendPeriod1.Label);
	arrayHeader.Add(Group::MaxSpendPeriod2.Label);
	arrayHeader.Add(Group::MaxSpendPeriod3.Label);
	arrayHeader.Add(Group::MaxSpendPeriod4.Label);
	arrayHeader.Add(Group::MaxSpendPeriod5.Label);
	arrayHeader.Add(Group::MaxSpendPeriod6.Label);
	/*****/
	arrayHeader.Add(Group::MaxOverdraftPeriod1.Label);
	arrayHeader.Add(Group::MaxOverdraftPeriod2.Label);
	arrayHeader.Add(Group::MaxOverdraftPeriod3.Label);
	arrayHeader.Add(Group::MaxOverdraftPeriod4.Label);
	arrayHeader.Add(Group::MaxOverdraftPeriod5.Label);
	arrayHeader.Add(Group::MaxOverdraftPeriod6.Label);
	/*****/
	arrayHeader.Add(Group::LegacyRevalueFlag1.Label);
	arrayHeader.Add(Group::LegacyRevalueDisable1.Label);
	arrayHeader.Add(Group::LegacyRevalueType1.Label);
	arrayHeader.Add(Group::LegacyRevalueTrip1.Label);
	arrayHeader.Add(Group::LegacyRevaluePoints1.Label);
	arrayHeader.Add(Group::LegacyRevalueBonus1.Label);
	arrayHeader.Add(Group::LegacyRevaluePercent1.Label);
	/*****/
	arrayHeader.Add(Group::LegacyRevalueFlag2.Label);
	arrayHeader.Add(Group::LegacyRevalueDisable2.Label);
	arrayHeader.Add(Group::LegacyRevalueType2.Label);
	arrayHeader.Add(Group::LegacyRevalueTrip2.Label);
	arrayHeader.Add(Group::LegacyRevaluePoints2.Label);
	arrayHeader.Add(Group::LegacyRevalueBonus2.Label);
	arrayHeader.Add(Group::LegacyRevaluePercent2.Label);
	/*****/
	arrayHeader.Add(Group::LegacyRevalueFlag3.Label);
	arrayHeader.Add(Group::LegacyRevalueDisable3.Label);
	arrayHeader.Add(Group::LegacyRevalueType3.Label);
	arrayHeader.Add(Group::LegacyRevalueTrip3.Label);
	arrayHeader.Add(Group::LegacyRevaluePoints3.Label);
	arrayHeader.Add(Group::LegacyRevalueBonus3.Label);
	arrayHeader.Add(Group::LegacyRevaluePercent3.Label);
	/*****/
	arrayHeader.Add(Group::LegacyRevalueFlag4.Label);
	arrayHeader.Add(Group::LegacyRevalueDisable4.Label);
	arrayHeader.Add(Group::LegacyRevalueType4.Label);
	arrayHeader.Add(Group::LegacyRevalueTrip4.Label);
	arrayHeader.Add(Group::LegacyRevaluePoints4.Label);
	arrayHeader.Add(Group::LegacyRevalueBonus4.Label);
	arrayHeader.Add(Group::LegacyRevaluePercent4.Label);
	/*****/
	arrayHeader.Add(Group::RewardFlag.Label);
	arrayHeader.Add(Group::RewardDisable.Label);
	arrayHeader.Add(Group::RewardType.Label);
	arrayHeader.Add(Group::RewardTrip.Label);
	arrayHeader.Add(Group::RewardPoints.Label);
	arrayHeader.Add(Group::RewardBonus.Label);
	arrayHeader.Add(Group::RewardLimit.Label);
	arrayHeader.Add(Group::RewardTrigger.Label);
	/*****/
	arrayHeader.Add(Group::RedeemFlag.Label);
	arrayHeader.Add(Group::RedeemDisable.Label);
	arrayHeader.Add(Group::RedeemType.Label);
	arrayHeader.Add(Group::RedeemTripAsInt.Label);
	arrayHeader.Add(Group::RedeemValue.Label);

	CSQLIn SQLIn(fileIn, m_strImportTableName, arrayHeader);

	CString strBuffer = "";
	while (SQLIn.ReadLine(strBuffer))
	{
		CCSV csvIn(strBuffer);

		CSQLRowGroup RowGroup;
		RowGroup.SetGroupNo(csvIn.GetInt(0));
		RowGroup.SetGroupName(csvIn.GetString(1));
		RowGroup.SetSchemeNo(csvIn.GetInt(2));
		RowGroup.SetStampOfferFlag(csvIn.GetBool(3));
		/*****/
		RowGroup.SetRefreshType(csvIn.GetInt(4));
		RowGroup.SetRefreshFlags(csvIn.GetInt(5));
		RowGroup.SetRefreshValue(csvIn.GetDouble(6));
		RowGroup.SetRefreshAppend(csvIn.GetInt(7));
		/*****/
		RowGroup.SetPurse1Allowed(csvIn.GetInt(8));
		RowGroup.SetPurse2Allowed(csvIn.GetInt(9));
		/*****/
		RowGroup.SetNoPluBonusFlag(csvIn.GetBool(10));
		RowGroup.SetNoDeptBonusFlag(csvIn.GetBool(11));
		/*****/
		RowGroup.SetMaxSpendType(csvIn.GetInt(12));
		/*****/
		RowGroup.SetMaxSpendPeriod1(csvIn.GetDouble(13));
		RowGroup.SetMaxSpendPeriod2(csvIn.GetDouble(14));
		RowGroup.SetMaxSpendPeriod3(csvIn.GetDouble(15));
		RowGroup.SetMaxSpendPeriod4(csvIn.GetDouble(16));
		RowGroup.SetMaxSpendPeriod5(csvIn.GetDouble(17));
		RowGroup.SetMaxSpendPeriod6(csvIn.GetDouble(18));
		/*****/
		RowGroup.SetMaxOverdraftPeriod1(csvIn.GetDouble(19));
		RowGroup.SetMaxOverdraftPeriod2(csvIn.GetDouble(20));
		RowGroup.SetMaxOverdraftPeriod3(csvIn.GetDouble(21));
		RowGroup.SetMaxOverdraftPeriod4(csvIn.GetDouble(22));
		RowGroup.SetMaxOverdraftPeriod5(csvIn.GetDouble(23));
		RowGroup.SetMaxOverdraftPeriod6(csvIn.GetDouble(24));
		/*****/

		/*
		int nPos = 25;
		for (int n = 1; n <= 4; n++)
		{
			RowGroup.SetRevalueFlag(n, csvIn.GetBool(nPos++));
			RowGroup.SetRevalueDisable(n, csvIn.GetBool(nPos++));
			RowGroup.SetRevalueType(n, csvIn.GetInt(nPos++));
			RowGroup.SetRevalueTrip(n, csvIn.GetDouble(nPos++));
			RowGroup.SetRevaluePoints(n, csvIn.GetInt(nPos++));
			RowGroup.SetRevalueBonus(n, csvIn.GetDouble(nPos++));
			RowGroup.SetRevaluePercent(n, csvIn.GetInt(nPos++));
		}
		*/

		/*****/
		RowGroup.SetRewardFlag(csvIn.GetBool(53));
		RowGroup.SetRewardDisable(csvIn.GetBool(54));
		RowGroup.SetRewardType(csvIn.GetInt(55));
		RowGroup.SetRewardTrip(csvIn.GetDouble(56));
		RowGroup.SetRewardPoints(csvIn.GetInt(57));
		RowGroup.SetRewardBonus(csvIn.GetDouble(58));
		RowGroup.SetRewardLimit(csvIn.GetDouble(59));
		RowGroup.SetRewardTrigger(csvIn.GetDouble(60));
		/*****/
		RowGroup.SetRedeemFlag(csvIn.GetBool(61));
		RowGroup.SetRedeemDisable(csvIn.GetBool(62));
		RowGroup.SetRedeemType(csvIn.GetInt(63));
		RowGroup.SetRedeemTrip(csvIn.GetInt(64));
		RowGroup.SetRedeemValue(csvIn.GetInt(65));

		if (InsertRow(RowGroup, pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
		{
			return FALSE;
		}

		WorkingDlg.IncrementRecordsChecked(10, FALSE);

		if (FALSE == bGotTopupBonuses)
		{
			int nOffset = 25;

			for (int t = 1; t <= 4; t++)
			{
				if (csvIn.GetBool(nOffset) == TRUE)
				{
					CSQLRowGroupTopupBonus RowTopupBonus;
					RowTopupBonus.SetGroupNo(csvIn.GetInt(0));
					RowTopupBonus.SetPurseNo(1);
					RowTopupBonus.SetTopupNo(t);
					RowTopupBonus.SetBonusFlag(TRUE);
					RowTopupBonus.SetBonusDisable(csvIn.GetBool(nOffset + 1));
					RowTopupBonus.SetBonusType(csvIn.GetInt(nOffset + 2));
					RowTopupBonus.SetBonusTrip(csvIn.GetDouble(nOffset + 3));
					RowTopupBonus.SetBonusPoints(csvIn.GetInt(nOffset + 4));
					RowTopupBonus.SetBonusValue(csvIn.GetDouble(nOffset + 5));
					RowTopupBonus.SetBonusPercent(csvIn.GetInt(nOffset + 6));

					CSQLRepositoryGroupTopupBonus repoBonus;
					if (repoBonus.InsertRow(RowTopupBonus, pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
					{
						return FALSE;
					}

					WorkingDlg.IncrementRecordsChecked(10, FALSE);
				}

				nOffset += 7;
			}
		}
	}

	return TRUE;
}

//**********************************************************************
