//$$******************************************************************
#include "..\SmartPay4ManagerBgnd\WebPaymentExport.h"
#include "..\SmartPay4ManagerBgnd\WebPaymentExportTxLog.h"
//$$******************************************************************
#include "ReportCSVFile.h"
//$$******************************************************************
#include "..\SmartPay4ManagerBgnd\WisePayExport.h"
//$$******************************************************************

//************************************************************************
//*** Create starting balance file from a list file **********************
//************************************************************************

bool CWisePayExport::SendOpeningBalanceList(const char* szListFilename)
{
	CString strMsg;
	int nCount1 = 0;
	int nCount2 = 0;
	bool bReply = FALSE;
	CString strUpdatesFile = "";
	CString strTitle = "Send WisePay Balances";

	// Diner Balances *******************************************************

	strUpdatesFile = m_data.GetStartingBalance1Filename();			// Diner balances
	nCount1 = CreateStartingBalancesFile(1, szListFilename, strUpdatesFile);

	strMsg.Format("%d Diner balance updates ready to send.", nCount1);
	if (nCount1 > 0)
	{
		strMsg += "\n\nDo you wish to view the data?";

		if (Prompter.YesNo(strMsg, strTitle, TRUE) == IDYES)
		{
			CCSVFileReport report("R217", strUpdatesFile, m_pParent);
			report.SetMode(P_WAIT);
			report.Show(strTitle);
		}

		if (Prompter.YesNo("Do you wish to send the Diner balances updates now?", strTitle) == IDYES)
		{
			CString strLoginReply = System.GetWebPaymentPath("PostBalance1LoginReply.xml");

			if ((bReply = SendTransactions1(strUpdatesFile, strLoginReply)) == TRUE)
			{
				LogBalanceUpdates(1, strUpdatesFile, nCount1);		// log Service1 balance updates

				if (m_data.m_bKeepXmlLogin == FALSE)
				{
					CFileRecycler::SendToRecycleBin(strLoginReply);
				}
			}
		}
	}
	else
	{
		Prompter.Info(strMsg, strTitle);				// no balance to send
	}

	if (m_data.m_bImportAccount2 == TRUE)
	{
		// Supplement Service *********************************************************

		strUpdatesFile = m_data.GetStartingBalance2Filename();		// Supplemenetary balances

		nCount2 = CreateStartingBalancesFile(2, szListFilename, strUpdatesFile);
		strMsg.Format("%d Supplementary balance updates ready to send.", nCount2);

		if (nCount2 > 0)
		{
			strMsg += "\n\nDo you wish to view the data?";

			if (Prompter.YesNo(strMsg, strTitle, TRUE) == IDYES)
			{
				CCSVFileReport report("R217", strUpdatesFile, m_pParent);
				report.SetMode(P_WAIT);
				report.Show(strTitle);
			}

			if (Prompter.YesNo("Do you wish to send the Supplementary balance updates now?", strTitle) == IDYES)
			{
				CString strLoginReply = System.GetWebPaymentPath("PostBalance2LoginReply.xml");

				if ((bReply = SendTransactions2(strUpdatesFile, strLoginReply)) == TRUE)
				{
					LogBalanceUpdates(2, strUpdatesFile, nCount2);				// log Service 2 balance updates

					if (m_data.m_bKeepXmlLogin == FALSE)
					{
						CFileRecycler::SendToRecycleBin(strLoginReply);
					}
				}
			}
		}
		else
		{
			Prompter.Info(strMsg, strTitle);				// no supplementary balances to send
		}
	}

	CString strLogComment;
	if (m_data.m_bImportAccount2 == TRUE)
	{
		strLogComment.Format("%d / %d lines", nCount1, nCount2);		// log number of lines sent in summary log
	}
	else
	{
		strLogComment.Format("%d lines", nCount1);					// log number of lines sent in summary log
	}

	CWebPaymentExportTxLog log;
	log.SaveLog(nWEBEXPORT_BALANCES, nWEBEXPORT_NOERROR, strLogComment);

	return bReply;
}

//$$******************************************************************
