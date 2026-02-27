//*****************************************************************************
#include "SmartPay4Manager.h"
//*****************************************************************************
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
#include "..\SmartPay4Shared\SQLTable_UserText\SQLRepositoryUserText.h"
//*****************************************************************************
#include "SetupUserTextsDlg.h"
//*****************************************************************************

CSetupUserTextsDlg::CSetupUserTextsDlg( CWnd* pParent)
	: CSSAutoShutdownDialog( AutoShutdownHelper,CSetupUserTextsDlg::IDD, pParent)
{
}

//*****************************************************************************

CSetupUserTextsDlg::~CSetupUserTextsDlg()
{
}

//*****************************************************************************

void CSetupUserTextsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_TICK1, m_editTick1);
	DDX_Control(pDX, IDC_EDIT_TICK2, m_editTick2);
	DDX_Control(pDX, IDC_EDIT_TICK3, m_editTick3);
	DDX_Control(pDX, IDC_EDIT_TICK4, m_editTick4);
	DDX_Control(pDX, IDC_EDIT_BIOCONSENT, m_editBioConsent);
	DDX_Control(pDX, IDC_COMBO_BIOCONSENT, m_comboBioConsent);
	DDX_Control(pDX, IDC_EDIT_INFO1, m_editInfo1);
	DDX_Control(pDX, IDC_EDIT_INFO2, m_editInfo2);
	DDX_Control(pDX, IDC_EDIT_INFO3, m_editInfo3);
	DDX_Control(pDX, IDC_EDIT_INFO4, m_editInfo4);
	DDX_Control(pDX, IDC_EDIT_INFO5, m_editInfo5);
	DDX_Control(pDX, IDC_EDIT_INFO6, m_editInfo6);
	DDX_Control(pDX, IDC_EDIT_INFO7, m_editInfo7);
	DDX_Control(pDX, IDC_EDIT_INFO8, m_editInfo8);
	DDX_Control(pDX, IDC_EDIT_DOB, m_editDOB);
	DDX_Control(pDX, IDC_EDIT_MEMBERID, m_editMemberID);
	DDX_Control(pDX, IDC_STATIC_EXTREF, m_staticExtRef);
	DDX_Control(pDX, IDC_EDIT_EXTREF, m_editExtRef1);
	DDX_Control(pDX, IDC_EDIT_EXTREF2, m_editExtRef2);
	DDX_Control(pDX, IDC_CHECK_EXTREF2, m_checkExtRef2);
}

//*****************************************************************************

BEGIN_MESSAGE_MAP(CSetupUserTextsDlg, CSSAutoShutdownDialog)
	ON_BN_CLICKED(IDC_CHECK_EXTREF2, OnToggleExtRef2)
	ON_CBN_SELCHANGE(IDC_COMBO_BIOCONSENT, OnSelectBioConsent)
END_MESSAGE_MAP()

//*****************************************************************************

void CSetupUserTextsDlg::PrepareEditField(CEdit& edit, CString strLabel)
{
	CSQLRepositoryUserText RepoText;
	edit.LimitText(UserText::UserText.Max);
	edit.SetWindowText(RepoText.GetUserText(strLabel));
}

//*****************************************************************************

BOOL CSetupUserTextsDlg::CSSAutoShutdownPostInitDialog()
{
	PrepareEditField(m_editTick1, Account::Tick1.Label);
	PrepareEditField(m_editTick2, Account::Tick2.Label);
	PrepareEditField(m_editTick3, Account::Tick3.Label);
	PrepareEditField(m_editTick4, Account::Tick4.Label);
	PrepareEditField(m_editInfo1, Account::Info1.Label);
	PrepareEditField(m_editInfo2, Account::Info2.Label);
	PrepareEditField(m_editInfo3, Account::Info3.Label);
	PrepareEditField(m_editInfo4, Account::Info4.Label);
	PrepareEditField(m_editInfo5, Account::Info5.Label);
	PrepareEditField(m_editInfo6, Account::Info6.Label);
	PrepareEditField(m_editInfo7, Account::Info7.Label);
	PrepareEditField(m_editInfo8, Account::Info8.Label);
	PrepareEditField(m_editDOB, Account::DOB.Label);
	PrepareEditField(m_editMemberID, Account::MemberID.Label);
	PrepareEditField(m_editExtRef1, Account::ExternalRef1.Label);
	PrepareEditField(m_editExtRef2, Account::ExternalRef2.Label);

	m_editBioConsent.ShowWindow(SW_HIDE);
	m_editBioConsent.SetWindowText("Bio Consent");
	
	switch (System.GetInterfaceType())
	{
	case nINTERFACE_BIOMETRIC:
	case nINTERFACE_MIFAREv1:
		m_staticExtRef.ShowWindow(SW_HIDE);
		m_editExtRef1.ShowWindow(SW_HIDE);
		m_editExtRef2.ShowWindow(SW_HIDE);
		m_checkExtRef2.ShowWindow(SW_HIDE);
		break;

	default:
	{
		CSQLRepositoryUserText RepoText;
		m_checkExtRef2.SetCheck(RepoText.IsExtRef2Reqd());
		m_editExtRef2.EnableWindow(IsTicked(m_checkExtRef2));
	}
	break;
	}

	{
		CArray<int, int> arrayStops;
		arrayStops.Add(10);
		arrayStops.Add(60);
		m_comboBioConsent.SetTabStops(arrayStops);
	}

	m_comboBioConsent.AddItem("None,Bio Consent assumed for all accounts");
	
	{
		CString strTick = "Tick X,Bio Consent only if ticked";

		for (int n = 1; n <= 4; n++)
		{
			strTick.SetAt(5, '0' + n);
			m_comboBioConsent.AddItem(strTick);
		}
	}
	
	int nBioConsentTick = System.GetBioConsentTickNo();
	if ((nBioConsentTick < 0) || (nBioConsentTick > 4))
	{
		nBioConsentTick = 0;
	}

	m_comboBioConsent.SetCurSel(nBioConsentTick);
	OnSelectBioConsent();

	return TRUE;  
}

//*****************************************************************************

void CSetupUserTextsDlg::OnToggleExtRef2()
{
	m_editExtRef2.EnableWindow(IsTicked(m_checkExtRef2));
	m_editExtRef2.SetFocus();
}

//*****************************************************************************

void CSetupUserTextsDlg::OnSelectBioConsent()
{
	int nXPos = 0;
	int nYPos = 0;
	int nSel = m_comboBioConsent.GetCurSel();

	switch (nSel)
	{
	case 1:
		nXPos = 157;
		nYPos = 28;
		break;

	case 2:
		nXPos = 157;
		nYPos = 49;
		break;

	case 3:
		nXPos = 259;
		nYPos = 28;
		break;

	case 4:
		nXPos = 259;
		nYPos = 49;
		break;

	case 0:
	default:
		nSel = 0;
		nXPos = 157;
		nYPos = 98;
		break;
	}

	MoveControl(&m_editBioConsent, nXPos, nYPos);
	m_editBioConsent.ShowWindow((0 == nSel) ? SW_HIDE : SW_SHOW);
	ShowAndEnableWindow(&m_editTick1, (nSel != 1));
	ShowAndEnableWindow(&m_editTick2, (nSel != 2));
	ShowAndEnableWindow(&m_editTick3, (nSel != 3));
	ShowAndEnableWindow(&m_editTick4, (nSel != 4));
}

//*****************************************************************************

void CSetupUserTextsDlg::OnOK()
{
	UpdateText(Account::Tick1.Label, GetEditBoxText(m_editTick1));
	UpdateText(Account::Tick2.Label, GetEditBoxText(m_editTick2));
	UpdateText(Account::Tick3.Label, GetEditBoxText(m_editTick3));
	UpdateText(Account::Tick4.Label, GetEditBoxText(m_editTick4));
	UpdateText(Account::Info1.Label, GetEditBoxText(m_editInfo1));
	UpdateText(Account::Info2.Label, GetEditBoxText(m_editInfo2));
	UpdateText(Account::Info3.Label, GetEditBoxText(m_editInfo3));
	UpdateText(Account::Info4.Label, GetEditBoxText(m_editInfo4));
	UpdateText(Account::Info5.Label, GetEditBoxText(m_editInfo5));
	UpdateText(Account::Info6.Label, GetEditBoxText(m_editInfo6));
	UpdateText(Account::Info7.Label, GetEditBoxText(m_editInfo7));
	UpdateText(Account::Info8.Label, GetEditBoxText(m_editInfo8));
	UpdateText(Account::DOB.Label, GetEditBoxText(m_editDOB));
	UpdateText(Account::MemberID.Label, GetEditBoxText(m_editMemberID));
	UpdateText(Account::ExternalRef1.Label, GetEditBoxText(m_editExtRef1));
	UpdateText(Account::ExternalRef2.Label, GetEditBoxText(m_editExtRef2), IsTicked(m_checkExtRef2));
	System.SetBioConsentTickNo(m_comboBioConsent.GetCurSel());
	EndDialog(IDOK);
}

//*****************************************************************************

void CSetupUserTextsDlg::UpdateText(CString strLabel, CString strText, bool bUseFlag)
{
	CSQLRepositoryUserText RepoText;
	RepoText.SetUserText(strLabel, strText, NULL, bUseFlag);
}

//*****************************************************************************
