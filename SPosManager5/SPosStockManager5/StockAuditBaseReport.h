#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
 
/**********************************************************************/
#include "Consolidation.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/
#define AUDIT_DELIVERY 1
#define AUDIT_DELIVERY_SUMMARY 2
#define AUDIT_ADJUSTMENT_STOCK 3
#define AUDIT_OPENING 4
#define AUDIT_STOCKTAKE 5
#define AUDIT_RETURNS 6
#define AUDIT_SALES 7
#define AUDIT_TRANSFER 8
#define AUDIT_CLOSING 9
#define AUDIT_ADJUSTMENT_PLU 10
#define AUDIT_ITEM_ACTIVITY 11
#define AUDIT_RETURNS_SUMMARY 12
/**********************************************************************/
#define AUDIT_EX_DELIVERY 101
#define AUDIT_EX_ADJUSTMENT_STOCK 103
#define AUDIT_EX_OPENING 104
#define AUDIT_EX_RETURNS 106
#define AUDIT_EX_SALES 107
#define AUDIT_EX_TRANSFER 108
#define AUDIT_EX_CLOSING 109
#define AUDIT_EX_ADJUSTMENT_PLU 110
#define AUDIT_EX_IMPORT 999
/**********************************************************************/
#define AUDIT_IMPORT_PLU_DELIVERY 201
#define AUDIT_IMPORT_PLU_ADJUSTMENT_STOCK 203
#define AUDIT_IMPORT_PLU_OPENING 204
#define AUDIT_IMPORT_PLU_RETURNS 206
#define AUDIT_IMPORT_PLU_SALES 207
#define AUDIT_IMPORT_PLU_TRANSFER 208
#define AUDIT_IMPORT_PLU_CLOSING 209
#define AUDIT_IMPORT_PLU_ADJUSTMENT_PLU 210
/**********************************************************************/
#define AUDIT_IMPORT_WRAPPER_DELIVERY 301
#define AUDIT_IMPORT_WRAPPER_OPENING 304
#define AUDIT_IMPORT_WRAPPER_RETURNS 306
#define AUDIT_IMPORT_WRAPPER_SALES 307
#define AUDIT_IMPORT_WRAPPER_TRANSFER 308
#define AUDIT_IMPORT_WRAPPER_CLOSING 309
/**********************************************************************/
#define AUDIT_RETRY_SALES 407
/**********************************************************************/
#define AUDIT_FAMILY_NORMAL 0
#define AUDIT_FAMILY_TEMP 1
#define AUDIT_FAMILY_TEMP2 2
#define AUDIT_FAMILY_PREV 3
#define AUDIT_FAMILY_TEMP_EX 4
#define AUDIT_FAMILY_TEMP2_EX 5
#define AUDIT_FAMILY_PREV_EX 6
#define AUDIT_FAMILY_IMPORT 7
#define AUDIT_FAMILY_RETRY 8
/**********************************************************************/
#define AUDIT_MODE_NORMAL 0
#define AUDIT_MODE_TEMP 1
#define AUDIT_MODE_TEMP2 2
#define AUDIT_MODE_PREVIEW 3
#define AUDIT_MODE_IMPORT 4
#define AUDIT_MODE_RETRY 5
/**********************************************************************/
#define ADD_STOCK_LEVELS 0
#define SUBTRACT_STOCK_LEVELS 1
#define OVERWRITE_STOCK_LEVELS 2
#define EDIT_STOCK_LEVELS 3
/**********************************************************************/

struct CStockAuditHeaderInfo
{
public:
	CStockAuditHeaderInfo();

public:
	COleDateTime m_date;
	CString m_strReference1;
	CString m_strReference2;
	CString m_strSuppName;
	CString m_strSpName1;
	CString m_strSpName2;
	CString m_strImportPath;
};

/**********************************************************************/

class CStockAuditBaseReport
{
public:
	CStockAuditBaseReport();
	void LockDatabase(){ m_bLockedDatabase = TRUE; }
	
public:
	bool MatchStockpoint ( int nSpIdx );
	const char* GetReportParamsFilename();
	const char* GetReportName();

public:
	virtual bool CreateReport( int nDbIdx );
	const char* GetHeaderLine( CStockAuditHeaderInfo& info );

public:
	void GetAuditFileList( int nSpIdx, CReportConsolidationArray<CSortedStringItem>& arrayFiles, __int64& nTotalFileSize );
	
public:
	virtual bool ProcessHeader( const char* szHeader );
	virtual bool ProcessLine( const char* szLine );
	const char* GetHeaderDate(){ return m_strHeaderDate; }
	const char* GetHeaderTime(){ return m_strHeaderTime; }
	
protected:
	virtual void SeparateStockpoints( CReportFile& ReportFile );
	virtual void SeparateStockpoints( CReportFile& ReportFile, int nSpIdx );
	virtual void AfterReport( CReportFile& ReportFile ){}

	virtual void AddColumns( CReportFile& ReportFile ){}
	virtual void WriteHeader( CReportFile& ReportFile ){}
	virtual void WriteLine( CReportFile& ReportFile, int nSpIdx ){}
	virtual void AppendTotals( CReportFile& ReportFile ){};
	virtual void AppendStockpointTotals( CReportFile& ReportFile ){};
	virtual void AppendGrandTotals( CReportFile& ReportFile ){};

protected:	
	const char* GetReportTitle();
	int GetAuditDataType();
	bool IsExceptionReport();
	bool IsImportReport();
	const char* GetDisplayStockCode ( const char* szCode );
	
public:
	void SetDbIdx( int nDbIdx ) { m_nDbIdx = nDbIdx; }
	void SetConLevel ( int nConLevel ) { m_nConLevel = nConLevel; }
	void SetTableNo ( int nTableNo ) { m_nTableNo = nTableNo; }
	void SetConLevelText ( const char* szConLevel ) { m_strConLevel = szConLevel; }

public:
	void SetReportType ( int nType ) { m_nReportType = nType; }
	void SetAuditMode ( int nMode ) { m_nAuditMode = nMode; }
	void SetHeaderReferenceFilter ( const char* szRef ) { m_strHeaderReferenceFilter = szRef; }
	void SetLineReferenceFilter ( const char* szRef ) { m_strLineReferenceFilter = szRef; }
	void SetAuditDateFrom ( COleDateTime date ) { m_AuditDateFrom = date; }
	void SetAuditDateTo ( COleDateTime date ) { m_AuditDateTo = date; }
	void SetStockCodeFrom( const char* sz ) { m_strStockCodeFrom = sz; }
	void SetStockCodeTo( const char* sz ) { m_strStockCodeTo = sz; }
	void SetPreviewHeader( const char* szHeader ) { m_strPreviewHeader = szHeader; }
	void SetWaitForDataFlag() { m_bWaitForData = TRUE; }	

protected:
	int CountStockpoints();
	void ShowStockpoints ( CReportFile& ReportFile );
		
protected:
	bool m_bProcessBlock;
	CString m_strPreviewHeader;
	
protected:
	int m_nReportType;
	int m_nAuditMode;
	int m_nDbIdx;
	int m_nConLevel;
	int m_nTableNo;
	CString m_strHeaderReferenceFilter;
	CString m_strLineReferenceFilter;
	COleDateTime m_AuditDateFrom;
	COleDateTime m_AuditDateTo;
	CString m_strStockCodeFrom;
	CString m_strStockCodeTo;
	bool m_bTransferIn;
	bool m_bLockedDatabase;
	
	bool m_bWaitForData;
	
protected:
	CString m_strHeaderLine;
	CString m_strHeaderDate;
	CString m_strHeaderTime;
	CString m_strHeaderReference1;
	CString m_strHeaderReference2;
	CString m_strHeaderSuppName;
	CString m_strHeaderSpName1;
	CString m_strHeaderSpName2;
	CString m_strHeaderImportPath;

protected:
	CString m_strConLevel;
	CString m_strReportName;

protected:
	CString m_strDisplayStockCode;
	CString m_strReportParamsFilename;
	CString m_strAuditFilename;
	CString m_strReportTitle;
};

/**********************************************************************/
#endif		//__STOCKAUDITBASEREPORT_H__
/**********************************************************************/
