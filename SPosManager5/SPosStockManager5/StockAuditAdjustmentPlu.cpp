/**********************************************************************/
#include "CCSVReportLine.h"
//#include "DataManager.h"
//#include "globalfunctions.h"
#include "RepCsv.h"
#include "ReportHelpers.h"
#include "SalesHistoryFields.h"
#include "StockpointCSVArray.h"
/**********************************************************************/
#include "StockAuditAdjustmentPlu.h"
/**********************************************************************/

CStockAuditAdjustmentPlu::CStockAuditAdjustmentPlu() : CStockAuditBaseReport() 
{
}

/**********************************************************************/

void CStockAuditAdjustmentPlu::AddColumns( CReportFile& ReportFile )
{
	ReportFile.SetStyle1 ( GetReportTitle() );
	ReportFile.AddColumn ( "Plu No", TA_LEFT, 350 );	
	ReportFile.AddColumn ( "Description", TA_LEFT, 300 );
	ReportFile.AddColumn ( "Quantity", TA_RIGHT, 200 );
	ReportFile.AddColumn ( "Value", TA_RIGHT, 200 );
	ReportFile.AddColumn ( "Reason", TA_LEFT, 300 );
	ReportFile.AddColumn ( "Code", TA_LEFT, 380 );
	ReportFile.AddColumn ( "Stock Qty", TA_RIGHT, 200 );
	ReportFile.AddColumn ( "Value", TA_RIGHT, 200 );
	ReportFile.AddColumn ( "Cost Now", TA_RIGHT, 200 );
}

/**********************************************************************/

bool CStockAuditAdjustmentPlu::ProcessHeader( const char* szHeader )
{
	m_dSessionStockQty = 0.0;
	m_dSessionStockValue = 0.0;
	m_dSessionStockCost = 0.0;
	return CStockAuditBaseReport::ProcessHeader( szHeader );
}

/**********************************************************************/

bool CStockAuditAdjustmentPlu::ProcessLine( const char* szLine )
{
	CCSV csv( szLine );

	switch( csv.GetInt(1) )
	{
	case AUDIT_ADJUSTMENT_STOCK:
		switch( csv.GetInt(2) )
		{
		case 1:
			m_nLineType = 2;
			m_strLinePluNo = "Stock Adjustment";
			m_strLineDescription = csv.GetString(4);
			m_dLinePluQty = 0.0;
			m_dLinePluValue = 0.0;
			m_strLineStockCode = csv.GetString(3);
			m_nLineSubUnits = csv.GetInt(6);
			m_dLineStockQty = csv.GetDouble(7);
			m_dLineStockValue = csv.GetDouble(8);
			m_strLineReason = csv.GetString(9);
			break;

		default:
			return FALSE;
		}	
		break;

	case AUDIT_ADJUSTMENT_PLU:
		switch( csv.GetInt(2) )
		{
		case 1:
			m_nLineType = csv.GetInt(3);
			m_strLinePluNo = csv.GetString(4);
			m_strLineDescription = csv.GetString(5);
			m_dLinePluQty = csv.GetDouble(6);
			m_dLinePluValue = csv.GetDouble(7);
			m_strLineStockCode = csv.GetString(8);
			m_nLineSubUnits = csv.GetInt(9);
			m_dLineStockQty = csv.GetDouble(10);
			m_dLineStockValue = csv.GetDouble(11);
			m_strLineReason = csv.GetString(12);
			break;

		default:
			return FALSE;
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

	bool bResult = TRUE;

	if ( m_strLineReferenceFilter != "" )
	{
		CString strTempRef1 = m_strLineReferenceFilter;
		CString strTempRef2 = m_strLineReason;

		strTempRef1.MakeUpper();
		strTempRef2.MakeUpper();

		bResult = ( strTempRef2.Find( strTempRef1 ) != -1 );
	}

	m_dLineStockCost = 0.0;

	if ( TRUE == bResult )
	{
		int nStockIdx;
		if ( DataManager.Stock.FindStockItemByCode( m_strLineStockCode, nStockIdx ) == TRUE )
		{
			CStockCSVRecord StockRecord;
			DataManager.Stock.GetAt( nStockIdx, StockRecord );
			m_dLineStockCost = m_dLineStockQty * StockRecord.GetUnitCost(0);
		}

		m_dSessionStockQty += m_dLineStockQty;
		m_dSessionStockValue += m_dLineStockValue;
		m_dSessionStockCost += m_dLineStockCost;
	}

	return bResult;
}

/**********************************************************************/

void CStockAuditAdjustmentPlu::WriteHeader( CReportFile& ReportFile )
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

void CStockAuditAdjustmentPlu::WriteLine( CReportFile& ReportFile, int nDummySpIdx )
{
	CStockReportLine csvOut;

	if ( 1 == m_nLineType )
		csvOut.AppendBlanks ( 5 );
	else
	{
		CString strPluNo = ReportHelpers.GetDisplayPluNo ( _atoi64(m_strLinePluNo) );
		csvOut.AppendString ( strPluNo );			
		csvOut.AppendString ( m_strLineDescription );
		
		if ( m_nLineType != 2 )
		{
			csvOut.AppendStockQty ( m_dLinePluQty, 1, TRUE );
			csvOut.AppendVal ( m_dLinePluValue );
		}
		else
			csvOut.AppendBlanks( 2 );

		csvOut.AppendString( m_strLineReason );
	}

	csvOut.AppendString ( GetDisplayStockCode ( m_strLineStockCode ) );
	csvOut.AppendStockQty ( m_dLineStockQty, m_nLineSubUnits, TRUE );
	csvOut.AppendVal ( m_dLineStockValue );
	csvOut.AppendVal ( m_dLineStockCost );
						
	ReportFile.WriteLine ( csvOut.GetLine() );
}

/**********************************************************************/

void CStockAuditAdjustmentPlu::AppendTotals ( CReportFile& ReportFile )
{
	CStockReportLine csvTotals;
	CCSV csvUnderline( '\t' );
		
	csvTotals.AppendBlanks( 1 );
	csvTotals.AppendString ( "Totals" );
	csvTotals.AppendBlanks( 4 );
		
	csvUnderline.Add ( "" );
	csvUnderline.Add ( "" );
	csvUnderline.Add ( "" );
	csvUnderline.Add ( "" );
	csvUnderline.Add ( "" );
	csvUnderline.Add ( "" );

	csvTotals.AppendStockQty ( m_dSessionStockQty, 1, TRUE );
	csvTotals.AppendVal ( m_dSessionStockValue );
	csvTotals.AppendVal ( m_dSessionStockCost );
	csvUnderline.Add ( "<LI>" );
	csvUnderline.Add ( "<LI>" );
	csvUnderline.Add ( "<LI>" );

	ReportFile.WriteLine( csvUnderline.GetLine() );
	ReportFile.WriteLine( csvTotals.GetLine() );	
	ReportFile.WriteLine ( "" );

	if ( m_bPageBreakFlag == TRUE )
		ReportFile.WriteLine ( "<NEWPAGE>" );
}

/**********************************************************************/

