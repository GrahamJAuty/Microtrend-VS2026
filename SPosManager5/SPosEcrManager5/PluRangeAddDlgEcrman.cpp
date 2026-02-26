/**********************************************************************/
#include "..\SPosEcrManager5\ListDataEcrmanSupplierDlg.h"
#include "MaxLengths.h"
#include "PluRangeModes.h"
 /**********************************************************************/
#include "PluRangeAddDlgEcrman.h"
/**********************************************************************/

CPluRangeAddDlgEcrman::CPluRangeAddDlgEcrman( CPluAddOrCopyInfo& PluInfo, CWnd* pParent )
	: CSSDialog(CPluRangeAddDlgEcrman::IDD, pParent),
	m_DepartmentSelector ( m_editDeptNo, m_comboDeptName ),
	m_EcrmanSupplierSelector( m_editSupplier, m_comboSupplier, FALSE ),
	m_PluInfo( PluInfo )
{
	//{{AFX_DATA_INIT(CPluRangeAddDlgEcrman)
	//}}AFX_DATA_INIT
	m_PluInfo.m_strNewPluNo = "";
	m_PluInfo.m_lNewPluCopyCount = 1;
	m_bShowSupplier = EcrmanOptions.GetFeaturesPluStockFlag();
}

/**********************************************************************/

INT_PTR CPluRangeAddDlgEcrman::DoModalWrapper()
{
	if ( TRUE == m_bShowSupplier )
	{
		CDataManagerInfo info;
		DataManagerNonDb.OpenEcrmanSupplier( DB_READONLY, info );
	}

	INT_PTR result = DoModal();
	
	if ( TRUE == m_bShowSupplier )
	{
		CDataManagerInfo info;
		DataManagerNonDb.CloseEcrmanSupplier( info );
	}
	
	return result;
}

/**********************************************************************/

void CPluRangeAddDlgEcrman::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPluRangeAddDlgEcrman)
	DDX_Control(pDX, IDC_STATIC_PLU, m_staticPlu);
	DDX_Control(pDX, IDC_STATIC_PLUNUMBER, m_staticPluNumber);
	DDX_Control(pDX, IDC_STATIC_DEPARTMENT, m_staticDepartment);
	DDX_Control(pDX, IDC_EDIT_DEPTNO, m_editDeptNo);
	DDX_Control(pDX, IDC_COMBO_DEPTNAME, m_comboDeptName);
	DDX_Control(pDX, IDC_STATIC_RANGE, m_staticRange);
	DDX_Control(pDX, IDC_STATIC_STOCK, m_staticStock);
	DDX_Control(pDX, IDC_STATIC_SUPPLIER, m_staticSupplier);
	DDX_Control(pDX, IDC_EDIT_SUPPLIER, m_editSupplier);
	DDX_Control(pDX, IDC_COMBO_SUPPLIER, m_comboSupplier);
	DDX_Control(pDX, IDC_BUTTON_SUPPLIER, m_buttonSupplier);
	DDX_Control(pDX, IDOK, m_buttonOK);
	DDX_Control(pDX, IDCANCEL, m_buttonCancel);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_RANGE, m_PluInfo.m_lNewPluCopyCount);
	DDV_MinMaxInt ( pDX, m_PluInfo.m_lNewPluCopyCount, 1, 1000 );
	DDX_IntegerString(pDX, IDC_EDIT_PLUNO, m_PluInfo.m_strNewPluNo);
	DDV_IntegerString(pDX, m_PluInfo.m_strNewPluNo, "1", Super.PluEnd() );
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPluRangeAddDlgEcrman, CDialog)
	//{{AFX_MSG_MAP(CPluRangeAddDlgEcrman)
	ON_CBN_SELCHANGE(IDC_COMBO_DEPTNAME, OnSelectDept)
	ON_EN_KILLFOCUS(IDC_EDIT_DEPTNO, OnKillFocusDeptNo)
	ON_BN_CLICKED(IDC_BUTTON_SUPPLIER, OnButtonSupplier)
	ON_EN_KILLFOCUS(IDC_EDIT_SUPPLIER, OnKillFocusSupplier)
	ON_CBN_SELCHANGE(IDC_COMBO_SUPPLIER, OnSelectSupplier)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/
CEdit* CPluRangeAddDlgEcrman::GetEditPluNumber() { return GetEdit( IDC_EDIT_PLUNO ); }
CEdit* CPluRangeAddDlgEcrman::GetEditRange() { return GetEdit( IDC_EDIT_RANGE ); }
/**********************************************************************/

BOOL CPluRangeAddDlgEcrman::OnInitDialog() 
{
	CDialog::OnInitDialog();
	SubclassEdit ( IDC_EDIT_PLUNO, SS_NUM, Super.MaxPluLen(), "%s" );
	SubclassEdit ( IDC_EDIT_RANGE, SS_NUM, 4, "%d" );

	m_PluInfo.m_nPluRangeMode = PLURANGE_MODE_ADD;
	CString strTitle = "Add Plu Range";

	SetWindowText( strTitle );

	m_editDeptNo.LimitText ( MAX_LENGTH_DEPTNO );
	m_DepartmentSelector.FillDepartmentComboDatabase( m_PluInfo.m_nNewPluDeptNo, DEPARTMENT_SELECTOR_SINGLE );
	
	if ( TRUE == m_bShowSupplier )
	{
		m_editSupplier.LimitText(3);
		m_EcrmanSupplierSelector.FillEcrmanSupplierCombo( m_PluInfo.m_nNewPluSuppNo );
	}
	else
	{
		ShowAndEnableWindow( &m_staticPlu, FALSE );
		ShowAndEnableWindow( &m_staticStock, FALSE );
		ShowAndEnableWindow( &m_staticSupplier, FALSE );
		ShowAndEnableWindow( &m_editSupplier, FALSE );
		ShowAndEnableWindow( &m_comboSupplier, FALSE );
		ShowAndEnableWindow( &m_buttonSupplier, FALSE );
		MoveControl( &m_staticPluNumber, 20, 17 );
		MoveControl( GetEditPluNumber(), 95, 17 );
		MoveControl( &m_staticDepartment, 20, 37 );
		MoveControl( &m_editDeptNo, 95, 37 );
		MoveControl( &m_comboDeptName, 127, 37 );
		MoveControl( &m_staticRange, 20, 67 );
		MoveControl( GetEditRange(), 95, 67 );
		MoveControl( &m_buttonOK, 280, 83 );
		MoveControl( &m_buttonCancel, 280, 99 );
		ResizeDialog( 337, 120 );
	}

	return TRUE;  
}

/**********************************************************************/

void CPluRangeAddDlgEcrman::OnKillFocusDeptNo() 
{
	m_DepartmentSelector.SelectDepartmentFromEditBox();
	m_PluInfo.m_nNewPluDeptNo = m_DepartmentSelector.GetDepartmentFilter();
}

/**********************************************************************/

void CPluRangeAddDlgEcrman::OnSelectDept() 
{
	m_DepartmentSelector.SelectDepartmentFromCombo();
	m_PluInfo.m_nNewPluDeptNo = m_DepartmentSelector.GetDepartmentFilter();
}

/**********************************************************************/

void CPluRangeAddDlgEcrman::OnKillFocusSupplier() 
{
	if ( TRUE == m_bShowSupplier )
	{
		m_EcrmanSupplierSelector.SelectEcrmanSupplierFromEditBox();
		m_PluInfo.m_nNewPluSuppNo = m_EcrmanSupplierSelector.GetEcrmanSupplierNo();
	}
}

/**********************************************************************/

void CPluRangeAddDlgEcrman::OnSelectSupplier() 
{
	if ( TRUE == m_bShowSupplier )
	{
		m_EcrmanSupplierSelector.SelectEcrmanSupplierFromCombo();
		m_PluInfo.m_nNewPluSuppNo = m_EcrmanSupplierSelector.GetEcrmanSupplierNo();	
	}
}

/**********************************************************************/

void CPluRangeAddDlgEcrman::OnButtonSupplier() 
{
	if ( TRUE == m_bShowSupplier )
	{
		CDataManagerInfo info;
		if ( DataManagerNonDb.OpenEcrmanSupplierBuffer( info ) == FALSE )
		{
			Prompter.ShareError( info );
			return;
		}

		//READING FROM THE FILE MAY CHANGE THE SUPPLIER LIST
		m_EcrmanSupplierSelector.FillEcrmanSupplierCombo( m_PluInfo.m_nNewPluSuppNo );

		CListDataEcrmanSupplierDlg dlg ( m_PluInfo.m_nNewPluSuppNo, this );
		
		int nSuppIdx;
		if ( ( m_PluInfo.m_nNewPluSuppNo >= EcrmanSupplier::SupplierNo.Min ) && ( m_PluInfo.m_nNewPluSuppNo <= EcrmanSupplier::SupplierNo.Max ) )
		{
			if ( DataManagerNonDb.EcrmanSupplier.FindSupplierByNumber( m_PluInfo.m_nNewPluSuppNo, nSuppIdx ) == FALSE )
			{
				CEcrmanSupplierCSVRecord Supplier;
				Supplier.SetSuppNo( m_PluInfo.m_nNewPluSuppNo );
				Supplier.SetSuppName( "" );
				DataManagerNonDb.EcrmanSupplier.InsertAt( nSuppIdx, Supplier );
			}
		}

		if ( dlg.DoModal() == IDOK )
		{
			if ( DataManagerNonDb.WriteEcrmanSupplierBuffer( info ) == FALSE )
				Prompter.WriteError( info );
			else
			{
				int nSuppIdx = dlg.GetListSelection();
				if ( nSuppIdx >= 0 )
				{
					CEcrmanSupplierCSVRecord Supplier;
					DataManagerNonDb.EcrmanSupplier.GetAt( nSuppIdx, Supplier );
					m_PluInfo.m_nNewPluSuppNo = Supplier.GetSuppNo();
				}

				m_EcrmanSupplierSelector.FillEcrmanSupplierCombo( m_PluInfo.m_nNewPluSuppNo );
			}
		}
		
		DataManagerNonDb.CloseEcrmanSupplierBuffer( info );
	}
}

/**********************************************************************/

void CPluRangeAddDlgEcrman::OnOK() 
{
	if ( UpdateData(TRUE) == TRUE )
	{
		OnKillFocusDeptNo();

		if ( TRUE == m_bShowSupplier )
			OnKillFocusSupplier();

		EndDialog(IDOK);
	}
}

/**********************************************************************/
