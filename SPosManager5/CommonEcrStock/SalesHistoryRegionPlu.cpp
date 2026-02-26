/**********************************************************************/
#include "share.h"
/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "EposReports.h"
#include "HistoryFileTypes.h"
#include "LocationCSVArray.h"
#include "LocationSetCSVArray.h"
#include "NetworkCSVArray.h"
#include "NodeTypes.h"
#include "PMSCheckoutDateArray.h"
#include "PMSOptions.h"
#include "PMSReportFileFinder.h"
#include "RepCSV.h"
#include "ReportHelpers.h"
#include "SalesHistoryFields.h"
#include "StatusProgress.h"
#include "StringDateChecker.h"
#include "SysInfo.h"
/**********************************************************************/
#include "SalesHistoryRegionPlu.h"
/**********************************************************************/

CSalesHistoryRegionPlu::CSalesHistoryRegionPlu()
{
	m_nReportType = SALESHISTORY_REGION_PLU;
	m_nZeroSkipType = ZEROSKIP_NONE;

	m_bItemRange = FALSE;
	m_nItemFrom = 0;
	m_nItemTo = 0;

	m_nDeptFilter = DEPARTMENTSET_ALL;
	m_DepartmentSet.CreateDepartmentSet( m_nDeptFilter );
	m_bSaleTime = FALSE;
	
	m_nTaxFilter = TAXBAND_FILTER_ALL;

	m_nDeptSortDbIdx = -1;
	m_bInvalidTotals = TRUE;
	m_bInvalidFilter = TRUE;
	m_bInvalidDeptSort = TRUE;
	m_bPriceBandSplit = FALSE;

	for ( int n = 0; n < MAX_PLU_PRICES + MAX_SBPLU_PRICES; n++ )
		m_pArrayCells[n] = new CArray<CSalesHistoryCell,CSalesHistoryCell>;
}

/**********************************************************************/

CSalesHistoryRegionPlu::~CSalesHistoryRegionPlu()
{
	for ( int n = 0; n < MAX_PLU_PRICES + MAX_SBPLU_PRICES; n++ )
	{
		if ( GetCellArray(n) != NULL )
		{
			GetCellArray(n) -> RemoveAll();
			delete( GetCellArray(n) );
		}
	}
}

/**********************************************************************/

CArray<CSalesHistoryCell,CSalesHistoryCell>* CSalesHistoryRegionPlu::GetCellArray( int n )
{
	if ( ( n >= 0 ) && ( n < MAX_PLU_PRICES + MAX_SBPLU_PRICES ) )
	{
		if ( m_pArrayCells[n] != NULL )
			return (CArray<CSalesHistoryCell,CSalesHistoryCell>*) m_pArrayCells[n];
	}
		
	return NULL;
}

/**********************************************************************/

void CSalesHistoryRegionPlu::Configure( COleDateTime& dateFrom, COleDateTime& dateTo, CArray<CDateRangeOrRegionColumnInfo,CDateRangeOrRegionColumnInfo>& arrayColumns, bool bByLocation, int nDeptFilter, int nTaxFilter, bool bItemRange, __int64 nItemFrom, __int64 nItemTo, bool bPriceBandSplit, CSessionCSVRecord& Session )
{
	m_bInvalidTotals = FALSE;
	m_bInvalidFilter = FALSE;
	m_bInvalidDeptSort = FALSE;

	if ( nDeptFilter != m_nDeptFilter )
	{
		m_bInvalidFilter = TRUE;
		m_nDeptFilter = nDeptFilter;
		m_DepartmentSet.CreateDepartmentSet( m_nDeptFilter );
	}

	if ( m_nTaxFilter != nTaxFilter )
	{
		m_bInvalidFilter = TRUE;
		m_bInvalidTotals = TRUE;
		m_nTaxFilter = nTaxFilter;
	}

	if ( bItemRange != m_bItemRange )
	{
		m_bInvalidFilter = TRUE;
		m_bItemRange = bItemRange;
	}

	if ( m_nItemFrom != nItemFrom )
	{
		m_bInvalidFilter = TRUE;
		m_nItemFrom = nItemFrom;
	}

	if ( m_nItemTo != nItemTo )
	{
		m_bInvalidFilter = TRUE;
		m_nItemTo = nItemTo;
	}

	if ( m_bPriceBandSplit != bPriceBandSplit )
	{
		m_bInvalidTotals = TRUE;
		m_bPriceBandSplit = bPriceBandSplit;
	}

	{
		CString strLastID = m_DateTimeFilter.GetSessionID();
		CSortedDateTimeItem itemLastTimeFrom = m_DateTimeFilter.GetDateFrom();
		CSortedDateTimeItem itemLastTimeTo = m_DateTimeFilter.GetDateTo();

		m_DateTimeFilter.SetDates( dateFrom, dateTo, Session );

		if ( strLastID != m_DateTimeFilter.GetSessionID() )
			m_bInvalidTotals = TRUE;

		if ( dateFrom != itemLastTimeFrom.m_oleDateTime )
			m_bInvalidTotals = TRUE;

		if ( dateTo != itemLastTimeTo.m_oleDateTime )
			m_bInvalidTotals = TRUE;
	}

	//FIDDLE - NON STANDARD USE OF ECR SELECT ARRAY AS LOC TO COLUMN MAP
	m_LocToColumnMap.RemoveAll();
	for ( int nColumn = 0; nColumn < arrayColumns.GetSize(); nColumn++ )
	{
		int nIndex = arrayColumns[nColumn].m_nTableIdx;

		switch ( arrayColumns[nColumn].m_nType )
		{
		case NODE_LOCATION:
			m_LocToColumnMap.MapLocToColumn( nIndex, nColumn );
			break;

		case NODE_LOCATIONSET:
			{
				CReportConsolidationArray<CSortedIntItem> arrayTemp;
				dbLocationSet.TidyNwkLocNoArray( nIndex, arrayTemp );

				CWordArray LocIdxArray;
				for ( int n = 0; n < arrayTemp.GetSize(); n++ )
				{
					CSortedIntItem item;
					arrayTemp.GetAt( n, item );
					LocIdxArray.Add( item.m_nItem );
				}

				for ( int n = 0; n < LocIdxArray.GetSize(); n++ )
				{
					int nLocIdx = LocIdxArray.GetAt(n);
					m_LocToColumnMap.MapLocToColumn( nLocIdx, nColumn );
				}
			}
			break;

		case NODE_DATABASE:
			{
				CArray<int,int> arrayLocIdx;
				dbLocation.GetDatabaseLocIdxSet( nIndex, arrayLocIdx );
				
				for ( int n = 0; n < arrayLocIdx.GetSize(); n++ )
				{
					int nLocIdx = arrayLocIdx.GetAt(n);
					m_LocToColumnMap.MapLocToColumn( nLocIdx, nColumn );
				}
			}
			break;
		}
	}

	if ( bByLocation == TRUE )
	{
		CWordArray arrayLocIdx;

		for ( int nLocIdx = 0; nLocIdx < dbLocation.GetSize(); nLocIdx++ )
			if ( m_LocToColumnMap.GetColumnCount( nLocIdx ) > 0 )
				arrayLocIdx.Add( nLocIdx );

		arrayColumns.RemoveAll();
		m_LocToColumnMap.RemoveAll();

		for ( int nCol = 0; nCol < arrayLocIdx.GetSize(); nCol++ )
		{
			int nLocIdx = arrayLocIdx.GetAt(nCol);
			m_LocToColumnMap.MapLocToColumn( nLocIdx, nCol );

			int nDbIdx = 0;
			int nDbNo = dbLocation.GetDbNo( nLocIdx );
			if ( dbDatabase.FindDatabaseByNumber( nDbNo, nDbIdx ) == FALSE )
				nDbIdx = 0;

			CDateRangeOrRegionColumnInfo info;
			info.m_nType = NODE_LOCATION;
			info.m_nDbIdx = nDbIdx;
			info.m_nTableIdx = nLocIdx;
			arrayColumns.Add( info );
		}
	}

	if ( arrayColumns.GetSize() != m_arrayColumns.GetSize() )
	{
		m_bInvalidFilter = TRUE;
		m_bInvalidTotals = TRUE;
	}
	else
	{
		for ( int n = 0; n < arrayColumns.GetSize(); n++ )
		{
			CDateRangeOrRegionColumnInfo info1, info2;
			info1 = arrayColumns.GetAt(n);
			info2 = m_arrayColumns.GetAt(n);

			if ( info1.m_nType != info2.m_nType )
			{
				m_bInvalidFilter = TRUE;
				m_bInvalidTotals = TRUE;
				break;
			}

			if ( info1.m_nDbIdx != info2.m_nDbIdx )
			{
				m_bInvalidFilter = TRUE;
				m_bInvalidTotals = TRUE;
				break;
			}

			if ( info1.m_nTableIdx != info2.m_nTableIdx )
			{
				m_bInvalidFilter = TRUE;
				m_bInvalidTotals = TRUE;
				break;
			}
		}
	}

	m_arrayColumns.RemoveAll();
	for ( int n = 0; n < arrayColumns.GetSize(); n++ )
	{
		CDateRangeOrRegionColumnInfo info = arrayColumns.GetAt(n);
		m_arrayColumns.Add( info );
	}
	
	int nDbIdx = 0;
	if ( m_arrayColumns.GetSize() != 0 )
		nDbIdx = m_arrayColumns[0].m_nDbIdx;

	CDataManagerInfo info;
	DataManager.OpenDatabaseReadOnly( nDbIdx, info, FALSE );

	if ( nDbIdx != m_nDeptSortDbIdx )
		m_bInvalidDeptSort = TRUE;
}

/**********************************************************************/

void CSalesHistoryRegionPlu::ProcessDate ( int nCol, COleDateTime& date )
{
	if ( nCol < 0 || nCol >= m_arrayDateRange.GetSize() )
		return;

	if ( date.GetStatus() != COleDateTime::valid )
		return;

	CDateRangeInfo info = m_arrayDateRange.GetAt(nCol);

	if ( info.m_dateStart.GetStatus() == COleDateTime::null )
	{
		info.m_dateStart = date;
		info.m_dateEnd = date;
	}
	else
	{
		if ( date < info.m_dateStart )
			info.m_dateStart = date;

		if ( date > info.m_dateEnd )
			info.m_dateEnd = date;
	}

	m_arrayDateRange.SetAt( nCol, info );
}

/**********************************************************************/

const char* CSalesHistoryRegionPlu::GetReportTitle( const char* szReportName )
{
	CSortedDateTimeItem infoFrom = m_DateTimeFilter.GetDateFrom();
	CSortedDateTimeItem infoTo = m_DateTimeFilter.GetDateTo();

	m_strTitle.Format ( "%s (%2.2d/%2.2d/%4.4d - %2.2d/%2.2d/%4.4d%s, %s)",
		 szReportName,
		 infoFrom.m_oleDateTime.GetDay(),
		 infoFrom.m_oleDateTime.GetMonth(),
		 infoFrom.m_oleDateTime.GetYear(),
		 infoTo.m_oleDateTime.GetDay(),
		 infoTo.m_oleDateTime.GetMonth(),
		 infoTo.m_oleDateTime.GetYear(),
		 m_DepartmentSet.GetFilterName( TRUE, TRUE),
		 m_DateTimeFilter.GetSessionName() );
	
	return m_strTitle;
}

/**********************************************************************/

const char* CSalesHistoryRegionPlu::GetColumnName( int nCol )
{
	m_strColumnName = "";

	if ( ( nCol >= 0 ) && ( nCol < m_arrayColumns.GetSize() ) )
	{
		int nTableIdx = m_arrayColumns[nCol].m_nTableIdx;

		switch( m_arrayColumns[nCol].m_nType )
		{
		case NODE_LOCATION:		
			m_strColumnName = dbLocation.GetName( nTableIdx );
			break;

		case NODE_LOCATIONSET:	
			m_strColumnName = dbLocationSet.GetName( nTableIdx );
			break;

		case NODE_DATABASE:		
			m_strColumnName = dbDatabase.GetName( nTableIdx );
			break;
		}
	}

	return m_strColumnName;
}

/**********************************************************************/

void CSalesHistoryRegionPlu::GetFileListPlu ( CReportConsolidationArray<CTermFileListInfo>& arrayFileInfo, int& nTotalFileSize )
{
	CWaitCursor wait;

	arrayFileInfo.RemoveAll();
	
	nTotalFileSize = 0;

	bool bWantPMSSales = FALSE;
	int nPMSLocIdx = 0;

	CSortedDateTimeItem itemFrom = m_DateTimeFilter.GetCheckDateFrom();
	CSortedDateTimeItem itemTo = m_DateTimeFilter.GetCheckDateTo();

	CString strCheckDateFrom = itemFrom.m_strDateTime;
	CString strCheckDateTo = itemTo.m_strDateTime;

	for ( int nLocIdx = 0; nLocIdx < dbLocation.GetSize(); nLocIdx++ )
	{
		if ( m_LocToColumnMap.GetColumnCount( nLocIdx ) == 0 )
			continue;

		if ( SysInfo.IsPMSSystem() == TRUE )
		{
			if ( dbLocation.IsPMSPseudoLocation( nLocIdx ) == TRUE )
			{
				bWantPMSSales = TRUE;
				nPMSLocIdx = nLocIdx;	
				continue;
			}
		}

		int nNetworkIdx;
		int nNetworkNo = dbLocation.GetNetworkNo( nLocIdx );
		if ( dbNetwork.FindNetworkByNumber ( nNetworkNo, nNetworkIdx ) == FALSE )
			continue;

		CString strDataPath = dbNetwork.GetFolderPathData ( nNetworkIdx );
		strDataPath += "\\";

		CStringDateChecker StringDateChecker( strCheckDateFrom, strCheckDateTo );
		
		CFileFind FileFinder;	
		bool bWorking = ( FileFinder.FindFile ( strDataPath + "*.*" ) != 0 );

		while (bWorking)   	
		{	
			( bWorking = FileFinder.FindNextFile() != 0 );
	
			//IGNORE CURRENT AND PARENT DIRECTORY MARKERS	
			if ( FileFinder.IsDots() == TRUE )	
				continue;
	
			//WE ONLY NEED TO CHECK DIRECTORIES	
			if ( FileFinder.IsDirectory() == FALSE )
				continue;

			CString strDateFolder = FileFinder.GetFileName();

			CString strDateTran = "";
			CString strDateSale = "";
			if ( StringDateChecker.CheckDate( m_bSaleTime, HISTORYFILE_TYPE_SALES, strDateFolder, strDateTran, strDateSale ) == FALSE )
				continue;

			CString strFolder;
			strFolder.Format ( "%s%s",
				(const char*) strDataPath,
				(const char*) strDateFolder );

			for ( int nTerminalIdx = 0; nTerminalIdx < dbLocation.GetTerminalCount( nLocIdx ); nTerminalIdx++ )
			{
				int nTerminalNo = dbLocation.GetTNo ( nLocIdx, nTerminalIdx );
			
				CString strFilename;
				strFilename.Format ( "%s\\sales.%3.3d", 
					(const char*) strFolder, nTerminalNo );

				CSSFile fileTest;
				if ( fileTest.Open ( strFilename, "rb", _SH_DENYNO ) == FALSE )
					continue;

				CTermFileListInfo infoFile;
				infoFile.m_strFilename = strFilename;
				infoFile.m_strDateFolder = strDateFolder;
				infoFile.m_strDateTran = strDateTran;
				infoFile.m_strDateSale = strDateSale;
				infoFile.m_nLocIdx = nLocIdx;
				arrayFileInfo.Consolidate( infoFile );
						
				nTotalFileSize += fileTest.GetLength();
			}	
		}
	}

#ifdef COMPILE_PMS
	if ( TRUE == bWantPMSSales )
	{
		CEposSelectArray SelectArray;
		SelectArray.SelectLocation( nPMSLocIdx );
		SelectArray.MakeList();

		PMSCheckoutDateArray.BuildCheckoutDateArray();
		CReportConsolidationArray<CSortedIntItem> arrayChecked;
			
		CPMSReportFileFinder PMSReportFileFinder;
		PMSReportFileFinder.SetIncludePendingFlag( FALSE );
		PMSReportFileFinder.SetIncludeActiveFlag( TRUE );
		PMSReportFileFinder.SetIncludeCancelledFlag( FALSE );

		PMSReportFileFinder.SetPMSDateMode( PMS_DATEMODE_ACTUAL, strCheckDateFrom, strCheckDateTo, FALSE );

		PMSReportFileFinder.GetPMSFileListComplete( SelectArray, arrayChecked, nPMSLocIdx, 0, arrayFileInfo, nTotalFileSize );
		PMSReportFileFinder.GetPMSFileListIncomplete( SelectArray, arrayChecked, nPMSLocIdx, 0, arrayFileInfo, nTotalFileSize );
	}
#endif
}

/**********************************************************************/

void CSalesHistoryRegionPlu::Consolidate()
{
	if ( m_bInvalidDeptSort == TRUE )
	{
		m_DepartmentSorter.Build( TRUE );
		m_nDeptSortDbIdx = DataManager.GetActiveDbIdx();
	}

	if ( m_bInvalidFilter == TRUE )	RebuildFilter();
	if ( m_bInvalidTotals == TRUE )	RebuildTotals();
}

/**********************************************************************/

void CSalesHistoryRegionPlu::RebuildFilter()
{
	bool bClearFilter = ( m_bInvalidDeptSort ) || ( m_arrayPluInfo.GetSize() != DataManager.Plu.GetSize() + 1 );
		
	{
		CDateRangeOrRegionPluInfo infoPluItem;
		infoPluItem.m_bShowItem = TRUE;
		infoPluItem.m_strReportText = "Unknown Plu Items";
		
		if ( bClearFilter == TRUE )
		{
			m_arrayPluInfo.RemoveAll();
			m_arrayPluInfo.Add( infoPluItem );
		}
		else
			m_arrayPluInfo.SetAt( 0, infoPluItem );
	}

	StatusProgress.Lock( TRUE, "Filtering PLU items" );
	for ( int nPluIdx = 0; nPluIdx < DataManager.Plu.GetSize(); nPluIdx++ )
	{
		StatusProgress.SetPos( nPluIdx, DataManager.Plu.GetSize() );

		CPluCSVRecord PluRecord;
		DataManager.Plu.GetAt( nPluIdx, PluRecord );			
		__int64 nPluNo = PluRecord.GetPluNoInt();

		bool bExcludeItem = FALSE;

		//FILTER BY PLU ITEM
		if ( m_bItemRange == TRUE )
		{
			if ( ( m_nItemFrom != 0  ) && ( nPluNo < m_nItemFrom ) )
				bExcludeItem = TRUE;
			else if ( ( m_nItemFrom != 0  ) && ( nPluNo > m_nItemTo ) )
				bExcludeItem = TRUE;
		}

		//FILTER BY DEPT NO
		int nDeptNo = PluRecord.GetBaseDeptNo();
		if ( m_DepartmentSet.GetDepartmentStatus ( nDeptNo ) == FALSE )
			bExcludeItem = TRUE;

		//FILTER BY TAX BAND
		CDateRangeOrRegionPluInfo infoPluItem;
		
		bool bGotTaxBand = FALSE;
		for ( int n = 0; n < 10; n++ )
		{
			int nTaxBand = PluRecord.GetModifierTaxBandIntForReport(n);
			infoPluItem.m_nDatabaseTaxBand[n] = nTaxBand;

			if ( ( FALSE == bExcludeItem ) && ( FALSE == bGotTaxBand ) )
				bGotTaxBand = infoPluItem.CheckTaxBand( m_nTaxFilter, nTaxBand );
		}
		
		infoPluItem.m_bShowItem = bGotTaxBand;
		infoPluItem.m_nDeptNo = nDeptNo;
		infoPluItem.m_strDisplayPluNo = ReportHelpers.GetDisplayPluNo( nPluNo );
		infoPluItem.m_strReportText = ReportHelpers.GetReportText( PluRecord );
		infoPluItem.m_dCostPerPlu = DataManager.Plu.GetPluPurchaseCost( PluRecord );
		
		if ( bClearFilter == TRUE )
			m_arrayPluInfo.Add( infoPluItem );
		else
			m_arrayPluInfo.SetAt( nPluIdx + 1, infoPluItem );
	}

	StatusProgress.Unlock();
}

/**********************************************************************/

void CSalesHistoryRegionPlu::RebuildTotals()
{
	//BUILD ARRAY OF DATE RANGES BY COLUMNS
	m_arrayDateRange.RemoveAll();

	CDateRangeInfo dateinfo;
	dateinfo.m_dateStart.SetStatus( COleDateTime::null );
	dateinfo.m_dateEnd.SetStatus( COleDateTime::null );

	for ( int nCol = 0; nCol < m_arrayColumns.GetSize(); nCol++ )
		m_arrayDateRange.Add( dateinfo );

	//BUILD EMPTY TABLE OF QTY AND VALUE
	for ( int p = 0; p < MAX_PLU_PRICES + MAX_SBPLU_PRICES; p++ ) 
	{
		if ( GetCellArray(p) != NULL )
			GetCellArray(p) -> RemoveAll();
	}

	m_nCols = m_arrayColumns.GetSize();
	m_nRows = DataManager.Plu.GetSize() + 1;
	m_nCells = m_nCols * m_nRows;
	
	CSalesHistoryCell cell;
	cell.m_dSalesQty = 0.0;
	cell.m_dSalesValue = 0.0;
	
	for ( int p = 0; p < MAX_PLU_PRICES + MAX_SBPLU_PRICES; p++ )
	{
		if ( ( GetCellArray(p) != NULL ) && ( DataManager.SBPriceText.IsActivePriceBand(p) == TRUE ) )
		{
			for ( int n = 0; n < m_nCells; n++ )
				GetCellArray(p) -> Add( cell );
		}
	}

	int nTotalFileSize;
	CReportConsolidationArray<CTermFileListInfo> arrayFileInfo;
	GetFileListPlu ( arrayFileInfo, nTotalFileSize );
		
	int nProgress = 0;	
	StatusProgress.Lock( TRUE, "Consolidating sales" );

#ifdef COMPILE_PMS
	CPMSReportFileFinder PMSReportFileFinder;
	PMSReportFileFinder.SetPMSDateMode( PMS_DATEMODE_ACTUAL, "", "", FALSE );
#endif
		
	for ( int nFileIdx = 0; nFileIdx < arrayFileInfo.GetSize(); nFileIdx++ )
	{
		CTermFileListInfo infoFile;
		arrayFileInfo.GetAt( nFileIdx, infoFile );
		
		bool bIsPMSLocation = dbLocation.IsPMSPseudoLocation( infoFile.m_nLocIdx );
				
		CSSFile fileSales;
		if ( fileSales.Open ( infoFile.m_strFilename, "rb", _SH_DENYNO ) == FALSE )
			continue;

		int nConnectionType = CONNECTION_TYPE_STANDARD_NONE;
		{
			int nDummy = 0;
			dbLocation.GetLocSPOSVersion( infoFile.m_nLocIdx, nDummy, nConnectionType );
		}

		CString strThisSaleDate = "";
		CString strLastSaleDate = "";
		CString strThisSaleTime = "";
		COleDateTime oleThisSaleDate = COleDateTime( 2000, 1, 1, 0, 0, 0 );
		
		if ( FALSE == bIsPMSLocation )
		{
			strThisSaleDate = infoFile.m_strDateTran;
			strLastSaleDate = infoFile.m_strDateTran;
			oleThisSaleDate = COleDateTime( atoi( strThisSaleDate.Left(4) ), atoi( strThisSaleDate.Mid(4,2) ), atoi( strThisSaleDate.Mid(6,2) ), 0, 0, 0 );
		}

		int nTaxDateCode = DataManager.HistoricalTaxRates.GetDateCode( infoFile.m_strDateTran );

		CSortedDateTimeItem itemFrom = m_DateTimeFilter.GetCheckDateFrom();
		CSortedDateTimeItem itemTo = m_DateTimeFilter.GetCheckDateTo();
		CStringDateChecker StringDateChecker( itemFrom.m_strDateTime, itemTo.m_strDateTime );

		CPluSalesLine csvSales ( infoFile.IsLegacySalesFolder() );

		int nLinesToRead;
		CString strBuffer;
		bool bGotSalesToday = FALSE;
		if ( ::FindFirstPluLine( fileSales, strBuffer, nLinesToRead ) == TRUE )
		{
			bool bPMSInclude = FALSE;

			do
			{
				StatusProgress.SetPos ( nProgress + fileSales.Tell(), nTotalFileSize );

#ifdef COMPILE_PMS
				//EXTRACT DATE AND TIME INFORMATION FROM #PMS_TIME LINE 
				if ( TRUE  == bIsPMSLocation )
				{
					if ( strBuffer.Left(9) == "#PMS_TIME" )
					{
						int nDummy;
						COleDateTime oleDummy;
						PMSReportFileFinder.ProcessPMSTimeLine( strBuffer, nDummy, oleThisSaleDate, strThisSaleDate, strThisSaleTime );
						bPMSInclude = StringDateChecker.CheckDate( strThisSaleDate, oleDummy );
						continue;
					}

					if ( FALSE == bPMSInclude )
						continue;
				}
#endif

				if ( ( strBuffer == "" ) || ( strBuffer.Left(1) == "#" ) )
					continue;

				csvSales.ParseLine( strBuffer );

				//CHECK FOR RELEVANT LINE TYPE
				int nLineType = csvSales.GetLineType();
				switch( nLineType )
				{
				case CSVPLUDATA_PROMO_TAXABLE:
				case CSVPLUDATA_PROMO_NONTAXABLE:
				case CSVPLUDATA_MIXMATCH_NONTAXABLE:
				case CSVPLUDATA_PLU_SALE:
				case CSVPLUDATA_PLU_DISCOUNT:
				case CSVPLUDATA_PLU_MIXMATCH:
					break;

				case CSVPLUDATA_PLU_CHEAPESTFREE:
				default:
					continue;
				}

				if (csvSales.GetPluSaleWastageFlag() == TRUE)
				{
					continue;
				}

				if (csvSales.GetPluSaleVoidFlag() == TRUE)
				{
					continue;
				}

				__int64 nBasePluNo = 0;
				int nModifier = 0;
				double dMultiplier = 1.0;

				switch( nLineType )
				{
				case CSVPLUDATA_PROMO_TAXABLE:
				case CSVPLUDATA_PROMO_NONTAXABLE:
					nBasePluNo = CEposReport::GetSeparatedPromoPluNo( csvSales.GetPromoNoSeparate() );
					break;

				case CSVPLUDATA_MIXMATCH_NONTAXABLE:
					nBasePluNo = CEposReport::GetSeparatedMixMatchPluNo( csvSales.GetMixMatchNo() );
					break;

				case CSVPLUDATA_PLU_SALE:
				case CSVPLUDATA_PLU_DISCOUNT:
				case CSVPLUDATA_PLU_MIXMATCH:
					{
						CSQLPluNoInfo PluNoInfo;
						PluNoInfo.m_nEposPluNo = csvSales.GetPluNoNew();
						::ProcessPluNo( PluNoInfo, FALSE, TRUE );
						if ( FALSE == PluNoInfo.m_bValid )
							continue;

						nBasePluNo = PluNoInfo.m_nBasePluNo;
						nModifier = PluNoInfo.m_nModifier;

						if ( nModifier != 0 )
							dMultiplier = DataManager.Modifier.GetMultiplierForQty ( nModifier );
					}
					break;
				}

				int nPluIdx;
				if ( DataManager.Plu.FindPluByNumber( nBasePluNo, nPluIdx ) == FALSE )
					nPluIdx = -1;

				{
					bool bAccept = TRUE;

					//NORMAL CASHRSP SALES TAKES DATE FROM FILE AND TIME FROM CSV LINE
					if ( FALSE == bIsPMSLocation )
					{
						if ( TRUE == m_bSaleTime )
							strThisSaleTime = csvSales.GetSaleTime();
						else
							strThisSaleTime = csvSales.GetTranTime();
		
						bAccept = ( m_DateTimeFilter.CheckTime( strThisSaleDate, strThisSaleTime ) != 0 );
					}
					//PMS ACTUAL TIME MODE TAKES DATE AND TIME FROM LATEST #PMS_TIME LINE
					else
					{
						bAccept = ( m_DateTimeFilter.CheckTime( strThisSaleDate, strThisSaleTime ) != 0 );
					}

					if ( FALSE == bAccept )
						continue;
				}

				if ( strThisSaleDate != strLastSaleDate )
				{
					nTaxDateCode = DataManager.HistoricalTaxRates.GetDateCode( strThisSaleDate );
					strLastSaleDate = strThisSaleDate;
				}

				nPluIdx++;
				if ( nPluIdx >= m_arrayPluInfo.GetSize() )
					continue;

				int nTaxBand = 0;
				if (DealerFlags.GetUseDatabaseVATBandFlag() == FALSE)
					nTaxBand = csvSales.GetNumericTaxBand();
				else
					nTaxBand = m_arrayPluInfo[nPluIdx].GetDatabaseTaxBand(nModifier);

				if ( CDateRangeOrRegionPluInfo::CheckTaxBand( m_nTaxFilter, nTaxBand ) == FALSE )
					continue;
				
				int nLevel = 0;
				if ( TRUE == m_bPriceBandSplit )
				{
					nLevel = csvSales.GetPriceBand();
					if ( CONNECTION_TYPE_SPTBOOK_NONE != nConnectionType )
					{
						if ( ( nLevel < 0 ) || ( nLevel >= MAX_PLU_PRICES ) )
							nLevel = 0;
					}
					else
					{
						nLevel = DataManager.SBPriceText.GetReportMapPriceBand( nLevel - 1 );
					}
				}

				for ( int nLocMapIdx = 0; nLocMapIdx < m_LocToColumnMap.GetColumnCount( infoFile.m_nLocIdx ); nLocMapIdx++ )
				{
					int nColumnIdx = m_LocToColumnMap.GetColumnIdx( infoFile.m_nLocIdx, nLocMapIdx );
				
					switch ( csvSales.GetLineType() )
					{
					case CSVPLUDATA_PLU_SALE:
						ConsolidateSale( nLevel, nColumnIdx, nPluIdx, csvSales.GetSaleQty() * dMultiplier, csvSales.GetValue() );
						break;

					case CSVPLUDATA_PLU_DISCOUNT:
					case CSVPLUDATA_PLU_MIXMATCH:
					case CSVPLUDATA_PROMO_TAXABLE:
					case CSVPLUDATA_PROMO_NONTAXABLE:
					case CSVPLUDATA_MIXMATCH_NONTAXABLE:
						ConsolidateSale( nLevel, nColumnIdx, nPluIdx, csvSales.GetSaleQty(), -csvSales.GetValue() );
						break;
					}
				
					ProcessDate ( nColumnIdx, oleThisSaleDate );
				}
			}
			while ( ( fileSales.ReadString( strBuffer ) == TRUE ) && ( --nLinesToRead != 0 ) );
		}
		nProgress += fileSales.GetLength();
		fileSales.Close();
	}
	StatusProgress.Unlock();
}

/**********************************************************************/

void CSalesHistoryRegionPlu::ConsolidateSale( int nLevel, int nCol, int nRow, double dQty, double dValue )
{
	if ( nCol < 0 || nCol >= m_nCols )
		return;

	if ( nRow < 0 || nRow >= m_nRows )
		return;

	if ( nLevel < 0 || nLevel >= MAX_PLU_PRICES + MAX_SBPLU_PRICES )
		return;

	if ( GetCellArray( nLevel ) == NULL )
		return;

	int nCell = nCol + ( nRow * m_nCols );

	if ( nCell < GetCellArray(nLevel) -> GetSize() )
	{
		CSalesHistoryCell cell = GetCellArray(nLevel) -> GetAt( nCell );
		cell.m_dSalesQty += dQty;
		cell.m_dSalesValue += dValue;
		GetCellArray(nLevel) -> SetAt( nCell, cell);
	}
}

/**********************************************************************/

void CSalesHistoryRegionPlu::GetQtyValueInfo( int nLevel, int nCol, int nRow, double& dQty, double& dValue )
{
	dQty = 0.0;
	dValue = 0.0;

	if ( nCol < 0 || nCol >= m_nCols )
		return;

	if ( nRow < 0 || nRow >= m_nRows )
		return;

	if ( nLevel < 0 || nLevel >= MAX_PLU_PRICES + MAX_SBPLU_PRICES )
		return;

	if ( GetCellArray( nLevel ) == NULL )
		return;

	int nCell = nCol + ( nRow * m_nCols );

	if ( nCell < GetCellArray(nLevel) -> GetSize() )
	{
		CSalesHistoryCell cell = GetCellArray(nLevel) -> GetAt( nCell );
		dQty = cell.m_dSalesQty;
		dValue = cell.m_dSalesValue;
	}
}

/**********************************************************************/

double CSalesHistoryRegionPlu::GetPercentage( double dValue, double dTotalValue )
{
	if ( dTotalValue == 0.0 )
		return 0.0;
	else
		return ( dValue * 100.0 ) / dTotalValue;
}

/**********************************************************************/

double CSalesHistoryRegionPlu::GetAverage( CCSV& csv, int nValuePos, int nQtyPos )
{
	double dValue = csv.GetDouble( nValuePos );
	double dQty = csv.GetDouble( nQtyPos );

	if ( dQty == 0.0 )
		return 0.0;
	else
		return dValue / dQty;
}

/**********************************************************************/

void CSalesHistoryRegionPlu::AddToCSV ( CCSV& csv, int nPos, double dToAdd, int nDP )
{
	csv.SetAt( nPos, csv.GetDouble( nPos ) + dToAdd, nDP );
}

/**********************************************************************/

bool CSalesHistoryRegionPlu::CreateReport()
{
	switch( m_nReportType )
	{
	case SALESHISTORY_REGION_PLU:		return CreatePluReport();
	case SALESHISTORY_REGION_BAND:		return CreatePluReport();
	case SALESHISTORY_REGION_DEPT:		return CreateDeptReport();
	default:							return FALSE;
	}
}

/**********************************************************************/

int CSalesHistoryRegionPlu::CreateGraph()
{
	switch( m_nReportType )
	{
	case SALESHISTORY_REGION_DEPT:	return CreateDeptGraph();
	default:						return 1;
	}
}

/**********************************************************************/

bool CSalesHistoryRegionPlu::CreateDeptReport()
{
	if ( m_ReportFile.Open ( Super.ReportFilename() ) == FALSE )
		return FALSE;

	m_ReportFile.SetStyle1 ( GetReportTitle ( "Plu Department Sales by Region" ) );
	m_ReportFile.AddColumn ( "Department", TA_LEFT, 300 );

	for ( int nColumnIdx = 0; nColumnIdx < m_arrayColumns.GetSize(); nColumnIdx++ )
	{
		m_ReportFile.AddColumn( GetColumnName( nColumnIdx ), TA_CENTER, 800 );	
		m_ReportFile.AddSubColumn ( nColumnIdx + 1, "Qty", TA_RIGHT, 25 );
		m_ReportFile.AddSubColumn ( nColumnIdx + 1, "Value", TA_RIGHT, 25 );
		m_ReportFile.AddSubColumn ( nColumnIdx + 1, "Ave.", TA_RIGHT, 25 );
		m_ReportFile.AddSubColumn ( nColumnIdx + 1, "%Sale", TA_RIGHT, 25 );
	}

	bool bFirstLine = TRUE;
	bool bIsFiltered = IsFiltered();

	CCSV csvTotal ( '\t', '"', TRUE, FALSE, FALSE );
	csvTotal.Add( "Grand Total" );

	for ( int nCol = 0; nCol < m_arrayColumns.GetSize(); nCol++ )
	{
		csvTotal.Add( 0.0, 5 );
		csvTotal.Add( 0.0, 5 );
		csvTotal.Add( 0.0, 5 );
		csvTotal.Add( "" );
	}

	CStringArray arrayBuffer;

	for ( int n = -1; n <= Dept::DeptNo.Max + 1; n++ )
	{
		if ( 0 == n )
			continue;

		int nDeptNo;
		if ( Dept::DeptNo.Max + 1 == n )
			nDeptNo = 0;
		else
			nDeptNo = n;

		if ( m_DepartmentSorter.GetItemCount( nDeptNo ) == 0 )
			continue;

		//AVOID INCLUDING UNKNOWN ITEMS UNLESS WE HAVE ALL DEPARTMENTS
		if ( -1 == nDeptNo ) 
		{
			if ( TRUE == bIsFiltered )
				continue;
		}
		else if ( m_DepartmentSet.GetDepartmentStatus( nDeptNo ) == FALSE )
			continue;
	
		CCSV csvDept ( '\t', '"', TRUE, FALSE, FALSE );
		csvDept.Add( GetDeptText( nDeptNo ) );

		for ( int nCol = 0; nCol < m_arrayColumns.GetSize(); nCol++ )
		{
			csvDept.Add( 0.0, 5 );
			csvDept.Add( 0.0, 5 );
			csvDept.Add( 0.0, 5 );
			csvDept.Add( 0.0, 2 );
		}

		bool bGotRowData = FALSE;

		for ( int nItem = 0; nItem < m_DepartmentSorter.GetItemCount( nDeptNo ); nItem++ )
		{
			int nPluIdx = m_DepartmentSorter.GetItemIndex( nDeptNo, nItem );

			if ( nDeptNo != -1 )
				nPluIdx++;

			if ( nPluIdx >= m_arrayPluInfo.GetSize() )
				continue;

			CDateRangeOrRegionPluInfo pluInfo = m_arrayPluInfo.GetAt( nPluIdx );
			if (pluInfo.m_bShowItem == FALSE )
				continue;

			for ( int nCol = 0; nCol < m_nCols; nCol++ )
			{
				double dCellSalesQty, dCellSalesValue;
				GetQtyValueInfo( 0, nCol, nPluIdx, dCellSalesQty, dCellSalesValue );

				if ( dCellSalesQty != 0.0 || dCellSalesValue != 0.0 )
					bGotRowData = TRUE;

				int nBase = ( nCol * 4 ) + 1;
				AddToCSV( csvDept, nBase, dCellSalesQty, 5 );
				AddToCSV( csvDept, nBase + 1, dCellSalesValue, 5 );
				AddToCSV( csvTotal, nBase, dCellSalesQty, 5 );
				AddToCSV( csvTotal, nBase + 1, dCellSalesValue, 5 );
			}			
		}

		if ( bGotRowData == TRUE || m_nZeroSkipType == ZEROSKIP_NONE )
			arrayBuffer.Add( csvDept.GetLine() );
	}

	for ( int n = 0; n < arrayBuffer.GetSize(); n++ )
	{
		CCSV csvDept ( arrayBuffer.GetAt(n), '\t' );

		for ( int nCol = 0; nCol < m_nCols; nCol++ )
		{
			int nBase = ( nCol * 4 ) + 1;

			double dDeptValue = csvDept.GetDouble( nBase + 1 );
			double dTotalValue = csvTotal.GetDouble( nBase + 1 );	
			double dPercent = GetPercentage( dDeptValue, dTotalValue );
			double dDeptAverage = GetAverage( csvDept, nBase + 1, nBase ); 
		
			AddToCSV( csvDept, nBase, 0.0, SysInfo.GetDPQty() );
			AddToCSV( csvDept, nBase + 1, 0.0, SysInfo.GetDPValue() );
			AddToCSV( csvDept, nBase + 2, dDeptAverage, SysInfo.GetDPValue() );
			AddToCSV( csvDept, nBase + 3, dPercent, 2 );
		}
		m_ReportFile.WriteLine( csvDept.GetLine() );
	}
	  
	CString strUnderline = "\t";
	for ( int nCol = 0; nCol < m_arrayColumns.GetSize(); nCol++ )
	{
		strUnderline += "<LI>\t<LI>\t<LI>";
		if ( nCol < m_arrayColumns.GetSize() - 1 )	strUnderline += "\t\t";
	}
	m_ReportFile.WriteLine( strUnderline );

	for ( int nCol = 0; nCol < m_arrayColumns.GetSize(); nCol++ )
	{
		int nBase = ( nCol * 4 ) + 1;
		double dTotalAverage = GetAverage( csvTotal, nBase + 1, nBase );
			
		AddToCSV( csvTotal, nBase, 0.0, SysInfo.GetDPQty() );
		AddToCSV( csvTotal, nBase + 1, 0.0, SysInfo.GetDPValue() );
		AddToCSV( csvTotal, nBase + 2, dTotalAverage, SysInfo.GetDPValue() );	
	}

	m_ReportFile.WriteLine ( csvTotal.GetLine() );
	m_ReportFile.WriteLine( "" );
	WriteColumnInfo();
	m_ReportFile.Close();
	return TRUE;
}

/**********************************************************************/

int CSalesHistoryRegionPlu::CreateDeptGraph()
{
	CSSFile fileGraph;
	if ( fileGraph.Open ( Super.ReportFilename(), "wb" ) == FALSE )
		return 1;

	CCSV csvOut;
	csvOut.Add( "Dept No" );
	csvOut.Add( "Sales" );
	csvOut.Add( "Plu Department Sales by Region" );
	csvOut.Add( GetReportTitle( "" ) );
	csvOut.Add( m_arrayColumns.GetSize() );

	for ( int nColumnIdx = 0; nColumnIdx < m_arrayColumns.GetSize(); nColumnIdx++ )
		csvOut.Add ( GetColumnName( nColumnIdx ) );

	fileGraph.WriteLine( csvOut.GetLine() );

	bool bGotGraphData = FALSE;
	bool bIsFiltered = IsFiltered();
	
	for ( int n = -1; n <= Dept::DeptNo.Max + 1; n++ )
	{
		if ( 0 == n )
			continue;

		int nDeptNo;
		if ( Dept::DeptNo.Max + 1 == n )
			nDeptNo = 0;
		else
			nDeptNo = n;

		if ( m_DepartmentSorter.GetItemCount( nDeptNo ) == 0 )
			continue;

		//AVOID INCLUDING UNKNOWN ITEMS UNLESS WE HAVE ALL DEPARTMENTS
		if ( -1 == nDeptNo ) 
		{
			if ( TRUE == bIsFiltered )
				continue;
		}
		else if ( m_DepartmentSet.GetDepartmentStatus( nDeptNo ) == FALSE )
			continue;
	
		CString strDeptNo, strDeptName;

		if ( -1 != nDeptNo )
		{
			strDeptNo.Format ( "%2.2d", nDeptNo );
			strDeptName = DataManager.Department.GetReportTextByDeptNo( nDeptNo );
		}
		else
		{
			strDeptNo = "??";
			strDeptName = "Unknown Items";
		}

		CCSV csvOut;
		csvOut.Add( strDeptNo );
		csvOut.Add( strDeptName );

		for ( int nCol = 0; nCol < m_arrayColumns.GetSize(); nCol++ )
			csvOut.Add( 0.0, 5 );
		
		bool bGotRowData = FALSE;

		for ( int nItem = 0; nItem < m_DepartmentSorter.GetItemCount( nDeptNo ); nItem++ )
		{
			int nPluIdx = m_DepartmentSorter.GetItemIndex( nDeptNo, nItem );

			if ( nDeptNo != -1 )
				nPluIdx++;

			if ( nPluIdx >= m_arrayPluInfo.GetSize() )
				continue;

			CDateRangeOrRegionPluInfo pluInfo = m_arrayPluInfo.GetAt( nPluIdx );
			if (pluInfo.m_bShowItem == FALSE )
				continue;

			for ( int nCol = 0; nCol < m_nCols; nCol++ )
			{
				double dCellSalesQty, dCellSalesValue;
				GetQtyValueInfo( 0, nCol, nPluIdx, dCellSalesQty, dCellSalesValue );

				if ( dCellSalesValue != 0.0 )
					bGotRowData = TRUE;

				AddToCSV( csvOut, nCol + 2, dCellSalesValue, 5 );
			}
		}

		if ( bGotRowData == TRUE || m_nZeroSkipType == ZEROSKIP_NONE )
		{
			for ( int nCol = 0; nCol < m_nCols; nCol++ )
				AddToCSV ( csvOut, nCol + 2, 0.0, SysInfo.GetDPValue() );

			fileGraph.WriteLine( csvOut.GetLine() );
			bGotGraphData = TRUE;
		}
	}
	  
	return ( bGotGraphData ) ? 0 : 2;
}

/**********************************************************************/

bool CSalesHistoryRegionPlu::CreatePluReport()
{
	if ( m_ReportFile.Open ( Super.ReportFilename() ) == FALSE )
		return FALSE;

	m_ReportFile.SetStyle1 ( GetReportTitle ( "Plu Sales by Region" ) );
	m_ReportFile.AddColumn ( "Plu Number", TA_LEFT, 350 );
	m_ReportFile.AddColumn ( "Description", TA_LEFT, 350 );
	m_ReportFile.AddColumn ( "Dept", TA_RIGHT, 200 );
	
	int nBaseColumn = 3;
	if ( TRUE == m_bPriceBandSplit )
	{
		m_ReportFile.AddColumn ( "Level", TA_LEFT, 250 );
		nBaseColumn = 4;
	}

	for ( int nColumnIdx = 0; nColumnIdx < m_arrayColumns.GetSize(); nColumnIdx++ )
	{
		m_ReportFile.AddColumn( GetColumnName( nColumnIdx ), TA_CENTER, 700 );
		m_ReportFile.AddSubColumn ( nColumnIdx + nBaseColumn, "Qty", TA_RIGHT, 35 );
		m_ReportFile.AddSubColumn ( nColumnIdx + nBaseColumn, "Value", TA_RIGHT, 35 );
		m_ReportFile.AddSubColumn ( nColumnIdx + nBaseColumn, "Ave.", TA_RIGHT, 30 );
	}

	CCSV csvTotal ( '\t', '"', TRUE, FALSE, FALSE );
	csvTotal.Add( "Grand Total" );
	
	for ( int x = 0; x < nBaseColumn - 1; x++ )
		csvTotal.Add( "" );
	
	for ( int nCol = 0; nCol < m_arrayColumns.GetSize(); nCol++ )
	{
		csvTotal.Add( 0.0, SysInfo.GetDPQty() );
		csvTotal.Add( 0.0, SysInfo.GetDPValue() );
		csvTotal.Add( 0.0, SysInfo.GetDPValue() );
	}

	bool bFirstLine = TRUE;
	bool bIsFiltered = IsFiltered();

	for ( int n = -1; n <= Dept::DeptNo.Max + 1; n++ )
	{
		if ( 0 == n )
			continue;

		int nDeptNo;
		if ( Dept::DeptNo.Max + 1 == n )
			nDeptNo = 0;
		else
			nDeptNo = n;

		if ( m_DepartmentSorter.GetItemCount( nDeptNo ) == 0 )
			continue;

		//AVOID INCLUDING UNKNOWN ITEMS UNLESS WE HAVE ALL DEPARTMENTS
		if ( -1 == nDeptNo ) 
		{
			if ( TRUE == bIsFiltered )
				continue;
		}
		else if ( m_DepartmentSet.GetDepartmentStatus( nDeptNo ) == FALSE )
			continue;
	
		bool bWantHeader = TRUE;

		CString strTotal;
		strTotal.Format ( "<..>Totals (%s)", GetDeptText( nDeptNo ) );
		
		CCSV csvDept ( '\t', '"', TRUE, FALSE, FALSE );
		csvDept.Add ( strTotal );
		
		for ( int x = 0; x < nBaseColumn - 1; x++ )
			csvDept.Add( "" );
	
		for ( int nCol = 0; nCol < m_arrayColumns.GetSize(); nCol++ )
		{
			csvDept.Add( 0.0, SysInfo.GetDPQty() );
			csvDept.Add( 0.0, SysInfo.GetDPValue() );
			csvDept.Add( 0.0, SysInfo.GetDPValue() );
		}
	
		for ( int nItem = 0; nItem < m_DepartmentSorter.GetItemCount( nDeptNo ); nItem++ )
		{
			int nPluIdx = m_DepartmentSorter.GetItemIndex( nDeptNo, nItem );

			if ( nDeptNo != -1 )
				nPluIdx++;

			if ( nPluIdx >= m_arrayPluInfo.GetSize() )
				continue;

			CDateRangeOrRegionPluInfo pluInfo = m_arrayPluInfo.GetAt( nPluIdx );
			if (pluInfo.m_bShowItem == FALSE )
				continue;

			CPluCSVRecord PluRecord;
			DataManager.Plu.GetAt( nPluIdx, PluRecord );

			int nPriceCount = 1;
			if ( TRUE == m_bPriceBandSplit )
				nPriceCount = MAX_PLU_PRICES + MAX_SBPLU_PRICES;

			int nMinLevels = GetMinLevelsToDisplay( nPluIdx );

			for ( int nLevel = 0; nLevel < nPriceCount; nLevel++ )
			{
				if ( DataManager.SBPriceText.IsActivePriceBand( nLevel ) == FALSE )
					continue;

				CCSV csvOut ( '\t', '"', TRUE, FALSE, FALSE );
				csvOut.Add ( pluInfo.m_strDisplayPluNo );
				csvOut.Add ( pluInfo.m_strReportText );
				csvOut.Add ( pluInfo.m_nDeptNo );

				if ( TRUE == m_bPriceBandSplit )
					csvOut.Add( DataManager.SBPriceText.GetDisplayText( nLevel ) );

				bool bGotRowData = FALSE;

				for ( int nCol = 0; nCol < m_nCols; nCol++ )
				{
					double dCellSalesQty, dCellSalesValue;
					GetQtyValueInfo( nLevel, nCol, nPluIdx, dCellSalesQty, dCellSalesValue );
					double dAverage = ( dCellSalesQty == 0.0 ) ? 0.0 : dCellSalesValue / dCellSalesQty;

					if ( dCellSalesQty != 0.0 || dCellSalesValue != 0.0 )
						bGotRowData = TRUE;
					
					csvOut.Add( dCellSalesQty, SysInfo.GetDPQty() );
					csvOut.Add( dCellSalesValue, SysInfo.GetDPValue() );
					csvOut.Add( dAverage, SysInfo.GetDPValue() );

					int nBase = nBaseColumn + ( nCol * 3 );
					AddToCSV ( csvDept, nBase, dCellSalesQty, 5 );
					AddToCSV ( csvDept, nBase + 1, dCellSalesValue, 5 );
					AddToCSV ( csvTotal, nBase, dCellSalesQty, 5 );
					AddToCSV ( csvTotal, nBase + 1, dCellSalesValue, 5 );				
				}
	 
				if ( ( bGotRowData == TRUE ) || ( nLevel < nMinLevels ) )
				{
					if ( bWantHeader == TRUE )
					{
						if ( bFirstLine == FALSE )
							m_ReportFile.WriteLine ( "" );
						else
							bFirstLine = FALSE;

						CString strLine = "<..>";
						strLine += GetDeptText( nDeptNo );
						
						m_ReportFile.WriteLine ( strLine );
						m_ReportFile.WriteLine ( "<LI>" );

						bWantHeader = FALSE;
					}

					m_ReportFile.WriteLine( csvOut.GetLine() );
				}
			}
		}

		if ( bWantHeader == FALSE )
		{
			CString strUnderline = "\t\t\t";
			if ( TRUE == m_bPriceBandSplit )
				strUnderline += "\t";

			for ( int nCol = 0; nCol < m_arrayColumns.GetSize(); nCol++ )
			{
				strUnderline += "<LI>\t<LI>\t<LI>\t";

				int nBase = nBaseColumn + ( nCol * 3 );

				double dAverage = GetAverage( csvDept, nBase + 1, nBase );

				AddToCSV( csvDept, nBase, 0.0, SysInfo.GetDPQty() );
				AddToCSV( csvDept, nBase + 1, 0.0, SysInfo.GetDPValue() );
				AddToCSV( csvDept, nBase + 2, dAverage, SysInfo.GetDPValue() );
			}

			m_ReportFile.WriteLine( strUnderline );
			m_ReportFile.WriteLine ( csvDept.GetLine() );
			m_ReportFile.WriteLine ( "" );
		}
	}  
	
	CString strUnderline = "\t\t\t";
	if ( TRUE == m_bPriceBandSplit )
		strUnderline += "\t";

	for ( int nCol = 0; nCol < m_arrayColumns.GetSize(); nCol++ )
		strUnderline += "<LI>\t<LI>\t<LI>\t";
	
	m_ReportFile.WriteLine( strUnderline );

	for ( int nCol = 0; nCol < m_arrayColumns.GetSize(); nCol++ )
	{
		int nBase = nBaseColumn + ( nCol * 3 );

		double dAverage = GetAverage( csvTotal, nBase + 1, nBase );

		AddToCSV( csvTotal, nBase, 0.0, SysInfo.GetDPQty() );
		AddToCSV( csvTotal, nBase + 1, 0.0, SysInfo.GetDPValue() );
		AddToCSV( csvTotal, nBase + 2, dAverage, SysInfo.GetDPValue() );
	}
	m_ReportFile.WriteLine ( csvTotal.GetLine() );
	
	m_ReportFile.WriteLine( "" );
	WriteColumnInfo();
	m_ReportFile.Close();
	return TRUE;
}

/**********************************************************************/

void CSalesHistoryRegionPlu::WriteColumnInfo()
{
	for ( int nCol = 0; nCol < m_arrayColumns.GetSize(); nCol++ )
	{
		CString strDateInfo = "(NO DATA)";
		bool bHideNoData = DealerFlags.GetHideNoDataFlag();
		
		COleDateTime dateStart = m_arrayDateRange[nCol].m_dateStart;
		COleDateTime dateEnd = m_arrayDateRange[nCol].m_dateEnd;

		if ( ( dateStart.GetStatus() == COleDateTime::valid ) && ( dateEnd.GetStatus() == COleDateTime::valid ) )
		{
			strDateInfo.Format ( "(%2.2d/%2.2d/%4.4d - %2.2d/%2.2d/%4.4d)",
				dateStart.GetDay(),
				dateStart.GetMonth(),
				dateStart.GetYear(),
				dateEnd.GetDay(),
				dateEnd.GetMonth(),
				dateEnd.GetYear() );

			bHideNoData = FALSE;
		}

		if ( FALSE == bHideNoData )
		{
			CString strLine;
			strLine.Format ( "<..>Ecr sales for %s %s",
				GetColumnName( nCol ),
				(const char*) strDateInfo );

			m_ReportFile.WriteLine( strLine );
		}
	}
}

/**********************************************************************/

int CSalesHistoryRegionPlu::GetMinLevelsToDisplay( int nPluIdx )
{
	switch( m_nZeroSkipType )
	{
	case ZEROSKIP_BAND:
		return 0;

	case ZEROSKIP_NONE:
		if ( FALSE == m_bPriceBandSplit )
			return 1;
		else
			return MAX_PLU_PRICES + MAX_SBPLU_PRICES;

	case ZEROSKIP_ITEM:
		if ( FALSE == m_bPriceBandSplit )
			return 0;
		else
		{
			for ( int nLevel = 0; nLevel < MAX_PLU_PRICES + MAX_SBPLU_PRICES; nLevel++ )
			{
				if ( DataManager.SBPriceText.IsActivePriceBand( nLevel ) == TRUE )
				{
					for ( int nCol = 0; nCol < m_nCols; nCol++ )
					{
						double dQty, dValue;
						GetQtyValueInfo( nLevel, nCol, nPluIdx, dQty, dValue );
						
						if ( dQty != 0.0 || dValue != 0.0 )
							return MAX_PLU_PRICES + MAX_SBPLU_PRICES;
					}
				}
			}
		}
		break;
	}
	
	return 0;
}

/**********************************************************************/

const char* CSalesHistoryRegionPlu::GetDeptText( int nDeptNo )
{
	if ( -1 != nDeptNo )
		m_strDeptText.Format( "D%3.3d %s", nDeptNo, DataManager.Department.GetReportTextByDeptNo( nDeptNo ) );
	else
		m_strDeptText = "Unknown Items";

	return m_strDeptText;
}

/**********************************************************************/

bool CSalesHistoryRegionPlu::IsFiltered()
{
	if ( TRUE == m_bItemRange )
		return TRUE;

	if ( TAXBAND_FILTER_ALL != m_nTaxFilter )
		return TRUE;

	for ( int nDeptNo = 0; nDeptNo <= Dept::DeptNo.Max; nDeptNo++ )
		if ( DataManager.Department.GetReportEnableByDeptNo( nDeptNo ) == TRUE )
			if ( m_DepartmentSet.GetDepartmentStatus( nDeptNo ) == FALSE )
				return TRUE;
	
	return FALSE;
}

/**********************************************************************/

