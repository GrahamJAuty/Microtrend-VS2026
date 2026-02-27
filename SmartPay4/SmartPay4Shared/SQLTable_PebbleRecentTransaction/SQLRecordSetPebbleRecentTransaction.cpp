//**********************************************************************
#include "..\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "SQLRecordSetPebbleRecentTransaction.h"
//**********************************************************************

CSQLRecordSetMetadataPebbleRecentTransaction::CSQLRecordSetMetadataPebbleRecentTransaction(bool bForExistenceCheck) : CSQLRecordSetMetadata()
{
    if (FALSE == bForExistenceCheck)
    {
        AddTextField(&m_strTransactionID, PebbleRecentTransaction::TransactionID.Label);
        AddTextField(&m_strDateTime, PebbleRecentTransaction::DateTime.Label);
    }
}

//**********************************************************************

void CSQLRecordSetMetadataPebbleRecentTransaction::AddParamsByTransactionID(const CString strTransactionID)
{
    AddTextParam(PebbleRecentTransaction::TransactionID.Label, strTransactionID);
}

//**********************************************************************

CSQLRecordSetPebbleRecentTransaction::CSQLRecordSetPebbleRecentTransaction(CDatabase* pDatabase, RSParams_PebbleRecentTransaction_NormalNoParams params) :
    CSQLRecordSetConnectionPool( pDatabase),
	m_MD()
{
    CommonInit();
}

//**********************************************************************

CSQLRecordSetPebbleRecentTransaction::CSQLRecordSetPebbleRecentTransaction(CDatabase* pDatabase, RSParams_PebbleRecentTransaction_NormalByTransactionID params, bool bForExistenceCheck) :
    CSQLRecordSetConnectionPool(pDatabase),
    m_MD(bForExistenceCheck)
{
    m_MD.AddParamsByTransactionID(params.strTransactionID);
	CommonInit();
}

//**********************************************************************

void CSQLRecordSetPebbleRecentTransaction::CommonInit()
{
    m_nFields = m_MD.GetFieldCount();
    m_nParams = m_MD.GetParamCount();
    SetMetadata(&m_MD);
    SetInitialFilterString();
}

//**********************************************************************

CString CSQLRecordSetPebbleRecentTransaction::GetDefaultSQL()
{
    return SQLTableNames::PebbleRecentTransactions;
}

//**********************************************************************

void CSQLRecordSetPebbleRecentTransaction::LoadFromPebbleRecentTransactionRow(CSQLRowPebbleRecentTransaction& RowPB)
{
    m_MD.m_strTransactionID = RowPB.GetTransactionID();
    m_MD.m_strDateTime = RowPB.GetDateTime();
}

//**********************************************************************

void CSQLRecordSetPebbleRecentTransaction::SaveToPebbleRecentTransactionRow(CSQLRowPebbleRecentTransaction& RowPB)
{
    RowPB.SetTransactionID(m_MD.m_strTransactionID);
    RowPB.SetDateTime(m_MD.m_strDateTime);
}

//**********************************************************************

bool CSQLRecordSetPebbleRecentTransaction::StepSelectAll(CSQLRowPebbleRecentTransaction& RowPB)
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

    SaveToPebbleRecentTransactionRow(RowPB);
    MoveNext();
    
    return TRUE;
}

//**********************************************************************
