//$$******************************************************************
#include "LegacyWebPaymentOptions.h"
//$$******************************************************************
static const char* szERROR1 = "This option is currently suspended as the time is outside the allotted period for web access!";
static const char* szERROR2 = "Balances have already been sent today!";
static const char* szERROR4 = "This option is currently suspended as today is outside the allotted days for web access!";
//$$******************************************************************

CLegacyWebPaymentOptions::CLegacyWebPaymentOptions()
{
	m_strFilename = Filenames.GetLegacyWebPaymentOptionsFilename();
}

//$$******************************************************************

void CLegacyWebPaymentOptions::Read()
{
	CSSIniFile file;
	if (file.Read(m_strFilename) == TRUE)
	{
		m_bPromptBeforeAccess = file.GetBool("PromptBeforeAccess", m_bPromptBeforeAccess);
		m_nPaymentDayFlags = file.GetInt("PaymentDays", m_nPaymentDayFlags);
		m_strPaymentStart = file.GetString("PaymentStart", m_strPaymentStart);
		m_strPaymentEnd = file.GetString("PaymentEnd", m_strPaymentEnd);
		m_strDateLastPayment = file.GetString("DateLastPayment", "");
		m_strTimeLastPayment = file.GetString("TimeLastPayment", "");
		m_strExcludeUploadStart = file.GetString("ExcludeUploadStart", m_strExcludeUploadStart);
		m_strExcludeUploadEnd = file.GetString("ExcludeUploadEnd", m_strExcludeUploadEnd);
		m_bExclude1Reqd = file.GetBool("ExcludeReqd", m_bExclude1Reqd);
		m_strExclude1Start = file.GetString("ExcludeStart", m_strExclude1Start);
		m_strExclude1End = file.GetString("ExcludeEnd", m_strExclude1End);
		m_bExclude2Reqd = file.GetBool("Exclude2Reqd", m_bExclude2Reqd);
		m_strExclude2Start = file.GetString("Exclude2Start", m_strExclude2Start);
		m_strExclude2End = file.GetString("Exclude2End", m_strExclude2End);
		m_bExclude3Reqd = file.GetBool("Exclude3Reqd", m_bExclude3Reqd);
		m_strExclude3Start = file.GetString("Exclude3Start", m_strExclude3Start);
		m_strExclude3End = file.GetString("Exclude3End", m_strExclude3End);
		m_bExclude4Reqd = file.GetBool("Exclude4Reqd", m_bExclude4Reqd);
		m_strExclude4Start = file.GetString("Exclude4Start", m_strExclude4Start);
		m_strExclude4End = file.GetString("Exclude4End", m_strExclude4End);
		m_strSendEODAfter = file.GetString("EndOfDayAfter", m_strSendEODAfter);
		m_strDateLastTransaction = file.GetString("DateLastTransaction", m_strDateLastTransaction);
		m_strTimeLastTransaction = file.GetString("TimeLastTransaction", m_strTimeLastTransaction);
		m_strDateLastBalance = file.GetString("DateLastBalance", m_strDateLastBalance);
		m_strTimeLastBalance = file.GetString("TimeLastBalance", m_strTimeLastBalance);
		m_bEODAfterPayments = file.GetBool("EODAfterPayments", m_bEODAfterPayments);
		m_bImportAccounts = file.GetBool("ImportAccounts", m_bImportAccounts);
		m_nAccountLinkDayFlags = file.GetInt("AccountLinkDays", m_nAccountLinkDayFlags);
		m_strAccountLinkStart = file.GetString("AccountLinkStart", m_strAccountLinkStart);
		m_strAccountLinkEnd = file.GetString("AccountLinkEnd", m_strAccountLinkEnd);
		m_strDateLastAccountLink = file.GetString("DateLastAccountLink", m_strDateLastAccountLink);
		m_strTimeLastAccountLink = file.GetString("TimeLastAccountLink", m_strTimeLastAccountLink);
	}
}

//$$******************************************************************
