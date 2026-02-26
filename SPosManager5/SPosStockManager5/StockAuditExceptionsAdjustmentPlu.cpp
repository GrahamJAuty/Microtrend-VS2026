/**********************************************************************/
#include "CCSVReportLine.h"
#include "ReportHelpers.h"
#include "SalesHistoryFields.h"
#include "SalesTypes.h"
#include "StockpointCSVArray.h"
/**********************************************************************/
#include "StockAuditExceptionsAdjustmentPlu.h"
/**********************************************************************/

CStockAuditExceptionsAdjustmentPlu::CStockAuditExceptionsAdjustmentPlu() : CStockAuditBaseReport() 
{
}

/**********************************************************************/

void CStockAuditExceptionsAdjustmentPlu::AddColumns( CReportFile& ReportFile )
{
	ReportFile.SetStyle1 ( GetReportTitle() );
	ReportFile.AddColumn ( "Date", TA_LEFT, 200 );
	ReportFile.AddColumn ( "Time", TA_LEFT, 200 );
	ReportFile.AddColumn ( "Plu No", TA_LEFT, 350 );
	ReportFile.AddColumn ( "Quantity", TA_RIGHT, 200 );
	ReportFile.AddColumn ( "Value", TA_RIGHT, 200 );
	ReportFile.AddColumn ( "Reason", TA_LEFT, 350 );
	ReportFile.AddColumn ( "Code", TA_LEFT, 380 );
	ReportFile.AddColumn ( "Problem", TA_LEFT, 380 );
}

/**********************************************************************/

bool CStockAuditExceptionsAdjustmentPlu::ProcessLine( const char* szLine )
{
	CCSV csv( szLine );

	if ( csv.GetInt(1) != m_nReportType )
		return FALSE;

	switch( csv.GetInt(2) )
	{
	case 1:
		m_strLinePluNo = csv.GetString(3);
		m_dLinePluQty = csv.GetDouble(4);
		m_dLinePluValue = csv.GetDouble(5);
		m_strLineReason = csv.GetString(6);
		m_strLineStockCode = csv.GetString(7);
		m_strLineProblem = csv.GetString(8);
		m_strLineSaleDate = csv.GetString(9);
		m_strLineSaleTime = csv.GetString(10);
		break;

	default:
		return FALSE;
	}

	return TRUE;
}

/**********************************************************************/

void CStockAuditExceptionsAdjustmentPlu::WriteLine( CReportFile& ReportFile, int nDummySpIdx )
{
	CStockReportLine csvOut;
		
	csvOut.AppendString ( m_strHeaderDate );
	csvOut.AppendString ( m_strHeaderTime );
	
	CString strPluNo = ReportHelpers.GetDisplayPluNo ( _atoi64(m_strLinePluNo) );
	csvOut.AppendString ( strPluNo );
						
	csvOut.AppendStockQty ( m_dLinePluQty, 1, TRUE );
	csvOut.AppendVal ( m_dLinePluValue );
	csvOut.AppendString ( m_strLineReason );
	csvOut.AppendString ( GetDisplayStockCode ( m_strLineStockCode ) );
	csvOut.AppendString( m_strLineProblem );
					
	ReportFile.WriteLine ( csvOut.GetLine() );
}

/**********************************************************************/

