//*******************************************************************
#include "GroupComboHelpers.h"
#include "SQLTranLinkPINNumbers.h"
#include "SQLTranUnlinkPINNumbers.h"
//*******************************************************************
#include "PINRangeDlg.h"
//*******************************************************************

CPINRangeDlg::CPINRangeDlg( CWnd* pParent)
	: CSSAutoShutdownDialog( AutoShutdownHelper, CPINRangeDlg::IDD, pParent)
{
}

//*******************************************************************

void CPINRangeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_KEEP, m_checkKeep);
	DDX_Control(pDX, IDC_COMBO_ADD_GROUP, m_tabcomboGroupAdd);
	DDX_Control(pDX, IDC_COMBO_REMOVE_GROUP, m_tabcomboGroupRemove);
	DDX_Control(pDX, IDC_EDIT_ADD_FROM, m_editCardNoAddFrom);
	DDX_Control(pDX, IDC_EDIT_ADD_TO, m_editCardNoAddTo);
	DDX_Control(pDX, IDC_EDIT_REMOVE_FROM, m_editCardNoRemoveFrom);
	DDX_Control(pDX, IDC_EDIT_REMOVE_TO, m_editCardNoRemoveTo);
}

//*******************************************************************

BEGIN_MESSAGE_MAP(CPINRangeDlg, CSSAutoShutdownDialog)
	//{{AFX_MSG_MAP(CPINRangeDlg)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE, OnButtonRemove)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//*******************************************************************

BOOL CPINRangeDlg::CSSAutoShutdownPostInitDialog()
{
	m_checkKeep.SetCheck(TRUE);
	CGroupComboHelpers::FillGroupCombo(m_tabcomboGroupAdd, 0, TRUE);
	CGroupComboHelpers::FillGroupCombo(m_tabcomboGroupRemove, 0, TRUE);

	m_editCardNoAddFrom.LimitText(System.GetUserIDLength());
	m_editCardNoAddFrom.SetWindowText(System.GetLowCardNo());

	m_editCardNoAddTo.LimitText(System.GetUserIDLength());
	m_editCardNoAddTo.SetWindowText(System.GetHighCardNo());

	m_editCardNoRemoveFrom.LimitText(System.GetUserIDLength());
	m_editCardNoRemoveFrom.SetWindowText(System.GetLowCardNo());

	m_editCardNoRemoveTo.LimitText(System.GetUserIDLength());
	m_editCardNoRemoveTo.SetWindowText(System.GetHighCardNo());

	return TRUE;
}

//*******************************************************************

void CPINRangeDlg::OnButtonAdd()
{
	int nGroupNo = CGroupComboHelpers::GetGroupNoFromCombo(m_tabcomboGroupAdd, 0);
	__int64 nFrom = GetEditBoxInt64(m_editCardNoAddFrom);
	__int64 nTo = GetEditBoxInt64(m_editCardNoAddTo);
	bool bKeepExisting = IsTicked(m_checkKeep);

	CStringArray arrayWarning;
	arrayWarning.Add("Link PIN Numbers to Accounts");
	arrayWarning.Add("New PIN numbers will be linked to the selected accounts.");
	
	if (IsTicked(m_checkKeep))
	{
		arrayWarning.Add("Any Existing PIN numbers for these accounts will be retained.");
	}
	else
	{
		arrayWarning.Add("Any Existing PIN numbers for these accounts will be overwritten.");
	}

	if ( Prompter.Warning(arrayWarning, FALSE) != IDYES )
	{
		return;
	}

	CSQLTranLinkPINNumbers Tran;
	Tran.BeginTrans();
	Tran.DoWork(bKeepExisting, nGroupNo, nFrom, nTo);

	switch (Tran.EndTrans())
	{
	case SQLTRAN_STATE_COMMITOK:
		{
			CString strMsg = "";
			strMsg.Format("PIN numbers have been linked to accounts as requested.\n\nNew PINs linked: %d", 
				Tran.m_nAdded);

			if ( TRUE == bKeepExisting )
			{
				strMsg.AppendFormat(", Existing PINs retained: %d", 
					Tran.m_nRetained);
			}
		
			Prompter.Info(strMsg);	
		}
		break;

	default:
		Tran.ShowErrorMessage("Link PIN Numbers");
		break;
	}
}

//*******************************************************************

void CPINRangeDlg::OnButtonRemove()
{
	int nGroupNo = CGroupComboHelpers::GetGroupNoFromCombo(m_tabcomboGroupRemove, 0);
	__int64 nFrom = GetEditBoxInt64(m_editCardNoRemoveFrom);
	__int64 nTo = GetEditBoxInt64(m_editCardNoRemoveTo);

	CStringArray arrayWarning;
	arrayWarning.Add("Remove PIN Numbers from Accounts");
	arrayWarning.Add("PIN numbers will be removed from the selected accounts.");
	arrayWarning.Add("Any PINs that are removed will be returned to the pool.");

	if (Prompter.Warning(arrayWarning, FALSE) != IDYES)
	{
		return;
	}

	CSQLTranUnlinkPINNumbers Tran;
	Tran.BeginTrans();
	Tran.DoWork(nGroupNo, nFrom, nTo);

	switch (Tran.EndTrans())
	{
	case SQLTRAN_STATE_COMMITOK:
		{
			CString strMsg = "";
			strMsg.Format("PIN numbers have been removed from accounts as requested.\n\nPINs removed: %d",
				Tran.m_nRemoved);

			Prompter.Info(strMsg);
		}
		break;

	default:
		Tran.ShowErrorMessage("Remove PIN Numbers from Accounts");
		break;
	}
}

//*******************************************************************

void CPINRangeDlg::OnOK()
{
	EndDialog(IDCANCEL);
}

//*******************************************************************

void CPINRangeDlg::OnCancel()
{
	EndDialog(IDCANCEL);
}

//*******************************************************************

void CPINRangeDlg::CSSAutoShutdownLogOff()
{
	OnCancel();
}

//*******************************************************************
