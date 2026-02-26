/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "DeliveryNoteDlg.h"
#include "ManualSalesDatabasePluDlg.h"
#include "ManualSalesDatabaseStockDlg.h"
#include "NodeTypes.h"
/**********************************************************************/
#include "StocktakeDlg.h"
/**********************************************************************/

void CStocktakeDlg::HandleDeliveries() 
{
	DataManager.CloseDatabase();

	CDataManagerInfo infoDM;

	CWordArray arrayReadWrite;
	arrayReadWrite.Add( NODE_STOCK );
	arrayReadWrite.Add( NODE_PLU );
	arrayReadWrite.Add( NODE_BARCODE );

	if ( ( Sysset.IsBarmanSystem() == TRUE ) && ( EcrmanOptions.GetLabelsProductAutoBatchFlag() == TRUE ) )
		arrayReadWrite.Add( NODE_LABELS_PRODUCT );

	CDataManagerInfo info;
	if ( DataManager.OpenDatabaseReadWrite( m_nDbIdx, arrayReadWrite, infoDM, FALSE ) == FALSE )
	{
		DataManager.CloseDatabase();
		Prompter.ShareError( infoDM );
	}
	
	if ( DataManager.OpenStockLevels( m_nSpIdx, DATAMANAGER_OPEN_READWRITE, info ) == FALSE )
	{
		DataManager.CloseDatabase();
		Prompter.ShareError( infoDM );
		return;
	}

	if ( DataManager.OpenTransferDetail( m_nSpIdx, DATAMANAGER_OPEN_READWRITE, info ) == FALSE )
	{
		DataManager.CloseDatabase();
		Prompter.ShareError( infoDM );
		return;
	}

	EditMiscStockLevels( NODE_DELIVERY );
}

/**********************************************************************/

void CStocktakeDlg::HandleReturns() 
{
	EditMiscStockLevels( NODE_RETURN );
}

/**********************************************************************/

void CStocktakeDlg::HandleAdjustmentPlu() 
{
	EditMiscStockLevels( NODE_ADJUSTMENT_PLU );
}

/**********************************************************************/

void CStocktakeDlg::HandleAdjustmentStock() 
{
	EditMiscStockLevels( NODE_ADJUSTMENT_STOCK );
}

/**********************************************************************/

void CStocktakeDlg::HandleSalesPlu() 
{
	EditMiscStockLevels( NODE_MANUALSALE_PLU );
}

/**********************************************************************/

void CStocktakeDlg::HandleSalesStock() 
{
	EditMiscStockLevels( NODE_MANUALSALE_STOCK );
}

/**********************************************************************/

void CStocktakeDlg::EditMiscStockLevels( int nType )
{
	if ( m_nSpIdx < 0 || m_nSpIdx >= dbStockpoint.GetSize() )
		return;

	int nDbIdx;
	int nDbNo = dbStockpoint.GetDbNo( m_nSpIdx );
	if ( dbDatabase.FindDatabaseByNumber ( nDbNo, nDbIdx ) == FALSE )
		return;

	int nSel = m_listStocktakes.GetCurSel();
	if ( nSel < 0 || nSel >= m_StocktakeArray.GetSize() )
		return;

	CStocktakeCSVRecord Stocktake;
	m_StocktakeArray.GetAt( nSel, Stocktake );

	switch ( nType )
	{
	case NODE_RETURN:
	case NODE_ADJUSTMENT_PLU:
	case NODE_ADJUSTMENT_STOCK:
	case NODE_MANUALSALE_PLU:
	case NODE_MANUALSALE_STOCK:

		if ( DataManager.Plu.GetSize() == 0 )
		{
			::StockMovementError( nType, TRUE );
			return;
		}

		if ( DataManager.Stock.GetSize() == 0 )
		{
			::StockMovementError( nType, FALSE );
			return;
		}

		break;
	}

	switch( nType )
	{
	case NODE_DELIVERY:
	case NODE_RETURN:
	case NODE_ADJUSTMENT_PLU:
	case NODE_ADJUSTMENT_STOCK:
		{
			CDeliveryNoteDlg dlg ( m_nSpIdx, nType, &Stocktake, this );
			dlg.DoModal();
		}
		break;

	case NODE_MANUALSALE_PLU:
		{
			DataManager.StockPlu.Invalidate();

			CManualSalesDatabasePluDlg dlg ( m_nSpIdx, &Stocktake, this );
			dlg.DoModal();

			if ( dlg.m_bSaveChanges == TRUE )
			{
				CDataManagerInfo info;
				if ( DataManager.WriteStockLevels( m_nSpIdx, info ) == FALSE )
					Prompter.WriteError( info );
			}
		}
		break;

	case NODE_MANUALSALE_STOCK:
		{
			DataManager.StockPlu.Invalidate();

			CManualSalesDatabaseStockDlg dlg( m_nSpIdx, &Stocktake, this );
			dlg.DoModal();

			if ( dlg.m_bSaveChanges == TRUE )
			{
				CDataManagerInfo info;
				if ( DataManager.WriteStockLevels( m_nSpIdx, info ) == FALSE )
					Prompter.WriteError( info );
			}
		}
		break;
	}
}

/**********************************************************************/

