//****************************************************************************
#include "GTData.h"
#include "GlobalFunctions.h"
#include "PayLinkAuditRecord.h"
#include "SystemData.h"
//****************************************************************************
#include "PostTransaction.h"
//****************************************************************************

CPostTransaction::CPostTransaction(void)
{
	m_strPayLinkTTLFilename = System.m_strPayLinkTTLFilename;
	m_strOfflineFilename = System.m_strOfflineLoyLogFilename;
}

//****************************************************************************

bool CPostTransaction::HaveFile()
{
	return ::FileExists(m_strPayLinkTTLFilename);
}

//****************************************************************************

bool CPostTransaction::AddToOffline()
{
	CSSFile fileSource;
	if (fileSource.Open(m_strPayLinkTTLFilename, "rb") == TRUE)
	{
		CString strFileAccess = "wb";
		if (::FileExists(m_strOfflineFilename) == TRUE)
		{
			strFileAccess = "ab";
		}

		CSSFile fileOffline;
		if (fileOffline.Open(m_strOfflineFilename, strFileAccess) == TRUE)
		{
			CString strTransLine = Create$TranLine(&fileSource);		// create pseudo $Tran line to send to Server

			if (strTransLine != "")
			{
				fileOffline.WriteLine(strTransLine);					// add line to offline file
			}

			fileOffline.Close();
		}

		fileSource.Close();
		::SendToRecycleBin(m_strPayLinkTTLFilename);
	}

	return !(::FileExists(m_strPayLinkTTLFilename));				// see if file gone ok
}

//****************************************************************************
// // Create a pseudo EPOS transaction line in the form if PayLink transaction exists
// $TRAN-BIO,123,1,,001000195,250,{purse},,19/07/2010,11:22

CString CPostTransaction::Create$TranLine(CSSFile* fp)
{
	CString strReply = "";

	CPayLinkAuditRecord	audit;
	if (audit.ReadLine(fp) == TRUE)
	{
		CGTData gt;
		CCSV csv;									// Create pseudo Epos $TRAN Header line

		csv.Add("$TRAN-BIO");						// KioskBioLink indicator
		csv.Add(audit.UserID());					// UserID
		csv.Add(System.m_nTerminalNo);				// Kiosk Termnal No
		csv.Add("");								// not used ( Server Number )
		csv.Add(gt.NextCCNo(TRUE));					// inc next CCNO
		csv.Add(audit.CoinageAdded());				// total PayLink transaction value (pence)
		csv.Add(audit.GetTopupPurse());				// ***** NEW in v1.08 **** identifies topup purse ( 1 or 3 )
		csv.Add("");
		csv.Add(audit.Date());						// Transaction DATE
		csv.Add(audit.Time());						// Time;
		strReply = csv.GetLine();
	}

	return strReply;
}

//****************************************************************************
