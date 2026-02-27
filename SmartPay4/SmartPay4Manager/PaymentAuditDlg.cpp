//$$******************************************************************
#include "ReportDefaultHandler.h"
#include "StandardCombos.h"
//$$******************************************************************
#include "PaymentAuditDlg.h"
//$$******************************************************************

CPaymentAuditDlg::CPaymentAuditDlg(const char* szLabel, CWnd* pParent)
	: CSSAutoShutdownDialog(AutoShutdownHelper, CPaymentAuditDlg::IDD, pParent)
	, m_DateSelectorRange(m_comboDateRange, m_DatePickerFrom, m_DatePickerTo)
	, m_TerminalListHandler(Filenames.GetTerminalListFilename())
	, m_ReportLocationControls(m_comboFolderSet, m_checkTerminals, m_comboTerminals, &m_TerminalListHandler)
{
	m_strReportLabel = szLabel;
	m_strReportTitle = "Epos Payment Audit";
	m_strReportFilename = Filenames.GetReportFilename(m_strReportLabel);
	m_strParamsFilename = Filenames.GetReportParamsFilename(m_strReportLabel);
	m_strReportKey = "";

	m_dateTo.SetCurrentDate();
	m_dateFrom.SetCurrentDate();
	m_dateAllFrom.SetCurrentDate();
}

//$$******************************************************************

CPaymentAuditDlg::~CPaymentAuditDlg()
{
}

//$$******************************************************************

void CPaymentAuditDlg::DoDataExchange(CDataExchange* pDX)
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
	DDX_Control(pDX, IDC_COMBO_PAYMENTS, m_comboPayments);
	DDX_Control(pDX, IDC_RADIO_PAYMENTALL, m_radioPaymentAll);
	DDX_Control(pDX, IDC_RADIO_PAYMENTLIST, m_radioPaymentList);
	DDX_Control(pDX, IDC_CHECK_SUMMARY, m_checkSummary);
	DDX_Control(pDX, IDC_COMBO_ORDERBY, m_comboOrderBy);
	DDX_Control(pDX, IDC_CHECK_SHOWEODLINES, m_checkEODLines);
	DDX_Control(pDX, IDC_BUTTON_ALTKEY1, m_buttonAltKey1);
	DDX_Control(pDX, IDC_BUTTON_ALTKEY2, m_buttonAltKey2);
	DDX_Control(pDX, IDC_BUTTON_ALTKEY3, m_buttonAltKey3);
}

//*******************************************************

BEGIN_MESSAGE_MAP(CPaymentAuditDlg, CSSAutoShutdownDialog)
	ON_CBN_SELCHANGE(IDC_COMBO_DATERANGE, OnSelectDateRange)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER1, OnChangeDateFrom)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER2, OnChangeDateTo)
	ON_CBN_SELCHANGE(IDC_COMBO_FOLDERSET, OnSelectFolderSet)
	ON_BN_CLICKED(IDC_CHECK_TERMINALS, OnToggleTerminals)
	ON_BN_CLICKED(IDC_RADIO_PAYMENTALL, OnRadioPaymentAll)
	ON_BN_CLICKED(IDC_RADIO_PAYMENTLIST, OnRadioPaymentList)
	ON_BN_CLICKED(IDC_BUTTON_ALTKEY1, OnButtonAltKey1)
	ON_BN_CLICKED(IDC_BUTTON_ALTKEY2, OnButtonAltKey2)
	ON_BN_CLICKED(IDC_BUTTON_ALTKEY3, OnButtonAltKey3)
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()

//$$******************************************************************

BOOL CPaymentAuditDlg::CSSAutoShutdownPostInitDialog()
{
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

	m_PaymentList.ReadComments(Filenames.GetPaymentListFilename());

	int nOrderBy = 0;
	int nFolderSet = 0;
	int nTerminalListType = 0;
	int nPaymentSelectType = 0;
	bool bSummary = FALSE;
	bool bEODLines = FALSE;
	bool bContinuous = TRUE;

	CString strLine;
	CReportDefaultHandler repdefs;
	if (repdefs.Read(m_strReportLabel, strLine) == TRUE)
	{
		CCSV csv(strLine);
		//0 WAS DATE SELECTION
		if (csv.IsEmpty(1) == FALSE)	strTimeStart = csv.GetString(1);
		if (csv.IsEmpty(2) == FALSE)	strTimeEnd = csv.GetString(2);
		if (csv.IsEmpty(3) == FALSE)	nOrderBy = csv.GetInt(3);
		if (csv.IsEmpty(4) == FALSE)	nTerminalListType = csv.GetInt(4);
		if (csv.IsEmpty(5) == FALSE)	nPaymentSelectType = csv.GetInt(5);
		if (csv.IsEmpty(6) == FALSE)	bSummary = csv.GetInt(6);
		if (csv.IsEmpty(7) == FALSE)	bEODLines = csv.GetBool(7);
		if (csv.IsEmpty(8) == FALSE)	strDateRangeType = csv.GetString(8);
		if (csv.IsEmpty(9) == FALSE)	strDateFrom = csv.GetString(9);
		if (csv.IsEmpty(10) == FALSE)	strDateTo = csv.GetString(10);
		if (csv.IsEmpty(11) == FALSE)	bContinuous = csv.GetBool(11);
		if (csv.IsEmpty(12) == FALSE)	nFolderSet = csv.GetInt(12);
	}

	CStandardCombos::FillPaymentOrderCombo( m_comboOrderBy, nOrderBy );

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

	// payment list
	if ((nPaymentSelectType < 0) || (nPaymentSelectType > 1))
	{
		nPaymentSelectType = 0;
	}
	m_radioPaymentAll.SetCheck(0 == nPaymentSelectType);
	m_radioPaymentList.SetCheck(1 == nPaymentSelectType);
	AddPayments();
	
	m_checkEODLines.SetCheck(bEODLines);
	if (FALSE == bEODLines)
	{
		m_checkEODLines.ShowWindow(SW_HIDE);
	}

	m_checkSummary.SetCheck(bSummary);

	SetDisplay();

	m_buttonAltKey1.ShowWindow(SW_HIDE);
	m_buttonAltKey2.ShowWindow(SW_HIDE);
	m_buttonAltKey3.ShowWindow(SW_HIDE);

	return TRUE;
}

//$$******************************************************************

void CPaymentAuditDlg::OnSelectDateRange()
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

void CPaymentAuditDlg::OnChangeDateFrom(NMHDR* pNMHDR, LRESULT* pResult)
{
	m_DateSelectorRange.OnChangeFromDate();
	m_DateSelectorRange.CheckForChangedDate();
	*pResult = 0;
}

//$$******************************************************************

void CPaymentAuditDlg::OnChangeDateTo(NMHDR* pNMHDR, LRESULT* pResult)
{
	m_DateSelectorRange.CheckForChangedDate();
	*pResult = 0;
}

//$$******************************************************************

void CPaymentAuditDlg::OnSelectFolderSet()
{
	m_ReportLocationControls.SelectFolderSet();
}

//*******************************************************************

void CPaymentAuditDlg::OnToggleTerminals()
{
	m_ReportLocationControls.ToggleTerminals();
}

//$$******************************************************************

void CPaymentAuditDlg::OnButtonAltKey1()
{
	m_ReportLocationControls.DeleteSelectedTerminalList();
}

//$$******************************************************************

void CPaymentAuditDlg::AddPayments()
{
	for (int i = 0; i < m_PaymentList.GetSize(); i++)
	{
		m_comboPayments.AddString(m_PaymentList.GetAt(i));
	}
	m_comboPayments.SetCurSel(0);
}

//$$******************************************************************

CString CPaymentAuditDlg::GetDateString()
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

void CPaymentAuditDlg::OnButtonAltKey2()
{
	int nPaymentList = (IsTicked(m_radioPaymentList) ? 1 : 0);

	CString strPaymentList;
	m_comboPayments.GetWindowText(strPaymentList);

	if ((strPaymentList != "") && (nPaymentList == 1))
	{
		m_PaymentList.Delete(strPaymentList);			// delete comment
		m_comboPayments.ResetContent();					// new list
		AddPayments();
	}
}

//$$******************************************************************

void CPaymentAuditDlg::OnButtonAltKey3()			
{ 
	m_checkEODLines.ShowWindow(SW_SHOW);	
}

//$$******************************************************************

void CPaymentAuditDlg::OnRadioPaymentAll()		
{ 
	SetDisplay(); 
}

//$$******************************************************************

void CPaymentAuditDlg::OnRadioPaymentList()
{
	SetDisplay();
	m_comboPayments.SetFocus();
}

//$$******************************************************************

void CPaymentAuditDlg::SetDisplay()
{
	m_comboPayments.EnableWindow(IsTicked(m_radioPaymentList));
}

//$$******************************************************************

bool CPaymentAuditDlg::IsValid ( CPaymentAuditRecord* atcRecord  )
{
	CSSDate dateAudit ( atcRecord->GetDateFiled() );
	CSSTime timeAudit ( atcRecord->GetTimeFiled() );

	if (dateAudit < m_dateFrom)							// not in range
	{
		return FALSE;
	}

	if (dateAudit > m_dateTo)								// not in range
	{
		return FALSE;										// skip
	}

	if (TRUE == m_bTimeRangeSet)
	{
		if (FALSE == m_bContinuous)
		{
			if (timeAudit < m_timeStart)
			{
				return FALSE;
			}

			if (timeAudit > m_timeEnd)
			{
				return FALSE;
			}
		}
		else
		{
			if (dateAudit == m_dateFrom)
			{
				if (timeAudit < m_timeStart)					// not in range
				{
					return FALSE;
				}
			}

			if (dateAudit == m_dateTo)
			{
				if (timeAudit > m_timeEnd)							// not in range
				{
					return FALSE;
				}
			}
		}
	}

// in date \ time range

	if (atcRecord->IsEODEntry() == TRUE)
	{
		if (m_nOrderBy == 0)
		{
			return (m_bEODLines != 0);	// only use when listing by date
		}
		else
		{
			return FALSE;
		}
	}

	if (m_TerminalListHandler.CheckTerminalFilter(atcRecord->GetFolderSet(), atcRecord->GetTerminalNo()) == FALSE)
	{
		return FALSE;
	}	

	if ( m_nPaymentListType == 1 )								// payment list
	{
		bool bFound = FALSE;
		int n = atcRecord->GetPaymentType();
		for ( int i = 0 ; i < m_arrayPayment.GetCount() ; i++ )
		{
			if ( n == m_arrayPayment.GetAt(i) )				// see if in list
			{
				bFound = TRUE;
				break;
			}
		}

		if (FALSE == bFound)
		{
			return FALSE;
		}
	}

	return TRUE;
}

//$$******************************************************************

void CPaymentAuditDlg::OnOK()
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
	m_TerminalListHandler.SetTerminalSelectType(bWantTerminals ? 1 : 0 );

	m_nPaymentListType = IsTicked(m_radioPaymentList) ? 1 : 0;

	CString strPaymentList = "";
	if ( 1 == m_nPaymentListType )
	{
		m_comboPayments.GetWindowText(strPaymentList);

		if (strPaymentList == "")
		{
			Prompter.FatalError("No payment types have been specified", "Entry Error");
			m_comboPayments.SetFocus();
			return;
		}
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

	m_nOrderBy = m_comboOrderBy.GetCurSel();
	m_bSummaryReqd = IsTicked(m_checkSummary);
	m_bEODLines = IsTicked(m_checkEODLines);
	m_bContinuous = IsTicked(m_checkContinuous);

	CString strDateRangeType;
	m_comboDateRange.GetWindowText(strDateRangeType);

	CCSV csv;
	csv.Add(0); //WAS DATE SELECTION
	csv.Add(strStartTime);
	csv.Add(strEndTime);
	csv.Add(m_nOrderBy);
	csv.Add(m_nTerminalListType);
	csv.Add(m_nPaymentListType);
	csv.Add(m_bSummaryReqd);
	csv.Add(m_bEODLines);
	csv.Add(strDateRangeType);
	csv.Add(m_dateFrom.GetDate());
	csv.Add(m_dateTo.GetDate());
	csv.Add(m_bContinuous);
	csv.Add(nFolderSet);

	CReportDefaultHandler repdefs;
	repdefs.Save(m_strReportLabel, &csv);

	CCSV csvReportKey;
	csvReportKey.Add(m_nOrderBy);
	csvReportKey.Add(m_nTerminalListType);
	csvReportKey.Add(m_nPaymentListType);
	csvReportKey.Add(m_bSummaryReqd);
	csvReportKey.Add(m_bEODLines);
	m_strReportKey = csvReportKey.GetLine();

	if (1 == m_nTerminalListType)
	{
		m_TerminalListHandler.TNoListSetMaxLength(strTerminalList.GetLength());	// stop entry being truncated
		m_TerminalListHandler.TNoListSave(strTerminalList);						// save terminal list

		m_TerminalListHandler.TNoArrayRemoveAll();
		m_TerminalListHandler.TNoListExtractTNos(strTerminalList);			// extract terminal numbers
	}

	if (1 == m_nPaymentListType)
	{
		m_PaymentList.SetMaxLength(strPaymentList.GetLength());	// stop entry being truncated
		m_PaymentList.Save(strPaymentList);						// save terminal list

		m_arrayPayment.RemoveAll();
		m_PaymentList.ExtractTNos(strPaymentList, &m_arrayPayment);		// extract numbers
	}

	EndDialog(IDOK);
}

//$$******************************************************************
