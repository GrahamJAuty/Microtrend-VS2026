#pragma once
//*******************************************************************

class CUserIDRecord
{
public:
	CUserIDRecord(void);
	CString GetError();

public:
	void CopyFrom(CUserIDRecord& source);

public:
	int m_nMessageNo;
	bool m_bRecordExists;

	CString m_strUSID;
	CString m_strMFID;
	CString m_strMF2ID;
	int m_nGroupNo;
	CString m_strForename;
	CString m_strSurname;
	CString m_strUserName;
	double m_dPurse1;
	double m_dAvailableSpend;
	double m_dSpendLimit;
	double m_dPurse3;
	double m_dAvailableSpend3;
	double m_dSpendLimit3;
	CString m_strImage;
	int m_nPointsBalance;
	int m_nPointsTD;
	CSSDate m_dateLastUsed;
	CSSTime m_timeLastUsed;

public:
	void SetUserID ( const char* szUserID ) { m_strUserID = szUserID; }
	CString GetUserID() { return m_strUserID; }

	bool ExtractUserData ( const char* szLine );
	CString GetGroupNoString();

private:
	const bool MatchField(const CString& strField, const CString strPrefix, CString& strValue);
	void Clear();

private:
	CString m_strUserID;
	CString m_strInternalError;
	CString m_strServerError;
};

//*******************************************************************