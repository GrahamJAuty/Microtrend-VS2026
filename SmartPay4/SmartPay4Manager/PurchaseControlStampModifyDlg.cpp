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
#include "PurchaseControlStampModifyDlg.h"
//*******************************************************************

CPurchaseControlStampModifyDlg::CPurchaseControlStampModifyDlg(bool bAllAccounts, __int64 nUserID, int nRuleID, CWnd* pParent /*=NULL*/)
	: CSSAutoShutdownDialog(AutoShutdownHelper, CPurchaseControlStampModifyDlg::IDD, pParent)
{
	m_bAllAccounts = bAllAccounts;
	m_nUserID = nUserID;
	m_nRuleID = nRuleID;
}

//*******************************************************************

void CPurchaseControlStampModifyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_ACTION1, m_checkAction1);
	DDX_Control(pDX, IDC_CHECK_ACTION2, m_checkAction2);
	DDX_Control(pDX, IDC_DATEPICKER_CURRENT_FROM, m_DatePickerCurrentFrom);
	DDX_Control(pDX, IDC_DATEPICKER_CURRENT_TO, m_DatePickerCurrentTo);
	DDX_Control(pDX, IDC_CHECK_CURRENT_NOEXPIRE, m_checkCurrentNoExpire);
	DDX_Control(pDX, IDC_DATEPICKER_NEW, m_DatePickerNew);
	DDX_Control(pDX, IDC_COMBO_NEWEXPIRY, m_comboNewExpiry);
}

//*******************************************************************
BEGIN_MESSAGE_MAP(CPurchaseControlStampModifyDlg, CSSAutoShutdownDialog)
	ON_BN_CLICKED(IDC_CHECK_ACTION1, OnToggleAction1)
	ON_CBN_SELCHANGE(IDC_COMBO_NEWEXPIRY, OnSelectNewExpiry)
	ON_STN_CLICKED(IDC_STATIC_TO, OnClickStaticTo)
END_MESSAGE_MAP()
//*******************************************************************

BOOL CPurchaseControlStampModifyDlg::CSSAutoShutdownPostInitDialog()
{
	CString strText = "";
	strText.Format("Modify Expiry for Stamps : Purchase Ctrl %d", m_nRuleID);
	
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

	m_DatePickerCurrentFrom.SetFormat("ddd dd MMM yyy");
	m_DatePickerCurrentTo.SetFormat("ddd dd MMM yyy");
	m_DatePickerNew.SetFormat("ddd dd MMM yyy");

	COleDateTime dateNow = COleDateTime::GetCurrentTime();
	COleDateTime dateMinExpire = dateNow;;
	COleDateTime dateMaxExpire = dateNow;
	dateMinExpire -= COleDateTimeSpan(1, 0, 0, 0);
	dateMaxExpire += COleDateTimeSpan(PurchaseControlRule::Expiry.Max, 0, 0, 0);

	m_DatePickerNew.SetRange(&dateMinExpire,&dateMaxExpire);
	m_DatePickerNew.SetTime(dateNow);

	COleDateTime dateMin = COleDateTime(2024, 1, 1, 0, 0, 0);
	m_DatePickerCurrentFrom.SetRange(&dateMin, &dateMaxExpire);
	m_DatePickerCurrentTo.SetRange(&dateMin, &dateMaxExpire);

	m_comboNewExpiry.AddString("Default expiry for control");
	m_comboNewExpiry.AddString("Fixed expiry date");
	m_comboNewExpiry.AddString("Never expire");
	m_comboNewExpiry.SetCurSel(0);

	SetWindowText(strText);
	OnToggleAction1();

	ShowAndEnableWindow(&m_checkAction2, FALSE);

	return TRUE;
}

//*******************************************************************

CString CPurchaseControlStampModifyDlg::GetAccountFilter()
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

void CPurchaseControlStampModifyDlg::OnToggleAction1()
{
	bool bEnable = IsTicked(m_checkAction1);
	m_DatePickerCurrentFrom.EnableWindow(bEnable);
	m_DatePickerCurrentTo.EnableWindow(bEnable);
	m_checkCurrentNoExpire.EnableWindow(bEnable);
	m_comboNewExpiry.EnableWindow(bEnable);
	m_DatePickerNew.EnableWindow(bEnable && ( m_comboNewExpiry.GetCurSel() == 1));
}

//*******************************************************************

void CPurchaseControlStampModifyDlg::OnSelectNewExpiry()
{
	m_DatePickerNew.EnableWindow(m_comboNewExpiry.GetCurSel() == 1);
}

//*******************************************************************

void CPurchaseControlStampModifyDlg::OnClickStaticTo()
{
	COleDateTime dateFrom;
	m_DatePickerCurrentFrom.GetTime(dateFrom);
	m_DatePickerCurrentTo.SetTime(dateFrom);
}

//*******************************************************************

void CPurchaseControlStampModifyDlg::OnOK()
{
	if ((IsTicked(m_checkAction1) == FALSE) && (IsTicked(m_checkAction2) == FALSE))
	{
		Prompter.Error("Please select at least one action to perform.");
		return;
	}

	if (Prompter.YesNo("Are you sure you wish to modify the selected stamps ?") != IDYES)
	{
		return;
	}

	m_strResults = "";

	if (IsTicked(m_checkAction1))
	{
		ModifyStampExpiryDates();
	}

	if (IsTicked(m_checkAction2))
	{
		MakeAllStampsCurrentEdition();
	}

	if (m_strResults != "")
	{
		Prompter.Info(m_strResults);
		EndDialog(IDOK);
	}
	else
	{
		Prompter.Info("No stamps were modified.");
		EndDialog(IDCANCEL);
	}
}

//*******************************************************************

void CPurchaseControlStampModifyDlg::ModifyStampExpiryDates()
{
	CSQLRepositoryPurchaseControlRule RepoRule;

	CSQLRowPurchaseControlRule RowRule;
	RowRule.SetRuleID(m_nRuleID);
	if (RepoRule.SelectRow(RowRule, NULL).GetSQLError() != SQLCRUD_ERR_NONE)
	{
		return;
	}

	CSQLRecordSetPurchaseControlStamp RecordSet(NULL, RSParams_PurchaseControlStamp_NormalNoParams());

	COleDateTime dateFrom, dateTo;
	m_DatePickerCurrentFrom.GetTime(dateFrom);
	m_DatePickerCurrentTo.GetTime(dateTo);

	CString strDateFrom = "";
	strDateFrom.Format("%4.4d%2.2d%2.2d",
				dateFrom.GetYear(),
				dateFrom.GetMonth(),
				dateFrom.GetDay());	

	CString strDateTo = "";
	strDateTo.Format("%4.4d%2.2d%2.2d",
						dateTo.GetYear(),
						dateTo.GetMonth(),
						dateTo.GetDay());

	RecordSet.m_strFilter.Format("%s%s = %d AND LEN(%s) = 8 AND (( %s >= %s AND %s <= %s ) OR ( %s = %s ) )",
		(const char*)GetAccountFilter(),
		PurchaseControlStamp::RuleID.Label,
		m_nRuleID,
		PurchaseControlStamp::ExpireDate.Label,
		PurchaseControlStamp::ExpireDate.Label,
		(const char*)strDateFrom,
		PurchaseControlStamp::ExpireDate.Label,
		(const char*)strDateTo,
		PurchaseControlStamp::ExpireDate.Label,
		IsTicked(m_checkCurrentNoExpire) ? ((const char*) CSQLRowPurchaseControlStamp::GetNeverExpireDate()) : "00");

	CString strNewDate = "";
	switch (m_comboNewExpiry.GetCurSel())
	{
	case 1:
		{
			COleDateTime dateNew;
			m_DatePickerNew.GetTime(dateNew);

			strNewDate.Format("%4.4d%2.2d%2.2d",
				dateNew.GetYear(),
				dateNew.GetMonth(),
				dateNew.GetDay());
		}
		break;

	case 2:
		strNewDate = CSQLRowPurchaseControlStamp::GetNeverExpireDate();
		break;

	case 0:
	default:
		strNewDate = "";
		break;
	}

	int nRows = 0;
	int nStamps = 0;
	UpdateRecords(RecordSet, APPNO_SO_MFIX_EXPIRE, RowRule.GetExpiryDays(), strNewDate, nRows, nStamps);

	if (nRows > 0)
	{
		CString strDetail = "";
	
		strDetail.Format("Modified expiry dates for %d stamp%s in %d row%s",
						nStamps,
						(1 == nStamps) ? "" : "s",
						nRows,
						(1 == nRows) ? "" : "s");

		m_strResults += strDetail;
	}
}

//*******************************************************************

void CPurchaseControlStampModifyDlg::MakeAllStampsCurrentEdition()
{
}

//*******************************************************************

void CPurchaseControlStampModifyDlg::UpdateRecords(CSQLRecordSetPurchaseControlStamp& RecordSet, int nAppNo, int nEditionOrExpiry, CString strNewDate, int& nRows, int& nStamps)
{
	nRows = 0;
	nStamps = 0;

	CArray<CSQLRowPurchaseControlStamp, CSQLRowPurchaseControlStamp> arrayStamps;

	CSQLRowPurchaseControlStamp RowOS;
	while (RecordSet.StepSelectAll(RowOS) == TRUE)
	{
		arrayStamps.Add(RowOS);
	}

	CSQLRowAccountFull RowAccount;
	__int64 nCurrentUserID = -1;

	for (int i = 0; i < arrayStamps.GetSize(); i++)
	{
		CSQLRowPurchaseControlStamp RowStampsOld = arrayStamps[i];
		CSQLRowPurchaseControlStamp RowStampsNew = arrayStamps[i];

		if (nAppNo == APPNO_SO_MFIX_EXPIRE)
		{
			if (strNewDate != "")
			{
				RowStampsNew.SetExpireDate(strNewDate);
			}
			else if (0 == nEditionOrExpiry)
			{
				RowStampsNew.SetExpireDate(CSQLRowPurchaseControlStamp::GetNeverExpireDate());
			}
			else
			{
				CString strAwardDate = RowStampsOld.GetAwardDate();

				if ((strAwardDate.GetLength() == 8) && (::TestNumeric(strAwardDate) == TRUE))
				{
					COleDateTime dateAward = COleDateTime(
						atoi(strAwardDate.Left(4)),
						atoi(strAwardDate.Mid(4, 2)),
						atoi(strAwardDate.Right(2)),
						0, 0, 0);

					COleDateTime dateExpiry = dateAward;

					if (nEditionOrExpiry > 1)
					{
						dateExpiry += COleDateTimeSpan(nEditionOrExpiry - 1, 0, 0, 0);
					}
					
					CString strExpiryDate = "";
					strExpiryDate.Format("%4.4d%2.2d%2.2d",
						dateExpiry.GetYear(),
						dateExpiry.GetMonth(),
						dateExpiry.GetDay());

					RowStampsNew.SetExpireDate(strExpiryDate);
				}
			}
		}
		/*
		else
		{
			RowStampsNew.SetEdition(nEditionOrExpiry);
		}
		*/

		SQLPurchaseControlBuffer.Reset();

		if (nCurrentUserID != RowStampsOld.GetUserID())
		{
			CSQLRepositoryAccount RepoAccount;
			RowAccount.SetUserID(RowStampsOld.GetUserID());
			if (RepoAccount.SelectRow(RowAccount, NULL).GetSQLError() != SQLCRUD_ERR_NONE)
			{
				nCurrentUserID = -1;
				continue;
			}
			else
			{
				nCurrentUserID = RowStampsOld.GetUserID();
			}
		}

		CSQLRepositoryPurchaseControlRule RepoRule;

		CSQLTranStampManualEdit Tran(nAppNo);
		Tran.BeginTrans();
		Tran.DoWork(RowAccount, RowStampsOld, RowStampsNew,
			RepoRule.GetRuleName(RowStampsOld.GetRuleID(), NULL));

		switch (Tran.EndTrans())
		{
		case SQLTRAN_STATE_COMMITOK:
			Tran.AfterTransaction();
			nRows++;
			nStamps += RowStampsOld.GetStampCount();
			break;

		default:
			break;
		}
	}
}

//*******************************************************************

void CPurchaseControlStampModifyDlg::OnCancel()
{
	EndDialog(IDCANCEL);
}

//*******************************************************************
