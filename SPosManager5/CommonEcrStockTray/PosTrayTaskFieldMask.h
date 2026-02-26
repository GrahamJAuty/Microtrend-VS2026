#pragma once
/**********************************************************************/

struct CPosTrayTaskFieldMask
{
public:
	CPosTrayTaskFieldMask();
	void SetMode( int nMode );

public:
	bool m_bWantTaskName;
	bool m_bWantBatchNo;
	bool m_bWantEmailNickname;
	bool m_bWantFormatEmail;
	bool m_bWantEmailByReport;
	bool m_bWantFormatExport;
	bool m_bWantExportFolder;
	bool m_bWantExportFilename;
	bool m_bWantExportFilenameFormat;
	bool m_bWantLocNameFlag;
	bool m_bWantFTPFlag;
	bool m_bWantFTPFolder;
	bool m_bWantExportDateType;
	bool m_bWantAutoPrintFlag;
	bool m_bWantLegacyTimeHour;
	bool m_bWantLegacyTimeMinute;
	bool m_bWantTimeHourStart;
	bool m_bWantTimeMinuteStart;
	bool m_bWantTimeHourEnd;
	bool m_bWantTimeMinuteEnd;
	bool m_bWantFrequency;
	bool m_bWantDaySunFlag;
	bool m_bWantDayMonFlag;
	bool m_bWantDayTueFlag;
	bool m_bWantDayWedFlag;
	bool m_bWantDayThuFlag;
	bool m_bWantDayFriFlag;
	bool m_bWantDaySatFlag;
	bool m_bWantMonthDays;
	bool m_bWantBacklogFlag;

public:
	bool m_bWantBatchName;
	bool m_bWantDateType;
	bool m_bWantSessionLegacy;
	bool m_bWantSessionEntity;
	bool m_bWantConType;
	bool m_bWantConDbNo;
	bool m_bWantConNwkNo;
	bool m_bWantConEntityNo;
	bool m_bWantConTermIdx;
	bool m_bWantTermDateType;
	bool m_bWantTableFilterType;	
	bool m_bWantTableFilterLevel;
	bool m_bWantTableFilterEntityNo;	
	bool m_bWantDeptFilter;
	bool m_bWantPluFilter;
	bool m_bWantSaleTime;
	bool m_bWantPreferConsolType;
	bool m_bWantJobSetConsolType;
	bool m_bWantReportEcr;
	bool m_bWantReportLoc;
	bool m_bWantReportAll;
	bool m_bWantReportCon;
	bool m_bWantItemServer;
	bool m_bWantTrainServer;
	bool m_bWantPMSType;
	bool m_bWantPMSPending;
	bool m_bWantPMSActive;
	bool m_bWantPMSComplete;
	bool m_bWantPMSCancelled;
	bool m_bWantReportList;
	bool m_bWantReportTime;
	bool m_bWantDateRoundTo;
	bool m_bWantTimeSlice;
	bool m_bWantPluRange;
	bool m_bWantPluFrom;
	bool m_bWantPluTo;
	bool m_bWantDeptFilterType;
	bool m_bWantModifierFilter;
	bool m_bWantModifierCustom;
	bool m_bWantTaxBandFilter;
	bool m_bWantTaxBandCustom;
	bool m_bWantPriceLevelFilter;
	bool m_bWantPriceLevelCustom;
	bool m_bWantEposSales;
	bool m_bWantManualSales;
	bool m_bWantImportSales;
	bool m_bWantPMSTerminal;
	bool m_bWantMonthSort;
	bool m_bWantDeptSort;
	bool m_bWantSupplierSort;
	bool m_bWantSupplierFilter;
	bool m_bWantStockZeroSkip;
	bool m_bWantPercentage;
	bool m_bWantEstProfit;
	bool m_bWantCustomerOnly;
	bool m_bWantVIPOnly;
	bool m_bWantChartType;
	bool m_bWantChartBottom;
	bool m_bWantChartValue;
	bool m_bWantChartSize;
	bool m_bWantChartCutOff;
	bool m_bWantTaxDP;
	bool m_bWantOfferFilter;
	bool m_bWantTimeAttend;
	bool m_bWantSalesExportFields;
	bool m_bWantPaymentExportFields;
	bool m_bWantVoidExportFields;
	bool m_bWantDiscountExportFields;

public:
	bool m_bWantMergeDelete;
};

/**********************************************************************/
