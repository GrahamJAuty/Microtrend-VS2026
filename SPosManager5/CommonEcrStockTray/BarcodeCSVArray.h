#pragma once
/**********************************************************************/
#include "PluBarcodeArray.h"
#include "..\SPosEcrManager5\PluMonitorEcrman.h"
#include "..\SPosStockManager5\PluMonitorStockman.h"
/**********************************************************************/
#define BARCODEDB_VERSION_NO 3
/**********************************************************************/

namespace Barcode
{
const recInt64 BarcodeNo	= { "BarcodeNo", 0, 999999999999999 };
const recInt64 PluNo		= { "PluNo", 0, 99999999999999 };
const recInt Modifier		= { "Modifier", 0, 9 };
const recBool PrintShelf	= { "PrintShelf" };
const recBool PrintProduct	= { "PrintProduct" };
};

/**********************************************************************/

class CBarcodeCSVRecord : public CCSVRecord
{
public:
	CBarcodeCSVRecord();
	virtual ~CBarcodeCSVRecord() {}
	virtual void ConvertToCSV ( CCSV& csv );
	virtual void ConvertFromCSV ( CCSV& csv );

	virtual void ConvertToCSVForCombinedExport ( CCSV& csv );
	
public:
	void SetBarcodeNo ( __int64 nBarcodeNo );
	void SetPluNo( __int64 nPluNo );
	void SetModifier( int nMod );
	void SetPrintShelfFlag( bool bFlag );
	void SetPrintProductFlag( bool bFlag );
	
public:
	__int64 GetBarcodeNoInt() { return m_nBarcodeNo; }
	__int64 GetPluNoInt() { return m_nPluNo; }
	int GetModifier() { return m_nModifier; }
	const char* GetBarcodeNoString();
	const char* GetPluNoString();
	
	const char* GetBarcodeNoForDownload();
	bool GetPrintShelfFlag() { return m_bPrintShelf; }
	bool GetPrintProductFlag() { return m_bPrintProduct; }

	const char* GetPrintFlagText( bool bShort = FALSE );

public:
	static CString GetHeader();
	void ImportLine ( CSSImportFile& import, CCSV& csv );
	
	static CString GetHeaderForCombinedExport();

private:
	void ClearRecord();
	void V1ConvertFromCSV ( CCSV& csv );
	void V2to3ConvertFromCSV ( CCSV& csv, int nVer );

private:
	__int64 m_nPluNo;
	int m_nModifier;
	__int64 m_nBarcodeNo;
	CString m_strBarcodeNoForDownload;
	CString m_strPrintFlagText;
	bool m_bPrintShelf;
	bool m_bPrintProduct;

private:
	CString m_strPluNoString;
	CString m_strBarcodeNoString;
};

/**********************************************************************/

class CBarcodeCSVArray : public CSharedCSVArray
{
public:
	CBarcodeCSVArray();

	int Open( const char* szFilename, int nMode );

	bool FindBarcodeByNumber ( __int64 nBarcodeNo, int& nIndex );
	__int64 GetBarcodeNoInt( int nIndex );
	const char* GetBarcodeNoString( int nIndex );

	void InvalidatePluBarcodeList() { m_bGotPluBarcodeList = FALSE; }
	void CreatePluBarcodeList();
	
	void GetBarcodeIdxArray( __int64 nPluNo, CArray<int,int>& BarcodeIdxArray );
	
	void InsertRecord( int nIndex, CBarcodeCSVRecord& BarcodeRecord );
	void RemoveRecord( int nIndex );

	void GetUnlinkedBarcodeList( CArray<int,int>& BarcodeIdxArray );
	void DeleteBarcodes( CArray<int,int>& BarcodeIdxArray );

	int ImportLine ( int nImportMethod, int& nIndex, CSSImportFile& import, const char* szImportLine );

	__int64 GetPluFromBarcode( __int64 nBarcodeNo );

private:
	CPluBarcodeArray m_PluBarcodeArray;
	bool m_bGotPluBarcodeList;

private:
	CString m_strBarcodeNo;
};

/**********************************************************************/
