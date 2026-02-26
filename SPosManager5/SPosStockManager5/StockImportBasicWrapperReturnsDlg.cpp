/**********************************************************************/
#include "NodeTypes.h"
#include "RepCSV.h"
#include "RepSpawn.h"
#include "StockAuditReturns.h"
#include "StockOptionsIni.h"
/**********************************************************************/
#include "StockImportBasicWrapperReturnsDlg.h"
/**********************************************************************/

CStockImportBasicWrapperReturnsDlg::CStockImportBasicWrapperReturnsDlg( int nSpIdx, int nSuppIdx, const char* szPath, const char* szReference1, const char* szReference2, CSuppRefFinder& SuppRefFinder, CStocktakeCSVRecord* pStocktake, CWnd* pParent )
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

	m_strImport = "Would you like to import these returns now?";
	m_strTitle = "Import Returns by Wrapper Code";
	m_strHeaderReference1 = szReference1;
	m_strHeaderReference2 = szReference2;
	m_nImportFileReportType = AUDIT_IMPORT_WRAPPER_RETURNS;
	m_nStockChangesReportType = AUDIT_RETURNS;
	m_nExceptionReportType = AUDIT_EX_RETURNS;
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

void CStockImportBasicWrapperReturnsDlg::FillActionCombo()
{
	m_comboAction.ResetContent();
	m_comboAction.AddString ( "Add to period returns level" );
	m_comboAction.AddString ( "Subtract from period returns level" );
	m_comboAction.SetItemData( 0, ADD_STOCK_LEVELS );
	m_comboAction.SetItemData( 1, SUBTRACT_STOCK_LEVELS );
	m_comboAction.SetCurSel(0);
	m_comboAction.EnableWindow( TRUE );
}

/**********************************************************************/

void CStockImportBasicWrapperReturnsDlg::ShowStockChanges() 
{
	CStockAuditReturns Audit;
	Audit.SetReportType ( AUDIT_RETURNS );
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

void CStockImportBasicWrapperReturnsDlg::AuditStockChanges ( CSSFile& file, CSSFile& fileTempReport, CCSV& csvOriginalData, int nStockIdx, int nStockSuppIdx, double dDUQty, double dTotalCost, int nAction )
{
	CStockCSVRecord StockRecord;
	DataManager.Stock.GetAt( nStockIdx, StockRecord );

	CString strCategory = DataManager.Category.GetText ( StockRecord.GetCategory() );
	CString strSuppRef = StockRecord.GetSuppRefExternal ( nStockSuppIdx, DealerFlags.GetSuppRefFlag() );

	double dCostPerCase = 0.0;
	if ( StockOptions.GetImportReturnValueFlag() == FALSE )
	{
		dCostPerCase = StockRecord.GetCost( nStockSuppIdx );
		dTotalCost = dCostPerCase * dDUQty;
	}
	else
		dCostPerCase = ( dDUQty != 0.0 ) ? dTotalCost / dDUQty : 0.0;
			
	double dSUQty = dDUQty * StockRecord.GetYield( nStockSuppIdx );
	int nDUItems = StockRecord.GetDUItems( nStockSuppIdx );
	double dDUItemSize = StockRecord.GetDUItemSize( nStockSuppIdx );
		
	if ( nAction == SUBTRACT_STOCK_LEVELS )
	{
		dDUQty = -dDUQty;
		dSUQty = -dSUQty;
		dTotalCost = -dTotalCost;
	}

	CCSVStockAudit csvOut;
	csvOut.Add ( "D" );
	csvOut.Add ( AUDIT_RETURNS );
	csvOut.Add ( 5 );
	csvOut.Add ( StockRecord.GetStockCode() );		//stock code
	csvOut.Add ( StockRecord.GetDescription() );	//description	
	csvOut.Add ( strCategory );						//category
	csvOut.AddStockAuditQty ( dDUQty );				//case qty
	csvOut.AddStockAuditQty ( dSUQty );				//stock unit qty
	csvOut.AddStockAuditVal ( dCostPerCase );		//case cost
	csvOut.AddStockAuditVal ( dTotalCost );			//stock cost
	csvOut.Add ( strSuppRef );						//supplier reference
	csvOut.Add ( nDUItems );						//items per du
	csvOut.AddStockAuditQty( dDUItemSize );			//stock units per item
	
	file.WriteLine ( csvOut.GetLine() );

	csvOut.RemoveAll();
	csvOut.Add ( "D" );
	csvOut.Add ( AUDIT_IMPORT_WRAPPER_RETURNS );
	csvOut.Add ( 1 );
	csvOut.Add ( csvOriginalData.GetString(0) );
	csvOut.Add ( csvOriginalData.GetString(1) );
	csvOut.Add ( csvOriginalData.GetString(2) );
	fileTempReport.WriteLine ( csvOut.GetLine() );
}

/**********************************************************************/

