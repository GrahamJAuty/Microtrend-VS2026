#pragma once
/**********************************************************************/
#define ACAT_VERSION_NO 1
/**********************************************************************/
#define MAX_ACAT_NO 99
#define MAX_LENGTH_CATNO 2
/**********************************************************************/
#define ACAT_FOOD 1
#define ACAT_DRINKS 2
#define ACAT_OTHER 3
/**********************************************************************/

namespace AnalysisCategory
{
const recString	EposText	= { "EposText", 0, 30 };
const recInt Type			= { "Type", 1, 3 };
};

/**********************************************************************/

class CAnalysisCategoryCSVRecord : public CCSVRecord
{
public:
	CAnalysisCategoryCSVRecord();
	virtual ~CAnalysisCategoryCSVRecord() {}
	virtual void ConvertToCSV ( CCSV& csv );
	virtual void ConvertFromCSV ( CCSV& csv );

public:
	void SetEposText ( const char* szEposText );
	void SetType( int nType );

public:
	const char* GetEposText() { return m_strEposText; }
	int GetType() { return m_nType; }
	
private:
	void ClearRecord();
	void V1ConvertFromCSV ( CCSV& csv );

private:
	CString m_strEposText;
	int m_nType;
};

/**********************************************************************/

class CAnalysisCategoryCSVArray : public CSharedCSVArray
{
public:
	CAnalysisCategoryCSVArray();

public:
	int Open( const char* szFilename, int nMode );

	void GetCategory( int nCatNo, CAnalysisCategoryCSVRecord& Record );
	void GetDisplayCategory( int nCatNo, CAnalysisCategoryCSVRecord& Record );
	void SetCategory( int nCatNo, CAnalysisCategoryCSVRecord& Record );

	const char* GetDisplayText( int nCatNo );
	const char* GetTabbedComboText( int nCatNo );

public:
	bool GotFile() { return m_bGotFile; }

public:
	void CreateInUseArray();
	int GetInUseArraySize(){ return m_bGotInUseArray ? m_arrayInUse.GetSize() : 0; }
	int GetInUseCatNo( int nIdx );

private:
	void RemoveAll();
	void GetDefaultRecord( int nCatNo, CAnalysisCategoryCSVRecord& Record );
	
public:
	void FillArray();

public:
	static void PrepareFilterCombo( CSSTabbedComboBox& comboCategory);
	static int GetFilterFromCombo(CSSTabbedComboBox& comboCategory);
	
private:
	CString m_strFilename;
	CString m_strDisplayText;
	CString m_strTabbedComboText;
	bool m_bGotFile;

private:
	bool m_bGotInUseArray;
	CArray<int,int> m_arrayInUse;
};

/**********************************************************************/
