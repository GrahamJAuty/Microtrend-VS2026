//**********************************************************************
#include "..\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "SQLRecordSetBromComPendingItem.h"
//**********************************************************************

CSQLRecordSetMetadataBromComPendingItem::CSQLRecordSetMetadataBromComPendingItem(bool bForExistenceCheck) : CSQLRecordSetMetadata()
{
    if (FALSE == bForExistenceCheck)
    {
        AddIntField(&m_nOrderID, BromComPendingItem::OrderID.Label);
        AddIntField(&m_nPersonID, BromComPendingItem::PersonID.Label);
        AddDecimalField(&m_strAmount, BromComPendingItem::Amount.Label);
    }
}

//**********************************************************************

void CSQLRecordSetMetadataBromComPendingItem::AddParamsByOrderID(const int nOrderID)
{
    AddIntParam(BromComPendingItem::OrderID.Label, nOrderID);
}

//**********************************************************************

void CSQLRecordSetMetadataBromComPendingItem::AddParamsByOrderIDPersonID(const int nOrderID, const int nPersonID)
{
	AddParamsByOrderID(nOrderID);
    AddIntParam(BromComPendingItem::PersonID.Label, nPersonID);
}

//**********************************************************************

CSQLRecordSetBromComPendingItem::CSQLRecordSetBromComPendingItem(CDatabase* pDatabase, RSParams_BromComPendingItem_NormalNoParams params) :
    CSQLRecordSetConnectionPool(pDatabase),
	m_MD() 
{
	CommonInit();
}

//**********************************************************************

CSQLRecordSetBromComPendingItem::CSQLRecordSetBromComPendingItem(CDatabase* pDatabase, RSParams_BromComPendingItem_NormalByOrderId params) :
    CSQLRecordSetConnectionPool(pDatabase),
    m_MD()
{
    m_MD.AddParamsByOrderID(params.nOrderID);
    CommonInit();
}

//**********************************************************************

CSQLRecordSetBromComPendingItem::CSQLRecordSetBromComPendingItem(CDatabase* pDatabase, RSParams_BromComPendingItem_NormalByOrderIdPersonId params, bool bForExistenceCheck) :
    CSQLRecordSetConnectionPool(pDatabase),
    m_MD(bForExistenceCheck)
{
    m_MD.AddParamsByOrderIDPersonID(params.nOrderID, params.nPersonID);
    CommonInit();
}

//**********************************************************************

void CSQLRecordSetBromComPendingItem::CommonInit()
{
    m_nFields = m_MD.GetFieldCount();
    m_nParams = m_MD.GetParamCount();
    SetMetadata(&m_MD);
    SetInitialFilterString();
}

//**********************************************************************

CString CSQLRecordSetBromComPendingItem::GetDefaultSQL()
{
    return SQLTableNames::BromComPendingItems;
}

//**********************************************************************

void CSQLRecordSetBromComPendingItem::LoadFromBromComPendingItemRow(CSQLRowBromComPendingItem& RowBC)
{
    m_MD.m_nOrderID = RowBC.GetOrderID();
    m_MD.m_nPersonID = RowBC.GetPersonID();
	SetDoubleString(m_MD.m_strAmount, RowBC.GetAmount());
}

//**********************************************************************

void CSQLRecordSetBromComPendingItem::SaveToBromComPendingItemRow(CSQLRowBromComPendingItem& RowBC)
{
    RowBC.SetOrderID(m_MD.m_nOrderID);
    RowBC.SetPersonID(m_MD.m_nPersonID);
    RowBC.SetAmount(atof(m_MD.m_strAmount));
}

//**********************************************************************

bool CSQLRecordSetBromComPendingItem::StepSelectAll(CSQLRowBromComPendingItem& RowBC)
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

    SaveToBromComPendingItemRow(RowBC);
    MoveNext();
    
    return TRUE;
}

//**********************************************************************
