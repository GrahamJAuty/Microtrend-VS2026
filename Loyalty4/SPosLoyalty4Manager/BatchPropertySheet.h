#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************
#include "..\SPosLoyalty4Shared\LoyaltySubs.h"
#include "..\SPosLoyalty4Shared\SQLTable_Account\SQLRowAccountFull.h"
//$$******************************************************************
#include "BatchPropertyPage1.h"
#include "BatchPropertyPage2.h"
#include "BatchPropertyPage3.h"
#include "BatchPropertyPage5.h"
#include "BatchUpdateHelpers.h"
#include "DbExportHandler.h"
//$$******************************************************************

class CBatchPropertySheet : public CSSPropertySheet
{
public:
	CBatchPropertySheet( const char* szTitle, int nUpdateType, CSQLRowAccountFull& RowAccount, CBatchAccountBalanceMemory& BalanceMemory, CBatchAccountDetailMemory& DetailMemory, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	~CBatchPropertySheet();

public:
	bool HaveRecords() { return m_UpdateHelpers.HaveRecords(); }
	bool HaveUpdates();
	
	CString GetComment() { return m_UpdateHelpers.GetComment(); }

public:
	CBatchPropertyPage1 m_Page1;
	CBatchPropertyPage2 m_Page2;
	CBatchPropertyPage3 m_Page3;
	CBatchPropertyPage5 m_Page5;
	CBatchUpdateHelpers m_UpdateHelpers;

public:
	void GetSQLConditions(CDbExportHandler& DbExportHandler) { m_Page3.GetSQLConditions(DbExportHandler); }
	void GetSQLFilters(CDbExportHandler& DbExportHandler) { m_UpdateHelpers.GetSQLFilters(DbExportHandler); }
	bool NonSQLFilter(CSQLRowAccountFull& RowAccount) { return m_UpdateHelpers.NonSQLFilter(RowAccount); }

public:
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

private:
	int m_nUpdateType;
};

//$$******************************************************************
