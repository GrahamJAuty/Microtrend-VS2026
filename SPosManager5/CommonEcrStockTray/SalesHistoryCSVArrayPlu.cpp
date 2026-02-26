/**********************************************************************/
#include "EposReportSelect.h"
#include "SysInfo.h"
/**********************************************************************/
#include "SalesHistoryCSVArrayPlu.h"
/**********************************************************************/

CSalesHistoryCSVRecordPlu::CSalesHistoryCSVRecordPlu()
{
	ClearRecord();	
}

/**********************************************************************/

void CSalesHistoryCSVRecordPlu::ClearRecord()
{
	m_strName = "";
	m_nReportType = SH_CUSTOM;
	m_nReportMode = SALESHISTORY_REPORTMODE_NORMAL;
	m_strEntityID = "";
	m_bIsBottom100 = FALSE;
	m_bIsSortedByValue = FALSE;
	m_nChartType = CHART_TYPE_MAXITEMS;
	m_nChartSize = 100;
	m_dChartCutOff = 0.0;
	m_strExportFilename = "";
	m_arrayFields.RemoveAll();
	m_bPreV3Record = FALSE;
}

/**********************************************************************/

void CSalesHistoryCSVRecordPlu::ConvertFromCSV ( CCSV& csv )
{
	ClearRecord();
	SetName ( csv.GetString (0) );
	SetReportType ( csv.GetInt(1) ); 

	switch ( csv.GetInt(2) )
	{
	case 1:		V1ConvertFromCSV ( csv );		break;
	case 2:		V2ConvertFromCSV ( csv );		break;
	case 3:		V3to5ConvertFromCSV ( csv, 3 );	break;
	case 4:		V3to5ConvertFromCSV ( csv, 4 );	break;
	case 5:		V3to5ConvertFromCSV ( csv, 5 );	break;
	default:	m_bPreV3Record = TRUE;			break;
	}
}

/**********************************************************************/

void CSalesHistoryCSVRecordPlu::V1ConvertFromCSV ( CCSV& csv )
{
	m_bPreV3Record = TRUE;

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

void CSalesHistoryCSVRecordPlu::V2ConvertFromCSV ( CCSV& csv )
{
	m_bPreV3Record = TRUE;

	SetReportMode( csv.GetBool(3) ? SALESHISTORY_REPORTMODE_CHART : SALESHISTORY_REPORTMODE_NORMAL );
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
	
void CSalesHistoryCSVRecordPlu::V3to5ConvertFromCSV ( CCSV& csv, int nVer )
{
	m_bPreV3Record = FALSE;

	SetEntityID( csv.GetString(3) );

	if ( nVer <= 4 )
		SetReportMode( csv.GetBool(4) ? SALESHISTORY_REPORTMODE_CHART : SALESHISTORY_REPORTMODE_NORMAL );
	else
		SetReportMode( csv.GetInt(4) );

	SetBottom100Flag( csv.GetBool(5) );
	SetSortedByValueFlag( csv.GetBool(6) );
	SetChartType( csv.GetInt(7) );
	SetChartSize( csv.GetInt(8) );
	SetChartCutOff( csv.GetDouble(9) );

	int nOffset = 10;

	if ( nVer >= 4 )
		SetExportFilename( csv.GetString( nOffset++) );
	
	if ( m_nReportType == SH_CUSTOM )
	{
		int nCount = csv.GetInt(nOffset++);
		for ( int nIndex = 0; nIndex < nCount; nIndex++ )
			m_arrayFields.Add( csv.GetInt( nOffset++ ) );
	}
}

/**********************************************************************/

void CSalesHistoryCSVRecordPlu::ConvertToCSV ( CCSV& csv )
{
	csv.Add ( m_strName );
	csv.Add ( m_nReportType );
	csv.Add ( SALESHISTORYPLU_VERSION );
	csv.Add ( m_strEntityID );
	csv.Add ( m_nReportMode );
	csv.Add ( m_bIsBottom100 );
	csv.Add ( m_bIsSortedByValue );
	csv.Add ( m_nChartType );
	csv.Add ( m_nChartSize );
	csv.Add ( m_dChartCutOff, 3 );
	csv.Add ( m_strExportFilename );
	
	if ( m_nReportType == SH_CUSTOM )
	{
		csv.Add( m_arrayFields.GetSize() );
		for ( int nIndex = 0; nIndex < m_arrayFields.GetSize(); nIndex++ )
			csv.Add ( m_arrayFields.GetAt ( nIndex ) );
	}
}

/**********************************************************************/

void CSalesHistoryCSVRecordPlu::SetReportMode( int n )
{
	switch(n)
	{
	case SALESHISTORY_REPORTMODE_NORMAL:
	case SALESHISTORY_REPORTMODE_TWOBLOCK:
	case SALESHISTORY_REPORTMODE_CHART:
	case SALESHISTORY_REPORTMODE_WEEKLY_VAL_FULL:
	case SALESHISTORY_REPORTMODE_WEEKLY_VAL_DISC:
	case SALESHISTORY_REPORTMODE_WEEKLY_QTYVAL_FULL:
	case SALESHISTORY_REPORTMODE_WEEKLY_QTYVAL_DISC:
	case SALESHISTORY_REPORTMODE_WEEKLY_QTY:
	case SALESHISTORY_REPORTMODE_MONTHLY_VAL_FULL:
	case SALESHISTORY_REPORTMODE_MONTHLY_VAL_DISC:
	case SALESHISTORY_REPORTMODE_MONTHLY_QTY:
		m_nReportMode = n;
		break;
	}
}

/**********************************************************************/

int CSalesHistoryCSVRecordPlu::GetReportMode()
{
	if ( SALESHISTORY_REPORTMODE_TWOBLOCK == m_nReportMode )
		return SALESHISTORY_REPORTMODE_TWOBLOCK;

	switch( m_nReportType )
	{
	case SH_CUSTOM:
		return m_nReportMode;

	case SH_PLU_TOP100:
	case SH_PLU_TOP100_DEPT:
	case SH_PLU_TOP100_MONTH:
	case SH_PLU_TOP100_MONTH_DEPT:
		return SALESHISTORY_REPORTMODE_CHART;

	case SH_PLU_DEPT_BYWDAY:		
	case SH_PLU_LOC_BYWDAY:		
	case SH_PLU_DAILY_BYWEEK:
		return SALESHISTORY_REPORTMODE_WEEKLY_VAL_DISC;

	default:
		return SALESHISTORY_REPORTMODE_NORMAL;
	}
}

/**********************************************************************/

void CSalesHistoryCSVRecordPlu::SetChartType( int nType )
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

void CSalesHistoryCSVRecordPlu::SetChartSize( int nSize )
{
	if ( nSize >= 1 && nSize <= 9999 ) m_nChartSize = nSize;
}

/**********************************************************************/

void CSalesHistoryCSVRecordPlu::SetExportFilename( const char* sz )
{
	CString str = sz;
	if ( str.GetLength() <= EPOS_MAXLEN_EXPORT_FILENAME )
		m_strExportFilename = str;
}

/**********************************************************************/

void CSalesHistoryCSVRecordPlu::SetReportFields( CWordArray& arrayTemp )
{
	m_arrayFields.RemoveAll();
	for ( int nIndex = 0; nIndex < arrayTemp.GetSize(); nIndex++ )
		m_arrayFields.Add( arrayTemp.GetAt( nIndex ) );
}

/**********************************************************************/

void CSalesHistoryCSVRecordPlu::GetReportFields( CWordArray& arrayTemp )
{
	arrayTemp.RemoveAll();
	for ( int nIndex = 0; nIndex < m_arrayFields.GetSize(); nIndex++ )
		arrayTemp.Add( m_arrayFields.GetAt( nIndex ) );
}

/**********************************************************************/

const char* CSalesHistoryCSVRecordPlu::GetKey( bool bPercent, bool bProfit )
{
	CCSV csvKey ( '.' );
	
	CString strField;
	strField.Format ( "%3.3d", GetReportType() );
	strField += ( bPercent ) ? "1" : "0";
	strField += ( bProfit ) ? "1" : "0";
	csvKey.Add( strField );
	
	for ( int nPos = 0; nPos < m_arrayFields.GetSize(); nPos++ )
		csvKey.Add ( m_arrayFields.GetAt( nPos ) );

	m_strKey = csvKey.GetLine();	
	return m_strKey;
}

/**********************************************************************/

const char* CSalesHistoryCSVRecordPlu::GetFieldListName( int nField )
{
	switch ( nField )
	{
	case SH_FIELD_MONTH:			m_strFieldListName = "Month";				break;
	case SH_FIELD_WEEK:				m_strFieldListName = "Week Starting";		break;
	case SH_FIELD_WEEKDAY:			m_strFieldListName = "Weekday";				break;
	case SH_FIELD_DATE_BUSINESS:	m_strFieldListName = "Full Business Date";	break;
	case SH_FIELD_DATE_CALENDAR:	m_strFieldListName = "Full Calendar Date";	break;
	case SH_FIELD_DBASE:			m_strFieldListName = "Database";			break;
	case SH_FIELD_LOC:				m_strFieldListName = "Location";			break;
	case SH_FIELD_TERMINAL:			m_strFieldListName = "Terminal No";			break;
	case SH_FIELD_TABLENO:			m_strFieldListName = "Table Name";			break;
	case SH_FIELD_TABLEGRP:			m_strFieldListName = "Table Group";			break;
	case SH_FIELD_DEPTNO_SALE:		m_strFieldListName = "Dept No (Sale)";		break;
	case SH_FIELD_DEPTNO_ITEM:		m_strFieldListName = "Dept No (Dbase)";		break;
	case SH_FIELD_DEPTNO_FILTER:	m_strFieldListName = "Dept No (Filter)";	break;
	case SH_FIELD_TRANSERVER:		m_strFieldListName = "Transaction Server";	break;
	case SH_FIELD_ITEMSERVER:		m_strFieldListName = "Item Server";			break;
	case SH_FIELD_CUSTOMER:			m_strFieldListName = "Customer";			break;
	case SH_FIELD_PLUSUPP:			m_strFieldListName = "Supplier";			break;
	case SH_FIELD_TIME:				m_strFieldListName = "Time";				break;
	case SH_FIELD_PLUNO:			m_strFieldListName = "Plu Number";			break;
	case SH_FIELD_DESCRIPTION:		m_strFieldListName = "Description";			break;
	case SH_FIELD_ATTRIBUTETEXT1:	m_strFieldListName = "Attribute1";			break;
	case SH_FIELD_CASHRSPTEXT:		m_strFieldListName = "CashRSP Text";		break;
	case SH_FIELD_SORTCODE:			m_strFieldListName = DealerFlags.GetSortCodeLabel(); break;
	case SH_FIELD_SEQNO:			m_strFieldListName = "Sequence No";			break;
	case SH_FIELD_LEVEL:			m_strFieldListName = "Price Band";			break;
	case SH_FIELD_TAXBAND:			m_strFieldListName = "Tax Band";			break;
	case SH_FIELD_MODIFIER:			m_strFieldListName = "Modifier";			break;
	case SH_FIELD_ACAT_SALE:		m_strFieldListName = "Analysis Category (Sale)";	break;
	case SH_FIELD_ACAT_ITEM:		m_strFieldListName = "Analysis Category (Item)";	break;
	case SH_FIELD_DEPTGRP_SALE:		m_strFieldListName = "Dept Group (Sale)";	break;
	case SH_FIELD_DEPTGRP_ITEM:		m_strFieldListName = "Dept Group (Dbase)";	break;
	case SH_FIELD_DEPTGRP_FILTER:	m_strFieldListName = "Dept Group (Filter)";	break;
	case SH_FIELD_REPGRP:			m_strFieldListName = "Report Group";		break;
	case SH_FIELD_CONGRP:			m_strFieldListName = "Consolidation Group";	break;
	case SH_FIELD_REPPOINT:			m_strFieldListName = "Report Point";		break;
	default:						m_strFieldListName = "Unknown";				break;
	}

	return m_strFieldListName;
}

/**********************************************************************/

const char* CSalesHistoryCSVRecordPlu::GetListName()
{
	switch( m_nReportType )
	{
	case SH_CUSTOM:		m_strListName = m_strName;			break;
	case SH_PLU_ITEM:		m_strListName = "Plu Item";			break;
	case SH_PLU_TOP100:		m_strListName = "Top/Bottom Items";	break;	
	case SH_PLU_UNSOLD:		m_strListName = "Unsold Items";		break;	
	case SH_PLU_DEPT:		m_strListName = "Department Summary";	break;	
	case SH_PLU_DEPT_BYWDAY:	m_strListName = "Department by Weekday";break;	
	case SH_PLU_LOC_BYWDAY:	m_strListName = "Location by Weekday";	break;	
	case SH_PLU_MONTHLY:	m_strListName = "Monthly Summary";		break;		
	case SH_PLU_WEEKLY:		m_strListName = "Weekly Summary";		break;	
	case SH_PLU_DAILY:		m_strListName = "Daily Summary";		break;		
	case SH_PLU_DAILY_BYWEEK:m_strListName = "Daily by Week";		break;	
	case SH_PLU_TAX:		m_strListName = EcrmanOptions.GetTaxString( "%T Summary" );		break;
	default:				m_strListName = "Unknown";			break;
	}
	return m_strListName;
}

/**********************************************************************/

const char* CSalesHistoryCSVRecordPlu::GetReportName()
{
	switch ( m_nReportType )
	{
	case SH_CUSTOM:
		m_strReportName = m_strName;
		break;

	case SH_PLU_ITEM:
	case SH_PLU_ITEM_DEPT:
	case SH_PLU_ITEM_MONTH:
	case SH_PLU_ITEM_MONTH_DEPT:		
		m_strReportName = "Plu Sales History";	
		break;

	case SH_PLU_UNSOLD:
	case SH_PLU_UNSOLD_DEPT:
	case SH_PLU_UNSOLD_MONTH:
	case SH_PLU_UNSOLD_MONTH_DEPT:
		m_strReportName = "Unsold Plu Items";	
		break;

	case SH_PLU_DEPT:
	case SH_PLU_DEPT_MONTH:			
		m_strReportName = "Plu Department Sales History";	
		break;

	case SH_PLU_DEPT_BYWDAY:			
		m_strReportName = "Plu Department Sales History by Weekday";
		break;

	case SH_PLU_LOC_BYWDAY:			
		m_strReportName = "Plu Location Sales History by Weekday";
		break;

	case SH_PLU_DAILY:
		m_strReportName = "Plu Daily Sales History" ;
		break;

	case SH_PLU_DAILY_BYWEEK:
		m_strReportName = "Plu Daily Sales History by Week" ;
		break;

	case SH_PLU_WEEKLY:
		m_strReportName = "Plu Weekly Sales History";
		break;

	case SH_PLU_MONTHLY:
		m_strReportName = "Plu Monthly Sales History";
		break;

	case SH_PLU_TAX:
		{
			m_strReportName = EcrmanOptions.GetSalesTaxName();
			m_strReportName += " Summary";
		}
		break;

	case SH_PLU_TOP100:
	case SH_PLU_TOP100_DEPT:
	case SH_PLU_TOP100_MONTH:
	case SH_PLU_TOP100_MONTH_DEPT:
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

void CSalesHistoryCSVRecordPlu::GetValidFieldList( CWordArray& array )
{
	array.RemoveAll();
	array.Add ( SH_FIELD_MONTH );
	array.Add ( SH_FIELD_WEEK );
	array.Add ( SH_FIELD_WEEKDAY );
	array.Add ( SH_FIELD_DATE_BUSINESS );
	array.Add ( SH_FIELD_DATE_CALENDAR );
	array.Add ( SH_FIELD_TIME );
	array.Add ( SH_FIELD_SEQNO );
	array.Add ( SH_FIELD_DBASE );
	array.Add ( SH_FIELD_LOC );
	array.Add ( SH_FIELD_TERMINAL );
	
	if ( SysInfo.IsEcrLiteSystem() == FALSE )
	{
		array.Add ( SH_FIELD_TABLEGRP );
		array.Add ( SH_FIELD_TABLENO );
	}
	
	array.Add ( SH_FIELD_REPPOINT );
	array.Add ( SH_FIELD_DEPTGRP_SALE );
	array.Add ( SH_FIELD_DEPTGRP_ITEM );
	array.Add ( SH_FIELD_DEPTGRP_FILTER );
	array.Add ( SH_FIELD_DEPTNO_SALE );
	array.Add ( SH_FIELD_DEPTNO_ITEM );
	array.Add ( SH_FIELD_DEPTNO_FILTER );
	array.Add ( SH_FIELD_REPGRP );
	array.Add ( SH_FIELD_CONGRP );
	array.Add ( SH_FIELD_ACAT_SALE );
	array.Add ( SH_FIELD_ACAT_ITEM );
	array.Add ( SH_FIELD_TRANSERVER );
	array.Add ( SH_FIELD_ITEMSERVER );
	array.Add ( SH_FIELD_CUSTOMER );

#ifndef STOCKMAN_SYSTEM
	array.Add ( SH_FIELD_PLUSUPP );
#endif

	array.Add ( SH_FIELD_TAXBAND );
	array.Add ( SH_FIELD_PLUNO );
	array.Add ( SH_FIELD_DESCRIPTION );
	array.Add ( SH_FIELD_CASHRSPTEXT );
	array.Add ( SH_FIELD_ATTRIBUTETEXT1);
	array.Add ( SH_FIELD_SORTCODE );

	if ( SysInfo.GetMaxBasePluLen() != 0 )
		array.Add ( SH_FIELD_MODIFIER );
	
	array.Add ( SH_FIELD_LEVEL );
}

/**********************************************************************/

int CSalesHistoryCSVRecordPlu::GetBaseReportType()
{
	switch ( m_nReportType )
	{
	case SH_PLU_ITEM_DEPT:
	case SH_PLU_ITEM_MONTH:
	case SH_PLU_ITEM_MONTH_DEPT:
		return SH_PLU_ITEM;

	case SH_PLU_DEPT_MONTH:
		return SH_PLU_DEPT;

	case SH_PLU_TOP100_DEPT:
	case SH_PLU_TOP100_MONTH:
	case SH_PLU_TOP100_MONTH_DEPT:
		return SH_PLU_TOP100;

	case SH_PLU_UNSOLD_DEPT:
	case SH_PLU_UNSOLD_MONTH:
	case SH_PLU_UNSOLD_MONTH_DEPT:
		return SH_PLU_UNSOLD;

	default:
		return m_nReportType;
	}
}

/**********************************************************************/

bool CSalesHistoryCSVRecordPlu::CanDoMonthlyBreakdown()
{
	bool bResult = FALSE;
		
	switch ( GetBaseReportType() )
	{
	case SH_PLU_ITEM:
	case SH_PLU_TOP100:
	case SH_PLU_UNSOLD:
	case SH_PLU_DEPT:
		bResult = TRUE;
	}

	return bResult;
}

/**********************************************************************/

bool CSalesHistoryCSVRecordPlu::CanDoDeptSort()
{
	bool bResult = FALSE;

	switch ( GetBaseReportType() )
	{
	case SH_PLU_ITEM:
	case SH_PLU_TOP100:
	case SH_PLU_UNSOLD:
		bResult = TRUE;
		break;
	}
	
	return bResult;
}

/**********************************************************************/

bool CSalesHistoryCSVRecordPlu::CanDoEstimatedProfit()
{
	bool bResult = FALSE;

	switch ( GetBaseReportType() )
	{
	case SH_PLU_ITEM:
	case SH_PLU_TOP100:
	case SH_PLU_DEPT:
	case SH_PLU_DAILY:
	case SH_PLU_WEEKLY:
	case SH_PLU_MONTHLY:
	case SH_PLU_DEPT_BYWDAY:			
	case SH_PLU_LOC_BYWDAY:			
	case SH_PLU_DAILY_BYWEEK:
	case SH_CUSTOM:	
		bResult = TRUE;
		break;
	}
	
	return bResult;
}

/**********************************************************************/

bool CSalesHistoryCSVRecordPlu::CanDoPercentSale()
{
	bool bResult = FALSE;

	switch ( GetBaseReportType() )
	{
	case SH_CUSTOM:
	case SH_PLU_TOP100:
	case SH_PLU_ITEM:
	case SH_PLU_DEPT:
	case SH_PLU_MONTHLY:
	case SH_PLU_WEEKLY:
	case SH_PLU_DAILY:
	case SH_PLU_DEPT_BYWDAY:			
	case SH_PLU_LOC_BYWDAY:			
	case SH_PLU_DAILY_BYWEEK:
		bResult = TRUE;
		break;
	}

	return bResult;
}

/**********************************************************************/

bool CSalesHistoryCSVRecordPlu::CanDoDateCompare()
{
	bool bResult = FALSE;

	switch ( GetBaseReportType() )
	{
	case SH_CUSTOM:
		switch( m_nReportMode )
		{
		case SALESHISTORY_REPORTMODE_NORMAL:
		case SALESHISTORY_REPORTMODE_TWOBLOCK:
			bResult = TRUE;
			break;
		}
		break;

	case SH_PLU_ITEM:
	case SH_PLU_DEPT:
		bResult = TRUE;
		break;
	}

	return bResult;
}

/**********************************************************************/

bool CSalesHistoryCSVRecordPlu::CanDoGraph()
{
bool bResult = FALSE;

	switch ( GetBaseReportType() )
	{
	case SH_PLU_DEPT:
	case SH_PLU_DAILY:
	case SH_PLU_WEEKLY:
	case SH_PLU_MONTHLY:
		bResult = TRUE;
		break;
	}

	return bResult;
}

/**********************************************************************/

bool CSalesHistoryCSVRecordPlu::CanBeCopied()
{
	bool bResult = FALSE;

	switch ( GetBaseReportType() )
	{
	case SH_PLU_ITEM:
	case SH_PLU_TOP100:
	case SH_PLU_DEPT:
	case SH_PLU_MONTHLY:
	case SH_PLU_WEEKLY:
	case SH_PLU_DAILY:
	case SH_PLU_DEPT_BYWDAY:			
	case SH_PLU_LOC_BYWDAY:			
	case SH_PLU_DAILY_BYWEEK:
	case SH_CUSTOM:
		bResult = TRUE;
		break;
	}

	return bResult;
}

/**********************************************************************/

bool CSalesHistoryCSVRecordPlu::CreateCopy( CSalesHistoryCSVRecordPlu& copy )
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

	case SH_PLU_ITEM:
		arrayFields.Add( SH_FIELD_PLUNO );
		arrayFields.Add( SH_FIELD_DEPTNO_FILTER );
		break;

	case SH_PLU_TOP100:
		arrayFields.Add( SH_FIELD_PLUNO );
		arrayFields.Add( SH_FIELD_DEPTNO_FILTER );
		copy.SetReportMode( SALESHISTORY_REPORTMODE_CHART );
		break;

	case SH_PLU_DEPT:
		arrayFields.Add( SH_FIELD_DEPTNO_FILTER );
		break;

	case SH_PLU_DAILY:
		arrayFields.Add( SH_FIELD_DATE_BUSINESS );
		break;

	case SH_PLU_MONTHLY:
		arrayFields.Add( SH_FIELD_MONTH );
		break;

	case SH_PLU_WEEKLY:
		arrayFields.Add( SH_FIELD_WEEK );
		break;

	case SH_PLU_DEPT_BYWDAY:	
		arrayFields.Add( SH_FIELD_DEPTNO_FILTER );
		copy.SetReportMode( SALESHISTORY_REPORTMODE_WEEKLY_VAL_DISC );
		break;

	case SH_PLU_LOC_BYWDAY:	
		arrayFields.Add( SH_FIELD_LOC );
		copy.SetReportMode( SALESHISTORY_REPORTMODE_WEEKLY_VAL_DISC );
		break;

	case SH_PLU_DAILY_BYWEEK:
		arrayFields.Add( SH_FIELD_WEEK );
		copy.SetReportMode( SALESHISTORY_REPORTMODE_WEEKLY_VAL_DISC );
		break;
	}

	copy.SetReportFields( arrayFields );
	return TRUE;
}
	
/**********************************************************************/

bool CSalesHistoryCSVRecordPlu::HasField( int nField )
{
	bool bResult = FALSE;
	
	CWordArray arrayTemp;
	GetReportFields( arrayTemp );

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

bool CSalesHistoryCSVRecordPlu::HasPluNoField()
{
	return HasField ( SH_FIELD_PLUNO );
}

/**********************************************************************/

bool CSalesHistoryCSVRecordPlu::HasModifierField()
{
	return HasField ( SH_FIELD_MODIFIER );
}

/**********************************************************************/

const char* CSalesHistoryCSVRecordPlu::GetEntityID()
{
	if ( SH_CUSTOM != m_nReportType )
		m_strEntityID.Format( "%4.4X", m_nReportType );

	return m_strEntityID;
}

/**********************************************************************/

void CSalesHistoryCSVRecordPlu::GetDefaultExportFilename( CString& strExportFilename )
{
	int nDummy = 1;
	CReportConsolidationArray<CSortedStringItem> arrayDummy;
	GetDefaultExportFilename( arrayDummy, nDummy, strExportFilename );
}

/**********************************************************************/

void CSalesHistoryCSVRecordPlu::GetDefaultExportFilename( CReportConsolidationArray<CSortedStringItem>& arrayExportLabels, int& nExtraReportID, CString& strExportFilename )
{
	strExportFilename = "";
	if ( SH_CUSTOM == m_nReportType )
	{
		strExportFilename = "P2";
						
		CString strEntityID = GetEntityID();
		if ( strEntityID.GetLength() != 12 )
			strExportFilename += strEntityID;
		else
		{
			CSortedStringItem item;
			item.m_strItem = strEntityID.Mid(6,4);
			strExportFilename += item.m_strItem;

			int nPos;
			if ( arrayExportLabels.Find( item, nPos ) == FALSE )
				arrayExportLabels.Consolidate( item );	
			else
			{
				CString strTemp;
				strTemp.Format( "%d", nExtraReportID++ );
				strExportFilename += strTemp;
			}
		}
	}
	else
	{
		strExportFilename.Format( "P1%3.3d", m_nReportType );
	}
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CSalesHistoryCSVArrayPlu::CSalesHistoryCSVArrayPlu() : CSharedCSVArray()
{
}

/**********************************************************************/

const char* CSalesHistoryCSVArrayPlu::GetName ( int nIndex )
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

const char* CSalesHistoryCSVArrayPlu::GetEntityID( int nIndex )
{
	m_strEntityID = "";

	if ( ( nIndex >= 0 ) && ( nIndex < GetSize() ) )
	{
		CSalesHistoryCSVRecordPlu Record;
		GetAt( nIndex, Record );
		m_strEntityID = Record.GetEntityID();
	}

	return m_strEntityID;
}

/**********************************************************************/

int CSalesHistoryCSVArrayPlu::GetReportType ( int nIndex )
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

void CSalesHistoryCSVArrayPlu::LoadAdhocReport( CSalesHistoryCSVRecordPlu& ReportRecord )
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

void CSalesHistoryCSVArrayPlu::SaveAdhocReport( CSalesHistoryCSVRecordPlu& ReportRecord )
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

int CSalesHistoryCSVArrayPlu::Open( const char* szFilename, int nMode )
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
				CSalesHistoryCSVRecordPlu Record;
				Record.SetReportType( ReportList.GetAt(n) );
				
				CString str;
				str.Format ( "%d", Record.GetReportType() );
				Record.SetEntityID( str );

				Add( Record );
			}
		}

		for ( int n = GetSize() - 1; n >=0; n-- )
			if ( AllowReport(GetReportType(n)) == FALSE )
				RemoveAt(n);

		if ( GetSize() != 0 )
		{
			CSalesHistoryCSVRecordPlu RecordZero;
			GetAt( 0, RecordZero );

			if ( RecordZero.GetPreV3RecordFlag() == TRUE )
			{
				for ( int n = 0; n < GetSize(); n++ )
				{
					CSalesHistoryCSVRecordPlu Record;
					GetAt( n, Record );

					if ( Record.GetReportType() == SH_CUSTOM )
					{
						CString str;
						str.Format ( "%5.5X", 0x10000 + n );
						Record.SetEntityID( str );
					}

					SetAt( n, Record );
				}
			}
		}
	}

	return nReply;
}

/**********************************************************************/

bool CSalesHistoryCSVArrayPlu::AllowReport( int nReportType )
{
	switch ( nReportType )
	{
	case SH_CUSTOM:
	case SH_PLU_ITEM:
	case SH_PLU_UNSOLD:
	case SH_PLU_TOP100:
	case SH_PLU_DEPT:
	case SH_PLU_DEPT_BYWDAY:
	case SH_PLU_LOC_BYWDAY:
	case SH_PLU_MONTHLY:
	case SH_PLU_WEEKLY:
	case SH_PLU_DAILY:
	case SH_PLU_DAILY_BYWEEK:
	case SH_PLU_TAX:
		return TRUE;

	default:
		return FALSE;
	}
}

/**********************************************************************/

void CSalesHistoryCSVArrayPlu::GetSystemReportList( CWordArray& array )
{
	array.RemoveAll();
	array.Add ( SH_PLU_ITEM );
	array.Add ( SH_PLU_UNSOLD );
	array.Add ( SH_PLU_TOP100 );
	array.Add ( SH_PLU_DEPT );
	array.Add ( SH_PLU_DEPT_BYWDAY );
	array.Add ( SH_PLU_LOC_BYWDAY );
	array.Add ( SH_PLU_MONTHLY );
	array.Add ( SH_PLU_WEEKLY );
	array.Add ( SH_PLU_DAILY );
	array.Add ( SH_PLU_DAILY_BYWEEK );
	array.Add ( SH_PLU_TAX );
}

/**********************************************************************/

void CSalesHistoryCSVArrayPlu::CopyFrom( CSalesHistoryCSVArrayPlu& Source )
{
	RemoveAt( 0, GetSize() );

	for ( int n = 0; n < Source.GetSize(); n++ )
	{
		CSalesHistoryCSVRecordPlu record;
		Source.GetAt( n, record );
		Add( record );
	}
}

/**********************************************************************/