/**********************************************************************/
//#include "DataManagerNonDb.h"
//#include "DealerFlags.h"
#include "SysInfo.h"
/**********************************************************************/
#include "StockReportSortOrderCSVArray.h"
/**********************************************************************/

CStockReportSortOrderCSVRecord::CStockReportSortOrderCSVRecord()
{
	ClearRecord();
}

/**********************************************************************/

void CStockReportSortOrderCSVRecord::ClearRecord()
{
	m_strName = "";
	m_nSortOrderType = SH_CUSTOM;
	m_arrayFields.RemoveAll();
}

/**********************************************************************/

void CStockReportSortOrderCSVRecord::ConvertFromCSV ( CCSV& csv )
{
	ClearRecord();
	SetName ( csv.GetString (0) );
	SetSortOrderType ( csv.GetInt(1) ); 

	switch ( csv.GetInt(2) )
	{
	case 1:	V1ConvertFromCSV ( csv );	break;
	}

	TidySortOrderFields();
}

/**********************************************************************/

void CStockReportSortOrderCSVRecord::TidySortOrderFields()
{
	switch( DataManagerNonDb.GetActiveStockReportSortOrderType() )
	{
	case STOCKREPORT_SORTORDER_SUPPLIER:
	case STOCKREPORT_SORTORDER_REORDER:
		{
			bool bGotSupplier = FALSE;
			
			if ( m_arrayFields.GetSize() != 0 )
				bGotSupplier = ( ( m_arrayFields.GetAt(0) % 100 ) == SH_FIELD_SUPP );

			if ( FALSE == bGotSupplier )
				m_arrayFields.InsertAt( 0, SH_FIELD_SUPP + SH_FIELD_HEADER + SH_FIELD_TOTAL );
			else
				m_arrayFields.SetAt( 0, SH_FIELD_SUPP + SH_FIELD_HEADER + SH_FIELD_TOTAL );
		}
		break;
	}

	switch( DataManagerNonDb.GetActiveStockReportSortOrderType() )
	{
	case STOCKREPORT_SORTORDER_REORDER:
		{
			bool bGotStockpoint = FALSE;

			for ( int n = m_arrayFields.GetSize() - 1; n >= 1; n-- )
			{
				if ( ( m_arrayFields.GetAt(n) % 100 ) == SH_FIELD_SP )
				{
					m_arrayFields.RemoveAt(n);
					bGotStockpoint = TRUE;
				}
			}

			if ( TRUE == bGotStockpoint )
				m_arrayFields.Add( SH_FIELD_SP );
		}
		break;
	}
}

/**********************************************************************/

void CStockReportSortOrderCSVRecord::V1ConvertFromCSV ( CCSV& csv )
{
	if ( m_nSortOrderType == SH_CUSTOM )
	{
		int nCount = csv.GetInt(3);
		int nOffset = 4;
		for ( int nIndex = 0; nIndex < nCount; nIndex++ )
			m_arrayFields.Add( csv.GetInt( nOffset++ ) );
	}
}

/**********************************************************************/
	
void CStockReportSortOrderCSVRecord::ConvertToCSV ( CCSV& csv )
{
	csv.Add ( m_strName );
	csv.Add ( m_nSortOrderType );
	csv.Add ( STOCKREPORTSORTORDER_VERSION );				
	
	if ( m_nSortOrderType == SH_CUSTOM )
	{
		csv.Add( m_arrayFields.GetSize() );
		for ( int nIndex = 0; nIndex < m_arrayFields.GetSize(); nIndex++ )
			csv.Add ( m_arrayFields.GetAt ( nIndex ) );
	}
}

/**********************************************************************/

void CStockReportSortOrderCSVRecord::SetSortOrderFields( CWordArray& arrayTemp )
{
	m_arrayFields.RemoveAll();
	for ( int nIndex = 0; nIndex < arrayTemp.GetSize(); nIndex++ )
		m_arrayFields.Add( arrayTemp.GetAt( nIndex ) );
}

/**********************************************************************/

const char* CStockReportSortOrderCSVRecord::GetKey()
{
	CCSV csvKey ( '.' );
	
	CString strField;
	strField.Format ( "%3.3d", GetSortOrderType() );
	csvKey.Add( strField );
	
	for ( int nPos = 0; nPos < m_arrayFields.GetSize(); nPos++ )
		csvKey.Add ( m_arrayFields.GetAt( nPos ) );

	m_strKey = csvKey.GetLine();	
	return m_strKey;
}

/**********************************************************************/

const char* CStockReportSortOrderCSVRecord::GetFieldListName( int nField )
{
	switch ( nField )
	{
	case SH_FIELD_SP:			
		m_strFieldListName = "Stockpoint";			
		break;

	case SH_FIELD_CAT_ITEM:		
		m_strFieldListName = "Category";			
		break;

	case SH_FIELD_SUPP:			
		m_strFieldListName = "Supplier Name";		
		break;

	case SH_FIELD_CUSTOM1:	
	case SH_FIELD_CUSTOM2:	
	case SH_FIELD_CUSTOM3:	
	case SH_FIELD_CUSTOM4:	
	case SH_FIELD_CUSTOM5:	
		m_strFieldListName = DataManagerNonDb.CustomFieldNames.GetName( nField + 1 - SH_FIELD_CUSTOM1 );	
		break;

	default:					
		m_strFieldListName = "Unknown";				
		break;
	}

	return m_strFieldListName;
}

/**********************************************************************/

const char* CStockReportSortOrderCSVRecord::GetListName()
{
	switch( m_nSortOrderType )
	{
	case SH_CUSTOM:				m_strListName = m_strName;							break;
	case SH_STKLVL_NONE:		m_strListName = "Stock Code";						break;
	case SH_STKLVL_SUPP:		m_strListName = "Supplier";							break;
	case SH_STKLVL_SUPP_SP:		m_strListName = "Supplier Stockpoint";				break;
	case SH_STKLVL_SUPP_SP_CAT:	m_strListName = "Supplier Stockpoint Category";		break;
	case SH_STKLVL_SUPP_CAT:	m_strListName = "Supplier Category";				break;
	case SH_STKLVL_SUPP_CAT_SP:	m_strListName = "Supplier Category Stockpoint";		break;
	case SH_STKLVL_SP:			m_strListName = "Stockpoint";						break;
	case SH_STKLVL_SP_SUPP:		m_strListName = "Stockpoint Supplier";				break;
	case SH_STKLVL_SP_SUPP_CAT:	m_strListName = "Stockpoint Supplier Category";		break;
	case SH_STKLVL_SP_CAT:		m_strListName = "Stockpoint Category";				break;
	case SH_STKLVL_SP_CAT_SUPP:	m_strListName = "Stockpoint Category Supplier";		break;
	case SH_STKLVL_CAT:			m_strListName = "Category";							break;
	case SH_STKLVL_CAT_SUPP:	m_strListName = "Category Supplier";				break;
	case SH_STKLVL_CAT_SUPP_SP:	m_strListName = "Category Supplier Stockpoint";		break;
	case SH_STKLVL_CAT_SP:		m_strListName = "Category Stockpoint";				break;
	case SH_STKLVL_CAT_SP_SUPP:	m_strListName = "Category Stockpoint Supplier";		break;
	default:					m_strListName = "Unknown";							break;
	}
	return m_strListName;
}

/**********************************************************************/

void CStockReportSortOrderCSVRecord::GetValidFieldList( CWordArray& array )
{
	array.RemoveAll();

	switch( DataManagerNonDb.GetActiveStockReportSortOrderType() )
	{
	case STOCKREPORT_SORTORDER_DELIVERY:
	case STOCKREPORT_SORTORDER_RETURNS:
		array.Add ( SH_FIELD_SP );
		array.Add ( SH_FIELD_CAT_ITEM );
		array.Add ( SH_FIELD_CUSTOM1 );
		array.Add ( SH_FIELD_CUSTOM2 );
		array.Add ( SH_FIELD_CUSTOM3 );
		array.Add ( SH_FIELD_CUSTOM4 );
		array.Add ( SH_FIELD_CUSTOM5 );
		break;

	case STOCKREPORT_SORTORDER_PRICE_LIST:
	case STOCKREPORT_SORTORDER_TRANSFER_PERIOD:
	case STOCKTAKE_SORTORDER_TRANSFER_CLOSING:
		array.Add ( SH_FIELD_CAT_ITEM );
		array.Add ( SH_FIELD_SUPP );
		array.Add ( SH_FIELD_CUSTOM1 );
		array.Add ( SH_FIELD_CUSTOM2 );
		array.Add ( SH_FIELD_CUSTOM3 );
		array.Add ( SH_FIELD_CUSTOM4 );
		array.Add ( SH_FIELD_CUSTOM5 );
		break;

	case STOCKREPORT_SORTORDER_SALES:
	case STOCKREPORT_SORTORDER_TOP_100_SALES:
	case STOCKREPORT_SORTORDER_ESTIMATED_PROFIT:
	case STOCKREPORT_SORTORDER_REORDER:
	case STOCKREPORT_SORTORDER_APPARENT_QTY:
	case STOCKREPORT_SORTORDER_APPARENT_QTY_DU:
	case STOCKREPORT_SORTORDER_APPARENT_SUMMARY:
	case STOCKREPORT_SORTORDER_APPARENT_SUMMARY_DU:
	case STOCKREPORT_SORTORDER_TOP_100_APPARENT:
	case STOCKREPORT_SORTORDER_APPARENT_VALUE:
	case STOCKREPORT_SORTORDER_OVERSTOCK_VALUE:
	case STOCKREPORT_SORTORDER_OPENING_VALUE:
	case STOCKREPORT_SORTORDER_ADJUST:
	case STOCKREPORT_SORTORDER_SHEET:
	case STOCKREPORT_SORTORDER_RETAIL_SUMMARY:
	case STOCKREPORT_SORTORDER_COST_SUMMARY:
	case STOCKREPORT_SORTORDER_TRANSFER_VALUE:
	case STOCKREPORT_SORTORDER_SUPPLIER:
	case STOCKTAKE_SORTORDER_CLOSING_VALUE:
	case STOCKTAKE_SORTORDER_VARIANCE:
	case STOCKTAKE_SORTORDER_ESTIMATE_ALLOWANCE:
	case STOCKTAKE_SORTORDER_RECONCILE:
	case STOCKTAKE_SORTORDER_RECONCILE_CATEGORY:
	case STOCKTAKE_SORTORDER_SHEET:			
	default:
		array.Add ( SH_FIELD_SP );
		array.Add ( SH_FIELD_CAT_ITEM );
		array.Add ( SH_FIELD_SUPP );
		array.Add ( SH_FIELD_CUSTOM1 );
		array.Add ( SH_FIELD_CUSTOM2 );
		array.Add ( SH_FIELD_CUSTOM3 );
		array.Add ( SH_FIELD_CUSTOM4 );
		array.Add ( SH_FIELD_CUSTOM5 );
		break;
	}
}

/**********************************************************************/

bool CStockReportSortOrderCSVRecord::CanBeCopied()
{
	return TRUE;
}

/**********************************************************************/

bool CStockReportSortOrderCSVRecord::CreateCopy( CStockReportSortOrderCSVRecord& copy )
{
	if ( CanBeCopied() == FALSE )
		return FALSE;

	copy.ClearRecord();
	copy.SetSortOrderType( SH_CUSTOM );
	copy.SetName( GetListName() );
	
	CWordArray arrayFields;
	GetSortOrderFieldsExternal( arrayFields );
	copy.SetSortOrderFields( arrayFields );
	return TRUE;
}

/**********************************************************************/

void CStockReportSortOrderCSVRecord::GetSortOrderFieldsExternal( CWordArray& arrayFields )
{
	arrayFields.RemoveAll();

	switch ( GetSortOrderType() )
	{
	case SH_CUSTOM:
		GetSortOrderFieldsInternal( arrayFields );
		break;

	case SH_STKLVL_NONE:		
		break;

	case SH_STKLVL_SUPP:
		arrayFields.Add( SH_FIELD_SUPP + SH_FIELD_HEADER + SH_FIELD_TOTAL );
		break;
	
	case SH_STKLVL_SUPP_SP:	
		arrayFields.Add( SH_FIELD_SUPP + SH_FIELD_HEADER + SH_FIELD_TOTAL );
		arrayFields.Add( SH_FIELD_SP + SH_FIELD_HEADER + SH_FIELD_TOTAL );
		break;
	
	case SH_STKLVL_SUPP_SP_CAT:
		arrayFields.Add( SH_FIELD_SUPP + SH_FIELD_HEADER + SH_FIELD_TOTAL );
		arrayFields.Add( SH_FIELD_SP + SH_FIELD_HEADER + SH_FIELD_TOTAL );
		arrayFields.Add( SH_FIELD_CAT_ITEM + SH_FIELD_HEADER + SH_FIELD_TOTAL );
		break;

	case SH_STKLVL_SUPP_CAT:
		arrayFields.Add( SH_FIELD_SUPP + SH_FIELD_HEADER + SH_FIELD_TOTAL );
		arrayFields.Add( SH_FIELD_CAT_ITEM + SH_FIELD_HEADER + SH_FIELD_TOTAL );
		break;

	case SH_STKLVL_SUPP_CAT_SP:
		arrayFields.Add( SH_FIELD_SUPP + SH_FIELD_HEADER + SH_FIELD_TOTAL );
		arrayFields.Add( SH_FIELD_CAT_ITEM + SH_FIELD_HEADER + SH_FIELD_TOTAL );
		arrayFields.Add( SH_FIELD_SP + SH_FIELD_HEADER + SH_FIELD_TOTAL );
		break;

	case SH_STKLVL_SP:
		arrayFields.Add( SH_FIELD_SP + SH_FIELD_HEADER + SH_FIELD_TOTAL );
		break;
	
	case SH_STKLVL_SP_SUPP:	
		arrayFields.Add( SH_FIELD_SP + SH_FIELD_HEADER + SH_FIELD_TOTAL );
		arrayFields.Add( SH_FIELD_SUPP + SH_FIELD_HEADER + SH_FIELD_TOTAL );
		break;

	case SH_STKLVL_SP_SUPP_CAT:
		arrayFields.Add( SH_FIELD_SP + SH_FIELD_HEADER + SH_FIELD_TOTAL );
		arrayFields.Add( SH_FIELD_SUPP + SH_FIELD_HEADER + SH_FIELD_TOTAL );
		arrayFields.Add( SH_FIELD_CAT_ITEM + SH_FIELD_HEADER + SH_FIELD_TOTAL );
		break;

	case SH_STKLVL_SP_CAT:
		arrayFields.Add( SH_FIELD_SP + SH_FIELD_HEADER + SH_FIELD_TOTAL );
		arrayFields.Add( SH_FIELD_CAT_ITEM + SH_FIELD_HEADER + SH_FIELD_TOTAL );
		break;

	case SH_STKLVL_SP_CAT_SUPP:	
		arrayFields.Add( SH_FIELD_SP + SH_FIELD_HEADER + SH_FIELD_TOTAL );
		arrayFields.Add( SH_FIELD_CAT_ITEM + SH_FIELD_HEADER + SH_FIELD_TOTAL );
		arrayFields.Add( SH_FIELD_SUPP + SH_FIELD_HEADER + SH_FIELD_TOTAL );
		break;

	case SH_STKLVL_CAT:
		arrayFields.Add( SH_FIELD_CAT_ITEM + SH_FIELD_HEADER + SH_FIELD_TOTAL );
		break;

	case SH_STKLVL_CAT_SUPP:
		arrayFields.Add( SH_FIELD_CAT_ITEM + SH_FIELD_HEADER + SH_FIELD_TOTAL );
		arrayFields.Add( SH_FIELD_SUPP + SH_FIELD_HEADER + SH_FIELD_TOTAL );
		break;

	case SH_STKLVL_CAT_SUPP_SP:
		arrayFields.Add( SH_FIELD_CAT_ITEM + SH_FIELD_HEADER + SH_FIELD_TOTAL );
		arrayFields.Add( SH_FIELD_SUPP + SH_FIELD_HEADER + SH_FIELD_TOTAL );
		arrayFields.Add( SH_FIELD_SP + SH_FIELD_HEADER + SH_FIELD_TOTAL );
		break;

	case SH_STKLVL_CAT_SP:
		arrayFields.Add( SH_FIELD_CAT_ITEM + SH_FIELD_HEADER + SH_FIELD_TOTAL );
		arrayFields.Add( SH_FIELD_SP + SH_FIELD_HEADER + SH_FIELD_TOTAL );
		break;

	case SH_STKLVL_CAT_SP_SUPP:
		arrayFields.Add( SH_FIELD_CAT_ITEM + SH_FIELD_HEADER + SH_FIELD_TOTAL );
		arrayFields.Add( SH_FIELD_SP + SH_FIELD_HEADER + SH_FIELD_TOTAL );
		arrayFields.Add( SH_FIELD_SUPP + SH_FIELD_HEADER + SH_FIELD_TOTAL );
		break;
	}	
}
	
/**********************************************************************/

void CStockReportSortOrderCSVRecord::GetSortOrderFieldsInternal( CWordArray& arrayTemp )
{
	arrayTemp.RemoveAll();
	for ( int nIndex = 0; nIndex < m_arrayFields.GetSize(); nIndex++ )
		arrayTemp.Add( m_arrayFields.GetAt( nIndex ) );
}

/**********************************************************************/

bool CStockReportSortOrderCSVRecord::HasField( int nField )
{
	bool bResult = FALSE;
	
	CWordArray arrayTemp;
	GetSortOrderFieldsInternal( arrayTemp );

	for ( int nPos = 0; nPos < arrayTemp.GetSize(); nPos++ )
	{
		int nArrayField = arrayTemp.GetAt( nPos ) % 100;
		if ( nArrayField == nField )
		{
			bResult = TRUE;
			break;
		}
	}
	
	return bResult;
}

/**********************************************************************/
			
CStockReportSortOrderCSVArray::CStockReportSortOrderCSVArray() : CSharedCSVArray()
{
}

/**********************************************************************/

const char* CStockReportSortOrderCSVArray::GetName ( int nIndex )
{
	m_strName = "";
	CString strTemp = GetAt ( nIndex );
	
	if ( strTemp != "" )
	{
		CCSV csv ( strTemp, ',', '"', 1 );
		m_strName = csv.GetString ( 0 );
	}

	return m_strName;
}

/**********************************************************************/

int CStockReportSortOrderCSVArray::GetSortOrderType ( int nIndex )
{
	CString strTemp = GetAt ( nIndex );
	
	if ( strTemp != "" )
	{
		CCSV csv ( strTemp, ',', '"', 2 );
		return csv.GetInt ( 1 );
	}

	return 0;
}

/**********************************************************************/

void CStockReportSortOrderCSVArray::LoadAdhocSortOrder( CStockReportSortOrderCSVRecord& SortOrderRecord )
{
	SortOrderRecord.SetName( m_adhocSortOrder.GetActualName() );
	SortOrderRecord.SetSortOrderType(0);

	CWordArray TempArray;
	m_adhocSortOrder.GetSortOrderFieldsInternal( TempArray );
	SortOrderRecord.SetSortOrderFields( TempArray );
}

/**********************************************************************/

void CStockReportSortOrderCSVArray::SaveAdhocSortOrder( CStockReportSortOrderCSVRecord& SortOrderRecord )
{
	m_adhocSortOrder.SetName( SortOrderRecord.GetActualName() );
	m_adhocSortOrder.SetSortOrderType(0);
	
	CWordArray TempArray;
	SortOrderRecord.GetSortOrderFieldsInternal( TempArray );
	m_adhocSortOrder.SetSortOrderFields( TempArray );
}

/**********************************************************************/

int CStockReportSortOrderCSVArray::Open( const char* szFilename, int nMode )
{
	RemoveAt( 0, GetSize() );

	int nReply = CSharedCSVArray::Open ( szFilename, nMode );
	
	if ( nReply != DB_ERR_SHARE )
	{
		if ( nMode == DB_READONLY )	// if file does not exist use default values
			nReply = DB_ERR_NONE;

		if ( GetSize() == 0 )
		{
			CWordArray ReportList;
			GetSystemSortOrderList( ReportList );

			for ( int n = 0; n < ReportList.GetSize(); n++ )
			{
				CStockReportSortOrderCSVRecord Record;
				Record.SetSortOrderType( ReportList.GetAt(n) );
				Add( Record );
			}
		}

		for ( int n = GetSize() - 1; n >=0; n-- )
			if ( AllowSortOrder(GetSortOrderType(n)) == FALSE )
				RemoveAt(n);
	}

	return nReply;
}

/**********************************************************************/

bool CStockReportSortOrderCSVArray::AllowSortOrder( int nSortOrderType )
{
	int nActiveSortOrderType = DataManagerNonDb.GetActiveStockReportSortOrderType();
	switch( nActiveSortOrderType )
	{
	case STOCKREPORT_SORTORDER_DELIVERY:
	case STOCKREPORT_SORTORDER_RETURNS:
		switch( nSortOrderType )
		{
		case SH_CUSTOM:				
		case SH_STKLVL_NONE:
		case SH_STKLVL_SP:
		case SH_STKLVL_SP_CAT:
		case SH_STKLVL_CAT:
		case SH_STKLVL_CAT_SP:
			return TRUE;

		default:
			return FALSE;
		}
		break;

	case STOCKREPORT_SORTORDER_SUPPLIER:
		switch( nSortOrderType )
		{
		case SH_CUSTOM:
		case SH_STKLVL_SUPP:
		case SH_STKLVL_SUPP_SP:
		case SH_STKLVL_SUPP_SP_CAT:	
		case SH_STKLVL_SUPP_CAT:
		case SH_STKLVL_SUPP_CAT_SP:
			return TRUE;

		default:
			return FALSE;
		}
		break;

	case STOCKREPORT_SORTORDER_REORDER:
		switch( nSortOrderType )
		{
		case SH_CUSTOM:
		case SH_STKLVL_SUPP:
		case SH_STKLVL_SUPP_SP:
		case SH_STKLVL_SUPP_CAT:
		case SH_STKLVL_SUPP_CAT_SP:
			return TRUE;

		default:
			return FALSE;
		}
		break;

	case STOCKREPORT_SORTORDER_PRICE_LIST:
	case STOCKREPORT_SORTORDER_TRANSFER_PERIOD:
	case STOCKTAKE_SORTORDER_TRANSFER_CLOSING:
		switch( nSortOrderType )
		{
		case SH_CUSTOM:				
		case SH_STKLVL_NONE:
		case SH_STKLVL_SUPP:
		case SH_STKLVL_SUPP_CAT:
		case SH_STKLVL_CAT:
		case SH_STKLVL_CAT_SUPP:
			return TRUE;

		default:
			return FALSE;
		}
		break;

	case STOCKREPORT_SORTORDER_SALES:
	case STOCKREPORT_SORTORDER_TOP_100_SALES:
	case STOCKREPORT_SORTORDER_ESTIMATED_PROFIT:
	case STOCKREPORT_SORTORDER_APPARENT_QTY:
	case STOCKREPORT_SORTORDER_APPARENT_QTY_DU:
	case STOCKREPORT_SORTORDER_APPARENT_SUMMARY:
	case STOCKREPORT_SORTORDER_APPARENT_SUMMARY_DU:
	case STOCKREPORT_SORTORDER_TOP_100_APPARENT:
	case STOCKREPORT_SORTORDER_APPARENT_VALUE:
	case STOCKREPORT_SORTORDER_OVERSTOCK_VALUE:
	case STOCKREPORT_SORTORDER_OPENING_VALUE:
	case STOCKREPORT_SORTORDER_ADJUST:
	case STOCKREPORT_SORTORDER_SHEET:
	case STOCKREPORT_SORTORDER_RETAIL_SUMMARY:
	case STOCKREPORT_SORTORDER_COST_SUMMARY:
	case STOCKREPORT_SORTORDER_TRANSFER_VALUE:
	case STOCKTAKE_SORTORDER_CLOSING_VALUE:
	case STOCKTAKE_SORTORDER_VARIANCE:
	case STOCKTAKE_SORTORDER_ESTIMATE_ALLOWANCE:
	case STOCKTAKE_SORTORDER_RECONCILE:
	case STOCKTAKE_SORTORDER_RECONCILE_CATEGORY:
	case STOCKTAKE_SORTORDER_SHEET:			
	default:
		switch( nSortOrderType )
		{
		case SH_CUSTOM:				
		case SH_STKLVL_SUPP:
		case SH_STKLVL_SUPP_SP:
		case SH_STKLVL_SUPP_SP_CAT:	
		case SH_STKLVL_SUPP_CAT:
		case SH_STKLVL_SUPP_CAT_SP:
		case SH_STKLVL_SP:
		case SH_STKLVL_SP_SUPP:
		case SH_STKLVL_SP_SUPP_CAT:
		case SH_STKLVL_SP_CAT:
		case SH_STKLVL_SP_CAT_SUPP:
		case SH_STKLVL_CAT:
		case SH_STKLVL_CAT_SUPP:
		case SH_STKLVL_CAT_SUPP_SP:
		case SH_STKLVL_CAT_SP:
		case SH_STKLVL_CAT_SP_SUPP:
			return TRUE;

		case SH_STKLVL_NONE:
			return ( nActiveSortOrderType != STOCKTAKE_SORTORDER_RECONCILE_CATEGORY );

		default:
			return FALSE;
		}
		break;
	}
}

/**********************************************************************/

void CStockReportSortOrderCSVArray::GetSystemSortOrderList( CWordArray& array )
{
	array.RemoveAll();

	int nActiveSortOrderType = DataManagerNonDb.GetActiveStockReportSortOrderType();
	switch( nActiveSortOrderType )
	{
	case STOCKREPORT_SORTORDER_DELIVERY:
	case STOCKREPORT_SORTORDER_RETURNS:
		array.Add( SH_STKLVL_NONE );		
		array.Add( SH_STKLVL_SP );			
		array.Add( SH_STKLVL_SP_CAT );		
		array.Add( SH_STKLVL_CAT );			
		array.Add( SH_STKLVL_CAT_SP );		
		break;

	case STOCKREPORT_SORTORDER_SUPPLIER:
		array.Add( SH_STKLVL_SUPP );		
		array.Add( SH_STKLVL_SUPP_SP );		
		array.Add( SH_STKLVL_SUPP_SP_CAT );	
		array.Add( SH_STKLVL_SUPP_CAT );	
		array.Add( SH_STKLVL_SUPP_CAT_SP );	
		break;

	case STOCKREPORT_SORTORDER_REORDER:
		array.Add( SH_STKLVL_SUPP );		
		array.Add( SH_STKLVL_SUPP_SP );		
		array.Add( SH_STKLVL_SUPP_CAT );	
		array.Add( SH_STKLVL_SUPP_CAT_SP );	
		break;

	case STOCKREPORT_SORTORDER_PRICE_LIST:
	case STOCKREPORT_SORTORDER_TRANSFER_PERIOD:
	case STOCKTAKE_SORTORDER_TRANSFER_CLOSING:
		array.Add( SH_STKLVL_NONE );		
		array.Add( SH_STKLVL_SUPP );		
		array.Add( SH_STKLVL_SUPP_CAT );	
		array.Add( SH_STKLVL_CAT );			
		array.Add( SH_STKLVL_CAT_SUPP );	
		break;

	case STOCKREPORT_SORTORDER_SALES:
	case STOCKREPORT_SORTORDER_TOP_100_SALES:
	case STOCKREPORT_SORTORDER_ESTIMATED_PROFIT:
	case STOCKREPORT_SORTORDER_APPARENT_QTY:
	case STOCKREPORT_SORTORDER_APPARENT_QTY_DU:
	case STOCKREPORT_SORTORDER_APPARENT_SUMMARY:
	case STOCKREPORT_SORTORDER_APPARENT_SUMMARY_DU:
	case STOCKREPORT_SORTORDER_TOP_100_APPARENT:
	case STOCKREPORT_SORTORDER_APPARENT_VALUE:
	case STOCKREPORT_SORTORDER_OVERSTOCK_VALUE:
	case STOCKREPORT_SORTORDER_OPENING_VALUE:
	case STOCKREPORT_SORTORDER_ADJUST:
	case STOCKREPORT_SORTORDER_SHEET:
	case STOCKREPORT_SORTORDER_RETAIL_SUMMARY:
	case STOCKREPORT_SORTORDER_COST_SUMMARY:
	case STOCKREPORT_SORTORDER_TRANSFER_VALUE:
	case STOCKTAKE_SORTORDER_CLOSING_VALUE:
	case STOCKTAKE_SORTORDER_VARIANCE:
	case STOCKTAKE_SORTORDER_ESTIMATE_ALLOWANCE:
	case STOCKTAKE_SORTORDER_RECONCILE:
	case STOCKTAKE_SORTORDER_RECONCILE_CATEGORY:
	case STOCKTAKE_SORTORDER_SHEET:			
	default:
		
		if ( nActiveSortOrderType != STOCKTAKE_SORTORDER_RECONCILE_CATEGORY )
			array.Add( SH_STKLVL_NONE );		
		
		array.Add( SH_STKLVL_SUPP );		
		array.Add( SH_STKLVL_SUPP_SP );		
		array.Add( SH_STKLVL_SUPP_SP_CAT );	
		array.Add( SH_STKLVL_SUPP_CAT );	
		array.Add( SH_STKLVL_SUPP_CAT_SP );	
		array.Add( SH_STKLVL_SP );			
		array.Add( SH_STKLVL_SP_SUPP );		
		array.Add( SH_STKLVL_SP_SUPP_CAT );	
		array.Add( SH_STKLVL_SP_CAT );		
		array.Add( SH_STKLVL_SP_CAT_SUPP );	
		array.Add( SH_STKLVL_CAT );			
		array.Add( SH_STKLVL_CAT_SUPP );	
		array.Add( SH_STKLVL_CAT_SUPP_SP );	
		array.Add( SH_STKLVL_CAT_SP );		
		array.Add( SH_STKLVL_CAT_SP_SUPP );
		break;
	}
}

/**********************************************************************/

void CStockReportSortOrderCSVArray::AddSortOrder( int nType )
{
	CStockReportSortOrderCSVRecord record;
	record.SetSortOrderType( nType );
	Add( record );
}

/**********************************************************************/

void CStockReportSortOrderCSVArray::CopyFrom( CStockReportSortOrderCSVArray& Source )
{
	RemoveAt( 0, GetSize() );

	for ( int n = 0; n < Source.GetSize(); n++ )
	{
		CStockReportSortOrderCSVRecord record;
		Source.GetAt( n, record );
		Add( record );
	}
}

/**********************************************************************/

void CStockReportSortOrderCSVArray::GetReportName( int nSortOrderType, CString& strName )
{
	switch( nSortOrderType )
	{
	case STOCKREPORT_SORTORDER_SALES:				strName = "Sales Breakdown";			break;
	case STOCKREPORT_SORTORDER_TOP_100_SALES:		strName = "Top/Bottom Sellers";			break;
	case STOCKREPORT_SORTORDER_ESTIMATED_PROFIT:	strName = "Estimated Profit";			break;
	case STOCKREPORT_SORTORDER_REORDER:				strName = "Stock Reorder";				break;
	case STOCKREPORT_SORTORDER_APPARENT_QTY:		strName = "Apparent Stock";				break;
	case STOCKREPORT_SORTORDER_APPARENT_QTY_DU:		strName = "Apparent Stock by DU";		break;
	case STOCKREPORT_SORTORDER_APPARENT_SUMMARY:	strName = "Apparent Stock Summary";		break;
	case STOCKREPORT_SORTORDER_APPARENT_SUMMARY_DU:	strName = "Apparent Stock Summary by DU";break;
	case STOCKREPORT_SORTORDER_TOP_100_APPARENT:	strName = "Top/Bottom Apparent";		break;
	case STOCKREPORT_SORTORDER_APPARENT_VALUE:		strName = "Apparent Stock Valuation";	break;
	case STOCKREPORT_SORTORDER_OVERSTOCK_VALUE:		strName = "Overstock Valuation";		break;
	case STOCKREPORT_SORTORDER_OPENING_VALUE:		strName = "Opening Stock Valuation";	break;
	case STOCKREPORT_SORTORDER_DELIVERY:			strName = "Delivery Summary";			break;
	case STOCKREPORT_SORTORDER_RETURNS:				strName = "Returns Summary";			break;
	case STOCKREPORT_SORTORDER_ADJUST:				strName = "Adjustment Summary";			break;
	case STOCKREPORT_SORTORDER_PRICE_LIST:			strName = "Stock Price List";			break;
	case STOCKREPORT_SORTORDER_RETAIL_SUMMARY:		strName = "Retail Summary";				break;
	case STOCKREPORT_SORTORDER_COST_SUMMARY:		strName = "Cost Summary";				break;
	case STOCKREPORT_SORTORDER_TRANSFER_VALUE:		strName = "Transfer Valuation";			break;
	case STOCKREPORT_SORTORDER_TRANSFER_PERIOD:		strName = "Transfer Stock Detail";		break;
	case STOCKREPORT_SORTORDER_SUPPLIER:			strName = "Stock Items by Supplier";	break;
	case STOCKREPORT_SORTORDER_SHEET:
	case STOCKTAKE_SORTORDER_SHEET:					strName = "Stock Sheet";				break;
	case STOCKTAKE_SORTORDER_CLOSING_VALUE:			strName = "Closing Stock Valuation";	break;
	case STOCKTAKE_SORTORDER_VARIANCE:				strName = "Closing Stock Variance";		break;
	case STOCKTAKE_SORTORDER_ESTIMATE_ALLOWANCE:	strName = "Estimated Allowance";		break;
	case STOCKTAKE_SORTORDER_RECONCILE:				strName = "Item Reconciliation";		break;
	case STOCKTAKE_SORTORDER_RECONCILE_CATEGORY:	strName = "Category Reconciliation";	break;
	case STOCKTAKE_SORTORDER_TRANSFER_CLOSING:		strName = "Stock Transfer Detail";		break;
	default:										strName = "Unknown";					break;
	}
}

/**********************************************************************/
