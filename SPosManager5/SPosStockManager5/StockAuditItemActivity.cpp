/**********************************************************************/
 
/**********************************************************************/
#include "CCSVReportLine.h"
#include "RepCsv.h"
#include "StockpointCSVArray.h"
/**********************************************************************/
#include "StockAuditItemActivity.h"
/**********************************************************************/

CStockAuditItemActivity::CStockAuditItemActivity() : CStockAuditBaseReport() 
{
	m_bPageBreakFlag = FALSE;
	m_bShowNetChange = FALSE;
	m_dNetChange = 0.0;
}

/**********************************************************************/

void CStockAuditItemActivity::AddColumns( CReportFile& ReportFile )
{
	ReportFile.SetStyle1 ( GetReportTitle() );
	ReportFile.AddColumn ( "Date", TA_LEFT, 250 );
	ReportFile.AddColumn ( "Time", TA_LEFT, 180 );		
	ReportFile.AddColumn ( "Code", TA_LEFT, 380 );
	ReportFile.AddColumn ( "Description", TA_LEFT, 380 );
	ReportFile.AddColumn ( "Action", TA_LEFT, 250 );
	ReportFile.AddColumn ( "SU Qty", TA_RIGHT, 250 );
	ReportFile.AddColumn ( "Type", TA_LEFT, 200 );

	if ( ( m_strStockCodeFrom != "" ) && ( m_strStockCodeFrom == m_strStockCodeTo ) )
	{
		ReportFile.AddColumn ( "Net Change", TA_RIGHT, 250 );
		m_bShowNetChange = TRUE;
		m_dNetChange = 0.0;
	}
}

/**********************************************************************/

void CStockAuditItemActivity::WriteHeader( CReportFile& ReportFile )
{
}

/**********************************************************************/

bool CStockAuditItemActivity::ProcessLine( const char* szLine )
{
	CCSV csv( szLine );

	switch( csv.GetInt(1) )
	{
	case AUDIT_DELIVERY:
		switch( csv.GetInt(2) )
		{
		case 1:
		case 2:
		case 3:
		case 4:
			m_strLineStockCode = csv.GetString(3);
			m_strLineDescription = csv.GetString(4);
			m_dLineSUQty = csv.GetDouble(7);
			m_nLineSubUnits = 1;
			m_nLineAction = AUDIT_DELIVERY;
			m_nLineType = ADD_STOCK_LEVELS;
			break;

		default:
			return FALSE;
		}
		break;

	case AUDIT_ADJUSTMENT_STOCK:
		switch( csv.GetInt(2) )
		{
		case 1:
			m_strLineStockCode = csv.GetString(3);
			m_strLineDescription = csv.GetString(4);
			m_nLineSubUnits = csv.GetInt(6);
			m_dLineSUQty = csv.GetDouble(7);
			m_nLineAction = AUDIT_ADJUSTMENT_STOCK;
			m_nLineType = SUBTRACT_STOCK_LEVELS;
			break;

		default:
			return FALSE;
		}
		break;

	case AUDIT_OPENING:
		switch( csv.GetInt(2) )
		{
		case 1:
			m_strLineStockCode = csv.GetString(3);
			m_strLineDescription = csv.GetString(4);
			m_nLineSubUnits = csv.GetInt(6);
			m_nLineType = csv.GetInt(7);
			m_dLineSUQty = csv.GetDouble(8);
			m_nLineAction = AUDIT_OPENING;
			break;

		case 2:
			m_strLineStockCode = csv.GetString(3);
			m_strLineDescription = csv.GetString(4);
			m_nLineSubUnits = csv.GetInt(6);
			m_nLineType = csv.GetInt(7);
			m_dLineSUQty = csv.GetDouble(9);
			m_nLineAction = AUDIT_OPENING;
			break;

		default:
			return FALSE;
		}
		break;

	case AUDIT_STOCKTAKE:
		switch( csv.GetInt(2) )
		{
		case 1:
			m_strLineStockCode = csv.GetString(3);
			m_strLineDescription = csv.GetString(4);
			m_nLineSubUnits = csv.GetInt(6);
			m_dLineSUQty = csv.GetDouble(8);
			m_nLineAction = AUDIT_STOCKTAKE;
			m_nLineType = EDIT_STOCK_LEVELS;
			break;

		default:
			return FALSE;
		}
		break;
	
	case AUDIT_RETURNS:
		switch( csv.GetInt(2) )
		{
		case 1:
		case 5:
			m_strLineStockCode = csv.GetString(3);
			m_strLineDescription = csv.GetString(4);
			m_dLineSUQty = csv.GetDouble(7);
			m_nLineSubUnits = 1;
			m_nLineAction = AUDIT_RETURNS;
			m_nLineType = SUBTRACT_STOCK_LEVELS;
			break;

		default:
			return FALSE;
		}
		break;

	case AUDIT_SALES:
		switch( csv.GetInt(2) )
		{
		case 1:
			m_strLineDescription = csv.GetString(6);
			m_strLineStockCode = csv.GetString(9);
			m_nLineSubUnits = csv.GetInt(10);
			m_dLineSUQty = csv.GetDouble(11);
			m_nLineAction = AUDIT_SALES;
			m_nLineType = SUBTRACT_STOCK_LEVELS;
			break;

		case 3:
			m_strLineDescription = csv.GetString(5);
			m_strLineStockCode = csv.GetString(4);
			m_nLineSubUnits = csv.GetInt(12);
			m_dLineSUQty = csv.GetDouble(13);
			m_nLineAction = AUDIT_SALES;
			m_nLineType = SUBTRACT_STOCK_LEVELS;
			break;

		default:
			return FALSE;
		}
		break;

	case AUDIT_TRANSFER:
		switch( csv.GetInt(2) )
		{
		case 1:
			m_strLineStockCode = csv.GetString(3);
			m_strLineDescription = csv.GetString(4);
			m_nLineSubUnits = csv.GetInt(6);
			m_dLineSUQty = csv.GetDouble(7);
			m_nLineAction = AUDIT_TRANSFER;
			m_nLineType = ( m_bTransferIn ) ? ADD_STOCK_LEVELS : SUBTRACT_STOCK_LEVELS;
			break;

		case 2:
			m_strLineStockCode = csv.GetString(3);
			m_strLineDescription = csv.GetString(4);
			m_nLineSubUnits = csv.GetInt(6);
			m_dLineSUQty = csv.GetDouble(8);
			m_nLineAction = AUDIT_TRANSFER;
			m_nLineType = ( m_bTransferIn ) ? ADD_STOCK_LEVELS : SUBTRACT_STOCK_LEVELS;
			break;

		default:
			return FALSE;
		}
		break;

	case AUDIT_ADJUSTMENT_PLU:
		switch( csv.GetInt(2) )
		{
		case 1:
			m_strLineDescription = csv.GetString(5);
			m_strLineStockCode = csv.GetString(8);
			m_nLineSubUnits = csv.GetInt(9);
			m_dLineSUQty = csv.GetDouble(10);
			m_nLineAction = AUDIT_ADJUSTMENT_PLU;
			m_nLineType = SUBTRACT_STOCK_LEVELS;
			break;
		}	
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

void CStockAuditItemActivity::WriteLine( CReportFile& ReportFile, int nDummySpIdx )
{
	CStockReportLine csvOut;
	csvOut.AppendString( m_strHeaderDate );
	csvOut.AppendString( m_strHeaderTime );
	csvOut.AppendString ( GetDisplayStockCode ( m_strLineStockCode ) );
	csvOut.AppendString ( m_strLineDescription );
	
	switch( m_nLineAction )
	{
	case AUDIT_DELIVERY:
		csvOut.AppendString( "Delivery" );
		break;

	case AUDIT_ADJUSTMENT_STOCK:
	case AUDIT_ADJUSTMENT_PLU:
		csvOut.AppendString( "Adjustment" );
		break;

	case AUDIT_OPENING:
		csvOut.AppendString( "Opening" );
		break;

	case AUDIT_STOCKTAKE:
		csvOut.AppendString( "Stocktake" );
		break;

	case AUDIT_RETURNS:
		csvOut.AppendString( "Returns" );
		break;

	case AUDIT_SALES:
		csvOut.AppendString( "Sales" );
		break;

	case AUDIT_TRANSFER:
		csvOut.AppendString( "Transfer" );
		break;

	default:
		csvOut.Add( "" );
		break;
	}

	csvOut.AppendStockQty( m_dLineSUQty );
	
	switch( m_nLineType )
	{
	case ADD_STOCK_LEVELS:
		csvOut.AppendString( "In" );
		break;

	case SUBTRACT_STOCK_LEVELS:
		csvOut.AppendString( "Out" );
		break;

	case EDIT_STOCK_LEVELS:
	case OVERWRITE_STOCK_LEVELS:
		csvOut.AppendString( "Set" );
		break;

	default:
		csvOut.AppendString( "" );
		break;
	}

	if ( TRUE == m_bShowNetChange )
	{
		switch( m_nLineType )
		{
		case ADD_STOCK_LEVELS:
			m_dNetChange += m_dLineSUQty;
			csvOut.AppendStockQty( m_dNetChange );
			break;

		case SUBTRACT_STOCK_LEVELS:
			m_dNetChange -= m_dLineSUQty;
			csvOut.AppendStockQty( m_dNetChange );
			break;

		case EDIT_STOCK_LEVELS:
		case OVERWRITE_STOCK_LEVELS:
			m_dNetChange = m_dLineSUQty;
			csvOut.AppendString( "****" );
			break;

		default:
			csvOut.AppendString( "" );
			break;
		}
	}

	ReportFile.WriteLine ( csvOut.GetLine() );
}

/**********************************************************************/

void CStockAuditItemActivity::AppendTotals( CReportFile& ReportFile )
{
}

/**********************************************************************/
