/**********************************************************************/
 
/**********************************************************************/
#include <math.h>
/**********************************************************************/
#include "DatabaseCSVArray.h"
//#include "DataManager.h"
#include "NodeTypes.h"
#include "Password.h"
#include "PriceHelpers.h"
#include "StockOptionsIni.h"
#include "StockpointCSVArray.h"
#include "SysInfo.h"
/**********************************************************************/
#include "ProcessedOrderPreviewDatabase.h"
/**********************************************************************/
static const int ORDER_START = 11;		//index of first order field
/**********************************************************************/

CProcessedOrderPreviewCSVRecord::CProcessedOrderPreviewCSVRecord()
{
	m_nStockIdx = 0;
	m_nStockSuppIdx = 0;
	m_nSupplierIdx = -1;
	m_nDUItems = 1;
	m_dDUItemSize = 1.0;
	m_nDUItemsRequired = 0;
	m_nDURequired = 0;
	m_dSUTotal = 0.0;
	m_dTotalCost = 0.0;
	m_dSUCost = 0.0;
	m_nDUItemSurplus = 0;

	m_dSUQtyArray.SetSize ( dbStockpoint.GetSize(), 10 );
	for ( int nSpIdx = 0; nSpIdx < GetStockpointCount(); nSpIdx++ )
		m_dSUQtyArray[nSpIdx] = 0.0;
	
}

/**********************************************************************/

CProcessedOrderPreviewCSVRecord::~CProcessedOrderPreviewCSVRecord()
{	
}

/**********************************************************************/

void CProcessedOrderPreviewCSVRecord::ClearOrders()
{
	for ( int nSpIdx = 0; nSpIdx < m_dSUQtyArray.GetSize(); nSpIdx++ )
		m_dSUQtyArray[nSpIdx] = 0.0;
}

/**********************************************************************/

void CProcessedOrderPreviewCSVRecord::ConvertToCSV ( CCSV& csv )
{
	csv.Add ( m_nStockIdx );
	csv.Add ( m_nStockSuppIdx );
	csv.Add ( m_nSupplierIdx );
	csv.Add ( m_nDUItems );
	csv.Add ( m_dDUItemSize, 3 );
	csv.Add ( m_nDUItemsRequired );
	csv.Add ( m_nDURequired );
	csv.Add ( m_dSUTotal, SysInfo.GetDPValue() );
	csv.Add ( m_dTotalCost, SysInfo.GetDPValue() );
	csv.Add ( m_dSUCost, SysInfo.GetDPValue() );
	csv.Add ( m_nDUItemSurplus );
	
	int nSize = GetStockpointCount();
	csv.Add ( nSize );

	for ( int nSpIdx = 0; nSpIdx < nSize; nSpIdx++ )
		csv.Add ( m_dSUQtyArray[ nSpIdx ], 3 );
}

/**********************************************************************/

void CProcessedOrderPreviewCSVRecord::ConvertFromCSV ( CCSV& csv )
{
	ClearOrders();
	
	m_nStockIdx = csv.GetInt(0);
	m_nStockSuppIdx = csv.GetInt(1);
	m_nSupplierIdx = csv.GetInt(2);
	m_nDUItems = csv.GetInt(3);
	m_dDUItemSize = csv.GetDouble(4);
	m_nDUItemsRequired = csv.GetInt(5);
	m_nDURequired = csv.GetInt(6);
	m_dSUTotal = csv.GetDouble(7);
	m_dTotalCost = csv.GetDouble(8);
	m_dSUCost = csv.GetDouble(9);
	m_nDUItemSurplus = csv.GetInt(10);

	int nOffset = ORDER_START;
	int nCount = csv.GetInt ( nOffset++ );

	for ( int nIndex = 0; ( nIndex < nCount ) && ( nIndex < GetStockpointCount() ); nIndex++ )
		m_dSUQtyArray[ nIndex ] = csv.GetDouble( nOffset++ );
}

/**********************************************************************/

bool CProcessedOrderPreviewCSVRecord::HasOrder( CLocationSelectorMiniLookup& LocSelMiniLookup )
{
	for ( int nSpIdx = 0; nSpIdx < GetStockpointCount(); nSpIdx++ )
	{
		if ( CProcessedOrderPreviewCSVArray::IsStockpointIncluded( nSpIdx, LocSelMiniLookup, FALSE ) )
			if ( GetSURequired( nSpIdx ) > 0.0 )
				return TRUE;
	}
	return FALSE;
}

/**********************************************************************/

double CProcessedOrderPreviewCSVRecord::GetSURequired( int nSpIdx )
{
	if ( nSpIdx < 0 || nSpIdx >= GetStockpointCount() )
		return 0.0;

	return m_dSUQtyArray[ nSpIdx ];
}

/**********************************************************************/

int CProcessedOrderPreviewCSVRecord::CalculateDUItemRequired( int nSpIdx )
{
	double dSUReq = GetSURequired( nSpIdx );
	if ( dSUReq <= 0.0 || m_dDUItemSize <= 0.0 )
		return 0;

	int nDUItems = int ( floor ( dSUReq / m_dDUItemSize ) );
	if ( CPriceHelpers::CompareDoubles ( m_dDUItemSize * nDUItems, dSUReq, 3 ) == -1 )
		nDUItems++;

	return nDUItems;
}

/**********************************************************************/

int CProcessedOrderPreviewCSVRecord::CalculateDURequired( int nSpIdx )
{
	double dSUReq = GetSURequired( nSpIdx );
	if ( dSUReq <= 0.0 )
		return 0;

	double dDUYield = m_dDUItemSize * m_nDUItems;
	if ( dDUYield <= 0.0 )
		return 0;

	int nDU = int ( floor ( dSUReq / dDUYield ) );
	if ( CPriceHelpers::CompareDoubles( dDUYield * nDU, dSUReq, 3 ) == -1 )
		nDU++;

	return nDU;
}

/**********************************************************************/

void CProcessedOrderPreviewCSVRecord::SetSURequired( int nSpIdx, double dSU )
{
	if ( nSpIdx >= 0 && nSpIdx < GetStockpointCount() )
		m_dSUQtyArray[ nSpIdx ] = dSU;
}

/**********************************************************************/

void CProcessedOrderPreviewCSVRecord::AddSURequired( int nSpIdx, double dSU )
{
	if ( nSpIdx >= 0 && nSpIdx < GetStockpointCount() )
		m_dSUQtyArray[ nSpIdx ] += dSU;
}

/**********************************************************************/

void CProcessedOrderPreviewCSVRecord::UpdateSupplierCosts( CLocationSelectorMiniLookup& LocSelMiniLookup )
{
	m_nDUItems = 1;
	m_dDUItemSize = 1.0;
	m_nSupplierIdx = -1;

	double dDUYield = 1.0;
	double dDUCost = 0.0;
		
	if ( m_nStockIdx >= 0 && m_nStockIdx < DataManager.Stock.GetSize() )
	{
		CStockCSVRecord StockRecord;
		DataManager.Stock.GetAt ( m_nStockIdx, StockRecord );

		if ( m_nStockSuppIdx >= 0 && m_nStockSuppIdx < StockRecord.GetSupplierCount() )
		{
			m_nDUItems = StockRecord.GetDUItems( m_nStockSuppIdx );
			m_dDUItemSize = StockRecord.GetDUItemSize( m_nStockSuppIdx );

			if ( m_nDUItems < 1 ) m_nDUItems = 1;
			if ( m_dDUItemSize <= 0.0 ) m_dDUItemSize = 1.0;

			dDUCost = StockRecord.GetCost ( m_nStockSuppIdx );
			dDUYield = m_nDUItems * m_dDUItemSize;

			if ( DataManager.Supplier.FindSupplierByNumber ( StockRecord.GetSuppNo ( m_nStockSuppIdx ), m_nSupplierIdx ) == FALSE )
				m_nSupplierIdx = -1;
		}
	}

	m_nDUItemsRequired = 0;
	double dTotalSURequired = 0.0;
	m_nDURequired = 0;
	
	for ( int nSpIdx = 0; nSpIdx < GetStockpointCount(); nSpIdx++ )
	{
		if ( CProcessedOrderPreviewCSVArray::IsStockpointIncluded( nSpIdx, LocSelMiniLookup, FALSE ) == FALSE )
			continue;

		double dSURequired = m_dSUQtyArray[ nSpIdx ];
		dTotalSURequired += dSURequired;
		
		m_nDUItemsRequired += CalculateDUItemRequired( nSpIdx );
			
		if ( StockOptions.GetOrderSeparationType() != ORDER_DUSEPARATE_NONE )
			m_nDURequired += CalculateDURequired( nSpIdx );
	}

	if ( StockOptions.GetOrderSeparationType() == ORDER_DUSEPARATE_NONE )
	{
		m_nDURequired = m_nDUItemsRequired / m_nDUItems;
		if ( ( m_nDURequired * m_nDUItems ) < m_nDUItemsRequired ) m_nDURequired++;
	}

	m_dSUTotal = m_nDURequired * dDUYield;
	m_nDUItemSurplus = ( m_nDURequired * m_nDUItems ) - m_nDUItemsRequired;
	m_dTotalCost = dDUCost * m_nDURequired;
	m_dSUCost = dDUCost / dDUYield;
}

/**********************************************************************/

void CProcessedOrderPreviewCSVRecord::AllocateSurplusStock ( int nSpIdx )
{
	if ( nSpIdx < 0 || nSpIdx >= GetStockpointCount() )
		return;

	int nDUItemsAssigned = 0;
	for ( int nIndex = 0; nIndex < GetStockpointCount(); nIndex++ )
		nDUItemsAssigned += CalculateDUItemRequired( nIndex );

	int nSurplusItems = ( m_nDUItems * m_nDURequired ) - nDUItemsAssigned;
	if ( nSurplusItems <= 0 )
		return;

	int nDUItems = CalculateDUItemRequired( nSpIdx ) + nSurplusItems;
	SetSURequired ( nSpIdx, m_dDUItemSize * nDUItems );
}

/**********************************************************************/

CProcessedOrderPreviewCSVRecord& CProcessedOrderPreviewCSVRecord::operator= ( CProcessedOrderPreviewCSVRecord& source )
{
	ClearOrders();

	m_nStockIdx = source.GetStockIdx();
	m_nStockSuppIdx = source.GetStockSuppIdx();
	m_nSupplierIdx = source.GetSupplierIdx();
	m_nDUItems = source.GetDUItems();
	m_dDUItemSize = source.GetDUItemSize();
	m_nDUItemsRequired = source.GetDUItemsRequired();
	m_nDURequired = source.GetDURequired();
	m_dSUTotal = source.GetSUTotal();
	m_dTotalCost = source.GetTotalCost();
	m_dSUCost = source.GetSUCost();
	m_nDUItemSurplus = source.GetDUItemSurplus();

	m_dSUQtyArray.SetSize( source.GetStockpointCount(), 10 );
	for ( int nSpIdx = 0; nSpIdx < source.GetStockpointCount(); nSpIdx++ )
		m_dSUQtyArray[ nSpIdx ] = source.GetSURequired( nSpIdx );
	
	return *this;
}

/**********************************************************************/

const char* CProcessedOrderPreviewCSVRecord::GetDUItemSurplusString()
{
	m_strDUItemSurplus = "0";
	if ( m_nDUItemSurplus > 0 )
		m_strDUItemSurplus.Format ( "%d x %.3f", m_nDUItemSurplus, m_dDUItemSize );

	return m_strDUItemSurplus;
}

/**********************************************************************/

bool CProcessedOrderPreviewCSVArray::FindStockIdx ( int nStockIdx, int& nIndex )
{
	int nStart = 0;
	int nEnd = GetSize() - 1;

	int nStockIdxToFind = nStockIdx;
	CProcessedOrderPreviewCSVRecord Record;

	while ( nStart <= nEnd )
	{
		nIndex = ( nStart + nEnd ) / 2;
		GetAt ( nIndex, Record );
		int nStockIdx = Record.GetStockIdx();

		if ( nStockIdx < nStockIdxToFind )
			nStart = nIndex + 1;

		if ( nStockIdx > nStockIdxToFind )
			nEnd = nIndex - 1;

		if ( nStockIdx == nStockIdxToFind )
			return TRUE;
	}
		
	nIndex = nStart;
	return FALSE;
}

/**********************************************************************/

bool CProcessedOrderPreviewCSVArray::IsStockpointIncluded( int nSpIdx, CLocationSelectorMiniLookup& LocSelMiniLookup, bool bGenerate )
{
	if ( nSpIdx < 0 || nSpIdx >= dbStockpoint.GetSize() )
		return FALSE;

	if ( PasswordArray.CheckLocationAccessSpIdx( nSpIdx ) == FALSE )
		return FALSE;

	if ( ( StockOptions.GetOrderRestrictPreviewFlag() == FALSE ) && ( bGenerate == FALSE ) )
	{
		int nDbIdx = DataManager.GetActiveDbIdx();
		if ( nDbIdx < 0 || nDbIdx >= dbDatabase.GetSize() )
			return FALSE;

		return ( dbStockpoint.GetDbNo( nSpIdx ) == dbDatabase.GetDbNo( nDbIdx ) );
	}

	switch ( LocSelMiniLookup.GetConType() )
	{
	case NODE_STOCKPOINT:	return ( dbStockpoint.GetSpNo( nSpIdx ) == LocSelMiniLookup.GetEntityNo() );
	case NODE_SITE:			return ( dbStockpoint.GetSiteNo( nSpIdx ) == LocSelMiniLookup.GetEntityNo() );
	case NODE_DATABASE:		return ( dbStockpoint.GetDbNo( nSpIdx ) == LocSelMiniLookup.GetEntityNo() );
	default:				return FALSE;
	}
}

/**********************************************************************/
