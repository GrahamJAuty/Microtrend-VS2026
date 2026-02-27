//**********************************************************************
#include "..\SmartPay4Shared\SQLTable_PurchaseControlRule\SQLRepositoryPurchaseControlRule.h"
#include "..\SmartPay4Shared\SQLTable_PurchaseControlRule\SQLRecordSetPurchaseControlRule.h"
//**********************************************************************
#include "..\SmartPay4Shared\SQLDb.h"
//**********************************************************************
#include "SQLRowSetPurchaseControlInfo.h"
//**********************************************************************

CSQLRowSetPurchaseControlInfo::CSQLRowSetPurchaseControlInfo()
{
}

//**********************************************************************

void CSQLRowSetPurchaseControlInfo::LoadDatabasePurchaseControlInfo()
{
	m_arrayRows.RemoveAll();
	
	CSQLRecordSetPurchaseControlRule RecordSet(NULL, RSParams_PurchaseControlRule_NormalNoParams());

	CSQLRowPurchaseControlRule RowSO;
	while (RecordSet.StepSelectAll(RowSO) == TRUE)
	{
		m_arrayRows.Add(RowSO);
	}
}

//**********************************************************************

void CSQLRowSetPurchaseControlInfo::GetRow(int nRow, CSQLRowPurchaseControlRule& Row)
{
	if ((nRow >= 0) && (nRow < GetSize()))
	{
		Row = m_arrayRows.GetAt(nRow);
	}
}

//**********************************************************************

void CSQLRowSetPurchaseControlInfo::SetRow(int nRow, CSQLRowPurchaseControlRule& Row)
{
	if ((nRow >= 0) && (nRow < GetSize()))
	{
		m_arrayRows.SetAt(nRow, Row);
	}
}

//**********************************************************************

