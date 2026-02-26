/**********************************************************************/
#include "ColumnManager.h"
#include "..\CommonEcrStock\CustomExportManager.h"
#include "DatabaseCSVArray.h"
#include "FilterTypes.h"
#include "MessageLogger.h"
#include "RepSpawn.h"
#include "..\CommonEcrStock\SimpleInfoDlg.h"
#include "StatusProgress.h"
#include "StockEditStockCodeDlg.h"
#include "StockOptionsIni.h"
/**********************************************************************/
#include "StockListStockCodeDlg.h"
/**********************************************************************/
	
CStockListStockCodeDlg::CStockListStockCodeDlg( CWnd* pParent) : CStockListDlg ( pParent )
{
	m_bCheckDuplicates = FALSE;
}; 
	
/**********************************************************************/

void CStockListStockCodeDlg::SetColumns()
{
	m_ColumnManager.Reset( TableNames::ChangeStock );
	m_ColumnManager.AddColumn ( ColumnNames::StockCode, "Stock Code", LVCFMT_LEFT, 115, 115 );
	m_ColumnManager.AddColumn ( ColumnNames::Desc, "Description", LVCFMT_LEFT, 135, 170 );
	m_ColumnManager.AddColumn ( ColumnNames::Category, "Category", LVCFMT_LEFT, 80, 140 );
	m_ColumnManager.AddColumn ( ColumnNames::StockCode2, "New Stock Code", LVCFMT_LEFT, 115, 115 );
	m_ColumnManager.LoadSettings();
}

/**********************************************************************/

void CStockListStockCodeDlg::SetFilterArray()
{
	m_pFilterArray = new CStockFilterArray ( FILTERTYPE_STOCKCODES );
	m_pFilterArray -> CreateNewStockCodeArray();
}

/**********************************************************************/

void CStockListStockCodeDlg::EditStockLevel()
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		int nSelection = m_listStock.GetCurSel();

		if ( nSelection >= 0 && nSelection < m_pFilterArray -> GetSize() )
		{
			m_pFilterArray -> SetArrayIdx ( nSelection );	
			CStockEditStockCodeDlg dlg ( m_listStock, m_pFilterArray, this );
			dlg.DoModal();
			DisplayScreen ( m_pFilterArray -> GetArrayIdx() );
		}
	}
}

/**********************************************************************/

void CStockListStockCodeDlg::HandleCancel()
{
	bool bChanged = FALSE;

	for ( int nStockIdx = 0; nStockIdx < DataManager.Stock.GetSize(); nStockIdx++ )
	{
		CString strNewStockCode = m_pFilterArray -> GetNewStockCode ( nStockIdx );
		
		if ( strNewStockCode != DataManager.Stock.GetStockCode ( nStockIdx ) )
		{
			bChanged = TRUE;
			break;
		}
	}

	if ( bChanged == TRUE )
	{
		int nChoice = Prompter.YesNoCancel ( "Stock database has been modified - save changes" );
		
		switch ( nChoice )
		{
		case IDNO:
			MessageLogger.LogSystem( 6, 5, FALSE );
			break;

		case IDCANCEL:
			return;

		case IDYES:
			{
				if ( m_bCheckDuplicates == TRUE )
					CheckDuplicates();

				if ( m_bCheckDuplicates == TRUE )
					return;

				CStringArray Warning;
				Warning.Add ( "Change Stock Codes" );
				Warning.Add ( "Stock codes will be changed as requested" );
				Warning.Add ( "" );
				Warning.Add ( "The stock code database will be rebuilt" );
				Warning.Add ( "" );
				Warning.Add ( "Stock codes for recipe items will be updated" );
				Warning.Add ( "" );
				Warning.Add ( "Stock codes for redirection will be updated" );
				Warning.Add ( "" );
				Warning.Add ( "Stock codes for outstanding orders will be updated" );
				
				if ( Prompter.Warning( Warning ) == IDYES )
				{
					ChangeStockCodes();
					MessageLogger.LogSystem( 6, 6, FALSE );
				}
				else
					return;
			}
			break;
		}
	}
	else
		MessageLogger.LogSystem( 6, 7, FALSE );

	EndDialog ( IDOK );
}

/**********************************************************************/

void CStockListStockCodeDlg::ChangeStockCodes()
{
	StatusProgress.Lock( TRUE, "Updating stock codes" );
	bool bResult = ChangeStockCodesInternal();
	StatusProgress.Unlock();

	if ( bResult == FALSE )
		Prompter.Error ( "Unable to change stock codes!" );
}

/**********************************************************************/

bool CStockListStockCodeDlg::ChangeStockCodesInternal()
{
	StatusProgress.SetDialogText( "Plu number links" );

	int nSize = DataManager.Plu.GetSize();
	for ( int nPluIdx = 0; nPluIdx < nSize; nPluIdx++ )
	{
		StatusProgress.SetPos ( nPluIdx, nSize );

		CRecipeCSVRecord Recipe;
		DataManager.Plu.GetRecipe ( nPluIdx, DataManager.Plu.GetPluNoInt( nPluIdx ), Recipe );
		bool bRecipeModified = FALSE;
	
		for ( int nItemIdx = 0; nItemIdx < Recipe.GetSize(); nItemIdx++ )
		{
			CString strStockCode = Recipe.GetStockCode( nItemIdx );

			if ( strStockCode != "" )
			{
				int nStockIdx;
				if ( DataManager.Stock.FindStockItemByCode ( strStockCode, nStockIdx ) == TRUE )
				{
					CString strNewStockCode = m_pFilterArray -> GetNewStockCode ( nStockIdx );

					if ( strNewStockCode != strStockCode )
					{
						Recipe.SetStockCode ( nItemIdx, strNewStockCode );
						bRecipeModified = TRUE;
					}
				}
			}
		}
		if ( bRecipeModified == TRUE )
			DataManager.Plu.SetRecipe ( nPluIdx, Recipe );
	}

	int nStartSpIdx, nEndSpIdx;
	dbStockpoint.GetSpIdxRange( DataManager.GetActiveDbIdx(), nStartSpIdx, nEndSpIdx );
	
	//LEGACY DATABASE ORDERS
	{
		CDataManagerInfo infoDb;
		DataManager.CloseOrder( infoDb );
		DataManager.OpenOrder( DATAMANAGER_OPEN_READWRITE, infoDb );

		StatusProgress.SetDialogText( "Legacy database orders" );
		
		COrderCSVRecord OrderRecord;
		nSize = DataManager.Order.GetSize();
		for ( int nOrderIdx = 0; nOrderIdx < nSize; nOrderIdx++ )
		{
			StatusProgress.SetPos ( nOrderIdx, nSize );
			DataManager.Order.GetAt ( nOrderIdx, OrderRecord );
				
			bool bOrderModified = FALSE;
			
			CString strStockCode = OrderRecord.GetStockCode();

			if ( strStockCode != "" )
			{
				int nStockIdx;
				if ( DataManager.Stock.FindStockItemByCode ( strStockCode, nStockIdx ) == TRUE )
				{
					CString strNewStockCode = m_pFilterArray -> GetNewStockCode ( nStockIdx );

					if ( strNewStockCode != strStockCode )
					{
						OrderRecord.SetStockCode ( strNewStockCode );
						bOrderModified = TRUE;
					}
				}
			}
			
			if ( bOrderModified == TRUE )
				DataManager.Order.SetAt ( nOrderIdx, OrderRecord );
		}

		DataManager.WriteOrder( infoDb );
	}

	//LEGACY STOCKPOINT ORDERS
	{
		for ( int nSpIdx = nStartSpIdx; nSpIdx <= nEndSpIdx; nSpIdx++ )
		{
			CDataManagerInfo infoDb;
			DataManager.CloseOrder( infoDb );
			DataManager.OpenOrderSp( nSpIdx, DATAMANAGER_OPEN_READWRITE, infoDb );

			CString strText;
			strText.Format( "Legacy Orders (%s)", dbStockpoint.GetName(nSpIdx) );
			StatusProgress.SetDialogText( strText );
			
			COrderCSVRecord OrderRecord;
			nSize = DataManager.Order.GetSize();
			for ( int nOrderIdx = 0; nOrderIdx < nSize; nOrderIdx++ )
			{
				StatusProgress.SetPos ( nOrderIdx, nSize );
				DataManager.Order.GetAt ( nOrderIdx, OrderRecord );
					
				bool bOrderModified = FALSE;
				
				CString strStockCode = OrderRecord.GetStockCode();

				if ( strStockCode != "" )
				{
					int nStockIdx;
					if ( DataManager.Stock.FindStockItemByCode ( strStockCode, nStockIdx ) == TRUE )
					{
						CString strNewStockCode = m_pFilterArray -> GetNewStockCode ( nStockIdx );

						if ( strNewStockCode != strStockCode )
						{
							OrderRecord.SetStockCode ( strNewStockCode );
							bOrderModified = TRUE;
						}
					}
				}
				
				if ( bOrderModified == TRUE )
					DataManager.Order.SetAt ( nOrderIdx, OrderRecord );
			}

			DataManager.WriteOrder( infoDb );
		}
	}

	//DATABASE ORDERS
	{
		StatusProgress.SetDialogText( "Database orders" );
	
		__int64 nTarget = DataManager.Supplier.GetSize();
		__int64 nProgress = 0;

		for ( int nSuppIdx = -1; nSuppIdx < DataManager.Supplier.GetSize(); nSuppIdx++ )
		{
			StatusProgress.SetPos( nProgress++, nTarget );

			int nSuppNo;
			if ( -1 == nSuppIdx )
				nSuppNo = 0;
			else
				nSuppNo = DataManager.Supplier.GetSuppNo( nSuppIdx );

			CProcessedOrderArray orderArray;
			
			orderArray.LoadDatabaseOrders( nSuppNo, DataManager.GetActiveDbIdx(), "", "", ORDER_TYPE_OPEN, TRUE );
			UpdateProcessedOrders( orderArray );

			orderArray.LoadDatabaseOrders( nSuppNo, DataManager.GetActiveDbIdx(), "", "", ORDER_TYPE_MIXED, TRUE );
			UpdateProcessedOrders( orderArray );

			orderArray.LoadDatabaseOrders( nSuppNo, DataManager.GetActiveDbIdx(), "", "", ORDER_TYPE_CLOSED, TRUE );
			UpdateProcessedOrders( orderArray );
		}
	}

	//STOCKPOINT ORDERS
	{
		StatusProgress.SetDialogText( "Stockpoint orders" );
		
		int nStartSpIdx, nEndSpIdx;
		dbStockpoint.GetSpIdxRange( DataManager.GetActiveDbIdx(), nStartSpIdx, nEndSpIdx );
		
		__int64 nTarget = ( nEndSpIdx + 1 - nStartSpIdx ) * DataManager.Supplier.GetSize();
		__int64 nProgress = 0;

		for ( int nSpIdx = nStartSpIdx; nSpIdx <= nEndSpIdx; nSpIdx++ )
		{
			for ( int nSuppIdx = -1; nSuppIdx < DataManager.Supplier.GetSize(); nSuppIdx++ )
			{
				StatusProgress.SetPos( nProgress++, nTarget );

				int nSuppNo;
				if ( -1 == nSuppIdx )
					nSuppNo = 0;
				else
					nSuppNo = DataManager.Supplier.GetSuppNo( nSuppIdx );

				CProcessedOrderArray orderArray;
				
				orderArray.LoadStockpointOrders( nSuppNo, nSpIdx, "", "", ORDER_TYPE_OPEN, TRUE );
				UpdateProcessedOrders( orderArray );

				orderArray.LoadStockpointOrders( nSuppNo, nSpIdx, "", "", ORDER_TYPE_MIXED, TRUE );
				UpdateProcessedOrders( orderArray );

				orderArray.LoadStockpointOrders( nSuppNo, nSpIdx, "", "", ORDER_TYPE_CLOSED, TRUE );
				UpdateProcessedOrders( orderArray );
			}
		}
	}
	
	StatusProgress.SetDialogText( "Stock database" );
	
	DataManager.Stock.ChangeStockCodes ( m_pFilterArray );
	for ( int nSpIdx = nStartSpIdx; nSpIdx <= nEndSpIdx; nSpIdx++ )
		dbStockpoint.RemoveSyncRequestStockLevels( nSpIdx );
	
	CDataManagerInfo info;
	
	if ( DataManager.WriteStock( info, TRUE, TRUE ) == FALSE )
		Prompter.WriteError( info );
	else
	{
		if ( DataManager.WritePlu( info ) == FALSE )
			Prompter.WriteError( info );
		else
			CCustomExportManagerPlu CustomExport( TRUE );
	}

	DataManager.StockPlu.Invalidate();
	return TRUE;
}

/**********************************************************************/

void CStockListStockCodeDlg::UpdateProcessedOrders( CProcessedOrderArray& orderArray )
{
	for ( int nHeaderIdx = 0; nHeaderIdx < orderArray.GetOrderCount(); nHeaderIdx++ )
	{
		CProcessedOrderEntity HeaderEntity;
		orderArray.GetHeaderEntity( nHeaderIdx, HeaderEntity );
		orderArray.MarkModifiedOrder( HeaderEntity );
	}

	for ( int nItemIdx = 0; nItemIdx < orderArray.GetItemCount(); nItemIdx++ )
	{
		CProcessedOrderItem ItemRecord;
		orderArray.GetItemRecord( nItemIdx, ItemRecord );

		CString strStockCode = ItemRecord.GetStockCode();
				
		int nStockIdx;
		if ( DataManager.Stock.FindStockItemByCode ( strStockCode, nStockIdx ) == TRUE )
		{
			CString strNewStockCode = m_pFilterArray -> GetNewStockCode ( nStockIdx );

			if ( strNewStockCode != strStockCode )
			{
				ItemRecord.SetStockCode ( strNewStockCode );
				orderArray.SetItemRecord( nItemIdx, ItemRecord );
			}
		}
	}
	
	orderArray.SaveOrders();
}

/**********************************************************************/

void CStockListStockCodeDlg::HandleCopySupplierStockCodes()
{
	int nSelectedStockIdx = 0;
	int nSelection = m_listStock.GetCurSel();

	if ( nSelection >= 0 && nSelection < m_pFilterArray -> GetSize() )
		nSelectedStockIdx = m_pFilterArray -> GetStockItemIndexAt ( nSelection );

	CStringArray arrayWarning;
	arrayWarning.Add ( "Copy Supplier References to Stock Codes" );
	arrayWarning.Add ( "Stock items which have a supplier reference for their preferred" );
	arrayWarning.Add ( "+supplier will use this as their new stock code." );
	arrayWarning.Add ( "" );
	arrayWarning.Add ( "Stock items which do not have a supplier reference will retain" );
	arrayWarning.Add ( "+their existing stock codes." );
	arrayWarning.Add ( "" );

	if ( m_pFilterArray -> HasAllRecords() == FALSE )
	{
		arrayWarning.Add ( "Stock items which are not included in the current filter will not" );
		arrayWarning.Add ( "+be affected by this operation." );
	}
	else
		arrayWarning.Add ( "Any other stock code changes will be cancelled." );

	if ( Prompter.Warning( arrayWarning ) != IDYES )
		return;

	StatusProgress.Lock( TRUE, "Copying supplier references" );
	int nCopied = CopySupplierStockCodesInternal();
	StatusProgress.Unlock();

	DisplayScreen ( nSelectedStockIdx );

	CDWordArray dw;
	dw.Add( nCopied );
	dw.Add( m_pFilterArray -> GetSize() );
	MessageLogger.LogSystem( 6, 3, FALSE, dw );

	CSimpleInfoDlg dlg ( "Copy Supplier References to Stock Codes", "Total stock items checked", "Supplier references copied", m_pFilterArray -> GetSize(), nCopied );
	dlg.DoModal();

	CheckDuplicates();
}

/**********************************************************************/

int CStockListStockCodeDlg::CopySupplierStockCodesInternal()
{
	int nSize = m_pFilterArray -> GetSize();

	for ( int n = 0; n < nSize; n++ )
	{
		StatusProgress.SetPos( n, nSize );
		int nStockIdx = m_pFilterArray -> GetStockItemIndexAt(n);
		m_pFilterArray -> SetNewStockCode( nStockIdx, DataManager.Stock.GetStockCode( nStockIdx ) );
	}

	int nCopied = 0;
	for ( int n = 0; n < nSize; n++ )
	{
		StatusProgress.SetPos( n, nSize );
		int nStockIdx = m_pFilterArray -> GetStockItemIndexAt(n);

		CStockCSVRecord StockRecord;
		DataManager.Stock.GetAt ( nStockIdx, StockRecord );

		if ( StockRecord.GetSupplierCount() >= 1 )
		{
			CString strSuppRef = StockRecord.GetSuppRefInternal(0);
			::TrimSpaces ( strSuppRef, FALSE );
			
			if ( strSuppRef.GetLength() > Stock::StockCode.Max )
				strSuppRef = strSuppRef.Left ( Stock::StockCode.Max );

			if ( strSuppRef != "" )
			{
				m_pFilterArray -> SetNewStockCode ( nStockIdx, strSuppRef );
				nCopied++;
			}
		}
	}

	return nCopied;
}

/**********************************************************************/

void CStockListStockCodeDlg::HandleCopyPluNumbers()
{
	int nSelectedStockIdx = 0;
	int nSelection = m_listStock.GetCurSel();

	if ( m_pFilterArray -> HasAllRecords() == FALSE )
	{
		Prompter.Error ( "You cannot use this option when the database is filtered" );
		return;
	}

	if ( nSelection >= 0 && nSelection < m_pFilterArray -> GetSize() )
		nSelectedStockIdx = m_pFilterArray -> GetStockItemIndexAt ( nSelection );

	CStringArray arrayWarning;
	arrayWarning.Add ( "Copy Plu Numbers to Stock Codes" );
	arrayWarning.Add ( "Stock items which are linked to a plu number will use the plu" );
	arrayWarning.Add ( "+plu number as their new stock code." );
	arrayWarning.Add ( "" );
	arrayWarning.Add ( "Stock items which are not linked to a plu number will retain" );
	arrayWarning.Add ( "+their existing stock codes." );
	arrayWarning.Add ( "" );
	arrayWarning.Add ( "Any other stock code changes will be cancelled." );
	
	if ( Prompter.Warning( arrayWarning ) != IDYES )
		return;

	StatusProgress.Lock( TRUE, "Copying PLU numbers" );
	int nCopied = CopyPluNumbersInternal();
	StatusProgress.Unlock();

	CDWordArray dw;
	dw.Add ( nCopied );
	dw.Add ( DataManager.Plu.GetSize() );
	MessageLogger.LogSystem( 6, 2, FALSE, dw );

	DisplayScreen ( nSelectedStockIdx );
	CSimpleInfoDlg dlg ( "Copy Plu Numbers to Stock Codes", "Total plu items checked", "Plu numbers copied", DataManager.Plu.GetSize(), nCopied );
	dlg.DoModal();

	CheckDuplicates();
}

/**********************************************************************/

int CStockListStockCodeDlg::CopyPluNumbersInternal()
{
	//THIS OPERATION MUST APPLY TO ALL RECORDS
	OnNofilterStockRecords();

	//REMOVE PREVIOUS CHANGES
	int nStockSize = DataManager.Stock.GetSize();
	for ( int nStockIdx = 0; nStockIdx < nStockSize; nStockIdx++ )
	{
		StatusProgress.SetPos ( nStockIdx, nStockSize );
		m_pFilterArray -> SetNewStockCode ( nStockIdx, DataManager.Stock.GetStockCode ( nStockIdx ) );
	}
	
	//COPY PLU NUMBERS
	int nCopied = 0;
	int nPluSize = DataManager.Plu.GetSize();

	for ( int nPluIdx = 0; nPluIdx < nPluSize; nPluIdx++ )
	{
		StatusProgress.SetPos ( nPluIdx, nPluSize );
		
		CPluCSVRecord PluRecord;
		DataManager.Plu.GetAt ( ( nPluSize - nPluIdx - 1 ), PluRecord );

		if ( DataManager.Plu.IsRecipe ( PluRecord.GetPluNoInt() ) )
			continue;
	
		CString strStockCode;
		DataManager.Plu.GetPluStockCode ( PluRecord.GetPluNoInt(), strStockCode, FALSE );
		if ( strStockCode == "" )
			continue;

		int nStockIdx;
		if ( DataManager.Stock.FindStockItemByCode ( strStockCode, nStockIdx ) == TRUE )
		{
			m_pFilterArray -> SetNewStockCode ( nStockIdx, PluRecord.GetPluNoString() );
			nCopied++;
		}
	}		
	return nCopied;
}

/**********************************************************************/

void CStockListStockCodeDlg::HandleRemoveStockZeros()
{
	int nSelectedStockIdx = 0;
	int nSelection = m_listStock.GetCurSel();

	if ( m_pFilterArray -> HasAllRecords() == FALSE )
	{
		Prompter.Error ( "You cannot use this option when the database is filtered" );
		return;
	}

	if ( nSelection >= 0 && nSelection < m_pFilterArray -> GetSize() )
		nSelectedStockIdx = m_pFilterArray -> GetStockItemIndexAt ( nSelection );

	CStringArray arrayWarning;
	arrayWarning.Add ( "Remove Leading Zeros from Stock Codes" );
	arrayWarning.Add ( "Leading zeros will be removed from all stock codes that do not" );
	arrayWarning.Add ( "+consist entirely of zeros." );
	arrayWarning.Add ( "" );
	arrayWarning.Add ( "Any other stock code changes will be cancelled." );
	
	if ( Prompter.Warning( arrayWarning ) != IDYES )
		return;

	StatusProgress.Lock( TRUE, "Removing leading zeros" );
	RemoveStockZerosInternal();
	StatusProgress.Unlock();

	MessageLogger.LogSystem( 6, 4, FALSE );

	DisplayScreen ( nSelectedStockIdx );
	Prompter.Info ( "Leading zeros have been removed as requested." );
	CheckDuplicates();
}

/**********************************************************************/

void CStockListStockCodeDlg::RemoveStockZerosInternal()
{
	//THIS OPERATION MUST APPLY TO ALL RECORDS
	OnNofilterStockRecords();

	//REMOVE PREVIOUS CHANGES
	int nSize = DataManager.Stock.GetSize();
	for ( int nStockIdx = 0; nStockIdx < nSize; nStockIdx++ )
	{
		StatusProgress.SetPos ( nStockIdx, nSize );
		m_pFilterArray -> SetNewStockCode ( nStockIdx, DataManager.Stock.GetStockCode ( nStockIdx ) );
	}
	
	for ( int nStockIdx = 0; nStockIdx < nSize; nStockIdx++ )
	{
		StatusProgress.SetPos ( nStockIdx, nSize );
	
		CStockCSVRecord StockRecord;
		DataManager.Stock.GetAt ( nStockIdx, StockRecord );

		CString strStockCode = StockRecord.GetStockCode();
		strStockCode.TrimLeft("0");

		CString strCheck = strStockCode;
		::TrimSpaces ( strCheck, FALSE );

		if ( strCheck != "" )
			m_pFilterArray -> SetNewStockCode ( nStockIdx, strStockCode );		
	}
}

/**********************************************************************/

void CStockListStockCodeDlg::CheckDuplicates()
{
	StatusProgress.Lock( TRUE, "Checking stock codes" );
	CheckDuplicatesInternal();
	StatusProgress.Unlock();

	if ( m_bCheckDuplicates == TRUE )
	{
		CString strMsg = "Some of the new stock codes are duplicated for two\n";
		strMsg += "or more items. You must assign a different code to\n";
		strMsg += "each item before you can use the new codes.";
		
		Prompter.Error ( strMsg );

		CReportManager ReportManager;
		ReportManager.DisplayReport ( Super.ReportFilename(), "", "", this );
	}
}

/**********************************************************************/

void CStockListStockCodeDlg::CheckDuplicatesInternal()
{
	m_bCheckDuplicates = FALSE;
	CConsolidateArray ConsolArray;

	int nSize = DataManager.Stock.GetSize();
	for ( int nStockIdx = 0; nStockIdx < nSize; nStockIdx++ )
	{
		StatusProgress.SetPos ( nStockIdx, nSize, 0, 50 );
		CString strLabel = m_pFilterArray -> GetNewStockCode ( nStockIdx );
		::AddTrailing ( strLabel, Stock::StockCode.Max, ' ' );
		strLabel += DataManager.Stock.GetStockCode ( nStockIdx );
		ConsolArray.Add ( strLabel );
	}
	
	CReportFile ReportFile;
	if ( ReportFile.Open ( Super.ReportFilename() ) == FALSE )
		return;
	
	ReportFile.SetStyle1 ( "Duplicated Stock Codes" );
	ReportFile.AddColumn ( "Current Stock Code", TA_LEFT, 400 );
	ReportFile.AddColumn ( "New Stock Code", TA_LEFT, 400 );

	CString strNewStockCode1 = "";
	nSize = ConsolArray.GetSize();
	int nStartPos = 0;

	int nIndex;
	for ( nIndex = 0; nIndex < nSize; nIndex++ )
	{
		StatusProgress.SetPos ( nIndex, nSize, 50, 50 );

		CString strLabel = ConsolArray.GetLabel ( nIndex );
		CString strNewStockCode2 = strLabel.Left ( Stock::StockCode.Max );

		if ( strNewStockCode2 != strNewStockCode1 )
		{
			if ( nStartPos < nIndex - 1 )
			{
				for ( int nToWrite = nStartPos; nToWrite < nIndex; nToWrite++ )
				{
					CString strLabelToWrite = ConsolArray.GetLabel ( nToWrite );
					CString strOldStockCode = strLabelToWrite.Right ( strLabelToWrite.GetLength() - Stock::StockCode.Max );

					CCSV csv ( '\t', '"', TRUE, FALSE, FALSE );
					csv.Add ( strOldStockCode );
					csv.Add ( strNewStockCode1 );
					ReportFile.WriteLine ( csv.GetLine() );
				}
				ReportFile.WriteLine ( "" );
				m_bCheckDuplicates = TRUE;
			}
			nStartPos = nIndex;
			strNewStockCode1 = strNewStockCode2;
		}
	}

	if ( nStartPos < nSize - 1 )
	{
		for ( int nToWrite = nStartPos; nToWrite < nIndex; nToWrite++ )
		{
			CString strLabelToWrite = ConsolArray.GetLabel ( nToWrite );
			CString strOldStockCode = strLabelToWrite.Right ( strLabelToWrite.GetLength() - Stock::StockCode.Max );

			CCSV csv ( '\t', '"', TRUE, FALSE, FALSE );
			csv.Add ( strOldStockCode );
			csv.Add ( strNewStockCode1 );
			ReportFile.WriteLine ( csv.GetLine() );
			m_bCheckDuplicates = TRUE;
		}		
	}
	ReportFile.Close();
}

/**********************************************************************/

const char* CStockListStockCodeDlg::GetSpName()
{
	m_strSpName = "";
	if ( dbDatabase.GetSize() >= 2 )
	{
		m_strSpName = dbDatabase.GetName( DataManager.GetActiveDbIdx() );
		m_strSpName += ", ";
	}
	return m_strSpName;
}

/**********************************************************************/

void CStockListStockCodeDlg::PrepareMenu()
{
	m_MenuChanger.KillParent ( IDM_EDIT_STOCKLEVEL );
	m_MenuChanger.KillParent( IDM_IMPORT_STOCKLEVEL );
	m_MenuChanger.KillItem ( IDM_COPY_LEVELS );
	m_MenuChanger.KillItem ( IDM_IMPORT_CREATESHEET );
	m_MenuChanger.KillParent ( IDM_PRINT_SESSION );

	if ( StockOptions.GetStockAddLeadingZerosFlag() )
	{
		m_MenuChanger.KillItem ( IDM_REMOVE_ZEROS );
		m_MenuChanger.KillItem ( IDM_COPY_SUPPLIER );
	}

	m_MenuChanger.KillItem( IDM_RANGE_ACTIVE );

	if ( DealerFlags.GetSuppRefFlag() )
		m_MenuChanger.KillItem ( IDM_COPY_SUPPLIER );
}

/**********************************************************************/



