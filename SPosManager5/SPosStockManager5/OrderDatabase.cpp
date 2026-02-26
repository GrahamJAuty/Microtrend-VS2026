/**********************************************************************/
#include "DatabaseCSVArray.h"
//#include "DataManager.h"
//#include "DealerFlags.h"
#include "FileRemover.h"
//#include "globalfunctions.h"
#include "NodeTypes.h"
#include "ProcessedOrder.h"
#include "StatusProgress.h"
#include "StockOptionsIni.h"
/**********************************************************************/
#include "OrderDatabase.h"
/**********************************************************************/
static const int ORDER_START = 10;		//index of first order field
/**********************************************************************/

COrderCSVRecord::COrderCSVRecord()
{
	ClearRecord();
}

/**********************************************************************/

void COrderCSVRecord::ClearRecord()
{
	m_nOrderNum = 0;
	m_nSuppNo = 0;
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
	m_nStockIdx = 0;
	
	ClearStockpointOrders();
}

/**********************************************************************/

COrderCSVRecord::COrderCSVRecord( CProcessedOrderPreviewCSVRecord& PreviewRecord, CStockCSVRecord& StockRecord, int nOrderNum, int nStockSuppIdx, CLocationSelectorLookup& LocSelLookup )
{
	SetOrderNum		( nOrderNum );
	
	int nSuppIdx = PreviewRecord.GetSupplierIdx();
	if ( nSuppIdx >= 0 && nSuppIdx < DataManager.Supplier.GetSize() )
		SetSuppNo( DataManager.Supplier.GetSuppNo( nSuppIdx ) );
	else
		SetSuppNo( 0 );
	
	SetDate				();
	SetStockCode		( StockRecord.GetStockCode() );
	SetDescription		( StockRecord.GetDescription() );
	SetSuppRef			( StockRecord.GetSuppRefExternal ( nStockSuppIdx, DealerFlags.GetSuppRefFlag() ) );
	SetCase				( StockRecord.GetCase ( nStockSuppIdx ) );
	SetDUItems			( PreviewRecord.GetDUItems() );
	SetDUItemSize		( PreviewRecord.GetDUItemSize() );
	SetDUQty			( PreviewRecord.GetDURequired() );
	SetDUCost			( StockRecord.GetCost ( nStockSuppIdx ) );
	SetStockIdx			( -1 );
	
	ClearDeliveredFlag();
	
	for ( int nSpIdx = 0; nSpIdx < PreviewRecord.GetStockpointCount(); nSpIdx++ )
	{
		if ( CProcessedOrderPreviewCSVArray::IsStockpointIncluded( nSpIdx, LocSelLookup, FALSE ) )
		{
			int nDUItems;
			if ( StockOptions.GetOrderSeparationType() != ORDER_DUSEPARATE_NONE )
				nDUItems = PreviewRecord.CalculateDURequired( nSpIdx ) * PreviewRecord.GetDUItems();
			else
				nDUItems = PreviewRecord.CalculateDUItemRequired( nSpIdx );

			CSPOrderInfo info;
			info.m_nSpNo = dbStockpoint.GetSpNo( nSpIdx );
			info.m_nDUItems = nDUItems;
			m_arraySPOrders.Add( info );
		}
	}
}

/**********************************************************************/

COrderCSVRecord::~COrderCSVRecord()
{	
	ClearStockpointOrders();
}

/**********************************************************************/

void COrderCSVRecord::AddOrder( int nSpNo, int nDUItems )
{
	CSPOrderInfo info;
	info.m_nSpNo = nSpNo;
	info.m_nDUItems = nDUItems;
	m_arraySPOrders.Add( info );
}

/**********************************************************************/

void COrderCSVRecord::ClearStockpointOrders()
{
	m_arraySPOrders.RemoveAll();
}

/**********************************************************************/

void COrderCSVRecord::ConvertToCSV ( CCSV& csv )
{
	csv.Add ( ORDERDB_VERSION );
	csv.Add ( m_nOrderNum );
	csv.Add ( m_nSuppNo );
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

void COrderCSVRecord::ConvertFromCSV ( CCSV& csv )
{
	ClearRecord();
	switch ( csv.GetInt(0) )
	{
	case 1:	V1ConvertFromCSV( csv );	break;
	}
}

/**********************************************************************/

void COrderCSVRecord::V1ConvertFromCSV ( CCSV& csv )
{
	int n = 1;
	SetOrderNum ( csv.GetInt(n++) );
	SetSuppNo( csv.GetInt(n++) );
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
	SetStockIdx( csv.GetInt(n++) );

	int nCount = csv.GetInt ( n++ );
	for ( int nIndex = 0; nIndex < nCount; nIndex++ )
	{
		CSPOrderInfo info;
		info.m_nSpNo = csv.GetInt(n++);
		info.m_nDUItems = csv.GetInt(n++);
		m_arraySPOrders.Add( info );
	}
}

/**********************************************************************/

double COrderCSVRecord::CalculateSUQty()
{
	return ( m_dDUItemSize * m_nDUItems * m_nDUQty );
}

/**********************************************************************/

double COrderCSVRecord::CalculateSUCost( double dQty )
{
	double dTotalSU = CalculateSUQty();
	double dTotalCost = m_dDUCost * m_nDUQty;
	
	if ( dTotalSU != 0.0 )
		return ( dTotalCost * dQty ) / dTotalSU;
	else
		return 0.0;
}

/**********************************************************************/

int COrderCSVRecord::GetStockpointOrder( int nSpNo )
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

int COrderCSVRecord::GetSpNo ( int nIndex )
{
	if ( nIndex >= 0 && nIndex < m_arraySPOrders.GetSize() )
		return m_arraySPOrders[nIndex].m_nSpNo;

	return -1;
}

/**********************************************************************/

int COrderCSVRecord::GetDUItems ( int nIndex )
{
		if ( nIndex >= 0 && nIndex < m_arraySPOrders.GetSize() )
		return m_arraySPOrders[nIndex].m_nDUItems;

	return 0;
}

/**********************************************************************/

void COrderCSVRecord::SetDUItems ( int nIndex, int nItems )
{
	if ( nIndex >= 0 && nIndex < m_arraySPOrders.GetSize() )
		m_arraySPOrders[nIndex].m_nDUItems = nItems;
}

/**********************************************************************/

void COrderCSVRecord::SetDate()
{
	CSSDate dateNow;
	dateNow.SetCurrentDate();
	m_strDate = dateNow.GetCSVDate();
}

/**********************************************************************/

void COrderCSVRecord::SetStockCode ( const char* szCode )
{
	m_strStockCode = szCode;
	::TrimSpaces ( m_strStockCode, FALSE );
}

/**********************************************************************/
//	Description
/**********************************************************************/

void COrderCSVRecord::SetDescription ( const char* szDesc )
{
	m_strDescription = szDesc;
	::TrimSpaces ( m_strDescription, FALSE );
	
	if ( m_strDescription == "" )
		m_strDescription = m_strStockCode;
}

/**********************************************************************/

COrderCSVRecord& COrderCSVRecord::operator= ( COrderCSVRecord& source )
{
	SetOrderNum	( source.GetOrderNum() );
	SetSuppNo		( source.GetSuppNo() );
	SetDate		( source.GetDate() );
	SetStockCode	( source.GetStockCode() );
	SetDescription	( source.GetDescription() );
	SetSuppRef	( source.GetSuppRef() );
	SetCase		( source.GetCase() );
	SetDUItems	( source.GetDUItems() );
	SetDUItemSize	( source.GetDUItemSize() );
	SetDUQty		( source.GetDUQty() );
	SetDUCost		( source.GetDUCost() );

	if ( source.GetDeliveredFlag() )
		SetDeliveredFlag();
	else
		ClearDeliveredFlag();

	SetStockIdx		( source.GetStockIdx() );

	m_arraySPOrders.RemoveAll();
	for ( int nIndex = 0; nIndex < source.GetStockpointCount(); nIndex++ )
	{	
		CSPOrderInfo info;
		info.m_nSpNo = source.GetSpNo(nIndex);
		info.m_nDUItems = source.GetDUItems(nIndex);
		m_arraySPOrders.Add( info );
	}

	return *this;
}

/**********************************************************************/

COrderCSVArray::COrderCSVArray() : CSharedCSVArray()
{
}

/**********************************************************************/

int COrderCSVArray::Open( const char* szFilename, int nMode )
{
	RemoveAt( 0, GetSize() );

	int nReply = CSharedCSVArray::Open ( szFilename, nMode );
	
	if ( nReply != DB_ERR_SHARE )
	{
		if ( nMode == DB_READONLY )	// if file does not exist use default values
			nReply = DB_ERR_NONE;
	}

	return nReply;
}

/**********************************************************************/

void COrderCSVArray::RemoveAll()
{
	RemoveAt ( 0, GetSize() );
}

/**********************************************************************/

int COrderCSVArray::AddNewOrderLine( COrderCSVRecord& NewRecord )
{
	int nSuppNo = NewRecord.GetSuppNo();
	int nOrderNum = NewRecord.GetOrderNum();

	int nIndex = FindEndOfOrder( nSuppNo, nOrderNum );
	InsertAt( nIndex, NewRecord );
	
	return ( nIndex );
}

/**********************************************************************/

bool COrderCSVArray::GetDeliveredFlag ( int nOrderIdx )
{
	if ( nOrderIdx < 0 || nOrderIdx >= GetSize() )
		return FALSE;

	COrderCSVRecord OrderRecord;
	GetAt ( nOrderIdx, OrderRecord );
	return OrderRecord.GetDeliveredFlag();
}

/**********************************************************************/

void COrderCSVArray::SortOrders()
{
	COrderCSVArray arrayTemp;

	int nSize = GetSize();
	
	StatusProgress.Lock( TRUE, "Sorting orders" );
	for ( int nOrderIdx = 0; nOrderIdx < GetSize(); nOrderIdx++ )
	{
		StatusProgress.SetPos( nOrderIdx, nSize );

		COrderCSVRecord OrderRecord;
		GetAt( nOrderIdx, OrderRecord );

		int nSuppNo = OrderRecord.GetSuppNo();
		int nOrderNum = OrderRecord.GetOrderNum();
		
		int nIndex = arrayTemp.FindEndOfOrder( nSuppNo, nOrderNum );
		arrayTemp.InsertAt( nIndex, OrderRecord );
	}
	
	RemoveAll();
	nSize = arrayTemp.GetSize();

	for ( int nOrderIdx = 0; nOrderIdx < nSize; nOrderIdx++ )
	{
		StatusProgress.SetPos( nOrderIdx, nSize );
	
		COrderCSVRecord OrderRecord;
		arrayTemp.GetAt( nOrderIdx, OrderRecord );
		Add( OrderRecord );
	}
	StatusProgress.Unlock();
}

/**********************************************************************/

int COrderCSVArray::FindEndOfOrder( int nSuppNo, int nOrderNum )
{
	int nStartIdx = 0;
	int nEndIdx = GetSize() - 1;

	while ( nStartIdx <= nEndIdx )
	{
		int nArrayIdx = ( nStartIdx + nEndIdx ) / 2;

		COrderCSVRecord OrderRecord;
		GetAt( nArrayIdx, OrderRecord );

		if ( OrderRecord.GetSuppNo() < nSuppNo )
		{
			nStartIdx = nArrayIdx + 1;
			continue;
		}

		if ( OrderRecord.GetSuppNo() > nSuppNo )
		{
			nEndIdx = nArrayIdx - 1;
			continue;
		}

		if ( OrderRecord.GetOrderNum() > nOrderNum )
		{
			nEndIdx = nArrayIdx - 1;
			continue;
		}

		nStartIdx = nArrayIdx + 1;
	}

	return nStartIdx;
}

/**********************************************************************/

int COrderCSVArray::FindEndOfOrder( int nSuppNo )
{
	int nStartIdx = 0;
	int nEndIdx = GetSize() - 1;

	while ( nStartIdx <= nEndIdx )
	{
		int nArrayIdx = ( nStartIdx + nEndIdx ) / 2;

		COrderCSVRecord OrderRecord;
		GetAt( nArrayIdx, OrderRecord );

		if ( OrderRecord.GetSuppNo() > nSuppNo )
			nEndIdx = nArrayIdx - 1;
		else
			nStartIdx = nArrayIdx + 1;
	}

	return nStartIdx;
}

/**********************************************************************/

int COrderCSVArray::GetOrderLineCount( int nSuppNo )
{
	int nStartIdx = FindEndOfOrder( nSuppNo - 1 );
	int nEndIdx = FindEndOfOrder( nSuppNo );
	int nLineCount = nEndIdx - nStartIdx;
	if ( nLineCount < 0 ) nLineCount = 0;
	return nLineCount;
}

/**********************************************************************/

int COrderCSVArray::GetOrderLineCount( int nSuppNo, int nOrderNum )
{
	int nStartIdx = FindEndOfOrder( nSuppNo, nOrderNum - 1 );
	int nEndIdx = FindEndOfOrder( nSuppNo, nOrderNum );
	int nLineCount = nEndIdx - nStartIdx;
	if ( nLineCount < 0 ) nLineCount = 0;
	return nLineCount;
}

/**********************************************************************/

void COrderCSVArray::FindOrderLines( int nSuppNo, int nOrderNum, int& nStartIdx, int& nEndIdx )
{
	nStartIdx = FindEndOfOrder( nSuppNo, nOrderNum - 1 );
	nEndIdx = FindEndOfOrder( nSuppNo, nOrderNum ) - 1;
}

/**********************************************************************/

void COrderCSVArray::FindOrderLines( int nSuppNo, int& nStartIdx, int& nEndIdx )
{
	nStartIdx = FindEndOfOrder( nSuppNo - 1 );
	nEndIdx = FindEndOfOrder( nSuppNo ) - 1;
}

/**********************************************************************/

bool COrderCSVArray::NeedUpdateDatabaseOrders()
{
	bool bResult = FALSE;
	
	for ( int nDbIdx = 0; nDbIdx < dbDatabase.GetSize(); nDbIdx++ )
	{
		CFilenameUpdater FnUp( SysFiles::Orders, nDbIdx );
		CString strFilename = FnUp.GetFilenameToUse();

		if ( Open( strFilename, DB_READONLY ) == DB_ERR_NONE )
		{
			bResult = ( GetSize() > 0 );
			Close();
		}

		if ( TRUE == bResult )
			break;
	}
	
	return bResult;
}

/**********************************************************************/

bool COrderCSVArray::UpdateDatabaseOrders( int nDbIdx )
{
	bool bResult = TRUE;

	CFilenameUpdater FnUp( SysFiles::Orders, nDbIdx );
	CString strFilename = FnUp.GetFilenameToUse();

	if ( ::FileExists( strFilename ) == TRUE )
	{
		if ( Open( strFilename, DB_READONLY ) == DB_ERR_NONE )
		{
			if ( GetSize() > 0 )
			{
				if ( DataManager.LockAllUsers() == TRUE )
				{
					UpdateDatabaseOrdersInternal( nDbIdx, strFilename );
					DataManager.UnlockAllUsers();
				}
				else
					bResult = FALSE;
			}

			Close();

			if ( TRUE == bResult )
				CFileRemover FileRemover( strFilename );
		}
	}
	
	return bResult;
}

/**********************************************************************/

void COrderCSVArray::UpdateDatabaseOrdersInternal( int nDbIdx, CString& strFilename )
{
	int nDbNo = dbDatabase.GetDbNo( nDbIdx );
	
	SortOrders();

	int nLastOrderNum = -1;
	int nLastSuppNo = -1;
	CProcessedOrder NewOrder;

	for ( int nItemIdx = 0; nItemIdx < GetSize(); nItemIdx++ )
	{
		COrderCSVRecord OldItem;
		GetAt( nItemIdx, OldItem );

		int nOrderNum = OldItem.GetOrderNum();
		int nSuppNo = OldItem.GetSuppNo();

		if ( ( nOrderNum != nLastOrderNum ) || ( nSuppNo != nLastSuppNo ) )
		{
			if ( nLastOrderNum != -1 )
				if ( NewOrder.GetItemCount() > 0 )
					NewOrder.WriteOrder();

			CString strOldDate = OldItem.GetDate();
					
			CString strNewDate = "00000000";
			CString strNewTime = "000000";

			if ( strOldDate.GetLength() == 8 )
				if ( ::TestNumeric( strOldDate ) == TRUE )
					strNewDate = strOldDate.Right(4) + strOldDate.Mid(2,2) + strOldDate.Left(2);
					
			NewOrder.Initialise( NODE_DATABASE, nDbNo, 0, nOrderNum, nSuppNo, ORDER_TYPE_OPEN, strNewDate, strNewTime );
					
			nLastOrderNum = nOrderNum;
			nLastSuppNo = nSuppNo;
		}

		CCSV csv;
		OldItem.ConvertToCSV( csv );

		if ( csv.GetInt(0) == 1 )
		{
			csv.SetAt( 0, "D" );
			csv.SetAt( 1, "1" );
			csv.RemoveAt(2);
			csv.InsertAt( 12, "0" );
					
			CProcessedOrderItem item;
			item.ConvertFromCSV( csv );	
			NewOrder.AddItem( item );
		}
	}

	if ( nLastOrderNum != -1 )
		if ( NewOrder.GetItemCount() > 0 )
			NewOrder.WriteOrder();
}

/**********************************************************************/

bool COrderCSVArray::NeedUpdateStockpointOrders()
{
	bool bResult = FALSE;
	
	for ( int nSpIdx = 0; nSpIdx < dbStockpoint.GetSize(); nSpIdx++ )
	{
		CFilenameUpdater FnUp( SysFiles::SpOrders, nSpIdx );
		CString strFilename = FnUp.GetFilenameToUse();

		if ( Open( strFilename, DB_READONLY ) == DB_ERR_NONE )
		{
			bResult = ( GetSize() > 0 );
			Close();
		}

		if ( TRUE == bResult )
			break;
	}
	
	return bResult;
}

/**********************************************************************/

bool COrderCSVArray::UpdateStockpointOrders( int nSpIdx )
{
	bool bResult = TRUE;

	CFilenameUpdater FnUp( SysFiles::SpOrders, nSpIdx );
	CString strFilename = FnUp.GetFilenameToUse();

	if ( ::FileExists( strFilename ) == TRUE )
	{
		if ( Open( strFilename, DB_READONLY ) == DB_ERR_NONE )
		{
			if ( GetSize() > 0 )
			{
				if ( DataManager.LockAllUsers() == TRUE )
				{
					UpdateStockpointOrdersInternal( nSpIdx, strFilename );
					DataManager.UnlockAllUsers();
				}
				else
					bResult = FALSE;
			}

			Close();

			if ( TRUE == bResult )
				CFileRemover FileRemover( strFilename );
		}
	}
	
	return bResult;
}

/**********************************************************************/

void COrderCSVArray::UpdateStockpointOrdersInternal( int nSpIdx, CString& strFilename )
{
	int nDbNo = dbStockpoint.GetDbNo( nSpIdx );
	int nSpNo = dbStockpoint.GetSpNo( nSpIdx );

	SortOrders();

	int nLastOrderNum = -1;
	int nLastSuppNo = -1;
	CProcessedOrder NewOrder;

	for ( int nItemIdx = 0; nItemIdx < GetSize(); nItemIdx++ )
	{
		COrderCSVRecord OldItem;
		GetAt( nItemIdx, OldItem );

		int nOrderNum = OldItem.GetOrderNum();
		int nSuppNo = OldItem.GetSuppNo();

		if ( ( nOrderNum != nLastOrderNum ) || ( nSuppNo != nLastSuppNo ) )
		{
			if ( nLastOrderNum != -1 )
				if ( NewOrder.GetItemCount() > 0 )
					NewOrder.WriteOrder();

			CString strOldDate = OldItem.GetDate();
							
			CString strNewDate = "00000000";
			CString strNewTime = "000000";

			if ( strOldDate.GetLength() == 8 )
				if ( ::TestNumeric( strOldDate ) == TRUE )
					strNewDate = strOldDate.Right(4) + strOldDate.Mid(2,2) + strOldDate.Left(2);
					
			NewOrder.Initialise( NODE_STOCKPOINT, nDbNo, nSpNo, nOrderNum, nSuppNo, ORDER_TYPE_OPEN, strNewDate, strNewTime );
					
			nLastOrderNum = nOrderNum;
			nLastSuppNo = nSuppNo;
		}

		CCSV csv;
		OldItem.ConvertToCSV( csv );

		if ( csv.GetInt(0) == 1 )
		{
			csv.SetAt( 0, "D" );
			csv.SetAt( 1, "1" );
			csv.RemoveAt(2);
			csv.InsertAt( 12, "0" );

			CProcessedOrderItem item;
			item.ConvertFromCSV( csv );	
			NewOrder.AddItem( item );
		}
	}

	if ( nLastOrderNum != -1 )
		if ( NewOrder.GetItemCount() > 0 )
			NewOrder.WriteOrder();

	Close();
	CFileRemover FileRemover( strFilename );
}

/**********************************************************************/

void COrderCSVArray::ShowUpgradeError()
{
	CString strMsg = "";
	strMsg += "Stock Manager was unable to update order files from a\nprevious version of this software.\n\n";
	strMsg += "Please log off all users, and then restart Stock Manager\nand try this action again.";
	Prompter.Error( strMsg );
}

/**********************************************************************/
