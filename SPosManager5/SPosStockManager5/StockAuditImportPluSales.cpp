/**********************************************************************/
#include "CCSVReportLine.h"
//#include "DealerFlags.h"
#include "RepCSV.h"
#include "StockOptionsIni.h"
#include "StockPointCSVArray.h"
#include "SysInfo.h"
/**********************************************************************/
#include "StockAuditImportPluSales.h"
/**********************************************************************/

CStockAuditImportPluSales::CStockAuditImportPluSales( int nLineType ) : CStockAuditBaseReport()
{
	if ( nLineType < 0 )
		m_nLineType = DealerFlags.GetPluCSVImportType();
	else
		m_nLineType = nLineType;
}

/**********************************************************************/

void CStockAuditImportPluSales::AddColumns( CReportFile& ReportFile )
{
	ReportFile.SetStyle1 ( GetReportName () );

	if ( DealerFlags.GetImportBarcodeFlag() == TRUE )
		ReportFile.AddColumn ( "Barcode No", TA_LEFT, 380 );
	
	ReportFile.AddColumn ( "Plu No", TA_LEFT, 380 );
	ReportFile.AddColumn ( "Description", TA_LEFT, 380 );

	switch( m_nLineType )
	{
	case 1:
		ReportFile.AddColumn ( "Quantity", TA_RIGHT, 200 );
		break;

	case 2:
		ReportFile.AddColumn ( "Level (FILE)", TA_RIGHT, 280 );
		ReportFile.AddColumn ( "Quantity", TA_RIGHT, 200 );
		ReportFile.AddColumn ( "Value", TA_RIGHT, 200 );
		break;

	case 3:
		ReportFile.AddColumn ( "Level (FILE)", TA_RIGHT, 280 );
		ReportFile.AddColumn ( "Quantity", TA_RIGHT, 200 );
		break;

	case 0:
	default:
		ReportFile.AddColumn ( "Quantity", TA_RIGHT, 200 );
		ReportFile.AddColumn ( "Value", TA_RIGHT, 200 );
		break;
	}

	ReportFile.AddColumn ( "Problem", TA_LEFT, 600 );
}

/**********************************************************************/

void CStockAuditImportPluSales::WriteHeader( CReportFile& ReportFile )
{
	CString strHeader;

	strHeader.Format ( "<..>%s",
		(const char*) m_strHeaderImportPath );
	
	ReportFile.WriteLine( strHeader );
	ReportFile.WriteLine( "<LI>" );
}

/**********************************************************************/

bool CStockAuditImportPluSales::ProcessLine( const char* szLine )
{
	CCSV csv( szLine );

	if ( csv.GetInt(1) != m_nReportType )
		return FALSE;

	switch( csv.GetInt(2) )
	{
	case 1:
		m_strLineBarcodeNo = "";
		m_strLinePluNo = csv.GetString(3);
		m_strLineDescription = csv.GetString(4);
		m_strLineLevel = csv.GetString(5);
		m_strLineQty = csv.GetString(6);
		m_strLineValue = csv.GetString(7);
		m_strLineProblem = csv.GetString(8);
		return TRUE;

	case 2:
		m_strLineBarcodeNo = csv.GetString(3);
		m_strLinePluNo = csv.GetString(4);
		m_strLineDescription = csv.GetString(5);
		m_strLineLevel = csv.GetString(6);
		m_strLineQty = csv.GetString(7);
		m_strLineValue = csv.GetString(8);
		m_strLineProblem = csv.GetString(9);
		return TRUE;

	default:
		return FALSE;
	}
}

/**********************************************************************/

void CStockAuditImportPluSales::WriteLine( CReportFile& ReportFile, int nDummySpIdx )
{	
	CCSV csvOut ( '\t' );
	
	if ( DealerFlags.GetImportBarcodeFlag() == TRUE )
		csvOut.Add( m_strLineBarcodeNo );

	csvOut.Add ( m_strLinePluNo );
	csvOut.Add ( m_strLineDescription );

	switch( m_nLineType )
	{
	case 1:
		csvOut.Add( m_strLineQty );
		break;

	case 2:
		csvOut.Add( m_strLineLevel );
		csvOut.Add( m_strLineQty );
		csvOut.Add( m_strLineValue );
		break;

	case 3:
		csvOut.Add( m_strLineLevel );
		csvOut.Add( m_strLineQty );
		break;

	case 0:
	default:
		csvOut.Add( m_strLineQty );
		csvOut.Add( m_strLineValue );
		break;
	}

	csvOut.Add ( m_strLineProblem );
	ReportFile.WriteLine ( csvOut.GetLine() );	
}

/**********************************************************************/

