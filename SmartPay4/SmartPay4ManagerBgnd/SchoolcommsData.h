#pragma once

//#include "SystemData.h"

class CSchoolcommsData  
{
public:
	CSchoolcommsData();
	void Read();
	void Save();
	bool IsSet();

	CString GetXmlLoginPath ( const char* szFilename );

	CString GetWebPaymentListFilename()		 { return System.GetWebPaymentPath ( "Scomlist.dat", nWEBPAYMENT_SCHOOLCOMMS ); }
	CString GetWebPaymentExceptionFilename() { return System.GetWebPaymentPath ( "ScFile01.dat", nWEBPAYMENT_SCHOOLCOMMS ); }
	CString GetUpdatesFilename()			 { return System.GetWebPaymentPath ( "ScFile03.dat", nWEBPAYMENT_SCHOOLCOMMS ); }
	CString GetBalanceDBListFilename()		 { return System.GetWebPaymentPath ( "ScFile04.dat", nWEBPAYMENT_SCHOOLCOMMS ); }

//*******************************************************************

public:
	CString m_strUploadHeader;

	bool m_bKeepXmlLogin;
	bool m_bKeepXmlFiles;
	CString m_strSoapVersion;
	CString m_strUsername;
	CString m_strSchoolID;
	CString m_strPassword;
	CString m_strMemorable;
	__int64 m_nLastPaymentID;
	int m_nAccountPurse;
	int m_nMaxTransactionLines;
	bool m_bIncludePurse2;
//	bool m_bImportPayments;
//	bool m_bExportUpdates;
//	bool m_bExportBalances;

private:
	CString m_strFilename;
};
