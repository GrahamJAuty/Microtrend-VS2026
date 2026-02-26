#pragma once
/**********************************************************************/
#include "BusinessDateInfo.h"
#include "EntityTree.h"
#include "TableReportHelpers.h"
#include "TimeMap.h"
#include "TimeSliceMapCSVArray.h"
/**********************************************************************/
#include "EposReportBase.h"
/**********************************************************************/

struct CPosTrayExportConsolidatorLineInfo
{
public:
	CPosTrayExportConsolidatorLineInfo();

public:
	int m_nDbNo;
	int m_nLocNo;
	int m_nRpNo;
	int m_nTrmNo;
	int m_nTableGrpNo;
	int m_nTableNo;
	CString m_strCalendarDate;
	CString m_strBusinessDate;
	CString m_strWeekStartDate;
	int m_nYear;
	int m_nMonthNo;
	int m_nWeekNo;
	int m_nWeekdayNo;
	int m_nFinalTimeHour;
	int m_nFinalTimeMinute;
	int m_nFinalTimeSecond;
	int m_nFinalTimeStars;
	int m_nActionTimeHour;
	int m_nActionTimeMinute;
	int m_nActionTimeSecond;
	int m_nActionTimeStars;
	__int64 m_nPluNo;
	int m_nModNo;
	int m_nDeptNoSale;
	int m_nDeptNoDBase;
	int m_nGroupNoSale;
	int m_nGroupNoDBase;
	int m_nACatNoSale;
	int m_nACatNoDBase;
	int m_nItemServerNo;
	int m_nTranServerNo;
	int m_nAuthServerNo;
	int m_nVoidSeqNo;
	int m_nVoidTableNo;
	int m_nPromoNo;
	int m_nMixTypeNo;
	int m_nMixMatchNo;
	int m_nPriceBand;
	int m_nTaxBand;
	int m_nPaymentNo;
	int m_nTranSeqNo;
	/**********/
	CString m_strDbName;
	CString m_strLocName;
	CString m_strRpName;
	CString m_strTableGrpName;
	CString m_strTableName;
	CString m_strModName;
	CString m_strDeptNameSale;
	CString m_strDeptNameDBase;
	CString m_strGroupNameSale;
	CString m_strGroupNameDBase;
	CString m_strACatNameSale;
	CString m_strACatNameDBase;
	CString m_strItemServerName;
	CString m_strTranServerName;
	CString m_strAuthServerName;
	CString m_strMonth;
	CString m_strWeekdayName;
	CString m_strVoidLineType;
	CString m_strVoidBeforeAfter;
	CString m_strVoidReason;
	CString m_strDiscountLineType;
	CString m_strDiscountMode;
	CString m_strDiscountType;
	CString m_strDiscountUsage;
	CString m_strDiscountRate;
	CString m_strPromoName;
	CString m_strMixTypeName;
	CString m_strMixMatchName;
	CString m_strPriceBandName;
	CString m_strTaxBandName;
	CString m_strPaymentName;
	CString m_strS4LabourHour;
};

/**********************************************************************/

struct CPosTrayExportConsolidatorNodeTotals
{
public:
	CPosTrayExportConsolidatorNodeTotals();

public:
	double m_dQtyBase;
	double m_dQtyMod;
	double m_dValueFull;
	double m_dDiscountVal;
	double m_dPremiumVal;
	double m_dValueAfterDiscount;
	double m_dTaxAmount;
	double m_dValueNet;
	double m_dCost;
	double m_dWasteQtyBase;
	double m_dWasteQtyMod;
	double m_dWasteCost;
	double m_dValueVoid;
	int m_nPaymentQty;
	int m_nCoversQty;
	double m_dTender;
	double m_dAmount;
	double m_dGratuity;
	double m_dCashback;
	double m_dSurplus;
	int m_nLineCount;
};

/**********************************************************************/

class CPosTrayExportConsolidator
{
public:
	CPosTrayExportConsolidator( int nEsportDateType, CEposSelectArray& SelectArray, bool& bQuitNow, void* pHostDlg );
	
public:
	void Reset( CArray<int,int>& arrayFields, CPosTrayTask& EmailTask, COleDateTime dateFrom, COleDateTime dateTo, CSessionCSVRecord& ReportSession, CTimeSliceMapCSVRecord& TimeSlice );

protected:
	void SetDates( COleDateTime dateFrom, COleDateTime dateTo, CSessionCSVRecord& ReportSession, CTimeSliceMapCSVRecord& TimeSlice );
	virtual void ResetTaskSpecific( CPosTrayTask& EmailTask ){}

protected:
	void AddAcceptedField( int nField );
	bool IsAcceptedField( int nField );

public:
	void SetTableFilter( CTableFilter info );

protected:
	virtual bool WantHeader(){ return TRUE; }
	virtual bool UseDirectLocIdx(){ return FALSE; }
	void CheckExportFields( CArray<int,int>& arrayFields, CPosTrayTask& EmailTask );
	bool PrepareFields( CEntityTree& Tree );
	
protected:
	void ShowProgress( __int64 nProgress, __int64 nTarget );

protected:
	void CreateTimeLabel( CString& strTime, bool bSortLabel );
	void ExpandTimeString( CString& strTime );
	const char* GetExportDate( COleDateTime& date );

protected:
	bool CreateReport(){ return FALSE; }
	__int64 CreateExport( CEntityTree* pEntityTree, const char* szProgressText, const char* szFilename, bool& bResult, bool bIsDiscountExport );
	__int64 CreateExportInternal( CEntityTree* pEntityTree, CSSFile& fileExport, bool bIsDiscountExport);
	void ProcessTreeBranch( CEntityTree* pEntityTree, int nDepth, int nBranchIdx, CSSFile& fileExport, __int64& nLineCount);
	virtual void GetNodeTotals( int nBranchIdx, int nLeafIdx ){};
	void CreateExportLine( CSSFile& fileExport );
	
protected:
	const char* GetLabelTimeString( bool bFinal );
	const char* GetRepMonth( int nMonth );

protected:
	CTimeMap m_TimeMap;
	CTimeSliceMapCSVRecord m_TimeSliceMap;
	CBusinessDateInfo m_BusinessDateInfo;
	
protected:
	CEposSelectArray& m_SelectArray;
	CReportConsolidationArray<CSortedIntItem> m_arrayAcceptedFields;
	CArray<int,int> m_arrayConsolidationFields;
	CArray<int,int> m_arrayOutputFields;
	int m_nTaxDecimalPlaces;

protected:
	CPosTrayExportConsolidatorLineInfo m_LineInfo;
	CPosTrayExportConsolidatorNodeTotals m_NodeTotals;

protected:
	bool m_bEODMode;
	bool m_bWantModName;
	bool m_bWantDeptNameSale;
	bool m_bWantDeptNameDBase;
	bool m_bWantGroupNameSale;
	bool m_bWantGroupNameDBase;
	bool m_bWantACatNameSale;
	bool m_bWantACatNameDBase;
	bool m_bWantTaxBandName;
	bool m_bWantPriceBandName;
	bool m_bWantPromoName;
	bool m_bWantMixTypeName;
	bool m_bWantMixMatchName;
	bool m_bWantTableName;
	bool m_bWantTableGroupName;
	bool m_bGotVoidLineType;
	bool m_bGotDiscountLineType;

protected:
	bool m_bWantPluInfo;
	bool m_bWantDeptInfo;
	bool m_bWantDeptGrpSale;
	bool m_bWantTranServer;
	bool m_bWantItemServer;
	bool m_bWantAuthServer;
	bool m_bWantCustomer;
	bool m_bWantTax;
	bool m_bWantCost;
	bool m_bWantWaste;
	bool m_bWantSales;
	bool m_bWantFinalTime;
	bool m_bWantVoidTime;
	bool m_bWantTable;
	
protected:
	CString m_strWeekDays[7];

protected:
	bool& m_bQuitNow;
	void* m_pHostDialog;
	int m_nProgress;
	int m_nDisplayedProgress;

protected:
	CTableReportHelpers m_TableReportHelpers;
	CTableFilter m_TableFilter;

protected:
	CString m_strLabelTimeString;
	CString m_strRepMonth;
	int m_nExportDateType;
	CString m_strExportDate;
};

/**********************************************************************/
