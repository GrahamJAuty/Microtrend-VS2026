//$$******************************************************************
#include "..\SmartPay4Shared\RepmanHandler.h"
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\SQLTable_AuditPeriod\SQLRowSetAuditPeriod.h"
//$$******************************************************************
#include "AuditReport.h"
#include "AuditReportActionDlg.h"
#include "CardPickerDlg.h"
#include "FindDlg.h"
#include "GroupComboHelpers.h"
#include "ReportDefaultHandler.h"
#include "SmartPay4Manager.h"
//$$******************************************************************
#include "AuditReportDlg.h"
//$$******************************************************************

CAuditReportDlg::CAuditReportDlg(CAuditReportConfig& config, CWnd* pParent)
	: CSSAutoShutdownDialog(AutoShutdownHelper, CAuditReportDlg::IDD, pParent)
	, m_DateSelectorRange(m_comboDateRange, m_DatePickerFrom, m_DatePickerTo)
	, m_ReportConfig(config)
	, m_ReportLocationControls(m_comboFolderSet, m_checkTerminals, m_comboTerminals, config.GetTerminalListHandler())
	, m_ReportConfigAction("","")
{
	SQLRowSetAuditPeriod.LoadPeriods();
}

//$$******************************************************************

void CAuditReportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	//date
	DDX_Control(pDX, IDC_COMBO_DATERANGE, m_comboDateRange);
	DDX_Control(pDX, IDC_DATEPICKER_FROM, m_DatePickerFrom);
	DDX_Control(pDX, IDC_DATEPICKER_TO, m_DatePickerTo);
	//time
	DDX_Control(pDX, IDC_COMBO_TIMERANGE, m_comboTimeRange);
	DDX_Control(pDX, IDC_TIMEPICKER_FROM, m_TimePickerFrom);
	DDX_Control(pDX, IDC_TIMEPICKER_TO, m_TimePickerTo);
	DDX_Control(pDX, IDC_CHECK_CONTINUOUS, m_checkContinuous);
	//account selection
	DDX_Control(pDX, IDC_RADIO_RANGE, m_radioRange);
	DDX_Control(pDX, IDC_RADIO_LIST, m_radioList);
	DDX_Control(pDX, IDC_RADIO_SINGLE, m_radioSingle);
	DDX_Control(pDX, IDC_BUTTON_LIST, m_buttonList);
	DDX_Control(pDX, IDC_EDIT_FROMCARDNO, m_editCardNoFrom);
	DDX_Control(pDX, IDC_EDIT_TOCARDNO, m_editCardNoTo);
	DDX_Control(pDX, IDC_COMBO_GROUP, m_tabcomboGroup);
	DDX_Control(pDX, IDC_CHECK_NEWPAGE, m_checkNewPage);
	DDX_Control(pDX, IDC_CHECK_SORTBYGROUP, m_checkSortByGroup);
	DDX_Control(pDX, IDC_CHECK_SORTBYCARDNO, m_checkSortByCardNo);
	//terminals
	DDX_Control(pDX, IDC_STATIC_FOLDERSET, m_staticFolderSet);
	DDX_Control(pDX, IDC_COMBO_FOLDERSET, m_comboFolderSet);
	DDX_Control(pDX, IDC_CHECK_TERMINALS, m_checkTerminals);
	DDX_Control(pDX, IDC_COMBO_TERMINALS, m_comboTerminals);
	//options
	DDX_Control(pDX, IDC_CHECK_SHOWSERVERLINES, m_checkShowServerLines);
	DDX_Control(pDX, IDC_CHECK_SHOWPCLINES, m_checkShowPcLines);
	DDX_Control(pDX, IDC_CHECK_SHOWPOSLINES, m_checkShowPosLines);
	DDX_Control(pDX, IDC_CHECK_SHOWREMOVED, m_checkShowDeletedLines);
	DDX_Control(pDX, IDC_CHECK_VENDING, m_checkShowVendingLines);
	DDX_Control(pDX, IDC_CHECK_SHOWEXTLINES, m_checkShowExternalLines);
	DDX_Control(pDX, IDC_CHECK_WEBPAYMENT, m_checkWebPayment);
	DDX_Control(pDX, IDC_CHECK_CARDLINK, m_checkCardLink);
	DDX_Control(pDX, IDC_CHECK_SHOWEODLINES, m_checkShowEODLines);
	DDX_Control(pDX, IDC_CHECK_SHOWECRTEXT, m_checkShowEcrText);
	DDX_Control(pDX, IDC_CHECK_PASSWORD, m_checkPassword);
	DDX_Control(pDX, IDC_CHECK_ACTION, m_checkAction);
	DDX_Control(pDX, IDC_BUTTON_ACTION, m_buttonAction);
	//request types
	DDX_Control(pDX, IDC_CHECK_LOOKUP_SCAN, m_checkLookupScan);
	DDX_Control(pDX, IDC_CHECK_LOOKUP_BIO, m_checkLookupBio);
	DDX_Control(pDX, IDC_CHECK_LOOKUP_MANUAL, m_checkLookupManual);
	DDX_Control(pDX, IDC_CHECK_LOOKUP_DEFAULT, m_checkLookupDefault);
	DDX_Control(pDX, IDC_CHECK_LOOKUP_OTHER, m_checkLookupOther);
	DDX_Control(pDX, IDC_CHECK_LOOKUP_INTERNAL, m_checkLookupInternal);
	DDX_Control(pDX, IDC_CHECK_LOOKUP_PIN, m_checkLookupPIN);
	DDX_Control(pDX, IDC_CHECK_LOOKUP_UNKNOWN, m_checkLookupUnknown);
	//altkey
	DDX_Control(pDX, IDC_BUTTON_ALTKEY1, m_buttonAltKey1);
	DDX_Control(pDX, IDC_BUTTON_ALTKEY2, m_buttonAltKey2);
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CAuditReportDlg, CSSAutoShutdownDialog)
	//date
	ON_CBN_SELCHANGE(IDC_COMBO_DATERANGE, OnSelectDateRange)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATEPICKER_FROM, OnChangeDateFrom)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATEPICKER_TO, OnChangeDateTo)
	//time
	ON_CBN_SELCHANGE(IDC_COMBO_TIMERANGE, OnSelectTimeRange)
	//account range
	ON_BN_CLICKED(IDC_RADIO_RANGE, OnRadioRange)
	ON_BN_CLICKED(IDC_RADIO_SINGLE, OnRadioSingle)
	ON_BN_DOUBLECLICKED(IDC_RADIO_SINGLE, OnDoubleClickRadioSingle)
	ON_BN_CLICKED(IDC_RADIO_LIST, OnRadioList)
	ON_BN_DOUBLECLICKED(IDC_RADIO_LIST, OnDoubleClickRadioList)
	ON_BN_CLICKED(IDC_BUTTON_LIST, OnButtonList)
	ON_BN_CLICKED(IDC_CHECK_SORTBYCARDNO, OnToggleSortByCardNo)
	//terminals
	ON_CBN_SELCHANGE(IDC_COMBO_FOLDERSET, OnSelectFolderSet)
	ON_BN_CLICKED(IDC_CHECK_TERMINALS, OnToggleTerminals)
	//options
	ON_BN_CLICKED(IDC_CHECK_PASSWORD, OnTogglePassword)
	ON_BN_CLICKED(IDC_CHECK_ACTION, OnToggleAction)
	ON_BN_CLICKED(IDC_BUTTON_ACTION, OnButtonAction)
	//altkey
	ON_BN_CLICKED(IDC_BUTTON_ALTKEY1, OnButtonAltKey1)
	ON_BN_CLICKED(IDC_BUTTON_ALTKEY2, OnButtonAltKey2)
END_MESSAGE_MAP()

//$$******************************************************************

BOOL CAuditReportDlg::CSSAutoShutdownPostInitDialog()
{
	SubclassEdit(IDC_EDIT_PASSWORD, SS_NUM, 2, "%d");

	SetWindowText(m_ReportConfig.GetReportTitle());

	m_editCardNoFrom.LimitText(System.GetUserIDLength());
	m_editCardNoFrom.SetWindowText(System.GetLowCardNo());

	m_editCardNoTo.LimitText(System.GetUserIDLength());
	m_editCardNoTo.SetWindowText(System.GetHighCardNo());

	{
		CSSDate dateFrom = m_ReportConfig.GetDateFrom();
		CTime timeFrom(dateFrom.GetYear(), dateFrom.GetMonth(), dateFrom.GetDay(), 0, 0, 0);
		m_DatePickerFrom.SetTime(&timeFrom);
	}

	{
		CSSDate dateTo = m_ReportConfig.GetDateTo();
		CTime timeTo(dateTo.GetYear(), dateTo.GetMonth(), dateTo.GetDay(), 0, 0, 0);
		m_DatePickerTo.SetTime(&timeTo);
	}

	CString strFromCardNo = m_ReportConfig.GetStringCardNoFrom();
	CString strToCardNo = m_ReportConfig.GetStringCardNoTo();
	bool bSortByGroup = m_ReportConfig.GetSortByGroupFlag();
	bool bSortByCardNo = m_ReportConfig.GetSortByCardNoFlag();
	bool bShowPosLines = m_ReportConfig.GetShowPosLinesFlag();
	bool bShowPcLines = m_ReportConfig.GetShowPcLinesFlag();
	bool bShowServerLines = m_ReportConfig.GetShowServerLinesFlag();
	bool bShowDeletedLines = m_ReportConfig.GetShowDeletedLinesFlag();
	bool bShowWebPaymentLines = m_ReportConfig.GetShowWebPaymentLinesFlag();
	bool bShowVendingLines = m_ReportConfig.GetShowVendingLinesFlag();
	bool bShowExternalLines = m_ReportConfig.GetShowExternalLinesFlag();
	bool bShowEODLines = m_ReportConfig.GetShowEODLinesFlag();
	bool bShowEcrText = m_ReportConfig.GetShowEcrTextFlag();
	bool bShowCardLinkLines = m_ReportConfig.GetShowCardLinkLinesFlag();
	int nReportRangeType = m_ReportConfig.GetReportRangeType();
	bool bNewPage = m_ReportConfig.GetNewPageFlag();
	int nTerminalListType = m_ReportConfig.GetTerminalListType();
	int nFolderSet = m_ReportConfig.GetFolderSet();
	int nSingleGroupNo = m_ReportConfig.GetSingleGroupNo();
	int nTimeCurSel = m_ReportConfig.GetTimeSelection();
	CString strTimeFrom = m_ReportConfig.GetTimeFromString();
	CString strTimeTo = m_ReportConfig.GetTimeToString();
	CString strDateRangeType = m_ReportConfig.GetDateRangeType();
	CString strDateFrom = m_ReportConfig.GetDateFromString();
	CString strDateTo = m_ReportConfig.GetDateToString();
	bool bContinuous = m_ReportConfig.GetContinousFlag();
	bool bShowLookupScan = m_ReportConfig.GetShowLookupScanFlag();
	bool bShowLookupBio = m_ReportConfig.GetShowLookupBioFlag();
	bool bShowLookupManual = m_ReportConfig.GetShowLookupManualFlag();
	bool bShowLookupDefault = m_ReportConfig.GetShowLookupDefaultFlag();
	bool bShowLookupOther = m_ReportConfig.GetShowLookupOtherFlag();
	bool bShowLookupInternal = m_ReportConfig.GetShowLookupInternalFlag();
	bool bShowLookupPIN = m_ReportConfig.GetShowLookupPINFlag();
	bool bShowLookupUnknown = m_ReportConfig.GetShowLookupUnknownFlag();
	int nPasswordFilter = m_ReportConfig.GetPasswordFilter();
	bool bShowAllActions = m_ReportConfig.GetShowAllActionsFlag();
	int nActionFlags1 = m_ReportConfig.GetShowActionsFlags1();
	int nActionFlags2 = m_ReportConfig.GetShowActionsFlags2();
	int nActionFlags3 = m_ReportConfig.GetShowActionsFlags3();

	CString strLine = "";
	CReportDefaultHandler repdefs;
	if (repdefs.Read(m_ReportConfig.GetReportLabel(), strLine) == TRUE)
	{
		CCSV csv(strLine);

		// if set - the following parameters have already been set
		if (m_ReportConfig.GetSingleCardFlag() == FALSE)
		{
			if (csv.IsEmpty(0) == FALSE)	nReportRangeType = csv.GetInt(0);
			if (csv.IsEmpty(1) == FALSE)	strFromCardNo = csv.GetString(1);
			if (csv.IsEmpty(2) == FALSE)	strToCardNo = csv.GetString(2);
			if (csv.IsEmpty(6) == FALSE)	bSortByCardNo = csv.GetBool(6);
		}

		if (csv.IsEmpty(3) == FALSE) bShowServerLines = csv.GetBool(3);
		if (csv.IsEmpty(4) == FALSE) bShowPcLines = csv.GetBool(4);
		if (csv.IsEmpty(5) == FALSE) bShowPosLines = csv.GetBool(5);
		if (csv.IsEmpty(7) == FALSE) bNewPage = csv.GetBool(7);
		//8 was date cur sel
		if (csv.IsEmpty(9) == FALSE) nTimeCurSel = csv.GetInt(9);
		//10 was summary
		if (csv.IsEmpty(11) == FALSE) nSingleGroupNo = csv.GetInt(11);
		if (csv.IsEmpty(12) == FALSE) bShowWebPaymentLines = csv.GetBool(12);
		if (csv.IsEmpty(13) == FALSE) bShowCardLinkLines = csv.GetBool(13);
		if (csv.IsEmpty(14) == FALSE) nTerminalListType = csv.GetInt(14);
		if (csv.IsEmpty(15) == FALSE) bShowEODLines = csv.GetBool(15);
		if (csv.IsEmpty(16) == FALSE) bShowDeletedLines = csv.GetBool(16);
		if (csv.IsEmpty(17) == FALSE) bShowVendingLines = csv.GetBool(17);
		if (csv.IsEmpty(18) == FALSE) strDateRangeType = csv.GetString(18);
		if (csv.IsEmpty(19) == FALSE) strDateFrom = csv.GetString(19);
		if (csv.IsEmpty(20) == FALSE) strDateTo = csv.GetString(20);
		if (csv.IsEmpty(21) == FALSE) strTimeFrom = csv.GetString(21);
		if (csv.IsEmpty(22) == FALSE) strTimeTo = csv.GetString(22);
		if (csv.IsEmpty(23) == FALSE) bContinuous = csv.GetBool(23);
		if (csv.IsEmpty(24) == FALSE) bShowExternalLines = csv.GetBool(24);
		if (csv.IsEmpty(25) == FALSE) bShowEcrText = csv.GetBool(25);
		if (csv.IsEmpty(26) == FALSE) bShowLookupScan = csv.GetBool(26);
		if (csv.IsEmpty(27) == FALSE) bShowLookupBio = csv.GetBool(27);
		if (csv.IsEmpty(28) == FALSE) bShowLookupManual = csv.GetBool(28);
		if (csv.IsEmpty(29) == FALSE) bShowLookupDefault = csv.GetBool(29);
		if (csv.IsEmpty(30) == FALSE) bShowLookupOther = csv.GetBool(30);
		if (csv.IsEmpty(31) == FALSE) bShowLookupInternal = csv.GetBool(31);
		if (csv.IsEmpty(32) == FALSE) bShowLookupUnknown = csv.GetBool(32);
		if (csv.IsEmpty(33) == FALSE) nPasswordFilter = csv.GetInt(33);
		if (csv.IsEmpty(34) == FALSE) nFolderSet = csv.GetInt(34);
		if (csv.IsEmpty(35) == FALSE) bSortByGroup = csv.GetBool(35);
		if (csv.IsEmpty(36) == FALSE) bShowAllActions = csv.GetBool(36);
		if (csv.IsEmpty(37) == FALSE) nActionFlags1 = csv.GetInt(37);
		if (csv.IsEmpty(38) == FALSE) nActionFlags2 = csv.GetInt(38);
		if (csv.IsEmpty(39) == FALSE) nActionFlags3 = csv.GetInt(39);
		if (csv.IsEmpty(40) == FALSE) bShowLookupPIN = csv.GetBool(40);
	}

	if ((nReportRangeType < 0) || (nReportRangeType > 2))
	{
		nReportRangeType = 0;
	}
	m_radioRange.SetCheck(0 == nReportRangeType);
	m_radioSingle.SetCheck(1 == nReportRangeType);
	m_radioList.SetCheck(2 == nReportRangeType);

	m_checkSortByGroup.SetCheck(bSortByGroup);
	m_checkSortByCardNo.SetCheck(bSortByCardNo);
	m_checkShowPosLines.SetCheck(bShowPosLines);
	m_checkShowPcLines.SetCheck(bShowPcLines);
	m_checkShowServerLines.SetCheck(bShowServerLines);
	m_checkShowDeletedLines.SetCheck(bShowDeletedLines);
	m_checkNewPage.SetCheck(bNewPage);
	m_checkWebPayment.SetCheck(bShowWebPaymentLines);
	m_checkCardLink.SetCheck(bShowCardLinkLines);
	m_checkShowVendingLines.SetCheck(bShowVendingLines);
	m_checkShowExternalLines.SetCheck(bShowExternalLines);
	m_checkShowEODLines.SetCheck(bShowEODLines);
	m_checkShowEcrText.SetCheck(bShowEcrText);

	if ((nPasswordFilter >= 0) && (nPasswordFilter <= 99))
	{
		m_checkPassword.SetCheck(TRUE);
		ShowAndEnableWindow(GetEditPassword(), TRUE);
		SetEditBoxInt(*GetEditPassword(), nPasswordFilter);
	}
	else
	{
		m_checkPassword.SetCheck(FALSE);
		ShowAndEnableWindow(GetEditPassword(), FALSE);
		GetEditPassword()->SetWindowText("0");
	}

	m_checkAction.SetCheck(FALSE == bShowAllActions);
	ShowAndEnableWindow(&m_buttonAction, (FALSE == bShowAllActions));
	m_ReportConfigAction.SetShowActionsFlags1(nActionFlags1);
	m_ReportConfigAction.SetShowActionsFlags2(nActionFlags2);
	m_ReportConfigAction.SetShowActionsFlags3(nActionFlags3);

	m_checkLookupScan.SetCheck(bShowLookupScan);
	m_checkLookupBio.SetCheck(bShowLookupBio);
	m_checkLookupManual.SetCheck(bShowLookupManual);
	m_checkLookupDefault.SetCheck(bShowLookupDefault);
	m_checkLookupOther.SetCheck(bShowLookupOther);
	m_checkLookupInternal.SetCheck(bShowLookupInternal);
	m_checkLookupPIN.SetCheck(bShowLookupPIN);
	m_checkLookupUnknown.SetCheck(bShowLookupUnknown);

	m_buttonAltKey1.ShowWindow(SW_HIDE);
	m_buttonAltKey2.ShowWindow(SW_HIDE);

	// Date range
	m_DateSelectorRange.InitialiseDateControls();
	m_comboDateRange.AddString(szRANGE_TODAY);
	m_comboDateRange.AddString(szRANGE_YESTERDAY);
	m_comboDateRange.AddString(szRANGE_THIS_WEEK);
	m_comboDateRange.AddString(szRANGE_THIS_MONTH);
	m_comboDateRange.AddString(szRANGE_THIS_YEAR);
	m_comboDateRange.AddString(szRANGE_LAST_WEEK);
	m_comboDateRange.AddString(szRANGE_LAST_MONTH);
	m_comboDateRange.AddString(szRANGE_LAST_YEAR);
	m_comboDateRange.AddString(szRANGE_LAST_3_MONTHS);
	m_comboDateRange.AddString(szRANGE_LAST_6_MONTHS);
	m_comboDateRange.AddString(szRANGE_LAST_9_MONTHS);
	m_comboDateRange.AddString(szRANGE_LAST_12_MONTHS);
	m_comboDateRange.AddString(szRANGE_ALL);
	m_comboDateRange.AddString(szRANGE_CUSTOM_DATE);
	m_comboDateRange.AddString(szRANGE_CUSTOM_RANGE);
	m_DateSelectorRange.SetComboSelection(strDateRangeType);

	if ((strDateRangeType == szRANGE_CUSTOM_DATE) || (strDateRangeType == szRANGE_CUSTOM_RANGE))
	{
		CSSDate dateFrom(strDateFrom);
		CSSDate dateTo(strDateTo);

		if (dateFrom.IsSet())
		{
			m_DatePickerFrom.SetTime(COleDateTime(dateFrom.GetYear(), dateFrom.GetMonth(), dateFrom.GetDay(), 0, 0, 0));
		}

		if (dateTo.IsSet())
		{
			m_DatePickerTo.SetTime(COleDateTime(dateTo.GetYear(), dateTo.GetMonth(), dateTo.GetDay(), 0, 0, 0));
		}
	}

	CGroupComboHelpers::FillGroupCombo(m_tabcomboGroup, nSingleGroupNo, TRUE);

	// add audit periods
	SQLRowSetAuditPeriod.LoadPeriods();

	m_comboTimeRange.AddString(szTIMERANGE_ALL);
	for (int nPeriodIndex = 0; nPeriodIndex < nMAXPERIODS; nPeriodIndex++)
	{
		if (SQLRowSetAuditPeriod.IsPeriodSet(nPeriodIndex) == TRUE)
		{
			m_comboTimeRange.AddString(SQLRowSetAuditPeriod.GetPeriodName(nPeriodIndex));
		}
	}

	if ((nTimeCurSel >= 0) && (nTimeCurSel < m_comboTimeRange.GetCount()))
	{
		m_comboTimeRange.SetCurSel(nTimeCurSel);
	}
	else
	{
		m_comboTimeRange.SetCurSel(0);
	}

	if (m_comboTimeRange.GetCount() < 2)
	{
		m_comboTimeRange.EnableWindow(FALSE);
	}

	m_TimePickerFrom.SetFormat("HH:mm");
	m_TimePickerTo.SetFormat("HH:mm");

	{
		CSSTime timeFrom(strTimeFrom);
		CSSTime timeTo(strTimeTo);

		if (timeFrom.IsSet() && timeTo.IsSet())
		{
			m_TimePickerFrom.SetTime(COleDateTime(2019, 1, 1, timeFrom.GetHour(), timeFrom.GetMinute(), 0));
			m_TimePickerTo.SetTime(COleDateTime(2019, 1, 1, timeTo.GetHour(), timeTo.GetMinute(), 0));
		}
		else
		{
			OnSelectTimeRange();
		}
	}

	m_checkContinuous.SetCheck(bContinuous);

	// card range
	SetCardRangeType(nReportRangeType);
	m_editCardNoFrom.SetWindowText(strFromCardNo);
	m_editCardNoTo.SetWindowText(strToCardNo);

	// terminal list
	m_ReportLocationControls.Init(nFolderSet, nTerminalListType);

	// other
	if (IsTicked(m_checkShowEODLines) == FALSE)
	{
		m_checkShowEODLines.ShowWindow(SW_HIDE);
	}

	if (System.GetWebPaymentType() == nWEBPAYMENT_NONE)
	{
		m_checkWebPayment.EnableWindow(FALSE);
	}
	else
	{
		CString strMsg = "";
		strMsg.Format("%s payments", (const char*)System.GetWebPaymentDisplayName());
		m_checkWebPayment.SetWindowText(strMsg);
	}

	// leavers audit
	if (m_ReportConfig.GetReportLabelNumber() == 127)						
	{
		m_checkShowServerLines.EnableWindow(FALSE);
		m_checkShowPcLines.EnableWindow(FALSE);
		m_checkShowPosLines.EnableWindow(FALSE);
		m_checkShowExternalLines.EnableWindow(FALSE);
		m_checkWebPayment.EnableWindow(FALSE);
		m_checkCardLink.EnableWindow(FALSE);

		m_comboFolderSet.EnableWindow(FALSE);
		m_checkTerminals.EnableWindow(FALSE);
		m_comboTerminals.EnableWindow(FALSE);
	}

	//PURSE CREDITS
	if (m_ReportConfig.GetReportLabelNumber() == 139)
	{
		ShowAndEnableWindow(&m_checkShowServerLines, FALSE);
		ShowAndEnableWindow(&m_checkWebPayment, FALSE);
		ShowAndEnableWindow(&m_checkShowVendingLines, FALSE);
		ShowAndEnableWindow(&m_checkCardLink, FALSE);
		ShowAndEnableWindow(&m_checkShowEODLines, FALSE);
		ShowAndEnableWindow(&m_checkShowEcrText, FALSE);
		MoveControl(&m_checkShowPcLines, 316, 42);
	}

	{
		bool bShowPurchaseTexts = System.GetShowPurchaseTextsFlag();

		switch (m_ReportConfig.GetReportLabelNumber())
		{
		case 101:
		case 111:
		case 115:
		case 117:
		case 140:
		case 141:
			break;

		default:
			bShowPurchaseTexts = FALSE;
			break;
		}

		if (FALSE == bShowPurchaseTexts )
		{
			m_checkShowEcrText.SetCheck(FALSE);
			ShowAndEnableWindow(&m_checkShowEcrText, FALSE);
		}
	}

	SetRangeOptions();
	OnSelectDateRange();

	if (m_comboTimeRange.GetCurSel() != 0)
	{
		OnSelectTimeRange();
	}

	// single audit report
	switch (m_ReportConfig.GetReportLabelNumber())
	{
	case 111:
	case 141:
		m_radioRange.EnableWindow(FALSE);
		m_radioSingle.EnableWindow(FALSE);
		m_editCardNoTo.EnableWindow(FALSE);
		m_radioList.EnableWindow(FALSE);
		m_tabcomboGroup.SetCurSel(0);
		break;
	}

	return TRUE;
}

//$$******************************************************************
CEdit* CAuditReportDlg::GetEditPassword() { return GetEdit(IDC_EDIT_PASSWORD); }
//$$******************************************************************

void CAuditReportDlg::OnSelectDateRange()
{
	m_DateSelectorRange.UpdateDateControls();
}

//$$******************************************************************

void CAuditReportDlg::OnChangeDateFrom(NMHDR* pNMHDR, LRESULT* pResult)
{
	m_DateSelectorRange.OnChangeFromDate();
	m_DateSelectorRange.CheckForChangedDate();
	*pResult = 0;
}

//$$******************************************************************

void CAuditReportDlg::OnChangeDateTo(NMHDR* pNMHDR, LRESULT* pResult)
{
	m_DateSelectorRange.CheckForChangedDate();
	*pResult = 0;
}

//$$******************************************************************

void CAuditReportDlg::OnSelectTimeRange()
{
	int nStartHour = 0;
	int nStartMins = 0;
	int nEndHour = 23;
	int nEndMins = 59;

	CString strTimeRange;
	m_comboTimeRange.GetWindowText(strTimeRange);
	SQLRowSetAuditPeriod.GetTimeRange(strTimeRange, nStartHour, nStartMins, nEndHour, nEndMins);

	COleDateTime startTime(2019, 1, 1, nStartHour, nStartMins, 0);
	m_TimePickerFrom.SetTime(startTime);

	COleDateTime endTime(2019, 1, 1, nEndHour, nEndMins, 0);
	m_TimePickerTo.SetTime(endTime);
}

//$$******************************************************************

void CAuditReportDlg::OnSelectFolderSet()
{
	m_ReportLocationControls.SelectFolderSet();
}

//*******************************************************************

void CAuditReportDlg::OnToggleTerminals()
{
	m_ReportLocationControls.ToggleTerminals();
}

//$$******************************************************************

void CAuditReportDlg::OnButtonAltKey1()
{
	m_ReportLocationControls.DeleteSelectedTerminalList();
}

//$$******************************************************************

void CAuditReportDlg::OnButtonAltKey2()
{
	m_checkShowEODLines.ShowWindow(SW_SHOW);
}

//$$******************************************************************
void CAuditReportDlg::OnRadioRange() { SetCardRangeType(0); }
void CAuditReportDlg::OnRadioSingle() { SetCardRangeType(1); }
void CAuditReportDlg::OnRadioList() { SetCardRangeType(2); }
//$$******************************************************************

void CAuditReportDlg::SetCardRangeType(int n)
{
	if ((n < 0) || (n > 2))
		n = 0;

	m_radioRange.SetCheck(0 == n);
	m_radioSingle.SetCheck(1 == n);
	m_radioList.SetCheck(2 == n);

	if (0 == n)
	{
		m_editCardNoFrom.SetWindowText(System.GetLowCardNo());
		m_editCardNoTo.SetWindowText(System.GetHighCardNo());
	}

	m_ReportConfig.SetReportRangeType(n);
	SetRangeOptions();
}

//$$******************************************************************

void CAuditReportDlg::OnToggleSortByCardNo() 
{ 
	SetRangeOptions(); 
}

//$$******************************************************************

void CAuditReportDlg::OnTogglePassword()
{
	bool bEnable = IsTicked(m_checkPassword);
	ShowAndEnableWindow(GetEditPassword(), bEnable);
}

//$$******************************************************************

void CAuditReportDlg::OnToggleAction()
{
	bool bEnable = IsTicked(m_checkAction);
	ShowAndEnableWindow(&m_buttonAction, bEnable);
}

//$$******************************************************************

void CAuditReportDlg::OnButtonAction()
{
	m_ReportConfigAction.SetShowAllActionsFlag(FALSE);
	CAuditReportActionDlg dlgAction(m_ReportConfigAction,this);
	dlgAction.DoModal();
}

//$$******************************************************************

void CAuditReportDlg::SetRangeOptions()
{
	bool bAllowFrom = TRUE;
	bool bAllowTo = TRUE;
	bool bAllowList = TRUE;
	bool bAllowSort = TRUE;
	bool bAllowGroup = (m_tabcomboGroup.GetCount() > 2) ? TRUE : FALSE;
	bool bAllowNewPage = (m_checkSortByCardNo.GetCheck() != 0);

	switch (m_ReportConfig.GetReportRangeType())
	{
	case nREPORT_RANGE:		bAllowList = FALSE;
		break;

	case nREPORT_SINGLE:	bAllowFrom = FALSE;
		bAllowList = FALSE;
		bAllowSort = FALSE;
		bAllowGroup = FALSE;
		bAllowNewPage = FALSE;
		break;

	case nREPORT_LIST:		bAllowTo = FALSE;
		bAllowFrom = FALSE;
		bAllowGroup = FALSE;
		break;
	}

	m_tabcomboGroup.EnableWindow(bAllowGroup);
	m_editCardNoFrom.EnableWindow(bAllowFrom);
	m_editCardNoTo.EnableWindow(bAllowTo);
	m_buttonList.EnableWindow(bAllowList);
	m_checkSortByCardNo.EnableWindow(bAllowSort);
	m_checkNewPage.EnableWindow(bAllowNewPage);

	switch (m_ReportConfig.GetReportRangeType())
	{
	case nREPORT_RANGE:
		m_editCardNoFrom.SetFocus();
		m_editCardNoFrom.SetSel(0, -1);
		break;

	case nREPORT_SINGLE:
		m_editCardNoTo, SetFocus();
		m_editCardNoTo.SetSel(0, -1);
		break;

	case nREPORT_LIST:
		m_buttonList.SetFocus();
		break;
	}
}

//$$******************************************************************
void CAuditReportDlg::OnDoubleClickRadioList() { OnButtonList(); }
//$$******************************************************************

void CAuditReportDlg::OnButtonList()
{
	CArray<CSQLRowAccountList, CSQLRowAccountList> arrayDummy;
	CCardPickerDlg dlg( m_ReportConfig.GetReportTitle(), m_ReportConfig.GetReportCardListFilename(), arrayDummy, this);
	dlg.m_bValidAllowed = TRUE;
	dlg.m_bExpiredAllowed = TRUE;
	dlg.m_bInactiveAllowed = TRUE;
	
	if (dlg.DoModal() == IDOK)
	{
		m_ReportConfig.ResetCardPicker();
	}
}

//$$******************************************************************

void CAuditReportDlg::OnDoubleClickRadioSingle()
{
	CFindDlg dlg( "", this);
	if (dlg.DoModal() == IDOK)
	{
		m_editCardNoTo.SetWindowText(FormatInt64Value(dlg.m_nRequestedUserID));
	}
}

//$$******************************************************************

void CAuditReportDlg::OnOK()
{
	CString strTerminalList = "";
	int nFolderSet = 0;
	bool bWantTerminals = FALSE;
	m_ReportLocationControls.GetSettings(nFolderSet, bWantTerminals, strTerminalList);

	if ((TRUE == bWantTerminals) && (strTerminalList == ""))
	{
		Prompter.FatalError("No EPOS terminal numbers have been specified!", "Entry Error");
		m_comboTerminals.SetFocus();
		return;
	}

	{
		CTime dateFrom;
		m_DatePickerFrom.GetTime(dateFrom);
		m_ReportConfig.SetDateFrom(dateFrom);
	}

	{
		CTime dateTo;
		m_DatePickerTo.GetTime(dateTo);
		m_ReportConfig.SetDateTo(dateTo);
	}

	CString strStartTime = "";
	{
		CTime time;
		m_TimePickerFrom.GetTime(time);
		strStartTime = time.Format("%H:%M");
		m_ReportConfig.SetTimeFrom(strStartTime);
	}

	CString strEndTime = "";
	{
		CTime time;
		m_TimePickerTo.GetTime(time);
		strEndTime = time.Format("%H:%M");
		m_ReportConfig.SetTimeTo(strEndTime);
	}

	m_ReportConfig.SetTimeRangeFlag(strStartTime != "00:00" || strEndTime != "23:59");

	int nSingleGroupNo = CGroupComboHelpers::GetGroupNoFromCombo(m_tabcomboGroup,0);
	
	int nReportRangeType = nREPORT_RANGE;
	if (IsTicked(m_radioSingle) == TRUE)
	{
		nReportRangeType = nREPORT_SINGLE;
	}
	else if (IsTicked(m_radioList) == TRUE)
	{
		nReportRangeType = nREPORT_LIST;
	}

	int nPasswordFilter = -1;
	if (IsTicked(m_checkPassword))
	{
		nPasswordFilter = GetEditBoxInt(*GetEditPassword());
	}

	if ( ( nPasswordFilter < 0 ) || ( nPasswordFilter > 99 ) )
	{
		nPasswordFilter = -1;
	}

	m_ReportConfig.SetStringCardNoFrom(GetEditBoxText(m_editCardNoFrom));
	m_ReportConfig.SetStringCardNoTo(GetEditBoxText(m_editCardNoTo));
	m_ReportConfig.UpdateInt64CardNo(nREPORT_SINGLE == nReportRangeType);
	m_ReportConfig.SetSortByGroupFlag(IsTicked(m_checkSortByGroup));
	m_ReportConfig.SetSortByCardNoFlag(IsTicked(m_checkSortByCardNo));
	m_ReportConfig.SetShowPosLinesFlag(IsTicked(m_checkShowPosLines));
	m_ReportConfig.SetShowPcLinesFlag(IsTicked(m_checkShowPcLines));
	m_ReportConfig.SetShowServerLinesFlag(IsTicked(m_checkShowServerLines));
	m_ReportConfig.SetShowDeletedLinesFlag(IsTicked(m_checkShowDeletedLines));
	m_ReportConfig.SetShowWebPaymentLinesFlag(IsTicked(m_checkWebPayment));
	m_ReportConfig.SetShowCardLinkLinesFlag(IsTicked(m_checkCardLink));
	m_ReportConfig.SetShowVendingLinesFlag(IsTicked(m_checkShowVendingLines));
	m_ReportConfig.SetShowExternalLinesFlag(IsTicked(m_checkShowExternalLines));
	m_ReportConfig.SetShowEODLinesFlag(IsTicked(m_checkShowEODLines));
	m_ReportConfig.SetShowEcrTextFlag(IsTicked(m_checkShowEcrText));
	m_ReportConfig.SetReportRangeType(nReportRangeType);
	m_ReportConfig.SetNewPageFlag(IsTicked(m_checkNewPage));
	m_ReportConfig.SetTerminalListType(bWantTerminals ? 1 : 0);
	m_ReportConfig.SetFolderSet(nFolderSet);
	m_ReportConfig.SetSingleGroupFlag(nSingleGroupNo > 0);
	m_ReportConfig.SetSingleGroupNo(nSingleGroupNo);
	m_ReportConfig.SetTimeSelection(m_comboTimeRange.GetCurSel());
	m_ReportConfig.SetContinuousFlag(IsTicked(m_checkContinuous));

	m_ReportConfig.SetShowLookupScanFlag(IsTicked(m_checkLookupScan));
	m_ReportConfig.SetShowLookupBioFlag(IsTicked(m_checkLookupBio));
	m_ReportConfig.SetShowLookupManualFlag(IsTicked(m_checkLookupManual));
	m_ReportConfig.SetShowLookupDefaultFlag(IsTicked(m_checkLookupDefault));
	m_ReportConfig.SetShowLookupOtherFlag(IsTicked(m_checkLookupOther));
	m_ReportConfig.SetShowLookupInternalFlag(IsTicked(m_checkLookupInternal));
	m_ReportConfig.SetShowLookupPINFlag(IsTicked(m_checkLookupPIN));
	m_ReportConfig.SetShowLookupUnknownFlag(IsTicked(m_checkLookupUnknown));

	m_ReportConfig.SetPasswordFilter(nPasswordFilter);
	m_ReportConfig.SetShowAllActionsFlag(IsTicked(m_checkAction) == FALSE );
	m_ReportConfig.SetShowActionsFlags1(m_ReportConfigAction.GetShowActionsFlags1());
	m_ReportConfig.SetShowActionsFlags2(m_ReportConfigAction.GetShowActionsFlags2());
	m_ReportConfig.SetShowActionsFlags3(m_ReportConfigAction.GetShowActionsFlags3());

	CString strDateRangeType;
	m_comboDateRange.GetWindowText(strDateRangeType);
	m_ReportConfig.SetDateRangeType(strDateRangeType);

	{
		CCSV csv;
		csv.Add(m_ReportConfig.GetReportRangeType());
		csv.Add(m_ReportConfig.GetStringCardNoFrom());
		csv.Add(m_ReportConfig.GetStringCardNoTo());
		csv.Add(m_ReportConfig.GetShowServerLinesFlag());
		csv.Add(m_ReportConfig.GetShowPcLinesFlag());
		csv.Add(m_ReportConfig.GetShowPosLinesFlag());
		csv.Add(m_ReportConfig.GetSortByCardNoFlag());
		csv.Add(m_ReportConfig.GetNewPageFlag());
		csv.Add("");	//WAS DATE SELECTION
		csv.Add(m_ReportConfig.GetTimeSelection());
		csv.Add(FALSE); //WAS SUMMARY
		csv.Add(m_ReportConfig.GetSingleGroupNo());
		csv.Add(m_ReportConfig.GetShowWebPaymentLinesFlag());
		csv.Add(m_ReportConfig.GetShowCardLinkLinesFlag());
		csv.Add(m_ReportConfig.GetTerminalListType());
		csv.Add(m_ReportConfig.GetShowEODLinesFlag());
		csv.Add(m_ReportConfig.GetShowDeletedLinesFlag());
		csv.Add(m_ReportConfig.GetShowVendingLinesFlag());
		csv.Add(m_ReportConfig.GetDateRangeType());
		csv.Add(m_ReportConfig.GetDateFromString());
		csv.Add(m_ReportConfig.GetDateToString());
		csv.Add(m_ReportConfig.GetTimeFromString());
		csv.Add(m_ReportConfig.GetTimeToString());
		csv.Add(m_ReportConfig.GetContinousFlag());
		csv.Add(m_ReportConfig.GetShowExternalLinesFlag());
		csv.Add(m_ReportConfig.GetShowEcrTextFlag());
		csv.Add(m_ReportConfig.GetShowLookupScanFlag());
		csv.Add(m_ReportConfig.GetShowLookupBioFlag());
		csv.Add(m_ReportConfig.GetShowLookupManualFlag());
		csv.Add(m_ReportConfig.GetShowLookupDefaultFlag());
		csv.Add(m_ReportConfig.GetShowLookupOtherFlag());
		csv.Add(m_ReportConfig.GetShowLookupInternalFlag());
		csv.Add(m_ReportConfig.GetShowLookupUnknownFlag());
		csv.Add(m_ReportConfig.GetPasswordFilter());
		csv.Add(m_ReportConfig.GetFolderSet());
		csv.Add(m_ReportConfig.GetSortByGroupFlag());
		csv.Add(m_ReportConfig.GetShowAllActionsFlag());
		csv.Add(m_ReportConfig.GetShowActionsFlags1());
		csv.Add(m_ReportConfig.GetShowActionsFlags2());
		csv.Add(m_ReportConfig.GetShowActionsFlags3());
		csv.Add(m_ReportConfig.GetShowLookupPINFlag());

		CReportDefaultHandler repdefs;
		repdefs.Save(m_ReportConfig.GetReportLabel(), &csv);

		CCSV csvReportKey;
		csvReportKey.Add(m_ReportConfig.GetReportRangeType());
		csvReportKey.Add(m_ReportConfig.GetShowServerLinesFlag());
		csvReportKey.Add(m_ReportConfig.GetShowPcLinesFlag());
		csvReportKey.Add(m_ReportConfig.GetShowPosLinesFlag());
		csvReportKey.Add(m_ReportConfig.GetShowWebPaymentLinesFlag());
		csvReportKey.Add(m_ReportConfig.GetShowCardLinkLinesFlag());
		csvReportKey.Add(m_ReportConfig.GetTerminalListType());
		m_ReportConfig.SetReportKey(csvReportKey.GetLine());
	}

	CRepmanHandler RepmanHandler("", this);
	RepmanHandler.SetTitle(m_ReportConfig.GetReportTitle());

	if (m_ReportConfig.GetTerminalListType() == 1)
	{
		m_ReportConfig.TNoListSetMaxLength(strTerminalList.GetLength());	// stop entry being truncated
		m_ReportConfig.TNoListSave(strTerminalList);						// save terminal list

		m_ReportConfig.TNoArrayRemoveAll();
		m_ReportConfig.TNoListExtractTNos(strTerminalList);			// extract terminal numbers
	}

	CAuditReport AuditReport( m_ReportConfig);

	int nErrNo = AuditReport.CreateReport();

	if (nREPORT_NOERROR == nErrNo)
	{
		RepmanHandler.DisplayReport(m_ReportConfig.GetReportFilename(), m_ReportConfig.GetParamsFilename(), m_ReportConfig.GetReportKey());
	}
	else
	{
		RepmanHandler.DisplayError(nErrNo);
	}
}

//$$******************************************************************
