#pragma once
//**********************************************************************
#include "..\SPOSLoyalty4Shared\SQLTable_StaffGiftException\SQLRowStaffGiftException.h"
#include "..\SPOSLoyalty4Shared\SQLTable_StaffGiftException\SQLFieldValidatorStaffGiftException.h"
//**********************************************************************

class CSQLRowSetStaffGiftException
{
public:
	CSQLRowSetStaffGiftException();

public:
	void LoadExceptions();

public:
	int GetSize() { return m_arrayRows.GetSize(); }
	void GetRow(int n, CSQLRowStaffGiftException& Row);
	void RemoveRow(int n);
	void InsertRow(int nPos, CSQLRowStaffGiftException& Row);
	void UpdateRow(int nPos, CSQLRowStaffGiftException& Row);

private:
	CArray<CSQLRowStaffGiftException, CSQLRowStaffGiftException> m_arrayRows;
};

//**********************************************************************
