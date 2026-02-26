//*******************************************************************
#include "RedeemPoints.h"
//*****************************************************************

CRedeemPoints::CRedeemPoints()
{
	m_bDisabled = FALSE;
	m_nTrip = 0;
	m_dValue = 0;
	m_nType = nREDEEM_STANDARD;
	m_nLastError = 0;
}

//******************************************************************
// MUST fill out all values before call this function 

int CRedeemPoints::Redeem ( int nPointsBalance, double& dValue )
{
	m_nLastError = 0;

	dValue = 0.0;									// value of points redeemed

	if ( m_nType == nREDEEM_FAMILY )				// no cash for points redemption with family promotion
	{
		m_nLastError = 1;
		return 0;
	}

	if ( m_nTrip == 0 )								// see if redemption point trip set up
	{
		m_nLastError = m_bDisabled ? 4 : 2;
		return 0;
	}

	if ( nPointsBalance < m_nTrip )					// not enough points
	{
		m_nLastError = m_bDisabled ? 4 : 3;
		return 0;
	}

	if (TRUE == m_bDisabled)
	{
		m_nLastError = 4;
	}

	int nFactor = ( nPointsBalance / m_nTrip ); 

	dValue	= ( m_dValue * nFactor );
	return ( m_nTrip * nFactor );					// return number of points redeemed
}

//******************************************************************
