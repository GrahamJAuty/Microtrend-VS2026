//******************************************************************
#include <mmsystem.h>
//******************************************************************
#include "GlobalFunctions.h"
#include "SelectionDlg.h"
#include "SystemData.h"
#include "WaitDlg.h"
//******************************************************************
#include "GetUserDetails.h"
//******************************************************************

CGetUserDetails::CGetUserDetails(const char* szFilename, CUserIDRecord* pRecord, CWnd* pParentWnd)
	: m_strFilename(szFilename)
	, m_pRecord(pRecord)
	, m_pParentWnd(pParentWnd)
{
	::SendToRecycleBin(System.m_strLoyReqTempFilename);
}

//**********************************************************************

bool CGetUserDetails::Extract(bool bServerValidate)
{
	bool bReply = TRUE;

	CSSFile file;
	if ((bReply = file.Open(m_strFilename, "rb")) == TRUE)
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

			// set passed record to last UserID so it can be used
			// incase server not running & Admin database can be checked

			m_pRecord->SetUserID(strUserID);
			if (FALSE == bServerValidate)					// Server not onlne
			{
				bReply = FALSE;
				break;
			}

			if (AddToLoyReqFile(strPrefix, strUserID) == FALSE)
			{
				bReply = FALSE;
				break;
			}
		}
		file.Close();
		::SendToRecycleBin(m_strFilename);

		if (TRUE == bReply)									// see if all ok so far
		{
			bReply = SendLoyReqFile();						// send request file to server
		}
	}

	return bReply;
}

//**********************************************************************
// szCommandPrefix = ID if biometric input or MF = Mifare input 

bool CGetUserDetails::AddToLoyReqFile(const char* szCommandPrefix, const char* szUserID)
{
	bool bReply = FALSE;

	CString strUserID = szUserID;
	strUserID.TrimLeft('0');								//	remove leading zeros if any

	CSSFile file;
	if (file.Open(System.m_strLoyReqTempFilename, "ab") == TRUE)
	{
		CString strLine = "";
		strLine.Format("%s=%s",
			szCommandPrefix,
			(const char*)strUserID);

		file.WriteLine(strLine);
		file.Close();
		bReply = TRUE;
	}
	else
	{
		m_nMessageNo = nMESSAGE_FILECREATEERROR;
	}

	return bReply;
}

//**********************************************************************

bool CGetUserDetails::SendLoyReqFile()
{
	bool bReply = FALSE;

	CWaitDlg waitDlg("Verifying Account Details", "Please wait", m_pParentWnd);
	waitDlg.PumpMessages();

	// create request file
	::SendToRecycleBin(System.m_strLoyReqReplyFilename);
	::SendToRecycleBin(System.m_strLoyReqFilename);
	int x = ::rename(System.m_strLoyReqTempFilename, System.m_strLoyReqFilename);

	// wait for server reply
	if (WaitReply(System.m_strLoyReqReplyFilename) == TRUE)
	{
		CSSFile file;
		if (file.Open(System.m_strLoyReqReplyFilename, "rb") == TRUE)
		{
			// scan reply file - may have multiple entries

			CString strLine = "";
			while (file.ReadString(strLine) == TRUE)
			{
				m_arrayUserLines.Add(strLine);
			}

			if (m_arrayUserLines.GetCount() == 1)
			{
				bReply = m_pRecord->ExtractUserData(m_arrayUserLines.GetAt(0));
				m_strErrorMessage = m_pRecord->GetError();
			}
			else
			{
				bReply = MultipleChoice();
			}

			file.Close();												//v1.04d	  28/04/2016
			if (TRUE == bReply)
			{
				::SendToRecycleBin(System.m_strLoyReqReplyFilename);	// clear reply file
			}
		}
		else
		{
			m_nMessageNo = nMESSAGE_FILEREADERROR;
		}
	}

	::SendToRecycleBin(System.m_strLoyReqFilename);
	return bReply;
}

//**********************************************************************

bool CGetUserDetails::WaitReply(const char* szReplyFilename)
{
	m_nMessageNo = nMESSAGE_NOSERVERRESPONSE;					// Server no reply

	Sleep(500);													// wait a bit

	for (int nCount = 0; nCount < 5; nCount++)
	{
		if (::FileExists(szReplyFilename) == TRUE)			// see if request been been processed
		{
			m_nMessageNo = nMESSAGE_NONE;
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

//**********************************************************************

bool CGetUserDetails::MultipleChoice()
{
	CSelectionDlg dlg(&m_arrayUserLines, m_pRecord, m_pParentWnd);

	if (dlg.DoModal() == IDOK)
	{
		return TRUE;
	}

	return FALSE;
}

//**********************************************************************
