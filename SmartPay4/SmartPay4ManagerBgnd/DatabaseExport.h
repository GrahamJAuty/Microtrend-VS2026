#pragma once
//**********************************************************************
#include "..\SmartPay4Shared\SQLTable_Account\SQLRowAccountFull.h"
//**********************************************************************

class CExportDatabase
{
public:
	CExportDatabase ( const char* szLabel, CWnd* pParent );
	
//******************************************************************
#ifdef SYSTEMTYPE_BACKGROUND
//******************************************************************

public:
	void Execute ( bool bShowProgress ){}

private:
	void ShowUserExportStats ( CWnd* pParent ){}

//******************************************************************
#else
//******************************************************************

public:
	void Execute ( bool bShowProgress );

private:
	void ShowUserExportStats ( CWnd* pParent );

//******************************************************************
#endif
//******************************************************************
	
public:
	int ExportRecord(CSQLRowAccountFull& RowAccount);

private:
	int CreateFile ( bool bShowProgress );

private:
	CString m_strLabel;
	CWnd* m_pParent;

	int m_nExportCount;
	CString m_strExportFilename;
	CString m_strTitle;
};

//**********************************************************************
