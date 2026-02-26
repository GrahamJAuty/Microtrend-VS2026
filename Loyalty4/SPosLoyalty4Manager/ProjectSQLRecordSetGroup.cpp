//**********************************************************************
#include "..\SPosLoyalty4Shared\SQLNamespaces.h"
//**********************************************************************
#include "ProjectSQLRecordSetGroup.h"
//**********************************************************************

CProjectSQLRecordSetGroup::CProjectSQLRecordSetGroup() : CSQLRecordSetGroup(NULL, RSParams_Group_NormalNoParams{})
{
}

//**********************************************************************

void CProjectSQLRecordSetGroup::ExportToScript(CSSFile& fileOut, CWorkingDlg& WorkingDlg)
{
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

	CSQLOut SQLOut(fileOut, SQLTableNames::AccountGroups, arrayHeader);

	SQLOut.WriteDelete();

	CSQLRowGroup RowGroup;
	while (StepSelectAll(RowGroup) == TRUE)
	{
		SQLOut.Add(RowGroup.GetGroupNo());
		SQLOut.Add(RowGroup.GetGroupName());
		SQLOut.Add(RowGroup.GetSchemeNo());
		SQLOut.Add(RowGroup.GetStampOfferFlag());
		/*****/
		SQLOut.Add(RowGroup.GetRefreshType());
		SQLOut.Add(RowGroup.GetRefreshFlags());
		SQLOut.Add(RowGroup.GetRefreshValue(), 2);
		SQLOut.Add(RowGroup.GetRefreshAppend());
		/*****/
		SQLOut.Add(RowGroup.GetPurse1Allowed());
		SQLOut.Add(RowGroup.GetPurse2Allowed());
		/*****/
		SQLOut.Add(RowGroup.GetNoPluBonusFlag());
		SQLOut.Add(RowGroup.GetNoDeptBonusFlag());
		/*****/
		SQLOut.Add(RowGroup.GetMaxSpendType());
		/*****/
		SQLOut.Add(RowGroup.GetMaxSpendPeriod1(), 2);
		SQLOut.Add(RowGroup.GetMaxSpendPeriod2(), 2);
		SQLOut.Add(RowGroup.GetMaxSpendPeriod3(), 2);
		SQLOut.Add(RowGroup.GetMaxSpendPeriod4(), 2);
		SQLOut.Add(RowGroup.GetMaxSpendPeriod5(), 2);
		SQLOut.Add(RowGroup.GetMaxSpendPeriod6(), 2);
		/*****/
		SQLOut.Add(RowGroup.GetMaxOverdraftPeriod1(), 2);
		SQLOut.Add(RowGroup.GetMaxOverdraftPeriod2(), 2);
		SQLOut.Add(RowGroup.GetMaxOverdraftPeriod3(), 2);
		SQLOut.Add(RowGroup.GetMaxOverdraftPeriod4(), 2);
		SQLOut.Add(RowGroup.GetMaxOverdraftPeriod5(), 2);
		SQLOut.Add(RowGroup.GetMaxOverdraftPeriod6(), 2);
		/*****/
		SQLOut.Add(RowGroup.GetRewardFlag());
		SQLOut.Add(RowGroup.GetRewardDisable());
		SQLOut.Add(RowGroup.GetRewardType());
		SQLOut.Add(RowGroup.GetRewardTrip(), 2);
		SQLOut.Add(RowGroup.GetRewardPoints());
		SQLOut.Add(RowGroup.GetRewardBonus(), 2);
		SQLOut.Add(RowGroup.GetRewardLimit(), 2);
		SQLOut.Add(RowGroup.GetRewardTrigger(), 2);
		/*****/
		SQLOut.Add(RowGroup.GetRedeemFlag());
		SQLOut.Add(RowGroup.GetRedeemDisable());
		SQLOut.Add(RowGroup.GetRedeemType());
		SQLOut.Add(RowGroup.GetRedeemTrip(), 2);
		SQLOut.Add(RowGroup.GetRedeemValue(), 2);

		SQLOut.WriteInsert();
		WorkingDlg.IncrementRecordsChecked(10, FALSE);
	}
}

//**********************************************************************
