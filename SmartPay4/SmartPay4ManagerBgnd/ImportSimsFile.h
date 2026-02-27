#pragma once
//******************************************************************
#include "..\SmartPay4Shared\SQLTable_Account\SQLRowAccountFull.h"
//******************************************************************
#include "ImportFlag.h"
#include "SimsData.h"
#include "SimsFile.h"
//******************************************************************

class CImportSimsFile
{

public:
	CImportSimsFile ( CSimsData* pSims, CWnd* pParent );

//******************************************************************
#ifdef SYSTEMTYPE_BACKGROUND
//******************************************************************

private:
	void ShowStatistics(){}
	bool ShowImportLine ( const char* szCardNo ){ return TRUE; }
	CString GetProgressTitle(){ return ""; }
	
//******************************************************************
#else
//******************************************************************
	
public:
	bool ShowImports ( const char* szFilename );

private:
	bool ShowImportLine ( const char* szCardNo );
	CString GetProgressTitle();
	void AddColumnText();
	void AddStatistics();
	void ShowStatistics();

private:
	CWnd* m_pParent;
	CReportFileWithEndText m_Report;
	CString m_strReportTitle;
	
//******************************************************************
#endif
//******************************************************************

private:
	void Init();

public:
	~CImportSimsFile();
	CString GetError() { return m_strError; }
	int GetUsedCount() { return m_nUsed; }

	void SetImportMethod ( int n ) { m_nImportMethod = n; }
	bool Import ( const char* szFilename );
	
private:
	void GetNewCardNo ( CString& strCardNo );
	bool MatchRecord(CSQLRowAccountFull& RowAccount);

	int ProcessFile();
	void SetRecord(CSQLRowAccountFull& RowAccount);

	void ImportLine ( const char* szCardNo );
	
private:
	CSimsData* m_pSims;

	int m_nCurrentRow;
	
	CSimsFile m_simsFile;
	
	CString m_strStartFromUserID;
	
	int m_nImportMethod;
	int m_nRead;
	int m_nIgnored;
	int m_nUsed;
	int m_nAdded;
	int m_nSkipped;

	CString m_strError;
	CString m_strImportFilename;
	CString m_strExceptionComment;

	CImportFlag m_ImportFlag;
};

//******************************************************************
