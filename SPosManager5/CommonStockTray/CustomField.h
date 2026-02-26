/**********************************************************************/
#pragma once
/**********************************************************************/
#if defined(STOCKMAN_SYSTEM) || defined(POSTRAY_UTILITY)
/**********************************************************************/
#include "ImportDefines.h"
/**********************************************************************/
#define CUSTOM_FIELD_DB_VERSION 1
#define MAX_ITEM_CUSTOM_FIELDS 5
#define MAX_CUSTOM_FIELD_VALUES 999
#define MAX_CUSTOM_FIELD_NAME_LEN 25
/**********************************************************************/

class CCustomFieldNameArray : public CSharedStringArray
{
public:
	int Open( const char* szFilename, int nMode );

public:
	const char* GetDefaultName ( int nFieldNo );
	const char* GetName ( int nFieldNo );
	bool CheckName( int nFieldNo );
	void SetName ( int nFieldNo, const char* szName );

private:
	CString m_strDefName;
	CString m_strName;
};

/**********************************************************************/

namespace CustomFieldValue
{
const recString	Key	=		{ "CustomValueKey", 1, 16 };
const recInt	Number =	{ "CustomValueNumber", 1, 999999 };
const recString	Name =		{ "CustomValueName", 0, 25 };
const recBool	Protect =	{ "CustomValueProtect" };
const recString	SetFlags =	{ "CustomValueSetFlags", 0, 8 };
};

/**********************************************************************/

class CCustomFieldValueCSVRecord : public CCSVRecord
{
public:
	CCustomFieldValueCSVRecord(); 
	virtual ~CCustomFieldValueCSVRecord() {}

	virtual void ConvertToCSV ( CCSV& csv ); 
	virtual void ConvertFromCSV ( CCSV& csv ); 
	
	static CString GetHeader();
	
	void ImportLine ( CSSImportFile& import, CCSV& csv );
	CString StripSpaces ( const char* szText );
	
	const char* GetKey() { return m_strKey; }
	int GetNumber(){ return m_nNumber; }
	const char* GetName(){ return m_strName; }
	bool GetProtectFlag(){ return m_bProtect; }
	const char* GetFieldSetFlags(){ return m_strFieldSetFlags; }
	
	void SetKey ( const char* szKey );
	void SetNumber ( int nNumber );
	void SetName ( const char* szName );
	void SetProtectFlag( bool b ){ m_bProtect = b; }
	void SetFieldSetString( const char* sz );
	void SetFieldSetFlags( const char* sz ); 

private:
	void ClearRecord();
	void V1ConvertFromCSV( CCSV& csv);

private:
	CString m_strKey;
	int m_nNumber;
	CString m_strName;
	bool m_bProtect;
	CString m_strFieldSetFlags;
};

/**********************************************************************/

struct CCustomFieldValueNumberInfo
{
	int m_nNumber;
	int m_nIdx;
};

/**********************************************************************/

class CCustomFieldValueCSVArray : public CSharedCSVArray
{
public:
	CCustomFieldValueCSVArray();
	void SetGlobalMode( bool b ){ m_bGlobalMode = b; }
	bool ValidateFieldValues();
	
public:
	void RemoveAll();

public:
	void PushNextNumberToArray();
	void PullNextNumberFromArray();

public:
	int GetFieldNo(){ return m_nFieldNo; }
	void SetFieldNo( int n );
	
public:
	int GetNextNumber(){ return m_nNextNumber; }
	void SetNextNumber( int n ){ m_nNextNumber = n; }

public:
	const char* GetKey ( int nIdx );
	int GetNumber ( int nIdx );
	const char* GetName ( int nIdx );
	
	bool FindValueByNumber ( int nNo, int& nIdx );
	bool FindValueByKey ( const char* szKey, int& nIdx );

	int FindFreeNumber();
	int AddValue( CCustomFieldValueCSVRecord& Value );
	void DeleteValue ( int nIdx );

	void CopyArray( CCustomFieldValueCSVArray& source );

	int ImportLine ( CSSImportFile& import, const char* szImportLine );

	bool IsValid() { return m_bIsValid; }
	int GetErrorType() { return m_nErrorType; }
	int GetErrorLine() { return m_nErrorLine; }

private:
	void AddToNumberMap( CCustomFieldValueNumberInfo& info );
	void RemoveFromNumberMap( CCustomFieldValueNumberInfo& info );
	
private:
	CArray<CCustomFieldValueNumberInfo,CCustomFieldValueNumberInfo> m_arrayNumberMap;
	
private:
	int m_nFieldNo;
	int m_nNextNumber;

private:
	bool m_bIsValid;
	int m_nErrorType;
	int m_nErrorLine;
	CString m_strTemp;

private:
	bool m_bGlobalMode;
};

/**********************************************************************/
#endif
/**********************************************************************/
