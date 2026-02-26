/**********************************************************************/
#include "DepartmentSetEditDlg.h"
#include "DatabaseCSVArray.h"
/**********************************************************************/
#include "ListDataDepartmentSetDlg.h"
/**********************************************************************/

CSingleListContentHelpersDepartmentSet::CSingleListContentHelpersDepartmentSet( CDepartmentSetCSVArray* pDeptSetArray ) 
{
	m_pDeptSetArray = pDeptSetArray;
}

/**********************************************************************/

int CSingleListContentHelpersDepartmentSet::GetArraySize()
{
	return m_pDeptSetArray -> GetSize();
}

/**********************************************************************/

bool CSingleListContentHelpersDepartmentSet::PrepareLine( int nIndex, CString& strLine )
{
	strLine = "";

	bool bResult = FALSE;
	if ( ( nIndex >= 0 ) && ( nIndex < GetArraySize() ) )
	{
		strLine = m_pDeptSetArray -> GetName ( nIndex );

#ifdef STOCKMAN_SYSTEM
		if (DealerFlags.GetBritanniaHotelsExportFlag() == TRUE)
		{
			strLine += "\t";
			strLine += m_pDeptSetArray -> GetBritanniaHotelsFlag ( nIndex ) ? "Yes" : "";
		}
#endif

		bResult = TRUE;
	}
	
	return bResult;
}

/**********************************************************************/

void CSingleListContentHelpersDepartmentSet::DeleteDatabaseEntry( int nIndex )
{
	if ( ( nIndex >= 0 ) && ( nIndex < m_pDeptSetArray -> GetSize() ) )
	{
		m_pDeptSetArray -> RemoveAt( nIndex );
		m_bDeletionEffect = TRUE;
	}
}

/**********************************************************************/

void CSingleListContentHelpersDepartmentSet::SetColumns(CSSListCtrlVirtual& list, CWnd* pParent)
{
	list.SubclassDlgItem(IDC_LIST, pParent);

	bool bBritanniaFlag = FALSE;

#ifdef STOCKMAN_SYSTEM
	bBritanniaFlag = DealerFlags.GetBritanniaHotelsExportFlag();
#endif

	if (FALSE == bBritanniaFlag)
	{
		list.InsertColumn(0, "Sets", LVCFMT_LEFT, 480);
	}
	else
	{
		list.InsertColumn(0, "Sets", LVCFMT_LEFT, 330);
		list.InsertColumn(1, "Britannia Export", LVCFMT_LEFT, 150);
	}
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CListDataDepartmentSetDlg::CListDataDepartmentSetDlg( bool bSystem, CWnd* pParent) : CListDataDlg ( pParent )
{
	m_bSystem = bSystem;
	m_pDeptSetArray = bSystem ? &DataManagerNonDb.SystemDepartmentSet : &DataManager.DepartmentSet;

	m_pContentHelpers = new CSingleListContentHelpersDepartmentSet( m_pDeptSetArray );
	m_pDisplayHelpers = new CSingleListDisplayHelpers( m_listData, *m_pContentHelpers );

	m_pDisplayHelpers -> SetTopSingleText( "department set" );
	m_pDisplayHelpers -> SetTopPluralText( "department sets" );
	m_pDisplayHelpers -> SetBottomSingleText( "set" );
	m_pDisplayHelpers -> SetBottomPluralText( "sets" );
}

/**********************************************************************/

CListDataDepartmentSetDlg::~CListDataDepartmentSetDlg()
{
	delete m_pDisplayHelpers;
	delete m_pContentHelpers;
}

/**********************************************************************/

void CListDataDepartmentSetDlg::SetTitle()
{
	if ( TRUE == m_bSystem )
	{
		SetWindowText( "Setup System Department Sets" );
	}
	else
	{
		CString strTitle = "Setup Department Sets ";
		strTitle += dbDatabase.GetDatabaseTitle( DataManager.GetActiveDbIdx(), EcrmanOptions.GetGlobalDepartmentFlag() ); 
		SetWindowText ( strTitle );
	}
}

/**********************************************************************/

void CListDataDepartmentSetDlg::SetButtonTexts()
{
	m_button1.SetWindowText ( "&Add" );
	m_button2.SetWindowText ( "&Edit" );
	m_button3.SetWindowText ( "&Delete" );
	m_button4.SetWindowText ( "Move &Up" );
	m_button5.SetWindowText ( "Move D&own" );
	m_button6.ShowWindow ( SW_HIDE );

	MoveControl( &m_button1, 368, 7 );
	MoveControl( &m_button2, 368, 47 );
	MoveControl( &m_button3, 368, 87 );
	MoveControl( &m_button4, 368, 147 );
	MoveControl( &m_button5, 368, 187 );
}

/**********************************************************************/

void CListDataDepartmentSetDlg::SetColumns()
{
	m_pContentHelpers -> SetColumns( m_listData, this );
}

/**********************************************************************/

void CListDataDepartmentSetDlg::HandleButton1() 
{
	int nInsertPos = GetSafeSelection() + 1;
	if ( nInsertPos == 0 ) nInsertPos = m_pDeptSetArray -> GetSize();

	CDepartmentSetCSVRecord NewRecord;
	NewRecord.SetName ( "DEPARTMENT SET" );
	CDepartmentSetEditDlg dlg ( m_bSystem, NewRecord, this );
	dlg.DoModal();
	
	m_pDeptSetArray -> InsertAt ( nInsertPos, NewRecord );
	m_listData.SetItemCountEx ( GetArraySize() );
	SelectLine ( nInsertPos );

	m_WarnCancelAfterChange.SetFlag();
}

/**********************************************************************/

void CListDataDepartmentSetDlg::HandleButton2() 
{
	int nSelection = GetSafeSelection();
	if ( nSelection == -1 )
		return;
	
	CDepartmentSetCSVRecord DepartmentSetRecord;
	m_pDeptSetArray -> GetAt ( nSelection, DepartmentSetRecord );
	CDepartmentSetEditDlg dlg ( m_bSystem, DepartmentSetRecord, this );
	
	if ( dlg.DoModal() == IDOK )
	{
		m_pDeptSetArray -> SetAt ( nSelection, DepartmentSetRecord );
		m_WarnCancelAfterChange.SetFlag();
	}

	SelectLine ( nSelection );
}
		
/**********************************************************************/

void CListDataDepartmentSetDlg::HandleButton3() 
{
	m_pDisplayHelpers -> HandleDelete( this );
	m_WarnCancelAfterChange.SetFlag( m_pContentHelpers -> GetDeletionEffectFlag() );
}

/**********************************************************************/

void CListDataDepartmentSetDlg::HandleButton4()  
{
	int nSelection = GetSafeSelection ( 1, GetArraySize() - 1 );
	if ( nSelection == -1 )
		return;
	
	CDepartmentSetCSVRecord TempRecord1; 
	CDepartmentSetCSVRecord TempRecord2; 
	m_pDeptSetArray -> GetAt ( nSelection, TempRecord1 );
	m_pDeptSetArray -> GetAt ( nSelection - 1, TempRecord2 );
	m_pDeptSetArray -> SetAt ( nSelection, TempRecord2 );
	m_pDeptSetArray -> SetAt ( nSelection - 1, TempRecord1 );
	SelectLine ( nSelection - 1 );

	m_WarnCancelAfterChange.SetFlag();
}

/**********************************************************************/

void CListDataDepartmentSetDlg::HandleButton5() 
{
	int nSelection = GetSafeSelection ( 0, GetArraySize() - 2 );
	if ( nSelection == -1 ) 
		return;

	CDepartmentSetCSVRecord TempRecord1; 
	CDepartmentSetCSVRecord TempRecord2; 
	m_pDeptSetArray -> GetAt ( nSelection, TempRecord1 );
	m_pDeptSetArray -> GetAt ( nSelection + 1, TempRecord2 );
	m_pDeptSetArray -> SetAt ( nSelection, TempRecord2 );
	m_pDeptSetArray -> SetAt ( nSelection + 1, TempRecord1 );
	SelectLine ( nSelection + 1 );

	m_WarnCancelAfterChange.SetFlag();
}

/**********************************************************************/

