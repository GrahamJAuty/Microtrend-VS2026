/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "LocationCSVArray.h"
#include "Password.h"
/**********************************************************************/
#include "ReportEposDlg.h"
/**********************************************************************/

CReportEposDlg::CReportEposDlg( int nPresetNo, bool bPresetPrint, bool bAutoRunPreset, CWnd* pParent)
	: CSSDialog(CReportEposDlg::IDD, pParent), 
	m_DepartmentSelector ( m_editDeptNo, m_comboDeptName ),
	m_DateSelectorRange( m_comboDates, m_DatePickerFrom, m_DatePickerTo )
{
	//{{AFX_DATA_INIT(CReportEposDlg)
	//}}AFX_DATA_INIT
	m_nPresetNo = nPresetNo;
	m_nPresetError = 0;
	m_bPresetPrint = bPresetPrint;
	m_bAutoRunPreset = bAutoRunPreset;
	DataManagerNonDb.ServerNameTable.BuildList();
	DataManagerNonDb.CustomerNameTable.BuildList();
	dbDatabase.BuildSortIdxTables();
	dbLocation.BuildSortIdxTables();
	DataManagerNonDb.EODDateTimeFilterArray.PrepareForUse();
	DataManagerNonDb.SessionDateTimeFilter.Reset();
}

/**********************************************************************/

CReportEposDlg::~CReportEposDlg()
{
	DataManagerNonDb.ServerNameTable.ClearList();
	DataManagerNonDb.CustomerNameTable.ClearList();
	DataManagerNonDb.EODDateTimeFilterArray.ClearAfterUse();
	DataManagerNonDb.SessionDateTimeFilter.Reset();
}

/**********************************************************************/

void CReportEposDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CReportEposDlg)

	DDX_Control(pDX, IDC_COMBO_BATCH, m_comboBatch);
	
	DDX_Control(pDX, IDC_CHECK_ITEMSERVER, m_checkItemServer);
	
	DDX_Control(pDX, IDC_COMBO_DATES, m_comboDates);
	DDX_Control(pDX, IDC_DATEPICKER_TO, m_DatePickerTo);
	DDX_Control(pDX, IDC_DATEPICKER_FROM, m_DatePickerFrom);
	DDX_Control(pDX, IDC_COMBO_SESSION, m_comboSession);

	DDX_Control(pDX, IDC_RADIO_CONSOL_DEFAULT, m_radioConsolDefault);
	DDX_Control(pDX, IDC_RADIO_CONSOL_SAVED, m_radioConsolSaved);
	DDX_Control(pDX, IDC_RADIO_CONSOL_ADHOC, m_radioConsolAdhoc);
	DDX_Control(pDX, IDC_RADIO_CONSOL_SYSTEM, m_radioConsolSystem);
	
	DDX_Control(pDX, IDC_CHECK_LOCATIONS, m_checkReportLocations);
	DDX_Control(pDX, IDC_CHECK_EPOS, m_checkReportEpos);
	DDX_Control(pDX, IDC_CHECK_ALL, m_checkReportAllLocations);
	DDX_Control(pDX, IDC_BUTTON_PRINT_QUICK, m_buttonPrintQuick);
	
	DDX_Control(pDX, IDC_STATIC_PRESET, m_staticPreset);
	DDX_Control(pDX, IDC_BUTTON_PRESET, m_buttonPreset);
	DDX_Control(pDX, IDC_COMBO_PRESET, m_comboPreset);
	DDX_Control(pDX, IDC_COMBO_LOCATION, m_comboLocation);
	DDX_Control(pDX, IDC_COMBO_TERMINAL, m_comboTerminal);
	DDX_Control(pDX, IDC_COMBO_TERMDATE, m_comboTermDate);
	DDX_Control(pDX, IDC_EDIT_DEPTNO, m_editDeptNo);
	DDX_Control(pDX, IDC_COMBO_DEPTNAME, m_comboDeptName);
	DDX_Control(pDX, IDC_COMBO_PLUFILTER, m_comboPluFilter);
	DDX_Control(pDX, IDC_COMBO_PLUTIME, m_comboPluTime);
	DDX_Control(pDX, IDC_BUTTON_GRAPH_SINGLE, m_buttonGraphSingle);
	DDX_Control(pDX, IDC_BUTTON_PRINT_MULTIPLE, m_buttonPrintMultiple);
	DDX_Control(pDX, IDC_BUTTON_DISPLAY_SINGLE, m_buttonDisplaySingle);
	
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CReportEposDlg, CDialog)
	//{{AFX_MSG_MAP(CReportEposDlg)

	ON_WM_MEASUREITEM()

	ON_CBN_SELCHANGE(IDC_COMBO_BATCH, OnSelectBatch)
	
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST, OnSelectReport)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDoubleClickList)
	
	ON_CBN_SELCHANGE(IDC_COMBO_DATES, OnSelectDates)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATEPICKER_FROM, OnChangeDateFrom)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATEPICKER_TO, OnChangeDateTo)

	ON_BN_CLICKED(IDC_CHECK_ALL, OnCheckQuickPrint)
	ON_BN_CLICKED(IDC_CHECK_EPOS, OnCheckQuickPrint)
	ON_BN_CLICKED(IDC_CHECK_LOCATIONS, OnCheckQuickPrint)
	ON_BN_CLICKED(IDC_BUTTON_PRINT_QUICK, OnButtonPrintQuick)
	
	ON_BN_CLICKED(IDC_BUTTON_PRESET, OnButtonPreset)
	ON_CBN_SELCHANGE(IDC_COMBO_PRESET, OnSelectPreset)
	ON_CBN_SELCHANGE(IDC_COMBO_LOCATION, OnSelectLocation)
	ON_EN_KILLFOCUS(IDC_EDIT_DEPTNO, OnKillFocusDeptNo)
	ON_CBN_SELCHANGE(IDC_COMBO_DEPTNAME, OnSelectDeptName)
	ON_BN_CLICKED(IDC_BUTTON_DISPLAY_SINGLE, OnButtonDisplaySingle)
	ON_BN_CLICKED(IDC_BUTTON_PRINT_MULTIPLE, OnButtonPrintMultiple)
	ON_BN_CLICKED(IDC_BUTTON_GRAPH_SINGLE, OnButtonGraphSingle)
	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

int CReportEposDlg::GetSelectedReportType()
{
	return m_dlgHelpers.GetSelectedReportType();
}

/**********************************************************************/

BOOL CReportEposDlg::OnInitDialog() 
{
	m_dlgHelpers.SetControlPointers( this );
	CDialog::OnInitDialog();
	m_dlgHelpers.HandleInitDialog( m_nPresetNo, m_bPresetPrint, m_bAutoRunPreset, m_nPresetError );

	if ( ( m_nPresetNo != 0 ) && ( TRUE == m_bAutoRunPreset ) )
	{
		EndDialog( IDCANCEL );
		return FALSE;
	}

	bool bPreset = PasswordArray.CheckLocationAccessSystem();
	ShowAndEnableWindow( &m_buttonPreset, bPreset );
	ShowAndEnableWindow( &m_staticPreset, FALSE == bPreset );

	return TRUE;  
}

/**********************************************************************/

void CReportEposDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	if ( nIDCtl == IDC_LIST )	
	{
		m_listReport.AdjustRowHeight( lpMeasureItemStruct, 10 );
	}
}

/**********************************************************************/

void CReportEposDlg::OnSelectBatch() 
{
	m_dlgHelpers.HandleSelectBatch();
}

/**********************************************************************/

void CReportEposDlg::OnSelectReport(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

    if ( ( pNMListView->uChanged & LVIF_STATE ) && ( pNMListView-> uNewState & LVNI_SELECTED ))
    {
		m_dlgHelpers.HandleSelectReport();
	}

	*pResult = 0;
}

/**********************************************************************/

void CReportEposDlg::OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnButtonDisplaySingle();
	*pResult = 0;
}

/**********************************************************************/

void CReportEposDlg::OnSelectDates() 
{
	m_dlgHelpers.HandleSelectDates();	
}

/**********************************************************************/

void CReportEposDlg::OnChangeDateFrom(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_dlgHelpers.HandleChangeDateFrom();
	*pResult = 0;
}

/**********************************************************************/

void CReportEposDlg::OnChangeDateTo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_dlgHelpers.HandleChangeDateTo();
	*pResult = 0;
}

/**********************************************************************/

void CReportEposDlg::OnCheckQuickPrint() 
{
	m_dlgHelpers.HandleCheckQuickPrint();
}

/**********************************************************************/

void CReportEposDlg::OnSelectLocation() 
{
	m_dlgHelpers.HandleSelectLocation();
}

/**********************************************************************/

void CReportEposDlg::OnKillFocusDeptNo() 
{
	m_dlgHelpers.HandleKillFocusDeptNo();
}

/**********************************************************************/

void CReportEposDlg::OnSelectDeptName() 
{
	m_dlgHelpers.HandleSelectDeptName();
}

/**********************************************************************/

void CReportEposDlg::OnButtonDisplaySingle() 
{
	m_dlgHelpers.HandleDisplaySingle();
}

/**********************************************************************/

void CReportEposDlg::OnButtonGraphSingle() 
{	
	m_dlgHelpers.HandleGraphSingle();
}

/**********************************************************************/

void CReportEposDlg::OnButtonPrintMultiple() 
{
	m_dlgHelpers.HandlePrintMultiple();
}

/**********************************************************************/

void CReportEposDlg::OnButtonPrintQuick() 
{
	m_dlgHelpers.HandlePrintQuick();
}

/**********************************************************************/

void CReportEposDlg::OnButtonPreset() 
{
	m_dlgHelpers.HandleButtonPreset();
}

/**********************************************************************/

void CReportEposDlg::OnSelectPreset() 
{
	m_dlgHelpers.HandleSelectPreset();
}

/**********************************************************************/







