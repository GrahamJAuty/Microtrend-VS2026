/**********************************************************************/
#include "ListDataGroupDlg.h"
#include "NodeTypes.h"
#include "SysInfo.h"
/**********************************************************************/
#include "PropPageDepartmentGeneral.h"
/**********************************************************************/

CPropPageDepartmentGeneral::CPropPageDepartmentGeneral() : CSSPropertyPage(CPropPageDepartmentGeneral::IDD),
	m_GroupSelectorEpos(m_editGroupEpos, m_comboGroupEpos, NODE_DEPT_GROUP_EPOS),
	m_GroupSelectorReport(m_editGroupReport, m_comboGroupReport, NODE_DEPT_GROUP_REPORT),
	m_GroupSelectorConsol(m_editGroupConsol, m_comboGroupConsol, NODE_DEPT_GROUP_CONSOL)
{
	//{{AFX_DATA_INIT(CPropPageDepartmentGeneral)
	//}}AFX_DATA_INIT
	m_pDeptRecord = NULL;
}

/**********************************************************************/

CPropPageDepartmentGeneral::~CPropPageDepartmentGeneral()
{
}

/**********************************************************************/

void CPropPageDepartmentGeneral::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPageDepartmentGeneral)

	DDX_Control(pDX, IDC_EDIT_EPOSTEXT, m_editEposText);
	DDX_Control(pDX, IDC_EDIT_GROUP_EPOS, m_editGroupEpos);
	DDX_Control(pDX, IDC_COMBO_GROUP_EPOS, m_comboGroupEpos);
	
	DDX_Control(pDX, IDC_EDIT_REPORTTEXT, m_editReportText);
	DDX_Control(pDX, IDC_EDIT_GROUP_REPORT, m_editGroupReport);
	DDX_Control(pDX, IDC_COMBO_GROUP_REPORT, m_comboGroupReport);
	DDX_Control(pDX, IDC_EDIT_GROUP_CONSOL, m_editGroupConsol);
	DDX_Control(pDX, IDC_COMBO_GROUP_CONSOL, m_comboGroupConsol);
	
	DDX_Control(pDX, IDC_CHECK_ENABLE, m_checkEnable );
	DDX_Control(pDX, IDC_CHECK_IDRAUGHT, m_checkIDraught);
	DDX_Control(pDX, IDC_STATIC_SPTBOOK, m_staticSptBook);
	DDX_Control(pDX, IDC_CHECK_SPTBOOK, m_checkSptBook);

	DDX_Text(pDX, IDC_EDIT_EPOSTEXT, m_strEposText);
	DDX_Text(pDX, IDC_EDIT_REPORTTEXT, m_strReportText);

	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPageDepartmentGeneral, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPageDepartmentGeneral)
	ON_CBN_SELCHANGE(IDC_COMBO_GROUP_EPOS, OnSelectGroupEpos)
	ON_EN_KILLFOCUS(IDC_EDIT_GROUP_EPOS, OnKillFocusGroupNoEpos)
	ON_BN_CLICKED(IDC_BUTTON_SET_EPOS, OnButtonSetEpos)
	ON_BN_CLICKED(IDC_BUTTON_SET_REPORT, OnButtonSetReport)
	ON_BN_CLICKED(IDC_BUTTON_SET_CONSOL, OnButtonSetConsol)
	ON_CBN_SELCHANGE(IDC_COMBO_GROUP_REPORT, OnSelectGroupReport)
	ON_EN_KILLFOCUS(IDC_EDIT_GROUP_REPORT, OnKillFocusGroupNoReport)
	ON_CBN_SELCHANGE(IDC_COMBO_GROUP_CONSOL, OnSelectGroupConsol)
	ON_EN_KILLFOCUS(IDC_EDIT_GROUP_CONSOL, OnKillFocusGroupNoConsol)
	ON_BN_CLICKED( IDC_CHECK_SPTBOOK, OnTickSportsBooker)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPropPageDepartmentGeneral::OnInitDialog() 
{
	GetRecordData();
	CPropertyPage::OnInitDialog();

	m_editEposText.LimitText( Dept::EposText.Max );
	m_editReportText.LimitText( Dept::RepText.Max );

	if ( SysInfo.IsIDraughtExportSystem() == FALSE )
	{
		ShowAndEnableWindow( &m_checkIDraught, FALSE );
		MoveControl( &m_checkSptBook, 25, 187 );
		MoveControl( &m_staticSptBook, 36, 188 );
	}

	if ( SysInfo.IsSportsBookerSystem() == FALSE )
	{
		m_staticSptBook.ShowWindow( SW_HIDE );
		ShowAndEnableWindow( &m_checkSptBook, FALSE );
	}

	return TRUE;  
}

/**********************************************************************/

BOOL CPropPageDepartmentGeneral::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	SaveRecord();
	return bResult;
}

/**********************************************************************/

BOOL CPropPageDepartmentGeneral::OnSetActive() 
{
	GetRecordData();
	bool bResult = ( CPropertyPage::OnSetActive() != 0 );
	SetRecordControls();
	return bResult;
}

/**********************************************************************/

void CPropPageDepartmentGeneral::GetRecordData()
{
	m_strEposText = m_pDeptRecord -> GetEposText();
	m_strReportText = m_pDeptRecord -> GetRepText();
}

/**********************************************************************/

void CPropPageDepartmentGeneral::SetRecordControls()
{
	m_GroupSelectorEpos.FillGroupCombo ( m_pDeptRecord -> GetEposGroup() );
	m_GroupSelectorReport.FillGroupCombo ( m_pDeptRecord -> GetReportGroup() );
	UpdateConsolGroupFromReportGroup();

	m_checkEnable.SetCheck( m_pDeptRecord -> GetReportEnable() );
	m_checkIDraught.SetCheck( m_pDeptRecord -> GetIDraughtExportFlag() );

	m_editEposText.SetSel ( 0, -1 );
	m_editEposText.SetFocus();

	m_checkSptBook.SetCheck( m_pDeptRecord -> GetDeptNo() == DataManager.Department.GetSportsBookerDeptNo() );
	OnTickSportsBooker();
}

/**********************************************************************/

void CPropPageDepartmentGeneral::OnTickSportsBooker()
{
	if ( SysInfo.IsSportsBookerSystem() == TRUE )
	{
		CString strText = "Sports Booker default department";

		if ( m_checkSptBook.GetCheck() != 0 )
		{
			m_checkSptBook.EnableWindow( FALSE );
			m_staticSptBook.SetWindowText( strText );
		}
		else
		{
			m_checkSptBook.EnableWindow( TRUE );
			
			int nSptBookDeptNo = DataManager.Department.GetSportsBookerDeptNo();

			if ( 0 != nSptBookDeptNo )
			{
				int nDeptIdx;
				if ( DataManager.Department.FindDeptByNumber( nSptBookDeptNo, nDeptIdx ) == FALSE )
				{
					nSptBookDeptNo = 0;
					DataManager.Department.SetSportsBookerDeptNo(0);
				}
			}

			if ( 0 == nSptBookDeptNo )
				m_staticSptBook.SetWindowText( strText + " (currently not set)" );
			else
			{
				CString strExtra;
				strExtra.Format ( " (currently department %d)", nSptBookDeptNo );
				m_staticSptBook.SetWindowText( strText + strExtra );
			}
		}
	}
}

/**********************************************************************/

bool CPropPageDepartmentGeneral::UpdateRecord()
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		SaveRecord();
		return TRUE;
	}
	return FALSE;
}

/**********************************************************************/

void CPropPageDepartmentGeneral::OnSelectGroupEpos() 
{
	m_GroupSelectorEpos.SelectGroupFromCombo();
}

/**********************************************************************/

void CPropPageDepartmentGeneral::OnKillFocusGroupNoEpos() 
{
	m_GroupSelectorEpos.SelectGroupFromEditBox();
}

/**********************************************************************/

void CPropPageDepartmentGeneral::OnButtonSetEpos()
{
	m_GroupSelectorEpos.SelectGroupFromEditBox();

	DataManager.EposGroup.RememberSettings();
	CListDataGroupDlg dlg( NODE_DEPT_GROUP_EPOS, m_GroupSelectorEpos.GetGroupFilter(), this );
	
	if ( dlg.DoModal() == IDOK )
	{
		CDataManagerInfo info;
		if ( DataManager.WriteEposGroup( info ) == FALSE )
			Prompter.WriteError( info );

		m_GroupSelectorEpos.FillGroupCombo ( dlg.GetGroupNoSel() );
	}
	else
		DataManager.EposGroup.RestoreSettings();
}

/**********************************************************************/

void CPropPageDepartmentGeneral::OnSelectGroupReport() 
{
	m_GroupSelectorReport.SelectGroupFromCombo();
	UpdateConsolGroupFromReportGroup();
}

/**********************************************************************/

void CPropPageDepartmentGeneral::OnKillFocusGroupNoReport() 
{
	m_GroupSelectorReport.SelectGroupFromEditBox();
	UpdateConsolGroupFromReportGroup();
}

/**********************************************************************/

void CPropPageDepartmentGeneral::UpdateConsolGroupFromReportGroup()
{
	int nGroupIdx;
	int nConsolGroup = 0;
	int nReportGroup = m_GroupSelectorReport.GetGroupFilter();
	if ( DataManager.ReportGroup.FindGroupByNumber( nReportGroup, nGroupIdx ) == TRUE )
	{
		CGroupCSVRecordReport ReportGroup;
		DataManager.ReportGroup.GetAt( nGroupIdx, ReportGroup );
		nConsolGroup = ReportGroup.GetParentGroupNo();
	}

	if ( 0 == nReportGroup )
	{
		m_GroupSelectorConsol.FillGroupCombo ( 0 );
		m_editGroupConsol.EnableWindow( FALSE );
		m_comboGroupConsol.EnableWindow( FALSE );
	}
	else
	{
		m_GroupSelectorConsol.FillGroupCombo ( nConsolGroup );
		m_editGroupConsol.EnableWindow( TRUE );
		m_comboGroupConsol.EnableWindow( TRUE );
	}
}

/**********************************************************************/

void CPropPageDepartmentGeneral::LinkReportGroupToConsolGroup()
{
	int nReportGroup = m_GroupSelectorReport.GetGroupFilter();
	int nConsolGroup = m_GroupSelectorConsol.GetGroupFilter();

	if ( 0 == nReportGroup )
		return;

	if ( ( nReportGroup < ReportGroup::GroupNo.Min ) || ( nReportGroup > ReportGroup::GroupNo.Max ) )
		return;

	if ( ( nConsolGroup < ConsolGroup::GroupNo.Min ) || ( nConsolGroup > ConsolGroup::GroupNo.Max ) )
		return;

	int nReportGroupIdx;
	if ( DataManager.ReportGroup.FindGroupByNumber( nReportGroup, nReportGroupIdx ) == FALSE )
	{
		CGroupCSVRecordReport ReportGroup;
		ReportGroup.SetDefaultGroup( nReportGroup );
		ReportGroup.SetParentGroupNo( nConsolGroup );
		DataManager.ReportGroup.InsertAt( nReportGroupIdx, ReportGroup );
	}
	else
	{
		CGroupCSVRecordReport ReportGroup;
		DataManager.ReportGroup.GetAt( nReportGroupIdx, ReportGroup );
		ReportGroup.SetParentGroupNo( nConsolGroup );
		DataManager.ReportGroup.SetAt( nReportGroupIdx, ReportGroup );
	}
}

/**********************************************************************/

void CPropPageDepartmentGeneral::OnButtonSetReport()
{
	m_GroupSelectorReport.SelectGroupFromEditBox();
	m_GroupSelectorConsol.SelectGroupFromEditBox();
	LinkReportGroupToConsolGroup();

	DataManager.ReportGroup.RememberSettings();
	CListDataGroupDlg dlg( NODE_DEPT_GROUP_REPORT, m_GroupSelectorReport.GetGroupFilter(), this );
	
	if ( dlg.DoModal() == IDOK )
	{
		CDataManagerInfo info;
		if ( DataManager.WriteReportGroup( info ) == FALSE )
			Prompter.WriteError( info );

		m_GroupSelectorReport.FillGroupCombo ( dlg.GetGroupNoSel() );
		UpdateConsolGroupFromReportGroup();
	}
	else
		DataManager.ReportGroup.RestoreSettings();
}

/**********************************************************************/

void CPropPageDepartmentGeneral::OnSelectGroupConsol() 
{
	m_GroupSelectorConsol.SelectGroupFromCombo();
}

/**********************************************************************/

void CPropPageDepartmentGeneral::OnKillFocusGroupNoConsol() 
{
	m_GroupSelectorConsol.SelectGroupFromEditBox();
}

/**********************************************************************/

void CPropPageDepartmentGeneral::OnButtonSetConsol()
{
	m_GroupSelectorConsol.SelectGroupFromEditBox();

	DataManager.ConsolGroup.RememberSettings();
	CListDataGroupDlg dlg( NODE_DEPT_GROUP_CONSOL, m_GroupSelectorConsol.GetGroupFilter(), this );
	
	if ( dlg.DoModal() == IDOK )
	{
		CDataManagerInfo info;
		if ( DataManager.WriteConsolGroup( info ) == FALSE )
			Prompter.WriteError( info );

		m_GroupSelectorConsol.FillGroupCombo ( dlg.GetGroupNoSel() );
	}
	else
		DataManager.ConsolGroup.RestoreSettings();
}

/**********************************************************************/

void CPropPageDepartmentGeneral::SaveRecord()
{
	m_GroupSelectorEpos.SelectGroupFromEditBox();
	m_GroupSelectorReport.SelectGroupFromEditBox();
	m_GroupSelectorConsol.SelectGroupFromEditBox();
	LinkReportGroupToConsolGroup();

	m_pDeptRecord -> SetEposText ( m_strEposText );
	m_pDeptRecord -> SetEposGroup ( m_GroupSelectorEpos.GetGroupFilter() );
	m_pDeptRecord -> SetReportGroup ( m_GroupSelectorReport.GetGroupFilter() );
	
	m_pDeptRecord -> SetRepText ( m_strReportText );
	m_pDeptRecord -> SetReportEnable ( m_checkEnable.GetCheck() != 0 );
	m_pDeptRecord -> SetIDraughtExportFlag( m_checkIDraught.GetCheck() != 0 );
		
	if ( m_checkSptBook.GetCheck() != 0 )
		DataManager.Department.SetSportsBookerDeptNo( m_pDeptRecord -> GetDeptNo() );
}

/**********************************************************************/

void CPropPageDepartmentGeneral::Refresh()
{
	GetRecordData();
	UpdateData ( FALSE );
	SetRecordControls();
}

/**********************************************************************/

