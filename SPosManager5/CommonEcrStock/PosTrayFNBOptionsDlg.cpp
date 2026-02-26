/**********************************************************************/
#include "..\CommonEcrStock\DefEditDlg.h"
/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "MaxLengths.h"
#include "PosTrayOptions.h"
/**********************************************************************/
#include "PosTrayFNBOptionsDlg.h"
/**********************************************************************/

CPosTrayFNBOptionsDlg::CPosTrayFNBOptionsDlg( int nDbIdx, CFNBDatabaseOptions& Options, CWnd* pParent )
	: CSSDialog(CPosTrayFNBOptionsDlg::IDD, pParent), m_Options( Options )
{
	m_nDbIdx = nDbIdx;
	m_nPause = PosTrayOptions.GetFNBDelayTime();
	m_strNewPassword = "";
	m_bGotOldPassword = FALSE;
}

/**********************************************************************/

CPosTrayFNBOptionsDlg::~CPosTrayFNBOptionsDlg()
{
}

/**********************************************************************/

void CPosTrayFNBOptionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control( pDX, IDC_CHECK_ENABLE, m_checkEnable );
	DDX_Control( pDX, IDC_COMBO_LOCATION, m_comboLocation );
	DDX_Control( pDX, IDC_EDIT_SERVER, m_editServer );
	DDX_Control( pDX, IDC_EDIT_USERNAME, m_editUserName );
	DDX_Control( pDX, IDC_EDIT_PASSWORD, m_editPassword );
	DDX_Control( pDX, IDC_BUTTON_PASSWORD, m_buttonPassword );
	DDX_Control( pDX, IDC_EDIT_FOLDER, m_editFolder );
	DDX_Text( pDX, IDC_EDIT_PAUSE, m_nPause );
	DDV_MinMaxInt( pDX, m_nPause, 1, 6 );
	DDX_Control(pDX, IDC_DATEPICKER_EXPORT, m_DatePickerExport);
	DDX_Control(pDX, IDC_STATIC_EXPORT, m_staticExport);
}

/**********************************************************************/
BEGIN_MESSAGE_MAP(CPosTrayFNBOptionsDlg, CDialog)
	ON_BN_CLICKED( IDC_CHECK_ENABLE, OnCheckEnable)
	ON_BN_CLICKED( IDC_BUTTON_PASSWORD, OnButtonPassword )
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATEPICKER_EXPORT, OnChangeDate)
	ON_EN_KILLFOCUS( IDC_EDIT_PAUSE, OnKillFocusPause)
END_MESSAGE_MAP()
/**********************************************************************/

BOOL CPosTrayFNBOptionsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	SubclassEdit(IDC_EDIT_PAUSE, SS_NUM, 1, "%d");

	CString strTitle;
	strTitle.Format("Configure Settings for FNB Export (%s)",
		dbDatabase.GetName(m_nDbIdx));

	SetWindowText(strTitle);

	m_editServer.LimitText(MAX_LENGTH_FTP_SERVER);
	m_editUserName.LimitText(MAX_LENGTH_FTP_USERNAME);
	m_editFolder.LimitText(MAX_LENGTH_FTP_FOLDER);

	m_checkEnable.SetCheck(m_Options.GetEnable());
	OnCheckEnable();

	CWordArray arrayTypes;
	arrayTypes.Add(NODE_DATABASE);
	arrayTypes.Add(NODE_LOCATION);
	arrayTypes.Add(NODE_LOCATIONSET);
	arrayTypes.Add(NODE_REPORTPOINT);
	m_LocationSelector.SetSingleDbIdx(m_nDbIdx);
	m_LocationSelector.BuildList(arrayTypes);
	m_LocationSelector.FillLocationCombo(&m_comboLocation);

	CLocationSelectorLookup Lookup(m_nDbIdx, m_Options);
	CLocationSelectorEntity LocSelEntity(Lookup);
	m_LocationSelector.ForceSelection(LocSelEntity);

	m_editServer.SetWindowText(m_Options.GetFTPServer());
	m_editUserName.SetWindowText(m_Options.GetFTPUserName());
	m_editFolder.SetWindowText(m_Options.GetFTPFolder());

	CString strPassword = m_Options.GetFTPPassword();

	if (strPassword != "")
	{
		m_editPassword.SetWindowText("<The password has been set>");
		m_bGotOldPassword = TRUE;
	}
	else
	{
		m_editPassword.SetWindowText("<No password set>");
		m_bGotOldPassword = FALSE;
	}

	COleDateTime oleDateStart = COleDateTime(2014, 1, 1, 0, 0, 0);
	COleDateTime oleDateEnd = COleDateTime(2500, 1, 1, 0, 0, 0);

	m_DatePickerExport.SetRange(&oleDateStart, &oleDateEnd);
	m_DatePickerExport.SetFormat("ddd dd MMM yyy");

	CString strDate = m_Options.GetLastExportDate(m_nDbIdx, TRUE);
	if (strDate == "")
	{
		strDate = m_Options.GetLastExportDate(m_nDbIdx, FALSE);
	}

	COleDateTime oleDayToExport, oleExportTime;
	if (m_Options.GetNextExportTime(oleDayToExport, oleExportTime, strDate) == FALSE)
	{
		oleDayToExport = oleDateStart;
	}

	if ((oleDayToExport < oleDateStart) || (oleDayToExport > oleDateEnd))
	{
		oleDayToExport = oleDateStart;
	}

	m_DatePickerExport.SetTime(oleDayToExport);

	ShowNextExportTime(oleExportTime);

	return TRUE;
}

/**********************************************************************/
CEdit* CPosTrayFNBOptionsDlg::GetEditPause(){ return GetEdit( IDC_EDIT_PAUSE ); }
/**********************************************************************/

void CPosTrayFNBOptionsDlg::OnCheckEnable()
{
	bool bEnable = ( m_checkEnable.GetCheck() != 0 );
	m_comboLocation.EnableWindow( bEnable );
	m_editServer.EnableWindow( bEnable );
	m_editUserName.EnableWindow( bEnable );
	m_buttonPassword.EnableWindow( bEnable );
	m_editFolder.EnableWindow( bEnable );
}

/**********************************************************************/

void CPosTrayFNBOptionsDlg::UpdateNextExportTime()
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

void CPosTrayFNBOptionsDlg::ShowNextExportTime( COleDateTime& time )
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

void CPosTrayFNBOptionsDlg::OnChangeDate(NMHDR *pNMHDR, LRESULT *pResult)
{
	UpdateNextExportTime();
	*pResult = 0;
}

/**********************************************************************/

void CPosTrayFNBOptionsDlg::OnKillFocusPause()
{
	CString str;
	GetEditPause() -> GetWindowText( str );
	
	int nTemp = PosTrayOptions.GetFNBDelayTime();
	PosTrayOptions.SetFNBDelayTime( atoi( str ) );
	UpdateNextExportTime();
	PosTrayOptions.SetFNBDelayTime( nTemp );
}

/**********************************************************************/

void CPosTrayFNBOptionsDlg::OnButtonPassword()
{
	CDefEditDlg dlg( "Set FNB Password", "", MAX_LENGTH_FTP_PASSWORD, "Password", this );
	if ( dlg.DoModal() == IDOK )
	{
		if ( dlg.m_strName != "" )
		{
			m_strNewPassword = dlg.m_strName;
			
			if ( TRUE == m_bGotOldPassword )
				m_editPassword.SetWindowText ( "<The password has been changed>" );
			else
				m_editPassword.SetWindowText ( "<The password has been set>" );
		}
	}
}

/**********************************************************************/

void CPosTrayFNBOptionsDlg::OnOK()
{
	if ( UpdateData( TRUE ) == TRUE )
	{
		m_Options.SetEnable( m_checkEnable.GetCheck() != 0 );
		
		CString str;
		m_editServer.GetWindowText( str );
		m_Options.SetFTPServer( str );

		m_editUserName.GetWindowText( str );
		m_Options.SetFTPUserName( str );

		m_editFolder.GetWindowText( str );
		m_Options.SetFTPFolder( str );

		if (m_strNewPassword != "")
		{
			m_Options.SetFTPPassword(m_strNewPassword);
		}

		CLocationSelectorEntity LocSelEntity;
		m_LocationSelector.GetSelectedEntity( LocSelEntity );
		m_Options.SetLocSelType( LocSelEntity.GetConType() );
		m_Options.SetLocSelNwkNo( LocSelEntity.GetNwkNo() );
		m_Options.SetLocSelEntityNo( LocSelEntity.GetEntityNo() );
		
		PosTrayOptions.SetFNBDelayTime( m_nPause );

		m_DatePickerExport.GetTime( m_oleDateNextExport );

		EndDialog( IDOK );
	}
}

/**********************************************************************/

