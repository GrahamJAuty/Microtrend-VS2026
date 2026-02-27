//*******************************************************
#include "resource.h"
//*******************************************************
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\SQLTable_AuditPeriod\SQLRowSetAuditPeriod.h"
#include "..\SmartPay4Shared\SQLTable_Group\SQLRepositoryGroup.h"
//*******************************************************
#include "CardPickerDlg.h"
#include "FindDlg.h"
#include "GroupComboHelpers.h"
#include "ReportDefaultHandler.h"
#include "StandardCombos.h"
//*******************************************************
#include "ReportWeeklySpendDlg.h"
//*******************************************************

CReportWeeklySpendDlg::CReportWeeklySpendDlg( const char* szLabel, CWnd* pParent /*=NULL*/)
	: CSSAutoShutdownDialog(AutoShutdownHelper, CReportWeeklySpendDlg::IDD, pParent)
	, m_DateSelectorRange(m_comboDateRange, m_DatePickerFrom, m_DatePickerTo)
	, m_TerminalListHandler(Filenames.GetTerminalListFilename())
	, m_ReportLocationControls(m_comboFolderSet, m_checkTerminals, m_comboTerminals, &m_TerminalListHandler)
{
	m_strReportLabel = szLabel;

	m_strTitle			= "Weekly Spend Report";
	m_strReportList		= Filenames.GetReportCardListFilename ( m_strReportLabel );
	m_strReportName		= Filenames.GetReportFilename ( m_strReportLabel );
	m_strParamsName		= Filenames.GetReportParamsFilename ( m_strReportLabel );
	m_strReportKey		= "";

	m_dateFrom.SetCurrentDate();					// default to today
	m_dateTo.SetCurrentDate();						// could be set before DoModal call
	m_bTimeRangeSet	= FALSE;
	m_bContinuous = FALSE;
}

//*******************************************************

CReportWeeklySpendDlg::~CReportWeeklySpendDlg()
{
}

//*******************************************************

void CReportWeeklySpendDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_DatePickerFrom);
	DDX_Control(pDX, IDC_DATETIMEPICKER2, m_DatePickerTo);
	DDX_Control(pDX, IDC_DATETIMEPICKER3, m_TimePickerStart);
	DDX_Control(pDX, IDC_DATETIMEPICKER4, m_TimePickerEnd);
	DDX_Control(pDX, IDC_COMBO_DATERANGE, m_comboDateRange);
	DDX_Control(pDX, IDC_COMBO_TIMERANGE, m_comboTimeRange);
	DDX_Control(pDX, IDC_CHECK_CONTINUOUS, m_checkContinuous);
	DDX_Control(pDX, IDC_EDIT_FROMCARDNO, m_editCardNoFrom);
	DDX_Control(pDX, IDC_EDIT_TOCARDNO, m_editCardNoTo);
	DDX_Control(pDX, IDC_RADIO_RANGE, m_radioRange);
	DDX_Control(pDX, IDC_RADIO_SINGLE, m_radioSingle);
	DDX_Control(pDX, IDC_RADIO_LIST, m_radioList);
	DDX_Control(pDX, IDC_BUTTON_LIST, m_buttonList);
	DDX_Control(pDX, IDC_COMBO_GROUP, m_tabcomboGroup);
	DDX_Control(pDX, IDC_STATIC_FOLDERSET, m_staticFolderSet);
	DDX_Control(pDX, IDC_COMBO_FOLDERSET, m_comboFolderSet);
	DDX_Control(pDX, IDC_CHECK_TERMINALS, m_checkTerminals);
	DDX_Control(pDX, IDC_COMBO_TERMINALS, m_comboTerminals);
	DDX_Control(pDX, IDC_CHECK_PURSE1SPEND, m_checkIncludePurse1);
	DDX_Control(pDX, IDC_CHECK_PURSE2SPEND, m_checkIncludePurse2);
	DDX_Control(pDX, IDC_CHECK_PURSE3SPEND, m_checkIncludePurse3);
	DDX_Control(pDX, IDC_CHECK_CASHSPEND, m_checkIncludeCash);
	DDX_Control(pDX, IDC_CHECK_SHOWREMOVED, m_checkShowDeletedLines);
	DDX_Control(pDX, IDC_CHECK_SHOWUNUSED, m_checkShowUnusedLines);
	DDX_Control(pDX, IDC_COMBO_SORTON, m_comboSortOn);
	DDX_Control(pDX, IDC_CHECK_MON_FRI, m_checkOnlyShowMonFri);
	DDX_Control(pDX, IDC_BUTTON_ALTKEY1, m_buttonAltKey1);
}

//*******************************************************

BEGIN_MESSAGE_MAP(CReportWeeklySpendDlg, CSSAutoShutdownDialog)
	ON_CBN_SELCHANGE(IDC_COMBO_DATERANGE, OnSelectDateRange)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER1, OnChangeDateFrom)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER2, OnChangeDateTo)
	ON_BN_CLICKED(IDC_RADIO_RANGE, OnRadioRange)
	ON_BN_CLICKED(IDC_RADIO_SINGLE, OnRadioSingle)
	ON_BN_CLICKED(IDC_RADIO_LIST, OnRadioList)
	ON_BN_CLICKED(IDC_BUTTON_LIST, OnButtonList)
	ON_BN_DOUBLECLICKED(IDC_RADIO_LIST, OnDoubleClickRadioList)
	ON_BN_DOUBLECLICKED(IDC_RADIO_SINGLE, OnDoubleClickRadioSingle)
	ON_CBN_SELCHANGE(IDC_COMBO_TIMERANGE, OnSelectTimeRange)
	ON_CBN_SELCHANGE(IDC_COMBO_FOLDERSET, OnSelectFolderSet)
	ON_BN_CLICKED(IDC_CHECK_TERMINALS, OnToggleTerminals)
	ON_BN_CLICKED(IDC_BUTTON_ALTKEY1, OnButtonAltKey1)
END_MESSAGE_MAP()

//*******************************************************

BOOL CReportWeeklySpendDlg::CSSAutoShutdownPostInitDialog()
{
	SetWindowText(m_strTitle);

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
	bool bContinuous = FALSE;

	int nTimeCursel = 0;
	bool bIncludePurse1 = FALSE;
	bool bIncludePurse2 = FALSE;
	bool bIncludePurse3 = FALSE;
	bool bIncludeCash = FALSE;
	int nSingleGroupNo = 0;
	int nSortOn = 0;
	bool bShowDeletedLines = FALSE;
	bool bShowUnusedLines = TRUE;
	bool bOnlyShowMonFri = FALSE;
	int nFolderSet = 0;
	int nTerminalListType = 0;
	int nReportRangeType = nREPORT_RANGE;
	CString strFromCardNo = System.GetLowCardNo();
	CString strToCardNo = System.GetHighCardNo();

	CString strLine;
	CReportDefaultHandler repdefs;
	if (repdefs.Read(m_strReportLabel, strLine) == TRUE)
	{
		CCSV csv(strLine);
		if (!csv.IsEmpty(0)) bIncludePurse1 = csv.GetBool(0);
		if (!csv.IsEmpty(1)) bIncludePurse2 = csv.GetBool(1);
		if (!csv.IsEmpty(2)) bIncludePurse3 = csv.GetBool(2);
		if (!csv.IsEmpty(3)) bIncludeCash = csv.GetBool(3);
		if (!csv.IsEmpty(4)) nReportRangeType = csv.GetInt(4);
		if (!csv.IsEmpty(5)) strFromCardNo = csv.GetString(5);
		if (!csv.IsEmpty(6)) strToCardNo = csv.GetString(6);
		//7 WAS DATE SELECTION
		if (!csv.IsEmpty(8)) nTimeCursel = csv.GetInt(8);
		if (!csv.IsEmpty(9)) nSingleGroupNo = csv.GetInt(9);
		if (!csv.IsEmpty(9)) nTerminalListType = csv.GetInt(10);
		if (!csv.IsEmpty(11)) nSortOn = csv.GetInt(11);
		if (!csv.IsEmpty(12)) bShowDeletedLines = csv.GetBool(12);
		if (!csv.IsEmpty(13)) bShowUnusedLines = csv.GetBool(13);
		if (!csv.IsEmpty(14)) bOnlyShowMonFri = csv.GetBool(14);
		if (!csv.IsEmpty(15)) strDateRangeType = csv.GetString(15);
		if (!csv.IsEmpty(16)) strDateFrom = csv.GetString(16);
		if (!csv.IsEmpty(17)) strDateTo = csv.GetString(17);
		if (!csv.IsEmpty(18)) strTimeStart = csv.GetString(18);
		if (!csv.IsEmpty(19)) strTimeEnd = csv.GetString(19);
		if (!csv.IsEmpty(20)) bContinuous = csv.GetBool(20);
		if (!csv.IsEmpty(21)) nFolderSet = csv.GetInt(21);
	}

	if (System.GetEnablePurse3Flag() == FALSE)
	{
		m_checkIncludePurse3.ShowWindow(SW_HIDE);
		bIncludePurse3 = FALSE;
	}

	CStandardCombos::FillSortOnCombo( m_comboSortOn, nSortOn);
	
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

	//Time Range
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

	m_checkContinuous.SetCheck(bContinuous);

//groups
	CGroupComboHelpers::FillGroupCombo(m_tabcomboGroup, nSingleGroupNo, TRUE);

	if (m_tabcomboGroup.GetCount() < 3)		// no individual groups added
	{
		m_tabcomboGroup.EnableWindow(FALSE);
	}

/// add audit periods
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

// card range
	SetCardRangeType(nReportRangeType);
	m_editCardNoFrom.SetWindowText(strFromCardNo);
	m_editCardNoTo.SetWindowText(strToCardNo);

// terminal list
	m_ReportLocationControls.Init(nFolderSet, nTerminalListType);

// other
	m_checkIncludePurse1.SetCheck(bIncludePurse1);
	m_checkIncludePurse2.SetCheck(bIncludePurse2);
	m_checkIncludePurse3.SetCheck(bIncludePurse3);
	m_checkIncludeCash.SetCheck(bIncludeCash);
	m_checkShowDeletedLines.SetCheck(bShowDeletedLines);
	m_checkShowUnusedLines.SetCheck(bShowUnusedLines);
	m_checkOnlyShowMonFri.SetCheck(bOnlyShowMonFri);

	SetRangeOptions();
	
//altkey
	m_buttonAltKey1.ShowWindow(SW_HIDE);

	return TRUE;
}

//*******************************************************

void CReportWeeklySpendDlg::OnSelectDateRange()
{
	m_DateSelectorRange.UpdateDateControls();
}

//$$******************************************************************

void CReportWeeklySpendDlg::OnChangeDateFrom(NMHDR* pNMHDR, LRESULT* pResult)
{
	m_DateSelectorRange.OnChangeFromDate();
	m_DateSelectorRange.CheckForChangedDate();
	*pResult = 0;
}

//$$******************************************************************

void CReportWeeklySpendDlg::OnChangeDateTo(NMHDR* pNMHDR, LRESULT* pResult)
{
	m_DateSelectorRange.CheckForChangedDate();
	*pResult = 0;
}

//$$******************************************************************

void CReportWeeklySpendDlg::OnSelectTimeRange()
{
	int nStartHour = 0;
	int nStartMins = 0;
	int nEndHour = 23;
	int nEndMins = 59;

	CString strTimeRange;
	m_comboTimeRange.GetWindowText(strTimeRange);
	SQLRowSetAuditPeriod.GetTimeRange(strTimeRange, nStartHour, nStartMins, nEndHour, nEndMins);

	COleDateTime startTime(2019, 1, 1, nStartHour, nStartMins, 0);
	m_TimePickerStart.SetTime(startTime);

	COleDateTime endTime(2019, 1, 1, nEndHour, nEndMins, 0);
	m_TimePickerEnd.SetTime(endTime);
}

//*******************************************************

void CReportWeeklySpendDlg::OnSelectFolderSet()
{
	m_ReportLocationControls.SelectFolderSet();
}

//*******************************************************************

void CReportWeeklySpendDlg::OnToggleTerminals()
{
	m_ReportLocationControls.ToggleTerminals();
}

//$$******************************************************************

void CReportWeeklySpendDlg::OnButtonAltKey1()
{
	m_ReportLocationControls.DeleteSelectedTerminalList();
}

//*******************************************************
void CReportWeeklySpendDlg::OnRadioRange() { SetCardRangeType(0); }
void CReportWeeklySpendDlg::OnRadioSingle() { SetCardRangeType(1); }
void CReportWeeklySpendDlg::OnRadioList() { SetCardRangeType(2); }
//*******************************************************

void CReportWeeklySpendDlg::SetCardRangeType(int n)
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

	m_nReportRangeType = n;
	SetRangeOptions();
}

//*******************************************************

void CReportWeeklySpendDlg::SetRangeOptions()
{
	bool bAllowFrom = TRUE;
	bool bAllowTo = TRUE;
	bool bAllowList = TRUE;
	bool bAllowGroup = TRUE;

	switch (m_nReportRangeType)
	{
	case nREPORT_RANGE:		
		bAllowList = FALSE;
		break;

	case nREPORT_SINGLE:	
		bAllowFrom = FALSE;
		bAllowList = FALSE;
		bAllowGroup = FALSE;
		break;

	case nREPORT_LIST:		
		bAllowTo = FALSE;
		bAllowFrom = FALSE;
		bAllowGroup = FALSE;
		break;
	}

	m_tabcomboGroup.EnableWindow(bAllowGroup);
	m_editCardNoFrom.EnableWindow(bAllowFrom);
	m_editCardNoTo.EnableWindow(bAllowTo);
	m_buttonList.EnableWindow(bAllowList);

	switch (m_nReportRangeType)
	{
	case nREPORT_RANGE:		m_editCardNoFrom.SetFocus();	break;
	case nREPORT_SINGLE:	m_editCardNoTo.SetFocus();		break;
	case nREPORT_LIST:		m_buttonList.SetFocus();		break;
	}
}

//*******************************************************

void CReportWeeklySpendDlg::OnDoubleClickRadioList() 
{ 
	OnButtonList(); 
}

//*******************************************************

void CReportWeeklySpendDlg::OnButtonList()
{
	CArray<CSQLRowAccountList, CSQLRowAccountList> arrayDummy;
	CCardPickerDlg dlg( m_strTitle, m_strReportList, arrayDummy, this);
	dlg.m_bValidAllowed = TRUE;
	dlg.m_bExpiredAllowed = TRUE;
	dlg.m_bInactiveAllowed = TRUE;

	if (dlg.DoModal() == IDOK)
	{
		m_Picker.Reset();
	}
}

//*******************************************************

void CReportWeeklySpendDlg::OnDoubleClickRadioSingle()
{
	CFindDlg dlg( "", this);
	if (dlg.DoModal() == IDOK)
	{
		m_editCardNoTo.SetWindowText(FormatInt64Value(dlg.m_nRequestedUserID));
	}
}

//*******************************************************

bool CReportWeeklySpendDlg::IsValid ( CSQLRowAccountFull& RowAccount )
{
	if (m_nReportRangeType == nREPORT_LIST)
	{
		return m_Picker.IsValid(RowAccount.GetUserIDString(), Filenames.GetReportCardListFilename(m_strReportLabel));
	}

	__int64 nCardNo = RowAccount.GetUserID();

	if (nCardNo < m_nRangeCardNoFrom || nCardNo > m_nRangeCardNoTo)
	{
		return FALSE;
	}

	if ((m_nGroupIndex > 0) && RowAccount.GetGroupNo() != m_nSingleGroupNo)	// not in single group
	{
		return FALSE;
	}

	return TRUE;
}

//*******************************************************

bool CReportWeeklySpendDlg::IsValid(CSQLAuditRecord* pAtc)
{
	if (pAtc->m_dateFiled < m_dateFrom)				//
		return FALSE;									//
	// //15/07/2016 - v1.12g
	if (pAtc->m_dateFiled > m_dateTo)					// 
		return FALSE;									//
	//
	if (m_bTimeRangeSet == TRUE)						//
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

	if (m_nReportRangeType == nREPORT_LIST)
	{
		return m_Picker.IsValid(pAtc->GetUserID(), Filenames.GetReportCardListFilename(m_strReportLabel));
	}

	__int64 nCardNo = _atoi64(pAtc->GetUserID());

	if (nCardNo < m_nRangeCardNoFrom || nCardNo > m_nRangeCardNoTo)
	{
		return FALSE;
	}

	if (m_nGroupIndex > 0 && pAtc->GetGroupNo() != m_nSingleGroupNo)	// not in single group
	{
		return FALSE;
	}

	return IsValidTerminalNo ( pAtc -> GetFolderSet(), pAtc->GetTerminalNo() );					// check is in terminal number range
}

//*******************************************************

bool CReportWeeklySpendDlg::IsValidTerminalNo ( int nFolderSet, int nTNo )
{
	return (m_TerminalListHandler.CheckTerminalFilter(nFolderSet, nTNo));
}

//*******************************************************

double CReportWeeklySpendDlg::GetSpendValue ( CSQLAuditRecord* pAtc )
{
	double dSpend = 0;

	if ( pAtc->GetApplicationNo() == APPNO_SALE )
	{
		if (m_bIncludePurse1 == TRUE)
		{
			dSpend = pAtc->GetTransactionPurse1Total();
		}

		if (m_bIncludePurse2 == TRUE)
		{
			dSpend += pAtc->GetTransactionPurse2Balance();
		}

		if (m_bIncludePurse3 == TRUE)
		{
			dSpend += pAtc->GetTransactionPurse3Total();
		}

		if (m_bIncludeCash == TRUE)
		{
			dSpend += pAtc->GetTransactionCash();
		}
	}

	return dSpend;
}

//*******************************************************

CString CReportWeeklySpendDlg::GetReportTitle()
{
	CString strText;

	if (m_bIncludePurse1 == TRUE)
	{
		strText = "P1";
	}

	if (m_bIncludePurse2 == TRUE)
	{
		if (strText != "")
		{
			strText += "+";
		}

		strText += "P2";
	}

	if (m_bIncludePurse3 == TRUE)
	{
		if (strText != "")
		{
			strText += "+";
		}

		strText += "P3";
	}

	if (m_bIncludeCash == TRUE)
	{
		if (strText != "")
		{
			strText += "+";
		}

		strText += "Cash";
	}

	CString strBuf = "";
	if (m_dateFrom != m_dateTo)
	{
		strBuf.Format(" (%s) %s - %s",
			(const char*)strText,
			m_dateFrom.GetDate(),
			m_dateTo.GetDate());
	}
	else
	{
		strBuf.Format(" (%s) %s",
			(const char*)strText,
			m_dateFrom.GetDate());			/* start date */
	}

	CString strReportTitle = m_strTitle;
	strReportTitle += strBuf;

	if (m_bTimeRangeSet == TRUE)
	{
		strBuf.Format(" (%s - %s)", m_timeStart.GetTime(), m_timeEnd.GetTime());
		strReportTitle += strBuf;
	}

	return strReportTitle;
}

//*******************************************************

void CReportWeeklySpendDlg::OnOK()
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

	m_bIncludePurse1 = IsTicked(m_checkIncludePurse1);
	m_bIncludePurse2 = IsTicked(m_checkIncludePurse2);
	m_bIncludePurse3 = IsTicked(m_checkIncludePurse3);
	m_bIncludeCash = IsTicked(m_checkIncludeCash);

	if ( ( FALSE == m_bIncludePurse1 ) && (FALSE == m_bIncludePurse2) && (FALSE == m_bIncludePurse3) && (FALSE == m_bIncludeCash) )
	{	
		Prompter.FatalError("No Include purse selections have been specified!", "Entry Error");
		m_checkIncludePurse1.SetFocus();
		return;
	}

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

	m_nReportRangeType = nREPORT_RANGE;
	if (IsTicked(m_radioSingle) == TRUE)
	{
		m_nReportRangeType = nREPORT_SINGLE;
	}
	else if (IsTicked(m_radioList) == TRUE)
	{
		m_nReportRangeType = nREPORT_LIST;
	}

	CString strCardNoFrom = GetEditBoxText(m_editCardNoFrom);
	CString strCardNoTo = GetEditBoxText(m_editCardNoTo);

	m_nSingleGroupNo = 0;
	m_strSingleGroupName = "";
	m_nGroupIndex = m_tabcomboGroup.GetCurSel();
	if (m_nGroupIndex > 0)
	{
		m_nSingleGroupNo = CGroupComboHelpers::GetGroupNoFromCombo(m_tabcomboGroup, 0);

		CSQLRowGroupFull RowGroup;
		RowGroup.SetGroupNo(m_nSingleGroupNo);

		CSQLRepositoryGroup RepoGroup;
		if (RepoGroup.SelectRow(RowGroup, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
		{
			m_strSingleGroupName = RowGroup.GetGroupName();
		}
	}

	m_nSortOn = m_comboSortOn.GetCurSel();

	m_bOnlyShowMonFri = IsTicked(m_checkOnlyShowMonFri);
	m_bShowDeletedLines = IsTicked(m_checkShowDeletedLines);
	m_bShowUnusedLines = IsTicked(m_checkShowUnusedLines);
	m_bContinuous = IsTicked(m_checkContinuous);

	CString strDateRangeType;
	m_comboDateRange.GetWindowText(strDateRangeType);

	CCSV csv;
	csv.Add(m_bIncludePurse1);
	csv.Add(m_bIncludePurse2);
	csv.Add(m_bIncludePurse3);
	csv.Add(m_bIncludeCash);
	csv.Add(m_nReportRangeType);
	csv.Add(strCardNoFrom);
	csv.Add(strCardNoTo);
	csv.Add(0); //WAS DATE SELECTIN
	csv.Add(m_comboTimeRange.GetCurSel());
	csv.Add(m_nSingleGroupNo);
	csv.Add(bWantTerminals ? 1 : 0);
	csv.Add(m_nSortOn);
	csv.Add(m_bShowDeletedLines);
	csv.Add(m_bShowUnusedLines);
	csv.Add(m_checkOnlyShowMonFri);
	csv.Add(strDateRangeType);
	csv.Add(m_dateFrom.GetDate());
	csv.Add(m_dateTo.GetDate());
	csv.Add(m_timeStart.GetTime());
	csv.Add(m_timeEnd.GetTime());
	csv.Add(m_bContinuous);

	CReportDefaultHandler repdefs;
	repdefs.Save(m_strReportLabel, &csv);

	CCSV csvReportKey;
	csvReportKey.Add(m_bIncludePurse1);
	csvReportKey.Add(m_bIncludePurse2);
	csvReportKey.Add(m_bIncludePurse3);
	csvReportKey.Add(m_bIncludeCash);
	csvReportKey.Add(m_nReportRangeType);
	csvReportKey.Add(m_nSingleGroupNo);
	csvReportKey.Add(m_bOnlyShowMonFri);
	m_strReportKey = csvReportKey.GetLine();

	m_nRangeCardNoFrom = _atoi64(strCardNoFrom);
	m_nRangeCardNoTo = _atoi64(strCardNoTo);

	if (m_nReportRangeType == nREPORT_SINGLE)
	{
		m_nRangeCardNoFrom = m_nRangeCardNoTo;
	}

	if (TRUE == bWantTerminals)
	{
		m_TerminalListHandler.TNoListSetMaxLength(strTerminalList.GetLength());	// stop entry being truncated
		m_TerminalListHandler.TNoListSave(strTerminalList);						// save terminal list

		m_TerminalListHandler.TNoArrayRemoveAll();
		m_TerminalListHandler.TNoListExtractTNos(strTerminalList);			// extract terminal numbers
	}

	EndDialog(IDOK);
}

//*******************************************************
