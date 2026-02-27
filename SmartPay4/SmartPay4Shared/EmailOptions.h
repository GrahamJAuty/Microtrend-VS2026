/**********************************************************************/
#pragma once
/**********************************************************************/
#include "..\SmartPay4Shared\Consolidation.h"
#include "..\SmartPay4Shared\EnhancedIniFile.h"
#include "..\SmartPay4Shared\ReportConsolidationArray.h"
/**********************************************************************/
#define MAX_LENGTH_SMTP_SERVER 50
#define MAX_LENGTH_SMTP_USERNAME 50
#define MAX_LENGTH_SMTP_PASSWORD 50
#define MAX_LENGTH_SMTP_ADDRESS 50
/**********************************************************************/
#define MAX_LENGTH_ADDRESS_MULTI_EMAIL 1000
#define MAX_LENGTH_ADDRESS_NAME 25
/**********************************************************************/

class CEmailOptions 
{
public:
	CEmailOptions();
	void Reset();
	
	bool Read();
	bool Write();

	bool GetFatalReadErrorFlag() { return m_bFatalReadError; }
	void ClearFatalReadErrorFlag() { m_bFatalReadError = FALSE; }

	void CopyFrom( CEmailOptions& source );	
	void SetDefaults();

private:
	bool ReadInternal();
	void ReloadFromIniFile( CEnhancedIniFile& iniFile );
	void PrepareIniFile( CEnhancedIniFile& iniFile );
	
public:
	//EMAIL TAB
	int GetEmailMethodInternal()				{ return m_EmailMethod.GetValue(); }
	CString GetEmailSMTPServerInternal()		{ return m_EmailServer.GetValue(); }
	CString GetEmailSMTPUsernameInternal()		{ return m_EmailUsername.GetValue(); }
	CString GetEmailSMTPPasswordInternal()		{ return m_EmailPassword.GetValue(); }
	CString GetEmailSMTPFromAddressInternal()	{ return m_EmailFromAddress.GetValue(); }
	CString GetEmailSMTPReplyToInternal()		{ return m_EmailReplyTo.GetValue(); }
	CString GetEmailRepmanToAddressInternal()	{ return m_EmailToAddress.GetValue(); }
	int GetEmailSMTPPortInternal()				{ return m_EmailPort.GetValue(); }
	bool GetEmailSMTPUseSSLFlagInternal()		{ return m_EmailSSL.GetValue(); }
	bool GetEmailSMTPSmartPayDirectFlagInternal(){ return m_EmailSmartPayDirect.GetValue(); }

public:
	bool UseSMTPForDirectEmail();

public:
	int GetEmailAddresses(CString& strNickname, CStringArray& arrayEmail, CString& strInfo);
	void ProcessNickname(CReportConsolidationArray<CSortedStringItem>& arrayNewNicknames, CReportConsolidationArray<CSortedStringItem>& arrayUsedNicknames, CReportConsolidationArray<CSortedStringItem>& arraySortedEmail, bool& bFound);

public:
	//EMAIL TAB
	int GetEmailMethodToUse();
	CString GetEmailSMTPServerToUse();
	CString GetEmailSMTPUsernameToUse();
	CString GetEmailSMTPPasswordToUse();
	CString GetEmailSMTPFromAddressToUse();
	CString GetEmailSMTPReplyToToUse();
	CString GetEmailRepmanToAddressToUse();
	int GetEmailSMTPPortToUse();
	bool GetEmailSMTPUseSSLFlagToUse();
	bool GetEmailSMTPSmartPayDirectFlagToUse();
	
	//EMAIL TAB
	void SetEmailMethodInternal( int n )						{ m_EmailMethod.SetValue(n); }
	void SetEmailSMTPServerInternal( const char* sz )			{ m_EmailServer.SetValue(sz); }
	void SetEmailSMTPUsernameInternal( const char* sz )			{ m_EmailUsername.SetValue(sz); }
	void SetEmailSMTPPasswordInternal( const char* sz )			{ m_EmailPassword.SetValue(sz); }
	void SetEmailSMTPFromAddressInternal( const char* sz )		{ m_EmailFromAddress.SetValue(sz); }
	void SetEmailSMTPReplyToInternal(const char* sz)			{ m_EmailReplyTo.SetValue(sz); }
	void SetEmailRepmanToAddressInternal( const char* sz )		{ m_EmailToAddress.SetValue(sz); }
	void SetEmailSMTPPortInternal( int n )						{ m_EmailPort.SetValue(n); }
	void SetEmailSMTPUseSSLFlagInternal( bool b )				{ m_EmailSSL.SetValue(b); }
	void SetEmailSMTPSmartPayDirectFlagInternal(bool b)			{ m_EmailSmartPayDirect.SetValue(b); }
	
private:
	//EMAIL TAB
	CIniFileInt m_EmailMethod;
	CIniFileString m_EmailServer;
	CIniFileString m_EmailUsername;
	CIniFileString m_EmailPassword;
	CIniFileString m_EmailFromAddress;
	CIniFileString m_EmailReplyTo;
	CIniFileString m_EmailToAddress;
	CIniFileInt m_EmailPort;
	CIniFileBool m_EmailSSL;
	CIniFileBool m_EmailSmartPayDirect;
	
private:
	bool m_bFatalReadError;
};

/**********************************************************************/
extern CEmailOptions EmailOptions;		/* global */
/**********************************************************************/
