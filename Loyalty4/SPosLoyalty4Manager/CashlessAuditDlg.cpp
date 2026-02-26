//$$******************************************************************
#include "ReportDefaultHandler.h"
//$$******************************************************************
#include "LoyaltyManager.h"
#include "RepmanHandler.h"
#include "ReportCashlessLiability.h"
#include "ReportCashlessPosting.h"
//$$******************************************************************
#include "CashlessAuditDlg.h"
//$$******************************************************************

CCashlessAuditDlg::CCashlessAuditDlg( void * pReport, CWnd* pParent )
	: CSSDialog(CCashlessAuditDlg::IDD, pParent)
	, m_DateSelectorRange( m_comboDateRange, m_DatePickerFrom, m_DatePickerTo )
{
	m_pReportInfoCashless = NULL;
	m_pReport = pReport;
	m_nFinalError = nREPORT_NOERROR;
}

//$$******************************************************************

void CCashlessAuditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	
	//date
	DDX_Control(pDX, IDC_COMBO_DATERANGE, m_comboDateRange);
	DDX_Control(pDX, IDC_DATEPICKER_FROM, m_DatePickerFrom);
	DDX_Control(pDX, IDC_DATEPICKER_TO, m_DatePickerTo);
	//time
	DDX_Control(pDX, IDC_TIMEPICKER_CUTOFF, m_TimePickerCutoff);
	DDX_Control(pDX, IDC_COMBO_NEXTDAY, m_comboNextDay);
	//lookback
	DDX_Control(pDX, IDC_COMBO_REFUND, m_comboRefund);
	DDX_Control(pDX, IDC_STATIC_PREVIOUS1, m_staticPrevious1);
	DDX_Control(pDX, IDC_STATIC_PREVIOUS2, m_staticPrevious2);
	DDX_Control(pDX, IDC_DATEPICKER_PREVIOUS, m_DatePickerPrevious);
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CCashlessAuditDlg, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO_DATERANGE, OnSelectDateRange)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATEPICKER_FROM, OnChangeDateFrom)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATEPICKER_TO, OnChangeDateTo)
END_MESSAGE_MAP()

//$$******************************************************************

BOOL CCashlessAuditDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CString strPreviousDateType = "";
	CString strPreviousDateFrom = "";
	CString strPreviousDateTo = "";
	CString strPreviousTimeCutoff = "";
	CString strLookbackDate = "";
	bool bCutoffNextDay = FALSE;
	int nRefundProcessType = 0;

	CReportDefaultHandler repdefs;

	CString strLine;
	if (repdefs.Read(m_pReportInfoCashless->GetReportLabel(), strLine) == TRUE)
	{
		CCSV csv(strLine);
		if (csv.IsEmpty(0) == FALSE)		strPreviousDateType = csv.GetString(0);
		if (csv.IsEmpty(1) == FALSE)		strPreviousTimeCutoff = csv.GetString(1);
		if (csv.IsEmpty(2) == FALSE)		strPreviousDateFrom = csv.GetString(2);
		if (csv.IsEmpty(3) == FALSE)		strPreviousDateTo = csv.GetString(3);
		if (csv.IsEmpty(4) == FALSE)		bCutoffNextDay = csv.GetBool(4);
		if (csv.IsEmpty(5) == FALSE)		strLookbackDate = csv.GetString(5);
		if (csv.IsEmpty(6) == FALSE)		nRefundProcessType = csv.GetInt(6);
	}

	SetWindowText ( m_pReportInfoCashless -> GetReportTitle() );

	// Date range
	m_DateSelectorRange.InitialiseDateControls();
	m_comboDateRange.AddString ( szRANGE_TODAY );
	m_comboDateRange.AddString ( szRANGE_YESTERDAY );
	m_comboDateRange.AddString ( szRANGE_THIS_WEEK );
	m_comboDateRange.AddString ( szRANGE_THIS_MONTH );
	m_comboDateRange.AddString ( szRANGE_THIS_YEAR );
	m_comboDateRange.AddString ( szRANGE_LAST_WEEK );
	m_comboDateRange.AddString ( szRANGE_LAST_MONTH );
	m_comboDateRange.AddString ( szRANGE_LAST_YEAR );
	m_comboDateRange.AddString ( szRANGE_LAST_3_MONTHS );
	m_comboDateRange.AddString ( szRANGE_LAST_6_MONTHS );
	m_comboDateRange.AddString ( szRANGE_LAST_9_MONTHS );
	m_comboDateRange.AddString ( szRANGE_LAST_12_MONTHS );
	m_comboDateRange.AddString ( szRANGE_ALL );
	m_comboDateRange.AddString ( szRANGE_CUSTOM_DATE );
	m_comboDateRange.AddString ( szRANGE_CUSTOM_RANGE );
	m_DateSelectorRange.SetComboSelection(strPreviousDateType);

	if ( ( strPreviousDateType == szRANGE_CUSTOM_DATE ) || ( strPreviousDateType == szRANGE_CUSTOM_RANGE ) )
	{
		CSSDate dateFrom( strPreviousDateFrom );
		CSSDate dateTo( strPreviousDateTo );
		
		if ( dateFrom.IsSet() )
		{
			m_DatePickerFrom.SetTime( COleDateTime( dateFrom.GetYear(), dateFrom.GetMonth(), dateFrom.GetDay(), 0, 0, 0 ) );
		}

		if ( dateTo.IsSet() )
		{
			m_DatePickerTo.SetTime( COleDateTime( dateTo.GetYear(), dateTo.GetMonth(), dateTo.GetDay(), 0, 0, 0 ) );
		}
	}

	//time
	m_TimePickerCutoff.SetFormat ( "HH:mm" );

	{
		CSSTime timeCutoff( strPreviousTimeCutoff );
		
		if ( timeCutoff.IsSet() )
		{
			m_TimePickerCutoff.SetTime( COleDateTime( 2019, 1, 1, timeCutoff.GetHour(), timeCutoff.GetMinute(), 0 ) );
		}
		else
		{
			m_TimePickerCutoff.SetTime( COleDateTime( 2019, 1, 1, 0, 0, 0 ) );
		}
	}

	m_comboNextDay.AddString("Same day");
	m_comboNextDay.AddString("Next day");
	m_comboNextDay.SetCurSel(bCutoffNextDay ? 1 : 0);

	m_comboRefund.AddString("Balance additions");
	m_comboRefund.AddString("Negative sales");
	m_comboRefund.SetCurSel(nRefundProcessType == 1 ? 1 : 0);

	{
		COleDateTime dateEnd = COleDateTime::GetCurrentTime();
		COleDateTime dateStart = COleDateTime(2000, 1, 1, 0, 0, 0);

		if (dateEnd.GetYear() < 2025)
		{
			dateEnd = COleDateTime(2025, 1, 1, 0, 0, 0);
		}

		m_DatePickerPrevious.SetRange(&dateStart, &dateEnd);
		m_DatePickerPrevious.SetFormat("ddd dd MMM yyy");

		CSSDate dateLookback(strLookbackDate);
		if (dateLookback.IsSet())
		{
			m_DatePickerPrevious.SetTime(COleDateTime(dateLookback.GetYear(), dateLookback.GetMonth(), dateLookback.GetDay(), 0, 0, 0));
		}
		else
		{
			m_DatePickerPrevious.SetTime(COleDateTime(2000, 1, 1, 0, 0, 0));
		}
	}

	if (m_pReportInfoCashless->GetLiabilityModeFlag() == FALSE)
	{
		m_staticPrevious1.ShowWindow(SW_HIDE);
		m_staticPrevious2.ShowWindow(SW_HIDE);
		m_DatePickerPrevious.ShowWindow(SW_HIDE);
	}

	return TRUE;
}

//$$******************************************************************

void CCashlessAuditDlg::OnSelectDateRange() 
{
	m_DateSelectorRange.UpdateDateControls();
}

//$$******************************************************************

void CCashlessAuditDlg::OnChangeDateFrom(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_DateSelectorRange.OnChangeFromDate();
	m_DateSelectorRange.CheckForChangedDate();
	*pResult = 0;
}

//$$******************************************************************

void CCashlessAuditDlg::OnChangeDateTo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_DateSelectorRange.CheckForChangedDate();
	*pResult = 0;
}

//$$******************************************************************

void CCashlessAuditDlg::OnOK()
{
	if (UpdateData(TRUE) == TRUE)
	{
		{
			CTime dateFrom;
			m_DatePickerFrom.GetTime(dateFrom);
			m_pReportInfoCashless->SetDateFrom(dateFrom);
			CTime datePrevious = dateFrom - CTimeSpan(1, 0, 0, 0);
			m_pReportInfoCashless->SetDatePrevious(datePrevious);
			CTime dateSearchTo = CTime(dateFrom.GetYear() + 1, dateFrom.GetMonth(), dateFrom.GetDay(), 0, 0, 0);
			m_pReportInfoCashless->SetDateSearchTo(dateSearchTo);
		}

		{
			CTime dateTo;
			m_DatePickerTo.GetTime(dateTo);
			m_pReportInfoCashless->SetDateTo(dateTo);
			CTime dateNext = dateTo + CTimeSpan(1, 0, 0, 0);
			m_pReportInfoCashless->SetDateNext(dateNext);
		}

		{
			CTime dateLookback;
			m_DatePickerPrevious.GetTime(dateLookback);
			m_pReportInfoCashless->SetDateSearchFrom(dateLookback);
		}

		CString strCutoffTime = "";

		{
			CTime timeCutoff;
			m_TimePickerCutoff.GetTime(timeCutoff);						// get cutoff time

			strCutoffTime.Format("%2.2d%2.2d", timeCutoff.GetHour(), timeCutoff.GetMinute());

			m_pReportInfoCashless->SetTimeCutoff(timeCutoff);
			m_pReportInfoCashless->SetCutoffTimeFlag(strCutoffTime != "0000");	// stop validating on cut off date
			m_pReportInfoCashless->SetCutoffNextDayFlag(m_comboNextDay.GetCurSel() == 1);
		}

		m_pReportInfoCashless->SetRefundProcessType(m_comboRefund.GetCurSel() == 1 ? 1 : 0);

		{
			CString strDateRangeType;
			m_comboDateRange.GetWindowText(strDateRangeType);

			CCSV csv;
			csv.Add(strDateRangeType);
			csv.Add(strCutoffTime);
			csv.Add(m_pReportInfoCashless->GetDateFromString());
			csv.Add(m_pReportInfoCashless->GetDateToString());
			csv.Add(m_pReportInfoCashless->GetCutoffNextDayFlag());
			csv.Add(m_pReportInfoCashless->GetDateSearchFromString());
			csv.Add(m_pReportInfoCashless->GetRefundProcessType());

			CReportDefaultHandler repdefs;
			repdefs.Save(m_pReportInfoCashless->GetReportLabel(), &csv);
		}

		if (m_pReportInfoCashless->GetLiabilityModeFlag() == FALSE)
		{
			m_nFinalError = ((CReportCashlessPosting*)m_pReport)->ShowInternal();
		}
		else
		{
			m_nFinalError = ((CReportCashlessLiability*)m_pReport)->ShowInternal();
		}

		switch (m_nFinalError)
		{
		case nREPORT_NOERROR:
		case nREPORT_CANCEL:
			break;

		default:
			EndDialog(IDCANCEL);
			break;
		}
	}
}

//$$******************************************************************
