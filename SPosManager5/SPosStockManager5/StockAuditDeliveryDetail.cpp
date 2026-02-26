/**********************************************************************/
 
/**********************************************************************/
#include "CCSVReportLine.h"
//#include "DealerFlags.h"
//#include "EcrmanOptionsIni.h"
//#include "globalfunctions.h"
#include "PriceHelpers.h"
#include "ReportHelpers.h"
#include "UnderlineMaker.h"
/**********************************************************************/
#include "StockAuditDeliveryDetail.h"
/**********************************************************************/

CStockAuditDeliveryBuffer::CStockAuditDeliveryBuffer( int nBufferType )
{
	m_nBufferType = nBufferType;
	Reset();
}

/**********************************************************************/

void CStockAuditDeliveryBuffer::Reset()
{
	m_strStockCode = "";
	m_strDescription = "";
	m_strCategory = "";
	m_strSuppRef = "";
	m_nDUItems = 0;
	m_dDUItemSize = 0;
	m_dCaseQty = 0.0;
	m_dCaseCost = 0.0;
	m_dCaseRetail = 0.0;
	m_dStockUnitQty = 0.0;
	m_dTotalCost = 0.0;
	m_dTotalCostTax = 0.0;
	m_dTotalCostWaste = 0.0;
	m_dTotalRetail = 0.0;
	m_dTotalRetailTax = 0.0;
	m_dGPPercent = 0.0;
	m_nHighestDataVersion = 0;
	m_bUnknownFormat = FALSE;
}

/**********************************************************************/

void CStockAuditDeliveryBuffer::Consolidate( CStockAuditDeliveryBuffer& source )
{
	m_dCaseQty += source.m_dCaseQty;
	m_dStockUnitQty += source.m_dStockUnitQty;
	m_dTotalCost += source.m_dTotalCost;
	m_dTotalCostTax += source.m_dTotalCostTax;
	m_dTotalCostWaste += source.m_dTotalCostWaste;
	m_dTotalRetail += source.m_dTotalRetail;
	m_dTotalRetailTax += source.m_dTotalRetailTax;

	if ( source.m_nHighestDataVersion > m_nHighestDataVersion )
		m_nHighestDataVersion += source.m_nHighestDataVersion;
}

/**********************************************************************/

double CStockAuditDeliveryBuffer::CalcGPPercent()
{
	return ReportHelpers.CalcGPPercent( m_dTotalRetail - m_dTotalRetailTax, 0.0, m_dTotalCost + m_dTotalCostWaste );
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CStockAuditDeliveryDetail::CStockAuditDeliveryDetail( bool bDetail ) : CStockAuditBaseReport(), m_bufferLine( BUFFER_TYPE_LINE ), m_bufferOrder( BUFFER_TYPE_ORDER ), m_bufferAudit( BUFFER_TYPE_AUDIT ) 
{
	m_strHeaderBuffer = "";
	m_bPageBreakFlag = FALSE;
	m_bDetail = bDetail;
}

/**********************************************************************/

void CStockAuditDeliveryDetail::AddColumns( CReportFile& ReportFile )
{
	ReportFile.SetStyle1 ( GetReportTitle() );
	ReportFile.AddColumn ( "Code", TA_LEFT, 380 );
	ReportFile.AddColumn ( "Description", TA_LEFT, 300 );
	ReportFile.AddColumn ( "Category", TA_LEFT, 250 );
	ReportFile.AddColumn ( "DU Qty", TA_RIGHT, 200 );
	ReportFile.AddColumn ( "DU Cost", TA_RIGHT, 220 );
	ReportFile.AddColumn ( "DU Yield", TA_RIGHT, 250 );

	if ( TRUE == m_bDetail )
		ReportFile.AddColumn ( "Est DU Retail", TA_RIGHT, 280 );
	
	ReportFile.AddColumn ( "Total Qty", TA_RIGHT, 250 );
	ReportFile.AddColumn ( "Total Cost", TA_RIGHT, 220 );
	ReportFile.AddColumn ( EcrmanOptions.GetTaxString( "Est Cost %T" ), TA_RIGHT, 280 );

	if ( TRUE == m_bDetail )
	{
		ReportFile.AddColumn ( "Est Waste Cost", TA_RIGHT, 280 );
		ReportFile.AddColumn ( "Est Total Retail", TA_RIGHT, 280 );
		ReportFile.AddColumn ( EcrmanOptions.GetTaxString( "Est Retail %T" ), TA_RIGHT, 280 );
		ReportFile.AddColumn ( "Est GP%", TA_RIGHT, 250 );
	}

	ReportFile.AddColumn ( "Reference", TA_LEFT, 220 );
}

/**********************************************************************/

bool CStockAuditDeliveryDetail::ProcessHeader( const char* szHeader )
{
	m_bufferOrder.Reset();
	return CStockAuditBaseReport::ProcessHeader( szHeader );
}

/**********************************************************************/

bool CStockAuditDeliveryDetail::ProcessLine( const char* szLine )
{
	CCSV csv( szLine );

	if ( csv.GetInt(1) != GetAuditDataType() )
		return FALSE;

	m_bufferLine.Reset();

	int nVer = csv.GetInt(2);

	switch( nVer )
	{
	case 1:
	case 2:
	case 3:
	case 4:
		m_bufferLine.m_strStockCode = csv.GetString(3);
		m_bufferLine.m_strDescription = csv.GetString(4);
		m_bufferLine.m_strCategory = csv.GetString(5);
		m_bufferLine.m_dCaseQty = csv.GetDouble(6);
		m_bufferLine.m_dStockUnitQty = csv.GetDouble(7);
		m_bufferLine.m_dCaseCost = csv.GetDouble(8);
		m_bufferLine.m_dTotalCost = csv.GetDouble(9);
		m_bufferLine.m_nHighestDataVersion = nVer;
		break;

	default:
		m_bufferLine.m_strStockCode = "Unknown format";
		m_bufferLine.m_strDescription.Format( "(Type %d)", nVer );
		m_bufferLine.m_bUnknownFormat = TRUE;
		break;
	}

	switch( nVer )
	{
	case 2:
	case 3:
	case 4:
		m_bufferLine.m_dCaseRetail = csv.GetDouble(10);
		m_bufferLine.m_dTotalRetail = csv.GetDouble(11);
		m_bufferLine.m_dTotalRetailTax = csv.GetDouble(12);
		m_bufferLine.m_dTotalCostWaste = csv.GetDouble(13);
		m_bufferLine.m_dGPPercent = csv.GetDouble(14);
		m_bufferLine.m_strSuppRef = csv.GetString(15);
		break;

	case 1:
		m_bufferLine.m_strSuppRef = csv.GetString(10);
		break;
	}

	switch( nVer )
	{
	case 3:
	case 4:
		m_bufferLine.m_nDUItems = csv.GetInt(16);
		m_bufferLine.m_dDUItemSize = csv.GetDouble(17);
		break;
	}
		
	switch( nVer )
	{
	case 4:
		m_bufferLine.m_dTotalCostTax = csv.GetDouble(18);
		break;
	}

	if ( m_strStockCodeFrom != "" )
		if ( m_bufferLine.m_strStockCode < m_strStockCodeFrom )
			return FALSE;
		
	if ( m_strStockCodeTo != "" )
		if ( m_bufferLine.m_strStockCode > m_strStockCodeTo )
			return FALSE;

	if ( m_strLineReferenceFilter != "" )
	{
		CString strTempRef1 = m_strLineReferenceFilter;
		CString strTempRef2 = m_bufferLine.m_strSuppRef;

		strTempRef1.MakeUpper();
		strTempRef2.MakeUpper();

		if ( strTempRef2.Find( strTempRef1 ) == -1 )
			return FALSE;
	}

	m_bufferOrder.Consolidate( m_bufferLine );
	m_bufferAudit.Consolidate( m_bufferLine );

	return TRUE;
}

/**********************************************************************/

void CStockAuditDeliveryDetail::WriteHeader( CReportFile& ReportFile )
{
	CString strReference = "";
	::FormatAuditReference( m_strHeaderReference1, m_strHeaderReference2, strReference );

	CString strLine;

	strLine.Format ( "<..>%s %s ( %s, reference %s )",  
		( const char* ) m_strHeaderDate,
		( const char* ) m_strHeaderTime,
		( const char* ) m_strHeaderSuppName,
		( const char* ) strReference );

	m_strHeaderBuffer = strLine;
}

/**********************************************************************/

void CStockAuditDeliveryDetail::WriteLine( CReportFile& ReportFile, int nDummySpIdx )
{
	CStockReportLine csvReportLine;
	csvReportLine.AppendString ( GetDisplayStockCode ( m_bufferLine.m_strStockCode ) );
	csvReportLine.AppendString ( m_bufferLine.m_strDescription );

	if ( FALSE == m_bufferLine.m_bUnknownFormat )
	{
		csvReportLine.AppendString ( m_bufferLine.m_strCategory );

		if ( m_bufferLine.m_dCaseQty != 0.0 )
		{
			csvReportLine.AppendStockQty ( m_bufferLine.m_dCaseQty, 1, TRUE );
			csvReportLine.AppendVal ( m_bufferLine.m_dCaseCost );

			CString strYield = "";
			if ( ( 0 != m_bufferLine.m_nDUItems ) && ( CPriceHelpers::CompareDoubles( m_bufferLine.m_dDUItemSize, 0.0, 3 ) != 0 ) )
				strYield.Format( "%d x %3.3f", m_bufferLine.m_nDUItems, m_bufferLine.m_dDUItemSize );
			csvReportLine.AppendString( strYield );

			if ( TRUE == m_bDetail )
			{
				if ( m_bufferLine.m_nHighestDataVersion >= 2 )
					csvReportLine.AppendVal ( m_bufferLine.m_dCaseRetail );
				else
					csvReportLine.AppendBlanks( 1 );
			}
		}
		else
		{
			csvReportLine.AppendBlanks( m_bDetail ? 4 : 3 );
		}

		csvReportLine.AppendStockQty ( m_bufferLine.m_dStockUnitQty, 1, TRUE );
		csvReportLine.AppendVal ( m_bufferLine.m_dTotalCost );

		if ( m_bufferLine.m_nHighestDataVersion >= 4 )
			csvReportLine.AppendVal ( m_bufferLine.m_dTotalCostTax );
		else
			csvReportLine.AppendString( "" );

		if ( TRUE == m_bDetail )
		{
			if ( m_bufferLine.m_nHighestDataVersion >= 2 )
			{
				csvReportLine.AppendVal( m_bufferLine.m_dTotalCostWaste );
				csvReportLine.AppendVal( m_bufferLine.m_dTotalRetail );
				csvReportLine.AppendVal( m_bufferLine.m_dTotalRetailTax );
				csvReportLine.AppendVal( m_bufferLine.CalcGPPercent() );
			}
			else
				csvReportLine.AppendBlanks( 4 );
		}
	}
	
	csvReportLine.AppendString ( m_bufferLine.m_strSuppRef );

	if ( m_strHeaderBuffer != "" )
	{
		ReportFile.WriteLine( m_strHeaderBuffer );

		if ( DealerFlags.GetDeliveryLinesFlag() == TRUE )
			ReportFile.WriteLine ( "<LI>" );

		m_strHeaderBuffer = "";
	}

	ReportFile.WriteLine ( csvReportLine.GetLine() );

	if ( DealerFlags.GetDeliveryLinesFlag() == TRUE )
		ReportFile.WriteLine ( "<LI>" );
};

/**********************************************************************/

void CStockAuditDeliveryDetail::AppendTotals ( CReportFile& ReportFile )
{
	WriteBuffer( ReportFile, m_bufferOrder );
};

/**********************************************************************/

void CStockAuditDeliveryDetail::AppendGrandTotals ( CReportFile& ReportFile )
{
	WriteBuffer( ReportFile, m_bufferAudit );
};

/**********************************************************************/

void CStockAuditDeliveryDetail::WriteBuffer( CReportFile& ReportFile, CStockAuditDeliveryBuffer& buffer )
{
	bool bWriteData = FALSE;
	switch( buffer.m_nBufferType )
	{
	case BUFFER_TYPE_ORDER:
		bWriteData = ( ( DealerFlags.GetDeliveryLinesFlag() == FALSE ) && ( m_strHeaderBuffer == "" ) );
		break;

	case BUFFER_TYPE_AUDIT:
		bWriteData = ( DealerFlags.GetDeliveryLinesFlag() == FALSE );
		break;
	}

	if ( TRUE == bWriteData )
	{
		CStockReportLine csvTotals;

		switch( buffer.m_nBufferType )
		{
		case BUFFER_TYPE_ORDER:
			csvTotals.AppendBlanks( 1 );
			csvTotals.AppendString ( "<..>Totals" );
			csvTotals.AppendBlanks( 1 );
			break;

		case BUFFER_TYPE_AUDIT:
			csvTotals.AppendBlanks( 1 );
			csvTotals.AppendString ( "<..>Grand Total" );
			csvTotals.AppendBlanks( 1 );
			break;
		}

		if ( buffer.m_dCaseQty != 0.0 )
			csvTotals.AppendStockQty ( buffer.m_dCaseQty, 1, TRUE );
		else
			csvTotals.AppendBlanks( 1 );
		
		csvTotals.AppendBlanks( m_bDetail ? 3 : 2 );

		csvTotals.AppendStockQty ( buffer.m_dStockUnitQty, 1, TRUE );
		csvTotals.AppendVal ( buffer.m_dTotalCost );
		
		if ( buffer.m_nHighestDataVersion >= 4 )
			csvTotals.AppendVal ( buffer.m_dTotalCostTax );
		else
			csvTotals.AppendBlanks( 1 );
			
		if ( TRUE == m_bDetail )
		{
			if ( buffer.m_nHighestDataVersion >= 2 )
			{
				csvTotals.AppendVal( buffer.m_dTotalCostWaste );
				csvTotals.AppendVal( buffer.m_dTotalRetail );
				csvTotals.AppendVal( buffer.m_dTotalRetailTax );
				csvTotals.AppendVal( buffer.CalcGPPercent() );
			}
		}

		CUnderlineMaker UnderlineMaker;
		UnderlineMaker.AddBlanks(3);
		UnderlineMaker.AddColumns( ( buffer.m_dCaseQty != 0.0 ), 1 );
		UnderlineMaker.AddBlanks( m_bDetail ? 3 : 2 );
		UnderlineMaker.AddTotals(2);
		UnderlineMaker.AddColumns( ( buffer.m_nHighestDataVersion >= 4 ), 1 );
		
		if ( TRUE == m_bDetail )
			UnderlineMaker.AddColumns( ( buffer.m_nHighestDataVersion >= 2 ), 4 );

		ReportFile.WriteLine( UnderlineMaker.GetLine() );
		ReportFile.WriteLine( csvTotals.GetLine() );
	}

	switch( buffer.m_nBufferType )
	{
	case BUFFER_TYPE_ORDER:
		if ( m_strHeaderBuffer == "" )
		{
			ReportFile.WriteLine ( "" );

			if ( m_bPageBreakFlag == TRUE )
				ReportFile.WriteLine ( "<NEWPAGE>" );
		}
		m_strHeaderBuffer = "";
		break;

	case BUFFER_TYPE_AUDIT:
		ReportFile.WriteLine ( "" );
		break;
	}
}

/**********************************************************************/

