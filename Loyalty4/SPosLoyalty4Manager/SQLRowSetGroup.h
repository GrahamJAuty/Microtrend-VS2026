#pragma once
//**********************************************************************
#include "..\SPosLoyalty4Shared\ReportConsolidationArray.h"
//**********************************************************************
#include "..\SPOSLoyalty4Shared\SQLTable_Group\SQLRowGroup.h"
#include "..\SPOSLoyalty4Shared\SQLTable_Group\SQLFieldValidatorGroup.h"
//**********************************************************************

class CSQLRowSetGroup
{
public:
	CSQLRowSetGroup();

public:
	void LoadDatabaseGroups();
	
public:
	int GetSize() { return m_arrayRows.GetSize(); }
	void GetRow(int n, CSQLRowGroup& Row);
	void RemoveRow(int n);
	void InsertRow(int nPos, CSQLRowGroup& Row);
	void UpdateRow(int nPos, CSQLRowGroup& Row);
	bool FindGroup(int nGroupNo, int& nPos);
	
private:
	CReportConsolidationArray<CSQLRowGroup> m_arrayRows;
};

//**********************************************************************
