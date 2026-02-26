/**********************************************************************/
#pragma once
/**********************************************************************/
#if defined(STOCKMAN_SYSTEM) || defined(POSTRAY_UTILITY)
/**********************************************************************/
#define STOCK_PRICES 10
/**********************************************************************/
#define STOCKTAKE_TYPE_PERIOD 0
#define STOCKTAKE_TYPE_YTD 1
#define STOCKTAKE_TYPE_ADJUST 2
/**********************************************************************/

class CStockLevelsCSVRecord : public CCSVRecord
{
public:
	CStockLevelsCSVRecord();
	virtual ~CStockLevelsCSVRecord();
	void Reset();

	virtual void ConvertToCSV ( CCSV& csv ); 
	virtual void ConvertFromCSV ( CCSV& csv ); 
	
	CStockLevelsCSVRecord& operator= ( const CStockLevelsCSVRecord& source );
	CStockLevelsCSVRecord& operator+ ( const CStockLevelsCSVRecord& source );

	const char* GetStockCode() const { return m_strStockCode; }
	int GetReferenceNo() const { return m_nReferenceNo; }
	double GetOpeningQty() const { return m_dOpeningQty; }
	double GetDeliveryQty() const { return m_dDeliveryQty; }
	double GetDeliveryValue() const { return m_dDeliveryValue; }
	double GetFreeQty() const { return m_dFreeQty; }
	double GetTransferInQty() const { return m_dTransferInQty; }
	double GetEposSalesQty();
	double GetEposSalesQty ( int nLevel );
	double GetEposSalesValue();
	double GetEposSalesValue ( int nLevel );
	double GetRecipeSalesQty() const { return m_dRecipeSalesQty; }
	double GetRecipeSalesValue() const { return m_dRecipeSalesValue; }
	double GetManualSalesQty() const { return m_dManualSalesQty; }
	double GetManualSalesValue() const { return m_dManualSalesValue; }
	double GetImportSalesQty() const { return m_dImportSalesQty; }
	double GetImportSalesValue() const { return m_dImportSalesValue; }
	double GetAdjustmentQty() const { return m_dAdjustmentQty; }
	double GetAdjustmentValue() const { return m_dAdjustmentValue; }
	double GetReturnsQty() const { return m_dReturnsQty; }
	double GetReturnsValue() const { return m_dReturnsValue; }
	double GetEcrWastageQty() const { return m_dEcrWastageQty; }
	double GetEcrWastageValue() const { return m_dEcrWastageValue; }
	double GetTransferOutQty() const { return m_dTransferOutQty; }
	double GetYTDSalesQty() const { return m_dYTDSalesQty; }
	double GetYTDSalesValue() const { return m_dYTDSalesValue; }
	double GetYTDAdjustmentQty() const { return m_dYTDAdjustmentQty; }
	double GetYTDAdjustmentValue() const { return m_dYTDAdjustmentValue; }
	double GetYTDEcrWastageQty() const { return m_dYTDEcrWastageQty; }
	double GetYTDEcrWastageValue() const { return m_dYTDEcrWastageValue; }
	double GetQtyOnOrder() const { return m_dQtyOnOrder; }
	double GetMinimumLevelQty() const { return m_dMinimumLevelQty; }
	double GetMaximumLevelQty() const { return m_dMaximumLevelQty; }
	double GetReorderLevelQty() const { return m_dReorderLevelQty; }
	double GetAutoWastagePercent() const { return m_dAutoWastagePercent; }
	double GetAutoWastageQty();
	double GetYTDAutoWastageQty();

	bool GetActiveFlag() const { return m_bActive; }
	int GetStocktakeNo() const { return m_nStocktakeNo; }
	double GetStocktakeDeliveryQty() const { return m_dStocktakeDeliveryQty; }
	double GetStocktakeDeliveryValue() const { return m_dStocktakeDeliveryValue; }
	double GetStocktakeFreeQty() const { return m_dStocktakeFreeQty; }
	double GetStocktakeTransferInQty() const { return m_dStocktakeTransferInQty; }
	double GetStocktakeEposSalesQty();
	double GetStocktakeEposSalesQty ( int nLevel );
	double GetStocktakeEposSalesValue();
	double GetStocktakeEposSalesValue ( int nLevel );
	double GetStocktakeRecipeSalesQty() const { return m_dStocktakeRecipeSalesQty; }
	double GetStocktakeRecipeSalesValue() const { return m_dStocktakeRecipeSalesValue; }
	double GetStocktakeManualSalesQty() const { return m_dStocktakeManualSalesQty; }
	double GetStocktakeManualSalesValue() const { return m_dStocktakeManualSalesValue; }
	double GetStocktakeImportSalesQty() const { return m_dStocktakeImportSalesQty; }
	double GetStocktakeImportSalesValue() const { return m_dStocktakeImportSalesValue; }
	double GetStocktakeAdjustmentQty() const { return m_dStocktakeAdjustmentQty; }
	double GetStocktakeAdjustmentValue() const { return m_dStocktakeAdjustmentValue; }
	double GetStocktakeReturnsQty() const { return m_dStocktakeReturnsQty; }
	double GetStocktakeReturnsValue() const { return m_dStocktakeReturnsValue; }
	double GetStocktakeEcrWastageQty() const { return m_dStocktakeEcrWastageQty; }
	double GetStocktakeEcrWastageValue() const { return m_dStocktakeEcrWastageValue; }
	double GetStocktakeTransferOutQty() const { return m_dStocktakeTransferOutQty; }
	double GetStocktakeApparentQty() const { return m_dStocktakeApparentQty; }
	double GetStocktakeClosingQty() const { return m_dStocktakeClosingQty; }

	double GetPendingStocktakeQty();

	double GetStocktakeAutoWastageQty();
	double GetTotalGoodsInQty();
	double GetTotalSalesQty();
	double GetTotalSalesValue();
	double GetApparentQty();
	
	void BeginStocktake( int nStocktakeNo, bool bClosingStockZero );
	void AbandonStocktake( int nStocktakeNo );
	void FinaliseStocktake( int nStocktakeNo, int nType, double dSURetail );

	double GetStocktakeTotalGoodsInQty();
	double GetStocktakeTotalSalesQty();
	double GetStocktakeTotalSalesValue();
	double EstimateStocktakeSalesQty();
	
	bool HasStockMovements();

	void SetStockCode( const char* szStockCode ) { m_strStockCode = szStockCode; }
	void SetReferenceNo( int nReferenceNo ) { m_nReferenceNo = nReferenceNo; }
	void SetOpeningQty( double dQty ) { m_dOpeningQty = dQty; }
	void SetDeliveryQty( double dQty ) { m_dDeliveryQty = dQty; }
	void SetDeliveryValue( double dValue ) { m_dDeliveryValue = dValue; }
	void SetFreeQty( double dQty ) { m_dFreeQty = dQty; }
	void SetTransferInQty( double dQty ) { m_dTransferInQty = dQty; }
	void SetEposSalesQty ( int nLevel, double dQty );
	void SetEposSalesValue ( int nLevel, double dValue );
	void SetRecipeSalesQty( double dValue ) { m_dRecipeSalesQty = dValue; }
	void SetRecipeSalesValue( double dValue ) { m_dRecipeSalesValue = dValue; }
	void SetManualSalesQty( double dQty ) { m_dManualSalesQty = dQty; }
	void SetManualSalesValue( double dValue ) { m_dManualSalesValue = dValue; }
	void SetImportSalesQty( double dQty ) { m_dImportSalesQty = dQty; }
	void SetImportSalesValue( double dValue ) { m_dImportSalesValue = dValue; }
	void SetAdjustmentQty( double dQty ) { m_dAdjustmentQty = dQty; } 
	void SetAdjustmentValue( double dValue ) { m_dAdjustmentValue = dValue; }
	void SetReturnsQty( double dQty ) { m_dReturnsQty = dQty; }
	void SetReturnsValue( double dValue ) { m_dReturnsValue = dValue; }
	void SetEcrWastageQty( double dQty ) { m_dEcrWastageQty = dQty; }
	void SetEcrWastageValue( double dValue ) { m_dEcrWastageValue = dValue; }
	void SetTransferOutQty( double dQty ) { m_dTransferOutQty = dQty; }
	void SetYTDSalesQty( double dQty ) { m_dYTDSalesQty = dQty; }
	void SetYTDSalesValue( double dValue ) { m_dYTDSalesValue = dValue; }
	void SetYTDAdjustmentQty( double dQty ) { m_dYTDAdjustmentQty = dQty; }
	void SetYTDAdjustmentValue( double dValue ) { m_dYTDAdjustmentValue = dValue; }
	void SetYTDEcrWastageQty( double dQty ) { m_dYTDEcrWastageQty = dQty; }
	void SetYTDEcrWastageValue( double dValue ) { m_dYTDEcrWastageValue = dValue; }
	void SetQtyOnOrder( double dQty ) { m_dQtyOnOrder = ( dQty > 0.0 ) ? dQty : 0; }
	void SetReorderLevelQty ( double dQty ) { m_dReorderLevelQty = dQty; }
	void SetMinimumLevelQty( double dQty ) { m_dMinimumLevelQty = dQty; }
	void SetMaximumLevelQty( double dQty ) { m_dMaximumLevelQty = dQty; }
	void SetAutoWastagePercent( double dPercent ) { m_dAutoWastagePercent = dPercent; }
	
	void SetActiveFlag( bool b ) { m_bActive = b; }
	void SetStocktakeNo( int nStocktakeNo ) { m_nStocktakeNo = nStocktakeNo; }
	void SetStocktakeDeliveryQty( double dQty ) { m_dStocktakeDeliveryQty = dQty; }
	void SetStocktakeDeliveryValue( double dValue ) { m_dStocktakeDeliveryValue = dValue; }
	void SetStocktakeFreeQty( double dQty ) { m_dStocktakeFreeQty = dQty; }
	void SetStocktakeTransferInQty( double dQty ) { m_dStocktakeTransferInQty = dQty; }
	void SetStocktakeEposSalesQty ( int nLevel, double dQty );
	void SetStocktakeEposSalesValue ( int nLevel, double dValue );
	void SetStocktakeRecipeSalesQty( double dValue ) { m_dStocktakeRecipeSalesQty = dValue; }
	void SetStocktakeRecipeSalesValue( double dValue ) { m_dStocktakeRecipeSalesValue = dValue; }
	void SetStocktakeManualSalesQty( double dQty ) { m_dStocktakeManualSalesQty = dQty; }
	void SetStocktakeManualSalesValue( double dValue ) { m_dStocktakeManualSalesValue = dValue; }
	void SetStocktakeImportSalesQty( double dQty ) { m_dStocktakeImportSalesQty = dQty; }
	void SetStocktakeImportSalesValue( double dValue ) { m_dStocktakeImportSalesValue = dValue; }
	void SetStocktakeAdjustmentQty( double dQty ) { m_dStocktakeAdjustmentQty = dQty; } 
	void SetStocktakeAdjustmentValue( double dValue ) { m_dStocktakeAdjustmentValue = dValue; }
	void SetStocktakeReturnsQty( double dQty ) { m_dStocktakeReturnsQty = dQty; }
	void SetStocktakeReturnsValue( double dValue ) { m_dStocktakeReturnsValue = dValue; }
	void SetStocktakeEcrWastageQty( double dQty ) { m_dStocktakeEcrWastageQty = dQty; }
	void SetStocktakeEcrWastageValue( double dValue ) { m_dStocktakeEcrWastageValue = dValue; }
	void SetStocktakeTransferOutQty( double dQty ) { m_dStocktakeTransferOutQty = dQty; }	
	void SetStocktakeApparentQty( double dQty ) { m_dStocktakeApparentQty = dQty; }
	void SetStocktakeClosingQty( double dQty ) { m_dStocktakeClosingQty = dQty; }
	
	void AddEposSales ( int nLevel, double dQty, double dValue );
	void AddManualSales ( double dQty, double dValue, bool bStocktake );
	void AddImportSales ( double dQty, double dValue, bool bStocktake );
	void AddRecipeSales ( double dQty, double dValue, bool bStocktake );
	void AddDelivery ( double dQty, double dValue, bool bStocktake );
	void AddFreeStock ( double dQty, bool bStocktake );
	void AddReturns ( double dQty, double dValue, bool bStocktake );
	void AddEcrWastage ( double dQty, double dValue );
	void AddAdjustment ( double dQty, double dValue, bool bStocktake );
	void AddOrder ( double dQty );

private:
	void ClearRecord();
	void V1ConvertFromCSV ( CCSV& csv );
	void V2ConvertFromCSV ( CCSV& csv );
	void V3ConvertFromCSV ( CCSV& csv );
	void V4ConvertFromCSV ( CCSV& csv );
	void V5to6ConvertFromCSV ( CCSV& csv );
	void V7to14ConvertFromCSV( CCSV& csv, int nVer );
	
private:
	CString m_strStockCode;
	int m_nReferenceNo;
	double m_dOpeningQty;
	double m_dDeliveryQty;
	double m_dDeliveryValue;
	double m_dFreeQty;
	double m_dTransferInQty;
	double m_dEposSalesValue [ STOCK_PRICES ];
	double m_dEposSalesQty [ STOCK_PRICES ];
	double m_dRecipeSalesQty;
	double m_dRecipeSalesValue;
	double m_dManualSalesQty;
	double m_dManualSalesValue;
	double m_dImportSalesQty;
	double m_dImportSalesValue;
	double m_dAdjustmentQty;
	double m_dAdjustmentValue;
	double m_dReturnsQty;
	double m_dReturnsValue;
	double m_dEcrWastageQty;
	double m_dEcrWastageValue;
	double m_dTransferOutQty;
	double m_dYTDSalesQty;
	double m_dYTDSalesValue;
	double m_dYTDAdjustmentQty;
	double m_dYTDAdjustmentValue;
	double m_dYTDEcrWastageQty;
	double m_dYTDEcrWastageValue;
	double m_dQtyOnOrder;
	double m_dReorderLevelQty;
	double m_dMinimumLevelQty;
	double m_dMaximumLevelQty;
	double m_dAutoWastagePercent;
	
	bool m_bActive;
	int m_nStocktakeNo;
	double m_dStocktakeDeliveryQty;
	double m_dStocktakeDeliveryValue;
	double m_dStocktakeFreeQty;
	double m_dStocktakeTransferInQty;
	double m_dStocktakeEposSalesValue [ STOCK_PRICES ];
	double m_dStocktakeEposSalesQty [ STOCK_PRICES ];
	double m_dStocktakeRecipeSalesQty;
	double m_dStocktakeRecipeSalesValue;
	double m_dStocktakeManualSalesQty;
	double m_dStocktakeManualSalesValue;
	double m_dStocktakeImportSalesQty;
	double m_dStocktakeImportSalesValue;
	double m_dStocktakeAdjustmentQty;
	double m_dStocktakeAdjustmentValue;
	double m_dStocktakeReturnsQty;
	double m_dStocktakeReturnsValue;
	double m_dStocktakeEcrWastageQty;
	double m_dStocktakeEcrWastageValue;
	double m_dStocktakeTransferOutQty;
	double m_dStocktakeApparentQty;
	double m_dStocktakeClosingQty;
};

/**********************************************************************/

class CStockLevelsCSVArray : public CSharedCSVArray
{
public:
	CStockLevelsCSVArray( int nSpNo );
	~CStockLevelsCSVArray();
	void RemoveAll();
	
	int GetSpNo() { return m_nSpNo; }
	
	const char* GetStockCode( int nIndex );
	bool FindStockItemByCode ( int nMasterIdx, const char* szCode, int& nIndex );
	
	bool IsInSync() { return m_bInSync; }
	void SetSyncFlag( bool bFlag ) { m_bInSync = bFlag; }

private:
	bool m_bInSync;
	int m_nSpNo;

private:
	CString m_strStockCode;	
};

/**********************************************************************/
#endif
/**********************************************************************/
