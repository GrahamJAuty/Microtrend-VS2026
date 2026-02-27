#pragma once
/**********************************************************************/
#include "..\SmartPay4Shared\SQLTable_Account\SQLRowAccountFull.h"
/**********************************************************************/
#include "ImportFlag.h"
#include "WondeData.h"
#include "WondeDatabase.h"
//********************************************************************

class CImportWondeFile
{
public:
	CImportWondeFile ( CWondeData* pData, CWnd* pParent );
	
//******************************************************************
#ifdef SYSTEMTYPE_BACKGROUND
//******************************************************************

private:
	CString GetProgressTitle(){ return ""; }
	bool ShowImportLine ( const char* szUserID ){ return TRUE; }
	void ShowStatistics(){}

//******************************************************************
#else
//******************************************************************

public:
	CString GetProgressTitle();
	bool ShowImports ( const char* szFilename );

private:
	void AddReportField ( CCSV* pCsv, const char* szData );
	bool ShowImportLine ( const char* szUserID );

	void AddColumnText();
	void AddStatistics();
	void ShowStatistics();

private:
	CReportFileWithEndText m_Report;
	CString m_strReportTitle;
	
//******************************************************************
#endif
//******************************************************************

public:
	void Init();
	
public:
	~CImportWondeFile();
	CString GetError() { return m_strError; }
	int GetUsedCount() { return m_nUsed; }

	void SetImportMethod ( int n ) { m_nImportMethod = n; }
	bool Import ( const char* szFilename );

private:
	int ProcessFile();
	void SetRecord (CSQLRowAccountFull& RowAccount, bool bNewRecord );

	bool ConvertJson ( const char* szJsonFilename, const char* strWondeDatabase );

	void ImportLine ( const char* szUserID );
	void GetNewCardNo( CString& strCardNo );

private:
	CWondeData* m_pData;
	CWnd* m_pParent;
	CString m_strReportLabel;
	CString m_strExceptionComment;

	int m_nCurrentRow;

	CWondeDatabase m_wondeDbase;
	
	bool m_bMembershipUpdated;

	int m_nImportMethod;
	int m_nRead;
	int m_nIgnored;
	int m_nUsed;
	int m_nAdded;
	int m_nSkipped;

	CString m_strPupilStartFrom;
	CString m_strStaffStartFrom;

	CString m_strError;	
	CString m_strImportFilename;

	CImportFlag m_ImportFlag;
};

//******************************************************************
