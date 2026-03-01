#pragma once
/**********************************************************************/
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
	CDatabase* GetDatabase() const { return m_pDatabase; }

	// NEW: Thread-local transaction context
	static CSQLTranBase* GetCurrentTransaction();
	static bool IsInTransaction();

protected:
	int m_nState;
	int m_nConnection;
	bool m_bCanCommit;
	CDatabase* m_pDatabase;

protected:
	CStringArray m_arrayAuditBuffer;
	CString m_strProcessError;

private:
	static thread_local CSQLTranBase* s_pCurrentTransaction;
};

/**********************************************************************/
