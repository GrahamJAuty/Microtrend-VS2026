#pragma once
//$$******************************************************************

class CImportLeaversList
{
public:
	CImportLeaversList( CWnd* pParent );
	~CImportLeaversList(void);

public:
	void Import();

private:
	int ProcessFile ( const char* szImportFile, const char* szLeaversYear, const char* szAuditComment );

private:
	CWnd* m_pParent;
	CString m_strError;
	CString m_strTitle;
};

//$$******************************************************************
