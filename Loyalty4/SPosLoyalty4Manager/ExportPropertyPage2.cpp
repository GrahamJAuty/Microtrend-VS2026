//$$******************************************************************
#include "..\SPosLoyalty4Shared\LoyaltyHeaderHelpers.h"
#include "..\SPosLoyalty4Shared\SQLNamespaces.h"
//$$******************************************************************
#include "CardPickerDlg.h"
#include "GroupComboHelpers.h"
#include "LoyaltyManager.h"
#include "SelectAlertDlg.h"
#include "StandardCombos.h"
//$$******************************************************************
#include "ExportPropertyPage2.h"
//$$******************************************************************

CExportPropertyPage2::CExportPropertyPage2()
	: CSSPropertyPage(CExportPropertyPage2::IDD)
{
	m_nMaxCardNoLength = System.GetMaxCardNoLength();
	m_nSingleGroupNo = 0;
	m_nCardRangeType = 0;
	m_alertText.Open(Filenames.GetAlertTextFilename());
	m_pDbExportHandler = NULL;
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
	DDX_Control(pDX, IDC_CHECK_VALID, m_checkValid);
	DDX_Control(pDX, IDC_CHECK_EXPIRED, m_checkExpired);
	DDX_Control(pDX, IDC_CHECK_HOTLISTED, m_checkHotlisted);
	DDX_Control(pDX, IDC_CHECK_INACTIVE, m_checkInactive);
	DDX_Control(pDX, IDC_CHECK_UNKNOWN, m_checkUnknown);
	/*****/
	DDX_Control(pDX, IDC_STATIC_ORDERBY, m_staticOrderBy);
	DDX_Control(pDX, IDC_COMBO_ORDERBY, m_comboOrderBy);
	DDX_Control(pDX, IDC_CHECK_SKIPIFBLANK, m_checkSkipBlanks);
	/*****/
	DDX_Control(pDX, IDC_RADIO_RANGE, m_radioRange);
	DDX_Control(pDX, IDC_RADIO_LIST, m_radioList);
	DDX_Control(pDX, IDC_EDIT_CARDNO_FROM, m_editCardNoFrom);
	DDX_Control(pDX, IDC_EDIT_CARDNO_TO, m_editCardNoTo);
	DDX_Control(pDX, IDC_BUTTON_LIST, m_buttonList);
	DDX_Control(pDX, IDC_COMBO_GROUP, m_comboGroup);
	DDX_Control(pDX, IDC_CHECK_SORTBYGROUP, m_checkSortByGroup);
	DDX_Control(pDX, IDC_CHECK_SUMMARY, m_checkGroupSummary);
	/*****/
	DDX_Control(pDX, IDC_COMBO_GENDER, m_comboGender);
	DDX_Control(pDX, IDC_STATIC_ALERT1, m_staticAlert[0]);
	DDX_Control(pDX, IDC_STATIC_ALERT2, m_staticAlert[1]);
	DDX_Control(pDX, IDC_STATIC_ALERT3, m_staticAlert[2]);
	DDX_Control(pDX, IDC_STATIC_ALERT4, m_staticAlert[3]);
	DDX_Control(pDX, IDC_STATIC_ALERT5, m_staticAlert[4]);
	DDX_Control(pDX, IDC_STATIC_ALERT6, m_staticAlert[5]);
	DDX_Control(pDX, IDC_STATIC_ALERT7, m_staticAlert[6]);
	DDX_Control(pDX, IDC_STATIC_ALERT8, m_staticAlert[7]);
	DDX_Control(pDX, IDC_STATIC_ALERT9, m_staticAlert[8]);
	DDX_Control(pDX, IDC_COMBO_ALERT1, m_comboAlert[0]);
	DDX_Control(pDX, IDC_COMBO_ALERT2, m_comboAlert[1]);
	DDX_Control(pDX, IDC_COMBO_ALERT3, m_comboAlert[2]);
	DDX_Control(pDX, IDC_COMBO_ALERT4, m_comboAlert[3]);
	DDX_Control(pDX, IDC_COMBO_ALERT5, m_comboAlert[4]);
	DDX_Control(pDX, IDC_COMBO_ALERT6, m_comboAlert[5]);
	DDX_Control(pDX, IDC_COMBO_ALERT7, m_comboAlert[6]);
	DDX_Control(pDX, IDC_COMBO_ALERT8, m_comboAlert[7]);
	DDX_Control(pDX, IDC_COMBO_ALERT9, m_comboAlert[8]);
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CExportPropertyPage2, CDialog)
	ON_BN_CLICKED(IDC_RADIO_RANGE, OnRadioRange)
	ON_BN_CLICKED(IDC_RADIO_LIST, OnRadioList)
	ON_BN_CLICKED(IDC_BUTTON_LIST, OnButtonList)
	ON_CBN_SELCHANGE(IDC_COMBO_GROUP, OnSelectGroup)
	ON_CBN_SELCHANGE(IDC_COMBO_ORDERBY, OnSelectOrderBy)
END_MESSAGE_MAP()

//$$******************************************************************

BOOL CExportPropertyPage2::OnInitDialog()
{
	m_Texts.Read();
	
	CPropertyPage::OnInitDialog();

	m_editCardNoFrom.LimitText(m_nMaxCardNoLength);
	m_editCardNoTo.LimitText(m_nMaxCardNoLength);

	if (m_pDbExportHandler->GetCardRangeIndex() == 0)
	{
		OnRadioRange();
	}
	else
	{
		OnRadioList();
	}

	//DO THIS AFTER ONRADIO() AS ONRADIO() MAY CHANGE CARD NUMBERS
	m_editCardNoFrom.SetWindowText(m_pDbExportHandler->GetStartCardNoString());
	m_editCardNoTo.SetWindowText(m_pDbExportHandler->GetEndCardNoString());

	int nGenderSel = 0;
	if (m_pDbExportHandler->GetGenderOption() != 0)
	{
		nGenderSel = m_pDbExportHandler->GetGenderCode() + 1;

		if ((nGenderSel < 0) || (nGenderSel > 3))
		{
			nGenderSel = 0;
		}
	}

	CStandardCombos::FillGenderCombo(m_comboGender, nGenderSel, TRUE);
	
	m_checkValid.SetCheck(m_pDbExportHandler->GetValidReqd());
	m_checkExpired.SetCheck(m_pDbExportHandler->GetExpiredReqd());
	m_checkHotlisted.SetCheck(m_pDbExportHandler->GetHotlistedReqd());
	m_checkInactive.SetCheck(m_pDbExportHandler->GetInactiveReqd());
	m_checkUnknown.SetCheck(m_pDbExportHandler->GetUnknownReqd());
	/*****/
	m_checkSkipBlanks.SetCheck(m_pDbExportHandler->GetSkipBlanks());
	/*****/
	m_checkSortByGroup.SetCheck(m_pDbExportHandler->GetSortByGroup());
	m_checkGroupSummary.SetCheck(m_pDbExportHandler->GetGroupSummary());
	/*****/

	//groups
	m_nSingleGroupNo = m_pDbExportHandler->GetGroupNo();
	CGroupComboHelpers::FillGroupCombo(m_comboGroup, m_nSingleGroupNo, TRUE);

	if (m_comboGroup.GetCount() < 3)		// no individual groups added
	{
		m_comboGroup.EnableWindow(FALSE);
	}

	//** SortOn

	int nOrderByCurSel = 0;
	CString strLabel = Account::UserID.Label;
	CString strOrderByLabel = m_pDbExportHandler->GetSortOnLabel();

	m_comboOrderBy.AddString(strLabel);				// setup default entry
	m_arrayOrderByLabels.Add(strLabel);				// set default label

	if (m_pDbExportHandler->m_bOrderByAllowed == FALSE)
	{
		m_staticOrderBy.ShowWindow(SW_HIDE);
		m_comboOrderBy.SetCurSel(0);					// needs to be set even though not displayed
		m_comboOrderBy.ShowWindow(SW_HIDE);				// so can extract default label on KillFocus	
		m_checkSkipBlanks.ShowWindow(SW_HIDE);
		MoveControl(&m_staticInclude, 7, 7, 238, 114);
	}
	else
	{
		CUserTexts texts;
		texts.Read();

		int nOrderByIndex = 0;
		if (strOrderByLabel == strLabel)
		{
			nOrderByCurSel = nOrderByIndex;
		}

		AddOrderByLabel(Account::Fullname.Label, Account::Fullname.Label, strOrderByLabel, nOrderByIndex, nOrderByCurSel);
		AddOrderByLabel(Account::Surname.Label, Account::Surname.Label, strOrderByLabel, nOrderByIndex, nOrderByCurSel);

		CString strText = "";

		switch (System.GetInterfaceType())
		{
		case nINTERFACE_MIFAREv2:
		case nINTERFACE_MIFAREv3: // ExtRef = Mifare card no.
			AddOrderByLabel(texts.GetText(LOYALTYFIELD_EXTERNALREF,""), Account::ExternalRef.Label, strOrderByLabel, nOrderByIndex, nOrderByCurSel);
			break;
		}

		AddOrderByLabel(m_Texts.GetText(LOYALTYFIELD_INFO1,""), Account::Info1.Label, strOrderByLabel, nOrderByIndex, nOrderByCurSel);
		AddOrderByLabel(m_Texts.GetText(LOYALTYFIELD_INFO2,""), Account::Info2.Label, strOrderByLabel, nOrderByIndex, nOrderByCurSel);
		AddOrderByLabel(m_Texts.GetText(LOYALTYFIELD_INFO3,""), Account::Info3.Label, strOrderByLabel, nOrderByIndex, nOrderByCurSel);
		AddOrderByLabel(m_Texts.GetText(LOYALTYFIELD_INFO4,""), Account::Info4.Label, strOrderByLabel, nOrderByIndex, nOrderByCurSel);

		AddOrderByLabel(m_Texts.GetText(LOYALTYFIELD_ADDRESS1, ""), Account::Address1.Label, strOrderByLabel, nOrderByIndex, nOrderByCurSel);
		AddOrderByLabel(m_Texts.GetText(LOYALTYFIELD_ADDRESS2, ""), Account::Address2.Label, strOrderByLabel, nOrderByIndex, nOrderByCurSel);
		AddOrderByLabel(m_Texts.GetText(LOYALTYFIELD_ADDRESS3, ""), Account::Address3.Label, strOrderByLabel, nOrderByIndex, nOrderByCurSel);
		AddOrderByLabel(m_Texts.GetText(LOYALTYFIELD_ADDRESS4, ""), Account::Address4.Label, strOrderByLabel, nOrderByIndex, nOrderByCurSel);
		AddOrderByLabel(m_Texts.GetText(LOYALTYFIELD_ADDRESS5, ""), Account::Address5.Label, strOrderByLabel, nOrderByIndex, nOrderByCurSel);

		m_comboOrderBy.SetCurSel(nOrderByCurSel);
		SetSkipText();
	}

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

	m_checkSortByGroup.ShowWindow(bSortByGroupReqd);
	m_checkGroupSummary.ShowWindow(bGroupSummaryReqd);
	m_checkUnknown.EnableWindow(m_pDbExportHandler->m_bUnknownAllowed);

	SetReportingRange();
	
	if (m_pDbExportHandler->m_strLabel == "R001")			// hotlist
	{
		m_checkValid.EnableWindow(FALSE);
		m_checkExpired.EnableWindow(FALSE);
		m_checkHotlisted.EnableWindow(FALSE);
		m_checkInactive.EnableWindow(FALSE);
	}

	OnSelectGroup();

	CArray<int, int> arrayAlertFilter;	
	CAlertText::ConvertFilterStringToArray(m_pDbExportHandler->GetAlertFilter(), arrayAlertFilter);
	
	for (int n = 1; n <= 9; n++)
	{
		CAlertTextInfo info;
		m_alertText.GetInfo(n, info);
		
		if (info.m_strText != "")
		{
			m_comboAlert[n - 1].AddString("Ignore");
			m_comboAlert[n - 1].AddString("Set");
			m_comboAlert[n - 1].AddString("Not Set");
			m_comboAlert[n - 1].SetCurSel(0);

			m_staticAlert[n - 1].SetWindowText(info.m_strText);

			switch (arrayAlertFilter[n-1])
			{
			case 1:	m_comboAlert[n - 1].SetCurSel(1);	break;
			case 2:	m_comboAlert[n - 1].SetCurSel(2);	break;
			}
		}
		else
		{
			m_comboAlert[n - 1].AddString("");
			m_comboAlert[n - 1].SetCurSel(0);
			m_staticAlert[n - 1].EnableWindow(FALSE);
			m_comboAlert[n - 1].EnableWindow(FALSE);
		}
	}
	
	return TRUE;
}

//$$******************************************************************

void CExportPropertyPage2::AddOrderByLabel(CString strComboLabel, CString strArrayLabel, CString strCheckLabel, int& nOrderByIndex, int& nOrderByCurSel)
{
	if (strComboLabel != "")
	{
		++nOrderByIndex;

		m_comboOrderBy.AddString(strComboLabel);
		m_arrayOrderByLabels.Add(strArrayLabel);

		if (strArrayLabel == strCheckLabel)
		{
			nOrderByCurSel = nOrderByIndex;
		}
	}
}

//$$******************************************************************

BOOL CExportPropertyPage2::OnKillActive()
{
	m_nSingleGroupNo = CGroupComboHelpers::GetGroupNoFromCombo(m_comboGroup,0);

	m_pDbExportHandler->SetStartCardNo(GetEditBoxText(m_editCardNoFrom));
	m_pDbExportHandler->SetEndCardNo(GetEditBoxText(m_editCardNoTo));
	m_pDbExportHandler->SetGroupNo(m_nSingleGroupNo);
	m_pDbExportHandler->SetValidReqd(IsTicked(m_checkValid));
	m_pDbExportHandler->SetHotlistedReqd(IsTicked(m_checkHotlisted));
	m_pDbExportHandler->SetExpiredReqd(IsTicked(m_checkExpired));
	m_pDbExportHandler->SetUnknownReqd(IsTicked(m_checkUnknown));
	m_pDbExportHandler->SetCardRangeIndex(m_nCardRangeType);
	m_pDbExportHandler->SetSortByGroup(IsTicked(m_checkSortByGroup));
	m_pDbExportHandler->SetGroupSummary(IsTicked(m_checkGroupSummary));
	m_pDbExportHandler->SetInactiveReqd(IsTicked(m_checkInactive));
	m_pDbExportHandler->SetSkipBlanks(IsTicked(m_checkSkipBlanks));
	m_pDbExportHandler->SetSortOn(m_arrayOrderByLabels.GetAt(m_comboOrderBy.GetCurSel()));

	{
		CString strFilter = "";
		for (int n = 0; n < 9; n++)
		{
			switch (m_comboAlert[n].GetCurSel())
			{
			case 1:		strFilter += "1";	break;
			case 2:		strFilter += "2";	break;
			default:	strFilter += "0";	break;
			}
		}

		m_pDbExportHandler->SetAlertFilter(strFilter);
	}

	int nGenderSel = m_comboGender.GetCurSel();
	if ((nGenderSel >= 1) && (nGenderSel <= 3))
	{
		m_pDbExportHandler->SetGenderOption(1);
		m_pDbExportHandler->SetGenderCode(nGenderSel - 1);
	}
	else
	{
		m_pDbExportHandler->SetGenderOption(0);
		m_pDbExportHandler->SetGenderCode(0);
	}

	return CPropertyPage::OnKillActive();
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
	bool bAllowGroup = (m_comboGroup.GetCount() > 2) ? TRUE : FALSE;

	if (m_nCardRangeType == 1)
	{
		bAllowRange = FALSE;
		bAllowGroup = FALSE;
		bAllowList = TRUE;
	}

	m_editCardNoFrom.EnableWindow(bAllowRange);
	m_editCardNoFrom.EnableWindow(bAllowRange);
	m_buttonList.EnableWindow(bAllowList);
	m_comboGroup.EnableWindow(bAllowGroup);
}

//$$******************************************************************

void CExportPropertyPage2::OnButtonList()
{
	CArray<CSQLRowAccountList, CSQLRowAccountList> arrayDummy;
	CString strReportList = Filenames.GetReportCardListFilename(m_pDbExportHandler->m_strLabel);
	CCardPickerDlg dlg( m_pDbExportHandler->m_strTitle, strReportList, arrayDummy, this);

	dlg.m_bValidAllowed = (IsTicked(m_checkValid));
	dlg.m_bExpiredAllowed = (IsTicked(m_checkExpired));
	dlg.m_bHotlistedAllowed = (IsTicked(m_checkHotlisted));

	dlg.DoModal();
}

//$$******************************************************************

void CExportPropertyPage2::OnSelectGroup()
{
	bool bAllowed = (m_comboGroup.GetCurSel() == 0) ? TRUE : FALSE;
	m_checkSortByGroup.EnableWindow(bAllowed);
}

//$$******************************************************************

void CExportPropertyPage2::OnSelectOrderBy()
{
	SetSkipText();
}

//$$******************************************************************

void CExportPropertyPage2::SetSkipText()
{
	bool bEnable = (m_arrayOrderByLabels.GetAt(m_comboOrderBy.GetCurSel()) == Account::UserID.Label) ? FALSE : TRUE;
	m_checkSkipBlanks.EnableWindow(bEnable);
}

//$$******************************************************************

