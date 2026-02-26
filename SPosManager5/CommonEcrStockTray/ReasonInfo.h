#pragma once
/**********************************************************************/
#define SS_REASON_REFUNDVOID 0
#define SS_REASON_DISCOUNT_VOID 1
#define SS_REASON_DISCOUNT_SUBT 2
#define SS_REASON_DISCOUNT_ITEM 3
#define SS_REASON_WASTAGE 4
#define SS_REASON_VIP 5
/**********************************************************************/
 
/**********************************************************************/

class CReasonInfo
{
public:
	CReasonInfo();
	void LoadCSVLine( CCSV& csv );
	void LoadLegacyLine( CCSV& csv );
	void SaveCSVLine( CCSV& csv );

private:
	void Reset();

public:
	int GetReasonType(){ return m_nReasonType; }
	int GetReasonCode(){ return m_nReasonCode; }
	const char* GetReasonText(){ return m_strReasonText; }
	const char* GetTimeTran(){ return m_strTimeTran; }
	const char* GetTimeAction(){ return m_strTimeAction; }
	int GetTranSeqNo(){ return m_nTranSeqNo; }
	int GetTranTableNo(){ return m_nTranTableNo; }
	int GetTranCovers(){ return m_nTranCovers; }
	int GetTranServerNo(){ return m_nTranServerNo; }

public:
	bool GetItemVoidFlag(){ return m_bItemVoid; }
	bool GetItemRefundFlag(){ return m_bItemRefund; }
	bool GetItemWastageVoidFlag(){ return m_bItemWastageVoid; }
	int GetItemServerNo(){ return m_nItemServerNo; }
	__int64 GetItemPluNo(){ return m_nItemPluNo; }
	int GetItemPriceBand(){ return m_nItemPriceBand; }
	double GetItemQty(){ return m_dItemQty; }
	double GetItemVal(){ return m_dItemVal; }
	const char* GetItemTaxBand(){ return m_strItemTaxBand; }
	int GetItemDeptNo(){ return m_nItemDeptNo; }
	int GetItemGroupNo(){ return m_nItemGroupNo; }
	int GetItemACatNo(){ return m_nItemACatNo; }
	const char* GetItemVoidBAType(){ return m_strItemVoidBAType; }
	int GetItemAuthServerNo(){ return m_nItemAuthServerNo; }
	
public:
	int GetVoidDiscountAmount(){ return m_nVoidDiscountAmount; }
	const char* GetVoidDiscountType(){ return m_strVoidDiscountType; }
	int GetVoidDiscountRate(){ return m_nVoidDiscountRate; }
	int GetVoidDiscountPromoNo(){ return m_nVoidDiscountPromoNo; }
	int GetVoidDiscountUsage(){ return m_nVoidDiscountUsage; }
	const char* GetVoidDiscountBAType(){ return m_strVoidDiscountBAType; }

public:
	int GetSDiscPromoNo(){ return m_nSDiscPromoNo; }
	const char* GetSDiscType(){ return m_strSDiscType; }
	int GetSDiscRate(){ return m_nSDiscRate; }
	int GetSDiscAmount(){ return m_nSDiscAmount; }

public:
	int GetIDiscServerNo(){ return m_nIDiscServerNo; }
	__int64 GetIDiscPluNo(){ return m_nIDiscPluNo; }
	int GetIDiscPromoNo(){ return m_nIDiscPromoNo; }
	int GetIDiscACatNo(){ return m_nIDiscACatNo; }
	const char* GetIDiscType(){ return m_strIDiscType; }
	int GetIDiscRate(){ return m_nIDiscRate; }
	int GetIDiscAmount(){ return m_nIDiscAmount; }

public:
	void SetReasonType( int n ){ m_nReasonType = n; }
	void SetReasonCode( int n ){ m_nReasonCode = n; }
	void SetReasonText( const char* sz ){ m_strReasonText = sz; }
	void SetTimeTran( const char* sz ){ m_strTimeTran = sz; }
	void SetTimeAction( const char* sz ){ m_strTimeAction = sz; }
	void SetTranSeqNo( int n ){ m_nTranSeqNo = n; }
	void SetTranTableNo( int n ){ m_nTranTableNo = n; }
	void SetTranCovers( int n ){ m_nTranCovers = n; }
	void SetTranServerNo( int n ){ m_nTranServerNo = n; }

public:
	void SetItemVoidFlag( bool b ){ m_bItemVoid = b; }
	void SetItemRefundFlag( bool b ){ m_bItemRefund = b; }
	void SetItemWastageVoidFlag( bool b ){ m_bItemWastageVoid = b; }
	void SetItemServerNo( int n ){ m_nItemServerNo = n; }
	void SetItemPluNo( __int64 n ){ m_nItemPluNo = n; }
	void SetItemPriceBand( int n ){ m_nItemPriceBand = n; }
	void SetItemQty( double d ){ m_dItemQty = d; }
	void SetItemVal( double d ){ m_dItemVal = d; }
	void SetItemTaxBand( const char* sz ){ m_strItemTaxBand = sz; }
	void SetItemDeptNo( int n ){ m_nItemDeptNo = n; }
	void SetItemGroupNo( int n ){ m_nItemGroupNo = n; }
	void SetItemACatNo( int n ){ m_nItemACatNo = n; }
	void SetItemVoidBAType( const char* sz ){ m_strItemVoidBAType = sz; }
	void SetItemAuthServerNo( int n ){ m_nItemAuthServerNo = n; }

public:
	void SetVoidDiscountAmount( int n ){ m_nVoidDiscountAmount = n; }
	void SetVoidDiscountType( const char* sz ){ m_strVoidDiscountType = sz; }
	void SetVoidDiscountRate( int n ){ m_nVoidDiscountRate = n; }
	void SetVoidDiscountPromoNo( int n ){ m_nVoidDiscountPromoNo = n; }
	void SetVoidDiscountUsage( int n ){ m_nVoidDiscountUsage = n; }
	void SetVoidDiscountBAType( const char* sz ){ m_strVoidDiscountBAType = sz; }

public:
	void SetSDiscPromoNo( int n ){ m_nSDiscPromoNo = n; }
	void SetSDiscType( const char* sz ){ m_strSDiscType = sz; }
	void SetSDiscRate( int n ){ m_nSDiscRate = n; }
	void SetSDiscAmount( int n ){ m_nSDiscAmount = n; }

public:
	void SetIDiscServerNo( int n ){ m_nIDiscServerNo = n; }
	void SetIDiscPluNo( __int64 n ){ m_nIDiscPluNo = n; }
	void SetIDiscPromoNo( int n ){ m_nIDiscPromoNo = n; }
	void SetIDiscACatNo( int n ){ m_nIDiscACatNo = n; }
	void SetIDiscType( const char* sz ){ m_strIDiscType = sz; }
	void SetIDiscRate( int n ){ m_nIDiscRate = n; }
	void SetIDiscAmount( int n ){ m_nIDiscAmount = n; }

private:
	int m_nReasonType;
	int m_nReasonCode;
	CString m_strReasonText;
	CString m_strTimeTran;
	CString m_strTimeAction;
	int m_nTranSeqNo;
	int m_nTranTableNo;
	int m_nTranCovers;
	int m_nTranServerNo;

private:
	bool m_bItemVoid;
	bool m_bItemRefund;
	bool m_bItemWastageVoid;
	int m_nItemServerNo;
	__int64 m_nItemPluNo;
	int m_nItemPriceBand;
	double m_dItemQty;
	double m_dItemVal;
	CString m_strItemTaxBand;
	int m_nItemDeptNo;
	int m_nItemGroupNo;
	int m_nItemACatNo;
	CString m_strItemVoidBAType;
	int m_nItemAuthServerNo;

private:
	int m_nVoidDiscountAmount;
	CString m_strVoidDiscountType;
	int m_nVoidDiscountRate;
	int m_nVoidDiscountPromoNo;
	int m_nVoidDiscountUsage;
	CString m_strVoidDiscountBAType;

private:
	int m_nSDiscPromoNo;
	const char* m_strSDiscType;
	int m_nSDiscRate;
	int m_nSDiscAmount;

private:
	int m_nIDiscServerNo;
	__int64 m_nIDiscPluNo;
	int m_nIDiscPromoNo;
	int m_nIDiscACatNo;
	const char* m_strIDiscType;
	int m_nIDiscRate;
	int m_nIDiscAmount;
};

/**********************************************************************/

struct CReasonFileListInfo
{
	int m_nSelIdx;
	int m_nNetworkIdx;
	CString m_strFilename;
};

/**********************************************************************/
