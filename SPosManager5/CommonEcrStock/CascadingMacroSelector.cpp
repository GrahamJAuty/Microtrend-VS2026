/**********************************************************************/
#include "CascadingMacroSelector.h"
/**********************************************************************/
static const int MACRO_UNKNOWN = 999999;
/**********************************************************************/

CCascadingMacroSelector::CCascadingMacroSelector( CEdit& edit, CSSComboBox& combo ) : m_EditBox ( edit ), m_ComboBox ( combo )
{
	m_nMacroNo = 0;
}

/**********************************************************************/

void CCascadingMacroSelector::FillMacroCombo( int nMacroNo )
{
	m_ComboBox.ResetContent();

	for ( int n = 0; n < DataManagerNonDb.CCMacroActive.GetSize(); n++ )
	{
		CCascadingMacroCSVRecord Macro;
		DataManagerNonDb.CCMacroActive.GetAt( n, Macro );
		int nInsertionPoint = m_ComboBox.AddString( Macro.GetParentMacroName() );
		m_ComboBox.SetItemData( nInsertionPoint, Macro.GetMacroNo() );
	}

	m_ComboBox.InsertString ( 0, "None" );
	m_ComboBox.SetItemData( 0, 0 );

	m_nMacroNo = nMacroNo;
	
	bool bFound = FALSE;
	for ( int nIndex = 0; nIndex < m_ComboBox.GetCount(); nIndex++ )
	{
		if ( m_ComboBox.GetItemData ( nIndex ) == (WORD) m_nMacroNo )
		{
			m_ComboBox.SetCurSel ( nIndex );
			bFound = TRUE;
			break;
		}
	}

	if ( bFound == FALSE )
	{
		m_ComboBox.InsertString( 0, "Unknown" );
		m_ComboBox.SetItemData( 0, MACRO_UNKNOWN );
		m_ComboBox.SetCurSel(0);
	}

	UpdateMacroEditBox();
}

/**********************************************************************/

void CCascadingMacroSelector::UpdateMacroCombo( int nNewMacroNo)
{
	m_nMacroNo = nNewMacroNo;
	bool bFound = 0;

	for ( int nIndex = 0; nIndex <= m_ComboBox.GetCount(); nIndex++ )
	{
		if ( m_ComboBox.GetItemData ( nIndex ) == ( WORD ) m_nMacroNo )
		{
			m_ComboBox.SetCurSel ( nIndex );
			bFound = TRUE;
			break;
		}
	}

	if ( bFound == FALSE )
	{
		if ( m_ComboBox.GetItemData(0) != MACRO_UNKNOWN )
		{
			m_ComboBox.InsertString( 0, "Unknown" );
			m_ComboBox.SetItemData( 0, MACRO_UNKNOWN );
		}
		m_ComboBox.SetCurSel(0);
	}
	else
	{
		if ( m_ComboBox.GetItemData(0) == MACRO_UNKNOWN )
			m_ComboBox.DeleteString(0);
	}
}

/**********************************************************************/

void CCascadingMacroSelector::UpdateMacroEditBox()
{
	CString strMacroNo = "";
	strMacroNo.Format ( "%d", m_nMacroNo );
	m_EditBox.SetWindowText ( strMacroNo );
}

/**********************************************************************/

void CCascadingMacroSelector::SelectMacroFromCombo() 
{
	int nIndex = m_ComboBox.GetCurSel();
	
	if ( nIndex != CB_ERR )
	{
		int nNewMacroNo = m_ComboBox.GetItemData ( nIndex );

		if ( ( nNewMacroNo != m_nMacroNo ) && ( nNewMacroNo != MACRO_UNKNOWN ) )
		{
			m_nMacroNo = nNewMacroNo;
			UpdateMacroEditBox();

			if ( m_ComboBox.GetItemData(0) == MACRO_UNKNOWN )
				m_ComboBox.DeleteString(0);
		}
	}
}

/**********************************************************************/

void CCascadingMacroSelector::SelectMacroFromEditBox() 
{
	CString strMacroNo;
	m_EditBox.GetWindowText( strMacroNo );
	
	::TrimSpaces ( strMacroNo, FALSE );
	
	if ( strMacroNo == "" )
	{
		UpdateMacroEditBox();
		return;
	}

	int nNewMacroNo = atoi ( strMacroNo );
	if ( nNewMacroNo != m_nMacroNo )
	{
		UpdateMacroCombo( nNewMacroNo );
		UpdateMacroEditBox();
	}
}

/**********************************************************************/
