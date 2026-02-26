/**********************************************************************/
#include "BarcodeCSVArray.h"
//#include "DataManager.h"
#include "DepartmentCSVArray.h"
#include "DepartmentSetCSVArray.h"
//#include "EcrmanOptionsIni.h"
#include "MaxLengths.h"
#include "SearchTypes.h"
#include "SysInfo.h"
/**********************************************************************/
#include "BarcodeFilterDlg.h"
/**********************************************************************/

CBarcodeFilterDlg::CBarcodeFilterDlg( CBarcodeFilterArray& FilterArray, CWnd* pParent )
	: CSSDialog(CBarcodeFilterDlg::IDD, pParent), 
	m_BarcodeFilterArray ( FilterArray ), 
	m_DepartmentSelector ( m_editDeptNo, m_comboDeptName ),
	m_CacheCSV ( '\t' )
{
	//{{AFX_DATA_INIT(CBarcodeFilterDlg)
	m_strStockCode = _T("");
	//}}AFX_DATA_INIT
	m_nBarcodeIdx = -1;
	m_strTitle = "";
	m_strSearchText = m_BarcodeFilterArray.GetDescription();
#ifdef STOCKMAN_SYSTEM
	m_strStockCode = m_BarcodeFilterArray.GetStockcodeFilter();
#endif
	m_nDepartmentFilter = m_BarcodeFilterArray.GetDepartmentFilter();
	m_nCacheIndex = -1;
}

/**********************************************************************/

void CBarcodeFilterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBarcodeFilterDlg)
	DDX_Control(pDX, IDC_STATIC_FILTER, m_staticFilter);
	DDX_Control(pDX, IDC_EDIT_DEPTNO, m_editDeptNo);
	DDX_Control(pDX, IDC_COMBO_DEPTNAME, m_comboDeptName);
	DDX_Control(pDX, IDC_STATIC_STOCKCODE, m_staticStockCode);
	DDX_Control(pDX, IDC_EDIT_STOCKCODE, m_editStockCode);
	DDX_Control(pDX, IDOK, m_buttonOK);
	DDX_Control(pDX, IDCANCEL, m_buttonCancel);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_PLUNAME, m_strSearchText);
	DDV_MaxChars(pDX, m_strSearchText, MAX_LENGTH_SEARCH_TEXT);
	DDX_Text(pDX, IDC_EDIT_STOCKCODE, m_strStockCode);
	DDV_MaxChars(pDX, m_strStockCode, Super.MaxStockLen());
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CBarcodeFilterDlg, CDialog)
	//{{AFX_MSG_MAP(CBarcodeFilterDlg)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDoubleClickList)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST, OnGetdispinfoList)
	ON_EN_KILLFOCUS(IDC_EDIT_DEPTNO, OnKillFocusDeptNo)
	ON_CBN_SELCHANGE(IDC_COMBO_DEPTNAME, OnSelectDepartment)
	ON_EN_KILLFOCUS(IDC_EDIT_STOCKCODE, OnSelectStockCode)
	ON_EN_KILLFOCUS(IDC_EDIT_PLUNAME, OnSelectSearchText)
	ON_NOTIFY(LVN_ODCACHEHINT, IDC_LIST, OnListCacheHint)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CBarcodeFilterDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_listBarcode.SubclassDlgItem ( IDC_LIST, this );

	if ( EcrmanOptions.AllowBarcodeModifierLinks() == TRUE  )
	{
		ResizeDialog( 424, 261 );
		ResizeControl( &m_staticFilter, 410, 59 );
		ResizeControl( &m_listBarcode, 410, 161 );
		MoveControl( &m_buttonOK, 312, 240 );
		MoveControl( &m_buttonCancel, 367, 240 );
	}
	
	m_listBarcode.InsertColumn ( 0, "Barcode No", LVCFMT_LEFT, 110 );
	m_listBarcode.InsertColumn ( 1, "Plu No", LVCFMT_LEFT, 110 );

	int nColumn = 2;
	if (  EcrmanOptions.AllowBarcodeModifierLinks() == TRUE  )
		m_listBarcode.InsertColumn ( nColumn++, "Modifier", LVCFMT_LEFT, 100 );

#ifdef STOCKMAN_SYSTEM
	m_listBarcode.InsertColumn ( nColumn++, "Description", LVCFMT_LEFT, 115 );
	m_listBarcode.InsertColumn ( nColumn++, "Stock Code", LVCFMT_LEFT, 110 );
#else
	m_listBarcode.InsertColumn ( nColumn++, "Epos Text", LVCFMT_LEFT, 110 );
	m_listBarcode.InsertColumn ( nColumn++, "Report Text", LVCFMT_LEFT, 115 );
#endif

	m_listBarcode.InsertColumn ( nColumn++, "Dpt", LVCFMT_RIGHT, 30 );

	m_strTitle = "Browse Barcode Database";
	
	m_editDeptNo.LimitText ( MAX_LENGTH_DEPTNO );
	m_DepartmentSelector.FillDepartmentComboDatabase( m_nDepartmentFilter, DEPARTMENT_SELECTOR_NORMAL );

#ifndef STOCKMAN_SYSTEM
	ShowAndEnableWindow( &m_staticStockCode, FALSE );
	ShowAndEnableWindow( &m_editStockCode, FALSE );
#endif

	RunSearch ( TRUE, TRUE );
	return TRUE;  
}

/**********************************************************************/

void CBarcodeFilterDlg::OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;
				
	int nIndex = m_listBarcode.GetCurSel();
	
	if ( nIndex >= 0 && nIndex < m_listBarcode.GetItemCount() )
	{
		m_nBarcodeIdx = m_BarcodeFilterArray.GetBarcodeItemIndexAt ( nIndex );	
		EndDialog ( IDOK );
	}	
}

/**********************************************************************/

void CBarcodeFilterDlg::RunSearch( bool bDoSearch, bool bUpdateDisplay )
{
	if ( bDoSearch == TRUE )
	{
		OnKillFocusDeptNo();
		m_BarcodeFilterArray.SetDescriptionFilter ( m_strSearchText, SEARCH_ALLTEXT, FALSE );
		m_BarcodeFilterArray.SetDepartmentFilter ( m_DepartmentSelector.GetDepartmentFilter() );
#ifdef STOCKMAN_SYSTEM
		m_BarcodeFilterArray.SetStockcodeFilter ( m_strStockCode );
#endif
		m_BarcodeFilterArray.BuildRecordList();
	}

	if ( bUpdateDisplay == TRUE )
	{
		m_listBarcode.SetItemCountEx ( m_BarcodeFilterArray.GetSize() );
		m_listBarcode.Invalidate();
		m_listBarcode.SetCurSel(0);
		UpdateTitle();
	}
}

/**********************************************************************/

void CBarcodeFilterDlg::UpdateTitle()
{
	CString strTitle;
	strTitle.Format ( "%s ( %d of %d records )",
		( const char* ) m_strTitle,
		m_BarcodeFilterArray.GetSize(),
		DataManager.Barcode.GetSize() );

	SetWindowText ( strTitle );
}

/**********************************************************************/

void CBarcodeFilterDlg::OnGetdispinfoList(NMHDR* pNMHDR, LRESULT* pResult) 
{	
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	if (( pDispInfo->item.mask & LVIF_TEXT ) != 0 )
	{
		int nIndex = pDispInfo->item.iItem;

		if ( nIndex != m_nCacheIndex  )
		{
			m_CacheCSV.ParseLine( m_BarcodeFilterArray.GetDisplayLine ( nIndex ) );
			m_nCacheIndex = nIndex;
		}

		lstrcpy ( pDispInfo->item.pszText, m_CacheCSV.GetString ( pDispInfo->item.iSubItem ) );
	}
	*pResult = 0;
}

/**********************************************************************/

void CBarcodeFilterDlg::OnKillFocusDeptNo() 
{
	int nOldDepartmentFilter = m_nDepartmentFilter;
	m_DepartmentSelector.SelectDepartmentFromEditBox();
	m_nDepartmentFilter = m_DepartmentSelector.GetDepartmentFilter();

	if ( m_nDepartmentFilter != nOldDepartmentFilter )
		RunSearch( TRUE, TRUE );
}

/**********************************************************************/

void CBarcodeFilterDlg::OnSelectDepartment() 
{
	int nOldDepartmentFilter = m_nDepartmentFilter;
	m_DepartmentSelector.SelectDepartmentFromCombo();
	m_nDepartmentFilter = m_DepartmentSelector.GetDepartmentFilter();

	if ( m_nDepartmentFilter != nOldDepartmentFilter )
		RunSearch( TRUE, TRUE );
}

/**********************************************************************/

void CBarcodeFilterDlg::OnSelectStockCode() 
{
#ifdef STOCKMAN_SYSTEM
	CString strStockCodeOld = m_strStockCode;

	if ( UpdateData ( TRUE ) == TRUE )
	{
		::TrimSpaces ( m_strStockCode, FALSE );
		
		if ( m_strStockCode != strStockCodeOld )
		{
			RunSearch ( TRUE, TRUE );
			UpdateData ( FALSE );
		}
	}
#endif
}

/**********************************************************************/

void CBarcodeFilterDlg::OnSelectSearchText() 
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

void CBarcodeFilterDlg::OnListCacheHint(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMLVCACHEHINT* pCacheHint = (NMLVCACHEHINT*)pNMHDR;
	m_nCacheIndex = -1;	
	*pResult = 0;
}

/**********************************************************************/

void CBarcodeFilterDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		m_nBarcodeIdx = -1;

		int nIndex = m_listBarcode.GetCurSel();
	
		if ( nIndex >= 0 && nIndex < m_listBarcode.GetItemCount() )
			m_nBarcodeIdx = m_BarcodeFilterArray.GetBarcodeItemIndexAt ( nIndex );
	
		EndDialog ( IDOK );
	}
}

/**********************************************************************/
