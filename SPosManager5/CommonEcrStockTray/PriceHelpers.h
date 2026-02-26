#pragma once
/**********************************************************************/
 
/**********************************************************************/
#include "PluCSVArray.h"
/**********************************************************************/

class CPriceHelpers  
{
public:
	static double GetUnitPriceRatio ( CPluCSVRecord& PluRecord );
	static double GetUnitPriceRatio ( const char* szText, double dSize );
	static double GetPrice1GPFixed ( double dTaxRate, double dCost, double dProfit );
	static double GetPrice1GPPercent ( double dTaxRate, double dCost, double dProfit );
	static double GetDatabaseRetail( double dPreciseRetail );
	static int CompareDoubles( double dDouble1, double dDouble2, int nDp );
	static void FormatDouble( double dDouble, int nDp, CString& strDouble );
};

/**********************************************************************/
