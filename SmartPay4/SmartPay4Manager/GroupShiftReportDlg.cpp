//$$******************************************************************
#include "..\SmartPay4Shared\RepmanHandler.h"
#include "..\SmartPay4Shared\SQLTable_AuditPeriod\SQLRowSetAuditPeriod.h"
//$$******************************************************************
#include "CardPickerDlg.h"
#include "FindDlg.h"
#include "GroupComboHelpers.h"
#include "GroupShiftReport.h"
#include "ReportDefaultHandler.h"
//$$******************************************************************
#include "GroupShiftReportDlg.h"
//$$******************************************************************

CGroupShiftReportDlg::CGroupShiftReportDlg(CGroupShiftReportConfig& config, CWnd* pParent)
	: CSSAutoShutdownDialog(AutoShutdownHelper, CGroupShiftReportDlg::IDD, pParent)
	, m_DateSelectorRange(m_comboDateRange, m_DatePickerFrom, m_DatePickerTo)
	, m_ReportConfig(config)
{
	SQLRowSetAuditPeriod.LoadPeriods();
}

//$$******************************************************************

void CGroupShiftReportDlg::DoDataExchange(CDataExchange* pDX)
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
	DDX_Control(pDX, IDC_CHECK_SORTBYCARDNO, m_checkSortByCardNo);
	DDX_Control(pDX, IDC_CHECK_SHOWREMOVED, m_checkShowRemoved);
	//source
	DDX_Control(pDX, IDC_CHECK_SOURCE_MANAGER, m_checkSourceManager);
	DDX_Control(pDX, IDC_CHECK_SOURCE_SERVER, m_checkSourceServer);
	DDX_Control(pDX, IDC_CHECK_SOURCE_BACKGROUND, m_checkSourceBackground);
	DDX_Control(pDX, IDC_CHECK_SOURCE_MANUAL, m_checkSourceManual);
	DDX_Control(pDX, IDC_CHECK_SHOW_PREVIOUS, m_checkShowPrevious);
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CGroupShiftReportDlg, CSSAutoShutdownDialog)
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
END_MESSAGE_MAP()

//$$******************************************************************

BOOL CGroupShiftReportDlg::CSSAutoShutdownPostInitDialog()
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

	CString strFromCardNo = m_ReportConfig.GetStringCardNoFrom();
	CString strToCardNo = m_ReportConfig.GetStringCardNoTo();
	bool bSortByCardNo = m_ReportConfig.GetSortByCardNoFlag();
	int nReportRangeType = m_ReportConfig.GetReportRangeType();
	bool bNewPage = m_ReportConfig.GetNewPageFlag();
	int nSingleGroupNo = m_ReportConfig.GetSingleGroupNo();
	int nTimeCurSel = m_ReportConfig.GetTimeSelection();
	CString strTimeFrom = m_ReportConfig.GetTimeFromString();
	CString strTimeTo = m_ReportConfig.GetTimeToString();
	CString strDateRangeType = m_ReportConfig.GetDateRangeType();
	CString strDateFrom = m_ReportConfig.GetDateFromString();
	CString strDateTo = m_ReportConfig.GetDateToString();
	bool bContinuous = m_ReportConfig.GetContinousFlag();
	bool bShowDeletedLines = m_ReportConfig.GetShowDeletedLinesFlag();
	bool bShowSourceManager = m_ReportConfig.GetShowSourceManagerFlag();
	bool bShowSourceServer = m_ReportConfig.GetShowSourceServerFlag();
	bool bShowSourceBackground = m_ReportConfig.GetShowSourceBackgroundFlag();
	bool bShowSourceManual = m_ReportConfig.GetShowSourceManualFlag();
	bool bShowPrevious = m_ReportConfig.GetShowPreviousFlag();

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

		if (csv.IsEmpty(4) == FALSE) bNewPage = csv.GetBool(4);
		if (csv.IsEmpty(5) == FALSE) nTimeCurSel = csv.GetInt(5);
		if (csv.IsEmpty(6) == FALSE) nSingleGroupNo = csv.GetInt(6);
		if (csv.IsEmpty(7) == FALSE) strDateRangeType = csv.GetString(7);
		if (csv.IsEmpty(8) == FALSE) strDateFrom = csv.GetString(8);
		if (csv.IsEmpty(9) == FALSE) strDateTo = csv.GetString(9);
		if (csv.IsEmpty(10) == FALSE) strTimeFrom = csv.GetString(10);
		if (csv.IsEmpty(11) == FALSE) strTimeTo = csv.GetString(11);
		if (csv.IsEmpty(12) == FALSE) bContinuous = csv.GetBool(12);
		if (csv.IsEmpty(13) == FALSE) bShowDeletedLines = csv.GetBool(13);
		if (csv.IsEmpty(14) == FALSE) bShowSourceManager = csv.GetBool(14);
		if (csv.IsEmpty(15) == FALSE) bShowSourceServer = csv.GetBool(15);
		if (csv.IsEmpty(16) == FALSE) bShowSourceBackground = csv.GetBool(16);
		if (csv.IsEmpty(17) == FALSE) bShowSourceManual = csv.GetBool(17);
		if (csv.IsEmpty(18) == FALSE) bShowPrevious = csv.GetBool(18);
	}

	if ((nReportRangeType < 0) || (nReportRangeType > 2))
	{
		nReportRangeType = 0;
	}
	m_radioRange.SetCheck(0 == nReportRangeType);
	m_radioSingle.SetCheck(1 == nReportRangeType);
	m_radioList.SetCheck(2 == nReportRangeType);

	m_checkSortByCardNo.SetCheck(bSortByCardNo);
	m_checkNewPage.SetCheck(bNewPage);
	
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

	// other
	SetRangeOptions();
	OnSelectDateRange();

	if (m_comboTimeRange.GetCurSel() != 0)
	{
		OnSelectTimeRange();
	}

	// single audit report
	if (m_ReportConfig.GetReportLabel() == "R141")
	{
		m_radioRange.EnableWindow(FALSE);
		m_radioSingle.EnableWindow(FALSE);
		m_editCardNoTo.EnableWindow(FALSE);
		m_radioList.EnableWindow(FALSE);
		m_tabcomboGroup.SetCurSel(0);
	}

	m_checkShowRemoved.SetCheck(bShowDeletedLines);
	m_checkSourceManager.SetCheck(bShowSourceManager);
	m_checkSourceServer.SetCheck(bShowSourceServer);
	m_checkSourceBackground.SetCheck(bShowSourceBackground);
	m_checkSourceManual.SetCheck(bShowSourceManual);
	m_checkShowPrevious.SetCheck(bShowPrevious);

	return TRUE;
}

//$$******************************************************************

void CGroupShiftReportDlg::OnSelectDateRange()
{
	m_DateSelectorRange.UpdateDateControls();
}

//$$******************************************************************

void CGroupShiftReportDlg::OnChangeDateFrom(NMHDR* pNMHDR, LRESULT* pResult)
{
	m_DateSelectorRange.OnChangeFromDate();
	m_DateSelectorRange.CheckForChangedDate();
	*pResult = 0;
}

//$$******************************************************************

void CGroupShiftReportDlg::OnChangeDateTo(NMHDR* pNMHDR, LRESULT* pResult)
{
	m_DateSelectorRange.CheckForChangedDate();
	*pResult = 0;
}

//$$******************************************************************

void CGroupShiftReportDlg::OnSelectTimeRange()
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
void CGroupShiftReportDlg::OnRadioRange() { SetCardRangeType(0); }
void CGroupShiftReportDlg::OnRadioSingle() { SetCardRangeType(1); }
void CGroupShiftReportDlg::OnRadioList() { SetCardRangeType(2); }
//$$******************************************************************

void CGroupShiftReportDlg::SetCardRangeType(int n)
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
void CGroupShiftReportDlg::OnToggleSortByCardNo() { SetRangeOptions(); }
//$$******************************************************************

void CGroupShiftReportDlg::SetRangeOptions()
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
void CGroupShiftReportDlg::OnDoubleClickRadioList() { OnButtonList(); }
//$$******************************************************************

void CGroupShiftReportDlg::OnButtonList()
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

void CGroupShiftReportDlg::OnDoubleClickRadioSingle()
{
	CFindDlg dlg( "", this);
	if (dlg.DoModal() == IDOK)
	{
		m_editCardNoTo.SetWindowText(FormatInt64Value(dlg.m_nRequestedUserID));
	}
}

//$$******************************************************************

void CGroupShiftReportDlg::OnOK()
{
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

	m_ReportConfig.SetStringCardNoFrom(GetEditBoxText(m_editCardNoFrom));
	m_ReportConfig.SetStringCardNoTo(GetEditBoxText(m_editCardNoTo));
	m_ReportConfig.UpdateInt64CardNo(nREPORT_SINGLE == nReportRangeType);
	m_ReportConfig.SetSortByCardNoFlag(IsTicked(m_checkSortByCardNo));
	m_ReportConfig.SetReportRangeType(nReportRangeType);
	m_ReportConfig.SetNewPageFlag(IsTicked(m_checkNewPage));
	m_ReportConfig.SetSingleGroupFlag(nSingleGroupNo > 0);
	m_ReportConfig.SetSingleGroupNo(nSingleGroupNo);
	m_ReportConfig.SetTimeSelection(m_comboTimeRange.GetCurSel());
	m_ReportConfig.SetContinuousFlag(IsTicked(m_checkContinuous));

	CString strDateRangeType;
	m_comboDateRange.GetWindowText(strDateRangeType);
	m_ReportConfig.SetDateRangeType(strDateRangeType);

	m_ReportConfig.SetShowDeletedLinesFlag(IsTicked(m_checkShowRemoved));
	m_ReportConfig.SetShowSourceManagerFlag(IsTicked(m_checkSourceManager));
	m_ReportConfig.SetShowSourceServerFlag(IsTicked(m_checkSourceServer));
	m_ReportConfig.SetShowSourceBackgroundFlag(IsTicked(m_checkSourceBackground));
	m_ReportConfig.SetShowSourceManualFlag(IsTicked(m_checkSourceManual));
	m_ReportConfig.SetShowPreviousFlag(IsTicked(m_checkShowPrevious));

	{
		CCSV csv;
		csv.Add(m_ReportConfig.GetReportRangeType());
		csv.Add(m_ReportConfig.GetStringCardNoFrom());
		csv.Add(m_ReportConfig.GetStringCardNoTo());
		csv.Add(m_ReportConfig.GetSortByCardNoFlag());
		csv.Add(m_ReportConfig.GetNewPageFlag());
		csv.Add(m_ReportConfig.GetTimeSelection());
		csv.Add(m_ReportConfig.GetSingleGroupNo());
		csv.Add(m_ReportConfig.GetDateRangeType());
		csv.Add(m_ReportConfig.GetDateFromString());
		csv.Add(m_ReportConfig.GetDateToString());
		csv.Add(m_ReportConfig.GetTimeFromString());
		csv.Add(m_ReportConfig.GetTimeToString());
		csv.Add(m_ReportConfig.GetContinousFlag());
		csv.Add(m_ReportConfig.GetShowDeletedLinesFlag());
		csv.Add(m_ReportConfig.GetShowSourceManagerFlag());
		csv.Add(m_ReportConfig.GetShowSourceServerFlag());
		csv.Add(m_ReportConfig.GetShowSourceBackgroundFlag());
		csv.Add(m_ReportConfig.GetShowSourceManualFlag());
		csv.Add(m_ReportConfig.GetShowPreviousFlag());

		CReportDefaultHandler repdefs;
		repdefs.Save(m_ReportConfig.GetReportLabel(), &csv);

		CCSV csvReportKey;
		m_ReportConfig.SetReportKey(csvReportKey.GetLine());
	}

	CRepmanHandler RepmanHandler("", this);
	RepmanHandler.SetTitle(m_ReportConfig.GetReportTitle());

	CGroupShiftReport GroupShiftReport( m_ReportConfig);

	int nErrNo = GroupShiftReport.CreateReport();

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

