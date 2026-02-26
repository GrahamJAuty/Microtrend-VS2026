/**********************************************************************/
#pragma once
/**********************************************************************/

/**********************************************************************/
/*	sample register.ini										*/
/**********************************************************************/
/*

	RegFilename=private\register\register.dbs
	LogFilename=private\register\register.log
	EcrsetFilename=private\register\ecrset.dbs

*/
/**********************************************************************/

class CSSRegister
{
public:
	CSSRegister();
	int RegisterSoftware ( const char* szProgramName, const char* szAccessCode, CString& strUserCode, bool bUserReg, bool bCheckDisable );
	CString GetUserData ( const char* szParams );
	const char* GetRegFilename() { return m_strRegFilename; }

private:
	void LogRegistration ( const char* szSerialNo, const char* szAccessCode, const char* szUserCode, const char* szProgramName );

private:
	CString m_strRegFilename;
	CString m_strLogFilename;
	CString m_strEcrsetFilename;
};

/**********************************************************************/

class CSSAccessCode
{
public:
	CSSAccessCode();
	bool DecodeAccessCode ( const char* szAccessCode, bool bWebAccess );
	CString GenerateAccessCode ( int nSerialNo, int nProgram, int nEnableNo );
	CString GetUserCode ( int n=0 );
	bool GetProgramText ( CString& strText );
	bool GetEnableText ( CString& strText );
	CString GetSerialNo() { return m_strSerialNo; }
	int GetProgramNo() { return m_nProgramNo; }
	bool IsDealerExpiry();

private:
	char Checksum ( const char* szBuf, int nLen );
	char SubMod10 ( char cDigit, int nOffset );
	char AddMod10 ( char cDigit, int nOffset );
	char GetDigit ( int i, int j );

private:
	CString m_strSerialNo;
	CString m_strAccessCode;
	int m_nProgramNo;
	int m_nEnableNo;

private:
	static int xlat[10][10];
};

/**********************************************************************/

extern const char* szVERSION_REGISTER;

/**********************************************************************/
