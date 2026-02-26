/**********************************************************************/
 
/**********************************************************************/
#include "DateRangeHelpers.h"
//#include "EcrmanOptionsIni.h"
#include "PosTrayExportCreatorDiscount.h"
#include "PosTrayExportCreatorPayment.h"
#include "PosTrayExportCreatorSales.h"
#include "PosTrayExportCreatorVoid.h"
 
/**********************************************************************/
#include "PosTrayManualExportDlg.h"
/**********************************************************************/

CPosTrayManualExportDlg::CPosTrayManualExportDlg( CPosTrayTaskInfo& TaskInfo, CPosTrayFileSet& FileSet, CPosTrayManualExportOptions& ExportOptions, CWnd* pParent )
	: CSSDialog(CPosTrayManualExportDlg::IDD, pParent), 
	m_PosTrayTaskInfo( TaskInfo ), 
	m_PosTrayFileSet( FileSet ), 
	m_ExportOptions( ExportOptions), 
	m_DateSelectorRange( m_comboDates, m_DatePickerFrom, m_DatePickerTo )
{
	m_bRunningExport = FALSE;
	m_bCancelExport = FALSE;
	m_bTwoProgressBars = FALSE;
}

/**********************************************************************/

CPosTrayManualExportDlg::~CPosTrayManualExportDlg()
{
}

/**********************************************************************/

void CPosTrayManualExportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_DATES, m_comboDates);
	DDX_Control(pDX, IDC_DATEPICKER_FROM, m_DatePickerFrom);
	DDX_Control(pDX, IDC_DATEPICKER_TO, m_DatePickerTo);
	DDX_Control(pDX, IDC_COMBO_INTERVAL, m_comboInterval);
	DDX_Control(pDX, IDC_BUTTON_CREATE, m_buttonCreate);
	DDX_Control(pDX, IDC_BUTTON_CANCEL, m_buttonCancel);
	DDX_Control(pDX, IDOK, m_buttonOK);
	DDX_Control(pDX, IDC_STATIC_PROGRESS1, m_staticProgress1);
	DDX_Control(pDX, IDC_STATIC_PROGRESS2, m_staticProgress2);
	DDX_Control(pDX, IDC_PROGRESS1, m_Progress1);
	DDX_Control(pDX, IDC_PROGRESS2, m_Progress2);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPosTrayManualExportDlg, CDialog)
	ON_CBN_SELCHANGE( IDC_COMBO_DATES, OnSelectDate )
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATEPICKER_FROM, OnChangeDateFrom)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATEPICKER_TO, OnChangeDateTo)
	ON_BN_CLICKED( IDC_BUTTON_CREATE, OnButtonCreate )
	ON_BN_CLICKED( IDC_BUTTON_CANCEL, OnButtonCancel )
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPosTrayManualExportDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CString strTitle;
	strTitle.Format( "PosTray Manual Export (%s)", (const char*) m_PosTrayTaskInfo.m_strTaskName );
	SetWindowText( strTitle );
	
	m_DateSelectorRange.InitialiseDateControls();
	m_DateSelectorRange.AddStandardReportRanges();
	
	if ( m_ExportOptions.m_strComboSelect == szRANGE_CUSTOM_DATE )
	{
		m_DatePickerFrom.SetTime( m_ExportOptions.m_oleDateFrom );
		m_DatePickerTo.SetTime( m_ExportOptions.m_oleDateFrom );
	}
	else if ( m_ExportOptions.m_strComboSelect == szRANGE_CUSTOM_RANGE )
	{
		m_DatePickerFrom.SetTime( m_ExportOptions.m_oleDateFrom );
		m_DatePickerTo.SetTime( m_ExportOptions.m_oleDateTo );
	}

	{
		int nResult = m_comboDates.FindStringExact( 0, m_ExportOptions.m_strComboSelect );
			
		if ( nResult != CB_ERR )
			m_comboDates.SetCurSel( nResult );
		else
			m_comboDates.SetCurSel( 0 );

		OnSelectDate();
	}

	m_Progress1.SetRange( 0, 100);
	m_Progress2.SetRange( 0, 100);
	SetPos( FALSE, 0, 100 );
	SetPos( TRUE, 0, 100 );

	ShowAndEnableWindow( &m_staticProgress1, FALSE );
	ShowAndEnableWindow( &m_staticProgress2, FALSE );
	ShowAndEnableWindow( &m_Progress1, FALSE );
	ShowAndEnableWindow( &m_Progress2, FALSE );
	ShowAndEnableWindow( &m_buttonCancel, FALSE );

	m_comboInterval.AddString( "Single consolidated file" );
	m_comboInterval.AddString( "Separate file per year" );
	m_comboInterval.AddString( "Separate file per month" );
	m_comboInterval.AddString( "Separate file per week" );
	m_comboInterval.AddString( "Separate file per day" );
	
	int nSel = m_ExportOptions.m_nInterval;
	if ( ( nSel < 0 ) || ( nSel > 4 ) )
		nSel = 0;
	
	m_comboInterval.SetCurSel( nSel );

	return TRUE;  
}

/**********************************************************************/

void CPosTrayManualExportDlg::OnSelectDate() 
{
	m_DateSelectorRange.UpdateDateControls();
}

/**********************************************************************/

void CPosTrayManualExportDlg::OnChangeDateFrom(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_DateSelectorRange.OnChangeFromDate();
	m_DateSelectorRange.CheckForChangedDate();
	*pResult = 0;
}

/**********************************************************************/

void CPosTrayManualExportDlg::OnChangeDateTo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_DateSelectorRange.CheckForChangedDate();
	*pResult = 0;
}

/**********************************************************************/

bool CPosTrayManualExportDlg::GetMergedTask( CPosTrayTask& merge )
{
	bool bGotTask = FALSE;
	
	CPosTrayTask task	( m_PosTrayFileSet.m_nTaskFileType );
	CPosTrayTask batch	( m_PosTrayFileSet.m_nBatchFileType );

	task.SetTaskNo( m_PosTrayTaskInfo.m_nTaskNo );
	if ( task.Read( m_PosTrayTaskInfo.m_bModified ) == TRUE )
	{
		int nBatchNo = task.GetTaskBatchNo();
		
		if ( nBatchNo != 0 )
		{
			batch.SetBatchNo( nBatchNo );
	
			if ( ::FileExists( batch.GetFilename( TRUE ) ) == TRUE )
			{
				if ( batch.Read( TRUE ) == TRUE )
					bGotTask = TRUE;
			}
			else if ( ::FileExists( batch.GetFilename( FALSE ) ) == TRUE )
			{
				if ( batch.Read( FALSE ) == TRUE )
					bGotTask = TRUE;
			}
		}
	}
	
	if ( TRUE == bGotTask )
	{
		merge.SetTaskNo( task.GetTaskNo() );
		merge.MergeTask( task );
		merge.MergeBatch( batch );
	}

	return bGotTask;
}

/**********************************************************************/

void CPosTrayManualExportDlg::OnButtonCreate()
{
	if ( TRUE == m_bRunningExport )
		return;

	COleDateTime dateFrom, dateTo;
	m_DatePickerFrom.GetTime( dateFrom );
	m_DatePickerTo.GetTime( dateTo );

	if ( dateFrom > dateTo )
	{
		Prompter.Error( "The end date for exports must not be\nearlier than the start date." );
		return;
	}

	CPosTrayTask merge( m_PosTrayFileSet.m_nAllFileType );
	if ( FALSE == GetMergedTask( merge ) )
	{
		Prompter.Error( "Unable to load PosTray task." );
		return;
	}

	m_nTargetFiles = 0;
	m_nNoDataFiles = 0;
	m_nNoExportFolder = 0;
	m_bRunningExport = TRUE;
	m_bCancelExport = FALSE;

	int nInterval = m_comboInterval.GetCurSel();
	m_bTwoProgressBars = ( nInterval != 0 ); 

	m_comboDates.EnableWindow( FALSE );
	m_DatePickerFrom.EnableWindow( FALSE );
	m_DatePickerTo.EnableWindow( FALSE );
	m_comboInterval.EnableWindow( FALSE );
	m_buttonOK.EnableWindow( FALSE );
	m_buttonCreate.EnableWindow( FALSE );
	MoveControl( &m_buttonCancel, 267, m_bTwoProgressBars ? 169 : 130 );
	ShowAndEnableWindow( &m_buttonCancel, TRUE );
	ShowAndEnableWindow( &m_staticProgress1, TRUE );
	ShowAndEnableWindow( &m_staticProgress2, m_bTwoProgressBars );
	ShowAndEnableWindow( &m_Progress1, TRUE );
	ShowAndEnableWindow( &m_Progress2, m_bTwoProgressBars );

	m_staticProgress1.SetWindowText( "" );
	m_staticProgress2.SetWindowText( "" );
	
	m_comboDates.RedrawWindow();
	m_DatePickerFrom.RedrawWindow();
	m_DatePickerTo.RedrawWindow();
	m_comboInterval.RedrawWindow();
	m_buttonOK.RedrawWindow();
	m_buttonCreate.RedrawWindow();
	m_buttonCancel.RedrawWindow();
	m_staticProgress1.RedrawWindow();
	m_staticProgress2.RedrawWindow();
	m_Progress1.RedrawWindow();
	m_Progress2.RedrawWindow();
	
	switch( nInterval )
	{
	case 0:	CreateSingleExport( dateFrom, dateTo, merge );		break;
	case 1:	CreateYearlyExport( dateFrom, dateTo, merge );		break;
	case 2:	CreateMonthlyExport( dateFrom, dateTo, merge );		break;
	case 3:	CreateWeeklyExport( dateFrom, dateTo, merge );		break;
	case 4:	CreateDailyExport( dateFrom, dateTo, merge );		break;
	}

	m_buttonCancel.EnableWindow( FALSE );
	
	if ( FALSE == m_bCancelExport )
	{
		SetPos( FALSE, 100, 100 );
		SetPos( TRUE, 100, 100);
		
		CString strAction;
		if ( ( 0 == m_nNoDataFiles ) && ( 0 == m_nNoExportFolder ) )
			strAction = "created";
		else
			strAction = "attempted";

		CString strMsg = "";

		if ( 1 == m_nTargetFiles )
			strMsg.Format ( "Exports were %s as requested (1 file).", (const char*) strAction );
		else
			strMsg.Format( "Exports were %s as requested (%d files).", (const char*) strAction, m_nTargetFiles );
		
		if ( m_nNoDataFiles != 0 )
		{
			CString strExtra;
			if ( 1 == m_nNoDataFiles )
				strExtra = "1 file was";
			else
				strExtra.Format( "%I64d files were", m_nNoDataFiles );

			strMsg += "\n\n";
			strMsg += strExtra;
			strMsg += " not created because no sales data was found.";
		}

		if ( m_nNoExportFolder != 0 )
		{
			CString strExtra;
			if ( 1 == m_nNoExportFolder )
				strExtra = "1 file was";
			else
				strExtra.Format( "%I64d files were", m_nNoExportFolder );

			strMsg += "\n\n";
			strMsg += strExtra;
			strMsg += " not created because the export folder did not exist.";
		}
	
		Prompter.Info( strMsg );
	}
	else
	{
		Prompter.Info( "Export file creation was cancelled." );
	}

	m_bRunningExport = FALSE;
	m_bCancelExport = FALSE;

	SetPos( FALSE, 0, 100 );
	SetPos( TRUE, 0, 100);

	m_comboDates.EnableWindow( TRUE );
	m_comboInterval.EnableWindow( TRUE );
	m_buttonOK.EnableWindow( TRUE );
	m_DateSelectorRange.UpdateDateControls();

	ShowAndEnableWindow( &m_buttonCancel, FALSE );
	ShowAndEnableWindow( &m_staticProgress1, FALSE );
	ShowAndEnableWindow( &m_staticProgress2, FALSE );
	ShowAndEnableWindow( &m_Progress1, FALSE );
	ShowAndEnableWindow( &m_Progress2, FALSE );
	ShowAndEnableWindow( &m_buttonCreate, TRUE );
}

/**********************************************************************/

void CPosTrayManualExportDlg::CreateSingleExport( COleDateTime& dateFrom, COleDateTime& dateTo, CPosTrayTask& task )
{
	m_nTargetFiles = 1;
	m_staticProgress1.SetWindowText( "File creation progress" );
	PerformExport( dateFrom, dateTo, task, task.GetNextManualExportNo() );
}

/**********************************************************************/

void CPosTrayManualExportDlg::CreateYearlyExport( COleDateTime& dateFrom, COleDateTime& dateTo, CPosTrayTask& task )
{
	m_nTargetFiles = 1 + dateTo.GetYear() - dateFrom.GetYear();
	__int64 nProgress = 0;

	CDateRangeHelpers DateRangeHelpers;
	COleDateTime ExportDateFrom = dateFrom;

	int nExportNo = task.GetNextManualExportNo();

	while ( ( ExportDateFrom <= dateTo ) && ( FALSE == m_bCancelExport ) )
	{
		CString str;
		str.Format( "Creating export file %I64d of %I64d", nProgress + 1, m_nTargetFiles );
		m_staticProgress1.SetWindowText( str );

		SetPos( FALSE, nProgress++, m_nTargetFiles );
		SetPos( TRUE, 0, 100 );

		int nYear = ExportDateFrom.GetYear() + 1;
		COleDateTime ExportDateTo = COleDateTime( nYear, 1, 1, 0, 0, 0 );
		DateRangeHelpers.AddDaysToTime( ExportDateTo, -1 );

		PerformExport( ExportDateFrom, ExportDateTo, task, nExportNo );

		ExportDateFrom = ExportDateTo;
		DateRangeHelpers.AddDaysToTime( ExportDateFrom, 1 );
	}
}

/**********************************************************************/

void CPosTrayManualExportDlg::CreateMonthlyExport( COleDateTime& dateFrom, COleDateTime& dateTo, CPosTrayTask& task )
{
	m_nTargetFiles = 1;
	m_nTargetFiles += 12 * ( dateTo.GetYear() - dateFrom.GetYear() );
	m_nTargetFiles += ( dateTo.GetMonth() - dateFrom.GetMonth() );
	__int64 nProgress = 0;

	CDateRangeHelpers DateRangeHelpers;
	COleDateTime ExportDateFrom = dateFrom;

	int nExportNo = task.GetNextManualExportNo();

	while ( ( ExportDateFrom <= dateTo ) && ( FALSE == m_bCancelExport ) )
	{
		CString str;
		str.Format( "Creating export file %I64d of %I64d", nProgress + 1, m_nTargetFiles );
		m_staticProgress1.SetWindowText( str );

		SetPos( FALSE, nProgress++, m_nTargetFiles );
		SetPos( TRUE, 0, 100 );

		int nYear = ExportDateFrom.GetYear();
		int nMonth = ExportDateFrom.GetMonth() + 1;
		
		if ( nMonth > 12 )
		{
			nYear++;
			nMonth = 1;
		}

		COleDateTime ExportDateTo = COleDateTime( nYear, nMonth, 1, 0, 0, 0 );
		DateRangeHelpers.AddDaysToTime( ExportDateTo, -1 );

		PerformExport( ExportDateFrom, ExportDateTo, task, nExportNo );
				
		ExportDateFrom = ExportDateTo;
		DateRangeHelpers.AddDaysToTime( ExportDateFrom, 1 );
	}
}

/**********************************************************************/

void CPosTrayManualExportDlg::CreateWeeklyExport( COleDateTime& dateFrom, COleDateTime& dateTo, CPosTrayTask& task )
{
	COleDateTimeSpan span = dateTo - dateFrom;
	int nWeekdayFrom = dateFrom.GetDayOfWeek() - EcrmanOptions.GetReportsWeekStartDay() - 1;
			
	if ( nWeekdayFrom < 0 )
		nWeekdayFrom += 7;

	m_nTargetFiles = ( 7 + nWeekdayFrom + span.GetDays() ) / 7;
	__int64 nProgress = 0;

	CDateRangeHelpers DateRangeHelpers;
	COleDateTime ExportDateFrom = dateFrom;

	int nExportNo = task.GetNextManualExportNo();

	while ( ( ExportDateFrom <= dateTo ) && ( FALSE == m_bCancelExport ) )
	{
		CString str;
		str.Format( "Creating export file %I64d of %I64d", nProgress + 1, m_nTargetFiles );
		m_staticProgress1.SetWindowText( str );

		SetPos( FALSE, nProgress++, m_nTargetFiles );
		SetPos( TRUE, 0, 100 );

		COleDateTime ExportDateTo = ExportDateFrom;

		if ( nProgress == 1 )
			DateRangeHelpers.AddDaysToTime( ExportDateTo, 6 - nWeekdayFrom );
		else
			DateRangeHelpers.AddDaysToTime( ExportDateTo, 6 );

		PerformExport( ExportDateFrom, ExportDateTo, task, nExportNo );

		ExportDateFrom = ExportDateTo;
		DateRangeHelpers.AddDaysToTime( ExportDateFrom, 1 );
	}
}

/**********************************************************************/

void CPosTrayManualExportDlg::CreateDailyExport( COleDateTime& dateFrom, COleDateTime& dateTo, CPosTrayTask& task )
{
	COleDateTimeSpan span = dateTo - dateFrom;

	m_nTargetFiles = span.GetDays() + 1;
	__int64 nProgress = 0;

	CDateRangeHelpers DateRangeHelpers;
	COleDateTime ExportDate = dateFrom;

	int nExportNo = task.GetNextManualExportNo();

	while ( ( ExportDate <= dateTo ) && ( FALSE == m_bCancelExport ) )
	{
		CString str;
		str.Format( "Creating export file %I64d of %I64d", nProgress + 1, m_nTargetFiles );
		m_staticProgress1.SetWindowText( str );

		SetPos( FALSE, nProgress++, m_nTargetFiles );
		SetPos( TRUE, 0, 100 );

		PerformExport( ExportDate, ExportDate, task, nExportNo );

		DateRangeHelpers.AddDaysToTime( ExportDate, 1 );
	}
}

/**********************************************************************/

void CPosTrayManualExportDlg::PerformExport( COleDateTime& DateFrom, COleDateTime& DateTo, CPosTrayTask& task, int nExportNo )
{
	__int64 nLineCount = 0;

	COleDateTime dateDummy = COleDateTime::GetCurrentTime();
			
	switch ( m_PosTrayFileSet.m_nAllFileType )
	{
	case POSTRAY_TASKFILE_EXPORT_SALES_ALL:
		{
			CPosTrayExportCreatorSales ExportCreator( task, m_bCancelExport, this, nExportNo );
			ExportCreator.FixDates( DateFrom, DateTo );
			nLineCount = ExportCreator.ProcessTask( dateDummy );
		}
		break;

	case POSTRAY_TASKFILE_EXPORT_PAYMENT_ALL:
		{
			CPosTrayExportCreatorPayment ExportCreator( task, m_bCancelExport, this, nExportNo );
			ExportCreator.FixDates( DateFrom, DateTo );
			nLineCount = ExportCreator.ProcessTask( dateDummy );
		}
		break;

	case POSTRAY_TASKFILE_EXPORT_VOID_ALL:
		{
			CPosTrayExportCreatorVoid ExportCreator( task, m_bCancelExport, this, nExportNo );
			ExportCreator.FixDates( DateFrom, DateTo );
			nLineCount = ExportCreator.ProcessTask( dateDummy );
		}
		break;

	case POSTRAY_TASKFILE_EXPORT_DISCOUNT_ALL:
		{
			CPosTrayExportCreatorDiscount ExportCreator( task, m_bCancelExport, this, nExportNo );
			ExportCreator.FixDates( DateFrom, DateTo );
			nLineCount = ExportCreator.ProcessTask( dateDummy );
		}
		break;
	}

	if ( nLineCount == -1 )
		m_nNoExportFolder++;
	else if ( nLineCount < 2 )
		m_nNoDataFiles++;
}

/**********************************************************************/

void CPosTrayManualExportDlg::OnButtonCancel()
{
	if ( ( TRUE == m_bRunningExport ) && ( FALSE == m_bCancelExport ) )
	{
		if ( Prompter.YesNo( "Are you sure you wish to cancel these exports." ) == IDYES )
		{
			m_bCancelExport = TRUE;
			ShowAndEnableWindow( &m_buttonCancel, TRUE, FALSE );
		}
	}
}

/**********************************************************************/

void CPosTrayManualExportDlg::UpdateFileProgressPos( __int64 nProgress, __int64 nTarget )
{
	SetPos( m_bTwoProgressBars, nProgress, nTarget );
}

/**********************************************************************/

void CPosTrayManualExportDlg::SetPos( bool bBottom, __int64 nProgress, __int64 nTarget )
{
	if ( FALSE == bBottom )
		SetPos( m_Progress1, m_nProgressPos1, nProgress, nTarget );
	else
		SetPos( m_Progress2, m_nProgressPos2, nProgress, nTarget );
}

/**********************************************************************/

void CPosTrayManualExportDlg::SetPos( CProgressCtrl& progress, int& nProgressPos, __int64 nProgress, __int64 nTarget )
{
	RestoreWaitCursor();
	PumpMessage();

	if ( nTarget != 0 )
	{
		int nPos = int ( ( nProgress * 100 ) / nTarget );
		
		if ( nPos < 0 || nPos > 100 ) 
			nPos = 0;
		
		if ( nPos != nProgressPos )
		{
			nProgressPos = nPos;
			progress.SetPos(nPos);
		}
	}
}

/**********************************************************************/

void CPosTrayManualExportDlg::UpdateExportOptions()
{
	m_comboDates.GetWindowText( m_ExportOptions.m_strComboSelect );
	m_DatePickerFrom.GetTime( m_ExportOptions.m_oleDateFrom );
	m_DatePickerTo.GetTime( m_ExportOptions.m_oleDateTo );
	m_ExportOptions.m_nInterval = m_comboInterval.GetCurSel();
}

/**********************************************************************/

void CPosTrayManualExportDlg::OnOK()
{
	if ( FALSE == m_bRunningExport )
	{
		UpdateExportOptions();
		EndDialog( IDOK );
	}
}

/**********************************************************************/

void CPosTrayManualExportDlg::OnCancel()
{
	if ( FALSE == m_bRunningExport )
	{
		UpdateExportOptions();
		EndDialog( IDCANCEL );
	}
}

/**********************************************************************/
