/**********************************************************************/
#include "NodeTypes.h"
#include "RepCSV.h"
#include "ReportPluInfoFinder.h"
#include "RepSpawn.h"
#include "SalesHistoryFields.h"
#include "SalesTypes.h"
#include "StockAuditExceptionsSales.h"
#include "StockAuditImportPluSales.h"
#include "StockAuditSales.h"
/**********************************************************************/
#include "StockImportBasicSalesDlg.h"
/**********************************************************************/

CStockImportBasicSalesDlg::CStockImportBasicSalesDlg( int nSpIdx, const char* szPath, int nLineType, CStocktakeCSVRecord* pStocktake, CWnd* pParent )
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

	if ( nLineType < 0 )
		m_nLineType = DealerFlags.GetPluCSVImportType();
	else
		m_nLineType = nLineType;
		
	m_strImport = "Would you like to import these sales now?";
	
	if ( DealerFlags.GetImportBarcodeFlag() == TRUE )
		m_strTitle = "Import Sales by Barcode Number";
	else
		m_strTitle = "Import Sales by Plu Number";
	
	m_nImportFileReportType = AUDIT_IMPORT_PLU_SALES;
	m_nStockChangesReportType = AUDIT_SALES;
	m_nExceptionReportType = AUDIT_EX_SALES;
	ProcessImportData( ADD_STOCK_LEVELS );
}

/**********************************************************************/

void CStockImportBasicSalesDlg::FillActionCombo()
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

void CStockImportBasicSalesDlg::ShowStockChanges() 
{
	CStockAuditSales Audit;
	Audit.SetReportType ( AUDIT_SALES );
	Audit.SetConLevel ( NODE_STOCKPOINT );
	Audit.SetConLevelText( dbStockpoint.GetName( m_nSpIdx ) );
	Audit.SetTableNo( dbStockpoint.GetSpNo( m_nSpIdx ) );
	Audit.SetAuditMode( AUDIT_MODE_PREVIEW );
	
	CStockAuditHeaderInfo info;
	info.m_date = m_ImportDate;
	Audit.SetPreviewHeader( Audit.GetHeaderLine( info ) );

	Audit.SetManualSalesFlag( TRUE );
	Audit.SetImportSalesFlag( TRUE );
	Audit.SetE1SalesFlag( FALSE );

	if ( Audit.CreateReport( DataManager.GetActiveDbIdx() ) )
	{
		CReportManager ReportManager;
		ReportManager.DisplayReport ( Super.ReportFilename(), Audit.GetReportParamsFilename(), "", this );
	}
}

/**********************************************************************/

void CStockImportBasicSalesDlg::ShowExceptions()
{
	CStockAuditExceptionsSales Audit;
	Audit.SetReportType ( AUDIT_EX_SALES );
	Audit.SetConLevel ( NODE_STOCKPOINT );
	Audit.SetConLevelText( dbStockpoint.GetName( m_nSpIdx ) );
	Audit.SetTableNo( dbStockpoint.GetSpNo( m_nSpIdx ) );
	Audit.SetAuditMode( AUDIT_MODE_PREVIEW );
	
	CStockAuditHeaderInfo info;
	info.m_date = m_ImportDate;
	Audit.SetPreviewHeader( Audit.GetHeaderLine( info ) );

	Audit.SetImportSalesFlag( TRUE );
	Audit.SetE1SalesFlag( FALSE );
	
	if ( Audit.CreateReport( DataManager.GetActiveDbIdx() ) )
	{
		CReportManager ReportManager;
		ReportManager.DisplayReport ( Super.ReportFilename(), Audit.GetReportParamsFilename(), "", this );
	}
}

/**********************************************************************/

void CStockImportBasicSalesDlg::ShowImportFile()
{
	CStockAuditImportPluSales Audit( m_nLineType );
	
	Audit.SetReportType ( m_nImportFileReportType );
	Audit.SetConLevel ( NODE_STOCKPOINT );			
	Audit.SetTableNo ( dbStockpoint.GetSpNo( m_nSpIdx ) );
	Audit.SetAuditMode ( AUDIT_MODE_IMPORT );
	
	CStockAuditHeaderInfo info;
	info.m_date = m_ImportDate;
	info.m_strSpName1 = m_strHeaderSpName1;
	info.m_strSpName2 = m_strHeaderSpName2;	
	info.m_strImportPath = m_strHeaderImportPath;
	Audit.SetPreviewHeader ( Audit.GetHeaderLine( info ) );

	if ( Audit.CreateReport( DataManager.GetActiveDbIdx() ) )
	{
		CReportManager ReportManager;
		ReportManager.DisplayReport ( Super.ReportFilename(), Audit.GetReportParamsFilename(), "", this );
	}
}

/**********************************************************************/

void CStockImportBasicSalesDlg::GetImportLineInfo( CCSV& csvImport, CString& strPluNo, CCSV& csvOriginalData, double& dPluQty, double& dPluValue )
{
	csvOriginalData.RemoveAll();
		
	if ( DealerFlags.GetImportBarcodeFlag() == TRUE )
	{
		CString strBarcodeNo = csvImport.GetString(0);
		__int64 nPluNo = DataManager.Barcode.GetPluFromBarcode( _atoi64 ( strBarcodeNo ) );
		
		::FormatPluNo( nPluNo, strPluNo );
		
		csvImport.SetAt( 0, strPluNo );
		CReportPluInfoFinder PluInfo ( csvImport, m_nLineType );
		dPluQty = PluInfo.GetImportLineQty();
		dPluValue = PluInfo.GetImportLineVal();

		CString strOriginalPluNo = strPluNo;
		::TrimSpaces( strOriginalPluNo, TRUE );

		csvOriginalData.Add( strBarcodeNo );
		csvOriginalData.Add( strOriginalPluNo );
	}
	else
	{
		strPluNo = csvImport.GetString ( 0 );
		::AddLeading ( strPluNo, Super.MaxPluLen(), '0' );

		CReportPluInfoFinder PluInfo ( csvImport, m_nLineType );
		dPluQty = PluInfo.GetImportLineQty();
		dPluValue = PluInfo.GetImportLineVal();

		csvOriginalData.Add( csvImport.GetString(0) );
	}

	switch( m_nLineType )
	{
	case 1:
		csvOriginalData.Add( "" );
		csvOriginalData.Add( csvImport.GetString(1) );
		csvOriginalData.Add( "" );
		break;

	case 2:
		csvOriginalData.Add( csvImport.GetString(1) );
		csvOriginalData.Add( csvImport.GetString(2) );
		csvOriginalData.Add( csvImport.GetString(3) );
		break;

	case 3:
		csvOriginalData.Add( csvImport.GetString(1) );
		csvOriginalData.Add( csvImport.GetString(2) );
		csvOriginalData.Add( "" );
		break;
	
	case 0:	
	default:
		csvOriginalData.Add( "" );
		csvOriginalData.Add( csvImport.GetString(1) );
		csvOriginalData.Add( csvImport.GetString(2) );
		break;
	}
}

/**********************************************************************/

void CStockImportBasicSalesDlg::AuditStockChanges( CSSFile& file, CSSFile& fileTempReport, CStringArray& StockDetails, CCSV& csvOriginalData, int nAction )
{	
	for ( int nIndex = 0; nIndex < StockDetails.GetSize(); nIndex++ )
	{
		CCSVExplodedPlu csvSale ( StockDetails.GetAt ( nIndex ) );
		
		double dPluQty = csvSale.GetPluQty();
		double dPluValue = csvSale.GetPluValue();
		double dStockQty = csvSale.GetStockQty();
		double dStockValue = csvSale.GetStockValue();

		if ( nAction == SUBTRACT_STOCK_LEVELS )
		{
			dPluQty = -dPluQty;
			dPluValue = -dPluValue;
			dStockQty = -dStockQty;
			dStockValue = -dStockValue;
		}

		CCSVStockAudit csvOut;
		csvOut.Add ( "D" );
		csvOut.Add ( AUDIT_SALES );
		csvOut.Add ( 1 );
		csvOut.Add ( ( nIndex == 0 ) ? SALES_TYPE_IMPORT : SALES_TYPE_RECIPEITEM);
		csvOut.Add ( csvSale.GetPluNo() );
		csvOut.Add ( "" );
		csvOut.Add ( csvSale.GetDescription() );
		csvOut.AddStockAuditQty ( dPluQty );
		csvOut.AddStockAuditVal ( dPluValue );
		csvOut.Add ( csvSale.GetStockCode() );
		csvOut.Add ( csvSale.GetSubUnits() );
		csvOut.AddStockAuditQty ( dStockQty );
		csvOut.AddStockAuditVal ( dStockValue );
		file.WriteLine ( csvOut.GetLine() );

		if ( nIndex == 0 )
		{
			csvOut.RemoveAll();
			csvOut.Add ( "D" );
			csvOut.Add ( AUDIT_IMPORT_PLU_SALES );

			if ( DealerFlags.GetImportBarcodeFlag() == TRUE )
			{
				csvOut.Add ( 2 );
				csvOut.Add ( csvOriginalData.GetString(0) );
				csvOut.Add ( csvOriginalData.GetString(1) );
				csvOut.Add ( csvSale.GetDescription() );
				csvOut.Add ( csvOriginalData.GetString(2) );
				csvOut.Add ( csvOriginalData.GetString(3) );
				csvOut.Add ( csvOriginalData.GetString(4) );
			}
			else
			{
				csvOut.Add ( 1 );
				csvOut.Add ( csvOriginalData.GetString(0) );
				csvOut.Add ( csvSale.GetDescription() );
				csvOut.Add ( csvOriginalData.GetString(1) );
				csvOut.Add ( csvOriginalData.GetString(2) );
				csvOut.Add ( csvOriginalData.GetString(3) );
			}

			fileTempReport.WriteLine ( csvOut.GetLine() );
		}
	}
}

/**********************************************************************/

void CStockImportBasicSalesDlg::AuditExceptions( CSSFile& file, CSSFile& fileTempReport, CStringArray& StockDetails, CCSV& csvOriginalData )
{
	for ( int nIndex = 0; nIndex < StockDetails.GetSize(); nIndex++ )
	{
		CCSVExplodedPlu csvProblem ( StockDetails.GetAt ( nIndex ) );
		
		CString strProblem = csvProblem.GetProblem();
		__int64 nPluOrBarcodeNo = csvProblem.GetPluNo();

		if ( DealerFlags.GetImportBarcodeFlag() == TRUE )
		{
			int nBarcodeIdx;
			__int64 nBarcodeNo = csvOriginalData.GetInt64(0);
			if ( DataManager.Barcode.FindBarcodeByNumber( nBarcodeNo, nBarcodeIdx ) == FALSE )
			{
				strProblem = "Barcode not found";
				nPluOrBarcodeNo = nBarcodeNo;
			}
		}

		CCSVStockAudit csvOut;
		csvOut.Add ( "D" );
		csvOut.Add ( AUDIT_EX_SALES );
		csvOut.Add ( 2 );
		csvOut.Add ( SALES_TYPE_IMPORT );
		csvOut.Add ( nPluOrBarcodeNo );
		csvOut.Add ( "" );
		csvOut.AddStockAuditQty ( csvProblem.GetPluQty() );
		csvOut.AddStockAuditVal ( csvProblem.GetPluValue() );		
		csvOut.Add ( csvProblem.GetStockCode() );
		csvOut.Add ( strProblem );
		csvOut.Add ( "" );		//IGNORE SALES DATE
		csvOut.Add ( "" );		//IGNORE SALES TIME
		csvOut.Add ( FALSE );	//NOT WRAPPER
		file.WriteLine( csvOut.GetLine() );

		csvOut.RemoveAll();
		csvOut.Add ( "D" );
		csvOut.Add ( AUDIT_IMPORT_PLU_SALES );
		
		if ( DealerFlags.GetImportBarcodeFlag() == TRUE )
		{
			csvOut.Add ( 2 );
			csvOut.Add ( csvOriginalData.GetString(0) );
			csvOut.Add ( csvOriginalData.GetString(1) );
			csvOut.Add ( csvProblem.GetDescription() );
			csvOut.Add ( csvOriginalData.GetString(2) );
			csvOut.Add ( csvOriginalData.GetString(3) );
			csvOut.Add ( csvOriginalData.GetString(4) );
			csvOut.Add ( strProblem );
		}
		else
		{
			csvOut.Add ( 1 );
			csvOut.Add ( csvOriginalData.GetString(0) );
			csvOut.Add ( csvProblem.GetDescription() );
			csvOut.Add ( csvOriginalData.GetString(1) );
			csvOut.Add ( csvOriginalData.GetString(2) );
			csvOut.Add ( csvOriginalData.GetString(3) );
			csvOut.Add ( strProblem );
		}

		fileTempReport.WriteLine ( csvOut.GetLine() );
	}
}

/**********************************************************************/

void CStockImportBasicSalesDlg::AuditIgnored ( CSSFile& fileTempReport, CStringArray& StockDetails, CCSV& csvOriginalData )
{	
	for ( int nIndex = 0; nIndex < StockDetails.GetSize(); nIndex++ )
	{
		CCSVExplodedPlu csvProblem ( StockDetails.GetAt ( nIndex ) );
		
		CCSV csvOut;
		csvOut.Add ( "D" );
		csvOut.Add ( m_nImportFileReportType );
		csvOut.Add ( 1 );
		csvOut.Add ( csvOriginalData.GetString(0) );
		csvOut.Add ( csvProblem.GetDescription() );
		csvOut.Add ( csvOriginalData.GetString(1) );
		csvOut.Add ( csvOriginalData.GetString(2) );
		csvOut.Add ( csvOriginalData.GetString(3) );		
		csvOut.Add ( csvProblem.GetProblem() );
		fileTempReport.WriteLine ( csvOut.GetLine() );
	}
}

/**********************************************************************/

