#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
 
/**********************************************************************/
#define CUSTOM_FIELD_SET_VERSION 1
/**********************************************************************/
#define MAX_CUSTOM_FIELD_SET 31
/**********************************************************************/

class CCustomFieldSetCSVRecord : public CCSVRecord
{
public:
	CCustomFieldSetCSVRecord(); 
	virtual ~CCustomFieldSetCSVRecord() {}
	virtual void ConvertToCSV ( CCSV& csv ); 
	virtual void ConvertFromCSV ( CCSV& csv ); 
	
	void SetNumber( int n );
	void SetName ( const char* szName ) { m_strName = szName; }
	void SetIncludeNAFlag( bool b ){ m_bIncludeNA = b; }
	
	int GetNumber(){ return m_nNumber; }
	const char* GetName() { return m_strName; }
	bool GetIncludeNAFlag(){ return m_bIncludeNA; }
	
private:
	void ClearRecord();
	void V1ConvertFromCSV( CCSV& csv );

private:
	int m_nNumber;
	CString m_strName;  
	bool m_bIncludeNA;
};

/**********************************************************************/

class CCustomFieldSetCSVArray : public CSharedCSVArray
{
public:
	CCustomFieldSetCSVArray();

public:
	void SetFieldNo( int n );
	
public:
	void InitialiseInUseFlags();
	void SetInUseFlag( int n, bool bFlag );
	
public:
	int GetNextFreeNumber();

public:
	int GetNumber( int nIndex );
	const char* GetName( int nIndex );

public:
	void CopyArray( CCustomFieldSetCSVArray& source );

private:
	int m_nFieldNo;
	CString m_strName;

private:
	bool m_bInUseFlag[ MAX_CUSTOM_FIELD_SET ];
};

/**********************************************************************/
#endif		
/**********************************************************************/
