/**********************************************************************/
#include "MaxLengths.h"
/**********************************************************************/
#include "DepartmentSetEditDlg.h"
/**********************************************************************/

CDepartmentSetEditDlg::CDepartmentSetEditDlg( bool bSystem, CDepartmentSetCSVRecord& DepartmentSetRecord, CWnd* pParent)
	: CSSDialog(CDepartmentSetEditDlg::IDD, pParent), m_DepartmentSetRecord ( DepartmentSetRecord )
{
	//{{AFX_DATA_INIT(CDepartmentSetEditDlg)
	m_strName = _T("");
	//}}AFX_DATA_INIT
	m_bSystem = bSystem;
	m_pDeptArray = bSystem ? &DataManagerNonDb.SystemDepartment : &DataManager.Department;
}

/**********************************************************************/

void CDepartmentSetEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDepartmentSetEditDlg)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_CHECK_BRITANNIA, m_checkBritannia);
	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
	DDV_MaxChars(pDX, m_strName, MAX_LENGTH_GENERAL_NAME);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CDepartmentSetEditDlg, CDialog)
	//{{AFX_MSG_MAP(CDepartmentSetEditDlg)
	ON_WM_MEASUREITEM()
	ON_BN_CLICKED(IDC_BUTTON_ALL, OnButtonAll)
	ON_BN_CLICKED(IDC_BUTTON_NONE, OnButtonNone)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CDepartmentSetEditDlg::OnInitDialog() 
{
	m_strName = m_DepartmentSetRecord.GetName();
	CDialog::OnInitDialog();	
	
	m_listDepartment.SubclassDlgItem ( IDC_LIST, this );
	m_listDepartment.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_listDepartment.InsertColumn ( 0, "Departments", LVCFMT_LEFT, 220 );
	m_listDepartment.SetLockItemZeroFlag( FALSE );
	m_checkBritannia.SetCheck( m_DepartmentSetRecord.GetBritanniaHotelsFlag() );

	bool bShowBritannia = FALSE;

#ifdef STOCKMAN_SYSTEM
	bShowBritannia = DealerFlags.GetBritanniaHotelsExportFlag();
#endif

	ShowAndEnableWindow(&m_checkBritannia, bShowBritannia);

	AddList();
	return TRUE;  
}

/**********************************************************************/

void CDepartmentSetEditDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
   if ( nIDCtl == IDC_LIST )  
   {
	   m_listDepartment.AdjustRowHeight( lpMeasureItemStruct, 10 ); 
   }
}

/**********************************************************************/

void CDepartmentSetEditDlg::OnButtonAll() 
{	
	m_listDepartment.SelectAll();
}

/**********************************************************************/

void CDepartmentSetEditDlg::OnButtonNone() 
{	
	m_listDepartment.SelectNone();
}

/**********************************************************************/

void CDepartmentSetEditDlg::AddList()
{
	m_listDepartment.ClearList();

	CArray<int,int> arrayDeptWithName;
	CArray<int,int> arrayDeptNoName;

	int nPromoIdx = -1;

	for ( int nDeptIdx = 0; nDeptIdx < m_pDeptArray -> GetSize(); nDeptIdx++ )
	{
		CDepartmentCSVRecord Dept;
		m_pDeptArray -> GetAt( nDeptIdx, Dept );
		int nDeptNo = Dept.GetDeptNo();

		CString strText = Dept.GetReportText();
			
		if ( ( 0 == nDeptNo ) && ( -1 == nPromoIdx ) )
		{
			nPromoIdx = nDeptIdx;
		}
		else if ( ( nDeptNo >= 1 ) && ( nDeptNo <= Dept::DeptNo.Max ) )
		{
			if ( ( strText != "" ) && ( strText != Dept.GetDefaultText() ) )
			{
				arrayDeptWithName.Add(nDeptIdx);
			}
			else
			{
				arrayDeptNoName.Add(nDeptNo);
			}
		}
	}

	for ( int n = 0; n < arrayDeptWithName.GetSize(); n++ )
	{
		CDepartmentCSVRecord Dept;
		m_pDeptArray -> GetAt( arrayDeptWithName.GetAt(n), Dept );
		
		CSSListTaggedSelectItem ListItem;
		ListItem.m_strText = Dept.GetReportText();
		ListItem.m_nData = Dept.GetDeptNo();
		ListItem.SetTagState( m_DepartmentSetRecord.GetDepartmentStatus ( Dept.GetDeptNo() ) ? 1 : 0 );
		m_listDepartment.AddSortedItemToList( ListItem );
	}

	if ( nPromoIdx != -1 )
	{
		CDepartmentCSVRecord Dept;
		m_pDeptArray -> GetAt( nPromoIdx, Dept );
		
		CString strText = Dept.GetReportText();
		if (strText == "")
		{
			strText = Dept.GetDefaultText();
		}

		CSSListTaggedSelectItem ListItem;
		ListItem.m_strText = strText;
		ListItem.m_nData = 0;
		ListItem.SetTagState( m_DepartmentSetRecord.GetDepartmentStatus ( 0 ) ? 1 : 0 );
		m_listDepartment.InsertItemIntoList( 0, ListItem );	
	}

	CDepartmentCSVRecord DeptRecord;
	for ( int n = 0; n < arrayDeptNoName.GetSize(); n++ )
	{
		int nDeptNo = arrayDeptNoName.GetAt(n);
		DeptRecord.SetDeptNo(nDeptNo);

		CSSListTaggedSelectItem ListItem;
		ListItem.m_strText = DeptRecord.GetDefaultText();
		ListItem.m_nData = nDeptNo;
		ListItem.SetTagState( m_DepartmentSetRecord.GetDepartmentStatus ( nDeptNo ) ? 1 : 0 );
		m_listDepartment.AddItemToList( ListItem );
	}
}

/**********************************************************************/

void CDepartmentSetEditDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		::TrimSpaces ( m_strName, FALSE );
		
		if (m_strName == "")
		{
			m_strName = "DEPARTMENT SET";
		}

		m_DepartmentSetRecord.SetName ( m_strName );
		
		for ( int nIndex = 0; nIndex < m_listDepartment.GetItemCount(); nIndex++ )
		{
			CSSListTaggedSelectItem ListItem;
			m_listDepartment.GetListItem( nIndex, ListItem );
			m_DepartmentSetRecord.IncludeDepartment( ListItem.m_nData, ListItem.GetTagState() == 1 );
		}

#ifdef STOCKMAN_SYSTEM
		if (DealerFlags.GetBritanniaHotelsExportFlag())
		{
			m_DepartmentSetRecord.SetBritanniaHotelsFlag(IsTicked(m_checkBritannia));
		}
#endif

		EndDialog ( IDOK );
	}
}

/**********************************************************************/
