//$$******************************************************************
//$$** exact same file used by LoyaltyManager / LoyaltyServer ********
//$$******************************************************************
#pragma once
//$$******************************************************************
#define ALERT_MAX 9
//$$******************************************************************

struct CAlertTextInfo
{
public:
	CAlertTextInfo();

public:
	void Reset();

public:
	CString m_strText;
	bool m_bEPOS;
};

//$$******************************************************************

class CAlertText_base : public CSharedStringArray
{
public:
	CAlertText_base() {}
	int Open(const char* szFilename, int nMode = DB_READONLY);
	
public:
	void GetInfo(int nAlertCode, CAlertTextInfo& info);
	void SetInfo(int nAlertCode, CAlertTextInfo& info);

public:
	int GetAlertCount();
	CString GetDisplayText(CString strAlertCodes);
	CString GetClearText();
};

//$$******************************************************************
