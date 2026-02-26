#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "..\CommonEcrStock\PresetReportBase.h"
/**********************************************************************/
#define PRESET_STOCKAUDIT_VERSION 1
/**********************************************************************/

class CPresetReportInfoStockAudit
{
public:
	CPresetReportInfoStockAudit();
	void Reset();

public:
	void ToString( CString& str );
	void FromString( CString& str );

public:
	void ConvertFromCSV ( CCSV& csv, CPresetReportInfo& infoBase );
	void V1ConvertFromCSV ( CCSV& csv, CPresetReportInfo& infoBase );
	void ConvertToCSV ( CCSV& csv, CPresetReportInfo& infoBase );

public:
	int GetReportType(){ return m_nReportType; }
	const char* GetDateType(){ return m_strDateType; }
	const char* GetDateFrom(){ return m_strDateFrom; }
	const char* GetDateTo(){ return m_strDateTo; }
	
	bool GetStockRangeFlag() { return m_bStockRange; }
	const char* GetStockFrom() { return m_strStockFrom; }
	const char* GetStockTo() { return m_strStockTo; }
	bool GetOnePerPageFlag(){ return m_bOnePerPage; }
	const char* GetHeaderReference(){ return m_strHeaderReference; }
	const char* GetLineReference(){ return m_strLineReference; }
	
public:
	void SetReportType( int n ){ m_nReportType = n; }	
	void SetDateType( const char* sz ){ m_strDateType = sz; }
	void SetDateFrom( const char* sz ){ m_strDateFrom = sz; }
	void SetDateTo( const char* sz ){ m_strDateTo = sz; }

	void SetStockRangeFlag( bool b ){ m_bStockRange = b; }
	void SetStockFrom( const char* sz ){ m_strStockFrom = sz; }
	void SetStockTo( const char* sz ){ m_strStockTo = sz; }
	void SetOnePerPageFlag( bool b ){ m_bOnePerPage = b; }
	void SetHeaderReference( const char* sz ){ m_strHeaderReference = sz; }
	void SetLineReference( const char* sz ){ m_strLineReference = sz; }
	
private:
	void ClearRecord();
	void V1ConvertFromCSV( CCSV& csv );

private:	
	int m_nReportType;
	CString m_strDateType;
	CString m_strDateFrom;
	CString m_strDateTo;
	
	bool m_bStockRange;
	CString m_strStockFrom;
	CString m_strStockTo;
	bool m_bOnePerPage;
	CString m_strHeaderReference;
	CString m_strLineReference;
};

/**********************************************************************/

class CPresetReportArrayStockAudit : public CPresetReportArray
{
public:
	CPresetReportArrayStockAudit(void);
	~CPresetReportArrayStockAudit(void);

public:
	const char* GetFilename();

private:
	void PrepareForEditingInternal();
	void PrepareForWrite();
};

/**********************************************************************/
#endif
/**********************************************************************/
#endif
/**********************************************************************/
