//**********************************************************************
#include "..\SQLNamespaces.h"
//**********************************************************************
#include "SQLRecordSetGroupFull.h"
//**********************************************************************

CSQLRecordSetMetadataGroupFull::CSQLRecordSetMetadataGroupFull() : CSQLRecordSetMetadata()
{
	AddIntField(&m_nGroupNo, Group::GroupNo.Label, 1);
	AddTextField(&m_strGroupName, Group::GroupName.Label);
	AddIntField(&m_nPriceLevel, Group::PriceLevel.Label);
}

//**********************************************************************

void CSQLRecordSetMetadataGroupFull::AddParamsByGroupNo(const int nGroupNo)
{
	AddIntParam(Group::GroupNo.Label, nGroupNo);
}

//**********************************************************************

CSQLRecordSetGroupFull::CSQLRecordSetGroupFull(CDatabase* pDatabase, RSParams_Group_NormalByGroupNo params) :
	CSQLRecordSetConnectionPool(pDatabase),
	m_MD()
{
	m_MD.AddParamsByGroupNo(params.nGroupNo);
	CommonInit();
}

//**********************************************************************

void CSQLRecordSetGroupFull::CommonInit()
{
	m_nFields = m_MD.GetFieldCount();
	m_nParams = m_MD.GetParamCount();
	SetMetadata(&m_MD);
	SetBasicFilterString();
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
	m_MD.m_nPriceLevel = RowGroup.GetPriceLevel();
}

//**********************************************************************

void CSQLRecordSetGroupFull::SaveToGroupRow(CSQLRowGroupFull& RowGroup)
{
    RowGroup.SetGroupNo(m_MD.m_nGroupNo);
    RowGroup.SetGroupName(m_MD.m_strGroupName);
    RowGroup.SetPriceLevel(m_MD.m_nPriceLevel);
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
