#pragma once
//*******************************************************************
#include "resource.h"
//*******************************************************************
#include "AuditRecord.h"
//*******************************************************************

class CCalculateBonus
{
public:
	CCalculateBonus ( bool bPurse2, double dCashAdded, CAuditRecord* pAtcRecord );
	~CCalculateBonus();

public:
	double GetBonusCash() { return m_dBonusCash; }
	int GetBonusPoints() { return m_nBonusPoints; }

private:
	void CalculateBonus();

private:
	double m_dCashAdded;
	CAuditRecord* m_pAtcRecord;
	double m_dBonusCash;
	int m_nBonusPoints;
	bool m_bPurse2;
};

//*******************************************************************
