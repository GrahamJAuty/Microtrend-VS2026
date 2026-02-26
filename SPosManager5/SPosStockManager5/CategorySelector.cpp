/**********************************************************************/
 
/**********************************************************************/
//#include "DataManager.h"
#include "FilterTypes.h"
/**********************************************************************/
#include "CategorySelector.h"
/**********************************************************************/

CCategorySelector::CCategorySelector()
{
	m_CategorySet.CreateCategorySet(0);
	m_bIncludeCategorySets = FALSE;
	m_bIncludeReportEnabled = FALSE;
	m_bIncludeAll = FALSE;
	m_bIncludeCustom = FALSE;
	m_nFilter = 0;
	m_bGotCombo = FALSE;
}

/**********************************************************************/

CCategorySelector::~CCategorySelector()
{
}

/**********************************************************************/

void CCategorySelector::SelectCategorySet ( int nFilter )
{
	if ( nFilter < 0 )
		m_CategorySet.CreateReportEnabledCategorySet( DataManager.Category );
	else
		m_CategorySet.CreateCategorySet ( nFilter );

	m_nFilter = nFilter;
}

/**********************************************************************/

void CCategorySelector::IncludeCategory ( int nCategory, bool bInclude )
{
	m_CategorySet.IncludeCategory ( nCategory, bInclude );
}

/**********************************************************************/

void CCategorySelector::FillCategoryCombo( CSSComboBox* pCategoryCombo )
{
	m_bGotCombo = TRUE;
	m_pCategoryCombo = pCategoryCombo;
	m_pCategoryCombo -> ResetContent();

	for ( int nCategory = 1; nCategory <= DataManager.Category.GetSize(); nCategory++ )
	{
		if ( m_CategorySet.GetCategoryStatus ( nCategory ) == TRUE )
		{
			CString strText = DataManager.Category.GetText ( nCategory );

			if ( strText != DataManager.Category.GetDefaultText ( nCategory ) )
			{
				int nInsertionPoint = m_pCategoryCombo -> AddString ( strText );
				m_pCategoryCombo -> SetItemData ( nInsertionPoint, ( WORD ) nCategory );
			}
		}
	}

	for ( int nCategory = 1; nCategory <= DataManager.Category.GetSize(); nCategory++ )
	{
		if ( m_CategorySet.GetCategoryStatus( nCategory ) == TRUE )
		{
			CString strText = DataManager.Category.GetText ( nCategory );

			if ( strText == DataManager.Category.GetDefaultText ( nCategory ) )
			{
				int nInsertionPoint = m_pCategoryCombo -> InsertString ( -1, strText );
				m_pCategoryCombo -> SetItemData ( nInsertionPoint, ( WORD ) nCategory );
			}
		}
	}

	if ( m_bIncludeReportEnabled == TRUE )
	{
		m_pCategoryCombo -> InsertString ( 0, "Report enabled" );
		m_pCategoryCombo -> SetItemData ( 0, FIRST_CATEGORYSET_FILTER - 1 );
	}
	
	if ( m_bIncludeAll == TRUE )
	{
		m_pCategoryCombo -> InsertString ( 0, "All" );
		m_pCategoryCombo -> SetItemData ( 0, 0 );
	}

	if ( m_bIncludeCategorySets == TRUE )
	{
		for ( int nIndex = 0; nIndex < DataManager.CategorySet.GetSize(); nIndex++ )
		{
			if ( ( m_nFilter <= 0 ) || ( m_nFilter = FIRST_CATEGORYSET_FILTER + nIndex ) )
			{
				m_pCategoryCombo -> InsertString ( nIndex + 1, DataManager.CategorySet.GetName ( nIndex ) );
				m_pCategoryCombo -> SetItemData ( nIndex + 1, FIRST_CATEGORYSET_FILTER + nIndex );
			}
		}
	}

	if ( m_bIncludeCustom == TRUE )
	{
		int nInsertionPoint = m_pCategoryCombo -> InsertString ( -1, "Custom" );
		m_pCategoryCombo -> SetItemData ( nInsertionPoint, FIRST_CATEGORYSET_FILTER - 2 );
	}
	
	int nCount = m_pCategoryCombo -> GetCount();
	if ( nCount >= 1 ) m_pCategoryCombo -> SetCurSel(0);
	m_pCategoryCombo -> EnableWindow ( nCount >= 2 );
}

/**********************************************************************/

int CCategorySelector::GetSelection()
{
	int nSelection = 0;
	int nPos = m_pCategoryCombo -> GetCurSel();

	if ( nPos >= 0 && nPos < m_pCategoryCombo -> GetCount() )
		nSelection = m_pCategoryCombo -> GetItemData ( nPos );

	return nSelection;
}

/**********************************************************************/

const char* CCategorySelector::GetCategoryText()
{
	m_strCategoryText = "Unknown Category";
	int nPos = m_pCategoryCombo -> GetCurSel();

	if ( nPos >= 0 && nPos < m_pCategoryCombo -> GetCount() )
	{
		if ( m_pCategoryCombo -> GetItemData ( nPos ) == 0 )
			m_strCategoryText = "All Categories";
		else
			m_pCategoryCombo -> GetLBText ( nPos, m_strCategoryText );
	}

	return m_strCategoryText;
}

/**********************************************************************/

void CCategorySelector::ForceSelection (  int nFilter )
{
	for ( int nPos = 0; nPos < m_pCategoryCombo -> GetCount(); nPos++ )
	{
		if ( m_pCategoryCombo -> GetItemData ( nPos ) == (DWORD) nFilter )
		{
			m_pCategoryCombo -> SetCurSel ( nPos );
			break;
		}
	}
}

/**********************************************************************/

