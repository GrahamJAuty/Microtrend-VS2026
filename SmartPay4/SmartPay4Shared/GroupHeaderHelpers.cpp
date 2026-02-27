//$$********************************************************************
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
//$$********************************************************************
#include "GroupHeaderHelpers.h"
//$$********************************************************************

CGroupHeaderHelpers::CGroupHeaderHelpers()
{
	m_strHeaderFull = "";
	m_HeaderMap.InitHashTable(103);

	AddFieldToHeader(Group::GroupNo.Label);
	AddFieldToHeader(Group::GroupName.Label);
	AddFieldToHeader(Group::UserType.Label);
	AddFieldToHeader(Group::SetNo.Label);
	AddFieldToHeader(Group::PriceLevel.Label);
	AddFieldToHeader(Group::VATShift.Label);
	/*****/
	AddFieldToHeader(Group::RefreshType.Label);
	AddFieldToHeader(Group::RefreshFlags.Label);
	AddFieldToHeader(Group::RefreshValue.Label);
	AddFieldToHeader(Group::RefreshAppend.Label);
	/*****/
	AddFieldToHeader(Group::RefreshTypePeriod1.Label);
	AddFieldToHeader(Group::RefreshTypePeriod2.Label);
	AddFieldToHeader(Group::RefreshTypePeriod3.Label);
	AddFieldToHeader(Group::RefreshTypePeriod4.Label);
	AddFieldToHeader(Group::RefreshTypePeriod5.Label);
	AddFieldToHeader(Group::RefreshTypePeriod6.Label);
	/*****/
	AddFieldToHeader(Group::RefreshValuePeriod1.Label);
	AddFieldToHeader(Group::RefreshValuePeriod2.Label);
	AddFieldToHeader(Group::RefreshValuePeriod3.Label);
	AddFieldToHeader(Group::RefreshValuePeriod4.Label);
	AddFieldToHeader(Group::RefreshValuePeriod5.Label);
	AddFieldToHeader(Group::RefreshValuePeriod6.Label);
	/*****/
	AddFieldToHeader(Group::Purse1Allowed.Label);
	AddFieldToHeader(Group::Purse2Allowed.Label);
	AddFieldToHeader(Group::Purse3Allowed.Label);
	/*****/
	AddFieldToHeader(Group::MaxSpendType.Label);
	/*****/
	AddFieldToHeader(Group::MaxSpendPurse1Period1.Label);
	AddFieldToHeader(Group::MaxSpendPurse1Period2.Label);
	AddFieldToHeader(Group::MaxSpendPurse1Period3.Label);
	AddFieldToHeader(Group::MaxSpendPurse1Period4.Label);
	AddFieldToHeader(Group::MaxSpendPurse1Period5.Label);
	AddFieldToHeader(Group::MaxSpendPurse1Period6.Label);
	/*****/
	AddFieldToHeader(Group::MaxSpendPurse3Period1.Label);
	AddFieldToHeader(Group::MaxSpendPurse3Period2.Label);
	AddFieldToHeader(Group::MaxSpendPurse3Period3.Label);
	AddFieldToHeader(Group::MaxSpendPurse3Period4.Label);
	AddFieldToHeader(Group::MaxSpendPurse3Period5.Label);
	AddFieldToHeader(Group::MaxSpendPurse3Period6.Label);
	/*****/
	AddFieldToHeader(Group::MaxOverdraftPurse1Period1.Label);
	AddFieldToHeader(Group::MaxOverdraftPurse1Period2.Label);
	AddFieldToHeader(Group::MaxOverdraftPurse1Period3.Label);
	AddFieldToHeader(Group::MaxOverdraftPurse1Period4.Label);
	AddFieldToHeader(Group::MaxOverdraftPurse1Period5.Label);
	AddFieldToHeader(Group::MaxOverdraftPurse1Period6.Label);
	/*****/
	AddFieldToHeader(Group::MaxOverdraftPurse3Period1.Label);
	AddFieldToHeader(Group::MaxOverdraftPurse3Period2.Label);
	AddFieldToHeader(Group::MaxOverdraftPurse3Period3.Label);
	AddFieldToHeader(Group::MaxOverdraftPurse3Period4.Label);
	AddFieldToHeader(Group::MaxOverdraftPurse3Period5.Label);
	AddFieldToHeader(Group::MaxOverdraftPurse3Period6.Label);
	/*****/
	AddFieldToHeader(Group::DeptInhibitPurse1.Label);
	AddFieldToHeader(Group::DeptInhibitPurse2.Label);
	AddFieldToHeader(Group::DeptInhibitPurse3.Label);
	AddFieldToHeader(Group::DeptInhibitCash.Label);
	/*****/
	AddFieldToHeader(Group::LowBalancePurse1.Label);
	AddFieldToHeader(Group::LowBalancePurse2.Label);
	AddFieldToHeader(Group::LowBalancePurse3.Label);
	/*****/
	AddFieldToHeader(Group::PointsAchievement.Label);
	/*****/
	AddFieldToHeader(Group::RevalueFlag.Label);
	AddFieldToHeader(Group::RevalueDisable.Label);
	AddFieldToHeader(Group::RevalueType.Label);
	AddFieldToHeader(Group::RevalueTrip1.Label);
	AddFieldToHeader(Group::RevalueTrip2.Label);
	AddFieldToHeader(Group::RevaluePoints.Label);
	AddFieldToHeader(Group::RevalueBonus.Label);
	/*****/
	AddFieldToHeader(Group::RewardFlag.Label);
	AddFieldToHeader(Group::RewardDisable.Label);
	AddFieldToHeader(Group::RewardType.Label);
	AddFieldToHeader(Group::RewardTrip.Label);
	AddFieldToHeader(Group::RewardPoints.Label);
	AddFieldToHeader(Group::RewardBonus.Label);
	AddFieldToHeader(Group::RewardLimit.Label);
	AddFieldToHeader(Group::RewardTrigger.Label);
	/*****/
	AddFieldToHeader(Group::RedeemFlag.Label);
	AddFieldToHeader(Group::RedeemDisable.Label);
	AddFieldToHeader(Group::RedeemTripAsInt.Label);
	AddFieldToHeader(Group::RedeemValue.Label);
	AddFieldToHeader(Group::RedeemPurse.Label);
}

//$$********************************************************************

void CGroupHeaderHelpers::AddFieldToHeader(CString strHeader)
{
	if (m_strHeaderFull != "")
	{
		m_strHeaderFull += ",";
	}

	m_strHeaderFull += strHeader;
}

//$$********************************************************************

void CGroupHeaderHelpers::AddFieldToHeaderMap(CString strField, int nFieldNo)
{
	strField.MakeUpper();
	m_HeaderMap.SetAt(strField, nFieldNo);
}

//$$********************************************************************

int CGroupHeaderHelpers::GetNumericField(CString strField)
{
	CArray<int, int> arrayHeader;
	CreateNumericHeader(strField, arrayHeader);

	if (arrayHeader.GetSize() > 0)
	{
		return arrayHeader.GetAt(0);
	}
	else
	{
		return -1;
	}
}

//$$********************************************************************

void CGroupHeaderHelpers::CreateNumericHeader(CString strHeader, CArray<int, int>& arrayHeader)
{
	CCSV csvHeader(strHeader);
	CreateNumericHeader(csvHeader, arrayHeader);
}

//$$********************************************************************

void CGroupHeaderHelpers::CreateNumericHeader(CCSV& csvHeader, CArray<int, int>& arrayHeader)
{
	arrayHeader.RemoveAll();

	if (m_HeaderMap.GetCount() == 0)
	{
		AddFieldToHeaderMap(Group::GroupNo.Label, GROUPFIELD_GROUPNO);
		AddFieldToHeaderMap(Group::GroupName.Label, GROUPFIELD_GROUPNAME);
		AddFieldToHeaderMap("Name", GROUPFIELD_GROUPNAME);
		AddFieldToHeaderMap(Group::UserType.Label, GROUPFIELD_USERTYPE);
		AddFieldToHeaderMap("UType", GROUPFIELD_USERTYPE);
		AddFieldToHeaderMap(Group::SetNo.Label, GROUPFIELD_SETNO);
		AddFieldToHeaderMap(Group::PriceLevel.Label, GROUPFIELD_PRICELEVEL);
		AddFieldToHeaderMap("PLevel", GROUPFIELD_PRICELEVEL);
		AddFieldToHeaderMap(Group::VATShift.Label, GROUPFIELD_VATSHIFT);
		AddFieldToHeaderMap("VShift", GROUPFIELD_VATSHIFT);
		/*****/
		AddFieldToHeaderMap(Group::RefreshType.Label, GROUPFIELD_REFRESH_TYPE);
		AddFieldToHeaderMap("RFType", GROUPFIELD_REFRESH_TYPE);
		AddFieldToHeaderMap(Group::RefreshFlags.Label, GROUPFIELD_REFRESH_FLAGS);
		AddFieldToHeaderMap("RFAllow", GROUPFIELD_REFRESH_FLAGS);
		AddFieldToHeaderMap(Group::RefreshValue.Label, GROUPFIELD_REFRESH_VALUE);
		AddFieldToHeaderMap("RFVal", GROUPFIELD_REFRESH_VALUE);
		AddFieldToHeaderMap(Group::RefreshAppend.Label, GROUPFIELD_REFRESH_APPEND);
		AddFieldToHeaderMap("RFAppend", GROUPFIELD_REFRESH_APPEND);
		/*****/
		AddFieldToHeaderMap(Group::RefreshTypePeriod1.Label, GROUPFIELD_REFRESH_TYPE_PERIOD1);
		AddFieldToHeaderMap("RFT1", GROUPFIELD_REFRESH_TYPE_PERIOD1);
		AddFieldToHeaderMap(Group::RefreshTypePeriod2.Label, GROUPFIELD_REFRESH_TYPE_PERIOD2);
		AddFieldToHeaderMap("RFT2", GROUPFIELD_REFRESH_TYPE_PERIOD2);
		AddFieldToHeaderMap(Group::RefreshTypePeriod3.Label, GROUPFIELD_REFRESH_TYPE_PERIOD3);
		AddFieldToHeaderMap("RFT3", GROUPFIELD_REFRESH_TYPE_PERIOD3);
		AddFieldToHeaderMap(Group::RefreshTypePeriod4.Label, GROUPFIELD_REFRESH_TYPE_PERIOD4);
		AddFieldToHeaderMap("RFT4", GROUPFIELD_REFRESH_TYPE_PERIOD4);
		AddFieldToHeaderMap(Group::RefreshTypePeriod5.Label, GROUPFIELD_REFRESH_TYPE_PERIOD5);
		AddFieldToHeaderMap("RFT5", GROUPFIELD_REFRESH_TYPE_PERIOD5);
		AddFieldToHeaderMap(Group::RefreshTypePeriod6.Label, GROUPFIELD_REFRESH_TYPE_PERIOD6);
		AddFieldToHeaderMap("RFT6", GROUPFIELD_REFRESH_TYPE_PERIOD6);
		/*****/
		AddFieldToHeaderMap(Group::RefreshValuePeriod1.Label, GROUPFIELD_REFRESH_VALUE_PERIOD1);
		AddFieldToHeaderMap("RF1", GROUPFIELD_REFRESH_VALUE_PERIOD1);
		AddFieldToHeaderMap(Group::RefreshValuePeriod2.Label, GROUPFIELD_REFRESH_VALUE_PERIOD2);
		AddFieldToHeaderMap("RF2", GROUPFIELD_REFRESH_VALUE_PERIOD2);
		AddFieldToHeaderMap(Group::RefreshValuePeriod3.Label, GROUPFIELD_REFRESH_VALUE_PERIOD3);
		AddFieldToHeaderMap("RF3", GROUPFIELD_REFRESH_VALUE_PERIOD3);
		AddFieldToHeaderMap(Group::RefreshValuePeriod4.Label, GROUPFIELD_REFRESH_VALUE_PERIOD4);
		AddFieldToHeaderMap("RF4", GROUPFIELD_REFRESH_VALUE_PERIOD4);
		AddFieldToHeaderMap(Group::RefreshValuePeriod5.Label, GROUPFIELD_REFRESH_VALUE_PERIOD5);
		AddFieldToHeaderMap("RF5", GROUPFIELD_REFRESH_VALUE_PERIOD5);
		AddFieldToHeaderMap(Group::RefreshValuePeriod6.Label, GROUPFIELD_REFRESH_VALUE_PERIOD6);
		AddFieldToHeaderMap("RF6", GROUPFIELD_REFRESH_VALUE_PERIOD6);
		/*****/
		AddFieldToHeaderMap(Group::Purse1Allowed.Label, GROUPFIELD_PURSE1_ALLOW);
		AddFieldToHeaderMap("P1Allow", GROUPFIELD_PURSE1_ALLOW);
		AddFieldToHeaderMap(Group::Purse2Allowed.Label, GROUPFIELD_PURSE2_ALLOW);
		AddFieldToHeaderMap("P2Allow", GROUPFIELD_PURSE2_ALLOW);
		AddFieldToHeaderMap(Group::Purse3Allowed.Label, GROUPFIELD_PURSE3_ALLOW);
		AddFieldToHeaderMap("P3Allow", GROUPFIELD_PURSE3_ALLOW);
		/*****/
		AddFieldToHeaderMap(Group::MaxSpendType.Label, GROUPFIELD_MAXSPEND_TYPE);
		AddFieldToHeaderMap("MSType", GROUPFIELD_MAXSPEND_TYPE);
		/*****/
		AddFieldToHeaderMap(Group::MaxSpendPurse1Period1.Label, GROUPFIELD_MAXSPEND_PURSE1_PERIOD1);
		AddFieldToHeaderMap("MSV1", GROUPFIELD_MAXSPEND_PURSE1_PERIOD1);
		AddFieldToHeaderMap(Group::MaxSpendPurse1Period2.Label, GROUPFIELD_MAXSPEND_PURSE1_PERIOD2);
		AddFieldToHeaderMap("MSV2", GROUPFIELD_MAXSPEND_PURSE1_PERIOD2);
		AddFieldToHeaderMap(Group::MaxSpendPurse1Period3.Label, GROUPFIELD_MAXSPEND_PURSE1_PERIOD3);
		AddFieldToHeaderMap("MSV3", GROUPFIELD_MAXSPEND_PURSE1_PERIOD3);
		AddFieldToHeaderMap(Group::MaxSpendPurse1Period4.Label, GROUPFIELD_MAXSPEND_PURSE1_PERIOD4);
		AddFieldToHeaderMap("MSV4", GROUPFIELD_MAXSPEND_PURSE1_PERIOD4);
		AddFieldToHeaderMap(Group::MaxSpendPurse1Period5.Label, GROUPFIELD_MAXSPEND_PURSE1_PERIOD5);
		AddFieldToHeaderMap("MSV5", GROUPFIELD_MAXSPEND_PURSE1_PERIOD5);
		AddFieldToHeaderMap(Group::MaxSpendPurse1Period6.Label, GROUPFIELD_MAXSPEND_PURSE1_PERIOD6);
		AddFieldToHeaderMap("MSV6", GROUPFIELD_MAXSPEND_PURSE1_PERIOD6);
		/*****/
		AddFieldToHeaderMap(Group::MaxSpendPurse3Period1.Label, GROUPFIELD_MAXSPEND_PURSE3_PERIOD1);
		AddFieldToHeaderMap("MSV31", GROUPFIELD_MAXSPEND_PURSE3_PERIOD1);
		AddFieldToHeaderMap(Group::MaxSpendPurse3Period2.Label, GROUPFIELD_MAXSPEND_PURSE3_PERIOD2);
		AddFieldToHeaderMap("MSV32", GROUPFIELD_MAXSPEND_PURSE3_PERIOD2);
		AddFieldToHeaderMap(Group::MaxSpendPurse3Period3.Label, GROUPFIELD_MAXSPEND_PURSE3_PERIOD3);
		AddFieldToHeaderMap("MSV33", GROUPFIELD_MAXSPEND_PURSE3_PERIOD3);
		AddFieldToHeaderMap(Group::MaxSpendPurse3Period4.Label, GROUPFIELD_MAXSPEND_PURSE3_PERIOD4);
		AddFieldToHeaderMap("MSV34", GROUPFIELD_MAXSPEND_PURSE3_PERIOD4);
		AddFieldToHeaderMap(Group::MaxSpendPurse3Period5.Label, GROUPFIELD_MAXSPEND_PURSE3_PERIOD5);
		AddFieldToHeaderMap("MSV35", GROUPFIELD_MAXSPEND_PURSE3_PERIOD5);
		AddFieldToHeaderMap(Group::MaxSpendPurse3Period6.Label, GROUPFIELD_MAXSPEND_PURSE3_PERIOD6);
		AddFieldToHeaderMap("MSV36", GROUPFIELD_MAXSPEND_PURSE3_PERIOD6);
		/*****/
		AddFieldToHeaderMap(Group::MaxOverdraftPurse1Period1.Label, GROUPFIELD_MAXOVERDRAFT_PURSE1_PERIOD1);
		AddFieldToHeaderMap("OSV1", GROUPFIELD_MAXOVERDRAFT_PURSE1_PERIOD1);
		AddFieldToHeaderMap(Group::MaxOverdraftPurse1Period2.Label, GROUPFIELD_MAXOVERDRAFT_PURSE1_PERIOD2);
		AddFieldToHeaderMap("OSV2", GROUPFIELD_MAXOVERDRAFT_PURSE1_PERIOD2);
		AddFieldToHeaderMap(Group::MaxOverdraftPurse1Period3.Label, GROUPFIELD_MAXOVERDRAFT_PURSE1_PERIOD3);
		AddFieldToHeaderMap("OSV3", GROUPFIELD_MAXOVERDRAFT_PURSE1_PERIOD3);
		AddFieldToHeaderMap(Group::MaxOverdraftPurse1Period4.Label, GROUPFIELD_MAXOVERDRAFT_PURSE1_PERIOD4);
		AddFieldToHeaderMap("OSV4", GROUPFIELD_MAXOVERDRAFT_PURSE1_PERIOD4);
		AddFieldToHeaderMap(Group::MaxOverdraftPurse1Period5.Label, GROUPFIELD_MAXOVERDRAFT_PURSE1_PERIOD5);
		AddFieldToHeaderMap("OSV5", GROUPFIELD_MAXOVERDRAFT_PURSE1_PERIOD5);
		AddFieldToHeaderMap(Group::MaxOverdraftPurse1Period6.Label, GROUPFIELD_MAXOVERDRAFT_PURSE1_PERIOD6);
		AddFieldToHeaderMap("OSV6", GROUPFIELD_MAXOVERDRAFT_PURSE1_PERIOD6);
		/*****/
		AddFieldToHeaderMap(Group::MaxOverdraftPurse3Period1.Label, GROUPFIELD_MAXOVERDRAFT_PURSE3_PERIOD1);
		AddFieldToHeaderMap("OSV31", GROUPFIELD_MAXOVERDRAFT_PURSE3_PERIOD1);
		AddFieldToHeaderMap(Group::MaxOverdraftPurse3Period2.Label, GROUPFIELD_MAXOVERDRAFT_PURSE3_PERIOD1);
		AddFieldToHeaderMap("OSV32", GROUPFIELD_MAXOVERDRAFT_PURSE3_PERIOD2);
		AddFieldToHeaderMap(Group::MaxOverdraftPurse3Period3.Label, GROUPFIELD_MAXOVERDRAFT_PURSE3_PERIOD1);
		AddFieldToHeaderMap("OSV33", GROUPFIELD_MAXOVERDRAFT_PURSE3_PERIOD3);
		AddFieldToHeaderMap(Group::MaxOverdraftPurse3Period4.Label, GROUPFIELD_MAXOVERDRAFT_PURSE3_PERIOD1);
		AddFieldToHeaderMap("OSV34", GROUPFIELD_MAXOVERDRAFT_PURSE3_PERIOD4);
		AddFieldToHeaderMap(Group::MaxOverdraftPurse3Period5.Label, GROUPFIELD_MAXOVERDRAFT_PURSE3_PERIOD1);
		AddFieldToHeaderMap("OSV35", GROUPFIELD_MAXOVERDRAFT_PURSE3_PERIOD5);
		AddFieldToHeaderMap(Group::MaxOverdraftPurse3Period6.Label, GROUPFIELD_MAXOVERDRAFT_PURSE3_PERIOD1);
		AddFieldToHeaderMap("OSV36", GROUPFIELD_MAXOVERDRAFT_PURSE3_PERIOD5);
		/*****/
		AddFieldToHeaderMap(Group::DeptInhibitPurse1.Label, GROUPFIELD_DEPTINHIBIT_PURSE1);
		AddFieldToHeaderMap("DLP1", GROUPFIELD_DEPTINHIBIT_PURSE1);
		AddFieldToHeaderMap(Group::DeptInhibitPurse2.Label, GROUPFIELD_DEPTINHIBIT_PURSE2);
		AddFieldToHeaderMap("DLP2", GROUPFIELD_DEPTINHIBIT_PURSE2);
		AddFieldToHeaderMap(Group::DeptInhibitPurse3.Label, GROUPFIELD_DEPTINHIBIT_PURSE3);
		AddFieldToHeaderMap("DLP3", GROUPFIELD_DEPTINHIBIT_PURSE3);
		AddFieldToHeaderMap(Group::DeptInhibitCash.Label, GROUPFIELD_DEPTINHIBIT_CASH);
		AddFieldToHeaderMap("DLP0", GROUPFIELD_DEPTINHIBIT_CASH);
		/*****/
		AddFieldToHeaderMap(Group::LowBalancePurse1.Label, GROUPFIELD_LOWBALANCE_PURSE1);
		AddFieldToHeaderMap("LBP1", GROUPFIELD_LOWBALANCE_PURSE1);
		AddFieldToHeaderMap(Group::LowBalancePurse2.Label, GROUPFIELD_LOWBALANCE_PURSE2);
		AddFieldToHeaderMap("LBP2", GROUPFIELD_LOWBALANCE_PURSE1);
		AddFieldToHeaderMap(Group::LowBalancePurse3.Label, GROUPFIELD_LOWBALANCE_PURSE3);
		AddFieldToHeaderMap("LBP3", GROUPFIELD_LOWBALANCE_PURSE1);
		/*****/
		AddFieldToHeaderMap(Group::PointsAchievement.Label, GROUPFIELD_POINTS_ACHIEVEMENT);
		AddFieldToHeaderMap("PAL1", GROUPFIELD_POINTS_ACHIEVEMENT);
		/*****/
		AddFieldToHeaderMap(Group::RevalueFlag.Label, GROUPFIELD_REVALUE_FLAG);
		AddFieldToHeaderMap("RVFlag", GROUPFIELD_REVALUE_FLAG);
		AddFieldToHeaderMap(Group::RevalueDisable.Label, GROUPFIELD_REVALUE_DISABLE);
		AddFieldToHeaderMap("RVDis", GROUPFIELD_REVALUE_DISABLE);
		AddFieldToHeaderMap(Group::RevalueType.Label, GROUPFIELD_REVALUE_TYPE);
		AddFieldToHeaderMap("RVType", GROUPFIELD_REVALUE_TYPE);
		AddFieldToHeaderMap(Group::RevalueTrip1.Label, GROUPFIELD_REVALUE_TRIP1);
		AddFieldToHeaderMap("RVTrp1", GROUPFIELD_REVALUE_TRIP1);
		AddFieldToHeaderMap(Group::RevalueTrip2.Label, GROUPFIELD_REVALUE_TRIP2);
		AddFieldToHeaderMap("RVTrp2", GROUPFIELD_REVALUE_TRIP2);
		AddFieldToHeaderMap(Group::RevaluePoints.Label, GROUPFIELD_REVALUE_POINTS);
		AddFieldToHeaderMap("RVPts", GROUPFIELD_REVALUE_POINTS);
		AddFieldToHeaderMap(Group::RevalueBonus.Label, GROUPFIELD_REVALUE_BONUS);
		AddFieldToHeaderMap("RVVal", GROUPFIELD_REVALUE_BONUS);
		/*****/
		AddFieldToHeaderMap(Group::RewardFlag.Label, GROUPFIELD_REWARD_FLAG);
		AddFieldToHeaderMap("LYFlag", GROUPFIELD_REWARD_FLAG);
		AddFieldToHeaderMap(Group::RewardDisable.Label, GROUPFIELD_REWARD_DISABLE);
		AddFieldToHeaderMap("LYDis", GROUPFIELD_REWARD_DISABLE);
		AddFieldToHeaderMap(Group::RewardType.Label, GROUPFIELD_REWARD_TYPE);
		AddFieldToHeaderMap("LYType", GROUPFIELD_REWARD_TYPE);
		AddFieldToHeaderMap(Group::RewardTrip.Label, GROUPFIELD_REWARD_TRIP);
		AddFieldToHeaderMap("LYTrp", GROUPFIELD_REWARD_TRIP);
		AddFieldToHeaderMap(Group::RewardPoints.Label, GROUPFIELD_REWARD_POINTS);
		AddFieldToHeaderMap("LYPts", GROUPFIELD_REWARD_POINTS);
		AddFieldToHeaderMap(Group::RewardBonus.Label, GROUPFIELD_REWARD_BONUS);
		AddFieldToHeaderMap("LYVal", GROUPFIELD_REWARD_BONUS);
		AddFieldToHeaderMap(Group::RewardLimit.Label, GROUPFIELD_REWARD_LIMIT);
		AddFieldToHeaderMap("LYLmt", GROUPFIELD_REWARD_LIMIT);
		AddFieldToHeaderMap(Group::RewardTrigger.Label, GROUPFIELD_REWARD_TRIGGER);
		AddFieldToHeaderMap("LYTrg", GROUPFIELD_REWARD_TRIGGER);
		/*****/
		AddFieldToHeaderMap(Group::RedeemFlag.Label, GROUPFIELD_REDEEM_FLAG);
		AddFieldToHeaderMap("RDFlag", GROUPFIELD_REDEEM_FLAG);
		AddFieldToHeaderMap(Group::RedeemDisable.Label, GROUPFIELD_REDEEM_DISABLE);
		AddFieldToHeaderMap("RDDis", GROUPFIELD_REDEEM_DISABLE);
		AddFieldToHeaderMap(Group::RedeemTripAsInt.Label, GROUPFIELD_REDEEM_TRIP);
		AddFieldToHeaderMap("RDTrp", GROUPFIELD_REDEEM_TRIP);
		AddFieldToHeaderMap(Group::RedeemValue.Label, GROUPFIELD_REDEEM_VALUE);
		AddFieldToHeaderMap("RDVal", GROUPFIELD_REDEEM_VALUE);
		AddFieldToHeaderMap(Group::RedeemPurse.Label, GROUPFIELD_REDEEM_PURSE);
		AddFieldToHeaderMap("RDPur", GROUPFIELD_REDEEM_PURSE);
	}

	for (int n = 0; n < csvHeader.GetSize(); n++)
	{
		CString strField = csvHeader.GetString(n);
		strField.MakeUpper();

		int nIndex = 0;
		if (m_HeaderMap.Lookup(strField, nIndex) == 0)
		{
			arrayHeader.Add(-1);
		}
		else
		{
			arrayHeader.Add(nIndex);
		}
	}
}

//$$********************************************************************
