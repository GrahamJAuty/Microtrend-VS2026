/**********************************************************************/

CSSListCtrl::CSSListCtrl()
{
	m_nCurSel = -1;
	m_bEnableColour = FALSE;
	m_bAdjustedRowHeight = FALSE;
}

/**********************************************************************/

CSSListCtrl::~CSSListCtrl()
{
}

/**********************************************************************/

void CSSListCtrl::AdjustRowHeight( LPMEASUREITEMSTRUCT lpMeasureItemStruct, int nAdjust )
{
   if ( FALSE == m_bAdjustedRowHeight )
   {
      lpMeasureItemStruct->itemHeight += nAdjust;
	  m_bAdjustedRowHeight = TRUE;
   }
}

/**********************************************************************/

void CSSListCtrl::EnableColour()
{
	if (( GetStyle() & LVS_OWNERDRAWFIXED ) != 0 )
		m_bEnableColour = TRUE;
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CSSListCtrl, CListCtrl)
	ON_NOTIFY_REFLECT_EX(LVN_ITEMCHANGED, OnItemChanged)
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDBLCLK()
END_MESSAGE_MAP()

/**********************************************************************/
#define TAB_GAP 2
/**********************************************************************/

void CSSListCtrl::DrawItem ( LPDRAWITEMSTRUCT lpdis )
{
	CDC* pDC = CDC::FromHandle ( lpdis->hDC );
	CRect rect = lpdis->rcItem;
	UINT nIndex = lpdis->itemID;

	DWORD dwColBrush;
	DWORD dwColText;

	if ( IsWindowEnabled() == TRUE )				// enabled
	{
		if ( lpdis->itemState & ODS_SELECTED )		// selected
		{
			dwColBrush = ::GetSysColor ( COLOR_HIGHLIGHT );
			dwColText  = ::GetSysColor ( COLOR_HIGHLIGHTTEXT );
		}
		else									// not selected
		{
			dwColBrush = ::GetSysColor ( COLOR_WINDOW );
			dwColText  = ( m_bEnableColour == TRUE ) ? lpdis->itemData : COLOR_WINDOWTEXT;
		}

		CBrush Brush ( dwColBrush );
		pDC->FillRect ( rect, &Brush );

		if ( lpdis->itemState & ODS_FOCUS )
			pDC->DrawFocusRect ( rect );
	}
	else										// disabled
	{
		dwColBrush = ::GetSysColor ( COLOR_SCROLLBAR );
		dwColText  = ( m_bEnableColour == TRUE ) ? lpdis->itemData : COLOR_WINDOWTEXT;

		CBrush Brush ( dwColBrush );
		pDC->FillRect ( rect, &Brush );
	}

	if ( nIndex != (UINT) -1 )
	{
		DWORD dwOldTextColour = pDC->SetTextColor ( dwColText );
		int nOldBkMode = pDC->SetBkMode ( TRANSPARENT );

		CRect rcItem = rect;
		rcItem.right = rcItem.left;

		LV_COLUMN lvc;
		lvc.mask = LVCF_FMT | LVCF_WIDTH;

		int nColumn = 0;
		while ( GetColumn ( nColumn, &lvc ) == TRUE )
		{
			rcItem.left = rcItem.right;
			rcItem.right += lvc.cx;

			/*
			UINT nAlign = DT_SINGLELINE | DT_NOPREFIX | DT_NOCLIP | DT_VCENTER;
			switch ( lvc.fmt & LVCFMT_JUSTIFYMASK )
			{
			case LVCFMT_LEFT:	nAlign |= DT_LEFT;		break;
			case LVCFMT_RIGHT:	nAlign |= DT_RIGHT;		break;
			case LVCFMT_CENTER:	nAlign |= DT_CENTER;	break;
			default:			nAlign |= DT_LEFT;		break;
			}
			*/

			int nAlign = 0;
			switch (lvc.fmt & LVCFMT_JUSTIFYMASK)
			{
			case LVCFMT_LEFT:	nAlign = TA_LEFT;		break;
			case LVCFMT_RIGHT:	nAlign = TA_RIGHT;		break;
			case LVCFMT_CENTER:	nAlign = TA_CENTER;		break;
			default:			nAlign = TA_LEFT;		break;
			}

			CRect rcLabel;
			rcLabel = rcItem;
			rcLabel.left += TAB_GAP;
			rcLabel.right -= TAB_GAP;

			DrawListText(pDC, GetItemText(nIndex, nColumn), rcLabel, nAlign);

			nColumn++;
		}

		pDC->SetTextColor ( dwOldTextColour );
		pDC->SetBkMode ( nOldBkMode );
	}
}

/**********************************************************************/

int CSSListCtrl::GetCurSel()
{
	return m_nCurSel;
}

/**********************************************************************/

int CSSListCtrl::SetCurSel ( int nSelect )
{
	if ( nSelect >= 0 && nSelect < GetItemCount() )
	{
		if ( EnsureVisible ( nSelect, FALSE ) != 0 )
		{
			if ( SetItemState ( nSelect, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED ) != 0 )
			{
				m_nCurSel = nSelect;

				return nSelect;
			}
		}
	}
	return -1;
}

/**********************************************************************/

static COLORREF GetRGBColour ( int nColour )
{
	COLORREF crColour;

	switch ( nColour & 0x0f )
	{
	case 0x00:	crColour = RGB (   0,  0,  0 );	break;
	case 0x01:	crColour = RGB (   0,  0,128 );	break;
	case 0x02:	crColour = RGB (   0,128,  0 );	break;
	case 0x03:	crColour = RGB (   0,128,128 );	break;
	case 0x04:	crColour = RGB ( 128,  0,  0 );	break;
	case 0x05:	crColour = RGB ( 128,  0,128 );	break;
	case 0x06:	crColour = RGB ( 128,128,  0 );	break;
	case 0x07:	crColour = RGB ( 128,128,128 );	break;
	case 0x08:	crColour = RGB (   0,  0,  0 );	break;
	case 0x09:	crColour = RGB (   0,  0,192 );	break;
	case 0x0a:	crColour = RGB (   0,192,  0 );	break;
	case 0x0b:	crColour = RGB (   0,192,192 );	break;
	case 0x0c:	crColour = RGB ( 192,  0,  0 );	break;
	case 0x0d:	crColour = RGB ( 192,  0,192 );	break;
	case 0x0e:	crColour = RGB ( 192,192,  0 );	break;
	case 0x0f:	crColour = RGB ( 192,192,192 );	break;
	default:		crColour = RGB (   0,  0,  0 );	break;
	}

	return crColour;
}

/**********************************************************************/

void CSSListCtrl::SetColour ( int nIndex, int nColour )
{
	SetColour ( nIndex, GetRGBColour(nColour) );
}

void CSSListCtrl::SetColour ( int nIndex, COLORREF crColour )
{
	if (nIndex >= 0 && nIndex < GetItemCount() && m_bEnableColour == TRUE)
	{
		SetItemData(nIndex, crColour);
	}
}

/**********************************************************************/

int CSSListCtrl::AddString ( const char* szText, int nColour )
{
	return AddString ( szText, GetRGBColour(nColour) );
}

int CSSListCtrl::AddString ( const char* szText, COLORREF crColour )
{
	CCSV csv ( szText, '\t' );

	int nIndex = GetItemCount();

	int nReply = InsertItem ( nIndex, csv.GetString(0) );

	if ( nReply != -1 )
	{
		for (int i = 1; i < csv.GetSize(); i++)
		{
			SetItemText(nIndex, i, csv.GetString(i));
		}

		SetColour ( nIndex, crColour );
	}

	return nReply;
}

/**********************************************************************/

int CSSListCtrl::InsertString ( int nIndex, const char* szText, int nColour )
{
	return InsertString ( nIndex, szText, GetRGBColour(nColour) );
}

int CSSListCtrl::InsertString ( int nIndex, const char* szText, COLORREF crColour )
{
	if ( nIndex >= 0 && nIndex < GetItemCount() )
	{
		CCSV csv ( szText, '\t' );

		int nReply = InsertItem ( nIndex, csv.GetString(0) );

		if ( nReply != -1 )
		{
			for (int i = 1; i < csv.GetSize(); i++)
			{
				SetItemText(nIndex, i, csv.GetString(i));
			}

			SetColour ( nIndex, crColour );
		}

		return nReply;
	}
	else
	{
		return AddString(szText, crColour);
	}
}

/**********************************************************************/

int CSSListCtrl::DeleteString ( int nIndex )
{
	if ( nIndex >= 0 && nIndex < GetItemCount() )
	{
		if (DeleteItem(nIndex) != 0)
		{
			return GetItemCount();
		}
	}

	return -1;
}

/**********************************************************************/

bool CSSListCtrl::DeleteAllItems()
{
	m_nCurSel = -1;

	return CListCtrl::DeleteAllItems() != 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

BOOL CSSListCtrl::OnItemChanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	
	if ((pNMListView->uNewState & LVIS_SELECTED) != 0)
	{
		m_nCurSel = pNMListView->iItem;
	}

	*pResult = 0;

//	TRUE, the message will go no further, just as in normal message reflection. 
//	But if it returns FALSE, the control's parent will get a crack at the 
//	message also.

	return FALSE;
}

/**********************************************************************/

void CSSListCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
    UINT uHitFlags = 0;
    int nItem = HitTest(point, &uHitFlags);

    if (uHitFlags & LVHT_NOWHERE)
    {
        // eat the message by just returning
        return;
    }

    CListCtrl::OnLButtonDown(nFlags, point);
}

/**********************************************************************/

void CSSListCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    UINT uHitFlags = 0;
    int nItem = HitTest(point, &uHitFlags);

    if (uHitFlags & LVHT_NOWHERE)
    {
        // eat the message by just returning
        return;
    }

	CListCtrl::OnLButtonDblClk(nFlags, point);
}

/**********************************************************************/

void CSSListCtrl::OnRButtonDown(UINT nFlags, CPoint point)
{
    UINT uHitFlags = 0;
    int nItem = HitTest(point, &uHitFlags);

    if (uHitFlags & LVHT_NOWHERE)
    {
        // eat the message by just returning
        return;
    }

    CListCtrl::OnRButtonDown(nFlags, point);
}

/**********************************************************************/

void CSSListCtrl::OnRButtonDblClk(UINT nFlags, CPoint point)
{
    UINT uHitFlags = 0;
    int nItem = HitTest(point, &uHitFlags);

    if (uHitFlags & LVHT_NOWHERE)
    {
        // eat the message by just returning
        return;
    }

    CListCtrl::OnRButtonDblClk(nFlags, point);
}

/**********************************************************************/