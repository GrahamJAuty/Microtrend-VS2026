//**********************************************************************
#include "..\SPosLoyalty4Shared\SQLDefines.h"
#include "..\SPosLoyalty4Shared\SQLNamespaces.h"
#include "..\SPosLoyalty4Shared\SQLTable_Account\SQLRepositoryAccount.h"
//**********************************************************************
#include "SPosLoyalty4Server.h"
//**********************************************************************
#include "ServerWrapper.h"
//**********************************************************************

void CServerWrapper::ProcessRegistration(const char* szMiFareNo, CCSV* pCsvLine)
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

		CSQLRepositoryAccount repoAccount;
		if (repoAccount.SelectRow(RowAccount, NULL).GetSQLResult() == SQLCRUD_ERR_NONE)
		{
			repoAccount.RemoveExtRef(strMiFareNo,NULL);
			
			RowAccount.SetExternalRef(strMiFareNo);

			repoAccount.UpdateRow(RowAccount, NULL);
			
			CAccountIdentifier AccountIdentifier(strAccountID, nINTERFACE_SWIPEv1);
			CreateBalanceReply1(AccountIdentifier, FALSE);
		}
		else
		{
			RegistrationError(strAccountID, StringErrorCodeUnknownID(), StringErrorTextUnknownID());		// no user of supplied
			return;
		}
	}
	else
	{
		RegistrationError(strAccountID, StringErrorCodeDbAccess(), StringErrorTextDbAccess2());	// Unable to access MiFare Database
	}
}

//**********************************************************************

void CServerWrapper::RegistrationError(const char* szAccountID, const char* szCode, const char* szText)
{
	CString strBuf = "";
	strBuf.Format("ID=%s", szAccountID);

	CCSV csvReply;
	csvReply.Add(strBuf);											// UserID first field in csv line
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

	pCsv->Add(strBuf);												// UserID first field in csv line
	pCsv->Add(StringErrorCodePaused());								// server currently paused
	pCsv->Add(StringErrorTextPaused());								// unable to access loyalty database
}

//**********************************************************************

void CServerWrapper::CreatePausedReply(const char* szEposUserID)
{
	CCSV csvReply;
	CreatePausedReplyCSVLine(szEposUserID, &csvReply);					// ID=nnnn,ER=107,EM=Server busy
	m_LoyReqThreadManager.AddResponseLine(csvReply.GetLine());
}

//**********************************************************************

bool CServerWrapper::HaveAccountAccess(const char* szAccountID)
{
	bool bReply = TRUE;
	CString strRecordEditFilename = Filenames.GetRecordEditFilename(szAccountID);

	if (SolutionGlobalFunctions::FileExists(strRecordEditFilename) == TRUE)			// see if edit file is present
	{
		if (::IsAccountInuse(szAccountID) == TRUE)				// see if still active
			bReply = FALSE;
	}
	return bReply;
}

//*******************************************************************
