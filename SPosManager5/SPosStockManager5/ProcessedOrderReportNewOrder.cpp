/**********************************************************************/
#include "CCSVReportLine.h"
//#include "DataManager.h"
#include "ReportHelpers.h"
/**********************************************************************/
#include "ProcessedOrderReportNewOrder.h"
/**********************************************************************/

CProcessedOrderReportNewOrder::CProcessedOrderReportNewOrder( CProcessedOrder& Order ) : CProcessedOrderBaseReport(), m_Order( Order )
{
}

/**********************************************************************/

bool CProcessedOrderReportNewOrder::CreateReport( int nDbIdx )
{
	m_nDbIdx = nDbIdx;
	bool bSuccess = FALSE;

	CReportFile ReportFile;
	if ( ReportFile.Open ( Super.ReportFilename() ) == TRUE )
	{
		bSuccess = TRUE;
		
		ReportFile.SetStyle1 ( GetReportTitle() );
		ReportFile.AddColumn ( "Order No.", TA_LEFT, 200 );
		ReportFile.AddColumn ( "Date", TA_LEFT, 200 );
		ReportFile.AddColumn ( "Code", TA_LEFT, 380 );
		ReportFile.AddColumn ( "Description", TA_LEFT, 300 );
		ReportFile.AddColumn ( "Reference", TA_LEFT, 380 );
		ReportFile.AddColumn ( "DU Qty", TA_RIGHT, 200 );
		ReportFile.AddColumn ( "Yield", TA_RIGHT, 250 );
		ReportFile.AddColumn ( "SU Qty", TA_RIGHT, 200 );
		ReportFile.AddColumn ( "DU Cost", TA_RIGHT, 200 );
		ReportFile.AddColumn ( "Total Cost", TA_RIGHT, 220 );
	
		WriteOrder ( ReportFile, nDbIdx );
		
		ShowStockpoints ( ReportFile );
		ReportFile.Close();
	}
	return bSuccess;
}
	
/**********************************************************************/

void CProcessedOrderReportNewOrder::WriteOrder( CReportFile& ReportFile, int nDbIdx )
{
	double dOrderCost = 0.0;

	CProcessedOrderHeader header;
	m_Order.GetHeader( header );

	int nSuppIdx;
	int nSuppNo = header.GetSuppNo();
	if ( DataManager.Supplier.FindSupplierByNumber( nSuppNo, nSuppIdx ) == FALSE )
	{
		nSuppIdx = -1;
		nSuppNo = 0;
	}

	CString strLine = "";

	if ( nSuppIdx >= 0 )
	{
		strLine.Format ( "<..>Supplier %s", 
			DataManager.Supplier.GetName ( nSuppIdx ) );	
	}
	else
		strLine = "<..>Unspecified Supplier";

	ReportFile.WriteLine ( strLine );
	ReportFile.WriteLine ( "<LI>" );

	for ( int nItemIdx = 0; nItemIdx < m_Order.GetItemCount(); nItemIdx++ )
	{
		CProcessedOrderItem item;
		m_Order.GetItem( nItemIdx, item );

		double dSUCost = item.GetDUCost() * item.GetDUQty();
		dOrderCost += dSUCost;
		
		CStockReportLine csvOut;
		CString strNum;
		strNum.Format ( "%6.6d", header.GetOrderNum() );
		csvOut.AppendString ( strNum );
		CSSDate date ( item.GetDate() );
		csvOut.AppendString ( date.GetDate() );
		csvOut.AppendString ( ReportHelpers.GetDisplayStockCode ( item.GetStockCode() ) );
		csvOut.AppendString ( item.GetDescription() );
		csvOut.AppendString ( item.GetSuppRef() );
		csvOut.AppendInt ( item.GetDUQty() );

		CString strItem;
		strItem.Format ( "%d x %.3f", item.GetDUItems(), item.GetDUItemSize() );
		csvOut.AppendString ( strItem );

		csvOut.AppendStockQty ( item.CalculateSUQty(), 1, TRUE );
		csvOut.AppendVal ( item.GetDUCost() );
		csvOut.AppendVal ( dSUCost );

		ReportFile.WriteLine ( csvOut.GetLine() );
	}

	ReportFile.WriteLine ( "\t\t\t\t\t\t\t\t\t<LI>" );
	CStockReportLine csvTotal;
	csvTotal.AppendBlanks ( 2 );
	csvTotal.AppendString ( "Supplier Total" );
	csvTotal.AppendBlanks ( 6 );
	csvTotal.AppendVal ( dOrderCost );
	ReportFile.WriteLine ( csvTotal.GetLine() );
	ReportFile.WriteLine ( "" );	
}
	
/**********************************************************************/
