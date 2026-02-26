/**********************************************************************/
#include "..\CommonEcrStock\SessionEditDlg.h"
/**********************************************************************/
#include "CommsHandshaker.h"
#include "NodeTypes.h"
#include "Password.h"
#include "RepSpawn.h"
#include "SysInfo.h"
/**********************************************************************/
#include "SalesHistoryReportBritanniaDlg.h"
/**********************************************************************/

CSalesHistoryReportBritanniaDlg::CSalesHistoryReportBritanniaDlg(CWnd* pParent /*=NULL*/)
	: CSSDialog(CSalesHistoryReportBritanniaDlg::IDD, pParent), 
		m_DateSelectorRange( m_comboDates, m_DatePickerFrom, m_DatePickerTo ),
		m_DepartmentSelector(m_editDeptNo, m_comboDeptName)
{
	//{{AFX_DATA_INIT(CSalesHistoryReportBritanniaDlg)
	//}}AFX_DATA_INIT
	m_nLocationComboDbIdx = -1;
}

/**********************************************************************/

CSalesHistoryReportBritanniaDlg::~CSalesHistoryReportBritanniaDlg()
{
}

/**********************************************************************/

void CSalesHistoryReportBritanniaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSalesHistoryReportBritanniaDlg)
	DDX_Control(pDX, IDC_COMBO_ROUND, m_comboRound);
	DDX_Control(pDX, IDC_COMBO_SESSION, m_comboSession);
	DDX_Control(pDX, IDC_COMBO_DATES, m_comboDates);
	DDX_Control(pDX, IDC_DATEPICKER_TO, m_DatePickerTo);
	DDX_Control(pDX, IDC_DATEPICKER_FROM, m_DatePickerFrom);
	DDX_Control(pDX, IDC_COMBO_LOCATION, m_comboLocation);
	DDX_Control(pDX, IDC_COMBO_TERMINAL, m_comboTerminal);
	DDX_Control(pDX, IDC_CHECK_GUEST, m_checkGuest);
	DDX_Control(pDX, IDC_EDIT_DEPTNO, m_editDeptNo);
	DDX_Control(pDX, IDC_COMBO_DEPTNAME, m_comboDeptName);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CSalesHistoryReportBritanniaDlg, CDialog)
	//{{AFX_MSG_MAP(CSalesHistoryReportBritanniaDlg)
	ON_CBN_SELCHANGE(IDC_COMBO_DATES, OnSelectDate)
	ON_CBN_SELCHANGE(IDC_COMBO_LOCATION, OnSelectLocation)
	ON_BN_CLICKED(IDC_BUTTON_DISPLAY, OnButtonDisplay)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATEPICKER_FROM, OnChangeDateFrom)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATEPICKER_TO, OnChangeDateTo)
	ON_EN_KILLFOCUS(IDC_EDIT_DEPTNO, OnKillFocusDeptNo)
	ON_CBN_SELCHANGE(IDC_COMBO_DEPTNAME, OnSelectDeptName)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CSalesHistoryReportBritanniaDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString strAllItems = "";
	if (DealerFlags.GetBritanniaAllItemsFlag() == TRUE)
	{
		strAllItems = ", include unsold items";
	}

	CString str = "";
	str.Format("CSV Sales Export for Britannia Hotels (%d prices%s)", DealerFlags.GetMaxBritanniaPriceLevel(), (const char*)strAllItems);
	SetWindowText(str);

	CWordArray arrayTypes;
	arrayTypes.Add(NODE_LOCATION);
	arrayTypes.Add(NODE_DATABASE);

	switch (PasswordArray.GetLocationAccessType())
	{
	case LOCATION_ACCESS_DB:
		m_LocationSelector.SetSingleDbIdx(PasswordArray.GetLocationAccessDbIdx());
		break;

	case LOCATION_ACCESS_LOC:
		m_LocationSelector.SetSingleLocIdx(PasswordArray.GetLocationAccessLocIdx());
		break;
	}

	m_LocationSelector.BuildList(arrayTypes);
	m_LocationSelector.FillLocationCombo(&m_comboLocation, &m_comboTerminal);

	CLocationSelectorEntity LocSelEntity;
	m_LocationSelector.GetSelectedEntity(LocSelEntity);

	CDataManagerInfo info;
	DataManager.OpenDatabaseReadOnly(LocSelEntity.GetDbIdx(), info, FALSE);

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
		m_comboSession.AddString(DataManagerNonDb.ReportSession.GetName(nIndex));
	}

	m_comboSession.AddString("Adhoc Session");

	if (m_comboSession.GetCount() < 2)
	{
		m_comboSession.InsertString(0, "00:00 to 23:59");
	}

	m_comboSession.EnableWindow(m_comboSession.GetCount() > 1);
	m_comboSession.SetCurSel(0);

	m_comboRound.AddString("Dates as selected");
	m_comboRound.AddString("Nearest full week");
	m_comboRound.AddString("Nearest full month");
	m_comboRound.SetCurSel(0);

	if (SysInfo.IsPMSSystem() == FALSE)
	{
		m_checkGuest.SetCheck(FALSE);
		m_checkGuest.EnableWindow(FALSE);
		m_checkGuest.ShowWindow(SW_HIDE);
	}

	m_editDeptNo.LimitText(MAX_LENGTH_DEPTNO);
	m_DepartmentSelector.FillDepartmentComboDatabase(DEPARTMENTSET_ALL, DEPARTMENT_SELECTOR_REPORT, FALSE, TRUE);

	return TRUE;
}

/**********************************************************************/

void CSalesHistoryReportBritanniaDlg::OnSelectLocation()
{
	if (UpdateData(TRUE) == TRUE)
	{
		int nOldDbIdx = DataManager.GetActiveDbIdx();

		CLocationSelectorEntity LocSelEntity;
		m_LocationSelector.GetSelectedEntity(LocSelEntity);
		m_LocationSelector.FillEposCombo();

		int nNewDbIdx = LocSelEntity.GetDbIdx();

		if (nOldDbIdx != nNewDbIdx)
		{
			CDataManagerInfo info;
			DataManager.OpenDatabaseReadOnly(nNewDbIdx, info, FALSE);

			int nFilter = m_DepartmentSelector.GetDepartmentFilter();
			m_DepartmentSelector.FillDepartmentComboDatabase(DEPARTMENTSET_ALL, DEPARTMENT_SELECTOR_NORMAL, FALSE, TRUE);
			m_DepartmentSelector.UpdateDepartmentCombo(nFilter);
			m_DepartmentSelector.UpdateDepartmentEditBox();
		}
	}
}

/**********************************************************************/

void CSalesHistoryReportBritanniaDlg::OnSelectDate() 
{
	m_DateSelectorRange.UpdateDateControls();
}

/**********************************************************************/

void CSalesHistoryReportBritanniaDlg::GetReportDates()
{
	m_DatePickerFrom.GetTime(m_oleDateFrom);
	m_DatePickerTo.GetTime(m_oleDateTo);

	switch (m_comboRound.GetCurSel())
	{
	case 1:	//ROUND TO WEEK
	{
		int nWeekStartDay = EcrmanOptions.GetReportsWeekStartDay();

		int nWeekDayFrom = m_oleDateFrom.GetDayOfWeek() - 1;
		int nOffsetFrom = nWeekDayFrom - nWeekStartDay;
		if (nOffsetFrom < 0) nOffsetFrom += 7;
		m_oleDateFrom -= COleDateTimeSpan(nOffsetFrom, 0, 0, 0);

		int nWeekDayTo = m_oleDateTo.GetDayOfWeek() - 1;
		int nOffsetTo = (nWeekStartDay - nWeekDayTo) - 1;
		if (nOffsetTo < 0) nOffsetTo += 7;
		m_oleDateTo += COleDateTimeSpan(nOffsetTo, 0, 0, 0);
	}
	break;

	case 2:	//ROUND TO MONTH
	{
		int nYearFrom = m_oleDateFrom.GetYear();
		int nMonthFrom = m_oleDateFrom.GetMonth();
		m_oleDateFrom = COleDateTime(nYearFrom, nMonthFrom, 1, 0, 0, 0);

		int nYearTo = m_oleDateTo.GetYear();
		int nMonthTo = m_oleDateTo.GetMonth();

		nMonthTo++;
		if (nMonthTo > 12)
		{
			nYearTo++;
			nMonthTo = 1;
		}

		m_oleDateTo = COleDateTime(nYearTo, nMonthTo, 1, 0, 0, 0);
		m_oleDateTo -= COleDateTimeSpan(1, 0, 0, 0);
	}

	default:
		break;
	}

	int n = m_comboSession.GetCurSel();
	if (n >= 0 && n < DataManagerNonDb.ReportSession.GetSize())
	{
		CSessionCSVRecord Session;
		DataManagerNonDb.ReportSession.GetAt(n, m_ReportSession);
	}
	else
	{
		m_ReportSession.ClearRecord();
	}
}

/**********************************************************************/

bool CSalesHistoryReportBritanniaDlg::GetAdhocReportSession()
{
	CSessionEditDlg dlg( m_ReportSessionAdhoc, TRUE, this );

	if (dlg.DoModal() != IDOK)
	{
		return FALSE;
	}

	CCSV csv;
	m_ReportSessionAdhoc.ConvertToCSV( csv );
	m_ReportSession.ConvertFromCSV( csv );
	return TRUE;
}

/**********************************************************************/

void CSalesHistoryReportBritanniaDlg::OnButtonDisplay() 
{
	if (UpdateData(TRUE) == FALSE)
	{
		return;
	}

	CWaitCursor wait;
	CommsHandshaker.ProcessSales();
	
	GetReportDates();

	if (m_comboSession.GetCurSel() == m_comboSession.GetCount() - 1)
	{
		if (GetAdhocReportSession() == FALSE)
		{
			return;
		}
	}

	//WORK OUT FILES WE NEED TO READ FOR REPORT
	CLocationSelectorEntity LocSelEntity;
	m_LocationSelector.GetSelectedEntity( LocSelEntity );
		
	CDataManagerInfo info;
	DataManager.OpenDatabaseReadOnly( LocSelEntity.GetDbIdx(), info, FALSE );
	
	CSalesHistoryCSVRecordPlu ReportRecord;
	ReportRecord.SetName( "CSV Sales Export for Britannia Hotels" );
	ReportRecord.SetReportType( SH_CUSTOM );

	CTimeSliceMapCSVRecord TimeSliceRecord;
	m_HistoryReport.ResetForBritannia( LocSelEntity, m_checkGuest.GetCheck() != 0 );
	m_HistoryReport.SetReportDates( m_oleDateFrom, m_oleDateTo, m_ReportSession, TimeSliceRecord );
	
	CString strSessionTitle;
	if (m_comboSession.GetCurSel() < m_comboSession.GetCount() - 1)
	{
		m_comboSession.GetWindowText(strSessionTitle);
	}
	else
	{
		strSessionTitle = m_ReportSessionAdhoc.GetName();
	}

	m_HistoryReport.SetSessionTitle( strSessionTitle );
	m_HistoryReport.SetPercentSaleFlag( FALSE );
	m_HistoryReport.SetEstimatedProfitFlag( FALSE );
	m_HistoryReport.SetVIPSalesFlag( FALSE );
	m_HistoryReport.m_ReportFilters.DeptFilter.SetDeptFilterNo( m_DepartmentSelector.GetDepartmentFilter() );
	m_HistoryReport.SetDepartmentByItemFlag( FALSE );
	m_HistoryReport.SetTaxFilter(0);
	m_HistoryReport.SetPriceLevelFilter(0);

	int nLocIdx = 0;
	if (LocSelEntity.GetConType() == NODE_LOCATION)
	{
		nLocIdx = LocSelEntity.GetEntityIdx();
	}
	else
	{
		int nDbNo = LocSelEntity.GetDbNo();
		for (int n = 0; n < dbLocation.GetSize(); n++)
		{
			if (dbLocation.GetDbNo(n) == nDbNo)
			{
				nLocIdx = n;
				break;
			}
		}
	}

	CBritanniaHotelsExportArray ExportArray; 
	m_HistoryReport.ConsolidateForBritannia( ExportArray );
	m_HistoryReport.CreateReportForBritannia( nLocIdx, ExportArray, this );
}

/**********************************************************************/

void CSalesHistoryReportBritanniaDlg::OnChangeDateFrom(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_DateSelectorRange.OnChangeFromDate();
	m_DateSelectorRange.CheckForChangedDate();
	*pResult = 0;
}

/**********************************************************************/

void CSalesHistoryReportBritanniaDlg::OnChangeDateTo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_DateSelectorRange.CheckForChangedDate();
	*pResult = 0;
}

/**********************************************************************/

void CSalesHistoryReportBritanniaDlg::OnKillFocusDeptNo()
{
	m_DepartmentSelector.SelectDepartmentFromEditBox();
}

/**********************************************************************/

void CSalesHistoryReportBritanniaDlg::OnSelectDeptName()
{
	m_DepartmentSelector.SelectDepartmentFromCombo();
}

/**********************************************************************/