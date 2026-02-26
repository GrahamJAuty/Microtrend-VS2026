#pragma once
 /**********************************************************************/
#define READ_STATUS_NONE 0
#define READ_STATUS_NOFILE 1
#define READ_STATUS_FAILED 2
#define READ_STATUS_OK 3
/**********************************************************************/
#define STOCKLEVELS_ACCESS_READWRITE 0
#define STOCKLEVELS_ACCESS_READONLY 1
#define STOCKLEVELS_ACCESS_NONE 2
#define STOCKLEVELS_ACCESS_PROMPT 3
/**********************************************************************/
#define SERVER_SORT_BYCODE 0
#define SERVER_SORT_BYNAME 1
#define SERVER_SORT_BYFILE 2
/**********************************************************************/
#define DEALERFLAG_REPORT		0
#define DEALERFLAG_REPORT1		1
#define DEALERFLAG_REPORT8		8
#define DEALERFLAG_REPORT9		9
#define DEALERFLAG_REPORT17		17
#define DEALERFLAG_REPORT18		18
#define DEALERFLAG_REPORT19		19
#define DEALERFLAG_REPORT20		20
/**********************************************************************/
#define DEALERFLAG_PLU			100
#define DEALERFLAG_PLU1			101
#define DEALERFLAG_PLU2			102
#define DEALERFLAG_PLU6			106
#define DEALERFLAG_PLU8			108
#define DEALERFLAG_PLU9			109
#define DEALERFLAG_PLU10		110
#define DEALERFLAG_PLU11		111
#define DEALERFLAG_PLU12		112
/**********************************************************************/
#define DEALERFLAG_CASHRSP		200
#define DEALERFLAG_CASHRSP1		201
#define DEALERFLAG_CASHRSP2		202
#define DEALERFLAG_CASHRSP3		203
#define DEALERFLAG_CASHRSP4		204
#define DEALERFLAG_CASHRSP5		205
#define DEALERFLAG_CASHRSP6		206
#define DEALERFLAG_CASHRSP7		207
#define DEALERFLAG_CASHRSP8		208
/**********************************************************************/
#define DEALERFLAG_MISC			300
//#define DEALERFLAG_MISC1		301 /OBSOLETE EPOS TITLE BAR FLAG
#define DEALERFLAG_MISC2		302
#define DEALERFLAG_MISC3		303
#define DEALERFLAG_MISC4		304
#define DEALERFLAG_MISC5		305
//#define DEALERFLAG_MISC6		306 //OBSOLETE JRTAYLOR FLAG
#define DEALERFLAG_MISC7		307
#define DEALERFLAG_MISC8		308
#define DEALERFLAG_MISC9		309
//#define DEALERFLAG_MISC12		312 //OBSOLETE FTP FLAG
//#define DEALERFLAG_MISC13		313	//OBSOLETE SMART ENTERTAINMENT FLAG
//#define DEALERFLAG_MISC14		314 //OBSOLETE KEYBOARD HANDSHAKE FLAG
#define DEALERFLAG_MISC15		315
#define DEALERFLAG_MISC16		316
#define DEALERFLAG_MISC17		317
#define DEALERFLAG_MISC18		318
#define DEALERFLAG_MISC19		319
#define DEALERFLAG_MISC20		320
#define DEALERFLAG_MISC21		321
#define DEALERFLAG_MISC22		322
#define DEALERFLAG_MISC23		323
#define DEALERFLAG_MISC25		325
#define DEALERFLAG_MISC26		326
#define DEALERFLAG_MISC27		327
#define DEALERFLAG_MISC28		328
#define DEALERFLAG_MISC29		329
#define DEALERFLAG_MISC30		330
//#define DEALERFLAG_MISC31		331 //OBSOLETE DATA TREE FLAG
#define DEALERFLAG_MISC32		332
#define DEALERFLAG_MISC33		333
#define DEALERFLAG_MISC34		334
#define DEALERFLAG_MISC35		335
#define DEALERFLAG_MISC36		336
#define DEALERFLAG_MISC37		337
#define DEALERFLAG_MISC38		338
#define DEALERFLAG_MISC39		339
#define DEALERFLAG_MISC40		340
#define DEALERFLAG_MISC41		341
#define DEALERFLAG_MISC42		342
#define DEALERFLAG_MISC43		343
#define DEALERFLAG_MISC44		344
#define DEALERFLAG_MISC45		345
#define DEALERFLAG_MISC46		346
#define DEALERFLAG_MISC47		347
#define DEALERFLAG_MISC48		348
#define DEALERFLAG_MISC49		349
#define DEALERFLAG_MISC50		350
#define DEALERFLAG_MISC51		351
/**********************************************************************/
#define DEALERFLAG_LABEL		400
#define DEALERFLAG_LABEL1		401
#define DEALERFLAG_LABEL2		402
/**********************************************************************/
#define DEALERFLAG_GRAPH		500
#define DEALERFLAG_GRAPH1		501
/**********************************************************************/
#define DEALERFLAG_SALESMAN		600
#define DEALERFLAG_SALESMAN1	601
#define DEALERFLAG_SALESMAN2	602
/**********************************************************************/
#define DEALERFLAG_STOCK		700
#define DEALERFLAG_STOCK1		701
#define DEALERFLAG_STOCK2		702
#define DEALERFLAG_STOCK3		703
#define DEALERFLAG_STOCK4		704
#define DEALERFLAG_STOCK5		705
#define DEALERFLAG_STOCK6		706
#define DEALERFLAG_STOCK7		707
#define DEALERFLAG_STOCK8		708
#define DEALERFLAG_STOCK9		709
#define DEALERFLAG_STOCK10		710
#define DEALERFLAG_STOCK11		711
#define DEALERFLAG_STOCK12		712
#define DEALERFLAG_STOCK13		713
/**********************************************************************/
#define DEALERFLAG_AUDIT		800
#define DEALERFLAG_AUDIT1		801
#define DEALERFLAG_AUDIT2		802
/**********************************************************************/
#define DEALERFLAG_ORDER		900
#define DEALERFLAG_ORDER1		901
/**********************************************************************/
#define DEALERFLAG_CUSTOMER		1100
#define DEALERFLAG_CUSTOMER1	1101
/**********************************************************************/
#define MAX_NOTOPUP_PLU 10
/**********************************************************************/

class CDealerFlags  
{
public:
	CDealerFlags();
	bool ReadFromFile();
	
	bool WriteToFile();
	void ReadFromArray( CStringArray& arrayFlags );
	void WriteToArray( CStringArray& arrayFlags );
	
	bool GetFatalReadErrorFlag(){ return m_bFatalReadError; }
	void ClearFatalReadErrorFlag() { m_bFatalReadError = FALSE; }

public:
	//REPORT
	bool GetLocComboOrderFlag() { return m_bLocComboOrder; } //1
	const char* GetLocComboOrder() { return m_strLocComboOrder; } //1
	void ProcessLocationSelectorOrder(); //1
	void GetLocationSelectorOrder( CWordArray& arrayNodeTypes ); //1
	bool GetSalesHistoryDateControlFlag(){ return m_bSalesHistoryDateControl; } //8
	int GetServerConsolidationType(){ return m_nServerConsolidationType; } //9
	bool GetHideNoDataFlag(){ return m_bHideNoData; } //17
	bool GetPrice1SeparateLineFlag(){ return m_bPrice1SeparateLine; } //18
	bool GetAssumeCoversFlag(){ return m_bAssumeCovers; } //19
	bool GetLoyaltyReconReportFlag() { return m_bLoyaltyReconReport; } //20

	//PLU
	int GetAddPluMode() { return m_nAddPluMode; } //1 & 2
	bool GetPriceSetExcludeByDefaultFlag() { return m_bPriceSetExcludeByDefault; } //6
	bool GetSpecialChangesOnlyFlag() { return m_bSpecialChangesOnly; } //8
	int GetPluCSVImportType(){ return m_nPluCSVImportType; } //9
	const char* GetDeptZeroText(){ return m_strDeptZeroText; } //10
	const char* GetGroupZeroText(){ return m_strGroupZeroText; } //10
	bool GetLegacyPluExportFlag() { return m_bLegacyPluExport; } //12

	//CASHRSP
	bool GetUseCashRSPTillNoFlag(){ return m_bUseCashRSPTillNo; } //1
	bool GetCashRSPPromoFixFlag(){ return m_bCashRSPPromoFix; } //2
	bool GetLogAllCashRSPFilesFlag(){ return m_bLogAllCashRSPFiles; } //3
	bool GetCashRSPV19SmartPayFlag(){ return m_bCashRSPV19SmartPay; } //4
	//OBSOLETE CASHRSP5, MOVED TO MISC 21
	bool GetCashRSPFixItemServerZeroFlag(){ return m_bCashRSPFixItemServerZero; } //6
	bool GetAutoRetryTermFileImportFlag(){ return m_bAutoRetryTermFileImport; } //7
	bool GetCashRSPCatchDuplicatesFlag(){ return m_bCashRSPCatchDuplicates; } //8

	//MISC
	bool GetIDraughtExportFlag(){ return m_bIDraughtExport; } //3
	bool GetBatchExportFlag(){ return m_bBatchExport; } //4
	int GetProdCSVImportExistingMode(){ return m_nProdCSVImportExistingMode; } //5
	int GetPrinterRedirectMode() { return m_nPrinterRedirectMode; } //7
	bool GetSageExportFlag(){ return m_bSageExport; } //8
	bool GetCascadeReportFlag(){ return m_bCascadeReport; } //9
	bool GetCostleyTransactionLogFlag(){ return m_bCostleyTransactionLog; } //15
	bool GetWeeklyReportNoZeroSkipFlag(){ return m_bWeeklyReportNoZeroSkip; } //16
	int GetSortCodeRenameType(){ return m_nSortCodeRenameType; } //17
	const char* GetSortCodeLabel();
	bool GetLogFileWritesFlag(){ return m_bLogFileWrites; } //18
	bool GetReportCostPriceFlag(){ return m_bReportCostPrice; } //19
	bool GetHoltsDiscountFlag(){ return m_bHoltsDiscount; } //21
	bool GetLocationNotesFlag(){ return m_bLocationNotes; } //25
	bool GetSpawnRepmanFlag(){ return m_bSpawnRepman; } //26
	
	int Get_LIPA_LJMU_ExportType()
	{
		switch (m_nLIPA_LJMU_ExportType)
		{
		case 1:
			return 1;

		case 2:
			return 2;

		default:
			return 0;
		}
	}

	bool GetLogNonDuplicatesFlag(){ return m_bLogNonDuplicates; } //28
	bool GetPMSExtendedRoomRateFlag(){ return m_bPMSExtendedRoomRate; } //29
	bool GetLogWebAPICallsFlag(){ return m_bLogWebAPICalls; } //30
	bool GetRemoveLoyaltyTopupFlag(){ return m_bRemoveLoyaltyTopup; } //32
	bool GetGlobalPromotionFlag(){ return m_bGlobalPromotion; } //33
	bool GetPosTraySetupNoLockAllUsersFlag() { return m_bPosTraySetupNoLockAllUsers; } //34 }
	bool GetReasonTicksNoDownloadFlag() { return m_bReasonTicksNoDownload; } //35
	bool GetReasonTicksVoidAfterFlag() { return m_bReasonTicksVoidAfter; } //36
	bool GetServerHomeLocationFlag() { return m_bServerHomeLocation; } //37
	bool GetServerImportExportFlag() { return m_bServerImportExport; } //38
	bool GetRefundFixFlag() { return m_bRefundFix; } //39
	bool GetIgnoreUnpaidTransactionsFlag() { return m_bIgnoreUnpaidTransactions; } //40
	bool GetRefund2FixFlag() { return m_bRefund2Fix; } //41
	bool GetGenRecExcludeLoyaltyFlag() { return m_bGenRecExcludeLoyalty; } //42
	bool GetUseDatabaseVATBandFlag() { return m_bUseDatabaseVATBand; } //43
	bool GetAllDiscountsVATableFlag() { return m_bAllDiscountsVATable; } //44
	bool Get15DigitKludgeDeptGroupFlag() { return m_b15DigitKludgeDeptGroup; } //45
	bool GetGratuityFixFlag() { return m_bGratuityFix; } //46
	bool GetGratuityFix2Flag() { return m_bGratuityFix2; } //47
	bool GetLinkPluFixFlag() { return m_bLinkPluFix; } //48
	bool GetDuplicateCheckIgnoreTimeFlag() { return m_bDuplicateCheckIgnoreTime; } //49
	bool GetFixDepositRecycleFlag() { return m_bFixDepositRecycle; } //50
	int GetFixDepositRedeemPayNo() { return m_nFixDepositRedeemPayNo; } //50
	int GetFixDepositRecyclePayNo() { return m_nFixDepositRecylePayNo; } //50
	__int64 GetFixDepositRecyclePluNo() { return m_nFixDepositRecyclePluNo; } //50
	bool GetCOFAllDiscountFlag() { return m_bCOFAllDiscount; } //51

	//LABEL
	bool GetEcrTextOnLabelsFlag() { return m_bEcrTextOnLabels; } //1
	int GetSPosBarcodeLength() { return m_nSPosBarcodeLength; } //2

	//GRAPH
	bool GetWeekNoGraphFlag() { return m_bWeekNoGraph; } //1

#if defined (STOCKMAN_SYSTEM) || defined (POSTRAY_UTILITY)

	//PLU
	bool GetImportBarcodeFlag() { return m_bImportBarcode; } //11

	//MISC
	bool GetBritanniaHotelsExportFlag(){ return m_bBritanniaHotelsExport; } //2
	int GetMaxBritanniaPriceLevel(); //2
	bool GetBritanniaAllItemsFlag(){ return m_bBritanniaAllItems; } //2
	bool GetRebuildStockSalesHistoryFlag(){ return m_bRebuildStockSalesHistory; } //20
	bool GetDorbiereImportFlag(){ return m_bDorbiereImport; } //22

	//STOCK
	bool GetSuppRefFlag() { return m_bSuppRef; } //1
	bool GetCopyPluToStockFlag() { return m_bCopyPluToStock; } //2
	bool GetObsoleteValuationSummaryFlag() { return m_bObsoleteValuationSummary; } //3
	int GetStockLevelsPluType() { return m_nStockLevelsPluType; } //4
	int GetStockLevelsStockType() { return m_nStockLevelsStockType; } //5
	bool GetTransitionalOrderFlag() { return m_bTransitionalOrder; } //6
	bool GetStockLevelFilterFlag() { return m_bStockLevelFilter; } //7
	bool GetExtendedStocktakeReportsFlag() { return m_bExtendedStocktakeReports; } //8
	bool GetObsoleteManualStockSalesFlag() { return m_bObsoleteManualStockSales; } //9
 	bool GetWarehouseToggleFlag() { return m_bWarehouseToggle; } //10
	const char* GetWarehouseType( bool bFirstLetterCaps ); //10
	bool GetExportLiveApparentStockFlag(){ return m_bExportLiveApparentStock; } //11
	bool GetPeriodDeliveryCostProfitFlag(){ return m_bPeriodDeliveryCostProfit; } //12
	bool GetStockReportUseRecipeCostLinkFlag(){ return m_bStockReportUseRecipeCostLink; } //13

	//AUDIT
	bool GetDeliveryLinesFlag() { return m_bDeliveryLines; } //1
	bool GetDeliveryDetailFlag() { return m_bDeliveryDetail; } //2

	//ORDER
	bool GetPurchaseOrderLinesFlag() { return m_bPurchaseOrderLines; } //1

	//NONFILE
	bool GetBlockExportLiveApparentStockFlag(){ return m_bBlockExportLiveApparentStock; }
	int GetCurrentStockLevelsAccessType() { return m_nCurrentStockLevelsAccessType; }
	
#else

	//MISC
	bool GetApparentStockDateFlag(){ return m_bApparentStockDate; } //23

#endif

public:
	//REPORT
	void SetLocComboOrderFlag( bool b ) { m_bLocComboOrder = b; } //1
	void SetLocComboOrder( const char* sz ) { m_strLocComboOrder = sz; } //1
	void SetSalesHistoryDateControlFlag( bool b ){ m_bSalesHistoryDateControl = b; } //8
	void SetServerConsolidationType( int n ); //9
	void SetHideNoDataFlag( bool b ){ m_bHideNoData = b; } //17
	void SetPrice1SeparateLineFlag( bool b ){ m_bPrice1SeparateLine = b; } //18
	void SetAssumeCoversFlag( bool b ){ m_bAssumeCovers = b; } //19
	void SetLoyaltyReconReportFlag(bool b) { m_bLoyaltyReconReport = b; } // 20

	//PLU
	void SetAddPluMode0() { m_nAddPluMode = 0; } //1 & 2
	void SetAddPluMode1() { m_nAddPluMode = 1; } //1 & 2	
	void SetAddPluMode2() { m_nAddPluMode = 2; } //1 & 2
	void SetPriceSetExcludeByDefaultFlag( bool b ) { m_bPriceSetExcludeByDefault = b; } //6
	void SetSpecialChangesOnlyFlag( bool b ){ m_bSpecialChangesOnly = b; } //8
	void SetPluCSVImportType( int n ); //9
	void SetDeptZeroText( const char* sz ){ m_strDeptZeroText = sz; } //10
	void SetGroupZeroText( const char* sz ) { m_strGroupZeroText = sz; } //10
	void SetLegacyPluExportFlag( bool b ){ m_bLegacyPluExport = b; } //12

	//CASHRSP
	void SetUseCashRSPTillNoFlag( bool b ){ m_bUseCashRSPTillNo = b; } //1
	void SetCashRSPPromoFixFlag( bool b ){ m_bCashRSPPromoFix = b; } //2
	void SetLogAllCashRSPFilesFlag( bool b ){ m_bLogAllCashRSPFiles = b; } //3
	void SetCashRSPV19SmartPayFlag( bool b ){ m_bCashRSPV19SmartPay = b; } //4
	//OBSOLETE CASHRSP5, MOVED TO MISC 21
	void SetCashRSPFixItemServerZeroFlag( bool b ){ m_bCashRSPFixItemServerZero = b; } //6
	void SetAutoRetryTermFileImportFlag( bool b ){ m_bAutoRetryTermFileImport = b; } //7
	void SetCashRSPCatchDuplicatesFlag( bool b ){ m_bCashRSPCatchDuplicates = b; } //8

	//MISC
	void SetIDraughtExportFlag( bool b ){ m_bIDraughtExport = b; } //3
	void SetBatchExportFlag( bool b ){ m_bBatchExport = b; } //4
	void SetPrinterRedirectMode( int n ); //7
	void SetSageExportFlag( bool b ){ m_bSageExport = b; } //8
	void SetCascadeReportFlag( bool b ){ m_bCascadeReport = b; } //9
	void SetCostleyTransactionLogFlag( bool b ){ m_bCostleyTransactionLog = b; } //15
	void SetWeeklyReportNoZeroSkipFlag( bool b ){ m_bWeeklyReportNoZeroSkip = b; } //16
	void SetSortCodeRenameType( int n ); //17
	void SetLogFileWritesFlag( bool b ){ m_bLogFileWrites = b; } //18
	void SetReportCostPriceFlag( bool b ){ m_bReportCostPrice = b; } //19
	void SetHoltsDiscountFlag( bool b ){ m_bHoltsDiscount = b; } //21
	void SetLocationNotesFlag( bool b ){ m_bLocationNotes = b; } //25
	void SetSpawnRepmanFlag( bool b ){ m_bSpawnRepman = b; } //26
	
	void Set_LIPA_LJMU_ExportType(int n)
	{
		if ((n >= 0) && (n <= 2))
		{
			m_nLIPA_LJMU_ExportType = n;
		}
	}
	
	void SetLogNonDuplicatesFlag( bool b ){ m_bLogNonDuplicates = b; } //28
	void SetPMSExtendedRoomRateFlag( bool b ){ m_bPMSExtendedRoomRate = b; } //29
	void SetLogWebAPICallsFlag( bool b ){ m_bLogWebAPICalls = b; } //30
	void SetRemoveLoyaltyTopupFlag( bool b ){ m_bRemoveLoyaltyTopup = b; } //32
	void SetGlobalPromotionFlag( bool b ){ m_bGlobalPromotion = b; } //33
	void SetPosTraySetupNoLockAllUsersFlag(bool b) { m_bPosTraySetupNoLockAllUsers = b; } //34
	void SetReasonTicksNoDownloadFlag(bool b) { m_bReasonTicksNoDownload = b; } // 35
	void SetReasonTicksVoidAfterFlag(bool b) { m_bReasonTicksVoidAfter = b; } // 36
	void SetServerHomeLocationFlag(bool b) { m_bServerHomeLocation = b; } // 37
	void SetServerImportExportFlag(bool b) { m_bServerImportExport = b; } // 38
	void SetRefundFixFlag(bool b) { m_bRefundFix = b; } // 39
	void SetIgnoreUnpaidTransactionsFlag(bool b) { m_bIgnoreUnpaidTransactions = b; } // 40
	void SetRefund2FixFlag(bool b) { m_bRefund2Fix = b; } // 41
	void SetGenRecExcludeLoyaltyFlag(bool b) { m_bGenRecExcludeLoyalty = b; } // 42
	void SetUseDatabaseVATBandFlag( bool b ) { m_bUseDatabaseVATBand = b; } //43
	void SetAllDiscountsVATableFlag( bool b ) { m_bAllDiscountsVATable = b; } //44
	void Set15DigitKludgeDeptGroupFlag(bool b) { m_b15DigitKludgeDeptGroup = b; } //45
	void SetGratuityFixFlag(bool b) { m_bGratuityFix = b; } //46
	void SetGratuityFix2Flag(bool b) { m_bGratuityFix2 = b; } //47
	void SetLinkPluFixFlag(bool b) { m_bLinkPluFix = b; } //48
	void SetDuplicateCheckIgnoreTimeFlag(bool b) { m_bDuplicateCheckIgnoreTime = b;  } //49
	void SetFixDepositRecycle(bool b, int nRedeemPayNo = 0, int nRecyclePayNo = 0, __int64 nPluNo = 0); //50
	void SetCOFAllDiscountFlag(bool b) { m_bCOFAllDiscount = b; } //51

	//LABEL
	void SetEcrTextOnLabelsFlag( bool b ) { m_bEcrTextOnLabels = b; } //1
	void SetSPosBarcodeLength( int n ); //2

	//GRAPH
	void SetWeekNoGraphFlag( bool b ) { m_bWeekNoGraph = b; } //1

#if defined (STOCKMAN_SYSTEM) || defined (POSTRAY_UTILITY)
	
	//PLU
	void SetImportBarcodeFlag( bool b ){ m_bImportBarcode = b; } //11

	//MISC
	void SetBritanniaHotelsExportFlag( bool b ){ m_bBritanniaHotelsExport = b; } //2
	void SetRebuildStockSalesHistoryFlag( bool b ){ m_bRebuildStockSalesHistory = b; } //20
	void SetDorbiereImportFlag( bool b ){ m_bDorbiereImport = b; } //22

	//STOCK
	void SetSuppRefFlag( bool b ) { m_bSuppRef = b; } //1
	void SetCopyPluToStockFlag( bool b ) { m_bCopyPluToStock = b; } //2
	void SetObsoleteValuationSummaryFlag( bool b ) { m_bObsoleteValuationSummary = b; } //3
	void SetStockLevelsPluType( int n ); //4
	void SetStockLevelsStockType( int n ); //5
	void SetTransitionalOrderFlag( bool b ){ m_bTransitionalOrder = b; } //6
	void SetStockLevelFilterFlag( bool b ){ m_bStockLevelFilter = b; } //7
	void SetExtendedStocktakeReportsFlag( bool b ){ m_bExtendedStocktakeReports = b; } //8
	void SetObsoleteManualStockSalesFlag( bool b ){ m_bObsoleteManualStockSales = b; } //9
	void SetWarehouseToggleFlag( bool b ){ m_bWarehouseToggle = b; } //10
	void SetExportLiveApparentStockFlag( bool b ){ m_bExportLiveApparentStock = b; } //11
	void SetPeriodDeliveryCostProfitFlag( bool b ){ m_bPeriodDeliveryCostProfit = b; } //12
	void SetStockReportUseRecipeCostLinkFlag( bool b ){ m_bStockReportUseRecipeCostLink = b; } //13

	//AUDIT
	void SetDeliveryLinesFlag( bool b ) { m_bDeliveryLines = b; } //1
	void SetDeliveryDetailFlag( bool b ) { m_bDeliveryDetail = b; } //2

	//ORDER
	void SetPurchaseOrderLinesFlag( bool b ) { m_bPurchaseOrderLines = b; } //1
	
	//NONFILE
	void SetBlockExportLiveApparentStockFlag( bool b ){ m_bBlockExportLiveApparentStock = b; }
	void SetCurrentStockLevelsAccessType( int n );

#else

	void SetApparentStockDateFlag( bool b ){ m_bApparentStockDate = b; } //23

#endif

private:
	//REPORT
	bool m_bLocComboOrder; //1
	CString m_strLocComboOrder;
	int m_nPriorityDb; //1
	int m_nPriorityLoc; //1
	int m_nPriorityRp; //1
	int m_nPriorityLs; //1
	int m_nPrioritySite; //1
	int m_nPrioritySp; //1 
	int m_nPrioritySys; //1
	bool m_bSalesHistoryDateControl; //8
	int m_nServerConsolidationType; //9
	bool m_bHideNoData; //17
	bool m_bPrice1SeparateLine; //18
	bool m_bAssumeCovers; //19
	bool m_bLoyaltyReconReport; //20

	//PLU
	int m_nAddPluMode; //1 & 2
	bool m_bPriceSetExcludeByDefault; //6
	bool m_bSpecialChangesOnly; //8
	int m_nPluCSVImportType; //9
	CString m_strDeptZeroText; //10
	CString m_strGroupZeroText; //10
	bool m_bLegacyPluExport; //12

	//CASHRSP
	bool m_bUseCashRSPTillNo; //1
	bool m_bCashRSPPromoFix; //2
	bool m_bLogAllCashRSPFiles; //3
	bool m_bCashRSPV19SmartPay; //4
	//OBSOLETE CASHRSP5, MOVED TO MISC 21
	bool m_bCashRSPFixItemServerZero; //6
	bool m_bAutoRetryTermFileImport; //7
	bool m_bCashRSPCatchDuplicates; //8

	//MISC
	bool m_bIDraughtExport; //3
	bool m_bBatchExport; //4
	int m_nProdCSVImportExistingMode; //5
	int m_nPrinterRedirectMode; //7
	bool m_bSageExport; //8
	bool m_bCascadeReport; //9
	bool m_bCostleyTransactionLog; //15
	bool m_bWeeklyReportNoZeroSkip; //16
	int m_nSortCodeRenameType; //17
	CString m_strSortCodeLabel; //17
	bool m_bLogFileWrites; //18
	bool m_bReportCostPrice; //19
	bool m_bHoltsDiscount; //21
	bool m_bLocationNotes; //25
	bool m_bSpawnRepman; //26
	int m_nLIPA_LJMU_ExportType; //27
	bool m_bLogNonDuplicates; //28
	bool m_bPMSExtendedRoomRate; //29
	bool m_bLogWebAPICalls; //30
	bool m_bRemoveLoyaltyTopup; //32
	CArray<__int64, __int64> m_arrayNoLoyaltyTopupPLUs; //32
	bool m_bGlobalPromotion; //33
	bool m_bPosTraySetupNoLockAllUsers; //34
	bool m_bReasonTicksNoDownload; // 35
	bool m_bReasonTicksVoidAfter; // 36
	bool m_bServerHomeLocation; //37
	bool m_bServerImportExport; //38
	bool m_bRefundFix; //39
	bool m_bIgnoreUnpaidTransactions; //40
	bool m_bRefund2Fix; //41
	bool m_bGenRecExcludeLoyalty; //42
	bool m_bUseDatabaseVATBand; //43
	bool m_bAllDiscountsVATable; //44
	bool m_b15DigitKludgeDeptGroup; //45
	bool m_bGratuityFix; //46
	bool m_bGratuityFix2; //47
	bool m_bLinkPluFix; //48
	bool m_bDuplicateCheckIgnoreTime; //49
	bool m_bFixDepositRecycle; //50
	int m_nFixDepositRedeemPayNo; //50
	int m_nFixDepositRecylePayNo; //50
	__int64 m_nFixDepositRecyclePluNo; //50
	bool m_bCOFAllDiscount; //51
	
	//LABEL
	bool m_bEcrTextOnLabels; //1
	int m_nSPosBarcodeLength; //2

	//GRAPH
	bool m_bWeekNoGraph; //1

#if defined (STOCKMAN_SYSTEM) || defined (POSTRAY_UTILITY)

	//PLU
	bool m_bImportBarcode; //11

	//MISC
	bool m_bBritanniaHotelsExport; //2
	int m_nMaxBritanniaPriceLevel; //2
	bool m_bBritanniaAllItems; //2
	bool m_bRebuildStockSalesHistory; //20
	bool m_bDorbiereImport; //22
	
	//STOCK
	bool m_bSuppRef; //1
	bool m_bCopyPluToStock; //2
	bool m_bObsoleteValuationSummary; //3
	int m_nStockLevelsPluType; //4
	int m_nStockLevelsStockType; //5
	bool m_bTransitionalOrder; //6
	bool m_bStockLevelFilter; //7
	bool m_bExtendedStocktakeReports; //8
	bool m_bObsoleteManualStockSales; //9
	bool m_bWarehouseToggle; //10
	CString m_strWarehouseType; //10
	bool m_bExportLiveApparentStock; //11
	bool m_bPeriodDeliveryCostProfit; //12
	bool m_bStockReportUseRecipeCostLink; //13

	//AUDIT
	bool m_bDeliveryLines; //1
	bool m_bDeliveryDetail; //2

	//ORDER
	bool m_bPurchaseOrderLines; //1

	//NON FILE
	int m_nCurrentStockLevelsAccessType;
	bool m_bBlockExportLiveApparentStock;

#else

	//MISC
	bool m_bApparentStockDate; //23
	
#endif
	
public:	
	int GetReadStatus() { return m_nReadStatus; }
	void AddToLocComboString( CCSV& csvOut, CString& strTest );

public:
	CString GetMisc32String();
	bool CheckNoLoyaltyTopupPLU(__int64 nPluNo); //32
	void ClearNoLoyaltyTopupPLUs(){ m_arrayNoLoyaltyTopupPLUs.RemoveAll(); } //32
	void AddNoLoyaltyTopupPLU(__int64 nPluNo); //32
	int GetNoLoyaltyTopupPLUCount() { return m_arrayNoLoyaltyTopupPLUs.GetSize(); } //32

private:
	void Reset();

	void ReadInternal();
	bool ReadNextLine( CString& strLine );
	
	void WriteInternal();
	void WriteNextLine( const char* sz );

	void CheckFlag( int nBase, const char* szType, CString& strFlag, int& nResult );
	void WriteFlag( bool bFlag, const char* szText, int nExtra = 9999 );

private:
	int m_nReadStatus;
	
private:
	bool m_bFatalReadError;

	bool m_bFromFile;
	CStringArray* m_pArray;
	CSSFile* m_pFile;
	int m_nArrayPos;
};

/**********************************************************************/
extern CDealerFlags DealerFlags;
/**********************************************************************/

