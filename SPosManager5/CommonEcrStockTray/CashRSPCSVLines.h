#pragma once
/**********************************************************************/
#define ENTRY_MODE_SUPPREF 0
#define ENTRY_MODE_PLUNO 1
#define ENTRY_MODE_STOCKCODE 2
#define ENTRY_MODE_WRAPPER 3
/**********************************************************************/
#define CASHRSP_LINETYPE_UNKNOWN 0
#define CASHRSP_LINETYPE_TRAN_NORMAL 1
#define CASHRSP_LINETYPE_TRAN_VOID 2
#define CASHRSP_LINETYPE_WASTAGE 3
#define CASHRSP_LINETYPE_NOSALE 4
#define CASHRSP_LINETYPE_AGEVERIFY 5
#define CASHRSP_LINETYPE_PAYMENT 6
#define CASHRSP_LINETYPE_ITEM 7
#define CASHRSP_LINETYPE_DISCOUNT_ITEM 8
#define CASHRSP_LINETYPE_DISCOUNT_SUBTOTAL 9
#define CASHRSP_LINETYPE_DISCOUNT_LOYALTY 10
#define CASHRSP_LINETYPE_ATTENDANCE 11
#define CASHRSP_LINETYPE_COMMENT_GENERAL 12
#define CASHRSP_LINETYPE_COMMENT_PMS 13
#define CASHRSP_LINETYPE_SSFT_DELIVERY 14
#define CASHRSP_LINETYPE_TRAN_PMS 15
#define CASHRSP_LINETYPE_DEPOSIT_PMS 16
#define CASHRSP_LINETYPE_HEADER_PMS 17
#define CASHRSP_LINETYPE_DISCOUNT_VOID 18
#define CASHRSP_LINETYPE_PAIDIO 19
#define CASHRSP_LINETYPE_VERSION 20
#define CASHRSP_LINETYPE_DISCOUNTTAX_SUBTOTAL 21
#define CASHRSP_LINETYPE_DISCOUNTTAX_LOYALTY 22
#define CASHRSP_LINETYPE_APPLICATION 23
#define CASHRSP_LINETYPE_SSFT_FOOTER 24
#define CASHRSP_LINETYPE_DEPOSITSTART 25
#define CASHRSP_LINETYPE_DEPOSITEND 26
#define CASHRSP_LINETYPE_DISCOUNT_PROMO 27
#define CASHRSP_LINETYPE_DISCOUNT_MIXMATCH 28
#define CASHRSP_LINETYPE_EODHEADER 29
#define CASHRSP_LINETYPE_EODVAR 30
#define CASHRSP_LINETYPE_EODVARTOT 31
#define CASHRSP_LINETYPE_EODVARFX 32
#define CASHRSP_LINETYPE_EODVARFXTOT 33
#define CASHRSP_LINETYPE_EODEND 34
#define CASHRSP_LINETYPE_MIXMATCH_USE 35
/**********************************************************************/
#define CASHRSP_ITEMPAYTYPE_NORMAL 0
#define CASHRSP_ITEMPAYTYPE_DEPOSIT 1
#define CASHRSP_ITEMPAYTYPE_CUSTOMER 2
#define CASHRSP_ITEMPAYTYPE_ROOM 3
#define CASHRSP_ITEMPAYTYPE_LOYALTY 4
#define CASHRSP_ITEMPAYTYPE_SMARTPAY 5
#define CASHRSP_ITEMPAYTYPE_SMARTPHONE 6
#define CASHRSP_ITEMPAYTYPE_SPTBOOK 7
/**********************************************************************/
#define DEPOSIT_ADJUST_ID -1
/**********************************************************************/

class CCashRSPVersionChecker;

class CCSVEposTermLine : public CCSV
{
public:
	CCSVEposTermLine(const char* szCSVLine);
	const char* GetLineTypeString() { return GetString(0); }
	int GetLineTypeInt();

	//TRANSACTION LINE $TRAN/$VTRN/$WAST
	int GetTransactionNumber() { return GetInt(1); }
	int GetTransactionTable() { return GetInt(2); }
	double GetTransactionQty() { return GetDouble(3); }
	int	GetTransactionCovers() { return GetInt(4); }
	//5...  ITEM COUNT
	int	GetTransactionValue() { return GetInt(6); }
	const char* GetTransactionDate() { return GetString(7); }
	const char* GetTransactionTime() { return GetString(8); }
	int	GetTransactionServer() { return GetInt(9); }
	const char* GetTransactionTradingDate( int nVersionNo );
	const char* GetTransactionEmail( int nVersionNo );
	int	GetTransactionReasonCode( int nVersionNo );
	const char*	GetTransactionReasonText( int nVersionNo );
	const char*	GetTransactionPaidIOLabel( int nVersionNo );
	const char*	GetTransactionReverseLabel( int nVersionNo );
	__int64 GetTransactionLoyaltyIDAsInt64(int nVersionNo);
	const char* GetTransactionLoyaltyIDAsString(int nVersionNo);

	//NOSALE LINE $NOSALE
	int GetNoSaleNumber() { return GetInt(1); }
	const char* GetNoSaleDate() { return GetString(2); }
	const char* GetNoSaleTime() { return GetString(3); }
	int	GetNoSaleServer() { return GetInt(4); }
	//5... TRADING DATE
	int	GetNoSaleReasonCode() { return GetInt(6); }
	const char* GetNoSaleReasonText() { return GetString(7); }
	
	//AGE VERIFICATION LINE $AGEVERIFY
	int GetAgeVerifyNumber() { return GetInt(1); }
	const char* GetAgeVerifyDate() { return GetString(2); }
	const char* GetAgeVerifyTime() { return GetString(3); }
	int	GetAgeVerifyServer() { return GetInt(4); }
	int	GetAgeVerifyResult() { return GetInt(5); }
	int	GetAgeVerifyLimit() { return GetInt(6); }

	//ITEM LINE $ITEM
	__int64 GetItemPluNo() { return GetInt64(1); }
	const char* GetItemPluNoString(){ return GetString(1); }
	int	GetItemDeptNo() { return GetInt(2); }
	double GetItemQty() { return GetDouble(3); }
	int	GetItemPriceBand() { return GetInt(4); }
	int GetItemUnitPrice() { return GetInt(5); }
	const char* GetItemTaxBand() { return GetString(6); }
	const char* GetItemRefundFlag() { return GetString(7); }
	const char* GetItemVoidFlag() { return GetString(8); }
	int GetItemGroupNo() { return GetInt(9); }
	//10..  PRICE OVERRIDE
	int GetItemDatabasePrice() { return GetInt(11); }
	int GetItemLinePrice() { return GetInt(12); }
	const char* GetItemBarCode() { return GetString(13); }
	int GetItemLegacyDepositID() { return GetInt(14); }
	int GetItemLineDiscountAmount(){ return GetInt(15); }
	//16 = ITEM TYPE
	//17 = ITEM ID
	void GetItemTypeAndID( CCashRSPVersionChecker& VersionChecker, int& nType, int& nID32, __int64& nID64 );
	//18 == SERVER NO
	int GetItemServerNo( int nVersionNo );
	//19 == ANALYSIS CATEGEORY
	int GetItemAnalysisCategory( int nVersionNo );
	//20 == MIX MATCH NUMBER
	int GetItemMixMatchNumber( int nVersionNo );
	//21 == MIX MATCH TYPE
	int GetItemMixMatchType( int nVersionNo );
	//22 == VOID TYPE
	const char* GetItemVoidBAType( int nVersionNo );
	//23 == SALE DATE
	const char* GetItemSaleDate( int nVersionNo );
	//24 = SALE TIME
	const char* GetItemSaleTime( int nVersionNo );
	//25 = REASON CODE
	int GetItemGeneralReasonCode( int nVersionNo );
	//26 = REASON TEXT
	const char* GetItemGeneralReasonText( int nVersionNo );
	//27 = ITEM DESCRIPTION
	const char* GetItemDescription( int nVersionNo );
	//28 = PRODUCT TYPE
	int GetItemProductType( int nVersionNo );
	//29 = WASTAGE FLAG
	bool GetItemWastageVoidFlag( int nVersionNo );
	//30 = VIP STATUS
	int GetItemVIPStatus( int nVersionNo );
	//31 = WASTAGE REASON CODE
	int GetItemWastageReasonCode( int nVersionNo );
	//32 = WASTAGE REASON TEXT
	const char* GetItemWastageReasonText( int nVersionNo );
	//33 = GET PROMO EXPORT TYPE
	int GetItemPromoExportType( int nVersionNo );
	//34 = TERMINAL NO
	int GetItemTerminalNo( int nVersionNo );
	//35 = VOID / REFUND AUTHORISATION SERVER
	int GetItemAuthServerNo( int nVersionNo );
	//36 = FREE TEXT
	const char* GetItemFreeText( int nVersionNo );

	double GetItemLineValueReport();

	void SetItemUnitPrice(int n) { SetAt(5, n); }
	void SetItemLinePrice(int n) { SetAt(12, n); }

	//PAYMENT LINE $PAYM
	int GetPaymentNumber() { return GetInt(1); }
	const char* GetPaymentName() { return GetString(2); }
	int GetPaymentTender() { return GetInt(3); }
	int GetPaymentAmount() { return GetInt(4); }
	int GetPaymentGratuity() { return GetInt(5); }
	int GetPaymentCashback() { return GetInt(6); }
	//7 = LEGACY ID
	int GetPaymentType() { return GetInt(8); }
	//9 = PAYMENT ID
	void GetPaymentAccountTypeAndID( CCashRSPVersionChecker& VersionChecker, int& nType, int& nID32, __int64& nID64, bool& bIsCash );
	//10 = TERMINAL NO
	int GetPaymentTerminalNo( int nVersionNo );
	//11 = TERMINAL NO
	int GetPaymentSurplus( int nVersionNo );

	void SetPaymentNumber(int n) { SetAt(1, n); }
	void SetPaymentTender(int n) { SetAt(3, n); }
	void SetPaymentAmount(int n) { SetAt(4, n); }
	void SetPaymentGratuity(int n) { SetAt(5, n); }

	//SUBTOTAL DISCOUNT $SDISC
	int GetSubtotalDiscountAmount() { return GetInt(1); }
	const char* GetSubtotalDiscountType() { return GetString(2); }
	int GetSubtotalDiscountRate(){ return GetInt(3); }
	int GetSubtotalDiscountPromo() { return GetInt(4); }
	bool GetSubtotalDiscountCashRSPTaxFlag() { return GetBool(5); }
	bool GetSubtotalDiscountMicrotrendTaxFlag();
	//6 = ANALYSIS CATEGORIES
	const char* GetSubtotalDiscountAnalysisCategories( int nVersionNo );
	
	//ITEM DISCOUNT $IDISC
	const char* GetItemDiscountPluNo() { return GetString(1); }
	__int64 GetItemDiscountPluNoInt() { return GetInt64(1); }
	int GetItemDiscountAmount() { return GetInt(2); }
	const char* GetItemDiscountType() { return GetString(3); }
	int GetItemDiscountRate(){ return GetInt(4); }
	//5 = BARCODE
	//6 = RESERVED
	//7 = SUBTOTAL DISCOUNT
	//8 = SERVER NO
	int GetItemDiscountServerNo( int nVersionNo );
	//9 = PROMO NO
	int GetItemDiscountPromoNo( int nVersionNo );
	//10 = ANALYSIS CATEGORY
	int GetItemDiscountAnalysisCategory( int nVersionNo );
	//11 = ANALYSIS CATEGORY
	const char* GetItemDiscountDescription( int nVersionNo );
	//12 = REASON CODE
	int	GetItemDiscountReasonCode( int nVersionNo );
	//13 = REASON TEXT
	const char*	GetItemDiscountReasonText( int nVersionNo );
	//14 = AUTHORISED SERVER NUMBER
	int	GetItemDiscountAuthorisedServerNo( int nVersionNo );
	//15 = DATE
	const char*	GetItemDiscountDate( int nVersionNo );
	//16 = TIME
	const char*	GetItemDiscountTime( int nVersionNo );
	//17 = TERMINAL NO
	int	GetItemDiscountTerminalNo( int nVersionNo );
	//18 = APPLIED AS
	const char*	GetItemDiscountAppliedAs( int nVersionNo );
	//19 = COST OF PLUS
	int	GetItemDiscountPluCost( int nVersionNo );
	//20 = QUANTITY OF PLUS
	int	GetItemDiscountPluQty( int nVersionNo );
	//21 = TAXABLE
	bool GetItemDiscountCashRSPTaxFlag( int nVersionNo );

	//VOID DISCOUNT $VDDISC
	int GetVoidDiscountAmount() { return GetInt(1); }
	const char* GetVoidDiscountType() { return GetString(2); }
	int GetVoidDiscountRate(){ return GetInt(3); }
	int GetVoidDiscountPromo() { return GetInt(4); }
	int GetVoidDiscountUsage() { return GetInt(5); }
	//6 == VOID TYPE
	const char* GetVoidDiscountBAType( int nVersionNo );
	//7 == REASON DATE
	const char* GetVoidDiscountReasonDate( int nVersionNo );
	//8 = REASON TIME
	const char* GetVoidDiscountReasonTime( int nVersionNo );
	//9 = REASON CODE
	int GetVoidDiscountReasonCode( int nVersionNo );
	//10 = REASON TEXT
	const char* GetVoidDiscountReasonText( int nVersionNo );
	//11 = DISCOUNT DESCRIPTION
	const char* GetVoidDiscountDescription( int nVersionNo );
	//12 = SERVER NO
	int GetVoidDiscountAuthServerNo( int nVersionNo );

	//PROMO DISCOUNT LINE $PDISC
	const char* GetPromoDiscountPluNo() { return GetString(1); }
	__int64 GetPromoDiscountPluNoInt() { return GetInt64(1); }
	int GetPromoDiscountPromoAmount() { return GetInt(2); }
	const char* GetPromoDiscountType() { return GetString(3); }
	int GetPromoDiscountRate(){ return GetInt(4); }
	//5 == RESERVED
	//6 == RESERVED
	//7 == RESERVED
	//8 == RESERVED
	int GetPromoDiscountPromoNo(){ return GetInt(9); }
	const char* GetPromoDiscountAnalysisCategories(){ return GetString(10); }
	const char* GetPromoDiscountDescription(){ return GetString(11); }
	int GetPromoDiscountReasonCode(){ return GetInt(12); }
	const char* GetPromoDiscountReasonText(){ return GetString(13); }
	int	GetPromoDiscountAuthorisedServerNo(){ return GetInt(14); }
	const char*	GetPromoDiscountDate(){ return GetString(15); }
	const char*	GetPromoDiscountTime(){ return GetString(16); }
	int	GetPromoDiscountTerminalNo(){ return GetInt(17); }
	const char*	GetPromoDiscountAppliedAs(){ return GetString(18); }
	int	GetPromoDiscountPluCost(){ return GetInt(19); }
	int	GetPromoDiscountPluQty(){ return GetInt(20); }
	bool GetPromoDiscountCashRSPTaxFlag(){ return GetBool(21); }
	bool GetPromoDiscountMicrotrendTaxFlag();
	
	//MIXMATCH DISCOUNT LINE $MMDISC
	const char* GetMixMatchDiscountPluNo() { return GetString(1); }
	__int64 GetMixMatchDiscountPluNoInt() { return GetInt64(1); }
	int GetMixMatchDiscountMixMatchNo() { return GetInt(2); }
	const char* GetMixMatchDiscountDescription() { return GetString(3); }
	int GetMixMatchDiscountAmount() { return GetInt(4); }
	int	GetMixMatchDiscountPluQty(){ return GetInt(5); }
	int	GetMixMatchDiscountPluCost(){ return GetInt(6); }
	int GetMixMatchDiscountType() { return GetInt(7); }
	int GetMixMatchDiscountRate(){ return GetInt(8); }
	int GetMixMatchDiscountAnalysisCategory(){ return GetInt(9); }
	const char* GetMixMatchDiscountTaxBand() { return GetString(10); }
	int GetMixMatchDiscountPromoNo(int nVersionNo);
	int GetMixMatchDiscountNSplit(int nVersionNo);

	//MIXMATCH USE LINE $MMUSE
	CString GetMMUseID() { return GetString(1); }
	int GetMMNo() { return GetInt(2); }
	int GetMMUseCount() { return GetInt(3); }

	//PAID IN/OUT $PAIDIO
	int GetPaidIOTillNo(){ return GetInt(1); }
	int GetPaidIODirection(){ return GetInt(2); }
	int GetPaidIONumber(){ return GetInt(3); }
	int GetPaidIOQty(){ return GetInt(4); }
	const char* GetPaidIOVoidFlag(){ return GetString(5); }
	int GetPaidIOValue(){ return GetInt(6); }

	//PAID IN/OUT $PAIDIO
	const char* GetDiscountTaxBand() { return GetString(1); }
	int GetDiscountTaxBandAmount() { return GetInt(2); }

	//VERSION NO
	int GetVersionNo(){ return m_nVersionNo; }

	//VERSION NO
	int GetApplicationType(){ return m_nApplicationType; }

private:
	int m_nVersionNo;
	int m_nApplicationType;

private:
	CString m_strField;
	CString m_strSubtotalDiscountAnalysisCategories;
};

/**********************************************************************/

class CCSVPMSTermLine : public CCSV
{
public:
	CCSVPMSTermLine ( const char* szCSVLine ) : CCSV(szCSVLine) {}
	const char* GetLineType() { return GetString(0); }

	//TRANSACTION LINE $TRAN/$VTRN/$WAST
	int GetTransactionNumber() { return GetInt(1); }
	int GetTransactionTable() { return GetInt(2); }
	double GetTransactionQty() { return GetDouble(3); }
	int	GetTransactionCovers() { return GetInt(4); }
	//5...  ITEM COUNT
	int	GetTransactionValue() { return GetInt(6); }
	const char* GetTransactionDate() { return GetString(7); }
	const char* GetTransactionTime() { return GetString(8); }
	int	GetTransactionServer() { return GetInt(9); }

	//ITEM LINE $ITEM
	__int64 GetItemPluNo() { return GetInt64(1); }
	int	GetItemDeptNo() { return GetInt(2); }
	double GetItemQty() { return GetDouble(3); }
	int	GetItemPriceBand() { return GetInt(4); }
	int GetItemUnitPrice() { return GetInt(5); }
	const char* GetItemTaxBand() { return GetString(6); }
	int GetItemNumericTaxBand();
	const char* GetItemRefundFlag() { return GetString(7); }
	int GetItemLinePrice() { return GetInt(8); }
	//9... COST
	const char* GetItemGroupNoString(){ return GetString(10); }
	int GetItemGroupNoInt(){ return GetInt(10); }

	double GetItemLineValueReport();
	void SetItemNumericTaxBand( int n );
	
	//SUBTOTAL DISCOUNT $SDISC
	int GetSubtotalDiscountAmount() { return GetInt(1); }
	const char* GetSubtotalDiscountType() { return GetString(2); }
	int GetSubtotalDiscountRate(){ return GetInt(3); }
	
	//ITEM DISCOUNT $IDISC
	const char* GetItemDiscountPluNo() { return GetString(1); }
	__int64 GetItemDiscountPluNoInt() { return GetInt64(1); }
	int GetItemDiscountAmount() { return GetInt(2); }
	const char* GetItemDiscountType() { return GetString(3); }
	int GetItemDiscountRate(){ return GetInt(4); }
};

/**********************************************************************/

