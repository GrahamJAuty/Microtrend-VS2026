#pragma once
//*******************************************************************
#define CASHLESS_SOURCE_ECR	0
#define CASHLESS_SOURCE_SRV	1
#define CASHLESS_SOURCE_PC	2
#define CASHLESS_SOURCE_EXT	3
#define CASHLESS_SOURCE_CNT 4
//*******************************************************************
#define CASHLESS_ACTION_SALE 0
#define CASHLESS_ACTION_MANUALSPEND 1
#define CASHLESS_ACTION_TOPUP 2
#define CASHLESS_ACTION_TOPUPBONUS 3
#define CASHLESS_ACTION_SPENDBONUS 4
#define CASHLESS_ACTION_DEPTBONUS 5
#define CASHLESS_ACTION_REDEEM 6
#define CASHLESS_ACTION_REFRESH 7
#define CASHLESS_ACTION_ADJUST 8
#define CASHLESS_ACTION_TRANSFER 9
#define CASHLESS_ACTION_CNT 10
//*******************************************************************

class CCashlessTotal
{
public:
	CCashlessTotal()
	{
		Reset();
	}

	void Reset()
	{
		m_dPurse1Plus = 0.0;
		m_dPurse1Minus = 0.0;
		m_dPurse2Plus = 0.0;
		m_dPurse2Minus = 0.0;
		m_nPurse1PlusCount = 0;
		m_nPurse1MinusCount = 0;
		m_nPurse2PlusCount = 0;
		m_nPurse2MinusCount = 0;
	}

	void Consolidate(int nPurseNo, double dAmount)
	{
		switch (nPurseNo)
		{
		case 1:
			if (dAmount < 0.0)
			{
				m_dPurse1Minus -= dAmount;
				m_nPurse1MinusCount++;
			}
			else
			{
				m_dPurse1Plus += dAmount;
				m_nPurse1PlusCount++;
			}
			break;

		case 2:
			if (dAmount < 0.0)
			{
				m_dPurse2Minus -= dAmount;
				m_nPurse2MinusCount++;
			}
			else
			{
				m_dPurse2Plus += dAmount;
				m_nPurse2PlusCount++;
			}
			break;
		}
	}

	void ConsolidateMinus(int nPurseNo, double dAmount)
	{
		switch (nPurseNo)
		{
		case 1:
			m_dPurse1Minus -= dAmount;
			m_nPurse1MinusCount++;
			break;

		case 2:
			m_dPurse2Minus -= dAmount;
			m_nPurse2MinusCount++;
			break;
		}
	}

	void ConsolidatePlus(int nPurseNo, double dAmount)
	{
		switch (nPurseNo)
		{
		case 1:
			m_dPurse1Plus += dAmount;
			m_nPurse1PlusCount++;
			break;

		case 2:
			m_dPurse2Plus += dAmount;
			m_nPurse2PlusCount++;
			break;
		}
	}

public:
	double m_dPurse1Plus;
	double m_dPurse1Minus;
	double m_dPurse2Plus;
	double m_dPurse2Minus;
	int m_nPurse1PlusCount;
	int m_nPurse1MinusCount;
	int m_nPurse2PlusCount;
	int m_nPurse2MinusCount;
};

//*******************************************************************

