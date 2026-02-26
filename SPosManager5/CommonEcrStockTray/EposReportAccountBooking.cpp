/**********************************************************************/
#include "ReportTypes.h"
/**********************************************************************/
#include "EposReportAccountBooking.h"
/**********************************************************************/

CEposReportAccountBooking::CEposReportAccountBooking( CEposSelectArray& SelectArray ) : CEposReportAccountReceive( SelectArray )
{
	m_nReportType = REPORT_TYPE_DEPOSIT_BOOKING_PURCHASE;
	m_strUnderline = "\t\t\t\t\t\t\t<LI>";
	m_nColumnsBeforeTotal = 7;
}

/**********************************************************************/

void CEposReportAccountBooking::SetConsolidationLocation( CTermFileListInfo& infoFile, CEposReportAccountReceiveInfo& infoReceive, bool& bGotDepositNames )
{
	if ( ( FALSE == bGotDepositNames ) || ( infoFile.m_nLocIdx != infoReceive.m_nLocIdx ) )
	{
		CDataManagerInfo infoDM; 
		
		if ( TRUE == bGotDepositNames )
		{
			DataManagerNonDb.CloseDepositNames( infoDM );
			bGotDepositNames = FALSE;
		}

		if ( DataManagerNonDb.OpenDepositNames( DB_READONLY, infoFile.m_nLocIdx, infoDM ) == TRUE )
			bGotDepositNames = TRUE;
	}

	infoReceive.m_nLocIdx = infoFile.m_nLocIdx;
}

/**********************************************************************/

void CEposReportAccountBooking::AddColumns()
{
	m_ReportFile.AddColumn ( "Event Date", TA_LEFT, 300 );
	m_ReportFile.AddColumn ( "Event Time", TA_LEFT, 250 );
	m_ReportFile.AddColumn ( "Purchaser Name", TA_LEFT, 500 );
	m_ReportFile.AddColumn ( "Booking Info", TA_LEFT, 500 );
	m_ReportFile.AddColumn ( "Deposit ID", TA_LEFT, 200 );
	m_ReportFile.AddColumn ( "Purchased", TA_LEFT, 300 );
	m_ReportFile.AddColumn ( "Payment", TA_LEFT, 350 );
	m_ReportFile.AddColumn ( "Amount", TA_RIGHT, 200 );
}

/**********************************************************************/

bool CEposReportAccountBooking::CreateReceiveLine( int nLocIdx, CEposReportConsolAccountReceive& infoLine, CCSV& csv )
{
	CString strID;
	strID.Format( "%8.8d", infoLine.m_nAccountID );

	CDepositNameCSVRecord DepositRecord;
	DataManagerNonDb.DepositNames.GetDepositByID( infoLine.m_nAccountID, DepositRecord );

	csv.Add( infoLine.GetDisplayEventDate() );
	csv.Add( infoLine.GetDisplayEventTime() );
	csv.Add( DepositRecord.GetBuyerName() );
	csv.Add( DepositRecord.GetCustomerInfoDisplayLine() );
	csv.Add( strID );
	csv.Add( infoLine.GetDisplayPurchaseDate() );
	csv.Add( GetPaymentLabel( infoLine.m_nPaymentType, "" ) );
	csv.Add( infoLine.m_dVal, 2 );

	return FALSE;
}

/**********************************************************************/

bool CEposReportAccountBooking::ValidateEventTime( int nLocIdx, int nAccountID, CString& strEventDate, CString& strEventTime )
{
	strEventDate = "";
	strEventTime = "";
	bool bResult = FALSE;

	int nPos;
	if ( DataManagerNonDb.DepositNames.FindDepositByID( nAccountID, nPos ) == TRUE )
	{
		CDepositNameCSVRecord DepositName;
		DataManagerNonDb.DepositNames.GetAt( nPos, DepositName );

		CString strLogEventDate = DepositName.GetBookingDate();
		CString strLogEventTime = DepositName.GetBookingTime();

		if ( ( strLogEventDate.GetLength() == 8 ) && ( strLogEventTime.GetLength() == 5 ) )
		{
			strEventDate += "20";
			strEventDate += strLogEventDate.Right(2);
			strEventDate += strLogEventDate.Mid(3,2);
			strEventDate += strLogEventDate.Left(2);

			strEventTime += strLogEventTime.Left(2);
			strEventTime += strLogEventTime.Right(2);
			strEventTime += "00";

			bResult = TRUE;
			if ( REPORT_TYPE_DEPOSIT_BOOKING_EVENT == m_nReportType )
			{
				COleDateTime timeDummy, dateDummy;
				bResult = ValidateAccountDate( nLocIdx, strEventDate, strEventTime, timeDummy, dateDummy );
			}
		}
	}

	return bResult;
}

/**********************************************************************/
