#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#define TABLEAREA_VERSION 1
/**********************************************************************/

namespace SPosTableArea
{
	const recInt		AreaNo =				{ "AreaNo", 1, 99 };
	const recString		AreaName =				{ "AreaName", 0, 40 };
	const recBool		AutoTabsOnly =			{ "AutoTabsOnly" };
	const recBool		SuppressCovers =		{ "SuppressCovers" };
	const recInt		TableOpenMacro =		{ "TableOpenMacro" };
	const recInt		KPFlags =				{ "KPFlags", 0, 0xFFFF };
	const recInt		TableNoLo =				{ "TableNoLo", 1, 9999 };
	const recInt		TableNoHi =				{ "TableNoHi", 1, 9999 };
};

/**********************************************************************/

class CTableAreaCSVRecord : public CCSVRecord
{
public:
	CTableAreaCSVRecord(); 
	virtual ~CTableAreaCSVRecord() {}
	virtual void ConvertToCSV ( CCSV& csv ); 
	virtual void ConvertFromCSV ( CCSV& csv ); 
	
public:
	int GetAreaNo()						{ return m_nAreaNo; }
	const char* GetAreaName()			{ return m_strAreaName; }
	bool GetAutoTabsOnlyFlag()			{ return m_bAutoTabsOnly; }
	bool GetSuppressCoversFlag()		{ return m_bSuppressCovers; }
	int GetTableOpenMacro()				{ return m_nTableOpenMacro; }
	int GetKPFlags()					{ return m_nKPFlags; }
	int GetTableNoLo()					{ return m_nTableNoLo; }
	int GetTableNoHi()					{ return m_nTableNoHi; }
	
	void SetAreaNo( int n );
	void SetAreaName( const char* sz );
	void SetAutoTabsOnlyFlag( bool b ){ m_bAutoTabsOnly = b; }
	void SetSuppressCoversFlag( bool b ){ m_bSuppressCovers = b; }
	void SetTableOpenMacro( int n );
	void SetKPFlags( int n );
	void SetTableNoLo( int n );
	void SetTableNoHi( int n );

public:
	bool GetKPFlag( int n );
	void SetKPFlag( int n, bool b );

public:
	void CheckMinimumSPOSVersion( CMinimumSPOSVersion& version );

private:
	void ClearRecord();
	void V1ConvertFromCSV( CCSV& csv );
	
private:
	int m_nAreaNo;
	CString m_strAreaName;
	bool m_bAutoTabsOnly;
	bool m_bSuppressCovers;
	int m_nTableOpenMacro;
	int m_nKPFlags;
	int m_nTableNoHi;
	int m_nTableNoLo;
};

/**********************************************************************/

class CTableAreaCSVArray : public CSharedCSVArray
{
public:
	CTableAreaCSVArray();
	~CTableAreaCSVArray();

public:
	int Open( const char* szFilename, int nMode );

public:
	int GetAreaNo( int nITableIdx );

public:
	bool FindAreaByNumber( int nAreaNo, int& nAreaIdx );
	int FindFirstFreeNumber();
};

/**********************************************************************/
#endif
/**********************************************************************/
