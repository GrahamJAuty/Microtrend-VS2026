//$$******************************************************************
#include "SmartPay4Manager.h"
//$$******************************************************************
#include "RefreshDaysDlg.h"
#include "RefreshWeeklyDlg.h"
#include "RefreshYearDlg.h"
#include "SetupGroupPeriods2Dlg.h"
#include "StandardCombos.h"
//$$******************************************************************
#include "GroupPropertyPage8.h"
//$$******************************************************************

CGroupPropertyPage8::CGroupPropertyPage8(CSQLRowGroupFull& RowGroup)
	: CSSAutoShutdownPropertyPage(AutoShutdownHelper, CGroupPropertyPage8::IDD)
	, m_RowGroup(RowGroup)
{
	m_bPurse2AppendRefresh = FALSE;
	m_dPurse1LowBalance = 0.0;
	m_dPurse2LowBalance = 0.0;
	m_dPurse2RefreshValue = 0.0;
	m_dPurse3LowBalance = 0.0;
	m_nPurse2RefreshType = 0;
}

//$$******************************************************************

CGroupPropertyPage8::~CGroupPropertyPage8()
{
}

//$$******************************************************************

void CGroupPropertyPage8::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_EDIT_PURSE1_DEPTSINHIBITED, m_editPurse1DeptInhibitList);
	/*****/
	DDX_Control(pDX, IDC_EDIT_PURSE2_DEPTSINHIBITED, m_editPurse2DeptInhibitList);
	DDX_Control(pDX, IDC_COMBO_PURSE2_TYPE, m_comboPurse2Type);
	DDX_Control(pDX, IDC_BUTTON_PURSE2_REFRESH, m_buttonPurse2Refresh);
	DDX_Control(pDX, IDC_CHECK_PURSE2_APPEND, m_checkPurse2AppendRefresh);
	DDX_Control(pDX, IDC_EDIT_PURSE2_REFRESHDAYS, m_editPurse2RefreshDays);
	DDX_Control(pDX, IDC_BUTTON_PURSE2_SET, m_buttonPurse2Set);
	/*****/
	DDX_Control(pDX, IDC_EDIT_PURSE3_DEPTSINHIBITED, m_editPurse3DeptInhibitList);
	/*****/
	DDX_Text(pDX, IDC_EDIT_PURSE1_DEPTSINHIBITED, m_strPurse1DeptInhibitList);
	DDX_Text(pDX, IDC_EDIT_PURSE1_LOWBALANCE, m_dPurse1LowBalance);
	/*****/
	DDX_Text(pDX, IDC_EDIT_PURSE2_DEPTSINHIBITED, m_strPurse2DeptInhibitList);
	DDX_Text(pDX, IDC_EDIT_PURSE2_LOWBALANCE, m_dPurse2LowBalance);
	DDX_CBIndex(pDX, IDC_COMBO_PURSE2_TYPE, m_nPurse2RefreshType);
	DDX_Text(pDX, IDC_EDIT_PURSE2_REFRESHDAYS, m_strPurse2RefreshDays);
	DDX_Text(pDX, IDC_EDIT_PURSE2_REFRESH, m_dPurse2RefreshValue);
	DDX_Check(pDX, IDC_CHECK_PURSE2_APPEND, m_bPurse2AppendRefresh);
	/*****/
	DDX_Text(pDX, IDC_EDIT_PURSE3_DEPTSINHIBITED, m_strPurse3DeptInhibitList);
	DDX_Text(pDX, IDC_EDIT_PURSE3_LOWBALANCE, m_dPurse3LowBalance);
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CGroupPropertyPage8, CSSAutoShutdownPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON_PURSE2_REFRESH, OnButtonPurse2Refresh)
	ON_CBN_SELCHANGE(IDC_COMBO_PURSE2_TYPE, OnSelectPurse2RefreshType)
	ON_BN_CLICKED(IDC_BUTTON_PURSE2_SET, OnButtonPurse2Set)
END_MESSAGE_MAP()

//$$******************************************************************

BOOL CGroupPropertyPage8::OnInitDialog()
{
	m_strPurse1DeptInhibitList		= m_RowGroup.GetDeptInhibitPurse1();
	m_dPurse1LowBalance				= m_RowGroup.GetLowBalancePurse1();
	/*****/
	m_strPurse2DeptInhibitList		= m_RowGroup.GetDeptInhibitPurse2();
	m_dPurse2LowBalance				= m_RowGroup.GetLowBalancePurse2();
	m_dPurse2RefreshValue			= m_RowGroup.GetRefreshValue();
	m_nPurse2RefreshType			= m_RowGroup.GetRefreshType();
	m_bPurse2AppendRefresh =		(m_RowGroup.GetRefreshAppendFlag() != 0);
	/*****/
	m_strPurse3DeptInhibitList		= m_RowGroup.GetDeptInhibitPurse3();
	m_dPurse3LowBalance				= m_RowGroup.GetLowBalancePurse3();
	
	CSSPropertyPage::OnInitDialog();
	CStandardCombos::FillRefreshPurseCombo( m_comboPurse2Type, m_nPurse2RefreshType );
	
	SubclassEdit ( IDC_EDIT_PURSE1_LOWBALANCE,	SS_NUM_SDP, 11, "%.2f" );
	SubclassEdit ( IDC_EDIT_PURSE2_LOWBALANCE,	SS_NUM_SDP, 11, "%.2f" );
	SubclassEdit ( IDC_EDIT_PURSE2_REFRESH,		SS_NUM_SDP, 11, "%.2f" ); 
	SubclassEdit ( IDC_EDIT_PURSE3_LOWBALANCE,	SS_NUM_SDP, 11, "%.2f" );
	
	if ( FALSE == System.GetEnablePurse3Flag() )
	{
		m_editPurse3DeptInhibitList.EnableWindow(FALSE);
		GetEditPurse3LowBalance()->EnableWindow(FALSE);
	}

	OnSelectPurse2RefreshType();
	
	return TRUE;
}

//$$******************************************************************
CEdit* CGroupPropertyPage8::GetEditPurse2LowBalance(){ return GetEdit( IDC_EDIT_PURSE2_LOWBALANCE ); }
CEdit* CGroupPropertyPage8::GetEditPurse2RefreshValue(){ return GetEdit( IDC_EDIT_PURSE2_REFRESH ); }
CEdit* CGroupPropertyPage8::GetEditPurse3LowBalance(){ return GetEdit( IDC_EDIT_PURSE3_LOWBALANCE ); }
//$$******************************************************************

BOOL CGroupPropertyPage8::OnSetActive()
{
	return CSSPropertyPage::OnSetActive();
}

//$$******************************************************************

BOOL CGroupPropertyPage8::OnKillActive()
{
	if ( UpdateData() == TRUE )
	{
		if (m_RowGroup.ValidateDeptInhibitList ( m_strPurse1DeptInhibitList ) == FALSE )
		{
			m_RowGroup.DisplayDeptListError();
			m_editPurse1DeptInhibitList.SetFocus();
			m_editPurse1DeptInhibitList.SetSel(0,-1);
			return FALSE;
		}

		/*****/

		if (m_RowGroup.ValidateDeptInhibitList ( m_strPurse2DeptInhibitList ) == FALSE )
		{
			m_RowGroup.DisplayDeptListError();
			m_editPurse2DeptInhibitList.SetFocus();
			m_editPurse2DeptInhibitList.SetSel(0,-1);
			return FALSE;
		}

		switch( m_nPurse2RefreshType )
		{
		case nREFRESHTYPE_DISABLED:
		case nREFRESHTYPE_MANUAL:
		case nREFRESHTYPE_PERIOD:
			break;
 
		default:
			if ( m_dPurse2RefreshValue == 0.0 )
			{
				Prompter.Error( "No Purse2 Refresh Value has been set!", "Input required" );
				GetEditPurse2RefreshValue() -> SetFocus();
				GetEditPurse2RefreshValue() -> SetSel(0,-1);
				return FALSE;
			}
			break;
		}

		switch( m_nPurse2RefreshType )
		{
		case nREFRESHTYPE_DAILY:
		case nREFRESHTYPE_PERIOD:
			if ( m_strPurse2RefreshDays == "" )
			{
				Prompter.Error( "No Purse2 Refresh on days have been set!", "Input required" );
				m_buttonPurse2Refresh.SetFocus();
				return FALSE;
			}
			break;
		}

		switch( m_nPurse2RefreshType )
		{
		case nREFRESHTYPE_PERIOD:
			if ( TRUE == m_bPurse2AppendRefresh )
			{
				Prompter.Error( "Cannot use 'carry forward balance' with Period Refresh!", "Invalid Entry" );
				m_comboPurse2Type.SetFocus();
				return FALSE;
			}
			break;
		}

		/*****/

		if (m_RowGroup.ValidateDeptInhibitList ( m_strPurse3DeptInhibitList ) == FALSE )
		{
			m_RowGroup.DisplayDeptListError();
			m_editPurse3DeptInhibitList.SetFocus();
			m_editPurse3DeptInhibitList.SetSel(0,-1);
			return FALSE;
		}

		m_RowGroup.SetDeptInhibitPurse1 ( m_strPurse1DeptInhibitList );
		m_RowGroup.SetLowBalancePurse1 ( m_dPurse1LowBalance );
		/*****/
		m_RowGroup.SetDeptInhibitPurse2 ( m_strPurse2DeptInhibitList );
		m_RowGroup.SetLowBalancePurse2 ( m_dPurse2LowBalance );
		m_RowGroup.SetRefreshType ( m_nPurse2RefreshType );
		m_RowGroup.SetRefreshValue ( m_dPurse2RefreshValue );
		m_RowGroup.SetRefreshAppendFlag ( m_bPurse2AppendRefresh ? 1 : 0 );
		/*****/
		if ( TRUE == System.GetEnablePurse3Flag() )
		{
			m_RowGroup.SetDeptInhibitPurse3 ( m_strPurse3DeptInhibitList );
			m_RowGroup.SetLowBalancePurse3 ( m_dPurse3LowBalance );
		}
	}

	return CSSPropertyPage::OnKillActive();
}

//$$******************************************************************

void CGroupPropertyPage8::OnButtonPurse2Refresh()
{
	if ( UpdateData() == TRUE )
	{
		switch( m_nPurse2RefreshType )
		{
		case nREFRESHTYPE_DAILY:
		case nREFRESHTYPE_PERIOD:
		{
			CRefreshDaysDlg dlg;

			for (int n = 1; n <= 7; n++)
			{
				dlg.m_bDayFlag[n - 1] = m_RowGroup.IsRefreshAllowed(n - 1);
			}
		
				if ( dlg.DoModal() == IDOK )
				{
					for (int n = 1; n <= 7; n++)
					{
						m_RowGroup.SetRefreshAllowed(n-1, (dlg.m_bDayFlag[n-1] != 0));
					}
				}
			}
			break;

		case nREFRESHTYPE_WEEKLY:
			{
				CRefreshWeeklyDlg dlg;
				dlg.m_nRefreshDay = m_RowGroup.GetRefreshWeeklyDayNo(System.GetStartOfWeek() - 1);

				if (dlg.DoModal() == TRUE)
				{
					m_RowGroup.SetRefreshWeeklyDayNo(dlg.m_nRefreshDay);
				}
			}
			break;

		case nREFRESHTYPE_YEARLY:				
			{
				int nDayNo;
				int nMonthNo;
				m_RowGroup.GetRefreshYearDate( nDayNo, nMonthNo );

				CRefreshYearDlg dlg;
				dlg.m_oleDateTime.SetDate( 2009, nMonthNo, nDayNo);

				if ( dlg.DoModal() == IDOK )
					m_RowGroup.SetRefreshYearDate( dlg.m_oleDateTime.GetDay(), dlg.m_oleDateTime.GetMonth() );

			}
			break;
		}

		DisplayPurse2RefreshDays();
	}
}

//$$******************************************************************

void CGroupPropertyPage8::DisplayPurse2RefreshDays()
{
	if ( UpdateData() == TRUE )
	{
		switch ( m_nPurse2RefreshType )
		{
		case nREFRESHTYPE_DISABLED:
		case nREFRESHTYPE_MANUAL:
		case nREFRESHTYPE_EOD:
			m_strPurse2RefreshDays = "";
									
			break;

		case nREFRESHTYPE_DAILY:
		case nREFRESHTYPE_PERIOD:	
			m_strPurse2RefreshDays = m_RowGroup.GetRefreshDaysText();
			break;

		case nREFRESHTYPE_WEEKLY:	
			m_RowGroup.SetRefreshWeeklyDayNo (m_RowGroup.GetRefreshWeeklyDayNo() );	// remove multiple entries (incase mutiple daily flags set )
			m_strPurse2RefreshDays = m_RowGroup.GetRefreshWeekText();
			break;

		case nREFRESHTYPE_MONTHLY:	
			m_strPurse2RefreshDays = m_RowGroup.GetRefreshMonthText();
			break;

		case nREFRESHTYPE_YEARLY:	
			m_strPurse2RefreshDays = m_RowGroup.GetRefreshYearText();
			break;
		}

		UpdateData(FALSE);
	}
}

//$$******************************************************************

void CGroupPropertyPage8::OnSelectPurse2RefreshType()
{
	if ( UpdateData() == TRUE )
	{
		bool bAllowEditRefresh = TRUE;
		bool bAllowCheckAppend = TRUE;
		bool bAllowInhibitDept = TRUE;
		bool bAllowRefreshDays = TRUE;
		bool bAllowEditLowBalance = FALSE;
		int nShowButtonSet = SW_HIDE;

		switch ( m_nPurse2RefreshType )
		{
		case nREFRESHTYPE_DISABLED:
			bAllowEditRefresh = FALSE;
			bAllowCheckAppend = FALSE;
			bAllowInhibitDept = FALSE;
			bAllowRefreshDays = FALSE;
			break;

		case nREFRESHTYPE_MANUAL:	
			bAllowEditRefresh = FALSE;
			bAllowCheckAppend = FALSE;
			bAllowInhibitDept = FALSE;
			bAllowRefreshDays = FALSE;
			bAllowEditLowBalance = TRUE;
			break;

		case nREFRESHTYPE_PERIOD:	
			bAllowEditRefresh = FALSE;
			bAllowCheckAppend = FALSE;
			nShowButtonSet = SW_SHOW;
			break;

		case nREFRESHTYPE_MONTHLY:	
		case nREFRESHTYPE_EOD:
			bAllowRefreshDays = FALSE;
			break;

		case nREFRESHTYPE_DAILY:
		case nREFRESHTYPE_WEEKLY:	
		case nREFRESHTYPE_YEARLY:	
			break;
		}

		GetEditPurse2RefreshValue() -> EnableWindow(bAllowEditRefresh);
		m_checkPurse2AppendRefresh.EnableWindow(bAllowCheckAppend);
		m_editPurse2DeptInhibitList.EnableWindow(bAllowInhibitDept);
		m_buttonPurse2Refresh.EnableWindow(bAllowRefreshDays);
		m_editPurse2RefreshDays.EnableWindow(bAllowRefreshDays);
		GetEditPurse2LowBalance()->EnableWindow(bAllowEditLowBalance);
		m_buttonPurse2Set.ShowWindow(nShowButtonSet);

		DisplayPurse2RefreshDays();
	}
}

//$$******************************************************************

void CGroupPropertyPage8::OnButtonPurse2Set()
{
	CSetupGroupPeriods2Dlg dlg (m_RowGroup, this );
	dlg.DoModal();
}

//$$******************************************************************
