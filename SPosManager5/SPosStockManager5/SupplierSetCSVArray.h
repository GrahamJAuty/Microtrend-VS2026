#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
 
/**********************************************************************/
#define SUPPLIERSET_VERSION 1
/**********************************************************************/

class CSupplierSetCSVRecord : public CCSVRecord
{
public:
	CSupplierSetCSVRecord(); 
	virtual ~CSupplierSetCSVRecord() {}
	virtual void ConvertToCSV ( CCSV& csv ); 
	virtual void ConvertFromCSV ( CCSV& csv ); 
	
	void RemoveAll() { m_arraySuppNo.RemoveAll(); }
	void SetName ( const char* szName ) { m_strName = szName; }
	void AddSuppNo ( int nSuppNo ) { m_arraySuppNo.Add ( nSuppNo ); }

	const char* GetName() { return m_strName; }
	int GetSupplierCount() { return m_arraySuppNo.GetSize(); }
	int GetSuppNo ( int nIndex );

	void GetSupplierFlags( CByteArray& SupplierFlags );
	void PurgeSupplier ( int nSuppNo );

private:
	void ClearRecord();
	void V1ConvertFromCSV( CCSV& csv );

private:
	CWordArray m_arraySuppNo;
	CString m_strName;  
};

/**********************************************************************/

class CSupplierSetCSVArray : public CSharedCSVArray
{
public:
	CSupplierSetCSVArray();

public:
	const char* GetName ( int nIndex );

	int CreateSuppSelectArray ( int nSupplierFilter, CWordArray& SuppSelectArray );
	void PurgeSupplier ( int nSuppNo );
	
private:
	CString m_strName;
};

/**********************************************************************/
#endif		
/**********************************************************************/
