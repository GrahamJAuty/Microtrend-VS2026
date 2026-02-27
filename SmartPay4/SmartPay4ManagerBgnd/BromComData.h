#pragma once
//*******************************************************************

namespace BromComDataFields
{
	const char* const KeepXmlLogin = "KeepXmlLogin";
	const char* const KeepXmlFiles = "KeepXmlFiles";
	const char* const SchoolID = "SchoolID";
	const char* const Username = "Username";
	const char* const Password = "Password";
	const char* const LastDate = "LastDate";
	const char* const LastTime = "LastTime";
	const char* const LastOrderID = "LastOrderID";
	const char* const LastOrderSQLDateTime = "LastOrderSQLDateTime";
	const char* const SQLFilterAdjustMinutes = "SQLFilterAdjustMinutes";
	const char* const Purse1OnlyBalance = "Purse1OnlyBalance";
	const char* const Purse2RefreshText = "Purse2RefreshText";
};

//*******************************************************************

class CBromComData  
{
public:
	CBromComData();
	void Read();
	void Save();
	bool IsSet();

	CString GetXmlLoginPath ( const char* szFilename );

	CString GetUpdatesFilename()			 { return System.GetWebPaymentPath ( "BcFile03.dat", nWEBPAYMENT_BROMCOM ); }
	CString GetBalanceDBListFilename()		 { return System.GetWebPaymentPath ( "BcFile04.dat", nWEBPAYMENT_BROMCOM ); }

	CString GetSQLFilterDateTime();

	CString GetUsername() const { return m_strUsername; }
	CString GetSchoolID() const { return m_strSchoolID; }
	CString GetPassword() const { return m_strPassword; }
	CString GetUsernameForConnection() const;
	CString GetSchoolIDForConnection() const;
	CString GetPasswordForConnection() const;
	CString GetBromComLastDate() const { return m_strLastDate; }
	CString GetBromComLastTime() const { return m_strLastTime; }
	CString GetLastOrderSQLDateTime() const { return m_strLastOrderSQLDateTime; }
	int GetSQLFilterAdjustMinutes() const { return m_nSQLFilterAdjustMinutes; }
	bool GetPurse1OnlyBalanceFlag() const { return m_bPurse1OnlyBalance; }
	CString GetPurse2RefreshText() const { return m_strPurse2RefreshText; }

	void SetUsername(const CString& strUsername) { m_strUsername = strUsername; }
	void SetSchoolID(const CString& strSchoolID) { m_strSchoolID = strSchoolID; }
	void SetPassword(const CString& strPassword) { m_strPassword = strPassword; }
	void SetBromComLastDate(const CString& strDate) { m_strLastDate = strDate; }
	void SetBromComLastTime(const CString& strTime) { m_strLastTime = strTime; }
	void SetLastOrderSQLDateTime(const CString& strDateTime) { m_strLastOrderSQLDateTime = strDateTime; }
	void SetSQLFilterAdjustMinutes(int nMinutes);
	void SetPurse1OnlyBalanceFlag(bool bPurse1OnlyBalance) { m_bPurse1OnlyBalance = bPurse1OnlyBalance; }
	void SetPurse2RefreshText(const CString& strText) { m_strPurse2RefreshText = strText; }

public:
	bool m_bKeepXmlLogin = FALSE;
	bool m_bKeepXmlFiles = FALSE;
	int m_nLastOrderID = 0;
	
private:
	CString m_strUsername = "";
	CString m_strSchoolID = "";
	CString m_strPassword = "";
	CString m_strLastDate = "20220101";
	CString m_strLastTime = "000000";
	CString m_strLastOrderSQLDateTime = "";
	int m_nSQLFilterAdjustMinutes = 0;
	bool m_bPurse1OnlyBalance = FALSE;
	CString m_strPurse2RefreshText = "";

private:
	CString m_strFilename = "";
};

//*******************************************************************
