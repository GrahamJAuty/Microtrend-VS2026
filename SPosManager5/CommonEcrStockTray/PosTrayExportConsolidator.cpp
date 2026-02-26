/**********************************************************************/
#include "CashRSPImporterDiscountHelpers.h"
#include "DatabaseCSVArray.h"
#include "LocationCSVArray.h"
#include "..\CommonEcrStock\PosTrayManualExportDlg.h"
#include "ReportPluInfoFinder.h"
#include "ReportPointCSVArray.h"
#include "StatusProgress.h"
#include "SysInfo.h"
/**********************************************************************/
#include "PosTrayExportConsolidator.h"
/**********************************************************************/

CPosTrayExportConsolidatorLineInfo::CPosTrayExportConsolidatorLineInfo()
{
	m_nDbNo = 0;
	m_nLocNo = 0;
	m_nRpNo = 0;
	m_nTrmNo = 0;
	m_nTableGrpNo = 0;
	m_nTableNo = 0;
	m_strCalendarDate = "";
	m_strBusinessDate = "";
	m_strWeekStartDate = "";
	m_nYear = 0;
	m_nMonthNo = 0;
	m_nWeekNo = 0;
	m_nWeekdayNo = 0;
	m_nFinalTimeHour = 0;
	m_nFinalTimeMinute = 0;
	m_nFinalTimeSecond = 0;
	m_nFinalTimeStars = 0;
	m_nActionTimeHour = 0;
	m_nActionTimeMinute = 0;
	m_nActionTimeSecond = 0;
	m_nActionTimeStars = 0;
	m_nPluNo = 0;
	m_nModNo = 0;
	m_nDeptNoSale = 0;
	m_nDeptNoDBase = 0;
	m_nGroupNoSale = 0;
	m_nGroupNoDBase = 0;
	m_nACatNoSale = 0;
	m_nACatNoDBase = 0;
	m_nItemServerNo = 0;
	m_nAuthServerNo = 0;
	m_nTranServerNo = 0;
	m_nVoidSeqNo = 0;
	m_nVoidTableNo = 0;
	m_nPromoNo = 0;
	m_nMixTypeNo = 0;
	m_nMixMatchNo = 0;
	m_nPriceBand = 0;
	m_nTaxBand = 0;
	m_nPaymentNo = 0;
	m_nTranSeqNo = 0;
	/**********/
	m_strDbName = "";
	m_strLocName = "";
	m_strRpName = "";
	m_strTableGrpName = "";
	m_strTableName = "";
	m_strModName = "";
	m_strDeptNameSale = "";
	m_strDeptNameDBase = "";
	m_strGroupNameSale = "";
	m_strGroupNameDBase = "";
	m_strACatNameSale = "";
	m_strACatNameDBase = "";
	m_strItemServerName = "";
	m_strAuthServerName = "";
	m_strTranServerName = "";
	m_strMonth = "";
	m_strWeekdayName = "";
	m_strVoidLineType = "";
	m_strVoidBeforeAfter = "";
	m_strVoidReason = "";
	m_strDiscountLineType = "";
	m_strDiscountMode = "";
	m_strDiscountType = "";
	m_strDiscountUsage = "";
	m_strDiscountRate = "";
	m_strPromoName = "";
	m_strMixTypeName = "";
	m_strMixMatchName = "";
	m_strPriceBandName = "";
	m_strTaxBandName = "";
	m_strPaymentName = "";
	m_strS4LabourHour = "";
}

/**********************************************************************/

CPosTrayExportConsolidatorNodeTotals::CPosTrayExportConsolidatorNodeTotals()
{
	m_dQtyBase = 0.0;
	m_dQtyMod = 0.0;
	m_dValueFull = 0.0;
	m_dDiscountVal = 0.0;
	m_dPremiumVal = 0.0;
	m_dValueAfterDiscount = 0.0;
	m_dTaxAmount = 0.0;
	m_dValueNet = 0.0;
	m_dCost = 0.0;
	m_dWasteQtyBase = 0.0;
	m_dWasteQtyMod = 0.0;
	m_dWasteCost = 0.0;
	m_dValueVoid = 0.0;
	m_nPaymentQty = 0;
	m_nCoversQty = 0;
	m_dTender = 0.0;
	m_dAmount = 0.0;
	m_dGratuity = 0.0;
	m_dCashback = 0.0;
	m_dSurplus = 0.0;
	m_nLineCount = 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CPosTrayExportConsolidator::CPosTrayExportConsolidator( int nExportDateType, CEposSelectArray& SelectArray, bool& bQuitNow, void* pHostDlg ) : m_SelectArray( SelectArray ), m_bQuitNow( bQuitNow )
{
	m_pHostDialog = pHostDlg;
	m_nDisplayedProgress = -1;

	m_nExportDateType = nExportDateType;

	m_strWeekDays[0] = "Sun";
	m_strWeekDays[1] = "Mon";
	m_strWeekDays[2] = "Tue";
	m_strWeekDays[3] = "Wed";
	m_strWeekDays[4] = "Thu";
	m_strWeekDays[5] = "Fri";
	m_strWeekDays[6] = "Sat";

	m_bWantModName = FALSE;
	m_bWantDeptNameSale = FALSE;
	m_bWantDeptNameDBase = FALSE;
	m_bWantGroupNameSale = FALSE;
	m_bWantGroupNameDBase = FALSE;
	m_bWantACatNameSale = FALSE;
	m_bWantACatNameDBase = FALSE;
	m_bWantTaxBandName = FALSE;
	m_bWantPriceBandName = FALSE;
	m_bWantPromoName = FALSE;
	m_bWantMixTypeName = FALSE;
	m_bWantMixMatchName = FALSE;
	m_bGotVoidLineType = FALSE;
	m_bGotDiscountLineType = FALSE;

	m_bWantPluInfo = FALSE;
	m_bWantDeptInfo = FALSE;
	m_bWantDeptGrpSale = FALSE;
	m_bWantTranServer = FALSE;
	m_bWantItemServer = FALSE;
	m_bWantAuthServer = FALSE;
	m_bWantCustomer = FALSE;
	m_bWantTax = FALSE;
	m_bWantCost = FALSE;
	m_bWantWaste = FALSE;
	m_bWantSales = FALSE;
	m_bWantFinalTime = FALSE;
	m_bWantVoidTime = FALSE;
	m_bWantTable = FALSE;
	
	m_nTaxDecimalPlaces = SysInfo.GetDPValue();
}

/**********************************************************************/

void CPosTrayExportConsolidator::AddAcceptedField( int nField )
{
	CSortedIntItem item;
	item.m_nItem = nField;
	m_arrayAcceptedFields.Consolidate( item );
}

/**********************************************************************/

bool CPosTrayExportConsolidator::IsAcceptedField( int nField )
{
	CSortedIntItem item;
	item.m_nItem = nField;

	int nPos;
	return m_arrayAcceptedFields.Find( item, nPos );
}

/**********************************************************************/

void CPosTrayExportConsolidator::Reset( CArray<int,int>& arrayFields, CPosTrayTask& EmailTask, COleDateTime dateFrom, COleDateTime dateTo, CSessionCSVRecord& ReportSession, CTimeSliceMapCSVRecord& TimeSlice )
{
	DataManagerNonDb.SessionDateTimeFilter.Reset();
	m_TimeSliceMap.SetReportType( SH_TIMESLICE_SECOND );
	m_TimeSliceMap.SetTimeSliceFileNo(0);
	SetDates( dateFrom, dateTo, ReportSession, TimeSlice );
	CheckExportFields( arrayFields, EmailTask );
	ResetTaskSpecific( EmailTask );
}

/**********************************************************************/

void CPosTrayExportConsolidator::CheckExportFields(CArray<int, int>& arrayFields, CPosTrayTask& EmailTask)
{
	m_bWantModName = FALSE;
	m_bWantDeptNameSale = FALSE;
	m_bWantDeptNameDBase = FALSE;
	m_bWantGroupNameSale = FALSE;
	m_bWantGroupNameDBase = FALSE;
	m_bWantACatNameSale = FALSE;
	m_bWantACatNameDBase = FALSE;
	m_bWantTaxBandName = FALSE;
	m_bWantPriceBandName = FALSE;
	m_bWantPromoName = FALSE;
	m_bWantMixTypeName = FALSE;
	m_bWantMixMatchName = FALSE;
	m_bWantTableName = FALSE;
	m_bWantTableGroupName = FALSE;

	m_bGotVoidLineType = FALSE;
	m_bGotDiscountLineType = FALSE;

	m_arrayConsolidationFields.RemoveAll();
	m_arrayOutputFields.RemoveAll();

	CReportConsolidationArray<CSortedIntItem> arrayUsedConsolidation;
	CReportConsolidationArray<CSortedIntItem> arrayUsedOutput;

	bool bGotLoc = FALSE;
	for (int n = 0; n < arrayFields.GetSize(); n++)
	{
		int nFieldType = arrayFields.GetAt(n);

		if (POSTRAY_EXPORTFIELD_DATE_BUS == nFieldType)
		{
			if (DataManagerNonDb.SessionDateTimeFilter.GetCalendarDateOnlyFlag() == TRUE)
			{
				nFieldType = POSTRAY_EXPORTFIELD_DATE_CAL;
			}
		}

		if (IsAcceptedField(nFieldType) == FALSE)
			continue;

		CSortedIntItem item;
		item.m_nItem = nFieldType;

		int nPos;
		if (arrayUsedOutput.Find(item, nPos) == TRUE)
			continue;

		arrayUsedOutput.Consolidate(item);

		bool bAllowOutputField = TRUE;

		int nConsolidationField = -1;
		switch (nFieldType)
		{
		case POSTRAY_EXPORTFIELD_PLUNO:
		case POSTRAY_EXPORTFIELD_REPTEXT_BASE:
		case POSTRAY_EXPORTFIELD_REPTEXT_MOD:
		case POSTRAY_EXPORTFIELD_SUPPREF:
		case POSTRAY_EXPORTFIELD_SORTCODE:
		case POSTRAY_EXPORTFIELD_STOCKCODE:
			nConsolidationField = POSTRAY_EXPORTFIELD_PLUNO;
			break;

		case POSTRAY_EXPORTFIELD_SERVER_TRAN:
		case POSTRAY_EXPORTFIELD_SRVNAME_TRAN:
			nConsolidationField = POSTRAY_EXPORTFIELD_SERVER_TRAN;
			break;

		case POSTRAY_EXPORTFIELD_SERVER_ITEM:
		case POSTRAY_EXPORTFIELD_SRVNAME_ITEM:
			nConsolidationField = POSTRAY_EXPORTFIELD_SERVER_ITEM;
			break;

		case POSTRAY_EXPORTFIELD_SERVER_AUTH:
		case POSTRAY_EXPORTFIELD_SRVNAME_AUTH:
			nConsolidationField = POSTRAY_EXPORTFIELD_SERVER_AUTH;
			break;

		case POSTRAY_EXPORTFIELD_DPTNO_DBASE:
		case POSTRAY_EXPORTFIELD_DPTNAME_DBASE:
			nConsolidationField = POSTRAY_EXPORTFIELD_DPTNO_DBASE;
			break;

		case POSTRAY_EXPORTFIELD_DPTNO_SALE:
		case POSTRAY_EXPORTFIELD_DPTNAME_SALE:
			nConsolidationField = POSTRAY_EXPORTFIELD_DPTNO_SALE;
			break;

		case POSTRAY_EXPORTFIELD_GRPNO_DBASE:
		case POSTRAY_EXPORTFIELD_GRPNAME_DBASE:
			nConsolidationField = POSTRAY_EXPORTFIELD_GRPNO_DBASE;
			break;

		case POSTRAY_EXPORTFIELD_GRPNO_SALE:
		case POSTRAY_EXPORTFIELD_GRPNAME_SALE:
			nConsolidationField = POSTRAY_EXPORTFIELD_GRPNO_SALE;
			break;

		case POSTRAY_EXPORTFIELD_ACATNO_DBASE:
		case POSTRAY_EXPORTFIELD_ACATNAME_DBASE:
			nConsolidationField = POSTRAY_EXPORTFIELD_ACATNO_DBASE;
			break;

		case POSTRAY_EXPORTFIELD_ACATNO_SALE:
		case POSTRAY_EXPORTFIELD_ACATNAME_SALE:
			nConsolidationField = POSTRAY_EXPORTFIELD_ACATNO_SALE;
			break;

		case POSTRAY_EXPORTFIELD_TAXNO:
		case POSTRAY_EXPORTFIELD_TAXNAME:
			nConsolidationField = POSTRAY_EXPORTFIELD_TAXNO;
			break;

		case POSTRAY_EXPORTFIELD_PBANDNO:
		case POSTRAY_EXPORTFIELD_PBANDNAME:
			nConsolidationField = POSTRAY_EXPORTFIELD_PBANDNO;
			break;

		case POSTRAY_EXPORTFIELD_MODNO:
		case POSTRAY_EXPORTFIELD_MODNAME:
			nConsolidationField = POSTRAY_EXPORTFIELD_MODNO;
			break;

		case POSTRAY_EXPORTFIELD_MONTHNO:
		case POSTRAY_EXPORTFIELD_MONTHNAME:
			nConsolidationField = POSTRAY_EXPORTFIELD_MONTHNO;
			break;

		case POSTRAY_EXPORTFIELD_DBNO:
		case POSTRAY_EXPORTFIELD_DBNAME:
			nConsolidationField = POSTRAY_EXPORTFIELD_DBNO;
			break;

		case POSTRAY_EXPORTFIELD_LOCNO:
		case POSTRAY_EXPORTFIELD_LOCNAME:
			nConsolidationField = POSTRAY_EXPORTFIELD_LOCNO;
			bGotLoc = TRUE;
			break;

		case POSTRAY_EXPORTFIELD_RPNO:
		case POSTRAY_EXPORTFIELD_RPNAME:
			nConsolidationField = POSTRAY_EXPORTFIELD_RPNO;
			break;

		case POSTRAY_EXPORTFIELD_TBLGRPNO:
		case POSTRAY_EXPORTFIELD_TBLGRPNAME:
			nConsolidationField = POSTRAY_EXPORTFIELD_TBLGRPNO;

			if (FALSE == bGotLoc)
			{
				bAllowOutputField = FALSE;
			}
			break;

		case POSTRAY_EXPORTFIELD_TBLNO:
		case POSTRAY_EXPORTFIELD_TBLNAME:
			nConsolidationField = POSTRAY_EXPORTFIELD_TBLNO;

			if (FALSE == bGotLoc)
			{
				bAllowOutputField = FALSE;
			}
			break;


		case POSTRAY_EXPORTFIELD_WEEKDAYNO:
		case POSTRAY_EXPORTFIELD_WEEKDAYNAME:
			nConsolidationField = POSTRAY_EXPORTFIELD_WEEKDAYNO;
			break;

		case POSTRAY_EXPORTFIELD_PAYNO:
		case POSTRAY_EXPORTFIELD_PAYNAME:
			nConsolidationField = POSTRAY_EXPORTFIELD_PAYNO;
			break;

		case POSTRAY_EXPORTFIELD_PROMONO:
		case POSTRAY_EXPORTFIELD_PROMONAME:
			nConsolidationField = POSTRAY_EXPORTFIELD_PROMONO;
			break;

		case POSTRAY_EXPORTFIELD_MIXMATCHNO:
		case POSTRAY_EXPORTFIELD_MIXMATCHNAME:
			nConsolidationField = POSTRAY_EXPORTFIELD_MIXMATCHNO;
			break;

		case POSTRAY_EXPORTFIELD_MIXTYPENO:
		case POSTRAY_EXPORTFIELD_MIXTYPENAME:
			nConsolidationField = POSTRAY_EXPORTFIELD_MIXTYPENO;
			break;

		case POSTRAY_EXPORTFIELD_DATE_CAL:
		case POSTRAY_EXPORTFIELD_DATE_BUS:
		case POSTRAY_EXPORTFIELD_DATE_WEEK:
		case POSTRAY_EXPORTFIELD_YEARNO:
		case POSTRAY_EXPORTFIELD_TIME_TRAN:
		case POSTRAY_EXPORTFIELD_TIME_SALE:
		case POSTRAY_EXPORTFIELD_TIME_DISCOUNT:
		case POSTRAY_EXPORTFIELD_TIME_VOID:
		case POSTRAY_EXPORTFIELD_TRMNO:
		case POSTRAY_EXPORTFIELD_WEEKNO:
		case POSTRAY_EXPORTFIELD_DISCOUNT_LINE_TYPE:
		case POSTRAY_EXPORTFIELD_DISCOUNT_MODE:
		case POSTRAY_EXPORTFIELD_DISCOUNT_USAGE:
		case POSTRAY_EXPORTFIELD_DISCOUNT_TYPE:
		case POSTRAY_EXPORTFIELD_DISCOUNT_RATE:
		case POSTRAY_EXPORTFIELD_VOID_SEQNO:
		case POSTRAY_EXPORTFIELD_VOID_TABLENO:
		case POSTRAY_EXPORTFIELD_VOID_BEFOREAFTER:
		case POSTRAY_EXPORTFIELD_VOID_REASON:
		case POSTRAY_EXPORTFIELD_VOID_LINE_TYPE:
		case POSTRAY_EXPORTFIELD_TRAN_SEQNO:
			nConsolidationField = nFieldType;
			break;
		}

		if (TRUE == bAllowOutputField)
		{
			m_arrayOutputFields.Add(nFieldType);

			if (nConsolidationField != -1)
			{
				CSortedIntItem item;
				item.m_nItem = nConsolidationField;

				int nPos;
				if (arrayUsedConsolidation.Find(item, nPos) == FALSE)
				{
					arrayUsedConsolidation.Consolidate(item);
					m_arrayConsolidationFields.Add(nConsolidationField);
				}
			}
		}

		switch (nFieldType)
		{
		case POSTRAY_EXPORTFIELD_MODNAME:			m_bWantModName = TRUE;			break;
		case POSTRAY_EXPORTFIELD_DPTNAME_SALE:		m_bWantDeptNameSale = TRUE;		break;
		case POSTRAY_EXPORTFIELD_DPTNAME_DBASE:		m_bWantDeptNameDBase = TRUE;	break;
		case POSTRAY_EXPORTFIELD_GRPNAME_SALE:		m_bWantGroupNameSale = TRUE;	break;
		case POSTRAY_EXPORTFIELD_GRPNAME_DBASE:		m_bWantGroupNameDBase = TRUE;	break;
		case POSTRAY_EXPORTFIELD_ACATNAME_SALE:		m_bWantACatNameSale = TRUE;		break;
		case POSTRAY_EXPORTFIELD_ACATNAME_DBASE:	m_bWantACatNameDBase = TRUE;	break;
		case POSTRAY_EXPORTFIELD_TAXNAME:			m_bWantTaxBandName = TRUE;		break;
		case POSTRAY_EXPORTFIELD_PBANDNAME:			m_bWantPriceBandName = TRUE;	break;
		case POSTRAY_EXPORTFIELD_PROMONAME:			m_bWantPromoName = TRUE;		break;
		case POSTRAY_EXPORTFIELD_MIXTYPENAME:		m_bWantMixTypeName = TRUE;		break;
		case POSTRAY_EXPORTFIELD_MIXMATCHNAME:		m_bWantMixMatchName = TRUE;		break;
		case POSTRAY_EXPORTFIELD_TBLGRPNAME:		m_bWantTableGroupName = TRUE;	break;
		case POSTRAY_EXPORTFIELD_TBLNAME:			m_bWantTableName = TRUE;		break;
		case POSTRAY_EXPORTFIELD_VOID_LINE_TYPE:	m_bGotVoidLineType = TRUE;		break;
		case POSTRAY_EXPORTFIELD_DISCOUNT_LINE_TYPE:m_bGotDiscountLineType = TRUE;	break;
		}
	}

	/*****/
	/*****/

	m_bWantPluInfo = FALSE;
	m_bWantDeptInfo = FALSE;
	m_bWantDeptGrpSale = FALSE;
	m_bWantTranServer = FALSE;
	m_bWantItemServer = FALSE;
	m_bWantAuthServer = FALSE;
	m_bWantCustomer = FALSE;
	m_bWantTax = FALSE;
	m_bWantCost = FALSE;
	m_bWantWaste = FALSE;
	m_bWantSales = FALSE;
	m_bWantFinalTime = FALSE;
	m_bWantVoidTime = FALSE;
	m_bWantTable = FALSE;

	for (int n = 0; n < m_arrayConsolidationFields.GetSize(); n++)
	{
		switch (m_arrayConsolidationFields.GetAt(n))
		{
		case POSTRAY_EXPORTFIELD_GRPNO_SALE:
			m_bWantDeptGrpSale = TRUE;
			break;

		case POSTRAY_EXPORTFIELD_DPTNO_DBASE:
		case POSTRAY_EXPORTFIELD_ACATNO_SALE:
		case POSTRAY_EXPORTFIELD_ACATNO_DBASE:
		case POSTRAY_EXPORTFIELD_TAXNO:
			m_bWantPluInfo = TRUE;
			break;

		case POSTRAY_EXPORTFIELD_GRPNO_DBASE:
			m_bWantPluInfo = TRUE;
			m_bWantDeptInfo = TRUE;
			break;

		case POSTRAY_EXPORTFIELD_SERVER_TRAN:
			m_bWantTranServer = TRUE;
			break;

		case POSTRAY_EXPORTFIELD_SERVER_ITEM:
			m_bWantItemServer = TRUE;
			break;

		case POSTRAY_EXPORTFIELD_SERVER_AUTH:
			m_bWantAuthServer = TRUE;
			break;

		case POSTRAY_EXPORTFIELD_TBLGRPNO:
			m_bWantTable = TRUE;
			break;
		}
	}

	for (int n = 0; n < m_arrayOutputFields.GetSize(); n++)
	{
		switch (m_arrayOutputFields.GetAt(n))
		{
		case POSTRAY_EXPORTFIELD_LINE_COUNT:
		case POSTRAY_EXPORTFIELD_QTY_BASE:
		case POSTRAY_EXPORTFIELD_QTY_MOD:
		case POSTRAY_EXPORTFIELD_VALUE_FULL:
		case POSTRAY_EXPORTFIELD_DISCOUNTPREMIUM:
		case POSTRAY_EXPORTFIELD_DISCOUNTONLY:
		case POSTRAY_EXPORTFIELD_PREMIUMONLY:
		case POSTRAY_EXPORTFIELD_VALUE_DISC:
			m_bWantSales = TRUE;
			break;

		case POSTRAY_EXPORTFIELD_TAX:
		case POSTRAY_EXPORTFIELD_VALUE_NET:
			m_bWantTax = TRUE;
			m_bWantPluInfo = TRUE;
			m_bWantSales = TRUE;
			break;

		case POSTRAY_EXPORTFIELD_COST:
			m_bWantPluInfo = TRUE;
			m_bWantCost = TRUE;
			m_bWantSales = TRUE;
			break;

		case POSTRAY_EXPORTFIELD_WASTE_COST:
			m_bWantPluInfo = TRUE;
			m_bWantCost = TRUE;
			m_bWantWaste = TRUE;
			break;

		case POSTRAY_EXPORTFIELD_WASTE_QTY_BASE:
		case POSTRAY_EXPORTFIELD_WASTE_QTY_MOD:
			m_bWantWaste = TRUE;
			break;

		case POSTRAY_EXPORTFIELD_TIME_TRAN:
			m_bWantFinalTime = TRUE;
			break;

		case POSTRAY_EXPORTFIELD_TIME_VOID:
			m_bWantVoidTime = TRUE;
			break;
		}
	}
}

/**********************************************************************/

bool CPosTrayExportConsolidator::PrepareFields( CEntityTree& Tree )
{
	bool bGotField = FALSE;

	for ( int n = 0; n < m_arrayConsolidationFields.GetSize(); n++ )
	{
		switch( m_arrayConsolidationFields.GetAt(n) )
		{
		case POSTRAY_EXPORTFIELD_PLUNO:
			Tree.AddField( SORT_TYPE_INT64 );
			bGotField = TRUE;
			break;

		case POSTRAY_EXPORTFIELD_SERVER_TRAN:
		case POSTRAY_EXPORTFIELD_SERVER_ITEM:
		case POSTRAY_EXPORTFIELD_SERVER_AUTH:
		case POSTRAY_EXPORTFIELD_TIME_TRAN:
		case POSTRAY_EXPORTFIELD_TIME_SALE:
		case POSTRAY_EXPORTFIELD_TIME_VOID:
		case POSTRAY_EXPORTFIELD_TIME_DISCOUNT:
		case POSTRAY_EXPORTFIELD_DISCOUNT_RATE:
		case POSTRAY_EXPORTFIELD_S4LABOUR_HOUR:
			Tree.AddField( SORT_TYPE_STRING );
			bGotField = TRUE;
			break;

		case POSTRAY_EXPORTFIELD_DBNO:
		case POSTRAY_EXPORTFIELD_LOCNO:
		case POSTRAY_EXPORTFIELD_RPNO:
		case POSTRAY_EXPORTFIELD_TRMNO:
		case POSTRAY_EXPORTFIELD_TBLGRPNO:
		case POSTRAY_EXPORTFIELD_TBLNO:
		case POSTRAY_EXPORTFIELD_MODNO:
		case POSTRAY_EXPORTFIELD_DPTNO_DBASE:
		case POSTRAY_EXPORTFIELD_DPTNO_SALE:
		case POSTRAY_EXPORTFIELD_GRPNO_DBASE:
		case POSTRAY_EXPORTFIELD_GRPNO_SALE:
		case POSTRAY_EXPORTFIELD_ACATNO_DBASE:
		case POSTRAY_EXPORTFIELD_ACATNO_SALE:
		case POSTRAY_EXPORTFIELD_TAXNO:
		case POSTRAY_EXPORTFIELD_PBANDNO:
		case POSTRAY_EXPORTFIELD_YEARNO:
		case POSTRAY_EXPORTFIELD_MONTHNO:
		case POSTRAY_EXPORTFIELD_WEEKNO:
		case POSTRAY_EXPORTFIELD_WEEKDAYNO:
		case POSTRAY_EXPORTFIELD_VOID_SEQNO:
		case POSTRAY_EXPORTFIELD_VOID_TABLENO:
		case POSTRAY_EXPORTFIELD_VOID_LINE_TYPE:
		case POSTRAY_EXPORTFIELD_VOID_BEFOREAFTER:
		case POSTRAY_EXPORTFIELD_DISCOUNT_LINE_TYPE:
		case POSTRAY_EXPORTFIELD_DISCOUNT_USAGE:
		case POSTRAY_EXPORTFIELD_DISCOUNT_MODE:
		case POSTRAY_EXPORTFIELD_DISCOUNT_TYPE:
		case POSTRAY_EXPORTFIELD_PROMONO:
		case POSTRAY_EXPORTFIELD_MIXMATCHNO:
		case POSTRAY_EXPORTFIELD_MIXTYPENO:
		case POSTRAY_EXPORTFIELD_PAYNO:
		case POSTRAY_EXPORTFIELD_TRAN_SEQNO:
		case POSTRAY_EXPORTFIELD_DUMMY:
			Tree.AddField( SORT_TYPE_INT );
			bGotField = TRUE;
			break;

		case POSTRAY_EXPORTFIELD_DATE_CAL:
		case POSTRAY_EXPORTFIELD_DATE_BUS:
		case POSTRAY_EXPORTFIELD_DATE_WEEK:
			Tree.AddField( SORT_TYPE_DATE );
			bGotField = TRUE;
			break;
		}
	}

	return bGotField;
}

/**********************************************************************/

void CPosTrayExportConsolidator::SetDates ( COleDateTime dateFrom, COleDateTime dateTo, CSessionCSVRecord& ReportSession, CTimeSliceMapCSVRecord& TimeSlice )
{
	DataManagerNonDb.SessionDateTimeFilter.SetDates( dateFrom, dateTo, ReportSession );
	m_bEODMode = ReportSession.GetEODFlag();

	m_TimeSliceMap.SetReportType( TimeSlice.GetReportType() );
	m_TimeSliceMap.SetTimeSliceFileNo( TimeSlice.GetTimeSliceFileNo() );

	if ( m_TimeSliceMap.GetReportType() == SH_CUSTOM )
		m_TimeMap.BuildMap( m_TimeSliceMap.GetTimeSliceFileNo() );
}

/**********************************************************************/

void CPosTrayExportConsolidator::SetTableFilter( CTableFilter info )
{
	m_TableFilter = info;
	m_TableReportHelpers.SetLocIdx(-1);
}

/**********************************************************************/

void CPosTrayExportConsolidator::ShowProgress( __int64 nProgress, __int64 nTarget ) 
{
#ifdef POSTRAY_UTILITY

	StatusProgress.SetPos ( nProgress, nTarget  );
	
#else
	
	CPosTrayManualExportDlg* pDlg = (CPosTrayManualExportDlg*) m_pHostDialog;
	pDlg -> UpdateFileProgressPos( nProgress, nTarget );

#endif
}

/**********************************************************************/

void CPosTrayExportConsolidator::ExpandTimeString( CString& strTime )
{
	switch( strTime.GetLength() )
	{
	case 6:
		break;

	case 4:
		strTime += "00";
		break;

	default:
		strTime = "000000";
		break;
	}
}

/**********************************************************************/

void CPosTrayExportConsolidator::CreateTimeLabel( CString& strTime, bool bSortLabel )
{
	if ( strTime.GetLength() == 6 )
	{
		int nHour = atoi( strTime.Left(2) );
		int nMinute = atoi( strTime.Mid(2,2) );
		int nSecond = atoi( strTime.Right(2) ); 
					
		//ROUND TO TIME SLICE IF REQUIRED
		switch( m_TimeSliceMap.GetReportType() )
		{
		case SH_TIMESLICE_SECOND:
			break;

		case SH_TIMESLICE_MINUTE:
			nSecond = 0;
			break;

		case SH_TIMESLICE_5MIN:
			nSecond = 0;
			nMinute -= ( nMinute % 5 );
			break;

		case SH_TIMESLICE_10MIN:
			nSecond = 0;
			nMinute -= ( nMinute % 10 );
			break;

		case SH_TIMESLICE_15MIN:
			nSecond = 0;
			nMinute -= ( nMinute % 15 );
			break;

		case SH_TIMESLICE_30MIN:
			nSecond = 0;
			nMinute -= ( nMinute % 30 );
			break;

		case SH_TIMESLICE_60MIN:
			nSecond = 0;
			nMinute = 0;
			break;
		}

		if ( m_TimeSliceMap.GetReportType() == SH_CUSTOM )
			strTime = m_TimeMap.GetSortOrder( nHour, nMinute );
		else
		{
			int nSortMode = 0;
			if ( TRUE == bSortLabel )
				nSortMode = m_BusinessDateInfo.GetTimeSortType() - 1;
			
			strTime.Format( "%1.1d%2.2d%2.2d%2.2d",
				nSortMode,			
				nHour,
				nMinute,
				nSecond );
		}
	}
}

/**********************************************************************/

__int64 CPosTrayExportConsolidator::CreateExport( CEntityTree* pEntityTree, const char* szProgressText, const char* szFilename, bool& bResult, bool bIsDiscountExport)
{
	__int64 nLineCount = 0;

	bResult = FALSE;

	CSSFile fileExport;
	if ( fileExport.Open( szFilename, "wb" ) == TRUE )
	{
		if ( NULL == m_pHostDialog )
		{
			StatusProgress.Lock( TRUE, szProgressText );
			nLineCount = CreateExportInternal( pEntityTree, fileExport, bIsDiscountExport );
			StatusProgress.Unlock();
		}
		else
		{
			nLineCount = CreateExportInternal( pEntityTree, fileExport, bIsDiscountExport );
		}

		bResult = TRUE;
	}
	
	return nLineCount;
}

/**********************************************************************/

__int64 CPosTrayExportConsolidator::CreateExportInternal( CEntityTree* pEntityTree, CSSFile& fileExport, bool bIsDiscountExport )
{
	if ( WantHeader() == TRUE )
	{
		CCSV csv;
		for ( int nPos = 0; nPos < m_arrayOutputFields.GetSize(); nPos++ )
		{
			CPosTrayExportFieldName Name;
			Name.m_nFieldType = m_arrayOutputFields.GetAt( nPos );
			DataManagerNonDb.PosTrayExportFieldNames.GetNameInfo( Name );

			CString strField = "";
			if (POSTRAY_EXPORTFIELD_DISCOUNTPREMIUM == Name.m_nFieldType)
			{
				CCSV csvField(Name.GetHeaderNameForExport());

				if (FALSE == bIsDiscountExport)
				{
					strField = csvField.GetString(0);
				}
				else
				{
					strField = csvField.GetString(1);
					if (strField == "")
					{
						strField = "Amount";
					}
				}
			}
			else
			{
				strField = Name.GetHeaderNameForExport();
			}

			csv.Add( strField );			
		}
		fileExport.WriteLine( csv.GetLine() );
	}

	__int64 nLineCount = 1;

	m_nProgress = 0;
	ProcessTreeBranch( pEntityTree, 0, 0, fileExport, nLineCount );
	
	return nLineCount;
}

/**********************************************************************/

void CPosTrayExportConsolidator::ProcessTreeBranch( CEntityTree* pEntityTree, int nDepth, int nBranchIdx, CSSFile& fileExport, __int64& nLineCount )
{
	int nFieldType = m_arrayConsolidationFields.GetAt( nDepth );	
	
	int nBranchSize = pEntityTree -> GetBranchSize(nBranchIdx);
	if ( nBranchSize == 0 )
		return;

	for ( int nLeafIdx = 0; ( nLeafIdx < nBranchSize ) && ( FALSE == m_bQuitNow ); nLeafIdx++ )
	{
		ShowProgress( m_nProgress++, pEntityTree -> GetNodeCount() );
		
		int nLineDbNo = -1;

		switch ( nFieldType )
		{
		case POSTRAY_EXPORTFIELD_DBNO:
			{
				int nSortIdx = pEntityTree -> GetLabelInt( nBranchIdx, nLeafIdx );
				int nDbIdx = dbDatabase.GetDbIdxFromSortIdx( nSortIdx );
		
				if ( ( nDbIdx >= 0 ) && ( nDbIdx < dbDatabase.GetSize() ) )
				{
					m_LineInfo.m_strDbName = dbDatabase.GetName( nDbIdx );
					m_LineInfo.m_nDbNo = dbDatabase.GetDbNo( nDbIdx );
					nLineDbNo = m_LineInfo.m_nDbNo;
				}
				else
				{
					m_LineInfo.m_strDbName = "";
					m_LineInfo.m_nDbNo = 0;
				}
			}
			break;

		case POSTRAY_EXPORTFIELD_LOCNO:
			{
				int nSortIdx = pEntityTree -> GetLabelInt( nBranchIdx, nLeafIdx );
				int nLocIdx = UseDirectLocIdx() ? nSortIdx : dbLocation.GetLocIdxFromSortIdx( nSortIdx );

				if ( TRUE == m_bWantTableName || m_bWantTableGroupName )
				{
					m_TableReportHelpers.SetLocIdx( nLocIdx );
					m_TableReportHelpers.BuildGroupMap( m_TableFilter );
				}

				m_LineInfo.m_nLocNo = 0;

				switch( nLocIdx )
				{
				case LOCIDX_SPECIAL_UNKNOWN:	m_LineInfo.m_strLocName = "";			break;
				default:		
					{
						m_LineInfo.m_nLocNo = dbLocation.GetNetworkLocNo( nLocIdx );
						m_LineInfo.m_strLocName = dbLocation.GetName( nLocIdx ); 
						nLineDbNo = dbLocation.GetDbNo( nLocIdx );
					}
					break;
				}
			}
			break;

		case POSTRAY_EXPORTFIELD_RPNO:
			{
				int nRepPoint = pEntityTree -> GetLabelInt( nBranchIdx, nLeafIdx );
				
				if ( ( nRepPoint < 1 ) || ( nRepPoint > dbReportpoint.GetSize() ) )
				{
					m_LineInfo.m_nRpNo = 0;
					m_LineInfo.m_strRpName = "";
				}
				else
				{
					int nRpIdx = dbReportpoint.GetRpIdxFromSortIdx( nRepPoint - 1 );
					m_LineInfo.m_nRpNo = dbReportpoint.GetRpNo( nRpIdx );
					m_LineInfo.m_strRpName = dbReportpoint.GetName( nRpIdx );
				}
			}
			break;

		case POSTRAY_EXPORTFIELD_TRMNO:
			m_LineInfo.m_nTrmNo = pEntityTree -> GetLabelInt( nBranchIdx, nLeafIdx );
			break;

		case POSTRAY_EXPORTFIELD_TBLGRPNO:
			m_LineInfo.m_nTableGrpNo = pEntityTree -> GetLabelInt( nBranchIdx, nLeafIdx );

			if ( TRUE == m_bWantTableGroupName )
			{
				m_LineInfo.m_strTableGrpName = m_TableReportHelpers.GetGroupName( m_LineInfo.m_nTableGrpNo );
			}
			break;

		case POSTRAY_EXPORTFIELD_TBLNO:
			m_LineInfo.m_nTableNo = pEntityTree -> GetLabelInt( nBranchIdx, nLeafIdx );

			if ( TRUE == m_bWantTableName )
			{
				m_LineInfo.m_strTableName = m_TableReportHelpers.GetTableName( m_LineInfo.m_nTableNo );
			}

			break;

		case POSTRAY_EXPORTFIELD_DATE_BUS:
			{	
				COleDateTime oleDate = pEntityTree -> GetLabelDate( nBranchIdx, nLeafIdx );
				m_LineInfo.m_strBusinessDate = GetExportDate( oleDate );
			}
			break;

		case POSTRAY_EXPORTFIELD_DATE_CAL:
			{	
				COleDateTime oleDate = pEntityTree -> GetLabelDate( nBranchIdx, nLeafIdx );
				m_LineInfo.m_strCalendarDate = GetExportDate( oleDate );
			}
			break;

		case POSTRAY_EXPORTFIELD_YEARNO:
			m_LineInfo.m_nYear = pEntityTree -> GetLabelInt( nBranchIdx, nLeafIdx );
			break;

		case POSTRAY_EXPORTFIELD_MONTHNO:
			m_LineInfo.m_nMonthNo = pEntityTree -> GetLabelInt( nBranchIdx, nLeafIdx );
			break;

		case POSTRAY_EXPORTFIELD_DATE_WEEK:
			{	
				COleDateTime oleWeekStart = pEntityTree -> GetLabelDate( nBranchIdx, nLeafIdx );
				m_LineInfo.m_strWeekStartDate = GetExportDate( oleWeekStart );
			}
			break;

		case POSTRAY_EXPORTFIELD_WEEKNO:
			m_LineInfo.m_nWeekNo = pEntityTree -> GetLabelInt( nBranchIdx, nLeafIdx );
			break;

		case POSTRAY_EXPORTFIELD_WEEKDAYNO:
			{
				int nWeekday = pEntityTree -> GetLabelInt( nBranchIdx, nLeafIdx );
				nWeekday += EcrmanOptions.GetReportsWeekStartDay();
				nWeekday %= 7;
				m_LineInfo.m_strWeekdayName = m_strWeekDays [ nWeekday ];
				m_LineInfo.m_nWeekdayNo = nWeekday + 1;
			}
			break;

		case POSTRAY_EXPORTFIELD_TIME_TRAN:
			{
				CString strLabel = pEntityTree -> GetLabelString( nBranchIdx, nLeafIdx );

				if ( m_TimeSliceMap.GetReportType() != SH_CUSTOM )
				{
					m_LineInfo.m_nFinalTimeHour = atoi( strLabel.Mid(1,2) );
					m_LineInfo.m_nFinalTimeMinute = atoi( strLabel.Mid(3,2) );
					m_LineInfo.m_nFinalTimeSecond = atoi( strLabel.Right(2) );
					m_LineInfo.m_nFinalTimeStars = atoi( strLabel.Left(1) );
				}
				else
					m_LineInfo.m_nFinalTimeHour = atoi( strLabel );
			}
			break;

		case POSTRAY_EXPORTFIELD_TIME_SALE:
		case POSTRAY_EXPORTFIELD_TIME_DISCOUNT:
		case POSTRAY_EXPORTFIELD_TIME_VOID:
			{
				CString strLabel = pEntityTree -> GetLabelString( nBranchIdx, nLeafIdx );

				if ( m_TimeSliceMap.GetReportType() != SH_CUSTOM )
				{
					m_LineInfo.m_nActionTimeHour = atoi( strLabel.Mid(1,2) );
					m_LineInfo.m_nActionTimeMinute = atoi( strLabel.Mid(3,2) );
					m_LineInfo.m_nActionTimeSecond = atoi( strLabel.Right(2) );
					m_LineInfo.m_nActionTimeStars = atoi( strLabel.Left(1) );
				}
				else
					m_LineInfo.m_nActionTimeHour = atoi( strLabel );
			}
			break;

		case POSTRAY_EXPORTFIELD_SERVER_TRAN:
			{
				CString strLabel = pEntityTree -> GetLabelString( nBranchIdx, nLeafIdx );
				DataManagerNonDb.ServerNameTable.ProcessServerLabel( strLabel, m_LineInfo.m_nTranServerNo, m_LineInfo.m_strTranServerName );
			}
			break;

		case POSTRAY_EXPORTFIELD_SERVER_ITEM:
			{
				CString strLabel = pEntityTree -> GetLabelString( nBranchIdx, nLeafIdx );
				DataManagerNonDb.ServerNameTable.ProcessServerLabel( strLabel, m_LineInfo.m_nItemServerNo, m_LineInfo.m_strItemServerName );
			}
			break;

		case POSTRAY_EXPORTFIELD_SERVER_AUTH:
			{
				CString strLabel = pEntityTree -> GetLabelString( nBranchIdx, nLeafIdx );
				DataManagerNonDb.ServerNameTable.ProcessServerLabel( strLabel, m_LineInfo.m_nAuthServerNo, m_LineInfo.m_strAuthServerName );
			}
			break;

		case POSTRAY_EXPORTFIELD_DPTNO_SALE:
			
			m_LineInfo.m_nDeptNoSale = pEntityTree -> GetLabelInt( nBranchIdx, nLeafIdx );
			if ( 1000 == m_LineInfo.m_nDeptNoSale ) m_LineInfo.m_nDeptNoSale = 0;

			if ( TRUE == m_bWantDeptNameSale ) 
				m_LineInfo.m_strDeptNameSale = DataManager.Department.GetReportTextByDeptNo( m_LineInfo.m_nDeptNoSale );

			break;

		case POSTRAY_EXPORTFIELD_DPTNO_DBASE:
			
			m_LineInfo.m_nDeptNoDBase = pEntityTree -> GetLabelInt( nBranchIdx, nLeafIdx );
			if ( 1000 == m_LineInfo.m_nDeptNoDBase ) m_LineInfo.m_nDeptNoDBase = 0;
			
			if ( TRUE == m_bWantDeptNameDBase ) 
				m_LineInfo.m_strDeptNameDBase = DataManager.Department.GetReportTextByDeptNo( m_LineInfo.m_nDeptNoDBase );
			
			break;

		case POSTRAY_EXPORTFIELD_GRPNO_SALE:
			
			m_LineInfo.m_nGroupNoSale = pEntityTree -> GetLabelInt( nBranchIdx, nLeafIdx );
			if ( 1000 == m_LineInfo.m_nGroupNoSale ) m_LineInfo.m_nGroupNoSale = 0;
			
			if ( TRUE == m_bWantGroupNameSale )
				m_LineInfo.m_strGroupNameSale = DataManager.EposGroup.GetGroupReportText( m_LineInfo.m_nGroupNoSale );
			
			break;

		case POSTRAY_EXPORTFIELD_GRPNO_DBASE:
			
			m_LineInfo.m_nGroupNoDBase = pEntityTree -> GetLabelInt( nBranchIdx, nLeafIdx );
			if ( 1000 == m_LineInfo.m_nGroupNoDBase ) m_LineInfo.m_nGroupNoDBase = 0;

			if ( TRUE == m_bWantGroupNameDBase )
				m_LineInfo.m_strGroupNameDBase = DataManager.EposGroup.GetGroupReportText( m_LineInfo.m_nGroupNoDBase );

			break;

		case POSTRAY_EXPORTFIELD_ACATNO_SALE:

			m_LineInfo.m_nACatNoSale = pEntityTree -> GetLabelInt( nBranchIdx, nLeafIdx );
			
			if ( TRUE == m_bWantACatNameSale )
				m_LineInfo.m_strACatNameSale = DataManager.AnalysisCategory.GetDisplayText( m_LineInfo.m_nACatNoSale );

			break;

		case POSTRAY_EXPORTFIELD_ACATNO_DBASE:
			
			m_LineInfo.m_nACatNoDBase = pEntityTree -> GetLabelInt( nBranchIdx, nLeafIdx );
			
			if ( TRUE == m_bWantACatNameDBase )
				m_LineInfo.m_strACatNameDBase = DataManager.AnalysisCategory.GetDisplayText( m_LineInfo.m_nACatNoDBase );
			
			break;

		case POSTRAY_EXPORTFIELD_PLUNO:
			m_LineInfo.m_nPluNo = pEntityTree -> GetLabelInt64( nBranchIdx, nLeafIdx );
			break;	

		case POSTRAY_EXPORTFIELD_MODNO:
			
			m_LineInfo.m_nModNo = pEntityTree -> GetLabelInt( nBranchIdx, nLeafIdx );
			
			if ( TRUE == m_bWantModName )
				m_LineInfo.m_strModName = DataManager.Modifier.GetDisplayName( m_LineInfo.m_nModNo );

			break;

		case POSTRAY_EXPORTFIELD_TAXNO:
			
			m_LineInfo.m_nTaxBand = pEntityTree -> GetLabelInt( nBranchIdx, nLeafIdx );
			
			if ( TRUE == m_bWantTaxBandName )
			{
				CTaxRateInfo infoTax;
				DataManager.TaxRates.GetTaxRateInfo( m_LineInfo.m_nTaxBand, infoTax );
				m_LineInfo.m_strTaxBandName = infoTax.m_strReportText;
			}
		
			break;

		case POSTRAY_EXPORTFIELD_PBANDNO:
			
			m_LineInfo.m_nPriceBand = pEntityTree -> GetLabelInt( nBranchIdx, nLeafIdx );

			if ( TRUE == m_bWantPriceBandName )
				m_LineInfo.m_strPriceBandName = DataManager.PriceText.GetReportText( m_LineInfo.m_nPriceBand - 1 );

			break;

		case POSTRAY_EXPORTFIELD_VOID_SEQNO:
			m_LineInfo.m_nVoidSeqNo = pEntityTree -> GetLabelInt( nBranchIdx, nLeafIdx );
			break;

		case POSTRAY_EXPORTFIELD_VOID_TABLENO:
			m_LineInfo.m_nVoidTableNo = pEntityTree -> GetLabelInt( nBranchIdx, nLeafIdx );
			break;

		case POSTRAY_EXPORTFIELD_VOID_LINE_TYPE:
			switch( pEntityTree -> GetLabelInt( nBranchIdx, nLeafIdx ) )
			{
			case 0:		m_LineInfo.m_strVoidLineType = "Sale";		break;
			case 1:		m_LineInfo.m_strVoidLineType = "Refund";	break;
			case 2:		m_LineInfo.m_strVoidLineType = "Discount";	break;
			case 3:		m_LineInfo.m_strVoidLineType = "Premium";	break;
			default:	m_LineInfo.m_strVoidLineType = "Unknown";	break;
			}
			break;

		case POSTRAY_EXPORTFIELD_VOID_BEFOREAFTER:
			m_LineInfo.m_strVoidBeforeAfter = ( pEntityTree -> GetLabelInt( nBranchIdx, nLeafIdx ) == 1 ) ? "After" : "Before";
			break;

		case POSTRAY_EXPORTFIELD_VOID_REASON:
			m_LineInfo.m_strVoidReason = pEntityTree -> GetLabelString( nBranchIdx, nLeafIdx );
			break;

		case POSTRAY_EXPORTFIELD_DISCOUNT_LINE_TYPE:
			switch( pEntityTree -> GetLabelInt( nBranchIdx, nLeafIdx ) )
			{
			case 0:		m_LineInfo.m_strDiscountLineType = "Discount";	break;
			case 1:		m_LineInfo.m_strDiscountLineType = "Premium";	break;
			default:	m_LineInfo.m_strDiscountLineType = "Unknown";	break;
			}
			break;

		case POSTRAY_EXPORTFIELD_DISCOUNT_MODE:
			switch( pEntityTree -> GetLabelInt( nBranchIdx, nLeafIdx ) )
			{
			case DISCOUNT_MODE_ITEM:		m_LineInfo.m_strDiscountMode = "Item";		break;
			case DISCOUNT_MODE_SUBTOTAL:	m_LineInfo.m_strDiscountMode = "Subtotal";	break;
			case DISCOUNT_MODE_LOYALTY:		m_LineInfo.m_strDiscountMode = "Loyalty";	break;
			case DISCOUNT_MODE_DIFFER:		m_LineInfo.m_strDiscountMode = "Differ";	break;
			case DISCOUNT_MODE_MIXMATCH:	m_LineInfo.m_strDiscountMode = "Mix Match";	break;
			default:						m_LineInfo.m_strDiscountMode = "Unknown";	break;
			}
			break;

		case POSTRAY_EXPORTFIELD_DISCOUNT_USAGE:
			switch( pEntityTree -> GetLabelInt( nBranchIdx, nLeafIdx ) )
			{
			case 0:		m_LineInfo.m_strDiscountUsage = "";			break;
			case 1:		m_LineInfo.m_strDiscountUsage = "Subtotal";	break;
			case 2:		m_LineInfo.m_strDiscountUsage = "Loyalty";	break;
			case 3:		m_LineInfo.m_strDiscountUsage = "Differ";	break;
			default:	m_LineInfo.m_strDiscountUsage = "Unknown";	break;
			}
			break;

		case POSTRAY_EXPORTFIELD_DISCOUNT_TYPE:
			switch( pEntityTree -> GetLabelInt( nBranchIdx, nLeafIdx ) )
			{
			case 0:		m_LineInfo.m_strDiscountType = "";			break;
			case 1:		m_LineInfo.m_strDiscountType = "Markdown";	break;
			case 2:		m_LineInfo.m_strDiscountType = "Discount";	break;
			default:	m_LineInfo.m_strDiscountType = "Unknown";	break;
			}
			break;

		case POSTRAY_EXPORTFIELD_DISCOUNT_RATE:
			m_LineInfo.m_strDiscountRate = pEntityTree -> GetLabelString( nBranchIdx, nLeafIdx );
			break;

		case POSTRAY_EXPORTFIELD_PROMONO:
			m_LineInfo.m_nPromoNo = pEntityTree -> GetLabelInt( nBranchIdx, nLeafIdx );

			if ( TRUE == m_bWantPromoName )
			{
				int nPromoIdx = 0;
				if ( 0 == m_LineInfo.m_nPromoNo )
				{
					m_LineInfo.m_strPromoName = "";
				}
				else if ( DataManager.Promotion.FindTableByNumber( m_LineInfo.m_nPromoNo, nPromoIdx ) == TRUE )
				{
					CPromotionsCSVRecord Promo;
					DataManager.Promotion.GetAt( nPromoIdx, Promo );
					m_LineInfo.m_strPromoName = Promo.GetDisplayName();
				}
				else
				{
					m_LineInfo.m_strPromoName.Format ( "Promo %d", m_LineInfo.m_nPromoNo );
				}                       
			}

			break;

		case POSTRAY_EXPORTFIELD_MIXTYPENO:
			m_LineInfo.m_nMixTypeNo = pEntityTree -> GetLabelInt( nBranchIdx, nLeafIdx );
		
			if ( TRUE == m_bWantMixTypeName )
			{
				m_LineInfo.m_strMixTypeName = "";
				::GetMixMatchOfferTypeName( m_LineInfo.m_nMixTypeNo, m_LineInfo.m_strMixTypeName, FALSE, "" );
			}
			break;

		case POSTRAY_EXPORTFIELD_MIXMATCHNO:
			m_LineInfo.m_nMixMatchNo = pEntityTree -> GetLabelInt( nBranchIdx, nLeafIdx );
			
			if ( TRUE == m_bWantMixMatchName )
			{
				int nMixMatchIdx = 0;
				if ( 0 == m_LineInfo.m_nMixMatchNo )
				{
					m_LineInfo.m_strMixMatchName = "";
				}
				else if ( DataManager.MixMatch.FindTableByNumber( m_LineInfo.m_nMixMatchNo, nMixMatchIdx ) == TRUE )
				{
					CMixMatchCSVRecord MixMatch;
					DataManager.MixMatch.GetAt( nMixMatchIdx, MixMatch );
					m_LineInfo.m_strMixMatchName = MixMatch.GetDisplayName();
				}
				else
				{
					m_LineInfo.m_strMixMatchName.Format ( "Mix Match %d", m_LineInfo.m_nMixMatchNo );
				}                       
			}

			break;

		case POSTRAY_EXPORTFIELD_PAYNO:
			m_LineInfo.m_nPaymentNo = pEntityTree -> GetLabelInt( nBranchIdx, nLeafIdx );
			m_LineInfo.m_strPaymentName = DataManager.Payment.GetDisplayName( m_LineInfo.m_nPaymentNo );
			break;

		case POSTRAY_EXPORTFIELD_TRAN_SEQNO:
			m_LineInfo.m_nTranSeqNo = pEntityTree -> GetLabelInt( nBranchIdx, nLeafIdx );
			break;

		case POSTRAY_EXPORTFIELD_S4LABOUR_HOUR:
			m_LineInfo.m_strS4LabourHour.Format( "REV%2.2d", atoi( pEntityTree -> GetLabelString( nBranchIdx, nLeafIdx ) ) + 1 );
			break;
		}

		if ( (nLineDbNo != -1) && ( nLineDbNo != dbDatabase.GetDbNo( DataManager.GetActiveDbIdx() ) ) )
		{
			int nDbIdx;
			if ( dbDatabase.FindDatabaseByNumber( nLineDbNo, nDbIdx ) == TRUE )
			{
				CDataManagerInfo info;
				DataManager.OpenDatabaseReadOnly( nDbIdx, info, FALSE );
			}
		}

		if ( nDepth == m_arrayConsolidationFields.GetSize() - 1 )
		{
			GetNodeTotals( nBranchIdx, nLeafIdx );
			CreateExportLine( fileExport );
			nLineCount++;
		}
		else
		{
			ProcessTreeBranch( pEntityTree, nDepth + 1, pEntityTree -> GetNextBranchIdx( nBranchIdx, nLeafIdx ), fileExport, nLineCount );
		}
	}
}

/**********************************************************************/

void CPosTrayExportConsolidator::CreateExportLine( CSSFile& fileExport )
{
	CCSV csv;

	CReportPluInfoFinder PluInfoFinder;

	for ( int n = 0; n < m_arrayOutputFields.GetSize(); n++ )
	{
		switch( m_arrayOutputFields.GetAt(n) )
		{	
		case POSTRAY_EXPORTFIELD_DBNO:					csv.Add( m_LineInfo.m_nDbNo );										break;
		case POSTRAY_EXPORTFIELD_LOCNO:					csv.Add( m_LineInfo.m_nLocNo );										break;
		case POSTRAY_EXPORTFIELD_RPNO:					csv.Add( m_LineInfo.m_nRpNo );										break;
		case POSTRAY_EXPORTFIELD_TRMNO:					csv.Add( m_LineInfo.m_nTrmNo );										break;
		case POSTRAY_EXPORTFIELD_TBLGRPNO:				csv.Add( m_LineInfo.m_nTableGrpNo );								break;
		case POSTRAY_EXPORTFIELD_TBLNO:					csv.Add( m_LineInfo.m_nTableNo );									break;
		case POSTRAY_EXPORTFIELD_DATE_CAL:				csv.Add( m_LineInfo.m_strCalendarDate );							break;
		case POSTRAY_EXPORTFIELD_DATE_BUS:				csv.Add( m_LineInfo.m_strBusinessDate );							break;
		case POSTRAY_EXPORTFIELD_YEARNO:				csv.Add( m_LineInfo.m_nYear );										break;
		case POSTRAY_EXPORTFIELD_MONTHNO:				csv.Add( m_LineInfo.m_nMonthNo );									break;
		case POSTRAY_EXPORTFIELD_DATE_WEEK:				csv.Add( m_LineInfo.m_strWeekStartDate );							break;
		case POSTRAY_EXPORTFIELD_WEEKNO:				csv.Add( m_LineInfo.m_nWeekNo );									break;
		case POSTRAY_EXPORTFIELD_WEEKDAYNO:				csv.Add( m_LineInfo.m_nWeekdayNo );									break;
		case POSTRAY_EXPORTFIELD_TIME_TRAN:				csv.Add( GetLabelTimeString( TRUE ) );								break;
		case POSTRAY_EXPORTFIELD_TIME_SALE:
		case POSTRAY_EXPORTFIELD_TIME_DISCOUNT:
		case POSTRAY_EXPORTFIELD_TIME_VOID:				csv.Add( GetLabelTimeString( FALSE ) );								break;
		case POSTRAY_EXPORTFIELD_PLUNO:					csv.Add( m_LineInfo.m_nPluNo );										break;
		case POSTRAY_EXPORTFIELD_MODNO:					csv.Add( m_LineInfo.m_nModNo );										break;
		case POSTRAY_EXPORTFIELD_DPTNO_DBASE:			csv.Add( m_LineInfo.m_nDeptNoDBase );								break;
		case POSTRAY_EXPORTFIELD_DPTNO_SALE:			csv.Add( m_LineInfo.m_nDeptNoSale );								break;	
		case POSTRAY_EXPORTFIELD_GRPNO_DBASE:			csv.Add( m_LineInfo.m_nGroupNoDBase );								break;
		case POSTRAY_EXPORTFIELD_GRPNO_SALE:			csv.Add( m_LineInfo.m_nGroupNoSale );								break;
		case POSTRAY_EXPORTFIELD_ACATNO_DBASE:			csv.Add( m_LineInfo.m_nACatNoDBase );								break;
		case POSTRAY_EXPORTFIELD_ACATNO_SALE:			csv.Add( m_LineInfo.m_nACatNoSale );								break;
		case POSTRAY_EXPORTFIELD_SERVER_TRAN:			csv.Add( m_LineInfo.m_nTranServerNo );								break;
		case POSTRAY_EXPORTFIELD_SERVER_ITEM:			csv.Add( m_LineInfo.m_nItemServerNo );								break;
		case POSTRAY_EXPORTFIELD_SERVER_AUTH:			csv.Add( m_LineInfo.m_nAuthServerNo );								break;
		case POSTRAY_EXPORTFIELD_TAXNO:					csv.Add( m_LineInfo.m_nTaxBand );									break;
		case POSTRAY_EXPORTFIELD_PBANDNO:				csv.Add( m_LineInfo.m_nPriceBand );									break;
		case POSTRAY_EXPORTFIELD_PAYNO:					csv.Add( m_LineInfo.m_nPaymentNo );									break;
		/*****/
		case POSTRAY_EXPORTFIELD_DBNAME:				csv.Add( m_LineInfo.m_strDbName );									break;
		case POSTRAY_EXPORTFIELD_LOCNAME:				csv.Add( m_LineInfo.m_strLocName );									break;
		case POSTRAY_EXPORTFIELD_RPNAME:				csv.Add( m_LineInfo.m_strRpName );									break;
		case POSTRAY_EXPORTFIELD_TBLGRPNAME:			csv.Add( m_LineInfo.m_strTableGrpName );							break;
		case POSTRAY_EXPORTFIELD_TBLNAME:				csv.Add( m_LineInfo.m_strTableName );								break;
		case POSTRAY_EXPORTFIELD_MONTHNAME:				csv.Add( GetRepMonth( m_LineInfo.m_nMonthNo ) );					break;
		case POSTRAY_EXPORTFIELD_WEEKDAYNAME:			csv.Add( m_LineInfo.m_strWeekdayName );								break;
		case POSTRAY_EXPORTFIELD_MODNAME:				csv.Add( m_LineInfo.m_strModName );									break;
		case POSTRAY_EXPORTFIELD_DPTNAME_DBASE:			csv.Add( m_LineInfo.m_strDeptNameDBase );							break;
		case POSTRAY_EXPORTFIELD_DPTNAME_SALE:			csv.Add( m_LineInfo.m_strDeptNameSale );							break;
		case POSTRAY_EXPORTFIELD_GRPNAME_DBASE:			csv.Add( m_LineInfo.m_strGroupNameDBase );							break;
		case POSTRAY_EXPORTFIELD_GRPNAME_SALE:			csv.Add( m_LineInfo.m_strGroupNameSale );							break;
		case POSTRAY_EXPORTFIELD_ACATNAME_DBASE:		csv.Add( m_LineInfo.m_strACatNameDBase );							break;
		case POSTRAY_EXPORTFIELD_ACATNAME_SALE:			csv.Add( m_LineInfo.m_strACatNameSale );							break;
		case POSTRAY_EXPORTFIELD_SRVNAME_TRAN:			csv.Add( m_LineInfo.m_strTranServerName );							break;
		case POSTRAY_EXPORTFIELD_SRVNAME_ITEM:			csv.Add( m_LineInfo.m_strItemServerName );							break;
		case POSTRAY_EXPORTFIELD_SRVNAME_AUTH:			csv.Add( m_LineInfo.m_strAuthServerName );							break;
		case POSTRAY_EXPORTFIELD_TAXNAME:				csv.Add( m_LineInfo.m_strTaxBandName );								break;
		case POSTRAY_EXPORTFIELD_PBANDNAME:				csv.Add( m_LineInfo.m_strPriceBandName );							break;
		case POSTRAY_EXPORTFIELD_PAYNAME:				csv.Add( m_LineInfo.m_strPaymentName );								break;
		/*****/
		case POSTRAY_EXPORTFIELD_VOID_SEQNO:			csv.Add( m_LineInfo.m_nVoidSeqNo );									break;
		case POSTRAY_EXPORTFIELD_VOID_TABLENO:			csv.Add( m_LineInfo.m_nVoidTableNo );								break;
		case POSTRAY_EXPORTFIELD_VOID_LINE_TYPE:		csv.Add( m_LineInfo.m_strVoidLineType );							break;
		case POSTRAY_EXPORTFIELD_VOID_BEFOREAFTER:		csv.Add( m_LineInfo.m_strVoidBeforeAfter );							break;
		case POSTRAY_EXPORTFIELD_VOID_REASON:			csv.Add( m_LineInfo.m_strVoidReason );								break;
		/*****/
		case POSTRAY_EXPORTFIELD_DISCOUNT_LINE_TYPE:	csv.Add( m_LineInfo.m_strDiscountLineType );						break;
		case POSTRAY_EXPORTFIELD_DISCOUNT_MODE:			csv.Add( m_LineInfo.m_strDiscountMode );							break;
		case POSTRAY_EXPORTFIELD_DISCOUNT_USAGE:		csv.Add( m_LineInfo.m_strDiscountUsage );							break;
		case POSTRAY_EXPORTFIELD_DISCOUNT_TYPE:			csv.Add( m_LineInfo.m_strDiscountType );							break;
		case POSTRAY_EXPORTFIELD_DISCOUNT_RATE:			csv.Add( m_LineInfo.m_strDiscountRate );							break;
		/*****/
		case POSTRAY_EXPORTFIELD_PROMONO:				csv.Add( m_LineInfo.m_nPromoNo );									break;
		case POSTRAY_EXPORTFIELD_PROMONAME:				csv.Add( m_LineInfo.m_strPromoName );								break;
		case POSTRAY_EXPORTFIELD_MIXMATCHNO:			csv.Add( m_LineInfo.m_nMixMatchNo );								break;
		case POSTRAY_EXPORTFIELD_MIXMATCHNAME:			csv.Add( m_LineInfo.m_strMixMatchName );							break;
		case POSTRAY_EXPORTFIELD_MIXTYPENO:				csv.Add( m_LineInfo.m_nMixTypeNo );									break;
		case POSTRAY_EXPORTFIELD_MIXTYPENAME:			csv.Add( m_LineInfo.m_strMixTypeName );								break;
		/*****/
		case POSTRAY_EXPORTFIELD_TRAN_SEQNO:			csv.Add( m_LineInfo.m_nTranSeqNo );									break;
		/*****/
		case POSTRAY_EXPORTFIELD_LINE_COUNT:			csv.Add( m_NodeTotals.m_nLineCount );								break;
		case POSTRAY_EXPORTFIELD_QTY_BASE:				csv.Add( m_NodeTotals.m_dQtyBase, SysInfo.GetDPQty() );				break;
		case POSTRAY_EXPORTFIELD_QTY_MOD:				csv.Add( m_NodeTotals.m_dQtyMod, SysInfo.GetDPQty() );				break;
		case POSTRAY_EXPORTFIELD_VALUE_FULL:			csv.Add( m_NodeTotals.m_dValueFull, SysInfo.GetDPValue() );			break;
		case POSTRAY_EXPORTFIELD_DISCOUNTPREMIUM:		csv.Add( m_NodeTotals.m_dDiscountVal - m_NodeTotals.m_dPremiumVal, SysInfo.GetDPValue() );	break;
		case POSTRAY_EXPORTFIELD_DISCOUNTONLY:			csv.Add( m_NodeTotals.m_dDiscountVal, SysInfo.GetDPValue());		break;
		case POSTRAY_EXPORTFIELD_PREMIUMONLY:			csv.Add( m_NodeTotals.m_dPremiumVal, SysInfo.GetDPValue());		break;
		case POSTRAY_EXPORTFIELD_VALUE_DISC:			csv.Add( m_NodeTotals.m_dValueAfterDiscount, SysInfo.GetDPValue() );break;
		case POSTRAY_EXPORTFIELD_TAX:					csv.Add( m_NodeTotals.m_dTaxAmount, m_nTaxDecimalPlaces );			break;
		case POSTRAY_EXPORTFIELD_VALUE_NET:				csv.Add( m_NodeTotals.m_dValueNet, m_nTaxDecimalPlaces );			break;
		case POSTRAY_EXPORTFIELD_COST:					csv.Add( m_NodeTotals.m_dCost, SysInfo.GetDPValue() );				break;
		case POSTRAY_EXPORTFIELD_WASTE_QTY_BASE:		csv.Add( m_NodeTotals.m_dWasteQtyBase, SysInfo.GetDPQty() );		break;
		case POSTRAY_EXPORTFIELD_WASTE_QTY_MOD:			csv.Add( m_NodeTotals.m_dWasteQtyMod, SysInfo.GetDPQty() );			break;
		case POSTRAY_EXPORTFIELD_WASTE_COST:			csv.Add( m_NodeTotals.m_dWasteCost, SysInfo.GetDPValue() );			break;
		case POSTRAY_EXPORTFIELD_VALUE_VOID:			csv.Add( m_NodeTotals.m_dValueVoid, SysInfo.GetDPValue() );			break;
		case POSTRAY_EXPORTFIELD_QTY_PAY:				csv.Add( m_NodeTotals.m_nPaymentQty );								break;
		case POSTRAY_EXPORTFIELD_QTY_COVERS:			csv.Add( m_NodeTotals.m_nCoversQty );								break;
		case POSTRAY_EXPORTFIELD_VALUE_TENDER:			csv.Add( m_NodeTotals.m_dTender, SysInfo.GetDPValue() );			break;
		case POSTRAY_EXPORTFIELD_VALUE_AMOUNT:			csv.Add( m_NodeTotals.m_dAmount, SysInfo.GetDPValue() );			break;
		case POSTRAY_EXPORTFIELD_VALUE_GRATUITY:		csv.Add( m_NodeTotals.m_dGratuity, SysInfo.GetDPValue() );			break;
		case POSTRAY_EXPORTFIELD_VALUE_CASHBACK:		csv.Add( m_NodeTotals.m_dCashback, SysInfo.GetDPValue() );			break;
		case POSTRAY_EXPORTFIELD_VALUE_SURPLUS:			csv.Add( m_NodeTotals.m_dSurplus, SysInfo.GetDPValue() );			break;
		/*****/
		case POSTRAY_EXPORTFIELD_EMPTY:					csv.Add( "" );														break;
		case POSTRAY_EXPORTFIELD_S4LABOUR_REV:			csv.Add( "REV" );													break;
		case POSTRAY_EXPORTFIELD_S4LABOUR_HOUR:			csv.Add( m_LineInfo.m_strS4LabourHour );							break;
		/*****/
		case POSTRAY_EXPORTFIELD_REPTEXT_BASE:		
			{
				PluInfoFinder.SetPluNo( m_LineInfo.m_nPluNo );
				csv.Add( PluInfoFinder.GetPluInfoRepText( TRUE) );
			}
			break;

		case POSTRAY_EXPORTFIELD_STOCKCODE:		
			{
				if ( SysInfo.IsStockSystem() == FALSE )
				{
					csv.Add( "" );
				}
				else
				{
					CString strStockCode = "";
					DataManager.Plu.GetPluStockCode( m_LineInfo.m_nPluNo, strStockCode, TRUE );
					csv.Add( strStockCode );
				}
			}
			break;

		case POSTRAY_EXPORTFIELD_SUPPREF:		
			{
				if ( SysInfo.IsStockSystem() == FALSE )
				{
					PluInfoFinder.SetPluNo( m_LineInfo.m_nPluNo );
					csv.Add( PluInfoFinder.GetSupplierRef() );
				}
				else
				{
					CString strSuppRef = "";
					DataManager.Plu.GetPluSuppRef( m_LineInfo.m_nPluNo, strSuppRef );
					csv.Add( strSuppRef );
				}
			}
			break;

		case POSTRAY_EXPORTFIELD_SORTCODE:		
			{
				PluInfoFinder.SetPluNo( m_LineInfo.m_nPluNo );
				csv.Add( PluInfoFinder.GetPluInfoSortCode(TRUE) );
			}
			break;

		case POSTRAY_EXPORTFIELD_REPTEXT_MOD:		
			{
				PluInfoFinder.SetPluNo( m_LineInfo.m_nPluNo );
				csv.Add( PluInfoFinder.GetPluInfoRepText( TRUE, m_LineInfo.m_nModNo, TRUE ) );
			}
			break;

		default:								
			csv.Add( "" );									
			break;
		}
	}

	fileExport.WriteLine( csv.GetLine() );
}

/**********************************************************************/

const char* CPosTrayExportConsolidator::GetLabelTimeString( bool bFinal )
{
	int nTimeHour = ( bFinal ) ? m_LineInfo.m_nFinalTimeHour : m_LineInfo.m_nActionTimeHour;
	int nTimeMinute = ( bFinal ) ? m_LineInfo.m_nFinalTimeMinute : m_LineInfo.m_nActionTimeMinute;
	int nTimeSecond = ( bFinal ) ? m_LineInfo.m_nFinalTimeSecond : m_LineInfo.m_nActionTimeSecond;
	
	m_strLabelTimeString = "";

	switch( m_TimeSliceMap.GetReportType() )
	{
	case SH_TIMESLICE_MINUTE:
		m_strLabelTimeString.Format( "%2.2d:%2.2d",
			nTimeHour,
			nTimeMinute );
		break;

	case SH_TIMESLICE_5MIN:
		m_strLabelTimeString.Format( "%2.2d:%2.2d - %2.2d:%2.2d",
			nTimeHour,
			nTimeMinute,
			nTimeHour,
			nTimeMinute + 4 );
		break;

	case SH_TIMESLICE_10MIN:
		m_strLabelTimeString.Format( "%2.2d:%2.2d - %2.2d:%2.2d",
			nTimeHour,
			nTimeMinute,
			nTimeHour,
			nTimeMinute + 9 );
		break;

	case SH_TIMESLICE_15MIN:
		m_strLabelTimeString.Format( "%2.2d:%2.2d - %2.2d:%2.2d",
			nTimeHour,
			nTimeMinute,
			nTimeHour,
			nTimeMinute + 14 );
		break;

	case SH_TIMESLICE_30MIN:
		m_strLabelTimeString.Format( "%2.2d:%2.2d - %2.2d:%2.2d",
			nTimeHour,
			nTimeMinute,
			nTimeHour,
			nTimeMinute + 29 );
		break;

	case SH_TIMESLICE_60MIN:
		m_strLabelTimeString.Format( "%2.2d:00 - %2.2d:59",
			nTimeHour,
			nTimeHour );
		break;

	case SH_CUSTOM:
		{
			m_strLabelTimeString.Format( "%4.4d", nTimeHour );
			m_strLabelTimeString = m_TimeMap.GetSortOrderName( m_strLabelTimeString );
		}
		break;

	case SH_TIMESLICE_SECOND:
	default:
		m_strLabelTimeString.Format( "%2.2d:%2.2d:%2.2d",
			nTimeHour,
			nTimeMinute,
			nTimeSecond );
		break;
	}

	return m_strLabelTimeString;
}

/**********************************************************************/

const char* CPosTrayExportConsolidator::GetRepMonth( int nMonth )
{
	m_strRepMonth = "";
	if ( ( nMonth >= 1 ) && ( nMonth <= 12 ) )
	{
		COleDateTime date = COleDateTime( 2015, nMonth, 1, 0, 0, 0 );
		m_strRepMonth = date.Format( "%B" );
	}
	return m_strRepMonth;
}

/**********************************************************************/

const char* CPosTrayExportConsolidator::GetExportDate( COleDateTime& date )
{
	switch( m_nExportDateType )
	{
	case 2:
		m_strExportDate = date.Format( "%d%m%Y" );
		break;

	case 3:
		m_strExportDate = date.Format( "%Y%m%d" );
		break;

	case 4:
		m_strExportDate = date.Format( "%Y/%m/%d" );
		break;

	case 1:
	default:
		m_strExportDate = date.Format( "%d/%m/%Y" );
		break;
	}

	return m_strExportDate;
}

/**********************************************************************/
