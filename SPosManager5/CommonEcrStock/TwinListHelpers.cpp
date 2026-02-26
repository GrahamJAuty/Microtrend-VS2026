/**********************************************************************/
#include "TwinListHelpers.h"
/**********************************************************************/

CTwinListHelpers::CTwinListHelpers( CSSListMultiSelectCtrlDblClick& listA, CSSListMultiSelectCtrlDblClick& listB, CReportConsolidationArray<CSortedIntWithFlag>& arrayA, CReportConsolidationArray<CSortedIntWithFlag>& arrayB ) : 
	m_listA( listA ), m_listB( listB ), m_arrayA( arrayA ), m_arrayB( arrayB )
{
	m_nMoveCount = 1;
}

/**********************************************************************/

CTwinListHelpers::~CTwinListHelpers(void)
{
}

/**********************************************************************/

CSSListMultiSelectCtrl* CTwinListHelpers::GetItemList( bool bListB )
{
	return ( bListB ) ? &m_listB : &m_listA;
}

/**********************************************************************/

CReportConsolidationArray<CSortedIntWithFlag>* CTwinListHelpers::GetItemArray( bool bListB )
{
	return ( bListB ) ? &m_arrayB : & m_arrayA;
}

/**********************************************************************/

void CTwinListHelpers::GetSelectedItemArray( bool bListB, CReportConsolidationArray<CSortedIntItem>& arrayItems )
{
	arrayItems.RemoveAll();

	POSITION pos = GetItemList( bListB ) -> GetFirstSelectedItemPosition();
	
	while (pos)
	{
		CSortedIntItem indexItem;
		indexItem.m_nItem = GetItemList( bListB ) -> GetNextSelectedItem(pos);
		arrayItems.Consolidate( indexItem );
	}
}

/**********************************************************************/

bool CTwinListHelpers::SelectSingleLine( bool bListB )
{
	bool bResult = FALSE;

	int nCurSel = GetItemList( bListB ) -> GetCurSel();
	if ( ( nCurSel >= 0 ) && ( nCurSel < GetItemList( bListB ) -> GetItemCount() ) )
	{
		SelectLine( bListB, nCurSel );
		bResult = TRUE;
	}

	return bResult;
}

/**********************************************************************/

void CTwinListHelpers::SelectLine( bool bListB, int nIndex )
{
	GetItemList( bListB ) -> Invalidate();

	for ( int n = 0; n < GetItemList( bListB ) -> GetItemCount(); n++ )
	{
		if ( n != nIndex )
			GetItemList( bListB ) -> SetItemState(n, 0, LVIS_SELECTED);
		else
		{
			GetItemList( bListB ) -> SetItemState(n, LVIS_SELECTED, LVIS_SELECTED);
			GetItemList( bListB ) -> SetSelectionMark(n);
		}
	}
}

/**********************************************************************/

void CTwinListHelpers::MoveItems( bool bFromListB )
{
	CSSListMultiSelectCtrl* pSourceList = GetItemList( bFromListB );
	CSSListMultiSelectCtrl* pDestList = GetItemList( FALSE == bFromListB );
	CReportConsolidationArray<CSortedIntWithFlag>* pSourceArray = GetItemArray( bFromListB );
	CReportConsolidationArray<CSortedIntWithFlag>* pDestArray = GetItemArray( FALSE == bFromListB );

	CReportConsolidationArray<CSortedIntItem> arrayIndex;
	GetSelectedItemArray( bFromListB, arrayIndex );

	if ( arrayIndex.GetSize() != 0 )
	{
		for ( int n = arrayIndex.GetSize() - 1; n >= 0; n-- )
		{
			CSortedIntItem indexItem;
			arrayIndex.GetAt( n, indexItem );

			CSortedIntWithFlag arrayItem;
			pSourceArray -> GetAt( indexItem.m_nItem, arrayItem );
			pSourceArray -> RemoveAt( indexItem.m_nItem );
			arrayItem.m_nFlag = m_nMoveCount;
			pDestArray -> Consolidate( arrayItem );
		}

		m_listA.SetItemCountEx( m_arrayA.GetSize() );
		m_listB.SetItemCountEx( m_arrayB.GetSize() );

		//SELECTED MOVED ITEMS IN DESTINATION LIST
		bool bGotMark = FALSE;
		for ( int n = 0; n < pDestArray -> GetSize(); n++ )
		{
			CSortedIntWithFlag arrayItem;
			pDestArray -> GetAt( n, arrayItem );

			if ( arrayItem.m_nFlag != m_nMoveCount )
				pDestList -> SetItemState(n, 0, LVIS_SELECTED);
			else
			{
				pDestList -> SetItemState(n, LVIS_SELECTED, LVIS_SELECTED);
			
				if ( FALSE == bGotMark )
				{
					pDestList -> SetSelectionMark(n);
					bGotMark = TRUE;
				}
			}
		}

		//SELECT INDEX ITEM IN SOURCE LIST
		CSortedIntItem indexItem;
		arrayIndex.GetAt( 0, indexItem );
		
		int nSourceSel = indexItem.m_nItem;
		if ( nSourceSel >= pSourceArray -> GetSize() )
			nSourceSel--;

		SelectLine( bFromListB, nSourceSel );

		m_listA.Invalidate();
		m_listB.Invalidate();
		
		m_nMoveCount++;
	}
}

/**********************************************************************/
