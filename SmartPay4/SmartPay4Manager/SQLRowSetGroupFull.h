#pragma once
//**********************************************************************
#include "..\SmartPay4Shared\SQLTable_Group\SQLRowGroupFull.h"
#include "..\SmartPay4Shared\SQLTable_Group\SQLFieldValidatorGroup.h"
//**********************************************************************

class CSQLRowSetGroupFull
{
public:
	CSQLRowSetGroupFull();

public:
	void LoadDatabaseGroups();
	
public:
	int GetSize() { return m_arrayRows.GetSize(); }
	void GetRow(int n, CSQLRowGroupFull& Row);
	void RemoveRow(int n);
	void InsertRow(int nPos, CSQLRowGroupFull& Row);
	void UpdateRow(int nPos, CSQLRowGroupFull& Row);
	bool FindGroup(int nGroupNo, int& nPos);
	
private:
	CArray<CSQLRowGroupFull, CSQLRowGroupFull> m_arrayRows;
};

//**********************************************************************
