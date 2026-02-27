//**********************************************************************
#include "..\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "SQLRecordSetBromComItem.h"
//**********************************************************************

CSQLRecordSetMetadataBromComItem::CSQLRecordSetMetadataBromComItem(bool bForExistenceCheck) : CSQLRecordSetMetadata()
{
    if (FALSE == bForExistenceCheck)
    {
        AddIntField(&m_nOrderID, BromComItem::OrderID.Label);
        AddIntField(&m_nPersonID, BromComItem::PersonID.Label);
        AddDecimalField(&m_strAmount, BromComItem::Amount.Label);
        AddTextField(&m_strStatus, BromComItem::Status.Label);
    }
}

//**********************************************************************

void CSQLRecordSetMetadataBromComItem::AddParamsByOrderID(const int nOrderID)
{
    AddIntParam(BromComItem::OrderID.Label, nOrderID);
}

//**********************************************************************

void CSQLRecordSetMetadataBromComItem::AddParamsByOrderIDPersonID(const int nOrderID, const int nPersonID)
{
	AddParamsByOrderID(nOrderID);
    AddIntParam(BromComItem::PersonID.Label, nPersonID);
}

//**********************************************************************

CSQLRecordSetBromComItem::CSQLRecordSetBromComItem(CDatabase* pDatabase, RSParams_BromComItem_NormalNoParams params) :
    CSQLRecordSetConnectionPool(pDatabase),
    m_MD()
{
    CommonInit();
}

//**********************************************************************

CSQLRecordSetBromComItem::CSQLRecordSetBromComItem(CDatabase* pDatabase, RSParams_BromComItem_NormalByOrderIdPersonId params, bool bForExistenceCheck) :
    CSQLRecordSetConnectionPool(pDatabase),
    m_MD(bForExistenceCheck)
{
    m_MD.AddParamsByOrderIDPersonID(params.nOrderID, params.nPersonID);
	CommonInit();
}

//**********************************************************************

CSQLRecordSetBromComItem::CSQLRecordSetBromComItem(CDatabase* pDatabase, RSParams_BromComItem_NormalByOrderId params) :
    CSQLRecordSetConnectionPool(pDatabase),
    m_MD()
{
    m_MD.AddParamsByOrderID(params.nOrderID); 
    CommonInit();
}

//**********************************************************************

void CSQLRecordSetBromComItem::CommonInit()
{
    m_nFields = m_MD.GetFieldCount();
	m_nParams = m_MD.GetParamCount();
	SetMetadata(&m_MD);
    SetInitialFilterString();
}
 
//**********************************************************************

CString CSQLRecordSetBromComItem::GetDefaultSQL()
{
    return SQLTableNames::BromComItems;
}

//**********************************************************************

void CSQLRecordSetBromComItem::LoadFromBromComItemRow(CSQLRowBromComItem& RowBC)
{
    m_MD.m_nOrderID = RowBC.GetOrderID();
    m_MD.m_nPersonID = RowBC.GetPersonID();
	SetDoubleString(m_MD.m_strAmount, RowBC.GetAmount());
    m_MD.m_strStatus = RowBC.GetStatus();
}

//**********************************************************************

void CSQLRecordSetBromComItem::SaveToBromComItemRow(CSQLRowBromComItem& RowBC)
{
    RowBC.SetOrderID(m_MD.m_nOrderID);
    RowBC.SetPersonID(m_MD.m_nPersonID);
    RowBC.SetAmount(atof(m_MD.m_strAmount));
    RowBC.SetStatus(m_MD.m_strStatus);
}

//**********************************************************************

bool CSQLRecordSetBromComItem::StepSelectAll(CSQLRowBromComItem& RowBC)
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

    SaveToBromComItemRow(RowBC);
    MoveNext();

    return TRUE;
}

//**********************************************************************
