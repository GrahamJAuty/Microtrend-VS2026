#pragma once

//******************************************************************

#define nWISEPAY_NEWINTAKE_MISID_INDEX		0		// create account by using uID 
#define nWISEPAY_NEWINTAKE_NEXTFREE_INDEX	1		// next free record

//******************************************************************

class CWisePayData  
{
public:
	CWisePayData();
	void Read();
	void Save();
	bool IsSet();

	bool ImportPayments();

	CString GetXmlLoginPath ( const char* szFilename );

	CString GetWebPaymentListFilename ( int nAccountNo );
	CString GetWebPaymentExceptionFilename ( int nAccountNo );
	CString GetImportDinerDataFilename()		 { return System.GetWebPaymentPath ( "wpFile01.dat", nWEBPAYMENT_WISEPAY ); }	// new diner data imports
	CString GetBalanceListFilename()			 { return System.GetWebPaymentPath ( "wpFile02.dat", nWEBPAYMENT_WISEPAY ); }	// selective balances list filen
	CString GetUpdatesFilename()				 { return System.GetWebPaymentPath ( "wpFile03.dat", nWEBPAYMENT_WISEPAY ); }

	CString GetStartingBalance1Filename()		 { return System.GetWebPaymentPath ( "wpOut-01.dat", nWEBPAYMENT_WISEPAY ); }	// Diner balances
	CString GetStartingBalance2Filename()		 { return System.GetWebPaymentPath ( "wpOut-02.dat", nWEBPAYMENT_WISEPAY ); }	// Supplement balances

	const char* GetNewIntakeAddedByText ( int nIndex = -1 );

public:
	CString m_strUploadHeader1;
	CString m_strUploadHeader2;
	CString m_strBalanceText;

	bool m_bKeepXmlLogin;
	bool m_bKeepXmlFiles;
	CString m_strSchoolID;
	CString m_strSoapVersion;
	CString m_strPassword;

	bool m_bImportAccount1;
	int m_nAccountID1;
	int m_nAccount1Purse;
	__int64 m_nLastAccount1;
	bool m_bImportAccount2;
	int m_nAccountID2;
	int m_nAccount2Purse;
	__int64 m_nLastAccount2;
	bool m_bIncludePurse2;

	bool m_bAddUnknowns;
	int m_nNewIntakeAccount;
	CString m_strStartFromUserID;
	bool m_bSaveEmail;
	int m_nEmailInfoIndex;
	int m_nYearInfoIndex;
	int m_nRegInfoIndex;
	int m_nDobInfoIndex;

private:
	CString m_strFilename;
};
