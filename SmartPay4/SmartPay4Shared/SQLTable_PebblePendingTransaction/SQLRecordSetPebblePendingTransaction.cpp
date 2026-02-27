//**********************************************************************
#include "..\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "SQLRecordSetPebblePendingTransaction.h"
//**********************************************************************

CSQLRecordSetMetadataPebblePendingTransaction::CSQLRecordSetMetadataPebblePendingTransaction(bool bForExistenceCheck) : CSQLRecordSetMetadata()
{
    if (FALSE == bForExistenceCheck)
    {
        AddTextField(&m_strTransactionID, PebblePendingTransaction::TransactionID.Label);
        AddTextField(&m_strMemberID, PebblePendingTransaction::MemberID.Label);
        AddTextField(&m_strDateTime, PebblePendingTransaction::DateTime.Label);
        AddTextField(&m_strName, PebblePendingTransaction::Name.Label);
        AddIntField(&m_nAmount, PebblePendingTransaction::Amount.Label);
        AddBOOLField(&m_bIsSale, PebblePendingTransaction::IsSale.Label);
    }
}

//**********************************************************************

void CSQLRecordSetMetadataPebblePendingTransaction::AddParamsByTransactionID(const CString strTransactionID)
{
    AddTextParam(PebblePendingTransaction::TransactionID.Label, strTransactionID);
}

//**********************************************************************

CSQLRecordSetPebblePendingTransaction::CSQLRecordSetPebblePendingTransaction(CDatabase* pDatabase, RSParams_PebblePendingTransaction_NormalNoParams params, bool bUnmatched) :
    CSQLRecordSetConnectionPool(pDatabase),
    m_MD()
{
    m_bUnmatched = bUnmatched;
    CommonInit();
}

//**********************************************************************

CSQLRecordSetPebblePendingTransaction::CSQLRecordSetPebblePendingTransaction(CDatabase* pDatabase, RSParams_PebblePendingTransaction_NormalByTransactionId params, bool bUnmatched, bool bForExistenceCheck) :
    CSQLRecordSetConnectionPool(pDatabase),
    m_MD(bForExistenceCheck)
{
    m_bUnmatched = bUnmatched;
    m_MD.AddParamsByTransactionID(params.strTransactionId);
	CommonInit();
}

//**********************************************************************

void CSQLRecordSetPebblePendingTransaction::CommonInit()
{
    m_nFields = m_MD.GetFieldCount();
    m_nParams = m_MD.GetParamCount();
    SetMetadata(&m_MD);
    SetInitialFilterString();
}

//**********************************************************************

CString CSQLRecordSetPebblePendingTransaction::GetDefaultSQL()
{
    if (FALSE == m_bUnmatched)
    {
        return SQLTableNames::PebblePendingTransactions;
    }
    else
    {
        return SQLTableNames::PebbleUnmatchedTransactions;
    }
}

//**********************************************************************

void CSQLRecordSetPebblePendingTransaction::LoadFromPebblePendingTransactionRow(CSQLRowPebblePendingTransaction& RowPB)
{
    m_MD.m_strTransactionID = RowPB.GetTransactionID();
    m_MD.m_strMemberID = RowPB.GetMemberID();
    m_MD.m_strDateTime = RowPB.GetDateTime();
    m_MD.m_strName = RowPB.GetName();
    m_MD.m_nAmount = RowPB.GetAmount();
    m_MD.m_bIsSale = RowPB.GetIsSaleFlag();
}

//**********************************************************************

void CSQLRecordSetPebblePendingTransaction::SaveToPebblePendingTransactionRow(CSQLRowPebblePendingTransaction& RowPB)
{
    RowPB.SetTransactionID(m_MD.m_strTransactionID);
    RowPB.SetMemberID(m_MD.m_strMemberID);
    RowPB.SetDateTime(m_MD.m_strDateTime);
    RowPB.SetName(m_MD.m_strName);
    RowPB.SetAmount(m_MD.m_nAmount);
    RowPB.SetIsSaleFlag(m_MD.m_bIsSale);
}

//**********************************************************************

bool CSQLRecordSetPebblePendingTransaction::StepSelectAll(CSQLRowPebblePendingTransaction& RowPB)
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

    SaveToPebblePendingTransactionRow(RowPB);
    MoveNext();
    
    return TRUE;
}

//**********************************************************************
