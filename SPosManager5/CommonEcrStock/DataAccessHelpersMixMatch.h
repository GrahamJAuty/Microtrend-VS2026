#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "DataAccessHelpers.h"
/**********************************************************************/

class CDataAccessHelpersMixMatch : public CDataAccessHelpers
{
public:
	CDataAccessHelpersMixMatch();

public:
	void EditMixMatch(int nDbNo);
	void EditPromotion(int nDbNo);
	void EditOfferGroup(int nDbNo);
	void EditTimePlan(int nDbNo);
	void EditWalletStampRule(int nDbNo);
};

/**********************************************************************/
#endif
/**********************************************************************/
