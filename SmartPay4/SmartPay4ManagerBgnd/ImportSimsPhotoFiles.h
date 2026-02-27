#pragma once
//********************************************************************
#include "ImportFlag.h"
#include "SimsData.h"
//********************************************************************

class CImportSimsPhotoFiles
{

public:
	CImportSimsPhotoFiles ( CSimsData* pSims, CWnd* pParent );

//******************************************************************
#ifdef SYSTEMTYPE_BACKGROUND
//******************************************************************

private:
	void ShowStatistics(){}
	bool ShowImportLine ( const char* szPhotoRef, const char* szCardNo, const char* szPhotoPathname, bool bReplacement ){ return TRUE; }

//******************************************************************
#else
//******************************************************************

public:
	bool ShowImports();
	
private:
	bool ShowImportLine ( const char* szPhotoRef, const char* szCardNo, const char* szPhotoPathname, bool bReplacement );
	void AddColumnText();
	void AddStatistics();
	void ShowStatistics();
	
private:
	CReportFileWithEndText m_Report;
	CString m_strReportTitle;
	
//******************************************************************
#endif
//******************************************************************

private:
	void Init();	
	int ProcessPhotos();

public:
	~CImportSimsPhotoFiles();
	bool Import();
	int GetUsedCount() { return m_nUsed; }
	CString GetError() { return m_strError; }
	
private:
	CSimsData* m_pSims;
	int m_nCurrentRow;
	
	CWnd* m_pParent;
	
	int m_nRead;
	int m_nIgnored;
	int m_nUsed;
	int m_nInvalid;
	int m_nAdded;

	CString m_strError;

	CImportFlag m_ImportFlag;
};

//******************************************************************
