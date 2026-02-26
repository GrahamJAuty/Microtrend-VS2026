#pragma once
/**********************************************************************/
#include "AddressInfo.h"
/**********************************************************************/
#define DATABASE_VERSION 2
/**********************************************************************/
#define DBIDX_SPECIAL_UNKNOWN 0x1000000
/**********************************************************************/

class CDatabaseCSVRecord : public CCSVRecord
{
public:
	CDatabaseCSVRecord(); 
	virtual ~CDatabaseCSVRecord(){};
	virtual void ConvertToCSV ( CCSV& csv ); 
	virtual void ConvertFromCSV ( CCSV& csv ); 
	
private:
	void SetDbNo( int nDbNo ) { m_nDbNo = nDbNo; }
	void SetName ( const char* szName );
	void SetLegacyAddress1 ( const char* szLine ) { m_strLegacyAddress1 = szLine; }
	void SetLegacyAddress2 ( const char* szLine ) { m_strLegacyAddress2 = szLine; }
	void SetLegacyAddress3 ( const char* szLine ) { m_strLegacyAddress3 = szLine; }
	void SetLegacyPostcode ( const char* szCode ) { m_strLegacyPostcode = szCode; }
	void SetLegacyPhone ( const char* szPhone ) { m_strLegacyPhone = szPhone; }
	void SetLegacyFax ( const char* szFax ) { m_strLegacyFax = szFax; }
	void SetLegacyEmail ( const char* szEmail ) { m_strLegacyEmail = szEmail; }
	void SetTaxNo ( const char* szTaxNo ) { m_strTaxNo = szTaxNo; }
		
	const char* GetSystemName();

private:
	void ClearRecord();
	virtual void V1ConvertFromCSV ( CCSV& csv ); 
	virtual void V2ConvertFromCSV ( CCSV& csv ); 
	
private:
	int m_nDbNo;
	CString m_strName;
	CString m_strSystemName;
	CString m_strLegacyAddress1;
	CString m_strLegacyAddress2;
	CString m_strLegacyAddress3;
	CString m_strLegacyPostcode;
	CString m_strLegacyPhone;
	CString m_strLegacyFax;
	CString m_strLegacyEmail;
	CString m_strTaxNo;
	
	friend class CDatabaseCSVArray;
};

/**********************************************************************/

class CDatabaseCSVArray : public CPtrArray
{
public:
	CDatabaseCSVArray();
	~CDatabaseCSVArray();
	void RemoveAll();

	bool RenameForDeletion( int nDbIdx );
	void ClearDatabaseFolder( int nDbIdx );
	void ClearDeletionFolder( int nDbIdx );

	int AddDatabase();
	int AddDatabase( int nDbNo, const char* szPath );
	bool WriteDatabase( int nDbIdx );

	void DeleteDatabaseByIndex ( int nDbIdx );
	void DeleteDatabaseByNumber ( int nDbNo );
	bool FindDatabaseByNumber ( int nDbNo, int& nDbIdx );

	void LoadDatabaseBuffer( int DbIdx, CDatabaseCSVArray& buffer );
	void SaveDatabaseBuffer( int DbIdx, CDatabaseCSVArray& buffer );
	
	const char* GetCSVLine( int nDbIdx );

	const char* GetFolderPathData ( int nDbIdx );
	const char* GetFolderPathDataDelete ( int nDbIdx );
	const char* GetFolderPathDataHistory( int nDbIdx );
	const char* GetFolderPathDataPluImage( int nDbIdx );
	const char* GetFolderPathDataKeyImage( int nDbIdx );
	const char* GetFolderPathDataPluInfo( int nDbIdx );
	
#ifdef STOCKMAN_SYSTEM
	const char* GetFolderPathDataOrdersRoot( int nDbIdx );
	const char* GetFolderPathDataOrdersSupplier( int nDbIdx, int nSuppNo );
	const char* GetFolderPathDataOrdersType( int nDbIdx, int nSuppNo, int nType );
	const char* GetFilePathDataOrder( int nDbIdx, int nSuppNo, int nType, int nOrderNum, CString& strDate, CString& strTime );
#endif

	const char* GetFilePathDataLocalPluField( int nDbIdx, int nSet );
	const char* GetFilePathDataLocalPluTemp( int nDbIdx, int nSet );
	
#ifdef STOCKMAN_SYSTEM
	const char* GetFilePathDataStockExport ( int nDbIdx );
	const char* GetFilePathDataGenDeptManualSales ( int nDbIdx );
	const char* GetFilePathDataGenDeptManualSalesBackup ( int nDbIdx );
	const char* GetFilePathDataGenDeptImportSales ( int nDbIdx );
	const char* GetFilePathDataGenDeptImportSalesBackup ( int nDbIdx );
#else
	const char* GetFilePathDataPluStockAuditBackup ( int nDbIdx );
	const char* GetFilePathDataPluStockAudit ( int nDbIdx );
	const char* GetFilePathDataPluStockAuditTemp ( int nDbIdx );
#endif

	const char* GetFolderPathSysset ( int nDbIdx );
	const char* GetFolderPathSyssetDelete ( int nDbIdx );
	
	const char* GetFilePathInfo ( int nDbIdx );
	
	int GetDbNo ( int nDbIdx );
	const char* GetName ( int nDbIdx );
	const char* GetSystemName ( int nDbIdx );
	
	const char* GetExtendedSystemName ( int nDbIdx );
	
	const char* GetLegacyAddress1 ( int nDbIdx );
	const char* GetLegacyAddress2 ( int nDbIdx );
	const char* GetLegacyAddress3 ( int nDbIdx );
	const char* GetLegacyPostcode ( int nDbIdx );
	const char* GetLegacyPhone ( int nDbIdx );
	const char* GetLegacyFax ( int nDbIdx );
	const char* GetLegacyEmail ( int nDbIdx );
	const char* GetTaxNo ( int nDbIdx );

	void SetDbNo ( int nDbIdx, int nDbNo );
	void SetName ( int nDbIdx, const char* szName );

	void SetLegacyAddress1 ( int nDbIdx, const char* szAddress1 );
	void SetLegacyAddress2 ( int nDbIdx, const char* szAddress2 );
	void SetLegacyAddress3 ( int nDbIdx, const char* szAddress3 );
	void SetLegacyPostcode ( int nDbIdx, const char* szPostcode );
	void SetLegacyPhone ( int nDbIdx, const char* szPhone );
	void SetLegacyFax ( int nDbIdx, const char* szFax );
	void SetLegacyEmail ( int nDbIdx, const char* szEmail );
	void SetTaxNo ( int nDbIdx, const char* szTaxNo );

	const char* GetDatabaseTitle( int nDbIdx, bool bGlobal, bool bBrackets = TRUE );
	
	void SortByName( CWordArray& arrayDbIdx );
	void SortDbListByName( CArray<int,int>& arrayDbIdx );

	void GetAddressInfo( int nDbIdx, CAddressInfo& info );
	void SetAddressInfo( int nDbIdx, CAddressInfo& info );

public:
	void BuildSortIdxTables();
	int GetSortIdxFromDbIdx( int nDbIdx );
	int GetDbIdxFromSortIdx( int nSortIdx );

private:
	CDatabaseCSVRecord* GetDatabase ( int nDbIdx ) { return (CDatabaseCSVRecord*) GetAt ( nDbIdx ); }
	void LogMessage ( const char* szMessage );
		
private:
	CString m_strName;
	CString m_strSystemName;
	CString m_strLegacyAddress1;
	CString m_strLegacyAddress2;
	CString m_strLegacyAddress3;
	CString m_strLegacyPhone;
	CString m_strLegacyFax;
	CString m_strLegacyEmail;
	CString m_strLegacyPostcode;
	CString m_strTaxNo;

	CString m_strFolderPath;
	CString m_strFolderPathOrder;
	CString m_strFolderPathDelete;

	CString m_strFilePath;
	CString m_strFilePathInfo;
	CString m_strFilePathLocalPluField;
	CString m_strFilePathLocalPluTemp;
	CString m_strFilePathKeyboardPlu;
	CString m_strFilePathKeyboardDept;
	
#ifdef STOCKMAN_SYSTEM
	CString m_strFilePathPluManualSales;
	CString m_strFilePathPluManualSalesBackup;
	CString m_strFilePathGenDeptManualSales;
	CString m_strFilePathGenDeptManualSalesBackup;
	CString m_strFilePathPluImportSales;
	CString m_strFilePathPluImportSalesBackup;
	CString m_strFilePathGenDeptImportSales;
	CString m_strFilePathGenDeptImportSalesBackup;
	CString m_strFilePathStockExport;
#else
	CString m_strFilePathPluStockAudit;
	CString m_strFilePathPluStockAuditTemp;
	CString m_strFilePathPluStockAuditBackup;
#endif

	CString m_strDatabaseTitle;

	CString m_strCSVLine;

	CWordArray m_arraySortIdxToDbIdx;
	CWordArray m_arrayDbIdxToSortIdx;
};

/**********************************************************************/
extern CDatabaseCSVArray dbDatabase;			//global
/**********************************************************************/





