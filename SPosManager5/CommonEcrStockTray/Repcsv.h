#pragma once
/**********************************************************************/
 
/**********************************************************************/
#define ENTRY_MODE_SUPPREF 0
#define ENTRY_MODE_PLUNO 1
#define ENTRY_MODE_STOCKCODE 2
#define ENTRY_MODE_WRAPPER 3
/**********************************************************************/

class CCSVRecipeBuffer : public CCSV
{
public:
	CCSVRecipeBuffer ( const char* szCSVLine ) : CCSV(szCSVLine) {}
	int GetStockIdx() { return GetInt ( 0 ); }
	double GetCost() { return GetDouble ( 1 ); }
	double GetQty() { return GetDouble ( 2 ); }
};

/**********************************************************************/

class CCSVExplodedPlu : public CCSV
{
public:
	CCSVExplodedPlu ( const char* szCSVLine ) : CCSV(szCSVLine) {}
	__int64 GetPluNo() { return GetInt64( 0 ); }
	double GetPluQty() { return GetDouble( 1 ); }
	double GetPluValue() { return GetDouble( 2 ); }
	const char* GetDescription() { return GetString( 3 ); }
	const char* GetStockCode() { return GetString( 4 ); }	
	const char* GetProblem() { return GetString( 5 ) ; }		
	int GetStockIdx() { return GetInt( 6 ); }
	double GetStockQty() { return GetDouble( 7 ); }
	double GetStockValue() { return GetDouble( 8 ); }
	int GetSubUnits() { return GetInt ( 9 ); }
};

/**********************************************************************/

class CCSVStockSalesKey : public CCSV
{
public:
	CCSVStockSalesKey ( const char* szCSVLine ) : CCSV(szCSVLine){}
	bool PrintEpos1() { return GetBool (0); }
	bool PrintEpos2() { return GetBool (1); }
	bool PrintEpos3() { return GetBool (2); }
	bool PrintEpos4() { return GetBool (3); }
	bool PrintImport() { return GetBool (4); }
	bool PrintManual() { return GetBool (5); }
	bool PrintRecipe() { return GetBool (6); }
	bool PrintAverage() { return GetBool (7); }
	bool ZeroSkip() { return GetBool(8); }
	int GetSalesReportType() { return GetInt(9); }
	bool PrintEposSummary() { return GetBool(10); }
	const char* GetDateSelectType() { return GetString(11); }
	const char* GetDateFrom() { return GetString(12); }
	const char* GetDateTo() { return GetString(13); }
	bool PrintSupplier() { return GetBool(14); }
	bool PrintTax() { return GetBool(15); }
	bool PrintCustomer() { return GetBool(16); }
	bool PrintEpos5(){ return GetBool(17); }
	bool PrintEpos6(){ return GetBool(18); }
	bool PrintEpos7(){ return GetBool(19); }
	bool PrintEpos8(){ return GetBool(20); }
	bool PrintEpos9(){ return GetBool(21); }
	bool PrintEpos10(){ return GetBool(22); }
};

/**********************************************************************/

class CCSVSalesHistory : public CCSV
{
public:
	CCSVSalesHistory ( const char* szCSVLine ) : CCSV(szCSVLine){}
	int GetMDay() { return GetInt(0); }
	const char* GetTime() { return GetString(1); }
	const char* GetStockCode() { return GetString(2); }
	int GetType() { return GetInt(3); }			
	double GetStockQty() { return GetDouble(4); }
	double GetStockValue() { return GetDouble(5); }
	int GetCategory() { return GetInt(6); }
	bool GetReorderedFlag() { return GetBool (7); } 
	int GetPriceBand() { return GetInt(8); }

public:
	void SetCategory(int n) { SetAt(6,n); }
	void SetReorderedFlag(bool b) { SetAt(7,b); }
};

/**********************************************************************/

class CCSVInternalReorderInfo : public CCSV
{
public:
	CCSVInternalReorderInfo ( const char* szCSVLine ) : CCSV(szCSVLine) {}
	int GetSpIdx() { return GetInt(0); }
	double GetApparentQty() { return GetDouble(1); }
	double GetBelowQty() { return GetDouble(2); }
	double GetSURequired() { return GetDouble(3); }
	double GetSUOnOrderQty() { return GetDouble(4); }
	double GetSUShortQty() { return GetDouble(5); }
	double GetDUItemSize() { return GetDouble(6); }
	int GetDUItemsRequired() { return GetInt(7); }
	int GetDUItemsPerDU() { return GetInt(8); }
	int GetDURequired() { return GetInt(9); }
	int GetDUItemsSurplus() { return GetInt(10); }
};

/**********************************************************************/

class CCSVPluDelivery : public CCSV
{
public:
	CCSVPluDelivery ( const char* szCSVLine ) : CCSV(szCSVLine) {}
	const char* GetDate() { return GetString(0); }
	const char* GetTime() { return GetString(1); }
	__int64 GetPluNo() { return GetInt64(2); }
	const char* GetRepText() { return GetString(3); }
	int GetDeptNo() { return GetInt(4); }
	bool GetClearFlag() { return GetBool(5); }
	double GetQty() { return GetDouble(6); }
	int GetSupplierNo() { return GetInt(7); }
};

/**********************************************************************/

class CCSVManualSales : public CCSV
{
public:
	CCSVManualSales( const char* szCSVLine ) : CCSV (szCSVLine) {}
	int GetSalesType() { return GetInt(0); }
	__int64 GetPluNo() { return GetInt64(1); }
	const char* GetDescription() { return GetString(2); }
	int GetPriceLevel() { return GetInt(3); }
	double GetQty() { return GetDouble(4); }
	double GetValue() { return GetDouble(5); }
	int GetDeptNo() { return GetInt(6); }
	int GetItemLookupMethod() { return GetInt(7); }
	const char* GetStockCode() { return GetString(8); }
	int GetModifier() { return GetInt(9); }
	double GetMultiplier() { return GetDouble(10); }
	double GetTotalQty() { return GetDouble(11); }
};

/**********************************************************************/

class CCSVAdjustmentPlu : public CCSV
{
public:
	CCSVAdjustmentPlu( const char* szCSVLine ) : CCSV (szCSVLine) {}
	__int64 GetPluNo() { return GetInt64(0); }
	const char* GetDescription() { return GetString(1); }
	int GetPriceLevel() { return GetInt(2); }
	double GetQty() { return GetDouble(3); }
	double GetValue() { return GetDouble(4); }
	int GetDeptNo() { return GetInt(5); }
	int GetItemLookupMethod() { return GetInt(6); }
	const char* GetStockCode() { return GetString(7); }
	int GetModifier() { return GetInt(8); }
	double GetMultiplier() { return GetDouble(9); }
	double GetTotalQty() { return GetDouble(10); }
	const char* GetReason(){ return GetString(11); }
};

/**********************************************************************/

class CCSVMiscStockLevels : public CCSV
{
public:
	CCSVMiscStockLevels( const char* szCSVLine ) : CCSV (szCSVLine) {}
	__int64 GetPluNoInt() { return GetInt64(0); }
	const char* GetPluNoString() { return GetString(0); }
	const char* GetSuppRef() { return GetString(1); }
	const char* GetStockCode() { return GetString(2); }
	const char* GetDescription() { return GetString(3); }
	bool GetFreeStockFlag() { return GetBool(4); }
	bool GetUseDUFlag() { return GetBool(5); }
	double GetDUQty() { return GetDouble(6); }
	int GetDUItems() { return GetInt(7); }
	double GetDUItemSize() { return GetDouble(8); }
	double GetSUQty() { return GetDouble(9); }
	double GetDUCost() { return GetDouble(10); }
	double GetTotalCost() { return GetDouble(11); }
	int GetItemLookupMethod() { return GetInt(12); }
	int GetSuppNo() { return GetInt(13); } 
	const char* GetWrapperCode() { return GetString(14); }
	int GetSalesType(){ return GetInt(15); }
	int GetStockSuppIdx() { return GetInt(16); }
};

/**********************************************************************/

class CCSVAdjustmentInfo : public CCSV
{
public:
	CCSVAdjustmentInfo( const char* szCSVLine ) : CCSV (szCSVLine) {}
	__int64 GetPluNo() { return GetInt64(0); }
	const char* GetStockCode() { return GetString(1); }
	const char* GetDescription() { return GetString(2); }
	double GetQuantity() { return GetDouble(3); }
	double GetValue() { return GetDouble(4); }
	int GetItemLookupMethod() { return GetInt(5); }
	int GetSubUnits() { return GetInt(6); }
	const char* GetReason() { return GetString(7); }	
};

/**********************************************************************/

class CCSVPendingStock : public CCSV
{
public:
	CCSVPendingStock( const char* szCSVLine ) : CCSV (szCSVLine) {}
	const char* GetDate(){ return GetString(0); }
	const char* GetTime(){ return GetString(1); }
	__int64 GetPluNo(){ return GetInt64(2); }
	int GetLevel() { return GetInt(3); }
	double GetQty() { return GetDouble(4); }
	double GetValue() { return GetDouble(5); }
	bool GetWastageFlag(){ return GetBool(6); }
};

/**********************************************************************/

class CCSVPendingStockDelivery : public CCSV
{
public:
	CCSVPendingStockDelivery( const char* szCSVLine ) : CCSV (szCSVLine) {}
	const char* GetDate(){ return GetString(0); }
	const char* GetTime(){ return GetString(1); }
	__int64 GetPluNo(){ return GetInt64(2); }
	double GetQty() { return GetDouble(3); }
};

/**********************************************************************/

class CCSVStockAudit : public CCSV
{
public:
	void AddStockAuditQty( double dQty ) { Add ( dQty, 5, TRUE ); }
	void AddStockAuditVal( double dVal ) { Add ( dVal, 3, TRUE ); }
};

/**********************************************************************/

class CCSVTransactionData : public CCSV
{
public:
	CCSVTransactionData ( const char* szCSVLine ) : CCSV(szCSVLine) {}
	const char* GetTime() { return GetString(0); }
	int GetServer() { return GetInt(1); }
	double GetQty() { return GetDouble(2); }
	double GetValue() { return GetDouble(3); }
	int GetCovers() { return GetInt(4); }
	int GetTrainingMode() { return GetInt(5); }
	int GetPaymentNumber() { return GetInt(6); }
	bool GetWastageFlag() { return GetBool(7); }
};

/**********************************************************************/

class CCSVEposTimeAttendData : public CCSV
{
public:
	CCSVEposTimeAttendData ( const char* szCSVLine ) : CCSV(szCSVLine) {}
	const char* GetLineType(){ return GetString(0); }
	__int64 GetEventId(){ return GetInt64(1); }
	int GetEditCount(){ return GetInt(2); }
	const char* GetEventDate(){ return GetString(3); }
	const char* GetEventTime(){ return GetString(4); }
	int GetServerNo(){ return GetInt(5); }
	int GetEventType(){ return GetInt(6); }
	int GetTerminalNo(){ return GetInt(7); }
	int GetEditServer(){ return GetInt(8); }
	const char* GetEditDate(){ return GetString(9); }
	const char* GetEditTime(){ return GetString(10); }
	const char* GetOriginalDate(){ return GetString(11); }
	const char* GetOriginalTime(){ return GetString(12); }
};

/**********************************************************************/

class CCSVReportTimeAttendData : public CCSV
{
public:
	CCSVReportTimeAttendData ( const char* szCSVLine ) : CCSV(szCSVLine) {}
	int GetEditCount(){ return GetInt(0); }
	int GetEventType(){ return GetInt(1); }
	int GetTerminalNo(){ return GetInt(2); }
	const char* GetEventDateUTC(){ return GetString(3); }
	const char* GetEventTimeUTC(){ return GetString(4); }
	const char* GetEventDateReport(){ return GetString(5); }
	const char* GetEventTimeReport(){ return GetString(6); }
	int GetEditServer(){ return GetInt(7); }
	const char* GetEditDate(){ return GetString(8); }
	const char* GetEditTime(){ return GetString(9); }
};

/**********************************************************************/

class CCSVReasonDataCON : public CCSV
{
public:
	CCSVReasonDataCON ( const char* szCSVLine ) : CCSV(szCSVLine, ' ' ) {}
	int GetTNo(){ return GetInt(0); }
	int GetCause(){ return GetInt(1); }
	const char* GetDate(){ return GetString(2); }
	const char* GetTime(){ return GetString(3); }
	int GetServer(){ return GetInt(4); }
	int GetTable(){ return GetInt(5); }
	__int64 GetPluNo(){ return GetInt64(6); }
	int GetReasonCode(){ return GetInt(7); }
};

/**********************************************************************/

class CCSVReasonDataCSV : public CCSV
{
public:
	CCSVReasonDataCSV ( const char* szCSVLine ) : CCSV(szCSVLine, ',' ) {}
	
	int GetVersion(){ return GetInt(0); }
	
	int GetTNo(){ return GetInt(1); }
	int GetCause(){ return GetInt(2); }
	const char* GetDate(){ return GetString(3); }
	const char* GetTime(){ return GetString(4); }
	int GetServer(){ return GetInt(5); }
	int GetTable(){ return GetInt(6); }
	__int64 GetPluNo(){ return GetInt64(7); }
	int GetReasonCode(){ return GetInt(8); }
	const char* GetReasonText(){ return GetString(9); }
	int GetValue(){ return GetInt(10); }
};

/**********************************************************************/
#define ACCOUNT_LINETYPE_HEADER 0
#define ACCOUNT_LINETYPE_CHARGE 1
#define ACCOUNT_LINETYPE_RECEIVE 2
#define ACCOUNT_LINETYPE_PAYMENT_RA 3
#define ACCOUNT_LINETYPE_MARKER 4
#define ACCOUNT_LINETYPE_PAYMENT_MIXED 5
#define ACCOUNT_LINETYPE_REFUND 6
/**********************************************************************/

class CCSVAccountHistory : public CCSV
{
public:
	CCSVAccountHistory ( const char* szCSVLine ) : CCSV(szCSVLine ){}
	
	int GetAccountLineType(){ return GetInt(0); }
	//LINE TYPE 0 = HEADER
	const char* GetHeaderLineTime(){ return GetString(1); }
	int GetHeaderLineServerNo(){ return GetInt(2); }
	//LINE TYPE 1 = CHARGE TO ACCOUNT
	int GetChargeType(){ return GetInt(1); }
	int GetChargeID(){ return GetInt(2); }
	double GetChargeTender(){ return GetDouble(3); }
	double GetChargeAmount(){ return GetDouble(4); }
	//LINE TYPE 2 = RECEIVE ON ACCOUNT
	int GetReceiveType(){ return GetInt(1); }
	int GetReceiveID(){ return GetInt(2); }
	double GetReceiveValue(){ return GetDouble(3); }
	//LINE TYPE 3 = RA PAYMENT / 5 = MIXED PAYMENT
	int GetPaymentNumber(){ return GetInt(1); }
	double GetPaymentAmount(){ return GetDouble(2); }
	//LINE TYPE 4 = CUSTOMER MARKER
	int GetMarkerType(){ return GetInt(1); }
	int GetMarkerID(){ return GetInt(2); }
	//LINE TYPE 6 = REFUND
	int GetRefundType(){ return GetInt(1); }
	int GetRefundID(){ return GetInt(2); }
	double GetRefundValue(){ return GetDouble(3); }
};

/**********************************************************************/
