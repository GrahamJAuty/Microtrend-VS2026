/**********************************************************************/
#include "CashRSPImporter.h"
#include "DateRangeHelpers.h"
#include "EposReportBase.h"
#include "FileRemover.h"
#include "HistoryFileTypes.h"
#include "LocationCSVArray.h"
#include "NetworkCSVArray.h"
#include "ReportHelpers.h"
#include "SageExportManager.h"
#include "SageOptions.h"
#include "StatusProgress.h"
#include "StringDateChecker.h"
/**********************************************************************/
#include "SageExportCreator.h"
/**********************************************************************/

CEposReportConsolSageGroup::CEposReportConsolSageGroup()
{
	Reset();
}

/**********************************************************************/

void CEposReportConsolSageGroup::Reset()
{
	m_bPromo = FALSE;
	m_strLineID = "";
	m_nTaxDateCode = 0;
	m_nTaxBand = 0;
	m_dVal = 0.0;
	m_dNonTax = 0.0;
}

/**********************************************************************/

void CEposReportConsolSageGroup::Add (CEposReportConsolSageGroup& source )
{
	m_dVal += source.m_dVal;
	m_dNonTax += source.m_dNonTax;
}

/**********************************************************************/

int CEposReportConsolSageGroup::Compare( CEposReportConsolSageGroup& source, int nHint )
{
	if ( m_bPromo != source.m_bPromo )
		return ( m_bPromo ? 1 : -1 );

	if ( m_strLineID != source.m_strLineID )
		return ( ( m_strLineID > source.m_strLineID ) ? 1 : -1 );

	if ( m_nTaxDateCode != source.m_nTaxDateCode )
		return ( ( m_nTaxDateCode > source.m_nTaxDateCode ) ? 1 : -1 );

	if ( m_nTaxBand != source.m_nTaxBand )
		return ( ( m_nTaxBand > source.m_nTaxBand ) ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CSageExportCreator::CSageExportCreator( COleDateTime& date )
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
}

/**********************************************************************/

int CSageExportCreator::CreateExport()
{
	CFilenameUpdater FnUp( SysFiles::SageTempReport );
	m_strReportFilename = FnUp.GetFilenameToUse();

	if ( ::FileExists( m_strReportFilename ) == TRUE )
		CFileRemover FileRemover( m_strReportFilename );

	if ( ::FileExists( m_strReportFilename ) == TRUE )
	{
		SageExportManager.SetExtraErrorInfo( m_strReportFilename );
		return SAGE_EXPORT_ERROR_DESTFILE_DELETE;
	}

	int nResult = GetFileList();
	if ( SAGE_EXPORT_ERROR_NONE != nResult )
		return nResult;

	if ( m_arrayFiles.GetSize() == 0 )
		return SAGE_EXPORT_ERROR_NODATA;

	CString strDate;
	strDate.Format( "%2.2d/%2.2d/%4.4d",
		m_dateFrom.GetDay(),
		m_dateFrom.GetMonth(),
		m_dateFrom.GetYear() );

	m_nProgress = 0;

	StatusProgress.Lock( TRUE, "Creating Sage export for" );
	StatusProgress.SetDialogText( strDate );
	nResult = CreateExportInternal();
	StatusProgress.Unlock();

	return nResult;
}

/**********************************************************************/

int CSageExportCreator::CreateExportInternal()
{
	int nDbIdx = SageExportManager.GetDatabaseIdx();

	CDataManagerInfo infoDb;
	if ( DataManager.OpenDatabaseReadOnly( nDbIdx, infoDb ) == FALSE )
		return SAGE_EXPORT_ERROR_DATABASE_OPEN;
	
	for ( int nIdx = 0; nIdx < m_arrayFiles.GetSize(); nIdx++ )
	{
		CSageTermFileInfo infoFile = m_arrayFiles.GetAt( nIdx );

		int nResult = ConsolidateSales( infoFile );
		if ( nResult != 0 )
			return nResult;
	}

	int nResult = WriteSales();
	if ( nResult != 0 )
		return nResult;

	CString strDestFile;
	strDestFile.Format( "Exports\\Sage\\%s_D%3.3d.csv",
		(const char*) m_strCheckDateFrom,
		dbDatabase.GetDbNo( SageExportManager.GetDatabaseIdx() ) );
	GetSyssetProgramPath( strDestFile );

	if ( ::FileExists( strDestFile ) == TRUE )
		CFileRemover FileRemover( strDestFile );

	if ( ::FileExists( strDestFile ) == TRUE )
	{
		SageExportManager.SetExtraErrorInfo( strDestFile );
		return SAGE_EXPORT_ERROR_DESTFILE_DELETE;
	}

	if ( rename( m_strReportFilename, strDestFile ) != 0 )
	{
		SageExportManager.SetExtraErrorInfo( strDestFile.Right(17) );
		return SAGE_EXPORT_ERROR_DESTFILE_RENAME;
	}
	
	return SAGE_EXPORT_ERROR_NONE;
}

/**********************************************************************/

int CSageExportCreator::GetFileList()
{
	m_arrayFiles.RemoveAll();
	m_nTotalFileSize = 0;

	int nDbIdx = SageExportManager.GetDatabaseIdx();
	
	CArray<int,int> arrayLocIdx;
	dbLocation.GetDatabaseLocIdxSet( nDbIdx, arrayLocIdx );

	for ( int n = 0; n < arrayLocIdx.GetSize(); n++ )
	{
		int nLocIdx = arrayLocIdx.GetAt(n);	
		int nTermCount = dbLocation.GetTerminalCount( nLocIdx );

		int nNwkIdx;
		int nNwkNo = dbLocation.GetNetworkNo( nLocIdx );
		if ( dbNetwork.FindNetworkByNumber( nNwkNo, nNwkIdx ) == FALSE )
			continue;

		CString strNetworkPath = dbNetwork.GetFolderPathData ( nNwkIdx );
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

			//FIND SALES FILES FOR THIS LOCATION
			for ( int nTermIdx = 0; nTermIdx < nTermCount; nTermIdx++ )
			{
				CString strSalesFile;
				strSalesFile.Format( "%s\\%s\\sales.%3.3d",
					dbNetwork.GetFolderPathData( nNwkIdx ),
					(const char*) strDateFolder,
					dbLocation.GetTNo( nLocIdx, nTermIdx ) );

				if ( ::FileExists( strSalesFile ) == TRUE )
				{
					CSSFile fileTest;
					if ( fileTest.Open( strSalesFile, "rb" ) == FALSE )
					{
						SageExportManager.SetExtraErrorInfo( strSalesFile );
						return SAGE_EXPORT_ERROR_SOURCEFILE_OPEN;
					}

					m_nTotalFileSize += fileTest.GetLength();

					CSageTermFileInfo info;
					info.m_nDbIdx = nDbIdx;
					info.m_nNetworkIdx = nNwkIdx;
					info.m_nLocIdx = nLocIdx;
					info.m_bStartDate = ( strDateTran == m_strCheckDateFrom );
					info.m_strDate = strDateTran;
					info.m_strFilename = strSalesFile;
					info.m_bLegacySales = ( strDateFolder.GetLength() == 8 );
					m_arrayFiles.Add( info );
				}
			}
		}
	}

	return SAGE_EXPORT_ERROR_NONE;
}

/**********************************************************************/

int CSageExportCreator::ConsolidateSales( CSageTermFileInfo& infoFile )
{
	m_arraySales.RemoveAll();

	CSSFile fileSales;
	if ( fileSales.Open( infoFile.m_strFilename, "rb" ) == FALSE )
	{
		SageExportManager.SetExtraErrorInfo( infoFile.m_strFilename );
		return SAGE_EXPORT_ERROR_SOURCEFILE_OPEN;
	}

	int arrayNominalCodeLookupGroup[100];
	int arrayNominalCodeLookupPromo[100];
	CStringArray arrayNominalCodeStringGroup;
	CStringArray arrayNominalCodeStringPromo;

	for ( int n = 0; n < 100; n++ )
	{
		arrayNominalCodeLookupGroup[n] = 0;
		arrayNominalCodeLookupPromo[n] = 0;
	}

	bool bGotGroup = FALSE;
	bool bGotPromo = FALSE;

	for ( int n = 0; n < DataManager.EposGroup.GetSize(); n++ )
	{
		CGroupCSVRecordEpos GroupRecord;
		DataManager.EposGroup.GetAt( n, GroupRecord );

		int nGroupNo = GroupRecord.GetGroupNo();
		if ( ( nGroupNo >= 1 ) && ( nGroupNo <= 99 ) )
		{
			int nCode = GroupRecord.GetSageNominalCode();
			int nMaxLen = SageOptions.GetNominalCodeLength();

			if ( nCode != 0 )
			{
				CString strID;
				strID.Format( "%*.*d", nMaxLen, nMaxLen, nCode );

				if ( strID.GetLength() <= nMaxLen )
				{
					arrayNominalCodeLookupGroup[ nGroupNo ] = arrayNominalCodeStringGroup.GetSize() + 1;
					arrayNominalCodeStringGroup.Add( strID );
					bGotGroup = TRUE;
				}
			}
		}
	}

	for ( int n = 0; n < DataManager.Promotion.GetSize(); n++ )
	{
		CPromotionsCSVRecord PromoRecord;
		DataManager.Promotion.GetAt( n, PromoRecord );

		int nPromoNo = PromoRecord.GetPromoNo();
		if ( ( nPromoNo >= 1 ) && ( nPromoNo <= 99 ) )
		{
			int nCode = PromoRecord.GetSageNominalCode();
			int nMaxLen = SageOptions.GetNominalCodeLength();

			if ( nCode != 0 )
			{
				CString strID;
				strID.Format( "%*.*d", nMaxLen, nMaxLen, nCode );

				if ( strID.GetLength() <= nMaxLen )
				{
					arrayNominalCodeLookupPromo[ nPromoNo ] = arrayNominalCodeStringPromo.GetSize() + 1;
					arrayNominalCodeStringPromo.Add( strID );
					bGotPromo = TRUE;
				}
			}
		}
	}

	if ( ( FALSE == bGotGroup ) && ( FALSE == bGotPromo ) )
		return SAGE_EXPORT_ERROR_NOCODES;

	int nTaxDateCode = DataManager.HistoricalTaxRates.GetDateCode( infoFile.m_strDate );

	CReportConsolidationArray<CEposReportConsolSageGroup> arrayTempSales;

	CPluSalesLine csv( infoFile.m_bLegacySales );

	CString strBuffer;
	while( fileSales.ReadString( strBuffer ) == TRUE )
	{
		StatusProgress.SetPos( m_nProgress + fileSales.Tell(), m_nTotalFileSize );

		csv.ParseLine( strBuffer );

		//CHECK FOR RELEVANT LINE TYPESS
		int nLineType = csv.GetLineType();
		switch( nLineType )
		{
		case CSVPLUDATA_PROMO_TAXABLE:
		case CSVPLUDATA_PROMO_NONTAXABLE:
		case CSVPLUDATA_PLU_SALE:
		case CSVPLUDATA_PLU_DISCOUNT:
		case CSVPLUDATA_PLU_MIXMATCH:
			break;

		case CSVPLUDATA_MIXMATCH_NONTAXABLE:
		case CSVPLUDATA_PLU_CHEAPESTFREE:
		default:
			continue;
		}

		CString strNominalCode = "";
		int nTaxBand = 0;
		bool bPromo = FALSE;

		switch( nLineType )
		{
		case CSVPLUDATA_PROMO_TAXABLE:
		case CSVPLUDATA_PROMO_NONTAXABLE:
			{
				int nPromoNo = csv.GetPromoNoSeparate();

				if ( ( nPromoNo >= 1 ) && ( nPromoNo <= 99 ) )
				{
					int nLookUp = arrayNominalCodeLookupPromo[ nPromoNo ];
					if ( ( nLookUp > 0 ) && ( nLookUp <= arrayNominalCodeStringPromo.GetSize() ) )
					{
						strNominalCode = arrayNominalCodeStringPromo.GetAt( nLookUp - 1 );
						nTaxBand = csv.GetNumericTaxBand();
						bPromo = TRUE;
					}
				}
			}
			break;

		case CSVPLUDATA_PLU_SALE:
		case CSVPLUDATA_PLU_DISCOUNT:
		case CSVPLUDATA_PLU_MIXMATCH:
			{
				int nGroupNo = csv.GetGroupNo() - 1;

				if ( ( nGroupNo >= 1 ) && ( nGroupNo <= 99 ) )
				{
					int nLookUp = arrayNominalCodeLookupGroup[ nGroupNo ];
					if ( ( nLookUp > 0 ) && ( nLookUp <= arrayNominalCodeStringGroup.GetSize() ) )
					{
						strNominalCode = arrayNominalCodeStringGroup.GetAt( nLookUp - 1 );
						nTaxBand = csv.GetNumericTaxBand();
						bPromo = FALSE;
					}
				}
			}
			break;
		}

		if ( strNominalCode == "" )
			continue;

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

		CEposReportConsolSageGroup infoConsol;
		infoConsol.m_bPromo = bPromo;
		infoConsol.m_strLineID = strNominalCode;
		infoConsol.m_nTaxDateCode = nTaxDateCode;
		infoConsol.m_nTaxBand = nTaxBand;
		infoConsol.m_dVal = csv.GetValue();

		switch( nLineType )
		{
		case CSVPLUDATA_PROMO_TAXABLE:
		case CSVPLUDATA_PROMO_NONTAXABLE:
			infoConsol.m_dVal *= -1;
			break;
		}

		arrayTempSales.Consolidate( infoConsol );	
	}

	int nGroups = 0;
	int nPromos = 0;

	for ( int n = 0; n < arrayTempSales.GetSize(); n++ )
	{
		CEposReportConsolSageGroup infoConsol;
		arrayTempSales.GetAt( n, infoConsol );
			
		if ( TRUE == infoConsol.m_bPromo )
			nPromos++;
		else
		{
			nGroups++;
			double dTaxRate = DataManager.HistoricalTaxRates.GetTaxRate( infoConsol.m_nTaxDateCode, infoConsol.m_nTaxBand );
			infoConsol.m_dNonTax = ReportHelpers.CalcNonTax( infoConsol.m_dVal, dTaxRate );
		}

		infoConsol.m_nTaxDateCode = 0;
		infoConsol.m_nTaxBand = 0;
		m_arraySales.Consolidate( infoConsol );
	}

	CString strExtra;
	strExtra.Format( "(Groups: %d, Promos: %d)", nGroups, nPromos );
	SageExportManager.SetExtraErrorInfo( strExtra );

	m_nProgress += fileSales.GetLength();
	
	return SAGE_EXPORT_ERROR_NONE;
}

/**********************************************************************/

int CSageExportCreator::WriteSales()
{
	CSSFile fileSales;
	if ( fileSales.Open( m_strReportFilename, "wb" ) == FALSE )
	{
		SageExportManager.SetExtraErrorInfo( m_strReportFilename );
		return SAGE_EXPORT_ERROR_DESTFILE_WRITE;
	}

	CCSV csv;
	csv.Add( "Type" );
	csv.Add( "Account Reference" );
	csv.Add( "Nominal A/C Ref" );
	csv.Add( "Department Code" );
	csv.Add( "Date" );
	csv.Add( "Reference" );
	csv.Add( "Details" );
	csv.Add( "Net Amount" );
	csv.Add( "Tax Code" );
	csv.Add( "Tax Amount" );
	csv.Add( "Exchange Rate" );
	csv.Add( "Extra Reference" );
	csv.Add( "User Name" );
	csv.Add( "Project Refn" );
	csv.Add( "Cost Code Refn" );

	fileSales.WriteLine( csv.GetLine() );

	CString strDate;
	strDate.Format ( "%s/%s/%s",
		(const char*) m_strCheckDateFrom.Right(2),
		(const char*) m_strCheckDateFrom.Mid(4,2),
		(const char*) m_strCheckDateFrom.Left(4) );

	for ( int n = 0; n < m_arraySales.GetSize(); n++ )
	{
		CEposReportConsolSageGroup infoConsol;
		m_arraySales.GetAt( n, infoConsol );

		CCSV csv;
		csv.Add( "BR" );
		csv.Add( SageOptions.GetAccountReference() );
		csv.Add( infoConsol.m_strLineID );
		csv.Add( "" );
		csv.Add( strDate );
		csv.Add( "" );
		csv.Add( "" );
		csv.Add( infoConsol.m_dVal, 2 );

		if ( TRUE == infoConsol.m_bPromo )
		{
			csv.Add( "T9" );
			csv.Add( "0.00" );
		}
		else
		{
			csv.Add( "T1" );
			csv.Add( infoConsol.m_dVal - infoConsol.m_dNonTax, 2 );
		}

		csv.Add( "" );
		csv.Add( "" );
		csv.Add( "" );
		csv.Add( "" );
		csv.Add( "" );

		fileSales.WriteLine( csv.GetLine() );
	}

	return SAGE_EXPORT_ERROR_NONE;
}

/**********************************************************************/
