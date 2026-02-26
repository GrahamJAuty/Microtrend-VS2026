/**********************************************************************/
#ifdef COMPILE_PMS 
/**********************************************************************/
#include "CashRSPImporter.h"
#include "FileRemover.h"
#include "PMSCheckoutDateArray.h"
#include "PMSInvoiceArray.h"
#include "PMSOptions.h"
#include "PMSRoomStatusTable.h"
#include "PriceHelpers.h"
/**********************************************************************/
#include "PMSReportFileFinder.h"
/**********************************************************************/

CPMSReportFileFinder::CPMSReportFileFinder()
{
	m_nPMSDateMode = PMS_DATEMODE_ACTUAL;
	m_strCheckDateFrom = "20110101";
	m_strCheckDateTo = "201101010";
	m_bCheckDeposits = TRUE;
	m_bIncludePending = TRUE;
	m_bIncludeActive = TRUE;
	m_bIncludeCancelled = TRUE;
}

/**********************************************************************/

void CPMSReportFileFinder::SetDefaultListInfo( CTermFileListInfo& info )
{
	m_infoDefault = info;
}

/**********************************************************************/

void CPMSReportFileFinder::SetPMSDateMode( int n, const char* szFrom, const char* szTo, bool bDeposits )
{
	switch( n )
	{
	case PMS_DATEMODE_ACTUAL:
	case PMS_DATEMODE_ACCOUNT:
		m_nPMSDateMode = n;
		break;
	}

	m_strCheckDateFrom = szFrom;
	m_strCheckDateTo = szTo;
	m_bCheckDeposits = bDeposits;
}

/**********************************************************************/

bool CPMSReportFileFinder::CheckPMSAccountDateRange( CSSFile& fileSales, int nPMSDayFrom, int nPMSDayTo )
{
	CString strBuffer;
	fileSales.ReadString( strBuffer );
					
	if ( strBuffer.Left(5) != "#DATE" )
		return FALSE;

	CCSV csvDates( strBuffer );
	//0 = #DATE
	//1 = DEPOSIT DATE FROM
	//2 = DEPOSIT DATE TO
	//3 = ACCOUNT DATE FROM
	//4 = ACCOUNT DATE TO

	if ( TRUE == m_bCheckDeposits )
	{
		if ( ( nPMSDayTo >= csvDates.GetInt(1) ) && ( nPMSDayFrom <= csvDates.GetInt(2) ) )
			return TRUE;
	}
				
	if ( ( nPMSDayTo >= csvDates.GetInt(3) ) && ( nPMSDayFrom <= csvDates.GetInt(4) ) )
		return TRUE;
	
	return FALSE;
}

/**********************************************************************/

void CPMSReportFileFinder::GetPMSFileListComplete( CEposSelectArray& arraySelect, CReportConsolidationArray<CSortedIntItem>& arrayChecked, int nPMSLocIdx, int nPMSSelArrayPos, CReportConsolidationArray<CTermFileListInfo>& arrayFileInfo, int& nTotalFileSize )
{	
	int nStartIdx = 0;
	int nEndIdx = PMSCheckoutDateArray.GetCompleteDateArraySize() - 1;
	
	if ( PMS_DATEMODE_ACCOUNT == m_nPMSDateMode )
		PMSCheckoutDateArray.GetCompleteDateArrayRange( m_strCheckDateFrom, m_strCheckDateTo, nStartIdx, nEndIdx );

	COleDateTime oleDateFrom, oleDateTo;
	::GetOleDateFromString( m_strCheckDateFrom, oleDateFrom );
	::GetOleDateFromString( m_strCheckDateTo, oleDateTo );

	int nPMSDayFrom = GetDayNumberSince2011( oleDateFrom );
	int nPMSDayTo = GetDayNumberSince2011( oleDateTo );
	
	for ( int nIdx = nStartIdx; nIdx <= nEndIdx; nIdx++ )
	{
		CString strDate;
		PMSCheckoutDateArray.GetCompleteDateArrayDate( nIdx, strDate );
		
		CString strIDs;
		PMSCheckoutDateArray.GetCompleteDateArrayIDs( nIdx, strIDs );
		
		CCSV csvIDs( strIDs );
		
		for ( int n = 0; n < csvIDs.GetSize(); n++ )
		{
			CSortedIntItem item;
			item.m_nItem = csvIDs.GetInt(n);

			int nPos;
			if ( arrayChecked.Find( item, nPos ) == TRUE )
				continue;

			arrayChecked.Consolidate( item );

			CString strFilename;
			strFilename.Format( "%s\\%d.dat",
				PMSOptions.GetPMSFolderBooking(),
				item.m_nItem );

			CSSFile fileBooking;
			if ( fileBooking.Open( strFilename, "rb" ) == FALSE )
				continue;

			CString strBuffer;
			fileBooking.ReadString( strBuffer );
			CCSV csvBooking( strBuffer );

			CPMSBookingCSVRecord BookingRecord;
			BookingRecord.ConvertFromCSV( csvBooking );

			int nAccounts = BookingRecord.GetAccountCount();
			if ( ( nAccounts < 1 ) || ( nAccounts > MAX_PMS_ACCOUNTS_PER_ROOM ) )
				continue;

			for ( int nAccountNo = 1; nAccountNo <= nAccounts; nAccountNo++ )
			{
				CString strPMSReportFile;
				strPMSReportFile.Format( "%s\\%d.%d",
					PMSOptions.GetPMSFolderReports(),
					item.m_nItem,
					nAccountNo );

				CSSFile fileTest;
				if ( fileTest.Open ( strPMSReportFile, "rb", _SH_DENYNO ) == FALSE )
					continue;

				if ( ( PMS_DATEMODE_ACCOUNT == m_nPMSDateMode ) || ( CheckPMSAccountDateRange( fileTest, nPMSDayFrom, nPMSDayTo ) ) )
				{
					CTermFileListInfo infoFile = m_infoDefault;
					infoFile.m_nSelectArrayIdx = nPMSSelArrayPos;
					infoFile.m_strFilename = strPMSReportFile;
					infoFile.m_strDateFolder = strDate;
					infoFile.m_strDateTran = strDate;
					infoFile.m_strDateSale = strDate;
					infoFile.m_nCashRSPMode = CASHRSP_SALES_PMS;
					infoFile.m_nDbIdx = 0;
					infoFile.m_nLocIdx = nPMSLocIdx;
					infoFile.m_nTermIdx = 0;
					arrayFileInfo.Consolidate( infoFile );

					nTotalFileSize += fileTest.GetLength();

					if ( PMS_DATEMODE_ACCOUNT == m_nPMSDateMode )
					{
						COleDateTime oleDate = COleDateTime (
							atoi(strDate.Left(4)),
							atoi(strDate.Mid(4,2)),
							atoi(strDate.Right(2)),
							0, 0, 0 );

						arraySelect.ProcessStartDate( nPMSSelArrayPos, oleDate );
						arraySelect.ProcessEndDate( nPMSSelArrayPos, oleDate );
					}
				}
			}
		}
	}
}

/**********************************************************************/

void CPMSReportFileFinder::GetPMSFileListIncomplete( CEposSelectArray& arraySelect, CReportConsolidationArray<CSortedIntItem>& arrayChecked, int nPMSLocIdx, int nPMSSelArrayPos, CReportConsolidationArray<CTermFileListInfo>& arrayFileInfo, int& nTotalFileSize )
{
	CString strFolder = "PMS_REPORT";
 	GetTempPath( strFolder );

	int nFileNo = 0;
	CFileRemover FileRemover;
	FileRemover.RemoveFolderContents( strFolder, nFileNo );

	::CreateSubdirectory( strFolder );

	COleDateTime oleToday = COleDateTime::GetCurrentTime();

	CString strDate;
	GetStringFromOleDate( oleToday, strDate );

	COleDateTime oleDateFrom, oleDateTo;
	::GetOleDateFromString( m_strCheckDateFrom, oleDateFrom );
	::GetOleDateFromString( m_strCheckDateTo, oleDateTo );

	int nPMSDayFrom = GetDayNumberSince2011( oleDateFrom );
	int nPMSDayTo = GetDayNumberSince2011( oleDateTo );

	//INCLUDE ACTIVE BOOKINGS
	if ( TRUE == m_bIncludeActive )
	{
		for ( int nIdx = 0; nIdx < PMSCheckoutDateArray.GetActiveArraySize(); nIdx++ )
		{
			CSortedIntItem item;
			item.m_nItem = PMSCheckoutDateArray.GetActiveID( nIdx );

			int nPos;
			if ( arrayChecked.Find( item, nPos ) == TRUE )
				continue;

			arrayChecked.Consolidate( item );

			CString strFilename;
			strFilename.Format( "%s\\%d.dat",
				PMSOptions.GetPMSFolderBooking(),
				item.m_nItem );

			CSSFile fileBooking;
			if ( fileBooking.Open( strFilename, "rb" ) == FALSE )
				continue;

			CString strBuffer;
			fileBooking.ReadString( strBuffer );
			CCSV csvBooking( strBuffer );

			CPMSBookingCSVRecord BookingRecord;
			BookingRecord.ConvertFromCSV( csvBooking );

			int nAccounts = BookingRecord.GetAccountCount();
			if ( ( nAccounts < 1 ) || ( nAccounts > MAX_PMS_ACCOUNTS_PER_ROOM ) )
				continue;

			for ( int nAccountNo = 1; nAccountNo <= nAccounts; nAccountNo++ )
			{
				CString strPMSReportFile;
				strPMSReportFile.Format( "%s\\%d.%d",
					(const char*) strFolder,
					item.m_nItem,
					nAccountNo );

				PMSOptions.SetPMSTime();
				PMSRoomStatusTable.LoadRoomAccountSettingsBuffer( BookingRecord, TRUE );
		 
				CPMSInvoiceArray arrayInvoice;
				bool bDBBCheck = ( BookingRecord.GetRoomRateType() == PMS_BOOKING_RATE_DBB ) && ( 1 == nAccountNo );
				arrayInvoice.BuildInvoiceArray( BookingRecord, nAccountNo, PMS_INVOICETYPE_EPOS_REPORT, bDBBCheck );
				arrayInvoice.CreateEPOSReportFile( MAX_PMS_ROOMS, nAccountNo - 1, TRUE );
				
				CSSFile fileTest;
				if ( fileTest.Open ( strPMSReportFile, "rb", _SH_DENYNO ) == FALSE )
					continue;

				if ( ( PMS_DATEMODE_ACCOUNT == m_nPMSDateMode ) || ( CheckPMSAccountDateRange( fileTest, nPMSDayFrom, nPMSDayTo ) ) )
				{
					CTermFileListInfo infoFile = m_infoDefault;
					infoFile.m_nSelectArrayIdx = nPMSSelArrayPos;
					infoFile.m_strFilename = strPMSReportFile;
					infoFile.m_strDateFolder = strDate;
					infoFile.m_strDateTran = strDate;
					infoFile.m_strDateSale = "";
					infoFile.m_nCashRSPMode = CASHRSP_SALES_PMS;
					infoFile.m_nDbIdx = 0;
					infoFile.m_nLocIdx = nPMSLocIdx;
					infoFile.m_nTermIdx = 0;
					infoFile.m_bIsActivePMSFile = TRUE;
					arrayFileInfo.Consolidate( infoFile );

					nTotalFileSize += fileTest.GetLength();

					if ( PMS_DATEMODE_ACCOUNT == m_nPMSDateMode )
					{
						arraySelect.ProcessStartDate( nPMSSelArrayPos, oleToday );
						arraySelect.ProcessEndDate( nPMSSelArrayPos, oleToday );
					}
				}
			}
		}
	}

	//INCLUDE DEPOSITS FOR PENDING OR CANCELLED BOOKINGS
	if ( TRUE == m_bCheckDeposits )
	{
		for ( int nBookingIdx = 0; nBookingIdx < DataManagerNonDb.PMSBooking.GetSize(); nBookingIdx++ )
		{
			CPMSBookingCSVRecord BookingRecord;
			DataManagerNonDb.PMSBooking.GetAt( nBookingIdx, BookingRecord );

			bool bInclude = FALSE;
			switch( BookingRecord.GetBookingStatus() )
			{
			case PMS_BOOKING_STATUS_PENDING:
				bInclude = m_bIncludePending;
				break;

			case PMS_BOOKING_STATUS_CANCELLED:
				bInclude = m_bIncludeCancelled;
				break;
			}

			if ( FALSE == bInclude )			
				continue;

			if ( BookingRecord.GetDepositCount() == 0 )
				continue;

			CString strPMSReportFile;
				strPMSReportFile.Format( "%s\\%d.1",
				(const char*) strFolder,
				BookingRecord.GetBookingId() );

			PMSOptions.SetPMSTime();
			PMSRoomStatusTable.LoadRoomAccountSettingsBuffer( BookingRecord, TRUE );
		 
			CPMSInvoiceArray arrayInvoice;
			arrayInvoice.BuildInvoiceArray( BookingRecord, 1, PMS_INVOICETYPE_EPOS_REPORT, FALSE );
			arrayInvoice.CreateEPOSReportFile( MAX_PMS_ROOMS, 0, TRUE );
				
			CSSFile fileTest;
			if ( fileTest.Open ( strPMSReportFile, "rb", _SH_DENYNO ) == FALSE )
				continue;

			if ( ( PMS_DATEMODE_ACCOUNT == m_nPMSDateMode ) || ( CheckPMSAccountDateRange( fileTest, nPMSDayFrom, nPMSDayTo ) ) )
			{
				CTermFileListInfo infoFile = m_infoDefault;
				infoFile.m_nSelectArrayIdx = nPMSSelArrayPos;
				infoFile.m_strFilename = strPMSReportFile;
				infoFile.m_strDateFolder = strDate;
				infoFile.m_strDateTran = strDate;
				infoFile.m_strDateSale = "";
				infoFile.m_nDbIdx = 0;
				infoFile.m_nLocIdx = nPMSLocIdx;
				infoFile.m_nTermIdx = 0;
				infoFile.m_bIsActivePMSFile = TRUE;
				arrayFileInfo.Consolidate( infoFile );

				nTotalFileSize += fileTest.GetLength();

				if ( PMS_DATEMODE_ACCOUNT == m_nPMSDateMode )
				{
					arraySelect.ProcessStartDate( nPMSSelArrayPos, oleToday );
					arraySelect.ProcessEndDate( nPMSSelArrayPos, oleToday );
				}
			}
		}
	}
}

/**********************************************************************/
// 1 ... SET PMS DATE FOR TAX PURPOSES, REGARDLESS OF DATE MODE
// 2 ... SET PMS DATE AND TIME FOR REPORTING BY ACTUAL TIME  
/**********************************************************************/

void CPMSReportFileFinder::ProcessPMSTimeLine( CString& strBuffer, int& nPMSDayCurrent, COleDateTime& oleDatePMS, CString& strPMSDate, CString& strPMSTime )
{
	CCSV csvTime( strBuffer );
	nPMSDayCurrent = csvTime.GetInt(1);

	if ( PMS_DATEMODE_ACTUAL == m_nPMSDateMode )
	{
		GetOleDateSince2011( nPMSDayCurrent, oleDatePMS );

		strPMSDate.Format( "%4.4d%2.2d%2.2d",
			oleDatePMS.GetYear(),
			oleDatePMS.GetMonth(),
			oleDatePMS.GetDay() );

		strPMSTime.Format( "%2.2d%2.2d00", csvTime.GetInt(2), csvTime.GetInt(3) );
	}
}

/**********************************************************************/
#endif
/**********************************************************************/
