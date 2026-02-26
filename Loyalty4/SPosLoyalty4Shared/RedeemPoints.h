#pragma once
//*******************************************************************

class CRedeemPoints
{
public:
	CRedeemPoints();
	int Redeem(int nPointsBalance, double& dValue);
	int GetLastError() { return m_nLastError; }

public:
	bool	m_bDisabled;
	int		m_nTrip;
	double	m_dValue;
	int		m_nType;						// standard / family free

private:
	int		m_nLastError;
};

//*******************************************************************
