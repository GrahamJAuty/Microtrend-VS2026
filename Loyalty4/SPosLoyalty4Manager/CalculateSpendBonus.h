#pragma once
//*******************************************************************
#include "resource.h"
//*******************************************************************
#include "AuditRecord.h"
//*******************************************************************

class CCalculateSpendBonus
{
public:
	CCalculateSpendBonus ( double dSpend, CAuditRecord* pAtcRecord );
	~CCalculateSpendBonus();

public:
	double GetBonusCash() { return m_dBonusCash; }
	int GetBonusPoints() { return m_nBonusPoints; }

private:
	void CalculateBonus();

private:
	double m_dSpend;
	CAuditRecord* m_pAtcRecord;

	double m_dBonusCash;
	int m_nBonusPoints;
};

//*******************************************************************
