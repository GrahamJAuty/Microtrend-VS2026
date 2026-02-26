//$$******************************************************************
#include "LoyaltyManager.h"
#include "StandardCombos.h"
//$$******************************************************************
#include "FamilyEditDlg.h"
//$$******************************************************************

CFamilyEditDlg::CFamilyEditDlg(const char* szTitle, CFamilyEditInfo& info, CWnd* pParent) 
	: CSSDialog(CFamilyEditDlg::IDD, pParent), m_editInfo(info)
{
	m_strTitle = szTitle;
}

//$$******************************************************************

CFamilyEditDlg::~CFamilyEditDlg()
{
}

//$$******************************************************************

void CFamilyEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_FORENAME, m_editForename);
	DDX_Control(pDX, IDC_EDIT_SURNAME, m_editSurname);
	DDX_Control(pDX, IDC_COMBO_GENDER, m_comboGender);
	DDX_Control(pDX, IDC_COMBO_MEMBERTYPE, m_comboMemberType);
}

//$$******************************************************************
BEGIN_MESSAGE_MAP(CFamilyEditDlg, CDialog)
END_MESSAGE_MAP()
//$$******************************************************************

BOOL CFamilyEditDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	SubclassEdit(IDC_EDIT_DOB, SS_DATE, 10, "");

	m_editForename.SetWindowText(m_editInfo.m_strForename);
	m_editSurname.SetWindowText(m_editInfo.m_strSurname);
	GetEditDOB()->SetWindowText(m_editInfo.m_strDOB);
	
	CStandardCombos::FillMemberTypeCombo( m_comboMemberType, m_editInfo.m_nMemberType );
	CStandardCombos::FillGenderCombo( m_comboGender, m_editInfo.m_strGender );

	SetWindowText ( m_strTitle );
	return TRUE;  
}

//$$******************************************************************
CEdit* CFamilyEditDlg::GetEditDOB() { return GetEdit(IDC_EDIT_DOB); }
//$$******************************************************************

void CFamilyEditDlg::OnOK()
{
	m_editInfo.m_strForename = GetEditBoxText(m_editForename);
	m_editInfo.m_strSurname = GetEditBoxText(m_editSurname);
	m_editInfo.m_strGender = GetComboBoxText(m_comboGender);
	m_editInfo.m_strDOB = GetEditBoxText(*GetEditDOB());
	m_editInfo.m_nMemberType = m_comboMemberType.GetCurSel();

	if (m_editInfo.m_strForename == "")
	{
		Prompter.Info("Please enter a Forename for this family member", "Input Required");
		m_editForename.SetFocus();
		return;
	}

	CSSDate date(m_editInfo.m_strDOB);
	if (date.IsSet() == FALSE)
	{
		Prompter.Info("Please enter a valid date of birth for this family member", "Input Required");
		GetEditDOB()->SetFocus();
		return;
	}

	EndDialog(IDOK);
}

//$$******************************************************************
