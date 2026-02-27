//$$******************************************************************
#include "..\SmartPay4Shared\EODTextData.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\PasswordData.h"
#include "..\SmartPay4Shared\SQLPurse2Handler.h"
#include "..\SmartPay4Shared\SQLTable_Group\SQLRepositoryGroup.h"
//$$******************************************************************
#include "ReportDefaultHandler.h"
#include "resource.h"
//$$******************************************************************
#include "CreditAdjustDlg.h"
//$$******************************************************************

CCreditAdjustOutput::CCreditAdjustOutput()
{
	m_strCommentUp = "";
	m_strCommentDown = "";
	m_dPurse1Credit = 0.0;
	m_dPurse3Credit = 0.0;
}

//$$******************************************************************

bool CCreditAdjustOutput::GotCredit()
{
	if (m_dPurse1Credit != 0.0) return TRUE;
	if (m_dPurse3Credit != 0.0) return TRUE;
	return FALSE;
}

//$$******************************************************************
//$$******************************************************************
//$$******************************************************************

CCreditAdjustDlg::CCreditAdjustDlg(CSQLRowAccountFull& RowAccount, CWnd* pParent)
	: CSSAutoShutdownDialog(AutoShutdownHelper, CCreditAdjustDlg::IDD, pParent),
	m_RowAccount(RowAccount)
{
	m_pParent = pParent;
	m_strReportLabel = "R802";

	m_strTitle.Format("Credit Adjustment : UserID %s", (const char*) System.FormatCardNo(m_RowAccount.GetUserIDString()));

	m_dPurse1Starting = m_RowAccount.GetPurse1Credit();
	m_dPurse3Starting = m_RowAccount.GetPurse3Credit();

	m_dateTransaction.SetCurrentDate();
	m_timeTransaction.SetCurrentTime();;

	m_RowGroup.SetGroupNo(m_RowAccount.GetGroupNo());

	CSQLRepositoryGroup RepoGroup;
	m_bKnownGroup = (RepoGroup.SelectRow(m_RowGroup, NULL).GetSQLError() == SQLCRUD_ERR_NONE);
}

//$$******************************************************************

CCreditAdjustDlg::~CCreditAdjustDlg()
{
}

//$$******************************************************************

void CCreditAdjustDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	
	DDX_Control(pDX, IDC_EDIT_USERNAME, m_editUserName);
	DDX_Control(pDX, IDC_EDIT_GROUPNAME, m_editGroupName);
	DDX_Control(pDX, IDC_EDIT_PURSE1_BAL, m_editPurse1Balance);
	DDX_Control(pDX, IDC_EDIT_PURSE3_BAL, m_editPurse3Balance);
	DDX_Control(pDX, IDC_EDIT_PURSE1DATELASTUSED, m_editPurse1Date);
	DDX_Control(pDX, IDC_EDIT_PURSE3DATELASTUSED, m_editPurse3Date);
	DDX_Control(pDX, IDC_EDIT_COMMENT_UP, m_editCommentUp);
	DDX_Control(pDX, IDC_EDIT_COMMENT_DOWN, m_editCommentDown);
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CCreditAdjustDlg, CSSAutoShutdownDialog)
	ON_EN_KILLFOCUS(IDC_EDIT_PURSE1_CREDIT, OnKillFocusPurse1Credit)
	ON_EN_KILLFOCUS(IDC_EDIT_PURSE3_CREDIT, OnKillFocusPurse3Credit)
END_MESSAGE_MAP()

//$$******************************************************************

void CCreditAdjustDlg::CSSAutoShutdownPreInitDialog()
{
	SubclassEdit(IDC_EDIT_PURSE1_CREDIT, SS_NUM_SDP, 11, "%.2f");
	SubclassEdit(IDC_EDIT_PURSE3_CREDIT, SS_NUM_SDP, 11, "%.2f");		
}

//$$******************************************************************

BOOL CCreditAdjustDlg::CSSAutoShutdownPostInitDialog()
{
	SetWindowText(m_strTitle);

	m_editUserName.SetWindowText(m_RowAccount.GetUsername());

	if (TRUE == m_bKnownGroup)
	{
		m_editGroupName.SetWindowText(m_RowGroup.GetListboxText());
	}
	else
	{
		CString strText = "";
		strText.Format("%d  :  Unknown", m_RowGroup.GetGroupNo());
		m_editGroupName.SetWindowText(strText);
	}

	DisplayBalance(GetEditPurse1Credit(), 0.0);
	DisplayBalance(GetEditPurse3Credit(), 0.0);
	DisplayBalance(&m_editPurse1Balance, m_dPurse1Starting);
	DisplayBalance(&m_editPurse3Balance, m_dPurse3Starting);
	GetEditPurse3Credit()->EnableWindow(System.GetEnablePurse3Flag());

	{
		CString strDate = m_RowAccount.GetPurse1LastSpendDate();

		if (strDate != "")
		{
			strDate += " @ ";
			strDate += m_RowAccount.GetPurse1LastSpendTime();
		}

		m_editPurse1Date.SetWindowText(strDate);
	}

	{
		CString strDate = m_RowAccount.GetPurse3LastSpendDate();

		if (strDate != "")
		{
			strDate += " @ ";
			strDate += m_RowAccount.GetPurse3LastSpendTime();
		}

		m_editPurse3Date.SetWindowText(strDate);
	}

	{
		CString strLine = "";
		CString strCommentUp = "";
		CString strCommentDown = "";

		CReportDefaultHandler repdefs;
		if (repdefs.Read(m_strReportLabel, strLine) == TRUE)
		{
			CCSV csv(strLine);
			strCommentUp = csv.GetString(0);
			strCommentDown = csv.GetString(1);
		}

		if ((strCommentUp == "") || (strCommentDown == ""))
		{
			CEODTextData eodText;
			eodText.Read();

			if (strCommentUp == "")
			{
				strCommentUp = eodText.GetText(nCREDITADJUSTPCUP);
			}

			if (strCommentDown == "")
			{
				strCommentDown = eodText.GetText(nCREDITADJUSTPCDOWN);
			}
		}

		if (strCommentUp == "")
		{
			strCommentUp = "Credit Adjust (+)";
		}

		if (strCommentDown == "")
		{
			strCommentDown = "Credit Adjust (-)";
		}


		m_editCommentUp.SetWindowText(strCommentUp);
		m_editCommentDown.SetWindowText(strCommentDown);
	}

	return TRUE;
}

//$$******************************************************************
CEdit* CCreditAdjustDlg::GetEditPurse1Credit() { return GetEdit(IDC_EDIT_PURSE1_CREDIT); }
CEdit* CCreditAdjustDlg::GetEditPurse3Credit() { return GetEdit(IDC_EDIT_PURSE3_CREDIT); }
//$$******************************************************************

void CCreditAdjustDlg::OnKillFocusPurse1Credit()
{
	double dCredit = atof(GetEditBoxText(*GetEditPurse1Credit()));
	DisplayBalance(&m_editPurse1Balance, m_dPurse1Starting + dCredit);
}

//$$******************************************************************

void CCreditAdjustDlg::OnKillFocusPurse3Credit()
{
	double dCredit = atof(GetEditBoxText(*GetEditPurse3Credit()));
	DisplayBalance(&m_editPurse3Balance, m_dPurse3Starting + dCredit);
}

//$$******************************************************************

void CCreditAdjustDlg::DisplayBalance( CEdit* pEdit, double d )
{
	CString strBalance = "";
	strBalance.Format("%.2f", d);
	pEdit->SetWindowText(strBalance);
}

//$$******************************************************************

bool CCreditAdjustDlg::UpdateAccount(CSQLRowAccountFull& RowAccount)
{
	if (m_CreditAdjustOutput.GotCredit() == FALSE)
	{
		return FALSE;
	}

	CString strCardNo = m_RowAccount.GetUserIDString();

	/*
	if (::IsAccountInuse(strCardNo) == TRUE)
	{
		CString strMsg = "";
		strMsg.Format("Account %s is already in use - update ignored!", strCardNo);
		Prompter.Error(strMsg, "Database Access");
		return FALSE;
	}
	*/

	m_atcRecord.Set(&m_RowAccount);
	m_atcRecord.SetSourceType(AUDIT_PC);
	m_atcRecord.SetOperatorID(Passwords.GetPasswordInUse());
	m_atcRecord.SetApplicationNo(APPNO_CREDIT);
	m_atcRecord.SetLineComment(m_CreditAdjustOutput.m_strCommentUp);
	m_atcRecord.SetTransactionPurse1Credit(0);
	m_atcRecord.SetTransactionPurse1Liability(0);
	m_atcRecord.SetTransactionPurse2Balance(0);
	m_atcRecord.SetTransactionPurse3Credit(0);
	m_atcRecord.SetTransactionPurse3Liability(0);

	CString strTrnDate = m_dateTransaction.GetDate();
	CString strTrnTime = m_timeTransaction.GetTime();

	{
		bool bGotCreditUp = FALSE;

		if (m_CreditAdjustOutput.m_dPurse1Credit > 0.0)
		{
			m_atcRecord.SetTransactionPurse1Credit(m_CreditAdjustOutput.m_dPurse1Credit);
			m_RowAccount.AddToPurse1Credit(m_CreditAdjustOutput.m_dPurse1Credit);
			bGotCreditUp = TRUE;
		}

		if (m_CreditAdjustOutput.m_dPurse3Credit > 0.0)
		{
			m_atcRecord.SetTransactionPurse3Credit(m_CreditAdjustOutput.m_dPurse3Credit);
			m_RowAccount.AddToPurse3Credit(m_CreditAdjustOutput.m_dPurse3Credit);
			bGotCreditUp = TRUE;
		}

		if (TRUE == bGotCreditUp)
		{
			SaveAudit();
		}
	}

	m_atcRecord.SetTransactionPurse1Credit(0);
	m_atcRecord.SetTransactionPurse3Credit(0);
	m_atcRecord.SetLineComment(m_CreditAdjustOutput.m_strCommentDown);

	{
		bool bGotCreditDown = FALSE;

		if (m_CreditAdjustOutput.m_dPurse1Credit < 0.0)
		{
			m_atcRecord.SetTransactionPurse1Credit(m_CreditAdjustOutput.m_dPurse1Credit);
			m_RowAccount.AddToPurse1Credit(m_CreditAdjustOutput.m_dPurse1Credit);
			bGotCreditDown = TRUE;
		}

		if (m_CreditAdjustOutput.m_dPurse3Credit < 0.0)
		{
			m_atcRecord.SetTransactionPurse3Credit(m_CreditAdjustOutput.m_dPurse3Credit);
			m_RowAccount.AddToPurse3Credit(m_CreditAdjustOutput.m_dPurse3Credit);
			bGotCreditDown = TRUE;
		}

		CSQLRepositoryAccount RepoAccount;
		RepoAccount.UpdateRow(m_RowAccount,NULL);
		
		if (TRUE == bGotCreditDown)
		{
			SaveAudit();
		}
	}


	return TRUE;
}

//$$******************************************************************

void CCreditAdjustDlg::SaveAudit()
{
	m_atcRecord.SetCurrentPurse1Liability(m_RowAccount.GetPurse1Liability());
	m_atcRecord.SetCurrentPurse1Credit(m_RowAccount.GetPurse1Credit());
	m_atcRecord.SetCurrentPurse2Balance(m_RowAccount.GetPurse2Balance());
	m_atcRecord.SetCurrentPurse3Liability(m_RowAccount.GetPurse3Liability());
	m_atcRecord.SetCurrentPurse3Credit(m_RowAccount.GetPurse3Credit());
	m_atcRecord.SetToDateCashSpend(m_RowAccount.GetCashSpendToDate());
	m_atcRecord.SetToDatePurse1Spend(m_RowAccount.GetPurse1SpendToDate());
	m_atcRecord.SetToDatePurse2Spend(m_RowAccount.GetPurse2SpendToDate());
	m_atcRecord.SetToDatePurse3Spend(m_RowAccount.GetPurse3SpendToDate());
	m_atcRecord.SaveNewLine();
}

//$$******************************************************************

void CCreditAdjustDlg::AddCreditMessage(CString& strMsg, int nPurse, double dAmount)
{
	if (dAmount == 0.0)
	{
		return;
	}
	else
	{
		if (strMsg != "")
		{
			strMsg += " AND\n";
		}

		CString strLine = "";
		if (dAmount > 0.0)
		{
			strLine.Format("Apply a credit of %.2lf to purse %d", dAmount, nPurse);
		}
		else
		{
			strLine.Format("Apply a minus credit of %.2lf to purse %d", dAmount * (-1), nPurse);
		}

		strMsg += strLine;
	}
}

//$$******************************************************************

bool CCreditAdjustDlg::CheckAdjust(int nPurse, double dCurrentCredit, double dNewCredit)
{
	bool bResult = TRUE;

	if (dNewCredit < 0.0)
	{
		if ((dCurrentCredit + dNewCredit) < 0.0)
		{
			CString strMsg = "";
			strMsg.Format("The current Purse%d credit total is %.2lf\n\nYou cannot apply a MINUS credit of %.2lf at this time\nas the resulting total would be negative.",
				nPurse,
				dCurrentCredit,
				dNewCredit * (-1));

			Prompter.Error(strMsg);
			bResult = FALSE;
		}
	}
	return bResult;
}

//$$******************************************************************

void CCreditAdjustDlg::OnOK()
{
	m_CreditAdjustOutput.m_strCommentUp = GetEditBoxText(m_editCommentUp);
	m_CreditAdjustOutput.m_strCommentDown = GetEditBoxText(m_editCommentDown);
	m_CreditAdjustOutput.m_dPurse1Credit = atof(GetEditBoxText(*GetEditPurse1Credit()));
	m_CreditAdjustOutput.m_dPurse3Credit = atof(GetEditBoxText(*GetEditPurse3Credit()));

	if (m_CreditAdjustOutput.GotCredit() == FALSE)
	{
		Prompter.Info("No credits have been applied.");
		GetEditPurse1Credit()->SetFocus();
		GetEditPurse1Credit()->SetSel(0, -1);
		return;
	}

	if (CheckAdjust(1, m_RowAccount.GetPurse1Credit(), m_CreditAdjustOutput.m_dPurse1Credit) == FALSE )
	{
		return;
	}

	if (CheckAdjust(3, m_RowAccount.GetPurse3Credit(), m_CreditAdjustOutput.m_dPurse3Credit) == FALSE)
	{
		return;
	}

	CString strMsg = "";
	AddCreditMessage(strMsg, 1, m_CreditAdjustOutput.m_dPurse1Credit);
	AddCreditMessage(strMsg, 3, m_CreditAdjustOutput.m_dPurse3Credit);

	int nReply = Prompter.YesNoCancel(strMsg, m_strTitle, TRUE);

	if (nReply == IDCANCEL)
	{
		return;
	}

	if (nReply == IDYES)
	{
		CCSV csv;
		csv.Add(m_CreditAdjustOutput.m_strCommentUp);
		csv.Add(m_CreditAdjustOutput.m_strCommentDown);

		CReportDefaultHandler repdefs;
		repdefs.Save(m_strReportLabel, &csv);

		EndDialog(IDOK);
	}
	else
	{
		CSSAutoShutdownDialog::OnCancel();
	}
}

//$$******************************************************************

