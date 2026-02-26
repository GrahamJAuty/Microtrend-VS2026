/**********************************************************************/
#include "..\CommonEcrStock\CloneDatabaseDlg.h"
#include "Password.h"
#include "..\CommonEcrStock\PluAutoImporterCSV.h"
#include "..\CommonEcrStock\PluAutoImporterEOD.h"
#include "StockOptionsIni.h"
#include "StockpointCSVArray.h"
#include "UnitechBatchJobDlg.h"
/**********************************************************************/
#include "..\CommonEcrStock\PropPageSystemDataList.h"
/**********************************************************************/

void CPropPageSystemDataList::PrepareStockDataPage()
{	
	m_Filter.PrepareStockDataFilter();
}

/**********************************************************************/

void CPropPageSystemDataList::BuildStockDataList()
{
	m_arrayLocation.RemoveAll();
	m_arrayAction.RemoveAll();

	if ( EcrmanOptions.GetReportsLocationSortType() == 0 )
	{
		for ( int nDbIdx = 0; nDbIdx < dbDatabase.GetSize(); nDbIdx++ )
			if ( PasswordArray.CheckLocationAccessDbIdx( nDbIdx ) == TRUE )
				BuildStockDataListDatabase( nDbIdx );
	}
	else
	{
		CWordArray arrayDbIdx;
		dbDatabase.SortByName( arrayDbIdx );

		for ( int n = 0; n < arrayDbIdx.GetSize(); n++ )
		{
			int nDbIdx = arrayDbIdx.GetAt(n);
			if ( PasswordArray.CheckLocationAccessDbIdx( nDbIdx ) == TRUE )
				BuildStockDataListDatabase( nDbIdx );
		}
	}
		
	m_buttonAction1.SetWindowText ( "Edit" );
	m_buttonAction2.SetWindowText ( "Clone" );
	m_buttonAction3.ShowWindow ( SW_HIDE );
	m_buttonAction4.SetWindowText ( "Reports" );
	m_buttonAction5.SetWindowText ( "Audit" );
	m_buttonAction6.ShowWindow ( SW_HIDE );
	
	if ( dbStockpoint.GetSize() == 0 )
	{
		m_buttonAction4.ShowWindow ( SW_HIDE );
		m_buttonAction5.ShowWindow ( SW_HIDE );
	}

	if ( CheckCloneDatabase() == FALSE )
		m_buttonAction2.ShowWindow( SW_HIDE );

	SelectLine( m_listLocation, 0 );
}

/**********************************************************************/

void CPropPageSystemDataList::BuildStockDataListDatabase( int nDbIdx )
{
	if ( GotStockpointData() == TRUE )	
	{
		CWordArray arraySpIdx;
		dbStockpoint.SortByName( arraySpIdx, nDbIdx );

		bool bGotDatabase = FALSE;
		for ( int n = 0; n < arraySpIdx.GetSize(); n++ )
		{
			int nSpIdx = arraySpIdx.GetAt(n);
			if ( PasswordArray.CheckLocationAccessSpIdx( nSpIdx ) == TRUE )
			{
				if ( FALSE == bGotDatabase )
				{
					AddDatabase( nDbIdx );
					bGotDatabase = TRUE;
				}

				AddStockpoint( nSpIdx );
			}
		}
	}
}

/**********************************************************************/

void CPropPageSystemDataList::SelectLocationStockData()
{
	int nAction;
	CLocationSelectorEntity LocSelEntity;
	GetPriceSetAction( LocSelEntity, nAction );

	m_arrayAction.RemoveAll();
	m_listAction.DeleteAllItems();

	AddAction( NODE_STOCK, "Stock Records" );

	if ( LocSelEntity.GetConType() == NODE_STOCKPOINT )
	{
		int nDbIdx = LocSelEntity.GetDbIdx();
		int nSpIdx = LocSelEntity.GetEntityIdx();

		if ( m_Filter.GetUnitechFlag() == SCOPE_STKPOINT )		AddAction ( NODE_UNITECH_STOCK, "Unitech Import" );
		if ( m_Filter.GetOpeningFlag() == SCOPE_STKPOINT )		AddAction ( NODE_OPENING, "Opening Stock" );		
		if ( m_Filter.GetDeliveriesFlag() == SCOPE_STKPOINT )	AddAction ( NODE_DELIVERY, "Deliveries" );		
	
		if ( m_Filter.GetManualSalesFlag() == SCOPE_STKPOINT )	
		{
			switch( StockOptions.GetStockSalesEntryType() )
			{
			case 0:
				AddAction ( NODE_MANUALSALE_PLU, "Manual Sales" );
				break;

			case 1:
				AddAction ( NODE_MANUALSALE_STOCK, "Manual Sales" );
				break;

			case 2:
				AddAction ( NODE_MANUALSALE_STOCK, "Manual Sales (Stock)" );
				AddAction ( NODE_MANUALSALE_PLU, "Manual Sales (Plu)" );
				break;
			}
		}
	
		if ( m_Filter.GetReturnsFlag() == SCOPE_STKPOINT )		AddAction ( NODE_RETURN, "Returns" );		
	
		if ( m_Filter.GetAdjustmentsFlag() == SCOPE_STKPOINT )	
		{
			switch( StockOptions.GetStockAdjustmentsEntryType() )
			{
			case 0:
				AddAction ( NODE_ADJUSTMENT_STOCK, "Adjustments" );
				break;

			case 1:
				AddAction ( NODE_ADJUSTMENT_PLU, "Adjustments" );
				break;

			case 2:
				AddAction ( NODE_ADJUSTMENT_STOCK, "Adjustments (Stock)" );
				AddAction ( NODE_ADJUSTMENT_PLU, "Adjustments (Plu)" );
				break;
			}
		}
		
		if ( m_Filter.GetTransfersFlag() == SCOPE_STKPOINT )
		{
			bool bCanTransfer = FALSE;
			if ( dbStockpoint.GetTransferOutFlag( nSpIdx ) == TRUE )
			{
				if ( dbStockpoint.GetTransferInFlag( nSpIdx ) == TRUE )
				{
					if ( m_Filter.GetDbTransferTargets(nDbIdx) >= 2 )
						bCanTransfer = TRUE;
				}
				else
				{
					if ( m_Filter.GetDbTransferTargets(nDbIdx) >= 1 )
						bCanTransfer = TRUE;
				}
			}

			if ( TRUE == bCanTransfer )
				AddAction ( NODE_TRANSFER, "Transfers" );
		}
			
		if ( m_Filter.GetReorderFlag() == SCOPE_STKPOINT )		
		{
			if ( StockOptions.GetOrderGenerationMethod() == ORDER_TYPE_SALES )
				AddAction ( NODE_REORDER, "Min / Max Levels" );
			else
				AddAction ( NODE_REORDER, "Min / Reorder / Max Levels" );
		}

		if ( m_Filter.GetActiveStockFlag() == SCOPE_STKPOINT )
			AddAction ( NODE_STOCK_ACTIVE, "Stockpoint Item Set" );
	}

	m_buttonAction2.EnableWindow( m_bCanCloneDatabase );
	ForceSelectAction( nAction );
}

/**********************************************************************/

void CPropPageSystemDataList::GetStockDataAction( CLocationSelectorEntity& LocSelEntity, int& nAction )
{
	LocSelEntity.SetSystemNode();
	nAction = 0;

	{
		int nSel = m_listLocation.GetCurSel();
		if ( ( nSel >= 0 ) && ( nSel < m_arrayLocation.GetSize() ) )
			LocSelEntity = m_arrayLocation.GetAt( nSel );
	}

	{
		int nSel = m_listAction.GetCurSel();
		if ( ( nSel >= 0 ) && ( nSel < m_arrayAction.GetSize() ) )
			nAction = m_arrayAction.GetAt(nSel);
	}
}

/**********************************************************************/

void CPropPageSystemDataList::Button1StockDataPage()
{
	int nAction;
	CLocationSelectorEntity LocSelEntity;
	GetStockDataAction( LocSelEntity, nAction );

	if ( NODE_STOCK == nAction )
	{
		m_pHelpers -> m_StockData.EditStock( LocSelEntity.GetDbNo(), FALSE );
	}
	else if ( NODE_STOCKPOINT == LocSelEntity.GetConType() )
	{
		int nSpIdx = LocSelEntity.GetEntityIdx();

		switch( nAction )
		{
		case NODE_OPENING:		
		case NODE_TRANSFER:		
		case NODE_REORDER:
		case NODE_STOCK_ACTIVE:
			m_pHelpers -> m_StockData.EditStockLevel(nSpIdx,nAction,FALSE);			
			break;

		case NODE_UNITECH_STOCK:
			{
				int nDbIdx = LocSelEntity.GetDbIdx();
				CUnitechBatchJobDlg dlgImport( NODE_UNITECH_STOCK, nSpIdx );
				dlgImport.DoModal();
				PluAutoImporterCSV.ImportPluExternal( nDbIdx );
				PluAutoImporterEOD.ImportPluExternal( nDbIdx );
			}
			break;

		case NODE_MANUALSALE_PLU:
		case NODE_MANUALSALE_STOCK:
			m_pHelpers -> m_StockData.EditManualSales(nSpIdx,nAction,FALSE);					
			break;

		case NODE_DELIVERY:		
		case NODE_RETURN:
		case NODE_ADJUSTMENT_PLU:
		case NODE_ADJUSTMENT_STOCK:
			m_pHelpers -> m_StockData.EditMiscStockLevels(nSpIdx,nAction,FALSE);		
			break;	
		}
	}
}

/**********************************************************************/

void CPropPageSystemDataList::Button2StockDataPage()
{
	int nAction;
	CLocationSelectorEntity LocSelEntity;
	GetStockDataAction( LocSelEntity, nAction );
	CloneDatabase( LocSelEntity.GetDbNo() );	
}

/**********************************************************************/

void CPropPageSystemDataList::Button4StockDataPage()
{
	int nAction;
	CLocationSelectorEntity LocSelEntity;
	GetStockDataAction( LocSelEntity, nAction );
	m_pHelpers -> m_StockData.HandleStockReports( 0, FALSE, LocSelEntity );
}

/**********************************************************************/

void CPropPageSystemDataList::Button5StockDataPage()
{
	int nAction;
	CLocationSelectorEntity LocSelEntity;
	GetStockDataAction( LocSelEntity, nAction );
	m_pHelpers -> m_StockData.HandleStockAudits( 0, FALSE, LocSelEntity );
}

/**********************************************************************/
