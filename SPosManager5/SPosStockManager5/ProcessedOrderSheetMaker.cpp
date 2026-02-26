/**********************************************************************/
#include "CCSVReportLine.h"
#include "CustomerStatementErrorInfo.h"
//#include "DataManager.h"
//#include "DealerFlags.h"
#include "EmailOptions.h"
#include "Repspawn.h"
#include "SMTPEmailHelpers.h"
/**********************************************************************/
#include "ProcessedOrderSheetMaker.h"
/**********************************************************************/

CProcessedOrderSheetMaker::CProcessedOrderSheetMaker()
{
	m_strName = "";
	m_strAddress1 = "";
	m_strAddress2 = "";
	m_strAddress3 = "";
	m_strPostcode = "";
	m_strTopName = "";
	m_strTopAddress1 = "";
	m_strTopAddress2 = "";
	m_strTopAddress3 = "";
	m_strTopPostcode = "";
	m_strTopPhone = "";
	m_strTopFax = "";
	m_strInstructions1 = "";
	m_strInstructions2 = "";
	m_strInstructions3 = "";
	m_strInstructions4 = "";
	m_strInstructions5 = "";
	m_strDate = "01/01/2000";
	m_bDeliverAsOrdered = TRUE;
	m_pEmailLog = NULL;
	m_bRepmanWarning = FALSE;
}

/**********************************************************************/

void CProcessedOrderSheetMaker::SetTopPhone ( const char* szPhone )
{
	m_strTopPhone = szPhone;

	if ( m_strTopPhone != "" )
	{
		CString strTemp = "T : ";
		strTemp += m_strTopPhone;
		m_strTopPhone = strTemp;
	}
}

/**********************************************************************/

void CProcessedOrderSheetMaker::SetTopFax ( const char* szFax )
{
	m_strTopFax = szFax;

	if ( m_strTopFax != "" )
	{
		CString strTemp = "F : ";
		strTemp += m_strTopFax;
		m_strTopFax = strTemp;
	}
}

/**********************************************************************/

void CProcessedOrderSheetMaker::SetDate()
{
	CTime time ( CTime::GetCurrentTime() );
	m_strDate = time.Format ( "%d/%m/%Y" );
}

/**********************************************************************/

void CProcessedOrderSheetMaker::CreateOrderSheet ( CProcessedOrder& Order, bool bPrint, bool bEmail )
{
	CProcessedOrderHeader header;
	Order.GetHeader( header );
	int nSuppNo = header.GetSuppNo();
	int nOrderNum = header.GetOrderNum();

	int nSuppIdx;
	if ( DataManager.Supplier.FindSupplierByNumber( nSuppNo, nSuppIdx ) == FALSE )
		nSuppIdx = -1;

	m_bRepmanWarning = FALSE;
	CString strFullLine = "<..>";

	CReportFile ReportFile;
	if ( ReportFile.Open ( Super.ReportFilename() ) == TRUE )
	{
		CString strSuppName = "";
		CString strSuppAddress1 = "";
		CString strSuppAddress2 = "";
		CString strSuppAddress3 = "";
		CString strSuppAddress4 = "";
		CString strSuppAddress5 = "";
		CString strSuppPostcode = "";
		CString strSuppFAO = "";
		CString strSuppPhone = "";
		CString strSuppFax = "";
		CString strAccountNo = "";
		
		if ( nSuppIdx != -1 )
		{
			CSupplierCSVRecord SupplierRecord;
			DataManager.Supplier.GetAt ( nSuppIdx, SupplierRecord );
			strSuppName = SupplierRecord.GetName();
			strSuppAddress1 = SupplierRecord.GetAddress1();
			strSuppAddress2 = SupplierRecord.GetAddress2();
			strSuppAddress3 = SupplierRecord.GetAddress3();
			strSuppAddress4 = SupplierRecord.GetAddress4();
			strSuppAddress5 = SupplierRecord.GetAddress5();
			strSuppPostcode = SupplierRecord.GetPostcode();
			strSuppPhone = SupplierRecord.GetPhone();
			strSuppFax = SupplierRecord.GetFax();
			strAccountNo = SupplierRecord.GetAccountNo();

			CString strFAO = SupplierRecord.GetFAO();
			if ( strFAO != "" )
			{
				strSuppFAO = "FAO : ";
				strSuppFAO += strFAO;
			}

			CString strPhone = SupplierRecord.GetPhone();
			if ( strPhone != "" )
			{
				strSuppPhone = "T : ";
				strSuppPhone += strPhone;
			}

			CString strFax = SupplierRecord.GetFax();
			if ( strFax != "" )
			{
				strSuppFax = "F : ";
				strSuppFax += strFax;
			}
		}

		if ( bEmail == FALSE )
			ReportFile.SetStyle1 ( "Order Sheet" );
		else
			ReportFile.SetStyle1 ( "" );

		ReportFile.AddColumn ( "", TA_LEFT, 200 );		
		ReportFile.AddColumn ( "", TA_LEFT, 200 );		
		ReportFile.AddColumn ( "", TA_LEFT, 350 );
		ReportFile.AddColumn ( "", TA_LEFT, 450 );
		ReportFile.AddColumn ( "", TA_LEFT, 200 );
		ReportFile.AddColumn ( "", TA_RIGHT, 250 );
		ReportFile.AddColumn ( "", TA_RIGHT, 250 );
		
		CStockReportLine csv;
		csv.AppendBlanks ( 4 );
		csv.AppendString ( strFullLine + m_strTopName );
		ReportFile.WriteLine ( csv.GetLine() );

		csv.RemoveAll();
		csv.AppendBlanks ( 4 );
		csv.AppendString ( strFullLine + m_strTopAddress1 );
		ReportFile.WriteLine ( csv.GetLine() );

		csv.RemoveAll();
		csv.AppendBlanks(4);
		csv.AppendString ( strFullLine + m_strTopAddress2 );
		ReportFile.WriteLine ( csv.GetLine() );

		csv.RemoveAll();
		csv.AppendBlanks ( 4 );
		csv.AppendString ( strFullLine + m_strTopAddress3 );
		ReportFile.WriteLine ( csv.GetLine() );

		csv.RemoveAll();
		csv.AppendBlanks(4);
		csv.AppendString( strFullLine + m_strTopPostcode );
		ReportFile.WriteLine ( csv.GetLine() );

		csv.RemoveAll();
		csv.AppendBlanks ( 4 );
		csv.AppendString ( strFullLine + m_strTopPhone );
		ReportFile.WriteLine ( csv.GetLine() );

		ReportFile.WriteLine ( "" );

		CTime time = CTime::GetCurrentTime();
		CString strTime = time.Format ( "%A %B %d %Y" );
		ReportFile.WriteLine ( strFullLine + strTime );

		ReportFile.WriteLine ( "" );
		ReportFile.WriteLine ( strFullLine + strSuppName );
		ReportFile.WriteLine ( strFullLine + strSuppAddress1 );
		ReportFile.WriteLine ( strFullLine + strSuppAddress2 );
		ReportFile.WriteLine ( strFullLine + strSuppAddress3 );

		if ( strSuppAddress4 != "" )
			ReportFile.WriteLine ( strFullLine + strSuppAddress4 );

		if ( strSuppAddress5 != "" )
			ReportFile.WriteLine ( strFullLine + strSuppAddress5 );

		ReportFile.WriteLine ( strFullLine + strSuppPostcode );

		if ( strSuppFAO != "" )
		{
			ReportFile.WriteLine ( "" );
			ReportFile.WriteLine ( strFullLine + strSuppFAO );
		}

		if ( ( strSuppPhone != "" ) || ( strSuppFax != "" ) )
		{
			ReportFile.WriteLine( "" );
			
			if ( strSuppPhone != "" )
				ReportFile.WriteLine( strFullLine + strSuppPhone );

			if ( strSuppFax != "" )
				ReportFile.WriteLine( strFullLine + strSuppFax );
		}

		ReportFile.WriteLine ( "" );
		ReportFile.WriteLine ( "" );

		CString strOrder;
		strOrder.Format ( "<..>PURCHASE ORDER No. %6.6d", nOrderNum );
		ReportFile.WriteLine ( strOrder );

		if ( strAccountNo != "" )
		{
			CString strAcNo;
			strAcNo.Format ( "<..>ACCOUNT No. %s", ( const char* ) strAccountNo );
			ReportFile.WriteLine ( "" );
			ReportFile.WriteLine ( strAcNo );
		}

		ReportFile.WriteLine ( "" );
		ReportFile.WriteLine ( "<LI>" );
		ReportFile.WriteLine ( "Qty\tYield\tStockCode\tDescription\t\tUnit Price\tTotal" );
		ReportFile.WriteLine ( "<LI>" );
		
		double dTotal = 0.0;

		for ( int nItemIdx = 0; nItemIdx < Order.GetItemCount(); nItemIdx++ )
		{
			CProcessedOrderItem item;
			Order.GetItem( nItemIdx, item );

			CStockReportLine csv;
			csv.AppendInt ( item.GetDUQty() );

			CString strItem;
			strItem.Format ( "%d x %.3f", item.GetDUItems(), item.GetDUItemSize() );
			csv.AppendString ( strItem );

			csv.AppendString ( item.GetSuppRef() );
	
			int nStockIdx;
			if ( DataManager.Stock.FindStockItemByCode ( item.GetStockCode(), nStockIdx ) == TRUE )
			{
				CStockCSVRecord StockRecord;
				DataManager.Stock.GetAt ( nStockIdx, StockRecord );
				CString strDesc = "<..>";
				strDesc += StockRecord.GetDescription();
				csv.AppendString ( strDesc );
				csv.AppendString ( "" );
			}
			else
				csv.AppendBlanks ( 2 );

			csv.AppendVal ( item.GetDUCost() );

			double dThisOrder = item.GetDUQty() * item.GetDUCost();
			csv.AppendVal ( dThisOrder );
			dTotal += dThisOrder;

			ReportFile.WriteLine ( csv.GetLine() );

			if ( DealerFlags.GetPurchaseOrderLinesFlag() == TRUE )
				ReportFile.WriteLine ( "<LI>" );
		}

		if ( DealerFlags.GetPurchaseOrderLinesFlag() == FALSE )
			ReportFile.WriteLine ( "\t\t\t\t\t\t<LI>" );

		csv.RemoveAll();
		csv.AppendString ( "Total" );
		csv.AppendBlanks ( 5 );
		csv.AppendVal ( dTotal );
		ReportFile.WriteLine ( csv.GetLine() );

		ReportFile.WriteLine ( "" );
		ReportFile.WriteLine ( "" );
		
		if ( m_strInstructions1 != "" )
		{
			csv.RemoveAll();
			csv.AppendBlanks(3);
			csv.AppendString ( strFullLine + m_strInstructions1 );
			ReportFile.WriteLine ( csv.GetLine() );
		}

		if ( m_strInstructions2 != "" )
		{
			csv.RemoveAll();
			csv.AppendBlanks(3);
			csv.AppendString ( strFullLine + m_strInstructions2 );
			ReportFile.WriteLine ( csv.GetLine() );
		}

		if ( m_strInstructions3 != "" )
		{
			csv.RemoveAll();
			csv.AppendBlanks(3);
			csv.AppendString ( strFullLine + m_strInstructions3 );
			ReportFile.WriteLine ( csv.GetLine() );
		}

		if ( m_strInstructions4 != "" )
		{
			csv.RemoveAll();
			csv.AppendBlanks(3);
			csv.AppendString ( strFullLine + m_strInstructions4 );
			ReportFile.WriteLine ( csv.GetLine() );
		}

		if ( m_strInstructions5 != "" )
		{
			csv.RemoveAll();
			csv.AppendBlanks(3);
			csv.AppendString ( strFullLine + m_strInstructions5 );
			ReportFile.WriteLine ( csv.GetLine() );
		}

		if ( m_bDeliverAsOrdered == FALSE )
		{
			ReportFile.WriteLine ( "<..>DELIVERY ADDRESS" );
			ReportFile.WriteLine ( "" );
			ReportFile.WriteLine ( strFullLine + m_strName );
			ReportFile.WriteLine ( strFullLine + m_strAddress1 );
			ReportFile.WriteLine ( strFullLine + m_strAddress2 );
			ReportFile.WriteLine ( strFullLine + m_strAddress3 );
			ReportFile.WriteLine ( strFullLine + m_strPostcode );
		}
		ReportFile.Close();

		CFilenameUpdater FnUp ( SysFiles::OrderPrm );
		
		if ( bEmail == FALSE )
		{
			CReportManager ReportManager;

			if ( bPrint == TRUE )
			{
				if ( ReportManager.PrintSilent ( Super.ReportFilename(), FnUp.GetFilenameToUse(), "", NULL ) == FALSE )
					m_bRepmanWarning = TRUE;
			}
			else
			{
				if ( ReportManager.DisplaySilent ( Super.ReportFilename(), FnUp.GetFilenameToUse(), "", NULL ) == FALSE )
					m_bRepmanWarning = TRUE;
			}
		}
	}
}

/**********************************************************************/

void CProcessedOrderSheetMaker::EmailOrderSheetMAPI ( CProcessedOrder& Order, bool bSend, bool bEmailLog )
{
	CProcessedOrderHeader header;
	Order.GetHeader( header );
	int nSuppNo = header.GetSuppNo();
	int nOrderNum = header.GetOrderNum();

	int nSuppIdx;
	if ( DataManager.Supplier.FindSupplierByNumber( nSuppNo, nSuppIdx ) == FALSE )
		return;

	CSupplierCSVRecord SupplierRecord;
	DataManager.Supplier.GetAt ( nSuppIdx, SupplierRecord );
	CString strSupplierName = SupplierRecord.GetName();
	CString strEmailAddress = SupplierRecord.GetEmail();
	
	CreateOrderSheet( Order, FALSE, TRUE );
	CReportFileDoc ReportFileDoc ( Super.ReportFilename() );
	ReportFileDoc.SaveAsHTML( Super.HTMLFilename() );

	char cBuffer [ _MAX_PATH ];
	cBuffer[0] = 0;
	strcat_s ( cBuffer, Super.HTMLFilename() );

	CSSMapi32 EmailHandler;
	bool bResult = EmailHandler.SendMessage( strEmailAddress, "Purchase Order", cBuffer, bSend );

	if ( FALSE == bResult )
	{
		CString strError = EmailHandler.GetError();
			
		if ( ( FALSE == bSend ) && ( FALSE == bEmailLog ) )
			Prompter.Error ( strError );
		else
		{
			if ( m_pEmailLog != NULL )
			{
				CString strNum;
				strNum.Format ( "%6.6d", nOrderNum );

				CCSV csv ( '\t' );
				csv.Add ( strSupplierName );
				csv.Add ( strNum );
				csv.Add ( strEmailAddress );
				csv.Add ( strError );
				m_pEmailLog -> AddLine ( csv.GetLine() );
			}
		}		
	}
}

/**********************************************************************/

void CProcessedOrderSheetMaker::EmailOrderSheetSMTP ( CSMTPEmailHelpers& SMTPEmailHelpers, CProcessedOrder& Order )
{
	CProcessedOrderHeader header;
	Order.GetHeader( header );
	int nSuppNo = header.GetSuppNo();
	int nOrderNum = header.GetOrderNum();

	int nSuppIdx;
	if ( DataManager.Supplier.FindSupplierByNumber( nSuppNo, nSuppIdx ) == FALSE )
		return;

	CSupplierCSVRecord SupplierRecord;
	DataManager.Supplier.GetAt ( nSuppIdx, SupplierRecord );

	CCSV csvFriendlyText;
	csvFriendlyText.Add( "Purchase Order" );
	csvFriendlyText.Add( SupplierRecord.GetName() );
	CString strFriendlyText = csvFriendlyText.GetLine();

	CString strEmailAddress = SupplierRecord.GetEmail();

	if ( strEmailAddress == "" )
		SMTPEmailHelpers.FlagUnsendableEmail( strFriendlyText, CUSTOMER_STATEMENT_ERROR_NOADDRESS );
	else
	{
		CString strEmailFilename;
		strEmailFilename.Format( "%d_%d", header.GetSuppNo(), header.GetOrderNum() );
		
		CreateOrderSheet( Order, FALSE, TRUE );
		CReportFileDoc ReportFileDoc ( Super.ReportFilename() );
		SMTPEmailHelpers.CreatePendingEmail( strEmailAddress, "Purchase Order", strFriendlyText, ReportFileDoc, strEmailFilename );
	}
}

/**********************************************************************/
