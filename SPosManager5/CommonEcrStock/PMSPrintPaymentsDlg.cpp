/**********************************************************************/
#ifdef COMPILE_PMS 
/**********************************************************************/
#include <share.h>
/**********************************************************************/
#include "PMSOptions.h"
#include "PMSReportPayments.h"
#include "SessionEditDlg.h"
/**********************************************************************/
#include "PMSPrintPaymentsDlg.h"
/**********************************************************************/

CPMSPrintPaymentsDlg::CPMSPrintPaymentsDlg(CWnd* pParent /*=NULL*/)
	: CSSDialog(CPMSPrintPaymentsDlg::IDD, pParent),
	m_DateSelectorRange( m_comboDates, m_DatePickerFrom, m_DatePickerTo )
{
	//{{AFX_DATA_INIT(CPMSPrintPaymentsDlg)
	//}}AFX_DATA_INIT
}

/**********************************************************************/

void CPMSPrintPaymentsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPMSPrintPaymentsDlg)
	DDX_Control(pDX, IDC_COMBO_ROUND, m_comboRound);
	DDX_Control(pDX, IDC_COMBO_DATES, m_comboDates);
	DDX_Control(pDX, IDC_DATEPICKER_TO, m_DatePickerTo);
	DDX_Control(pDX, IDC_DATEPICKER_FROM, m_DatePickerFrom);
	DDX_Control(pDX, IDC_COMBO_SESSION, m_comboSession);
	DDX_Control(pDX, IDC_CHECK_TRAINING, m_checkTraining);
	DDX_Check(pDX, IDC_CHECK_PENDING, m_bPending);
	DDX_Check(pDX, IDC_CHECK_ACTIVE, m_bActive);
	DDX_Check(pDX, IDC_CHECK_COMPLETE, m_bComplete);
	DDX_Check(pDX, IDC_CHECK_CANCELLED, m_bCancelled);
	DDX_Check(pDX, IDC_CHECK_DEPOSIT, m_bDeposit);
	DDX_Check(pDX, IDC_CHECK_PAYMENT, m_bPayment);
	DDX_Check(pDX, IDC_CHECK_TRAINING, m_bTraining);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPMSPrintPaymentsDlg, CDialog)
	//{{AFX_MSG_MAP(CPMSPrintPaymentsDlg)
	ON_CBN_SELCHANGE(IDC_COMBO_DATES, OnSelectDate)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATEPICKER_FROM, OnChangeDateFrom)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATEPICKER_TO, OnChangeDateTo)
	ON_BN_CLICKED(IDC_BUTTON_DISPLAY, OnButtonDisplay)
	ON_BN_CLICKED(IDC_BUTTON_DEFAULT, OnButtonDefault)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPMSPrintPaymentsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_DateSelectorRange.InitialiseDateControls();
	m_DateSelectorRange.SetMode( DATE_SELECTOR_MODE_PAST );
	m_DateSelectorRange.AddPastReportRanges();
	m_DateSelectorRange.SetComboSelection( 0, szRANGE_TODAY );

	m_comboRound.AddString ( "Dates as selected" );
	m_comboRound.AddString ( "Nearest full week" );
	m_comboRound.AddString ( "Nearest full month" );
	m_comboRound.SetCurSel(0);

	for ( int nIndex = 0; nIndex < DataManagerNonDb.ReportSession.GetSize(); nIndex++ )
		m_comboSession.AddString( DataManagerNonDb.ReportSession.GetName( nIndex ) );

	m_comboSession.AddString( "Adhoc Session" );

	if ( m_comboSession.GetCount() < 2 )
		m_comboSession.InsertString ( 0, "00:00 to 23:59" );
	
	m_comboSession.EnableWindow( m_comboSession.GetCount() > 1 );
	m_comboSession.SetCurSel(0);

	CString strBuffer;
	bool bGotFile = FALSE;
	
	CSSFile fileConfig;

	CFilenameUpdater FnUp( SysFiles::PMSPaymentsDef );
	if ( fileConfig.Open ( FnUp.GetFilenameToUse(), "rb" ) == TRUE )
	{
		if ( fileConfig.ReadString ( strBuffer ) == TRUE )
		{
			bGotFile = TRUE;
			
			CCSVPMSPrintPaymentsKey csv ( strBuffer );

			m_bPending = csv.IncludePending();
			m_bActive = csv.IncludeActive();
			m_bComplete = csv.IncludeComplete();
			m_bCancelled = csv.IncludeCancelled();
			m_bDeposit = csv.IncludeDeposit();
			m_bPayment = csv.IncludePayment();
			m_bTraining = csv.IncludeTraining();
			
			UpdateData ( FALSE );
		}
	}
		
	if ( bGotFile == FALSE )
		OnButtonDefault();

	if ( PMSOptions.GetMiscEnableTrainingMode() == FALSE )
	{
		m_checkTraining.SetCheck( FALSE );
		m_checkTraining.EnableWindow( FALSE );
		m_checkTraining.ShowWindow( SW_HIDE );
	}
	
	return TRUE;  
}

/**********************************************************************/

void CPMSPrintPaymentsDlg::OnSelectDate() 
{
	m_DateSelectorRange.UpdateDateControls();
}

/**********************************************************************/

void CPMSPrintPaymentsDlg::GetReportDates()
{
	COleDateTime dateFrom,dateTo;
	m_DatePickerFrom.GetTime( dateFrom );
	m_DatePickerTo.GetTime( dateTo );

	switch ( m_comboRound.GetCurSel() )
	{
	case 1:	//ROUND TO WEEK
		{
			int nWeekStartDay = EcrmanOptions.GetReportsWeekStartDay();
				
			int nWeekDayFrom = dateFrom.GetDayOfWeek() - 1;
			int nOffsetFrom = nWeekDayFrom - nWeekStartDay;
			if ( nOffsetFrom < 0 ) nOffsetFrom += 7;
			dateFrom -= COleDateTimeSpan ( nOffsetFrom, 0, 0, 0 );

			int nWeekDayTo = dateTo.GetDayOfWeek() - 1;
			int nOffsetTo = ( nWeekStartDay - nWeekDayTo ) - 1;
			if ( nOffsetTo < 0 ) nOffsetTo += 7;
			dateTo += COleDateTimeSpan ( nOffsetTo, 0, 0, 0 );
		}
		break;

	case 2:	//ROUND TO MONTH
		{
			int nYearFrom = dateFrom.GetYear();
			int nMonthFrom = dateFrom.GetMonth();
			dateFrom = COleDateTime( nYearFrom, nMonthFrom, 1, 0, 0, 0 );
		
			int nYearTo = dateTo.GetYear();
			int nMonthTo = dateTo.GetMonth();
				
			nMonthTo++;
			if ( nMonthTo > 12 )
			{
				nYearTo++;
				nMonthTo = 1;
			}

			dateTo = COleDateTime( nYearTo, nMonthTo, 1, 0, 0, 0 );
			dateTo -= COleDateTimeSpan ( 1, 0, 0, 0 );
		}

	default:
		break;
	}

	m_OleDateFrom = dateFrom;
	m_OleDateTo = dateTo;

	int n = m_comboSession.GetCurSel();
	if ( n >= 0 && n < DataManagerNonDb.ReportSession.GetSize() )
	{
		CSessionCSVRecord Session;
		DataManagerNonDb.ReportSession.GetAt ( n, m_ReportSession );
	}
	else
		m_ReportSession.ClearRecord();
}

/**********************************************************************/

bool CPMSPrintPaymentsDlg::GetAdhocReportSession()
{
	CSessionEditDlg dlg( m_ReportSessionAdhoc, TRUE, this );

	if ( dlg.DoModal() != IDOK )
		return FALSE;
	
	CCSV csv;
	m_ReportSessionAdhoc.ConvertToCSV( csv );
	m_ReportSession.ConvertFromCSV( csv );
	return TRUE;
}

/**********************************************************************/

void CPMSPrintPaymentsDlg::OnChangeDateFrom(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_DateSelectorRange.OnChangeFromDate();
	m_DateSelectorRange.CheckForChangedDate();
	*pResult = 0;
}

/**********************************************************************/

void CPMSPrintPaymentsDlg::OnChangeDateTo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_DateSelectorRange.CheckForChangedDate();
	*pResult = 0;
}

/**********************************************************************/

void CPMSPrintPaymentsDlg::SetDefaults()
{
	m_bPending = TRUE;
	m_bActive = TRUE;
	m_bComplete = FALSE;
	m_bCancelled = FALSE;
	m_bDeposit = TRUE;
	m_bPayment = FALSE;
	m_bTraining = FALSE;
}

/**********************************************************************/

void CPMSPrintPaymentsDlg::OnButtonDefault() 
{
	SetDefaults();
	UpdateData ( FALSE );
}

/**********************************************************************/

void CPMSPrintPaymentsDlg::OnButtonSave() 
{
	UpdateData( TRUE, FALSE );

	CSSFile fileConfig;
	CFilenameUpdater FnUp( SysFiles::PMSPaymentsDef );
	if ( fileConfig.Open ( FnUp.GetFilenameToUse(), "wb" ) == TRUE )
		fileConfig.WriteLine ( GetCSVLine() );
}

/**********************************************************************/

const char* CPMSPrintPaymentsDlg::GetCSVLine()
{
	CCSV csv;	
	csv.Add( m_bPending );
	csv.Add( m_bActive );
	csv.Add( m_bComplete );
	csv.Add( m_bDeposit );
	csv.Add( m_bPayment );
	csv.Add( m_bTraining );
	csv.Add( m_bCancelled );
	m_strFieldSelect = csv.GetLine();
	return m_strFieldSelect;
}

/**********************************************************************/

void CPMSPrintPaymentsDlg::OnButtonDisplay() 
{
	if ( UpdateData( TRUE ) == TRUE )
	{
		CString str = "";
			
		if ( ( 0 == m_bPending ) && ( 0 == m_bActive ) && ( 0 == m_bComplete ) && ( 0 == m_bCancelled ) )
		{
			str += "You must select at least one booking status type\n";
			str += "to include in this report.";
			Prompter.Error( str );
		}
		else if ( ( 0 == m_bDeposit ) && ( 0 == m_bPayment ) )
		{
			str += "You must select at least one PMS payment type\n";
			str += "to include in this report.";
			Prompter.Error( str );
		}
		else
		{
			GetReportDates();

			if ( m_comboSession.GetCurSel() == m_comboSession.GetCount() - 1 )
			if ( GetAdhocReportSession() == FALSE )
				return;

			CString strSessionTitle;
			if ( m_comboSession.GetCurSel() < m_comboSession.GetCount() - 1 )
				m_comboSession.GetWindowText( strSessionTitle );
			else
				strSessionTitle = m_ReportSessionAdhoc.GetName();

			CPMSReportPayments Report;
			Report.SetReportDates( m_OleDateFrom, m_OleDateTo, m_ReportSession );
			Report.SetSessionTitle( strSessionTitle );
			Report.SetIncludePendingFlag( m_bPending != 0 );
			Report.SetIncludeActiveFlag( m_bActive != 0 );
			Report.SetIncludeCompleteFlag( m_bComplete != 0 );
			Report.SetIncludeCancelledFlag( m_bCancelled != 0 );
			Report.SetIncludeDepositFlag( m_bDeposit != 0 );
			Report.SetIncludePaymentFlag( m_bPayment != 0 );
			Report.SetIncludeTrainingFlag( m_bTraining != 0 );
			Report.CreateReport();
		}
	}
}

/**********************************************************************/
#endif
/**********************************************************************/
