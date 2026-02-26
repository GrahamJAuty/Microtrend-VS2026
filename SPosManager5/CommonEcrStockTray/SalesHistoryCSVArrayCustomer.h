#pragma once
/**********************************************************************/
 
#include "SalesHistoryFields.h"
/**********************************************************************/
#define SALESHISTORYCUSTOMER_VERSION 1
/**********************************************************************/

class CSalesHistoryCSVRecordCustomer : public CCSVRecord
{
public:
	CSalesHistoryCSVRecordCustomer(); 
	virtual ~CSalesHistoryCSVRecordCustomer() {}
	virtual void ConvertToCSV ( CCSV& csv ); 
	virtual void ConvertFromCSV ( CCSV& csv ); 
	void Reset() { ClearRecord(); }
	
	const char* GetActualName(){ return m_strName; }
	int GetReportType() { return m_nReportType; }
	void GetReportFields( CWordArray& arrayTemp );
	
	void SetName ( const char* szName ) { m_strName = szName; }
	void SetReportType ( int nType ) { m_nReportType = nType; }
	void SetReportFields( CWordArray& arrayTemp );

	bool CreateCopy( CSalesHistoryCSVRecordCustomer& copy ); 

	const char* GetFieldListName( int nField );
	const char* GetKey();

private:
	void ClearRecord();
	void V1ConvertFromCSV( CCSV& csv );
	
public:
	const char* GetListName(); 
	const char* GetReportName(); 
	bool CanBeCopied();
	void GetValidFieldList( CWordArray& array ); 
	
protected:
	CString m_strName;
	int m_nReportType;
	CWordArray m_arrayFields;
	
protected:
	CString m_strListName;
	CString m_strReportName;
	CString m_strFieldListName;
	
protected:
	CString m_strKey;
};

/**********************************************************************/

class CSalesHistoryCSVArrayCustomer : public CSharedCSVArray
{
public:
	CSalesHistoryCSVArrayCustomer();

public:
	int Open( const char* szFilename, int nMode );
	
public:
	const char* GetName ( int nIndex );
	int GetReportType( int nIndex );
	
	void LoadAdhocReport( CSalesHistoryCSVRecordCustomer& ReportRecord );
	void SaveAdhocReport( CSalesHistoryCSVRecordCustomer& ReportRecord );

	bool AllowReport( int nReportType );
	int GetCustomSalesType();
	void GetSystemReportList( CWordArray& array );

	void CopyFrom( CSalesHistoryCSVArrayCustomer& Source );

protected:	
	CString m_strName;
	CString m_strKey;

protected:
	CSalesHistoryCSVRecordCustomer m_adhocReport;
};

/**********************************************************************/
