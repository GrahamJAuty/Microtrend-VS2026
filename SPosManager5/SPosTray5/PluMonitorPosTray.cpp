/**********************************************************************/
 
/**********************************************************************/
//#include "DataManager.h"
//#include "DealerFlags.h"
#include "SysInfo.h"
/**********************************************************************/
#include "PluMonitorPosTray.h"
/**********************************************************************/

CPluMonitorPosTray::CPluMonitorPosTray()
{
}

/**********************************************************************/

CPluMonitorPosTray::~CPluMonitorPosTray()
{
}

/**********************************************************************/

void CPluMonitorPosTray::Reset()
{
	while ( m_PluDatabase.GetSize() > 0 )
		m_PluDatabase.RemoveAt( m_PluDatabase.GetSize() - 1 );
	
	while ( m_RecipeDatabase.GetSize() > 0 )
		m_RecipeDatabase.RemoveAt( m_RecipeDatabase.GetSize() - 1 );
}

/**********************************************************************/

void CPluMonitorPosTray::GetRecipe ( int nMasterIdx, __int64 nPluNo, CRecipeCSVRecord& RecipeRecord )
{
	CString strPluNo;
	::FormatPluNo( nPluNo, strPluNo );

	RecipeRecord.RemoveAll();

	int nRecipeIdx;
	if ( m_RecipeDatabase.FindPluByNumber( nMasterIdx, strPluNo, nRecipeIdx ) ) 
		m_RecipeDatabase.GetAt( nRecipeIdx, RecipeRecord );
	else
		RecipeRecord.SetRecipePluNo( strPluNo );
}

/**********************************************************************/

double CPluMonitorPosTray::GetRecipeCost( CRecipeCSVRecord& RecipeRecord )
{
	double dCost = 0.0;

	for ( int nItem = 0; nItem < RecipeRecord.GetSize(); nItem++ )
	{
		CString strStockCode = RecipeRecord.GetStockCode ( nItem );
				
		int nStockIdx;
		if ( DataManager.Stock.FindStockItemByCode ( strStockCode, nStockIdx ) == TRUE )
		{
			CStockCSVRecord StockRecord;
			DataManager.Stock.GetAt ( nStockIdx, StockRecord );
			dCost += ( StockRecord.GetUnitCost() * RecipeRecord.GetStockQty( nItem ) );
		}
	}
	return dCost;
}

/**********************************************************************/

double CPluMonitorPosTray::GetPluPurchaseCost( CPluCSVRecord& PluRecord )
{
	double dCost = 0.0;
	
	if ( SysInfo.IsStockSystem() == FALSE )
		dCost = PluRecord.GetPurchaseCost();
	else
	{
		CRecipeCSVRecord RecipeRecord;
		GetRecipe( -1, PluRecord.GetPluNoInt(), RecipeRecord );

		if ( RecipeRecord.GetPluStockLinkFlag() == FALSE )
			dCost = PluRecord.GetPurchaseCost();
		else
			dCost = GetRecipeCost( RecipeRecord );
	}

	return dCost;
}

/**********************************************************************/

void CPluMonitorPosTray::GetPluSuppRef( __int64 nPluNo, CString& strSuppRef )
{
	strSuppRef = "";
	
	if ( SysInfo.IsStockSystem() == TRUE )
	{
		CRecipeCSVRecord RecipeRecord;
		GetRecipe( -1, nPluNo, RecipeRecord );

		if ( RecipeRecord.GetPluStockLinkFlag() == TRUE )
		{
			if ( RecipeRecord.GetSize() == 1 )
			{
				CString strStockCode = RecipeRecord.GetStockCode ( 0 );
				
				int nStockIdx;
				if ( DataManager.Stock.FindStockItemByCode ( strStockCode, nStockIdx ) == TRUE )
				{
					CStockCSVRecord StockRecord;
					DataManager.Stock.GetAt ( nStockIdx, StockRecord );

					if ( StockRecord.GetSupplierCount() > 0 )
						strSuppRef = StockRecord.GetSuppRefExternal( 0, DealerFlags.GetSuppRefFlag() );
				}
			}
		}
	}
}

/**********************************************************************/

void CPluMonitorPosTray::GetPluStockCode ( __int64 nPluNo, CString& strStockCode, bool bRecipe )
{
	strStockCode = "";

	if ( SysInfo.IsStockSystem() == TRUE )
	{
		CRecipeCSVRecord RecipeRecord;
		GetRecipe ( -1, nPluNo, RecipeRecord );

		if ( RecipeRecord.GetPluStockLinkFlag() == FALSE )
			strStockCode = ( bRecipe == TRUE ) ? "Non Stock" : "";
		else
		{
			switch ( RecipeRecord.GetSize() )
			{
			case 1:
				strStockCode = RecipeRecord.GetStockCode(0);
				break;

			case 0:
				strStockCode = "";
				break;

			default:
				strStockCode = ( bRecipe == TRUE ) ? "Recipe" : "";
				break;
			}
		}
	}
}

/**********************************************************************/
