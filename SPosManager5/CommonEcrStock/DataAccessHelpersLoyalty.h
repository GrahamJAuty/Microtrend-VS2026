#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "DataAccessHelpers.h"
/**********************************************************************/

class CDataAccessHelpersLoyalty : public CDataAccessHelpers
{
public:
	CDataAccessHelpersLoyalty();

public:
	void RunLoyaltyV2();
	void RunLoyaltyV3();
	void RunLoyaltyV4();
	void RunSmartPayV1();
	void RunSmartPayV2();
	void RunSmartPayV4();
};

/**********************************************************************/
#endif
/**********************************************************************/
