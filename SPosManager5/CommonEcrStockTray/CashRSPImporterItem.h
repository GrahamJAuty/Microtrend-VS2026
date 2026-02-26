#pragma once
/**********************************************************************/

class CCashRSPImporterItem
{
public:
	CCashRSPImporterItem();

public:
	void SetPluNo( __int64 n )				{ m_nPluNo = n; }
	void SetVoidFlag( bool b )				{ m_bVoid = b; }
	void SetRefundFlag( bool b )			{ m_bRefund = b; }
	void SetWastageVoidFlag( bool b )		{ m_bWastageVoid = b; }
	void Set15DigitFlag( bool b )			{ m_b15Digit = b; }
	void SetRAFlag( bool b )				{ m_bRA = b; }
	void SetAccountType( int n )			{ m_nAccountType = n; }
	void SetAccountID64( __int64 n )		{ m_nAccountID = n; }
	void SetPriceBand( int n )				{ m_nPriceBand = n; }
	void SetDeptNo( int n )					{ m_nDeptNo = n; }
	void SetGroupNo( int n )				{ m_nGroupNo = n; }
	void SetTaxBand( const char* sz )		{ m_strTaxBand = sz; }
	void SetServerNo( int n )				{ m_nServerNo = n; }
	void SetQty( double d )					{ m_dQty = d; }
	void SetLineValueAsInt( int n )			{ m_nLineValueAsInt = n; }
	void SetLineValueAsDouble( double d )	{ m_dLineValueAsDouble = d; }
	void SetACatNo( int n )					{ m_nACatNo = n; }
	void SetMixMatchNo( int n )				{ m_nMixMatchNo = n; }
	void SetMixMatchType( int n )			{ m_nMixMatchType = n; }
	void SetSaleDate( const char* sz )		{ m_strSaleDate = sz; }
	void SetSaleTime( const char* sz )		{ m_strSaleTime = sz; }
	void SetVoidBAType( const char* sz )	{ m_strVoidBAType = sz; }
	void SetAuthServerNo( int n )			{ m_nAuthServerNo = n; }
	void SetVIPStatus( int n )				{ m_nVIPStatus = n; }
	void SetPromoExportType( int n )		{ m_nPromoExportType = n; }
	void SetReasonCode( int n )				{ m_nReasonCode = n; }
	void SetReasonText( const char* sz )	{ m_strReasonText = sz; }
	void SetWastageCode( int n )			{ m_nWastageCode = n; }
	void SetWastageText( const char* sz )	{ m_strWastageText = sz; }
	void SetTranSeqNo( int n )				{ m_nTranSeqNo = n; }
	void SetDescription( const char* sz )	{ m_strDescription = sz; }
	void SetAttributeText(const char* sz)	{ m_strAttributeText = sz; }

public:
	__int64 GetPluNoInt()					{ return m_nPluNo; }
	const char* GetPluNoString();
	bool GetVoidFlag()						{ return m_bVoid; }
	bool GetRefundFlag()					{ return m_bRefund; }
	bool GetWastageVoidFlag()				{ return m_bWastageVoid; }
	bool Get15DigitFlag()					{ return m_b15Digit; }
	bool GetRAFlag()						{ return m_bRA; }
	int GetAccountType()					{ return m_nAccountType; }
	__int64 GetAccountID64()				{ return m_nAccountID; }
	int GetPriceBand()						{ return m_nPriceBand; }
	int GetDeptNo()							{ return m_nDeptNo; }
	int GetGroupNo()						{ return m_nGroupNo; }
	const char* GetTaxBand()				{ return m_strTaxBand; }
	int GetServerNo()						{ return m_nServerNo; }
	double GetQty()							{ return m_dQty; }
	int GetLineValueAsInt()					{ return m_nLineValueAsInt; }
	double GetLineValueAsDouble()			{ return m_dLineValueAsDouble; }
	int GetACatNo()							{ return m_nACatNo; }
	int GetMixMatchNo()						{ return m_nMixMatchNo; }
	int GetMixMatchType()					{ return m_nMixMatchType; }
	const char* GetSaleDate()				{ return m_strSaleDate; }
	const char* GetSaleTime()				{ return m_strSaleTime; }
	const char* GetVoidBAType()				{ return m_strVoidBAType; }
	int GetAuthServerNo()					{ return m_nAuthServerNo; }
	int GetVIPStatus()						{ return m_nVIPStatus; }
	int GetPromoExportType()				{ return m_nPromoExportType; }
	int GetReasonCode()						{ return m_nReasonCode; }
	const char* GetReasonText()				{ return m_strReasonText; }
	int GetWastageCode()					{ return m_nWastageCode; }
	const char* GetWastageText()			{ return m_strWastageText; }
	int GetTranSeqNo()						{ return m_nTranSeqNo; }
	const char* GetDescription()			{ return m_strDescription; }
	const char* GetAttributeText()			{ return m_strAttributeText; }

private:
	__int64 m_nPluNo;
	CString m_strPluNo;
	bool m_bVoid;
	bool m_bRefund;
	bool m_bWastageVoid;
	bool m_b15Digit;
	bool m_bRA;
	int m_nAccountType;
	__int64 m_nAccountID;
	int m_nPriceBand;
	int m_nDeptNo;
	int m_nGroupNo;
	CString m_strTaxBand;
	int m_nServerNo;
	double m_dQty;
	int m_nLineValueAsInt;
	double m_dLineValueAsDouble;
	int m_nACatNo;
	int m_nMixMatchNo;
	int m_nMixMatchType;
	CString m_strSaleDate;
	CString m_strSaleTime;
	CString m_strVoidBAType;
	int m_nAuthServerNo;
	int m_nVIPStatus;
	int m_nPromoExportType;
	int m_nReasonCode;
	CString m_strReasonText;
	int m_nWastageCode;
	CString m_strWastageText;
	int m_nTranSeqNo;
	CString m_strDescription;
	CString m_strAttributeText;
};

/**********************************************************************/
