#pragma once
/**********************************************************************/

class CEposReportCustomSettingsHourly
{
public:
	CEposReportCustomSettingsHourly();

public:
	void GetSettingsCSVLine( CString& strLine );
	void SetSettingsCSVLine( CString& strLine );

private:
	void Reset();

public:
	int GetTimeSliceType()				{ return m_nTimeSliceType; }
	const char* GetTimeSliceEntityID()	{ return m_strTimeSliceEntityID; }
	bool GetAllowAdhocFlag()			{ return m_bAllowAdhoc; }

public:
	void SetTimeSliceType( int n );
	void SetTimeSliceEntityID( const char* sz );
	void SetAllowAdhocFlag( bool b )	{ m_bAllowAdhoc = b; }

private:
	int m_nTimeSliceType;
	CString m_strTimeSliceEntityID;
	bool m_bAllowAdhoc;
};

/**********************************************************************/

class CEposReportCustomSettingsGeneral
{
public:
	CEposReportCustomSettingsGeneral();

public:
	void GetSettingsCSVLine( CString& strLine );
	void SetSettingsCSVLine( CString& strLine );

private:
	void Reset();

public:
	bool GetWeekdayFlag()			{ return m_bWeekday; }
	bool GetQuantityFlag()			{ return m_bQuantity; }
	bool GetTaxBandFlag()			{ return m_bTaxBand; }
	bool GetTaxAmountFlag()			{ return m_bTaxAmount; }
	bool GetPromoDetailFlag()		{ return m_bPromoDetail; }
	bool GetPromoSummaryFlag()		{ return m_bPromoSummary; }
	bool GetPromoPositiveFlag()		{ return m_bPromoPositive; }
	bool GetReconciliationFlag()	{ return m_bReconciliation; }
	bool GetVoidFlag()				{ return m_bVoid; }
	int GetVoidQtyType()			{ return m_nVoidQtyType; }
	bool GetNoSaleFlag()			{ return m_bNoSale; }
	bool GetConsolDeptFlag()		{ return m_bConsolDept; }
	bool GetConsolGroupReportFlag()	{ return m_bConsolGroupReport; }
	bool GetConsolGroupConsolFlag()	{ return m_bConsolGroupConsol; }
	bool GetConsolGroupEposFlag()	{ return m_bConsolGroupEpos; }
	bool GetSalesPromoFlag()		{ return m_bSalesPromo; }
	bool GetSeparateChangeFlag()	{ return m_bSeparateChange; }
	bool GetCCDVarianceFlag()		{ return m_bCCDVariance; }
	bool GetAllowAdhocFlag()		{ return m_bAllowAdhoc; }

public:
	void SetWeekdayFlag( bool b )			{ m_bWeekday = b; }
	void SetQuantityFlag( bool b )			{ m_bQuantity = b; }
	void SetTaxBandFlag( bool b )			{ m_bTaxBand = b; }
	void SetTaxAmountFlag( bool b )			{ m_bTaxAmount = b; }
	void SetPromoDetailFlag( bool b )		{ m_bPromoDetail = b; }
	void SetPromoSummaryFlag( bool b )		{ m_bPromoSummary = b; }
	void SetPromoPositiveFlag( bool b )		{ m_bPromoPositive = b; }
	void SetReconciliationFlag( bool b )	{ m_bReconciliation = b; }
	void SetVoidFlag( bool b )				{ m_bVoid = b; }
	void SetVoidQtyType( int n );			
	void SetNoSaleFlag( bool b )			{ m_bNoSale = b; }
	void SetConsolDeptFlag( bool b )		{ m_bConsolDept = b; }
	void SetConsolGroupReportFlag( bool b )	{ m_bConsolGroupReport = b; }
	void SetConsolGroupConsolFlag( bool b )	{ m_bConsolGroupConsol = b; }
	void SetConsolGroupEposFlag( bool b )	{ m_bConsolGroupEpos = b; }
	void SetSalesPromoFlag( bool b )		{ m_bSalesPromo = b; }
	void SetSeparateChangeFlag( bool b )	{ m_bSeparateChange = b; }
	void SetCCDVarianceFlag( bool b )		{ m_bCCDVariance = b; }
	void SetAllowAdhocFlag( bool b )		{ m_bAllowAdhoc = b; }

public:
	void ValidateConsolSettings();
	
private:
	bool m_bWeekday;
	bool m_bQuantity;
	bool m_bTaxBand;
	bool m_bTaxAmount;
	bool m_bPromoDetail;
	bool m_bPromoSummary;
	bool m_bPromoPositive;
	bool m_bReconciliation;
	bool m_bSeparateChange;
	bool m_bCCDVariance;
	bool m_bVoid;
	int m_nVoidQtyType;
	bool m_bNoSale;
	bool m_bConsolDept;
	bool m_bConsolGroupReport;
	bool m_bConsolGroupConsol;
	bool m_bConsolGroupEpos;
	bool m_bSalesPromo;
	bool m_bSeperateChange;
	bool m_bAllowAdhoc;
};

/**********************************************************************/

class CEposReportCustomSettingsTransaction
{
public:
	CEposReportCustomSettingsTransaction();

public:
	void GetSettingsCSVLine( CString& strLine );
	void SetSettingsCSVLine( CString& strLine );

private:
	void Reset();
	void FixRange( int& nStart, int& nEnd );

public:
	bool GetDiscrepancyFlag()			{ return m_bDiscrepancy; }
	bool GetCashRSPVersionFlag()		{ return m_bCashRSPVersion; }
	int GetSortType()					{ return m_nSortType; }
	int GetTimeType()					{ return m_nTimeType; }
	bool GetItemSaleDateFlag()			{ return m_bItemSaleDate; }
	const char* GetFilterText()			{ return m_strFilterText; }
	int GetFilterTextType()				{ return m_nFilterTextType; }
	int GetSeqNoFrom()					{ return m_nSeqNoFrom; }
	int GetSeqNoTo()					{ return m_nSeqNoTo; }
	int GetTranServerFrom()				{ return m_nTranServerFrom; }
	int GetTranServerTo()				{ return m_nTranServerTo; }
	int GetItemServerFrom()				{ return m_nItemServerFrom; }
	int GetItemServerTo()				{ return m_nItemServerTo; }
	bool GetShowItemServerFlag()		{ return m_bShowItemServer; }
	bool GetModifierTextFlag()			{ return m_bModifierText; }
	bool GetAllowAdhocFlag()			{ return m_bAllowAdhoc; }

public:
	void SetDiscrepancyFlag( bool b )	{ m_bDiscrepancy = b; }
	void SetCashRSPVersionFlag( bool b ){ m_bCashRSPVersion = b; }
	void SetSortType( int n );
	void SetTimeType( int n );
	void SetItemSaleDateFlag( bool b ){ m_bItemSaleDate = b; }
	void SetTranServerFrom( int n );
	void SetTranServerTo( int n );
	void SetItemServerFrom( int n );
	void SetItemServerTo( int n );
	void SetShowItemServerFlag( bool b ){ m_bShowItemServer = b; }
	void SetModifierTextFlag( bool b ){ m_bModifierText = b; }
	void SetSeqNoFrom( int n );
	void SetSeqNoTo( int n );
	void SetFilterText( const char* sz ){ m_strFilterText = sz; }
	void SetFilterTextType( int n );
	void SetAllowAdhocFlag( bool b )	{ m_bAllowAdhoc = b; }

private:
	bool m_bDiscrepancy;
	bool m_bCashRSPVersion;
	int m_nSortType;
	int m_nTimeType;
	bool m_bItemSaleDate;
	int m_nSeqNoFrom;
	int m_nSeqNoTo;
	int m_nTranServerFrom;
	int m_nTranServerTo;
	int m_nItemServerFrom;
	int m_nItemServerTo;
	bool m_bShowItemServer;
	bool m_bModifierText;
	CString m_strFilterText;
	int m_nFilterTextType;
	bool m_bAllowAdhoc;
};

/**********************************************************************/

class CEposReportCustomSettingsDeposit
{
public:
	CEposReportCustomSettingsDeposit();

public:
	void GetSettingsCSVLine( CString& strLine );
	void SetSettingsCSVLine( CString& strLine );

private:
	void Reset();

public:
	int GetReportType()					{ return m_nReportType; }
	bool GetShowRefundFlag()			{ return m_bShowRefund; }
	bool GetShowRedeemFlag()			{ return m_bShowRedeem; }
	bool GetShowPaymentFlag()			{ return m_bShowPayment; }
	bool GetEndDateCutOffFlag()			{ return m_bEndDateCutOff; }
	bool GetAllowAdhocFlag()			{ return m_bAllowAdhoc; }

public:
	void SetReportType( int n );
	void SetShowRefundFlag( bool b )	{ m_bShowRefund = b; }
	void SetShowRedeemFlag( bool b )	{ m_bShowRedeem = b; }
	void SetShowPaymentFlag( bool b )	{ m_bShowPayment = b; }
	void SetEndDateCutOffFlag( bool b )	{ m_bEndDateCutOff = b; }
	void SetAllowAdhocFlag( bool b )	{ m_bAllowAdhoc = b; }

private:
	int m_nReportType;
	bool m_bShowRefund;
	bool m_bShowRedeem;
	bool m_bShowPayment;
	bool m_bEndDateCutOff;
	bool m_bAllowAdhoc;
};

/**********************************************************************/

class CEposReportCustomSettingsMixMatch
{
public:
	CEposReportCustomSettingsMixMatch();

public:
	void GetSettingsCSVLine( CString& strLine );
	void SetSettingsCSVLine( CString& strLine );

private:
	void Reset();

public:
	int GetReportType()					{ return m_nReportType; }
	int GetOfferFilter()				{ return m_nOfferFilter; }
	bool GetCoversFlag()				{ return m_bCovers; }
	bool GetAllowAdhocFlag()			{ return m_bAllowAdhoc; }

public:
	void SetReportType( int n );
	void SetOfferFilter( int n );
	void SetAllowAdhocFlag( bool b )	{ m_bAllowAdhoc = b; }
	void SetCoversFlag( bool b )		{ m_bCovers = b; }

private:
	int m_nReportType;
	int m_nOfferFilter;
	bool m_bAllowAdhoc;
	bool m_bCovers;
};

/**********************************************************************/

class CEposReportCustomSettingsDiscount
{
public:
	CEposReportCustomSettingsDiscount();

public:
	void GetSettingsCSVLine( CString& strLine );
	void SetSettingsCSVLine( CString& strLine );

private:
	void Reset();

public:
	int GetOfferFilter()				{ return m_nOfferFilter; }
	bool GetByServerFlag()				{ return m_bByServer; }
	bool GetSubtotalItemsFlag()			{ return m_bSubtotalItems; }
	bool GetAllowAdhocFlag()			{ return m_bAllowAdhoc; }

public:
	void SetOfferFilter( int n );
	void SetByServerFlag( bool b )		{ m_bByServer = b; }
	void SetSubtotalItemsFlag( bool b )	{ m_bSubtotalItems = b; }
	void SetAllowAdhocFlag( bool b )	{ m_bAllowAdhoc = b; }

private:
	int m_nOfferFilter;
	bool m_bByServer;
	bool m_bSubtotalItems;
	bool m_bAllowAdhoc;
};

/**********************************************************************/

class CEposReportCustomSettingsLoyaltyRecon
{
public:
	CEposReportCustomSettingsLoyaltyRecon();

public:
	void GetSettingsCSVLine(CString& strLine);
	void SetSettingsCSVLine(CString& strLine);

private:
	void Reset();

public:
	const char* GetLoyaltyRequestPath() { return m_strLoyaltyRequestPath; }
	bool GetAllowAdhocFlag() { return m_bAllowAdhoc; }
	bool GetUnreconOnlyFlag() { return m_bUnreconOnly; }

public:
	void SetLoyaltyRequestPath(const char* sz);
	void SetAllowAdhocFlag(bool b) { m_bAllowAdhoc = b; }
	void SetUnreconOnlyFlag(bool b) { m_bUnreconOnly = b;  }

private:
	CString m_strLoyaltyRequestPath;
	bool m_bAllowAdhoc;
	bool m_bUnreconOnly;
};

/**********************************************************************/

class CEposReportCustomSettingsPaymentDetail
{
public:
	CEposReportCustomSettingsPaymentDetail();

public:
	void GetSettingsCSVLine(CString& strLine);
	void SetSettingsCSVLine(CString& strLine);

private:
	void Reset();

public:
	bool GetShowTableNoFlag() { return m_bShowTableNo; }
	bool GetShowTableNameFlag() { return m_bShowTableName; }
	bool GetShowSEQNoFlag() { return m_bShowSEQNo; }
	bool GetShowServerFlag() { return m_bShowServer; }
	bool GetAllowAdhocFlag() { return m_bAllowAdhoc; }
	bool GetTableFilterFlag() { return m_bTableFilter; }
	int GetTableNoFrom() { return m_nTableNoFrom; }
	int GetTableNoTo() { return m_nTableNoTo; }
	bool GetPayNumFilterFlag() { return m_bPayNumFilter; }
	int GetPaymentTypeFilter() { return m_nPaymentTypeFilter; }
	CString GetPayNumFilter() { return m_strPayNumFilter; }

public:
	void SetShowTableNoFlag(bool b) { m_bShowTableNo = b; }
	void SetShowTableNameFlag(bool b) { m_bShowTableName = b; }
	void SetTableFilterFlag(bool b) { m_bTableFilter = b; }
	void SetTableNoFrom(int n);
	void SetTableNoTo(int n);
	void SetShowSEQNoFlag(bool b) { m_bShowSEQNo = b; }
	void SetShowServerFlag(bool b) { m_bShowServer = b; }
	void SetPayNumFilterFlag(bool b) { m_bPayNumFilter = b; }
	void SetPayNumFilter(CString str);
	void SetPaymentTypeFilter(int n);
	void SetAllowAdhocFlag(bool b) { m_bAllowAdhoc = b; }

public:
	bool IsPaymentTypeEnabled(int nType);
	
private:
	bool m_bShowTableNo = FALSE;
	bool m_bShowTableName = FALSE;
	bool m_bTableFilter = FALSE;
	int m_nTableNoFrom = 0;
	int m_nTableNoTo = 999;
	bool m_bShowSEQNo = FALSE;
	bool m_bShowServer = FALSE;
	bool m_bPayNumFilter = FALSE;
	CString m_strPayNumFilter = "";
	int m_nPaymentTypeFilter = 0;
	bool m_bAllowAdhoc = FALSE;
};

/**********************************************************************/
#define EPOSPRICEBANDREPORT_PLUSALES 0
#define EPOSPRICEBANDREPORT_NOZEROSKIP 1
#define EPOSPRICEBANDREPORT_VIP 2
#define EPOSPRICEBANDREPORT_WASTE 3
/**********************************************************************/

class CEposReportCustomSettingsPluPriceBand
{
public:
	CEposReportCustomSettingsPluPriceBand();

public:
	void GetSettingsCSVLine(CString& strLine);
	void SetSettingsCSVLine(CString& strLine);

private:
	void Reset();

public:
	bool GetAllowAdhocFlag() { return m_bAllowAdhoc; }
	int GetPriceBandFlags() { return m_nPriceBandFlags; }
	bool GetColumnNamesFlag() { return m_bColumnNames; }
	bool GetSortByServerFlag() { return m_bSortByServer; }
	int GetReportSubType() { return m_nReportSubType; }

	bool IsPriceBandEnabled(int nBand)
	{
		if ((nBand >= 0) && (nBand <= 9))
		{
			return (m_nPriceBandFlags & (1 << nBand)) != 0;
		}
		else
		{
			return FALSE;
		}
	}

public:
	void SetAllowAdhocFlag(bool b) { m_bAllowAdhoc = b; }
	void SetColumnNamesFlag(bool b) { m_bColumnNames = b; }
	void SetSortByServerFlag(bool b) { m_bSortByServer = b; }

	void SetPriceBandFlags(int n) 
	{ 
		if ((n >= 0) && (n <= 1023))
		{
			m_nPriceBandFlags = n;
		}
	}

	void SetPriceBandEnabled(int nBand, bool bEnable)
	{
		if ((nBand >= 0) && (nBand <= 9))
		{
			if (bEnable)
			{
				m_nPriceBandFlags |= (1 << nBand);
			}
			else
			{
				m_nPriceBandFlags &= ~(1 << nBand);
			}
		}
	}

	void SetReportSubType(int n)
	{
		switch (n)
		{
		case EPOSPRICEBANDREPORT_PLUSALES:
		case EPOSPRICEBANDREPORT_NOZEROSKIP:
		case EPOSPRICEBANDREPORT_VIP:
		case EPOSPRICEBANDREPORT_WASTE:
			m_nReportSubType = n;
			break;
		}
	}

private:
	bool m_bAllowAdhoc;
	int m_nPriceBandFlags;
	bool m_bColumnNames;
	bool m_bSortByServer;
	int m_nReportSubType;
};

/**********************************************************************/