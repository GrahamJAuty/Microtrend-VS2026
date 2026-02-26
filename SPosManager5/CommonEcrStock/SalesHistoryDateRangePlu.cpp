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
#include "ReportpointCSVArray.h"
#include "SalesHistoryFields.h"
#include "StatusProgress.h"
#include "StringDateChecker.h"
#include "SysInfo.h"
/**********************************************************************/
#include "SalesHistoryDateRangePlu.h"
/**********************************************************************/

CSalesHistoryDateRangeCell::CSalesHistoryDateRangeCell()
{
	m_dSalesQty = 0.0;
	m_dSalesValue = 0.0;
	m_dWasteQty = 0.0;
	m_dCost = 0.0;
	m_strExtraData = "";
	m_nTaxBand = 0xFFFF;
	m_nTaxDateCode = 0xFFFF;
}

/**********************************************************************/

CSalesHistoryDateRangePlu::CSalesHistoryDateRangePlu()
{
	m_nZeroSkipType = ZEROSKIP_NONE;
	m_nReportType = SALESHISTORY_DATERANGE_LYCOMPARE;
	m_bInvalidTotals = TRUE;
	m_bInvalidFilter = TRUE;
	m_bInvalidDeptSort = TRUE;
	m_bPriceBandSplit = FALSE;

	m_bItemRange = FALSE;
	m_nItemFrom = 0;
	m_nItemTo = 0;

	m_nDeptFilter = DEPARTMENTSET_ALL;
	m_DepartmentSet.CreateDepartmentSet( m_nDeptFilter );
	
	m_nTaxFilter = TAXBAND_FILTER_ALL;

	m_nDeptSortDbIdx = -1;

	m_bSaleTime = FALSE;

	m_RegionInfo.m_nDbIdx = -1;
	m_RegionInfo.m_nTableIdx = -1;
	m_RegionInfo.m_nType = -1;
	
	m_DepartmentSet.CreateDepartmentSet( DEPARTMENTSET_ALL );

	for ( int n = 0; n < MAX_PLU_PRICES + MAX_SBPLU_PRICES; n++ )
		m_pArrayCells[n] = new CArray<CSalesHistoryDateRangeCell,CSalesHistoryDateRangeCell>;
}

/**********************************************************************/

CSalesHistoryDateRangePlu::~CSalesHistoryDateRangePlu()
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

CArray<CSalesHistoryDateRangeCell,CSalesHistoryDateRangeCell>* CSalesHistoryDateRangePlu::GetCellArray( int n )
{
	if ( ( n >= 0 ) && ( n < MAX_PLU_PRICES + MAX_SBPLU_PRICES ) )
	{
		if ( m_pArrayCells[n] != NULL )
			return (CArray<CSalesHistoryDateRangeCell,CSalesHistoryDateRangeCell>*) m_pArrayCells[n];
	}
		
	return NULL;
}

/**********************************************************************/

void CSalesHistoryDateRangePlu::Configure( CDateRangeOrRegionColumnInfo& RegionInfo, CArray<CDateRangeInfo,CDateRangeInfo>& DateInfo, int nDeptFilter, int nTaxFilter, bool bItemRange, __int64 nItemFrom, __int64 nItemTo, bool bPriceBandSplit, CSessionCSVRecord& Session )
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
		
		COleDateTime dateNow = COleDateTime::GetCurrentTime();
		m_DateTimeFilter.SetDates( dateNow, dateNow, Session );

		if ( strLastID != m_DateTimeFilter.GetSessionID() )
			m_bInvalidTotals = TRUE;
	}

	if ( m_RegionInfo.m_nDbIdx != RegionInfo.m_nDbIdx )
	{
		m_RegionInfo.m_nDbIdx = RegionInfo.m_nDbIdx;
		m_bInvalidDeptSort = TRUE;
		m_bInvalidTotals = TRUE;
		m_bInvalidFilter = TRUE;
	}

	if ( m_RegionInfo.m_nTableIdx != RegionInfo.m_nTableIdx )
	{
		m_RegionInfo.m_nTableIdx = RegionInfo.m_nTableIdx;
		m_bInvalidTotals = TRUE;
	}

	if ( m_RegionInfo.m_nType != RegionInfo.m_nType )
	{
		m_RegionInfo.m_nType = RegionInfo.m_nType;
		m_bInvalidTotals = TRUE;
	}

	bool bSameDates = TRUE;

	if ( m_arrayDateRange.GetSize() != DateInfo.GetSize() )
		bSameDates = FALSE;
	else
	{
		for ( int n = 0; n < m_arrayDateRange.GetSize(); n++ )
		{
			CDateRangeInfo infoOld = m_arrayDateRange.GetAt(n);
			CDateRangeInfo infoNew = DateInfo.GetAt(n);

			if ( infoOld.m_dateStart != infoNew.m_dateStart )
			{
				bSameDates = FALSE;
				break;
			}

			if ( infoOld.m_dateEnd != infoNew.m_dateEnd )
			{
				bSameDates = FALSE;
				break;
			}

			//OK TO COPY NAME IF DATE RANGE IS THE SAME
			if ( infoOld.m_strName != infoNew.m_strName )
			{
				infoOld.m_strName = infoNew.m_strName;
				m_arrayDateRange.SetAt( n, infoOld );
			}
		}
	}

	if ( bSameDates == FALSE )
	{
		m_arrayDateRange.RemoveAll();
		m_bInvalidTotals = TRUE;

		for ( int n = 0; n < DateInfo.GetSize(); n++ )
		{
			CDateRangeInfo info = DateInfo.GetAt(n);
			m_arrayDateRange.Add( info );
		}
	}

	CDataManagerInfo info;
	DataManager.OpenDatabaseReadOnly ( m_RegionInfo.m_nDbIdx, info, FALSE );

	if ( m_RegionInfo.m_nDbIdx != m_nDeptSortDbIdx )
		m_bInvalidDeptSort = TRUE;

	int nIndex = m_RegionInfo.m_nTableIdx;
	switch ( m_RegionInfo.m_nType )
	{
	case NODE_LOCATION:
		{
			CLocationSelectorEntity LocSelEntity;
			LocSelEntity.SetLocationNode( nIndex );
			LocSelEntity.GetEPOSSelectArray( m_SelectArray );
			m_strColumnName = dbLocation.GetName( nIndex );
		}
		break;

	case NODE_LOCATIONSET:
		{
			CLocationSelectorEntity LocSelEntity;
			LocSelEntity.SetLocationSetNode( nIndex );
			LocSelEntity.GetEPOSSelectArray( m_SelectArray );
			m_strColumnName = dbLocationSet.GetName( nIndex );
		}
		break;
	
	case NODE_DATABASE:
		{
			CLocationSelectorEntity LocSelEntity;
			LocSelEntity.SetDatabaseNode( nIndex );
			LocSelEntity.GetEPOSSelectArray( m_SelectArray );
			m_strColumnName = dbDatabase.GetName( nIndex );
		}
		break;

	case NODE_REPORTPOINT:
		{
			CLocationSelectorEntity LocSelEntity;
			LocSelEntity.SetReportpointNode( nIndex );
			LocSelEntity.GetEPOSSelectArray( m_SelectArray );
			m_strColumnName = dbReportpoint.GetName( nIndex );
		}
		break;
	}
	m_SelectArray.MakeList();
}

/**********************************************************************/

const char* CSalesHistoryDateRangePlu::GetReportTitle( const char* szReportName )
{
	COleDateTime dateWeekStart = m_arrayDateRange[0].m_dateStart;

	m_strTitle.Format ( "%s (%s%s, %s)",
		 szReportName,
		 (const char*) m_strColumnName,
		 m_DepartmentSet.GetFilterName( TRUE, TRUE ),
		 m_DateTimeFilter.GetSessionName() );
	
	return m_strTitle;
}

/**********************************************************************/

void CSalesHistoryDateRangePlu::GetFileListPlu ( CReportConsolidationArray<CTermFileListInfo>& arrayFileInfo, int& nTotalFileSize )
{
	CWaitCursor wait;

	arrayFileInfo.RemoveAll();
	
	nTotalFileSize = 0;

	//BUILD AN ARRAY OF THE CLUMPS OF LOCATIONS IN THE SELECT ARRAY
	CWordArray arrayLocGroupStart;
	{
		int nSelIdx;
		int nCurrentLocIdx = -1;
		for ( nSelIdx = 0; nSelIdx < m_SelectArray.GetListSize(); nSelIdx++ )
		{
			CEposSelectArrayListItem ListItem;
			m_SelectArray.GetListItem( nSelIdx, ListItem );

			if ( ListItem.m_nLineType != EPOSSELECT_TERMINAL )
				break;

			if ( ListItem.m_nLocIdx != nCurrentLocIdx )
			{
				nCurrentLocIdx = ListItem.m_nLocIdx;
				arrayLocGroupStart.Add ( nSelIdx );
			}
		}
		arrayLocGroupStart.Add ( nSelIdx );
	}

	bool bWantPMSSales = FALSE;
	int nPMSLocIdx = 0;

	for ( int nRange = 0; nRange < m_arrayDateRange.GetSize(); nRange++ )
	{
		{
			COleDateTime oleDateFrom = m_arrayDateRange[nRange].m_dateStart;
			COleDateTime oleDateTo = m_arrayDateRange[nRange].m_dateEnd;

			CSessionCSVRecord DummySession;
			m_DateTimeFilter.SetDates( oleDateFrom, oleDateTo, DummySession, FALSE );
		}

		CSortedDateTimeItem CheckDateFrom = m_DateTimeFilter.GetCheckDateFrom();
		CSortedDateTimeItem CheckDateTo = m_DateTimeFilter.GetCheckDateTo();

		//WORK THROUGH THE CLUMPS OF LOCATIONS TO CREATE THE FILE LIST
		for ( int nIndex = 0; nIndex < arrayLocGroupStart.GetSize() - 1; nIndex++ )
		{
			int nStartPos = arrayLocGroupStart.GetAt( nIndex );
			int nEndPos =	arrayLocGroupStart.GetAt( nIndex + 1 ) - 1;

			CEposSelectArrayListItem ListItemLoc;
			m_SelectArray.GetListItem( nStartPos, ListItemLoc );
			
			int nLocIdx = ListItemLoc.m_nLocIdx;

			if ( SysInfo.IsPMSSystem() == TRUE )
			{
				if ( dbLocation.IsPMSPseudoLocation( nLocIdx ) == TRUE )
				{
					if ( FALSE == bWantPMSSales )
					{
						bWantPMSSales = TRUE;
						nPMSLocIdx = nLocIdx;	
					}
					continue;
				}
			}			

			int nNetworkIdx;
			int nNetworkNo = dbLocation.GetNetworkNo( nLocIdx );
			if ( dbNetwork.FindNetworkByNumber ( nNetworkNo, nNetworkIdx ) == FALSE )
				continue;

			CString strDataPath = dbNetwork.GetFolderPathData ( nNetworkIdx );
			strDataPath += "\\";

			CStringDateChecker StringDateChecker( CheckDateFrom.m_strDateTime, CheckDateTo.m_strDateTime );
		
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

				for ( int nPos = nStartPos; nPos <= nEndPos; nPos++ )
				{
					CEposSelectArrayListItem ListItemTerm;
					m_SelectArray.GetListItem( nPos, ListItemTerm );
					int nTerminalNo = dbLocation.GetTNo ( ListItemLoc.m_nLocIdx, ListItemTerm.m_nTermIdx );

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
					infoFile.m_nRange = nRange;
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

			CTermFileListInfo infoList;
			infoList.m_nRange = nRange;
			
			CPMSReportFileFinder PMSReportFileFinder;
			PMSReportFileFinder.SetIncludePendingFlag( FALSE );
			PMSReportFileFinder.SetIncludeActiveFlag( TRUE );
			PMSReportFileFinder.SetIncludeCancelledFlag( FALSE );

			PMSReportFileFinder.SetDefaultListInfo( infoList );
			PMSReportFileFinder.SetPMSDateMode( PMS_DATEMODE_ACTUAL, CheckDateFrom.m_strDateTime, CheckDateTo.m_strDateTime, FALSE );

			PMSReportFileFinder.GetPMSFileListComplete( SelectArray, arrayChecked, nPMSLocIdx, 0, arrayFileInfo, nTotalFileSize );
			PMSReportFileFinder.GetPMSFileListIncomplete( SelectArray, arrayChecked, nPMSLocIdx, 0, arrayFileInfo, nTotalFileSize );
		}
#endif
	}
}

/**********************************************************************/

void CSalesHistoryDateRangePlu::RebuildFilter()
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
			if ( ( m_nItemFrom != 0 ) && ( nPluNo < m_nItemFrom ) )
				bExcludeItem = TRUE;
			if ( ( m_nItemTo != 0 ) && ( nPluNo > m_nItemTo ) )
				bExcludeItem = TRUE;
		}

		//FILTER BY DEPT NO
		int nDeptNo = PluRecord.GetBaseDeptNo();
		if ( m_DepartmentSet.GetDepartmentStatus ( nDeptNo ) == FALSE )
			bExcludeItem = TRUE;

		CDateRangeOrRegionPluInfo infoPluItem;
		
		//FILTER BY TAX BAND
		bool bGotTaxBand = FALSE;
		for ( int nMod = 0; nMod < 10; nMod++ )
		{
			int nTaxBand = PluRecord.GetModifierTaxBandIntForReport(nMod);
			infoPluItem.m_nDatabaseTaxBand[nMod] = nTaxBand;

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

void CSalesHistoryDateRangePlu::Consolidate()
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

void CSalesHistoryDateRangePlu::RebuildTotals()
{
	//BUILD EMPTY TABLE OF QTY AND VALUE
	for ( int p = 0; p < MAX_PLU_PRICES + MAX_SBPLU_PRICES; p++ ) 
	{
		if ( GetCellArray(p) != NULL )
			GetCellArray(p) -> RemoveAll();
	}
		
	m_nCols = m_arrayDateRange.GetSize();
	m_nRows = DataManager.Plu.GetSize() + 1;
	m_nCells = m_nCols * m_nRows;

	CSalesHistoryDateRangeCell cell;
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

#ifdef COMPILE_PMS
	CPMSReportFileFinder PMSReportFileFinder;
	PMSReportFileFinder.SetPMSDateMode( PMS_DATEMODE_ACTUAL, "", "", FALSE );
#endif

	if ( DealerFlags.GetReportCostPriceFlag() == TRUE )
		m_ReportCostFinder.Init();
	
	int nProgress = 0;
	StatusProgress.Lock( TRUE, "Consolidating sales" );
	for ( int nFileIdx = 0; nFileIdx < arrayFileInfo.GetSize(); nFileIdx++ )
	{
		CTermFileListInfo infoFile;
		arrayFileInfo.GetAt(nFileIdx, infoFile);

		{
			COleDateTime oleDateFrom = m_arrayDateRange[infoFile.m_nRange].m_dateStart;
			COleDateTime oleDateTo = m_arrayDateRange[infoFile.m_nRange].m_dateEnd;
			
			CSessionCSVRecord DummySession;
			m_DateTimeFilter.SetDates( oleDateFrom, oleDateTo, DummySession, FALSE );
		}

		CSortedDateTimeItem CheckDateFrom = m_DateTimeFilter.GetCheckDateFrom();
		CSortedDateTimeItem CheckDateTo = m_DateTimeFilter.GetCheckDateTo();

		bool bIsPMSLocation = dbLocation.IsPMSPseudoLocation( infoFile.m_nLocIdx );
	
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

		CString strDateCheckFrom;
		strDateCheckFrom.Format( "%4.4d%2.2d%2.2d",
			CheckDateFrom.m_oleDateTime.GetYear(),
			CheckDateFrom.m_oleDateTime.GetMonth(),
			CheckDateFrom.m_oleDateTime.GetDay() );

		CString strDateCheckTo;
		strDateCheckTo.Format( "%4.4d%2.2d%2.2d",
			CheckDateTo.m_oleDateTime.GetYear(),
			CheckDateTo.m_oleDateTime.GetMonth(),
			CheckDateTo.m_oleDateTime.GetDay() );
	
		CSSFile fileSales;
		if ( fileSales.Open ( infoFile.m_strFilename, "rb", _SH_DENYNO ) == FALSE )
			continue;

		CPluSalesLine csvSales( infoFile.IsLegacySalesFolder() );

		int nConnectionType = CONNECTION_TYPE_STANDARD_NONE;
		{
			int nDummy = 0;
			dbLocation.GetLocSPOSVersion( infoFile.m_nLocIdx, nDummy, nConnectionType );
		}

		CStringDateChecker StringDateChecker( strDateCheckFrom, strDateCheckTo );

		int nLinesToRead;
		CString strBuffer;
		bool bGotSalesToday = FALSE;
		if ( ::FindFirstPluLine( fileSales, strBuffer, nLinesToRead ) == TRUE )
		{
			bool bPMSInclude = FALSE;

			do
			{
				StatusProgress.SetPos ( nProgress + fileSales.Tell(), nTotalFileSize );

				//EXTRACT DATE AND TIME INFORMATION FROM #PMS_TIME LINE 
#ifdef COMPILE_PMS
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
						CSQLPluNoInfo infoPluNo;
						infoPluNo.m_nEposPluNo = csvSales.GetPluNoNew();
						::ProcessPluNo( infoPluNo, FALSE, TRUE );
					
						if ( FALSE == infoPluNo.m_bValid )
							continue;

						nBasePluNo = infoPluNo.m_nBasePluNo;
						nModifier = infoPluNo.m_nModifier;

						if ( nModifier != 0 )
							dMultiplier = DataManager.Modifier.GetMultiplierForQty ( nModifier );
					}
					break;
				}

				int nPluIdx;
				if ( DataManager.Plu.FindPluByNumber( nBasePluNo, nPluIdx ) == FALSE )
					nPluIdx = -1;

				nPluIdx++;
				if ( nPluIdx >= m_arrayPluInfo.GetSize() )
					continue;

				{
					//NORMAL CASHRSP SALES TAKES DATE FROM FILE AND TIME FROM CSV LINE
					if ( FALSE == bIsPMSLocation )
					{
						if ( TRUE == m_bSaleTime )
							strThisSaleTime = csvSales.GetSaleTime();
						else
							strThisSaleTime = csvSales.GetTranTime();
					}

					if ( m_DateTimeFilter.CheckTime( strThisSaleDate, strThisSaleTime ) == FALSE )
						continue;
				}

				if ( strThisSaleDate != strLastSaleDate )
				{
					nTaxDateCode = DataManager.HistoricalTaxRates.GetDateCode( strThisSaleDate );
					strLastSaleDate = strThisSaleDate;
				}

				double dTotalQty = csvSales.GetSaleQty() * dMultiplier;

				int nLineLevel = 0;
				if ( TRUE == m_bPriceBandSplit )
				{
					nLineLevel = csvSales.GetPriceBand();
					if ( CONNECTION_TYPE_SPTBOOK_NONE != nConnectionType )
					{
						if ( ( nLineLevel < 0 ) || ( nLineLevel >= MAX_PLU_PRICES ) )
							nLineLevel = 0;
					}
					else
					{
						nLineLevel = DataManager.SBPriceText.GetReportMapPriceBand( nLineLevel - 1 );
					}
				}

				int nTaxBand = 0;
				if (DealerFlags.GetUseDatabaseVATBandFlag() == FALSE)
					nTaxBand = csvSales.GetNumericTaxBand();
				else
					nTaxBand = m_arrayPluInfo[nPluIdx].GetDatabaseTaxBand(nModifier);

				if ( CDateRangeOrRegionPluInfo::CheckTaxBand( m_nTaxFilter, nTaxBand ) == TRUE )
				{
					switch( csvSales.GetLineType() )
					{
					case CSVPLUDATA_PLU_SALE:
						ConsolidateSale( infoFile.m_nLocIdx, nLineLevel, infoFile.m_nRange, nPluIdx, nTaxDateCode, nTaxBand, dTotalQty, csvSales.GetValue(), 0.0 );
						break;

					case CSVPLUDATA_PLU_DISCOUNT:
					case CSVPLUDATA_PLU_MIXMATCH:
					case CSVPLUDATA_PROMO_TAXABLE:
					case CSVPLUDATA_PROMO_NONTAXABLE:
					case CSVPLUDATA_MIXMATCH_NONTAXABLE:
						ConsolidateSale( infoFile.m_nLocIdx, nLineLevel, infoFile.m_nRange, nPluIdx, nTaxDateCode, nTaxBand, csvSales.GetSaleQty(), -csvSales.GetValue(), 0.0 );
						break;
					}
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

void CSalesHistoryDateRangePlu::ConsolidateSale( int nLocIdx, int nLevel, int nCol, int nRow, int nTaxDateCode, int nTaxBand, double dSalesQty, double dSalesValue, double dWasteQty )
{
	if ( nCol < 0 || nCol >= m_nCols )
		return;

	if ( nRow < 0 || nRow >= m_nRows )
		return;

	if ( ( nLevel < 0 ) || ( nLevel >= MAX_PLU_PRICES + MAX_SBPLU_PRICES ) )
		return;

	int nCell = nCol + ( nRow * m_nCols );
	if ( nCell >= GetCellArray(nLevel) -> GetSize() )
		return;

	CSalesHistoryDateRangeCell cell = GetCellArray(nLevel) -> GetAt( nCell );

	bool bMatchBase = FALSE;
	if ( ( nTaxDateCode == cell.m_nTaxDateCode ) && ( nTaxBand == cell.m_nTaxBand  ) )
		bMatchBase = TRUE;
	else if ( ( 0xFFFF == cell.m_nTaxDateCode ) && ( 0xFFFF == cell.m_nTaxBand  ) )
		bMatchBase = TRUE;

	double dCost = 0.0;
	if ( DealerFlags.GetReportCostPriceFlag() == TRUE )
		dCost = m_ReportCostFinder.GetPluCostByPluIdx( nRow - 1, nLocIdx ) * ( dSalesQty + dWasteQty );

	if ( TRUE == bMatchBase  )
	{
		cell.m_nTaxDateCode = nTaxDateCode;
		cell.m_nTaxBand = nTaxBand;
		cell.m_dSalesQty += dSalesQty;
		cell.m_dSalesValue += dSalesValue;
		cell.m_dWasteQty += dWasteQty;
		cell.m_dCost += dCost;
	}
	else
	{
		CCSV csv( cell.m_strExtraData );
		
		bool bFound = FALSE;
		
		int nSize = csv.GetSize();
		for ( int nPos = 0; nPos < csv.GetSize(); nPos += 6 )
		{
			if  ( ( csv.GetInt(nPos) == nTaxDateCode ) && ( csv.GetInt(nPos + 1 ) == nTaxBand ) )
			{
				csv.SetAt( nPos + 2, csv.GetDouble( nPos + 2 ) + dSalesQty, 7 );
				csv.SetAt( nPos + 3, csv.GetDouble( nPos + 3 ) + dSalesValue, 7 );
				csv.SetAt( nPos + 4, csv.GetDouble( nPos + 4 ) + dWasteQty, 7 );
				
				if ( DealerFlags.GetReportCostPriceFlag() == TRUE )
					csv.SetAt( nPos + 5, csv.GetDouble( nPos + 5 ) + dCost, 7 );
				
				bFound = TRUE;
				break;
			}
		}

		if ( FALSE == bFound )
		{
			csv.Add( nTaxDateCode );
			csv.Add( nTaxBand );
			csv.Add( dSalesQty, 7 );
			csv.Add( dSalesValue, 7 );
			csv.Add( dWasteQty, 7 );
			csv.Add( dCost, 7 );
		}

		cell.m_strExtraData = csv.GetLine();
	}

	GetCellArray(nLevel) -> SetAt( nCell, cell);
}

/**********************************************************************/

void CSalesHistoryDateRangePlu::GetQtyValueInfo( int nLevel, int nCol, int nRow, double& dSalesQty, double& dSalesValue, double& dWasteQty, double &dCost, double& dCellTax )
{
	dSalesQty = 0.0;
	dSalesValue = 0.0;
	dWasteQty = 0.0;
	dCellTax = 0.0;
	dCost = 0.0;

	if ( nCol < 0 || nCol >= m_nCols )
		return;

	if ( nRow < 0 || nRow >= m_nRows )
		return;

	if ( ( nLevel < 0 ) || ( nLevel >= MAX_PLU_PRICES + MAX_SBPLU_PRICES ) )
		return;

	int nCell = nCol + ( nRow * m_nCols );
	if ( nCell >= GetCellArray(nLevel) -> GetSize() )
		return;

	CSalesHistoryDateRangeCell cell = GetCellArray(nLevel) -> GetAt( nCell );
	
	dSalesQty = cell.m_dSalesQty;
	dSalesValue = cell.m_dSalesValue;
	dWasteQty = cell.m_dWasteQty;
	dCost = cell.m_dCost;
	
	{
		int nTaxDateCode = cell.m_nTaxDateCode;
		if ( 0xFFFF != nTaxDateCode )
		{
			double dTaxRate = DataManager.HistoricalTaxRates.GetTaxRate( nTaxDateCode, cell.m_nTaxBand );
			dCellTax = dSalesValue - ReportHelpers.CalcNonTax( dSalesValue, dTaxRate );
		}
	}

	if ( "" != cell.m_strExtraData )
	{
		CCSV csv( cell.m_strExtraData );

		for ( int x = 0; x < csv.GetSize(); x += 6 )
		{
			int nTaxDateCode = csv.GetInt(x);
			int nTaxBand = csv.GetInt( x + 1 );
		
			double dQty = csv.GetDouble(x+2);
			double dVal = csv.GetDouble(x+3);
			double dWst = csv.GetDouble(x+4);
			double dCst = csv.GetDouble(x+5);

			dSalesQty += dQty;
			dSalesValue += dVal;
			dWasteQty += dWst;
			dCost += dCst;

			double dTaxRate = DataManager.HistoricalTaxRates.GetTaxRate( nTaxDateCode, nTaxBand );
			dCellTax += dVal - ReportHelpers.CalcNonTax( dVal, dTaxRate );
		}
	}
}

/**********************************************************************/

double CSalesHistoryDateRangePlu::GetPercentage( double dValue, double dTotalValue )
{
	if ( dTotalValue == 0.0 )
		return 0.0;
	else
		return ( dValue * 100.0 ) / dTotalValue;
}

/**********************************************************************/

void CSalesHistoryDateRangePlu::AddToCSV ( CCSV& csv, int nPos, double dToAdd, int nDP )
{
	csv.SetAt( nPos, csv.GetDouble( nPos ) + dToAdd, nDP );
}

/**********************************************************************/

bool CSalesHistoryDateRangePlu::CreateReport()
{
	switch( m_nReportType )
	{
	case SALESHISTORY_DATERANGE_LYCOMPARE:		return CreateLYCompareReport();
	case SALESHISTORY_DATERANGE_TRADING_DEPT:	return CreateTradingDeptReport();
	case SALESHISTORY_DATERANGE_TRADING_PLU:	return CreateTradingPluReport();
	case SALESHISTORY_DATERANGE_TRADING_BAND:	return CreateTradingPluReport();
	default:									return FALSE;
	}
}

/**********************************************************************/

int CSalesHistoryDateRangePlu::CreateGraph()
{
	switch( m_nReportType )
	{
	case SALESHISTORY_DATERANGE_TRADING_DEPT:	return CreateTradingDeptGraph();
	default:								return 1;
	}
}

/**********************************************************************/

bool CSalesHistoryDateRangePlu::CreateLYCompareReport()
{
	if ( m_ReportFile.Open ( Super.ReportFilename() ) == FALSE )
		return FALSE;

	m_ReportFile.SetStyle1 ( GetReportTitle ( "Plu Sales LY Comparison" ) );
	
	m_ReportFile.AddColumn ( "Department", TA_LEFT, 300 );

	int nDateBlocks = m_arrayDateRange.GetSize() / 2;

	for ( int n = 0; n < nDateBlocks; n++ )
	{
		m_ReportFile.AddColumn( m_arrayDateRange[n*2].m_strName, TA_CENTER, 1100 );

		m_ReportFile.AddSubColumn ( n + 1, "Value", TA_RIGHT, 20 );
		m_ReportFile.AddSubColumn ( n + 1, "%Sale", TA_RIGHT, 20 );
		m_ReportFile.AddSubColumn ( n + 1, "LY %Sale", TA_RIGHT, 20 );
		m_ReportFile.AddSubColumn ( n + 1, "GP", TA_RIGHT, 20 );
		m_ReportFile.AddSubColumn ( n + 1, "GP%", TA_RIGHT, 20 );
	}

	bool bFirstLine = TRUE;
	
	CCSV csvTotal ( '\t', '"', TRUE, FALSE, FALSE );
	csvTotal.Add( "Grand Total" );

	for ( int n = 0; n < nDateBlocks; n++ )
	{
		csvTotal.Add( 0.0, 5 );	//SALES VALUE
		csvTotal.Add( "" );		//SALES %
		csvTotal.Add( "" );		//SALES % LY
		csvTotal.Add( 0.0, 5 );	//GP AMOUNT
		csvTotal.Add( 0.0, 2 );	//GP %
		csvTotal.Add( 0.0, 5 );	//SALES COST (NOT DISPLAYED)
		csvTotal.Add( 0.0, 5 );	//SALES TAX (NOT DISPLAYED)
		csvTotal.Add( 0.0, 5 );	//SALES VALUE LY (NOT DISPLAYED)
	}

	CStringArray arrayBuffer;

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

		CCSV csvDept ( '\t', '"', TRUE, FALSE, FALSE );
		csvDept.Add( GetDeptText( nDeptNo ) );

		for ( int n = 0; n < nDateBlocks; n++ )
		{
			csvDept.Add( 0.0, 5 );	//SALES VALUE
			csvDept.Add( 0.0, 2 );	//SALES %
			csvDept.Add( 0.0, 2 );	//SALES % LY
			csvDept.Add( 0.0, 5 );	//GP AMOUNT
			csvDept.Add( 0.0, 2 );	//GP %
			csvDept.Add( 0.0, 5 );	//SALES COST (NOT DISPLAYED)
			csvDept.Add( 0.0, 5 );	//SALES TAX (NOT DISPLAYED)
			csvDept.Add( 0.0, 5 );	//SALES VALUE LY (NOT DISPLAYED)
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

			double dCostPerPlu = pluInfo.m_dCostPerPlu;
			
			double dCellSalesQty, dCellSalesValue, dCellWasteQty, dCellCost, dCellTax;

			for ( int n = 0; n < nDateBlocks; n++ )
			{
				int nBaseCol = n * 8;

				//MAIN DATE RANGE FOR BLOCK
				GetQtyValueInfo ( 0, n * 2, nPluIdx, dCellSalesQty, dCellSalesValue, dCellWasteQty, dCellCost, dCellTax );
				if ( dCellSalesQty != 0.0 || dCellSalesValue != 0.0 || dCellWasteQty != 0.0 || dCellCost != 0.0 )
				{
					bGotRowData = TRUE;

					if ( DealerFlags.GetReportCostPriceFlag() == FALSE )
						dCellCost = ( dCellSalesQty + dCellWasteQty ) * dCostPerPlu;

					AddToCSV( csvDept, nBaseCol + 6, dCellCost, 5 );
					AddToCSV( csvTotal, nBaseCol + 6, dCellCost, 5 );

					AddToCSV( csvDept, nBaseCol + 7, dCellTax, 5 );
					AddToCSV( csvTotal, nBaseCol + 7, dCellTax, 5 );

					AddToCSV( csvDept, nBaseCol + 1, dCellSalesValue, 5 );
					AddToCSV( csvTotal, nBaseCol + 1, dCellSalesValue, 5 );
				}
			
				//SAME DATE RANGE LAST YEAR
				GetQtyValueInfo( 0, ( n * 2 ) + 1, nPluIdx, dCellSalesQty, dCellSalesValue, dCellWasteQty, dCellCost, dCellTax );
				if ( dCellSalesValue != 0.0 )
				{
					bGotRowData = TRUE;
					AddToCSV( csvDept, nBaseCol + 8, dCellSalesValue, 5 );
					AddToCSV( csvTotal, nBaseCol + 8, dCellSalesValue, 5 );
				}
			}
		}

		if ( bGotRowData == TRUE || m_nZeroSkipType == ZEROSKIP_NONE )
			arrayBuffer.Add( csvDept.GetLine() );
	}

	for ( int l = 0; l < arrayBuffer.GetSize(); l++ )
	{
		CCSV csvDept ( arrayBuffer.GetAt(l), '\t' );

		for ( int n = nDateBlocks - 1; n >= 0; n-- )
		{
			int nBaseCol = n * 8;

			double dDeptValue = csvDept.GetDouble( nBaseCol + 1);
			double dTotalValue = csvTotal.GetDouble( nBaseCol + 1);
			double dLYDeptValue = csvDept.GetDouble( nBaseCol + 8 );
			double dLYTotalValue = csvTotal.GetDouble( nBaseCol + 8 );
		
			double dPercent = GetPercentage( dDeptValue, dTotalValue );
			double dLYPercent = GetPercentage( dLYDeptValue, dLYTotalValue );
		
			double dDeptCost = csvDept.GetDouble( nBaseCol + 6 );
			double dDeptTax = csvDept.GetDouble( nBaseCol + 7 );
		
			double dGP = dDeptValue - dDeptCost - dDeptTax;
			double dGPPercent = ReportHelpers.CalcGPPercent( dDeptValue - dDeptTax, 0.0, dDeptCost );

			AddToCSV( csvDept, nBaseCol + 1, 0.0, SysInfo.GetDPValue() );
			AddToCSV( csvDept, nBaseCol + 2, dPercent, 2 );
			AddToCSV( csvDept, nBaseCol + 3, dLYPercent, 2 );
			AddToCSV( csvDept, nBaseCol + 4, dGP, SysInfo.GetDPValue() );
			AddToCSV( csvDept, nBaseCol + 5, dGPPercent, 2 );

			csvDept.RemoveAt( nBaseCol + 8 );
			csvDept.RemoveAt( nBaseCol + 7 );
			csvDept.RemoveAt( nBaseCol + 6 );
		}
			
		m_ReportFile.WriteLine( csvDept.GetLine() );
	}
	  
	CString strUnderline = "\t";
	for ( int n = 0; n < nDateBlocks; n++ )
		strUnderline += "<LI>\t\t\t<LI>\t\t";
	m_ReportFile.WriteLine( strUnderline );

	for ( int n = nDateBlocks - 1; n >= 0; n-- )
	{
		int nBaseCol = n * 8;

		double dTotalValue = csvTotal.GetDouble( nBaseCol + 1);
		double dLYTotalValue = csvTotal.GetDouble( nBaseCol + 8 );
		
		double dTotalCost = csvTotal.GetDouble( nBaseCol + 6 );
		double dTotalTax = csvTotal.GetDouble( nBaseCol + 7 );
		
		double dGP = dTotalValue - dTotalCost - dTotalTax;
		double dGPPercent = ReportHelpers.CalcGPPercent( dTotalValue - dTotalTax, 0.0, dTotalCost );

		AddToCSV( csvTotal, nBaseCol + 1, 0.0, SysInfo.GetDPValue() );
		AddToCSV( csvTotal, nBaseCol + 4, dGP, SysInfo.GetDPValue() );
		AddToCSV( csvTotal, nBaseCol + 5, dGPPercent, 2 );

		csvTotal.RemoveAt( nBaseCol + 8 );
		csvTotal.RemoveAt( nBaseCol + 7 );
		csvTotal.RemoveAt( nBaseCol + 6 );
	}
		
	m_ReportFile.WriteLine ( csvTotal.GetLine() );
	m_ReportFile.WriteLine( "" );
	
	m_ReportFile.Close();
	return TRUE;
}

/**********************************************************************/

bool CSalesHistoryDateRangePlu::CreateTradingDeptReport()
{
	if ( m_ReportFile.Open ( Super.ReportFilename() ) == FALSE )
		return FALSE;

	m_ReportFile.SetStyle1 ( GetReportTitle ( "Plu Department Sales by Date Range" ) );
	m_ReportFile.AddColumn ( "Department", TA_LEFT, 300 );

	int nDateBlocks = m_arrayDateRange.GetSize();

	for ( int n = 0; n < nDateBlocks; n++ )
	{
		m_ReportFile.AddColumn( m_arrayDateRange[n].m_strName, TA_CENTER, 880 );

		m_ReportFile.AddSubColumn ( n + 1, "Qty", TA_RIGHT, 25 );
		m_ReportFile.AddSubColumn ( n + 1, "Value", TA_RIGHT, 25 );
		m_ReportFile.AddSubColumn ( n + 1, "GP", TA_RIGHT, 25 );
		m_ReportFile.AddSubColumn ( n + 1, "GP%", TA_RIGHT, 25 );
	}

	bool bFirstLine = TRUE;
	
	CCSV csvTotal ( '\t', '"', TRUE, FALSE, FALSE );
	csvTotal.Add( "Grand Total" );

	for ( int n = 0; n < nDateBlocks; n++ )
	{
		csvTotal.Add( 0.0, 5 );	//SALES QTY
		csvTotal.Add( 0.0, 5 );	//SALES VALUE
		csvTotal.Add( 0.0, 5 );	//GP AMOUNT
		csvTotal.Add( 0.0, 2 );	//GP %
		csvTotal.Add( 0.0, 5 );	//SALES COST (NOT DISPLAYED)
		csvTotal.Add( 0.0, 5 );	//SALES TAX (NOT DISPLAYED)
	}

	CStringArray arrayBuffer;

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

		CCSV csvDept ( '\t', '"', TRUE, FALSE, FALSE );
		csvDept.Add( GetDeptText( nDeptNo ) );
		
		for ( int n = 0; n < nDateBlocks; n++ )
		{
			csvDept.Add( 0.0, 5 );	//SALES QTY
			csvDept.Add( 0.0, 5 );	//SALES VALUE
			csvDept.Add( 0.0, 5 );	//GP AMOUNT
			csvDept.Add( 0.0, 2 );	//GP %
			csvDept.Add( 0.0, 5 );	//SALES COST (NOT DISPLAYED)
			csvDept.Add( 0.0, 5 );	//SALES TAX (NOT DISPLAYED)
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

			double dCostPerPlu = pluInfo.m_dCostPerPlu;
			
			double dCellSalesQty, dCellSalesValue, dCellWasteQty, dCellCost, dCellTax;

			for ( int n = 0; n < nDateBlocks; n++ )
			{
				int nBaseCol = n * 6;

				//MAIN DATE RANGE FOR BLOCK
				GetQtyValueInfo ( 0, n, nPluIdx, dCellSalesQty, dCellSalesValue, dCellWasteQty, dCellCost, dCellTax );
				if ( dCellSalesQty != 0.0 || dCellSalesValue != 0.0 || dCellWasteQty != 0.0)
				{
					bGotRowData = TRUE;

					if ( DealerFlags.GetReportCostPriceFlag() == FALSE )
						dCellCost = ( dCellSalesQty + dCellWasteQty ) * dCostPerPlu;

					AddToCSV( csvDept, nBaseCol + 5, dCellCost, 5 );
					AddToCSV( csvTotal, nBaseCol + 5, dCellCost, 5 );

					AddToCSV( csvDept, nBaseCol + 6, dCellTax, 5 );
					AddToCSV( csvTotal, nBaseCol + 6, dCellTax, 5 );

					AddToCSV( csvDept, nBaseCol + 1, dCellSalesQty, 5 );
					AddToCSV( csvTotal, nBaseCol + 1, dCellSalesQty, 5 );

					AddToCSV( csvDept, nBaseCol + 2, dCellSalesValue, 5 );
					AddToCSV( csvTotal, nBaseCol + 2, dCellSalesValue, 5 );
				}
			}
		}

		if ( bGotRowData == TRUE || m_nZeroSkipType == ZEROSKIP_NONE )
		{
			for ( int n = nDateBlocks - 1; n >= 0; n-- )
			{
				int nBaseCol = n * 6;

				double dDeptValue = csvDept.GetDouble( nBaseCol + 2);
				double dDeptCost = csvDept.GetDouble( nBaseCol + 5 );
				double dDeptTax = csvDept.GetDouble( nBaseCol + 6 );
		
				double dGP = dDeptValue - dDeptCost - dDeptTax;
				double dGPPercent = ReportHelpers.CalcGPPercent( dDeptValue - dDeptTax, 0.0, dDeptCost );

				AddToCSV( csvDept, nBaseCol + 1, 0.0, SysInfo.GetDPQty() );
				AddToCSV( csvDept, nBaseCol + 2, 0.0, SysInfo.GetDPValue() );
				AddToCSV( csvDept, nBaseCol + 3, dGP, SysInfo.GetDPValue() );
				AddToCSV( csvDept, nBaseCol + 4, dGPPercent, 2 );

				csvDept.RemoveAt( nBaseCol + 6 );
				csvDept.RemoveAt( nBaseCol + 5 );
			}
			
			m_ReportFile.WriteLine( csvDept.GetLine() );
		}
	}
	  
	CString strUnderline = "\t";
	for ( int n = 0; n < nDateBlocks; n++ )
		strUnderline += "<LI>\t<LI>\t<LI>\t\t";
	m_ReportFile.WriteLine( strUnderline );

	for ( int n = nDateBlocks - 1; n >= 0; n-- )
	{
		int nBaseCol = n * 6;

		double dTotalValue = csvTotal.GetDouble( nBaseCol + 2);
		double dTotalCost = csvTotal.GetDouble( nBaseCol + 5 );
		double dTotalTax = csvTotal.GetDouble( nBaseCol + 6 );
		
		double dGP = dTotalValue - dTotalCost - dTotalTax;
		double dGPPercent = ReportHelpers.CalcGPPercent( dTotalValue - dTotalTax, 0.0, dTotalCost );

		AddToCSV( csvTotal, nBaseCol + 1, 0.0, SysInfo.GetDPQty() );
		AddToCSV( csvTotal, nBaseCol + 2, 0.0, SysInfo.GetDPValue() );
		AddToCSV( csvTotal, nBaseCol + 3, dGP, SysInfo.GetDPValue() );
		AddToCSV( csvTotal, nBaseCol + 4, dGPPercent, 2 );

		csvTotal.RemoveAt( nBaseCol + 6 );
		csvTotal.RemoveAt( nBaseCol + 5 );
	}
		
	m_ReportFile.WriteLine ( csvTotal.GetLine() );
	m_ReportFile.WriteLine( "" );
	
	m_ReportFile.Close();

	return TRUE;
}

/**********************************************************************/

int CSalesHistoryDateRangePlu::CreateTradingDeptGraph()
{
	CSSFile fileGraph;
	if ( fileGraph.Open ( Super.ReportFilename(), "wb" ) == FALSE )
		return 1;

	CCSV csvOut;
	csvOut.Add( "Dept No" );
	csvOut.Add( "Sales" );
	csvOut.Add( "Plu Department Sales by Date Range" );
	csvOut.Add( GetReportTitle( "" ) );
	csvOut.Add( m_arrayDateRange.GetSize() );

	for ( int nColumnIdx = 0; nColumnIdx < m_arrayDateRange.GetSize(); nColumnIdx++ )
		csvOut.Add ( m_arrayDateRange[nColumnIdx].m_strName );

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

		for ( int nCol = 0; nCol < m_arrayDateRange.GetSize(); nCol++ )
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
				double dCellSalesQty, dCellSalesValue, dCellWasteQty, dDummyCost, dDummyTax;
				GetQtyValueInfo( 0, nCol, nPluIdx, dCellSalesQty, dCellSalesValue, dCellWasteQty, dDummyCost, dDummyTax );

				if ( dCellSalesValue != 0.0 )
					bGotRowData = TRUE;

				AddToCSV( csvOut, nCol + 2, dCellSalesValue, 5 );
			}
		}

		if ( bGotRowData == TRUE || m_nZeroSkipType == ZEROSKIP_NONE )
		{
			for ( int nCol = 0; nCol < m_nCols; nCol++ )
				AddToCSV( csvOut, nCol + 2, 0.0, SysInfo.GetDPValue() );

			fileGraph.WriteLine( csvOut.GetLine() );
			bGotGraphData = TRUE;
		}
	}
	  
	return ( bGotGraphData ) ? 0 : 2;
}

/**********************************************************************/

bool CSalesHistoryDateRangePlu::CreateTradingPluReport()
{
	if ( m_ReportFile.Open ( Super.ReportFilename() ) == FALSE )
		return FALSE;

	int nDateBlocks = m_arrayDateRange.GetSize();

	m_ReportFile.SetStyle1 ( GetReportTitle ( "Plu Sales by Date Range" ) );
	m_ReportFile.AddColumn ( "Plu Number", TA_LEFT, 350 );
	m_ReportFile.AddColumn ( "Description", TA_LEFT, 350 );
	m_ReportFile.AddColumn ( "Dept", TA_RIGHT, 200 );

	int nBaseColumn = 3;
	if ( TRUE == m_bPriceBandSplit )
	{
		m_ReportFile.AddColumn ( "Level", TA_LEFT, 250 );
		nBaseColumn = 4;
	}

	for ( int n = 0; n < nDateBlocks; n++ )
	{
		m_ReportFile.AddColumn( m_arrayDateRange[n].m_strName, TA_CENTER, 880 );
		m_ReportFile.AddSubColumn ( n + nBaseColumn, "Qty", TA_RIGHT, 25 );
		m_ReportFile.AddSubColumn ( n + nBaseColumn, "Value", TA_RIGHT, 25 );
		m_ReportFile.AddSubColumn ( n + nBaseColumn, "GP", TA_RIGHT, 25 );
		m_ReportFile.AddSubColumn ( n + nBaseColumn, "GP%", TA_RIGHT, 25 );
	}

	CCSV csvTotal ( '\t', '"', TRUE, FALSE, FALSE );
	csvTotal.Add( "Grand Total" );
	
	for ( int x = 0; x < nBaseColumn - 1; x++ )
		csvTotal.Add( "" );
	
	for ( int nCol = 0; nCol < nDateBlocks; nCol++ )
	{
		csvTotal.Add( 0.0, SysInfo.GetDPQty() );	//SALES QTY
		csvTotal.Add( 0.0, SysInfo.GetDPValue() );	//SALES VALUE
		csvTotal.Add( 0.0, SysInfo.GetDPValue() );	//GP AMOUNT
		csvTotal.Add( 0.0, 2 );	//GP %
		csvTotal.Add( 0.0, 5 );	//SALES COST (NOT DISPLAYED)
		csvTotal.Add( 0.0, 5 );	//SALES TAX (NOT DISPLAYED)
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
		csvDept.Add( strTotal );
	
		for ( int x = 0; x < nBaseColumn - 1; x++ )
			csvDept.Add( "" );
	
		for ( int n = 0; n < nDateBlocks; n++ )
		{
			csvDept.Add( 0.0, SysInfo.GetDPQty() );	//SALES QTY
			csvDept.Add( 0.0, SysInfo.GetDPValue() );	//SALES VALUE
			csvDept.Add( 0.0, SysInfo.GetDPValue() );	//GP AMOUNT
			csvDept.Add( 0.0, 2 );	//GP %
			csvDept.Add( 0.0, 5 );	//SALES COST (NOT DISPLAYED)
			csvDept.Add( 0.0, 5 );	//SALES TAX (NOT DISPLAYED)
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

				for ( int n = 0; n < nDateBlocks; n++ )
				{
					int nBaseCol = ( n * 6 ) + nBaseColumn - 1;

					double dCellSalesQty, dCellSalesValue, dCellWasteQty, dCellCost, dCellTax;
					GetQtyValueInfo( nLevel, n, nPluIdx, dCellSalesQty, dCellSalesValue, dCellWasteQty, dCellCost, dCellTax );
			
					if ( dCellSalesQty != 0.0 || dCellSalesValue != 0.0 )
						bGotRowData = TRUE;
				
					double dCostPerPlu = pluInfo.m_dCostPerPlu;
					
					if ( DealerFlags.GetReportCostPriceFlag() == FALSE )
						dCellCost = ( dCellSalesQty + dCellWasteQty ) * dCostPerPlu;

					AddToCSV( csvDept, nBaseCol + 5, dCellCost, 5 );
					AddToCSV( csvTotal, nBaseCol + 5, dCellCost, 5 );

					AddToCSV( csvDept, nBaseCol + 6, dCellTax, 5 );
					AddToCSV( csvTotal, nBaseCol + 6, dCellTax, 5 );

					AddToCSV( csvDept, nBaseCol + 1, dCellSalesQty, 5 );
					AddToCSV( csvTotal, nBaseCol + 1, dCellSalesQty, 5 );

					AddToCSV( csvDept, nBaseCol + 2, dCellSalesValue, 5 );
					AddToCSV( csvTotal, nBaseCol + 2, dCellSalesValue, 5 );
				
					double dGP = dCellSalesValue - dCellCost - dCellTax;
					double dGPPercent = ReportHelpers.CalcGPPercent( dCellSalesValue - dCellTax, 0.0, dCellCost );

					csvOut.Add( dCellSalesQty, SysInfo.GetDPQty() );
					csvOut.Add( dCellSalesValue, SysInfo.GetDPValue() );
					csvOut.Add( dGP, SysInfo.GetDPValue() );
					csvOut.Add( dGPPercent, 2 );
				}
 
				if ( bGotRowData == TRUE || nLevel < nMinLevels )
				{
					if ( bWantHeader == TRUE )
					{
						if ( bFirstLine == FALSE )
							m_ReportFile.WriteLine ( "" );
						else
							bFirstLine = FALSE;

						CString strLine = "<..>";
						strLine += GetDeptText( nDeptNo );
						m_ReportFile.WriteLine( strLine );
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

			for ( int n = nDateBlocks - 1; n >= 0; n-- )
			{
				int nBaseCol = ( n * 6 ) + nBaseColumn - 1;

				strUnderline += "<LI>\t<LI>\t<LI>\t\t";
	
				double dDeptValue = csvDept.GetDouble( nBaseCol + 2);
				double dDeptCost = csvDept.GetDouble( nBaseCol + 5 );
				double dDeptTax = csvDept.GetDouble( nBaseCol + 6 );
		
				double dGP = dDeptValue - dDeptCost - dDeptTax;
				double dGPPercent = ReportHelpers.CalcGPPercent( dDeptValue - dDeptTax, 0.0, dDeptCost );

				AddToCSV( csvDept, nBaseCol + 1, 0.0, SysInfo.GetDPQty() );
				AddToCSV( csvDept, nBaseCol + 2, 0.0, SysInfo.GetDPValue() );
				AddToCSV( csvDept, nBaseCol + 3, dGP, SysInfo.GetDPValue() );
				AddToCSV( csvDept, nBaseCol + 4, dGPPercent, 2 );

				csvDept.RemoveAt( nBaseCol + 6 );
				csvDept.RemoveAt( nBaseCol + 5 );
			}

			m_ReportFile.WriteLine( strUnderline );
			m_ReportFile.WriteLine ( csvDept.GetLine() );
			m_ReportFile.WriteLine ( "" );
		}
	}
		
	CString strUnderline = "\t\t\t";
	if ( TRUE == m_bPriceBandSplit )
		strUnderline += "\t";

	for ( int n = 0; n < nDateBlocks; n++ )
		strUnderline += "<LI>\t<LI>\t<LI>\t\t";

	m_ReportFile.WriteLine( strUnderline );

	for ( int n = nDateBlocks - 1; n >= 0; n-- )
	{
		int nBaseCol = ( n * 6 ) + nBaseColumn - 1;

		double dTotalValue = csvTotal.GetDouble( nBaseCol + 2);
		double dTotalCost = csvTotal.GetDouble( nBaseCol + 5 );
		double dTotalTax = csvTotal.GetDouble( nBaseCol + 6 );
		
		double dGP = dTotalValue - dTotalCost - dTotalTax;
		double dGPPercent = ReportHelpers.CalcGPPercent( dTotalValue - dTotalTax, 0.0, dTotalCost );

		AddToCSV( csvTotal, nBaseCol + 1, 0.0, SysInfo.GetDPQty() );
		AddToCSV( csvTotal, nBaseCol + 2, 0.0, SysInfo.GetDPValue() );
		AddToCSV( csvTotal, nBaseCol + 3, dGP, SysInfo.GetDPValue() );
		AddToCSV( csvTotal, nBaseCol + 4, dGPPercent, 2 );

		csvTotal.RemoveAt( nBaseCol + 6 );
		csvTotal.RemoveAt( nBaseCol + 5 );
	}
		
	m_ReportFile.WriteLine ( csvTotal.GetLine() );
	m_ReportFile.WriteLine( "" );
		
	m_ReportFile.Close();
	
	return TRUE;
}

/**********************************************************************/

int CSalesHistoryDateRangePlu::GetMinLevelsToDisplay( int nPluIdx )
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
						double dQty, dValue, dWaste, dDummyCost, dDummyTax;
						GetQtyValueInfo( nLevel, nCol, nPluIdx, dQty, dValue, dWaste, dDummyCost, dDummyTax );
						
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

const char* CSalesHistoryDateRangePlu::GetDeptText( int nDeptNo )
{
	if ( -1 != nDeptNo )
		m_strDeptText.Format( "D%3.3d %s", nDeptNo, DataManager.Department.GetReportTextByDeptNo( nDeptNo ) );
	else
		m_strDeptText = "Unknown Items";

	return m_strDeptText;
}

/**********************************************************************/

bool CSalesHistoryDateRangePlu::IsFiltered()
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
