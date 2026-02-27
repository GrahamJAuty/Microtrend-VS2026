#pragma once
//**********************************************************************

namespace TransRef
{
const recString		AccountNo		= { "Account", 1, 10 };
const recInt64		ReferenceNo		= { "RefNo", 0 , 99999999999999 };
}

//**********************************************************************

class CTransactionReferenceDatabase : public CSSStringDatabase
{
public:
	CTransactionReferenceDatabase();
	virtual const char* GetHeader();
	virtual const char* FormatKey ( const char* szKey );

	int OpenForSharing ( const char* szFilename, int nMode = DB_READWRITE );
	CString GetNextReference();

private:
	CString m_strDatabaseFilename;
	CString m_strKey;
};

//**********************************************************************
