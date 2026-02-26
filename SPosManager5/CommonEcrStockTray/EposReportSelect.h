#pragma once
/**********************************************************************/
#include "Consolidation.h"
#include "PosTrayTask.h"
#include "PosTrayEposConsolParamsInfo.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/
 
/**********************************************************************/
#define EPOS_CUSTOM_FAMILY_NONE 0
#define EPOS_CUSTOM_FAMILY_GENERAL 1
#define EPOS_CUSTOM_FAMILY_TRANSACTION 2
#define EPOS_CUSTOM_FAMILY_DEPOSIT 3
#define EPOS_CUSTOM_FAMILY_MIXMATCH 4
#define EPOS_CUSTOM_FAMILY_DISCOUNT 5
#define EPOS_CUSTOM_FAMILY_HOURLY 6
#define EPOS_CUSTOM_FAMILY_COVERS_HOURLY 7
#define EPOS_CUSTOM_FAMILY_COVERS_HOURLYGRP 8
#define EPOS_CUSTOM_FAMILY_HOURLYGRP 9
#define EPOS_CUSTOM_FAMILY_COVERS_HOURLYDPT 10
#define EPOS_CUSTOM_FAMILY_HOURLYDPT 11
#define EPOS_CUSTOM_FAMILY_LOYALTYRECON 12
#define EPOS_CUSTOM_FAMILY_PAYMENTDETAIL 13
#define EPOS_CUSTOM_FAMILY_PLU_PRICEBAND 14
/**********************************************************************/
#define EPOS_CUSTOM_FIRST_REPORT 1000
/**********************************************************************/
#define EPOS_CUSTOM_MAXLEN_NAME 50
#define EPOS_CUSTOM_MAXLEN_PAYNOFILTER 200
/**********************************************************************/
#define EPOS_CUSTOM_MAXLEN_PARAMS 100
/**********************************************************************/
#define EPOS_MAXLEN_CONSOL_PREFS 50
/**********************************************************************/
#define EPOS_MAXLEN_EXPORT_FILENAME 50
/**********************************************************************/

struct CEposReportSelectInfo
{
public:
	CEposReportSelectInfo();
	void Reset();

public:
	int GetReportType(){ return m_nReportType; }
	bool GetDefaultSelect(){ return m_bDefaultSelect; }
	bool GetCurrentSelect(){ return m_bCurrentSelect; }
	int GetDefaultSortOrder(){ return m_nDefaultSortOrder; }
	int GetCurrentSortOrder(){ return m_nCurrentSortOrder; }
	const char* GetExportFilename(){ return m_strExportFilename; }
	const char* GetConsolPrefsDefault(){ return m_strConsolPrefsDefault; }
	const char* GetConsolPrefsSaved(){ return m_strConsolPrefsSaved; }
	const char* GetConsolPrefsAdhoc(){ return m_strConsolPrefsAdhoc; }
	const char* GetConsolPrefsSummary(){ return m_strConsolPrefsSummary; }
	bool GetSystemEnableFlag(){ return m_bSystemEnable; }
	
public:
	int GetCustomReportSerialNo(){ return m_nCustomReportSerialNo; }
	int GetCustomReportFamily(){ return m_nCustomReportFamily; }
	const char* GetCustomReportName(){ return m_strCustomReportName; }
	const char* GetCustomReportParams(){ return m_strCustomReportParams; }
	
public:
	void SetReportType( int n );
	void SetDefaultSelect( bool b ){ m_bDefaultSelect = b; }
	void SetCurrentSelect( bool b ){ m_bCurrentSelect = b; }
	void SetDefaultSortOrder( int n );
	void SetCurrentSortOrder( int n );
	void SetExportFilename( const char* sz );
	void SetConsolPrefsDefault( const char* sz );
	void SetConsolPrefsSaved( const char* sz );
	void SetConsolPrefsAdhoc( const char* sz );
	void SetConsolPrefsSummary( const char* sz );
	void SetSystemEnableFlag( bool b ){ m_bSystemEnable = b; }

public:
	void SetCustomReportSerialNo( int n ){ m_nCustomReportSerialNo = n; }
	void SetCustomReportFamily( int n );
	void SetCustomReportName( const char* sz );
	void SetCustomReportParams( const char* sz );

public:
	bool LoadConsolPrefs();
	bool SaveConsolPrefs();
	void InitialiseCustomReportConsolPrefs();
	void LoadPosTrayConsolPrefs( CPosTrayEposConsolParamsInfo& infoParams, CPosTrayTask& Task );

public:
	int Compare ( CEposReportSelectInfo& source, int nHint = 0 );
	void Add	( CEposReportSelectInfo& source );
		
private:
	int m_nReportType;
	bool m_bDefaultSelect;
	bool m_bCurrentSelect;
	int m_nDefaultSortOrder;
	int m_nCurrentSortOrder;
	CString m_strExportFilename;
	CString m_strConsolPrefsDefault;
	CString m_strConsolPrefsSaved;
	CString m_strConsolPrefsAdhoc;
	CString m_strConsolPrefsSummary;
	bool m_bSystemEnable;
	
private:
	int m_nCustomReportSerialNo;
	int m_nCustomReportFamily;
	CString m_strCustomReportName;
	CString m_strCustomReportParams;
};

/**********************************************************************/

struct CEposReportSortedInfo
{
public:
	CEposReportSortedInfo();
	void Reset();

public:
	int Compare ( CEposReportSortedInfo& source, int nHint = 0 );
	void Add	( CEposReportSortedInfo& source );

public:
	int m_nCurrentSortOrder;
	int m_nDefaultSortOrder;
	int m_nReportType;
	int m_nMoveCount;
};

/**********************************************************************/

class CEposReportSelect
{
public:
	CEposReportSelect(void);

private:
	void AddDefaultReport( int nReportType, const char* szDefaultPrefs, const char* szSummaryPrefs, bool bDefaultSelect, bool bSystemEnable );

public:
	void LoadReportSelection();
	void SaveReportSelection();
	
public:
	int GetSelectedReportCount();
	int GetSelectedReportType( int nPos );
	int GetAvailableReportCount();
	int GetAvailableReportType( int nPos );
	void UpdateReportStatus( CArray<CSortedIntWithFlag,CSortedIntWithFlag>& arraySelected, CArray<CSortedIntWithFlag,CSortedIntWithFlag>& arrayAvailable );

public:
	bool AddCustomReport( CEposReportSelectInfo& infoReport, int nFamily );
	void RemoveCustomReport( CEposReportSelectInfo& infoReport );
	const char* GetCustomReportName( int nReportType );
	int GetCustomReportFamily(int nReportType);


public:
	bool GetReportInfo( CEposReportSelectInfo& infoReport );
	bool SetReportInfo( CEposReportSelectInfo& infoReport );
	
public:
	void ApplyDefaultSortOrder( CArray<CSortedIntWithFlag,CSortedIntWithFlag>& arrayToSort );

public:
	static void GetDefaultExportFilename( int nReportType, CString& strName );

private:
	void BuildExternalReportLists();

private:
	CReportConsolidationArray<CEposReportSelectInfo> m_arrayReports;
	CArray<int,int> m_arraySelected;
	CArray<int,int> m_arrayAvailable;

private:
	//HELPERS WHEN BUILDING THE REPORT ARRAY
	int m_nLastReportType;
	int m_nLastReportPos;

private:
	bool m_bExternalListsValid;

private:
	CString m_strCustomReportName;
	int m_nNextCustomReportSerialNo;
};

/**********************************************************************/
