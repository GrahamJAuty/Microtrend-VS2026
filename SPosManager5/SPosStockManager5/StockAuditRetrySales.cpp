/**********************************************************************/
#include "CCSVReportLine.h"
#include "ExplodePlu.h"
#include "RepCSV.h"
#include "ReportHelpers.h"
#include "SalesHistoryFields.h"
#include "StockOptionsIni.h"
#include "StockpointCSVArray.h"
/**********************************************************************/
#include "StockAuditRetrySales.h"
/**********************************************************************/

CStockAuditRetrySales::CStockAuditRetrySales() : CStockAuditBaseReport() 
{
	m_bSpecialOK = FALSE;
	m_nLastSpIdx = -1;
}

/**********************************************************************/

void CStockAuditRetrySales::AddColumns( CReportFile& ReportFile )
{
	ReportFile.SetStyle1 ( GetReportTitle() );
	ReportFile.AddColumn ( "Report Date", TA_LEFT, 250 );
	ReportFile.AddColumn ( "Type", TA_LEFT, 300 );
	ReportFile.AddColumn ( "Plu No", TA_LEFT, 350 );
	ReportFile.AddColumn ( "Location", TA_LEFT, 450 );
	ReportFile.AddColumn ( "Quantity", TA_RIGHT, 200 );
	ReportFile.AddColumn ( "Value", TA_RIGHT, 200 );
	ReportFile.AddColumn ( "Status Now", TA_LEFT, 380 );
}

/**********************************************************************/

const char* CStockAuditRetrySales::GetHeaderLine( COleDateTime& date, const char* szDateOfSale, const char* szTimeOfSale, const char* szEcrName )
{
	CString strDate;
	strDate.Format ( "%2.2d/%2.2d/%4.4d", date.GetDay(), date.GetMonth(), date.GetYear() );

	CCSV csv;
	csv.Add ( "H" );				//HEADER LINE
	csv.Add ( 1 );					//VERSION NO
	csv.Add ( strDate );
	csv.Add ( szDateOfSale );
	csv.Add ( szTimeOfSale );
	csv.Add ( szEcrName );

	m_strHeaderLine = csv.GetLine();
	return m_strHeaderLine;
}

/**********************************************************************/

bool CStockAuditRetrySales::ProcessHeader( const char* szHeader )
{
	CCSV csv ( szHeader );
	
	switch ( csv.GetInt(1) )
	{
	case 1:
		m_strHeaderDate = csv.GetString(2);
		m_strHeaderDateOfSale = csv.GetString(3);
		m_strHeaderTimeOfSale = csv.GetString(4);
		m_strHeaderEcrName = csv.GetString(5);
		break;

	default:
		m_strHeaderDate = "";
		m_strHeaderDateOfSale = "";
		m_strHeaderTimeOfSale = "";
		m_strHeaderEcrName = "";
		break;
	}

	return TRUE;
}

/**********************************************************************/

bool CStockAuditRetrySales::ProcessLine( const char* szLine )
{
	CCSV csv( szLine );

	switch( csv.GetInt(1) )
	{
	case 1:
		m_nLineActionType = csv.GetInt(2);
		m_strLinePluNo = csv.GetString(3);
		m_nLinePluLevel = csv.GetInt(4);
		m_dLinePluQty = csv.GetDouble(5);
		m_dLinePluValue = csv.GetDouble(6);
		break;

	default:
		return FALSE;
	}

	switch( m_nLineActionType )
	{
	case STOCK_XXX_SALES:
		m_strLineActionType = "Sale";
		break;

	case STOCK_XXX_WASTAGE:
		m_strLineActionType = "Wastage";
		break;

	case STOCK_XXX_DELIVERY:
		m_strLineActionType = "Delivery";
		break;

	default:
		m_strLineActionType = "Unknown";
		break;
	}

	return TRUE;
}

/**********************************************************************/

void CStockAuditRetrySales::WriteLine( CReportFile& ReportFile, int nSpIdx )
{
	if ( nSpIdx != m_nLastSpIdx )
	{
		CDataManagerInfo info;
		DataManager.OpenStockLevels( nSpIdx, DATAMANAGER_OPEN_READONLY, info );
		m_nLastSpIdx = nSpIdx;
	}

	CStockReportLine csvOut;
		
	csvOut.AppendString ( m_strHeaderDate );
	csvOut.AppendString ( m_strLineActionType );

	CString strPluNo = ReportHelpers.GetDisplayPluNo ( _atoi64(m_strLinePluNo) );
	
	CSQLPluNoInfo infoPluNo;
	infoPluNo.m_nEposPluNo = _atoi64( strPluNo );
	::ProcessPluNo( infoPluNo );

	CExplodePluStockLevel ExplodePlu( dbStockpoint.GetSpNo( nSpIdx ) );
	
	CStringArray StockDetails;
	int nExplodeResult = ExplodePlu.ExplodePlu( infoPluNo.m_nBasePluNo, StockDetails, ( STOCK_XXX_DELIVERY == m_nLineActionType ) ? AUDIT_DELIVERY : AUDIT_SALES, 0, 0 );

	CString strProblem = "Unknown";
	if ( StockDetails.GetSize() >= 1 )
	{
		CCSVExplodedPlu csv( StockDetails.GetAt(0) );
		strProblem = csv.GetProblem();
		
		switch( nExplodeResult )
		{
		case PLU_EXCEPTION_NONE:
			strProblem = "OK";
			if ( STOCK_XXX_DELIVERY == m_nLineActionType )
			{
				CStockCSVRecord StockRecord;
				DataManager.Stock.GetAt( csv.GetStockIdx(), StockRecord );
				m_dLinePluValue = StockRecord.GetUnitCost() * m_dLinePluQty;
			}
			break;

		case PLU_EXCEPTION_PLUNONSTOCK:
			strProblem = "OK **";
			m_bSpecialOK = TRUE;
			break;

		case PLU_EXCEPTION_PLUNOTLINKED:
			if ( StockOptions.GetImportSalesExceptionFlag() == FALSE )
			{
				strProblem = "OK **";
				m_bSpecialOK = TRUE;
			}
			break;
		}
	}

	csvOut.AppendString ( strPluNo );					
	csvOut.AppendString ( m_strHeaderEcrName );
	csvOut.AppendStockQty ( m_dLinePluQty, 1, TRUE );
	csvOut.AppendVal ( m_dLinePluValue );
	
	csvOut.Add( strProblem );
	ReportFile.WriteLine ( csvOut.GetLine() );
}

/**********************************************************************/

void CStockAuditRetrySales::AfterReport( CReportFile& ReportFile )
{
	if ( TRUE == m_bSpecialOK )
	{
		ReportFile.WriteLine( "<..>( OK ** indicates previous exceptions that are now flagged as non-stock items )" );
		ReportFile.WriteLine( "" );
		ReportFile.WriteLine( "" );
	}
}

/**********************************************************************/
