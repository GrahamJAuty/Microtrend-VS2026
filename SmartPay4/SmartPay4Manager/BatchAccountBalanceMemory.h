#pragma once
//$$******************************************************************
#include "..\SmartPay4Shared\SQLTable_Account\SQLRowAccountFull.h"
//$$******************************************************************

class CBatchAccountBalanceMemory
{
public:
	CBatchAccountBalanceMemory();
	bool HaveUpdates();
	bool UpdateRecord(CSQLRowAccountFull& RowAccount);
	void TidyOptions();
	void AddWarnings(CStringArray* pArray);

public:
	bool m_bAddPurse1Liability;
	bool m_bAddPurse1Credit;
	bool m_bAddPurse3Liability;
	bool m_bAddPurse3Credit;
	bool m_bAddPoints;
	bool m_bAddPurse1SpendTD;
	bool m_bAddPurse2SpendTD;
	bool m_bAddPurse3SpendTD;
	bool m_bAddPointsTD;
	bool m_bClearPurse1Liability;
	bool m_bClearPurse1Credit;
	bool m_bClearPurse3Liability;
	bool m_bClearPurse3Credit;
	bool m_bClearPoints;
	bool m_bClearPurse1SpendTD;
	bool m_bClearPurse2SpendTD;
	bool m_bClearPurse3SpendTD;
	bool m_bClearCashSpendTD;
	bool m_bClearPointsTD;
	double m_dPurse1Liability;
	double m_dPurse1Credit;
	double m_dPurse3Liability;
	double m_dPurse3Credit;
	int m_nPoints;
	double m_dPurse1SpendTD;
	double m_dPurse2SpendTD;
	double m_dPurse3SpendTD;
	int m_nPointsTD;
};

//$$******************************************************************
