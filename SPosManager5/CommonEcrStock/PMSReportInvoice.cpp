/**********************************************************************/
#ifdef COMPILE_PMS 
/**********************************************************************/
#include "CCSVReportLine.h"
#include "PMSOptions.h"
#include "PMSRoomStatusTable.h"
#include "PriceHelpers.h"
#include "ReportTypes.h"
#include "StatusProgress.h"
/**********************************************************************/
#include "PMSReportInvoice.h"
/**********************************************************************/

CPMSReportInvoice::CPMSReportInvoice( int nRoomGridRoomIdx, int nRoomGridAccountIdx, int nInvoiceType, bool bPendingBooking )
{
	m_bPendingBooking = bPendingBooking;

	PMSRoomStatusTable.GetRoomInfo( nRoomGridRoomIdx, m_infoRoom );
	PMSRoomStatusTable.GetAccountInfo( nRoomGridRoomIdx, nRoomGridAccountIdx, m_infoAccount );
	m_nAccountIdx = nRoomGridAccountIdx;

	m_nReportType = REPORT_TYPE_PMS_TRANSACTION;
	m_nInvoiceType = nInvoiceType;

	CPMSBookingCSVRecord BookingRecordInvoice;
	DataManagerNonDb.PMSInvoiceBookingBuffer.GetInvoiceBooking( BookingRecordInvoice );

	bool bDBBCredit = FALSE;
	if ( BookingRecordInvoice.GetRoomRateType() == PMS_BOOKING_RATE_DBB ) 
		if ( 0 == nRoomGridAccountIdx )
			bDBBCredit = TRUE;

	m_arrayInvoiceItems.BuildInvoiceArray( BookingRecordInvoice, nRoomGridAccountIdx + 1, m_nInvoiceType, bDBBCredit );
}

/**********************************************************************/

void CPMSReportInvoice::Consolidate()
{
}

/**********************************************************************/

const char* CPMSReportInvoice::GetParamsFilename()
{
	CFilenameUpdater FnUp( SysFiles::EcrReportPrm, 1, m_nReportType );
	m_strParamsFilename = FnUp.GetFilenameToUse();
	return m_strParamsFilename;
}

/**********************************************************************/

bool CPMSReportInvoice::CreateReport()
{
	if ( m_ReportFile.Open ( Super.ReportFilename() ) == FALSE )
		return FALSE;

	//GET BOOKING RECORD FOR INVOICE AND REFERENCE LINE
	CPMSBookingCSVRecord BookingRecordInvoice, BookingRecordDatabase;
	DataManagerNonDb.PMSInvoiceBookingBuffer.GetInvoiceBooking( BookingRecordInvoice );
	DataManagerNonDb.PMSInvoiceBookingBuffer.GetDatabaseBooking( BookingRecordDatabase );

	//CREATE INVOICE NUMBER FOR BOOKING IF REQUIRED
	if ( BookingRecordInvoice.GetBookingStatus() == PMS_BOOKING_STATUS_COMPLETE )
	{
		if ( BookingRecordDatabase.UpdateInvoiceNum() == TRUE )
		{
			BookingRecordInvoice.SetInvoiceNum( BookingRecordDatabase.GetInvoiceNum() );
			DataManagerNonDb.PMSInvoiceBookingBuffer.SetDatabaseBooking( BookingRecordDatabase );
			DataManagerNonDb.PMSInvoiceBookingBuffer.SetInvoiceBooking( BookingRecordInvoice );
			DataManagerNonDb.PMSBooking.InsertAndSaveBooking( BookingRecordDatabase );
		}
	}

	int nBlankLines = PMSOptions.GetInvoiceHeaderLines();
	if ( ( nBlankLines >= 1 ) && ( nBlankLines <= 20 ) )
	{
		for ( int n = 0; n < nBlankLines; n++ )
			m_ReportFile.SetHeader( "" );
	}
	
	//m_ReportFile.SetStyle1 ( "Guest Account" );
	m_ReportFile.SetTitle ( "Guest Account" );

	int nBlanks = 0;

	switch( m_nInvoiceType )
	{
	case PMS_INVOICETYPE_TRANSACTION:
		nBlanks = 1;
		m_ReportFile.AddColumn ( "", TA_LEFT, 850 );	//BLANK
		m_ReportFile.AddColumn ( "", TA_LEFT, 500 );	//TYPE
		m_ReportFile.AddColumn ( "", TA_LEFT, 350 );	//TIME
		m_ReportFile.AddColumn ( "", TA_RIGHT, 200 );	//VALUE
		break;

	case PMS_INVOICETYPE_GROUP:
		nBlanks = 3;
		m_ReportFile.AddColumn ( "", TA_LEFT, 350 );	//TIME		
		m_ReportFile.AddColumn ( "", TA_LEFT, 250 );	//GROUP NO
		m_ReportFile.AddColumn ( "", TA_LEFT, 850 );	//DESCRIPTION
		m_ReportFile.AddColumn ( "", TA_LEFT, 100 );	//BLANK
		m_ReportFile.AddColumn ( "", TA_RIGHT, 150 );	//QTY
		m_ReportFile.AddColumn ( "", TA_RIGHT, 200 );	//VALUE
		break;

	case PMS_INVOICETYPE_ITEM:
	default:
		nBlanks = 4;
		m_ReportFile.AddColumn ( "", TA_LEFT, 350 );	//TIME		
		m_ReportFile.AddColumn ( "", TA_LEFT, 250 );	//TYPE
		m_ReportFile.AddColumn ( "", TA_LEFT, 350 );	//PLU NUMBER
		m_ReportFile.AddColumn ( "", TA_LEFT, 500 );	//DESCRIPTION
		m_ReportFile.AddColumn ( "", TA_LEFT, 100 );	//BLANK
		m_ReportFile.AddColumn ( "", TA_RIGHT, 150 );	//QTY
		m_ReportFile.AddColumn ( "", TA_RIGHT, 200 );	//VALUE
		break;
	}

	CString strFullLine = "<..>";

	//HOTEL ADDRESS
	if ( PMSOptions.GetInvoiceShowAddressFlag() == TRUE )
	{
		CCSVReportLine csv;
		csv.AppendBlanks ( nBlanks );
		csv.AppendString ( strFullLine + PMSOptions.GetHotelName() );
		m_ReportFile.WriteLine ( csv.GetLine() );

		csv.RemoveAll();
		csv.AppendBlanks ( nBlanks );
		csv.AppendString ( strFullLine + PMSOptions.GetHotelAddress1() );
		m_ReportFile.WriteLine ( csv.GetLine() );

		csv.RemoveAll();
		csv.AppendBlanks ( nBlanks );
		csv.AppendString ( strFullLine + PMSOptions.GetHotelAddress2() );
		m_ReportFile.WriteLine ( csv.GetLine() );

		csv.RemoveAll();
		csv.AppendBlanks ( nBlanks );
		csv.AppendString ( strFullLine + PMSOptions.GetHotelAddress3() );
		m_ReportFile.WriteLine ( csv.GetLine() );

		csv.RemoveAll();
		csv.AppendBlanks ( nBlanks );
		csv.AppendString( strFullLine + PMSOptions.GetHotelPostcode() );
		m_ReportFile.WriteLine ( csv.GetLine() );

		bool bWantBlankLine = TRUE;

		CString strPhone = PMSOptions.GetHotelPhone();
		::TrimSpaces( strPhone, FALSE );

		if ( strPhone != "" )
		{
			CString strStart = strFullLine + "T : ";

			csv.RemoveAll();
			csv.AppendBlanks ( nBlanks );
			csv.AppendString ( strStart + strPhone );
			m_ReportFile.WriteLine( "" );
			m_ReportFile.WriteLine ( csv.GetLine() );
			bWantBlankLine = FALSE;
		}

		CString strFax = PMSOptions.GetHotelFax();
		::TrimSpaces( strFax, FALSE );

		if ( strFax != "" )
		{
			if ( TRUE == bWantBlankLine )
			{
				m_ReportFile.WriteLine( "" );
				bWantBlankLine = FALSE;
			}

			CString strStart = strFullLine + "F : ";

			csv.RemoveAll();
			csv.AppendBlanks ( nBlanks );
			csv.AppendString ( strStart + strFax );
			m_ReportFile.WriteLine ( csv.GetLine() );
		}

		CString strEmail = PMSOptions.GetHotelEmail();
		::TrimSpaces( strEmail, FALSE );

		if ( strEmail != "" )
		{
			if ( TRUE == bWantBlankLine )
			{
				m_ReportFile.WriteLine( "" );
				bWantBlankLine = FALSE;
			}

			CString strStart = strFullLine + "E : ";

			csv.RemoveAll();
			csv.AppendBlanks ( nBlanks );
			csv.AppendString ( strStart + strEmail );
			m_ReportFile.WriteLine ( csv.GetLine() );
		}

		CString strTaxNo = PMSOptions.GetHotelTaxNo();
		::TrimSpaces( strTaxNo, FALSE );

		if ( strTaxNo != "" )
		{
			CString strStart = EcrmanOptions.GetTaxString( strFullLine + "%T : " );

			csv.RemoveAll();
			csv.AppendBlanks ( nBlanks );
			csv.AppendString ( strStart + strTaxNo );
			m_ReportFile.WriteLine( "" );
			m_ReportFile.WriteLine ( csv.GetLine() );
		}

		m_ReportFile.WriteLine ( "" );
	}

	int nCustomerId;
	if ( FALSE == m_bPendingBooking )
		nCustomerId = m_infoAccount.GetCustomerId();
	else
		nCustomerId = BookingRecordInvoice.GetCustomerId();

	//BILLING ADDRESS
	int nCustomerIdx;
	CPMSCustomerCSVRecord Customer;
	if ( DataManagerNonDb.PMSCustomer.FindCustomerById( nCustomerId, nCustomerIdx ) == TRUE )
		DataManagerNonDb.PMSCustomer.GetAt( nCustomerIdx, Customer );
	
	CString strBillingName = Customer.GetBillingName();
	::TrimSpaces( strBillingName, FALSE );

	CString strGuestName = Customer.GetGuestName();
	::TrimSpaces( strGuestName, FALSE );

	if ( strGuestName == strBillingName )
		strBillingName = "";
	
	m_ReportFile.WriteLine( "" );

	if ( strBillingName != "" )	
		m_ReportFile.WriteLine( strFullLine + strBillingName );
	else if ( strGuestName != "" )	
		m_ReportFile.WriteLine( strFullLine + strGuestName );

	m_ReportFile.WriteLine( strFullLine + Customer.GetAddress1() );
	m_ReportFile.WriteLine( strFullLine + Customer.GetAddress2() );
	m_ReportFile.WriteLine( strFullLine + Customer.GetAddress3() );
	m_ReportFile.WriteLine( strFullLine + Customer.GetPostcode() );

	//DATE TODAY
	CTime time = CTime::GetCurrentTime();
	CString strTime = time.Format ( "%A %B %d %Y" );
	m_ReportFile.WriteLine( "" );
	m_ReportFile.WriteLine ( strFullLine + strTime );

	//INVOICE LINE
	CString strInvoice = "";
	if ( ( PMSOptions.GetInvoiceHideNumberFlag() == FALSE ) && ( BookingRecordInvoice.GetTrainingFlag() == FALSE ) )
	{
		if ( BookingRecordInvoice.GetBookingStatus() == PMS_BOOKING_STATUS_COMPLETE )
		{
			if ( m_nAccountIdx == 0 )
				strInvoice.Format ( "<..>Invoice no : %8.8d", BookingRecordInvoice.GetInvoiceNum() );
			else
				strInvoice.Format ( "<..>Invoice no : %8.8d-%d", BookingRecordInvoice.GetInvoiceNum(), m_nAccountIdx + 1 );
		}
		else if ( ( PMSOptions.GetInvoiceHideInterimFlag() == FALSE ) && ( FALSE == m_bPendingBooking ) )
			strInvoice = "<..>Interim invoice";
	}

	if ( strInvoice != "" )
	{	
		CCSVReportLine csv;
		
		switch( m_nInvoiceType )
		{
		case PMS_INVOICETYPE_TRANSACTION:
			csv.AppendBlanks(2);
			break;

		case PMS_INVOICETYPE_GROUP:
			csv.AppendBlanks(3);
			break;

		case PMS_INVOICETYPE_ITEM:
		default:
			csv.AppendBlanks(4);
		}

		csv.AppendString( strInvoice );
		m_ReportFile.WriteLine( csv.GetLine() );
	}
	else
		m_ReportFile.WriteLine( "" );

	//REFERENCE LINE
	CString strRoomName;
	::GetPMSRoomText( m_infoRoom.GetRoomNo(), m_infoRoom.GetRoomName(), strRoomName );

	CString strReference = "<..>";

	if ( ( PMSOptions.GetInvoiceHideReferenceFlag() == FALSE ) && ( BookingRecordInvoice.GetTrainingFlag() == FALSE ) )
	{
		CString strRef;
		strRef.Format ( "Ref : %8.8d / ",
			BookingRecordInvoice.GetBookingId() );
		strReference += strRef;
	}

	strReference += strRoomName;

	if ( ( strBillingName != "" ) && ( strGuestName != "" ) )
	{
		strReference += " / ";
		strReference += strGuestName;
	}

	if ( PMSOptions.GetInvoiceShowNoteFlag() == TRUE )
	{
		CString strNote = BookingRecordInvoice.GetBookingNote();
		if ( strNote != "" )
		{
			strReference += " / ";
			strReference += strNote;
		}
	}

	m_ReportFile.WriteLine( strReference );

	//DATES OF STAY
	if ( PMSOptions.GetInvoiceHideDatesOfStayFlag() == FALSE )
	{
		COleDateTime dateStayStart, dateStayEnd;

		CPMSBookingPartInfo infoPart;
		BookingRecordInvoice.GetPartInfo( 0, infoPart );
		GetOleDateSince2011( infoPart.GetStartDay(), dateStayStart );
		
		switch( BookingRecordInvoice.GetBookingStatus() )
		{
		case PMS_BOOKING_STATUS_COMPLETE:
		case PMS_BOOKING_STATUS_PENDING:
			BookingRecordInvoice.GetFinalPartInfo( infoPart );
			GetOleDateSince2011( infoPart.GetStartDay() + infoPart.GetNights(), dateStayEnd );
			break;

		default:
			GetOleDateSince2011( PMSOptions.GetPMSDayNumber(), dateStayEnd );
			break;
		}

		CString strDates;
		strDates.Format( "<..>Dates of stay : %2.2d/%2.2d/%4.4d - %2.2d/%2.2d/%4.4d",
			dateStayStart.GetDay(),
			dateStayStart.GetMonth(),
			dateStayStart.GetYear(),
			dateStayEnd.GetDay(),
			dateStayEnd.GetMonth(),
			dateStayEnd.GetYear() );
		
		m_ReportFile.WriteLine( "" );
		m_ReportFile.WriteLine( strDates );
	}

	//TRANSACTIONS
	StatusProgress.Lock( TRUE, "Creating report" );

	switch( m_nInvoiceType )
	{
	case PMS_INVOICETYPE_TRANSACTION:
		CreateTransactionInvoice();
		break;

	case PMS_INVOICETYPE_GROUP:
		CreateGroupInvoice();
		break;

	case PMS_INVOICETYPE_ITEM:
	default:
		CreateItemInvoice();
		break;
	}

	StatusProgress.Unlock();

	m_ReportFile.WriteLine( "" );
	m_ReportFile.Close();
	
	return TRUE;
}

/**********************************************************************/

void CPMSReportInvoice::CreateItemInvoice()
{
	m_ReportFile.WriteLine ( "" );
	m_ReportFile.WriteLine ( "<LI>" );
	m_ReportFile.WriteLine ( "Time\tType\tPlu Number\tDescription\t\tQty\tTotal" );
	m_ReportFile.WriteLine ( "<LI>" );
	
	double dGrandTotal = 0.0;
	double dSalesTotal = 0.0;
	double dAllowanceTotal = 0.0;
	CString strDisplayDate = "";
	bool bWrittenItem = FALSE;
	
	for ( int n = 0; n < m_arrayInvoiceItems.GetInvoiceItemCount(); n++ )
	{
		StatusProgress.SetPos( n, m_arrayInvoiceItems.GetInvoiceItemCount() );

		CPMSInvoiceLineInfo infoSale;
		m_arrayInvoiceItems.GetInvoiceItem( n, infoSale );

		if ( PMS_LINE_SALE == infoSale.m_nLineType )
		{
			__int64 nPluNo = infoSale.m_nBasePluNo;

			if (	( PMSOptions.GetEposReportRoomOnlyPlu() == nPluNo ) ||
					( PMSOptions.GetEposReportRoomBBPlu() == nPluNo ) ||
					( PMSOptions.GetEposReportRoomDBBPlu() == nPluNo ) )
			{
				infoSale.m_nLineType = PMS_LINE_ACCOM;
			}
		}

		switch( infoSale.m_nLineType )
		{
		case PMS_LINE_TRAN:
			if ( FALSE == m_bPendingBooking )
			{
				strDisplayDate.Format ( "%2.2d/%2.2d/%4.4d  %2.2d:%2.2d",
					infoSale.m_dateSale.GetDay(),
					infoSale.m_dateSale.GetMonth(),
					infoSale.m_dateSale.GetYear(),
					infoSale.m_dateSale.GetHour(),
					infoSale.m_dateSale.GetMinute() );
			}
			break;

		case PMS_LINE_SALE:
			if ( FALSE == m_bPendingBooking )
			{
				if ( CPriceHelpers::CompareDoubles( infoSale.m_dValue, 0.0, 3 ) != 0 )
				{
					CCSV csv( '\t' );
					csv.Add( strDisplayDate );
					csv.Add( "SALE" );
					csv.Add( infoSale.m_nBasePluNo );
					csv.Add( infoSale.m_strDescription );
					csv.Add( "" );
					csv.Add( infoSale.m_dBasePluQty, 2 );
					csv.Add( GetCurrencyString ( infoSale.m_dValue ) );
					m_ReportFile.WriteLine( csv.GetLine() );
					bWrittenItem = TRUE;

					dGrandTotal += infoSale.m_dValue;
					dSalesTotal += infoSale.m_dValue;
				}

				if ( CPriceHelpers::CompareDoubles( infoSale.m_dDiscount, 0.0, 3 ) != 0 )
				{
					CCSV csv( '\t' );
					csv.Add( strDisplayDate );
					csv.Add( "DISCOUNT" );
					csv.Add( infoSale.m_nBasePluNo );
					csv.Add( infoSale.m_strDescription );
					csv.Add( "" );
					csv.Add( "" );
					csv.Add( GetCurrencyString ( -infoSale.m_dDiscount ) );
					m_ReportFile.WriteLine( csv.GetLine() );
					bWrittenItem = TRUE;

					dGrandTotal -= infoSale.m_dDiscount;
					dSalesTotal -= infoSale.m_dDiscount;
				}
			}
			break;

		case PMS_LINE_DINNER:
			if ( FALSE == m_bPendingBooking )
			{
				if ( CPriceHelpers::CompareDoubles( infoSale.m_dValue, 0.0, 3 ) != 0 )
				{
					CCSV csv( '\t' );
					csv.Add( strDisplayDate.Left(10) );
					csv.Add( "" );
					csv.Add( "" );
					csv.Add( infoSale.m_strDescription );
					csv.Add( "" );
					csv.Add( "" );
					csv.Add( GetCurrencyString ( infoSale.m_dValue ) );
					m_ReportFile.WriteLine( csv.GetLine() );
					bWrittenItem = TRUE;

					dGrandTotal += infoSale.m_dValue;
					dSalesTotal += infoSale.m_dValue;
				}
			}
			break;

		case PMS_LINE_ACCOM:
		case PMS_LINE_EXTRABED:
		case PMS_LINE_EXTRACOT:
		case PMS_LINE_BREAKFAST:
			if ( FALSE == m_bPendingBooking )
			{
				if ( CPriceHelpers::CompareDoubles( infoSale.m_dValue, 0.0, 3 ) != 0 )
				{
					CCSV csv( '\t' );
					csv.Add( "" );
					csv.Add( "" );
					csv.Add( "" );
					csv.Add( infoSale.m_strDescription );
					csv.Add( "" );
					csv.Add( infoSale.m_dBasePluQty, 0 );
					csv.Add( GetCurrencyString ( infoSale.m_dValue ) );
					m_ReportFile.WriteLine( csv.GetLine() );
					bWrittenItem = TRUE;

					dGrandTotal += infoSale.m_dValue;
					dSalesTotal += infoSale.m_dValue;
				}

				if ( CPriceHelpers::CompareDoubles( infoSale.m_dDiscount, 0.0, 3 ) != 0 )
				{
					CCSV csv( '\t' );
					csv.Add( "" );
					csv.Add( "DISCOUNT" );
					csv.Add( "" );
					csv.Add( infoSale.m_strDescription );
					csv.Add( "" );
					csv.Add( "" );
					csv.Add( GetCurrencyString ( -infoSale.m_dDiscount ) );
					m_ReportFile.WriteLine( csv.GetLine() );
					bWrittenItem = TRUE;

					dGrandTotal -= infoSale.m_dDiscount;
					dSalesTotal -= infoSale.m_dDiscount;
				}
			}
			break;

		case PMS_LINE_REFUND:
			if ( FALSE == m_bPendingBooking )
			{
				CCSV csv( '\t' );
				csv.Add( strDisplayDate );
				csv.Add( "REFUND" );
				csv.Add( infoSale.m_nBasePluNo );
				csv.Add( infoSale.m_strDescription );
				csv.Add( "" );
				csv.Add( infoSale.m_dBasePluQty, 2 );
				csv.Add( GetCurrencyString ( -infoSale.m_dValue ) );
				m_ReportFile.WriteLine( csv.GetLine() );
				bWrittenItem = TRUE;

				dGrandTotal -= infoSale.m_dValue;
				dSalesTotal -= infoSale.m_dValue;
			}
			break;
		
		case PMS_LINE_ALLOWANCE:
			if ( FALSE == m_bPendingBooking )
			{
				CCSV csv( '\t' );
				csv.Add( strDisplayDate );
				csv.Add( "ALLOWANCE" );
				csv.Add( infoSale.m_nBasePluNo );
				csv.Add( infoSale.m_strDescription );
				csv.Add( "" );
				csv.Add( infoSale.m_dBasePluQty, 2 );
				csv.Add( GetCurrencyString ( -infoSale.m_dAllowance ) );
				m_ReportFile.WriteLine( csv.GetLine() );
				bWrittenItem = TRUE;

				dAllowanceTotal += infoSale.m_dAllowance;
			}
			break;
		
		case PMS_LINE_SDISC:
			if ( FALSE == m_bPendingBooking )
			{
				CCSV csv( '\t' );
				csv.Add( strDisplayDate );
				csv.Add( "DISCOUNT" );
				csv.Add( "" );
				csv.Add( "(Subtotal)" );
				csv.Add( "" );
				csv.Add( "" );
				csv.Add( GetCurrencyString ( -infoSale.m_dDiscount ) );
				m_ReportFile.WriteLine( csv.GetLine() );
				bWrittenItem = TRUE;

				dGrandTotal -= infoSale.m_dDiscount;
				dSalesTotal -= infoSale.m_dDiscount;
			}
			break;

		case PMS_LINE_LDISC:
			if ( FALSE == m_bPendingBooking )
			{
				CCSV csv( '\t' );
				csv.Add( strDisplayDate );
				csv.Add( "DISCOUNT" );
				csv.Add( "" );
				csv.Add( "(Loyalty)" );
				csv.Add( "" );
				csv.Add( "" );
				csv.Add( GetCurrencyString ( -infoSale.m_dDiscount ) );
				m_ReportFile.WriteLine( csv.GetLine() );
				bWrittenItem = TRUE;

				dGrandTotal -= infoSale.m_dDiscount;
				dSalesTotal -= infoSale.m_dDiscount;
			}
			break;

		case PMS_LINE_PAYMENT:
		case PMS_LINE_DEPOSIT:
			{
				CCSV csv( '\t' );

				if ( PMS_LINE_DEPOSIT == infoSale.m_nLineType )
				{
					csv.Add( strDisplayDate.Left(10) );
					csv.Add( "DEPOSIT" );
				}
				else
				{
					csv.Add( strDisplayDate );
					csv.Add( "PAYMENT" );
				}

				csv.Add( "" );
				csv.Add( infoSale.m_strDescription );
				csv.Add( "" );
				csv.Add( "" );
				csv.Add( GetCurrencyString ( infoSale.m_dValue ) );
				m_ReportFile.WriteLine( csv.GetLine() );
				bWrittenItem = TRUE;
			}
			break;
		}
	}

	if ( TRUE == bWrittenItem )
	{
		CCSVReportLine csv;
		csv.AppendBlanks ( 6 );
		csv.AppendString( "<LI>" );
		m_ReportFile.WriteLine( csv.GetLine() );
	}

	if ( FALSE == m_bPendingBooking )
	{
		CCSVReportLine csv;
		csv.AppendBlanks ( 4 );
		csv.AppendString( "<..>Sales Total" );
		csv.AppendBlanks ( 1 );
		csv.AppendString( GetCurrencyString ( dGrandTotal ) );
		m_ReportFile.WriteLine( csv.GetLine() );
	}

	if ( ( CPriceHelpers::CompareDoubles( dAllowanceTotal, 0.0, 3 ) != 0 ) && ( FALSE == m_bPendingBooking ) )
	{
		{
			CCSVReportLine csv;
			csv.AppendBlanks ( 4 );
			csv.AppendString( "<..>Allowances" );
			csv.AppendBlanks ( 1 );
			csv.AppendString( GetCurrencyString (-dAllowanceTotal ) );
			m_ReportFile.WriteLine( csv.GetLine() );
		}

		dGrandTotal -= dAllowanceTotal;

		{
			CCSVReportLine csv;
			csv.AppendBlanks ( 4 );
			csv.AppendString( "<..>Subtotal" );
			csv.AppendBlanks ( 1 );
			csv.AppendString( GetCurrencyString( dGrandTotal ) );
			m_ReportFile.WriteLine( csv.GetLine() );
		}
	}

	if ( FALSE == m_bPendingBooking )
	{
		CString strTaxLabel = "";
		strTaxLabel += "<..>";
		strTaxLabel += EcrmanOptions.GetTaxContentString();
	
		CCSVReportLine csv;
		csv.AppendBlanks ( 4 );
		csv.AppendString( strTaxLabel );
		csv.AppendBlanks( 1 );
		csv.AppendString( GetCurrencyString ( m_arrayInvoiceItems.GetInvoiceTaxContent() ) );
		m_ReportFile.WriteLine( csv.GetLine() );
	}

	if ( FALSE == m_bPendingBooking )
	{
		CCSVReportLine csv;
		csv.AppendBlanks ( 4 );
		csv.AppendString( "<..>Net Total" );
		csv.AppendBlanks( 1 );
		csv.AppendString( GetCurrencyString ( dGrandTotal - m_arrayInvoiceItems.GetInvoiceTaxContent() ) );
		m_ReportFile.WriteLine( csv.GetLine() );
	}

	if ( m_arrayInvoiceItems.GetPaymentTotalCount() > 0 )
	{
		CString strFullLine = "<..>";

		bool bWrittenPayment = FALSE;
		for ( int n = 0; n < m_arrayInvoiceItems.GetPaymentTotalCount(); n++ )
		{	
			CPMSInvoiceLineInfo infoPayment;
			m_arrayInvoiceItems.GetPaymentTotal( n, infoPayment );

			if ( CPriceHelpers::CompareDoubles( infoPayment.m_dValue, 0.0, 3 ) != 0 )
			{
				CCSVReportLine csv;
				csv.AppendBlanks ( 3 );

				if ( PMS_LINE_DEPOSIT == infoPayment.m_nLineType )
					csv.AppendString( "Deposit" );
				else
					csv.AppendString( "Payment Total" );

				csv.AppendString( strFullLine + infoPayment.m_strDescription );
				csv.AppendBlanks( 1 );
				csv.AppendString( GetCurrencyString ( infoPayment.m_dValue ) );

				if ( FALSE == bWrittenPayment )
					m_ReportFile.WriteLine( "" );

				m_ReportFile.WriteLine( csv.GetLine() );

				dGrandTotal -= infoPayment.m_dValue;
				bWrittenPayment = TRUE;
			}
		}
	}

	{
		CCSVReportLine csv;
		csv.AppendBlanks ( 6 );
		csv.AppendString( "<LI>" );
		m_ReportFile.WriteLine( csv.GetLine() );
	}

	CCSVReportLine csv;
	csv.AppendBlanks ( 4 );
		
	switch( CPriceHelpers::CompareDoubles( dGrandTotal, 0.0, 2 ) )
	{
	case 1:
		csv.AppendString( "<..>Balance" );
		csv.AppendBlanks( 1 );
		csv.AppendString( GetCurrencyString ( dGrandTotal, TRUE ) );
		break;

	case -1:
		csv.AppendString( "<..>Credit" );
		csv.AppendBlanks( 1 );
		csv.AppendString( GetCurrencyString ( -dGrandTotal, TRUE ) );
		break;

	case 0:
	default:
		csv.AppendString( "<..>Balance" );
		csv.AppendBlanks( 1 );
		csv.AppendString( GetCurrencyString ( 0.0, TRUE ) );
		break;
	}

	m_ReportFile.WriteLine( csv.GetLine() );
}

/**********************************************************************/

void CPMSReportInvoice::CreateGroupInvoice()
{
	m_ReportFile.WriteLine ( "" );
	m_ReportFile.WriteLine ( "<LI>" );
	m_ReportFile.WriteLine ( "Date\tType\tGroup\t\tQty\tTotal" );
	m_ReportFile.WriteLine ( "<LI>" );
	
	double dGrandTotal = 0.0;
	CString strDisplayDate = "";
	bool bWrittenItem = FALSE;
	
	for ( int n = 0; n < m_arrayInvoiceItems.GetInvoiceItemCount(); n++ )
	{
		StatusProgress.SetPos( n, m_arrayInvoiceItems.GetInvoiceItemCount() );

		CPMSInvoiceLineInfo infoSale;
		m_arrayInvoiceItems.GetInvoiceItem( n, infoSale );

		switch( infoSale.m_nLineType )
		{
		case PMS_LINE_TRAN:
			if ( FALSE == m_bPendingBooking )
			{
				strDisplayDate.Format ( "%2.2d/%2.2d/%4.4d",
					infoSale.m_dateSale.GetDay(),
					infoSale.m_dateSale.GetMonth(),
					infoSale.m_dateSale.GetYear() );
			}
			break;

		case PMS_LINE_ACCOM:
		case PMS_LINE_EXTRABED:
		case PMS_LINE_EXTRACOT:
		case PMS_LINE_BREAKFAST:
			if ( FALSE == m_bPendingBooking )
			{
				if ( CPriceHelpers::CompareDoubles( infoSale.m_dValue, 0.0, 3 ) != 0 )
				{
					CCSV csv( '\t' );
					csv.Add( "" );
					csv.Add( "" );
					csv.Add( infoSale.m_strDescription );
					csv.Add( "" );
					csv.Add( infoSale.m_dBasePluQty, 0 );
					csv.Add( GetCurrencyString ( infoSale.m_dValue ) );
					m_ReportFile.WriteLine( csv.GetLine() );
					bWrittenItem = TRUE;

					dGrandTotal += infoSale.m_dValue;
				}
			}
			break;

		case PMS_LINE_SALE:
			if ( FALSE == m_bPendingBooking )
			{
				if ( CPriceHelpers::CompareDoubles( infoSale.m_dValue, 0.0, 3 ) != 0 )
				{
					int nGroupNo = int ( infoSale.m_nBasePluNo );

					CString strGroupName = "";

					int nGroupIdx;
					if ( DataManager.EposGroup.FindGroupByNumber( nGroupNo, nGroupIdx ) == TRUE )
						strGroupName = DataManager.EposGroup.GetGroupReportText( nGroupNo );
					else
						strGroupName.Format( "Group %d", nGroupNo );

					CCSV csv( '\t' );
					csv.Add( strDisplayDate );
					csv.Add( "SALES" );
					csv.Add( strGroupName );
					csv.Add( "" );
					csv.Add( infoSale.m_dBasePluQty, 2 );
					csv.Add( GetCurrencyString ( infoSale.m_dValue ) );
					m_ReportFile.WriteLine( csv.GetLine() );
					bWrittenItem = TRUE;

					dGrandTotal += infoSale.m_dValue;
				}
			}
			break;

		case PMS_LINE_DINNER:
			if ( FALSE == m_bPendingBooking )
			{
				if ( CPriceHelpers::CompareDoubles( infoSale.m_dValue, 0.0, 3 ) != 0 )
				{
					CCSV csv( '\t' );
					csv.Add( strDisplayDate );
					csv.Add( "" );
					csv.Add( infoSale.m_strDescription );
					csv.Add( "" );
					csv.Add( "" );
					csv.Add( GetCurrencyString ( infoSale.m_dValue ) );
					m_ReportFile.WriteLine( csv.GetLine() );
					bWrittenItem = TRUE;

					dGrandTotal += infoSale.m_dValue;
				}
			}
			break;

		case PMS_LINE_SDISC:
			if ( FALSE == m_bPendingBooking )
			{
				CCSV csv( '\t' );
				csv.Add( strDisplayDate );
				csv.Add( "DISCOUNT" );
				csv.Add( "(Subtotal)" );
				csv.Add( "" );
				csv.Add( "" );
				csv.Add( GetCurrencyString ( -infoSale.m_dDiscount ) );
				m_ReportFile.WriteLine( csv.GetLine() );
				bWrittenItem = TRUE;

				dGrandTotal -= infoSale.m_dDiscount;
			}
			break;

		case PMS_LINE_LDISC:
			if ( FALSE == m_bPendingBooking )
			{
				CCSV csv( '\t' );
				csv.Add( strDisplayDate );
				csv.Add( "DISCOUNT" );
				csv.Add( "(Loyalty)" );
				csv.Add( "" );
				csv.Add( "" );
				csv.Add( GetCurrencyString ( -infoSale.m_dDiscount ) );
				m_ReportFile.WriteLine( csv.GetLine() );
				bWrittenItem = TRUE;

				dGrandTotal -= infoSale.m_dDiscount;
			}
			break;
		}
	}

	//ALLOWANCES
	double dAllowanceTotal = 0.0;

	if ( FALSE == m_bPendingBooking )
	{
		CString strFullLine = "<..>";

		bool bWrittenAllowance = FALSE;
		for ( int n = 0; n < m_arrayInvoiceItems.GetAllowanceTotalCount(); n++ )
		{
			CPMSInvoiceLineInfo infoAllowance;
			m_arrayInvoiceItems.GetAllowanceTotal( n, infoAllowance );

			if ( CPriceHelpers::CompareDoubles( infoAllowance.m_dAllowance, 0.0, 3 ) != 0 )
			{
				CCSVReportLine csv;
				csv.AppendBlanks ( 1 );
				csv.AppendString( "ALLOWANCE" );
				csv.AppendString( strFullLine + infoAllowance.m_strDescription );
				csv.AppendBlanks ( 2 );
				csv.AppendString( GetCurrencyString ( -infoAllowance.m_dAllowance ) );

				if ( ( FALSE == bWrittenAllowance ) && ( TRUE == bWrittenItem ) )
					m_ReportFile.WriteLine( "" );

				m_ReportFile.WriteLine( csv.GetLine() );
				
				bWrittenItem = TRUE;
				bWrittenAllowance = TRUE;
				dAllowanceTotal += infoAllowance.m_dAllowance;
			}
		}
	}

	if ( TRUE == bWrittenItem )
	{
		CCSVReportLine csv;
		csv.AppendBlanks ( 5 );
		csv.AppendString( "<LI>" );
		m_ReportFile.WriteLine( csv.GetLine() );
	}

	if ( FALSE == m_bPendingBooking )
	{
		CCSVReportLine csv;
		csv.AppendBlanks ( 3 );
		csv.AppendString( "<..>Sales Total" );
		csv.AppendBlanks ( 1 );
		csv.AppendString( GetCurrencyString ( dGrandTotal ) );
		m_ReportFile.WriteLine( csv.GetLine() );
	}

	if ( ( CPriceHelpers::CompareDoubles( dAllowanceTotal, 0.0, 3 ) != 0 ) && ( FALSE == m_bPendingBooking ) )
	{
		{
			CCSVReportLine csv;
			csv.AppendBlanks ( 3 );
			csv.AppendString( "<..>Allowances" );
			csv.AppendBlanks ( 1 );
			csv.AppendString( GetCurrencyString ( -dAllowanceTotal ) );
			m_ReportFile.WriteLine( csv.GetLine() );
		}

		dGrandTotal -= dAllowanceTotal;

		{
			CCSVReportLine csv;
			csv.AppendBlanks ( 3 );
			csv.AppendString( "<..>Subtotal" );
			csv.AppendBlanks ( 1 );
			csv.AppendString( GetCurrencyString ( dGrandTotal ) );
			m_ReportFile.WriteLine( csv.GetLine() );
		}
	}

	if ( FALSE == m_bPendingBooking )
	{
		CCSVReportLine csv;
		csv.AppendBlanks ( 3 );
		csv.AppendString( EcrmanOptions.GetTaxString( "<..>%T Content" ) );
		csv.AppendBlanks ( 1 );
		csv.AppendString( GetCurrencyString ( m_arrayInvoiceItems.GetInvoiceTaxContent() ) );
		m_ReportFile.WriteLine( csv.GetLine() );
	}

	if ( FALSE == m_bPendingBooking )
	{
		CCSVReportLine csv;
		csv.AppendBlanks ( 3 );
		csv.AppendString( "<..>Net Total" );
		csv.AppendBlanks ( 1 );
		csv.AppendString( GetCurrencyString ( dGrandTotal - m_arrayInvoiceItems.GetInvoiceTaxContent() ) );
		m_ReportFile.WriteLine( csv.GetLine() );
	}

	if ( m_arrayInvoiceItems.GetPaymentTotalCount() > 0 )
	{
		CString strFullLine = "<..>";

		bool bWrittenPayment = FALSE;
		for ( int n = 0; n < m_arrayInvoiceItems.GetPaymentTotalCount(); n++ )
		{	
			CPMSInvoiceLineInfo infoPayment;
			m_arrayInvoiceItems.GetPaymentTotal( n, infoPayment );

			if ( CPriceHelpers::CompareDoubles( infoPayment.m_dValue, 0.0, 3 ) != 0 )
			{
				CCSVReportLine csv;
				csv.AppendBlanks ( 2 );

				if ( PMS_LINE_DEPOSIT == infoPayment.m_nLineType )
					csv.AppendString( "Deposit" );
				else
					csv.AppendString( "Payment Total" );

				csv.AppendString( strFullLine + infoPayment.m_strDescription );
				csv.AppendBlanks ( 1 );
				csv.AppendString( GetCurrencyString ( infoPayment.m_dValue ) );

				if ( FALSE == bWrittenPayment )
					m_ReportFile.WriteLine( "" );

				m_ReportFile.WriteLine( csv.GetLine() );

				dGrandTotal -= infoPayment.m_dValue;
				bWrittenPayment = TRUE;
			}
		}
	}

	{
		CCSVReportLine csv;
		csv.AppendBlanks ( 5 );
		csv.AppendString( "<LI>" );
		m_ReportFile.WriteLine( csv.GetLine() );
	}

	CCSVReportLine csv;
	csv.AppendBlanks ( 3 );
	
	switch( CPriceHelpers::CompareDoubles( dGrandTotal, 0.0, 2 ) )
	{
	case 1:
		csv.AppendString( "<..>Balance" );
		csv.AppendBlanks( 1 );
		csv.AppendString( GetCurrencyString ( dGrandTotal, TRUE ) );
		break;

	case -1:
		csv.AppendString( "<..>Credit" );
		csv.AppendBlanks( 1 );
		csv.AppendString( GetCurrencyString ( -dGrandTotal, TRUE ) );
		break;

	case 0:
	default:
		csv.AppendString( "<..>Balance" );
		csv.AppendBlanks( 1 );
		csv.AppendString( GetCurrencyString ( 0.0, TRUE ) );
		break;
	}

	m_ReportFile.WriteLine( csv.GetLine() );
}

/**********************************************************************/

void CPMSReportInvoice::CreateTransactionInvoice()
{
	m_ReportFile.WriteLine ( "" );
	m_ReportFile.WriteLine ( "<LI>" );
	m_ReportFile.WriteLine ( "\t\tDate\tTotal" );
	m_ReportFile.WriteLine ( "<LI>" );
	
	double dGrandTotal = 0.0;
	CString strDisplayDate = "";
	bool bWrittenItem = FALSE;
	
	for ( int n = 0; n < m_arrayInvoiceItems.GetInvoiceItemCount(); n++ )
	{
		StatusProgress.SetPos( n, m_arrayInvoiceItems.GetInvoiceItemCount() );

		CPMSInvoiceLineInfo infoSale;
		m_arrayInvoiceItems.GetInvoiceItem( n, infoSale );
		
		switch( infoSale.m_nLineType )
		{
		case PMS_LINE_TRAN:
			if ( FALSE == m_bPendingBooking )
			{
				strDisplayDate.Format ( "%2.2d/%2.2d/%4.4d",
					infoSale.m_dateSale.GetDay(),
					infoSale.m_dateSale.GetMonth(),
					infoSale.m_dateSale.GetYear() );

				if ( ( CPriceHelpers::CompareDoubles( infoSale.m_dValue, 0.0, 3 ) != 0 ) || ( CPriceHelpers::CompareDoubles( infoSale.m_dAllowance, 0.0, 3 ) != 0 ) )
				{
					CCSV csv( '\t' );
					csv.Add( "" );
					csv.Add( "" );
					csv.Add( strDisplayDate );
					csv.Add( GetCurrencyString ( infoSale.m_dValue ) );
					m_ReportFile.WriteLine( csv.GetLine() );
					bWrittenItem = TRUE;

					dGrandTotal += infoSale.m_dValue;
				}
			}
			break;

		case PMS_LINE_ACCOM:
		case PMS_LINE_EXTRABED:
		case PMS_LINE_EXTRACOT:
		case PMS_LINE_BREAKFAST:
			if ( FALSE == m_bPendingBooking )
			{
				if ( CPriceHelpers::CompareDoubles( infoSale.m_dValue, 0.0, 3 ) != 0 )
				{
					CString strExpand = "<..>";

					CCSV csv( '\t' );
					csv.Add( "" );
					csv.Add( strExpand + infoSale.m_strDescription );
					csv.Add( "" );
					csv.Add( GetCurrencyString ( infoSale.m_dValue ) );
					m_ReportFile.WriteLine( csv.GetLine() );
					bWrittenItem = TRUE;

					dGrandTotal += infoSale.m_dValue;
				}
			}
			break;

		case PMS_LINE_DINNER:
			if ( FALSE == m_bPendingBooking )
			{
				if ( CPriceHelpers::CompareDoubles( infoSale.m_dValue, 0.0, 3 ) != 0 )
				{
					CCSV csv( '\t' );
					csv.Add( "" );
					csv.Add( infoSale.m_strDescription );
					csv.Add( strDisplayDate );
					csv.Add( GetCurrencyString ( infoSale.m_dValue ) );
					m_ReportFile.WriteLine( csv.GetLine() );
					bWrittenItem = TRUE;

					dGrandTotal += infoSale.m_dValue;
				}
			}
			break;
		}
	}

	double dAllowanceTotal = 0.0;

	if ( FALSE == m_bPendingBooking )
	{
		for ( int n = 0; n < m_arrayInvoiceItems.GetAllowanceTotalCount(); n++ )
		{
			CPMSInvoiceLineInfo infoAllowance;
			m_arrayInvoiceItems.GetAllowanceTotal( n, infoAllowance );
			dAllowanceTotal += infoAllowance.m_dAllowance;
		}
	}

	if ( TRUE == bWrittenItem )
	{
		CCSVReportLine csv;
		csv.AppendBlanks ( 3 );
		csv.AppendString( "<LI>" );
		m_ReportFile.WriteLine( csv.GetLine() );
	}

	if ( FALSE == m_bPendingBooking )
	{
		CCSVReportLine csv;
		csv.AppendBlanks ( 2 );
		csv.AppendString( "<..>Sales Total" );
		csv.AppendString( GetCurrencyString ( dGrandTotal ) );
		m_ReportFile.WriteLine( csv.GetLine() );
	}

	if ( ( CPriceHelpers::CompareDoubles( dAllowanceTotal, 0.0, 3 ) != 0 ) && ( FALSE == m_bPendingBooking ) )
	{
		{
			CCSVReportLine csv;
			csv.AppendBlanks ( 2 );
			csv.AppendString( "<..>Allowances" );
			csv.AppendString( GetCurrencyString ( -dAllowanceTotal ) );
			m_ReportFile.WriteLine( csv.GetLine() );
		}

		dGrandTotal -= dAllowanceTotal;

		{
			CCSVReportLine csv;
			csv.AppendBlanks ( 2 );
			csv.AppendString( "<..>Subtotal" );
			csv.AppendString( GetCurrencyString ( dGrandTotal ) );
			m_ReportFile.WriteLine( csv.GetLine() );
		}
	}

	if ( FALSE == m_bPendingBooking )
	{
		CCSVReportLine csv;
		csv.AppendBlanks ( 2 );
		csv.AppendString( EcrmanOptions.GetTaxString( "<..>%T Content" ) );
		csv.AppendString( GetCurrencyString ( m_arrayInvoiceItems.GetInvoiceTaxContent() ) );
		m_ReportFile.WriteLine( csv.GetLine() );
	}

	if ( FALSE == m_bPendingBooking )
	{
		CCSVReportLine csv;
		csv.AppendBlanks ( 2 );
		csv.AppendString( "<..>Net Total" );
		csv.AppendString( GetCurrencyString ( dGrandTotal - m_arrayInvoiceItems.GetInvoiceTaxContent() ) );
		m_ReportFile.WriteLine( csv.GetLine() );
	}

	if ( m_arrayInvoiceItems.GetPaymentTotalCount() > 0 )
	{
		CString strFullLine = "<..>";

		bool bWrittenPayment = FALSE;
		for ( int n = 0; n < m_arrayInvoiceItems.GetPaymentTotalCount(); n++ )
		{	
			CPMSInvoiceLineInfo infoPayment;
			m_arrayInvoiceItems.GetPaymentTotal( n, infoPayment );

			if ( CPriceHelpers::CompareDoubles( infoPayment.m_dValue, 0.0, 3 ) != 0 )
			{
				CCSVReportLine csv;
				csv.AppendBlanks( 1 );

				if ( PMS_LINE_DEPOSIT == infoPayment.m_nLineType )
					csv.AppendString( "Deposit" );
				else
					csv.AppendString( "Payment Total" );

				csv.AppendString( strFullLine + infoPayment.m_strDescription );
				csv.AppendString( GetCurrencyString ( infoPayment.m_dValue ) );

				if ( FALSE == bWrittenPayment )
					m_ReportFile.WriteLine( "" );

				m_ReportFile.WriteLine( csv.GetLine() );

				dGrandTotal -= infoPayment.m_dValue;
				bWrittenPayment = TRUE;
			}
		}
	}

	{
		CCSVReportLine csv;
		csv.AppendBlanks ( 3 );
		csv.AppendString( "<LI>" );
		m_ReportFile.WriteLine( csv.GetLine() );
	}

	CCSVReportLine csv;
	csv.AppendBlanks ( 2 );
	
	switch( CPriceHelpers::CompareDoubles( dGrandTotal, 0.0, 2 ) )
	{
	case 1:
		csv.AppendString( "<..>Balance" );
		csv.AppendString( GetCurrencyString ( dGrandTotal, TRUE ) );
		break;

	case -1:
		csv.AppendString( "<..>Credit" );
		csv.AppendString( GetCurrencyString ( -dGrandTotal, TRUE ) );
		break;

	case 0:
	default:
		csv.AppendString( "<..>Balance" );
		csv.AppendString( GetCurrencyString ( 0.0, TRUE ) );
		break;
	}

	m_ReportFile.WriteLine( csv.GetLine() );
}

/**********************************************************************/

const char* CPMSReportInvoice::GetCurrencyString( double dAmount, bool bBottomLine )
{
	CString strSymbol = "";

	int nSize = GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SCURRENCY, NULL, 0);

	if ( 0 != nSize )
	{
		LPSTR lpBuf = (LPSTR) malloc( nSize );
		
		if ( NULL != lpBuf )
		{
			int nResult = GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SCURRENCY, lpBuf, nSize);

			if ( 0 != nResult )
				strSymbol = lpBuf;

			free( lpBuf );
		}
	}

	const char* szSymbol = strSymbol;

	if ( CPriceHelpers::CompareDoubles( dAmount, 0.0, 3 ) >= 0 )
	{
		int nFormat = 0;

		DWORD value;
		if ( GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_ICURRENCY | LOCALE_RETURN_NUMBER, (LPTSTR)&value, sizeof(value) / sizeof(TCHAR) ) != 0 )
			nFormat = value;
		
		if ( FALSE == bBottomLine )
		{
			m_strCurrency.Format( "%.2f", dAmount );
		}
		else
		{
			switch( nFormat )
			{
			case 1:		m_strCurrency.Format( "%.2f%s", dAmount, szSymbol );	break;
			case 2:		m_strCurrency.Format( "%s %.2f", szSymbol, dAmount );	break;
			case 3:		m_strCurrency.Format( "%.2f %s", dAmount, szSymbol );	break;
			case 0:
			default:	m_strCurrency.Format( "%s%.2f", szSymbol, dAmount );	break;
			}
		}
	}
	else
	{
		dAmount = -dAmount;

		int nFormat = 0;

		DWORD value;
		if ( GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_INEGCURR | LOCALE_RETURN_NUMBER, (LPTSTR)&value, sizeof(value) / sizeof(TCHAR) ) != 0 )
			nFormat = value;
		
		if ( FALSE == bBottomLine )
		{
			switch( nFormat )
			{
			case 3:		
			case 6:		
			case 7:		
			case 10:	
			case 11:	
			case 13:	m_strCurrency.Format( "%.2f-", dAmount );	break;
			case 1:
			case 2:		
			case 5:		
			case 8:		
			case 9:		
			case 12:	m_strCurrency.Format( "-%.2f", dAmount );	break;
			case 0:		
			case 4:		
			case 14:	
			case 15:	
			default:	m_strCurrency.Format( "(%.2f)", dAmount );	break;
			}
		}
		else
		{
			switch( nFormat )
			{
			case 1:		m_strCurrency.Format( "-%s%.2f", szSymbol, dAmount );	break;
			case 2:		m_strCurrency.Format( "%s-%.2f", szSymbol, dAmount );	break;
			case 3:		m_strCurrency.Format( "%s%.2f-", szSymbol, dAmount );	break;
			case 4:		m_strCurrency.Format( "(%.2f%s)", dAmount, szSymbol );	break;
			case 5:		m_strCurrency.Format( "-%.2f%s", dAmount, szSymbol );	break;
			case 6:		m_strCurrency.Format( "%.2f-%s", dAmount, szSymbol );	break;
			case 7:		m_strCurrency.Format( "%.2f%s-", dAmount, szSymbol );	break;
			case 8:		m_strCurrency.Format( "-%.2f %s", dAmount, szSymbol );	break;
			case 9:		m_strCurrency.Format( "-%s %.2f", szSymbol, dAmount) ;	break;
			case 10:	m_strCurrency.Format( "%.2f %s-", dAmount, szSymbol );	break;
			case 11:	m_strCurrency.Format( "%s %.2f-", szSymbol, dAmount );	break;
			case 12:	m_strCurrency.Format( "%s -%.2f", szSymbol, dAmount );	break;
			case 13:	m_strCurrency.Format( "%.2f- %s", dAmount, szSymbol );	break;
			case 14:	m_strCurrency.Format( "(%s %.2f)", szSymbol, dAmount );	break;
			case 15:	m_strCurrency.Format( "(%.2f %s)", dAmount, szSymbol );	break;
			case 0:
			default:	m_strCurrency.Format( "(%s%.2f)", szSymbol, dAmount );	break;
			}
		}
	}

	return m_strCurrency;
}

/**********************************************************************/
#endif
/**********************************************************************/
