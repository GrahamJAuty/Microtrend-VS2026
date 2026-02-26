/**********************************************************************/
#include "CategorySetCSVArray.h"
#include "FilterTypes.h"
/**********************************************************************/
#include "CategorySelectorNew.h"
/**********************************************************************/

CCategorySelectorNew::CCategorySelectorNew( CSSComboBox& combo ) : m_ComboBox ( combo )
{
}

/**********************************************************************/

void CCategorySelectorNew::FillCategoryComboDatabase( int nCategoryFilter, int nMode, bool bMatchString )
{
	FillCategoryComboInternal( DataManager.Category, DataManager.CategorySet, nCategoryFilter, nMode, bMatchString );
}

/**********************************************************************/

void CCategorySelectorNew::FillCategoryComboSystem( int nCategoryFilter, bool bMatchString )
{
	FillCategoryComboInternal( DataManagerNonDb.SystemCategory, DataManagerNonDb.SystemCategorySet, nCategoryFilter, NEWCATEGORY_SELECTOR_REPORT, bMatchString );
}

/**********************************************************************/

void CCategorySelectorNew::FillCategoryComboInternal( CDefTextCategory& CatArray, CCategorySetCSVArray& CatSetArray, int nCategoryFilter, int nMode, bool bMatchString )
{
	CCategorySetCSVRecord CategorySet;
	CategorySet.CreateReportEnabledCategorySet(CatArray);

	CString strOldString = "";
	if ( TRUE == bMatchString )
	{
		m_ComboBox.GetWindowText( strOldString );
		strOldString.MakeUpper();
	}
	
	m_ComboBox.ResetContent();

	for ( int nCatNo = 1; nCatNo <= CatArray.GetSize(); nCatNo++ )
	{
		if ( ( nMode != NEWCATEGORY_SELECTOR_REPORT ) || CategorySet.GetCategoryStatus ( nCatNo ) == TRUE )
		{
			CString strText = CatArray.GetText(nCatNo);

			if ( strText != CatArray.GetDefaultText(nCatNo) )
			{
				int nInsertionPoint = m_ComboBox.AddString ( strText );
				m_ComboBox.SetItemData ( nInsertionPoint, ( WORD ) nCatNo );
			}
		}
	}

	for ( int nCatNo = 1; nCatNo <= CatArray.GetSize(); nCatNo++ )
	{
		if ( ( nMode != NEWCATEGORY_SELECTOR_REPORT ) || CategorySet.GetCategoryStatus ( nCatNo ) == TRUE )
		{
			CString strText = CatArray.GetText(nCatNo);

			if ( strText == CatArray.GetDefaultText(nCatNo) )
			{
				int nInsertionPoint = m_ComboBox.InsertString ( -1, strText );
				m_ComboBox.SetItemData ( nInsertionPoint, ( WORD ) nCatNo );
			}
		}
	}

	switch ( nMode )
	{
	case NEWCATEGORY_SELECTOR_NORMAL:
		m_ComboBox.InsertString ( 0, "All" );
		m_ComboBox.SetItemData ( 0, DEPARTMENTSET_ALL );
		break;

	case NEWCATEGORY_SELECTOR_REPORT:
		m_ComboBox.InsertString ( 0, "Report enabled" );
		m_ComboBox.SetItemData ( 0, DEPARTMENTSET_REPORT );
		break;

	case NEWCATEGORY_SELECTOR_SINGLE:
	default:
		break;
	}

	switch ( nMode )
	{
	case NEWCATEGORY_SELECTOR_NORMAL:
	case NEWCATEGORY_SELECTOR_REPORT:
		{
			int nCategorySetPos = 1;
			for ( int nIndex = 0; nIndex < CatSetArray.GetSize(); nIndex++ )
			{
				m_ComboBox.InsertString ( nCategorySetPos, CatSetArray.GetName ( nIndex ) );
				m_ComboBox.SetItemData ( nCategorySetPos++, FIRST_CATEGORYSET_FILTER + nIndex );
			}
		}
		break;

	case NEWCATEGORY_SELECTOR_SINGLE:
	default:
		break;
	}

	m_ComboBox.SetCurSel(0);

	bool bFound = FALSE;
	
	if ( FALSE == bMatchString )
	{
		for ( int nIndex = 0; nIndex < m_ComboBox.GetCount(); nIndex++ )
		{
			if ( m_ComboBox.GetItemData ( nIndex ) == (WORD) nCategoryFilter )
			{
				m_ComboBox.SetCurSel( nIndex );
				bFound = TRUE;
				break;
			}
		}
	}
	else if ( strOldString != "" )
	{
		for ( int nIndex = 0; nIndex < m_ComboBox.GetCount(); nIndex++ )
		{
			CString strNewString = "";
			m_ComboBox.GetLBText(nIndex,strNewString);
			strNewString.MakeUpper();

			if ( strNewString == strOldString )
			{
				m_ComboBox.SetCurSel( nIndex );
				bFound = TRUE;
				break;
			}
		}
	}

	if ( ( bFound == FALSE ) && ( nMode == NEWCATEGORY_SELECTOR_SINGLE ) )
	{
		m_ComboBox.InsertString ( 0, "Unknown" );
		m_ComboBox.SetItemData ( 0, nCategoryFilter  );
		m_ComboBox.SetCurSel(0);
	}
}

/**********************************************************************/

void CCategorySelectorNew::UpdateCategoryCombo( int nNewCategoryFilter)
{
	for ( int nIndex = 0; nIndex <= m_ComboBox.GetCount(); nIndex++ )
	{
		if ( m_ComboBox.GetItemData ( nIndex ) == ( WORD ) nNewCategoryFilter )
		{
			m_ComboBox.SetCurSel ( nIndex );
			return;
		}
	}
}

/**********************************************************************/

int CCategorySelectorNew::GetCategoryFilter() 
{
	int nIndex = m_ComboBox.GetCurSel();
	
	if ( nIndex != CB_ERR )
		return m_ComboBox.GetItemData ( nIndex );
	else
		return 1;
}

/**********************************************************************/

