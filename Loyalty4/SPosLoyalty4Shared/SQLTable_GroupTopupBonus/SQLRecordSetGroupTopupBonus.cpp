//**********************************************************************
#include "..\SQLNamespaces.h"
//**********************************************************************
#include "SQLRecordSetGroupTopupBonus.h"
//**********************************************************************

CSQLRecordSetMetadataGroupTopupBonus::CSQLRecordSetMetadataGroupTopupBonus(bool bForExistenceCheck) : CSQLRecordSetMetadata()
{
    if (FALSE == bForExistenceCheck)
    {
        AddIntField(&m_nGroupNo, GroupTopupBonus::GroupNo.Label, 1);
        AddIntField(&m_nPurseNo, GroupTopupBonus::PurseNo.Label, 1);
        AddIntField(&m_nTopupNo, GroupTopupBonus::TopupNo.Label, 1);
        /*****/
        AddBOOLField(&m_bBonusFlag, GroupTopupBonus::BonusFlag.Label, FALSE);
        AddBOOLField(&m_bBonusDisable, GroupTopupBonus::BonusDisable.Label, FALSE);
        AddIntField(&m_nBonusType, GroupTopupBonus::BonusType.Label, GroupTopupBonus::BonusType.Min);
        AddDecimalField(&m_strBonusTrip, GroupTopupBonus::BonusTrip.Label);
        AddIntField(&m_nBonusPoints, GroupTopupBonus::BonusPoints.Label);
        AddDecimalField(&m_strBonusValue, GroupTopupBonus::BonusValue.Label);
        AddIntField(&m_nBonusPercent, GroupTopupBonus::BonusPercent.Label);
    }
}

//**********************************************************************

void CSQLRecordSetMetadataGroupTopupBonus::AddParamsByGroupNo(const int nGroupNo)
{
    AddIntParam(GroupTopupBonus::GroupNo.Label, nGroupNo);
}

//**********************************************************************

void CSQLRecordSetMetadataGroupTopupBonus::AddParamsByGroupNoPurseNoTopupNo(const int nGroupNo, const int nPurseNo, const int nTopupNo)
{
	AddParamsByGroupNo(nGroupNo);
    AddIntParam(GroupTopupBonus::PurseNo.Label, nPurseNo);
    AddIntParam(GroupTopupBonus::TopupNo.Label, nTopupNo);
}

//**********************************************************************

CSQLRecordSetGroupTopupBonus::CSQLRecordSetGroupTopupBonus(CDatabase* pDatabase, RSParams_GroupTopupBonus_NormalNoParams params) :
    CSQLRecordSetConnectionPool(pDatabase),
    m_MD()
{
    CommonInit();
}

//**********************************************************************

CSQLRecordSetGroupTopupBonus::CSQLRecordSetGroupTopupBonus(CDatabase* pDatabase, RSParams_GroupTopupBonus_NormalByGroupNo params) :
    CSQLRecordSetConnectionPool(pDatabase),
    m_MD()
{
    m_MD.AddParamsByGroupNo(params.m_nGroupNo);
    CommonInit();
}

//**********************************************************************

CSQLRecordSetGroupTopupBonus::CSQLRecordSetGroupTopupBonus(CDatabase* pDatabase, RSParams_GroupTopupBonus_NormalByFullKey params, bool bForExistenceCheck) :
    CSQLRecordSetConnectionPool(pDatabase),
    m_MD(bForExistenceCheck)
{
    m_MD.AddParamsByGroupNoPurseNoTopupNo(params.m_nGroupNo, params.m_nPurseNo, params.m_nTopupNo);
    CommonInit();
}

//**********************************************************************

void CSQLRecordSetGroupTopupBonus::CommonInit()
{
    m_nFields = m_MD.GetFieldCount();
    m_nParams = m_MD.GetParamCount();
    SetMetadata(&m_MD);
    SetInitialFilterString();
}

//**********************************************************************

CString CSQLRecordSetGroupTopupBonus::GetDefaultSQL()
{ 
    return SQLTableNames::GroupTopupBonuses; 
}

//**********************************************************************

void CSQLRecordSetGroupTopupBonus::LoadFromGroupTopupBonusRow(CSQLRowGroupTopupBonus& RowGroup)
{
    m_MD.m_nGroupNo = RowGroup.GetGroupNo();
    m_MD.m_nPurseNo = RowGroup.GetPurseNo();
    m_MD.m_nTopupNo = RowGroup.GetTopupNo();
    /*****/
    m_MD.m_bBonusFlag = RowGroup.GetBonusFlag();
    m_MD.m_bBonusDisable = RowGroup.GetBonusDisable();
    m_MD.m_nBonusType = RowGroup.GetBonusType();
	SetDoubleString(m_MD.m_strBonusTrip, RowGroup.GetBonusTrip());
    m_MD.m_nBonusPoints = RowGroup.GetBonusPoints();
	SetDoubleString(m_MD.m_strBonusValue, RowGroup.GetBonusValue());
    m_MD.m_nBonusPercent = RowGroup.GetBonusPercent();
}

//**********************************************************************

void CSQLRecordSetGroupTopupBonus::SaveToGroupTopupBonusRow(CSQLRowGroupTopupBonus& RowGroup)
{
    RowGroup.SetGroupNo(m_MD.m_nGroupNo);
    RowGroup.SetPurseNo(m_MD.m_nPurseNo);
    RowGroup.SetTopupNo(m_MD.m_nTopupNo);
    /*****/
    RowGroup.SetBonusFlag(m_MD.m_bBonusFlag);
    RowGroup.SetBonusDisable(m_MD.m_bBonusDisable);
    RowGroup.SetBonusType(m_MD.m_nBonusType);
    RowGroup.SetBonusTrip(atof(m_MD.m_strBonusTrip));
    RowGroup.SetBonusPoints(m_MD.m_nBonusPoints);
    RowGroup.SetBonusValue(atof(m_MD.m_strBonusValue));
    RowGroup.SetBonusPercent(m_MD.m_nBonusPercent);
}

//**********************************************************************

bool CSQLRecordSetGroupTopupBonus::StepSelectAll(CSQLRowGroupTopupBonus& RowGroup)
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

    SaveToGroupTopupBonusRow(RowGroup);
    MoveNext();
    
    return TRUE;
}

//**********************************************************************
