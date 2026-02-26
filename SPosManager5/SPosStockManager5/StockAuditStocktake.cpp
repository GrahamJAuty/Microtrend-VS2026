/**********************************************************************/
 
/**********************************************************************/
#include "CCSVReportLine.h"
//#include "globalfunctions.h"
#include "RepCsv.h"
#include "StockpointCSVArray.h"
/**********************************************************************/
#include "StockAuditStocktake.h"
/**********************************************************************/

CStockAuditStocktake::CStockAuditStocktake() : CStockAuditBaseReport() 
{
}

/**********************************************************************/

void CStockAuditStocktake::AddColumns( CReportFile& ReportFile )
{
	ReportFile.SetStyle1 ( GetReportTitle() );
	ReportFile.AddColumn ( "Code", TA_LEFT, 380 );
	ReportFile.AddColumn ( "Description", TA_LEFT, 300 );
	ReportFile.AddColumn ( "Category", TA_LEFT, 250 );
	ReportFile.AddColumn ( "Apparent", TA_RIGHT, 200 );
	ReportFile.AddColumn ( "Actual", TA_RIGHT, 200 );
}

/**********************************************************************/

void CStockAuditStocktake::WriteHeader( CReportFile& ReportFile )
{
	CString strReference = "";
	::FormatAuditReference( m_strHeaderReference1, m_strHeaderReference2, strReference );
	
	CString strLine;
	strLine.Format ( "<..>%s %s ( reference %s )",  
		( const char* ) m_strHeaderDate,
		( const char* ) m_strHeaderTime,
		( const char* ) strReference );
				
	ReportFile.WriteLine ( strLine );
}

/**********************************************************************/

bool CStockAuditStocktake::ProcessLine( const char* szLine )
{
	CCSV csv( szLine );

	if ( csv.GetInt(1) != GetAuditDataType() )
		return FALSE;

	switch( csv.GetInt(2) )
	{
	case 1:
		m_strLineStockCode = csv.GetString(3);
		m_strLineDescription = csv.GetString(4);
		m_strLineCategory = csv.GetString(5);
		m_nLineSubUnits = csv.GetInt(6);
		m_dLineApparentQty = csv.GetDouble(7);
		m_dLineActualQty = csv.GetDouble(8);
		break;

	default:
		return FALSE;
	}

	if ( m_strStockCodeFrom != "" )
		if ( m_strLineStockCode < m_strStockCodeFrom )
			return FALSE;
		
	if ( m_strStockCodeTo != "" )
		if ( m_strLineStockCode > m_strStockCodeTo )
			return FALSE;

	return TRUE;
}

/**********************************************************************/

void CStockAuditStocktake::WriteLine( CReportFile& ReportFile, int nDummySpIdx )
{
	CStockReportLine csvOut;
	csvOut.AppendString ( GetDisplayStockCode ( m_strLineStockCode ) );
	csvOut.AppendString ( m_strLineDescription );
	csvOut.AppendString ( m_strLineCategory );
	csvOut.AppendStockQty ( m_dLineApparentQty, m_nLineSubUnits, TRUE );
	csvOut.AppendStockQty ( m_dLineActualQty, m_nLineSubUnits, TRUE );

	ReportFile.WriteLine ( csvOut.GetLine() );
}

/**********************************************************************/

void CStockAuditStocktake::AppendTotals( CReportFile& ReportFile )
{
	ReportFile.WriteLine ( "" );
}

/**********************************************************************/
