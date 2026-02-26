/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "SalesHistoryCSVRecordAddStockDlg.h"
#include "SalesHistoryCSVRecordEditStockDlg.h"
/**********************************************************************/
#include "ListDataSalesHistoryStockDlg.h"
/**********************************************************************/

CSingleListContentHelpersSalesHistoryStock::CSingleListContentHelpersSalesHistoryStock()
{
}

/**********************************************************************/

int CSingleListContentHelpersSalesHistoryStock::GetArraySize()
{
	return DataManagerNonDb.SalesHistoryStock.GetSize();
}

/**********************************************************************/

bool CSingleListContentHelpersSalesHistoryStock::PrepareLine( int nIndex, CString& strLine )
{
	strLine = "";

	bool bResult = FALSE;
	if ( ( nIndex >= 0 ) && ( nIndex < GetArraySize() ) )
	{
		CSalesHistoryCSVRecordStock ReportRecord;
		DataManagerNonDb.SalesHistoryStock.GetAt( nIndex, ReportRecord );

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

void CSingleListContentHelpersSalesHistoryStock::DeleteDatabaseEntry( int nIndex )
{
	if ( ( nIndex >= 0 ) && ( nIndex < DataManagerNonDb.SalesHistoryStock.GetSize() ) )
	{
		DataManagerNonDb.SalesHistoryStock.RemoveAt ( nIndex );
		m_bDeletionEffect = TRUE;
	}
}

/**********************************************************************/

void CSingleListContentHelpersSalesHistoryStock::SetColumns( CSSListCtrlVirtual& list, CWnd* pParent )
{
	list.SubclassDlgItem ( IDC_LIST, pParent );
	list.InsertColumn ( 0, "Type", LVCFMT_LEFT, 100 );
	list.InsertColumn ( 1, "Report Name", LVCFMT_LEFT, 380 );
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CListDataSalesHistoryStockDlg::CListDataSalesHistoryStockDlg( int nInitialSel, CWnd* pParent) : CListDataDlg ( pParent )
{
	m_nInitialSel = nInitialSel;
	
	m_pContentHelpers = new CSingleListContentHelpersSalesHistoryStock();
	m_pDisplayHelpers = new CSingleListDisplayHelpers( m_listData, *m_pContentHelpers );

	m_pDisplayHelpers -> SetTopSingleText( "report" );
	m_pDisplayHelpers -> SetTopPluralText( "reports" );
	m_pDisplayHelpers -> SetBottomSingleText( "report" );
	m_pDisplayHelpers -> SetBottomPluralText( "reports" );
}

/**********************************************************************/

CListDataSalesHistoryStockDlg::~CListDataSalesHistoryStockDlg()
{
	delete m_pDisplayHelpers;
	delete m_pContentHelpers;
}

/**********************************************************************/

void CListDataSalesHistoryStockDlg::SetTitle()
{
	SetWindowText ( "Setup Stock Sales History Reports" );
}

/**********************************************************************/

void CListDataSalesHistoryStockDlg::SetButtonTexts()
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

void CListDataSalesHistoryStockDlg::SetColumns()
{
	m_pContentHelpers -> SetColumns( m_listData, this );
}

/**********************************************************************/

void CListDataSalesHistoryStockDlg::PrepareList()
{
	CListDataDlg::PrepareList();

	if ( m_nInitialSel >= 100 )
		SelectLine( m_nInitialSel - 100 );
	else if ( m_nInitialSel != 0 )
	{
		for ( int nIndex = 0; nIndex < DataManagerNonDb.SalesHistoryStock.GetSize(); nIndex++ )
		{
			if ( DataManagerNonDb.SalesHistoryStock.GetReportType( nIndex ) == m_nInitialSel )
			{
				SelectLine( nIndex );
				break;
			}
		}
	}
}

/**********************************************************************/

void CListDataSalesHistoryStockDlg::AddOrFindReport( int nInsertPos, int nReportType )
{
	for ( int nPos = 0; nPos < DataManagerNonDb.SalesHistoryStock.GetSize(); nPos++ )
	{
		int nArrayReportType = DataManagerNonDb.SalesHistoryStock.GetReportType( nPos );
		if ( nArrayReportType == nReportType )
		{
			SelectLine( nPos );
			return;
		}
	}

	CSalesHistoryCSVRecordStock NewRecord;
	NewRecord.SetName( "" );
	NewRecord.SetReportType( nReportType );
	DataManagerNonDb.SalesHistoryStock.InsertAt ( nInsertPos, NewRecord );
	m_listData.SetItemCountEx ( GetArraySize() );
	SelectLine ( nInsertPos );

	m_WarnCancelAfterChange.SetFlag();
}

/**********************************************************************/

void CListDataSalesHistoryStockDlg::HandleButton1() 
{
	int nInsertPos = GetSafeSelection() + 1;
	if ( nInsertPos == 0 ) nInsertPos = DataManagerNonDb.SalesHistoryStock.GetSize();

	CSalesHistoryCSVRecordAddStockDlg dlg ( this );
	
	if ( dlg.DoModal() == IDOK )
	{
		switch ( dlg.m_nReportType )
		{
		case SH_STK_ITEM:
		case SH_STK_UNSOLD:
		case SH_STK_TOP100:
		case SH_STK_CAT:
		case SH_STK_CAT_BYWDAY:
		case SH_STK_SP_BYWDAY:
		case SH_STK_MONTHLY:
		case SH_STK_WEEKLY:
		case SH_STK_DAILY:
		case SH_STK_DAILY_BYWEEK:
			AddOrFindReport( nInsertPos, dlg.m_nReportType );
			break;

		case 999:
			{
				for ( int nPos = 0; nPos < DataManagerNonDb.SalesHistoryStock.GetSize(); nPos++ )
				{
					if ( DataManagerNonDb.SalesHistoryStock.GetReportType( nPos ) == 0 )
						continue;

					DataManagerNonDb.SalesHistoryStock.RemoveAt ( nPos );
					m_listData.SetItemCountEx ( GetArraySize() );
					nPos--;
				}

				AddOrFindReport( 0, SH_STK_ITEM );
				AddOrFindReport( 1, SH_STK_UNSOLD );
				AddOrFindReport( 2, SH_STK_TOP100 );
				AddOrFindReport( 3, SH_STK_CAT );
				AddOrFindReport( 4, SH_STK_CAT_BYWDAY );
				AddOrFindReport( 5, SH_STK_SP_BYWDAY );
				AddOrFindReport( 6, SH_STK_MONTHLY );
				AddOrFindReport( 7, SH_STK_WEEKLY );
				AddOrFindReport( 8, SH_STK_DAILY );
				AddOrFindReport( 9, SH_STK_DAILY_BYWEEK );

				m_WarnCancelAfterChange.SetFlag();
			}
			break;

		case SH_CUSTOM:
		default:
			{
				CSalesHistoryCSVRecordStock NewRecord;
				NewRecord.SetName( "Custom Report" );
				DataManagerNonDb.SalesHistoryStock.InsertAt ( nInsertPos, NewRecord );
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

void CListDataSalesHistoryStockDlg::HandleButton2() 
{
	int nSelection = GetSafeSelection();
	if ( nSelection == -1 )
		return;
	
	CSalesHistoryCSVRecordStock ReportRecord;
	DataManagerNonDb.SalesHistoryStock.GetAt ( nSelection, ReportRecord );
	
	if ( ReportRecord.GetReportType() != 0 )
	{
		Prompter.Error ( "You cannot edit the standard reports" );
		return;
	}
	
	CSalesHistoryCSVRecordEditStockDlg dlg ( ReportRecord, FALSE, this );
	
	if ( dlg.DoModal() == IDOK )
	{
		DataManagerNonDb.SalesHistoryStock.SetAt ( nSelection, ReportRecord );
		m_WarnCancelAfterChange.SetFlag();
	}
	
	SelectLine ( nSelection );
}
		
/**********************************************************************/

void CListDataSalesHistoryStockDlg::HandleButton3() 
{
	m_pDisplayHelpers -> HandleDelete( this );
	m_WarnCancelAfterChange.SetFlag( m_pContentHelpers -> GetDeletionEffectFlag() );
}

/**********************************************************************/

void CListDataSalesHistoryStockDlg::HandleButton4()  
{
	int nSelection = GetSafeSelection ( 1, GetArraySize() - 1 );
	if ( nSelection == -1 )
		return;
	
	CSalesHistoryCSVRecordStock TempRecord1; 
	CSalesHistoryCSVRecordStock TempRecord2; 
	DataManagerNonDb.SalesHistoryStock.GetAt ( nSelection, TempRecord1 );
	DataManagerNonDb.SalesHistoryStock.GetAt ( nSelection - 1, TempRecord2 );
	DataManagerNonDb.SalesHistoryStock.SetAt ( nSelection, TempRecord2 );
	DataManagerNonDb.SalesHistoryStock.SetAt ( nSelection - 1, TempRecord1 );
	SelectLine ( nSelection - 1 );

	m_WarnCancelAfterChange.SetFlag();
}

/**********************************************************************/

void CListDataSalesHistoryStockDlg::HandleButton5() 
{
	int nSelection = GetSafeSelection ( 0, GetArraySize() - 2 );
	if ( nSelection == -1 ) 
		return;

	CSalesHistoryCSVRecordStock TempRecord1; 
	CSalesHistoryCSVRecordStock TempRecord2; 
	DataManagerNonDb.SalesHistoryStock.GetAt ( nSelection, TempRecord1 );
	DataManagerNonDb.SalesHistoryStock.GetAt ( nSelection + 1, TempRecord2 );
	DataManagerNonDb.SalesHistoryStock.SetAt ( nSelection, TempRecord2 );
	DataManagerNonDb.SalesHistoryStock.SetAt ( nSelection + 1, TempRecord1 );
	SelectLine ( nSelection + 1 );

	m_WarnCancelAfterChange.SetFlag();
}

/**********************************************************************/

void CListDataSalesHistoryStockDlg::HandleButton6()
{
	int nSelection = GetSafeSelection ( 0, GetArraySize() - 1 );
	if ( nSelection == -1 ) 
		return;

	CSalesHistoryCSVRecordStock TempRecord1, TempRecord2; 
	DataManagerNonDb.SalesHistoryStock.GetAt ( nSelection, TempRecord1 );

	if ( TempRecord1.CreateCopy( TempRecord2 ) == FALSE )
		Prompter.Error ( "The selected report cannot be copied." );
	else
	{
		DataManagerNonDb.SalesHistoryStock.Add( TempRecord2 );
		m_listData.SetItemCountEx ( GetArraySize() );
		SelectLine( GetArraySize() - 1 );

		m_WarnCancelAfterChange.SetFlag();
	}
}

/**********************************************************************/