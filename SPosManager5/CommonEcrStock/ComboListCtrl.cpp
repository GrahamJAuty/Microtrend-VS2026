/**********************************************************************/
#include "InPlaceComboNormal.h"
#include "InPlaceComboTabbed.h"
#include "InPlaceEdit.h"
/**********************************************************************/
#include "..\CommonEcrStock\ComboListCtrl.h"
/**********************************************************************/
#define FIRST_COLUMN				0
#define MIN_COLUMN_WIDTH			10
#define MAX_DROP_DOWN_ITEM_COUNT	20
/**********************************************************************/

CellTypeInfoTextItem::CellTypeInfoTextItem()
{
	m_strCellText = "";
	m_strListText = "";
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CellTypeInfoTextBuffer::CellTypeInfoTextBuffer()
{
}

/**********************************************************************/

void CellTypeInfoTextBuffer::GetItem( int n, CellTypeInfoTextItem& item )
{
	if ( ( n >= 0 ) && ( n < GetSize() ) )
		item = m_arrayItems.GetAt( n );
}

/**********************************************************************/

void CellTypeInfoTextBuffer::AddNoChangeHeader( bool bTabbed )
{
	CellTypeInfoTextItem item;

	item.m_strCellText = "";
	item.m_strListText = ( bTabbed ) ? "," : "";
	item.m_strListText += "<No Change>";

	m_arrayItems.Add( item );
}

/**********************************************************************/

void CellTypeInfoTextBuffer::AddYesNoTexts()
{
	AddNoChangeHeader( TRUE );
	AddTabbedText( "Yes" );
	AddTabbedText( "No" );
}

/**********************************************************************/

void CellTypeInfoTextBuffer::AddText( const char* szText )
{
	CellTypeInfoTextItem item;
	item.m_strListText = szText;
	item.m_strCellText = szText;
	m_arrayItems.Add( item );
}

/**********************************************************************/

void CellTypeInfoTextBuffer::AddText( const char* szList, const char* szCell )
{
	CellTypeInfoTextItem item;
	item.m_strListText = szList;
	item.m_strCellText = szCell;
	m_arrayItems.Add( item );
}

/**********************************************************************/

void CellTypeInfoTextBuffer::AddTabbedText( const char* szText )
{
	CString strComma = ",";
	CellTypeInfoTextItem item;
	item.m_strListText = strComma + szText;
	item.m_strCellText = szText;
	m_arrayItems.Add( item );
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CellTypeInfo::CellTypeInfo()
{
	m_nCol = 0;
	m_nRow = 0;
	m_nType = CELL_TYPE_COMBO_NORMAL;
	//EDIT CONTROL
	m_nSSEditType = 0;
	m_nSSEditWidth = 0;
	m_strSSEditFormat = "";
	//COMBO CONTROL
	m_strComboTabs = "";
	m_strComboListText = "";
	m_strComboCellText = "";
	m_nComboSelect = 0;
	//CURRENT VALUE
	m_strCurrent = "";
	//VALIDATION
	m_nMinInt = 0;
	m_nMaxInt = 0;
	m_dMinDouble = 0.0;
	m_dMaxDouble = 0.0;
}

/**********************************************************************/

void CellTypeInfo::ResetForCombo( CellTypeInfoTextBuffer& arrayTexts )
{
	m_nType = ( m_strComboTabs != "" ) ? CELL_TYPE_COMBO_TABBED : CELL_TYPE_COMBO_NORMAL;
	m_nMinInt = 0;
	m_nMaxInt = arrayTexts.GetSize() - 1;
	m_dMinDouble = 0.0;
	m_dMaxDouble = 0.0;
	
	CCSV csvList;
	CCSV csvCell;

	for ( int n = 0; n < arrayTexts.GetSize(); n++ )
	{
		CellTypeInfoTextItem item;
		arrayTexts.GetItem( n, item );
		csvList.Add( item.m_strListText );
		csvCell.Add( item.m_strCellText );
	}

	m_strComboListText = csvList.GetLine();
	m_strComboCellText = csvCell.GetLine();

	m_strCurrent = "";
	if ( arrayTexts.GetSize() != 0 )
	{
		CellTypeInfoTextItem item;
		arrayTexts.GetItem( 0, item );
		m_strCurrent = item.m_strCellText;
	}
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CComboListCtrl::CComboListCtrl()
{
	m_bAdjustedRowHeight = FALSE;

	m_nEditRow = -1;
	m_nEditCol = -1;
	m_nCellXMargin = 10;
	m_nCellYMargin = 5;

	m_dwEditCtrlStyle = ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_LEFT | ES_NOHIDESEL;
	m_dwDropDownCtrlStyle = WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | ES_AUTOVSCROLL | 
							CBS_DROPDOWNLIST;

	AllowListScroll( TRUE );
	m_bInEdit = FALSE;
}

/**********************************************************************/

CComboListCtrl::~CComboListCtrl()
{
	CInPlaceComboNormal::DeleteInstance();
	CInPlaceComboTabbed::DeleteInstance();
	CInPlaceEdit::DeleteInstance();
	CInPlaceEditText::DeleteInstance(); 
}

/**********************************************************************/

void CComboListCtrl::AdjustRowHeight( LPMEASUREITEMSTRUCT lpMeasureItemStruct, int nAdjust )
{
   if ( FALSE == m_bAdjustedRowHeight )
   {
      lpMeasureItemStruct->itemHeight += nAdjust;
	  m_bAdjustedRowHeight = TRUE;
   }
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CComboListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CComboListCtrl)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDBLCLK()
	ON_NOTIFY_REFLECT(LVN_ENDLABELEDIT, OnEndLabelEdit)
	ON_NOTIFY_REFLECT(LVN_BEGINLABELEDIT, OnBeginLabelEdit)
	ON_WM_MOUSEWHEEL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

void CComboListCtrl::PreSubclassWindow()
{
	ModifyStyle( 0, LVS_SHOWSELALWAYS, 0 );
}

/**********************************************************************/

void CComboListCtrl::DrawItem ( LPDRAWITEMSTRUCT lpdis )
{
	CDC* pDC = CDC::FromHandle ( lpdis->hDC );
	CRect rcLine = lpdis->rcItem;
	UINT nIndex = lpdis->itemID;

	DWORD dwColBrush;
	DWORD dwColText;
	
	if ( nIndex != (UINT) -1 )
	{
		CRect rcCell = rcLine;
		rcCell.right = rcCell.left;

		LV_COLUMN lvc;
		lvc.mask = LVCF_FMT | LVCF_WIDTH;

		int nColumn = 0;
		while ( GetColumn ( nColumn, &lvc ) == TRUE )
		{
			rcCell.left = rcCell.right;
			rcCell.right += lvc.cx;

			if ( IsWindowEnabled() == TRUE )				// enabled
			{
				bool bSelected = ( lpdis->itemState & ODS_SELECTED );

				if ( ( nIndex == m_nEditRow ) && ( nColumn >= m_nEditCol ) )
					bSelected = FALSE;

				if ( TRUE == bSelected )		// selected
				{
					dwColBrush = ::GetSysColor ( COLOR_HIGHLIGHT );
					dwColText  = ::GetSysColor ( COLOR_HIGHLIGHTTEXT );
				}
				else									// not selected
				{
					dwColBrush = ::GetSysColor ( COLOR_WINDOW );
					dwColText = COLOR_WINDOWTEXT;
				}

				CBrush Brush ( dwColBrush );
				pDC->FillRect ( rcCell, &Brush );
			}
			else										// disabled
			{
				dwColBrush = ::GetSysColor ( COLOR_SCROLLBAR );
				dwColText = COLOR_WINDOWTEXT;

				CBrush Brush ( dwColBrush );
				pDC->FillRect ( rcCell, &Brush );
			}

			DWORD dwOldTextColour = pDC->SetTextColor ( dwColText );
			int nOldBkMode = pDC->SetBkMode ( TRANSPARENT );

			UINT nAlign = DT_SINGLELINE | DT_NOPREFIX | DT_NOCLIP | DT_VCENTER;

			switch ( lvc.fmt & LVCFMT_JUSTIFYMASK )
			{
			case LVCFMT_LEFT:	nAlign |= DT_LEFT;		break;
			case LVCFMT_RIGHT:	nAlign |= DT_RIGHT;		break;
			case LVCFMT_CENTER:	nAlign |= DT_CENTER;	break;
			default:			nAlign |= DT_LEFT;		break;
			}

			CRect rcLabel;
			rcLabel = rcCell;
			rcLabel.left += m_nCellXMargin;
			rcLabel.right -= m_nCellXMargin;

			pDC->DrawText ( SSMakeShortString(pDC,GetItemText(nIndex,nColumn),rcLabel.Width()), -1, rcLabel, nAlign );

			pDC->SetTextColor ( dwOldTextColour );
			pDC->SetBkMode ( nOldBkMode );
			
			nColumn++;
		}

		if ( IsWindowEnabled() == TRUE )
			if ( lpdis->itemState & ODS_FOCUS )
				pDC->DrawFocusRect ( rcLine );
	}
}

/**********************************************************************/

CInPlaceComboNormal* CComboListCtrl::ShowInPlaceComboNormal(int iRowIndex, int iColumnIndex, CStringList& rComboItemsList, int iSel /*= -1*/)
{
	// The returned obPointer should not be saved
	
	// Make sure that the item is visible
	if (!EnsureVisible(iRowIndex, TRUE))
	{
		return NULL;
	}

	// Make sure that iColumnIndex is valid 
	CHeaderCtrl* pHeader = static_cast<CHeaderCtrl*> (GetDlgItem(FIRST_COLUMN));

	int iColumnCount = pHeader->GetItemCount();

	if (iColumnIndex >= iColumnCount || GetColumnWidth(iColumnIndex) < MIN_COLUMN_WIDTH) 
	{
		return NULL;
	}

	// Calculate the rectangle specifications for the combo box
	CRect obCellRect(0, 0, 0, 0);
	CalculateCellRect(iColumnIndex, iRowIndex, obCellRect, FALSE );
	m_nEditRow = iRowIndex;
	m_nEditCol = iColumnIndex;

	int iHeight = obCellRect.Height();  
	int iCount = rComboItemsList.GetCount();

	iCount = (iCount < MAX_DROP_DOWN_ITEM_COUNT) ? 
		iCount + MAX_DROP_DOWN_ITEM_COUNT : (MAX_DROP_DOWN_ITEM_COUNT + 1); 

	obCellRect.bottom += iHeight * iCount; 

	// Create the in place combobox
	CInPlaceComboNormal* pInPlaceCombo = CInPlaceComboNormal::GetInstance();
	pInPlaceCombo->ShowComboCtrl(m_dwDropDownCtrlStyle, obCellRect, this, 0, iRowIndex, iColumnIndex, &rComboItemsList, iSel);

	m_bInEdit = FALSE;
	AllowListScroll( FALSE );
	return pInPlaceCombo;
}

/**********************************************************************/

CInPlaceComboTabbed* CComboListCtrl::ShowInPlaceComboTabbed(int iRowIndex, int iColumnIndex, CStringList& rComboItemsList, int iSel /*= -1*/)
{
	// The returned obPointer should not be saved
	
	// Make sure that the item is visible
	if (!EnsureVisible(iRowIndex, TRUE))
	{
		return NULL;
	}

	// Make sure that iColumnIndex is valid 
	CHeaderCtrl* pHeader = static_cast<CHeaderCtrl*> (GetDlgItem(FIRST_COLUMN));

	int iColumnCount = pHeader->GetItemCount();

	if (iColumnIndex >= iColumnCount || GetColumnWidth(iColumnIndex) < MIN_COLUMN_WIDTH) 
	{
		return NULL;
	}

	// Calculate the rectangle specifications for the combo box
	CRect obCellRect(0, 0, 0, 0);
	CalculateCellRect(iColumnIndex, iRowIndex, obCellRect, TRUE );
	m_nEditRow = iRowIndex;
	m_nEditCol = iColumnIndex;

	int iHeight = obCellRect.Height();  
	int iCount = rComboItemsList.GetCount();

	iCount = (iCount < MAX_DROP_DOWN_ITEM_COUNT) ? 
		iCount + MAX_DROP_DOWN_ITEM_COUNT : (MAX_DROP_DOWN_ITEM_COUNT + 1); 

	obCellRect.bottom += iHeight * iCount; 

	// Create the in place combobox
	CInPlaceComboTabbed* pInPlaceCombo = CInPlaceComboTabbed::GetInstance();
	pInPlaceCombo->ShowComboCtrl(m_dwDropDownCtrlStyle | CBS_OWNERDRAWFIXED, obCellRect, this, 0, iRowIndex, iColumnIndex, &rComboItemsList, iSel);

	m_bInEdit = FALSE;
	AllowListScroll( FALSE );
	return pInPlaceCombo;
}

/**********************************************************************/

CInPlaceEdit* CComboListCtrl::ShowInPlaceEdit(int iRowIndex, int iColumnIndex, CString& rstrCurSelection, CellTypeInfo& info )
{
	// Create an in-place edit control
	CInPlaceEdit* pInPlaceEdit = CInPlaceEdit::GetInstance();
		
	CRect obCellRect(0, 0, 0, 0);
	CalculateCellRect(iColumnIndex, iRowIndex, obCellRect, FALSE );
	m_nEditRow = iRowIndex;
	m_nEditCol = iColumnIndex;
				
	pInPlaceEdit->ShowEditCtrl(m_dwEditCtrlStyle, obCellRect, this, 0, 
							   iRowIndex, iColumnIndex,
							   rstrCurSelection);

	if ( info.m_nSSEditType != 0xFFFF )
		pInPlaceEdit->SetFormat( info.m_nSSEditType, info.m_strSSEditFormat );

	pInPlaceEdit->LimitText( info.m_nSSEditWidth );

	m_bInEdit = TRUE;
	AllowListScroll( FALSE );
	return pInPlaceEdit;
}

/**********************************************************************/

CInPlaceEditText* CComboListCtrl::ShowInPlaceEditText(int iRowIndex, int iColumnIndex, CString& rstrCurSelection, CellTypeInfo& info )
{
	// Create an in-place edit control
	CInPlaceEditText* pInPlaceEdit = CInPlaceEditText::GetInstance();
		
	CRect obCellRect(0, 0, 0, 0);
	CalculateCellRect(iColumnIndex, iRowIndex, obCellRect, FALSE );
	m_nEditRow = iRowIndex;
	m_nEditCol = iColumnIndex;

	pInPlaceEdit->ShowEditCtrl(m_dwEditCtrlStyle, obCellRect, this, 0, 
							   iRowIndex, iColumnIndex,
							   rstrCurSelection);

	pInPlaceEdit->LimitText( info.m_nSSEditWidth );

	m_bInEdit = TRUE;
	AllowListScroll( FALSE );
	return pInPlaceEdit;
}

/**********************************************************************/

void CComboListCtrl::OnHScroll(UINT iSBCode, UINT iPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default

	if (GetFocus() != this)
	{
		SetFocus();
	}

	CListCtrl::OnHScroll(iSBCode, iPos, pScrollBar);
}

/**********************************************************************/

void CComboListCtrl::OnVScroll(UINT iSBCode, UINT iPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	if ( m_bAllowListScroll == FALSE )
		return;

	if (GetFocus() != this)
	{
		SetFocus();
	}

	CListCtrl::OnVScroll(iSBCode, iPos, pScrollBar);
}

/**********************************************************************/

void CComboListCtrl::HandleReturn( int nActionCol )
{
	POSITION pos = GetFirstSelectedItemPosition();

	if ( pos != NULL )
	{
		int nRow = GetNextSelectedItem( pos );
		if ( ( nRow >= 0 ) && ( nRow < GetItemCount() ) )
			ProcessEmbeddedControl( nRow, nActionCol );
	}
}

/**********************************************************************/

void CComboListCtrl::OnLButtonDown(UINT iFlags, CPoint point ) 
{
	UINT uHitFlags;
    int nItem = HitTest(point, &uHitFlags);

    if (uHitFlags & LVHT_NOWHERE)
    {
        // eat the message by just returning
        return;
    }

	// TODO: Add your message handler code here and/or call default

	int iColumnIndex = -1;
	int iRowIndex = -1;

	// Get the current column and row
	if (!HitTestEx( point, &iRowIndex, &iColumnIndex))
	{
		return;
	}

	CListCtrl::OnLButtonDown(iFlags, point);
	
	// If column is not read only then
	// If the SHIFT or CTRL key is down call the base class
	// Check the high bit of GetKeyState to determine whether SHIFT or CTRL key is down
	if ((GetKeyState(VK_SHIFT) & 0x80) || (GetKeyState(VK_CONTROL) & 0x80))
	{
		return;
	}

	ProcessEmbeddedControl( iRowIndex, iColumnIndex );
}

/**********************************************************************/

void CComboListCtrl::ProcessEmbeddedControl( int nRow, int nCol )
{	
	if (-1 != nRow )
	{
		UINT flag = LVIS_FOCUSED;
		
		if ((GetItemState( nRow, flag ) & flag) == flag)
		{
			// Add check for LVS_EDITLABELS
			if (GetWindowLong(m_hWnd, GWL_STYLE) & LVS_EDITLABELS)
			{
				CellTypeInfo info;
				info.m_nCol = nCol;
				info.m_nRow = nRow;
				info.m_nType = 0;
				info.m_nSSEditType = 0xFFFF;
				info.m_strSSEditFormat = "%d";
				info.m_nComboSelect = 0;

				GetParent() -> SendMessage(WM_GET_CELLTYPE, 0, (LPARAM) &info );

				switch( info.m_nType )
				{
				case CELL_TYPE_COMBO_NORMAL:	//COMBO
					{
						CStringList obComboItemsList;
										
						GetParent()->SendMessage(WM_SET_ITEMS, (WPARAM)nRow, (LPARAM)&obComboItemsList);  
					
						CInPlaceComboNormal* pInPlaceComboBox = ShowInPlaceComboNormal( nRow, nCol, obComboItemsList, info.m_nComboSelect );
						ASSERT(pInPlaceComboBox); 
					
						if ( ( info.m_nComboSelect >= 0 ) && ( info.m_nComboSelect < pInPlaceComboBox -> GetCount() ) )
							pInPlaceComboBox -> SetCurSel( info.m_nComboSelect );
					}
					break;

				case CELL_TYPE_COMBO_TABBED:
					{
						CStringList obComboItemsList;
										
						GetParent()->SendMessage(WM_SET_ITEMS, (WPARAM)nRow, (LPARAM)&obComboItemsList);  
					
						CInPlaceComboTabbed* pInPlaceComboBox = ShowInPlaceComboTabbed( nRow, nCol, obComboItemsList, info.m_nComboSelect );
						ASSERT(pInPlaceComboBox); 
					
						if ( ( info.m_nComboSelect >= 0 ) && ( info.m_nComboSelect < pInPlaceComboBox -> GetCount() ) )
							pInPlaceComboBox -> SetCurSel( info.m_nComboSelect );
					}
					break;

				case CELL_TYPE_EDIT:	//EDIT BOX
					{
						CString strCurSelection = GetItemText( nRow, nCol );
						ShowInPlaceEdit( nRow, nCol, strCurSelection, info );
					}
					break;

				case CELL_TYPE_EDIT_TEXT:
					{
						CString strCurSelection = GetItemText( nRow, nCol );
						ShowInPlaceEditText( nRow, nCol, strCurSelection, info );
					}
					break;

				case CELL_TYPE_BUTTON:
					GetParent() -> SendMessage(WM_CELL_BUTTON, 0, (LPARAM) &info );
					break;
				}
			}
		}
	}  
}

/**********************************************************************/

void CComboListCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    UINT uHitFlags;
    int nItem = HitTest(point, &uHitFlags);

    if (uHitFlags & LVHT_NOWHERE)
    {
        // eat the message by just returning
        return;
    }

	 CListCtrl::OnLButtonDblClk(nFlags, point);
}

/**********************************************************************/

void CComboListCtrl::OnRButtonDown(UINT nFlags, CPoint point)
{
    UINT uHitFlags;
    int nItem = HitTest(point, &uHitFlags);

    if (uHitFlags & LVHT_NOWHERE)
    {
        // eat the message by just returning
        return;
    }

    CListCtrl::OnRButtonDown(nFlags, point);
}

/**********************************************************************/

void CComboListCtrl::OnRButtonDblClk(UINT nFlags, CPoint point)
{
    UINT uHitFlags;
    int nItem = HitTest(point, &uHitFlags);

    if (uHitFlags & LVHT_NOWHERE)
    {
        // eat the message by just returning
        return;
    }

    CListCtrl::OnRButtonDblClk(nFlags, point);
}

/**********************************************************************/

bool CComboListCtrl::HitTestEx(CPoint &obPoint, int* pRowIndex, int* pColumnIndex) const
{
	if (!pRowIndex || !pColumnIndex)
	{
		return false;
	}

	// Get the row index
	*pRowIndex = HitTest(obPoint, NULL);

	if (pColumnIndex)
	{
		*pColumnIndex = 0;
	}

	// Make sure that the ListView is in LVS_REPORT
	if ((GetWindowLong(m_hWnd, GWL_STYLE) & LVS_TYPEMASK) != LVS_REPORT)
	{
		return false;
	}

	// Get the number of columns
	CHeaderCtrl* pHeader = (CHeaderCtrl*)GetDlgItem(0);

	int iColumnCount = pHeader->GetItemCount();

	// Get bounding rect of item and check whether obPoint falls in it.
	CRect obCellRect;
	GetItemRect(*pRowIndex, &obCellRect, LVIR_BOUNDS);
	
	if (obCellRect.PtInRect(obPoint))
	{
		// Now find the column
		for (*pColumnIndex = 0; *pColumnIndex < iColumnCount; (*pColumnIndex)++)
		{
			int iColWidth = GetColumnWidth(*pColumnIndex);
			
			if (obPoint.x >= obCellRect.left && obPoint.x <= (obCellRect.left + iColWidth))
			{
				return true;
			}
			obCellRect.left += iColWidth;
		}
	}
	return false;
}

/**********************************************************************/

void CComboListCtrl::CalculateCellRect(int iColumnIndex, int iRowIndex, CRect& robCellRect, bool bCombo )
{
	GetItemRect(iRowIndex, &robCellRect, LVIR_BOUNDS);
	
	CRect rcClient;
	GetClientRect(&rcClient);

	if (robCellRect.right > rcClient.right) 
	{
		robCellRect.right = rcClient.right;
	}

	ScrollToView(iColumnIndex, robCellRect); 

	if ( FALSE == bCombo )
	{
		robCellRect.top += m_nCellYMargin;
		robCellRect.bottom -= m_nCellYMargin;
	}

	robCellRect.left += m_nCellXMargin;
	robCellRect.right -= m_nCellXMargin;
}

/**********************************************************************/

void CComboListCtrl::OnEndLabelEdit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	// Update the item text with the new text
	SetItemText(pDispInfo->item.iItem, pDispInfo->item.iSubItem, pDispInfo->item.pszText);

	m_bInEdit = FALSE;
	GetParent()->SendMessage(WM_VALIDATE, GetDlgCtrlID(), (LPARAM)pDispInfo); 
	*pResult = 0;
}

/**********************************************************************/

void CComboListCtrl::EnableHScroll(bool bEnable /*= true*/)
{
	if (bEnable)
	{
		m_dwDropDownCtrlStyle |= WS_HSCROLL;
	}
	else
	{
		m_dwDropDownCtrlStyle &= ~WS_HSCROLL;
	}	
}

/**********************************************************************/

void CComboListCtrl::EnableVScroll(bool bEnable /*= true*/)
{
	if (bEnable)
	{
		m_dwDropDownCtrlStyle |= WS_VSCROLL;
	}
	else
	{
		m_dwDropDownCtrlStyle &= ~WS_VSCROLL;
	}
}

/**********************************************************************/

void CComboListCtrl::ScrollToView(int iColumnIndex, /*int iOffSet, */CRect& robCellRect)
{
	// Now scroll if we need to expose the column
	CRect rcClient;
	GetClientRect(&rcClient);

	int iColumnWidth = GetColumnWidth(iColumnIndex);

	// Get the column iOffset
	int iOffSet = 0;
	for (int iIndex_ = 0; iIndex_ < iColumnIndex; iIndex_++)
	{
		iOffSet += GetColumnWidth(iIndex_);
	}

	// If x1 of cell rect is < x1 of ctrl rect or
	// If x1 of cell rect is > x1 of ctrl rect or **Should not ideally happen**
	// If the width of the cell extends beyond x2 of ctrl rect then
	// Scroll

	CSize obScrollSize(0, 0);

	if (((iOffSet + robCellRect.left) < rcClient.left) || 
		((iOffSet + robCellRect.left) > rcClient.right))
	{
		obScrollSize.cx = iOffSet + robCellRect.left;
	}
	else if ((iOffSet + robCellRect.left + iColumnWidth) > rcClient.right)
	{
		obScrollSize.cx = iOffSet + robCellRect.left + iColumnWidth - rcClient.right;
	}

	Scroll(obScrollSize);
	robCellRect.left -= obScrollSize.cx;
	
	// Set the width to the column width
	robCellRect.left += iOffSet;
	robCellRect.right = robCellRect.left + iColumnWidth;
}

/**********************************************************************/

void CComboListCtrl::OnBeginLabelEdit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	
	CellTypeInfo info;
	info.m_nCol = pDispInfo->item.iSubItem;
	info.m_nRow = pDispInfo->item.iItem;
	info.m_nType = 0;

	GetParent() -> SendMessage(WM_GET_CELLTYPE, 0, (LPARAM) &info );

	switch( info.m_nType )
	{
	case CELL_TYPE_EDIT:
	case CELL_TYPE_COMBO_NORMAL:
	case CELL_TYPE_COMBO_TABBED:
		*pResult = 0;
		break;

	case CELL_TYPE_BUTTON:
	case CELL_TYPE_READONLY:
	default:
		*pResult = 1;
		break;
	}
}

/**********************************************************************/

BOOL CComboListCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	if ( m_bAllowListScroll == FALSE )
		return TRUE;

	return CListCtrl::OnMouseWheel(nFlags, zDelta, pt);
}

/**********************************************************************/

void CComboListCtrl::AllowListScroll( bool bEnable )
{
	m_bAllowListScroll = bEnable;
}

/**********************************************************************/
