/**********************************************************************/
 
/**********************************************************************/
//#include "DataManagerNonDb.h"
#include "TimeSliceMapCSVArray.h"
/**********************************************************************/
#include "TimeSliceMapAddDlg.h"
/**********************************************************************/

CTimeSliceMapAddDlg::CTimeSliceMapAddDlg( CWnd* pParent)
	: CSalesHistoryCSVRecordAddDlg(pParent)
{
}

/**********************************************************************/

void CTimeSliceMapAddDlg::PrepareComboAndTitle()
{
	m_comboReport.AddString ( "Custom Time Slice Map" );
	m_comboReport.AddString( "All Standard Time Slice Maps" );

	CTimeSliceMapCSVRecord Report;
	DataManagerNonDb.TimeSliceList.GetSystemReportList( m_arrayReportTypes );
			
	for ( int n = 0; n < m_arrayReportTypes.GetSize(); n++ )
	{
		Report.SetReportType ( m_arrayReportTypes.GetAt(n) );
		m_comboReport.AddString( Report.GetListName() );
	}

	SetWindowText ( "Add Time Slice Map" );

	m_arrayReportTypes.InsertAt( 0, SH_CUSTOM, 1 );
	m_arrayReportTypes.InsertAt( 1, 999, 1 );

	m_comboReport.SetCurSel(0);
}

/**********************************************************************/
