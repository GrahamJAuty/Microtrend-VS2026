/**********************************************************************/

CSSTabbedComboBox::CSSTabbedComboBox()
{
	m_nMaxColumns = 99;
}

/**********************************************************************/

CSSTabbedComboBox::~CSSTabbedComboBox()
{
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CSSTabbedComboBox, CComboBox)
	//{{AFX_MSG_MAP(CSSTabbedComboBox)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

void CSSTabbedComboBox::SetTabStops(CArray<int,int>& arrayTabStops )
{
	m_arrayTabStops.RemoveAll();
	for ( int n = 0; n < arrayTabStops.GetSize(); n++ )
		m_arrayTabStops.Add( arrayTabStops.GetAt(n) );
}

/**********************************************************************/

void CSSTabbedComboBox::ResetContent()
{
	CComboBox::ResetContent();
	m_arrayStrings.RemoveAll();
}

/**********************************************************************/

int CSSTabbedComboBox::AddItem( const char* szItem )
{
	int n = AddString( "xx" );

	if ( n >= 0 )
		m_arrayStrings.InsertAt( n, szItem );

	return n;
}

/**********************************************************************/

void CSSTabbedComboBox::InsertItem( int nPos, const char* szItem )
{
	int n = InsertString( nPos, "xx" );

	if ( ( n != CB_ERR ) && ( n != CB_ERRSPACE ) )
		m_arrayStrings.InsertAt( nPos, szItem );
}

/**********************************************************************/

bool CSSTabbedComboBox::SelectUsingItemData( int nItemData, int nDefault )
{
	int nItemCount = GetCount();

	bool bSetCurSel = FALSE;
	for ( int n = 0; n < nItemCount; n++ )
	{
		if ( GetItemData(n) == nItemData )
		{
			SetCurSel(n);
			bSetCurSel = TRUE;
			break;
		}
	}

	if ( FALSE == bSetCurSel )
	{
		if ( nDefault >= 0 ) 
		{
			if ( nDefault < nItemCount )
			{
				SetCurSel(nDefault);
				bSetCurSel = TRUE;
			}
			else if ( nItemCount > 0 )
			{
				SetCurSel(0);
				bSetCurSel = TRUE;
			}
		}
	}

	return bSetCurSel;
}

/**********************************************************************/

void CSSTabbedComboBox::DrawItem(LPDRAWITEMSTRUCT lpDraw) 
{
	CDC dc;
	dc.Attach( lpDraw -> hDC );

	CRect rect = lpDraw -> rcItem;
	int nIndex = (int) lpDraw -> itemID;

	bool bSelected = lpDraw -> itemState & ODS_SELECTED;

	CBrush* pBrush = new CBrush ( ::GetSysColor(( bSelected ) ? COLOR_HIGHLIGHT : COLOR_WINDOW ) );
	dc.FillRect( rect, pBrush );
	delete pBrush;

	if ( lpDraw -> itemState & ODS_FOCUS )
		dc.DrawFocusRect( rect );

	int nTabStopListSize = m_arrayTabStops.GetSize();
	
	if ( m_arrayTabStops.GetSize() >= m_nMaxColumns )
		nTabStopListSize = m_nMaxColumns;

	if ( bSelected == TRUE )
	{
		dc.SetBkMode(TRANSPARENT);
		dc.SetTextColor( GetSysColor( COLOR_HIGHLIGHTTEXT ) );
	}
	else
		dc.SetTextColor( GetSysColor( COLOR_WINDOWTEXT ) );

	if ( nIndex >= 0 && nIndex < m_arrayStrings.GetSize() )
	{
		CCSV csv( m_arrayStrings.GetAt(nIndex) );

		for ( int n = 0; ( n < csv.GetSize() ) && ( n < nTabStopListSize ); n++ )
		{
			CRect rectDraw = lpDraw -> rcItem;
			
			if ( n < nTabStopListSize - 1 )
				rectDraw.right = rectDraw.left + m_arrayTabStops[ n + 1 ] - 5; 
			
			rectDraw.left += m_arrayTabStops[n];
			rectDraw.top += 2;

			dc.ExtTextOut ( rectDraw.left, rectDraw.top, ETO_CLIPPED, &rectDraw, csv.GetString(n), NULL );
		}
	}
	
	dc.Detach();
}

/**********************************************************************/
