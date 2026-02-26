#pragma once
/**********************************************************************/

class CSPOSKeyboardPluFilterArray 
{
public:
	CSPOSKeyboardPluFilterArray(); 
	
	//CRITERIA FOR SELECTING RECORDS INTO SUBSET
	void SetDescriptionFilter ( const char* szDescription );
	void SetShowModifiersFlag( bool bFlag ) { m_bShowModifiers = bFlag; }
	
	//CHECK CURRENT STATUS
	const char* GetDescription() { return m_strDescription; }
	bool GetShowModifiersFlag() { return m_bShowModifiers; }
	bool GetAllFlag() { return m_bAllFlag; }

	//COPY SETTINGS FROM ANOTHER FILTER ARRAY
	void CopySettingsFrom ( CSPOSKeyboardPluFilterArray& Source );

	//CREATE SUBSET BASED ON CURRENT CRITERIA
	void BuildRecordList();
	bool BuildFirst();
	bool BuildNext();
	
	//GET DISPLAY LINE BASED ON TYPE OF LIST 
	const char* GetDisplayLine ( int nArrayIdx );
	void GetLine( int nArrayIdx  );
	
	//RETURNS NUMBER OF ITEMS IN SUBSET
	int GetSize(); 
	
	//CONVERT SUBSET INDEX TO PLU INDEX
	int GetRecordIndexAt( int nArrayIdx );
	int GetRecordIndex() { return GetRecordIndexAt ( m_nArrayIdx ); }
	
	//NAVIGATE THE SUBSET
	void SetArrayIdx( int nArrayIdx ); 	
	int GetArrayIdx() { return m_nArrayIdx; }	
	void Next();
	void Previous();
	bool HasNext();
	bool HasPrevious();
	
	//FIND ITEMS 
	bool ComparePluDescription( const char* szText, int nArrayIdx );
	bool FindPluItemByNumber ( __int64 nNumber, int& nArrayIdx );
	bool FindPluItemByDescription ( const char* szText, int nStartPos, int nSearchType, bool bMatchCase, int& nArrayIdx );
	bool FindPluItemByIndex ( int nPluIdx, int& nArrayIdx );
	
private:
	CWordArray m_IndexArray;
	int m_nBuildIndex;
	int m_nArrayIdx;

private:
	CString m_strDescription;
	bool m_bShowModifiers;
	bool m_bAllFlag;

private:
	CString m_strRecord;
	CString m_strPluNo;	
};

/**********************************************************************/
