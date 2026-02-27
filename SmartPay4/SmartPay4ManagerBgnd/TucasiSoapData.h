#pragma once

//#include "SystemData.h"

class CTucasiSoapData  
{
public:
	CTucasiSoapData();
	void Read();
	void Save();
	bool IsSet();

	CString GetXmlLoginPath ( const char* szFilename );
	CString GetUploadPurchaseHeader();

	CString GetWebPaymentListFilename()		 { return System.GetWebPaymentPath ( "Tuclist.dat", nWEBPAYMENT_TUCASI ); }
	CString GetWebPaymentExceptionFilename() { return System.GetWebPaymentPath ( "Tucexc.dat", nWEBPAYMENT_TUCASI ); }
	CString GetUpdatesFilename()			 { return System.GetWebPaymentPath ( "TuFile03.dat", nWEBPAYMENT_TUCASI ); }

public:
	bool m_bKeepXmlLogin;
	bool m_bKeepXmlFiles;
	CString m_strUserName;
	CString m_strPassword;
	CString m_strSchoolID;
	CString m_strSoapVersion;
	__int64 m_nLastPaymentID;
	int m_nAccountPurse;
	bool m_bIncludePurse2;
	bool m_bImportPayments;
	bool m_bExportUpdates;
	bool m_bExportBalances;


private:
	CString m_strFilename;
	CString m_strUploadPurchaseHeader;
};
