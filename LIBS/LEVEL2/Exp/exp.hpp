/**********************************************************************/
#pragma once
/**********************************************************************/
#include "md5.hpp"
/**********************************************************************/

class CSSUnicodeFile
{
public:
	CSSUnicodeFile();
	virtual ~CSSUnicodeFile();
	bool Open ( const char* szFilename, const char* szMode );
	bool Open ( const char* szFilename, const char* szMode, int nShare, int nRetries=10 );
	int Close();

	bool ReadString ( CString& strBuf );		// read line and strip crlf
	bool WriteString ( const char* szBuf );		// write string dont add crlf

	bool ReadLine ( CString& strBuf );			// read line including crlf
	bool WriteLine ( const char* szBuf );		// add crlf and write

private:
	FILE* m_fp;
	wchar_t m_wBuf[32768];
};

/**********************************************************************/
/* private classes for use by CSSIniFile only!					*/
/**********************************************************************/

class CIniObj : public CObject
{
public:
	CIniObj ( const char* szSection, const char* szLabel, const char* szValue );

public:
	CString m_strSection;
	CString m_strLabel;
	CString m_strValue;
};

class CIniArray : public CObArray
{
public:
	virtual ~CIniArray() { RemoveAll(); }
	CString GetValue ( int n );
	void RemoveAll();
};

/**********************************************************************/

#define SS_INI_NORMAL	0	// <plain text>
#define SS_INI_PROTECTED 1	// <warning line> <md5> <plain text>
#define SS_INI_HEX		2	// <md5> <Hex encrypted>
#define SS_INI_AES		4	// <md5> <AES encrypted>

class CSSIniFile
{
public:
	CSSIniFile ( int nFlags = SS_INI_NORMAL );
	~CSSIniFile();

	bool Read ( const char* szFilename );
	bool Write ( const char* szFilename );
	bool Write();	// uses filename from Read() function

	CString GetString ( const char* szLabel, const char* szDefault );
	int GetInt ( const char* szLabel, int nDefault );
	__int64 GetInt64 ( const char* szLabel, __int64 nDefault );
	long GetLong ( const char* szLabel, long lDefault );
	double GetDouble ( const char* szLabel, double dDefault );
	bool GetBool ( const char* szLabel, bool bDefault );

	void Set ( const char* szLabel, const char* szValue );
	void Set ( const char* szLabel, int nValue );
	void Set ( const char* szLabel, __int64 nValue );
	void Set ( const char* szLabel, bool bValue );
	void Set ( const char* szLabel, double dValue, int nDp );
	void Set ( const char* szLabel, double dValue, int nDp, bool bMinimize );

	CString GetString ( const char* szSection, const char* szLabel, const char* szDefault );
	int GetInt ( const char* szSection, const char* szLabel, int nDefault );
	__int64 GetInt64 ( const char* szSection, const char* szLabel, __int64 nDefault );
	long GetLong ( const char* szSection, const char* szLabel, long lDefault );
	double GetDouble ( const char* szSection, const char* szLabel, double dDefault );
	bool GetBool ( const char* szSection, const char* szLabel, bool bDefault );

	void Set ( const char* szSection, const char* szLabel, const char* szValue );
	void Set ( const char* szSection, const char* szLabel, int nValue );
	void Set ( const char* szSection, const char* szLabel, __int64 nValue );
	void Set ( const char* szSection, const char* szLabel, bool bValue );
	void Set ( const char* szSection, const char* szLabel, double dValue, int nDp );
	void Set ( const char* szSection, const char* szLabel, double dValue, int nDp, bool bMinimize );

public:
	void WriteDataNormal(CSSFile& file);

private:
	bool Find ( const char* szSection, const char* szLabel, int& nIndex );
	void ReadDataNormal ( CSSFile& file );
	void ReadDataHex ( CSSFile& file );
	int ReadDataAES ( CSSFile& file );
	void WriteDataHex ( CSSFile& file );
	int WriteDataAES ( CSSFile& file );
	CString CalcMD5();

protected:
	CIniArray m_array;
	CString m_strFilename;
	int m_nFlags;
	bool m_bModified;
};

/**********************************************************************/

class CSecurityCode
{
public:
	static CString GetCode ( int nSerialNo );
	static int GetSerialNo ( const char* szSecurityCode );	// returns -1 if invalid security code
};

/**********************************************************************/

class CSSMD5
{
public:
	static CString CalcMD5 ( const char* szData );
	static void GetKey16 ( const char* szPassword, unsigned char* pKey );
};

/**********************************************************************/

class CSSMD5Stream
{
public:
	CSSMD5Stream();

public:
	void Begin();
	void Process(const char* szData);
	CString GetResult();

private:
	MD5_CTX m_mdContext;
	bool m_bCalledBegin;
	bool m_bCalledGetResult;
	CString m_strResult;
};

/**********************************************************************/

class CSSAES
{
public:
	// nLength = Length of source in bytes, pDest should be size of pSource + 16
	static int Encrypt ( const unsigned char* pSource, int nLength, unsigned char* pDest, const char* szPassword );

	// nLength = Length of source in bytes, pDest should be size of pSource
	static int Decrypt ( const unsigned char* pSource, int nLength, unsigned char* pDest, const char* szPassword );
};

/**********************************************************************/

// windows 7:  C:\Users\<Username>\AppData\Local\SharpSoft\<Program>\<Program>.ini
// windows XP: C:\Documents and Settings\<Username>\Local Settings\Application Data\SharpSoft\<Program>\<Program>.ini

CString SS_GetIniFilename ( const char* szProgram );

/**********************************************************************/

// windows 7:  C:\Users\<Username>\AppData\Local\SharpSoft\<Program>\ws\<md5(szId)>
// windows XP: C:\Documents and Settings\<Username>\Local Settings\Application Data\SharpSoft\<Program>\ws\<md5(szId)>

void SS_SaveUserWindowSize ( CWnd* pWnd, const char* szProgram, const char* szId=""  );
void SS_RestoreUserWindowSize ( CWnd* pWnd, const char* szProgram, const char* szId="" );

/**********************************************************************/

void SS_SaveWindowSize ( CWnd* pWnd, const char* szFilename );
void SS_RestoreWindowSize ( CWnd* pWnd, const char* szFilename );

/**********************************************************************/

extern const char* szVERSION_EXP;

/**********************************************************************/
