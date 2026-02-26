/**********************************************************************/
 
/**********************************************************************/
//#include "DataManager.h"
//#include "DealerFlags.h"
#include "ExplodePlu.h"
//#include "globalfunctions.h"
#include "RepCSV.h"
#include "ReportHelpers.h"
#include "StockAuditBaseReport.h"
#include "StockReportNames.h"
/**********************************************************************/
#include "StockTreeReportNewProfit.h"
/**********************************************************************/

CStockTreeReportNewProfit::CStockTreeReportNewProfit( CStockLevelTreeNew& StockLevelTree ) : CStockTreeReportNew( StockLevelTree )
{
	m_nReportParamsType = STOCK_PROFIT;
	m_nTotalisersPerLine = 0;
}

/**********************************************************************/

void CStockTreeReportNewProfit::PrepareReport()
{
	m_strReportName = GetReportTitle();

	m_arrayColumnNames.RemoveAll();
	m_arrayTotalPos.RemoveAll();

	AddStockCodeColumns( FALSE );
	AddReportColumns();
}

/**********************************************************************/

void CStockTreeReportNewProfit::AddReportColumns()
{
	AddColumn ( "Plu No", TA_LEFT, 350, FALSE );
	AddColumn ( "Reference", TA_LEFT, 350, FALSE );
	AddColumn ( "SU Cost", TA_RIGHT, 180, FALSE );
	
	if ( m_bSubUnits == TRUE )
		AddColumn ( "SU Waste", TA_RIGHT, 220, FALSE );
	
	AddColumn ( "SU Retail", TA_RIGHT, 180, FALSE );
	AddColumn ( EcrmanOptions.GetTaxBandString(), TA_RIGHT, 180, FALSE );
	AddColumn ( "GP", TA_RIGHT, 180, FALSE );
	AddColumn ( "GP%", TA_RIGHT, 180, FALSE );	
}

/**********************************************************************/

void CStockTreeReportNewProfit::CreateReportLine( CStockLevelsCSVRecord& StockLevels )
{	
	CWordArray arrayStockSuppIdx;

	if ( m_nLabelSuppIdx == 0 )
	{
		arrayStockSuppIdx.Add( 0 );
	}
	else if ( m_nLabelSuppIdx >= 1 )
	{
		int nSuppNo = DataManager.Supplier.GetSuppNo( m_nLabelSuppIdx - 1 );

		for ( int n = 0; n < m_LabelStockRecord.GetSupplierCount(); n++ )
			if ( m_LabelStockRecord.GetSuppNo( n ) == nSuppNo )
				arrayStockSuppIdx.Add( n );
	}

	for ( int n = 0; n < arrayStockSuppIdx.GetSize(); n++ )
		ProcessStockRecord ( arrayStockSuppIdx.GetAt(n) );
}

/**********************************************************************/

void CStockTreeReportNewProfit::ProcessStockRecord( int nStockSuppIdx )
{
	int nStockPluIdx;
	CString strStockCode = m_LabelStockRecord.GetStockCode();	
	if ( DataManager.StockPlu.FindStockCode ( strStockCode, nStockPluIdx ) == FALSE )
		return;

	bool bGotFirstLine = FALSE;
	for ( int nListPos = 0; nListPos < DataManager.StockPlu.GetPluNoCount ( nStockPluIdx ); nListPos++ )
	{
		__int64 nPluNo = DataManager.StockPlu.GetPluNo ( nStockPluIdx, nListPos );
			
		int nPluIdx;
		if ( DataManager.Plu.FindPluByNumber ( nPluNo, nPluIdx ) == FALSE )
			continue;

		CPluCSVRecord PluRecord;
		DataManager.Plu.GetAt ( nPluIdx, PluRecord );

		double dStockQty;
		DataManager.Plu.GetPluStockQty ( PluRecord.GetPluNoInt(), dStockQty );
			
		CString strSuppRef = m_LabelStockRecord.GetSuppRefExternal( nStockSuppIdx, DealerFlags.GetSuppRefFlag() );

		double dStockCost = m_LabelStockRecord.GetUnitCost ( nStockSuppIdx );
		double dStockWaste = EstimateWastageFromSales( dStockCost );
		
		double dPluCost = dStockQty * dStockCost;
		double dPluWaste = dStockQty * dStockWaste;
		double dPluCostAndWaste = dPluCost + dPluWaste;

		CTaxRateInfo TaxRateInfo;
		DataManager.TaxRates.GetTaxRateInfo( PluRecord.GetBaseTaxBandInt(), TaxRateInfo );

		double dPluRetail = PluRecord.GetPrice( 0 ); 
		double dPluExTax = ReportHelpers.CalcNonTax ( dPluRetail, TaxRateInfo.m_dTaxRate );
		double dPluTax = dPluRetail - dPluExTax;
		double dPluGP = dPluExTax - dPluCostAndWaste;
		double dPluGPPercent = CalculateGPPercent( dPluExTax, dPluCostAndWaste );
			
		CStockReportLine csvOut;

		if ( bGotFirstLine == FALSE )
		{
			OutputStockCodeFields( csvOut, FALSE );
			bGotFirstLine = TRUE;
		}
		else
			csvOut.AppendBlanks(4);

		csvOut.AppendString ( ReportHelpers.GetDisplayPluNo( nPluNo ) );
		csvOut.AppendString ( strSuppRef );		

		if ( dStockQty != 0.0 )
		{
			csvOut.AppendVal ( dStockCost );
			
			if ( m_bSubUnits ) 
				csvOut.AppendVal ( dStockWaste );		
			
			csvOut.AppendVal ( dPluRetail / dStockQty );
			csvOut.AppendVal ( dPluTax / dStockQty );
			csvOut.AppendVal ( dPluGP / dStockQty );
			csvOut.AppendVal ( dPluGPPercent );
		}
		else
		{
			csvOut.AppendVal ( 0.0 );

			if ( m_bSubUnits )
				csvOut.AppendVal( 0.0 );

			csvOut.AppendVal ( 0.0 );
			csvOut.AppendVal ( 0.0 );
			csvOut.AppendVal ( 0.0 );
			csvOut.AppendVal ( 0.0 );
		}
			
		WriteReportLine( csvOut.GetLine() );
	}
		
	for ( int nListPos = 0; nListPos < DataManager.StockPlu.GetRecipePluNoCount ( nStockPluIdx ); nListPos++ )
	{
		__int64 nPluNo = DataManager.StockPlu.GetRecipePluNo ( nStockPluIdx, nListPos );
			
		int nPluIdx;
		if ( DataManager.Plu.FindPluByNumber ( nPluNo, nPluIdx ) == FALSE )
			continue;

		CPluCSVRecord PluRecord;
		DataManager.Plu.GetAt ( nPluIdx, PluRecord );

		CExplodePluDatabase ExplodePlu;
		
		CStringArray StockDetails;
		if ( ExplodePlu.ExplodePlu ( nPluNo, StockDetails, AUDIT_SALES, 1.0, PluRecord.GetPrice( 0) ) != PLU_EXCEPTION_NONE )
			continue;

		int nRecipePos = -1;
		for ( int nIndex = 0; nIndex < StockDetails.GetSize(); nIndex++ )
		{
			CCSVExplodedPlu csv ( StockDetails.GetAt ( nIndex ) );
			CString strStockCode = m_LabelStockRecord.GetStockCode();

			if ( strStockCode == csv.GetStockCode() )
			{
				nRecipePos = nIndex;
				break;
			}
		}

		if ( nRecipePos == -1 )
			continue;

		CCSVExplodedPlu csvSale ( StockDetails.GetAt ( nRecipePos ) );
		double dStockQty = csvSale.GetStockQty();
		
		double dStockCost = m_LabelStockRecord.GetUnitCost ( nStockSuppIdx );
		double dStockWaste = EstimateWastageFromSales( dStockCost );

		double dPluCost = dStockQty * dStockCost;
		double dPluWaste = dStockQty * dStockWaste;
		double dPluCostAndWaste = dPluCost + dPluWaste;

		CTaxRateInfo TaxRateInfo;
		DataManager.TaxRates.GetTaxRateInfo( PluRecord.GetBaseTaxBandInt(), TaxRateInfo );

		double dPluRetail = csvSale.GetStockValue();
		double dPluExTax = ReportHelpers.CalcNonTax ( dPluRetail, TaxRateInfo.m_dTaxRate );
		double dPluTax = dPluRetail - dPluExTax;
		double dPluGP = dPluExTax - dPluCostAndWaste;
		double dPluGPPercent = CalculateGPPercent( dPluExTax, dPluCostAndWaste );

		CString strSuppRef = m_LabelStockRecord.GetSuppRefExternal( nStockSuppIdx, DealerFlags.GetSuppRefFlag() );

		CStockReportLine csvOut;

		if ( bGotFirstLine == FALSE )
		{
			OutputStockCodeFields( csvOut, FALSE );
			bGotFirstLine = TRUE;
		}
		else
			csvOut.AppendBlanks(4);

		csvOut.AppendString ( ReportHelpers.GetDisplayPluNo( nPluNo ) );
		csvOut.AppendString ( strSuppRef );		
				
		if ( dStockQty != 0.0 )
		{
			csvOut.AppendVal ( dStockCost );
			
			if ( m_bSubUnits ) 
				csvOut.AppendVal ( dStockWaste );		
			
			csvOut.AppendVal ( dPluRetail / dStockQty );
			csvOut.AppendVal ( dPluTax / dStockQty );
			csvOut.AppendVal ( dPluGP / dStockQty );
			csvOut.AppendVal ( dPluGPPercent );
		}
		else
		{
			csvOut.AppendVal ( 0.0 );

			if ( m_bSubUnits )
				csvOut.AppendVal( 0.0 );

			csvOut.AppendVal ( 0.0 );
			csvOut.AppendVal ( 0.0 );
			csvOut.AppendVal ( 0.0 );
			csvOut.AppendVal ( 0.0 );
		}	
		
		WriteReportLine( csvOut.GetLine() );
	}	
}

/**********************************************************************/
