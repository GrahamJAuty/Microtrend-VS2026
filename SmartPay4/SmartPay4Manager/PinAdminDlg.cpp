//*******************************************************************
#include "..\SmartPay4Shared\WorkingDlg.h"
//*******************************************************************
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
//*******************************************************************
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SmartPay4Shared\SQLTable_PINNumber\SQLRepositoryPINNumber.h"
//*******************************************************************
#include "SQLTranCreatePINNumbers.h"
#include "SQLTranImportPINNumbers.h"
//*******************************************************************
#include "PINAdminDlg.h"
//*******************************************************************

CPINAdminDlg::CPINAdminDlg( CWnd* pParent /*=NULL*/)
	: CSSAutoShutdownDialog( AutoShutdownHelper, CPINAdminDlg::IDD, pParent)
{
}

//*******************************************************************

void CPINAdminDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_EXCLUDE, m_checkExclude);
	DDX_Control(pDX, IDC_CHECK_REMOVE, m_checkRemove);
}

//*******************************************************************

BEGIN_MESSAGE_MAP(CPINAdminDlg, CSSAutoShutdownDialog)
	ON_BN_CLICKED(IDC_BUTTON_RESET, OnButtonReset)
	ON_BN_CLICKED(IDC_BUTTON_IMPORT, OnButtonImport)
	ON_BN_CLICKED(IDC_BUTTON_ORPHAN, OnButtonOrphan)
END_MESSAGE_MAP()

//*******************************************************************

BOOL CPINAdminDlg::CSSAutoShutdownPostInitDialog()
{
	return TRUE;
}

//*******************************************************************

void CPINAdminDlg::OnButtonReset()
{
	bool bExclude = IsTicked(m_checkExclude);

	CStringArray arrayWarning;
	arrayWarning.Add("Reset PINs");
	arrayWarning.Add("Existing PINs will be removed from all accounts.");

	if (TRUE == bExclude)
	{
		arrayWarning.Add("Weak PINs such as 1234 will be removed from the pool.");
	}

	if (Prompter.Warning(arrayWarning, FALSE) == IDYES)
	{
		CSQLTranCreatePINNumbers Tran;
		Tran.BeginTrans();
		Tran.DoWork(TRUE, bExclude);
		
		switch (Tran.EndTrans())
		{
		case SQLTRAN_STATE_COMMITOK:
			Prompter.Info("The PIN pool has been reset as requested.");
			break;

		default:
			Tran.ShowErrorMessage("Reset PIN Pool");
			break;
		}
	}
}

//*******************************************************************

void CPINAdminDlg::OnButtonImport()
{
	CFileDialog dlg(TRUE, "csv", "ImportPIN.csv", OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_NOCHANGEDIR, "CSV Data Files (*.csv)|*.csv|All Files (*.*)|*.*||", this);
	dlg.m_ofn.lpstrTitle = "Import PIN Links";
	dlg.m_ofn.lpstrInitialDir = Sysset.GetDataPath();

	if (DoFileDialog(dlg) == IDCANCEL)
	{
		return;
	}

	CSSFile fileImport;
	if (fileImport.Open(dlg.GetPathName(), "rb") == FALSE)
	{
		Prompter.Error("Unable to open import file.", "Import PIN Links");
		return;
	}

	CReportConsolidationArray<CSortedIntByInt64> arrayPINs;

	CWorkingDlg dlgWorking("Checking PIN Link Import File", FALSE);
	dlgWorking.Create(NULL);
	int nChecked = 0;

	CString strBuffer = "";
	while (fileImport.ReadLine(strBuffer) == TRUE)
	{
		dlgWorking.SetCaption2RecordsChecked(++nChecked, FALSE);

		CCSV csv(strBuffer);

		__int64 nUserID = csv.GetInt64(0);
		int nPIN = csv.GetInt(1);

		if ((nUserID < Account::UserID.Min) || (nUserID > Account::UserID.Max))
		{
			continue;
		}

		CString strPINNumber = "";
		strPINNumber.Format("%4.4d", nPIN);
		if ( CSQLRowPINNumber::IsValidPINNumber(strPINNumber) == FALSE)
		{
			continue;
		}

		CSQLRowAccountFull RowAccount;
		RowAccount.SetUserID(nUserID);

		CSQLRepositoryAccount RepoAccount;
		if (RepoAccount.SelectRow(RowAccount, NULL).GetSQLError() != SQLCRUD_ERR_NONE)
		{
			continue;
		}

		CSQLRowPINNumber RowPINNumber;
		RowPINNumber.SetPINNumber(strPINNumber);

		CSQLRepositoryPINNumber RepoPIN;
		if (RepoPIN.SelectRow(RowPINNumber, NULL).GetSQLError() != SQLCRUD_ERR_NONE)
		{
			continue;
		}

		CSortedIntByInt64 item;
		item.m_nKey = nUserID;
		item.m_nVal = nPIN;

		int nPos = 0;
		if (arrayPINs.Find(item, nPos) == FALSE)
		{
			arrayPINs.InsertAt(nPos, item);
		}
		else
		{
			arrayPINs.SetAt(nPos, item);
		}
	}
	
	fileImport.Close();

	CReportConsolidationArray<CSortedIntItem> arrayUsedPINs;

	dlgWorking.SetCaption1("Checking for Duplicate PINs");

	for (int n = arrayPINs.GetSize() - 1; n >= 0; n--)
	{
		dlgWorking.SetCaption2RecordsOfTotal(arrayPINs.GetSize() + 1 - n, arrayPINs.GetSize(), FALSE);

		CSortedIntByInt64 item;
		arrayPINs.GetAt(n, item);

		CSortedIntItem itemUsedPIN;
		itemUsedPIN.m_nItem = item.m_nVal;

		int nPos = 0;
		if (arrayUsedPINs.Find(itemUsedPIN, nPos) == TRUE)
		{
			arrayPINs.RemoveAt(n);
		}
		else
		{
			arrayUsedPINs.InsertAt(nPos, itemUsedPIN);
		}
	}

	dlgWorking.DestroyWindow();
	arrayUsedPINs.RemoveAll();

	if (arrayPINs.GetSize() == 0)
	{
		Prompter.Info("No valid PIN links were found to import.", "Import PIN Links");
		return;
	}

	CStringArray arrayWarning;
	arrayWarning.Add("Import PIN Links");

	CString strLine = "";
	strLine.Format("The PIN links from the file will be imported (%d found)", arrayPINs.GetSize());
	arrayWarning.Add(strLine);

	if (IsTicked(m_checkRemove) == TRUE)
	{
		arrayWarning.Add("Any previous PIN links will be removed.");
	}
	else
	{
		arrayWarning.Add("Any previous links for the same PINs will be removed.");
	}

	if (Prompter.Warning(arrayWarning, FALSE) == IDYES)
	{
		CSQLTranImportPINNumbers Tran;
		Tran.BeginTrans();
		Tran.DoWork(arrayPINs, IsTicked(m_checkRemove));

		switch (Tran.EndTrans())
		{
		case SQLTRAN_STATE_COMMITOK:
			Prompter.Info("PIN links have been imported as requested.", "Import PIN Links");
			break;

		default:
			Prompter.Error(Tran.m_strError);
			break;
		}
	}
}

//*******************************************************************

void CPINAdminDlg::OnButtonOrphan()
{
	int nChecked = 0;

	CWorkingDlg dlgWorking("Checking for Unknown Accounts", FALSE);
	dlgWorking.Create(NULL);

	CStringArray arrayAccountIDsToUnlink;

	{
		CSQLRecordSetPINNumber RecordSet(NULL,
			RSParams_PINNumber_NormalNoParams{});

		RecordSet.m_strFilter.Format("%s IS NOT NULL",
			(const char*)PINNumber::UserID.Label);

		CSQLRowPINNumber RowPINNumber;
		while (RecordSet.StepSelectAll(RowPINNumber) == TRUE)
		{
			dlgWorking.SetCaption2RecordsChecked(++nChecked, FALSE);

			CSQLRowAccountFull RowAccount;
			RowAccount.SetUserIDString(RowPINNumber.GetUserID());

			CSQLRepositoryAccount repoAccount;
			if (repoAccount.SelectRow(RowAccount, NULL).GetSQLError() != SQLCRUD_ERR_NONE)
			{
				arrayAccountIDsToUnlink.Add(RowPINNumber.GetUserID());
			}
		}
	}

	for ( int n = 0; n < arrayAccountIDsToUnlink.GetCount(); n++)
	{
		dlgWorking.SetCaption2RecordsChecked(++nChecked, FALSE);
		CSQLRepositoryPINNumber RepoPIN;
		RepoPIN.RemoveAccountIDLink(_atoi64(arrayAccountIDsToUnlink.GetAt(n)), NULL);	
	}

	dlgWorking.DestroyWindow();

	int nOrphans = arrayAccountIDsToUnlink.GetCount();
	CString strMsg = "Check complete:\n\n";
	
	switch (nOrphans)
	{
	case 0:
		strMsg += "No PINs were linked to unknown accounts.";
		break;

	case 1:
		strMsg += "1 PIN was linked to an unknown account.\n\nThis PIN has been returned to the pool";
		break;

	default:
		strMsg.AppendFormat("%d PINs were linked to unknown accounts.\n\nThese PINs have been returned to the pool.",
			nOrphans);
	}

	Prompter.Info(strMsg);
}

//*******************************************************************

void CPINAdminDlg::OnOK()
{
	EndDialog(IDCANCEL);
}

//*******************************************************************

void CPINAdminDlg::OnCancel()
{
	EndDialog(IDCANCEL);
}

//*******************************************************************

void CPINAdminDlg::CSSAutoShutdownLogOff()
{
	OnCancel();
}

//*******************************************************************
