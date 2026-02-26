/**********************************************************************/
#include "NodeTypes.h"
#include "RepCSV.h"
#include "ReportHelpers.h"
#include "RepSpawn.h"
#include "StockAuditDeliveryDetail.h"
#include "StockOptionsIni.h"
/**********************************************************************/
#include "StockImportBasicDeliveryDlg.h"
/**********************************************************************/

CStockImportBasicDeliveryDlg::CStockImportBasicDeliveryDlg ( int nSpIdx, int nSuppIdx, const char* szPath, const char* szReference1, const char* szReference2, CStocktakeCSVRecord* pStocktake, CWnd* pParent )
	: CStockImportBasicDlg ( nSpIdx, szPath, pParent ) 
{
	if ( NULL == pStocktake )
	{
		m_nStocktakeNo = -1;
		m_ImportDate = COleDateTime::GetCurrentTime();
	}
	else
	{
		m_nStocktakeNo = pStocktake -> GetStocktakeNo();
		m_ImportDate = pStocktake -> GetOleDateTime();
	}

	m_strImport = "Would you like to import these deliveries now?";
	
	if ( DealerFlags.GetImportBarcodeFlag() == TRUE )
		m_strTitle = "Import Deliveries by Barcode Number";
	else
		m_strTitle = "Import Deliveries by Plu Number";
	
	m_strHeaderReference1 = szReference1;
	m_strHeaderReference2 = szReference2;
	m_nImportFileReportType = AUDIT_IMPORT_PLU_DELIVERY;
	m_nStockChangesReportType = AUDIT_DELIVERY;
	m_nExceptionReportType = AUDIT_EX_DELIVERY;
	m_nSuppIdx = nSuppIdx;

	if ( m_nSuppIdx >= 0 && m_nSuppIdx < DataManager.Supplier.GetSize() )
		m_strHeaderSuppName = DataManager.Supplier.GetName( m_nSuppIdx );
	else
		m_strHeaderSuppName = "Unspecified";
		
	ProcessImportData( ADD_STOCK_LEVELS );
}

/**********************************************************************/

void CStockImportBasicDeliveryDlg::FillActionCombo()
{
	m_comboAction.ResetContent();
	m_comboAction.AddString ( "Add to period delivery level" );
	m_comboAction.AddString ( "Subtract from period delivery level" );
	m_comboAction.SetItemData( 0, ADD_STOCK_LEVELS );
	m_comboAction.SetItemData( 1, SUBTRACT_STOCK_LEVELS );
	m_comboAction.SetCurSel(0);
	m_comboAction.EnableWindow( TRUE );
}

/**********************************************************************/

void CStockImportBasicDeliveryDlg::ShowStockChanges() 
{
	CStockAuditDeliveryDetail Audit( DealerFlags.GetDeliveryDetailFlag() );
	Audit.SetReportType ( AUDIT_DELIVERY );
	Audit.SetConLevel ( NODE_STOCKPOINT );
	Audit.SetConLevelText( dbStockpoint.GetName( m_nSpIdx ) );
	Audit.SetTableNo( dbStockpoint.GetSpNo( m_nSpIdx ) );
	Audit.SetAuditMode( AUDIT_MODE_PREVIEW );

	CStockAuditHeaderInfo info;
	info.m_date = m_ImportDate;
	info.m_strReference1 = m_strHeaderReference1;
	info.m_strReference2 = m_strHeaderReference2;
	info.m_strSuppName = m_strHeaderSuppName;
	Audit.SetPreviewHeader( Audit.GetHeaderLine( info ) );

	if ( Audit.CreateReport( DataManager.GetActiveDbIdx() ) )
	{
		CReportManager ReportManager;
		ReportManager.DisplayReport ( Super.ReportFilename(), Audit.GetReportParamsFilename(), "", this );
	}
}

/**********************************************************************/

void CStockImportBasicDeliveryDlg::AuditStockChanges ( CSSFile& file, CSSFile& fileTempReport, CStringArray& StockDetails, CCSV& csvOriginalData, int nAction )
{
	for ( int nIndex = 0; nIndex < StockDetails.GetSize(); nIndex++ )
	{
		CCSVExplodedPlu csvDelivery ( StockDetails.GetAt ( nIndex ) );
		
		int nStockIdx = csvDelivery.GetStockIdx();
		double dStockQty = csvDelivery.GetStockQty();

		CStockCSVRecord StockRecord;
		DataManager.Stock.GetAt ( nStockIdx, StockRecord );
		CString strCategory = DataManager.Category.GetText ( StockRecord.GetCategory() );

		double dStockValue = 0.0;
		CString strSuppRef = "";

		if ( StockOptions.GetImportAdhocValueFlag() == TRUE )
			dStockValue = csvDelivery.GetStockValue();
		else
			dStockValue = dStockQty * StockRecord.GetUnitCost(); 

		if ( m_nSuppIdx >= 0 && m_nSuppIdx < DataManager.Supplier.GetSize() )
		{
			int nSuppNo = DataManager.Supplier.GetSuppNo ( m_nSuppIdx );
			int nStockSuppIdxToUse = -1;

			for ( int nStockSuppIdx = 0; nStockSuppIdx < StockRecord.GetSupplierCount(); nStockSuppIdx++ )
			{
				if ( StockRecord.GetSuppNo ( nStockSuppIdx ) == nSuppNo )
				{
					nStockSuppIdxToUse = nStockSuppIdx;
					break;
				}
			}

			if ( nStockSuppIdxToUse >= 0 && nStockSuppIdxToUse < StockRecord.GetSupplierCount() )
			{
				strSuppRef = StockRecord.GetSuppRefExternal ( nStockSuppIdxToUse, DealerFlags.GetSuppRefFlag() );

				if ( StockOptions.GetImportAdhocValueFlag() == FALSE )
					dStockValue = dStockQty * StockRecord.GetUnitCost( nStockSuppIdxToUse ); 
			} 
		}

		double dLineRetail = DataManager.Stock.CalculateRetailValue( StockRecord, dStockQty );
		double dLineTax = DataManager.Stock.CalculateTaxContentFromValue( StockRecord, dLineRetail );
	
		double dLineWaste = 0.0;
		if ( StockOptions.GetStockSubUnitsFlag() )
		{
			double dNonWastePercent = 100.0 - StockRecord.GetWastePercent();
		
			if ( dNonWastePercent != 0.0 )
			{
				double dCostWithWaste = ( dStockValue * 100.0 ) / dNonWastePercent;  
				dLineWaste = dCostWithWaste - dStockValue;
			}
		}

		double dGPPercent = ReportHelpers.CalcGPPercent( dLineRetail - dLineTax, 0, dStockValue + dLineWaste );
		double dLineCostTax = DataManager.Stock.CalculateTaxOnCost( StockRecord, dStockValue );
		
		if ( nAction == SUBTRACT_STOCK_LEVELS )
		{
			dStockQty = -dStockQty;
			dStockValue = -dStockValue;
			dLineRetail = -dLineRetail;
			dLineTax = -dLineTax;
			dLineWaste = -dLineWaste;
			dLineCostTax = -dLineCostTax;
		}

		CCSVStockAudit csvOut;
		csvOut.Add ( "D" );
		csvOut.Add ( AUDIT_DELIVERY );
		csvOut.Add ( 4 );
		csvOut.Add ( StockRecord.GetStockCode() );	//stock code
		csvOut.Add ( StockRecord.GetDescription() );	//description	
		csvOut.Add ( strCategory );					//category
		csvOut.AddStockAuditQty ( 0.0 );			//case qty
		csvOut.AddStockAuditQty ( dStockQty );		//stock unit qty
		csvOut.AddStockAuditVal ( 0.0 );			//case cost
		csvOut.AddStockAuditVal ( dStockValue );	//stock cost
		csvOut.AddStockAuditVal ( 0.0 );			//case retail
		csvOut.AddStockAuditVal ( dLineRetail );	//total retail
		csvOut.AddStockAuditVal ( dLineTax );		//total tax
		csvOut.AddStockAuditVal ( dLineWaste );		//total waste
		csvOut.Add ( dGPPercent, 2 );				//profit margin
		csvOut.Add ( strSuppRef );					//supplier reference
		csvOut.Add ( 0 );							//delivery items
		csvOut.AddStockAuditQty( 0.0 );				//delivery item size
		csvOut.AddStockAuditVal( dLineCostTax );	//estimated tax on cost
	
		file.WriteLine ( csvOut.GetLine() );

		if ( nIndex == 0 )
		{
			csvOut.RemoveAll();
			csvOut.Add ( "D" );
			csvOut.Add ( AUDIT_IMPORT_PLU_DELIVERY );
			
			if ( DealerFlags.GetImportBarcodeFlag() == TRUE )
			{
				csvOut.Add ( 2 );	
				csvOut.Add ( csvOriginalData.GetString(0) );
				csvOut.Add ( csvOriginalData.GetString(1) );
				csvOut.Add ( csvDelivery.GetDescription() );
				csvOut.Add ( csvOriginalData.GetString(3) );
				csvOut.AddStockAuditVal ( dStockValue );
			}
			else
			{
				csvOut.Add ( 1 );	
				csvOut.Add ( csvOriginalData.GetString(0) );
				csvOut.Add ( csvDelivery.GetDescription() );
				csvOut.Add ( csvOriginalData.GetString(2) );
				csvOut.AddStockAuditVal ( dStockValue );
			}
				
			fileTempReport.WriteLine ( csvOut.GetLine() );
		}
	}
}

/**********************************************************************/

