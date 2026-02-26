//$$******************************************************************
#include "..\SPosLoyalty4Shared\AccountFieldValidator.h"
#include "..\SPosLoyalty4Shared\SQLNamespaces.h"
//$$******************************************************************
#include "GroupComboHelpers.h"
#include "LoyaltyManager.h"
#include "SelectAlertDlg.h"
#include "StandardCombos.h"
//$$******************************************************************
#include "BatchPropertyPage5.h"
//$$******************************************************************

CBatchPropertyPage5::CBatchPropertyPage5(CSQLRowAccountFull& RowAccount, CBatchAccountDetailMemory& Memory)
	: CSSPropertyPage(CBatchPropertyPage5::IDD), m_RowAccount(RowAccount), m_Memory(Memory)
{
	m_dateToday = COleDateTime::GetCurrentTime();
	Server.Read(Filenames.GetServerDataFilename());
	m_alertText.Open(Filenames.GetAlertTextFilename());
}

//$$******************************************************************

void CBatchPropertyPage5::RememberGroupNo()
{
	m_Memory.m_nGroupNo = m_RowAccount.GetGroupNo();
}

//$$******************************************************************

void CBatchPropertyPage5::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_SETNAME, m_checkSetName);
	DDX_Control(pDX, IDC_CHECK_SETGROUP, m_checkSetGroup);
	DDX_Control(pDX, IDC_CHECK_SETSCHEME, m_checkSetScheme);
	DDX_Control(pDX, IDC_CHECK_SETEXPIRY, m_checkSetExpiry);
	DDX_Control(pDX, IDC_CHECK_DATEEXPIRY, m_checkDateExpiry);
	DDX_Control(pDX, IDC_DATEPICKER_EXPIRY, m_datePickerExpiry);
	DDX_Control(pDX, IDC_CHECK_SETDOB, m_checkSetDOB);
	DDX_Control(pDX, IDC_CHECK_DATEDOB, m_checkDateDOB);
	DDX_Control(pDX, IDC_DATEPICKER_DOB, m_datePickerDOB);
	DDX_Control(pDX, IDC_CHECK_SETMAXSPEND, m_checkSetMaxSpend);
	DDX_Control(pDX, IDC_CHECK_SETOVERSPEND, m_checkSetOverSpend);
	DDX_Control(pDX, IDC_CHECK_SETALERT, m_checkSetAlert);
	DDX_Control(pDX, IDC_CHECK_SETHOTLIST, m_checkSetHotlist);
	DDX_Control(pDX, IDC_CHECK_SETINACTIVE, m_checkSetInactive);
	DDX_Control(pDX, IDC_CHECK_SETGENDER, m_checkSetGender);
	DDX_Control(pDX, IDC_EDIT_NAME, m_editName);
	DDX_Control(pDX, IDC_COMBO_GROUP, m_comboGroup);
	DDX_Control(pDX, IDC_COMBO_SCHEME, m_comboScheme);
	DDX_Control(pDX, IDC_BUTTON_ALERT, m_buttonAlert);
	DDX_Control(pDX, IDC_EDIT_ALERTTEXT, m_editAlert);
	DDX_Control(pDX, IDC_COMBO_HOTLIST, m_comboHotlist);
	DDX_Control(pDX, IDC_COMBO_INACTIVE, m_comboInactive);
	DDX_Control(pDX, IDC_COMBO_GENDER, m_comboGender);
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CBatchPropertyPage5, CPropertyPage)
	ON_BN_CLICKED(IDC_CHECK_SETNAME, OnToggleSetName)
	ON_BN_CLICKED(IDC_CHECK_SETGROUP, OnToggleSetGroup)
	ON_CBN_SELCHANGE(IDC_COMBO_GROUP, OnSelectGroup)
	ON_BN_CLICKED(IDC_CHECK_SETSCHEME, OnToggleSetScheme)
	ON_BN_CLICKED(IDC_CHECK_SETEXPIRY, OnToggleSetExpiry)
	ON_BN_CLICKED(IDC_CHECK_DATEEXPIRY, OnToggleDateExpiry)
	ON_BN_CLICKED(IDC_CHECK_SETDOB, OnToggleSetDOB)
	ON_BN_CLICKED(IDC_CHECK_DATEDOB, OnToggleDateDOB)
	ON_BN_CLICKED(IDC_CHECK_SETMAXSPEND, OnToggleSetMaxSpend)
	ON_BN_CLICKED(IDC_CHECK_SETOVERSPEND, OnToggleSetOverSpend)
	ON_BN_CLICKED(IDC_CHECK_SETALERT, OnToggleSetAlert)
	ON_BN_CLICKED(IDC_CHECK_SETHOTLIST, OnToggleSetHotlist)
	ON_BN_CLICKED(IDC_CHECK_SETINACTIVE, OnToggleSetInactive)
	ON_BN_CLICKED(IDC_CHECK_SETGENDER, OnToggleSetGender)
	ON_BN_CLICKED(IDC_BUTTON_ALERT, OnButtonAlert)
END_MESSAGE_MAP()

//$$******************************************************************

BOOL CBatchPropertyPage5::OnInitDialog()
{
	CSSPropertyPage::OnInitDialog();

	m_Memory.TidyOptions();

	m_strAlertCodes = m_Memory.m_strAlertCodes;

	m_checkSetName.SetCheck(m_Memory.m_bSetName);
	m_checkSetGroup.SetCheck(m_Memory.m_bSetGroup);
	m_checkSetScheme.SetCheck(m_Memory.m_bSetScheme);
	m_checkSetMaxSpend.SetCheck(m_Memory.m_bSetMaxSpend);
	m_checkSetOverSpend.SetCheck(m_Memory.m_bSetOverSpend);
	m_checkSetAlert.SetCheck(m_Memory.m_bSetAlert);
	m_checkSetHotlist.SetCheck(m_Memory.m_bSetHotlist);
	m_checkSetInactive.SetCheck(m_Memory.m_bSetInactive);
	m_checkSetGender.SetCheck(m_Memory.m_bSetGender);

	//DOB
	{
		COleDateTime oleMin = CAccountFieldValidator::GetMinDob();
		COleDateTime oleMax = CAccountFieldValidator::GetMaxDob();
		m_datePickerDOB.SetRange(&oleMin, &oleMax);

		if (BATCH_DATEACTION_SET == m_Memory.m_nDOBAction)
		{
			if (CAccountFieldValidator::ValidateDOB(m_Memory.m_dateDOB) == FALSE)
			{
				m_Memory.m_nDOBAction = BATCH_DATEACTION_NONE;
			}
		}

		bool bSet = FALSE;
		bool bDate = FALSE;

		switch (m_Memory.m_nDOBAction)
		{
		case BATCH_DATEACTION_CLEAR:
			bSet = TRUE;
			break;

		case BATCH_DATEACTION_SET:
			bSet = TRUE;
			bDate = TRUE;
			break;
		}

		m_checkSetDOB.SetCheck(bSet);
		m_checkDateDOB.EnableWindow(bSet);
		m_checkDateDOB.SetCheck(bDate);
		m_datePickerDOB.EnableWindow(bDate);
		m_datePickerDOB.SetTime(bDate ? m_Memory.m_dateDOB : m_dateToday);
	}

	//EXPIRY
	{
		COleDateTime oleMin = CAccountFieldValidator::GetMinExpiry();
		COleDateTime oleMax = CAccountFieldValidator::GetMaxExpiry();
		m_datePickerExpiry.SetRange(&oleMin, &oleMax);

		if (BATCH_DATEACTION_SET == m_Memory.m_nExpiryAction)
		{
			if (CAccountFieldValidator::ValidateExpiry(m_Memory.m_dateExpiry) == FALSE)
			{
				m_Memory.m_nExpiryAction = BATCH_DATEACTION_NONE;
			}
		}

		bool bSet = FALSE;
		bool bDate = FALSE;

		switch (m_Memory.m_nExpiryAction)
		{
		case BATCH_DATEACTION_CLEAR:
			bSet = TRUE;
			break;

		case BATCH_DATEACTION_SET:
			bSet = TRUE;
			bDate = TRUE;
			break;
		}

		m_checkSetExpiry.SetCheck(bSet);
		m_checkDateExpiry.EnableWindow(bSet);
		m_checkDateExpiry.SetCheck(bDate);
		m_datePickerExpiry.EnableWindow(bDate);
		m_datePickerExpiry.SetTime(bDate ? m_Memory.m_dateExpiry : m_dateToday);
	}
		

	SubclassEdit(IDC_EDIT_OVERSPEND, SS_NUM_SDP, 11, "%.2f");
	SubclassEdit(IDC_EDIT_MAXSPEND, SS_NUM_SDP, 11, "%.2f");

	m_editName.EnableWindow(m_Memory.m_bSetName);
	m_editName.SetWindowText(m_Memory.m_strName);

	CGroupComboHelpers::FillGroupCombo(m_comboGroup, m_Memory.m_nGroupNo,FALSE);
	m_comboGroup.EnableWindow(m_Memory.m_bSetGroup);

	CGroupComboHelpers::FillSchemeCombo(m_comboScheme, m_Memory.m_nGroupNo, m_Memory.m_nSchemeNo, TRUE, m_Memory.m_bSetGroup);
	m_comboScheme.EnableWindow(m_Memory.m_bSetScheme);

	GetEditMaxSpend()->EnableWindow(m_Memory.m_bSetMaxSpend);
	SetEditDouble(GetEditMaxSpend(), m_Memory.m_dMaxSpend);

	GetEditOverSpend()->EnableWindow(m_Memory.m_bSetOverSpend);
	SetEditDouble(GetEditOverSpend(), m_Memory.m_dOverSpend);

	if (Server.GetAllowOverSpendFlag() == FALSE)
	{
		GetEditOverSpend()->ShowWindow(SW_HIDE);
		m_checkSetOverSpend.ShowWindow(SW_HIDE);
	}

	if (TRUE == m_Memory.m_bSetAlert)
	{
		m_buttonAlert.EnableWindow(TRUE);
		m_editAlert.SetWindowText(m_alertText.GetDisplayText(m_strAlertCodes));
	}
	else
	{
		m_buttonAlert.EnableWindow(FALSE);
		m_editAlert.SetWindowText("");
	}

	m_comboHotlist.EnableWindow(m_Memory.m_bSetHotlist);
	CStandardCombos::FillNoYesCombo(m_comboHotlist, m_Memory.m_nHotlistIndex);

	m_comboInactive.EnableWindow(m_Memory.m_bSetInactive);
	CStandardCombos::FillNoYesCombo(m_comboInactive, m_Memory.m_nInactiveIndex);

	m_comboGender.EnableWindow(m_Memory.m_bSetGender);
	CStandardCombos::FillGenderCombo(m_comboGender, m_Memory.m_nGenderIndex);

	return TRUE;
}

//$$******************************************************************
CEdit* CBatchPropertyPage5::GetEditMaxSpend(){ return GetEdit( IDC_EDIT_MAXSPEND ); }
CEdit* CBatchPropertyPage5::GetEditOverSpend(){ return GetEdit( IDC_EDIT_OVERSPEND ); }
//$$******************************************************************

void CBatchPropertyPage5::SetEditDouble( CEdit* pEdit, double d )
{
	CString str;
	str.Format( "%.2f", d );
	pEdit -> SetWindowText( str );
}

//$$******************************************************************

void CBatchPropertyPage5::SetEditInt( CEdit* pEdit, int n )
{
	CString str;
	str.Format( "%d", n );
	pEdit -> SetWindowText( str );
}

//$$******************************************************************

double CBatchPropertyPage5::GetEditDouble( CEdit* pEdit )
{
	CString str;
	pEdit -> GetWindowText( str );
	return atof( str );
}

//$$******************************************************************

int CBatchPropertyPage5::GetEditInt( CEdit* pEdit )
{
	CString str;
	pEdit -> GetWindowText( str );
	return atoi( str );
}

//$$******************************************************************

void CBatchPropertyPage5::UpdateMemory()
{
	m_Memory.m_bSetName = (m_checkSetName.GetCheck() != 0);
	m_editName.GetWindowText(m_Memory.m_strName);

	m_Memory.m_bSetGroup = (m_checkSetGroup.GetCheck() != 0);

	m_Memory.m_nGroupNo = Group::GroupNo.Min;
	if (m_Memory.m_bSetGroup == TRUE)
	{
		m_Memory.m_nGroupNo = CGroupComboHelpers::GetGroupNoFromCombo(m_comboGroup,Group::GroupNo.Min);
	}

	m_Memory.m_bSetScheme = (m_checkSetScheme.GetCheck() != 0);

	m_Memory.m_nSchemeNo = Scheme::SchemeNo.Min;
	
	if (m_Memory.m_bSetScheme == TRUE )
	{
		m_Memory.m_nSchemeNo = CGroupComboHelpers::GetGroupNoFromCombo(m_comboScheme, Scheme::SchemeNo.Min);
	}

	//EXPIRY
	m_Memory.m_nExpiryAction = BATCH_DATEACTION_NONE;

	if (IsTicked(m_checkSetExpiry))
	{
		m_Memory.m_nExpiryAction = BATCH_DATEACTION_CLEAR;

		if (IsTicked(m_checkDateExpiry))
		{
			COleDateTime oleExpiry;
			m_datePickerExpiry.GetTime(oleExpiry);
			if (CAccountFieldValidator::ValidateExpiry(oleExpiry) == FALSE)
			{
				m_Memory.m_nExpiryAction = BATCH_DATEACTION_NONE;
			}
			else
			{
				m_Memory.m_nExpiryAction = BATCH_DATEACTION_SET;
				m_Memory.m_dateExpiry = oleExpiry;
			}
		}
	}

	//DOB
	m_Memory.m_nDOBAction = BATCH_DATEACTION_NONE;

	if (IsTicked(m_checkSetDOB))
	{
		m_Memory.m_nDOBAction = BATCH_DATEACTION_CLEAR;

		if (IsTicked(m_checkDateDOB))
		{
			COleDateTime oleDOB;
			m_datePickerDOB.GetTime(oleDOB);
			if (CAccountFieldValidator::ValidateDOB(oleDOB) == FALSE)
			{
				m_Memory.m_nDOBAction = BATCH_DATEACTION_NONE;
			}
			else
			{
				m_Memory.m_nDOBAction = BATCH_DATEACTION_SET;
				m_Memory.m_dateDOB = oleDOB;
			}
		}
	}

	m_Memory.m_bSetMaxSpend = (m_checkSetMaxSpend.GetCheck() != 0);
	m_Memory.m_dMaxSpend = GetEditDouble(GetEditMaxSpend());

	m_Memory.m_bSetOverSpend = (m_checkSetOverSpend.GetCheck() != 0);
	m_Memory.m_dOverSpend = GetEditDouble(GetEditOverSpend());

	m_Memory.m_bSetAlert = (m_checkSetAlert.GetCheck() != 0);
	m_Memory.m_strAlertCodes = m_strAlertCodes;

	m_Memory.m_bSetHotlist = (m_checkSetHotlist.GetCheck() != 0);
	m_Memory.m_nHotlistIndex = m_comboHotlist.GetCurSel();

	m_Memory.m_bSetInactive = (m_checkSetInactive.GetCheck() != 0);
	m_Memory.m_nInactiveIndex = m_comboInactive.GetCurSel();

	m_Memory.m_bSetGender = (m_checkSetGender.GetCheck() != 0);
	m_Memory.m_nGenderIndex = m_comboGender.GetCurSel();

	m_Memory.TidyOptions();
}

//$$******************************************************************

void CBatchPropertyPage5::OnToggleSetName()
{
	m_editName.EnableWindow( m_checkSetName.GetCheck() != 0 );
	m_editName.SetFocus();
}

//$$******************************************************************

void CBatchPropertyPage5::OnToggleSetGroup()
{
	bool bUseGroup = IsTicked(m_checkSetGroup);

	m_comboGroup.EnableWindow(bUseGroup);
	m_comboGroup.SetFocus();

	int nSchemeNo = CGroupComboHelpers::GetGroupNoFromCombo(m_comboScheme, Scheme::SchemeNo.Min);

	if (TRUE == bUseGroup)
	{
		int nGroupNo = CGroupComboHelpers::GetGroupNoFromCombo(m_comboGroup, Group::GroupNo.Min);
		CGroupComboHelpers::FillSchemeCombo(m_comboScheme, nGroupNo, nSchemeNo, TRUE, bUseGroup);
	}
	else
	{
		CGroupComboHelpers::FillSchemeCombo(m_comboScheme, 0, nSchemeNo, TRUE, FALSE);
	}
}

//$$******************************************************************

void CBatchPropertyPage5::OnSelectGroup()
{
	int nGroupNo = CGroupComboHelpers::GetGroupNoFromCombo(m_comboGroup, Group::GroupNo.Min);
	int nSchemeNo = CGroupComboHelpers::GetGroupNoFromCombo(m_comboScheme, Scheme::SchemeNo.Min);
	CGroupComboHelpers::FillSchemeCombo(m_comboScheme, nGroupNo, nSchemeNo, TRUE, TRUE);
}

//$$******************************************************************

void CBatchPropertyPage5::OnToggleSetScheme()
{
	m_comboScheme.EnableWindow(m_checkSetScheme.GetCheck() != 0);
	m_comboScheme.SetFocus();
}

//$$******************************************************************

void CBatchPropertyPage5::OnToggleSetExpiry()
{
	m_checkDateExpiry.EnableWindow(IsTicked(m_checkSetExpiry));
	m_datePickerExpiry.EnableWindow(IsTicked(m_checkDateExpiry));
}

//$$******************************************************************

void CBatchPropertyPage5::OnToggleDateExpiry()
{
	m_datePickerExpiry.EnableWindow(IsTicked(m_checkDateExpiry));
}

//$$******************************************************************

void CBatchPropertyPage5::OnToggleSetDOB()
{
	m_checkDateDOB.EnableWindow(IsTicked(m_checkSetDOB));
	m_datePickerDOB.EnableWindow(IsTicked(m_checkDateDOB));
}

//$$******************************************************************

void CBatchPropertyPage5::OnToggleDateDOB()
{
	m_datePickerDOB.EnableWindow(IsTicked(m_checkDateDOB));
}

//$$******************************************************************

void CBatchPropertyPage5::OnToggleSetMaxSpend()
{
	GetEditMaxSpend() -> EnableWindow( m_checkSetMaxSpend.GetCheck() != 0 );
	GetEditMaxSpend() -> SetFocus();
}

//$$******************************************************************

void CBatchPropertyPage5::OnToggleSetOverSpend()
{
	GetEditOverSpend() -> EnableWindow( m_checkSetOverSpend.GetCheck() != 0 );
	GetEditOverSpend() -> SetFocus();
}

//$$******************************************************************

void CBatchPropertyPage5::OnToggleSetAlert()
{
	m_buttonAlert.EnableWindow(m_checkSetAlert.GetCheck() != 0);
	m_buttonAlert.SetFocus();

	if (m_checkSetAlert.GetCheck() == 0)
	{
		m_editAlert.SetWindowText("");
	}
	else
	{
		m_editAlert.SetWindowText(m_alertText.GetDisplayText(m_strAlertCodes));
	}
}

//$$******************************************************************

void CBatchPropertyPage5::OnButtonAlert()
{
	CSelectAlertDlg dlg{};

	dlg.m_strAlertCodes = m_strAlertCodes;
	if ( dlg.DoModal() == IDOK )
	{
		m_strAlertCodes = dlg.m_strAlertCodes;
		m_editAlert.SetWindowText(m_alertText.GetDisplayText(m_strAlertCodes)); 
	}
}

//$$******************************************************************

void CBatchPropertyPage5::OnToggleSetHotlist()
{
	m_comboHotlist.EnableWindow( m_checkSetHotlist.GetCheck() != 0 );
}

//$$******************************************************************

void CBatchPropertyPage5::OnToggleSetInactive()
{
	m_comboInactive.EnableWindow( m_checkSetInactive.GetCheck() != 0 );
}

//$$******************************************************************

void CBatchPropertyPage5::OnToggleSetGender()
{
	m_comboGender.EnableWindow( m_checkSetGender.GetCheck() != 0 );
}

//$$******************************************************************

bool CBatchPropertyPage5::HaveUpdates()
{
	return m_Memory.HaveUpdates();
}

//$$******************************************************************

bool CBatchPropertyPage5::UpdateRecord(CSQLRowAccountFull& RowAccount)
{
	return m_Memory.UpdateRecord(RowAccount);
}

//$$******************************************************************

BOOL CBatchPropertyPage5::OnKillActive()
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	UpdateMemory();
	return bResult;
}

//$$******************************************************************

