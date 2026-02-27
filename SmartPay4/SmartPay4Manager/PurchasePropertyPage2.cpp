//**************************************************************
#include "..\SmartPay4Shared\SQLTable_UserText\SQLRepositoryUserText.h"
#include "..\SmartPay4Shared\SQLTable_AuditPeriod\SQLRowSetAuditPeriod.h"
#include "..\SmartPay4Shared\TLogFile.h"
//**************************************************************
#include "SmartPay4Manager.h"
#include "CardPickerDlg.h"
#include "GroupPickerDlg.h"
#include "ReportDefaultHandler.h"
#include "FindDlg.h"
#include "PurchasePropertyPage2.h"
//**************************************************************

CPurchasePropertyPage2::CPurchasePropertyPage2(CReportPurchaseHelpers& ReportPurchaseHelpers)
	: CSSAutoShutdownPropertyPage(AutoShutdownHelper, CPurchasePropertyPage2::IDD), 
	m_ReportPurchaseHelpers(ReportPurchaseHelpers),
	m_DateSelectorRange(m_comboDateRange, m_datePickerStart, m_datePickerEnd),
	m_ReportLocationControls(m_comboFolderSet, m_checkTerminals, m_comboTerminals, ReportPurchaseHelpers.GetTerminalListHandler())
{
}

//*************************************************************************

void CPurchasePropertyPage2::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_datePickerStart);
	DDX_Control(pDX, IDC_DATETIMEPICKER2, m_datePickerEnd);
	DDX_Control(pDX, IDC_DATETIMEPICKER3, m_timePickerFrom);
	DDX_Control(pDX, IDC_DATETIMEPICKER4, m_timePickerTo);
	DDX_Control(pDX, IDC_CHECK_CONTINUOUS, m_checkContinuous);
	DDX_Control(pDX, IDC_COMBO_DATERANGE, m_comboDateRange);
	DDX_Control(pDX, IDC_COMBO_TIMERANGE, m_comboTimeRange);
	DDX_Control(pDX, IDC_EDIT_FROMCARDNO, m_editFromCardNo);
	DDX_Control(pDX, IDC_EDIT_TOCARDNO, m_editToCardNo);
	DDX_Control(pDX, IDC_RADIO_RANGE, m_radioRange);
	DDX_Control(pDX, IDC_RADIO_SINGLE, m_radioSingle);
	DDX_Control(pDX, IDC_RADIO_LIST, m_radioList);
	DDX_Control(pDX, IDC_RADIO_ALLGROUPS, m_radioAllGroups);
	DDX_Control(pDX, IDC_RADIO_LISTGROUPS, m_radioListGroups);
	DDX_Control(pDX, IDC_CHECK_NEWPAGE, m_checkNewPage);
	DDX_Control(pDX, IDC_CHECK_SKIPONBLANK, m_checkSkipOnBlank);
	DDX_Control(pDX, IDC_CHECK_SORTBYGROUP, m_checkSortByGroup);
	DDX_Control(pDX, IDC_STATIC_FOLDERSET, m_staticFolderSet);
	DDX_Control(pDX, IDC_COMBO_FOLDERSET, m_comboFolderSet);
	DDX_Control(pDX, IDC_CHECK_TERMINALS, m_checkTerminals);
	DDX_Control(pDX, IDC_COMBO_TERMINALS, m_comboTerminals);
	DDX_Control(pDX, IDC_CHECK_EXCLUDZERO, m_checkExcludeZeroPrice);
	DDX_Control(pDX, IDC_COMBO_INFO, m_comboInfoText);
	DDX_Control(pDX, IDC_BUTTON_ALTKEY1, m_buttonAltKey1);
	DDX_Control(pDX, IDC_BUTTON_LIST, m_buttonList);
	DDX_Control(pDX, IDC_BUTTON_LIST2, m_buttonList2);
}

//**************************************************************

BEGIN_MESSAGE_MAP(CPurchasePropertyPage2, CSSAutoShutdownPropertyPage)
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
	ON_CBN_SELCHANGE(IDC_COMBO_INFO, OnSelectInfo)
	ON_CBN_SELCHANGE(IDC_COMBO_GROUP, OnSelectGroup)
	ON_BN_CLICKED(IDC_BUTTON_LIST2, OnButtonList2)
	ON_BN_CLICKED(IDC_RADIO_ALLGROUPS, OnRadioGroupAll)
	ON_BN_CLICKED(IDC_RADIO_LISTGROUPS, OnRadioGroupList)
	ON_BN_CLICKED(IDC_BUTTON_ALTKEY1, OnButtonAltKey1)
END_MESSAGE_MAP()

//**************************************************************

BOOL CPurchasePropertyPage2::OnInitDialog()
{
	m_ReportPurchaseHelpers.SetGroupPickerFilename();

	int nReportNo = 0;
	int nTimeCursel = 0;
	int nGroupCurSel = 0;
	CString strDateRangeType = szRANGE_TODAY;
	
	CString strFromCardNo = m_ReportPurchaseHelpers.GetFromCardNo();
	CString strToCardNo = m_ReportPurchaseHelpers.GetToCardNo();

	CString strLine;
	CReportDefaultHandler repdefs;
	if (repdefs.Read(m_ReportPurchaseHelpers.GetReportLabelInternal(), strLine) == TRUE)
	{
		CCSV csv(strLine);

		if (csv.IsEmpty(0) == FALSE) m_ReportPurchaseHelpers.SetReportRangeType( csv.GetInt(0));
		if (csv.IsEmpty(1) == FALSE) strFromCardNo = csv.GetString(1);
		if (csv.IsEmpty(2) == FALSE) strToCardNo = csv.GetString(2);
		if (csv.IsEmpty(3) == FALSE) m_ReportPurchaseHelpers.SetAllNewPageFlags(csv.GetBool(3));
		//4 WAS DATE COMBO SELECTION
		if (csv.IsEmpty(5) == FALSE) nTimeCursel = csv.GetInt(5);
		if (csv.IsEmpty(6) == FALSE) m_ReportPurchaseHelpers.SetGroupListFlag(csv.GetInt(6) != 0 );
		if (csv.IsEmpty(7) == FALSE) m_ReportPurchaseHelpers.SetTerminalListType(csv.GetInt(7) == 1);
		if (csv.IsEmpty(8) == FALSE) m_ReportPurchaseHelpers.SetAllExcludeZeroPriceFlags( csv.GetBool(8));
		if (csv.IsEmpty(9) == FALSE) m_ReportPurchaseHelpers.SetInfoTextIndex(csv.GetInt(9));
		if (csv.IsEmpty(10) == FALSE) m_ReportPurchaseHelpers.SetSkipOnBlankFlag(csv.GetBool(10));
		if (csv.IsEmpty(11) == FALSE) m_ReportPurchaseHelpers.SetSortByGroupFlag( csv.GetBool(11));
		if (csv.IsEmpty(12) == FALSE) m_ReportPurchaseHelpers.SetContinuousFlag( csv.GetBool(12));
		if (csv.IsEmpty(13) == FALSE) strDateRangeType = csv.GetString(13);
		if (csv.IsEmpty(14) == FALSE) m_ReportPurchaseHelpers.SetDateFrom(csv.GetString(14));
		if (csv.IsEmpty(15) == FALSE) m_ReportPurchaseHelpers.SetDateTo(csv.GetString(15));
		if (csv.IsEmpty(16) == FALSE)	m_ReportPurchaseHelpers.SetFolderSet(csv.GetInt(16));
	}

	CPropertyPage::OnInitDialog();
	SetWindowText(m_ReportPurchaseHelpers.GetWindowTitle());

	m_editFromCardNo.LimitText(10);
	m_editToCardNo.LimitText(10);
	m_editFromCardNo.SetWindowText(strFromCardNo);
	m_editToCardNo.SetWindowText(strToCardNo);
	
	m_checkSortByGroup.SetCheck(m_ReportPurchaseHelpers.GetSortByGroupFlag());
	m_checkNewPage.SetCheck(m_ReportPurchaseHelpers.GetAllNewPageFlags() != 0);
	m_checkExcludeZeroPrice.SetCheck(m_ReportPurchaseHelpers.GetAllExcludeZeroPriceFlags() != 0);
	m_checkSkipOnBlank.SetCheck(m_ReportPurchaseHelpers.GetSkipOnBlankFlag());

	m_radioAllGroups.SetCheck(m_ReportPurchaseHelpers.GetGroupListFlag() == FALSE);
	m_radioListGroups.SetCheck(m_ReportPurchaseHelpers.GetGroupListFlag() == TRUE);

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
	m_comboDateRange.AddString(szRANGE_CUSTOM_DATE);
	m_comboDateRange.AddString(szRANGE_CUSTOM_RANGE);
	m_DateSelectorRange.SetComboSelection(strDateRangeType);

	if ((strDateRangeType == szRANGE_CUSTOM_DATE) || (strDateRangeType == szRANGE_CUSTOM_RANGE))
	{
		COleDateTime timeFrom(
			m_ReportPurchaseHelpers.GetDateFrom().GetYear(),
			m_ReportPurchaseHelpers.GetDateFrom().GetMonth(),
			m_ReportPurchaseHelpers.GetDateFrom().GetDay(), 0, 0, 0);

		if (timeFrom.m_status == COleDateTime::valid)
		{
			m_datePickerStart.SetTime(timeFrom);
		}

		COleDateTime timeTo(
			m_ReportPurchaseHelpers.GetDateTo().GetYear(),
			m_ReportPurchaseHelpers.GetDateTo().GetMonth(),
			m_ReportPurchaseHelpers.GetDateTo().GetDay(), 0, 0, 0);

		if (timeTo.m_status == COleDateTime::valid)
		{
			m_datePickerEnd.SetTime(timeTo);
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

	m_timePickerFrom.SetFormat("HH:mm");
	m_timePickerTo.SetFormat("HH:mm");

	m_checkContinuous.SetCheck(m_ReportPurchaseHelpers.GetContinuousFlag());

	// Info
	m_comboInfoText.AddString(szDONTCARE);

	for (int n = 1; n <= nMAX_INFOTEXTS; n++)
	{
		CSQLRepositoryUserText RepoText;
		m_comboInfoText.AddString(RepoText.GetDisplayInfoText(n));
	}

	m_comboInfoText.SetCurSel(m_ReportPurchaseHelpers.GetInfoTextIndex());

	// terminal list
	m_ReportLocationControls.Init(m_ReportPurchaseHelpers.GetFolderSet(), m_ReportPurchaseHelpers.GetTerminalListType());

	OnSelectDateRange();
	OnSelectTimeRange();
	SetDisplay();

	switch (m_ReportPurchaseHelpers.GetReportRangeType())
	{
	case 1:
		OnRadioSingle();
		break;

	case 2:
		OnRadioList();
		break;

	case 0:
	default:
		OnRadioRange();
		m_editFromCardNo.SetWindowText(strFromCardNo);
		m_editToCardNo.SetWindowText(strToCardNo);
		break;
	}

	m_buttonAltKey1.ShowWindow(SW_HIDE);

	return TRUE;
}

//**************************************************************

void CPurchasePropertyPage2::OnSelectFolderSet()
{
	m_ReportLocationControls.SelectFolderSet();
}

//*******************************************************************

void CPurchasePropertyPage2::OnToggleTerminals()
{
	m_ReportLocationControls.ToggleTerminals();
}

//$$******************************************************************

void CPurchasePropertyPage2::OnButtonAltKey1()
{
	m_ReportLocationControls.DeleteSelectedTerminalList();
}

//**************************************************************

BOOL CPurchasePropertyPage2::OnKillActive()
{
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

	m_ReportPurchaseHelpers.SetFolderSet(nFolderSet);
	m_ReportPurchaseHelpers.SetTerminalListType(bWantTerminals ? 1 : 0);

	if (m_ReportPurchaseHelpers.GetTerminalListType() == 1)
	{
		m_ReportPurchaseHelpers.TNoListSetMaxLength(strTerminalList.GetLength());	// stop entry being truncated
		m_ReportPurchaseHelpers.TNoListSave(strTerminalList);						// save terminal list

		m_ReportPurchaseHelpers.TNoArrayRemoveAll();
		m_ReportPurchaseHelpers.TNoListExtractTNos(strTerminalList);			// extract terminal numbers
	}

	return CPropertyPage::OnKillActive();
}

//**************************************************************

void CPurchasePropertyPage2::OnSelectDateRange() 
{
	m_DateSelectorRange.UpdateDateControls();
}

//**************************************************************

void CPurchasePropertyPage2::OnChangeDateFrom(NMHDR* pNMHDR, LRESULT* pResult)
{
	m_DateSelectorRange.OnChangeFromDate();
	m_DateSelectorRange.CheckForChangedDate();
	*pResult = 0;
}

//$$******************************************************************

void CPurchasePropertyPage2::OnChangeDateTo(NMHDR* pNMHDR, LRESULT* pResult)
{
	m_DateSelectorRange.CheckForChangedDate();
	*pResult = 0;
}

//$$******************************************************************

void CPurchasePropertyPage2::OnSelectTimeRange() 
{
	int nStartHour = 0;
	int nStartMins = 0;
	int nEndHour = 23;
	int nEndMins = 59;

	CString strTimeRange = GetComboBoxText(m_comboTimeRange);
	SQLRowSetAuditPeriod.GetTimeRange(strTimeRange, nStartHour, nStartMins, nEndHour, nEndMins);

	COleDateTime time;

	time.SetTime(nStartHour, nStartMins, 0);
	m_timePickerFrom.SetTime(time);

	time.SetTime(nEndHour, nEndMins, 0);
	m_timePickerTo.SetTime(time);
}

//**************************************************************

void CPurchasePropertyPage2::OnRadioRange()
{
	m_radioRange.SetCheck(TRUE);
	m_radioSingle.SetCheck(FALSE);
	m_radioList.SetCheck(FALSE);
	m_ReportPurchaseHelpers.SetReportRangeType(nREPORT_RANGE);
	m_editFromCardNo.SetWindowText(System.GetLowCardNo());
	m_editToCardNo.SetWindowText(System.GetHighCardNo());
	SetRangeOptions();
}

//**************************************************************

void CPurchasePropertyPage2::OnRadioSingle()	
{ 
	m_radioRange.SetCheck(FALSE);
	m_radioSingle.SetCheck(TRUE);
	m_radioList.SetCheck(FALSE);
	m_ReportPurchaseHelpers.SetReportRangeType(nREPORT_SINGLE);
	SetRangeOptions();
}

//**************************************************************

void CPurchasePropertyPage2::OnRadioList()
{ 
	m_radioRange.SetCheck(FALSE);
	m_radioSingle.SetCheck(FALSE);
	m_radioList.SetCheck(TRUE);
	m_ReportPurchaseHelpers.SetReportRangeType(nREPORT_LIST);
	SetRangeOptions();
}

//**************************************************************

void CPurchasePropertyPage2::SetRangeOptions()
{
	SetDisplay();

	switch ( m_ReportPurchaseHelpers.GetReportRangeType() )
	{
	case nREPORT_RANGE:		
		m_editFromCardNo.SetFocus();	
		break;

	case nREPORT_SINGLE:	
		m_editToCardNo.SetFocus();		
		break;

	case nREPORT_LIST:
		m_buttonList.SetFocus();		
		break;
	}
}

//**************************************************************

void CPurchasePropertyPage2::OnSelectInfo()		
{ 
	m_ReportPurchaseHelpers.SetInfoTextIndex(m_comboInfoText.GetCurSel());
	SetDisplay();
}

//**************************************************************

void CPurchasePropertyPage2::OnSelectGroup()
{ 
	SetDisplay(); 
}

//**************************************************************

void CPurchasePropertyPage2::OnRadioGroupAll()	
{ 
	m_radioAllGroups.SetCheck(TRUE);
	m_radioListGroups.SetCheck(FALSE);
	m_ReportPurchaseHelpers.SetGroupListFlag(FALSE);
	SetDisplay();
}

//**************************************************************

void CPurchasePropertyPage2::OnRadioGroupList()	
{ 
	m_radioAllGroups.SetCheck(FALSE);
	m_radioListGroups.SetCheck(TRUE);
	m_ReportPurchaseHelpers.SetGroupListFlag(TRUE);
	SetDisplay();
}

//**************************************************************

void CPurchasePropertyPage2::SetDisplay()
{
	bool bAllowFrom = TRUE;
	bool bAllowTo = TRUE;
	bool bAllowList = TRUE;
	bool bAllowGroup = TRUE;
	bool bSortByGroup = TRUE;
	bool bAllowNewPage = TRUE;
	bool bSkipBlank = TRUE;
	bool bAllowExcludeGroup = TRUE;

	switch (m_ReportPurchaseHelpers.GetReportRangeType())
	{
	case nREPORT_RANGE:
		bAllowList = FALSE;
		break;

	case nREPORT_SINGLE:
		bAllowFrom = FALSE;
		bAllowList = FALSE;
		bAllowGroup = FALSE;
		bSortByGroup = FALSE;
		bAllowNewPage = FALSE;
		bAllowExcludeGroup = FALSE;
		break;

	case nREPORT_LIST:		
		bAllowTo = FALSE;
		bAllowFrom = FALSE;
		bAllowGroup = FALSE;
		break;
	}

	if (m_ReportPurchaseHelpers.GetInfoTextIndex() == 0)
	{
		bSkipBlank = FALSE;
		bAllowNewPage = FALSE;
	}

	bool bAllowed = m_ReportPurchaseHelpers.GetGroupListFlag();
	m_buttonList2.EnableWindow(bAllowed);
	m_editFromCardNo.EnableWindow(bAllowFrom);
	m_editToCardNo.EnableWindow(bAllowTo);
	m_buttonList.EnableWindow(bAllowList);
	m_checkNewPage.EnableWindow(bAllowNewPage);
	m_checkSkipOnBlank.EnableWindow(bSkipBlank);
	m_checkSortByGroup.EnableWindow(bSortByGroup);
}

//**************************************************************

void CPurchasePropertyPage2::OnDoubleClickRadioList() 
{ 
	OnButtonList(); 
}

//**************************************************************

void CPurchasePropertyPage2::OnButtonList()
{
	CArray<CSQLRowAccountList, CSQLRowAccountList> arrayDummy;
	CCardPickerDlg dlg(m_ReportPurchaseHelpers.GetWindowTitle(), m_ReportPurchaseHelpers.GetCardListFilename(), arrayDummy, this);
	dlg.m_bValidAllowed = TRUE;
	dlg.m_bExpiredAllowed = TRUE;
	dlg.m_bAlertAllowed = TRUE;

	if (dlg.DoModal() == IDOK)
	{
		m_ReportPurchaseHelpers.ResetCardPicker();
	}
}

//**************************************************************

void CPurchasePropertyPage2::OnDoubleClickRadioSingle()
{
	CFindDlg dlg("", this);
	if (dlg.DoModal() == IDOK)
	{
		m_editFromCardNo.SetWindowText(FormatInt64Value(dlg.m_nRequestedUserID));
	}
}

//**************************************************************

void CPurchasePropertyPage2::OnButtonList2()
{
	CGroupPickerDlg dlg( "Select Group", m_ReportPurchaseHelpers.GetGroupPickerFilename(), this);

	if (dlg.DoModal() == IDOK)
	{
		m_ReportPurchaseHelpers.ResetGroupPicker();
	}
}

//**************************************************************

void CPurchasePropertyPage2::OnOK()
{
	{
		CTime dateFrom;
		m_datePickerStart.GetTime(dateFrom);
		m_ReportPurchaseHelpers.SetDateFrom(dateFrom);
	}

	{
		CTime dateTo;
		m_datePickerEnd.GetTime(dateTo);
		m_ReportPurchaseHelpers.SetDateTo(dateTo);
	}

	COleDateTime time;
	m_timePickerFrom.GetTime(time);
	CString strStartTime = time.Format("%H:%M");
	m_ReportPurchaseHelpers.SetTimeFrom(strStartTime);

	m_timePickerTo.GetTime(time);
	CString strEndTime = time.Format("%H:%M");
	m_ReportPurchaseHelpers.SetTimeTo(strEndTime);

	int nReportRangeType = nREPORT_RANGE;
	if (IsTicked(m_radioSingle) == TRUE)
	{
		nReportRangeType = nREPORT_SINGLE;
	}
	else if (IsTicked(m_radioList) == TRUE)
	{
		nReportRangeType = nREPORT_LIST;
	}

	m_ReportPurchaseHelpers.SetTimeRangeSetFlag((strStartTime != "00:00") || (strEndTime != "23:59"));
	m_ReportPurchaseHelpers.SetReportRangeType(nReportRangeType);
	m_ReportPurchaseHelpers.SetFromCardNo(GetEditBoxText(m_editFromCardNo));
	m_ReportPurchaseHelpers.SetToCardNo(GetEditBoxText(m_editToCardNo));
	m_ReportPurchaseHelpers.SetTimeRangeSetFlag((strStartTime != "00:00") || (strEndTime != "23:59"));
	m_ReportPurchaseHelpers.SetContinuousFlag( IsTicked(m_checkContinuous));
	m_ReportPurchaseHelpers.SetSortByGroupFlag(IsTicked(m_checkSortByGroup));
	m_ReportPurchaseHelpers.SetAllExcludeZeroPriceFlags(IsTicked(m_checkExcludeZeroPrice) ? 1 : 0);
	m_ReportPurchaseHelpers.SetAllNewPageFlags(IsTicked(m_checkNewPage) ? 1 : 0);
	m_ReportPurchaseHelpers.SetSkipOnBlankFlag(IsTicked(m_checkSkipOnBlank));

	CCSV csv;
	csv.Add(m_ReportPurchaseHelpers.GetReportRangeType());
	csv.Add(m_ReportPurchaseHelpers.GetFromCardNo());
	csv.Add(m_ReportPurchaseHelpers.GetToCardNo());
	csv.Add(m_ReportPurchaseHelpers.GetAllNewPageFlags() != 0);
	csv.Add(0); //WAS DATE RANGE CURSEL
	csv.Add(m_comboTimeRange.GetCurSel());
	csv.Add(m_ReportPurchaseHelpers.GetGroupListFlag());
	csv.Add(m_ReportPurchaseHelpers.GetTerminalListType());
	csv.Add(m_ReportPurchaseHelpers.GetAllExcludeZeroPriceFlags() != 0 );
	csv.Add(m_ReportPurchaseHelpers.GetInfoTextIndex());
	csv.Add(m_ReportPurchaseHelpers.GetSkipOnBlankFlag());
	csv.Add(m_ReportPurchaseHelpers.GetSortByGroupFlag());
	csv.Add(m_ReportPurchaseHelpers.GetContinuousFlag());
	csv.Add(GetComboBoxText(m_comboDateRange));
	csv.Add(m_ReportPurchaseHelpers.GetDateFrom().GetDate());
	csv.Add(m_ReportPurchaseHelpers.GetDateTo().GetDate());
	csv.Add(m_ReportPurchaseHelpers.GetFolderSet());

	CReportDefaultHandler repdefs;
	repdefs.Save(m_ReportPurchaseHelpers.GetReportLabelInternal(), &csv);

	CCSV csvReportKey;
	csvReportKey.Add(m_ReportPurchaseHelpers.GetReportRangeType());
	csvReportKey.Add(m_ReportPurchaseHelpers.GetGroupListFlag());
	csvReportKey.Add(m_ReportPurchaseHelpers.GetAllNewPageFlags() != 0);
	csvReportKey.Add(m_ReportPurchaseHelpers.GetTerminalListType());
	csvReportKey.Add(m_ReportPurchaseHelpers.GetSkipOnBlankFlag());
	csvReportKey.Add(m_ReportPurchaseHelpers.GetSortByGroupFlag());
	m_ReportPurchaseHelpers.SetReportKey(csvReportKey.GetLine());

	m_ReportPurchaseHelpers.SetRangeFromCardNo(_atoi64(m_ReportPurchaseHelpers.GetFromCardNo()));
	m_ReportPurchaseHelpers.SetRangeToCardNo(_atoi64(m_ReportPurchaseHelpers.GetToCardNo()));

	if (m_ReportPurchaseHelpers.GetReportRangeType() == nREPORT_SINGLE)
	{
		m_ReportPurchaseHelpers.SetRangeFromCardNo(m_ReportPurchaseHelpers.GetRangeToCardNo());
	}

	if (m_ReportPurchaseHelpers.GetGroupListFlag() == TRUE)
	{
		m_ReportPurchaseHelpers.SetSingleGroupNoFromGroupPicker();
	}

	EndDialog(IDOK);
}

//**************************************************************
