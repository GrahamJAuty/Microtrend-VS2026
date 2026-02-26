/**********************************************************************/
#include "CCSVReportLine.h"
//#include "DealerFlags.h"
#include "RepCSV.h"
#include "StockOptionsIni.h"
#include "StockPointCSVArray.h"
#include "SysInfo.h"
/**********************************************************************/
#include "StockAuditImportPlu.h"
/**********************************************************************/

CStockAuditImportPlu::CStockAuditImportPlu() : CStockAuditBaseReport()
{
}

/**********************************************************************/

void CStockAuditImportPlu::AddColumns( CReportFile& ReportFile )
{
	ReportFile.SetStyle1 ( GetReportName () );

	if ( DealerFlags.GetImportBarcodeFlag() == TRUE )
		ReportFile.AddColumn ( "Barcode No", TA_LEFT, 380 );

	ReportFile.AddColumn ( "Plu No", TA_LEFT, 380 );
	ReportFile.AddColumn ( "Description", TA_LEFT, 380 );
	ReportFile.AddColumn ( "Quantity", TA_RIGHT, 200 );
		
	switch ( m_nReportType )
	{
	case AUDIT_IMPORT_PLU_DELIVERY:
		if ( StockOptions.GetImportAdhocValueFlag() == TRUE )
			ReportFile.AddColumn ( "Value", TA_RIGHT, 200 );
		break;

	case AUDIT_IMPORT_PLU_RETURNS:
		if ( StockOptions.GetImportReturnValueFlag() == TRUE )
			ReportFile.AddColumn ( "Value", TA_RIGHT, 200 );
		break;

	case AUDIT_IMPORT_PLU_ADJUSTMENT_STOCK:
		if ( StockOptions.GetImportAdjustValueFlag() == TRUE )
			ReportFile.AddColumn ( "Value", TA_RIGHT, 200 );
		break;
	}

	ReportFile.AddColumn ( "Problem", TA_LEFT, 600 );
}

/**********************************************************************/

void CStockAuditImportPlu::WriteHeader( CReportFile& ReportFile )
{
	CString strHeader;

	if ( m_nReportType != AUDIT_IMPORT_PLU_TRANSFER )
	{
		strHeader.Format ( "<..>%s",
			(const char*) m_strHeaderImportPath );
	}
	else
	{
		strHeader.Format ( "<..>%s to %s, %s",
			(const char*) m_strHeaderSpName1,
			(const char*) m_strHeaderSpName2,
			(const char*) m_strHeaderImportPath );
	}

	ReportFile.WriteLine( strHeader );
	ReportFile.WriteLine( "<LI>" );
}

/**********************************************************************/

bool CStockAuditImportPlu::ProcessLine( const char* szLine )
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
		m_strLineQty = csv.GetString(5);
		m_strLineValue = csv.GetString(6);
		m_strLineProblem = csv.GetString(7);
		return TRUE;

	case 2:
		m_strLineBarcodeNo = csv.GetString(3);
		m_strLinePluNo = csv.GetString(4);
		m_strLineDescription = csv.GetString(5);
		m_strLineQty = csv.GetString(6);
		m_strLineValue = csv.GetString(7);
		m_strLineProblem = csv.GetString(8);
		return TRUE;

	default:
		return FALSE;
	}
}

/**********************************************************************/

void CStockAuditImportPlu::WriteLine( CReportFile& ReportFile, int nDummySpIdx )
{	
	CCSV csvOut ( '\t' );
	
	if ( DealerFlags.GetImportBarcodeFlag() == TRUE )
		csvOut.Add( m_strLineBarcodeNo );

	csvOut.Add ( m_strLinePluNo );
	csvOut.Add ( m_strLineDescription );
	csvOut.Add ( m_strLineQty );

	switch ( m_nReportType )
	{
	case AUDIT_IMPORT_PLU_DELIVERY:
		if ( StockOptions.GetImportAdhocValueFlag() == TRUE )
			csvOut.Add ( m_strLineValue );
		break;

	case AUDIT_IMPORT_PLU_RETURNS:
		if ( StockOptions.GetImportReturnValueFlag() == TRUE )
			csvOut.Add ( m_strLineValue );
		break;

	case AUDIT_IMPORT_PLU_ADJUSTMENT_STOCK:
		if ( StockOptions.GetImportAdjustValueFlag() == TRUE )
			csvOut.Add ( m_strLineValue );
		break;
	}

	csvOut.Add ( m_strLineProblem );
	ReportFile.WriteLine ( csvOut.GetLine() );	
}

/**********************************************************************/

