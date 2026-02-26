/**********************************************************************/
#include "AnalysisCategoryCSVArray.h"
/**********************************************************************/
#include "SPOSKeyboardAnalysisCategorySelector.h"
/**********************************************************************/

CSPOSKeyboardAnalysisCategorySelector::CSPOSKeyboardAnalysisCategorySelector( CEdit& edit, CSSComboBox& combo ) : m_EditBox ( edit ), m_ComboBox ( combo )
{
	m_nCategoryFilter = CATEGORYSET_ALL;
}

/**********************************************************************/

void CSPOSKeyboardAnalysisCategorySelector::FillCategoryCombo( int nCategoryFilter )
{
	m_ComboBox.ResetContent();
	DataManager.AnalysisCategory.CreateInUseArray();
	
	for ( int nIndex = 0; nIndex < DataManager.AnalysisCategory.GetInUseArraySize(); nIndex++ )
	{
		int nCatNo = DataManager.AnalysisCategory.GetInUseCatNo( nIndex );

		CAnalysisCategoryCSVRecord Category;
		DataManager.AnalysisCategory.GetCategory( nCatNo, Category );
		
		CString strText = Category.GetEposText();
		int nInsertionPoint = m_ComboBox.AddString ( strText );
		m_ComboBox.SetItemData ( nInsertionPoint, ( WORD ) nCatNo );
	}

	m_ComboBox.InsertString ( 0, "All" );
	m_ComboBox.SetItemData ( 0, CATEGORYSET_ALL );
	
	m_ComboBox.SetCurSel(0);
	m_nCategoryFilter = m_ComboBox.GetItemData(0);

	bool bFound = FALSE;
	for ( int nIndex = 0; nIndex < m_ComboBox.GetCount(); nIndex++ )
	{
		if ( m_ComboBox.GetItemData ( nIndex ) == (WORD) nCategoryFilter )
		{
			m_ComboBox.SetCurSel ( nIndex );
			m_nCategoryFilter = nCategoryFilter;
			bFound = TRUE;
			break;
		}
	}

	if ( FALSE == bFound )
	{
		m_ComboBox.InsertString ( 0, "Unknown" );
		m_ComboBox.SetItemData ( 0, nCategoryFilter  );
		m_ComboBox.SetCurSel(0);
		m_nCategoryFilter = nCategoryFilter;
	}

	UpdateCategoryEditBox();
}

/**********************************************************************/

void CSPOSKeyboardAnalysisCategorySelector::UpdateCategoryCombo( int nNewCategoryFilter)
{
	for ( int nIndex = 0; nIndex <= m_ComboBox.GetCount(); nIndex++ )
	{
		if ( m_ComboBox.GetItemData ( nIndex ) == ( WORD ) nNewCategoryFilter )
		{
			m_ComboBox.SetCurSel ( nIndex );
			m_nCategoryFilter = nNewCategoryFilter;
			return;
		}
	}
}

/**********************************************************************/

void CSPOSKeyboardAnalysisCategorySelector::UpdateCategoryEditBox()
{
	CString strCatNo = "";
	if ( m_nCategoryFilter >= 1 && m_nCategoryFilter <= MAX_ACAT_NO )
		strCatNo.Format ( "%d", m_nCategoryFilter );

	m_EditBox.SetWindowText ( strCatNo );
}

/**********************************************************************/

void CSPOSKeyboardAnalysisCategorySelector::SelectCategoryFromCombo() 
{
	int nIndex = m_ComboBox.GetCurSel();
	
	if ( nIndex != CB_ERR )
	{
		int nNewCategoryFilter = m_ComboBox.GetItemData ( nIndex );

		if ( nNewCategoryFilter != m_nCategoryFilter )
		{
			m_nCategoryFilter = nNewCategoryFilter;
			UpdateCategoryEditBox();
		}
	}
}

/**********************************************************************/

void CSPOSKeyboardAnalysisCategorySelector::SelectCategoryFromEditBox() 
{
	CString strCatNo;
	m_EditBox.GetWindowText( strCatNo );
	
	::TrimSpaces ( strCatNo, FALSE );
	
	if ( strCatNo == "" )
	{
		UpdateCategoryEditBox();
		return;
	}

	int nNewCategoryFilter = atoi ( strCatNo );
	if ( nNewCategoryFilter != m_nCategoryFilter )
	{
		UpdateCategoryCombo( nNewCategoryFilter );
		UpdateCategoryEditBox();
	}
}

/**********************************************************************/
