//$$******************************************************************
#include "..\SPosLoyalty4Shared\SQLTable_AuditPeriod\SQLRowSetAuditPeriod.h"
#include "..\SPosLoyalty4Shared\SQLTable_EcrmanDatabase\SQLRepositoryEcrmanDatabase.h"
//$$******************************************************************
#include "GroupComboHelpers.h"
#include "LoyaltyManager.h"
#include "CardPickerDlg.h"
#include "FindDlg.h"
#include "ReportDefaultHandler.h"
#include "StandardCombos.h"
//$$******************************************************************
#include "ReportPurchaseHistoryDlg.h"
//$$******************************************************************

CReportPurchaseHistoryDlg::CReportPurchaseHistoryDlg( CReportInfoPurchaseHistory& ReportInfo, CWnd* pParent )
	: CSSDialog(CReportPurchaseHistoryDlg::IDD, pParent)
	, m_DateSelectorRange( m_comboDateRange, m_DatePickerFrom, m_DatePickerTo )
	, m_ReportLocationControls(m_comboDatabase, m_comboFolderSet, m_checkTerminals, m_comboTerminals, ReportInfo.GetTerminalListHandler())
	, m_ReportInfo( ReportInfo )
{
	SQLRowSetAuditPeriod.LoadPeriods(NULL);
	m_nSalesType = 0;
}

//$$******************************************************************

CReportPurchaseHistoryDlg::~CReportPurchaseHistoryDlg()
{
}

//$$******************************************************************

void CReportPurchaseHistoryDlg::DoDataExchange(CDataExchange* pDX)
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
	DDX_Control(pDX, IDC_EDIT_FROMCARDNO, m_editFromCardNo);
	DDX_Control(pDX, IDC_EDIT_TOCARDNO, m_editToCardNo);
	DDX_Control(pDX, IDC_COMBO_GROUP, m_comboGroup);
	DDX_Control(pDX, IDC_CHECK_NEWPAGE, m_checkNewPage);
	//terminals
	DDX_Control(pDX, IDC_COMBO_DATABASE, m_comboDatabase);
	DDX_Control(pDX, IDC_COMBO_FOLDERSET, m_comboFolderSet);
	DDX_Control(pDX, IDC_CHECK_TERMINALS, m_checkTerminals);
	DDX_Control(pDX, IDC_COMBO_TERMINALS, m_comboTerminals);
	//options
	DDX_Control(pDX, IDC_CHECK_SUMMARY, m_checkSummary);
	DDX_Control(pDX, IDC_RADIO_SALES_ALL, m_radioSalesAll);
	DDX_Control(pDX, IDC_RADIO_SALES_ITEM, m_radioSalesItem);
	DDX_Control(pDX, IDC_RADIO_SALES_RENEWAL, m_radioSalesRenewal);
	//altkey
	DDX_Control(pDX, IDC_BUTTON_ALTKEY1, m_buttonAltKey1);
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CReportPurchaseHistoryDlg, CSSDialog)
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
	//terminals
	ON_CBN_SELCHANGE(IDC_COMBO_DATABASE, OnSelectDatabase)
	ON_CBN_SELCHANGE(IDC_COMBO_FOLDERSET, OnSelectFolderSet)
	ON_BN_CLICKED(IDC_CHECK_TERMINALS, OnToggleTerminals)
	//options
	ON_BN_CLICKED(IDC_RADIO_SALES_ALL, OnRadioSalesAll)
	ON_BN_CLICKED(IDC_RADIO_SALES_ITEM, OnRadioSalesItem)
	ON_BN_CLICKED(IDC_RADIO_SALES_RENEWAL, OnRadioSalesRenewal)
	//altkey
	ON_BN_CLICKED(IDC_BUTTON_ALTKEY1, OnButtonAltKey1)

END_MESSAGE_MAP()

//$$******************************************************************

BOOL CReportPurchaseHistoryDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_editFromCardNo.LimitText(System.GetMaxCardNoLength());
	m_editFromCardNo.SetWindowText(System.GetLowCardNo());

	m_editToCardNo.LimitText(System.GetMaxCardNoLength());
	m_editToCardNo.SetWindowText(System.GetHighCardNo());

	int nPreviousCardRangeType = 0;
	CString strPreviousCardFrom = System.GetLowCardNo();
	CString strPreviousCardTo = System.GetHighCardNo();
	bool bPreviousNewPage = FALSE;
	CString strPreviousDateType = "";
	int nPreviousTimeCurSel = 0;
	bool bPreviousSummary = FALSE;
	int nPreviousSingleGroupNo = 0;
	int nPreviousTerminalListType = 0;
	int nPreviousSalesType = 0;
	CString strPreviousDateFrom = "";
	CString strPreviousDateTo = "";
	CString strPreviousTimeFrom = "";
	CString strPreviousTimeTo = "";
	int nPreviousDbNo = 1;
	int nPreviousFolderSet = 0;

	CString strLine;
	CReportDefaultHandler repdefs;
	if (repdefs.Read(m_ReportInfo.GetReportLabel(), strLine) == TRUE)
	{
		CCSV csv(strLine);

		if (csv.IsEmpty(0) == FALSE)		nPreviousCardRangeType = csv.GetInt(0);
		if (csv.IsEmpty(1) == FALSE)		strPreviousCardFrom = csv.GetString(1);
		if (csv.IsEmpty(2) == FALSE)		strPreviousCardTo = csv.GetString(2);
		if (csv.IsEmpty(3) == FALSE)		bPreviousNewPage = csv.GetBool(3);
		if (csv.IsEmpty(4) == FALSE)		strPreviousDateType = csv.GetString(4);
		if (csv.IsEmpty(5) == FALSE)		nPreviousTimeCurSel = csv.GetInt(5);
		if (csv.IsEmpty(6) == FALSE)		bPreviousSummary = csv.GetBool(6);
		if (csv.IsEmpty(7) == FALSE)		nPreviousSingleGroupNo = csv.GetInt(7);
		if (csv.IsEmpty(7) == FALSE)		nPreviousTerminalListType = csv.GetInt(8);
		if (csv.IsEmpty(9) == FALSE)
		{
			if (csv.GetInt(1) == 1)
			{
				nPreviousSalesType = 2;
			}
		}
		if (csv.IsEmpty(10) == FALSE)		strPreviousDateFrom = csv.GetString(10);
		if (csv.IsEmpty(11) == FALSE)		strPreviousDateTo = csv.GetString(11);
		if (csv.IsEmpty(12) == FALSE)		strPreviousTimeFrom = csv.GetString(12);
		if (csv.IsEmpty(13) == FALSE)		strPreviousTimeTo = csv.GetString(13);
		if (csv.IsEmpty(14) == FALSE)		nPreviousSalesType = csv.GetInt(14);
		if (csv.IsEmpty(15) == FALSE)		nPreviousDbNo = csv.GetInt(15);
		if (csv.IsEmpty(16) == FALSE)		nPreviousFolderSet = csv.GetInt(16);
	}

	m_checkNewPage.SetCheck(bPreviousNewPage);
	m_checkSummary.SetCheck(bPreviousSummary);

	switch (nPreviousSalesType)
	{
	case 1:
		OnRadioSalesItem();
		break;

	case 2:
		OnRadioSalesRenewal();
		break;

	case 0:
	default:
		OnRadioSalesAll();
		break;
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

	// audit period and time range

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

	//groups
	CGroupComboHelpers::FillGroupCombo(m_comboGroup, nPreviousSingleGroupNo, TRUE);

	if (m_comboGroup.GetCount() < 3)		// no individual groups added
	{
		m_comboGroup.EnableWindow(FALSE);
	}

	//card range

	SetCardRangeType(nPreviousCardRangeType);
	m_editFromCardNo.SetWindowText(strPreviousCardFrom);
	m_editToCardNo.SetWindowText(strPreviousCardTo);

	// terminal list
	m_ReportLocationControls.Init(nPreviousDbNo, nPreviousFolderSet, nPreviousTerminalListType, TRUE);

	// other

	{
		CString strReportLabel = m_ReportInfo.GetReportLabel();
		if (strReportLabel == "R105")					// single history
		{
			m_radioRange.EnableWindow(FALSE);
			m_radioList.EnableWindow(FALSE);
			m_radioSingle.EnableWindow(FALSE);
		}
	}

	SetRangeOptions();

	//altkey

	m_buttonAltKey1.ShowWindow(SW_HIDE);

	return TRUE;
}

//$$******************************************************************

void CReportPurchaseHistoryDlg::OnSelectDateRange() 
{
	m_DateSelectorRange.UpdateDateControls();
}

//$$******************************************************************

void CReportPurchaseHistoryDlg::OnChangeDateFrom(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_DateSelectorRange.OnChangeFromDate();
	m_DateSelectorRange.CheckForChangedDate();
	*pResult = 0;
}

/**********************************************************************/

void CReportPurchaseHistoryDlg::OnChangeDateTo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_DateSelectorRange.CheckForChangedDate();
	*pResult = 0;
}

/**********************************************************************/

void CReportPurchaseHistoryDlg::OnSelectTimeRange() 
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
void CReportPurchaseHistoryDlg::OnRadioRange() { SetCardRangeType(0); }
void CReportPurchaseHistoryDlg::OnRadioSingle() { SetCardRangeType(1); }
void CReportPurchaseHistoryDlg::OnRadioList() { SetCardRangeType(2); }
//$$******************************************************************

void CReportPurchaseHistoryDlg::SetCardRangeType( int n )
{
	if ( ( n < 0 ) || ( n > 2 ) )
		n = 0;

	m_radioRange.SetCheck( 0 == n );
	m_radioSingle.SetCheck( 1 == n );
	m_radioList.SetCheck( 2 == n );

	if ( 0 == n )
	{
		m_editFromCardNo.SetWindowText( System.GetLowCardNo() );
		m_editToCardNo.SetWindowText( System.GetHighCardNo() );
	}

	m_ReportInfo.SetCardRangeType(n);
	SetRangeOptions();
}

//$$******************************************************************

void CReportPurchaseHistoryDlg::SetRangeOptions()
{
	CString strReportLabel = m_ReportInfo.GetReportLabel();
	bool bAllowed = ( strReportLabel != "R105" ); //single history

	bool bAllowFrom		= bAllowed;
	bool bAllowTo		= bAllowed;
	bool bAllowList		= bAllowed;
	bool bAllowGroup	= ( m_comboGroup.GetCount() > 2 ) ? bAllowed : FALSE;
	bool bAllowNewPage	= bAllowed;

	switch ( m_ReportInfo.GetCardRangeType() )
	{
	case nREPORT_RANGE:		bAllowList	 = FALSE;
							break;

	case nREPORT_SINGLE:	bAllowFrom		= FALSE;
							bAllowList		= FALSE;
							bAllowGroup		= FALSE;
							bAllowNewPage	= FALSE;
							break;

	case nREPORT_LIST:		bAllowTo		= FALSE;
							bAllowFrom		= FALSE;
							bAllowGroup		= FALSE;
							break;
	}

	m_comboGroup.EnableWindow(bAllowGroup);
	m_editFromCardNo.EnableWindow(bAllowFrom);
	m_editToCardNo.EnableWindow(bAllowTo);
	m_buttonList.EnableWindow(bAllowList);
	m_checkNewPage.EnableWindow(bAllowNewPage);

	switch ( m_ReportInfo.GetCardRangeType() )
	{
	case nREPORT_RANGE:		m_editFromCardNo.SetFocus();	break;
	case nREPORT_SINGLE:	m_editToCardNo.SetFocus();		break;
	case nREPORT_LIST:		m_buttonList.SetFocus();		break;
	}
}

//$$******************************************************************

void CReportPurchaseHistoryDlg::OnDoubleClickRadioSingle()
{
	CFindDlg dlg{};
	if ( dlg.DoModal() == IDOK )
	{
		m_editToCardNo.SetWindowText(SimpleFormatInt64Value(dlg.m_nRequestedCardNo));
	}
}

//$$******************************************************************

void CReportPurchaseHistoryDlg::OnDoubleClickRadioList() 
{ 
	OnButtonList();
}

//$$******************************************************************

void CReportPurchaseHistoryDlg::OnButtonList()
{
	CArray<CSQLRowAccountList, CSQLRowAccountList> arrayDummy;
	CString strReportList = Filenames.GetReportCardListFilename(m_ReportInfo.GetReportLabel());
	CCardPickerDlg dlg("Purchase History", strReportList, arrayDummy, this);
	dlg.m_bValidAllowed = TRUE;
	dlg.m_bExpiredAllowed = TRUE;
	dlg.m_bHotlistedAllowed = TRUE;

	if (dlg.DoModal() == IDOK)
	{
		m_ReportInfo.ResetCardPicker();
	}
}

//$$******************************************************************

void CReportPurchaseHistoryDlg::OnSelectDatabase()
{
	m_ReportLocationControls.SelectDatabase();
}

//*******************************************************************

void CReportPurchaseHistoryDlg::OnSelectFolderSet()
{
	m_ReportLocationControls.SelectFolderSet();
}

//*******************************************************************

void CReportPurchaseHistoryDlg::OnToggleTerminals()
{
	m_ReportLocationControls.ToggleTerminals();
}

//$$******************************************************************

void CReportPurchaseHistoryDlg::OnButtonAltKey1()
{
	m_ReportLocationControls.DeleteSelectedTerminalList();	
}

//$$******************************************************************
void CReportPurchaseHistoryDlg::OnRadioSalesAll() { SetSalesType(0); }
void CReportPurchaseHistoryDlg::OnRadioSalesItem() { SetSalesType(1); }
void CReportPurchaseHistoryDlg::OnRadioSalesRenewal() { SetSalesType(2); }
//$$******************************************************************

void CReportPurchaseHistoryDlg::SetSalesType(int n)
{
	if ((n < 0) || (n > 2))
	{
		n = 0;
	}

	m_nSalesType = n;
	m_radioSalesAll.SetCheck(0 == n);
	m_radioSalesItem.SetCheck(1 == n);
	m_radioSalesRenewal.SetCheck(2 == n);
}

//$$******************************************************************

void CReportPurchaseHistoryDlg::OnOK()
{
	if (UpdateData() == TRUE)
	{
		CString strTerminalList = "";
		int nDbNo = 0;
		int nFolderSet = 0;
		bool bWantTerminals = FALSE;
		m_ReportLocationControls.GetSettings(nDbNo, nFolderSet, bWantTerminals, strTerminalList);

		if ((TRUE == bWantTerminals) && (strTerminalList == ""))
		{
			Prompter.FatalError("No EPOS terminal numbers have been specified!", "Entry Error");
			m_comboTerminals.SetFocus();
			return;
		}

		{
			CTime dateFrom;
			m_DatePickerFrom.GetTime(dateFrom);
			m_ReportInfo.SetDateFrom(dateFrom);
		}

		{
			CTime dateTo;
			m_DatePickerTo.GetTime(dateTo);
			m_ReportInfo.SetDateTo(dateTo);
		}

		{
			CTime timeFrom;
			m_TimePickerFrom.GetTime(timeFrom);
			CString strStartTime = timeFrom.Format("%H:%M");
			m_ReportInfo.SetTimeFrom(timeFrom);

			CTime timeTo;
			m_TimePickerTo.GetTime(timeTo);
			CString strEndTime = timeTo.Format("%H:%M");
			m_ReportInfo.SetTimeTo(timeTo);

			m_ReportInfo.SetTimeRangeFlag(strStartTime != "00:00" || strEndTime != "23:59");
		}

		m_ReportInfo.SetNewPageFlag(m_checkNewPage.GetCheck() != 0);
		m_ReportInfo.SetSummaryFlag(m_checkSummary.GetCheck() != 0);
		m_ReportInfo.SetSalesTypeFilter(m_nSalesType);

		{
			int nGroupNo = CGroupComboHelpers::GetGroupNoFromCombo(m_comboGroup, 0);
			m_ReportInfo.SetSingleGroupNo(nGroupNo);
			m_ReportInfo.SetGotSingleGroupFlag(0 != nGroupNo);
		}

		{
			CString strFromCardNo;
			m_editFromCardNo.GetWindowText(strFromCardNo);

			CString strToCardNo;
			m_editToCardNo.GetWindowText(strToCardNo);

			m_ReportInfo.SetRangeToCardNo(_atoi64(strToCardNo));

			if (nREPORT_SINGLE == m_ReportInfo.GetCardRangeType())
			{
				m_ReportInfo.SetRangeFromCardNo(_atoi64(strToCardNo));
			}
			else
			{
				m_ReportInfo.SetRangeFromCardNo(_atoi64(strFromCardNo));
			}
		}

		m_ReportInfo.SetDbNo(nDbNo);
		m_ReportInfo.SetFolderSet(nFolderSet);

		{
			CString strDateRangeType;
			m_comboDateRange.GetWindowText(strDateRangeType);

			CCSV csv;
			csv.Add(m_ReportInfo.GetCardRangeType());
			csv.Add(m_ReportInfo.GetRangeFromCardNo());
			csv.Add(m_ReportInfo.GetRangeToCardNo());
			csv.Add(m_ReportInfo.GetNewPageFlag());
			csv.Add(strDateRangeType);
			csv.Add(m_comboTimeRange.GetCurSel());
			csv.Add(m_ReportInfo.GetSummaryFlag());
			csv.Add(m_ReportInfo.GetSingleGroupNo());
			csv.Add(m_ReportInfo.GetTerminalListType());
			csv.Add(0); //LEGACY RENEWAL FLAG
			csv.Add(m_ReportInfo.GetDateFromString());
			csv.Add(m_ReportInfo.GetDateToString());
			csv.Add(m_ReportInfo.GetTimeFromString());
			csv.Add(m_ReportInfo.GetTimeToString());
			csv.Add(m_ReportInfo.GetSalesTypeFilter());
			csv.Add(m_ReportInfo.GetDbNo());
			csv.Add(m_ReportInfo.GetFolderSet());

			CReportDefaultHandler repdefs;
			repdefs.Save(m_ReportInfo.GetReportLabel(), &csv);

			CCSV csvReportKey;
			csvReportKey.Add(m_ReportInfo.GetCardRangeType());
			csvReportKey.Add(m_ReportInfo.GetSummaryFlag());
			csvReportKey.Add(m_ReportInfo.GetSingleGroupNo());
			m_ReportInfo.SetReportKey(csvReportKey.GetLine());
		}

		if (m_ReportInfo.GetTerminalListType() == 1)
		{
			m_ReportInfo.TNoListSetMaxLength(strTerminalList.GetLength());	// stop entry being truncated
			m_ReportInfo.TNoListSave(strTerminalList);						// save terminal list
			m_ReportInfo.TNoArrayRemoveAll();
			m_ReportInfo.TNoListExtractTNos(strTerminalList);			// extract terminal numbers
		}

		{
			CString strReportTitle = "Purchase History";

			if (m_ReportInfo.GetSummaryFlag())
			{
				strReportTitle += " Summary";
			}

			CString strBuf;
			{
				CString strDateFrom = m_ReportInfo.GetDateFromString();
				CString strDateTo = m_ReportInfo.GetDateToString();

				if (strDateFrom != strDateTo)
				{
					strBuf.Format(" (%s - %s)", (const char*)strDateFrom, (const char*)strDateTo);
				}
				else
				{
					strBuf.Format(" (%s)", (const char*)strDateFrom);
				}
			}

			strReportTitle += strBuf;

			if (m_ReportInfo.GetTimeRangeFlag() == TRUE)
			{
				strBuf.Format(" (%s - %s)", m_ReportInfo.GetTimeFrom().GetTime(), m_ReportInfo.GetTimeTo().GetTime());
				strReportTitle += strBuf;
			}

			m_ReportInfo.SetReportTitle(strReportTitle);
		}

		EndDialog(IDOK);
	}
}

//$$******************************************************************
