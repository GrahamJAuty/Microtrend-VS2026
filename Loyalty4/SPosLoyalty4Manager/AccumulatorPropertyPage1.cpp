//$$******************************************************************
#include "..\SPosLoyalty4Shared\SQLTable_AuditPeriod\SQLRowSetAuditPeriod.h"
#include "..\SPosLoyalty4Shared\SQLTable_EcrmanDatabase\SQLRepositoryEcrmanDatabase.h"
//$$******************************************************************
#include "AccumulatorPropertySheet.h"
#include "LoyaltyManager.h"
#include "ReportDefaultHandler.h"
#include "StandardCombos.h"
//$$******************************************************************
#include "AccumulatorPropertyPage1.h"
//$$******************************************************************

CAccumulatorPropertyPage1::CAccumulatorPropertyPage1()
	: CSSPropertyPage(CAccumulatorPropertyPage1::IDD)
	, m_DateSelectorRange( m_comboDateRange, m_DatePickerFrom, m_DatePickerTo )
	, m_ReportLocationControls(m_comboDatabase, m_comboFolderSet, m_checkTerminals, m_comboTerminals, NULL)
{
	m_dThreshold = 0.0;
	m_nTop100 = 100;
	m_pReportInfo = NULL;
	SQLRowSetAuditPeriod.LoadPeriods(NULL);
}

//$$******************************************************************

CAccumulatorPropertyPage1::~CAccumulatorPropertyPage1()
{
}

//$$******************************************************************

void CAccumulatorPropertyPage1::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	//criteria
	DDX_Control(pDX, IDC_STATIC_TYPE, m_staticType);
	DDX_Control(pDX, IDC_STATIC_TOP100, m_staticTop100);
	DDX_Control(pDX, IDC_EDIT_TOP100, m_editTop100);
	DDX_Control(pDX, IDC_CHECK_NOZEROSKIP, m_checkNoZeroSkip);
	DDX_Control(pDX, IDC_STATIC_REPORTON, m_staticReportOn);
	DDX_Control(pDX, IDC_COMBO_RANKBY, m_comboRankBy);
	DDX_Control(pDX, IDC_CHECK_UNSOLD, m_checkUnsold);
	//date
	DDX_Control(pDX, IDC_STATIC_DATE, m_staticDate);
	DDX_Control(pDX, IDC_COMBO_DATERANGE, m_comboDateRange);
	DDX_Control(pDX, IDC_STATIC_DATEFROM, m_staticDateFrom);
	DDX_Control(pDX, IDC_STATIC_DATETO, m_staticDateTo);
	DDX_Control(pDX, IDC_DATEPICKER_FROM, m_DatePickerFrom);
	DDX_Control(pDX, IDC_DATEPICKER_TO, m_DatePickerTo);
	//time
	DDX_Control(pDX, IDC_COMBO_TIMERANGE, m_comboTimeRange);
	DDX_Control(pDX, IDC_STATIC_TIMESTART, m_staticTimeStart);
	DDX_Control(pDX, IDC_STATIC_TIMEEND, m_staticTimeEnd);
	DDX_Control(pDX, IDC_TIMEPICKER_FROM, m_TimePickerFrom);
	DDX_Control(pDX, IDC_TIMEPICKER_TO, m_TimePickerTo);
	//terminals
	DDX_Control(pDX, IDC_STATIC_EPOS, m_staticEPOS);
	DDX_Control(pDX, IDC_STATIC_DATABASE, m_staticDatabase);
	DDX_Control(pDX, IDC_COMBO_DATABASE, m_comboDatabase);
	DDX_Control(pDX, IDC_STATIC_FOLDERSET, m_staticFolderSet);
	DDX_Control(pDX, IDC_COMBO_FOLDERSET, m_comboFolderSet);
	DDX_Control(pDX, IDC_CHECK_TERMINALS, m_checkTerminals);
	DDX_Control(pDX, IDC_COMBO_TERMINALS, m_comboTerminals);
	//altkey
	DDX_Control(pDX, IDC_BUTTON_ALTKEY1, m_buttonAltKey1);

	//criteria
	DDX_Text(pDX, IDC_EDIT_THRESHOLD, m_dThreshold);
	DDX_Text(pDX, IDC_EDIT_TOP100, m_nTop100);
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CAccumulatorPropertyPage1, CPropertyPage)
	//date
	ON_CBN_SELCHANGE(IDC_COMBO_DATERANGE, OnSelectDateRange)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATEPICKER_FROM, OnChangeDateFrom)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATEPICKER_TO, OnChangeDateTo)
	//time
	ON_CBN_SELCHANGE(IDC_COMBO_TIMERANGE, OnSelectTimeRange)
	//terminals
	ON_CBN_SELCHANGE(IDC_COMBO_DATABASE, OnSelectDatabase)
	ON_CBN_SELCHANGE(IDC_COMBO_FOLDERSET, OnSelectFolderSet)
	ON_BN_CLICKED(IDC_CHECK_TERMINALS, OnToggleTerminals)
	//altkey
	ON_BN_CLICKED(IDC_BUTTON_ALTKEY1, OnButtonAltKey1)
END_MESSAGE_MAP()

//$$******************************************************************

BOOL CAccumulatorPropertyPage1::OnInitDialog()
{
	m_ReportLocationControls.SetTerminalListHandler(m_pReportInfo->GetTerminalListHandler());

	int nPreviousTimeCurSel = 0;
	bool bPreviousNoZeroSkip = FALSE;
	int nPreviousRankByType = 1;
	int nPreviousTerminalListType = 0;
	CString strPreviousDateType = "";
	CString strPreviousDateFrom = "";
	CString strPreviousDateTo = "";
	CString strPreviousTimeFrom = "";
	CString strPreviousTimeTo = "";
	int nPreviousDbNo = 1;
	int nPreviousFolderSet = 0;
	//bool bShowDatabaseCombo = FALSE;
	bool bPreviousExcludeUnsold = TRUE;

	CString strLine;
	CReportDefaultHandler repdefs;
	if (repdefs.Read(m_pReportInfo->GetReportLabel(), strLine) == TRUE)
	{
		CCSV csv(strLine);
		//0 was old date current selection
		if (csv.IsEmpty(1) == FALSE)	nPreviousTimeCurSel = csv.GetInt(1);
		if (csv.IsEmpty(2) == FALSE)	m_dThreshold = csv.GetDouble(2);
		if (csv.IsEmpty(3) == FALSE)	bPreviousNoZeroSkip = csv.GetBool(3);
		if (csv.IsEmpty(4) == FALSE)	nPreviousRankByType = csv.GetInt(4);
		if (csv.IsEmpty(5) == FALSE)	m_nTop100 = csv.GetInt(5);
		if (csv.IsEmpty(6) == FALSE)	nPreviousTerminalListType = csv.GetInt(6);
		if (csv.IsEmpty(7) == FALSE)	strPreviousDateType = csv.GetString(7);
		if (csv.IsEmpty(8) == FALSE)	strPreviousDateFrom = csv.GetString(8);
		if (csv.IsEmpty(9) == FALSE)	strPreviousDateTo = csv.GetString(9);
		if (csv.IsEmpty(10) == FALSE)	strPreviousTimeFrom = csv.GetString(10);
		if (csv.IsEmpty(11) == FALSE)	strPreviousTimeTo = csv.GetString(11);
		if (csv.IsEmpty(12) == FALSE)	nPreviousDbNo = csv.GetInt(12);
		if (csv.IsEmpty(13) == FALSE)	nPreviousFolderSet = csv.GetInt(13);
		if (csv.IsEmpty(14) == FALSE)	bPreviousExcludeUnsold = csv.GetInt(14);
	}

	CString strThresholdTitle = "";
	bool bAllowAnyDb = TRUE;

	switch ( m_pReportInfo -> GetReportType() )
	{
	case nACCUMULATE_LOWPOINTS:		
		strThresholdTitle = "Points less than";		// cards < threshold							
		break;

	case nACCUMULATE_LOWSPEND:		
		strThresholdTitle = "Spend less than";		// cards < threshold
		break;

	case nACCUMULATE_RANKPOINTS:	
		strThresholdTitle = "Rank points above";	// cards > threshold
		break;

	case nACCUMULATE_RANKSPEND:		
		strThresholdTitle = "Rank spend above";		// cards > threshold
		break;

	case nACCUMULATE_RANKPURCHASES:	
		strThresholdTitle = "Rank by";				// Rank By ( Quantity / Value )
		bPreviousNoZeroSkip = FALSE;				// threshold not used - checks specfically for > 0
		bAllowAnyDb = FALSE;
		break;

	case nACCUMULATE_ACTIVE:		
		bPreviousNoZeroSkip = FALSE;				// threshold not used - checks specfically for > 0	
		break;

	case nACCUMULATE_INACTIVE:		
		bPreviousNoZeroSkip = TRUE;					// cards < 1 & zero
		break;
	}

	CSSPropertyPage::OnInitDialog();

	m_staticReportOn.SetWindowText(strThresholdTitle);
	m_checkNoZeroSkip.SetCheck(bPreviousNoZeroSkip);

	switch (m_pReportInfo->GetReportType())
	{
	case nACCUMULATE_ACTIVE:
	case nACCUMULATE_INACTIVE:
		ShowAndEnableWindow(&m_staticType, FALSE);
		ShowAndEnableWindow(&m_staticTop100, FALSE);
		ShowAndEnableWindow(&m_editTop100, FALSE);
		ShowAndEnableWindow(GetEditThreshold(), FALSE);
		ShowAndEnableWindow(&m_comboRankBy, FALSE);
		ShowAndEnableWindow(&m_checkNoZeroSkip, FALSE);
		ShowAndEnableWindow(&m_staticReportOn, FALSE);
		MoveControl(&m_staticDate, 7, 7, 208, 261);
		MoveControl(&m_comboDateRange, 24, 31);
		MoveControl(&m_staticDateFrom, 24, 53);
		MoveControl(&m_DatePickerFrom, 51, 51);
		MoveControl(&m_staticDateTo, 24, 73);
		MoveControl(&m_DatePickerTo, 51, 71);
		MoveControl(&m_comboTimeRange, 24, 109);
		MoveControl(&m_staticTimeStart, 24, 131);
		MoveControl(&m_TimePickerFrom, 71, 129);
		MoveControl(&m_staticTimeEnd, 24, 151);
		MoveControl(&m_TimePickerTo, 71, 149);
		MoveControl(&m_staticEPOS, 223, 7, 224, 261);
		MoveControl(&m_staticDatabase, 240, 33);
		MoveControl(&m_comboDatabase, 298, 31);
		MoveControl(&m_staticFolderSet, 240, 53);
		MoveControl(&m_comboFolderSet, 298, 51);
		MoveControl(&m_checkTerminals, 240, 73);
		MoveControl(&m_comboTerminals, 298, 71);
		break;

	case nACCUMULATE_LOWPOINTS:
		SubclassEdit(IDC_EDIT_THRESHOLD, SS_NUM_S, 8, "%.0f");
		ShowAndEnableWindow(&m_comboRankBy, FALSE);
		ShowAndEnableWindow(&m_staticTop100, FALSE);
		ShowAndEnableWindow(&m_editTop100, FALSE);
		break;

	case nACCUMULATE_LOWSPEND:
		SubclassEdit(IDC_EDIT_THRESHOLD, SS_NUM_SDP, 11, "%.*f", System.GetDPValue());
		ShowAndEnableWindow(&m_comboRankBy, FALSE);
		ShowAndEnableWindow(&m_staticTop100, FALSE);
		ShowAndEnableWindow(&m_editTop100, FALSE);
		break;

	case nACCUMULATE_RANKPOINTS:
		SubclassEdit(IDC_EDIT_THRESHOLD, SS_NUM_S, 8, "%.0f");
		ShowAndEnableWindow(&m_checkNoZeroSkip, FALSE);
		ShowAndEnableWindow(&m_comboRankBy, FALSE);
		break;

	case nACCUMULATE_RANKSPEND:
		SubclassEdit(IDC_EDIT_THRESHOLD, SS_NUM_SDP, 11, "%.*f", System.GetDPValue());
		ShowAndEnableWindow(&m_checkNoZeroSkip, FALSE);
		ShowAndEnableWindow(&m_comboRankBy, FALSE);
		break;

	case nACCUMULATE_RANKPURCHASES:
		ShowAndEnableWindow(GetEditThreshold(), FALSE);
		ShowAndEnableWindow(&m_checkNoZeroSkip, FALSE);
		break;
	}

	{
		bool bPurchases = (nACCUMULATE_RANKPURCHASES == m_pReportInfo->GetReportType());
		ShowAndEnableWindow(&m_checkUnsold, bPurchases);

		if (TRUE == bPurchases)
		{
			m_checkUnsold.SetCheck(bPreviousExcludeUnsold);
		}
	}

// Date range

	m_DateSelectorRange.InitialiseDateControls();
	m_comboDateRange.AddString ( szRANGE_TODAY );
	m_comboDateRange.AddString ( szRANGE_YESTERDAY );
	m_comboDateRange.AddString ( szRANGE_THIS_WEEK );
	m_comboDateRange.AddString ( szRANGE_THIS_MONTH );
	m_comboDateRange.AddString ( szRANGE_THIS_YEAR );
	m_comboDateRange.AddString ( szRANGE_LAST_WEEK );
	m_comboDateRange.AddString ( szRANGE_LAST_MONTH );
	m_comboDateRange.AddString ( szRANGE_LAST_YEAR );
	m_comboDateRange.AddString ( szRANGE_LAST_3_MONTHS );
	m_comboDateRange.AddString ( szRANGE_LAST_6_MONTHS );
	m_comboDateRange.AddString ( szRANGE_LAST_9_MONTHS );
	m_comboDateRange.AddString ( szRANGE_LAST_12_MONTHS );
	m_comboDateRange.AddString ( szRANGE_ALL );
	m_comboDateRange.AddString ( szRANGE_CUSTOM_DATE );
	m_comboDateRange.AddString ( szRANGE_CUSTOM_RANGE );
	m_DateSelectorRange.SetComboSelection(strPreviousDateType);

	if ( ( strPreviousDateType == szRANGE_CUSTOM_DATE ) || ( strPreviousDateType == szRANGE_CUSTOM_RANGE ) )
	{
		CSSDate dateFrom( strPreviousDateFrom );
		CSSDate dateTo( strPreviousDateTo );
		
		if ( dateFrom.IsSet() )
		{
			m_DatePickerFrom.SetTime( COleDateTime( dateFrom.GetYear(), dateFrom.GetMonth(), dateFrom.GetDay(), 0, 0, 0 ) );
		}

		if ( dateTo.IsSet() )
		{
			m_DatePickerTo.SetTime( COleDateTime( dateTo.GetYear(), dateTo.GetMonth(), dateTo.GetDay(), 0, 0, 0 ) );
		}
	}

// add audit periods

	m_comboTimeRange.AddString ( szTIMERANGE_ALL );
	for ( int nPeriodIndex = 0 ; nPeriodIndex < nMAXPERIODS ; nPeriodIndex++ )
	{
		if (SQLRowSetAuditPeriod.IsPeriodSet(nPeriodIndex) == TRUE)
		{
			m_comboTimeRange.AddString(SQLRowSetAuditPeriod.GetPeriodName(nPeriodIndex));
		}
	}
	m_comboTimeRange.SetCurSel(nPreviousTimeCurSel);

	if (m_comboTimeRange.GetCount() < 2)
	{
		m_comboTimeRange.EnableWindow(FALSE);
	}

	m_TimePickerFrom.SetFormat ( "HH:mm" );
	m_TimePickerTo.SetFormat ( "HH:mm" );

	{
		CSSTime timeFrom( strPreviousTimeFrom );
		CSSTime timeTo( strPreviousTimeTo );

		if ( timeFrom.IsSet() )
		{
			m_TimePickerFrom.SetTime( COleDateTime( 2019, 1, 1, timeFrom.GetHour(), timeFrom.GetMinute(), 0 ) );
		}
		else
		{
			m_TimePickerFrom.SetTime( COleDateTime( 2019, 1, 1, 0, 0, 0 ) );
		}

		if ( timeTo.IsSet() )
		{
			m_TimePickerTo.SetTime( COleDateTime( 2019, 1, 1, timeTo.GetHour(), timeTo.GetMinute(), 0 ) );
		}
		else
		{
			m_TimePickerTo.SetTime( COleDateTime( 2019, 1, 1, 23, 59, 0 ) );
		}
	}

// Rank by

	m_comboRankBy.AddString ( "Quantity" );
	m_comboRankBy.AddString ( "Value" );

	Server.Read ( Filenames.GetServerDataFilename() );
	if (Server.GetEnablePluBonusFlag() == TRUE)
	{
		m_comboRankBy.AddString("Points");
	}
	
	if ((nPreviousRankByType < 0) || (nPreviousRankByType >= m_comboRankBy.GetCount()))
	{
		nPreviousRankByType = 0;
	}

	m_comboRankBy.SetCurSel(nPreviousRankByType);

// terminal list
	m_ReportLocationControls.Init(nPreviousDbNo, nPreviousFolderSet, nPreviousTerminalListType, bAllowAnyDb);

// other

	m_buttonAltKey1.ShowWindow( SW_HIDE );

	return TRUE;
}

//$$******************************************************************
CEdit* CAccumulatorPropertyPage1::GetEditThreshold(){ return GetEdit( IDC_EDIT_THRESHOLD ); }
//$$******************************************************************

void CAccumulatorPropertyPage1::OnSelectDateRange() 
{
	m_DateSelectorRange.UpdateDateControls();
}

//$$******************************************************************

void CAccumulatorPropertyPage1::OnChangeDateFrom(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_DateSelectorRange.OnChangeFromDate();
	m_DateSelectorRange.CheckForChangedDate();
	*pResult = 0;
}

/**********************************************************************/

void CAccumulatorPropertyPage1::OnChangeDateTo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_DateSelectorRange.CheckForChangedDate();
	*pResult = 0;
}

/**********************************************************************/

void CAccumulatorPropertyPage1::OnSelectTimeRange() 
{
	int nStartHour = 0;
	int nStartMins = 0;
	int nEndHour = 23;
	int nEndMins = 59;

	CString strTimeRange;
	m_comboTimeRange.GetWindowText( strTimeRange );
	SQLRowSetAuditPeriod.GetTimeRange ( strTimeRange, nStartHour, nStartMins, nEndHour, nEndMins );

	COleDateTime startTime( 2019, 1, 1, nStartHour, nStartMins, 0 );
	m_TimePickerFrom.SetTime ( startTime );

	COleDateTime endTime( 2019, 1, 1, nEndHour, nEndMins, 0 );
	m_TimePickerTo.SetTime ( endTime );
}

//$$******************************************************************

void CAccumulatorPropertyPage1::OnSelectDatabase()
{
	m_ReportLocationControls.SelectDatabase();
}

//*******************************************************************

void CAccumulatorPropertyPage1::OnSelectFolderSet()
{
	m_ReportLocationControls.SelectFolderSet();
}

//*******************************************************************

void CAccumulatorPropertyPage1::OnToggleTerminals()
{
	m_ReportLocationControls.ToggleTerminals();
}

//$$******************************************************************

void CAccumulatorPropertyPage1::OnButtonAltKey1()
{
	m_ReportLocationControls.DeleteSelectedTerminalList();
}

//$$******************************************************************

BOOL CAccumulatorPropertyPage1::OnKillActive()
{
	if (UpdateData() == TRUE)
	{
		CString strTerminalList = "";
		int nDbNo = 0;
		int nFolderSet = 0;
		bool bWantTerminals = FALSE;
		m_ReportLocationControls.GetSettings(nDbNo, nFolderSet, bWantTerminals, strTerminalList);

		if ((TRUE == bWantTerminals) && (strTerminalList == ""))
		{
			Prompter.FatalError("No EPOS terminal numbers have been specified!", "Entry Error");
			m_comboTerminals.SetFocus();
			return FALSE;
		}

		{
			CTime dateFrom;
			m_DatePickerFrom.GetTime(dateFrom);
			m_pReportInfo->SetDateFrom(dateFrom);
		}

		{
			CTime dateTo;
			m_DatePickerTo.GetTime(dateTo);
			m_pReportInfo->SetDateTo(dateTo);
		}

		{
			CTime timeFrom;
			m_TimePickerFrom.GetTime(timeFrom);
			CString strStartTime = timeFrom.Format("%H:%M");
			m_pReportInfo->SetTimeFrom(timeFrom);

			CTime timeTo;
			m_TimePickerTo.GetTime(timeTo);
			CString strEndTime = timeTo.Format("%H:%M");
			m_pReportInfo->SetTimeTo(timeTo);

			m_pReportInfo->SetTimeRangeFlag(strStartTime != "00:00" || strEndTime != "23:59");
		}

		m_pReportInfo->SetNoZeroSkipFlag(m_checkNoZeroSkip.GetCheck() != 0);
		m_pReportInfo->SetRankByType(m_comboRankBy.GetCurSel());
		m_pReportInfo->SetTop100ChartSize(m_nTop100);
		m_pReportInfo->SetThreshold(m_dThreshold);
		m_pReportInfo->SetDbNo(nDbNo);
		m_pReportInfo->SetFolderSet(nFolderSet);
		m_pReportInfo->SetExcludeUnsoldFlag(IsTicked(m_checkUnsold));
		m_pReportInfo->SetTerminalListType(bWantTerminals ? 1 : 0);

		{
			CString strDateRangeType;
			m_comboDateRange.GetWindowText(strDateRangeType);

			CCSV csv;
			csv.Add(0); //PREVIOUS DATE COMBO SEL
			csv.Add(m_comboTimeRange.GetCurSel());
			csv.Add(m_pReportInfo->GetThreshold(), 2);
			csv.Add(m_pReportInfo->GetNoZeroSkipFlag());
			csv.Add(m_pReportInfo->GetRankByType());
			csv.Add(m_pReportInfo->GetTop100ChartSize());
			csv.Add(m_pReportInfo->GetTerminalListType());
			csv.Add(strDateRangeType);
			csv.Add(m_pReportInfo->GetDateFromString());
			csv.Add(m_pReportInfo->GetDateToString());
			csv.Add(m_pReportInfo->GetTimeFromString());
			csv.Add(m_pReportInfo->GetTimeToString());
			csv.Add(m_pReportInfo->GetDbNo());
			csv.Add(m_pReportInfo->GetFolderSet());
			csv.Add(m_pReportInfo->GetExcludeUnsoldFlag());
			
			CReportDefaultHandler repdefs;
			repdefs.Save(m_pReportInfo->GetReportLabel(), &csv);
		}

		if (m_pReportInfo->GetTerminalListType() == 1)
		{
			m_pReportInfo->TNoListSetMaxLength(strTerminalList.GetLength());	// stop entry being truncated
			m_pReportInfo->TNoListSave(strTerminalList);						// save terminal list

			m_pReportInfo->TNoArrayRemoveAll();
			m_pReportInfo->TNoListExtractTNos(strTerminalList);			// extract terminal numbers
		}
	}

	return CPropertyPage::OnKillActive();
}

//$$******************************************************************

