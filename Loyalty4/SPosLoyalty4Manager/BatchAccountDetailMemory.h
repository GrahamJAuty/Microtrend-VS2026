#pragma once
//$$******************************************************************
#include "..\SPosLoyalty4Shared\SQLTable_Account\SQLRowAccountFull.h"
//$$******************************************************************
#define BATCH_DATEACTION_NONE 0
#define BATCH_DATEACTION_CLEAR 1
#define BATCH_DATEACTION_SET 2
//$$******************************************************************

class CBatchAccountDetailMemory
{
public:
	CBatchAccountDetailMemory();
	bool HaveUpdates();
	bool UpdateRecord(CSQLRowAccountFull& RowAccount);
	void TidyOptions();

public:
	bool m_bSetName;
	bool m_bSetGroup;
	bool m_bSetScheme;
	int m_nExpiryAction;
	int m_nDOBAction;
	bool m_bSetMaxSpend;
	bool m_bSetOverSpend;
	bool m_bSetAlert;
	bool m_bSetHotlist;
	bool m_bSetInactive;
	bool m_bSetGender;

	CString m_strName;
	int m_nGroupNo;
	int m_nSchemeNo;
	COleDateTime m_dateExpiry;
	COleDateTime m_dateDOB;
	double m_dMaxSpend;
	double m_dOverSpend;
	CString m_strAlertCodes;
	int m_nHotlistIndex;
	int m_nInactiveIndex;
	int m_nGenderIndex;
};

//$$******************************************************************
