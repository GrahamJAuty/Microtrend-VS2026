 /**********************************************************************/
#include "SysInfo.h"
/**********************************************************************/
#include "PriceHelpers.h"
/**********************************************************************/

double CPriceHelpers::GetPrice1GPFixed ( double dTaxRate, double dCost, double dProfit )
{
	double dPreciseRetail = ( dCost + dProfit ) * ( 1 + ( dTaxRate / 100.0 ) );
	return GetDatabaseRetail ( dPreciseRetail );
}

/**********************************************************************/

double CPriceHelpers::GetPrice1GPPercent ( double dTaxRate, double dCost, double dProfit )
{
	double dCostPercent = 100.0 - dProfit;

	if ( dCostPercent == 0.0 )
		return 0.0;
	else
	{
		double dPreciseRetail = ( dCost * 100 ) / dCostPercent;
		dPreciseRetail *= ( 1 + ( dTaxRate / 100.0 ) );

		return GetDatabaseRetail( dPreciseRetail );
	}
}

/**********************************************************************/

double CPriceHelpers::GetDatabaseRetail( double dPreciseRetail )
{
	//WORK ROUND FOR A MICROSOFT FUNNY IN WHICH NUMBERS SUCH AS 0.605 ARE
	//ROUNDED DOWN TO 0.60 INSTEAD OF UP TO 0.61

	CString strRoundedRetail;
	strRoundedRetail.Format ( "%.3f1", dPreciseRetail );
	double dRoundedRetail = atof ( strRoundedRetail );

	CString strDatabaseRetail;
	strDatabaseRetail.Format( "%.*f", SysInfo.GetDPValue(), dRoundedRetail );
	double dDatabaseRetail = atof( strDatabaseRetail );

	return dDatabaseRetail;
}

/**********************************************************************/

double CPriceHelpers::GetUnitPriceRatio ( CPluCSVRecord& PluRecord ) 
{
	CString strText = PluRecord.GetUnitPriceText();
	double dSize = PluRecord.GetUnitPriceSize();
	return GetUnitPriceRatio ( strText, dSize );
}

/**********************************************************************/

double CPriceHelpers::GetUnitPriceRatio ( const char* szText, double dSize )
{
	CString strText = szText;
	CString strUnit = "";
	bool bGotUnit = FALSE;

	for ( int nPos = 0; nPos < strText.GetLength(); nPos++ )
	{
		char c = strText.GetAt ( nPos );

		if ( c >= '0' && c <= '9' )
		{
			strUnit += c;
			bGotUnit = TRUE;
		}
		else if ( bGotUnit == TRUE )
			break;
	}

	int nUnit = atoi ( strUnit );
	if ( nUnit == 0 ) nUnit = 1;

	return ( dSize / ( double ) nUnit );	
}

/**********************************************************************/

int CPriceHelpers::CompareDoubles( double dDouble1, double dDouble2, int nDp )
{
	CString strDouble1, strDouble2;
	strDouble1.Format ( "%.*f", nDp, dDouble1 );
	strDouble2.Format ( "%.*f", nDp, dDouble2 );

	if ( strDouble1 == strDouble2 )
		return 0;

	if ( ( strDouble1.Left(1) == "-" ) != ( strDouble2.Left(1) == "-" ) )
	{
		CString strNegativeZero = "-0";

		if ( nDp != 0 )
		{
			strNegativeZero += ".";
			::AddTrailing( strNegativeZero, nDp + 3, '0' );
		}

		if ( strDouble1 == strNegativeZero )
			strDouble1 = strDouble1.Right( strDouble1.GetLength() - 1 );

		if ( strDouble2 == strNegativeZero )
			strDouble2 = strDouble2.Right( strDouble2.GetLength() - 1 );
	}

	if ( strDouble1 == strDouble2 )
		return 0;
	else if ( dDouble1 < dDouble2 )
		return -1;
	else 
		return 1;
}

/**********************************************************************/

void CPriceHelpers::FormatDouble( double dDouble, int nDp, CString& strDouble )
{
	strDouble.Format ( "%.*f", nDp, dDouble );

	if ( strDouble.Left(1) == "-" )
	{
		CString strNegativeZero = "-0";

		if ( nDp != 0 )
		{
			strNegativeZero += ".";
			::AddTrailing( strNegativeZero, nDp + 3, '0' );
		}

		if ( strDouble == strNegativeZero )
			strDouble = strDouble.Right( strDouble.GetLength() - 1 );
	}
}

/**********************************************************************/
