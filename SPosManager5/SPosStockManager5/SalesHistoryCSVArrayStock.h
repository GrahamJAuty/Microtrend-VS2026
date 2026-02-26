#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
 
#include "SalesHistoryFields.h"
/**********************************************************************/
#define SALESHISTORYSTOCK_VERSION 3
/**********************************************************************/

class CSalesHistoryCSVRecordStock : public CCSVRecord
{
public:
	CSalesHistoryCSVRecordStock(); 
	virtual ~CSalesHistoryCSVRecordStock() {}
	virtual void ConvertToCSV ( CCSV& csv ); 
	virtual void ConvertFromCSV ( CCSV& csv ); 
	void Reset() { ClearRecord(); }

	const char* GetActualName(){ return m_strName; }
	int GetReportType() { return m_nReportType; }
	int GetReportMode();
	bool IsBottom100() { return m_bIsBottom100; }
	bool IsSortedByValue() { return m_bIsSortedByValue; }
	int GetChartType() { return m_nChartType; }
	int GetChartSize() { return m_nChartSize; }
	double GetChartCutOff() { return m_dChartCutOff; }
	void GetReportFields( CWordArray& arrayTemp );
	
	void SetName ( const char* szName ) { m_strName = szName; }
	void SetReportType ( int nType ) { m_nReportType = nType; }
	void SetReportMode ( int n );
	void SetBottom100Flag( bool bFlag ) { m_bIsBottom100 = bFlag; }
	void SetSortedByValueFlag( bool bFlag ) { m_bIsSortedByValue = bFlag; }
	void SetChartType( int nType );
	void SetChartSize( int nSize );
	void SetChartCutOff( double d ){ m_dChartCutOff = d; }
	void SetReportFields( CWordArray& arrayTemp );

	bool CreateCopy ( CSalesHistoryCSVRecordStock& copy );
	
	const char* GetFieldListName( int nField );
	const char* GetKey( bool bPercent, bool bProfit, bool bApparent );

private:
	void ClearRecord();
	void V1ConvertFromCSV( CCSV& csv );
	void V2to3ConvertFromCSV( CCSV& csv, int nVer );

public:
	const char* GetListName(); 
	const char* GetReportName(); 
	void GetValidFieldList( CWordArray& array ); 
	int GetBaseReportType();
	bool HasField( int nField );
	bool HasStockCodeField();
	bool CanDoEstimatedProfit();
	bool CanDoMonthlyBreakdown();
	bool CanDoCategorySort();
	bool CanDoPercentSale();
	bool CanDoGraph();
	bool CanBeCopied();
	bool CanDoApparent();

protected:
	CString m_strName;
	int m_nReportType;
	int m_nReportMode;
	bool m_bIsBottom100;
	bool m_bIsSortedByValue;
	int m_nChartType;
	int m_nChartSize;
	double m_dChartCutOff;
	CWordArray m_arrayFields;
	
protected:
	CString m_strListName;
	CString m_strReportName;
	CString m_strFieldListName;
	
protected:
	CString m_strKey;
};

/**********************************************************************/

class CSalesHistoryCSVArrayStock : public CSharedCSVArray
{
public:
	CSalesHistoryCSVArrayStock();

public:
	int Open( const char* szFilename, int nMode );
	void CopyFrom( CSalesHistoryCSVArrayStock& Source );

	const char* GetName ( int nIndex );
	int GetReportType( int nIndex );
	
	void LoadAdhocReport( CSalesHistoryCSVRecordStock& ReportRecord );
	void SaveAdhocReport( CSalesHistoryCSVRecordStock& ReportRecord );

	bool AllowReport( int nReportType );
	void GetSystemReportList( CWordArray& array );

protected:	
	CString m_strName;
	CString m_strKey;

protected:
	CSalesHistoryCSVRecordStock m_adhocReport;
};

/**********************************************************************/
#endif
/**********************************************************************/
