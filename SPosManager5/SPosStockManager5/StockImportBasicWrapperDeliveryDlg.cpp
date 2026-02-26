/**********************************************************************/
#include "NodeTypes.h"
#include "RepCSV.h"
#include "ReportHelpers.h"
#include "RepSpawn.h"
#include "StockAuditDeliveryDetail.h"
#include "StockOptionsIni.h"
/**********************************************************************/
#include "StockImportBasicWrapperDeliveryDlg.h"
/**********************************************************************/

CStockImportBasicWrapperDeliveryDlg::CStockImportBasicWrapperDeliveryDlg( int nSpIdx, int nSuppIdx, const char* szPath, const char* szReference1, const char* szReference2, CSuppRefFinder& SuppRefFinder, CStocktakeCSVRecord* pStocktake, CWnd* pParent )
	: CStockImportBasicWrapperDlg ( nSpIdx, szPath, SuppRefFinder, pParent )
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
	m_strTitle = "Import Deliveries by Wrapper Code";
	m_strHeaderReference1 = szReference1;
	m_strHeaderReference2 = szReference2;
	m_nImportFileReportType = AUDIT_IMPORT_WRAPPER_DELIVERY;
	m_nStockChangesReportType = AUDIT_DELIVERY;
	m_nExceptionReportType = AUDIT_EX_DELIVERY;
	m_nSuppIdx = nSuppIdx;

	if ( m_nSuppIdx >= 0 && m_nSuppIdx < DataManager.Supplier.GetSize() )
	{
		m_strHeaderSuppName = DataManager.Supplier.GetName( m_nSuppIdx );
		m_nSuppNo = DataManager.Supplier.GetSuppNo( nSuppIdx );
	}
	else
	{
		m_strHeaderSuppName = "Unspecified";
		m_nSuppNo = 0;
	}

	ProcessImportData( ADD_STOCK_LEVELS );
}

/**********************************************************************/

void CStockImportBasicWrapperDeliveryDlg::FillActionCombo()
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

void CStockImportBasicWrapperDeliveryDlg::ShowStockChanges() 
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

void CStockImportBasicWrapperDeliveryDlg::AuditStockChanges ( CSSFile& file, CSSFile& fileTempReport, CCSV& csvOriginalData, int nStockIdx, int nStockSuppIdx, double dDUQty, double dTotalCost, int nAction )
{
	CStockCSVRecord StockRecord;
	DataManager.Stock.GetAt( nStockIdx, StockRecord );

	CString strCategory = DataManager.Category.GetText ( StockRecord.GetCategory() );
	CString strSuppRef = StockRecord.GetSuppRefExternal ( nStockSuppIdx, DealerFlags.GetSuppRefFlag() );

	double dCostPerCase = 0.0;
	if ( StockOptions.GetImportAdhocValueFlag() == FALSE )
	{
		dCostPerCase = StockRecord.GetCost( nStockSuppIdx );
		dTotalCost = dCostPerCase * dDUQty;
	}
	else
		dCostPerCase = ( dDUQty != 0.0 ) ? dTotalCost / dDUQty : 0.0;
			
	double dSUQty = dDUQty * StockRecord.GetYield( nStockSuppIdx );
	double dLineRetail = DataManager.Stock.CalculateRetailValue( StockRecord, dSUQty );
	double dCaseRetail = ( dDUQty != 0.0 ) ? dLineRetail / dDUQty : 0.0;
	double dLineTax = DataManager.Stock.CalculateTaxContentFromValue( StockRecord, dLineRetail );
	int nDUItems = StockRecord.GetDUItems( nStockSuppIdx );
	double dDUItemSize = StockRecord.GetDUItemSize( nStockSuppIdx );

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
	double dLineCostTax = DataManager.Stock.CalculateTaxOnCost( StockRecord, dTotalCost );
		
	if ( nAction == SUBTRACT_STOCK_LEVELS )
	{
		dDUQty = -dDUQty;
		dSUQty = -dSUQty;
		dTotalCost = -dTotalCost;
		dLineRetail = -dLineRetail;
		dLineTax = -dLineTax;
		dLineWaste = -dLineWaste;
		dLineCostTax = -dLineCostTax;
	}

	CCSVStockAudit csvOut;
	csvOut.Add ( "D" );
	csvOut.Add ( AUDIT_DELIVERY );
	csvOut.Add ( 4 );
	csvOut.Add ( StockRecord.GetStockCode() );		//stock code
	csvOut.Add ( StockRecord.GetDescription() );	//description	
	csvOut.Add ( strCategory );						//category
	csvOut.AddStockAuditQty ( dDUQty );				//case qty
	csvOut.AddStockAuditQty ( dSUQty );				//stock unit qty
	csvOut.AddStockAuditVal ( dCostPerCase );		//case cost
	csvOut.AddStockAuditVal ( dTotalCost );			//stock cost
	csvOut.AddStockAuditVal ( dCaseRetail );		//case retail
	csvOut.AddStockAuditVal ( dLineRetail );		//total retail
	csvOut.AddStockAuditVal ( dLineTax );			//total tax
	csvOut.AddStockAuditVal ( dLineWaste );			//total waste
	csvOut.Add ( dGPPercent, 2 );					//profit margin
	csvOut.Add ( strSuppRef );						//supplier reference
	csvOut.Add ( nDUItems );						//items per du
	csvOut.AddStockAuditQty( dDUItemSize );			//stock units per item
	csvOut.AddStockAuditVal( dLineCostTax );		//estimated tax on cost
	
	file.WriteLine ( csvOut.GetLine() );

	csvOut.RemoveAll();
	csvOut.Add ( "D" );
	csvOut.Add ( AUDIT_IMPORT_WRAPPER_DELIVERY );
	csvOut.Add ( 1 );
	csvOut.Add ( csvOriginalData.GetString(0) );
	csvOut.Add ( csvOriginalData.GetString(1) );
	csvOut.Add ( csvOriginalData.GetString(2) );
	fileTempReport.WriteLine ( csvOut.GetLine() );
}

/**********************************************************************/

