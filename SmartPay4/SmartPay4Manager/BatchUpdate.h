#pragma once
//***************************************************************
#include "..\SmartPay4Shared\ServerData.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRowAccountFull.h"
//***************************************************************
#include "BatchAccountBalanceMemory.h"
#include "BatchAccountDetailMemory.h"
#include "BatchPropertySheet.h"
//***************************************************************
#define	nBATCHUPDATE_UPDATE	0
#define nBATCHUPDATE_DELETE	1
#define nBATCHUPDATE_REDEEM	2
//***************************************************************

class CBatchUpdate
{
public:
	CBatchUpdate( int nUpdateType, CWnd* pParent );
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
	CString m_strLeaversYear;

private:
	CArray<CSQLRowAccountList, CSQLRowAccountList> m_arraySourceItems;
};

//***************************************************************
