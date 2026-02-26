#pragma once
/**********************************************************************/
 
/**********************************************************************/

class CPluBarcodeRecord : public CCSVRecord
{
public:
	CPluBarcodeRecord(); //fill with default values
	void SetPluNo( __int64 nPluNo ) { m_nPluNo = nPluNo; }
	virtual void ConvertToCSV ( CCSV& csv ); 
	virtual void ConvertFromCSV ( CCSV& csv );
	
	__int64 GetPluNo() { return m_nPluNo; }

	int GetBarcodeNoCount() { return m_arrayBarcodeNo.GetSize(); }
	__int64 GetBarcodeNo ( int nListPos );
	bool AddBarcodeNo ( __int64 );
	bool RemoveBarcodeNo ( __int64 );

private:
	__int64 m_nPluNo;
	CArray<__int64,__int64> m_arrayBarcodeNo;
};

/**********************************************************************/

class CPluBarcodeArray : public CCSVArray
{
public:
	bool FindPluNo ( __int64 nPluNo, int& nIndex );
	__int64 GetPluNo ( int nIndex );
	
	int GetBarcodeNoCount ( int nIndex );
	__int64 GetBarcodeNo ( int nIndex, int nListPos );
	bool AddLink ( __int64 nPluNo, __int64 nBarcodeNo );
	bool RemoveLink ( __int64 nPluNo, __int64 nBarcodeNo );
	
	void RemoveAll();
	void Rebuild();

private:
	void RebuildInternal();
};

/**********************************************************************/
