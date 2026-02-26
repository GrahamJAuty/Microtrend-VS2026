#pragma once
//*******************************************************************
#include "resource.h"
//*******************************************************************
#include "..\SPosLoyalty4Shared\SQLTable_Account\SQLRowAccountFull.h"
//*******************************************************************

class CExportDatabase
{
public:
	CExportDatabase ( const char* szLabel, CWnd* pParent );
	void Execute ( bool bShowProgress );
	int ExportRecord( CSQLRowAccountFull& RowAccount);

private:
	int CreateFile ( bool bShowProgress, int& nExportCount );

private:
	CString m_strLabel;
	CWnd* m_pParent;

private:
	int m_nErrorNo;
	CString m_strExportFilename;
	CString m_strTitle;
};

//*******************************************************************
