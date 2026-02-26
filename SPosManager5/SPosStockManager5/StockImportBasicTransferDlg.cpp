/**********************************************************************/
#include "NodeTypes.h"
#include "RepCSV.h"
#include "RepSpawn.h"
#include "StockAuditTransfer.h"
/**********************************************************************/
#include "StockImportBasicTransferDlg.h"
/**********************************************************************/

CStockImportBasicTransferDlg::CStockImportBasicTransferDlg( int nSpIdx, int nSpIdx2, const char* szPath, const char* szReference1, const char* szReference2, CWnd* pParent )
	: CStockImportBasicDlg ( nSpIdx, szPath, pParent ) 
{
	m_nSpIdx2 = nSpIdx2;

	m_strHeaderSpName1 = dbStockpoint.GetName( nSpIdx );
	m_strHeaderSpName2 = dbStockpoint.GetName( nSpIdx2 );
	m_strHeaderReference1 = szReference1;
	m_strHeaderReference2 = szReference2;
	m_strImport = "Would you like to import stock transfers now?";
	
	if ( DealerFlags.GetImportBarcodeFlag() == TRUE )
		m_strTitle = "Import Stock Transfers by Barcode Number";
	else
		m_strTitle = "Import Stock Transfers by Plu Number";
		
	m_nImportFileReportType = AUDIT_IMPORT_PLU_TRANSFER;
	m_nStockChangesReportType = AUDIT_TRANSFER;
	m_nExceptionReportType = AUDIT_EX_TRANSFER;
	ProcessImportData( ADD_STOCK_LEVELS );
}

/**********************************************************************/

void CStockImportBasicTransferDlg::FillActionCombo()
{
	m_comboAction.ResetContent();
	m_comboAction.AddString ( "Add to period stock transfer level" );
	m_comboAction.AddString ( "Subtract from period stock transfer level" );
	m_comboAction.SetItemData( 0, ADD_STOCK_LEVELS );
	m_comboAction.SetItemData( 1, SUBTRACT_STOCK_LEVELS );
	m_comboAction.SetCurSel(0);
	m_comboAction.EnableWindow( TRUE );
}

/**********************************************************************/

void CStockImportBasicTransferDlg::ShowStockChanges() 
{
	CStockAuditTransfer Audit;
	Audit.SetReportType ( AUDIT_TRANSFER );
	Audit.SetConLevel ( NODE_STOCKPOINT );
	Audit.SetConLevelText( dbStockpoint.GetName( m_nSpIdx ) );
	Audit.SetTableNo( dbStockpoint.GetSpNo( m_nSpIdx ) );
	Audit.SetAuditMode( AUDIT_MODE_PREVIEW );
	
	CStockAuditHeaderInfo info;
	info.m_date = m_ImportDate;
	info.m_strSpName1 = m_strHeaderSpName1;
	info.m_strSpName2 = m_strHeaderSpName2;	
	info.m_strReference1 = m_strHeaderReference1;
	info.m_strReference2 = m_strHeaderReference2;
	Audit.SetPreviewHeader( Audit.GetHeaderLine( info ) );

	if ( Audit.CreateReport( DataManager.GetActiveDbIdx() ) )
	{
		CReportManager ReportManager;
		ReportManager.DisplayReport ( Super.ReportFilename(), Audit.GetReportParamsFilename(), "", this );
	}
}

/**********************************************************************/

void CStockImportBasicTransferDlg::AuditStockChanges( CSSFile& file, CSSFile& fileTempReport, CStringArray& StockDetails, CCSV& csvOriginalData, int nAction )
{
	for ( int nIndex = 0; nIndex < StockDetails.GetSize(); nIndex++ )
	{
		CCSVExplodedPlu csvTransfer ( StockDetails.GetAt ( nIndex ) );
			
		int nStockIdx = csvTransfer.GetStockIdx();
		CStockCSVRecord StockRecord;
		DataManager.Stock.GetAt ( nStockIdx, StockRecord );
	
		int nSpNo = dbStockpoint.GetSpNo( m_nSpIdx );

		double dQty = csvTransfer.GetStockQty();
		if ( nAction == SUBTRACT_STOCK_LEVELS )
			dQty = -dQty;

		CCSVStockAudit csvOut;
		csvOut.Add ( "D" );
		csvOut.Add ( AUDIT_TRANSFER );
		csvOut.Add ( 1 );
		csvOut.Add ( StockRecord.GetStockCode() );
		csvOut.Add ( StockRecord.GetDescription() );
		csvOut.Add ( DataManager.Category.GetText ( StockRecord.GetCategory() ) );
		csvOut.Add ( DataManager.Stock.GetSubUnits( StockRecord ) );
		csvOut.AddStockAuditQty ( dQty );
		file.WriteLine ( csvOut.GetLine() );

		if ( nIndex == 0 )
		{
			csvOut.RemoveAll();
			csvOut.Add ( "D" );
			csvOut.Add ( AUDIT_IMPORT_PLU_TRANSFER );
			
			if ( DealerFlags.GetImportBarcodeFlag() == TRUE )
			{
				csvOut.Add ( 2 );
				csvOut.Add ( csvOriginalData.GetString(0) );
				csvOut.Add ( csvOriginalData.GetString(1) );
				csvOut.Add ( csvTransfer.GetDescription() );
				csvOut.Add ( csvOriginalData.GetString(3) );
				csvOut.Add ( csvOriginalData.GetString(4) );
			}
			else
			{
				csvOut.Add ( 1 );
				csvOut.Add ( csvOriginalData.GetString(0) );
				csvOut.Add ( csvTransfer.GetDescription() );
				csvOut.Add ( csvOriginalData.GetString(2) );
				csvOut.Add ( csvOriginalData.GetString(3) );
			}

			fileTempReport.WriteLine ( csvOut.GetLine() );
		}
	}
}

/**********************************************************************/

