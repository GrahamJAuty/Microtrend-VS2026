//**********************************************************************
#include "..\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "SQLRecordSetBromComTransaction.h"
//**********************************************************************

CSQLRecordSetMetadataBromComTransaction::CSQLRecordSetMetadataBromComTransaction(bool bForExistenceCheck) : CSQLRecordSetMetadata()
{
    if (FALSE == bForExistenceCheck)
    {
        AddInt64Field(&m_nLineID, BromComTransaction::LineID.Label);
        AddIntField(&m_nPersonID, BromComTransaction::PersonID.Label);
        AddTextField(&m_strDetails, BromComTransaction::Details.Label);
        AddDecimalField(&m_strAmount, BromComTransaction::Amount.Label);
        AddDecimalField(&m_strBalance, BromComTransaction::Balance.Label);
        AddTextField(&m_strDateTime, BromComTransaction::DateTime.Label);
        AddTextField(&m_strType, BromComTransaction::Type.Label);
        AddIntField(&m_nTransactionID, BromComTransaction::TransactionID.Label);
        AddTextField(&m_strWebComment, BromComTransaction::WebComment.Label);
    }
}

//**********************************************************************

void CSQLRecordSetMetadataBromComTransaction::AddParamsByLineID(const CString strLineID)
{
    AddTextParam(BromComTransaction::LineID.Label, strLineID);
}

//**********************************************************************

CSQLRecordSetBromComTransaction::CSQLRecordSetBromComTransaction(CDatabase* pDatabase, RSParams_BromComTransaction_NormalNoParams params) :
    CSQLRecordSetConnectionPool(pDatabase),
	m_MD()
{
	CommonInit();
}

//**********************************************************************

CSQLRecordSetBromComTransaction::CSQLRecordSetBromComTransaction(CDatabase* pDatabase, RSParams_BromComTransaction_NormalByLineId params, bool bForExistenceCheck) :
    CSQLRecordSetConnectionPool(pDatabase),
    m_MD(bForExistenceCheck)
{
    m_MD.AddParamsByLineID(params.strLineID);
    CommonInit();
}

//**********************************************************************

void CSQLRecordSetBromComTransaction::CommonInit()
{
    m_nFields = m_MD.GetFieldCount();
    m_nParams = m_MD.GetParamCount();
    SetMetadata(&m_MD);
    SetInitialFilterString();
}

//**********************************************************************

CString CSQLRecordSetBromComTransaction::GetDefaultSQL()
{
    return SQLTableNames::BromComTransactions;
}

//**********************************************************************

void CSQLRecordSetBromComTransaction::LoadFromBromComTransactionRow(CSQLRowBromComTransaction& RowBC)
{
    m_MD.m_nLineID = RowBC.GetLineID();
    m_MD.m_nPersonID = RowBC.GetPersonID();
    m_MD.m_strDetails = RowBC.GetDetails();
	SetDoubleString(m_MD.m_strAmount, RowBC.GetAmount());
	SetDoubleString(m_MD.m_strBalance, RowBC.GetBalance());
    m_MD.m_strDateTime = RowBC.GetDateTime();
    m_MD.m_strType = RowBC.GetType();
    m_MD.m_nTransactionID = RowBC.GetTransactionID();
    m_MD.m_strWebComment = RowBC.GetWebComment();
}

//**********************************************************************

void CSQLRecordSetBromComTransaction::SaveToBromComTransactionRow(CSQLRowBromComTransaction& RowBC)
{
    RowBC.SetLineID(m_MD.m_nLineID);
    RowBC.SetPersonID(m_MD.m_nPersonID);
    RowBC.SetDetails(m_MD.m_strDetails);
	RowBC.SetAmount(atof(m_MD.m_strAmount));
	RowBC.SetBalance(atof(m_MD.m_strBalance));
    RowBC.SetDateTime(m_MD.m_strDateTime);
    RowBC.SetType(m_MD.m_strType);
    RowBC.SetTransactionID(m_MD.m_nTransactionID);
    RowBC.SetWebComment(m_MD.m_strWebComment);
}

//**********************************************************************

bool CSQLRecordSetBromComTransaction::StepSelectAll(CSQLRowBromComTransaction& RowBC)
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

    SaveToBromComTransactionRow(RowBC);
    MoveNext();
    
    return TRUE;
}

//**********************************************************************
