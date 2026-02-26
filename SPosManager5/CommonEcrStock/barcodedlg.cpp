/**********************************************************************/
#include "BarcodeCSVArray.h"
#include "BarcodePluNoTable.h"
#include "DatabaseCSVArray.h"
//#include "DataManager.h"
//#include "DealerFlags.h"
//#include "EcrmanOptionsIni.h"
//#include "globalfunctions.h"
#include "MaxLengths.h"
#include "NodeTypes.h"
#include "Password.h"
/**********************************************************************/
#include "BarcodeDlg.h"
/**********************************************************************/

CBarcodeDlg::CBarcodeDlg( bool bShelf, CWnd* pParent ) : CSSDialog(CBarcodeDlg::IDD, pParent),
	m_DepartmentSelector ( m_editDeptNo, m_comboDeptName )
{
	//{{AFX_DATA_INIT(CBarcodeDlg)
	//}}AFX_DATA_INIT
	m_nType = 0;
	m_nItemLabels = 1;
	m_nDepartmentFilter = 0;
	m_bShelf = bShelf;
	
	m_nPluNoFrom = 0;
	m_nPluNoTo = 0;
	m_nBarcodeNoFrom = 0;
	m_nBarcodeNoTo = 0;

	m_strPluNoFrom = "";
	m_strPluNoTo = "";
	m_strBarcodeNoFrom = "";
	m_strBarcodeNoTo = "";
}

/**********************************************************************/

void CBarcodeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBarcodeDlg)
	DDX_Control(pDX, IDC_COMBO_LOCATION, m_comboLocation);
	DDX_Control(pDX, IDC_RADIO_TYPE2, m_radioType2);
	DDX_Control(pDX, IDC_RADIO_TYPE, m_radioType);
	DDX_Control(pDX, IDC_STATIC_TYPE, m_staticType);
	DDX_Control(pDX, IDC_EDIT_DEPTNO, m_editDeptNo);
	DDX_Control(pDX, IDC_COMBO_DEPTNAME, m_comboDeptName);
	DDX_Text(pDX, IDC_EDIT_ITEM_LABEL, m_nItemLabels);
	DDV_MinMaxInt(pDX, m_nItemLabels, 1, 99);
	DDX_Radio(pDX, IDC_RADIO_TYPE, m_nType);
	//}}AFX_DATA_MAP
	DDX_IntegerString(pDX, IDC_EDIT_PLU_RANGE_START, m_strPluNoFrom);
	DDV_IntegerString(pDX, m_strPluNoFrom, "1", Super.PluEnd() );
	DDX_IntegerString(pDX, IDC_EDIT_PLU_RANGE_END, m_strPluNoTo);
	DDV_IntegerString(pDX, m_strPluNoTo, "1", Super.PluEnd() );
	DDX_IntegerString(pDX, IDC_EDIT_BARCODE_RANGE_START, m_strBarcodeNoFrom);
	DDV_IntegerString(pDX, m_strBarcodeNoFrom, "1", Super.BarcodeEnd() );
	DDX_IntegerString(pDX, IDC_EDIT_BARCODE_RANGE_END, m_strBarcodeNoTo);
	DDV_IntegerString(pDX, m_strBarcodeNoTo, "1", Super.BarcodeEnd() );
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CBarcodeDlg, CDialog)
	//{{AFX_MSG_MAP(CBarcodeDlg)
	ON_CBN_SELCHANGE(IDC_COMBO_DEPTNAME, OnSelchangeComboDeptname)
	ON_EN_KILLFOCUS(IDC_EDIT_DEPTNO, OnKillFocusDeptNumber)
	ON_EN_KILLFOCUS(IDC_EDIT_PLU_RANGE_END, OnKillFocusPluNo)
	ON_BN_CLICKED(IDC_STATIC_RANGE, OnStaticRange)
	ON_BN_CLICKED(IDC_STATIC_RANGE2, OnStaticRange2)
	ON_EN_KILLFOCUS(IDC_EDIT_BARCODE_RANGE_END, OnKillFocusBarcodeNo)
	ON_EN_KILLFOCUS(IDC_EDIT_PLU_RANGE_START, OnKillFocusPluNo)
	ON_EN_KILLFOCUS(IDC_EDIT_BARCODE_RANGE_START, OnKillFocusBarcodeNo)
	ON_BN_CLICKED(IDC_RADIO_TYPE, OnRadioType)
	ON_BN_CLICKED(IDC_RADIO_TYPE2, OnRadioType2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/
CEdit* CBarcodeDlg::GetEditPluFrom() { return GetEdit( IDC_EDIT_PLU_RANGE_START ); }
CEdit* CBarcodeDlg::GetEditPluTo() { return GetEdit( IDC_EDIT_PLU_RANGE_END ); }
CEdit* CBarcodeDlg::GetEditBarcodeFrom() { return GetEdit( IDC_EDIT_BARCODE_RANGE_START ); }
CEdit* CBarcodeDlg::GetEditBarcodeTo() { return GetEdit( IDC_EDIT_BARCODE_RANGE_END ); }
CEdit* CBarcodeDlg::GetEditItemLabel() { return GetEdit( IDC_EDIT_ITEM_LABEL ); }
/**********************************************************************/

BOOL CBarcodeDlg::OnInitDialog() 
{
	m_strPluNoFrom = "";
	m_strPluNoTo = "";
	m_strBarcodeNoFrom = "";
	m_strBarcodeNoTo = "";

	if ( 0 != m_nPluNoFrom )
		::FormatPluNo( m_nPluNoFrom, m_strPluNoFrom );

	if ( 0 != m_nPluNoTo )
		::FormatPluNo( m_nPluNoTo, m_strPluNoTo );

	if ( 0 != m_nBarcodeNoFrom )
		::FormatBarcodeNo( m_nBarcodeNoFrom, m_strBarcodeNoFrom );

	if ( 0 != m_nBarcodeNoTo )
		::FormatBarcodeNo( m_nBarcodeNoTo, m_strBarcodeNoTo );

	CDialog::OnInitDialog();

	if ( m_bShelf == FALSE )
		SetWindowText ( "Print Product Labels" );
	else
		SetWindowText ( "Print Shelf Edge Labels" );

	SubclassEdit ( IDC_EDIT_LABEL, SS_NUM, 3, "%d" );
	SubclassEdit ( IDC_EDIT_ITEM_LABEL, SS_NUM, 2, "%d" );
	SubclassEdit ( IDC_EDIT_PLU_RANGE_START, SS_NUM, Super.MaxPluLen(), "%s" );
	SubclassEdit ( IDC_EDIT_PLU_RANGE_END, SS_NUM, Super.MaxPluLen(), "%s" );
	SubclassEdit ( IDC_EDIT_BARCODE_RANGE_START, SS_NUM, Super.MaxBarcodeLen(), "%s" );
	SubclassEdit ( IDC_EDIT_BARCODE_RANGE_END, SS_NUM, Super.MaxBarcodeLen(), "%s" );

	m_editDeptNo.LimitText ( MAX_LENGTH_DEPTNO );
	m_DepartmentSelector.FillDepartmentComboDatabase( m_nDepartmentFilter, DEPARTMENT_SELECTOR_NORMAL );
	
	if ( m_nDepartmentFilter != DEPARTMENTSET_ALL )
	{
		m_comboDeptName.EnableWindow ( FALSE );
		m_editDeptNo.EnableWindow ( FALSE );
	}
	
	CWordArray arrayTypes;

#ifdef STOCKMAN_SYSTEM
	
	switch( PasswordArray.GetLocationAccessType() )
	{
	case LOCATION_ACCESS_SYSTEM:
		arrayTypes.Add ( NODE_STOCKPOINT );
		arrayTypes.Add ( NODE_SITE );
		arrayTypes.Add ( NODE_DATABASE );
		break;

	case LOCATION_ACCESS_DB:
		arrayTypes.Add ( NODE_STOCKPOINT );
		arrayTypes.Add ( NODE_SITE );
		arrayTypes.Add ( NODE_DATABASE );
		break;

	case LOCATION_ACCESS_LOC:
		arrayTypes.Add ( NODE_STOCKPOINT );
		m_LocationSelector.SetSingleSpIdx( PasswordArray.GetLocationAccessSpIdx() );
		break;
	}
	
	bool bNoApparentStock = FALSE;

	if ( DealerFlags.GetCurrentStockLevelsAccessType() == STOCKLEVELS_ACCESS_NONE )
		bNoApparentStock = TRUE;

	if ( PasswordArray.CheckLocationAccessSp() == FALSE )
		bNoApparentStock = TRUE;
	
	if ( FALSE == bNoApparentStock )	
		m_staticType.ShowWindow( SW_HIDE );
	else
	{
		m_radioType.ShowWindow( SW_HIDE );
		m_radioType2.ShowWindow( SW_HIDE );
		m_comboLocation.ShowWindow( SW_HIDE );
	}

#else

	arrayTypes.Add ( NODE_DATABASE );

	if ( EcrmanOptions.GetFeaturesPluStockFlag() == TRUE )
		m_staticType.ShowWindow( SW_HIDE );
	else
	{
		m_radioType.ShowWindow( SW_HIDE );
		m_radioType2.ShowWindow( SW_HIDE );
		m_comboLocation.ShowWindow( SW_HIDE );
	}

#endif
	
	m_LocationSelector.SetSingleDbIdx( DataManager.GetActiveDbIdx() );
	m_LocationSelector.BuildList( arrayTypes );
	m_LocationSelector.FillLocationCombo ( &m_comboLocation );

	//ASSUME THAT WE START WITH LABELS PER ITEM
	m_comboLocation.SetCurSel(0);
	m_comboLocation.EnableWindow( FALSE );

	return TRUE;  
}

/**********************************************************************/

void CBarcodeDlg::OnKillFocusDeptNumber() 
{
	m_DepartmentSelector.SelectDepartmentFromEditBox();
}

/**********************************************************************/

void CBarcodeDlg::OnSelchangeComboDeptname() 
{
	m_DepartmentSelector.SelectDepartmentFromCombo();
}

/**********************************************************************/

void CBarcodeDlg::OnKillFocusPluNo() 
{
	UpdateData( TRUE, FALSE );
	m_strPluNoFrom = BarcodePluNoTable.ConvertPlu ( m_strPluNoFrom );
	m_strPluNoTo = BarcodePluNoTable.ConvertPlu ( m_strPluNoTo );
	UpdateData ( FALSE );
}

/**********************************************************************/

void CBarcodeDlg::OnKillFocusBarcodeNo() 
{
	UpdateData( TRUE, FALSE );
	m_strBarcodeNoFrom = BarcodePluNoTable.ConvertBarcode( m_strBarcodeNoFrom );
	m_strBarcodeNoTo = BarcodePluNoTable.ConvertBarcode( m_strBarcodeNoTo );
	UpdateData( FALSE );
}

/**********************************************************************/

void CBarcodeDlg::OnStaticRange() 
{
	CString strStart = "1";
	::AddLeading ( strStart, Super.MaxPluLen(), '0' );
	GetEditPluFrom() -> SetWindowText ( strStart );
	
	CString strEnd = "9";
	::AddLeading ( strEnd, Super.MaxPluLen(), '9' );
	GetEditPluTo() -> SetWindowText( strEnd );	
}

/**********************************************************************/

void CBarcodeDlg::OnStaticRange2() 
{
	CString strStart;
	::FormatBarcodeNo( 1, strStart );
	GetEditBarcodeFrom() -> SetWindowText ( strStart );
	
	CString strEnd;
	::FormatBarcodeNo( Super.MaxBarcodeInt(), strStart );
	GetEditBarcodeFrom() -> SetWindowText( strEnd );
}

/**********************************************************************/

void CBarcodeDlg::OnRadioType() 
{
	m_comboLocation.EnableWindow( FALSE );
	GetEditItemLabel() -> EnableWindow( TRUE );
}

/**********************************************************************/

void CBarcodeDlg::OnRadioType2() 
{
	m_comboLocation.EnableWindow( m_comboLocation.GetCount() > 1 );
	GetEditItemLabel() -> EnableWindow( FALSE );
}

/**********************************************************************/

void CBarcodeDlg::OnOK() 
{
	if ( UpdateData ( TRUE, TRUE ) )
	{
		OnKillFocusDeptNumber();
		OnKillFocusPluNo();
		OnKillFocusBarcodeNo();

		m_LocationSelector.GetSelectedEntity( m_LocSelEntity );

		m_nDepartmentFilter = m_DepartmentSelector.GetDepartmentFilter();

		m_nPluNoTo = _atoi64( m_strPluNoTo );
		m_nPluNoFrom = _atoi64( m_strPluNoFrom );
		m_nBarcodeNoTo = _atoi64( m_strBarcodeNoTo );
		m_nBarcodeNoFrom = _atoi64( m_strBarcodeNoFrom );

		EndDialog (IDOK);
	}
}

/**********************************************************************/

