/**********************************************************************/
#include "KbdListNameArray.h"
/**********************************************************************/
#include "KbdListSelector.h"
/**********************************************************************/
static const int KBDLIST_UNKNOWN = 999999;
/**********************************************************************/

CKbdListSelector::CKbdListSelector( CEdit& edit, CSSComboBox& combo, bool bModifier ) : m_EditBox ( edit ), m_ComboBox ( combo )
{
	m_nListNo = 0;
	m_bModifier = bModifier;
	m_nUnknownPos = m_bModifier ? 1 : 0;
}

/**********************************************************************/

void CKbdListSelector::FillKbdListCombo( int nListNo )
{
	m_ComboBox.ResetContent();
	m_arrayHidden.RemoveAll();

	for ( int n = 0; n < KbdListNameArray.GetSize(); n++ )
	{
		if (KbdListNameArray.GetOfferPluLinkFlag(n) == TRUE)
		{
			int nInsertionPoint = m_ComboBox.AddString(KbdListNameArray.GetListName(n));
			m_ComboBox.SetItemData(nInsertionPoint, KbdListNameArray.GetListNo(n) + 1);
		}
		else
		{
			CSortedStringByInt item;
			item.m_nItem = KbdListNameArray.GetListNo(n);
			item.m_strItem = KbdListNameArray.GetListName(n);
			m_arrayHidden.Consolidate(item);
		}
	}

	m_ComboBox.InsertString ( 0, "None" );
	m_ComboBox.SetItemData( 0, 1 );

	if ( TRUE == m_bModifier )
	{
		m_ComboBox.InsertString ( 0, "As Base Plu" );
		m_ComboBox.SetItemData( 0, 0 );
	}

	m_nListNo = nListNo;
	
	bool bFound = FALSE;
	for ( int nIndex = 0; nIndex < m_ComboBox.GetCount(); nIndex++ )
	{
		if ( m_ComboBox.GetItemData( nIndex ) == (WORD) ( m_nListNo + 1 ) )
		{
			m_ComboBox.SetCurSel ( nIndex );
			bFound = TRUE;
			break;
		}
	}

	if ( bFound == FALSE )
	{
		m_ComboBox.InsertString( m_nUnknownPos, "Unknown" );
		m_ComboBox.SetItemData( m_nUnknownPos, KBDLIST_UNKNOWN );
		m_ComboBox.SetCurSel( m_nUnknownPos );
	}

	UpdateKbdListEditBox();
}

/**********************************************************************/

void CKbdListSelector::UpdateKbdListCombo(int nNewListNo)
{
	m_nListNo = nNewListNo;
	bool bFound = 0;

	for (int nIndex = 0; nIndex <= m_ComboBox.GetCount(); nIndex++)
	{
		if (m_ComboBox.GetItemData(nIndex) == (WORD)(m_nListNo + 1))
		{
			m_ComboBox.SetCurSel(nIndex);
			bFound = TRUE;
			break;
		}
	}

	if (bFound == FALSE)
	{
		CSortedStringByInt item;
		item.m_nItem = nNewListNo;

		int nPos = 0;
		if (m_arrayHidden.Find(item, nPos) == TRUE)
		{
			m_arrayHidden.GetAt(nPos, item);
			
			int nInsertionPoint = m_ComboBox.AddString(item.m_strItem);
			m_ComboBox.SetItemData(nInsertionPoint, item.m_nItem + 1);
			m_ComboBox.SetCurSel(nInsertionPoint);

			m_arrayHidden.RemoveAt(nPos);
			bFound = TRUE;
		}
	}


	if (FALSE == bFound)
	{
		if (m_ComboBox.GetItemData(m_nUnknownPos) != KBDLIST_UNKNOWN)
		{
			m_ComboBox.InsertString(m_nUnknownPos, "Unknown");
			m_ComboBox.SetItemData(m_nUnknownPos, KBDLIST_UNKNOWN);
		}
		m_ComboBox.SetCurSel(m_nUnknownPos);
	}
	else
	{
		if (m_ComboBox.GetItemData(m_nUnknownPos) == KBDLIST_UNKNOWN)
			m_ComboBox.DeleteString(m_nUnknownPos);
	}
}

/**********************************************************************/

void CKbdListSelector::UpdateKbdListEditBox()
{
	CString strKbdListNo = "";

	if ( m_nListNo >= 0 )
	{
		strKbdListNo.Format ( "%d", m_nListNo );
	}

	m_EditBox.SetWindowText ( strKbdListNo );
}

/**********************************************************************/

void CKbdListSelector::SelectKbdListFromCombo() 
{
	int nIndex = m_ComboBox.GetCurSel();
	
	if ( nIndex != CB_ERR )
	{
		int nNewKbdListNo = m_ComboBox.GetItemData( nIndex ) - 1;

		if ( ( nNewKbdListNo != m_nListNo ) && ( nNewKbdListNo != KBDLIST_UNKNOWN - 1 ) )
		{
			m_nListNo = nNewKbdListNo;
			UpdateKbdListEditBox();

			if ( m_ComboBox.GetItemData( m_nUnknownPos ) == KBDLIST_UNKNOWN )
				m_ComboBox.DeleteString( m_nUnknownPos );
		}
	}
}

/**********************************************************************/

void CKbdListSelector::SelectKbdListFromEditBox() 
{
	CString strKbdListNo;
	m_EditBox.GetWindowText( strKbdListNo );
	
	::TrimSpaces ( strKbdListNo, FALSE );
	
	int nNewKbdListNo = 0;
	if ( strKbdListNo == "" )
	{
		nNewKbdListNo = m_bModifier ? -1 : m_nListNo;
	}
	else
	{
		nNewKbdListNo = atoi ( strKbdListNo );
	}

	if ( nNewKbdListNo != m_nListNo )
	{
		UpdateKbdListCombo( nNewKbdListNo );
	}
	
	UpdateKbdListEditBox();
}

/**********************************************************************/
