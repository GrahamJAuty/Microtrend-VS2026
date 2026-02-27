//$$******************************************************************
#include "SystemPropertyPage3.h"
//$$******************************************************************

CSystemPropertyPage3::CSystemPropertyPage3()
	: CSSAutoShutdownPropertyPage(AutoShutdownHelper, CSystemPropertyPage3::IDD)
{
	m_bPebbleMode = FALSE;
}

//$$******************************************************************

void CSystemPropertyPage3::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_PURSES, m_staticPurses);
	DDX_Control(pDX, IDC_CHECK_SHOWCASHBUTTON, m_checkShowPurseTopup);
	DDX_Control(pDX, IDC_CHECK_SHOWPURSE1, m_checkEditPurse1);
	DDX_Control(pDX, IDC_CHECK_SHOWPURSE2, m_checkEditPurse2);
	DDX_Control(pDX, IDC_CHECK_ENABLEPURSE3, m_checkEnablePurse3);
	DDX_Control(pDX, IDC_CHECK_SHOWPURSE3, m_checkEditPurse3);
	/*****/
	DDX_Control(pDX, IDC_STATIC_POINTS, m_staticPoints);
	DDX_Control(pDX, IDC_CHECK_ENABLEPOINTS, m_checkEnablePoints);
	DDX_Control(pDX, IDC_CHECK_SHOWPOINTS, m_checkEditPoints);
	DDX_Control(pDX, IDC_CHECK_SHOWPOINTSBUTTON, m_checkShowPointsTopup);
	/*****/
	DDX_Control(pDX, IDC_STATIC_MISC, m_staticMisc);
	DDX_Control(pDX, IDC_STATIC_DEFAULT, m_staticDefault);
	DDX_Control(pDX, IDC_COMBO_PAGE, m_comboPage);
	DDX_Control(pDX, IDC_CHECK_EXTERNAL, m_checkExternal);
	DDX_Control(pDX, IDC_CHECK_BATCHUPDATES, m_checkEnableBatchUpdates);
	DDX_Control(pDX, IDC_BUTTON_ALTKEY1, m_buttonAltKey1);
	DDX_Control(pDX, IDC_CHECK_ENABLENOAUDIT, m_checkEnableNoAudit);
	DDX_Control(pDX, IDC_CHECK_STATUSCOLOUR, m_checkStatusColour);
	DDX_Control(pDX, IDC_CHECK_PROMPTCANCEL, m_checkPromptCancel);
	DDX_Control(pDX, IDC_CHECK_AUDITIMPORTFILTER, m_checkAuditImportFilter);
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CSystemPropertyPage3, CSSAutoShutdownPropertyPage)
	ON_BN_CLICKED(IDC_CHECK_ENABLEPURSE3, OnToggleEnablePurse3)
	ON_BN_CLICKED(IDC_CHECK_ENABLEPOINTS, OnToggleEnablePoints)
	ON_BN_CLICKED(IDC_BUTTON_ALTKEY1, OnButtonAltKey1)
END_MESSAGE_MAP()

//$$******************************************************************

BOOL CSystemPropertyPage3::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	m_bPebbleMode = (System.GetWebPaymentType() == nWEBPAYMENT_PEBBLE);

	if (TRUE == m_bPebbleMode)
	{
		DisplayPebbleMode();
	}

	m_checkShowPurseTopup.SetCheck( System.GetShowTopupButtonFlag() );
	m_checkEditPurse1.SetCheck( System.GetFileEditPurse1Flag() );
	m_checkEditPurse2.SetCheck( System.GetFileEditPurse2Flag() );
	m_checkEnablePurse3.SetCheck( System.GetEnablePurse3Flag() );
	m_checkEditPurse3.SetCheck( System.GetFileEditPurse3Flag() );
	/*****/
	m_checkEnablePoints.SetCheck( System.GetFileEnablePointsFlag() );
	m_checkEditPoints.SetCheck( System.GetFileEditPointsFlag() );
	m_checkShowPointsTopup.SetCheck( System.GetFileShowPointsButtonFlag() );
	/*****/
	m_checkExternal.SetCheck(System.GetExternalAccountFlag());
	m_checkEnableBatchUpdates.SetCheck( System.GetEnableBatchUpdatesFlag() );
	m_checkEnableNoAudit.SetCheck( System.GetEnableNoAuditFlag() );
	m_checkStatusColour.SetCheck(System.GetEnableAccountColourFlag());
	m_checkPromptCancel.SetCheck(System.GetAccountCancelPromptFlag());
	m_checkAuditImportFilter.SetCheck(System.GetAuditImportFilterFlag());

	if (FALSE == System.GetEnableNoAuditFlag())
	{
		m_checkEnableNoAudit.ShowWindow(SW_HIDE);
	}

	m_buttonAltKey1.ShowWindow(SW_HIDE);

	m_comboPage.AddString("Details");
	m_comboPage.AddString("Balances");
	m_comboPage.SetCurSel(System.GetDefaultAccountEditPage() == 1 ? 1 : 0);

	SetDisplay();
	return TRUE;
}

//$$******************************************************************

BOOL CSystemPropertyPage3::OnSetActive()
{
	if (FALSE == m_bPebbleMode)
	{
		m_bPebbleMode = (System.GetWebPaymentType() == nWEBPAYMENT_PEBBLE);

		if (TRUE == m_bPebbleMode)
		{
			DisplayPebbleMode();
		}
	}

	return CPropertyPage::OnSetActive();
}

//$$******************************************************************

BOOL CSystemPropertyPage3::OnKillActive()
{
	if (FALSE == m_bPebbleMode)
	{
		System.SetFileEditPurse1Flag(IsTicked(m_checkEditPurse1));
		System.SetFileEditPurse2Flag(IsTicked(m_checkEditPurse2));
		System.SetFileEditPurse3Flag(IsTicked(m_checkEditPurse3));
		System.SetFileEnablePointsFlag(IsTicked(m_checkEnablePoints));
		System.SetFileEditPointsFlag(IsTicked(m_checkEditPoints));
		System.SetFileShowPointsButtonFlag(IsTicked(m_checkShowPointsTopup));
	}

	System.SetShowTopupButtonFlag(IsTicked(m_checkShowPurseTopup));
	System.SetEnablePurse3Flag (IsTicked(m_checkEnablePurse3));
	/*****/
	System.SetExternalAccountFlag(IsTicked(m_checkExternal));
	System.SetEnableBatchUpdatesFlag(IsTicked(m_checkEnableBatchUpdates));
	System.SetEnableNoAuditFlag (IsTicked(m_checkEnableNoAudit));
	System.SetEnableAccountColourFlag(IsTicked(m_checkStatusColour));
	System.SetAccountCancelPromptFlag(IsTicked(m_checkPromptCancel));
	System.SetAuditImportFilterFlag(IsTicked(m_checkAuditImportFilter));
	/*****/
	System.SetDefaultAccountEditPage(m_comboPage.GetCurSel());

	return CPropertyPage::OnKillActive();
}

//$$******************************************************************
void CSystemPropertyPage3::OnToggleEnablePurse3()	{ SetDisplay();	}
void CSystemPropertyPage3::OnToggleEnablePoints()	{ SetDisplay(); }
//$$******************************************************************

void CSystemPropertyPage3::DisplayPebbleMode()
{
	ShowAndEnableWindow(&m_staticPurses, FALSE);
	ShowAndEnableWindow(&m_staticPoints, FALSE);
	ShowAndEnableWindow(&m_checkEditPurse1, FALSE);
	ShowAndEnableWindow(&m_checkEditPurse2, FALSE);
	ShowAndEnableWindow(&m_checkEditPurse3, FALSE);
	ShowAndEnableWindow(&m_checkEnablePoints, FALSE);
	ShowAndEnableWindow(&m_checkEditPoints, FALSE);
	ShowAndEnableWindow(&m_checkShowPointsTopup, FALSE);
	MoveControl(&m_staticMisc, 7, 7, 466, 336);
	MoveControl(&m_staticDefault, 24, 30, 64, 8);
	MoveControl(&m_comboPage, 89, 28, 69, 12);
	MoveControl(&m_checkExternal, 24, 52, 200, 10);
	MoveControl(&m_checkEnableBatchUpdates, 24, 68, 200, 10);
	MoveControl(&m_checkShowPurseTopup, 24, 84, 200, 10);
	MoveControl(&m_checkEnablePurse3, 24, 100, 200, 10);
	MoveControl(&m_checkStatusColour, 24, 116, 200, 10);
	MoveControl(&m_checkPromptCancel, 24, 132, 200, 10);
	MoveControl(&m_checkAuditImportFilter, 24, 148, 200, 10);
	MoveControl(&m_checkEnableNoAudit, 24, 164, 200, 10);
}

//$$******************************************************************

void CSystemPropertyPage3::SetDisplay()
{
	if (FALSE == m_bPebbleMode)
	{
		bool bPurse3 = (m_checkEnablePurse3.GetCheck() != 0);
		bool bPoints = (m_checkEnablePoints.GetCheck() != 0);

		m_checkEditPurse3.EnableWindow(bPurse3);
		m_checkEditPoints.EnableWindow(bPoints);
		m_checkShowPointsTopup.EnableWindow(bPoints);
	}
}

//$$******************************************************************

void CSystemPropertyPage3::OnButtonAltKey1()		
{
	m_checkEnableNoAudit.ShowWindow( SW_SHOW );
}

//$$******************************************************************
