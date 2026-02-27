//$$******************************************************************
#include "ReportDefaultHandler.h"
#include "StandardCombos.h"
//$$******************************************************************
#include "ReportGroupPeriodDlg.h"
//$$******************************************************************

CReportGroupPeriodDlg::CReportGroupPeriodDlg(const char* szLabel, CWnd* pParent)
	: CSSAutoShutdownDialog(AutoShutdownHelper, CReportGroupPeriodDlg::IDD, pParent)
	, m_DateSelectorRange(m_comboDateRange, m_DatePickerFrom, m_DatePickerTo)
	, m_TerminalListHandler(Filenames.GetTerminalListFilename())
	, m_ReportLocationControls(m_comboFolderSet, m_checkTerminals, m_comboTerminals, &m_TerminalListHandler)
{
	m_strReportLabel = szLabel;
	m_strReportKey = "";

	m_dateTo.SetCurrentDate();
	m_dateFrom.SetCurrentDate();
	m_dateAllFrom.SetCurrentDate();
	m_bContinuous = FALSE;
}

//$$******************************************************************

CReportGroupPeriodDlg::~CReportGroupPeriodDlg()
{
}

//$$******************************************************************

void CReportGroupPeriodDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_DATERANGE, m_comboDateRange);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_DatePickerFrom);
	DDX_Control(pDX, IDC_DATETIMEPICKER2, m_DatePickerTo);
	DDX_Control(pDX, IDC_DATETIMEPICKER3, m_TimePickerStart);
	DDX_Control(pDX, IDC_DATETIMEPICKER4, m_TimePickerEnd);
	DDX_Control(pDX, IDC_CHECK_CONTINUOUS, m_checkContinuous);
	DDX_Control(pDX, IDC_STATIC_FOLDERSET, m_staticFolderSet);
	DDX_Control(pDX, IDC_COMBO_FOLDERSET, m_comboFolderSet);
	DDX_Control(pDX, IDC_CHECK_TERMINALS, m_checkTerminals);
	DDX_Control(pDX, IDC_COMBO_TERMINALS, m_comboTerminals);
	DDX_Control(pDX, IDC_RADIO_GROUPNO, m_radioGroupNo);
	DDX_Control(pDX, IDC_RADIO_GROUPSETNO, m_radioGroupSetNo);
	DDX_Control(pDX, IDC_CHECK_SUMMARY, m_checkSummary);
	DDX_Control(pDX, IDC_BUTTON_ALTKEY1, m_buttonAltKey1);
}

//*******************************************************

BEGIN_MESSAGE_MAP(CReportGroupPeriodDlg, CSSAutoShutdownDialog)
	ON_CBN_SELCHANGE(IDC_COMBO_DATERANGE, OnSelectDateRange)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER1, OnChangeDateFrom)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER2, OnChangeDateTo)
	ON_CBN_SELCHANGE(IDC_COMBO_FOLDERSET, OnSelectFolderSet)
	ON_BN_CLICKED(IDC_CHECK_TERMINALS, OnToggleTerminals)
	ON_BN_CLICKED(IDC_BUTTON_ALTKEY1, OnButtonAltKey1)
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()

//$$******************************************************************

BOOL CReportGroupPeriodDlg::CSSAutoShutdownPostInitDialog()
{
	CString strDateRangeType = szRANGE_TODAY;

	CSSDate dateNow;
	dateNow.SetCurrentDate();
	COleDateTime oleDateNow(dateNow.GetYear(), dateNow.GetMonth(), dateNow.GetDay(), 0, 0, 0);
	m_DatePickerFrom.SetTime(oleDateNow);
	m_DatePickerTo.SetTime(oleDateNow);
	CString strDateFrom = dateNow.GetDate();
	CString strDateTo = dateNow.GetDate();
	bool bContinuous = TRUE;

	COleDateTime oleTimeStart(2020, 1, 1, 0, 0, 0);
	COleDateTime oleTimeEnd(2020, 1, 1, 23, 59, 0);
	m_TimePickerStart.SetTime(oleTimeStart);
	m_TimePickerEnd.SetTime(oleTimeEnd);
	CString strTimeStart = "00:00";
	CString strTimeEnd = "23:59";
	
	int nFolderSet = 0;
	int nTerminalListType = 0;
	bool bSummary = FALSE;
	int nReportBy = 0;
	
	CString strLine;
	CReportDefaultHandler repdefs;
	if (repdefs.Read(m_strReportLabel, strLine) == TRUE)
	{
		CCSV csv(strLine);
		//0 WAS DATE CURSEL
		if (csv.IsEmpty(1) == FALSE)		bSummary = csv.GetBool(1);
		if (csv.IsEmpty(2) == FALSE)		nReportBy = csv.GetBool(2);
		if (csv.IsEmpty(3) == FALSE)		strTimeStart = (csv.GetString(3));
		if (csv.IsEmpty(4) == FALSE)		strTimeEnd = (csv.GetString(4));
		if (csv.IsEmpty(5) == FALSE)		nTerminalListType = csv.GetInt(5);
		if (csv.IsEmpty(6) == FALSE)		strDateRangeType = csv.GetString(6);
		if (csv.IsEmpty(7) == FALSE)		strDateFrom = csv.GetString(7);
		if (csv.IsEmpty(8) == FALSE)		strDateTo = csv.GetString(8);
		if (csv.IsEmpty(9) == FALSE)		bContinuous = csv.GetBool(9);
		if (csv.IsEmpty(10) == FALSE)		nFolderSet = csv.GetInt(10);
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

// set start & end time 

	m_TimePickerStart.SetFormat("HH:mm");
	m_TimePickerEnd.SetFormat("HH:mm");

	{
		CSSTime timeFrom(strTimeStart);
		CSSTime timeTo(strTimeEnd);

		if (timeFrom.IsSet() && timeTo.IsSet())
		{
			m_TimePickerStart.SetTime(COleDateTime(2020, 1, 1, timeFrom.GetHour(), timeFrom.GetMinute(), 0));
			m_TimePickerEnd.SetTime(COleDateTime(2020, 1, 1, timeTo.GetHour(), timeTo.GetMinute(), 0));
		}
		else
		{
			m_TimePickerStart.SetTime(COleDateTime(2020, 1, 1, 0, 0, 0));
			m_TimePickerEnd.SetTime(COleDateTime(2020, 1, 1, 23, 59, 0));
		}
	}

	m_checkContinuous.SetCheck(bContinuous);

	// terminal list
	m_ReportLocationControls.Init(nFolderSet, nTerminalListType);

	if ((nReportBy < 0) || (nReportBy > 1))
	{
		nReportBy = 0;
	}
	m_radioGroupNo.SetCheck(0 == nReportBy);
	m_radioGroupSetNo.SetCheck(1 == nReportBy);
	
	m_checkSummary.SetCheck(bSummary);
	m_buttonAltKey1.ShowWindow(SW_HIDE);

	return TRUE;
}

//$$******************************************************************

void CReportGroupPeriodDlg::OnSelectDateRange()
{
	m_DateSelectorRange.UpdateDateControls();

	CString strRange;
	m_comboDateRange.GetWindowText(strRange);
	if (strRange == szRANGE_ALL)
	{
		COleDateTime date = COleDateTime(m_dateAllFrom.GetYear(), m_dateAllFrom.GetMonth(), m_dateAllFrom.GetDay(), 0, 0, 0);
		m_DatePickerFrom.SetTime(date);
	}
}

//$$******************************************************************

void CReportGroupPeriodDlg::OnChangeDateFrom(NMHDR* pNMHDR, LRESULT* pResult)
{
	m_DateSelectorRange.OnChangeFromDate();
	m_DateSelectorRange.CheckForChangedDate();
	*pResult = 0;
}

//$$******************************************************************

void CReportGroupPeriodDlg::OnChangeDateTo(NMHDR* pNMHDR, LRESULT* pResult)
{
	m_DateSelectorRange.CheckForChangedDate();
	*pResult = 0;
}

//$$******************************************************************

void CReportGroupPeriodDlg::OnSelectFolderSet()
{
	m_ReportLocationControls.SelectFolderSet();
}

//*******************************************************************

void CReportGroupPeriodDlg::OnToggleTerminals()
{
	m_ReportLocationControls.ToggleTerminals();
}

//$$******************************************************************

void CReportGroupPeriodDlg::OnButtonAltKey1()
{
	m_ReportLocationControls.DeleteSelectedTerminalList();
}

//$$******************************************************************

CString CReportGroupPeriodDlg::GetDateString()
{
	CString strDateString = "";

	if (m_dateFrom != m_dateTo)
	{
		strDateString.Format(" (%s - %s) ", m_dateFrom.GetDate(), m_dateTo.GetDate());
	}
	else
	{
		strDateString.Format(" (%s) ", m_dateFrom.GetDate());
	}

	return strDateString;
}

//$$******************************************************************

bool CReportGroupPeriodDlg::IsValidDate(CSQLAuditRecord* atcRecord)
{
	CSSDate dateAudit(atcRecord->GetDateFiled());
	CSSTime timeAudit(atcRecord->GetTimeFiled());

	if (dateAudit < m_dateFrom)							// not in range
		return FALSE;

	if (dateAudit > m_dateTo)								// not in range
		return FALSE;										// skip

	if (FALSE == m_bContinuous)
	{
		if (timeAudit < m_timeStart)
			return FALSE;

		if (timeAudit > m_timeEnd)
			return FALSE;
	}
	else
	{
		if (dateAudit == m_dateFrom)
		{
			if (timeAudit < m_timeStart)
				return FALSE;
		}

		if (dateAudit == m_dateTo)
		{
			if (timeAudit > m_timeEnd)
				return FALSE;
		}
	}

	return TRUE;
}

//$$******************************************************************

bool CReportGroupPeriodDlg::IsValidTerminalNo(int nFolderSet, int nTNo)
{
	return (m_TerminalListHandler.CheckTerminalFilter(nFolderSet, nTNo));
}

//$$******************************************************************

void CReportGroupPeriodDlg::OnOK()
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

	m_TerminalListHandler.SetTargetFolderSet(nFolderSet);
	m_TerminalListHandler.SetTerminalSelectType(bWantTerminals ? 1 : 0);

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
	m_bContinuous = IsTicked(m_checkContinuous);
	
	m_bSummaryReqd = IsTicked(m_checkSummary);
	m_nReportBy = IsTicked(m_radioGroupSetNo) ? 1 : 0;
	
	CString strDateRangeType;
	m_comboDateRange.GetWindowText(strDateRangeType);

	CCSV csv;
	csv.Add(0); //WAS DATE SELECTION
	csv.Add(m_bSummaryReqd);
	csv.Add(m_nReportBy);
	csv.Add(strStartTime);
	csv.Add(strEndTime);
	csv.Add(bWantTerminals ? 1 : 0);
	csv.Add(strDateRangeType);
	csv.Add(m_dateFrom.GetDate());
	csv.Add(m_dateTo.GetDate());
	csv.Add(m_bContinuous);
	csv.Add(nFolderSet);

	CReportDefaultHandler repdefs;
	repdefs.Save(m_strReportLabel, &csv);

	CCSV csvReportKey;
	csvReportKey.Add(m_bSummaryReqd);
	csvReportKey.Add(m_nReportBy);
	csvReportKey.Add(strStartTime);
	csvReportKey.Add(strEndTime);
	csvReportKey.Add(bWantTerminals ? 1 : 0);
	m_strReportKey = csvReportKey.GetLine();

	if (TRUE == bWantTerminals)
	{
		m_TerminalListHandler.TNoListSetMaxLength(strTerminalList.GetLength());	// stop entry being truncated
		m_TerminalListHandler.TNoListSave(strTerminalList);						// save terminal list

		m_TerminalListHandler.TNoArrayRemoveAll();
		m_TerminalListHandler.TNoListExtractTNos(strTerminalList);			// extract terminal numbers
	}

	EndDialog(IDOK);
}

//$$******************************************************************
