/**********************************************************************/
#include "MaxLengths.h"
#include "MessageLogger.h"
#include "ListDataSupplierDlg.h"
#include "StockOptionsIni.h"
/**********************************************************************/
#include "PluRangeAddDlgStockman.h"
/**********************************************************************/

CPluRangeAddDlgStockman::CPluRangeAddDlgStockman( CPluAddOrCopyInfo& PluInfo, CWnd* pParent )
	: CSSDialog(CPluRangeAddDlgStockman::IDD, pParent),
	m_DepartmentSelector ( m_editDeptNo, m_comboDeptName ),
	m_PluInfo( PluInfo )
{
	//{{AFX_DATA_INIT(CPluRangeAddDlgStockman)
	//}}AFX_DATA_INIT
	m_PluInfo.m_bNewPluCreateStock = FALSE;
	m_PluInfo.m_strNewPluNo = "";
	m_PluInfo.m_lNewPluCopyCount = 1;
}

/**********************************************************************/

void CPluRangeAddDlgStockman::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPluRangeAddDlgStockman)
	DDX_Control(pDX, IDC_EDIT_DEPTNO, m_editDeptNo);
	DDX_Control(pDX, IDC_COMBO_DEPTNAME, m_comboDeptName);
	DDX_Control(pDX, IDC_BUTTON_SUPPLIER, m_buttonSupplier);
	DDX_Control(pDX, IDC_COMBO_SUPPLIER, m_comboSupplier);
	DDX_Control(pDX, IDC_CHECK_CREATE, m_checkCreate);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_RANGE, m_PluInfo.m_lNewPluCopyCount);
	DDV_MinMaxInt ( pDX, m_PluInfo.m_lNewPluCopyCount, 1, 1000 );
	DDX_IntegerString(pDX, IDC_EDIT_START, m_PluInfo.m_strNewPluNo );
	DDV_IntegerString(pDX, m_PluInfo.m_strNewPluNo, "1", Super.PluEnd() );
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPluRangeAddDlgStockman, CDialog)
	//{{AFX_MSG_MAP(CPluRangeAddDlgStockman)
	ON_CBN_SELCHANGE(IDC_COMBO_DEPTNAME, OnSelectDept)
	ON_EN_KILLFOCUS(IDC_EDIT_DEPTNO, OnKillFocusDeptNo)
	ON_BN_CLICKED(IDC_CHECK_CREATE, OnToggleCreateStock)
	ON_BN_CLICKED(IDC_BUTTON_SUPPLIER, OnButtonSupplier)
	ON_CBN_SELCHANGE(IDC_COMBO_SUPPLIER, OnSelectSupplier)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPluRangeAddDlgStockman::OnInitDialog() 
{
	CDialog::OnInitDialog();
	SubclassEdit ( IDC_EDIT_START, SS_NUM, Super.MaxPluLen(), "%s" );
	SubclassEdit ( IDC_EDIT_RANGE, SS_NUM, 4, "%d" );	

	m_editDeptNo.LimitText ( MAX_LENGTH_DEPTNO );
	m_DepartmentSelector.FillDepartmentComboDatabase( m_PluInfo.m_nNewPluDeptNo, DEPARTMENT_SELECTOR_SINGLE );
	
	FillSupplierCombo();

	bool bCreate = StockOptions.GetStockAutoCreateFlag();
	m_checkCreate.SetCheck ( StockOptions.GetStockAutoCreateFlag() );
	m_comboSupplier.EnableWindow( bCreate );
	m_buttonSupplier.EnableWindow( bCreate );

	return TRUE;  
}

/**********************************************************************/

void CPluRangeAddDlgStockman::OnKillFocusDeptNo() 
{
	m_DepartmentSelector.SelectDepartmentFromEditBox();
	m_PluInfo.m_nNewPluDeptNo = m_DepartmentSelector.GetDepartmentFilter();
}

/**********************************************************************/

void CPluRangeAddDlgStockman::OnSelectDept() 
{
	m_DepartmentSelector.SelectDepartmentFromCombo();
	m_PluInfo.m_nNewPluDeptNo = m_DepartmentSelector.GetDepartmentFilter();
}

/**********************************************************************/

void CPluRangeAddDlgStockman::OnToggleCreateStock() 
{
	m_comboSupplier.EnableWindow( m_checkCreate.GetCheck() != 0 );
	m_buttonSupplier.EnableWindow( m_checkCreate.GetCheck() != 0 );
}

/**********************************************************************/

void CPluRangeAddDlgStockman::FillSupplierCombo()
{
	m_comboSupplier.ResetContent();

	for ( int nIndex = 0; nIndex < DataManager.Supplier.GetSize(); nIndex++ )
	{
		int nInsertionPoint = m_comboSupplier.AddString ( DataManager.Supplier.GetName ( nIndex ) );
		m_comboSupplier.SetItemData ( nInsertionPoint, DataManager.Supplier.GetSuppNo ( nIndex  ) );
	}
	
	m_comboSupplier.InsertString ( 0, "Unspecified" );
	m_comboSupplier.SetItemData ( 0, 0 );
	
	bool bFoundSupplier = FALSE;

	for ( int nIndex = 0; nIndex < m_comboSupplier.GetCount(); nIndex++ )
	{
		if ( m_comboSupplier.GetItemData ( nIndex ) == ( WORD ) m_PluInfo.m_nNewPluSuppNo )
		{
			m_comboSupplier.SetCurSel ( nIndex );
			bFoundSupplier = TRUE;
			break;
		}
	}

	if ( bFoundSupplier == FALSE )
		m_comboSupplier.SetCurSel ( 0 );

	int nSelection = m_comboSupplier.GetCurSel();
	m_PluInfo.m_nNewPluSuppNo = m_comboSupplier.GetItemData(nSelection);
}

/**********************************************************************/

void CPluRangeAddDlgStockman::OnButtonSupplier() 
{
	if ( DataManager.Supplier.IsValid() == FALSE )
	{
		Prompter.SupplierError();
		return;
	}

	CDataManagerInfo info;
	if ( DataManager.OpenSupplierBuffer( info ) == FALSE )
	{
		Prompter.ShareError( info );
		return;
	}

	//READING FROM THE FILE MAY CHANGE THE SUPPLIER LIST
	FillSupplierCombo();

	UpdateData( TRUE, FALSE );
	
	CListDataSupplierDlg dlg ( m_PluInfo.m_nNewPluSuppNo, this );
	
	bool bSavedSuppliers = FALSE;

	if ( dlg.DoModal() == IDOK )
	{
		if ( DataManager.WriteSupplierBuffer( info ) == TRUE )
			bSavedSuppliers = TRUE;
		else
			Prompter.WriteError( info );
	}

	DataManager.CloseSupplierBuffer( info );

	if ( bSavedSuppliers == TRUE )		
	{
		if ( dlg.GetListSelection() >= 0 )
			m_PluInfo.m_nNewPluSuppNo = DataManager.Supplier.GetSuppNo ( dlg.GetListSelection() );

		FillSupplierCombo();
		UpdateData ( FALSE );
		
		int nSelection = m_comboSupplier.GetCurSel();
		
		if ( nSelection >= 0 )
			m_PluInfo.m_nNewPluSuppNo = m_comboSupplier.GetItemData ( nSelection );	
		else
			m_PluInfo.m_nNewPluSuppNo = 0;
	}
}

/**********************************************************************/

void CPluRangeAddDlgStockman::OnSelectSupplier() 
{
	int nSelection = m_comboSupplier.GetCurSel();
	m_PluInfo.m_nNewPluSuppNo = m_comboSupplier.GetItemData( nSelection );	
}

/**********************************************************************/

void CPluRangeAddDlgStockman::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		m_PluInfo.m_bNewPluCreateStock = ( m_checkCreate.GetCheck() != 0 );
		EndDialog ( IDOK );
	}
}

/**********************************************************************/


