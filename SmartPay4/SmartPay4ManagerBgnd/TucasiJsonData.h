#pragma once

//#include "SystemData.h"

class CTucasiJsonData  
{
public:
	CTucasiJsonData();
	void Read();
	void Save();
	bool IsSet();
	CString GetWebPaymentListFilename()		 { return System.GetWebPaymentPath ( "Tv2list.dat", nWEBPAYMENT_TUCASIv2 ); }
	CString GetWebPaymentList2Filename()	 { return System.GetWebPaymentPath ( "Tv2File01.dat", nWEBPAYMENT_TUCASIv2 ); }

	CString GetWebPaymentExceptionFilename() { return System.GetWebPaymentPath ( "Tv2exc.dat", nWEBPAYMENT_TUCASIv2 ); }
	CString GetNewUpdatesFilename()			 { return System.GetWebPaymentPath ( "Tv2new.dat", nWEBPAYMENT_TUCASIv2 ); }
	CString GetUpdatesFilename()			 { return System.GetWebPaymentPath ( "Tv2File03.dat", nWEBPAYMENT_TUCASIv2 ); }

	CString GetXmlLoginPath ( const char* szFilename );
	CString GetResponseLogFilename();


//*******************************************************************

public:
	CString m_strIPAddress;
	CString m_strUsername;
	CString m_strPassword;
	CString m_strSchoolID;
	CString m_strProviderName;
	__int64 m_nLastPaymentID;
	int m_nAccountPurse;
	int m_nMaxTransactionLines;
	bool m_bImportPayments;
	bool m_bExportBalances;
	bool m_bIncludePurse2;
	bool m_bKeepJsonFile;

	int m_nAccountNameFormat; 

	CString m_strPaymentHeader;
	CString m_strUploadPurchaseHeader;

private:
	CString m_strFilename;
};
