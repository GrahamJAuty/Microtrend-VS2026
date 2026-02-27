//*******************************************************************
#include <io.h>
//*******************************************************************
#include "Cardlink.h"
#include "Defines.h"
#include "GlobalFunctions.h"
#include "GTData.h"
#include "ImageHandler.h"
#include "PayLinkAuditRecord.h"
#include "PostTransaction.h"
#include "SystemData.h"
#include "TerminalData.h"
#include "WaitPostTransactionDlg.h"
//*******************************************************************
#include "PhotoIDEnquiryDlg.h"
//*******************************************************************

CPhotoIDEnquiryDlg::CPhotoIDEnquiryDlg(CUserIDRecord* pRecord, int nTopupPurse, CWnd* pParent)
	: CSSColourDialog(CPhotoIDEnquiryDlg::IDD, pParent)
	, m_pRecord(pRecord)
	, m_nTopupPurse(nTopupPurse)
{
	m_nPayLinkRunning = (FALSE == System.GetBalanceOnlyFlag()) ? nSTART_REQD : nDONT_START;	// -1=not started, -2 Don't start
	m_nTimeoutInterval = System.m_nEnquiryTimeout;

	if (pRecord->m_dateLastUsed.IsSet() == TRUE)
	{
		CSSDate dateToday;
		dateToday.SetCurrentDate();

		CSSDate dateYesterday;
		CSSDateRange date;
		date.GetRange("Yesterday", dateYesterday, dateYesterday);

		CString strLastUsed = "";

		if (pRecord->m_dateLastUsed == dateToday)
		{
			strLastUsed = "Today";
		}
		else if (pRecord->m_dateLastUsed == dateYesterday)
		{
			strLastUsed = "Yesterday";
		}
		else
		{
			strLastUsed = pRecord->m_dateLastUsed.GetDate();
		}

		m_strDateLastUsed.Format("%s @ %s",
			(const char*)strLastUsed,
			pRecord->m_timeLastUsed.GetTime());
	}
}

//*******************************************************************

CPhotoIDEnquiryDlg::~CPhotoIDEnquiryDlg()
{
}

//*******************************************************************

void CPhotoIDEnquiryDlg::DoDataExchange(CDataExchange* pDX)
{
	CSSColourDialog::DoDataExchange(pDX);
}

//*******************************************************************

BEGIN_MESSAGE_MAP(CPhotoIDEnquiryDlg, CSSColourDialog)
	ON_WM_TIMER()
END_MESSAGE_MAP()

//*******************************************************************

BOOL CPhotoIDEnquiryDlg::OnInitDialog()
{
	CString strBuf = "";

	{
		CImageHandler image(System.m_strSysFilesImagePath, "EntryScreen");

		if (image.GetFirst() == TRUE)
		{
			strBuf = image.GetFilepath();
		}
		else
		{
			strBuf = "";
		}
	}

	bool bRounded = (strBuf == "");

	CSSColourDialog::OnInitDialog(BLACK, bRounded);
	CSSColourDialog::SetImage(strBuf, TRANSPARENT_COLOUR);

	{
		CImageHandler image(System.m_strSysFilesImagePath, "EntryFinishButton");

		if (image.GetFirst() == TRUE)
		{
			strBuf = image.GetFilepath();
		}
		else
		{
			strBuf = "";
		}
	}

	m_buttonOK.Init(IDOK, this, BLACK, MYCOLOUR_BUTTON, "", 35);
	m_buttonOK.SetImage(strBuf, TRANSPARENT_COLOUR);

	m_staticTextCardNo.Init(IDC_STATIC_TEXTCARDNO, this, WHITE, BLACK, "");
	m_staticSurname.Init(IDC_STATIC_SURNAME, this, WHITE, BLACK, "");
	m_staticGroupNo.Init(IDC_STATIC_GROUPNO, this, WHITE, BLACK, "");
	m_staticTextGroupNo.Init(IDC_STATIC_TEXTGROUPNO, this, WHITE, BLACK, "");

	m_staticTextBalance.Init(IDC_STATIC_TEXTBALANCE, this, RED, BLACK, "");
	CString strText = (3 == m_nTopupPurse) ? System.m_strPurse3Text : System.m_strPurse1Text;

	if (strText != "")
	{
		strText += " ";
	}

	strText += "Balance";
	m_staticTextBalance.SetWindowText(strText);

	m_staticBalance.Init(IDC_STATIC_BALANCE, this, WHITE, BLACK, "");
	m_staticTextAvailableSpend.Init(IDC_STATIC_TEXTAVAILABLESPEND, this, WHITE, BLACK, "");
	m_staticAvailableSpend.Init(IDC_STATIC_AVAILABLESPEND, this, WHITE, BLACK, "");
	m_staticTextLastUsed.Init(IDC_STATIC_TEXTLASTUSED, this, WHITE, BLACK, "");
	m_staticLastUsed.Init(IDC_STATIC_LASTUSED, this, WHITE, BLACK, "");
	m_staticNotAccepted.Init(IDC_STATIC_NOTACCEPTED, this, WHITE, BLACK, "");

	m_staticTextPoints.Init(IDC_STATIC_TEXTPOINTS, this, WHITE, BLACK, "");
	m_staticPoints.Init(IDC_STATIC_POINTS, this, WHITE, BLACK, "");

	m_staticNoImage.Init(IDC_STATIC_NOIMAGE, this, WHITE, BLACK, "");
	m_staticPicture.Init(IDC_STATIC_PICTURE, this, WHITE, BLACK, "");

	m_staticTextAdded.Init(IDC_STATIC_TEXTADDED, this, RED, BLACK, "");
	m_staticCashAdded.Init(IDC_STATIC_CASHADDED, this, WHITE, BLACK, "");

	if ((TRUE == System.GetBalanceOnlyFlag()) || (TRUE == System.GetNoSmallCoinWarningFlag()))
	{
		HideCashAddedBox();
		m_staticNotAccepted.ShowWindow(SW_HIDE);
	}

	// PhotoID image

	strBuf = (TRUE == System.GetShowPhotoIDFlag()) ? m_pRecord->m_strImage : "";		// get records image file

	if (strBuf == "")									// no file passed
	{
		CImageHandler image(System.m_strSysFilesImagePath, "NoImage");
		if (image.GetFirst() == TRUE)					// see if have default photo screen
		{
			strBuf = image.GetFilepath();
		}
	}

	if (strBuf != "")
	{
		m_staticNoImage.ShowWindow(SW_HIDE);
		m_staticPicture.SetImage(strBuf);
	}

	// CardNo \ Account ID

	CString strCardNo = m_pRecord->GetUserID();
	CString strCardNoText = "";

	switch (System.m_nInterfaceType)
	{
	case nINTERFACE_BIOMETRIC:
	case nINTERFACE_DUAL:
		strCardNoText = "Account";
		break;

	default:
	{
		strCardNoText = System.m_strCardNoText.Left(1);				// text = "card" or "fob"
		strCardNoText.MakeUpper();									// becomes
		strCardNoText += System.m_strCardNoText.Mid(1);				// "Card" or "Fob"

		if (nINTERFACE_MIFAREv2 == System.m_nInterfaceType)
		{
			CTerminalData terminal;
			terminal.Read();

			if (1 == terminal.m_nLocateOn)
			{
				strCardNo = m_pRecord->m_strMF2ID;
			}
			else
			{
				strCardNo = m_pRecord->m_strMFID;
			}
		}
	}
	break;
	}

	if (TRUE == System.GetHideAccountIDFlag())
	{
		int nLength = strCardNo.GetLength();

		if (nLength <= 6)
		{
			strCardNo = "* * * *";
		}
		else if (nLength > 12)										// HID card hex string of 16
		{
			strCardNo = "";
		}
		else
		{
			for (int n = 0; n < (nLength - 3); n++)				// leave last 3 digits visable
			{
				strCardNo.SetAt(n, '*');
			}
		}
	}

	if (strCardNo == "")
	{
		m_staticTextCardNo.ShowWindow(SW_HIDE);			// no show card number
	}
	else
	{
		strBuf.Format("%s  %s",
			(const char*)strCardNoText,
			(const char*)strCardNo);

		m_staticTextCardNo.SetWindowText(strBuf);
	}

	// general details

	m_staticSurname.SetWindowText(m_pRecord->m_strUserName);
	m_staticLastUsed.SetWindowText(m_strDateLastUsed);

	if (FALSE == System.GetShowGroupFlag())
	{
		m_staticTextGroupNo.ShowWindow(SW_HIDE);
		m_staticGroupNo.ShowWindow(SW_HIDE);
	}
	else
	{
		if (nGROUP_ADMIN == m_pRecord->m_nGroupNo)
		{
			strBuf = " **";
		}
		else
		{
			strBuf.Format("%d", m_pRecord->m_nGroupNo);
		}

		m_staticGroupNo.SetWindowText(strBuf);
	}

	if (FALSE == System.GetShowAvailableSpendFlag())
	{
		m_staticTextAvailableSpend.ShowWindow(SW_HIDE);
		m_staticAvailableSpend.ShowWindow(SW_HIDE);
	}

	LONGLONG timeNow = GetTickCount64();
	m_nLastTimeoutTime = timeNow;
	m_nLastInputTime = timeNow;

	DisplayBalances();
	StartTimer();
	return TRUE;
}

//*******************************************************************

void CPhotoIDEnquiryDlg::HideCashAddedBox()
{
	m_staticTextAdded.ShowWindow(SW_HIDE);
	m_staticCashAdded.ShowWindow(SW_HIDE);
}

//*******************************************************************

void CPhotoIDEnquiryDlg::StartTimer()
{
	m_hTimer = SetTimer(ID_TIMER_PHOTOID, 500, NULL);
}

//**********************************************************************

void CPhotoIDEnquiryDlg::StopTimer()
{
	if (m_hTimer != 0)
	{
		KillTimer(m_hTimer);
	}

	m_hTimer = 0;
}

//**********************************************************************

void CPhotoIDEnquiryDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == m_hTimer)
	{
		if (FALSE == m_bInTimer)					// re-entrance guard
		{
			m_bInTimer = TRUE;
			HandleTimer();

			ResetEventIfFired(m_nLastTimeoutTime);
			ResetEventIfFired(m_nLastInputTime);

			m_bInTimer = FALSE;
		}
	}
}

//**********************************************************************

void CPhotoIDEnquiryDlg::HandleTimer()
{
	if (nSTART_REQD == m_nPayLinkRunning)						// see if interface need initalising
	{
		OpenPayLinkInterface();
	}

	// COMMENT OUT THE CHECK BELOW TO SIMULATE COIN INPUT
	if (nSTARTED == m_nPayLinkRunning)
	{
		long lPayLinkValue = m_paylink.GetCurrentValue();		// read current value from PayLink interface

		//FIDDLE : UNCOMMENT TO SIMULATE COIN INPUT
		//lPayLinkValue = m_lPayLinkCurrentValue + 1;

		if (lPayLinkValue != m_lPayLinkCurrentValue)			// see if changed since last time
		{
			m_nLastTimeoutTime = 0;

			long lValueAdded = lPayLinkValue - m_lPayLinkCurrentValue;	// value of coin added

			m_lPayLinkTTLAdded += lValueAdded;					// total added in this session
			m_lPayLinkCurrentValue = lPayLinkValue;

			LogPayLinkAddition(lValueAdded);					// log individual item  + Total so far 

			DisplayBalances();
			Invalidate();
		}
	}

	// see if waited long enough
	if ((m_nTimeoutInterval != 0) && ( m_nLastTimeoutTime != 0 ) && (CheckEventDue(m_nLastTimeoutTime,m_nTimeoutInterval)))	
	{
		OnOK();
	}
	else if (CheckEventDue(m_nLastInputTime, m_nInputInterval))
	{
		if (::FileExists(System.m_strUserIDFilename) == TRUE)
		{
			m_bHaveInputFile = TRUE;
			OnOK();
		}
	}
}

//*******************************************************************

void CPhotoIDEnquiryDlg::DisplayBalances()
{
	double dPayLinkValueAdded = (double)(m_lPayLinkTTLAdded) / 100;
	double dBalance = dPayLinkValueAdded;			// cash added so far

	double dSpendLimit = 0;
	double dAvailableSpend = 0;

	if (3 == m_nTopupPurse)								// see which purse is bing topped up
	{
		dBalance += m_pRecord->m_dPurse3;			// add current purse3 balance
		dAvailableSpend = m_pRecord->m_dAvailableSpend3 + dPayLinkValueAdded;
		dSpendLimit = m_pRecord->m_dSpendLimit3;
	}
	else
	{
		dBalance += m_pRecord->m_dPurse1;			// add current purse1 balance
		dAvailableSpend = m_pRecord->m_dAvailableSpend + dPayLinkValueAdded;
		dSpendLimit = m_pRecord->m_dSpendLimit;
	}

	if (dSpendLimit != 0)									// see if daily spend limit set 
	{
		if (dAvailableSpend > dSpendLimit)				// see if over limit
		{
			dAvailableSpend = dSpendLimit;
		}
	}

	CString strBuf = "";

	strBuf.Format("£ %.2lf", dBalance);
	m_staticBalance.SetWindowText(strBuf);

	strBuf.Format("£ %.2lf", dPayLinkValueAdded);
	m_staticCashAdded.SetWindowText(strBuf);

	strBuf.Format("£ %.2lf", dAvailableSpend);
	m_staticAvailableSpend.SetWindowText(strBuf);

	// points

	if (System.GetShowPointsFlag() == FALSE)
	{
		m_staticTextPoints.ShowWindow(SW_HIDE);
		m_staticPoints.ShowWindow(SW_HIDE);
	}
	else
	{
		strBuf.Format("%d", m_pRecord->m_nPointsBalance);
		m_staticPoints.SetWindowText(strBuf);
	}
}

//*******************************************************************

void CPhotoIDEnquiryDlg::OpenPayLinkInterface()
{
	if (m_paylink.Enable(this) == TRUE)							// enabel Paylink interface box
	{
		m_lPayLinkCurrentValue = m_paylink.GetCurrentValue();	// save paylin StartLevel
		m_nPayLinkRunning = nSTARTED;						// set PayLink interface running
	}
	else
	{
		HideCashAddedBox();										// hidden cash added box
		::DisplayMessage(this, m_paylink.GetError(), "PayLink Initialization FAILED", MB_ICONSTOP);
		m_nPayLinkRunning = nSTART_FAILED;
	}
}

//*******************************************************************

void CPhotoIDEnquiryDlg::LogPayLinkAddition(long lCoinageAdded)
{
	CGTData gt;

	int nCCNo = gt.NextCCNo(FALSE);						// get what will be the next ccno
	gt.UpdateGTTotals(lCoinageAdded);				// update GT & CID

	CPayLinkAuditRecord	audit;
	audit.SetCCNo(nCCNo);
	audit.SetUserID(m_pRecord->GetUserID());
	audit.SetUserName(m_pRecord->m_strSurname);
	audit.SetGroupNo(m_pRecord->m_nGroupNo);
	audit.SetCoinageAdded(lCoinageAdded);		// value of single coin added
	audit.SetGTValue(gt.GetGT());				// current GT
	audit.SetTopupPurse(m_nTopupPurse);

	CSSFile file;
	if (file.Open(System.m_strPayLinkLogFilename, "ab") == TRUE)	// log individual value line
	{
		audit.SaveLine(&file);
		file.Close();
	}

	if (file.Open(System.m_strPayLinkTTLFilename, "wb") == TRUE)	// make as last transaction total
	{
		audit.SetCoinageAdded(m_lPayLinkTTLAdded);			// <-- overwrite with current transaction total value
		audit.SaveLine(&file);
		file.Close();
	}

	LogPaylinkCurrentLevel();									// log Paylink Level
}

//*******************************************************************

void CPhotoIDEnquiryDlg::LogPaylinkCurrentLevel()
{
	CSSFile file;
	if (file.Open(System.m_strPayLinkLevelFilename, "wb") == TRUE)	// save current Paylink Current Value
	{																	// also holds last users \ name etc
		CPayLinkAuditRecord	audit;										// used on checking for startup differs
		audit.SetUserID(m_pRecord->GetUserID());						// last person to use kiosk
		audit.SetUserName(m_pRecord->m_strSurname);
		audit.SetGroupNo(m_pRecord->m_nGroupNo);

		audit.SetCoinageAdded(m_lPayLinkCurrentValue);				// current PayLink EndLevel in pence
		audit.SetTopupPurse(m_nTopupPurse);
		audit.SaveLine(&file);
		file.Close();
	}
}

//********************************************************************

void CPhotoIDEnquiryDlg::OnOK()
{
	StopTimer();

	if (nSTARTED == m_nPayLinkRunning)					// check PayLink interface was running
	{
		m_paylink.Disable();							// disable Paylink interface
	}

	int nMessage = nMESSAGE_FINISHED;

	if (m_lPayLinkTTLAdded != 0)						// see if any cash added
	{
		double dPayLinkValueAdded = (double)(m_lPayLinkTTLAdded) / 100;

		CPostTransaction trans;
		if (trans.HaveFile() == TRUE)					// see if coinage added
		{
			CWaitPostTransactionDlg dlgPost(nTRANPOST_ACTION_SENDNORMAL, this);
			dlgPost.DoModal();

			switch (dlgPost.GetIntResult())					// send file across
			{
			case nPOST_ERROR:
				nMessage = nMESSAGE_FILEREADERROR;	// file read error
				break;

			case nPOST_SENT:
				nMessage = nMESSAGE_TOPUP_OK;		// file save to LoyLog folder ok

				if (3 == m_nTopupPurse)
				{
					m_pRecord->m_dPurse3 += dPayLinkValueAdded;			// update user record, as when using Purse1 AND Purse3 account details					
				}
				else
				{
					m_pRecord->m_dPurse1 += dPayLinkValueAdded;			// only retreived at very start.
				}

				break;

			case nPOST_OFFLINE:
				nMessage = nMESSAGE_TOPUP_FAILED;	// file saved to offline file						
				break;
			}
		}
		else
		{
			nMessage = nMESSAGE_FILEREADERROR;
		}
	}
	else
	{
		LogPaylinkCurrentLevel();						// no input but log Paylink current level
	}

	::PlayMessage(nMessage);
	if (nMESSAGE_TOPUP_FAILED == nMessage)
	{
		::DisplayMessage(this, "Topup value saved to offline pending file", "Kiosk Server Offline", 0);
	}

	CSSColourDialog::OnOK();
}

//**********************************************************************