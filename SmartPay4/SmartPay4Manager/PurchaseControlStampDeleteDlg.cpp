//*******************************************************************
#include "..\SmartPay4Shared\SQLTable_PurchaseControlStamp\SQLRepositoryPurchaseControlStamp.h"
#include "..\SmartPay4Shared\SQLTable_PurchaseControlStamp\SQLRecordSetPurchaseControlStamp.h"
//*******************************************************************
#include "..\SmartPay4Shared\SQLTable_PurchaseControlRule\SQLRepositoryPurchaseControlRule.h"
//*******************************************************************
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
//*******************************************************************
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
//*******************************************************************
#include "..\SmartPay4Shared\SQLAuditRecord_base.h"
#include "..\SmartPay4Shared\SQLPurchaseControlBuffer.h"
//*******************************************************************
#include "SQLTranStampManualEdit.h"
//*******************************************************************
#include "PurchaseControlStampDeleteDlg.h"
//*******************************************************************

CPurchaseControlStampDeleteDlg::CPurchaseControlStampDeleteDlg(bool bAllAccounts, __int64 nUserID, int nRuleID, CWnd* pParent)
	: CSSAutoShutdownDialog(AutoShutdownHelper, CPurchaseControlStampDeleteDlg::IDD, pParent)
{
	m_bAllAccounts = bAllAccounts;
	m_nUserID = nUserID;
	m_nRuleID = nRuleID;
}

//*******************************************************************

void CPurchaseControlStampDeleteDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_ACTION1, m_checkAction1);
	DDX_Control(pDX, IDC_CHECK_ACTION2, m_checkAction2);
}

//*******************************************************************
BEGIN_MESSAGE_MAP(CPurchaseControlStampDeleteDlg, CSSAutoShutdownDialog)
END_MESSAGE_MAP()
//*******************************************************************

BOOL CPurchaseControlStampDeleteDlg::CSSAutoShutdownPostInitDialog()
{
	CString strText = "";
	strText.Format("Delete Expired Stamps : Purchase Ctrl %d", m_nRuleID);

	if (TRUE == m_bAllAccounts)
	{
		strText += ", All Accounts";
	}
	else
	{
		CString strAccount = "";
		strAccount.Format(", Account %I64d", m_nUserID);
		strText += strAccount;
	}

	ShowAndEnableWindow(&m_checkAction2, FALSE);

	SetWindowText(strText);

	return TRUE;
}

//*******************************************************************

CString CPurchaseControlStampDeleteDlg::GetAccountFilter()
{
	CString strFilter = "";

	if (FALSE == m_bAllAccounts)
	{
		strFilter += PurchaseControlStamp::UserID.Label;
		strFilter += " = ";

		CString strUserID = "";
		strUserID.Format("%I64d", m_nUserID);
		strFilter += strUserID;

		strFilter += " AND ";
	}

	return strFilter;
}

//*******************************************************************

void CPurchaseControlStampDeleteDlg::OnOK()
{
	if ((IsTicked(m_checkAction1) == FALSE) && (IsTicked(m_checkAction2) == FALSE))
	{
		Prompter.Error("Please select at least one action to perform.");
		return;
	}

	if (Prompter.YesNo("Are you sure you wish to delete the selected stamps?") != IDYES)
	{
		return;
	}

	m_strResults = "";

	if (IsTicked(m_checkAction1))
	{
		DeleteExpiredStamps();
	}

	/*FIDDLE
	if (IsTicked(m_checkAction2))
	{
		DeletePreviousEditionStamps();
	}
	*/

	if (m_strResults != "")
	{
		Prompter.Info(m_strResults);
		EndDialog(IDOK);
	}
	else
	{
		Prompter.Info("No stamps were deleted.");
		EndDialog(IDCANCEL);
	}
}

//*******************************************************************

void CPurchaseControlStampDeleteDlg::DeleteExpiredStamps()
{
	COleDateTime dateToday = COleDateTime::GetCurrentTime();

	CString strDateToday = "";
	strDateToday.Format("%4.4d%2.2d%2.2d",
		dateToday.GetYear(),
		dateToday.GetMonth(),
		dateToday.GetDay());

	CSQLRecordSetPurchaseControlStamp RecordSet(NULL, RSParams_PurchaseControlStamp_NormalNoParams());

	RecordSet.m_strFilter.Format("%s%s < %s AND LEN (%s) = 8 AND %s = %d",
		(const char*) GetAccountFilter(),
		PurchaseControlStamp::ExpireDate.Label,
		(const char*) strDateToday,
		PurchaseControlStamp::ExpireDate.Label,
		PurchaseControlStamp::RuleID.Label,
		m_nRuleID);

	int nRows = 0;
	int nStamps = 0;
	DeleteRecords(RecordSet, APPNO_SO_MDEL_EXPIRE, nRows, nStamps);

	if (nRows > 0)
	{
		CString strDetail = "";
		strDetail.Format("Deleted %d expired stamp%s from %d row%s", 
			nStamps,
			( 1 == nStamps ) ? "" : "s",
			nRows,
			( 1 == nRows ) ? "" : "s" );

		m_strResults += strDetail;
	}
}

//*******************************************************************

void CPurchaseControlStampDeleteDlg::DeletePreviousEditionStamps()
{
}

//*******************************************************************

void CPurchaseControlStampDeleteDlg::OnCancel()
{
	EndDialog(IDCANCEL);
}

//*******************************************************************
//*******************************************************************
//*******************************************************************

void CPurchaseControlStampDeleteDlg::AutoDeleteExpiredStamps()
{
	int nDays = 0;
	switch (System.GetAutoDeletePCtrlType())
	{
	case 1:
		nDays = 2;
		break;

	case 2:
		nDays = 5;
		break;

	case 3:
		nDays = 7;
		break;

	case 4:
		nDays = 14;
		break;

	case 5:
		nDays = 21;
		break;

	case 6:
		nDays = 28;
		break;

	case 7:
		nDays = 50;
		break;

	case 8:
		nDays = 100;
		break;
	}

	if (0 == nDays)
	{
		return;
	}

	COleDateTime dateExpire = COleDateTime::GetCurrentTime();
	dateExpire -= COleDateTimeSpan(nDays, 0, 0, 0);

	CString strDateExpire = "";
	strDateExpire.Format("%4.4d%2.2d%2.2d",
		dateExpire.GetYear(),
		dateExpire.GetMonth(),
		dateExpire.GetDay());

	CSQLRecordSetPurchaseControlStamp RecordSet(NULL, RSParams_PurchaseControlStamp_NormalNoParams());

	RecordSet.m_strFilter.Format("%s < %s AND LEN (%s) = 8",
		PurchaseControlStamp::ExpireDate.Label,
		(const char*)strDateExpire,
		PurchaseControlStamp::ExpireDate.Label );

	int nRows = 0;
	int nStamps = 0;
	DeleteRecords(RecordSet, APPNO_SO_XDEL_EXPIRE, nRows, nStamps);
}

//*******************************************************************

void CPurchaseControlStampDeleteDlg::DeleteRecords(CSQLRecordSetPurchaseControlStamp& RecordSet, int nAppNo, int& nRows, int& nStamps)
{
	nRows = 0;
	nStamps = 0;

	CArray<CSQLRowPurchaseControlStamp, CSQLRowPurchaseControlStamp> arrayStamps;

	CSQLRowPurchaseControlStamp RowOS;
	while (RecordSet.StepSelectAll(RowOS) == TRUE)
	{
		arrayStamps.Add(RowOS);
	}

	CSQLRepositoryPurchaseControlRule RepoRule;
	CSQLRepositoryPurchaseControlStamp RepoStamp;

	for (int i = 0; i < arrayStamps.GetSize(); i++)
	{
		if (RepoStamp.DeleteRow(arrayStamps[i], NULL).GetSQLError() == SQLCRUD_ERR_NONE)
		{
			SQLPurchaseControlBuffer.Reset();

			CSQLRepositoryAccount RepoAccount;

			CSQLRowAccountFull RowAccount;
			RowAccount.SetUserID(arrayStamps[i].GetUserID());
			RepoAccount.SelectRow(RowAccount, NULL);


			SQLPurchaseControlBuffer.BufferPCAction(
				nAppNo,
				RowAccount,
				arrayStamps[i],
				RepoRule.GetRuleName(RowOS.GetRuleID(), NULL),
				arrayStamps[i].GetStampCount() * (-1),
				0
			);

			SQLPurchaseControlBuffer.WritePCAuditLines(NULL);

			nRows++;
			nStamps += arrayStamps[i].GetStampCount();
		}
	}
}

//*******************************************************************
