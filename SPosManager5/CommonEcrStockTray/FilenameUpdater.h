#pragma once
/**********************************************************************/
#define FNUP_FAMILY_SYSTEM 0
#define FNUP_FAMILY_REPORT 1
#define FNUP_FAMILY_DATABASE 2
#define FNUP_FAMILY_LOCATION 3
#define FNUP_FAMILY_STKPOINT 4
#define FNUP_FAMILY_PC_CLIENT 5
#define FNUP_FAMILY_INSTANCE 6
#define FNUP_FAMILY_ECRREP 7
#define FNUP_FAMILY_STOCKAUDIT 8
#define FNUP_FAMILY_LOC_COMMS 9
//UNUSED FANILY 10
#define FNUP_FAMILY_SITE 11
#define FNUP_FAMILY_NETWORK 12
#define FNUP_FAMILY_SYSSET 14
#define FNUP_FAMILY_PMS_SYSFILE 15
#define FNUP_FAMILY_PMS_BOOKING 16
#define FNUP_FAMILY_KBSET 17
#define FNUP_FAMILY_PMS_CUSTOMER 18
//UNUSED FAMILY 19
#define FNUP_FAMILY_IDRAUGHT 20
#define FNUP_FAMILY_SAGE 21
#define FNUP_FAMILY_EMAIL 22
#define FNUP_FAMILY_FNB 23
#define FNUP_FAMILY_TERM 24
#define FNUP_FAMILY_LAYOUT 25
#define FNUP_FAMILY_S4LABOUR 26
#define FNUP_FAMILY_LIPA 27
#define FNUP_FAMILY_CHARTWELLS 28
#define FNUP_FAMILY_PC_HOST 29
/**********************************************************************/

struct SysFileInfo
{
	int m_nFamily;
	int m_nFileNo;
};

/**********************************************************************/

namespace SysFiles
{
	//system configuration files
	const SysFileInfo Passwords =							{ FNUP_FAMILY_SYSTEM, 1 };
	//UNUSED 0002 - 0003
	const SysFileInfo LegacyEcrmanOptions =					{ FNUP_FAMILY_SYSTEM, 4 };
	const SysFileInfo LegacyStockOptions =					{ FNUP_FAMILY_SYSTEM, 5 };
	//UNUSED 0006
	const SysFileInfo AutoExpire =							{ FNUP_FAMILY_SYSTEM, 7 };
	const SysFileInfo DealerFlags =							{ FNUP_FAMILY_SYSTEM, 8 };
	//UNUSED 0009
	const SysFileInfo SystemLogNew =						{ FNUP_FAMILY_SYSTEM, 10 };
	const SysFileInfo SystemLogOld =						{ FNUP_FAMILY_SYSTEM, 11 };
	const SysFileInfo Columns =								{ FNUP_FAMILY_SYSTEM, 12 };
	const SysFileInfo PluHistory =							{ FNUP_FAMILY_SYSTEM, 13 };
	//UNUSED 0014
	const SysFileInfo StockHistory =						{ FNUP_FAMILY_SYSTEM, 15 };
	const SysFileInfo BarcodeEntryTable =					{ FNUP_FAMILY_SYSTEM, 16 };
	const SysFileInfo PluUnit =								{ FNUP_FAMILY_SYSTEM, 17 }; 
	const SysFileInfo StockUnit =							{ FNUP_FAMILY_SYSTEM, 18 }; 
	const SysFileInfo SubUnit =								{ FNUP_FAMILY_SYSTEM, 19 }; 
	const SysFileInfo AdjustText =							{ FNUP_FAMILY_SYSTEM, 20 }; 
	const SysFileInfo OrderNum =							{ FNUP_FAMILY_SYSTEM, 21 };
	const SysFileInfo EcrmanSupplier =						{ FNUP_FAMILY_SYSTEM, 22 };	
	const SysFileInfo System =								{ FNUP_FAMILY_SYSTEM, 23 };
	const SysFileInfo PresetRepEposBatch =					{ FNUP_FAMILY_SYSTEM, 24 };
	//UNUSED 0025 - 0026
	const SysFileInfo DelUnit =								{ FNUP_FAMILY_SYSTEM, 27 };
	//UNUSED 0028 - 0029
	const SysFileInfo OrderInstructions =					{ FNUP_FAMILY_SYSTEM, 30 };
	const SysFileInfo EmailInstructions =					{ FNUP_FAMILY_SYSTEM, 31 };
	//UNUSED 0032
	const SysFileInfo PluCustomDef =						{ FNUP_FAMILY_SYSTEM, 33 };
	//UNSED 0034 - 0035
	const SysFileInfo PluCSVDef =							{ FNUP_FAMILY_SYSTEM, 36 };
	const SysFileInfo PluLinkDef =							{ FNUP_FAMILY_SYSTEM, 37 };
	const SysFileInfo StockCSVDef =							{ FNUP_FAMILY_SYSTEM, 38 };
	const SysFileInfo StockSalesDef =						{ FNUP_FAMILY_SYSTEM, 39 };
	const SysFileInfo AdhocDelConfig =						{ FNUP_FAMILY_SYSTEM, 40 };
	//UNUSED 0041-0042
	const SysFileInfo PresetRepPluHist =					{ FNUP_FAMILY_SYSTEM, 43 };
	//UNUSED 0044
	const SysFileInfo PresetRepStockHist =					{ FNUP_FAMILY_SYSTEM, 45 };
	//UNUSED 0046
	const SysFileInfo StockCustomDef =						{ FNUP_FAMILY_SYSTEM, 47 };
	const SysFileInfo Tasks =								{ FNUP_FAMILY_SYSTEM, 48 };
	//UNUSED 0049 - 0050
	const SysFileInfo ValidationErrors =					{ FNUP_FAMILY_SYSTEM, 51 };
	const SysFileInfo DeliveryRef =							{ FNUP_FAMILY_SYSTEM, 52 };
	const SysFileInfo ReturnRef =							{ FNUP_FAMILY_SYSTEM, 53 };
	const SysFileInfo AdjustmentRef =						{ FNUP_FAMILY_SYSTEM, 54 };
	const SysFileInfo TransferRef =							{ FNUP_FAMILY_SYSTEM, 55 };
	//UNUSED 0056
	const SysFileInfo ReportSession =						{ FNUP_FAMILY_SYSTEM, 57 };
	//UNUSED 0058 - 0060
	const SysFileInfo Modifier =							{ FNUP_FAMILY_SYSTEM, 61 };
	const SysFileInfo Payment =								{ FNUP_FAMILY_SYSTEM, 62 };
	const SysFileInfo AnalysisCategory =					{ FNUP_FAMILY_SYSTEM, 63 };
	const SysFileInfo TimeSliceList =						{ FNUP_FAMILY_SYSTEM, 64 };
	const SysFileInfo TimeSliceDetail =						{ FNUP_FAMILY_SYSTEM, 65 };
	const SysFileInfo TimeSliceBackup =						{ FNUP_FAMILY_SYSTEM, 66 };
	//UNUSED_0067
	const SysFileInfo BarLinkCustomDef =					{ FNUP_FAMILY_SYSTEM, 68 };
	//UNUSED 0069 - 0071
	const SysFileInfo CustomerHistory =						{ FNUP_FAMILY_SYSTEM, 72 };
	const SysFileInfo PresetRepCustHist =					{ FNUP_FAMILY_SYSTEM, 73 };
	const SysFileInfo SupplierImport =						{ FNUP_FAMILY_SYSTEM, 74 };
	//UNUSED 0075 - 0076
	const SysFileInfo PluChangesTask =						{ FNUP_FAMILY_SYSTEM, 77 };
	//UNUSED 0078 - 0081
	const SysFileInfo EmailAddress =						{ FNUP_FAMILY_SYSTEM, 82 };
	//UNUSED 0083
	const SysFileInfo StockDeleteDef =						{ FNUP_FAMILY_SYSTEM, 84 };
	const SysFileInfo PluDeleteDef =						{ FNUP_FAMILY_SYSTEM, 85 };
	const SysFileInfo StockReportList =						{ FNUP_FAMILY_SYSTEM, 86 };
	const SysFileInfo EposReportListOld =					{ FNUP_FAMILY_SYSTEM, 87 };
	//UNUSED 0088-0092
	const SysFileInfo StockImportDef =						{ FNUP_FAMILY_SYSTEM, 93 };
	//UNUSED 0094
	const SysFileInfo PMSAccountFindDef =					{ FNUP_FAMILY_SYSTEM, 95 };
	const SysFileInfo PMSPaymentsDef =						{ FNUP_FAMILY_SYSTEM, 96 };
	//UNUSED 0097
	const SysFileInfo PMSRoomDef =							{ FNUP_FAMILY_SYSTEM, 98 };
	const SysFileInfo PMSUnpaidDef =						{ FNUP_FAMILY_SYSTEM, 99 };
	const SysFileInfo StockReportSortOrder =				{ FNUP_FAMILY_SYSTEM, 100 };
	const SysFileInfo PresetRepStockLevel =					{ FNUP_FAMILY_SYSTEM, 101 };
	//UNUSED 0102
	const SysFileInfo PosTrayOptions =						{ FNUP_FAMILY_SYSTEM, 103 };
	const SysFileInfo PosTrayBatchEpos =					{ FNUP_FAMILY_SYSTEM, 104 };
	const SysFileInfo PosTrayBatchEposTemp =				{ FNUP_FAMILY_SYSTEM, 105 };
	const SysFileInfo PosTrayTaskEpos =						{ FNUP_FAMILY_SYSTEM, 106 };
	const SysFileInfo PosTrayTaskEposTemp =					{ FNUP_FAMILY_SYSTEM, 107 };
	const SysFileInfo PosTrayBatchHist =					{ FNUP_FAMILY_SYSTEM, 108 };
	const SysFileInfo PosTrayBatchHistTemp =				{ FNUP_FAMILY_SYSTEM, 109 };
	const SysFileInfo PosTrayTaskHist =						{ FNUP_FAMILY_SYSTEM, 110 };
	const SysFileInfo PosTrayTaskHistTemp =					{ FNUP_FAMILY_SYSTEM, 111 };
	//UNUSED 0112 - 0114
	const SysFileInfo CustomerHeader =						{ FNUP_FAMILY_SYSTEM, 115 };
	const SysFileInfo EposReportListBeta =					{ FNUP_FAMILY_SYSTEM, 116 };
	const SysFileInfo EcrmanOptions =						{ FNUP_FAMILY_SYSTEM, 117 };
	const SysFileInfo StockOptions =						{ FNUP_FAMILY_SYSTEM, 118 };
	const SysFileInfo EposReportList =						{ FNUP_FAMILY_SYSTEM, 119 };
	const SysFileInfo PosTrayConsolEpos =					{ FNUP_FAMILY_SYSTEM, 120 };
	const SysFileInfo PosTrayConsolTemp =					{ FNUP_FAMILY_SYSTEM, 121 };
	//UNUSED 0122 - 0123
	const SysFileInfo EmailOptions =						{ FNUP_FAMILY_SYSTEM, 124 };
	const SysFileInfo BillReceiptText =						{ FNUP_FAMILY_SYSTEM, 125 };
	const SysFileInfo BillReceiptLock =						{ FNUP_FAMILY_SYSTEM, 126 };
	const SysFileInfo SBPaymentMap =						{ FNUP_FAMILY_SYSTEM, 127 };
	const SysFileInfo PosTrayBatchExportSales =				{ FNUP_FAMILY_SYSTEM, 128 };
	const SysFileInfo PosTrayBatchExportSalesTemp =			{ FNUP_FAMILY_SYSTEM, 129 };
	const SysFileInfo PosTrayTaskExportSales =				{ FNUP_FAMILY_SYSTEM, 130 };
	const SysFileInfo PosTrayTaskExportSalesTemp =			{ FNUP_FAMILY_SYSTEM, 131 };
	const SysFileInfo PosTrayBatchExportPayment =			{ FNUP_FAMILY_SYSTEM, 132 };
	const SysFileInfo PosTrayBatchExportPaymentTemp =		{ FNUP_FAMILY_SYSTEM, 133 };
	const SysFileInfo PosTrayTaskExportPayment =			{ FNUP_FAMILY_SYSTEM, 134 };
	const SysFileInfo PosTrayTaskExportPaymentTemp =		{ FNUP_FAMILY_SYSTEM, 135 };
	const SysFileInfo PosTrayExportFieldNames =				{ FNUP_FAMILY_SYSTEM, 136 };
	const SysFileInfo PosTrayBatchExportVoid =				{ FNUP_FAMILY_SYSTEM, 137 };
	const SysFileInfo PosTrayBatchExportVoidTemp =			{ FNUP_FAMILY_SYSTEM, 138 };
	const SysFileInfo PosTrayTaskExportVoid =				{ FNUP_FAMILY_SYSTEM, 139 };
	const SysFileInfo PosTrayTaskExportVoidTemp =			{ FNUP_FAMILY_SYSTEM, 140 };
	const SysFileInfo PosTrayBatchExportDiscount =			{ FNUP_FAMILY_SYSTEM, 141 };
	const SysFileInfo PosTrayBatchExportDiscountTemp =		{ FNUP_FAMILY_SYSTEM, 142 };
	const SysFileInfo PosTrayTaskExportDiscount =			{ FNUP_FAMILY_SYSTEM, 143 };
	const SysFileInfo PosTrayTaskExportDiscountTemp =		{ FNUP_FAMILY_SYSTEM, 144 };
	const SysFileInfo PaymentGroup =						{ FNUP_FAMILY_SYSTEM, 145 };
	const SysFileInfo NetworkResources =					{ FNUP_FAMILY_SYSTEM, 146 };
	const SysFileInfo SEPaymentMap =						{ FNUP_FAMILY_SYSTEM, 147 };
	const SysFileInfo AllergenList =						{ FNUP_FAMILY_SYSTEM, 148 };
	const SysFileInfo PresetRepEpos =						{ FNUP_FAMILY_SYSTEM, 149 };
	const SysFileInfo PresetRepStockAudit =					{ FNUP_FAMILY_SYSTEM, 150 };
	const SysFileInfo DorbiereToken =						{ FNUP_FAMILY_SYSTEM, 151 };
	const SysFileInfo FunctionAuthority =					{ FNUP_FAMILY_SYSTEM, 152 };
	const SysFileInfo PosTrayBatchStkLevelEcr =				{ FNUP_FAMILY_SYSTEM, 153 };
	const SysFileInfo PosTrayBatchStkLevelEcrTemp =			{ FNUP_FAMILY_SYSTEM, 154 };
	const SysFileInfo PosTrayTaskStkLevelEcr =				{ FNUP_FAMILY_SYSTEM, 155 };
	const SysFileInfo PosTrayTaskStkLevelEcrTemp =			{ FNUP_FAMILY_SYSTEM, 156 };
	const SysFileInfo PosTrayBatchStkLevelStk =				{ FNUP_FAMILY_SYSTEM, 157 };
	const SysFileInfo PosTrayBatchStkLevelStkTemp =			{ FNUP_FAMILY_SYSTEM, 158 };
	const SysFileInfo PosTrayTaskStkLevelStk =				{ FNUP_FAMILY_SYSTEM, 159 };
	const SysFileInfo PosTrayTaskStkLevelStkTemp =			{ FNUP_FAMILY_SYSTEM, 160 };
	const SysFileInfo OPGroup =								{ FNUP_FAMILY_SYSTEM, 161 };
	const SysFileInfo ServerPreset =						{ FNUP_FAMILY_SYSTEM, 162 };
	const SysFileInfo StockImportSheetDef =					{ FNUP_FAMILY_SYSTEM, 163 };
	const SysFileInfo PresetRepEcrStock =					{ FNUP_FAMILY_SYSTEM, 164 };
	const SysFileInfo SpecialKeys =							{ FNUP_FAMILY_SYSTEM, 165 };
	const SysFileInfo PresetRepTimeAttend =					{ FNUP_FAMILY_SYSTEM, 166 };
	const SysFileInfo SystemDepartment =					{ FNUP_FAMILY_SYSTEM, 167 };
	const SysFileInfo SystemDepartmentSet =					{ FNUP_FAMILY_SYSTEM, 168 };
	const SysFileInfo SystemCategory =						{ FNUP_FAMILY_SYSTEM, 169 };
	const SysFileInfo SystemCategorySet =					{ FNUP_FAMILY_SYSTEM, 170 };
	const SysFileInfo SystemCustomFieldSet =				{ FNUP_FAMILY_SYSTEM, 171 };
	const SysFileInfo SysLoyaltyScheme =					{ FNUP_FAMILY_SYSTEM, 172 };
	const SysFileInfo CascadingMacro =						{ FNUP_FAMILY_SYSTEM, 173 };
	const SysFileInfo PosTrayBatchProcessTran =				{ FNUP_FAMILY_SYSTEM, 174 };
	const SysFileInfo PosTrayBatchProcessTranTemp =			{ FNUP_FAMILY_SYSTEM, 175 };
	const SysFileInfo PosTrayTaskProcessTran =				{ FNUP_FAMILY_SYSTEM, 176 };
	const SysFileInfo PosTrayTaskProcessTranTemp =			{ FNUP_FAMILY_SYSTEM, 177 };
	const SysFileInfo ReasonTextItemVoid =					{ FNUP_FAMILY_SYSTEM, 178 };
	const SysFileInfo ReasonTextTranVoid =					{ FNUP_FAMILY_SYSTEM, 179 };
	const SysFileInfo ReasonTextRefund =					{ FNUP_FAMILY_SYSTEM, 180 };
	const SysFileInfo ReasonTextVIP =						{ FNUP_FAMILY_SYSTEM, 181 };
	const SysFileInfo ReasonTextPromo =						{ FNUP_FAMILY_SYSTEM, 182 };
	const SysFileInfo ReasonTextWastage =					{ FNUP_FAMILY_SYSTEM, 183 };
	const SysFileInfo ReasonTextNoSale =					{ FNUP_FAMILY_SYSTEM, 184 };
	const SysFileInfo ReasonTextVoidSurcharge =				{ FNUP_FAMILY_SYSTEM, 185 };
	const SysFileInfo PosTrayBatchTimeAttend =				{ FNUP_FAMILY_SYSTEM, 186 };
	const SysFileInfo PosTrayBatchTimeAttendTemp =			{ FNUP_FAMILY_SYSTEM, 187 };
	const SysFileInfo PosTrayTaskTimeAttend =				{ FNUP_FAMILY_SYSTEM, 188 };
	const SysFileInfo PosTrayTaskTimeAttendTemp =			{ FNUP_FAMILY_SYSTEM, 189 };
	const SysFileInfo TimeAttendReportNames =				{ FNUP_FAMILY_SYSTEM, 190 };
	const SysFileInfo ServerCSVDef =						{ FNUP_FAMILY_SYSTEM, 191 };
	const SysFileInfo StartupMessage =						{ FNUP_FAMILY_SYSTEM, 192 };
	const SysFileInfo OPName =								{ FNUP_FAMILY_SYSTEM, 193 };
	const SysFileInfo ChartwellsOptions =					{ FNUP_FAMILY_SYSTEM, 194 };
	const SysFileInfo ChartwellsOptionsTemp =				{ FNUP_FAMILY_SYSTEM, 195 };
	const SysFileInfo SQLExceptions =						{ FNUP_FAMILY_SYSTEM, 196 };
	const SysFileInfo ChartwellsDbCheck =					{ FNUP_FAMILY_SYSTEM, 197 };
	const SysFileInfo ChartwellsSiteListOut =				{ FNUP_FAMILY_SYSTEM, 198 };
	const SysFileInfo ChartwellsSiteListIn =				{ FNUP_FAMILY_SYSTEM, 199 };
	const SysFileInfo TaxImportMap =						{ FNUP_FAMILY_SYSTEM, 200 };
	const SysFileInfo PosTrayBackupToken =					{ FNUP_FAMILY_SYSTEM, 201 };
	//AVAILABLE 0202 - 899
	//UNUSED 0900 - 0903
	//AVAILABLE 0904 - 0996
	const SysFileInfo RebuildDailyTran =					{ FNUP_FAMILY_SYSTEM, 997 };
	const SysFileInfo SchedulerQuit =						{ FNUP_FAMILY_SYSTEM, 998 };
	const SysFileInfo DataValidationQuit =					{ FNUP_FAMILY_SYSTEM, 999 };

	//report param files
	const SysFileInfo PluApparentPrm =						{ FNUP_FAMILY_REPORT, 1 };
	const SysFileInfo PluDelAuditPrm =						{ FNUP_FAMILY_REPORT, 2 };
	const SysFileInfo SchedLogPrm =							{ FNUP_FAMILY_REPORT, 3 };
	const SysFileInfo UserLogPrm =							{ FNUP_FAMILY_REPORT, 4 };
	const SysFileInfo MixMatchPrm =							{ FNUP_FAMILY_REPORT, 5 };
	//UNUSED 1006 - 1008
	const SysFileInfo OrderPrm =							{ FNUP_FAMILY_REPORT, 9 };
	const SysFileInfo SupplierPrm =							{ FNUP_FAMILY_REPORT, 10 };
	const SysFileInfo DepartmentPrm =						{ FNUP_FAMILY_REPORT, 11 };
	const SysFileInfo CategoryPrm =							{ FNUP_FAMILY_REPORT, 12 };
	const SysFileInfo OrderPreviewPrm =						{ FNUP_FAMILY_REPORT, 13 };
	//UNUSED 1013
	const SysFileInfo CustomHistPrm =						{ FNUP_FAMILY_REPORT, 15 };
	//UNUSED 1016 - 1019
	const SysFileInfo RegionHistPrm =						{ FNUP_FAMILY_REPORT, 20 };
	//UNUSED 1021 - 1022
	const SysFileInfo SupplierDelPrm =						{ FNUP_FAMILY_REPORT, 23 };
	const SysFileInfo MinLevelImport =						{ FNUP_FAMILY_REPORT, 24 };
	const SysFileInfo EcrmanUpgrade =						{ FNUP_FAMILY_REPORT, 25 };
	const SysFileInfo PluPriceList =						{ FNUP_FAMILY_REPORT, 26 };
	const SysFileInfo PluProgramming =						{ FNUP_FAMILY_REPORT, 27 };
	const SysFileInfo PluCustomList =						{ FNUP_FAMILY_REPORT, 28 };
	const SysFileInfo OrderBasePrm =						{ FNUP_FAMILY_REPORT, 29 };
	const SysFileInfo PluLinkPrm =							{ FNUP_FAMILY_REPORT, 30 };
	const SysFileInfo StockReportPrm =						{ FNUP_FAMILY_REPORT, 31 };
	//UNUSED 1032 - 1039	
	const SysFileInfo ReorderCopyPrm =						{ FNUP_FAMILY_REPORT, 40 };
	//UNUSED 1041 - 1044
	const SysFileInfo TimeAttendPrm =						{ FNUP_FAMILY_REPORT, 45 };
	const SysFileInfo NetCheckPrm =							{ FNUP_FAMILY_REPORT, 46 };
	const SysFileInfo PaymentPrm =							{ FNUP_FAMILY_REPORT, 47 };
	const SysFileInfo EposGroupPrm =						{ FNUP_FAMILY_REPORT, 48 };
	const SysFileInfo ServerPrm =							{ FNUP_FAMILY_REPORT, 49 };
	const SysFileInfo ServerNamePrm =						{ FNUP_FAMILY_REPORT, 50 };
	const SysFileInfo BarcodePrm =							{ FNUP_FAMILY_REPORT, 51 };
	const SysFileInfo OfferGroupPrm =						{ FNUP_FAMILY_REPORT, 52 };
	const SysFileInfo PromoPrm =							{ FNUP_FAMILY_REPORT, 53 };
	const SysFileInfo BarLinkPrm =							{ FNUP_FAMILY_REPORT, 54 };
	//UNUSED 1055 - 1059
	const SysFileInfo TimePlanPrm =							{ FNUP_FAMILY_REPORT, 60 };
	//UNUSED 1061 - 1063
	const SysFileInfo OrderExceptionsPrm =					{ FNUP_FAMILY_REPORT, 64 };
	const SysFileInfo HistoricalTaxPrm =					{ FNUP_FAMILY_REPORT, 65 };
	//UNUSED 1066
	const SysFileInfo EmailLogPrm =							{ FNUP_FAMILY_REPORT, 67 };
	const SysFileInfo StockDeletePrm =						{ FNUP_FAMILY_REPORT, 68 };
	const SysFileInfo PluDeletePrm =						{ FNUP_FAMILY_REPORT, 69 };
	const SysFileInfo AutoClosingPrm =						{ FNUP_FAMILY_REPORT, 70 };
	//UNUSED 1071
	const SysFileInfo StockImportPrm =						{ FNUP_FAMILY_REPORT, 72 };
	//UNUSED 1073 - 1075
	const SysFileInfo PMSUnpaidPrm =						{ FNUP_FAMILY_REPORT, 76 };
	//UNUSED 1077
	const SysFileInfo PMSAccountFindPrm =					{ FNUP_FAMILY_REPORT, 78 };
	const SysFileInfo PMSPaymentsPrm =						{ FNUP_FAMILY_REPORT, 79 };
	const SysFileInfo BritanniaExportPrm =					{ FNUP_FAMILY_REPORT, 80 };
	const SysFileInfo PMSRoomListPrm =						{ FNUP_FAMILY_REPORT, 81 };
	//UNUSED 1082
	const SysFileInfo IDraughtReportPrm =					{ FNUP_FAMILY_REPORT, 83 };
	const SysFileInfo IDraughtStatusPrm =					{ FNUP_FAMILY_REPORT, 84 };
	const SysFileInfo IDraughtLogPrm =						{ FNUP_FAMILY_REPORT, 85 };
	//UNUSED 1086	
	const SysFileInfo CustomerAccountPrm =					{ FNUP_FAMILY_REPORT, 87 };
	const SysFileInfo ReorderLevelPrm =						{ FNUP_FAMILY_REPORT, 88 };
	const SysFileInfo SageLogPrm =							{ FNUP_FAMILY_REPORT, 89 };
	const SysFileInfo FNBLogPrm =							{ FNUP_FAMILY_REPORT, 90 };
	const SysFileInfo ReportGroupPrm =						{ FNUP_FAMILY_REPORT, 91 };
	const SysFileInfo ConsolGroupPrm =						{ FNUP_FAMILY_REPORT, 92 };
	const SysFileInfo EcrCustomReportPrm =					{ FNUP_FAMILY_REPORT, 93 };
	const SysFileInfo EcrReportConSys =						{ FNUP_FAMILY_REPORT, 94 };
	const SysFileInfo EcrReportConCust =					{ FNUP_FAMILY_REPORT, 95 };
	//UNUSED 1096
	const SysFileInfo PluFilterPrm =						{ FNUP_FAMILY_REPORT, 97 };
	const SysFileInfo CustomerOneOffPrm =					{ FNUP_FAMILY_REPORT, 98 };
	const SysFileInfo FlexDetailPrm =						{ FNUP_FAMILY_REPORT, 99 };
	const SysFileInfo PaymentVersionPrm =					{ FNUP_FAMILY_REPORT, 100 };
	const SysFileInfo PromoVersionPrm =						{ FNUP_FAMILY_REPORT, 101 };
	const SysFileInfo MixMatchVersionPrm =					{ FNUP_FAMILY_REPORT, 102 };
	const SysFileInfo OfferGroupVersionPrm =				{ FNUP_FAMILY_REPORT, 103 };
	const SysFileInfo ServerVersionPrm =					{ FNUP_FAMILY_REPORT, 104 };
	const SysFileInfo PaymentGroupPrm =						{ FNUP_FAMILY_REPORT, 105 };
	const SysFileInfo AllergenPrm =							{ FNUP_FAMILY_REPORT, 106 };
	const SysFileInfo FTPLogPrm =							{ FNUP_FAMILY_REPORT, 107 };
	const SysFileInfo S4LabourLogPrm =						{ FNUP_FAMILY_REPORT, 108 };
	const SysFileInfo FileChangesLogPrm =					{ FNUP_FAMILY_REPORT, 109 };
	const SysFileInfo StockSheetImportPrm =					{ FNUP_FAMILY_REPORT, 110 };
	const SysFileInfo PluPriceSetList =						{ FNUP_FAMILY_REPORT, 111 };
	const SysFileInfo PosTrayLogActionPrm =					{ FNUP_FAMILY_REPORT, 112 };
	const SysFileInfo PosTrayLogExportPrm =					{ FNUP_FAMILY_REPORT, 113 };
	const SysFileInfo TimePlanVersionPrm =					{ FNUP_FAMILY_REPORT, 114 };
	const SysFileInfo AutoDeleteCustLogPrm =				{ FNUP_FAMILY_REPORT, 115 };
	const SysFileInfo LoyaltySchemePrm =					{ FNUP_FAMILY_REPORT, 116 };
	const SysFileInfo TableNamePrm =						{ FNUP_FAMILY_REPORT, 117 };
	const SysFileInfo TableGroupPrm =						{ FNUP_FAMILY_REPORT, 118 };
	const SysFileInfo TableGroupSetPrm =					{ FNUP_FAMILY_REPORT, 119 };
	const SysFileInfo TableAreaPrm =						{ FNUP_FAMILY_REPORT, 120 };
	const SysFileInfo MacroListPrm =						{ FNUP_FAMILY_REPORT, 121 };
	const SysFileInfo CustomerTrackTracePrm =				{ FNUP_FAMILY_REPORT, 122 };
	const SysFileInfo WalletStampRulePrm =					{ FNUP_FAMILY_REPORT, 123 };
	const SysFileInfo WalletStampRuleVersionPrm =			{ FNUP_FAMILY_REPORT, 124 };
	const SysFileInfo ChartwellsLogPrm =					{ FNUP_FAMILY_REPORT, 125 };
	const SysFileInfo TimePlanTemplatePrm =					{ FNUP_FAMILY_REPORT, 126 };
	const SysFileInfo PromoPrm2 =							{ FNUP_FAMILY_REPORT, 127 };
	const SysFileInfo MixMatchPrm2 =						{ FNUP_FAMILY_REPORT, 128 };

	//database specific files
	const SysFileInfo DbAddress =							{ FNUP_FAMILY_DATABASE, 0 };
	const SysFileInfo PluDatabase =							{ FNUP_FAMILY_DATABASE, 1 };
	const SysFileInfo PluImport =							{ FNUP_FAMILY_DATABASE, 2 };
	const SysFileInfo StockDatabase =						{ FNUP_FAMILY_DATABASE, 3 };
	const SysFileInfo StockImport =							{ FNUP_FAMILY_DATABASE, 4 };
	const SysFileInfo RecipeDatabase =						{ FNUP_FAMILY_DATABASE, 5 };
	//UNUSED 2006
	const SysFileInfo Department =							{ FNUP_FAMILY_DATABASE, 7 };
	const SysFileInfo DepartmentSet =						{ FNUP_FAMILY_DATABASE, 8 };
	const SysFileInfo DepartmentEposGroup =					{ FNUP_FAMILY_DATABASE, 9 };
	//UNUSED 2010 - 2013
	const SysFileInfo Promotion =							{ FNUP_FAMILY_DATABASE, 14 };
	//UNUSED 2015 - 2016
	const SysFileInfo TaxRates =							{ FNUP_FAMILY_DATABASE, 17 };
	const SysFileInfo Allowance =							{ FNUP_FAMILY_DATABASE, 18 };
	const SysFileInfo Category =							{ FNUP_FAMILY_DATABASE, 19 };
	const SysFileInfo CategorySet =							{ FNUP_FAMILY_DATABASE, 20 };
	const SysFileInfo Orders =								{ FNUP_FAMILY_DATABASE, 21 };
	const SysFileInfo Supplier =							{ FNUP_FAMILY_DATABASE, 22 };
	const SysFileInfo SupplierSet =							{ FNUP_FAMILY_DATABASE, 23 };
	//UNUSED 2024
	const SysFileInfo KeyboardPlu =							{ FNUP_FAMILY_DATABASE, 25 };
	const SysFileInfo DbPriceSetFields =					{ FNUP_FAMILY_DATABASE, 26 };
	const SysFileInfo DbPriceSetTemp =						{ FNUP_FAMILY_DATABASE, 27 };
	const SysFileInfo DbShelfLabels =						{ FNUP_FAMILY_DATABASE, 28 };
	const SysFileInfo DbProductLabels =						{ FNUP_FAMILY_DATABASE, 29 };
	const SysFileInfo DbSavedShelfLabels =					{ FNUP_FAMILY_DATABASE, 30 };
	const SysFileInfo DbSavedProductLabels =				{ FNUP_FAMILY_DATABASE, 31 };
	const SysFileInfo ExportDate =							{ FNUP_FAMILY_DATABASE, 32 };
	const SysFileInfo OpeningTemp =							{ FNUP_FAMILY_DATABASE, 33 };
	const SysFileInfo PluStockAudit =						{ FNUP_FAMILY_DATABASE, 34 };
	const SysFileInfo PluStockAuditTmp =					{ FNUP_FAMILY_DATABASE, 35 };
	const SysFileInfo PluStockAuditBak =					{ FNUP_FAMILY_DATABASE, 36 };
	//UNUSED 2037 - 2041	
	const SysFileInfo Barcode =								{ FNUP_FAMILY_DATABASE, 42 };
	const SysFileInfo OfferGroup =							{ FNUP_FAMILY_DATABASE, 43 };
	const SysFileInfo OfferEntry =							{ FNUP_FAMILY_DATABASE, 44 };
	const SysFileInfo MixMatch =							{ FNUP_FAMILY_DATABASE, 45 };
	//UNUSED 2046 - 2050
	const SysFileInfo TimePlan =							{ FNUP_FAMILY_DATABASE, 51 };
	const SysFileInfo TimePlanEntry =						{ FNUP_FAMILY_DATABASE, 52 };
	//UNUSED 2053 - 2054
	const SysFileInfo HistoricalTax =						{ FNUP_FAMILY_DATABASE, 55 };
	const SysFileInfo ExportLiveApptStockToken =			{ FNUP_FAMILY_DATABASE, 56 };
	const SysFileInfo ExportLiveApptStockLock =				{ FNUP_FAMILY_DATABASE, 57 };
	const SysFileInfo CustomFieldName =						{ FNUP_FAMILY_DATABASE, 58 };
	const SysFileInfo CustomFieldValue =					{ FNUP_FAMILY_DATABASE, 59 };
	const SysFileInfo CustomFieldSet =						{ FNUP_FAMILY_DATABASE, 60 };
	//UNUSED 2061 - 2062
	const SysFileInfo PaidInText =							{ FNUP_FAMILY_DATABASE, 63 };
	const SysFileInfo PaidOutText =							{ FNUP_FAMILY_DATABASE, 64 };
	const SysFileInfo SageDate =							{ FNUP_FAMILY_DATABASE, 65 };
	const SysFileInfo UnitechPlu =							{ FNUP_FAMILY_DATABASE, 66 };
	const SysFileInfo UnitechDbLock =						{ FNUP_FAMILY_DATABASE, 67 };
	const SysFileInfo FNBOptions =							{ FNUP_FAMILY_DATABASE, 68 };
	const SysFileInfo FNBOptionsTemp =						{ FNUP_FAMILY_DATABASE, 69 };
	const SysFileInfo DeptReportGroup =						{ FNUP_FAMILY_DATABASE, 70 };
	const SysFileInfo DeptConsolGroup =						{ FNUP_FAMILY_DATABASE, 71 };
	//UNUSED 2072
	const SysFileInfo PriceText =							{ FNUP_FAMILY_DATABASE, 73 };
	const SysFileInfo PluFilterNames =						{ FNUP_FAMILY_DATABASE, 74 };
	const SysFileInfo PluFilterItems =						{ FNUP_FAMILY_DATABASE, 75 };
	const SysFileInfo SBPriceText =							{ FNUP_FAMILY_DATABASE, 76 };
	const SysFileInfo PriceSetCustomDef =					{ FNUP_FAMILY_DATABASE, 77 };
	const SysFileInfo PresetPluFilters =					{ FNUP_FAMILY_DATABASE, 78 };
	const SysFileInfo LoyaltyStampOffer =					{ FNUP_FAMILY_DATABASE, 79 };
	const SysFileInfo SmartPayPurchaseControl =				{ FNUP_FAMILY_DATABASE, 80 };
	const SysFileInfo DbLoyaltyScheme =						{ FNUP_FAMILY_DATABASE, 81 };
	const SysFileInfo TimePlanTemplateEntry =				{ FNUP_FAMILY_DATABASE, 82 };

	//location specific files
	const SysFileInfo LocShelfLabels =						{ FNUP_FAMILY_LOCATION, 1 };
	const SysFileInfo LocSavedShelfLabels =					{ FNUP_FAMILY_LOCATION, 2 };
	const SysFileInfo LocSavedProductLabels =				{ FNUP_FAMILY_LOCATION, 3 };
	const SysFileInfo LocPriceSetFields =					{ FNUP_FAMILY_LOCATION, 4 };
	const SysFileInfo LocPriceSetTemp =						{ FNUP_FAMILY_LOCATION, 5 };
	const SysFileInfo PriceSetExclude =						{ FNUP_FAMILY_LOCATION, 6 };
	const SysFileInfo PriceSetExcludeTemp =					{ FNUP_FAMILY_LOCATION, 7 };
	const SysFileInfo PriceSetInclude =						{ FNUP_FAMILY_LOCATION, 8 };
	const SysFileInfo PriceSetIncludeTemp =					{ FNUP_FAMILY_LOCATION, 9 };
	const SysFileInfo KeyboardLock =						{ FNUP_FAMILY_LOCATION, 10 };
	//UNUSED 3011 - 3018
	const SysFileInfo Servers =								{ FNUP_FAMILY_LOCATION, 19 };
	//UNUSED 3019 - 3027
	const SysFileInfo KeyboardNames =						{ FNUP_FAMILY_LOCATION, 28 };
	const SysFileInfo KeyboardNameLock =					{ FNUP_FAMILY_LOCATION, 29 };
	//UNUSED 3030
	const SysFileInfo HandheldToken1 =						{ FNUP_FAMILY_LOCATION, 31 };
	//UNUSED 3032
	const SysFileInfo LocPresetDate =						{ FNUP_FAMILY_LOCATION, 33 };
	const SysFileInfo CustomerAccount =						{ FNUP_FAMILY_LOCATION, 34 };
	const SysFileInfo CustomerSyncDate =					{ FNUP_FAMILY_LOCATION, 35 };
	const SysFileInfo DepositNames =						{ FNUP_FAMILY_LOCATION, 36 };
	const SysFileInfo CustomerOneOff =						{ FNUP_FAMILY_LOCATION, 37 };
	const SysFileInfo LocInfoDate =							{ FNUP_FAMILY_LOCATION, 38 };
	const SysFileInfo EODDates =							{ FNUP_FAMILY_LOCATION, 39 };
	const SysFileInfo ServerRestriction =					{ FNUP_FAMILY_LOCATION, 40 };
	const SysFileInfo ExtraCommsInFolders =					{ FNUP_FAMILY_LOCATION, 41 };
	const SysFileInfo S4LabourOptions =						{ FNUP_FAMILY_LOCATION, 42 };
	const SysFileInfo S4LabourOptionsTemp =					{ FNUP_FAMILY_LOCATION, 43 };
	const SysFileInfo LocationNotes =						{ FNUP_FAMILY_LOCATION, 44 };
	const SysFileInfo TableNames =							{ FNUP_FAMILY_LOCATION, 45 };
	const SysFileInfo TableGroups =							{ FNUP_FAMILY_LOCATION, 46 };
	const SysFileInfo TableGroupSets =						{ FNUP_FAMILY_LOCATION, 47 };
	const SysFileInfo TableAreas =							{ FNUP_FAMILY_LOCATION, 48 };
	const SysFileInfo ServerExport =						{ FNUP_FAMILY_LOCATION, 49 };
	const SysFileInfo ServerImport =						{ FNUP_FAMILY_LOCATION, 50 };
	const SysFileInfo ServerLogons =						{ FNUP_FAMILY_LOCATION, 51 };

	//stockpoint specific files
	const SysFileInfo SpAddress =							{ FNUP_FAMILY_STKPOINT, 0 };
	const SysFileInfo StockLevels =							{ FNUP_FAMILY_STKPOINT, 1 };
	const SysFileInfo StockTakes =							{ FNUP_FAMILY_STKPOINT, 2 };
	const SysFileInfo SpOrders =							{ FNUP_FAMILY_STKPOINT, 3 };
	//UNUSED 4004
	const SysFileInfo RedirectedSales =						{ FNUP_FAMILY_STKPOINT, 5 };
	const SysFileInfo RedirectedSalesPMS =					{ FNUP_FAMILY_STKPOINT, 6 };
	const SysFileInfo RedirectedDeliveries =				{ FNUP_FAMILY_STKPOINT, 7 };
	const SysFileInfo TransferDetail =						{ FNUP_FAMILY_STKPOINT, 8 };
	const SysFileInfo UnitechSpLock =						{ FNUP_FAMILY_STKPOINT, 9 };
	const SysFileInfo LastImportSheetFile =					{ FNUP_FAMILY_STKPOINT, 10 };
	
	//PC specific files
	//UNSED 5001 - 5003
	const SysFileInfo UnitechOptions =						{ FNUP_FAMILY_PC_CLIENT, 4 };
	const SysFileInfo UnitechOptionsLock =					{ FNUP_FAMILY_PC_CLIENT, 5 };
	const SysFileInfo UnitechBatch =						{ FNUP_FAMILY_PC_CLIENT, 6 };
	const SysFileInfo UnitechClear =						{ FNUP_FAMILY_PC_CLIENT, 7 };
	const SysFileInfo UnitechNetUseOn =						{ FNUP_FAMILY_PC_CLIENT, 8 };
	const SysFileInfo UnitechNetUseOff =					{ FNUP_FAMILY_PC_CLIENT, 9 };
	const SysFileInfo PCOptionsClient =						{ FNUP_FAMILY_PC_CLIENT, 10 };
	const SysFileInfo MiFareTerminalData =					{ FNUP_FAMILY_PC_CLIENT, 11 };
	const SysFileInfo PCOptionsHost =						{ FNUP_FAMILY_PC_HOST, 12 };
	const SysFileInfo PosTrayBackupMessage =				{ FNUP_FAMILY_PC_HOST, 13 };
	const SysFileInfo PosTrayBackupResult =					{ FNUP_FAMILY_PC_HOST, 14 };

	//temp instance files
	//UNUSED 6001 - 6004
	const SysFileInfo TempCommsLog =						{ FNUP_FAMILY_INSTANCE, 5 }; //.DAT
	const SysFileInfo BatchPassword =						{ FNUP_FAMILY_INSTANCE, 5 }; //.BAT
	//UNUSED 6006

	//ecr report param files
	const SysFileInfo EcrReportPrm =						{ FNUP_FAMILY_ECRREP, 1 };
	
	//stock audit param files
	const SysFileInfo StockAuditPrm =						{ FNUP_FAMILY_STOCKAUDIT, 1 };
	
	//location comms files
	//UNUSED 9001 - 9002
	const SysFileInfo LocPluEcr =							{ FNUP_FAMILY_LOC_COMMS, 3 };
		
	//site specific files
	const SysFileInfo SiteAddress =							{ FNUP_FAMILY_SITE, 0 };

	//network files
	const SysFileInfo NetworkCommsLock =					{ FNUP_FAMILY_NETWORK, 1 };
	const SysFileInfo IDraughtDate =						{ FNUP_FAMILY_NETWORK, 2 };
	const SysFileInfo SportsBookerSalesXML =				{ FNUP_FAMILY_NETWORK, 3 };
	const SysFileInfo SportsBookerDates =					{ FNUP_FAMILY_NETWORK, 4 };
	const SysFileInfo SmartEntertainmentSalesXML =			{ FNUP_FAMILY_NETWORK, 5 };
	const SysFileInfo SmartEntertainmentDates =				{ FNUP_FAMILY_NETWORK, 6 };

	//sysset files
	const SysFileInfo AutoBackupOptions =					{ FNUP_FAMILY_SYSSET, 1 };
	const SysFileInfo PosTrayEmailLock =					{ FNUP_FAMILY_SYSSET, 2 };
	//UNUSED D002
	const SysFileInfo PosTrayPrintLock =					{ FNUP_FAMILY_SYSSET, 4 };
	const SysFileInfo PMSLock =								{ FNUP_FAMILY_SYSSET, 5 };
	//UNUSED D006
	const SysFileInfo PosTrayStopFile =						{ FNUP_FAMILY_SYSSET, 7 };
	const SysFileInfo PosTrayUpdateFile =					{ FNUP_FAMILY_SYSSET, 8 };
	const SysFileInfo PosTrayLockFile =						{ FNUP_FAMILY_SYSSET, 9 };
	const SysFileInfo FNBSenderFTPLock =					{ FNUP_FAMILY_SYSSET, 10 };
	const SysFileInfo PosTrayManualExportCount =			{ FNUP_FAMILY_SYSSET, 11 };
	const SysFileInfo PosTrayFTPLock =						{ FNUP_FAMILY_SYSSET, 12 };
	const SysFileInfo S4LabourSenderFTPLock =				{ FNUP_FAMILY_SYSSET, 13 };
	const SysFileInfo PosTrayExportLock =					{ FNUP_FAMILY_SYSSET, 14 };
	const SysFileInfo PosTraySettingsLock =					{ FNUP_FAMILY_SYSSET, 15 };
	const SysFileInfo ChartwellsSenderFTPLock =				{ FNUP_FAMILY_SYSSET, 16 };

	//layout
	const SysFileInfo ButtonOptions =						{ FNUP_FAMILY_LAYOUT, 1 };
	const SysFileInfo ButtonActions =						{ FNUP_FAMILY_LAYOUT, 2 };

	//PMS SysFiles
	const SysFileInfo PMSOptionsOld =						{ FNUP_FAMILY_PMS_SYSFILE, 1 };
	const SysFileInfo PMSRooms =							{ FNUP_FAMILY_PMS_SYSFILE, 2 };
	//UNUSED E003
	const SysFileInfo PMSLockFile =							{ FNUP_FAMILY_PMS_SYSFILE, 4 };
	const SysFileInfo PMSRoomType =							{ FNUP_FAMILY_PMS_SYSFILE, 5 };
	const SysFileInfo PMSCustomerOld =						{ FNUP_FAMILY_PMS_SYSFILE, 6 };
	const SysFileInfo PMSInvoiceNumber =					{ FNUP_FAMILY_PMS_SYSFILE, 7 };
	const SysFileInfo PMSOptionsNew =						{ FNUP_FAMILY_PMS_SYSFILE, 8 };
	const SysFileInfo PMSClosureReasons =					{ FNUP_FAMILY_PMS_SYSFILE, 9 };
	const SysFileInfo PMSEmailSignatureBase =				{ FNUP_FAMILY_PMS_SYSFILE, 10 };
	const SysFileInfo PMSEmailSignatureExtra =				{ FNUP_FAMILY_PMS_SYSFILE, 11 };
	const SysFileInfo PMSRoomGroup	=						{ FNUP_FAMILY_PMS_SYSFILE, 12 };
	
	//Keyboard Set Files
	const SysFileInfo KbSetNames =							{ FNUP_FAMILY_KBSET, 1 };
	const SysFileInfo KbSetLock =							{ FNUP_FAMILY_KBSET, 2 };
	const SysFileInfo KbSetNameLock =						{ FNUP_FAMILY_KBSET, 3 };

	//PMS Booking
	const SysFileInfo PMSBooking =							{ FNUP_FAMILY_PMS_BOOKING, 1 };
	const SysFileInfo PMSClosure =							{ FNUP_FAMILY_PMS_BOOKING, 2 };

	//PMS Customer
	const SysFileInfo PMSCustomerNew =						{ FNUP_FAMILY_PMS_CUSTOMER, 1 };

	//IDraught files
	const SysFileInfo IDraughtLockFile =					{ FNUP_FAMILY_IDRAUGHT, 0 };
	const SysFileInfo IDraughtOptions =						{ FNUP_FAMILY_IDRAUGHT, 1 };
	const SysFileInfo IDraughtTempReport =					{ FNUP_FAMILY_IDRAUGHT, 2 };

	//Sage files
	const SysFileInfo SageLockFile =						{ FNUP_FAMILY_SAGE, 0 };
	const SysFileInfo SageOptions =							{ FNUP_FAMILY_SAGE, 1 };
	const SysFileInfo SageTempReport =						{ FNUP_FAMILY_SAGE, 2 };

	//FNB files
	const SysFileInfo FNBExportDate =						{ FNUP_FAMILY_FNB, 1 };
	const SysFileInfo FNBExportDateTemp =					{ FNUP_FAMILY_FNB, 2 };

	//S4 labour files
	const SysFileInfo S4LabourExportDate =					{ FNUP_FAMILY_S4LABOUR, 1 };
	const SysFileInfo S4LabourExportDateTemp =				{ FNUP_FAMILY_S4LABOUR, 2 };

	//Chartwells files
	const SysFileInfo ChartwellsExportDate =				{ FNUP_FAMILY_CHARTWELLS, 1 };
	const SysFileInfo ChartwellsExportDateTemp =			{ FNUP_FAMILY_CHARTWELLS, 2 };

	//LIPA files
	const SysFileInfo LIPAExportDate =						{ FNUP_FAMILY_LIPA, 1 };
	const SysFileInfo LIPAExportDateTemp =					{ FNUP_FAMILY_LIPA, 2 };

	//SPos Email files
	const SysFileInfo PosTrayLiveTask =						{ FNUP_FAMILY_EMAIL, 1 };
	const SysFileInfo PosTrayLiveTaskDate =					{ FNUP_FAMILY_EMAIL, 2 };
	const SysFileInfo PosTrayLiveTaskDateTemp =				{ FNUP_FAMILY_EMAIL, 3 };
};

/**********************************************************************/

class CFilenameUpdater  
{
public:
	CFilenameUpdater( SysFileInfo info, int nIndex = -1, int nSubInfo = -1, int nFolderFamily = -1 );
	
public:
	const char* GetFilenameToUse(){ return m_strFilenameToUse; }

public:
	const char* GetInstanceFilenameToUse();
	const char* GetInstanceFilenameToUse( int nInstanceNo );
		
private:
	CString m_strFilenameToUse;
	CString m_strInstanceFilenameToUse;
};

/**********************************************************************/
