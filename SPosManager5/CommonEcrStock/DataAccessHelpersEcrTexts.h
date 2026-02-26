#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "DataAccessHelpers.h"
/**********************************************************************/

class CDataAccessHelpersEcrTexts : public CDataAccessHelpers
{
public:
	CDataAccessHelpersEcrTexts();

public:
	void EditPluUnit();
	void EditPriceText();
	void EditSBPriceText();
	void EditModifier( int nDbNo );
	void EditPayment( int nDbNo );
	void EditSBPaymentMap( int nDbNo );
	void EditSEPaymentMap( int nDbNo );
	void EditPaidInOutText( int nDbNo );
	void EditAnalysisCategory( int nDbNo );
	void EditAllergen();
};

/**********************************************************************/
#endif
/**********************************************************************/
