/**********************************************************************/
//#include "DataManager.h"
//#include "DataManagerNonDb.h"
#include "HistoryFileTypes.h"
#include "LocationCSVArray.h"
#include "PMSDefines.h"
#include "ReportTypes.h"
#include "StatusProgress.h"
/**********************************************************************/
#include "EposReportDepositUnredeemed.h"
/**********************************************************************/

CEposReportConsolDepositUnredeemed::CEposReportConsolDepositUnredeemed()
{
	Reset();
}

/**********************************************************************/

void CEposReportConsolDepositUnredeemed::Reset()
{
	m_nLocSort = 0;
	m_strDate = "";
	m_strTime = "";
	m_nDepositID = 0;
	m_dVal = 0.0;
}

/**********************************************************************/

void CEposReportConsolDepositUnredeemed::Add ( CEposReportConsolDepositUnredeemed& source )
{
	m_dVal += source.m_dVal;
}

/**********************************************************************/

int CEposReportConsolDepositUnredeemed::Compare( CEposReportConsolDepositUnredeemed& source, int nHint )
{
	if ( m_nLocSort != source.m_nLocSort )
		return ( m_nLocSort > source.m_nLocSort ? 1 : -1 );

	if ( m_strDate != source.m_strDate )
		return ( m_strDate > source.m_strDate ? 1 : -1 );

	if ( m_strTime != source.m_strTime )
		return ( m_strTime > source.m_strTime ? 1 : -1 );

	if ( m_nDepositID != source.m_nDepositID )
		return ( m_nDepositID > source.m_nDepositID ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEposReportConsolDepositCharged::CEposReportConsolDepositCharged()
{
	Reset();
}

/**********************************************************************/

void CEposReportConsolDepositCharged::Reset()
{
	m_nLocSort = 0;
	m_nDepositID = 0;
	m_nSelectArrayIdx = 0;
	m_strDate = "";
}

/**********************************************************************/

void CEposReportConsolDepositCharged::Add ( CEposReportConsolDepositCharged& source )
{
}

/**********************************************************************/

int CEposReportConsolDepositCharged::Compare( CEposReportConsolDepositCharged& source, int nHint )
{
	if ( m_nLocSort != source.m_nLocSort )
		return ( m_nLocSort > source.m_nLocSort ? 1 : -1 );

	if ( m_nDepositID != source.m_nDepositID )
		return ( m_nDepositID > source.m_nDepositID ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEposReportDepositUnredeemed::CEposReportDepositUnredeemed( CEposSelectArray& SelectArray ) : CEposReport( SelectArray )
{
	m_nReportType = REPORT_TYPE_DEPOSIT_UNREDEEMED;
	SetPMSAllowMode( PMS_ALLOWMODE_NONE );
}

/**********************************************************************/

void CEposReportDepositUnredeemed::WriteTotal( double dTotal )
{
	m_ReportFile.WriteReportMiscLine( "\t\t\t\t<LI>" );

	CCSV csv( '\t' );
	csv.Add( "" );
	csv.Add( "" );
	csv.Add( "" );
	csv.Add( "Total" );
	csv.Add( dTotal, 2 );
	m_ReportFile.WriteReportMiscLine( csv.GetLine() );
}

/**********************************************************************/

void CEposReportDepositUnredeemed::Consolidate()
{
	CSortedDateTimeItem PurchaseCheckDateTo = DataManagerNonDb.SessionDateTimeFilter.GetCheckDateTo();
	DataManagerNonDb.SessionDateTimeFilter.SetDepositUnredeemedCheckDateTo();

	m_SelectArray.TagOriginalSelectionStatus( TRUE );

	int nTotalFileSize;
	CReportConsolidationArray<CTermFileListInfo> arrayFileInfo;
	CEposReportFileListMaker FileListMaker( m_bEODMode, m_bSaleTime, m_PMSModes );
	FileListMaker.GetFileList ( arrayFileInfo, HISTORYFILE_TYPE_ACCOUNT, m_SelectArray, FALSE, nTotalFileSize );

	int nProgress = 0;	
	StatusProgress.Lock( TRUE, "Checking purchased deposits" );

	for ( int nFileIdx = 0; nFileIdx < arrayFileInfo.GetSize(); nFileIdx++ )
	{
		CTermFileListInfo infoFile;
		arrayFileInfo.GetAt( nFileIdx, infoFile );

		CSSFile fileSales;
		if ( fileSales.Open ( infoFile.m_strFilename, "rb", _SH_DENYNO ) == FALSE )
			continue;

		COleDateTime timeTransaction = COleDateTime( 2011, 1, 1, 0, 0, 0 );
		COleDateTime dateTransaction = COleDateTime( 2011, 1, 1, 0, 0, 0 );
		CString strDate = "";
		CString strTime = "";
		bool bValidTime = FALSE;

		CString strBuffer;
		while( fileSales.ReadString( strBuffer ) == TRUE )
		{
			StatusProgress.SetPos ( nProgress + fileSales.Tell(), nTotalFileSize );

			CCSVAccountHistory csvIn ( strBuffer );

			int nAccountLineType = csvIn.GetAccountLineType(); 
			switch( nAccountLineType )
			{
			case ACCOUNT_LINETYPE_HEADER:
				strDate = infoFile.m_strDateTran;
				strTime = csvIn.GetHeaderLineTime();
				bValidTime = ValidateAccountDate( infoFile.m_nLocIdx, strDate, strTime, timeTransaction, dateTransaction );
				break;

			case ACCOUNT_LINETYPE_RECEIVE:
				if ( ( TRUE == bValidTime ) && ( csvIn.GetReceiveType() == CASHRSP_ITEMPAYTYPE_DEPOSIT ) )
				{
					//PURCHASED DEPOSITS ARE ONLY CHECKED AGAINST REQUESTED TERMINALS
					if ( m_SelectArray.GetOriginalSelectionStatus( infoFile.m_nLocIdx, infoFile.m_nTermIdx ) == TRUE )
					{
						//PURCHASED DEPOSITS ARE ONLY CHECKED AGAINST REQUESTED DATE RANGE
						if ( strDate <= PurchaseCheckDateTo.m_strDateTime )
						{
							CEposReportConsolDepositUnredeemed info;
							info.m_nLocSort = dbLocation.GetSortIdxFromLocIdx( infoFile.m_nLocIdx );
							info.m_strDate = strDate;
							info.m_strTime = strTime;
							info.m_nDepositID = csvIn.GetReceiveID();
							info.m_dVal = csvIn.GetReceiveValue();
							m_arrayDepositsByDate.Consolidate( info );
				
							m_SelectArray.ProcessStartDate ( infoFile.m_nSelectArrayIdx, dateTransaction );
							m_SelectArray.ProcessEndDate( infoFile.m_nSelectArrayIdx, dateTransaction );
						}
					}
				}
				break;

			case ACCOUNT_LINETYPE_CHARGE:
			case ACCOUNT_LINETYPE_REFUND:
				//DON'T USE SESSION FILTER FOR REDEMPTION
				if ( csvIn.GetChargeType() == CASHRSP_ITEMPAYTYPE_DEPOSIT )
				{
					CEposReportConsolDepositCharged info;
					info.m_nLocSort = dbLocation.GetSortIdxFromLocIdx( infoFile.m_nLocIdx );
					info.m_nDepositID = ( ACCOUNT_LINETYPE_REFUND == nAccountLineType ) ? csvIn.GetRefundID() : csvIn.GetChargeID();
					info.m_nSelectArrayIdx = infoFile.m_nSelectArrayIdx;
					info.m_strDate = strDate;
					m_arrayDepositsCharged.Consolidate( info );
				}
				break;
			}
		}

		nProgress += fileSales.GetLength();
		fileSales.Close();
	}

	StatusProgress.Unlock();
}

/**********************************************************************/

bool CEposReportDepositUnredeemed::CreateReport()
{
	if ( m_ReportFile.Open ( Super.ReportFilename() ) == FALSE )
		return FALSE;

	m_ReportFile.SetStyle1 ( GetReportTitle(FALSE) );
	m_ReportFile.AddColumn ( "Date", TA_LEFT, 250 );
	m_ReportFile.AddColumn ( "Time", TA_LEFT, 200 );
	m_ReportFile.AddColumn ( "Deposit ID", TA_LEFT, 200 );
	m_ReportFile.AddColumn ( "Purchaser Name", TA_LEFT, 500 );
	m_ReportFile.AddColumn ( "Amount", TA_RIGHT, 200 );
	
	int nThisBlockPos = 0;
	int nNextBlockPos = 0;

	int nSize = m_arrayDepositsByDate.GetSize();
	StatusProgress.Lock( TRUE, "Creating report" );
	
	while ( nThisBlockPos < m_arrayDepositsByDate.GetSize() )
	{
		CEposReportConsolDepositUnredeemed infoThisBlock;
		m_arrayDepositsByDate.GetAt( nThisBlockPos, infoThisBlock );

		int nLocIdx = dbLocation.GetLocIdxFromSortIdx( infoThisBlock.m_nLocSort );
		int nDbNo = dbLocation.GetDbNo( nLocIdx );

		CEposReportConsolDepositUnredeemed infoNextBlock;
		infoNextBlock.m_nLocSort = infoThisBlock.m_nLocSort + 1;
		m_arrayDepositsByDate.Find( infoNextBlock, nNextBlockPos );

		CString strLocation;
		strLocation.Format( "<..>%s", dbLocation.GetName( nLocIdx ) );

		if ( 0 != nThisBlockPos )
			m_ReportFile.RequestBlankLines(2);

		m_ReportFile.WriteReportMiscLine( strLocation + ", Deposits Unredeemed" );
		m_ReportFile.WriteReportMiscLine( "<LI>" );

		double dTotal = 0.0;

		CDataManagerInfo infoDM;
		DataManagerNonDb.OpenDepositNames( DB_READONLY, nLocIdx, infoDM );
		
		for ( int nPos = nThisBlockPos; nPos < nNextBlockPos; nPos++ )
		{
			CEposReportConsolDepositUnredeemed infoLine;
			m_arrayDepositsByDate.GetAt( nPos, infoLine );

			CEposReportConsolDepositCharged infoCharged;
			infoCharged.m_nLocSort = infoLine.m_nLocSort;
			infoCharged.m_nDepositID = infoLine.m_nDepositID;

			int nRedemptionPos;
			if ( m_arrayDepositsCharged.Find( infoCharged, nRedemptionPos ) == FALSE )
			{
				dTotal += infoLine.m_dVal;

				CString strDate;
				strDate.Format( "%s/%s/%s",
					(const char*) infoLine.m_strDate.Right(2),
					(const char*) infoLine.m_strDate.Mid(4,2),
					(const char*) infoLine.m_strDate.Left(4) );

				CString strTime;
				strTime.Format( "%s:%s:%s",
					(const char*) infoLine.m_strTime.Left(2),
					(const char*) infoLine.m_strTime.Mid(2,2),
					(const char*) infoLine.m_strTime.Right(2) );

				CString strDepositID;
				strDepositID.Format( "%8.8d",
					infoLine.m_nDepositID );

				CCSV csv( '\t' );
				csv.Add( strDate );
				csv.Add( strTime );
				csv.Add( strDepositID );
				csv.Add( DataManagerNonDb.DepositNames.GetBuyerNameByDepositID( infoLine.m_nDepositID ) );
				csv.Add( infoLine.m_dVal, 2 );
				m_ReportFile.WriteReportDataLine( csv.GetLine() );
			}
			else
			{
				CEposReportConsolDepositCharged infoLine;
				m_arrayDepositsCharged.GetAt( nRedemptionPos, infoLine );

				COleDateTime date = COleDateTime(
										atoi( infoLine.m_strDate.Left(4) ),
										atoi( infoLine.m_strDate.Mid(4,2) ),
										atoi( infoLine.m_strDate.Right(2) ),
										0, 0, 0 );

				m_SelectArray.ProcessStartDate ( infoLine.m_nSelectArrayIdx, date );
				m_SelectArray.ProcessEndDate ( infoLine.m_nSelectArrayIdx, date );
			}
		}

		WriteTotal( dTotal );

		DataManagerNonDb.CloseDepositNames( infoDM );
		
		nThisBlockPos = nNextBlockPos;
	}

	StatusProgress.Unlock();
				
	ShowEposDetails();
	m_ReportFile.Close();

	return TRUE;
}

/**********************************************************************/
