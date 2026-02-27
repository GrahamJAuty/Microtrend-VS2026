#pragma once
//*******************************************************************

class CParentMailData  
{
public:
	CParentMailData();

public:
	void Read();
	void Save();
	bool IsSet();

	CString GetXmlLoginPath ( const char* szFilename );
	__int64 GetNextTransactionID() { return ++m_nLastTransactionID; }

	CString GetWebPaymentListFilename()		  { return System.GetWebPaymentPath ( "Pmlist.dat",   nWEBPAYMENT_PARENTMAIL ); }
	CString GetWebPaymentExceptionFilename()  { return System.GetWebPaymentPath ( "PmFile01.dat", nWEBPAYMENT_PARENTMAIL ); }
	CString GetNewUpdatesFilename()			  { return System.GetWebPaymentPath ( "PmFile02.dat", nWEBPAYMENT_PARENTMAIL ); }
	CString GetUpdatesFilename()			  { return System.GetWebPaymentPath ( "PmFile03.dat", nWEBPAYMENT_PARENTMAIL ); }

	bool ValidateStaffList ( const char* szText );
	virtual void DisplayStaffListError ( CWnd* pParent );
	bool IsStaffGroup ( int nGroupNo );

public:
	bool m_bKeepXmlLogin;
	bool m_bKeepXmlFiles;
	CString m_strSoapVersion;
	CString m_strUsername;
	CString m_strPassword;
	CString m_strSchoolID;
	__int64 m_nLastPaymentID;
	__int64 m_nLastTransactionID;
	int m_nAccountPurse;
	int m_nFormNameRefField;
	int m_nAdditionalRefField;
	int m_nMaxTransactionLines;
	CString m_strStaffGroupList;
	
private:
	CString m_strFilename;
	CUIntArray m_arrayStaffList;
};

//*******************************************************************
