/**********************************************************************/
#include "CCSVReportLine.h"
#include "ExplodePlu.h"
#include "RepCSV.h"
#include "ReportHelpers.h"
#include "StockAuditBaseReport.h"
#include "StockOptionsIni.h"
/**********************************************************************/
#include "RecipeProfitSheet.h"
/**********************************************************************/

CRecipeProfitSheet::CRecipeProfitSheet()
{
}

/**********************************************************************/

int CRecipeProfitSheet::CreateProfitSheet( int nStockIdx )
{
	CStockCSVRecord StockRecord;
	DataManager.Stock.GetAt( nStockIdx, StockRecord );
	CString strStockCode = StockRecord.GetStockCode();

	CString strTitle;
	strTitle.Format ( "Stock Unit GP Info for %s, %s",
		(const char*) strStockCode,
		StockRecord.GetDescription() );

	CReportFile ReportFile;
	if ( ReportFile.Open( Super.ReportFilename() ) == FALSE )
		return 2;

	ReportFile.SetStyle1 ( strTitle );

	ReportFile.AddColumn ( "Stock Code", TA_LEFT, 400 );
	ReportFile.AddColumn ( "Description", TA_LEFT, 350 );
	ReportFile.AddColumn ( "SU Qty", TA_RIGHT, 250 );
	ReportFile.AddColumn ( "SU Qty Cost", TA_RIGHT, 250 );
	ReportFile.AddColumn ( "%Cost", TA_RIGHT, 250 );
	ReportFile.AddColumn ( "Plu Retail", TA_RIGHT, 250 );
	ReportFile.AddColumn ( "SU Qty Retail", TA_RIGHT, 250 );
	ReportFile.AddColumn ( "SU Retail", TA_RIGHT, 250 );
	ReportFile.AddColumn ( "SU Cost", TA_RIGHT, 250 );
	ReportFile.AddColumn ( EcrmanOptions.GetTaxString( "SU %T" ), TA_RIGHT, 250 );
	ReportFile.AddColumn ( "SU GP", TA_RIGHT, 250 );
	
	int nStockPluIdx;
	if ( DataManager.StockPlu.FindStockCode( strStockCode, nStockPluIdx ) == FALSE )
		return 1;

	CStockPluRecord StockPluRecord;
	DataManager.StockPlu.GetAt( nStockPluIdx, StockPluRecord );

	int nRedirectionCount = StockPluRecord.GetPluNoCount();
	int nRecipeCount = StockPluRecord.GetRecipePluNoCount();

	if ( nRedirectionCount == 0 && nRecipeCount == 0 )
		return 1;

	for ( int n = 0; n < nRedirectionCount; n++ )
	{
		__int64 nPluNo = StockPluRecord.GetPluNo(n);
		BreakdownPluNo( ReportFile, nPluNo );
		ReportFile.WriteLine( "" );
		ReportFile.WriteLine( "" );
	}

	for ( int n = 0; n < nRecipeCount; n++ )
	{
		__int64 nPluNo = StockPluRecord.GetRecipePluNo(n);
		BreakdownPluNo( ReportFile, nPluNo );
		ReportFile.WriteLine( "" );
		ReportFile.WriteLine( "" );
	}

	ReportFile.WriteLine( "" );
	return 0;
}

/**********************************************************************/
//IN ORDER TO MAKE THIS REPORT UNAMBIGUOUS, WE WILL USE THE FULL TEXT
//FOR PLU NUMBERS AND STOCK CODES AND OUTPUT THE STOCK UNIT QTY TO 3
//DECIMAL PLACES IRRESPECTIVE OF WHETHER SUB UNITS ARE IN USE
/**********************************************************************/

void CRecipeProfitSheet::BreakdownPluNo( CReportFile& ReportFile, __int64 nPluNo )
{
	CString strPluNo;
	::FormatPluNo( nPluNo, strPluNo );

	int nPluIdx;
	if ( DataManager.Plu.FindPluByNumber( nPluNo, nPluIdx ) == FALSE )
	{	
		CString strHeader;
		strHeader.Format ( "<..>PLU %s, Unknown Plu",
			strPluNo );

		ReportFile.WriteLine( strHeader );
		ReportFile.WriteLine( "<LI>" );
		return;
	}

	CPluCSVRecord PluRecord;
	DataManager.Plu.GetAt( nPluIdx, PluRecord );

	CString strHeader;
	strHeader.Format ( "<..>PLU %s, %s",
		strPluNo,
		ReportHelpers.GetReportText( PluRecord ) );

	ReportFile.WriteLine( strHeader );
	ReportFile.WriteLine( "<LI>" );

	CRecipeCSVRecord RecipeRecord;
	DataManager.Plu.GetRecipe( nPluIdx, nPluNo, RecipeRecord );
	
	if ( RecipeRecord.GetSize() == 0 )
		return;

	if ( RecipeRecord.GetPluStockLinkFlag() == FALSE )
		return;

	CTaxRateInfo TaxRateInfo;
	DataManager.TaxRates.GetTaxRateInfo( PluRecord.GetBaseTaxBandInt(), TaxRateInfo );

	double dTotalCost = DataManager.Plu.GetRecipeCost( RecipeRecord );
	double dTotalRetail = PluRecord.GetPrice(0);
	double dTotalTax = dTotalRetail - ReportHelpers.CalcNonTax( dTotalRetail, TaxRateInfo.m_dTaxRate );

	for ( int n = 0; n < RecipeRecord.GetSize(); n++ )
	{
		CString strStockCode = RecipeRecord.GetStockCode(n);
		double dStockQty = RecipeRecord.GetStockQty(n);

		int nStockIdx;
		if ( DataManager.Stock.FindStockItemByCode( strStockCode, nStockIdx ) == FALSE )
			continue;

		CStockCSVRecord StockRecord;
		DataManager.Stock.GetAt( nStockIdx, StockRecord );
		double dSUCost = StockRecord.GetUnitCost(0);
		double dLineCost = dStockQty * dSUCost;

		double dLinePercent;
		if ( dTotalCost != 0 )
			dLinePercent = ( dLineCost * 100.0 ) / dTotalCost;
		else
			dLinePercent = 100.0 / RecipeRecord.GetSize();

		double dLineRetail = ( dTotalRetail * dLinePercent ) / 100.0;
		
		double dSURetail;
		if ( dStockQty != 0.0 )
			dSURetail = dLineRetail / dStockQty;
		else
			dSURetail = 0.0;

		double dSUTax;
		if ( dStockQty != 0.0 )
			dSUTax = ( dTotalTax * dLinePercent ) / ( 100.0 * dStockQty );
		else
			dSUTax = 0.0;

		CStockReportLine csv;
		csv.AppendString( strStockCode );
		csv.AppendString( StockRecord.GetDescription() );
		csv.AppendStockQty( dStockQty, 1, TRUE, 3 );
		csv.AppendVal( dLineCost );
		csv.AppendVal( dLinePercent, 2 );
		csv.AppendVal( dTotalRetail );
		csv.AppendVal( dLineRetail );
		csv.AppendVal( dSURetail );
		csv.AppendVal( dSUCost );
		csv.AppendVal( dSUTax );
		csv.AppendVal( dSURetail - dSUCost - dSUTax );

		ReportFile.WriteLine( csv.GetLine() );
	}
}

/**********************************************************************/
