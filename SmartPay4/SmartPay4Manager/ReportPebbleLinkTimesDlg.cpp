//$$******************************************************************
#include "..\SmartPay4Shared\SQLTable_AuditPeriod\SQLRowSetAuditPeriod.h"
//$$******************************************************************
#include "..\SmartPay4Shared\Defines.h"
//$$******************************************************************
#include "FindDlg.h"
#include "GroupComboHelpers.h"
#include "ReportDefaultHandler.h"
//$$******************************************************************
#include "ReportPebbleLinkTimesDlg.h"
//$$******************************************************************

CString CReportPebbleLinkInfo::GetDateString()
{
	CString strBuf = "";

	if (m_dateFrom != m_dateTo)
	{
		strBuf.Format(" (%s - %s)", m_dateFrom.GetDate(), m_dateTo.GetDate());
	}
	else
	{
		strBuf.Format(" (%s)", m_dateFrom.GetDate());			/* start date */
	}

	return strBuf;
}

//$$******************************************************************

CReportPebbleLinkTimesDlg::CReportPebbleLinkTimesDlg( CReportPebbleLinkInfo& ReportInfo, CWnd* pParent /*=NULL*/)
	: CSSAutoShutdownDialog(AutoShutdownHelper, CReportPebbleLinkTimesDlg::IDD, pParent)
	, m_ReportInfo(ReportInfo)
	, m_DateSelectorRange(m_comboDateRange, m_DatePickerFrom, m_DatePickerTo)
{
	m_ReportInfo.m_strReportLabel = "R144";
	m_ReportInfo.m_strReportFilename = Filenames.GetReportFilename(m_ReportInfo.m_strReportLabel);
	m_ReportInfo.m_strParamsFilename = Filenames.GetReportParamsFilename(m_ReportInfo.m_strReportLabel);
	m_ReportInfo.m_strReportKey = "";

	m_ReportInfo.m_strReportTitle = "Pebble Link Time Report";

	m_ReportInfo.m_bTimeRangeSet = FALSE;
	m_ReportInfo.m_bContinuous = FALSE;

	m_ReportInfo.m_dateFrom.SetCurrentDate();					// default to today
	m_ReportInfo.m_dateTo.SetCurrentDate();						// could be set before DoModal call
}

//$$******************************************************************

CReportPebbleLinkTimesDlg::~CReportPebbleLinkTimesDlg()
{
}

//$$******************************************************************

void CReportPebbleLinkTimesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_DatePickerFrom);
	DDX_Control(pDX, IDC_DATETIMEPICKER2, m_DatePickerTo);
	DDX_Control(pDX, IDC_DATETIMEPICKER3, m_TimePickerStart);
	DDX_Control(pDX, IDC_DATETIMEPICKER4, m_TimePickerEnd);
	DDX_Control(pDX, IDC_COMBO_DATERANGE, m_comboDateRange);
	DDX_Control(pDX, IDC_COMBO_TIMERANGE, m_comboTimeRange);
	DDX_Control(pDX, IDC_CHECK_CONTINUOUS, m_checkContinuous);
	/*****/
	DDX_Control(pDX, IDC_RADIO_RANGE, m_radioRange);
	DDX_Control(pDX, IDC_RADIO_SINGLE, m_radioSingle);
	DDX_Control(pDX, IDC_EDIT_FROMCARDNO, m_editCardNoFrom);
	DDX_Control(pDX, IDC_EDIT_TOCARDNO, m_editCardNoTo);
	DDX_Control(pDX, IDC_COMBO_GROUP, m_tabcomboGroup);
	DDX_Control(pDX, IDC_CHECK_SORTDATE, m_checkSortDate);
	DDX_Control(pDX, IDC_CHECK_SORTGROUP, m_checkSortGroup);
	DDX_Control(pDX, IDC_CHECK_SORTID, m_checkSortID);
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CReportPebbleLinkTimesDlg, CSSAutoShutdownDialog)
	ON_CBN_SELCHANGE(IDC_COMBO_DATERANGE, OnSelectDateRange)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER1, OnChangeDateFrom)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER2, OnChangeDateTo)
	ON_CBN_SELCHANGE(IDC_COMBO_TIMERANGE, OnSelectTimeRange)
	/*****/
	ON_BN_CLICKED(IDC_RADIO_RANGE, OnRadioRange)
	ON_BN_CLICKED(IDC_RADIO_SINGLE, OnRadioSingle)
	ON_BN_DOUBLECLICKED(IDC_RADIO_SINGLE, OnDoubleClickRadioSingle)
END_MESSAGE_MAP()

//$$******************************************************************

BOOL CReportPebbleLinkTimesDlg::CSSAutoShutdownPostInitDialog()
{
	m_editCardNoFrom.LimitText(System.GetUserIDLength());
	m_editCardNoTo.LimitText(System.GetUserIDLength());
	
	CString strDateRangeType = szRANGE_TODAY;

	CSSDate dateNow;
	dateNow.SetCurrentDate();
	COleDateTime oleDateNow(dateNow.GetYear(), dateNow.GetMonth(), dateNow.GetDay(), 0, 0, 0);
	m_DatePickerFrom.SetTime(oleDateNow);
	m_DatePickerTo.SetTime(oleDateNow);
	CString strDateFrom = dateNow.GetDate();
	CString strDateTo = dateNow.GetDate();

	COleDateTime oleTimeStart(2020, 1, 1, 0, 0, 0);
	COleDateTime oleTimeEnd(2020, 1, 1, 23, 59, 0);
	m_TimePickerStart.SetTime(oleTimeStart);
	m_TimePickerEnd.SetTime(oleTimeEnd);
	CString strTimeStart = "00:00";
	CString strTimeEnd = "23:59";
	
	int nTimeCursel = 0;
	m_ReportInfo.m_bContinuous = FALSE;
	m_ReportInfo.m_nSingleGroupNo = 0;
	m_ReportInfo.m_strCardNoFrom = System.GetLowCardNo();
	m_ReportInfo.m_strCardNoTo = System.GetHighCardNo();
	m_ReportInfo.m_bSortByDate = FALSE;
	m_ReportInfo.m_bSortByGroup = FALSE;
	m_ReportInfo.m_bSortByID = FALSE;

	CString strLine;
	CReportDefaultHandler repdefs;
	if (repdefs.Read(m_ReportInfo.m_strReportLabel, strLine) == TRUE)
	{
		CCSV csv(strLine);
		if (csv.IsEmpty(0) == FALSE)	nTimeCursel = csv.GetInt(0);
		if (csv.IsEmpty(1) == FALSE)	strDateRangeType = csv.GetString(1);
		if (csv.IsEmpty(2) == FALSE)	strDateFrom = csv.GetString(2);
		if (csv.IsEmpty(3) == FALSE)	strDateTo = csv.GetString(3);
		if (csv.IsEmpty(4) == FALSE)	strTimeStart = csv.GetString(4);
		if (csv.IsEmpty(5) == FALSE)	strTimeEnd = csv.GetString(5);
		if (csv.IsEmpty(6) == FALSE)	m_ReportInfo.m_bContinuous = csv.GetBool(6);
		if (csv.IsEmpty(7) == FALSE)	m_ReportInfo.m_nSingleGroupNo = csv.GetInt(7);
		if (csv.IsEmpty(8) == FALSE)	m_ReportInfo.m_nReportRangeType = csv.GetInt(8);
		if (csv.IsEmpty(9) == FALSE)	m_ReportInfo.m_strCardNoFrom = csv.GetString(9);
		if (csv.IsEmpty(10) == FALSE)	m_ReportInfo.m_strCardNoTo = csv.GetString(10);
		if (csv.IsEmpty(11) == FALSE)	m_ReportInfo.m_bSortByDate = csv.GetBool(11);
		if (csv.IsEmpty(12) == FALSE)	m_ReportInfo.m_bSortByGroup = csv.GetBool(12);
		if (csv.IsEmpty(13) == FALSE)	m_ReportInfo.m_bSortByID = csv.GetBool(13);
	}

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

	if ((nTimeCursel >= 0) && (nTimeCursel < m_comboTimeRange.GetCount()))
	{
		m_comboTimeRange.SetCurSel(nTimeCursel);
	}
	else
	{
		m_comboTimeRange.SetCurSel(0);
	}

	if (m_comboTimeRange.GetCount() < 2)
	{
		m_comboTimeRange.EnableWindow(FALSE);
	}

	m_TimePickerStart.SetFormat("HH:mm");
	m_TimePickerEnd.SetFormat("HH:mm");

	{
		CSSTime timeFrom(strTimeStart);
		CSSTime timeTo(strTimeEnd);

		if (timeFrom.IsSet() && timeTo.IsSet())
		{
			m_TimePickerStart.SetTime(COleDateTime(2019, 1, 1, timeFrom.GetHour(), timeFrom.GetMinute(), 0));
			m_TimePickerEnd.SetTime(COleDateTime(2019, 1, 1, timeTo.GetHour(), timeTo.GetMinute(), 0));
		}
		else
		{
			OnSelectTimeRange();
		}
	}

	m_checkContinuous.SetCheck(m_ReportInfo.m_bContinuous);

	CGroupComboHelpers::FillGroupCombo(m_tabcomboGroup, m_ReportInfo.m_nSingleGroupNo, TRUE);

	if ((m_ReportInfo.m_nReportRangeType < 0) || (m_ReportInfo.m_nReportRangeType > 2))
	{
		m_ReportInfo.m_nReportRangeType = 0;
	}
	m_radioRange.SetCheck(0 == m_ReportInfo.m_nReportRangeType);
	m_radioSingle.SetCheck(1 == m_ReportInfo.m_nReportRangeType);

	SetCardRangeType(m_ReportInfo.m_nReportRangeType);
	m_editCardNoFrom.SetWindowText(m_ReportInfo.m_strCardNoFrom);
	m_editCardNoTo.SetWindowText(m_ReportInfo.m_strCardNoTo);

	m_checkSortDate.SetCheck(m_ReportInfo.m_bSortByDate);
	m_checkSortGroup.SetCheck(m_ReportInfo.m_bSortByGroup);
	m_checkSortID.SetCheck(m_ReportInfo.m_bSortByID);

	return TRUE;
}

//$$******************************************************************

void CReportPebbleLinkTimesDlg::OnSelectDateRange()
{
	m_DateSelectorRange.UpdateDateControls();
}

//$$******************************************************************

void CReportPebbleLinkTimesDlg::OnChangeDateFrom(NMHDR* pNMHDR, LRESULT* pResult)
{
	m_DateSelectorRange.OnChangeFromDate();
	m_DateSelectorRange.CheckForChangedDate();
	*pResult = 0;
}

//$$******************************************************************

void CReportPebbleLinkTimesDlg::OnChangeDateTo(NMHDR* pNMHDR, LRESULT* pResult)
{
	m_DateSelectorRange.CheckForChangedDate();
	*pResult = 0;
}

//$$******************************************************************

void CReportPebbleLinkTimesDlg::OnSelectTimeRange()
{
	int nStartHour = 0;
	int nStartMins = 0;
	int nEndHour = 0;
	int nEndMins = 0;

	CString strTimeRange = "";
	m_comboTimeRange.GetWindowText(strTimeRange);
	SQLRowSetAuditPeriod.GetTimeRange(strTimeRange, nStartHour, nStartMins, nEndHour, nEndMins);

	COleDateTime time;

	time.SetTime(nStartHour, nStartMins, 0);
	m_TimePickerStart.SetTime(time);

	time.SetTime(nEndHour, nEndMins, 0);
	m_TimePickerEnd.SetTime(time);
}

//$$******************************************************************


//$$******************************************************************
void CReportPebbleLinkTimesDlg::OnRadioRange() { SetCardRangeType(0); }
void CReportPebbleLinkTimesDlg::OnRadioSingle() { SetCardRangeType(1); }
//$$******************************************************************

void CReportPebbleLinkTimesDlg::SetCardRangeType(int n)
{
	if ((n < 0) || (n > 1))
	{
		n = 0;
	}

	m_radioRange.SetCheck(0 == n);
	m_radioSingle.SetCheck(1 == n);
	
	if (0 == n)
	{
		m_editCardNoFrom.SetWindowText(System.GetLowCardNo());
		m_editCardNoTo.SetWindowText(System.GetHighCardNo());
	}

	m_ReportInfo.m_nReportRangeType = n;
	SetRangeOptions();
}

//$$******************************************************************

void CReportPebbleLinkTimesDlg::OnDoubleClickRadioSingle()
{
	CFindDlg dlg("", this);
	if (dlg.DoModal() == IDOK)
	{
		m_editCardNoTo.SetWindowText(FormatInt64Value(dlg.m_nRequestedUserID));
	}
}

//$$******************************************************************

void CReportPebbleLinkTimesDlg::SetRangeOptions()
{
	bool bAllowFrom = TRUE;
	bool bAllowGroup = (m_tabcomboGroup.GetCount() > 2) ? TRUE : FALSE;
	
	switch (m_ReportInfo.m_nReportRangeType)
	{
	case nREPORT_SINGLE:
		bAllowFrom = FALSE;
		break;
	}

	m_tabcomboGroup.EnableWindow(bAllowGroup);
	m_editCardNoFrom.EnableWindow(bAllowFrom);

	switch (m_ReportInfo.m_nReportRangeType)
	{
	case nREPORT_RANGE:
		m_editCardNoFrom.SetFocus();
		m_editCardNoFrom.SetSel(0, -1);
		break;

	case nREPORT_SINGLE:
		m_editCardNoTo, SetFocus();
		m_editCardNoTo.SetSel(0, -1);
		break;
	}
}

//$$******************************************************************

void CReportPebbleLinkTimesDlg::OnOK()
{
	CTime dateFrom;
	m_DatePickerFrom.GetTime(dateFrom);
	m_ReportInfo.m_dateFrom.SetDate(dateFrom);

	CTime dateTo;
	m_DatePickerTo.GetTime(dateTo);
	m_ReportInfo.m_dateTo.SetDate(dateTo);

	CTime timeStart;
	m_TimePickerStart.GetTime(timeStart);
	CString strStartTime = timeStart.Format("%H:%M");
	m_ReportInfo.m_timeStart.SetTime(strStartTime);

	CTime timeEnd;
	m_TimePickerEnd.GetTime(timeEnd);
	CString strEndTime = timeEnd.Format("%H:%M");
	m_ReportInfo.m_timeEnd.SetTime(strEndTime);
	
	m_ReportInfo.m_bTimeRangeSet = ((strStartTime == "00:00") && (strEndTime == "23:59")) ? FALSE : TRUE;
	m_ReportInfo.m_bContinuous = IsTicked(m_checkContinuous);

	CString strDateRangeType;
	m_comboDateRange.GetWindowText(strDateRangeType);

	m_ReportInfo.m_nSingleGroupNo = CGroupComboHelpers::GetGroupNoFromCombo(m_tabcomboGroup, 0);
	m_ReportInfo.m_strCardNoFrom = GetEditBoxText(m_editCardNoFrom);
	m_ReportInfo.m_strCardNoTo = GetEditBoxText(m_editCardNoTo);
	
	m_ReportInfo.m_bSortByDate = IsTicked(m_checkSortDate);
	m_ReportInfo.m_bSortByGroup = IsTicked(m_checkSortGroup);
	m_ReportInfo.m_bSortByID = IsTicked(m_checkSortID);

	CCSV csv;
	csv.Add(m_comboTimeRange.GetCurSel());
	csv.Add(strDateRangeType);
	csv.Add(m_ReportInfo.m_dateFrom.GetDate());
	csv.Add(m_ReportInfo.m_dateTo.GetDate());
	csv.Add(m_ReportInfo.m_timeStart.GetTime());
	csv.Add(m_ReportInfo.m_timeEnd.GetTime());
	csv.Add(m_ReportInfo.m_bContinuous);
	csv.Add(m_ReportInfo.m_nSingleGroupNo);
	csv.Add(m_ReportInfo.m_nReportRangeType);
	csv.Add(m_ReportInfo.m_strCardNoFrom);
	csv.Add(m_ReportInfo.m_strCardNoTo);
	csv.Add(m_ReportInfo.m_bSortByDate);
	csv.Add(m_ReportInfo.m_bSortByGroup);
	csv.Add(m_ReportInfo.m_bSortByID);

	CReportDefaultHandler repdefs;
	repdefs.Save(m_ReportInfo.m_strReportLabel, &csv);

	CCSV csvReportKey;
	csvReportKey.Add("X");
	m_ReportInfo.m_strReportKey = csvReportKey.GetLine();

	EndDialog(IDOK);
}

//$$******************************************************************
