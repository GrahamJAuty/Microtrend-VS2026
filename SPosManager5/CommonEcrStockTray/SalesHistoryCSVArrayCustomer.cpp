/**********************************************************************/
//#include "DealerFlags.h"
/**********************************************************************/
#include "SalesHistoryCSVArrayCustomer.h"
/**********************************************************************/

CSalesHistoryCSVRecordCustomer::CSalesHistoryCSVRecordCustomer()
{
	ClearRecord();	
}

/**********************************************************************/

void CSalesHistoryCSVRecordCustomer::ClearRecord()
{
	m_strName = "";
	m_nReportType = SH_CUSTOM;
	m_arrayFields.RemoveAll();
}

/**********************************************************************/

void CSalesHistoryCSVRecordCustomer::ConvertFromCSV ( CCSV& csv )
{
	ClearRecord();
	SetName ( csv.GetString (0) );
	SetReportType ( csv.GetInt(1) ); 

	switch ( csv.GetInt(2) )
	{
	case 1:	V1ConvertFromCSV ( csv );	break;
	}
}

/**********************************************************************/

void CSalesHistoryCSVRecordCustomer::V1ConvertFromCSV ( CCSV& csv )
{
	if ( m_nReportType == SH_CUSTOM )
	{
		int nCount = csv.GetInt(3);
		int nOffset = 4;
	
		bool bGotLoc = FALSE;
		for ( int nIndex = 0; nIndex < nCount; nIndex++ )
		{
			int nField = csv.GetInt(nOffset++);

			switch( nField % 100)
			{
			case SH_FIELD_LOC:
				bGotLoc = TRUE;
				m_arrayFields.Add( nField );
				break;

			case SH_FIELD_TERMINAL:
				if ( bGotLoc == TRUE )
					m_arrayFields.Add( nField );
				break;

			default:
				m_arrayFields.Add( nField );
				break;
			}
		}
	}
}

/**********************************************************************/
	
void CSalesHistoryCSVRecordCustomer::ConvertToCSV ( CCSV& csv )
{
	csv.Add ( m_strName );
	csv.Add ( m_nReportType );
	csv.Add ( SALESHISTORYCUSTOMER_VERSION );				
	
	if ( m_nReportType == SH_CUSTOM )
	{
		csv.Add( m_arrayFields.GetSize() );
		
		for ( int nIndex = 0; nIndex < m_arrayFields.GetSize(); nIndex++ )
			csv.Add ( m_arrayFields.GetAt ( nIndex ) );
	}
}

/**********************************************************************/

void CSalesHistoryCSVRecordCustomer::SetReportFields( CWordArray& arrayTemp )
{
	m_arrayFields.RemoveAll();
	for ( int nIndex = 0; nIndex < arrayTemp.GetSize(); nIndex++ )
		m_arrayFields.Add( arrayTemp.GetAt( nIndex ) );
}

/**********************************************************************/

void CSalesHistoryCSVRecordCustomer::GetReportFields( CWordArray& arrayTemp )
{
	arrayTemp.RemoveAll();
	for ( int nIndex = 0; nIndex < m_arrayFields.GetSize(); nIndex++ )
		arrayTemp.Add( m_arrayFields.GetAt( nIndex ) );
}

/**********************************************************************/

const char* CSalesHistoryCSVRecordCustomer::GetKey()
{
	CCSV csvKey ( '.' );
	
	CString strField;
	strField.Format ( "%3.3d", GetReportType() );
	csvKey.Add( strField );
	
	for ( int nPos = 0; nPos < m_arrayFields.GetSize(); nPos++ )
		csvKey.Add ( m_arrayFields.GetAt( nPos ) );

	m_strKey = csvKey.GetLine();	
	return m_strKey;
}

/**********************************************************************/

const char* CSalesHistoryCSVRecordCustomer::GetFieldListName( int nField )
{
	switch ( nField )
	{
	case SH_FIELD_MONTH:		m_strFieldListName = "Month";				break;
	case SH_FIELD_WEEK:			m_strFieldListName = "Week Starting";		break;
	case SH_FIELD_WEEKDAY:		m_strFieldListName = "Weekday";				break;
	case SH_FIELD_DATE_BUSINESS:	m_strFieldListName = "Full Date";			break;
	case SH_FIELD_TIME:			m_strFieldListName = "Time";				break;
	case SH_FIELD_TERMINAL:		m_strFieldListName = "Terminal No";			break;
	case SH_FIELD_TRANSERVER:	m_strFieldListName = "Server";				break;
	default:					m_strFieldListName = "Unknown";				break;
	}

	return m_strFieldListName;

}

/**********************************************************************/

const char* CSalesHistoryCSVRecordCustomer::GetListName()
{
	switch( m_nReportType )
	{
	case SH_CUSTOM:				m_strListName = m_strName;				break;
	case SH_CUST_SUMMARY:		m_strListName = "Customer Summary";		break;
	case SH_CUST_DETAIL:		m_strListName = "Customer Detail";		break;
	default:					m_strListName = "Unknown";				break;
	}
	return m_strListName;
}

/**********************************************************************/

const char* CSalesHistoryCSVRecordCustomer::GetReportName()
{
	switch ( m_nReportType )
	{
	case SH_CUSTOM:
		m_strReportName = m_strName;
		break;

	case SH_CUST_SUMMARY:
		m_strReportName = "Customer Summary";
		break;

	case SH_CUST_DETAIL:
		m_strReportName = "Customer Detail";
		break;

	default:
		m_strReportName = "Unknown";
		break;
	}
	
	return m_strReportName;
}

/**********************************************************************/

void CSalesHistoryCSVRecordCustomer::GetValidFieldList( CWordArray& array )
{
	array.RemoveAll();
	array.Add ( SH_FIELD_MONTH );
	array.Add ( SH_FIELD_WEEK );
	array.Add ( SH_FIELD_WEEKDAY );
	array.Add ( SH_FIELD_DATE_BUSINESS );
	array.Add ( SH_FIELD_TIME );
	array.Add ( SH_FIELD_TERMINAL );
	array.Add ( SH_FIELD_TRANSERVER );
}

/**********************************************************************/

bool CSalesHistoryCSVRecordCustomer::CanBeCopied()
{
	bool bResult = FALSE;

	switch ( GetReportType() )
	{
	case SH_CUST_SUMMARY:
	case SH_CUST_DETAIL:
		bResult = TRUE;
		break;
	}

	return bResult;
}

/**********************************************************************/

bool CSalesHistoryCSVRecordCustomer::CreateCopy( CSalesHistoryCSVRecordCustomer& copy )
{
	if ( CanBeCopied() == FALSE )
		return FALSE;

	copy.ClearRecord();
	copy.SetReportType( SH_CUSTOM );
	copy.SetName( GetListName() );
	
	CWordArray arrayFields;
		
	switch ( GetReportType() )
	{
	case SH_CUSTOM:
		GetReportFields( arrayFields );
		break;

	case SH_CUST_SUMMARY:
		break;

	case SH_CUST_DETAIL:
		arrayFields.Add( SH_FIELD_DATE_BUSINESS );
		arrayFields.Add( SH_FIELD_TIME );
		arrayFields.Add( SH_FIELD_TRANSERVER );
		break;
	}

	copy.SetReportFields( arrayFields );
	return TRUE;
}
	
/**********************************************************************/
			
CSalesHistoryCSVArrayCustomer::CSalesHistoryCSVArrayCustomer() : CSharedCSVArray()
{
}

/**********************************************************************/

const char* CSalesHistoryCSVArrayCustomer::GetName ( int nIndex )
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

int CSalesHistoryCSVArrayCustomer::GetReportType ( int nIndex )
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

void CSalesHistoryCSVArrayCustomer::LoadAdhocReport( CSalesHistoryCSVRecordCustomer& ReportRecord )
{
	ReportRecord.SetName( m_adhocReport.GetActualName() );
	ReportRecord.SetReportType(0);

	CWordArray TempArray;
	m_adhocReport.GetReportFields( TempArray );
	ReportRecord.SetReportFields( TempArray );
}

/**********************************************************************/

void CSalesHistoryCSVArrayCustomer::SaveAdhocReport( CSalesHistoryCSVRecordCustomer& ReportRecord )
{
	m_adhocReport.SetName( ReportRecord.GetActualName() );
	m_adhocReport.SetReportType(0);

	CWordArray TempArray;
	ReportRecord.GetReportFields( TempArray );
	m_adhocReport.SetReportFields( TempArray );
}

/**********************************************************************/

int CSalesHistoryCSVArrayCustomer::Open( const char* szFilename, int nMode )
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
				CSalesHistoryCSVRecordCustomer Record;
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

bool CSalesHistoryCSVArrayCustomer::AllowReport( int nReportType )
{
	switch ( nReportType )
	{
	case SH_CUSTOM:
	case SH_CUST_SUMMARY:
	case SH_CUST_DETAIL:
		return TRUE;

	default:
		return FALSE;
	}
}

/**********************************************************************/

void CSalesHistoryCSVArrayCustomer::GetSystemReportList( CWordArray& array )
{
	array.RemoveAll();
	array.Add ( SH_CUST_SUMMARY );
	array.Add ( SH_CUST_DETAIL );
}

/**********************************************************************/

void CSalesHistoryCSVArrayCustomer::CopyFrom( CSalesHistoryCSVArrayCustomer& Source )
{
	RemoveAt( 0, GetSize() );

	for ( int n = 0; n < Source.GetSize(); n++ )
	{
		CSalesHistoryCSVRecordCustomer record;
		Source.GetAt( n, record );
		Add( record );
	}
}

/**********************************************************************/

