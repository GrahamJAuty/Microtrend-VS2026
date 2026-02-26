#pragma once
//*******************************************************************
#include "resource.h"
//*******************************************************************
#define IMPORT_METHOD_ALL			0
#define IMPORT_METHOD_NEWONLY		1
#define IMPORT_METHOD_EXISTONLY		2
//*******************************************************************
#include "..\SPosLoyalty4Shared\SQLTable_Account\SQLRowAccountFull.h"
//*******************************************************************

class CImportDatabaseForManager
{
public:
	CImportDatabaseForManager ( const char* szFilename, CWnd* pParent = NULL );
	~CImportDatabaseForManager();

public:
	void SetBackgroundMode ( bool bMode ) { m_bBackgroundMode = bMode; }
	void SetFilename ( const char* szFilename ) { m_strFilename = szFilename; }
	bool IsValidFile();
	bool IsValidFileForUpgrade();
	void UserImport();													// requires user input
	bool DirectImport ( int nImportMethod, bool bForUpgrade = FALSE );		// used by auto background import

private:
	void DisplayMessage ( const char* szMsg, UINT uType );
	
private:
	CWnd* m_pParent;
	bool m_bBackgroundMode;

	CString m_strTitle;
	CString m_strFilename;

	int m_nIgnored;
	int m_nRead;
	int m_nUsed;
	int m_nInvalid;
	int m_nAdded;
	int m_nIndexCardNo;
};

//*******************************************************************
