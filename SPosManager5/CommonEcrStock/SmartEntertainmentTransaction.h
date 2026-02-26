#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
 
/**********************************************************************/

class CSmartEntertainmentItem
{
public:
	CSmartEntertainmentItem(void);
	~CSmartEntertainmentItem(void);

public:
	const char* GetPluNo(){ return m_strPluNo; }
	int GetDeptNo(){ return m_nDeptNo; }
	int GetQty(){ return m_nQty; }
	const char* GetLineCost(){ return m_strLineCost; }
	const char* GetDescription(){ return m_strDescription; }
	
public:
	void SetPluNo( const char* sz ){ m_strPluNo = sz; }
	void SetDeptNo( int n ){ m_nDeptNo = n; }
	void SetQty( int n ){ m_nQty = n; }
	void SetLineCost( const char* sz ){ m_strLineCost = sz; }
	void SetDescription( const char* sz ){ m_strDescription = sz; }

public:
	void Reset();

private:
	CString m_strPluNo;
	int m_nDeptNo;
	int m_nQty;
	CString m_strLineCost;
	CString m_strDescription;
};

/**********************************************************************/

class CSmartEntertainmentPayment
{
public:
	CSmartEntertainmentPayment(void);
	~CSmartEntertainmentPayment(void);

public:
	int GetID(){ return m_nID; }
	const char* GetAmount(){ return m_strAmount; }
	
public:
	void SetID( int n ){ m_nID = n; }
	void SetAmount( const char* sz ){ m_strAmount = sz; }
	
public:
	void Reset();

private:
	int m_nID;
	CString m_strAmount;
};

/**********************************************************************/

class CSmartEntertainmentTransaction
{
public:
	CSmartEntertainmentTransaction(void);
	~CSmartEntertainmentTransaction(void);

public:
	void Reset();

public:
	const char* GetSaleDate(){ return m_strSaleDate; }
	const char* GetPaymentDate(){ return m_strPaymentDate; }
	const char* GetTranId(){ return m_strTranId; }
	const char* GetAmount(){ return m_strAmount; }

public:
	void SetSaleDate( const char* sz ){ m_strSaleDate = sz; }
	void SetPaymentDate( const char* sz ){ m_strPaymentDate = sz; }
	void SetTranId( const char* sz ){ m_strTranId = sz; }
	void SetAmount( const char* sz ){ m_strAmount = sz; }
	
public:
	int GetQty(){ return m_nQty; }
	int GetItemCount(){ return m_arrayItems.GetCount(); }
	void AddItem( CSmartEntertainmentItem& Item );
	void GetItem( int nIdx, CSmartEntertainmentItem& Item );

public:
	int GetPaymentCount(){ return m_arrayPayments.GetCount(); }
	void AddPayment( CSmartEntertainmentPayment& Payment );
	void GetPayment( int nIdx, CSmartEntertainmentPayment& Payment );
	
public:
	const char* GetCashRSPSaleDate();
	const char* GetCashRSPSaleTime();
	const char* GetCashRSPPaymentDate();
	const char* GetCashRSPPaymentTime();

private:
	const char* GetCashRSPDate( CString& strDateTime );
	const char* GetCashRSPTime( CString& strDateTime );

private:
	CString m_strSaleDate;
	CString m_strPaymentDate;
	CString m_strTranId;
	CString m_strAmount;
	int m_nQty;
	
private:
	CString m_strCashRSPDate;
	CString m_strCashRSPTime;

private:
	CArray<CSmartEntertainmentItem,CSmartEntertainmentItem> m_arrayItems;
	CArray<CSmartEntertainmentPayment,CSmartEntertainmentPayment> m_arrayPayments;
};

/**********************************************************************/
#endif
/**********************************************************************/
