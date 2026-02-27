#pragma once
//**********************************************************************
#include "..\SmartPay4Shared\SQLAuditRecord.h"
#include "..\SmartPay4Shared\SmartPayAuditFile.h"
//**********************************************************************
#include "..\SmartPay4ManagerBgnd\CSVImportData.h"
//**********************************************************************

class CDatabaseAuditImport
{
public:
	CDatabaseAuditImport ( CWnd* pParent = NULL );
	void ProcessAudit();

private:
	void Import ( CSmartPayAuditFile* pAuditFile );
	bool ImportInternal(CDatabase* pDatabase, CSmartPayAuditFile* pAuditFile);
	void UpdateRecord (CDatabase* pDatabase, CSQLAuditRecord* pAtc );
	void ShowStatistics();

private:
	CWnd* m_pParent;
	CString m_strTitle;

	CCsvImportData m_data;

	int m_nImportMethod;
	int m_nRead;
	int m_nUsed;
	int m_nAdded;
	int m_nDeleted;
	int m_nIgnored;
};

//**********************************************************************
