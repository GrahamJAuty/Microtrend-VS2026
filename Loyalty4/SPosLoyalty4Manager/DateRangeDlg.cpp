//$$******************************************************************
// used by	"R116"	Daily Differ Report
//			"R121"	Visit Totaliser
//$$******************************************************************
#include "..\SPosLoyalty4Shared\SQLTable_AuditPeriod\SQLRowSetAuditPeriod.h"
//$$******************************************************************
#include "LoyaltyManager.h"
#include "ReportDefaultHandler.h"
//$$******************************************************************
#include "DateRangeDlg.h"
//$$******************************************************************

CDateRangeDlg::CDateRangeDlg( const char* szLabel, CWnd* pParent)
	: CSSDialog(CDateRangeDlg::IDD, pParent)
	, m_DateSelectorRange( m_comboDateRange, m_DatePickerFrom, m_DatePickerTo )
{
	m_pReportInfo = NULL;
	m_strReportLabel = szLabel;
	SQLRowSetAuditPeriod.LoadPeriods(NULL);
}

//$$******************************************************************

void CDateRangeDlg::DoDataExchange(CDataExchange* pDX)
{
	CSSDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_RANGE, m_staticRange);
	//date
	DDX_Control(pDX, IDC_COMBO_DATERANGE, m_comboDateRange);
	DDX_Control(pDX, IDC_DATEPICKER_FROM, m_DatePickerFrom);
	DDX_Control(pDX, IDC_DATEPICKER_TO, m_DatePickerTo);
	//time
	DDX_Control(pDX, IDC_COMBO_TIMERANGE, m_comboTimeRange);
	DDX_Control(pDX, IDC_STATIC_START, m_staticStart);
	DDX_Control(pDX, IDC_TIMEPICKER_FROM, m_TimePickerFrom);
	DDX_Control(pDX, IDC_STATIC_END, m_staticEnd);
	DDX_Control(pDX, IDC_TIMEPICKER_TO, m_TimePickerTo);
	//Options
	DDX_Control(pDX, IDC_CHECK_SUMMARY, m_checkSummary);
	//Close buttons
	DDX_Control(pDX, IDOK, m_buttonOK);
	DDX_Control(pDX, IDCANCEL, m_buttonCancel);
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CDateRangeDlg, CSSDialog)

	//date
	ON_CBN_SELCHANGE(IDC_COMBO_DATERANGE, OnSelectDateRange)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATEPICKER_FROM, OnChangeDateFrom)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATEPICKER_TO, OnChangeDateTo)
	//time
	ON_CBN_SELCHANGE(IDC_COMBO_TIMERANGE, OnSelectTimeRange)

END_MESSAGE_MAP()

//$$******************************************************************

BOOL CDateRangeDlg::OnInitDialog()
{
	CSSDialog::OnInitDialog();
	SetWindowText(m_pReportInfo->GetReportTitle());

	int nPreviousTimeCurSel = 0;
	bool bPreviousSummary = FALSE;
	CString strPreviousDateType = "";
	CString strPreviousDateFrom = "";
	CString strPreviousDateTo = "";
	CString strPreviousTimeFrom = "";
	CString strPreviousTimeTo = "";

	CString strLine;
	CReportDefaultHandler repdefs;
	if (repdefs.Read(m_strReportLabel, strLine) == TRUE)
	{
		CCSV csv(strLine);
		if (csv.IsEmpty(0) == FALSE)	strPreviousDateType = csv.GetString(0);
		if (csv.IsEmpty(1) == FALSE)	nPreviousTimeCurSel = csv.GetInt(1);
		if (csv.IsEmpty(2) == FALSE)	bPreviousSummary = csv.GetBool(2);
		if (csv.IsEmpty(3) == FALSE)	strPreviousDateFrom = csv.GetString(3);
		if (csv.IsEmpty(4) == FALSE)	strPreviousDateTo = csv.GetString(4);
		if (csv.IsEmpty(5) == FALSE)	strPreviousTimeFrom = csv.GetString(5);
		if (csv.IsEmpty(6) == FALSE)	strPreviousTimeTo = csv.GetString(6);
	}

	m_checkSummary.SetCheck(bPreviousSummary);

	// Date range
	m_DateSelectorRange.InitialiseDateControls();

	if (m_strReportLabel != "R121")				// Visit Totaliser
	{
		m_comboDateRange.AddString(szRANGE_TODAY);
		m_comboDateRange.AddString(szRANGE_YESTERDAY);
		m_comboDateRange.AddString(szRANGE_THIS_WEEK);
		m_comboDateRange.AddString(szRANGE_THIS_MONTH);
	}

	m_comboDateRange.AddString(szRANGE_THIS_YEAR);

	if (m_strReportLabel != "R121")				// Visit Totaliser
	{
		m_comboDateRange.AddString(szRANGE_LAST_WEEK);
		m_comboDateRange.AddString(szRANGE_LAST_MONTH);
	}

	m_comboDateRange.AddString(szRANGE_LAST_YEAR);
	m_comboDateRange.AddString(szRANGE_LAST_3_MONTHS);
	m_comboDateRange.AddString(szRANGE_LAST_6_MONTHS);
	m_comboDateRange.AddString(szRANGE_LAST_9_MONTHS);
	m_comboDateRange.AddString(szRANGE_LAST_12_MONTHS);
	m_comboDateRange.AddString(szRANGE_ALL);
	m_comboDateRange.AddString(szRANGE_CUSTOM_DATE);
	m_comboDateRange.AddString(szRANGE_CUSTOM_RANGE);

	m_DateSelectorRange.SetComboSelection(strPreviousDateType);

	if ((strPreviousDateType == szRANGE_CUSTOM_DATE) || (strPreviousDateType == szRANGE_CUSTOM_RANGE))
	{
		CSSDate dateFrom(strPreviousDateFrom);
		CSSDate dateTo(strPreviousDateTo);

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

	m_comboTimeRange.AddString(szTIMERANGE_ALL);
	for (int nPeriodIndex = 0; nPeriodIndex < nMAXPERIODS; nPeriodIndex++)
	{
		if (SQLRowSetAuditPeriod.IsPeriodSet(nPeriodIndex) == TRUE)
		{
			m_comboTimeRange.AddString(SQLRowSetAuditPeriod.GetPeriodName(nPeriodIndex));
		}
	}
	m_comboTimeRange.SetCurSel(nPreviousTimeCurSel);

	if (m_comboTimeRange.GetCount() < 2)
	{
		m_comboTimeRange.EnableWindow(FALSE);
	}

	m_TimePickerFrom.SetFormat("HH:mm");
	m_TimePickerTo.SetFormat("HH:mm");

	{
		CSSTime timeFrom(strPreviousTimeFrom);
		CSSTime timeTo(strPreviousTimeTo);

		if (timeFrom.IsSet())
		{
			m_TimePickerFrom.SetTime(COleDateTime(2019, 1, 1, timeFrom.GetHour(), timeFrom.GetMinute(), 0));
		}
		else
		{
			OnSelectTimeRange();
		}

		if (timeTo.IsSet())
		{
			m_TimePickerTo.SetTime(COleDateTime(2019, 1, 1, timeTo.GetHour(), timeTo.GetMinute(), 0));
		}
		else
		{
			OnSelectTimeRange();
		}
	}

	// set display

	if (m_pReportInfo->IsSummaryAllowed() == FALSE)
	{
		m_checkSummary.ShowWindow(SW_HIDE);
	}
	else
	{
		m_checkSummary.SetWindowText(m_pReportInfo->GetSummaryText());
	}

	if (m_pReportInfo->IsTimeAllowed() == FALSE)
	{
		m_staticRange.SetWindowText("Date Range");
		ShowAndEnableWindow(&m_comboTimeRange, FALSE);
		ShowAndEnableWindow(&m_TimePickerFrom, FALSE);
		ShowAndEnableWindow(&m_TimePickerTo, FALSE);
		ShowAndEnableWindow(&m_staticStart, FALSE);
		ShowAndEnableWindow(&m_staticEnd, FALSE);
		MoveControl(&m_staticRange, 7, 7, 160, 91);
		MoveControl(&m_buttonOK, 186, 89, 50, 14);
		MoveControl(&m_buttonCancel, 186, 105, 50, 14);
		ResizeDialog(243, 126);
	}

	return TRUE;
}

//$$******************************************************************

void CDateRangeDlg::OnSelectDateRange() 
{
	m_DateSelectorRange.UpdateDateControls();
}

//$$******************************************************************

void CDateRangeDlg::OnChangeDateFrom(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_DateSelectorRange.OnChangeFromDate();
	m_DateSelectorRange.CheckForChangedDate();
	*pResult = 0;
}

//$$******************************************************************

void CDateRangeDlg::OnChangeDateTo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_DateSelectorRange.CheckForChangedDate();
	*pResult = 0;
}

//$$******************************************************************

void CDateRangeDlg::OnSelectTimeRange() 
{
	int nStartHour = 0;
	int nStartMins = 0;
	int nEndHour = 23;
	int nEndMins = 59;

	CString strTimeRange;
	m_comboTimeRange.GetWindowText( strTimeRange );
	SQLRowSetAuditPeriod.GetTimeRange ( strTimeRange, nStartHour, nStartMins, nEndHour, nEndMins );

	COleDateTime startTime( 2019, 1, 1, nStartHour, nStartMins, 0 );
	m_TimePickerFrom.SetTime ( startTime );

	COleDateTime endTime( 2019, 1, 1, nEndHour, nEndMins, 0 );
	m_TimePickerTo.SetTime ( endTime );
}

//$$******************************************************************

void CDateRangeDlg::OnOK()
{
	if ( UpdateData() == TRUE )
	{
		{
			CTime dateFrom;
			m_DatePickerFrom.GetTime( dateFrom );
			m_pReportInfo -> SetDateFrom( dateFrom );
		}

		{
			CTime dateTo;
			m_DatePickerTo.GetTime( dateTo );
			m_pReportInfo -> SetDateTo( dateTo );
		}

		{
			CTime timeFrom;
			m_TimePickerFrom.GetTime ( timeFrom );
			CString strStartTime = timeFrom.Format ( "%H:%M" );
			m_pReportInfo -> SetTimeFrom( timeFrom );

			CTime timeTo;
			m_TimePickerTo.GetTime ( timeTo );
			CString strEndTime = timeTo.Format ( "%H:%M" );
			m_pReportInfo -> SetTimeTo( timeTo );

			m_pReportInfo -> SetTimeRangeFlag( strStartTime != "00:00" || strEndTime != "23:59" );
		}

		m_pReportInfo -> SetSummaryFlag( m_checkSummary.GetCheck() != 0 );

		{
			CString strDateRangeType;
			m_comboDateRange.GetWindowText( strDateRangeType );
	
			CCSV csv;
			csv.Add ( strDateRangeType );
			csv.Add ( m_comboTimeRange.GetCurSel() );
			csv.Add ( m_pReportInfo -> GetSummaryFlag() );
			csv.Add ( m_pReportInfo -> GetDateFromString() );
			csv.Add ( m_pReportInfo -> GetDateToString() );
			csv.Add ( m_pReportInfo -> GetTimeFromString() );
			csv.Add ( m_pReportInfo -> GetTimeToString() );

			CReportDefaultHandler repdefs;
			repdefs.Save ( m_strReportLabel, &csv );

			CCSV csvReportKey;
			csvReportKey.Add ( m_pReportInfo -> GetSummaryFlag() );
			m_pReportInfo -> SetReportKey( csvReportKey.GetLine() );
		}
	}

	EndDialog( IDOK );
}

//$$******************************************************************
