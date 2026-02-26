//$$******************************************************************
#include "LoyaltyManager.h"
#include "ReportDefaultHandler.h"
//$$******************************************************************
#include "ReportStartingBalanceDlg.h"
//$$******************************************************************

CReportStartingBalanceDlg::CReportStartingBalanceDlg( CWnd* pParent /*=NULL*/)
	: CSSDialog(CReportStartingBalanceDlg::IDD, pParent)
	, m_DateSelector( m_comboDate, m_DatePicker) 
{
	m_pReportInfo = NULL;
}

//$$******************************************************************

void CReportStartingBalanceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_DATE, m_comboDate);
	DDX_Control(pDX, IDC_DATEPICKER, m_DatePicker);
	DDX_Control(pDX, IDC_DATEPICKER_PREVIOUS, m_DatePickerPrevious);
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CReportStartingBalanceDlg, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO_DATE, OnSelectDate)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATEPICKER, OnChangeDate)
END_MESSAGE_MAP()

//$$******************************************************************

BOOL CReportStartingBalanceDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString strPreviousDateType = "";
	CString strPreviousDate = "";
	CString strLookbackDate = "";
	
	CString strLine;
	CReportDefaultHandler repdefs;
	if (repdefs.Read(m_pReportInfo->GetReportLabel(), strLine) == TRUE)
	{
		CCSV csv(strLine);
		if (csv.IsEmpty(0) == FALSE)		strPreviousDateType = csv.GetString(0);
		if (csv.IsEmpty(1) == FALSE)		strPreviousDate = csv.GetString(1);
		if (csv.IsEmpty(2) == FALSE)		strLookbackDate = csv.GetString(2);
	}

	m_comboDate.AddString ( szRANGE_TODAY );
	m_comboDate.AddString ( szRANGE_YESTERDAY );
	m_comboDate.AddString ( szRANGE_THIS_WEEK );
	m_comboDate.AddString ( szRANGE_THIS_MONTH );
	m_comboDate.AddString ( szRANGE_THIS_YEAR );
	m_comboDate.AddString ( szRANGE_LAST_WEEK );
	m_comboDate.AddString ( szRANGE_LAST_MONTH );
	m_comboDate.AddString ( szRANGE_LAST_YEAR );
	m_comboDate.AddString ( szRANGE_LAST_3_MONTHS );
	m_comboDate.AddString ( szRANGE_LAST_6_MONTHS );
	m_comboDate.AddString ( szRANGE_LAST_9_MONTHS );
	m_comboDate.AddString ( szRANGE_LAST_12_MONTHS );
	m_comboDate.AddString ( szRANGE_ALL );
	m_comboDate.AddString ( szRANGE_CUSTOM_DATE );
	m_DateSelector.SetComboSelection(strPreviousDateType);

	if ( strPreviousDateType == szRANGE_CUSTOM_DATE )
	{
		CSSDate date( strPreviousDate );
		
		if ( date.IsSet() )
		{
			m_DatePicker.SetTime( COleDateTime( date.GetYear(), date.GetMonth(), date.GetDay(), 0, 0, 0 ) );
		}
	}

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
	
	return TRUE;
}

//$$******************************************************************

void CReportStartingBalanceDlg::OnSelectDate() 
{
	m_DateSelector.UpdateDateControl();
}

//$$******************************************************************

void CReportStartingBalanceDlg::OnChangeDate(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_DateSelector.CheckForChangedDate();
	*pResult = 0;
}

//$$******************************************************************

void CReportStartingBalanceDlg::OnOK()
{
	if ( UpdateData() == TRUE )
	{
		{
			CTime date;
			m_DatePicker.GetTime(date);
			m_pReportInfo->SetDateBalance(date);

			CTime dateEnd = CTime(date.GetYear() + 1, date.GetMonth(), date.GetDay(), 0, 0, 0);
			m_pReportInfo->SetDateSearchTo(dateEnd);
		}

		{
			CTime dateLookback;
			m_DatePickerPrevious.GetTime(dateLookback);
			m_pReportInfo->SetDateSearchFrom(dateLookback);
		}

		{
			CString strDateType = "";
			m_comboDate.GetWindowText(strDateType);

			CCSV csv;
			csv.Add(strDateType);
			csv.Add(m_pReportInfo->GetDateBalanceString());
			csv.Add(m_pReportInfo->GetDateSearchFromString());

			CReportDefaultHandler repdefs;
			repdefs.Save(m_pReportInfo->GetReportLabel(), &csv);
		}

		EndDialog( IDOK );
	}
}

//$$******************************************************************

