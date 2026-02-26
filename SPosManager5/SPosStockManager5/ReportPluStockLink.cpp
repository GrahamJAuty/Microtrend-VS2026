/**********************************************************************/
#include "..\SPosStockManager5\PluPrtStockLinkDlg.h"
#include "ReportHelpers.h"
#include "RepSpawn.h"
#include "StatusProgress.h"
/**********************************************************************/
#include "ReportPluStockLink.h"
/**********************************************************************/
static const int REPORT_OKAY = 0;
static const int REPORT_ERROR_RANGE = 1;
static const int REPORT_ERROR_FILE = 2;
/**********************************************************************/
static const int LEN_PLUNO	= 350;
static const int LEN_REPTEXT	= 400;
static const int LEN_DEPTNO	= 100;
static const int LEN_STKCODE	= 380;
static const int LEN_STKQTY	= 220;
static const int LEN_STKUNIT	= 400;
/**********************************************************************/

CReportPluStockLink::CReportPluStockLink( CPluFilterArray& FilterArray, const char* szKey ) : m_FilterArray ( FilterArray )
{
	CFilenameUpdater FnUp( SysFiles::PluLinkPrm );
	m_strParamsFilename = FnUp.GetFilenameToUse();
	m_strKey = szKey;
}

/**********************************************************************/

void CReportPluStockLink::CreateReport()
{
	StatusProgress.Lock( TRUE, "Creating report" );
	CCSVPluPrtStockLinkKey csvKey ( m_strKey );
	CString strRepType = ( csvKey.PrintProfit() == TRUE ) ? "1" : "0";
	int nResult = CreatePluStockLinkReport();
	StatusProgress.Unlock();

	switch( nResult )
	{
	case 0:
		{
			CReportManager ReportManager;
			ReportManager.DisplayReport ( Super.ReportFilename(), m_strParamsFilename, strRepType, NULL );
		}
		break;

	case 1:
		Prompter.Error ( "There are no Plus in the selected range" );
		break;

	case 2:
		Prompter.Error ( "Unable to create Plu report file" );
		break;
	}
}

/**********************************************************************/

int CReportPluStockLink::CreatePluStockLinkReport()
{
	int nStartIdx, nRange;
	__int64 nPluFrom = _atoi64( m_strPluFrom );
	__int64 nPluTo = _atoi64( m_strPluTo );
	m_FilterArray.GetArrayIdxRange( nPluFrom, nPluTo, nStartIdx, nRange );

	int nProgress = 0;
	
	CConsolidateArray ConsolArray;

	if ( 0 == nRange )
		return REPORT_ERROR_RANGE;				//no plus in range

	if ( m_ReportFile.Open ( Super.ReportFilename() ) == FALSE )
		return REPORT_ERROR_FILE;				//unable to create file
	
	m_ReportFile.SetStyle1 ( "Plu Stock Links" );
	CreatePluStockLinkReportHeader ();
	
	CDepartmentSetCSVRecord DepartmentSet;
	DepartmentSet.CreateDepartmentSet ( m_nDepartmentFilter );

	CCSVPluPrtStockLinkKey csv ( m_strKey );
	
	for ( int nStep = 0; nStep < nRange; nStep++ )	
	{ 
		int nMultiplyBy = ( m_bSortByDept == TRUE ) ? 50 : 100;
		StatusProgress.SetPos ( nStep, nRange, 0, nMultiplyBy );
	
		CPluCSVRecord PluRecord;
		int nPluIdx = m_FilterArray.GetPluItemIndexAt ( nStartIdx + nStep );
		DataManager.Plu.GetAt ( nPluIdx, PluRecord );

		__int64 nPluNo = PluRecord.GetPluNoInt();
		if ( ( nPluNo < nPluFrom ) || ( nPluNo > nPluTo ) )
			continue;
						
		if ( DepartmentSet.GetDepartmentStatus ( PluRecord.GetBaseDeptNo() ) == FALSE )
			continue;

		if ( ( csv.PrintZeroPriced() == FALSE ) && ( PluRecord.GetPrice ( 0 ) == 0.0 ) )
			continue;

		if ( m_bSortByDept == TRUE )
		{
			CString strLabel;
			strLabel.Format ( "%2.2d%10.10d", PluRecord.GetBaseDeptNo(), nPluIdx );
			ConsolArray.Add ( strLabel );	//non zero value ensures key is consolidated
		}
		else
			CreatePluStockLinkReportLine ( PluRecord );
	}
	
	nRange = ConsolArray.GetSize();
	int nCurrentDept = -1;

	for ( int nStep = 0; nStep < nRange; nStep++ )
	{
		StatusProgress.SetPos ( nStep, nRange, 50, 50 );
		CString strLabel = ConsolArray.GetLabel(nStep);	
		int nIndex = atoi ( strLabel.Right(10) );

		CPluCSVRecord PluRecord;
		DataManager.Plu.GetAt ( nIndex, PluRecord );
		
		if ( PluRecord.GetBaseDeptNo() != nCurrentDept )		//Check for new department
		{
			if ( nCurrentDept != -1 )
				m_ReportFile.WriteLine ( "" );

			nCurrentDept = PluRecord.GetBaseDeptNo();
			CString strTemp;
			strTemp.Format ( "<..>D%2.2d %s", nCurrentDept, DataManager.Department.GetReportTextByDeptNo ( nCurrentDept ) );
			m_ReportFile.WriteLine ( strTemp );
			if ( csv.PrintSeparate() == TRUE ) m_ReportFile.WriteLine ( "<LI>" );
		}
		CreatePluStockLinkReportLine ( PluRecord );
	}

	m_ReportFile.WriteLine ( "" );	
	m_ReportFile.Close();
	return 0;			//function completed properly
}

/**********************************************************************/

void CReportPluStockLink::CreatePluStockLinkReportHeader()
{
	CCSVPluPrtStockLinkKey csvKey ( m_strKey );

	m_ReportFile.AddColumn ( "Plu Number", TA_LEFT, LEN_PLUNO );
	m_ReportFile.AddColumn ( "Plu Description", TA_LEFT, LEN_REPTEXT );
	m_ReportFile.AddColumn ( "Dp", TA_RIGHT, LEN_DEPTNO );
	m_ReportFile.AddColumn ( "Stock Code", TA_LEFT, LEN_STKCODE );
	m_ReportFile.AddColumn ( "Stock Description", TA_LEFT, LEN_REPTEXT );
	m_ReportFile.AddColumn ( "Stock Qty", TA_RIGHT, LEN_STKQTY );
	m_ReportFile.AddColumn ( "Unit", TA_LEFT, LEN_STKUNIT );

	if ( csvKey.PrintProfit() == TRUE )
	{
		m_ReportFile.AddColumn ( "Cost", TA_RIGHT, LEN_STKQTY );
		m_ReportFile.AddColumn ( "%Cost", TA_RIGHT, LEN_STKQTY );
		m_ReportFile.AddColumn ( "Retail", TA_RIGHT, LEN_STKQTY );
		m_ReportFile.AddColumn ( EcrmanOptions.GetSalesTaxName(), TA_RIGHT, LEN_STKQTY );
		m_ReportFile.AddColumn ( "GP", TA_RIGHT, LEN_STKQTY );
	}
}

/**********************************************************************/

void CReportPluStockLink::CreatePluStockLinkReportLine ( CPluCSVRecord& PluRecord )
{
	CCSVPluPrtStockLinkKey csvKey ( m_strKey );

	CRecipeCSVRecord RecipeRecord;
	DataManager.Plu.GetRecipe( -1, PluRecord.GetPluNoInt(), RecipeRecord );

	int nItemCount = RecipeRecord.GetSize();

	bool bPrintLine = FALSE;
	
	switch( nItemCount )
	{
	case 0:
		bPrintLine = csvKey.PrintUnlinked();
		break;

	case 1:
		bPrintLine = csvKey.PrintLinked();
		break;

	default:
		if ( nItemCount >= 2 ) bPrintLine = csvKey.PrintRecipe();
		break;
	}

	if ( bPrintLine == FALSE )
		return;

	int nLinesToPrint = nItemCount;
	if ( nLinesToPrint == 0 ) nLinesToPrint = 1;

	CTaxRateInfo TaxRateInfo;
	DataManager.TaxRates.GetTaxRateInfo( PluRecord.GetBaseTaxBandInt(), TaxRateInfo );

	double dTotalCost = 0.0;
	if ( RecipeRecord.GetPluStockLinkFlag() == TRUE )
		dTotalCost = DataManager.Plu.GetRecipeCost( RecipeRecord );
	else
		dTotalCost = PluRecord.GetPurchaseCost();

	double dTotalRetail = PluRecord.GetPrice(0);
	double dTotalTax = dTotalRetail - ReportHelpers.CalcNonTax( dTotalRetail, TaxRateInfo.m_dTaxRate );

	for ( int nLine = 0; nLine < nLinesToPrint; nLine++ )
	{
		CCSV csvOut( '\t', '"', TRUE, FALSE, FALSE );		/* tab separated csv line */

		if ( nLine == 0 )
		{
			csvOut.Add ( ReportHelpers.GetDisplayPluNo ( PluRecord.GetPluNoInt() ) );
			csvOut.Add ( ReportHelpers.GetReportText ( PluRecord ) );
			csvOut.Add ( PluRecord.GetBaseDeptNo() );
		}
		else
		{
			csvOut.Add ( "" );
			csvOut.Add ( "" );
			csvOut.Add ( "" );
		}

		if ( nLine < nItemCount )
		{
			CString strStockCode = RecipeRecord.GetStockCode( nLine );
			double dLineStockQty = RecipeRecord.GetStockQty( nLine );

			double dLineCost = 0.0;
			double dLinePercent = 0.0;
			double dLineRetail = 0.0;
			double dLineTax = 0.0;
			
			int nStockIdx;
			if ( DataManager.Stock.FindStockItemByCode( strStockCode, nStockIdx ) == TRUE )
			{			
				CStockCSVRecord StockRecord;
				DataManager.Stock.GetAt( nStockIdx, StockRecord );
				dLineCost = StockRecord.GetUnitCost(0) * dLineStockQty;

				if ( dTotalCost != 0.0 )
					dLinePercent = ( dLineCost * 100.0 ) / dTotalCost;
				else
					dLinePercent = 100.0 / RecipeRecord.GetSize();

				dLineRetail = ( dTotalRetail * dLinePercent ) / 100.0;
				dLineTax = ( dTotalTax * dLinePercent ) / 100.0;
			
				csvOut.Add ( ReportHelpers.GetDisplayStockCode ( strStockCode ) );
				csvOut.Add ( StockRecord.GetDescription() );
				csvOut.Add ( DataManager.Stock.GetStockLevelStringForPluEdit ( StockRecord, dLineStockQty, 5 ) );
				csvOut.Add ( DataManager.Stock.GetStockSubUnitNameForPluEdit ( StockRecord ) );
				
				if ( csvKey.PrintProfit() == TRUE )
				{
					csvOut.Add ( dLineCost, 3 );
					csvOut.Add ( dLinePercent, 2 );
					csvOut.Add ( dLineRetail, 3 );
					csvOut.Add ( dLineTax, 3 );
					csvOut.Add ( dLineRetail - dLineCost - dLineTax, 3 );
				}
			}
			else
			{
				csvOut.Add ( ReportHelpers.GetDisplayStockCode ( strStockCode ) );
				csvOut.Add ( "Unknown" );
				csvOut.Add ( dLineStockQty, 5 );
			}
		}
		m_ReportFile.WriteLine ( csvOut.GetLine() );

		if ( csvKey.PrintProfit() == TRUE )
		{
			if ( ( nLine == nLinesToPrint - 1 ) && ( nLinesToPrint >= 2 ) )
			{
				m_ReportFile.WriteLine ( "\t\t\t\t\t\t\t<LI>\t\t<LI>\t<LI>\t<LI>" );

				CCSV csvOut( '\t', '"', TRUE, FALSE, FALSE );		/* tab separated csv line */	
				csvOut.Add ( "" );
				csvOut.Add ( "" );
				csvOut.Add ( "" );
				csvOut.Add ( "" );
				csvOut.Add ( "" );
				csvOut.Add ( "" );
				csvOut.Add ( "" );
				csvOut.Add ( dTotalCost, 3 );
				csvOut.Add ( "" );
				csvOut.Add ( dTotalRetail, 3 );
				csvOut.Add ( dTotalTax, 3 );
				csvOut.Add ( dTotalRetail - dTotalCost - dTotalTax, 3 );
				m_ReportFile.WriteLine ( csvOut.GetLine() );
			}
		}
	}
	if ( csvKey.PrintSeparate() == TRUE ) m_ReportFile.WriteLine ( "<LI>" );
}

/**********************************************************************/

