/**********************************************************************/
 
/**********************************************************************/
//#include "globalfunctions.h"
#include "MaxLengths.h"
/**********************************************************************/
#include "TimeSliceEditDlg.h"
/**********************************************************************/

CTimeSliceEditDlg::CTimeSliceEditDlg( CTimeSliceCSVRecord& TimeSlice, CWnd* pParent)
	: CSSDialog(CTimeSliceEditDlg::IDD, pParent), m_TimeSlice( TimeSlice )
{
	//{{AFX_DATA_INIT(CTimeSliceEditDlg)
	m_strName = _T("");
	//}}AFX_DATA_INIT
}

/**********************************************************************/

void CTimeSliceEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTimeSliceEditDlg)
	DDX_Control(pDX, IDC_TIMEPICKER_START, m_TimePickerStart);
	DDX_Control(pDX, IDC_TIMEPICKER_END, m_TimePickerEnd);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
	DDV_MaxChars(pDX, m_strName, MAX_LENGTH_GENERAL_NAME);
	DDX_Text(pDX, IDC_EDIT_GROUP, m_nGroup );
	DDV_MinMaxInt(pDX, m_nGroup, 0, 99 );
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CTimeSliceEditDlg, CDialog)
	//{{AFX_MSG_MAP(CTimeSliceEditDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CTimeSliceEditDlg::OnInitDialog() 
{
	m_strName = m_TimeSlice.GetName();
	m_nGroup = m_TimeSlice.GetSortOrder();

	CDialog::OnInitDialog();	
	SubclassEdit( IDC_EDIT_GROUP, SS_NUM, 2, "%d" );

	m_TimePickerStart.SetFormat ( "HH:mm" );
	m_TimePickerEnd.SetFormat ( "HH:mm" );

	CTime timeStart = CTime ( 1999, 1, 1, m_TimeSlice.GetStart() / 60, m_TimeSlice.GetStart() % 60, 0, -1 );
	CTime timeEnd = CTime ( 1999, 1, 1, m_TimeSlice.GetEnd() / 60, m_TimeSlice.GetEnd() % 60, 0, -1 );

	m_TimePickerStart.SetTime ( &timeStart );
	m_TimePickerEnd.SetTime ( &timeEnd );

	return TRUE;  
}

/**********************************************************************/

void CTimeSliceEditDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		CTime timeStart, timeEnd;
		m_TimePickerStart.GetTime ( timeStart );
		m_TimePickerEnd.GetTime ( timeEnd );

		::TrimSpaces ( m_strName, FALSE );
		if ( m_strName == "" )
		{
			m_strName.Format ( "%2.2d:%2.2d to %2.2d:%2.2d",
				timeStart.GetHour(),
				timeStart.GetMinute(),
				timeEnd.GetHour(),
				timeEnd.GetMinute() );
		}

		m_TimeSlice.SetName( m_strName );
		m_TimeSlice.SetSortOrder( m_nGroup );
		m_TimeSlice.SetStart( ( timeStart.GetHour() * 60 ) + timeStart.GetMinute() );
		m_TimeSlice.SetEnd( ( timeEnd.GetHour() * 60 ) + timeEnd.GetMinute() );

		EndDialog ( IDOK );
	}
}

/**********************************************************************/
