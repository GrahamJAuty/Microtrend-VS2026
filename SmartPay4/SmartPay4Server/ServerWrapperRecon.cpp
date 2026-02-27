/**********************************************************************/
#include "..\SmartPay4Shared\SmartPayAuditFile.h"
/**********************************************************************/
#include "..\SmartPay4Shared\MessageLogger.h"
/**********************************************************************/
#include "ServerWrapper.h"
/**********************************************************************/

void CServerWrapper::ProcessReconRequest()
{
	{
		CString strMsg;
		strMsg.Format("==> REQ: %s",
			(const char*)m_LoyReqThreadManager.GetREQFilepath());

		MessageLogger.LogSystemMessage(strMsg);
	}

	ProcessReconRequestInternal();

	if (m_LoyReqThreadManager.GetResponseLineCount() > 2)
	{
		int nThread = m_LoyReqThreadManager.GetCurrentThread();

		m_LoyRspThreadManager.QueueResponseHeader(nThread);
		for (int n = 0; n < m_LoyReqThreadManager.GetResponseLineCount(); n++)
		{
			m_LoyRspThreadManager.QueueResponseLine(nThread, m_LoyReqThreadManager.GetResponseLine(n));
		}
		m_LoyRspThreadManager.CheckThread(nThread);

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

	CSmartPayAuditFile auditFile(Filenames.GetAuditFilename());
	if (auditFile.Open() == FALSE)
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

	CSQLAuditRecord_base atc;
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

		if ((atc.GetFolderSet() == 0) || (atc.GetTerminalNo() == 0) || (atc.GetAuditLineExtraText() == ""))
		{
			continue;
		}

		CCSV csvLine;
		csvLine.Add(1);	//version no;
		csvLine.Add(atc.GetFolderSet());
		csvLine.Add(atc.GetTerminalNo());
		csvLine.Add(atc.GetAuditLineExtraText());
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
