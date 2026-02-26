/**********************************************************************/
#include "DepartmentCSVArray.h"
#include "DepartmentSetCSVArray.h"
#include "MaxLengths.h"
#include "SearchTypes.h"
/**********************************************************************/
#include "PluFilterDlgEcrman.h"
/**********************************************************************/

CPluFilterDlgEcrman::CPluFilterDlgEcrman( CPluFilterArray& FilterArray, CWnd* pParent )
	: CDialog(CPluFilterDlgEcrman::IDD, pParent), m_PluFilterArray ( FilterArray ), m_DepartmentSelector ( m_editDeptNo, m_comboDeptName )
{
	//{{AFX_DATA_INIT(CPluFilterDlgEcrman)
	//}}AFX_DATA_INIT
	m_nPluIdx = -1;
	m_strTitle = "";
	m_strSearchText = m_PluFilterArray.GetDescription();
	m_nDepartmentFilter = m_PluFilterArray.GetDepartmentFilter();
	m_nCategoryFilter = m_PluFilterArray.GetAnalysisCategoryFilter();
}

/**********************************************************************/

void CPluFilterDlgEcrman::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPluFilterDlgEcrman)
	DDX_Control(pDX, IDC_EDIT_DEPTNO, m_editDeptNo);
	DDX_Control(pDX, IDC_COMBO_DEPTNAME, m_comboDeptName);
	DDX_Control(pDX, IDC_COMBO_CATEGORY, m_comboCategory);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_PLUNAME, m_strSearchText);
	DDV_MaxChars(pDX, m_strSearchText, MAX_LENGTH_SEARCH_TEXT);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPluFilterDlgEcrman, CDialog)
	//{{AFX_MSG_MAP(CPluFilterDlgEcrman)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDoubleClickList)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST, OnGetdispinfoList)
	ON_EN_KILLFOCUS(IDC_EDIT_DEPTNO, OnKillFocusDeptNo)
	ON_CBN_SELCHANGE(IDC_COMBO_DEPTNAME, OnSelectDepartment)
	ON_CBN_SELCHANGE(IDC_COMBO_CATEGORY, OnSelectCategory)
	ON_EN_KILLFOCUS(IDC_EDIT_PLUNAME, OnSelectSearchText)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPluFilterDlgEcrman::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_listPlu.SubclassDlgItem ( IDC_LIST, this );
	m_listPlu.InsertColumn ( 0, "Plu No", LVCFMT_LEFT, 110 );
	m_listPlu.InsertColumn ( 1, "Description", LVCFMT_LEFT, 205 );
	m_listPlu.InsertColumn ( 2, "Department", LVCFMT_LEFT, 120 );
	m_listPlu.InsertColumn ( 3, "Price 1", LVCFMT_RIGHT, 80 );
	
	m_strTitle = "Browse Plu Database";	
	
	m_editDeptNo.LimitText ( MAX_LENGTH_DEPTNO );
	m_DepartmentSelector.FillDepartmentComboDatabase( m_nDepartmentFilter, DEPARTMENT_SELECTOR_NORMAL );

	CAnalysisCategoryCSVArray::PrepareFilterCombo(m_comboCategory);
	m_comboCategory.SelectUsingItemData(m_PluFilterArray.GetAnalysisCategoryFilter());

	RunSearch( TRUE, TRUE );
	return TRUE;  
}

/**********************************************************************/

void CPluFilterDlgEcrman::OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;
				
	int nIndex = m_listPlu.GetCurSel();
	
	if ( nIndex >= 0 && nIndex < m_listPlu.GetItemCount() )
	{
		m_nPluIdx = m_PluFilterArray.GetPluItemIndexAt ( nIndex );	
		EndDialog ( IDOK );
	}	
}

/**********************************************************************/

void CPluFilterDlgEcrman::RunSearch( bool bDoSearch, bool bUpdateDisplay )
{
	if ( bDoSearch == TRUE )
	{
		OnKillFocusDeptNo();
		m_PluFilterArray.SetDescriptionFilter ( m_strSearchText, SEARCH_ALLTEXT, FALSE );
		m_PluFilterArray.SetDepartmentFilter ( m_DepartmentSelector.GetDepartmentFilter() );
		m_PluFilterArray.SetAnalysisCategoryFilter(CAnalysisCategoryCSVArray::GetFilterFromCombo(m_comboCategory));
		m_PluFilterArray.BuildRecordList();
	}

	if ( bUpdateDisplay == TRUE )
	{
		m_listPlu.SetItemCountEx ( m_PluFilterArray.GetSize() );
		m_listPlu.Invalidate();
		m_listPlu.SetCurSel(0);
		UpdateTitle();
	}
}

/**********************************************************************/

void CPluFilterDlgEcrman::UpdateTitle()
{
	CString strTitle;
	strTitle.Format ( "%s ( %d of %d records )",
		( const char* ) m_strTitle,
		m_PluFilterArray.GetSize(),
		DataManager.Plu.GetSize() );

	SetWindowText ( strTitle );
}

/**********************************************************************/

void CPluFilterDlgEcrman::OnGetdispinfoList(NMHDR* pNMHDR, LRESULT* pResult) 
{	
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	if (( pDispInfo->item.mask & LVIF_TEXT ) != 0 )
	{
		int nIndex = pDispInfo->item.iItem;
		CCSV csv ( m_PluFilterArray.GetDisplayLine ( nIndex ), '\t' );
		lstrcpy ( pDispInfo->item.pszText, csv.GetString ( pDispInfo->item.iSubItem ) );
	}
	*pResult = 0;
}

/**********************************************************************/

void CPluFilterDlgEcrman::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		m_nPluIdx = -1;

		int nIndex = m_listPlu.GetCurSel();
	
		if ( nIndex >= 0 && nIndex < m_listPlu.GetItemCount() )
			m_nPluIdx = m_PluFilterArray.GetPluItemIndexAt ( nIndex );
	
		EndDialog ( IDOK );
	}
}

/**********************************************************************/

void CPluFilterDlgEcrman::OnKillFocusDeptNo() 
{
	int nOldDepartmentFilter = m_nDepartmentFilter;
	m_DepartmentSelector.SelectDepartmentFromEditBox();
	m_nDepartmentFilter = m_DepartmentSelector.GetDepartmentFilter();

	if ( m_nDepartmentFilter != nOldDepartmentFilter )
		RunSearch( TRUE, TRUE );
}

/**********************************************************************/

void CPluFilterDlgEcrman::OnSelectDepartment() 
{
	int nOldDepartmentFilter = m_nDepartmentFilter;
	m_DepartmentSelector.SelectDepartmentFromCombo();
	m_nDepartmentFilter = m_DepartmentSelector.GetDepartmentFilter();

	if ( m_nDepartmentFilter != nOldDepartmentFilter )
		RunSearch( TRUE, TRUE );
}

/**********************************************************************/

void CPluFilterDlgEcrman::OnSelectCategory()
{
	int nOldCategoryFilter = m_nCategoryFilter;
	m_nCategoryFilter = CAnalysisCategoryCSVArray::GetFilterFromCombo(m_comboCategory);

	if (m_nCategoryFilter != nOldCategoryFilter)
	{
		RunSearch(TRUE, TRUE);
	}
}

/**********************************************************************/

void CPluFilterDlgEcrman::OnSelectSearchText() 
{
	CString strSearchTextOld = m_strSearchText;

	if ( UpdateData ( TRUE ) == TRUE )
	{
		::TrimSpaces ( m_strSearchText, FALSE );
		if ( m_strSearchText != strSearchTextOld )
		{
			RunSearch ( TRUE, TRUE );
			UpdateData ( FALSE );
		}
	}
}

/**********************************************************************/

