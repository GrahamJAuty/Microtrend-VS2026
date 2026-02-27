//$$******************************************************************
#include "..\SmartPay4Shared\RepmanHandler.h"
//$$******************************************************************
#include "PasswordLogReport.h"
//$$******************************************************************
#include "PasswordLogDlg.h"
//$$******************************************************************

CPasswordLogDlg::CPasswordLogDlg(CWnd* pParent)
	: CSSAutoShutdownDialog(AutoShutdownHelper, CPasswordLogDlg::IDD, pParent)
{
}

//$$******************************************************************

CPasswordLogDlg::~CPasswordLogDlg()
{
}

//$$******************************************************************

void CPasswordLogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_DATERANGE, m_comboDateRange);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_DatePickerFrom);
	DDX_Control(pDX, IDC_DATETIMEPICKER2, m_DatePickerTo);
	DDX_Control(pDX, IDC_CHECK_SORT, m_checkSort);
	DDX_Control(pDX, IDC_RADIO_TYPE1, m_radioType1);
	DDX_Control(pDX, IDC_RADIO_TYPE2, m_radioType2);
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CPasswordLogDlg, CSSAutoShutdownDialog)
	ON_CBN_SELCHANGE(IDC_COMBO_DATERANGE, OnSelectDateRange)
END_MESSAGE_MAP()

//$$******************************************************************

BOOL CPasswordLogDlg::CSSAutoShutdownPostInitDialog()
{
	{
		COleDateTime timeNow = COleDateTime::GetCurrentTime();
		m_DatePickerFrom.SetTime(timeNow);
		m_DatePickerTo.SetTime(timeNow);
	}

	// Date range
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
	m_comboDateRange.SetCurSel(1);

	OnSelectDateRange();

	m_radioType1.SetCheck(TRUE);
	m_radioType2.SetCheck(FALSE);

	return TRUE;
}

//$$******************************************************************

void CPasswordLogDlg::OnSelectDateRange()
{
	CSSDate dateFrom;
	CSSDate dateTo;

	CString strDateRange = "";
	int nSel = m_comboDateRange.GetCurSel();
	m_comboDateRange.GetLBText(nSel, strDateRange);

	if (strDateRange == szRANGE_ALL)
	{
		dateFrom.SetDate("01/01/2000");
		dateTo.SetCurrentDate();
	}
	else
	{
		CSSDateRange date(System.GetStartOfWeek(), 1, 1, 1);
		date.GetRange(strDateRange, dateFrom, dateTo);
	}

	CTime timeFrom(dateFrom.GetYear(), dateFrom.GetMonth(), dateFrom.GetDay(), 0, 0, 0);
	m_DatePickerFrom.SetTime(&timeFrom);

	CTime timeTo(dateTo.GetYear(), dateTo.GetMonth(), dateTo.GetDay(), 0, 0, 0);
	m_DatePickerTo.SetTime(&timeTo);
}

//$$******************************************************************

void CPasswordLogDlg::OnOK()
{
	COleDateTime dateStart;
	m_DatePickerFrom.GetTime(dateStart);

	COleDateTime dateEnd;
	m_DatePickerTo.GetTime(dateEnd);

	CString strDateStart;
	strDateStart.Format("%4.4d%2.2d%2.2d",
		dateStart.GetYear(),
		dateStart.GetMonth(),
		dateStart.GetDay());

	CString strDateEnd;
	strDateEnd.Format("%4.4d%2.2d%2.2d",
		dateEnd.GetYear(),
		dateEnd.GetMonth(),
		dateEnd.GetDay());

	CPasswordLogReport Report(strDateStart, strDateEnd, IsTicked(m_checkSort), IsTicked(m_radioType2));
	Report.CreateReport();

	CRepmanHandler RepmanHandler("User Log", this);
	RepmanHandler.DisplayReport(Filenames.GetUserLogReportFilename(), IsTicked(m_radioType2) ? "2" : "1", Filenames.GetUserLogParamsFilename());
}

//$$******************************************************************

