#pragma once
/**********************************************************************/
 
/**********************************************************************/
#define TABLENAME_VERSION 1
/**********************************************************************/

namespace SPosTableName
{
	const recInt		TableNo =						{ "TableNo", 1, 999 };
	const recString		TableName =						{ "TableName", 0, 40 };
	const recBool		AccountTable =					{ "AccountTable" };
	const recBool		EnableLimit =					{ "EnableLimit" };
	const recInt		LimitPounds =					{ "LimitPounds", 0, 9999 };
	const recBool		AutoStatusEnable =				{ "AutoStatusEnable" };
	const recBool		AutoStatusUnusedAfterPayment =	{ "AutoStatusUnusedAfterPayment" };
	const recBool		ShowAsFilter =					{ "ShowAsFilter" };
	const recInt		ReportGroup =					{ "ReportGroup", 1, 20 };
};

/**********************************************************************/

class CTableNameCSVRecord : public CCSVRecord
{
public:
	CTableNameCSVRecord(); 
	virtual ~CTableNameCSVRecord() {}
	virtual void ConvertToCSV ( CCSV& csv ); 
	virtual void ConvertFromCSV ( CCSV& csv ); 
	
public:
	int GetTableNo()							{ return m_nTableNo; }
	const char* GetTableName()					{ return m_strTableName; }
	bool GetShowAsFilterFlag()					{ return m_bShowAsFilter; }
	bool GetAccountTableFlag()					{ return m_bAccountTable; }
	bool GetEnableLimitFlag()					{ return m_bEnableLimit; }
	int GetLimitPounds()						{ return m_nLimitPounds; }
	bool GetAutoStatusEnableFlag()				{ return m_bAutoStatusEnable; }
	bool GetAutoStatusUnusedAfterPaymentFlag()	{ return m_bAutoStatusUnusedAfterPayment; }
	int GetReportGroup( int n );
	
	void SetTableNo( int n );
	void SetTableName( const char* sz );
	void SetShowAsFilterFlag( bool b )					{ m_bShowAsFilter = b; }
	void SetAccountTableFlag( bool b )					{ m_bAccountTable = b; }
	void SetEnableLimitFlag( bool b )					{ m_bEnableLimit = b; }
	void SetLimitPounds( int n );
	void SetAutoStatusEnableFlag( bool b )				{ m_bAutoStatusEnable = b; }
	void SetAutoStatusUnusedAfterPaymentFlag( bool b )	{ m_bAutoStatusUnusedAfterPayment = b; }
	void SetReportGroup( int n, int nGrp );

public:
	void CheckMinimumSPOSVersion( CMinimumSPOSVersion& version );

private:
	void ClearRecord();
	void V1ConvertFromCSV( CCSV& csv );
	
private:
	int m_nTableNo;
	CString m_strTableName;
	bool m_bShowAsFilter;
	bool m_bAccountTable;
	bool m_bEnableLimit;
	int m_nLimitPounds;
	bool m_bAutoStatusEnable;
	bool m_bAutoStatusUnusedAfterPayment;
	int m_nReportGroup[4];
};

/**********************************************************************/

class CTableNameCSVArray : public CSharedCSVArray
{
public:
	CTableNameCSVArray();
	~CTableNameCSVArray();

public:
	int Open( const char* szFilename, int nMode );

public:
	int GetTableNo( int nITableIdx );

public:
	bool FindTableByNumber( int nTableNo, int& nTableIdx );
	int FindFirstFreeNumber();
};

/**********************************************************************/
