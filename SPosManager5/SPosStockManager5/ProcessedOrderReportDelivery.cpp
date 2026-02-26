/**********************************************************************/
#include "CCSVReportLine.h"
#include "DatabaseCSVArray.h"
//#include "DataManager.h"
//#include "DealerFlags.h"
//#include "EcrmanOptionsIni.h"
#include "ProcessedOrderItem.h"
#include "ReportHelpers.h"
#include "StockpointCSVArray.h"
/**********************************************************************/
#include "ProcessedOrderReportDelivery.h"
/**********************************************************************/

CProcessedOrderReportDelivery::CProcessedOrderReportDelivery( CWordArray& wSuppSelectArray ) : CProcessedOrderBaseReport()
{
	m_bInRange = FALSE;
	m_bLegacyOrder = FALSE;
	m_nOrderNum = 0;
	m_bLatestFlag = FALSE;
	m_bPageBreakFlag = FALSE;
	m_strStockCodeFrom = "";
	m_strStockCodeTo = "";

	for ( int nIndex = 0; nIndex < wSuppSelectArray.GetSize(); nIndex++ )
		m_wSuppSelectArray.Add ( wSuppSelectArray.GetAt ( nIndex ) );
}

/**********************************************************************/

bool CProcessedOrderReportDelivery::CreateReport( int nDbIdx )
{
	m_nDbIdx = nDbIdx;
	bool bSuccess = FALSE;

	CReportFile ReportFile;
	if ( ReportFile.Open ( Super.ReportFilename() ) == TRUE )
	{
		bSuccess = TRUE;
		ReportFile.SetStyle1 ( GetReportTitle() );
		ReportFile.AddColumn ( "Ordered", TA_LEFT, 200 );
		ReportFile.AddColumn ( "Order No.", TA_LEFT, 200 );
		ReportFile.AddColumn ( "Code", TA_LEFT, 380 );
		ReportFile.AddColumn ( "Description", TA_LEFT, 300 );
		ReportFile.AddColumn ( "DU Qty", TA_RIGHT, 200 );
		ReportFile.AddColumn ( "Yield", TA_RIGHT, 200 );
		ReportFile.AddColumn ( "SU Qty", TA_RIGHT, 200 );
		ReportFile.AddColumn ( "DU Cost", TA_RIGHT, 200 );
		ReportFile.AddColumn ( "Total Cost", TA_RIGHT, 220 );

		CArray<int,int> arraySpIdx;

		for ( int nSpIdx = 0; nSpIdx < dbStockpoint.GetSize(); nSpIdx++ )
			if ( MatchStockpoint ( nSpIdx ) == TRUE )
				arraySpIdx.Add( nSpIdx );

		//if ( EcrmanOptions.GetReportsLocationSortType() != 0 )
			dbStockpoint.SortSpListByName( arraySpIdx );

		for ( int n = 0; n < arraySpIdx.GetSize(); n++ )
		{
			int nSpIdx = arraySpIdx.GetAt(n);
			ReportFile.AddColumn ( dbStockpoint.GetName( nSpIdx ), TA_RIGHT, 260 );
		}		
			
		WriteDeliveries ( ReportFile );
		ShowStockpoints ( ReportFile );

		ReportFile.Close();
	}
	return bSuccess;
}

/**********************************************************************/

void CProcessedOrderReportDelivery::WriteDeliveries( CReportFile& ReportFile )
{
	CSSDate DateFrom ( m_strDateFrom );
	CSSDate DateTo ( m_strDateTo );
	double dOrderCost = 0.0;

	//loop through all selected suppliers
	for ( int nSelectIdx = 0; nSelectIdx < m_wSuppSelectArray.GetSize(); nSelectIdx++)
	{
		CString strLine;
		
		int nSuppIdx = m_wSuppSelectArray.GetAt ( nSelectIdx ) - 1;

		if ( nSuppIdx >= 0 )
			strLine.Format ( "<..>Supplier %s", DataManager.Supplier.GetName ( nSuppIdx ) );
		else 
			strLine = "<..>Unspecified Supplier";
		
		CString strFilename = dbDatabase.GetFolderPathData ( m_nDbIdx );
		strFilename += "\\";

		if ( m_bLatestFlag == TRUE )
			strFilename += DataManager.Supplier.GetTempFilename ( nSuppIdx );
		else
			strFilename += DataManager.Supplier.GetAuditFilename ( nSuppIdx );

		CSSFile fileAudit;
		if ( fileAudit.Open ( strFilename, "rb" ) == FALSE )
			continue;

		CString strBuffer;

		while ( fileAudit.ReadString ( strBuffer ) == TRUE )
		{
			CCSV csv ( strBuffer );
			CString strTest = csv.GetString(0);

			if ( ( strTest == "HEADER2" ) || ( strTest == "HEADER3" ) )
			{
				if ( strTest == "HEADER2" )
					m_bLegacyOrder = TRUE;
				else
				{
					m_bLegacyOrder = FALSE;
					m_nOrderNum = csv.GetInt(4);
				}

				CSSDate date ( csv.GetString(1) );

				if ( m_bLatestFlag == TRUE || ( ( date >= DateFrom ) && ( date <= DateTo ) ) )
				{
					if ( m_bInRange == TRUE )
						WriteOrderTotal ( ReportFile, dOrderCost );
					
					dOrderCost = 0.0;	
					m_bInRange = TRUE;
						
					if ( m_strReference != "" )
					{
						CString strAuditReference = csv.GetString(2);
						CString strReference = m_strReference;
						strAuditReference.MakeUpper();
						strReference.MakeUpper();

						if ( strAuditReference.Find ( strReference ) == -1 )
							m_bInRange = FALSE;
					}

					if ( m_bInRange == TRUE )
					{
						CString strAuditReference = csv.GetString(2);
						
						if ( strAuditReference == "" )
							strAuditReference = "None";

						CString strLine;
					
						if ( nSuppIdx >= 0 )
						{
							strLine.Format ( "<..>Supplier %s, Delivery reference %s, Arrived %s", 
								DataManager.Supplier.GetName ( nSuppIdx ), 
								(const char*) strAuditReference, 
								date.GetDate() );
						}
						else
						{
							strLine.Format ( "<..>Unspecified Supplier, Delivery reference %s, Arrived %s", 
								(const char*) strAuditReference, 
								date.GetDate() );
						}

						ReportFile.WriteLine ( strLine );
						ReportFile.WriteLine ( "<LI>" );
					}
				}
				else
					m_bInRange = FALSE;
			}
			else
			{
				if ( TRUE == m_bInRange )
				{
					CProcessedOrderItem item;

					if ( TRUE == m_bLegacyOrder )
					{
						COrderCSVRecord OrderRecord;
						OrderRecord.ConvertFromCSV( csv );
						
						item.SetStockCode( OrderRecord.GetStockCode() );
						item.SetDescription( OrderRecord.GetDescription() );
						item.SetDUItems( OrderRecord.GetDUItems() );
						item.SetDUItemSize( OrderRecord.GetDUItemSize() );
						item.SetDUCost( OrderRecord.GetDUCost() );
						item.SetDUQty( OrderRecord.GetDUQty() );
						item.SetDate( OrderRecord.GetDate() );

						for ( int n = 0; n < OrderRecord.GetStockpointCount(); n++ )
						{
							int nSpNo = OrderRecord.GetSpNo( n );
							int nDUItems = OrderRecord.GetDUItems( n );
							item.AddOrder( nSpNo, nDUItems );
						}

						m_nOrderNum = OrderRecord.GetOrderNum();
					}
					else
						item.ConvertFromCSV( csv );

					bool bStockCodeOK = TRUE;
					CString strOrderStockCode = item.GetStockCode();

					if ( m_strStockCodeFrom != "" )
						if ( strOrderStockCode < m_strStockCodeFrom )
							bStockCodeOK = FALSE;
		
					if ( m_strStockCodeTo != "" )
						if ( strOrderStockCode > m_strStockCodeTo )
							bStockCodeOK = FALSE;

					if ( TRUE == bStockCodeOK )
					{
						double dSUCost = item.GetDUCost() * item.GetDUQty();
						dOrderCost += dSUCost;

						CStockReportLine csvOut;
						CSSDate date ( item.GetDate() );
						csvOut.AppendString ( date.GetDate() );
						CString strNum;
						strNum.Format ( "%6.6d", m_nOrderNum );
						csvOut.AppendString ( strNum );
						csvOut.AppendString ( ReportHelpers.GetDisplayStockCode ( strOrderStockCode ) );
						csvOut.AppendString ( item.GetDescription() );
						csvOut.AppendInt ( item.GetDUQty() );

						CString strItem;
						strItem.Format ( "%d x %.3f", item.GetDUItems(), item.GetDUItemSize() );
						csvOut.AppendString ( strItem );

						csvOut.AppendStockQty ( item.CalculateSUQty(), 1, TRUE );
						csvOut.AppendVal ( item.GetDUCost() );
						csvOut.AppendVal ( dSUCost );

						CArray<int,int> arraySpIdx;

						for ( int nSpIdx = 0; nSpIdx < dbStockpoint.GetSize(); nSpIdx++ )
							if ( MatchStockpoint ( nSpIdx ) == TRUE )
								arraySpIdx.Add( nSpIdx );

						//if ( EcrmanOptions.GetReportsLocationSortType() != 0 )
							dbStockpoint.SortSpListByName( arraySpIdx );

						for ( int n = 0; n < arraySpIdx.GetSize(); n++ )
						{
							int nSpIdx = arraySpIdx.GetAt(n);
							int nSpNo = dbStockpoint.GetSpNo( nSpIdx );
							csvOut.AppendStockQty ( item.GetStockpointOrder ( nSpNo ) * item.GetDUItemSize(), 1, TRUE );		
						}

						ReportFile.WriteLine ( csvOut.GetLine() );

						if ( DealerFlags.GetDeliveryLinesFlag() == TRUE )
							ReportFile.WriteLine( "<LI>" );
					}
				}
			}
		}
		if ( m_bInRange == TRUE )
		{
			WriteOrderTotal ( ReportFile, dOrderCost );
			m_bInRange = FALSE;
		}	
		ReportFile.WriteLine ( "" );
	}
}

/**********************************************************************/

void CProcessedOrderReportDelivery::WriteOrderTotal ( CReportFile& ReportFile, double dTotal )
{
	if ( DealerFlags.GetDeliveryLinesFlag() == FALSE )
	{
		ReportFile.WriteLine ( "\t\t\t\t\t\t\t\t<LI>" );
	
		CStockReportLine csvTotal;
		csvTotal.AppendBlanks ( 2 );
		csvTotal.AppendString ( "Order Total" );
		csvTotal.AppendBlanks ( 5 );
		csvTotal.AppendVal ( dTotal );
		ReportFile.WriteLine ( csvTotal.GetLine() );
	}

	ReportFile.WriteLine ( "" );
	if ( m_bPageBreakFlag == TRUE )
		ReportFile.WriteLine ( "<NEWPAGE>" );
}				

/**********************************************************************/
	