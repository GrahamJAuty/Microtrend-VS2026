#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "DataAccessHelpers.h"
/**********************************************************************/

class CDataAccessHelpersEcrData : public CDataAccessHelpers
{
public:
	CDataAccessHelpersEcrData();

public:
	void EditBarcode(int nDbNo);
	void EditDept(int nDbNo);
	void EditGroup(int nDbNo);
	void EditTax(int nDbNo);
	void EditLoyaltyScheme(int nDbNo);

private:
	void EditGlobalTax();
	bool EditTaxInternal();
};

/**********************************************************************/
#endif
/**********************************************************************/
