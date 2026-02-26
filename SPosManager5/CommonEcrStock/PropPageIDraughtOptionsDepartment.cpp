/**********************************************************************/
#include "IDraughtOptions.h"
#include "NodeTypes.h"
/**********************************************************************/
#include "PropPageIDraughtOptionsDepartment.h"
/**********************************************************************/

CPropPageIDraughtOptionsDepartment::CPropPageIDraughtOptionsDepartment() : CSSPropertyPage(CPropPageIDraughtOptionsDepartment::IDD)
{
	//{{AFX_DATA_INIT(CPropPageIDraughtOptionsDepartment)
	//}}AFX_DATA_INIT
	m_nCurrentDbIdx = -1;
}

/**********************************************************************/

CPropPageIDraughtOptionsDepartment::~CPropPageIDraughtOptionsDepartment()
{
}

/**********************************************************************/

void CPropPageIDraughtOptionsDepartment::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPageIDraughtOptionsDepartment)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_COMBO_DATABASE, m_comboDatabase);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPageIDraughtOptionsDepartment, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPageIDraughtOptionsDepartment)
	//}}AFX_MSG_MAP
	ON_CBN_SELCHANGE( IDC_COMBO_DATABASE, OnSelectDatabase)
	ON_BN_CLICKED( IDC_BUTTON_ADD, OnButtonAdd )
	ON_BN_CLICKED( IDC_BUTTON_REMOVE, OnButtonRemove )
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_SELECTED, OnDoubleClickListSelected)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_AVAILABLE, OnDoubleClickListAvailable)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPropPageIDraughtOptionsDepartment::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	CWordArray arrayTypes;

	if ( EcrmanOptions.GetGlobalDepartmentFlag() == TRUE )
	{
		arrayTypes.Add( NODE_SYSTEM );
		m_LocationSelector.SetForceSystemFlag( TRUE );
	}
	else
		arrayTypes.Add( NODE_DATABASE );

	m_LocationSelector.BuildList( arrayTypes );
	m_LocationSelector.FillLocationCombo ( &m_comboDatabase );

	m_listSelected.SubclassDlgItem ( IDC_LIST_SELECTED, this );
	m_listSelected.InsertColumn ( 0, "No", LVCFMT_LEFT, 30 );
	m_listSelected.InsertColumn ( 1, "Department", LVCFMT_LEFT, 130 );

	m_listAvailable.SubclassDlgItem ( IDC_LIST_AVAILABLE, this );
	m_listAvailable.InsertColumn ( 0, "No", LVCFMT_LEFT, 30 );
	m_listAvailable.InsertColumn ( 1, "Department", LVCFMT_LEFT, 130 );

	LoadDatabaseDepartments(0);
	m_nCurrentDbIdx = 0;
	AddList();

	if ( m_arraySelected.GetSize() > 0 )
		m_listSelected.SetCurSel(0);

	if ( m_arrayAvailable.GetSize() > 0 )
		m_listAvailable.SetCurSel(0);

	return TRUE;  
}

/**********************************************************************/

void CPropPageIDraughtOptionsDepartment::LoadDatabaseDepartments( int nDbIdx )
{
	m_arraySelected.RemoveAll();
	m_arrayAvailable.RemoveAll();

	CIDraughtDeptFilter info;
	IDraughtOptions.GetDatabaseDepartmentFilter( nDbIdx, info );

	CDataManagerInfo infoDb;
	DataManager.OpenDatabaseQuickNoFiles( nDbIdx, FALSE );
	DataManager.OpenDepartment( DB_READONLY, infoDb );

	for ( int nDeptIdx = 0; nDeptIdx < DataManager.Department.GetSize(); nDeptIdx++ )
	{
		CDepartmentCSVRecord DeptRecord;
		DataManager.Department.GetAt( nDeptIdx, DeptRecord );
		
		int nDeptNo = DeptRecord.GetDeptNo();
		if ( ( nDeptNo >= 1 ) && ( nDeptNo <= 99 ) )
		{
			CSortedIntItem item;
			item.m_nItem = nDeptNo;

			if ( IDraughtOptions.GetDatabaseDepartmentFlag( nDbIdx, nDeptNo ) == TRUE )
				m_arraySelected.Consolidate( item );
			else
				m_arrayAvailable.Consolidate( item );
		}
	}
}

/**********************************************************************/

void CPropPageIDraughtOptionsDepartment::SaveDatabaseDepartments( int nDbIdx )
{
	CSortedIntItem item;

	for ( int n = 0; n < m_arrayAvailable.GetSize(); n++ )
	{
		m_arrayAvailable.GetAt( n, item );
		IDraughtOptions.SetDatabaseDepartmentFlag( nDbIdx, item.m_nItem, FALSE );
	}

	for ( int n = 0; n < m_arraySelected.GetSize(); n++ )
	{
		m_arraySelected.GetAt( n, item );
		IDraughtOptions.SetDatabaseDepartmentFlag( nDbIdx, item.m_nItem, TRUE );
	}
}

/**********************************************************************/

void CPropPageIDraughtOptionsDepartment::OnSelectDatabase()
{
	CLocationSelectorEntity LocSelEntity;
	m_LocationSelector.GetSelectedEntity( LocSelEntity );
	
	if ( LocSelEntity.GetDbIdx() != m_nCurrentDbIdx )
	{
		SaveDatabaseDepartments( m_nCurrentDbIdx );
		LoadDatabaseDepartments( LocSelEntity.GetDbIdx() );
		m_nCurrentDbIdx = LocSelEntity.GetDbIdx();
		AddList();
	}
}

/**********************************************************************/

void CPropPageIDraughtOptionsDepartment::AddList()
{
	CSortedIntItem item;

	m_listSelected.DeleteAllItems();

	for ( int nIndex = 0; nIndex < m_arraySelected.GetSize(); nIndex++ )
	{
		m_arraySelected.GetAt( nIndex, item );
		
		CString strLine;
		strLine.Format ( "%2.2d\t%s",
			item.m_nItem,
			DataManager.Department.GetReportTextByDeptNo( item.m_nItem ) );
	
		m_listSelected.AddString ( strLine );
	}

	m_listAvailable.DeleteAllItems();

	for ( int nIndex = 0; nIndex < m_arrayAvailable.GetSize(); nIndex++ )
	{
		m_arrayAvailable.GetAt( nIndex, item );
		
		CString strLine;
		strLine.Format ( "%2.2d\t%s",
			item.m_nItem,
			DataManager.Department.GetReportTextByDeptNo( item.m_nItem ) );
	
		m_listAvailable.AddString ( strLine );
	}
}

/**********************************************************************/

void CPropPageIDraughtOptionsDepartment::OnButtonAdd()
{
	MoveDepartment( m_listAvailable.GetCurSel(), m_listAvailable, m_listSelected, m_arrayAvailable, m_arraySelected );
}

/**********************************************************************/

void CPropPageIDraughtOptionsDepartment::OnButtonRemove()
{
	MoveDepartment( m_listSelected.GetCurSel(), m_listSelected, m_listAvailable, m_arraySelected, m_arrayAvailable );
}

/**********************************************************************/

void CPropPageIDraughtOptionsDepartment::OnDoubleClickListSelected(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnButtonRemove();
	*pResult = 0;
}

/**********************************************************************/

void CPropPageIDraughtOptionsDepartment::OnDoubleClickListAvailable(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnButtonAdd();
	*pResult = 0;
}

/**********************************************************************/

void CPropPageIDraughtOptionsDepartment::MoveDepartment( int nSel, CSSListCtrl& listSource, CSSListCtrl& listDest, CReportConsolidationArray<CSortedIntItem>& arraySource, CReportConsolidationArray<CSortedIntItem>& arrayDest )
{
	if ( ( nSel >= 0 ) && ( nSel < arraySource.GetSize() ) )
	{
		CSortedIntItem item;
		arraySource.GetAt( nSel, item );
		arraySource.RemoveAt( nSel );
		
		arrayDest.Consolidate( item );
		AddList();

		if ( nSel < arraySource.GetSize() )
			listSource.SetCurSel( nSel );
		else
			listSource.SetCurSel( nSel - 1 );

		if ( arrayDest.Find( item, nSel ) == TRUE )
			listDest.SetCurSel( nSel );
	}
}

/**********************************************************************/

BOOL CPropPageIDraughtOptionsDepartment::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	SaveRecord();
	return bResult;
}

/**********************************************************************/

BOOL CPropPageIDraughtOptionsDepartment::OnSetActive() 
{
	bool bResult = ( CPropertyPage::OnSetActive() != 0 );
	return bResult;
}

/**********************************************************************/

void CPropPageIDraughtOptionsDepartment::SaveRecord()
{
	if ( -1 != m_nCurrentDbIdx )
		SaveDatabaseDepartments( m_nCurrentDbIdx );
}

/**********************************************************************/

