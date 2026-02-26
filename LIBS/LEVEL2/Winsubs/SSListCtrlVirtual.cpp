/**********************************************************************/

CSSListCtrlVirtual::CSSListCtrlVirtual()
{
	m_nCurSel = -1;
	m_bEnableColour = FALSE;
	m_bAdjustedRowHeight = FALSE;
	m_bBoldMode = FALSE;
	m_nFixedBackgroundColour = 0;
	m_nFixedForegroundColour = 0;
}

/**********************************************************************/

CSSListCtrlVirtual::~CSSListCtrlVirtual()
{
}

/**********************************************************************/

void CSSListCtrlVirtual::AdjustRowHeight( LPMEASUREITEMSTRUCT lpMeasureItemStruct, int nAdjust )
{
   if ( FALSE == m_bAdjustedRowHeight )
   {
      lpMeasureItemStruct->itemHeight += nAdjust;
	  m_bAdjustedRowHeight = TRUE;
   }
}

/**********************************************************************/

void CSSListCtrlVirtual::EnableColour()
{
	if ((GetStyle() & LVS_OWNERDRAWFIXED) != 0)
	{
		m_bEnableColour = TRUE;
	}
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CSSListCtrlVirtual, CListCtrl)
	ON_NOTIFY_REFLECT_EX(LVN_ITEMCHANGED, OnItemChanged)
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDBLCLK()
END_MESSAGE_MAP()

/**********************************************************************/

#define TAB_GAP 2

void CSSListCtrlVirtual::DrawItem ( LPDRAWITEMSTRUCT lpdis )
{
	CDC* pDC = CDC::FromHandle ( lpdis->hDC );
	CRect rect = lpdis->rcItem;
	UINT nIndex = lpdis->itemID;

	CFont* pOldFont = NULL;

	if (TRUE == m_bBoldMode)
	{
		CFont Font;
		GetBoldFont(pDC, Font);
		pOldFont = pDC->SelectObject(&Font);
	}

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
			dwColText  = ( m_bEnableColour == TRUE ) ? GetColour(nIndex) : COLOR_WINDOWTEXT;
		}

		if (m_nFixedBackgroundColour != 0)
		{
			dwColBrush = m_nFixedBackgroundColour;
		}

		if (m_nFixedForegroundColour != 0)
		{
			dwColText = m_nFixedForegroundColour;
		}

		CBrush Brush ( dwColBrush );
		pDC->FillRect ( rect, &Brush );

		if ((m_nFixedBackgroundColour == 0) && (m_nFixedForegroundColour == 0))
		{
			if (lpdis->itemState & ODS_FOCUS)
			{
				pDC->DrawFocusRect(rect);
			}
		}
	}
	else										// disabled
	{
		dwColBrush = ::GetSysColor ( COLOR_SCROLLBAR );
		dwColText  = ( m_bEnableColour == TRUE ) ? GetColour(nIndex) : COLOR_WINDOWTEXT;

		if (m_nFixedBackgroundColour != 0)
		{
			dwColBrush = m_nFixedBackgroundColour;
		}

		if (m_nFixedForegroundColour != 0)
		{
			dwColText = m_nFixedForegroundColour;
		}

		CBrush Brush ( dwColBrush );
		pDC->FillRect ( rect, &Brush );
	}

	if ( nIndex != (UINT) -1 )
	{
		DWORD dwOldTextColour = pDC->SetTextColor ( dwColText );
		int nOldBkMode = pDC->SetBkMode ( TRANSPARENT );

		CRect rcItem = rect;
		rcItem.right = rcItem.left;

		LV_COLUMN lvc{};
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

	if (TRUE == m_bBoldMode)
	{
		pDC->SelectObject(pOldFont);
	}
}

/**********************************************************************/

int CSSListCtrlVirtual::GetCurSel()
{
	return m_nCurSel;
}

/**********************************************************************/

int CSSListCtrlVirtual::SetCurSel ( int nSelect )
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

DWORD CSSListCtrlVirtual::GetColour ( int nIndex )
{
	if (nIndex < m_crColourArray.GetSize())
	{
		return m_crColourArray.GetAt(nIndex);
	}

	return 0;
}

/**********************************************************************/

static COLORREF GetRGBColour ( BYTE ucColour )
{
	COLORREF crColour;

	switch ( ucColour & 0x0f )
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

void CSSListCtrlVirtual::SetColour ( int nIndex, int nColour )
{
	SetColour ( nIndex, GetRGBColour(nColour) );
}

void CSSListCtrlVirtual::SetColour ( int nIndex, COLORREF crColour )
{
	if ( nIndex >= 0 && nIndex < GetItemCount() && m_bEnableColour == TRUE )
		m_crColourArray.SetAtGrow ( nIndex, crColour );
}

/**********************************************************************/

void CSSListCtrlVirtual::SetItemCountEx ( int nCount, DWORD dwFlags )
{
	CListCtrl::SetItemCountEx ( nCount, dwFlags );

	m_crColourArray.SetSize ( nCount );
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

BOOL CSSListCtrlVirtual::OnItemChanged(NMHDR* pNMHDR, LRESULT* pResult) 
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

void CSSListCtrlVirtual::OnLButtonDown(UINT nFlags, CPoint point)
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

void CSSListCtrlVirtual::OnLButtonDblClk(UINT nFlags, CPoint point)
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

void CSSListCtrlVirtual::OnRButtonDown(UINT nFlags, CPoint point)
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

void CSSListCtrlVirtual::OnRButtonDblClk(UINT nFlags, CPoint point)
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

void CSSListCtrlVirtual::GetBoldFont(CDC* pDC, CFont& Font)
{
	pDC->SetMapMode(MM_TEXT);

	LOGFONT lf;
	lf.lfHeight = 100;
	lf.lfWidth = 0;
	lf.lfEscapement = 0;
	lf.lfOrientation = 0;
	lf.lfWeight = FW_BOLD;
	lf.lfItalic = FALSE;
	lf.lfUnderline = FALSE;
	lf.lfStrikeOut = FALSE;
	lf.lfCharSet = ANSI_CHARSET;
	lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
	lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	lf.lfQuality = DEFAULT_QUALITY;
	lf.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
	strcpy_s(lf.lfFaceName, "Tahoma");

	Font.CreatePointFontIndirect(&lf, pDC);
}

/**********************************************************************/