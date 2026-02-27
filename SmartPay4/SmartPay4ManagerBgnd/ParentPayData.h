#pragma once
//**********************************************************************

#define nPARENTPAY_LOCATEBY_ROLLNO_INDEX		0		// modify exiting account by Adno
#define nPARENTPAY_LOCATEBY_UPN_INDEX			1		// by UPN
#define nPARENTPAY_LOCATEBY_MISID_INDEX			2		// by Person_id

#define nPARENTPAY_NEWINTAKE_MISID_INDEX		0		// create account by using MisID 
#define nPARENTPAY_NEWINTAKE_ROLLNO_INDEX		1		// create account by using RollNo
#define nPARENTPAY_NEWINTAKE_NEXTFREE_INDEX		2		// next free record

//*******************************************************************

class CParentPayData  
{
public:
	CParentPayData();
	void Read();
	void Save();
	bool IsSet();
	bool ImportPayments();

	const char* GetDinerMatchHeader()	{ return "connectingPOSTxnID,parentPayAccountID,posInternalAccountID,externalAccountReference,openingBalance"; }

	CString GetXmlLoginPath ( const char* szFilename );
	CString GetWebPaymentListFilename ( int nServiceNo );
	CString GetWebPaymentExceptionFilename ( int nServiceNo );
	CString GetImportDataFilename()			 { return System.GetWebPaymentPath ( "ppFile01.dat", nWEBPAYMENT_PARENTPAY ); }	// new diner data
	CString GetNewUpdatesFilename()			 { return System.GetWebPaymentPath ( "ppFile02.dat", nWEBPAYMENT_PARENTPAY ); }	// new updates to add to web updates ( as updates.csv )
	CString GetBalanceListFilename()		 { return System.GetWebPaymentPath ( "ppFile03.dat", nWEBPAYMENT_PARENTPAY ); }	// selective balances list filen
	CString GetDinerMatchListFilename()		 { return System.GetWebPaymentPath ( "ppFile04.dat", nWEBPAYMENT_PARENTPAY ); }	// selective diner match list filen
	CString GetRetryQueueFilename()			 { return System.GetWebPaymentPath ( "ppFile05.dat", nWEBPAYMENT_PARENTPAY ); }	// list of jobs to retry before doing anything else

	CString GetDinerMatchAdviceFilename()	 { return System.GetWebPaymentPath ( "ppOut-01.dat", nWEBPAYMENT_PARENTPAY ); }
	CString GetPaymentEchoFilename()		 { return System.GetWebPaymentPath ( "ppOut-02.dat", nWEBPAYMENT_PARENTPAY ); }
	CString GetUpdatesFilename()			 { return System.GetWebPaymentPath ( "ppOut-03.dat", nWEBPAYMENT_PARENTPAY ); }	// web updates to send
	CString GetStartingBalanceFilename()	 { return System.GetWebPaymentPath ( "ppOut-04.dat", nWEBPAYMENT_PARENTPAY ); }

	CString GetLegacyLocateByText ( int nIndex = -1 );
	CString GetLegacyLocateByLabel();
	CString GetLocateByLabelRollNo();
	CString GetLocateByLabelUPN();
	CString GetLocateByLabelMisID();

	const char* GetNewIntakeAddedByText ( int nIndex = -1 );

	__int64 GetLastPosTxnID() { return m_nLastPosTxnID; }
	void SetLastPosTxnID ( __int64 n ) { m_nLastPosTxnID = n; }

	__int64 GetNextPosTxnID ( bool bIncrementReqd );
	void SetNextPosTxnID ( __int64 n ) { m_nNextPosTxnID = n; }

	int GetMealSessionNo ( int nPurseNo );
	int GetMisIDUserTextNo()	{ return m_nMisIDInfoIndex; }
	int GetUPNUserTextNo()		{ return m_nUPNInfoIndex; }
	int GetRollNoUserTextNo()	{ return m_nRollNoInfoIndex; }

	bool RollNoRequired();
	bool UPNRequired();
	bool MISIDRequired();

public:
	int GetExportPurse2SalesFlag(){ return m_bExportPurse2Sales; }
	void SetExportPurse2SalesFlag( bool b ){ m_bExportPurse2Sales = b; }

private:
	CString GetTextLabel ( int n );

public:
	bool m_bKeepXmlLogin;
	bool m_bKeepXmlFiles;
	CString m_strSchoolID;
	CString m_strSoapVersion;
	CString m_strUsername;
	CString m_strPassword;
	CString m_strPosID;
	CString m_strVendorName;

	bool m_bImportAccount1;
	__int64 m_nAccountID1;
	int m_nAccount1Purse;
	int m_nMealSession1;
	__int64 m_nLastAccount1;

	bool m_bImportAccount2;
	__int64 m_nAccountID2;
	int m_nAccount2Purse;
	int m_nMealSession2;
	__int64 m_nLastAccount2;

	bool m_bImportWithPreview;
	
	__int64 m_nLastDinerSnapshotID;
	__int64 m_nLastBalanceSnapshotID;

	int m_nMisIDInfoIndex;				// MIS ID
	int m_nUPNInfoIndex;
	int m_nRollNoInfoIndex;
	int m_nYearInfoIndex;
	int m_nRegInfoIndex;
	int m_nDobInfoIndex;
	int m_nNewIntakeAccount;
	int m_nLegacyLocateBy;
	bool m_bAddUnknowns;
	CString m_strStartFromUserID;

private:
	bool m_bExportPurse2Sales;

private:
	CString m_strFilename;
	__int64 m_nLastPosTxnID;
	__int64 m_nNextPosTxnID;
};

//*******************************************************************
