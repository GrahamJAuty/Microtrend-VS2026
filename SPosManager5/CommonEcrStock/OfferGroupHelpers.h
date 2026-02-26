#pragma once
/**********************************************************************/
//include "MyComboBox.h"
/**********************************************************************/

class COfferGroupHelpers
{
public:
	static void FillOfferGroupCombo( CSSComboBox& combo, int nOfferGroup );
	static int GetOfferGroup( CSSComboBox& combo );
	static void RefillOfferGroupCombo( CSSComboBox& combo );
	static bool EditOfferGroupInternal( bool bWalletStampRule, CSSComboBox& combo, CWnd* pParent );	
};

/**********************************************************************/
