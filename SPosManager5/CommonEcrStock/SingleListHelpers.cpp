/**********************************************************************/
#include "ListDataDeleteDlg.h"
/**********************************************************************/
 #include "..\CommonEcrStock\SingleListHelpers.h"
/**********************************************************************/

CSingleListContentHelpers::CSingleListContentHelpers()
{
	m_bPreventDeletion = FALSE;
	m_bDeletionEffect = FALSE;
	m_nListLineColour = -1;
}

/**********************************************************************/

CSingleListContentHelpers::~CSingleListContentHelpers()
{
}

/**********************************************************************/

void CSingleListContentHelpers::FilterSelectedItemArray( CReportConsolidationArray<CSortedIntItem>& arraySelected )
{
	m_bPreventDeletion = FALSE;
}

/**********************************************************************/

int CSingleListContentHelpers::GetArrayIdxFromListIdx( int nListIdx )
{
	return nListIdx;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CSingleListDisplayHelpers::CSingleListDisplayHelpers( CSSListCtrlVirtual& list, CSingleListContentHelpers& Content ) : m_list( list ), m_Content( Content )
{
	m_strTopSingle = "";
	m_strTopPlural = "";
	m_strBottomSingle = "";
	m_strBottomPlural = "";
	m_pDeletionList = NULL;
	m_nFirstDeletedItem = 0;
}

/**********************************************************************/

CSingleListDisplayHelpers::~CSingleListDisplayHelpers(void)
{
	m_arraySelected.RemoveAll();
}

/**********************************************************************/

void CSingleListDisplayHelpers::PrepareDatabaseList( bool bMultiSelect, bool bEnableColour )
{
	m_list.SetItemCountEx ( m_Content.GetArraySize() );
	
	if ( FALSE == bMultiSelect )
		m_list.ModifyStyle( 0, LVS_SINGLESEL, 0 );

	if ( TRUE == bEnableColour )
		m_list.EnableColour();

	m_list.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );

	SelectDatabaseLine(0);
	m_list.SetFocus();
}

/**********************************************************************/

void CSingleListDisplayHelpers::PrepareDeletionList( bool bEnableColour )
{
	m_pDeletionList -> SetItemCountEx ( m_arraySelected.GetSize() );
	m_pDeletionList -> ModifyStyle( 0, LVS_SINGLESEL, 0 );

	if ( TRUE == bEnableColour )
		m_pDeletionList -> EnableColour();

	m_pDeletionList -> SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );

	SelectDeletionLine(0);
	m_pDeletionList -> SetFocus();
}

/**********************************************************************/

void CSingleListDisplayHelpers::SelectDatabaseLine ( int nIndex )
{
	m_list.Invalidate();

	for ( int n = 0; n < m_list.GetItemCount(); n++ )
	{
		if ( n != nIndex )
			m_list.SetItemState(n, 0, LVIS_SELECTED);
		else
		{
			m_list.SetItemState(n, LVIS_SELECTED, LVIS_SELECTED);
			m_list.SetSelectionMark(n);
			m_list.SetCurSel(n);
		}
	}
}

/**********************************************************************/

void CSingleListDisplayHelpers::SelectDeletionLine ( int nIndex )
{
	m_pDeletionList -> Invalidate();

	for ( int n = 0; n < m_pDeletionList -> GetItemCount(); n++ )
	{
		if ( n != nIndex )
			m_pDeletionList -> SetItemState(n, 0, LVIS_SELECTED);
		else
		{
			m_pDeletionList -> SetItemState(n, LVIS_SELECTED, LVIS_SELECTED);
			m_pDeletionList -> SetSelectionMark(n);
			m_pDeletionList -> SetCurSel(n);
		}
	}
}

/**********************************************************************/

void CSingleListDisplayHelpers::UpdateSelectedItemArray()
{
	m_arraySelected.RemoveAll();

	POSITION pos = m_list.GetFirstSelectedItemPosition();
	
	while (pos)
	{
		CSortedIntItem indexItem;
		indexItem.m_nItem = m_list.GetNextSelectedItem(pos);
		m_arraySelected.Consolidate( indexItem );
	}
}

/**********************************************************************/

int CSingleListDisplayHelpers::GetSelectedLine( int nIndex )
{
	int nResult = 0;
	if ( ( nIndex >= 0 ) && ( nIndex < m_arraySelected.GetSize() ) )
	{
		CSortedIntItem item;
		m_arraySelected.GetAt( nIndex, item );
		nResult = item.m_nItem;
	}
	return nResult;
}

/**********************************************************************/

void CSingleListDisplayHelpers::HandleDelete( CWnd* pParent )
{
	m_nFirstDeletedItem = 0;

	UpdateSelectedItemArray();
	m_Content.FilterSelectedItemArray( m_arraySelected );
	m_Content.ClearDeletionEffectFlag();

	int nSize = m_arraySelected.GetSize();
	if ( nSize > 0 )
	{
		CListDataDeleteDlg dlg( &m_Content, this, pParent );
		
		if ( dlg.DoModal() == IDOK )
		{
			for ( int n = m_arraySelected.GetSize() - 1; n >= 0; n-- )
			{
				CSortedIntItem indexItem;
				m_arraySelected.GetAt( n, indexItem );
				m_Content.DeleteDatabaseEntry( indexItem.m_nItem );
			}

			m_list.SetItemCountEx ( m_Content.GetArraySize() );

			CSortedIntItem indexItem;
			m_arraySelected.GetAt( 0, indexItem );		
			SelectDatabaseLine ( indexItem.m_nItem );
			m_nFirstDeletedItem = indexItem.m_nItem;
		}
	}

	m_arraySelected.RemoveAll();
}

/**********************************************************************/


