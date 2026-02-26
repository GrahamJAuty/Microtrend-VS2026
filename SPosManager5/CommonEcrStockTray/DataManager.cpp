 /**********************************************************************/
#include "..\CommonEcrStock\PriceSetItemManager.h"
 /**********************************************************************/
#include "FileRemover.h"
#include "LocationCSVArray.h"
#include "NodeTypes.h"
#include "SysInfo.h"
/**********************************************************************/
#include "DataManager.h"
/**********************************************************************/

#ifndef POSTRAY_UTILITY

CDataManager::CDataManager() : CDataManagerBase(), 
	ShelfLabels( FALSE ),
	OfferGroup( NODE_OFFER_GROUP ),
	TimePlanEntry(FALSE),
	TimePlanTemplateEntry(TRUE),
	DbPluFilterNames(NODE_PLUFILTER_NAME)

#else

CDataManager::CDataManager() : CDataManagerBase(),
	TimePlanEntry(FALSE),
	TimePlanTemplateEntry(TRUE),
	DbPluFilterNames(NODE_PLUFILTER_NAME)

#endif
	
{
	m_nUserLockCount = 0;
	m_nActiveDbIdx = -1;
	m_bPluPriceSetMode = FALSE;
	
	m_nStatusBarcode = DATAMANAGER_CLOSED;
	m_nStatusDepartment = DATAMANAGER_CLOSED;
	m_nStatusDepartmentSet = DATAMANAGER_CLOSED;
	m_nStatusHistoricalTax = DATAMANAGER_CLOSED;
	m_nStatusPaidInText = DATAMANAGER_CLOSED;
	m_nStatusPaidOutText = DATAMANAGER_CLOSED;
	m_nStatusPayment = DATAMANAGER_CLOSED;
	m_nStatusPaymentGroup = DATAMANAGER_CLOSED;
	m_nStatusSBPaymentMap = DATAMANAGER_CLOSED;
	m_nStatusSEPaymentMap = DATAMANAGER_CLOSED;
	m_nStatusAnalysisCategory = DATAMANAGER_CLOSED;
	m_nStatusDbPluFilterNames = DATAMANAGER_CLOSED;
	m_nStatusDbPluFilterItems = DATAMANAGER_CLOSED;
	m_nStatusPlu = DATAMANAGER_CLOSED;
	m_nStatusPriceText = DATAMANAGER_CLOSED;
	m_nStatusSBPriceText = DATAMANAGER_CLOSED;
	m_nStatusPromotion = DATAMANAGER_CLOSED;
	m_nStatusEposGroup = DATAMANAGER_CLOSED;
	m_nStatusReportGroup = DATAMANAGER_CLOSED;
	m_nStatusConsolGroup = DATAMANAGER_CLOSED;
	m_nStatusMixMatch = DATAMANAGER_CLOSED;
	m_nStatusModifier = DATAMANAGER_CLOSED;
	m_nStatusOfferGroup = DATAMANAGER_CLOSED;
	m_nStatusOfferGroupEntry = DATAMANAGER_CLOSED;
	m_nStatusTaxRates = DATAMANAGER_CLOSED;
	m_nStatusTimePlan = DATAMANAGER_CLOSED;
	m_nStatusTimePlanEntry = DATAMANAGER_CLOSED;
	m_nStatusTimePlanTemplateEntry = DATAMANAGER_CLOSED;
	m_nStatusWalletStampRule = DATAMANAGER_CLOSED;
	m_nStatusLoyaltyScheme = DATAMANAGER_CLOSED;
	
	m_nStatusShelfLabels = DATAMANAGER_CLOSED;
}	
/**********************************************************************/

CDataManager::~CDataManager()
{
}

/**********************************************************************/

bool CDataManager::CloseStringArray( CSharedStringArray& array, CDataManagerInfo& info, int& nCurrentState )
{
	if ( m_nActiveDbIdx == -1 )
	{
		info.m_nErrorType = DATAMANAGER_ERROR_NOTOPEN;
		return FALSE;
	}

	return CDataManagerBase::CloseStringArray( array, info, nCurrentState );
}

/**********************************************************************/

bool CDataManager::WriteStringArray( CSharedStringArray& array, CDataManagerInfo& info, int& nCurrentState )
{
	if ( m_nActiveDbIdx == -1 )
	{
		info.m_nErrorType = DATAMANAGER_ERROR_NOTOPEN;
		return FALSE;
	}

	return CDataManagerBase::WriteStringArray( array, info, nCurrentState );
}

/**********************************************************************/

bool CDataManager::CloseCSVArray( CSharedCSVArray& array, CDataManagerInfo& info, int& nCurrentState )
{
	if ( m_nActiveDbIdx == -1 )
	{
		info.m_nErrorType = DATAMANAGER_ERROR_NOTOPEN;
		return FALSE;
	}

	return CDataManagerBase::CloseCSVArray( array, info, nCurrentState );
}

/**********************************************************************/

bool CDataManager::WriteCSVArray( CSharedCSVArray& array, CDataManagerInfo& info, int& nCurrentState )
{
	if ( m_nActiveDbIdx == -1 )
	{
		info.m_nErrorType = DATAMANAGER_ERROR_NOTOPEN;
		return FALSE;
	}

	return CDataManagerBase::WriteCSVArray( array, info, nCurrentState );
}

/**********************************************************************/

void CDataManager::OpenDatabaseQuickNoFiles( int nDbIdx, bool bKeepIfAlreadyOpen )
{
	if ( ( FALSE == bKeepIfAlreadyOpen ) || ( m_nActiveDbIdx != nDbIdx ) )
	{
		CloseDatabase();
		m_nActiveDbIdx = nDbIdx;
	}
}

/**********************************************************************/

bool CDataManager::OpenDatabaseReadOnly( int nDbIdx, CDataManagerInfo& info, bool bDummyStockLevels )
{
	bool bSameDb = ( m_nActiveDbIdx == nDbIdx );
	m_nActiveDbIdx = nDbIdx;

	if ( ( m_nStatusPlu != DATAMANAGER_OPEN_READWRITE ) || ( bSameDb == FALSE ) )
	{
		ClosePlu( info );
		OpenPlu( DB_READONLY, info ); 
	}

	if ( ( m_nStatusBarcode != DATAMANAGER_OPEN_READWRITE ) || ( bSameDb == FALSE ) )
	{
		CloseBarcode( info );
		OpenBarcode( DB_READONLY, info );
	}

	if ( ( m_nStatusDepartment != DATAMANAGER_OPEN_READWRITE ) || ( bSameDb == FALSE ) )
	{
		CloseDepartment( info );
		OpenDepartment( DB_READONLY, info );
	}

	if ( ( m_nStatusDepartmentSet != DATAMANAGER_OPEN_READWRITE ) || ( bSameDb == FALSE ) )
	{
		CloseDepartmentSet( info );
		OpenDepartmentSet( DB_READONLY, info );
	}

	if ( ( m_nStatusEposGroup != DATAMANAGER_OPEN_READWRITE ) || ( bSameDb == FALSE ) )
	{
		CloseEposGroup( info );
		OpenEposGroup( DB_READONLY, info );
	}

	if ( ( m_nStatusReportGroup != DATAMANAGER_OPEN_READWRITE ) || ( bSameDb == FALSE ) )
	{
		CloseReportGroup( info );
		OpenReportGroup( DB_READONLY, info );
	}

	if ( ( m_nStatusConsolGroup != DATAMANAGER_OPEN_READWRITE ) || ( bSameDb == FALSE ) )
	{
		CloseConsolGroup( info );
		OpenConsolGroup( DB_READONLY, info );
	}

	if ( ( m_nStatusHistoricalTax != DATAMANAGER_OPEN_READWRITE ) || ( bSameDb == FALSE ) )
	{
		CloseHistoricalTax( info );
		OpenHistoricalTax( DB_READONLY, info );
	}
	
	if ( ( m_nStatusMixMatch != DATAMANAGER_OPEN_READWRITE ) || ( bSameDb == FALSE ) )
	{
		CloseMixMatch( info );
		OpenMixMatch( DB_READONLY, info );
	}
	
	if ( ( m_nStatusModifier != DATAMANAGER_OPEN_READWRITE ) || ( bSameDb == FALSE ) )
	{
		CloseModifier( info );
		OpenModifier( DB_READONLY, info );
	}
	
#ifndef POSTRAY_UTILITY

	if ( ( m_nStatusOfferGroup != DATAMANAGER_OPEN_READWRITE ) || ( bSameDb == FALSE ) )
	{
		CloseOfferGroup( info );
		OpenOfferGroup( DB_READONLY, info );
	}
	
	if ( ( m_nStatusOfferGroupEntry != DATAMANAGER_OPEN_READWRITE ) || ( bSameDb == FALSE ) )
	{
		CloseOfferGroupEntry( info );
		OpenOfferGroupEntry( DB_READONLY, info );
	}

	if ((m_nStatusLoyaltyScheme != DATAMANAGER_OPEN_READWRITE) || (bSameDb == FALSE))
	{
		CloseLoyaltyScheme(info);
		OpenLoyaltyScheme(DB_READONLY, info);
	}

#endif

	if ( ( m_nStatusPaidInText != DATAMANAGER_OPEN_READWRITE ) || ( bSameDb == FALSE ) )
	{
		ClosePaidInText( info );
		OpenPaidInText( DB_READONLY, info );
	}

	if ( ( m_nStatusPaidOutText != DATAMANAGER_OPEN_READWRITE ) || ( bSameDb == FALSE ) )
	{
		ClosePaidOutText( info );
		OpenPaidOutText( DB_READONLY, info );
	}
	
	if ( ( m_nStatusPayment != DATAMANAGER_OPEN_READWRITE ) || ( bSameDb == FALSE ) )
	{
		ClosePayment( info );
		OpenPayment( DB_READONLY, info );
	}

	if ( ( m_nStatusPaymentGroup != DATAMANAGER_OPEN_READWRITE ) || ( bSameDb == FALSE ) )
	{
		ClosePaymentGroup( info );
		OpenPaymentGroup( DB_READONLY, info );
	}

	if ( ( m_nStatusSBPaymentMap != DATAMANAGER_OPEN_READWRITE ) || ( bSameDb == FALSE ) )
	{
		CloseSBPaymentMap( info );
		OpenSBPaymentMap( DB_READONLY, info );
	}

	if ( ( m_nStatusSEPaymentMap != DATAMANAGER_OPEN_READWRITE ) || ( bSameDb == FALSE ) )
	{
		CloseSEPaymentMap( info );
		OpenSEPaymentMap( DB_READONLY, info );
	}

	if ( ( m_nStatusAnalysisCategory != DATAMANAGER_OPEN_READWRITE ) || ( bSameDb == FALSE ) )
	{
		CloseAnalysisCategory( info );
		OpenAnalysisCategory( DB_READONLY, info );
	}

	if ( ( m_nStatusDbPluFilterNames != DATAMANAGER_OPEN_READWRITE ) || ( bSameDb == FALSE ) )
	{
		CloseDbPluFilterNames( info );
		OpenDbPluFilterNames( DB_READONLY, info );
	}
	
	if ( ( m_nStatusDbPluFilterItems != DATAMANAGER_OPEN_READWRITE ) || ( bSameDb == FALSE ) )
	{
		CloseDbPluFilterItems( info );
		OpenDbPluFilterItems( DB_READONLY, info );
	}

	if ( ( m_nStatusPriceText != DATAMANAGER_OPEN_READWRITE ) || ( bSameDb == FALSE ) )
	{
		ClosePriceText( info );
		OpenPriceText( DB_READONLY, info );
	}
	
	if ( ( m_nStatusPromotion != DATAMANAGER_OPEN_READWRITE ) || ( bSameDb == FALSE ) )
	{
		ClosePromotion( info );
		OpenPromotion( DB_READONLY, info );
	}

	if ( ( m_nStatusSBPriceText != DATAMANAGER_OPEN_READWRITE ) || ( bSameDb == FALSE ) )
	{
		CloseSBPriceText( info );
		OpenSBPriceText( DB_READONLY, info );
	}
	
	if ( ( m_nStatusTaxRates != DATAMANAGER_OPEN_READWRITE ) || ( bSameDb == FALSE ) )
	{
		CloseTaxRates( info );
		OpenTaxRates( DB_READONLY, info );
	}
	
	if ( ( m_nStatusTimePlan != DATAMANAGER_OPEN_READWRITE ) || ( bSameDb == FALSE ) )
	{
		CloseTimePlan( info );
		OpenTimePlan( DB_READONLY, info );
	}
	
	if ( ( m_nStatusTimePlanEntry != DATAMANAGER_OPEN_READWRITE ) || ( bSameDb == FALSE ) )
	{
		CloseTimePlanEntry( info );
		OpenTimePlanEntry( DB_READONLY, info );
	}

	if ((m_nStatusTimePlanTemplateEntry != DATAMANAGER_OPEN_READWRITE) || (bSameDb == FALSE))
	{
		CloseTimePlanTemplateEntry(info);
		OpenTimePlanTemplateEntry(DB_READONLY, info);
	}

	if ((m_nStatusWalletStampRule != DATAMANAGER_OPEN_READWRITE) || (bSameDb == FALSE))
	{
		CloseWalletStampRule(info);
		OpenWalletStampRule(DB_READONLY, info);
	}

	OpenSpecialReadOnly( bSameDb, info );

	info.m_nNodeType = 0;
	info.m_nErrorType = DATAMANAGER_ERROR_NONE;

	return TRUE;
}

/**********************************************************************/

bool CDataManager::OpenDatabaseReadWrite( int nDbIdx, CWordArray& arrayReadWrite, CDataManagerInfo& info, bool bDummyStockLevels )
{
	CWordArray arrayDummy;
	return OpenDatabaseReadWrite( nDbIdx, arrayReadWrite, arrayDummy, DB_READONLY, info );
}

/**********************************************************************/

bool CDataManager::OpenDatabaseReadWrite( int nDbIdx, CWordArray& arrayReadWrite, CWordArray& arrayReadOnly, CDataManagerInfo& info, bool bDummyStockLevels )
{
	return OpenDatabaseReadWrite( nDbIdx, arrayReadWrite, arrayReadOnly, 0, info );
}

/**********************************************************************/

bool CDataManager::OpenDatabaseReadWrite( int nDbIdx, CWordArray& arrayReadWrite, CWordArray& arrayReadOnly, int nDefaultType, CDataManagerInfo& info )
{
	CloseDatabase();

	int nAccessBarcode = nDefaultType;
	int nAccessDepartment = nDefaultType;
	int nAccessDepartmentSet = nDefaultType;
	int nAccessHistoricalTax = nDefaultType;
	int nAccessPayment = nDefaultType;
	int nAccessPaymentGroup = nDefaultType;
	int nAccessSBPaymentMap = nDefaultType;
	int nAccessSEPaymentMap = nDefaultType;
	int nAccessAnalysisCategory = nDefaultType;
	int nAccessPlu = nDefaultType;
	int nAccessDbPluFilterNames = nDefaultType;
	int nAccessDbPluFilterItems = nDefaultType;
	int nAccessEposGroup = nDefaultType;
	int nAccessReportGroup = nDefaultType;
	int nAccessConsolGroup = nDefaultType;
	int nAccessModifier = nDefaultType;
	int nAccessMixMatch = nDefaultType;
	int nAccessOfferGroup = nDefaultType;
	int nAccessOfferGroupEntry = nDefaultType;
	int nAccessPaidInText = nDefaultType;
	int nAccessPaidOutText = nDefaultType;
	int nAccessPriceText = nDefaultType;
	int nAccessPromotion = nDefaultType;
	int nAccessSBPriceText = nDefaultType;
	int nAccessTaxRates = nDefaultType;
	int nAccessTimePlan = nDefaultType;
	int nAccessTimePlanEntry = nDefaultType;
	int nAccessTimePlanTemplate = nDefaultType;
	int nAccessTimePlanTemplateEntry = nDefaultType;
	int nAccessWalletStampRule = nDefaultType;
	int nAccessLoyaltyScheme = nDefaultType;
	
	for ( int n = 0; n < arrayReadWrite.GetSize(); n++ )
	{
		switch( arrayReadWrite.GetAt(n) )
		{
		case NODE_BARCODE:				nAccessBarcode = DB_READWRITE;			break;
		case NODE_DEPARTMENT:			nAccessDepartment = DB_READWRITE;		break;
		case NODE_DEPARTMENT_SET:		nAccessDepartmentSet = DB_READWRITE;	break;
		case NODE_DEPT_GROUP_EPOS:		nAccessEposGroup = DB_READWRITE;		break;
		case NODE_DEPT_GROUP_REPORT:	nAccessReportGroup = DB_READWRITE;		break;
		case NODE_DEPT_GROUP_CONSOL:	nAccessConsolGroup = DB_READWRITE;		break;
		case NODE_MODIFIER:				nAccessModifier = DB_READWRITE;			break;
		case NODE_MIXMATCH:				nAccessMixMatch = DB_READWRITE;			break;
		case NODE_OFFER_GROUP:			nAccessOfferGroup = DB_READWRITE;		break;
		case NODE_OFFER_ENTRY:			nAccessOfferGroupEntry = DB_READWRITE;	break;
		case NODE_PAIDIN_TEXT:			nAccessPaidInText = DB_READWRITE;		break;
		case NODE_PAIDOUT_TEXT:			nAccessPaidOutText = DB_READWRITE;		break;
		case NODE_PAYMENT:				nAccessPayment = DB_READWRITE;			break;
		case NODE_PAYMENT_GROUP:		nAccessPaymentGroup = DB_READWRITE;		break;
		case NODE_SBPAYMENTMAP:			nAccessSBPaymentMap = DB_READWRITE;		break;
		case NODE_SEPAYMENTMAP:			nAccessSEPaymentMap = DB_READWRITE;		break;
		case NODE_ACAT:					nAccessAnalysisCategory = DB_READWRITE;	break;
		case NODE_PLU:					nAccessPlu = DB_READWRITE;				break;
		case NODE_PLUFILTER_NAME:		nAccessDbPluFilterNames = DB_READWRITE;	break;
		case NODE_PLUFILTER_ITEM:		nAccessDbPluFilterItems = DB_READWRITE;	break;
		case NODE_PRICETEXT:			nAccessPriceText = DB_READWRITE;		break;
		case NODE_PROMOTION:			nAccessPromotion = DB_READWRITE;		break;
		case NODE_SBPRICETEXT:			nAccessSBPriceText = DB_READWRITE;		break;
		case NODE_TAX:					nAccessTaxRates = DB_READWRITE;			break;
		case NODE_TAX_DATE:				nAccessHistoricalTax = DB_READWRITE;	break;
		case NODE_TIMEPLAN:				nAccessTimePlan = DB_READWRITE;			break;
		case NODE_TIMEPLAN_ENTRY:		nAccessTimePlanEntry = DB_READWRITE;	break;
		case NODE_TIMEPLAN_TEMPLATE_ENTRY:	nAccessTimePlanTemplateEntry = DB_READWRITE;	break;
		case NODE_WALLETSTAMPRULE:		nAccessWalletStampRule = DB_READWRITE;	break;
		case NODE_LOYALTY_SCHEME:		nAccessLoyaltyScheme = DB_READWRITE;	break;
		}
	}

	for ( int n = 0; n < arrayReadOnly.GetSize(); n++ )
	{
		switch( arrayReadOnly.GetAt(n) )
		{
		case NODE_BARCODE:				nAccessBarcode = DB_READONLY;			break;
		case NODE_DEPARTMENT:			nAccessDepartment = DB_READONLY;		break;
		case NODE_DEPARTMENT_SET:		nAccessDepartmentSet = DB_READONLY;		break;
		case NODE_OFFER_GROUP:			nAccessOfferGroup = DB_READONLY;		break;
		case NODE_OFFER_ENTRY:			nAccessOfferGroupEntry = DB_READONLY;	break;
		case NODE_DEPT_GROUP_EPOS:		nAccessEposGroup = DB_READONLY;			break;
		case NODE_DEPT_GROUP_REPORT:	nAccessReportGroup = DB_READONLY;		break;
		case NODE_DEPT_GROUP_CONSOL:	nAccessConsolGroup = DB_READONLY;		break;
		case NODE_MIXMATCH:				nAccessMixMatch = DB_READONLY;			break;
		case NODE_MODIFIER:				nAccessModifier = DB_READONLY;			break;
		case NODE_PAIDIN_TEXT:			nAccessPaidInText = DB_READONLY;		break;
		case NODE_PAIDOUT_TEXT:			nAccessPaidOutText = DB_READONLY;		break;
		case NODE_PAYMENT:				nAccessPayment = DB_READONLY;			break;
		case NODE_PAYMENT_GROUP:		nAccessPaymentGroup = DB_READONLY;		break;
		case NODE_SBPAYMENTMAP:			nAccessSBPaymentMap = DB_READONLY;		break;
		case NODE_SEPAYMENTMAP:			nAccessSEPaymentMap = DB_READONLY;		break;
		case NODE_ACAT:					nAccessAnalysisCategory = DB_READONLY;	break;
		case NODE_PLU:					nAccessPlu = DB_READONLY;				break;
		case NODE_PLUFILTER_NAME:		nAccessDbPluFilterNames = DB_READONLY;	break;
		case NODE_PLUFILTER_ITEM:		nAccessDbPluFilterItems = DB_READONLY;	break;
		case NODE_PRICETEXT:			nAccessPriceText = DB_READONLY;			break;
		case NODE_PROMOTION:			nAccessPromotion = DB_READONLY;			break;
		case NODE_SBPRICETEXT:			nAccessSBPriceText = DB_READONLY;		break;
		case NODE_TAX:					nAccessTaxRates = DB_READONLY;			break;
		case NODE_TAX_DATE:				nAccessHistoricalTax = DB_READONLY;		break;
		case NODE_TIMEPLAN:				nAccessTimePlan = DB_READONLY;			break;
		case NODE_TIMEPLAN_ENTRY:		nAccessTimePlanEntry = DB_READONLY;		break;
		case NODE_TIMEPLAN_TEMPLATE_ENTRY: nAccessTimePlanTemplateEntry = DB_READONLY; break;
		case NODE_WALLETSTAMPRULE:		nAccessWalletStampRule = DB_READONLY;	break;
		case NODE_LOYALTY_SCHEME:		nAccessLoyaltyScheme = DB_READONLY;		break;
		}
	}

	m_nActiveDbIdx = nDbIdx;

	if ( nAccessPlu != 0 )
	{
		if ( OpenPlu( nAccessPlu, info ) == FALSE )
		{
			CloseDatabase();
			return FALSE;
		}
	}

	if ( nAccessBarcode != 0 )
	{
		if ( OpenBarcode( nAccessBarcode, info ) == FALSE )
		{
			CloseDatabase();
			return FALSE;
		}
	}

	if ( nAccessDepartment != 0 )
	{
		if ( OpenDepartment( nAccessDepartment, info ) == FALSE )
		{
			CloseDatabase();
			return FALSE;
		}
	}

	if ( nAccessDepartmentSet != 0 )
	{
		if ( OpenDepartmentSet( nAccessDepartmentSet, info ) == FALSE )
		{
			CloseDatabase();
			return FALSE;
		}
	}

	if ( nAccessEposGroup != 0 )
	{
		if ( OpenEposGroup( nAccessEposGroup, info ) == FALSE )
		{
			CloseDatabase();
			return FALSE;
		}
	}

	if ( nAccessReportGroup != 0 )
	{
		if ( OpenReportGroup( nAccessReportGroup, info ) == FALSE )
		{
			CloseDatabase();
			return FALSE;
		}
	}

	if ( nAccessConsolGroup != 0 )
	{
		if ( OpenConsolGroup( nAccessConsolGroup, info ) == FALSE )
		{
			CloseDatabase();
			return FALSE;
		}
	}

	if ( nAccessHistoricalTax != 0 )
	{
		if ( OpenHistoricalTax( nAccessHistoricalTax, info ) == FALSE )
		{
			CloseDatabase();
			return FALSE;
		}
	}

	if ( nAccessMixMatch != 0 )
	{
		if ( OpenMixMatch( nAccessMixMatch, info ) == FALSE )
		{
			CloseDatabase();
			return FALSE;
		}
	}

	if ( nAccessModifier != 0 )
	{
		if ( OpenModifier( nAccessModifier, info ) == FALSE )
		{
			CloseDatabase();
			return FALSE;
		}
	}

#ifndef POSTRAY_UTILITY

	if ( nAccessOfferGroup != 0 )
	{
		if ( OpenOfferGroup( nAccessOfferGroup, info ) == FALSE )
		{
			CloseDatabase();
			return FALSE;
		}
	}

	if ( nAccessOfferGroupEntry != 0 )
	{
		if ( OpenOfferGroupEntry( nAccessOfferGroupEntry, info ) == FALSE )
		{
			CloseDatabase();
			return FALSE;
		}
	}

	if (nAccessLoyaltyScheme != 0)
	{
		if (OpenLoyaltyScheme(nAccessLoyaltyScheme, info) == FALSE)
		{
			CloseDatabase();
			return FALSE;
		}
	}

#endif

	if ( nAccessPaidInText != 0 )
	{
		if ( OpenPaidInText( nAccessPaidInText, info ) == FALSE )
		{
			CloseDatabase();
			return FALSE;
		}
	}

	if ( nAccessPaidOutText != 0 )
	{
		if ( OpenPaidOutText( nAccessPaidOutText, info ) == FALSE )
		{
			CloseDatabase();
			return FALSE;
		}
	}

	if ( nAccessPayment != 0 )
	{
		if ( OpenPayment( nAccessPayment, info ) == FALSE )
		{
			CloseDatabase();
			return FALSE;
		}
	}

	if ( nAccessPaymentGroup != 0 )
	{
		if ( OpenPaymentGroup( nAccessPaymentGroup, info ) == FALSE )
		{
			CloseDatabase();
			return FALSE;
		}
	}

	if ( nAccessSBPaymentMap != 0 )
	{
		if ( OpenSBPaymentMap( nAccessSBPaymentMap, info ) == FALSE )
		{
			CloseDatabase();
			return FALSE;
		}
	}

	if ( nAccessSEPaymentMap != 0 )
	{
		if ( OpenSEPaymentMap( nAccessSEPaymentMap, info ) == FALSE )
		{
			CloseDatabase();
			return FALSE;
		}
	}

	if ( nAccessAnalysisCategory != 0 )
	{
		if ( OpenAnalysisCategory( nAccessAnalysisCategory, info ) == FALSE )
		{
			CloseDatabase();
			return FALSE;
		}
	}

	if ( nAccessDbPluFilterNames != 0 )
	{
		if ( OpenDbPluFilterNames( nAccessDbPluFilterNames, info ) == FALSE )
		{
			CloseDatabase();
			return FALSE;
		}
	}

	if ( nAccessDbPluFilterItems != 0 )
	{
		if ( OpenDbPluFilterItems( nAccessDbPluFilterItems, info ) == FALSE )
		{
			CloseDatabase();
			return FALSE;
		}
	}

	if ( nAccessPriceText != 0 )
	{
		if ( OpenPriceText( nAccessPriceText, info ) == FALSE )
		{
			CloseDatabase();
			return FALSE;
		}
	}

	if ( nAccessPromotion != 0 )
	{
		if ( OpenPromotion( nAccessPromotion, info ) == FALSE )
		{
			CloseDatabase();
			return FALSE;
		}
	}

	if ( nAccessSBPriceText != 0 )
	{
		if ( OpenSBPriceText( nAccessSBPriceText, info ) == FALSE )
		{
			CloseDatabase();
			return FALSE;
		}
	}

	if ( nAccessTaxRates != 0 )
	{
		if ( OpenTaxRates( nAccessTaxRates, info ) == FALSE )
		{
			CloseDatabase();
			return FALSE;
		}
	}

	if ( nAccessTimePlan != 0 )
	{
		if ( OpenTimePlan( nAccessTimePlan, info ) == FALSE )
		{
			CloseDatabase();
			return FALSE;
		}
	}

	if ( nAccessTimePlanEntry != 0 )
	{
		if ( OpenTimePlanEntry( nAccessTimePlanEntry, info ) == FALSE )
		{
			CloseDatabase();
			return FALSE;
		}
	}

	if (nAccessTimePlanTemplateEntry != 0)
	{
		if (OpenTimePlanTemplateEntry(nAccessTimePlanTemplateEntry, info) == FALSE)
		{
			CloseDatabase();
			return FALSE;
		}
	}

	if (nAccessWalletStampRule != 0)
	{
		if (OpenWalletStampRule(nAccessWalletStampRule, info) == FALSE)
		{
			CloseDatabase();
			return FALSE;
		}
	}

	if ( OpenSpecialReadWrite( nDbIdx, arrayReadWrite, arrayReadOnly, nDefaultType, info ) == FALSE )
	{
		CloseDatabase();
		return FALSE;
	}

	info.m_nNodeType = 0;
	info.m_nErrorType = DATAMANAGER_ERROR_NONE;

	return TRUE;
}

/**********************************************************************/

void CDataManager::CloseDatabase()
{
	CDataManagerInfo info;
	CloseBarcode(info);
	CloseDepartment(info);
	CloseDepartmentSet(info);
	CloseEposGroup(info);
	CloseReportGroup(info);
	CloseConsolGroup(info);
	CloseHistoricalTax(info);
	CloseMixMatch(info);
	CloseModifier(info);

#ifndef POSTRAY_UTILITY

	CloseOfferGroup(info);
	CloseOfferGroupEntry(info);
	CloseShelfLabels(info);
	CloseLoyaltyScheme(info);

#endif

	ClosePaidInText(info);
	ClosePaidOutText(info);
	ClosePayment(info);
	ClosePaymentGroup(info);
	CloseSBPaymentMap(info);
	CloseSEPaymentMap(info);
	CloseAnalysisCategory(info);
	ClosePlu(info);
	CloseDbPluFilterNames(info);
	CloseDbPluFilterItems(info);
	ClosePriceText(info);
	ClosePromotion(info);
	CloseSBPriceText(info);
	CloseTaxRates(info);
	CloseTimePlan(info);
	CloseTimePlanEntry(info);
	CloseTimePlanTemplateEntry(info);
	CloseWalletStampRule(info);

	CloseSpecial();

	m_nActiveDbIdx = -1;
}

/**********************************************************************/

bool CDataManager::OpenBarcode( int nType, CDataManagerInfo& info )
{
	ResetInfo( info, NODE_BARCODE );
	CString strFilename = GetFilePathBarcode( m_nActiveDbIdx );
	Barcode.InvalidatePluBarcodeList();
	return OpenCSVArray( Barcode, info, strFilename, m_nStatusBarcode, nType, FALSE );
}

/**********************************************************************/

bool CDataManager::ReopenBarcode( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_BARCODE );
	
	bool bResult = FALSE;
	switch( m_nStatusBarcode )
	{
	case DATAMANAGER_OPEN_READWRITE:
		bResult = TRUE;
		break;

	case DATAMANAGER_OPEN_READONLY:
		CloseBarcode( info );
		bResult = OpenBarcode( DB_READONLY, info );
		break;

	case DATAMANAGER_CLOSED:
		bResult = OpenBarcode( DB_READONLY, info );
		break;

	default:
		bResult = FALSE;
		break;
	}

	return bResult;
}

/**********************************************************************/

bool CDataManager::CloseBarcode( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_BARCODE );
	Barcode.InvalidatePluBarcodeList();
	return CloseCSVArray( Barcode, info, m_nStatusBarcode );
}

bool CDataManager::WriteBarcode( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_BARCODE );
	return WriteCSVArray( Barcode, info, m_nStatusBarcode );
}

/**********************************************************************/

bool CDataManager::OpenDepartment( int nType, CDataManagerInfo& info )
{
	ResetInfo( info, NODE_DEPARTMENT );
	CString strFilename = GetFilePathDepartment( m_nActiveDbIdx );
	return OpenCSVArray( Department, info, strFilename, m_nStatusDepartment, nType, FALSE );
}

bool CDataManager::ReopenDepartment( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_DEPARTMENT );
	
	bool bResult = FALSE;
	switch( m_nStatusDepartment )
	{
	case DATAMANAGER_OPEN_READWRITE:
		bResult = TRUE;
		break;

	case DATAMANAGER_OPEN_READONLY:
		CloseDepartment( info );
		bResult = OpenDepartment( DB_READONLY, info );
		break;

	case DATAMANAGER_CLOSED:
		bResult = OpenDepartment( DB_READONLY, info );
		break;

	default:
		bResult = FALSE;
		break;
	}

	return bResult;
}

bool CDataManager::CloseDepartment( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_DEPARTMENT );
	return CloseCSVArray( Department, info, m_nStatusDepartment );
}

bool CDataManager::WriteDepartment( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_DEPARTMENT );
	Department.FixSportsBookerDeptNo();
	return WriteCSVArray( Department, info, m_nStatusDepartment );
}

/**********************************************************************/

bool CDataManager::OpenDepartmentSet( int nType, CDataManagerInfo& info )
{
	if ( SysInfo.IsEcrLiteSystem() == FALSE )
	{
		ResetInfo( info, NODE_DEPARTMENT_SET );
		CString strFilename = GetFilePathDepartmentSet( m_nActiveDbIdx );
		return OpenCSVArray( DepartmentSet, info, strFilename, m_nStatusDepartmentSet, nType, TRUE );
	}
	return TRUE;
}

bool CDataManager::CloseDepartmentSet( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_DEPARTMENT_SET );
	return CloseCSVArray( DepartmentSet, info, m_nStatusDepartmentSet );
}

bool CDataManager::WriteDepartmentSet( CDataManagerInfo& info )
{
	if ( SysInfo.IsEcrLiteSystem() == FALSE )
	{
		ResetInfo( info, NODE_DEPARTMENT_SET );
		return WriteCSVArray( DepartmentSet, info, m_nStatusDepartmentSet );
	}
	return TRUE;
}

/**********************************************************************/

bool CDataManager::OpenEposGroup( int nType, CDataManagerInfo& info )
{
	ResetInfo( info, NODE_DEPT_GROUP_EPOS );
	CString strFilename = GetFilePathEposGroup( m_nActiveDbIdx );
	return OpenCSVArray( EposGroup, info, strFilename, m_nStatusEposGroup, nType, FALSE );
}

bool CDataManager::ReopenEposGroup( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_DEPT_GROUP_EPOS );
	
	bool bResult = FALSE;
	switch( m_nStatusEposGroup )
	{
	case DATAMANAGER_OPEN_READWRITE:
		bResult = TRUE;
		break;

	case DATAMANAGER_OPEN_READONLY:
		CloseEposGroup( info );
		bResult = OpenEposGroup( DB_READONLY, info );
		break;

	case DATAMANAGER_CLOSED:
		bResult = OpenEposGroup( DB_READONLY, info );
		break;

	default:
		bResult = FALSE;
		break;
	}

	return bResult;
}

bool CDataManager::CloseEposGroup( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_DEPT_GROUP_EPOS );
	return CloseCSVArray( EposGroup, info, m_nStatusEposGroup );
}

bool CDataManager::WriteEposGroup( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_DEPT_GROUP_EPOS );
	EposGroup.FixSportsBookerGroupNo();
	return WriteCSVArray( EposGroup, info, m_nStatusEposGroup );
}

/**********************************************************************/

bool CDataManager::OpenReportGroup( int nType, CDataManagerInfo& info )
{
	ResetInfo( info, NODE_DEPT_GROUP_REPORT );
	CString strFilename = GetFilePathReportGroup( m_nActiveDbIdx );
	return OpenCSVArray( ReportGroup, info, strFilename, m_nStatusReportGroup, nType, FALSE );
}

bool CDataManager::ReopenReportGroup( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_DEPT_GROUP_REPORT );
	
	bool bResult = FALSE;
	switch( m_nStatusReportGroup )
	{
	case DATAMANAGER_OPEN_READWRITE:
		bResult = TRUE;
		break;

	case DATAMANAGER_OPEN_READONLY:
		CloseReportGroup( info );
		bResult = OpenReportGroup( DB_READONLY, info );
		break;

	case DATAMANAGER_CLOSED:
		bResult = OpenReportGroup( DB_READONLY, info );
		break;

	default:
		bResult = FALSE;
		break;
	}

	return bResult;
}

bool CDataManager::CloseReportGroup( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_DEPT_GROUP_REPORT );
	return CloseCSVArray( ReportGroup, info, m_nStatusReportGroup );
}

bool CDataManager::WriteReportGroup( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_DEPT_GROUP_REPORT );
	return WriteCSVArray( ReportGroup, info, m_nStatusReportGroup );
}

/**********************************************************************/

bool CDataManager::OpenConsolGroup( int nType, CDataManagerInfo& info )
{
	ResetInfo( info, NODE_DEPT_GROUP_CONSOL );
	CString strFilename = GetFilePathConsolGroup( m_nActiveDbIdx );
	return OpenCSVArray( ConsolGroup, info, strFilename, m_nStatusConsolGroup, nType, FALSE );
}

bool CDataManager::ReopenConsolGroup( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_DEPT_GROUP_CONSOL );
	
	bool bResult = FALSE;
	switch( m_nStatusConsolGroup )
	{
	case DATAMANAGER_OPEN_READWRITE:
		bResult = TRUE;
		break;

	case DATAMANAGER_OPEN_READONLY:
		CloseConsolGroup( info );
		bResult = OpenConsolGroup( DB_READONLY, info );
		break;

	case DATAMANAGER_CLOSED:
		bResult = OpenConsolGroup( DB_READONLY, info );
		break;

	default:
		bResult = FALSE;
		break;
	}

	return bResult;
}

bool CDataManager::CloseConsolGroup( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_DEPT_GROUP_CONSOL );
	return CloseCSVArray( ConsolGroup, info, m_nStatusConsolGroup );
}

bool CDataManager::WriteConsolGroup( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_DEPT_GROUP_CONSOL );
	return WriteCSVArray( ConsolGroup, info, m_nStatusConsolGroup );
}

/**********************************************************************/

bool CDataManager::OpenHistoricalTax( int nType, CDataManagerInfo& info )
{
	ResetInfo( info, NODE_TAX_DATE );
	CString strFilename = GetFilePathHistoricalTax( m_nActiveDbIdx );
	return OpenStringArray( HistoricalTaxRates, info, strFilename, m_nStatusHistoricalTax, nType, FALSE );
}

bool CDataManager::CloseHistoricalTax( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_TAX_DATE );
	return CloseStringArray( HistoricalTaxRates, info, m_nStatusHistoricalTax );
}

bool CDataManager::WriteHistoricalTax( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_TAX_DATE );
	HistoricalTaxRates.PrepareForWrite();
	return WriteStringArray( HistoricalTaxRates, info, m_nStatusHistoricalTax );
}

/**********************************************************************/

bool CDataManager::OpenMixMatch( int nType, CDataManagerInfo& info )
{
	if ( SysInfo.IsEcrLiteSystem() == FALSE )
	{
		ResetInfo( info, NODE_MIXMATCH );
		CString strFilename = GetFilePathMixMatch( m_nActiveDbIdx );
		return OpenCSVArray( MixMatch, info, strFilename, m_nStatusMixMatch, nType, FALSE );
	}
	return TRUE;
}

bool CDataManager::ReopenMixMatch( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_MIXMATCH );
	
	bool bResult = FALSE;
	switch( m_nStatusMixMatch )
	{
	case DATAMANAGER_OPEN_READWRITE:
		bResult = TRUE;
		break;

	case DATAMANAGER_OPEN_READONLY:
		CloseMixMatch( info );
		bResult = OpenMixMatch( DB_READONLY, info );
		break;

	case DATAMANAGER_CLOSED:
		bResult = OpenMixMatch( DB_READONLY, info );
		break;

	default:
		bResult = FALSE;
		break;
	}

	return bResult;
}

bool CDataManager::CloseMixMatch( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_MIXMATCH );
	return CloseCSVArray( MixMatch, info, m_nStatusMixMatch );
}

bool CDataManager::WriteMixMatch( CDataManagerInfo& info )
{
	if ( SysInfo.IsEcrLiteSystem() == FALSE )
	{
		ResetInfo( info, NODE_MIXMATCH );
		return WriteCSVArray( MixMatch, info, m_nStatusMixMatch );
	}
	return TRUE;
}

/**********************************************************************/

bool CDataManager::OpenModifier( int nType, CDataManagerInfo& info )
{
	ResetInfo( info, NODE_MODIFIER );
	CString strFilename = GetFilePathModifier( m_nActiveDbIdx );
	return OpenCSVArray( Modifier, info, strFilename, m_nStatusModifier, nType, FALSE );
}

bool CDataManager::ReopenModifier( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_MODIFIER );
	
	bool bResult = FALSE;
	switch( m_nStatusModifier )
	{
	case DATAMANAGER_OPEN_READWRITE:
		bResult = TRUE;
		break;

	case DATAMANAGER_OPEN_READONLY:
		CloseModifier( info );
		bResult = OpenModifier( DB_READONLY, info );
		break;

	case DATAMANAGER_CLOSED:
		bResult = OpenModifier( DB_READONLY, info );
		break;

	default:
		bResult = FALSE;
		break;
	}

	return bResult;
}

bool CDataManager::CloseModifier( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_MODIFIER );
	return CloseCSVArray( Modifier, info, m_nStatusModifier );
}

bool CDataManager::WriteModifier( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_MODIFIER );
	return WriteCSVArray( Modifier, info, m_nStatusModifier );
}

/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/

bool CDataManager::OpenLoyaltyScheme(int nType, CDataManagerInfo& info)
{
	ResetInfo(info, NODE_LOYALTY_SCHEME);
	CString strFilename = GetFilePathLoyaltyScheme(m_nActiveDbIdx);
	bool bResult = OpenCSVArray(LoyaltyScheme, info, strFilename, m_nStatusLoyaltyScheme, nType, FALSE);

	if ((LoyaltyScheme.GetSize() == 0) && (0 == m_nActiveDbIdx))
	{
		CLoyaltySchemeCSVArray arrayLegacy;

		CFilenameUpdater FnUp(SysFiles::SysLoyaltyScheme);
		CString strFilename = FnUp.GetFilenameToUse();

		if (arrayLegacy.Open(strFilename, DB_READONLY) == DB_ERR_NONE)
		{
			for (int n = 0; n < arrayLegacy.GetSize(); n++)
			{
				CLoyaltySchemeCSVRecord recordLegacy;
				arrayLegacy.GetAt(n, recordLegacy);
				LoyaltyScheme.Add(recordLegacy);
			}

			arrayLegacy.Close();
		}
	}

	return bResult;
}

/**********************************************************************/

bool CDataManager::ReopenLoyaltyScheme(CDataManagerInfo& info)
{
	ResetInfo(info, NODE_LOYALTY_SCHEME);

	bool bResult = FALSE;
	switch (m_nStatusLoyaltyScheme)
	{
	case DATAMANAGER_OPEN_READWRITE:
		bResult = TRUE;
		break;

	case DATAMANAGER_OPEN_READONLY:
		CloseLoyaltyScheme(info);
		bResult = OpenLoyaltyScheme(DB_READONLY, info);
		break;

	case DATAMANAGER_CLOSED:
		bResult = OpenLoyaltyScheme(DB_READONLY, info);
		break;

	default:
		bResult = FALSE;
		break;
	}

	return bResult;
}

bool CDataManager::CloseLoyaltyScheme(CDataManagerInfo& info)
{
	ResetInfo(info, NODE_LOYALTY_SCHEME);
	return CloseCSVArray(LoyaltyScheme, info, m_nStatusLoyaltyScheme);
}

bool CDataManager::WriteLoyaltyScheme(CDataManagerInfo& info)
{
	ResetInfo(info, NODE_LOYALTY_SCHEME);
	bool b = WriteCSVArray(LoyaltyScheme, info, m_nStatusLoyaltyScheme);

	if ((TRUE == b) && (DataManager.GetActiveDbIdx() == 0))
	{
		CFilenameUpdater FnUp(SysFiles::SysLoyaltyScheme);
		CString strFilename = FnUp.GetFilenameToUse();
		CFileRemover FileRemover(strFilename);
	}

	return b;
}

/**********************************************************************/

bool CDataManager::OpenOfferGroup( int nType, CDataManagerInfo& info )
{
	if ( SysInfo.IsEcrLiteSystem() == FALSE )
	{
		ResetInfo( info, NODE_OFFER_GROUP );
		CString strFilename = GetFilePathOfferGroup( m_nActiveDbIdx );
		return OpenCSVArray( OfferGroup, info, strFilename, m_nStatusOfferGroup, nType, FALSE );
	}
	return TRUE;
}

bool CDataManager::ReopenOfferGroup( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_OFFER_GROUP );
	
	bool bResult = FALSE;
	switch( m_nStatusOfferGroup )
	{
	case DATAMANAGER_OPEN_READWRITE:
		bResult = TRUE;
		break;

	case DATAMANAGER_OPEN_READONLY:
		CloseOfferGroup( info );
		bResult = OpenOfferGroup( DB_READONLY, info );
		break;

	case DATAMANAGER_CLOSED:
		bResult = OpenOfferGroup( DB_READONLY, info );
		break;

	default:
		bResult = FALSE;
		break;
	}

	return bResult;
}

bool CDataManager::CloseOfferGroup( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_OFFER_GROUP );
	return CloseCSVArray( OfferGroup, info, m_nStatusOfferGroup );
}

bool CDataManager::WriteOfferGroup( CDataManagerInfo& info )
{
	if ( SysInfo.IsEcrLiteSystem() == FALSE )
	{
		ResetInfo( info, NODE_OFFER_GROUP );
		return WriteCSVArray( OfferGroup, info, m_nStatusOfferGroup );
	}
	return TRUE;
}

/**********************************************************************/

bool CDataManager::OpenOfferGroupEntry( int nType, CDataManagerInfo& info )
{
	if ( SysInfo.IsEcrLiteSystem() == FALSE )
	{
		ResetInfo( info, NODE_OFFER_ENTRY );
		CString strFilename = GetFilePathOfferGroupEntry( m_nActiveDbIdx );
		return OpenCSVArray( OfferGroupEntry, info, strFilename, m_nStatusOfferGroupEntry, nType, FALSE );
	}
	return TRUE;
}

bool CDataManager::ReopenOfferGroupEntry( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_OFFER_ENTRY );
	
	bool bResult = FALSE;
	switch( m_nStatusOfferGroupEntry )
	{
	case DATAMANAGER_OPEN_READWRITE:
		bResult = TRUE;
		break;

	case DATAMANAGER_OPEN_READONLY:
		CloseOfferGroupEntry( info );
		bResult = OpenOfferGroupEntry( DB_READONLY, info );
		break;

	case DATAMANAGER_CLOSED:
		bResult = OpenOfferGroupEntry( DB_READONLY, info );
		break;

	default:
		bResult = FALSE;
		break;
	}

	return bResult;
}

bool CDataManager::CloseOfferGroupEntry( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_OFFER_ENTRY );
	return CloseCSVArray( OfferGroupEntry, info, m_nStatusOfferGroupEntry );
}

bool CDataManager::WriteOfferGroupEntry( CDataManagerInfo& info )
{
	if ( SysInfo.IsEcrLiteSystem() == FALSE )
	{
		ResetInfo( info, NODE_OFFER_ENTRY );
		return WriteCSVArray( OfferGroupEntry, info, m_nStatusOfferGroupEntry );
	}
	return TRUE;
}

/**********************************************************************/

bool CDataManager::OpenShelfLabels( int nType, CDataManagerInfo& info )
{
	ResetInfo( info, NODE_LABELS_SHELF );
	CString strFilename = GetFilePathShelfLabels( m_nActiveDbIdx );
	OpenCSVArray( ShelfLabels, info, strFilename, m_nStatusShelfLabels, nType, TRUE );
	return ( info.m_nErrorType == DATAMANAGER_ERROR_NONE ); 
}

bool CDataManager::OpenShelfLabels( int nType, int nLocIdx, int nPriceSet, CDataManagerInfo& info )
{
	ResetInfo( info, NODE_LABELS_SHELF );
	CString strFilename = GetFilePathShelfLabels( nLocIdx, nPriceSet );
	OpenCSVArray( ShelfLabels, info, strFilename, m_nStatusShelfLabels, nType, TRUE );
	return ( info.m_nErrorType == DATAMANAGER_ERROR_NONE ); 
}

bool CDataManager::CloseShelfLabels( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_LABELS_SHELF );
	return CloseCSVArray( ShelfLabels, info, m_nStatusShelfLabels );
}

bool CDataManager::WriteShelfLabels( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_LABELS_SHELF );
	return WriteCSVArray( ShelfLabels, info, m_nStatusShelfLabels );
}

/**********************************************************************/
#endif
/**********************************************************************/

bool CDataManager::OpenPaidInText( int nType, CDataManagerInfo& info )
{
	ResetInfo( info, NODE_PAIDIN_TEXT );
	CString strFilename = GetFilePathPaidInText( m_nActiveDbIdx );
	return OpenCSVArray( PaidInText, info, strFilename, m_nStatusPaidInText, nType, FALSE );
}

bool CDataManager::ReopenPaidInText( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_PAIDIN_TEXT );
	
	bool bResult = FALSE;
	switch( m_nStatusPaidInText )
	{
	case DATAMANAGER_OPEN_READWRITE:
		bResult = TRUE;
		break;

	case DATAMANAGER_OPEN_READONLY:
		ClosePaidInText( info );
		bResult = OpenPaidInText( DB_READONLY, info );
		break;

	case DATAMANAGER_CLOSED:
		bResult = OpenPaidInText( DB_READONLY, info );
		break;

	default:
		bResult = FALSE;
		break;
	}

	return bResult;
}

bool CDataManager::ClosePaidInText( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_PAIDIN_TEXT );
	return CloseCSVArray( PaidInText, info, m_nStatusPaidInText );
}

bool CDataManager::WritePaidInText( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_PAIDIN_TEXT );
	return WriteCSVArray( PaidInText, info, m_nStatusPaidInText );
}

/**********************************************************************/

bool CDataManager::OpenPaidOutText( int nType, CDataManagerInfo& info )
{
	ResetInfo( info, NODE_PAIDOUT_TEXT );
	CString strFilename = GetFilePathPaidOutText( m_nActiveDbIdx );
	return OpenCSVArray( PaidOutText, info, strFilename, m_nStatusPaidOutText, nType, FALSE );
}

bool CDataManager::ReopenPaidOutText( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_PAIDOUT_TEXT );
	
	bool bResult = FALSE;
	switch( m_nStatusPaidOutText )
	{
	case DATAMANAGER_OPEN_READWRITE:
		bResult = TRUE;
		break;

	case DATAMANAGER_OPEN_READONLY:
		ClosePaidOutText( info );
		bResult = OpenPaidOutText( DB_READONLY, info );
		break;

	case DATAMANAGER_CLOSED:
		bResult = OpenPaidOutText( DB_READONLY, info );
		break;

	default:
		bResult = FALSE;
		break;
	}

	return bResult;
}

bool CDataManager::ClosePaidOutText( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_PAIDOUT_TEXT );
	return CloseCSVArray( PaidOutText, info, m_nStatusPaidOutText );
}

bool CDataManager::WritePaidOutText( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_PAIDOUT_TEXT );
	return WriteCSVArray( PaidOutText, info, m_nStatusPaidOutText );
}

/**********************************************************************/

bool CDataManager::OpenPayment( int nType, CDataManagerInfo& info )
{
	ResetInfo( info, NODE_PAYMENT );
	CString strFilename = GetFilePathPayment( m_nActiveDbIdx );
	return OpenCSVArray( Payment, info, strFilename, m_nStatusPayment, nType, FALSE );
}

bool CDataManager::ReopenPayment( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_PAYMENT );
	
	bool bResult = FALSE;
	switch( m_nStatusPayment )
	{
	case DATAMANAGER_OPEN_READWRITE:
		bResult = TRUE;
		break;

	case DATAMANAGER_OPEN_READONLY:
		ClosePayment( info );
		bResult = OpenPayment( DB_READONLY, info );
		break;

	case DATAMANAGER_CLOSED:
		bResult = OpenPayment( DB_READONLY, info );
		break;

	default:
		bResult = FALSE;
		break;
	}

	return bResult;
}

bool CDataManager::ClosePayment( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_PAYMENT );
	return CloseCSVArray( Payment, info, m_nStatusPayment );
}

bool CDataManager::WritePayment( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_PAYMENT );
	return WriteCSVArray( Payment, info, m_nStatusPayment );
}

/**********************************************************************/

bool CDataManager::OpenPaymentGroup( int nType, CDataManagerInfo& info )
{
	ResetInfo( info, NODE_PAYMENT_GROUP );
	CString strFilename = GetFilePathPaymentGroup( m_nActiveDbIdx );
	return OpenCSVArray( PaymentGroup, info, strFilename, m_nStatusPaymentGroup, nType, FALSE );
}

bool CDataManager::ReopenPaymentGroup( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_PAYMENT_GROUP );
	
	bool bResult = FALSE;
	switch( m_nStatusPaymentGroup )
	{
	case DATAMANAGER_OPEN_READWRITE:
		bResult = TRUE;
		break;

	case DATAMANAGER_OPEN_READONLY:
		ClosePayment( info );
		bResult = OpenPaymentGroup( DB_READONLY, info );
		break;

	case DATAMANAGER_CLOSED:
		bResult = OpenPaymentGroup( DB_READONLY, info );
		break;

	default:
		bResult = FALSE;
		break;
	}

	return bResult;
}

bool CDataManager::ClosePaymentGroup( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_PAYMENT_GROUP );
	return CloseCSVArray( PaymentGroup, info, m_nStatusPaymentGroup );
}

bool CDataManager::WritePaymentGroup( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_PAYMENT_GROUP );
	return WriteCSVArray( PaymentGroup, info, m_nStatusPaymentGroup );
}

/**********************************************************************/

bool CDataManager::OpenSBPaymentMap( int nType, CDataManagerInfo& info )
{
	ResetInfo( info, NODE_SBPAYMENTMAP );
	CString strFilename = GetFilePathSptBookPaymentMap( m_nActiveDbIdx );
	return OpenCSVArray( SBPaymentMap, info, strFilename, m_nStatusSBPaymentMap, nType, FALSE );
}

bool CDataManager::CloseSBPaymentMap( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_SBPAYMENTMAP );
	return CloseCSVArray( SBPaymentMap, info, m_nStatusSBPaymentMap );
}

bool CDataManager::WriteSBPaymentMap( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_SBPAYMENTMAP );
	return WriteCSVArray( SBPaymentMap, info, m_nStatusSBPaymentMap );
}

/**********************************************************************/

bool CDataManager::OpenSEPaymentMap( int nType, CDataManagerInfo& info )
{
	ResetInfo( info, NODE_SEPAYMENTMAP );
	CString strFilename = GetFilePathSmartEntPaymentMap( m_nActiveDbIdx );
	return OpenCSVArray( SEPaymentMap, info, strFilename, m_nStatusSEPaymentMap, nType, FALSE );
}

bool CDataManager::CloseSEPaymentMap( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_SEPAYMENTMAP );
	return CloseCSVArray( SEPaymentMap, info, m_nStatusSEPaymentMap );
}

bool CDataManager::WriteSEPaymentMap( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_SEPAYMENTMAP );
	return WriteCSVArray( SEPaymentMap, info, m_nStatusSEPaymentMap );
}

/**********************************************************************/

bool CDataManager::OpenAnalysisCategory( int nType, CDataManagerInfo& info )
{
	ResetInfo( info, NODE_ACAT );
	CString strFilename = GetFilePathAnalysisCategory( m_nActiveDbIdx );
	return OpenCSVArray( AnalysisCategory, info, strFilename, m_nStatusAnalysisCategory, nType, FALSE );
}

bool CDataManager::ReopenAnalysisCategory( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_ACAT );
	
	bool bResult = FALSE;
	switch( m_nStatusAnalysisCategory )
	{
	case DATAMANAGER_OPEN_READWRITE:
		bResult = TRUE;
		break;

	case DATAMANAGER_OPEN_READONLY:
		CloseAnalysisCategory( info );
		bResult = OpenAnalysisCategory( DB_READONLY, info );
		break;

	case DATAMANAGER_CLOSED:
		bResult = OpenAnalysisCategory( DB_READONLY, info );
		break;

	default:
		bResult = FALSE;
		break;
	}

	return bResult;
}

bool CDataManager::CloseAnalysisCategory( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_ACAT );
	return CloseCSVArray( AnalysisCategory, info, m_nStatusAnalysisCategory );
}

bool CDataManager::WriteAnalysisCategory( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_ACAT );
	return WriteCSVArray( AnalysisCategory, info, m_nStatusAnalysisCategory );
}

/**********************************************************************/

bool CDataManager::OpenDbPluFilterNames( int nType, CDataManagerInfo& info )
{
	if ( SysInfo.IsEcrLiteSystem() == FALSE )
	{
		ResetInfo( info, NODE_PLUFILTER_NAME );
		CString strFilename = GetFilePathDbPluFilterNames( m_nActiveDbIdx );
		return OpenCSVArray( DbPluFilterNames, info, strFilename, m_nStatusDbPluFilterNames, nType, FALSE );
	}
	return TRUE;
}

bool CDataManager::ReopenDbPluFilterNames( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_PLUFILTER_NAME );
	
	bool bResult = FALSE;
	switch( m_nStatusDbPluFilterNames )
	{
	case DATAMANAGER_OPEN_READWRITE:
		bResult = TRUE;
		break;

	case DATAMANAGER_OPEN_READONLY:
		CloseDbPluFilterNames( info );
		bResult = OpenDbPluFilterNames( DB_READONLY, info );
		break;

	case DATAMANAGER_CLOSED:
		bResult = OpenDbPluFilterNames( DB_READONLY, info );
		break;

	default:
		bResult = FALSE;
		break;
	}

	return bResult;
}

bool CDataManager::CloseDbPluFilterNames( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_PLUFILTER_NAME );
	return CloseCSVArray( DbPluFilterNames, info, m_nStatusDbPluFilterNames );
}

bool CDataManager::WriteDbPluFilterNames( CDataManagerInfo& info )
{
	if ( SysInfo.IsEcrLiteSystem() == FALSE )
	{
		ResetInfo( info, NODE_PLUFILTER_NAME );
		return WriteCSVArray( DbPluFilterNames, info, m_nStatusDbPluFilterNames );
	}
	return TRUE;
}

/**********************************************************************/

bool CDataManager::OpenDbPluFilterItems( int nType, CDataManagerInfo& info )
{
	if ( SysInfo.IsEcrLiteSystem() == FALSE )
	{
		ResetInfo( info, NODE_PLUFILTER_ITEM );
		CString strFilename = GetFilePathDbPluFilterItems( m_nActiveDbIdx );
		return OpenCSVArray( DbPluFilterItems, info, strFilename, m_nStatusDbPluFilterItems, nType, FALSE );
	}
	return TRUE;
}

bool CDataManager::ReopenDbPluFilterItems( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_PLUFILTER_ITEM );
	
	bool bResult = FALSE;
	switch( m_nStatusDbPluFilterItems )
	{
	case DATAMANAGER_OPEN_READWRITE:
		bResult = TRUE;
		break;

	case DATAMANAGER_OPEN_READONLY:
		CloseDbPluFilterItems( info );
		bResult = OpenDbPluFilterItems( DB_READONLY, info );
		break;

	case DATAMANAGER_CLOSED:
		bResult = OpenDbPluFilterItems( DB_READONLY, info );
		break;

	default:
		bResult = FALSE;
		break;
	}

	return bResult;
}

bool CDataManager::CloseDbPluFilterItems( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_PLUFILTER_ITEM );
	return CloseCSVArray( DbPluFilterItems, info, m_nStatusDbPluFilterItems );
}

bool CDataManager::WriteDbPluFilterItems( CDataManagerInfo& info )
{
	if ( SysInfo.IsEcrLiteSystem() == FALSE )
	{
		ResetInfo( info, NODE_PLUFILTER_ITEM );
		return WriteCSVArray( DbPluFilterItems, info, m_nStatusDbPluFilterItems );
	}
	return TRUE;
}

/**********************************************************************/

bool CDataManager::OpenPriceText( int nType, CDataManagerInfo& info )
{
	ResetInfo( info, NODE_PRICETEXT );
	CString strFilename = GetFilePathPriceText( m_nActiveDbIdx );
	return OpenCSVArray( PriceText, info, strFilename, m_nStatusPriceText, nType, FALSE );
}

bool CDataManager::ClosePriceText( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_PRICETEXT );
	return CloseCSVArray( PriceText, info, m_nStatusPriceText );
}

bool CDataManager::WritePriceText( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_PRICETEXT );
	return WriteCSVArray( PriceText, info, m_nStatusPriceText );
}

/**********************************************************************/

bool CDataManager::OpenPromotion( int nType, CDataManagerInfo& info )
{
	if ( SysInfo.IsEcrLiteSystem() == FALSE )
	{
		ResetInfo( info, NODE_PROMOTION );
		CString strFilename = GetFilePathPromotion( m_nActiveDbIdx );
		return OpenCSVArray( Promotion, info, strFilename, m_nStatusPromotion, nType, FALSE );
	}
	return TRUE;
}

bool CDataManager::ReopenPromotion( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_PROMOTION );
	
	bool bResult = FALSE;
	switch( m_nStatusPromotion )
	{
	case DATAMANAGER_OPEN_READWRITE:
		bResult = TRUE;
		break;

	case DATAMANAGER_OPEN_READONLY:
		ClosePromotion( info );
		bResult = OpenPromotion( DB_READONLY, info );
		break;

	case DATAMANAGER_CLOSED:
		bResult = OpenPromotion( DB_READONLY, info );
		break;

	default:
		bResult = FALSE;
		break;
	}

	return bResult;
}

bool CDataManager::ClosePromotion( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_PROMOTION );
	return CloseCSVArray( Promotion, info, m_nStatusPromotion );
}

bool CDataManager::WritePromotion( CDataManagerInfo& info )
{
	if ( SysInfo.IsEcrLiteSystem() == FALSE )
	{
		ResetInfo( info, NODE_PROMOTION );
		return WriteCSVArray( Promotion, info, m_nStatusPromotion );
	}
	return TRUE;
}

/**********************************************************************/

bool CDataManager::OpenSBPriceText( int nType, CDataManagerInfo& info )
{
	ResetInfo( info, NODE_SBPRICETEXT );
	CString strFilename = GetFilePathSBPriceText( m_nActiveDbIdx );
	return OpenCSVArray( SBPriceText, info, strFilename, m_nStatusSBPriceText, nType, FALSE );
}

bool CDataManager::CloseSBPriceText( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_SBPRICETEXT );
	return CloseCSVArray( SBPriceText, info, m_nStatusSBPriceText );
}

bool CDataManager::WriteSBPriceText( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_SBPRICETEXT );
	return WriteCSVArray( SBPriceText, info, m_nStatusSBPriceText );
}

/**********************************************************************/

bool CDataManager::OpenTaxRates( int nType, CDataManagerInfo& info )
{
	ResetInfo( info, NODE_TAX );	
	CString strFilename = GetFilePathTaxRates( m_nActiveDbIdx );
	return OpenStringArray( TaxRates, info, strFilename, m_nStatusTaxRates, nType, FALSE );
}

bool CDataManager::ReopenTaxRates( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_TAX );
	
	bool bResult = FALSE;
	switch( m_nStatusTaxRates )
	{
	case DATAMANAGER_OPEN_READWRITE:
		bResult = TRUE;
		break;

	case DATAMANAGER_OPEN_READONLY:
		CloseTaxRates( info );
		bResult = OpenTaxRates( DB_READONLY, info );
		break;

	case DATAMANAGER_CLOSED:
		bResult = OpenTaxRates( DB_READONLY, info );
		break;

	default:
		bResult = FALSE;
		break;
	}

	return bResult;
}

bool CDataManager::CloseTaxRates( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_TAX );
	return CloseStringArray( TaxRates, info, m_nStatusTaxRates );
}

bool CDataManager::WriteTaxRates( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_TAX );
	TaxRates.PrepareForWrite();
	return WriteStringArray( TaxRates, info, m_nStatusTaxRates );
}

/**********************************************************************/

bool CDataManager::OpenTimePlan( int nType, CDataManagerInfo& info )
{
	if ( SysInfo.IsEcrLiteSystem() == FALSE )
	{
		ResetInfo( info, NODE_TIMEPLAN );
		CString strFilename = GetFilePathTimePlan( m_nActiveDbIdx );
		return OpenCSVArray( TimePlan, info, strFilename, m_nStatusTimePlan, nType, FALSE );
	}
	return TRUE;
}

bool CDataManager::ReopenTimePlan( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_TIMEPLAN );
	
	bool bResult = FALSE;
	switch( m_nStatusTimePlan )
	{
	case DATAMANAGER_OPEN_READWRITE:
		bResult = TRUE;
		break;

	case DATAMANAGER_OPEN_READONLY:
		CloseTimePlan( info );
		bResult = OpenTimePlan( DB_READONLY, info );
		break;

	case DATAMANAGER_CLOSED:
		bResult = OpenTimePlan( DB_READONLY, info );
		break;

	default:
		bResult = FALSE;
		break;
	}

	return bResult;
}

bool CDataManager::CloseTimePlan( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_TIMEPLAN );
	return CloseCSVArray( TimePlan, info, m_nStatusTimePlan );
}

bool CDataManager::WriteTimePlan( CDataManagerInfo& info )
{
	if ( SysInfo.IsEcrLiteSystem() == FALSE )
	{
		ResetInfo( info, NODE_TIMEPLAN );
		return WriteCSVArray( TimePlan, info, m_nStatusTimePlan );
	}
	return TRUE;
}

/**********************************************************************/

bool CDataManager::OpenTimePlanEntry( int nType, CDataManagerInfo& info )
{
	if ( SysInfo.IsEcrLiteSystem() == FALSE )
	{
		ResetInfo( info, NODE_TIMEPLAN_ENTRY );
		CString strFilename = GetFilePathTimePlanEntry( m_nActiveDbIdx );
		return OpenCSVArray( TimePlanEntry, info, strFilename, m_nStatusTimePlanEntry, nType, FALSE );
	}
	return TRUE;
}

bool CDataManager::ReopenTimePlanEntry( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_TIMEPLAN_ENTRY );
	
	bool bResult = FALSE;
	switch( m_nStatusTimePlanEntry )
	{
	case DATAMANAGER_OPEN_READWRITE:
		bResult = TRUE;
		break;

	case DATAMANAGER_OPEN_READONLY:
		CloseTimePlanEntry( info );
		bResult = OpenTimePlanEntry( DB_READONLY, info );
		break;

	case DATAMANAGER_CLOSED:
		bResult = OpenTimePlanEntry( DB_READONLY, info );
		break;

	default:
		bResult = FALSE;
		break;
	}

	return bResult;
}

bool CDataManager::CloseTimePlanEntry( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_TIMEPLAN_ENTRY );
	return CloseCSVArray( TimePlanEntry, info, m_nStatusTimePlanEntry );
}

bool CDataManager::WriteTimePlanEntry( CDataManagerInfo& info )
{
	if ( SysInfo.IsEcrLiteSystem() == FALSE )
	{
		ResetInfo( info, NODE_TIMEPLAN_ENTRY );
		return WriteCSVArray( TimePlanEntry, info, m_nStatusTimePlanEntry );
	}
	return TRUE;
}

/**********************************************************************/

bool CDataManager::OpenTimePlanTemplateEntry(int nType, CDataManagerInfo& info)
{
	if (SysInfo.IsEcrLiteSystem() == FALSE)
	{
		ResetInfo(info, NODE_TIMEPLAN_TEMPLATE_ENTRY);
		CString strFilename = GetFilePathTimePlanTemplateEntry(m_nActiveDbIdx);
		return OpenCSVArray(TimePlanTemplateEntry, info, strFilename, m_nStatusTimePlanTemplateEntry, nType, FALSE);
	}
	return TRUE;
}

bool CDataManager::ReopenTimePlanTemplateEntry(CDataManagerInfo& info)
{
	ResetInfo(info, NODE_TIMEPLAN_TEMPLATE_ENTRY);

	bool bResult = FALSE;
	switch (m_nStatusTimePlanTemplateEntry)
	{
	case DATAMANAGER_OPEN_READWRITE:
		bResult = TRUE;
		break;

	case DATAMANAGER_OPEN_READONLY:
		CloseTimePlanTemplateEntry(info);
		bResult = OpenTimePlanTemplateEntry(DB_READONLY, info);
		break;

	case DATAMANAGER_CLOSED:
		bResult = OpenTimePlanTemplateEntry(DB_READONLY, info);
		break;

	default:
		bResult = FALSE;
		break;
	}

	return bResult;
}

bool CDataManager::CloseTimePlanTemplateEntry(CDataManagerInfo& info)
{
	ResetInfo(info, NODE_TIMEPLAN_TEMPLATE_ENTRY);
	return CloseCSVArray(TimePlanTemplateEntry, info, m_nStatusTimePlanTemplateEntry);
}

bool CDataManager::WriteTimePlanTemplateEntry(CDataManagerInfo& info)
{
	if (SysInfo.IsEcrLiteSystem() == FALSE)
	{
		ResetInfo(info, NODE_TIMEPLAN_TEMPLATE_ENTRY);
		return WriteCSVArray(TimePlanTemplateEntry, info, m_nStatusTimePlanTemplateEntry);
	}
	return TRUE;
}

/**********************************************************************/

bool CDataManager::OpenWalletStampRule(int nType, CDataManagerInfo& info)
{
	if (SysInfo.IsEcrLiteSystem() == FALSE)
	{
		ResetInfo(info, NODE_WALLETSTAMPRULE);
		CString strFilename = GetFilePathWalletStampRule(m_nActiveDbIdx);
		return OpenCSVArray(WalletStampRule, info, strFilename, m_nStatusWalletStampRule, nType, FALSE);
	}
	return TRUE;
}

bool CDataManager::ReopenWalletStampRule(CDataManagerInfo& info)
{
	ResetInfo(info, NODE_WALLETSTAMPRULE);

	bool bResult = FALSE;
	switch (m_nStatusWalletStampRule)
	{
	case DATAMANAGER_OPEN_READWRITE:
		bResult = TRUE;
		break;

	case DATAMANAGER_OPEN_READONLY:
		CloseWalletStampRule(info);
		bResult = OpenWalletStampRule(DB_READONLY, info);
		break;

	case DATAMANAGER_CLOSED:
		bResult = OpenWalletStampRule(DB_READONLY, info);
		break;

	default:
		bResult = FALSE;
		break;
	}

	return bResult;
}

bool CDataManager::CloseWalletStampRule(CDataManagerInfo& info)
{
	ResetInfo(info, NODE_WALLETSTAMPRULE);
	return CloseCSVArray(WalletStampRule, info, m_nStatusWalletStampRule);
}

bool CDataManager::WriteWalletStampRule(CDataManagerInfo& info)
{
	if (SysInfo.IsEcrLiteSystem() == FALSE)
	{
		ResetInfo(info, NODE_WALLETSTAMPRULE);
		return WriteCSVArray(WalletStampRule, info, m_nStatusWalletStampRule);
	}
	return TRUE;
}

/**********************************************************************/

bool CDataManager::LockAllUsers()
{
	bool bResult = FALSE;
	
	if ( m_nUserLockCount > 0 )
	{
		m_nUserLockCount++;
		bResult = TRUE;	
	}
	else if ( Sysset.LockAllUsers() == TRUE ) 
	{
		m_nUserLockCount = 1;
		bResult = TRUE;
	}

	return bResult;
}

/**********************************************************************/

void CDataManager::UnlockAllUsers()
{
	if ( m_nUserLockCount > 1 )
		m_nUserLockCount--;
	else
	{
		Sysset.UnlockAllUsers();
		m_nUserLockCount = 0;
	}
}

/**********************************************************************/

const char* CDataManager::GetFilePathPlu ( int nDbIdx )
{
	CFilenameUpdater FnUp( SysFiles::PluDatabase, nDbIdx );
	m_strFilePathData = FnUp.GetFilenameToUse();
	return m_strFilePathData;
}

/**********************************************************************/

const char* CDataManager::GetFilePathDepartment ( int nDbIdx )
{
	if ( EcrmanOptions.GetGlobalDepartmentFlag() == FALSE )
	{
		CFilenameUpdater FnUp( SysFiles::Department, nDbIdx );
		m_strFilePathData = FnUp.GetFilenameToUse();
	}
	else
	{
		CFilenameUpdater FnUp( SysFiles::Department, -1 );
		m_strFilePathData = FnUp.GetFilenameToUse();
	}
		
	return m_strFilePathData;
}

/**********************************************************************/

const char* CDataManager::GetFilePathDepartmentSet ( int nDbIdx )
{
	if ( EcrmanOptions.GetGlobalDepartmentFlag() == FALSE )
	{
		CFilenameUpdater FnUp( SysFiles::DepartmentSet, nDbIdx );
		m_strFilePathData = FnUp.GetFilenameToUse();
	}
	else
	{
		CFilenameUpdater FnUp( SysFiles::DepartmentSet, -1 );
		m_strFilePathData = FnUp.GetFilenameToUse();
	}
		
	return m_strFilePathData;
}

/**********************************************************************/

const char* CDataManager::GetFilePathEposGroup ( int nDbIdx )
{
	if ( EcrmanOptions.GetGlobalDeptGroupEposFlag() == FALSE )
	{
		CFilenameUpdater FnUp( SysFiles::DepartmentEposGroup, nDbIdx );
		m_strFilePathData = FnUp.GetFilenameToUse();
	}
	else
	{
		CFilenameUpdater FnUp( SysFiles::DepartmentEposGroup, -1 );
		m_strFilePathData = FnUp.GetFilenameToUse();
	}
		
	return m_strFilePathData;
}

/**********************************************************************/

const char* CDataManager::GetFilePathReportGroup ( int nDbIdx )
{
	if ( EcrmanOptions.GetGlobalDeptGroupReportFlag() == FALSE )
	{
		CFilenameUpdater FnUp( SysFiles::DeptReportGroup, nDbIdx );
		m_strFilePathData = FnUp.GetFilenameToUse();
	}
	else
	{
		CFilenameUpdater FnUp( SysFiles::DeptReportGroup, -1 );
		m_strFilePathData = FnUp.GetFilenameToUse();
	}
		
	return m_strFilePathData;
}

/**********************************************************************/

const char* CDataManager::GetFilePathConsolGroup ( int nDbIdx )
{
	if ( EcrmanOptions.GetGlobalDeptGroupConsolFlag() == FALSE )
	{
		CFilenameUpdater FnUp( SysFiles::DeptConsolGroup, nDbIdx );
		m_strFilePathData = FnUp.GetFilenameToUse();
	}
	else
	{
		CFilenameUpdater FnUp( SysFiles::DeptConsolGroup, -1 );
		m_strFilePathData = FnUp.GetFilenameToUse();
	}
		
	return m_strFilePathData;
}

/**********************************************************************/

const char* CDataManager::GetFilePathHistoricalTax ( int nDbIdx )
{
	if ( EcrmanOptions.GetGlobalTaxFlag() == FALSE )
	{
		CFilenameUpdater FnUp( SysFiles::HistoricalTax, nDbIdx );
		m_strFilePathData = FnUp.GetFilenameToUse();
	}
	else
	{
		CFilenameUpdater FnUp( SysFiles::HistoricalTax, -1 );
		m_strFilePathData = FnUp.GetFilenameToUse();
	}
		
	return m_strFilePathData;
}

/**********************************************************************/

const char* CDataManager::GetFilePathModifier ( int nDbIdx )
{
	if ( EcrmanOptions.GetGlobalModifierFlag() == FALSE )
	{
		CFilenameUpdater FnUp( SysFiles::Modifier, nDbIdx, -1, FNUP_FAMILY_DATABASE );
		m_strFilePathData = FnUp.GetFilenameToUse();
	}
	else
	{
		CFilenameUpdater FnUp( SysFiles::Modifier );
		m_strFilePathData = FnUp.GetFilenameToUse();
	}
		
	return m_strFilePathData;
}

/**********************************************************************/

const char* CDataManager::GetFilePathPaidInText ( int nDbIdx )
{
	if ( EcrmanOptions.GetGlobalPaidInOutFlag() == FALSE )
	{
		CFilenameUpdater FnUp( SysFiles::PaidInText, nDbIdx, -1, FNUP_FAMILY_DATABASE );
		m_strFilePathData = FnUp.GetFilenameToUse();
	}
	else
	{
		CFilenameUpdater FnUp( SysFiles::PaidInText );
		m_strFilePathData = FnUp.GetFilenameToUse();
	}
		
	return m_strFilePathData;
}

/**********************************************************************/

const char* CDataManager::GetFilePathPaidOutText ( int nDbIdx )
{
	if ( EcrmanOptions.GetGlobalPaidInOutFlag() == FALSE )
	{
		CFilenameUpdater FnUp( SysFiles::PaidOutText, nDbIdx, -1, FNUP_FAMILY_DATABASE );
		m_strFilePathData = FnUp.GetFilenameToUse();
	}
	else
	{
		CFilenameUpdater FnUp( SysFiles::PaidOutText );
		m_strFilePathData = FnUp.GetFilenameToUse();
	}
		
	return m_strFilePathData;
}

/**********************************************************************/

const char* CDataManager::GetFilePathPayment ( int nDbIdx )
{
	if ( EcrmanOptions.GetGlobalPaymentFlag() == FALSE )
	{
		CFilenameUpdater FnUp( SysFiles::Payment, nDbIdx, -1, FNUP_FAMILY_DATABASE );
		m_strFilePathData = FnUp.GetFilenameToUse();
	}
	else
	{
		CFilenameUpdater FnUp( SysFiles::Payment );
		m_strFilePathData = FnUp.GetFilenameToUse();
	}
		
	return m_strFilePathData;
}

/**********************************************************************/

const char* CDataManager::GetFilePathPaymentGroup ( int nDbIdx )
{
	if ( EcrmanOptions.GetGlobalPaymentGroupFlag() == FALSE )
	{
		CFilenameUpdater FnUp( SysFiles::PaymentGroup, nDbIdx, -1, FNUP_FAMILY_DATABASE );
		m_strFilePathData = FnUp.GetFilenameToUse();
	}
	else
	{
		CFilenameUpdater FnUp( SysFiles::PaymentGroup );
		m_strFilePathData = FnUp.GetFilenameToUse();
	}
		
	return m_strFilePathData;
}

/**********************************************************************/

const char* CDataManager::GetFilePathSptBookPaymentMap ( int nDbIdx )
{
	if ( EcrmanOptions.GetGlobalPaymentFlag() == FALSE )
	{
		CFilenameUpdater FnUp( SysFiles::SBPaymentMap, nDbIdx, -1, FNUP_FAMILY_DATABASE );
		m_strFilePathData = FnUp.GetFilenameToUse();
	}
	else
	{
		CFilenameUpdater FnUp( SysFiles::SBPaymentMap );
		m_strFilePathData = FnUp.GetFilenameToUse();
	}
		
	return m_strFilePathData;
}

/**********************************************************************/

const char* CDataManager::GetFilePathSmartEntPaymentMap ( int nDbIdx )
{
	if ( EcrmanOptions.GetGlobalPaymentFlag() == FALSE )
	{
		CFilenameUpdater FnUp( SysFiles::SEPaymentMap, nDbIdx, -1, FNUP_FAMILY_DATABASE );
		m_strFilePathData = FnUp.GetFilenameToUse();
	}
	else
	{
		CFilenameUpdater FnUp( SysFiles::SEPaymentMap );
		m_strFilePathData = FnUp.GetFilenameToUse();
	}
		
	return m_strFilePathData;
}

/**********************************************************************/

const char* CDataManager::GetFilePathAnalysisCategory ( int nDbIdx )
{
	if ( EcrmanOptions.GetGlobalAnalysisCategoryFlag() == FALSE )
	{
		CFilenameUpdater FnUp( SysFiles::AnalysisCategory, nDbIdx, -1, FNUP_FAMILY_DATABASE );
		m_strFilePathData = FnUp.GetFilenameToUse();
	}
	else
	{
		CFilenameUpdater FnUp( SysFiles::AnalysisCategory );
		m_strFilePathData = FnUp.GetFilenameToUse();
	}
		
	return m_strFilePathData;
}

/**********************************************************************/

const char* CDataManager::GetFilePathDbPluFilterNames( int nDbIdx )
{
	CFilenameUpdater FnUp( SysFiles::PluFilterNames, nDbIdx );
	m_strFilePathData = FnUp.GetFilenameToUse();
	return m_strFilePathData;
}

/**********************************************************************/

const char* CDataManager::GetFilePathDbPluFilterItems( int nDbIdx )
{
	CFilenameUpdater FnUp( SysFiles::PluFilterItems, nDbIdx );
	m_strFilePathData = FnUp.GetFilenameToUse();
	return m_strFilePathData;
}

/**********************************************************************/

const char* CDataManager::GetFilePathPriceText( int nDbIdx )
{
	CFilenameUpdater FnUp( SysFiles::PriceText );
	m_strFilePathData = FnUp.GetFilenameToUse();
	return m_strFilePathData;
}

/**********************************************************************/

const char* CDataManager::GetFilePathSBPriceText( int nDbIdx )
{
	CFilenameUpdater FnUp( SysFiles::SBPriceText );
	m_strFilePathData = FnUp.GetFilenameToUse();
	return m_strFilePathData;
}

/**********************************************************************/

const char* CDataManager::GetFilePathBarcode( int nDbIdx )
{
	CFilenameUpdater FnUp( SysFiles::Barcode, nDbIdx );
	m_strFilePathData = FnUp.GetFilenameToUse();
	return m_strFilePathData;
}

/**********************************************************************/

const char* CDataManager::GetFilePathShelfLabels( int nDbIdx )
{
	CFilenameUpdater FnUp( SysFiles::DbShelfLabels, nDbIdx );
	m_strFilePathData = FnUp.GetFilenameToUse();
	return m_strFilePathData;
}

/**********************************************************************/

const char* CDataManager::GetFilePathShelfLabels( int nLocIdx, int nPriceSet )
{
	m_strFilePathData = "";
	
	if ( nPriceSet >= 1 && nPriceSet <= MAX_PRICE_SETS )
	{
		CFilenameUpdater FnUp( SysFiles::LocShelfLabels, nLocIdx, nPriceSet );
		m_strFilePathData = FnUp.GetFilenameToUse();
	}

	return m_strFilePathData;
}

/**********************************************************************/

const char* CDataManager::GetFilePathTaxRates ( int nDbIdx )
{
	if ( EcrmanOptions.GetGlobalTaxFlag() == FALSE )
	{
		CFilenameUpdater FnUp( SysFiles::TaxRates, nDbIdx );
		m_strFilePathData = FnUp.GetFilenameToUse();
	}
	else
	{
		CFilenameUpdater FnUp( SysFiles::TaxRates, -1 );
		m_strFilePathData = FnUp.GetFilenameToUse();
	}
		
	return m_strFilePathData;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

int CDataManager::GetPromoDbIdx( int nDbIdx )
{
	if ( DealerFlags.GetGlobalPromotionFlag() == TRUE )
	{
		return 0;
	}
	else
	{
		return nDbIdx;
	}
}

/**********************************************************************/

const char* CDataManager::GetFilePathPromotion( int nDbIdx )
{
	CFilenameUpdater FnUp( SysFiles::Promotion, GetPromoDbIdx( nDbIdx ) );
	m_strFilePathData = FnUp.GetFilenameToUse();
	return m_strFilePathData;
}

/**********************************************************************/

const char* CDataManager::GetFilePathMixMatch( int nDbIdx )
{
	CFilenameUpdater FnUp( SysFiles::MixMatch, GetPromoDbIdx( nDbIdx ) );
	m_strFilePathData = FnUp.GetFilenameToUse();
	return m_strFilePathData;
}

/**********************************************************************/

const char* CDataManager::GetFilePathOfferGroup( int nDbIdx )
{
	CFilenameUpdater FnUp( SysFiles::OfferGroup, GetPromoDbIdx( nDbIdx ) );
	m_strFilePathData = FnUp.GetFilenameToUse();
	return m_strFilePathData;
}

/**********************************************************************/

const char* CDataManager::GetFilePathOfferGroupEntry( int nDbIdx )
{
	CFilenameUpdater FnUp( SysFiles::OfferEntry, GetPromoDbIdx( nDbIdx ) );
	m_strFilePathData = FnUp.GetFilenameToUse();
	return m_strFilePathData;
}

/**********************************************************************/

const char* CDataManager::GetFilePathTimePlan( int nDbIdx )
{
	CFilenameUpdater FnUp( SysFiles::TimePlan, GetPromoDbIdx( nDbIdx ) );
	m_strFilePathData = FnUp.GetFilenameToUse();
	return m_strFilePathData;
}

/**********************************************************************/

const char* CDataManager::GetFilePathTimePlanEntry( int nDbIdx )
{
	CFilenameUpdater FnUp( SysFiles::TimePlanEntry, GetPromoDbIdx( nDbIdx ) );
	m_strFilePathData = FnUp.GetFilenameToUse();
	return m_strFilePathData;
}

/**********************************************************************/

const char* CDataManager::GetFilePathTimePlanTemplateEntry(int nDbIdx)
{
	CFilenameUpdater FnUp(SysFiles::TimePlanTemplateEntry, GetPromoDbIdx(nDbIdx));
	m_strFilePathData = FnUp.GetFilenameToUse();
	return m_strFilePathData;
}

/**********************************************************************/

const char* CDataManager::GetFilePathWalletStampRule(int nDbIdx)
{
	int nDbIdxToUse = 0;
	if (EcrmanOptions.GetFeaturesGlobalWalletStampRuleFlag() == FALSE)
	{
		nDbIdxToUse = GetPromoDbIdx(nDbIdx);
	}
	else
	{
		nDbIdxToUse = -1;
	}

	switch (SysInfo.GetLoyaltyOrSmartPayType())
	{
	case LOYALTY_TYPE_SMP_V4:
	{
		CFilenameUpdater FnUp(SysFiles::SmartPayPurchaseControl, nDbIdxToUse);
		m_strFilePathData = FnUp.GetFilenameToUse();
	}
	break;

	case LOYALTY_TYPE_LOY_V4:
	default:
	{
		CFilenameUpdater FnUp(SysFiles::LoyaltyStampOffer, nDbIdxToUse);
		m_strFilePathData = FnUp.GetFilenameToUse();
	}
	break;
	}

	return m_strFilePathData;
}

/**********************************************************************/

const char* CDataManager::GetFilePathLoyaltyScheme(int nDbIdx)
{
	CFilenameUpdater FnUp(SysFiles::DbLoyaltyScheme, nDbIdx);
	m_strFilePathData = FnUp.GetFilenameToUse();
	return m_strFilePathData;
}

/**********************************************************************/

