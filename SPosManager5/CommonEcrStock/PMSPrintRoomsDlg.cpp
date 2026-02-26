/**********************************************************************/
#ifdef COMPILE_PMS
/**********************************************************************/
#include "PMSOptions.h"
#include "PMSReportRoomList.h"
/**********************************************************************/
#include "PMSPrintRoomsDlg.h"
/**********************************************************************/

CPMSPrintRoomsDlg::CPMSPrintRoomsDlg( CWnd* pParent /*=NULL*/)
	: CSSDialog(CPMSPrintRoomsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPMSPrintRoomsDlg)
	//}}AFX_DATA_INIT
}

/**********************************************************************/

void CPMSPrintRoomsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPMSPrintRoomsDlg)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_CHECK_OCCUPIED1, m_checkOccupied1);
	DDX_Control(pDX, IDC_CHECK_OCCUPIED2, m_checkOccupied2);
	DDX_Control(pDX, IDC_CHECK_OCCUPIED3, m_checkOccupied3);
	DDX_Control(pDX, IDC_CHECK_VACANT1, m_checkVacant1);
	DDX_Control(pDX, IDC_CHECK_VACANT2, m_checkVacant2);
	DDX_Control(pDX, IDC_CHECK_VACANT3, m_checkVacant3);
	DDX_Control(pDX, IDC_CHECK_BALANCE, m_checkBalance);
	DDX_Control(pDX, IDC_CHECK_LINES, m_checkLines);
	DDX_Control(pDX, IDC_CHECK_TRAINING, m_checkTraining);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPMSPrintRoomsDlg, CDialog)
	//{{AFX_MSG_MAP(CPMSPrintRoomsDlg)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_DEFAULT, OnButtonDefault)
	ON_BN_CLICKED(IDC_BUTTON_ALL, OnButtonAll)
	ON_BN_CLICKED(IDC_BUTTON_NONE, OnButtonNone)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPMSPrintRoomsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CString strBuffer;
	bool bGotFile = FALSE;
	
	CSSFile fileConfig;

	CFilenameUpdater FnUp( SysFiles::PMSRoomDef );
	if ( fileConfig.Open ( FnUp.GetFilenameToUse(), "rb" ) == TRUE )
	{
		if ( fileConfig.ReadString ( strBuffer ) == TRUE )
		{
			bGotFile = TRUE;
			
			CCSVPMSPrintRoomKey csv ( strBuffer );

			m_checkOccupied1.SetCheck( csv.GetFilterOccupied1() );
			m_checkOccupied2.SetCheck( csv.GetFilterOccupied2() );
			m_checkOccupied3.SetCheck( csv.GetFilterOccupied3() );
			m_checkVacant1.SetCheck( csv.GetFilterVacant1() );
			m_checkVacant2.SetCheck( csv.GetFilterVacant2() );
			m_checkVacant3.SetCheck( csv.GetFilterVacant3() );
			m_checkBalance.SetCheck( csv.GetBalanceFlag() );
			m_checkLines.SetCheck( csv.GetLinesFlag() );
			m_checkTraining.SetCheck( csv.GetTrainingFlag() & PMSOptions.GetMiscEnableTrainingMode() );
			
			UpdateData ( FALSE );
		}
	}

	if ( PMSOptions.GetMiscEnableTrainingMode() == FALSE )
	{
		m_checkTraining.SetCheck( FALSE );
		m_checkTraining.ShowWindow( SW_HIDE );
		m_checkTraining.EnableWindow( FALSE );
	}
		
	if ( bGotFile == FALSE )
		OnButtonDefault();
	
	return TRUE;  
}

/**********************************************************************/

void CPMSPrintRoomsDlg::OnButtonDefault() 
{
	OnButtonAll();
	m_checkLines.SetCheck( FALSE );
	m_checkBalance.SetCheck( FALSE );
	m_checkTraining.SetCheck( FALSE );
	UpdateData( FALSE );
}

/**********************************************************************/

void CPMSPrintRoomsDlg::OnButtonSave() 
{
	UpdateData( TRUE, FALSE );

	CSSFile fileConfig;
	CFilenameUpdater FnUp( SysFiles::PMSRoomDef );
	if ( fileConfig.Open ( FnUp.GetFilenameToUse(), "wb" ) == TRUE )
		fileConfig.WriteLine ( GetCSVLine() );
}

/**********************************************************************/

void CPMSPrintRoomsDlg::OnButtonAll()
{
	m_checkOccupied1.SetCheck( TRUE );
	m_checkOccupied2.SetCheck( TRUE );
	m_checkOccupied3.SetCheck( TRUE );
	m_checkVacant1.SetCheck( TRUE );
	m_checkVacant2.SetCheck( TRUE );
	m_checkVacant3.SetCheck( TRUE );
}

/**********************************************************************/

void CPMSPrintRoomsDlg::OnButtonNone()
{
	m_checkOccupied1.SetCheck( FALSE );
	m_checkOccupied2.SetCheck( FALSE );
	m_checkOccupied3.SetCheck( FALSE );
	m_checkVacant1.SetCheck( FALSE );
	m_checkVacant2.SetCheck( FALSE );
	m_checkVacant3.SetCheck( FALSE );
}

/**********************************************************************/

const char* CPMSPrintRoomsDlg::GetCSVLine()
{
	CCSV csv;	
	csv.Add( m_checkOccupied1.GetCheck() );
	csv.Add( m_checkOccupied2.GetCheck() );
	csv.Add( m_checkOccupied3.GetCheck() );
	csv.Add( m_checkVacant1.GetCheck() );
	csv.Add( m_checkVacant2.GetCheck() );
	csv.Add( m_checkVacant3.GetCheck() );
	csv.Add( m_checkBalance.GetCheck() );
	csv.Add( m_checkLines.GetCheck() );
	csv.Add( m_checkTraining.GetCheck() && PMSOptions.GetMiscEnableTrainingMode() );
	m_strFieldSelect = csv.GetLine();
	return m_strFieldSelect;
}

/**********************************************************************/

void CPMSPrintRoomsDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		CPMSReportRoomList RoomListReport;

		RoomListReport.SetFilterOccupied1( m_checkOccupied1.GetCheck() != 0 );
		RoomListReport.SetFilterOccupied2( m_checkOccupied2.GetCheck() != 0 );
		RoomListReport.SetFilterOccupied3( m_checkOccupied3.GetCheck() != 0 );
		RoomListReport.SetFilterVacant1( m_checkVacant1.GetCheck() != 0 );
		RoomListReport.SetFilterVacant2( m_checkVacant2.GetCheck() != 0 );
		RoomListReport.SetFilterVacant3( m_checkVacant3.GetCheck() != 0 );
		RoomListReport.SetBalanceFlag( m_checkBalance.GetCheck() != 0 );
		RoomListReport.SetLinesFlag( m_checkLines.GetCheck() != 0 );
		RoomListReport.SetTrainingFlag( m_checkTraining.GetCheck() != 0 );
		
		RoomListReport.CreateReport();
	}
}

/**********************************************************************/
#endif
/**********************************************************************/
