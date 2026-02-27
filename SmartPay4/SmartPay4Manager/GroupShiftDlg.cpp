//**********************************************************************
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SmartPay4Shared\SQLTable_Group\SQLRepositoryGroup.h"
//**********************************************************************
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "..\SmartPay4Shared\GroupShiftLogger.h"
#include "..\SmartPay4Shared\SQLFieldValidatorBase.h"
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\SQLHelpers.h"
//**********************************************************************
#include "AutoShutdownStacker.h"
#include "GroupComboHelpers.h"
#include "GroupShiftReportWrapper.h"
//**********************************************************************
#include "GroupShiftDlg.h"
//**********************************************************************

CGroupShiftDlg::CGroupShiftDlg(CSQLRowAccountFull& RowAccount, CGroupShiftValidator& GroupShiftValidator, CWnd* pParent)
	: CSSAutoShutdownDialog(AutoShutdownHelper, CGroupShiftDlg::IDD, pParent),
	m_RowAccount(RowAccount),
	m_GroupShiftValidator(GroupShiftValidator)
{
	m_bGroupShiftEnabled = FALSE;
	m_nPendingStatus = 0;

	CString strDurations = "";
	{
		CString strIn = System.GetGroupShiftDurations();
		CString strError = "";
		System.ValidateGroupShiftDurations(strIn, strDurations, strError);

		if (strDurations == "")
		{
			System.RestoreDefaultGroupShiftDurations();
			strDurations = System.GetGroupShiftDurations();
		}
	}

	CCSV csvDurations(strDurations, ':');
	for (int n = 0; n < csvDurations.GetSize(); n++)
	{
		m_arrayStandardDurations.Add(csvDurations.GetInt(n));
	}
}

//**********************************************************************

CGroupShiftDlg::~CGroupShiftDlg()
{
}

//**********************************************************************

void CGroupShiftDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_USERNAME, m_editUserName);
	DDX_Control(pDX, IDC_EDIT_GROUPNO, m_editGroupName);
	DDX_Control(pDX, IDC_STATIC0, m_static0);
	DDX_Control(pDX, IDC_STATIC1, m_static1);
	DDX_Control(pDX, IDC_STATIC2, m_static2);
	DDX_Control(pDX, IDC_STATIC3, m_static3);
	DDX_Control(pDX, IDC_STATIC4, m_static4);
	DDX_Control(pDX, IDC_STATIC_STATUS, m_staticStatus);
	DDX_Control(pDX, IDC_BUTTON_GROUPSHIFT, m_buttonGroupShift);
	DDX_Control(pDX, IDC_COMBO_GROUPSHIFT, m_comboGroupShift);
	DDX_Control(pDX, IDC_COMBO_DURATION, m_comboDuration);
	DDX_Control(pDX, IDC_DATEPICKER_START, m_DatePickerStart);
	DDX_Control(pDX, IDC_DATEPICKER_END, m_DatePickerEnd);
	DDX_Control(pDX, IDOK, m_buttonOK);
	DDX_Control(pDX, IDCANCEL, m_buttonCancel);
}

//**********************************************************************
BEGIN_MESSAGE_MAP(CGroupShiftDlg, CSSAutoShutdownDialog)
	ON_BN_CLICKED(IDC_BUTTON_LOG, OnButtonLog)
	ON_BN_CLICKED(IDC_BUTTON_GROUPSHIFT,OnButtonGroupShift)
	ON_CBN_SELCHANGE(IDC_COMBO_DURATION, OnSelectDuration)
	ON_CBN_KILLFOCUS(IDC_COMBO_DURATION, OnKillFocusDuration)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATEPICKER_START, OnDatePickerStart)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATEPICKER_END, OnDatePickerEnd)
END_MESSAGE_MAP()
//**********************************************************************

BOOL CGroupShiftDlg::CSSAutoShutdownPostInitDialog()
{
	CWnd* pComboEdit = m_comboDuration.GetWindow(GW_CHILD);

	if (pComboEdit != NULL)
	{
		pComboEdit->ModifyStyle(0, ES_NUMBER);
		((CEdit*)pComboEdit)->LimitText(3);
	}

	m_DatePickerStart.SetFormat("ddd dd MMM yyy");
	m_DatePickerEnd.SetFormat("ddd dd MMM yyy");

	{
		COleDateTime minStart = CGroupShiftValidator::GetMinStartOleDate();
		COleDateTime maxStart = CGroupShiftValidator::GetMaxStartOleDate();
		COleDateTime minEnd = CGroupShiftValidator::GetMinEndOleDate();
		COleDateTime maxEnd = CGroupShiftValidator::GetMaxEndOleDate();
		m_DatePickerStart.SetRange(&minStart, &maxStart);
		m_DatePickerEnd.SetRange(&minEnd,&maxEnd);
	}
	m_editUserName.SetWindowText(m_RowAccount.GetUsername());

	int nUsualGroup = 0;
	int nSwitchGroup = 0;
	switch (m_GroupShiftValidator.GetEditedGroupShiftStatus())
	{
	//GROUP SHIFT HAPPENING NOW
	case 1:
		nUsualGroup = m_GroupShiftValidator.GetEditedGroupShiftFutureGroup();
		nSwitchGroup = m_GroupShiftValidator.GetEditedGroupNow();
		ShowGroupShiftControls();
		break;

	//GROUP SHIFT IN FUTURE
	case 2:
		nUsualGroup = m_GroupShiftValidator.GetEditedGroupNow();
		nSwitchGroup = m_GroupShiftValidator.GetEditedGroupShiftFutureGroup();
		ShowGroupShiftControls();
		break;

	//NO GROUP SHIFT
	case 0:
	default:
		nUsualGroup = m_GroupShiftValidator.GetEditedGroupNow();
		nSwitchGroup = 0;
		HideGroupShiftControls();
		break;
	}

	{
		CSQLRowGroupFull RowGroup;
		RowGroup.SetGroupNo(nUsualGroup);

		CSQLRepositoryGroup RepoGroup;
		if (RepoGroup.SelectRow(RowGroup, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
		{
			m_editGroupName.SetWindowText(RowGroup.GetListboxText());

			CString strIn = RowGroup.GetShiftGroups();
			CString strOut = "";
			CString strError = "";
			RowGroup.ValidateShiftGroups(strIn, strOut, strError);

			CCSV csvGroups(strOut, ':');
			for (int n = 0; n < csvGroups.GetSize(); n++)
			{
				m_arrayShiftGroups.Add(csvGroups.GetInt(n));
			}
		}
		else
		{
			CString strText = "";
			strText.Format("%d  :  Unknown", RowGroup.GetGroupNo());
			m_editGroupName.SetWindowText(strText);
		}
	}

	if (nSwitchGroup != 0)
	{
		CGroupShiftValidator::InsertIntoArray(m_arrayShiftGroups, nSwitchGroup);
	}


	if (m_GroupShiftValidator.GetEditedGroupShiftStatus() != 0)
	{
		m_nPendingStatus = m_GroupShiftValidator.GetEditedGroupShiftStatus();
		CopyGroupShiftFutureGroupToControls();
		CopyGroupShiftDatesToControls();
	}

	return TRUE;
}

//**********************************************************************

void CGroupShiftDlg::OnButtonLog()
{
	__int64 nUserID = m_RowAccount.GetUserID();

	if (nUserID >= 0)
	{
		CSQLRepositoryAccount RepoAccount;
		if (RepoAccount.RowExists(nUserID, NULL).GetSQLResult() != 0)
		{
			CAutoShutdownDlgStacker Stacker(this);
			CGroupShiftReportWrapper wrapper("R141", nUserID, this);
			wrapper.ShowReport();
		}
	}
}

//$$******************************************************************

void CGroupShiftDlg::CopyGroupShiftDatesToControls()
{
	{
		COleDateTime dateStart = m_GroupShiftValidator.GetEditedStartOleDate();
		COleDateTime dateEnd = m_GroupShiftValidator.GetEditedEndOleDate();

		m_DatePickerStart.SetTime(dateStart);
		m_DatePickerEnd.SetTime(dateEnd);

		COleDateTimeSpan span = dateEnd - dateStart;
		FillDurationCombo(span.GetDays());
	}

	int nSwitchGroup = 0;
	if (m_GroupShiftValidator.GetEditedGroupShiftStatus() == 1)
	{
		nSwitchGroup = m_GroupShiftValidator.GetEditedGroupNow();
	}
	else
	{
		nSwitchGroup = m_GroupShiftValidator.GetEditedGroupShiftFutureGroup();
	}

	CGroupComboHelpers::FillGroupCombo(m_comboGroupShift, m_arrayShiftGroups, nSwitchGroup);

	switch (m_nPendingStatus)
	{
	case 1:
		m_staticStatus.SetWindowText("Group Shift Active Now");
		break;

	case 2:
		m_staticStatus.SetWindowText("Group Shift Starting in Future");
		break;

	case 0:
	default:
		m_staticStatus.SetWindowText("Group Shift Finished");
		break;
	}
}

//**********************************************************************

void CGroupShiftDlg::CopyGroupShiftFutureGroupToControls()
{
	int nSwitchGroup = 0;
	if (m_GroupShiftValidator.GetEditedGroupShiftStatus() == 1)
	{
		nSwitchGroup = m_GroupShiftValidator.GetEditedGroupNow();
	}
	else
	{
		nSwitchGroup = m_GroupShiftValidator.GetEditedGroupShiftFutureGroup();
	}

	CGroupComboHelpers::FillGroupCombo(m_comboGroupShift, m_arrayShiftGroups, nSwitchGroup);
}

//**********************************************************************

void CGroupShiftDlg::OnButtonGroupShift()
{
	if (TRUE == m_bGroupShiftEnabled)
	{
		int nGroupNo = CGroupComboHelpers::GetGroupNoFromCombo(m_comboGroupShift, 1);

		if (m_GroupShiftValidator.GetEditedGroupShiftStatus() == 1)
		{
			m_GroupShiftValidator.SetEditedGroupNow(nGroupNo);
		}
		else
		{
			m_GroupShiftValidator.SetEditedGroupShiftFutureGroup(nGroupNo);
		}

		HideGroupShiftControls();
	}
	else
	{
		if (m_arrayShiftGroups.GetSize() == 0)
		{
			Prompter.Error("There are no shift groups defined for this account.");
		}
		else
		{
			if (m_GroupShiftValidator.GetEditedGroupShiftStatus() == 0)
			{
				m_GroupShiftValidator.CreateEditedGroupShift(m_arrayShiftGroups.GetAt(0), m_arrayStandardDurations.GetAt(0), m_nPendingStatus);
			}
			else
			{
				m_nPendingStatus = m_GroupShiftValidator.GetEditedGroupShiftStatus();
			}
			
			CopyGroupShiftFutureGroupToControls();
			CopyGroupShiftDatesToControls();
			ShowGroupShiftControls();
		}
	}
}

//**********************************************************************

void CGroupShiftDlg::OnKillFocusDuration()
{
	int nDuration = atoi(GetComboBoxText(m_comboDuration));

	if ((nDuration >= 1) && (nDuration <= 999))
	{
		COleDateTime dateStart = m_GroupShiftValidator.GetEditedStartOleDate();
		COleDateTime dateEnd = dateStart + COleDateTimeSpan(nDuration, 0, 0, 0);
		m_GroupShiftValidator.SetEditedEndOleDate(dateEnd);
		m_GroupShiftValidator.ValidateEditedDates(m_nPendingStatus);
	}

	CopyGroupShiftDatesToControls();
}

//**********************************************************************

void CGroupShiftDlg::OnSelectDuration()
{
	int nSel = m_comboDuration.GetCurSel();

	if ((nSel >= 0) && (nSel < m_arrayDurations.GetSize()))
	{
		COleDateTime dateStart = m_GroupShiftValidator.GetEditedStartOleDate();
		COleDateTime dateEnd = dateStart + COleDateTimeSpan(m_arrayDurations.GetAt(nSel), 0, 0, 0);
		m_GroupShiftValidator.SetEditedEndOleDate(dateEnd);
		m_GroupShiftValidator.ValidateEditedDates(m_nPendingStatus);
	}

	CopyGroupShiftDatesToControls();
}

//**********************************************************************

void CGroupShiftDlg::OnDatePickerStart(NMHDR* pNMHDR, LRESULT* pResult)
{
	*pResult = 0;
	COleDateTime dateStart;
	m_DatePickerStart.GetTime(dateStart);
	m_GroupShiftValidator.SetEditedStartOleDate(dateStart);
	m_GroupShiftValidator.ValidateEditedDates(m_nPendingStatus);
	CopyGroupShiftDatesToControls();
}

//**********************************************************************

void CGroupShiftDlg::OnDatePickerEnd(NMHDR* pNMHDR, LRESULT* pResult)
{
	*pResult = 0;
	COleDateTime dateEnd;
	m_DatePickerEnd.GetTime(dateEnd);
	m_GroupShiftValidator.SetEditedEndOleDate(dateEnd);
	m_GroupShiftValidator.ValidateEditedDates(m_nPendingStatus);
	CopyGroupShiftDatesToControls();
}

//**********************************************************************

void CGroupShiftDlg::FillDurationCombo(int nDays)
{
	m_arrayDurations.RemoveAll();
	for (int n = 0; n < m_arrayStandardDurations.GetSize(); n++)
	{
		m_arrayDurations.Add(m_arrayStandardDurations.GetAt(n));
	}

	if (nDays > 0)
	{
		CGroupShiftValidator::InsertIntoArray(m_arrayDurations, nDays);
	}

	int nSel = 0;
	m_comboDuration.ResetContent();
	for (int n = 0; n < m_arrayDurations.GetSize(); n++)
	{
		int nItem = m_arrayDurations.GetAt(n);
		
		CString str = "";
		str.Format("%d", m_arrayDurations.GetAt(n));
		m_comboDuration.AddString(str);

		if (nItem == nDays)
		{
			nSel = n;
		}
	}
	m_comboDuration.SetCurSel(nSel);
}

//**********************************************************************

void CGroupShiftDlg::ShowGroupShiftControls()
{
	m_buttonGroupShift.SetWindowText("Cancel Group Shift");

	ResizeDialog(357, 285);
	MoveControl(&m_static0, 7, 83, 343, 157);
	MoveControl(&m_buttonOK, 300, 248);
	MoveControl(&m_buttonCancel, 300, 264);

	ShowAndEnableWindow(&m_staticStatus, TRUE);
	ShowAndEnableWindow(&m_static1, TRUE);
	ShowAndEnableWindow(&m_static2, TRUE);
	ShowAndEnableWindow(&m_static3, TRUE);
	ShowAndEnableWindow(&m_static4, TRUE);
	ShowAndEnableWindow(&m_comboDuration, TRUE);
	ShowAndEnableWindow(&m_comboGroupShift, TRUE);
	ShowAndEnableWindow(&m_DatePickerStart, TRUE);
	ShowAndEnableWindow(&m_DatePickerEnd, TRUE);

	m_bGroupShiftEnabled = TRUE;
}

//**********************************************************************

void CGroupShiftDlg::HideGroupShiftControls()
{
	m_buttonGroupShift.SetWindowText("Apply Group Shift");
	
	ResizeDialog(357, 185);
	MoveControl(&m_static0, 7, 83, 343, 57);
	MoveControl(&m_buttonOK, 300, 148);
	MoveControl(&m_buttonCancel, 300, 164);
	
	ShowAndEnableWindow(&m_staticStatus, FALSE);
	ShowAndEnableWindow(&m_static1, FALSE);
	ShowAndEnableWindow(&m_static2, FALSE);
	ShowAndEnableWindow(&m_static3, FALSE);
	ShowAndEnableWindow(&m_static4, FALSE);
	ShowAndEnableWindow(&m_comboDuration, FALSE);
	ShowAndEnableWindow(&m_comboGroupShift, FALSE);
	ShowAndEnableWindow(&m_DatePickerStart, FALSE);
	ShowAndEnableWindow(&m_DatePickerEnd, FALSE);

	m_bGroupShiftEnabled = FALSE;
}

//**********************************************************************

void CGroupShiftDlg::OnOK()
{
	if (TRUE == m_bGroupShiftEnabled)
	{
		int nGroupNo = CGroupComboHelpers::GetGroupNoFromCombo(m_comboGroupShift, 1);

		if (m_GroupShiftValidator.GetEditedGroupShiftStatus() == 1)
		{
			m_GroupShiftValidator.SetEditedGroupNow(nGroupNo);
		}
		else
		{
			m_GroupShiftValidator.SetEditedGroupShiftFutureGroup(nGroupNo);
		}
	}
	else
	{
		m_GroupShiftValidator.ResetEditedGroupShift();
	}

	m_GroupShiftValidator.ValidateEditedGroupShift();

	if (m_GroupShiftValidator.UpdateAccount(m_RowAccount) == TRUE)
	{
		CString strError = "";
		if (CSQLHelpers::UpdateAccountWithVersionCheck(m_RowAccount, strError) == FALSE)
		{
			Prompter.Error(strError);
			return;
		}

		GroupShiftLogger.LogManualChange(m_RowAccount, m_GroupShiftValidator, 3);
	}

	EndDialog(IDOK);
}

//**********************************************************************
