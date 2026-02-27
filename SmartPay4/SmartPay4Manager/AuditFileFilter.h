#pragma once
//$$******************************************************************

struct CRunningAuditTotals
{
public:
	double m_dPurse1Credit = 0.0;
	double m_dPurse1Liability = 0.0;
	double m_dPurse2Balance = 0.0;
	double m_dPurse3Credit = 0.0;
	double m_dPurse3Liability = 0.0;
	int m_nGroupNo = 0;
	int m_nPoints = 0;
	double m_dPurse1ToDate = 0.0;
	double m_dPurse2ToDate = 0.0;
	double m_dPurse3ToDate = 0.0;
	double m_dCashSpendToDate = 0.0;
	int m_nPointsToDate = 0;
	CString m_strUsername = "";
};

//$$******************************************************************

class CAuditFileFilter
{
public:
	void FilterAuditFile();
};

//$$******************************************************************
