/**********************************************************************/
 
/**********************************************************************/
#include "DatabaseCSVArray.h"
//#include "globalfunctions.h"
#include "NodeTypes.h"
#include "StockpointCSVArray.h"
/**********************************************************************/
#include "ProcessedOrder.h"
/**********************************************************************/

CProcessedOrder::CProcessedOrder()
{
	m_bInitialised = FALSE;
	m_nConLevel = NODE_DATABASE;
	m_nDbNo = 1;
	m_nSpNo = 0;
}

/**********************************************************************/

void CProcessedOrder::Initialise( int nConLevel, int nDbNo, int nSpNo, int nOrderNum, int nSuppNo, int nOrderType, CString& strDate, CString& strTime )
{
	m_nConLevel = nConLevel;
	m_nDbNo = nDbNo;
	m_nSpNo = nSpNo;
	
	m_OrderHeader.SetOrderNum( nOrderNum );
	m_OrderHeader.SetSuppNo( nSuppNo );
	
	if ( ( strDate.GetLength() == 8 ) && ( strTime.GetLength() == 6 ) )
	{
		m_OrderHeader.SetDateString( strDate );
		m_OrderHeader.SetTimeString( strTime );
	}
	else
		m_OrderHeader.SetDate();
	
	m_OrderHeader.SetOrderType( nOrderType );

	m_arrayOrderItems.RemoveAll();

	m_bInitialised = TRUE;
}

/**********************************************************************/

void CProcessedOrder::GetHeader( CProcessedOrderHeader& header)
{
	header = m_OrderHeader;
}

/**********************************************************************/

void CProcessedOrder::SetHeader( CProcessedOrderHeader& header)
{
	m_OrderHeader = header;
}

/**********************************************************************/

bool CProcessedOrder::GetItem( int nItemIdx, CProcessedOrderItem& item )
{
	if ( ( nItemIdx < 0 ) || ( nItemIdx >= GetItemCount() ) )
		return FALSE;

	CCSV csv( m_arrayOrderItems.GetAt( nItemIdx ) );
	item.ConvertFromCSV( csv );
	return TRUE;
}

/**********************************************************************/

bool CProcessedOrder::GetItemString( int nItemIdx, CString& strItem )
{
	strItem = "";

	if ( ( nItemIdx < 0 ) || ( nItemIdx >= GetItemCount() ) )
		return FALSE;

	strItem = m_arrayOrderItems.GetAt( nItemIdx );
	return TRUE;
}

/**********************************************************************/

bool CProcessedOrder::AddItem( CProcessedOrderItem& item )
{
	if ( FALSE == m_bInitialised )
		return FALSE;

	CCSV csv;
	item.ConvertToCSV( csv );
	m_arrayOrderItems.Add( csv.GetLine() );
	
	return TRUE;
}

/**********************************************************************/

void CProcessedOrder::ReadOrder()
{
	if ( m_bInitialised != TRUE )
		return;

	CString strFilename = GetOrderFilename();

	if ( strFilename != "" )
	{
		CSSFile fileOrder;
		if ( fileOrder.Open( strFilename, "rb" ) == TRUE )
		{
			bool bGotHeader = FALSE;

			CString strBuffer;
			while( fileOrder.ReadString( strBuffer ) == TRUE )
			{
				CCSV csv( strBuffer );

				CString strType = csv.GetString(0);

				if ( ( strType == "H" ) && ( FALSE == bGotHeader ) )
				{
					m_OrderHeader.ConvertFromCSV( csv );
					bGotHeader = TRUE;
				}
				else if ( strType == "D" )
					m_arrayOrderItems.Add( strBuffer );
			}
		}
	}
}

/**********************************************************************/

void CProcessedOrder::WriteOrder()
{
	if ( m_bInitialised != TRUE )
		return;

	CString strFilename = GetOrderFilename();
	
	if ( strFilename != "" )
	{
		CSSFile fileOrder;
		if ( fileOrder.Open( strFilename, "wb" ) == TRUE )
		{
			CCSV csv;
			m_OrderHeader.ConvertToCSVForFile( csv );
			fileOrder.WriteLine( csv.GetLine() );

			for ( int n = 0; n < m_arrayOrderItems.GetSize(); n++ )
				fileOrder.WriteLine( m_arrayOrderItems.GetAt(n) );
		}
	}
}

/**********************************************************************/

const char* CProcessedOrder::GetOrderFilename()
{
	int nSuppNo = m_OrderHeader.GetSuppNo();
	int nOrderNum = m_OrderHeader.GetOrderNum();
	int nOrderType = m_OrderHeader.GetOrderType();

	CString strDate = m_OrderHeader.GetDateString();
	CString strTime = m_OrderHeader.GetTimeString();

	m_strFilename = "";

	switch( m_nConLevel )
	{
	case NODE_DATABASE:
		{
			int nDbIdx;
			if ( dbDatabase.FindDatabaseByNumber( m_nDbNo, nDbIdx ) == TRUE )
			{
				CreateDatabaseOrderFolders( nDbIdx, nSuppNo );
				m_strFilename = dbDatabase.GetFilePathDataOrder( nDbIdx, nSuppNo, nOrderType, nOrderNum, strDate, strTime );
			}
		}
		break;

	case NODE_STOCKPOINT:
		{
			int nSpIdx;
			if ( dbStockpoint.FindStockpointByNumber( m_nDbNo, m_nSpNo, nSpIdx ) == TRUE )
			{
				CreateStockpointOrderFolders( nSpIdx, nSuppNo );
				m_strFilename = dbStockpoint.GetFilePathDataOrder( nSpIdx, nSuppNo, nOrderType, nOrderNum, strDate, strTime );
			}
		}
		break;
	}

	return m_strFilename;
}

/**********************************************************************/

void CProcessedOrder::CreateDatabaseOrderFolders( int nDbIdx, int nSuppNo )
{
	CString strFolder = dbDatabase.GetFolderPathDataOrdersRoot( nDbIdx );
	::CreateSubdirectory( strFolder );
	strFolder = dbDatabase.GetFolderPathDataOrdersSupplier( nDbIdx, nSuppNo );
	::CreateSubdirectory( strFolder );
	strFolder = dbDatabase.GetFolderPathDataOrdersType( nDbIdx, nSuppNo, ORDER_TYPE_OPEN );
	::CreateSubdirectory( strFolder );
	strFolder = dbDatabase.GetFolderPathDataOrdersType( nDbIdx, nSuppNo, ORDER_TYPE_MIXED );
	::CreateSubdirectory( strFolder );
	strFolder = dbDatabase.GetFolderPathDataOrdersType( nDbIdx, nSuppNo, ORDER_TYPE_CLOSED );
	::CreateSubdirectory( strFolder );
}

/**********************************************************************/

void CProcessedOrder::CreateStockpointOrderFolders( int nSpIdx, int nSuppNo )
{
	CString strFolder = dbStockpoint.GetFolderPathDataOrdersRoot( nSpIdx );
	::CreateSubdirectory( strFolder );
	strFolder = dbStockpoint.GetFolderPathDataOrdersSupplier( nSpIdx, nSuppNo );
	::CreateSubdirectory( strFolder );
	strFolder = dbStockpoint.GetFolderPathDataOrdersType( nSpIdx, nSuppNo, ORDER_TYPE_OPEN );
	::CreateSubdirectory( strFolder );
	strFolder = dbStockpoint.GetFolderPathDataOrdersType( nSpIdx, nSuppNo, ORDER_TYPE_MIXED );
	::CreateSubdirectory( strFolder );
	strFolder = dbStockpoint.GetFolderPathDataOrdersType( nSpIdx, nSuppNo, ORDER_TYPE_CLOSED );
	::CreateSubdirectory( strFolder );
}

/**********************************************************************/
