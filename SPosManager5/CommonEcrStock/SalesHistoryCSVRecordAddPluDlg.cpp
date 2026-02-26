/**********************************************************************/
 
/**********************************************************************/
//#include "DataManagerNonDb.h"
/**********************************************************************/
#include "SalesHistoryCSVRecordAddPluDlg.h"
/**********************************************************************/

CSalesHistoryCSVRecordAddPluDlg::CSalesHistoryCSVRecordAddPluDlg( CWnd* pParent)
	: CSalesHistoryCSVRecordAddDlg(pParent)
{
}

/**********************************************************************/

void CSalesHistoryCSVRecordAddPluDlg::PrepareComboAndTitle()
{
	m_comboReport.AddString ( "Custom Report" );
	m_comboReport.AddString( "All standard Reports" );

	CSalesHistoryCSVRecordPlu Report;
	DataManagerNonDb.SalesHistoryPlu.GetSystemReportList( m_arrayReportTypes );
			
	for ( int n = 0; n < m_arrayReportTypes.GetSize(); n++ )
	{
		Report.SetReportType ( m_arrayReportTypes.GetAt(n) );
		m_comboReport.AddString( Report.GetListName() );
	}

	SetWindowText ( "Add Plu Sales History Report" );

	m_arrayReportTypes.InsertAt( 0, SH_CUSTOM, 1 );
	m_arrayReportTypes.InsertAt( 1, 999, 1 );

	m_comboReport.SetCurSel(0);
}

/**********************************************************************/
