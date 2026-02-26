/**********************************************************************/
#include "StockOptionsIni.h"
/**********************************************************************/
#include "CategoryComboHelpers.h"
/**********************************************************************/

CCategoryComboHelpers::CCategoryComboHelpers(void)
{
	m_nCurrentDbIdx = -1;
	m_bCurrentSystemFlag = TRUE;
	m_pLocationSelector = NULL;
	m_pCategorySelector = NULL;
	m_bDoCustomFields = FALSE;
}

/**********************************************************************/

CCategoryComboHelpers::~CCategoryComboHelpers(void)
{
}

/**********************************************************************/

void CCategoryComboHelpers::SetControlPointers( CLocationSelector* pLocationSelector, CCategorySelectorNew* pCategorySelector )
{
	m_pLocationSelector = pLocationSelector;
	m_pCategorySelector = pCategorySelector;
}

/**********************************************************************/

void CCategoryComboHelpers::UpdateCombos()
{
	if ( m_pLocationSelector != NULL )
	{
		CLocationSelectorEntity LocSelEntity;
		m_pLocationSelector -> GetSelectedEntity( LocSelEntity );

		int nOldDbIdx = m_nCurrentDbIdx;
		int nNewDbIdx = LocSelEntity.GetDbIdx();
		bool bOldSystemFlag = m_bCurrentSystemFlag;
		bool bNewSystemFlag = ( LocSelEntity.GetConType() == NODE_SYSTEM );
		bool bGlobalCat = StockOptions.GetGlobalCategoryFlag();
		bool bGlobalCustomField = StockOptions.GetGlobalCustomFieldFlag();

		if ( ( nNewDbIdx != nOldDbIdx ) || ( bNewSystemFlag != bOldSystemFlag ) )
		{
			if ( ( FALSE == bNewSystemFlag ) || ( nOldDbIdx < 0 ) )
			{
				CDataManagerInfo info;
				DataManager.OpenDatabaseReadOnly( nNewDbIdx, info, FALSE );
				m_nCurrentDbIdx = nNewDbIdx;
			}
			
			if ( ( FALSE == bGlobalCat ) || ( nOldDbIdx < 0 ) )
			{			
				if ( ( FALSE == bNewSystemFlag ) || ( TRUE == bGlobalCat ) )
					m_pCategorySelector -> FillCategoryComboDatabase( 0, NEWCATEGORY_SELECTOR_REPORT, TRUE );
				else
					m_pCategorySelector -> FillCategoryComboSystem( 0, TRUE );
			}

			if ( TRUE == m_bDoCustomFields )
			{
				for ( int nSlot = 0; nSlot < DataManager.CustomFieldManager.GetSlotCount(); nSlot++ )
				{
					CString strText = DataManager.CustomFieldManager.GetComboText(nSlot);

					if ( ( FALSE == bNewSystemFlag ) || ( TRUE == bGlobalCustomField ) )
						DataManager.CustomFieldManager.FillComboSelectorDatabase( nSlot );
					else
						DataManager.CustomFieldManager.FillComboSelectorSystem( nSlot );

					DataManager.CustomFieldManager.ForceComboSelection( nSlot, strText );
				}
			}

			m_bCurrentSystemFlag = bNewSystemFlag;
		}
	}
}

/**********************************************************************/
