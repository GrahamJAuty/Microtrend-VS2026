#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "LocationSelectorEntity.h"
#include "..\CommonEcrStock\PluFilterArray.h"
#include "..\CommonStockTray\StockCSVArray.h"
/**********************************************************************/
#define LABELDB_VERSION 2
/**********************************************************************/

class CLabelListCSVRecord : public CCSVRecord
{
public:
	CLabelListCSVRecord(); //fill with default values
	virtual void ConvertToCSV ( CCSV& csv ); 
	virtual void ConvertFromCSV ( CCSV& csv ); 
	
	__int64 GetBarcodeNoInt(){ return m_nBarcodeNo; }
	__int64 GetPluNoInt() { return m_nPluNo; }
	int GetModifier() { return m_nModifier; }
	const char* GetBarcodeNoString();
	const char* GetPluNoString();
	int GetQtyToPrint() { return m_nQtyToPrint; }
	const char* GetRepText() { return m_strRepText; }
	double GetFirstPrice() { return m_dFirstPrice; }
	double GetSecondPrice() { return m_dSecondPrice; }
	const char* GetStockCode() { return m_strStockCode; }
	const char* GetLabelText() { return m_strLabelText; }
	double GetUnitPriceFactor() { return m_dUnitPriceFactor; }
	bool GetDirectPluFlag(){ return m_bDirectPlu; }

	void SetBarcodeNo( __int64 nBarcodeNo );
	void SetPluNo ( __int64 nPluNo );
	void SetModifier( int nMod );
	void SetQtyToPrint( int nQty ) { m_nQtyToPrint = nQty; }
	void SetRepText( const char* szText ) { m_strRepText = szText; }
	void SetFirstPrice ( double dPrice ) { m_dFirstPrice = dPrice; }
	void SetSecondPrice ( double dPrice ) { m_dSecondPrice = dPrice; }
	void SetStockCode( const char* szCode ) { m_strStockCode = szCode; }
	void SetLabelText ( const char* szText ) { m_strLabelText = szText; }
	void SetUnitPriceFactor ( double dVal ) { m_dUnitPriceFactor = dVal; }
	void SetDirectPluFlag( bool bFlag ){ m_bDirectPlu = bFlag; }

private:
	void ClearRecord();
	void V1To2ConvertFromCSV ( CCSV& csv, int nVer );

private:
	//Record fields
	__int64 m_nBarcodeNo;
	__int64 m_nPluNo;
	int m_nModifier;
	int m_nQtyToPrint;
	CString m_strRepText;
	double m_dFirstPrice;
	double m_dSecondPrice;
	CString m_strStockCode;
	CString m_strLabelText;
	double m_dUnitPriceFactor;
	bool m_bOldRecord;
	bool m_bDirectPlu;

private:
	CString m_strBarcodeNo;
	CString m_strPluNo;
};

/**********************************************************************/

class CLabelListCSVArray : public CSharedCSVArray
{
public:
	CLabelListCSVArray( bool bProductLabels );
	void RemoveAll();
	
	bool SaveList();
	void RestoreSavedList();

	int GetTotalLabelCount();
	
	void CreateAdhocList( CPluFilterArray& FilterArray, __int64 nPluFrom, __int64 nPluNo, __int64 nBarcodeFrom, __int64 nBarcodeTo, int nDepartmentFilter, int nType, const char* szDate, int nItemLabels );
	void CreateApparentList( CPluFilterArray& FilterArray, __int64 nPluFrom, __int64 nPluNo, __int64 nBarcodeFrom, __int64 nBarcodeTo, int nDepartmentFilter, int nType, const char* szDate, CLocationSelectorEntity& LocSelEntity );
	void CreateSingleItemList( int nPluIdx );
	void CreateShelfChangesList();
	void UpdateShelfChangesList();

#ifdef STOCKMAN_SYSTEM
	void AutoBarcode( CStockCSVRecord& StockRecord, int nQty );
	bool AutoPluBarcode( CPluCSVRecord& PluRecord, int nQty );
	bool AutoExternalBarcode( CPluCSVRecord& PluRecord, int nQty );
#endif

	bool FindBarcodeByNumber ( __int64 nBarcodeNo, int& nIndex );
	bool InsertRecord ( CLabelListCSVRecord& NewRecord, int& nIndex );
	
	__int64 GetBarcodeNoInt ( int nIndex );
	const char* GetBarcodeNoString ( int nIndex );
	
	bool IsProductLabelList() { return m_bProductLabels; }

	double CalculateLabelPrice( double dRetailPrice, double dTaxRate, int nTaxType );

public:
	void MergePendingFiles();
	void DeletePendingFiles();
	void ClearPendingFileList();
	void MergeLabels( CLabelListCSVArray& arraySource );

private:
	void CreateAdhocListInternal( CPluFilterArray& FilterArray, __int64 nPluFrom, __int64 nPluNo, __int64 nBarcodeFrom, __int64 nBarcodeTo, int nDepartmentFilter, int nType, const char* szDate, int nItemLabels );
	void CreateApparentListInternal( CPluFilterArray& FilterArray, __int64 nPluFrom, __int64 nPluNo, __int64 nBarcodeFrom, __int64 nBarcodeTo, int nDepartmentFilter, int nType, const char* szDate, CLocationSelectorEntity& LocSelEntity );
	
	void RestoreSavedListInternal();
	void GetPriceInfo( CPluCSVRecord& PluRecord, int nMod, double& dFirstPrice, double& dSecondPrice );

private:
	void CreateShelfChangesListInternal();
	void UpdateShelfChangesListInternal();

private:
	bool m_bProductLabels;
	CString m_strBarcodeNo;

private:
	CStringArray m_arrayPendingFiles;
};

/**********************************************************************/
#endif
/**********************************************************************/

