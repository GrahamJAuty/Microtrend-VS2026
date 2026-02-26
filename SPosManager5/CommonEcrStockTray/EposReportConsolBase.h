#pragma once
/**********************************************************************/
#include "EposReportBlockMap.h"
/**********************************************************************/

struct CEposReportConsolBase
{
public:
	CEposReportConsolBase();
	void BaseReset();

public:
	void BaseAdd(CEposReportConsolBase& source);
	void SetDiscountOrPremium(CPluSalesLine& SalesLine, bool bAllowPremium);

public:
	double m_dValBeforeDiscount;
	double m_dDiscountVal;
	double m_dPremiumVal;
};

/**********************************************************************/

