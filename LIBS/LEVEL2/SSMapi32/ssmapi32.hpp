/**********************************************************************/
#ifndef __SSMAPI32_HPP__
#define __SSMAPI32_HPP__
/**********************************************************************/

#include <mapi.h>
#include <io.h>

/**********************************************************************/

class CSSMapi32
{
public:
	CSSMapi32();
	~CSSMapi32();

	// N.B. szFilename must be full path and filename! (not relative)

	bool SendMessage (  const char* szSubject, const char* szFilename );
	bool SendMessage (  const char* szEmail, const char* szSubject, const char* szFilename, bool bImmediate );
	bool SendMessage (  CStringArray& recipArray, const char* szSubject, const char* szFilename, bool bImmediate );

	bool SendAttachment ( const char* szSubject, const char* szFilename );
	bool SendAttachment (  const char* szEmail, const char* szSubject, const char* szFilename, bool bImmediate );
	bool SendAttachment (  CStringArray& recipArray, const char* szSubject, const char* szFilename, bool bImmediate );

	// N.B. szDestPath must be full path! (not relative) needs trailing backslash

	bool ExtractAttachments ( const char* szSubject, const char* szDestPath, bool bDeleteAfterCopy, int& nTotalFilesCopied );

	const char* GetError() { return m_strError; }

	// low level routines - not for general use

	bool LoadLibrary();
	void FreeLibrary();
	bool Logon ( ULONG ulUIParam, LPTSTR lpszProfileName, LPTSTR lpszPassword, FLAGS flFlags, ULONG ulReserved, LPLHANDLE lhSession );
	bool Logoff ( LHANDLE lhSession, ULONG ulUIParam, FLAGS flFlags, ULONG ulReserved );
	ULONG FindNext ( LHANDLE lhSession, ULONG ulUIParam, LPTSTR lpszMessageType, LPTSTR szSeedMessageID, FLAGS flFlags, ULONG ulReserved, LPTSTR lpszMessageID );
	bool ReadMail ( LHANDLE lhSession, ULONG ulUIParam, LPTSTR lpszMessageID, FLAGS flFlags, ULONG ulReserved, lpMapiMessage FAR* lpMessage );
	bool FreeBuffer ( LPVOID lpMessage );
	bool SendMail ( LHANDLE lhSession, ULONG ulUIParam, lpMapiMessage lpMessage, FLAGS flFlags, ULONG ulReserved );
	bool DeleteMail ( LHANDLE lhSession, ULONG ulUIParam, LPTSTR lpszMessageID, FLAGS flFlags, ULONG ulReserved );

private:
	ULONG (FAR PASCAL* m_pMAPILogon) ( ULONG, LPTSTR, LPTSTR, FLAGS, ULONG, LPLHANDLE );
	ULONG (FAR PASCAL* m_pMAPILogoff) ( LHANDLE, ULONG, FLAGS, ULONG );
	ULONG (FAR PASCAL* m_pMAPIFindNext) ( LHANDLE, ULONG, LPTSTR, LPTSTR, FLAGS, ULONG, LPTSTR );
	ULONG (FAR PASCAL* m_pMAPIReadMail) ( LHANDLE, ULONG, LPTSTR, FLAGS, ULONG, lpMapiMessage FAR * );
	ULONG (FAR PASCAL* m_pMAPIFreeBuffer) ( LPVOID );
	ULONG (FAR PASCAL* m_pMAPISendMail) ( LHANDLE, ULONG, lpMapiMessage, FLAGS, ULONG );
	ULONG (FAR PASCAL* m_pMAPIDeleteMail) ( LHANDLE, ULONG, LPTSTR, FLAGS, ULONG );

	bool SendMail ( CStringArray& recipArray, const char* szSubject, const char* szMessage, const char* szFilename, bool bImmediate );

	CString GetMapiError ( ULONG ulError );

private:
	HINSTANCE m_hInst;
	CString m_strError;
};

/**********************************************************************/

class CChilkatSendMail
{
public:
	int SendMail(
		CString strServer,
		CString strUsername,
		CString strPassword,
		int nPort,
		bool bSSL,
		CString strSubject,
		CString strBodyTextFilename,
		CString strReportFilename,
		CString strExt,
		bool bAttachReport,
		CString strFrom,
		CString strReplyTo,
		CString strTo,
		bool bTest);
};

extern const char* szVERSION_SSMAPI32;

/**********************************************************************/
#endif	/* __SSMAPI32_HPP__ */
/**********************************************************************/