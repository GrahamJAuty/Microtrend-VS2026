#pragma once
/**********************************************************************/

namespace AdminDB
{
	const recString	UserID = { "RefNo", 1, 10 };
	const recString	DateLastUsed = { "Date", 0, 8 };
	const recString	TimeLastUsed = { "Time", 0, 8 };
}

/**********************************************************************/

class CAdminDatabase : public CSSStringDatabase
{
public:
	CAdminDatabase();
	virtual const char* FormatKey ( const char* szKey );

	int OpenForSharing ( int nMode = DB_READONLY );
	const char* GetHeader();

	void Update ( const char* szUserID );
	bool Exists ( const char* szUserID );

	CString GetUserID()							{ return (CString)CSSDatabase::GetString ( AdminDB::UserID.Label ); }
	void SetUserID ( const char* szText )		{ SetField ( AdminDB::UserID.Label, szText, AdminDB::UserID.Max ); }

	void SetDateLastUsed ( const char* szDate )	{ SetDateField ( AdminDB::DateLastUsed.Label, szDate ); }
	CString GetDateLastUsed()					{ return(CString)CSSDatabase::GetString ( AdminDB::DateLastUsed.Label ); }

	void SetTimeLastUsed ( const char* szTime )	{ SetTimeField ( AdminDB::TimeLastUsed.Label, szTime ); }
	CString GetTimeLastUsed()					{ return (CString)CSSDatabase::GetString ( AdminDB::TimeLastUsed.Label ); }

private:
	void SetField ( const char* strLabel, const char* szText, int nMax );
	void SetDateField ( const char* szLabel, const char* szDate );
	void SetTimeField ( const char* szLabel, const char* szTime );

private:
	CString m_strKey = "";
	CString m_strFilename = "";
};

/**********************************************************************/