//$$******************************************************************
#include "..\SPosLoyalty4Shared\SQLTable_AuditPeriod\SQLRowSetAuditPeriod.h"
#include "..\SPosLoyalty4Shared\SQLTable_EcrmanDatabase\SQLRepositoryEcrmanDatabase.h"
//$$******************************************************************
#include "AuditReport.h"
#include "AuditReportActionDlg.h"
#include "CardPickerDlg.h"
#include "FindDlg.h"
#include "GroupComboHelpers.h"
#include "LoyaltyManager.h"
#include "RepmanHandler.h"
#include "ReportDefaultHandler.h"
#include "StandardCombos.h"
//$$******************************************************************
#include "AuditReportDlg.h"
//$$******************************************************************

CAuditReportDlg::CAuditReportDlg( CAuditReportConfig& config, CWnd* pParent)
	: CSSDialog(CAuditReportDlg::IDD, pParent)
	, m_DateSelectorRange(m_comboDateRange, m_DatePickerFrom, m_DatePickerTo)
	, m_ReportLocationControls(m_comboDatabase, m_comboFolderSet, m_checkTerminals, m_comboTerminals, config.GetTerminalListHandler())
	, m_ReportConfig(config)
	, m_ReportConfigAction("","")
{
	SQLRowSetAuditPeriod.LoadPeriods(NULL);
}

//$$******************************************************************

CAuditReportDlg::~CAuditReportDlg()
{
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
	//account selection
	DDX_Control(pDX, IDC_RADIO_RANGE, m_radioRange);
	DDX_Control(pDX, IDC_RADIO_LIST, m_radioList);
	DDX_Control(pDX, IDC_RADIO_SINGLE, m_radioSingle);
	DDX_Control(pDX, IDC_BUTTON_LIST, m_buttonList);
	DDX_Control(pDX, IDC_EDIT_FROMCARDNO, m_editCardNoFrom);
	DDX_Control(pDX, IDC_EDIT_TOCARDNO, m_editCardNoTo);
	DDX_Control(pDX, IDC_COMBO_GROUP, m_comboGroup);
	DDX_Control(pDX, IDC_CHECK_SORTBYGROUPNO, m_checkSortByGroupNo);
	DDX_Control(pDX, IDC_CHECK_NEWPAGE, m_checkNewPage);
	DDX_Control(pDX, IDC_CHECK_SORTBYCARDNO, m_checkSortByCardNo);
	//terminals
	DDX_Control(pDX, IDC_COMBO_DATABASE, m_comboDatabase);
	DDX_Control(pDX, IDC_COMBO_FOLDERSET, m_comboFolderSet);
	DDX_Control(pDX, IDC_CHECK_TERMINALS, m_checkTerminals);
	DDX_Control(pDX, IDC_COMBO_TERMINALS, m_comboTerminals);
	//options
	DDX_Control(pDX, IDC_CHECK_SHOWSERVERLINES, m_checkShowServerLines);
	DDX_Control(pDX, IDC_CHECK_SHOWPCLINES, m_checkShowPcLines);
	DDX_Control(pDX, IDC_CHECK_SHOWPOSLINES, m_checkShowPosLines);
	DDX_Control(pDX, IDC_CHECK_SHOWEXTLINES, m_checkShowExtLines);
	DDX_Control(pDX, IDC_CHECK_SHOWREMOVED, m_checkShowDeletedLines);
	DDX_Control(pDX, IDC_CHECK_SHOWECRTEXT, m_checkShowEcrText);
	DDX_Control(pDX, IDC_CHECK_SHOWNONPURSE, m_checkShowNonPurse);
	DDX_Control(pDX, IDC_CHECK_ACTION, m_checkAction);
	DDX_Control(pDX, IDC_BUTTON_ACTION, m_buttonAction);
	//altkey
	DDX_Control(pDX, IDC_BUTTON_ALTKEY1, m_buttonAltKey1);
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CAuditReportDlg, CSSDialog)
	
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
	ON_BN_CLICKED(IDC_CHECK_SORTBYGROUP, OnToggleSortByCardNo)
	//terminals
	ON_CBN_SELCHANGE(IDC_COMBO_DATABASE, OnSelectDatabase)
	ON_CBN_SELCHANGE(IDC_COMBO_FOLDERSET, OnSelectFolderSet)
	ON_BN_CLICKED(IDC_CHECK_TERMINALS, OnToggleTerminals)
	//options
	ON_BN_CLICKED(IDC_CHECK_ACTION, OnToggleAction)
	ON_BN_CLICKED(IDC_BUTTON_ACTION, OnButtonAction)
	//altkey
	ON_BN_CLICKED(IDC_BUTTON_ALTKEY1, OnButtonAltKey1)
	
END_MESSAGE_MAP()

//$$******************************************************************

BOOL CAuditReportDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetWindowText(m_ReportConfig.GetReportTitle());

	m_editCardNoFrom.LimitText(System.GetMaxCardNoLength());
	m_editCardNoFrom.SetWindowText(System.GetLowCardNo());

	m_editCardNoTo.LimitText(System.GetMaxCardNoLength());
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
	bool bSortByCardNo = m_ReportConfig.GetSortByCardNoFlag();
	bool bSortByGroupNo = m_ReportConfig.GetSortByGroupNoFlag();
	bool bShowPosLines = m_ReportConfig.GetShowPosLinesFlag();
	bool bShowPcLines = m_ReportConfig.GetShowPcLinesFlag();
	bool bShowExtLines = m_ReportConfig.GetShowExtLinesFlag();
	bool bShowServerLines = m_ReportConfig.GetShowServerLinesFlag();
	bool bShowDeletedLines = m_ReportConfig.GetShowDeletedLinesFlag();
	bool bShowEcrText = m_ReportConfig.GetShowEcrTextFlag();
	int nReportRangeType = m_ReportConfig.GetReportRangeType();
	bool bNewPage = m_ReportConfig.GetNewPageFlag();
	int nTerminalSelectType = m_ReportConfig.GetTerminalSelectType();
	int nSingleGroupNo = m_ReportConfig.GetSingleGroupNo();
	int nTimeCurSel = m_ReportConfig.GetTimeSelection();
	CString strTimeFrom = m_ReportConfig.GetTimeFromString();
	CString strTimeTo = m_ReportConfig.GetTimeToString();
	CString strDateRangeType = m_ReportConfig.GetDateRangeType();
	CString strDateFrom = m_ReportConfig.GetDateFromString();
	CString strDateTo = m_ReportConfig.GetDateToString();
	int nDbNo = m_ReportConfig.GetDbNo();
	int nFolderSet = m_ReportConfig.GetFolderSet();
	bool bShowNonPurse = m_ReportConfig.GetShowNonPurseFlag();
	bool bShowAllActions = m_ReportConfig.GetShowAllActionsFlag();
	int nActionFlags1 = m_ReportConfig.GetShowActionsFlags1();
	int nActionFlags2 = m_ReportConfig.GetShowActionsFlags2();
	int nActionFlags3 = m_ReportConfig.GetShowActionsFlags3();

	CString strLine;
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
		if (csv.IsEmpty(8) == FALSE) strDateRangeType = csv.GetString(8);
		if (csv.IsEmpty(9) == FALSE) nTimeCurSel = csv.GetInt(9);
		//10 was summary
		if (csv.IsEmpty(11) == FALSE) nSingleGroupNo = csv.GetInt(11);
		if (csv.IsEmpty(12) == FALSE) nTerminalSelectType = csv.GetInt(12);
		if (csv.IsEmpty(13) == FALSE) bShowDeletedLines = csv.GetBool(13);
		if (csv.IsEmpty(14) == FALSE) strDateFrom = csv.GetString(14);
		if (csv.IsEmpty(15) == FALSE) strDateTo = csv.GetString(15);
		if (csv.IsEmpty(16) == FALSE) strTimeFrom = csv.GetString(16);
		if (csv.IsEmpty(17) == FALSE) strTimeTo = csv.GetString(17);
		if (csv.IsEmpty(18) == FALSE) bShowExtLines = csv.GetBool(18);
		if (csv.IsEmpty(19) == FALSE) bShowEcrText = csv.GetBool(19);
		if (csv.IsEmpty(20) == FALSE) nDbNo = csv.GetInt(20);
		if (csv.IsEmpty(21) == FALSE) nFolderSet = csv.GetInt(21);
		if (csv.IsEmpty(22) == FALSE) bSortByGroupNo = csv.GetInt(22);
		if (csv.IsEmpty(23) == FALSE) bShowNonPurse = csv.GetBool(23);
		if (csv.IsEmpty(24) == FALSE) bShowAllActions = csv.GetBool(24);
		if (csv.IsEmpty(25) == FALSE) nActionFlags1 = csv.GetInt(25);
		if (csv.IsEmpty(26) == FALSE) nActionFlags2 = csv.GetInt(26);
		if (csv.IsEmpty(27) == FALSE) nActionFlags3 = csv.GetInt(27);
	}

	if ((nReportRangeType < 0) || (nReportRangeType > 2))
	{
		nReportRangeType = 0;
	}
	m_radioRange.SetCheck(0 == nReportRangeType);
	m_radioSingle.SetCheck(1 == nReportRangeType);
	m_radioList.SetCheck(2 == nReportRangeType);

	m_checkSortByCardNo.SetCheck(bSortByCardNo);
	m_checkSortByGroupNo.SetCheck(bSortByGroupNo);
	m_checkShowPosLines.SetCheck(bShowPosLines);
	m_checkShowPcLines.SetCheck(bShowPcLines);
	m_checkShowExtLines.SetCheck(bShowExtLines);
	m_checkShowServerLines.SetCheck(bShowServerLines);
	m_checkShowDeletedLines.SetCheck(bShowDeletedLines);
	m_checkShowEcrText.SetCheck(bShowEcrText);
	m_checkShowNonPurse.SetCheck(bShowNonPurse);
	m_checkNewPage.SetCheck(bNewPage);

	m_buttonAltKey1.ShowWindow(SW_HIDE);

	//options
	m_checkAction.SetCheck(FALSE == bShowAllActions);
	ShowAndEnableWindow(&m_buttonAction, (FALSE == bShowAllActions));
	m_ReportConfigAction.SetShowActionsFlags1(nActionFlags1);
	m_ReportConfigAction.SetShowActionsFlags2(nActionFlags2);
	m_ReportConfigAction.SetShowActionsFlags3(nActionFlags3);
	
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

	int nReportLabelNumber = m_ReportConfig.GetReportLabelNumber();

	switch (nReportLabelNumber)
	{
	case 123:	//AUDIT SINGLE
	case 133:	//SUMMARY SINGLE
		nSingleGroupNo = 0;
		break;
	}

	//groups
	CGroupComboHelpers::FillGroupCombo(m_comboGroup, nSingleGroupNo, TRUE);

	if (m_comboGroup.GetCount() < 3)		// no individual groups added
	{
		m_comboGroup.EnableWindow(FALSE);
	}

	SQLRowSetAuditPeriod.LoadPeriods(NULL);

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

	// card range
	SetCardRangeType(nReportRangeType);
	m_editCardNoFrom.SetWindowText(strFromCardNo);
	m_editCardNoTo.SetWindowText(strToCardNo);

	// terminal list
	m_ReportLocationControls.Init(nDbNo, nFolderSet, nTerminalSelectType, TRUE);

	{
		bool bShowPurchaseTexts = FALSE;

		switch (nReportLabelNumber)
		{
		case 101:	//STANDARD AUDIT
		case 115:	//CARD SALES
		case 123:	//SINGLE ACCOUNT AUDIT
		case 132:	//SUMMARY
		case 133:	//SINGLE ACCOUNT SUMMARY
			bShowPurchaseTexts = System.GetShowAuditPurchasesFlag();
			break;
		}
		
		if (FALSE == bShowPurchaseTexts)
		{
			m_checkShowEcrText.SetCheck(FALSE);
			ShowAndEnableWindow(&m_checkShowEcrText, FALSE);
		}
	}

	// other
	SetRangeOptions();
	OnSelectDateRange();

	if (m_comboTimeRange.GetCurSel() != 0)
	{
		OnSelectTimeRange();
	}

	switch (nReportLabelNumber)
	{
	case 123:
	case 133:
		m_radioRange.EnableWindow(FALSE);
		m_radioSingle.EnableWindow(FALSE);
		m_editCardNoTo.EnableWindow(FALSE);
		m_radioList.EnableWindow(FALSE);
		break;
	}

	switch (nReportLabelNumber)
	{
	case 132:
	case 133:
		break;

	default:
		ShowAndEnableWindow(&m_checkShowNonPurse, FALSE);
		break;
	}

	return TRUE;
}

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

void CAuditReportDlg::OnSelectDatabase()
{
	m_ReportLocationControls.SelectDatabase();
}

//*******************************************************************

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

void CAuditReportDlg::OnRadioRange() 
{ 
	SetCardRangeType(0); 
}

//$$******************************************************************

void CAuditReportDlg::OnRadioSingle()
{ 
	SetCardRangeType(1); 
}

//$$******************************************************************

void CAuditReportDlg::OnRadioList() 
{ 
	SetCardRangeType(2); 
}

//$$******************************************************************

void CAuditReportDlg::SetCardRangeType(int n)
{
	if ((n < 0) || (n > 2))
	{
		n = 0;
	}

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
void CAuditReportDlg::OnToggleSortByCardNo() { SetRangeOptions(); }
//$$******************************************************************

void CAuditReportDlg::SetRangeOptions()
{
	bool bAllowFrom = TRUE;
	bool bAllowTo = TRUE;
	bool bAllowList = TRUE;
	bool bAllowSort = TRUE;
	bool bAllowGroup = (m_comboGroup.GetCount() > 2) ? TRUE : FALSE;
	bool bAllowNewPage = IsTicked(m_checkSortByCardNo) || IsTicked(m_checkSortByGroupNo);

	switch (m_ReportConfig.GetReportRangeType())
	{
	case nREPORT_RANGE:	
		bAllowList = FALSE;
		break;

	case nREPORT_SINGLE:	
		bAllowFrom = FALSE;
		bAllowList = FALSE;
		bAllowSort = FALSE;
		bAllowGroup = FALSE;
		bAllowNewPage = FALSE;
		break;

	case nREPORT_LIST:	
		bAllowTo = FALSE;
		bAllowFrom = FALSE;
		bAllowGroup = FALSE;
		break;
	}

	m_comboGroup.EnableWindow(bAllowGroup);
	m_editCardNoFrom.EnableWindow(bAllowFrom);
	m_editCardNoTo.EnableWindow(bAllowTo);
	m_buttonList.EnableWindow(bAllowList);
	m_checkSortByCardNo.EnableWindow(bAllowSort);
	m_checkSortByGroupNo.EnableWindow(bAllowSort);
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
	dlg.m_bHotlistedAllowed = TRUE;

	if (dlg.DoModal() == IDOK)
	{
		m_ReportConfig.ResetCardPicker();
	}
}

//$$******************************************************************

void CAuditReportDlg::OnDoubleClickRadioSingle()
{
	CFindDlg dlg(this);
	if (dlg.DoModal() == IDOK)
	{
		m_editCardNoTo.SetWindowText(SimpleFormatInt64Value(dlg.m_nRequestedCardNo));
	}
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
	CAuditReportActionDlg dlgAction(m_ReportConfigAction, this);
	dlgAction.DoModal();
}

//$$******************************************************************

void CAuditReportDlg::OnOK()
{
	CString strTerminalList = "";
	int nDbNo = 0;
	int nFolderSet = 0;
	bool bWantTerminals = FALSE;
	m_ReportLocationControls.GetSettings(nDbNo, nFolderSet, bWantTerminals, strTerminalList);

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

	int nSingleGroupNo = CGroupComboHelpers::GetGroupNoFromCombo(m_comboGroup,0);
	
	int nReportRangeType = nREPORT_RANGE;
	if (IsTicked(m_radioSingle) == TRUE)
	{
		nReportRangeType = nREPORT_SINGLE;
	}
	else if (IsTicked(m_radioList) == TRUE)
	{
		nReportRangeType = nREPORT_LIST;
	}

	m_ReportConfig.SetStringCardNoFrom(GetEditBoxText(m_editCardNoFrom));
	m_ReportConfig.SetStringCardNoTo(GetEditBoxText(m_editCardNoTo));
	m_ReportConfig.UpdateInt64CardNo(nREPORT_SINGLE == nReportRangeType);
	m_ReportConfig.SetSortByCardNoFlag(IsTicked(m_checkSortByCardNo));
	m_ReportConfig.SetSortByGroupNoFlag(IsTicked(m_checkSortByGroupNo));
	m_ReportConfig.SetShowPosLinesFlag(IsTicked(m_checkShowPosLines));
	m_ReportConfig.SetShowPcLinesFlag(IsTicked(m_checkShowPcLines));
	m_ReportConfig.SetShowExtLinesFlag(IsTicked(m_checkShowExtLines));
	m_ReportConfig.SetShowServerLinesFlag(IsTicked(m_checkShowServerLines));
	m_ReportConfig.SetShowDeletedLinesFlag(IsTicked(m_checkShowDeletedLines));
	m_ReportConfig.SetShowEcrTextFlag(IsTicked(m_checkShowEcrText));
	m_ReportConfig.SetReportRangeType(nReportRangeType);
	m_ReportConfig.SetNewPageFlag(IsTicked(m_checkNewPage));
	m_ReportConfig.SetTerminalSelectType(bWantTerminals);
	m_ReportConfig.SetSingleGroupFlag(nSingleGroupNo > 0);
	m_ReportConfig.SetSingleGroupNo(nSingleGroupNo);
	m_ReportConfig.SetTimeSelection(m_comboTimeRange.GetCurSel());
	m_ReportConfig.SetDbNo(nDbNo);
	m_ReportConfig.SetFolderSet(nFolderSet);
	m_ReportConfig.SetShowNonPurseFlag(IsTicked(m_checkShowNonPurse));
	m_ReportConfig.SetShowAllActionsFlag(IsTicked(m_checkAction) == FALSE);
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
		csv.Add(strDateRangeType);
		csv.Add(m_ReportConfig.GetTimeSelection());
		csv.Add(FALSE); //WAS SUMMARY
		csv.Add(m_ReportConfig.GetSingleGroupNo());
		csv.Add(m_ReportConfig.GetTerminalSelectType());
		csv.Add(m_ReportConfig.GetShowDeletedLinesFlag());
		csv.Add(m_ReportConfig.GetDateFromString());
		csv.Add(m_ReportConfig.GetDateToString());
		csv.Add(m_ReportConfig.GetTimeFromString());
		csv.Add(m_ReportConfig.GetTimeToString());
		csv.Add(m_ReportConfig.GetShowExtLinesFlag());
		csv.Add(m_ReportConfig.GetShowEcrTextFlag());
		csv.Add(m_ReportConfig.GetDbNo());
		csv.Add(m_ReportConfig.GetFolderSet());
		csv.Add(m_ReportConfig.GetSortByGroupNoFlag());
		csv.Add(m_ReportConfig.GetShowNonPurseFlag());
		csv.Add(m_ReportConfig.GetShowAllActionsFlag());
		csv.Add(m_ReportConfig.GetShowActionsFlags1());
		csv.Add(m_ReportConfig.GetShowActionsFlags2());
		csv.Add(m_ReportConfig.GetShowActionsFlags3());
		
		CReportDefaultHandler repdefs;
		repdefs.Save(m_ReportConfig.GetReportLabel(), &csv);

		CCSV csvReportKey;
		csvReportKey.Add(m_ReportConfig.GetReportRangeType());
		csvReportKey.Add(m_ReportConfig.GetShowServerLinesFlag());
		csvReportKey.Add(m_ReportConfig.GetShowPcLinesFlag());
		csvReportKey.Add(m_ReportConfig.GetShowPosLinesFlag());
		csvReportKey.Add(m_ReportConfig.GetTerminalSelectType());
		csvReportKey.Add(m_ReportConfig.GetShowExtLinesFlag());

		switch (m_ReportConfig.GetReportLabelNumber())
		{
		case 132:
		case 133:
			csvReportKey.Add(m_ReportConfig.GetShowNonPurseFlag());
			break;
		}

		m_ReportConfig.SetReportKey(csvReportKey.GetLine());
	}

	if (m_ReportConfig.GetTerminalSelectType() == 1)
	{
		m_ReportConfig.TNoListSetMaxLength(strTerminalList.GetLength());	// stop entry being truncated
		m_ReportConfig.TNoListSave(strTerminalList);						// save terminal list

		m_ReportConfig.TNoArrayRemoveAll();
		m_ReportConfig.TNoListExtractTNos(strTerminalList);			// extract terminal numbers
	}

	CRepmanHandler RepmanHandler("", this);
	RepmanHandler.SetTitle(m_ReportConfig.GetReportTitle());

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

