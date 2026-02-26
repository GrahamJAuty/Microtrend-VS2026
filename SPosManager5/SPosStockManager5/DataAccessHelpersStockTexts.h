#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "..\CommonEcrStock\DataAccessHelpers.h"
/**********************************************************************/

class CDataAccessHelpersStockTexts : public CDataAccessHelpers
{
public:
	CDataAccessHelpersStockTexts();

public:
	void EditCategory( int nDbNo );
	void EditCustomField( int nDbNo );
	void EditAllowance( int nDbNo );
	void EditSupplier();
	void EditStockUnit();
	void EditSubUnit();
	void EditAdjustmentTexts();
	void EditDeliveryUnit();
};

/**********************************************************************/
#endif
/**********************************************************************/
