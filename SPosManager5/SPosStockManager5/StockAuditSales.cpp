/**********************************************************************/
#include "CCSVReportLine.h"
#include "RepCsv.h"
#include "ReportHelpers.h"
#include "SalesHistoryFields.h"
#include "SalesTypes.h"
#include "StockpointCSVArray.h"
/**********************************************************************/
#include "StockAuditSales.h"
/**********************************************************************/

CStockAuditSales::CStockAuditSales() : CStockAuditBaseReport() 
{
	m_bIncludeRecipeItems = FALSE;
	m_bE1Sales = FALSE;
	m_bImportSales = FALSE;
	m_bManualSales = FALSE;
	m_bHidePluNo = FALSE;
	m_bIsStockOnlyLine = FALSE;
}

/**********************************************************************/

void CStockAuditSales::AddColumns( CReportFile& ReportFile )
{
	ReportFile.SetStyle1 ( GetReportTitle() );
	ReportFile.AddColumn ( "Date", TA_LEFT, 200 );
	ReportFile.AddColumn ( "Time", TA_LEFT, 200 );
	ReportFile.AddColumn ( "Type", TA_LEFT, 250 );

	if ( FALSE == m_bHidePluNo )
	{
		ReportFile.AddColumn ( "Plu No", TA_LEFT, 350 );

		if ( m_bE1Sales == TRUE )
			ReportFile.AddColumn ( "Location", TA_LEFT, 450 );
		
		ReportFile.AddColumn ( "Description", TA_LEFT, 300 );
		ReportFile.AddColumn ( "Quantity", TA_RIGHT, 200 );
		ReportFile.AddColumn ( "Value", TA_RIGHT, 200 );
		ReportFile.AddColumn ( "Code", TA_LEFT, 380 );
	}
	else
	{
		ReportFile.AddColumn ( "Code", TA_LEFT, 380 );
		ReportFile.AddColumn ( "Description", TA_LEFT, 300 );
	}

	ReportFile.AddColumn ( "Stock Qty", TA_RIGHT, 200 );
	ReportFile.AddColumn ( "Value", TA_RIGHT, 200 );
}

/**********************************************************************/

bool CStockAuditSales::ProcessLine( const char* szLine )
{
	CCSV csv( szLine );

	if ( csv.GetInt(1) != GetAuditDataType() )
		return FALSE;

	switch( csv.GetInt(2) )
	{
	case 1:
		m_nLineType = csv.GetInt(3);
		m_strLinePluNo = csv.GetString(4);
		m_strLineLoc = csv.GetString(5);
		m_strLineDescription = csv.GetString(6);
		m_dLinePluQty = csv.GetDouble(7);
		m_dLinePluValue = csv.GetDouble(8);
		m_strLineStockCode = csv.GetString(9);
		m_nLineSubUnits = csv.GetInt(10);
		m_dLineStockQty = csv.GetDouble(11);
		m_dLineStockValue = csv.GetDouble(12);
		m_strLineSaleDate = csv.GetString(13);
		m_strLineSaleTime = csv.GetString(14);
		break;

	case 3:
		m_nLineType = csv.GetInt(3);
		m_strLinePluNo = "";
		m_strLineLoc = "";
		m_strLineDescription = csv.GetString(5);
		m_dLinePluQty = 0.0;
		m_dLinePluValue = 0.0;
		m_strLineStockCode = csv.GetString(4);
		m_nLineSubUnits = csv.GetInt(12);
		m_dLineStockQty = csv.GetDouble(13);
		m_dLineStockValue = csv.GetDouble(14);
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

	m_bIsStockOnlyLine = FALSE;
	switch( m_nLineType )
	{
	case SALES_TYPE_MANUAL_STOCK:
	case SALES_TYPE_IMPORT_STOCK:
		m_bIsStockOnlyLine = TRUE;
		break;
	}

	switch( m_nLineType )
	{
	case SALES_TYPE_E1:
		m_bIncludeRecipeItems = m_bE1Sales;
		m_strLineType = "EPOS";
		return m_bE1Sales;

	case SALES_TYPE_MANUAL:
	case SALES_TYPE_MANUAL_STOCK:
		m_bIncludeRecipeItems = m_bManualSales;
		m_strLineType = "Manual";
		return m_bManualSales;

	case SALES_TYPE_IMPORT:
	case SALES_TYPE_IMPORT_STOCK:
		m_bIncludeRecipeItems = m_bImportSales;
		m_strLineType = "Import";
		return m_bImportSales;
	
	case SALES_TYPE_RECIPEITEM:
		return m_bIncludeRecipeItems;
		break;

	default:
		m_bIncludeRecipeItems = FALSE;
		return FALSE;
	}
}

/**********************************************************************/

void CStockAuditSales::WriteLine( CReportFile& ReportFile, int nDummySpIdx )
{
	CStockReportLine csvOut;

	if ( FALSE == m_bHidePluNo )
	{
		if ( m_nLineType == SALES_TYPE_RECIPEITEM )
			csvOut.AppendBlanks ( ( m_bE1Sales == TRUE ) ? 8 : 7 );
		else
		{
			csvOut.AppendString ( m_strHeaderDate );
			csvOut.AppendString ( m_strHeaderTime );
			csvOut.AppendString ( m_strLineType );

			if ( FALSE == m_bIsStockOnlyLine )
			{
				CString strPluNo = ReportHelpers.GetDisplayPluNo ( _atoi64(m_strLinePluNo) );
				csvOut.AppendString ( strPluNo );
						
				if ( m_bE1Sales == TRUE )
					csvOut.AppendString ( m_strLineLoc );
						
				csvOut.AppendString ( m_strLineDescription );
				csvOut.AppendStockQty ( m_dLinePluQty, 1, TRUE );
				csvOut.AppendVal ( m_dLinePluValue );
			}
			else
			{
				csvOut.AppendString( "Stock Sale" );
				csvOut.AppendString( "" );
				csvOut.AppendString( m_strLineDescription );
				csvOut.AppendString ( "" );
				csvOut.AppendString ( "" );
			}
		}

		csvOut.AppendString ( GetDisplayStockCode ( m_strLineStockCode ) );
		csvOut.AppendStockQty ( m_dLineStockQty, m_nLineSubUnits, TRUE );
		csvOut.AppendVal ( m_dLineStockValue );
	}
	else
	{
		csvOut.AppendString ( m_strHeaderDate );
		csvOut.AppendString ( m_strHeaderTime );
		csvOut.AppendString ( m_strLineType );
		csvOut.AppendString ( GetDisplayStockCode ( m_strLineStockCode ) );
		csvOut.AppendString ( m_strLineDescription );
		csvOut.AppendStockQty ( m_dLineStockQty, m_nLineSubUnits, TRUE );
		csvOut.AppendVal ( m_dLineStockValue );	
	}
						
	ReportFile.WriteLine ( csvOut.GetLine() );
}

/**********************************************************************/

