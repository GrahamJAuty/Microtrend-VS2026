/**********************************************************************/
#include "CCSVReportLine.h"
//#include "globalfunctions.h"
#include "ReportHelpers.h"
#include "SalesHistoryFields.h"
#include "SalesTypes.h"
#include "StockpointCSVArray.h"
#include "SysInfo.h"
/**********************************************************************/
#include "StockAuditExceptions.h"
/**********************************************************************/

CStockAuditExceptionTypes::CStockAuditExceptionTypes() 
{
	m_bOpening = FALSE;
	m_bDelivery = FALSE;
	m_bReturns = FALSE;
	m_bSales = FALSE;
	m_bAdjustment = FALSE;
	m_bTransfer = FALSE;
	m_bStocktaking = FALSE;
	m_bShowHeaders = FALSE;
}

/**********************************************************************/

CStockAuditExceptions::CStockAuditExceptions( CStockAuditExceptionTypes types ) : CStockAuditBaseReport(), m_Types( types )
{
	m_Types = types;
	m_bPageBreakFlag = FALSE;
	m_nHeaderType = 0;
}

/**********************************************************************/

CStockAuditExceptions::CStockAuditExceptions( int nType )
{
	SetReportType( nType );
	m_bPageBreakFlag = FALSE;
	m_nHeaderType = 0;

	switch ( nType )
	{
	case AUDIT_EX_DELIVERY:			m_Types.m_bDelivery = TRUE;			break;
	case AUDIT_EX_ADJUSTMENT_STOCK:	m_Types.m_bAdjustment = TRUE;		break;
	case AUDIT_EX_OPENING:			m_Types.m_bOpening = TRUE;			break;
	case AUDIT_EX_RETURNS:			m_Types.m_bReturns = TRUE;			break;
	case AUDIT_EX_TRANSFER:			m_Types.m_bTransfer = TRUE;			break;
	case AUDIT_EX_CLOSING:			m_Types.m_bStocktaking = TRUE;		break;
	case AUDIT_EX_SALES:			m_Types.m_bSales = TRUE;			break;
	}

	m_Types.m_bShowHeaders = TRUE;
}

/**********************************************************************/

void CStockAuditExceptions::AddColumns( CReportFile& ReportFile )
{
	ReportFile.SetStyle1 ( GetReportTitle() );
	ReportFile.AddColumn ( "Type", TA_LEFT, 320 );
	ReportFile.AddColumn ( "Plu No", TA_LEFT, 350 );
	ReportFile.AddColumn ( "Quantity", TA_RIGHT, 180 );
	ReportFile.AddColumn ( "Code", TA_LEFT, 380 );
	ReportFile.AddColumn ( "Problem", TA_LEFT, 600 );
}

/**********************************************************************/

bool CStockAuditExceptions::ProcessHeader( const char* szHeader )
{
	CCSV csv ( szHeader );
	
	bool bAccept = FALSE;
	switch ( csv.GetInt(1) )
	{
	case AUDIT_EX_OPENING:			bAccept = m_Types.m_bOpening;		break;
	case AUDIT_EX_DELIVERY:			bAccept = m_Types.m_bDelivery;		break;
	case AUDIT_EX_RETURNS:			bAccept = m_Types.m_bReturns;		break;
	case AUDIT_EX_SALES:			bAccept = m_Types.m_bSales;			break;
	case AUDIT_EX_ADJUSTMENT_STOCK:	bAccept = m_Types.m_bAdjustment;	break;
	case AUDIT_EX_ADJUSTMENT_PLU:	bAccept = m_Types.m_bAdjustment;	break;
	case AUDIT_EX_TRANSFER:			bAccept = m_Types.m_bTransfer;		break;
	case AUDIT_EX_CLOSING:			bAccept = m_Types.m_bStocktaking;	break;
	}

	if ( bAccept == FALSE )
		return FALSE;

	m_nHeaderType = csv.GetInt(1);

	switch ( csv.GetInt(2) )
	{
	case 1:
		m_strHeaderDate = csv.GetString(3);
		m_strHeaderTime = csv.GetString(4);
		m_strHeaderReference1 = csv.GetString(5);
		m_strHeaderReference2 = "";
		m_strHeaderSuppName = csv.GetString(6);
		m_strHeaderSpName1 = csv.GetString(7);
		m_strHeaderSpName2 = csv.GetString(8);
		m_strHeaderImportPath = csv.GetString(9);
		break;

	case 2:
		m_strHeaderDate = csv.GetString(3);
		m_strHeaderTime = csv.GetString(4);
		m_strHeaderReference1 = csv.GetString(5);
		m_strHeaderReference2 = csv.GetString(6);
		m_strHeaderSuppName = csv.GetString(7);
		m_strHeaderSpName1 = csv.GetString(8);
		m_strHeaderSpName2 = csv.GetString(9);
		m_strHeaderImportPath = csv.GetString(10);
		break;

	default:
		m_strHeaderDate = "";
		m_strHeaderTime = "";
		m_strHeaderReference1 = "";
		m_strHeaderReference2 = "";
		m_strHeaderSuppName = "";
		m_strHeaderSpName1 = "";
		m_strHeaderSpName2 = "";
		m_strHeaderImportPath = "";
		break;
	}

	CString strTempRef1 = m_strHeaderReferenceFilter;
	CString strTempRef2 = m_strHeaderReference1;
	CString strTempRef3 = m_strHeaderReference2;

	strTempRef1.MakeUpper();
	strTempRef2.MakeUpper();
	strTempRef3.MakeUpper();

	bool bFound = FALSE;

	if ( strTempRef1 != "" )
	{
		bFound |= ( strTempRef2.Find( strTempRef1 ) != -1 );
		bFound |= ( strTempRef3.Find( strTempRef1 ) != -1 );
	}
	else
		bFound = TRUE;

	return bFound;
}

/**********************************************************************/

bool CStockAuditExceptions::ProcessLine( const char* szLine )
{
	CCSV csv( szLine );

	if ( csv.GetInt(1) != m_nHeaderType )
		return FALSE;

	switch( m_nHeaderType )
	{
	case AUDIT_EX_SALES:
		switch( csv.GetInt(2) )
		{
		case 1:
			m_nLineType = csv.GetInt(3);
			m_strLinePluNo = csv.GetString(4);
			m_dLineQty = csv.GetDouble(6);
			m_strLineStockCode = csv.GetString(8);
			m_strLineProblem = csv.GetString(9);
			m_strLineSaleDate = csv.GetString(10);
			m_strLineSaleTime = csv.GetString(11);
			m_bLineIsWrapper = FALSE;
			return TRUE;

		case 2:
			m_nLineType = csv.GetInt(3);
			m_strLinePluNo = csv.GetString(4);
			m_dLineQty = csv.GetDouble(6);
			m_strLineStockCode = csv.GetString(8);
			m_strLineProblem = csv.GetString(9);
			m_strLineSaleDate = csv.GetString(10);
			m_strLineSaleTime = csv.GetString(11);
			m_bLineIsWrapper = csv.GetBool(12);
			return TRUE;

		default:
			return FALSE;
		}
		break;

	case AUDIT_EX_ADJUSTMENT_PLU:
		switch( csv.GetInt(2) )
		{
		case 1:
			m_strLinePluNo = csv.GetString(3);
			m_dLineQty = csv.GetDouble(4);
			m_strLineStockCode = csv.GetString(7);
			m_strLineProblem = csv.GetString(8);
			m_bLineIsWrapper = FALSE;
			m_strLineSaleDate = "";
			m_strLineSaleTime = "";
			return TRUE;

		default:
			return FALSE;
		}
		break;

	default:
		switch( csv.GetInt(2) )
		{
		case 1:
			m_strLinePluNo = csv.GetString(3);
			m_dLineQty = csv.GetDouble(4);
			m_strLineStockCode = csv.GetString(5);
			m_strLineProblem = csv.GetString(6);
			m_strLineSaleDate = csv.GetString(7);
			m_strLineSaleTime = csv.GetString(8);
			m_bLineIsWrapper = FALSE;
			return TRUE;

		case 2:
			m_strLinePluNo = csv.GetString(3);
			m_dLineQty = csv.GetDouble(4);
			m_strLineStockCode = csv.GetString(5);
			m_strLineProblem = csv.GetString(6);
			m_strLineSaleDate = csv.GetString(7);
			m_strLineSaleTime = csv.GetString(8);
			m_bLineIsWrapper = csv.GetBool(9);
			return TRUE;
	
		default:
			return FALSE;
		}
		break;
	}
}

/**********************************************************************/

void CStockAuditExceptions::WriteHeader( CReportFile& ReportFile )
{
	if ( m_Types.m_bShowHeaders == FALSE )
		return;

	CString strReference = "";
	::FormatAuditReference( m_strHeaderReference1, m_strHeaderReference2, strReference );
	
	CString strLine;

	switch( m_nHeaderType )
	{
	case AUDIT_EX_OPENING:
	case AUDIT_EX_SALES:
		strLine.Format ( "<..>%s %s %s",  
			GetHeaderTypeString(),
			( const char* ) m_strHeaderDate,
			( const char* ) m_strHeaderTime );

		break;

	default:
		strLine.Format ( "<..>%s %s %s ( reference %s )",  
			GetHeaderTypeString(),
			( const char* ) m_strHeaderDate,
			( const char* ) m_strHeaderTime,
			( const char* ) strReference );

		break;
	}
						
	ReportFile.WriteLine ( strLine );
}

/**********************************************************************/

void CStockAuditExceptions::WriteLine( CReportFile& ReportFile, int nDummySpIdx )
{
	CStockReportLine csvOut;
	csvOut.AppendString ( GetHeaderTypeString() );
	
	if ( m_bLineIsWrapper == FALSE )
		csvOut.AppendString ( ReportHelpers.GetDisplayPluNo ( _atoi64(m_strLinePluNo ) ) );	
	else
		csvOut.AppendString ( m_strLinePluNo );

	csvOut.AppendStockQty ( m_dLineQty, 1, TRUE, SysInfo.GetDPQty() );
	csvOut.AppendString ( GetDisplayStockCode ( m_strLineStockCode ) );
	csvOut.AppendString ( m_strLineProblem );						
	ReportFile.WriteLine ( csvOut.GetLine() );
}

/**********************************************************************/

void CStockAuditExceptions::AppendTotals ( CReportFile& ReportFile )
{
	if ( m_bPageBreakFlag == TRUE )
		ReportFile.WriteLine ( "<NEWPAGE>" );
	else if ( m_Types.m_bShowHeaders == TRUE )
		ReportFile.WriteLine( "" );
}

/**********************************************************************/

const char* CStockAuditExceptions::GetHeaderTypeString()
{
	m_strHeaderType = "";
	
	switch( m_nHeaderType )
	{
	case AUDIT_EX_DELIVERY:			m_strHeaderType = "Deliveries";		break;
	case AUDIT_EX_ADJUSTMENT_STOCK:	m_strHeaderType = "Adjustments";	break;
	case AUDIT_EX_ADJUSTMENT_PLU:	m_strHeaderType = "Adjustments";	break;
	case AUDIT_EX_OPENING:			m_strHeaderType = "Opening Stock";	break;
	case AUDIT_EX_RETURNS:			m_strHeaderType = "Returns";		break;
	case AUDIT_EX_TRANSFER:			m_strHeaderType = "Transfers";		break;
	case AUDIT_EX_CLOSING:			m_strHeaderType = "Stocktaking";	break;
	case AUDIT_EX_SALES:
		switch( m_nLineType )
		{
		case SALES_TYPE_E1:			m_strHeaderType = "EPOS Sale";			break;
		case SALES_TYPE_IMPORT:		m_strHeaderType = "Import Sale";		break;
		case SALES_TYPE_MANUAL:		m_strHeaderType = "PMS Manual Sale";	break;
		}
		break;
	}

	return m_strHeaderType;
}

/**********************************************************************/
