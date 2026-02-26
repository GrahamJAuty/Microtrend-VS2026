/**********************************************************************/
 
/**********************************************************************/
#include "PriceHelpers.h"
/**********************************************************************/
#include "..\CommonStockTray\SalesCostCSVArray.h"
/**********************************************************************/

CSalesCostCSVRecord::CSalesCostCSVRecord()
{
	ClearRecord();
}

/**********************************************************************/

CSalesCostCSVRecord::~CSalesCostCSVRecord()
{
}

/**********************************************************************/

void CSalesCostCSVRecord::ClearRecord()
{
	m_strStockCode = "";
	Reset();
}

/**********************************************************************/

void CSalesCostCSVRecord::Reset()
{
	m_dRecipeSalesCost = 0.0;
	m_dManualSalesCost = 0.0;
	m_dImportSalesCost = 0.0;
	
	for ( int nLevel = 0; nLevel < STOCK_PRICES; nLevel++ )
		m_dEposSalesCost [ nLevel ] = 0.0;

	m_dEcrWasteCost = 0.0;
	m_dAutoWasteCost = 0.0;
}

/**********************************************************************/

void CSalesCostCSVRecord::ConvertFromCSV ( CCSV& csv )
{
	ClearRecord();

	SetStockCode ( csv.GetString(0) );
	switch ( csv.GetInt(1) )
	{
	case 1:		V1ConvertFromCSV( csv );		break;
	}
}

/**********************************************************************/

void CSalesCostCSVRecord::V1ConvertFromCSV ( CCSV& csv )
{
	int n = 2;
	SetRecipeSalesCost( csv.GetDouble(n++) );
	SetManualSalesCost( csv.GetDouble(n++) );
	SetImportSalesCost( csv.GetDouble(n++) );
	
	int nCount = csv.GetInt(n++);
	
	for ( int nLevel = 0; nLevel < nCount; nLevel++ )
	{
		if ( nLevel < STOCK_PRICES )
			SetEposSalesCost( nLevel, csv.GetDouble( n++ ) );
		else
			n++;
	}

	SetEcrWasteCost( csv.GetDouble(n++) );
	SetAutoWasteCost( csv.GetDouble(n++) );
}

/**********************************************************************/

void CSalesCostCSVRecord::ConvertToCSV ( CCSV& csv )
{
	csv.Add ( m_strStockCode );
	csv.Add ( SALESCOSTDB_VERSION );
	csv.Add ( GetRecipeSalesCost(), 3 );		
	csv.Add ( GetManualSalesCost(), 3 ); 
	csv.Add ( GetImportSalesCost(), 3 ); 
	
	csv.Add ( STOCK_PRICES );
	for ( int nLevel = 0; nLevel < STOCK_PRICES; nLevel++ )
		csv.Add ( GetEposSalesCost( nLevel ), 3 );

	csv.Add( GetEcrWasteCost(), 3 );
	csv.Add( GetAutoWasteCost(), 3 );
}

/**********************************************************************/

void CSalesCostCSVRecord::SetEposSalesCost ( int nLevel, double dValue )
{
	if ( nLevel >= 0 && nLevel < STOCK_PRICES )
		m_dEposSalesCost [ nLevel ] = dValue;
}

/**********************************************************************/

double CSalesCostCSVRecord::GetEposSalesCost ( int nLevel )
{
	if ( nLevel >= 0 && nLevel < STOCK_PRICES )
		return m_dEposSalesCost [ nLevel ];
	else
		return 0.0;
}

/**********************************************************************/

double CSalesCostCSVRecord::GetEposSalesCost()
{
	double dBuffer = 0.0;

	for ( int nLevel = 0; nLevel < STOCK_PRICES; nLevel++ )
		dBuffer += m_dEposSalesCost [ nLevel ];

	return dBuffer;
}

/**********************************************************************/

CSalesCostCSVRecord& CSalesCostCSVRecord::operator= ( CSalesCostCSVRecord& source )
{
	m_strStockCode = source.GetStockCode();
	m_dRecipeSalesCost = source.GetRecipeSalesCost();
	m_dManualSalesCost = source.GetManualSalesCost();
	m_dImportSalesCost = source.GetImportSalesCost();
	
	for ( int nLevel = 0; nLevel < STOCK_PRICES; nLevel++ )
		m_dEposSalesCost [ nLevel ] = source.m_dEposSalesCost [ nLevel ];
	
	m_dEcrWasteCost = source.GetEcrWasteCost();
	m_dAutoWasteCost = source.GetAutoWasteCost();

	return *this;
}

/**********************************************************************/

CSalesCostCSVRecord& CSalesCostCSVRecord::operator+ ( CSalesCostCSVRecord& source )
{
	m_dRecipeSalesCost += source.GetRecipeSalesCost();
	m_dManualSalesCost += source.GetManualSalesCost();
	m_dImportSalesCost += source.GetImportSalesCost();
	
	for ( int nLevel = 0; nLevel < STOCK_PRICES; nLevel++ )
		m_dEposSalesCost [ nLevel ] += source.m_dEposSalesCost [ nLevel ];
	
	m_dEcrWasteCost += source.GetEcrWasteCost();
	m_dAutoWasteCost += source.GetAutoWasteCost();

	return *this;
}

/**********************************************************************/

double CSalesCostCSVRecord::GetTotalSalesCost()
{
	double dBuffer = 0.0;

	for ( int nLevel = 0; nLevel < STOCK_PRICES; nLevel++ )
		dBuffer += m_dEposSalesCost [ nLevel ];

	dBuffer += m_dManualSalesCost;
	dBuffer += m_dImportSalesCost;
	dBuffer += m_dRecipeSalesCost;

	return dBuffer;
}

/**********************************************************************/

void CSalesCostCSVRecord::AddEposSalesCost( int nLevel, double dCost )
{
	if ( nLevel >= 0 && nLevel < STOCK_PRICES )
		m_dEposSalesCost [ nLevel ] += dCost;
}

/**********************************************************************/

void CSalesCostCSVRecord::SetCosts( CStockCSVRecord& StockRecord, CStockLevelsCSVRecord& StockLevels, bool bAutoWaste )
{
	double dDelQty = StockLevels.GetDeliveryQty() + StockLevels.GetFreeQty();
	double dDelVal = StockLevels.GetDeliveryValue();
		
	double dSUCost = 0.0;
	if ( CPriceHelpers::CompareDoubles( dDelQty, 0.0, 5 ) != 0 )
		dSUCost = dDelVal / dDelQty;
	else
		dSUCost = StockRecord.GetUnitCost(0);

	for ( int n = 0; n < STOCK_PRICES; n++ )
		m_dEposSalesCost[n] = StockLevels.GetEposSalesQty(n) * dSUCost;
	
	m_dRecipeSalesCost = StockLevels.GetRecipeSalesQty() * dSUCost;
	m_dManualSalesCost = StockLevels.GetManualSalesQty() * dSUCost;
	m_dImportSalesCost = StockLevels.GetImportSalesQty() * dSUCost;
	m_dEcrWasteCost = StockLevels.GetEcrWastageQty() * dSUCost;
	m_dAutoWasteCost = 0.0;

	if ( TRUE == bAutoWaste )
	{
		double dWastePercent = StockRecord.GetWastePercent();
		double dNonWastePercent = 100.0 - dWastePercent;

		if ( ( dWastePercent > 0.0 ) && ( dNonWastePercent > 0.0 ) )
		{
			double dTotalSales = StockLevels.GetTotalSalesQty();
			double dAutoWasteQty = ( dTotalSales * dWastePercent ) / dNonWastePercent;
			m_dAutoWasteCost = dAutoWasteQty * dSUCost;
		}
	}
}

/**********************************************************************/
