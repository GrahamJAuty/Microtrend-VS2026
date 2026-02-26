#pragma once
//**********************************************************************
#include "..\SPosLoyalty4Shared\ReportConsolidationArray.h"
//**********************************************************************
#include "..\SPOSLoyalty4Shared\SQLTable_GroupTopupBonus\SQLRowGroupTopupBonus.h"
#include "..\SPOSLoyalty4Shared\SQLTable_GroupTopupBonus\SQLFieldValidatorGroupTopupBonus.h"
//**********************************************************************

class CSQLRowSetGroupTopupBonus
{
public:
	CSQLRowSetGroupTopupBonus();

public:
	void LoadDatabaseGroups();
	
public:
	int GetSize() { return m_arrayRows.GetSize(); }
	void GetRow(int n, CSQLRowGroupTopupBonus& Row);
	void RemoveRow(int n);
	void InsertRow(int nPos, CSQLRowGroupTopupBonus& Row);
	void UpdateRow(int nPos, CSQLRowGroupTopupBonus& Row);
	void RemoveGroupTopups(int nGroupNo);
	void GetGroupTopups(int nGroupNo, CReportConsolidationArray<CSQLRowGroupTopupBonus>& arrayGroupTopups);
	void SetGroupTopups(int nGroupNo, CReportConsolidationArray<CSQLRowGroupTopupBonus>& arrayGroupTopups);
	
private:
	CReportConsolidationArray<CSQLRowGroupTopupBonus> m_arrayRows;
};

//**********************************************************************
