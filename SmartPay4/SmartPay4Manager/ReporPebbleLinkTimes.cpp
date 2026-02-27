//$$******************************************************************
#include "ReportDefaultHandler.h"
//$$******************************************************************
#include "..\SmartPay4Shared\SQLTable_AuditPeriod\SQLRowSetAuditPeriod.h"
//$$******************************************************************
#include "ReportKioskAuditDlg.h"
//$$******************************************************************

CReportKioskAuditDlg::CReportKioskAuditDlg( const char* szLabel, CWnd* pParent /*=NULL*/)
	: CSSAutoShutdownDialog(AutoShutdownHelper, CReportKioskAuditDlg::IDD, pParent)
	, m_DateSelectorRange(m_comboDateRange, m_DatePickerFrom, m_DatePickerTo)
{
	m_strReportLabel = szLabel;
	m_strReportList = Filenames.GetReportCardListFilename(m_strReportLabel);
	m_strReportFilename = Filenames.GetReportFilename(m_strReportLabel);
	m_strParamsFilename = Filenames.GetReportParamsFilename(m_strReportLabel);
	m_strReportKey = "";

	m_strReportTitle = "Kiosk Cash Taken Audit Report";

	m_bSingleKiosk = FALSE;
	m_bTimeRangeSet = FALSE;
	m_bContinuous = FALSE;

	m_dateFrom.SetCurrentDate();					// default to today
	m_dateTo.SetCurrentDate();						// could be set before DoModal call
}

//$$******************************************************************

CReportKioskAuditDlg::~CReportKioskAuditDlg()
{
}

//$$******************************************************************

void CReportKioskAuditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_DatePickerFrom);
	DDX_Control(pDX, IDC_DATETIMEPICKER2, m_DatePickerTo);
	DDX_Control(pDX, IDC_DATETIMEPICKER3, m_TimePickerStart);
	DDX_Control(pDX, IDC_DATETIMEPICKER4, m_TimePickerEnd);
	DDX_Control(pDX, IDC_COMBO_DATERANGE, m_comboDateRange);
	DDX_Control(pDX, IDC_COMBO_TIMERANGE, m_comboTimeRange);
	DDX_Control(pDX, IDC_CHECK_CONTINUOUS, m_checkContinuous);
	DDX_Control(pDX, IDC_CHECK_NEWPAGE, m_checkNewPage);
	DDX_Control(pDX, IDC_CHECK_SUMMARY, m_checkSummaryReport);
	DDX_Control(pDX, IDC_CHECK_SINGLE, m_checkSingleKiosk);
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CReportKioskAuditDlg, CSSAutoShutdownDialog)
	ON_CBN_SELCHANGE(IDC_COMBO_DATERANGE, OnSelectDateRange)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER1, OnChangeDateFrom)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER2, OnChangeDateTo)
	ON_CBN_SELCHANGE(IDC_COMBO_TIMERANGE, OnSelectTimeRange)
	ON_BN_CLICKED(IDC_CHECK_SINGLE, OnToggleSingle)
END_MESSAGE_MAP()

//$$******************************************************************
CEdit* CReportKioskAuditDlg::GetEditKioskNo() { return GetEdit(IDC_EDIT_KIOSKNO); }
//$$******************************************************************

BOOL CReportKioskAuditDlg::CSSAutoShutdownPostInitDialog()
{
	SetWindowText(m_strReportTitle);

	SubclassEdit(IDC_EDIT_KIOSKNO, SS_NUM, 6, "%d");

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
	bool bNewPage = FALSE;
	bool bSummaryReport = FALSE;
	bool bSingleKiosk = FALSE;
	bool bContinuous = FALSE;
	int nKioskNo = 0;
	
	CString strLine;
	CReportDefaultHandler repdefs;
	if (repdefs.Read(m_strReportLabel, strLine) == TRUE)
	{
		CCSV csv(strLine);
		//0 WAS DATE CURSEL
		if (csv.IsEmpty(1) == FALSE)	nTimeCursel = csv.GetInt(1);
		if (csv.IsEmpty(2) == FALSE)	bNewPage = csv.GetBool(2);
		if (csv.IsEmpty(3) == FALSE)	bSummaryReport = csv.GetBool(3);
		if (csv.IsEmpty(4) == FALSE)	bSingleKiosk = csv.GetBool(4);
		if (csv.IsEmpty(5) == FALSE)	nKioskNo = csv.GetInt(5);
		if (csv.IsEmpty(6) == FALSE)	strDateRangeType = csv.GetString(6);
		if (csv.IsEmpty(7) == FALSE)	strDateFrom = csv.GetString(7);
		if (csv.IsEmpty(8) == FALSE)	strDateTo = csv.GetString(8);
		if (csv.IsEmpty(9) == FALSE)	strTimeStart = csv.GetString(9);
		if (csv.IsEmpty(10) == FALSE)	strTimeEnd = csv.GetString(10);
		if (csv.IsEmpty(11) == FALSE)	bContinuous = csv.GetBool(11);
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

	m_checkContinuous.SetCheck(bContinuous);

	// other
	m_checkNewPage.SetCheck(bNewPage);
	m_checkSummaryReport.SetCheck(bSummaryReport);
	m_checkSingleKiosk.SetCheck(bSingleKiosk);
	SetEditBoxInt(*GetEditKioskNo(), nKioskNo);
	OnToggleSingle();
	
	return TRUE;
}

//$$******************************************************************

void CReportKioskAuditDlg::OnSelectDateRange()
{
	m_DateSelectorRange.UpdateDateControls();
}

//$$******************************************************************

void CReportKioskAuditDlg::OnChangeDateFrom(NMHDR* pNMHDR, LRESULT* pResult)
{
	m_DateSelectorRange.OnChangeFromDate();
	m_DateSelectorRange.CheckForChangedDate();
	*pResult = 0;
}

//$$******************************************************************

void CReportKioskAuditDlg::OnChangeDateTo(NMHDR* pNMHDR, LRESULT* pResult)
{
	m_DateSelectorRange.CheckForChangedDate();
	*pResult = 0;
}

//$$******************************************************************

void CReportKioskAuditDlg::OnSelectTimeRange()
{
	int nStartHour;
	int nStartMins;
	int nEndHour;
	int nEndMins;

	CString strTimeRange;
	m_comboTimeRange.GetWindowText(strTimeRange);
	SQLRowSetAuditPeriod.GetTimeRange(strTimeRange, nStartHour, nStartMins, nEndHour, nEndMins);

	COleDateTime time;

	time.SetTime(nStartHour, nStartMins, 0);
	m_TimePickerStart.SetTime(time);

	time.SetTime(nEndHour, nEndMins, 0);
	m_TimePickerEnd.SetTime(time);
}

//$$******************************************************************

bool CReportKioskAuditDlg::IsValid(CSQLAuditRecord* pAtc)
{
	if (pAtc->m_dateFiled < m_dateFrom)
		return FALSE;

	if (pAtc->m_dateFiled > m_dateTo)
		return FALSE;

	if (m_bTimeRangeSet == TRUE)
	{
		if (FALSE == m_bContinuous)
		{
			if (pAtc->m_timeFiled < m_timeStart)
				return FALSE;

			if (pAtc->m_timeFiled > m_timeEnd)
				return FALSE;
		}
		else
		{
			if (pAtc->m_dateFiled == m_dateFrom)
			{
				if (pAtc->m_timeFiled < m_timeStart)
					return FALSE;
			}

			if (pAtc->m_dateFiled == m_dateTo)
			{
				if (pAtc->m_timeFiled > m_timeEnd)
					return FALSE;
			}
		}
	}

	if (pAtc->GetSourceType() != AUDIT_KIOSKLINK)
		return FALSE;

	int nTNo = pAtc->GetTerminalNo();

	if (nTNo == 0)
		return FALSE;

	if ((m_bSingleKiosk == TRUE) && (nTNo != m_nKioskNo))
		return FALSE;

	return TRUE;
}

//$$******************************************************************

CString CReportKioskAuditDlg::GetDateString()
{
	CString strBuf;

	if (m_dateFrom != m_dateTo)	strBuf.Format(" (%s - %s)", m_dateFrom.GetDate(), m_dateTo.GetDate());
	else						strBuf.Format(" (%s)", m_dateFrom.GetDate());			/* start date */

	return strBuf;
}

//$$******************************************************************

void CReportKioskAuditDlg::OnToggleSingle()
{
	bool bSingle = IsTicked(m_checkSingleKiosk);
	m_checkNewPage.EnableWindow(!bSingle);
	GetEditKioskNo()->EnableWindow(bSingle);
	GetEditKioskNo()->SetFocus();
}

//$$******************************************************************

void CReportKioskAuditDlg::OnOK()
{
	CTime dateFrom;
	m_DatePickerFrom.GetTime(dateFrom);
	m_dateFrom.SetDate(dateFrom);

	CTime dateTo;
	m_DatePickerTo.GetTime(dateTo);
	m_dateTo.SetDate(dateTo);

	CTime timeStart;
	m_TimePickerStart.GetTime(timeStart);
	CString strStartTime = timeStart.Format("%H:%M");
	m_timeStart.SetTime(strStartTime);

	CTime timeEnd;
	m_TimePickerEnd.GetTime(timeEnd);
	CString strEndTime = timeEnd.Format("%H:%M");
	m_timeEnd.SetTime(strEndTime);
	
	m_bTimeRangeSet = ((strStartTime == "00:00") && (strEndTime == "23:59")) ? FALSE : TRUE;
	m_bSingleKiosk = IsTicked(m_checkSingleKiosk);
	m_bSummaryReport = IsTicked(m_checkSummaryReport);
	m_bContinuous = IsTicked(m_checkContinuous);
	m_bNewPage = IsTicked(m_checkNewPage);
	m_nKioskNo = GetEditBoxInt(*GetEditKioskNo());

	CString strDateRangeType;
	m_comboDateRange.GetWindowText(strDateRangeType);

	CCSV csv;
	csv.Add(0); //WAS DATE RANGE TYPE
	csv.Add(m_comboTimeRange.GetCurSel());
	csv.Add(IsTicked(m_checkNewPage));
	csv.Add(IsTicked(m_checkSummaryReport));
	csv.Add(IsTicked(m_checkSingleKiosk));
	csv.Add(m_nKioskNo);
	csv.Add(strDateRangeType);
	csv.Add(m_dateFrom.GetDate());
	csv.Add(m_dateTo.GetDate());
	csv.Add(m_timeStart.GetTime());
	csv.Add(m_timeEnd.GetTime());
	csv.Add(m_bContinuous);

	CReportDefaultHandler repdefs;
	repdefs.Save(m_strReportLabel, &csv);

	CCSV csvReportKey;
	csvReportKey.Add(m_comboDateRange.GetCurSel());
	csvReportKey.Add(m_comboTimeRange.GetCurSel());
	csv.Add(IsTicked(m_checkNewPage));
	csv.Add(IsTicked(m_checkSummaryReport));
	csv.Add(IsTicked(m_checkSingleKiosk));
	m_strReportKey = csvReportKey.GetLine();

	EndDialog(IDOK);
}

//$$******************************************************************
