#pragma once
//*******************************************************************
#define ALERT_MAX		9
//*******************************************************************

class CAlertText : public CSharedStringArray
{
public:
	CAlertText() {}
	int Open ( const char* szFilename, int nMode = DB_READONLY );
	int GetAlertCount();

	CString GetText ( int nAlertCode );
	void SetText ( int InhibitCode, const char* text );

	CString GetHotlistText ( int nAlertCode );
};

//*******************************************************************
