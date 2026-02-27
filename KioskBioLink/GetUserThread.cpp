/***********************************************************************/
#include "GlobalFunctions.h"	
/***********************************************************************/
#include "GetUserThread.h"
/**********************************************************************/

CGetUserThreadInfo::CGetUserThreadInfo()
{
}

/**********************************************************************/

UINT CGetUserThread::DoWork(LPVOID pParam)
{
	CGetUserThreadInfo* pInfo = (CGetUserThreadInfo*)pParam;
	DoWorkInternal(pInfo);
	return 0;
}

/**********************************************************************/

void CGetUserThread::DoWorkInternal(CGetUserThreadInfo* pInfo)
{
	::SendToRecycleBin(pInfo->m_strLoyReqTempFilename);
	pInfo->m_bBoolResult = Extract(pInfo);
}

/**********************************************************************/

bool CGetUserThread::Extract(CGetUserThreadInfo* pInfo)
{
	bool bReply = TRUE;

	CSSFile file;
	if ((bReply = file.Open(pInfo->m_strFilename, "rb")) == TRUE)
	{
		CString strLine = "";
		while (file.ReadString(strLine) == TRUE)
		{
			CString strPrefix = "ID";						// assume it was biometric input;
			CString strUserID = strLine;

			if (strLine.Left(3) == "MF=")					// Dual interface / MiFare card used
			{
				strUserID = strLine.Mid(3);					// lose 'MF='
				strPrefix = "MF";
			}

			else if (strLine.Left(4) == "MF2=")			// ExtRef2 interface
			{
				strUserID = strLine.Mid(4);					// lose 'MF2='
				strPrefix = "MF2";
			}

			else if (strLine.Left(4) == "PIN=")
			{
				strUserID = strLine.Mid(4);
				strPrefix = "PIN";
			}

			// set passed record to last UserID so it can be used
			// incase server not running & Admin database can be checked

			pInfo->m_UserIDRecord.SetUserID(strUserID);
			if (FALSE == pInfo->m_bServerValidate)					// Server not online
			{
				bReply = FALSE;
				break;
			}

			if (AddToLoyReqFile(pInfo, strPrefix, strUserID) == FALSE)
			{
				bReply = FALSE;
				break;
			}
		}
		file.Close();
		::SendToRecycleBin(pInfo->m_strFilename);

		if (TRUE == bReply)									// see if all ok so far
		{
			bReply = SendLoyReqFile(pInfo);					// send request file to server
		}
	}

	return bReply;
}

/**********************************************************************/

bool CGetUserThread::AddToLoyReqFile(CGetUserThreadInfo* pInfo, CString strPrefix, CString strUserID)
{
	bool bReply = FALSE;

	strUserID.TrimLeft('0');

	CSSFile file;
	if (file.Open(pInfo->m_strLoyReqTempFilename, "ab") == TRUE)
	{
		CString strLine = "";
		strLine.Format("%s=%s",
			(const char*)strPrefix,
			(const char*)strUserID);

		file.WriteLine(strLine);
		file.Close();
		bReply = TRUE;
	}
	else
	{
		pInfo->m_nMessageNo = nMESSAGE_FILECREATEERROR;
	}

	return bReply;
}

/**********************************************************************/

bool CGetUserThread::SendLoyReqFile(CGetUserThreadInfo* pInfo)
{
	bool bReply = FALSE;

	// create request file
	::SendToRecycleBin(pInfo->m_strLoyReqReplyFilename);
	::SendToRecycleBin(pInfo->m_strLoyReqFilename);
	::rename(pInfo->m_strLoyReqTempFilename, pInfo->m_strLoyReqFilename);

	// wait for server reply
	if (WaitReply(pInfo) == TRUE)
	{
		CSSFile file;
		if (file.Open(pInfo->m_strLoyReqReplyFilename, "rb") == TRUE)
		{
			// scan reply file - may have multiple entries

			CString strLine = "";
			while (file.ReadString(strLine) == TRUE)
			{
				pInfo->m_arrayUserLines.Add(strLine);
			}

			bReply = (pInfo->m_arrayUserLines.GetSize() != 0);

			file.Close();
			::SendToRecycleBin(pInfo->m_strLoyReqReplyFilename);	// clear reply file
		}
		else
		{
			pInfo -> m_nMessageNo = nMESSAGE_FILEREADERROR;
		}
	}

	::SendToRecycleBin(pInfo -> m_strLoyReqFilename);
	return bReply;
}

/**********************************************************************/

bool CGetUserThread::WaitReply(CGetUserThreadInfo* pInfo)
{
	pInfo->m_nMessageNo = nMESSAGE_NOSERVERRESPONSE;					// Server no reply

	Sleep(500);													// wait a bit

	for (int nCount = 0; nCount < 5; nCount++)
	{
		if (::FileExists(pInfo -> m_strLoyReqReplyFilename) == TRUE)			// see if request been been processed
		{
			pInfo->m_nMessageNo = nMESSAGE_NONE;
			return TRUE;
		}

		if (2 == nCount)
		{
			::PlayMessage(nMESSAGE_WAITINGFORSERVER);
		}

		Sleep(1000);											// wait again
	}

	return FALSE;
}

/**********************************************************************/