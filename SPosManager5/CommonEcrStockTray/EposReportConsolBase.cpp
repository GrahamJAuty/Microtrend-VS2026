/**********************************************************************/
#include "EposReportConsolBase.h"
/**********************************************************************/

CEposReportConsolBase::CEposReportConsolBase()
{
	m_dValBeforeDiscount = 0.0;
	m_dDiscountVal = 0.0;
	m_dPremiumVal = 0.0;
}

/**********************************************************************/

void CEposReportConsolBase::BaseReset()
{
	m_dValBeforeDiscount = 0.0;
	m_dDiscountVal = 0.0;
	m_dPremiumVal = 0.0;
}

/**********************************************************************/

void CEposReportConsolBase::SetDiscountOrPremium(CPluSalesLine& SalesLine, bool bAllowPremium)
{
	if ((TRUE == bAllowPremium) && (SalesLine.IsPremium() == TRUE))
	{
		m_dDiscountVal = 0.0;
		m_dPremiumVal = SalesLine.GetValue() * (-1);
	}
	else
	{
		m_dDiscountVal = SalesLine.GetValue();
		m_dPremiumVal = 0.0;
	}
}

/**********************************************************************/

void CEposReportConsolBase::BaseAdd (CEposReportConsolBase& source )
{
	m_dValBeforeDiscount += source.m_dValBeforeDiscount;
	m_dDiscountVal += source.m_dDiscountVal;
	m_dPremiumVal += source.m_dPremiumVal;
}

/**********************************************************************/

