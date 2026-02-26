/**********************************************************************/
#ifdef COMPILE_PMS 
/**********************************************************************/
#include "PMSCalendarTable.h"
#include "PMSOptions.h"
#include "PMSRoomStatusTable.h"
/**********************************************************************/
#include "PMSCheckoutDlg.h"
/**********************************************************************/

CPMSCheckoutDlg::CPMSCheckoutDlg( int nRoomIdx, CPMSBookingCSVRecord& BookingRecord, CWnd* pParent /*=NULL*/)
	: CSSDialog(CPMSCheckoutDlg::IDD, pParent), m_BookingRecord( BookingRecord )
{
	//{{AFX_DATA_INIT(CPMSCheckoutDlg)
	//}}AFX_DATA_INIT
	m_nRoomIdx = nRoomIdx;
	PMSRoomStatusTable.GetRoomInfo( nRoomIdx, m_infoRoom );
}

/**********************************************************************/

void CPMSCheckoutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPMSCheckoutDlg)
	//}}AFX_DATA_MAP
	DDX_Control(pDX,IDC_COMBO_CHECKIN, m_comboCheckIn);
	DDX_Control(pDX,IDC_COMBO_CHECKOUT_EXPECTED, m_comboCheckOutExpected);
	DDX_Control(pDX,IDC_COMBO_CHECKOUT_ACTUAL, m_comboCheckOutActual);
	DDX_Control(pDX,IDC_STATIC_WARNING, m_staticWarning);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPMSCheckoutDlg, CDialog)
	//{{AFX_MSG_MAP(CPMSCheckoutDlg)
	//}}AFX_MSG_MAP
	ON_CBN_SELCHANGE(IDC_COMBO_CHECKOUT_ACTUAL,UpdateWarningText)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPMSCheckoutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CArray<int, int> arrayStops;
	arrayStops.Add(5);
	arrayStops.Add(45);
	arrayStops.Add(65);
	arrayStops.Add(100);
	m_comboCheckIn.SetTabStops( arrayStops );
	m_comboCheckOutExpected.SetTabStops( arrayStops );
	m_comboCheckOutActual.SetTabStops( arrayStops );

	CString strRoomName;
	::GetPMSRoomText( m_infoRoom.GetRoomNo(), m_infoRoom.GetRoomName(), strRoomName );

	CString strRoomText;
	strRoomText.Format( "%s",
		(const char*) strRoomName );
	
	CString str;
	str.Format ( "Check Out (%s)", (const char*) strRoomText );

	if ( m_BookingRecord.GetTrainingFlag() == TRUE )
		str += " (Training Mode)";

	SetWindowText( str );

	int nExpectedCheckOutDay = m_infoRoom.GetStartDay() + m_infoRoom.GetNights();

	COleDateTime dateCheckIn;
	GetOleDateSince2011( m_infoRoom.GetStartDay(), dateCheckIn );

	COleDateTime dateCheckOut;
	GetOleDateSince2011( nExpectedCheckOutDay, dateCheckOut );
	
	{
		m_comboCheckIn.ResetContent();

		CString strDayName = dateCheckIn.Format( "%a" );
		CString strDayNum = dateCheckIn.Format( "%d" );
		CString strMonth = dateCheckIn.Format( "%b" );
		CString strYear = dateCheckIn.Format( "%Y" );
			
		CCSV csv;
		csv.Add( strDayName );
		csv.Add( strDayNum );
		csv.Add( strMonth );
		csv.Add( strYear );
		m_comboCheckIn.AddItem( csv.GetLine() );
	}

	{
		m_comboCheckOutExpected.ResetContent();

		CString strDayName = dateCheckOut.Format( "%a" );
		CString strDayNum = dateCheckOut.Format( "%d" );
		CString strMonth = dateCheckOut.Format( "%b" );
		CString strYear = dateCheckOut.Format( "%Y" );
			
		CCSV csv;
		csv.Add( strDayName );
		csv.Add( strDayNum );
		csv.Add( strMonth );
		csv.Add( strYear );
		m_comboCheckOutExpected.AddItem( csv.GetLine() );
	}

	CPMSBookingQueryInfo Query;
	Query.m_nRoomNo = m_infoRoom.GetRoomNo();
	Query.m_nStartDay = nExpectedCheckOutDay;
	Query.m_nMinNights = 1;
	Query.m_nMaxNights = PMSBooking::Nights.Max;

	CPMSBookingResponseInfo Response;

	PMSCalendarTable.CheckRoomAvailability( Query, Response );

	int nExtraNights = 0;
	if ( TRUE == Response.m_bSuccess )
		nExtraNights = Response.m_nMaxNights;

	if ( nExpectedCheckOutDay + nExtraNights > PMSOptions.GetPMSDayNumber() + 1 )
		nExtraNights = PMSOptions.GetPMSDayNumber() + 1 - nExpectedCheckOutDay;

	{
		m_comboCheckOutActual.ResetContent();
		m_nLastPossibleCheckOutDay = nExpectedCheckOutDay + nExtraNights;

		int nSel = 0;

		for ( int n = 7; n >= 0; n-- )
		{
			int nPMSDayToAdd = m_nLastPossibleCheckOutDay - n;

			COleDateTime dateCheckOut;
			GetOleDateSince2011( nPMSDayToAdd, dateCheckOut );

			if ( ( dateCheckOut >= dateCheckIn ) || ( n == 0 ) )
			{
				CString strDayName = dateCheckOut.Format( "%a" );
				CString strDayNum = dateCheckOut.Format( "%d" );
				CString strMonth = dateCheckOut.Format( "%b" );
				CString strYear = dateCheckOut.Format( "%Y" );
				
				CCSV csv;
				csv.Add( strDayName );
				csv.Add( strDayNum );
				csv.Add( strMonth );
				csv.Add( strYear );
				
				int nPos = m_comboCheckOutActual.AddItem( csv.GetLine() );
				if ( nPMSDayToAdd <= PMSOptions.GetPMSDayNumber() )
					nSel = nPos;
			}
		}

		m_comboCheckOutActual.SetCurSel( nSel );
	}

	m_comboCheckIn.SetCurSel(0);
	m_comboCheckOutExpected.SetCurSel(0);

	UpdateWarningText();
	
	return TRUE;  
}

/**********************************************************************/

void CPMSCheckoutDlg::UpdateWarningText()
{
	COleDateTime dateExpected;
	GetOleDateSince2011( m_infoRoom.GetStartDay() + m_infoRoom.GetNights(), dateExpected );
	
	COleDateTime dateActual;
	GetOleDateSince2011 ( m_nLastPossibleCheckOutDay, dateActual );
	dateActual -= COleDateTimeSpan( m_comboCheckOutActual.GetCount() - 1 );
	dateActual += COleDateTimeSpan( m_comboCheckOutActual.GetCurSel(), 0, 0, 0 );

	COleDateTimeSpan dateDiffer = dateActual - dateExpected;

	int nDays = dateDiffer.GetDays();

	if ( nDays == 0 )
	{
		m_staticWarning.SetWindowText( "" );
	}
	else if ( nDays == -1 )
	{
		m_staticWarning.SetWindowText( "Check out is 1 day earlier than expected." );
	}
	else if ( nDays == 1 )
	{
		m_staticWarning.SetWindowText( "Check out is 1 day later than expected." );
	}
	else if ( nDays < 0 )
	{
		CString str;
		str.Format( "Check out is %d days earlier than expected.", -nDays );
		m_staticWarning.SetWindowText( str );
	}
	else
	{
		CString str;
		str.Format( "Check out is %d days later than expected.", nDays );
		m_staticWarning.SetWindowText( str );
	}
}

/**********************************************************************/

void CPMSCheckoutDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		int nCheckOutDay = m_nLastPossibleCheckOutDay;
		nCheckOutDay -= ( m_comboCheckOutActual.GetCount() - 1 );
		nCheckOutDay += m_comboCheckOutActual.GetCurSel();

		int nNights = nCheckOutDay - m_infoRoom.GetStartDay();
		if ( ( nNights < 0 ) || ( nNights > PMSBooking::Nights.Max ) )
			nNights = 0;

		//bool bWarning = FALSE;

		//int nBookingIdx;
		//if ( DataManagerNonDb.PMSBooking.FindBookingById( m_infoRoom.GetBookingId(), nBookingIdx ) == TRUE )
		{
			//CPMSBookingCSVRecord BookingRecord;
			//DataManagerNonDb.PMSBooking.GetAt( nBookingIdx, BookingRecord );

			CPMSBookingPartInfo infoPart;
			m_BookingRecord.GetFinalPartInfo( infoPart );
			infoPart.SetNights ( nNights );
			m_BookingRecord.SetFinalPartInfo( infoPart );
		
			//CPMSBookingCSVArray::InsertAndSaveBooking( BookingRecord );

			//bWarning = BookingRecord.GetTrainingFlag();
		}

		/*
		if ( TRUE == bWarning )
		{
			CString strMsg = "";
			strMsg += "You are checking out a booking for which training mode is enabled.\n\n";
			strMsg += "This booking will be deleted when you close the calendar.";
			Prompter.Info( strMsg );
		}
		*/

		EndDialog( IDOK );
	}
}

/**********************************************************************/
#endif
/**********************************************************************/


