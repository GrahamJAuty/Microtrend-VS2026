/**********************************************************************/
#include <process.h>
/**********************************************************************/
#include "DateSelectorRange.h"
#include "FilterTypes.h"
#include "MaxLengths.h"
#include "NodeTypes.h"
#include "Password.h"
#include "PosTrayOptions.h"
#include "SysInfo.h"
/**********************************************************************/
#include "PosTrayBatchReportTimeAttendDlg.h"
/**********************************************************************/

CPosTrayBatchReportTimeAttendDlg::CPosTrayBatchReportTimeAttendDlg( CPosTrayTask& BatchEmail, CWnd* pParent /*=NULL*/)
	: CSSDialog(CPosTrayBatchReportTimeAttendDlg::IDD, pParent), 
		m_BatchEmail( BatchEmail )
{
	//{{AFX_DATA_INIT(CPosTrayBatchReportTimeAttendDlg)
	//}}AFX_DATA_INIT
	DataManagerNonDb.ServerNameTable.BuildList();
	m_nServerPickType = 0;
}

/**********************************************************************/

CPosTrayBatchReportTimeAttendDlg::~CPosTrayBatchReportTimeAttendDlg()
{
	DataManagerNonDb.ServerNameTable.ClearList();
}

/**********************************************************************/

void CPosTrayBatchReportTimeAttendDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPosTrayBatchReportTimeAttendDlg)
	
	DDX_Control(pDX, IDC_TAB_FILTER, m_TabFilter );

	DDX_Control(pDX, IDC_STATIC_NAME, m_staticName);
	DDX_Control(pDX, IDC_EDIT_NAME, m_editName);

	DDX_Control(pDX, IDC_STATIC_DATE, m_staticDate);
	DDX_Control(pDX, IDC_STATIC_DATE1, m_staticDate1);
	DDX_Control(pDX, IDC_STATIC_DATE2, m_staticDate2);
	DDX_Control(pDX, IDC_STATIC_DATE3, m_staticDate3);
	DDX_Control(pDX, IDC_COMBO_DATES, m_comboDates);
	DDX_Control(pDX, IDC_COMBO_ROUND, m_comboRound);
	DDX_Control(pDX, IDC_COMBO_SESSION, m_comboSession);
	
	DDX_Control(pDX, IDC_STATIC_DATABASE, m_staticDatabase);
	DDX_Control(pDX, IDC_STATIC_DATABASE1, m_staticDatabase1);
	DDX_Control(pDX, IDC_COMBO_LOCATION, m_comboLocation);
	
	DDX_Control(pDX, IDC_STATIC_SERVERS, m_staticServers);
	DDX_Control(pDX, IDC_RADIO_ALL, m_radioAll);
	DDX_Control(pDX, IDC_RADIO_RANGE, m_radioRange);
	DDX_Control(pDX, IDC_STATIC_SERVERS1, m_staticServers1);

	DDX_Control(pDX, IDC_STATIC_OPTIONS, m_staticOptions);
	DDX_Control(pDX, IDC_STATIC_OPTIONS1, m_staticOptions1);
	DDX_Control(pDX, IDC_STATIC_OPTIONS2, m_staticOptions2);
	DDX_Control(pDX, IDC_STATIC_OPTIONS3, m_staticOptions3);
	DDX_Control(pDX, IDC_COMBO_GROUP, m_comboGroup);
	DDX_Control(pDX, IDC_COMBO_EDIT, m_comboEdit);
	DDX_Control(pDX, IDC_COMBO_TOTALHOURS, m_comboTotalHours);
	DDX_Control(pDX, IDC_CHECK_OPENSHIFT, m_checkOpenShift);
	DDX_Control(pDX, IDC_CHECK_CLOSINGTIME, m_checkClosingTime);

	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPosTrayBatchReportTimeAttendDlg, CDialog)
	//{{AFX_MSG_MAP(CPosTrayBatchReportTimeAttendDlg)
	ON_WM_MEASUREITEM()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_FILTER, OnSelectTabFilter)
	ON_BN_CLICKED(IDC_RADIO_ALL, OnRadioAll)
	ON_BN_CLICKED(IDC_RADIO_RANGE, OnRadioRange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPosTrayBatchReportTimeAttendDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SubclassEdit(IDC_EDIT_SERVER1, SS_NUM, 4, "%d");
	SubclassEdit(IDC_EDIT_SERVER2, SS_NUM, 4, "%d");

	m_listReport.SubclassDlgItem(IDC_LIST, this);
	m_listReport.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_listReport.InsertColumn(0, "", LVCFMT_LEFT, 250);
	m_listReport.SetLockItemZeroFlag(FALSE);

	FillReportList();

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

	//DATE AND REPORT SESSION SELECTION
	if (PosTrayOptions.GetBusinessDateFlag() == FALSE)
	{
		m_comboDates.AddString(szRANGE_TODAY);
		m_comboDates.AddString(szRANGE_YESTERDAY);
	}
	else
	{
		m_comboDates.AddString(szRANGE_THIS_BUSDAY);
		m_comboDates.AddString(szRANGE_LAST_BUSDAY);
	}
	CDateSelectorRange::AddSingleDayRanges(m_comboDates);
	CDateSelectorRange::AddPreviousDayRanges(m_comboDates);
	m_comboDates.AddString(szRANGE_THIS_WEEK);
	m_comboDates.AddString(szRANGE_THIS_MONTH);
	CDateSelectorRange::AddThisTaxQuarterRange(m_comboDates);
	m_comboDates.AddString(szRANGE_THIS_YEAR);
	m_comboDates.AddString(szRANGE_THIS_FINANCIAL_YEAR);
	m_comboDates.AddString(szRANGE_LAST_WEEK);
	m_comboDates.AddString(szRANGE_LAST_MONTH);
	CDateSelectorRange::AddLastTaxQuarterRange(m_comboDates);
	m_comboDates.AddString(szRANGE_LAST_YEAR);
	m_comboDates.AddString(szRANGE_LAST_FINANCIAL_YEAR);
	m_comboDates.AddString(szRANGE_LAST_12_MONTHS);
	m_comboDates.SetCurSel(0);

	for (int nIndex = 0; nIndex < DataManagerNonDb.ReportSession.GetSize(); nIndex++)
	{
		m_comboSession.AddString(DataManagerNonDb.ReportSession.GetName(nIndex));
	}

	if (EcrmanOptions.GetReportsEODSessionFlag() == TRUE)
	{
		m_comboSession.AddString("EOD Business Day");
	}

	if (m_comboSession.GetCount() == 0)
	{
		m_comboSession.InsertString(0, "00:00 to 23:59");
	}

	m_comboSession.EnableWindow(m_comboSession.GetCount() > 1);
	m_comboSession.SetCurSel(0);

	m_comboRound.AddString("Dates as selected");
	m_comboRound.AddString("Nearest full week");
	m_comboRound.AddString("Nearest full month");
	m_comboRound.SetCurSel(0);

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

	LoadEmailBatchSettings();

	m_TabFilter.InsertItem(0, "General");
	m_TabFilter.InsertItem(1, "Options");
	m_nTabPageFilter = 0;
	SelectTabFilter();
	/****/
	MoveControl(&m_staticServers, 7, 28);
	MoveControl(&m_radioAll, 22, 46);
	MoveControl(&m_radioRange, 22, 65);
	MoveControl(GetEditServer1(), 88, 63);
	MoveControl(&m_staticServers1, 127, 65);
	MoveControl(GetEditServer2(), 138, 63);
	/****/
	MoveControl(&m_staticOptions, 7, 198);
	MoveControl(&m_staticOptions1, 22, 216);
	MoveControl(&m_comboGroup, 104, 216);
	MoveControl(&m_staticOptions2, 22, 234);
	MoveControl(&m_comboEdit, 104, 234);
	MoveControl(&m_staticOptions3, 22, 252);
	MoveControl(&m_comboTotalHours, 104, 252);
	MoveControl(&m_checkOpenShift, 22, 276);
	MoveControl(&m_checkClosingTime, 22, 292);
	/****/
	ResizeDialog(520, 367);

	return TRUE;
}

/**********************************************************************/
CEdit* CPosTrayBatchReportTimeAttendDlg::GetEditServer1() { return GetEdit(IDC_EDIT_SERVER1); }
CEdit* CPosTrayBatchReportTimeAttendDlg::GetEditServer2() { return GetEdit(IDC_EDIT_SERVER2); }
/**********************************************************************/

void CPosTrayBatchReportTimeAttendDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{  
	if ( nIDCtl == IDC_LIST )    
	{
		m_listReport.AdjustRowHeight( lpMeasureItemStruct, 10 ); 
	}
}

/**********************************************************************/

void CPosTrayBatchReportTimeAttendDlg::OnSelectTabFilter(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;

	m_nTabPageFilter = m_TabFilter.GetCurSel();
	
	if ( ( m_nTabPageFilter < 0 ) || ( m_nTabPageFilter > 1 ) )
		m_nTabPageFilter = 0;

	SelectTabFilter();
}

/**********************************************************************/

void CPosTrayBatchReportTimeAttendDlg::SelectTabFilter()
{
	ShowAndEnableWindow(&m_staticName, 0 == m_nTabPageFilter);
	ShowAndEnableWindow(&m_editName, 0 == m_nTabPageFilter);
	ShowAndEnableWindow(&m_staticDate, 0 == m_nTabPageFilter);
	ShowAndEnableWindow(&m_staticDate1, 0 == m_nTabPageFilter);
	ShowAndEnableWindow(&m_staticDate2, 0 == m_nTabPageFilter);
	ShowAndEnableWindow(&m_staticDate3, 0 == m_nTabPageFilter);
	ShowAndEnableWindow(&m_comboDates, 0 == m_nTabPageFilter);
	ShowAndEnableWindow(&m_comboRound, 0 == m_nTabPageFilter);
	ShowAndEnableWindow(&m_comboSession, 0 == m_nTabPageFilter);
	ShowAndEnableWindow(&m_staticDatabase, 0 == m_nTabPageFilter);
	ShowAndEnableWindow(&m_staticDatabase1, 0 == m_nTabPageFilter);
	ShowAndEnableWindow(&m_comboLocation, 0 == m_nTabPageFilter);
	/****/
	ShowAndEnableWindow(&m_staticServers, 1 == m_nTabPageFilter);
	ShowAndEnableWindow(&m_radioAll, 1 == m_nTabPageFilter);
	ShowAndEnableWindow(&m_radioRange, 1 == m_nTabPageFilter);
	ShowAndEnableWindow(&m_staticServers1, 1 == m_nTabPageFilter);
	ShowAndEnableWindow(GetEditServer1(), (1 == m_nTabPageFilter), (1 == m_nServerPickType));
	ShowAndEnableWindow(GetEditServer2(), (1 == m_nTabPageFilter), (1 == m_nServerPickType));
	/****/
	ShowAndEnableWindow(&m_staticOptions, 1 == m_nTabPageFilter);
	ShowAndEnableWindow(&m_staticOptions1, 1 == m_nTabPageFilter);
	ShowAndEnableWindow(&m_staticOptions2, 1 == m_nTabPageFilter);
	ShowAndEnableWindow(&m_staticOptions3, 1 == m_nTabPageFilter);
	ShowAndEnableWindow(&m_comboGroup, 1 == m_nTabPageFilter);
	ShowAndEnableWindow(&m_comboEdit, 1 == m_nTabPageFilter);
	ShowAndEnableWindow(&m_comboTotalHours, 1 == m_nTabPageFilter);
	ShowAndEnableWindow(&m_checkOpenShift, 1 == m_nTabPageFilter);
	ShowAndEnableWindow(&m_checkClosingTime, 1 == m_nTabPageFilter);
}

/**********************************************************************/

void CPosTrayBatchReportTimeAttendDlg::OnRadioAll()
{
	m_nServerPickType = 0;
	m_radioAll.SetCheck(TRUE);
	m_radioRange.SetCheck(FALSE);
	GetEditServer1()->EnableWindow(FALSE);
	GetEditServer2()->EnableWindow(FALSE);
}

/**********************************************************************/

void CPosTrayBatchReportTimeAttendDlg::OnRadioRange()
{
	m_nServerPickType = 1;
	m_radioAll.SetCheck(FALSE);
	m_radioRange.SetCheck(TRUE);
	GetEditServer1()->EnableWindow(TRUE);
	GetEditServer2()->EnableWindow(TRUE);
}

/**********************************************************************/

void CPosTrayBatchReportTimeAttendDlg::LoadEmailBatchSettings()
{
	//LOCATION SELECT
	{
		CLocationSelectorLookup LocSelLookup ( m_BatchEmail );
		CLocationSelectorEntity LocSelEntity( LocSelLookup );
		m_LocationSelector.ForceSelection( LocSelEntity );
	}

	//BATCH NAME
	m_editName.SetWindowText( m_BatchEmail.GetBatchName() );

	//DATE TYPE
	int nResult = m_comboDates.FindStringExact( 0, m_BatchEmail.GetBatchDateType() );
	if (nResult != CB_ERR)
	{
		m_comboDates.SetCurSel(nResult);
	}

	//DATE ROUNDING
	int nRoundTo = m_BatchEmail.GetBatchDateRoundTo();
	if ( ( nRoundTo < 0 ) || ( nRoundTo > 2 ) ) nRoundTo = 0;
	m_comboRound.SetCurSel( nRoundTo );

	//REPORT SESSION
	{
		int nSel = 0;
		CString strSession = m_BatchEmail.GetBatchSessionEntity();
		
		if (strSession == "")
		{
			strSession = m_BatchEmail.GetBatchSessionLegacy();
		}

		if ( strSession == "EOD" )
		{
			if (EcrmanOptions.GetReportsEODSessionFlag() == TRUE)
			{
				nSel = DataManagerNonDb.ReportSession.GetSize();
			}
		}
		else
		{
			for ( int n = 0; n < DataManagerNonDb.ReportSession.GetSize(); n++ )
			{
				CSessionCSVRecord Session;
				DataManagerNonDb.ReportSession.GetAt( n, Session );

				if ( strSession == Session.GetEntityID() )
				{
					nSel = n;
					break;
				}
			}
		}

		if (nSel >= m_comboSession.GetCount())
		{
			nSel = 0;
		}

		m_comboSession.SetCurSel( nSel );
	}
	
	//REPORT SELECTION
	//REPORT TYPES
	CCSV csvReports( m_BatchEmail.GetBatchReportList() );
	CReportConsolidationArray<CSortedStringItem> arrayTemp;
	for ( int n = 0; n < csvReports.GetSize(); n++ )
	{
		CSortedStringItem item;
		item.m_strItem = csvReports.GetString(n);
		arrayTemp.Consolidate( item );
	}

	for ( int n = 0; n < m_listReport.GetItemCount(); n++ )
	{
		CSortedStringItem item;
		item.m_strItem = m_arrayReportEntityIDs.GetAt(n);
		
		int nPos;
		m_listReport.SetItemTagState( n, arrayTemp.Find( item, nPos ) ? 1 : 0 );
	}

	m_comboGroup.SelectUsingItemData(m_BatchEmail.GetBatchTAGroup(), 0);
	m_comboEdit.SelectUsingItemData(m_BatchEmail.GetBatchTAEdit(), 0);
	m_comboTotalHours.SelectUsingItemData(m_BatchEmail.GetBatchTATotalHours(), 0);
	m_checkOpenShift.SetCheck(m_BatchEmail.GetBatchTAOpenShiftFlag());
	m_checkClosingTime.SetCheck(m_BatchEmail.GetBatchTAClosingTimeFlag());

	if (m_BatchEmail.GetBatchTAServerPickType() == 1)
	{
		OnRadioRange();

		int nFrom = m_BatchEmail.GetBatchTAServerPickFrom();
		if ((nFrom < Server::ServerNo.Min) || (nFrom > Server::ServerNo.Max))
		{
			nFrom = Server::ServerNo.Min;
		}
		
		int nTo = m_BatchEmail.GetBatchTAServerPickTo();
		if ((nTo < Server::ServerNo.Min) || (nTo > Server::ServerNo.Max))
		{
			nTo = Server::ServerNo.Max;
		}

		SetEditBoxInt(*GetEditServer1(), nFrom);
		SetEditBoxInt(*GetEditServer2(), nTo);
	}
	else
	{
		OnRadioAll();
		SetEditBoxInt(*GetEditServer1(), Server::ServerNo.Min);
		SetEditBoxInt(*GetEditServer2(), Server::ServerNo.Max);
	}
}

/**********************************************************************/

void CPosTrayBatchReportTimeAttendDlg::SaveEmailBatchSettings()
{
	//LOCATION SELECT
	{
		CLocationSelectorEntity LocSelEntity;
		m_LocationSelector.GetSelectedEntity( LocSelEntity );
		m_BatchEmail.SetBatchConType( LocSelEntity.GetConType() );
		m_BatchEmail.SetBatchConDbNo( LocSelEntity.GetDbNo() );
		m_BatchEmail.SetBatchConNwkNo( LocSelEntity.GetNwkNo() );
		m_BatchEmail.SetBatchConEntityNo( LocSelEntity.GetEntityNo() );
	}

	//BATCH NAME
	CString strName;
	m_editName.GetWindowText( strName );
	m_BatchEmail.SetBatchName( strName );

	//DATE TYPE
	CString strDateType;
	m_comboDates.GetWindowText( strDateType );
	m_BatchEmail.SetBatchDateType( strDateType );

	//DATE ROUND
	m_BatchEmail.SetBatchDateRoundTo( m_comboRound.GetCurSel() );

	//REPORT SESSION
	{
		int nSel = m_comboSession.GetCurSel();
		
		if ( ( nSel >= 0 ) && ( nSel < DataManagerNonDb.ReportSession.GetSize() ) )
		{
			CSessionCSVRecord Session;
			DataManagerNonDb.ReportSession.GetAt( nSel, Session );
			m_BatchEmail.SetBatchSessionEntity( Session.GetEntityID() );
		}
		else if ((nSel == DataManagerNonDb.ReportSession.GetSize()) && (EcrmanOptions.GetReportsEODSessionFlag() == TRUE))
		{
			m_BatchEmail.SetBatchSessionEntity("EOD");
		}
		else
		{
			m_BatchEmail.SetBatchSessionEntity("");
		}

		m_BatchEmail.SetBatchSessionLegacy( "" );
	}

	//REPORT SELECTION
	CCSV csvReports;
	for ( int n = 0; n < m_listReport.GetItemCount(); n++ )
	{
		if ( m_listReport.GetItemTagState(n) == 1 )
		{
			csvReports.Add( m_arrayReportEntityIDs.GetAt(n) );
		}
	}

	m_BatchEmail.SetBatchReportList( csvReports.GetLine() );

	m_BatchEmail.SetBatchTAGroup(m_comboGroup.GetItemDataOfSelection());
	m_BatchEmail.SetBatchTAEdit(m_comboEdit.GetItemDataOfSelection());
	m_BatchEmail.SetBatchTATotalHours(m_comboTotalHours.GetItemDataOfSelection());
	m_BatchEmail.SetBatchTAOpenShiftFlag(IsTicked(m_checkOpenShift));
	m_BatchEmail.SetBatchTAClosingTimeFlag(IsTicked(m_checkClosingTime));
	m_BatchEmail.SetBatchTAServerPickType(m_nServerPickType);

	int nFrom = GetEditBoxInt(*GetEditServer1());
	if ((nFrom < Server::ServerNo.Min) || (nFrom > Server::ServerNo.Max))
	{
		nFrom = Server::ServerNo.Min;
	}

	int nTo = GetEditBoxInt(*GetEditServer2());
	if ((nTo < Server::ServerNo.Min) || (nTo > Server::ServerNo.Max))
	{
		nTo = Server::ServerNo.Max;
	}

	m_BatchEmail.SetBatchTAServerPickFrom(nFrom);
	m_BatchEmail.SetBatchTAServerPickTo(nTo);
}

/**********************************************************************/

void CPosTrayBatchReportTimeAttendDlg::FillReportList()
{
	m_listReport.ClearList();
	m_arrayReportEntityIDs.RemoveAll();

	CStringArray arrayReports;
	arrayReports.Add("Detailed Report");
	arrayReports.Add("Detailed Report + Est Pay");
	arrayReports.Add("Estimated Pay Summary");
	arrayReports.Add("Export Summary (shifts only)");
	arrayReports.Add("Export Summary (with breaks)");
	arrayReports.Add("Session Summary");

	CArray<int, int> arrayReportTypes;
	arrayReportTypes.Add(0);
	arrayReportTypes.Add(3);
	arrayReportTypes.Add(4);
	arrayReportTypes.Add(1);
	arrayReportTypes.Add(2);
	arrayReportTypes.Add(5);

	for ( int nPos = 0; nPos < arrayReports.GetSize(); nPos++ )
	{
		CSSListTaggedSelectItem item;
		item.m_nData = 0;
		item.SetTagState(0);
		item.m_strText = arrayReports.GetAt(nPos);
		m_listReport.AddItemToList( item );

		CString strType = "";
		strType.Format("%d", arrayReportTypes.GetAt(nPos));
		m_arrayReportEntityIDs.Add( strType );
	}

	m_listReport.SetCurSel( 0 );
}

/**********************************************************************/

void CPosTrayBatchReportTimeAttendDlg::OnOK()
{
	SaveEmailBatchSettings();
	EndDialog( IDOK );
}

/**********************************************************************/
