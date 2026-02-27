//*******************************************************************
#include "..\SmartPay4Shared\SQLTable_AuditPeriod\SQLRowSetAuditPeriod.h"
#include "..\SmartPay4Shared\SQLTable_UserText\SQLRepositoryUserText.h"
//*******************************************************************
#include "SmartPay4Manager.h"
#include "ReportDefaultHandler.h"
#include "AccumulatorPropertySheet.h"
#include "AccumulatorPropertyPage1.h"
//*******************************************************************

CAccumulatorPropertyPage1::CAccumulatorPropertyPage1()
	: CSSAutoShutdownPropertyPage(AutoShutdownHelper, CAccumulatorPropertyPage1::IDD)
	, m_DateSelectorRange(m_comboDateRange, m_DatePickerFrom, m_DatePickerTo)
	, m_ReportLocationControls(m_comboFolderSet, m_checkTerminals, m_comboTerminals, NULL)
{
	m_pReportInfo = NULL;
}

//*******************************************************************

CAccumulatorPropertyPage1::~CAccumulatorPropertyPage1()
{
}

//*******************************************************************

void CAccumulatorPropertyPage1::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	
	//criteria
	DDX_Control(pDX, IDC_STATIC_TYPE, m_staticType);
	DDX_Control(pDX, IDC_STATIC_TOP100, m_staticTop100);
	DDX_Control(pDX, IDC_CHECK_NOZEROSKIP, m_checkNoZeroSkip);
	DDX_Control(pDX, IDC_STATIC_REPORTON, m_staticReportOn);
	DDX_Control(pDX, IDC_COMBO_RANKBY, m_comboRankBy);
	DDX_Control(pDX, IDC_STATIC_INFO, m_staticInfo);
	DDX_Control(pDX, IDC_COMBO_INFO, m_comboInfo);
	//date
	DDX_Control(pDX, IDC_STATIC_DATE, m_staticDate);
	DDX_Control(pDX, IDC_COMBO_DATERANGE, m_comboDateRange);
	DDX_Control(pDX, IDC_STATIC_DATEFROM, m_staticDateFrom);
	DDX_Control(pDX, IDC_STATIC_DATETO, m_staticDateTo);
	DDX_Control(pDX, IDC_DATEPICKER_FROM, m_DatePickerFrom);
	DDX_Control(pDX, IDC_DATEPICKER_TO, m_DatePickerTo);
	//time
	DDX_Control(pDX, IDC_COMBO_TIMERANGE, m_comboTimeRange);
	DDX_Control(pDX, IDC_STATIC_TIMESTART, m_staticTimeStart);
	DDX_Control(pDX, IDC_STATIC_TIMEEND, m_staticTimeEnd);
	DDX_Control(pDX, IDC_TIMEPICKER_FROM, m_TimePickerFrom);
	DDX_Control(pDX, IDC_TIMEPICKER_TO, m_TimePickerTo);
	DDX_Control(pDX, IDC_CHECK_CONTINUOUS, m_checkContinuous);
	//terminals
	DDX_Control(pDX, IDC_STATIC_EPOS, m_staticEPOS);
	DDX_Control(pDX, IDC_STATIC_FOLDERSET, m_staticFolderSet);
	DDX_Control(pDX, IDC_COMBO_FOLDERSET, m_comboFolderSet);
	DDX_Control(pDX, IDC_CHECK_TERMINALS, m_checkTerminals);
	DDX_Control(pDX, IDC_COMBO_TERMINALS, m_comboTerminals);
	//altkey
	DDX_Control(pDX, IDC_BUTTON_ALTKEY1, m_buttonAltKey1);
}

//*******************************************************************

BEGIN_MESSAGE_MAP(CAccumulatorPropertyPage1, CSSAutoShutdownPropertyPage)
	ON_CBN_SELCHANGE(IDC_COMBO_DATERANGE, OnSelectDateRange)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATEPICKER_FROM, OnChangeDateFrom)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATEPICKER_TO, OnChangeDateTo)
	ON_CBN_SELCHANGE(IDC_COMBO_TIMERANGE, OnSelectTimeRange)
	ON_CBN_SELCHANGE(IDC_COMBO_FOLDERSET, OnSelectFolderSet)
	ON_BN_CLICKED(IDC_CHECK_TERMINALS, OnToggleTerminals)
	ON_BN_CLICKED(IDC_BUTTON_ALTKEY1, OnButtonAltKey1)
END_MESSAGE_MAP()

//*******************************************************************

BOOL CAccumulatorPropertyPage1::OnInitDialog()
{
	m_ReportLocationControls.SetTerminalListHandler(m_pReportInfo->GetTerminalListHandler());

	int nTimeCursel = 0;
	CString strDateType = "";
	CString strTimeFrom = "";
	CString strTimeTo = "";
	CString strDateFrom = "";
	CString strDateTo = "";

	int nInfoSort = m_pReportInfo->GetInfoSortType();
	int nRankBy = m_pReportInfo->GetRankByType();
	int nTop100 = m_pReportInfo->GetTop100ChartSize();
	double dThreshold = m_pReportInfo->GetThreshold();
	bool bNoZeroSkip = m_pReportInfo->GetNoZeroSkipFlag();
	bool bContinuous = m_pReportInfo->GetContinuousFlag();
	int nFolderSet = m_pReportInfo->GetFolderSet();
	int nTerminalListType = m_pReportInfo->GetTerminalListType();
	
	CString strLine = "";
	CReportDefaultHandler repdefs;
	if (repdefs.Read(m_pReportInfo -> GetReportLabel(), strLine) == TRUE)
	{
		CCSV csv(strLine);
		//0 was old date cursel
		if (csv.IsEmpty(1) == FALSE)	nTimeCursel = csv.GetInt(1);
		if (csv.IsEmpty(2) == FALSE)	dThreshold = csv.GetDouble(2);
		if (csv.IsEmpty(3) == FALSE)	bNoZeroSkip = csv.GetBool(3);
		if (csv.IsEmpty(4) == FALSE)	nRankBy = csv.GetInt(4);
		if (csv.IsEmpty(5) == FALSE)	nTop100 = csv.GetInt(5);
		if (csv.IsEmpty(6) == FALSE)	nTerminalListType = csv.GetInt(6);
		if (csv.IsEmpty(7) == FALSE)	bContinuous = csv.GetBool(7);
		if (csv.IsEmpty(8) == FALSE)	nInfoSort = csv.GetInt(8);
		if (csv.IsEmpty(9) == FALSE)	nFolderSet = csv.GetInt(9);
		if (csv.IsEmpty(10) == FALSE)	strDateType = csv.GetString(10);
		if (csv.IsEmpty(11) == FALSE)	strDateFrom = csv.GetString(11);
		if (csv.IsEmpty(12) == FALSE)	strDateTo = csv.GetString(12);
		if (csv.IsEmpty(13) == FALSE)	strTimeFrom = csv.GetString(13);
		if (csv.IsEmpty(14) == FALSE)	strTimeTo = csv.GetString(14);
	}

	if ((nInfoSort < 0) || (nInfoSort > nMAX_INFOTEXTS))
	{
		nInfoSort = 0;
	}

	//m_nReportType = nACCUMULATE_INACTIVE;

	CString strThresholdTitle = "";

	switch (m_pReportInfo->GetReportType())
	{
	case nACCUMULATE_LOWPOINTS:
		strThresholdTitle = "Points less than";		// cards < threshold
		break;

	case nACCUMULATE_LOWSPEND:
		strThresholdTitle = "Spend less than";		// cards < threshold
		break;

	case nACCUMULATE_RANKPOINTS:
		strThresholdTitle = "Rank points above";	// cards > threshold
		break;

	case nACCUMULATE_RANKSPEND:
		strThresholdTitle = "Rank spend above";		// cards > threshold
		break;

	case nACCUMULATE_RANKPURCHASES:
		strThresholdTitle = "Rank by";				// Rank By ( Quantity / Value )
		bNoZeroSkip = FALSE;						// threshold not used - checks specfically for > 0
		break;

	case nACCUMULATE_ACTIVE:
		bNoZeroSkip = FALSE;						// threshold not used - checks specfically for > 0
		break;

	case nACCUMULATE_INACTIVE:
		bNoZeroSkip = TRUE;						// cards < 1 & zero
		break;
	}

	CSSPropertyPage::OnInitDialog();

	m_staticReportOn.SetWindowText(strThresholdTitle);
	bool bShowNoZeroSkip = FALSE;
	bool bShowEditThreshold = TRUE;
	bool bShowRankBy = FALSE;
	bool bShowTop100 = FALSE;
	bool bShowInfo = FALSE;

	switch (m_pReportInfo->GetReportType())
	{
	case nACCUMULATE_LOWPOINTS:									// low points
		SubclassEdit(IDC_EDIT_THRESHOLD, SS_NUM_S, 8, "%.0f");
		bShowNoZeroSkip = TRUE;
		break;

	case nACCUMULATE_LOWSPEND:									// low spend
		SubclassEdit(IDC_EDIT_THRESHOLD, SS_NUM_SDP, 11, "%.*f", System.GetDPValue());
		bShowNoZeroSkip = TRUE;
		break;

	case nACCUMULATE_RANKPOINTS:								// ranked points
		SubclassEdit(IDC_EDIT_THRESHOLD, SS_NUM_S, 8, "%.0f");
		bShowTop100 = TRUE;
		break;

	case nACCUMULATE_RANKSPEND:									// ranked spend
		SubclassEdit(IDC_EDIT_THRESHOLD, SS_NUM_SDP, 11, "%.*f", System.GetDPValue());
		bShowTop100 = TRUE;
		break;

	case nACCUMULATE_ACTIVE:
	case nACCUMULATE_INACTIVE:
		bShowEditThreshold = FALSE;
		break;

	case nACCUMULATE_RANKPURCHASES:
		bShowEditThreshold = FALSE;
		bShowRankBy = TRUE;
		bShowTop100 = TRUE;
		bShowInfo = TRUE;
		break;
	}

	ShowAndEnableWindow(GetEditThreshold(), bShowEditThreshold);
	ShowAndEnableWindow(&m_checkNoZeroSkip, bShowNoZeroSkip);
	ShowAndEnableWindow(&m_staticTop100, bShowTop100);
	ShowAndEnableWindow(GetEditTop100(), bShowTop100);
	ShowAndEnableWindow(&m_comboRankBy, bShowRankBy);
	ShowAndEnableWindow(&m_staticInfo, bShowInfo);
	ShowAndEnableWindow(&m_comboInfo, bShowInfo);

	{
		m_comboRankBy.AddString("Quantity");
		m_comboRankBy.AddString("Value");

		if (System.GetActiveEnablePointsFlag() == TRUE)
		{
			m_comboRankBy.AddString("Points");
		}

		if ((nRankBy < 0) || (nRankBy >= m_comboRankBy.GetCount()))
		{
			nRankBy = nRANKBY_VALUE;
		}

		m_comboRankBy.SetCurSel(nRankBy);
	}

	{
		m_comboInfo.AddString("None");

		for (int n = 1; n <= nMAX_INFOTEXTS; n++)
		{
			CSQLRepositoryUserText RepoText;
			m_comboInfo.AddString(RepoText.GetDisplayInfoText(n));
		}

		m_comboInfo.SetCurSel(nInfoSort);
	}

	SetEditBoxInt(*GetEditTop100(), nTop100);
	SetEditBoxDouble(*GetEditThreshold(), dThreshold, System.GetDPValue());
	m_checkNoZeroSkip.SetCheck(bNoZeroSkip);
	m_checkContinuous.SetCheck(bContinuous);

	if ((FALSE == bShowTop100) && (FALSE == bShowRankBy) && (FALSE == bShowNoZeroSkip) && (FALSE == bShowEditThreshold) && (FALSE == bShowInfo))
	{
		ShowAndEnableWindow(&m_staticType, FALSE);
		ShowAndEnableWindow(&m_staticTop100, FALSE);
		ShowAndEnableWindow(GetEditTop100(), FALSE);
		ShowAndEnableWindow(GetEditThreshold(), FALSE);
		ShowAndEnableWindow(&m_comboRankBy, FALSE);
		ShowAndEnableWindow(&m_checkNoZeroSkip, FALSE);
		ShowAndEnableWindow(&m_staticReportOn, FALSE);
		MoveControl(&m_staticDate, 7, 7, 208, 261);
		MoveControl(&m_comboDateRange, 24, 31);
		MoveControl(&m_staticDateFrom, 24, 53);
		MoveControl(&m_DatePickerFrom, 51, 51);
		MoveControl(&m_staticDateTo, 24, 73);
		MoveControl(&m_DatePickerTo, 51, 71);
		MoveControl(&m_comboTimeRange, 24, 109);
		MoveControl(&m_staticTimeStart, 24, 131);
		MoveControl(&m_TimePickerFrom, 71, 129);
		MoveControl(&m_staticTimeEnd, 24, 151);
		MoveControl(&m_TimePickerTo, 71, 149);
		MoveControl(&m_checkContinuous, 24, 180);
		MoveControl(&m_staticEPOS, 223, 7, 224, 261);
		MoveControl(&m_staticFolderSet, 240, 33);
		MoveControl(&m_comboFolderSet, 298, 31);
		MoveControl(&m_checkTerminals, 240, 53);
		MoveControl(&m_comboTerminals, 298, 51);
	}

	// terminal list
	m_ReportLocationControls.Init(nFolderSet, nTerminalListType);

	// Date range
	m_DateSelectorRange.InitialiseDateControls();
	m_comboDateRange.AddString(szRANGE_ALL);
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
	m_DateSelectorRange.SetComboSelection(strDateType);
	
	if ((strDateType == szRANGE_CUSTOM_DATE) || (strDateType == szRANGE_CUSTOM_RANGE))
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
	m_comboTimeRange.SetCurSel(nTimeCursel);

	if (m_comboTimeRange.GetCount() < 2)
	{
		m_comboTimeRange.EnableWindow(FALSE);
	}

	m_TimePickerFrom.SetFormat("HH:mm");
	m_TimePickerTo.SetFormat("HH:mm");

	{
		CSSTime timeFrom(strTimeFrom);
		CSSTime timeTo(strTimeTo);

		if (timeFrom.IsSet())
		{
			m_TimePickerFrom.SetTime(COleDateTime(2019, 1, 1, timeFrom.GetHour(), timeFrom.GetMinute(), 0));
		}
		else
		{
			m_TimePickerFrom.SetTime(COleDateTime(2019, 1, 1, 0, 0, 0));
		}

		if (timeTo.IsSet())
		{
			m_TimePickerTo.SetTime(COleDateTime(2019, 1, 1, timeTo.GetHour(), timeTo.GetMinute(), 0));
		}
		else
		{
			m_TimePickerTo.SetTime(COleDateTime(2019, 1, 1, 23, 59, 0));
		}
	}

	m_buttonAltKey1.ShowWindow(SW_HIDE);

	return TRUE;
}

//*****************************************************************

CEdit* CAccumulatorPropertyPage1::GetEditThreshold()
{ 
	return GetEdit(IDC_EDIT_THRESHOLD);
}

//*****************************************************************

CEdit* CAccumulatorPropertyPage1::GetEditTop100()
{ 
	return GetEdit(IDC_EDIT_TOP100);
}

//*****************************************************************

void CAccumulatorPropertyPage1::OnSelectFolderSet()
{
	m_ReportLocationControls.SelectFolderSet();
}

//*******************************************************************

void CAccumulatorPropertyPage1::OnToggleTerminals()
{
	m_ReportLocationControls.ToggleTerminals();
}

//$$******************************************************************

void CAccumulatorPropertyPage1::OnButtonAltKey1()
{
	m_ReportLocationControls.DeleteSelectedTerminalList();
}

//$$******************************************************************

void CAccumulatorPropertyPage1::OnSelectDateRange()
{
	m_DateSelectorRange.UpdateDateControls();
}

//**********************************************************************

void CAccumulatorPropertyPage1::OnChangeDateFrom(NMHDR* pNMHDR, LRESULT* pResult)
{
	m_DateSelectorRange.OnChangeFromDate();
	m_DateSelectorRange.CheckForChangedDate();
	*pResult = 0;
}

/**********************************************************************/

void CAccumulatorPropertyPage1::OnChangeDateTo(NMHDR* pNMHDR, LRESULT* pResult)
{
	m_DateSelectorRange.CheckForChangedDate();
	*pResult = 0;
}

/**********************************************************************/

void CAccumulatorPropertyPage1::OnSelectTimeRange()
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

//**********************************************************************

BOOL CAccumulatorPropertyPage1::OnKillActive()
{
	if (UpdateData() == FALSE)
	{
		return FALSE;
	}

	CString strTerminalList = "";
	int nFolderSet = 0;
	bool bWantTerminals = FALSE;
	m_ReportLocationControls.GetSettings(nFolderSet, bWantTerminals, strTerminalList);

	if ((TRUE == bWantTerminals) && (strTerminalList == ""))
	{
		Prompter.FatalError("No EPOS terminal numbers have been specified!", "Entry Error");
		m_comboTerminals.SetFocus();
		return FALSE;
	}

	{
		CTime dateFrom;
		m_DatePickerFrom.GetTime(dateFrom);
		m_pReportInfo->SetDateFrom(dateFrom);
	}

	{
		CTime dateTo;
		m_DatePickerTo.GetTime(dateTo);
		m_pReportInfo->SetDateTo(dateTo);
	}

	{
		CTime timeFrom;
		m_TimePickerFrom.GetTime(timeFrom);
		CString strStartTime = timeFrom.Format("%H:%M");
		m_pReportInfo->SetTimeFrom(timeFrom);

		CTime timeTo;
		m_TimePickerTo.GetTime(timeTo);
		CString strEndTime = timeTo.Format("%H:%M");
		m_pReportInfo->SetTimeTo(timeTo);

		m_pReportInfo->SetTimeRangeFlag(strStartTime != "00:00" || strEndTime != "23:59");
	}

	m_pReportInfo->SetRankByType(m_comboRankBy.GetCurSel());
	m_pReportInfo->SetInfoSortType(m_comboInfo.GetCurSel());
	m_pReportInfo->SetTop100ChartSize(GetEditBoxInt(*GetEditTop100()));
	m_pReportInfo->SetThreshold(GetEditBoxDouble(*GetEditThreshold()));
	m_pReportInfo->SetNoZeroSkipFlag(IsTicked(m_checkNoZeroSkip));
	m_pReportInfo->SetContinuousFlag(IsTicked(m_checkContinuous));
	m_pReportInfo->SetFolderSet(nFolderSet);
	m_pReportInfo->SetTerminalListType(bWantTerminals ? 1 : 0);

	if (m_pReportInfo->GetDateFrom() != m_pReportInfo->GetDateTo())
	{
		m_pReportInfo->m_strDateString.Format(" (%s - %s)",
			m_pReportInfo->GetDateFrom().GetDate(),
			m_pReportInfo->GetDateTo().GetDate());
	}
	else
	{
		m_pReportInfo->m_strDateString.Format(" (%s)",
			m_pReportInfo->GetDateFrom().GetDate());			/* start date */
	}

	if (m_pReportInfo->GetTimeRangeFlag() == TRUE)
	{
		m_pReportInfo->m_strTimeString.Format(" (%s - %s)",
			(const char*)m_pReportInfo->GetTimeFromString(),
			(const char*)m_pReportInfo->GetTimeToString());
	}
	else
	{
		m_pReportInfo->m_strTimeString = "";
	}

	CString strDateRangeType;
	m_comboDateRange.GetWindowText(strDateRangeType);

	CCSV csv;
	csv.Add(0); //0 WAS OLD DATE CUR SEL
	csv.Add(m_comboTimeRange.GetCurSel());
	csv.Add(m_pReportInfo->GetThreshold(), 2);
	csv.Add(m_pReportInfo->GetNoZeroSkipFlag());
	csv.Add(m_pReportInfo->GetRankByType());
	csv.Add(m_pReportInfo->GetTop100ChartSize());
	csv.Add(m_pReportInfo->GetTerminalListType());
	csv.Add(m_pReportInfo->GetContinuousFlag());
	csv.Add(m_pReportInfo->GetInfoSortType());
	csv.Add(m_pReportInfo->GetFolderSet());
	csv.Add(strDateRangeType);
	csv.Add(m_pReportInfo->GetDateFromString());
	csv.Add(m_pReportInfo->GetDateToString());
	csv.Add(m_pReportInfo->GetTimeFromString());
	csv.Add(m_pReportInfo->GetTimeToString());

	CReportDefaultHandler repdefs;
	repdefs.Save(m_pReportInfo -> GetReportLabel(), & csv);

	if (m_pReportInfo->GetTerminalListType() == 1)
	{
		m_pReportInfo->TNoListSetMaxLength(strTerminalList.GetLength());	// stop entry being truncated
		m_pReportInfo->TNoListSave(strTerminalList);						// save terminal list

		m_pReportInfo->TNoArrayRemoveAll();
		m_pReportInfo->TNoListExtractTNos(strTerminalList);			// extract terminal numbers
	}

	return CPropertyPage::OnKillActive();
}

//********************************************************************

