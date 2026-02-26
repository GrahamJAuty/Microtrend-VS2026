/**********************************************************************/
 
/**********************************************************************/
#include "DatabaseCSVArray.h"
//#include "globalfunctions.h"
#include "MessageLogger.h"
#include "RepCSV.h"
#include "SalesTypes.h"
#include "StatusProgress.h"
/**********************************************************************/
#include "PluSalesSorterNonEcr.h"
/**********************************************************************/

CPluSalesSorterNonEcr::CPluSalesSorterNonEcr ( int nDbIdx, CStringArray& AuditedSales, const char* szLogTime )
{
	m_strLogTime = szLogTime;

	CCSV csvDate ( m_strLogTime );
	m_strReportTime = csvDate.GetString(1);
	m_strReportDate = csvDate.GetString(2);

	if ( m_strReportDate.GetLength() != 8 )
		return;

	if ( ::TestNumeric( m_strReportDate ) == FALSE )
		return;

	StatusProgress.Lock( "Updating sales history" );
	UpdateSales ( nDbIdx, AuditedSales );
	StatusProgress.Unlock();
}

/**********************************************************************/

void CPluSalesSorterNonEcr::UpdateSales ( int nDbIdx, CStringArray& AuditedSales )
{		
	CString strDestPath = dbDatabase.GetFolderPathDataHistory( nDbIdx );
	::CreateSubdirectory( strDestPath );

	strDestPath += "\\";
	strDestPath += m_strReportDate;

	CString strDestPathManual = strDestPath + ".002";
	CString strDestPathImport = strDestPath + ".003";

	CSSFile fileHistoryManual;
	if ( fileHistoryManual.Open ( strDestPathManual, "ab" ) == FALSE )
		return;
	
	CSSFile fileHistoryImport;
	if ( fileHistoryImport.Open ( strDestPathImport, "ab" ) == FALSE )
		return;
	
	int nLineCountManual = 0;
	int nLineCountImport = 0;
	
	int nSize = AuditedSales.GetSize();
	for ( int nIndex = 0; nIndex < nSize; nIndex++ )
	{
		CCSVManualSales csvSale ( AuditedSales.GetAt( nIndex ) );
		
		CString strPluNo;
		strPluNo.Format ( "%I64d%1.1d",
			csvSale.GetPluNo(),
			csvSale.GetModifier() );
		
		//WE MAY GET SOME LINES WITH ZERO QTY AND VALUE IF PLU
		//ZERO SKIPPING IS SWITCHED OFF IN THE TERMINAL SETTINGS
		double dQty = csvSale.GetQty();
		double dValue = csvSale.GetValue();
		if ( dQty == 0.0 && dValue == 0.0 )
			continue;

		CCSV csvOut( ',', '"', FALSE, TRUE, TRUE );
		csvOut.Add( m_strReportTime );
		csvOut.Add( 0 );				//SERVER NO
		csvOut.Add( strPluNo );
		csvOut.Add( csvSale.GetPriceLevel() );	
		csvOut.Add( dQty, 5 );
		csvOut.Add( dValue, 5 );
		csvOut.Add( csvSale.GetDeptNo() );

		if ( csvSale.GetSalesType() == SALES_TYPE_MANUAL )
		{
			fileHistoryManual.WriteLine( csvOut.GetLine() );
			nLineCountManual++;
		}
		else if ( csvSale.GetSalesType() == SALES_TYPE_IMPORT )
		{
			fileHistoryImport.WriteLine( csvOut.GetLine() );
			nLineCountImport++;
		}

		StatusProgress.SetPos ( nIndex, nSize );	
	}

	if ( nLineCountManual > 0 )
	{
		CString strMsg;
		strMsg.Format ( "     Sales History %d lines (manual)", nLineCountManual );
		
		CMessageLoggerInfo info;
		info.m_bAddDate = TRUE;
		info.m_bAddTimeOfDay = TRUE;
		info.m_bAddNetworkName = FALSE;
		info.m_strMessage = strMsg;
		info.m_nHideLevel = 1;
		MessageLogger.LogSchedulerMessage( info, FALSE, TRUE, FALSE );
	}

	if ( nLineCountImport > 0 )
	{
		CString strMsg;
		strMsg.Format ( "     Sales History %d lines (import)", nLineCountImport );
		
		CMessageLoggerInfo info;
		info.m_bAddDate = TRUE;
		info.m_bAddTimeOfDay = TRUE;
		info.m_bAddNetworkName = FALSE;
		info.m_strMessage = strMsg;
		info.m_nHideLevel = 1;
		MessageLogger.LogSchedulerMessage( info, FALSE, TRUE, FALSE );
	}
}

/**********************************************************************/

