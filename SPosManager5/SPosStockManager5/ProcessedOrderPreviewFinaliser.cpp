/**********************************************************************/
#include <math.h>
#include "DatabaseCSVArray.h"
#include "DatabaseOrderCreator.h"
#include "EmailOptions.h"
#include "FileRemover.h"
#include "NodeTypes.h"
#include "PriceHelpers.h"
#include "ProcessedOrder.h"
#include "ProcessedOrderLog.h"
#include "ProcessedOrderNum.h"
#include "ProcessedOrderReportNewOrder.h"
#include "ProcessedOrderSheetMaker.h"
#include "PropertySheetPrintOrder.h"
#include "RepSpawn.h"
#include "..\CommonEcrStock\SMTPResultsDlg.h"
#include "StatusProgress.h"
#include "StockOptionsIni.h"
#include "StockpointOrderCreator.h"
#include "StockReportNames.h"
/**********************************************************************/
#include "ProcessedOrderPreviewFinaliser.h"
/**********************************************************************/

CProcessedOrderPreviewFinaliser::CProcessedOrderPreviewFinaliser( CWnd* pParentWnd, CLocationSelectorMiniLookup& LocSelMiniLookup, const char* szMonthFrom, const char* szMonthTo ) : m_LocSelMiniLookup( LocSelMiniLookup )
{
	m_pParentWnd = pParentWnd;
	m_strMonthFrom = szMonthFrom;
	m_strMonthTo = szMonthTo;
}

/**********************************************************************/

void CProcessedOrderPreviewFinaliser::RemoveTemporarySalesFiles( bool bCopy )
{
	for ( int nSpIdx = 0; nSpIdx < dbStockpoint.GetSize(); nSpIdx++ )
	{
		__int64 nTotalLength = 0;
		CStringArray FileArray, MonthArray;
		dbStockpoint.GetHistoryFileList( nSpIdx, m_strMonthFrom, m_strMonthTo, FileArray, MonthArray, nTotalLength, FALSE );

		for ( int nFileIdx = 0; nFileIdx < FileArray.GetSize(); nFileIdx++ )
		{
			CString strHistoryFile = FileArray.GetAt( nFileIdx );
			
			CString strTempFile = strHistoryFile;
			strTempFile = strTempFile.Left ( strTempFile.GetLength() - 4 );
			strTempFile += ".tmp";

			if ( ::FileExists ( strTempFile ) )
			{
				if ( bCopy )
				{
					CFileRemover FileRemover ( strHistoryFile );
					rename ( strTempFile, strHistoryFile );
				}
				else
					CFileRemover FileRemover ( strTempFile );
			}
		}
	}
}

/**********************************************************************/

void CProcessedOrderPreviewFinaliser::CreateDatabaseOrders( CProcessedOrderPreviewCSVArray& OrderArray )
{
	CDatabaseOrderCreator OrderCreator( OrderArray );

	bool bPrintPurchase = StockOptions.GetOrderAutoPrintPurchaseFlag();
	bool bEmailPurchase = StockOptions.GetOrderAutoEmailPurchaseFlag();

	CProcessedOrderDetails OrderDetailsPrint ( FALSE );
	CProcessedOrderDetails OrderDetailsEmail ( TRUE );

	//GET ORDER NUMBER FOR NEXT ORDER
	CProcessedOrderNum OrderNum;
	int nThisOrderNum = OrderNum.GetOrderNum();
	
	//LOG EMAIL ERRORS IF REQUIRED
	CBatchEmailLog EmailLog;
	EmailLog.Open ( BATCH_EMAIL_ORDER_SHEET );

	//LOG ORDER GENERATION ERRORS
	CProcessedOrderLog OrderLog;
	OrderLog.Open();

	//REQUEST EXTRA DETAILS FOR PRINTING PURCHASE ORDERS
	if ( TRUE == bPrintPurchase ) 
	{
		CPropertySheetPrintOrder propSheet( FALSE, OrderDetailsPrint, -1, m_pParentWnd );
		
		if ( propSheet.DoModal() == IDCANCEL )
			bPrintPurchase = FALSE;
	}

	//REQUEST EXTRA DETAILS FOR EMAILING PURCHASE ORDERS
	CSMTPEmailHelpers SMTPEmailHelpers;
	if ( TRUE == bEmailPurchase ) 
	{
		CPropertySheetPrintOrder propSheet( TRUE, OrderDetailsEmail, -1, m_pParentWnd );
		
		if (propSheet.DoModal() == IDCANCEL)
		{
			bEmailPurchase = FALSE;
		}
	}

	if ((TRUE == bEmailPurchase) && (EmailOptions.GetEmailMethodToUse() != 0))
	{
		SMTPEmailHelpers.OpenSession(3);
	}

	int nGenerated = 0;
	bool bRepmanWarning = FALSE;

	//MAP THE ORDER START POSITIONS
	int nCurrentSuppIdx = -2;
	CArray<int,int> arrayOrderPos;

	int nSize = OrderCreator.GetOrderSize();
	for ( int n = 0; n < nSize ; n++ )
	{
		CDbOrderCreatorInfo info;
		OrderCreator.GetOrderInfo( n, info );

		if ( info.m_nSuppIdx != nCurrentSuppIdx )
		{
			arrayOrderPos.Add(n);
			nCurrentSuppIdx = info.m_nSuppIdx;
		}
	}
	arrayOrderPos.Add( nSize );

	StatusProgress.Lock( "Generating Orders" );

	for ( int n = 0; n < ( arrayOrderPos.GetSize() - 1 ); n++ )
	{
		int nStart = arrayOrderPos.GetAt(n);
		int nEnd = arrayOrderPos.GetAt(n+1) - 1;

		CDbOrderCreatorInfo info;
		OrderCreator.GetOrderInfo( nStart, info );
		int nSuppIdx = info.m_nSuppIdx;
		int nSuppNo = ( nSuppIdx >= 0 ) ? DataManager.Supplier.GetSuppNo( nSuppIdx ) : 0;

		CProcessedOrder Order;
		int nDbNo = dbDatabase.GetDbNo( DataManager.GetActiveDbIdx() );

		CString strDummy = "";
		Order.Initialise( NODE_DATABASE, nDbNo, 0, nThisOrderNum, nSuppNo, ORDER_TYPE_OPEN, strDummy, strDummy );

		//WORK THROUGH THE INDIVIDUAL LINES FOR THIS ORDER
		for ( int nOrderPos = nStart; nOrderPos <= nEnd; nOrderPos++ )
		{
			StatusProgress.SetPos( nOrderPos, nSize );

			CDbOrderCreatorInfo info;
			OrderCreator.GetOrderInfo( nOrderPos, info );
			int nOrderIdx = info.m_nOrderIdx;

			CProcessedOrderPreviewCSVRecord PreviewRecord;
			OrderArray.GetAt( nOrderIdx, PreviewRecord );

			int nStockIdx = PreviewRecord.GetStockIdx();
			int nStockSuppIdx = PreviewRecord.GetStockSuppIdx();
			
			CStockCSVRecord StockRecord;
			DataManager.Stock.GetAt( nStockIdx, StockRecord );
			
			CProcessedOrderItem item;
			item.SetDate			();
			item.SetStockCode		( StockRecord.GetStockCode() );
			item.SetDescription		( StockRecord.GetDescription() );
			item.SetSuppRef			( StockRecord.GetSuppRefExternal ( nStockSuppIdx, DealerFlags.GetSuppRefFlag() ) );
			item.SetCase			( StockRecord.GetCase ( nStockSuppIdx ) );
			item.SetDUItems			( PreviewRecord.GetDUItems() );
			item.SetDUItemSize		( PreviewRecord.GetDUItemSize() );
			item.SetDUQty			( PreviewRecord.GetDURequired() );
			item.SetDUCost			( StockRecord.GetCost ( nStockSuppIdx ) );
			item.SetStockIdx		( -1 );
	
			item.ClearDeliveredFlag();
	
			for ( int nSpIdx = 0; nSpIdx < PreviewRecord.GetStockpointCount(); nSpIdx++ )
			{
				if ( CProcessedOrderPreviewCSVArray::IsStockpointIncluded( nSpIdx, m_LocSelMiniLookup, FALSE ) )
				{
					CStockLevelsCSVRecord StockLevels;
					int nSpNo = dbStockpoint.GetSpNo( nSpIdx );
					DataManager.Stock.GetStockpointTotals( nStockIdx, StockRecord, nSpNo, StockLevels );
					
					if ( StockLevels.GetActiveFlag() == TRUE )
					{
						int nDUItems;
						if ( StockOptions.GetOrderSeparationType() != ORDER_DUSEPARATE_NONE )
							nDUItems = PreviewRecord.CalculateDURequired( nSpIdx ) * PreviewRecord.GetDUItems();
						else
							nDUItems = PreviewRecord.CalculateDUItemRequired( nSpIdx );

						item.AddOrder( nSpNo, nDUItems );

						StockLevels.AddOrder ( PreviewRecord.GetDUItemSize() * nDUItems );
						DataManager.Stock.SetStockpointTotals( nStockIdx, StockRecord, nSpNo, StockLevels );
					}
				}
			}

			Order.AddItem( item );
			nGenerated++;
		}

		Order.WriteOrder();
		
		//PRINT OUT THE ORDER REPORT
		if ( StockOptions.GetOrderAutoPrintOrderFlag() == TRUE )
		{
			CProcessedOrderReportNewOrder NewOrderReport ( Order );

			NewOrderReport.SetReportType ( ORDER_ONORDER_SUPPLIER );
			NewOrderReport.SetConLevel ( NODE_DATABASE );
			NewOrderReport.SetTableNo ( dbDatabase.GetDbNo( DataManager.GetActiveDbIdx() ) );
			NewOrderReport.SetConLevelText ( dbDatabase.GetName( DataManager.GetActiveDbIdx() ) );
			
			if ( NewOrderReport.CreateReport( DataManager.GetActiveDbIdx() ) )
			{
				CReportManager ReportManager;
				bRepmanWarning |= ( ReportManager.PrintSilent ( Super.ReportFilename(), NewOrderReport.GetReportParamsFilename( ORDER_ONORDER_SUPPLIER ), "", m_pParentWnd ) == FALSE );
			}
		}

		//PRINT OUT THE PURCHASE ORDER
		if ( TRUE == bPrintPurchase )
		{
			CProcessedOrderSheetMaker SheetMaker;
			SheetMaker.SetDate();
			SheetMaker.SetDeliverAsOrderedFlag ( OrderDetailsPrint.m_bDeliverAsOrdered );
			SheetMaker.SetTopAddress1 ( OrderDetailsPrint.m_strTopAddress1 );
			SheetMaker.SetTopAddress2 ( OrderDetailsPrint.m_strTopAddress2 );
			SheetMaker.SetTopAddress3 ( OrderDetailsPrint.m_strTopAddress3 );
			SheetMaker.SetTopPostcode ( OrderDetailsPrint.m_strTopPostcode );
			SheetMaker.SetTopFax ( OrderDetailsPrint.m_strTopFax );
			SheetMaker.SetTopName ( OrderDetailsPrint.m_strTopName );
			SheetMaker.SetTopPhone ( OrderDetailsPrint.m_strTopPhone );
			SheetMaker.SetName ( OrderDetailsPrint.m_strName );
			SheetMaker.SetAddress1 ( OrderDetailsPrint.m_strAddress1 );
			SheetMaker.SetAddress2 ( OrderDetailsPrint.m_strAddress2 );
			SheetMaker.SetAddress3 ( OrderDetailsPrint.m_strAddress3 );
			SheetMaker.SetPostcode ( OrderDetailsPrint.m_strPostcode );
			SheetMaker.SetInstructions1 ( OrderDetailsPrint.m_strInstructions1 );
			SheetMaker.SetInstructions2 ( OrderDetailsPrint.m_strInstructions2 );
			SheetMaker.SetInstructions3 ( OrderDetailsPrint.m_strInstructions3 );
			SheetMaker.SetInstructions4 ( OrderDetailsPrint.m_strInstructions4 );
			SheetMaker.SetInstructions5 ( OrderDetailsPrint.m_strInstructions5 );
			SheetMaker.CreateOrderSheet ( Order, TRUE );	
			bRepmanWarning |= SheetMaker.GetRepmanWarningFlag();
		}
	
		//EMAIL THE PURCHASE ORDER
		if ( ( TRUE == bEmailPurchase ) && ( nSuppIdx >= 0 ) )
		{
			CProcessedOrderSheetMaker SheetMaker;
			SheetMaker.SetDate();
			SheetMaker.SetDeliverAsOrderedFlag ( OrderDetailsEmail.m_bDeliverAsOrdered );
			SheetMaker.SetTopAddress1 ( OrderDetailsEmail.m_strTopAddress1 );
			SheetMaker.SetTopAddress2 ( OrderDetailsEmail.m_strTopAddress2 );
			SheetMaker.SetTopAddress3 ( OrderDetailsEmail.m_strTopAddress3 );
			SheetMaker.SetTopPostcode ( OrderDetailsEmail.m_strTopPostcode );
			SheetMaker.SetTopFax ( OrderDetailsEmail.m_strTopFax );
			SheetMaker.SetTopName ( OrderDetailsEmail.m_strTopName );
			SheetMaker.SetTopPhone ( OrderDetailsEmail.m_strTopPhone );
			SheetMaker.SetName ( OrderDetailsEmail.m_strName );
			SheetMaker.SetAddress1 ( OrderDetailsEmail.m_strAddress1 );
			SheetMaker.SetAddress2 ( OrderDetailsEmail.m_strAddress2 );
			SheetMaker.SetAddress3 ( OrderDetailsEmail.m_strAddress3 );
			SheetMaker.SetPostcode ( OrderDetailsEmail.m_strPostcode );
			SheetMaker.SetInstructions1 ( OrderDetailsEmail.m_strInstructions1 );
			SheetMaker.SetInstructions2 ( OrderDetailsEmail.m_strInstructions2 );
			SheetMaker.SetInstructions3 ( OrderDetailsEmail.m_strInstructions3 );
			SheetMaker.SetInstructions4 ( OrderDetailsEmail.m_strInstructions4 );
			SheetMaker.SetInstructions5 ( OrderDetailsEmail.m_strInstructions5 );
			SheetMaker.SetEmailLog ( &EmailLog );

			if (EmailOptions.GetEmailMethodToUse() == 0)
			{
				SheetMaker.EmailOrderSheetMAPI(Order, TRUE, TRUE);
			}
			else
			{
				SheetMaker.EmailOrderSheetSMTP(SMTPEmailHelpers, Order);
			}

			m_pParentWnd -> BringWindowToTop();
		}

		OrderNum.NextOrder();
		nThisOrderNum = OrderNum.GetOrderNum();
	}
	StatusProgress.Unlock();
	
	CDataManagerInfo info;
	if ( DataManager.WriteStock( info, TRUE, TRUE ) == FALSE )
		Prompter.WriteError( info );

	if ( m_strMonthFrom != "" )
		RemoveTemporarySalesFiles( TRUE );

	EmailLog.Close();
	OrderLog.Close();

	if ( EmailLog.GetLineCount() > 0 )
	{
		CString strMsg = "";
		strMsg += "One of more of the generated orders was not emailed because the email attempt failed.\n\n";
		strMsg += "Please click OK to see a summary of email errors.";
		
		Prompter.Info( strMsg );
		
		CReportManager ReportManager;
		if ( ReportManager.DisplaySilent ( Super.EmailLog(), "", "", m_pParentWnd ) == FALSE )
			bRepmanWarning = TRUE;
	}

	if ( SMTPEmailHelpers.GetSessionSize() > 0 )
	{
		SMTPEmailHelpers.CreateIndexFile();
		CSMTPResultsDlg dlg( 3, SMTPEmailHelpers, m_pParentWnd );
		dlg.DoModal();
		SMTPEmailHelpers.DeleteSession();
	}

	if ( OrderLog.GetLineCount() > 0 )
	{
		CString strMsg = "";
		strMsg += "Orders could not be generated for one or more stockpoints because the order files are in use.\n\n";
		strMsg += "Please click OK to see a list of the stockpoints for which orders caould not be generated.";
		
		Prompter.Info( strMsg );
		
		CReportManager ReportManager;
		if ( ReportManager.DisplaySilent ( Super.OrderLog(), "", "", m_pParentWnd ) == FALSE )
			bRepmanWarning = TRUE;
	}
	
	if ( TRUE == bRepmanWarning )
	{
		CReportManager ReportManager;
		ReportManager.ReportManagerError();
	}

	CString strMsg;
	switch ( nGenerated )
	{
	case 0:		strMsg = "No order lines were generated";					break;
	case 1:		strMsg = "1 order line was generated";						break;
	default:	strMsg.Format ( "%d order lines were generated", nGenerated );	break;
	}
	Prompter.Info ( strMsg );
}

/**********************************************************************/

void CProcessedOrderPreviewFinaliser::CreateStockpointOrders( CProcessedOrderPreviewCSVArray& OrderArray )
{
	CStockpointOrderCreator OrderCreator( OrderArray );

	bool bPrintPurchase = StockOptions.GetOrderAutoPrintPurchaseFlag();
	bool bEmailPurchase = StockOptions.GetOrderAutoEmailPurchaseFlag();

	CProcessedOrderDetails OrderDetailsPrint ( FALSE );
	CProcessedOrderDetails OrderDetailsEmail ( TRUE );

	//GET ORDER NUMBER FOR NEXT ORDER
	CProcessedOrderNum OrderNum;
	int nThisOrderNum = OrderNum.GetOrderNum();

	//LOG EMAIL ERRORS IF REQUIRED
	CBatchEmailLog EmailLog;
	EmailLog.Open ( BATCH_EMAIL_ORDER_SHEET );

	CSMTPEmailHelpers SMTPEmailHelpers;
	if ((TRUE == bEmailPurchase) && (EmailOptions.GetEmailMethodToUse() != 0))
	{
		SMTPEmailHelpers.OpenSession(3);
	}

	//LOG ORDER GENERATION ERRORS
	CProcessedOrderLog OrderLog;
	OrderLog.Open();

	int nGenerated = 0;
	bool bRepmanWarning = FALSE;

	//MAP THE ORDER START POSITIONS
	int nCurrentSpIdx = -1;
	int nCurrentSuppIdx = -1;
	CArray<int,int> arrayOrderPos;

	int nSize = OrderCreator.GetOrderSize();
	for ( int n = 0; n < nSize ; n++ )
	{
		CSpOrderCreatorInfo info;
		OrderCreator.GetOrderInfo( n, info );

		if ( ( info.m_nSpIdx != nCurrentSpIdx ) || ( info.m_nSuppIdx != nCurrentSuppIdx ) )
		{
			arrayOrderPos.Add(n);
			nCurrentSpIdx = info.m_nSpIdx;
			nCurrentSuppIdx = info.m_nSuppIdx;
		}
	}
	arrayOrderPos.Add( nSize );

	StatusProgress.Lock( TRUE, "Generating orders" );
	for ( int n = 0; n < ( arrayOrderPos.GetSize() - 1 ); n++ )
	{
		int nStart = arrayOrderPos.GetAt(n);
		int nEnd = arrayOrderPos.GetAt(n+1) - 1;

		CSpOrderCreatorInfo info;
		OrderCreator.GetOrderInfo( nStart, info );
		int nSuppIdx = info.m_nSuppIdx;
		int nSuppNo = ( nSuppIdx >= 0 ) ? DataManager.Supplier.GetSuppNo( nSuppIdx ) : 0;
		int nSpIdx = info.m_nSpIdx;

		CProcessedOrder Order;
		int nDbNo = dbStockpoint.GetDbNo( nSpIdx );
		int nSpNo = dbStockpoint.GetSpNo( nSpIdx );

		CString strDummy = "";
		Order.Initialise( NODE_STOCKPOINT, nDbNo, nSpNo, nThisOrderNum, nSuppNo, ORDER_TYPE_OPEN, strDummy, strDummy );
		
		//WORK THROUGH THE INDIVIDUAL LINES FOR THIS ORDER
		for ( int nOrderPos = nStart; nOrderPos <= nEnd; nOrderPos++ )
		{
			StatusProgress.SetPos( nOrderPos, nSize );

			CSpOrderCreatorInfo info;
			OrderCreator.GetOrderInfo( nOrderPos, info );
			
			int nStockIdx = info.m_nStockIdx;
			int nStockSuppIdx = info.m_nStockSuppIdx;
			int nDUItems = info.m_nDUItems;
			double dDUItemSize = info.m_dDUItemSize;
			double dSpOrder = info.m_dSpOrder;

			CStockCSVRecord StockRecord;
			DataManager.Stock.GetAt( nStockIdx, StockRecord );
			
			double dDUYield = dDUItemSize * nDUItems;
			int nDUQty = int ( floor ( dSpOrder / dDUYield ) );

			if ( CPriceHelpers::CompareDoubles( dSpOrder, ( dDUYield * nDUQty ), 3 ) == 1 )
				nDUQty++;

			CProcessedOrderItem item;
			item.SetDate();
			item.SetStockCode( StockRecord.GetStockCode() );
			item.SetDescription( StockRecord.GetDescription() );
			item.SetSuppRef( StockRecord.GetSuppRefExternal ( nStockSuppIdx, DealerFlags.GetSuppRefFlag() ) );
			item.SetCase( StockRecord.GetCase( nStockSuppIdx ) );
			item.SetDUItems( nDUItems );
			item.SetDUItemSize( dDUItemSize );
			item.SetDUQty( nDUQty );
			item.SetDUCost( StockRecord.GetCost( info.m_nStockSuppIdx ) );
			item.ClearDeliveredFlag();
			item.SetStockIdx(-1);
			
			int nSpNo = dbStockpoint.GetSpNo( info.m_nSpIdx );
			item.AddOrder( nSpNo, nDUQty * nDUItems );
			
			Order.AddItem( item );
			
			nGenerated++;

			CStockLevelsCSVRecord StockLevels;
			DataManager.Stock.GetStockpointTotals( nStockIdx, StockRecord, nSpNo, StockLevels );
			StockLevels.AddOrder ( dDUYield * nDUQty );
			DataManager.Stock.SetStockpointTotals( nStockIdx, StockRecord, nSpNo, StockLevels );
		}

		Order.WriteOrder();

		//PRINT OUT THE ORDER REPORT
		if ( StockOptions.GetOrderAutoPrintOrderFlag() == TRUE )
		{
			CProcessedOrderReportNewOrder NewOrderReport( Order );

			NewOrderReport.SetReportType ( ORDER_ONORDER_SUPPLIER );
			NewOrderReport.SetConLevel ( NODE_STOCKPOINT );
			NewOrderReport.SetTableNo ( dbStockpoint.GetSpNo( nSpIdx ) );
			NewOrderReport.SetConLevelText ( dbStockpoint.GetName( nSpIdx ) );
			
			if ( NewOrderReport.CreateReport( DataManager.GetActiveDbIdx() ) )
			{
				CReportManager ReportManager;
				bRepmanWarning |= ( ReportManager.PrintSilent ( Super.ReportFilename(), NewOrderReport.GetReportParamsFilename( ORDER_ONORDER_SUPPLIER ), "", m_pParentWnd ) == FALSE );
			}
		}

		//PRINT OUT THE PURCHASE ORDER
		if ( TRUE == bPrintPurchase )
		{
			CAddressInfo infoAddressDb;
			int nDbIdx = DataManager.GetActiveDbIdx();
			dbDatabase.GetAddressInfo( nDbIdx, infoAddressDb );

			CAddressInfo infoAddressSp;
			dbStockpoint.GetAddressInfo( nSpIdx, infoAddressSp );

			CProcessedOrderSheetMaker SheetMaker;
			
			SheetMaker.SetDate();
			SheetMaker.SetDeliverAsOrderedFlag ( FALSE );
			
			SheetMaker.SetTopName( dbDatabase.GetName( nDbIdx ) );
			SheetMaker.SetTopAddress1( infoAddressDb.m_strAddress1 );
			SheetMaker.SetTopAddress2( infoAddressDb.m_strAddress2 );
			SheetMaker.SetTopAddress3( infoAddressDb.m_strAddress3 );
			SheetMaker.SetTopPostcode( infoAddressDb.m_strPostcode );
			SheetMaker.SetTopPhone( infoAddressDb.m_strPhone );
			SheetMaker.SetTopFax( infoAddressDb.m_strFax );
			
			SheetMaker.SetName( dbStockpoint.GetName( nSpIdx ) );
			SheetMaker.SetAddress1( infoAddressSp.m_strAddress1 );
			SheetMaker.SetAddress2( infoAddressSp.m_strAddress2 );
			SheetMaker.SetAddress3( infoAddressSp.m_strAddress3 );
			SheetMaker.SetPostcode( infoAddressSp.m_strPostcode );
			
			SheetMaker.SetInstructions1 ( "" );
			SheetMaker.SetInstructions2 ( "" );
			SheetMaker.SetInstructions3 ( "" );
			SheetMaker.SetInstructions4 ( "" );
			SheetMaker.SetInstructions5 ( "" );
			
			SheetMaker.CreateOrderSheet ( Order, TRUE );
			bRepmanWarning |= SheetMaker.GetRepmanWarningFlag();
		}
	
		//EMAIL THE PURCHASE ORDER
		if ( ( TRUE == bEmailPurchase ) && ( nSuppIdx >= 0 ) )
		{
			CAddressInfo infoAddressDb;
			int nDbIdx = DataManager.GetActiveDbIdx();
			dbDatabase.GetAddressInfo( nDbIdx, infoAddressDb );

			CAddressInfo infoAddressSp;
			dbStockpoint.GetAddressInfo( nSpIdx, infoAddressSp );

			CProcessedOrderSheetMaker SheetMaker;
			
			SheetMaker.SetDate();
			SheetMaker.SetDeliverAsOrderedFlag ( FALSE );
			
			SheetMaker.SetTopName( dbDatabase.GetName( nDbIdx ) );
			SheetMaker.SetTopAddress1( infoAddressDb.m_strAddress1 );
			SheetMaker.SetTopAddress2( infoAddressDb.m_strAddress2 );
			SheetMaker.SetTopAddress3( infoAddressDb.m_strAddress3 );
			SheetMaker.SetTopPostcode( infoAddressDb.m_strPostcode );
			SheetMaker.SetTopPhone( infoAddressDb.m_strPhone );
			SheetMaker.SetTopFax( infoAddressDb.m_strFax );
			
			SheetMaker.SetName( dbStockpoint.GetName( nSpIdx ) );
			SheetMaker.SetAddress1( infoAddressSp.m_strAddress1 );
			SheetMaker.SetAddress2( infoAddressSp.m_strAddress2 );
			SheetMaker.SetAddress3( infoAddressSp.m_strAddress3 );
			SheetMaker.SetPostcode( infoAddressSp.m_strPostcode );
			
			SheetMaker.SetInstructions1 ( "" );
			SheetMaker.SetInstructions2 ( "" );
			SheetMaker.SetInstructions3 ( "" );
			SheetMaker.SetInstructions4 ( "" );
			SheetMaker.SetInstructions5 ( "" );
			
			SheetMaker.SetEmailLog ( &EmailLog );

			if (EmailOptions.GetEmailMethodToUse() == 0)
			{
				SheetMaker.EmailOrderSheetMAPI(Order, TRUE, TRUE);
			}
			else
			{
				SheetMaker.EmailOrderSheetSMTP(SMTPEmailHelpers, Order);
			}

			m_pParentWnd -> BringWindowToTop();
		}

		OrderNum.NextOrder();
		nThisOrderNum = OrderNum.GetOrderNum();
	}
	StatusProgress.Unlock();

	CDataManagerInfo infoDM;
	if ( DataManager.WriteStock( infoDM, TRUE, TRUE ) == FALSE )
		Prompter.WriteError( infoDM );

	if ( m_strMonthFrom != "" )
		RemoveTemporarySalesFiles( TRUE );

	EmailLog.Close();
	OrderLog.Close();

	if ( EmailLog.GetLineCount() > 0 )
	{
		CString strMsg = "";
		strMsg += "One of more of the generated orders was not emailed because the email attempt failed.\n\n";
		strMsg += "Please click OK to see a summary of email errors.";
		
		Prompter.Info( strMsg );
		
		CReportManager ReportManager;
		if ( ReportManager.DisplaySilent ( Super.EmailLog(), "", "", m_pParentWnd ) == FALSE )
			bRepmanWarning = TRUE;
	}

	if ( SMTPEmailHelpers.GetSessionSize() > 0 )
	{
		SMTPEmailHelpers.CreateIndexFile();
		CSMTPResultsDlg dlg( 3, SMTPEmailHelpers, m_pParentWnd );
		dlg.DoModal();
		SMTPEmailHelpers.DeleteSession();
	}

	if ( OrderLog.GetLineCount() > 0 )
	{
		CString strMsg = "";
		strMsg += "Orders could not be generated for one or more stockpoints because the order files are in use.\n\n";
		strMsg += "Please click OK to see a list of the stockpoints for which orders caould not be generated.";
		
		Prompter.Info( strMsg );
		
		CReportManager ReportManager;
		if ( ReportManager.DisplaySilent ( Super.OrderLog(), "", "", m_pParentWnd ) == FALSE )
			bRepmanWarning = TRUE;
	}

	if ( TRUE == bRepmanWarning )
	{
		CReportManager ReportManager;
		ReportManager.ReportManagerError();
	}
		
	CString strMsg;	
	switch ( nGenerated )
	{
	case 0:		strMsg = "No order lines were generated";					break;
	case 1:		strMsg = "1 order line was generated";						break;
	default:	strMsg.Format ( "%d order lines were generated", nGenerated );	break;
	}

	Prompter.Info ( strMsg );
}

/**********************************************************************/
