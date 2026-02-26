#pragma once
/**********************************************************************/
 
/**********************************************************************/
#include "Consolidation.h"
#include "ReportConsolidationArray.h"
#include "SalesHistoryFields.h"
/**********************************************************************/
#define SALESHISTORYPLU_VERSION 5
/**********************************************************************/

class CSalesHistoryCSVRecordPlu : public CCSVRecord
{
public:
	CSalesHistoryCSVRecordPlu(); 
	virtual ~CSalesHistoryCSVRecordPlu() {}
	virtual void ConvertToCSV ( CCSV& csv ); 
	virtual void ConvertFromCSV ( CCSV& csv ); 
	void Reset() { ClearRecord(); }
	
	const char* GetActualName(){ return m_strName; }
	int GetReportType() { return m_nReportType; }
	int GetReportMode();
	const char* GetEntityID();
	bool IsBottom100() { return m_bIsBottom100; }
	bool IsSortedByValue() { return m_bIsSortedByValue; }
	int GetChartType() { return m_nChartType; }
	int GetChartSize() { return m_nChartSize; }
	double GetChartCutOff() { return m_dChartCutOff; }
	const char* GetExportFilename() { return m_strExportFilename; }
	void GetReportFields( CWordArray& arrayTemp );
	
	void SetName ( const char* szName ) { m_strName = szName; }
	void SetReportType ( int nType ) { m_nReportType = nType; }
	void SetReportMode ( int n );
	void SetEntityID( const char* sz ){ m_strEntityID = sz; }
	void SetBottom100Flag( bool bFlag ) { m_bIsBottom100 = bFlag; }
	void SetSortedByValueFlag( bool bFlag ) { m_bIsSortedByValue = bFlag; }
	void SetChartType( int nType );
	void SetChartSize( int nSize );
	void SetChartCutOff( double d ){ m_dChartCutOff = d; }
	void SetExportFilename( const char* sz );
	void SetReportFields( CWordArray& arrayTemp );
	bool HasModifierField();
	bool HasPluNoField();
	
	bool CreateCopy( CSalesHistoryCSVRecordPlu& copy ); 

	const char* GetFieldListName( int nField );
	const char* GetKey( bool bPercent, bool bProfit );

	bool GetPreV3RecordFlag(){ return m_bPreV3Record; }

public:
	void GetDefaultExportFilename( CString& strExportFilename );
	void GetDefaultExportFilename( CReportConsolidationArray<CSortedStringItem>& arrayExportLabels, int& nExtraReportID, CString& strExportFilename );

private:
	bool HasField( int nField );

private:
	void ClearRecord();
	void V1ConvertFromCSV( CCSV& csv );
	void V2ConvertFromCSV( CCSV& csv );
	void V3to5ConvertFromCSV( CCSV& csv, int nVer );
	
public:
	const char* GetListName(); 
	const char* GetReportName(); 
	void GetValidFieldList( CWordArray& array ); 
	int GetBaseReportType(); 
	bool CanDoMonthlyBreakdown();
	bool CanDoDeptSort();
	bool CanDoEstimatedProfit();
	bool CanDoPercentSale();
	bool CanDoDateCompare();
	bool CanDoGraph();
	bool CanBeCopied();

protected:
	CString m_strName;
	int m_nReportType;
	int m_nReportMode;
	CString m_strEntityID;
	bool m_bIsBottom100;
	bool m_bIsSortedByValue;
	int m_nChartType;
	int m_nChartSize;
	double m_dChartCutOff;
	CString m_strExportFilename;
	CWordArray m_arrayFields;
	bool m_bPreV3Record;
	
protected:
	CString m_strListName;
	CString m_strReportName;
	CString m_strFieldListName;
	
protected:
	CString m_strKey;
};

/**********************************************************************/

class CSalesHistoryCSVArrayPlu : public CSharedCSVArray
{
public:
	CSalesHistoryCSVArrayPlu();

public:
	int Open( const char* szFilename, int nMode );
	
public:
	const char* GetName ( int nIndex );
	const char* GetEntityID( int nIndex );
	int GetReportType( int nIndex );
	
	void LoadAdhocReport( CSalesHistoryCSVRecordPlu& ReportRecord );
	void SaveAdhocReport( CSalesHistoryCSVRecordPlu& ReportRecord );

	bool AllowReport( int nReportType );
	void GetSystemReportList( CWordArray& array );

	void CopyFrom( CSalesHistoryCSVArrayPlu& Source );

protected:	
	CString m_strName;
	CString m_strKey;
	CString m_strEntityID;

protected:
	CSalesHistoryCSVRecordPlu m_adhocReport;
};

/**********************************************************************/
