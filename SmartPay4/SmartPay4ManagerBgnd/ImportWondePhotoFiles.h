#pragma once
/**********************************************************************/
#include "ImportFlag.h"
#include "WondeData.h"
/**********************************************************************/

class CImportWondePhotoFiles
{
public:
	CImportWondePhotoFiles ( CWondeData* pData, CWnd* pParent );

//******************************************************************
#ifdef SYSTEMTYPE_BACKGROUND
//******************************************************************

public:
	void ShowImports(){}

private:
	bool ShowImportLine( const char* szPhotoRef, const char* szCardNo, const char* szPhotoPathname, bool bReplacement ){ return TRUE; }
	void ShowStatistics(){};
	
//******************************************************************
#else
//******************************************************************

public:
	void ShowImports();

private:
	void AddColumnText();
	void AddStatistics();
	bool ShowImportLine ( const char* szPhotoRef, const char* szCardNo, const char* szPhotoPathname, bool bReplacement );
	void ShowStatistics();

private:
	CReportFileWithEndText m_Report;
	CString m_strReportTitle;

//******************************************************************
#endif
//******************************************************************

private:
	void Init();

public:
	~CImportWondePhotoFiles();
	int Import();

private:
	void ProcessPhotos();
	
private:
	CWondeData* m_pData;
	CWnd* m_pParent;
	int m_nCurrentRow;

	int m_nRead;
	int m_nIgnored;
	int m_nUsed;
	int m_nInvalid;
	int m_nAdded;

	CImportFlag m_ImportFlag;
};

/**********************************************************************/
