/**********************************************************************/
#include "..\SposStockManager5\StockOptionsIni.h"
#include "..\SposStockManager5\StockpointCSVArray.h"
#include "..\SposStockManager5\UnitechOptions.h"
/**********************************************************************/
#include "LocationCSVArray.h"
#include "Password.h"
#include "SysInfo.h"
/**********************************************************************/
#include "DataPageFilter.h"
/**********************************************************************/

CDataPageFilter::CDataPageFilter()
{
	SetAllToHide();
}

/**********************************************************************/

void CDataPageFilter::SetAllToHide()
{
	m_nShowAdjustments = SCOPE_HIDE;
	m_nShowAdjustText = SCOPE_HIDE;
	m_nShowAllergen = SCOPE_HIDE;
	m_nShowAllowance = SCOPE_HIDE;
	m_nShowBarcode = SCOPE_HIDE;
	m_nShowBatchEpos = SCOPE_HIDE;
	m_nShowCategory = SCOPE_HIDE;
	m_nShowCategorySet = SCOPE_HIDE;
	m_nShowConsolGroup = SCOPE_HIDE;
	m_nShowCustomField = SCOPE_HIDE;
	m_nShowCustomFieldSet = SCOPE_HIDE;
	m_nShowDeliveries = SCOPE_HIDE;
	m_nShowDelUnit = SCOPE_HIDE;
	m_nShowDept = SCOPE_HIDE;
	m_nShowDeptSet = SCOPE_HIDE;
	m_nShowEcrSupplier = SCOPE_HIDE;
	m_nShowEposGroup = SCOPE_HIDE;
	m_nShowEposReports = SCOPE_HIDE;
	m_nShowManualSales = SCOPE_HIDE;
	m_nShowMixMatch = SCOPE_HIDE;
	m_nShowWalletStampRule = SCOPE_HIDE;
	m_nShowLoyaltyScheme = SCOPE_HIDE;
	m_nShowModifier = SCOPE_HIDE;
	m_nShowOfferGroup = SCOPE_HIDE;
	m_nShowOpening = SCOPE_HIDE;
	m_nShowPaidInOutText = SCOPE_HIDE;
	m_nShowPayment = SCOPE_HIDE;
	m_nShowPlu = SCOPE_HIDE;
	m_nShowAnalysisCategory = SCOPE_HIDE;
	m_nShowPluFilter = SCOPE_HIDE;
	m_nShowPluUnit = SCOPE_HIDE;
	m_nShowPriceText = SCOPE_HIDE;
	m_nShowPromotion = SCOPE_HIDE;
	m_nShowReorder = SCOPE_HIDE;
	m_nShowActiveStock = SCOPE_HIDE;
	m_nShowReportGroup = SCOPE_HIDE;
	m_nShowReturns = SCOPE_HIDE;
	m_nShowSalesHistoryPlu = SCOPE_HIDE;
	m_nShowSalesHistoryStock = SCOPE_HIDE;
	m_nShowSession = SCOPE_HIDE;
	m_nShowStock = SCOPE_HIDE;
	m_nShowStockLevelReports = SCOPE_HIDE;
	m_nShowStockUnit = SCOPE_HIDE;
	m_nShowSubUnit = SCOPE_HIDE;
	m_nShowSupplier = SCOPE_HIDE;
	m_nShowSupplierSet = SCOPE_HIDE;
	m_nShowTax = SCOPE_HIDE;
	m_nShowTimePlan = SCOPE_HIDE;
	m_nShowTimeSliceMaps = SCOPE_HIDE;
	m_nShowTransfers = SCOPE_HIDE;
	m_nShowUnitech = SCOPE_HIDE;
}

/**********************************************************************/

void CDataPageFilter::PrepareEcrDataFilter()
{
	//LOCAL OR GLOBAL
	m_nShowUnitech = SCOPE_DBASE;
	m_nShowPlu = SCOPE_DBASE;
	m_nShowBarcode = SCOPE_DBASE;
	m_nShowLoyaltyScheme = SCOPE_DBASE;
	
	m_nShowDept = ( EcrmanOptions.GetGlobalDepartmentFlag() ) ? SCOPE_GLOBAL : SCOPE_DBASE;
	m_nShowEposGroup = ( EcrmanOptions.GetGlobalDeptGroupEposFlag() ) ? SCOPE_GLOBAL : SCOPE_DBASE;
	m_nShowReportGroup = ( EcrmanOptions.GetGlobalDeptGroupReportFlag() ) ? SCOPE_GLOBAL : SCOPE_DBASE;
	m_nShowConsolGroup = ( EcrmanOptions.GetGlobalDeptGroupConsolFlag() ) ? SCOPE_GLOBAL : SCOPE_DBASE;
	m_nShowTax = ( EcrmanOptions.GetGlobalTaxFlag() ) ? SCOPE_GLOBAL : SCOPE_DBASE;
	
	//ENABLED OR DISABLED
#ifdef STOCKMAN_SYSTEM
	if ( UnitechOptions.GetEnableFlag() == FALSE )
		m_nShowUnitech = SCOPE_HIDE;
#else
		m_nShowUnitech = SCOPE_HIDE;
#endif

	//PASSWORD CONTROL
	bool bMask = PasswordArray.GetEnable( PasswordTicks::EditEposData );
	if (!(bMask && PasswordArray.GetUnitechPluEnable())) m_nShowUnitech = SCOPE_HIDE;
	if (!(bMask && PasswordArray.GetEnable( PasswordTicks::EditPluRecords ))) m_nShowPlu = SCOPE_HIDE;
	
	if (!(bMask && PasswordArray.GetEnable( PasswordTicks::EditPluRecords ))) 
	{
		m_nShowPlu = SCOPE_HIDE;
		m_nShowBarcode = SCOPE_HIDE;
	}
	
	if (!(bMask && PasswordArray.GetEnable( PasswordTicks::EditDepartment ))) m_nShowDept = SCOPE_HIDE;
	if (!(bMask && PasswordArray.GetEnable( PasswordTicks::EditGroup ))) m_nShowEposGroup = SCOPE_HIDE;
	if (!(bMask && PasswordArray.GetEnable( PasswordTicks::EditTax ))) m_nShowTax = SCOPE_HIDE;

	//LOYALTY
	bool bCanDoLoyaltyScheme = TRUE;
	if (SysInfo.GotLoyaltyLink() == FALSE)
	{
		if (SysInfo.GetLoyaltyOrSmartPayType() != LOYALTY_TYPE_UNKNOWN)
		{
			bCanDoLoyaltyScheme = FALSE;
		}
	}

	if (!(bMask && bCanDoLoyaltyScheme)) m_nShowLoyaltyScheme = SCOPE_HIDE;
}

/**********************************************************************/

void CDataPageFilter::PreparePriceSetFilter()
{
	//LOCAL OR GLOBAL
	m_nShowPlu = SCOPE_DBASE;

	//PASSWORD CONTROL
	bool bMask = PasswordArray.GetEnable(PasswordTicks::EditEposData);
	if (!(bMask && PasswordArray.GetEnable(PasswordTicks::EditPluRecords)))
	{
		m_nShowPlu = SCOPE_HIDE;
	}

	//ENABLED OR DISABLED
	if (EcrmanOptions.GetPluPriceSetsFlag() == FALSE)
	{
		m_nShowPlu = SCOPE_HIDE;
	}

	//LOCATION ACCESS
	bool bGotLocation = FALSE;
	for (int nDbIdx = 0; nDbIdx < dbDatabase.GetSize(); nDbIdx++)
	{
		if (PasswordArray.CheckLocationAccessDbIdx(nDbIdx) == TRUE)
		{
			CArray<int, int> arrayLocSet;
			dbLocation.GetDatabaseLocIdxSet(nDbIdx, arrayLocSet);
			if (arrayLocSet.GetSize() > 0)
			{
				bGotLocation = TRUE;
				break;
			}
		}
	}

	if (FALSE == bGotLocation)
	{
		m_nShowPlu = SCOPE_HIDE;
	}
}

/**********************************************************************/

void CDataPageFilter::PrepareMixMatchFilter()
{	
	//LOCAL OR GLOBAL
	{
		int nMixMatchScope = DealerFlags.GetGlobalPromotionFlag() ? SCOPE_GLOBAL : SCOPE_DBASE; 
		m_nShowMixMatch = nMixMatchScope;
		m_nShowPromotion = nMixMatchScope;
		m_nShowWalletStampRule = nMixMatchScope;
		m_nShowOfferGroup = nMixMatchScope;
		m_nShowTimePlan = nMixMatchScope;
	}

	//ENABLED OR DISABLED
	if ( EcrmanOptions.GetFeaturesMixMatchFlag() == FALSE )
	{
		m_nShowMixMatch = SCOPE_HIDE;
		m_nShowPromotion = SCOPE_HIDE;
		m_nShowTimePlan = SCOPE_HIDE;
	}

	if (EcrmanOptions.GetFeaturesWalletStampRuleFlag() == FALSE)
	{
		m_nShowWalletStampRule = SCOPE_HIDE;
	}

	if ((EcrmanOptions.GetFeaturesMixMatchFlag() == FALSE) && (EcrmanOptions.GetFeaturesWalletStampRuleFlag() == FALSE))
	{
		m_nShowOfferGroup = SCOPE_HIDE;
	}

	if ( EcrmanOptions.GetFeaturesPromotionFlag() == FALSE )
	{
		m_nShowPromotion = SCOPE_HIDE;
	}

	if ( EcrmanOptions.GetFeaturesTimePlanFlag() == FALSE )
	{
		m_nShowTimePlan = SCOPE_HIDE;
	}

	//PASSWORD CONTROL
	bool bMask = PasswordArray.GetEnable( PasswordTicks::EditEposData );
	
	if (!(bMask && PasswordArray.GetEnable( PasswordTicks::EditMixMatch ))) 
	{	
		m_nShowMixMatch = SCOPE_HIDE;
		m_nShowPromotion = SCOPE_HIDE;
		m_nShowOfferGroup = SCOPE_HIDE;
	}

	if (!(bMask && PasswordArray.GetEnable(PasswordTicks::EditTimePlan)))
	{
		m_nShowTimePlan = SCOPE_HIDE;
	}

	if (!(bMask && PasswordArray.GetEnable(PasswordTicks::EditWalletStampRule)))
	{
		m_nShowWalletStampRule = SCOPE_HIDE;
	}
}

/**********************************************************************/

void CDataPageFilter::PrepareEcrTextFilter()
{
	//LOCAL OR GLOBAL
	m_nShowAllergen = SCOPE_GLOBAL;
	m_nShowPluUnit = SCOPE_GLOBAL;
	m_nShowPriceText = SCOPE_GLOBAL;
	m_nShowPayment = ( EcrmanOptions.GetGlobalPaymentFlag() == TRUE ) ? SCOPE_GLOBAL : SCOPE_DBASE;
	m_nShowModifier = ( EcrmanOptions.GetGlobalModifierFlag() == TRUE ) ? SCOPE_GLOBAL : SCOPE_DBASE;
	m_nShowAnalysisCategory = ( EcrmanOptions.GetGlobalAnalysisCategoryFlag() == TRUE ) ? SCOPE_GLOBAL : SCOPE_DBASE;
	m_nShowPaidInOutText = ( EcrmanOptions.GetGlobalPaidInOutFlag() == TRUE ) ? SCOPE_GLOBAL : SCOPE_DBASE;
	//ENABLED OR DISABLED
	if ( EcrmanOptions.GetFeaturesAllergenFlag() == FALSE ) m_nShowAllergen = SCOPE_HIDE;
	//PASSWORD CONTROL
	bool bEposTexts = PasswordArray.GetEnable( PasswordTicks::DatabaseMenu ) && PasswordArray.GetEnable( PasswordTicks::EditEposData );
	if ( ( bEposTexts == FALSE ) || PasswordArray.GetEnable( PasswordTicks::EditAllergen ) == FALSE ) m_nShowAllergen = SCOPE_HIDE;
	if ( ( bEposTexts == FALSE ) || PasswordArray.GetEnable( PasswordTicks::EditPayment ) == FALSE ) m_nShowPayment = SCOPE_HIDE;
	if ( ( bEposTexts == FALSE ) || PasswordArray.GetEnable( PasswordTicks::EditModifier ) == FALSE ) m_nShowModifier = SCOPE_HIDE;
	if ( ( bEposTexts == FALSE ) || PasswordArray.GetEnable( PasswordTicks::EditAnalysisCategory ) == FALSE ) m_nShowAnalysisCategory = SCOPE_HIDE;
	if ( ( bEposTexts == FALSE ) || PasswordArray.GetEnable( PasswordTicks::EditPaidInOutText ) == FALSE ) m_nShowPaidInOutText = SCOPE_HIDE;
	//SYSSET CONTROL
	if ( !Sysset.IsBarmanSystem() ) m_nShowPluUnit = SCOPE_HIDE;
	//8 DIGIT PLU SYSTEM
	if ( SysInfo.GetMaxBasePluLen() == 0 ) m_nShowModifier = SCOPE_HIDE;
}

/**********************************************************************/

void CDataPageFilter::PrepareReportSetFilter()
{
	//LOCAL OR GLOBAL
	m_nShowBatchEpos = SCOPE_GLOBAL;
	m_nShowEposReports = SCOPE_GLOBAL;
	m_nShowSession = SCOPE_GLOBAL;
	m_nShowSalesHistoryPlu = SCOPE_GLOBAL;
	m_nShowTimeSliceMaps = SCOPE_GLOBAL;
	m_nShowPluFilter = SCOPE_DBASE;
	m_nShowDeptSet = ( EcrmanOptions.GetGlobalDepartmentFlag() ) ? SCOPE_GLOBAL : SCOPE_DBASE;
	m_nShowTax = ( EcrmanOptions.GetGlobalTaxFlag() ) ? SCOPE_GLOBAL : SCOPE_DBASE;
	
#ifdef STOCKMAN_SYSTEM
	m_nShowSupplierSet = SCOPE_GLOBAL;
	m_nShowCategorySet = ( StockOptions.GetGlobalCategoryFlag() ) ? SCOPE_GLOBAL : SCOPE_DBASE;
	m_nShowCustomFieldSet = ( StockOptions.GetGlobalCustomFieldFlag() ) ? SCOPE_GLOBAL : SCOPE_DBASE;
	m_nShowSalesHistoryStock = SCOPE_GLOBAL;
	m_nShowStockLevelReports = SCOPE_GLOBAL;
#else
	m_nShowEcrSupplier = SCOPE_GLOBAL;
#endif

	//EPOSMAN OPTIONS
	if ( EcrmanOptions.GetFeaturesPluStockFlag() == FALSE ) m_nShowEcrSupplier = SCOPE_HIDE;
}

/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/

void CDataPageFilter::PrepareStockDataFilter()
{
	//LOCAL OR GLOBAL
	m_nShowStock = SCOPE_DBASE;
	m_nShowOpening = SCOPE_STKPOINT;
	m_nShowDeliveries = SCOPE_STKPOINT;
	m_nShowManualSales = SCOPE_STKPOINT;
	m_nShowReturns = SCOPE_STKPOINT;
	m_nShowAdjustments = SCOPE_STKPOINT;
	m_nShowTransfers = SCOPE_STKPOINT;
	m_nShowReorder = SCOPE_STKPOINT;
	m_nShowActiveStock = SCOPE_STKPOINT;
	m_nShowUnitech = SCOPE_STKPOINT;
	//ENABLED OR DISABLED
	if ( UnitechOptions.GetEnableFlag() == FALSE ) m_nShowUnitech = SCOPE_HIDE;
	//PASSWORD CONTROL
	bool bMask = PasswordArray.GetEnable( PasswordTicks::EditStockData );
	if (!(bMask & PasswordArray.GetEnable( PasswordTicks::EditStockRecords ))) m_nShowStock = SCOPE_HIDE; 
	if (!(bMask & PasswordArray.GetEnable( PasswordTicks::EditOpening ))) m_nShowOpening = SCOPE_HIDE;
	if (!(bMask & PasswordArray.GetEnable( PasswordTicks::EditDeliveries ))) m_nShowDeliveries = SCOPE_HIDE;
	if (!(bMask & PasswordArray.GetEnable( PasswordTicks::EditManualSales ))) m_nShowManualSales = SCOPE_HIDE;
	if (!(bMask & PasswordArray.GetEnable( PasswordTicks::EditReturns ))) m_nShowReturns = SCOPE_HIDE;
	if (!(bMask & PasswordArray.GetEnable( PasswordTicks::EditAdjustments ))) m_nShowAdjustments = SCOPE_HIDE;
	if (!(bMask & PasswordArray.GetEnable( PasswordTicks::EditTransfers ))) m_nShowTransfers = SCOPE_HIDE;
	if (!(bMask & PasswordArray.GetEnable( PasswordTicks::EditReorder ))) m_nShowReorder = SCOPE_HIDE;
	if (!(bMask & PasswordArray.GetEnable( PasswordTicks::EditActiveStock ))) m_nShowActiveStock = SCOPE_HIDE;
	if (!(bMask & PasswordArray.GetUnitechStockEnable())) m_nShowUnitech = SCOPE_HIDE;
		
	if ( dbStockpoint.GetSize() == 0 )
	{
		m_nShowOpening = SCOPE_HIDE;
		m_nShowDeliveries = SCOPE_HIDE;
		m_nShowManualSales = SCOPE_HIDE;
		m_nShowReturns = SCOPE_HIDE;
		m_nShowAdjustments = SCOPE_HIDE;
		m_nShowReorder = SCOPE_HIDE;
		m_nShowActiveStock = SCOPE_HIDE;
		m_nShowUnitech = SCOPE_HIDE;
	}

	//CHECK STOCKPOINT COUNTS
	m_arrayDbTransferTargets.RemoveAll();
	for ( int nDbIdx = 0; nDbIdx < dbDatabase.GetSize(); nDbIdx++ )
	{
		int nStartIdx, nEndIdx;
		dbStockpoint.GetSpIdxRange( nDbIdx, nStartIdx, nEndIdx );
		
		int nTransferTargets = 0;

		for ( int nSpIdx = nStartIdx; nSpIdx <= nEndIdx; nSpIdx++ )
		{
			if ( dbStockpoint.GetTransferInFlag(nSpIdx) == TRUE )
				nTransferTargets++;
		}

		m_arrayDbTransferTargets.Add( nTransferTargets );
	}
}

/**********************************************************************/

void CDataPageFilter::PrepareStockTextFilter()
{
	//LOCAL OR GLOBAL
	m_nShowSupplier = SCOPE_GLOBAL;
	m_nShowStockUnit = SCOPE_GLOBAL;
	m_nShowSubUnit = SCOPE_GLOBAL;
	m_nShowDelUnit = SCOPE_GLOBAL;
	m_nShowAdjustText = SCOPE_GLOBAL;
	m_nShowAllowance = ( StockOptions.GetGlobalAllowanceFlag() ) ? SCOPE_GLOBAL : SCOPE_DBASE;
	m_nShowCategory = ( StockOptions.GetGlobalCategoryFlag() ) ? SCOPE_GLOBAL : SCOPE_DBASE;

	if ( SysInfo.IsRetailStockSystem() == TRUE )
		m_nShowCustomField = ( StockOptions.GetGlobalCustomFieldFlag() ) ? SCOPE_GLOBAL : SCOPE_DBASE;
	else
		m_nShowCustomField = SCOPE_HIDE;
}

/**********************************************************************/
#endif
/**********************************************************************/

bool CDataPageFilter::GotLocalData( int nDataListType )
{
	switch ( nDataListType )
	{
	case DATAPAGE_TYPE_ECRDATA:

		if (m_nShowPlu == SCOPE_DBASE)
		{
			return TRUE;
		}

		if (m_nShowBarcode == SCOPE_DBASE)
		{
			return TRUE;
		}

		if (m_nShowDept == SCOPE_DBASE)
		{
			return TRUE;
		}

		if (m_nShowEposGroup == SCOPE_DBASE)
		{
			return TRUE;
		}

		if (m_nShowTax == SCOPE_DBASE)
		{
			return TRUE;
		}

		if (m_nShowLoyaltyScheme == SCOPE_DBASE)
		{
			return TRUE;
		}

		return FALSE;

	case DATAPAGE_TYPE_PRICESET:

		if ( EcrmanOptions.GetPluPriceSetsFlag() == TRUE )
		{
			if (m_nShowPlu == SCOPE_DBASE)
			{
				return TRUE;
			}
		}

		return FALSE;

	case DATAPAGE_TYPE_MIXMATCH:
		
		if ( m_nShowMixMatch == SCOPE_DBASE )		
			return TRUE;
	
		if ( m_nShowPromotion == SCOPE_DBASE )		
			return TRUE;

		if (m_nShowWalletStampRule == SCOPE_DBASE)
			return TRUE;
	
		if ( m_nShowTimePlan == SCOPE_DBASE )
			return TRUE;

		return FALSE;

	case DATAPAGE_TYPE_STOCKDATA:

		if ( m_nShowStock == SCOPE_DBASE )		
			return TRUE;

		return FALSE;

	case DATAPAGE_TYPE_ECRTEXT:
		
		if ( m_nShowPayment == SCOPE_DBASE )		
			return TRUE;

		if ( m_nShowAnalysisCategory == SCOPE_DBASE )	
			return TRUE;

		if ( m_nShowPaidInOutText == SCOPE_DBASE )	
			return TRUE;

		if ( m_nShowModifier == SCOPE_DBASE )		
			return TRUE;

		return FALSE;

	case DATAPAGE_TYPE_STOCKTEXT:
		
		if ( m_nShowCategory == SCOPE_DBASE )	
			return TRUE;

		if ( m_nShowCustomField == SCOPE_DBASE )	
			return TRUE;

		if ( m_nShowAllowance == SCOPE_DBASE )	
			return TRUE;

		return FALSE;

	case DATAPAGE_TYPE_REPORTSET:

		if ( SysInfo.IsEcrLiteSystem() == FALSE )
		{
			if ( m_nShowPluFilter == SCOPE_DBASE )		return TRUE;
			if ( m_nShowDeptSet == SCOPE_DBASE )		return TRUE;
			if ( m_nShowTax == SCOPE_DBASE )			return TRUE;
			if ( m_nShowCategorySet == SCOPE_DBASE )	return TRUE;
			if ( m_nShowCustomFieldSet == SCOPE_DBASE )	return TRUE;
		}

		return FALSE;

	case DATAPAGE_TYPE_SERVER:
		
		if ( EcrmanOptions.GetFeaturesServerSettingsType() == SERVER_NONE )
			return FALSE;

		if ( ( PasswordArray.GetEnable( PasswordTicks::EditServerDetails ) == FALSE ) &&
			 ( PasswordArray.GetEnable( PasswordTicks::EditServerNames ) == FALSE ) )
					return FALSE;

		return TRUE;

	case DATAPAGE_TYPE_SVRPRESET:
		
		if ( EcrmanOptions.GetFeaturesServerSettingsType() == SERVER_NONE )
			return FALSE;

		if ( PasswordArray.GetEnable( PasswordTicks::EditServerPresets ) == FALSE )
			return FALSE;

		return TRUE;

	case DATAPAGE_TYPE_CUSTOMER:
		
		if ( EcrmanOptions.GetFeaturesCustomerBalanceFlag() == FALSE )
			return FALSE;

		if ( PasswordArray.GetEnable( PasswordTicks::EditCustomers ) == FALSE )
			return FALSE;

		return TRUE;

	default:
		return FALSE;
	}
}

/**********************************************************************/

bool CDataPageFilter::GotGlobalData( int nDataListType )
{
	switch ( nDataListType )
	{
	case DATAPAGE_TYPE_ECRDATA:
		if ( m_nShowDept == SCOPE_GLOBAL )			return TRUE;
		if ( m_nShowTax == SCOPE_GLOBAL )			return TRUE;	
		if ( m_nShowEposGroup == SCOPE_GLOBAL )		return TRUE;
		return FALSE;

	case DATAPAGE_TYPE_STOCKDATA:
		return FALSE;

	case DATAPAGE_TYPE_ECRTEXT:
		if ( m_nShowPluUnit == SCOPE_GLOBAL )		return TRUE;
		if ( m_nShowPriceText == SCOPE_GLOBAL )		return TRUE;
		if ( m_nShowModifier == SCOPE_GLOBAL )		return TRUE;
		if ( m_nShowPayment == SCOPE_GLOBAL )		return TRUE;
		if ( m_nShowAnalysisCategory == SCOPE_GLOBAL )	return TRUE;
		if ( m_nShowPaidInOutText == SCOPE_GLOBAL )	return TRUE;
		return FALSE;	

	case DATAPAGE_TYPE_STOCKTEXT:
		if ( m_nShowSupplier == SCOPE_GLOBAL )	return TRUE;
		if ( m_nShowCategory == SCOPE_GLOBAL )	return TRUE;
		if ( m_nShowCustomField == SCOPE_GLOBAL )	return TRUE;
		if ( m_nShowAllowance == SCOPE_GLOBAL )	return TRUE;
		if ( m_nShowStockUnit == SCOPE_GLOBAL )	return TRUE;
		if ( m_nShowSubUnit == SCOPE_GLOBAL )	return TRUE;
		if ( m_nShowDelUnit == SCOPE_GLOBAL )	return TRUE;
		if ( m_nShowAdjustText == SCOPE_GLOBAL ) return TRUE;
		return FALSE;

	case DATAPAGE_TYPE_REPORTSET:
		if ( SysInfo.IsEcrLiteSystem() == FALSE )
		{
			if ( m_nShowDeptSet == SCOPE_GLOBAL )				return TRUE;
			if ( m_nShowTax == SCOPE_GLOBAL )					return TRUE;
			if ( m_nShowCategorySet == SCOPE_GLOBAL )			return TRUE;
			if ( m_nShowCustomFieldSet == SCOPE_GLOBAL )		return TRUE;
			if ( m_nShowEcrSupplier == SCOPE_GLOBAL )			return TRUE;
		}

#ifdef STOCKMAN_SYSTEM
		if ( m_nShowSupplierSet == SCOPE_GLOBAL )			return TRUE;
#endif	
		
		return FALSE;

	case DATAPAGE_TYPE_MIXMATCH:
		if ( m_nShowMixMatch == SCOPE_GLOBAL )				return TRUE;
		if ( m_nShowPromotion == SCOPE_GLOBAL )				return TRUE;
		if ( m_nShowWalletStampRule == SCOPE_GLOBAL)				return TRUE;
		if ( m_nShowTimePlan == SCOPE_GLOBAL )				return TRUE;
		return FALSE;
	
	default:
		return FALSE;
	}
}

/**********************************************************************/

bool CDataPageFilter::GotTimeSettingData( int nDataListType )
{
	switch ( nDataListType )
	{
	case DATAPAGE_TYPE_REPORTSET:
		if ( m_nShowSession == SCOPE_GLOBAL )				return TRUE;
		if ( m_nShowTimeSliceMaps == SCOPE_GLOBAL )			return TRUE;
		return FALSE;

	default:
		return FALSE;
	}
}

/**********************************************************************/

bool CDataPageFilter::GotReportListData( int nDataListType )
{
	switch ( nDataListType )
	{
	case DATAPAGE_TYPE_REPORTSET:
		if ( m_nShowBatchEpos == SCOPE_GLOBAL )				return TRUE;
		if ( m_nShowEposReports == SCOPE_GLOBAL )			return TRUE;
		if ( m_nShowSalesHistoryPlu == SCOPE_GLOBAL )		return TRUE;

#ifdef STOCKMAN_SYSTEM
		if ( m_nShowSalesHistoryStock == SCOPE_GLOBAL )		return TRUE;
		if ( m_nShowStockLevelReports == SCOPE_GLOBAL )		return TRUE;
#endif	
	
		return FALSE;

	default:
		return FALSE;
	}
}

/**********************************************************************/

bool CDataPageFilter::GotSystemFilterData( int nDataListType )
{
	if ( ( Sysset.GetMaxDatabases() < 2 ) || ( dbDatabase.GetSize() < 2 ) )
	{
		return FALSE;
	}
	else
	{
		switch ( nDataListType )
		{
		case DATAPAGE_TYPE_ECRDATA:
			if ( m_nShowDept == SCOPE_DBASE ) return TRUE;
			return FALSE;

		case DATAPAGE_TYPE_STOCKDATA:
			return FALSE;

		case DATAPAGE_TYPE_ECRTEXT:
			return FALSE;	

		case DATAPAGE_TYPE_STOCKTEXT:
			if ( m_nShowCategory == SCOPE_DBASE ) return TRUE;
			return FALSE;

		case DATAPAGE_TYPE_REPORTSET:
			if ( SysInfo.IsEcrLiteSystem() == FALSE )
			{
				if ( m_nShowDeptSet == SCOPE_DBASE )				return TRUE;
				if ( m_nShowCategorySet == SCOPE_DBASE )			return TRUE;
				if ( m_nShowPluFilter == SCOPE_DBASE )				return TRUE;
				if ( m_nShowCustomFieldSet == SCOPE_DBASE )			return TRUE;
			}	
			return FALSE;
		
		default:
			return FALSE;
		}
	}
}

/**********************************************************************/

bool CDataPageFilter::GotStockpointData( int nDataListType )
{

#ifndef STOCKMAN_SYSTEM

	return FALSE;

#else

	switch ( nDataListType )
	{
	case DATAPAGE_TYPE_STOCKDATA:
		if ( m_nShowOpening == SCOPE_STKPOINT )		return TRUE;
		if ( m_nShowDeliveries == SCOPE_STKPOINT )	return TRUE;
		if ( m_nShowManualSales == SCOPE_STKPOINT )	return TRUE;
		if ( m_nShowReturns == SCOPE_STKPOINT )		return TRUE;
		if ( m_nShowAdjustments == SCOPE_STKPOINT )	return TRUE;
		if ( m_nShowTransfers == SCOPE_STKPOINT )	return TRUE;
		if ( m_nShowReorder == SCOPE_STKPOINT )		return TRUE;
		if ( m_nShowActiveStock == SCOPE_STKPOINT )	return TRUE;
		if ( m_nShowUnitech == SCOPE_STKPOINT )		return TRUE;
		return FALSE;
	}
	return FALSE;

#endif
}

/**********************************************************************/

int CDataPageFilter::GetDbTransferTargets( int nDbIdx )
{
	if ( ( nDbIdx >= 0 ) && ( nDbIdx < m_arrayDbTransferTargets.GetSize() ) )
		return m_arrayDbTransferTargets.GetAt( nDbIdx );
	else
		return 0;
}

/**********************************************************************/
