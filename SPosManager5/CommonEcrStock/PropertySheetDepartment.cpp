/**********************************************************************/
#include "PropertySheetDepartment.h"
/**********************************************************************/
IMPLEMENT_DYNAMIC(CPropertySheetDepartment, CSpinPropertySheet)
/**********************************************************************/

CPropertySheetDepartment::CPropertySheetDepartment( WORD wSpinID, int nDeptIdx, CListDataDlgChangeFlag& WarnCancelAfterChange, CWnd* pWndParent)
	 : CSpinPropertySheet( TRUE, wSpinID, "", pWndParent ), m_WarnCancelAfterChange( WarnCancelAfterChange )
{	
	m_pListDataDlg = NULL;

	m_psh.dwFlags |= PSH_NOAPPLYNOW;

	m_Page1.SetDepartmentRecord ( &m_DeptRecord );
	AddPage( &m_Page1 );
	m_PageArray.Add ( 1 );

	m_Page2.SetDepartmentRecord ( &m_DeptRecord );
	AddPage( &m_Page2 );
	m_PageArray.Add ( 2 );

	m_Page3.SetDepartmentRecord ( &m_DeptRecord );
	AddPage( &m_Page3 );
	m_PageArray.Add ( 3 );

	m_Page4.SetDepartmentRecord ( &m_DeptRecord );
	AddPage( &m_Page4 );
	m_PageArray.Add ( 4 );

	m_nDeptIdx = nDeptIdx;
	DataManager.Department.GetAt ( m_nDeptIdx, m_DeptRecord );
	DataManager.ListKeyImageWrapperDept.SetCurrentDeptNo( DEPTINFO_MODE_EDIT, m_DeptRecord.GetDeptNo(), m_DeptRecord.GetImageFilename() );
	UpdateTitle();
}

/**********************************************************************/

CPropertySheetDepartment::~CPropertySheetDepartment()
{
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropertySheetDepartment, CSpinPropertySheet)
	//{{AFX_MSG_MAP(CPropertySheetDepartment)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

void CPropertySheetDepartment::SpinNext()
{
	if ( m_nDeptIdx < DataManager.Department.GetSize() - 1 )
	{
		int nPage = m_PageArray.GetAt ( GetActiveIndex() );

		bool bUpdate = FALSE;

		switch ( nPage )
		{
		case 1:	bUpdate = m_Page1.UpdateRecord();	break;
		case 2:	bUpdate = m_Page2.UpdateRecord();	break;
		case 3:	bUpdate = m_Page3.UpdateRecord();	break;
		case 4:	bUpdate = m_Page4.UpdateRecord();	break;
		}
		
		if ( bUpdate == FALSE )
			return;

		SaveRecord();
		m_nDeptIdx++;
		DataManager.Department.GetAt ( m_nDeptIdx, m_DeptRecord );
		DataManager.ListKeyImageWrapperDept.SetCurrentDeptNo( DEPTINFO_MODE_EDIT, m_DeptRecord.GetDeptNo(), m_DeptRecord.GetImageFilename() );
		UpdateTitle();

		switch ( nPage )
		{
		case 1:	m_Page1.Refresh();	break;
		case 2:	m_Page2.Refresh();	break;
		case 3:	m_Page3.Refresh();	break;
		case 4:	m_Page4.Refresh();	break;
		}

		if ( m_pListDataDlg != NULL )
			m_pListDataDlg -> SelectLine ( m_nDeptIdx );
	}	
}

/**********************************************************************/

void CPropertySheetDepartment::SpinPrevious()
{
	if ( m_nDeptIdx >= 2 )
	{
		int nPage = m_PageArray.GetAt ( GetActiveIndex() );

		bool bUpdate = FALSE;

		switch ( nPage )
		{
		case 1:	bUpdate = m_Page1.UpdateRecord();	break;
		case 2:	bUpdate = m_Page2.UpdateRecord();	break;
		case 3:	bUpdate = m_Page3.UpdateRecord();	break;
		case 4:	bUpdate = m_Page4.UpdateRecord();	break;
		}
		
		if ( bUpdate == FALSE )
			return;

		SaveRecord();
		m_nDeptIdx--;
		DataManager.Department.GetAt ( m_nDeptIdx, m_DeptRecord );
		DataManager.ListKeyImageWrapperDept.SetCurrentDeptNo( DEPTINFO_MODE_EDIT, m_DeptRecord.GetDeptNo(), m_DeptRecord.GetImageFilename() );
		UpdateTitle();

		switch ( nPage )
		{
		case 1:	m_Page1.Refresh();	break;
		case 2:	m_Page2.Refresh();	break;
		case 3:	m_Page3.Refresh();	break;
		case 4:	m_Page4.Refresh();	break;
		}

		if ( m_pListDataDlg != NULL )
			m_pListDataDlg -> SelectLine ( m_nDeptIdx );
	}
}

/**********************************************************************/

void CPropertySheetDepartment::SaveRecord()
{
	DataManager.ListKeyImageWrapperDept.SaveRecordChanges();
	DataManager.Department.SetAt ( m_nDeptIdx, m_DeptRecord );
	m_WarnCancelAfterChange.SetFlag();
}

/**********************************************************************/

void CPropertySheetDepartment::UndoChanges()
{
	DataManager.ListKeyImageWrapperDept.UndoRecordChanges();
}

/**********************************************************************/

void CPropertySheetDepartment::UpdateTitle()
{
	CString strTitle;
	strTitle.Format ( "D%3.3d %s", m_DeptRecord.GetDeptNo(), m_DeptRecord.GetDefaultText() );
	SetTitle ( strTitle, 0 );
}

/**********************************************************************/
