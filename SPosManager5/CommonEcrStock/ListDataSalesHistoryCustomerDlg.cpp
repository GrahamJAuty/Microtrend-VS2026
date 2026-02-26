/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "SalesHistoryCSVRecordAddCustomerDlg.h"
#include "SalesHistoryCSVRecordEditCustomerDlg.h"
/**********************************************************************/
#include "ListDataSalesHistoryCustomerDlg.h"
/**********************************************************************/

CSingleListContentHelpersSalesHistoryCustomer::CSingleListContentHelpersSalesHistoryCustomer()
{
}

/**********************************************************************/

int CSingleListContentHelpersSalesHistoryCustomer::GetArraySize()
{
	return DataManagerNonDb.SalesHistoryCustomer.GetSize();
}

/**********************************************************************/

bool CSingleListContentHelpersSalesHistoryCustomer::PrepareLine( int nIndex, CString& strLine )
{
	strLine = "";

	bool bResult = FALSE;
	if ( ( nIndex >= 0 ) && ( nIndex < GetArraySize() ) )
	{
		CSalesHistoryCSVRecordCustomer ReportRecord;
		DataManagerNonDb.SalesHistoryCustomer.GetAt( nIndex, ReportRecord );

		CCSV csvOut ( '\t' );

		if ( ReportRecord.GetReportType() == SH_CUSTOM )
			csvOut.Add( "Custom" );
		else
			csvOut.Add( "Standard" );
				
		csvOut.Add( ReportRecord.GetListName() );
		strLine = csvOut.GetLine();

		bResult = TRUE;
	}
	
	return bResult;
}

/**********************************************************************/

void CSingleListContentHelpersSalesHistoryCustomer::DeleteDatabaseEntry( int nIndex )
{
	if ( ( nIndex >= 0 ) && ( nIndex < DataManagerNonDb.SalesHistoryCustomer.GetSize() ) )
	{
		DataManagerNonDb.SalesHistoryCustomer.RemoveAt ( nIndex );
		m_bDeletionEffect = TRUE;
	}
}

/**********************************************************************/

void CSingleListContentHelpersSalesHistoryCustomer::SetColumns( CSSListCtrlVirtual& list, CWnd* pParent )
{
	list.SubclassDlgItem ( IDC_LIST, pParent );
	list.InsertColumn ( 0, "Type", LVCFMT_LEFT, 100 );
	list.InsertColumn ( 1, "Report Name", LVCFMT_LEFT, 380 );
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CListDataSalesHistoryCustomerDlg::CListDataSalesHistoryCustomerDlg( int nInitialSel, CWnd* pParent) : CListDataDlg ( pParent )
{
	m_nInitialSel = nInitialSel;

	m_pContentHelpers = new CSingleListContentHelpersSalesHistoryCustomer();
	m_pDisplayHelpers = new CSingleListDisplayHelpers( m_listData, *m_pContentHelpers );

	m_pDisplayHelpers -> SetTopSingleText( "report" );
	m_pDisplayHelpers -> SetTopPluralText( "reports" );
	m_pDisplayHelpers -> SetBottomSingleText( "report" );
	m_pDisplayHelpers -> SetBottomPluralText( "reports" );
}

/**********************************************************************/

CListDataSalesHistoryCustomerDlg::~CListDataSalesHistoryCustomerDlg()
{
	delete m_pDisplayHelpers;
	delete m_pContentHelpers;
}

/**********************************************************************/

void CListDataSalesHistoryCustomerDlg::SetTitle()
{
	SetWindowText ( "Setup Customer Sales Reports" );
}

/**********************************************************************/

void CListDataSalesHistoryCustomerDlg::SetButtonTexts()
{
	m_button1.SetWindowText ( "&Add" );
	m_button2.SetWindowText ( "&Edit" );
	m_button3.SetWindowText ( "&Delete" );
	m_button4.SetWindowText ( "Move &Up" );
	m_button5.SetWindowText ( "Move D&own" );
	m_button6.SetWindowText ( "&Copy" );

	MoveControl( &m_button1, 368, 7 );
	MoveControl( &m_button2, 368, 42 );
	MoveControl( &m_button3, 368, 77 );
	MoveControl( &m_button4, 368, 122 );
	MoveControl( &m_button5, 368, 157 );
	MoveControl( &m_button6, 368, 202 );
}

/**********************************************************************/

void CListDataSalesHistoryCustomerDlg::SetColumns()
{
	m_pContentHelpers -> SetColumns( m_listData, this );
}

/**********************************************************************/

void CListDataSalesHistoryCustomerDlg::PrepareList()
{
	CListDataDlg::PrepareList();

	if ( m_nInitialSel >= 100 )
		SelectLine( m_nInitialSel - 100 );
	else if ( m_nInitialSel != 0 )
	{
		for ( int nIndex = 0; nIndex < DataManagerNonDb.SalesHistoryCustomer.GetSize(); nIndex++ )
		{
			if ( DataManagerNonDb.SalesHistoryCustomer.GetReportType( nIndex ) == m_nInitialSel )
			{
				SelectLine( nIndex );
				break;
			}
		}
	}
}

/**********************************************************************/

void CListDataSalesHistoryCustomerDlg::AddOrFindReport( int nInsertPos, int nReportType )
{
	for ( int nPos = 0; nPos < DataManagerNonDb.SalesHistoryCustomer.GetSize(); nPos++ )
	{
		int nArrayReportType = DataManagerNonDb.SalesHistoryCustomer.GetReportType( nPos );
		if ( nArrayReportType == nReportType )
		{
			SelectLine( nPos );
			return;
		}
	}

	CSalesHistoryCSVRecordCustomer NewRecord;
	NewRecord.SetName( "" );
	NewRecord.SetReportType( nReportType );
	DataManagerNonDb.SalesHistoryCustomer.InsertAt ( nInsertPos, NewRecord );
	m_listData.SetItemCountEx ( GetArraySize() );
	SelectLine ( nInsertPos );

	m_WarnCancelAfterChange.SetFlag();
}

/**********************************************************************/

void CListDataSalesHistoryCustomerDlg::HandleButton1() 
{
	int nInsertPos = GetSafeSelection() + 1;
	if ( nInsertPos == 0 ) nInsertPos = DataManagerNonDb.SalesHistoryCustomer.GetSize();

	CSalesHistoryCSVRecordAddCustomerDlg dlg ( this );
	
	if ( dlg.DoModal() == IDOK )
	{
		switch ( dlg.m_nReportType )
		{
		case SH_CUST_SUMMARY:
		case SH_CUST_DETAIL:
			AddOrFindReport( nInsertPos, dlg.m_nReportType );
			break;

		case 999:
			{
				for ( int nPos = 0; nPos < DataManagerNonDb.SalesHistoryCustomer.GetSize(); nPos++ )
				{
					if ( DataManagerNonDb.SalesHistoryCustomer.GetReportType( nPos ) == 0 )
						continue;

					DataManagerNonDb.SalesHistoryCustomer.RemoveAt ( nPos );
					m_listData.SetItemCountEx ( GetArraySize() );
					nPos--;
				}

				AddOrFindReport( 0, SH_CUST_SUMMARY );
				AddOrFindReport( 1, SH_CUST_DETAIL );
				
				m_WarnCancelAfterChange.SetFlag();
			}
			break;

		case SH_CUSTOM:
		default:
			{
				CSalesHistoryCSVRecordCustomer NewRecord;
				NewRecord.SetName( "Custom Report" );
				DataManagerNonDb.SalesHistoryCustomer.InsertAt ( nInsertPos, NewRecord );
				m_listData.SetItemCountEx ( GetArraySize() );
				SelectLine ( nInsertPos );
				HandleButton2();

				m_WarnCancelAfterChange.SetFlag();
			}
			break;
		}
	}
}

/**********************************************************************/

void CListDataSalesHistoryCustomerDlg::HandleButton2() 
{
	int nSelection = GetSafeSelection();
	if ( nSelection == -1 )
		return;
	
	CSalesHistoryCSVRecordCustomer ReportRecord;
	DataManagerNonDb.SalesHistoryCustomer.GetAt ( nSelection, ReportRecord );
	
	if ( ReportRecord.GetReportType() != 0 )
	{
		Prompter.Error ( "You cannot edit the standard reports" );
		return;
	}
	
	CSalesHistoryCSVRecordEditCustomerDlg dlg ( ReportRecord, FALSE, this );
	
	if ( dlg.DoModal() == IDOK )
	{
		DataManagerNonDb.SalesHistoryCustomer.SetAt ( nSelection, ReportRecord );
		m_WarnCancelAfterChange.SetFlag();
	}
	
	SelectLine ( nSelection );
}
		
/**********************************************************************/
	
void CListDataSalesHistoryCustomerDlg::HandleButton3() 
{
	m_pDisplayHelpers -> HandleDelete( this );
	m_WarnCancelAfterChange.SetFlag( m_pContentHelpers -> GetDeletionEffectFlag() );
}

/**********************************************************************/

void CListDataSalesHistoryCustomerDlg::HandleButton4()  
{
	int nSelection = GetSafeSelection ( 1, GetArraySize() - 1 );
	if ( nSelection == -1 )
		return;
	
	CSalesHistoryCSVRecordCustomer TempRecord1; 
	CSalesHistoryCSVRecordCustomer TempRecord2; 
	DataManagerNonDb.SalesHistoryCustomer.GetAt ( nSelection, TempRecord1 );
	DataManagerNonDb.SalesHistoryCustomer.GetAt ( nSelection - 1, TempRecord2 );
	DataManagerNonDb.SalesHistoryCustomer.SetAt ( nSelection, TempRecord2 );
	DataManagerNonDb.SalesHistoryCustomer.SetAt ( nSelection - 1, TempRecord1 );
	SelectLine ( nSelection - 1 );

	m_WarnCancelAfterChange.SetFlag();
}

/**********************************************************************/

void CListDataSalesHistoryCustomerDlg::HandleButton5() 
{
	int nSelection = GetSafeSelection ( 0, GetArraySize() - 2 );
	if ( nSelection == -1 ) 
		return;

	CSalesHistoryCSVRecordCustomer TempRecord1; 
	CSalesHistoryCSVRecordCustomer TempRecord2; 
	DataManagerNonDb.SalesHistoryCustomer.GetAt ( nSelection, TempRecord1 );
	DataManagerNonDb.SalesHistoryCustomer.GetAt ( nSelection + 1, TempRecord2 );
	DataManagerNonDb.SalesHistoryCustomer.SetAt ( nSelection, TempRecord2 );
	DataManagerNonDb.SalesHistoryCustomer.SetAt ( nSelection + 1, TempRecord1 );
	SelectLine ( nSelection + 1 );

	m_WarnCancelAfterChange.SetFlag();
}

/**********************************************************************/

void CListDataSalesHistoryCustomerDlg::HandleButton6()
{
	int nSelection = GetSafeSelection ( 0, GetArraySize() - 1 );
	if ( nSelection == -1 ) 
		return;

	CSalesHistoryCSVRecordCustomer TempRecord1, TempRecord2; 
	DataManagerNonDb.SalesHistoryCustomer.GetAt ( nSelection, TempRecord1 );

	if ( TempRecord1.CreateCopy( TempRecord2 ) == FALSE )
		Prompter.Error ( "The selected report cannot be copied." );
	else
	{
		DataManagerNonDb.SalesHistoryCustomer.Add( TempRecord2 );
		m_listData.SetItemCountEx ( GetArraySize() );
		SelectLine( GetArraySize() - 1 );

		m_WarnCancelAfterChange.SetFlag();
	}
}

/**********************************************************************/
