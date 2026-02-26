/**********************************************************************/
#ifdef COMPILE_PMS 
/**********************************************************************/
#include "..\CommonEcrStock\ListDataUnitDlg.h"
#include "PMSRoomStatusTable.h"
/**********************************************************************/
#include "PMSCloseRoomDlg.h"
/**********************************************************************/
IMPLEMENT_DYNAMIC(CPMSCloseRoomDlg, CDialog)
/**********************************************************************/

CPMSCloseRoomDlg::CPMSCloseRoomDlg( bool bEdit, int nRoomIdx, CPMSBookingResponseInfo& Response, CPMSClosedRoomCSVRecord& ClosureRecord, CWnd* pParent )
	: CSSDialog(CPMSCloseRoomDlg::IDD, pParent), m_Response ( Response ), m_ClosureRecord( ClosureRecord ) 
{
	m_bEdit = bEdit;

	DataManagerNonDb.PMSRoom.GetAt( nRoomIdx, m_Room );

	CPMSRoomStatusInfo infoRoom;
	PMSRoomStatusTable.GetRoomInfo( nRoomIdx, infoRoom );

	m_nSearchCheckInDay = ClosureRecord.GetStartDay();
	m_nSearchCheckOutDay = ClosureRecord.GetStartDay() + ClosureRecord.GetNights();
		
	m_nSearchCheckInBase = m_nSearchCheckInDay;
	m_nSearchCheckOutBase = m_nSearchCheckOutDay;

	m_nAction = 0;
}

/**********************************************************************/

CPMSCloseRoomDlg::~CPMSCloseRoomDlg()
{
}

/**********************************************************************/

void CPMSCloseRoomDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control( pDX, IDC_STATIC_MAXNIGHTS, m_staticMaxNights );

	DDX_Control(pDX, IDC_COMBO_CHECKIN, m_comboCheckIn);
	DDX_Control(pDX, IDC_DATEPICKER_CHECKIN, m_datePickerCheckIn);

	DDX_Control(pDX, IDC_COMBO_CHECKOUT, m_comboCheckOut);
	DDX_Control(pDX, IDC_DATEPICKER_CHECKOUT, m_datePickerCheckOut);
	
	DDX_Control(pDX, IDC_BUTTON_ACTION1, m_buttonAction1);
	DDX_Control(pDX, IDC_BUTTON_ACTION2, m_buttonAction2);

	DDX_Control(pDX, IDC_COMBO_REASON, m_comboReason);
	DDX_Control(pDX, IDC_EDIT_REASON, m_editReason);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPMSCloseRoomDlg, CDialog)

	ON_CBN_SELCHANGE(IDC_COMBO_CHECKIN, OnChangeDateCheckIn)
	ON_CBN_DROPDOWN(IDC_COMBO_CHECKIN, OnOpenCheckIn)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATEPICKER_CHECKIN, OnDatePickerCheckIn)
	
	ON_EN_KILLFOCUS(IDC_EDIT_NIGHTS,OnKillFocusNights)

	ON_CBN_SELCHANGE(IDC_COMBO_CHECKOUT, OnChangeDateCheckOut)
	ON_CBN_DROPDOWN(IDC_COMBO_CHECKOUT, OnOpenCheckOut)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATEPICKER_CHECKOUT, OnDatePickerCheckOut)

	ON_BN_CLICKED( IDC_BUTTON_ACTION1, OnButtonAction1 )
	ON_BN_CLICKED( IDC_BUTTON_ACTION2, OnButtonAction2 )

	ON_CBN_SELCHANGE(IDC_COMBO_REASON, OnSelectReason)
	ON_BN_CLICKED(IDC_BUTTON_REASON, OnButtonReason)

END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPMSCloseRoomDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_editReason.LimitText( PMSClosure::Reason.Max );

	CString strRoomName;
	::GetPMSRoomText( m_Room.GetRoomNo(), m_Room.GetRoomName(), strRoomName );

	CString strAction;
	if ( FALSE == m_bEdit )
		strAction = "Close Room";
	else
		strAction = "Edit Closure";

	CString strTitle;
	strTitle.Format( "%s (%s, %s)",
		(const char*) strAction,
		(const char*) strRoomName,
		m_Room.GetRoomTypeName(FALSE) );

	SetWindowText( strTitle );

	m_datePickerCheckIn.SetFormat( "'calendar'" );
	m_datePickerCheckOut.SetFormat( "'calendar'" );

	SubclassEdit( IDC_EDIT_NIGHTS, SS_NUM, 3, "%d" );
	
	CArray<int, int> arrayStops;
	arrayStops.Add(5);
	arrayStops.Add(35);
	arrayStops.Add(55);
	arrayStops.Add(80);
	m_comboCheckIn.SetTabStops( arrayStops );
	m_comboCheckOut.SetTabStops( arrayStops );

	FillCheckInCombo();
	UpdateNights();
	UpdateCheckOutDay();
	FillCheckOutCombo();

	COleDateTime dateCheckInMin, dateCheckInMax, dateCheckOutMin, dateCheckOutMax;
	GetOleDateSince2011( m_Response.m_nMinStartDay, dateCheckInMin );
	GetOleDateSince2011( m_Response.m_nMaxStartDay, dateCheckInMax );
	GetOleDateSince2011( m_Response.m_nMinStartDay + 1, dateCheckOutMin );
	GetOleDateSince2011( m_Response.m_nMaxStartDay + 1, dateCheckOutMax );
	
	m_datePickerCheckIn.SetRange( &dateCheckInMin, &dateCheckInMax );
	m_datePickerCheckOut.SetRange( &dateCheckOutMin, &dateCheckOutMax );	
	UpdateDatePickerDates();

	UpdateMaxNightsDisplay();

	if ( FALSE == m_bEdit )
	{
		m_buttonAction1.EnableWindow( FALSE );
		m_buttonAction1.ShowWindow( SW_HIDE );
		m_buttonAction2.SetWindowText( "Close\nRoom" );
	}
	else
	{
		m_buttonAction1.SetWindowText( "Delete\nClosure" );
		m_buttonAction2.SetWindowText( "Save\nChanges" );
	}

	FillReasonCombo( m_ClosureRecord.GetReason() );

	return TRUE;  
}

/**********************************************************************/
CEdit* CPMSCloseRoomDlg::GetEditNights(){ return GetEdit( IDC_EDIT_NIGHTS ); }
/**********************************************************************/

void CPMSCloseRoomDlg::UpdateMaxNightsDisplay()
{
	m_nSearchCheckInDay = m_nSearchCheckInBase + m_comboCheckIn.GetCurSel();
	int nMaxNights = ( m_Response.m_nMaxStartDay + 1 ) - m_nSearchCheckInDay;

	switch( nMaxNights )
	{
	case 0:		
		m_staticMaxNights.SetWindowText( "" );		
		break;

	case 1:		
		m_staticMaxNights.SetWindowText( "( Maximum 1 night )" );
		break;

	default:
		{
			if ( nMaxNights > PMSBooking::Nights.Max )
				nMaxNights = PMSBooking::Nights.Max;

			CString str;
			str.Format( "( Maximum %d nights )",
				nMaxNights );

			m_staticMaxNights.SetWindowText( str );
		}
		break;
	}
}

/**********************************************************************/

void CPMSCloseRoomDlg::FillCheckInCombo()
{
	FillDateCombo( m_comboCheckIn, m_nSearchCheckInBase, m_nSearchCheckInDay, FALSE );
}

/**********************************************************************/

void CPMSCloseRoomDlg::FillCheckOutCombo()
{
	FillDateCombo( m_comboCheckOut, m_nSearchCheckOutBase, m_nSearchCheckOutDay, TRUE );
}

/**********************************************************************/

void CPMSCloseRoomDlg::FillDateCombo( CSSTabbedComboBox& combo, int& nBaseDay, int nSelDay, bool bCheckout )
{
	int nMinDay = m_Response.m_nMinStartDay;
	int nMaxDay = m_Response.m_nMaxStartDay;

	if ( TRUE == bCheckout )
	{
		nMinDay++;
		nMaxDay++;
	}

	nBaseDay = nSelDay - 7;
	
	if ( nBaseDay < nMinDay )
		nBaseDay = nMinDay;

	int nEndDay = nSelDay + 7;
	if ( nEndDay > nMaxDay )
		nEndDay = nMaxDay;

	if ( nBaseDay > nEndDay )
		nBaseDay = nEndDay;

	combo.ResetContent();

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
		combo.AddItem( csv.GetLine() );
	}

	combo.SetCurSel( nSelDay - nBaseDay );
}

/**********************************************************************/

void CPMSCloseRoomDlg::OnOpenCheckIn()
{
	FillCheckInCombo();
}

/**********************************************************************/

void CPMSCloseRoomDlg::OnOpenCheckOut()
{
	FillCheckOutCombo();
}

/**********************************************************************/

void CPMSCloseRoomDlg::UpdateNights()
{
	int nNights = m_nSearchCheckOutDay - m_nSearchCheckInDay;
	if ( ( nNights >= 1 ) && ( nNights <= PMSBooking::Nights.Max ) )
	{
		CString strNights;
		strNights.Format( "%d", nNights );
		GetEditNights() -> SetWindowText( strNights );
	}
	else
		GetEditNights() -> SetWindowText( "" );
}

/**********************************************************************/

void CPMSCloseRoomDlg::UpdateCheckOutDay()
{
	CString strNights;
	GetEditNights() -> GetWindowText( strNights );
	int nNights = atoi( strNights );

	if ( ( nNights < 1 ) || ( nNights > PMSBooking::Nights.Max ) )
		nNights = 1;

	m_nSearchCheckOutDay = m_nSearchCheckInDay + nNights;

	if ( m_nSearchCheckOutDay > m_Response.m_nMaxStartDay + 1 )
		m_nSearchCheckOutDay = m_Response.m_nMaxStartDay + 1;
}

/**********************************************************************/

void CPMSCloseRoomDlg::UpdateDatePickerDates()
{
	{
		int nDay = m_nSearchCheckInDay;
		
		if ( ( nDay < 0 ) || ( nDay > MAX_PMS_DAY - 1 ) )
			nDay = 0;

		COleDateTime date;
		GetOleDateSince2011( nDay, date );
		m_datePickerCheckIn.SetTime( date );
	}

	{
		int nDay = m_nSearchCheckOutDay;
		
		if ( ( nDay < 1 ) || ( nDay > MAX_PMS_DAY ) )
			nDay = 1;

		COleDateTime date;
		GetOleDateSince2011( nDay, date );
		m_datePickerCheckOut.SetTime( date );
	}
}

/**********************************************************************/

void CPMSCloseRoomDlg::OnChangeDateCheckIn() 
{
	m_nSearchCheckInDay = m_nSearchCheckInBase + m_comboCheckIn.GetCurSel();
	
	UpdateCheckOutDay();	
	UpdateNights();
	FillCheckOutCombo();
	UpdateDatePickerDates();
	UpdateMaxNightsDisplay();
}

/**********************************************************************/

void CPMSCloseRoomDlg::OnDatePickerCheckIn(NMHDR* pNMHDR, LRESULT* pResult) 
{
	COleDateTime date;
	m_datePickerCheckIn.GetTime( date );

	int nDate = GetDayNumberSince2011( date );

	if ( ( nDate >= m_Response.m_nMinStartDay ) && ( nDate <= m_Response.m_nMaxStartDay ) )
	{
		m_nSearchCheckInDay = nDate;

		FillCheckInCombo();
		UpdateCheckOutDay();
		UpdateNights();
		FillCheckOutCombo();
	}
	
	UpdateDatePickerDates();
	*pResult = 0;
}

/**********************************************************************/

void CPMSCloseRoomDlg::OnKillFocusNights()
{
	ValidateNights( FALSE );
}

/**********************************************************************/

bool CPMSCloseRoomDlg::ValidateNights( bool bPrompt )
{
	CString str;
	GetEditNights() -> GetWindowText( str );
	int nNights = atoi( str );

	if ( ( nNights < 1 ) || ( nNights > PMSBooking::Nights.Max ) )
	{
		if ( TRUE == bPrompt )
		{
			CString strMsg;
			strMsg.Format ( "Please specify the required length of closure\nbetween 1 and %d nights.", PMSBooking::Nights.Max );
			Prompter.Error( strMsg );
		}

		return FALSE;
	}

	{
		m_nSearchCheckInDay = m_nSearchCheckInBase + m_comboCheckIn.GetCurSel();
		int nMaxNights = ( m_Response.m_nMaxStartDay + 1 ) - m_nSearchCheckInDay;

		if ( nNights <= nMaxNights )
		{
			m_nSearchCheckOutDay = m_nSearchCheckInDay + nNights;
			UpdateNights();
			FillCheckOutCombo();
			UpdateDatePickerDates();
			return TRUE;
		}
		else
		{
			if ( TRUE == bPrompt )
			{
				CString strMaxNights;
				if ( 1 == nMaxNights )
					strMaxNights = "1 night";
				else
					strMaxNights.Format( "%d nights", nMaxNights );

				COleDateTime dateCheckIn;
				GetOleDateSince2011( m_nSearchCheckInDay, dateCheckIn );

				CString strDate;
				strDate.Format( "%2.2d/%2.2d/%4.4d",
					dateCheckIn.GetDay(),
					dateCheckIn.GetMonth(),
					dateCheckIn.GetYear() );

				CString strError = "";
				strError += "This room is already booked for a guest arriving\n";
				strError += strMaxNights;
				strError += " after the closure date of ";
				strError += strDate;
				Prompter.Error( strError );
			}

			return FALSE;
		}
	}

	return FALSE;
}

/**********************************************************************/

void CPMSCloseRoomDlg::OnChangeDateCheckOut() 
{
	m_nSearchCheckInDay = m_nSearchCheckInBase + m_comboCheckIn.GetCurSel();
	m_nSearchCheckOutDay = m_nSearchCheckOutBase + m_comboCheckOut.GetCurSel();
	
	UpdateNights();
	UpdateDatePickerDates();
}

/**********************************************************************/

void CPMSCloseRoomDlg::OnDatePickerCheckOut(NMHDR* pNMHDR, LRESULT* pResult) 
{
	COleDateTime date;
	m_datePickerCheckOut.GetTime( date );

	int nDate = GetDayNumberSince2011( date );

	if ( ( nDate >= m_Response.m_nMinStartDay + 1 ) && ( nDate <= m_Response.m_nMaxStartDay + 1 ) )
	{
		m_nSearchCheckOutDay = nDate;

		UpdateNights();
		FillCheckOutCombo();
	}

	UpdateDatePickerDates();
	*pResult = 0;
}

/**********************************************************************/

void CPMSCloseRoomDlg::OnSelectReason()
{
	int nSel = m_comboReason.GetCurSel();
	
	if ( nSel == m_comboReason.GetCount() - 1 )
	{
		m_editReason.ShowWindow( SW_SHOW );
		m_editReason.EnableWindow( TRUE );
	}
	else
	{
		m_editReason.ShowWindow( SW_HIDE );
		m_editReason.EnableWindow( FALSE );

	}
}

/**********************************************************************/

void CPMSCloseRoomDlg::OnButtonReason()
{
	CDataManagerInfo info;
	if ( DataManagerNonDb.OpenPMSClosureReasonsBuffer( info ) == FALSE )
	{
		Prompter.ShareError( info );
		return;
	}

	bool bCustom = FALSE;
	CString strReason = "";
	GetSelectedReason( strReason, bCustom );
	
	CString strDlgReason = "";
	if (FALSE == bCustom)
	{
		strDlgReason = strReason;
	}

	CListDataUnitDlg dlg ( DataManagerNonDb.PMSClosureReasons, strDlgReason, this );	
	
	if ( dlg.DoModal() == IDOK )
	{
		if (DataManagerNonDb.WritePMSClosureReasonsBuffer(info) == FALSE)
		{
			Prompter.WriteError(info);
		}
		else
		{
			CString strComboTarget = "";
			if (TRUE == bCustom)
			{
				strComboTarget = strReason;
			}
			else
			{
				strComboTarget = dlg.GetSelectedText();
			}

			FillReasonCombo(strComboTarget);
		}
	}
	
	DataManagerNonDb.ClosePMSClosureReasonsBuffer( info );
}

/**********************************************************************/

void CPMSCloseRoomDlg::OnButtonAction1()
{
	if ( Prompter.YesNo( "Are you sure you wish to delete\nthis room closure.", FALSE ) == IDYES )
	{
		m_nAction = 1;
		EndDialog( IDOK );
	}
}

/**********************************************************************/

void CPMSCloseRoomDlg::OnButtonAction2()
{
	if ( ValidateEntry() == TRUE )
	{
		m_nAction = 2;
		EndDialog( IDOK );
	}
}

/**********************************************************************/

void CPMSCloseRoomDlg::OnOK()
{
	OnButtonAction2();
}

/**********************************************************************/

void CPMSCloseRoomDlg::OnCancel()
{
	EndDialog( IDCANCEL );
}

/**********************************************************************/

bool CPMSCloseRoomDlg::ValidateEntry()
{	
	if ( UpdateData( TRUE ) == FALSE )
		return FALSE;

	if ( m_nSearchCheckOutDay <= m_nSearchCheckInDay )
	{
		Prompter.Error( "The end date for this room closure\nmust be after the start date" );
		return FALSE;
	}

	if ( ValidateNights( TRUE ) == FALSE )
	{
		GetEditNights() -> SetFocus();
		GetEditNights() -> SetSel( 0, -1 );
		return FALSE;
	}

	m_ClosureRecord.SetStartDay( m_nSearchCheckInDay );
	m_ClosureRecord.SetNights( m_nSearchCheckOutDay - m_nSearchCheckInDay );
	
	CString strReason = "";
	bool bCustom = FALSE;
	GetSelectedReason( strReason, bCustom );
	m_ClosureRecord.SetReason( strReason );

	return TRUE;
}

/**********************************************************************/

void CPMSCloseRoomDlg::GetSelectedReason( CString& strReason, bool& bCustom )
{
	strReason = "";
	bCustom = TRUE;

	int nSel = m_comboReason.GetCurSel();
	int nCount = DataManagerNonDb.PMSClosureReasons.GetSize();

	if ( ( nSel >= 0 ) && ( nSel < nCount ) )
	{
		strReason = DataManagerNonDb.PMSClosureReasons.GetText( nSel );
		bCustom = FALSE;
	}
	else if ( nSel == nCount )
	{
		m_editReason.GetWindowText( strReason );
		bCustom = TRUE;
	}
}

/**********************************************************************/

void CPMSCloseRoomDlg::FillReasonCombo( const char* szReason )
{
	m_comboReason.ResetContent();

	CString strReason = szReason;
	strReason.MakeUpper();

	int nReasonPos = -1;
	for ( int n = 0; n < DataManagerNonDb.PMSClosureReasons.GetSize(); n++ )
	{
		CString strArrayReason = DataManagerNonDb.PMSClosureReasons.GetText(n);
		m_comboReason.AddString( strArrayReason );

		if ( -1 == nReasonPos )
		{
			strArrayReason.MakeUpper();
			
			if ( strArrayReason == strReason )
				nReasonPos = n;
		}
	}

	m_comboReason.AddString ( "Other (Please specify below)" );

	if ( nReasonPos != -1 )
	{
		m_comboReason.SetCurSel( nReasonPos );
		m_editReason.ShowWindow( SW_HIDE );
	}
	else
	{
		m_comboReason.SetCurSel( DataManagerNonDb.PMSClosureReasons.GetSize() );
		m_editReason.SetWindowText( szReason );
	}
}

/**********************************************************************/
#endif
/**********************************************************************/
