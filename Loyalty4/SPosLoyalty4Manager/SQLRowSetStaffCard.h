#pragma once
//**********************************************************************
#include "..\SPOSLoyalty4Shared\SQLTable_StaffCard\SQLRowStaffCard.h"
#include "..\SPOSLoyalty4Shared\SQLTable_StaffCard\SQLFieldValidatorStaffCard.h"
//**********************************************************************

class CSQLRowSetStaffCard
{
public:
	CSQLRowSetStaffCard();

public:
	void LoadDatabaseCards();

public:
	int GetSize() { return m_arrayRows.GetSize(); }
	void GetRow(int n, CSQLRowStaffCard& Row);
	void RemoveRow(int n);
	void InsertRow(int nPos, CSQLRowStaffCard& Row);
	void UpdateRow(int nPos, CSQLRowStaffCard& Row);
	bool FindCard(int nServerNo, int nFolderIndexDbNo, int nFolderIndexSetNo, int& nPos);
	int AddOrInsertRow(CSQLRowStaffCard& RowNew);

private:
	CArray<CSQLRowStaffCard, CSQLRowStaffCard> m_arrayRows;
};

//**********************************************************************
