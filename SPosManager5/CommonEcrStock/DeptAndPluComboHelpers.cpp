/**********************************************************************/
#include "DeptAndPluComboHelpers.h"
/**********************************************************************/

CDeptAndPluComboHelpers::CDeptAndPluComboHelpers(void)
{
	m_nCurrentDbIdx = -1;
	m_bCurrentSystemFlag = TRUE;
	m_pComboPluFilter = NULL;
	m_pLocationSelector = NULL;
	m_pDepartmentSelector = NULL;
}

/**********************************************************************/

CDeptAndPluComboHelpers::~CDeptAndPluComboHelpers(void)
{
}

/**********************************************************************/

void CDeptAndPluComboHelpers::SetControlPointers( CSSComboBox* pComboPluFilter, CLocationSelector* pLocationSelector, CDepartmentSelector* pDepartmentSelector )
{
	m_pComboPluFilter = pComboPluFilter;
	m_pLocationSelector = pLocationSelector;
	m_pDepartmentSelector = pDepartmentSelector;
}

/**********************************************************************/

void CDeptAndPluComboHelpers::UpdateCombos()
{
	if ( m_pLocationSelector != NULL )
	{
		CLocationSelectorEntity LocSelEntity;
		m_pLocationSelector -> GetSelectedEntity( LocSelEntity );

		int nOldDbIdx = m_nCurrentDbIdx;
		int nNewDbIdx = LocSelEntity.GetDbIdx();
		bool bOldSystemFlag = m_bCurrentSystemFlag;
		bool bNewSystemFlag = ( LocSelEntity.GetConType() == NODE_SYSTEM );
		bool bGlobalDept = EcrmanOptions.GetGlobalDepartmentFlag();

		if ( ( nNewDbIdx != nOldDbIdx ) || ( bNewSystemFlag != bOldSystemFlag ) )
		{
			if ( ( FALSE == bNewSystemFlag ) || ( nOldDbIdx < 0 ) )
			{
				CDataManagerInfo info;
				DataManager.OpenDatabaseReadOnly( nNewDbIdx, info, FALSE );
				m_nCurrentDbIdx = nNewDbIdx;
			}
			
			if ( ( FALSE == bGlobalDept ) || ( nOldDbIdx < 0 ) )
			{			
				if ( ( FALSE == bNewSystemFlag ) || ( TRUE == bGlobalDept ) )
					m_pDepartmentSelector -> FillDepartmentComboDatabase( 0, DEPARTMENT_SELECTOR_REPORT, TRUE );
				else
					m_pDepartmentSelector -> FillDepartmentComboSystem( 0, TRUE );
			}

			if ( ( nNewDbIdx != nOldDbIdx ) || ( bNewSystemFlag != bOldSystemFlag ) )
			{
				ReloadPluFilterCombo(bNewSystemFlag);
			}

			m_bCurrentSystemFlag = bNewSystemFlag;
		}
	}
}

/**********************************************************************/

int CDeptAndPluComboHelpers::GetPluFilterNo()
{
	int nPluFilterNo = 0;

	COfferGroupsCSVArray* pFilterNames = m_bCurrentSystemFlag ? &DataManagerNonDb.SystemPluFilterNames : &DataManager.DbPluFilterNames;
		
	if ( m_pComboPluFilter != NULL )
	{
		int nSel = m_pComboPluFilter -> GetCurSel();
		
		if ( ( nSel >= 1 ) && ( nSel <= pFilterNames -> GetSize() ) )
		{
			COfferGroupEntriesCSVRecord PluFilter;
			pFilterNames -> GetAt( nSel - 1, PluFilter );
			nPluFilterNo = PluFilter.GetGroupNo();
		}
	}

	return nPluFilterNo;
}

/**********************************************************************/

void CDeptAndPluComboHelpers::SetPluFilterNo( int nFilterNo )
{
	if ( m_pComboPluFilter != NULL )
	{
		m_pComboPluFilter -> SetCurSel(0);

		COfferGroupsCSVArray* pFilterNames = m_bCurrentSystemFlag ? &DataManagerNonDb.SystemPluFilterNames : &DataManager.DbPluFilterNames;
	
		for( int n = 0; n < pFilterNames -> GetSize(); n++ )
		{
			COfferGroupsCSVRecord PluFilter( NODE_PLUFILTER_NAME );
			pFilterNames -> GetAt( n, PluFilter );
			
			if ( PluFilter.GetGroupNo() == nFilterNo )
			{
				m_pComboPluFilter -> SetCurSel(n + 1);
				break;
			}
		}
	}
}

/**********************************************************************/

void CDeptAndPluComboHelpers::ReloadPluFilterCombo( bool bSystem )
{
	if ( m_pComboPluFilter != NULL )
	{
		CString strOldString = "";
		m_pComboPluFilter ->GetWindowText(strOldString);
		strOldString.MakeUpper();

		m_pComboPluFilter -> ResetContent();
		m_pComboPluFilter -> AddString( "All items" );

		COfferGroupsCSVArray* pFilterNames = bSystem ? &DataManagerNonDb.SystemPluFilterNames : &DataManager.DbPluFilterNames;

		int nSel = 0;
		for ( int n = 0; n < pFilterNames -> GetSize(); n++ )
		{
			COfferGroupsCSVRecord PluFilter( NODE_PLUFILTER_NAME );
			pFilterNames -> GetAt( n, PluFilter );
		
			CString strName = PluFilter.GetDisplayName();
			m_pComboPluFilter -> AddString( strName );

			if ( 0 == nSel )
			{
				strName.MakeUpper();
				if ( strOldString == strName )
					nSel = n + 1;
			}
		}

		m_pComboPluFilter -> SetCurSel( nSel );
		m_pComboPluFilter -> EnableWindow( m_pComboPluFilter -> GetCount() > 1 );
	}
}

/**********************************************************************/
