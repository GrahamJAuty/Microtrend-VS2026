//****************************************************************************
#include "EmptyCashDlg.h"
#include "GlobalFunctions.h"
#include "GTData.h"
#include "ImageHandler.h"
#include "KeypadDlg.h"
#include "PayLinkAuditRecord.h"
#include "Status.h"
#include "SystemData.h"
#include "WaitStatusDlg.h"
//****************************************************************************
#include "GetAdminDlg.h"
//****************************************************************************

IMPLEMENT_DYNAMIC(CGetAdminDlg, CDialog)

CGetAdminDlg::CGetAdminDlg(CUserIDRecord* pRecord, bool bOnline, CWnd* pParent)
	: CSSColourDialog(CGetAdminDlg::IDD, pParent)
{
	m_pRecord = pRecord;
	m_bOnline = bOnline;
}

//****************************************************************************

void CGetAdminDlg::DoDataExchange(CDataExchange* pDX)
{
	CSSColourDialog::DoDataExchange(pDX);
}

//****************************************************************************

BEGIN_MESSAGE_MAP(CGetAdminDlg, CSSColourDialog)
	ON_BN_CLICKED(IDC_BUTTON_TOPUP, OnButtonTopup)
	ON_BN_CLICKED(IDC_BUTTON_SETUP, OnButtonSetup)
	ON_BN_CLICKED(IDC_BUTTON_EMPTYCASH, OnButtonEmptyCash)
END_MESSAGE_MAP()

//****************************************************************************

BOOL CGetAdminDlg::OnInitDialog()
{
	CString strFilename = "";
	{
		CImageHandler image ( System.m_strSysFilesImagePath, "AdminBackground" );
		if (image.GetFirst() == TRUE)
		{
			strFilename = image.GetFilepath();
		}

		bool bRounded = (strFilename == "");
		
		CSSColourDialog::OnInitDialog( MYCOLOUR_DIALOG, bRounded );
		CSSColourDialog::SetImage ( strFilename, TRANSPARENT_COLOUR );
	}

	CImageHandler image ( System.m_strSysFilesImagePath, "AdminButton" );
	
	if (image.GetFirst() == TRUE)
	{
		strFilename = image.GetFilepath();
	}
	else
	{
		strFilename = "";
	}

	m_buttonCancel.Init( IDCANCEL, this, BLACK, MYCOLOUR_BUTTON, "", 20 );
	m_buttonCancel.SetImage ( strFilename, TRANSPARENT_COLOUR );

	m_buttonAdmin.Init(IDC_BUTTON_SETUP, this, BLACK, MYCOLOUR_BUTTON, "", 20 );
	m_buttonAdmin.SetImage ( strFilename, TRANSPARENT_COLOUR );

	m_buttonTopup.Init(IDC_BUTTON_TOPUP, this, BLACK, MYCOLOUR_BUTTON, "", 20);
	m_buttonTopup.SetImage(strFilename, TRANSPARENT_COLOUR);
	m_buttonTopup.EnableWindow(m_bOnline);

	if (TRUE == System.GetBalanceOnlyFlag())
	{
		m_buttonTopup.SetWindowText("Balance");
	}
	
	m_buttonEmptyCash.Init(IDC_BUTTON_EMPTYCASH, this, BLACK, MYCOLOUR_BUTTON, "", 20 );
	m_buttonEmptyCash.SetImage ( strFilename, TRANSPARENT_COLOUR );
	m_buttonEmptyCash.EnableWindow(m_bOnline);

	return TRUE;
}

//****************************************************************************

void CGetAdminDlg::OnButtonTopup()
{
	m_nClosingState = nGETADMIN_WANTTOPUP;
	CDialog::OnOK();
}

//****************************************************************************

void CGetAdminDlg::OnButtonSetup()
{
	if (CheckPassword() == TRUE)
	{
		m_nClosingState = nGETADMIN_WANTSETUP;
		CDialog::OnOK();
	}
	else
	{
		m_nClosingState = nGETADMIN_CANCEL;
		CDialog::OnCancel();
	}
}

//*******************************************************************

bool CGetAdminDlg::CheckPassword()
{
	// admin password already checked if PinPad system
	if (System.m_nInterfaceType == nINTERFACE_PINPAD)			
	{
		return TRUE;
	}

	return ::GetAdminPassword(this);
}

//*******************************************************************

void CGetAdminDlg::OnButtonEmptyCash()
{
	CGTData gt;
	long lExpectedCID = gt.GetCID();						// get current cash in drawer (pence) - 
	bool bCIDCleared = TRUE;

	CEmptyCashDlg dlgEmpty(&gt, this);
	dlgEmpty.DoModal();

	if (IDYES == dlgEmpty.m_nReply)
	{
		CStatus status(System.m_nTerminalNo);

		CWaitStatusDlg dlgWaitStatus(status, nWAITSTATUS_ACTION_EMPTYCASH, TRUE, this);
		dlgWaitStatus.DoModal();
		bCIDCleared = dlgWaitStatus.GetBoolResult();

		if (TRUE == bCIDCleared)
		{
			CPayLinkAuditRecord	audit;
			audit.SetUserID(m_pRecord->GetUserID());	// use who selected empty cash
			audit.SetUserName("EMPTY");
			audit.SetCoinageAdded(lExpectedCID);		// expected value in cash drawer
			audit.SetGTValue(gt.GetGT());			// what gt was at time empted

			CSSFile file;
			if (file.Open(System.m_strPayLinkLogFilename, "ab") == TRUE)	// log individual value line
			{
				audit.SaveLine(&file);
				file.Close();
			}
			gt.ClearCID();								// clear current cash in drawer
		}
	}
	else
	{
		bCIDCleared = FALSE;
	}

	CString strBuf = "";
	CString strTitle = "";

	if (0 == System.m_nTerminalNo)
	{
		strBuf = "Kiosk";
	}
	else
	{
		strBuf.Format("Kiosk %d", System.m_nTerminalNo);
	}

	if (bCIDCleared == TRUE)
	{
		strTitle.Format("%s  :  CID cleared",
			(const char*)strBuf);
	}
	else
	{
		strTitle.Format("%s  :  CID remains @ %s",
			(const char*)strBuf,
			(const char*)gt.GetCIDValue());
	}

	strBuf.Format("Current GT value  :  £ %.2lf",
		gt.GetGTValue());

	::DisplayMessage(this, strBuf, strTitle, MB_ICONINFORMATION);
}

//*******************************************************************
