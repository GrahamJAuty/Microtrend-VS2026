/**********************************************************************/
#include "MaxLengths.h"
#include "BarcodePluNoTable.h"
#include "StatusProgress.h"
#include "SysInfo.h"
/**********************************************************************/
#include "PluRangeStockLevelsDlg.h"
/**********************************************************************/

CPluRangeStockLevelsDlg::CPluRangeStockLevelsDlg( CPluFilterArray& FilterArray, CWnd* pParent )
	: CSSDialog(CPluRangeStockLevelsDlg::IDD, pParent), m_FilterArray ( FilterArray ),
	m_DepartmentSelector ( m_editDeptNo, m_comboDeptName )
{
	//{{AFX_DATA_INIT(CPluRangeStockLevelsDlg)
	//}}AFX_DATA_INIT
}

/**********************************************************************/

void CPluRangeStockLevelsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPluRangeStockLevelsDlg)
	DDX_Control(pDX, IDC_EDIT_DEPTNO, m_editDeptNo);
	DDX_Control(pDX, IDC_COMBO_DEPTNAME, m_comboDeptName);
	DDX_Control(pDX, IDC_COMBO_CATEGORY, m_comboCategory);
	//}}AFX_DATA_MAP
	DDX_IntegerString(pDX, IDC_EDIT_PLU_RANGE_START, m_strPluFrom);
	DDV_IntegerString(pDX, m_strPluFrom, "1", Super.PluEnd() );
	DDX_IntegerString(pDX, IDC_EDIT_PLU_RANGE_END, m_strPluTo);
	DDV_IntegerString(pDX, m_strPluTo, "1", Super.PluEnd() );
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPluRangeStockLevelsDlg, CDialog)
	//{{AFX_MSG_MAP(CPluRangeStockLevelsDlg)
	ON_CBN_SELCHANGE(IDC_COMBO_DEPTNAME, OnSelchangeComboDeptname)
	ON_EN_KILLFOCUS(IDC_EDIT_DEPTNO, OnKillFocusDeptNumber)
	ON_EN_KILLFOCUS(IDC_EDIT_PLU_RANGE_END, OnKillFocusPluNo)
	ON_EN_KILLFOCUS(IDC_EDIT_PLU_RANGE_START, OnKillFocusPluNo)
	ON_BN_CLICKED(IDC_STATIC_RANGE, OnStaticRange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/
CEdit* CPluRangeStockLevelsDlg::GetEditPluFrom() { return GetEdit( IDC_EDIT_PLU_RANGE_START ); }
CEdit* CPluRangeStockLevelsDlg::GetEditPluTo() { return GetEdit( IDC_EDIT_PLU_RANGE_END ); }
/**********************************************************************/

BOOL CPluRangeStockLevelsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	SubclassEdit ( IDC_EDIT_PLU_RANGE_START, SS_NUM, Super.MaxPluLen(), "%s" );
	SubclassEdit ( IDC_EDIT_PLU_RANGE_END, SS_NUM, Super.MaxPluLen(), "%s" );
	
	m_editDeptNo.LimitText ( MAX_LENGTH_DEPTNO );
	m_DepartmentSelector.FillDepartmentComboDatabase( m_FilterArray.GetDepartmentFilter(), DEPARTMENT_SELECTOR_NORMAL );
	
	if ( m_FilterArray.GetDepartmentFilter() != DEPARTMENTSET_ALL )
	{
		m_editDeptNo.EnableWindow ( FALSE );
		m_comboDeptName.EnableWindow ( FALSE );
	}

	CAnalysisCategoryCSVArray::PrepareFilterCombo(m_comboCategory);
	
	return TRUE;  
}

/**********************************************************************/

void CPluRangeStockLevelsDlg::OnKillFocusDeptNumber() 
{
	m_DepartmentSelector.SelectDepartmentFromEditBox();
}

/**********************************************************************/

void CPluRangeStockLevelsDlg::OnSelchangeComboDeptname() 
{
	m_DepartmentSelector.SelectDepartmentFromCombo();
}

/**********************************************************************/

void CPluRangeStockLevelsDlg::OnKillFocusPluNo() 
{
	UpdateData( TRUE, FALSE );
	m_strPluFrom = BarcodePluNoTable.ConvertPlu ( m_strPluFrom );
	m_strPluTo = BarcodePluNoTable.ConvertPlu ( m_strPluTo );
	UpdateData ( FALSE );
}

/**********************************************************************/

void CPluRangeStockLevelsDlg::OnStaticRange() 
{
	CString strStart = "1";
	::AddLeading ( strStart, Super.MaxPluLen(), '0' );
	GetEditPluFrom() -> SetWindowText ( strStart );
	
	CString strEnd = "9";
	::AddLeading ( strEnd, Super.MaxPluLen(), '9' );
	GetEditPluTo() -> SetWindowText( strEnd );
}

/**********************************************************************/

void CPluRangeStockLevelsDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == FALSE )
		return;

	OnKillFocusDeptNumber();
	OnKillFocusPluNo();

	CStringArray arrayWarning;
	arrayWarning.Add ( "Clear Apparent Stock Levels" );
	arrayWarning.Add ( "Apparent stock for the selected plu items will be reset to zero." );
	
	if ( Prompter.Warning( arrayWarning ) != IDYES )
		return;

	int nStartIdx, nRange;
	__int64 nPluFrom = _atoi64( m_strPluFrom );
	__int64 nPluTo = _atoi64( m_strPluTo );
	m_FilterArray.GetArrayIdxRange( nPluFrom, nPluTo, nStartIdx, nRange );
	
	CDepartmentSetCSVRecord DepartmentSet;
	DepartmentSet.CreateDepartmentSet ( m_DepartmentSelector.GetDepartmentFilter() );

	int nCategory = CAnalysisCategoryCSVArray::GetFilterFromCombo(m_comboCategory);

	StatusProgress.Lock( TRUE, "Clearing PLU stock levels" );

	int nCount = 0;
	for ( int nOffset = 0; nOffset < nRange; nOffset++ )
	{
		StatusProgress.SetPos ( nOffset, nRange ); 
		CPluCSVRecord Record;
		int nPluIdx = m_FilterArray.GetPluItemIndexAt ( nStartIdx + nOffset );
		DataManager.Plu.GetAt ( nPluIdx, Record );

		__int64 nPluNo = Record.GetPluNoInt();
		if ( ( nPluNo < nPluFrom ) || ( nPluNo > nPluTo ) )
			continue;

		//skip plus with non matching departments
		if ( DepartmentSet.GetDepartmentStatus ( Record.GetBaseDeptNo() ) == FALSE )
			continue;

		if ((nCategory != 9999) && (nCategory != Record.GetBaseAnalysisCategory()))
			continue;

		Record.SetApparentLevel(0.0);
		DataManager.Plu.SavePluRecord ( nPluIdx, Record );
		nCount++;
	}	

	StatusProgress.Unlock();

	CString strBuf;
	switch ( nCount )
	{
	case 0:
		Prompter.Info ( "No records were changed" );
		break;

	case 1:
		Prompter.Info ( "1 record was changed" );
		break;

	default:
		strBuf.Format ( "%d records were changed", nCount );
		Prompter.Info ( strBuf );
		break;
	}
		
	EndDialog ( IDOK );
}

/**********************************************************************/







