/**********************************************************************/
#include "BatchRepEditDlg.h"
#include "DatabaseCSVArray.h"
#include "PresetReportManager.h"
/**********************************************************************/
#include "ListDataBatchRepDlg.h"
/**********************************************************************/

CSingleListContentHelpersBatchRep::CSingleListContentHelpersBatchRep() 
{
}

/**********************************************************************/

int CSingleListContentHelpersBatchRep::GetArraySize()
{
	return PresetReportManager.EposBatch.GetSize();
}

/**********************************************************************/

void CSingleListContentHelpersBatchRep::DeleteDatabaseEntry( int nIndex )
{
	if ( ( nIndex >= 0 ) && ( nIndex < PresetReportManager.EposBatch.GetSize() ) )
		PresetReportManager.EposBatch.RemoveAt ( nIndex );
}

/**********************************************************************/

bool CSingleListContentHelpersBatchRep::PrepareLine( int nIndex, CString& strLine )
{
	strLine = "";

	bool bResult = FALSE;
	if ( ( nIndex >= 0 ) && ( nIndex < GetArraySize() ) )
	{
		int nEntityNo = 0;
		CString strName = "";
		PresetReportManager.EposBatch.GetEntityNoAndListName( nIndex, nEntityNo, strName );

		strLine.Format( "%3.3d\t%s",
			nEntityNo,
			(const char*) strName );

		bResult = TRUE;
	}

	return bResult;
}

/**********************************************************************/

void CSingleListContentHelpersBatchRep::SetColumns( CSSListCtrlVirtual& list, CWnd* pParent )
{
	list.SubclassDlgItem ( IDC_LIST, pParent );
	list.InsertColumn ( 0, "Num", LVCFMT_LEFT, 40 );
	list.InsertColumn ( 1, "Batch Name", LVCFMT_LEFT, 440 );
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CListDataBatchRepDlg::CListDataBatchRepDlg( CWnd* pParent) : CListDataDlg ( pParent )
{
	m_pContentHelpers = new CSingleListContentHelpersBatchRep();
	m_pDisplayHelpers = new CSingleListDisplayHelpers( m_listData, *m_pContentHelpers );
	m_pDisplayHelpers -> SetTopSingleText( "report batch" );
	m_pDisplayHelpers -> SetTopPluralText( "report batches" );
	m_pDisplayHelpers -> SetBottomSingleText( "batch" );
	m_pDisplayHelpers -> SetBottomPluralText( "batches" );
}

/**********************************************************************/

CListDataBatchRepDlg::~CListDataBatchRepDlg()
{
	delete m_pDisplayHelpers;
	delete m_pContentHelpers;
}

/**********************************************************************/

void CListDataBatchRepDlg::SetTitle()
{
	SetWindowText ( "Setup Batch Printing for EPOS Reports" );
}

/**********************************************************************/

void CListDataBatchRepDlg::SetButtonTexts()
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

void CListDataBatchRepDlg::SetColumns()
{
	m_pContentHelpers -> SetColumns( m_listData, this );
}

/**********************************************************************/

void CListDataBatchRepDlg::HandleButton1() 
{
	int nInsertPos = GetSafeSelection() + 1;
	if ( nInsertPos == 0 ) 
	{
		nInsertPos = PresetReportManager.EposBatch.GetSize();
	}

	CPresetReportInfo Base;
	Base.SetEntityNo( PresetReportManager.EposBatch.GetNextFreeEntityNo() );
	Base.SetName ( "BATCH REPORTS" );
	
	CBatchRepEditDlg dlg ( Base, this );
	dlg.DoModal();
	
	PresetReportManager.EposBatch.InsertAt( nInsertPos, Base );
	m_listData.SetItemCountEx ( GetArraySize() );
	SelectLine ( nInsertPos );

	m_WarnCancelAfterChange.SetFlag();
}

/**********************************************************************/

void CListDataBatchRepDlg::HandleButton2() 
{
	int nIndex = GetSafeSelection();
	if ( nIndex == -1 ) 
		return;

	CPresetReportInfo Base;
	PresetReportManager.EposBatch.GetAt ( nIndex, Base );
	CBatchRepEditDlg dlg ( Base, this );
	
	if ( dlg.DoModal() == IDOK )
	{
		PresetReportManager.EposBatch.SetAt ( nIndex, Base );
		m_WarnCancelAfterChange.SetFlag();
	}
	
	SelectLine ( nIndex );
}
		
/**********************************************************************/
	
void CListDataBatchRepDlg::HandleButton3() 
{
	int nSize = GetArraySize();

	m_pDisplayHelpers -> HandleDelete( this );

	if ( nSize != GetArraySize() )
	{
		m_WarnCancelAfterChange.SetFlag();
	}
}

/**********************************************************************/

void CListDataBatchRepDlg::HandleButton4()  
{
	int nIndex = GetSafeSelection( 1, GetArraySize() - 1 );
	if ( nIndex == -1 ) 
		return;

	PresetReportManager.EposBatch.SwapItems( nIndex - 1 );
	SelectLine ( nIndex - 1 );

	m_WarnCancelAfterChange.SetFlag();
}

/**********************************************************************/

void CListDataBatchRepDlg::HandleButton5() 
{
	int nIndex = GetSafeSelection( 0, GetArraySize() - 2 );
	if ( nIndex == -1 ) 
		return;
	
	PresetReportManager.EposBatch.SwapItems( nIndex );
	SelectLine ( nIndex + 1 );

	m_WarnCancelAfterChange.SetFlag();
}

/**********************************************************************/
