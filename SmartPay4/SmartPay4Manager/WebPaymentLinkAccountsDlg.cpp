// used by	sQuid
//			ParentMail
//******************************************************************
#include "..\SmartPay4ManagerBgnd\ParentMailExport.h"
#include "..\SmartPay4ManagerBgnd\SquidExport.h"
//******************************************************************
#include "..\SmartPay4Shared\RepmanHandler.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\SQLDb.h"
#include "..\SmartPay4Shared\WorkingDlg.h"
//******************************************************************
#include "CardPickerDlg.h"
//******************************************************************
#include "WebPaymentLinkAccountsDlg.h"
//******************************************************************

CWebPaymentLinkAccountsDlg::CWebPaymentLinkAccountsDlg( CWnd* pParent )
	: CSSAutoShutdownDialog(AutoShutdownHelper, CWebPaymentLinkAccountsDlg::IDD, pParent)
{
	m_pParent = pParent;
}

//******************************************************************

void CWebPaymentLinkAccountsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_IMPORT2, m_buttonImport2);
}

//******************************************************************

BEGIN_MESSAGE_MAP(CWebPaymentLinkAccountsDlg, CSSAutoShutdownDialog)
	ON_BN_CLICKED(IDC_BUTTON_IMPORT, OnButtonImport)
	ON_BN_CLICKED(IDC_BUTTON_IMPORT2, OnButtonImport2)
END_MESSAGE_MAP()

//*************************************************************************

BOOL CWebPaymentLinkAccountsDlg::CSSAutoShutdownPostInitDialog()
{
	CString strTitle = "";
	
	strTitle.Format ( "Link %s Accounts", 
		(const char*) System.GetWebPaymentDisplayName() );

	SetWindowText ( strTitle );

	if (System.GetWebPaymentType() == nWEBPAYMENT_PARENTMAIL)
	{
		m_buttonImport2.ShowWindow(SW_HIDE);
	}

	return TRUE;
}

//******************************************************************

void CWebPaymentLinkAccountsDlg::OnButtonImport()
{
	CArray<CSQLRowAccountList, CSQLRowAccountList> arrayAccountList;

	if (GetUnlinkedAccountsList(arrayAccountList) < 1 )
	{
		Prompter.Info("No records were found that required linking");
		return;
	}

	CString strTmpFile = System.GetWebPaymentPath(::GetUniqueTempFilename("$23"));

	CCardPickerDlg dlg("Unlinked Active Accounts", strTmpFile, arrayAccountList, m_pParent);
	if (dlg.DoModal() == IDOK)
	{
		if (System.GetWebPaymentType() == nWEBPAYMENT_PARENTMAIL)
		{
			CParentMailExport pme(m_pParent);				// not in background mode
			pme.LinkAccounts(strTmpFile);
		}
		else
		{
			CSquidExport squid(m_pParent);
			squid.LinkAccounts(strTmpFile);
		}
	}

	CFileRecycler::SendToRecycleBin(strTmpFile);
}

//******************************************************************

void CWebPaymentLinkAccountsDlg::OnButtonImport2()
{
	CSquidExport squid ( m_pParent );
	squid.DisassociateCards();
}

//******************************************************************

int CWebPaymentLinkAccountsDlg::GetUnlinkedAccountsList(CArray<CSQLRowAccountList, CSQLRowAccountList>& arrayAccountList)
{
	arrayAccountList.RemoveAll();
	CSQLRowAccountList RowDummy;
	arrayAccountList.Add(RowDummy);

	CWorkingDlg progress(szMSG_SCANNINGDATABASE);					// "Scanning database";
	progress.Create();

	int nCount = 0;
	int nChecked = 0;
	CSQLRecordSetAccountFull RecordSet(NULL, RSParams_AccountFull_NoParams{}, FALSE);

	CSQLRowAccountFull RowAccount;
	while (RecordSet.StepSelectAll(RowAccount) == TRUE)
	{
		if (progress.SetCaption2RecordsChecked(++nChecked) == TRUE)
		{
			break;
		}

		if (LinkRequired(RowAccount) == TRUE)
		{
			CSQLRowAccountList RowList;
			RowList.SetUserID(RowAccount.GetUserID());
			RowList.SetGroupNo(RowAccount.GetGroupNo());
			RowList.SetUsername(RowAccount.GetUsername());
			RowList.SetForename(RowAccount.GetForename());
			RowList.SetExpiryDate(RowAccount.GetExpiryDate());
			RowList.SetInactive(RowAccount.GetInactive());
			RowList.SetAlertCode(RowAccount.GetAlertCode());
			RowList.SetMemberID(RowAccount.GetMemberID());
			arrayAccountList.Add(RowList);
		}
	}

	return arrayAccountList.GetSize() - 1;
}

//******************************************************************

bool CWebPaymentLinkAccountsDlg::LinkRequired(CSQLRowAccountFull& RowAccount)
{
	if (RowAccount.GetUserID() == 0)			return FALSE;				// Cash account record
	if (RowAccount.GetMemberID() != "")			return FALSE;				// already assigned
	if (RowAccount.GetInactive() == TRUE)		return FALSE;
	if (RowAccount.HasUserExpired() == TRUE)	return FALSE;
	return TRUE;
}

//**********************************************************************


