//**********************************************************************
#include "..\SmartPay4Shared\SQLHelpers.h"
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
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

bool CProjectSQLRepositoryGroup::ImportFromScript(CSSFile& fileIn, CDatabase* pDatabase, CWorkingDlg& WorkingDlg)
{
	WorkingDlg.SetCaption1("Preparing Account Groups Table");
	if (CSQLHelpers::DeleteTableRows(m_strImportTableName, pDatabase) == FALSE)
	{
		return FALSE;
	}
	WorkingDlg.SetCaption1("Restoring Account Groups Table");

	fileIn.Seek(0, CFile::begin);

	CStringArray arrayHeader;
	arrayHeader.Add(Group::GroupNo.Label);
	arrayHeader.Add(Group::GroupName.Label);
	arrayHeader.Add(Group::UserType.Label);
	arrayHeader.Add(Group::SetNo.Label);
	arrayHeader.Add(Group::ShiftGroups.Label);
	arrayHeader.Add(Group::PriceLevel.Label);
	arrayHeader.Add(Group::VATShift.Label);
	arrayHeader.Add(Group::PurchaseControlFlag.Label);
	arrayHeader.Add(Group::AllowRevalKioskFlag.Label);
	/*****/
	arrayHeader.Add(Group::RefreshType.Label);
	arrayHeader.Add(Group::RefreshFlags.Label);
	arrayHeader.Add(Group::RefreshValue.Label);
	arrayHeader.Add(Group::RefreshAppend.Label);
	arrayHeader.Add(Group::RefreshTypePeriod1.Label);
	arrayHeader.Add(Group::RefreshTypePeriod2.Label);
	arrayHeader.Add(Group::RefreshTypePeriod3.Label);
	arrayHeader.Add(Group::RefreshTypePeriod4.Label);
	arrayHeader.Add(Group::RefreshTypePeriod5.Label);
	arrayHeader.Add(Group::RefreshTypePeriod6.Label);
	arrayHeader.Add(Group::RefreshValuePeriod1.Label);
	arrayHeader.Add(Group::RefreshValuePeriod2.Label);
	arrayHeader.Add(Group::RefreshValuePeriod3.Label);
	arrayHeader.Add(Group::RefreshValuePeriod4.Label);
	arrayHeader.Add(Group::RefreshValuePeriod5.Label);
	arrayHeader.Add(Group::RefreshValuePeriod6.Label);
	/*****/
	arrayHeader.Add(Group::Purse1Allowed.Label);
	arrayHeader.Add(Group::Purse2Allowed.Label);
	arrayHeader.Add(Group::Purse3Allowed.Label);
	/*****/
	arrayHeader.Add(Group::MaxSpendType.Label);
	/*****/
	arrayHeader.Add(Group::MaxSpendPurse1Period1.Label);
	arrayHeader.Add(Group::MaxSpendPurse1Period2.Label);
	arrayHeader.Add(Group::MaxSpendPurse1Period3.Label);
	arrayHeader.Add(Group::MaxSpendPurse1Period4.Label);
	arrayHeader.Add(Group::MaxSpendPurse1Period5.Label);
	arrayHeader.Add(Group::MaxSpendPurse1Period6.Label);
	/*****/
	arrayHeader.Add(Group::MaxSpendPurse3Period1.Label);
	arrayHeader.Add(Group::MaxSpendPurse3Period2.Label);
	arrayHeader.Add(Group::MaxSpendPurse3Period3.Label);
	arrayHeader.Add(Group::MaxSpendPurse3Period4.Label);
	arrayHeader.Add(Group::MaxSpendPurse3Period5.Label);
	arrayHeader.Add(Group::MaxSpendPurse3Period6.Label);
	/*****/
	arrayHeader.Add(Group::MaxOverdraftPurse1Period1.Label);
	arrayHeader.Add(Group::MaxOverdraftPurse1Period2.Label);
	arrayHeader.Add(Group::MaxOverdraftPurse1Period3.Label);
	arrayHeader.Add(Group::MaxOverdraftPurse1Period4.Label);
	arrayHeader.Add(Group::MaxOverdraftPurse1Period5.Label);
	arrayHeader.Add(Group::MaxOverdraftPurse1Period6.Label);
	/*****/
	arrayHeader.Add(Group::MaxOverdraftPurse3Period1.Label);
	arrayHeader.Add(Group::MaxOverdraftPurse3Period2.Label);
	arrayHeader.Add(Group::MaxOverdraftPurse3Period3.Label);
	arrayHeader.Add(Group::MaxOverdraftPurse3Period4.Label);
	arrayHeader.Add(Group::MaxOverdraftPurse3Period5.Label);
	arrayHeader.Add(Group::MaxOverdraftPurse3Period6.Label);
	/*****/
	arrayHeader.Add(Group::DeptInhibitPurse1.Label);
	arrayHeader.Add(Group::DeptInhibitPurse2.Label);
	arrayHeader.Add(Group::DeptInhibitPurse3.Label);
	arrayHeader.Add(Group::DeptInhibitCash.Label);
	/*****/
	arrayHeader.Add(Group::LowBalancePurse1.Label);
	arrayHeader.Add(Group::LowBalancePurse2.Label);
	arrayHeader.Add(Group::LowBalancePurse3.Label);
	/*****/
	arrayHeader.Add(Group::PointsAchievement.Label);
	/*****/
	arrayHeader.Add(Group::RevalueFlag.Label);
	arrayHeader.Add(Group::RevalueDisable.Label);
	arrayHeader.Add(Group::RevalueType.Label);
	arrayHeader.Add(Group::RevalueTrip1.Label);
	arrayHeader.Add(Group::RevalueTrip2.Label);
	arrayHeader.Add(Group::RevaluePoints.Label);
	arrayHeader.Add(Group::RevalueBonus.Label);
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
	arrayHeader.Add(Group::RedeemTripAsInt.Label);
	arrayHeader.Add(Group::RedeemValue.Label);
	arrayHeader.Add(Group::RedeemPurse.Label);

	CSQLIn SQLIn(fileIn, m_strImportTableName, arrayHeader);

	CString strBuffer = "";
	while (SQLIn.ReadLine(strBuffer))
	{
		CCSV csvIn(strBuffer);

		CSQLRowGroupFull RowGroup;
		RowGroup.SetGroupNo(csvIn.GetInt(0));
		RowGroup.SetGroupName(csvIn.GetString(1));
		RowGroup.SetUserType(csvIn.GetInt(2));
		RowGroup.SetSetNo(csvIn.GetInt(3));
		RowGroup.SetShiftGroups(csvIn.GetString(4));
		RowGroup.SetPriceLevel(csvIn.GetInt(5));
		RowGroup.SetVatShiftFlag(csvIn.GetInt(6));
		RowGroup.SetPurchaseControlFlag(csvIn.GetBool(7));
		RowGroup.SetAllowRevalKioskFlag(csvIn.GetBool(8));
		/*****/
		RowGroup.SetRefreshType(csvIn.GetInt(9));
		RowGroup.SetRefreshFlags(csvIn.GetInt(10));
		RowGroup.SetRefreshValue(csvIn.GetDouble(11));
		RowGroup.SetRefreshAppendFlag(csvIn.GetInt(12));
		RowGroup.SetRefreshTypePeriod1(csvIn.GetInt(13));
		RowGroup.SetRefreshTypePeriod2(csvIn.GetInt(14));
		RowGroup.SetRefreshTypePeriod3(csvIn.GetInt(15));
		RowGroup.SetRefreshTypePeriod4(csvIn.GetInt(16));
		RowGroup.SetRefreshTypePeriod5(csvIn.GetInt(17));
		RowGroup.SetRefreshTypePeriod6(csvIn.GetInt(18));
		RowGroup.SetRefreshValuePeriod1(csvIn.GetDouble(19));
		RowGroup.SetRefreshValuePeriod2(csvIn.GetDouble(20));
		RowGroup.SetRefreshValuePeriod3(csvIn.GetDouble(21));
		RowGroup.SetRefreshValuePeriod4(csvIn.GetDouble(22));
		RowGroup.SetRefreshValuePeriod5(csvIn.GetDouble(23));
		RowGroup.SetRefreshValuePeriod6(csvIn.GetDouble(24));
		/*****/
		RowGroup.SetPurse1Allowed(csvIn.GetInt(25));
		RowGroup.SetPurse2Allowed(csvIn.GetInt(26));
		RowGroup.SetPurse3Allowed(csvIn.GetInt(27));
		/*****/
		RowGroup.SetMaxSpendType(csvIn.GetInt(28));
		/*****/
		RowGroup.SetMaxSpendPurse1Period1(csvIn.GetDouble(29));
		RowGroup.SetMaxSpendPurse1Period2(csvIn.GetDouble(30));
		RowGroup.SetMaxSpendPurse1Period3(csvIn.GetDouble(31));
		RowGroup.SetMaxSpendPurse1Period4(csvIn.GetDouble(32));
		RowGroup.SetMaxSpendPurse1Period5(csvIn.GetDouble(33));
		RowGroup.SetMaxSpendPurse1Period6(csvIn.GetDouble(34));
		/*****/
		RowGroup.SetMaxSpendPurse3Period1(csvIn.GetDouble(35));
		RowGroup.SetMaxSpendPurse3Period2(csvIn.GetDouble(36));
		RowGroup.SetMaxSpendPurse3Period3(csvIn.GetDouble(37));
		RowGroup.SetMaxSpendPurse3Period4(csvIn.GetDouble(38));
		RowGroup.SetMaxSpendPurse3Period5(csvIn.GetDouble(39));
		RowGroup.SetMaxSpendPurse3Period6(csvIn.GetDouble(40));
		/*****/
		RowGroup.SetMaxOverdraftPurse1Period1(csvIn.GetDouble(41));
		RowGroup.SetMaxOverdraftPurse1Period2(csvIn.GetDouble(42));
		RowGroup.SetMaxOverdraftPurse1Period3(csvIn.GetDouble(43));
		RowGroup.SetMaxOverdraftPurse1Period4(csvIn.GetDouble(44));
		RowGroup.SetMaxOverdraftPurse1Period5(csvIn.GetDouble(45));
		RowGroup.SetMaxOverdraftPurse1Period6(csvIn.GetDouble(46));
		/*****/
		RowGroup.SetMaxOverdraftPurse3Period1(csvIn.GetDouble(47));
		RowGroup.SetMaxOverdraftPurse3Period2(csvIn.GetDouble(48));
		RowGroup.SetMaxOverdraftPurse3Period3(csvIn.GetDouble(49));
		RowGroup.SetMaxOverdraftPurse3Period4(csvIn.GetDouble(50));
		RowGroup.SetMaxOverdraftPurse3Period5(csvIn.GetDouble(51));
		RowGroup.SetMaxOverdraftPurse3Period6(csvIn.GetDouble(52));
		/*****/
		RowGroup.SetDeptInhibitPurse1(csvIn.GetString(53));
		RowGroup.SetDeptInhibitPurse2(csvIn.GetString(54));
		RowGroup.SetDeptInhibitPurse3(csvIn.GetString(55));
		RowGroup.SetDeptInhibitCash(csvIn.GetString(56));
		/*****/
		RowGroup.SetLowBalancePurse1(csvIn.GetDouble(57));
		RowGroup.SetLowBalancePurse2(csvIn.GetDouble(58));
		RowGroup.SetLowBalancePurse3(csvIn.GetDouble(59));
		/*****/
		RowGroup.SetPointsAchievement(csvIn.GetInt(60));
		/*****/
		RowGroup.SetFileRevalueFlag(csvIn.GetBool(61));
		RowGroup.SetRevalueDisable(csvIn.GetBool(62));
		RowGroup.SetRevalueType(csvIn.GetInt(63));
		RowGroup.SetRevalueTrip1(csvIn.GetDouble(64));
		RowGroup.SetRevalueTrip2(csvIn.GetDouble(65));
		RowGroup.SetRevaluePoints(csvIn.GetInt(66));
		RowGroup.SetRevalueBonus(csvIn.GetDouble(67));
		/*****/
		RowGroup.SetFileRewardFlag(csvIn.GetBool(68));
		RowGroup.SetRewardDisable(csvIn.GetBool(69));
		RowGroup.SetRewardType(csvIn.GetInt(70));
		RowGroup.SetRewardTrip(csvIn.GetDouble(71));
		RowGroup.SetRewardPoints(csvIn.GetInt(72));
		RowGroup.SetRewardBonus(csvIn.GetDouble(73));
		RowGroup.SetRewardLimit(csvIn.GetDouble(74));
		RowGroup.SetRewardTrigger(csvIn.GetDouble(75));
		/*****/
		RowGroup.SetFileRedeemFlag(csvIn.GetBool(76));
		RowGroup.SetRedeemDisable(csvIn.GetBool(77));
		RowGroup.SetRedeemTrip(csvIn.GetInt(78));
		RowGroup.SetRedeemValue(csvIn.GetDouble(79));
		RowGroup.SetRedeemPurse(csvIn.GetInt(80));

		if (InsertRow(RowGroup, pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
		{
			return FALSE;
		}

		WorkingDlg.IncrementRecordsChecked(10, FALSE);
	}

	return TRUE;
}

//**********************************************************************
