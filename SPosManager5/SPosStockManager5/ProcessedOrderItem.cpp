/**********************************************************************/
 
/**********************************************************************/
#include "DatabaseCSVArray.h"
//#include "DataManager.h"
/**********************************************************************/
#include "ProcessedOrderItem.h"
/**********************************************************************/

CProcessedOrderItem::CProcessedOrderItem()
{
	ClearRecord();
}

/**********************************************************************/

void CProcessedOrderItem::ClearRecord()
{
	SetDate();
	m_strStockCode = "";
	m_strDescription = "";
	m_strSuppRef = "";
	m_strCase = "";
	m_nDUItems = 0;
	m_dDUItemSize = 0.0;
	m_nDUQty = 0;
	m_dDUCost = 0.0;
	m_bDelivered = FALSE;
	m_bDeliveredOld = FALSE;
	m_nStockIdx = 0;
	
	ClearStockpointOrders();
}

/**********************************************************************/

CProcessedOrderItem::~CProcessedOrderItem()
{	
	ClearStockpointOrders();
}

/**********************************************************************/

void CProcessedOrderItem::AddOrder( int nSpNo, int nDUItems )
{
	COrderItemSpInfo info;
	info.m_nSpNo = nSpNo;
	info.m_nDUItems = nDUItems;
	m_arraySPOrders.Add( info );
}

/**********************************************************************/

void CProcessedOrderItem::ClearStockpointOrders()
{
	m_arraySPOrders.RemoveAll();
}

/**********************************************************************/

void CProcessedOrderItem::ContractStockpointOrders()
{
	for ( int nIndex = m_arraySPOrders.GetSize() -1; nIndex >= 0; nIndex-- )
		if ( m_arraySPOrders[nIndex].m_nDUItems <= 0 )
			m_arraySPOrders.RemoveAt( nIndex );
}

/**********************************************************************/

void CProcessedOrderItem::ExpandStockpointOrders( int nStockIdx, CStockCSVRecord& StockRecord )
{
	int nSpStartIdx, nSpEndIdx;
	dbStockpoint.GetSpIdxRange( DataManager.GetActiveDbIdx(), nSpStartIdx, nSpEndIdx );

	CArray<COrderItemSpInfo,COrderItemSpInfo> arrayTemp;

	//MAKE SURE THAT WE HAVE A LINE FOR EACH STOCKPOINT
	for ( int nSpIdx = nSpStartIdx; nSpIdx <= nSpEndIdx; nSpIdx++ )
	{
		int nSpNo = dbStockpoint.GetSpNo( nSpIdx );
		
		COrderItemSpInfo info;
		info.m_nSpNo = nSpNo;
		info.m_nDUItems = GetStockpointOrder( nSpNo );
		
		bool bInclude = TRUE;
		if ( 0 == info.m_nDUItems )
		{
			CStockLevelsCSVRecord StockLevels;
			DataManager.Stock.GetStockpointTotals( nStockIdx, StockRecord, nSpNo, StockLevels );
			bInclude = StockLevels.GetActiveFlag();
		}
		
		if ( TRUE == bInclude )
			arrayTemp.Add( info );
	}

	//INCLUDE ORDERS FOR ANY UNRECOGNIZED STOCKPOINTS
	int nDbNo = dbDatabase.GetDbNo( DataManager.GetActiveDbIdx() );
	for ( int n = 0; n < m_arraySPOrders.GetSize(); n++ )
	{
		COrderItemSpInfo info = m_arraySPOrders.GetAt(n);

		int nSpIdx;
		if ( dbStockpoint.FindStockpointByNumber( nDbNo, info.m_nSpNo, nSpIdx ) == FALSE )
			arrayTemp.Add( info );
	}
	
	m_arraySPOrders.RemoveAll();
	for ( int nIndex = 0; nIndex < arrayTemp.GetSize(); nIndex++ )
		m_arraySPOrders.Add( arrayTemp.GetAt( nIndex ) );
}

/**********************************************************************/

void CProcessedOrderItem::ConvertToCSV ( CCSV& csv )
{
	csv.Add ( "D" );
	csv.Add ( ORDERITEM_VERSION );
	csv.Add ( m_strDate );
	csv.Add ( m_strStockCode );
	csv.Add ( m_strDescription );
	csv.Add ( m_strSuppRef );
	csv.Add ( m_strCase );
	csv.Add ( m_nDUItems );
	csv.Add ( m_dDUItemSize, 3 );
	csv.Add ( m_nDUQty );
	csv.Add ( m_dDUCost, 5 );
	csv.Add ( m_bDelivered );
	csv.Add ( m_bDeliveredOld );
	csv.Add ( m_nStockIdx );

	int nSize = m_arraySPOrders.GetSize();
	csv.Add ( nSize );

	for ( int nIndex = 0; nIndex < nSize; nIndex++ )
	{
		csv.Add ( m_arraySPOrders[nIndex].m_nSpNo );
		csv.Add ( m_arraySPOrders[nIndex].m_nDUItems );
	}
}

/**********************************************************************/

void CProcessedOrderItem::ConvertFromCSV ( CCSV& csv )
{
	ClearRecord();
	switch ( csv.GetInt(1) )
	{
	case 1:	V1ConvertFromCSV( csv );	break;
	}
}

/**********************************************************************/

void CProcessedOrderItem::V1ConvertFromCSV ( CCSV& csv )
{
	int n = 2;
	SetDate ( csv.GetString(n++) );
	SetStockCode( csv.GetString(n++) );
	SetDescription( csv.GetString(n++) );
	SetSuppRef( csv.GetString(n++) );
	SetCase( csv.GetString(n++) );
	SetDUItems( csv.GetInt(n++) );
	SetDUItemSize( csv.GetDouble(n++) );
	SetDUQty( csv.GetInt(n++) );
	SetDUCost( csv.GetDouble(n++) );
	m_bDelivered = csv.GetBool(n++);
	m_bDeliveredOld = csv.GetBool(n++);
	SetStockIdx( csv.GetInt(n++) );
	
	int nCount = csv.GetInt ( n++ );
	for ( int nIndex = 0; nIndex < nCount; nIndex++ )
	{	
		COrderItemSpInfo info;
		info.m_nSpNo = csv.GetInt(n++);
		info.m_nDUItems = csv.GetInt(n++);
		m_arraySPOrders.Add( info );
	}
}

/**********************************************************************/

double CProcessedOrderItem::CalculateSUQty()
{
	return ( m_dDUItemSize * m_nDUItems * m_nDUQty );
}

/**********************************************************************/

double CProcessedOrderItem::CalculateSUCost( double dQty )
{
	double dTotalSU = CalculateSUQty();
	double dTotalCost = m_dDUCost * m_nDUQty;
	
	if ( dTotalSU != 0.0 )
		return ( dTotalCost * dQty ) / dTotalSU;
	else
		return 0.0;
}

/**********************************************************************/

int CProcessedOrderItem::GetStockpointOrder( int nSpNo )
{
	int nResult = 0;

	for ( int nIndex = 0; nIndex < m_arraySPOrders.GetSize(); nIndex++ )
	{			
		if ( m_arraySPOrders[nIndex].m_nSpNo == nSpNo )
		{
			nResult = m_arraySPOrders[nIndex].m_nDUItems;
			break;
		}
	}
	return nResult;			
}

/**********************************************************************/

int CProcessedOrderItem::GetSpNo ( int nIndex )
{
	if ( nIndex >= 0 && nIndex < m_arraySPOrders.GetSize() )
		return m_arraySPOrders[nIndex].m_nSpNo;

	return -1;
}

/**********************************************************************/

int CProcessedOrderItem::GetDUItems ( int nIndex )
{
		if ( nIndex >= 0 && nIndex < m_arraySPOrders.GetSize() )
		return m_arraySPOrders[nIndex].m_nDUItems;

	return 0;
}

/**********************************************************************/

void CProcessedOrderItem::SetDUItems ( int nIndex, int nItems )
{
	if ( nIndex >= 0 && nIndex < m_arraySPOrders.GetSize() )
		m_arraySPOrders[nIndex].m_nDUItems = nItems;
}

/**********************************************************************/

void CProcessedOrderItem::SetDate()
{
	CSSDate dateNow;
	dateNow.SetCurrentDate();
	m_strDate = dateNow.GetCSVDate();
}

/**********************************************************************/

void CProcessedOrderItem::SetStockCode ( const char* szCode )
{
	m_strStockCode = szCode;
	::TrimSpaces ( m_strStockCode, FALSE );
}

/**********************************************************************/
//	Description
/**********************************************************************/

void CProcessedOrderItem::SetDescription ( const char* szDesc )
{
	m_strDescription = szDesc;
	::TrimSpaces ( m_strDescription, FALSE );
	
	if ( m_strDescription == "" )
		m_strDescription = m_strStockCode;
}

/**********************************************************************/

CProcessedOrderItem& CProcessedOrderItem::operator= ( CProcessedOrderItem& source )
{
	SetDate			( source.GetDate() );
	SetStockCode	( source.GetStockCode() );
	SetDescription	( source.GetDescription() );
	SetSuppRef		( source.GetSuppRef() );
	SetCase			( source.GetCase() );
	SetDUItems		( source.GetDUItems() );
	SetDUItemSize	( source.GetDUItemSize() );
	SetDUQty		( source.GetDUQty() );
	SetDUCost		( source.GetDUCost() );

	if ( source.GetDeliveredFlag() )
		SetDeliveredFlag();
	else
		ClearDeliveredFlag();

	if ( source.GetDeliveredOldFlag() )
		SetDeliveredOldFlag();
	else
		ClearDeliveredOldFlag();

	SetStockIdx		( source.GetStockIdx() );

	m_arraySPOrders.RemoveAll();
	for ( int nIndex = 0; nIndex < source.GetStockpointCount(); nIndex++ )
	{	
		COrderItemSpInfo info;
		info.m_nSpNo = source.GetSpNo(nIndex);
		info.m_nDUItems = source.GetDUItems(nIndex);
		m_arraySPOrders.Add( info );
	}

	return *this;
}

/**********************************************************************/

