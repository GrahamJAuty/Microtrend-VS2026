/**********************************************************************/
#include "..\CommonEcrStock\PresetReportManager.h"
/**********************************************************************/
#include "ExplodePlu.h"
#include "FileRemover.h"
#include "MessageLogger.h"
#include "NodeTypes.h"
#include "Password.h"
#include "PeriodEndWarningDlg.h"
#include "PriceHelpers.h" 
#include "RepCSV.h"
#include "ReportStocktakeNewDlg.h"
#include "StatusProgress.h"
#include "StockAuditBaseReport.h"
#include "StockListStocktakeDlg.h"
#include "StockmateLink.h"
#include "StockOptionsIni.h"
#include "StockTakeLevelsDlg.h"
#include "StocktakeNewDlg.h"
/**********************************************************************/
#include "StocktakeDlg.h"
/**********************************************************************/

void CStocktakeDlg::HandleAbandon() 
{
	int nSel = m_listStocktakes.GetCurSel();
	if ( nSel < 0 || nSel >= m_StocktakeArray.GetSize() )
		return;

	CStocktakeCSVRecord Stocktake;
	m_StocktakeArray.GetAt( nSel, Stocktake );

	CString strTitle;
	strTitle.Format ( "Abandon Stocktake (%s,%s,%s)",
		dbStockpoint.GetName( m_nSpIdx ),
		Stocktake.GetCategory(),
		Stocktake.GetFormattedDate() );

	CStringArray arrayWarning;
	arrayWarning.Add( strTitle );
	arrayWarning.Add( "The stock take shown above will be abandoned." );
	arrayWarning.Add( "" );
	arrayWarning.Add( "All stock levels entered for this stock take will be lost." );
	arrayWarning.Add( "" );
	arrayWarning.Add( "You will not be able to edit this stock take again." );

	if ( Prompter.Warning( arrayWarning, FALSE ) != IDYES )
		return;

	int nSize = DataManager.Stock.GetSize();
	int nSpNo = dbStockpoint.GetSpNo( m_nSpIdx );
	int nStocktakeNo = Stocktake.GetStocktakeNo();
	
	StatusProgress.Lock( TRUE, "Abandoning stocktake" );
	for ( int nStockIdx = 0; nStockIdx < nSize; nStockIdx++ )
	{
		StatusProgress.SetPos( nStockIdx, nSize );

		CStockCSVRecord StockRecord;
		DataManager.Stock.GetAt( nStockIdx, StockRecord );

		CStockLevelsCSVRecord StockLevels;
		DataManager.Stock.GetStockpointTotals( nStockIdx, StockRecord, nSpNo, StockLevels );
		
		if ( StockLevels.GetStocktakeNo() == nStocktakeNo )
		{
			StockLevels.AbandonStocktake( nStocktakeNo );
			DataManager.Stock.SetStockpointTotals( nStockIdx, StockRecord, nSpNo, StockLevels );

			CTransferDetailCSVRecord TransferDetail;
			DataManager.Stock.GetTransferTotals( nStockIdx, StockRecord, nSpNo, TransferDetail );
			TransferDetail.AbandonStocktake();
			DataManager.Stock.SetTransferTotals( nStockIdx, StockRecord, nSpNo, TransferDetail );
		}
	}
	StatusProgress.Unlock();

	m_StocktakeArray.RemoveStocktake( nSel );
	
	nSize = m_StocktakeArray.GetSize();
	m_listStocktakes.SetItemCountEx( nSize );
	m_listStocktakes.SetCurSel( ( nSel < nSize ) ? nSel : nSize - 1 );
	m_listStocktakes.Invalidate();
	
	CDataManagerInfo info;
	if ( DataManager.WriteStock( info, FALSE, TRUE ) == FALSE )
	{
		Prompter.WriteError( info );
		return;
	}
	else
	{
		m_StocktakeArray.Write( m_nSpIdx );

		CDWordArray dw;
		dw.Add( dbStockpoint.GetDbNo( m_nSpIdx ) );
		dw.Add( dbStockpoint.GetSpNo( m_nSpIdx ) );
		dw.Add( nStocktakeNo );
		dw.Add( Stocktake.GetItemCount() );
		dw.Add( DataManager.Stock.GetSize() );

		CStringArray str;
		str.Add( Stocktake.GetSupplier() );
		str.Add( Stocktake.GetCategory() );

		MessageLogger.LogSystem( 8, 2, m_bLogDate, dw, str );
		m_bLogDate = FALSE;

		Prompter.Info ( "The stocktake was abandoned" );
	}
}

/**********************************************************************/

void CStocktakeDlg::HandleNew() 
{
	if ( m_StocktakeArray.GetNextFreeNumber() <= 0)
	{
		Prompter.Error ( "You have already created the maximum number\nof concurrent stocktakes for this stockpoint." );
		return;
	}

	CWordArray arraySuppIdx;
	CCategorySetCSVRecord CategorySet;
	CStocktakeNewDlg dlg ( TRUE, CategorySet, arraySuppIdx, this );
	dlg.SetImportSheetHelpers(m_nSpIdx, &m_StocktakeArray, m_strDate, 0);
	if (dlg.DoModal() == IDCANCEL)
	{
		return;
	}
	
	int nStocktakeIdx = 0;
	int nStocktakeNo = 0;
	m_StocktakeArray.AddStocktake( nStocktakeIdx, nStocktakeNo );
	if (nStocktakeIdx < 0)
	{
		return;
	}

	CStocktakeCSVRecord Stocktake;

	m_StocktakeArray.GetAt( nStocktakeIdx, Stocktake );
	Stocktake.SetItemCount ( BeginStocktake( nStocktakeNo, CategorySet, arraySuppIdx, ( dlg.m_bActive != 0 ), ( dlg.m_bClosingZero != 0 ), TRUE, m_nSpIdx ) );
	Stocktake.SetStocktakeType( dlg.m_nStocktakeType );
	Stocktake.SetClosingStockZeroFlag( dlg.m_bClosingZero != 0 );
	Stocktake.SetCategory( dlg.m_strCategory );
	Stocktake.SetSupplier( dlg.m_strSupplier );
	Stocktake.SetActiveOnlyFlag( dlg.m_bActive );
	Stocktake.SetComment( dlg.m_strComment );
	Stocktake.SetActiveOnlyFlag( dlg.m_bActive );
	Stocktake.SetDate( m_strDate );
	m_StocktakeArray.SetAt( nStocktakeIdx, Stocktake );

	m_listStocktakes.SetItemCountEx( m_StocktakeArray.GetSize() );
	m_listStocktakes.SetCurSel( nStocktakeIdx );
	m_listStocktakes.Invalidate();
	
	CDataManagerInfo info;
	if ( DataManager.WriteStock( info, FALSE, TRUE ) == FALSE )
	{
		Prompter.WriteError( info );
		return;
	}
	else
	{
		m_StocktakeArray.Write( m_nSpIdx );

		CDWordArray dw;
		dw.Add( dbStockpoint.GetDbNo( m_nSpIdx ) );
		dw.Add( dbStockpoint.GetSpNo( m_nSpIdx ) );
		dw.Add( nStocktakeNo );
		dw.Add( Stocktake.GetItemCount() );
		dw.Add( DataManager.Stock.GetSize() );
		dw.Add( dlg.m_nStocktakeType );
		dw.Add( dlg.m_bClosingZero );

		CStringArray str;
		str.Add( dlg.m_strSupplier );
		str.Add( dlg.m_strCategory );

		MessageLogger.LogSystem( 8, 1, m_bLogDate, dw, str );
		m_bLogDate = FALSE;
	}
}

/**********************************************************************/

void CStocktakeDlg::HandleEdit() 
{
	int nSel = m_listStocktakes.GetCurSel();
	if (nSel < 0 || nSel >= m_StocktakeArray.GetSize())
	{
		return;
	}

	CStocktakeCSVRecord Stocktake;
	m_StocktakeArray.GetAt( nSel, Stocktake );

	CCategorySetCSVRecord CategorySet;
	CWordArray arraySuppIdx;
	CStocktakeNewDlg dlg ( FALSE, CategorySet, arraySuppIdx, this );
	dlg.SetImportSheetHelpers(m_nSpIdx, &m_StocktakeArray, m_strDate, Stocktake.GetStocktakeNo());

	dlg.m_nStocktakeType = Stocktake.GetStocktakeType();
	dlg.m_bClosingZero = Stocktake.GetClosingStockZeroFlag();
	dlg.m_strCategory = Stocktake.GetCategory();
	dlg.m_strSupplier = Stocktake.GetSupplier();
	dlg.m_strComment = Stocktake.GetComment();
	dlg.m_bActive = Stocktake.GetActiveOnlyFlag();
		
	if ( dlg.DoModal() == IDCANCEL )
		return;
	
	Stocktake.SetStocktakeType( dlg.m_nStocktakeType );
	Stocktake.SetComment( dlg.m_strComment );
	
	m_StocktakeArray.SetAt( nSel, Stocktake );
	m_StocktakeArray.Write( m_nSpIdx );

	m_listStocktakes.Invalidate();
}

/**********************************************************************/

void CStocktakeDlg::HandleLevels() 
{
	CStocktakeLevelsDlg dlgLevels( m_nSpIdx, this );
	if ( dlgLevels.DoModal() == IDOK )
	{
		switch( dlgLevels.m_nNodeType )
		{
		case NODE_CLOSING:				HandleClosing();			break;
		case NODE_DELIVERY:				HandleDeliveries();			break;
		case NODE_RETURN:				HandleReturns();			break;
		case NODE_ADJUSTMENT_PLU:		HandleAdjustmentPlu();		break;
		case NODE_ADJUSTMENT_STOCK:		HandleAdjustmentStock();	break;
		case NODE_MANUALSALE_PLU:		HandleSalesPlu();			break;
		case NODE_MANUALSALE_STOCK:		HandleSalesStock();			break;
		}
	}
}


/**********************************************************************/

void CStocktakeDlg::HandleClosing() 
{
	int nSel = m_listStocktakes.GetCurSel();
	if ( nSel < 0 || nSel >= m_StocktakeArray.GetSize() )
		return;

	CStocktakeCSVRecord Stocktake;
	m_StocktakeArray.GetAt( nSel, Stocktake );

	CDWordArray dw;
	dw.Add( dbStockpoint.GetDbNo( m_nSpIdx ) );
	dw.Add( dbStockpoint.GetSpNo( m_nSpIdx ) );
	dw.Add( Stocktake.GetStocktakeNo() );
	dw.Add( Stocktake.GetItemCount() );
	dw.Add( DataManager.Stock.GetSize() );
	
	CStringArray str;
	str.Add( Stocktake.GetSupplier() );
	str.Add( Stocktake.GetCategory() );

	MessageLogger.LogSystem( 8, 4, m_bLogDate, dw, str );
	m_bLogDate = FALSE;
	
	CStockListStocktakeDlg dlg ( m_nSpIdx, Stocktake, this );
	
	if (dlg.DoModal() == IDCANCEL)
	{
		if (dlg.GetStockWriteErrorFlag() == TRUE)
		{
			return;
		}
	}

	Stocktake.SetItemCount( dlg.GetUpdatedItemCount() );
	m_StocktakeArray.SetAt( nSel, Stocktake );
	m_listStocktakes.Invalidate();
}

/**********************************************************************/

void CStocktakeDlg::HandleReports() 
{
	DataManager.StockPlu.Invalidate();

	bool bFixStockpoint = ( DealerFlags.GetExtendedStocktakeReportsFlag() == FALSE );
	bFixStockpoint |= ( PasswordArray.GetLocationAccessType() == LOCATION_ACCESS_LOC );

	CDataManagerInfo info;

	DataManagerNonDb.OpenCustomFieldNames( DB_READONLY, info );
	PresetReportManager.Open( NODE_PRESET_STOCK_CLOSING, DB_READONLY, info );
	DataManagerNonDb.OpenAllStockTakeSortOrders();

	CReportStocktakeNewDlg dlg ( this );    
	dlg.SetDefaultSelection( m_nSpIdx, NODE_STOCKPOINT, m_listStocktakes.GetCurSel(), bFixStockpoint );
	dlg.DoModal();

	DataManagerNonDb.CloseCustomFieldNames( info );
	PresetReportManager.Close( NODE_PRESET_STOCK_CLOSING, info );
	DataManagerNonDb.CloseAllStockTakeSortOrders();
}

/**********************************************************************/

void CStocktakeDlg::HandleFinalise() 
{
	int nSel = m_listStocktakes.GetCurSel();
	if ( nSel < 0 || nSel >= m_StocktakeArray.GetSize() )
		return;

	CStocktakeCSVRecord Stocktake;
	m_StocktakeArray.GetAt( nSel, Stocktake );

	CStringArray arrayWarning;

	CString strTitle;
	strTitle.Format ( "Finalise Stocktake (%s,%s,%s)",
		dbStockpoint.GetName( m_nSpIdx ),
		Stocktake.GetCategory(),
		Stocktake.GetFormattedDate() );

	arrayWarning.Add( strTitle );

	switch ( Stocktake.GetStocktakeType() )
	{
	case STOCKTAKE_TYPE_PERIOD:	
		arrayWarning.Add ( "The new stock period will start from the date and time shown above." );
		arrayWarning.Add ( "" );
		arrayWarning.Add ( "Period stock levels from before this time will be deleted." );
		arrayWarning.Add ( "" );
		arrayWarning.Add ( "Year to date sales and adjustments will increase accordingly." );
		arrayWarning.Add ( "" );
		arrayWarning.Add ( "Stock sales history and audits will not be affected." );
		arrayWarning.Add ( "" );
		arrayWarning.Add ( "You will not be able to edit this stock take again." );
		break;

	case STOCKTAKE_TYPE_YTD:
		arrayWarning.Add ( "The new stock period will start from the date and time shown above." );
		arrayWarning.Add ( "" );
		arrayWarning.Add ( "Period stock levels from before this time will be deleted." );
		arrayWarning.Add ( "" );
		arrayWarning.Add ( "Year to date sales and adjustments will be deleted." );
		arrayWarning.Add ( "" );
		arrayWarning.Add ( "Stock sales history and audits will not be affected." );
		arrayWarning.Add ( "" );
		arrayWarning.Add ( "You will not be able to edit this stock take again." );
		break;

	case STOCKTAKE_TYPE_ADJUST:
		arrayWarning.Add ( "Apparent stock will be adjusted to match the figures in your stock take." );
		arrayWarning.Add ( "" );
		arrayWarning.Add ( "Other period and year to date levels will not be affected." );
		arrayWarning.Add ( "" );
		arrayWarning.Add ( "Stock sales history and audits will not be affected." );
		arrayWarning.Add ( "" );
		arrayWarning.Add ( "You will not be able to edit this stock take again." );
		break;
	}

	if ( Prompter.Warning( arrayWarning, FALSE ) != IDYES )
		return;
	
	int nStockDbSize = DataManager.Stock.GetSize();
	int nSpNo = dbStockpoint.GetSpNo( m_nSpIdx );
	int nStocktakeNo = Stocktake.GetStocktakeNo();

	//CHECK TO SEE IF WE NEED CLOSING STOCK WARNING
	int nStockItems = 0;
	int nZeroClosing = 0;
	
	StatusProgress.Lock( TRUE, "Checking closing stock levels" );
	for ( int nStockIdx = 0; nStockIdx < nStockDbSize; nStockIdx++ )
	{
		StatusProgress.SetPos( nStockIdx, nStockDbSize );

		CStockCSVRecord StockRecord;
		DataManager.Stock.GetAt( nStockIdx, StockRecord );
		
		CStockLevelsCSVRecord StockLevels;
		DataManager.Stock.GetStockpointTotals( nStockIdx, StockRecord, nSpNo, StockLevels );
		
		if ( StockLevels.GetStocktakeNo() == nStocktakeNo )
		{
			nStockItems++;
			if ( CPriceHelpers::CompareDoubles( StockLevels.GetStocktakeClosingQty(), 0.0, 3 ) == 0 )
				nZeroClosing++;
		}
	}
	StatusProgress.Unlock();

	if ( ( nZeroClosing > 0 ) && ( nZeroClosing * 10 >= nStockItems ) )
	{
		CPeriodEndWarningDlg dlgWarning( nStockItems, nZeroClosing, Stocktake.GetStocktakeType(), this );
		if ( dlgWarning.DoModal() != IDOK )
			return;
	}

	//FINALISE AND AUDIT THE STOCKTAKE	
	COleDateTime dateStocktake = Stocktake.GetOleDateTime();

	CSSFile fileAudit;
	CString strAuditFilename = dbStockpoint.GetFilePathAudit( m_nSpIdx, dateStocktake, AUDIT_FAMILY_NORMAL );
	if ( fileAudit.Open( strAuditFilename, "ab" ) == FALSE )
		return;

	CStockAuditBaseReport Audit;
	Audit.SetReportType( AUDIT_STOCKTAKE );
	
	CStockAuditHeaderInfo infoHeader;
	infoHeader.m_date = dateStocktake;
	infoHeader.m_strReference1 = Stocktake.GetComment();
	infoHeader.m_strReference2 = "";
	fileAudit.WriteLine ( Audit.GetHeaderLine( infoHeader ) );

	int nItemsFinalised = 0;
	StatusProgress.Lock( TRUE, "Finalising stocktake" );
	for ( int nStockIdx = 0; nStockIdx < nStockDbSize; nStockIdx++ )
	{
		StatusProgress.SetPos( nStockIdx, nStockDbSize );

		CStockCSVRecord StockRecord;
		DataManager.Stock.GetAt( nStockIdx, StockRecord );
		double dSURetail = DataManager.Stock.CalculateRetailValue( StockRecord, 1.0 );

		CStockLevelsCSVRecord StockLevels;
		DataManager.Stock.GetStockpointTotals( nStockIdx, StockRecord, nSpNo, StockLevels );

		{
			CCSVStockAudit csv;
			csv.Add ( "D" );
			csv.Add ( AUDIT_STOCKTAKE );
			csv.Add ( 99 );
			csv.Add ( Stocktake.GetStocktakeType() + 1 );
			fileAudit.WriteLine( csv.GetLine() );
		}
		
		if ( StockLevels.GetStocktakeNo() == nStocktakeNo )
		{
			CCSVStockAudit csv;
			csv.Add ( "D" );
			csv.Add ( AUDIT_STOCKTAKE );
			csv.Add ( 1 );
			csv.Add ( StockRecord.GetStockCode() );
			csv.Add ( StockRecord.GetDescription() );
			csv.Add ( DataManager.Category.GetText ( StockRecord.GetCategory() ) );
			csv.Add ( StockRecord.GetSubUnits( StockOptions.GetStockSubUnitsFlag() ) );
			csv.AddStockAuditQty ( StockLevels.GetStocktakeApparentQty() );
			csv.AddStockAuditQty ( StockLevels.GetStocktakeClosingQty() );
			fileAudit.WriteLine( csv.GetLine() );

			StockLevels.FinaliseStocktake( nStocktakeNo, Stocktake.GetStocktakeType(), dSURetail );
			DataManager.Stock.SetStockpointTotals( nStockIdx, StockRecord, nSpNo, StockLevels );
			nItemsFinalised++;
		
			CTransferDetailCSVRecord TransferDetail;
			DataManager.Stock.GetTransferTotals( nStockIdx, StockRecord, nSpNo, TransferDetail );
			TransferDetail.FinaliseStocktake( Stocktake.GetStocktakeType() );
			
			if ( TransferDetail.GetSpCount() != 0 )
				DataManager.Stock.SetTransferTotals( nStockIdx, StockRecord, nSpNo, TransferDetail );
			else
				DataManager.Stock.RemoveTransferTotals( nStockIdx, StockRecord, nSpNo );
		}
	}
	StatusProgress.Unlock();
	fileAudit.Close();

	m_StocktakeArray.RemoveStocktake( nSel );
	
	int nListSize = m_StocktakeArray.GetSize();
	m_listStocktakes.SetItemCountEx( nListSize );
	m_listStocktakes.SetCurSel( ( nSel < nListSize ) ? nSel : nListSize - 1 );
	m_listStocktakes.Invalidate();
	
	CDataManagerInfo info;
	if ( DataManager.WriteStock( info, FALSE, TRUE ) == FALSE )
	{
		Prompter.WriteError( info );
		return;
	}

	m_StocktakeArray.Write( m_nSpIdx );

	if ( nItemsFinalised == nStockDbSize )
	{
		switch ( Stocktake.GetStocktakeType() )
		{
		case STOCKTAKE_TYPE_PERIOD:	
		case STOCKTAKE_TYPE_YTD:
			{
				CString strDate;
				strDate.Format( "%2.2d%2.2d%4.4d", dateStocktake.GetDay(), dateStocktake.GetMonth(), dateStocktake.GetYear() );
				dbStockpoint.SetPeriodStart( m_nSpIdx, strDate );
				dbStockpoint.WriteStockpoint( m_nSpIdx );
			}
			break;
		}
	}

	CDWordArray dw;
	dw.Add( dbStockpoint.GetDbNo( m_nSpIdx ) );
	dw.Add( dbStockpoint.GetSpNo( m_nSpIdx ) );
	dw.Add( nStocktakeNo );
	dw.Add( nStockItems );
	dw.Add( DataManager.Stock.GetSize() );
	dw.Add( nZeroClosing );

	CStringArray str;
	str.Add( Stocktake.GetSupplier() );
	str.Add( Stocktake.GetCategory() );

	MessageLogger.LogSystem( 8, 3, m_bLogDate, dw, str );
	m_bLogDate = FALSE;
	
	Prompter.Info ( "Stock finalisation complete" );
}

/**********************************************************************/

int CStocktakeDlg::BeginStocktake(int nStocktakeNo, CCategorySetCSVRecord& CategorySet, CWordArray& arraySuppIdx, bool bActiveOnly, bool bClosingStockZero, bool bDoTransfers, int nSpIdx)
{
	CByteArray arraySuppNoFlags;
	arraySuppNoFlags.InsertAt(0, 0, 1001);

	for (int n = 0; n < arraySuppIdx.GetSize(); n++)
	{
		int nSuppIdx = arraySuppIdx.GetAt(n);

		if (nSuppIdx == 0)
		{
			arraySuppNoFlags.SetAt(0, 1);
		}
		else
		{
			nSuppIdx--;
			if (nSuppIdx >= 0 && nSuppIdx <= DataManager.Supplier.GetSize())
			{
				int nSuppNo = DataManager.Supplier.GetSuppNo(nSuppIdx);
				if (nSuppNo >= 1 && nSuppNo <= 1000)
				{
					arraySuppNoFlags.SetAt(nSuppNo, 1);
				}
			}
		}
	}

	int nSpNo = dbStockpoint.GetSpNo(nSpIdx);

	int nItemCount = 0;
	int nSize = DataManager.Stock.GetSize();

	StatusProgress.Lock(TRUE, "Opening stocktake");
	for (int nStockIdx = 0; nStockIdx < nSize; nStockIdx++)
	{
		StatusProgress.SetPos(nStockIdx, nSize);

		int nCategory = DataManager.Stock.GetCategory(nStockIdx);
		if (CategorySet.GetCategoryStatus(nCategory) == FALSE)
		{
			continue;
		}

		CStockCSVRecord StockRecord;
		DataManager.Stock.GetAt(nStockIdx, StockRecord);

		int nSuppNo = 0;
		if (StockRecord.GetSupplierCount() != 0)
		{
			nSuppNo = StockRecord.GetSuppNo(0);
		}

		if (nSuppNo < 0 || nSuppNo > 1000)
		{
			continue;
		}

		if (arraySuppNoFlags.GetAt(nSuppNo) == 0)
		{
			continue;
		}

		CStockLevelsCSVRecord StockLevels;
		DataManager.Stock.GetStockpointTotals(nStockIdx, StockRecord, nSpNo, StockLevels);

		if (TRUE == bActiveOnly)
		{
			if (StockLevels.GetActiveFlag() == FALSE)
			{
				continue;
			}
		}

		if (StockLevels.GetStocktakeNo() == 0)
		{
			StockLevels.BeginStocktake(nStocktakeNo, bClosingStockZero);
			DataManager.Stock.SetStockpointTotals(nStockIdx, StockRecord, nSpNo, StockLevels);

			if (TRUE == bDoTransfers)
			{
				CTransferDetailCSVRecord TransferDetail;
				DataManager.Stock.GetTransferTotals(nStockIdx, StockRecord, nSpNo, TransferDetail);
				TransferDetail.BeginStocktake();
				DataManager.Stock.SetTransferTotals(nStockIdx, StockRecord, nSpNo, TransferDetail);
			}

			nItemCount++;
		}
	}
	StatusProgress.Unlock();

	return nItemCount;
}

/**********************************************************************/

void CStocktakeDlg::HandleStockmate()
{
	int nStocktakeIdx = m_listStocktakes.GetCurSel();
	if ((nStocktakeIdx < 0) || (nStocktakeIdx >= m_StocktakeArray.GetSize()))
	{
		return;
	}

	CStocktakeCSVRecord Stocktake;
	m_StocktakeArray.GetAt(nStocktakeIdx, Stocktake);

	CString strTitle;
	strTitle.Format("Prepare Stockmate Data (%s,%s,%s)",
		dbStockpoint.GetName(m_nSpIdx),
		Stocktake.GetCategory(),
		Stocktake.GetFormattedDate());

	CStockmateLink StockmateLink(m_nSpIdx, Stocktake);
	StockmateLink.CreateFiles(this);
}

/**********************************************************************/

