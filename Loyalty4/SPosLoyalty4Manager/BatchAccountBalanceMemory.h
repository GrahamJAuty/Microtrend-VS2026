#pragma once
//$$******************************************************************
#include "..\SPosLoyalty4Shared\SQLTable_Account\SQLRowAccountFull.h"
//$$******************************************************************

class CBatchAccountBalanceMemory
{
public:
	CBatchAccountBalanceMemory();
	bool HaveUpdates();
	bool UpdateRecord(CSQLRowAccountFull& RowAccount);
	void TidyOptions();

public:
	bool m_bAddPurse1;
	bool m_bAddPurse2;
	bool m_bAddPoints;
	bool m_bAddPurse1SpendTD;
	bool m_bAddPurse2SpendTD;
	bool m_bAddCashSpendTD;
	bool m_bAddPointsTD;
	bool m_bClearPurse1;
	bool m_bClearPurse2;
	bool m_bClearPoints;
	bool m_bClearPurse1SpendTD;
	bool m_bClearPurse2SpendTD;
	bool m_bClearCashSpendTD;
	bool m_bClearPointsTD;
	double m_dPurse1;
	double m_dPurse2;
	int m_nPoints;
	double m_dPurse1SpendTD;
	double m_dPurse2SpendTD;
	double m_dCashSpendTD;
	int m_nPointsTD;
};

//$$******************************************************************
