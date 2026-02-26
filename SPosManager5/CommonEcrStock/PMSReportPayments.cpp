/**********************************************************************/
#ifdef COMPILE_PMS 
/**********************************************************************/
#include "CCSVReportLine.h"
#include "PMSInvoiceArray.h"
#include "PriceHelpers.h"
#include "RepSpawn.h"
#include "StatusProgress.h"
/**********************************************************************/
#include "PMSReportPayments.h"
/**********************************************************************/

CEposReportConsolPMSPayments::CEposReportConsolPMSPayments()
{
	Reset();
}

/**********************************************************************/

void CEposReportConsolPMSPayments::Reset()
{
	m_nPaymentNo = 0;
	m_dAccount = 0.0;
	m_dDeposit = 0.0;
}

/**********************************************************************/

void CEposReportConsolPMSPayments::Add ( CEposReportConsolPMSPayments& source )
{
	m_dAccount += source.m_dAccount;
	m_dDeposit += source.m_dDeposit;
}

/**********************************************************************/

int CEposReportConsolPMSPayments::Compare( CEposReportConsolPMSPayments& source, int nHint )
{
	if ( m_nPaymentNo != source.m_nPaymentNo )
		return ( m_nPaymentNo > source.m_nPaymentNo ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CPMSReportPayments::CPMSReportPayments()
{
	CFilenameUpdater FnUp( SysFiles::PMSPaymentsPrm );
	m_strParamsFilename = FnUp.GetFilenameToUse();
	m_bIncludePending = TRUE;
	m_bIncludeActive = TRUE;
	m_bIncludeComplete = FALSE;
	m_bIncludeCancelled = FALSE;
	m_bIncludeDeposit = TRUE;
	m_bIncludePayment = TRUE;
	m_bIncludeTraining = FALSE;
	m_strSessionTitle = "";
}

/**********************************************************************/

void CPMSReportPayments::SetReportDates( COleDateTime& dateFrom, COleDateTime& dateTo, CSessionCSVRecord& ReportSession )
{
	DataManagerNonDb.SessionDateTimeFilter.SetDates( dateFrom, dateTo, ReportSession );
}

/**********************************************************************/

bool CPMSReportPayments::CheckTime( int nPMSDay, COleDateTime& oleTime )
{
	COleDateTime date;
	GetOleDateSince2011( nPMSDay, date );

	CString strDate;
	strDate.Format( "%4.4d%2.2d%2.2d",
		date.GetYear(),
		date.GetMonth(),
		date.GetDay() );

	CString strTime;
	strTime.Format( "%2.2d%2.2d",
		oleTime.GetHour(),
		oleTime.GetMinute() );

	return ( DataManagerNonDb.SessionDateTimeFilter.CheckTime( strDate, strTime ) != 0 );
}

/**********************************************************************/

void CPMSReportPayments::CreateReport()
{
	bool bResult = FALSE;

	StatusProgress.Lock( TRUE, "Creating report" );

	bResult = CreateReportInternal();
	
	StatusProgress.Unlock();
	
	if ( TRUE == bResult )
	{
		CReportManager ReportManager;
		ReportManager.DisplayReport ( Super.ReportFilename(), m_strParamsFilename, "", NULL );
	}
	else
		Prompter.Error( "Unable to create report." );
}

/**********************************************************************/

bool CPMSReportPayments::CreateReportInternal()
{
	if ( m_ReportFile.Open ( Super.ReportFilename() ) == FALSE )
		return FALSE;

	{
		COleDateTime dateFrom = DataManagerNonDb.SessionDateTimeFilter.GetDateFrom().m_oleDateTime;
		COleDateTime dateTo = DataManagerNonDb.SessionDateTimeFilter.GetDateTo().m_oleDateTime;

		CString strTitle;
		strTitle.Format( "PMS Payment Summary (%2.2d/%2.2d/%4.4d - %2.2d/%2.2d/%4.4d, %s)",
			dateFrom.GetDay(),
			dateFrom.GetMonth(),
			dateFrom.GetYear(),
			dateTo.GetDay(),
			dateTo.GetMonth(),
			dateTo.GetYear(),
			(const char*) m_strSessionTitle );
	
		m_ReportFile.SetStyle1 ( strTitle );
	}

	m_ReportFile.AddColumn ( "Type", TA_LEFT, 120 );
	m_ReportFile.AddColumn ( "Name", TA_LEFT, 300 );

	if ( TRUE == m_bIncludeDeposit )
		m_ReportFile.AddColumn ( "Deposit", TA_RIGHT, 200 );

	if ( TRUE == m_bIncludePayment )
		m_ReportFile.AddColumn ( "Account", TA_RIGHT, 200 );
	
	if ( ( TRUE == m_bIncludeDeposit ) && ( TRUE == m_bIncludePayment ) )
		m_ReportFile.AddColumn ( "Total", TA_RIGHT, 200 );

	StatusProgress.Lock( TRUE, "Creating Report" );

	CReportConsolidationArray<CEposReportConsolPMSPayments> arrayPayments;
	
	int nSize = DataManagerNonDb.PMSBooking.GetSize();
	for ( int nBookingIdx = 0; nBookingIdx < nSize; nBookingIdx++ )
	{
		StatusProgress.SetPos( nBookingIdx, nSize );

		CPMSBookingCSVRecord BookingRecord;
		DataManagerNonDb.PMSBooking.GetAt( nBookingIdx, BookingRecord );

		if ( FALSE == m_bIncludeTraining )
			if ( BookingRecord.GetTrainingFlag() == TRUE )
				continue;

		bool bAcceptStatus = FALSE;
		switch ( BookingRecord.GetBookingStatus() )
		{
		case PMS_BOOKING_STATUS_PENDING:
			bAcceptStatus = m_bIncludePending;
			break;

		case PMS_BOOKING_STATUS_ACTIVE:
			bAcceptStatus = m_bIncludeActive;
			break;

		case PMS_BOOKING_STATUS_COMPLETE:
			bAcceptStatus = m_bIncludeComplete;
			break;

		case PMS_BOOKING_STATUS_CANCELLED:
			bAcceptStatus = m_bIncludeCancelled;
			break;
		}

		if ( FALSE == bAcceptStatus )
			continue;

		if ( TRUE == m_bIncludeDeposit )
		{
			for ( int n = 0; n < BookingRecord.GetDepositCount(); n++ )
			{
				CPMSDepositInfo infoDeposit; 
				BookingRecord.GetDepositInfo( n, infoDeposit );

				COleDateTime timeDeposit = COleDateTime( 2012, 1, 1, infoDeposit.GetHour(), infoDeposit.GetMinute(), 0 );

				if ( CheckTime( infoDeposit.GetPMSDay(), timeDeposit ) == TRUE )
				{
					double dDeposit = infoDeposit.GetValue();
					
					if ( infoDeposit.GetRefundFlag() == TRUE )
						dDeposit *= -1;
				
					if ( CPriceHelpers::CompareDoubles( dDeposit, 0.0, 3 ) != 0 )
					{
						CEposReportConsolPMSPayments infoConsol;
						infoConsol.m_nPaymentNo = infoDeposit.GetPaymentType();
						infoConsol.m_dDeposit = dDeposit;
						infoConsol.m_dAccount = 0.0;
						arrayPayments.Consolidate( infoConsol );
					}
				}
			}
		}
		
		if ( TRUE == m_bIncludePayment )
		{
			for ( int nAccountIdx = 0; nAccountIdx < BookingRecord.GetAccountCount(); nAccountIdx++ )
			{
				CPMSInvoiceArray InvoiceArray;
				InvoiceArray.BuildInvoiceArray( BookingRecord, nAccountIdx + 1, PMS_INVOICETYPE_EDIT_PAYMENTS, FALSE );

				for ( int nItemIdx = 0; nItemIdx < InvoiceArray.GetInvoiceItemCount(); nItemIdx++ )
				{
					CPMSInvoiceLineInfo InvoiceItem;
					InvoiceArray.GetInvoiceItem( nItemIdx, InvoiceItem );

					if ( PMS_LINE_PAYMENT == InvoiceItem.m_nLineType )
					{	
						int nPMSDay = GetDayNumberSince2011( InvoiceItem.m_dateSale );
						COleDateTime timePayment ( 2012, 1, 1, InvoiceItem.m_dateSale.GetHour(), InvoiceItem.m_dateSale.GetMinute(), 0 ); 
						
						if ( CheckTime( nPMSDay, timePayment ) == TRUE )
						{
							CEposReportConsolPMSPayments infoConsol;
							infoConsol.m_nPaymentNo = (int) InvoiceItem.m_nBasePluNo;
							infoConsol.m_dAccount = InvoiceItem.m_dValue;
							infoConsol.m_dDeposit = 0.0;
							arrayPayments.Consolidate( infoConsol );
						}
					}
				}
			}
		}
	}

	double dDepositTotal = 0.0;
	double dAccountTotal = 0.0;

	for ( int n = 0; n < arrayPayments.GetSize(); n++ )
	{
		CString strPaymentType;
		CString strPaymentName;

		CEposReportConsolPMSPayments infoConsol;
		arrayPayments.GetAt( n, infoConsol );
		
		int nPaymentIdx;
		if ( DataManager.Payment.FindPaymentByNumber( infoConsol.m_nPaymentNo, nPaymentIdx ) == TRUE )
		{
			CPaymentCSVRecord Payment;
			DataManager.Payment.GetAt( nPaymentIdx, Payment );
			strPaymentName = Payment.GetDisplayName();
		}
		else
			strPaymentName = "Unknown";

		strPaymentType.Format( "%d", infoConsol.m_nPaymentNo );

		CCSVReportLine csv;
		csv.AppendString( strPaymentType );
		csv.AppendString( strPaymentName );

		if ( TRUE == m_bIncludeDeposit )
			csv.AppendVal( infoConsol.m_dDeposit, 2 );
		
		if ( TRUE == m_bIncludePayment )
			csv.AppendVal( infoConsol.m_dAccount, 2 );
		
		if ( ( TRUE == m_bIncludeDeposit ) && ( TRUE == m_bIncludePayment ) )
			csv.AppendVal( infoConsol.m_dDeposit + infoConsol.m_dAccount, 2 );
		
		m_ReportFile.WriteLine( csv.GetLine() );

		dDepositTotal += infoConsol.m_dDeposit;
		dAccountTotal += infoConsol.m_dAccount;
	}

	m_ReportFile.WriteLine( "\t\t<LI>\t<LI>\t<LI>" );

	{
		CCSVReportLine csv;
		csv.AppendString( "Total" );
		csv.AppendString( "" );

		if ( TRUE == m_bIncludeDeposit )
			csv.AppendVal( dDepositTotal, 2 );
		
		if ( TRUE == m_bIncludePayment )
			csv.AppendVal( dAccountTotal, 2 );
		
		if ( ( TRUE == m_bIncludeDeposit ) && ( TRUE == m_bIncludePayment ) )
			csv.AppendVal( dDepositTotal + dAccountTotal, 2 );
		
		m_ReportFile.WriteLine( csv.GetLine() );
	}

	StatusProgress.Unlock();

	m_ReportFile.WriteLine( "" );	
	m_ReportFile.Close();

	return TRUE;
}

/**********************************************************************/
#endif
/**********************************************************************/
