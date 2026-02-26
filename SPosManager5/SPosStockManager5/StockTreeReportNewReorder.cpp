/**********************************************************************/
 
/**********************************************************************/
#include "Consolidation.h"
#include "DatabaseCSVArray.h"
#include "NodeTypes.h"
#include "RepCSV.h"
#include "ReportConsolidationArray.h"
#include "StockOptionsIni.h"
#include "StockReportNames.h"
#include "SysInfo.h"
/**********************************************************************/
#include "StockTreeReportNewReorder.h"
/**********************************************************************/

CStockTreeReportNewReorder::CStockTreeReportNewReorder( CStockLevelTreeNew& StockLevelTree ) : CStockTreeReportNew( StockLevelTree )
{
	m_strDateFrom = "";
	m_strDateTo = "";
	m_bSeparateStockpoints = FALSE;
	m_bSupplierName = FALSE;
	m_bSupplierRef = FALSE;
	m_bStockUnit = FALSE;
	m_nReportParamsType = STOCK_REORDER;
	m_nTotalisersPerLine = 1;
}

/**********************************************************************/

void CStockTreeReportNewReorder::PrepareReport()
{
	if ( m_StockLevelTree.GetConLevel() == NODE_STOCKPOINT )
		m_bSeparateStockpoints = FALSE;

	m_strReportName = GetReportTitle();

	if ( StockOptions.GetOrderGenerationMethod() == ORDER_TYPE_SALES )
	{
		CString strDate;
		strDate.Format ( " (%s - %s)",
			(const char*) m_strDateFrom,
			(const char*) m_strDateTo );

		m_strReportName += strDate;
	}

	m_arrayColumnNames.RemoveAll();
	m_arrayTotalPos.RemoveAll();

	AddStockCodeColumns( m_bStockUnit );
	AddReportColumns();
	AddSupplierColumns( m_bSupplierName, m_bSupplierRef );

	if ( TRUE == m_bSeparateStockpoints )
		m_strItemTotalsUnderline = "\t\t\t\t\t";
	else
		m_strItemTotalsUnderline = "\t\t\t\t";

	if ( StockOptions.GetOrderSeparationType() == ORDER_DUSEPARATE_NONE )
	{
		if ( StockOptions.GetOrderGenerationMethod() != ORDER_TYPE_SALES )
			m_strItemTotalsUnderline += "<LI>\t<LI>\t<LI>\t<LI>\t<LI>";
		else
			m_strItemTotalsUnderline += "<LI>";
	}
	else
	{
		if ( StockOptions.GetOrderGenerationMethod() != ORDER_TYPE_SALES )
			m_strItemTotalsUnderline += "<LI>\t<LI>\t<LI>\t<LI>\t<LI>\t<LI>";
		else
			m_strItemTotalsUnderline += "<LI>\t<LI>";
	}
}

/**********************************************************************/

void CStockTreeReportNewReorder::AddReportColumns()
{	
	if ( m_bSeparateStockpoints == TRUE )
		AddColumn ( "Stockpoint", TA_LEFT, 250, FALSE );

	if ( StockOptions.GetOrderGenerationMethod() != ORDER_TYPE_SALES )
	{
		AddColumn ( "Apparent", TA_RIGHT, 200, FALSE );
		AddColumn ( "Below", TA_RIGHT, 180, FALSE );
		AddColumn ( "SU Req.", TA_RIGHT, 180, FALSE );
		AddColumn ( "On Order", TA_RIGHT, 200, FALSE );
		AddColumn ( "SU Short", TA_RIGHT, 180, FALSE );
	}
	else
		AddColumn ( "Sales", TA_RIGHT, 200, FALSE );
			
	AddColumn ( "DU Req.", TA_RIGHT, 180, FALSE );
	AddColumn ( "DU Cost", TA_RIGHT, 180, FALSE );
	AddColumn ( "Total Cost", TA_RIGHT, 220, TRUE );
	
	m_nGrandTotalColumn = m_arrayColumnNames.GetSize() - 1;
}

/**********************************************************************/

void CStockTreeReportNewReorder::CreateReportLine( CStockLevelsCSVRecord& StockLevels )
{
	CStringArray arrayReorderInfo;
	int nDbIdx = m_StockLevelTree.GetDbIdx();

	CLocationSelectorMiniLookup LocSelMiniLookup(
		m_StockLevelTree.GetConLevel(),
		m_StockLevelTree.GetTableNo() );
	
	DataManager.Stock.GetReorderInfo( nDbIdx, m_nLabelStockIdx, LocSelMiniLookup, arrayReorderInfo, FALSE );

	int nInfoSize = arrayReorderInfo.GetSize();
	if ( nInfoSize < 2 )
		return;

	if ( m_bSeparateStockpoints == TRUE )
	{
		CStringArray arraySpInfo;

		WriteBlankLine( TRUE );

		for ( int nIndex = 0; nIndex < nInfoSize - 1; nIndex++ )
		{
			CCSVInternalReorderInfo csvIn ( arrayReorderInfo.GetAt ( nIndex ) );
			CStockReportLine csvOut;
	
			OutputStockCodeFields( csvOut, m_bStockUnit );
			
			csvOut.AppendString ( dbStockpoint.GetName( csvIn.GetSpIdx() ) );	
			
			if ( StockOptions.GetOrderGenerationMethod() != ORDER_TYPE_SALES )
			{
				//KLUDGE - IGNORE SUB UNITS
				csvOut.AppendStockQty ( csvIn.GetApparentQty() );
				csvOut.AppendStockQty ( csvIn.GetBelowQty() );
				csvOut.AppendStockQty ( csvIn.GetSURequired() );
				csvOut.AppendStockQty ( csvIn.GetSUOnOrderQty() );
				csvOut.AppendStockQty ( csvIn.GetSUShortQty() );
			}
			else
				csvOut.AppendStockQty ( -csvIn.GetApparentQty() );
			
			if ( StockOptions.GetOrderSeparationType() != ORDER_DUSEPARATE_NONE )
				csvOut.AppendInt ( csvIn.GetDURequired() );
	
			csvOut.InsertAt( 0, dbStockpoint.GetName( csvIn.GetSpIdx() ) );
			arraySpInfo.Add( csvOut.GetLine() );
		}

		if ( EcrmanOptions.GetReportsLocationSortType() == 0 )
		{
			for ( int n = 0; n < arraySpInfo.GetSize(); n++ )
			{
				CCSV csv ( arraySpInfo.GetAt(n), '\t' );
				csv.RemoveAt(0);

				if ( n != 0 )
				{
					csv.SetAt( 0, "" );
					csv.SetAt( 1, "" );
					csv.SetAt( 2, "" );
				}

				WriteReportLine( csv.GetLine() );
			}
		}
		else
		{
			CReportConsolidationArray<CSortedStringByString> arraySpInfoSorted( TRUE, 1 );
			
			for ( int n = 0; n < arraySpInfo.GetSize(); n++ )
			{
				CCSV csv ( arraySpInfo.GetAt(n), '\t' );

				CSortedStringByString item;
				item.m_strKey = csv.GetString(0);
				item.m_strData = csv.GetLine();
				arraySpInfoSorted.Consolidate( item );
			}

			for ( int n = 0; n < arraySpInfoSorted.GetSize(); n++ )
			{
				CSortedStringByString item;
				arraySpInfoSorted.GetAt( n, item );

				CCSV csv ( item.m_strData, '\t' );
				csv.RemoveAt(0);
				
				if ( n != 0 )
				{
					csv.SetAt( 0, "" );
					csv.SetAt( 1, "" );
					csv.SetAt( 2, "" );
				}
				
				WriteReportLine( csv.GetLine() );
			}
		}

		CCSVInternalReorderInfo csvIn ( arrayReorderInfo.GetAt ( nInfoSize - 1 ) );
		CStockReportLine csvOut;

		csvOut.AppendBlanks( m_bSeparateStockpoints ? 5 : 4 );
		
		if ( StockOptions.GetOrderGenerationMethod() != ORDER_TYPE_SALES )
		{
			//KLUDGE - IGNORE SUB UNITS
			csvOut.AppendStockQty ( csvIn.GetApparentQty() );
			csvOut.AppendStockQty ( csvIn.GetBelowQty() );
			csvOut.AppendStockQty ( csvIn.GetSURequired() );
			csvOut.AppendStockQty ( csvIn.GetSUOnOrderQty() );
			csvOut.AppendStockQty ( csvIn.GetSUShortQty() );
		}
		else
			csvOut.AppendStockQty ( -csvIn.GetApparentQty() );
		
		int nDUReq = csvIn.GetDURequired();
		double dUnitCost = m_LabelStockRecord.GetCost(0);
		double dLineCost = dUnitCost * nDUReq;

		csvOut.AppendInt ( nDUReq );
		csvOut.AppendVal ( dUnitCost );
		csvOut.AppendVal ( dLineCost );

		OutputSupplierFields( csvOut, m_bSupplierName, m_bSupplierRef );
	
		WriteReportLine ( m_strItemTotalsUnderline );
		WriteReportLine ( csvOut.GetLine() );

		AddToTotalisers( 0, dLineCost );
	}
	else
	{
		CCSVInternalReorderInfo csvIn ( arrayReorderInfo.GetAt ( nInfoSize - 1 ) );
		CStockReportLine csvOut;
	
		OutputStockCodeFields( csvOut, m_bStockUnit );

		if ( StockOptions.GetOrderGenerationMethod() != ORDER_TYPE_SALES )
		{
			//KLUDGE - IGNORE SUB UNITS
			csvOut.AppendStockQty ( csvIn.GetApparentQty() );
			csvOut.AppendStockQty ( csvIn.GetBelowQty()  );
			csvOut.AppendStockQty ( csvIn.GetSURequired()  );
			csvOut.AppendStockQty ( csvIn.GetSUOnOrderQty() );
			csvOut.AppendStockQty ( csvIn.GetSUShortQty()  );
		}
		else
			csvOut.AppendStockQty ( -csvIn.GetApparentQty() );
		
		int nDUReq = csvIn.GetDURequired();
		double dUnitCost = m_LabelStockRecord.GetCost(0);
		double dLineCost = dUnitCost * nDUReq;

		csvOut.AppendInt ( nDUReq );
		csvOut.AppendVal ( dUnitCost );
		csvOut.AppendVal ( dLineCost );

		OutputSupplierFields( csvOut, m_bSupplierName, m_bSupplierRef );
		WriteReportLine ( csvOut.GetLine() );

		AddToTotalisers( 0, dLineCost );
	}
}

/**********************************************************************/

void CStockTreeReportNewReorder::CreateTotalsLine( CStockReportLine& csv, int nDepth )
{
	double dTotalCost = GetTotaliser( nDepth, 0 );
	csv.SetAt( m_nGrandTotalColumn, dTotalCost, SysInfo.GetDPValue() );
}

/**********************************************************************/



