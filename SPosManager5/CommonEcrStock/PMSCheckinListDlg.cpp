/**********************************************************************/
#ifdef COMPILE_PMS 
/**********************************************************************/
#include "PMSAccountFindReportDlg.h"
#include "PMSOptions.h"
#include "PMSRoomStatusTable.h"
/**********************************************************************/
#include "PMSCheckinListDlg.h"
/**********************************************************************/

CPMSCheckInListDlg::CPMSCheckInListDlg( int nRoomNo, CWnd* pParent )
	: CSSDialog(CPMSCheckInListDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPMSCheckInListDlg)
	//}}AFX_DATA_INIT

	m_strName = "";
	m_strReference = "";

	if ( nRoomNo == 0 )
		m_strRoomNo = "";
	else
		m_strRoomNo.Format( "%d", nRoomNo );

	m_nSelectedRoomIdx;
	m_bTraining = FALSE;

	for ( int nRoomIdx = 0; nRoomIdx < DataManagerNonDb.PMSRoom.GetSize(); nRoomIdx++ )
	{
		CPMSRoomStatusInfo infoRoom;
		PMSRoomStatusTable.GetRoomInfo( nRoomIdx, infoRoom );

		int nCurrentId = infoRoom.GetBookingId();
		int nExpectedId = PMSRoomStatusTable.GetExpectedBookingId( nRoomIdx );

		bool bInclude = ( nExpectedId != 0 ) && ( nCurrentId != nExpectedId );

		if ( TRUE == bInclude )
		{
			int nBookingIdx;
			if ( DataManagerNonDb.PMSBooking.FindBookingById( nExpectedId, nBookingIdx ) == TRUE )
			{
				CPMSBookingCSVRecord BookingRecord;
				DataManagerNonDb.PMSBooking.GetAt( nBookingIdx, BookingRecord );

				int nCustomerIdx;
				CPMSCustomerCSVRecord CustomerRecord;
				if ( DataManagerNonDb.PMSCustomer.FindCustomerById( BookingRecord.GetCustomerId(), nCustomerIdx ) == TRUE )
					DataManagerNonDb.PMSCustomer.GetAt( nCustomerIdx, CustomerRecord );

				CPMSBookingPartInfo infoPart;
				BookingRecord.GetPartInfo( 0, infoPart );

				CPMSAccountFindInfo infoBooking;
				infoBooking.m_nCheckInDay = infoPart.GetStartDay();
				infoBooking.m_nBookingId = BookingRecord.GetBookingId();
				infoBooking.m_nBookingPart = 0;
				infoBooking.m_nRoomNo = infoRoom.GetRoomNo();
				infoBooking.m_strRoomName = infoRoom.GetRoomName();
				infoBooking.m_nCustomerId = BookingRecord.GetCustomerId();
				infoBooking.m_nGuests = BookingRecord.GetGuests();
				infoBooking.m_strGuestName = CustomerRecord.GetGuestName();
				infoBooking.m_strBillingName = CustomerRecord.GetBillingName();
				infoBooking.m_nBookingPartStartDay = infoPart.GetStartDay();
				infoBooking.m_nBookingPartNights = infoPart.GetNights();
				infoBooking.m_bCheckedOut = FALSE;
				m_arrayBookingInfo.Add( infoBooking );
			}
		}
	}

	UpdateList();
}

/**********************************************************************/

void CPMSCheckInListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPMSCheckInListDlg)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_CHECK_TRAINING, m_checkTraining);
	DDX_Check(pDX, IDC_CHECK_TRAINING, m_bTraining);
	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
	DDV_MaxChars(pDX, m_strName, PMSCustomer::GuestName.Max);
	DDX_Text(pDX, IDC_EDIT_ROOM, m_strRoomNo);
	DDX_Text(pDX, IDC_EDIT_REFERENCE, m_strReference);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPMSCheckInListDlg, CDialog)
	//{{AFX_MSG_MAP(CPMSCheckInListDlg)
	//}}AFX_MSG_MAP
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST, OnGetDispInfoList)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDoubleClickList)
	ON_BN_CLICKED(IDC_BUTTON_UPDATE, OnButtonUpdate)
	ON_BN_CLICKED(IDC_BUTTON_PRINT, OnButtonPrint)
	ON_EN_KILLFOCUS(IDC_EDIT_REFERENCE, OnKillFocusReference)
	ON_BN_CLICKED(IDC_CHECK_TRAINING, OnButtonUpdate)
END_MESSAGE_MAP()

/**********************************************************************/

int CPMSCheckInListDlg::GetArraySize()
{
	return m_FilterArray.GetSize();
}

/**********************************************************************/

BOOL CPMSCheckInListDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_listData.SubclassDlgItem ( IDC_LIST, this );
	m_listData.InsertColumn ( 0, "Reference", LVCFMT_LEFT, 85 );
	m_listData.InsertColumn ( 1, "Booking Name", LVCFMT_LEFT, 130 );
	m_listData.InsertColumn ( 2, "No", LVCFMT_LEFT, 50 );
	m_listData.InsertColumn ( 3, "Room", LVCFMT_LEFT, 130 );
	m_listData.InsertColumn ( 4, "Nights", LVCFMT_LEFT, 50 );
	m_listData.InsertColumn ( 5, "Check Out", LVCFMT_LEFT, 85 );
	
	m_listData.SetItemCountEx ( GetArraySize() );
	m_listData.SetCurSel(0);

	SubclassEdit( IDC_EDIT_ROOM, SS_NUM, 4, "%s" );
	SubclassEdit( IDC_EDIT_REFERENCE, SS_NUM, 8, "%s" );

	m_checkTraining.SetCheck( FALSE );
		
	if ( PMSOptions.GetMiscEnableTrainingMode() == FALSE )
	{
		m_checkTraining.EnableWindow( FALSE );
		m_checkTraining.ShowWindow( SW_HIDE );
	}

	return TRUE;  
}

/**********************************************************************/
CEdit* CPMSCheckInListDlg::GetEditReference(){ return GetEdit( IDC_EDIT_REFERENCE ); }
/**********************************************************************/

void CPMSCheckInListDlg::OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	if (( pDispInfo->item.mask & LVIF_TEXT ) != 0 )
	{
		int nIndex = pDispInfo->item.iItem;
		PrepareLine( nIndex );
		CCSV csv ( m_strDisplayLine, '\t' );
		lstrcpy ( pDispInfo->item.pszText, csv.GetString ( pDispInfo->item.iSubItem ) );
	}
	*pResult = 0;
}

/**********************************************************************/

void CPMSCheckInListDlg::OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnOK();
	*pResult = 0;
}

/**********************************************************************/

void CPMSCheckInListDlg::OnKillFocusReference()
{
	UpdateData( TRUE, FALSE );
	int nRef = atoi( m_strReference );
	
	if ( nRef == 0 )
		GetEditReference() -> SetWindowText( "" );
	else
	{
		CString strRef;
		strRef.Format( "%8.8d", nRef );
		GetEditReference() -> SetWindowText( strRef );
	}	
}

/**********************************************************************/

bool CPMSCheckInListDlg::PrepareLine( int nIndex )
{
	if ( nIndex < 0 || nIndex >= GetArraySize() )
		return FALSE;

	int nFilterIdx = m_FilterArray.GetAt( nIndex );
	if ( nFilterIdx < 0 || nFilterIdx >= m_arrayBookingInfo.GetSize() )
		return FALSE;

	CPMSAccountFindInfo infoBooking = m_arrayBookingInfo.GetAt( nFilterIdx );

	COleDateTime dateOut;
	int nDateOut = infoBooking.m_nBookingPartStartDay + infoBooking.m_nBookingPartNights;
	GetOleDateSince2011( nDateOut, dateOut );

	CString strDateOut;
	strDateOut.Format( "%2.2d/%2.2d/%4.4d",
		dateOut.GetDay(),
		dateOut.GetMonth(),
		dateOut.GetYear() );

	CString strBookingId;
	strBookingId.Format( "%8.8d", infoBooking.m_nBookingId );

	CCSV csv( '\t' );
	csv.Add( strBookingId );
	csv.Add( infoBooking.m_strGuestName );
	csv.Add( infoBooking.m_nRoomNo );
	csv.Add( infoBooking.m_strRoomName );
	csv.Add( infoBooking.m_nBookingPartNights );
	csv.Add( strDateOut );
	
	m_strDisplayLine = csv.GetLine();
	
	return TRUE;
}

/**********************************************************************/

void CPMSCheckInListDlg::OnButtonUpdate()
{
	if ( UpdateData( TRUE ) == TRUE )
	{
		UpdateList();
		m_listData.SetItemCountEx ( GetArraySize() );
		m_listData.SetCurSel(0);
		m_listData.Invalidate();
	}
}

/**********************************************************************/

void CPMSCheckInListDlg::UpdateList()
{
	m_FilterArray.RemoveAll();

	int nRoomNo = atoi( m_strRoomNo );
	int nBookingId = atoi( m_strReference );

	for ( int nIdx = 0; nIdx < m_arrayBookingInfo.GetSize(); nIdx++ )
	{
		CPMSAccountFindInfo infoBooking = m_arrayBookingInfo.GetAt( nIdx );

		if ( nRoomNo != 0 )
			if ( infoBooking.m_nRoomNo != nRoomNo )
				continue;

		if ( nBookingId != 0 )
			if ( infoBooking.m_nBookingId != nBookingId )
				continue;

		if ( 0 == m_bTraining )
		{
			int nBookingIdx;
			if ( DataManagerNonDb.PMSBooking.FindBookingById( infoBooking.m_nBookingId, nBookingIdx ) == TRUE )
			{
				CPMSBookingCSVRecord BookingRecord;
				DataManagerNonDb.PMSBooking.GetAt( nBookingIdx, BookingRecord );

				if ( BookingRecord.GetTrainingFlag() == TRUE )
					continue;
			}
		}
		
		if ( m_strName != "" )
		{
			CString strTemp1 = m_strName;
			strTemp1.MakeUpper();

			CString strTemp2 = infoBooking.m_strGuestName;
			strTemp2.MakeUpper();

			if ( strTemp2.Find( strTemp1 ) == -1 )
				continue;
		}

		m_FilterArray.Add( nIdx );
	}
}

/**********************************************************************/

void CPMSCheckInListDlg::OnButtonPrint()
{
	CPMSAccountFindReportDlg dlgReport( m_arrayBookingInfo, m_FilterArray, TRUE, IDM_PMSMENUBAR_CHECK_IN, this );
	dlgReport.DoModal();
}

/**********************************************************************/

void CPMSCheckInListDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		int nResult = IDCANCEL;

		int nSel = m_listData.GetCurSel();
		if ( ( nSel >= 0 ) && ( nSel < m_FilterArray.GetSize() ) )
		{
			int nIdx = m_FilterArray.GetAt( nSel );
			if ( ( nIdx >= 0 ) && ( nIdx < m_arrayBookingInfo.GetSize() ) )
			{
				CPMSAccountFindInfo infoBooking = m_arrayBookingInfo.GetAt( nIdx );
				if ( DataManagerNonDb.PMSRoom.FindRoomByNumber( infoBooking.m_nRoomNo, m_nSelectedRoomIdx ) == TRUE )
					nResult = IDOK;
			}
		}

		EndDialog( nResult );
	}
}

/**********************************************************************/
#endif
/**********************************************************************/


