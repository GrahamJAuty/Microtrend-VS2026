//**********************************************************************
#include "..\SmartPay4Shared\SQLTable_PebblePaymentType\SQLRepositoryPebblePaymentType.h"
#include "..\SmartPay4Shared\SQLTable_PebblePaymentType\SQLRecordSetPebblePaymentType.h"
//**********************************************************************
#include "..\SmartPay4Shared\SQLDb.h"
//**********************************************************************
#include "SQLRowSetPebblePayment.h"
//**********************************************************************

CSQLRowSetPebblePayment::CSQLRowSetPebblePayment()
{
}

//**********************************************************************

bool CSQLRowSetPebblePayment::LoadPebblePayments()
{
	m_arrayRows.RemoveAll();
	
	CSQLRecordSetPebblePaymentType RecordSet(NULL, RSParams_PebblePaymentType_NormalNoParams());

	CSQLRowPebblePaymentType RowPayment;
	while (RecordSet.StepSelectAll(RowPayment) == TRUE)
	{
		m_arrayRows.Add(RowPayment);
	}

	return (m_arrayRows.GetSize() > 0);
}

//**********************************************************************

bool CSQLRowSetPebblePayment::FindPayment(int nPaymentType, int& nPos)
{
	//FIDDLE SHOULD BE BINARY SEARCH
	
	CSQLRowPebblePaymentType RowPayment;
	for (nPos = 0; nPos < GetSize(); nPos++)
	{
		RowPayment = m_arrayRows.GetAt(nPos);
		
		if (RowPayment.GetSPOSPaymentType() >= nPaymentType)
		{
			return (RowPayment.GetSPOSPaymentType() == nPaymentType);
		}
	}

	return FALSE;
}

//**********************************************************************

void CSQLRowSetPebblePayment::GetRow(int nRow, CSQLRowPebblePaymentType& Row)
{
	if ((nRow >= 0) && (nRow < GetSize()))
	{
		Row = m_arrayRows.GetAt(nRow);
	}
}

//**********************************************************************

void CSQLRowSetPebblePayment::InsertRow(int n, CSQLRowPebblePaymentType& Row)
{
	if ((n>= 0) && (n <= GetSize()))
	{
		m_arrayRows.InsertAt( n, Row);
	}
}

//**********************************************************************

void CSQLRowSetPebblePayment::UpdateRow(int n, CSQLRowPebblePaymentType& Row)
{
	if ((n >= 0) && (n < GetSize()))
	{
		m_arrayRows.SetAt(n, Row);
	}
}

//**********************************************************************

void CSQLRowSetPebblePayment::RemoveRow(int n)
{
	if ((n >= 0) && (n < GetSize()))
	{
		m_arrayRows.RemoveAt(n);
	}
}

//**********************************************************************
