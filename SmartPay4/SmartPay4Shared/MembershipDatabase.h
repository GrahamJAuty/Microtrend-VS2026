#pragma once
/**********************************************************************/

namespace Membership
{
const recString		MemberID	= { "MemberID", 1, 16 };				// also used by Custom Importing for field header translation
const recString		UserID		= { "UserID",   0, 16 };				// card no 0000 = cash sales
}

/**********************************************************************/

class CMemberDatabase : public CSSStringDatabase
{
public:
	CMemberDatabase(CString strFilename);
	virtual const char* FormatKey ( const char* szKey );
	bool OpenForSharing ( int nMode = DB_READONLY );
	CString GetError() { return m_strError; }
	
	CString GetDbaseFilename() { return m_strFilename; }
	void SetDatabaseName ( const char* szFilename ) { m_strFilename = szFilename; }
	
	const char* GetHeader();
	int GetPercentPosition();

	CString GetMemberID() { return (CString)CSSDatabase::GetString ( Membership::MemberID.Label ); }

	CString GetUserID() { return (CString)CSSDatabase::GetString ( Membership::UserID.Label ); }
	void SetUserID ( const char* szText ) { SetField ( Membership::UserID.Label, szText, Membership::UserID.Max ); }

private:
	void SetField ( const char* strLabel, const char* szText, int nMax );

private:
	CString m_strKey;
	CString m_strFilename;
};

/**********************************************************************/