//******************************************************************
#include <mmsystem.h>
//******************************************************************
#include "GlobalFunctions.h"
#include "SelectionDlg.h"
#include "SystemData.h"
#include "WaitDlg.h"
//******************************************************************
#include "GetEnrolmentDetails.h"
//******************************************************************

CGetEnrolmentDetails::CGetEnrolmentDetails(const char* szMiFareNo, const char* szAccountID, CUserIDRecord* pRecord, CWnd* pParent)
	: m_strMiFareNo(szMiFareNo)
	, m_strAccountID(szAccountID)
	, m_pRecord(pRecord)
{
	m_pParent = pParent;
	::SendToRecycleBin(System.m_strLoyReqTempFilename);		
}

//******************************************************************

bool CGetEnrolmentDetails::Extract()
{
	bool bReply = CreateLoyReqFile();

	// send request file to server
	if (TRUE == bReply)
	{
		bReply = SendLoyReqFile();
	}

	return bReply;
}

//**********************************************************************

bool CGetEnrolmentDetails::CreateLoyReqFile()
{
	bool bResult = FALSE;

	CSSFile file;
	if (file.Open(System.m_strLoyReqTempFilename, "ab") == TRUE)
	{
		CString strBuf = "";

		CCSV csvLine;

		strBuf.Format("MF=%s",
			(const char*)m_strMiFareNo);

		csvLine.Add(strBuf);

		strBuf.Format("ID=%s",
			(const char*)m_strAccountID);

		csvLine.Add(strBuf);

		file.Write(csvLine);
		file.Close();

		bResult = TRUE;
	}
	else
	{
		m_nMessageNo = nMESSAGE_FILECREATEERROR;
	}

	return bResult;
}

//**********************************************************************

bool CGetEnrolmentDetails::SendLoyReqFile()
{
	bool bReply = FALSE;

	CWaitDlg waitDlg("Retrieving Account Details", "Please wait", m_pParent);
	waitDlg.PumpMessages();

	::SendToRecycleBin(System.m_strLoyReqReplyFilename);		// clear reply file if any
	::SendToRecycleBin(System.m_strLoyReqFilename);							// just in case
	int x = ::rename(System.m_strLoyReqTempFilename, System.m_strLoyReqFilename);	// enable Server to see req file

	if (WaitReply(System.m_strLoyReqReplyFilename) == TRUE)				// wait for server reply
	{
		CSSFile file;
		if (file.Open(System.m_strLoyReqReplyFilename, "rb") == TRUE)		// scan reply file - may have multiple entries
		{
			CString strLine = "";
			if (file.ReadString(strLine) == TRUE)
			{
				bReply = m_pRecord->ExtractUserData(strLine);
				m_strErrorMessage = m_pRecord->GetError();
			}
			else
			{
				m_nMessageNo = nMESSAGE_SERVERERROR;
			}

			file.Close();												//v1.04d	  28/04/2016
			if (bReply == TRUE)
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

bool CGetEnrolmentDetails::WaitReply(const char* szReplyFilename)
{
	m_nMessageNo = nMESSAGE_NOSERVERRESPONSE;					// Server no reply
	Sleep(500);													// wait a bit
	bool bReply = FALSE;

	for (int nCount = 0; nCount < 5; nCount++)
	{
		// see if request been been processed
		if (::FileExists(szReplyFilename) == TRUE)
		{
			m_nMessageNo = nMESSAGE_NONE;
			bReply = TRUE;
			break;
		}

		if (nCount == 2)
		{
			::PlayMessage(nMESSAGE_WAITINGFORSERVER);
		}

		Sleep(1000);											// wait again
	}

	return bReply;
}

//*****************************************************************
