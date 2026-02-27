#pragma once

#define GROUPSET_MAX		9

class CGroupSetText : public CSharedStringArray
{
public:
	CGroupSetText() {}
	int Open ( const char* szFilename, int nMode = DB_READONLY );
//	int GetCount();

	CString GetText ( int nSetNo );
	void SetText ( int nSetNo, const char* text );
};

