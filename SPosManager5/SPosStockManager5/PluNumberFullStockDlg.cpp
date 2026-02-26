/**********************************************************************/
#include "BarcodePluNoTable.h"
#include "FilterTypes.h"
#include "ListDataSupplierDlg.h"
#include "MaxLengths.h"
#include "StockFilterDlg.h"
#include "StockOptionsIni.h"
 /**********************************************************************/
#include "PluNumberFullStockDlg.h"
/**********************************************************************/

CPluNumberFullStockDlg::CPluNumberFullStockDlg( bool bFullEdit, CPluAddOrCopyInfo& PluInfo, CWnd* pParent )
	: CSSDialog("IDD_PLU_NUMBER_FULLSTOCK", pParent),
	m_DepartmentSelector ( m_editDeptNo, m_comboDeptName ),
	m_PluInfo( PluInfo )
{
	//{{AFX_DATA_INIT(CPluNumberFullStockDlg)
	//}}AFX_DATA_INIT
	m_PluInfo.m_strNewPluStockCode = "";
	m_bFullEdit = bFullEdit;
}

/**********************************************************************/

void CPluNumberFullStockDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPluNumberFullStockDlg)
	DDX_Control(pDX, IDC2_STATIC_PLUNUMBER, m_staticPluNumber);
	DDX_Control(pDX, IDC2_STATIC_DEPARTMENT, m_staticDepartment);
	DDX_Control(pDX, IDC2_EDIT_DEPTNO, m_editDeptNo);
	DDX_Control(pDX, IDC2_COMBO_DEPTNAME, m_comboDeptName);
	DDX_Control(pDX, IDC2_STATIC_STOCK, m_staticStock);
	DDX_Control(pDX, IDC2_EDIT_STOCKCODE, m_editStockCode);
	DDX_Control(pDX, IDC2_BUTTON_BROWSE, m_buttonBrowse);
	DDX_Control(pDX, IDC2_STATIC_SUPPLIER, m_staticSupplier);
	DDX_Control(pDX, IDC2_COMBO_SUPPLIER, m_comboSupplier);
	DDX_Control(pDX, IDC2_BUTTON_SUPPLIER, m_buttonSupplier);
	DDX_Control(pDX, IDOK, m_buttonOK);
	DDX_Control(pDX, IDCANCEL, m_buttonCancel);
	//}}AFX_DATA_MAP
	DDX_IntegerString(pDX, IDC2_EDIT_PLUNO, m_PluInfo.m_strNewPluNo);
	DDV_IntegerString(pDX, m_PluInfo.m_strNewPluNo, "1", Super.PluEnd() );
	DDX_Text(pDX, IDC2_EDIT_STOCKCODE, m_PluInfo.m_strNewPluStockCode);
	DDV_MaxChars(pDX, m_PluInfo.m_strNewPluStockCode, Stock::StockCode.Max);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPluNumberFullStockDlg, CDialog)
	//{{AFX_MSG_MAP(CPluNumberFullStockDlg)
	ON_EN_KILLFOCUS(IDC2_EDIT_PLUNO, OnKillFocusPluNo)
	ON_CBN_SELCHANGE(IDC2_COMBO_DEPTNAME, OnSelectDept)
	ON_EN_KILLFOCUS(IDC2_EDIT_DEPTNO, OnKillFocusDeptNo)
	ON_EN_KILLFOCUS(IDC2_EDIT_STOCKCODE, OnKillFocusStockCode)
	ON_BN_CLICKED(IDC2_BUTTON_BROWSE, OnButtonBrowse)
	ON_BN_CLICKED(IDC2_BUTTON_SUPPLIER, OnButtonSupplier)
	ON_CBN_SELCHANGE(IDC2_COMBO_SUPPLIER, OnSelectSupplier)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/
CEdit* CPluNumberFullStockDlg::GetEditPluNumber() { return GetEdit( IDC2_EDIT_PLUNO ); }
/**********************************************************************/

BOOL CPluNumberFullStockDlg::OnInitDialog() 
{
	UpdateStockCode();
	CDialog::OnInitDialog();
	SubclassEdit ( IDC2_EDIT_PLUNO, SS_NUM, Super.MaxPluLen(), "%s" );
	GetEditPluNumber() -> EnableWindow ( m_bFullEdit );

	if ( FALSE == m_bFullEdit )
	{
		ShowAndEnableWindow( &m_staticSupplier, FALSE );
		ShowAndEnableWindow( &m_comboSupplier, FALSE );
		ShowAndEnableWindow( &m_buttonSupplier, FALSE );
		ResizeControl( &m_staticStock, 289,58 );
		MoveControl( &m_buttonOK, 310, 116 );
		MoveControl( &m_buttonCancel, 310, 132 );
		ResizeDialog( 367, 155 );
	}
	else
		FillSupplierCombo();

	m_editDeptNo.LimitText ( MAX_LENGTH_DEPTNO );
	m_DepartmentSelector.FillDepartmentComboDatabase( m_PluInfo.m_nNewPluDeptNo, DEPARTMENT_SELECTOR_SINGLE );
	
	return TRUE;  
}

/**********************************************************************/

void CPluNumberFullStockDlg::OnKillFocusPluNo() 
{
	UpdateData( TRUE, FALSE );
	UpdateStockCode();
	UpdateData( FALSE );
	UpdateSupplierEnables();
}

/**********************************************************************/

void CPluNumberFullStockDlg::OnKillFocusDeptNo() 
{
	m_DepartmentSelector.SelectDepartmentFromEditBox();
	m_PluInfo.m_nNewPluDeptNo = m_DepartmentSelector.GetDepartmentFilter();
}

/**********************************************************************/

void CPluNumberFullStockDlg::OnSelectDept() 
{
	m_DepartmentSelector.SelectDepartmentFromCombo();
	m_PluInfo.m_nNewPluDeptNo = m_DepartmentSelector.GetDepartmentFilter();
}

/**********************************************************************/

void CPluNumberFullStockDlg::OnKillFocusStockCode() 
{
	UpdateData( TRUE, FALSE );
	m_PluInfo.m_strNewPluStockCode = BarcodePluNoTable.ConvertStockCode ( m_PluInfo.m_strNewPluStockCode );
	UpdateData( FALSE );
	UpdateSupplierEnables();
}

/**********************************************************************/

void CPluNumberFullStockDlg::UpdateStockCode()
{
	m_PluInfo.m_strNewPluStockCode = "";
	if ( StockOptions.GetStockAutoCreateFlag() )
		m_PluInfo.m_strNewPluStockCode = BarcodePluNoTable.ConvertStockCode ( m_PluInfo.m_strNewPluNo );	
}

/**********************************************************************/

void CPluNumberFullStockDlg::OnButtonBrowse() 
{
	CStockFilterArray FilterArray( FILTERTYPE_BROWSE_STOCK );
	CStockFilterDlg dlgBrowse ( FilterArray, this );
		
	if ( dlgBrowse.DoModal() != IDOK )
		return;

	int nStockIdx = dlgBrowse.GetStockIdx();
	if ( nStockIdx < 0 )
		return;

	UpdateData( TRUE, FALSE );
	m_PluInfo.m_strNewPluStockCode = DataManager.Stock.GetStockCode ( nStockIdx );
	UpdateData( FALSE );

	UpdateSupplierEnables();
	
	m_editStockCode.SetSel( 0, -1 );
	m_editStockCode.SetFocus();
}

/**********************************************************************/

void CPluNumberFullStockDlg::FillSupplierCombo()
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

void CPluNumberFullStockDlg::OnButtonSupplier() 
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

	CListDataSupplierDlg dlg ( m_PluInfo.m_nNewPluSuppNo, this );
	
	if ( dlg.DoModal() == IDOK )
	{
		if ( DataManager.WriteSupplierBuffer( info ) == FALSE )
			Prompter.WriteError( info );
		else
		{
			if ( dlg.GetListSelection() >= 0 )
				m_PluInfo.m_nNewPluSuppNo = DataManager.Supplier.GetSuppNo ( dlg.GetListSelection() );
		}
	}

	DataManager.CloseSupplierBuffer( info );

	FillSupplierCombo();
	UpdateSupplierEnables();
		
	int nSelection = m_comboSupplier.GetCurSel();
		
	if ( nSelection >= 0 )
		m_PluInfo.m_nNewPluSuppNo = m_comboSupplier.GetItemData ( nSelection );	
	else
		m_PluInfo.m_nNewPluSuppNo = 0;
}

/**********************************************************************/

void CPluNumberFullStockDlg::OnSelectSupplier() 
{	
	int nSelection = m_comboSupplier.GetCurSel();
	m_PluInfo.m_nNewPluSuppNo = m_comboSupplier.GetItemData( nSelection );	
}

/**********************************************************************/

void CPluNumberFullStockDlg::UpdateSupplierEnables()
{
	bool bPreviousEnable = ( m_comboSupplier.IsWindowEnabled() != 0 );
	
	int nStockIdx;
	bool bEnable = m_bFullEdit;
	bEnable &= ( DataManager.Stock.FindStockItemByCode( m_PluInfo.m_strNewPluStockCode, nStockIdx ) == FALSE );
	
	m_comboSupplier.EnableWindow( bEnable );
	m_buttonSupplier.EnableWindow( bEnable );

	if ( bEnable == FALSE && bPreviousEnable == TRUE )
	{
		m_editStockCode.SetSel( 0, -1 );
		m_editStockCode.SetFocus();
	}
}

/**********************************************************************/

void CPluNumberFullStockDlg::OnOK() 
{
	if ( UpdateData(TRUE) == TRUE )
	{
		OnKillFocusDeptNo();
		::TrimSpaces ( m_PluInfo.m_strNewPluStockCode, FALSE );
		EndDialog(IDOK);
	}
}

/**********************************************************************/
