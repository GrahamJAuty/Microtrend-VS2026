/**********************************************************************/
 
/**********************************************************************/
#include "Append.h"
//#include "DataManager.h"
//#include "EcrmanOptionsIni.h"
#include "ProcessedOrderEditDlg.h"
 
#include "ReportHelpers.h"
#include "StockOptionsIni.h"
#include "StatusProgress.h"
#include "StockAuditBaseReport.h"
/**********************************************************************/
#include "ProcessedOrderDatabaseDlg.h"
/**********************************************************************/

bool CProcessedOrderDatabaseDlg::ToggleDelivery ( int nHeaderIdx, int nItemIdx )
{
	if ( nHeaderIdx < 0 || nHeaderIdx >= m_OrderArray.GetOrderCount() )
		return FALSE;

	if ( nItemIdx < 0 || nItemIdx >= m_OrderArray.GetItemCount() )
		return FALSE;

	CProcessedOrderItem ItemRecord;
	m_OrderArray.GetItemRecord( nItemIdx, ItemRecord );

	//SIGN OF 1 MEANS STOCK IS BEING DELIVERED, -1 MEANS UNDELIVERED
	int nSign = ( ItemRecord.GetDeliveredFlag() == TRUE ) ? -1 : 1;
		
	int nStockIdx;
	if ( DataManager.Stock.FindStockItemByCode ( ItemRecord.GetStockCode(), nStockIdx ) == FALSE )
		return FALSE;

	CStockCSVRecord StockRecord;
	DataManager.Stock.GetAt ( nStockIdx, StockRecord );

	ItemRecord.ExpandStockpointOrders( nStockIdx, StockRecord );

	for ( int nIndex = 0; nIndex < ItemRecord.GetStockpointCount(); nIndex++ )
	{
		int nSpNo = ItemRecord.GetSpNo( nIndex );
	
		double dQtyActual = ItemRecord.GetDUItemSize() * ItemRecord.GetDUItems( nIndex );
		double dValActual = ItemRecord.CalculateSUCost( dQtyActual );
		
		if ( dQtyActual != 0.0 )
		{
			CStockLevelsCSVRecord StockLevels;
			DataManager.Stock.GetStockpointTotals( nStockIdx, StockRecord, nSpNo, StockLevels );

			StockLevels.AddOrder ( -nSign * dQtyActual );
				
			if ( dValActual != 0.0 )
				StockLevels.AddDelivery ( nSign * dQtyActual, nSign * dValActual, FALSE );
			else
				StockLevels.AddFreeStock ( nSign * dQtyActual, FALSE );

			DataManager.Stock.SetStockpointTotals( nStockIdx, StockRecord, nSpNo, StockLevels );
		}
	}

	if ( nSign == 1 )
		ItemRecord.SetDeliveredFlag();
	else
		ItemRecord.ClearDeliveredFlag();

	ItemRecord.ContractStockpointOrders();
	m_OrderArray.SetItemRecord( nItemIdx, ItemRecord );

	CProcessedOrderEntity HeaderEntity;
	m_OrderArray.GetHeaderEntity( nHeaderIdx, HeaderEntity );
	m_OrderArray.MarkModifiedOrder( HeaderEntity );
	
	return TRUE;
}

/**********************************************************************/

void CProcessedOrderDatabaseDlg::OnButtonReplace() 
{
	PartialDelivery ( ORDER_EDIT_REPLACE );
}

/**********************************************************************/

void CProcessedOrderDatabaseDlg::OnDeliverPart() 
{
	PartialDelivery ( ORDER_EDIT_PART );
}

/**********************************************************************/

void CProcessedOrderDatabaseDlg::PartialDelivery( int nMode )
{
	if ( UpdateData ( TRUE ) == FALSE )
		return;

	int nListSelection = m_listOrder.GetCurSel();
	if ( ( nListSelection < 0 ) || ( nListSelection >= m_OrderFilterArray.GetSize() ) )
		return;	

	int nHeaderIdx = m_OrderFilterArray.GetHeaderIndexAt( nListSelection );
	int nItemIdxOld = m_OrderFilterArray.GetItemIndexAt( nListSelection );
	
	CProcessedOrderEntity HeaderEntity;
	m_OrderArray.GetHeaderEntity( nHeaderIdx, HeaderEntity );
	
	CProcessedOrderItem itemOld;
	m_OrderArray.GetItemRecord( nItemIdxOld, itemOld );
	
	if ( itemOld.GetDeliveredFlag() == TRUE )
	{
		Prompter.Error ( "This item has already been delivered." );
		return;
	}

	int nStockIdx;
	if ( DataManager.Stock.FindStockItemByCode ( itemOld.GetStockCode(), nStockIdx ) == FALSE )
	{
		Prompter.Error ( "Unable to deliver this item - stock code\ndoes not exist in database" );
		return;
	}

	CStockCSVRecord StockRecord;
	DataManager.Stock.GetAt ( nStockIdx, StockRecord );

	itemOld.ExpandStockpointOrders( nStockIdx, StockRecord );

	CProcessedOrderItem itemNew;
	itemNew = itemOld;
	CProcessedOrderEditDlg dlg ( itemNew, itemOld, nStockIdx, StockRecord, m_nDbIdx, nMode, m_nSelectedSuppNo, m_nSingleSpIdx, this );
	if ( dlg.DoModal() != IDOK )
		return;

	if ( dlg.GetChangedStockRecordFlag() == TRUE )
		SaveChangedStockRecord( StockRecord );

	//MARK THE ORDER ARRAY AS MODIFIED
	m_OrderArray.MarkModifiedOrder( HeaderEntity ); 
	
	//REMOVE THE ENTIRE ORDER FROM THE ONORDER COUNT
	for ( int nIndex = 0; nIndex < itemOld.GetStockpointCount(); nIndex++ )
	{
		int nSpNo = itemOld.GetSpNo( nIndex );
		double dSUQty = itemOld.GetDUItemSize() * itemOld.GetDUItems( nIndex );
		
		CStockLevelsCSVRecord StockLevels;
		DataManager.Stock.GetStockpointTotals( nStockIdx, StockRecord, nSpNo, StockLevels );
		StockLevels.AddOrder( -dSUQty );
		DataManager.Stock.SetStockpointTotals( nStockIdx, StockRecord, nSpNo, StockLevels );
	}

	//FLAG DELIVERY OF THE STOCK THAT ACTUALLY ARRIVED
	for ( int nIndex = 0; nIndex < itemNew.GetStockpointCount(); nIndex++ )
	{
		int nSpNo = itemNew.GetSpNo( nIndex );
		double dSUQty = itemNew.GetDUItemSize() * itemNew.GetDUItems( nIndex );
		double dSUVal = itemNew.CalculateSUCost( dSUQty );

		CStockLevelsCSVRecord StockLevels;
		DataManager.Stock.GetStockpointTotals( nStockIdx, StockRecord, nSpNo, StockLevels );
		
		if ( dSUVal != 0.0 )
			StockLevels.AddDelivery( dSUQty, dSUVal, FALSE );
		else
			StockLevels.AddFreeStock( dSUQty, FALSE );

		DataManager.Stock.SetStockpointTotals( nStockIdx, StockRecord, nSpNo, StockLevels );
	}

	//MARK THE ORDER AS DELIVERED
	itemNew.SetDeliveredFlag();
	m_OrderArray.SetItemRecord( nItemIdxOld, itemNew );
	
	//ALL DONE IF THIS IS A REPLACEMENT DELIVERY
	if ( nMode != ORDER_EDIT_PART )
	{
		DisplayScreen ( nListSelection );
		return;
	}

	//ALL DONE IF THE FULL AMOUNT ORDERED HAS ARRIVED
	int nDUPending = itemOld.GetDUQty() - itemNew.GetDUQty();
	if ( nDUPending <= 0 )
	{
		DisplayScreen ( nListSelection );
		return;
	}

	//RE-USE THE ORIGINAL ORDER RECORD
	itemNew.ClearDeliveredFlag();

	//CREATE NEW ORDER WITH SAME DELIVERY UNIT
	itemNew.SetDUQty( nDUPending );
	
	int nDUItemsSpare = nDUPending * itemNew.GetDUItems();	
	for ( int nIndex = 0; nIndex < itemOld.GetStockpointCount(); nIndex++ )
	{
		int nDUItemsNeeded = itemOld.GetDUItems( nIndex ) - itemNew.GetDUItems( nIndex );

		if ( nDUItemsNeeded <= 0 )
			itemNew.SetDUItems( nIndex, 0 );
		else if ( nDUItemsNeeded <= nDUItemsSpare )
		{
			itemNew.SetDUItems( nIndex, nDUItemsNeeded );
			nDUItemsSpare -= nDUItemsNeeded;
		}
		else
		{
			itemNew.SetDUItems( nIndex, nDUItemsSpare );
			nDUItemsSpare = 0;
		}
	}

	for ( int nIndex = 0; nIndex < itemNew.GetStockpointCount(); nIndex++ )
	{
		int nSpNo = itemNew.GetSpNo( nIndex );
		double dSUQty = itemNew.GetDUItemSize() * itemNew.GetDUItems( nIndex );
		
		CStockLevelsCSVRecord StockLevels;
		DataManager.Stock.GetStockpointTotals( nStockIdx, StockRecord, nSpNo, StockLevels );
		StockLevels.AddOrder ( dSUQty );
		DataManager.Stock.SetStockpointTotals( nStockIdx, StockRecord, nSpNo, StockLevels );
	}
		
	//ADD THE NEWLY CREATED PART ORDER
	itemNew.ContractStockpointOrders();

	int nNewItemIdx = m_OrderArray.AddItem( HeaderEntity, itemNew, nItemIdxOld );

	BuildRecordList();
	m_OrderFilterArray.ShowDebugInfo();

	int nPos;
	m_OrderFilterArray.FindItemIdx( nNewItemIdx, nPos );

	m_listOrder.SetItemCountEx ( m_OrderFilterArray.GetSize() );
	DisplayScreen ( nPos );
}

/**********************************************************************/

void CProcessedOrderDatabaseDlg::OnDeliverItem() 
{
	if ( UpdateData ( TRUE ) == FALSE )
		return;

	int nListSelection = m_listOrder.GetCurSel();
	if ( ( nListSelection < 0 ) || ( nListSelection >= m_OrderFilterArray.GetSize() ) )
		return;

	int nHeaderIdx = m_OrderFilterArray.GetHeaderIndexAt( nListSelection );
	int nItemIdx = m_OrderFilterArray.GetItemIndexAt ( nListSelection );

	CProcessedOrderItem ItemRecord;
	m_OrderArray.GetItemRecord( nItemIdx, ItemRecord );

	if ( ItemRecord.GetDeliveredFlag() == TRUE )
	{
		Prompter.Error ( "This item has already been delivered." );
		return;
	}

	if ( CanDeliver ( nItemIdx ) == FALSE )
		Prompter.Error ( "Unable to deliver this item - stock code\ndoes not exist in database" );
	else
	{
		ToggleDelivery ( nHeaderIdx, nItemIdx );

		//FIND NEXT DELIVERED ITEM
		while ( nListSelection < m_OrderFilterArray.GetSize() - 1 )
		{
			CProcessedOrderItem ItemRecord;
			int nItemIdx = m_OrderFilterArray.GetItemIndexAt( nListSelection );
			m_OrderArray.GetItemRecord( nItemIdx, ItemRecord );

			if ( ItemRecord.GetDeliveredFlag() == FALSE )
				break;

			nListSelection++;
		}

		DisplayScreen ( nListSelection );
	}
}

/**********************************************************************/

void CProcessedOrderDatabaseDlg::OnDeliverOrder() 
{
	if ( UpdateData ( TRUE ) == FALSE )
		return;
		
	int nListSelection = m_listOrder.GetCurSel();
		
	CProcessedOrderHeader HeaderRecord;
	CProcessedOrderEntity HeaderEntity;
	if ( GetSelectedHeader( HeaderRecord, HeaderEntity ) == FALSE )
	{
		Prompter.Error ( "You must select an item from the list\nbefore you can deliver its order." );
		return;
	}

	if ( m_OrderFilterArray.IsFilteredByDescription() == TRUE )
	{
		Prompter.Error ( "You cannot deliver an entire order when the filter is in use" );
		return;
	}

	CString strMsg;
	strMsg.Format ( "Are you sure you wish to deliver ALL items\nfrom order number %6.6d", HeaderEntity.m_nOrderNo );
	if ( Prompter.YesNo ( strMsg ) == IDNO )
		return;

	bool bGotError = FALSE;

	int nStartIdx, nEndIdx;
	m_OrderArray.GetItemIdxRange( HeaderEntity, nStartIdx, nEndIdx );

	int nHeaderIdx = m_OrderFilterArray.GetHeaderIndexAt( nListSelection );

	for ( int nItemIdx = nStartIdx; nItemIdx <= nEndIdx; nItemIdx++ )
	{	
		if ( CanDeliver ( nItemIdx ) == FALSE )
			bGotError = TRUE;
		else
		{
			CProcessedOrderItem ItemRecord;
			m_OrderArray.GetItemRecord( nItemIdx, ItemRecord );
				
			if ( ItemRecord.GetDeliveredFlag() == FALSE )
				ToggleDelivery ( nHeaderIdx, nItemIdx );
		}
	}
	
	//FIND NEXT UNDELIVERED ORDER
	while ( nListSelection < m_OrderFilterArray.GetSize() - 1 )
	{
		int nHeaderIdx = m_OrderFilterArray.GetHeaderIndexAt( nListSelection );
		int nItemIdx = m_OrderFilterArray.GetItemIndexAt( nListSelection );

		CProcessedOrderEntity NextHeaderEntity;
		m_OrderArray.GetHeaderEntity( nHeaderIdx, NextHeaderEntity );

		CProcessedOrderItem ItemRecord;
		m_OrderArray.GetItemRecord( nItemIdx, ItemRecord );

		if ( ( HeaderEntity.Compare( NextHeaderEntity ) != 0 ) && ( ItemRecord.GetDeliveredFlag() == FALSE ) )
			break;

		nListSelection++;
	}

	DisplayScreen(nListSelection);

	if ( bGotError == TRUE )
	{
		CString strMsg = "One or more items could not be\n";
		strMsg += "delivered because the stock codes\n";
		strMsg += "they refer to no longer exist.";
		Prompter.Error ( strMsg );
	}
}

/**********************************************************************/

void CProcessedOrderDatabaseDlg::AuditDeliveries()
{
	m_bWrittenHeader = FALSE;
	m_arrayWrittenHeader.RemoveAll();
	
	for ( int nSpIdx = 0; nSpIdx < dbStockpoint.GetSize(); nSpIdx++ )
	{
		dbStockpoint.RemoveTemporaryAudits( nSpIdx );
		m_arrayWrittenHeader.Add(0);
	}

	COleDateTime date = COleDateTime::GetCurrentTime();
	
	StatusProgress.Lock( TRUE, "Auditing deliveries" );
	AuditDeliveriesInternal( date );
	StatusProgress.Unlock();

	int nStartIdx, nEndIdx;
	dbStockpoint.GetSpIdxRange( m_nDbIdx, nStartIdx, nEndIdx );
	for ( int nSpIdx = nStartIdx; nSpIdx <= nEndIdx; nSpIdx++ )
	{
		CString strTemp = dbStockpoint.GetFilePathAudit( nSpIdx,date, AUDIT_FAMILY_TEMP );
		CString strPeriod = dbStockpoint.GetFilePathAudit( nSpIdx,date, AUDIT_FAMILY_NORMAL );
		::Append( strTemp, strPeriod );
		dbStockpoint.RemoveTemporaryAudits( nSpIdx );
	}
}

/**********************************************************************/

void CProcessedOrderDatabaseDlg::AuditDeliveriesInternal( COleDateTime date )
{
	for ( int nHeaderIdx = 0; nHeaderIdx < m_OrderArray.GetOrderCount(); nHeaderIdx++ )
	{
		StatusProgress.SetPos ( nHeaderIdx, m_OrderArray.GetOrderCount() );
		
		CProcessedOrderEntity HeaderEntity;
		m_OrderArray.GetHeaderEntity( nHeaderIdx, HeaderEntity );
		
		int nStartIdx, nEndIdx;
		m_OrderArray.GetItemIdxRange( HeaderEntity, nStartIdx, nEndIdx );

		for ( int nItemIdx = nStartIdx; nItemIdx <= nEndIdx; nItemIdx++ )
		{
			CProcessedOrderItem ItemRecord;
			m_OrderArray.GetItemRecord( nItemIdx, ItemRecord );

			if ( ItemRecord.GetDeliveredFlag() == FALSE )
				continue;

			if ( ItemRecord.GetDeliveredOldFlag() == TRUE )
				continue;
		
			int nStockIdx = ItemRecord.GetStockIdx();
			if ( nStockIdx < 0 || nStockIdx >= DataManager.Stock.GetSize() )
				continue;
	
			ItemRecord.ContractStockpointOrders();
			AuditOrder( date, ItemRecord, HeaderEntity.m_nOrderNo );

			if ( Sysset.IsBarmanSystem() && EcrmanOptions.GetLabelsProductAutoBatchFlag() )
			{
				double dTotalOrderQty = 0.0;
				for ( int nIndex = 0; nIndex < ItemRecord.GetStockpointCount(); nIndex++ )
					dTotalOrderQty += ItemRecord.GetDUItems ( nIndex );

				CStockCSVRecord StockRecord;
				DataManager.Stock.GetAt( nStockIdx, StockRecord );
				m_arrayLabels.AutoBarcode ( StockRecord, int ( dTotalOrderQty ) );
			}
		}
	}
}

/**********************************************************************/

void CProcessedOrderDatabaseDlg::AuditOrder( COleDateTime date, CProcessedOrderItem& item, int nOrderNum )
{
	CSSFile fileAudit;

	if ( fileAudit.Open ( m_strTempFile, "ab" ) == TRUE )
	{
		if ( m_bWrittenHeader == FALSE )
		{
			CSSDate dateNow;
			dateNow.SetCurrentDate();
			
			CCSV csv;
			csv.Add ( "HEADER3" );
			csv.Add ( dateNow.GetCSVDate() );
			csv.Add ( m_strReference );
			csv.Add ( "" );
			csv.Add ( nOrderNum );
			fileAudit.WriteLine ( csv.GetLine() );

			m_bWrittenHeader = TRUE;
		}

		CCSV csv( ',', '"', FALSE, TRUE, TRUE );
		item.ConvertToCSV ( csv );
		fileAudit.WriteLine ( csv.GetLine() );

		int nStockIdx;
		CString strCategory = "";
		CStockCSVRecord StockRecord;
		CString strStockCode = item.GetStockCode();
		if ( DataManager.Stock.FindStockItemByCode( strStockCode, nStockIdx ) == TRUE )
		{
			DataManager.Stock.GetAt( nStockIdx, StockRecord );
			strCategory = DataManager.Category.GetText( StockRecord.GetCategory() );
		}

		CString strSupplier = "";
		if ( m_nSelectedSuppIdx >= 0 && m_nSelectedSuppIdx < DataManager.Supplier.GetSize() )
			strSupplier = DataManager.Supplier.GetName( m_nSelectedSuppIdx );
		else
			strSupplier = "Unspecified";

		for ( int nOrderSpIdx = 0; nOrderSpIdx < item.GetStockpointCount(); nOrderSpIdx++ )
		{
			int nDbNo = dbDatabase.GetDbNo( m_nDbIdx );
			int nSpNo = item.GetSpNo( nOrderSpIdx );

			int nSpIdx;
			if ( dbStockpoint.FindStockpointByNumber( nDbNo, nSpNo, nSpIdx ) == TRUE )
			{
				CString strFilename = dbStockpoint.GetFilePathAudit( nSpIdx, date, AUDIT_FAMILY_TEMP );
				
				CSSFile fileAdhocAudit;
				if ( fileAdhocAudit.Open( strFilename, "ab" ) == TRUE )
				{
					if ( m_arrayWrittenHeader.GetAt( nSpIdx ) == 0 )
					{			
						CStockAuditBaseReport Audit;
						Audit.SetReportType( AUDIT_DELIVERY );
						
						CStockAuditHeaderInfo info;
						info.m_date = date;					
						info.m_strReference1 = m_strReference;
						info.m_strReference2 = "";
						info.m_strSuppName = strSupplier;
						fileAdhocAudit.WriteLine( Audit.GetHeaderLine( info ) );
						
						m_arrayWrittenHeader.SetAt( nSpIdx, 1 );
					}

					double dDUItems = item.GetDUItems( nOrderSpIdx );
					double dDUItemSize = item.GetDUItemSize();
					double dTotalQty = dDUItems * dDUItemSize;
					double dTotalCost = item.CalculateSUCost( dTotalQty );

					double dLineRetail = DataManager.Stock.CalculateRetailValue( StockRecord, dTotalQty );
					double dLineTax = DataManager.Stock.CalculateTaxContentFromValue( StockRecord, dLineRetail );
	
					double dLineWaste = 0.0;
					if ( StockOptions.GetStockSubUnitsFlag() )
					{
						double dNonWastePercent = 100.0 - StockRecord.GetWastePercent();
		
						if ( dNonWastePercent != 0.0 )
						{
							double dCostWithWaste = ( dTotalCost * 100.0 ) / dNonWastePercent;  
							dLineWaste = dCostWithWaste - dTotalCost;
						}
					}

					double dGPPercent = ReportHelpers.CalcGPPercent( dLineRetail - dLineTax, 0, dTotalCost + dLineWaste );
					double dCostTax = DataManager.Stock.CalculateTaxOnCost( StockRecord, dTotalCost );

					CCSVStockAudit csv;
					csv.Add ( "D" );
					csv.Add ( AUDIT_DELIVERY );
					csv.Add ( 4 );
					csv.Add ( item.GetStockCode() );
					csv.Add ( item.GetDescription() );
					csv.Add ( strCategory );
					csv.AddStockAuditQty ( item.GetDUQty() );	//CASE QTY
					csv.AddStockAuditQty ( dTotalQty );			//TOTAL QTY
					csv.AddStockAuditVal ( item.GetDUCost() );	//CASE COST
					csv.AddStockAuditVal ( dTotalCost );		//TOTAL COST
					csv.AddStockAuditVal ( 0.0 );				//CASE RETAIL
					csv.AddStockAuditVal ( dLineRetail );		//TOTAL RETAIL
					csv.AddStockAuditVal ( dLineTax );			//TOTAL TAX
					csv.AddStockAuditVal ( dLineWaste );		//TOTAL WASTE
					csv.Add ( dGPPercent, 2 );					//GP PERCENT
					csv.Add ( item.GetSuppRef() );
					csv.Add ( dDUItems, 0 );
					csv.AddStockAuditQty ( dDUItemSize );
					csv.AddStockAuditVal ( dCostTax );

					fileAdhocAudit.WriteLine( csv.GetLine() );
				}
			}
		}
	}
}

/**********************************************************************/

bool CProcessedOrderDatabaseDlg::CanDeliver( int nItemIdx )
{
	if ( nItemIdx < 0 || nItemIdx >= m_OrderArray.GetItemCount() )
		return FALSE;

	CProcessedOrderItem ItemRecord;
	m_OrderArray.GetItemRecord( nItemIdx, ItemRecord );
	
	if ( ItemRecord.GetDeliveredFlag() == TRUE )
		return TRUE;

	int nStockIdx;
	if ( DataManager.Stock.FindStockItemByCode ( ItemRecord.GetStockCode(), nStockIdx ) == FALSE )
		return FALSE;
	
	return TRUE;
}

/**********************************************************************/

void CProcessedOrderDatabaseDlg::OnButtonUndeliverItem() 
{
	if ( UpdateData ( TRUE ) == FALSE )
		return;
	
	int nListSelection = m_listOrder.GetCurSel();
	if ( ( nListSelection < 0 ) || ( nListSelection >= m_OrderFilterArray.GetSize() ) )
		return;

	int nHeaderIdx = m_OrderFilterArray.GetHeaderIndexAt( nListSelection );
	int nItemIdx = m_OrderFilterArray.GetItemIndexAt( nListSelection );
	
	CProcessedOrderItem ItemRecord;
	m_OrderArray.GetItemRecord( nItemIdx, ItemRecord );

	if ( ItemRecord.GetDeliveredFlag() == FALSE )
	{
		Prompter.Error ( "This item has not been delivered." );
		return;
	}

	ToggleDelivery ( nHeaderIdx, nItemIdx );

	//FIND NEXT DELIVERED ITEM
	while ( nListSelection < m_OrderFilterArray.GetSize() - 1 )
	{
		int nItemIdx = m_OrderFilterArray.GetItemIndexAt( nListSelection );

		CProcessedOrderItem ItemRecord;
		m_OrderArray.GetItemRecord ( nItemIdx, ItemRecord );
		if ( ItemRecord.GetDeliveredFlag() == TRUE )
			break;

		nListSelection++;
	}

	DisplayScreen ( nListSelection );
}

/**********************************************************************/

void CProcessedOrderDatabaseDlg::OnButtonUndeliverOrder() 
{
	if ( UpdateData ( TRUE ) == FALSE )
		return;
	
	if ( m_OrderFilterArray.GetSize() == 0 )
		return;

	int nListSelection = m_listOrder.GetCurSel();
	
	CProcessedOrderHeader HeaderRecord;
	CProcessedOrderEntity HeaderEntity;
	if ( GetSelectedHeader( HeaderRecord, HeaderEntity ) == FALSE )
	{	
		Prompter.Error ( "You must select an item from the list\nbefore you can undeliver its order." );
		return;
	}
	
	if ( m_OrderFilterArray.IsFilteredByDescription() == TRUE )
	{
		Prompter.Error ( "You cannot undeliver an entire order when the filter is in use" );
		return;
	}

	CString strMsg;
	strMsg.Format ( "Are you sure you wish to undeliver ALL items\nfrom order number %6.6d.", HeaderEntity.m_nOrderNo );
	if ( Prompter.YesNo ( strMsg ) == IDNO )
		return;

	{
		int nHeaderIdx;
		if ( m_OrderArray.FindHeader( HeaderEntity, nHeaderIdx ) == TRUE )
		{
			int nStartIdx, nEndIdx;
			m_OrderArray.GetItemIdxRange( HeaderEntity, nStartIdx, nEndIdx );

			for ( int nItemIdx = nStartIdx; nItemIdx <= nEndIdx; nItemIdx++ )
			{
				CProcessedOrderItem ItemRecord;
				m_OrderArray.GetItemRecord( nItemIdx, ItemRecord );
	
				if ( ( ItemRecord.GetDeliveredFlag() == TRUE ) && ( ItemRecord.GetDeliveredOldFlag() == FALSE ) )
					ToggleDelivery( nHeaderIdx, nItemIdx );
			}
		}
	}

	//FIND NEXT DELIVERED ORDER
	while ( nListSelection < m_OrderFilterArray.GetSize() - 1 )
	{
		int nHeaderIdx = m_OrderFilterArray.GetHeaderIndexAt( nListSelection );
		int nItemIdx = m_OrderFilterArray.GetItemIndexAt( nListSelection );

		CProcessedOrderEntity HeaderEntityNext;
		m_OrderArray.GetHeaderEntity( nHeaderIdx, HeaderEntityNext );

		CProcessedOrderItem ItemRecord;
		m_OrderArray.GetItemRecord( nItemIdx, ItemRecord );

		if ( ( HeaderEntity.Compare( HeaderEntityNext ) != 0 ) && ( ItemRecord.GetDeliveredFlag() == TRUE ) )
			break;

		nListSelection++;
	}

	DisplayScreen(nListSelection);
}

/**********************************************************************/


