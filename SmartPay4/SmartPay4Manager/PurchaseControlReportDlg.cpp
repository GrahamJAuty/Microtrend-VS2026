//$$******************************************************************
#include "..\SmartPay4Shared\Defines.h"
#include "..\SmartPay4Shared\RepmanHandler.h"
#include "..\SmartPay4Shared\SQLTable_AuditPeriod\SQLRowSetAuditPeriod.h"
#include "..\SmartPay4Shared\SQLTable_PurchaseControlRule\SQLRecordSetPurchaseControlRule.h"
#include "..\SmartPay4Shared\SQLTable_PurchaseControlRule\SQLRowPurchaseControlRule.h"
//$$******************************************************************
#include "AutoShutdownStacker.h"
#include "CardPickerDlg.h"
#include "FindDlg.h"
#include "GroupComboHelpers.h"
#include "ReportDefaultHandler.h"
#include "PurchaseControlReport.h"
//$$******************************************************************
#include "PurchaseControlReportDlg.h"
//$$******************************************************************

CPurchaseControlReportDlg::CPurchaseControlReportDlg( CPurchaseControlReportConfig& config, CWnd* pParent)
	: CSSAutoShutdownDialog(AutoShutdownHelper,CPurchaseControlReportDlg::IDD, pParent)
	, m_DateSelectorRange(m_comboDateRange, m_DatePickerFrom, m_DatePickerTo)
	, m_ReportConfig(config)
	, m_ReportLocationControls(m_comboFolderSet, m_checkTerminals, m_comboTerminals, config.GetTerminalListHandler())
{
	SQLRowSetAuditPeriod.LoadPeriods();
}

//$$******************************************************************

CPurchaseControlReportDlg::~CPurchaseControlReportDlg()
{
}

//$$******************************************************************

void CPurchaseControlReportDlg::DoDataExchange(CDataExchange* pDX)
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
	DDX_Control(pDX, IDC_CHECK_NEWPAGE, m_checkNewPage);
	DDX_Control(pDX, IDC_CHECK_SORTBYCARDNO, m_checkSortByCardNo);
	//terminals
	DDX_Control(pDX, IDC_STATIC_FOLDERSET, m_staticFolderSet);
	DDX_Control(pDX, IDC_COMBO_FOLDERSET, m_comboFolderSet);
	DDX_Control(pDX, IDC_CHECK_TERMINALS, m_checkTerminals);
	DDX_Control(pDX, IDC_COMBO_TERMINALS, m_comboTerminals);
	//options
	DDX_Control(pDX, IDC_CHECK_EPOSAWARD, m_checkShowEposAward);
	DDX_Control(pDX, IDC_CHECK_AUTODELETE, m_checkShowAutoDelete);
	DDX_Control(pDX, IDC_CHECK_MANUALADD, m_checkShowManualAdd);
	DDX_Control(pDX, IDC_CHECK_MANUALEDIT, m_checkShowManualEdit);
	DDX_Control(pDX, IDC_CHECK_MANUALDELETE, m_checkShowManualDelete);
	DDX_Control(pDX, IDC_CHECK_TRANSFER, m_checkShowTransfer);
	DDX_Control(pDX, IDC_CHECK_HOTLIST, m_checkShowHotlist);
	DDX_Control(pDX, IDC_COMBO_RULE, m_comboRule);
	DDX_Control(pDX, IDC_CHECK_SHOWREMOVED, m_checkShowDeletedLines);
	//altkey
	DDX_Control(pDX, IDC_BUTTON_ALTKEY1, m_buttonAltKey1);
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CPurchaseControlReportDlg, CSSAutoShutdownDialog)
	
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
	//altkey
	ON_BN_CLICKED(IDC_BUTTON_ALTKEY1, OnButtonAltKey1)
	
END_MESSAGE_MAP()

//$$******************************************************************

BOOL CPurchaseControlReportDlg::CSSAutoShutdownPostInitDialog()
{
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

	{
		CArray<int, int> arrayStops;
		arrayStops.Add(5);
		arrayStops.Add(50);
		m_comboRule.SetTabStops(arrayStops);

		m_comboRule.AddItem(",All Controls");
		m_comboRule.SetItemData(0, 0);

		{
			CSQLRecordSetPurchaseControlRule RecordSet(NULL, RSParams_PurchaseControlRule_NormalNoParams());

			int nPos = 1;
			CSQLRowPurchaseControlRule RowSO;
			while (RecordSet.StepSelectAll(RowSO) == TRUE)
			{
				int nRuleID = RowSO.GetRuleID();

				CString str = "";
				str.Format("%d,%s",
					nRuleID,
					(const char*)RowSO.GetDisplayDescription());

				m_comboRule.AddItem(str);
				m_comboRule.SetItemData(nPos++, nRuleID);
			}
		}
	}

	CString strFromCardNo = m_ReportConfig.GetStringCardNoFrom();
	CString strToCardNo = m_ReportConfig.GetStringCardNoTo();
	bool bSortByCardNo = m_ReportConfig.GetSortByCardNoFlag();
	bool bShowEposAward = m_ReportConfig.GetShowEPOSAwardFlag();
	bool bShowAutoDelete = m_ReportConfig.GetShowAutoDeleteFlag();
	bool bShowManualAdd = m_ReportConfig.GetShowManualAddFlag();
	bool bShowManualEdit = m_ReportConfig.GetShowManualEditFlag();
	bool bShowManualDelete = m_ReportConfig.GetShowManualDeleteFlag();
	bool bShowHotlist = m_ReportConfig.GetShowHotlistFlag();
	bool bShowTransfer = m_ReportConfig.GetShowTransferFlag();
	bool bShowDeletedLines = m_ReportConfig.GetShowDeletedLinesFlag();
	int nReportRangeType = m_ReportConfig.GetReportRangeType();
	bool bNewPage = m_ReportConfig.GetNewPageFlag();
	int nFolderSet = m_ReportConfig.GetFolderSet();
	int nTerminalListType = m_ReportConfig.GetTerminalListType();
	int nSingleGroupNo = m_ReportConfig.GetSingleGroupNo();
	int nSingleRuleNo = m_ReportConfig.GetSingleRuleNo();
	int nTimeCurSel = m_ReportConfig.GetTimeSelection();
	CString strTimeFrom = m_ReportConfig.GetTimeFromString();
	CString strTimeTo = m_ReportConfig.GetTimeToString();
	CString strDateRangeType = m_ReportConfig.GetDateRangeType();
	CString strDateFrom = m_ReportConfig.GetDateFromString();
	CString strDateTo = m_ReportConfig.GetDateToString();

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
			if (csv.IsEmpty(3) == FALSE)	bSortByCardNo = csv.GetBool(3);
		}

		if (csv.IsEmpty(4) == FALSE) bShowEposAward = csv.GetBool(4);
		if (csv.IsEmpty(5) == FALSE) bShowManualAdd = csv.GetBool(5);
		if (csv.IsEmpty(6) == FALSE) bShowManualEdit = csv.GetBool(6);
		if (csv.IsEmpty(7) == FALSE) bShowManualDelete = csv.GetBool(7);
		if (csv.IsEmpty(8) == FALSE) bShowHotlist = csv.GetBool(8);
		if (csv.IsEmpty(9) == FALSE) bShowTransfer = csv.GetBool(9);
		if (csv.IsEmpty(10) == FALSE) bNewPage = csv.GetBool(10);
		if (csv.IsEmpty(11) == FALSE) bShowDeletedLines = csv.GetBool(11);
		if (csv.IsEmpty(12) == FALSE) nSingleGroupNo = csv.GetInt(12);
		if (csv.IsEmpty(13) == FALSE) nSingleRuleNo = csv.GetInt(13);
		if (csv.IsEmpty(14) == FALSE) nTerminalListType = csv.GetInt(14);
		if (csv.IsEmpty(15) == FALSE) strDateRangeType = csv.GetString(15);
		if (csv.IsEmpty(16) == FALSE) strDateFrom = csv.GetString(16);
		if (csv.IsEmpty(17) == FALSE) strDateTo = csv.GetString(17);
		if (csv.IsEmpty(18) == FALSE) nTimeCurSel = csv.GetInt(18);
		if (csv.IsEmpty(19) == FALSE) strTimeFrom = csv.GetString(19);
		if (csv.IsEmpty(20) == FALSE) strTimeTo = csv.GetString(20);
		if (csv.IsEmpty(21) == FALSE) bShowAutoDelete = csv.GetString(21);
		if (csv.IsEmpty(22) == FALSE) nFolderSet = csv.GetInt(22);
	}

	if ((nReportRangeType < 0) || (nReportRangeType > 2))
	{
		nReportRangeType = 0;
	}
	m_radioRange.SetCheck(0 == nReportRangeType);
	m_radioSingle.SetCheck(1 == nReportRangeType);
	m_radioList.SetCheck(2 == nReportRangeType);

	m_checkSortByCardNo.SetCheck(bSortByCardNo);
	m_checkShowEposAward.SetCheck(bShowEposAward);
	m_checkShowAutoDelete.SetCheck(bShowAutoDelete);
	m_checkShowManualAdd.SetCheck(bShowManualAdd);
	m_checkShowManualEdit.SetCheck(bShowManualEdit);
	m_checkShowManualDelete.SetCheck(bShowManualDelete);
	m_checkShowHotlist.SetCheck(bShowHotlist);
	m_checkShowTransfer.SetCheck(bShowTransfer);
	m_checkShowDeletedLines.SetCheck(bShowDeletedLines);
	m_checkNewPage.SetCheck(bNewPage);

	m_buttonAltKey1.ShowWindow(SW_HIDE);
	
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

	CString strReportLabel = m_ReportConfig.GetReportLabel();

	if (strReportLabel == "R143")
	{
		nSingleGroupNo = 0;
	}

	//groups
	CGroupComboHelpers::FillGroupCombo(m_comboGroup, nSingleGroupNo, TRUE);

	if (m_comboGroup.GetCount() < 3)		// no individual groups added
	{
		m_comboGroup.EnableWindow(FALSE);
	}

	SQLRowSetAuditPeriod.LoadPeriods();

	m_comboTimeRange.AddString(szTIMERANGE_ALL);
	for (int nPeriodIndex = 0; nPeriodIndex < nMAXPERIODS; nPeriodIndex++)
	{
		if (SQLRowSetAuditPeriod.IsPeriodSet(nPeriodIndex) == TRUE)
		{
			m_comboTimeRange.AddString(SQLRowSetAuditPeriod.GetPeriodName(nPeriodIndex));
		}
	}

	m_comboRule.SelectUsingItemData(nSingleRuleNo);

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
	m_ReportLocationControls.Init(nFolderSet, nTerminalListType);

	// other
	SetRangeOptions();
	OnSelectDateRange();

	if (m_comboTimeRange.GetCurSel() != 0)
	{
		OnSelectTimeRange();
	}

	if (strReportLabel == "R143")							// single audit report
	{
		m_radioRange.EnableWindow(FALSE);
		m_radioSingle.EnableWindow(FALSE);
		m_editCardNoTo.EnableWindow(FALSE);
		m_radioList.EnableWindow(FALSE);
	}

	return TRUE;
}

//$$******************************************************************

void CPurchaseControlReportDlg::OnSelectDateRange()
{
	m_DateSelectorRange.UpdateDateControls();
}

//$$******************************************************************

void CPurchaseControlReportDlg::OnChangeDateFrom(NMHDR* pNMHDR, LRESULT* pResult)
{
	m_DateSelectorRange.OnChangeFromDate();
	m_DateSelectorRange.CheckForChangedDate();
	*pResult = 0;
}

//$$******************************************************************

void CPurchaseControlReportDlg::OnChangeDateTo(NMHDR* pNMHDR, LRESULT* pResult)
{
	m_DateSelectorRange.CheckForChangedDate();
	*pResult = 0;
}

//$$******************************************************************

void CPurchaseControlReportDlg::OnSelectTimeRange()
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

void CPurchaseControlReportDlg::OnSelectFolderSet()
{
	m_ReportLocationControls.SelectFolderSet();
}

//*******************************************************************

void CPurchaseControlReportDlg::OnToggleTerminals()
{
	m_ReportLocationControls.ToggleTerminals();
}

//$$******************************************************************

void CPurchaseControlReportDlg::OnButtonAltKey1()
{
	m_ReportLocationControls.DeleteSelectedTerminalList();
}

//$$******************************************************************
void CPurchaseControlReportDlg::OnRadioRange() { SetCardRangeType(0); }
void CPurchaseControlReportDlg::OnRadioSingle() { SetCardRangeType(1); }
void CPurchaseControlReportDlg::OnRadioList() { SetCardRangeType(2); }
//$$******************************************************************

void CPurchaseControlReportDlg::SetCardRangeType(int n)
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
void CPurchaseControlReportDlg::OnToggleSortByCardNo() { SetRangeOptions(); }
//$$******************************************************************

void CPurchaseControlReportDlg::SetRangeOptions()
{
	bool bAllowFrom = TRUE;
	bool bAllowTo = TRUE;
	bool bAllowList = TRUE;
	bool bAllowSort = TRUE;
	bool bAllowGroup = (m_comboGroup.GetCount() > 2) ? TRUE : FALSE;
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

	m_comboGroup.EnableWindow(bAllowGroup);
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
void CPurchaseControlReportDlg::OnDoubleClickRadioList() { OnButtonList(); }
//$$******************************************************************

void CPurchaseControlReportDlg::OnButtonList()
{
	CAutoShutdownDlgStacker AutoShutdownStacker(this);

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

void CPurchaseControlReportDlg::OnDoubleClickRadioSingle()
{
	CFindDlg dlg("",this);
	if (dlg.DoModal() == IDOK)
	{
		m_editCardNoTo.SetWindowText(FormatInt64Value(dlg.m_nRequestedUserID));
	}
}

//$$******************************************************************

void CPurchaseControlReportDlg::OnOK()
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

	int nSingleGroupNo = CGroupComboHelpers::GetGroupNoFromCombo(m_comboGroup,0);
	
	int nSingleRuleNo = 0;
	{
		int nSel = m_comboRule.GetCurSel();
		if ((nSel >= 0) && (nSel < m_comboRule.GetCount()))
		{
			nSingleRuleNo = m_comboRule.GetItemData(nSel);
		}
	}

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
	m_ReportConfig.SetShowEPOSAwardFlag(IsTicked(m_checkShowEposAward));
	m_ReportConfig.SetShowManualAddFlag(IsTicked(m_checkShowManualAdd));
	m_ReportConfig.SetShowManualEditFlag(IsTicked(m_checkShowManualEdit));
	m_ReportConfig.SetShowManualDeleteFlag(IsTicked(m_checkShowManualDelete));
	m_ReportConfig.SetShowHotlistFlag(IsTicked(m_checkShowHotlist));
	m_ReportConfig.SetShowTransferFlag(IsTicked(m_checkShowTransfer));
	m_ReportConfig.SetShowDeletedLinesFlag(IsTicked(m_checkShowDeletedLines));
	m_ReportConfig.SetReportRangeType(nReportRangeType);
	m_ReportConfig.SetNewPageFlag(IsTicked(m_checkNewPage));
	m_ReportConfig.SetTerminalListType(bWantTerminals ? 1 : 0);
	m_ReportConfig.SetSingleGroupFlag(nSingleGroupNo > 0);
	m_ReportConfig.SetSingleGroupNo(nSingleGroupNo);
	m_ReportConfig.SetSingleRuleNo(nSingleRuleNo);
	m_ReportConfig.SetTimeSelection(m_comboTimeRange.GetCurSel());
	m_ReportConfig.SetShowAutoDeleteFlag(IsTicked(m_checkShowAutoDelete));
	m_ReportConfig.SetFolderSet(nFolderSet);

	CString strDateRangeType;
	m_comboDateRange.GetWindowText(strDateRangeType);
	m_ReportConfig.SetDateRangeType(strDateRangeType);

	{
		CCSV csv;
		csv.Add(m_ReportConfig.GetReportRangeType());
		csv.Add(m_ReportConfig.GetStringCardNoFrom());
		csv.Add(m_ReportConfig.GetStringCardNoTo());
		csv.Add(m_ReportConfig.GetSortByCardNoFlag());
		csv.Add(m_ReportConfig.GetShowEPOSAwardFlag());
		csv.Add(m_ReportConfig.GetShowManualAddFlag());
		csv.Add(m_ReportConfig.GetShowManualEditFlag());
		csv.Add(m_ReportConfig.GetShowManualDeleteFlag());
		csv.Add(m_ReportConfig.GetShowHotlistFlag());
		csv.Add(m_ReportConfig.GetShowTransferFlag());
		csv.Add(m_ReportConfig.GetNewPageFlag());
		csv.Add(m_ReportConfig.GetShowDeletedLinesFlag());
		csv.Add(m_ReportConfig.GetSingleGroupNo());
		csv.Add(m_ReportConfig.GetSingleRuleNo());
		csv.Add(m_ReportConfig.GetTerminalListType());
		csv.Add(strDateRangeType);
		csv.Add(m_ReportConfig.GetDateFromString());
		csv.Add(m_ReportConfig.GetDateToString());
		csv.Add(m_ReportConfig.GetTimeSelection());
		csv.Add(m_ReportConfig.GetTimeFromString());
		csv.Add(m_ReportConfig.GetTimeToString());
		csv.Add(m_ReportConfig.GetShowAutoDeleteFlag());
		csv.Add(m_ReportConfig.GetFolderSet());
		
		CReportDefaultHandler repdefs;
		repdefs.Save(m_ReportConfig.GetReportLabel(), &csv);

		CCSV csvReportKey;
		csvReportKey.Add(m_ReportConfig.GetReportRangeType());
		csvReportKey.Add(m_ReportConfig.GetShowEPOSAwardFlag());
		csvReportKey.Add(m_ReportConfig.GetShowManualAddFlag());
		csvReportKey.Add(m_ReportConfig.GetShowManualEditFlag());
		csvReportKey.Add(m_ReportConfig.GetShowManualDeleteFlag());
		csvReportKey.Add(m_ReportConfig.GetTerminalListType());
		m_ReportConfig.SetReportKey(csvReportKey.GetLine());
	}

	if (m_ReportConfig.GetTerminalListType() == 1)
	{
		m_ReportConfig.TNoListSetMaxLength(strTerminalList.GetLength());	// stop entry being truncated
		m_ReportConfig.TNoListSave(strTerminalList);						// save terminal list

		m_ReportConfig.TNoArrayRemoveAll();
		m_ReportConfig.TNoListExtractTNos(strTerminalList);			// extract terminal numbers
	}

	CRepmanHandler RepmanHandler("", this);
	RepmanHandler.SetTitle(m_ReportConfig.GetReportTitle());

	CPurchaseControlReport PurchaseControlReport( m_ReportConfig);

	int nErrNo = PurchaseControlReport.CreateReport();

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

