/**********************************************************************/
#include "NodeTypes.h"
#include "RepCSV.h"
#include "RepSpawn.h"
#include "StockAuditAdjustmentStock.h"
#include "StockOptionsIni.h"
#include "StockpointCSVArray.h"
/**********************************************************************/
#include "StockImportBasicAdjustmentStockDlg.h"
/**********************************************************************/

CStockImportBasicAdjustmentStockDlg::CStockImportBasicAdjustmentStockDlg( int nSpIdx, const char* szPath, const char* szReference1, const char* szReference2, const char* szReason, CStocktakeCSVRecord* pStocktake, CWnd* pParent )
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

	m_strImport = "Would you like to import adjustments now?";
	
	if ( DealerFlags.GetImportBarcodeFlag() == TRUE )
		m_strTitle = "Import Adjustments by Barcode Number";
	else
		m_strTitle = "Import Adjustments by Plu Number";
	
	m_nImportFileReportType = AUDIT_IMPORT_PLU_ADJUSTMENT_STOCK;
	m_nStockChangesReportType = AUDIT_ADJUSTMENT_STOCK;
	m_nExceptionReportType = AUDIT_EX_ADJUSTMENT_STOCK;
	m_strHeaderReference1 = szReference1;
	m_strHeaderReference2 = szReference2;
	m_strReason = szReason;

	ProcessImportData( ADD_STOCK_LEVELS );
}

/**********************************************************************/

void CStockImportBasicAdjustmentStockDlg::FillActionCombo()
{
	m_comboAction.ResetContent();
	m_comboAction.AddString ( "Add to period adjustment level" );
	m_comboAction.AddString ( "Subtract from period adjustment level" );
	m_comboAction.SetItemData( 0, ADD_STOCK_LEVELS );
	m_comboAction.SetItemData( 1, SUBTRACT_STOCK_LEVELS );
	m_comboAction.SetCurSel(0);
	m_comboAction.EnableWindow( TRUE );
}

/**********************************************************************/

void CStockImportBasicAdjustmentStockDlg::ShowStockChanges() 
{
	CStockAuditAdjustmentStock Audit;
	Audit.LockDatabase();
	Audit.SetReportType ( AUDIT_ADJUSTMENT_STOCK );
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

void CStockImportBasicAdjustmentStockDlg::AuditStockChanges ( CSSFile& file, CSSFile& fileTempReport, CStringArray& StockDetails, CCSV& csvOriginalData, int nAction )	
{
	for ( int nIndex = 0; nIndex < StockDetails.GetSize(); nIndex++ )
	{
		CCSVExplodedPlu csvAdjustment ( StockDetails.GetAt ( nIndex ) );
			
		int nStockIdx = csvAdjustment.GetStockIdx();
		CStockCSVRecord StockRecord;
		DataManager.Stock.GetAt ( nStockIdx, StockRecord );

		double dAdjustStockQty = csvAdjustment.GetStockQty();
		
		double dAdjustStockValue = 0.0;
		if ( StockOptions.GetImportAdjustValueFlag() == TRUE )
			dAdjustStockValue = csvAdjustment.GetStockValue();
		else
			dAdjustStockValue = DataManager.Stock.CalculateRetailValue( StockRecord, dAdjustStockQty ); 
		
		if ( nAction == SUBTRACT_STOCK_LEVELS )
		{
			dAdjustStockQty = -dAdjustStockQty;
			dAdjustStockValue = -dAdjustStockValue;
		}

		CCSVStockAudit csvOut;
		csvOut.Add ( "D" );
		csvOut.Add ( AUDIT_ADJUSTMENT_STOCK );
		csvOut.Add ( 1 );
		csvOut.Add ( StockRecord.GetStockCode() );
		csvOut.Add ( StockRecord.GetDescription() );
		csvOut.Add ( DataManager.Category.GetText ( StockRecord.GetCategory() ) );
		csvOut.Add ( DataManager.Stock.GetSubUnits( StockRecord ) );
		csvOut.AddStockAuditQty ( dAdjustStockQty );
		csvOut.AddStockAuditVal ( dAdjustStockValue );
		csvOut.Add ( m_strReason );
		file.WriteLine ( csvOut.GetLine() );

		if ( nIndex == 0 )
		{
			csvOut.RemoveAll();
			csvOut.Add ( "D" );
			csvOut.Add ( AUDIT_IMPORT_PLU_ADJUSTMENT_STOCK );
			
			if ( DealerFlags.GetImportBarcodeFlag() == TRUE )
			{
				csvOut.Add ( 2 );	
				csvOut.Add ( csvOriginalData.GetString(0) );
				csvOut.Add ( csvOriginalData.GetString(1) );
				csvOut.Add ( csvAdjustment.GetDescription() );
				csvOut.Add ( csvOriginalData.GetString(3) );
				csvOut.Add ( csvOriginalData.GetString(4) );
			}
			else
			{
				csvOut.Add ( 1 );	
				csvOut.Add ( csvOriginalData.GetString(0) );
				csvOut.Add ( csvAdjustment.GetDescription() );
				csvOut.Add ( csvOriginalData.GetString(2) );
				csvOut.Add ( csvOriginalData.GetString(3) );
			}

			fileTempReport.WriteLine ( csvOut.GetLine() );
		}
	}
}

/**********************************************************************/

