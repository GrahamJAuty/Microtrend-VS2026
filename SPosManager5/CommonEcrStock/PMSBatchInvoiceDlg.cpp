/**********************************************************************/
#ifdef COMPILE_PMS
/**********************************************************************/
#include "PMSAccountFindReportDlg.h"
#include "PMSCalendarDlg.h"
#include "PMSOptions.h"
#include "PMSReportInvoice.h"
#include "PMSRoomStatusTable.h"
#include "RepSpawn.h"
/**********************************************************************/
#include "PMSBatchInvoiceDlg.h"
/**********************************************************************/
IMPLEMENT_DYNAMIC(CPMSBatchInvoiceDlg, CDialog)
/**********************************************************************/

CPMSBatchInvoiceDlg::CPMSBatchInvoiceDlg( CWnd* pParent )
	: CSSDialog(CPMSBatchInvoiceDlg::IDD, pParent) 
{
	m_nSearchCheckOutBase = PMSOptions.GetPMSDayNumber();
	m_nSearchCheckOutDay = m_nSearchCheckOutBase + 1;
}

/**********************************************************************/

CPMSBatchInvoiceDlg::~CPMSBatchInvoiceDlg()
{
}

/**********************************************************************/

void CPMSBatchInvoiceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_CHECKOUT, m_comboCheckOut);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPMSBatchInvoiceDlg, CDialog)

	ON_CBN_SELCHANGE(IDC_COMBO_CHECKOUT, OnChangeDateCheckOut)
	ON_CBN_DROPDOWN(IDC_COMBO_CHECKOUT, OnOpenCheckOut)
	ON_BN_CLICKED(IDC_BUTTON_PRINT_INVOICE, OnButtonPrintInvoice)
	ON_BN_CLICKED(IDC_BUTTON_PRINT_LIST, OnButtonPrintList)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDoubleClickList)

END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPMSBatchInvoiceDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CArray<int, int> arrayStops;
	arrayStops.Add(5);
	arrayStops.Add(35);
	arrayStops.Add(55);
	arrayStops.Add(80);
	m_comboCheckOut.SetTabStops( arrayStops );

	FillCheckOutCombo();

	m_listBookings.SubclassDlgItem ( IDC_LIST, this );
	m_listBookings.InsertColumn ( 0, "", LVCFMT_LEFT, 26 );
	m_listBookings.InsertColumn ( 1, "Check In", LVCFMT_LEFT, 85 );
	m_listBookings.InsertColumn ( 2, "Nights", LVCFMT_LEFT, 50 );
	m_listBookings.InsertColumn ( 3, "Check Out", LVCFMT_LEFT, 85 );
	m_listBookings.InsertColumn ( 4, "No", LVCFMT_LEFT, 44 );
	m_listBookings.InsertColumn ( 5, "Room", LVCFMT_LEFT, 120 );
	m_listBookings.InsertColumn ( 6, "Booking Name", LVCFMT_LEFT, 120 );
	m_listBookings.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES /* | LVS_EX_GRIDLINES */ );

	OnChangeDateCheckOut();

	return TRUE;  
}

/**********************************************************************/

void CPMSBatchInvoiceDlg::FillCheckOutCombo()
{
	int nMinDay = PMSOptions.GetPMSDayNumber();
	int nMaxDay = nMinDay + 7;

	int nSelDay = m_nSearchCheckOutDay;
	int nBaseDay = m_nSearchCheckOutDay - 7;
	
	if ( nBaseDay < nMinDay )
		nBaseDay = nMinDay;

	int nEndDay = nSelDay + 7;
	if ( nEndDay > nMaxDay )
		nEndDay = nMaxDay;

	if ( nBaseDay > nEndDay )
		nBaseDay = nEndDay;

	m_comboCheckOut.ResetContent();

	for ( int nDay = nBaseDay; nDay <= nEndDay; nDay++ )
	{
		COleDateTime date;
		GetOleDateSince2011( nDay, date );

		CString strDayName = date.Format( "%a" );
		CString strDayNum = date.Format( "%d" );
		CString strMonth = date.Format( "%b" );
		CString strYear = date.Format( "%Y" );
		
		CCSV csv;
		csv.Add( strDayName );
		csv.Add( strDayNum );
		csv.Add( strMonth );
		csv.Add( strYear );
		m_comboCheckOut.AddItem( csv.GetLine() );
	}

	m_comboCheckOut.SetCurSel( nSelDay - nBaseDay );
}

/**********************************************************************/

void CPMSBatchInvoiceDlg::OnOpenCheckOut()
{
	FillCheckOutCombo();
}

/**********************************************************************/

void CPMSBatchInvoiceDlg::OnChangeDateCheckOut()
{
	m_nSearchCheckOutDay = m_nSearchCheckOutBase + m_comboCheckOut.GetCurSel();

	m_arrayBookingInfo.RemoveAll();
	m_listBookings.DeleteAllItems();

	for ( int nRoomIdx = 0; nRoomIdx < DataManagerNonDb.PMSRoom.GetSize(); nRoomIdx++ )
	{
		CPMSRoomStatusInfo infoRoom;
		PMSRoomStatusTable.GetRoomInfo( nRoomIdx, infoRoom );

		if ( infoRoom.GetBookingId() == 0 )
			continue;

		if ( infoRoom.GetStartDay() + infoRoom.GetNights() != m_nSearchCheckOutDay )
			continue;

		int nBookingIdx;
		if ( DataManagerNonDb.PMSBooking.FindBookingById( infoRoom.GetBookingId(), nBookingIdx ) == FALSE )
			continue;

		CPMSBookingCSVRecord BookingRecord;
		DataManagerNonDb.PMSBooking.GetAt( nBookingIdx, BookingRecord );

		if ( BookingRecord.GetBookingStatus() != PMS_BOOKING_STATUS_ACTIVE )
			continue;

		CPMSBookingPartInfo infoFinalPart;
		BookingRecord.GetFinalPartInfo( infoFinalPart );

		if ( infoFinalPart.GetRoomNo() != infoRoom.GetRoomNo() )
			continue;

		if ( infoFinalPart.GetStartDay() + infoFinalPart.GetNights() != m_nSearchCheckOutDay )
			continue;

		CPMSRoomAccountInfo infoAccount;
		PMSRoomStatusTable.GetAccountInfo( nRoomIdx, 0, infoAccount );

		CPMSBookingPartInfo infoFirstPart;
		BookingRecord.GetPartInfo( 0, infoFirstPart );

		CPMSAccountFindInfo infoBooking;
		infoBooking.m_nCheckInDay = infoFirstPart.GetStartDay();
		infoBooking.m_nBookingId = infoRoom.GetBookingId();
		infoBooking.m_nBookingPart = BookingRecord.GetPartCount() - 1;
		infoBooking.m_nRoomNo = infoRoom.GetRoomNo();
		infoBooking.m_strRoomName = infoRoom.GetRoomName();
		infoBooking.m_nCustomerId = BookingRecord.GetCustomerId();
		infoBooking.m_strGuestName = infoAccount.GetGuestName();
		infoBooking.m_strBillingName = infoAccount.GetBillingName();
		infoBooking.m_nBookingPartStartDay = infoFinalPart.GetStartDay();
		infoBooking.m_nBookingPartNights = infoFinalPart.GetNights();
		infoBooking.m_bCheckedOut = FALSE;
		infoBooking.m_nGuests = BookingRecord.GetGuests();
		infoBooking.m_nInvoiceNum = BookingRecord.GetInvoiceNum();
		m_arrayBookingInfo.Add( infoBooking );

		COleDateTime dateIn;
		GetOleDateSince2011( infoBooking.m_nBookingPartStartDay, dateIn );

		CString strDateIn;
		strDateIn.Format( "%2.2d/%2.2d/%4.4d",
			dateIn.GetDay(),
			dateIn.GetMonth(),
			dateIn.GetYear() );

		COleDateTime dateOut;
		int nDateOut = infoBooking.m_nBookingPartStartDay + infoBooking.m_nBookingPartNights;
		GetOleDateSince2011( nDateOut, dateOut );

		CString strDateOut;
		strDateOut.Format( "%2.2d/%2.2d/%4.4d",
			dateOut.GetDay(),
			dateOut.GetMonth(),
			dateOut.GetYear() );

		CString strRoomNo;
		strRoomNo.Format( "%d", infoBooking.m_nRoomNo );

		CString strNights;
		strNights.Format( "%d", infoBooking.m_nBookingPartNights );

		int nPos = m_arrayBookingInfo.GetSize() - 1;
		m_listBookings.InsertItem ( nPos, "" );
		m_listBookings.SetItem ( nPos, 1, LVIF_TEXT, strDateIn, 0, 0, 0, NULL );
		m_listBookings.SetItem ( nPos, 2, LVIF_TEXT, strNights, 0, 0, 0, NULL );
		m_listBookings.SetItem ( nPos, 3, LVIF_TEXT, strDateOut, 0, 0, 0, NULL );
		m_listBookings.SetItem ( nPos, 4, LVIF_TEXT, strRoomNo, 0, 0, 0, NULL );
		m_listBookings.SetItem ( nPos, 5, LVIF_TEXT, infoBooking.m_strRoomName, 0, 0, 0, NULL );
		m_listBookings.SetItem ( nPos, 6, LVIF_TEXT, infoBooking.m_strGuestName, 0, 0, 0, NULL );
		m_listBookings.SetCheck( nPos, TRUE );
	}
}

/**********************************************************************/

void CPMSBatchInvoiceDlg::OnButtonPrintList()
{
	CArray<int,int> arrayDummy;
	CPMSAccountFindReportDlg dlgReport( m_arrayBookingInfo, arrayDummy, FALSE, IDM_PMSMENUBAR_BATCH_INVOICE, this );
	dlgReport.DoModal();
}

/**********************************************************************/

void CPMSBatchInvoiceDlg::OnButtonPrintInvoice()
{
	bool bFail = FALSE;

	for ( int n = 0; n < m_listBookings.GetItemCount(); n++ )
	{
		if ( m_listBookings.GetCheck(n) == 0 )
			continue;

		int nBookingIdx;
		int nBookingId = m_arrayBookingInfo[n].m_nBookingId;
		if ( DataManagerNonDb.PMSBooking.FindBookingById( nBookingId, nBookingIdx ) == FALSE )
			continue;

		CPMSBookingCSVRecord BookingRecord;
		DataManagerNonDb.PMSBooking.GetAt( nBookingIdx, BookingRecord );

		DataManagerNonDb.PMSInvoiceBookingBuffer.SetDatabaseBooking( BookingRecord );
		BookingRecord.SetBookingStatus( PMS_BOOKING_STATUS_COMPLETE );
		DataManagerNonDb.PMSInvoiceBookingBuffer.SetInvoiceBooking( BookingRecord );
		DataManagerNonDb.PMSInvoiceBookingBuffer.UpdateAllBalances();

		PMSRoomStatusTable.LoadRoomAccountSettingsBuffer( BookingRecord, TRUE );
		
		CPMSRoomStatusInfo infoRoom;
		PMSRoomStatusTable.GetRoomInfo( MAX_PMS_ROOMS, infoRoom );

		int nInvoiceType = 0;

		for ( int nAccountIdx = 0; nAccountIdx < infoRoom.GetSafeAccountCount(); nAccountIdx++ )
		{
			PMSRoomStatusTable.UpdateAccountBalance( MAX_PMS_ROOMS, nAccountIdx, TRUE );
			CPMSReportInvoice Report( MAX_PMS_ROOMS, nAccountIdx, PMSOptions.GetInvoiceDefaultType(), FALSE );

			CString strKey;
			strKey.Format( "%d", PMSOptions.GetInvoiceDefaultType() );

			if ( Report.CreateReport() == TRUE )
			{
				CReportManager ReportManager;
				ReportManager.PrintReport( Super.ReportFilename(), Report.GetParamsFilename(), strKey, this );
			}
			else
				bFail = TRUE;
		}
	}

	if ( TRUE == bFail )
	{
		CString strMsg = "";
		strMsg += "One or more of the selected invoices could not be printed.\n\n";
		strMsg += "Please check your printer and then try again.";
		Prompter.Error( strMsg );
	}
}

/**********************************************************************/

void CPMSBatchInvoiceDlg::OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int nSel = m_listBookings.GetSelectionMark();
	
	if ( ( nSel >= 0 ) && ( nSel < m_arrayBookingInfo.GetSize() ) )
	{
		int nBookingIdx;
		int nBookingId = m_arrayBookingInfo[nSel].m_nBookingId;
		if ( DataManagerNonDb.PMSBooking.FindBookingById( nBookingId, nBookingIdx ) == TRUE )
		{
			CPMSBookingCSVRecord BookingRecord;
			DataManagerNonDb.PMSBooking.GetAt( nBookingIdx, BookingRecord );
			PMSRoomStatusTable.LoadRoomAccountSettingsBuffer( BookingRecord, TRUE );
			CPMSCalendarDlg::ProcessInvoiceActive( MAX_PMS_ROOMS, TRUE, this );
		}
	}
			
	*pResult = 0;
}

/**********************************************************************/

void CPMSBatchInvoiceDlg::OnOK()
{
	OnButtonPrintInvoice();
}

/**********************************************************************/

void CPMSBatchInvoiceDlg::OnCancel()
{
	EndDialog( IDCANCEL );
}

/**********************************************************************/
#endif
/**********************************************************************/
