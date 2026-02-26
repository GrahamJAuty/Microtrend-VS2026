/**********************************************************************/
#include "RepCSV.h"
#include "ReportHelpers.h"
#include "StockAuditBaseReport.h"
/**********************************************************************/
#include "ExplodePlu.h"
/**********************************************************************/

CExplodePluBase::CExplodePluBase()
{
	m_nPriceLevel = 0;
}

/**********************************************************************/

int CExplodePluBase::GetRedirectionSpNo( __int64 nPluNo )
{
	int nRedirectionSpNo = 0;

	CString strPluNo;
	::FormatPluNo( nPluNo, strPluNo );

	int nPluIdx;
	if ( DataManager.Plu.FindPluByNumber ( nPluNo, nPluIdx ) == TRUE )
	{
		CPluCSVRecord PluRecord;
		DataManager.Plu.GetAt ( nPluIdx, PluRecord );

		CRecipeCSVRecord RecipeRecord;
		DataManager.Plu.GetRecipe( nPluIdx, PluRecord.GetPluNoInt(), RecipeRecord );
		nRedirectionSpNo = RecipeRecord.GetSpNo();
	}

	return nRedirectionSpNo;
}

/**********************************************************************/

int CExplodePluBase::ExplodePlu ( __int64 nPluNo, CStringArray& StockDetails, int nReportType, double dPluQty, double dPluValue )
{
	StockDetails.RemoveAll();
	CStringArray ItemCostsArray;
	
	CString strPluNo;
	::FormatPluNo( nPluNo, strPluNo );

	int nPluIdx;
	if ( DataManager.Plu.FindPluByNumber ( nPluNo, nPluIdx ) == FALSE )
	{
		CCSV csvOut;
		csvOut.Add ( strPluNo );
		csvOut.Add ( dPluQty, 10 );
		csvOut.Add ( dPluValue, 10 );
		csvOut.Add ( "" );
		csvOut.Add ( "" );
		csvOut.Add ( "Plu not found" );
		StockDetails.Add ( csvOut.GetLine() );
		return PLU_EXCEPTION_PLUNOTEXIST;
	}
	
	CPluCSVRecord PluRecord;
	DataManager.Plu.GetAt ( nPluIdx, PluRecord );

	CRecipeCSVRecord RecipeRecord;
	DataManager.Plu.GetRecipe( nPluIdx, PluRecord.GetPluNoInt(), RecipeRecord );
	bool bReverse = RecipeRecord.GetReverseSaleFlag();

	if ( RecipeRecord.GetPluStockLinkFlag() == FALSE )
	{
		CCSV csvOut;
		csvOut.Add ( strPluNo );
		csvOut.Add ( dPluQty, 10 );
		csvOut.Add ( dPluValue, 10 );
		csvOut.Add ( ReportHelpers.GetReportText ( PluRecord ) );
		csvOut.Add ( "" );
		csvOut.Add ( "Non stock item" );
		StockDetails.Add ( csvOut.GetLine() );
		return PLU_EXCEPTION_PLUNONSTOCK;
	}

	if ( RecipeRecord.IsRecipe() == FALSE )
	{
		CString strStockCode = "";
		double dStockQtyPerPlu = 0.0;

		if ( RecipeRecord.GetSize() == 1 )
		{
			strStockCode = RecipeRecord.GetStockCode(0);
			dStockQtyPerPlu = RecipeRecord.GetStockQty(0);
			
			if ( TRUE == bReverse )
				dStockQtyPerPlu *= -1;
		}

		if ( strStockCode == "" )
		{
			CCSV csvOut;
			csvOut.Add ( strPluNo );
			csvOut.Add ( dPluQty, 10 );
			csvOut.Add ( dPluValue, 10 );
			csvOut.Add ( ReportHelpers.GetReportText ( PluRecord ) );
			csvOut.Add ( "" );
			csvOut.Add ( "No stock code" );
			StockDetails.Add ( csvOut.GetLine() );
			return PLU_EXCEPTION_PLUNOTLINKED;
		}

		int nStockIdx;
		int nException;
		int nExceptionTextHint;
		CStockCSVRecord StockRecord;
		CheckStockCode( strStockCode, nStockIdx, StockRecord, nException, nExceptionTextHint );

		if ( PLU_EXCEPTION_NONE != nException )
		{
			CCSV csvOut;
			csvOut.Add ( strPluNo );
			csvOut.Add ( dPluQty, 10 );
			csvOut.Add ( dPluValue, 10 );
			csvOut.Add ( ReportHelpers.GetReportText ( PluRecord ) );
			csvOut.Add ( strStockCode );
			csvOut.Add ( GetStockExceptionText( nException, nExceptionTextHint ) );
			StockDetails.Add ( csvOut.GetLine() );
		}
		else
		{
			CCSV csvOut;
			csvOut.Add ( strPluNo );
			csvOut.Add ( dPluQty, 10 );
			csvOut.Add ( dPluValue, 10 );
			csvOut.Add ( ReportHelpers.GetReportText ( PluRecord ) );
			csvOut.Add ( strStockCode );
			csvOut.Add ( "" );
			csvOut.Add ( nStockIdx );
			csvOut.Add ( dPluQty * dStockQtyPerPlu, 10 );
			csvOut.Add ( dPluValue, 10 ); 
			csvOut.Add ( DataManager.Stock.GetSubUnits( StockRecord ) );
			StockDetails.Add ( csvOut.GetLine() );
		}

		return nException; 
	}
	else
	{
		switch ( nReportType )
		{
		case AUDIT_DELIVERY:
		case AUDIT_OPENING:
		case AUDIT_STOCKTAKE:
		case AUDIT_CLOSING:
		case AUDIT_RETURNS:
		case AUDIT_ADJUSTMENT_STOCK:
		case AUDIT_TRANSFER:
			CCSV csvOut;
			csvOut.Add ( strPluNo );
			csvOut.Add ( dPluQty, 10 );
			csvOut.Add ( dPluValue, 10 );
			csvOut.Add ( ReportHelpers.GetReportText ( PluRecord ) );
			csvOut.Add ( "" );
			csvOut.Add ( "Plu is a recipe" );
			StockDetails.Add ( csvOut.GetLine() );
			return PLU_EXCEPTION_PLUISRECIPE;
		}

		double dTotalCost = 0.0;
		int nException = PLU_EXCEPTION_NONE;
		
		for ( int nItem = 0; nItem < RecipeRecord.GetSize(); nItem++ )
		{
			CString strStockCode = RecipeRecord.GetStockCode ( nItem );

			int nStockIdx;
			int nExceptionTextHint;
			CStockCSVRecord StockRecord;
			CheckStockCode( strStockCode, nStockIdx, StockRecord, nException, nExceptionTextHint );

			if ( PLU_EXCEPTION_NONE != nException )
			{
				CCSV csvOut;
				csvOut.Add ( strPluNo );
				csvOut.Add ( dPluQty, 10 );
				csvOut.Add ( dPluValue, 10 );
				csvOut.Add ( ReportHelpers.GetReportText ( PluRecord ) );
				csvOut.Add ( strStockCode );
				csvOut.Add ( GetStockExceptionText( nException, nExceptionTextHint ) );
				StockDetails.Add ( csvOut.GetLine() );
				break;
			}

			double dStockQty = dPluQty * ( RecipeRecord.GetStockQty( nItem ) );
			double dStockCost = StockRecord.GetUnitCost() * dStockQty;

			if ( TRUE == bReverse )
				dStockQty *= -1;
			
			dTotalCost += dStockCost; 						
			
			CCSV csvItem;
			csvItem.Add ( nStockIdx );
			csvItem.Add ( dStockCost, 10 );
			csvItem.Add ( dStockQty, 10 );			
			ItemCostsArray.Add ( csvItem.GetLine() );
		}

		if ( PLU_EXCEPTION_NONE == nException )
		{
			for ( int nItem = 0; nItem < ItemCostsArray.GetSize(); nItem++ )
			{
				CCSVRecipeBuffer csvItem ( ItemCostsArray.GetAt ( nItem ) );					
				double dTotalItemValue;

				if ( dTotalCost != 0.0 )
					dTotalItemValue = ( ( dPluValue * csvItem.GetCost() ) / dTotalCost  );
				else
					dTotalItemValue = ( dPluValue / ItemCostsArray.GetSize() );

				int nStockIdx = csvItem.GetStockIdx();
				CStockCSVRecord StockRecord;
				DataManager.Stock.GetAt ( nStockIdx, StockRecord );

				CCSV csvOut;
				csvOut.Add ( strPluNo );
				csvOut.Add ( dPluQty, 10 );
				csvOut.Add ( dPluValue, 10 );
				csvOut.Add ( ReportHelpers.GetReportText ( PluRecord ) );
				csvOut.Add ( StockRecord.GetStockCode() );
				csvOut.Add ( "" );
				csvOut.Add ( nStockIdx );
				csvOut.Add ( csvItem.GetQty(), 10 );
				csvOut.Add ( dTotalItemValue, 10 );
				csvOut.Add ( DataManager.Stock.GetSubUnits( StockRecord ) );
				StockDetails.Add ( csvOut.GetLine() );
			}
		}

		return nException;
	}
}

/**********************************************************************/

void CExplodePluBase::CheckStockCode( CString& strStockCode, int& nStockIdx, CStockCSVRecord& StockRecord, int& nExceptionThisLine, int& nExceptionTextHint )
{
	nExceptionThisLine = PLU_EXCEPTION_NONE;
	nExceptionTextHint = 0;

	if ( DataManager.Stock.FindStockItemByCode ( strStockCode, nStockIdx ) == FALSE )
	{
		nExceptionThisLine = PLU_EXCEPTION_STOCKNOTEXIST;
	}
	else 
	{
		DataManager.Stock.GetAt ( nStockIdx, StockRecord );

		if ( GetSpNo1() != 0 )
		{
			CStockLevelsCSVRecord StockLevels;
			DataManager.Stock.GetStockpointTotals( nStockIdx, StockRecord, GetSpNo1(), StockLevels );

			if ( GetStkTakeNo() != -1 )
			{				
				if ( StockLevels.GetStocktakeNo() != GetStkTakeNo() )
				{
					nExceptionThisLine = PLU_EXCEPTION_STOCKTAKE;
					nExceptionTextHint = GetStkTakeNo();
				}
			}

			if ( ( GetStkTakeNo() <= 0 ) && ( PLU_EXCEPTION_NONE == nExceptionThisLine ) )
			{
				if ( StockLevels.GetActiveFlag() == FALSE )
				{
					nExceptionThisLine = PLU_EXCEPTION_STOCKINACTIVE;
					nExceptionTextHint = ( GetSpNo2() == 0 ) ? 0 : 1;
				}
				else if ( GetSpNo2() != 0 )
				{
					DataManager.Stock.GetStockpointTotals( nStockIdx, StockRecord, GetSpNo2(), StockLevels );

					if ( StockLevels.GetActiveFlag() == FALSE )
					{
						nExceptionThisLine = PLU_EXCEPTION_STOCKINACTIVE;
						nExceptionTextHint = 2;
					}
				}
			}
		}
	}
}

/**********************************************************************/

const char* CExplodePluBase::GetStockExceptionText( int nExceptionThisLine, int nExceptionTextHint )
{
	m_strStockExceptionText = "";
				
	switch( nExceptionThisLine )
	{
	case PLU_EXCEPTION_STOCKNOTEXIST:
		m_strStockExceptionText = "Stock code not found";
		break;

	case PLU_EXCEPTION_STOCKTAKE:
		switch( nExceptionTextHint )
		{
		case 0:		m_strStockExceptionText = "Stock code in pending stock take";	break;
		default:	m_strStockExceptionText = "Stock code not in this stock take";	break;
		}
		break;

	case PLU_EXCEPTION_STOCKINACTIVE:
		switch( nExceptionTextHint )
		{
		case 0:		m_strStockExceptionText = "Stock code not active at this stockpoint";	break;
		case 1:		m_strStockExceptionText = "Stock code not active at source stockpoint";	break;
		case 2:		m_strStockExceptionText = "Stock code not active at target stockpoint";	break;
		}
		break;
	}

	return m_strStockExceptionText;
}

/**********************************************************************/

CExplodePluDatabase::CExplodePluDatabase() : CExplodePluBase()
{
}

/**********************************************************************/

CExplodePluStockLevel::CExplodePluStockLevel( int nSpNo1, int nSpNo2 ) : CExplodePluBase()
{
	m_nSpNo1 = nSpNo1;
	m_nSpNo2 = nSpNo2;
}

/**********************************************************************/

CExplodePluStockTake::CExplodePluStockTake( int nSpNo1, int nStkTakeNo ) : CExplodePluBase()
{
	m_nSpNo1 = nSpNo1;
	m_nStkTakeNo = nStkTakeNo;
}

/**********************************************************************/
