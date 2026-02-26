/**********************************************************************/
#include "NodeTypes.h"
#include "RepCSV.h"
#include "ReportPluInfoFinder.h"
#include "RepSpawn.h"
#include "SalesHistoryFields.h"
#include "SalesTypes.h"
#include "StockAuditAdjustmentPlu.h"
#include "StockAuditExceptionsAdjustmentPlu.h"
#include "StockAuditImportAdjustmentPlu.h"
#include "StockAuditSales.h"
/**********************************************************************/
#include "StockImportBasicAdjustmentPluDlg.h"
/**********************************************************************/

CStockImportBasicAdjustmentPluDlg::CStockImportBasicAdjustmentPluDlg( int nSpIdx, const char* szPath, int nLineType, const char* szReason, const char* szReference1, const char* szReference2, CStocktakeCSVRecord* pStocktake, CWnd* pParent )
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
		
	m_strImport = "Would you like to import these adjustments now?";
	
	if ( DealerFlags.GetImportBarcodeFlag() == TRUE )
		m_strTitle = "Import Adjustments by Barcode Number";
	else
		m_strTitle = "Import Adjustments by Plu Number";
	
	m_strReason = szReason;
	m_strHeaderReference1 = szReference1;
	m_strHeaderReference2 = szReference2;

	m_nImportFileReportType = AUDIT_IMPORT_PLU_ADJUSTMENT_PLU;
	m_nStockChangesReportType = AUDIT_ADJUSTMENT_PLU;
	m_nExceptionReportType = AUDIT_EX_ADJUSTMENT_PLU;
	ProcessImportData( ADD_STOCK_LEVELS );
}

/**********************************************************************/

void CStockImportBasicAdjustmentPluDlg::FillActionCombo()
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

void CStockImportBasicAdjustmentPluDlg::ShowStockChanges() 
{
	CStockAuditAdjustmentPlu Audit;
	Audit.LockDatabase();
	Audit.SetReportType ( AUDIT_ADJUSTMENT_PLU );
	Audit.SetConLevel ( NODE_STOCKPOINT );
	Audit.SetConLevelText( dbStockpoint.GetName( m_nSpIdx ) );
	Audit.SetTableNo( dbStockpoint.GetSpNo( m_nSpIdx ) );
	Audit.SetAuditMode( AUDIT_MODE_PREVIEW );
	
	CStockAuditHeaderInfo info;
	info.m_date = m_ImportDate;
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

void CStockImportBasicAdjustmentPluDlg::ShowExceptions()
{
	CStockAuditExceptionsAdjustmentPlu Audit;
	Audit.SetReportType ( AUDIT_EX_ADJUSTMENT_PLU );
	Audit.SetConLevel ( NODE_STOCKPOINT );
	Audit.SetConLevelText( dbStockpoint.GetName( m_nSpIdx ) );
	Audit.SetTableNo( dbStockpoint.GetSpNo( m_nSpIdx ) );
	Audit.SetAuditMode( AUDIT_MODE_PREVIEW );
	
	CStockAuditHeaderInfo info;
	info.m_date = m_ImportDate;
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

void CStockImportBasicAdjustmentPluDlg::ShowImportFile()
{
	CStockAuditImportAdjustmentPlu Audit( m_nLineType );
	
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

void CStockImportBasicAdjustmentPluDlg::GetImportLineInfo( CCSV& csvImport, CString& strPluNo, CCSV& csvOriginalData, double& dPluQty, double& dPluValue )
{
	csvOriginalData.RemoveAll();
		
	if ( DealerFlags.GetImportBarcodeFlag() == TRUE )
	{
		CString strBarcodeNo = csvImport.GetString(0);
		__int64 nBarcodeNo = _atoi64( strBarcodeNo );
		__int64 nPluNo = DataManager.Barcode.GetPluFromBarcode( nBarcodeNo );
		
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

void CStockImportBasicAdjustmentPluDlg::AuditStockChanges( CSSFile& file, CSSFile& fileTempReport, CStringArray& StockDetails, CCSV& csvOriginalData, int nAction )
{	
	for ( int nIndex = 0; nIndex < StockDetails.GetSize(); nIndex++ )
	{
		CCSVExplodedPlu csvAdjust ( StockDetails.GetAt ( nIndex ) );
		
		double dPluQty = csvAdjust.GetPluQty();
		double dPluValue = csvAdjust.GetPluValue();
		double dStockQty = csvAdjust.GetStockQty();
		double dStockValue = csvAdjust.GetStockValue();

		if ( nAction == SUBTRACT_STOCK_LEVELS )
		{
			dPluQty = -dPluQty;
			dPluValue = -dPluValue;
			dStockQty = -dStockQty;
			dStockValue = -dStockValue;
		}

		CCSVStockAudit csvOut;
		csvOut.Add ( "D" );
		csvOut.Add ( AUDIT_ADJUSTMENT_PLU );
		csvOut.Add ( 1 );
		csvOut.Add ( ( nIndex == 0 ) ? 0 : 1 );
		csvOut.Add ( csvAdjust.GetPluNo() );
		csvOut.Add ( csvAdjust.GetDescription() );
		csvOut.AddStockAuditQty ( dPluQty );
		csvOut.AddStockAuditVal ( dPluValue );
		csvOut.Add ( csvAdjust.GetStockCode() );
		csvOut.Add ( csvAdjust.GetSubUnits() );
		csvOut.AddStockAuditQty ( dStockQty );
		csvOut.AddStockAuditVal ( dStockValue );
		csvOut.Add( m_strReason );
		file.WriteLine ( csvOut.GetLine() );

		if ( nIndex == 0 )
		{
			csvOut.RemoveAll();
			csvOut.Add ( "D" );
			csvOut.Add ( AUDIT_IMPORT_PLU_ADJUSTMENT_PLU );

			if ( DealerFlags.GetImportBarcodeFlag() == TRUE )
			{
				csvOut.Add ( 2 );
				csvOut.Add ( csvOriginalData.GetString(0) );
				csvOut.Add ( csvOriginalData.GetString(1) );
				csvOut.Add ( csvAdjust.GetDescription() );
				csvOut.Add ( csvOriginalData.GetString(2) );
				csvOut.Add ( csvOriginalData.GetString(3) );
				csvOut.Add ( csvOriginalData.GetString(4) );
			}
			else
			{
				csvOut.Add ( 1 );
				csvOut.Add ( csvOriginalData.GetString(0) );
				csvOut.Add ( csvAdjust.GetDescription() );
				csvOut.Add ( csvOriginalData.GetString(1) );
				csvOut.Add ( csvOriginalData.GetString(2) );
				csvOut.Add ( csvOriginalData.GetString(3) );
			}

			fileTempReport.WriteLine ( csvOut.GetLine() );
		}
	}
}

/**********************************************************************/

void CStockImportBasicAdjustmentPluDlg::AuditExceptions( CSSFile& file, CSSFile& fileTempReport, CStringArray& StockDetails, CCSV& csvOriginalData )
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
		csvOut.Add ( AUDIT_EX_ADJUSTMENT_PLU );
		csvOut.Add ( 1 );
		csvOut.Add ( nPluOrBarcodeNo );
		csvOut.AddStockAuditQty ( csvProblem.GetPluQty() );
		csvOut.AddStockAuditVal ( csvProblem.GetPluValue() );		
		csvOut.Add ( m_strReason );
		csvOut.Add ( csvProblem.GetStockCode() );
		csvOut.Add ( strProblem );
		file.WriteLine( csvOut.GetLine() );

		csvOut.RemoveAll();
		csvOut.Add ( "D" );
		csvOut.Add ( AUDIT_IMPORT_PLU_ADJUSTMENT_PLU );
		
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

void CStockImportBasicAdjustmentPluDlg::AuditIgnored ( CSSFile& fileTempReport, CStringArray& StockDetails, CCSV& csvOriginalData )
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

