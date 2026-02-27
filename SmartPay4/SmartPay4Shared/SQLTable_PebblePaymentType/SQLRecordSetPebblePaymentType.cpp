//**********************************************************************
#include "..\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "SQLRecordSetPebblePaymentType.h"
//**********************************************************************

CSQLRecordSetMetadataPebblePaymentType::CSQLRecordSetMetadataPebblePaymentType(bool bForExistenceCheck) : CSQLRecordSetMetadata()
{
    if (FALSE == bForExistenceCheck)
    {
        AddIntField(&m_nSPOSPaymentType, PebblePaymentType::SPOSPaymentType.Label, 1);
        AddTextField(&m_strPebblePaymentName, PebblePaymentType::PebblePaymentName.Label);
        AddIntField(&m_nPebbleTranType, PebblePaymentType::PebbleTranType.Label);
    }
}

//**********************************************************************

void CSQLRecordSetMetadataPebblePaymentType::AddParamsByPaymentType(const int nPaymentType)
{
    AddIntParam(PebblePaymentType::SPOSPaymentType.Label, nPaymentType);
}

//**********************************************************************

CSQLRecordSetPebblePaymentType::CSQLRecordSetPebblePaymentType(CDatabase* pDatabase, RSParams_PebblePaymentType_NormalNoParams params) :
    CSQLRecordSetConnectionPool(pDatabase),
	m_MD() 
{
	CommonInit();
}

//**********************************************************************

CSQLRecordSetPebblePaymentType::CSQLRecordSetPebblePaymentType(CDatabase* pDatabase, RSParams_PebblePaymentType_NormalByPaymentType params, bool bForExistenceCheck) :
    CSQLRecordSetConnectionPool(pDatabase),
    m_MD(bForExistenceCheck)
{
	m_MD.AddParamsByPaymentType(params.nPaymentType);
	CommonInit();
}

//**********************************************************************

void CSQLRecordSetPebblePaymentType::CommonInit()
{
    m_nFields = m_MD.GetFieldCount();
    m_nParams = m_MD.GetParamCount();
    SetMetadata(&m_MD);
    SetInitialFilterString();
}

//**********************************************************************

CString CSQLRecordSetPebblePaymentType::GetDefaultSQL()
{
    return SQLTableNames::PebblePaymentTypes;
}

//**********************************************************************

void CSQLRecordSetPebblePaymentType::LoadFromPebblePaymentTypeRow(CSQLRowPebblePaymentType& RowPB)
{
    m_MD.m_nSPOSPaymentType = RowPB.GetSPOSPaymentType();
    m_MD.m_strPebblePaymentName = RowPB.GetPebblePaymentName();
    m_MD.m_nPebbleTranType = RowPB.GetPebbleTranType();
}

//**********************************************************************

void CSQLRecordSetPebblePaymentType::SaveToPebblePaymentTypeRow(CSQLRowPebblePaymentType& RowPB)
{
    RowPB.SetSPOSPaymentType(m_MD.m_nSPOSPaymentType);
    RowPB.SetPebblePaymentName(m_MD.m_strPebblePaymentName);
    RowPB.SetPebbleTranType(m_MD.m_nPebbleTranType);
}

//**********************************************************************

bool CSQLRecordSetPebblePaymentType::StepSelectAll(CSQLRowPebblePaymentType& RowPB)
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

    SaveToPebblePaymentTypeRow(RowPB);
    MoveNext();
    
    return TRUE;
}

//**********************************************************************
