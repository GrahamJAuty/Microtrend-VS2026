/**********************************************************************/
#include <share.h>
/**********************************************************************/
#include "CashRspImporter.h"
#include "CommsHandshaker.h"
#include "DateRangeHelpers.h"
#include "NetworkCSVArray.h"
#include "NodeTypes.h"
#include "Password.h"
#include "PresetReportManager.h"
#include "ReportTimeAndAttendance.h"
#include "Repspawn.h"
#include "ServerSelectDlg.h"
#include "SessionEditDlg.h"
#include "StatusProgress.h"
/**********************************************************************/
#include "ReportTimeAndAttendanceDlg.h"
/**********************************************************************/

CReportTimeAndAttendanceDlg::CReportTimeAndAttendanceDlg( int nPresetNo, bool bAutoRunPreset, CWnd* pParent /*=NULL*/)
	: CSSDialog(CReportTimeAndAttendanceDlg::IDD, pParent),
	m_DateSelectorRange( m_comboDates, m_DatePickerFrom, m_DatePickerTo )
{
	//{{AFX_DATA_INIT(CReportTimeAndAttendanceDlg)
	//}}AFX_DATA_INIT
	m_nPresetNo = nPresetNo;
	m_nPresetError = 0;
	m_bAutoRunPreset = bAutoRunPreset;
	m_nServerPickType = 0;
	m_nServerStart = 1;
	m_nServerEnd = 9999;
	m_ServerSelectionList.BuildList();
}

/**********************************************************************/

void CReportTimeAndAttendanceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CReportTimeAndAttendanceDlg)
	DDX_Control(pDX, IDC_COMBO_ROUND, m_comboRound);
	DDX_Control(pDX, IDC_COMBO_DATES, m_comboDates);
	DDX_Control(pDX, IDC_COMBO_SESSION, m_comboSession);
	DDX_Control(pDX, IDC_STATIC_PRESET, m_staticPreset);
	DDX_Control(pDX, IDC_BUTTON_PRESET, m_buttonPreset);
	DDX_Control(pDX, IDC_COMBO_PRESET, m_comboPreset);
	DDX_Control(pDX, IDC_COMBO_REPORTTYPE, m_comboReportType);
	DDX_Control(pDX, IDC_COMBO_EDIT, m_comboEdit);
	DDX_Control(pDX, IDC_COMBO_GROUP, m_comboGroup);
	DDX_Control(pDX, IDC_COMBO_TOTALHOURS, m_comboTotalHours);
	DDX_Control(pDX, IDC_CHECK_CLOSINGTIME, m_checkClosingTime);
	DDX_Control(pDX, IDC_CHECK_OPENSHIFT, m_checkOpenShift);
	DDX_Control(pDX, IDC_DATEPICKER_TO, m_DatePickerTo);
	DDX_Control(pDX, IDC_DATEPICKER_FROM, m_DatePickerFrom);
	DDX_Control(pDX, IDC_COMBO_LOCATION, m_comboLocation);
	DDX_Control(pDX, IDC_RADIO_ALL, m_radioAll);
	DDX_Control(pDX, IDC_RADIO_RANGE, m_radioRange);
	DDX_Control(pDX, IDC_RADIO_LIST, m_radioList);
	DDX_Text(pDX, IDC_EDIT_SERVER1, m_nServerStart );
	DDV_MinMaxInt( pDX, m_nServerStart, Server::ServerNo.Min, Server::ServerNo.Max );
	DDX_Text(pDX, IDC_EDIT_SERVER2, m_nServerEnd );
	DDV_MinMaxInt( pDX, m_nServerEnd, Server::ServerNo.Min, Server::ServerNo.Max );
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CReportTimeAndAttendanceDlg, CDialog)
	//{{AFX_MSG_MAP(CReportTimeAndAttendanceDlg)
	ON_CBN_SELCHANGE(IDC_COMBO_DATES, OnSelectDate)
	ON_BN_CLICKED(IDC_BUTTON_PRESET, OnButtonPreset)
	ON_CBN_SELCHANGE(IDC_COMBO_PRESET, OnSelectPreset)
	ON_CBN_SELCHANGE(IDC_COMBO_REPORTTYPE, OnSelectReportType)
	ON_BN_CLICKED(IDC_BUTTON_DISPLAY, OnButtonDisplay)
	ON_BN_CLICKED(IDC_RADIO_ALL, OnRadioAll)
	ON_BN_CLICKED(IDC_RADIO_RANGE, OnRadioRange)
	ON_BN_CLICKED(IDC_RADIO_LIST, OnRadioList)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATEPICKER_FROM, OnChangeDateFrom)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATEPICKER_TO, OnChangeDateTo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CReportTimeAndAttendanceDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SubclassEdit(IDC_EDIT_SERVER1, SS_NUM, 4, "%d");
	SubclassEdit(IDC_EDIT_SERVER2, SS_NUM, 4, "%d");

	CWordArray arrayTypes;

	switch (PasswordArray.GetLocationAccessType())
	{
	case LOCATION_ACCESS_SYSTEM:
		arrayTypes.Add(NODE_LOCATION);
		arrayTypes.Add(NODE_DATABASE);
		arrayTypes.Add(NODE_LOCATIONSET);
		arrayTypes.Add(NODE_REPORTPOINT);
		arrayTypes.Add(NODE_SYSTEM);
		break;

	case LOCATION_ACCESS_DB:
		arrayTypes.Add(NODE_LOCATION);
		arrayTypes.Add(NODE_DATABASE);
		arrayTypes.Add(NODE_LOCATIONSET);
		arrayTypes.Add(NODE_REPORTPOINT);
		m_LocationSelector.SetSingleDbIdx(PasswordArray.GetLocationAccessDbIdx());
		break;

	case LOCATION_ACCESS_LOC:
		arrayTypes.Add(NODE_LOCATION);
		m_LocationSelector.SetSingleLocIdx(PasswordArray.GetLocationAccessLocIdx());
		break;
	}

	m_LocationSelector.ExcludeWebSales();
	m_LocationSelector.BuildList(arrayTypes);
	m_LocationSelector.FillLocationCombo(&m_comboLocation, NULL);

	CDialog::OnInitDialog();
	m_DateSelectorRange.InitialiseDateControls();
	m_DateSelectorRange.AddStandardReportRanges();
	m_DateSelectorRange.SetComboSelection(0, szRANGE_TODAY);

	{
		int nResult = m_comboDates.FindStringExact(0, EcrmanOptions.GetReportsDefaultDateRange());

		if (nResult != CB_ERR)
		{
			m_comboDates.SetCurSel(nResult);
		}
		else
		{
			m_comboDates.SetCurSel(0);
		}

		OnSelectDate();
	}

	for (int nIndex = 0; nIndex < DataManagerNonDb.ReportSession.GetSize(); nIndex++)
	{
		CSessionCSVRecord Session;
		DataManagerNonDb.ReportSession.GetAt(nIndex, Session);

		if (Session.GetCombinedSessionFlag() == FALSE)
		{
			int nPos = m_comboSession.AddString(DataManagerNonDb.ReportSession.GetName(nIndex));
			m_comboSession.SetItemData(nPos, nIndex + 2);
		}
	}

	{
		int nPos = m_comboSession.AddString("Adhoc Session");
		m_comboSession.SetItemData(nPos, 0);
	}

	if (m_comboSession.GetCount() < 2)
	{
		m_comboSession.InsertString(0, "00:00 to 23:59");
		m_comboSession.SetItemData(0, 1);
	}

	m_comboSession.EnableWindow(m_comboSession.GetCount() > 1);
	m_comboSession.SetCurSel(0);

	m_comboRound.AddString("Dates as selected");
	m_comboRound.AddString("Nearest full week");
	m_comboRound.AddString("Nearest full month");
	m_comboRound.SetCurSel(0);

	m_comboReportType.AddString("Detailed Report");
	m_comboReportType.AddString("Detailed Report + Est Pay");
	m_comboReportType.AddString("Estimated Pay Summary");
	m_comboReportType.AddString("Export Summary (shifts only)");
	m_comboReportType.AddString("Export Summary (with breaks)");
	m_comboReportType.AddString("Session Summary");
	m_comboReportType.SetItemData(0, 0);
	m_comboReportType.SetItemData(1, 3);
	m_comboReportType.SetItemData(2, 4);
	m_comboReportType.SetItemData(3, 1);
	m_comboReportType.SetItemData(4, 2);
	m_comboReportType.SetItemData(5, 5);
	m_comboReportType.SetCurSel(0);

	m_comboGroup.AddString("Location");
	m_comboGroup.AddString("Server");
	m_comboGroup.SetCurSel(0);
	m_comboGroup.SetItemData(0, 0);
	m_comboGroup.SetItemData(1, 1);

	if (EcrmanOptions.GetFeaturesServerSettingsType() == SERVER_LOC)
	{
		m_comboGroup.EnableWindow(FALSE);
	}

	m_comboEdit.AddString("Mark with an asterix");
	m_comboEdit.AddString("Show original event time");
	m_comboEdit.AddString("Show detailed information");
	m_comboEdit.SetItemData(0, 0);
	m_comboEdit.SetItemData(1, 1);
	m_comboEdit.SetItemData(2, 2);
	m_comboEdit.SetCurSel(0);

	m_comboTotalHours.AddString("From event times in report");
	m_comboTotalHours.AddString("From event times by second");
	m_comboTotalHours.SetItemData(0, 0);
	m_comboTotalHours.SetItemData(1, 1);
	m_comboTotalHours.SetCurSel(0);

	CLocationSelectorEntity LocSelEntity;
	m_LocationSelector.GetSelectedEntity(LocSelEntity);

	OnRadioAll();

	if (DealerFlags.GetServerConsolidationType() == SERVER_SORT_BYCODE)
	{
		m_radioList.EnableWindow(FALSE);
		m_radioList.ShowWindow(SW_HIDE);
	}

	PresetReportManager.TimeAttend.SetPasswordAccess();
	FillPresetCombo(-1);

	if (m_nPresetNo != 0)
	{
		m_nPresetError = PresetReportManager.SelectFromCombo(NODE_PRESET_TIMEATTEND, m_comboPreset, m_nPresetNo);

		if (0 == m_nPresetError)
		{
			OnSelectPreset();

			if (TRUE == m_bAutoRunPreset)
			{
				OnButtonDisplay();
			}
		}

		if (TRUE == m_bAutoRunPreset)
		{
			EndDialog(IDCANCEL);
			return FALSE;
		}
	}

	bool bPreset = PasswordArray.CheckLocationAccessSystem();
	ShowAndEnableWindow(&m_buttonPreset, bPreset);
	ShowAndEnableWindow(&m_staticPreset, FALSE == bPreset);

	return TRUE;
}

/**********************************************************************/
CEdit* CReportTimeAndAttendanceDlg::GetEditServer1(){ return GetEdit( IDC_EDIT_SERVER1 ); }
CEdit* CReportTimeAndAttendanceDlg::GetEditServer2(){ return GetEdit( IDC_EDIT_SERVER2 ); }
/**********************************************************************/

void CReportTimeAndAttendanceDlg::OnButtonPreset() 
{
	CPresetReportInfo Preset;
	CreatePreset( Preset );
	PresetReportManager.PresetButton( NODE_PRESET_TIMEATTEND, m_comboPreset, Preset, m_pParentWnd );
}

/**********************************************************************/

void CReportTimeAndAttendanceDlg::CreatePreset( CPresetReportInfo& Base ) 
{
	CPresetReportInfoTimeAttend Preset;

	//DATE RANGE GROUP
	CString strDateType;
	m_comboDates.GetWindowText( strDateType );
	Preset.SetDateType( strDateType );

	CString strDateFrom;
	COleDateTime dateFrom;
	m_DatePickerFrom.GetTime( dateFrom );
	::GetStringFromOleDate( dateFrom, strDateFrom );
	Preset.SetDateFrom( strDateFrom );
	
	CString strDateTo;
	COleDateTime dateTo;
	m_DatePickerTo.GetTime( dateTo );
	::GetStringFromOleDate( dateTo, strDateTo );
	Preset.SetDateTo( strDateTo );

	Preset.SetRoundToType( m_comboRound.GetCurSel() );

	CString strSession;
	m_comboSession.GetWindowText( strSession );
	Preset.SetSessionName( strSession );

	//SERVER GROUP
	Preset.SetServerPickType( m_nServerPickType );
	
	{
		CString str;
		GetEditServer1() -> GetWindowText( str );
		Preset.SetServerStart( atoi(str) );
		GetEditServer2() -> GetWindowText( str );
		Preset.SetServerEnd( atoi(str) );
	}

	//DATABASE GROUP
	CLocationSelectorEntity LocSelEntity;
	m_LocationSelector.GetSelectedEntity( LocSelEntity );
	Base.SetPresetConLevel( LocSelEntity.GetConType() );

	int nEntityNo1, nEntityNo2;
	CPresetReportArray::GetLocSelEntityNo( LocSelEntity, nEntityNo1, nEntityNo2 );
	Base.SetPresetEntityNo1( nEntityNo1 );
	Base.SetPresetEntityNo2( nEntityNo2 );

	//OPTIONS GROUP
	Preset.SetReportType( m_comboReportType.GetItemDataOfSelection() );
	Preset.SetGroupBy( m_comboGroup.GetItemDataOfSelection() );
	Preset.SetEditMode( m_comboEdit.GetItemDataOfSelection() );
	Preset.SetClosingTimeFlag(IsTicked(m_checkClosingTime));
	Preset.SetOpenShiftFlag(IsTicked(m_checkOpenShift));
	Preset.SetEventsBySecondFlag( m_comboTotalHours.GetCurSel() == 1 );

	//SAVE PRESET	
	CString str;
	Preset.ToString( str );
	Base.SetOtherFields( str );
}

/**********************************************************************/

void CReportTimeAndAttendanceDlg::OnSelectPreset() 
{
	int nSel = m_comboPreset.GetCurSel() - 1;
	if (nSel < 0 || nSel >= PresetReportManager.TimeAttend.GetSize())
	{
		return;
	}

	//LOAD PRESET
	CPresetReportInfo Base;
	PresetReportManager.TimeAttend.GetAt( nSel, Base );

	CPresetReportInfoTimeAttend Preset;
	{
		CString str = Base.GetOtherFields();
		Preset.FromString( str );
	}

	//DATE RANGE GROUP
	int nResult = m_comboDates.FindStringExact( 0, Preset.GetDateType() );
		
	if (nResult != CB_ERR)
	{
		m_comboDates.SetCurSel(nResult);
	}

	CString strDateFrom = Preset.GetDateFrom();
	CString strDateTo = Preset.GetDateTo();

	COleDateTime dateFrom, dateTo;
	
	if (::GetOleDateFromString(strDateFrom, dateFrom) == TRUE)
	{
		m_DatePickerFrom.SetTime(dateFrom);
	}

	if (::GetOleDateFromString(strDateTo, dateTo) == TRUE)
	{
		m_DatePickerTo.SetTime(dateTo);
	}

	OnSelectDate();

	int nType = Preset.GetRoundToType();
	if (nType >= 0 && nType < m_comboRound.GetCount())
	{
		m_comboRound.SetCurSel(nType);
	}

	nResult = m_comboSession.FindStringExact( 0, Preset.GetSessionName() );
		
	if (nResult != CB_ERR)
	{
		m_comboSession.SetCurSel(nResult);
	}

	//SERVER GROUP
	switch( Preset.GetServerPickType() )
	{
	case 0:
		OnRadioAll();
		break;

	case 1:
		{
			OnRadioRange();

			int n = Preset.GetServerStart();
			if ( ( n >= Server::ServerNo.Min ) && ( n <= Server::ServerNo.Max ) )
			{
				CString str;
				str.Format( "%d", n );
				GetEditServer1() -> SetWindowText(str);
			}

			n = Preset.GetServerEnd();
			if ( ( n >= Server::ServerNo.Min ) && ( n <= Server::ServerNo.Max ) )
			{
				CString str;
				str.Format( "%d", n );
				GetEditServer2() -> SetWindowText(str);
			}
		}
		break;

	case 2:
		OnRadioList();
		break;
	}

	//DATABASE GROUP
	int nConLevel = Base.GetPasswordConLevel();
	int nEntity1 = Base.GetPasswordEntityNo1();
	int nEntity2 = Base.GetPasswordEntityNo2();
	
	int nEntityIdx = 0;
	CPresetReportArray::GetEntityIdx( nConLevel, nEntity1, nEntity2, nEntityIdx );	
	m_LocationSelector.ForceSelection( nConLevel, nEntityIdx );

	//REPORT TYPE
	m_comboReportType.SelectUsingItemData( Preset.GetReportType(), m_comboReportType.GetCurSel() );
	m_comboGroup.SelectUsingItemData( Preset.GetGroupBy(), m_comboGroup.GetCurSel() );
	m_comboEdit.SelectUsingItemData( Preset.GetEditMode(), m_comboEdit.GetCurSel() );
	m_comboTotalHours.SelectUsingItemData( Preset.GetEventsBySecondFlag(), m_comboTotalHours.GetCurSel() );

	if (EcrmanOptions.GetFeaturesServerSettingsType() == SERVER_LOC)
	{
		m_comboGroup.SetCurSel(0);
	}

	m_checkClosingTime.SetCheck( Preset.GetClosingTimeFlag() );
	m_checkOpenShift.SetCheck(Preset.GetOpenShiftFlag());

	OnSelectReportType();
}

/**********************************************************************/

void CReportTimeAndAttendanceDlg::FillPresetCombo( int nPresetIdxSel )
{
	PresetReportManager.FillPresetCombo( NODE_PRESET_TIMEATTEND, m_comboPreset, nPresetIdxSel );
}

/**********************************************************************/

void CReportTimeAndAttendanceDlg::OnSelectReportType() 
{
	m_comboEdit.EnableWindow( m_comboReportType.GetCurSel() == 0 );
}

/**********************************************************************/

void CReportTimeAndAttendanceDlg::OnSelectDate() 
{
	m_DateSelectorRange.UpdateDateControls();
}

/**********************************************************************/

void CReportTimeAndAttendanceDlg::GetReportDates()
{
	COleDateTime dateFrom, dateTo;
	m_DatePickerFrom.GetTime(dateFrom);
	m_DatePickerTo.GetTime(dateTo);

	switch (m_comboRound.GetCurSel())
	{
	case 1:	//ROUND TO WEEK
	{
		int nWeekStartDay = EcrmanOptions.GetReportsWeekStartDay();

		int nWeekDayFrom = dateFrom.GetDayOfWeek() - 1;
		int nOffsetFrom = nWeekDayFrom - nWeekStartDay;
		if (nOffsetFrom < 0) nOffsetFrom += 7;
		dateFrom -= COleDateTimeSpan(nOffsetFrom, 0, 0, 0);

		int nWeekDayTo = dateTo.GetDayOfWeek() - 1;
		int nOffsetTo = (nWeekStartDay - nWeekDayTo) - 1;
		if (nOffsetTo < 0) nOffsetTo += 7;
		dateTo += COleDateTimeSpan(nOffsetTo, 0, 0, 0);
	}
	break;

	case 2:	//ROUND TO MONTH
	{
		int nYearFrom = dateFrom.GetYear();
		int nMonthFrom = dateFrom.GetMonth();
		dateFrom = COleDateTime(nYearFrom, nMonthFrom, 1, 0, 0, 0);

		int nYearTo = dateTo.GetYear();
		int nMonthTo = dateTo.GetMonth();

		nMonthTo++;
		if (nMonthTo > 12)
		{
			nYearTo++;
			nMonthTo = 1;
		}

		dateTo = COleDateTime(nYearTo, nMonthTo, 1, 0, 0, 0);
		dateTo -= COleDateTimeSpan(1, 0, 0, 0);
	}

	default:
		break;
	}

	m_OleDateFrom = dateFrom;
	m_OleDateTo = dateTo;

	bool bGotSession = FALSE;
	int nSel = m_comboSession.GetCurSel();
	if ((nSel >= 0) && (nSel < m_comboSession.GetCount()))
	{
		int nIdx = m_comboSession.GetItemData(nSel) - 2;
		if ((nIdx >= 0) && (nIdx < DataManagerNonDb.ReportSession.GetSize()))
		{
			DataManagerNonDb.ReportSession.GetAt(nIdx, m_ReportSession);
			bGotSession = TRUE;
		}
	}

	if (FALSE == bGotSession)
	{
		m_ReportSession.ClearRecord();
	}
}

/**********************************************************************/

bool CReportTimeAndAttendanceDlg::GetAdhocReportSession()
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

void CReportTimeAndAttendanceDlg::OnRadioAll()
{
	m_nServerPickType = 0;
	m_radioAll.SetCheck( TRUE );
	m_radioRange.SetCheck( FALSE );
	m_radioList.SetCheck( FALSE );
	GetEditServer1() -> EnableWindow( FALSE );
	GetEditServer2() -> EnableWindow( FALSE );
}

/**********************************************************************/

void CReportTimeAndAttendanceDlg::OnRadioRange()
{
	m_nServerPickType = 1;
	m_radioAll.SetCheck( FALSE );
	m_radioRange.SetCheck( TRUE );
	m_radioList.SetCheck( FALSE );
	GetEditServer1() -> EnableWindow( TRUE );
	GetEditServer2() -> EnableWindow( TRUE );
}

/**********************************************************************/

void CReportTimeAndAttendanceDlg::OnRadioList()
{
	m_nServerPickType = 2;
	m_radioAll.SetCheck( FALSE );
	m_radioRange.SetCheck( FALSE );
	m_radioList.SetCheck( TRUE );
	GetEditServer1() -> EnableWindow( FALSE );
	GetEditServer2() -> EnableWindow( FALSE );
}

/**********************************************************************/

void CReportTimeAndAttendanceDlg::OnButtonDisplay() 
{
	if (UpdateData(TRUE) == FALSE)
	{
		return;
	}

	CWaitCursor wait;
	CommsHandshaker.ProcessSales();
	
	GetReportDates();

	CString strSession;
	m_comboSession.GetWindowText( strSession );
	{
		int nSel = m_comboSession.GetCurSel();
		if ( ( nSel >= 0 ) && ( nSel < m_comboSession.GetCount() ) )
		{
			switch( m_comboSession.GetItemData(nSel) )
			{
			case 0:
				if ( GetAdhocReportSession() == FALSE )
				{
					return;
				}
				strSession = m_ReportSessionAdhoc.GetName();
				break;

			case 1:
				m_ReportSession.ClearRecord();
				break;
			}
		}
	}

	//WORK OUT FILES WE NEED TO READ FOR REPORT
	CLocationSelectorEntity LocSelEntity;
	m_LocationSelector.GetSelectedEntity( LocSelEntity );

	LocSelEntity.GetEPOSSelectArray( m_SelectArray );
	
	//GET SERVER LIST
	//MUST DO THIS BEFORE WE FINALISE ECR SELECT ARRAY
	if ( m_nServerPickType == 2 )
	{
		CServerSelectDlg dlg( m_ServerSelectionList, m_SelectArray, this );
		if (dlg.DoModal() != IDOK)
		{
			return;
		}
	}
	
	m_SelectArray.MakeList( TRUE, TRUE );
	
	CStringArray FileList;
	CWordArray DbIdxList, LocIdxList, SelIdxList;
	
	__int64 nTotalFileSize;
	
	CReportTimeAndAttendance::GetFileList( m_SelectArray, FileList, DbIdxList, LocIdxList, SelIdxList, nTotalFileSize );
	
	//ADD TITLE TO REPORT
	CString strDateFrom = m_OleDateFrom.Format ( "%d/%m/%Y" );
	CString strDateTo = m_OleDateTo.Format ( "%d/%m/%Y" );

	CString strConTitle = LocSelEntity.GetConsolidationTitle();

	CString strTitle;
	strTitle.Format ( " ( %s, %s - %s, %s )",
		(const char*) strConTitle,
		(const char*) strDateFrom,
		(const char*) strDateTo,
		(const char*) strSession );

	CReportTimeAndAttendance ServerReport( m_SelectArray, m_ServerSelectionList, ( m_comboGroup.GetCurSel() == 1 ) );
	ServerReport.SetServerPickType( m_nServerPickType );
	ServerReport.SetServerRange( m_nServerStart, m_nServerEnd );
	ServerReport.SetTAReportTitleInfo ( strTitle );
	ServerReport.SetDates( m_OleDateFrom, m_OleDateTo, m_ReportSession );
	ServerReport.SetShowEditDetailType( m_comboEdit.GetCurSel() );
	ServerReport.SetTAReportType( m_comboReportType.GetCurSel() );
	ServerReport.SetClosingTimeFlag(IsTicked(m_checkClosingTime));
	ServerReport.SetOpenShiftFlag(IsTicked(m_checkOpenShift));
	ServerReport.SetEventsBySecondFlag( m_comboTotalHours.GetCurSel() == 1 );

	__int64 nProgress = 0;

	StatusProgress.Lock( TRUE, "Consolidating server data" );
	
	for ( int n = 0; n < FileList.GetSize(); n++ )
	{	
		CSSFile fileServer;
		if (fileServer.Open(FileList.GetAt(n), "rb") == FALSE)
		{
			continue;
		}

		int nDbNo = 0;
		int nDbIdx = DbIdxList.GetAt(n) - 1;
		if (nDbIdx >= 0 && nDbIdx < dbDatabase.GetSize())
		{
			nDbNo = dbDatabase.GetDbNo(nDbIdx);
		}

		int nLocNo = 0;
		int nNwkNo = 0;
		int nLocIdx = LocIdxList.GetAt(n) - 1;
		if ( nLocIdx >= 0 && nLocIdx < dbLocation.GetSize() )
		{
			nLocNo = dbLocation.GetLocNo( nLocIdx );
			nNwkNo = dbLocation.GetNetworkNo( nLocIdx );
		}

		CString strBuffer;
		while ( fileServer.ReadString( strBuffer ) )
		{
			StatusProgress.SetPos( nProgress + fileServer.Tell(), nTotalFileSize );

			int nSelArrayIdx = SelIdxList.GetAt(n);
			CCSVEposTimeAttendData csv ( strBuffer );
			ServerReport.ConsolidateServerData( csv, nNwkNo, nDbNo, nLocNo, m_SelectArray, nSelArrayIdx );
		}
		nProgress += fileServer.GetLength();
	}

	StatusProgress.Unlock();
	
	ServerReport.SortByEventDateAndTime();
	ServerReport.CreateReport();

	//SHOWS ECRS AND THE DATES FOR WHICH WE HAVE DATA
	CSSFile fileReport;
	if ( fileReport.Open ( Super.ReportFilename(), "ab" ) == TRUE )
	{
		for ( int n = 0; n < m_SelectArray.GetListSize(); n++ )
		{
			CEposSelectArrayListItem ListItem;
			m_SelectArray.GetListItem( n, ListItem );

			{
				int nSPOSVersion;
				int nConnectionType;
				dbLocation.GetLocSPOSVersion( ListItem.m_nLocIdx, nSPOSVersion, nConnectionType );
				if ( CONNECTION_TYPE_STANDARD_NONE != nConnectionType )
					continue;
			}
				
			CString strName;
			bool bHideNoData = FALSE;
			strName.Format ( "<..>Time & Attendance for %s %s", 
					dbLocation.GetName( ListItem.m_nLocIdx ), 
					m_SelectArray.GetDateInfo( ListItem, bHideNoData ) );
				
			if ( FALSE == bHideNoData )
				fileReport.WriteLine ( strName );
		}
		fileReport.Close();
	}

	CString strKey = "";
	strKey.Format("%3.3d", m_comboReportType.GetCurSel() + 1);

	CReportManager ReportManager;
	CFilenameUpdater FnUp ( SysFiles::TimeAttendPrm );
	ReportManager.DisplayReport ( Super.ReportFilename(), FnUp.GetFilenameToUse(), strKey, this );
}

/**********************************************************************/

void CReportTimeAndAttendanceDlg::OnChangeDateFrom(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_DateSelectorRange.OnChangeFromDate();
	m_DateSelectorRange.CheckForChangedDate();
	*pResult = 0;
}

/**********************************************************************/

void CReportTimeAndAttendanceDlg::OnChangeDateTo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_DateSelectorRange.CheckForChangedDate();
	*pResult = 0;
}

/**********************************************************************/
