/**********************************************************************/
#include "CashRSPImporter.h"
#include "DateRangeHelpers.h"
#include "EposReportBase.h"
#include "FileRemover.h"
#include "HistoryFileTypes.h"
#include "IDraughtExportManager.h"
#include "IDraughtOptions.h"
#include "LocationCSVArray.h"
#include "NetworkCSVArray.h"
#include "ReportPluInfoFinder.h"
#include "StatusProgress.h"
#include "StringDateChecker.h"
/**********************************************************************/
#include "IDraughtExportCreator.h"
/**********************************************************************/

CIDraughtExportLineInfo::CIDraughtExportLineInfo()
{
	Reset();
}

/**********************************************************************/

void CIDraughtExportLineInfo::Reset()
{
	m_nHour = 0;
	m_nPluNo = 0;
	m_dQty = 0.0;
}

/**********************************************************************/

void CIDraughtExportLineInfo::Add ( CIDraughtExportLineInfo& source )
{
	m_dQty += source.m_dQty;
}

/**********************************************************************/

int CIDraughtExportLineInfo::Compare( CIDraughtExportLineInfo& source, int nHint )
{
	if ( m_nHour != source.m_nHour )
		return ( m_nHour > source.m_nHour ? 1 : -1 );

	if ( m_nPluNo != source.m_nPluNo )
		return ( m_nPluNo > source.m_nPluNo ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CIDraughtExportCreator::CIDraughtExportCreator( COleDateTime& date )
{
	m_strReportFilename = "";

	m_dateFrom = COleDateTime ( date.GetYear(), date.GetMonth(), date.GetDay(), 0, 0, 0 );

	m_strCheckDateFrom.Format( "%4.4d%2.2d%2.2d",
		m_dateFrom.GetYear(),
		m_dateFrom.GetMonth(),
		m_dateFrom.GetDay() );

	if ( ( EcrmanOptions.GetReportsDayStartHour() == 0 ) && ( EcrmanOptions.GetReportsDayStartMinute() == 0 ) )
	{
		m_dateTo = m_dateFrom;
		m_strCheckDateTo = m_strCheckDateFrom;
	}
	else
	{
		m_dateTo = m_dateFrom;
		
		CDateRangeHelpers DateRangeHelpers;
		DateRangeHelpers.AddDaysToTime( m_dateTo, 1 );

		m_strCheckDateTo.Format( "%4.4d%2.2d%2.2d",
			m_dateTo.GetYear(),
			m_dateTo.GetMonth(),
			m_dateTo.GetDay() );
	}

	IDraughtOptions.ResetDepartmentFilters();
}

/**********************************************************************/

int CIDraughtExportCreator::CreateExport()
{
	CString strSentFile;
	strSentFile.Format( "IDraught\\Sent\\%s_N%3.3d.csv",
		(const char*) m_strCheckDateFrom,
		dbNetwork.GetNetworkNo( IDraughtExportManager.GetNetworkIdx() ) );
	GetDataProgramPath( strSentFile );

	if ( ::FileExists( strSentFile ) == TRUE )
	{
		IDraughtExportManager.SetExtraErrorInfo( strSentFile.Right(17) );
		return IDRAUGHT_EXPORT_ERROR_ALREADY_SENT;
	}

	CFilenameUpdater FnUp( SysFiles::IDraughtTempReport );
	m_strReportFilename = FnUp.GetFilenameToUse();

	if ( ::FileExists( m_strReportFilename ) == TRUE )
		CFileRemover FileRemover( m_strReportFilename );

	if ( ::FileExists( m_strReportFilename ) == TRUE )
	{
		IDraughtExportManager.SetExtraErrorInfo( m_strReportFilename );
		return IDRAUGHT_EXPORT_ERROR_DESTFILE_DELETE;
	}

	int nResult = GetFileList();
	if ( IDRAUGHT_EXPORT_ERROR_NONE != nResult )
		return nResult;

	if ( m_arrayFiles.GetSize() == 0 )
		return IDRAUGHT_EXPORT_ERROR_NONE;

	CString strDate;
	strDate.Format( "%2.2d/%2.2d/%4.4d",
		m_dateFrom.GetDay(),
		m_dateFrom.GetMonth(),
		m_dateFrom.GetYear() );

	m_nProgress = 0;

	StatusProgress.Lock( TRUE, "Creating i Draught export for" );
	StatusProgress.SetDialogText( strDate );
	nResult = CreateExportInternal();
	StatusProgress.Unlock();

	return nResult;
}

/**********************************************************************/

int CIDraughtExportCreator::CreateExportInternal()
{
	int nCurrentLocIdx = -1;
	int nCurrentDbIdx = -1;

	for ( int nIdx = 0; nIdx < m_arrayFiles.GetSize(); nIdx++ )
	{
		CIDraughtTermFileInfo infoFile = m_arrayFiles.GetAt( nIdx );

		if ( nCurrentDbIdx != infoFile.m_nDbIdx )
		{
			CDataManagerInfo infoDb;
			if ( DataManager.OpenDatabaseReadOnly( infoFile.m_nDbIdx, infoDb ) == FALSE )
			{
				CString str;
				str.Format( "%3.3d", dbDatabase.GetDbNo( infoFile.m_nDbIdx ) );
				IDraughtExportManager.SetExtraErrorInfo( str );
				return IDRAUGHT_EXPORT_ERROR_DATABASE_OPEN;
			}

			nCurrentDbIdx = infoFile.m_nDbIdx;
		}

		if ( nCurrentLocIdx != infoFile.m_nLocIdx )
		{
			int nResult = WriteSales( nCurrentLocIdx );
			if ( nResult != 0 )
				return nResult;

			m_arraySales.RemoveAll();
			nCurrentLocIdx = infoFile.m_nLocIdx;
		}

		int nResult = ConsolidateSales( infoFile );
		if ( nResult != 0 )
			return nResult;
	}

	if ( nCurrentLocIdx != -1 )
	{
		int nResult = WriteSales( nCurrentLocIdx );
		if ( nResult != 0 )
			return nResult;
	}

	CString strDestFile;
	strDestFile.Format( "IDraught\\Pending\\%s_N%3.3d.csv",
		(const char*) m_strCheckDateFrom,
		dbNetwork.GetNetworkNo( IDraughtExportManager.GetNetworkIdx() ) );
	GetDataProgramPath( strDestFile );

	if ( rename( m_strReportFilename, strDestFile ) != 0 )
	{
		IDraughtExportManager.SetExtraErrorInfo( strDestFile.Right(17) );
		return IDRAUGHT_EXPORT_ERROR_DESTFILE_RENAME;
	}

	return IDRAUGHT_EXPORT_ERROR_NONE;
}

/**********************************************************************/

int CIDraughtExportCreator::GetFileList()
{
	m_arrayFiles.RemoveAll();
	m_nTotalFileSize = 0;

	int nNetworkIdx = IDraughtExportManager.GetNetworkIdx();
	CCSV csvLocations ( IDraughtExportManager.GetLocationList() );

	CString strNoDeptsEnabled = "";
	::AddLeading( strNoDeptsEnabled, Dept::DeptNo.Max + 1, '0' );

	for ( int nIdx = 0; nIdx < csvLocations.GetSize(); nIdx += 2 )
	{
		int nDbIdx = csvLocations.GetInt( nIdx );
		int nLocIdx = csvLocations.GetInt( nIdx + 1 );
		
		int nTermCount = dbLocation.GetTerminalCount( nLocIdx );

		CIDraughtDeptFilter infoFilter;
		IDraughtOptions.GetDatabaseDepartmentFilter( nDbIdx, infoFilter );

		if ( TRUE == infoFilter.m_bInvalid )
		{
			CString strError;
			strError.Format ( "%3.3d", infoFilter.m_nErrorInfo );
			IDraughtExportManager.SetExtraErrorInfo( strError );
			return IDRAUGHT_EXPORT_ERROR_DEPT_ENABLE;
		}

		CString strDataPath = dbNetwork.GetFolderPathData ( nNetworkIdx );
		strDataPath += "\\";

		CStringDateChecker StringDateChecker( m_strCheckDateFrom, m_strCheckDateTo );

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
			if ( StringDateChecker.CheckDate( FALSE, HISTORYFILE_TYPE_SALES, strDateFolder, strDateTran, strDateSale ) == FALSE )
				continue;
	
			//FIND SALES FILES FOR THIS LOCATION
			for ( int nTermIdx = 0; nTermIdx < nTermCount; nTermIdx++ )
			{
				CString strSalesFile;
				strSalesFile.Format( "%s\\%s\\sales.%3.3d",
					dbNetwork.GetFolderPathData( nNetworkIdx ),
					(const char*) strDateFolder,
					dbLocation.GetTNo( nLocIdx, nTermIdx ) );

				if ( ::FileExists( strSalesFile ) == TRUE )
				{
					CSSFile fileTest;
					if ( fileTest.Open( strSalesFile, "rb" ) == FALSE )
					{
						IDraughtExportManager.SetExtraErrorInfo( strSalesFile );
						return IDRAUGHT_EXPORT_ERROR_SOURCEFILE_OPEN;
					}

					m_nTotalFileSize += fileTest.GetLength();

					CIDraughtTermFileInfo info;
					info.m_nDbIdx = nDbIdx;
					info.m_nNetworkIdx = nNetworkIdx;
					info.m_nLocIdx = nLocIdx;
					info.m_bStartDate = ( m_strCheckDateFrom == strDateTran );
					info.m_strFilename = strSalesFile;
					info.m_bLegacySales = ( strDateFolder.GetLength() == 8 );
					m_arrayFiles.Add( info );
				}
			}
		}
	}

	return IDRAUGHT_EXPORT_ERROR_NONE;
}

/**********************************************************************/

int CIDraughtExportCreator::ConsolidateSales( CIDraughtTermFileInfo& infoFile )
{
	CSSFile fileSales;
	if ( fileSales.Open( infoFile.m_strFilename, "rb" ) == FALSE )
	{
		IDraughtExportManager.SetExtraErrorInfo( infoFile.m_strFilename );
		return IDRAUGHT_EXPORT_ERROR_SOURCEFILE_OPEN;
	}

	CPluSalesLine csv( infoFile.m_bLegacySales );

	CString strBuffer;
	while( fileSales.ReadString( strBuffer ) == TRUE )
	{
		StatusProgress.SetPos( m_nProgress + fileSales.Tell(), m_nTotalFileSize );

		csv.ParseLine( strBuffer );

		//FILTER FOR WASTAGE
		if (csv.GetPluSaleWastageFlag() == TRUE)
		{
			continue;
		}

		if (csv.GetPluSaleVoidFlag() == TRUE)
		{
			continue;
		}

		//FILTER FOR DATE AND TIME
		CString strTime = csv.GetTranTime();
		
		if ( strTime.GetLength() != 6 )
			continue;
		
		int nHour = atoi( strTime.Left(2) );
		if ( ( nHour < 0 ) || ( nHour > 23 ) )
			continue;

		if ( TRUE == infoFile.m_bStartDate )
		{
			if ( nHour < EcrmanOptions.GetReportsDayStartHour() )
				continue;
		}
		else
		{
			if ( nHour >= EcrmanOptions.GetReportsDayStartHour() )
				continue;
			
			nHour += 24;
		}

		CSQLPluNoInfo infoPluNo;
		infoPluNo.m_nEposPluNo = csv.GetPluNoNew();
		::ProcessPluNo( infoPluNo );
				
		if ( FALSE == infoPluNo.m_bValid )
			continue;

		if ( IDraughtOptions.GetDatabaseDepartmentFlag( infoFile.m_nDbIdx, csv.GetDeptNo() ) == FALSE )
			continue;

		double dMultiplier = 1.0;
		if ( infoPluNo.m_nModifier != 0 )
			dMultiplier = DataManager.Modifier.GetMultiplierForQty ( infoPluNo.m_nModifier );
		
		if ( csv.GetLineType() == CSVPLUDATA_PLU_SALE )
		{
			CIDraughtExportLineInfo infoLine;
			infoLine.m_nHour = nHour;
			infoLine.m_nPluNo = infoPluNo.m_nBasePluNo;
			infoLine.m_dQty = csv.GetSaleQty() * dMultiplier;
			m_arraySales.Consolidate( infoLine );
		}
	}

	m_nProgress += fileSales.GetLength();
	return IDRAUGHT_EXPORT_ERROR_NONE;
}

/**********************************************************************/

int CIDraughtExportCreator::WriteSales( int nLocIdx)
{
	CSSFile fileSales;
	if ( fileSales.Open( m_strReportFilename, "ab" ) == FALSE )
	{
		IDraughtExportManager.SetExtraErrorInfo( m_strReportFilename );
		return IDRAUGHT_EXPORT_ERROR_DESTFILE_APPEND;
	}

	CString strSiteID = dbLocation.GetIDraughtID( nLocIdx );

	CString strDateFrom;
	strDateFrom.Format( "%2.2d/%2.2d/%4.4d",
		m_dateFrom.GetDay(),
		m_dateFrom.GetMonth(),
		m_dateFrom.GetYear() );
	
	CString strDateTo;
	strDateTo.Format( "%2.2d/%2.2d/%4.4d",
		m_dateTo.GetDay(),
		m_dateTo.GetMonth(),
		m_dateTo.GetYear() );
	
	for ( int n = 0; n < m_arraySales.GetSize(); n++ )
	{
		CIDraughtExportLineInfo infoLine;
		m_arraySales.GetAt( n, infoLine );
		
		CString strDateTime;

		if ( infoLine.m_nHour < 24 )
		{
			strDateTime.Format( "%s %2.2d:00",
				(const char*) strDateFrom,
				infoLine.m_nHour );
		}
		else
		{
			strDateTime.Format( "%s %2.2d:00",
				(const char*) strDateTo,
				infoLine.m_nHour - 24 );
		}

		CString strPluNo;
		::FormatPluNo( infoLine.m_nPluNo, strPluNo );

		CReportPluInfoFinder PluInfoFinder( infoLine.m_nPluNo );

		CCSV csv;
		csv.Add( strSiteID );
		csv.Add( strDateTime );
		csv.Add( strPluNo );
		csv.Add( PluInfoFinder.GetPluInfoRepText(FALSE) );
		csv.Add( infoLine.m_dQty, 2 );

		fileSales.WriteLine( csv.GetLine() );
	}

	return IDRAUGHT_EXPORT_ERROR_NONE;
}

/**********************************************************************/
