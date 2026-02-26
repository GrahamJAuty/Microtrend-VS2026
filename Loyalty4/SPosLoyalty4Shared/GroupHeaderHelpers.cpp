//$$********************************************************************
#include "..\SPosLoyalty4Shared\SQLNamespaces.h"
//$$********************************************************************
#include "GroupHeaderHelpers.h"
//$$********************************************************************

CGroupHeaderHelpers::CGroupHeaderHelpers()
{
	m_strHeaderFull = "";
	m_HeaderMap.InitHashTable(103);

	AddFieldToHeader(Group::GroupNo.Label);
	AddFieldToHeader(Group::GroupName.Label);
	AddFieldToHeader(Group::SchemeNo.Label);
	/*****/
	AddFieldToHeader(Group::RefreshType.Label);
	AddFieldToHeader(Group::RefreshFlags.Label);
	AddFieldToHeader(Group::RefreshValue.Label);
	AddFieldToHeader(Group::RefreshAppend.Label);
	/*****/
	AddFieldToHeader(Group::Purse1Allowed.Label);
	AddFieldToHeader(Group::Purse2Allowed.Label);
	/*****/
	AddFieldToHeader(Group::NoPluBonus.Label);
	AddFieldToHeader(Group::NoDeptBonus.Label);
	/*****/
	AddFieldToHeader(Group::MaxSpendType.Label);
	AddFieldToHeader(Group::MaxSpendPeriod1.Label);
	AddFieldToHeader(Group::MaxSpendPeriod2.Label);
	AddFieldToHeader(Group::MaxSpendPeriod3.Label);
	AddFieldToHeader(Group::MaxSpendPeriod4.Label);
	AddFieldToHeader(Group::MaxSpendPeriod5.Label);
	AddFieldToHeader(Group::MaxSpendPeriod6.Label);
	/*****/
	AddFieldToHeader(Group::MaxOverdraftPeriod1.Label);
	AddFieldToHeader(Group::MaxOverdraftPeriod2.Label);
	AddFieldToHeader(Group::MaxOverdraftPeriod3.Label);
	AddFieldToHeader(Group::MaxOverdraftPeriod4.Label);
	AddFieldToHeader(Group::MaxOverdraftPeriod5.Label);
	AddFieldToHeader(Group::MaxOverdraftPeriod6.Label);
	/*****/
	AddFieldToHeader(Group::LegacyRevalueFlag1.Label);
	AddFieldToHeader(Group::LegacyRevalueDisable1.Label);
	AddFieldToHeader(Group::LegacyRevalueTrip1.Label);
	AddFieldToHeader(Group::LegacyRevalueTrip2.Label);
	AddFieldToHeader(Group::LegacyRevalueType1.Label);
	AddFieldToHeader(Group::LegacyRevaluePoints1.Label);
	AddFieldToHeader(Group::LegacyRevalueBonus1.Label);
	AddFieldToHeader(Group::LegacyRevaluePercent1.Label);
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
	AddFieldToHeader(Group::RedeemType.Label);
	AddFieldToHeader(Group::RedeemTripAsInt.Label);
	AddFieldToHeader(Group::RedeemValue.Label);
}

//$$********************************************************************

CGroupHeaderHelpers::~CGroupHeaderHelpers()
{
	m_HeaderMap.RemoveAll();
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
		AddFieldToHeaderMap(Group::SchemeNo.Label, GROUPFIELD_SCHEMENO);
		/*****/
		AddFieldToHeaderMap(Group::RefreshType.Label, GROUPFIELD_REFRESH_TYPE);
		AddFieldToHeaderMap("RFType", GROUPFIELD_REFRESH_TYPE);
		AddFieldToHeaderMap(Group::RefreshFlags.Label, GROUPFIELD_REFRESH_FLAGS);
		AddFieldToHeaderMap("RFFlags", GROUPFIELD_REFRESH_FLAGS);
		AddFieldToHeaderMap(Group::RefreshValue.Label, GROUPFIELD_REFRESH_VALUE);
		AddFieldToHeaderMap("RFVal", GROUPFIELD_REFRESH_VALUE);
		AddFieldToHeaderMap(Group::RefreshAppend.Label, GROUPFIELD_REFRESH_APPEND);
		/*****/
		AddFieldToHeaderMap(Group::Purse1Allowed.Label, GROUPFIELD_PURSE1_ALLOW);
		AddFieldToHeaderMap("P1Allow", GROUPFIELD_PURSE1_ALLOW);
		AddFieldToHeaderMap(Group::Purse2Allowed.Label, GROUPFIELD_PURSE2_ALLOW);
		AddFieldToHeaderMap("P2Allow", GROUPFIELD_PURSE2_ALLOW);
		/*****/
		AddFieldToHeaderMap(Group::NoPluBonus.Label, GROUPFIELD_NOPLUBONUS);
		AddFieldToHeaderMap("NoPts", GROUPFIELD_NOPLUBONUS);
		AddFieldToHeaderMap(Group::NoDeptBonus.Label, GROUPFIELD_NODEPTBONUS);
		/*****/
		AddFieldToHeaderMap(Group::MaxSpendType.Label, GROUPFIELD_MAXSPEND_TYPE);
		AddFieldToHeaderMap("MSType", GROUPFIELD_MAXSPEND_TYPE);
		AddFieldToHeaderMap(Group::MaxSpendPeriod1.Label, GROUPFIELD_MAXSPEND_PERIOD1);
		AddFieldToHeaderMap("MSV1", GROUPFIELD_MAXSPEND_PERIOD1);
		AddFieldToHeaderMap(Group::MaxSpendPeriod2.Label, GROUPFIELD_MAXSPEND_PERIOD2);
		AddFieldToHeaderMap("MSV2", GROUPFIELD_MAXSPEND_PERIOD2);
		AddFieldToHeaderMap(Group::MaxSpendPeriod3.Label, GROUPFIELD_MAXSPEND_PERIOD3);
		AddFieldToHeaderMap("MSV3", GROUPFIELD_MAXSPEND_PERIOD3);
		AddFieldToHeaderMap(Group::MaxSpendPeriod4.Label, GROUPFIELD_MAXSPEND_PERIOD4);
		AddFieldToHeaderMap("MSV4", GROUPFIELD_MAXSPEND_PERIOD4);
		AddFieldToHeaderMap(Group::MaxSpendPeriod5.Label, GROUPFIELD_MAXSPEND_PERIOD5);
		AddFieldToHeaderMap("MSV5", GROUPFIELD_MAXSPEND_PERIOD5);
		AddFieldToHeaderMap(Group::MaxSpendPeriod6.Label, GROUPFIELD_MAXSPEND_PERIOD6);
		AddFieldToHeaderMap("MSV6", GROUPFIELD_MAXSPEND_PERIOD6);
		/*****/
		AddFieldToHeaderMap(Group::MaxOverdraftPeriod1.Label, GROUPFIELD_MAXOVERDRAFT_PERIOD1);
		AddFieldToHeaderMap("Overspend", GROUPFIELD_MAXOVERDRAFT_PERIOD1);
		AddFieldToHeaderMap(Group::MaxOverdraftPeriod2.Label, GROUPFIELD_MAXOVERDRAFT_PERIOD2);
		AddFieldToHeaderMap("OSV2", GROUPFIELD_MAXOVERDRAFT_PERIOD2);
		AddFieldToHeaderMap(Group::MaxOverdraftPeriod3.Label, GROUPFIELD_MAXOVERDRAFT_PERIOD3);
		AddFieldToHeaderMap("OSV3", GROUPFIELD_MAXOVERDRAFT_PERIOD3);
		AddFieldToHeaderMap(Group::MaxOverdraftPeriod4.Label, GROUPFIELD_MAXOVERDRAFT_PERIOD4);
		AddFieldToHeaderMap("OSV4", GROUPFIELD_MAXOVERDRAFT_PERIOD4);
		AddFieldToHeaderMap(Group::MaxOverdraftPeriod5.Label, GROUPFIELD_MAXOVERDRAFT_PERIOD5);
		AddFieldToHeaderMap("OSV5", GROUPFIELD_MAXOVERDRAFT_PERIOD5);
		AddFieldToHeaderMap(Group::MaxOverdraftPeriod6.Label, GROUPFIELD_MAXOVERDRAFT_PERIOD6);
		AddFieldToHeaderMap("OSV6", GROUPFIELD_MAXOVERDRAFT_PERIOD6);
		/*****/
		AddFieldToHeaderMap(Group::LegacyRevalueFlag1.Label, GROUPFIELD_REVALUE_FLAG);
		AddFieldToHeaderMap("RFlag", GROUPFIELD_REVALUE_FLAG);
		AddFieldToHeaderMap(Group::LegacyRevalueDisable1.Label, GROUPFIELD_REVALUE_DISABLE);
		AddFieldToHeaderMap("RDis", GROUPFIELD_REVALUE_DISABLE);
		AddFieldToHeaderMap(Group::LegacyRevalueType1.Label, GROUPFIELD_REVALUE_TYPE);
		AddFieldToHeaderMap("RType", GROUPFIELD_REVALUE_TYPE);
		AddFieldToHeaderMap(Group::LegacyRevalueTrip1.Label, GROUPFIELD_REVALUE_TRIP1);
		AddFieldToHeaderMap("RTrip1", GROUPFIELD_REVALUE_TRIP1);
		AddFieldToHeaderMap(Group::LegacyRevalueTrip2.Label, GROUPFIELD_REVALUE_TRIP2);
		AddFieldToHeaderMap("RTrip2", GROUPFIELD_REVALUE_TRIP2);
		AddFieldToHeaderMap(Group::LegacyRevaluePoints1.Label, GROUPFIELD_REVALUE_POINTS);
		AddFieldToHeaderMap("RPts", GROUPFIELD_REVALUE_POINTS);
		AddFieldToHeaderMap(Group::LegacyRevalueBonus1.Label, GROUPFIELD_REVALUE_BONUS);
		AddFieldToHeaderMap("RVal", GROUPFIELD_REVALUE_BONUS);
		AddFieldToHeaderMap(Group::LegacyRevaluePercent1.Label, GROUPFIELD_REVALUE_PERCENT);
		AddFieldToHeaderMap("RPcnt", GROUPFIELD_REVALUE_PERCENT);
		/*****/
		AddFieldToHeaderMap(Group::RewardFlag.Label, GROUPFIELD_REWARD_FLAG);
		AddFieldToHeaderMap("LFlag", GROUPFIELD_REWARD_FLAG);
		AddFieldToHeaderMap(Group::RewardDisable.Label, GROUPFIELD_REWARD_DISABLE);
		AddFieldToHeaderMap("LDis", GROUPFIELD_REWARD_DISABLE);
		AddFieldToHeaderMap(Group::RewardType.Label, GROUPFIELD_REWARD_TYPE);
		AddFieldToHeaderMap("LType", GROUPFIELD_REWARD_TYPE);
		AddFieldToHeaderMap(Group::RewardTrip.Label, GROUPFIELD_REWARD_TRIP);
		AddFieldToHeaderMap("LTrip", GROUPFIELD_REWARD_TRIP);
		AddFieldToHeaderMap(Group::RewardPoints.Label, GROUPFIELD_REWARD_POINTS);
		AddFieldToHeaderMap("LPts", GROUPFIELD_REWARD_POINTS);
		AddFieldToHeaderMap(Group::RewardBonus.Label, GROUPFIELD_REWARD_BONUS);
		AddFieldToHeaderMap("LVal", GROUPFIELD_REWARD_BONUS);
		AddFieldToHeaderMap(Group::RewardLimit.Label, GROUPFIELD_REWARD_LIMIT);
		AddFieldToHeaderMap("LUlmt", GROUPFIELD_REWARD_LIMIT);
		AddFieldToHeaderMap(Group::RewardTrigger.Label, GROUPFIELD_REWARD_TRIGGER);
		AddFieldToHeaderMap("LTrig", GROUPFIELD_REWARD_TRIGGER);
		/*****/
		AddFieldToHeaderMap(Group::RedeemFlag.Label, GROUPFIELD_REDEEM_FLAG);
		AddFieldToHeaderMap("RDFlag", GROUPFIELD_REDEEM_FLAG);
		AddFieldToHeaderMap(Group::RedeemDisable.Label, GROUPFIELD_REDEEM_DISABLE);
		AddFieldToHeaderMap("RDDis", GROUPFIELD_REDEEM_DISABLE);
		AddFieldToHeaderMap(Group::RedeemType.Label, GROUPFIELD_REDEEM_TYPE);
		AddFieldToHeaderMap("RDTyp", GROUPFIELD_REDEEM_TYPE);
		AddFieldToHeaderMap(Group::RedeemTripAsInt.Label, GROUPFIELD_REDEEM_TRIP);
		AddFieldToHeaderMap("RDTrp", GROUPFIELD_REDEEM_TRIP);
		AddFieldToHeaderMap(Group::RedeemValue.Label, GROUPFIELD_REDEEM_VALUE);
		AddFieldToHeaderMap("RDVal", GROUPFIELD_REDEEM_VALUE);
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
