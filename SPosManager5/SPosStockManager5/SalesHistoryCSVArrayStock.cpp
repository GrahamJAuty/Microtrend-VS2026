/**********************************************************************/
#include "SysInfo.h"
/**********************************************************************/
#include "SalesHistoryCSVArrayStock.h"
/**********************************************************************/

CSalesHistoryCSVRecordStock::CSalesHistoryCSVRecordStock()
{
	ClearRecord();	
}

/**********************************************************************/

void CSalesHistoryCSVRecordStock::ClearRecord()
{
	m_strName = "";
	m_nReportType = SH_CUSTOM;
	m_nReportMode = SALESHISTORY_REPORTMODE_NORMAL;
	m_bIsBottom100 = FALSE;
	m_bIsSortedByValue = FALSE;
	m_nChartType = CHART_TYPE_MAXITEMS;
	m_nChartSize = 100;
	m_dChartCutOff = 0.0;
	m_arrayFields.RemoveAll();
}

/**********************************************************************/

void CSalesHistoryCSVRecordStock::ConvertFromCSV ( CCSV& csv )
{
	ClearRecord();
	SetName ( csv.GetString (0) );
	SetReportType ( csv.GetInt(1) ); 

	switch ( csv.GetInt(2) )
	{
	case 1:	V1ConvertFromCSV ( csv );		break;
	case 2:	V2to3ConvertFromCSV ( csv, 2 );	break;
	case 3:	V2to3ConvertFromCSV ( csv, 3 );	break;
	}
}

/**********************************************************************/

void CSalesHistoryCSVRecordStock::V1ConvertFromCSV ( CCSV& csv )
{
	SetReportMode( csv.GetBool(3) ? SALESHISTORY_REPORTMODE_CHART : SALESHISTORY_REPORTMODE_NORMAL );
	SetBottom100Flag( csv.GetBool(4) );
	SetSortedByValueFlag( csv.GetBool(5) );
	SetChartSize( csv.GetInt(6) );
	
	if ( m_nReportType == SH_CUSTOM )
	{
		int nCount = csv.GetInt(7);
		int nOffset = 8;
		for ( int nIndex = 0; nIndex < nCount; nIndex++ )
			m_arrayFields.Add( csv.GetInt( nOffset++ ) );
	}
}

/**********************************************************************/

void CSalesHistoryCSVRecordStock::V2to3ConvertFromCSV ( CCSV& csv, int nVer )
{
	if ( nVer <= 2 )
		SetReportMode( csv.GetBool(3) ? SALESHISTORY_REPORTMODE_CHART : SALESHISTORY_REPORTMODE_NORMAL );
	else
		SetReportMode( csv.GetInt(3) );

	SetBottom100Flag( csv.GetBool(4) );
	SetSortedByValueFlag( csv.GetBool(5) );
	SetChartType( csv.GetInt(6) );
	SetChartSize( csv.GetInt(7) );
	SetChartCutOff( csv.GetDouble(8) );

	if ( m_nReportType == SH_CUSTOM )
	{
		int nCount = csv.GetInt(9);
		int nOffset = 10;
		for ( int nIndex = 0; nIndex < nCount; nIndex++ )
			m_arrayFields.Add( csv.GetInt( nOffset++ ) );
	}
}

/**********************************************************************/
	
void CSalesHistoryCSVRecordStock::ConvertToCSV ( CCSV& csv )
{
	csv.Add ( m_strName );
	csv.Add ( m_nReportType );
	csv.Add ( SALESHISTORYSTOCK_VERSION );				
	csv.Add ( m_nReportMode );
	csv.Add ( m_bIsBottom100 );
	csv.Add ( m_bIsSortedByValue );
	csv.Add ( m_nChartType );
	csv.Add ( m_nChartSize );
	csv.Add ( m_dChartCutOff, 3 );
	
	if ( m_nReportType == SH_CUSTOM )
	{
		csv.Add( m_arrayFields.GetSize() );
		for ( int nIndex = 0; nIndex < m_arrayFields.GetSize(); nIndex++ )
			csv.Add ( m_arrayFields.GetAt ( nIndex ) );
	}
}

/**********************************************************************/

void CSalesHistoryCSVRecordStock::SetReportMode( int n )
{
	switch(n)
	{
	case SALESHISTORY_REPORTMODE_NORMAL:
	case SALESHISTORY_REPORTMODE_CHART:
	case SALESHISTORY_REPORTMODE_WEEKLY_VAL_FULL:
	case SALESHISTORY_REPORTMODE_WEEKLY_QTY:
	case SALESHISTORY_REPORTMODE_MONTHLY_VAL_FULL:
	case SALESHISTORY_REPORTMODE_MONTHLY_QTY:
		m_nReportMode = n;
		break;
	}
}

/**********************************************************************/

int CSalesHistoryCSVRecordStock::GetReportMode()
{
	switch( m_nReportType )
	{
	case SH_CUSTOM:
		return m_nReportMode;

	case SH_STK_TOP100:
	case SH_STK_TOP100_CAT:
	case SH_STK_TOP100_MONTH:
	case SH_STK_TOP100_MONTH_CAT:
		return SALESHISTORY_REPORTMODE_CHART;

	case SH_STK_CAT_BYWDAY:	
	case SH_STK_SP_BYWDAY:	
	case SH_STK_DAILY_BYWEEK:
		return SALESHISTORY_REPORTMODE_WEEKLY_VAL_FULL;

	default:
		return SALESHISTORY_REPORTMODE_NORMAL;
	}
}

/**********************************************************************/

void CSalesHistoryCSVRecordStock::SetChartType( int nType )
{
	switch( nType )
	{
	case CHART_TYPE_UNLIMITED:
	case CHART_TYPE_CUTOFF:
	case CHART_TYPE_MAXITEMS:
		m_nChartType = nType;
		break;
	}
}

/**********************************************************************/

void CSalesHistoryCSVRecordStock::SetChartSize( int nSize )
{
	if ( nSize >= 1 && nSize <= 9999 ) m_nChartSize = nSize;
}

/**********************************************************************/

void CSalesHistoryCSVRecordStock::SetReportFields( CWordArray& arrayTemp )
{
	m_arrayFields.RemoveAll();
	for ( int nIndex = 0; nIndex < arrayTemp.GetSize(); nIndex++ )
		m_arrayFields.Add( arrayTemp.GetAt( nIndex ) );
}

/**********************************************************************/

void CSalesHistoryCSVRecordStock::GetReportFields( CWordArray& arrayTemp )
{
	arrayTemp.RemoveAll();
	for ( int nIndex = 0; nIndex < m_arrayFields.GetSize(); nIndex++ )
		arrayTemp.Add( m_arrayFields.GetAt( nIndex ) );
}

/**********************************************************************/

const char* CSalesHistoryCSVRecordStock::GetKey( bool bPercent, bool bProfit, bool bApparent )
{
	CCSV csvKey ( '.' );
	
	CString strField;
	strField.Format ( "%3.3d", GetReportType() );
	strField += ( bPercent ) ? "1" : "0";
	strField += ( bProfit ) ? "1" : "0";
	strField += ( bApparent ) ? "1" : "0";
	csvKey.Add( strField );
	
	for ( int nPos = 0; nPos < m_arrayFields.GetSize(); nPos++ )
		csvKey.Add ( m_arrayFields.GetAt( nPos ) );

	m_strKey = csvKey.GetLine();	
	return m_strKey;
}

/**********************************************************************/

const char* CSalesHistoryCSVRecordStock::GetFieldListName( int nField )
{
	switch ( nField )
	{
	case SH_FIELD_MONTH:			m_strFieldListName = "Month";				break;
	case SH_FIELD_WEEK:				m_strFieldListName = "Week Starting";		break;
	case SH_FIELD_WEEKDAY:			m_strFieldListName = "Weekday";				break;
	case SH_FIELD_DATE_BUSINESS:	m_strFieldListName = "Full Date";			break;
	case SH_FIELD_TIME:				m_strFieldListName = "Time";				break;
	case SH_FIELD_DBASE:			m_strFieldListName = "Database";			break;
	case SH_FIELD_SITE:				m_strFieldListName = "Site";				break;
	case SH_FIELD_SP:				m_strFieldListName = "Stockpoint";			break;
	case SH_FIELD_CAT_SALE:			m_strFieldListName = "Category (Sale)";		break;
	case SH_FIELD_CAT_ITEM:			m_strFieldListName = "Category (Dbase)";	break;
	case SH_FIELD_CAT_FILTER:		m_strFieldListName = "Category (Filter)";	break;
	case SH_FIELD_SUPP:				m_strFieldListName = "Supplier Name";		break;
	case SH_FIELD_SUPPREF:			m_strFieldListName = "Supplier Reference";	break;
	case SH_FIELD_CODE:				m_strFieldListName = "Stockcode";			break;
	case SH_FIELD_DESCRIPTION:		m_strFieldListName = "Description";			break;
	case SH_FIELD_CUSTOM1:	
	case SH_FIELD_CUSTOM2:	
	case SH_FIELD_CUSTOM3:	
	case SH_FIELD_CUSTOM4:	
	case SH_FIELD_CUSTOM5:	
		m_strFieldListName = DataManagerNonDb.CustomFieldNames.GetName( nField + 1 - SH_FIELD_CUSTOM1 );	
		break;
	default:					m_strFieldListName = "Unknown";				break;
	}

	return m_strFieldListName;

}

/**********************************************************************/

const char* CSalesHistoryCSVRecordStock::GetListName()
{
	switch( m_nReportType )
	{
	case SH_CUSTOM:		m_strListName = m_strName;			break;
	case SH_STK_ITEM:		m_strListName = "Stock Item";			break;
	case SH_STK_TOP100:		m_strListName = "Top/Bottom Items";	break;	
	case SH_STK_UNSOLD:		m_strListName = "Unsold Items";		break;	
	case SH_STK_CAT:		m_strListName = "Category Summary";	break;	
	case SH_STK_CAT_BYWDAY:	m_strListName = "Category by Weekday";	break;	
	case SH_STK_SP_BYWDAY:	m_strListName = "Stockpoint by Weekday";break;	
	case SH_STK_MONTHLY:	m_strListName = "Monthly Summary";		break;		
	case SH_STK_WEEKLY:		m_strListName = "Weekly Summary";		break;	
	case SH_STK_DAILY:		m_strListName = "Daily Summary";		break;		
	case SH_STK_DAILY_BYWEEK:m_strListName = "Daily by Week";		break;	
	default:				m_strListName = "Unknown";			break;
	}
	return m_strListName;
}

/**********************************************************************/

const char* CSalesHistoryCSVRecordStock::GetReportName()
{
	switch ( m_nReportType )
	{
	case SH_CUSTOM:
		m_strReportName = m_strName;
		break;

	case SH_STK_ITEM:
	case SH_STK_ITEM_CAT:
	case SH_STK_ITEM_MONTH:
	case SH_STK_ITEM_MONTH_CAT:		
		m_strReportName = "Stock Sales History";	
		break;

	case SH_STK_UNSOLD:
	case SH_STK_UNSOLD_CAT:
	case SH_STK_UNSOLD_MONTH:
	case SH_STK_UNSOLD_MONTH_CAT:
		m_strReportName = "Unsold Stock Items";	
		break;

	case SH_STK_CAT:
	case SH_STK_CAT_MONTH:			
		m_strReportName = "Stock Category Sales History";	
		break;

	case SH_STK_CAT_BYWDAY:			
		m_strReportName = "Stock Category Sales History by Weekday";
		break;

	case SH_STK_SP_BYWDAY:			
		m_strReportName = "Stockpoint Sales History by Weekday";
		break;

	case SH_STK_DAILY:
		m_strReportName = "Stock Daily Sales History" ;
		break;

	case SH_STK_DAILY_BYWEEK:
		m_strReportName = "Stock Daily Sales History by Week" ;
		break;

	case SH_STK_WEEKLY:
		m_strReportName = "Stock Weekly Sales History";
		break;

	case SH_STK_MONTHLY:
		m_strReportName = "Stock Monthly Sales History";
		break;

	case SH_STK_TOP100:
	case SH_STK_TOP100_CAT:
	case SH_STK_TOP100_MONTH:
	case SH_STK_TOP100_MONTH_CAT:
		//THE NAME OF THESE REPORTS IS NOT KNOWN UNTIL WE FIND
		//OUT WHAT TYPE OF CHART IS NEEDED
		m_strReportName = "";
		break;

	default:
		m_strReportName = "Unknown";
		break;
	}
	
	return m_strReportName;
}

/**********************************************************************/

void CSalesHistoryCSVRecordStock::GetValidFieldList( CWordArray& array )
{
	array.RemoveAll();
	array.Add ( SH_FIELD_MONTH );
	array.Add ( SH_FIELD_WEEK );
	array.Add ( SH_FIELD_WEEKDAY );
	array.Add ( SH_FIELD_DATE_BUSINESS );
	array.Add ( SH_FIELD_TIME );
	array.Add ( SH_FIELD_DBASE );
	array.Add ( SH_FIELD_SITE );
	array.Add ( SH_FIELD_SP );
	array.Add ( SH_FIELD_CAT_SALE );
	array.Add ( SH_FIELD_CAT_ITEM );
	array.Add ( SH_FIELD_CAT_FILTER );

	if ( SysInfo.IsRetailStockSystem() == TRUE )
	{
		array.Add ( SH_FIELD_CUSTOM1 );
		array.Add ( SH_FIELD_CUSTOM2 );
		array.Add ( SH_FIELD_CUSTOM3 );
		array.Add ( SH_FIELD_CUSTOM4 );
		array.Add ( SH_FIELD_CUSTOM5 );
	}

	array.Add ( SH_FIELD_SUPP );
	array.Add ( SH_FIELD_SUPPREF );
	array.Add ( SH_FIELD_CODE );
	array.Add ( SH_FIELD_DESCRIPTION );
}

/**********************************************************************/

int CSalesHistoryCSVRecordStock::GetBaseReportType()
{
	switch ( m_nReportType )
	{
	case SH_STK_ITEM_CAT:
	case SH_STK_ITEM_MONTH:
	case SH_STK_ITEM_MONTH_CAT:
		return SH_STK_ITEM;

	case SH_STK_CAT_MONTH:
		return SH_STK_CAT;

	case SH_STK_TOP100_CAT:
	case SH_STK_TOP100_MONTH:
	case SH_STK_TOP100_MONTH_CAT:
		return SH_STK_TOP100;

	case SH_STK_UNSOLD_CAT:
	case SH_STK_UNSOLD_MONTH:
	case SH_STK_UNSOLD_MONTH_CAT:
		return SH_STK_UNSOLD;

	default:
		return m_nReportType;
	}
}

/**********************************************************************/

bool CSalesHistoryCSVRecordStock::CanDoMonthlyBreakdown()
{
	bool bResult = FALSE;
		
	switch ( GetBaseReportType() )
	{
	case SH_STK_ITEM:
	case SH_STK_TOP100:
	case SH_STK_UNSOLD:
	case SH_STK_CAT:
		bResult = TRUE;
	}

	return bResult;
}

/**********************************************************************/

bool CSalesHistoryCSVRecordStock::CanDoCategorySort()
{
	bool bResult = FALSE;

	switch ( GetBaseReportType() )
	{
	case SH_STK_ITEM:
	case SH_STK_TOP100:
	case SH_STK_UNSOLD:
		bResult = TRUE;
		break;
	}
	
	return bResult;
}

/**********************************************************************/

bool CSalesHistoryCSVRecordStock::CanDoEstimatedProfit()
{
	bool bResult = FALSE;

	switch ( GetBaseReportType() )
	{
	case SH_STK_ITEM:
	case SH_STK_TOP100:
	case SH_STK_CAT:
	case SH_STK_DAILY:
	case SH_STK_WEEKLY:
	case SH_STK_MONTHLY:
	case SH_STK_CAT_BYWDAY:	
	case SH_STK_SP_BYWDAY:	
	case SH_STK_DAILY_BYWEEK:
	case SH_CUSTOM:	
		bResult = TRUE;
		break;
	}
	
	return bResult;
}

/**********************************************************************/

bool CSalesHistoryCSVRecordStock::CanDoPercentSale()
{
	bool bResult = FALSE;

	switch ( GetBaseReportType() )
	{
	case SH_CUSTOM:
	case SH_STK_TOP100:
	case SH_STK_ITEM:
	case SH_STK_CAT:
	case SH_STK_MONTHLY:
	case SH_STK_WEEKLY:
	case SH_STK_DAILY:
	case SH_STK_CAT_BYWDAY:	
	case SH_STK_SP_BYWDAY:	
	case SH_STK_DAILY_BYWEEK:
		bResult = TRUE;
		break;
	}

	return bResult;
}

/**********************************************************************/

bool CSalesHistoryCSVRecordStock::CanDoGraph()
{
	bool bResult = FALSE;

	switch ( GetBaseReportType() )
	{
	case SH_STK_CAT:
	case SH_STK_DAILY:
	case SH_STK_WEEKLY:
	case SH_STK_MONTHLY:
		bResult = TRUE;
		break;
	}

	return bResult;
}

/**********************************************************************/

bool CSalesHistoryCSVRecordStock::CanBeCopied()
{
	bool bResult = FALSE;

	switch ( GetBaseReportType() )
	{
	case SH_STK_ITEM:
	case SH_STK_TOP100:
	case SH_STK_CAT:
	case SH_STK_MONTHLY:
	case SH_STK_WEEKLY:
	case SH_STK_DAILY:
	case SH_STK_CAT_BYWDAY:	
	case SH_STK_SP_BYWDAY:	
	case SH_STK_DAILY_BYWEEK:
	case SH_CUSTOM:
		bResult = TRUE;
		break;
	}

	return bResult;
}

/**********************************************************************/

bool CSalesHistoryCSVRecordStock::CanDoApparent()
{
	switch ( GetBaseReportType() )
	{
	case SH_STK_ITEM:
	case SH_STK_TOP100:
		return TRUE;
		
	case SH_CUSTOM:
		return HasStockCodeField();
	
	default:
		return FALSE;
	}
}

/**********************************************************************/

bool CSalesHistoryCSVRecordStock::CreateCopy( CSalesHistoryCSVRecordStock& copy )
{
	if ( CanBeCopied() == FALSE )
		return FALSE;

	copy.ClearRecord();
	copy.SetReportType( SH_CUSTOM );
	copy.SetName( GetListName() );
	copy.SetReportMode( SALESHISTORY_REPORTMODE_NORMAL );
	
	CWordArray arrayFields;
		
	switch ( GetBaseReportType() )
	{
	case SH_CUSTOM:
		GetReportFields( arrayFields );
		copy.SetReportMode( GetReportMode() );
		break;

	case SH_STK_ITEM:
		arrayFields.Add( SH_FIELD_CODE );
		arrayFields.Add( SH_FIELD_CAT_FILTER );
		break;

	case SH_STK_TOP100:
		arrayFields.Add( SH_FIELD_CODE );
		arrayFields.Add( SH_FIELD_CAT_FILTER );
		copy.SetReportMode( SALESHISTORY_REPORTMODE_CHART );
		break;

	case SH_STK_CAT:
		arrayFields.Add( SH_FIELD_CAT_FILTER );
		break;

	case SH_STK_DAILY:
		arrayFields.Add( SH_FIELD_DATE_BUSINESS );
		break;

	case SH_STK_MONTHLY:
		arrayFields.Add( SH_FIELD_MONTH );
		break;

	case SH_STK_WEEKLY:
		arrayFields.Add( SH_FIELD_WEEK );
		break;

	case SH_STK_CAT_BYWDAY:	
		arrayFields.Add( SH_FIELD_CAT_FILTER );
		copy.SetReportMode( SALESHISTORY_REPORTMODE_WEEKLY_VAL_FULL );
		break;

	case SH_STK_SP_BYWDAY:
		arrayFields.Add( SH_FIELD_SP );
		copy.SetReportMode( SALESHISTORY_REPORTMODE_WEEKLY_VAL_FULL );
		break;

	case SH_STK_DAILY_BYWEEK:
		arrayFields.Add( SH_FIELD_WEEK );
		copy.SetReportMode( SALESHISTORY_REPORTMODE_WEEKLY_VAL_FULL );
		break;
	}

	copy.SetReportFields( arrayFields );
	return TRUE;
}
	
/**********************************************************************/

bool CSalesHistoryCSVRecordStock::HasField( int nField )
{
	CWordArray arrayTemp;
	GetReportFields( arrayTemp );

	for ( int nPos = 0; nPos < arrayTemp.GetSize(); nPos++ )
		if ( ( arrayTemp.GetAt( nPos ) % 100 ) == nField )
			return TRUE;

	return FALSE;
}

/**********************************************************************/

bool CSalesHistoryCSVRecordStock::HasStockCodeField()
{
	return HasField ( SH_FIELD_CODE );
}

/**********************************************************************/
			
CSalesHistoryCSVArrayStock::CSalesHistoryCSVArrayStock() : CSharedCSVArray()
{
}

/**********************************************************************/

int CSalesHistoryCSVArrayStock::Open( const char* szFilename, int nMode )
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
			GetSystemReportList( ReportList );

			for ( int n = 0; n < ReportList.GetSize(); n++ )
			{
				CSalesHistoryCSVRecordStock Record;
				Record.SetReportType( ReportList.GetAt(n) );
				Add( Record );
			}
		}

		for ( int n = GetSize() - 1; n >=0; n-- )
			if ( AllowReport(GetReportType(n)) == FALSE )
				RemoveAt(n);
	}

	return nReply;
}

/**********************************************************************/

const char* CSalesHistoryCSVArrayStock::GetName ( int nIndex )
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

int CSalesHistoryCSVArrayStock::GetReportType ( int nIndex )
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

void CSalesHistoryCSVArrayStock::LoadAdhocReport( CSalesHistoryCSVRecordStock& ReportRecord )
{
	ReportRecord.SetName( m_adhocReport.GetActualName() );
	ReportRecord.SetReportMode( m_adhocReport.GetReportMode() );
	ReportRecord.SetBottom100Flag( m_adhocReport.IsBottom100() );
	ReportRecord.SetSortedByValueFlag ( m_adhocReport.IsSortedByValue() );
	ReportRecord.SetChartSize( m_adhocReport.GetChartSize() );
	ReportRecord.SetReportType(0);

	CWordArray TempArray;
	m_adhocReport.GetReportFields( TempArray );
	ReportRecord.SetReportFields( TempArray );
}

/**********************************************************************/

void CSalesHistoryCSVArrayStock::SaveAdhocReport( CSalesHistoryCSVRecordStock& ReportRecord )
{
	m_adhocReport.SetName( ReportRecord.GetActualName() );
	m_adhocReport.SetReportType(0);
	m_adhocReport.SetReportMode( ReportRecord.GetReportMode() );
	m_adhocReport.SetBottom100Flag( ReportRecord.IsBottom100() );
	m_adhocReport.SetSortedByValueFlag( ReportRecord.IsSortedByValue() );
	m_adhocReport.SetChartSize( ReportRecord.GetChartSize() ); 

	CWordArray TempArray;
	ReportRecord.GetReportFields( TempArray );
	m_adhocReport.SetReportFields( TempArray );
}

/**********************************************************************/

bool CSalesHistoryCSVArrayStock::AllowReport( int nReportType )
{
	switch ( nReportType )
	{
	case SH_CUSTOM:
	case SH_STK_ITEM:
	case SH_STK_UNSOLD:
	case SH_STK_TOP100:
	case SH_STK_CAT:
	case SH_STK_CAT_BYWDAY:
	case SH_STK_SP_BYWDAY:
	case SH_STK_MONTHLY:
	case SH_STK_WEEKLY:
	case SH_STK_DAILY:
	case SH_STK_DAILY_BYWEEK:
		return TRUE;

	default:
		return FALSE;
	}
}

/**********************************************************************/

void CSalesHistoryCSVArrayStock::GetSystemReportList( CWordArray& array )
{
	array.RemoveAll();
	array.Add ( SH_STK_ITEM );
	array.Add ( SH_STK_UNSOLD );
	array.Add ( SH_STK_TOP100 );
	array.Add ( SH_STK_CAT );
	array.Add ( SH_STK_CAT_BYWDAY );
	array.Add ( SH_STK_SP_BYWDAY );
	array.Add ( SH_STK_MONTHLY );
	array.Add ( SH_STK_WEEKLY );
	array.Add ( SH_STK_DAILY );
	array.Add ( SH_STK_DAILY_BYWEEK );
}

/**********************************************************************/

void CSalesHistoryCSVArrayStock::CopyFrom( CSalesHistoryCSVArrayStock& Source )
{
	RemoveAt( 0, GetSize() );

	for ( int n = 0; n < Source.GetSize(); n++ )
	{
		CSalesHistoryCSVRecordStock record;
		Source.GetAt( n, record );
		Add( record );
	}
}

/**********************************************************************/
