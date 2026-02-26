/**********************************************************************/
#ifdef COMPILE_PMS
/**********************************************************************/
#include "PMSOptions.h"
#include "PMSReportUnpaid.h" 
/**********************************************************************/
#include "PMSPrintUnpaidDlg.h"
/**********************************************************************/

CPMSPrintUnpaidDlg::CPMSPrintUnpaidDlg( CWnd* pParent /*=NULL*/)
	: CSSDialog(CPMSPrintUnpaidDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPMSPrintUnpaidDlg)
	//}}AFX_DATA_INIT
	SetDefaults();
}

/**********************************************************************/

void CPMSPrintUnpaidDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPMSPrintUnpaidDlg)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_CHECK_TRAINING, m_checkIncludeTraining);
	DDX_Check(pDX, IDC_CHECK_PENDING, m_bPending);
	DDX_Check(pDX, IDC_CHECK_ACTIVE, m_bActive);
	DDX_Check(pDX, IDC_CHECK_COMPLETE, m_bComplete);
	DDX_Check(pDX, IDC_CHECK_CANCELLED, m_bCancelled);
	DDX_Check(pDX, IDC_CHECK_LINES, m_bPrintLines);
	DDX_Check(pDX, IDC_CHECK_TRAINING, m_bIncludeTraining);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPMSPrintUnpaidDlg, CDialog)
	//{{AFX_MSG_MAP(CPMSPrintUnpaidDlg)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_DEFAULT, OnButtonDefault)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPMSPrintUnpaidDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CString strBuffer;
	bool bGotFile = FALSE;
	
	CSSFile fileConfig;

	CFilenameUpdater FnUp( SysFiles::PMSUnpaidDef );
	if ( fileConfig.Open ( FnUp.GetFilenameToUse(), "rb" ) == TRUE )
	{
		if ( fileConfig.ReadString ( strBuffer ) == TRUE )
		{
			bGotFile = TRUE;
			
			CCSVPMSPrintUnpaidKey csv ( strBuffer );

			m_bPending = csv.IncludePending();
			m_bActive = csv.IncludeActive();
			m_bComplete = csv.IncludeComplete();
			m_bCancelled = csv.IncludeCancelled();
			m_bPrintLines = csv.PrintLines();
			m_bIncludeTraining = csv.IncludeTraining() & PMSOptions.GetMiscEnableTrainingMode();
			
			UpdateData ( FALSE );
		}
	}
		
	if ( bGotFile == FALSE )
		OnButtonDefault();

	if ( PMSOptions.GetMiscEnableTrainingMode() == FALSE )
	{
		m_checkIncludeTraining.SetCheck( FALSE );
		m_checkIncludeTraining.EnableWindow( FALSE );
		m_checkIncludeTraining.ShowWindow( SW_HIDE );
	}

	return TRUE;  
}

/**********************************************************************/

void CPMSPrintUnpaidDlg::SetDefaults()
{
	m_bPending = TRUE;
	m_bActive = TRUE;
	m_bComplete = FALSE;
	m_bCancelled = FALSE;
	m_bPrintLines = FALSE;
	m_bIncludeTraining = FALSE;
}

/**********************************************************************/

void CPMSPrintUnpaidDlg::OnButtonDefault() 
{
	SetDefaults();
	UpdateData ( FALSE );
}

/**********************************************************************/

void CPMSPrintUnpaidDlg::OnButtonSave() 
{
	UpdateData( TRUE, FALSE );

	CSSFile fileConfig;
	CFilenameUpdater FnUp( SysFiles::PMSUnpaidDef );
	if ( fileConfig.Open ( FnUp.GetFilenameToUse(), "wb" ) == TRUE )
		fileConfig.WriteLine ( GetCSVLine() );
}

/**********************************************************************/

const char* CPMSPrintUnpaidDlg::GetCSVLine()
{
	CCSV csv;	
	csv.Add( m_bPending );
	csv.Add( m_bActive );
	csv.Add( m_bComplete );
	csv.Add( m_bCancelled );
	csv.Add( m_bPrintLines );
	csv.Add( m_bIncludeTraining && PMSOptions.GetMiscEnableTrainingMode() );
	m_strFieldSelect = csv.GetLine();
	return m_strFieldSelect;
}

/**********************************************************************/

void CPMSPrintUnpaidDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		if ( ( 0 == m_bPending ) && ( 0 == m_bActive ) && ( 0 == m_bComplete ) && ( 0 == m_bCancelled ) )
		{
			CString str = "";
			str += "You must select at least one booking status type\n";
			str += "to include in this report.";
			Prompter.Error( str );
		}
		else
		{
			CPMSReportUnpaid Report;
			Report.SetIncludePendingFlag( m_bPending != 0 );
			Report.SetIncludeActiveFlag( m_bActive != 0 );
			Report.SetIncludeCompleteFlag( m_bComplete != 0 );
			Report.SetIncludeCancelledFlag( m_bCancelled != 0 );
			Report.SetLinesFlag( m_bPrintLines != 0 );
			Report.SetIncludeTrainingFlag( m_bIncludeTraining != 0 );
			Report.CreateReport();
		}
	}
}

/**********************************************************************/
#endif
/**********************************************************************/


