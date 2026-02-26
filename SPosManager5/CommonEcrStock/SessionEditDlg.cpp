/**********************************************************************/
#include "MaxLengths.h"
/**********************************************************************/
#include "SessionEditDlg.h"
/**********************************************************************/

CSessionEditDlg::CSessionEditDlg( CSessionCSVRecord& Session, bool bAdhoc, CWnd* pParent)
	: CSSDialog(CSessionEditDlg::IDD, pParent), m_Session( Session )
{
	//{{AFX_DATA_INIT(CSessionEditDlg)
	m_strName = _T("");
	//}}AFX_DATA_INIT
	m_bAdhoc = bAdhoc;
	m_nSessionStartType = Session.GetSessionStartType();
}

/**********************************************************************/

void CSessionEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSessionEditDlg)
	DDX_Control(pDX, IDC_STATIC_GENERAL, m_staticGeneral);
	DDX_Control(pDX, IDC_STATIC_NAME, m_staticName);
	DDX_Control(pDX, IDC_EDIT_NAME, m_editName);
	DDX_Control(pDX, IDC_STATIC_START, m_staticStart);
	DDX_Control(pDX, IDC_TIMEPICKER_START, m_TimePickerStart);
	DDX_Control(pDX, IDC_STATIC_END, m_staticEnd);
	DDX_Control(pDX, IDC_TIMEPICKER_END, m_TimePickerEnd);
	DDX_Control(pDX, IDC_STATIC_MATCH, m_staticMatch);
	DDX_Control(pDX, IDC_COMBO_MATCH, m_comboMatch);
	DDX_Control(pDX, IDC_STATIC_SESSION, m_staticSession);
	DDX_Radio(pDX, IDC_RADIO_STARTTIME, m_nSessionStartType);
	DDX_Control(pDX, IDC_RADIO_STARTTIME2, m_radioStartTime2);
	DDX_Control(pDX, IDC_STATIC_LIST, m_staticList);
	DDX_Control(pDX, IDC_COMBO_LIST, m_comboList);
	DDX_Control(pDX, IDOK, m_buttonOK);
	DDX_Control(pDX, IDCANCEL, m_buttonCancel);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
	DDV_MaxChars(pDX, m_strName, MAX_LENGTH_GENERAL_NAME);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CSessionEditDlg, CDialog)
	//{{AFX_MSG_MAP(CSessionEditDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CSessionEditDlg::OnInitDialog() 
{
	m_strName = m_Session.GetName();

	CDialog::OnInitDialog();	

	m_TimePickerStart.SetFormat ( "HH:mm" );
	m_TimePickerEnd.SetFormat ( "HH:mm" );

	CTime timeStart = CTime ( 1999, 1, 1, m_Session.GetSessionStartHour(), m_Session.GetSessionStartMinute(), 0, -1 );
	CTime timeEnd = CTime ( 1999, 1, 1, m_Session.GetSessionEndHour(), m_Session.GetSessionEndMinute(), 0, -1 );

	m_TimePickerStart.SetTime ( &timeStart );
	m_TimePickerEnd.SetTime ( &timeEnd );

	COleDateTime timeDayStart = COleDateTime( 2005, 1, 1, EcrmanOptions.GetReportsDayStartHour(), EcrmanOptions.GetReportsDayStartMinute(), 0 );
	COleDateTime timeDayEnd = timeDayStart - COleDateTimeSpan( 0, 0, 1, 0 );

	CString strText;
	strText.Format ( "Relative to business day (%2.2d:%2.2d to %2.2d:%2.2d)",
		timeDayStart.GetHour(), timeDayStart.GetMinute(), timeDayEnd.GetHour(), timeDayEnd.GetMinute() );

	m_radioStartTime2.SetWindowText( strText );

	m_comboMatch.AddString( "Separately for each selected date" );
	m_comboMatch.AddString( "From start date/time to end date/time" );
	m_comboMatch.SetCurSel( m_Session.GetContinuousFlag() ? 1 : 0 );

	if ( DealerFlags.GetSalesHistoryDateControlFlag() == TRUE )
	{
		m_comboList.AddString( "Calendar date of sale" );
		m_comboList.AddString( "Business date of sale" );
		m_comboList.AddString( "Calendar date of session start" );
		m_comboList.AddString( "Business date of session start" );
		m_comboList.SetCurSel( m_Session.GetHistoryDateType() );
	}
	else
	{
		m_staticList.ShowWindow( SW_HIDE );
		m_comboList.EnableWindow( FALSE );
		m_comboList.ShowWindow( SW_HIDE );
	}

	if ( TRUE == m_bAdhoc )
	{
		m_staticName.ShowWindow( SW_HIDE );
		m_editName.ShowWindow( SW_HIDE );
		SetWindowText( "Adhoc Report Session" );
		MoveControl( &m_staticStart, 23, 24 );
		MoveControl( &m_TimePickerStart, 65, 24 );
		MoveControl( &m_staticEnd, 23, 44 );
		MoveControl( &m_TimePickerEnd, 65, 44 );
		MoveControl( &m_staticMatch, 23, 82 );
		MoveControl( &m_comboMatch, 23, 96 );
		MoveControl( &m_staticList, 225, 82 );
		MoveControl( &m_comboList, 225, 96 );
		ResizeControl( &m_staticGeneral, 196, 124 );
		ResizeControl( &m_staticSession, 196, 124 );
		MoveControl( &m_buttonOK, 299, 141 );
		MoveControl( &m_buttonCancel, 355, 141 );
		ResizeDialog( 412, 162 );
	}

	return TRUE;  
}

/**********************************************************************/

void CSessionEditDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		CTime timeStart, timeEnd;
		m_TimePickerStart.GetTime ( timeStart );
		m_TimePickerEnd.GetTime ( timeEnd );

		::TrimSpaces ( m_strName, FALSE );
		if ( ( m_strName == "" ) || ( TRUE == m_bAdhoc ) )
		{
			m_strName.Format ( "%2.2d:%2.2d to %2.2d:%2.2d",
				timeStart.GetHour(),
				timeStart.GetMinute(),
				timeEnd.GetHour(),
				timeEnd.GetMinute() );
		}

		m_Session.SetName( m_strName );
		m_Session.SetSessionStartHour( timeStart.GetHour() );
		m_Session.SetSessionStartMinute( timeStart.GetMinute() );
		m_Session.SetSessionEndHour( timeEnd.GetHour() );
		m_Session.SetSessionEndMinute( timeEnd.GetMinute() );
		m_Session.SetSessionStartType( m_nSessionStartType );
		m_Session.SetContinuousFlag( m_comboMatch.GetCurSel() == 1 );
		
		if (DealerFlags.GetSalesHistoryDateControlFlag() == TRUE)
		{
			m_Session.SetHistoryDateType(m_comboList.GetCurSel());
		}

		EndDialog ( IDOK );
	}
}

/**********************************************************************/
