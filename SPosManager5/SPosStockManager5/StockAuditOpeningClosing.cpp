/**********************************************************************/
 
/**********************************************************************/
#include "CCSVReportLine.h"
#include "RepCsv.h"
#include "StockpointCSVArray.h"
/**********************************************************************/
#include "StockAuditOpeningClosing.h"
/**********************************************************************/

CStockAuditOpeningClosing::CStockAuditOpeningClosing() : CStockAuditBaseReport() 
{
}

/**********************************************************************/

void CStockAuditOpeningClosing::AddColumns( CReportFile& ReportFile )
{
	ReportFile.SetStyle1 ( GetReportTitle() );
	ReportFile.AddColumn ( "Date", TA_LEFT, 200 );
	ReportFile.AddColumn ( "Time", TA_LEFT, 200 );
	ReportFile.AddColumn ( "Code", TA_LEFT, 380 );
	ReportFile.AddColumn ( "Description", TA_LEFT, 300 );
	ReportFile.AddColumn ( "Category", TA_LEFT, 250 );
	ReportFile.AddColumn ( "Action", TA_LEFT, 200 );
	ReportFile.AddColumn ( "DU Qty", TA_RIGHT, 200 );
	ReportFile.AddColumn ( "SU Qty", TA_RIGHT, 200 );
	ReportFile.AddColumn ( "New Qty", TA_RIGHT, 200 );
}

/**********************************************************************/

bool CStockAuditOpeningClosing::ProcessLine( const char* szLine )
{
	CCSV csv( szLine );

	if ( csv.GetInt(1) != GetAuditDataType() )
		return FALSE;

	bool bResult = TRUE;

	switch( csv.GetInt(2) )
	{
	case 1:
		m_strLineStockCode = csv.GetString(3);
		m_strLineDescription = csv.GetString(4);
		m_strLineCategory = csv.GetString(5);
		m_nLineSubUnits = csv.GetInt(6);
		m_nLineType = csv.GetInt(7);
		m_dLineDUQty = 0.0;
		m_dLineSUQty = csv.GetDouble(8);
		m_dLineNewQty = csv.GetDouble(9);
		break;

	case 2:
		m_strLineStockCode = csv.GetString(3);
		m_strLineDescription = csv.GetString(4);
		m_strLineCategory = csv.GetString(5);
		m_nLineSubUnits = csv.GetInt(6);
		m_nLineType = csv.GetInt(7);
		m_dLineDUQty = csv.GetDouble(8);
		m_dLineSUQty = csv.GetDouble(9);
		m_dLineNewQty = csv.GetDouble(10);
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

void CStockAuditOpeningClosing::WriteLine( CReportFile& ReportFile, int nDummySpIdx )
{
	CStockReportLine csvOut;
	csvOut.AppendString ( m_strHeaderDate );
	csvOut.AppendString ( m_strHeaderTime );
	csvOut.AppendString ( GetDisplayStockCode ( m_strLineStockCode ) );
	csvOut.AppendString ( m_strLineDescription );
	csvOut.AppendString ( m_strLineCategory );

	switch ( m_nLineType )
	{
	case ADD_STOCK_LEVELS:
		csvOut.AppendString ( "Add" );
		break;

	case SUBTRACT_STOCK_LEVELS:
		csvOut.AppendString ( "Subtract" );
		break;

	case EDIT_STOCK_LEVELS:
		csvOut.AppendString ( "Edit" );
		break;

	case OVERWRITE_STOCK_LEVELS:
	default:
		csvOut.AppendString ( "Set" );
		break;
	}

	if ( m_dLineDUQty != 0.0 )
		csvOut.AppendStockQty ( m_dLineDUQty, 1, TRUE );
	else
		csvOut.AppendBlanks(1);
	
	csvOut.AppendStockQty ( m_dLineSUQty, m_nLineSubUnits, TRUE );
	csvOut.AppendStockQty ( m_dLineNewQty, m_nLineSubUnits, TRUE );
	ReportFile.WriteLine ( csvOut.GetLine() );
}

/**********************************************************************/

