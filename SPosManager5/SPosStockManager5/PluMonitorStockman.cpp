/**********************************************************************/
 
/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "MessageLogger.h"
#include "PriceHelpers.h"
#include "..\CommonEcrStock\PriceSetItemManager.h"
#include "ReportHelpers.h"
#include "StatusProgress.h"
#include "SysInfo.h"
/**********************************************************************/
#include "PluMonitorStockman.h"
/**********************************************************************/

CPluMonitorStockman::CPluMonitorStockman()
{
}

/**********************************************************************/

CPluMonitorStockman::~CPluMonitorStockman()
{
}

/**********************************************************************/

void CPluMonitorStockman::Reset()
{
	while ( m_PluDatabase.GetSize() > 0 )
		m_PluDatabase.RemoveAt( m_PluDatabase.GetSize() - 1 );
	
	while ( m_RecipeDatabase.GetSize() > 0 )
		m_RecipeDatabase.RemoveAt( m_RecipeDatabase.GetSize() - 1 );
}

/**********************************************************************/

int CPluMonitorStockman::InsertRecord ( CPluCSVRecord& PluRecord, const char* szStockCode, bool bExportFlag )
{
	CRecipeCSVRecord RecipeRecord;

	CString strStockCode = szStockCode;
	::TrimSpaces( strStockCode, FALSE );
	if ( strStockCode != "" )
		RecipeRecord.AddStockCode( szStockCode, 1.0 );
	
	return InsertRecord ( PluRecord, RecipeRecord, bExportFlag );
}

/**********************************************************************/

int CPluMonitorStockman::InsertRecord( CPluCSVRecord& PluRecord, CRecipeCSVRecord& RecipeRecord, bool bExportFlag )
{	
	int nPluIdx;
	if ( FindPluByNumber ( PluRecord.GetPluNoInt(), nPluIdx ) )
		return nPluIdx;

	CString strPluNo = PluRecord.GetPluNoString();

	if ( bExportFlag == TRUE )
		PluRecord.SetExport( TRUE );

	m_PluDatabase.InsertAt ( nPluIdx, PluRecord );

	int nRecipeIdx;
	RecipeRecord.SetRecipePluNo( strPluNo );
	
	if ( m_RecipeDatabase.FindPluByNumber( -1, strPluNo,  nRecipeIdx ) )
	{
		m_RecipeDatabase.SetSyncFlag( FALSE );
		m_RecipeDatabase.SetAt( nRecipeIdx, RecipeRecord );
		return nPluIdx;
	}

	if ( nRecipeIdx != nPluIdx )
		m_RecipeDatabase.SetSyncFlag( FALSE );

	m_RecipeDatabase.InsertAt( nRecipeIdx, RecipeRecord );
	return nPluIdx;
}

/**********************************************************************/

void CPluMonitorStockman::RemoveAt ( int nPluIdx )
{
	if ( nPluIdx < 0 || nPluIdx >= m_PluDatabase.GetSize() )
		return;

	CString strPluNo = m_PluDatabase.GetPluNoString ( nPluIdx );
	m_PluDatabase.RemoveAt ( nPluIdx );

	int nRecipeIdx;
	if ( m_RecipeDatabase.FindPluByNumber( nPluIdx, strPluNo, nRecipeIdx ) )
		m_RecipeDatabase.RemoveAt( nRecipeIdx );
}

/**********************************************************************/

void CPluMonitorStockman::RemoveAll()
{
	m_PluDatabase.RemoveAt( 0, m_PluDatabase.GetSize() );
	m_RecipeDatabase.RemoveAt( 0, m_RecipeDatabase.GetSize() );
}

/**********************************************************************/

void CPluMonitorStockman::GetRecipe ( int nMasterIdx, __int64 nPluNo, CRecipeCSVRecord& RecipeRecord )
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

void CPluMonitorStockman::SetRecipe ( int nMasterIdx, CRecipeCSVRecord& RecipeRecord )
{
	CString strPluNo = RecipeRecord.GetRecipePluNo();
	
	int nRecipeIdx;
	if ( m_RecipeDatabase.FindPluByNumber( nMasterIdx, strPluNo, nRecipeIdx ) ) 
		m_RecipeDatabase.SetAt( nRecipeIdx, RecipeRecord );
	else
		m_RecipeDatabase.InsertAt( nRecipeIdx, RecipeRecord );
}

/**********************************************************************/

bool CPluMonitorStockman::IsModified()
{
	return ( m_PluDatabase.IsModified() || m_RecipeDatabase.IsModified() );
}

/**********************************************************************/

bool CPluMonitorStockman::Synchronise( bool bOnlyIfFlagged )
{
	if ( bOnlyIfFlagged )
	{
		if ( m_RecipeDatabase.IsInSync() )
			return FALSE;

		CMessageLoggerInfo info;
		info.m_strMessage.Format( "Synchronise Recipes (DB%3.3d)", dbDatabase.GetDbNo( DataManager.GetActiveDbIdx() ) ); 
		info.m_nHideLevel = 2;
		
		MessageLogger.LogSchedulerMessage( info, FALSE, TRUE, FALSE );
	}

	//PLACE EXISTING RECIPES IN A TEMPORARY ARAY
	//MAKING SURE THEY ARE SORTED WITH NO DUPLICATES
	CRecipeCSVArray tempRecipeDatabase;

	StatusProgress.Lock( TRUE, "Synchronising recipe file" );
	int nSize = m_RecipeDatabase.GetSize();
	for ( int nRecipeIdx = 0; nRecipeIdx < m_RecipeDatabase.GetSize(); nRecipeIdx++ )
	{
		StatusProgress.SetPos( nRecipeIdx, nSize );

		CRecipeCSVRecord RecipeRecord;
		m_RecipeDatabase.GetAt( nRecipeIdx, RecipeRecord );
		CString strPluNo = RecipeRecord.GetRecipePluNo();

		int nNewRecipeIdx;
		if ( tempRecipeDatabase.FindPluByNumber ( -1, strPluNo, nNewRecipeIdx ) == FALSE )
			tempRecipeDatabase.InsertAt( nNewRecipeIdx, RecipeRecord );
	}

	//REBUILD RECIPE FILE TO EXACTLY MATCH PLU DATABASE
	while ( m_RecipeDatabase.GetSize() > 0 )
		m_RecipeDatabase.RemoveAt( m_RecipeDatabase.GetSize() - 1 );
	
	for ( int nPluIdx = 0; nPluIdx < GetSize(); nPluIdx++ )
	{
		StatusProgress.SetPos( nPluIdx, GetSize() );
		CString strPluNo = GetPluNoString( nPluIdx );

		int nNewRecipeIdx;
		if ( tempRecipeDatabase.FindPluByNumber( -1, strPluNo, nNewRecipeIdx ) == FALSE )
		{
			CRecipeCSVRecord RecipeRecord;
			RecipeRecord.SetRecipePluNo( strPluNo );
			m_RecipeDatabase.Add( RecipeRecord );
		}
		else
		{
			CRecipeCSVRecord RecipeRecord;
			tempRecipeDatabase.GetAt( nNewRecipeIdx, RecipeRecord );
			m_RecipeDatabase.Add( RecipeRecord );
		}
	}
	StatusProgress.Unlock();
	m_RecipeDatabase.SetSyncFlag( TRUE );

	return TRUE;
}

/**********************************************************************/

int CPluMonitorStockman::ImportLinePriceSet ( int& nPos, CSSImportFile& import, const char* szImportLine, CString& strOldRecord )
{
	return m_PluDatabase.ImportLinePriceSet( nPos, import, szImportLine, strOldRecord );
}

/**********************************************************************/

int CPluMonitorStockman::ImportLine ( int nImportMethod, int& nPos, CSSImportFile& import, const char* szImportLine, bool bCreateStock, bool bWithStock )
{
	int nResult = m_PluDatabase.ImportLine( nImportMethod, nPos, import, szImportLine, bWithStock );

	if ( nResult == IMPORT_LINE_ADDED )
	{
		CString strPluNo = GetPluNoString( nPos );
		
		if ( bCreateStock == FALSE )
		{
			CRecipeCSVRecord RecipeRecord;
			RecipeRecord.SetRecipePluNo( strPluNo );
			SetRecipe( -1, RecipeRecord );
		}
		else
		{
			CPluCSVRecord PluRecord;
			GetAt( nPos, PluRecord );
			
			CRecipeCSVRecord RecipeRecord;
			RecipeRecord.SetRecipePluNo ( strPluNo );
			RecipeRecord.AddStockCode ( strPluNo, 1.0 );
			SetRecipe ( -1, RecipeRecord );
		
			//CREATE THE STOCK ITEM IF IT DOES NOT EXIST ALREADY
			CStockCSVRecord StockRecord;
			StockRecord.SetStockCode ( strPluNo );
			StockRecord.SetDescription ( ReportHelpers.GetReportText( PluRecord ) );
			StockRecord.SetCategory ( PluRecord.GetBaseDeptNo() );
			int nStockIdx;
			DataManager.Stock.InsertRecord ( StockRecord, nStockIdx );
		}
	}
	
	return nResult;
}

/**********************************************************************/

int CPluMonitorStockman::ImportRecipeLine ( CSSImportFile& import, const char* szImportLine, bool& bIncompleteLine )
{
	return m_RecipeDatabase.ImportLine( import, szImportLine, bIncompleteLine );
}

/**********************************************************************/
	
void CPluMonitorStockman::RemoveDeadStockLinks()
{
	int nSize = m_RecipeDatabase.GetSize();

	StatusProgress.Lock( TRUE, "Removing unused stock links");
	for ( int nPluIdx = 0; nPluIdx < GetSize(); nPluIdx++ )
	{
		StatusProgress.SetPos( nPluIdx, GetSize() );
		
		CPluCSVRecord PluRecord;
		CRecipeCSVRecord RecipeRecord;
		GetAt( nPluIdx, PluRecord );
		GetRecipe( nPluIdx, PluRecord.GetPluNoInt(), RecipeRecord );

		for ( int nIndex = 0; nIndex < RecipeRecord.GetSize(); nIndex++ )
		{
			CString strStockCode = RecipeRecord.GetStockCode ( nIndex );
				
			int nStockIdx;
			if ( DataManager.Stock.FindStockItemByCode ( strStockCode, nStockIdx ) == FALSE )
			{
				RecipeRecord.RemoveItem ( nIndex );
				nIndex--;
			}
		}

		SetRecipe( nPluIdx, RecipeRecord );
		UpdatePluPrice ( PluRecord, 0.0, TRUE );
		SavePluRecord ( nPluIdx, PluRecord );
	}
	StatusProgress.Unlock();
}

/**********************************************************************/

bool CPluMonitorStockman::IsRecipe( __int64 nPluNo )
{
	int nPluIdx;
	if ( m_PluDatabase.FindPluByNumber( nPluNo, nPluIdx ) == FALSE )
		return FALSE;

	CRecipeCSVRecord RecipeRecord;
	GetRecipe ( nPluIdx, nPluNo, RecipeRecord );
	return RecipeRecord.IsRecipe();
}

/**********************************************************************/

double CPluMonitorStockman::GetRecipeCost( CRecipeCSVRecord& RecipeRecord )
{
#ifdef STOCKMAN_SYSTEM
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
#else
	return 0.0;
#endif
}

/**********************************************************************/

void CPluMonitorStockman::GetRecipeCSVLine ( int nPluIdx, __int64 nPluNo, CString& strLine )
{
	CRecipeCSVRecord RecipeRecord;
	GetRecipe ( nPluIdx, nPluNo, RecipeRecord );

	CCSV csv;
	csv.Add ( nPluNo );
	csv.Add ( RecipeRecord.GetSize() );

	for ( int nIndex = 0; nIndex < RecipeRecord.GetSize(); nIndex++ )
	{
		csv.Add ( RecipeRecord.GetStockCode ( nIndex ) );
		csv.Add ( RecipeRecord.GetStockQty ( nIndex ), 5 );
	}

	strLine = csv.GetLine();
}

/**********************************************************************/

int CPluMonitorStockman::SavePluRecord ( int nPluIdx, CPluCSVRecord& PluRecord, bool bResourceChangeInfo, bool bResourceChangePlu )
{
	int nResult = 0;
	if ( ( nPluIdx >= 0 ) && ( nPluIdx < GetSize() ) )
	{
		CPluCSVRecord OldRecord;
		GetAt ( nPluIdx, OldRecord );
		nResult = OldRecord.GetChange( PluRecord );

		bool bSetInfoDate = bResourceChangeInfo || ( ( OldRecord.GetPluInfoModifierFlag() == FALSE ) && ( PluRecord.GetPluInfoModifierFlag() == TRUE ) );
		bool bSetPluDate = bResourceChangePlu;
		bool bSetExportFlag = FALSE;
		
		switch ( nResult )
		{
		case 1:
			bSetPluDate = TRUE;
			bSetExportFlag = TRUE;
			break;

		case 2:
			bSetExportFlag = TRUE;
			break;

		default:
			break;
		}

		if ( TRUE == bSetInfoDate )
			PluRecord.SetModifiedDateTimeInfo();

		if ( TRUE == bSetPluDate )
			PluRecord.SetModifiedDateTimePlu();

		if ( TRUE == bSetExportFlag )
			PluRecord.SetExport( TRUE );

		if ( TRUE == ( bSetInfoDate | bSetPluDate | bSetExportFlag ) )
			SetAt ( nPluIdx, PluRecord );
	}

	return nResult;
}

/**********************************************************************/

void CPluMonitorStockman::UpdatePrices()
{
	int nSize = GetSize();

	StatusProgress.Lock( TRUE, "Updating PLU prices" );
	for ( int nPluIdx = 0; nPluIdx < nSize; nPluIdx++ )
	{
		StatusProgress.SetPos ( nPluIdx, nSize ); 
		CPluCSVRecord PluRecord;
		GetAt ( nPluIdx, PluRecord );
		UpdatePluPrice ( PluRecord, 0.0, TRUE );
		SavePluRecord ( nPluIdx, PluRecord );
	}
	StatusProgress.Unlock();
}

/**********************************************************************/

void CPluMonitorStockman::UpdatePrices( CReportConsolidationArray<CSortedInt64Item>& arrayPluNo )
{
	int nSize = arrayPluNo.GetSize();

	if ( nSize > 0 )
	{
		StatusProgress.Lock( TRUE, "Updating PLU prices" );
		
		for ( int nIdx = 0; nIdx < nSize; nIdx++ )
		{
			StatusProgress.SetPos( nIdx, nSize );

			CSortedInt64Item item;
			arrayPluNo.GetAt( nIdx, item );

			int nPluIdx;
			if ( FindPluByNumber( item.m_nItem, nPluIdx ) == TRUE )
			{
				CPluCSVRecord PluRecord;
				GetAt ( nPluIdx, PluRecord );
				UpdatePluPrice ( PluRecord, 0.0, TRUE );
				SavePluRecord ( nPluIdx, PluRecord );
			}
		}
		StatusProgress.Unlock();
	}
}

/**********************************************************************/

void CPluMonitorStockman::UpdatePluPrice( CPluCSVRecord& Record, double dCost, bool bGetCost )
{
	switch ( Record.GetGPType() )
	{
	//skip plus with fixed prices
	case PLU_GPTYPE_VARIABLE:
		break;

	case PLU_GPTYPE_FIXED:
		{
			CTaxRateInfo TaxRateInfo;
			DataManager.TaxRates.GetTaxRateInfo( Record.GetBaseTaxBandInt(), TaxRateInfo );
			if ( bGetCost ) dCost = GetPluPurchaseCost( Record );
			double dProfit = Record.GetProfitMargin();
			double dOldPrice1 = Record.GetPrice(0);
			double dNewPrice1 = CPriceHelpers::GetPrice1GPFixed ( TaxRateInfo.m_dTaxRate, dCost, dProfit );
			Record.SetPrice ( 0, dNewPrice1 );
		}
		break;

	case PLU_GPTYPE_PERCENT:
		{
			CTaxRateInfo TaxRateInfo;
			DataManager.TaxRates.GetTaxRateInfo( Record.GetBaseTaxBandInt(), TaxRateInfo );
			if ( bGetCost ) dCost = GetPluPurchaseCost( Record );
			double dProfit = Record.GetProfitMargin();
			double dOldPrice1 = Record.GetPrice(0);
			double dNewPrice1 = CPriceHelpers::GetPrice1GPPercent ( TaxRateInfo.m_dTaxRate, dCost, dProfit );
			Record.SetPrice ( 0, dNewPrice1 );
		}
		break;
	}
}

/**********************************************************************/

double CPluMonitorStockman::GetPluPurchaseCost( CPluCSVRecord& PluRecord )
{
	CRecipeCSVRecord RecipeRecord;
	GetRecipe( -1, PluRecord.GetPluNoInt(), RecipeRecord );

	double dCost = 0.0;
	
	if ( RecipeRecord.GetPluStockLinkFlag() == FALSE )
		dCost = PluRecord.GetPurchaseCost();
	else
		dCost = GetRecipeCost( RecipeRecord );

	return dCost;
}

/**********************************************************************/

void CPluMonitorStockman::GetPluStockQty( __int64 nPluNo, double& dQty )
{
	CRecipeCSVRecord RecipeRecord;
	GetRecipe ( -1, nPluNo, RecipeRecord );

	if ( RecipeRecord.GetSize() != 1 )
		dQty = 0.0;
	else
		dQty = RecipeRecord.GetStockQty(0);
}

/**********************************************************************/

void CPluMonitorStockman::GetPluStockCode ( __int64 nPluNo, CString& strStockCode, bool bRecipe )
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

/**********************************************************************/

void CPluMonitorStockman::GetPluSuppRef( __int64 nPluNo, CString& strSuppRef )
{
	strSuppRef = "";
	
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

/**********************************************************************/

void CPluMonitorStockman::GetPluSupplier ( __int64 nPluNo, CString& strSupplier )
{
	strSupplier = "";

	CRecipeCSVRecord RecipeRecord;
	GetRecipe ( -1, nPluNo, RecipeRecord );

	if ( RecipeRecord.GetSize() != 1 )
		return;

	int nStockIdx;
	CString strStockCode = RecipeRecord.GetStockCode(0);
	if ( DataManager.Stock.FindStockItemByCode( strStockCode, nStockIdx ) == FALSE )
		return;

	CStockCSVRecord StockRecord;
	DataManager.Stock.GetAt( nStockIdx, StockRecord );

	if ( StockRecord.GetSupplierCount() == 0 )
	{
		strSupplier = "Unspecified";
		return;
	}

	int nSuppNo = StockRecord.GetSuppNo(0);

	int nSuppIdx;
	if ( DataManager.Supplier.FindSupplierByNumber( nSuppNo, nSuppIdx ) == FALSE )
	{
		strSupplier = "Unknown";
		return;
	}
	
	strSupplier = DataManager.Supplier.GetName( nSuppIdx );
}

/**********************************************************************/

void CPluMonitorStockman::PurgeForDownload( int nLocIdx, CPluCSVArray& PluArray, bool bChanges )
{
	CString strDateNow;
	COleDateTime dateNow = COleDateTime::GetCurrentTime();
	strDateNow.Format( "%4.4d%2.2d%2.2d",
		dateNow.GetYear(),
		dateNow.GetMonth(),
		dateNow.GetDay() );

	for ( int nPluIdx = 0; nPluIdx < GetSize(); nPluIdx++ )
	{
		bool bIncludeThisRecord = TRUE;

		if ( PriceSetItemManager.IsPluMaster() == FALSE )
		{
			if ( PriceSetItemManager.GetPluStatus( nPluIdx ) == LOCALPLU_EXCLUDE )
				bIncludeThisRecord = FALSE;
		}

		CPluCSVRecord PluRecord;
		DataManager.Plu.GetAt ( nPluIdx, PluRecord );
			
		if ( bIncludeThisRecord == TRUE )
			PluArray.Add( PluRecord );
	}
}

/**********************************************************************/

void CPluMonitorStockman::UpdateSalesManCostPrice()
{
	/*
	if ( EcrmanOptions.GetFeaturesSalesManLinkFlag() == FALSE )
		return;

	if ( EcrmanOptions.GetFeaturesSalesManCostPriceFlag() == FALSE )
		return;

	StatusProgress.Lock( TRUE, "Updating Sales Manager Cost Price" );

	for ( int nPluIdx = 0; nPluIdx < GetSize(); nPluIdx++ )
	{
		CPluCSVRecord PluRecord;
		GetAt( nPluIdx, PluRecord );
		PluRecord.SetCost( GetPluCost( PluRecord ) );
		SetAt( nPluIdx, PluRecord );
	}

	StatusProgress.Unlock();
	*/
}

/**********************************************************************/
