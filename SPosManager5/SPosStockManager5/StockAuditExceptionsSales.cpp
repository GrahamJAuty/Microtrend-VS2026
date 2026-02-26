/**********************************************************************/
#include "CCSVReportLine.h"
#include "ReportHelpers.h"
#include "SalesHistoryFields.h"
#include "SalesTypes.h"
#include "StockpointCSVArray.h"
/**********************************************************************/
#include "StockAuditExceptionsSales.h"
/**********************************************************************/

CStockAuditExceptionsSales::CStockAuditExceptionsSales() : CStockAuditBaseReport() 
{
	m_bE1Sales = FALSE;
	m_bImportSales = FALSE;
	m_bManualSales = FALSE;
}

/**********************************************************************/

void CStockAuditExceptionsSales::AddColumns( CReportFile& ReportFile )
{
	ReportFile.SetStyle1 ( GetReportTitle() );
	ReportFile.AddColumn ( "Date", TA_LEFT, 200 );
	ReportFile.AddColumn ( "Time", TA_LEFT, 200 );
	ReportFile.AddColumn ( "Type", TA_LEFT, 180 );
	ReportFile.AddColumn ( "Plu No", TA_LEFT, 350 );

	if ( m_bE1Sales == TRUE )
		ReportFile.AddColumn ( "Location", TA_LEFT, 450 );
		
	ReportFile.AddColumn ( "Quantity", TA_RIGHT, 200 );
	ReportFile.AddColumn ( "Value", TA_RIGHT, 200 );
	ReportFile.AddColumn ( "Code", TA_LEFT, 380 );
	ReportFile.AddColumn ( "Problem", TA_LEFT, 380 );
}

/**********************************************************************/

bool CStockAuditExceptionsSales::ProcessLine( const char* szLine )
{
	CCSV csv( szLine );

	if ( csv.GetInt(1) != m_nReportType )
		return FALSE;

	switch( csv.GetInt(2) )
	{
	case 1:
		m_nLineType = csv.GetInt(3);
		m_strLinePluNo = csv.GetString(4);
		m_strLineLoc = csv.GetString(5);
		m_dLinePluQty = csv.GetDouble(6);
		m_dLinePluValue = csv.GetDouble(7);
		m_strLineStockCode = csv.GetString(8);
		m_strLineProblem = csv.GetString(9);
		m_strLineSaleDate = csv.GetString(10);
		m_strLineSaleTime = csv.GetString(11);
		m_bLineIsWrapper = FALSE;
		break;

	case 2:
		m_nLineType = csv.GetInt(3);
		m_strLinePluNo = csv.GetString(4);
		m_strLineLoc = csv.GetString(5);
		m_dLinePluQty = csv.GetDouble(6);
		m_dLinePluValue = csv.GetDouble(7);
		m_strLineStockCode = csv.GetString(8);
		m_strLineProblem = csv.GetString(9);
		m_strLineSaleDate = csv.GetString(10);
		m_strLineSaleTime = csv.GetString(11);
		m_bLineIsWrapper = csv.GetBool(12);
		break;

	default:
		return FALSE;
	}

	switch( m_nLineType )
	{
	case SALES_TYPE_E1:
		m_strLineType = "EPOS";
		return m_bE1Sales;

	case SALES_TYPE_IMPORT:
		m_strLineType = "Import";
		return m_bImportSales;

	case SALES_TYPE_MANUAL:
		m_strLineType = "Manual";
		return m_bManualSales;

	default:
		return FALSE;
	}
}

/**********************************************************************/

void CStockAuditExceptionsSales::WriteLine( CReportFile& ReportFile, int nDummySpIdx )
{
	CStockReportLine csvOut;
		
	csvOut.AppendString ( m_strHeaderDate );
	csvOut.AppendString ( m_strHeaderTime );
	csvOut.AppendString ( m_strLineType );

	CString strPluNo = ReportHelpers.GetDisplayPluNo ( _atoi64(m_strLinePluNo) );
	csvOut.AppendString ( strPluNo );
						
	if ( m_bE1Sales == TRUE )
		csvOut.AppendString ( m_strLineLoc );
						
	csvOut.AppendStockQty ( m_dLinePluQty, 1, TRUE );
	csvOut.AppendVal ( m_dLinePluValue );
	csvOut.AppendString ( GetDisplayStockCode ( m_strLineStockCode ) );
	csvOut.AppendString( m_strLineProblem );
					
	ReportFile.WriteLine ( csvOut.GetLine() );
}

/**********************************************************************/

