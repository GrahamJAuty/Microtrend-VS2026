/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "ExplodePlu.h"
#include "RepCSV.h"
#include "ReportHelpers.h"
#include "SalesHistoryFields.h"
#include "SiteCSVArray.h"
#include "StockAuditBaseReport.h"
#include "StatusProgress.h"
#include "StockOptionsIni.h"
#include "StockReportNames.h"
#include "SysInfo.h"
/**********************************************************************/
#include "StockTreeReportNew.h"
/**********************************************************************/

CStockTreeReportNew::CStockTreeReportNew( CStockLevelTreeNew& StockLevelTree ) : m_StockLevelTree( StockLevelTree )
{
	m_strConLevelText = "";
	m_nTotalisersPerLine = 0;
	m_nReportParamsType = 0;
	m_bDrawLines = FALSE;
	m_nItemIDColumns = 0;
	m_bForceBlankColumn = FALSE;
	m_bGotCategoryField = FALSE;
	m_bBlankBeforeHeader = TRUE;
	m_bSubUnits = StockOptions.GetStockSubUnitsFlag();
	m_bWantTransferDetail = FALSE;
	m_bWantSalesCost = FALSE;
}

/**********************************************************************/

CStockTreeReportNew::~CStockTreeReportNew()
{
}

/**********************************************************************/

void CStockTreeReportNew::SetChartInfo( CSalesHistoryCSVRecordStock& chartinfo )
{
	m_chartinfo.SetChartType( chartinfo.GetChartType() );
	m_chartinfo.SetBottom100Flag( chartinfo.IsBottom100() );
	m_chartinfo.SetSortedByValueFlag( chartinfo.IsSortedByValue() );
	m_chartinfo.SetChartSize( chartinfo.GetChartSize() );
	m_chartinfo.SetChartCutOff( chartinfo.GetChartCutOff() );
}

/**********************************************************************/

void CStockTreeReportNew::PrepareArrays()
{
	m_bShowSubUnits = FALSE;

	m_StockLevelTree.GetReportFields( m_arrayReportFields );

	m_arrayHeaderFlags.RemoveAll();
	m_arrayTotalFlags.RemoveAll();

	m_arrayHeaderFlags.Add(0);
	m_arrayTotalFlags.Add(1);
	
	int nFieldCount = m_arrayReportFields.GetSize();
	for ( int nPos = 0; nPos < nFieldCount; nPos++ )
	{
		int nField = m_arrayReportFields.GetAt(nPos);
		int nFieldType = nField % 100;
		int nTotalType = nField - nFieldType;
		bool bHeader = ( nTotalType == SH_FIELD_HEADER || nTotalType == SH_FIELD_HEADER + SH_FIELD_TOTAL );
		bool bTotal = ( nTotalType == SH_FIELD_TOTAL || nTotalType == SH_FIELD_HEADER + SH_FIELD_TOTAL );
	
		m_arrayReportFields.SetAt( nPos, nFieldType );
		m_arrayHeaderFlags.Add ( bHeader ? 1 : 0 );
		m_arrayTotalFlags.Add ( bTotal ? 1 : 0 );

		if ( ( nFieldType == SH_FIELD_CODE ) && ( StockOptions.GetStockSubUnitsFlag() == TRUE ) )
			m_bShowSubUnits = TRUE;
	}

	m_arrayTotalisers.RemoveAll();
	int nTotalisers = ( m_nTotalisersPerLine ) * m_arrayHeaderFlags.GetSize();
	for ( int n = 0; n < nTotalisers; n++ )
		m_arrayTotalisers.Add( 0.0 );
}

/**********************************************************************/

void CStockTreeReportNew::AddColumn( const char* szText, int nFormat, int nWidth, bool bUnderline )
{
	CStockTreeColumnNew column;
	column.m_strText = szText;
	column.m_nFormat = nFormat;
	column.m_nWidth = nWidth;
	m_arrayColumnNames.Add( column );
	
	if ( bUnderline == TRUE )
		m_arrayTotalPos.Add( m_arrayColumnNames.GetSize() - 1 );
}

/**********************************************************************/

void CStockTreeReportNew::AddStockCodeColumns( bool bStockUnit )
{
	/*
	if ( m_StockLevelTree.GetSummaryFlag() == FALSE )
	{
		AddColumn( "Code", TA_LEFT, 380 );
		AddColumn( "Description", TA_LEFT, 300 );
		AddColumn( "Category", TA_LEFT, 300 );
		
		if ( TRUE == bStockUnit )
		{
			AddColumn( "Stock Unit", TA_LEFT, 300 );
			m_nItemIDColumns = 4;
		}
		else
			m_nItemIDColumns = 3;
	}
	else
	{
		AddColumn( "", TA_LEFT, 500 );
		m_nItemIDColumns = 1;
	}
	*/

	m_nItemIDColumns = 0;

	m_bGotCategoryField = FALSE;
	m_bForceBlankColumn = FALSE;
	
	int nFieldCount = m_arrayReportFields.GetSize();
	for ( int nPos = 0; nPos < nFieldCount; nPos++ )
	{
		int nField = m_arrayReportFields.GetAt(nPos);
		int nFieldType = nField % 100;

		if ( SH_FIELD_CAT_ITEM == nFieldType )
		{
			m_bGotCategoryField = TRUE;
			break;
		}
	}

	for ( int nPos = 0; nPos < nFieldCount; nPos++ )
	{
		int nField = m_arrayReportFields.GetAt(nPos);
		int nFieldType = nField % 100;

		switch( nFieldType )
		{
		case SH_FIELD_CODE:
			if ( m_StockLevelTree.GetSummaryFlag() == FALSE )
			{
				AddColumn( "Code", TA_LEFT, 380 );
				AddColumn( "Description", TA_LEFT, 300 );
				m_nItemIDColumns += 2;
				
				if ( FALSE == m_bGotCategoryField )
				{
					AddColumn( "Category", TA_LEFT, 300 );
					m_nItemIDColumns += 1;
				}
			}
			break;

		case SH_FIELD_CAT_ITEM:
			AddColumn( "Category", TA_LEFT, 300 );
			m_nItemIDColumns += 1;
			break;

		case SH_FIELD_SUPP:
			AddColumn( "Supplier", TA_LEFT, 350 );
			m_nItemIDColumns += 1;
			break;

		case SH_FIELD_SP:
			AddColumn( "Stockpoint", TA_LEFT, 300 );
			m_nItemIDColumns += 1;	
			break;

		case SH_FIELD_CUSTOM1:
		case SH_FIELD_CUSTOM2:
		case SH_FIELD_CUSTOM3:
		case SH_FIELD_CUSTOM4:
		case SH_FIELD_CUSTOM5:
			{
				int nFieldNo = nFieldType + 1 - SH_FIELD_CUSTOM1;
				AddColumn( DataManager.CustomFieldManager.GetFieldNameByFieldNo( nFieldNo ), TA_LEFT, 300 );
				m_nItemIDColumns += 1;
			}
			break;
		}
	}

	if ( 0 == m_nItemIDColumns )
	{
		AddColumn( "", TA_LEFT, 300 );
		m_bForceBlankColumn = TRUE;
		m_nItemIDColumns++;
	}

	if ( ( TRUE == bStockUnit ) && ( m_StockLevelTree.GetSummaryFlag() == FALSE ) )
	{
		AddColumn( "Stock Unit", TA_LEFT, 300 );
		m_nItemIDColumns++;
	}
}

/**********************************************************************/

void CStockTreeReportNew::OutputStockCodeFields( CCSV& csv, bool bStockUnit )
{
	int nColumns = 0;

	int nFieldCount = m_arrayReportFields.GetSize();
	for ( int nPos = 0; nPos < nFieldCount; nPos++ )
	{
		int nField = m_arrayReportFields.GetAt(nPos);
		int nFieldType = nField % 100;

		switch( nFieldType )
		{
		case SH_FIELD_CODE:

			if ( m_StockLevelTree.GetSummaryFlag() == FALSE )
			{
				csv.Add ( ReportHelpers.GetDisplayStockCode( m_LabelStockRecord.GetStockCode() ) );
				csv.Add ( m_LabelStockRecord.GetDescription() );
				
				if ( FALSE == m_bGotCategoryField )
					csv.Add ( DataManager.Category.GetText( m_LabelStockRecord.GetCategory() ) );
			}

			break;

		case SH_FIELD_CAT_ITEM:
			csv.Add ( m_strLabelCategory );
			break;

		case SH_FIELD_SUPP:
			csv.Add( m_strLabelSupplier );
			break;

		case SH_FIELD_SP:
			csv.Add( m_strLabelStockpoint );
			break;

		case SH_FIELD_CUSTOM1:
		case SH_FIELD_CUSTOM2:
		case SH_FIELD_CUSTOM3:
		case SH_FIELD_CUSTOM4:
		case SH_FIELD_CUSTOM5:
			csv.Add( m_strLabelCustomField[ nFieldType + 1 - SH_FIELD_CUSTOM1 ] );
			break;
		}
	}

	if ( TRUE == m_bForceBlankColumn )
		csv.Add( "Grand Total" );

	if ( TRUE == bStockUnit )
		csv.Add( DataManager.Stock.GetStockSubUnitName( m_LabelStockRecord ) );
}

/**********************************************************************/

void CStockTreeReportNew::AddSupplierColumns( bool bName, bool bRef )
{
	if ( m_StockLevelTree.GetSummaryFlag() == FALSE )
	{
		if ( bName == TRUE )	AddColumn( "Supplier", TA_LEFT, 350 );
		if ( bRef == TRUE )		AddColumn( "Reference", TA_LEFT, 220 );
	}
}

/**********************************************************************/

void CStockTreeReportNew::OutputSupplierFields( CCSV& csv, bool bName, bool bRef, int nStockSuppIdx )
{
	if ( ( nStockSuppIdx >= 0 ) && ( nStockSuppIdx < m_LabelStockRecord.GetSupplierCount() ) )
	{
		if ( bName == TRUE )
		{
			int nSuppIdx;
			if ( DataManager.Supplier.FindSupplierByNumber ( m_LabelStockRecord.GetSuppNo( nStockSuppIdx ), nSuppIdx ) == TRUE )
				csv.Add( DataManager.Supplier.GetName( nSuppIdx ) );
			else
				csv.Add( "" );
		}
				
		if ( bRef == TRUE )
			csv.Add( m_LabelStockRecord.GetSuppRefExternal( nStockSuppIdx, DealerFlags.GetSuppRefFlag() ) );
	}	
	else
	{
		if ( bName == TRUE )	csv.Add ( "" );
		if ( bRef == TRUE )		csv.Add ( "" );
	}
}

/**********************************************************************/

bool CStockTreeReportNew::CreateReport()
{
	StatusProgress.Lock( TRUE, "Creating report" );
	bool bResult = CreateReportInternal();
	StatusProgress.Unlock();
	return bResult;
}

/**********************************************************************/

bool CStockTreeReportNew::CreateReportInternal()
{	
	CWordArray arraySuppIdx;
	m_StockLevelTree.GetSuppIdxArray( arraySuppIdx );
	m_SupplierReportMap.CreateMap( arraySuppIdx );

	m_bLastLineWasBlank = TRUE;

	if ( m_ReportFile.Open ( Super.ReportFilename() ) == FALSE )
		return FALSE;

	bool bGotData = FALSE;
	m_ReportFile.SetStyle1 ( m_strReportName );

	for ( int n = 0; n < m_arrayColumnNames.GetSize(); n++ )
	{
		CStockTreeColumnNew column = m_arrayColumnNames.GetAt( n );
		m_ReportFile.AddColumn( column.m_strText, column.m_nFormat, column.m_nWidth );
	}

	m_nProgress = 0;
	m_nProgressTarget = m_StockLevelTree.GetNodeCount();

	switch( m_nReportParamsType )
	{
	case STOCK_TOP_100_APPARENT:
	case STOCK_TOP_100_SALES:
		m_nProgressTarget *= 2;
		break;
	}

	ProcessStockLevelsArray(0,0 );

	WriteBlankLine( FALSE );
	ShowStockpoints();

	m_ReportFile.Close();

	return TRUE;
}

/**********************************************************************/

void CStockTreeReportNew::ProcessStockLevelsArray(int nArray, int nDepth )
{
	int nFieldType = m_arrayReportFields.GetAt( nDepth );	
	bool bHeader = ( m_arrayHeaderFlags.GetAt(nDepth) == 1 );
	bool bTotal = ( m_arrayTotalFlags.GetAt(nDepth) == 1 );
	
	int nArraySize = m_StockLevelTree.GetArraySize(nArray);
	if ( nArraySize == 0 )
		return;

	for ( int n = m_nTotalisersPerLine * nDepth; n < m_nTotalisersPerLine * ( nDepth + 1 ); n++ )
		m_arrayTotalisers[n] = 0.0;
	
	CStockLevelsCSVRecord StockLevels;

	if ( bHeader == TRUE )
		WriteHeader( nDepth );

	for ( int nNodeIdx = 0; nNodeIdx < nArraySize; nNodeIdx++ )
	{
		StatusProgress.SetPos( m_nProgress++, m_nProgressTarget );
		
		int n = m_StockLevelTree.GetLabelInt( nArray, nNodeIdx ); 

		switch ( nFieldType )
		{
		case SH_FIELD_CODE:		
			m_nLabelStockIdx = m_StockLevelTree.m_StockArrayIndexer.GetNativeIdxForSortedIdx(n);
			DataManager.Stock.GetAt( m_nLabelStockIdx, m_LabelStockRecord );
			break;
		
		case SH_FIELD_CAT_ITEM:		
			m_nLabelCategory = n;	
			m_strLabelCategory = DataManager.Category.GetText(n);
			break;

		case SH_FIELD_CUSTOM1:
		case SH_FIELD_CUSTOM2:
		case SH_FIELD_CUSTOM3:
		case SH_FIELD_CUSTOM4:
		case SH_FIELD_CUSTOM5:
			{
				int nFieldNo = ( nFieldType - SH_FIELD_CUSTOM1 ) + 1;
				
				if ( CUSTOM_FIELD_SORTIDX_NA == n )
				{
					m_strLabelCustomField[ nFieldNo ] = DataManager.CustomFieldManager.GetFieldNameByFieldNo( nFieldNo );
					m_strLabelCustomField[ nFieldNo ] += " N/A";
				}
				else if ( ( n < 0 ) || ( n >= DataManager.CustomFieldValue[ nFieldNo ].GetSize() ) )
				{
					m_strLabelCustomField[ nFieldNo ] = DataManager.CustomFieldManager.GetFieldNameByFieldNo( nFieldNo );
					m_strLabelCustomField[ nFieldNo ] = " Unknown";
				}
				else
					m_strLabelCustomField[ nFieldNo ] = DataManager.CustomFieldValue[ nFieldNo ].GetName( n );
			}
			break;

		case SH_FIELD_SUPP:		
			{
				m_nLabelSuppIdx = m_SupplierReportMap.GetSuppIdxBySortIdx(n);		
			
				switch ( m_nLabelSuppIdx )
				{
				case -1:
					m_strLabelSupplier = "Unknown Supplier";
					break;

				case 0:
					m_strLabelSupplier = "Unspecified Supplier";
					break;

				default:
					m_strLabelSupplier = DataManager.Supplier.GetName( m_nLabelSuppIdx - 1 );
					break;
				}					
			}
			break;
		
		case SH_FIELD_DBASE:
			m_nLabelDbIdx = dbDatabase.GetDbIdxFromSortIdx( n );
			m_strLabelDatabase = dbDatabase.GetName( m_nLabelDbIdx );
			break;

		case SH_FIELD_SITE:		
			m_nLabelSiteNo = n;	
			
			if ( n == 0 )
				m_strLabelSite = "No Site";
			else
			{
				int nSiteIdx;
				int nDbNo = dbDatabase.GetDbNo( DataManager.GetActiveDbIdx() );
				if ( dbSite.FindSiteByNumber( nDbNo, n, nSiteIdx ) == TRUE )
					m_strLabelSite = dbSite.GetName( nSiteIdx );
				else
					m_strLabelSite =  "Unknown Site";
			}
			break;
		
		case SH_FIELD_SP:		
			m_nLabelSpIdx = dbStockpoint.GetSpIdxFromSortIdx( n );		
			m_strLabelStockpoint = dbStockpoint.GetName( m_nLabelSpIdx );
			break;
		}

		int nNextArrayIdx = m_StockLevelTree.GetNextArrayIdx( nArray, nNodeIdx );
		
		if ( nDepth == m_arrayReportFields.GetSize() - 1 )
		{
			if ( TRUE == m_bWantTransferDetail )
			{
				CStockLevelsCSVRecord StockLevels;
				StockLevels = m_StockLevelTree.GetStockLevels( nArray, nNodeIdx );
				
				CTransferDetailCSVRecord TransferDetail;
				m_StockLevelTree.GetTransferDetail( nArray, nNodeIdx, TransferDetail );
				
				CreateReportLine( StockLevels, TransferDetail );
			}
			else if ( TRUE == m_bWantSalesCost )
			{
				CStockLevelsCSVRecord StockLevels;
				StockLevels = m_StockLevelTree.GetStockLevels( nArray, nNodeIdx );
				
				CSalesCostCSVRecord SalesCost;
				m_StockLevelTree.GetSalesCost( nArray, nNodeIdx, SalesCost );
				
				CreateReportLine( StockLevels, SalesCost );
			}
			else
			{
				CStockLevelsCSVRecord StockLevels;
				StockLevels = m_StockLevelTree.GetStockLevels( nArray, nNodeIdx );
				CreateReportLine( StockLevels );		
			}
		}
		else
			ProcessStockLevelsArray( nNextArrayIdx, nDepth + 1 );
	}

	if ( bTotal == TRUE )
		CreateTotalsLine( nDepth );
}

/**********************************************************************/

const char* CStockTreeReportNew::GetHeaderLine( int nDepth )
{
	CString strSpace = " ";
	CCSV csvHeader;
	
	for ( int nPos = 0; nPos < nDepth; nPos++ )
	{
		int nFieldType = m_arrayReportFields.GetAt( nPos );
		switch ( nFieldType )
		{
		case SH_FIELD_SP:		csvHeader.Add( strSpace + m_strLabelStockpoint );	break;
		case SH_FIELD_SITE:		csvHeader.Add( strSpace + m_strLabelSite );		break;
		case SH_FIELD_DBASE:	csvHeader.Add( strSpace + m_strLabelDatabase );	break;
		case SH_FIELD_CAT_ITEM:	csvHeader.Add( strSpace + m_strLabelCategory );	break;
		case SH_FIELD_SUPP:		csvHeader.Add( strSpace + m_strLabelSupplier );	break;
		case SH_FIELD_CUSTOM1:	csvHeader.Add( strSpace + m_strLabelCustomField[1] );	break;
		case SH_FIELD_CUSTOM2:	csvHeader.Add( strSpace + m_strLabelCustomField[2] );	break;
		case SH_FIELD_CUSTOM3:	csvHeader.Add( strSpace + m_strLabelCustomField[3] );	break;
		case SH_FIELD_CUSTOM4:	csvHeader.Add( strSpace + m_strLabelCustomField[4] );	break;
		case SH_FIELD_CUSTOM5:	csvHeader.Add( strSpace + m_strLabelCustomField[5] );	break;
		case SH_FIELD_CODE:	
			{
				CString strStockCode = m_LabelStockRecord.GetStockCode();
				CString strDisplayCode = ReportHelpers.GetDisplayStockCode( strStockCode );
				csvHeader.Add( strSpace + strDisplayCode ); 
			}
			break;
		}
	}

	m_strHeader = csvHeader.GetLine();
	return m_strHeader;
}

/**********************************************************************/

void CStockTreeReportNew::AddTotalColumnText( CCSV& csv, int nDepth )
{
	CString strSpace = " ";
	CCSV csvHeader;
	
	csv.RemoveAll();
	
	if ( nDepth <= 0 )
	{
		csv.Add ( "<..>Grand Total" );
	}
	else if ( nDepth < m_arrayReportFields.GetSize() )
	{
		if ( ( m_StockLevelTree.GetSummaryFlag() == FALSE ) || ( m_arrayReportFields.GetSize() != nDepth + 1 ) )
		{
			for ( int nPos = 0; nPos < nDepth; nPos++ )
			{
				int nFieldType = m_arrayReportFields.GetAt( nPos );
				switch ( nFieldType )
				{
				case SH_FIELD_SP:		csvHeader.Add( strSpace + m_strLabelStockpoint );	break;
				case SH_FIELD_SITE:		csvHeader.Add( strSpace + m_strLabelSite );		break;
				case SH_FIELD_DBASE:	csvHeader.Add( strSpace + m_strLabelDatabase );	break;
				case SH_FIELD_CAT_ITEM:	csvHeader.Add( strSpace + m_strLabelCategory );	break;
				case SH_FIELD_SUPP:		csvHeader.Add( strSpace + m_strLabelSupplier );	break;
				case SH_FIELD_CUSTOM1:	csvHeader.Add( strSpace + m_strLabelCustomField[1] );	break;
				case SH_FIELD_CUSTOM2:	csvHeader.Add( strSpace + m_strLabelCustomField[2] );	break;
				case SH_FIELD_CUSTOM3:	csvHeader.Add( strSpace + m_strLabelCustomField[3] );	break;
				case SH_FIELD_CUSTOM4:	csvHeader.Add( strSpace + m_strLabelCustomField[4] );	break;
				case SH_FIELD_CUSTOM5:	csvHeader.Add( strSpace + m_strLabelCustomField[5] );	break;
				}
			}

			CString strTotal = "<..>";
			strTotal += "Total (";
			strTotal += csvHeader.GetLine();
			strTotal += " )";

			csv.Add( strTotal );
		}
		else
		{
			CString strTotal = "<..>";
		
			int nPos = nDepth - 1;
			if ( nPos >= 0 )
			{
				int nFieldType = m_arrayReportFields.GetAt( nPos );
				switch ( nFieldType )
				{
				case SH_FIELD_SP:		strTotal += m_strLabelStockpoint;		break;
				case SH_FIELD_SITE:		strTotal += m_strLabelSite;				break;
				case SH_FIELD_DBASE:	strTotal += m_strLabelDatabase;			break;
				case SH_FIELD_CAT_ITEM:	strTotal += m_strLabelCategory;			break;
				case SH_FIELD_SUPP:		strTotal += m_strLabelSupplier;			break;
				case SH_FIELD_CUSTOM1:	strTotal += m_strLabelCustomField[1];	break;
				case SH_FIELD_CUSTOM2:	strTotal += m_strLabelCustomField[2];	break;
				case SH_FIELD_CUSTOM3:	strTotal += m_strLabelCustomField[3];	break;
				case SH_FIELD_CUSTOM4:	strTotal += m_strLabelCustomField[4];	break;
				case SH_FIELD_CUSTOM5:	strTotal += m_strLabelCustomField[5];	break;
				}
			}

			csv.Add( strTotal );
		}
	}

	while ( csv.GetSize() < m_nItemIDColumns  )
		csv.Add( "" );
}

/**********************************************************************/

void CStockTreeReportNew::WriteReportLine( const char* szLine )
{
	CString strLine = szLine;
	if ( strLine == "" )
		return;

	m_ReportFile.WriteLine( strLine );
	m_bLastLineWasBlank = FALSE;
}

/**********************************************************************/

void CStockTreeReportNew::WriteHeader( int nDepth )
{
	if ( m_bBlankBeforeHeader == TRUE )
		WriteBlankLine( TRUE );
	
	CString strTemp = GetHeaderLine(nDepth);

	if ( strTemp.GetLength() >= 1 ) 
	{
		strTemp = strTemp.Right( strTemp.GetLength() - 1 );
		CString strHeader = "<..>";
		strHeader += strTemp;
		WriteReportLine( strHeader );
		WriteReportLine( "<LI>" );
	}
}

/**********************************************************************/

void CStockTreeReportNew::WriteBlankLine( bool bOnlyIfNeeded )
{
	if ( bOnlyIfNeeded && m_bLastLineWasBlank )
		return;

	m_ReportFile.WriteLine ( "" );
	m_bLastLineWasBlank = TRUE;
}

/**********************************************************************/

double CStockTreeReportNew::CalculateAverage( double dValue, double dQuantity )
{
	if ( dQuantity == 0.0 )
		return 0.0;
	else
		return dValue / dQuantity;
}

/**********************************************************************/

double CStockTreeReportNew::CalculateGPPercent ( double dRetailExTax, double dCostAndWaste )
{
	double dGPPercent = 0.0;
	
	if ( dRetailExTax != 0.0 ) 
		dGPPercent = ( ( dRetailExTax - dCostAndWaste ) * 100  ) / dRetailExTax;
	
	if ( dGPPercent < 0.0 ) dGPPercent = 0.0;
	if ( dGPPercent > 100.0 ) dGPPercent = 100.0;
	
	return dGPPercent;
}

/**********************************************************************/

double CStockTreeReportNew::EstimateWastageFromSales( double dSales )
{	
	double dWastePercent = m_LabelStockRecord.GetWastePercent();
	double dNonWastePercent = 100.0 - dWastePercent;

	if ( dWastePercent <= 0.0 || dNonWastePercent <= 0.0 )
		return 0.0;
	else
		return ( dSales * dWastePercent ) / dNonWastePercent;
}

/**********************************************************************/

double CStockTreeReportNew::EstimateCostFromSUQty( double dSUQty )
{
	return m_LabelStockRecord.GetUnitCost( 0 ) * dSUQty;
}

/**********************************************************************/

double CStockTreeReportNew::EstimateWastageFromSalesAndWastage( double dValue )
{
	double dWastePercent = m_LabelStockRecord.GetWastePercent();
	double dNonWastePercent = 100.0 - dWastePercent;

	if ( dWastePercent <= 0.0 || dNonWastePercent <= 0.0 )
		return 0.0;
	else
		return ( dValue * dWastePercent ) / 100.0;
}

/**********************************************************************/

void CStockTreeReportNew::GetSortedUnitCosts( CArray<double,double>& arrayCosts, double dQty, int nSize )
{
	arrayCosts.RemoveAll();

	bool bNegative = FALSE;
	if ( dQty < 0.0 )
	{
		dQty = -dQty;
		bNegative = TRUE;
	}

	for ( int n = 0; n < m_LabelStockRecord.GetSupplierCount(); n++ )
	{
		if ( m_LabelStockRecord.GetSuppNo(n) <= 0 )
			continue;

		double dValue = m_LabelStockRecord.GetUnitCost(n) * dQty;

		bool bInserted = FALSE;
		for ( int x = 0; x < arrayCosts.GetSize(); x++ ) 
		{
			if ( dValue <= arrayCosts.GetAt( x ) )
			{
				arrayCosts.InsertAt( x, dValue );
				bInserted = TRUE;
				break;
			}
		}

		if ( bInserted == FALSE )
			arrayCosts.Add( dValue );
	}

	double dFill = 0.0;
	if ( arrayCosts.GetSize() > 0 )
		dFill = arrayCosts.GetAt( arrayCosts.GetSize() - 1 );

	while( arrayCosts.GetSize() < nSize )
		arrayCosts.Add( dFill );

	while( arrayCosts.GetSize() > nSize )
		arrayCosts.RemoveAt( arrayCosts.GetSize() - 1 );

	double dPrefCost = m_LabelStockRecord.GetUnitCost(0) * dQty;
	arrayCosts.InsertAt( 0, dPrefCost );

	if ( bNegative == TRUE )
	{
		for ( int n = 0; n < arrayCosts.GetSize(); n++ )
			arrayCosts.SetAt( n, -arrayCosts.GetAt(n) );
	}
}

/**********************************************************************/

bool CStockTreeReportNew::GetRetailPluInfo( bool bWantPluStockQty )
{
	bool bResult = FALSE;

	m_strRetailPluNo = "";

	int nStockPluIdx;
	CString strStockCode = m_LabelStockRecord.GetStockCode();
	if ( DataManager.StockPlu.FindStockCode ( strStockCode, nStockPluIdx ) == TRUE )
	{
		if ( DataManager.StockPlu.GetPluNoCount ( nStockPluIdx ) != 0 )
		{
			int nPluIdx;
			__int64 nPluNo = DataManager.StockPlu.GetPluNo ( nStockPluIdx, 0 );	
			if ( DataManager.Plu.FindPluByNumber ( nPluNo, nPluIdx ) == TRUE )
			{
				CPluCSVRecord PluRecord;
				DataManager.Plu.GetAt( nPluIdx, PluRecord );
				
				m_nRetailPluIdx = nPluIdx;
				m_strRetailPluNo = PluRecord.GetPluNoString();
				m_nRetailTaxBand = PluRecord.GetBaseTaxBandInt();

				CTaxRateInfo TaxRateInfo;
				DataManager.TaxRates.GetTaxRateInfo( m_nRetailTaxBand, TaxRateInfo );

				m_dRetailTaxRate = TaxRateInfo.m_dTaxRate;
				m_dRetailPrice1 = PluRecord.GetPrice(0);

				if ( bWantPluStockQty == TRUE )
				{
					__int64 nPluNo = _atoi64( m_strRetailPluNo );
					DataManager.Plu.GetPluStockQty( nPluNo, m_dRetailStockQty );
				}

				bResult = TRUE;
			}
		}
		else if ( DealerFlags.GetStockReportUseRecipeCostLinkFlag() == TRUE ) 
		{
			if ( DataManager.StockPlu.GetRecipePluNoCount( nStockPluIdx ) != 0 )
			{
				int nPluIdx;
				__int64 nPluNo = DataManager.StockPlu.GetRecipePluNo ( nStockPluIdx, 0 );
				if ( DataManager.Plu.FindPluByNumber ( nPluNo, nPluIdx ) == TRUE )
				{
					CPluCSVRecord PluRecord;
					DataManager.Plu.GetAt( nPluIdx, PluRecord );
			
					CExplodePluDatabase ExplodePlu;
					
					CStringArray StockDetails;
					if ( ExplodePlu.ExplodePlu( nPluNo, StockDetails, AUDIT_SALES, 1.00, PluRecord.GetPrice(0) ) == PLU_EXCEPTION_NONE )
					{
						int nRecipePos = -1;
	
						for ( int nIndex = 0; nIndex < StockDetails.GetSize(); nIndex++ )
						{
							CCSVExplodedPlu csv ( StockDetails.GetAt ( nIndex ) );
					
							if ( strStockCode == csv.GetStockCode() )
							{
								nRecipePos = nIndex;
								break;
							}
						}
					
						if ( nRecipePos != -1 )
						{
							CCSVExplodedPlu csvSale ( StockDetails.GetAt ( nRecipePos ) );

							m_nRetailPluIdx = nPluIdx;
							m_strRetailPluNo = PluRecord.GetPluNoString();
							m_nRetailTaxBand = PluRecord.GetBaseTaxBandInt();

							CTaxRateInfo TaxRateInfo;
							DataManager.TaxRates.GetTaxRateInfo( m_nRetailTaxBand, TaxRateInfo );

							m_dRetailTaxRate = TaxRateInfo.m_dTaxRate;
							m_dRetailPrice1 = csvSale.GetStockValue();

							if ( TRUE == bWantPluStockQty )
								m_dRetailStockQty = csvSale.GetStockQty();

							bResult = TRUE;
						}
					}
				}
			}
		}
	}

	return bResult;
}

/*********************************************************************/

void CStockTreeReportNew::ShowStockpoints()
{
	WriteReportLine ( "<..>Report includes stockpoints" );
	WriteReportLine ( "<LI>" );

	CArray<int,int> arraySpIdx;
	
	for ( int nSpIdx = 0; nSpIdx < dbStockpoint.GetSize(); nSpIdx++ )
		if ( m_StockLevelTree.MatchStockpoint ( nSpIdx ) == TRUE )
			arraySpIdx.Add( nSpIdx );

	//if ( EcrmanOptions.GetSortLocNameFlag() == TRUE )
		dbStockpoint.SortSpListByName( arraySpIdx );

	for ( int n = 0; n < arraySpIdx.GetSize(); n++ )
	{
		int nSpIdx = arraySpIdx.GetAt(n);

		CString strLine = "<..>";
		strLine += dbStockpoint.GetName( nSpIdx );
		WriteReportLine ( strLine );
	}
}

/**********************************************************************/

const char* CStockTreeReportNew::GetReportParamsFilename()
{
	CFilenameUpdater FnUp( SysFiles::StockReportPrm, 0, m_nReportParamsType );
	m_strReportParamsFilename = FnUp.GetFilenameToUse();
	return m_strReportParamsFilename;
}

/**********************************************************************/

const char* CStockTreeReportNew::GetReportParamsKey()
{
	CCSV csv;

	for ( int n = 0; n < m_arrayColumnNames.GetSize(); n++ )
		csv.Add ( m_arrayColumnNames.GetAt(n).m_strText );

	m_strReportParamsKey = csv.GetLine();
	return m_strReportParamsKey;
}

/**********************************************************************/

const char* CStockTreeReportNew::GetReportTitle()
{
	CStockReportNames ReportNames;
	m_strReportTitle = ReportNames.GetComboName( m_nReportParamsType );
	m_strReportTitle += " report for ";
	m_strReportTitle += m_strConLevelText;
	return m_strReportTitle;
}

/**********************************************************************/

const char* CStockTreeReportNew::GetRetailTaxBandString()
{
	m_strRetailTaxBand = CTaxArray::GetTaxBandFromNumber(m_nRetailTaxBand, 0, "?");
	return m_strRetailTaxBand;
}

/**********************************************************************/
