#pragma once
//$$******************************************************************
#include "..\SmartPay4Shared\AlertText.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRowAccountFull.h"
//$$******************************************************************

class CBatchAccountDetailMemory
{
public:
	CBatchAccountDetailMemory();
	bool HaveUpdates();
	bool UpdateRecord(CSQLRowAccountFull& RowAccount);
	void TidyOptions();
	void AddWarnings(CStringArray* pArray, CAlertText& AlertText);

public:
	bool m_bSetGroup;
	bool m_bSetExpiry;
	bool m_bSetDob;
	bool m_bSetMaxSpend;
	bool m_bSetOverSpend;
	bool m_bSetAlert;

	bool m_bClearGroup;
	bool m_bClearExpiry;
	bool m_bClearDob;
	bool m_bClearMaxSpend;
	bool m_bClearOverSpend;

	bool m_bSetName;

	int m_nGroupNo;
	COleDateTime m_dateExpiry;
	COleDateTime m_dateDob;
	double m_dMaxSpend;
	double m_dOverSpend;
	int m_nAlertCode;
};

//$$******************************************************************
