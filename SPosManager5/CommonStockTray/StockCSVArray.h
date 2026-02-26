/**********************************************************************/
#pragma once
/**********************************************************************/
#if defined(STOCKMAN_SYSTEM) || defined(POSTRAY_UTILITY)
/**********************************************************************/
#include "ImportDefines.h"
#include "SupplierCSVArray.h"
/**********************************************************************/
/*	STOCK DATABASE												*/
/**********************************************************************/
#define STOCKDB_VERSION 14
/**********************************************************************/
#define MAX_ITEM_SUPPLIERS 5
#define MAX_ITEM_CUSTOM_FIELDS 5
/**********************************************************************/
#define SUPPLIER_PURGE_NOCHANGE 0
#define SUPPLIER_PURGE_CHANGESTOCK 1
#define SUPPLIER_PURGE_CHANGEPLU 2
/**********************************************************************/
#define WEIGHTTYPE_GALLON 0
#define WEIGHTTYPE_PINT 1
#define WEIGHTTYPE_FLOUNCE 2
#define WEIGHTTYPE_LITRE 3
#define WEIGHTTYPE_CLITRE 4
#define WEIGHTTYPE_MLITRE 5
#define WEIGHTTYPE_KILO 6
#define WEIGHTTYPE_GRAM 7

#define QTYWGHT_GALLON_GALLON 0
#define QTYWGHT_GALLON_PINT 1
#define QTYWGHT_GALLON_FLOUNCE 2
#define QTYWGHT_GALLON_LITRE 3
#define QTYWGHT_GALLON_CLITRE 4
#define QTYWGHT_GALLON_MLITRE 5

#define QTYWGHT_PINT_PINT 101
#define QTYWGHT_PINT_FLOUNCE 102
#define QTYWGHT_PINT_LITRE 103
#define QTYWGHT_PINT_CLITRE 104
#define QTYWGHT_PINT_MLITRE 105

#define QTYWGHT_FLOUNCE_FLOUNCE 202

#define QTYWGHT_LITRE_PINT 301
#define QTYWGHT_LITRE_FLOUNCE 302
#define QTYWGHT_LITRE_LITRE 303
#define QTYWGHT_LITRE_CLITRE 304
#define QTYWGHT_LITRE_MLITRE 305

#define QTYWGHT_CLITRE_CLITRE 404
#define QTYWGHT_CLITRE_MLITRE 405

#define QTYWGHT_MLITRE_MLITRE 505

#define QTYWGHT_KILO_KILO 606
#define QTYWGHT_KILO_GRAM 607

#define QTYWGHT_GRAM_GRAM 707
/**********************************************************************/

class CStockItemSupplier
{
public:
	CStockItemSupplier();
	void Clear();
	
public:
	int m_nSuppNo;
	CString m_strCase;
	int m_nDUItems;
	double m_dDUItemSize;
	double m_dCost;
	CString m_strSuppRef;
	CString m_strWrapper;
	double m_dRetail;
};

/**********************************************************************/

namespace Stock
{
const recString	StockCode		= { "StockCode", 1, 16 };
const recString	ImportSheetCode	= { "ImportSheetCode", 0, 10 };
const recString	Description		= { "Description", 0, 100 };
const recInt	Category		= { "Category", 1, 999 };
const recInt	CustomField		= { "CustomField", 0, 999999 };
const recString	StockUnit		= { "StockUnit", 0, 25 };
const recString	StockmateText	= { "StockmateText", 0, 8 };
const recInt	SUYield			= { "SUYield", 1, 9999 };
const recString	SubUnit			= { "SubUnit", 0, 25 };
const recInt	SUWeightType	= { "SUWeightType", 0, 9999 };
const recDouble	SUWeight		= { "SUWeight", 0.0, 10000.0 };
const recDouble	WastagePercent	= { "WastagePercent", 0, 100.0 };
const recInt	DisableOrder	= { "DisableOrder", 0, 1 };
const recInt	DisableRange	= { "DisableRange", 0, 1 };
const recInt	LocalSupplier	= { "LocalSupplier", 0, 1 };
const recBool	AutoLabel		= { "AutoLabel" };
const recInt	SupplierNo		= { "SupplierNo", 1, 999 };
const recString	DeliveryUnit	= { "DeliveryUnit", 0, 25 };
const recInt	DUItems			= { "DUItems", 1, 999999 };
const recDouble	DUItemSize		= { "DUItemSize", 0.1, 99999.99 };
const recDouble	DUCost			= { "DUCost", 0.0, 99999999.0 };
const recString	SupplierRef		= { "SupplierRef", 0, 16 };
const recString	Wrapper			= { "Wrapper", 0, 16 };
const recDouble	DURetail		= { "DURetail", 0.0, 99999999.0 };
};

/**********************************************************************/

class CStockCSVRecord : public CCSVRecord
{
public:
	CStockCSVRecord(); //fill with default values
	virtual ~CStockCSVRecord();

	virtual void ConvertToCSV ( CCSV& csv ); 
	void ConvertToCSVSimpleExport ( CCSV& csv, CSupplierCSVArray& SupplierArray, CPtrArray& CustomFieldArrays, int nSupplierCount = -1 ); 
	virtual void ConvertFromCSV ( CCSV& csv ); 
	
	static CString GetHeader ( int nSupplierCount );
	void ImportLine ( CSSImportFile& import, CCSV& csv, int nSupplierCount, CSupplierCSVArray& SupplierArray, CPtrArray& CustomFieldArrays );
	CString StripSpaces ( const char* szText );

	//stock level format conversions
	void ConvertStockLevelToStockSub ( double dLevel, __int64& nStock, __int64& nSub );
	void ConvertStockLevelToStockSubForPluEdit ( double dLevel, __int64& nStock, double& dRemainder );
	void ConvertStockLevelToTotalSubUnits( double dLevel, __int64& nSubUnits );
	void ConvertTotalSubUnitsToStockLevel( __int64 nTotalSub, double& dLevel );

	//return all suppliers to default settings
	void ClearSuppliers();
	void ClearSupplier( int nSuppIdx );

	//removes any suppliers with number zero, shuffles array downwards and 
	//places the number of actual suppliers in m_nSupplierCount
	void TidySuppliers( int nUserMaxSuppliers );

	//swaps the given supplier with supplier zero
	void MakePreferred( int nSuppIdx );

	//purge routines to keep in synch with other databases
	int PurgeSupplier ( int nSuppNo, int nUserMaxSuppliers );
	
	//get functions for basic stock code data
	const char* GetStockCode() { return m_strStockCode; }
	const char* GetImportSheetCode() { return m_strImportSheetCode; }
	const char* GetDescription() { return m_strDescription; }
	int GetCategory() { return m_nCategory; }
	int GetCustomFieldValueNo( int nFieldNo );
	int GetSubUnits( bool bEnabled );
	const char* GetStockUnitName() { return m_strStockUnit; }
	const char* GetStockmateText() { return m_strStockmateText; }
	const char* GetSubUnitName() { return m_strSubUnit; } 
	int GetSUWeightType() { return m_nSUWeightType; }
	int GetUnitWeightType() { return m_nSUWeightType / 100; }
	int GetSaleWeightType() { return m_nSUWeightType % 100; }
	double GetSUWeight( bool bEnabled );
	double GetWastePercent() { return m_dWastePercent; }
	bool GetDisableAutoOrderFlag() { return m_bDisableAutoOrder; }
	bool GetDisablePluRangeFlag() { return m_bDisablePluRange; }
	bool GetAutoLabelFlag() { return m_bAutoLabel; }
	bool GetLocalSupplierFlag() { return m_bLocalSupplier; }

	double GetQtyToWeightFactor();
	static void GetValidSaleWeights( int nUnitWeight, CWordArray& SaleWeights );
	static void GetUnitAndSaleWeights( int nWeightType, int& nUnitWeight, int& nSaleWeight );

	//get functions for supplier/yield list
	int GetSupplierCount() { return m_nSupplierCount; }
	int GetSuppNo ( int nIndex ); 
	const char* GetCase ( int nIndex );
	int GetDUItems( int nIndex );
	double GetDUItemSize( int nIndex );
	double GetCost ( int nIndex ); 
	const char* GetSuppRefInternal ( int nIndex );
	const char* GetSuppRefExternal ( int nIndex, bool bUseStockCode );
	const char* GetWrapper( int nIndex );
	double GetRetail( int nIndex );

	double GetYield ( int nIndex ) { return GetDUItemSize( nIndex ) * GetDUItems( nIndex ); } 
	
	//set functions for basic stock code data
	void SetStockCode( const char* szCode );
	void SetImportSheetCode( const char* szCode );
	void SetDescription ( const char* szDescription );
	void SetCategory ( int nCategory );
	void SetCustomFieldValueNo ( int nFieldNo, int nNo );
	void SetStockUnitName ( const char* szName );
	void SetStockmateText ( const char* szText );
	void SetSubUnits ( int nQty );
	void SetSubUnitName ( const char* szName );
	void SetSUWeightType( int nType );
	void SetSUWeightType( int nUnitType, int nSaleType );
	void SetSUWeight( double dWeight );
	void SetWastePercent ( double dValue ); 
	void SetDisableAutoOrderFlag ( bool bFlag ); 
	void SetDisablePluRangeFlag ( bool bFlag ); 
	void SetAutoLabelFlag( bool bFlag );
	void SetLocalSupplierFlag( bool bFlag );

	//set functions for supplier/yield list
	void SetSuppNo ( int nIndex, int nSuppNo );
	void SetCase ( int nIndex, const char* szCase ); 
	void SetDUItems ( int nIndex, int nDUItems );
	void SetDUItemSize( int nIndex, double dDUItemSize );
	void SetCost ( int nIndex, double dCost ); 
	void SetSuppRef ( int nIndex, const char* szSuppRef ); 
	void SetWrapper( int nIndex, const char* szWrapper );
	void SetRetail( int nIndex, double dRetail );
	
	//get unit cost per supplier
	double GetUnitCost();
	double GetUnitCost ( int nIndex );

	//get retail per supplier
	double GetUnitRetail();
	double GetUnitRetail ( int nIndex );

private:
	void ClearRecord();
	void V1ConvertFromCSV ( CCSV& csv );
	void V2ConvertFromCSV ( CCSV& csv );
	void V3to5ConvertFromCSV ( CCSV& csv );
	void V6to7ConvertFromCSV( CCSV& csv );
	void V8to14ConvertFromCSV( CCSV& csv, int nVer );

private:
	void ExportCustomFieldKey( CCSV& csv, int nFieldNo, CPtrArray& CustomFieldArrays );
	void ImportCustomFieldKey( CSSImportFile& import, int n, CCSV& csv, int nFieldNo, CPtrArray& CustomFieldArrays );

private:
	//Record fields
	CString m_strStockCode;
	CString m_strImportSheetCode;
	CString m_strDescription;
	CString m_strStockLevel;
	CString m_strPluNo;
	int m_nCategory;
	int m_nCustomFieldValueNo[MAX_ITEM_CUSTOM_FIELDS];
	CString m_strStockUnit;
	CString m_strStockmateText;
	CString m_strSubUnit;
	int m_nSubUnits;
	int m_nSUWeightType;
	double m_dSUWeight;
	double m_dWastePercent;
	int m_nSupplierCount;
	bool m_bDisableAutoOrder;
	bool m_bDisablePluRange;
	bool m_bAutoLabel;
	bool m_bLocalSupplier;
	
private:
	//supplier details
	CStockItemSupplier Suppliers[MAX_ITEM_SUPPLIERS];
};

/**********************************************************************/

class CStockCSVArray : public CSharedCSVArray
{
public:
	CStockCSVArray();

public:
	bool FindStockItemByCode ( const char* szCode, int& nIndex );
	const char* GetStockCode ( int nIndex );
	const char* GetDescription( int nIndex );
	int GetCategory ( int nIndex );
	int PurgeSupplier ( int nSuppNo, int nUserMaxSupplier );
	
	//IMPORTS A LINE OF STOCK INFORMATION
	int ImportLine ( int nImportMethod, CSSImportFile& import, const char* szImportLine, int nSupplierCount, CSupplierCSVArray& SupplierArray, CPtrArray& CustomFieldArrays, int nUserMaxSuppliers );

private:
	int PurgeSupplierInternal ( int nSuppNo, int nUserMaxSupplier );

private:
	CString m_strStockCode;
	CString m_strDescription;
};

/**********************************************************************/
#endif
/**********************************************************************/
