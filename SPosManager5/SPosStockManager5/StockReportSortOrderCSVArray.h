#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
 
#include "SalesHistoryFields.h"
/**********************************************************************/
#define STOCKREPORTSORTORDER_VERSION 1
/**********************************************************************/
#define SORTORDER_SELECT_CURRENT 0
#define SORTORDER_SELECT_INDEX 1
#define SORTORDER_SELECT_ITEMDATA 2
/**********************************************************************/

class CStockReportSortOrderCSVRecord : public CCSVRecord
{
public:
	CStockReportSortOrderCSVRecord(); 
	virtual ~CStockReportSortOrderCSVRecord() {}
	virtual void ConvertToCSV ( CCSV& csv ); 
	virtual void ConvertFromCSV ( CCSV& csv ); 
	void Reset() { ClearRecord(); }

	const char* GetActualName(){ return m_strName; }
	int GetSortOrderType() { return m_nSortOrderType; }
	void GetSortOrderFieldsInternal( CWordArray& arrayTemp );
	void GetSortOrderFieldsExternal( CWordArray& arrayTemp );
	
	void SetName ( const char* szName ) { m_strName = szName; }
	void SetSortOrderType ( int nType ) { m_nSortOrderType = nType; }
	void SetSortOrderFields( CWordArray& arrayTemp );
	void TidySortOrderFields();
	
	bool CanBeCopied();
	bool CreateCopy( CStockReportSortOrderCSVRecord& copy ); 

	const char* GetFieldListName( int nField );
	const char* GetKey();

private:
	bool HasField( int nField );

private:
	void ClearRecord();
	void V1ConvertFromCSV( CCSV& csv );
	
public:
	const char* GetListName(); 
	void GetValidFieldList( CWordArray& array ); 
	
protected:
	CString m_strName;
	int m_nSortOrderType;
	CWordArray m_arrayFields;
	
protected:
	CString m_strListName;
	CString m_strFieldListName;
	
protected:
	CString m_strKey;
};

/**********************************************************************/

class CStockReportSortOrderCSVArray : public CSharedCSVArray
{
public:
	CStockReportSortOrderCSVArray();
	
public:
	int Open( const char* szFilename, int nMode );
	
public:
	const char* GetName ( int nIndex );
	int GetSortOrderType( int nIndex );
	
	void LoadAdhocSortOrder( CStockReportSortOrderCSVRecord& SortOrder );
	void SaveAdhocSortOrder( CStockReportSortOrderCSVRecord& SortOrder );

	bool AllowSortOrder( int nSortOrderType );
	void GetSystemSortOrderList( CWordArray& array );

	void AddSortOrder( int nType );

	void CopyFrom( CStockReportSortOrderCSVArray& Source );

public:
	static void GetReportName( int nSortOrderType, CString& strName );

protected:	
	CString m_strName;
	CString m_strKey;

protected:
	CStockReportSortOrderCSVRecord m_adhocSortOrder;
};

/**********************************************************************/
#endif
/**********************************************************************/
