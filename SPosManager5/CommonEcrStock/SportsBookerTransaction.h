#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
 
/**********************************************************************/

class CSportsBookerOrderItem
{
public:
	CSportsBookerOrderItem(void);
	~CSportsBookerOrderItem(void);

public:
	const char* GetPluNo(){ return m_strPluNo; }
	const char* GetPriceBand(){ return m_strPriceBand; }
	const char* GetDeptNo(){ return m_strDeptNo; }
	const char* GetTaxBand(){ return m_strTaxBand; }
	int GetQty(){ return m_nQty; }
	const char* GetCostPerItem(){ return m_strCostPerItem; }
	
public:
	void SetPluNo( const char* sz ){ m_strPluNo = sz; }
	void SetPriceBand( const char* sz ){ m_strPriceBand = sz; }
	void SetDeptNo( const char* sz ){ m_strDeptNo = sz; }
	void SetTaxBand( const char* sz ){ m_strTaxBand = sz; }
	void SetQty( int n ){ m_nQty = n; }
	void SetCostPerItem( const char* sz ){ m_strCostPerItem = sz; }
	
public:
	void Reset();

private:
	CString m_strPluNo;
	CString m_strPriceBand;
	CString m_strDeptNo;
	CString m_strTaxBand;
	int m_nQty;
	CString m_strCostPerItem;
};

/**********************************************************************/

class CSportsBookerTransaction
{
public:
	CSportsBookerTransaction(void);
	~CSportsBookerTransaction(void);

public:
	void Reset();

public:
	const char* GetDate(){ return m_strDate; }
	const char* GetType(){ return m_strType; }
	const char* GetTranId(){ return m_strTranId; }
	const char* GetAmount(){ return m_strAmount; }
	const char* GetUserId(){ return m_strUserId; }
	const char* GetPriceBand(){ return m_strPriceBand; }
	const char* GetPaymentId(){ return m_strPaymentId; }
	const char* GetPaymentName(){ return m_strPaymentName; }
	const char* GetDescription(){ return m_strDescription; }
	int GetQty(){ return m_nQty; }
	
public:
	void SetDate( const char* sz ){ m_strDate = sz; }
	void SetType( const char* sz ){ m_strType = sz; }
	void SetTranId( const char* sz ){ m_strTranId = sz; }
	void SetAmount( const char* sz ){ m_strAmount = sz; }
	void SetUserId( const char* sz ){ m_strUserId = sz; }
	void SetPriceBand( const char* sz ){ m_strPriceBand = sz; }
	void SetPaymentId( const char* sz ){ m_strPaymentId = sz; }
	void SetPaymentName( const char* sz ){ m_strPaymentName = sz; }
	void SetDescription( const char* sz ){ m_strDescription = sz; }
	
public:
	int GetItemCount(){ return m_arrayItems.GetCount(); }
	void AddOrderItem( CSportsBookerOrderItem& orderItem );
	void GetOrderItem( int nIdx, CSportsBookerOrderItem& orderItem );
	
public:
	const char* GetCashRSPDate();
	const char* GetCashRSPTime();
	
private:
	CString m_strDate;
	CString m_strType;
	CString m_strTranId;
	CString m_strAmount;
	CString m_strUserId;
	CString m_strPriceBand;
	CString m_strPaymentId;
	CString m_strPaymentName;
	CString m_strDescription;
	int m_nQty;
	
private:
	CString m_strCashRSPDate;
	CString m_strCashRSPTime;

private:
	CArray<CSportsBookerOrderItem,CSportsBookerOrderItem> m_arrayItems;
};

/**********************************************************************/
#endif
/**********************************************************************/
