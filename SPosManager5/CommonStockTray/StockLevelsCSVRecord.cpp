/**********************************************************************/
#include "..\CommonStockTray\StockCSVArray.h"
/**********************************************************************/
#include "StockLevelsCSVArray.h"
/**********************************************************************/

CStockLevelsCSVRecord::CStockLevelsCSVRecord()
{
	ClearRecord();
}

/**********************************************************************/

CStockLevelsCSVRecord::~CStockLevelsCSVRecord()
{
}

/**********************************************************************/

void CStockLevelsCSVRecord::ClearRecord()
{
	m_strStockCode = "";
	Reset();
}

/**********************************************************************/

void CStockLevelsCSVRecord::Reset()
{
	m_dOpeningQty = 0.0;
	m_dDeliveryQty = 0.0;
	m_dDeliveryValue = 0.0;
	m_dFreeQty = 0.0;
	m_dTransferInQty = 0.0;
	m_dRecipeSalesQty = 0.0;
	m_dRecipeSalesValue = 0.0;
	m_dManualSalesQty = 0.0;
	m_dManualSalesValue = 0.0;
	m_dImportSalesQty = 0.0;
	m_dImportSalesValue = 0.0;
	m_dAdjustmentQty = 0.0;
	m_dAdjustmentValue = 0.0;
	m_dReturnsValue = 0.0;
	m_dReturnsQty = 0.0;
	m_dEcrWastageQty = 0.0;
	m_dEcrWastageValue = 0.0;
	m_dTransferOutQty = 0.0;
	m_dYTDSalesQty = 0.0;
	m_dYTDSalesValue = 0.0;
	m_dYTDAdjustmentQty = 0.0; 
	m_dYTDAdjustmentValue = 0.0;
	m_dYTDEcrWastageQty = 0.0; 
	m_dYTDEcrWastageValue = 0.0;
	m_dQtyOnOrder = 0.0;
	m_dMinimumLevelQty = 0.0;
	m_dMaximumLevelQty = 0.0;
	m_dReorderLevelQty = 0.0;
	m_dAutoWastagePercent = 0.0;

	m_bActive = TRUE;
	m_nStocktakeNo = 0;
	m_dStocktakeDeliveryQty = 0.0;
	m_dStocktakeDeliveryValue = 0.0;
	m_dStocktakeFreeQty = 0.0;
	m_dStocktakeTransferInQty = 0.0;
	m_dStocktakeRecipeSalesQty = 0.0;
	m_dStocktakeRecipeSalesValue = 0.0;
	m_dStocktakeManualSalesQty = 0.0;
	m_dStocktakeManualSalesValue = 0.0;
	m_dStocktakeImportSalesQty = 0.0;
	m_dStocktakeImportSalesValue = 0.0;
	m_dStocktakeAdjustmentQty = 0.0;
	m_dStocktakeAdjustmentValue = 0.0;
	m_dStocktakeReturnsQty = 0.0;
	m_dStocktakeReturnsValue = 0.0;
	m_dStocktakeEcrWastageQty = 0.0;
	m_dStocktakeEcrWastageValue = 0.0;
	m_dStocktakeTransferOutQty = 0.0;
	m_dStocktakeApparentQty = 0.0;
	m_dStocktakeClosingQty = 0.0;

	for ( int nLevel = 0; nLevel < STOCK_PRICES; nLevel++ )
	{
		m_dEposSalesQty [ nLevel ] = 0.0;
		m_dEposSalesValue [ nLevel ] = 0.0;
		m_dStocktakeEposSalesQty [ nLevel ] = 0.0;
		m_dStocktakeEposSalesValue [ nLevel ] = 0.0;
	}
}

/**********************************************************************/

void CStockLevelsCSVRecord::ConvertFromCSV ( CCSV& csv )
{
	ClearRecord();

	SetStockCode ( csv.GetString(0) );

	int nVer = csv.GetInt(1);
	switch ( nVer )
	{
	case 1:		V1ConvertFromCSV( csv );			break;
	case 2:		V2ConvertFromCSV( csv );			break;
	case 3:		V3ConvertFromCSV( csv );			break;
	case 4:		V4ConvertFromCSV( csv );			break;
	case 5:	
	case 6:		V5to6ConvertFromCSV( csv );			break;
	case 7:	
	case 8:	
	case 9: 
	case 10:	
	case 11:	
	case 12:	
	case 13:	
	case 14:	V7to14ConvertFromCSV( csv, nVer );	break;
	}
}

/**********************************************************************/

void CStockLevelsCSVRecord::V1ConvertFromCSV ( CCSV& csv )
{
	int n = 2;
	SetOpeningQty ( csv.GetDouble(n++) );
	SetDeliveryQty( csv.GetDouble(n++) );
	SetDeliveryValue( csv.GetDouble(n++) );
	SetFreeQty( csv.GetDouble(n++) );
	SetTransferInQty( csv.GetDouble(n++) );
	SetRecipeSalesQty( csv.GetDouble(n++) );
	SetRecipeSalesValue( csv.GetDouble(n++) );
	SetManualSalesQty( csv.GetDouble(n++) );
	SetManualSalesValue( csv.GetDouble(n++) );
	SetImportSalesQty( csv.GetDouble(n++) );
	SetImportSalesValue( csv.GetDouble(n++) );
	SetAdjustmentQty( csv.GetDouble(n++) );
	SetAdjustmentValue( csv.GetDouble(n++) );
	SetReturnsQty( csv.GetDouble(n++) );
	SetReturnsValue( csv.GetDouble(n++) );
	SetTransferOutQty( csv.GetDouble(n++) );
	n++;	//SKIP LEGACY CLOSING QTY
	SetYTDSalesQty( csv.GetDouble(n++) );
	SetYTDSalesValue( csv.GetDouble(n++) );
	SetYTDAdjustmentQty( csv.GetDouble(n++) );
	SetYTDAdjustmentValue( csv.GetDouble(n++) );
	SetQtyOnOrder ( csv.GetDouble(n++) );
	SetMinimumLevelQty ( csv.GetDouble(n++) );
	SetReorderLevelQty( csv.GetDouble(n++) );
	n++;	//SKIP LEGACY CUSTOMER SALES QTY
	n++;	//SKIP LEGACY CUSTOMER SALES VALUE
	
	//STOCK_PRICES IS NOW 10, BUT THIS VERSION ONLY HAS 7 PRICES
	for ( int nLevel = 0; nLevel < 7; nLevel++ )
	{
		SetEposSalesQty( nLevel, csv.GetDouble( n++ ) );
		SetEposSalesValue( nLevel, csv.GetDouble( n++ ) );
	}
}

/**********************************************************************/

void CStockLevelsCSVRecord::V2ConvertFromCSV ( CCSV& csv )
{
	int n = 2;
	SetOpeningQty ( csv.GetDouble(n++) );
	SetDeliveryQty( csv.GetDouble(n++) );
	SetDeliveryValue( csv.GetDouble(n++) );
	SetFreeQty( csv.GetDouble(n++) );
	SetTransferInQty( csv.GetDouble(n++) );
	SetRecipeSalesQty( csv.GetDouble(n++) );
	SetRecipeSalesValue( csv.GetDouble(n++) );
	SetManualSalesQty( csv.GetDouble(n++) );
	SetManualSalesValue( csv.GetDouble(n++) );
	SetImportSalesQty( csv.GetDouble(n++) );
	SetImportSalesValue( csv.GetDouble(n++) );
	SetAdjustmentQty( csv.GetDouble(n++) );
	SetAdjustmentValue( csv.GetDouble(n++) );
	SetReturnsQty( csv.GetDouble(n++) );
	SetReturnsValue( csv.GetDouble(n++) );
	SetTransferOutQty( csv.GetDouble(n++) );
	n++;	//SKIP LEGACY CLOSING QTY
	SetYTDSalesQty( csv.GetDouble(n++) );
	SetYTDSalesValue( csv.GetDouble(n++) );
	SetYTDAdjustmentQty( csv.GetDouble(n++) );
	SetYTDAdjustmentValue( csv.GetDouble(n++) );
	SetQtyOnOrder ( csv.GetDouble(n++) );
	SetMinimumLevelQty ( csv.GetDouble(n++) );
	SetReorderLevelQty( csv.GetDouble(n++) );
	n++;	//SKIP LEGACY CUSTOMER SALES QTY
	n++;	//SKIP LEGACY CUSTOMER SALES VALUE
	n++;	//IGNORED FROZEN STOCK QTY
	n++;	//IGNORED FROZEN STOCK VALUE

	//STOCK_PRICES IS NOW 8, BUT THIS VERSION ONLY HAS 7 PRICES
	for ( int nLevel = 0; nLevel < 7; nLevel++ )
	{
		SetEposSalesQty( nLevel, csv.GetDouble( n++ ) );
		SetEposSalesValue( nLevel, csv.GetDouble( n++ ) );
	}
}

/**********************************************************************/

void CStockLevelsCSVRecord::V3ConvertFromCSV ( CCSV& csv )
{
	int n = 2;
	SetOpeningQty ( csv.GetDouble(n++) );
	SetDeliveryQty( csv.GetDouble(n++) );
	SetDeliveryValue( csv.GetDouble(n++) );
	SetFreeQty( csv.GetDouble(n++) );
	SetTransferInQty( csv.GetDouble(n++) );
	SetRecipeSalesQty( csv.GetDouble(n++) );
	SetRecipeSalesValue( csv.GetDouble(n++) );
	SetManualSalesQty( csv.GetDouble(n++) );
	SetManualSalesValue( csv.GetDouble(n++) );
	SetImportSalesQty( csv.GetDouble(n++) );
	SetImportSalesValue( csv.GetDouble(n++) );
	SetAdjustmentQty( csv.GetDouble(n++) );
	SetAdjustmentValue( csv.GetDouble(n++) );
	SetReturnsQty( csv.GetDouble(n++) );
	SetReturnsValue( csv.GetDouble(n++) );
	SetEcrWastageQty( csv.GetDouble(n++) );
	SetEcrWastageValue( csv.GetDouble(n++) );
	SetTransferOutQty( csv.GetDouble(n++) );
	n++;	//SKIP LEGACY CLOSING QTY
	SetYTDSalesQty( csv.GetDouble(n++) );
	SetYTDSalesValue( csv.GetDouble(n++) );
	SetYTDAdjustmentQty( csv.GetDouble(n++) );
	SetYTDAdjustmentValue( csv.GetDouble(n++) );
	SetQtyOnOrder ( csv.GetDouble(n++) );
	SetMinimumLevelQty ( csv.GetDouble(n++) );
	SetReorderLevelQty( csv.GetDouble(n++) );
	n++;	//SKIP LEGACY CUSTOMER SALES QTY
	n++;	//SKIP LEGACY CUSTOMER SALES VALUE
	
	int nCount = csv.GetInt(n++);
	
	for ( int nLevel = 0; nLevel < nCount; nLevel++ )
	{
		if ( nLevel < STOCK_PRICES )
		{
			SetEposSalesQty( nLevel, csv.GetDouble( n++ ) );
			SetEposSalesValue( nLevel, csv.GetDouble( n++ ) );
		}
		else
			n += 2;
	}
}

/**********************************************************************/

void CStockLevelsCSVRecord::V4ConvertFromCSV ( CCSV& csv )
{
	int n = 2;
	SetOpeningQty ( csv.GetDouble(n++) );
	SetDeliveryQty( csv.GetDouble(n++) );
	SetDeliveryValue( csv.GetDouble(n++) );
	SetFreeQty( csv.GetDouble(n++) );
	SetTransferInQty( csv.GetDouble(n++) );
	SetRecipeSalesQty( csv.GetDouble(n++) );
	SetRecipeSalesValue( csv.GetDouble(n++) );
	SetManualSalesQty( csv.GetDouble(n++) );
	SetManualSalesValue( csv.GetDouble(n++) );
	SetImportSalesQty( csv.GetDouble(n++) );
	SetImportSalesValue( csv.GetDouble(n++) );
	SetAdjustmentQty( csv.GetDouble(n++) );
	SetAdjustmentValue( csv.GetDouble(n++) );
	SetReturnsQty( csv.GetDouble(n++) );
	SetReturnsValue( csv.GetDouble(n++) );
	SetEcrWastageQty( csv.GetDouble(n++) );
	SetEcrWastageValue( csv.GetDouble(n++) );
	SetTransferOutQty( csv.GetDouble(n++) );
	n++;	//SKIP LEGACY CLOSING QTY
	SetYTDSalesQty( csv.GetDouble(n++) );
	SetYTDSalesValue( csv.GetDouble(n++) );
	SetYTDAdjustmentQty( csv.GetDouble(n++) );
	SetYTDAdjustmentValue( csv.GetDouble(n++) );
	SetYTDEcrWastageQty( csv.GetDouble(n++) );
	SetYTDEcrWastageValue( csv.GetDouble(n++) );
	SetQtyOnOrder ( csv.GetDouble(n++) );
	SetMinimumLevelQty ( csv.GetDouble(n++) );
	SetReorderLevelQty( csv.GetDouble(n++) );
	n++;	//SKIP LEGACY CUSTOMER SALES QTY
	n++;	//SKIP LEGACY CUSTOMER SALES VALUE
	
	int nCount = csv.GetInt(n++);
	
	for ( int nLevel = 0; nLevel < nCount; nLevel++ )
	{
		if ( nLevel < STOCK_PRICES )
		{
			SetEposSalesQty( nLevel, csv.GetDouble( n++ ) );
			SetEposSalesValue( nLevel, csv.GetDouble( n++ ) );
		}
		else
			n += 2;
	}
}

/**********************************************************************/

void CStockLevelsCSVRecord::V5to6ConvertFromCSV ( CCSV& csv )
{
	int n = 2;
	SetOpeningQty ( csv.GetDouble(n++) );
	SetDeliveryQty( csv.GetDouble(n++) );
	SetDeliveryValue( csv.GetDouble(n++) );
	SetFreeQty( csv.GetDouble(n++) );
	SetTransferInQty( csv.GetDouble(n++) );
	SetRecipeSalesQty( csv.GetDouble(n++) );
	SetRecipeSalesValue( csv.GetDouble(n++) );
	SetManualSalesQty( csv.GetDouble(n++) );
	SetManualSalesValue( csv.GetDouble(n++) );
	SetImportSalesQty( csv.GetDouble(n++) );
	SetImportSalesValue( csv.GetDouble(n++) );
	SetAdjustmentQty( csv.GetDouble(n++) );
	SetAdjustmentValue( csv.GetDouble(n++) );
	SetReturnsQty( csv.GetDouble(n++) );
	SetReturnsValue( csv.GetDouble(n++) );
	SetEcrWastageQty( csv.GetDouble(n++) );
	SetEcrWastageValue( csv.GetDouble(n++) );
	SetTransferOutQty( csv.GetDouble(n++) );
	n++;	//SKIP LEGACY CLOSING QTY
	SetYTDSalesQty( csv.GetDouble(n++) );
	SetYTDSalesValue( csv.GetDouble(n++) );
	SetYTDAdjustmentQty( csv.GetDouble(n++) );
	SetYTDAdjustmentValue( csv.GetDouble(n++) );
	SetYTDEcrWastageQty( csv.GetDouble(n++) );
	SetYTDEcrWastageValue( csv.GetDouble(n++) );
	SetQtyOnOrder ( csv.GetDouble(n++) );
	SetMinimumLevelQty ( csv.GetDouble(n++) );
	SetReorderLevelQty( csv.GetDouble(n++) );
	
	int nCount = csv.GetInt(n++);
	
	for ( int nLevel = 0; nLevel < nCount; nLevel++ )
	{
		if ( nLevel < STOCK_PRICES )
		{
			SetEposSalesQty( nLevel, csv.GetDouble( n++ ) );
			SetEposSalesValue( nLevel, csv.GetDouble( n++ ) );
		}
		else
			n += 2;
	}
}

/**********************************************************************/

void CStockLevelsCSVRecord::V7to14ConvertFromCSV ( CCSV& csv, int nVer )
{
	int n = 2;
	SetOpeningQty ( csv.GetDouble(n++) );
	SetDeliveryQty( csv.GetDouble(n++) );
	SetDeliveryValue( csv.GetDouble(n++) );
	SetFreeQty( csv.GetDouble(n++) );
	SetTransferInQty( csv.GetDouble(n++) );
	SetRecipeSalesQty( csv.GetDouble(n++) );
	SetRecipeSalesValue( csv.GetDouble(n++) );
	SetManualSalesQty( csv.GetDouble(n++) );
	SetManualSalesValue( csv.GetDouble(n++) );
	SetImportSalesQty( csv.GetDouble(n++) );
	SetImportSalesValue( csv.GetDouble(n++) );
	SetAdjustmentQty( csv.GetDouble(n++) );
	SetAdjustmentValue( csv.GetDouble(n++) );
	SetReturnsQty( csv.GetDouble(n++) );
	SetReturnsValue( csv.GetDouble(n++) );
	SetEcrWastageQty( csv.GetDouble(n++) );
	SetEcrWastageValue( csv.GetDouble(n++) );
	SetTransferOutQty( csv.GetDouble(n++) );
	SetYTDSalesQty( csv.GetDouble(n++) );
	SetYTDSalesValue( csv.GetDouble(n++) );
	SetYTDAdjustmentQty( csv.GetDouble(n++) );
	SetYTDAdjustmentValue( csv.GetDouble(n++) );
	SetYTDEcrWastageQty( csv.GetDouble(n++) );
	SetYTDEcrWastageValue( csv.GetDouble(n++) );
	SetQtyOnOrder ( csv.GetDouble(n++) );
	SetMinimumLevelQty ( csv.GetDouble(n++) );

	if (nVer >= 11)
	{
		SetMaximumLevelQty(csv.GetDouble(n++));
	}

	SetReorderLevelQty( csv.GetDouble(n++) );
	
	int nCount = csv.GetInt(n++);
	
	for (int nLevel = 0; nLevel < nCount; nLevel++)
	{
		if (nLevel < STOCK_PRICES)
		{
			SetEposSalesQty(nLevel, csv.GetDouble(n++));
			SetEposSalesValue(nLevel, csv.GetDouble(n++));
		}
		else
		{
			n += 2;
		}
	}

	if (nVer >= 13)
	{
		SetActiveFlag(csv.GetBool(n++));
	}

	int nStocktakeNo = csv.GetInt(n++);
	SetStocktakeNo ( nStocktakeNo );

	if ( nStocktakeNo != 0 )
	{
		SetStocktakeDeliveryQty( csv.GetDouble(n++) );
		SetStocktakeDeliveryValue( csv.GetDouble(n++) );
		SetStocktakeFreeQty( csv.GetDouble(n++) );
		SetStocktakeTransferInQty( csv.GetDouble(n++) );
		SetStocktakeRecipeSalesQty( csv.GetDouble(n++) );
		SetStocktakeRecipeSalesValue( csv.GetDouble(n++) );
		SetStocktakeManualSalesQty( csv.GetDouble(n++) );
		SetStocktakeManualSalesValue( csv.GetDouble(n++) );
		SetStocktakeImportSalesQty( csv.GetDouble(n++) );
		SetStocktakeImportSalesValue( csv.GetDouble(n++) );
		SetStocktakeAdjustmentQty( csv.GetDouble(n++) );
		SetStocktakeAdjustmentValue( csv.GetDouble(n++) );
		SetStocktakeReturnsQty( csv.GetDouble(n++) );
		SetStocktakeReturnsValue( csv.GetDouble(n++) );
		SetStocktakeEcrWastageQty( csv.GetDouble(n++) );
		SetStocktakeEcrWastageValue( csv.GetDouble(n++) );
		SetStocktakeTransferOutQty( csv.GetDouble(n++) );
		SetStocktakeApparentQty( csv.GetDouble(n++) );
		SetStocktakeClosingQty( csv.GetDouble(n++) );

		int nCount = csv.GetInt(n++);
	
		for ( int nLevel = 0; nLevel < nCount; nLevel++ )
		{
			if ( nLevel < STOCK_PRICES )
			{
				SetStocktakeEposSalesQty( nLevel, csv.GetDouble( n++ ) );
				SetStocktakeEposSalesValue( nLevel, csv.GetDouble( n++ ) );
			}
			else
			{
				n += 2;
			}
		}
	}
}

/**********************************************************************/

void CStockLevelsCSVRecord::ConvertToCSV ( CCSV& csv )
{
	csv.Add ( m_strStockCode );
	csv.Add ( STOCKDB_VERSION );
	csv.Add ( GetOpeningQty(), 5 ); 
	csv.Add ( GetDeliveryQty(), 5 ); 
	csv.Add ( GetDeliveryValue(), 3 ); 
	csv.Add ( GetFreeQty(), 5 ); 
	csv.Add ( GetTransferInQty(), 5 ); 		 
	csv.Add ( GetRecipeSalesQty (), 5 );
	csv.Add ( GetRecipeSalesValue(), 3 );		
	csv.Add ( GetManualSalesQty(), 5 ); 
	csv.Add ( GetManualSalesValue(), 3 ); 
	csv.Add ( GetImportSalesQty(), 5 ); 
	csv.Add ( GetImportSalesValue(), 3 ); 
	csv.Add ( GetAdjustmentQty(), 5); 
	csv.Add ( GetAdjustmentValue(), 3 ); 
	csv.Add ( GetReturnsQty(), 5); 
	csv.Add ( GetReturnsValue(), 3 ); 
	csv.Add ( GetEcrWastageQty(), 5 );
	csv.Add ( GetEcrWastageValue(), 3 );
	csv.Add ( GetTransferOutQty(), 5 ); 
	csv.Add ( GetYTDSalesQty(), 5 ); 
	csv.Add ( GetYTDSalesValue(), 3 ); 
	csv.Add ( GetYTDAdjustmentQty(), 5 ); 
	csv.Add ( GetYTDAdjustmentValue(), 3 ); 		
	csv.Add ( GetYTDEcrWastageQty(), 5 ); 
	csv.Add ( GetYTDEcrWastageValue(), 3 ); 		
	csv.Add ( GetQtyOnOrder(), 3 );
	csv.Add ( GetMinimumLevelQty(), 5 ); 
	csv.Add ( GetMaximumLevelQty(), 5 ); 
	csv.Add ( GetReorderLevelQty(), 5 ); 
	csv.Add ( STOCK_PRICES );

	for ( int nLevel = 0; nLevel < STOCK_PRICES; nLevel++ )
	{
		csv.Add ( GetEposSalesQty( nLevel ), 5 ); 
		csv.Add ( GetEposSalesValue( nLevel ), 3 ); 
	}

	csv.Add( GetActiveFlag() );
	csv.Add ( GetStocktakeNo() );

	if ( GetStocktakeNo() != 0 )
	{
		csv.Add ( GetStocktakeDeliveryQty(), 5 ); 
		csv.Add ( GetStocktakeDeliveryValue(), 3 ); 
		csv.Add ( GetStocktakeFreeQty(), 5 ); 
		csv.Add ( GetStocktakeTransferInQty(), 5 ); 		 
		csv.Add ( GetStocktakeRecipeSalesQty (), 5 );
		csv.Add ( GetStocktakeRecipeSalesValue(), 3 );		
		csv.Add ( GetStocktakeManualSalesQty(), 5 ); 
		csv.Add ( GetStocktakeManualSalesValue(), 3 ); 
		csv.Add ( GetStocktakeImportSalesQty(), 5 ); 
		csv.Add ( GetStocktakeImportSalesValue(), 3 ); 
		csv.Add ( GetStocktakeAdjustmentQty(), 5); 
		csv.Add ( GetStocktakeAdjustmentValue(), 3 ); 
		csv.Add ( GetStocktakeReturnsQty(), 5); 
		csv.Add ( GetStocktakeReturnsValue(), 3 ); 
		csv.Add ( GetStocktakeEcrWastageQty(), 5 );
		csv.Add ( GetStocktakeEcrWastageValue(), 3 );
		csv.Add ( GetStocktakeTransferOutQty(), 5 );  
		csv.Add ( GetStocktakeApparentQty(), 5 );
		csv.Add ( GetStocktakeClosingQty(), 5 );

		csv.Add ( STOCK_PRICES );

		for ( int nLevel = 0; nLevel < STOCK_PRICES; nLevel++ )
		{
			csv.Add ( GetStocktakeEposSalesQty( nLevel ), 5 ); 
			csv.Add ( GetStocktakeEposSalesValue( nLevel ), 3 ); 
		}
	}
}

/**********************************************************************/

void CStockLevelsCSVRecord::SetEposSalesQty ( int nLevel, double dQty )
{
	if ( nLevel >= 0 && nLevel < STOCK_PRICES )
		m_dEposSalesQty [ nLevel ] = dQty;
}

/**********************************************************************/

void CStockLevelsCSVRecord::SetEposSalesValue ( int nLevel, double dValue )
{
	if ( nLevel >= 0 && nLevel < STOCK_PRICES )
		m_dEposSalesValue [ nLevel ] = dValue;
}

/**********************************************************************/

double CStockLevelsCSVRecord::GetEposSalesQty ( int nLevel )
{
	if ( nLevel >= 0 && nLevel < STOCK_PRICES )
		return m_dEposSalesQty [ nLevel ];
	else
		return 0.0;
}

/**********************************************************************/

double CStockLevelsCSVRecord::GetEposSalesValue ( int nLevel )
{
	if ( nLevel >= 0 && nLevel < STOCK_PRICES )
		return m_dEposSalesValue [ nLevel ];
	else
		return 0.0;
}

/**********************************************************************/

double CStockLevelsCSVRecord::GetEposSalesQty()
{
	double dBuffer = 0.0;

	for ( int nLevel = 0; nLevel < STOCK_PRICES; nLevel++ )
		dBuffer += m_dEposSalesQty [ nLevel ];

	return dBuffer;
}

/**********************************************************************/

double CStockLevelsCSVRecord::GetEposSalesValue()
{
	double dBuffer = 0.0;

	for ( int nLevel = 0; nLevel < STOCK_PRICES; nLevel++ )
		dBuffer += m_dEposSalesValue [ nLevel ];

	return dBuffer;
}

/**********************************************************************/

void CStockLevelsCSVRecord::SetStocktakeEposSalesQty ( int nLevel, double dQty )
{
	if ( nLevel >= 0 && nLevel < STOCK_PRICES )
		m_dStocktakeEposSalesQty [ nLevel ] = dQty;
}

/**********************************************************************/

void CStockLevelsCSVRecord::SetStocktakeEposSalesValue ( int nLevel, double dValue )
{
	if ( nLevel >= 0 && nLevel < STOCK_PRICES )
		m_dStocktakeEposSalesValue [ nLevel ] = dValue;
}

/**********************************************************************/

double CStockLevelsCSVRecord::GetStocktakeEposSalesQty ( int nLevel )
{
	if ( nLevel >= 0 && nLevel < STOCK_PRICES )
		return m_dStocktakeEposSalesQty [ nLevel ];
	else
		return 0.0;
}

/**********************************************************************/

double CStockLevelsCSVRecord::GetStocktakeEposSalesValue ( int nLevel )
{
	if ( nLevel >= 0 && nLevel < STOCK_PRICES )
		return m_dStocktakeEposSalesValue [ nLevel ];
	else
		return 0.0;
}

/**********************************************************************/

double CStockLevelsCSVRecord::GetStocktakeEposSalesQty()
{
	double dBuffer = 0.0;

	for ( int nLevel = 0; nLevel < STOCK_PRICES; nLevel++ )
		dBuffer += m_dStocktakeEposSalesQty [ nLevel ];

	return dBuffer;
}

/**********************************************************************/

double CStockLevelsCSVRecord::GetStocktakeEposSalesValue()
{
	double dBuffer = 0.0;

	for ( int nLevel = 0; nLevel < STOCK_PRICES; nLevel++ )
		dBuffer += m_dStocktakeEposSalesValue [ nLevel ];

	return dBuffer;
}

/**********************************************************************/

CStockLevelsCSVRecord& CStockLevelsCSVRecord::operator= ( const CStockLevelsCSVRecord& source )
{
	m_strStockCode = source.GetStockCode();
	m_dOpeningQty = source.GetOpeningQty();
	m_dDeliveryQty = source.GetDeliveryQty();
	m_dDeliveryValue = source.GetDeliveryValue();
	m_dFreeQty = source.GetFreeQty();
	m_dTransferInQty = source.GetTransferInQty();
	m_dRecipeSalesQty = source.GetRecipeSalesQty();
	m_dRecipeSalesValue = source.GetRecipeSalesValue();
	m_dManualSalesQty = source.GetManualSalesQty();
	m_dManualSalesValue = source.GetManualSalesValue();
	m_dImportSalesQty = source.GetImportSalesQty();
	m_dImportSalesValue = source.GetImportSalesValue();
	m_dAdjustmentQty = source.GetAdjustmentQty();
	m_dAdjustmentValue = source.GetAdjustmentValue();
	m_dReturnsQty = source.GetReturnsQty();
	m_dReturnsValue = source.GetReturnsValue();
	m_dEcrWastageQty = source.GetEcrWastageQty();
	m_dEcrWastageValue = source.GetEcrWastageValue();
	m_dTransferOutQty = source.GetTransferOutQty();
	m_dYTDSalesQty = source.GetYTDSalesQty();
	m_dYTDSalesValue = source.GetYTDSalesValue();
	m_dYTDAdjustmentQty = source.GetYTDAdjustmentQty(); 
	m_dYTDAdjustmentValue = source.GetYTDAdjustmentValue();
	m_dYTDEcrWastageQty = source.GetYTDEcrWastageQty(); 
	m_dYTDEcrWastageValue = source.GetYTDEcrWastageValue();
	m_dQtyOnOrder = source.GetQtyOnOrder();
	m_dMinimumLevelQty = source.GetMinimumLevelQty();
	m_dMaximumLevelQty = source.GetMaximumLevelQty();
	m_dReorderLevelQty = source.GetReorderLevelQty();

	m_dStocktakeDeliveryQty = source.GetStocktakeDeliveryQty();
	m_dStocktakeDeliveryValue = source.GetStocktakeDeliveryValue();
	m_dStocktakeFreeQty = source.GetStocktakeFreeQty();
	m_dStocktakeTransferInQty = source.GetStocktakeTransferInQty();
	m_dStocktakeRecipeSalesQty = source.GetStocktakeRecipeSalesQty();
	m_dStocktakeRecipeSalesValue = source.GetStocktakeRecipeSalesValue();
	m_dStocktakeManualSalesQty = source.GetStocktakeManualSalesQty();
	m_dStocktakeManualSalesValue = source.GetStocktakeManualSalesValue();
	m_dStocktakeImportSalesQty = source.GetStocktakeImportSalesQty();
	m_dStocktakeImportSalesValue = source.GetStocktakeImportSalesValue();
	m_dStocktakeAdjustmentQty = source.GetStocktakeAdjustmentQty();
	m_dStocktakeAdjustmentValue = source.GetStocktakeAdjustmentValue();
	m_dStocktakeReturnsQty = source.GetStocktakeReturnsQty();
	m_dStocktakeReturnsValue = source.GetStocktakeReturnsValue();
	m_dStocktakeEcrWastageQty = source.GetStocktakeEcrWastageQty();
	m_dStocktakeEcrWastageValue = source.GetStocktakeEcrWastageValue();
	m_dStocktakeTransferOutQty = source.GetStocktakeTransferOutQty();
	m_dStocktakeApparentQty = source.GetStocktakeApparentQty();
	m_dStocktakeClosingQty = source.GetStocktakeClosingQty();
	
	for ( int nLevel = 0; nLevel < STOCK_PRICES; nLevel++ )
	{
		m_dEposSalesQty [ nLevel ] = source.m_dEposSalesQty [ nLevel ];
		m_dEposSalesValue [ nLevel ] = source.m_dEposSalesValue [ nLevel ];
		m_dStocktakeEposSalesQty [ nLevel ] = source.m_dStocktakeEposSalesQty [ nLevel ];
		m_dStocktakeEposSalesValue [ nLevel ] = source.m_dStocktakeEposSalesValue [ nLevel ];
	}
	
	return *this;
}

/**********************************************************************/

CStockLevelsCSVRecord& CStockLevelsCSVRecord::operator+ ( const CStockLevelsCSVRecord& source )
{
	m_dOpeningQty += source.GetOpeningQty();
	m_dDeliveryQty += source.GetDeliveryQty();
	m_dDeliveryValue += source.GetDeliveryValue();
	m_dFreeQty += source.GetFreeQty();
	m_dTransferInQty += source.GetTransferInQty();
	m_dRecipeSalesQty += source.GetRecipeSalesQty();
	m_dRecipeSalesValue += source.GetRecipeSalesValue();
	m_dManualSalesQty += source.GetManualSalesQty();
	m_dManualSalesValue += source.GetManualSalesValue();
	m_dImportSalesQty += source.GetImportSalesQty();
	m_dImportSalesValue += source.GetImportSalesValue();
	m_dAdjustmentQty += source.GetAdjustmentQty();
	m_dAdjustmentValue += source.GetAdjustmentValue();
	m_dReturnsQty += source.GetReturnsQty();
	m_dReturnsValue += source.GetReturnsValue();
	m_dEcrWastageQty += source.GetEcrWastageQty();
	m_dEcrWastageValue += source.GetEcrWastageValue();
	m_dTransferOutQty += source.GetTransferOutQty();
	m_dYTDSalesQty += source.GetYTDSalesQty();
	m_dYTDSalesValue += source.GetYTDSalesValue();
	m_dYTDAdjustmentQty += source.GetYTDAdjustmentQty(); 
	m_dYTDAdjustmentValue += source.GetYTDAdjustmentValue();
	m_dYTDEcrWastageQty += source.GetYTDEcrWastageQty(); 
	m_dYTDEcrWastageValue += source.GetYTDEcrWastageValue();
	m_dQtyOnOrder += source.GetQtyOnOrder();
	m_dMinimumLevelQty += source.GetMinimumLevelQty();
	m_dMaximumLevelQty += source.GetMaximumLevelQty();
	m_dReorderLevelQty += source.GetReorderLevelQty();

	m_dStocktakeDeliveryQty += source.GetStocktakeDeliveryQty();
	m_dStocktakeDeliveryValue += source.GetStocktakeDeliveryValue();
	m_dStocktakeFreeQty += source.GetStocktakeFreeQty();
	m_dStocktakeTransferInQty += source.GetStocktakeTransferInQty();
	m_dStocktakeRecipeSalesQty += source.GetStocktakeRecipeSalesQty();
	m_dStocktakeRecipeSalesValue += source.GetStocktakeRecipeSalesValue();
	m_dStocktakeManualSalesQty += source.GetStocktakeManualSalesQty();
	m_dStocktakeManualSalesValue += source.GetStocktakeManualSalesValue();
	m_dStocktakeImportSalesQty += source.GetStocktakeImportSalesQty();
	m_dStocktakeImportSalesValue += source.GetStocktakeImportSalesValue();
	m_dStocktakeAdjustmentQty += source.GetStocktakeAdjustmentQty();
	m_dStocktakeAdjustmentValue += source.GetStocktakeAdjustmentValue();
	m_dStocktakeReturnsQty += source.GetStocktakeReturnsQty();
	m_dStocktakeReturnsValue += source.GetStocktakeReturnsValue();
	m_dStocktakeEcrWastageQty += source.GetStocktakeEcrWastageQty();
	m_dStocktakeEcrWastageValue += source.GetStocktakeEcrWastageValue();
	m_dStocktakeTransferOutQty += source.GetStocktakeTransferOutQty();
	m_dStocktakeApparentQty += source.GetStocktakeApparentQty();
	m_dStocktakeClosingQty += source.GetStocktakeClosingQty();
	
	for ( int nLevel = 0; nLevel < STOCK_PRICES; nLevel++ )
	{
		m_dEposSalesQty [ nLevel ] += source.m_dEposSalesQty [ nLevel ];
		m_dEposSalesValue [ nLevel ] += source.m_dEposSalesValue [ nLevel ];
		m_dStocktakeEposSalesQty [ nLevel ] += source.m_dStocktakeEposSalesQty [ nLevel ];
		m_dStocktakeEposSalesValue [ nLevel ] += source.m_dStocktakeEposSalesValue [ nLevel ];
	}
	
	return *this;
}

/**********************************************************************/

bool CStockLevelsCSVRecord::HasStockMovements()
{
	if ( GetTotalSalesQty() != 0.0 )
		return TRUE;

	if ( m_dAdjustmentQty != 0.0 || m_dDeliveryQty != 0.0 )
		return TRUE;
		
	if ( m_dFreeQty != 0.0 || m_dOpeningQty != 0.0 )
		return TRUE;

	if ( m_dReturnsQty != 0.0 || m_dTransferInQty != 0.0 || m_dTransferOutQty != 0.0 )
		return TRUE;

	if ( GetPendingStocktakeQty() != 0.0 )
		return TRUE;

	return FALSE;
}

/**********************************************************************/

double CStockLevelsCSVRecord::GetTotalSalesQty()
{
	double dBuffer = 0.0;

	for ( int nLevel = 0; nLevel < STOCK_PRICES; nLevel++ )
		dBuffer += m_dEposSalesQty [ nLevel ];

	dBuffer += m_dManualSalesQty;
	dBuffer += m_dImportSalesQty;
	dBuffer += m_dRecipeSalesQty;

	return dBuffer;
}

/**********************************************************************/

double CStockLevelsCSVRecord::GetTotalSalesValue()
{
	double dBuffer = 0.0;

	for ( int nLevel = 0; nLevel < STOCK_PRICES; nLevel++ )
		dBuffer += m_dEposSalesValue [ nLevel ];

	dBuffer += m_dManualSalesValue;
	dBuffer += m_dImportSalesValue;
	dBuffer += m_dRecipeSalesValue;

	return dBuffer;
}

/**********************************************************************/

double CStockLevelsCSVRecord::GetPendingStocktakeQty()
{
	return m_dStocktakeApparentQty - m_dStocktakeClosingQty;
}

/**********************************************************************/

double CStockLevelsCSVRecord::GetApparentQty()
{
	double dBuffer = m_dOpeningQty;
	dBuffer += m_dDeliveryQty;
	dBuffer += m_dFreeQty;
	dBuffer += m_dTransferInQty;
	dBuffer -= m_dReturnsQty;
	dBuffer -= GetTotalSalesQty();
	dBuffer -= m_dAdjustmentQty;
	dBuffer -= GetAutoWastageQty();	
	dBuffer -= GetEcrWastageQty();
	dBuffer -= m_dTransferOutQty;
	dBuffer -= GetPendingStocktakeQty();
	return dBuffer;
}

/**********************************************************************/

double CStockLevelsCSVRecord::GetTotalGoodsInQty()
{
	double dBuffer = m_dDeliveryQty;
	dBuffer += m_dFreeQty;
	dBuffer += m_dTransferInQty;
	return dBuffer;
}

/**********************************************************************/

double CStockLevelsCSVRecord::GetAutoWastageQty()
{
	double dWasteQty = 0.0;

	double dNonWastePercent = 100 - m_dAutoWastagePercent;
	double dSalesQty = GetTotalSalesQty();

	if ( dNonWastePercent != 0.0 )
		dWasteQty = ( dSalesQty * m_dAutoWastagePercent ) / dNonWastePercent;
	
	return dWasteQty;
}

/**********************************************************************/

double CStockLevelsCSVRecord::GetYTDAutoWastageQty()
{
	double dWasteQty = 0.0;

	double dNonWastePercent = 100 - m_dAutoWastagePercent;
	double dSalesQty = GetYTDSalesQty();

	if ( dNonWastePercent != 0.0 )
		dWasteQty = ( dSalesQty * m_dAutoWastagePercent ) / dNonWastePercent;
	
	return dWasteQty;
}

/**********************************************************************/

void CStockLevelsCSVRecord::AddEposSales( int nLevel, double dQty, double dValue )
{
	if ( nLevel >= 0 && nLevel < STOCK_PRICES )
	{
		m_dEposSalesQty [ nLevel ] += dQty;
		m_dEposSalesValue [ nLevel ] += dValue;
	}
}

/**********************************************************************/

void CStockLevelsCSVRecord::AddManualSales ( double dQty, double dValue, bool bStocktake )
{
	m_dManualSalesQty += dQty;
	m_dManualSalesValue += dValue;

	if ( TRUE == bStocktake )
	{
		m_dStocktakeManualSalesQty += dQty;
		m_dStocktakeManualSalesValue += dValue;
		m_dStocktakeApparentQty -= dQty;
	}
}

/**********************************************************************/

void CStockLevelsCSVRecord::AddImportSales ( double dQty, double dValue, bool bStocktake )
{
	m_dImportSalesQty += dQty;
	m_dImportSalesValue += dValue;

	if ( TRUE == bStocktake )
	{
		m_dStocktakeImportSalesQty += dQty;
		m_dStocktakeImportSalesValue += dValue;
		m_dStocktakeApparentQty -= dQty;
	}
}

/**********************************************************************/

void CStockLevelsCSVRecord::AddRecipeSales ( double dQty, double dValue, bool bStocktake )
{
	m_dRecipeSalesQty += dQty;
	m_dRecipeSalesValue += dValue;

	if ( TRUE == bStocktake )
	{
		m_dStocktakeRecipeSalesQty += dQty;
		m_dStocktakeRecipeSalesValue += dValue;
		m_dStocktakeApparentQty -= dQty;
	}
}

/**********************************************************************/

void CStockLevelsCSVRecord::AddDelivery ( double dQty, double dValue, bool bStocktake )
{
	m_dDeliveryQty += dQty;
	m_dDeliveryValue += dValue;

	if ( TRUE == bStocktake )
	{
		m_dStocktakeDeliveryQty += dQty;
		m_dStocktakeDeliveryValue += dValue;
		m_dStocktakeApparentQty += dQty;
	}
}

/**********************************************************************/

void CStockLevelsCSVRecord::AddFreeStock ( double dQty, bool bStocktake )
{
	m_dFreeQty += dQty;

	if ( TRUE == bStocktake )
	{
		m_dStocktakeFreeQty += dQty;
		m_dStocktakeApparentQty += dQty;
	}
}

/**********************************************************************/

void CStockLevelsCSVRecord::AddReturns ( double dQty, double dValue, bool bStocktake )
{
	m_dReturnsQty += dQty;
	m_dReturnsValue += dValue;

	if ( TRUE == bStocktake )
	{
		m_dStocktakeReturnsQty += dQty;
		m_dStocktakeReturnsValue += dValue;
		m_dStocktakeApparentQty -= dQty;
	}
}

/**********************************************************************/

void CStockLevelsCSVRecord::AddEcrWastage ( double dQty, double dValue )
{
	m_dEcrWastageQty += dQty;
	m_dEcrWastageValue += dValue;
}

/**********************************************************************/

void CStockLevelsCSVRecord::AddAdjustment ( double dQty, double dValue, bool bStocktake )
{
	m_dAdjustmentQty += dQty;
	m_dAdjustmentValue += dValue;

	if ( TRUE == bStocktake )
	{
		m_dStocktakeAdjustmentQty += dQty;
		m_dStocktakeAdjustmentValue += dValue;
		m_dStocktakeApparentQty -= dQty;
	}
}

/**********************************************************************/

void CStockLevelsCSVRecord::AddOrder( double dQty )
{
	m_dQtyOnOrder += dQty;
}

/**********************************************************************/

void CStockLevelsCSVRecord::BeginStocktake( int nStocktakeNo, bool bClosingStockZero )
{
	SetStocktakeNo( nStocktakeNo);
	SetStocktakeDeliveryQty( GetDeliveryQty() );
	SetStocktakeDeliveryValue( GetDeliveryValue() );
	SetStocktakeFreeQty( GetFreeQty() );
	SetStocktakeTransferInQty( GetTransferInQty() );
	SetStocktakeRecipeSalesQty( GetRecipeSalesQty() );
	SetStocktakeRecipeSalesValue( GetRecipeSalesValue() );
	SetStocktakeManualSalesQty( GetManualSalesQty() );
	SetStocktakeManualSalesValue( GetManualSalesValue() );
	SetStocktakeImportSalesQty( GetImportSalesQty() );
	SetStocktakeImportSalesValue( GetImportSalesValue() );
	SetStocktakeAdjustmentQty( GetAdjustmentQty() );
	SetStocktakeAdjustmentValue( GetAdjustmentValue() );
	SetStocktakeReturnsQty( GetReturnsQty() );
	SetStocktakeReturnsValue( GetReturnsValue() );
	SetStocktakeEcrWastageQty( GetEcrWastageQty() );
	SetStocktakeEcrWastageValue( GetEcrWastageValue() );
	SetStocktakeTransferOutQty( GetTransferOutQty() );

	for ( int nLevel = 0; nLevel < STOCK_PRICES; nLevel++ )
	{
		SetStocktakeEposSalesQty( nLevel, GetEposSalesQty( nLevel ) );
		SetStocktakeEposSalesValue( nLevel, GetEposSalesValue( nLevel ) );
	}

	//PREVENT ANY PREVIOUS STOCK TAKE FROM CONFUSING THE APPARENT STOCK LEVEL
	SetStocktakeApparentQty( 0.0 );
	SetStocktakeClosingQty( 0.0 );

	double dApparentQty = GetApparentQty();
	SetStocktakeApparentQty( dApparentQty );
	
	if (bClosingStockZero == FALSE)
	{
		SetStocktakeClosingQty(dApparentQty);
	}
	else
	{
		SetStocktakeClosingQty(0.0);
	}
}

/**********************************************************************/

void CStockLevelsCSVRecord::AbandonStocktake( int nStocktakeNo )
{
	//USE STOCK TAKE ZERO TO ABANDON ALL STOCK TAKES
	if ( ( nStocktakeNo == 0 ) || ( nStocktakeNo == m_nStocktakeNo ) )
	{
		SetStocktakeNo(0);
		SetStocktakeApparentQty(0.0);
		SetStocktakeClosingQty(0.0);
	}
}

/**********************************************************************/

void CStockLevelsCSVRecord::FinaliseStocktake( int nStocktakeNo, int nType, double dSURetail )
{
	if ( nStocktakeNo != m_nStocktakeNo )
		return;

	switch ( nType )
	{
	//ADJUST APPARENT STOCK
	case STOCKTAKE_TYPE_ADJUST:
		{
			double dChange = m_dStocktakeApparentQty - m_dStocktakeClosingQty;
			AddAdjustment( dChange, dChange * dSURetail, FALSE );
			m_nStocktakeNo = 0;
		}
		break;
	
	//PERIOD END FROM STOCK TAKE DATE
	case STOCKTAKE_TYPE_PERIOD:
	case STOCKTAKE_TYPE_YTD:
		{
			if ( nType != STOCKTAKE_TYPE_YTD )
			{
				m_dYTDSalesQty += GetStocktakeTotalSalesQty();
				m_dYTDSalesValue += GetStocktakeTotalSalesValue();
				m_dYTDAdjustmentQty += m_dStocktakeAdjustmentQty;
				m_dYTDAdjustmentValue += m_dStocktakeAdjustmentValue;
				m_dYTDEcrWastageQty += m_dStocktakeEcrWastageQty;
				m_dYTDEcrWastageValue += m_dStocktakeEcrWastageValue;
			}
			else
			{
				m_dYTDSalesQty = 0.0;
				m_dYTDSalesValue = 0.0;
				m_dYTDAdjustmentQty = 0.0;
				m_dYTDAdjustmentValue = 0.0;
				m_dYTDEcrWastageQty = 0.0;
				m_dYTDEcrWastageValue = 0.0;
			}

			m_dOpeningQty = m_dStocktakeClosingQty;
			m_dDeliveryQty -= m_dStocktakeDeliveryQty;
			m_dDeliveryValue -= m_dStocktakeDeliveryValue;
			m_dFreeQty -= m_dStocktakeFreeQty;
			m_dTransferInQty -= m_dStocktakeTransferInQty;
			m_dRecipeSalesQty -= m_dStocktakeRecipeSalesQty;
			m_dRecipeSalesValue -= m_dStocktakeRecipeSalesValue;
			m_dManualSalesQty -= m_dStocktakeManualSalesQty;
			m_dManualSalesValue -= m_dStocktakeManualSalesValue;
			m_dImportSalesQty -= m_dStocktakeImportSalesQty;
			m_dImportSalesValue -= m_dStocktakeImportSalesValue;
			m_dAdjustmentQty -= m_dStocktakeAdjustmentQty;
			m_dAdjustmentValue -= m_dStocktakeAdjustmentValue;
			m_dReturnsQty -= m_dStocktakeReturnsQty;
			m_dReturnsValue -= m_dStocktakeReturnsValue;
			m_dEcrWastageQty -= m_dStocktakeEcrWastageQty;
			m_dEcrWastageValue -= m_dStocktakeEcrWastageValue;
			m_dTransferOutQty -= m_dStocktakeTransferOutQty;
		
			for ( int nLevel = 0; nLevel < STOCK_PRICES; nLevel++ )
			{
				m_dEposSalesQty[ nLevel ] -= m_dStocktakeEposSalesQty[ nLevel ];
				m_dEposSalesValue[ nLevel ] -= m_dStocktakeEposSalesValue[ nLevel ];
			}

			m_nStocktakeNo = 0;
			m_dStocktakeApparentQty = 0.0;
			m_dStocktakeClosingQty = 0.0;
		}
		break;
	}
}

/**********************************************************************/

double CStockLevelsCSVRecord::GetStocktakeTotalGoodsInQty()
{
	double dBuffer = m_dStocktakeDeliveryQty;
	dBuffer += m_dStocktakeFreeQty;
	dBuffer += m_dStocktakeTransferInQty;
	return dBuffer;
}

/**********************************************************************/

double CStockLevelsCSVRecord::GetStocktakeTotalSalesQty()
{
	double dBuffer = 0.0;

	for ( int nLevel = 0; nLevel < STOCK_PRICES; nLevel++ )
		dBuffer += m_dStocktakeEposSalesQty [ nLevel ];

	dBuffer += m_dStocktakeManualSalesQty;
	dBuffer += m_dStocktakeImportSalesQty;
	dBuffer += m_dStocktakeRecipeSalesQty;

	return dBuffer;
}

/**********************************************************************/

double CStockLevelsCSVRecord::GetStocktakeTotalSalesValue()
{
	double dBuffer = 0.0;

	for ( int nLevel = 0; nLevel < STOCK_PRICES; nLevel++ )
		dBuffer += m_dStocktakeEposSalesValue [ nLevel ];

	dBuffer += m_dStocktakeManualSalesValue;
	dBuffer += m_dStocktakeImportSalesValue;
	dBuffer += m_dStocktakeRecipeSalesValue;

	return dBuffer;
}

/**********************************************************************/

double CStockLevelsCSVRecord::GetStocktakeAutoWastageQty()
{
	double dWasteQty = 0.0;

	double dNonWastePercent = 100 - m_dAutoWastagePercent;
	double dSalesQty = GetStocktakeTotalSalesQty();

	if ( dNonWastePercent != 0.0 )
		dWasteQty = ( dSalesQty * m_dAutoWastagePercent ) / dNonWastePercent;
	
	return dWasteQty;
}

/**********************************************************************/

double CStockLevelsCSVRecord::EstimateStocktakeSalesQty()
{
	double dBuffer = m_dOpeningQty;
	dBuffer += GetStocktakeTotalGoodsInQty();
	dBuffer -= m_dStocktakeReturnsQty;
	dBuffer -= m_dStocktakeTransferOutQty;
	dBuffer -= m_dStocktakeAdjustmentQty;
	dBuffer -= m_dStocktakeEcrWastageQty;
	dBuffer -= m_dStocktakeClosingQty;
	return dBuffer;
}

/**********************************************************************/
