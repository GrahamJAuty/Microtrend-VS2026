/**********************************************************************/
#include "CCSVReportLine.h"
#include "RepCSV.h"
#include "StockOptionsIni.h"
#include "StockPointCSVArray.h"
#include "SysInfo.h"
/**********************************************************************/
#include "StockAuditImportWrapper.h"
/**********************************************************************/

CStockAuditImportWrapper::CStockAuditImportWrapper() : CStockAuditBaseReport()
{
}

/**********************************************************************/

void CStockAuditImportWrapper::AddColumns( CReportFile& ReportFile )
{
	ReportFile.SetStyle1 ( GetReportName () );

	ReportFile.AddColumn ( "Wrapper", TA_LEFT, 380 );
	ReportFile.AddColumn ( "Quantity", TA_RIGHT, 200 );
		
	switch ( m_nReportType )
	{
	case AUDIT_IMPORT_WRAPPER_DELIVERY:
		if ( StockOptions.GetImportAdhocValueFlag() == TRUE )
			ReportFile.AddColumn ( "Value", TA_RIGHT, 200 );
		break;

	case AUDIT_IMPORT_WRAPPER_RETURNS:
		if ( StockOptions.GetImportReturnValueFlag() == TRUE )
			ReportFile.AddColumn ( "Value", TA_RIGHT, 200 );
		break;

	case AUDIT_IMPORT_WRAPPER_SALES:
		ReportFile.AddColumn ( "Value", TA_RIGHT, 200 );
		break;
	}

	ReportFile.AddColumn ( "Problem", TA_LEFT, 600 );
}

/**********************************************************************/

void CStockAuditImportWrapper::WriteHeader( CReportFile& ReportFile )
{
	CString strHeader;

	if ( m_nReportType != AUDIT_IMPORT_WRAPPER_TRANSFER )
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

bool CStockAuditImportWrapper::ProcessLine( const char* szLine )
{
	CCSV csv( szLine );

	if ( csv.GetInt(1) != m_nReportType )
		return FALSE;

	switch( csv.GetInt(2) )
	{
	case 1:
		m_strLineWrapper = csv.GetString(3);
		m_strLineQty = csv.GetString(4);
		m_strLineValue = csv.GetString(5);
		m_strLineProblem = csv.GetString(6);
		return TRUE;

	default:
		return FALSE;
	}
}

/**********************************************************************/

void CStockAuditImportWrapper::WriteLine( CReportFile& ReportFile, int nDummySpIdx )
{	
	CCSV csvOut ( '\t' );
	csvOut.Add ( m_strLineWrapper );
	csvOut.Add ( m_strLineQty );

	switch ( m_nReportType )
	{
	case AUDIT_IMPORT_WRAPPER_DELIVERY:
		if ( StockOptions.GetImportAdhocValueFlag() == TRUE )
			csvOut.Add ( m_strLineValue );
		break;

	case AUDIT_IMPORT_WRAPPER_RETURNS:
		if ( StockOptions.GetImportReturnValueFlag() == TRUE )
			csvOut.Add ( m_strLineValue );
		break;

	case AUDIT_IMPORT_WRAPPER_SALES:			
		csvOut.Add ( m_strLineValue );
		break;
	}

	csvOut.Add ( m_strLineProblem );
	ReportFile.WriteLine ( csvOut.GetLine() );	
}

/**********************************************************************/

