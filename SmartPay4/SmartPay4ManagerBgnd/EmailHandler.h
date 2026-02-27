#pragma once
//*******************************************************************

class CEmailHandler  
{
public:
	CEmailHandler();

public:
	bool SendEmail ( const char* szEmailAddress, const char* szSubject, const char* szSourcePath );

private:
	bool CreateSMTPMessage( const char* szEmailAddress, const char* szSubject, const char* szSourcePath );

private:
	bool IsLongFilename ( const char* szFilename );

public:
	CString m_strError;
	int m_nCount;
};

//*******************************************************************
