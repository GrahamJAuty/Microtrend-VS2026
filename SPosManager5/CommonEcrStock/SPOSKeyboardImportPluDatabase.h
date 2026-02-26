#pragma once
/**********************************************************************/

class CSPOSKeyboardImportPluCSVRecord : public CCSVRecord
{
public:
	CSPOSKeyboardImportPluCSVRecord(); 
	virtual void ConvertFromCSV ( CCSV& csv ); 
	
public:
	__int64 GetPluNumber() { return m_nPluNumber; }
	int GetDeptNo(){ return m_nDeptNo; }
	int GetCategoryNo(){ return m_nCategoryNo; }
	const char* GetFormattedPluNumber();
	const char* GetRepText() { return m_strRepText; }
	const char* GetEcrText() { return m_strEcrText; }
	const char* GetKeyText() { return m_strKeyText; }
	const char* GetBasePluEcrText() { return m_strBasePluEcrText; }
	const char* GetImageFilename() { return m_strImageFilename; }
	int GetVersion(){ return m_nVersion; }

	void AddToCombo( CSSComboBox& Combo );
	void AddToList( CSSListCtrl& List );

public:
	const char* GetComboOrListText();
	const char* GetSharpPosDefaultText();
	const char* GetBackOfficeDefaultText();

private:
	const char* GetPluText( CStringArray& arrayTexts );

private:
	void V1ConvertFromCSV ( CCSV& csv );
	void V2ConvertFromCSV ( CCSV& csv );
	void V3To5ConvertFromCSV ( CCSV& csv, int nVer );
	
private:
	__int64 m_nPluNumber;
	int m_nDeptNo;
	int m_nCategoryNo;
	CString m_strPluNumber;
	CString m_strEcrText;
	CString m_strKeyText;
	CString m_strRepText;
	CString m_strBasePluEcrText;
	CString m_strImageFilename;
	int m_nVersion;
	
private:
	CString m_strPluText;
};

/**********************************************************************/

class CSPOSKeyboardImportPluCSVArray : public CCSVArray
{
public:
	CSPOSKeyboardImportPluCSVArray();
	void Reset();

public:
	BOOL Read( const char* szPath );
	BOOL ReRead();
	
	void SetImportPluFlag( BOOL bFlag ) { m_bImportPlu = bFlag; }
	BOOL GetImportPluFlag() { return m_bImportPlu; }	
	
	BOOL FindPluItem ( __int64 nPluNo, int& nIndex );

	bool GotDeptAndCategory();
		
private:
	CString m_strDescription;
	BOOL m_bImportPlu;

private:
	CString m_strPath;
};

/**********************************************************************/
