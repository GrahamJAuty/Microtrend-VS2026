//*******************************************************************
#include "..\SmartPay4Shared\ServerData.h"
#include "..\SmartPay4Shared\SmartPayLockManager.h"
#include "..\SmartPay4Shared\WaitDlg.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
//*******************************************************************
#include "..\SmartPay4ManagerBgnd\ImportPebbleMembers.h"
#include "..\SmartPay4ManagerBgnd\PebbleHelpers.h"
//*******************************************************************
#include "ImportParentPayFile.h"
//*******************************************************************
#include "AccountLinkPebbleDlg.h"
//*******************************************************************

CAccountLinkPebbleDlg::CAccountLinkPebbleDlg(CWnd* pParent)
	: CSSAutoShutdownDialog(AutoShutdownHelper, CAccountLinkPebbleDlg::IDD, pParent)
{
	m_pParent = pParent;
	m_configPebble.LoadRow(NULL);
	m_bDonePreview = FALSE;
	m_bDoneImport = FALSE;
}

//*******************************************************************

CAccountLinkPebbleDlg::~CAccountLinkPebbleDlg()
{
}

//*******************************************************************

void CAccountLinkPebbleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_START, m_buttonStart);
	DDX_Control(pDX, IDC_BUTTON_IMPORT, m_buttonImport);
}

//*******************************************************************

BEGIN_MESSAGE_MAP(CAccountLinkPebbleDlg, CSSAutoShutdownDialog)
	ON_BN_CLICKED(IDC_BUTTON_START, OnButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_IMPORT, OnButtonImport)
	//ON_BN_CLICKED(IDC_BUTTON_PURSES, OnButtonPurses)
	//ON_BN_CLICKED(IDC_BUTTON_ZERO, OnButtonZero)
END_MESSAGE_MAP()

//*******************************************************************

BOOL CAccountLinkPebbleDlg::CSSAutoShutdownPostInitDialog()
{
	SetDisplay();
	return TRUE;
}

//*******************************************************************

void CAccountLinkPebbleDlg::SetDisplay()
{
	m_buttonStart.EnableWindow(TRUE);

	if (FALSE == m_bDoneImport)
	{
		m_buttonStart.SetWindowText("Preview Links");
		m_buttonImport.EnableWindow(m_arrayPebbleMembers.GetSize() != 0);
	}
	else
	{
		m_buttonStart.SetWindowText("Review Links");
		m_buttonImport.EnableWindow(FALSE);
	}
}

//*******************************************************************
// access relevant web site
//*******************************************************************

void CAccountLinkPebbleDlg::OnButtonStart()
{
	bool bFirstTime = FALSE;

	if (FALSE == m_bDonePreview)
	{
		bFirstTime = TRUE;

		CString strError = AccessPebble();

		if (strError != "")
		{
			Prompter.Error(strError, "Access Pebble API");
			return;
		}
	}

	CSmartPayLockRequest Request;
	Request.m_bWantLockPebble = TRUE;
	Request.m_bOneShot = FALSE;

	CSmartPayLockManager LockManager;
	if (LockManager.AcquireLocks(Request, this) == LOCKMANAGER_OK)
	{
		m_bDonePreview = TRUE;
		SetDisplay();

		CImportPebbleMembers pp(&m_configPebble, m_arrayPebbleMembers, this);
		pp.ShowImports(bFirstTime);
	}
}

//*******************************************************************

CString CAccountLinkPebbleDlg::AccessPebble()
{
	CString strError = "";

	{
		CSmartPayLockManager LockManager;

		CSmartPayLockRequest Request;
		Request.m_bWantLockPebble = TRUE;
		Request.m_bOneShot = FALSE;

		if (LockManager.AcquireLocks(Request, m_pParent) != LOCKMANAGER_OK)
		{
			strError = "Unable to acquire lock for Pebble imports";
		}
		else
		{
			CWorkingDlg dlgWorking("Checking Pebble Members", FALSE);
			dlgWorking.Create();

			Server.SetPebbleDiagnosticMessageBoxFlag(TRUE);
			strError = pPebbleHelpers->GetMembers(m_configPebble, m_arrayPebbleMembers, &dlgWorking);
		}
	}

	if (strError == "")
	{
		if (m_arrayPebbleMembers.GetSize() == 0)
		{
			strError = "No new member details have been received";
		}
	}

	return strError;
}

//*******************************************************************
// process imported data
//*******************************************************************

void CAccountLinkPebbleDlg::OnButtonImport()
{
	CString strTitle = "Link Pebble Members";

	CStringArray Warning;
	Warning.Add(strTitle);
	Warning.Add("Pebble members will be linked to SmartPay accounts.");
	Warning.Add("Accounts will be matched by UPN or MIS ID.");
	//Warning.Add("Pebble credit purses will be created as required.");
	//Warning.Add("Pebble credit purses will be linked to SmartPay accounts.");

	if (Prompter.Warning(Warning) == IDNO)
	{
		return;
	}

	CString strError = ImportPebble();

	if (strError != "")
	{
		Prompter.Error(strError, strTitle);
	}
	else
	{
		m_bDoneImport = TRUE;
	}

	SetDisplay();
}

//*******************************************************************

CString CAccountLinkPebbleDlg::ImportPebble()
{
	CString strError = "";

	CImportPebbleMembers pp(&m_configPebble, m_arrayPebbleMembers, this);

	if (pp.ImportMembers() == TRUE)
	{
		CImportStatsDlgConfig ImportStatsConfig;
		//CreateCreditPurses(ImportStatsConfig);
		pp.ShowStatistics(ImportStatsConfig);
	}
	else
	{
		strError = pp.GetError();
	}

	return strError;
}

//*******************************************************************

void CAccountLinkPebbleDlg::OnCancel()
{
	OnOK();
}

//*******************************************************************

void CAccountLinkPebbleDlg::OnOK()
{
	if ((m_arrayPebbleMembers.GetSize() != 0) && (FALSE == m_bDoneImport))
	{
		if (Prompter.YesNo("You have previewed Pebble member links but not applied them.\n\nAre you sure you want to exit this dialog", FALSE) != IDYES)
		{
			return;
		}
	}

	EndDialog(IDOK);
}

//*******************************************************************

/*
void CAccountLinkPebbleDlg::DeleteCreditPurses()
{
	CSQLRecordSetAccountFull RecordSet(NULL, AccountSetFullType::NormalNoParams, NULL);
	CSQLRowAccountFull RowAccount;

	while (RecordSet.StepSelectAll(RowAccount) == TRUE)
	{
		CString strMemberID = RowAccount.GetMemberID();
		CString strPurseID = RowAccount.GetPebblePurseID();

		if ((strMemberID != "") && (strPurseID != ""))
		{
			Server.SetPebbleDiagnosticMessageBoxFlag(TRUE);
			pPebbleHelpers -> DeleteCreditPurse(m_configPebble, strMemberID, strPurseID);
		}
	}
}
*/

//*******************************************************************
