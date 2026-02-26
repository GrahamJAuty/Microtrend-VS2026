/**********************************************************************/

CSSComboBox::CSSComboBox()
{
}

/**********************************************************************/

CSSComboBox::~CSSComboBox()
{
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CSSComboBox, CComboBox)
	//{{AFX_MSG_MAP(CSSComboBox)
	ON_CONTROL_REFLECT(CBN_DROPDOWN, OnDropDown)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

void CSSComboBox::OnDropDown() 
{
	// Reset the dropped width
    int nNumEntries = GetCount();
    int nWidth = 0;
    CString str;

    CClientDC dc(this);
    int nSave = dc.SaveDC();
    dc.SelectObject(GetFont());

    int nScrollWidth = ::GetSystemMetrics(SM_CXVSCROLL);
    for (int i = 0; i < nNumEntries; i++)
    {
        GetLBText(i, str);
        int nLength = dc.GetTextExtent(str).cx + nScrollWidth;
        nWidth = max(nWidth, nLength);
    }
    
    // Add margin space to the calculations
    nWidth += dc.GetTextExtent("0").cx;
    
    dc.RestoreDC(nSave);
    SetDroppedWidth(nWidth);	
}

/**********************************************************************/

void CSSComboBox::LoadStrings( CComboTextInfo info, int nStart )
{
	ResetContent();
	
	CCSV csv ( info.szComboItems );
	for ( int n = nStart; n < csv.GetSize(); n++ )
	{
		int nPos = AddString( csv.GetString(n) );
		SetItemData( nPos, n );
	}
}

/**********************************************************************/

bool CSSComboBox::SelectUsingItemData( int nItemData, int nDefault )
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

int CSSComboBox::GetItemDataOfSelection()
{
	int nSel = GetCurSel();

	if ( nSel < 0 || nSel >= GetCount() )
		return 0;
	else
		return GetItemData( nSel );
}

/**********************************************************************/
