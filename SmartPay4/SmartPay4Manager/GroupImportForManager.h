#pragma once
//*******************************************************************
#include "resource.h"
//*******************************************************************
#include "..\SmartPay4Shared\SQLTable_Group\SQLRowGroupFull.h"
//*******************************************************************

class CImportGroupForManager
{
public:
	CImportGroupForManager ( const char* szFilename, CWnd* pParent = NULL );
	~CImportGroupForManager();

public:
	void SetFilename ( const char* szFilename ) { m_strFilename = szFilename; }
	bool IsValidFileForUpgrade();
	bool DirectImport ( bool bSilent = FALSE );		// used by auto background import

private:
	bool ImportLine ( CArray<int, int>& arrayHeader, const char* szImportLine );
	void ImportFields (CArray<int, int>& arrayHeader, CCSV* pCsvData, CSQLRowGroupFull& RowGroup );
	
private:
	CWnd* m_pParent;
	
	CString m_strTitle;
	CString m_strFilename;

	int m_nIgnored;
	int m_nRead;
	int m_nUsed;
	int m_nInvalid;
	int m_nAdded;
	int m_nIndexGroupNo;
};

//*******************************************************************
