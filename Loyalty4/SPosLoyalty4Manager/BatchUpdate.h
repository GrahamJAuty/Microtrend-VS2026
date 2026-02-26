#pragma once
//*******************************************************************
#include "resource.h"
//*******************************************************************
#include "..\SPosLoyalty4Shared\SQLDb.h"
#include "..\SPosLoyalty4Shared\SQLTable_Account\SQLRowAccountFull.h"
//*******************************************************************
#include "BatchPropertySheet.h"
//*******************************************************************
#define	nBATCHUPDATE_UPDATE	0
#define nBATCHUPDATE_DELETE	1
#define nBATCHUPDATE_REDEEM	2
//*******************************************************************

class CBatchUpdate
{
public:
	CBatchUpdate( int nUpdateType, CSQLRowAccountFull& RowAccount, CWnd* pParent );
	~CBatchUpdate();
	
public:
	const char* DoUpdate();
	const char* GetTitle(){ return m_strTitle; }

private:
	int CreateUpdateList ( CBatchPropertySheet* pProp );
	bool ValidateList ( CBatchPropertySheet* pProp );
	int Update ( CBatchPropertySheet* pProp, int nPossibleCount, CBatchAccountBalanceMemory& BalanceMemory, CBatchAccountDetailMemory& DetailMemory);
	
private:
	CWnd* m_pParent;
	int m_nUpdateType;

private:
	CString m_strTitle;
	CString m_strFinalMessage;
	CSQLRowAccountFull& m_RowAccount;
	CArray<CSQLRowAccountList, CSQLRowAccountList> m_arraySourceItems;
};

//*******************************************************************
