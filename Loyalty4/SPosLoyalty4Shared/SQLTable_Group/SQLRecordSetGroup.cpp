//**********************************************************************
#include "..\SQLNamespaces.h"
//**********************************************************************
#include "SQLRecordSetGroup.h"
//**********************************************************************

CSQLRecordSetMetadataGroup::CSQLRecordSetMetadataGroup(bool bForExistenceCheck) : CSQLRecordSetMetadata()
{
    if (FALSE == bForExistenceCheck)
    {
        AddIntField(&m_nGroupNo, Group::GroupNo.Label, 1);
        AddTextField(&m_strGroupName, Group::GroupName.Label);
        AddIntField(&m_nSchemeNo, Group::SchemeNo.Label, 1);
        AddBOOLField(&m_bStampOfferFlag, Group::StampOfferFlag.Label, FALSE);
        /*****/
        AddIntField(&m_nRefreshType, Group::RefreshType.Label, 1);
        AddIntField(&m_nRefreshFlags, Group::RefreshFlags.Label);
        AddDecimalField(&m_strRefreshValue, Group::RefreshValue.Label);
        AddIntField(&m_nRefreshAppend, Group::RefreshAppend.Label);
        /****/
        AddIntField(&m_nPurse1Allowed, Group::Purse1Allowed.Label);
        AddIntField(&m_nPurse2Allowed, Group::Purse2Allowed.Label);
        /****/
        AddBOOLField(&m_bNoPluBonus, Group::NoPluBonus.Label, FALSE);
        AddBOOLField(&m_bNoDeptBonus, Group::NoDeptBonus.Label, FALSE);
        /****/
        AddIntField(&m_nMaxSpendType, Group::MaxSpendType.Label, 1);
        /****/
        AddDecimalField(&m_strMaxSpend[0], Group::MaxSpendPeriod1.Label);
        AddDecimalField(&m_strMaxSpend[1], Group::MaxSpendPeriod2.Label);
        AddDecimalField(&m_strMaxSpend[2], Group::MaxSpendPeriod3.Label);
        AddDecimalField(&m_strMaxSpend[3], Group::MaxSpendPeriod4.Label);
        AddDecimalField(&m_strMaxSpend[4], Group::MaxSpendPeriod5.Label);
        AddDecimalField(&m_strMaxSpend[5], Group::MaxSpendPeriod6.Label);
        AddDecimalField(&m_strMaxOverdraft[0], Group::MaxOverdraftPeriod1.Label);
        AddDecimalField(&m_strMaxOverdraft[1], Group::MaxOverdraftPeriod2.Label);
        AddDecimalField(&m_strMaxOverdraft[2], Group::MaxOverdraftPeriod3.Label);
        AddDecimalField(&m_strMaxOverdraft[3], Group::MaxOverdraftPeriod4.Label);
        AddDecimalField(&m_strMaxOverdraft[4], Group::MaxOverdraftPeriod5.Label);
        AddDecimalField(&m_strMaxOverdraft[5], Group::MaxOverdraftPeriod6.Label);
        /****/
        AddBOOLField(&m_bRewardFlag, Group::RewardFlag.Label, FALSE);
        AddBOOLField(&m_bRewardDisable, Group::RewardDisable.Label, FALSE);
        AddIntField(&m_nRewardType, Group::RewardType.Label, 1);
        AddDecimalField(&m_strRewardTrip, Group::RewardTrip.Label);
        AddIntField(&m_nRewardPoints, Group::RewardPoints.Label, 0);
        AddDecimalField(&m_strRewardBonus, Group::RewardBonus.Label);
        AddDecimalField(&m_strRewardLimit, Group::RewardLimit.Label);
        AddDecimalField(&m_strRewardTrigger, Group::RewardTrigger.Label);
        /****/
        AddBOOLField(&m_bRedeemFlag, Group::RedeemFlag.Label, FALSE);
        AddBOOLField(&m_bRedeemDisable, Group::RedeemDisable.Label, FALSE);
        AddIntField(&m_nRedeemType, Group::RedeemType.Label, 1);
        AddIntField(&m_nRedeemTrip, Group::RedeemTripAsInt.Label, 0);
        AddDecimalField(&m_strRedeemValue, Group::RedeemValue.Label);
    }
}

//**********************************************************************

void CSQLRecordSetMetadataGroup::AddParamsByGroupNo(const int nGroupNo)
{
    AddIntParam(Group::GroupNo.Label, nGroupNo);
}

//**********************************************************************

CSQLRecordSetGroup::CSQLRecordSetGroup(CDatabase* pDatabase, RSParams_Group_NormalNoParams params) :
    CSQLRecordSetConnectionPool(pDatabase),
    m_MD()
{
    CommonInit();
}

//**********************************************************************

CSQLRecordSetGroup::CSQLRecordSetGroup(CDatabase* pDatabase, RSParams_Group_NormalByGroupNo params, bool bForExistenceCheck) :
    CSQLRecordSetConnectionPool(pDatabase),
    m_MD(bForExistenceCheck)
{
    m_MD.AddParamsByGroupNo(params.nGroupNo);
    CommonInit();
}

//**********************************************************************

void CSQLRecordSetGroup::CommonInit()
{
    m_nFields = m_MD.GetFieldCount();
    m_nParams = m_MD.GetParamCount();
    SetMetadata(&m_MD);
    SetInitialFilterString();
}

//**********************************************************************

CString CSQLRecordSetGroup::GetDefaultSQL()
{ 
    return SQLTableNames::AccountGroups; 
}

//**********************************************************************

void CSQLRecordSetGroup::LoadFromGroupRow(CSQLRowGroup& RowGroup)
{
    m_MD.m_nGroupNo = RowGroup.GetGroupNo();
    m_MD.m_strGroupName = RowGroup.GetGroupName();
    m_MD.m_nSchemeNo = RowGroup.GetSchemeNo();
    m_MD.m_bStampOfferFlag = RowGroup.GetStampOfferFlag();
    /*****/
    m_MD.m_nRefreshType = RowGroup.GetRefreshType();
    m_MD.m_nRefreshFlags = RowGroup.GetRefreshFlags();
	SetDoubleString(m_MD.m_strRefreshValue, RowGroup.GetRefreshValue());
    m_MD.m_nRefreshAppend = RowGroup.GetRefreshAppend();
    /*****/
    m_MD.m_nPurse1Allowed = RowGroup.GetPurse1Allowed();
    m_MD.m_nPurse2Allowed = RowGroup.GetPurse2Allowed();
    /*****/
    m_MD.m_bNoPluBonus = RowGroup.GetNoPluBonusFlag();
    m_MD.m_bNoDeptBonus = RowGroup.GetNoDeptBonusFlag();
    /*****/
    m_MD.m_nMaxSpendType = RowGroup.GetMaxSpendType();
    /*****/
	SetDoubleString(m_MD.m_strMaxSpend[0], RowGroup.GetMaxSpendPeriod1());
	SetDoubleString(m_MD.m_strMaxSpend[1], RowGroup.GetMaxSpendPeriod2());
	SetDoubleString(m_MD.m_strMaxSpend[2], RowGroup.GetMaxSpendPeriod3());
	SetDoubleString(m_MD.m_strMaxSpend[3], RowGroup.GetMaxSpendPeriod4());
	SetDoubleString(m_MD.m_strMaxSpend[4], RowGroup.GetMaxSpendPeriod5());
	SetDoubleString(m_MD.m_strMaxSpend[5], RowGroup.GetMaxSpendPeriod6());
	/*****/
	SetDoubleString(m_MD.m_strMaxOverdraft[0], RowGroup.GetMaxOverdraftPeriod1());
	SetDoubleString(m_MD.m_strMaxOverdraft[1], RowGroup.GetMaxOverdraftPeriod2());
	SetDoubleString(m_MD.m_strMaxOverdraft[2], RowGroup.GetMaxOverdraftPeriod3());
	SetDoubleString(m_MD.m_strMaxOverdraft[3], RowGroup.GetMaxOverdraftPeriod4());
	SetDoubleString(m_MD.m_strMaxOverdraft[4], RowGroup.GetMaxOverdraftPeriod5());
	SetDoubleString(m_MD.m_strMaxOverdraft[5], RowGroup.GetMaxOverdraftPeriod6());
    /*****/
    m_MD.m_bRewardFlag = RowGroup.GetRewardFlag();
    m_MD.m_bRewardDisable = RowGroup.GetRewardDisable();
    m_MD.m_nRewardType = RowGroup.GetRewardType();
	SetDoubleString(m_MD.m_strRewardTrip, RowGroup.GetRewardTrip());
    m_MD.m_nRewardPoints = RowGroup.GetRewardPoints();
	SetDoubleString(m_MD.m_strRewardBonus, RowGroup.GetRewardBonus());
	SetDoubleString(m_MD.m_strRewardLimit, RowGroup.GetRewardLimit());
	SetDoubleString(m_MD.m_strRewardTrigger, RowGroup.GetRewardTrigger());
    /*****/
    m_MD.m_bRedeemFlag = RowGroup.GetRedeemFlag();
    m_MD.m_bRedeemDisable = RowGroup.GetRedeemDisable();
    m_MD.m_nRedeemType = RowGroup.GetRedeemType();
    m_MD.m_nRedeemTrip = RowGroup.GetRedeemTrip();
	SetDoubleString(m_MD.m_strRedeemValue, RowGroup.GetRedeemValue());
}

//**********************************************************************

void CSQLRecordSetGroup::SaveToGroupRow(CSQLRowGroup& RowGroup)
{
    RowGroup.SetGroupNo(m_MD.m_nGroupNo);
    RowGroup.SetGroupName(m_MD.m_strGroupName);
    RowGroup.SetSchemeNo(m_MD.m_nSchemeNo);
    RowGroup.SetStampOfferFlag(m_MD.m_bStampOfferFlag);
    /*****/
    RowGroup.SetRefreshType(m_MD.m_nRefreshType);
    RowGroup.SetRefreshFlags(m_MD.m_nRefreshFlags);
    RowGroup.SetRefreshValue(atof(m_MD.m_strRefreshValue));
    RowGroup.SetRefreshAppend(m_MD.m_nRefreshAppend);
    /*****/
    RowGroup.SetPurse1Allowed(m_MD.m_nPurse1Allowed);
    RowGroup.SetPurse2Allowed(m_MD.m_nPurse2Allowed);
    /*****/
    RowGroup.SetNoPluBonusFlag(m_MD.m_bNoPluBonus);
    RowGroup.SetNoDeptBonusFlag(m_MD.m_bNoDeptBonus);
    /*****/
    RowGroup.SetMaxSpendType(m_MD.m_nMaxSpendType);
    /*****/
    RowGroup.SetMaxSpendPeriod1(atof(m_MD.m_strMaxSpend[0]));
	RowGroup.SetMaxSpendPeriod2(atof(m_MD.m_strMaxSpend[1]));
	RowGroup.SetMaxSpendPeriod3(atof(m_MD.m_strMaxSpend[2]));
	RowGroup.SetMaxSpendPeriod4(atof(m_MD.m_strMaxSpend[3]));
	RowGroup.SetMaxSpendPeriod5(atof(m_MD.m_strMaxSpend[4]));
	RowGroup.SetMaxSpendPeriod6(atof(m_MD.m_strMaxSpend[5]));
	/*****/
	RowGroup.SetMaxOverdraftPeriod1(atof(m_MD.m_strMaxOverdraft[0]));
	RowGroup.SetMaxOverdraftPeriod2(atof(m_MD.m_strMaxOverdraft[1]));
	RowGroup.SetMaxOverdraftPeriod3(atof(m_MD.m_strMaxOverdraft[2]));
	RowGroup.SetMaxOverdraftPeriod4(atof(m_MD.m_strMaxOverdraft[3]));
	RowGroup.SetMaxOverdraftPeriod5(atof(m_MD.m_strMaxOverdraft[4]));
	RowGroup.SetMaxOverdraftPeriod6(atof(m_MD.m_strMaxOverdraft[5]));
    /*****/
    RowGroup.SetRewardFlag(m_MD.m_bRewardFlag);
    RowGroup.SetRewardDisable(m_MD.m_bRewardDisable);
    RowGroup.SetRewardType(m_MD.m_nRewardType);
	RowGroup.SetRewardTrip(atof(m_MD.m_strRewardTrip));
    RowGroup.SetRewardPoints(m_MD.m_nRewardPoints);
	RowGroup.SetRewardBonus(atof(m_MD.m_strRewardBonus));
	RowGroup.SetRewardLimit(atof(m_MD.m_strRewardLimit));
	RowGroup.SetRewardTrigger(atof(m_MD.m_strRewardTrigger));
    /*****/
    RowGroup.SetRedeemFlag(m_MD.m_bRedeemFlag);
    RowGroup.SetRedeemDisable(m_MD.m_bRedeemDisable);
    RowGroup.SetRedeemType(m_MD.m_nRedeemType);
    RowGroup.SetRedeemTrip(m_MD.m_nRedeemTrip);
    RowGroup.SetRedeemValue(atof(m_MD.m_strRedeemValue));
}

//**********************************************************************

bool CSQLRecordSetGroup::StepSelectAll(CSQLRowGroup& RowGroup)
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
