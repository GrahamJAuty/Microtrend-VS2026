/**********************************************************************/
#include "DepartmentCSVArray.h"
#include "DepartmentSetCSVArray.h"
#include "MaxLengths.h"
#include "NodeTypes.h"
#include "SearchTypes.h"
/**********************************************************************/
#include "PluMiniBrowseDlg.h"
/**********************************************************************/

CPluMiniBrowseDlg::CPluMiniBrowseDlg( int nNodeType, CPluFilterArray& FilterArray, CWnd* pParent )
	: CDialog(CPluMiniBrowseDlg::IDD, pParent), m_PluFilterArray ( FilterArray ), m_DepartmentSelector ( m_editDeptNo, m_comboDeptName )
{
	//{{AFX_DATA_INIT(CPluMiniBrowseDlg)
	//}}AFX_DATA_INIT
	m_nNodeType = nNodeType;
	m_nPluIdx = -1;
	m_strSearchText = m_PluFilterArray.GetDescription();
	m_nDepartmentFilter = m_PluFilterArray.GetDepartmentFilter();
	m_pMessageTarget = pParent;
}

/**********************************************************************/

void CPluMiniBrowseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPluMiniBrowseDlg)
	DDX_Control(pDX, IDC_EDIT_DEPTNO, m_editDeptNo);
	DDX_Control(pDX, IDC_COMBO_DEPTNAME, m_comboDeptName);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_PLUNAME, m_strSearchText);
	DDV_MaxChars(pDX, m_strSearchText, MAX_LENGTH_SEARCH_TEXT);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPluMiniBrowseDlg, CDialog)
	//{{AFX_MSG_MAP(CPluMiniBrowseDlg)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDoubleClickList)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST, OnGetdispinfoList)
	ON_EN_KILLFOCUS(IDC_EDIT_DEPTNO, OnKillFocusDeptNo)
	ON_CBN_SELCHANGE(IDC_COMBO_DEPTNAME, OnSelectDepartment)
	ON_EN_KILLFOCUS(IDC_EDIT_PLUNAME, OnSelectSearchText)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPluMiniBrowseDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	switch( m_nNodeType )
	{
	case NODE_PLU:
		SetWindowText( "Link Products to Plu Item" );
		break;

	case NODE_PLUFILTER_NAME:
		SetWindowText( "Add Products to Plu Filter" );
		break;

	case NODE_OFFER_GROUP:
	default:
		SetWindowText( "Add Products to Offer Group" );
		break;
	}

	m_listPlu.SubclassDlgItem ( IDC_LIST, this );
	m_listPlu.InsertColumn ( 0, "Plu No", LVCFMT_LEFT, 90 );
	m_listPlu.InsertColumn ( 1, "Description", LVCFMT_LEFT, 205 );
		
	m_editDeptNo.LimitText ( MAX_LENGTH_DEPTNO );
	m_DepartmentSelector.FillDepartmentComboDatabase( m_nDepartmentFilter, DEPARTMENT_SELECTOR_NORMAL );	

	RunSearch( TRUE, TRUE );
	return TRUE;  
}

/**********************************************************************/

void CPluMiniBrowseDlg::OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;
				
	int nIndex = m_listPlu.GetCurSel();
	
	if ( nIndex >= 0 && nIndex < m_listPlu.GetItemCount() )
	{
		m_nPluIdx = m_PluFilterArray.GetPluItemIndexAt ( nIndex );	
		OnOK();
	}	
}

/**********************************************************************/

void CPluMiniBrowseDlg::RunSearch( bool bDoSearch, bool bUpdateDisplay )
{
	if ( bDoSearch == TRUE )
	{
		OnKillFocusDeptNo();
		m_PluFilterArray.SetDescriptionFilter ( m_strSearchText, SEARCH_ALLTEXT, FALSE );
		m_PluFilterArray.SetDepartmentFilter ( m_DepartmentSelector.GetDepartmentFilter() );
		m_PluFilterArray.BuildRecordList();
	}

	if ( bUpdateDisplay == TRUE )
	{
		m_listPlu.SetItemCountEx ( m_PluFilterArray.GetSize() );
		m_listPlu.Invalidate();
		m_listPlu.SetCurSel(0);
	}
}

/**********************************************************************/

void CPluMiniBrowseDlg::OnGetdispinfoList(NMHDR* pNMHDR, LRESULT* pResult) 
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

void CPluMiniBrowseDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		m_nPluIdx = -1;

		int nIndex = m_listPlu.GetCurSel();
	
		if ( nIndex >= 0 && nIndex < m_listPlu.GetItemCount() )
			m_nPluIdx = m_PluFilterArray.GetPluItemIndexAt ( nIndex );
	
		if ( m_pMessageTarget != NULL )
			m_pMessageTarget -> PostMessage ( WM_APP, ADDPLU_MESSAGE, (long) this );
	}
}

/**********************************************************************/

void CPluMiniBrowseDlg::OnKillFocusDeptNo() 
{
	int nOldDepartmentFilter = m_nDepartmentFilter;
	m_DepartmentSelector.SelectDepartmentFromEditBox();
	m_nDepartmentFilter = m_DepartmentSelector.GetDepartmentFilter();

	if ( m_nDepartmentFilter != nOldDepartmentFilter )
		RunSearch( TRUE, TRUE );
}

/**********************************************************************/

void CPluMiniBrowseDlg::OnSelectDepartment() 
{
	int nOldDepartmentFilter = m_nDepartmentFilter;
	m_DepartmentSelector.SelectDepartmentFromCombo();
	m_nDepartmentFilter = m_DepartmentSelector.GetDepartmentFilter();

	if ( m_nDepartmentFilter != nOldDepartmentFilter )
		RunSearch( TRUE, TRUE );
}

/**********************************************************************/

void CPluMiniBrowseDlg::OnSelectSearchText() 
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

