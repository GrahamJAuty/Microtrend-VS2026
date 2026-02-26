/**********************************************************************/
#include "DateRangeHelpers.h"
#include "LIPAExportDate.h"
#include "PosTrayOptions.h"
/**********************************************************************/
#include "PropPagePosTrayOptionsLIPA.h"
/**********************************************************************/

CPropPagePosTrayOptionsLIPA::CPropPagePosTrayOptionsLIPA() : CSSPropertyPage(CPropPagePosTrayOptionsLIPA::IDD)
{
	//{{AFX_DATA_INIT(CPropPagePosTrayOptionsLIPA)
	//}}AFX_DATA_INIT
}

/**********************************************************************/

CPropPagePosTrayOptionsLIPA::~CPropPagePosTrayOptionsLIPA()
{
}

/**********************************************************************/

void CPropPagePosTrayOptionsLIPA::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPagePosTrayOptionsLIPA)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_DATEPICKER_EXPORT, m_DatePickerExport);
	DDX_Control(pDX, IDC_STATIC_EXPORT, m_staticExport);
	DDX_Control(pDX, IDC_COMBO_HOUR, m_comboHour);
	DDX_Control(pDX, IDC_CHECK_FOLDER, m_checkFolder);
	DDX_Control(pDX, IDC_EDIT_FOLDER, m_editFolder);
	DDX_Control(pDX, IDC_BUTTON_FOLDER, m_buttonFolder);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPagePosTrayOptionsLIPA, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPagePosTrayOptionsLIPA)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATEPICKER_EXPORT, OnChangeDate)
	ON_CBN_SELCHANGE( IDC_COMBO_HOUR, OnSelectHour)
	ON_BN_CLICKED(IDC_CHECK_FOLDER, OnToggleFolder)
	ON_BN_CLICKED(IDC_BUTTON_FOLDER, OnButtonFolder)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPropPagePosTrayOptionsLIPA::OnInitDialog() 
{
	GetRecordData();
	CPropertyPage::OnInitDialog();
	
	COleDateTime oleDateStart = COleDateTime( 2014, 1, 1, 0, 0, 0 );
	COleDateTime oleDateEnd = COleDateTime( 2500, 1, 1, 0, 0, 0 );

	m_DatePickerExport.SetRange ( &oleDateStart, &oleDateEnd );
	m_DatePickerExport.SetFormat( "ddd dd MMM yyy" );

	CLIPAExportDate LIPAExportDate;

	CString strDate = LIPAExportDate.GetLastExportDate( TRUE );
	if (strDate == "")
	{
		strDate = LIPAExportDate.GetLastExportDate(FALSE);
	}

	COleDateTime oleDayToExport, oleExportTime;
	if (LIPAExportDate.GetNextExportTime(oleDayToExport, oleExportTime, strDate) == FALSE)
	{
		oleDayToExport = oleDateStart;
	}

	if ((oleDayToExport < oleDateStart) || (oleDayToExport > oleDateEnd))
	{
		oleDayToExport = oleDateStart;
	}

	m_DatePickerExport.SetTime( oleDayToExport );

	for ( int n = 0; n <= 23; n++ )
	{
		CString strHour;
		strHour.Format( "%2.2d:00", n );
		m_comboHour.AddString( strHour );
	}

	{
		int nSel = PosTrayOptions.GetLIPAExportHour();
		if ((nSel >= 0) && (nSel <= 23))
		{
			m_comboHour.SetCurSel(nSel);
		}
		else
		{
			m_comboHour.SetCurSel(0);
		}
	}

	ShowNextExportTime( oleExportTime );

	{
		bool bEnableFolder = TRUE;
	
		CString strFolder = PosTrayOptions.GetLIPAExportFolder();
		::TrimSpaces( strFolder, FALSE );
		if ( strFolder == "" )
		{
			strFolder = "LIPA\\Export";
			::GetSyssetProgramPath( strFolder );
			strFolder = strFolder.Right(23);
			bEnableFolder = FALSE;
		}
		
		m_checkFolder.SetCheck( bEnableFolder );
		m_editFolder.SetWindowText( strFolder );
		m_editFolder.SetReadOnly( FALSE == bEnableFolder );
		m_buttonFolder.EnableWindow( bEnableFolder );
	}

	return TRUE;  
}

/**********************************************************************/

void CPropPagePosTrayOptionsLIPA::UpdateNextExportTime()
{
	COleDateTime dateExport;
	m_DatePickerExport.GetTime( dateExport );

	int nOldHour = PosTrayOptions.GetLIPAExportHour();

	if (dateExport.GetStatus() == COleDateTime::valid)
	{
		PosTrayOptions.SetLIPAExportHour(m_comboHour.GetCurSel());

		dateExport -= COleDateTimeSpan(1, 0, 0, 0);

		CString strDate;
		strDate.Format("%4.4d%2.2d%2.2d",
			dateExport.GetYear(),
			dateExport.GetMonth(),
			dateExport.GetDay());

		CLIPAExportDate LIPAExportDate;

		COleDateTime oleDayToExport, oleExportTime;
		if (LIPAExportDate.GetNextExportTime(oleDayToExport, oleExportTime, strDate) == TRUE)
		{
			ShowNextExportTime(oleExportTime);
		}
	}

	PosTrayOptions.SetLIPAExportHour( nOldHour );
}

/**********************************************************************/

void CPropPagePosTrayOptionsLIPA::ShowNextExportTime( COleDateTime& time )
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

void CPropPagePosTrayOptionsLIPA::OnChangeDate(NMHDR *pNMHDR, LRESULT *pResult)
{
	UpdateNextExportTime();
	*pResult = 0;
}

/**********************************************************************/

void CPropPagePosTrayOptionsLIPA::OnToggleFolder()
{
	bool bEnableFolder = ( m_checkFolder.GetCheck() != 0 );

	CString strFolder = "";
	if ( FALSE == bEnableFolder )
	{
		strFolder = "LIPA\\Export";
		::GetSyssetProgramPath( strFolder );
		strFolder = strFolder.Right(26);
	}
		
	m_checkFolder.SetCheck( bEnableFolder );
	m_editFolder.SetWindowText( strFolder );
	m_editFolder.SetReadOnly( FALSE == bEnableFolder );
	m_buttonFolder.EnableWindow( bEnableFolder );
}

/**********************************************************************/

void CPropPagePosTrayOptionsLIPA::OnButtonFolder()
{
	CString strPathname = "";
	if (BrowseFolder(strPathname, "Select Export Folder Path", NULL, this) == TRUE)
	{
		m_editFolder.SetWindowText(strPathname);
	}
}

/**********************************************************************/

BOOL CPropPagePosTrayOptionsLIPA::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	SaveRecord();
	return bResult;
}

/**********************************************************************/

BOOL CPropPagePosTrayOptionsLIPA::OnSetActive() 
{
	GetRecordData();
	return ( CPropertyPage::OnSetActive() != 0 );
}

/**********************************************************************/

void CPropPagePosTrayOptionsLIPA::GetRecordData()
{
}

/**********************************************************************/

void CPropPagePosTrayOptionsLIPA::OnSelectHour()
{
	int nTemp = PosTrayOptions.GetLIPAExportHour();
	PosTrayOptions.SetLIPAExportHour( m_comboHour.GetCurSel() );
	UpdateNextExportTime();
	PosTrayOptions.SetLIPAExportHour( nTemp );
}

/**********************************************************************/

void CPropPagePosTrayOptionsLIPA::SaveRecord()
{
	m_DatePickerExport.GetTime( m_oleDateNextExport );

	CLIPAExportDate LIPAExportDate;
	COleDateTime dateNextExport;
	GetDateNextExport( dateNextExport );
	CDateRangeHelpers DateRangeHelpers;
	DateRangeHelpers.AddDaysToTime( dateNextExport, -1 );
	LIPAExportDate.SetLastExportDate( dateNextExport, TRUE );

	{
		CString strExportFolder = "";
		
		if (m_checkFolder.GetCheck() != 0)
		{
			m_editFolder.GetWindowText(strExportFolder);
		}

		::TrimSpaces( strExportFolder, FALSE );

		PosTrayOptions.SetLIPAExportFolder( strExportFolder );
	}

	PosTrayOptions.SetLIPAExportHour( m_comboHour.GetCurSel() );
}

/**********************************************************************/
