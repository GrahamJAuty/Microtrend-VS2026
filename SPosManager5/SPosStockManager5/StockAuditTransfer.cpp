/**********************************************************************/
 
/**********************************************************************/
#include "CCSVReportLine.h"
//#include "globalfunctions.h"
#include "RepCsv.h"
#include "StockpointCSVArray.h"
/**********************************************************************/
#include "StockAuditTransfer.h"
/**********************************************************************/

CStockAuditTransfer::CStockAuditTransfer() : CStockAuditBaseReport() 
{
	m_bPageBreakFlag = FALSE;
}

/**********************************************************************/

void CStockAuditTransfer::AddColumns( CReportFile& ReportFile )
{
	ReportFile.SetStyle1 ( GetReportTitle() );
	ReportFile.AddColumn ( "Code", TA_LEFT, 380 );
	ReportFile.AddColumn ( "Description", TA_LEFT, 380 );
	ReportFile.AddColumn ( "Category", TA_LEFT, 250 );
	ReportFile.AddColumn ( "DU Qty", TA_RIGHT, 200 );
	ReportFile.AddColumn ( "SU Qty", TA_RIGHT, 200 );
	ReportFile.AddColumn ( "Type", TA_LEFT, 200 );
}

/**********************************************************************/

void CStockAuditTransfer::WriteHeader( CReportFile& ReportFile )
{
	CString strReference = "";
	::FormatAuditReference( m_strHeaderReference1, m_strHeaderReference2, strReference );

	CString strLine;
	strLine.Format ( "<..>%s %s, %s to %s ( reference %s )",
		m_strHeaderDate,
		m_strHeaderTime,
		m_strHeaderSpName1,
		m_strHeaderSpName2,
		strReference );
	
	ReportFile.WriteLine ( strLine );
	ReportFile.WriteLine ( "<LI>" );
}

/**********************************************************************/

bool CStockAuditTransfer::ProcessLine( const char* szLine )
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
		m_dLineDUQty = 0.0;
		m_dLineSUQty = csv.GetDouble(7);
		break;

	case 2:
		m_strLineStockCode = csv.GetString(3);
		m_strLineDescription = csv.GetString(4);
		m_strLineCategory = csv.GetString(5);
		m_nLineSubUnits = csv.GetInt(6);
		m_dLineDUQty = csv.GetDouble(7);
		m_dLineSUQty = csv.GetDouble(8);
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

void CStockAuditTransfer::WriteLine( CReportFile& ReportFile, int nDummySpIdx )
{
	CStockReportLine csvOut;
	csvOut.AppendString ( GetDisplayStockCode ( m_strLineStockCode ) );
	csvOut.AppendString ( m_strLineDescription );
	csvOut.AppendString ( m_strLineCategory );

	if ( m_dLineDUQty != 0.0 )
		csvOut.AppendStockQty ( m_dLineDUQty, 1, TRUE );
	else
		csvOut.AppendBlanks(1);
	
	csvOut.AppendStockQty ( m_dLineSUQty, m_nLineSubUnits, TRUE );
	csvOut.AppendString ( m_bTransferIn ? "IN" : "OUT" );

	ReportFile.WriteLine ( csvOut.GetLine() );
}

/**********************************************************************/

void CStockAuditTransfer::AppendTotals( CReportFile& ReportFile )
{
	ReportFile.WriteLine ( m_bPageBreakFlag ? "<NEWPAGE>" : "" );
}

/**********************************************************************/
