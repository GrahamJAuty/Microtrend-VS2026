#pragma once
/**********************************************************************/
 
/**********************************************************************/
#include "ColumnManager.h"
#include "ColumnNames.h"
#include "Consolidation.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/
#define COLUMN_STATUS_HEIGHT 0
#define COLUMN_STATUS_LOCKED 1
#define COLUMN_STATUS_SHOWN 2
#define COLUMN_STATUS_HIDDEN 3
/**********************************************************************/

struct CColumnInfo
{
public:
	CColumnInfo();

public:
	int m_nType;
	CString m_strName;
	CString m_strHeaderText;
	int m_nColumnFormat;
};

/**********************************************************************/

struct CColumnNameMapInfo
{
public:
	CColumnNameMapInfo();
	void Reset();

public:
	int Compare ( CColumnNameMapInfo& source, int nHint = 0 );
	void Add	( CColumnNameMapInfo& source );
	
public:
	CString m_strName;
	int m_nIndex;
};

/**********************************************************************/

struct CColumnStatusInfo
{
public:
	CColumnStatusInfo();
	void Reset();

public:
	int Compare ( CColumnStatusInfo& source, int nHint = 0 );
	void Add	( CColumnStatusInfo& source );
	
public:
	int m_nStatus;
	bool m_bNotInFile;
	int m_nSortOrder;
	int m_nArrayIndex;
	int m_nWidth;
	int m_nDefaultWidth;

public:
	int m_nType;
	CString m_strName;
	CString m_strHeaderText;
	int m_nColumnFormat;
};

/**********************************************************************/

class CColumnManager
{
public:
	CColumnManager();
	
public:
	void Reset( TableName name, int nMinimumLineWidth = 400, int nHiresWidth = 780 );
	void AddColumn( const ColumnName& ColumnName, const char* szHeaderText, int nFormat, int nLores, int nHires, int nStatus = COLUMN_STATUS_LOCKED );
	void LoadSettings();
	void UpdateCurrentSettings();

public:
	int GetListColumnCount(){ return m_arrayStatusCurrent.GetSize(); }
	int GetListColumnStatus( int nPos );
	int GetListColumnType( int nPos );

public:
	void GetListColumnDetails( CArray<CColumnStatusInfo,CColumnStatusInfo>& arrayPos );
	void SetListColumnDetails( CArray<CColumnStatusInfo,CColumnStatusInfo>& arrayPos );

public:
	void RememberCurrentLine( CListCtrl* pList, CWnd* pWnd );
	void SaveCurrentLine( CListCtrl* pList, CWnd* pWnd );
	void RestoreDefaultLine();
	void RestoreStartOfSessionLine();
	
public:
	void RedrawList( CListCtrl* pList, CWnd* pWnd );

private:
	bool LoadSettingsInternal( const char* szFilename, const char* szType, bool bLegacy );
	
private:
	bool AllowWideColumns();

private:
	TableName m_TableName;
	int m_nMinimumLineWidth;
	int m_nHiresWidth;

private:
	CArray<CColumnInfo,CColumnInfo> m_arrayColumns;
	CReportConsolidationArray<CColumnStatusInfo> m_arrayStatusDefault;
	CReportConsolidationArray<CColumnStatusInfo> m_arrayStatusRestore;
	CReportConsolidationArray<CColumnStatusInfo> m_arrayStatusCurrent;
};

/**********************************************************************/
