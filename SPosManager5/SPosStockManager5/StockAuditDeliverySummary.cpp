/**********************************************************************/
 
/**********************************************************************/
#include "CCSVReportLine.h"
//#include "globalfunctions.h"
#include "PriceHelpers.h"
#include "RepCsv.h"
#include "StockpointCSVArray.h"
/**********************************************************************/
#include "StockAuditDeliverySummary.h"
/**********************************************************************/

CStockAuditDeliverySummary::CStockAuditDeliverySummary() : CStockAuditBaseReport() 
{
	m_dSessionStockUnitQty = 0.0;
	m_dSessionCaseQty = 0.0;
	m_dSessionOrderCost = 0.0;
	m_dSpStockUnitQty = 0.0;
	m_dSpCaseQty = 0.0;
	m_dSpOrderCost = 0.0;
	m_dTotalStockUnitQty = 0.0;
	m_dTotalCaseQty = 0.0;
	m_dTotalOrderCost = 0.0;
}

/**********************************************************************/

void CStockAuditDeliverySummary::AddColumns( CReportFile& ReportFile )
{
	ReportFile.SetStyle1 ( GetReportTitle() );
	ReportFile.AddColumn ( "Date", TA_LEFT, 200 );
	ReportFile.AddColumn ( "Time", TA_LEFT, 200 );
	ReportFile.AddColumn ( "Supplier", TA_LEFT, 400 );
	ReportFile.AddColumn ( "Reference", TA_LEFT, 400 );
	ReportFile.AddColumn ( "DU Qty", TA_RIGHT, 200 );
	ReportFile.AddColumn ( "Stock Qty", TA_RIGHT, 250 );
	ReportFile.AddColumn ( "Cost", TA_RIGHT, 200 );
}

/**********************************************************************/

bool CStockAuditDeliverySummary::ProcessHeader( const char* szHeader )
{
	m_dSessionStockUnitQty = 0.0;
	m_dSessionCaseQty = 0.0;
	m_dSessionOrderCost = 0.0;
	return CStockAuditBaseReport::ProcessHeader( szHeader );
}

/**********************************************************************/

bool CStockAuditDeliverySummary::ProcessLine( const char* szLine )
{
	CCSV csv( szLine );

	if ( csv.GetInt(1) != GetAuditDataType() )
		return FALSE;

	switch( csv.GetInt(2) )
	{
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
		{
			CString strStockCode = csv.GetString(3);

			if ( m_strStockCodeFrom != "" )
				if ( strStockCode < m_strStockCodeFrom )
					return FALSE;
		
			if ( m_strStockCodeTo != "" )
				if ( strStockCode > m_strStockCodeTo )
					return FALSE;
	
			m_dSessionStockUnitQty += csv.GetDouble(7);
			m_dSessionCaseQty += csv.GetDouble(6);
			m_dSessionOrderCost += csv.GetDouble(9);
		}
		return TRUE;
		
	default:
		return FALSE;
	}
}

/**********************************************************************/

void CStockAuditDeliverySummary::AppendTotals ( CReportFile& ReportFile )
{
	CString strReference = "";
	::FormatAuditReference( m_strHeaderReference1, m_strHeaderReference2, strReference );

	CStockReportLine csvTotals;
	csvTotals.AppendString ( m_strHeaderDate );
	csvTotals.AppendString ( m_strHeaderTime );
	csvTotals.AppendString ( m_strHeaderSuppName );
	csvTotals.AppendString ( strReference );
	csvTotals.AppendStockQty ( m_dSessionCaseQty, 1, TRUE );
	csvTotals.AppendStockQty ( m_dSessionStockUnitQty, 1, TRUE );
	csvTotals.AppendVal ( m_dSessionOrderCost );
	ReportFile.WriteLine ( csvTotals.GetLine() );

	m_dSpCaseQty += m_dSessionCaseQty;
	m_dSpStockUnitQty += m_dSessionStockUnitQty;
	m_dSpOrderCost += m_dSessionOrderCost;
	m_dSessionStockUnitQty = 0.0;
	m_dSessionCaseQty = 0.0;
	m_dSessionOrderCost = 0.0;
}

/**********************************************************************/

void CStockAuditDeliverySummary::AppendStockpointTotals ( CReportFile& ReportFile )
{
	CStockReportLine csvTotals;
	ReportFile.WriteLine ( "\t\t\t\t<LI>\t<LI>\t<LI>\t<LI>" );
	csvTotals.AppendBlanks(2);
	csvTotals.AppendString ( "Stockpoint Totals" );
	csvTotals.AppendBlanks(1);
	csvTotals.AppendStockQty ( m_dSpCaseQty, 1, TRUE );
	csvTotals.AppendStockQty ( m_dSpStockUnitQty, 1, TRUE );
	csvTotals.AppendVal ( m_dSpOrderCost );
	ReportFile.WriteLine ( csvTotals.GetLine() );
	ReportFile.WriteLine ( "" );

	m_dTotalCaseQty += m_dSpCaseQty;
	m_dTotalStockUnitQty += m_dSpStockUnitQty;
	m_dTotalOrderCost += m_dSpOrderCost;
	m_dSpStockUnitQty = 0.0;
	m_dSpCaseQty = 0.0;
	m_dSpOrderCost = 0.0;
}

/**********************************************************************/

void CStockAuditDeliverySummary::AppendGrandTotals ( CReportFile& ReportFile )
{
	CStockReportLine csvTotals;
	ReportFile.WriteLine ( "\t\t\t\t<LI>\t<LI>\t<LI>\t<LI>" );
	csvTotals.AppendBlanks(2);
	csvTotals.AppendString ( "Grand Totals" );
	csvTotals.AppendBlanks(1);
	csvTotals.AppendStockQty ( m_dTotalCaseQty, 1, TRUE );
	csvTotals.AppendStockQty ( m_dTotalStockUnitQty, 1, TRUE );
	csvTotals.AppendVal ( m_dTotalOrderCost );
	ReportFile.WriteLine ( csvTotals.GetLine() );
	ReportFile.WriteLine ( "" );					
}

/**********************************************************************/
