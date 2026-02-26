//**********************************************************************
#include "..\SQLNamespaces.h"
//**********************************************************************
#include "SQLRecordSetStaffCard.h"
//**********************************************************************

CSQLRecordSetMetadataStaffCard::CSQLRecordSetMetadataStaffCard() : CSQLRecordSetMetadata()
{
    AddIntField(&m_nFolderIndexDbNo, StaffCards::FolderIndexDbNo.Label);
    AddIntField(&m_nFolderIndexSetNo, StaffCards::FolderIndexSetNo.Label);
    AddIntField(&m_nServerNo, StaffCards::ServerNo.Label, 1);
    AddInt64Field(&m_nUserID, StaffCards::UserID.Label);
}

//**********************************************************************

void CSQLRecordSetMetadataStaffCard::AddParamsByDbNoSetNoServerNo(const int nDbNo, const int nSetNo, const int nServerNo)
{
    AddIntParam(StaffCards::FolderIndexDbNo.Label, nDbNo);
    AddIntParam(StaffCards::FolderIndexSetNo.Label, nSetNo);
    AddIntParam(StaffCards::ServerNo.Label, nServerNo);
}

//**********************************************************************

CSQLRecordSetStaffCard::CSQLRecordSetStaffCard(CDatabase* pDatabase, RSParams_StaffCard_NormalNoParams params) :
    CSQLRecordSetConnectionPool(pDatabase),
	m_MD()
{
	CommonInit();
}

//**********************************************************************

CSQLRecordSetStaffCard::CSQLRecordSetStaffCard(CDatabase* pDatabase, RSParams_StaffCard_NormalByDbNoSetNoServerNo params) :
    CSQLRecordSetConnectionPool(pDatabase),
    m_MD()
{
    m_MD.AddParamsByDbNoSetNoServerNo(params.nDbNo, params.nSetNo, params.nServerNo);
	CommonInit();
}

//**********************************************************************

void CSQLRecordSetStaffCard::CommonInit()
{
    m_nFields = m_MD.GetFieldCount();
    m_nParams = m_MD.GetParamCount();
    SetMetadata(&m_MD);
    SetInitialFilterString();
}

//**********************************************************************

CString CSQLRecordSetStaffCard::GetDefaultSQL()
{
    return SQLTableNames::StaffCards;
}

//**********************************************************************

void CSQLRecordSetStaffCard::DoFieldExchange(CFieldExchange* pFX)
{
	if ((0 == m_MD.m_nFolderIndexDbNo) && (m_MD.m_nFolderIndexSetNo != 0))
	{
        m_MD.m_nFolderIndexDbNo = 1;
	}

    CSQLRecordSetConnectionPool::DoFieldExchange(pFX);
   
    if ((0 == m_MD.m_nFolderIndexDbNo) && (m_MD.m_nFolderIndexSetNo != 0))
    {
        m_MD.m_nFolderIndexDbNo = 1;
    }
}

//**********************************************************************

void CSQLRecordSetStaffCard::LoadFromStaffCardRow(CSQLRowStaffCard& RowSC)
{
    m_MD.m_nFolderIndexDbNo = RowSC.GetFolderIndexDbNo();
    m_MD.m_nFolderIndexSetNo = RowSC.GetFolderIndexSetNo();
    m_MD.m_nServerNo = RowSC.GetServerNo();
    m_MD.m_nUserID = RowSC.GetUserID();
}

//**********************************************************************

void CSQLRecordSetStaffCard::SaveToStaffCardRow(CSQLRowStaffCard& RowSC)
{
    RowSC.SetFolderIndexDbNo(m_MD.m_nFolderIndexDbNo);
    RowSC.SetFolderIndexSetNo(m_MD.m_nFolderIndexSetNo);
    RowSC.SetServerNo(m_MD.m_nServerNo);
    RowSC.SetUserID(m_MD.m_nUserID);
}

//**********************************************************************

bool CSQLRecordSetStaffCard::StepSelectAll(CSQLRowStaffCard& RowBC)
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

    SaveToStaffCardRow(RowBC);
    MoveNext();
    
    return TRUE;
}

//**********************************************************************
