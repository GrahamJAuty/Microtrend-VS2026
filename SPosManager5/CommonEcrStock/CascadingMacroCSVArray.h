#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#define CASCADINGMACRO_VERSION 1
/**********************************************************************/

namespace CascadingMacro
{
	const recInt		MacroNo =						{ "MacroNo", 1, 9999 };
	const recInt		ParentMacroTypeInternal =		{ "ParentMacroTypeInternal", 0, 4 };
	const recString		MacroName =						{ "MacroName", 0, 50 };
	const recString		MacroData =						{ "MacroData", 0, 100 };
	const recBool		LocalMacroFlag =				{ "LocalMacro" };	
};

/**********************************************************************/

class CCascadingMacroCSVRecord : public CCSVRecord
{
public:
	CCascadingMacroCSVRecord(); 
	virtual ~CCascadingMacroCSVRecord() {}
	virtual void ConvertToCSV ( CCSV& csv ); 
	virtual void ConvertFromCSV ( CCSV& csv ); 
	
public:
	int GetMacroNo()					{ return m_nMacroNo; }
	int GetParentMacroTypeExternal();
	const char* GetParentMacroName()	{ return m_strParentMacroName; }
	const char* GetParentMacroData()	{ return m_strParentMacroData; }
	bool GetLocalMacroFlag()			{ return m_bLocalMacro; }
	const char* GetLocalMacroName()		{ return m_strLocalMacroName; }
	const char* GetLocalMacroData()		{ return m_strLocalMacroData; }
	
	void SetMacroNo( int n );
	void SetParentMacroTypeExternal( int n );
	void SetParentMacroName( const char* sz );
	void SetParentMacroData( const char* sz );
	void SetLocalMacroFlag( bool b ){ m_bLocalMacro = b; }
	void SetLocalMacroName( const char* sz );
	void SetLocalMacroData( const char* sz );

private:
	int GetParentMacroTypeInternal(){ return m_nParentMacroTypeInternal; }
	void SetParentMacroTypeInternal( int n );
	
public:
	void CheckMinimumSPOSVersion( CMinimumSPOSVersion& version );

private:
	void ClearRecord();
	void V1ConvertFromCSV( CCSV& csv );
	
private:
	int m_nMacroNo;
	int m_nParentMacroTypeInternal;
	CString m_strParentMacroName;
	CString m_strParentMacroData;
	bool m_bLocalMacro;
	CString m_strLocalMacroName;
	CString m_strLocalMacroData;
};

/**********************************************************************/

class CCascadingMacroCSVArray : public CSharedCSVArray
{
public:
	CCascadingMacroCSVArray();
	~CCascadingMacroCSVArray();

public:
	void Reset();

public:
	int Open( const char* szFilename, int nMode );

public:
	int GetMacroNo( int nMacroIdx );
	const char* GetMacroNameFromNumber( int nMacroNo );

public:
	bool FindMacroByNumber( int nMacroNo, int& nMacroIdx );
	int FindFirstFreeNumber();

public:
	void LoadParentMacrosSystem();
	void LoadParentMacrosDatabase( int nDbIdx );
	void LoadParentMacrosLocation( int nLocIdx );
	void LoadParentMacrosKeyboard();
	
private:
	void LoadParentMacros( CLocationSelectorEntity& LocSelEntity );

public:
	void PrepareForWrite();

private:
	CString m_strMacroName;
};

/**********************************************************************/
#endif
/**********************************************************************/
