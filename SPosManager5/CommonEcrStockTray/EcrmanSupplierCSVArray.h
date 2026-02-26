#pragma once
/**********************************************************************/
 
/**********************************************************************/
#define ECRMANSUPPLIER_VERSION_NO 1
/**********************************************************************/

namespace EcrmanSupplier
{
const recInt		SupplierNo	= { "SupplierNo", 1, 999 };
const recString	SupplierName	= { "SupplierName", 0, 32 };
};

/**********************************************************************/

class CEcrmanSupplierCSVRecord : public CCSVRecord
{
public:
	CEcrmanSupplierCSVRecord();
	virtual ~CEcrmanSupplierCSVRecord() {}
	virtual void ConvertToCSV ( CCSV& csv );
	virtual void ConvertFromCSV ( CCSV& csv );

	const char* GetDefaultName();

public:
	void SetSuppNo ( int nSuppNo );
	void SetSuppName ( const char* szSuppName );
	
public:
	int GetSuppNo() { return m_nSuppNo; }
	const char* GetSuppName() { return m_strSuppName; }
	
private:
	void ClearRecord();
	void V1ConvertFromCSV ( CCSV& csv );

private:
	int m_nSuppNo;
	CString m_strSuppName;

private:
	CString m_strDefaultName;
};

/**********************************************************************/

class CEcrmanSupplierCSVArray : public CSharedCSVArray
{
public:
	CEcrmanSupplierCSVArray();
	
public:
	int Open( const char* szFilename, int nMode );

public:
	void RemoveAll();
	void CopyFrom( CEcrmanSupplierCSVArray& Source );

	bool FindSupplierByNumber ( int nSuppNo, int& nIndex );
	int GetNextFreeNumber();
};

/**********************************************************************/
