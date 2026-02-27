#pragma once
//**********************************************************************
#include "..\Consolidation.h"
#include "..\SQLResultInfo.h"
//**********************************************************************
#include "SQLRowPurchaseControlRule.h"
//**********************************************************************

struct CEnabledPurchaseControlInfo
{
public:
	int m_nRuleID;

public:
	CEnabledPurchaseControlInfo()
	{
		m_nRuleID = 1;
	}
};

//**********************************************************************

class CSQLRepositoryPurchaseControlRule
{
public:
	CSQLRepositoryPurchaseControlRule();

public:
	CSQLResultInfo SelectRow(CSQLRowPurchaseControlRule& SQLRowOS, CDatabase* pDatabase);
	CSQLResultInfo UpdateRow(CSQLRowPurchaseControlRule& SQLRowOS, CDatabase* pDatabase);
	CSQLResultInfo InsertRow(CSQLRowPurchaseControlRule& SQLRowOS, CDatabase* pDatabase);
	CSQLResultInfo UpsertRow(CSQLRowPurchaseControlRule& SQLRowOS, CDatabase* pDatabase);
	CSQLResultInfo DeleteRow(CSQLRowPurchaseControlRule& SQLRowOS, CDatabase* pDatabase);
	CSQLResultInfo RowExists(CSQLRowPurchaseControlRule& SQLRowOS, CDatabase* pDatabase);
	
public:
	void ImportPurchaseControlInfo(CString strFilename);
	void GetEnabledRules(CArray<CEnabledPurchaseControlInfo, CEnabledPurchaseControlInfo>& arrayRules);
	void GetRuleName(int nRuleID, int& nLastRuleID, CString& strRuleName, CDatabase* pDatabase);
	CString GetRuleName(int nRuleID, CDatabase* pDatabase);
};

//**********************************************************************
