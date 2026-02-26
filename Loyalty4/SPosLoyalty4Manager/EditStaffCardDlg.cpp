//*******************************************************************
#include "FindDlg.h"
#include "LoyaltyManager.h"
#include "StandardCombos.h"
//*******************************************************************
#include "..\SPosLoyalty4Shared\SQLNamespaces.h"
#include "..\SPosLoyalty4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SPosLoyalty4Shared\SQLTable_EcrmanDatabase\SQLRepositoryEcrmanDatabase.h"
//*******************************************************************
#include "EditStaffCardDlg.h"
//*******************************************************************

CEditStaffCardDlg::CEditStaffCardDlg(CSQLRowStaffCard& RowSC, bool bEdit, CWnd* pParent)
	: CSSDialog(CEditStaffCardDlg::IDD, pParent), m_RowSC( RowSC )
{
	m_bEdit = bEdit;
}

//*******************************************************************

void CEditStaffCardDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_DATABASE, m_comboDatabase);
	DDX_Control(pDX, IDC_COMBO_FOLDERSET, m_comboFolderSet);
	DDX_Control(pDX, IDC_EDIT_NAME, m_editName);
}

//*******************************************************************

BEGIN_MESSAGE_MAP(CEditStaffCardDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_FIND,OnButtonFind)
	ON_EN_KILLFOCUS(IDC_EDIT_ACCOUNT,OnKillFocusAccount)
	ON_CBN_SELCHANGE(IDC_COMBO_DATABASE, OnSelectDatabase)
END_MESSAGE_MAP()

//*******************************************************************

BOOL CEditStaffCardDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	SubclassEdit( IDC_EDIT_SERVER, SS_NUM, 4, "%d" ); 
	SubclassEdit( IDC_EDIT_ACCOUNT, SS_NUM, System.GetMaxCardNoDlgLen(), "%s");
	SetEditBoxInt(*GetEditServer(), m_RowSC.GetServerNo());

	CStandardCombos::FillDatabaseCombo(m_comboDatabase, TRUE, m_RowSC.GetFolderIndexDbNo());
	CStandardCombos::FillFolderSetCombo(m_comboFolderSet,m_RowSC.GetFolderIndexDbNo(), m_RowSC.GetFolderIndexSetNo());

	GetEditAccount()->SetWindowText(m_RowSC.GetDisplayUserID());
	OnKillFocusAccount();

	if (TRUE == m_bEdit)
	{
		GetEditServer()->EnableWindow(FALSE);
		m_comboDatabase.EnableWindow(FALSE);
		m_comboFolderSet.EnableWindow(FALSE);
	}
	else
	{
		GetEditServer()->SetWindowText("");
		m_comboDatabase.SetCurSel(0);
		m_comboFolderSet.SetCurSel(0);
	}

	return TRUE;
}

//*******************************************************************

void CEditStaffCardDlg::OnSelectDatabase()
{
	CStandardCombos::FillFolderSetCombo(m_comboFolderSet,m_comboDatabase.GetCurSel(), m_comboFolderSet.GetCurSel());
}

//*******************************************************************
CEdit* CEditStaffCardDlg::GetEditServer() { return GetEdit(IDC_EDIT_SERVER); }
CEdit* CEditStaffCardDlg::GetEditAccount() { return GetEdit(IDC_EDIT_ACCOUNT); }
//*******************************************************************

void CEditStaffCardDlg::OnButtonFind()
{
	CFindDlg dlg{};
	dlg.SetTitle("Assign Staff Card To SPOS Server");

	if (dlg.DoModal() == IDOK)
	{
		SetEditBoxInt64(*GetEditAccount(), dlg.m_nRequestedCardNo);
		GetEditAccount() -> SetFocus();
		OnKillFocusAccount();
	}
}

//*******************************************************************

void CEditStaffCardDlg::OnKillFocusAccount()
{
	__int64 nUserID = GetEditBoxInt64(*GetEditAccount());

	CSQLRowAccountFull RowAccount;
	RowAccount.SetUserID(nUserID);

	CSQLRepositoryAccount repoAccount;
	if (repoAccount.SelectRow(RowAccount, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
	{
		m_editName.SetWindowText(RowAccount.GetFullname());
	}
	else
	{
		m_editName.SetWindowText("Unknown");
	}
}

//*******************************************************************

void CEditStaffCardDlg::OnOK()
{
	int nServerNo = GetEditBoxInt(*GetEditServer());
	__int64 nAccountID = GetEditBoxInt64(*GetEditAccount());

	if (FALSE == m_bEdit)
	{
		if ((nServerNo < StaffCards::ServerNo.Min) || (nServerNo > StaffCards::ServerNo.Max))
		{
			CString strError = "";
			strError.Format("Please enter a Server Number\nBetween %d and %d",
				StaffCards::ServerNo.Min,
				StaffCards::ServerNo.Max);

			Prompter.Error(strError);
			GetEditServer()->SetSel(0, -1);
			GetEditServer()->SetFocus();
			return;
		}
	}

	if ((nAccountID < StaffCards::UserID.Min) || (nServerNo > StaffCards::UserID.Max))
	{
		CString strError = "";
		strError.Format("Please enter an Account Number\nBetween %I64d and %I64d",
			StaffCards::UserID.Min,
			StaffCards::UserID.Max);

		Prompter.Error(strError);
		GetEditAccount()->SetSel(0, -1);
		GetEditAccount()->SetFocus();
		return;
	}

	if ( UpdateData() == TRUE )
	{
		if (FALSE == m_bEdit)
		{
			m_RowSC.SetServerNo(nServerNo);
			m_RowSC.SetFolderIndexDbNo(m_comboDatabase.GetCurSel());
			m_RowSC.SetFolderIndexSetNo(m_comboFolderSet.GetCurSel());
		}

		m_RowSC.SetUserID(nAccountID);
		m_RowSC.SetDisplayUserInfo();

		CDialog::OnOK();
	}
}

//*******************************************************************
