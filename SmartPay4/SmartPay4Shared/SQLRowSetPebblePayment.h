#pragma once
//**********************************************************************
#include "..\SmartPay4Shared\SQLTable_PebblePaymentType\SQLRowPebblePaymentType.h"
#include "..\SmartPay4Shared\SQLTable_PebblePaymentType\SQLFieldValidatorPebblePaymentType.h"
//**********************************************************************

class CSQLRowSetPebblePayment
{
public:
	CSQLRowSetPebblePayment();

public:
	bool LoadPebblePayments();
	
public:
	int GetSize() { return m_arrayRows.GetSize(); }
	void GetRow(int n, CSQLRowPebblePaymentType& Row);
	void RemoveRow(int n);
	void InsertRow(int nPos, CSQLRowPebblePaymentType& Row);
	void UpdateRow(int nPos, CSQLRowPebblePaymentType& Row);
	bool FindPayment(int nPaymentType, int& nPos);
	
private:
	CArray<CSQLRowPebblePaymentType, CSQLRowPebblePaymentType> m_arrayRows;
};

//**********************************************************************
