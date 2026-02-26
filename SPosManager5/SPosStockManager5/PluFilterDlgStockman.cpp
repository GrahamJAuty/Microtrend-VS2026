/**********************************************************************/
#include "DepartmentCSVArray.h"
#include "DepartmentSetCSVArray.h"
#include "MaxLengths.h"
#include "SearchTypes.h"
#include "StockOptionsIni.h"
/**********************************************************************/
#include "PluFilterDlgStockman.h"
/**********************************************************************/

CPluFilterDlgStockman::CPluFilterDlgStockman( CPluFilterArray& FilterArray, CWnd* pParent )
	: CDialog(CPluFilterDlgStockman::IDD, pParent), m_PluFilterArray ( FilterArray ), m_DepartmentSelector ( m_editDeptNo, m_comboDeptName )
{
	//{{AFX_DATA_INIT(CPluFilterDlgStockman)
	m_strStockCode = _T("");
	//}}AFX_DATA_INIT
	m_nPluIdx = -1;
	m_strTitle = "";
	m_strSearchText = m_PluFilterArray.GetDescription();
	m_strStockCode = m_PluFilterArray.GetStockmanStockCodeFilter();
	m_nDepartmentFilter = m_PluFilterArray.GetDepartmentFilter();
	m_nCategoryFilter = m_PluFilterArray.GetAnalysisCategoryFilter();

	if ( StockOptions.GetPluBrowseStockCodesFlag() == FALSE )
		m_strStockCode = "";
}

/**********************************************************************/

void CPluFilterDlgStockman::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPluFilterDlgStockman)
	DDX_Control(pDX, IDC_STATIC_STOCKCODE, m_staticStockCode);
	DDX_Control(pDX, IDC_EDIT_STOCKCODE, m_editStockCode);
	DDX_Control(pDX, IDC_EDIT_DEPTNO, m_editDeptNo);
	DDX_Control(pDX, IDC_COMBO_DEPTNAME, m_comboDeptName);
	DDX_Control(pDX, IDC_COMBO_CATEGORY, m_comboCategory);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_PLUNAME, m_strSearchText);
	DDV_MaxChars(pDX, m_strSearchText, MAX_LENGTH_SEARCH_TEXT);
	DDX_Text(pDX, IDC_EDIT_STOCKCODE, m_strStockCode);
	DDV_MaxChars(pDX, m_strStockCode, Super.MaxStockLen());
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPluFilterDlgStockman, CDialog)
	//{{AFX_MSG_MAP(CPluFilterDlgStockman)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDoubleClickList)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST, OnGetdispinfoList)
	ON_EN_KILLFOCUS(IDC_EDIT_DEPTNO, OnKillFocusDeptNo)
	ON_CBN_SELCHANGE(IDC_COMBO_DEPTNAME, OnSelectDepartment)
	ON_CBN_SELCHANGE(IDC_COMBO_CATEGORY, OnSelectCategory)
	ON_EN_KILLFOCUS(IDC_EDIT_STOCKCODE, OnSelectStockCode)
	ON_EN_KILLFOCUS(IDC_EDIT_PLUNAME, OnSelectSearchText)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPluFilterDlgStockman::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_listPlu.SubclassDlgItem ( IDC_LIST, this );

	if ( StockOptions.GetPluBrowseStockCodesFlag() == FALSE )
	{
		m_listPlu.InsertColumn ( 0, "Plu No", LVCFMT_LEFT, 115 );
		m_listPlu.InsertColumn ( 1, "Description", LVCFMT_LEFT, 200 );
		m_listPlu.InsertColumn ( 2, "Department", LVCFMT_LEFT, 120 );
		m_listPlu.InsertColumn ( 3, "Price 1", LVCFMT_RIGHT, 80 );
		m_staticStockCode.ShowWindow( SW_HIDE );
		m_editStockCode.ShowWindow( SW_HIDE );
	}
	else
	{
		m_listPlu.InsertColumn ( 0, "Plu No", LVCFMT_LEFT, 125 );
		m_listPlu.InsertColumn ( 1, "Description", LVCFMT_LEFT, 200 );
		m_listPlu.InsertColumn ( 2, "Stock Code", LVCFMT_LEFT, 130 );
		m_listPlu.InsertColumn ( 3, "Dept", LVCFMT_LEFT, 60 );
	}
	
	m_strTitle = "Browse Plu Database";
	
	m_editDeptNo.LimitText ( MAX_LENGTH_DEPTNO );
	m_DepartmentSelector.FillDepartmentComboDatabase( m_nDepartmentFilter, DEPARTMENT_SELECTOR_NORMAL );
	
	CAnalysisCategoryCSVArray::PrepareFilterCombo(m_comboCategory);
	m_comboCategory.SelectUsingItemData(m_PluFilterArray.GetAnalysisCategoryFilter());

	RunSearch ( TRUE, TRUE );
	return TRUE;  
}

/**********************************************************************/

void CPluFilterDlgStockman::OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult) 
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

void CPluFilterDlgStockman::RunSearch( bool bDoSearch, bool bUpdateDisplay )
{
	if ( bDoSearch == TRUE )
	{
		OnKillFocusDeptNo();
		m_PluFilterArray.SetDescriptionFilter ( m_strSearchText, SEARCH_ALLTEXT, FALSE );
		m_PluFilterArray.SetDepartmentFilter ( m_DepartmentSelector.GetDepartmentFilter() );
		m_PluFilterArray.SetAnalysisCategoryFilter(CAnalysisCategoryCSVArray::GetFilterFromCombo(m_comboCategory));
		m_PluFilterArray.SetStockmanStockCodeFilter ( m_strStockCode );
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

void CPluFilterDlgStockman::UpdateTitle()
{
	CString strTitle;
	strTitle.Format ( "%s ( %d of %d records )",
		( const char* ) m_strTitle,
		m_PluFilterArray.GetSize(),
		DataManager.Plu.GetSize() );

	SetWindowText ( strTitle );
}

/**********************************************************************/

void CPluFilterDlgStockman::OnGetdispinfoList(NMHDR* pNMHDR, LRESULT* pResult) 
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

void CPluFilterDlgStockman::OnOK() 
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

void CPluFilterDlgStockman::OnKillFocusDeptNo() 
{
	int nOldDepartmentFilter = m_nDepartmentFilter;
	m_DepartmentSelector.SelectDepartmentFromEditBox();
	m_nDepartmentFilter = m_DepartmentSelector.GetDepartmentFilter();

	if ( m_nDepartmentFilter != nOldDepartmentFilter )
		RunSearch( TRUE, TRUE );
}

/**********************************************************************/

void CPluFilterDlgStockman::OnSelectDepartment() 
{
	int nOldDepartmentFilter = m_nDepartmentFilter;
	m_DepartmentSelector.SelectDepartmentFromCombo();
	m_nDepartmentFilter = m_DepartmentSelector.GetDepartmentFilter();

	if ( m_nDepartmentFilter != nOldDepartmentFilter )
		RunSearch( TRUE, TRUE );
}

/**********************************************************************/

void CPluFilterDlgStockman::OnSelectCategory()
{
	int nOldCategoryFilter = m_nCategoryFilter;
	m_nCategoryFilter = CAnalysisCategoryCSVArray::GetFilterFromCombo(m_comboCategory);

	if (m_nCategoryFilter != nOldCategoryFilter)
	{
		RunSearch(TRUE, TRUE);
	}
}

/**********************************************************************/

void CPluFilterDlgStockman::OnSelectStockCode() 
{
	CString strStockCodeOld = m_strStockCode;

	if ( UpdateData ( TRUE ) == TRUE )
	{
		::TrimSpaces ( m_strStockCode, FALSE );
		
		if ( m_strStockCode != strStockCodeOld )
		{
			RunSearch ( TRUE, TRUE );
			UpdateData ( FALSE );
		}
	}
}

/**********************************************************************/

void CPluFilterDlgStockman::OnSelectSearchText() 
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
