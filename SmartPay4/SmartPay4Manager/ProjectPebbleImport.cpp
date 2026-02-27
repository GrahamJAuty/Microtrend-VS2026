//*******************************************************************
#include "..\SmartPay4ManagerBgnd\WebPaymentDefines.h"
//*******************************************************************
#include "WebPaymentStatsDlg.h"
#include "WebPaymentReporter.h"
//*******************************************************************
#include "..\SmartPay4ManagerBgnd\PebbleImport.h"
//*******************************************************************

int CPebbleImport::DisplayMessage ( CString strMsg, UINT nType, int nBackgroundReply  )
{
	return Prompter.DisplayMessage( strMsg, m_strTitle, nType );
}

//******************************************************************

void CPebbleImport::DisplayImportResults(CSQLTranPebbleImportStage2& Tran)
{
	if (m_nValidPaymentCount == 0)								// check for any valid imports
	{
		CString strMsg = "";
		if (Tran.GetNewPaymentsFlag() == TRUE)
		{
			strMsg = "No new transactions have been received";
		}
		else
		{
			strMsg = "No outstanding transactions have been reprocessed";
		}

		if (Tran.GetExceptionCount() == 0)										// no exceptions
		{
			DisplayMessage(strMsg, MB_ICONINFORMATION, IDOK);
		}
		else
		{
			strMsg += szMSG_HAVEEXCEPTIONS;					// "\n\nHowever, exceptions were found.\n\nDo you wish to view the exceptions (y/n)";

			if (DisplayMessage(strMsg, MB_YESNO, IDYES) == IDYES)
			{
				CWebPaymentReporter report("R203", nWEBPAYMENT_PEBBLE, Tran.GetTempExceptionFile(), m_pParent);
				report.Show(P_WAIT);
			}
		}
	}
	else			//  have some ValidCount
	{
		CWebPaymentStatsDlg dlg(nWEBPAYMENT_PEBBLE, m_pParent);
		dlg.m_nRead = m_nTotalPaymentCount;
		dlg.m_nValid = m_nValidPaymentCount;
		dlg.m_nExceptions = Tran.GetExceptionCount();
		dlg.m_strImportFilename = Tran.GetTempLogFile();			// file showing valid import lines
		dlg.m_strExceptionFilename = Tran.GetTempExceptionFile();	// file showing import exception lines
		dlg.DoModal();

		// sort out WebPayment list file & update main pending files if required

		CString strFile = "";											// master import log file
		strFile.Format("%s.dat", 
			(const char*) Tran.GetLogFileName());

		strFile = System.GetWebPaymentPath(strFile);
		::AppendDataFile(Tran.GetTempLogFile(), strFile);
	}
}

//******************************************************************
