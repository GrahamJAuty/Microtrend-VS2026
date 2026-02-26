/**********************************************************************/
#include "EposReportCustomSettings.h"
#include "Password.h"
/**********************************************************************/
#include "EposReportCustomHourlyDlg.h"
/**********************************************************************/

CEposReportCustomHourlyDlg::CEposReportCustomHourlyDlg( int nFamily, CEposReportSelect& EposReportSelect, CEposReportSelectInfo& infoReport, bool bAdhoc, CWnd* pParent)
: CDialog(CEposReportCustomHourlyDlg::IDD, pParent), m_EposReportSelect( EposReportSelect ), m_infoReport( infoReport )
{
	m_nFamily = nFamily;
	m_bAdhoc = bAdhoc;
}

/**********************************************************************/

CEposReportCustomHourlyDlg::~CEposReportCustomHourlyDlg()
{
}

/**********************************************************************/

void CEposReportCustomHourlyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_NAME, m_editName);
	DDX_Control(pDX, IDC_COMBO_TIMESLICE, m_comboTimeSlice);
	DDX_Control(pDX, IDC_CHECK_ADHOC, m_checkAdhoc);
	DDX_Control(pDX, IDC_BUTTON_SAVE, m_buttonSave);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CEposReportCustomHourlyDlg, CDialog)
	ON_BN_CLICKED( IDC_BUTTON_SAVE,OnButtonSave)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CEposReportCustomHourlyDlg::OnInitDialog() 
{
	CEposReportCustomSettingsHourly Settings;
	CString strParams = m_infoReport.GetCustomReportParams();
	Settings.SetSettingsCSVLine( strParams );
	
	CDialog::OnInitDialog();
	
	m_editName.LimitText( EPOS_CUSTOM_MAXLEN_NAME );
	m_editName.SetWindowText( m_infoReport.GetCustomReportName() );

	CArray<int,int> arrayTimeTypes;
	arrayTimeTypes.Add( SH_TIMESLICE_10MIN );
	arrayTimeTypes.Add( SH_TIMESLICE_15MIN );
	arrayTimeTypes.Add( SH_TIMESLICE_30MIN );
	arrayTimeTypes.Add( SH_TIMESLICE_60MIN );

	for ( int n = 0; n < arrayTimeTypes.GetSize(); n++ )
	{
		CTimeSliceMapCSVRecord ReportRecord;
		ReportRecord.SetReportType( arrayTimeTypes.GetAt(n) );
		m_comboTimeSlice.AddString ( ReportRecord.GetListName() );
	}

	for ( int n = 0; n < DataManagerNonDb.TimeSliceList.GetSize(); n++ )
	{
		if ( DataManagerNonDb.TimeSliceList.GetReportType(n) == SH_CUSTOM )
		{
			int nPos = m_comboTimeSlice.AddString( DataManagerNonDb.TimeSliceList.GetName(n) );
			m_comboTimeSlice.SetItemData( nPos, n );
		}
	}

	m_comboTimeSlice.SetCurSel(2);

	bool bFound = FALSE;
	for ( int n = 0; n < arrayTimeTypes.GetSize(); n++ )
	{
		if ( Settings.GetTimeSliceType() == arrayTimeTypes.GetAt(n) )
		{
			m_comboTimeSlice.SetCurSel(n);
			bFound = TRUE;
			break;
		}
	}

	if ( FALSE == bFound )
	{
		CString strEntityID = Settings.GetTimeSliceEntityID();
		for ( int n = arrayTimeTypes.GetSize(); n < m_comboTimeSlice.GetCount(); n++ )
		{
			int nIdx = m_comboTimeSlice.GetItemData(n);
			if ( strEntityID == DataManagerNonDb.TimeSliceList.GetEntityID(nIdx) )
			{
				m_comboTimeSlice.SetCurSel(n);
				break;
			}
		}
	}
	
	m_checkAdhoc.SetCheck( Settings.GetAllowAdhocFlag() );
	
	bool bEnableSave = TRUE;
	bEnableSave &= PasswordArray.GetEnable( PasswordTicks::SetupMenu );
	bEnableSave &= PasswordArray.GetEnable( PasswordTicks::SetupMenuTexts );

	CString strType = "";
	
	switch( m_nFamily )
	{
	case EPOS_CUSTOM_FAMILY_HOURLYGRP:
		strType = "Hourly Group";
		break;

	case EPOS_CUSTOM_FAMILY_HOURLYDPT:
		strType = "Hourly Department";
		break;

	case EPOS_CUSTOM_FAMILY_COVERS_HOURLY:
		strType = "Covers Hourly";
		break;

	case EPOS_CUSTOM_FAMILY_COVERS_HOURLYGRP:
		strType = "Covers Hourly Group";
		break;

	case EPOS_CUSTOM_FAMILY_COVERS_HOURLYDPT:
		strType = "Covers Hourly Department";
		break;

	case EPOS_CUSTOM_FAMILY_HOURLY:
	default:
		strType = "Hourly";					  
		break;	
	}

	if ( TRUE == m_bAdhoc )
	{
		m_editName.EnableWindow( FALSE );
		m_checkAdhoc.ShowWindow( SW_HIDE );
		m_checkAdhoc.EnableWindow( FALSE );
		
		CString strTitle;
		strTitle.Format( "Adhoc %s Report Settings",
			(const char*) strType );
			
		SetWindowText( strTitle );
	}
	else
	{
		CString strTitle;
		strTitle.Format( "Setup Custom %s Report",
			(const char*) strType );
			
		SetWindowText( strTitle );
	}
	
	if ( ( FALSE == m_bAdhoc ) || ( FALSE == bEnableSave ) )
	{
		m_buttonSave.ShowWindow( SW_HIDE );
		m_buttonSave.EnableWindow( FALSE );
	}

	return TRUE;
}

/**********************************************************************/

bool CEposReportCustomHourlyDlg::GetUpdatedSettings( CEposReportSelectInfo& infoReport )
{
	UpdateData( TRUE );

	CEposReportCustomSettingsHourly Settings;
	Settings.SetAllowAdhocFlag( m_checkAdhoc.GetCheck() != 0 );
	
	infoReport.SetCustomReportFamily( m_nFamily );

	{
		CString strText;
		m_editName.GetWindowText( strText );
		infoReport.SetCustomReportName( strText );
	}

	int nSel = m_comboTimeSlice.GetCurSel(); 
	
	switch( nSel )
	{
	case 0:		
		Settings.SetTimeSliceType( SH_TIMESLICE_10MIN );
		Settings.SetTimeSliceEntityID( "" );
		break;

	case 1:
		Settings.SetTimeSliceType( SH_TIMESLICE_15MIN );
		Settings.SetTimeSliceEntityID( "" );
		break;

	case 2:
		Settings.SetTimeSliceType( SH_TIMESLICE_30MIN );
		Settings.SetTimeSliceEntityID( "" );
		break;

	case 3:
		Settings.SetTimeSliceType( SH_TIMESLICE_60MIN );
		Settings.SetTimeSliceEntityID( "" );
		break;

	default:
		if ( ( nSel >= 4 ) && ( nSel < m_comboTimeSlice.GetCount() ) )
		{
			int nIdx = m_comboTimeSlice.GetItemData( nSel );
			Settings.SetTimeSliceType( SH_CUSTOM );
			Settings.SetTimeSliceEntityID( DataManagerNonDb.TimeSliceList.GetEntityID(nIdx) );
		}
		else
		{
			Settings.SetTimeSliceType( SH_TIMESLICE_30MIN );
			Settings.SetTimeSliceEntityID( "" );
		}
		break;
	}

	CString strLine;
	Settings.GetSettingsCSVLine( strLine );
	infoReport.SetCustomReportParams( strLine );
	
	return TRUE;
}

/**********************************************************************/

void CEposReportCustomHourlyDlg::OnButtonSave()
{
	CEposReportSelectInfo infoReport = m_infoReport;
	
	if ( GetUpdatedSettings( infoReport ) == TRUE )
	{
		m_EposReportSelect.SetReportInfo( infoReport );
		m_EposReportSelect.SaveReportSelection();
	}
}

/**********************************************************************/

void CEposReportCustomHourlyDlg::OnOK() 
{
	if ( GetUpdatedSettings( m_infoReport ) == TRUE )
		EndDialog( IDOK );
}

/**********************************************************************/
