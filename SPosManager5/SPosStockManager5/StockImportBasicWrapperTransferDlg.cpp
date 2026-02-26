/**********************************************************************/
#include "NodeTypes.h"
#include "RepCSV.h"
#include "ReportHelpers.h"
#include "RepSpawn.h"
#include "StockAuditTransfer.h"
#include "StockOptionsIni.h"
#include "WrapperErrors.h"
/**********************************************************************/
#include "StockImportBasicWrapperTransferDlg.h"
/**********************************************************************/

CStockImportBasicWrapperTransferDlg::CStockImportBasicWrapperTransferDlg ( int nSpIdx, int nSpIdx2, const char* szPath, const char* szReference1, const char* szReference2, CSuppRefFinder& SuppRefFinder, CWnd* pParent )
	: CStockImportBasicWrapperDlg( nSpIdx, szPath, SuppRefFinder, pParent )
{
	m_nSpIdx2 = nSpIdx2;
	m_strHeaderSpName1 = dbStockpoint.GetName( nSpIdx );
	m_strHeaderSpName2 = dbStockpoint.GetName( nSpIdx2 );
	m_strImport = "Would you like to import these transfers now?";
	m_strTitle = "Import Transfers by Wrapper Code";
	m_strHeaderReference1 = szReference1;
	m_strHeaderReference2 = szReference2;
	m_nImportFileReportType = AUDIT_IMPORT_WRAPPER_TRANSFER;
	m_nStockChangesReportType = AUDIT_TRANSFER;
	m_nExceptionReportType = AUDIT_EX_TRANSFER;
	ProcessImportData( ADD_STOCK_LEVELS );
}

/**********************************************************************/

void CStockImportBasicWrapperTransferDlg::FillActionCombo()
{
	m_comboAction.ResetContent();
	m_comboAction.AddString ( "Add to period transfer level" );
	m_comboAction.AddString ( "Subtract from period transfer level" );
	m_comboAction.SetItemData( 0, ADD_STOCK_LEVELS );
	m_comboAction.SetItemData( 1, SUBTRACT_STOCK_LEVELS );
	m_comboAction.SetCurSel(0);
	m_comboAction.EnableWindow( TRUE );
}

/**********************************************************************/

void CStockImportBasicWrapperTransferDlg::ShowStockChanges() 
{
	CStockAuditTransfer Audit;
	Audit.SetReportType( AUDIT_TRANSFER );
	Audit.SetConLevel ( NODE_STOCKPOINT );
	Audit.SetConLevelText( dbStockpoint.GetName( m_nSpIdx ) );
	Audit.SetTableNo( dbStockpoint.GetSpNo( m_nSpIdx ) );
	Audit.SetAuditMode( AUDIT_MODE_PREVIEW );
		
	CStockAuditHeaderInfo info;
	info.m_date = m_ImportDate;
	info.m_strReference1 = m_strHeaderReference1;
	info.m_strReference2 = m_strHeaderReference2;
	info.m_strSpName1 = m_strHeaderSpName1;
	info.m_strSpName2 = m_strHeaderSpName2;
	Audit.SetPreviewHeader( Audit.GetHeaderLine( info ) );

	if ( Audit.CreateReport( DataManager.GetActiveDbIdx() ) )
	{
		CReportManager ReportManager;
		ReportManager.DisplayReport ( Super.ReportFilename(), Audit.GetReportParamsFilename(), "", this );
	}
}

/**********************************************************************/

void CStockImportBasicWrapperTransferDlg::AuditStockChanges ( CSSFile& file, CSSFile& fileTempReport, CCSV& csvOriginalData, int nStockIdx, int nStockSuppIdx, double dDUQty, double dTotalCost, int nAction )
{
	CStockCSVRecord StockRecord;
	DataManager.Stock.GetAt( nStockIdx, StockRecord );
		
	double dSUQty = dDUQty * StockRecord.GetYield( nStockSuppIdx );
		
	if ( nAction == SUBTRACT_STOCK_LEVELS )
		dSUQty = -dSUQty;
	
	CCSVStockAudit csvOut;
	csvOut.Add ( "D" );
	csvOut.Add ( AUDIT_TRANSFER );
	csvOut.Add ( 2 );
	csvOut.Add ( StockRecord.GetStockCode() );
	csvOut.Add ( StockRecord.GetDescription() );
	csvOut.Add ( DataManager.Category.GetText ( StockRecord.GetCategory() ) );
	csvOut.Add ( DataManager.Stock.GetSubUnits( StockRecord ) );
	csvOut.AddStockAuditQty ( dDUQty );
	csvOut.AddStockAuditQty ( dSUQty );
	file.WriteLine ( csvOut.GetLine() );

	csvOut.RemoveAll();
	csvOut.Add ( "D" );
	csvOut.Add ( AUDIT_IMPORT_WRAPPER_TRANSFER );
	csvOut.Add ( 1 );
	csvOut.Add ( csvOriginalData.GetString(0) );
	csvOut.Add ( csvOriginalData.GetString(1) );
	csvOut.Add ( csvOriginalData.GetString(2) );
	fileTempReport.WriteLine ( csvOut.GetLine() );
}

/**********************************************************************/

int CStockImportBasicWrapperTransferDlg::CheckStockLevelStatus( int nStockIdx )
{
	int nError = WRAPPER_ERROR_NONE;

	CStockCSVRecord StockRecord;
	CStockLevelsCSVRecord StockLevels;
	
	DataManager.Stock.GetAt( nStockIdx, StockRecord );
	DataManager.Stock.GetStockpointTotals( nStockIdx, StockRecord, dbStockpoint.GetSpNo( m_nSpIdx ), StockLevels );

	if ( StockLevels.GetActiveFlag() == FALSE )
	{
		nError = WRAPPER_ERROR_STOCK_INACTIVE2;
	}
	else
	{
		DataManager.Stock.GetStockpointTotals( nStockIdx, StockRecord, dbStockpoint.GetSpNo( m_nSpIdx2 ), StockLevels );
		
		if ( StockLevels.GetActiveFlag() == FALSE )
			nError = WRAPPER_ERROR_STOCK_INACTIVE3;
	}

	return nError;
}

/**********************************************************************/


