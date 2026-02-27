//**********************************************************************
#include "..\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "SQLRecordSetGroupFull.h"
//**********************************************************************

CSQLRecordSetMetadataGroupFull::CSQLRecordSetMetadataGroupFull(bool bForExistenceCheck) : CSQLRecordSetMetadata()
{
	if ( TRUE == bForExistenceCheck)
	{
		return;
	}

	AddIntField(&m_nGroupNo, Group::GroupNo.Label, 1);
	AddTextField(&m_strGroupName, Group::GroupName.Label);
	AddIntField(&m_nUserType, Group::UserType.Label);
	AddIntField(&m_nSetNo, Group::SetNo.Label);
	AddTextField(&m_strShiftGroups, Group::ShiftGroups.Label);
	AddIntField(&m_nPriceLevel, Group::PriceLevel.Label);
	AddIntField(&m_nVATShiftFlag, Group::VATShift.Label);
	AddBOOLField(&m_bPurchaseControlFlag, Group::PurchaseControlFlag.Label);
	AddBOOLField(&m_bAllowRevalKioskFlag, Group::AllowRevalKioskFlag.Label);
	/*****/
	AddIntField(&m_nRefreshType, Group::RefreshType.Label);
	AddIntField(&m_nRefreshFlags, Group::RefreshFlags.Label);
	AddDecimalField(&m_strRefreshValue, Group::RefreshValue.Label);
	AddIntField(&m_nRefreshAppendFlag, Group::RefreshAppend.Label);
	AddIntField(&m_nRefreshTypePeriod1, Group::RefreshTypePeriod1.Label);
	AddIntField(&m_nRefreshTypePeriod2, Group::RefreshTypePeriod2.Label);
	AddIntField(&m_nRefreshTypePeriod3, Group::RefreshTypePeriod3.Label);
	AddIntField(&m_nRefreshTypePeriod4, Group::RefreshTypePeriod4.Label);
	AddIntField(&m_nRefreshTypePeriod5, Group::RefreshTypePeriod5.Label);
	AddIntField(&m_nRefreshTypePeriod6, Group::RefreshTypePeriod6.Label);
	AddDecimalField(&m_strRefreshValuePeriod1, Group::RefreshValuePeriod1.Label);
	AddDecimalField(&m_strRefreshValuePeriod2, Group::RefreshValuePeriod2.Label);
	AddDecimalField(&m_strRefreshValuePeriod3, Group::RefreshValuePeriod3.Label);
	AddDecimalField(&m_strRefreshValuePeriod4, Group::RefreshValuePeriod4.Label);
	AddDecimalField(&m_strRefreshValuePeriod5, Group::RefreshValuePeriod5.Label);
	AddDecimalField(&m_strRefreshValuePeriod6, Group::RefreshValuePeriod6.Label);
	/*****/
	AddIntField(&m_nPurse1Allowed, Group::Purse1Allowed.Label);
	AddIntField(&m_nPurse2Allowed, Group::Purse2Allowed.Label);
	AddIntField(&m_nPurse3Allowed, Group::Purse3Allowed.Label);
	/*****/
	AddIntField(&m_nMaxSpendType, Group::MaxSpendType.Label);
	/*****/
	AddDecimalField(&m_strMaxSpend[0][0], Group::MaxSpendPurse1Period1.Label);
	AddDecimalField(&m_strMaxSpend[0][1], Group::MaxSpendPurse1Period2.Label);
	AddDecimalField(&m_strMaxSpend[0][2], Group::MaxSpendPurse1Period3.Label);
	AddDecimalField(&m_strMaxSpend[0][3], Group::MaxSpendPurse1Period4.Label);
	AddDecimalField(&m_strMaxSpend[0][4], Group::MaxSpendPurse1Period5.Label);
	AddDecimalField(&m_strMaxSpend[0][5], Group::MaxSpendPurse1Period6.Label);
	/*****/
	AddDecimalField(&m_strMaxSpend[1][0], Group::MaxSpendPurse3Period1.Label);
	AddDecimalField(&m_strMaxSpend[1][1], Group::MaxSpendPurse3Period2.Label);
	AddDecimalField(&m_strMaxSpend[1][2], Group::MaxSpendPurse3Period3.Label);
	AddDecimalField(&m_strMaxSpend[1][3], Group::MaxSpendPurse3Period4.Label);
	AddDecimalField(&m_strMaxSpend[1][4], Group::MaxSpendPurse3Period5.Label);
	AddDecimalField(&m_strMaxSpend[1][5], Group::MaxSpendPurse3Period6.Label);
	/*****/
	AddDecimalField(&m_strMaxOverdraft[0][0], Group::MaxOverdraftPurse1Period1.Label);
	AddDecimalField(&m_strMaxOverdraft[0][1], Group::MaxOverdraftPurse1Period2.Label);
	AddDecimalField(&m_strMaxOverdraft[0][2], Group::MaxOverdraftPurse1Period3.Label);
	AddDecimalField(&m_strMaxOverdraft[0][3], Group::MaxOverdraftPurse1Period4.Label);
	AddDecimalField(&m_strMaxOverdraft[0][4], Group::MaxOverdraftPurse1Period5.Label);
	AddDecimalField(&m_strMaxOverdraft[0][5], Group::MaxOverdraftPurse1Period6.Label);
	/*****/
	AddDecimalField(&m_strMaxOverdraft[1][0], Group::MaxOverdraftPurse3Period1.Label);
	AddDecimalField(&m_strMaxOverdraft[1][1], Group::MaxOverdraftPurse3Period2.Label);
	AddDecimalField(&m_strMaxOverdraft[1][2], Group::MaxOverdraftPurse3Period3.Label);
	AddDecimalField(&m_strMaxOverdraft[1][3], Group::MaxOverdraftPurse3Period4.Label);
	AddDecimalField(&m_strMaxOverdraft[1][4], Group::MaxOverdraftPurse3Period5.Label);
	AddDecimalField(&m_strMaxOverdraft[1][5], Group::MaxOverdraftPurse3Period6.Label);
	/*****/
	AddTextField(&m_strDeptInhibitPurse1, Group::DeptInhibitPurse1.Label);
	AddTextField(&m_strDeptInhibitPurse2, Group::DeptInhibitPurse2.Label);
	AddTextField(&m_strDeptInhibitPurse3, Group::DeptInhibitPurse3.Label);
	AddTextField(&m_strDeptInhibitCash, Group::DeptInhibitCash.Label);
	/*****/
	AddDecimalField(&m_strLowBalancePurse1, Group::LowBalancePurse1.Label);
	AddDecimalField(&m_strLowBalancePurse2, Group::LowBalancePurse2.Label);
	AddDecimalField(&m_strLowBalancePurse3, Group::LowBalancePurse3.Label);
	/*****/
	AddIntField(&m_nPointsAchievement, Group::PointsAchievement.Label);
	/*****/
	AddBOOLField(&m_bRevalueFlag, Group::RevalueFlag.Label, FALSE);
	AddBOOLField(&m_bRevalueDisable, Group::RevalueDisable.Label, FALSE);
	AddIntField(&m_nRevalueType, Group::RevalueType.Label);
	AddDecimalField(&m_strRevalueTrip1, Group::RevalueTrip1.Label);
	AddDecimalField(&m_strRevalueTrip2, Group::RevalueTrip2.Label);
	AddIntField(&m_nRevaluePoints, Group::RevaluePoints.Label);
	AddDecimalField(&m_strRevalueBonus, Group::RevalueBonus.Label);
	/*****/
	AddBOOLField(&m_bRewardFlag, Group::RewardFlag.Label, FALSE);
	AddBOOLField(&m_bRewardDisable, Group::RewardDisable.Label, FALSE);
	AddIntField(&m_nRewardType, Group::RewardType.Label);
	AddDecimalField(&m_strRewardTrip, Group::RewardTrip.Label);
	AddIntField(&m_nRewardPoints, Group::RewardPoints.Label);
	AddDecimalField(&m_strRewardBonus, Group::RewardBonus.Label);
	AddDecimalField(&m_strRewardLimit, Group::RewardLimit.Label);
	AddDecimalField(&m_strRewardTrigger, Group::RewardTrigger.Label);
	/*****/
	AddBOOLField(&m_bRedeemFlag, Group::RedeemFlag.Label, FALSE);
	AddBOOLField(&m_bRedeemDisable, Group::RedeemDisable.Label, FALSE);
	AddIntField(&m_nRedeemTrip, Group::RedeemTripAsInt.Label);
	AddDecimalField(&m_strRedeemValue, Group::RedeemValue.Label);
	AddIntField(&m_nRedeemPurse, Group::RedeemPurse.Label);
}

//**********************************************************************

void CSQLRecordSetMetadataGroupFull::AddParamsByGroupNo(const int nGroupNo)
{
	AddIntParam(Group::GroupNo.Label, nGroupNo);
}

//**********************************************************************

void CSQLRecordSetMetadataGroupFull::AddParamsByRange(const int nGroupNoFrom, const int nGroupNoTo)
{
	AddParamsByGroupNo(nGroupNoFrom);
	AddIntParam(Group::GroupNo.Label, nGroupNoTo);
}

//**********************************************************************

CSQLRecordSetGroupFull::CSQLRecordSetGroupFull(CDatabase* pDatabase, RSParams_Group_NormalNoParams params) :
	CSQLRecordSetConnectionPool(pDatabase),
	m_MD()
{
	CommonInit();
}

//**********************************************************************

CSQLRecordSetGroupFull::CSQLRecordSetGroupFull(CDatabase* pDatabase, RSParams_Group_NormalByGroupNo params, bool bForExistenceCheck) :
	CSQLRecordSetConnectionPool(pDatabase),
	m_MD(bForExistenceCheck)
{
	m_MD.AddParamsByGroupNo(params.nGroupNo);
	CommonInit();
}

//**********************************************************************

CSQLRecordSetGroupFull::CSQLRecordSetGroupFull(CDatabase* pDatabase, RSParams_Group_CountRowsByRange params) :
	CSQLRecordSetConnectionPool(pDatabase),
	m_MD(TRUE)
{
	m_bRangeFilter = TRUE;
	m_MD.AddParamsByRange(params.nGroupNoFrom, params.nGroupNoTo);
	CommonInit();
}

//**********************************************************************

void CSQLRecordSetGroupFull::CommonInit()
{
	m_nFields = m_MD.GetFieldCount();
	m_nParams = m_MD.GetParamCount();
	SetMetadata(&m_MD);
	SetInitialFilterString();
}

//**********************************************************************

CString CSQLRecordSetGroupFull::GetDefaultSQL()
{ 
    return SQLTableNames::AccountGroups; 
}

//**********************************************************************

void CSQLRecordSetGroupFull::LoadFromGroupRow(CSQLRowGroupFull& RowGroup)
{
    m_MD.m_nGroupNo = RowGroup.GetGroupNo();
	m_MD.m_strGroupName = RowGroup.GetGroupName();
	m_MD.m_nUserType = RowGroup.GetUserType();
	m_MD.m_nSetNo = RowGroup.GetSetNo();
	m_MD.m_strShiftGroups = RowGroup.GetShiftGroups();
	m_MD.m_nPriceLevel = RowGroup.GetPriceLevel();
	m_MD.m_nVATShiftFlag = RowGroup.GetVatShiftFlag();
	m_MD.m_bPurchaseControlFlag = RowGroup.GetPurchaseControlFlag();
	m_MD.m_bAllowRevalKioskFlag = RowGroup.GetAllowRevalKioskFlag();
    /*****/
	m_MD.m_nRefreshType = RowGroup.GetRefreshType();
	m_MD.m_nRefreshFlags = RowGroup.GetRefreshFlags();
	SetDoubleString(m_MD.m_strRefreshValue, RowGroup.GetRefreshValue());
	m_MD.m_nRefreshAppendFlag = RowGroup.GetRefreshAppendFlag();
	m_MD.m_nRefreshTypePeriod1 = RowGroup.GetRefreshTypePeriod1();
	m_MD.m_nRefreshTypePeriod2 = RowGroup.GetRefreshTypePeriod2();
	m_MD.m_nRefreshTypePeriod3 = RowGroup.GetRefreshTypePeriod3();
	m_MD.m_nRefreshTypePeriod4 = RowGroup.GetRefreshTypePeriod4();
	m_MD.m_nRefreshTypePeriod5 = RowGroup.GetRefreshTypePeriod5();
	m_MD.m_nRefreshTypePeriod6 = RowGroup.GetRefreshTypePeriod6();
	SetDoubleString(m_MD.m_strRefreshValuePeriod1, RowGroup.GetRefreshValuePeriod1());
	SetDoubleString(m_MD.m_strRefreshValuePeriod2, RowGroup.GetRefreshValuePeriod2());
	SetDoubleString(m_MD.m_strRefreshValuePeriod3, RowGroup.GetRefreshValuePeriod3());
	SetDoubleString(m_MD.m_strRefreshValuePeriod4, RowGroup.GetRefreshValuePeriod4());
	SetDoubleString(m_MD.m_strRefreshValuePeriod5, RowGroup.GetRefreshValuePeriod5());
	SetDoubleString(m_MD.m_strRefreshValuePeriod6, RowGroup.GetRefreshValuePeriod6());
    /*****/
	m_MD.m_nPurse1Allowed = RowGroup.GetPurse1Allowed();
	m_MD.m_nPurse2Allowed = RowGroup.GetPurse2Allowed();
	m_MD.m_nPurse3Allowed = RowGroup.GetPurse3Allowed();
    /*****/
	m_MD.m_nMaxSpendType = RowGroup.GetMaxSpendType();
    /*****/
	SetDoubleString(m_MD.m_strMaxSpend[0][0], RowGroup.GetMaxSpendPurse1Period1());
	SetDoubleString(m_MD.m_strMaxSpend[0][1], RowGroup.GetMaxSpendPurse1Period2());
	SetDoubleString(m_MD.m_strMaxSpend[0][2], RowGroup.GetMaxSpendPurse1Period3());
	SetDoubleString(m_MD.m_strMaxSpend[0][3], RowGroup.GetMaxSpendPurse1Period4());
	SetDoubleString(m_MD.m_strMaxSpend[0][4], RowGroup.GetMaxSpendPurse1Period5());
	SetDoubleString(m_MD.m_strMaxSpend[0][5], RowGroup.GetMaxSpendPurse1Period6());
	/*****/
	SetDoubleString(m_MD.m_strMaxSpend[1][0], RowGroup.GetMaxSpendPurse3Period1());
	SetDoubleString(m_MD.m_strMaxSpend[1][1], RowGroup.GetMaxSpendPurse3Period2());
	SetDoubleString(m_MD.m_strMaxSpend[1][2], RowGroup.GetMaxSpendPurse3Period3());
	SetDoubleString(m_MD.m_strMaxSpend[1][3], RowGroup.GetMaxSpendPurse3Period4());
	SetDoubleString(m_MD.m_strMaxSpend[1][4], RowGroup.GetMaxSpendPurse3Period5());
	SetDoubleString(m_MD.m_strMaxSpend[1][5], RowGroup.GetMaxSpendPurse3Period6());
	/*****/
	SetDoubleString(m_MD.m_strMaxOverdraft[0][0], RowGroup.GetMaxOverdraftPurse1Period1());
	SetDoubleString(m_MD.m_strMaxOverdraft[0][1], RowGroup.GetMaxOverdraftPurse1Period2());
	SetDoubleString(m_MD.m_strMaxOverdraft[0][2], RowGroup.GetMaxOverdraftPurse1Period3());
	SetDoubleString(m_MD.m_strMaxOverdraft[0][3], RowGroup.GetMaxOverdraftPurse1Period4());
	SetDoubleString(m_MD.m_strMaxOverdraft[0][4], RowGroup.GetMaxOverdraftPurse1Period5());
	SetDoubleString(m_MD.m_strMaxOverdraft[0][5], RowGroup.GetMaxOverdraftPurse1Period6());
	/*****/
	SetDoubleString(m_MD.m_strMaxOverdraft[1][0], RowGroup.GetMaxOverdraftPurse3Period1());
	SetDoubleString(m_MD.m_strMaxOverdraft[1][1], RowGroup.GetMaxOverdraftPurse3Period2());
	SetDoubleString(m_MD.m_strMaxOverdraft[1][2], RowGroup.GetMaxOverdraftPurse3Period3());
	SetDoubleString(m_MD.m_strMaxOverdraft[1][3], RowGroup.GetMaxOverdraftPurse3Period4());
	SetDoubleString(m_MD.m_strMaxOverdraft[1][4], RowGroup.GetMaxOverdraftPurse3Period5());
	SetDoubleString(m_MD.m_strMaxOverdraft[1][5], RowGroup.GetMaxOverdraftPurse3Period6());
    /*****/
	m_MD.m_strDeptInhibitPurse1 = RowGroup.GetDeptInhibitPurse1();
	m_MD.m_strDeptInhibitPurse2 = RowGroup.GetDeptInhibitPurse2();
	m_MD.m_strDeptInhibitPurse3 = RowGroup.GetDeptInhibitPurse3();
	m_MD.m_strDeptInhibitCash = RowGroup.GetDeptInhibitCash();
    /*****/
	SetDoubleString(m_MD.m_strLowBalancePurse1, RowGroup.GetLowBalancePurse1());
	SetDoubleString(m_MD.m_strLowBalancePurse2, RowGroup.GetLowBalancePurse2());
	SetDoubleString(m_MD.m_strLowBalancePurse3, RowGroup.GetLowBalancePurse3());
    /*****/
	m_MD.m_nPointsAchievement = RowGroup.GetPointsAchievement();
    /*****/
	m_MD.m_bRevalueFlag = RowGroup.GetFileRevalueFlag();
	m_MD.m_bRevalueDisable = RowGroup.GetRevalueDisable();
	m_MD.m_nRevalueType = RowGroup.GetRevalueType();
	SetDoubleString(m_MD.m_strRevalueTrip1, RowGroup.GetRevalueTrip1());
	SetDoubleString(m_MD.m_strRevalueTrip2, RowGroup.GetRevalueTrip2());
	m_MD.m_nRevaluePoints = RowGroup.GetRewardPoints();
	SetDoubleString(m_MD.m_strRevalueBonus, RowGroup.GetRevalueBonus());
    /*****/
	m_MD.m_bRewardFlag = RowGroup.GetFileRewardFlag();
	m_MD.m_bRewardDisable = RowGroup.GetRewardDisable();
	m_MD.m_nRewardType = RowGroup.GetRewardType();
	SetDoubleString(m_MD.m_strRewardTrip, RowGroup.GetRewardTrip());
	m_MD.m_nRewardPoints = RowGroup.GetRewardPoints();
	SetDoubleString(m_MD.m_strRewardBonus, RowGroup.GetRewardBonus());
    SetDoubleString(m_MD.m_strRewardLimit, RowGroup.GetRewardLimit());
	SetDoubleString(m_MD.m_strRewardTrigger, RowGroup.GetRewardTrigger());
    /*****/
	m_MD.m_bRedeemFlag = RowGroup.GetFileRedeemFlag();
	m_MD.m_bRedeemDisable = RowGroup.GetRedeemDisable();
	m_MD.m_nRedeemTrip = RowGroup.GetRedeemTrip();
	SetDoubleString(m_MD.m_strRedeemValue, RowGroup.GetRedeemValue());
	m_MD.m_nRedeemPurse = RowGroup.GetRedeemPurse();
}

//**********************************************************************

void CSQLRecordSetGroupFull::SaveToGroupRow(CSQLRowGroupFull& RowGroup)
{
    RowGroup.SetGroupNo(m_MD.m_nGroupNo);
    RowGroup.SetGroupName(m_MD.m_strGroupName);
    RowGroup.SetUserType(m_MD.m_nUserType);
    RowGroup.SetSetNo(m_MD.m_nSetNo);
    RowGroup.SetShiftGroups(m_MD.m_strShiftGroups);
    RowGroup.SetPriceLevel(m_MD.m_nPriceLevel);
    RowGroup.SetVatShiftFlag(m_MD.m_nVATShiftFlag);
    RowGroup.SetPurchaseControlFlag(m_MD.m_bPurchaseControlFlag);
	RowGroup.SetAllowRevalKioskFlag(m_MD.m_bAllowRevalKioskFlag);
    /*****/
    RowGroup.SetRefreshType(m_MD.m_nRefreshType);
    RowGroup.SetRefreshFlags(m_MD.m_nRefreshFlags);
	RowGroup.SetRefreshValue(atof(m_MD.m_strRefreshValue));
    RowGroup.SetRefreshAppendFlag(m_MD.m_nRefreshAppendFlag);
    RowGroup.SetRefreshTypePeriod1(m_MD.m_nRefreshTypePeriod1);
    RowGroup.SetRefreshTypePeriod2(m_MD.m_nRefreshTypePeriod2);
    RowGroup.SetRefreshTypePeriod3(m_MD.m_nRefreshTypePeriod3);
    RowGroup.SetRefreshTypePeriod4(m_MD.m_nRefreshTypePeriod4);
    RowGroup.SetRefreshTypePeriod5(m_MD.m_nRefreshTypePeriod5);
    RowGroup.SetRefreshTypePeriod6(m_MD.m_nRefreshTypePeriod6);
	RowGroup.SetRefreshValuePeriod1(atof(m_MD.m_strRefreshValuePeriod1));
	RowGroup.SetRefreshValuePeriod2(atof(m_MD.m_strRefreshValuePeriod2));
	RowGroup.SetRefreshValuePeriod3(atof(m_MD.m_strRefreshValuePeriod3));
    RowGroup.SetRefreshValuePeriod4(atof(m_MD.m_strRefreshValuePeriod4));
	RowGroup.SetRefreshValuePeriod5(atof(m_MD.m_strRefreshValuePeriod5));
    RowGroup.SetRefreshValuePeriod6(atof(m_MD.m_strRefreshValuePeriod6));
    /*****/
    RowGroup.SetPurse1Allowed(m_MD.m_nPurse1Allowed);
    RowGroup.SetPurse2Allowed(m_MD.m_nPurse2Allowed);
    RowGroup.SetPurse3Allowed(m_MD.m_nPurse3Allowed);
    /*****/
    RowGroup.SetMaxSpendType(m_MD.m_nMaxSpendType);
    /*****/
	RowGroup.SetMaxSpendPurse1Period1(atof(m_MD.m_strMaxSpend[0][0]));
	RowGroup.SetMaxSpendPurse1Period2(atof(m_MD.m_strMaxSpend[0][1]));
	RowGroup.SetMaxSpendPurse1Period3(atof(m_MD.m_strMaxSpend[0][2]));
	RowGroup.SetMaxSpendPurse1Period4(atof(m_MD.m_strMaxSpend[0][3]));
	RowGroup.SetMaxSpendPurse1Period5(atof(m_MD.m_strMaxSpend[0][4]));
	RowGroup.SetMaxSpendPurse1Period6(atof(m_MD.m_strMaxSpend[0][5]));
	/*****/
	RowGroup.SetMaxSpendPurse3Period1(atof(m_MD.m_strMaxSpend[1][0]));
	RowGroup.SetMaxSpendPurse3Period2(atof(m_MD.m_strMaxSpend[1][1]));
	RowGroup.SetMaxSpendPurse3Period3(atof(m_MD.m_strMaxSpend[1][2]));
	RowGroup.SetMaxSpendPurse3Period4(atof(m_MD.m_strMaxSpend[1][3]));
	RowGroup.SetMaxSpendPurse3Period5(atof(m_MD.m_strMaxSpend[1][4]));
	RowGroup.SetMaxSpendPurse3Period6(atof(m_MD.m_strMaxSpend[1][5]));
	/*****/
	RowGroup.SetMaxOverdraftPurse1Period1(atof(m_MD.m_strMaxOverdraft[0][0]));
	RowGroup.SetMaxOverdraftPurse1Period2(atof(m_MD.m_strMaxOverdraft[0][1]));
	RowGroup.SetMaxOverdraftPurse1Period3(atof(m_MD.m_strMaxOverdraft[0][2]));
	RowGroup.SetMaxOverdraftPurse1Period4(atof(m_MD.m_strMaxOverdraft[0][3]));
	RowGroup.SetMaxOverdraftPurse1Period5(atof(m_MD.m_strMaxOverdraft[0][4]));
	RowGroup.SetMaxOverdraftPurse1Period6(atof(m_MD.m_strMaxOverdraft[0][5]));
	/*****/
	RowGroup.SetMaxOverdraftPurse3Period1(atof(m_MD.m_strMaxOverdraft[1][0]));
	RowGroup.SetMaxOverdraftPurse3Period2(atof(m_MD.m_strMaxOverdraft[1][1]));
	RowGroup.SetMaxOverdraftPurse3Period3(atof(m_MD.m_strMaxOverdraft[1][2]));
	RowGroup.SetMaxOverdraftPurse3Period4(atof(m_MD.m_strMaxOverdraft[1][3]));
	RowGroup.SetMaxOverdraftPurse3Period5(atof(m_MD.m_strMaxOverdraft[1][4]));
	RowGroup.SetMaxOverdraftPurse3Period6(atof(m_MD.m_strMaxOverdraft[1][5]));
    /*****/
    RowGroup.SetDeptInhibitPurse1(m_MD.m_strDeptInhibitPurse1);
    RowGroup.SetDeptInhibitPurse2(m_MD.m_strDeptInhibitPurse2);
    RowGroup.SetDeptInhibitPurse3(m_MD.m_strDeptInhibitPurse3);
    RowGroup.SetDeptInhibitCash(m_MD.m_strDeptInhibitCash);
    /*****/
	RowGroup.SetLowBalancePurse1(atof(m_MD.m_strLowBalancePurse1));
    RowGroup.SetLowBalancePurse2(atof(m_MD.m_strLowBalancePurse2));
	RowGroup.SetLowBalancePurse3(atof(m_MD.m_strLowBalancePurse3));
    /*****/
    RowGroup.SetPointsAchievement(m_MD.m_nPointsAchievement);
    /*****/
    RowGroup.SetFileRevalueFlag(m_MD.m_bRevalueFlag);
    RowGroup.SetRevalueDisable(m_MD.m_bRevalueDisable);
    RowGroup.SetRevalueType(m_MD.m_nRevalueType);
	RowGroup.SetRevalueTrip1(atof(m_MD.m_strRevalueTrip1));
	RowGroup.SetRevalueTrip2(atof(m_MD.m_strRevalueTrip2));
    RowGroup.SetRevaluePoints(m_MD.m_nRevaluePoints);
	RowGroup.SetRevalueBonus(atof(m_MD.m_strRevalueBonus));
    /*****/
    RowGroup.SetFileRewardFlag(m_MD.m_bRewardFlag);
    RowGroup.SetRewardDisable(m_MD.m_bRewardDisable);
    RowGroup.SetRewardType(m_MD.m_nRewardType);
	RowGroup.SetRewardTrip(atof(m_MD.m_strRewardTrip));
    RowGroup.SetRewardPoints(m_MD.m_nRewardPoints);
	RowGroup.SetRewardBonus(atof(m_MD.m_strRewardBonus));
	RowGroup.SetRewardLimit(atof(m_MD.m_strRewardLimit));
    RowGroup.SetRewardTrigger(atof(m_MD.m_strRewardTrigger));
    /*****/
    RowGroup.SetFileRedeemFlag(m_MD.m_bRedeemFlag);
    RowGroup.SetRedeemDisable(m_MD.m_bRedeemDisable);
    RowGroup.SetRedeemTrip(m_MD.m_nRedeemTrip);
	RowGroup.SetRedeemValue(atof(m_MD.m_strRedeemValue));
    RowGroup.SetRedeemPurse(m_MD.m_nRedeemPurse);
}

//**********************************************************************

bool CSQLRecordSetGroupFull::StepSelectAll(CSQLRowGroupFull& RowGroup)
{
    if (IsOpen() == FALSE)
    {
        if (Open(CRecordset::forwardOnly, NULL, CRecordset::readOnly) == FALSE)
        {
            return FALSE;
        }
    }

    if (IsEOF() == TRUE)
    {
        return FALSE;
    }

    SaveToGroupRow(RowGroup);
    MoveNext();
    
    return TRUE;
}

//**********************************************************************
