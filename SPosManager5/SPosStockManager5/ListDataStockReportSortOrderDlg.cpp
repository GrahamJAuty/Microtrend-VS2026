/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "StockReportSortOrderAddDlg.h"
#include "StockReportSortOrderEditDlg.h"
/**********************************************************************/
#include "ListDataStockReportSortOrderDlg.h"
/**********************************************************************/

CSingleListContentHelpersStockReportSortOrder::CSingleListContentHelpersStockReportSortOrder()
{
}

/**********************************************************************/

int CSingleListContentHelpersStockReportSortOrder::GetArraySize()
{
	return DataManagerNonDb.GetActiveStockReportSortOrder() -> GetSize();
}

/**********************************************************************/

bool CSingleListContentHelpersStockReportSortOrder::PrepareLine( int nIndex, CString& strLine )
{
	strLine = "";

	bool bResult = FALSE;
	if ( ( nIndex >= 0 ) && ( nIndex < GetArraySize() ) )
	{
		CStockReportSortOrderCSVRecord SortOrderRecord;
		DataManagerNonDb.GetActiveStockReportSortOrder() -> GetAt( nIndex, SortOrderRecord );

		CCSV csvOut ( '\t' );

		if ( SortOrderRecord.GetSortOrderType() == SH_CUSTOM )
			csvOut.Add( "Custom" );
		else
			csvOut.Add( "Standard" );
				
		csvOut.Add( SortOrderRecord.GetListName() );
		strLine = csvOut.GetLine();

		bResult = TRUE;
	}
	
	return bResult;
}

/**********************************************************************/

void CSingleListContentHelpersStockReportSortOrder::DeleteDatabaseEntry( int nIndex )
{
	if ( ( nIndex >= 0 ) && ( nIndex < DataManagerNonDb.GetActiveStockReportSortOrder() -> GetSize() ) )
	{
		DataManagerNonDb.GetActiveStockReportSortOrder() -> RemoveAt ( nIndex );
		m_bDeletionEffect = TRUE;
	}
}

/**********************************************************************/

void CSingleListContentHelpersStockReportSortOrder::SetColumns( CSSListCtrlVirtual& list, CWnd* pParent )
{
	list.SubclassDlgItem ( IDC_LIST, pParent );
	list.InsertColumn ( 0, "Type", LVCFMT_LEFT, 80 );
	list.InsertColumn ( 1, "Sort Order Name", LVCFMT_LEFT, 400 );
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CListDataStockReportSortOrderDlg::CListDataStockReportSortOrderDlg( int nInitialSel, CWnd* pParent) : CListDataDlg ( pParent )
{
	m_nInitialSel = nInitialSel;

	m_pContentHelpers = new CSingleListContentHelpersStockReportSortOrder();
	m_pDisplayHelpers = new CSingleListDisplayHelpers( m_listData, *m_pContentHelpers );

	m_pDisplayHelpers -> SetTopSingleText( "sort order" );
	m_pDisplayHelpers -> SetTopPluralText( "sort orders" );
	m_pDisplayHelpers -> SetBottomSingleText( "sort order" );
	m_pDisplayHelpers -> SetBottomPluralText( "sort orders" );
}

/**********************************************************************/

CListDataStockReportSortOrderDlg::~CListDataStockReportSortOrderDlg()
{
	delete m_pDisplayHelpers;
	delete m_pContentHelpers;
}

/**********************************************************************/

void CListDataStockReportSortOrderDlg::SetTitle()
{
	CString strName = "";
	CStockReportSortOrderCSVArray::GetReportName( DataManagerNonDb.GetActiveStockReportSortOrderType(), strName );

	CString strTitle;
	strTitle.Format( "Setup Sort Orders for %s Report", (const char*) strName );
		
	SetWindowText ( strTitle );
}

/**********************************************************************/

void CListDataStockReportSortOrderDlg::SetButtonTexts()
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

void CListDataStockReportSortOrderDlg::SetColumns()
{
	m_pContentHelpers -> SetColumns( m_listData, this );
}

/**********************************************************************/

void CListDataStockReportSortOrderDlg::PrepareList()
{
	CListDataDlg::PrepareList();

	if ( m_nInitialSel >= 500 )
	{
		int nCustomPos = m_nInitialSel - 500;
		for ( int nIndex = 0; nIndex < DataManagerNonDb.GetActiveStockReportSortOrder() -> GetSize(); nIndex++ )
		{
			if ( DataManagerNonDb.GetActiveStockReportSortOrder() -> GetSortOrderType( nIndex ) == SH_CUSTOM )
			{
				if ( (nCustomPos--) <= 0 )
				{
					SelectLine( nIndex );
					break;
				}
			}
		}
	}
	else if ( m_nInitialSel != 0 )
	{
		for ( int nIndex = 0; nIndex < DataManagerNonDb.GetActiveStockReportSortOrder() -> GetSize(); nIndex++ )
		{
			if ( DataManagerNonDb.GetActiveStockReportSortOrder() -> GetSortOrderType( nIndex ) == m_nInitialSel )
			{
				SelectLine( nIndex );
				break;
			}
		}
	}
}

/**********************************************************************/

void CListDataStockReportSortOrderDlg::AddOrFindSortOrder( int nInsertPos, int nSortOrderType )
{
	for ( int nPos = 0; nPos < DataManagerNonDb.GetActiveStockReportSortOrder() -> GetSize(); nPos++ )
	{
		int nArraySortOrderType = DataManagerNonDb.GetActiveStockReportSortOrder() -> GetSortOrderType( nPos );
		if ( nArraySortOrderType == nSortOrderType )
		{
			SelectLine( nPos );
			return;
		}
	}

	CStockReportSortOrderCSVRecord NewRecord;
	NewRecord.SetName( "" );
	NewRecord.SetSortOrderType( nSortOrderType );
	DataManagerNonDb.GetActiveStockReportSortOrder() -> InsertAt ( nInsertPos, NewRecord );
	m_listData.SetItemCountEx ( GetArraySize() );
	SelectLine ( nInsertPos );

	m_WarnCancelAfterChange.SetFlag();
}

/**********************************************************************/

void CListDataStockReportSortOrderDlg::HandleButton1() 
{
	int nInsertPos = GetSafeSelection() + 1;
	if ( nInsertPos == 0 ) nInsertPos = DataManagerNonDb.GetActiveStockReportSortOrder() -> GetSize();

	CStockReportSortOrderAddDlg dlg ( this );
	
	if ( dlg.DoModal() == IDOK )
	{
		switch ( dlg.m_nReportType )
		{
		case SH_STKLVL_NONE:		
		case SH_STKLVL_SUPP:		
		case SH_STKLVL_SUPP_SP:		
		case SH_STKLVL_SUPP_SP_CAT:
		case SH_STKLVL_SUPP_CAT:
		case SH_STKLVL_SUPP_CAT_SP:	
		case SH_STKLVL_SP:			
		case SH_STKLVL_SP_SUPP:		
		case SH_STKLVL_SP_SUPP_CAT:	
		case SH_STKLVL_SP_CAT:		
		case SH_STKLVL_SP_CAT_SUPP:	
		case SH_STKLVL_CAT:			
		case SH_STKLVL_CAT_SUPP:	
		case SH_STKLVL_CAT_SUPP_SP:	
		case SH_STKLVL_CAT_SP:		
		case SH_STKLVL_CAT_SP_SUPP:	
			AddOrFindSortOrder( nInsertPos, dlg.m_nReportType );
			break;

		case 999:
			{
				for ( int nPos = 0; nPos < DataManagerNonDb.GetActiveStockReportSortOrder() -> GetSize(); nPos++ )
				{
					if ( DataManagerNonDb.GetActiveStockReportSortOrder() -> GetSortOrderType( nPos ) == 0 )
						continue;

					DataManagerNonDb.GetActiveStockReportSortOrder() -> RemoveAt ( nPos );
					m_listData.SetItemCountEx ( GetArraySize() );
					nPos--;
				}

				switch( DataManagerNonDb.GetActiveStockReportSortOrderType() )
				{
				case STOCKREPORT_SORTORDER_DELIVERY:
				case STOCKREPORT_SORTORDER_RETURNS:
					AddOrFindSortOrder( 0,		SH_STKLVL_NONE );		
					AddOrFindSortOrder( 1,		SH_STKLVL_SP );			
					AddOrFindSortOrder( 2,		SH_STKLVL_SP_CAT );		
					AddOrFindSortOrder( 3,		SH_STKLVL_CAT );			
					AddOrFindSortOrder( 4,		SH_STKLVL_CAT_SP );	
					break;

				case STOCKREPORT_SORTORDER_PRICE_LIST:
				case STOCKREPORT_SORTORDER_TRANSFER_PERIOD:
				case STOCKTAKE_SORTORDER_TRANSFER_CLOSING:
					AddOrFindSortOrder( 0,		SH_STKLVL_NONE );		
					AddOrFindSortOrder( 1,		SH_STKLVL_SUPP );		
					AddOrFindSortOrder( 2,		SH_STKLVL_SUPP_CAT );
					AddOrFindSortOrder( 3,		SH_STKLVL_CAT );			
					AddOrFindSortOrder( 4,		SH_STKLVL_CAT_SUPP );	
					break;

				case STOCKREPORT_SORTORDER_SUPPLIER:
					AddOrFindSortOrder( 0,		SH_STKLVL_SUPP );		
					AddOrFindSortOrder( 1,		SH_STKLVL_SUPP_SP );		
					AddOrFindSortOrder( 2,		SH_STKLVL_SUPP_SP_CAT );
					AddOrFindSortOrder( 3,		SH_STKLVL_SUPP_CAT );
					AddOrFindSortOrder( 4,		SH_STKLVL_SUPP_CAT_SP );
					break;

				case STOCKREPORT_SORTORDER_REORDER:
					AddOrFindSortOrder( 0,		SH_STKLVL_SUPP );		
					AddOrFindSortOrder( 1,		SH_STKLVL_SUPP_SP );		
					AddOrFindSortOrder( 2,		SH_STKLVL_SUPP_CAT );
					AddOrFindSortOrder( 3,		SH_STKLVL_SUPP_CAT_SP );
					break;

				case STOCKREPORT_SORTORDER_SALES:
				case STOCKREPORT_SORTORDER_TOP_100_SALES:
				case STOCKREPORT_SORTORDER_ESTIMATED_PROFIT:
				case STOCKREPORT_SORTORDER_APPARENT_QTY:
				case STOCKREPORT_SORTORDER_APPARENT_QTY_DU:
				case STOCKREPORT_SORTORDER_APPARENT_SUMMARY:
				case STOCKREPORT_SORTORDER_APPARENT_SUMMARY_DU:
				case STOCKREPORT_SORTORDER_TOP_100_APPARENT:
				case STOCKREPORT_SORTORDER_APPARENT_VALUE:
				case STOCKREPORT_SORTORDER_OVERSTOCK_VALUE:
				case STOCKREPORT_SORTORDER_OPENING_VALUE:
				case STOCKREPORT_SORTORDER_ADJUST:
				case STOCKREPORT_SORTORDER_SHEET:
				case STOCKREPORT_SORTORDER_RETAIL_SUMMARY:
				case STOCKREPORT_SORTORDER_COST_SUMMARY:
				case STOCKREPORT_SORTORDER_TRANSFER_VALUE:
				case STOCKTAKE_SORTORDER_CLOSING_VALUE:
				case STOCKTAKE_SORTORDER_VARIANCE:
				case STOCKTAKE_SORTORDER_ESTIMATE_ALLOWANCE:
				case STOCKTAKE_SORTORDER_RECONCILE:
				case STOCKTAKE_SORTORDER_RECONCILE_CATEGORY:
				case STOCKTAKE_SORTORDER_SHEET:
				default:
					AddOrFindSortOrder( 0,		SH_STKLVL_NONE );		
					AddOrFindSortOrder( 1,		SH_STKLVL_SUPP );		
					AddOrFindSortOrder( 2,		SH_STKLVL_SUPP_SP );		
					AddOrFindSortOrder( 3,		SH_STKLVL_SUPP_SP_CAT );
					AddOrFindSortOrder( 4,		SH_STKLVL_SUPP_CAT );
					AddOrFindSortOrder( 5,		SH_STKLVL_SUPP_CAT_SP );	
					AddOrFindSortOrder( 6,		SH_STKLVL_SP );			
					AddOrFindSortOrder( 7,		SH_STKLVL_SP_SUPP );		
					AddOrFindSortOrder( 8,		SH_STKLVL_SP_SUPP_CAT );	
					AddOrFindSortOrder( 9,		SH_STKLVL_SP_CAT );		
					AddOrFindSortOrder( 10,		SH_STKLVL_SP_CAT_SUPP );	
					AddOrFindSortOrder( 11,		SH_STKLVL_CAT );			
					AddOrFindSortOrder( 12,		SH_STKLVL_CAT_SUPP );	
					AddOrFindSortOrder( 13,		SH_STKLVL_CAT_SUPP_SP );	
					AddOrFindSortOrder( 14,		SH_STKLVL_CAT_SP );		
					AddOrFindSortOrder( 15,		SH_STKLVL_CAT_SP_SUPP );
					break;
				}

				m_WarnCancelAfterChange.SetFlag();
			}
			break;

		case SH_CUSTOM:
		default:
			{
				CStockReportSortOrderCSVRecord NewRecord;
				NewRecord.SetName( "Custom Sort Order" );
				DataManagerNonDb.GetActiveStockReportSortOrder() -> InsertAt ( nInsertPos, NewRecord );
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

void CListDataStockReportSortOrderDlg::HandleButton2() 
{
	int nSelection = GetSafeSelection();
	if ( nSelection == -1 )
		return;
	
	CStockReportSortOrderCSVRecord SortOrderRecord;
	DataManagerNonDb.GetActiveStockReportSortOrder() -> GetAt ( nSelection, SortOrderRecord );
	
	if ( SortOrderRecord.GetSortOrderType() != 0 )
	{
		Prompter.Error ( "You cannot edit the standard sort orders" );
		return;
	}
	
	CStockReportSortOrderEditDlg dlg ( SortOrderRecord, FALSE, this );
	
	if ( dlg.DoModal() == IDOK )
	{
		DataManagerNonDb.GetActiveStockReportSortOrder() -> SetAt ( nSelection, SortOrderRecord );
		m_WarnCancelAfterChange.SetFlag();
	}

	SelectLine ( nSelection );
}
		
/**********************************************************************/
	
void CListDataStockReportSortOrderDlg::HandleButton3() 
{
	m_pDisplayHelpers -> HandleDelete( this );
	m_WarnCancelAfterChange.SetFlag( m_pContentHelpers -> GetDeletionEffectFlag() );
}

/**********************************************************************/

void CListDataStockReportSortOrderDlg::HandleButton4()  
{
	int nSelection = GetSafeSelection ( 1, GetArraySize() - 1 );
	if ( nSelection == -1 )
		return;
	
	CStockReportSortOrderCSVRecord TempRecord1; 
	CStockReportSortOrderCSVRecord TempRecord2; 
	DataManagerNonDb.GetActiveStockReportSortOrder() -> GetAt ( nSelection, TempRecord1 );
	DataManagerNonDb.GetActiveStockReportSortOrder() -> GetAt ( nSelection - 1, TempRecord2 );
	DataManagerNonDb.GetActiveStockReportSortOrder() -> SetAt ( nSelection, TempRecord2 );
	DataManagerNonDb.GetActiveStockReportSortOrder() -> SetAt ( nSelection - 1, TempRecord1 );
	SelectLine ( nSelection - 1 );

	m_WarnCancelAfterChange.SetFlag();
}

/**********************************************************************/

void CListDataStockReportSortOrderDlg::HandleButton5() 
{
	int nSelection = GetSafeSelection ( 0, GetArraySize() - 2 );
	if ( nSelection == -1 ) 
		return;

	CStockReportSortOrderCSVRecord TempRecord1; 
	CStockReportSortOrderCSVRecord TempRecord2; 
	DataManagerNonDb.GetActiveStockReportSortOrder() -> GetAt ( nSelection, TempRecord1 );
	DataManagerNonDb.GetActiveStockReportSortOrder() -> GetAt ( nSelection + 1, TempRecord2 );
	DataManagerNonDb.GetActiveStockReportSortOrder() -> SetAt ( nSelection, TempRecord2 );
	DataManagerNonDb.GetActiveStockReportSortOrder() -> SetAt ( nSelection + 1, TempRecord1 );
	SelectLine ( nSelection + 1 );

	m_WarnCancelAfterChange.SetFlag();
}

/**********************************************************************/

void CListDataStockReportSortOrderDlg::HandleButton6()
{
	int nSelection = GetSafeSelection ( 0, GetArraySize() - 1 );
	if ( nSelection == -1 ) 
		return;

	CStockReportSortOrderCSVRecord TempRecord1, TempRecord2; 
	DataManagerNonDb.GetActiveStockReportSortOrder() -> GetAt ( nSelection, TempRecord1 );

	if ( TempRecord1.CreateCopy( TempRecord2 ) == FALSE )
		Prompter.Error ( "The selected sort order cannot be copied." );
	else
	{
		DataManagerNonDb.GetActiveStockReportSortOrder() -> Add( TempRecord2 );
		m_listData.SetItemCountEx ( GetArraySize() );
		SelectLine( GetArraySize() - 1 );

		m_WarnCancelAfterChange.SetFlag();
	}
}

/**********************************************************************/