//*******************************************************************
#include "GTData.h"
#include "PayLinkAuditRecord.h"
#include "PostTransaction.h"
#include "SystemData.h"
#include "WaitPostTransactionDlg.h"
//*******************************************************************
#include "PaylinkInterface.h"
//*******************************************************************

CPaylinkInterface::CPaylinkInterface(void)
{
}

//*******************************************************************

bool CPaylinkInterface::Enable(CWnd* pParentWnd)
{
	long nOpenStatus = OpenMHE();							// Open PayLink interface

	switch (nOpenStatus)
	{
	case 13:	m_strError = "DLL or PayLink device incompatible";		break;
	case 20:	m_strError = "Cannot find required PayLink device";		break;
	case 21:	m_strError = "PayLink device is not ready";				break;
	case 31:	m_strError = "PayLink device driver not running";		break;
	case 170:	m_strError = "USB link is in use";						break;
	case 1167:	m_strError = "PayLink device is not connected";			break;

	default:
		m_strError.Format("PayLink open status failed (E:%ld)", nOpenStatus);
		break;
	}

	if (nOpenStatus == 0)
	{
		if (CheckSerialNumber() == TRUE)				// check serial numbers match otherwise set error message
		{
			m_bStartLevelOK = CheckStartLevel(pParentWnd);		// check StartLevel = LastEndLevel otherwise last user terminated early & lost coinage

			EnableInterface();							// enable Paylink interface
			return TRUE;
		}
	}

	return FALSE;
}

//*******************************************************************
//1. -1 indicates that a serial number has not been set in the device.
//2. 0 indicates that the device firmware does not support serial numbers
//3. >0 = Serial number

bool CPaylinkInterface::CheckSerialNumber()
{
	long lSerialNoPaylink = SerialNumber();				// get PayLink serial number

	if (lSerialNoPaylink > -1)							// see if set
	{
		if (System.GetPaylinkSerialNo() == 0)
		{
			System.SetPaylinkSerialNo(lSerialNoPaylink);		// not beend set set - so save it
			System.Write();
		}

		if (lSerialNoPaylink != System.GetPaylinkSerialNo())
		{
			m_strError.Format("PayLink device serial no. mismatch (%ld:%ld)", lSerialNoPaylink, System.GetPaylinkSerialNo());
			return FALSE;
		}
	}

	return TRUE;
}

//********************************************************************
// See if Paylink StartLevel is the same as the previous Paylink EndLevel
// if not create transaction as if from kiosk
// & log in pending exception file

bool CPaylinkInterface::CheckStartLevel(CWnd* pParentWnd)
{
	bool bReply = TRUE;
	long lStartValue = GetCurrentValue();			// get current level from Paylink interface

	CSSFile file;
	if (file.Open(System.m_strPayLinkLevelFilename, "rb") == TRUE)	// get last paylink End value & last user details
	{
		CPayLinkAuditRecord	audit;
		if (audit.ReadLine(&file) == TRUE)						// read line
		{
			file.Close();

			long lLastValue = audit.CoinageAdded();					// get the previous Paylink EndLevel value
			if (lLastValue > 0)
			{
				if (lStartValue != lLastValue)						// it should be same as next Paylink StartLevel
				{
					long lCoinageAdded = lStartValue - lLastValue;

					if (lCoinageAdded > 0)							// just in case
					{													// could happen if paylink box changed 
						CGTData gt;

						int nCCNo = gt.NextCCNo(FALSE);					// get what will be the next ccno
						gt.UpdateGTTotals(lCoinageAdded);			// update GT & CID

						audit.SetCCNo(nCCNo);
						audit.SetCoinageAdded(lCoinageAdded);		// the difference in End & Start levels = coinage missing
						audit.SetGTValue(gt.GetGT());				// current GT

						// make an transaction audit record as if come from from kiosk

						if (file.Open(System.m_strPayLinkTTLFilename, "wb") == TRUE)	// make as last transaction total
						{
							audit.SaveLine(&file);
							file.Close();
						}

						// add missing transaction to Offline pending file so it gets sent to server before next user input

						CPostTransaction trans;
						trans.AddToOffline();	// add to offline file

						// post transaction now
						CWaitPostTransactionDlg dlgPost(nTRANPOST_ACTION_SENDOFFLINE, pParentWnd);	
						dlgPost.DoModal();			

						// save to log of early terminated transactions

						CSSFile file;
						if (file.Open(System.m_strPendingLogFilename, "ab") == TRUE)
						{
							audit.SaveLine(&file);						// create a list jobs that were sent as pending updates
							file.Close();
						}

						// save transaction to kiosk log file

						if (file.Open(System.m_strPayLinkLogFilename, "ab") == TRUE)	// log individual value line
						{
							audit.SetText(szTEXT_STARTUP_POSTINGOK);
							audit.SaveLine(&file);
							file.Close();
						}
					}
					// reset Paylink Startup value

					if (file.Open(System.m_strPayLinkLevelFilename, "wb") == TRUE)	// reset Paylink level indicators
					{
						audit.SetText("");							// clear text
						audit.SetCoinageAdded(lStartValue);			// new Startlevel
						audit.SaveLine(&file);
						file.Close();
					}

					bReply = FALSE;
				}
			}
		}
	}

	return bReply;
}

//********************************************************************
