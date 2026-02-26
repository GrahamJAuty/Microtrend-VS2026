/**********************************************************************/
//#include "EcrmanOptionsIni.h"
#include "MaxLengths.h"
#include "BarcodePluNoTable.h"
#include "ReportPluStockLink.h"
/**********************************************************************/
#include "PluPrtStockLinkDlg.h"
/**********************************************************************/

CPluPrtStockLinkDlg::CPluPrtStockLinkDlg( CPluFilterArray& FilterArray, CWnd* pParent /*=NULL*/)
	: CSSDialog(CPluPrtStockLinkDlg::IDD, pParent), m_FilterArray ( FilterArray ), m_DepartmentSelector ( m_editDeptNo, m_comboDeptName )
{
	//{{AFX_DATA_INIT(CPluPrtStockLinkDlg)
	m_bLinked = FALSE;
	m_bRecipe = FALSE;
	m_bUnlinked = FALSE;
	m_bSeparate = FALSE;
	m_bZeroPriced = FALSE;
	m_bProfit = FALSE;
	//}}AFX_DATA_INIT
	m_nDepartmentFilter = m_FilterArray.GetDepartmentFilter();
	m_bSortByDept = FALSE;
	SetDefaults();
}

/**********************************************************************/

void CPluPrtStockLinkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPluPrtStockLinkDlg)
	DDX_Control(pDX, IDC_CHECK_SEPARATE, m_checkSeparate);
	DDX_Control(pDX, IDC_EDIT_DEPTNO, m_editDeptNo);
	DDX_Control(pDX, IDC_CHECK_SORT_BY_DEPT, m_checkDeptSort);
	DDX_Control(pDX, IDC_COMBO_DEPTNAME, m_comboDeptName);
	DDX_Check(pDX, IDC_CHECK_SORT_BY_DEPT, m_bSortByDept);
	DDX_Check(pDX, IDC_CHECK_LINKED, m_bLinked);
	DDX_Check(pDX, IDC_CHECK_RECIPE, m_bRecipe);
	DDX_Check(pDX, IDC_CHECK_UNLINKED, m_bUnlinked);
	DDX_Check(pDX, IDC_CHECK_SEPARATE, m_bSeparate);
	DDX_Check(pDX, IDC_CHECK_ZEROPRICED, m_bZeroPriced);
	DDX_Check(pDX, IDC_CHECK_PROFIT, m_bProfit);
	//}}AFX_DATA_MAP
	DDX_IntegerString(pDX, IDC_EDIT_PLU_RANGE_START, m_strPluFrom);
	DDV_IntegerString(pDX, m_strPluFrom, "1", Super.PluEnd() );
	DDX_IntegerString(pDX, IDC_EDIT_PLU_RANGE_END, m_strPluTo);
	DDV_IntegerString(pDX, m_strPluTo, "1", Super.PluEnd() );
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPluPrtStockLinkDlg, CDialog)
	//{{AFX_MSG_MAP(CPluPrtStockLinkDlg)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	ON_CBN_SELCHANGE(IDC_COMBO_DEPTNAME, OnSelchangeComboDeptname)
	ON_EN_KILLFOCUS(IDC_EDIT_DEPTNO, OnKillFocusEditDeptNumber)
	ON_EN_KILLFOCUS(IDC_EDIT_PLU_RANGE_END, OnKillFocusPluNo)
	ON_BN_CLICKED(IDC_BUTTON_DEFAULT, OnButtonDefault)
	ON_BN_CLICKED(IDC_STATIC_RANGE, OnStaticRange)
	ON_EN_KILLFOCUS(IDC_EDIT_PLU_RANGE_START, OnKillFocusPluNo)
	ON_BN_CLICKED(IDC_CHECK_RECIPE, OnToggleRecipeProfit)
	ON_BN_CLICKED(IDC_CHECK_PROFIT, OnToggleRecipeProfit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/
CEdit* CPluPrtStockLinkDlg::GetEditPluFrom() { return GetEdit( IDC_EDIT_PLU_RANGE_START ); }
CEdit* CPluPrtStockLinkDlg::GetEditPluTo() { return GetEdit( IDC_EDIT_PLU_RANGE_END ); }
/**********************************************************************/

BOOL CPluPrtStockLinkDlg::OnInitDialog() 
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

	CFilenameUpdater FnUp( SysFiles::PluLinkDef );
	if ( filePriceList.Open ( FnUp.GetFilenameToUse(), "rb" ) == TRUE )
	{
		if ( filePriceList.ReadString ( strBuffer ) == TRUE )
		{
			bGotFile = TRUE;
			CCSVPluPrtStockLinkKey csv ( strBuffer );
			m_bUnlinked = csv.PrintUnlinked();
			m_bLinked = csv.PrintLinked();
			m_bRecipe = csv.PrintRecipe();
			m_bProfit = csv.PrintProfit();
			m_bZeroPriced = csv.PrintZeroPriced();
			m_bSeparate = csv.PrintSeparate();

			if ( ( m_bRecipe != 0 ) && ( m_bProfit != 0 ) )
				m_bSeparate = 1;
			
			UpdateData ( FALSE );
			OnKillFocusEditDeptNumber();
		}
	}
		
	if ( bGotFile == FALSE )
		OnButtonDefault();

	return TRUE;  
}

/**********************************************************************/

void CPluPrtStockLinkDlg::SetDefaults()
{
	m_bUnlinked = FALSE;
	m_bLinked = TRUE;
	m_bRecipe = TRUE;
	m_bZeroPriced = FALSE;
	m_bSeparate = FALSE;
}

/**********************************************************************/

void CPluPrtStockLinkDlg::OnButtonDefault() 
{
	SetDefaults();
	UpdateData ( FALSE );
}

/**********************************************************************/

void CPluPrtStockLinkDlg::OnButtonSave() 
{
	UpdateData( TRUE, FALSE );

	CSSFile filePriceList;
	CFilenameUpdater FnUp( SysFiles::PluLinkDef );
	if ( filePriceList.Open ( FnUp.GetFilenameToUse(), "wb" ) == TRUE )
		filePriceList.WriteLine ( GetCSVLine() );
}

/**********************************************************************/

const char* CPluPrtStockLinkDlg::GetCSVLine()
{
	CCSV csv;	
	csv.Add ( (int) m_bUnlinked );
	csv.Add ( (int) m_bLinked );
	csv.Add ( (int) m_bRecipe );
	csv.Add ( 0 );	//FIDDLE - WAS INHIBITED
	csv.Add ( (int) m_bZeroPriced );
	csv.Add ( (int) m_bSeparate );
	csv.Add ( (int) m_bProfit );
	m_strFieldSelect = csv.GetLine();
	return m_strFieldSelect;
}

/**********************************************************************/

void CPluPrtStockLinkDlg::OnKillFocusEditDeptNumber() 
{
	m_DepartmentSelector.SelectDepartmentFromEditBox();
}

/**********************************************************************/

void CPluPrtStockLinkDlg::OnSelchangeComboDeptname() 
{
	m_DepartmentSelector.SelectDepartmentFromCombo();
}

/**********************************************************************/

void CPluPrtStockLinkDlg::OnKillFocusPluNo() 
{
	UpdateData( TRUE, FALSE );
	m_strPluFrom = BarcodePluNoTable.ConvertPlu ( m_strPluFrom );
	m_strPluTo = BarcodePluNoTable.ConvertPlu ( m_strPluTo );
	UpdateData ( FALSE );
}

/**********************************************************************/

void CPluPrtStockLinkDlg::OnStaticRange() 
{
	CString strStart = "1";
	::AddLeading ( strStart, Super.MaxPluLen(), '0' );
	GetEditPluFrom() -> SetWindowText ( strStart );
	
	CString strEnd = "9";
	::AddLeading ( strEnd, Super.MaxPluLen(), '9' );
	GetEditPluTo() -> SetWindowText( strEnd );
}

/**********************************************************************/

void CPluPrtStockLinkDlg::OnToggleRecipeProfit() 
{
	UpdateData( TRUE, FALSE );

	if ( ( m_bProfit != 0 ) && ( m_bRecipe != 0 ) )
	{
		m_checkSeparate.SetCheck( TRUE );
		m_checkSeparate.EnableWindow( FALSE );
	}
	else
		m_checkSeparate.EnableWindow( TRUE );
}

/**********************************************************************/

void CPluPrtStockLinkDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		OnKillFocusEditDeptNumber();
		OnKillFocusPluNo();

		if ( m_bUnlinked == FALSE && m_bLinked == FALSE && m_bRecipe == FALSE )
		{
			Prompter.Error ( "You must select at least one type of stock\nlink before you can print this report." );
			return;
		}

		if ( ( m_bRecipe != 0 ) && ( m_bProfit != 0 ) )
			m_bSeparate = 1;

		CReportPluStockLink StockLinkReport ( m_FilterArray, GetCSVLine() );
		StockLinkReport.SetPluFrom ( m_strPluFrom );
		StockLinkReport.SetPluTo ( m_strPluTo );
		StockLinkReport.SetDepartmentFilter ( m_DepartmentSelector.GetDepartmentFilter() );
		StockLinkReport.SetSortByDeptFlag ( m_bSortByDept != 0 );
		StockLinkReport.CreateReport();
	}
}

/**********************************************************************/



