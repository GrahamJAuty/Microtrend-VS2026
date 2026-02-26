/**********************************************************************/
#include "MaxLengths.h"
#include "MessageLogger.h"
#include "BarcodePluNoTable.h"
#include "StatusProgress.h"
#include "StockOptionsIni.h"
#include "..\CommonEcrStock\SimpleInfoDlg.h"
#include "SysInfo.h"
/**********************************************************************/
#include "PluRangeStockDlg.h"
/**********************************************************************/

CPluRangeStockDlg::CPluRangeStockDlg( CPluFilterArray& FilterArray, CWnd* pParent )
	: CSSDialog(CPluRangeStockDlg::IDD, pParent), m_FilterArray ( FilterArray ),
	m_DepartmentSelector ( m_editDeptNo, m_comboDeptName )
{
	//{{AFX_DATA_INIT(CPluRangeStockDlg)
	m_bCreateStock = FALSE;
	m_bDepartment = FALSE;
	m_bDescription = FALSE;
	m_bPluNumbers = TRUE;
	//}}AFX_DATA_INIT
}

/**********************************************************************/

void CPluRangeStockDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPluRangeStockDlg)
	DDX_Control(pDX, IDC_EDIT_DEPTNO, m_editDeptNo);
	DDX_Control(pDX, IDC_CHECK_CREATESTOCK, m_checkCreateStock);
	DDX_Control(pDX, IDC_CHECK_PLUNUMBERS, m_checkPluNumbers);
	DDX_Control(pDX, IDC_COMBO_DEPTNAME, m_comboDeptName);
	DDX_Control(pDX, IDC_COMBO_CATEGORY, m_comboCategory);
	DDX_Check(pDX, IDC_CHECK_CREATESTOCK, m_bCreateStock);
	DDX_Check(pDX, IDC_CHECK_DEPARTMENT, m_bDepartment);
	DDX_Check(pDX, IDC_CHECK_DESCRIPTION, m_bDescription);
	DDX_Check(pDX, IDC_CHECK_PLUNUMBERS, m_bPluNumbers);
	//}}AFX_DATA_MAP
	DDX_IntegerString(pDX, IDC_EDIT_PLU_RANGE_START, m_strPluFrom);
	DDV_IntegerString(pDX, m_strPluFrom, "1", Super.PluEnd() );
	DDX_IntegerString(pDX, IDC_EDIT_PLU_RANGE_END, m_strPluTo);
	DDV_IntegerString(pDX, m_strPluTo, "1", Super.PluEnd() );
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPluRangeStockDlg, CDialog)
	//{{AFX_MSG_MAP(CPluRangeStockDlg)
	ON_CBN_SELCHANGE(IDC_COMBO_DEPTNAME, OnSelchangeComboDeptname)
	ON_EN_KILLFOCUS(IDC_EDIT_DEPTNO, OnKillFocusDeptNumber)
	ON_EN_KILLFOCUS(IDC_EDIT_PLU_RANGE_END, OnKillFocusPluNo)
	ON_BN_CLICKED(IDC_CHECK_CREATESTOCK, OnToggleCreateStock)
	ON_EN_KILLFOCUS(IDC_EDIT_PLU_RANGE_START, OnKillFocusPluNo)
	ON_BN_CLICKED(IDC_STATIC_RANGE, OnStaticRange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/
CEdit* CPluRangeStockDlg::GetEditPluFrom() { return GetEdit( IDC_EDIT_PLU_RANGE_START ); }
CEdit* CPluRangeStockDlg::GetEditPluTo() { return GetEdit( IDC_EDIT_PLU_RANGE_END ); }
/**********************************************************************/

BOOL CPluRangeStockDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	SubclassEdit ( IDC_EDIT_PLU_RANGE_START, SS_NUM, Super.MaxPluLen(), "%s" );
	SubclassEdit ( IDC_EDIT_PLU_RANGE_END, SS_NUM, Super.MaxPluLen(), "%s" );
	
	m_editDeptNo.LimitText ( MAX_LENGTH_DEPTNO );
	m_DepartmentSelector.FillDepartmentComboDatabase( m_FilterArray.GetDepartmentFilter(), DEPARTMENT_SELECTOR_NORMAL );

	if ( m_FilterArray.GetDepartmentFilter() != DEPARTMENTSET_ALL )
	{
		m_comboDeptName.EnableWindow ( FALSE );
		m_editDeptNo.EnableWindow ( FALSE );
	}

	CAnalysisCategoryCSVArray::PrepareFilterCombo(m_comboCategory);

	m_checkPluNumbers.EnableWindow ( FALSE );
	return TRUE;  
}

/**********************************************************************/

void CPluRangeStockDlg::OnKillFocusDeptNumber() 
{
	m_DepartmentSelector.SelectDepartmentFromEditBox();
}

/**********************************************************************/

void CPluRangeStockDlg::OnSelchangeComboDeptname() 
{
	m_DepartmentSelector.SelectDepartmentFromCombo();
}

/**********************************************************************/

void CPluRangeStockDlg::OnKillFocusPluNo() 
{
	UpdateData( TRUE, FALSE );
	m_strPluFrom = BarcodePluNoTable.ConvertPlu ( m_strPluFrom );
	m_strPluTo = BarcodePluNoTable.ConvertPlu ( m_strPluTo );
	UpdateData ( FALSE );
}

/**********************************************************************/

void CPluRangeStockDlg::UpdateStock( int& nStockCreated, int& nStockModified )
{
	nStockCreated = 0;
	nStockModified = 0;

	int nStartIdx, nRange;
	__int64 nPluFrom = _atoi64( m_strPluFrom );
	__int64 nPluTo = _atoi64( m_strPluTo );
	m_FilterArray.GetArrayIdxRange( nPluFrom, nPluTo, nStartIdx, nRange );
		
	int nNextStockCode = 0;

	//REMEMBER THE ITEMS WE HAVE ALREADY CHANGED
	CConsolidateArray ConsolArrayStock;

	CDepartmentSetCSVRecord DepartmentSet;
	DepartmentSet.CreateDepartmentSet ( m_DepartmentSelector.GetDepartmentFilter() );

	int nCategory = CAnalysisCategoryCSVArray::GetFilterFromCombo(m_comboCategory);

	for ( int nOffset = 0; nOffset < nRange; nOffset++ )
	{
		StatusProgress.SetPos ( nOffset + 1, nRange ); 

		CPluCSVRecord PluRecord;
		int nPluIdx = m_FilterArray.GetPluItemIndexAt ( nStartIdx + nOffset );
		DataManager.Plu.GetAt ( nPluIdx, PluRecord );

			__int64 nPluNo = PluRecord.GetPluNoInt();
		if ( ( nPluNo < nPluFrom ) || ( nPluNo > nPluTo ) )
			continue;

		if ( DepartmentSet.GetDepartmentStatus ( PluRecord.GetBaseDeptNo() ) == FALSE )
			continue;

		if ((nCategory != 9999) && (nCategory != PluRecord.GetBaseAnalysisCategory()))
			continue;

		if ( DataManager.Plu.IsRecipe( PluRecord.GetPluNoInt() ) )
			continue;

		CString strStockCode;
		DataManager.Plu.GetPluStockCode( PluRecord.GetPluNoInt(), strStockCode, FALSE );
		
		if ( strStockCode == "" && m_bCreateStock == TRUE )
		{
			if ( m_bPluNumbers == TRUE )
			{
				strStockCode = PluRecord.GetPluNoString();

				CRecipeCSVRecord NewRecipe;
				NewRecipe.SetRecipePluNo( PluRecord.GetPluNoString() );
				NewRecipe.AddStockCode( PluRecord.GetPluNoString(), 1.0 );
				DataManager.Plu.SetRecipe( nPluIdx, NewRecipe );
			}
			else
			{
				bool bResult;

				do
				{
					nNextStockCode++;
					strStockCode.Format ( "STK%5.5d", nNextStockCode );
					int nStockIdx;
					bResult = DataManager.Stock.FindStockItemByCode ( strStockCode, nStockIdx );
				}
				while ( ( bResult == TRUE ) && nNextStockCode <= 99999 );

				if ( nNextStockCode <= 99999 )
				{
					CRecipeCSVRecord NewRecipe;
					NewRecipe.SetRecipePluNo( PluRecord.GetPluNoString() );
					NewRecipe.AddStockCode( strStockCode, 1.0 );
					DataManager.Plu.SetRecipe( -1, NewRecipe );
				}
			}
		}

		if ( strStockCode == "" )
			continue;

		int nStockIdx;
		bool bFoundStock = DataManager.Stock.FindStockItemByCode ( strStockCode, nStockIdx );
				
		if ( bFoundStock == FALSE && m_bCreateStock == FALSE )
			continue;
	
		if ( bFoundStock == FALSE )
		{
			CStockCSVRecord StockRecord;
			StockRecord.SetStockCode ( strStockCode );
			StockRecord.SetDescription ( PluRecord.GetReportText() );
			StockRecord.SetCategory ( PluRecord.GetBaseDeptNo() );
			DataManager.Stock.InsertRecord ( StockRecord, nStockIdx );
			ConsolArrayStock.Add ( strStockCode );
			nStockCreated++;
		}
		else
		{
			CStockCSVRecord StockRecord;
			DataManager.Stock.GetAt ( nStockIdx, StockRecord );

			if ( StockRecord.GetDisablePluRangeFlag() == FALSE )
			{
				int nSize = ConsolArrayStock.GetSize();
				ConsolArrayStock.Add ( StockRecord.GetStockCode() );

				if ( nSize != ConsolArrayStock.GetSize() )
				{
					if ( m_bDescription == TRUE )
						StockRecord.SetDescription ( PluRecord.GetReportText() );

					if ( m_bDepartment == TRUE )
						StockRecord.SetCategory ( PluRecord.GetBaseDeptNo() );
					
					if ( m_bDescription == TRUE || m_bDepartment == TRUE )
						nStockModified++;
					
					DataManager.Stock.SetAt ( nStockIdx, StockRecord );
				}
			}
		}
	}
}

/**********************************************************************/

void CPluRangeStockDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == FALSE )
		return;

	OnKillFocusDeptNumber();
	OnKillFocusPluNo();
	
	int nStockCreated = 0;
	int nStockModified = 0;

	if ( m_bCreateStock == TRUE || m_bDescription == TRUE || m_bDepartment == TRUE )
	{
		StatusProgress.Lock( TRUE, "Updating stock items" );
		UpdateStock ( nStockCreated, nStockModified );
		StatusProgress.Unlock();
	}

	CSimpleInfoDlg dlg ( "Update Linked Stock Items", "New stock items created", "Existing stock items modified", nStockCreated, nStockModified, this );
	dlg.DoModal();

	CDWordArray dw;
	dw.Add( nStockCreated );
	dw.Add( nStockModified );
	MessageLogger.LogSystem( 2, 11, FALSE, dw );

	EndDialog ( IDOK );
}

/**********************************************************************/

void CPluRangeStockDlg::OnStaticRange() 
{
	CString strStart = "1";
	::AddLeading ( strStart, Super.MaxPluLen(), '0' );
	GetEditPluFrom() -> SetWindowText ( strStart );
	
	CString strEnd = "9";
	::AddLeading ( strEnd, Super.MaxPluLen(), '9' );
	GetEditPluTo() -> SetWindowText( strEnd );
}

/**********************************************************************/

void CPluRangeStockDlg::OnToggleCreateStock() 
{
	m_checkPluNumbers.EnableWindow ( m_checkCreateStock.GetCheck() );	
}

/**********************************************************************/







