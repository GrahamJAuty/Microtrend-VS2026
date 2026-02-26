//$$******************************************************************
#include "..\SPosLoyalty4Shared\SQLDefines.h"
#include "..\SPosLoyalty4Shared\SQLNamespaces.h"
//$$******************************************************************
#include "GroupComboHelpers.h"
#include "LoyaltyManager.h"
#include "RefreshDaysDlg.h"
#include "RefreshWeeklyDlg.h"
#include "RefreshYearlyDlg.h"
#include "SetupGroupPeriodsDlg.h"
#include "StandardCombos.h"
//$$******************************************************************
#include "GroupPropertyPage1.h"
//$$******************************************************************

CGroupPropertyPage1::CGroupPropertyPage1(CSQLRowGroup& RowGroup)
	: CSSPropertyPage(CGroupPropertyPage1::IDD), m_RowGroup(RowGroup)
{
	m_bRefreshDay0 = FALSE;
	m_bRefreshDay1 = FALSE;
	m_bRefreshDay2 = FALSE;
	m_bRefreshDay3 = FALSE;
	m_bRefreshDay4 = FALSE;
	m_bRefreshDay5 = FALSE;
	m_bRefreshDay6 = FALSE;
	m_dMaxSpendValue = 0.0;
	m_dOverSpend = 0.0;
	m_dRefreshValue = 0.0;
	m_nRefreshWeeklyDay = 0;
	m_nRefreshYearlyDay = 1;
	m_nRefreshYearlyMonth = 1;
}

//$$******************************************************************

CGroupPropertyPage1::~CGroupPropertyPage1()
{
}

//$$******************************************************************

void CGroupPropertyPage1::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_NAME, m_editName);
	DDX_Control(pDX, IDC_COMBO_SCHEME, m_comboScheme);
	DDX_Control(pDX, IDC_CHECK_PLUBONUS, m_checkPluBonus);
	DDX_Control(pDX, IDC_CHECK_DEPTBONUS, m_checkDeptBonus);
	DDX_Control(pDX, IDC_CHECK_COMPLIMENTARY, m_checkComplimentary);
	DDX_Control(pDX, IDC_CHECK_STAMPOFFER, m_checkStampOffer);
	/*****/
	DDX_Control(pDX, IDC_COMBO_REFRESH, m_comboRefresh);
	DDX_Control(pDX, IDC_EDIT_REFRESHDAYS, m_editRefreshDays);
	DDX_Control(pDX, IDC_BUTTON_REFRESH, m_buttonRefresh);
	/*****/
	DDX_Control(pDX, IDC_COMBO_SPEND, m_comboSpend);
	DDX_Control(pDX, IDC_BUTTON_SPEND, m_buttonSpend);
	DDX_Control(pDX, IDC_STATIC_OVERSPEND, m_staticOverSpend);
	/*****/
	DDX_Text(pDX, IDC_EDIT_REFRESH, m_dRefreshValue);
	/*****/
	DDX_Text(pDX, IDC_EDIT_OVERSPEND, m_dOverSpend);
	DDX_Text(pDX, IDC_EDIT_MAXSPEND, m_dMaxSpendValue);
	/*****/

	if (TRUE == pDX->m_bSaveAndValidate)
	{
		if (ValidateMaxSpend(
			GetEditMaxSpend(),
			Group::MaxSpendPeriod1.Min,
			Group::MaxSpendPeriod1.Max,
			"Max Spend") == FALSE)
		{
			pDX->Fail();
			return;
		}

		if (ValidateMaxSpend(
			GetEditOverSpend(),
			Group::MaxOverdraftPeriod1.Min,
			Group::MaxOverdraftPeriod1.Max,
			"Overdraft") == FALSE)
		{
			pDX->Fail();
			return;
		}
	}
}

//$$******************************************************************

bool CGroupPropertyPage1::ValidateMaxSpend(CEdit* pEdit, double dMin, double dMax, CString strType)
{
	bool bResult = TRUE;

	if (pEdit->IsWindowEnabled() == TRUE)
	{
		double dValue = GetEditBoxDouble(*pEdit);

		if ((dValue < dMin) || (dValue > dMax))
		{
			CString strError = "";
			strError.Format("The %s value is out of range.\n\nPlease enter a value between %.2f and %.2f.",
				(const char*)strType,
				dMin,
				dMax);

			Prompter.Error(strError, "Error");
			pEdit->SetFocus();
			pEdit->SetSel(0, -1);
			bResult = FALSE;
		}
	}

	return bResult;
}

//*******************************************************

BEGIN_MESSAGE_MAP(CGroupPropertyPage1, CPropertyPage)
	ON_CBN_SELCHANGE(IDC_COMBO_REFRESH, OnSelectRefresh)
	ON_BN_CLICKED(IDC_BUTTON_REFRESH, OnButtonRefresh)
	/****/
	ON_CBN_SELCHANGE(IDC_COMBO_SPEND, OnSelectSpend)
	ON_BN_CLICKED(IDC_BUTTON_SPEND, OnButtonSpend)
END_MESSAGE_MAP()

//$$******************************************************************

BOOL CGroupPropertyPage1::OnInitDialog()
{
	m_dOverSpend = m_RowGroup.GetMaxOverdraftPeriod1();
	m_dRefreshValue = m_RowGroup.GetRefreshValue();

	if (m_RowGroup.GetMaxSpendType() == nSPENDTYPE_PERIOD)
	{
		m_dMaxSpendValue = 0;
		m_dOverSpend = 0;
	}
	else
	{
		m_dMaxSpendValue = m_RowGroup.GetMaxSpendPeriod1();
		m_dOverSpend = m_RowGroup.GetMaxOverdraftPeriod1();
	}

	m_nRefreshYearlyMonth = 1;
	m_nRefreshYearlyDay = 1;
	m_nRefreshWeeklyDay = 0;
	m_bRefreshDay0 = TRUE;
	m_bRefreshDay1 = FALSE;
	m_bRefreshDay2 = FALSE;
	m_bRefreshDay3 = FALSE;
	m_bRefreshDay4 = FALSE;
	m_bRefreshDay5 = FALSE;
	m_bRefreshDay6 = FALSE;

	switch (m_RowGroup.GetRefreshType())
	{
	case nREFRESHTYPE_DAILY:
		m_bRefreshDay0 = m_RowGroup.IsRefreshAllowed(0);
		m_bRefreshDay1 = m_RowGroup.IsRefreshAllowed(1);
		m_bRefreshDay2 = m_RowGroup.IsRefreshAllowed(2);
		m_bRefreshDay3 = m_RowGroup.IsRefreshAllowed(3);
		m_bRefreshDay4 = m_RowGroup.IsRefreshAllowed(4);
		m_bRefreshDay5 = m_RowGroup.IsRefreshAllowed(5);
		m_bRefreshDay6 = m_RowGroup.IsRefreshAllowed(6);
		break;

	case nREFRESHTYPE_WEEKLY:
		m_nRefreshWeeklyDay = m_RowGroup.GetRefreshWeeklyDayNo();
		break;

	case nREFRESHTYPE_MONTHLY:
		break;

	case nREFRESHTYPE_YEARLY:
		m_RowGroup.GetRefreshYearDate(m_nRefreshYearlyDay, m_nRefreshYearlyMonth);
		break;
	}

	CSSPropertyPage::OnInitDialog();

	SubclassEdit(IDC_EDIT_OVERSPEND, SS_NUM_SDP, 8, "%.2f");
	SubclassEdit(IDC_EDIT_REFRESH, SS_NUM_SDP, 11, "%.2f");
	SubclassEdit(IDC_EDIT_MAXSPEND, SS_NUM_SDP, 8, "%.2f");

	m_editName.LimitText(Group::GroupName.Max);
	m_editName.SetWindowText(m_RowGroup.GetGroupName());
	m_checkPluBonus.SetCheck(m_RowGroup.GetNoPluBonusFlag() == FALSE );
	m_checkDeptBonus.SetCheck(m_RowGroup.GetNoDeptBonusFlag() == FALSE );

	int nSchemeNo = m_RowGroup.GetSchemeNo();
	if (nSchemeNo == 0)
	{
		nSchemeNo = m_RowGroup.GetGroupNo();
	}

	CGroupComboHelpers::FillSchemeCombo(m_comboScheme, m_RowGroup.GetGroupNo(), nSchemeNo, FALSE, FALSE);
	
	m_checkStampOffer.SetCheck(m_RowGroup.GetStampOfferFlag());
	
	CStandardCombos::FillAutoRefreshCombo(m_comboRefresh, m_RowGroup.GetRefreshType());
	CStandardCombos::FillDailySpendCombo(m_comboSpend, m_RowGroup.GetMaxSpendType());

	Server.Read(Filenames.GetServerDataFilename());
	if (Server.GetAllowOverSpendFlag() == FALSE)
	{
		m_staticOverSpend.ShowWindow(SW_HIDE);
		GetEditOverSpend()->ShowWindow(SW_HIDE);
	}

	if (System.GetEnablePurse2Flag() == FALSE)
	{
		m_comboRefresh.EnableWindow(FALSE);
	}

	if (System.GetEnableFamilyFlag() == FALSE)
	{
		m_checkComplimentary.ShowWindow(SW_HIDE);
	}

	SetDisplayMaxSpend();
	SetAuditPeriodButton();
	OnSelectRefresh();

	return TRUE;
}

//$$******************************************************************
CEdit* CGroupPropertyPage1::GetEditOverSpend(){ return GetEdit( IDC_EDIT_OVERSPEND ); }
CEdit* CGroupPropertyPage1::GetEditMaxSpend(){ return GetEdit( IDC_EDIT_MAXSPEND ); }
CEdit* CGroupPropertyPage1::GetEditRefresh(){ return GetEdit( IDC_EDIT_REFRESH ); }
//$$******************************************************************

void CGroupPropertyPage1::OnButtonRefresh()
{
	if ( UpdateData() == TRUE )
	{
		switch( m_comboRefresh.GetCurSel() )
		{
		case nREFRESHTYPE_DAILY:
			{
				CRefreshDaysDlg dlg;
				dlg.m_bDay1 = m_bRefreshDay0;
				dlg.m_bDay2 = m_bRefreshDay1;
				dlg.m_bDay3 = m_bRefreshDay2;
				dlg.m_bDay4 = m_bRefreshDay3;
				dlg.m_bDay5 = m_bRefreshDay4;
				dlg.m_bDay6 = m_bRefreshDay5;
				dlg.m_bDay7 = m_bRefreshDay6;

				if ( dlg.DoModal() == IDOK )
				{
					m_bRefreshDay0 = ( dlg.m_bDay1 != 0 );
					m_bRefreshDay1 = ( dlg.m_bDay2 != 0 );
					m_bRefreshDay2 = ( dlg.m_bDay3 != 0 );
					m_bRefreshDay3 = ( dlg.m_bDay4 != 0 );
					m_bRefreshDay4 = ( dlg.m_bDay5 != 0 );
					m_bRefreshDay5 = ( dlg.m_bDay6 != 0 );
					m_bRefreshDay6 = ( dlg.m_bDay7 != 0 );
				}
			}
			break;

		case nREFRESHTYPE_WEEKLY:
			{
				CRefreshWeeklyDlg dlg;
				dlg.m_nRefreshDay = m_nRefreshWeeklyDay;

				if ( dlg.DoModal() == TRUE )
					m_nRefreshWeeklyDay = dlg.m_nRefreshDay;
			}
			break;

		case nREFRESHTYPE_YEARLY:		
			{
				CRefreshYearlyDlg dlg;
				dlg.m_oleDateTime.SetDate( 2013, m_nRefreshYearlyMonth, m_nRefreshYearlyMonth );

				if ( dlg.DoModal() == IDOK )
				{
					m_nRefreshYearlyDay = dlg.m_oleDateTime.GetDay();
					m_nRefreshYearlyMonth = dlg.m_oleDateTime.GetMonth();
				}
			}
			break;
		}

		DisplayRefreshDays();
	}
}

//$$******************************************************************

void CGroupPropertyPage1::DisplayRefreshDays()
{
	if ( UpdateData() == TRUE )
	{
		switch ( m_comboRefresh.GetCurSel() )
		{
		case nREFRESHTYPE_NORMAL:	m_editRefreshDays.SetWindowText("");
									break;

		case nREFRESHTYPE_DAILY:	m_RowGroup.SetRefreshAllowed( 0, m_bRefreshDay0 );
									m_RowGroup.SetRefreshAllowed( 1, m_bRefreshDay1 );
									m_RowGroup.SetRefreshAllowed( 2, m_bRefreshDay2 );
									m_RowGroup.SetRefreshAllowed( 3, m_bRefreshDay3 );
									m_RowGroup.SetRefreshAllowed( 4, m_bRefreshDay4 );
									m_RowGroup.SetRefreshAllowed( 5, m_bRefreshDay5 );
									m_RowGroup.SetRefreshAllowed( 6, m_bRefreshDay6 );
									m_editRefreshDays.SetWindowText(m_RowGroup.GetRefreshDaysText() );
									break;

		case nREFRESHTYPE_WEEKLY:	m_RowGroup.SetRefreshWeeklyDayNo ( m_nRefreshWeeklyDay );	// remove multiple entries (incase mutiple daily flags set )
									m_editRefreshDays.SetWindowText(m_RowGroup.GetRefreshWeekText() );
									break;

		case nREFRESHTYPE_MONTHLY:	m_editRefreshDays.SetWindowText(m_RowGroup.GetRefreshMonthText() );
									break;

		case nREFRESHTYPE_YEARLY:	m_RowGroup.SetRefreshYearDate( m_nRefreshYearlyDay, m_nRefreshYearlyMonth );
									m_editRefreshDays.SetWindowText(m_RowGroup.GetRefreshYearText() );
									break;
		}

		UpdateData(FALSE);
	}
}

//$$******************************************************************

void CGroupPropertyPage1::OnSelectRefresh()
{
	if ( UpdateData() == TRUE )
	{
		bool bAllowButton = TRUE;
		bool bAllowEdit = TRUE;
		
		switch( m_comboRefresh.GetCurSel() )
		{
		case nREFRESHTYPE_NORMAL:
			bAllowButton = FALSE;
			bAllowEdit = FALSE;
			break;

		case nREFRESHTYPE_MONTHLY:
			bAllowButton = FALSE;
			break;
		}

		m_buttonRefresh.EnableWindow(bAllowButton);
		GetEditRefresh() -> EnableWindow(bAllowEdit);
		m_editRefreshDays.EnableWindow(bAllowEdit);

		DisplayRefreshDays();
	}
}

//$$******************************************************************

void CGroupPropertyPage1::OnSelectSpend()
{
	if ( UpdateData() == TRUE )
	{
		SetDisplayMaxSpend();
		SetAuditPeriodButton();
	}
}

//$$******************************************************************

void CGroupPropertyPage1::SetDisplayMaxSpend()
{
	bool bAllowed = ( m_comboSpend.GetCurSel() == nSPENDTYPE_PERIOD ) ? FALSE : TRUE;
	GetEditMaxSpend()->EnableWindow(bAllowed);
	GetEditOverSpend()->EnableWindow(bAllowed);

}

//$$******************************************************************

void CGroupPropertyPage1::SetAuditPeriodButton()
{
	m_buttonSpend.EnableWindow(m_comboSpend.GetCurSel() == nSPENDTYPE_PERIOD);
}

//$$******************************************************************

void CGroupPropertyPage1::OnButtonSpend()
{
	if ( UpdateData() == TRUE )
	{
		bool bPurse2Allowed = ( m_comboRefresh.GetCurSel() != nREFRESHTYPE_NORMAL );
		CSetupGroupPeriodsDlg dlg(m_RowGroup, bPurse2Allowed, this);
		dlg.DoModal();
	}
}

//$$******************************************************************

BOOL CGroupPropertyPage1::OnKillActive()
{
	if (UpdateData() == TRUE)
	{
		int nRefreshType = m_comboRefresh.GetCurSel();
		int nMaxSpendType = m_comboSpend.GetCurSel();

		if ((nRefreshType != nREFRESHTYPE_NORMAL) && (m_dRefreshValue == 0.0))
		{
			if (Prompter.YesNo("The Purse2 Refresh Value has been set to zero for the group.\n\nPlease click Yes if this is intentional, or No to edit.") != IDYES)
			{
				GetEditRefresh()->SetFocus();
				return FALSE;
			}
		}

		CString strName = GetEditBoxText(m_editName);

		if (strName == "")
		{
			strName.Format("Group %d", m_RowGroup.GetGroupNo());
		}

		m_RowGroup.SetGroupName(strName);
		m_RowGroup.SetNoPluBonusFlag(IsTicked(m_checkPluBonus) == FALSE);
		m_RowGroup.SetNoDeptBonusFlag(IsTicked(m_checkDeptBonus) == FALSE);
		m_RowGroup.SetRefreshType(nRefreshType);
		m_RowGroup.SetRefreshValue(m_dRefreshValue);
		m_RowGroup.SetMaxOverdraftPeriod1(m_dOverSpend);
		m_RowGroup.SetMaxSpendType(nMaxSpendType);

		if (nMaxSpendType != nSPENDTYPE_PERIOD)
		{
			m_RowGroup.SetMaxOverdraftPeriod1(m_dOverSpend);
			m_RowGroup.SetMaxSpendPeriod1(m_dMaxSpendValue);
		}

		switch (m_comboRefresh.GetCurSel())
		{
		case nREFRESHTYPE_DAILY:
			m_RowGroup.SetRefreshAllowed(0, m_bRefreshDay0);
			m_RowGroup.SetRefreshAllowed(1, m_bRefreshDay1);
			m_RowGroup.SetRefreshAllowed(2, m_bRefreshDay2);
			m_RowGroup.SetRefreshAllowed(3, m_bRefreshDay3);
			m_RowGroup.SetRefreshAllowed(4, m_bRefreshDay4);
			m_RowGroup.SetRefreshAllowed(5, m_bRefreshDay5);
			m_RowGroup.SetRefreshAllowed(6, m_bRefreshDay6);
			break;

		case nREFRESHTYPE_WEEKLY:
			m_RowGroup.SetRefreshWeeklyDayNo(m_nRefreshWeeklyDay);
			break;

		case nREFRESHTYPE_MONTHLY:
			break;

		case nREFRESHTYPE_YEARLY:
			m_RowGroup.SetRefreshYearDate(m_nRefreshYearlyDay, m_nRefreshYearlyMonth);
			break;
		}

		int nSchemeNo = CGroupComboHelpers::GetGroupNoFromCombo(m_comboScheme, 0);

		if (nSchemeNo == m_RowGroup.GetGroupNo())
		{
			nSchemeNo = 0;
		}
		
		m_RowGroup.SetSchemeNo(nSchemeNo);
		
		m_RowGroup.SetStampOfferFlag(IsTicked(m_checkStampOffer));

		return CSSPropertyPage::OnKillActive();
	}
	else
	{
		return FALSE;
	}
}

//$$******************************************************************
