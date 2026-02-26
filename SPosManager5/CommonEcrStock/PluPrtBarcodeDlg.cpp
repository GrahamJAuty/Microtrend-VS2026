/**********************************************************************/
//#include "EcrmanOptionsIni.h"
#include "MaxLengths.h"
#include "BarcodePluNoTable.h"
#include "ReportPluBarcode.h"
/**********************************************************************/
#include "PluPrtBarcodeDlg.h"
/**********************************************************************/

CPluPrtBarcodeDlg::CPluPrtBarcodeDlg( CPluFilterArray& FilterArray, CWnd* pParent /*=NULL*/)
	: CSSDialog(CPluPrtBarcodeDlg::IDD, pParent), m_FilterArray ( FilterArray ), m_DepartmentSelector ( m_editDeptNo, m_comboDeptName )
{
	//{{AFX_DATA_INIT(CPluPrtBarcodeDlg)
	m_bSeparate = FALSE;
	m_bZeroPriced = FALSE;
	m_bDirect = FALSE;
	//}}AFX_DATA_INIT
	m_nDepartmentFilter = m_FilterArray.GetDepartmentFilter();
	m_bSortByDept = FALSE;
	SetDefaults();
}

/**********************************************************************/

void CPluPrtBarcodeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPluPrtBarcodeDlg)
	DDX_Control(pDX, IDC_EDIT_DEPTNO, m_editDeptNo);
	DDX_Control(pDX, IDC_CHECK_SORT_BY_DEPT, m_checkDeptSort);
	DDX_Control(pDX, IDC_COMBO_DEPTNAME, m_comboDeptName);
	DDX_Check(pDX, IDC_CHECK_SORT_BY_DEPT, m_bSortByDept);
	DDX_Check(pDX, IDC_CHECK_SEPARATE, m_bSeparate);
	DDX_Check(pDX, IDC_CHECK_ZEROPRICED, m_bZeroPriced);
	DDX_Check(pDX, IDC_CHECK_DIRECT, m_bDirect);
	//}}AFX_DATA_MAP
	DDX_IntegerString(pDX, IDC_EDIT_PLU_RANGE_START, m_strPluFrom);
	DDV_IntegerString(pDX, m_strPluFrom, "1", Super.PluEnd() );
	DDX_IntegerString(pDX, IDC_EDIT_PLU_RANGE_END, m_strPluTo);
	DDV_IntegerString(pDX, m_strPluTo, "1", Super.PluEnd() );
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPluPrtBarcodeDlg, CDialog)
	//{{AFX_MSG_MAP(CPluPrtBarcodeDlg)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	ON_CBN_SELCHANGE(IDC_COMBO_DEPTNAME, OnSelchangeComboDeptname)
	ON_EN_KILLFOCUS(IDC_EDIT_DEPTNO, OnKillFocusEditDeptNumber)
	ON_EN_KILLFOCUS(IDC_EDIT_PLU_RANGE_END, OnKillFocusPluNo)
	ON_BN_CLICKED(IDC_BUTTON_DEFAULT, OnButtonDefault)
	ON_EN_KILLFOCUS(IDC_EDIT_PLU_RANGE_START, OnKillFocusPluNo)
	ON_BN_CLICKED(IDC_STATIC_RANGE, OnStaticRange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/
CEdit* CPluPrtBarcodeDlg::GetEditPluFrom() { return GetEdit( IDC_EDIT_PLU_RANGE_START ); }
CEdit* CPluPrtBarcodeDlg::GetEditPluTo() { return GetEdit( IDC_EDIT_PLU_RANGE_END ); }
/**********************************************************************/

BOOL CPluPrtBarcodeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	SubclassEdit ( IDC_EDIT_PLU_RANGE_START, SS_NUM, Super.MaxPluLen(), "%s" );
	SubclassEdit ( IDC_EDIT_PLU_RANGE_END, SS_NUM, Super.MaxPluLen(), "%s" );
	
	m_editDeptNo.LimitText ( MAX_LENGTH_DEPTNO );
	m_DepartmentSelector.FillDepartmentComboDatabase( m_nDepartmentFilter, DEPARTMENT_SELECTOR_NORMAL );

	if ( m_nDepartmentFilter != DEPARTMENTSET_ALL )
	{
		m_editDeptNo.EnableWindow ( FALSE );
		m_comboDeptName.EnableWindow ( FALSE );
	}
	
	CString strBuffer;
	bool bGotFile = FALSE;
	
	CSSFile filePriceList;

	CFilenameUpdater FnUp( SysFiles::BarLinkCustomDef );

	if ( filePriceList.Open ( FnUp.GetFilenameToUse(), "rb" ) == TRUE )
	{
		if ( filePriceList.ReadString ( strBuffer ) == TRUE )
		{
			bGotFile = TRUE;
			CCSVPluPrtBarcodeKey csv ( strBuffer );
			m_bZeroPriced = csv.PrintZeroPriced();
			m_bSeparate = csv.PrintSeparate();
			m_bDirect = csv.PrintDirect();
			
			UpdateData ( FALSE );
			OnKillFocusEditDeptNumber();
		}
	}
		
	if ( bGotFile == FALSE )
		OnButtonDefault();

	return TRUE;  
}

/**********************************************************************/

void CPluPrtBarcodeDlg::SetDefaults()
{
	m_bZeroPriced = FALSE;
	m_bSeparate = FALSE;
	m_bDirect = FALSE;
}

/**********************************************************************/

void CPluPrtBarcodeDlg::OnButtonDefault() 
{
	SetDefaults();
	UpdateData ( FALSE );
}

/**********************************************************************/

void CPluPrtBarcodeDlg::OnButtonSave() 
{
	UpdateData( TRUE, FALSE );

	CSSFile filePriceList;
	CFilenameUpdater FnUp( SysFiles::BarLinkCustomDef );
	if ( filePriceList.Open ( FnUp.GetFilenameToUse(), "wb" ) == TRUE )
		filePriceList.WriteLine ( GetCSVLine() );
}

/**********************************************************************/

const char* CPluPrtBarcodeDlg::GetCSVLine()
{
	CCSV csv;	
	csv.Add ( 0 );		//FIDDLE - WAS INHIBITED
	csv.Add ( (int) m_bZeroPriced );
	csv.Add ( (int) m_bSeparate );
	csv.Add ( (int) m_bDirect );
	m_strFieldSelect = csv.GetLine();
	return m_strFieldSelect;
}

/**********************************************************************/

void CPluPrtBarcodeDlg::OnKillFocusEditDeptNumber() 
{
	m_DepartmentSelector.SelectDepartmentFromEditBox();
}

/**********************************************************************/

void CPluPrtBarcodeDlg::OnSelchangeComboDeptname() 
{
	m_DepartmentSelector.SelectDepartmentFromCombo();
}

/**********************************************************************/

void CPluPrtBarcodeDlg::OnKillFocusPluNo() 
{
	UpdateData( TRUE, FALSE );
	m_strPluFrom = BarcodePluNoTable.ConvertPlu ( m_strPluFrom );
	m_strPluTo = BarcodePluNoTable.ConvertPlu ( m_strPluTo );
	UpdateData ( FALSE );
}

/**********************************************************************/

void CPluPrtBarcodeDlg::OnStaticRange() 
{
	CString strStart = "1";
	::AddLeading ( strStart, Super.MaxPluLen(), '0' );
	GetEditPluFrom() -> SetWindowText ( strStart );
	
	CString strEnd = "9";
	::AddLeading ( strEnd, Super.MaxPluLen(), '9' );
	GetEditPluTo() -> SetWindowText( strEnd );
}

/**********************************************************************/

void CPluPrtBarcodeDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		OnKillFocusEditDeptNumber();
		OnKillFocusPluNo();
		CReportPluBarcode BarcodeReport ( m_FilterArray, GetCSVLine() );
		BarcodeReport.SetPluFrom ( _atoi64( m_strPluFrom ) );
		BarcodeReport.SetPluTo ( _atoi64 (m_strPluTo ) );
		BarcodeReport.SetDepartmentFilter ( m_DepartmentSelector.GetDepartmentFilter() );
		BarcodeReport.SetSortByDeptFlag ( m_bSortByDept != 0 );
		BarcodeReport.SetDirectFlag( m_bDirect != 0 );
		BarcodeReport.CreateReport();
	}
}

/**********************************************************************/
