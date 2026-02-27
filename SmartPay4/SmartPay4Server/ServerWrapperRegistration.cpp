/**********************************************************************/
#include "..\SmartPay4Shared\MembershipDatabase.h"
#include "..\SmartPay4Shared\MessageLogger.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
/**********************************************************************/
#include "ServerWrapper.h"
/**********************************************************************/

void CServerWrapper::ProcessRegistration(const char* szMiFareNo, int nMiFareDbNo, CCSV* pCsvLine)
{
	CString strMiFareNo = szMiFareNo;
	strMiFareNo.TrimLeft('0');										// remove leading 0's

	if (strMiFareNo == "")
	{
		RegistrationError("", StringErrorCodeUnknownID(), StringErrorTextUnknownID());		// no MiFare number supplied
		return;
	}

	CString strAccountID = "";
	for (int i = 1; i < pCsvLine->GetSize(); i++)
	{
		CString strBuf = pCsvLine->GetString(i);
		if (strBuf.Left(3) == "ID=")
		{
			strAccountID = strBuf.Mid(3);
			break;
		}
	}

	if (strAccountID == "")
	{
		RegistrationError("", StringErrorCodeUnknownID(), StringErrorTextUnknownID());		// no user of supplied
		return;
	}

	bool bReply = TRUE;
	if (HaveAccountAccess(strAccountID) == TRUE)
	{
		CSQLRowAccountFull RowAccount;
		RowAccount.SetUserIDString(strAccountID);

		CSQLRepositoryAccount RepoAccount;
		if (RepoAccount.SelectRow(RowAccount,NULL).GetSQLError() == SQLCRUD_ERR_NONE)
		{
			if (2 == nMiFareDbNo)
			{
				RepoAccount.RemoveExtRef2(strMiFareNo,NULL);
				RowAccount.SetExternalRef2(strMiFareNo);
			}
			else
			{
				RepoAccount.RemoveExtRef1(strMiFareNo,NULL);
				RowAccount.SetExternalRef1(strMiFareNo);
			}

			RepoAccount.UpdateRow(RowAccount,NULL);
			CreateRequestReply(strAccountID, NULL);
		}
		else
		{
			RegistrationError(strAccountID, StringErrorCodeUnknownID(), StringErrorTextUnknownID());		// no user of supplied
			return;
		}
	}
	else
	{
		RegistrationError(strAccountID, StringErrorCodeDbAccess(), StringErrorTextDbAccessV2());	// Unable to access MiFare Database
	}
}

//**********************************************************************
// Requestline = MF=xxxxxxxxxx,ID=nnnnnn
//**********************************************************************

void CServerWrapper::RegistrationError(const char* szAccountID, const char* szCode, const char* szText)
{
	CCSV csvReply;

	{
		CString strBuf = "";
		strBuf += "ID=";
		strBuf += szAccountID;
		csvReply.Add(strBuf);
	}

	csvReply.Add(szCode);
	csvReply.Add(szText);
	m_LoyReqThreadManager.AddResponseLine(csvReply.GetLine());
}

//**********************************************************************
// ID=nnnn,ER=107,EM=Server busy

void CServerWrapper::CreatePausedReplyCSVLine(const char* szEposUserID, CCSV* pCsv)
{
	CString strBuf = "";
	strBuf.Format("ID=%s", szEposUserID);		// pass back original user ID in reply line
	pCsv->Add(strBuf);							// UserID first field in csv line

	pCsv->Add(StringErrorCodePaused());			// server currently paused
	pCsv->Add(StringErrorTextPaused());			// unable to access smartpay database
}

//**********************************************************************

void CServerWrapper::CreatePausedReply(const char* szEposUserID)
{
	// ID=nnnn,ER=107,EM=Server busy
	CCSV csvReply;
	CreatePausedReplyCSVLine(szEposUserID, &csvReply);
	m_LoyReqThreadManager.AddResponseLine(csvReply.GetLine());
}

//**********************************************************************

bool CServerWrapper::HaveAccountAccess(const char* szAccountID)
{
	bool bReply = TRUE;
	CString strRecordEditFilename = Filenames.GetRecordEditFilename(szAccountID);

	if (::FileExists(strRecordEditFilename) == TRUE)			// see if edit file is present
	{
		if (::IsAccountInuse(szAccountID) == TRUE)				// see if still active
		{
			bReply = FALSE;
		}
	}
	return bReply;
}

//**********************************************************************
