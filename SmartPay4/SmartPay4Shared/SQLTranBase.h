#pragma once
/**********************************************************************/
#include "SQLAuditRecord_base.h"
#include "SQLDb.h"
#include "SQLDefines.h"
/**********************************************************************/

class CSQLTranBase  
{
public:
	CSQLTranBase();
	~CSQLTranBase();

public:
	int BeginTrans();
	int EndTrans();

public:
	virtual void AfterTransaction();
	void ShowErrorMessage(CString strAction);
	int GetState() { return m_nState; }

public:
	static void AddAlterTableAddColumnCommand(CStringArray& arrayCommand, CString strTable, CString strField, CString strType, bool bNotNull, CString strDefault = "");
	static void AddAlterTableNotNullCommand(CStringArray& arrayCommand, CString strTable, CString strField, CString strType, CString strDefault = "");

protected:
	int m_nState;
	int m_nConnection;
	bool m_bCanCommit;
	CDatabase* m_pDatabase;

protected:
	CSQLAuditRecord_base m_atc;
};

/**********************************************************************/
