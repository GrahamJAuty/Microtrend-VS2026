//**********************************************************************
#include "..\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "SQLRecordSetPebbleExportTransactionSent.h"
//**********************************************************************

CSQLRecordSetMetadataPebbleExportTransactionSent::CSQLRecordSetMetadataPebbleExportTransactionSent(bool bForExistenceCheck) : CSQLRecordSetMetadata()
{
    if (FALSE == bForExistenceCheck)
    {
        AddInt64Field(&m_nLineID, PebbleExportTransactionSent::LineID.Label);
        AddInt64Field(&m_nTranLineID, PebbleExportTransactionSent::TranLineID.Label);
    }
}

//**********************************************************************

void CSQLRecordSetMetadataPebbleExportTransactionSent::AddParamsByLineID(const CString strLineID)
{
    AddTextParam(PebbleExportTransactionSent::LineID.Label, strLineID);
}

//**********************************************************************

CSQLRecordSetPebbleExportTransactionSent::CSQLRecordSetPebbleExportTransactionSent(CDatabase* pDatabase, RSParams_PebbleTransactionSent_NormalNoParams params ) :
    CSQLRecordSetConnectionPool(pDatabase),
    m_MD()
{
	CommonInit();
}

//**********************************************************************

CSQLRecordSetPebbleExportTransactionSent::CSQLRecordSetPebbleExportTransactionSent(CDatabase* pDatabase, RSParams_PebbleTransactionSent_NormalByLineId params, bool bForExistenceCheck) :
    CSQLRecordSetConnectionPool(pDatabase),
    m_MD(bForExistenceCheck)
{
    m_MD.AddParamsByLineID(params.strLineId);
    CommonInit();
}

//**********************************************************************

void CSQLRecordSetPebbleExportTransactionSent::CommonInit()
{
    m_nFields = m_MD.GetFieldCount();
    m_nParams = m_MD.GetParamCount();
    SetMetadata(&m_MD);
    SetInitialFilterString();
}

//**********************************************************************

CString CSQLRecordSetPebbleExportTransactionSent::GetDefaultSQL()
{
    return SQLTableNames::PebbleExportTransactionsSent;
}

//**********************************************************************

void CSQLRecordSetPebbleExportTransactionSent::LoadFromPebbleExportTransactionSentRow(CSQLRowPebbleExportTransactionSent& RowPB)
{
    m_MD.m_nLineID = RowPB.GetLineID();
    m_MD.m_nTranLineID = RowPB.GetTranLineID();
}

//**********************************************************************

void CSQLRecordSetPebbleExportTransactionSent::SaveToPebbleExportTransactionSentRow(CSQLRowPebbleExportTransactionSent& RowPB)
{
    RowPB.SetLineID(m_MD.m_nLineID);
	RowPB.SetTranLineID(m_MD.m_nTranLineID);
}

//**********************************************************************

bool CSQLRecordSetPebbleExportTransactionSent::StepSelectAll(CSQLRowPebbleExportTransactionSent& RowBC)
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

    SaveToPebbleExportTransactionSentRow(RowBC);
    MoveNext();
    
    return TRUE;
}

//**********************************************************************
