/**********************************************************************/
#include "FilterTypes.h"
#include "StatusProgress.h"
#include "StockDeleteRangeSetDlg.h"
/**********************************************************************/
#include "StockListItemSetDlg.h"
/**********************************************************************/

void CStockListItemSetDlg::SetColumns()
{
	m_ColumnManager.Reset( TableNames::StockFilter );
	m_ColumnManager.AddColumn ( ColumnNames::StockCode, "Stock Code", LVCFMT_LEFT, 115, 115 );
	m_ColumnManager.AddColumn ( ColumnNames::Desc, "Description", LVCFMT_LEFT, 135, 170 );
	m_ColumnManager.AddColumn ( ColumnNames::Category, "Category", LVCFMT_LEFT, 80, 140 );
	m_ColumnManager.AddColumn ( ColumnNames::StockUnit, "Unit", LVCFMT_LEFT, 100, 100 );
	m_ColumnManager.AddColumn ( ColumnNames::StockFilter, "Include", LVCFMT_RIGHT, 75, 75 );
	m_ColumnManager.LoadSettings();
}

/**********************************************************************/

void CStockListItemSetDlg::EditStockLevel()
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		int nSelection = m_listStock.GetCurSel();

		if ( ( nSelection >= 0 ) && ( nSelection < m_pFilterArray -> GetSize() ) )
		{		
			m_pFilterArray -> SetArrayIdx ( nSelection );

			int nStockIdx = m_pFilterArray ->GetStockItemIndexAt( nSelection );
			
			CStockCSVRecord StockRecord;
			DataManager.Stock.GetAt( nStockIdx, StockRecord );
			
			CStockLevelsCSVRecord StockLevels;
			DataManager.Stock.GetStockpointTotals( nStockIdx, StockRecord, m_pFilterArray ->GetSpNo(), StockLevels);
			StockLevels.SetActiveFlag( StockLevels.GetActiveFlag() == FALSE );			
			DataManager.Stock.SetStockpointTotals( nStockIdx, StockRecord, m_pFilterArray ->GetSpNo(), StockLevels);
			
			DisplayScreen ( m_pFilterArray -> GetArrayIdx() );
		}
	}
}

/**********************************************************************/

void CStockListItemSetDlg::DisplayScreen ( int nIndex )
{
	int nSize = m_pFilterArray -> GetSize();
	
	CString strFiltered = "";
	if ( m_pFilterArray -> GetFilteredFlag() == TRUE )
		strFiltered = ", Filtered";
	
	CString strText;
	strText.Format ( "%s (%s%d of %d records%s)",
		GetTitle(),
		GetSpName(),
		nSize,
		DataManager.Stock.GetSize(),
		(const char*) strFiltered );
	
	SetWindowText ( strText );

	if ( ( nIndex >= 0 ) && ( nIndex < nSize ) )
		m_listStock.SetCurSel ( nIndex );
	else
		m_listStock.SetCurSel ( nSize - 1 );

	m_listStock.Invalidate();
}

/**********************************************************************/

void CStockListItemSetDlg::SetFilterArray()
{
	m_pFilterArray = new CStockFilterArray ( FILTERTYPE_STOCK_ITEMSET );
	m_pFilterArray -> SetSpIdx ( m_nSpIdx, 0 );
	m_pFilterArray -> SetStocktakeFilter ( -1 );
	m_pFilterArray -> BuildRecordList();
}

/**********************************************************************/

void CStockListItemSetDlg::PrepareMenu()
{
	m_MenuChanger.KillItem ( IDM_IMPORT_CREATESHEET );
	m_MenuChanger.KillItem ( IDM_EDIT_STOCKLEVEL2 );
	m_MenuChanger.KillItem ( IDM_COPY_LEVELS );
	m_MenuChanger.KillItem ( IDM_COPY_PLU );
	m_MenuChanger.KillItem ( IDM_REMOVE_ZEROS );
	m_MenuChanger.KillItem ( IDM_COPY_SUPPLIER );
	m_MenuChanger.KillParent ( IDM_EDIT_STOCKLEVEL );
	m_MenuChanger.KillParent ( IDM_IMPORT_STOCKLEVEL );
	m_MenuChanger.KillParent ( IDM_PRINT_SESSION );
}

/**********************************************************************/

void CStockListItemSetDlg::HandleRangeActive()
{
	int nSelection = m_listStock.GetCurSel();

	CStockDeleteRangeSetDlg dlg ( this );

	if ( nSelection >= 0 )
	{
		int nStockIdx = m_pFilterArray -> GetStockItemIndexAt ( nSelection );
		CString strStockCode = DataManager.Stock.GetStockCode ( nStockIdx );
		dlg.m_strStockCodeFrom = strStockCode; 
		dlg.m_strStockCodeTo = strStockCode;
	}
	else
	{
		dlg.m_strStockCodeFrom = "";
		dlg.m_strStockCodeTo = "";
	}
		
	dlg.m_nCategoryFilter = 0;

	if ( dlg.DoModal() == IDOK )
	{
		int nStartIdx, nRange;
		m_pFilterArray -> GetArrayIdxRange( dlg.m_strStockCodeFrom, dlg.m_strStockCodeTo, nStartIdx, nRange );
		
		CCategorySetCSVRecord CategorySet;
		CategorySet.CreateCategorySet ( dlg.m_nCategoryFilter );

		StatusProgress.Lock( TRUE, "Updating Stockpoint Item Set" );

		int nCount = 0;
		for ( int nOffset = 0; nOffset < nRange; nOffset++ )
		{
			StatusProgress.SetPos ( nOffset, nRange ); 

			CStockCSVRecord StockRecord;
			int nStockIdx = m_pFilterArray -> GetStockItemIndexAt ( nStartIdx + nOffset );
			
			DataManager.Stock.GetAt ( nStockIdx, StockRecord );
			
			CString strStockCode = StockRecord.GetStockCode();
			if ( ( strStockCode < dlg.m_strStockCodeFrom ) || ( strStockCode > dlg.m_strStockCodeTo ) )
				continue;
	
			if ( CategorySet.GetCategoryStatus ( StockRecord.GetCategory() ) == FALSE )
				continue;

			CStockLevelsCSVRecord StockLevels;
			DataManager.Stock.GetStockpointTotals( nStockIdx, StockRecord, m_pFilterArray ->GetSpNo(), StockLevels);
			StockLevels.SetActiveFlag( 0 == dlg.m_nDelete );			
			DataManager.Stock.SetStockpointTotals( nStockIdx, StockRecord, m_pFilterArray ->GetSpNo(), StockLevels);

			nCount++;
		}
		StatusProgress.Unlock();
		
		DisplayScreen( nSelection );
		
		CString strBuf;

		switch ( nCount )
		{
		case 0:
			Prompter.Info ( "No records were matched" );
			break;

		case 1:
			Prompter.Info ( "1 record was matched" );
			break;

		default:
			strBuf.Format ( "%d records were matched", nCount );
			Prompter.Info ( strBuf );
			break;
		}
	}
}

/**********************************************************************/
