//$$******************************************************************
#include "..\SmartPay4Shared\AlertText.h"
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
#include "..\SmartPay4Shared\SQLTable_UserText\SQLRepositoryUserText.h"
//$$******************************************************************
#include "CardPickerDlg.h"
#include "GroupComboHelpers.h"
#include "SmartPay4Manager.h"
//$$******************************************************************
#include "ExportPropertyPage2.h"
//$$******************************************************************

CExportPropertyPage2::CExportPropertyPage2()
	: CSSAutoShutdownPropertyPage(AutoShutdownHelper, CExportPropertyPage2::IDD)
{
	m_nCardRangeType = 0;
	m_pDbExportHandler = NULL;
	m_nTick1FilterType = 0;
	m_nTick2FilterType = 0;
	m_nTick3FilterType = 0;
	m_nTick4FilterType = 0;
	m_nSingleGroupNo = 0;
}

//$$******************************************************************

CExportPropertyPage2::~CExportPropertyPage2()
{
}

//$$******************************************************************

void CExportPropertyPage2::SetPointer(CDbExportHandler* pDbExportHandler)
{
	m_pDbExportHandler = pDbExportHandler;
}

//$$******************************************************************

void CExportPropertyPage2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_INCLUDE, m_staticInclude);
	DDX_Control(pDX, IDC_CHECK_VALID, m_checkIncludeValid);
	DDX_Control(pDX, IDC_CHECK_UNISSUED, m_checkIncludeUnissued);
	DDX_Control(pDX, IDC_CHECK_ALERT, m_checkIncludeAlert);
	DDX_Control(pDX, IDC_CHECK_INACTIVE, m_checkIncludeInactive);	
	DDX_Control(pDX, IDC_CHECK_EXPIRED, m_checkIncludeExpired);
	DDX_Control(pDX, IDC_CHECK_UNKNOWN, m_checkIncludeUnknown);
	DDX_Control(pDX, IDC_CHECK_GROUPSHIFT_NONE, m_checkIncludeGroupShiftNone);
	DDX_Control(pDX, IDC_CHECK_GROUPSHIFT_CURRENT, m_checkIncludeGroupShiftCurrent);
	DDX_Control(pDX, IDC_CHECK_GROUPSHIFT_FUTURE, m_checkIncludeGroupShiftFuture);
	/*****/
	DDX_Control(pDX, IDC_STATIC_TICKBOX, m_staticTickBox);
	DDX_Control(pDX, IDC_STATIC_TICK1, m_staticTick1);
	DDX_Control(pDX, IDC_RADIO_TICK1, m_radioTick1Ignore);
	DDX_Control(pDX, IDC_RADIO_TICKYES1, m_radioTick1Yes);
	DDX_Control(pDX, IDC_RADIO_TICKNO1, m_radioTick1No);
	DDX_Control(pDX, IDC_STATIC_TICK2, m_staticTick2);
	DDX_Control(pDX, IDC_RADIO_TICK2, m_radioTick2Ignore);
	DDX_Control(pDX, IDC_RADIO_TICKYES2, m_radioTick2Yes);
	DDX_Control(pDX, IDC_RADIO_TICKNO2, m_radioTick2No);
	DDX_Control(pDX, IDC_STATIC_TICK3, m_staticTick3);
	DDX_Control(pDX, IDC_RADIO_TICK3, m_radioTick3Ignore);
	DDX_Control(pDX, IDC_RADIO_TICKYES3, m_radioTick3Yes);
	DDX_Control(pDX, IDC_RADIO_TICKNO3, m_radioTick3No);
	DDX_Control(pDX, IDC_STATIC_TICK4, m_staticTick4);
	DDX_Control(pDX, IDC_RADIO_TICK4, m_radioTick4Ignore);
	DDX_Control(pDX, IDC_RADIO_TICKYES4, m_radioTick4Yes);
	DDX_Control(pDX, IDC_RADIO_TICKNO4, m_radioTick4No);
	/*****/
	DDX_Control(pDX, IDC_STATIC_USER, m_staticUser);
	DDX_Control(pDX, IDC_RADIO_RANGE, m_radioRange);
	DDX_Control(pDX, IDC_RADIO_LIST, m_radioList);
	DDX_Control(pDX, IDC_EDIT_CARDNO_FROM, m_editCardNoFrom);
	DDX_Control(pDX, IDC_EDIT_CARDNO_TO, m_editCardNoTo);
	DDX_Control(pDX, IDC_BUTTON_LIST, m_buttonList);
	DDX_Control(pDX, IDC_COMBO_GROUP, m_tabcomboGroup);
	DDX_Control(pDX, IDC_CHECK_SORTBYGROUP, m_checkSortByGroup);
	DDX_Control(pDX, IDC_CHECK_SUMMARY, m_checkGroupSummary);
	/*****/
	DDX_Control(pDX, IDC_STATIC_MISC, m_staticMisc);
	DDX_Control(pDX, IDC_STATIC_ORDERBY, m_staticOrderBy);
	DDX_Control(pDX, IDC_COMBO_ORDERBY, m_comboSortOn);
	DDX_Control(pDX, IDC_CHECK_SKIPONBLANK, m_checkSkipBlanks);
	DDX_Control(pDX, IDC_CHECK_TOTALISE, m_checkTotalise);
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CExportPropertyPage2, CSSAutoShutdownPropertyPage)

	ON_BN_CLICKED(IDC_RADIO_TICK1, OnRadioTick1Ignore)
	ON_BN_CLICKED(IDC_RADIO_TICKYES1, OnRadioTick1Yes)
	ON_BN_CLICKED(IDC_RADIO_TICKNO1, OnRadioTick1No)
	ON_BN_CLICKED(IDC_RADIO_TICK2, OnRadioTick2Ignore)
	ON_BN_CLICKED(IDC_RADIO_TICKYES2, OnRadioTick2Yes)
	ON_BN_CLICKED(IDC_RADIO_TICKNO2, OnRadioTick2No)
	ON_BN_CLICKED(IDC_RADIO_TICK3, OnRadioTick3Ignore)
	ON_BN_CLICKED(IDC_RADIO_TICKYES3, OnRadioTick3Yes)
	ON_BN_CLICKED(IDC_RADIO_TICKNO3, OnRadioTick3No)
	ON_BN_CLICKED(IDC_RADIO_TICK4, OnRadioTick4Ignore)
	ON_BN_CLICKED(IDC_RADIO_TICKYES4, OnRadioTick4Yes)
	ON_BN_CLICKED(IDC_RADIO_TICKNO4, OnRadioTick4No)
	/*****/
	ON_BN_CLICKED(IDC_RADIO_RANGE, OnRadioRange)
	ON_BN_CLICKED(IDC_RADIO_LIST, OnRadioList)
	ON_BN_DOUBLECLICKED(IDC_RADIO_LIST, OnDoubleClickRadioList)
	ON_BN_CLICKED(IDC_BUTTON_LIST, OnButtonList)
	ON_CBN_SELCHANGE(IDC_COMBO_GROUP, OnSelectGroup)
	/*****/
	ON_CBN_SELCHANGE(IDC_COMBO_ORDERBY, OnSelectOrderBy)

END_MESSAGE_MAP()

//$$******************************************************************

void CExportPropertyPage2::AddSortOnOption(CString strLabel, CString strComboText, CString& strSortOnLabel, int& nSortOnIndex, int& nSortOnCurSel)
{
	if ( strComboText != "")
	{
		++nSortOnIndex;
		m_comboSortOn.AddString(strComboText);
		m_arraySortOnLabels.Add(strLabel);
		
		if (strSortOnLabel == strLabel)
		{
			nSortOnCurSel = nSortOnIndex;
		}
	}
}

//$$******************************************************************

void CExportPropertyPage2::CheckTicks(CString strText, CStatic& static1, CButton& radio1, CButton& radio2, CButton& radio3, int& nTickCount)
{
	if (strText == "")
	{
		static1.ShowWindow(SW_HIDE);
		radio1.ShowWindow(SW_HIDE);
		radio2.ShowWindow(SW_HIDE);
		radio3.ShowWindow(SW_HIDE);
		--nTickCount;
	}
	else
	{
		static1.SetWindowText(strText);
	}
}

//$$******************************************************************

BOOL CExportPropertyPage2::OnInitDialog()
{
	int nGroupCurSel = 0;
	
	m_nSingleGroupNo = m_pDbExportHandler->GetGroupNo();

	CPropertyPage::OnInitDialog();

	if (m_pDbExportHandler->GetCardRangeIndex() == 0)
	{
		OnRadioRange();
	}
	else
	{
		OnRadioList();
	}

	m_editCardNoFrom.LimitText(10);
	m_editCardNoTo.LimitText(10);

	m_editCardNoFrom.SetWindowText(m_pDbExportHandler->GetStartCardNoString());
	m_editCardNoTo.SetWindowText(m_pDbExportHandler->GetEndCardNoString());

	m_checkIncludeValid.SetCheck(m_pDbExportHandler->GetValidReqd());
	m_checkIncludeInactive.SetCheck(m_pDbExportHandler->GetInactiveReqd());
	m_checkIncludeExpired.SetCheck(m_pDbExportHandler->GetExpiredReqd());
	m_checkIncludeUnknown.SetCheck(m_pDbExportHandler->GetUnknownReqd());
	m_checkIncludeAlert.SetCheck(m_pDbExportHandler->GetAlertReqd());
	m_checkIncludeUnissued.SetCheck(m_pDbExportHandler->GetUnissuedReqd());
	m_checkIncludeGroupShiftNone.SetCheck(m_pDbExportHandler->GetGroupShiftNoneReqd());
	m_checkIncludeGroupShiftCurrent.SetCheck(m_pDbExportHandler->GetGroupShiftCurrentReqd());
	m_checkIncludeGroupShiftFuture.SetCheck(m_pDbExportHandler->GetGroupShiftFutureReqd());

	m_checkSortByGroup.SetCheck(m_pDbExportHandler->GetSortByGroup());
	m_checkGroupSummary.SetCheck(m_pDbExportHandler->GetGroupSummary());
	m_checkSkipBlanks.SetCheck(m_pDbExportHandler->GetSkipBlanks());

	m_checkTotalise.SetCheck(m_pDbExportHandler->GetTotalise());

	SetRadioTick1(m_pDbExportHandler->GetTick1());
	SetRadioTick2(m_pDbExportHandler->GetTick2());
	SetRadioTick3(m_pDbExportHandler->GetTick3());
	SetRadioTick4(m_pDbExportHandler->GetTick4());

	// groups ******************
	CGroupComboHelpers::FillGroupCombo(m_tabcomboGroup, m_nSingleGroupNo, TRUE);

	// SortOn ********************

	int nSortOnIndex = -1;
	int nSortOnCurSel = 0;
	CString strSortOnLabel = m_pDbExportHandler->GetSortOnLabel();

	AddSortOnOption(Account::UserID.Label, Account::UserID.Label, strSortOnLabel, nSortOnIndex, nSortOnCurSel);

	if (m_pDbExportHandler->m_bSortOnAllowed == FALSE)
	{
		m_comboSortOn.SetCurSel(0);						// needs to be set even though not displayed
		m_comboSortOn.ShowWindow(SW_HIDE);				// so can extract default label on KillFocus	
		m_staticOrderBy.ShowWindow(SW_HIDE);
		m_checkSkipBlanks.ShowWindow(SW_HIDE);
	}
	else
	{
		CSQLRepositoryUserText Repo;
		AddSortOnOption(Account::MemberID.Label, Repo.GetMemberIDText(), strSortOnLabel, nSortOnIndex, nSortOnCurSel);
		AddSortOnOption(Account::Surname.Label, Account::Surname.Label, strSortOnLabel, nSortOnIndex, nSortOnCurSel);
		AddSortOnOption(Account::Username.Label, Account::Username.Label, strSortOnLabel, nSortOnIndex, nSortOnCurSel);

		switch (System.GetInterfaceType())
		{
		case nINTERFACE_MIFAREv2:	
		case nINTERFACE_DUAL:
			AddSortOnOption(Account::ExternalRef1.Label, Repo.GetExtRef1Text(), strSortOnLabel, nSortOnIndex, nSortOnCurSel);
			AddSortOnOption(Account::ExternalRef2.Label, Repo.GetExtRef2Text(), strSortOnLabel, nSortOnIndex, nSortOnCurSel);
			break;
		}

		AddSortOnOption(Account::Info1.Label, Repo.GetInfo1Text(), strSortOnLabel, nSortOnIndex, nSortOnCurSel);
		AddSortOnOption(Account::Info2.Label, Repo.GetInfo2Text(), strSortOnLabel, nSortOnIndex, nSortOnCurSel);
		AddSortOnOption(Account::Info3.Label, Repo.GetInfo3Text(), strSortOnLabel, nSortOnIndex, nSortOnCurSel);
		AddSortOnOption(Account::Info4.Label, Repo.GetInfo4Text(), strSortOnLabel, nSortOnIndex, nSortOnCurSel);
		AddSortOnOption(Account::Info5.Label, Repo.GetInfo5Text(), strSortOnLabel, nSortOnIndex, nSortOnCurSel);
		AddSortOnOption(Account::Info6.Label, Repo.GetInfo6Text(), strSortOnLabel, nSortOnIndex, nSortOnCurSel);
		AddSortOnOption(Account::Info7.Label, Repo.GetInfo7Text(), strSortOnLabel, nSortOnIndex, nSortOnCurSel);
		AddSortOnOption(Account::Info8.Label, Repo.GetInfo8Text(), strSortOnLabel, nSortOnIndex, nSortOnCurSel);

		m_comboSortOn.SetCurSel(nSortOnCurSel);
		SetSkipText();
	}

	// other  *********************

	bool bGroupSummaryReqd = FALSE;
	bool bSortByGroupReqd = FALSE;
	
	if (m_pDbExportHandler->m_nExportFlag != nDBEXPORTFIELDS_DATA)
	{
		if (m_pDbExportHandler->m_bCardListAllowed == FALSE)
		{
			m_radioList.EnableWindow(FALSE);
			m_buttonList.EnableWindow(FALSE);
		}

		bSortByGroupReqd = m_pDbExportHandler->m_bGroupSortAllowed;
		bGroupSummaryReqd = m_pDbExportHandler->m_bGroupSummaryAllowed;
	}

	if (FALSE == m_pDbExportHandler->m_bTotaliseAllowed)
	{
		m_checkTotalise.ShowWindow(SW_HIDE);
	}

	if (FALSE == m_pDbExportHandler->m_bSortOnAllowed)
	{
		m_comboSortOn.SetCurSel(0);						// needs to be set even though not displayed
		m_comboSortOn.ShowWindow(SW_HIDE);				// so can extract default label on KillFocus	
		m_staticOrderBy.ShowWindow(SW_HIDE);
		m_checkSkipBlanks.ShowWindow(SW_HIDE);
	}

	if ( (FALSE == m_pDbExportHandler->m_bTotaliseAllowed) && (FALSE == m_pDbExportHandler->m_bSortOnAllowed) )
	{
		m_staticMisc.ShowWindow(SW_HIDE);
		MoveControl(&m_staticUser, 232, 7, 182, 246);
	}

	m_checkSortByGroup.ShowWindow(bSortByGroupReqd);
	m_checkGroupSummary.ShowWindow(bGroupSummaryReqd);
	m_checkIncludeUnknown.EnableWindow(m_pDbExportHandler->m_bUnknownAllowed);

	SetReportingRange();

	CAlertText alert;
	alert.Open(Filenames.GetAlertFilename());
	if (alert.GetAlertCount() == 0)
	{
		m_checkIncludeAlert.ShowWindow(SW_HIDE);
		MoveControl(&m_checkIncludeInactive, 22, 66);
		MoveControl(&m_checkIncludeExpired, 22, 87);
		MoveControl(&m_checkIncludeUnknown, 22, 108);
	}
	else
	{
		if (System.GetInterfaceType() == nINTERFACE_MIFAREv1)
		{
			m_checkIncludeAlert.SetWindowText("Hotlist");
		}
	}

	// user ticks
	int nTickCount = 4;
	CSQLRepositoryUserText Repo;
	CheckTicks(Repo.GetTick1Text(), m_staticTick1, m_radioTick1Ignore, m_radioTick1Yes, m_radioTick1No, nTickCount);
	CheckTicks(Repo.GetTick2Text(), m_staticTick2, m_radioTick2Ignore, m_radioTick2Yes, m_radioTick2No, nTickCount);
	CheckTicks(Repo.GetTick3Text(), m_staticTick3, m_radioTick3Ignore, m_radioTick3Yes, m_radioTick3No, nTickCount);
	CheckTicks(Repo.GetTick4Text(), m_staticTick4, m_radioTick4Ignore, m_radioTick4Yes, m_radioTick4No, nTickCount);

	if (nTickCount == 0)
	{
		m_staticTickBox.ShowWindow(SW_HIDE);
		MoveControl(&m_staticInclude, 7, 7, 216, 246);
	}

	if (System.GetInterfaceType() == nINTERFACE_BIOMETRIC || System.GetInterfaceType() == nINTERFACE_MIFAREv1)
	{
		m_checkIncludeUnissued.EnableWindow(FALSE);
	}

	OnSelectGroup();

	return TRUE;
}

//$$******************************************************************
void CExportPropertyPage2::OnRadioTick1Ignore() { SetRadioTick1(0); }
void CExportPropertyPage2::OnRadioTick1Yes() { SetRadioTick1(1); }
void CExportPropertyPage2::OnRadioTick1No() { SetRadioTick1(2); }
//$$******************************************************************

void CExportPropertyPage2::SetRadioTick1(int n)
{
	m_radioTick1Ignore.SetCheck(0 == n);
	m_radioTick1Yes.SetCheck(1 == n);
	m_radioTick1No.SetCheck(2 == n);
	m_nTick1FilterType = n;
}

//$$******************************************************************
void CExportPropertyPage2::OnRadioTick2Ignore() { SetRadioTick2(0); }
void CExportPropertyPage2::OnRadioTick2Yes() { SetRadioTick2(1); }
void CExportPropertyPage2::OnRadioTick2No() { SetRadioTick2(2); }
//$$******************************************************************

void CExportPropertyPage2::SetRadioTick2(int n)
{
	m_radioTick2Ignore.SetCheck(0 == n);
	m_radioTick2Yes.SetCheck(1 == n);
	m_radioTick2No.SetCheck(2 == n);
	m_nTick2FilterType = n;
}

//$$******************************************************************
void CExportPropertyPage2::OnRadioTick3Ignore() { SetRadioTick3(0); }
void CExportPropertyPage2::OnRadioTick3Yes() { SetRadioTick3(1); }
void CExportPropertyPage2::OnRadioTick3No() { SetRadioTick3(2); }
//$$******************************************************************

void CExportPropertyPage2::SetRadioTick3(int n)
{
	m_radioTick3Ignore.SetCheck(0 == n);
	m_radioTick3Yes.SetCheck(1 == n);
	m_radioTick3No.SetCheck(2 == n);
	m_nTick3FilterType = n;
}

//$$******************************************************************
void CExportPropertyPage2::OnRadioTick4Ignore() { SetRadioTick4(0); }
void CExportPropertyPage2::OnRadioTick4Yes() { SetRadioTick4(1); }
void CExportPropertyPage2::OnRadioTick4No() { SetRadioTick4(2); }
//$$******************************************************************

void CExportPropertyPage2::SetRadioTick4(int n)
{
	m_radioTick4Ignore.SetCheck(0 == n);
	m_radioTick4Yes.SetCheck(1 == n);
	m_radioTick4No.SetCheck(2 == n);
	m_nTick4FilterType = n;
}

//$$******************************************************************

void CExportPropertyPage2::OnRadioRange()
{
	m_radioRange.SetCheck(TRUE);
	m_radioList.SetCheck(FALSE);
	m_nCardRangeType = 0;

	m_editCardNoFrom.SetWindowText(System.GetLowCardNo());
	m_editCardNoTo.SetWindowText(System.GetHighCardNo());
	SetReportingRange();
}

//$$******************************************************************

void CExportPropertyPage2::OnRadioList()
{ 
	m_radioRange.SetCheck(FALSE);
	m_radioList.SetCheck(TRUE);
	m_nCardRangeType = 1;
	SetReportingRange(); 
}

//$$******************************************************************

void CExportPropertyPage2::SetReportingRange()
{
	bool bAllowRange = TRUE;
	bool bAllowList = FALSE;
	bool bAllowGroup = (m_tabcomboGroup.GetCount() < 2) ? FALSE : TRUE;

	if (m_nCardRangeType == 1)
	{
		bAllowRange = FALSE;
		bAllowGroup = FALSE;
		bAllowList = TRUE;
	}

	m_editCardNoFrom.EnableWindow(bAllowRange);
	m_editCardNoTo.EnableWindow(bAllowRange);
	m_buttonList.EnableWindow(bAllowList);
	m_tabcomboGroup.EnableWindow(bAllowGroup);
}

//$$******************************************************************
void CExportPropertyPage2::OnDoubleClickRadioList() { OnButtonList(); }
//$$******************************************************************

void CExportPropertyPage2::OnButtonList()
{
	CArray<CSQLRowAccountList, CSQLRowAccountList> arrayDummy;
	CString strReportList = Filenames.GetReportCardListFilename(m_pDbExportHandler->m_strLabel);
	CCardPickerDlg dlg( m_pDbExportHandler->m_strTitle, strReportList, arrayDummy, this);

	dlg.m_bValidAllowed = (IsTicked(m_checkIncludeValid));
	dlg.m_bExpiredAllowed = (IsTicked(m_checkIncludeExpired));
	dlg.m_bInactiveAllowed = (IsTicked(m_checkIncludeInactive));

	dlg.DoModal();
}

//$$******************************************************************

void CExportPropertyPage2::OnSelectGroup()
{
	m_checkSortByGroup.EnableWindow(m_tabcomboGroup.GetCurSel() == 0);
}

//$$******************************************************************

void CExportPropertyPage2::OnSelectOrderBy()
{
	SetSkipText();
}

//$$******************************************************************

void CExportPropertyPage2::SetSkipText()
{
	bool bEnable = (m_arraySortOnLabels.GetAt(m_comboSortOn.GetCurSel()) == Account::UserID.Label) ? FALSE : TRUE;
	m_checkSkipBlanks.EnableWindow(bEnable);
	m_checkTotalise.EnableWindow(bEnable);
}

//$$******************************************************************

BOOL CExportPropertyPage2::OnKillActive()
{
	m_nSingleGroupNo = 0;

	{
		int nSel = m_tabcomboGroup.GetCurSel();
		if (nSel > 0)
		{
			m_nSingleGroupNo = CGroupComboHelpers::GetGroupNoFromCombo(m_tabcomboGroup, 0);
		}
	}

	m_pDbExportHandler->SetStartCardNo(GetEditBoxText(m_editCardNoFrom));
	m_pDbExportHandler->SetEndCardNo(GetEditBoxText(m_editCardNoTo));
	m_pDbExportHandler->SetGroupNo(m_nSingleGroupNo);
	m_pDbExportHandler->SetValidReqd(IsTicked(m_checkIncludeValid));
	m_pDbExportHandler->SetInactiveReqd(IsTicked(m_checkIncludeInactive));
	m_pDbExportHandler->SetExpiredReqd(IsTicked(m_checkIncludeExpired));
	m_pDbExportHandler->SetUnknownReqd(IsTicked(m_checkIncludeUnknown));
	m_pDbExportHandler->SetGroupShiftNoneReqd(IsTicked(m_checkIncludeGroupShiftNone));
	m_pDbExportHandler->SetGroupShiftCurrentReqd(IsTicked(m_checkIncludeGroupShiftCurrent));
	m_pDbExportHandler->SetGroupShiftFutureReqd(IsTicked(m_checkIncludeGroupShiftFuture));
	m_pDbExportHandler->SetCardRangeIndex(m_nCardRangeType);
	m_pDbExportHandler->SetSortByGroup(IsTicked(m_checkSortByGroup));
	m_pDbExportHandler->SetGroupSummary(IsTicked(m_checkGroupSummary));
	m_pDbExportHandler->SetSkipBlanks(IsTicked(m_checkSkipBlanks));
	m_pDbExportHandler->SetAlertReqd(IsTicked(m_checkIncludeAlert));
	m_pDbExportHandler->SetTick1(m_nTick1FilterType);
	m_pDbExportHandler->SetTick2(m_nTick2FilterType);
	m_pDbExportHandler->SetTick3(m_nTick3FilterType);
	m_pDbExportHandler->SetTick4(m_nTick4FilterType);
	m_pDbExportHandler->SetUnissuedReqd(IsTicked(m_checkIncludeUnissued));
	m_pDbExportHandler->SetSortOn(m_arraySortOnLabels.GetAt(m_comboSortOn.GetCurSel()));
	m_pDbExportHandler->SetTotalise(IsTicked(m_checkTotalise));

	return CPropertyPage::OnKillActive();
}

//$$******************************************************************
