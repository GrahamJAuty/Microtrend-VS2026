//$$******************************************************************
#include "..\SPosLoyalty4Shared\SQLNamespaces.h"
//$$******************************************************************
#include "GroupComboHelpers.h"
#include "LoyaltyManager.h"
#include "SetupEposPathsDlg.h"
//$$******************************************************************
#include "ServerPropertyPage9.h"
//$$******************************************************************
extern CSysset Sysset;
//$$******************************************************************

CServerPropertyPage9::CServerPropertyPage9()
	: CSSPropertyPage(CServerPropertyPage9::IDD)
{
}

//$$******************************************************************

CServerPropertyPage9::~CServerPropertyPage9()
{
}

//$$******************************************************************

void CServerPropertyPage9::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_ADDUNKNOWNS, m_checkAddUnknowns);
	DDX_Control(pDX, IDC_CHECK_UNKNOWNCARDGROUP, m_checkUnknownCardGroup);
	DDX_Control(pDX, IDC_COMBO_IDMINLEN, m_comboIdMinLen);
	DDX_Control(pDX, IDC_COMBO_GROUP, m_comboGroup);
	DDX_Control(pDX, IDC_EDIT_PROMPT1, m_editPrompt1);
	DDX_Control(pDX, IDC_EDIT_PROMPT2, m_editPrompt2);
	DDX_Control(pDX, IDC_EDIT_PROMPT3, m_editPrompt3);
	DDX_Control(pDX, IDC_EDIT_PENDING, m_editPending);
	DDX_Control(pDX, IDC_EDIT_CREATE, m_editCreate);
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CServerPropertyPage9, CPropertyPage)
	ON_BN_CLICKED(IDC_CHECK_ADDUNKNOWNS, OnToggleAddUnknowns)
	ON_BN_CLICKED(IDC_CHECK_UNKNOWNCARDGROUP, OnToggleAddUnknowns)
	ON_EN_KILLFOCUS(IDC_EDIT_IDMIN, OnKillFocusEditIDMin)
	ON_EN_KILLFOCUS(IDC_EDIT_IDMAX, OnKillFocusEditIDMax)	
END_MESSAGE_MAP()

//$$******************************************************************

BOOL CServerPropertyPage9::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	SubclassEdit(IDC_EDIT_IDMIN, SS_NUM, 12, "%s");
	SubclassEdit(IDC_EDIT_IDMAX, SS_NUM, 12, "%s");
	m_editPrompt1.LimitText(MAX_UNKNOWN_ACCOUNT_PROMPTLEN);
	m_editPrompt2.LimitText(MAX_UNKNOWN_ACCOUNT_PROMPTLEN);
	m_editPrompt3.LimitText(MAX_UNKNOWN_ACCOUNT_PROMPTLEN);
	m_editPending.LimitText(MAX_UNKNOWN_ACCOUNT_NAMELEN);
	m_editCreate.LimitText(MAX_UNKNOWN_ACCOUNT_NAMELEN);

	/*****/

	m_checkAddUnknowns.SetCheck(Server.GetAddUnknownCardsFlag());
	m_checkUnknownCardGroup.SetCheck(Server.GetUnknownCardGroupRSPFlag());

	{
		CString str = "";
		str.Format("%12.12I64d", Server.GetUnknownCardMinID());
		GetEditIDMin()->SetWindowText(str);
	}

	{
		CString str = "";
		str.Format("%12.12I64d", Server.GetUnknownCardMaxID());
		GetEditIDMax()->SetWindowText(str);
	}

	int nMinLen = Server.GetUnknownCardMinLength();
	if ((nMinLen < 1) || (nMinLen > 12))
	{
		nMinLen = 1;
	}

	for (int n = 1; n <= 12; n++)
	{
		CString str = "";
		str.Format("%d digit", n);

		if (n > 1)
		{
			str += "s";
		}

		m_comboIdMinLen.AddString(str);
	}

	m_comboIdMinLen.SetCurSel(nMinLen - 1);

	int nGroupNo = Server.GetUnknownCardGroupNo();
	CGroupComboHelpers::FillGroupCombo(m_comboGroup, nGroupNo, FALSE);

	m_editPrompt1.SetWindowText(Server.GetUnknownAccountLine1());
	m_editPrompt2.SetWindowText(Server.GetUnknownAccountLine2());
	m_editPrompt3.SetWindowText(Server.GetUnknownAccountLine3());
	m_editPending.SetWindowText(Server.GetUnknownAccountPendingName());
	m_editCreate.SetWindowText(Server.GetUnknownAccountCreateName());

	OnToggleAddUnknowns();

	/*****/

	return TRUE;
}

//$$******************************************************************
CEdit* CServerPropertyPage9::GetEditIDMin() { return GetEdit(IDC_EDIT_IDMIN); }
CEdit* CServerPropertyPage9::GetEditIDMax() { return GetEdit(IDC_EDIT_IDMAX); }
//$$******************************************************************

void CServerPropertyPage9::OnToggleAddUnknowns()
{
	bool bAddUnknowns = IsTicked(m_checkAddUnknowns);
	bool bUnknownGroups = IsTicked(m_checkUnknownCardGroup);
	m_checkUnknownCardGroup.EnableWindow(bAddUnknowns);
	m_comboGroup.EnableWindow((bAddUnknowns) && (bUnknownGroups) && (m_comboGroup.GetCount() > 0));
	m_comboIdMinLen.EnableWindow(bAddUnknowns);
	GetEditIDMin()->EnableWindow(bAddUnknowns);
	GetEditIDMax()->EnableWindow(bAddUnknowns);
	m_editPrompt1.EnableWindow(bAddUnknowns);
	m_editPrompt2.EnableWindow(bAddUnknowns);
	m_editPrompt3.EnableWindow(bAddUnknowns);
	m_editPending.EnableWindow(bAddUnknowns);
	m_editCreate.EnableWindow(bAddUnknowns);
}

//$$******************************************************************

int CServerPropertyPage9::ExtractGroupNo()
{
	return CGroupComboHelpers::GetGroupNoFromCombo(m_comboGroup, Group::GroupNo.Min);
}

//$$******************************************************************
//$$******************************************************************
//$$******************************************************************

void CServerPropertyPage9::OnKillFocusEditIDMin()
{
	CString strText = GetEditBoxText(*GetEditIDMin());
	
	if (strText == "")
	{
		strText = "1";
	}

	::AddLeading(strText, 12, '0');
	GetEditIDMin()->SetWindowText(strText);
}

//$$******************************************************************

void CServerPropertyPage9::OnKillFocusEditIDMax()
{
	CString strText = GetEditBoxText(*GetEditIDMax());

	if (strText == "")
	{
		strText = "999999999999";
	}
	::AddLeading(strText, 12, '0');
	GetEditIDMax()->SetWindowText(strText);
}

//$$******************************************************************
//$$******************************************************************
//$$******************************************************************

BOOL CServerPropertyPage9::OnKillActive()
{
	CServerDataFolderSetIndex FolderSetIndex(1, 1);

	Server.SetAddUnknownCardsFlag(IsTicked(m_checkAddUnknowns));
	Server.SetUnknownCardGroupRSPFlag(IsTicked(m_checkUnknownCardGroup));
	Server.SetUnknownCardMinLength(m_comboIdMinLen.GetCurSel() + 1);
	Server.SetUnknownCardMinID(GetEditBoxInt64(*GetEditIDMin()));
	Server.SetUnknownCardMaxID(GetEditBoxInt64(*GetEditIDMax()));
	Server.SetUnknownCardGroupNo(ExtractGroupNo());
	Server.SetUnknownAccountLine1(GetEditBoxText(m_editPrompt1));
	Server.SetUnknownAccountLine2(GetEditBoxText(m_editPrompt2));
	Server.SetUnknownAccountLine3(GetEditBoxText(m_editPrompt3));
	Server.SetUnknownAccountPendingName(GetEditBoxText(m_editPending));
	Server.SetUnknownAccountCreateName(GetEditBoxText(m_editCreate));
	
	/*****/

	return CPropertyPage::OnKillActive();
}

//$$******************************************************************
