/**********************************************************************/
#include "NodeTypes.h"
#include "RepCSV.h"
#include "RepSpawn.h"
#include "SalesHistoryFields.h"
#include "SalesTypes.h"
#include "StockAuditSales.h"
#include "StockOptionsIni.h"
#include "WrapperErrors.h"
/**********************************************************************/
#include "StockImportBasicWrapperSalesDlg.h"
/**********************************************************************/

CStockImportBasicWrapperSalesDlg::CStockImportBasicWrapperSalesDlg( int nSpIdx, const char* szPath, CSuppRefFinder& SuppRefFinder, CStocktakeCSVRecord* pStocktake, CWnd* pParent )
	: CStockImportBasicWrapperDlg( nSpIdx, szPath, SuppRefFinder, pParent )
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

	m_strImport = "Would you like to import these sales now?";
	m_strTitle = "Import Sales by Wrapper Code";
	
	m_nImportFileReportType = AUDIT_IMPORT_WRAPPER_SALES;
	m_nStockChangesReportType = AUDIT_SALES;
	m_nExceptionReportType = AUDIT_EX_SALES;
	
	ProcessImportData( ADD_STOCK_LEVELS );
}

/**********************************************************************/

void CStockImportBasicWrapperSalesDlg::FillActionCombo()
{
	m_comboAction.ResetContent();
	m_comboAction.AddString ( "Add to period sales level" );
	m_comboAction.AddString ( "Subtract from period sales level" );
	m_comboAction.SetItemData( 0, ADD_STOCK_LEVELS );
	m_comboAction.SetItemData( 1, SUBTRACT_STOCK_LEVELS );
	m_comboAction.SetCurSel(0);
	m_comboAction.EnableWindow( TRUE );
}

/**********************************************************************/

void CStockImportBasicWrapperSalesDlg::ShowStockChanges() 
{
	CStockAuditSales Audit;
	Audit.SetReportType ( AUDIT_SALES );
	Audit.SetConLevel ( NODE_STOCKPOINT );
	Audit.SetConLevelText( dbStockpoint.GetName( m_nSpIdx ) );
	Audit.SetTableNo( dbStockpoint.GetSpNo( m_nSpIdx ) );
	Audit.SetAuditMode( AUDIT_MODE_PREVIEW );
	Audit.SetManualSalesFlag( TRUE );
	Audit.SetImportSalesFlag( TRUE );
	Audit.SetE1SalesFlag( FALSE );
	Audit.SetHidePluNoFlag( TRUE );

	CStockAuditHeaderInfo info;
	info.m_date = m_ImportDate;
	Audit.SetPreviewHeader( Audit.GetHeaderLine( info ) );

	if ( Audit.CreateReport( DataManager.GetActiveDbIdx() ) )
	{
		CReportManager ReportManager;
		ReportManager.DisplayReport ( Super.ReportFilename(), Audit.GetReportParamsFilename(), "", this );
	}
}

/**********************************************************************/

void CStockImportBasicWrapperSalesDlg::AuditStockChanges ( CSSFile& file, CSSFile& fileTempReport, CCSV& csvOriginalData, int nStockIdx, int nStockSuppIdx, double dDUQty, double dTotalRetail, int nAction )
{
	CStockCSVRecord StockRecord;
	DataManager.Stock.GetAt( nStockIdx, StockRecord );

	CString strCategory = DataManager.Category.GetText ( StockRecord.GetCategory() );
	CString strSuppRef = StockRecord.GetSuppRefExternal ( nStockSuppIdx, DealerFlags.GetSuppRefFlag() );

	double dRetailPerCase = 0.0;
	
	switch( DealerFlags.GetPluCSVImportType() )
	{
	case 0:
	case 2:
		dRetailPerCase = ( dDUQty != 0.0 ) ? dTotalRetail / dDUQty : 0.0;
		break;

	default:
		dRetailPerCase = StockRecord.GetRetail( nStockSuppIdx );
		dTotalRetail = dRetailPerCase * dDUQty;
		break;
	}

	double dSUQty = dDUQty * StockRecord.GetYield( nStockSuppIdx );
		
	if ( nAction == SUBTRACT_STOCK_LEVELS )
	{
		dDUQty = -dDUQty;
		dSUQty = -dSUQty;
		dTotalRetail = -dTotalRetail;
	}

	int nSubUnits = DataManager.Stock.GetSubUnits( StockRecord );

	CCSVStockAudit csvOut;
	csvOut.Add ( "D" );
	csvOut.Add ( AUDIT_SALES );
	csvOut.Add ( 1 );
	csvOut.Add ( SALES_TYPE_IMPORT_STOCK );
	csvOut.Add ( "" );
	csvOut.Add ( "" );
	csvOut.Add ( StockRecord.GetDescription() );
	csvOut.Add ( "" );
	csvOut.Add ( "" );
	csvOut.Add ( StockRecord.GetStockCode() );
	csvOut.Add ( nSubUnits );
	csvOut.AddStockAuditQty ( dSUQty );		
	csvOut.AddStockAuditVal ( dTotalRetail );
	file.WriteLine ( csvOut.GetLine() );

	csvOut.RemoveAll();
	csvOut.Add ( "D" );
	csvOut.Add ( AUDIT_IMPORT_WRAPPER_SALES );
	csvOut.Add ( 1 );
	csvOut.Add ( csvOriginalData.GetString(0) );
	csvOut.Add ( csvOriginalData.GetString(1) );
	csvOut.Add ( csvOriginalData.GetString(2) );
	fileTempReport.WriteLine ( csvOut.GetLine() );
}

/**********************************************************************/

void CStockImportBasicWrapperSalesDlg::AuditExceptions ( CSSFile& file, CSSFile& fileTempReport, CCSV& csvOriginalData, int nReason, const char* szStockCode )
{
	CString strError;
	switch( nReason )
	{
	case WRAPPER_ERROR_IN_STOCKTAKE:		strError = "Stockcode in pending stock take";				break;
	case WRAPPER_ERROR_WRONG_STOCKTAKE:		strError = "Stockcode not in this stock take";				break;
	case WRAPPER_ERROR_NOT_FOUND:			strError = "Unknown wrapper code";							break;
	case WRAPPER_ERROR_STOCK_INACTIVE:		strError = "Stock item not active at this stockpoint";		break;
	default:								strError = "Unknown error";									break;
	}

	CCSVStockAudit csvOut;
	csvOut.Add ( "D" );
	csvOut.Add ( AUDIT_EX_SALES );
	csvOut.Add ( 2 );
	csvOut.Add ( SALES_TYPE_IMPORT );
	csvOut.Add ( csvOriginalData.GetString(0) );
	csvOut.Add ( "" );
	csvOut.Add ( csvOriginalData.GetString(1) );
	csvOut.Add ( csvOriginalData.GetString(2) );		
	csvOut.Add ( szStockCode );	
	csvOut.Add ( strError );
	csvOut.Add ( "" );		//IGNORE SALES DATE
	csvOut.Add ( "" );		//IGNORE SALES TIME
	csvOut.Add ( TRUE );	//IS WRAPPER
	file.WriteLine( csvOut.GetLine() );

	csvOut.RemoveAll();
	csvOut.Add ( "D" );
	csvOut.Add ( m_nImportFileReportType );
	csvOut.Add ( 1 );
	csvOut.Add ( csvOriginalData.GetString(0) );
	csvOut.Add ( csvOriginalData.GetString(1) );
	csvOut.Add ( csvOriginalData.GetString(2) );
	csvOut.Add ( strError );

	fileTempReport.WriteLine ( csvOut.GetLine() );
}

/**********************************************************************/
