#pragma once
/**********************************************************************/
 
/**********************************************************************/

struct CBarcodePluNoTableInfo
{
	CString m_strBarcodeFormat;
	CString m_strPluFormat;
};

/**********************************************************************/

class CBarcodePluNoTable : public CFileStringArray
{
public:
	CBarcodePluNoTable();
	
	bool Write();
	bool Read();
	
	const char* GetBarcodeFormat ( int nBarcodeNoLen );
	const char* GetPluFormat ( int nPluNoLen );

	const char* GetDefaultBarcodeFormat ( int nBarcodeNoLen );
	const char* GetDefaultPluFormat ( int nPluNoLen );
	
	bool BarcodeHasCheckDigit ( int nBarcodeNoLen );
	bool PluHasCheckDigit ( int nPluNoLen );

	void SetBarcodeFormat ( int nBarcodeNoLen, const char* szFormat );
	void SetPluFormat ( int nPluNoLen, const char* szFormat );

	const char* ConvertBarcode ( const char* szUserBarcode );
	const char* ConvertBarcodeInt ( __int64 nBarcodeNo );

	const char* ConvertPlu ( const char* szUserPlu );
	const char* ConvertPluInt ( __int64 nPluNo );
	const char* ConvertShortPlu( const char* szUserPluNo );

	const char* ConvertStockCode( const char* szUserStock );
	
	void RestoreDefaults( int nBarcodeAction, int nPluAction );

	int GetTableSize(){ return m_arrayTable.GetSize(); }
	
public:
	static bool ValidateCheckDigit ( const char* szBarcodeNo );
	static bool IsValidEan( const char* szBarcodeNo );
	static bool IsValidEan( __int64 nBarcodeNo );
	bool GetFatalReadErrorFlag(){ return m_bFatalReadError; }

public:
	void RememberSettings();
	void RestoreSettings();

private:	
	int GetSize(){ return CFileStringArray::GetSize(); }
	static int GetCheckDigit( const char* szBarcodeNo );
	
private:
	CArray<CBarcodePluNoTableInfo,CBarcodePluNoTableInfo> m_arrayTable;
	CArray<CBarcodePluNoTableInfo,CBarcodePluNoTableInfo> m_arrayMemory;
	CString m_strText;
	bool m_bFatalReadError;
};

/**********************************************************************/
extern CBarcodePluNoTable BarcodePluNoTable;
/**********************************************************************/

