/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "EntityIDCreator.h"
#include "SalesHistoryCSVRecordAddPluDlg.h"
#include "SalesHistoryCSVRecordEditPluDlg.h"
/**********************************************************************/
#include "ListDataSalesHistoryPluDlg.h"
/**********************************************************************/

CSingleListContentHelpersSalesHistoryPlu::CSingleListContentHelpersSalesHistoryPlu()
{
}

/**********************************************************************/

int CSingleListContentHelpersSalesHistoryPlu::GetArraySize()
{
	return DataManagerNonDb.SalesHistoryPlu.GetSize();
}

/**********************************************************************/

bool CSingleListContentHelpersSalesHistoryPlu::PrepareLine( int nIndex, CString& strLine )
{
	strLine = "";

	bool bResult = FALSE;
	if ( ( nIndex >= 0 ) && ( nIndex < GetArraySize() ) )
	{
		CSalesHistoryCSVRecordPlu ReportRecord;
		DataManagerNonDb.SalesHistoryPlu.GetAt( nIndex, ReportRecord );

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

void CSingleListContentHelpersSalesHistoryPlu::DeleteDatabaseEntry( int nIndex )
{
	if ( ( nIndex >= 0 ) && ( nIndex < DataManagerNonDb.SalesHistoryPlu.GetSize() ) )
	{
		DataManagerNonDb.SalesHistoryPlu.RemoveAt ( nIndex );
		m_bDeletionEffect = TRUE;
	}
}

/**********************************************************************/

void CSingleListContentHelpersSalesHistoryPlu::SetColumns( CSSListCtrlVirtual& list, CWnd* pParent )
{
	list.SubclassDlgItem ( IDC_LIST, pParent );
	list.InsertColumn ( 0, "Type", LVCFMT_LEFT, 100 );
	list.InsertColumn ( 1, "Report Name", LVCFMT_LEFT, 380 );
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CListDataSalesHistoryPluDlg::CListDataSalesHistoryPluDlg( int nInitialSel, CWnd* pParent) : CListDataDlg ( pParent )
{
	m_nInitialSel = nInitialSel;
	
	m_pContentHelpers = new CSingleListContentHelpersSalesHistoryPlu();
	m_pDisplayHelpers = new CSingleListDisplayHelpers( m_listData, *m_pContentHelpers );

	m_pDisplayHelpers -> SetTopSingleText( "report" );
	m_pDisplayHelpers -> SetTopPluralText( "reports" );
	m_pDisplayHelpers -> SetBottomSingleText( "report" );
	m_pDisplayHelpers -> SetBottomPluralText( "reports" );
}

/**********************************************************************/

CListDataSalesHistoryPluDlg::~CListDataSalesHistoryPluDlg()
{
	delete m_pDisplayHelpers;
	delete m_pContentHelpers;
}

/**********************************************************************/

void CListDataSalesHistoryPluDlg::SetTitle()
{
	SetWindowText ( "Setup Plu Sales History Reports" );
}

/**********************************************************************/

void CListDataSalesHistoryPluDlg::SetButtonTexts()
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

void CListDataSalesHistoryPluDlg::SetColumns()
{
	m_pContentHelpers -> SetColumns( m_listData, this );
}

/**********************************************************************/

void CListDataSalesHistoryPluDlg::PrepareList()
{
	CListDataDlg::PrepareList();

	if ( m_nInitialSel >= 100 )
		SelectLine( m_nInitialSel - 100 );
	else if ( m_nInitialSel != 0 )
	{
		for ( int nIndex = 0; nIndex < DataManagerNonDb.SalesHistoryPlu.GetSize(); nIndex++ )
		{
			if ( DataManagerNonDb.SalesHistoryPlu.GetReportType( nIndex ) == m_nInitialSel )
			{
				SelectLine( nIndex );
				break;
			}
		}
	}
}

/**********************************************************************/

void CListDataSalesHistoryPluDlg::AddOrFindReport( int nInsertPos, int nReportType )
{
	for ( int nPos = 0; nPos < DataManagerNonDb.SalesHistoryPlu.GetSize(); nPos++ )
	{
		int nArrayReportType = DataManagerNonDb.SalesHistoryPlu.GetReportType( nPos );
		if ( nArrayReportType == nReportType )
		{
			SelectLine( nPos );
			return;
		}
	}

	CSalesHistoryCSVRecordPlu NewRecord;
	NewRecord.SetName( "" );
	NewRecord.SetReportType( nReportType );
	DataManagerNonDb.SalesHistoryPlu.InsertAt ( nInsertPos, NewRecord );
	m_listData.SetItemCountEx ( GetArraySize() );
	SelectLine ( nInsertPos );

	m_WarnCancelAfterChange.SetFlag();
}

/**********************************************************************/

void CListDataSalesHistoryPluDlg::HandleButton1() 
{
	int nInsertPos = GetSafeSelection() + 1;
	if ( nInsertPos == 0 ) nInsertPos = DataManagerNonDb.SalesHistoryPlu.GetSize();

	CSalesHistoryCSVRecordAddPluDlg dlg ( this );
	
	if ( dlg.DoModal() == IDOK )
	{
		switch ( dlg.m_nReportType )
		{
		case SH_PLU_ITEM:
		case SH_PLU_UNSOLD:
		case SH_PLU_TOP100:
		case SH_PLU_DEPT:
		case SH_PLU_DEPT_BYWDAY:
		case SH_PLU_LOC_BYWDAY:
		case SH_PLU_MONTHLY:
		case SH_PLU_WEEKLY:
		case SH_PLU_DAILY:
		case SH_PLU_DAILY_BYWEEK:
		case SH_PLU_TAX:
			AddOrFindReport( nInsertPos, dlg.m_nReportType );
			break;

		case 999:
			{
				for ( int nPos = 0; nPos < DataManagerNonDb.SalesHistoryPlu.GetSize(); nPos++ )
				{
					if ( DataManagerNonDb.SalesHistoryPlu.GetReportType( nPos ) == 0 )
						continue;

					DataManagerNonDb.SalesHistoryPlu.RemoveAt ( nPos );
					m_listData.SetItemCountEx ( GetArraySize() );
					nPos--;
				}

				AddOrFindReport( 0, SH_PLU_ITEM );
				AddOrFindReport( 1, SH_PLU_UNSOLD );
				AddOrFindReport( 2, SH_PLU_TOP100 );
				AddOrFindReport( 3, SH_PLU_DEPT );
				AddOrFindReport( 4, SH_PLU_DEPT_BYWDAY );
				AddOrFindReport( 5, SH_PLU_LOC_BYWDAY );
				AddOrFindReport( 6, SH_PLU_MONTHLY );
				AddOrFindReport( 7, SH_PLU_WEEKLY );
				AddOrFindReport( 8, SH_PLU_DAILY );
				AddOrFindReport( 9, SH_PLU_DAILY_BYWEEK );
				AddOrFindReport( 10, SH_PLU_TAX );

				m_WarnCancelAfterChange.SetFlag();
			}
			break;

		case SH_CUSTOM:
		default:
			{
				CSalesHistoryCSVRecordPlu NewRecord;
				NewRecord.SetName( "Custom Report" );
			
				CString strID = "";
				CEntityIDCreator<CSalesHistoryCSVArrayPlu> EntityIDCreator;
				if ( EntityIDCreator.CreateEntityID( DataManagerNonDb.SalesHistoryPlu, strID ) == TRUE )
				{
					NewRecord.SetEntityID( strID );
					DataManagerNonDb.SalesHistoryPlu.InsertAt ( nInsertPos, NewRecord );
					m_listData.SetItemCountEx ( GetArraySize() );
					SelectLine ( nInsertPos );
					HandleButton2();

					m_WarnCancelAfterChange.SetFlag();
				}
			}
			break;
		}
	}	
}

/**********************************************************************/

void CListDataSalesHistoryPluDlg::HandleButton2() 
{
	int nSelection = GetSafeSelection();
	if ( nSelection == -1 )
		return;
	
	CSalesHistoryCSVRecordPlu ReportRecord;
	DataManagerNonDb.SalesHistoryPlu.GetAt ( nSelection, ReportRecord );
	
	if ( ReportRecord.GetReportType() != 0 )
	{
		Prompter.Error ( "You cannot edit the standard reports" );
		return;
	}
	
	CSalesHistoryCSVRecordEditPluDlg dlg ( ReportRecord, FALSE, this );
	
	if ( dlg.DoModal() == IDOK )
	{
		DataManagerNonDb.SalesHistoryPlu.SetAt ( nSelection, ReportRecord );
		m_WarnCancelAfterChange.SetFlag();
	}

	SelectLine ( nSelection );
}
		
/**********************************************************************/

void CListDataSalesHistoryPluDlg::HandleButton3() 
{
	m_pDisplayHelpers -> HandleDelete( this );
	m_WarnCancelAfterChange.SetFlag( m_pContentHelpers -> GetDeletionEffectFlag() );
}

/**********************************************************************/

void CListDataSalesHistoryPluDlg::HandleButton4()  
{
	int nSelection = GetSafeSelection ( 1, GetArraySize() - 1 );
	if ( nSelection == -1 )
		return;
	
	CSalesHistoryCSVRecordPlu TempRecord1; 
	CSalesHistoryCSVRecordPlu TempRecord2; 
	DataManagerNonDb.SalesHistoryPlu.GetAt ( nSelection, TempRecord1 );
	DataManagerNonDb.SalesHistoryPlu.GetAt ( nSelection - 1, TempRecord2 );
	DataManagerNonDb.SalesHistoryPlu.SetAt ( nSelection, TempRecord2 );
	DataManagerNonDb.SalesHistoryPlu.SetAt ( nSelection - 1, TempRecord1 );
	SelectLine ( nSelection - 1 );

	m_WarnCancelAfterChange.SetFlag();
}

/**********************************************************************/

void CListDataSalesHistoryPluDlg::HandleButton5() 
{
	int nSelection = GetSafeSelection ( 0, GetArraySize() - 2 );
	if ( nSelection == -1 ) 
		return;

	CSalesHistoryCSVRecordPlu TempRecord1; 
	CSalesHistoryCSVRecordPlu TempRecord2; 
	DataManagerNonDb.SalesHistoryPlu.GetAt ( nSelection, TempRecord1 );
	DataManagerNonDb.SalesHistoryPlu.GetAt ( nSelection + 1, TempRecord2 );
	DataManagerNonDb.SalesHistoryPlu.SetAt ( nSelection, TempRecord2 );
	DataManagerNonDb.SalesHistoryPlu.SetAt ( nSelection + 1, TempRecord1 );
	SelectLine ( nSelection + 1 );

	m_WarnCancelAfterChange.SetFlag();
}

/**********************************************************************/

void CListDataSalesHistoryPluDlg::HandleButton6()
{
	int nSelection = GetSafeSelection ( 0, GetArraySize() - 1 );
	if ( nSelection == -1 ) 
		return;

	CSalesHistoryCSVRecordPlu TempRecord1, TempRecord2; 
	DataManagerNonDb.SalesHistoryPlu.GetAt ( nSelection, TempRecord1 );

	if ( TempRecord1.CreateCopy( TempRecord2 ) == FALSE )
		Prompter.Error ( "The selected report cannot be copied." );
	else
	{
		CString strID = "";
		CEntityIDCreator<CSalesHistoryCSVArrayPlu> EntityIDCreator;
		if ( EntityIDCreator.CreateEntityID( DataManagerNonDb.SalesHistoryPlu, strID ) == TRUE )
		{
			TempRecord2.SetEntityID( strID );
			DataManagerNonDb.SalesHistoryPlu.Add( TempRecord2 );
			m_listData.SetItemCountEx ( GetArraySize() );
			SelectLine( GetArraySize() - 1 );

			m_WarnCancelAfterChange.SetFlag();
		}
	}
}

/**********************************************************************/
