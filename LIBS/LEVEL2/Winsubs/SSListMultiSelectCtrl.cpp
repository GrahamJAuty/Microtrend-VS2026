/**********************************************************************/

CSSListMultiSelectCtrl::CSSListMultiSelectCtrl() : CSSListCtrl()
{
}

/**********************************************************************/

void CSSListMultiSelectCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
    ClickHandler( nFlags, point, FALSE );
}

/**********************************************************************/

void CSSListMultiSelectCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	HandleLeftDoubleClick( nFlags, point );
}

/**********************************************************************/

void CSSListMultiSelectCtrl::OnRButtonDown(UINT nFlags, CPoint point)
{
	ClickHandler( nFlags, point, FALSE );
}

/**********************************************************************/

void CSSListMultiSelectCtrl::OnRButtonDblClk(UINT nFlags, CPoint point)
{  
	ClickHandler( nFlags, point, FALSE );
}

/**********************************************************************/

void CSSListMultiSelectCtrl::HandleLeftDoubleClick(UINT nFlags, CPoint point)
{
	ClickHandler( nFlags, point, FALSE );
}

/**********************************************************************/

void CSSListMultiSelectCtrl::ClickHandler(UINT nFlags, CPoint point, bool bPassThrough )
{
	UINT uHitFlags = 0;   
	int nItem = HitTest(point, &uHitFlags);

	if ( ( ( uHitFlags & LVHT_NOWHERE ) == FALSE ) && ( nItem >= 0 ) )
	{
		if ( GetItemState( nItem, LVIS_SELECTED ) != 0 )
		{
			SetItemState( nItem, 0, LVIS_SELECTED );
		}
		else
		{
			SetItemState( nItem, LVIS_SELECTED, LVIS_SELECTED );
			SetSelectionMark( nItem );
		}

		if ( TRUE == bPassThrough )
		{
			CListCtrl::OnLButtonDblClk(nFlags, point);
		}
	}
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

void CSSListMultiSelectCtrlDblClick::HandleLeftDoubleClick(UINT nFlags, CPoint point)
{
	ClickHandler( nFlags, point, TRUE );
}

/**********************************************************************/
