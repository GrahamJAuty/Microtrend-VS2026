//**********************************************************************
#include "..\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "SQLRecordSetBromComBalanceChanges.h"
//**********************************************************************

CSQLRecordSetMetadataBromComBalanceChanges::CSQLRecordSetMetadataBromComBalanceChanges(bool bForExistenceCheck)
{
    if (FALSE == bForExistenceCheck)
    {
        AddIntField(&m_nPersonID, BromComBalanceChanges::PersonID.Label);
        AddInt64Field(&m_nAccountID, BromComBalanceChanges::AccountID.Label);
    }
}

//**********************************************************************

void CSQLRecordSetMetadataBromComBalanceChanges::AddParamsByPersonID(const int nPersonID)
{
    AddIntParam(BromComBalanceChanges::PersonID.Label, nPersonID);
}
    
//**********************************************************************

void CSQLRecordSetMetadataBromComBalanceChanges::AddParamsByRange(const int nPersonIDFrom, const int nPersonIDTo)
{
    AddIntParam(BromComBalanceChanges::PersonID.Label, nPersonIDFrom);
    AddIntParam(BromComBalanceChanges::PersonID.Label, nPersonIDTo);
}

//**********************************************************************

CSQLRecordSetBromComBalanceChanges::CSQLRecordSetBromComBalanceChanges(CDatabase* pDatabase, RSParams_BromComBalanceChanges_NormalNoParams params) :
    CSQLRecordSetConnectionPool(pDatabase),
	m_MD()
{
	CommonInit();
}

//**********************************************************************

CSQLRecordSetBromComBalanceChanges::CSQLRecordSetBromComBalanceChanges(CDatabase* pDatabase, RSParams_BromComBalanceChanges_NormalByPersonId params, bool bForExistenceCheck ) :
    CSQLRecordSetConnectionPool(pDatabase),
    m_MD(bForExistenceCheck)
{
    m_MD.AddParamsByPersonID(params.nPersonID);
    CommonInit();
}

//**********************************************************************

CSQLRecordSetBromComBalanceChanges::CSQLRecordSetBromComBalanceChanges(CDatabase* pDatabase, RSParams_BromComBalanceChanges_CountRowsByRange params ) :
    CSQLRecordSetConnectionPool(pDatabase),
    m_MD(TRUE)
{
	m_bRangeFilter = TRUE;
    m_MD.AddParamsByRange(params.nPersonIDFrom, params.nPersonIDTo);
    CommonInit();
}

//**********************************************************************

void CSQLRecordSetBromComBalanceChanges::CommonInit()
{
    m_nFields = m_MD.GetFieldCount();
    m_nParams = m_MD.GetParamCount();
    SetMetadata(&m_MD);
    SetInitialFilterString();
}

//**********************************************************************

CString CSQLRecordSetBromComBalanceChanges::GetDefaultSQL()
{
    return SQLTableNames::BromComBalanceChanges;
}

//**********************************************************************

void CSQLRecordSetBromComBalanceChanges::LoadFromBromComBalanceChangesRow(CSQLRowBromComBalanceChanges& RowBC)
{
    m_MD.m_nPersonID = RowBC.GetPersonID();
    m_MD.m_nAccountID = RowBC.GetAccountID();
}

//**********************************************************************

void CSQLRecordSetBromComBalanceChanges::SaveToBromComBalanceChangesRow(CSQLRowBromComBalanceChanges& RowBC)
{
    RowBC.SetPersonID(m_MD.m_nPersonID);
    RowBC.SetAccountID(m_MD.m_nAccountID);
}

//**********************************************************************

bool CSQLRecordSetBromComBalanceChanges::StepSelectAll(CSQLRowBromComBalanceChanges& RowBC)
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

    SaveToBromComBalanceChangesRow(RowBC);
    MoveNext();
    
    return TRUE;
}

//**********************************************************************
