/**********************************************************************/
#include "CashRSPImporterDiscountHelpers.h"
#include "HistoryFileTypes.h"
#include "LocationCSVArray.h"
#include "NetworkCSVArray.h"
#include "NodeTypes.h"
#include "PMSOptions.h"
#include "PMSReportFileFinder.h"
#include "PriceHelpers.h"
#include "ReportHelpers.h"
#include "ReportpointCSVArray.h"
#include "ReportTypes.h"
#include "StatusProgress.h"
#include "SysInfo.h"
/**********************************************************************/
#include "EposReportGeneral.h"
/**********************************************************************/

CEposReportConsolGeneralMisc::CEposReportConsolGeneralMisc()
{
	Reset();
}

/**********************************************************************/

void CEposReportConsolGeneralMisc::Reset()
{
	m_nLineType = 0;
	m_nLineData = 0;
	
	for ( int n = 0; n < 8; n++ )
	{
		m_dQty[n] = 0.0;
		m_dValue[n] = 0.0;
	}
}

/**********************************************************************/

void CEposReportConsolGeneralMisc::Add (CEposReportConsolGeneralMisc& source )
{
	for ( int n = 0; n < 8; n++ )
	{
		m_dQty[n] += source.m_dQty[n];
		m_dValue[n] += source.m_dValue[n];
	}
}

/**********************************************************************/

int CEposReportConsolGeneralMisc::Compare(CEposReportConsolGeneralMisc& source, int nHint)
{
	if (m_nLineType != source.m_nLineType)
	{
		return ((m_nLineType > source.m_nLineType) ? 1 : -1);
	}

	if (m_nLineData != source.m_nLineData)
	{
		return ((m_nLineData > source.m_nLineData) ? 1 : -1);
	}
	else
	{
		return 0;
	}
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEposReportConsolGeneralVariance::CEposReportConsolGeneralVariance()
{
	Reset();
}

/**********************************************************************/

void CEposReportConsolGeneralVariance::Reset()
{
	m_nMethodNo = 0;
	m_strDescription = "";
	
	for (int n = 0; n < 8; n++)
	{
		m_dValue[n] = 0.0;
	}
}

/**********************************************************************/

void CEposReportConsolGeneralVariance::Add(CEposReportConsolGeneralVariance& source)
{
	for (int n = 0; n < 8; n++)
	{
		m_dValue[n] += source.m_dValue[n];
	}
}

/**********************************************************************/

int CEposReportConsolGeneralVariance::Compare(CEposReportConsolGeneralVariance& source, int nHint)
{
	if (m_bTotal != source.m_bTotal)
	{
		return ((TRUE == m_bTotal) ? 1 : -1);
	}

	if (TRUE == m_bTotal)
	{
		return 0;
	}

	if (m_nMethodNo != source.m_nMethodNo)
	{
		return ((m_nMethodNo > source.m_nMethodNo) ? 1 : -1);
	}

	if (m_strDescription != source.m_strDescription)
	{
		return ((m_strDescription > source.m_strDescription) ? 1 : -1);
	}
	else
	{
		return 0;
	}
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEposReportConsolGeneralTaxPlu::CEposReportConsolGeneralTaxPlu()
{
	Reset();
}

/**********************************************************************/

void CEposReportConsolGeneralTaxPlu::Reset()
{
	m_nTaxDateCode = 0;
	m_nPluNo = 0;
	m_nModifier = 0;
	
	for ( int n = 0; n < 8; n++ )
	{
		m_dQty[n] = 0.0;
		m_dValue[n] = 0.0;
	}
}

/**********************************************************************/

void CEposReportConsolGeneralTaxPlu::Add (CEposReportConsolGeneralTaxPlu& source )
{
	for ( int n = 0; n < 8; n++ )
	{
		m_dQty[n] += source.m_dQty[n];
		m_dValue[n] += source.m_dValue[n];
	}
}

/**********************************************************************/

int CEposReportConsolGeneralTaxPlu::Compare(CEposReportConsolGeneralTaxPlu& source, int nHint)
{
	if (m_nTaxDateCode != source.m_nTaxDateCode)
	{
		return ((m_nTaxDateCode > source.m_nTaxDateCode) ? 1 : -1);
	}

	if (m_nPluNo != source.m_nPluNo)
	{
		return ((m_nPluNo > source.m_nPluNo) ? 1 : -1);
	}

	if (m_nModifier != source.m_nModifier)
	{
		return ((m_nModifier > source.m_nModifier) ? 1 : -1);
	}
	else
	{
		return 0;
	}
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEposReportConsolGeneralTaxBand::CEposReportConsolGeneralTaxBand()
{
	Reset();
}

/**********************************************************************/

void CEposReportConsolGeneralTaxBand::Reset()
{
	m_nTaxDateCode = 0;
	m_nTaxBand = 0;
	
	for ( int n = 0; n < 8; n++ )
	{
		m_dQty[n] = 0.0;
		m_dValue[n] = 0.0;
	}
}

/**********************************************************************/

void CEposReportConsolGeneralTaxBand::Add (CEposReportConsolGeneralTaxBand& source )
{
	for ( int n = 0; n < 8; n++ )
	{
		m_dQty[n] += source.m_dQty[n];
		m_dValue[n] += source.m_dValue[n];
	}
}

/**********************************************************************/

int CEposReportConsolGeneralTaxBand::Compare(CEposReportConsolGeneralTaxBand& source, int nHint)
{
	if (m_nTaxBand != source.m_nTaxBand)
	{
		return ((m_nTaxBand > source.m_nTaxBand) ? 1 : -1);
	}

	if (m_nTaxDateCode != source.m_nTaxDateCode)
	{
		return ((m_nTaxDateCode > source.m_nTaxDateCode) ? 1 : -1);
	}
	else
	{
		return 0;
	}
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEposReportGeneralBlock::CEposReportGeneralBlock()
{
	m_nEntityType = 0;
	m_nDbIdx = 0;
	m_nLocIdx = 0;
	m_nRpIdx = 0;
	m_nTermIdx = 0;
	m_nOriginalTaxBandSize = 0;
	m_pSales = NULL;
	m_pTaxByBand = NULL;
	m_pTaxByPlu = NULL;
	m_pPayment = NULL;
	m_pVariance = NULL;
	m_pVarianceForEx = NULL;
	m_pPromoDetail = NULL;
	m_pPromoSummary = NULL;
	m_pVoid = NULL;
	m_pNoSale = NULL;
	m_pRecon = NULL;
}

/**********************************************************************/

bool CEposReportGeneralBlock::GotData()
{
	if (m_pSales->GetNodeCount() < 2)
	{
		return TRUE;
	}

	if (m_pTaxByBand->GetSize() != 0)
	{
		return TRUE;
	}

	if (m_pTaxByPlu->GetSize() != 0)
	{
		return TRUE;
	}

	if (m_pPayment->GetSize() != 0)
	{
		return TRUE;
	}

	if (m_pVariance->GetSize() != 0)
	{
		return TRUE;
	}

	if (m_pVarianceForEx->GetSize() != 0)
	{
		return TRUE;
	}

	if (m_pPromoDetail->GetNodeCount() < 2)
	{
		return TRUE;
	}

	if (m_pPromoSummary->GetSize() != 0)
	{
		return TRUE;
	}

	if (m_pVoid->GetSize() != 0)
	{
		return TRUE;
	}

	if (m_pNoSale->GetSize() != 0)
	{
		return TRUE;
	}

	return FALSE;
}

/**********************************************************************/

__int64 CEposReportGeneralBlock::GetLineCount( CEposReportCustomSettingsGeneral& Settings )
{
	__int64 nCount = 0;
	
	nCount += m_pSales -> GetNodeCount();
	nCount += m_pPayment -> GetSize();
	nCount += m_pVariance -> GetSize();
	nCount += m_pVarianceForEx -> GetSize();

	if (Settings.GetTaxBandFlag() || Settings.GetTaxAmountFlag())
	{
		nCount += m_pTaxByPlu->GetSize();
	}

	m_nOriginalTaxBandSize = m_pTaxByBand -> GetSize();

	if (Settings.GetTaxBandFlag())
	{
		nCount += m_nOriginalTaxBandSize;
	}

	if (Settings.GetTaxAmountFlag())
	{
		nCount += m_nOriginalTaxBandSize;
	}

	if (Settings.GetPromoDetailFlag())
	{
		nCount += m_pPromoDetail->GetNodeCount();
	}

	if (Settings.GetPromoSummaryFlag())
	{
		nCount += m_pPromoSummary->GetSize();
	}

	if (Settings.GetReconciliationFlag())
	{
		nCount += m_pRecon->GetSize();
	}

	if (Settings.GetVoidFlag())
	{
		nCount += m_pVoid->GetSize();
	}

	if (Settings.GetNoSaleFlag())
	{
		nCount += m_pNoSale->GetSize();
	}

	return nCount;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CGeneralReportSalesBlockEntity::CGeneralReportSalesBlockEntity()
{
	m_nEntityType = 0;
	m_nEntityNum = 0;
	m_strHeader = "";
	m_strMixMatchType = "";
	m_strTotal = "";
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEposReportGeneral::CEposReportGeneral( CEposSelectArray& SelectArray, int nReportType ) : CEposReport( SelectArray )
{
	m_bCanConsolidateGlobally = TRUE;
	
	switch( nReportType )
	{
	case REPORT_TYPE_GENERAL_GROUP:
	case REPORT_TYPE_GENERAL_GROUP_QTY:
	case REPORT_TYPE_WEEKLY_GROUP:
	case REPORT_TYPE_WEEKLY_GROUP_QTY:
	case REPORT_TYPE_GENERAL_DEPT:
	case REPORT_TYPE_GENERAL_DEPT_QTY:
	case REPORT_TYPE_WEEKLY_DEPT:
	case REPORT_TYPE_WEEKLY_DEPT_QTY:
		m_nReportType = nReportType;
		break;

	default:
		m_nReportType = REPORT_TYPE_GENERAL_DEPT;
		break;
	}

	m_ReportSettings.SetWeekdayFlag( FALSE );
	m_ReportSettings.SetPromoDetailFlag( FALSE );
	m_ReportSettings.SetPromoSummaryFlag( FALSE );
	m_ReportSettings.SetReconciliationFlag( TRUE );
	m_ReportSettings.SetVoidFlag( FALSE );
	m_ReportSettings.SetNoSaleFlag( FALSE );
	m_ReportSettings.SetConsolDeptFlag( FALSE );
	m_ReportSettings.SetConsolGroupReportFlag( FALSE );
	m_ReportSettings.SetConsolGroupConsolFlag( FALSE );
	m_ReportSettings.SetConsolGroupEposFlag( FALSE );
	m_ReportSettings.SetQuantityFlag( FALSE );

	//WEEKLY FLAG
	switch( m_nReportType )
	{
	case REPORT_TYPE_WEEKLY_DEPT:
	case REPORT_TYPE_WEEKLY_DEPT_QTY:
	case REPORT_TYPE_WEEKLY_GROUP:
	case REPORT_TYPE_WEEKLY_GROUP_QTY:
		m_ReportSettings.SetWeekdayFlag( TRUE );
		break;
	}

	//REPORT TYPE
	switch( m_nReportType )
	{
	case REPORT_TYPE_GENERAL_GROUP_QTY:
	case REPORT_TYPE_WEEKLY_GROUP_QTY:
	case REPORT_TYPE_GENERAL_DEPT_QTY:
	case REPORT_TYPE_WEEKLY_DEPT_QTY:
		m_ReportSettings.SetPromoDetailFlag( TRUE );
		m_ReportSettings.SetPromoSummaryFlag( TRUE );
		m_ReportSettings.SetReconciliationFlag( FALSE );
		m_ReportSettings.SetVoidFlag( TRUE );
		m_ReportSettings.SetNoSaleFlag( TRUE );
		m_ReportSettings.SetQuantityFlag( TRUE );
		break;
	}

	//CONSOLIDATE BY DEPT OR GROUP
	switch( nReportType )
	{
	case REPORT_TYPE_GENERAL_DEPT:
	case REPORT_TYPE_GENERAL_DEPT_QTY:
	case REPORT_TYPE_WEEKLY_DEPT:
	case REPORT_TYPE_WEEKLY_DEPT_QTY:
		m_ReportSettings.SetConsolDeptFlag( TRUE );
		break;

	case REPORT_TYPE_GENERAL_GROUP:
	case REPORT_TYPE_GENERAL_GROUP_QTY:
	case REPORT_TYPE_WEEKLY_GROUP:
	case REPORT_TYPE_WEEKLY_GROUP_QTY:
		m_ReportSettings.SetConsolGroupEposFlag( TRUE );
		break;
	}

	InitialiseHelperStrings();
	SetPMSDepositsFlag( TRUE );
}

/**********************************************************************/

CEposReportGeneral::CEposReportGeneral( CEposSelectArray& SelectArray, int nReportType, const char* szCustomSettings ) : CEposReport( SelectArray )
{
	m_nReportType = nReportType;
	
	CString strParams = szCustomSettings;
	m_ReportSettings.SetSettingsCSVLine( strParams );

	InitialiseHelperStrings();
	SetPMSDepositsFlag( TRUE );
}

/**********************************************************************/

void CEposReportGeneral::InitialiseHelperStrings()
{
	m_strWeekDays[0] = "Sun";
	m_strWeekDays[1] = "Mon";
	m_strWeekDays[2] = "Tue";
	m_strWeekDays[3] = "Wed";
	m_strWeekDays[4] = "Thu";
	m_strWeekDays[5] = "Fri";
	m_strWeekDays[6] = "Sat";

	if ( m_ReportSettings.GetWeekdayFlag() == TRUE )
	{
		if ( m_ReportSettings.GetQuantityFlag() == TRUE )
		{
			m_strUnderline = "\t\t<LI>\t<LI>\t<LI>\t<LI>\t<LI>\t<LI>\t<LI>\t<LI>\t<LI>\t<LI>\t<LI>\t<LI>\t<LI>\t<LI>\t<LI>\t<LI>";
			m_strReconUnderline = "\t\t\t<LI>\t\t<LI>\t\t<LI>\t\t<LI>\t\t<LI>\t\t<LI>\t\t<LI>\t\t<LI>";
		}
		else
		{
			m_strUnderline = "\t\t<LI>\t<LI>\t<LI>\t<LI>\t<LI>\t<LI>\t<LI>\t<LI>";	
			m_strReconUnderline = m_strUnderline;
		}
	}
	else
	{
		if ( m_ReportSettings.GetQuantityFlag() == TRUE )
		{
			m_strUnderline = "\t\t<LI>\t<LI>";
			m_strReconUnderline = "\t\t\t<LI>";
		}
		else
		{
			m_strUnderline =  "\t\t<LI>";
			m_strReconUnderline = m_strUnderline;
		}
	}
}

/**********************************************************************/

CEposReportGeneral::~CEposReportGeneral()
{
	for ( int n = 0; n < m_arrayReportBlocks.GetSize(); n++ )
	{
		m_arrayReportBlocks[n].m_pSales -> DestroyTree();
		m_arrayReportBlocks[n].m_pTaxByBand -> RemoveAll();
		m_arrayReportBlocks[n].m_pTaxByPlu -> RemoveAll();
		m_arrayReportBlocks[n].m_pPayment -> RemoveAll();
		m_arrayReportBlocks[n].m_pVariance -> RemoveAll();
		m_arrayReportBlocks[n].m_pVarianceForEx -> RemoveAll();
		m_arrayReportBlocks[n].m_pPromoDetail -> DestroyTree();
		m_arrayReportBlocks[n].m_pPromoSummary -> RemoveAll();
		m_arrayReportBlocks[n].m_pVoid -> RemoveAll();
		m_arrayReportBlocks[n].m_pNoSale -> RemoveAll();
		m_arrayReportBlocks[n].m_pRecon -> RemoveAll();
		delete ( m_arrayReportBlocks[n].m_pSales );
		delete ( m_arrayReportBlocks[n].m_pTaxByBand );
		delete ( m_arrayReportBlocks[n].m_pTaxByPlu );
		delete ( m_arrayReportBlocks[n].m_pPayment );
		delete ( m_arrayReportBlocks[n].m_pVariance );
		delete ( m_arrayReportBlocks[n].m_pVarianceForEx );
		delete ( m_arrayReportBlocks[n].m_pPromoDetail );
		delete ( m_arrayReportBlocks[n].m_pPromoSummary );
		delete ( m_arrayReportBlocks[n].m_pVoid );
		delete ( m_arrayReportBlocks[n].m_pNoSale );
		delete ( m_arrayReportBlocks[n].m_pRecon );
	}
}

/**********************************************************************/

void CEposReportGeneral::GetConsolidationParams( CEposReportConsolParamsStandard& Params )
{
	CString strParams = "";
	switch( m_nPreferConsolType )
	{
	case EPOSREPORT_CONSOL_SAVED:
		strParams = m_EposReportSelectInfo.GetConsolPrefsSaved();
		break;

	case EPOSREPORT_CONSOL_ADHOC:
		strParams = m_EposReportSelectInfo.GetConsolPrefsAdhoc();
		break;
		
	case EPOSREPORT_CONSOL_DEFAULT:
		strParams = m_EposReportSelectInfo.GetConsolPrefsDefault();
		break;
		
	case EPOSREPORT_CONSOL_SYSTEM:
	default:
		strParams = m_EposReportSelectInfo.GetConsolPrefsSummary();
		break;
	}

	Params.LoadSettings( strParams );

	if ( ( Params.m_bConsolSys | Params.m_bConsolDb | Params.m_bConsolLoc | Params.m_bConsolTerm ) == FALSE )
	{
		strParams = m_EposReportSelectInfo.GetConsolPrefsDefault();
		Params.LoadSettings( strParams );
	}

	if ( ( FALSE == m_bCanConsolidateGlobally ) && ( TRUE == Params.m_bConsolSys ) )
	{
		Params.m_bConsolSys = FALSE;
		Params.m_bConsolDb = TRUE;
	}
}

/**********************************************************************/

void CEposReportGeneral::InitialiseConsolidationBlocks()
{
	m_bCanConsolidateGlobally = ( NODE_SYSTEM == m_nConLevel );
	m_bCanConsolidateGlobally &= EcrmanOptions.GetGlobalPaymentFlag();
	m_bCanConsolidateGlobally &= ( m_ReportSettings.GetPromoSummaryFlag() == FALSE );
	m_bCanConsolidateGlobally &= ( m_ReportSettings.GetPromoDetailFlag() == FALSE );

	//CONSOLIDATE SALES BY DEPARTMENT AND / OR SELECTED GROUP TYPES
	m_arraySalesLevels.RemoveAll();
	
	if ( m_ReportSettings.GetConsolGroupConsolFlag() == TRUE )
	{
		m_arraySalesLevels.Add( NODE_DEPT_GROUP_CONSOL );
		m_bCanConsolidateGlobally &= EcrmanOptions.GetGlobalDeptGroupConsolFlag();
	}

	if ( m_ReportSettings.GetConsolGroupReportFlag() == TRUE )
	{
		m_arraySalesLevels.Add( NODE_DEPT_GROUP_REPORT );
		m_bCanConsolidateGlobally &= EcrmanOptions.GetGlobalDeptGroupReportFlag();
	}

	if ( m_arraySalesLevels.GetSize() == 0 )
	{
		if ( m_ReportSettings.GetConsolGroupEposFlag() == TRUE )
		{
			m_arraySalesLevels.Add( NODE_DEPT_GROUP_EPOS );
			m_bCanConsolidateGlobally &= EcrmanOptions.GetGlobalDeptGroupEposFlag();
		}
	}

	if ( ( m_ReportSettings.GetConsolDeptFlag() == TRUE ) || ( m_arraySalesLevels.GetSize() == 0 ) )
	{
		m_arraySalesLevels.Add( NODE_DEPARTMENT );
		m_bCanConsolidateGlobally &= EcrmanOptions.GetGlobalDepartmentFlag();
	}

	if ( m_ReportSettings.GetTaxBandFlag() )
		m_bCanConsolidateGlobally &= EcrmanOptions.GetGlobalTaxFlag();

	m_arrayPromoLevels.RemoveAll();
	m_arrayPromoLevels.Add( NODE_PROMOTION );

	for ( int n = 0; n < m_arraySalesLevels.GetSize(); n++ )
		m_arrayPromoLevels.Add( m_arraySalesLevels.GetAt(n) );

	CEposReportConsolParamsStandard Params;
	GetConsolidationParams( Params );
	
	//BUILD BLOCK MAP OF LOCATIONS, TERMINALS ETC AND ADD RECONCILIATION SECTION
	m_SelectArray.MakeList();
	m_BlockMap.SetConsolidateByTerminalFlag( Params.m_bConsolTerm );
	m_BlockMap.SetConsolidateByLocationFlag( Params.m_bConsolLoc );
	m_BlockMap.SetConsolidateByDatabaseFlag( Params.m_bConsolDb );
	m_BlockMap.SetConsolidateBySystemFlag( Params.m_bConsolSys );
	m_BlockMap.BuildMap( m_SelectArray );

	for ( int n = 0; n < m_BlockMap.GetBlockMapEntityCount(); n++ )
	{
		CEposReportBlockMapEntity entity;
		m_BlockMap.GetBlockMapEntity( n, entity );

		CEposReportGeneralBlock block;
		block.m_nEntityType = entity.m_nEntityType;
		block.m_nDbIdx = entity.m_nDbIdx;
		block.m_nLocIdx = entity.m_nLocIdx;
		block.m_nRpIdx = entity.m_nRpIdx;
		block.m_nTermIdx = entity.m_nTermIdx;
		block.m_pSales = new CEntityTreeGeneralSales;
		block.m_pTaxByBand = new CReportConsolidationArray<CEposReportConsolGeneralTaxBand>;
		block.m_pTaxByPlu = new CReportConsolidationArray<CEposReportConsolGeneralTaxPlu>;
		block.m_pPayment = new CReportConsolidationArray<CEposReportConsolGeneralMisc>;
		block.m_pVariance = new CReportConsolidationArray<CEposReportConsolGeneralVariance>;
		block.m_pVarianceForEx = new CReportConsolidationArray<CEposReportConsolGeneralVariance>;
		block.m_pPromoDetail = new CEntityTreeGeneralSales;
		block.m_pPromoSummary = new CReportConsolidationArray<CEposReportConsolGeneralMisc>;
		block.m_pVoid = new CReportConsolidationArray<CEposReportConsolGeneralMisc>;
		block.m_pNoSale = new CReportConsolidationArray<CEposReportConsolGeneralMisc>;
		block.m_pRecon = new CReportConsolidationArray<CEposReportConsolGeneralMisc>;

		for ( int n = 0; n < m_arraySalesLevels.GetSize(); n++ )
			block.m_pSales -> AddField( SORT_TYPE_INT );
	
		for ( int n = 0; n < m_arrayPromoLevels.GetSize(); n++ )
			block.m_pPromoDetail -> AddField( SORT_TYPE_INT );
		
		if ( m_ReportSettings.GetReconciliationFlag() == TRUE )
		{
			CArray<int,int> arrayCount;
			arrayCount.Add( GENREP_RECON_SALE );			//ITEM SALES
			arrayCount.Add( GENREP_RECON_SALE_FILTERED );	//ITEM SALES
			arrayCount.Add( GENREP_RECON_RA_DEPOSIT );		//DEPOSITS PURCHASED
			arrayCount.Add( GENREP_RECON_RA_CUSTOMER );		//CUSTOMER RA
			arrayCount.Add( GENREP_RECON_RA_ROOM );			//ROOM RA
			arrayCount.Add( GENREP_RECON_RA_LOYALTY );		//LOYALTY RA
			arrayCount.Add( GENREP_RECON_RA_SMARTPAY );		//SMARTPAY RA
			arrayCount.Add( GENREP_RECON_RA_SMARTPHONE );	//SMARTPHONE RA
			arrayCount.Add( GENREP_RECON_RA_SPTBOOK );		//SPORTS BOOKER RA
			arrayCount.Add( GENREP_RECON_GRATUITY );		//GRATUITIES
			arrayCount.Add( GENREP_RECON_CASHBACK );		//CASHBACK
			arrayCount.Add( GENREP_RECON_SURPLUS );			//SURPLUS
			arrayCount.Add( GENREP_RECON_CHANGE );			//CHANGE
			arrayCount.Add( GENREP_RECON_PAYIN01 );			//PAID IN 1
			arrayCount.Add( GENREP_RECON_PAYIN02 );			//PAID IN 2
			arrayCount.Add( GENREP_RECON_PAYIN03 );			//PAID IN 3
			arrayCount.Add( GENREP_RECON_PAYIN04 );			//PAID IN 4
			arrayCount.Add( GENREP_RECON_PAYIN05 );			//PAID IN 5
			arrayCount.Add( GENREP_RECON_PAYIN06 );			//PAID IN 6
			arrayCount.Add( GENREP_RECON_PAYIN07 );			//PAID IN 7
			arrayCount.Add( GENREP_RECON_PAYIN08 );			//PAID IN 8
			arrayCount.Add( GENREP_RECON_PAYIN09 );			//PAID IN 9
			arrayCount.Add( GENREP_RECON_PAYIN10 );			//PAID IN 10
			arrayCount.Add( GENREP_RECON_PAYIN11 );			//PAID IN 11
			arrayCount.Add( GENREP_RECON_PAYIN12 );			//PAID IN 12
			arrayCount.Add( GENREP_RECON_PAYIN13 );			//PAID IN 13
			arrayCount.Add( GENREP_RECON_PAYIN14 );			//PAID IN 14
			arrayCount.Add( GENREP_RECON_PAYIN15 );			//PAID IN 15
			arrayCount.Add( GENREP_RECON_PAYIN16 );			//PAID IN 16
			arrayCount.Add( GENREP_RECON_PAYIN17 );			//PAID IN 17
			arrayCount.Add( GENREP_RECON_PAYIN18 );			//PAID IN 18
			arrayCount.Add( GENREP_RECON_PAYIN19 );			//PAID IN 19
			arrayCount.Add( GENREP_RECON_PAYIN20 );			//PAID IN 20
			arrayCount.Add( GENREP_RECON_PAYINXX );			//PAID IN OTHER
			arrayCount.Add( GENREP_RECON_CHARGE_PAID );		//PAYMENTS RECEIVED
			arrayCount.Add( GENREP_RECON_REFUND_DEPOSIT );	//DEPOSITS REFUNDED
			arrayCount.Add( GENREP_RECON_CHARGE_DEPOSIT );	//DEPOSITS REDEEMED
			arrayCount.Add( GENREP_RECON_CHARGE_CUSTOMER );	//POST TO ACCOUNT
			arrayCount.Add( GENREP_RECON_CHARGE_ROOM );		//POST TO ROOM
			arrayCount.Add( GENREP_RECON_CHARGE_LOYALTY );	//LOYALTY REDEMPTION
			arrayCount.Add( GENREP_RECON_CHARGE_SMARTPAY );	//SMARTPAY REDEMPTION
			arrayCount.Add( GENREP_RECON_CHARGE_SMARTPHONE );//SMARTPHONE REDEMPTION
			arrayCount.Add( GENREP_RECON_CHARGE_SPTBOOK );	//SPORTS BOOKER REDEMPTION
			arrayCount.Add( GENREP_RECON_PAYOUT01 );			//PAID OUT 1
			arrayCount.Add( GENREP_RECON_PAYOUT02 );			//PAID OUT 2
			arrayCount.Add( GENREP_RECON_PAYOUT03 );			//PAID OUT 3
			arrayCount.Add( GENREP_RECON_PAYOUT04 );			//PAID OUT 4
			arrayCount.Add( GENREP_RECON_PAYOUT05 );			//PAID OUT 5
			arrayCount.Add( GENREP_RECON_PAYOUT06 );			//PAID OUT 6
			arrayCount.Add( GENREP_RECON_PAYOUT07 );			//PAID OUT 7
			arrayCount.Add( GENREP_RECON_PAYOUT08 );			//PAID OUT 8
			arrayCount.Add( GENREP_RECON_PAYOUT09 );			//PAID OUT 9
			arrayCount.Add( GENREP_RECON_PAYOUT10 );			//PAID OUT 10
			arrayCount.Add( GENREP_RECON_PAYOUT11 );			//PAID OUT 11
			arrayCount.Add( GENREP_RECON_PAYOUT12 );			//PAID OUT 12
			arrayCount.Add( GENREP_RECON_PAYOUT13 );			//PAID OUT 13
			arrayCount.Add( GENREP_RECON_PAYOUT14 );			//PAID OUT 14
			arrayCount.Add( GENREP_RECON_PAYOUT15 );			//PAID OUT 15
			arrayCount.Add( GENREP_RECON_PAYOUT16 );			//PAID OUT 16
			arrayCount.Add( GENREP_RECON_PAYOUT17 );			//PAID OUT 17
			arrayCount.Add( GENREP_RECON_PAYOUT18 );			//PAID OUT 18
			arrayCount.Add( GENREP_RECON_PAYOUT19 );			//PAID OUT 19
			arrayCount.Add( GENREP_RECON_PAYOUT20 );			//PAID OUT 20
			arrayCount.Add( GENREP_RECON_PAYOUTXX );			//PAID OUT OTHER
			arrayCount.Add( GENREP_RECON_HOLTS_PROMO );		//PROMOS FILTERED BY MISC21
			arrayCount.Add( GENREP_RECON_BALANCE );			//BALANCE

			for ( int r = 0; r < arrayCount.GetSize(); r++ )
			{
				CEposReportConsolGeneralMisc Recon;
				Recon.m_nLineType = arrayCount.GetAt(r);
				Recon.m_nLineData = 0;
				block.m_pRecon -> Consolidate( Recon );
			}
		}

		m_arrayReportBlocks.Add( block );
	}
}

/**********************************************************************/

void CEposReportGeneral::ConsolidateItemSale( int nWeekday, CArray<int,int>& arrayGroupOrDept, __int64 nPluNo, int nModifier, int nTaxDateCode, int nTaxBand, double dQty, double dAmount ) 
{
	int nLevels = arrayGroupOrDept.GetSize();

	for ( int x = 0; x < nLevels; x++ )
		if ( arrayGroupOrDept.GetAt(x) == 0 )
			arrayGroupOrDept.SetAt( x, 1000 );

	for ( int n = 0; n < m_arrayTerminalBlocks.GetSize(); n++ )
	{
		int nBlock = m_arrayTerminalBlocks.GetAt(n);

		CEntityTreeGeneralSales* pConsolArray = m_arrayReportBlocks[ nBlock ].m_pSales;
		
		CEntityTreeGeneralSalesNode Sales;		
		Sales.SetQty( nWeekday, dQty );
		Sales.SetVal( nWeekday, dAmount );
		Sales.SetQty( 7, dQty );
		Sales.SetVal( 7, dAmount );

		for ( int n = 0; n < nLevels; n++ )
			pConsolArray -> SetBufferInt( n, arrayGroupOrDept.GetAt(n) );

		pConsolArray -> Consolidate( Sales );

		if ( m_ReportSettings.GetTaxBandFlag() || m_ReportSettings.GetTaxAmountFlag() )
		{
			if ( (DealerFlags.GetUseDatabaseVATBandFlag() == FALSE) || ( 0 == nPluNo ) )
			{
				CEposReportConsolGeneralTaxBand infoTax;
				infoTax.m_nTaxDateCode = nTaxDateCode;
				infoTax.m_nTaxBand = nTaxBand;
				infoTax.m_dQty[nWeekday] = dQty;
				infoTax.m_dValue[nWeekday] = dAmount;
				infoTax.m_dQty[7] = dQty;
				infoTax.m_dValue[7] = dAmount;
				m_arrayReportBlocks[nBlock].m_pTaxByBand -> Consolidate( infoTax );
			}
			else
			{
				CEposReportConsolGeneralTaxPlu infoTax;
				infoTax.m_nTaxDateCode = nTaxDateCode;
				infoTax.m_nPluNo = nPluNo;
				infoTax.m_nModifier = nModifier;
				infoTax.m_dQty[nWeekday] = dQty;
				infoTax.m_dValue[nWeekday] = dAmount;
				infoTax.m_dQty[7] = dQty;
				infoTax.m_dValue[7] = dAmount;
				m_arrayReportBlocks[nBlock].m_pTaxByPlu -> Consolidate( infoTax );
			}
		}
	}

	ConsolidateReconcile( nWeekday, GENREP_RECON_SALE, dQty, dAmount );
	ConsolidateReconcile( nWeekday, GENREP_RECON_BALANCE, dQty, dAmount );
}

/**********************************************************************/

void CEposReportGeneral::ConsolidateItemPromo( int nWeekday, CArray<int,int>& arrayGroupOrDept, int nPromoNo, int nLineType, double dAmount ) 
{
	int nLevels = arrayGroupOrDept.GetSize();

	for ( int x = 0; x < nLevels; x++ )
		if ( arrayGroupOrDept.GetAt(x) == 0 )
			arrayGroupOrDept.SetAt( x, 1000 );

	for ( int n = 0; n < m_arrayTerminalBlocks.GetSize(); n++ )
	{
		int nBlock = m_arrayTerminalBlocks.GetAt(n);

		CEntityTreeGeneralSalesNode Sales;		
		Sales.SetQty( nWeekday, 1.0 );
		Sales.SetVal( nWeekday, dAmount );
		Sales.SetQty( 7, 1.0 );
		Sales.SetVal( 7, dAmount );

		for ( int n = 0; n < nLevels; n++ )
			m_arrayReportBlocks[ nBlock ].m_pPromoDetail -> SetBufferInt( n, arrayGroupOrDept.GetAt(n) );

		m_arrayReportBlocks[ nBlock ].m_pPromoDetail -> Consolidate( Sales );

		if ( m_ReportSettings.GetPromoSummaryFlag() == TRUE )
		{
			CEposReportConsolGeneralMisc Promo;
			Promo.m_nLineType = nLineType; 
			Promo.m_nLineData = nPromoNo;
			Promo.m_dQty[ nWeekday ] = 1.0;
			Promo.m_dValue[ nWeekday ] = dAmount;
			Promo.m_dQty[ 7 ] = 1.0;
			Promo.m_dValue[ 7 ] = dAmount;
			m_arrayReportBlocks[ nBlock ].m_pPromoSummary -> Consolidate( Promo );

			Promo.m_nLineType += 3;
			Promo.m_nLineData = 0;
			m_arrayReportBlocks[ nBlock ].m_pPromoSummary -> Consolidate( Promo );

			Promo.m_nLineType = GENREP_PROMO_TOTAL_ALL;
			m_arrayReportBlocks[ nBlock ].m_pPromoSummary -> Consolidate( Promo );
		}
	}
}

/**********************************************************************/

void CEposReportGeneral::ConsolidateSpecialItem( int nWeekday, int nType, double dQty, double dAmount, bool bRefund ) 
{
	int nLineData = 0;

	switch( nType )
	{
	case CASHRSP_ITEMPAYTYPE_DEPOSIT:		nLineData = GENREP_RECON_RA_DEPOSIT;	break;
	case CASHRSP_ITEMPAYTYPE_CUSTOMER:		nLineData = GENREP_RECON_RA_CUSTOMER;	break;
	case CASHRSP_ITEMPAYTYPE_ROOM:			nLineData = GENREP_RECON_RA_ROOM;		break;
	case CASHRSP_ITEMPAYTYPE_LOYALTY:		nLineData = GENREP_RECON_RA_LOYALTY;	break;
	case CASHRSP_ITEMPAYTYPE_SMARTPAY:		nLineData = GENREP_RECON_RA_SMARTPAY;	break;
	case CASHRSP_ITEMPAYTYPE_SMARTPHONE:	nLineData = GENREP_RECON_RA_SMARTPHONE;	break;
	case CASHRSP_ITEMPAYTYPE_SPTBOOK:		nLineData = GENREP_RECON_RA_SPTBOOK;	break;
	}

	if ( nLineData != 0 )
	{
		if ( ( GENREP_RECON_RA_DEPOSIT == nLineData ) && ( TRUE == bRefund ) )
			ConsolidateReconcile( nWeekday, GENREP_RECON_REFUND_DEPOSIT, dQty, -dAmount );
		else
			ConsolidateReconcile( nWeekday, nLineData, dQty, dAmount );

		ConsolidateReconcile( nWeekday, GENREP_RECON_BALANCE, dQty, dAmount );
	}
}

/**********************************************************************/

void CEposReportGeneral::ConsolidateVariance( int nWeekday, bool bFX, bool bTotal, int nMethodNo, const char* szDescription, double dValue )
{
	for ( int n = 0; n < m_arrayTerminalBlocks.GetSize(); n++ )
	{
		int nBlock = m_arrayTerminalBlocks.GetAt(n);

		CEposReportConsolGeneralVariance Variance;
		Variance.m_bTotal = bTotal;
		Variance.m_nMethodNo = nMethodNo;
		Variance.m_strDescription = szDescription;
		Variance.m_dValue[7] = dValue;
		Variance.m_dValue[nWeekday] = dValue;

		if ( TRUE == bFX )
			m_arrayReportBlocks[ nBlock ].m_pVarianceForEx -> Consolidate( Variance );
		else
			m_arrayReportBlocks[ nBlock ].m_pVariance -> Consolidate( Variance );
	}
}

/**********************************************************************/

void CEposReportGeneral::ConsolidatePaymentBuffer( int nWeekday ) 
{
	if ( m_ReportSettings.GetReconciliationFlag() == TRUE )
	{
		if ( m_arrayPaidIOBuffer.GetSize() != 0 )
		{
			CCSVEposTermLine csvIn ( m_arrayPaidIOBuffer.GetAt(0) );
			bool bPaidOut = ( csvIn.GetPaidIODirection() == 0 );
					
			for ( int n = 0; n < m_arrayPaidIOBuffer.GetSize(); n++ )
			{
				CCSVEposTermLine csvIn ( m_arrayPaidIOBuffer.GetAt(n) );

				CString strVoid = csvIn.GetPaidIOVoidFlag();
				if ( strVoid != "V" )
				{
					int nNo = csvIn.GetPaidIONumber();
					double dQty = csvIn.GetPaidIOQty();
					
					if ((nNo < 1) || (nNo > PAIDIO_TEXT_COUNT))
					{
						nNo = PAIDIO_TEXT_COUNT + 1;
					}

					double dValue = ( ( double ) csvIn.GetPaidIOValue() ) / 100.0;
						
					if (FALSE == bPaidOut)
					{
						ConsolidateReconcile(nWeekday, 100 + nNo, dQty, dValue);		//PAID IN
					}
					else
					{
						ConsolidateReconcile(nWeekday, 300 + nNo, dQty, -dValue);		//PAID OUT
					}

					ConsolidateReconcile( nWeekday, GENREP_RECON_BALANCE, dQty, dValue );	//RECONCILE BALANCE
				}
			}
		}
	}
	
	for ( int n = 0; n < m_arrayPaymentBuffer.GetSize(); n++ )
	{
		CCSVEposTermLine csvIn ( m_arrayPaymentBuffer.GetAt(n) );

		int nPaymentNo = csvIn.GetPaymentNumber();
		int nTender = csvIn.GetPaymentTender();
		int nAmount = csvIn.GetPaymentAmount();
		int nGratuity = csvIn.GetPaymentGratuity();
		int nCashback = csvIn.GetPaymentCashback();
		int nSurplus = csvIn.GetPaymentSurplus( m_CashRSPVersionChecker.GetCurrentTransactionVersionNo() );
		int nChange = nTender - ( nAmount + nGratuity + nSurplus );
		
		int nAccountType, nDummy32;
		__int64 nDummy64;
		bool bIsCash;
		csvIn.GetPaymentAccountTypeAndID( m_CashRSPVersionChecker, nAccountType, nDummy32, nDummy64, bIsCash );
		bool bUsedDeposit = ( CASHRSP_ITEMPAYTYPE_DEPOSIT == nAccountType );

		double dTender = ( (double) nTender ) / 100.0;

		int nLineType = 0;

		int nPaymentNoForChange = 1;
		if ( ( nPaymentNo != 1 ) && ( TRUE == bIsCash ) )
		{
			switch( EcrmanOptions.GetReportsMultiCashMethod() )
			{
			case REPORTCASH_ALL:
				nPaymentNoForChange = nPaymentNo;
				break;

			case REPORTCASH_METHOD1_CONSOLIDATE:
				nPaymentNo = 1;
				break;

			case REPORTCASH_METHOD1_CHANGE:
				break;

			case REPORTCASH_METHOD1_ONLY:
			default:
				bIsCash = FALSE;
				break;
			}
		}

		switch( nAccountType )
		{
		case CASHRSP_ITEMPAYTYPE_DEPOSIT:		
			nLineType = GENREP_RECON_CHARGE_DEPOSIT;
			break;

		case CASHRSP_ITEMPAYTYPE_CUSTOMER:	
			nLineType = GENREP_RECON_CHARGE_CUSTOMER;
			break;

		case CASHRSP_ITEMPAYTYPE_ROOM:	
			nLineType = GENREP_RECON_CHARGE_ROOM;	
			break;

		case CASHRSP_ITEMPAYTYPE_LOYALTY:		
			if ( DealerFlags.GetGenRecExcludeLoyaltyFlag() == TRUE )
			{
				nAccountType = CASHRSP_ITEMPAYTYPE_NORMAL;
				nLineType = GENREP_RECON_CHARGE_PAID;
			}
			else
			{
				nLineType = GENREP_RECON_CHARGE_LOYALTY;
			}
			break;

		case CASHRSP_ITEMPAYTYPE_SMARTPAY:
			nLineType = GENREP_RECON_CHARGE_SMARTPAY;
			break;

		case CASHRSP_ITEMPAYTYPE_SMARTPHONE:
			nLineType = GENREP_RECON_CHARGE_SMARTPHONE;
			break;

		case CASHRSP_ITEMPAYTYPE_SPTBOOK:	
			nLineType = GENREP_RECON_CHARGE_SPTBOOK;
			break;

		case CASHRSP_ITEMPAYTYPE_NORMAL:	
		default:							
			nLineType = GENREP_RECON_CHARGE_PAID;	
			break;
		}

		ConsolidateReconcile( nWeekday, nLineType, 1.0, dTender );				//RECONCILE PAYMENT
		ConsolidateReconcile( nWeekday, GENREP_RECON_BALANCE, 1.0, -dTender );	//RECONCILE BALANCE

		if (CASHRSP_ITEMPAYTYPE_NORMAL == nAccountType)
		{
			ConsolidatePayment(nWeekday, nPaymentNo, 1.0, dTender);
		}

		if ( 0 != nChange )
		{
			double dChange = ( (double) nChange ) / 100.0;

			ConsolidatePayment( nWeekday, nPaymentNoForChange, 0.0, -dChange ); 
			
			if (m_ReportSettings.GetSeparateChangeFlag() == TRUE)
			{
				ConsolidateReconcile(nWeekday, GENREP_RECON_CHANGE, 0.0, dChange);	//RECONCILE PAYMENT
			}
			else
			{
				ConsolidateReconcile(nWeekday, GENREP_RECON_CHARGE_PAID, 0.0, -dChange);	//RECONCILE CASHBACK
			}

			ConsolidateReconcile( nWeekday, GENREP_RECON_BALANCE, 0.0, dChange );	//RECONCILE BALANCE
		}
		
		if ( 0 != nCashback )
		{
			double dCashback = ( (double) nCashback ) / 100.0;

			ConsolidatePayment( nWeekday, 1, 0.0, -dCashback );
			
			if (m_ReportSettings.GetSeparateChangeFlag() == TRUE)
			{
				ConsolidateReconcile(nWeekday, GENREP_RECON_CASHBACK, 0.0, dCashback);		//RECONCILE CASHBACK
			}
			else
			{
				ConsolidateReconcile(nWeekday, GENREP_RECON_CHARGE_PAID, 0.0, -dCashback);	//RECONCILE CASHBACK
			}

			ConsolidateReconcile( nWeekday, GENREP_RECON_BALANCE, 0.0, dCashback );		//RECONCILE BALANCE
		}

		if ( 0 != nSurplus )
		{
			double dSurplus = ( (double) nSurplus ) / 100.0; 
			ConsolidateReconcile( nWeekday, GENREP_RECON_SURPLUS, 0.0, dSurplus );		//RECONCILE CASHBACK
			ConsolidateReconcile( nWeekday, GENREP_RECON_BALANCE, 0.0, dSurplus );		//RECONCILE BALANCE
		}

		if ( 0 != nGratuity )
		{
			double dGratuity = ( (double) nGratuity ) / 100.0;

			ConsolidateReconcile( nWeekday, GENREP_RECON_GRATUITY, 1.0, dGratuity );	//RECONCILE GRATUITY
			ConsolidateReconcile( nWeekday, GENREP_RECON_BALANCE, 0.0, dGratuity );		//RECONCILE BALANCE
		}
	}

	m_arrayPaymentBuffer.RemoveAll();
	m_arrayPaidIOBuffer.RemoveAll();
}

/**********************************************************************/

void CEposReportGeneral::ConsolidatePayment( int nWeekday, int nType, double dQty, double dValue )
{
	for ( int n = 0; n < m_arrayTerminalBlocks.GetSize(); n++ )
	{
		int nBlock = m_arrayTerminalBlocks.GetAt(n);

		CEposReportConsolGeneralMisc Payment;
		Payment.m_nLineType = GENREP_PAYMENT_LINE;
		Payment.m_nLineData = nType;
		Payment.m_dQty[ nWeekday ] = dQty;
		Payment.m_dValue[ nWeekday ] = dValue;
		Payment.m_dQty[ 7 ] = dQty;
		Payment.m_dValue[ 7 ] = dValue;
		m_arrayReportBlocks[ nBlock ].m_pPayment -> Consolidate( Payment );

		Payment.m_nLineType = GENREP_PAYMENT_TOTAL;
		Payment.m_nLineData = 0;
		m_arrayReportBlocks[ nBlock ].m_pPayment -> Consolidate( Payment );
	}
}

/**********************************************************************/

void CEposReportGeneral::ConsolidateVoid( int nWeekday, int nType, int nTotalType, double dQty, double dValue )
{
	for ( int n = 0; n < m_arrayTerminalBlocks.GetSize(); n++ )
	{
		int nBlock = m_arrayTerminalBlocks.GetAt(n);

		CEposReportConsolGeneralMisc Void;
		Void.m_nLineType = nType;  
		Void.m_nLineData = 0;
		Void.m_dQty[ nWeekday ] = dQty;
		Void.m_dValue[ nWeekday ] = dValue;
		Void.m_dQty[ 7 ] = dQty;
		Void.m_dValue[ 7 ] = dValue;
		m_arrayReportBlocks[ nBlock ].m_pVoid -> Consolidate( Void );

		Void.m_nLineType = nTotalType;
		m_arrayReportBlocks[ nBlock ].m_pVoid -> Consolidate( Void );
	}
}

/**********************************************************************/

void CEposReportGeneral::ConsolidateNoSale( CTermFileListInfo& infoFile, CCSVEposTermLine& csvIn )
{
	CString strDate = csvIn.GetNoSaleDate();
	CString strTime = csvIn.GetNoSaleTime();
								
	//FILTER FOR DATE AND TIME
	if ( strDate.GetLength() != 8 || strTime.GetLength() != 8 )
		return;
	
	CString strTranDate;
	strTranDate.Format ( "20%s%s%s",
		(const char*) strDate.Right(2),
		(const char*) strDate.Mid(3,2),
		(const char*) strDate.Left(2) );
			
	CString strTranTime;
	strTranTime.Format ( "%s%s%s",
		(const char*) strTime.Left(2),
		(const char*) strTime.Mid(3,2),
		(const char*) strTime.Right(2) );

	int nWeekday = 0;
	if ( GetWeekday( infoFile.m_nLocIdx, strTranDate, strTranTime, nWeekday ) == TRUE )
	{
		for ( int n = 0; n < m_arrayTerminalBlocks.GetSize(); n++ )
		{
			int nBlock = m_arrayTerminalBlocks.GetAt(n);

			CEposReportConsolGeneralMisc NoSale;
			NoSale.m_nLineType = 1;  
			NoSale.m_nLineData = 0;
			NoSale.m_dQty[ nWeekday ] = 1.0;
			NoSale.m_dValue[ nWeekday ] = 0;
			NoSale.m_dQty[ 7 ] = 1.0;
			NoSale.m_dValue[ 7 ] = 0;

			m_arrayReportBlocks[ nBlock ].m_pNoSale -> Consolidate( NoSale );
		}
		
		COleDateTime oleDate;
		::GetOleDateFromString( strTranDate, oleDate );

		m_SelectArray.ProcessStartDate ( infoFile.m_nSelectArrayIdx, oleDate );
		m_SelectArray.ProcessEndDate( infoFile.m_nSelectArrayIdx, oleDate );
	}
}

/**************************************************************************/

void CEposReportGeneral::ConsolidateReconcile( int nWeekday, int nType, double dQty, double dValue )
{
	for ( int n = 0; n < m_arrayTerminalBlocks.GetSize(); n++ )
	{
		int nBlock = m_arrayTerminalBlocks.GetAt(n);

		CEposReportConsolGeneralMisc Recon;
		Recon.m_nLineType = nType;
		Recon.m_nLineData = 0;
		Recon.m_dQty[ nWeekday ] = dQty;
		Recon.m_dValue[ nWeekday ] = dValue;
		Recon.m_dQty[ 7 ] = dQty;
		Recon.m_dValue[ 7 ] = dValue;
		m_arrayReportBlocks[ nBlock ].m_pRecon -> Consolidate( Recon );
	}
}

/**********************************************************************/

void CEposReportGeneral::Consolidate()
{
	m_BusinessDateInfo.Reset();
	InitialiseConsolidationBlocks();
	ConsolidateTransactions();
	ConsolidateSales();
}

/**********************************************************************/

void CEposReportGeneral::ResetDeptGroupLinks()
{
	CDeptGroupLinks info;
	info.m_nReportGroup = -1;
	info.m_nConsolGroup = -1;

	m_arrayDeptGroupLinks.RemoveAll();
	m_arrayDeptGroupLinks.InsertAt( 0, info, Dept::DeptNo.Max + 1 );
}

/**********************************************************************/

int CEposReportGeneral::GetReportGroupForDeptNo( int nDeptNo )
{
	int nNum = 0;
	if ( ( nDeptNo >= 1 ) && ( nDeptNo < m_arrayDeptGroupLinks.GetSize() ) )
	{
		nNum = m_arrayDeptGroupLinks[ nDeptNo ].m_nReportGroup;

		if ( nNum == -1 )
		{
			int nDeptIdx;
			if ( DataManager.Department.FindDeptByNumber( nDeptNo, nDeptIdx ) == TRUE )
			{
				CDepartmentCSVRecord Dept;
				DataManager.Department.GetAt( nDeptIdx, Dept );
				nNum = Dept.GetReportGroup();
				m_arrayDeptGroupLinks[ nDeptNo ].m_nReportGroup = nNum;
			}
			else
			{
				nNum = 0;
				m_arrayDeptGroupLinks[ nDeptNo ].m_nReportGroup = 0;
			}
		}
	}
	return nNum;
}

/**********************************************************************/

int CEposReportGeneral::GetConsolGroupForDeptNo( int nDeptNo )
{
	int nNum = 0;
	if ( ( nDeptNo >= 1 ) && ( nDeptNo < m_arrayDeptGroupLinks.GetSize() ) )
	{
		nNum = m_arrayDeptGroupLinks[ nDeptNo ].m_nConsolGroup;

		if ( nNum == -1 )
		{							
			int nRepGroupIdx;
			int nRepGroupNo = GetReportGroupForDeptNo( nDeptNo );
			
			if ( DataManager.ReportGroup.FindGroupByNumber( nRepGroupNo, nRepGroupIdx ) == TRUE )
			{
				CGroupCSVRecordReport Group;
				DataManager.ReportGroup.GetAt( nRepGroupIdx, Group );
				nNum = Group.GetParentGroupNo();
				m_arrayDeptGroupLinks[ nDeptNo ].m_nConsolGroup = nNum;
			}
			else
			{
				nNum = 0;
				m_arrayDeptGroupLinks[ nDeptNo ].m_nConsolGroup = 0;
			}
		}
	}
	return nNum;
}

/**********************************************************************/

void CEposReportGeneral::ConsolidateSales()
{
	bool bNoDiscountWithSales = ( ( DealerFlags.GetHoltsDiscountFlag() == TRUE ) || ( m_ReportSettings.GetSalesPromoFlag() == FALSE ) ); 

	int nTotalFileSize;
	CReportConsolidationArray<CTermFileListInfo> arrayFileInfo;
	CEposReportFileListMaker FileListMaker( m_bEODMode, m_bSaleTime, m_PMSModes );
	FileListMaker.GetFileList ( arrayFileInfo, HISTORYFILE_TYPE_SALES, m_SelectArray, TRUE, nTotalFileSize );

	int nProgress = 0;	
	StatusProgress.Lock( TRUE, "Consolidating Sales" );

	m_ReportFilters.ResetDbIdx();
	for ( int nFileIdx = 0; nFileIdx < arrayFileInfo.GetSize(); nFileIdx++ )
	{
		CTermFileListInfo infoFile;
		arrayFileInfo.GetAt( nFileIdx, infoFile );

		CString strCurrentTaxDate = infoFile.m_strDateTran;
		int nCurrentTaxDateCode = DataManager.HistoricalTaxRates.GetDateCode( strCurrentTaxDate );

		m_BlockMap.GetBlockListForTerminal( infoFile.m_nSelectArrayIdx, m_arrayTerminalBlocks );

		int nPMSStatus = m_PMSModes.GetPMSLocationStatus( infoFile );
		
		if ( 0 == nPMSStatus)
			continue;

		bool bIsPMSLocation = ( 1 == nPMSStatus );

		CSSFile fileSales;
		if ( fileSales.Open ( infoFile.m_strFilename, "rb", _SH_DENYNO ) == FALSE )
			continue;

		CString strThisSaleDate = "";
		CString strThisSaleTime = "";
		m_PMSModes.GetInitialSaleDateTime( bIsPMSLocation, infoFile, m_bSaleTime, strThisSaleDate, strThisSaleTime );
		int nTaxDateCode = DataManager.HistoricalTaxRates.GetDateCode( infoFile.m_strDateTran );
		CString strLastSaleDate = strThisSaleDate;
		CString strLastSaleTime = strThisSaleTime;

		int nWeekday = 0;
		bool bGotWeekday = FALSE;
		if ( ( TRUE == bIsPMSLocation ) && ( m_PMSModes.GetDateMode() == PMS_DATEMODE_ACCOUNT ) )
		{	
			bGotWeekday = GetWeekday( infoFile.m_nLocIdx, strThisSaleDate, strThisSaleTime, nWeekday );
			if ( FALSE == bGotWeekday )
				continue;
		}

		if ( m_ReportFilters.SetDatabase( infoFile.m_nDbIdx ) == TRUE )
			ResetDeptGroupLinks();

		CPluSalesLine csv( infoFile.IsLegacySalesFolder() );

		int nLinesToRead;
		CString strBuffer;
		if ( ::FindFirstPluLine( fileSales, strBuffer, nLinesToRead ) == TRUE )
		{
			COleDateTime oleDatePMS = COleDateTime::GetCurrentTime();
			bool bPMSInclude = FALSE;

			do
			{
				StatusProgress.SetPos ( nProgress + fileSales.Tell(), nTotalFileSize );

				//EXTRACT DATE AND TIME INFORMATION FROM #PMS_TIME LINE 
				if ( m_PMSModes.CheckPMSTimeLine( bIsPMSLocation, strBuffer, strThisSaleDate, strThisSaleTime, oleDatePMS, bPMSInclude ) == FALSE )
					continue;
			
				csv.ParseLine( strBuffer );

				//CHECK FOR RELEVANT LINE TYPE
				int nLineType = csv.GetLineType();
				switch( nLineType )
				{
				case CSVPLUDATA_PROMO_TAXABLE:
				case CSVPLUDATA_PROMO_NONTAXABLE:
				case CSVPLUDATA_MIXMATCH_NONTAXABLE:
				case CSVPLUDATA_PLU_SALE:
				case CSVPLUDATA_PLU_DISCOUNT:
				case CSVPLUDATA_PLU_MIXMATCH:
					break;

				case CSVPLUDATA_PLU_CHEAPESTFREE:
				default:
					continue;
				}

				//FILTER FOR WASTAGE
				if (csv.GetPluSaleWastageFlag() == TRUE)
				{
					continue;
				}

				if (csv.GetPluSaleVoidFlag() == TRUE)
				{
					continue;
				}

				//FILTER FOR POST TO ROOM
				if ( SysInfo.IsPMSSystem() == TRUE )
					if ( ( m_PMSModes.GetEposFlag() == FALSE ) && ( FALSE == bIsPMSLocation ) )
						if ( DataManager.Payment.IsPMSPaymentType( csv.GetPluPaymentNo() ) )
							continue;

				//CHECK SALE DATE AND TIME
				if ( CheckPluSaleDateTime( bIsPMSLocation, infoFile, csv, oleDatePMS, strThisSaleDate, strThisSaleTime ) == FALSE )
					continue;

				if ( ( TRUE == bIsPMSLocation ) && ( strThisSaleDate != strLastSaleDate ) )
				{
					nTaxDateCode = DataManager.HistoricalTaxRates.GetDateCode( strThisSaleDate );
					bGotWeekday = GetWeekday( infoFile.m_nLocIdx, strThisSaleDate, strThisSaleTime, nWeekday );
					strLastSaleDate = strThisSaleDate;
					strLastSaleTime = strThisSaleTime;
				}
				else if ( strThisSaleTime != strLastSaleTime )
				{
					bGotWeekday = GetWeekday( infoFile.m_nLocIdx, strThisSaleDate, strThisSaleTime, nWeekday );
					strLastSaleTime = strThisSaleTime;
				}

				if ( FALSE == bGotWeekday )
					continue;

				__int64 nPluNo = 0;
				double dMultiplier = 1.0;
				int nModifier = 0;
				
				switch( nLineType )
				{
				case CSVPLUDATA_PROMO_TAXABLE:
				case CSVPLUDATA_PROMO_NONTAXABLE:
					nPluNo = GetSeparatedPromoPluNo( csv.GetPromoNoSeparate() );
					break;

				case CSVPLUDATA_MIXMATCH_NONTAXABLE:
					nPluNo = GetSeparatedMixMatchPluNo( csv.GetMixMatchNo() );
					break;

				case CSVPLUDATA_PLU_SALE:
				case CSVPLUDATA_PLU_DISCOUNT:
				case CSVPLUDATA_PLU_MIXMATCH:
					{
						CSQLPluNoInfo infoPluNo;
						infoPluNo.m_nEposPluNo = csv.GetPluNoNew();
						::ProcessPluNo( infoPluNo, FALSE, TRUE );

						if ( FALSE == infoPluNo.m_bValid )
							continue;

						if ( infoPluNo.m_nModifier != 0 )
							dMultiplier = DataManager.Modifier.GetMultiplierForQty ( infoPluNo.m_nModifier );

						nPluNo = infoPluNo.m_nBasePluNo;
						nModifier = infoPluNo.m_nModifier;
					}
					break;
				}

				double dQty = csv.GetSaleQty();
				double dValue = csv.GetValue();

				bool bPositivePromo = FALSE;

				//SALES BLOCK
				switch ( nLineType )
				{
				case CSVPLUDATA_PLU_DISCOUNT:
				case CSVPLUDATA_PLU_MIXMATCH:
					dQty = 0.0;
					dValue *= -1;
					break;

				case CSVPLUDATA_PROMO_TAXABLE:
				case CSVPLUDATA_PROMO_NONTAXABLE:
				case CSVPLUDATA_MIXMATCH_NONTAXABLE:
					dValue *= -1;
					break;

				case CSVPLUDATA_PLU_SALE:
					dQty *= dMultiplier;
					break;
				}

				int nDeptNo = csv.GetDeptNo();
				if ( m_ReportFilters.CheckDepartment( nDeptNo ) == FALSE )
				{
					ConsolidateReconcile( nWeekday, GENREP_RECON_SALE_FILTERED, dQty, dValue );
					ConsolidateReconcile( nWeekday, GENREP_RECON_BALANCE, dQty, dValue );
					continue;
				}

				CArray<int,int> arrayConsolLevels;
				for ( int n = 0; n < m_arraySalesLevels.GetSize(); n++ )
				{
					int nNum = 0;

					switch( m_arraySalesLevels.GetAt(n) )
					{
					case NODE_DEPARTMENT:
						nNum = nDeptNo;
						break;

					case NODE_DEPT_GROUP_EPOS:
						{
							nNum = csv.GetGroupNo() - 1;

							if ( nNum < 0 )
								nNum = 0;
						}
						break;

					case NODE_DEPT_GROUP_REPORT:
						nNum = GetReportGroupForDeptNo( nDeptNo );
						break;

					case NODE_DEPT_GROUP_CONSOL:
						nNum = GetConsolGroupForDeptNo( nDeptNo );
						break;
					}

					arrayConsolLevels.Add( nNum );
				}

				int nTaxBand = 0;
				if ((CSVPLUDATA_PROMO_NONTAXABLE != nLineType) && (CSVPLUDATA_MIXMATCH_NONTAXABLE != nLineType))
				{
					if (DealerFlags.GetUseDatabaseVATBandFlag() == FALSE)
					{
						nTaxBand = csv.GetNumericTaxBand();
					}
				}

				if ((CSVPLUDATA_PLU_SALE == nLineType) || (FALSE == bNoDiscountWithSales))
				{
					ConsolidateItemSale(nWeekday, arrayConsolLevels, nPluNo, nModifier, nCurrentTaxDateCode, nTaxBand, dQty, dValue);
				}
				else
				{
					ConsolidateReconcile( nWeekday, GENREP_RECON_HOLTS_PROMO, dQty, dValue * -1 );
					ConsolidateReconcile( nWeekday, GENREP_RECON_BALANCE, dQty, dValue );
				}

				if ( m_ReportSettings.GetPromoPositiveFlag() == TRUE )
					dValue *= -1;

				//PROMO BLOCK
				switch ( nLineType )
				{
				case CSVPLUDATA_PLU_DISCOUNT:
					{
						int nPromoNo = csv.GetPromoNoForPlu();
						arrayConsolLevels.InsertAt( 0, nPromoNo  );
						int nDiscountMode = ( csv.GetDiscountMode() == DISCOUNT_MODE_ITEM ) ? GENREP_PROMO_LINE_ITEM : GENREP_PROMO_LINE_SUBT;
						ConsolidateItemPromo( nWeekday, arrayConsolLevels, nPromoNo, nDiscountMode, dValue );
					}
					break;

				case CSVPLUDATA_PROMO_TAXABLE:
				case CSVPLUDATA_PROMO_NONTAXABLE:
					{
						int nPromoNo = csv.GetPromoNoSeparate();
						arrayConsolLevels.InsertAt( 0, nPromoNo  );
						int nDiscountMode = ( csv.GetDiscountMode() == DISCOUNT_MODE_ITEM ) ? GENREP_PROMO_LINE_ITEM : GENREP_PROMO_LINE_SUBT;
						ConsolidateItemPromo( nWeekday, arrayConsolLevels, nPromoNo, nDiscountMode, dValue );
					}
					break;

				case CSVPLUDATA_PLU_MIXMATCH:
				case CSVPLUDATA_MIXMATCH_NONTAXABLE:
					{
						int nMMLabel = ( csv.GetMixMatchType() * 10000 ) + csv.GetMixMatchNo();
						arrayConsolLevels.InsertAt( 0, nMMLabel );
						ConsolidateItemPromo( nWeekday, arrayConsolLevels, nMMLabel, GENREP_PROMO_LINE_MIXMATCH, dValue );
					}
					break;
				}
			}
			while ( ( fileSales.ReadString( strBuffer ) == TRUE ) && ( --nLinesToRead != 0 ) );
		}

		nProgress += fileSales.GetLength();
		fileSales.Close();
	}

	StatusProgress.Unlock();
}

/**********************************************************************/

void CEposReportGeneral::ConsolidateTransactions()
{
	int nTotalFileSize;
	CReportConsolidationArray<CTermFileListInfo> arrayFileInfo;
	CEposReportFileListMaker FileListMaker( m_bEODMode, m_bSaleTime, m_PMSModes );
	FileListMaker.GetTermFileList ( arrayFileInfo, m_SelectArray, TRUE, nTotalFileSize );

	int nProgress = 0;	
	StatusProgress.Lock( TRUE, "Consolidating payments" );
	
	bool bInclude = FALSE;
	m_arrayPaymentBuffer.RemoveAll();
	m_arrayPaidIOBuffer.RemoveAll();

	m_ReportFilters.ResetDbIdx();
	for ( int nFileIdx = 0; ( nFileIdx < arrayFileInfo.GetSize() ) && ( m_CashRSPVersionChecker.GetCashRSPFormatFailFlag() == FALSE ); nFileIdx++ )
	{
		CTermFileListInfo infoFile;
		arrayFileInfo.GetAt( nFileIdx, infoFile );

		m_BlockMap.GetBlockListForTerminal( infoFile.m_nSelectArrayIdx, m_arrayTerminalBlocks );

		int nPMSStatus = m_PMSModes.GetPMSLocationStatus( infoFile );
		
		if ( 0 == nPMSStatus)
			continue;

		bool bIsPMSLocation = ( 1 == nPMSStatus );

		int nPMSDateType = 1;
				
		CSSFile fileSales;
		if ( fileSales.Open ( infoFile.m_strFilename, "rb", _SH_DENYNO ) == FALSE )
			continue;

		int nConnectionType = CONNECTION_TYPE_STANDARD_NONE;
		{
			int nDummy = 0;
			dbLocation.GetLocSPOSVersion( infoFile.m_nLocIdx, nDummy, nConnectionType );
		}

		int nWeekday = 999;
		
		m_ReportFilters.SetDatabase( infoFile.m_nDbIdx );

		int nLinesToRead;
		CString strBuffer;
		bool bIsVoidTran = FALSE;
		if ( ::FindFirstTermLine( fileSales, strBuffer, nLinesToRead ) == TRUE )
		{
			m_CashRSPVersionChecker.ResetTransactionVersions();
			
			do
			{
				StatusProgress.SetPos ( nProgress + fileSales.Tell(), nTotalFileSize );

				CCSVEposTermLine csvIn ( strBuffer );

				int nCashRSPLineType = csvIn.GetLineTypeInt();

				if ( m_PMSModes.CheckPMSTransactionType( bIsPMSLocation, nCashRSPLineType ) == FALSE )
					continue;

				switch( nCashRSPLineType )
				{
				case CASHRSP_LINETYPE_TRAN_NORMAL:
				case CASHRSP_LINETYPE_TRAN_VOID:
					{
						ConsolidatePaymentBuffer( nWeekday );
						m_CashRSPVersionChecker.ApplyPendingTransactionInfo();
							
						bInclude = FALSE;

						if ( ( TRUE == bIsPMSLocation ) && ( m_PMSModes.GetDateMode() == PMS_DATEMODE_ACCOUNT ) )
						{
							CString strCheckTime = "000000";
							if ( GetWeekday ( infoFile.m_nLocIdx, infoFile.m_strDateTran, strCheckTime, nWeekday ) == TRUE )
							{
								bInclude = TRUE;
								bIsVoidTran = ( CASHRSP_LINETYPE_TRAN_VOID == nCashRSPLineType );
								COleDateTime oleDate;
								::GetOleDateFromString( infoFile.m_strDateTran, oleDate );
								m_SelectArray.ProcessStartDate ( infoFile.m_nSelectArrayIdx, oleDate );
								m_SelectArray.ProcessEndDate( infoFile.m_nSelectArrayIdx, oleDate );
							}
						}
						else
						{
							CString strDate = csvIn.GetTransactionDate();
							CString strTime = csvIn.GetTransactionTime();

							if ( strDate.GetLength() != 8 || strTime.GetLength() != 8 )
								continue;
						
							CString strCheckDate;
							strCheckDate.Format ( "20%s%s%s",
								(const char*) strDate.Right(2),
								(const char*) strDate.Mid(3,2),
								(const char*) strDate.Left(2) );

							CString strCheckTime;
							strCheckTime.Format ( "%s%s%s",
								(const char*) strTime.Left(2),
								(const char*) strTime.Mid(3,2),
								(const char*) strTime.Right(2) );
						
							if ( GetWeekday ( infoFile.m_nLocIdx, strCheckDate, strCheckTime, nWeekday ) == TRUE )
							{
								bInclude = TRUE;
								bIsVoidTran = ( CASHRSP_LINETYPE_TRAN_VOID == nCashRSPLineType );
								COleDateTime oleDate;
								::GetOleDateFromString( strCheckDate, oleDate );
								m_SelectArray.ProcessStartDate ( infoFile.m_nSelectArrayIdx, oleDate );
								m_SelectArray.ProcessEndDate( infoFile.m_nSelectArrayIdx, oleDate );
							}
						}
					}
					break;

				case CASHRSP_LINETYPE_EODHEADER:
					{
						ConsolidatePaymentBuffer( nWeekday );
						m_CashRSPVersionChecker.ApplyPendingTransactionInfo();

						bInclude = FALSE;

						switch( strBuffer.GetLength() )
						{
						case 19:
						case 28:
						case 39:
							{
								CString strCheckDate = strBuffer.Mid(4,8);
								CString strCheckTime = strBuffer.Mid(13,6);

								if ( ::TestNumeric( strCheckDate ) && ::TestNumeric( strCheckTime) )
								{
									if ( GetWeekday ( infoFile.m_nLocIdx, strCheckDate, strCheckTime, nWeekday ) == TRUE )
									{
										bInclude = TRUE;
										bIsVoidTran = FALSE;
										COleDateTime oleDate;
										::GetOleDateFromString( strCheckDate, oleDate );
										m_SelectArray.ProcessStartDate ( infoFile.m_nSelectArrayIdx, oleDate );
										m_SelectArray.ProcessEndDate( infoFile.m_nSelectArrayIdx, oleDate );
									}
								}
							}
							break;
						}
					}
					break;

				case CASHRSP_LINETYPE_WASTAGE:
				case CASHRSP_LINETYPE_AGEVERIFY:
				case CASHRSP_LINETYPE_NOSALE:
					ConsolidatePaymentBuffer( nWeekday );
					m_CashRSPVersionChecker.ApplyPendingTransactionInfo();
					bInclude = FALSE;	
					break;

				case CASHRSP_LINETYPE_ITEM:
					if ( ( TRUE == bInclude ) && ( FALSE == bIsVoidTran ) )
					{
						int nType, nDummy32;
						__int64 nDummy64;
						csvIn.GetItemTypeAndID( m_CashRSPVersionChecker, nType, nDummy32, nDummy64 );

						CString strVoid = csvIn.GetItemVoidFlag();
						
						switch( nType )
						{
						case CASHRSP_ITEMPAYTYPE_CUSTOMER:
						case CASHRSP_ITEMPAYTYPE_ROOM:
						case CASHRSP_ITEMPAYTYPE_LOYALTY:
						case CASHRSP_ITEMPAYTYPE_SMARTPAY:
						case CASHRSP_ITEMPAYTYPE_SMARTPHONE:
						case CASHRSP_ITEMPAYTYPE_SPTBOOK:
							if ( strVoid != "V" )	
								ConsolidateSpecialItem( nWeekday, nType, 1.0, csvIn.GetItemLineValueReport(), FALSE );
							break;

						case CASHRSP_ITEMPAYTYPE_DEPOSIT:
							if ( strVoid != "V" )	
							{
								CString strRefund = csvIn.GetItemRefundFlag();
								ConsolidateSpecialItem( nWeekday, nType, 1.0, csvIn.GetItemLineValueReport(), strRefund == "R" );
							}
							break;
						}
					}
					break;

				case CASHRSP_LINETYPE_PAYMENT:
				case CASHRSP_LINETYPE_DEPOSIT_PMS:
					if ( ( TRUE == bInclude ) && ( FALSE == bIsVoidTran ) )
					{
						int nPaymentNo = csvIn.GetPaymentNumber();

						switch( nConnectionType )
						{
						case CONNECTION_TYPE_SPTBOOK_NONE:
							csvIn.SetPaymentNumber( DataManager.SBPaymentMap.GetSharpPosPaymentNo( nPaymentNo ) );
							strBuffer = csvIn.GetLine();
							break;

						case CONNECTION_TYPE_SMARTENT_NONE:
							csvIn.SetPaymentNumber( DataManager.SEPaymentMap.GetSharpPosPaymentNo( nPaymentNo ) );
							strBuffer = csvIn.GetLine();
							break;

						case CONNECTION_TYPE_STANDARD_NONE:
						default:
							{
								if ( SysInfo.IsPMSSystem() == TRUE )
									if ( ( m_PMSModes.GetEposFlag() == FALSE ) && ( FALSE == bIsPMSLocation ) )
										if ( DataManager.Payment.IsPMSPaymentType( nPaymentNo ) == TRUE )
											bInclude = FALSE;
							}
							break;
						}
						
						if ( TRUE == bInclude )
							m_arrayPaymentBuffer.Add( strBuffer );
					}
					break;

				case CASHRSP_LINETYPE_PAIDIO:
					if ( ( TRUE == bInclude ) && ( m_ReportSettings.GetReconciliationFlag() == TRUE ) && ( FALSE == bIsVoidTran ) )
						m_arrayPaidIOBuffer.Add( strBuffer );
					break;

				case CASHRSP_LINETYPE_EODVAR:
					if ( TRUE == bInclude )
					{
						double dVariance = ( (double) csvIn.GetInt(3) ) / 100.0;
						ConsolidateVariance( nWeekday, FALSE, FALSE, csvIn.GetInt(1), csvIn.GetString(2), dVariance );
					}
					break;

				case CASHRSP_LINETYPE_EODVARTOT:
					if ( TRUE == bInclude )
					{
						double dVariance = ( (double) csvIn.GetInt(3) ) / 100.0;
						ConsolidateVariance( nWeekday, FALSE, TRUE, 0, "", dVariance );
					}
					break;

				case CASHRSP_LINETYPE_EODVARFX:
					if ( TRUE == bInclude )
					{
						double dVariance = ( (double) csvIn.GetInt(3) ) / 100.0;
						ConsolidateVariance( nWeekday, TRUE, FALSE, csvIn.GetInt(1), csvIn.GetString(2), dVariance );
					}
					break;

				case CASHRSP_LINETYPE_EODVARFXTOT:
					if ( TRUE == bInclude )
					{
						double dVariance = ( (double) csvIn.GetInt(3) ) / 100.0;
						ConsolidateVariance( nWeekday, TRUE, TRUE, 0, "", dVariance );
					}
					break;
				}

				//EXTRA CONSOLIDATION FOR ACTIVITY REPORTS
				if ( ( CASHRSP_LINETYPE_NOSALE == nCashRSPLineType ) && ( FALSE == bIsVoidTran ) )
				{
					if (m_ReportSettings.GetNoSaleFlag() == TRUE)
					{
						ConsolidateNoSale(infoFile, csvIn);
					}
				}
				else if ( TRUE == bInclude )
				{
					switch( nCashRSPLineType )
					{
					case CASHRSP_LINETYPE_ITEM:
						if ( m_ReportSettings.GetVoidFlag() == TRUE )
						{
							int nType, nDummy32;
							__int64 nDummy64;
							csvIn.GetItemTypeAndID( m_CashRSPVersionChecker, nType, nDummy32, nDummy64 );

							CString strVoid = csvIn.GetItemVoidFlag();

							if ( ( strVoid == "V" ) || ( TRUE == bIsVoidTran ) )
							{
								switch( nType )
								{
								case CASHRSP_ITEMPAYTYPE_DEPOSIT:
								case CASHRSP_ITEMPAYTYPE_CUSTOMER:
								case CASHRSP_ITEMPAYTYPE_ROOM:
								case CASHRSP_ITEMPAYTYPE_LOYALTY:
								case CASHRSP_ITEMPAYTYPE_SMARTPAY:
								case CASHRSP_ITEMPAYTYPE_SMARTPHONE:
								case CASHRSP_ITEMPAYTYPE_SPTBOOK:
									break;

								default:
									{
										bool bAllowVoid = TRUE;

										int nDeptNo = csvIn.GetItemDeptNo();
										if (m_ReportFilters.CheckDepartment(nDeptNo) == FALSE)
										{
											bAllowVoid = FALSE;
										}

										double dQty = 0.0;

										if ( TRUE == bAllowVoid )
										{
											switch( m_ReportSettings.GetVoidQtyType() )
											{
											case 1:
												{
													CSQLPluNoInfo infoPluNo;
													infoPluNo.m_nEposPluNo = csvIn.GetItemPluNo();
													::ProcessPluNo( infoPluNo, FALSE, TRUE );
													
													if (FALSE == infoPluNo.m_bValid)
													{
														bAllowVoid = FALSE;
													}
													else
													{
														dQty = csvIn.GetItemQty();

														if (infoPluNo.m_nModifier != 0)
														{
															dQty *= DataManager.Modifier.GetMultiplierForQty(infoPluNo.m_nModifier);
														}
													}
												}
												break;

											case 2:
												dQty = 1.0;
												break;

											case 0:
											default:
												dQty = csvIn.GetItemQty();
												break;
											}
										}

										if ( TRUE == bAllowVoid )
										{
											CString strVoidBAType = csvIn.GetItemVoidBAType( m_CashRSPVersionChecker.GetCurrentTransactionVersionNo() );
										
											int nLineType = GENREP_VOID_SALE_LEGACY;
											int nTotalType = GENREP_VOID_SALE_TOTAL;
										
											if (TRUE == bIsVoidTran)
											{
												nLineType = GENREP_VOID_SALE_TRAN;
											}
											else if (strVoidBAType == "B")
											{
												nLineType = GENREP_VOID_SALE_BEFORE;
											}
											else if (strVoidBAType == "A")
											{
												nLineType = GENREP_VOID_SALE_AFTER;
											}

											double dValue = csvIn.GetItemLineValueReport();

											CString strRefund = csvIn.GetItemRefundFlag();
											if ( strRefund == "R" )
											{
												switch( nLineType )
												{
												case GENREP_VOID_SALE_LEGACY:	nLineType = GENREP_VOID_REFUND_LEGACY;
												case GENREP_VOID_SALE_TRAN:		nLineType = GENREP_VOID_REFUND_TRAN;
												case GENREP_VOID_SALE_BEFORE:	nLineType = GENREP_VOID_REFUND_BEFORE;
												case GENREP_VOID_SALE_AFTER:	nLineType = GENREP_VOID_REFUND_AFTER;
												}
												nTotalType = GENREP_VOID_REFUND_TOTAL;
												dValue *= -1;
											}
									
											ConsolidateVoid( nWeekday, nLineType, nTotalType, dQty, dValue );
										}
									}
									break;
								}
							}
						}
						break;
					}
				}
				
				switch( nCashRSPLineType )
				{
				case CASHRSP_LINETYPE_VERSION:
					m_CashRSPVersionChecker.ProcessCashRSPVersionLine( csvIn );
					break;

				case CASHRSP_LINETYPE_APPLICATION:
					m_CashRSPVersionChecker.ProcessAppTypeLine( csvIn );
					break;

				default:
					m_CashRSPVersionChecker.ResetPendingTransactionInfo();
					break;
				}
			}
			while ( ( fileSales.ReadString( strBuffer ) == TRUE ) && ( --nLinesToRead != 0 ) && ( m_CashRSPVersionChecker.GetCashRSPFormatFailFlag() == FALSE ) );
		}
		
		ConsolidatePaymentBuffer( nWeekday );
					
		nProgress += fileSales.GetLength();
		fileSales.Close();
	}
	StatusProgress.Unlock();
}

/**********************************************************************/

bool CEposReportGeneral::GetWeekday( int nLocIdx, CString& strTranDate, CString& strTranTime, int& nWeekday )
{
	nWeekday = 0;
	bool bResult = FALSE;
	
	if ( m_BusinessDateInfo.ValidateBusinessDate( nLocIdx, m_bEODMode, strTranDate, strTranTime ) == TRUE )
	{
		if (m_ReportSettings.GetWeekdayFlag() == TRUE)
		{
			nWeekday = m_BusinessDateInfo.GetWeekday();
		}

		bResult = TRUE;
	}

	return bResult;
}

/**********************************************************************/

bool CEposReportGeneral::CreateReport()
{
	if ( m_ReportFile.Open ( Super.ReportFilename() ) == FALSE )
		return FALSE;
	
	m_ReportFile.SetStyle1 ( GetReportTitle(FALSE) );
	m_ReportFile.AddColumn ( "No", TA_LEFT, 200 );
	m_ReportFile.AddColumn ( "Description", TA_LEFT, 550 );

	if ( m_ReportSettings.GetWeekdayFlag() == TRUE )
	{
		if ( m_ReportSettings.GetQuantityFlag() == TRUE )
		{
			int nDay = EcrmanOptions.GetReportsWeekStartDay();
			if ( nDay < 0 || nDay > 6 ) nDay = 0;

			for ( int n = 0; n < 7; n++ )
			{
				CString strDay = m_strWeekDays [ (nDay++) % 7 ];
				m_ReportFile.AddColumn ( strDay + "Qty", TA_RIGHT, 200 );
				m_ReportFile.AddColumn ( strDay + "Val", TA_RIGHT, 200 );
			}

			m_ReportFile.AddColumn( "Total Qty", TA_RIGHT, 200 );
			m_ReportFile.AddColumn( "Total Val", TA_RIGHT, 200 );
		}
		else
		{
			int nDay = EcrmanOptions.GetReportsWeekStartDay();
			
			if (nDay < 0 || nDay > 6)
			{
				nDay = 0;
			}

			for (int n = 0; n < 7; n++)
			{
				m_ReportFile.AddColumn(m_strWeekDays[(nDay++) % 7], TA_RIGHT, 200);
			}

			m_ReportFile.AddColumn( "Total", TA_RIGHT, 200 );
		}
	}
	else
	{
		if ( m_ReportSettings.GetQuantityFlag() == TRUE )
		{
			m_ReportFile.AddColumn ( "Qty", TA_RIGHT, 250 );
			m_ReportFile.AddColumn ( "Value", TA_RIGHT, 250 );
		}
		else
		{
			m_ReportFile.AddColumn ( "Value", TA_RIGHT, 250 );
		}
	}

	m_nCreateReportCount = 0;
	m_nCreateReportTarget = 0;
	m_nCreateReportMiniCount = 0;
	m_nCreateReportMiniTarget = 0;
	for ( int n = 0; n < m_arrayReportBlocks.GetSize(); n++ )
	{
		CEposReportGeneralBlock ReportBlock = m_arrayReportBlocks.GetAt( n );
		m_nCreateReportTarget += ReportBlock.GetLineCount( m_ReportSettings );		
	}

	m_nCreateReportMiniTarget = m_nCreateReportTarget / 100;
	if (m_nCreateReportMiniTarget < 1)
	{
		m_nCreateReportMiniTarget = 1;
	}

	StatusProgress.Lock( TRUE, "Creating report" );

	m_BlockMap.BuildSortArray();
	for ( int n = 0; n < m_BlockMap.GetSortArraySize(); n++ )
	{
		int nIdx = m_BlockMap.GetSortArrayIdx(n);
		CEposReportGeneralBlock ReportBlock = m_arrayReportBlocks.GetAt( nIdx );

		if (ReportBlock.GotData() == FALSE)
		{
			continue;
		}

		m_ReportFile.HandleNewPage();

		if ( FALSE == m_bCanConsolidateGlobally )
		{
			CDataManagerInfo info;
			DataManager.OpenDatabaseReadOnly( ReportBlock.m_nDbIdx, info, FALSE );
		}

		CString strHeader = "<..>";
		switch( ReportBlock.m_nEntityType )
		{
		case NODE_LOCATION_TERMINAL:

			if (dbLocation.IsPMSPseudoLocation(ReportBlock.m_nLocIdx) == TRUE)
			{
				strHeader += "Guest Accounts";
			}
			else
			{
				strHeader += dbLocation.GetName( ReportBlock.m_nLocIdx );
				strHeader += ", ";
				strHeader += dbLocation.GetTerminalName( ReportBlock.m_nLocIdx, ReportBlock.m_nTermIdx );
			}
			break;

		case NODE_LOCATION:
			if (dbLocation.IsPMSPseudoLocation(ReportBlock.m_nLocIdx) == TRUE)
			{
				strHeader += "Guest Accounts";
			}
			else
			{
				strHeader += dbLocation.GetName(ReportBlock.m_nLocIdx);
			}
			break;

		case NODE_REPORTPOINT:
			strHeader += dbReportpoint.GetName( ReportBlock.m_nRpIdx );
			break;
		
		case NODE_DATABASE:
			strHeader += dbDatabase.GetName( ReportBlock.m_nDbIdx );
			break;

		case NODE_SYSTEM:
			strHeader += "System";
			break;
		}
		
		if ( ( ReportBlock.m_pSales -> GetSize() > 0 ) || ( ReportBlock.m_pPayment -> GetSize() > 0 ) )
		{
			if ( ReportBlock.m_pSales -> GetSize() > 0 )
			{
				m_ReportFile.RequestBlankLines(2);
				m_ReportFile.WriteReportMiscLine( strHeader + ", Sales" );
				m_ReportFile.WriteReportMiscLine( "<LI>" );
				m_ReportFile.LockBlankLines( ( m_arraySalesLevels.GetSize() >= 2 ) ? 1 : 0 );
				CreateSalesSection( ReportBlock, FALSE );
			}

			if ( m_ReportSettings.GetTaxBandFlag() == TRUE )
			{
				m_ReportFile.RequestBlankLines(2);
				CreateTaxSection( ReportBlock, FALSE );
			}

			if ( m_ReportSettings.GetTaxAmountFlag() == TRUE )
			{
				m_ReportFile.RequestBlankLines(2);
				CreateTaxSection( ReportBlock, TRUE );
			}

			m_ReportFile.RequestBlankLines(2);
			m_ReportFile.WriteReportMiscLine( strHeader + ", Payments" );
			m_ReportFile.WriteReportMiscLine( "<LI>" );
			m_ReportFile.LockBlankLines(0);
			CreatePaymentSection( ReportBlock );
		}

		if ( m_ReportSettings.GetCCDVarianceFlag() == TRUE )
		{
			if ( ReportBlock.m_pVariance -> GetSize() > 1 )
			{
				m_ReportFile.RequestBlankLines(2);
				m_ReportFile.WriteReportMiscLine( strHeader + ", Variance" );
				m_ReportFile.WriteReportMiscLine( "<LI>" );
				m_ReportFile.LockBlankLines(0);
				CreateVarianceSection( ReportBlock, FALSE );
			}
			if ( ReportBlock.m_pVarianceForEx -> GetSize() > 1 )
			{
				m_ReportFile.RequestBlankLines(2);
				m_ReportFile.WriteReportMiscLine( strHeader + ", ForEx Variance" );
				m_ReportFile.WriteReportMiscLine( "<LI>" );
				m_ReportFile.LockBlankLines(0);
				CreateVarianceSection( ReportBlock, TRUE );
			}
		}

		if ( ( ReportBlock.m_pPromoDetail -> GetSize() > 0 ) && ( m_ReportSettings.GetPromoDetailFlag() == TRUE ) )
		{
			m_ReportFile.RequestBlankLines(2);
			m_ReportFile.WriteReportMiscLine( strHeader + ", Promo Detail" );
			m_ReportFile.WriteReportMiscLine( "<LI>" );
			m_ReportFile.LockBlankLines( 1 );
			CreateSalesSection( ReportBlock, TRUE );
		}

		if (m_ReportSettings.GetPromoSummaryFlag() == TRUE)
		{
			CreatePromoSection(ReportBlock, strHeader);
		}

		if (m_ReportSettings.GetReconciliationFlag() == TRUE)
		{
			CreateReconciliationSection(ReportBlock, strHeader);
		}

		bool bVoid = ( ( m_ReportSettings.GetVoidFlag() ) && ( ReportBlock.m_pVoid -> GetSize() > 0 ) );
		bool bNoSale = ( ( m_ReportSettings.GetNoSaleFlag() ) && ( ReportBlock.m_pNoSale -> GetSize() > 0 ) );

		if ( ( TRUE == bVoid ) || ( TRUE == bNoSale ) )
		{
			m_ReportFile.RequestBlankLines(2);

			if (FALSE == bVoid)
			{
				m_ReportFile.WriteReportMiscLine(strHeader + ", No Sales");
			}
			else if (FALSE == bNoSale)
			{
				m_ReportFile.WriteReportMiscLine(strHeader + ", Voids");
			}
			else
			{
				m_ReportFile.WriteReportMiscLine(strHeader + ", Voids & No Sales");
			}

			m_ReportFile.WriteReportMiscLine( "<LI>" );
			
			if (TRUE == bVoid)
			{
				CreateVoidSection(ReportBlock);
			}

			if (TRUE == bNoSale)
			{
				CreateNoSaleSection(ReportBlock);
			}
		}
	}

	StatusProgress.Unlock();
	ShowEposDetails();
	m_ReportFile.Close();
	
	return TRUE;	
}

/**********************************************************************/

void CEposReportGeneral::CreateSalesSection( CEposReportGeneralBlock& Block, bool bPromo )
{
	m_bCreatingPromoSection = bPromo;

	CString strBlockNum = "";	
	if ( FALSE == bPromo )
	{
		m_pSalesOrPromoSection = Block.m_pSales;
		m_pSalesOrPromoLevels = &m_arraySalesLevels;
		strBlockNum = "Sales";
	}
	else
	{
		m_pSalesOrPromoSection = Block.m_pPromoDetail;
		m_pSalesOrPromoLevels = &m_arrayPromoLevels;
		strBlockNum = "Promo";
	}

	m_arraySalesHeaders.RemoveAll();
	m_arraySalesTotals.RemoveAll();

	CEntityTreeGeneralSalesNode GrandTotalNode;
	m_pSalesOrPromoSection -> GetGrandTotalNode( GrandTotalNode );
	
	CGeneralReportSalesBlockEntity GrandTotalEntity;
	GrandTotalEntity.m_nEntityType = NODE_PLACEHOLDER;
	GrandTotalEntity.m_nEntityNum = 0;
	ProcessSalesTreeBranch( 0, 0, GrandTotalNode, GrandTotalEntity );
}

/**********************************************************************/

void CEposReportGeneral::ProcessSalesTreeBranch( int nDepth, int nBranchIdx, CEntityTreeGeneralSalesNode& BlockTotalNode, CGeneralReportSalesBlockEntity& BlockEntity )
{
	int nBranchSize = m_pSalesOrPromoSection -> GetBranchSize(nBranchIdx);
	if ( nBranchSize == 0 )
		return;

	GetGroupNumberAndName( BlockEntity );
	m_arraySalesHeaders.Add( BlockEntity );

	CGeneralReportSalesBlockEntity ItemEntity;
	ItemEntity.m_nEntityType = m_pSalesOrPromoLevels -> GetAt( nDepth );
	
	for ( int nLeafIdx = 0; nLeafIdx < nBranchSize; nLeafIdx++ )
	{
		UpdateCreateReportProgress();

		CEntityTreeGeneralSalesNode CurrentNode;
		m_pSalesOrPromoSection -> GetNode( nBranchIdx, nLeafIdx, CurrentNode );
		
		int nLeafData = m_pSalesOrPromoSection -> GetLabelInt( nBranchIdx, nLeafIdx );
		if (1000 == nLeafData)
		{
			nLeafData = 0;
		}

		ItemEntity.m_nEntityNum = nLeafData;
		GetGroupNumberAndName( ItemEntity );
			
		if ( nDepth == m_pSalesOrPromoLevels -> GetSize() - 1 )
		{	
			if ( nLeafIdx == 0 )
			{
				m_ReportFile.RequestBlankLines(2);

				int nSize = m_arraySalesHeaders.GetSize();
				
				for (int n = 1; n < nSize - 1; n++)
				{
					m_ReportFile.WriteReportMiscLine(m_arraySalesHeaders[n].m_strHeader);
				}

				if ( nSize > 1 )
				{
					m_ReportFile.WriteReportMiscLine( m_arraySalesHeaders[nSize - 1].m_strHeader );

					if (m_arraySalesHeaders[nSize - 1].m_strMixMatchType != "")
					{
						m_ReportFile.WriteReportMiscLine(m_arraySalesHeaders[nSize - 1].m_strMixMatchType);
					}

					m_ReportFile.WriteReportMiscLine( "<LI>" );
				}
			}

			CCSV csv( '\t' );
			AppendLineTotals( csv, CurrentNode.m_dQty, CurrentNode.m_dVal, FALSE, ( FALSE == m_bCreatingPromoSection ), FALSE );
			
			CString strLine = "";
			strLine += ItemEntity.m_strHeader;
			strLine += "\t";
			strLine += csv.GetLine();
			
			m_ReportFile.WriteReportDataLine( strLine );
		}
		else
		{
			ProcessSalesTreeBranch( nDepth + 1, m_pSalesOrPromoSection -> GetNextBranchIdx( nBranchIdx, nLeafIdx ), CurrentNode, ItemEntity );
		}
	}

	{
		CCSV csv( '\t' );
		AppendLineTotals( csv, BlockTotalNode.m_dQty, BlockTotalNode.m_dVal, m_bCreatingPromoSection, TRUE, FALSE );
		
		CString strTotal = "";
		strTotal += BlockEntity.m_strHeader;
		strTotal += "\t";
		strTotal += csv.GetLine();
		
		if ( nDepth != m_pSalesOrPromoLevels -> GetSize() - 1 ) 
		{	
			m_ReportFile.RequestBlankLines(2);

			int nSize = m_arraySalesHeaders.GetSize();
			for (int n = (1 == nSize) ? 0 : 1; n < nSize; n++)
			{
				m_ReportFile.WriteReportMiscLine(m_arraySalesHeaders[n].m_strHeader);
			}

			m_ReportFile.WriteReportMiscLine( "<LI>" );
	
			int nSubTotalCount = m_arraySalesTotals.GetSize();
			
			if ( nSubTotalCount != 0 )
			{
				int nSubTotalType = m_arraySalesTotals[ nSubTotalCount - 1 ].m_nEntityType;
				int nSubTotalIdx = nSubTotalCount - 2;

				for (; nSubTotalIdx >= 0; nSubTotalIdx--)
				{
					if (m_arraySalesTotals[nSubTotalIdx].m_nEntityType != nSubTotalType)
					{
						break;
					}
				}

				int nSubTotalBatchStart = nSubTotalIdx + 1;
		
				for (int n = nSubTotalBatchStart; n < nSubTotalCount; n++)
				{
					m_ReportFile.WriteReportMiscLine(m_arraySalesTotals[n].m_strTotal);
				}

				m_arraySalesTotals.RemoveAt( nSubTotalBatchStart, nSubTotalCount - nSubTotalBatchStart );
			}
		}

		m_ReportFile.WriteReportMiscLine( m_bCreatingPromoSection ? m_strReconUnderline : m_strUnderline );
		m_ReportFile.WriteReportMiscLine( strTotal );
		
		BlockEntity.m_strTotal = strTotal;
		m_arraySalesTotals.Add( BlockEntity );
	}

	int nSize = m_arraySalesHeaders.GetSize(); 
	
	if (nSize > 0)
	{
		m_arraySalesHeaders.RemoveAt(nSize - 1);
	}
}

/**********************************************************************/

void CEposReportGeneral::GetGroupNumberAndName( CGeneralReportSalesBlockEntity& BlockEntity )
{
	CString strNum = "";
	CString strName = "";

	switch( BlockEntity.m_nEntityType )
	{
	case NODE_PLACEHOLDER:
			
		//if ( FALSE == m_bDorbiereMode )
			strNum = ( m_bCreatingPromoSection ) ? "Promo" : "Sales";
		
		strName = "Total";
		break;

	case NODE_PROMOTION:
		{
			if ( BlockEntity.m_nEntityNum < 10000 )
			{
				strNum.Format( "P%3.3d", BlockEntity.m_nEntityNum );

				int nPromoIdx;
				if ( DataManager.Promotion.FindTableByNumber( BlockEntity.m_nEntityNum, nPromoIdx ) == TRUE )
				{
					CPromotionsCSVRecord Promo;
					DataManager.Promotion.GetAt( nPromoIdx, Promo );
					strName = Promo.GetDisplayName();
				}
				else
				{
					strName.Format("Promo %d", BlockEntity.m_nEntityNum);
				}
			}
			else
			{
				int nMMNo = BlockEntity.m_nEntityNum % 10000;

				strNum.Format( "M%3.3d", nMMNo );

				int nMMIdx;
				if ( DataManager.MixMatch.FindTableByNumber( nMMNo, nMMIdx ) == TRUE )
				{
					CMixMatchCSVRecord MixMatch;
					DataManager.MixMatch.GetAt( nMMIdx, MixMatch );
					strName = MixMatch.GetDisplayName();
				}
				else
				{
					strName.Format("Mix Match %d", nMMNo);
				}

				{
					CString strType;
					int nMMType = BlockEntity.m_nEntityNum / 10000;
					::GetMixMatchOfferTypeName( nMMType, strType, FALSE, "Unknown Mix Match Type" );
					BlockEntity.m_strMixMatchType = "\t<..>(";
					BlockEntity.m_strMixMatchType += strType;
					BlockEntity.m_strMixMatchType += ")";
				}
			}
		}
		break;

	case NODE_DEPARTMENT:	
		{
			strNum.Format("D%3.3d", BlockEntity.m_nEntityNum);

			int nDeptIdx;
			if (DataManager.Department.FindDeptByNumber(BlockEntity.m_nEntityNum, nDeptIdx) == TRUE)
			{
				CDepartmentCSVRecord DeptRecord;
				DataManager.Department.GetAt(nDeptIdx, DeptRecord);
				strName = DeptRecord.GetReportText();
			}
			else
			{
				strName.Format("Department %d", BlockEntity.m_nEntityNum);
			}
		}
		break;

	case NODE_DEPT_GROUP_EPOS:	
		{
			strNum.Format( "G%2.2d", BlockEntity.m_nEntityNum );

			int nGroupIdx;
			if ( DataManager.EposGroup.FindGroupByNumber( BlockEntity.m_nEntityNum, nGroupIdx ) == TRUE )
			{
				CGroupCSVRecordEpos GroupRecord;
				DataManager.EposGroup.GetAt( nGroupIdx, GroupRecord );
				strName = GroupRecord.GetReportText();
			}
			else
			{
				strName.Format("Group %d", BlockEntity.m_nEntityNum);
			}
		}
		break;

	case NODE_DEPT_GROUP_REPORT:	
		{
			strNum.Format("RG%2.2d", BlockEntity.m_nEntityNum);

			int nGroupIdx;
			if (DataManager.ReportGroup.FindGroupByNumber(BlockEntity.m_nEntityNum, nGroupIdx) == TRUE)
			{
				CGroupCSVRecordReport GroupRecord;
				DataManager.ReportGroup.GetAt(nGroupIdx, GroupRecord);
				strName = GroupRecord.GetReportText();
			}
			else
			{
				strName.Format("Rep. Group %d", BlockEntity.m_nEntityNum);
			}
		}
		break;

	case NODE_DEPT_GROUP_CONSOL:	
		{
			strNum.Format("CG%2.2d", BlockEntity.m_nEntityNum);

			int nGroupIdx;
			if (DataManager.ConsolGroup.FindGroupByNumber(BlockEntity.m_nEntityNum, nGroupIdx) == TRUE)
			{
				CGroupCSVRecordConsol GroupRecord;
				DataManager.ConsolGroup.GetAt(nGroupIdx, GroupRecord);
				strName = GroupRecord.GetReportText();
			}
			else
			{
				strName.Format("Con. Group %d", BlockEntity.m_nEntityNum);
			}
		}
		break;
	}

	CCSV csv( '\t' );
	csv.Add( strNum );
	csv.Add( strName );
	BlockEntity.m_strHeader = csv.GetLine();
}
	
/**********************************************************************/

void CEposReportGeneral::CreatePaymentSection( CEposReportGeneralBlock& Block )
{
	if ( Block.m_pSales -> GetSize() > 0 )
		m_ReportFile.RequestBlankLines(1);

	for ( int p = 0; p < Block.m_pPayment -> GetSize(); p++ )
	{
		CEposReportConsolGeneralMisc Payment;
		Block.m_pPayment -> GetAt( p, Payment );
		UpdateCreateReportProgress();

		if ( GENREP_PAYMENT_TOTAL == Payment.m_nLineType )
		{
			m_ReportFile.WriteReportMiscLine( m_strUnderline );

			CCSV csv( '\t' );
			csv.Add( "" );
			csv.Add( "Total" );
			AppendLineTotals( csv, Payment.m_dQty, Payment.m_dValue, FALSE, FALSE, FALSE );
			m_ReportFile.WriteReportMiscLine( csv.GetLine() );
		}
		else
		{
			CCSV csv( '\t' );
		
			CString strNum;
			strNum.Format( "P%3.3d", Payment.m_nLineData );
			csv.Add( strNum );
			csv.Add( DataManager.Payment.GetDisplayName( Payment.m_nLineData ) );
			
			AppendLineTotals( csv, Payment.m_dQty, Payment.m_dValue, FALSE, FALSE, FALSE );
			m_ReportFile.WriteReportDataLine( csv.GetLine() );
		}
	}
}

/**********************************************************************/

void CEposReportGeneral::CreateVarianceSection( CEposReportGeneralBlock& Block, bool bForEx )
{
	CReportConsolidationArray<CEposReportConsolGeneralVariance>* pSource;
	if (TRUE == bForEx)
	{
		pSource = Block.m_pVarianceForEx;
	}
	else
	{
		pSource = Block.m_pVariance;
	}

	double dDummyQty[8];

	for ( int p = 0; p < pSource -> GetSize(); p++ )
	{
		CEposReportConsolGeneralVariance Variance;
		pSource -> GetAt( p, Variance );
		UpdateCreateReportProgress();

		if ( TRUE == Variance.m_bTotal )
		{
			m_ReportFile.WriteReportMiscLine( m_strUnderline );

			CCSV csv( '\t' );
			csv.Add( "" );
			csv.Add( "Total" );
			AppendLineTotals( csv, dDummyQty, Variance.m_dValue, TRUE, FALSE, FALSE );
			m_ReportFile.WriteReportMiscLine( csv.GetLine() );
		}
		else
		{
			CCSV csv( '\t' );
		
			CString strNum;
			strNum.Format( "P%3.3d", Variance.m_nMethodNo );
			csv.Add( strNum );
			csv.Add( Variance.m_strDescription );
			
			AppendLineTotals( csv, dDummyQty, Variance.m_dValue, TRUE, FALSE, FALSE );
			m_ReportFile.WriteReportDataLine( csv.GetLine() );
		}
	}
}

/**********************************************************************/

void CEposReportGeneral::CreateTaxSection( CEposReportGeneralBlock& Block, bool bTaxAmount )
{
	CString strType = ( bTaxAmount ) ? EcrmanOptions.GetEstTaxString() : "Sales";

	for ( int s = Block.m_pTaxByPlu -> GetSize() - 1; s >= 0; s-- )
	{
		CEposReportConsolGeneralTaxPlu Item;
		Block.m_pTaxByPlu -> GetAt( s, Item );
		UpdateCreateReportProgress();

		m_PluInfoFinder.SetPluNo( Item.m_nPluNo );

		CEposReportConsolGeneralTaxBand ConsolInfo;
		ConsolInfo.m_nTaxDateCode = Item.m_nTaxDateCode;
		ConsolInfo.m_nTaxBand = m_PluInfoFinder.GetModifierTaxBand( Item.m_nModifier );

		for ( int n = 0; n < 8; n++ )
		{
			ConsolInfo.m_dQty[n] = Item.m_dQty[n];
			ConsolInfo.m_dValue[n] = Item.m_dValue[n];
		}

		Block.m_pTaxByBand -> Consolidate( ConsolInfo );
		Block.m_pTaxByPlu -> RemoveAt( s );
	}

	int nSize = Block.m_pTaxByBand -> GetSize();

	if ( nSize != 0 )
	{
		CEposReportConsolGeneralTaxBand LineTotal;
		CEposReportConsolGeneralTaxBand TaxBandTotal;
		CEposReportConsolGeneralTaxBand SalesTotal;
		TaxBandTotal.m_nTaxBand = -1;
	
		for ( int p = 0; p <= nSize; p++ )
		{
			bool bNewBlock = FALSE;
			
			if ( p < nSize )
			{
				Block.m_pTaxByBand -> GetAt( p, LineTotal );
				bNewBlock = ( TaxBandTotal.m_nTaxBand != LineTotal.m_nTaxBand );

				if (TRUE == bTaxAmount)
				{
					//REPLACE SALES VALUE WITH TAX CONTENT
					int nMaxDay = (m_ReportSettings.GetWeekdayFlag() ? 7 : 0);
					double dTaxRate = DataManager.HistoricalTaxRates.GetTaxRate(LineTotal.m_nTaxDateCode, LineTotal.m_nTaxBand);

					for (int nDay = 0; nDay <= nMaxDay; nDay++)
					{
						LineTotal.m_dValue[nDay] = LineTotal.m_dValue[nDay] - ReportHelpers.CalcNonTax(LineTotal.m_dValue[nDay], dTaxRate);
					}
				}
				
				SalesTotal.Add( LineTotal );
				
				if (p < Block.m_nOriginalTaxBandSize)
				{
					UpdateCreateReportProgress();
				}

			}

			if ( p != 0 ) 
			{
				if ( ( TRUE == bNewBlock ) || ( p == nSize ) )
				{
					CTaxRateInfo TaxRateInfo;
					DataManager.TaxRates.GetTaxRateInfo( TaxBandTotal.m_nTaxBand, TaxRateInfo );

					CString strBand = "Band 0";
					if ( ( TaxBandTotal.m_nTaxBand >= 1 ) && ( TaxBandTotal.m_nTaxBand <= MAX_TAX_BANDS ) )
					{
						strBand = "Band ";
						strBand += char ( 'A' + TaxBandTotal.m_nTaxBand - 1 );
					}

					CString strBandName = TaxRateInfo.m_strReportText;
					if ( ( strBandName != "" ) && ( strBandName != strBand ) )
					{
						strBand += " : ";
						strBand += strBandName;
					}

					CCSV csv( '\t' );
					csv.Add( strType );
					csv.Add( strBand );
					AppendLineTotals( csv, TaxBandTotal.m_dQty, TaxBandTotal.m_dValue, FALSE, TRUE, FALSE );
					m_ReportFile.WriteReportDataLine( csv.GetLine() );
				}
			}

			if (TRUE == bNewBlock)
			{
				TaxBandTotal = LineTotal;
			}
			else
			{
				TaxBandTotal.Add(LineTotal);
			}
		}

		m_ReportFile.WriteReportMiscLine( m_strUnderline );

		CCSV csv( '\t' );
		csv.Add( strType );
		csv.Add( "Total" );
		AppendLineTotals( csv, SalesTotal.m_dQty, SalesTotal.m_dValue, FALSE, TRUE, FALSE );
		m_ReportFile.WriteReportMiscLine( csv.GetLine() );
	}
}

/**********************************************************************/

void CEposReportGeneral::CreatePromoSection( CEposReportGeneralBlock& Block, CString& strHeader )
{
	bool bGotItemPromo = FALSE;
	bool bGotSubtotalPromo = FALSE;
	bool bGotMixMatch = FALSE;

	if ( Block.m_pPromoSummary -> GetSize() != 0 )
	{
		m_ReportFile.RequestBlankLines(2);
		m_ReportFile.WriteReportMiscLine( strHeader + ", Promotions & Surcharges" );
		m_ReportFile.WriteReportMiscLine( "<LI>" );
	}

	int nLastLineType = -1;
	for ( int r = 0; r < Block.m_pPromoSummary -> GetSize(); r++ )
	{
		CEposReportConsolGeneralMisc Promo;
		Block.m_pPromoSummary -> GetAt( r, Promo );
		UpdateCreateReportProgress();

		switch( Promo.m_nLineType )
		{
		case GENREP_PROMO_LINE_ITEM:
		case GENREP_PROMO_LINE_SUBT:
			{
				int nPromoIdx;
				CString strPromoName = "";
				if ( DataManager.Promotion.FindTableByNumber( Promo.m_nLineData, nPromoIdx ) == TRUE )
				{
					CPromotionsCSVRecord PromoRecord;
					DataManager.Promotion.GetAt( nPromoIdx, PromoRecord );
					strPromoName = PromoRecord.GetDisplayName();
				}

				if (strPromoName == "")
				{
					strPromoName.Format("Promo %d", Promo.m_nLineData);
				}

				CCSV csv( '\t' );

				if ( GENREP_PROMO_LINE_ITEM == Promo.m_nLineType )
				{
					csv.Add( "Item" );
					bGotItemPromo = TRUE;
				}
				else
				{
					csv.Add( "Subtotal" );
					bGotSubtotalPromo = TRUE;
				}

				csv.Add( strPromoName );
				AppendLineTotals( csv, Promo.m_dQty, Promo.m_dValue, FALSE, FALSE, FALSE );
				m_ReportFile.WriteReportDataLine( csv.GetLine() );
			}
			break;

		case GENREP_PROMO_LINE_MIXMATCH:
			{
				int nMixMatchIdx;
				CString strMixMatchName = "";
				if ( DataManager.MixMatch.FindTableByNumber( Promo.m_nLineData % 10000, nMixMatchIdx ) == TRUE )
				{
					CMixMatchCSVRecord MixMatchRecord;
					DataManager.MixMatch.GetAt( nMixMatchIdx, MixMatchRecord );
					strMixMatchName = MixMatchRecord.GetDisplayName();
				}

				if (strMixMatchName == "")
				{
					strMixMatchName.Format("Mix Match %d", Promo.m_nLineData % 10000);
				}

				CCSV csv( '\t' );
				csv.Add( "Mix Match" );
				csv.Add( strMixMatchName );
				AppendLineTotals( csv, Promo.m_dQty, Promo.m_dValue, FALSE, FALSE, FALSE );

				if (nLastLineType <= 3)
				{
					m_ReportFile.WriteReportMiscLine(m_strUnderline);
				}

				m_ReportFile.WriteReportDataLine( csv.GetLine() );

				bGotMixMatch = TRUE;
			}
			break;

		case GENREP_PROMO_TOTAL_ITEM:
			if ( TRUE == bGotItemPromo ) 
			{
				if ( ( TRUE == bGotSubtotalPromo ) || ( TRUE == bGotMixMatch ) )
				{
					CCSV csv( '\t' );
					csv.Add( "Item" );
					csv.Add( "Promo (Total)" );
					AppendLineTotals( csv, Promo.m_dQty, Promo.m_dValue, FALSE, FALSE, FALSE );
					
					if (nLastLineType <= 3)
					{
						m_ReportFile.WriteReportMiscLine(m_strUnderline);
					}
					
					m_ReportFile.WriteReportMiscLine( csv.GetLine() );
				}
			}
			break;

		case GENREP_PROMO_TOTAL_SUBT:
			if ( TRUE == bGotSubtotalPromo ) 
			{
				if ( ( TRUE == bGotItemPromo ) || ( TRUE == bGotMixMatch ) )
				{
					CCSV csv( '\t' );
					csv.Add( "Subtotal" );
					csv.Add( "Promo (Total)" );
					AppendLineTotals( csv, Promo.m_dQty, Promo.m_dValue, FALSE, FALSE, FALSE );
					
					if (nLastLineType <= 3)
					{
						m_ReportFile.WriteReportMiscLine(m_strUnderline);
					}

					m_ReportFile.WriteReportDataLine( csv.GetLine() );
				}
			}
			break;

		case GENREP_PROMO_TOTAL_MIXMATCH:
			if ( TRUE == bGotMixMatch ) 
			{
				if ( ( TRUE == bGotItemPromo ) || ( TRUE == bGotSubtotalPromo ) )
				{
					CCSV csv( '\t' );
					csv.Add( "Mix Match" );
					csv.Add( "(Total)" );
					AppendLineTotals( csv, Promo.m_dQty, Promo.m_dValue, FALSE, FALSE, FALSE );
					m_ReportFile.WriteReportMiscLine( m_strUnderline );
					m_ReportFile.WriteReportMiscLine( csv.GetLine() );
				}
			}
			break;

		case GENREP_PROMO_TOTAL_ALL:
			{
				CCSV csv( '\t' );
				csv.Add( "" );
				csv.Add( "Promo (Total)" );
				AppendLineTotals( csv, Promo.m_dQty, Promo.m_dValue, FALSE, FALSE, FALSE );
				m_ReportFile.WriteReportMiscLine( m_strUnderline );
				m_ReportFile.WriteReportMiscLine( csv.GetLine() );
			}
			break;
		}

		nLastLineType = Promo.m_nLineType;
	}
}

/**********************************************************************/

void CEposReportGeneral::CreateVoidSection( CEposReportGeneralBlock& Block )
{
	bool bGotVoidSaleBAType[4];
	bGotVoidSaleBAType[0] = FALSE;
	bGotVoidSaleBAType[1] = FALSE;
	bGotVoidSaleBAType[2] = FALSE;
	bGotVoidSaleBAType[3] = FALSE;

	bool bGotVoidRefundBAType[4];
	bGotVoidRefundBAType[0] = FALSE;
	bGotVoidRefundBAType[1] = FALSE;
	bGotVoidRefundBAType[2] = FALSE;
	bGotVoidRefundBAType[3] = FALSE;

	bool bBlankAfterSale = FALSE;

	CArray<int,int> arrayVoidTypes;
	arrayVoidTypes.Add( GENREP_VOID_SALE_TRAN );
	arrayVoidTypes.Add( GENREP_VOID_SALE_BEFORE );
	arrayVoidTypes.Add( GENREP_VOID_SALE_AFTER );
	arrayVoidTypes.Add( GENREP_VOID_SALE_TOTAL );
	arrayVoidTypes.Add( GENREP_VOID_REFUND_TRAN );
	arrayVoidTypes.Add( GENREP_VOID_REFUND_BEFORE );
	arrayVoidTypes.Add( GENREP_VOID_REFUND_AFTER );
	arrayVoidTypes.Add( GENREP_VOID_REFUND_TOTAL );

	CEposReportConsolGeneralMisc Void;
	for ( int n = 0; n < arrayVoidTypes.GetSize(); n++ )
	{
		Void.m_nLineType = arrayVoidTypes[n];
		Block.m_pVoid ->Consolidate( Void );
	}

	for ( int r = 0; r < Block.m_pVoid -> GetSize(); r++ )
	{
		CEposReportConsolGeneralMisc Void;
		Block.m_pVoid -> GetAt( r, Void );
		UpdateCreateReportProgress();

		bool bShowLine = FALSE;

		CCSV csv( '\t' );
		csv.Add( "" );

		switch( Void.m_nLineType )
		{
		case GENREP_VOID_SALE_LEGACY:
			bGotVoidSaleBAType[0] = TRUE;
			csv.Add( "Void Sales" );
			bShowLine = TRUE;
			break;

		case GENREP_VOID_SALE_TRAN:
			bGotVoidSaleBAType[1] = TRUE;
			csv.Add( "Void Sales (Transaction)" );
			bShowLine = TRUE;
			break;

		case GENREP_VOID_SALE_BEFORE:
			bGotVoidSaleBAType[2] = TRUE;
			csv.Add( "Void Sales (Before)" );
			bShowLine = TRUE;
			break;

		case GENREP_VOID_SALE_AFTER:
			bGotVoidSaleBAType[3] = TRUE;
			csv.Add( "Void Sales (After)" );
			bShowLine = TRUE;
			break;

		case GENREP_VOID_SALE_TOTAL:
			{
				int nVoidCount = 0;
				csv.Add( "Void Sales (Total)" );
				if ( bGotVoidSaleBAType[0] == TRUE )	nVoidCount++;
				if ( bGotVoidSaleBAType[1] == TRUE )	nVoidCount++;
				if ( bGotVoidSaleBAType[2] == TRUE )	nVoidCount++;
				if ( bGotVoidSaleBAType[3] == TRUE )	nVoidCount++;
				bShowLine = ( nVoidCount >= 2 );
			}
			break;

		case GENREP_VOID_REFUND_LEGACY:
			bGotVoidRefundBAType[0] = TRUE;
			csv.Add( "Void Refunds" );
			bShowLine = TRUE;
			break;

		case GENREP_VOID_REFUND_TRAN:
			bGotVoidSaleBAType[1] = TRUE;
			csv.Add( "Void Refunds (Transaction)" );
			bShowLine = TRUE;
			break;

		case GENREP_VOID_REFUND_BEFORE:
			bGotVoidRefundBAType[2] = TRUE;
			csv.Add( "Void Refunds (Before)" );
			bShowLine = TRUE;
			break;

		case GENREP_VOID_REFUND_AFTER:
			bGotVoidRefundBAType[3] = TRUE;
			csv.Add( "Void Refunds (After)" );
			bShowLine = TRUE;
			break;

		case GENREP_VOID_REFUND_TOTAL:
			{
				int nVoidCount = 0;
				csv.Add( "Void Refunds (Total)" );
				if ( bGotVoidRefundBAType[0] == TRUE )	nVoidCount++;
				if ( bGotVoidRefundBAType[1] == TRUE )	nVoidCount++;
				if ( bGotVoidRefundBAType[2] == TRUE )	nVoidCount++;
				if ( bGotVoidRefundBAType[3] == TRUE )	nVoidCount++;
				bShowLine = ( nVoidCount >= 2 );
			}
			break;
		}

		if ( TRUE == bShowLine )
		{
			if (TRUE == bBlankAfterSale)
			{
				m_ReportFile.RequestBlankLines(1);
			}
			
			switch( Void.m_nLineType )
			{
			case GENREP_VOID_SALE_TOTAL:
			case GENREP_VOID_REFUND_TOTAL: 
				m_ReportFile.WriteReportMiscLine( m_strUnderline );
				break;
			}

			AppendLineTotals( csv, Void.m_dQty, Void.m_dValue, FALSE, FALSE, FALSE );
			m_ReportFile.WriteReportDataLine( csv.GetLine() );
		}

		bBlankAfterSale = ( GENREP_VOID_SALE_TOTAL == Void.m_nLineType );
	}
}

/**********************************************************************/

void CEposReportGeneral::CreateNoSaleSection( CEposReportGeneralBlock& Block )
{
	if ( Block.m_pNoSale -> GetSize() != 0 )
	{
		if (Block.m_pVoid->GetSize() != 0)
		{
			m_ReportFile.RequestBlankLines(1);
		}

		CEposReportConsolGeneralMisc NoSale;
		Block.m_pNoSale -> GetAt( 0, NoSale );
		UpdateCreateReportProgress();

		CCSV csv( '\t' );
		csv.Add( "" );
		csv.Add( "No Sale" );

		AppendLineTotals( csv, NoSale.m_dQty, NoSale.m_dQty, FALSE, FALSE, TRUE, TRUE );
		
		m_ReportFile.WriteReportDataLine( csv.GetLine() );
	}
}

/**********************************************************************/

void CEposReportGeneral::CreateReconciliationSection( CEposReportGeneralBlock& Block, CString& strHeader )
{
	bool bShowReconciliationBlock = FALSE;
	CStringArray arrayReconciliationBuffer;

	arrayReconciliationBuffer.Add( strHeader + ", Reconciliation" );
	arrayReconciliationBuffer.Add( "<LI>" );
		
	for ( int r = 0; r < Block.m_pRecon -> GetSize(); r++ )
	{
		CEposReportConsolGeneralMisc Recon;
		Block.m_pRecon -> GetAt( r, Recon );
		UpdateCreateReportProgress();
		
		switch( Recon.m_nLineType )
		{
		case GENREP_RECON_CHARGE_PAID:	//PAYMENTS RECEIVED
		case GENREP_RECON_BALANCE:		//BALANCE
			arrayReconciliationBuffer.Add( m_strReconUnderline );
			break;
		}

		bool bShowIfZero = FALSE;
		CString strText = "";
					
		switch( Recon.m_nLineType )
		{
		case GENREP_RECON_SALE:	
			strText = "Items Purchased";			
			bShowIfZero = TRUE;	
			break;

		case GENREP_RECON_SALE_FILTERED:	
			strText = "Items Filtered by Department";			
			break;

		case GENREP_RECON_RA_DEPOSIT:
			strText = "Deposits Purchased";			
			break;

		case GENREP_RECON_RA_CUSTOMER:	
			strText = "Received on Customer Account";	
			break;

		case GENREP_RECON_RA_ROOM:	
			strText = "Received as Room Settlement";	
			break;

		case GENREP_RECON_RA_LOYALTY:	
			strText = "Loyalty Top-ups";	
			break;

		case GENREP_RECON_RA_SMARTPAY:	
			strText = "SmartPay Top-ups";	
			break;

		case GENREP_RECON_RA_SMARTPHONE:	
			strText = "mySmartPhoneMoney Top-ups";	
			break;

		case GENREP_RECON_RA_SPTBOOK:	
			strText = "Received on Sports Booker Account";	
			break;

		case GENREP_RECON_GRATUITY:	
			strText = "Gratutities";			
			break;

		case GENREP_RECON_CASHBACK:	
			strText = "Cashback";			
			break;

		case GENREP_RECON_SURPLUS:	
			strText = "Surplus";			
			break;

		case GENREP_RECON_CHANGE:	
			strText = "Change";		
			break;

		case GENREP_RECON_PAYIN01:	
		case GENREP_RECON_PAYIN02:	
		case GENREP_RECON_PAYIN03:	
		case GENREP_RECON_PAYIN04:	
		case GENREP_RECON_PAYIN05:	
		case GENREP_RECON_PAYIN06:	
		case GENREP_RECON_PAYIN07:	
		case GENREP_RECON_PAYIN08:
		case GENREP_RECON_PAYIN09:
		case GENREP_RECON_PAYIN10:
		case GENREP_RECON_PAYIN11:
		case GENREP_RECON_PAYIN12:
		case GENREP_RECON_PAYIN13:
		case GENREP_RECON_PAYIN14:
		case GENREP_RECON_PAYIN15:
		case GENREP_RECON_PAYIN16:
		case GENREP_RECON_PAYIN17:
		case GENREP_RECON_PAYIN18:
		case GENREP_RECON_PAYIN19:
		case GENREP_RECON_PAYIN20:
		case GENREP_RECON_PAYINXX:
			{
				CPaidInOutTextCSVRecord Text;
				DataManager.PaidInText.GetReportPaidInOutText( Recon.m_nLineType - 100, Text );
				strText.Format( "Paid In (%s)", Text.GetEposText() );
			}
			break;

		case GENREP_RECON_CHARGE_PAID:	
			strText = "Money Received";			
			bShowIfZero = TRUE; 
			break;

		case GENREP_RECON_REFUND_DEPOSIT:
			strText = "Deposits Refunded";
			break;

		case GENREP_RECON_CHARGE_DEPOSIT:
			strText = "Deposit Redemption";
			break;

		case GENREP_RECON_CHARGE_CUSTOMER:	
			strText = "Charged to Customer Account";	
			break;

		case GENREP_RECON_CHARGE_ROOM:	
			strText = "Charged to Room";	
			break;

		case GENREP_RECON_CHARGE_LOYALTY:	
			strText = "Loyalty Redemption";	
			break;

		case GENREP_RECON_CHARGE_SMARTPAY:	
			strText = "SmartPay Redemption";	
			break;

		case GENREP_RECON_CHARGE_SMARTPHONE:	
			strText = "mySmartPhoneMoney Redemption";	
			break;

		case GENREP_RECON_CHARGE_SPTBOOK:	
			strText = "Charged to Sports Booker Account";	
			break;

		case GENREP_RECON_PAYOUT01:	
		case GENREP_RECON_PAYOUT02:	
		case GENREP_RECON_PAYOUT03:	
		case GENREP_RECON_PAYOUT04:	
		case GENREP_RECON_PAYOUT05:	
		case GENREP_RECON_PAYOUT06:	
		case GENREP_RECON_PAYOUT07:	
		case GENREP_RECON_PAYOUT08:
		case GENREP_RECON_PAYOUT09:
		case GENREP_RECON_PAYOUT10:
		case GENREP_RECON_PAYOUT11:
		case GENREP_RECON_PAYOUT12:
		case GENREP_RECON_PAYOUT13:
		case GENREP_RECON_PAYOUT14:
		case GENREP_RECON_PAYOUT15:
		case GENREP_RECON_PAYOUT16:
		case GENREP_RECON_PAYOUT17:
		case GENREP_RECON_PAYOUT18:
		case GENREP_RECON_PAYOUT19:
		case GENREP_RECON_PAYOUT20:
		case GENREP_RECON_PAYOUTXX:
			{
				CPaidInOutTextCSVRecord Text;
				DataManager.PaidOutText.GetReportPaidInOutText( Recon.m_nLineType - 300, Text );
				strText.Format( "Paid Out (%s)", Text.GetEposText() );
			}
			break;

		case GENREP_RECON_HOLTS_PROMO:
			strText = "Promotions and Mix Match";
			break;

		case GENREP_RECON_BALANCE:	
			strText = "Balance";					
			bShowIfZero = TRUE; 
			break;
		}

		bool bShowLine = TRUE;

		int nValuesToCheck = ( m_ReportSettings.GetWeekdayFlag() ? 7 : 1 );
		
		bool bIsZeroLine = TRUE;
		for (int n = 0; (n < nValuesToCheck) && (TRUE == bIsZeroLine); n++)
		{
			bIsZeroLine &= (CPriceHelpers::CompareDoubles(Recon.m_dValue[n], 0.0, SysInfo.GetDPValue()) == 0);
		}

		if (TRUE == bIsZeroLine)
		{
			bShowLine = bShowIfZero;
		}
		else if ((FALSE == bShowIfZero) || (GENREP_RECON_BALANCE == Recon.m_nLineType))
		{
			bShowReconciliationBlock = TRUE;
		}

		if ( TRUE == bShowLine )
		{
			CCSV csv( '\t' );
			csv.Add( "" );
			csv.Add( strText );
			AppendLineTotals( csv, Recon.m_dQty, Recon.m_dValue, TRUE, FALSE, FALSE );
			arrayReconciliationBuffer.Add( csv.GetLine() );
		}
	}

	if ( TRUE == bShowReconciliationBlock )
	{
		int nSize = arrayReconciliationBuffer.GetSize();

		if (nSize > 0)
		{
			m_ReportFile.RequestBlankLines(2);
			for (int n = 0; n < nSize; n++)
			{
				m_ReportFile.WriteReportDataLine(arrayReconciliationBuffer.GetAt(n));
			}
		}
	}
}

/**********************************************************************/

void CEposReportGeneral::AppendLineTotals( CCSV& csvOut, double dQty[8], double dVal[8], bool bHideQty, bool bShowFractionQty, bool bHideVal, bool bNoSale )
{
	if ( m_ReportSettings.GetWeekdayFlag() == TRUE )
	{
		if ( m_ReportSettings.GetQuantityFlag() == TRUE )
		{
			for ( int nDayAfterWeekStart = 0; nDayAfterWeekStart < 7; nDayAfterWeekStart++ )
			{
				AppendQty ( csvOut, dQty[nDayAfterWeekStart], bHideQty, bShowFractionQty );
				AppendValue ( csvOut, dVal[nDayAfterWeekStart], bHideVal );
			}

			AppendQty ( csvOut, dQty[7], bHideQty, bShowFractionQty );
			AppendValue ( csvOut, dVal[7], bHideVal );
		}
		else
		{
			int nDay = EcrmanOptions.GetReportsWeekStartDay();

			if ( FALSE == bNoSale )
			{
				for (int nDayAfterWeekStart = 0; nDayAfterWeekStart < 7; nDayAfterWeekStart++)
				{
					AppendValue(csvOut, dVal[nDayAfterWeekStart], FALSE);
				}

				AppendValue ( csvOut, dVal[7], FALSE );
			}
			else
			{
				for (int nDayAfterWeekStart = 0; nDayAfterWeekStart < 7; nDayAfterWeekStart++)
				{
					AppendQty(csvOut, dQty[nDayAfterWeekStart], FALSE, FALSE);
				}

				AppendQty ( csvOut, dQty[7], FALSE, FALSE );
			}
		}
	}
	else
	{
		if ( m_ReportSettings.GetQuantityFlag() == TRUE )
		{
			AppendQty ( csvOut, dQty[0], bHideQty, bShowFractionQty );
			AppendValue ( csvOut, dVal[0], bHideVal );
		}
		else
		{
			if (FALSE == bNoSale)
			{
				AppendValue(csvOut, dVal[0], FALSE);
			}
			else
			{
				AppendQty(csvOut, dQty[0], FALSE, FALSE);
			}
		}
	}
}

/**********************************************************************/

void CEposReportGeneral::AppendValue( CCSV& csvOut, double dValue, bool bHideVal )
{
	if ( FALSE == bHideVal )
	{
		int nDP = SysInfo.GetDPValue();

		if (CPriceHelpers::CompareDoubles(dValue, 0.0, nDP) == 0)
		{
			dValue = 0.0;
		}

		csvOut.Add( dValue, nDP );
	}
	else
	{
		csvOut.Add("");
	}
}

/**********************************************************************/

void CEposReportGeneral::AppendQty(CCSV& csvOut, double dQty, bool bHideQty, bool bShowFraction)
{
	if (FALSE == bHideQty)
	{
		int nDP = (bShowFraction) ? SysInfo.GetDPQty() : 0;

		if (CPriceHelpers::CompareDoubles(dQty, 0.0, nDP) == 0)
		{
			dQty = 0.0;
		}

		csvOut.Add(dQty, nDP);
	}
	else
	{
		csvOut.Add("");
	}
}

/**********************************************************************/

