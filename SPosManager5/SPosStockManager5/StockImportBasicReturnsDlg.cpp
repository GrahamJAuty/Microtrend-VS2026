/**********************************************************************/
#include "NodeTypes.h"
#include "RepCSV.h"
#include "RepSpawn.h"
#include "StockAuditReturns.h"
#include "StockOptionsIni.h"
/**********************************************************************/
#include "StockImportBasicReturnsDlg.h"
/**********************************************************************/

CStockImportBasicReturnsDlg::CStockImportBasicReturnsDlg( int nSpIdx, int nSuppIdx, const char* szPath, const char* szReference1, const char* szReference2, CStocktakeCSVRecord* pStocktake, CWnd* pParent )
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

	m_strImport = "Would you like to import these returns now?";
	
	if ( DealerFlags.GetImportBarcodeFlag() == TRUE )
		m_strTitle = "Import Returns by Barcode Number";
	else
		m_strTitle = "Import Returns by Plu Number";
	
	m_strHeaderReference1 = szReference1;
	m_strHeaderReference2 = szReference2;
	m_nImportFileReportType = AUDIT_IMPORT_PLU_RETURNS;
	m_nStockChangesReportType = AUDIT_RETURNS;
	m_nExceptionReportType = AUDIT_EX_RETURNS;
	m_nSuppIdx = nSuppIdx;

	if ( m_nSuppIdx >= 0 && m_nSuppIdx < DataManager.Supplier.GetSize() )
		m_strHeaderSuppName = DataManager.Supplier.GetName( m_nSuppIdx );
	else
		m_strHeaderSuppName = "Unspecified";
		
	ProcessImportData( ADD_STOCK_LEVELS );
}

/**********************************************************************/

void CStockImportBasicReturnsDlg::FillActionCombo()
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

void CStockImportBasicReturnsDlg::ShowStockChanges() 
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

void CStockImportBasicReturnsDlg::AuditStockChanges( CSSFile& file, CSSFile& fileTempReport, CStringArray& StockDetails, CCSV& csvOriginalData, int nAction )
{
	for ( int nIndex = 0; nIndex < StockDetails.GetSize(); nIndex++ )
	{
		CCSVExplodedPlu csvReturns ( StockDetails.GetAt ( nIndex ) );
		
		int nStockIdx = csvReturns.GetStockIdx();
		double dStockQty = csvReturns.GetStockQty();

		CStockCSVRecord StockRecord;
		DataManager.Stock.GetAt ( nStockIdx, StockRecord );
		CString strCategory = DataManager.Category.GetText ( StockRecord.GetCategory() );

		double dStockValue = 0.0;
		CString strSuppRef = "";

		if ( StockOptions.GetImportReturnValueFlag() == TRUE )
			dStockValue = csvReturns.GetStockValue();
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
				strSuppRef = StockRecord.GetSuppRefExternal( nStockSuppIdxToUse, DealerFlags.GetSuppRefFlag() );

				if ( StockOptions.GetImportReturnValueFlag() == FALSE )
					dStockValue = dStockQty * StockRecord.GetUnitCost( nStockSuppIdxToUse ); 
			} 
		}

		if ( nAction == SUBTRACT_STOCK_LEVELS )
		{
			dStockQty = -dStockQty;
			dStockValue = -dStockValue;
		}

		CCSVStockAudit csvOut;
		csvOut.Add ( "D" );
		csvOut.Add ( AUDIT_RETURNS );
		csvOut.Add ( 1 );
		csvOut.Add ( StockRecord.GetStockCode() );	//stock code
		csvOut.Add ( StockRecord.GetDescription() );	//description	
		csvOut.Add ( strCategory );				//category
		csvOut.AddStockAuditQty ( 0.0 );			//case qty
		csvOut.AddStockAuditQty ( dStockQty );		//stock unit qty
		csvOut.AddStockAuditVal ( 0.0 );			//case cost
		csvOut.AddStockAuditVal ( dStockValue );	//stock cost
		csvOut.Add ( strSuppRef );				//supplier reference
	
		file.WriteLine ( csvOut.GetLine() );

		if ( nIndex == 0 )
		{
			csvOut.RemoveAll();
			csvOut.Add ( "D" );
			csvOut.Add ( AUDIT_IMPORT_PLU_RETURNS );

			if ( DealerFlags.GetImportBarcodeFlag() == TRUE )
			{
				csvOut.Add ( 2 );
				csvOut.Add ( csvOriginalData.GetString(0) );
				csvOut.Add ( csvOriginalData.GetString(1) );
				csvOut.Add ( csvReturns.GetDescription() );
				csvOut.Add ( csvOriginalData.GetString(3) );
				csvOut.Add ( csvOriginalData.GetString(4) );
			}
			else
			{
				csvOut.Add ( 1 );
				csvOut.Add ( csvOriginalData.GetString(0) );
				csvOut.Add ( csvReturns.GetDescription() );
				csvOut.Add ( csvOriginalData.GetString(2) );
				csvOut.Add ( csvOriginalData.GetString(3) );
			}

			fileTempReport.WriteLine ( csvOut.GetLine() );
		}
	}
}

/**********************************************************************/

