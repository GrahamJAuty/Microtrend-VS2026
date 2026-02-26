/**********************************************************************/
#include "PriceHelpers.h"
#include "..\CommonStockTray\StockLevelsCSVArray.h"
/**********************************************************************/
#include "TransferDetailCSVArray.h"
/**********************************************************************/

CTransferDetailCSVRecord::CTransferDetailCSVRecord()
{
	ClearRecord();
}

/**********************************************************************/

CTransferDetailCSVRecord::~CTransferDetailCSVRecord()
{
}

/**********************************************************************/

void CTransferDetailCSVRecord::ClearRecord()
{
	m_strStockCode = "";
	Reset();
}

/**********************************************************************/

void CTransferDetailCSVRecord::Reset()
{
	m_arrayDetail.Reset();
}

/**********************************************************************/

void CTransferDetailCSVRecord::ConvertFromCSV ( CCSV& csv )
{
	ClearRecord();

	SetStockCode ( csv.GetString(0) );
	switch ( csv.GetInt(1) )
	{
	case 9: V9ConvertFromCSV( csv );	break;
	}
}

/**********************************************************************/

void CTransferDetailCSVRecord::V9ConvertFromCSV ( CCSV& csv )
{
	int n = 2;
	
	int nCount = csv.GetInt(n++);

	for ( int x = 0; x < nCount; x++ )
	{
		CTransferDetailInfo info;
		info.m_nSpNo = csv.GetInt(n++);
		info.m_dTransferInQty = csv.GetInt(n++);
		info.m_dTransferOutQty = csv.GetInt(n++);
		info.m_dStocktakeTransferInQty = csv.GetInt(n++);
		info.m_dStocktakeTransferOutQty = csv.GetInt(n++);
		AddTransferDetailInfo(info);
	}
}

/**********************************************************************/

void CTransferDetailCSVRecord::ConvertToCSV ( CCSV& csv )
{
	csv.Add ( m_strStockCode );
	csv.Add ( TRANSFERDB_VERSION );
	AddTransferDetailToCSVLine( csv );
}

/**********************************************************************/

CTransferDetailCSVRecord& CTransferDetailCSVRecord::operator= ( CTransferDetailCSVRecord& source )
{
	m_strStockCode = source.GetStockCode();
	
	m_arrayDetail.Reset();
	for ( int x = 0; x < source.GetSpCount(); x++ )
	{
		CTransferDetailInfo info;
		source.GetTransferDetailInfo( x, info );
		AddTransferDetailInfo( info );
	}

	return *this;
}

/**********************************************************************/

CTransferDetailCSVRecord& CTransferDetailCSVRecord::operator+ ( CTransferDetailCSVRecord& source )
{
	for ( int x = 0; x < source.GetSpCount(); x++ )
	{
		CTransferDetailInfo info;
		source.GetTransferDetailInfo( x, info );
		AddTransferDetailInfo( info );
	}

	return *this;
}

/**********************************************************************/

void CTransferDetailCSVRecord::BeginStocktake()
{
	for ( int nIdx = 0; nIdx < GetSpCount(); nIdx++ )
	{
		CTransferDetailInfo info;
		GetTransferDetailInfo( nIdx, info );
		info.m_dStocktakeTransferInQty = info.m_dTransferInQty;
		info.m_dStocktakeTransferOutQty = info.m_dTransferOutQty;
		SetTransferDetailInfo( nIdx, info );
	}
}

/**********************************************************************/

void CTransferDetailCSVRecord::AbandonStocktake()
{
	for ( int nIdx = 0; nIdx < GetSpCount(); nIdx++ )
	{
		CTransferDetailInfo info;
		GetTransferDetailInfo( nIdx, info );
		info.m_dStocktakeTransferInQty = 0.0;
		info.m_dStocktakeTransferOutQty = 0.0;
		SetTransferDetailInfo( nIdx, info );
	}	
}

/**********************************************************************/

void CTransferDetailCSVRecord::FinaliseStocktake( int nType )
{
	switch ( nType )
	{
	//ADJUST APPARENT STOCK
	case STOCKTAKE_TYPE_ADJUST:
		{
			for ( int nIdx = GetSpCount() - 1; nIdx >= 0; nIdx-- )
			{
				CTransferDetailInfo info;
				GetTransferDetailInfo( nIdx, info );
				info.m_dStocktakeTransferInQty = 0.0;
				info.m_dStocktakeTransferOutQty = 0.0;
				
				if ( IsZero( info ) == TRUE )
					RemoveTransferDetailInfo( nIdx );
				else
					SetTransferDetailInfo( nIdx, info );
			}
		}
		break;
	
	//PERIOD END FROM STOCK TAKE DATE
	case STOCKTAKE_TYPE_PERIOD:
	case STOCKTAKE_TYPE_YTD:
		{
			for ( int nIdx = GetSpCount() - 1; nIdx >= 0; nIdx-- )
			{
				CTransferDetailInfo info;
				GetTransferDetailInfo( nIdx, info );
				info.m_dTransferInQty -= info.m_dStocktakeTransferInQty;
				info.m_dTransferOutQty -= info.m_dStocktakeTransferOutQty;
				info.m_dStocktakeTransferInQty = 0.0;
				info.m_dStocktakeTransferOutQty = 0.0;
				
				if ( IsZero( info ) == TRUE )
					RemoveTransferDetailInfo( nIdx );
				else
					SetTransferDetailInfo( nIdx, info );
			}
		}
		break;
	}
}

/**********************************************************************/

bool CTransferDetailCSVRecord::IsZero( CTransferDetailInfo& info )
{
	if ( CPriceHelpers::CompareDoubles( info.m_dTransferInQty, 0.0, 5 ) != 0 )
		return FALSE;

	if ( CPriceHelpers::CompareDoubles( info.m_dTransferOutQty, 0.0, 5 ) != 0 )
		return FALSE;

	if ( CPriceHelpers::CompareDoubles( info.m_dStocktakeTransferInQty, 0.0, 5 ) != 0 )
		return FALSE;

	if ( CPriceHelpers::CompareDoubles( info.m_dStocktakeTransferOutQty, 0.0, 5 ) != 0 )
		return FALSE;

	return TRUE;
}

/**********************************************************************/
