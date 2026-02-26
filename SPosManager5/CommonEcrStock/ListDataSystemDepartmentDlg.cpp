/**********************************************************************/
#include "..\CommonEcrStock\DefEditDlg.h"
#include "NewRecordNumberDlg.h"
/**********************************************************************/
#include "ListDataSystemDepartmentDlg.h"
/**********************************************************************/

CSingleListContentHelpersSystemDepartment::CSingleListContentHelpersSystemDepartment()
{
}

/**********************************************************************/

int CSingleListContentHelpersSystemDepartment::GetArraySize()
{
	return DataManagerNonDb.SystemDepartment.GetSize();
}

/**********************************************************************/

bool CSingleListContentHelpersSystemDepartment::PrepareLine( int nIndex, CString& strLine )
{
	strLine = "";

	bool bResult = FALSE;
	if ( ( nIndex >= 0 ) && ( nIndex < GetArraySize() ) )
	{
		CDepartmentCSVRecord DeptRecord;
		DataManagerNonDb.SystemDepartment.GetAt ( nIndex, DeptRecord );
	
		strLine.Format ( "%d\t%s",
			DeptRecord.GetDeptNo(),
			DeptRecord.GetReportText() );

		bResult = TRUE;
	}
	
	return bResult;
}

/**********************************************************************/

void CSingleListContentHelpersSystemDepartment::DeleteDatabaseEntry( int nIndex )
{
	if ( ( nIndex >= 0 ) && ( nIndex < DataManagerNonDb.SystemDepartment.GetSize() ) )
	{
		CDepartmentCSVRecord DeptRecord;
		DataManagerNonDb.SystemDepartment.GetAt( nIndex, DeptRecord );
		DataManagerNonDb.SystemDepartment.RemoveAt( nIndex );
		m_bDeletionEffect = TRUE;
	}
}

/**********************************************************************/

void CSingleListContentHelpersSystemDepartment::FilterSelectedItemArray( CReportConsolidationArray<CSortedIntItem>& arraySelected )
{
	m_bPreventDeletion = FALSE;

	for ( int n = 0; n < arraySelected.GetSize(); n++ )
	{
		CSortedIntItem item;
		arraySelected.GetAt( n, item );

		CDepartmentCSVRecord dept;
		DataManagerNonDb.SystemDepartment.GetAt( item.m_nItem, dept );

		if ( dept.GetDeptNo() == 0 )
		{
			arraySelected.RemoveAt( n );
			m_bPreventDeletion = TRUE;
			break;
		}
	}

	m_bPreventDeletion &= ( arraySelected.GetSize() == 0 );
}

/**********************************************************************/

void CSingleListContentHelpersSystemDepartment::SetColumns( CSSListCtrlVirtual& list, CWnd* pParent )
{
	list.SubclassDlgItem ( IDC_LIST, pParent );
	list.InsertColumn ( 0, "No", LVCFMT_LEFT, 60 );
	list.InsertColumn ( 1, "Name", LVCFMT_LEFT, 420 );
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CListDataSystemDepartmentDlg::CListDataSystemDepartmentDlg( CWnd* pParent) : CListDataDlg ( pParent )
{
	m_pContentHelpers = new CSingleListContentHelpersSystemDepartment();
	m_pDisplayHelpers = new CSingleListDisplayHelpers( m_listData, *m_pContentHelpers );

	m_pDisplayHelpers -> SetTopSingleText( "Department" );
	m_pDisplayHelpers -> SetTopPluralText( "Departments" );
	m_pDisplayHelpers -> SetBottomSingleText( "department" );
	m_pDisplayHelpers -> SetBottomPluralText( "departments" );
}

/**********************************************************************/

CListDataSystemDepartmentDlg::~CListDataSystemDepartmentDlg()
{
	delete m_pDisplayHelpers;
	delete m_pContentHelpers;
}

/**********************************************************************/

void CListDataSystemDepartmentDlg::SetTitle()
{
	SetWindowText ( "System Department Names" );
}

/**********************************************************************/

void CListDataSystemDepartmentDlg::SetButtonTexts()
{
	m_button1.SetWindowText ( "&Add" );
	m_button2.SetWindowText ( "&Edit" );
	m_button3.SetWindowText ( "&Delete" );
	m_button4.SetWindowText ( "Au&to" );
	m_button5.ShowWindow ( SW_HIDE );
	m_button6.ShowWindow ( SW_HIDE );

	MoveControl( &m_button1, 368, 7 );
	MoveControl( &m_button2, 368, 47 );
	MoveControl( &m_button3, 368, 87 );
	MoveControl( &m_button4, 368, 147 );
	MoveControl( &m_button5, 368, 187 );
}

/**********************************************************************/

void CListDataSystemDepartmentDlg::SetColumns()
{
	m_pContentHelpers -> SetColumns( m_listData, this );
}

/**********************************************************************/

void CListDataSystemDepartmentDlg::HandleButton1()
{
	bool bAdd = FALSE;
	int nDeptIdx, nDeptNo;
		
	for ( nDeptNo = 1; nDeptNo <= Dept::DeptNo.Max; nDeptNo++ )
	{
		if ( DataManagerNonDb.SystemDepartment.FindDeptByNumber ( nDeptNo, nDeptIdx ) == FALSE )
		{
			bAdd = TRUE;
			break;
		}
	}

	if ( bAdd == FALSE )
	{
		Prompter.Error ( "All available department numbers are already in use." );
		return;
	}

	CNewRecordNumberDlg dlg ( NODE_DEPARTMENT, this );
	dlg.m_nNumber = nDeptNo;
	if ( dlg.DoModal() != IDOK )
		return;

	nDeptNo = dlg.m_nNumber;
	if ( DataManagerNonDb.SystemDepartment.FindDeptByNumber ( nDeptNo, nDeptIdx ) )
	{
		Prompter.Error ( "The selected department number is already in use." );
		return;
	}

	CDepartmentCSVRecord Department;
	Department.SetDeptNo( nDeptNo );
	Department.SetEposText( "" );
	Department.SetRepText( Department.GetDefaultText() );
	Department.SetEposGroup( 0 );
	Department.SetReportGroup( 0 );
	Department.SetReportEnable(TRUE);
	DataManagerNonDb.SystemDepartment.InsertAt ( nDeptIdx, Department );

	CChangeSystemDepartmentNameDlg dlgEdit( nDeptIdx, this );
	if ( dlgEdit.DoModal() == IDOK )
	{
		Department.SetRepText( dlgEdit.m_strName );
		DataManagerNonDb.SystemDepartment.SetAt( nDeptIdx, Department );
	}
	
	m_listData.SetItemCountEx ( GetArraySize() );
	SelectLine ( nDeptIdx );

	m_WarnCancelAfterChange.SetFlag();
}

/**********************************************************************/

void CListDataSystemDepartmentDlg::HandleButton2() 
{
	int nIndex = GetSafeSelection();
	if ( nIndex == -1 )
		return;

	if ( IsDepartmentZero( nIndex ) == TRUE )
		return;

	CChangeSystemDepartmentNameDlg dlgEdit( nIndex, this );
	
	if ( dlgEdit.DoModal() == IDOK )
	{
		CDepartmentCSVRecord DeptRecord;
		DataManagerNonDb.SystemDepartment.GetAt( nIndex, DeptRecord );
		DeptRecord.SetReportEnable(TRUE);
		DeptRecord.SetRepText( dlgEdit.m_strName );
		DataManagerNonDb.SystemDepartment.SetAt( nIndex, DeptRecord );
		SelectLine ( nIndex );

		m_WarnCancelAfterChange.SetFlag();
	}
}

/**********************************************************************/

void CListDataSystemDepartmentDlg::HandleButton3()
{
	m_pDisplayHelpers -> HandleDelete( this );
	m_WarnCancelAfterChange.SetFlag( m_pContentHelpers -> GetDeletionEffectFlag() );

	if ( m_pContentHelpers -> GetPreventDeletionFlag() == TRUE )
		DepartmentZeroMessage();
}

/**********************************************************************/

void CListDataSystemDepartmentDlg::HandleButton4()
{
	if ( Prompter.YesNo( "Do you wish to copy department names\nfrom your system databases" ) == IDYES )
	{
		int nLastNewDeptIdx = -1;

		for ( int nDbIdx = 0; nDbIdx < dbDatabase.GetSize(); nDbIdx++ )
		{
			DataManager.OpenDatabaseQuickNoFiles( nDbIdx, FALSE );

			CDataManagerInfo info;
			DataManager.OpenDepartment( DB_READONLY, info );

			for ( int nDeptIdx = 0; nDeptIdx < DataManager.Department.GetSize(); nDeptIdx++ )
			{
				CDepartmentCSVRecord DeptRecord;
				DataManager.Department.GetAt( nDeptIdx, DeptRecord );
				int nDeptNo = DeptRecord.GetDeptNo();

				if ( nDeptNo != 0 )
				{
					int nSysDeptIdx;
					if ( DataManagerNonDb.SystemDepartment.FindDeptByNumber( nDeptNo, nSysDeptIdx ) == FALSE )
					{
						CString strText = DeptRecord.GetReportText();
						
						if ( strText == "" )
							strText.Format( "Department %d", nDeptNo );

						CDepartmentCSVRecord SysDeptRecord;
						SysDeptRecord.SetReportEnable(TRUE);
						SysDeptRecord.SetRepText( strText );
						DataManagerNonDb.SystemDepartment.InsertAt( nSysDeptIdx, DeptRecord );

						nLastNewDeptIdx = nSysDeptIdx;
					}
				}
			}

			DataManager.CloseDatabase();
		}

		if ( nLastNewDeptIdx != -1 )
		{
			m_listData.SetItemCountEx ( GetArraySize() );
			SelectLine ( nLastNewDeptIdx );
		}

		m_WarnCancelAfterChange.SetFlag();
	}
}

/**********************************************************************/

bool CListDataSystemDepartmentDlg::IsDepartmentZero( int nIndex )
{
	CDepartmentCSVRecord dept;
	DataManagerNonDb.SystemDepartment.GetAt( nIndex, dept );

	if ( dept.GetDeptNo() == 0 )
	{
		DepartmentZeroMessage();
		return TRUE;
	}

	return FALSE;
}

/**********************************************************************/

void CListDataSystemDepartmentDlg::DepartmentZeroMessage()
{
	Prompter.Error( "Department zero is for internal EPOS use only and cannot be edited or deleted." );
}

/**********************************************************************/

