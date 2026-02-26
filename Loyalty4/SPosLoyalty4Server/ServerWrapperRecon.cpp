/**********************************************************************/
#include "..\SPosLoyalty4Shared\LoyaltyAuditFile.h"
#include "..\SPosLoyalty4Shared\MessageLogger.h"
/**********************************************************************/
#include "ServerWrapper.h"
/**********************************************************************/

void CServerWrapper::ProcessReconRequest()
{
	if (Server.GetLogActionFlag() == TRUE)
	{
		CString strMsg;
		strMsg.Format("==> REQ: %s",
			(const char*)m_LoyReqThreadManager.GetREQFilepath());

		MessageLogger.LogSystemMessage(strMsg);
	}

	ProcessReconRequestInternal();

	if (m_LoyReqThreadManager.GetResponseLineCount() > 2)
	{
		CServerDataFolderSetIndex FolderSetIndex = m_LoyReqThreadManager.GetThreadIndex();

		m_LoyRspThreadManager.QueueResponseHeader(FolderSetIndex);
		for (int n = 0; n < m_LoyReqThreadManager.GetResponseLineCount(); n++)
		{
			m_LoyRspThreadManager.QueueResponseLine(FolderSetIndex, m_LoyReqThreadManager.GetResponseLine(n));
		}
		m_LoyRspThreadManager.CheckThread(FolderSetIndex);

		if (Server.GetLogActionFlag() == TRUE)
		{
			CString strMsg = "";
			strMsg += "<== RSP: ";
			strMsg += m_LoyReqThreadManager.GetResponseLine(1);
			MessageLogger.LogSystemMessage(strMsg);
		}
	}
}

/**********************************************************************/

void CServerWrapper::ProcessReconRequestInternal()
{
	CCSV csvLine(m_LoyReqThreadManager.GetRequestLine(0));

	if (csvLine.GetInt(0) != 1)
	{
		AddReconStatusLine(RECON_ERROR_REQUEST, 0);
		return;
	}

	CLoyaltyAuditFile auditFile(Filenames.GetAuditFilename());
	if (auditFile.OpenReadOnly() == FALSE)
	{
		AddReconStatusLine(RECON_ERROR_AUDIT, 0);
		return;
	}

	CString strDateFrom = csvLine.GetString(1);
	CString strDateTo = csvLine.GetString(2);

	if (ValidateReconDate(strDateFrom) == FALSE)
	{
		AddReconStatusLine(RECON_ERROR_DATE, 0);
		return;
	}

	if (ValidateReconDate(strDateTo) == FALSE)
	{
		AddReconStatusLine(RECON_ERROR_DATE, 0);
		return;
	}

	CString strCSSDateFrom = strDateFrom.Right(2) + strDateFrom.Mid(4, 2) + strDateFrom.Left(4);
	CSSDate cssDateFrom(strCSSDateFrom);

	CString strCSSDateTo = strDateTo.Right(2) + strDateTo.Mid(4, 2) + strDateTo.Left(4);
	CSSDate cssDateTo(strCSSDateTo);

	auditFile.JumpIn(strCSSDateFrom);

	int nLineCount = 0;

	m_LoyReqThreadManager.AddResponseLine("0,0");
	int nPos = m_LoyReqThreadManager.GetResponseLineCount() - 1;

	CAuditRecord_base atc;
	while (auditFile.ReadLine(&atc) == TRUE)
	{
		CString strDateFiled = atc.GetDateFiled();
		
		if (atc.m_dateFiled < cssDateFrom)
		{
			continue;
		}

		if (atc.m_dateFiled > cssDateTo)
		{
			break;
		}

		CServerDataFolderSetIndex FolderSetIndex = atc.GetFolderSetIndex();

		if (0 == FolderSetIndex.m_nDbNo)
		{
			FolderSetIndex.m_nDbNo = 1;
		}

		if ((0 == FolderSetIndex.m_nSetNo) || (atc.GetTerminalNo() == 0) || (atc.GetEposTranNo() == ""))
		{
			continue;
		}

		CCSV csvLine;
		csvLine.Add(2);	//version no;
		csvLine.Add(FolderSetIndex.m_nDbNo);
		csvLine.Add(FolderSetIndex.m_nSetNo);
		csvLine.Add(atc.GetTerminalNo());
		csvLine.Add(atc.GetEposTranNo());
		csvLine.Add(atc.GetDateFiled());
		csvLine.Add(atc.GetTimeFiled());
		m_LoyReqThreadManager.AddResponseLine(csvLine.GetLine());
		nLineCount++;
	}

	if (nLineCount > 0)
	{
		CCSV csv;
		csv.Add(0);
		csv.Add(nLineCount);
		m_LoyReqThreadManager.SetResponseLine(nPos, csv.GetLine());
	}
}

/**********************************************************************/

bool CServerWrapper::ValidateReconDate(CString strDate)
{
	if (strDate.GetLength() != 8)
	{
		return FALSE;
	}

	if (::TestNumeric(strDate) == FALSE)
	{
		return FALSE;
	}

	COleDateTime oleDate = COleDateTime(
		atoi(strDate.Left(4)),
		atoi(strDate.Mid(4, 2)),
		atoi(strDate.Right(2)),
		0, 0, 0);

	return (oleDate.m_status == COleDateTime::valid);
}

/**********************************************************************/

void CServerWrapper::AddReconStatusLine(int nStatus, int nDetail)
{
	CCSV csv;
	csv.Add(nStatus);
	csv.Add(nDetail);
	m_LoyReqThreadManager.AddResponseLine(csv.GetLine());
}

/**********************************************************************/
