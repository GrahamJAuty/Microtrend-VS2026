/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "NodeTypes.h"
#include "RepCSV.h"
#include "StockReportNames.h"
/**********************************************************************/
#include "StockTreeReportNewSupplier.h"
/**********************************************************************/

CStockTreeReportNewSupplier::CStockTreeReportNewSupplier( CStockLevelTreeNew& StockLevelTree ) : CStockTreeReportNew( StockLevelTree )
{
	m_bSupplierName = FALSE;
	m_bSupplierRef = FALSE;
	m_bStockUnit = FALSE;
	m_nReportParamsType = STOCK_SUPPLIER;
	m_nTotalisersPerLine = 0;
	m_bGotStockpointField = FALSE;
}

/**********************************************************************/

void CStockTreeReportNewSupplier::PrepareReport()
{
	m_strReportName = GetReportTitle();

	m_arrayColumnNames.RemoveAll();
	m_arrayTotalPos.RemoveAll();

	AddStockCodeColumns( m_bStockUnit );
	AddReportColumns();
	AddSupplierColumns( m_bSupplierName, m_bSupplierRef );

	m_bGotStockpointField = FALSE;
	for ( int n = 0; n < m_arrayReportFields.GetSize(); n++ )
	{
		int nFieldType = ( m_arrayReportFields.GetAt(n) % 100 );

		if ( SH_FIELD_SP == nFieldType )
			m_bGotStockpointField = TRUE;
	}
}

/**********************************************************************/

void CStockTreeReportNewSupplier::AddReportColumns()
{
	AddColumn ( "Apparent", TA_RIGHT, 200, FALSE );
	AddColumn ( "Below", TA_RIGHT, 180, FALSE );
	AddColumn ( "SU Req", TA_RIGHT, 180, FALSE );
	AddColumn ( "DU Cost", TA_RIGHT, 180, FALSE );
}

/**********************************************************************/

void CStockTreeReportNewSupplier::CreateReportLine( CStockLevelsCSVRecord& StockLevels )
{
	CStringArray arrayReorderInfo;
	int nDbIdx = DataManager.GetActiveDbIdx();

	if ( TRUE == m_bGotStockpointField )
	{
		CLocationSelectorMiniLookup LocSelMiniLookup (
			NODE_STOCKPOINT,
			dbStockpoint.GetSpNo( m_nLabelSpIdx ) );
		
		DataManager.Stock.GetReorderInfo ( nDbIdx, m_nLabelStockIdx, LocSelMiniLookup, arrayReorderInfo, TRUE );
	}
	else
	{
		CLocationSelectorMiniLookup LocSelMiniLookup (
			m_StockLevelTree.GetConLevel(),
			m_StockLevelTree.GetTableNo() );

		DataManager.Stock.GetReorderInfo ( nDbIdx, m_nLabelStockIdx, LocSelMiniLookup, arrayReorderInfo, TRUE );
	}

	CWordArray arrayStockSuppIdx;

	if ( m_nLabelSuppIdx == 0 )
	{
		arrayStockSuppIdx.Add( 0 );
	}
	else if ( m_nLabelSuppIdx >= 1 )
	{
		int nSuppNo = DataManager.Supplier.GetSuppNo( m_nLabelSuppIdx - 1 );

		for ( int n = 0; n < m_LabelStockRecord.GetSupplierCount(); n++ )
			if ( m_LabelStockRecord.GetSuppNo( n ) == nSuppNo )
				arrayStockSuppIdx.Add( n + 1 );
	}

	for ( int n = 0; n < arrayStockSuppIdx.GetSize(); n++ )
	{
		int nStockSuppIdx = arrayStockSuppIdx.GetAt(n) - 1;

		CStockReportLine csvOut;
		OutputStockCodeFields( csvOut, m_bStockUnit );

		int nBlanks = 3;
		int nInfoSize = arrayReorderInfo.GetSize();
		if ( nInfoSize >= 1 )
		{
			CCSVInternalReorderInfo csvIn ( arrayReorderInfo.GetAt ( nInfoSize - 1 ) );
			csvOut.AppendStockQty ( csvIn.GetApparentQty() );
			nBlanks = 2;

			if ( m_LabelStockRecord.GetDisableAutoOrderFlag() == FALSE )
			{
				if ( csvIn.GetBelowQty() > 0.0 )
				{
					csvOut.AppendStockQty ( csvIn.GetBelowQty() );
					csvOut.AppendStockQty ( csvIn.GetSUShortQty() );
					nBlanks = 0;
				}
			}
		}

		csvOut.AppendBlanks( nBlanks );	

		if ( nStockSuppIdx >= 0 )
		{
			csvOut.AppendVal ( m_LabelStockRecord.GetCost ( nStockSuppIdx  ) );
			OutputSupplierFields( csvOut, m_bSupplierName, m_bSupplierRef, nStockSuppIdx );
		}
		
		WriteReportLine( csvOut.GetLine() );

		if ( m_bDrawLines == TRUE )
			WriteReportLine ( "<LI>" );
	}
}

/**********************************************************************/

