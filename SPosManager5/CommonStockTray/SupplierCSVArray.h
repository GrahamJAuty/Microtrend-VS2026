/**********************************************************************/
#pragma once
/**********************************************************************/
#if defined(STOCKMAN_SYSTEM) || defined(POSTRAY_UTILITY)
/**********************************************************************/
 
/**********************************************************************/
#define SUPPLIERDB_VERSION 1
#define MAX_SYSTEM_SUPPLIERS 999
/**********************************************************************/

namespace Supplier
{
const recString	SupplierKey	= { "SupplierKey", 1, 16 };
const recInt	SupplierNo	= { "SupplierNo", 1, 999 };
const recString	Name		= { "Name", 0, 25 };
const recString	Address1	= { "Address1", 0, 35 };
const recString	Address2	= { "Address2", 0, 35 };
const recString	Address3	= { "Address3", 0, 35 };
const recString	Address4	= { "Address4", 0, 35 };
const recString	Address5	= { "Address5", 0, 35 };
const recString	Postcode	= { "Postcode", 0, 10 };
const recString	FAO			= { "FAO", 0, 35 };
const recString	Phone		= { "Phone", 0, 20 };
const recString	Fax			= { "Fax", 0, 25 };
const recString	Email		= { "Email", 0, 50 };
const recString	AccountNo	= { "AccountNo", 0, 50 };
};

/**********************************************************************/

class CSupplierCSVRecord : public CCSVRecord
{
public:
	CSupplierCSVRecord(); 
	virtual ~CSupplierCSVRecord() {}

	virtual void ConvertToCSV ( CCSV& csv ); 
	virtual void ConvertFromCSV ( CCSV& csv ); 
	
	static CString GetHeader();
	
	void ImportLine ( CSSImportFile& import, CCSV& csv, bool bImportSuppNo );
	CString StripSpaces ( const char* szText );
	
	int GetSuppNo(){ return m_nSuppNo; }
	const char* GetKey() { return m_strKey; }
	const char* GetName(){ return m_strName; }
	const char* GetAddress1() { return m_strAddress1; }
	const char* GetAddress2() { return m_strAddress2; }
	const char* GetAddress3() { return m_strAddress3; }
	const char* GetAddress4() { return m_strAddress4; }
	const char* GetAddress5() { return m_strAddress5; }
	const char* GetPostcode() { return m_strPostcode; }
	const char* GetFAO() { return m_strFAO; }
	const char* GetPhone() { return m_strPhone; }
	const char* GetFax() { return m_strFax; }
	const char* GetEmail() { return m_strEmail; }
	const char* GetAccountNo() { return m_strAccountNo; }
	const char* GetOrderFilename();
	const char* GetAuditFilename();
	const char* GetTempFilename();
	
	void SetSuppNo ( int nSuppNo );
	void SetKey ( const char* szKey );
	void SetName ( const char* szName );
	void SetAddress1 ( const char* szAddress1 );
	void SetAddress2 ( const char* szAddress2 );
	void SetAddress3 ( const char* szAddress3 );
	void SetAddress4 ( const char* szAddress4 );
	void SetAddress5 ( const char* szAddress5 );
	void SetPostcode ( const char* szPostcode );
	void SetFAO ( const char* szFAO );
	void SetPhone ( const char* szPhone );
	void SetFax ( const char* szFax );
	void SetEmail ( const char* szEmail );
	void SetAccountNo ( const char* szAccountNo );

private:
	void ClearRecord();
	void V1ConvertFromCSV( CCSV& csv);

private:
	int m_nSuppNo;
	CString m_strKey;
	CString m_strName;
	CString m_strAddress1;
	CString m_strAddress2;
	CString m_strAddress3;
	CString m_strAddress4;
	CString m_strAddress5;
	CString m_strPostcode;
	CString m_strFAO;
	CString m_strPhone;
	CString m_strFax;
	CString m_strEmail;
	CString m_strLine;
	CString m_strAccountNo;
	CString m_strOrderFilename;
	CString m_strAuditFilename;
	CString m_strTempFilename;
};

/**********************************************************************/

class CSupplierCSVArray : public CSharedCSVArray
{
public:
	CSupplierCSVArray();
	
	bool ValidateSuppliers();
	
	void RemoveAll();
	
	int GetSuppNo ( int nSuppIdx );
	const char* GetKey ( int nSuppIdx );
	const char* GetName ( int nSuppIdx );
	const char* GetOrderFilename ( int nSuppIdx );
	const char* GetAuditFilename ( int nSuppIdx );
	const char* GetTempFilename ( int nSuppIdx );
	
	bool FindSupplierByNumber ( int nSuppNo, int& nSuppIdx );
	bool FindSupplierByKey ( const char* szKey, int& nSuppIdx );

	int FindFreeNumber();
	int AddSupplier( CSupplierCSVRecord& SupplierRecord );
	void DeleteSupplier ( int nSuppIdx );

	void CopyArray( CSupplierCSVArray& source );

	//IMPORTS A LINE OF SUPPLER INFORMATION
	int ImportLine ( CSSImportFile& import, const char* szImportLine );

	bool IsValid() { return m_bIsValid; }
	int GetErrorType() { return m_nErrorType; }
	int GetErrorLine() { return m_nErrorLine; }

private:
	CString m_strTemp;
	CString m_strOrderFilename;
	CString m_strAuditFilename;
	CString m_strTempFilename;
	
private:
	CWordArray m_arraySuppNoTable;
	
private:
	bool m_bIsValid;
	int m_nErrorType;
	int m_nErrorLine;
};

/**********************************************************************/
#endif
/**********************************************************************/
