#pragma once
//$$******************************************************************
#define USERTYPE_MAX 9
//$$******************************************************************

class CUserTypeText : public CSharedStringArray
{
public:
	CUserTypeText(){}
	int Open ( const char* szFilename, int nMode = DB_READONLY );
	
public:
	int GetCount();
	CString GetText ( int nTypeNo );
	void SetText ( int nTypeNo, const char* text );
};

//$$******************************************************************