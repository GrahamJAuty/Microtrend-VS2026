/**********************************************************************/
#ifdef COMPILE_PMS 
/**********************************************************************/
#include "PMSAccountFindReportDlg.h"
#include "PMSOptions.h"
#include "PMSRoomStatusTable.h"
/**********************************************************************/
#include "PMSCheckOutListDlg.h"
/**********************************************************************/

CPMSCheckOutListDlg::CPMSCheckOutListDlg( int nRoomNo, CWnd* pParent )
	: CSSDialog(CPMSCheckOutListDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPMSCheckOutListDlg)
	//}}AFX_DATA_INIT

	m_strName = "";
	
	if ( nRoomNo == 0 )
		m_strRoomNo = "";
	else
		m_strRoomNo.Format( "%d", nRoomNo );

	m_bAllAccounts = FALSE;
	m_bTraining = FALSE;

	m_nSelectedRoomIdx;

	for ( int nRoomIdx = 0; nRoomIdx < DataManagerNonDb.PMSRoom.GetSize(); nRoomIdx++ )
	{
		CPMSRoomStatusInfo infoRoom;
		PMSRoomStatusTable.GetRoomInfo( nRoomIdx, infoRoom );

		int nCurrentId = infoRoom.GetBookingId();
		int nExpectedId = PMSRoomStatusTable.GetExpectedBookingId( nRoomIdx );

		if ( nCurrentId != 0 )
		{
			int nBookingIdx;
			if ( DataManagerNonDb.PMSBooking.FindBookingById( nCurrentId, nBookingIdx ) == TRUE )
			{
				CPMSBookingCSVRecord BookingRecord;
				DataManagerNonDb.PMSBooking.GetAt( nBookingIdx, BookingRecord );

				int nCustomerIdx;
				CPMSCustomerCSVRecord CustomerRecord;
				if ( DataManagerNonDb.PMSCustomer.FindCustomerById( BookingRecord.GetCustomerId(), nCustomerIdx ) == TRUE )
					DataManagerNonDb.PMSCustomer.GetAt( nCustomerIdx, CustomerRecord );

				CPMSBookingPartInfo infoFirstPart;
				BookingRecord.GetPartInfo( 0, infoFirstPart );

				CPMSBookingPartInfo infoFinalPart;
				BookingRecord.GetFinalPartInfo( infoFinalPart );

				CPMSAccountFindInfo infoBooking;
				infoBooking.m_nCheckInDay = infoFirstPart.GetStartDay();
				infoBooking.m_nBookingId = BookingRecord.GetBookingId();
				infoBooking.m_nBookingPart = 0;
				infoBooking.m_nRoomNo = infoRoom.GetRoomNo();
				infoBooking.m_strRoomName = infoRoom.GetRoomName();
				infoBooking.m_nCustomerId = BookingRecord.GetCustomerId();
				infoBooking.m_nGuests = BookingRecord.GetGuests();
				infoBooking.m_strGuestName = CustomerRecord.GetGuestName();
				infoBooking.m_strBillingName = CustomerRecord.GetBillingName();
				infoBooking.m_nBookingPartStartDay = infoFinalPart.GetStartDay();
				infoBooking.m_nBookingPartNights = infoFinalPart.GetNights();
				infoBooking.m_bCheckedOut = FALSE;
				m_arrayBookingInfo.Add( infoBooking );
			}
		}
	}

	UpdateList();
}

/**********************************************************************/

void CPMSCheckOutListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPMSCheckOutListDlg)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_CHECK_TRAINING, m_checkTraining);
	DDX_Check(pDX, IDC_CHECK_TRAINING, m_bTraining);
	DDX_Check(pDX, IDC_CHECK_ALLACCOUNTS, m_bAllAccounts);
	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
	DDV_MaxChars(pDX, m_strName, PMSCustomer::GuestName.Max);
	DDX_Text(pDX, IDC_EDIT_ROOM, m_strRoomNo);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPMSCheckOutListDlg, CDialog)
	//{{AFX_MSG_MAP(CPMSCheckOutListDlg)
	//}}AFX_MSG_MAP
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST, OnGetDispInfoList)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDoubleClickList)
	ON_BN_CLICKED(IDC_BUTTON_UPDATE, OnButtonUpdate)
	ON_BN_CLICKED(IDC_BUTTON_PRINT, OnButtonPrint)
	ON_BN_CLICKED(IDC_CHECK_ALLACCOUNTS, OnToggleToday)
	ON_BN_CLICKED(IDC_CHECK_TRAINING, OnToggleToday)
END_MESSAGE_MAP()

/**********************************************************************/

int CPMSCheckOutListDlg::GetArraySize()
{
	return m_FilterArray.GetSize();
}

/**********************************************************************/

BOOL CPMSCheckOutListDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_listData.SubclassDlgItem ( IDC_LIST, this );
	m_listData.InsertColumn ( 0, "No", LVCFMT_LEFT, 50 );
	m_listData.InsertColumn ( 1, "Room", LVCFMT_LEFT, 130 );
	m_listData.InsertColumn ( 2, "Booking Name", LVCFMT_LEFT, 130 );
	m_listData.InsertColumn ( 3, "Check In", LVCFMT_LEFT, 85 );
	m_listData.InsertColumn ( 4, "Nights", LVCFMT_LEFT, 50 );
	m_listData.InsertColumn ( 5, "Check Out", LVCFMT_LEFT, 85 );
	
	m_listData.SetItemCountEx ( GetArraySize() );
	m_listData.SetCurSel(0);

	SubclassEdit( IDC_EDIT_ROOM, SS_NUM, 4, "%s" );

	m_checkTraining.SetCheck( FALSE );
		
	if ( PMSOptions.GetMiscEnableTrainingMode() == FALSE )
	{
		m_checkTraining.EnableWindow( FALSE );
		m_checkTraining.ShowWindow( SW_HIDE );
	}

	return TRUE;  
}

/**********************************************************************/

void CPMSCheckOutListDlg::OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult) 
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

void CPMSCheckOutListDlg::OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnOK();
	*pResult = 0;
}

/**********************************************************************/

bool CPMSCheckOutListDlg::PrepareLine( int nIndex )
{
	if ( nIndex < 0 || nIndex >= GetArraySize() )
		return FALSE;

	int nFilterIdx = m_FilterArray.GetAt( nIndex );
	if ( nFilterIdx < 0 || nFilterIdx >= m_arrayBookingInfo.GetSize() )
		return FALSE;

	CPMSAccountFindInfo infoBooking = m_arrayBookingInfo.GetAt( nFilterIdx );

	COleDateTime dateIn;
	int nDateIn = infoBooking.m_nBookingPartStartDay;
	GetOleDateSince2011( nDateIn, dateIn );

	CString strDateIn;
	strDateIn.Format( "%2.2d/%2.2d/%4.4d",
		dateIn.GetDay(),
		dateIn.GetMonth(),
		dateIn.GetYear() );

	COleDateTime dateOut;
	int nDateOut = nDateIn + infoBooking.m_nBookingPartNights;
	GetOleDateSince2011( nDateOut, dateOut );

	CString strDateOut;
	strDateOut.Format( "%2.2d/%2.2d/%4.4d",
		dateOut.GetDay(),
		dateOut.GetMonth(),
		dateOut.GetYear() );

	CCSV csv( '\t' );
	csv.Add( infoBooking.m_nRoomNo );
	csv.Add( infoBooking.m_strRoomName );
	csv.Add( infoBooking.m_strGuestName );
	csv.Add( strDateIn );
	csv.Add( infoBooking.m_nBookingPartNights );
	csv.Add( strDateOut );
	
	m_strDisplayLine = csv.GetLine();
	
	return TRUE;
}

/**********************************************************************/

void CPMSCheckOutListDlg::OnToggleToday()
{
	OnButtonUpdate();
}

/**********************************************************************/

void CPMSCheckOutListDlg::OnButtonUpdate()
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

void CPMSCheckOutListDlg::UpdateList()
{
	int nRoomNo = atoi( m_strRoomNo );

	m_FilterArray.RemoveAll();

	for ( int nIdx = 0; nIdx < m_arrayBookingInfo.GetSize(); nIdx++ )
	{
		CPMSAccountFindInfo infoBooking = m_arrayBookingInfo.GetAt( nIdx );

		if ( nRoomNo != 0 )
			if ( infoBooking.m_nRoomNo != nRoomNo )
				continue;

		if ( m_bAllAccounts == 0 )
			if ( infoBooking.m_nBookingPartStartDay + infoBooking.m_nBookingPartNights > PMSOptions.GetPMSDayNumber() )
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

void CPMSCheckOutListDlg::OnButtonPrint()
{
	CPMSAccountFindReportDlg dlgReport( m_arrayBookingInfo, m_FilterArray, TRUE, IDM_PMSMENUBAR_CHECK_OUT_ACTIVE, this );
	dlgReport.DoModal();
}

/**********************************************************************/

void CPMSCheckOutListDlg::OnOK() 
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
