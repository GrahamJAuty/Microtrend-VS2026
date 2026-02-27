//*******************************************************************
#pragma once
//*******************************************************************
#include "..\SmartPay4Shared\SQLTable_Account\SQLRowAccountFull.h"
//*******************************************************************
#include "BatchAccountBalanceMemory.h"
#include "BatchAccountDetailMemory.h"
#include "BatchPropertyPage1.h"
#include "BatchPropertyPage2.h"
#include "BatchPropertyPage3.h"
#include "BatchPropertyPage5.h"
//*******************************************************************

class CBatchPropertySheet : public CSSAutoShutdownPropertySheet
{
// Construction
public:
	CBatchPropertySheet( const char* szTitle, int m_nUpdateType, CBatchAccountBalanceMemory& BalanceMemory, CBatchAccountDetailMemory& DetailMemory, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CBatchPropertySheet() {}

	bool IsValid(CSQLRowAccountFull& RowAccount);
	bool HaveRecords() { return m_Page1.HaveRecords(); }
	bool HaveUpdates();
	void AddWarnings( CStringArray* pArray );
	bool UpdateRecord(CSQLRowAccountFull& RowAccount);
	
	const char* GetComment() { return m_Page1.GetComment(); }
	void SetComment ( const char* szComment ) { m_Page1.m_strComment = szComment; }

// Attributes
public:
	CBatchPropertyPage1 m_Page1;
	CBatchPropertyPage2 m_Page2;
	CBatchPropertyPage3 m_Page3;
	CBatchPropertyPage5 m_Page5;

public:
	DECLARE_MESSAGE_MAP()

private:
	int m_nUpdateType;
};

//*******************************************************************
