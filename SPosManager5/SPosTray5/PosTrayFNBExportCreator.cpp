/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "EposReportBase.h"
#include "LocationSelectorEntity.h"
#include "HistoryFileTypes.h"
#include "LocationCSVArray.h"
#include "NetworkCSVArray.h"
#include "RepCSV.h"
#include "StringDateChecker.h"
#include "SysInfo.h"
/**********************************************************************/
#include "PosTrayFNBExportCreator.h"
/**********************************************************************/

CPosTrayFNBExportCreator::CPosTrayFNBExportCreator( CPosTrayFNBScheduleInfo& infoExport, CFNBDatabaseOptions& Options ) : m_infoExport( infoExport ), m_Options( Options )
{
}

/**********************************************************************/

CPosTrayFNBExportCreator::~CPosTrayFNBExportCreator()
{
}

/**********************************************************************/

bool CPosTrayFNBExportCreator::ProcessExport( COleDateTime& dateReport  )
{
	CString strFolder = "";
	strFolder += dbDatabase.GetFolderPathSysset( m_infoExport.m_nDbIdx );
	strFolder += "\\FNB";
	::CreateSubdirectory( strFolder );
	strFolder += "\\EXPORTS";
	::CreateSubdirectory( strFolder );

	m_strCheckDateFrom.Format( "%4.4d%2.2d%2.2d",
		dateReport.GetYear(),
		dateReport.GetMonth(),
		dateReport.GetDay() );

	COleDateTime dateNext = dateReport;
	dateNext += COleDateTimeSpan( 1, 0, 0, 0 );

	m_strCheckDateTo.Format( "%4.4d%2.2d%2.2d",
		dateNext.GetYear(),
		dateNext.GetMonth(),
		dateNext.GetDay() );

	m_strSODTime.Format( "%2.2d%2.2d00",
		EcrmanOptions.GetReportsDayStartHour(),
		EcrmanOptions.GetReportsDayStartMinute() );

	CLocationSelectorLookup Lookup( m_infoExport.m_nDbIdx, m_Options );
	CLocationSelectorEntity LocSelEntity( Lookup );
	LocSelEntity.GetEPOSSelectArray( m_SelectArray );
	m_SelectArray.MakeList();

	int nTotalFileSize = 0;
	CArray<CTermFileListInfo,CTermFileListInfo> arrayFileInfo;
	GetFileList( arrayFileInfo, nTotalFileSize );

	CString strOutputFile;
	strOutputFile.Format( "%s\\%s.dat",
		(const char*) strFolder,
		(const char*) m_strCheckDateFrom );

	CSSFile fileOutput;
	if ( fileOutput.Open( strOutputFile, "wb" ) == FALSE )
		return FALSE;

	CDataManagerInfo info;
	DataManager.OpenDatabaseReadOnly( m_infoExport.m_nDbIdx, info, FALSE );

	for ( int nFileIdx = 0; nFileIdx < arrayFileInfo.GetSize(); nFileIdx++ )
	{
		CTermFileListInfo infoFile = arrayFileInfo.GetAt( nFileIdx );

		CSSFile fileSales;
		if ( fileSales.Open ( infoFile.m_strFilename, "rb", _SH_DENYNO ) == FALSE )
			continue;

		CPluSalesLine csv( infoFile.IsLegacySalesFolder() );

		int nLinesToRead;
		CString strBuffer;
		if ( ::FindFirstPluLine( fileSales, strBuffer, nLinesToRead ) == TRUE )
		{
			do
			{
				csv.ParseLine(strBuffer);

				//FILTER FOR WASTAGE
				if (csv.GetPluSaleWastageFlag() == TRUE)
				{
					continue;
				}

				if (csv.GetPluSaleVoidFlag() == TRUE)
				{
					continue;
				}

				//FILTER FOR DISCOUNTS
				if ( csv.GetLineType() != CSVPLUDATA_PLU_SALE )
					continue;

				//NORMAL CASHRSP SALES TAKES DATE FROM FILE AND TIME FROM CSV LINE
				CString strLineTime = csv.GetTranTime();
				if ( CheckTime( strLineTime, infoFile.m_strDateTran ) == FALSE )
					continue;
				
				double dMultiplier = 1.0;
				__int64 nPluNo = 0;

				CSQLPluNoInfo infoPluNo;
				infoPluNo.m_nEposPluNo = csv.GetPluNoNew();
				::ProcessPluNo( infoPluNo );
					
				if ( FALSE == infoPluNo.m_bValid )
					continue;
		
				dMultiplier = 1.0;
				if ( infoPluNo.m_nModifier != 0 )
					dMultiplier = DataManager.Modifier.GetMultiplierForQty ( infoPluNo.m_nModifier );

				nPluNo = infoPluNo.m_nBasePluNo;
				
				int nPriceBand = csv.GetPriceBand();
				if ( ( nPriceBand < 0 ) || ( nPriceBand >= MAX_PLU_PRICES ) )
					nPriceBand = 0;

				CCSV csvOutput;
				csvOutput.Add( infoPluNo.m_nBasePluNo );
				csvOutput.Add( nPriceBand );
				csvOutput.Add( csv.GetSaleQty() * dMultiplier, SysInfo.GetDPQty() );
				csvOutput.Add( csv.GetValue(), SysInfo.GetDPValue() );
				fileOutput.WriteLine( csvOutput.GetLine() );
			}
			while ( ( fileSales.ReadString( strBuffer ) == TRUE ) && ( --nLinesToRead != 0 ) );
		}

		fileSales.Close();
	}
	
	return TRUE;
}

/**********************************************************************/

void CPosTrayFNBExportCreator::GetFileList ( CArray<CTermFileListInfo,CTermFileListInfo>& arrayFileInfo, int& nTotalFileSize )
{
	CWaitCursor wait;

	arrayFileInfo.RemoveAll();
	nTotalFileSize = 0;

	//BUILD AN ARRAY OF THE CLUMPS OF LOCATIONS IN THE SELECT ARRAY
	int nCurrentLocIdx = -1;
	CWordArray arrayLocGroupStart;
	for ( int nIndex = 0; nIndex < m_SelectArray.GetListSize(); nIndex++ )
	{
		CEposSelectArrayListItem ListItem;
		m_SelectArray.GetListItem( nIndex, ListItem );

		if ( ListItem.m_nLocIdx != nCurrentLocIdx )
		{
			nCurrentLocIdx = ListItem.m_nLocIdx;
			arrayLocGroupStart.Add ( nIndex );
		}
	}
	arrayLocGroupStart.Add ( m_SelectArray.GetListSize() );

	//WORK THROUGH THE CLUMPS OF LOCATIONS TO CREATE THE FILE LIST
	for ( int nIndex = 0; nIndex < arrayLocGroupStart.GetSize() - 1; nIndex++ )
	{
		int nStartPos = arrayLocGroupStart.GetAt( nIndex );
		int nEndPos =	arrayLocGroupStart.GetAt( nIndex + 1 ) - 1;

		CEposSelectArrayListItem ListItemLoc;
		m_SelectArray.GetListItem( nStartPos, ListItemLoc );

		int nNetworkIdx;
		int nNetworkNo = dbLocation.GetNetworkNo( ListItemLoc.m_nLocIdx );
		if ( dbNetwork.FindNetworkByNumber ( nNetworkNo, nNetworkIdx ) == FALSE )
			continue;

		CString strNetworkPath = dbNetwork.GetFolderPathData ( nNetworkIdx );
		strNetworkPath += "\\";

		CStringDateChecker StringDateChecker( m_strCheckDateFrom, m_strCheckDateTo );

		CFileFind FileFinder;	
		bool bWorking = ( FileFinder.FindFile ( strNetworkPath + "*.*" ) != 0 );

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
			if ( StringDateChecker.CheckDate( FALSE, HISTORYFILE_TYPE_SALES, strDateFolder, strDateTran, strDateSale ) == FALSE )
				continue;

			for ( int nPos = nStartPos; nPos <= nEndPos; nPos++ )
			{
				CEposSelectArrayListItem ListItemTerm;
				m_SelectArray.GetListItem( nPos, ListItemTerm );
				int nTerminalNo = dbLocation.GetTNo( ListItemLoc.m_nLocIdx, ListItemTerm.m_nTermIdx );
				
				CString strFilename;
				strFilename.Format ( "%s\\SALES.%3.3d", 
					(const char*) strDateFolder, nTerminalNo );

				CSSFile fileTest;
				if ( fileTest.Open ( strNetworkPath + strFilename, "rb", _SH_DENYNO ) == FALSE )
					continue;

				CTermFileListInfo infoFile;
				infoFile.m_nSelectArrayIdx = nPos;
				infoFile.m_strFilename = strNetworkPath + strFilename;
				infoFile.m_strDateFolder = strDateFolder;
				infoFile.m_strDateTran = strDateTran;
				infoFile.m_strDateSale = strDateSale;
				infoFile.m_nDbIdx = ListItemLoc.m_nDbIdx;
				infoFile.m_nLocIdx = ListItemLoc.m_nLocIdx;
				infoFile.m_nTermIdx = ListItemTerm.m_nTermIdx;
				arrayFileInfo.Add( infoFile );
				
				nTotalFileSize += fileTest.GetLength();
			}
		}
	}
}

/**********************************************************************/

bool CPosTrayFNBExportCreator::CheckTime( CString& strTime, CString& strDate )
{
	if ( strDate == m_strCheckDateFrom )
		return ( strTime >= m_strSODTime );
	else if ( strDate == m_strCheckDateTo )
		return ( strTime < m_strSODTime );
	else
		return FALSE;
}

/**********************************************************************/
