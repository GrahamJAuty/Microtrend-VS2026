#pragma once
//*******************************************************************
#include "resource.h"
//*******************************************************************

class CTLogDatabase : public CSSUnsortedDatabase
{
public:
	CTLogDatabase(void);
	int Open ( const char* szFilename );

	bool Find (  const char* szDate, const char* szTime, int nCCNo );
	CString GetSalesText() { return m_strSalesText; }

private:
	CString MakeKey ( const char* szDate, const char* szTime, int nCCNo );

private:
	CString m_strSalesText;
};

//**********************************************************************
