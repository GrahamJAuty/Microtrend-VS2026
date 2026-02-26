#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "..\CommonEcrStock\PresetReportBase.h"
/**********************************************************************/
#define PRESET_EPOS_VERSION 3
/**********************************************************************/

class CPresetReportInfoEpos
{
public:
	CPresetReportInfoEpos();
	void Reset();

public:
	void ToString( CString& str );
	void FromString( CString& str );

public:
	void ConvertFromCSV ( CCSV& csv, CPresetReportInfo& infoBase );
	void V1To3ConvertFromCSV ( CCSV& csv, CPresetReportInfo& infoBase, int nVer );
	void ConvertToCSV ( CCSV& csv, CPresetReportInfo& infoBase );

public:
	int GetBatchNo(){ return m_nBatchNo; }
	int GetReportType(){ return m_nReportType; }
	
	const char* GetDateType(){ return m_strDateType; }
	const char* GetDateFrom(){ return m_strDateFrom; }
	const char* GetDateTo(){ return m_strDateTo; }
	const char* GetSessionName(){ return m_strSessionName; }
	int GetPluTimeType(){ return m_nPluTimeType; }

	int GetPMSConsolidateType(){ return m_nPMSConsolidateType; }
	bool GetPMSPendingFlag(){ return m_bPMSPending; }
	bool GetPMSActiveFlag(){ return m_bPMSActive; }
	bool GetPMSCompleteFlag(){ return m_bPMSComplete; }
	bool GetPMSCancelledFlag(){ return m_bPMSCancelled; }

	int GetBlockType(){ return m_nBlockType; }

	int GetDepartmentFilter(){ return m_nDepartmentFilter; }
	int GetPluFilterNo(){ return m_nPluFilterNo; }
	bool GetItemServerFlag(){ return m_bItemServer; }
	
	int GetTermDateType() { return m_nTermDateType; }

public:
	void SetBatchNo( int n );
	void SetReportType( int n ){ m_nReportType = n; }

	void SetDateType( const char* sz ){ m_strDateType = sz; }
	void SetDateFrom( const char* sz ){ m_strDateFrom = sz; }
	void SetDateTo( const char* sz ){ m_strDateTo = sz; }
	void SetSessionName( const char* sz ){ m_strSessionName = sz; }
	void SetPluTimeType( int n ){ m_nPluTimeType = n; }

	void SetPMSConsolidateType( int n ){ m_nPMSConsolidateType = n; }
	void SetPMSPendingFlag( bool b ){ m_bPMSPending = b; }
	void SetPMSActiveFlag( bool b ){ m_bPMSActive = b; }
	void SetPMSCompleteFlag( bool b ){ m_bPMSComplete = b; }
	void SetPMSCancelledFlag( bool b ){ m_bPMSCancelled = b; }

	void SetBlockType( int n ){ m_nBlockType = n; }

	void SetDepartmentFilter( int n ){ m_nDepartmentFilter = n; }
	void SetPluFilterNo( int n ){ m_nPluFilterNo = n; }
	void SetItemServerFlag( bool b ){ m_bItemServer = b; }
		
	void SetTermDateType(int n) { m_nTermDateType = n;  }

private:
	int m_nBatchNo;
	int m_nReportType;
	
	CString m_strDateType;
	CString m_strDateFrom;
	CString m_strDateTo;
	CString m_strSessionName;
	int m_nPluTimeType;

	int m_nPMSConsolidateType;
	bool m_bPMSPending;
	bool m_bPMSActive;
	bool m_bPMSComplete;
	bool m_bPMSCancelled;

	int m_nBlockType;

	int m_nDepartmentFilter;
	int m_nPluFilterNo;
	bool m_bItemServer;

	int m_nTermDateType;
};

/**********************************************************************/

class CPresetReportArrayEpos : public CPresetReportArray
{
public:
	CPresetReportArrayEpos(void);
	~CPresetReportArrayEpos(void);

public:
	const char* GetFilename();

private:
	void PrepareForEditingInternal();
	void PrepareForWrite();
};

/**********************************************************************/
#endif
/**********************************************************************/
