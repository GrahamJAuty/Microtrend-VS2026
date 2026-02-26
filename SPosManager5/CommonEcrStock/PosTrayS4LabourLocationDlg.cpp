/**********************************************************************/
#include "PosTrayOptions.h"
/**********************************************************************/
#include "PosTrayS4LabourLocationDlg.h"
/**********************************************************************/

CPosTrayS4LabourLocationDlg::CPosTrayS4LabourLocationDlg( int nLocIdx, CS4LabourLocationOptions& Options, CWnd* pParent )
	: CSSDialog(CPosTrayS4LabourLocationDlg::IDD, pParent), m_Options( Options )
{
	m_nLocIdx = nLocIdx;
}

/**********************************************************************/

CPosTrayS4LabourLocationDlg::~CPosTrayS4LabourLocationDlg()
{
}

/**********************************************************************/

void CPosTrayS4LabourLocationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control( pDX, IDC_CHECK_ENABLE, m_checkEnable );
	DDX_Control( pDX, IDC_EDIT_LOCID, m_editLocID );
	DDX_Control(pDX, IDC_COMBO_TERMINAL, m_comboTerminal);
	DDX_Control(pDX, IDC_COMBO_HOUR, m_comboHour);
	DDX_Control(pDX, IDC_DATEPICKER_EXPORT, m_DatePickerExport);
	DDX_Control(pDX, IDC_STATIC_EXPORT, m_staticExport);
}

/**********************************************************************/
BEGIN_MESSAGE_MAP(CPosTrayS4LabourLocationDlg, CDialog)
	ON_BN_CLICKED( IDC_CHECK_ENABLE, OnCheckEnable)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATEPICKER_EXPORT, OnChangeDate)
	ON_CBN_SELCHANGE( IDC_COMBO_HOUR, OnSelectHour)
END_MESSAGE_MAP()
/**********************************************************************/

BOOL CPosTrayS4LabourLocationDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_editLocID.LimitText( MAXLENGTH_S4LABOUR_LOCID );
	
	m_checkEnable.SetCheck( m_Options.GetEnable() );
	OnCheckEnable();

	m_editLocID.SetWindowText( m_Options.GetLocID() );

	m_comboTerminal.AddString( "Terminal ID of sale" );
	m_comboTerminal.AddString( "Use zero for all sales" );
	m_comboTerminal.SetCurSel( m_Options.GetUseTermIDFlag() ? 0 : 1 );
	
	for ( int n = 0; n <= 23; n++ )
	{
		CString strHour;
		strHour.Format( "%2.2d:00", n );
		m_comboHour.AddString( strHour );
	}

	{
		int nSel = PosTrayOptions.GetS4LabourExportHour();
		if ( ( nSel >= 0 ) && ( nSel <= 23 ) )
			m_comboHour.SetCurSel(nSel);
		else
			m_comboHour.SetCurSel(0);
	}

	COleDateTime oleDateStart = COleDateTime( 2014, 1, 1, 0, 0, 0 );
	COleDateTime oleDateEnd = COleDateTime( 2500, 1, 1, 0, 0, 0 );

	m_DatePickerExport.SetRange ( &oleDateStart, &oleDateEnd );
	m_DatePickerExport.SetFormat( "ddd dd MMM yyy" );

	CString strDate = m_Options.GetLastExportDate( m_nLocIdx, TRUE );
	if ( strDate == "" )
		strDate = m_Options.GetLastExportDate( m_nLocIdx, FALSE );

	COleDateTime oleDayToExport, oleExportTime;
	if ( m_Options.GetNextExportTime( oleDayToExport, oleExportTime, strDate ) == FALSE )
		oleDayToExport = oleDateStart;
		
	if ( ( oleDayToExport < oleDateStart ) || ( oleDayToExport > oleDateEnd ) )
		oleDayToExport = oleDateStart;

	m_DatePickerExport.SetTime( oleDayToExport );

	ShowNextExportTime( oleExportTime );
	
	return TRUE;
}

/**********************************************************************/

void CPosTrayS4LabourLocationDlg::OnCheckEnable()
{
	bool bEnable = ( m_checkEnable.GetCheck() != 0 );
	m_editLocID.EnableWindow( bEnable );
	m_comboHour.EnableWindow( bEnable );
	m_DatePickerExport.EnableWindow( bEnable );
}

/**********************************************************************/

void CPosTrayS4LabourLocationDlg::UpdateNextExportTime()
{
	COleDateTime dateExport;
	m_DatePickerExport.GetTime( dateExport );

	if ( dateExport.GetStatus() == COleDateTime::valid )
	{
		dateExport -= COleDateTimeSpan( 1, 0, 0, 0 );

		CString strDate;
		strDate.Format( "%4.4d%2.2d%2.2d",
			dateExport.GetYear(),
			dateExport.GetMonth(),
			dateExport.GetDay() );

		COleDateTime oleDayToExport, oleExportTime;
		if ( m_Options.GetNextExportTime( oleDayToExport, oleExportTime, strDate ) == TRUE )
			ShowNextExportTime( oleExportTime );
	}
}

/**********************************************************************/

void CPosTrayS4LabourLocationDlg::ShowNextExportTime( COleDateTime& time )
{
	CString strExport;
	strExport.Format( "( Export due at %2.2d:%2.2d at %2.2d/%2.2d/%4.4d )",
		time.GetHour(),
		time.GetMinute(),
		time.GetDay(),
		time.GetMonth(),
		time.GetYear() );
	m_staticExport.SetWindowText( strExport );
}

/**********************************************************************/

void CPosTrayS4LabourLocationDlg::OnChangeDate(NMHDR *pNMHDR, LRESULT *pResult)
{
	UpdateNextExportTime();
	*pResult = 0;
}

/**********************************************************************/

void CPosTrayS4LabourLocationDlg::OnSelectHour()
{
	int nTemp = PosTrayOptions.GetS4LabourExportHour();
	PosTrayOptions.SetS4LabourExportHour( m_comboHour.GetCurSel() );
	UpdateNextExportTime();
	PosTrayOptions.SetS4LabourExportHour( nTemp );
}

/**********************************************************************/

void CPosTrayS4LabourLocationDlg::OnOK()
{
	if ( UpdateData( TRUE ) == TRUE )
	{
		m_Options.SetEnable( m_checkEnable.GetCheck() != 0 );
		
		CString str;
		m_editLocID.GetWindowText( str );
		m_Options.SetLocID( str );

		m_Options.SetUseTermIDFlag( m_comboTerminal.GetCurSel() == 0 );

		PosTrayOptions.SetS4LabourExportHour( m_comboHour.GetCurSel() );

		m_DatePickerExport.GetTime( m_oleDateNextExport );

		EndDialog( IDOK );
	}
}

/**********************************************************************/

