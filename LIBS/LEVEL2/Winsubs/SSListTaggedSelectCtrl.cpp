/**********************************************************************/
#define TAB_GAP 2
/**********************************************************************/

CSSListTaggedSelectItem::CSSListTaggedSelectItem( int nTagStateType )
{
	m_strText = "";
	m_nData = 0;
	m_nTagState = 0;
	m_bLocked = FALSE;

	switch( nTagStateType )
	{
	case TAGSTATETYPE_2STATEGREEN:
	case TAGSTATETYPE_2STATEBLUE:
	case TAGSTATETYPE_3STATEBLUEGREEN:
		m_nTagStateType = nTagStateType;
		break;

	default:
		m_nTagStateType = TAGSTATETYPE_2STATEGREEN;
		break;
	}
}

/**********************************************************************/

void CSSListTaggedSelectItem::SetTagState(int nState)
{
	int nMaxState = 1;
	if (TAGSTATETYPE_3STATEBLUEGREEN == m_nTagStateType)
	{
		nMaxState = 2;
	}

	if ((nState >= 0) && (nState <= nMaxState))
	{
		m_nTagState = nState;
	}
}

/**********************************************************************/

void CSSListTaggedSelectItem::NextTagState()
{
	int nMaxState = 1;
	if (TAGSTATETYPE_3STATEBLUEGREEN == m_nTagStateType)
	{
		nMaxState = 2;
	}

	m_nTagState++;
	if ((m_nTagState < 0) || (m_nTagState > nMaxState))
	{
		m_nTagState = 0;
	}
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

BEGIN_MESSAGE_MAP(CSSListTaggedSelectCtrl, CSSListCtrl)
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDBLCLK()
END_MESSAGE_MAP()

/**********************************************************************/

CSSListTaggedSelectCtrl::CSSListTaggedSelectCtrl() : CSSListCtrl()
{
	m_bUseExternalArray = FALSE;
	m_pExternalArrayItems = NULL;
	m_bLockItemZero = FALSE;
	m_bTagStateChanged = FALSE;
	m_pTagStateNotifyWnd = NULL;
	m_nTagStateNotifyMessage = 0;
	m_nTaggedItemCount = 0;
	m_bMultiColumns = FALSE;
}

/**********************************************************************/

void CSSListTaggedSelectCtrl::SetTagStateNotifyMessage(CWnd* pWnd, int nMessage)
{
	m_pTagStateNotifyWnd = pWnd;
	m_nTagStateNotifyMessage = nMessage;
}

/**********************************************************************/

void CSSListTaggedSelectCtrl::SendTagStateNotifyMessage()
{
	if (m_pTagStateNotifyWnd != NULL)
	{
		m_pTagStateNotifyWnd->SendMessage(m_nTagStateNotifyMessage, 0, 0);
	}
}

/**********************************************************************/

void CSSListTaggedSelectCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_bTagStateChanged = FALSE;
    ClickHandler( nFlags, point );
	CSSListCtrl::OnLButtonDown( nFlags, point );
}

/**********************************************************************/

void CSSListTaggedSelectCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	m_bTagStateChanged = FALSE;
	ClickHandler( nFlags, point );
	CSSListCtrl::OnLButtonDblClk( nFlags, point );
}

/**********************************************************************/

void CSSListTaggedSelectCtrl::OnRButtonDown(UINT nFlags, CPoint point)
{
	m_bTagStateChanged = FALSE;
	ClickHandler( nFlags, point );
	CSSListCtrl::OnRButtonDown( nFlags, point );
}

/**********************************************************************/

void CSSListTaggedSelectCtrl::OnRButtonDblClk(UINT nFlags, CPoint point)
{  
	m_bTagStateChanged = FALSE;
	ClickHandler( nFlags, point );
	CSSListCtrl::OnRButtonDblClk( nFlags, point );
}

/**********************************************************************/
    
void CSSListTaggedSelectCtrl::SetUseExternalArray(bool bUseExternalArray)
{
    m_bUseExternalArray = bUseExternalArray;
}

/**********************************************************************/

void CSSListTaggedSelectCtrl::SetExternalArray(CArray<CSSListTaggedSelectItem, CSSListTaggedSelectItem>* pExternalArray)
{
    m_pExternalArrayItems = pExternalArray;
}

/**********************************************************************/

CArray<CSSListTaggedSelectItem, CSSListTaggedSelectItem>& CSSListTaggedSelectCtrl::GetArray()
{
    return m_bUseExternalArray && m_pExternalArrayItems ? *m_pExternalArrayItems : m_InternalArrayItems;
}

/**********************************************************************/

void CSSListTaggedSelectCtrl::FindArrayItem(CSSListTaggedSelectItem& item, int& nPos)
{
    CString strText = item.m_strText;
    strText.MakeUpper();

    int nStart = 0;
    int nEnd = GetArray().GetSize() - 1;

    while (nStart <= nEnd)
    {
        nPos = (nStart + nEnd) / 2;
        CString strArrayText = GetArray()[nPos].m_strText;
        strArrayText.MakeUpper();

        if (strArrayText < strText)
        {
            nStart = nPos + 1;
        }
        else if (strArrayText > strText)
        {
            nEnd = nPos - 1;
        }
        else
        {
            return;
        }
    }

    nPos = nStart;
}

/**********************************************************************/

void CSSListTaggedSelectCtrl::AddItemToList(CSSListTaggedSelectItem& item)
{
    GetArray().Add(item);

    if (FALSE == m_bUseExternalArray)
    {
        int nPos = GetItemCount();
        InsertItem(nPos, "");
        SetItem(nPos, 0, LVIF_TEXT, "TEXT", 0, 0, 0, NULL);
    }

    if (item.GetTagState() != 0)
    {
        m_nTaggedItemCount++;
        SendTagStateNotifyMessage();
    }
}

/**********************************************************************/

void CSSListTaggedSelectCtrl::AddSortedItemToList(CSSListTaggedSelectItem& item)
{
    int nPos = 0;
    FindArrayItem(item, nPos);
    InsertItemIntoList(nPos, item);
}

/**********************************************************************/

void CSSListTaggedSelectCtrl::InsertItemIntoList(int nPos, CSSListTaggedSelectItem& item)
{
    GetArray().InsertAt(nPos, item);

	if (FALSE == m_bUseExternalArray)
	{
		InsertItem(nPos, "");
		SetItem(nPos, 0, LVIF_TEXT, "TEXT", 0, 0, 0, NULL);
	}

    if (item.GetTagState() != 0)
    {
        m_nTaggedItemCount++;
        SendTagStateNotifyMessage();
    }
}

/**********************************************************************/

void CSSListTaggedSelectCtrl::GetListItem(int nIndex, CSSListTaggedSelectItem& item)
{
    if ((nIndex >= 0) && (nIndex < GetArray().GetSize()))
    {
        item = GetArray().GetAt(nIndex);
    }
}

/**********************************************************************/

void CSSListTaggedSelectCtrl::SetListItem(int nIndex, CSSListTaggedSelectItem& item)
{
    UpdateListItemInternal(nIndex, item);
}

/**********************************************************************/

void CSSListTaggedSelectCtrl::UpdateListItemInternal(int nIndex, CSSListTaggedSelectItem& itemNew)
{
    if ((nIndex >= 0) && (nIndex < GetArray().GetSize()))
    {
        CSSListTaggedSelectItem itemOld = GetArray().GetAt(nIndex);
        GetArray().SetAt(nIndex, itemNew);

        int nTagStateOld = itemOld.GetTagState();
        int nTagStateNew = itemNew.GetTagState();

        if ((nTagStateNew != 0) && (0 == nTagStateOld))
        {
            m_nTaggedItemCount++;
            SendTagStateNotifyMessage();
        }
        else if ((nTagStateOld != 0) && (0 == nTagStateNew))
        {
            m_nTaggedItemCount--;
            SendTagStateNotifyMessage();
        }
    }
}

/**********************************************************************/

void CSSListTaggedSelectCtrl::ClickHandler(UINT nFlags, CPoint point)
{
	int nMinItem = ( m_bLockItemZero ) ? 1 : 0;
	int nMaxItem = GetArray().GetSize() - 1;

	UINT uHitFlags = 0;   
	int nItem = HitTest(point, &uHitFlags);
	
	if ( ( ( uHitFlags & LVHT_NOWHERE ) == FALSE ) && ( nItem >= nMinItem ) && ( nItem <= nMaxItem ) && ( point.x < 20 ) )
	{
		if ( GetArray()[nItem].m_bLocked == FALSE )
		{
			int nOldTagState = GetArray()[nItem].GetTagState();
			GetArray()[nItem].NextTagState();
			int nNewTagState = GetArray()[nItem].GetTagState();
			Update( nItem );
			m_bTagStateChanged = TRUE;

			if ((0 == nOldTagState) && (nNewTagState != 0))
			{
				m_nTaggedItemCount++;
				SendTagStateNotifyMessage();
			}
			else if ((0 == nNewTagState) && (nOldTagState != 0))
			{
				m_nTaggedItemCount--;
				SendTagStateNotifyMessage();
			}
		}
	}
}

/**********************************************************************/

void CSSListTaggedSelectCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
    CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
    CRect rect = lpDrawItemStruct->rcItem;
    int nItemIdx = lpDrawItemStruct->itemID;

    pDC->FillSolidRect(rect.left, rect.top, rect.Width(), rect.Height(), COLORREF(0xFFFFFF));

    bool bSelected = (GetItemState(nItemIdx, LVIS_SELECTED) != 0);

    if (TRUE == bSelected)
    {
        pDC->FillSolidRect(rect.left + 20, rect.top, rect.Width() - 20, rect.Height(), COLORREF(0xFF901E));
    }

    if ((nItemIdx >= 0) && (nItemIdx < GetArray().GetSize()))
    {
        CSSListTaggedSelectItem ListItem = GetArray().GetAt(nItemIdx);

        bool bLockedItem = FALSE;
        bLockedItem |= ((0 == nItemIdx) && (m_bLockItemZero));
        bLockedItem |= ListItem.m_bLocked;

        int nGreen = 0x66FF00;
        int nBlue = 0xFF6600;

        if (TRUE == bLockedItem)
        {
            nGreen = 0x55CC00;
            nBlue = 0xCC5500;
        }

        switch (ListItem.GetTagState())
        {
        case 2:
            switch (ListItem.GetTagStateType())
            {
            case TAGSTATETYPE_3STATEBLUEGREEN:
                pDC->FillSolidRect(rect.left + 4, rect.top + 6, 12, rect.Height() - 12, COLORREF(nGreen));
                break;
            }
            break;

        case 1:
            switch (ListItem.GetTagStateType())
            {
            case TAGSTATETYPE_2STATEGREEN:
                pDC->FillSolidRect(rect.left + 4, rect.top + 6, 12, rect.Height() - 12, COLORREF(nGreen));
                break;

            case TAGSTATETYPE_2STATEBLUE:
            case TAGSTATETYPE_3STATEBLUEGREEN:
                pDC->FillSolidRect(rect.left + 4, rect.top + 6, 12, rect.Height() - 12, COLORREF(nBlue));
                break;
            }
            break;
        }

        {
            CBrush Brush;
            Brush.CreateSolidBrush(0x00);

            CRect rectBorder(rect.left + 4, rect.top + 6, rect.left + 16, rect.bottom - 6);

            CRgn region;
            region.CreateRectRgnIndirect(&rectBorder);
            pDC->FrameRgn(&region, &Brush, 1, 1);
            Brush.DeleteObject();
        }

        CString strText = "";
        if (FALSE == m_bUseExternalArray)
        {
            strText = ListItem.m_strText;
        }

        if (TRUE == bSelected)
        {
            pDC->SetTextColor(COLORREF(0xFFFFFF));
        }
        else if (TRUE == bLockedItem)
        {
            pDC->SetTextColor(COLORREF(0x777777));
        }
        else
        {
            pDC->SetTextColor(COLORREF(0x0));
        }

        CCSV csvText(strText, '|');

        CRect rcItem = rect;
        rcItem.right = rcItem.left;

        LV_COLUMN lvc;
        lvc.mask = LVCF_FMT | LVCF_WIDTH;

        int nColumn = 0;
        while (GetColumn(nColumn, &lvc) == TRUE)
        {
            CString strCellText = "";
            if (m_bUseExternalArray)
            {
                strCellText = GetItemText(nItemIdx, nColumn);
            }
            else if (FALSE == m_bMultiColumns)
            {
                strCellText = strText;
            }
            else
            {
                strCellText = csvText.GetString(nColumn);
            }

            rcItem.left = rcItem.right;
            rcItem.right += lvc.cx;

            if (0 == nColumn)
            {
                rcItem.left += 24;
            }

            int nAlign = 0;
            switch (lvc.fmt & LVCFMT_JUSTIFYMASK)
            {
            case LVCFMT_LEFT: nAlign = TA_LEFT; break;
            case LVCFMT_RIGHT: nAlign = TA_RIGHT; break;
            case LVCFMT_CENTER: nAlign = TA_CENTER; break;
            default: nAlign = TA_LEFT; break;
            }

            CRect rcLabel;
            rcLabel = rcItem;
            rcLabel.left += TAB_GAP;
            rcLabel.right -= TAB_GAP;
            rcLabel.top += 4;

            DrawListText(pDC, strCellText, rcLabel, nAlign, -2);

            nColumn++;
        }
    }
}

/**********************************************************************/

void CSSListTaggedSelectCtrl::SwapItems( int nIndex1, int nIndex2 )
{
    if (nIndex1 == nIndex2)
    {
        return;
    }

    if ((nIndex1 < 0) || (nIndex1 >= GetItemCount()) || (nIndex1 >= GetArray().GetSize()))
    {
        return;
    }

    if ((nIndex2 < 0) || (nIndex2 >= GetItemCount()) || (nIndex2 >= GetArray().GetSize()))
    {
        return;
    }

	CSSListTaggedSelectItem item1 = GetArray().GetAt(nIndex1);
	CSSListTaggedSelectItem item2 = GetArray().GetAt(nIndex2);

	GetArray().SetAt(nIndex1, item2);
	GetArray().SetAt(nIndex2, item1);

	SetCurSel( nIndex2 );

	Update( nIndex1 );
	Update( nIndex2 );
}

/**********************************************************************/

void CSSListTaggedSelectCtrl::ClearList()
{
	DeleteAllItems();
	GetArray().RemoveAll();
	m_nTaggedItemCount = 0;
	SendTagStateNotifyMessage();
}

/**********************************************************************/

void CSSListTaggedSelectCtrl::SetItemTagState(int nIndex, int nTag)
{
	if ((nIndex >= 0) && (nIndex < GetArray().GetSize()))
	{
		CSSListTaggedSelectItem item = GetArray().GetAt(nIndex);
		item.SetTagState(nTag);
		UpdateListItemInternal(nIndex, item);
	}
}

/**********************************************************************/

int CSSListTaggedSelectCtrl::GetItemTagState( int nIndex )
{
	if ((nIndex >= 0) && (nIndex < GetArray().GetSize()))
	{
		return GetArray()[nIndex].GetTagState();
	}
	else
	{
		return FALSE;
	}
}

/**********************************************************************/

void CSSListTaggedSelectCtrl::SelectAll()
{
	SelectAllInternal(TRUE);
}

/**********************************************************************/

void CSSListTaggedSelectCtrl::SelectNone()
{
	SelectAllInternal(FALSE);
}

/**********************************************************************/

void CSSListTaggedSelectCtrl::SelectAllInternal( bool bSelect )
{
	m_nTaggedItemCount = 0;

	for ( int nIndex = 0; nIndex < GetArray().GetSize(); nIndex++)
	{
		if ( TRUE == GetArray()[nIndex].m_bLocked )
		{
			if (GetArray()[nIndex].GetTagState() != 0)
			{
				m_nTaggedItemCount++;
			}

			continue;
		}

		if ( ( 0 == nIndex ) && ( TRUE == m_bLockItemZero ) )
		{
			if (GetArray()[nIndex].GetTagState() != 0)
			{
				m_nTaggedItemCount++;
			}

			continue;
		}

		if (TRUE == bSelect)
		{
			GetArray()[nIndex].SetTagState(1);
			m_nTaggedItemCount++;
		}
		else
		{
			GetArray()[nIndex].SetTagState(0);

		}
	}
	Invalidate();
}

/**********************************************************************/