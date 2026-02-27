//**********************************************************************
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "ProjectSQLRecordSetGroupFull.h"
//**********************************************************************

CProjectSQLRecordSetGroupFull::CProjectSQLRecordSetGroupFull() : CSQLRecordSetGroupFull(NULL, RSParams_Group_NormalNoParams{})
{
}

//**********************************************************************

void CProjectSQLRecordSetGroupFull::ExportToScript(CSSFile& fileOut, CWorkingDlg& WorkingDlg)
{
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

	CSQLOut SQLOut(fileOut, SQLTableNames::AccountGroups, arrayHeader);

	SQLOut.WriteDelete();

	CSQLRowGroupFull RowGroup;
	while (StepSelectAll(RowGroup) == TRUE)
	{
		SQLOut.Add(RowGroup.GetGroupNo());
		SQLOut.Add(RowGroup.GetGroupName());
		SQLOut.Add(RowGroup.GetUserType());
		SQLOut.Add(RowGroup.GetSetNo());
		SQLOut.Add(RowGroup.GetShiftGroups());
		SQLOut.Add(RowGroup.GetPriceLevel());
		SQLOut.Add(RowGroup.GetVatShiftFlag());
		SQLOut.Add(RowGroup.GetPurchaseControlFlag());
		SQLOut.Add(RowGroup.GetAllowRevalKioskFlag());
		/*****/
		SQLOut.Add(RowGroup.GetRefreshType());
		SQLOut.Add(RowGroup.GetRefreshFlags());
		SQLOut.Add(RowGroup.GetRefreshValue(), 2);
		SQLOut.Add(RowGroup.GetRefreshAppendFlag());
		SQLOut.Add(RowGroup.GetRefreshTypePeriod1());
		SQLOut.Add(RowGroup.GetRefreshTypePeriod2());
		SQLOut.Add(RowGroup.GetRefreshTypePeriod3());
		SQLOut.Add(RowGroup.GetRefreshTypePeriod4());
		SQLOut.Add(RowGroup.GetRefreshTypePeriod5());
		SQLOut.Add(RowGroup.GetRefreshTypePeriod6());
		SQLOut.Add(RowGroup.GetRefreshValuePeriod1(), 2);
		SQLOut.Add(RowGroup.GetRefreshValuePeriod2(), 2);
		SQLOut.Add(RowGroup.GetRefreshValuePeriod3(), 2);
		SQLOut.Add(RowGroup.GetRefreshValuePeriod4(), 2);
		SQLOut.Add(RowGroup.GetRefreshValuePeriod5(), 2);
		SQLOut.Add(RowGroup.GetRefreshValuePeriod6(), 2);
		/*****/
		SQLOut.Add(RowGroup.GetPurse1Allowed());
		SQLOut.Add(RowGroup.GetPurse2Allowed());
		SQLOut.Add(RowGroup.GetPurse3Allowed());
		/*****/
		SQLOut.Add(RowGroup.GetMaxSpendType());
		/*****/
		SQLOut.Add(RowGroup.GetMaxSpendPurse1Period1(), 2);
		SQLOut.Add(RowGroup.GetMaxSpendPurse1Period2(), 2);
		SQLOut.Add(RowGroup.GetMaxSpendPurse1Period3(), 2);
		SQLOut.Add(RowGroup.GetMaxSpendPurse1Period4(), 2);
		SQLOut.Add(RowGroup.GetMaxSpendPurse1Period5(), 2);
		SQLOut.Add(RowGroup.GetMaxSpendPurse1Period6(), 2);
		/*****/
		SQLOut.Add(RowGroup.GetMaxSpendPurse3Period1(), 2);
		SQLOut.Add(RowGroup.GetMaxSpendPurse3Period2(), 2);
		SQLOut.Add(RowGroup.GetMaxSpendPurse3Period3(), 2);
		SQLOut.Add(RowGroup.GetMaxSpendPurse3Period4(), 2);
		SQLOut.Add(RowGroup.GetMaxSpendPurse3Period5(), 2);
		SQLOut.Add(RowGroup.GetMaxSpendPurse3Period6(), 2);
		/*****/
		SQLOut.Add(RowGroup.GetMaxOverdraftPurse1Period1(), 2);
		SQLOut.Add(RowGroup.GetMaxOverdraftPurse1Period2(), 2);
		SQLOut.Add(RowGroup.GetMaxOverdraftPurse1Period3(), 2);
		SQLOut.Add(RowGroup.GetMaxOverdraftPurse1Period4(), 2);
		SQLOut.Add(RowGroup.GetMaxOverdraftPurse1Period5(), 2);
		SQLOut.Add(RowGroup.GetMaxOverdraftPurse1Period6(), 2);
		/*****/
		SQLOut.Add(RowGroup.GetMaxOverdraftPurse3Period1(), 2);
		SQLOut.Add(RowGroup.GetMaxOverdraftPurse3Period2(), 2);
		SQLOut.Add(RowGroup.GetMaxOverdraftPurse3Period3(), 2);
		SQLOut.Add(RowGroup.GetMaxOverdraftPurse3Period4(), 2);
		SQLOut.Add(RowGroup.GetMaxOverdraftPurse3Period5(), 2);
		SQLOut.Add(RowGroup.GetMaxOverdraftPurse3Period6(), 2);
		/*****/
		SQLOut.Add(RowGroup.GetDeptInhibitPurse1());
		SQLOut.Add(RowGroup.GetDeptInhibitPurse2());
		SQLOut.Add(RowGroup.GetDeptInhibitPurse3());
		SQLOut.Add(RowGroup.GetDeptInhibitCash());
		/*****/
		SQLOut.Add(RowGroup.GetLowBalancePurse1(), 2);
		SQLOut.Add(RowGroup.GetLowBalancePurse2(), 2);
		SQLOut.Add(RowGroup.GetLowBalancePurse3(), 2);
		/*****/
		SQLOut.Add(RowGroup.GetPointsAchievement());
		/*****/
		SQLOut.Add(RowGroup.GetFileRevalueFlag());
		SQLOut.Add(RowGroup.GetRevalueDisable());
		SQLOut.Add(RowGroup.GetRevalueType());
		SQLOut.Add(RowGroup.GetRevalueTrip1(), 2);
		SQLOut.Add(RowGroup.GetRevalueTrip2(), 2);
		SQLOut.Add(RowGroup.GetRevaluePoints());
		SQLOut.Add(RowGroup.GetRevalueBonus(), 2);
		/*****/
		SQLOut.Add(RowGroup.GetFileRewardFlag());
		SQLOut.Add(RowGroup.GetRewardDisable());
		SQLOut.Add(RowGroup.GetRewardType());
		SQLOut.Add(RowGroup.GetRewardTrip(), 2);
		SQLOut.Add(RowGroup.GetRewardPoints());
		SQLOut.Add(RowGroup.GetRewardBonus(), 2);
		SQLOut.Add(RowGroup.GetRewardLimit(), 2);
		SQLOut.Add(RowGroup.GetRewardTrigger(), 2);
		/*****/
		SQLOut.Add(RowGroup.GetFileRedeemFlag());
		SQLOut.Add(RowGroup.GetRedeemDisable());
		SQLOut.Add(RowGroup.GetRedeemTrip());
		SQLOut.Add(RowGroup.GetRedeemValue(), 2);
		SQLOut.Add(RowGroup.GetRedeemPurse());

		SQLOut.WriteInsert();
		fileOut.WriteLine("");

		WorkingDlg.IncrementRecordsChecked(10, TRUE);
	}
}

//**********************************************************************
