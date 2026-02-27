//**********************************************************************
#include "..\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "SQLRecordSetBromComPendingOrder.h"
//**********************************************************************

CSQLRecordSetMetadataBromComPendingOrder::CSQLRecordSetMetadataBromComPendingOrder(bool bForExistenceCheck) : CSQLRecordSetMetadata()
{
    if (FALSE == bForExistenceCheck)
    {
        AddIntField(&m_nOrderID, BromComPendingOrder::OrderID.Label);
        AddTextField(&m_strStatus, BromComPendingOrder::Status.Label);
        AddTextField(&m_strLastModified, BromComPendingOrder::LastModified.Label);
        AddBOOLField(&m_bNewOrder, BromComPendingOrder::NewOrder.Label);
    }
}

//**********************************************************************

void CSQLRecordSetMetadataBromComPendingOrder::AddParamsByOrderID(const int nOrderID)
{
    AddIntParam(BromComPendingOrder::OrderID.Label, nOrderID);
}

//**********************************************************************

void CSQLRecordSetMetadataBromComPendingOrder::AddParamsForRowCount(const int nOrderIDFrom, const int nOrderIDTo)
{
    AddIntParam(BromComPendingOrder::OrderID.Label, nOrderIDFrom);
    AddIntParam(BromComPendingOrder::OrderID.Label, nOrderIDTo);
}

//**********************************************************************

CSQLRecordSetBromComPendingOrder::CSQLRecordSetBromComPendingOrder(CDatabase* pDatabase, RSParams_BromComPendingOrder_NormalNoParams params) :
    CSQLRecordSetConnectionPool( pDatabase),
	m_MD()
{
	CommonInit();
}

//**********************************************************************

CSQLRecordSetBromComPendingOrder::CSQLRecordSetBromComPendingOrder(CDatabase* pDatabase, RSParams_BromComPendingOrder_NormalByOrderId params, bool bForExistenceCheck) :
    CSQLRecordSetConnectionPool(pDatabase),
    m_MD(bForExistenceCheck)
{
	m_MD.AddParamsByOrderID(params.nOrderID);
	CommonInit();
}

//**********************************************************************

CSQLRecordSetBromComPendingOrder::CSQLRecordSetBromComPendingOrder(CDatabase* pDatabase, RSParams_BromComPendingOrder_CountRowsByRange params) :
    CSQLRecordSetConnectionPool(pDatabase),
    m_MD(TRUE)
{
	m_bRangeFilter = TRUE;
    m_MD.AddParamsForRowCount(params.nOrderIDFrom,params.nOrderIDTo);
    CommonInit();
}

//**********************************************************************

void CSQLRecordSetBromComPendingOrder::CommonInit()
{
    m_nFields = m_MD.GetFieldCount();
    m_nParams = m_MD.GetParamCount();
    SetMetadata(&m_MD);
    SetInitialFilterString();
}

//**********************************************************************

CString CSQLRecordSetBromComPendingOrder::GetDefaultSQL()
{
    return SQLTableNames::BromComPendingOrders;
}

//**********************************************************************

void CSQLRecordSetBromComPendingOrder::LoadFromBromComPendingOrderRow(CSQLRowBromComPendingOrder& RowBC)
{
    m_MD.m_nOrderID = RowBC.GetOrderID();
    m_MD.m_strStatus = RowBC.GetStatus();
    m_MD.m_strLastModified = RowBC.GetLastModified();
    m_MD.m_bNewOrder = RowBC.GetNewOrderFlag();
}

//**********************************************************************

void CSQLRecordSetBromComPendingOrder::SaveToBromComPendingOrderRow(CSQLRowBromComPendingOrder& RowBC)
{
    RowBC.SetOrderID(m_MD.m_nOrderID);
    RowBC.SetStatus(m_MD.m_strStatus);
    RowBC.SetLastModified(m_MD.m_strLastModified);
    RowBC.SetNewOrderFlag(m_MD.m_bNewOrder);
}

//**********************************************************************

bool CSQLRecordSetBromComPendingOrder::StepSelectAll(CSQLRowBromComPendingOrder& RowBC)
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

    SaveToBromComPendingOrderRow(RowBC);
    MoveNext();
    
    return TRUE;
}

//**********************************************************************
