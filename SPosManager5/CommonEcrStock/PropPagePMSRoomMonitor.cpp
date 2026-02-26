/**********************************************************************/
#ifdef COMPILE_PMS 
/**********************************************************************/
#include "CommsHandshaker.h"
#include "PMSBookingCSVArray.h"
#include "PMSOptions.h"
#include "PMSRoomStatusTable.h"
#include "PriceHelpers.h"
#include "SysInfo.h"
/**********************************************************************/
#include "PropPagePMSRoomMonitor.h"
/**********************************************************************/
static const UINT BALANCE_TIMER_GRID = 2;
/**********************************************************************/

CPropPagePMSRoomMonitor::CPropPagePMSRoomMonitor() : CPropertyPage(CPropPagePMSRoomMonitor::IDD)
{
	//{{AFX_DATA_INIT(CPropPagePMSRoomMonitor)
	//}}AFX_DATA_INIT

	m_hTimer = 0;

	m_nRoomIdxStart = 0;
	m_nRoomIdxEnd = 0;

	m_nButtonID[0] = IDC_BUTTON_ROOM1;
	m_nButtonID[1] = IDC_BUTTON_ROOM2;
	m_nButtonID[2] = IDC_BUTTON_ROOM3;
	m_nButtonID[3] = IDC_BUTTON_ROOM4;
	m_nButtonID[4] = IDC_BUTTON_ROOM5;
	m_nButtonID[5] = IDC_BUTTON_ROOM6;
	m_nButtonID[6] = IDC_BUTTON_ROOM7;
	m_nButtonID[7] = IDC_BUTTON_ROOM8;
	m_nButtonID[8] = IDC_BUTTON_ROOM9;
	m_nButtonID[9] = IDC_BUTTON_ROOM10;
	m_nButtonID[10] = IDC_BUTTON_ROOM11;
	m_nButtonID[11] = IDC_BUTTON_ROOM12;
	m_nButtonID[12] = IDC_BUTTON_ROOM13;
	m_nButtonID[13] = IDC_BUTTON_ROOM14;
	m_nButtonID[14] = IDC_BUTTON_ROOM15;
	
	m_CheckBalanceTime = COleDateTime( 1980, 1, 1, 0, 0, 0 );
}

/**********************************************************************/

CPropPagePMSRoomMonitor::~CPropPagePMSRoomMonitor()
{
}

/**********************************************************************/

void CPropPagePMSRoomMonitor::SetRoomIdxRange( int nStart, int nEnd )
{
	m_nRoomIdxStart = nStart;
	m_nRoomIdxEnd = nEnd;

	int nRooms = DataManagerNonDb.PMSRoom.GetSize();
	if ( nRooms > SysInfo.GetMaxPMSRooms() )
		nRooms = SysInfo.GetMaxPMSRooms();

	for ( int nRoomIdx = m_nRoomIdxStart; nRoomIdx <= m_nRoomIdxEnd; nRoomIdx++ )
	{
		int nButtonIdx = nRoomIdx - m_nRoomIdxStart;

		m_buttonRoom[nButtonIdx].m_nButtonNo = nButtonIdx;
		
		m_buttonRoom[nButtonIdx].m_strLine1 = "";
		m_buttonRoom[nButtonIdx].m_strLine2 = "";
		m_buttonRoom[nButtonIdx].m_strLine3 = "";
		m_buttonRoom[nButtonIdx].m_strLine4 = "";
		m_buttonRoom[nButtonIdx].m_strLine5 = "";
		m_buttonRoom[nButtonIdx].m_strLine6 = "";
		m_buttonRoom[nButtonIdx].m_strLine7 = "";

		if ( nRoomIdx < nRooms )
		{
			CPMSRoomCSVRecord Room;
			DataManagerNonDb.PMSRoom.GetAt( nRoomIdx, Room );
			m_buttonRoom[nButtonIdx].m_strLine1.Format( "Room %d", Room.GetRoomNo() );
		}
	}
}

/**********************************************************************/

void CPropPagePMSRoomMonitor::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPagePMSRoomMonitor)
	//}}AFX_DATA_MAP

	for ( int n = 0; n < 15; n++ )
		DDX_Control(pDX, m_nButtonID[n], m_buttonRoom[n]);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPagePMSRoomMonitor, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPagePMSRoomMonitor)
	//}}AFX_MSG_MAP
	ON_WM_TIMER()
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPropPagePMSRoomMonitor::OnInitDialog() 
{
	CPropertyPage::OnInitDialog(); 
	return TRUE;  
}

/**********************************************************************/

void CPropPagePMSRoomMonitor::UpdateButtonGrid()
{
	for ( int nButtonIdx = 0; nButtonIdx < 15; nButtonIdx++ )
		UpdateButton( nButtonIdx );
}

/**********************************************************************/

void CPropPagePMSRoomMonitor::UpdateButton( int nButtonIdx )
{
	int nRooms = DataManagerNonDb.PMSRoom.GetSize();
	if ( nRooms > SysInfo.GetMaxPMSRooms() )
		nRooms = SysInfo.GetMaxPMSRooms();

	int nRoomIdx = m_nRoomIdxStart + nButtonIdx;

	m_buttonRoom[nButtonIdx].m_strLine1 = "";
	m_buttonRoom[nButtonIdx].m_strLine2 = "";
	m_buttonRoom[nButtonIdx].m_strLine3 = "";
	m_buttonRoom[nButtonIdx].m_strLine4 = "";
	m_buttonRoom[nButtonIdx].m_strLine5 = "";
	m_buttonRoom[nButtonIdx].m_strLine6 = "";
	m_buttonRoom[nButtonIdx].m_strLine7 = "";

	if ( nRoomIdx >= nRooms )
	{
		m_buttonRoom[nButtonIdx].EnableWindow( FALSE );
		m_buttonRoom[nButtonIdx].ShowWindow( SW_HIDE );
	}
	else 
	{
		CPMSRoomStatusInfo infoRoom;
		PMSRoomStatusTable.GetRoomInfo( nRoomIdx, infoRoom );

		CPMSRoomAccountInfo infoAccount;
		PMSRoomStatusTable.GetAccountInfo( nRoomIdx, 0, infoAccount );

		double dBalance = infoAccount.CalculateBalance();
	
		CString strBalance = "";
		switch ( CPriceHelpers::CompareDoubles( dBalance, 0.0, 2 ) )
		{
		case 1:
			CPriceHelpers::FormatDouble( dBalance, 2, strBalance );
			break;

		case -1:
			CPriceHelpers::FormatDouble( -dBalance, 2, strBalance );
			strBalance += " CR";
			break;

		case 0:
		default:
			strBalance = "0.00";
			break;
		}

		CString strLine1 = "";
		CString strLine2 = "";
		CString strLine3 = "";
		CString strLine4 = "";
		CString strLine5 = "";
		CString strLine6 = "";
		CString strLine7 = "";
		int nMainBackColour = 0;
		int nMainTextColour = 0;
		int nBottomBackColour = 0;
		int nBottomTextColour = 0;

		CString strRoomName;
		::GetPMSRoomText( infoRoom.GetRoomNo(), infoRoom.GetRoomName(), strLine1, FALSE );

		if ( infoRoom.GetBookingId() == 0 )
		{
			if ( PMSRoomStatusTable.GetClosureFlag( nRoomIdx ) == TRUE )
			{
				nMainBackColour = PMSOptions.GetColourClosedBack();
				nMainTextColour = PMSOptions.GetColourClosedText();
				nBottomBackColour = PMSOptions.GetColourVacantBack();
				nBottomTextColour = PMSOptions.GetColourVacantText();	
				
				int nClosureIdx;
				if ( DataManagerNonDb.PMSClosure.FindClosureById( PMSRoomStatusTable.GetClosureId( nRoomIdx ), nClosureIdx ) == TRUE )
				{
					CPMSClosedRoomCSVRecord ClosureRecord;
					DataManagerNonDb.PMSClosure.GetAt( nClosureIdx, ClosureRecord );
					strLine2 = ClosureRecord.GetReason();
				}
	
				strLine7 = "Room Closed";
			}
			else if ( PMSRoomStatusTable.GetExpectedBookingId( nRoomIdx ) != 0 )
			{
				nMainBackColour = PMSOptions.GetColourPendingBack();
				nMainTextColour = PMSOptions.GetColourPendingText();
				nBottomBackColour = PMSOptions.GetColourActiveBack();
				nBottomTextColour = PMSOptions.GetColourActiveText();

				CString strGuest = PMSRoomStatusTable.GetExpectedGuestName( nRoomIdx );
				CString strBilling = PMSRoomStatusTable.GetExpectedBillingName( nRoomIdx );

				::TrimSpaces( strGuest, FALSE );
				::TrimSpaces( strBilling, FALSE );

				if ( strGuest == strBilling )
					strBilling = "";

				strLine2 = strGuest;
				strLine3 = strBilling;
				strLine7 = "Check In Due";
			}
			else
			{
				nMainBackColour = PMSOptions.GetColourVacantBack();
				nMainTextColour = PMSOptions.GetColourVacantText();
				nBottomBackColour = PMSOptions.GetColourPendingBack();
				nBottomTextColour = PMSOptions.GetColourPendingText();
			
				int nNights = PMSRoomStatusTable.GetAvailableNights( nRoomIdx );

				if ( nNights == 0 )
					strLine7 = "";
				else 
					strLine7 = "Vacant";
			}
		}
		else
		{
			CString strGuest = infoAccount.GetGuestName();
			CString strBilling = infoAccount.GetBillingName();

			CString strGuests = "";
			CString strTariff = "";

			int nBookingIdx;
			if ( DataManagerNonDb.PMSBooking.FindBookingById( infoRoom.GetBookingId(), nBookingIdx ) == TRUE )
			{
				CPMSBookingCSVRecord BookingRecord;
				DataManagerNonDb.PMSBooking.GetAt( nBookingIdx, BookingRecord );

				int nGuests = BookingRecord.GetGuests();

				if ( 1 == nGuests )
					strGuests = "1 Guest";
				else
					strGuests.Format( "%d Guests", nGuests );

				switch( BookingRecord.GetRoomRateType() )
				{
				case PMS_BOOKING_RATE_ROOM:		strTariff = "Room Only";		break;
				case PMS_BOOKING_RATE_BB:		strTariff = "B & B";			break;
				case PMS_BOOKING_RATE_DBB:		strTariff = "DB & B";			break;
				case PMS_BOOKING_RATE_FREE:		strTariff = "Free Room";		break;
				}
			}
			
			::TrimSpaces( strGuest, FALSE );
			::TrimSpaces( strBilling, FALSE );

			if ( strGuest == strBilling )
				strBilling = "";

			strLine2 = strGuest;
			strLine3 = strBilling;
			strLine4 = strBalance;
			strLine5 = strGuests;
			strLine6 = strTariff;
			nMainBackColour = PMSOptions.GetColourActiveBack();
			nMainTextColour = PMSOptions.GetColourActiveText();
		
			if ( PMSRoomStatusTable.GetExpectedBookingId( nRoomIdx ) != infoRoom.GetBookingId() )
			{
				nBottomBackColour = PMSOptions.GetColourCompleteBack();
				nBottomTextColour = PMSOptions.GetColourCompleteText();
				strLine7 = "Check Out Due";
			}
		}

		m_buttonRoom[nButtonIdx].m_strLine1 = strLine1;
		m_buttonRoom[nButtonIdx].m_strLine2 = strLine2;
		m_buttonRoom[nButtonIdx].m_strLine3 = strLine3;
		m_buttonRoom[nButtonIdx].m_strLine4 = strLine4;
		m_buttonRoom[nButtonIdx].m_strLine5 = strLine5;
		m_buttonRoom[nButtonIdx].m_strLine6 = strLine6;
		m_buttonRoom[nButtonIdx].m_strLine7 = strLine7;
		m_buttonRoom[nButtonIdx].m_ColourRefBack = COLORREF( nMainBackColour );
		m_buttonRoom[nButtonIdx].m_ColourRefText = COLORREF( nMainTextColour );
		m_buttonRoom[nButtonIdx].m_ColourRefBottomLineBack = COLORREF( nBottomBackColour );
		m_buttonRoom[nButtonIdx].m_ColourRefBottomLineText = COLORREF( nBottomTextColour );
	}
		
	m_buttonRoom[ nButtonIdx ].Invalidate();
}

/**********************************************************************/

BOOL CPropPagePMSRoomMonitor::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	KillUpdateTimer();
	return bResult;
}

/**********************************************************************/

BOOL CPropPagePMSRoomMonitor::OnSetActive() 
{
	bool bResult = ( CPropertyPage::OnSetActive() != 0 );
	UpdateButtonGrid();
	StartUpdateTimer();
	return bResult;
}

/**********************************************************************/

void CPropPagePMSRoomMonitor::StartUpdateTimer()
{
	if ( m_hTimer == 0 )
		m_hTimer = SetTimer ( BALANCE_TIMER_GRID, 1000, NULL );
}

/**********************************************************************/

void CPropPagePMSRoomMonitor::KillUpdateTimer()
{
	if ( m_hTimer > 0 )
	{
		KillTimer ( m_hTimer );
		m_hTimer = 0;
	}
}

/**********************************************************************/

void CPropPagePMSRoomMonitor::OnTimer(UINT nIDEvent) 
{
	switch( nIDEvent )
	{
	case BALANCE_TIMER_GRID:
		if ( CommsHandshaker.GetConfirmStoppedFlag() == TRUE )
		{
			for ( int nRoomIdx = m_nRoomIdxStart; nRoomIdx <= m_nRoomIdxEnd; nRoomIdx++ )
			{
				PMSRoomStatusTable.UpdateAccountBalance( nRoomIdx, FALSE );
				int nButtonIdx = nRoomIdx - m_nRoomIdxStart;
				UpdateButton( nButtonIdx );
			}

			CommsHandshaker.PMSPulse();
		}

		break;
	}
}

/**********************************************************************/
#endif
/**********************************************************************/
