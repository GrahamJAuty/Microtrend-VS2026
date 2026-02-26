#pragma once
/**********************************************************************/
#include "DepartmentSetCSVArray.h"
#include "..\SPosEcrManager5\PluMonitorEcrman.h"
#include "..\SPosStockManager5\PluMonitorStockman.h"
/**********************************************************************/

class CBarcodeFilterArray 
{
public:
	CBarcodeFilterArray( int nFilterType ); 
	
	//CRITERIA FOR SELECTING RECORDS INTO SUBSET
	void SetDepartmentFilter ( int nDepartmentFilter = 0 );
	void SetDescriptionFilter ( const char* szDescription, int nSearchType, bool bMatchCase );
	void SetEanFlag( bool bEan ) { m_bEan = bEan; }

	//CHECK CURRENT STATUS
	int GetFilterType() { return m_nFilterType; }
	int GetDepartmentFilter() { return m_nDepartmentFilter; }
	const char* GetDescription() { return m_strDescription; }
	int GetSearchType() { return m_nSearchType; }
	bool GetMatchCase() { return m_bMatchCase; }
	bool GetAllFlag() { return m_bAllFlag; }
	bool GetEanFlag() { return m_bEan; }

	//COPY SETTINGS FROM ANOTHER FILTER ARRAY
	void CopySettingsFrom ( CBarcodeFilterArray& Source );

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
	int GetBarcodeItemIndexAt( int nArrayIdx );
	int GetBarcodeItemIndex() { return GetBarcodeItemIndexAt ( m_nArrayIdx ); }
	
	//NAVIGATE THE SUBSET
	void SetArrayIdx( int nArrayIdx ); 	
	int GetArrayIdx() { return m_nArrayIdx; }	
	void Next();
	void Previous();
	bool HasNext();
	bool HasPrevious();
	
	//FIND ITEMS 
	bool ComparePluDescription( const char* szText, int nArrayIdx, int nSearchType, bool bMatchCase );
	bool FindBarcodeByNumber ( __int64 nBarcodeNo, int& nArrayIdx );
	bool FindBarcodeByDescription ( const char* szText, int nStartPos, int nSearchType, bool bMatchCase, int& nArrayIdx );
	bool FindBarcodeByIndex ( int nBarcodeIdx, int& nArrayIdx );

public:	
	void GetDefaultRange ( int nArrayIdx, __int64& nBarcodeFrom, __int64& nBarcodeTo );
	__int64 GetFirstBarcodeNo();
	__int64 GetLastBarcodeNo();

#ifdef STOCKMAN_SYSTEM
	
	//CRITERIA FOR SELECTING RECORDS INTO SUBSET
	void SetStockcodeFilter ( const char* szCode ) { m_strStockcode = szCode; }
		
	//CHECK CURRENT STATUS
	const char* GetStockcodeFilter() { return m_strStockcode; }
	
#endif

private:
	CArray<int,int> m_arrayBarcodeItemIndex;
	int m_nBuildIndex;
	int m_nArrayIdx;
	
private:
	CDepartmentSetCSVRecord m_DepartmentSet;

	int m_nDepartmentFilter;
	int m_nFilterType;
	CString m_strBarcodeNo;
	CString m_strRecord;
	CString m_strDescription;
	int m_nSearchType;
	bool m_bMatchCase;
	bool m_bAllFlag;
	bool m_bEan;

#ifdef STOCKMAN_SYSTEM
	CString m_strStockcode;
#endif
};

/**********************************************************************/
