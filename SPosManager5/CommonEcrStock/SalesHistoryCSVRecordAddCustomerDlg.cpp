/**********************************************************************/
 
/**********************************************************************/
//#include "DataManagerNonDb.h"
/**********************************************************************/
#include "SalesHistoryCSVRecordAddCustomerDlg.h"
/**********************************************************************/

CSalesHistoryCSVRecordAddCustomerDlg::CSalesHistoryCSVRecordAddCustomerDlg( CWnd* pParent)
	: CSalesHistoryCSVRecordAddDlg(pParent)
{
}

/**********************************************************************/

void CSalesHistoryCSVRecordAddCustomerDlg::PrepareComboAndTitle()
{
	m_comboReport.AddString ( "Custom Report" );
	m_comboReport.AddString( "All standard Reports" );

	CSalesHistoryCSVRecordCustomer Report;
	DataManagerNonDb.SalesHistoryCustomer.GetSystemReportList( m_arrayReportTypes );
			
	for ( int n = 0; n < m_arrayReportTypes.GetSize(); n++ )
	{
		Report.SetReportType ( m_arrayReportTypes.GetAt(n) );
		m_comboReport.AddString( Report.GetListName() );
	}

	SetWindowText ( "Add Customer Sales Report" );

	m_arrayReportTypes.InsertAt( 0, SH_CUSTOM, 1 );
	m_arrayReportTypes.InsertAt( 1, 999, 1 );

	m_comboReport.SetCurSel(0);
}

/**********************************************************************/
