//**************************************************************
#include "..\SmartPay4Shared\SmartPayLockManager.h"
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\SQLTable_UserText\SQLRepositoryUserText.h"
#include "..\SmartPay4Shared\SQLTable_AuditPeriod\SQLRowSetAuditPeriod.h"
#include "..\SmartPay4Shared\TLogFile.h"
//**************************************************************
#include "SmartPay4Manager.h"
#include "CardPickerDlg.h"
#include "FindDlg.h"
#include "GroupComboHelpers.h"
#include "ReportDefaultHandler.h"
//*********************************************************************
#include "PurchasePropertyPage1.h"
//*********************************************************************

static const char* szREPORT1	= "Itemised report";		// nPURCHASE_ITEMISED			}
static const char* szREPORT2	= "Account item summary";	// nPURCHASE_ACCOUNTITEM		} keep togther & before group reporting
//static const char* szREPORT3	= "Account Dept summary";	// nPURCHASE_ACCOUNTDEPT		}
//static const char* szREPORT4	= "Group item summary";		// nPURCHASE_GROUPITEM
//static const char* szREPORT5	= "Group Dept summary";		// nPURCHASE_GROUPDEPT

//*********************************************************************

CPurchasePropertyPage1::CPurchasePropertyPage1(CReportPurchaseHelpers& ReportPurchaseHelpers)
	: CSSAutoShutdownPropertyPage(AutoShutdownHelper, CPurchasePropertyPage1::IDD), 
	m_ReportPurchaseHelpers(ReportPurchaseHelpers),
	m_DateSelectorRange(m_comboDateRange, m_datePickerStart, m_datePickerEnd),
	m_ReportLocationControls(m_comboFolderSet, m_checkTerminals, m_comboTerminals, ReportPurchaseHelpers.GetTerminalListHandler())
{
	m_strLastInfoFile = "";
}

//*************************************************************************

void CPurchasePropertyPage1::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_datePickerStart);
	DDX_Control(pDX, IDC_DATETIMEPICKER2, m_datePickerEnd);
	DDX_Control(pDX, IDC_DATETIMEPICKER3, m_timePickerFrom);
	DDX_Control(pDX, IDC_DATETIMEPICKER4, m_timePickerTo);
	DDX_Control(pDX, IDC_CHECK_CONTINUOUS, m_checkContinuous);
	DDX_Control(pDX, IDC_COMBO_DATERANGE, m_comboDateRange);
	DDX_Control(pDX, IDC_COMBO_TIMERANGE, m_comboTimeRange);
	DDX_Control(pDX, IDC_RADIO_RANGE, m_radioRange);
	DDX_Control(pDX, IDC_RADIO_SINGLE, m_radioSingle);
	DDX_Control(pDX, IDC_RADIO_LIST, m_radioList);
	DDX_Control(pDX, IDC_EDIT_FROMCARDNO, m_editFromCardNo);
	DDX_Control(pDX, IDC_EDIT_TOCARDNO, m_editToCardNo);
	DDX_Control(pDX, IDC_BUTTON_LIST, m_buttonList);
	DDX_Control(pDX, IDC_CHECK_NEWPAGE, m_checkNewPage);
	DDX_Control(pDX, IDC_COMBO_GROUP, m_tabcomboGroup);
	DDX_Control(pDX, IDC_STATIC_FOLDERSET, m_staticFolderSet);
	DDX_Control(pDX, IDC_COMBO_FOLDERSET, m_comboFolderSet);
	DDX_Control(pDX, IDC_CHECK_TERMINALS, m_checkTerminals);
	DDX_Control(pDX, IDC_COMBO_TERMINALS, m_comboTerminals);
	DDX_Control(pDX, IDC_COMBO_SUMMARY, m_comboSummary);
	DDX_Control(pDX, IDC_CHECK_EXCLUDZERO, m_checkExcludeZeroPrice);
	DDX_Control(pDX, IDC_CHECK_SHOWNAME, m_checkShowAccountName);
	DDX_Control(pDX, IDC_COMBO_INFO, m_comboInfoText);
	DDX_Control(pDX, IDC_COMBO_INFOFILTER, m_comboInfoFilter);
	DDX_Control(pDX, IDC_BUTTON_ALTKEY1, m_buttonAltKey1);
	DDX_Control(pDX, IDC_BUTTON_ALTKEY2, m_buttonAltKey2);
	DDX_Control(pDX, IDC_BUTTON_ALTKEY3, m_buttonAltKey3);
	DDX_Control(pDX, IDC_BUTTON_SORT, m_buttonSort);
}

//**********************************************************************

BEGIN_MESSAGE_MAP(CPurchasePropertyPage1, CSSAutoShutdownPropertyPage)
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
	ON_CBN_SELCHANGE(IDC_COMBO_SUMMARY, OnSelectSummary)
	ON_BN_CLICKED(IDC_BUTTON_SORT, OnButtonSort)
	ON_CBN_SELCHANGE(IDC_COMBO_INFO, OnSelectInfo)
	ON_BN_CLICKED(IDC_BUTTON_ALTKEY1, OnButtonAltKey1 )
	ON_BN_CLICKED(IDC_BUTTON_ALTKEY2, OnButtonAltKey2 )
	ON_BN_CLICKED(IDC_BUTTON_ALTKEY3, OnButtonAltKey3 )
END_MESSAGE_MAP()

//**********************************************************************

BOOL CPurchasePropertyPage1::OnInitDialog()
{
	if (m_ReportPurchaseHelpers.IsPurchaseHistoryReportSingle() || m_ReportPurchaseHelpers.IsStatementReportSingle())
	{
		m_ReportPurchaseHelpers.SetReportRangeType(nREPORT_SINGLE);
	}

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

		if (m_ReportPurchaseHelpers.GetReportRangeType() != nREPORT_SINGLE)					// if set - the following parameters have already been set
		{
			if (csv.IsEmpty(0) == FALSE)	m_ReportPurchaseHelpers.SetReportRangeType(csv.GetInt(0));
			if (csv.IsEmpty(1) == FALSE)	strFromCardNo = csv.GetString(1);
			if (csv.IsEmpty(2) == FALSE)	strToCardNo = csv.GetString(2);
		}

		if (csv.IsEmpty(3) == FALSE)	m_ReportPurchaseHelpers.SetAllNewPageFlags(csv.GetBool(3));
		//4 WAS DATE COMBO SELECTION
		if (csv.IsEmpty(5) == FALSE)	nTimeCursel = csv.GetInt(5);
		if (csv.IsEmpty(6) == FALSE)	m_ReportPurchaseHelpers.SetPurchaseReportType(csv.GetInt(6));	
		if (csv.IsEmpty(7) == FALSE)	m_ReportPurchaseHelpers.SetSingleGroupNo(csv.GetInt(7));
		if (csv.IsEmpty(8) == FALSE)	m_ReportPurchaseHelpers.SetTerminalListType(csv.GetInt(8));
		if (csv.IsEmpty(9) == FALSE)	m_ReportPurchaseHelpers.SetAllExcludeZeroPriceFlags(csv.GetInt(9));
		if (csv.IsEmpty(10) == FALSE)	m_ReportPurchaseHelpers.SetShowAccountNameFlag(csv.GetBool(10));
		if (csv.IsEmpty(11) == FALSE)	m_ReportPurchaseHelpers.SetInfoTextIndex(csv.GetInt(11));
		if (csv.IsEmpty(12) == FALSE)	m_ReportPurchaseHelpers.SetInfoTextFilter(csv.GetString(12));
		if (csv.IsEmpty(13) == FALSE)	m_ReportPurchaseHelpers.SetContinuousFlag(csv.GetBool(13));
		if (csv.IsEmpty(14) == FALSE)	strDateRangeType = csv.GetString(14);
		if (csv.IsEmpty(15) == FALSE)	m_ReportPurchaseHelpers.SetDateFrom(csv.GetString(15));
		if (csv.IsEmpty(16) == FALSE)	m_ReportPurchaseHelpers.SetDateTo(csv.GetString(16));
		if (csv.IsEmpty(17) == FALSE)	m_ReportPurchaseHelpers.SetFolderSet(csv.GetInt(17));
	}

	CPropertyPage::OnInitDialog();
	SetWindowText(m_ReportPurchaseHelpers.GetWindowTitle());

	m_editFromCardNo.LimitText(10);
	m_editToCardNo.LimitText(10);
	m_editFromCardNo.SetWindowText(strFromCardNo);
	m_editToCardNo.SetWindowText(strToCardNo);

	//groups
	CGroupComboHelpers::FillGroupCombo(m_tabcomboGroup, m_ReportPurchaseHelpers.GetSingleGroupNo(), TRUE);

	// fill out report summary combo
	FillReportCombo(m_ReportPurchaseHelpers.GetPurchaseReportType());

	// Info

	m_comboInfoText.AddString(szDONTCARE);

	for (int n = 1; n <= nMAX_INFOTEXTS; n++)
	{
		CSQLRepositoryUserText RepoText;
		m_comboInfoText.AddString(RepoText.GetDisplayInfoText(n));
	}

	m_comboInfoText.SetCurSel(m_ReportPurchaseHelpers.GetInfoTextIndex());
	OnSelectInfo();

	// terminal list
	m_ReportLocationControls.Init(m_ReportPurchaseHelpers.GetFolderSet(), m_ReportPurchaseHelpers.GetTerminalListType());

	// other
	if (m_ReportPurchaseHelpers.IsSingleAccountReport() == TRUE)					// single history or statement
	{
		m_radioRange.EnableWindow(FALSE);
		m_radioList.EnableWindow(FALSE);
		m_radioSingle.EnableWindow(FALSE);
		m_tabcomboGroup.EnableWindow(FALSE);

		m_editFromCardNo.EnableWindow(FALSE);
		m_editToCardNo.EnableWindow(FALSE);
		m_buttonList.EnableWindow(FALSE);
		m_comboInfoText.EnableWindow(FALSE);							//10/10/2016 - v1.13x
		m_comboInfoFilter.EnableWindow(FALSE);							//10/10/2016 - v1.13x
		m_checkNewPage.EnableWindow(FALSE);
	}

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

	OnSelectTimeRange();
	OnSelectSummary();

	m_checkShowAccountName.SetCheck(m_ReportPurchaseHelpers.GetShowAccountNameFlag());
	m_checkContinuous.SetCheck(m_ReportPurchaseHelpers.GetContinuousFlag());

	m_buttonSort.ShowWindow(SW_HIDE);

	m_buttonAltKey1.ShowWindow(SW_HIDE);
	m_buttonAltKey2.ShowWindow(SW_HIDE);
	m_buttonAltKey3.ShowWindow(SW_HIDE);

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

	return TRUE;
}

//*******************************************************************

void CPurchasePropertyPage1::FillReportCombo ( int nCurrentReportType )
{
	m_comboSummary.AddString ( szREPORT1 );					// "Itemised report"
	m_arrayReportReqd.Add ( nPURCHASE_ITEMISED );

	m_comboSummary.AddString ( szREPORT2 );					// "Plu Summary by account"
	m_arrayReportReqd.Add ( nPURCHASE_ACCOUNTITEM );

//	m_comboSummary.AddString ( szREPORT3 );					// "Dept Summary by account"
//	m_arrayReportReqd.Add ( nPURCHASE_ACCOUNTDEPT );
//
//	if ( m_strReportLabel == "R104" )						// Purchase History - all records
//	{
//		m_comboSummary.AddString ( szREPORT4 );				// Plu summary by group
//		m_arrayReportReqd.Add ( nPURCHASE_GROUPITEM );
//
//		m_comboSummary.AddString ( szREPORT5 );				// Dept summary by group
//		m_arrayReportReqd.Add ( nPURCHASE_GROUPDEPT );
//	}

	int nCurSel = 0;
	for ( int i = 0 ; i < m_arrayReportReqd.GetCount() ; i++ )
	{
		if ( m_arrayReportReqd.GetAt(i) == nCurrentReportType )
		{
			nCurSel = i;
			break;
		}
	}

	m_comboSummary.SetCurSel ( nCurSel );
}

//********************************************************************

void CPurchasePropertyPage1::AddInfoSelections()
{
	CString strCommentFile = Filenames.InfoListSelections(m_ReportPurchaseHelpers.GetInfoTextIndex());
	if (strCommentFile != m_strLastInfoFile)
	{
		m_InfoList.ReadComments(strCommentFile);
		m_strLastInfoFile = strCommentFile;					// stop same file from being read again 
	}

	m_comboInfoFilter.ResetContent();						// new list

	for (int i = 0; i < m_InfoList.GetSize(); i++)		// read comments
	{
		m_comboInfoFilter.AddString(m_InfoList.GetAt(i));
	}

	int nCurSel = 0;
	for (int i = 0; i < m_InfoList.GetSize(); i++)		// do it outside abount loop as combo items get sorted
	{
		CString strInfo = "";
		m_comboInfoFilter.GetLBText(i, strInfo);
		if (strInfo.CompareNoCase(m_ReportPurchaseHelpers.GetInfoTextFilter()) == 0)
		{
			nCurSel = i;
		}
	}

	m_comboInfoFilter.SetCurSel(nCurSel);
}

//**********************************************************

void CPurchasePropertyPage1::OnButtonAltKey3()
{
	CString strInfoTextFilter = GetComboBoxText(m_comboInfoFilter);
	m_ReportPurchaseHelpers.SetInfoTextIndex(m_comboInfoText.GetCurSel());

	if ((strInfoTextFilter != "") && (m_ReportPurchaseHelpers.GetInfoTextIndex() > 0))
	{
		m_InfoList.Delete(strInfoTextFilter);		// delete comment
		AddInfoSelections();
	}
}

//**********************************************************

void CPurchasePropertyPage1::OnSelectFolderSet()
{
	m_ReportLocationControls.SelectFolderSet();
}

//*******************************************************************

void CPurchasePropertyPage1::OnToggleTerminals()
{
	m_ReportLocationControls.ToggleTerminals();
}

//$$******************************************************************

void CPurchasePropertyPage1::OnButtonAltKey1()
{
	m_ReportLocationControls.DeleteSelectedTerminalList();
}

//$$******************************************************************

BOOL CPurchasePropertyPage1::OnKillActive()
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

//******************************************************************

void CPurchasePropertyPage1::OnSelectDateRange()
{
	m_DateSelectorRange.UpdateDateControls();
}

//**********************************************************

void CPurchasePropertyPage1::OnChangeDateFrom(NMHDR* pNMHDR, LRESULT* pResult)
{
	m_DateSelectorRange.OnChangeFromDate();
	m_DateSelectorRange.CheckForChangedDate();
	*pResult = 0;
}

//$$******************************************************************

void CPurchasePropertyPage1::OnChangeDateTo(NMHDR* pNMHDR, LRESULT* pResult)
{
	m_DateSelectorRange.CheckForChangedDate();
	*pResult = 0;
}

//$$******************************************************************

void CPurchasePropertyPage1::OnSelectTimeRange()
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

//*******************************************************************

void CPurchasePropertyPage1::OnRadioRange()
{
	m_radioRange.SetCheck(TRUE);
	m_radioSingle.SetCheck(FALSE);
	m_radioList.SetCheck(FALSE);
	m_ReportPurchaseHelpers.SetReportRangeType(nREPORT_RANGE);
	m_editFromCardNo.SetWindowText(System.GetLowCardNo());
	m_editToCardNo.SetWindowText(System.GetHighCardNo());
	SetRangeOptions();
}

//**********************************************************

void CPurchasePropertyPage1::OnRadioSingle()		 
{ 
	m_radioRange.SetCheck(FALSE);
	m_radioSingle.SetCheck(TRUE);
	m_radioList.SetCheck(FALSE);
	m_ReportPurchaseHelpers.SetReportRangeType(nREPORT_SINGLE);
	SetRangeOptions();
}

//**********************************************************

void CPurchasePropertyPage1::OnRadioList()		
{ 
	m_radioRange.SetCheck(FALSE);
	m_radioSingle.SetCheck(FALSE);
	m_radioList.SetCheck(TRUE);
	m_ReportPurchaseHelpers.SetReportRangeType(nREPORT_LIST);
	SetRangeOptions();
}

//**********************************************************

void CPurchasePropertyPage1::SetRangeOptions()
{
	bool bAllowFrom = TRUE;
	bool bAllowTo = TRUE;
	bool bAllowList = TRUE;
	bool bAllowGroup = TRUE;
	bool bAllowNewPage = TRUE;

	switch (m_ReportPurchaseHelpers.GetReportRangeType())
	{
	case nREPORT_RANGE:	
		bAllowList = FALSE;
		break;

	case nREPORT_SINGLE:	
		bAllowFrom = FALSE;
		bAllowList = FALSE;
		bAllowGroup = FALSE;
		bAllowNewPage = FALSE;
		break;

	case nREPORT_LIST:		
		bAllowTo = FALSE;
		bAllowFrom = FALSE;
		bAllowGroup = FALSE;
		break;
	}

	m_tabcomboGroup.EnableWindow(bAllowGroup);
	m_editFromCardNo.EnableWindow(bAllowFrom);
	m_editToCardNo.EnableWindow(bAllowTo);
	m_buttonList.EnableWindow(bAllowList);
	m_checkNewPage.EnableWindow(bAllowNewPage);

	switch (m_ReportPurchaseHelpers.GetReportRangeType())
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

//*******************************************************************

void CPurchasePropertyPage1::OnDoubleClickRadioList()
{ 
	OnButtonList();
}

//*******************************************************************

void CPurchasePropertyPage1::OnButtonList()
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

//*******************************************************************

void CPurchasePropertyPage1::OnDoubleClickRadioSingle()
{
	CFindDlg dlg("", this);
	if (dlg.DoModal() == IDOK)
	{
		m_editFromCardNo.SetWindowText(FormatInt64Value(dlg.m_nRequestedUserID));
	}
}

//*******************************************************************

void CPurchasePropertyPage1::OnSelectSummary()
{
	int nSel = m_comboSummary.GetCurSel();

	if ((nSel >= 0) && (nSel < m_arrayReportReqd.GetSize()))
	{
		int nReportType = m_arrayReportReqd.GetAt(nSel);
		m_ReportPurchaseHelpers.SetPurchaseReportType(nReportType);
		m_checkNewPage.SetCheck(m_ReportPurchaseHelpers.GetNewPageFlag(nReportType));
		m_checkExcludeZeroPrice.SetCheck(m_ReportPurchaseHelpers.GetExcludeZeroPriceFlag(nReportType));
	}
}

//*******************************************************************

void CPurchasePropertyPage1::OnSelectInfo()
{
	m_ReportPurchaseHelpers.SetInfoTextIndex(m_comboInfoText.GetCurSel());
	AddInfoSelections();
	m_comboInfoFilter.EnableWindow(m_ReportPurchaseHelpers.GetInfoTextIndex() != 0);
	m_comboInfoFilter.SetFocus();
}

//*******************************************************************
// altkey option to re-sort a single purchase history file

void CPurchasePropertyPage1::OnButtonAltKey2()
{
	if (m_ReportPurchaseHelpers.GetReportRangeType() == nREPORT_SINGLE)
	{
		m_buttonSort.ShowWindow(SW_SHOW);
	}
}

//*******************************************************************

void CPurchasePropertyPage1::OnButtonSort()
{
	CSmartPayLockRequest Request;
	Request.m_bWantLockBgnd = TRUE;
	Request.m_bWantLockServerInner = TRUE;
	
	CString strMsg = "";

	{
		CSmartPayLockManager LockManager;
		int nLockError = LockManager.AcquireLocks(Request, this);

		if (LOCKMANAGER_OK == nLockError)
		{
			CString strCardNo = GetEditBoxText(m_editToCardNo);
			CString strTLogFile = Filenames.GetPurchaseHistoryFilename(strCardNo);	// UserID maybe "" for cash account log

			CTLogFile tlog(strCardNo, strTLogFile);
			if (tlog.Sort(strTLogFile) == TRUE)
			{
				strMsg = "Purchase file sorted, no errors reported";
			}
		}
	}

	if (strMsg != "")
	{
		Prompter.Info(strMsg, "Sort History");
	}
	
}

//*******************************************************************

void CPurchasePropertyPage1::OnOK()
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

	m_ReportPurchaseHelpers.SetTimeRangeSetFlag((strStartTime != "00:00") || (strEndTime != "23:59"));

	m_ReportPurchaseHelpers.SetGroupComboIndex(m_tabcomboGroup.GetCurSel());
	if (m_ReportPurchaseHelpers.GetGroupComboIndex() > 0)
	{
		m_ReportPurchaseHelpers.SetSingleGroupNo( CGroupComboHelpers::GetGroupNoFromCombo(m_tabcomboGroup, 0));
	}
	else
	{
		m_ReportPurchaseHelpers.SetSingleGroupNo(0);
	}

	m_ReportPurchaseHelpers.SetNewPageFlag(m_ReportPurchaseHelpers.GetPurchaseReportType(), IsTicked(m_checkNewPage));								// set new page reqd indicator for selected report type
	m_ReportPurchaseHelpers.SetExcludeZeroPriceFlag(m_ReportPurchaseHelpers.GetPurchaseReportType(), IsTicked(m_checkExcludeZeroPrice));

	m_ReportPurchaseHelpers.SetFromCardNo(GetEditBoxText(m_editFromCardNo));
	m_ReportPurchaseHelpers.SetToCardNo(GetEditBoxText(m_editToCardNo));
	m_ReportPurchaseHelpers.SetInfoTextFilter(GetComboBoxText(m_comboInfoFilter));
	m_ReportPurchaseHelpers.SetInfoTextIndex(m_comboInfoText.GetCurSel());
	m_ReportPurchaseHelpers.SetContinuousFlag(IsTicked(m_checkContinuous));
	m_ReportPurchaseHelpers.SetShowAccountNameFlag(IsTicked(m_checkShowAccountName));

	CCSV csv;
	csv.Add(m_ReportPurchaseHelpers.GetReportRangeType());
	csv.Add(m_ReportPurchaseHelpers.GetFromCardNo());
	csv.Add(m_ReportPurchaseHelpers.GetToCardNo());
	csv.Add(m_ReportPurchaseHelpers.GetAllNewPageFlags());
	csv.Add(0); //WAS DATE RANGE COMBO SEL
	csv.Add(m_comboTimeRange.GetCurSel());
	csv.Add(m_ReportPurchaseHelpers.GetPurchaseReportType());
	csv.Add(m_ReportPurchaseHelpers.GetSingleGroupNo());
	csv.Add(m_ReportPurchaseHelpers.GetTerminalListType());
	csv.Add(m_ReportPurchaseHelpers.GetAllExcludeZeroPriceFlags());
	csv.Add(m_ReportPurchaseHelpers.GetShowAccountNameFlag());
	csv.Add(m_ReportPurchaseHelpers.GetInfoTextIndex());
	csv.Add(m_ReportPurchaseHelpers.GetInfoTextFilter());
	csv.Add(m_ReportPurchaseHelpers.GetContinuousFlag());
	csv.Add(GetComboBoxText(m_comboDateRange));
	csv.Add(m_ReportPurchaseHelpers.GetDateFrom().GetDate());
	csv.Add(m_ReportPurchaseHelpers.GetDateTo().GetDate());
	csv.Add(m_ReportPurchaseHelpers.GetFolderSet());

	CReportDefaultHandler repdefs;
	repdefs.Save(m_ReportPurchaseHelpers.GetReportLabelInternal(), &csv);

	CCSV csvReportKey;
	csvReportKey.Add(m_ReportPurchaseHelpers.GetReportRangeType());
	csvReportKey.Add(m_ReportPurchaseHelpers.GetPurchaseReportType());
	csvReportKey.Add(m_ReportPurchaseHelpers.GetSingleGroupNo());
	m_ReportPurchaseHelpers.SetReportKey(csvReportKey.GetLine());

	m_ReportPurchaseHelpers.SetRangeFromCardNo( _atoi64(m_ReportPurchaseHelpers.GetFromCardNo()));
	m_ReportPurchaseHelpers.SetRangeToCardNo( _atoi64(m_ReportPurchaseHelpers.GetToCardNo()));

	if (m_ReportPurchaseHelpers.GetReportRangeType() == nREPORT_SINGLE)
	{
		m_ReportPurchaseHelpers.SetRangeFromCardNo( m_ReportPurchaseHelpers.GetRangeToCardNo());
	}

	if (m_ReportPurchaseHelpers.GetInfoTextIndex() > 0)
	{
		CString strFilterText = m_ReportPurchaseHelpers.GetInfoTextFilter();
		m_InfoList.SetMaxLength(strFilterText.GetLength());	// stop entry being truncated
		m_InfoList.Save(strFilterText);						// save terminal list
	}

	EndDialog(IDOK);
}

//*******************************************************************
