//$$******************************************************************
#include "GroupComboHelpers.h"
#include "LoyaltyManager.h"
#include "ReportDefaultHandler.h"
#include "CardPickerDlg.h"
#include "SelectAlertDlg.h"
#include "BatchUpdate.h"
#include "StandardCombos.h"
//$$******************************************************************
#include "BatchPropertyPage1.h"
//$$******************************************************************

CBatchPropertyPage1::CBatchPropertyPage1()
	: CSSPropertyPage(CBatchPropertyPage1::IDD)
	, m_Comments(Filenames.GetAuditCommentsFilename())
{
	m_strTitle = "";
	m_strLabel = "";
	m_nUpdateType = 0;
	m_nSingleGroupNo = 0;
	m_nMaxCardNoLength = System.GetMaxCardNoLength();
	m_pUpdateHelpers = NULL;

	m_alertText.Open(Filenames.GetAlertTextFilename());
}

//$$******************************************************************

void CBatchPropertyPage1::SetPointer( CBatchUpdateHelpers* pUpdateHelpers, const char* szTitle, int nUpdateType)
{
	m_strTitle = szTitle;
	m_nUpdateType = nUpdateType;
	m_pUpdateHelpers = pUpdateHelpers;
}

//$$******************************************************************

void CBatchPropertyPage1::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_VALID, m_checkValid);
	DDX_Control(pDX, IDC_CHECK_EXPIRED, m_checkExpired);
	DDX_Control(pDX, IDC_CHECK_HOTLISTED, m_checkHotlisted);
	DDX_Control(pDX, IDC_CHECK_INACTIVE, m_checkInactive);
	/*****/
	DDX_Control(pDX, IDC_RADIO_RANGE, m_radioRange);
	DDX_Control(pDX, IDC_RADIO_LIST, m_radioList);
	DDX_Control(pDX, IDC_EDIT_CARDNO_FROM, m_editCardNoFrom);
	DDX_Control(pDX, IDC_EDIT_CARDNO_TO, m_editCardNoTo);
	DDX_Control(pDX, IDC_BUTTON_LIST, m_buttonList);
	DDX_Control(pDX, IDC_COMBO_GROUP, m_comboGroup);
	DDX_Control(pDX, IDC_CHECK_BIRTHDAY, m_checkBirthday);
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
	/*****/
	DDX_Control(pDX, IDC_STATIC_COMMENT, m_staticComment);
	DDX_Control(pDX, IDC_COMBO_COMMENT, m_comboComment);
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CBatchPropertyPage1, CPropertyPage)
	ON_BN_CLICKED(IDC_RADIO_RANGE, OnRadioRange)
	ON_BN_CLICKED(IDC_RADIO_LIST, OnRadioList)
	ON_BN_CLICKED(IDC_BUTTON_LIST, OnButtonList)
	ON_STN_CLICKED(IDC_STATIC_COMMENT, OnStaticComment)
END_MESSAGE_MAP()

//$$******************************************************************

BOOL CBatchPropertyPage1::OnInitDialog()
{
	int nCardRangeType = 0;
	CString strCardNoFrom = System.GetLowCardNo();
	CString strCardNoTo = System.GetHighCardNo();
	bool bIncludeValid = FALSE;
	bool bIncludeExpired = FALSE;
	bool bIncludeHotlisted = FALSE;
	bool bIncludeInactive = FALSE;
	int nGenderOption = 0;
	int nGenderCode = 0;
	CString strAlertFilter = "";
	CString strCommentMsg = "";
	bool bBirthday = FALSE;

	if ( m_nUpdateType == nBATCHUPDATE_UPDATE )
	{
		m_strLabel = "R107";								// Batch updates
		strCommentMsg = "Batch Update comment.";

		CString strLine;
		CReportDefaultHandler repdefs;
		if (repdefs.Read(m_strLabel, strLine) == TRUE)
		{
			CCSV csv(strLine);
			if (csv.IsEmpty(0) == FALSE)	nCardRangeType = csv.GetInt(0);
			if (csv.IsEmpty(1) == FALSE)	strCardNoFrom = csv.GetString(1);
			if (csv.IsEmpty(2) == FALSE)	strCardNoTo = csv.GetString(2);
			if (csv.IsEmpty(3) == FALSE)	m_nSingleGroupNo = csv.GetInt(3);
			if (csv.IsEmpty(4) == FALSE)	bIncludeValid = csv.GetBool(4);
			if (csv.IsEmpty(5) == FALSE)	bIncludeExpired = csv.GetBool(5);
			if (csv.IsEmpty(6) == FALSE)	bIncludeHotlisted = csv.GetBool(6);
			if (csv.IsEmpty(7) == FALSE)	bIncludeInactive = csv.GetBool(7);
			//8 was VALIDATE ON ALERT
			//9 was ALERT CODE
			if (csv.IsEmpty(10) == FALSE)	nGenderOption = csv.GetInt(10);
			if (csv.IsEmpty(11) == FALSE)	nGenderCode = csv.GetInt(11);
			if (csv.IsEmpty(12) == FALSE)	strAlertFilter = csv.GetString(12);
			if (csv.IsEmpty(13) == FALSE)	bBirthday = csv.GetBool(13);
		}
	}
	else if ( m_nUpdateType == nBATCHUPDATE_DELETE )
	{
		m_strLabel = "R108";									// Batch deletes - always make user enter selection
		strCommentMsg = "Batch Delete comment.";
		::SendToRecycleBin ( Filenames.GetReportCardListFilename(m_strLabel));	// delete last list file, if found
	}
	else		// nBATCHUPDATE_REDEEM
	{
		m_strLabel = "R111";
		bIncludeValid = TRUE;									// default to update valid record
		strCommentMsg = "Batch Redeem comment.";
		::SendToRecycleBin ( Filenames.GetReportCardListFilename(m_strLabel));	// delete last list file, if found
	}

	CPropertyPage::OnInitDialog();

	m_editCardNoFrom.LimitText(m_nMaxCardNoLength);
	m_editCardNoTo.LimitText(m_nMaxCardNoLength);

	if (0 == nCardRangeType)
	{
		OnRadioRange();
	}
	else
	{
		OnRadioList();
	}

	//DO THIS AFTER ONRADIO() AS ONRADIO() MAY CHANGE CARD NUMBERS
	m_editCardNoFrom.SetWindowText(strCardNoFrom);
	m_editCardNoTo.SetWindowText(strCardNoTo);

	int nGenderSel = 0;
	if (nGenderOption != 0)
	{
		nGenderSel = nGenderCode + 1;

		if ((nGenderSel < 0) || (nGenderSel > 3))
		{
			nGenderSel = 0;
		}
	}

	CStandardCombos::FillGenderCombo(m_comboGender, nGenderSel, TRUE);

	m_checkValid.SetCheck(bIncludeValid);
	m_checkExpired.SetCheck(bIncludeExpired);
	m_checkHotlisted.SetCheck(bIncludeHotlisted);
	m_checkInactive.SetCheck(bIncludeInactive);
	
	m_staticComment.SetWindowText ( strCommentMsg );

	CGroupComboHelpers::FillGroupCombo(m_comboGroup, m_nSingleGroupNo, TRUE);
	
	if (m_comboGroup.GetCount() < 3)		// no individual groups added
	{
		m_comboGroup.EnableWindow(FALSE);
	}

	m_checkBirthday.SetCheck(bBirthday);

	{
		CArray<int, int> arrayAlertFilter;
		CAlertText::ConvertFilterStringToArray(strAlertFilter, arrayAlertFilter);

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

				switch ( arrayAlertFilter[n - 1])
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
	}

	AddComments();
	SetReportingRange();

	return TRUE;
}

//$$******************************************************************

void CBatchPropertyPage1::AddComments()
{
	for (int i = 0; i < m_Comments.GetSize(); i++)		// read previous comments
	{
		m_comboComment.AddString(m_Comments.GetAt(i));
	}
}

//$$******************************************************************

void CBatchPropertyPage1::OnStaticComment()
{
	CString strComment = "";
	m_comboComment.GetWindowText(strComment);

	if (strComment != "")
	{
		m_Comments.Delete(strComment);			// delete comment
		m_comboComment.ResetContent();				// new list
		AddComments();
		m_comboComment.SetWindowText("");
	}
}

//*******************************************************************

BOOL CBatchPropertyPage1::OnKillActive()
{
	m_nSingleGroupNo = CGroupComboHelpers::GetGroupNoFromCombo(m_comboGroup,0);

	CCSV csv;
	csv.Add(m_nCardRangeType);
	csv.Add(GetEditBoxText(m_editCardNoFrom));
	csv.Add(GetEditBoxText(m_editCardNoTo));
	csv.Add(m_nSingleGroupNo);
	csv.Add(IsTicked(m_checkValid));
	csv.Add(IsTicked(m_checkExpired));
	csv.Add(IsTicked(m_checkHotlisted));
	csv.Add(IsTicked(m_checkInactive));
	csv.Add(0); //WAS VALIDATE ON ALERT
	csv.Add(0); //WAS ALERT CODE;

	int nGenderSel = m_comboGender.GetCurSel();
	if ((nGenderSel >= 1) && (nGenderSel <= 3))
	{
		csv.Add(1);
		csv.Add(nGenderSel - 1);
	}
	else
	{
		csv.Add(0);
		csv.Add(0);
	}

	CString strAlertFilter = "";

	for (int n = 0; n < 9; n++)
	{
		switch (m_comboAlert[n].GetCurSel())
		{
		case 1:		strAlertFilter += "1";	break;
		case 2:		strAlertFilter += "2";	break;
		default:	strAlertFilter += "0";	break;
		}
	}

	csv.Add(strAlertFilter);
	csv.Add(IsTicked(m_checkBirthday));

	CReportDefaultHandler repdefs;
	repdefs.Save(m_strLabel, &csv);

	CString strComment = "";
	m_comboComment.GetWindowText(strComment);
	m_Comments.Save(strComment);				// save comment if required

	m_pUpdateHelpers->SetIncludeValidFlag(IsTicked(m_checkValid));
	m_pUpdateHelpers->SetIncludeExpiredFlag(IsTicked(m_checkExpired));
	m_pUpdateHelpers->SetIncludeHotlistedFlag(IsTicked(m_checkHotlisted));
	m_pUpdateHelpers->SetIncludeInactiveFlag(IsTicked(m_checkInactive));
	m_pUpdateHelpers->SetCardRangeType(m_nCardRangeType);
	m_pUpdateHelpers->SetRangeFromCardNo(_atoi64(GetEditBoxText(m_editCardNoFrom)));
	m_pUpdateHelpers->SetRangeToCardNo(_atoi64(GetEditBoxText(m_editCardNoTo)));
	m_pUpdateHelpers->SetGroupSel(m_comboGroup.GetCurSel());
	m_pUpdateHelpers->SetSingleGroupNo(m_nSingleGroupNo);
	m_pUpdateHelpers->SetGenderSel(m_comboGender.GetCurSel());
	m_pUpdateHelpers->SetLabel(m_strLabel);
	m_pUpdateHelpers->SetComment(strComment);
	m_pUpdateHelpers->SetAlertFilter(strAlertFilter);
	m_pUpdateHelpers->SetBirthdayFilter(IsTicked(m_checkBirthday));

	return CPropertyPage::OnKillActive();
}

//$$******************************************************************

void CBatchPropertyPage1::OnRadioRange()
{
	m_radioRange.SetCheck(TRUE);
	m_radioList.SetCheck(FALSE);

	m_nCardRangeType = 0;
	m_editCardNoFrom.SetWindowText(System.GetLowCardNo());
	m_editCardNoTo.SetWindowText(System.GetHighCardNo());
	SetReportingRange();
}

//$$******************************************************************

void CBatchPropertyPage1::OnRadioList()
{
	m_radioRange.SetCheck(FALSE);
	m_radioList.SetCheck(TRUE);

	m_nCardRangeType = 1;
	SetReportingRange();
}

//$$******************************************************************

void CBatchPropertyPage1::SetReportingRange()
{
	bool bAllowFrom = TRUE;
	bool bAllowTo = TRUE;
	bool bAllowList = TRUE;
	bool bAllowGroup = (m_comboGroup.GetCount() > 2) ? TRUE : FALSE;

	switch (m_nCardRangeType)
	{
	case nBATCH_RANGE:		
		bAllowList = FALSE;
		break;

	case nBATCH_LIST:		
		bAllowTo = FALSE;
		bAllowFrom = FALSE;
		bAllowGroup = FALSE;
		break;
	}

	m_editCardNoFrom.EnableWindow(bAllowFrom);
	m_editCardNoTo.EnableWindow(bAllowTo);
	m_buttonList.EnableWindow(bAllowList);
	m_comboGroup.EnableWindow(bAllowGroup);

	switch (m_nCardRangeType)
	{
	case nBATCH_RANGE:		m_editCardNoFrom.SetFocus();	break;
	case nBATCH_LIST:		m_buttonList.SetFocus();		break;
	}
}

//$$******************************************************************

void CBatchPropertyPage1::OnButtonList()
{
	CArray<CSQLRowAccountList, CSQLRowAccountList> arrayDummy;
	CString strReportList = Filenames.GetReportCardListFilename(m_strLabel);
	CCardPickerDlg dlg( m_strTitle, strReportList, arrayDummy, this);

	dlg.m_bValidAllowed = (IsTicked(m_checkValid));
	dlg.m_bExpiredAllowed = (IsTicked(m_checkExpired));
	dlg.m_bHotlistedAllowed = (IsTicked(m_checkHotlisted));
	dlg.m_bInactiveAllowed = (IsTicked(m_checkInactive));

	if (dlg.DoModal() == IDOK)
	{
		m_pUpdateHelpers -> ResetPicker();
	}
}

//$$******************************************************************

