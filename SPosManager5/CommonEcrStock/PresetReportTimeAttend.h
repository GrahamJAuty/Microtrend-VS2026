#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "..\CommonEcrStock\PresetReportBase.h"
/**********************************************************************/
#define PRESET_TIMEATTEND_VERSION 3
/**********************************************************************/

class CPresetReportInfoTimeAttend
{
public:
	CPresetReportInfoTimeAttend();
	void Reset();

public:
	void ToString( CString& str );
	void FromString( CString& str );

public:
	void ConvertFromCSV ( CCSV& csv, CPresetReportInfo& infoBase );
	void V1To3ConvertFromCSV ( CCSV& csv, CPresetReportInfo& infoBase, int nVer );
	void ConvertToCSV ( CCSV& csv, CPresetReportInfo& infoBase );

public:
	int GetReportType(){ return m_nReportType; }
	const char* GetDateType(){ return m_strDateType; }
	const char* GetDateFrom(){ return m_strDateFrom; }
	const char* GetDateTo(){ return m_strDateTo; }
	int GetRoundToType() { return m_nRoundToType; }
	const char* GetSessionName(){ return m_strSessionName; }
	int GetGroupBy(){ return m_nGroupBy; }
	int GetEditMode(){ return m_nEditMode; }
	int GetServerPickType(){ return m_nServerPickType; }
	int GetServerStart(){ return m_nServerStart; }
	int GetServerEnd(){ return m_nServerEnd; }
	bool GetClosingTimeFlag(){ return m_bClosingTime; }
	bool GetOpenShiftFlag() { return m_bOpenShift; }
	bool GetEventsBySecondFlag(){ return m_bEventsBySecond; }

	void SetReportType( int n ){ m_nReportType = n; }	
	void SetDateType( const char* sz ){ m_strDateType = sz; }
	void SetDateFrom( const char* sz ){ m_strDateFrom = sz; }
	void SetDateTo( const char* sz ){ m_strDateTo = sz; }
	void SetRoundToType( int n ){ m_nRoundToType = n; }
	void SetSessionName( const char* sz ){ m_strSessionName = sz; }
	void SetGroupBy( int n ){ m_nGroupBy = n; }
	void SetEditMode( int n ){ m_nEditMode = n; }
	void SetServerPickType( int n ){ m_nServerPickType = n; }
	void SetServerStart( int n ){ m_nServerStart = n; }
	void SetServerEnd( int n ){ m_nServerEnd = n; }
	void SetClosingTimeFlag( bool b ){ m_bClosingTime = b; }
	void SetOpenShiftFlag(bool b) { m_bOpenShift = b; }
	void SetEventsBySecondFlag( bool b ){ m_bEventsBySecond = b; }

private:	
	int m_nReportType;
	CString m_strDateType;
	CString m_strDateFrom;
	CString m_strDateTo;
	int m_nRoundToType;
	CString m_strSessionName;
	int m_nGroupBy;
	int m_nEditMode;
	int m_nServerPickType;
	int m_nServerStart;
	int m_nServerEnd;
	bool m_bClosingTime;
	bool m_bOpenShift;
	bool m_bEventsBySecond;
};

/**********************************************************************/

class CPresetReportArrayTimeAttend : public CPresetReportArray
{
public:
	CPresetReportArrayTimeAttend(void);
	~CPresetReportArrayTimeAttend(void);

public:
	const char* GetFilename();

private:
	void PrepareForEditingInternal();
	void PrepareForWrite();
};

/**********************************************************************/
#endif
/**********************************************************************/
