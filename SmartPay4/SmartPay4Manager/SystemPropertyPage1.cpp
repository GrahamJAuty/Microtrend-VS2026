//$$******************************************************************
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
//$$******************************************************************
#include "..\SmartPay4Shared\SQLTable_Account\SQLRecordSetAccountUserID.h"
#include "..\SmartPay4Shared\SQLTable_Group\SQLRepositoryGroup.h"
//$$******************************************************************
#include "..\SmartPay4Shared\GroupSetText.h"
#include "..\SmartPay4Shared\MembershipDatabase.h"
#include "..\SmartPay4Shared\ServerData.h"
#include "..\SmartPay4Shared\SQLBuilderBind.h"
#include "..\SmartPay4Shared\SQLWhereBuilder.h"
#include "..\SmartPay4Shared\UserTypeText.h"
//$$******************************************************************
#include "AutoShutdownStacker.h"
#include "PasswordListDlg.h"
#include "PasswordLogger.h"
#include "PINAdminDlg.h"
#include "PINRangeDlg.h"
#include "SetupBackgroundOptionsDlg.h"
#include "SetupFieldsDlg.h"
#include "SetupReaderDlg.h"
#include "SQLTweakDlg.h"
#include "StandardCombos.h"
#include "SystemPropertySheet.h"
//$$******************************************************************
#include "SystemPropertyPage1.h"
//$$******************************************************************

CSystemPropertyPage1::CSystemPropertyPage1()
	: CSSAutoShutdownPropertyPage(AutoShutdownHelper, CSystemPropertyPage1::IDD)
	, m_bRestartServerReqd(FALSE)
{
	m_nCurrentInterface = 0;

	m_bShowGroupShiftOnDuty = FALSE;

	{
		CSQLRowGroupFull RowGroup;
		
		RowGroup.SetGroupNo(3);

		CSQLRepositoryGroup RepoGroup;
		if (RepoGroup.SelectRow(RowGroup, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
		{
			if (RowGroup.GetUserType() == 3)
			{
				RowGroup.SetGroupNo(4);
				if (RepoGroup.SelectRow(RowGroup, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
				{
					if (RowGroup.GetUserType() == 4)
					{
						m_bShowGroupShiftOnDuty = TRUE;
					}
				}
			}
		}
	}
}

//$$******************************************************************

void CSystemPropertyPage1::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	/*****/
	DDX_Control(pDX, IDC_BUTTON_READER, m_buttonReader);
	DDX_Control(pDX, IDC_COMBO_INTERFACE, m_comboInterface);
	DDX_Control(pDX, IDC_CHECK_DELPCTRL, m_checkAutoDelPCtrlType);
	DDX_Control(pDX, IDC_STATIC_DELPCTRL, m_staticAutoDelPCtrlType);
	DDX_Control(pDX, IDC_COMBO_DELPCTRL, m_comboAutoDelPCtrlType);
	DDX_Control(pDX, IDC_CHECK_BIOCONSENT, m_checkBioConsent);
	DDX_Control(pDX, IDC_CHECK_PINNUMBER, m_checkPINNumbers);
	/*****/
	DDX_Control(pDX, IDC_CHECK_GROUPSETS, m_checkGroupSets);
	DDX_Control(pDX, IDC_BUTTON_GROUPSETS, m_buttonGroupSets);
	DDX_Control(pDX, IDC_CHECK_GS_DATERANGE, m_checkGroupShiftDateRange);
	DDX_Control(pDX, IDC_STATIC_GROUPSHIFT, m_staticGroupShift);
	DDX_Control(pDX, IDC_EDIT_GROUPSHIFT, m_editGroupShift);
	DDX_Control(pDX, IDC_CHECK_GS_ONDUTY, m_checkGroupShiftOnDuty);
	/*****/
	DDX_Control(pDX, IDC_CHECK_ENABLELOGOFF, m_checkLogOff);
	DDX_Control(pDX, IDC_COMBO_LOGOFF, m_comboLogOff);
	DDX_Control(pDX, IDC_STATIC_LOGOFF1, m_staticLogOff1);
	DDX_Control(pDX, IDC_STATIC_LOGOFF2, m_staticLogOff2);
	/*****/
	DDX_Control(pDX, IDC_CHECK_PASSWORDS, m_checkPasswords);
	DDX_Control(pDX, IDC_BUTTON_PASSWORDS, m_buttonPasswords);
	/*****/
	DDX_Control(pDX, IDC_CHECK_ENABLEBACKGROUND, m_checkBackground);
	DDX_Control(pDX, IDC_BUTTON_BACKGROUND, m_buttonBackground);
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CSystemPropertyPage1, CSSAutoShutdownPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON_INTERFACE, OnButtonInterface)
	ON_CBN_SELCHANGE(IDC_COMBO_INTERFACE, OnSelectInterface)
	ON_BN_CLICKED(IDC_CHECK_DELPCTRL, OnToggleAutoDelPCtrl)
	ON_BN_CLICKED(IDC_BUTTON_READER, OnButtonReader)
	/*****/
	ON_BN_CLICKED(IDC_BUTTON_USERTYPES, OnButtonUserTypes)
	ON_BN_CLICKED(IDC_CHECK_GROUPSETS, OnToggleGroupSets)
	ON_BN_CLICKED(IDC_BUTTON_GROUPSETS, OnButtonGroupSets)
	ON_BN_CLICKED(IDC_CHECK_GS_DATERANGE, OnToggleGroupShiftDateRange)
	/*****/
	ON_BN_CLICKED(IDC_CHECK_ENABLELOGOFF, OnToggleLogOff)
	/*****/
	ON_BN_CLICKED(IDC_CHECK_PASSWORDS, OnTogglePasswords)
	ON_BN_CLICKED(IDC_BUTTON_PASSWORDS, OnButtonPasswords)
	ON_BN_CLICKED(IDC_CHECK_ENABLEBACKGROUND, OnToggleBackground)
	ON_BN_CLICKED(IDC_BUTTON_BACKGROUND, OnButtonBackground)
	/*****/
	ON_BN_CLICKED(IDC_BUTTON_ALTKEY1, OnButtonAltKey1)
	ON_BN_CLICKED(IDC_BUTTON_ALTKEY2, OnButtonAltKey2)
	ON_BN_CLICKED(IDC_BUTTON_ALTKEY3, OnButtonAltKey3)
	ON_BN_CLICKED(IDC_BUTTON_ALTKEY4, OnButtonAltKey4)
END_MESSAGE_MAP()

//$$******************************************************************

BOOL CSystemPropertyPage1::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	
	m_nCurrentInterface = System.GetInterfaceType();
	CStandardCombos::FillInterfaceCombo( m_comboInterface, m_nCurrentInterface );
	OnSelectInterface();
	m_comboInterface.EnableWindow(FALSE);

	m_comboAutoDelPCtrlType.AddString("2 days");
	m_comboAutoDelPCtrlType.AddString("5 days");
	m_comboAutoDelPCtrlType.AddString("7 days");
	m_comboAutoDelPCtrlType.AddString("14 days");
	m_comboAutoDelPCtrlType.AddString("21 days");
	m_comboAutoDelPCtrlType.AddString("28 days");
	m_comboAutoDelPCtrlType.AddString("50 days");
	m_comboAutoDelPCtrlType.AddString("100 days");

	{
		int nType = System.GetAutoDeletePCtrlType();

		if ((nType < 0) || (nType > 8))
		{
			nType = 0;
		}

		if (0 == nType)
		{
			m_checkAutoDelPCtrlType.SetCheck(FALSE);
			m_comboAutoDelPCtrlType.SetCurSel(7);
		}
		else
		{
			m_checkAutoDelPCtrlType.SetCheck(TRUE);
			m_comboAutoDelPCtrlType.SetCurSel(nType - 1);
		}

		OnToggleAutoDelPCtrl();
	}

	m_checkGroupSets.SetCheck( System.GetAllowGroupSetsFlag() );

	{
		CString strDurations = "";
		CString strError = "";
		System.ValidateGroupShiftDurations(System.GetGroupShiftDurations(), strDurations, strError);

		if (strDurations == "")
		{
			System.RestoreDefaultGroupShiftDurations();
			strDurations = System.GetGroupShiftDurations();
		}

		bool bEnable = System.GetAllowGroupShiftDateRangeFlag();
		m_checkGroupShiftDateRange.SetCheck(bEnable);
		m_editGroupShift.LimitText(39);
		m_editGroupShift.SetWindowText(strDurations);
		m_staticGroupShift.EnableWindow(bEnable);
		m_editGroupShift.EnableWindow(bEnable);
	}

	m_checkGroupShiftOnDuty.SetCheck(System.GetAllowGroupShiftOnDutyFlag());
	m_checkPasswords.SetCheck( System.GetUsePasswordsFlag() );
	m_checkBackground.SetCheck(System.GetEnableBackgroundFlag() );

	{
		m_comboLogOff.AddString("After 1 minute");
		m_comboLogOff.AddString("After 5 minutes");
		m_comboLogOff.AddString("After 10 minutes");
		m_comboLogOff.AddString("After 15 minutes");
		m_comboLogOff.AddString("After 20 minutes");
		m_comboLogOff.AddString("After 30 minutes");

		int nType = System.GetAutoShutdownType();
		if ((nType < 0) || (nType > 6))
		{
			nType = 0;
		}

		if (0 == nType)
		{
			m_checkLogOff.SetCheck(FALSE);
			m_comboLogOff.SetCurSel(4);
			m_comboLogOff.EnableWindow(FALSE);
		}
		else
		{
			m_checkLogOff.SetCheck(TRUE);
			m_comboLogOff.SetCurSel(nType - 1);
			m_comboLogOff.EnableWindow(TRUE);
		}
	}

	SetDisplay();

	if (Passwords.GetPasswordInUse() != 0)
	{
		m_checkLogOff.EnableWindow(FALSE);
		m_comboLogOff.EnableWindow(FALSE);
		m_staticLogOff1.EnableWindow(FALSE);
		m_staticLogOff2.EnableWindow(FALSE);
		m_checkPasswords.EnableWindow(FALSE);
		m_buttonPasswords.EnableWindow(FALSE);
	}

	if (Passwords.IsSetupBgndAllowed() == FALSE)
	{
		m_checkBackground.EnableWindow(FALSE);
		m_buttonBackground.EnableWindow(FALSE);
	}

	if (FALSE == m_bShowGroupShiftOnDuty)
	{
		ShowAndEnableWindow(&m_checkGroupShiftOnDuty, FALSE);
	}

	{
		bool bAssumeBioConsent = System.GetAssumeBioConsentFlag();
		m_checkBioConsent.SetCheck(bAssumeBioConsent);
		ShowAndEnableWindow(&m_checkBioConsent, bAssumeBioConsent);
	}

	m_checkPINNumbers.SetCheck(System.GetAllowPINNumbersFlag());

	return TRUE;
}

//$$******************************************************************

void CSystemPropertyPage1::OnToggleAutoDelPCtrl()
{
	bool bEnable = IsTicked(m_checkAutoDelPCtrlType);
	m_staticAutoDelPCtrlType.EnableWindow(bEnable);
	m_comboAutoDelPCtrlType.EnableWindow(bEnable);
}

//$$******************************************************************

BOOL CSystemPropertyPage1::OnKillActive()
{
	{
		int nInterfaceType = m_comboInterface.GetCurSel();

		if (nInterfaceType != m_nCurrentInterface)
		{
			m_bRestartServerReqd = TRUE;
		}

		System.SetInterfaceType(nInterfaceType);
	}

	if (IsTicked(m_checkAutoDelPCtrlType))
	{
		System.SetAutoDeletePCtrlType(m_comboAutoDelPCtrlType.GetCurSel() + 1);
	}
	else
	{
		System.SetAutoDeletePCtrlType(0);
	}

	if (Passwords.IsSetupBgndAllowed() == TRUE)
	{
		System.SetEnableBackgroundFlag (IsTicked(m_checkBackground));
	}

	System.SetAllowGroupSetsFlag(IsTicked(m_checkGroupSets));

	if (Passwords.GetPasswordInUse() == 0)
	{
		System.SetUsePasswordsFlag (IsTicked(m_checkPasswords));

		if (IsTicked(m_checkLogOff))
		{
			System.SetAutoShutdownType(m_comboLogOff.GetCurSel() + 1);
		}
		else
		{
			System.SetAutoShutdownType(0);
		}
	}

	System.SetAllowGroupShiftDateRangeFlag(IsTicked(m_checkGroupShiftDateRange));
	
	if ( System.GetAllowGroupShiftDateRangeFlag() == TRUE )
	{
		CString strIn = GetEditBoxText(m_editGroupShift);
		CString strOut = "";
		CString strError = "";

		System.ValidateGroupShiftDurations(strIn, strOut, strError);

		if (strError != "")
		{
			CString strMsg = "";
			strMsg += "Invalid entry for group shift lengths.\n\n";
			strMsg += strError;
			Prompter.Error(strMsg);
			m_editGroupShift.SetFocus();
			m_editGroupShift.SetSel(0, -1);
			return FALSE;
		}

		System.SetGroupShiftDurations(strOut);
	}

	System.SetAllowGroupShiftOnDutyFlag(IsTicked(m_checkGroupShiftOnDuty));
	System.SetAssumeBioConsentFlag(IsTicked(m_checkBioConsent));
	System.SetAllowPINNumbersFlag(IsTicked(m_checkPINNumbers));

	return CPropertyPage::OnKillActive();
}

//$$******************************************************************

void CSystemPropertyPage1::SetDisplay()
{
	if (Passwords.IsSetupBgndAllowed() == TRUE)
	{
		m_buttonBackground.EnableWindow(IsTicked(m_checkBackground));
	}

	m_buttonPasswords.EnableWindow(IsTicked(m_checkPasswords));
	m_buttonGroupSets.EnableWindow(IsTicked(m_checkGroupSets));
}

//$$******************************************************************
//$$******************************************************************
//$$******************************************************************

void CSystemPropertyPage1::OnButtonInterface()
{
	CAutoShutdownPropSheetStacker Stacker ( (CSSAutoShutdownPropertySheet*) GetParent() );

	CString strMsg = "Changing the system interface will seriously alter the behaviour of SmartPay!\n\nAre you sure you wish to change it?";
	if ( Prompter.YesNo( strMsg, "Confirmation Required" ) == IDYES )
	{
		m_comboInterface.EnableWindow(TRUE);
		m_comboInterface.SetFocus();
	}
}

//$$******************************************************************

void CSystemPropertyPage1::OnSelectInterface()
{
	m_buttonReader.EnableWindow(m_comboInterface.GetCurSel() != nINTERFACE_BIOMETRIC);
}

//$$******************************************************************

void CSystemPropertyPage1::OnButtonReader()
{
	CAutoShutdownPropSheetStacker Stacker ((CSSAutoShutdownPropertySheet*)GetParent());
	CSetupReaderDlg dlg ( this );
	dlg.DoModal();
}

//$$******************************************************************
//$$******************************************************************
//$$******************************************************************

void CSystemPropertyPage1::OnButtonUserTypes()
{
	CAutoShutdownPropSheetStacker Stacker((CSSAutoShutdownPropertySheet*)GetParent());

	CUserTypeText text;
	text.Open ( Filenames.GetUserTypeFilename(), DB_READWRITE );		// create default text if not found
	text.Close();

	CSetupFieldsDlg dlg ( Filenames.GetUserTypeFilename(), "Setup Group User Types", "Type" );
	dlg.DoModal();
}

//$$******************************************************************

void CSystemPropertyPage1::OnToggleGroupSets()
{
	SetDisplay();
	m_buttonGroupSets.SetFocus();
}

//$$******************************************************************

void CSystemPropertyPage1::OnButtonGroupSets()
{
	if ( m_checkGroupSets.GetCheck() != 0 )
	{
		CAutoShutdownPropSheetStacker Stacker((CSSAutoShutdownPropertySheet*)GetParent());

		CGroupSetText text;
		text.Open ( Filenames.GetGroupSetFilename(), DB_READWRITE );		// create default text if not found
		text.Close();

		CSetupFieldsDlg dlg ( Filenames.GetGroupSetFilename(), "Setup Group Sets", "GroupSet" );
		dlg.DoModal();
	}
}

//$$******************************************************************

void CSystemPropertyPage1::OnToggleGroupShiftDateRange()
{
	bool bEnable = IsTicked(m_checkGroupShiftDateRange);

	if (FALSE == bEnable)
	{
		CSQLBuilderBind BuilderBind;
		CSQLWhereBuilder WhereBuilder;
		WhereBuilder.AppendConditionInt(BuilderBind, Account::GroupShiftStatus.Label, szNOTEQUAL, 0);

		CSQLRecordSetAccountUserID RecordSet(NULL,
			RSParams_AccountUserId_NormalByParams{ &BuilderBind, WhereBuilder.GetWhereString()});

		__int64 nUserID = 0;
		if (RecordSet.StepSelect(nUserID) == TRUE)
		{
			bEnable = TRUE;
			m_checkGroupShiftDateRange.SetCheck(bEnable);
			Prompter.Info("The Group Shift feature cannot be turned off now because\n\nit is active for one or more accounts.");
		}
	}

	m_staticGroupShift.EnableWindow(bEnable);
	m_editGroupShift.EnableWindow(bEnable);
}

//$$******************************************************************
//$$******************************************************************
//$$******************************************************************

void CSystemPropertyPage1::OnToggleLogOff()
{
	if (Passwords.GetPasswordInUse() == 0)
	{
		m_comboLogOff.EnableWindow(IsTicked(m_checkLogOff));
	}
}

//$$******************************************************************

void CSystemPropertyPage1::OnTogglePasswords()
{
	if (Passwords.GetPasswordInUse() == 0)
	{
		SetDisplay();
		m_buttonPasswords.SetFocus();
	}
}

//$$******************************************************************

void CSystemPropertyPage1::OnButtonPasswords()
{
	if (Passwords.GetPasswordInUse() == 0)
	{
		CAutoShutdownPropSheetStacker Stacker((CSSAutoShutdownPropertySheet*)GetParent());

		CPasswordListDlg dlg(&Passwords);

		if (dlg.DoModal() == IDOK)
		{
			Passwords.Save();			// save new settings
		}
		else
		{
			Passwords.ReLoad();			// reload old settings
		}

		Passwords.GetPassword(0);									// reload master password
	}
}

//$$******************************************************************
//$$******************************************************************
//$$******************************************************************

void CSystemPropertyPage1::OnToggleBackground()
{
	if (Passwords.IsSetupBgndAllowed() == TRUE)
	{
		SetDisplay();
		m_buttonBackground.SetFocus();
	}
}

//$$******************************************************************

void CSystemPropertyPage1::OnButtonBackground()
{
	if (Passwords.IsSetupBgndAllowed() == TRUE)
	{
		CAutoShutdownPropSheetStacker Stacker((CSSAutoShutdownPropertySheet*)GetParent());
		CSetupBackgroundOptionsDlg dlg;
		dlg.DoModal();
	}
}

//$$******************************************************************

void CSystemPropertyPage1::OnButtonAltKey1()
{
	if ((System.GetDoneSQLFileSizeFlag() == FALSE) || (System.GetDoneSQLShrinkLogFlag() == FALSE))
	{
		CSQLTweakDlg dlg(this);
		dlg.DoModal();
	}
}

//$$******************************************************************

void CSystemPropertyPage1::OnButtonAltKey2()
{
	ShowAndEnableWindow(&m_checkBioConsent, TRUE);
}

//$$******************************************************************

void CSystemPropertyPage1::OnButtonAltKey3()
{
	if (IsTicked(m_checkPINNumbers))
	{
		if (Server.GetAllowPINRangeFlag() == TRUE)
		{
			CPINRangeDlg dlg(this);
			dlg.DoModal();
		}
	}
}

//$$******************************************************************

void CSystemPropertyPage1::OnButtonAltKey4()
{
	if (IsTicked(m_checkPINNumbers))
	{
		if (Server.GetAllowPINAdminFlag() == TRUE)
		{
			CPINAdminDlg dlg(this);
			dlg.DoModal();
		}
	}
}

//$$******************************************************************

