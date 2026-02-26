//$$******************************************************************
#include "..\SPosLoyalty4Shared\SQLTable_AuditPeriod\SQLRowSetAuditPeriod.h"
//$$******************************************************************
#include "LoyaltyManager.h"
#include "ReportDefaultHandler.h"
//$$******************************************************************
#include "ReportGroupPeriodSalesDlg.h"
//$$******************************************************************

CReportGroupPeriodSalesDlg::CReportGroupPeriodSalesDlg( CWnd* pParent)
	: CSSDialog(CReportGroupPeriodSalesDlg::IDD, pParent)
	, m_DateSelectorRange( m_comboDateRange, m_DatePickerFrom, m_DatePickerTo )
	, m_ReportLocationControls(m_comboDatabase, m_comboFolderSet, m_checkTerminals, m_comboTerminals, NULL)
{
	m_strReportLabel = "R118";
	m_nSalesType = 0;
	m_pReportInfo = NULL;
	SQLRowSetAuditPeriod.LoadPeriods(NULL);
}

//$$******************************************************************

void CReportGroupPeriodSalesDlg::DoDataExchange(CDataExchange* pDX)
{
	CSSDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_RANGE, m_staticRange);
	//date
	DDX_Control(pDX, IDC_COMBO_DATERANGE, m_comboDateRange);
	DDX_Control(pDX, IDC_DATEPICKER_FROM, m_DatePickerFrom);
	DDX_Control(pDX, IDC_DATEPICKER_TO, m_DatePickerTo);
	//Options
	DDX_Control(pDX, IDC_CHECK_SUMMARY, m_checkSummary);
	DDX_Control(pDX, IDC_RADIO_SALES_ALL, m_radioSalesAll);
	DDX_Control(pDX, IDC_RADIO_SALES_ITEM, m_radioSalesItem);
	DDX_Control(pDX, IDC_RADIO_SALES_RENEWAL, m_radioSalesRenewal);
	DDX_Control(pDX, IDC_CHECK_MANUAL, m_checkManual);
	DDX_Control(pDX, IDC_CHECK_DISCOUNT, m_checkDiscount);
	//terminals
	DDX_Control(pDX, IDC_COMBO_DATABASE, m_comboDatabase);
	DDX_Control(pDX, IDC_COMBO_FOLDERSET, m_comboFolderSet);
	DDX_Control(pDX, IDC_CHECK_TERMINALS, m_checkTerminals);
	DDX_Control(pDX, IDC_COMBO_TERMINALS, m_comboTerminals);
	DDX_Control(pDX, IDC_BUTTON_ALTKEY1, m_buttonAltKey1);
	//Close buttons
	DDX_Control(pDX, IDOK, m_buttonOK);
	DDX_Control(pDX, IDCANCEL, m_buttonCancel);
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CReportGroupPeriodSalesDlg, CSSDialog)

	//date
	ON_CBN_SELCHANGE(IDC_COMBO_DATERANGE, OnSelectDateRange)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATEPICKER_FROM, OnChangeDateFrom)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATEPICKER_TO, OnChangeDateTo)
	//other
	ON_BN_CLICKED(IDC_RADIO_SALES_ALL, OnRadioSalesAll)
	ON_BN_CLICKED(IDC_RADIO_SALES_ITEM, OnRadioSalesItem)
	ON_BN_CLICKED(IDC_RADIO_SALES_RENEWAL, OnRadioSalesRenewal)
	//terminals
	ON_CBN_SELCHANGE(IDC_COMBO_DATABASE, OnSelectDatabase)
	ON_CBN_SELCHANGE(IDC_COMBO_FOLDERSET, OnSelectFolderSet)
	ON_BN_CLICKED(IDC_CHECK_TERMINALS, OnToggleTerminals)
	//altkey
	ON_BN_CLICKED(IDC_BUTTON_ALTKEY1, OnButtonAltKey1)

END_MESSAGE_MAP()

//$$******************************************************************

BOOL CReportGroupPeriodSalesDlg::OnInitDialog()
{
	CSSDialog::OnInitDialog();
	m_buttonAltKey1.ShowWindow(SW_HIDE);
	SetWindowText(m_pReportInfo->GetReportTitle());
	m_ReportLocationControls.SetTerminalListHandler(m_pReportInfo->GetTerminalListHandler());

	bool bPreviousSummary = FALSE;
	CString strPreviousDateType = "";
	CString strPreviousDateFrom = "";
	CString strPreviousDateTo = "";
	int nSalesType = 0;
	bool bManual = FALSE;
	bool bDiscount = FALSE;
	int nDbNo = 0;
	int nFolderSet = 0;
	int nTerminalSelectType = 0;

	CString strLine;
	CReportDefaultHandler repdefs;
	if (repdefs.Read( m_strReportLabel, strLine) == TRUE)
	{
		CCSV csv(strLine);
		if (csv.IsEmpty(0) == FALSE)	strPreviousDateType = csv.GetString(0);
		//if (csv.IsEmpty(1) == FALSE)	nPreviousTimeCurSel = csv.GetInt(1);
		if (csv.IsEmpty(2) == FALSE)	bPreviousSummary = csv.GetBool(2);
		if (csv.IsEmpty(3) == FALSE)	strPreviousDateFrom = csv.GetString(3);
		if (csv.IsEmpty(4) == FALSE)	strPreviousDateTo = csv.GetString(4);
		//if (csv.IsEmpty(5) == FALSE)	strPreviousTimeFrom = csv.GetString(5);
		//if (csv.IsEmpty(6) == FALSE)	strPreviousTimeTo = csv.GetString(6);
		if (csv.IsEmpty(7) == FALSE)	nSalesType = csv.GetInt(7);
		if (csv.IsEmpty(8) == FALSE)	bManual = csv.GetInt(8);
		if (csv.IsEmpty(9) == FALSE)	nDbNo = csv.GetInt(9);
		if (csv.IsEmpty(10) == FALSE)	nFolderSet = csv.GetInt(10);
		if (csv.IsEmpty(11) == FALSE)	nTerminalSelectType = csv.GetInt(11);
		if (csv.IsEmpty(12) == FALSE)	bDiscount = csv.GetInt(12);
	}

	m_checkSummary.SetCheck(bPreviousSummary);
	m_checkManual.SetCheck(bManual);
	m_checkDiscount.SetCheck(bDiscount);
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

	switch (nSalesType)
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

	// terminal list
	m_ReportLocationControls.Init(nDbNo, nFolderSet, nTerminalSelectType, TRUE);

	return TRUE;
}

//$$******************************************************************

void CReportGroupPeriodSalesDlg::OnSelectDateRange()
{
	m_DateSelectorRange.UpdateDateControls();
}

//$$******************************************************************

void CReportGroupPeriodSalesDlg::OnChangeDateFrom(NMHDR* pNMHDR, LRESULT* pResult)
{
	m_DateSelectorRange.OnChangeFromDate();
	m_DateSelectorRange.CheckForChangedDate();
	*pResult = 0;
}

//$$******************************************************************

void CReportGroupPeriodSalesDlg::OnChangeDateTo(NMHDR* pNMHDR, LRESULT* pResult)
{
	m_DateSelectorRange.CheckForChangedDate();
	*pResult = 0;
}

//$$******************************************************************
void CReportGroupPeriodSalesDlg::OnRadioSalesAll() { SetSalesType(0); }
void CReportGroupPeriodSalesDlg::OnRadioSalesItem() { SetSalesType(1); }
void CReportGroupPeriodSalesDlg::OnRadioSalesRenewal() { SetSalesType(2); }
//$$******************************************************************

void CReportGroupPeriodSalesDlg::SetSalesType(int n)
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

void CReportGroupPeriodSalesDlg::OnSelectDatabase()
{
	m_ReportLocationControls.SelectDatabase();
}

//*******************************************************************

void CReportGroupPeriodSalesDlg::OnSelectFolderSet()
{
	m_ReportLocationControls.SelectFolderSet();
}

//*******************************************************************

void CReportGroupPeriodSalesDlg::OnToggleTerminals()
{
	m_ReportLocationControls.ToggleTerminals();
}

//$$******************************************************************

void CReportGroupPeriodSalesDlg::OnButtonAltKey1()
{
	m_ReportLocationControls.DeleteSelectedTerminalList();
}

//$$******************************************************************

void CReportGroupPeriodSalesDlg::OnOK()
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
			m_pReportInfo->SetDateFrom(dateFrom);
		}

		{
			CTime dateTo;
			m_DatePickerTo.GetTime(dateTo);
			m_pReportInfo->SetDateTo(dateTo);
		}

		m_pReportInfo->SetDbNo(nDbNo);
		m_pReportInfo->SetFolderSet(nFolderSet);
		m_pReportInfo->SetTerminalSelectType(bWantTerminals ? 1 : 0);
		m_pReportInfo->SetSummaryFlag(IsTicked(m_checkSummary));
		m_pReportInfo->SetManualSpendFlag(IsTicked(m_checkManual));
		m_pReportInfo->SetDiscountFlag(IsTicked(m_checkDiscount));
		m_pReportInfo->SetSalesTypeFilter(m_nSalesType);

		{
			CString strDateRangeType;
			m_comboDateRange.GetWindowText(strDateRangeType);

			CCSV csv;
			csv.Add(strDateRangeType);
			csv.Add(0); // TIME SELECTION
			csv.Add(m_pReportInfo->GetSummaryFlag());
			csv.Add(m_pReportInfo->GetDateFromString());
			csv.Add(m_pReportInfo->GetDateToString());
			csv.Add(""); //TIME FROM
			csv.Add(""); //TIME TO
			csv.Add(m_nSalesType);
			csv.Add(m_pReportInfo->GetManualSpendFlag());
			csv.Add(m_pReportInfo->GetDbNo());
			csv.Add(m_pReportInfo->GetFolderSet());
			csv.Add(m_pReportInfo->GetTerminalSelectType());
			csv.Add(m_pReportInfo->GetDiscountFlag());

			CReportDefaultHandler repdefs;
			repdefs.Save(m_strReportLabel, &csv);

			CCSV csvReportKey;
			csvReportKey.Add(m_pReportInfo->GetSummaryFlag());
			m_pReportInfo->SetReportKey(csvReportKey.GetLine());
		}
	}

	EndDialog(IDOK);
}

//$$******************************************************************
