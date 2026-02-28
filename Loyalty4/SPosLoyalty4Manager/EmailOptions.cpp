/**********************************************************************/
#include "DefTextEmailAddress.h"
#include "SMTPEmailHelpers.h"
/**********************************************************************/
#include "EmailOptions.h"
/**********************************************************************/

CEmailOptions::CEmailOptions()
{
	m_EmailMethod.Create("EmailMethod", 0, 2, 0);
	m_EmailServer.Create("EmailSMTPServer", MAX_LENGTH_SMTP_SERVER, "");
	m_EmailUsername.Create("EmailSMTPUsername", MAX_LENGTH_SMTP_USERNAME, "");
	m_EmailPassword.Create("EmailSMTPPassword", MAX_LENGTH_SMTP_PASSWORD, "");
	m_EmailFromAddress.Create("EmailSMTPFromAddress", MAX_LENGTH_SMTP_ADDRESS, "");
	m_EmailReplyTo.Create("EmailSMTPReplyTo", MAX_LENGTH_SMTP_ADDRESS, "");
	m_EmailToAddress.Create("EmailRepmanToAddress", MAX_LENGTH_SMTP_ADDRESS, "");
	m_EmailPort.Create("EmailSMTPPort", 1, 65535, 25);
	m_EmailSSL.Create("EmailSMTPUseSSL", TRUE);

	Reset();
	m_bFatalReadError = FALSE;
}

/**********************************************************************/

void CEmailOptions::CopyFrom(CEmailOptions& source)
{
	CEnhancedIniFile iniFile;
	source.PrepareIniFile(iniFile);
	ReloadFromIniFile(iniFile);
}

/**********************************************************************/

void CEmailOptions::ReloadFromIniFile(CEnhancedIniFile& iniFile)
{
	iniFile.ReadInt(m_EmailMethod);
	iniFile.ReadString(m_EmailServer);
	iniFile.ReadString(m_EmailUsername);
	iniFile.ReadString(m_EmailPassword);
	iniFile.ReadString(m_EmailFromAddress);
	iniFile.ReadString(m_EmailReplyTo);
	iniFile.ReadString(m_EmailToAddress);
	iniFile.ReadInt(m_EmailPort);
	iniFile.ReadBool(m_EmailSSL);
}

/**********************************************************************/

void CEmailOptions::PrepareIniFile(CEnhancedIniFile& iniFile)
{
	iniFile.WriteInt(m_EmailMethod);
	iniFile.WriteString(m_EmailServer);
	iniFile.WriteString(m_EmailUsername);
	iniFile.WriteString(m_EmailPassword);
	iniFile.WriteString(m_EmailFromAddress);
	iniFile.WriteString(m_EmailReplyTo);
	iniFile.WriteString(m_EmailToAddress);
	iniFile.WriteInt(m_EmailPort);
	iniFile.WriteBool(m_EmailSSL);
}

/**********************************************************************/

void CEmailOptions::Reset()
{
	m_EmailMethod.Reset();
	m_EmailServer.Reset();
	m_EmailUsername.Reset();
	m_EmailPassword.Reset();
	m_EmailFromAddress.Reset();
	m_EmailReplyTo.Reset();
	m_EmailToAddress.Reset();
	m_EmailPort.Reset();
	m_EmailSSL.Reset();
}

/**********************************************************************/

void CEmailOptions::SetDefaults()
{
	Reset();
}

/**********************************************************************/

bool CEmailOptions::Read()
{
	bool bResult = ReadInternal();
	return bResult;
}

/**********************************************************************/

bool CEmailOptions::ReadInternal()
{
	if (SolutionGlobalFunctions::FileExists(Filenames.GetEmailOptionsFilename()) == TRUE)
	{
		CEnhancedIniFile iniFile(SS_INI_AES);
		if (iniFile.Read(Filenames.GetEmailOptionsFilename()) == FALSE)
		{
			m_bFatalReadError = TRUE;
			return FALSE;
		}

		ReloadFromIniFile(iniFile);
		return TRUE;
	}

	return FALSE;
}

/**********************************************************************/

bool CEmailOptions::Write()
{
	CEnhancedIniFile iniFile(SS_INI_AES);
	PrepareIniFile(iniFile);
	return iniFile.Write(Filenames.GetEmailOptionsFilename());
}

/**********************************************************************/

int CEmailOptions::GetEmailMethodToUse()
{
	int nMethod = GetEmailMethodInternal();

	if (System.IsSPOSSMTPServerSystem() == FALSE)
	{
		if (2 == nMethod)
		{
			nMethod = 1;
		}
	}

	return nMethod;
}

/**********************************************************************/

CString CEmailOptions::GetEmailSMTPServerToUse()
{
	CString strServer = "";

	if (GetEmailMethodToUse() == 1)
	{
		strServer = GetEmailSMTPServerInternal();
		SolutionGlobalFunctions::TrimSpacesFromString(strServer, FALSE);

		CString strTest = strServer;
		strTest.MakeUpper();
		if (strTest.Find("3LEGS.COM") != -1)
		{
			strServer = "";
		}
	}

	if (GetEmailMethodToUse() == 2)
	{
		strServer = "mx1.3legs.com";
	}

	return strServer;
}

/**********************************************************************/

CString CEmailOptions::GetEmailSMTPUsernameToUse()
{
	CString strUsername = "";

	if (GetEmailMethodToUse() == 1)
	{
		strUsername = GetEmailSMTPUsernameInternal();
	}

	if (GetEmailMethodToUse() == 2)
	{
		strUsername = "noreply@sposreports.com";
	}

	return strUsername;
}

/**********************************************************************/

CString CEmailOptions::GetEmailSMTPPasswordToUse()
{
	CString strPassword = "";

	if (GetEmailMethodToUse() == 1)
	{
		strPassword = GetEmailSMTPPasswordInternal();
	}

	if (GetEmailMethodToUse() == 2)
	{ 
		strPassword = "gP#Pzd8JV4s";
	}

	return strPassword;
}

/**********************************************************************/

CString CEmailOptions::GetEmailSMTPFromAddressToUse()
{
	CString strAddress = GetEmailSMTPFromAddressInternal();

	switch (GetEmailMethodToUse())
	{
	case 1:
	case 2:
		if (strAddress == "")
		{
			strAddress = GetEmailSMTPUsernameToUse();
		}
		break;
	}

	return strAddress;
}

/**********************************************************************/

CString CEmailOptions::GetEmailSMTPReplyToToUse()
{
	CString strAddress = "";

	switch (GetEmailMethodToUse())
	{
	case 1:
	case 2:
		strAddress = GetEmailSMTPReplyToInternal();
		break;
	}

	return strAddress;
}

/**********************************************************************/

CString CEmailOptions::GetEmailRepmanToAddressToUse()
{
	CString strAddress = "";

	if (GetEmailMethodToUse() == 1)
	{
		strAddress = GetEmailRepmanToAddressInternal();
	}

	if (GetEmailMethodToUse() == 2)
	{
		strAddress = GetEmailRepmanToAddressInternal();
	}

	return strAddress;
}

/**********************************************************************/

int CEmailOptions::GetEmailSMTPPortToUse()
{
	int nPort = 0;

	if (GetEmailMethodToUse() == 1)
	{
		nPort = GetEmailSMTPPortInternal();
	}

	if (GetEmailMethodToUse() == 2)
	{
		nPort = 587;
	}

	return nPort;
}

/**********************************************************************/

bool CEmailOptions::GetEmailSMTPUseSSLFlagToUse()
{
	bool b = FALSE;

	if (GetEmailMethodToUse() == 1)
	{
		b = GetEmailSMTPUseSSLFlagInternal();
	}

	if (GetEmailMethodToUse() == 2)
	{
		b = TRUE;
	}

	return b;
}

/**********************************************************************/

int CEmailOptions::GetEmailAddresses(CString& strNickname, CStringArray& arrayEmail, CString& strInfo)
{
	strInfo = "";
	arrayEmail.RemoveAll();

	if (strNickname.Find("@", 0) != -1)
	{
		arrayEmail.Add(strNickname);
		return 0;
	}

	bool bFound = FALSE;
	CReportConsolidationArray<CSortedStringItem> arraySortedEmail;

	{
		CReportConsolidationArray<CSortedStringItem> arrayUsedNicknames;
		CReportConsolidationArray<CSortedStringItem> arrayNewNicknames;

		CSortedStringItem itemNN;
		itemNN.m_strItem = strNickname;
		arrayNewNicknames.Consolidate(itemNN);
		arrayUsedNicknames.Consolidate(itemNN);

		while (arrayNewNicknames.GetSize() > 0)
		{
			ProcessNickname(arrayNewNicknames, arrayUsedNicknames, arraySortedEmail, bFound);
		}
	}

	for (int n = 0; n < arraySortedEmail.GetSize(); n++)
	{
		CSortedStringItem itemNN;
		arraySortedEmail.GetAt(n, itemNN);
		arrayEmail.Add(itemNN.m_strItem);
	}

	if (FALSE == bFound)
	{
		strInfo = strNickname;
		return SMTP_SEND_ERROR_NICKNAME_FIND;
	}

	if (arrayEmail.GetSize() == 0)
	{
		strInfo = strNickname;
		return SMTP_SEND_ERROR_NICKNAME_EMAIL;
	}

	return 0;
}

/**********************************************************************/

void CEmailOptions::ProcessNickname(
	CReportConsolidationArray<CSortedStringItem>& arrayNewNicknames,
	CReportConsolidationArray<CSortedStringItem>& arrayUsedNicknames,
	CReportConsolidationArray<CSortedStringItem>& arraySortedEmail,
	bool& bFound)
{
	if (arrayNewNicknames.GetSize() == 0)
	{
		return;
	}

	CSortedStringItem itemNN;
	arrayNewNicknames.GetAt(0, itemNN);
	arrayNewNicknames.RemoveAt(0);

	for (int n = 0; n < EmailAddresses.GetSize(); n++)
	{
		CString strArrayNickname = EmailAddresses.GetNickname(n);

		if (strArrayNickname == itemNN.m_strItem)
		{
			bFound = TRUE;

			for (int e = 0; e < EmailAddresses.GetEmailAddressCount(n); e++)
			{
				CString strAddress = EmailAddresses.GetEmailAddress(n, e);

				if (strAddress.Find("@", 0) != -1)
				{
					CSortedStringItem itemEmail;
					itemEmail.m_strItem = strAddress;
					arraySortedEmail.Consolidate(itemEmail);
				}
				else
				{
					CSortedStringItem itemNew;
					itemNew.m_strItem = strAddress;

					int nPos = 0;
					if (arrayUsedNicknames.Find(itemNew, nPos) == FALSE)
					{
						arrayUsedNicknames.InsertAt(nPos, itemNew);
						arrayNewNicknames.Consolidate(itemNew);
					}
				}
			}

			break;
		}
	}
}

/**********************************************************************/