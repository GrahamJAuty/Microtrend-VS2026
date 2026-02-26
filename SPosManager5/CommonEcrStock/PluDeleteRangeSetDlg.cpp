/**********************************************************************/
#include "DepartmentSetCSVArray.h"
#include "MaxLengths.h"
#include "PluCSVArray.h"
#include "BarcodePluNoTable.h"
/**********************************************************************/
#include "PluDeleteRangeSetDlg.h"
/**********************************************************************/

CPluDeleteRangeSetDlg::CPluDeleteRangeSetDlg( CWnd* pParent )
	: CSSDialog(CPluDeleteRangeSetDlg::IDD, pParent),
	m_DepartmentSelector ( m_editDeptNo, m_comboDeptName )
{
	//{{AFX_DATA_INIT(CPluDeleteRangeSetDlg)
	//}}AFX_DATA_INIT
	m_nDelete = 0;
	m_nDepartmentFilter = DEPARTMENTSET_ALL;
}

/**********************************************************************/

void CPluDeleteRangeSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPluDeleteRangeSetDlg)
	DDX_Control(pDX, IDC_EDIT_DEPTNO, m_editDeptNo);
	DDX_Control(pDX, IDC_COMBO_DEPTNAME, m_comboDeptName);
	DDX_Radio(pDX, IDC_RADIO_YES, m_nDelete);
	//}}AFX_DATA_MAP
	DDX_IntegerString(pDX, IDC_EDIT_PLU_RANGE_START, m_strPluFrom);
	DDV_IntegerString(pDX, m_strPluFrom, "1", Super.PluEnd() ); 
	DDX_IntegerString(pDX, IDC_EDIT_PLU_RANGE_END, m_strPluTo);
	DDV_IntegerString(pDX, m_strPluTo, "1", Super.PluEnd() );
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPluDeleteRangeSetDlg, CDialog)
	//{{AFX_MSG_MAP(CPluDeleteRangeSetDlg)
	ON_CBN_SELCHANGE(IDC_COMBO_DEPTNAME, OnSelchangeComboDeptname)
	ON_EN_KILLFOCUS(IDC_EDIT_DEPTNO, OnKillFocusDeptNumber)
	ON_EN_KILLFOCUS(IDC_EDIT_PLU_RANGE_END, OnKillFocusPluNo)
	ON_EN_KILLFOCUS(IDC_EDIT_PLU_RANGE_START, OnKillFocusPluNo)
	ON_BN_CLICKED(IDC_STATIC_RANGE, OnStaticRange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/
CEdit* CPluDeleteRangeSetDlg::GetEditPluFrom() { return GetEdit ( IDC_EDIT_PLU_RANGE_START ); }
CEdit* CPluDeleteRangeSetDlg::GetEditPluTo() { return GetEdit ( IDC_EDIT_PLU_RANGE_END ); }
/**********************************************************************/

BOOL CPluDeleteRangeSetDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	SubclassEdit ( IDC_EDIT_PLU_RANGE_START, SS_NUM, Super.MaxPluLen(), "%s" );
	SubclassEdit ( IDC_EDIT_PLU_RANGE_END, SS_NUM, Super.MaxPluLen(), "%s" );
	
	m_editDeptNo.LimitText ( MAX_LENGTH_DEPTNO );
	m_DepartmentSelector.FillDepartmentComboDatabase( DEPARTMENTSET_ALL, DEPARTMENT_SELECTOR_NORMAL );
		
	return TRUE;  
}

/**********************************************************************/

void CPluDeleteRangeSetDlg::OnKillFocusDeptNumber() 
{
	m_DepartmentSelector.SelectDepartmentFromEditBox();
}

/**********************************************************************/

void CPluDeleteRangeSetDlg::OnSelchangeComboDeptname() 
{
	m_DepartmentSelector.SelectDepartmentFromCombo();
}

/**********************************************************************/

void CPluDeleteRangeSetDlg::OnKillFocusPluNo() 
{
	UpdateData( TRUE, FALSE );
	m_strPluFrom = BarcodePluNoTable.ConvertPlu ( m_strPluFrom );
	m_strPluTo = BarcodePluNoTable.ConvertPlu ( m_strPluTo );
	UpdateData ( FALSE );
}

/**********************************************************************/

void CPluDeleteRangeSetDlg::OnStaticRange() 
{
	CString strStart = "1";
	::AddLeading ( strStart, Super.MaxPluLen(), '0' );
	GetEditPluFrom() -> SetWindowText ( strStart );
	
	CString strEnd = "9";
	::AddLeading ( strEnd, Super.MaxPluLen(), '9' );
	GetEditPluTo() -> SetWindowText( strEnd );
}

/**********************************************************************/

void CPluDeleteRangeSetDlg::OnOK() 
{
	if ( UpdateData( TRUE ) == TRUE )
	{
		OnKillFocusDeptNumber();
		m_nDepartmentFilter = m_DepartmentSelector.GetDepartmentFilter();
		EndDialog( IDOK );
	}
}

/**********************************************************************/








