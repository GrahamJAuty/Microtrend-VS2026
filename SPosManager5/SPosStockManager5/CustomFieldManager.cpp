/**********************************************************************/
#include "FilterTypes.h"
/**********************************************************************/
#include "CustomFieldManager.h"
/**********************************************************************/

CCustomFieldManager::CCustomFieldManager()
{
	Clear();
}

/**********************************************************************/

void CCustomFieldManager::Clear()
{
	m_nSlotCount = 0;
}

/**********************************************************************/

void CCustomFieldManager::Initialise()
{
	m_nSlotCount = 0;

	for ( int nSlot = 0; nSlot < MAX_ITEM_CUSTOM_FIELDS; nSlot++ )
	{
		m_nFieldNoBySlot[nSlot] = -1;
		m_strFieldNameBySlot[nSlot] = "";
		
		m_arrayValueMapInfo[ nSlot ].m_nCurrentFilter = -1;
		m_arrayValueMapInfo[ nSlot ].m_nCurrentDbIdx = -1;
		m_arrayValueMapInfo[ nSlot ].m_bIncludeNA = FALSE;
		m_arrayValueMapInfo[ nSlot ].m_nLastValueNo = -1;
		m_arrayValueMapInfo[ nSlot ].m_nLastSortIdx = -1;
		
		m_arrayValueMap[ nSlot ].RemoveAll();
		
		m_pComboSelector[nSlot] = NULL;
	}

	for ( int nFieldNo = 0; nFieldNo <= MAX_ITEM_CUSTOM_FIELDS; nFieldNo++ )
		m_nSlotByFieldNo[nFieldNo] = -1;

	for ( int nFieldNo = 1; nFieldNo <= MAX_ITEM_CUSTOM_FIELDS; nFieldNo++ )
	{
		if ( DataManagerNonDb.CustomFieldNames.CheckName( nFieldNo ) == TRUE )
		{
			m_nSlotByFieldNo[ nFieldNo ] = m_nSlotCount;
			m_nFieldNoBySlot [ m_nSlotCount ] = nFieldNo;
			m_strFieldNameBySlot[ m_nSlotCount ] = DataManagerNonDb.CustomFieldNames.GetName( nFieldNo ); 
			m_nSlotCount++;
		}
	}

	m_bIncludeAll = TRUE;
	m_bIncludeNA = TRUE;
	m_bIncludeSets = TRUE;
}

/**********************************************************************/

int CCustomFieldManager::GetSlotByFieldNo( int nFieldNo )
{
	if ( ( nFieldNo >= 1 ) && ( nFieldNo <= MAX_ITEM_CUSTOM_FIELDS ) )
		return m_nSlotByFieldNo[ nFieldNo ];
	else
		return -1;
}

/**********************************************************************/

int CCustomFieldManager::GetFieldNoBySlot( int nSlot )
{
	if ( ( nSlot >= 0 ) && ( nSlot < m_nSlotCount ) )
		return m_nFieldNoBySlot[ nSlot ];
	else
		return 0;
}

/**********************************************************************/

const char* CCustomFieldManager::GetFieldNameBySlot( int nSlot )
{
	if ( ( nSlot >= 0 ) && ( nSlot < m_nSlotCount ) )
		m_strFieldName = m_strFieldNameBySlot[ nSlot ];
	else
		m_strFieldName = "";

	return m_strFieldName;
}

/**********************************************************************/

const char* CCustomFieldManager::GetFieldNameByFieldNo( int nFieldNo )
{
	int nSlot = GetSlotByFieldNo( nFieldNo );

	if ( ( nSlot >= 0 ) && ( nSlot < m_nSlotCount ) )
		m_strFieldName = m_strFieldNameBySlot[ nSlot ];
	else
		m_strFieldName.Format( "Custom Field %d", nFieldNo );

	return m_strFieldName;
}

/**********************************************************************/
//SORT ORDER FOR CUSTOM FIELD VALUES IN REPORTS
/**********************************************************************/

void CCustomFieldManager::CreateValueMap( int nSlot, int nFilter )
{
	int nFieldNo = GetFieldNoBySlot( nSlot );
	if ( ( nFieldNo < 1 ) || ( nFieldNo > MAX_ITEM_CUSTOM_FIELDS ) )
		return;

	m_arrayValueMapInfo[ nSlot ].m_nCurrentFilter = nFilter;
	m_arrayValueMapInfo[ nSlot ].m_bIncludeNA = FALSE;
	m_arrayValueMapInfo[ nSlot ].m_nLastValueNo = -1;
	m_arrayValueMapInfo[ nSlot ].m_nLastSortIdx = -1;
	
	m_arrayValueMap[ nSlot ].RemoveAll();

	//CANNOT BUILD MAP IF FILTER IS BELOW ZERO
	if ( nFilter < 0 )
		return;

	//SIMPLE BUILD IF FILTER IS TO ONLY INCLUDE N/A
	if ( 1 == nFilter )
	{
		m_arrayValueMapInfo[ nSlot ].m_bIncludeNA = TRUE;

		for ( int nValueIdx = 0; nValueIdx < DataManager.CustomFieldValue[ nFieldNo ].GetSize(); nValueIdx++ )
		{
			CCustomFieldValueCSVRecord record;
			DataManager.CustomFieldValue[ nFieldNo ].GetAt( nValueIdx, record );
			
			CCustomFieldValueMapEntry entry;
			entry.m_nValueNo = record.GetNumber();
			entry.m_nValueIdx = nValueIdx;
			entry.m_nSortIdx = -1;

			int nPos;
			if ( FindMapValue( nSlot, entry.m_nValueNo, nPos ) == FALSE )
				m_arrayValueMap[nSlot].InsertAt( nPos, entry );
		}
		return;
	}

	//SIMPLE BUILD IF FILTER IS TO INCLUDE ALL FIELD VALUES
	if ( 0 == nFilter )
	{
		m_arrayValueMapInfo[ nSlot ].m_bIncludeNA = TRUE;

		for ( int nValueIdx = 0; nValueIdx < DataManager.CustomFieldValue[ nFieldNo ].GetSize(); nValueIdx++ )
		{
			CCustomFieldValueCSVRecord record;
			DataManager.CustomFieldValue[ nFieldNo ].GetAt( nValueIdx, record );
			
			CCustomFieldValueMapEntry entry;
			entry.m_nValueNo = record.GetNumber();
			entry.m_nValueIdx = nValueIdx;
			entry.m_nSortIdx = nValueIdx;

			int nPos;
			if ( FindMapValue( nSlot, entry.m_nValueNo, nPos ) == FALSE )
				m_arrayValueMap[nSlot].InsertAt( nPos, entry );
		}

		return;
	}

	//SIMPLE BUILD IF FILTER IS TO INCLUDE A SINGLE VALUE
	if ( ( nFilter >= 2 ) && ( nFilter < DataManager.CustomFieldValue[ nFieldNo ].GetSize() + 2 ) )
	{
		for ( int nValueIdx = 0; nValueIdx < DataManager.CustomFieldValue[ nFieldNo ].GetSize(); nValueIdx++ )
		{
			CCustomFieldValueCSVRecord record;
			DataManager.CustomFieldValue[ nFieldNo ].GetAt( nValueIdx, record );
			
			CCustomFieldValueMapEntry entry;
			entry.m_nValueNo = record.GetNumber();
			entry.m_nValueIdx = nValueIdx;
			entry.m_nSortIdx = ( nFilter - 2 == nValueIdx ) ? nValueIdx : -1; 

			int nPos;
			if ( FindMapValue( nSlot, entry.m_nValueNo, nPos ) == FALSE )
				m_arrayValueMap[nSlot].InsertAt( nPos, entry );
		}

		return;
	}

	//BUILD FOR CATEGORY FIELD SET
	int nBit = ( nFilter - FIRST_CATEGORYSET_FILTER ) + 1;
	if ( ( nBit >= 1 ) && ( nBit <= 31 ) )
	{
		for ( int n = 0; n < DataManager.CustomFieldSet[ nFieldNo ].GetSize(); n++ )
		{
			if ( DataManager.CustomFieldSet[ nFieldNo ].GetNumber( n ) == nBit )
			{
				CCustomFieldSetCSVRecord Set;
				DataManager.CustomFieldSet[ nFieldNo ].GetAt( n, Set );
				m_arrayValueMapInfo[ nSlot ].m_bIncludeNA = Set.GetIncludeNAFlag();
				break;
			}
		}

		for ( int nValueIdx = 0; nValueIdx < DataManager.CustomFieldValue[ nFieldNo ].GetSize(); nValueIdx++ )
		{
			CCustomFieldValueCSVRecord record;
			DataManager.CustomFieldValue[ nFieldNo ].GetAt( nValueIdx, record );
			CString strFlags = record.GetFieldSetFlags();
		
			CCustomFieldValueMapEntry entry;
			entry.m_nValueNo = record.GetNumber();
			entry.m_nValueIdx = nValueIdx;
			entry.m_nSortIdx = ( GetHexStringFlag( strFlags, nBit ) ) ? nValueIdx : -1; 	
				
			int nPos;
			if ( FindMapValue( nSlot, entry.m_nValueNo, nPos ) == FALSE )
				m_arrayValueMap[nSlot].InsertAt( nPos, entry );
		}
	}
}

/**********************************************************************/

int CCustomFieldManager::GetSortIdxByValueNo( int nSlot, int nValueNo )
{
	int nFieldNo = GetFieldNoBySlot( nSlot );
	if ( ( nFieldNo < 1 ) || ( nFieldNo > MAX_ITEM_CUSTOM_FIELDS ) )
		return -1;

	if ( nValueNo == m_arrayValueMapInfo[ nSlot ].m_nLastValueNo )
		return m_arrayValueMapInfo[ nSlot ].m_nLastSortIdx;

	int nSortIdx = -1;

	int nPos;
	if ( FindMapValue( nSlot, nValueNo, nPos ) == TRUE )
		nSortIdx = m_arrayValueMap[ nSlot ][ nPos ].m_nSortIdx;
	else if ( m_arrayValueMapInfo[ nSlot ].m_bIncludeNA == TRUE )
		nSortIdx = CUSTOM_FIELD_SORTIDX_NA;
	
	m_arrayValueMapInfo[ nSlot ].m_nLastValueNo = nValueNo;
	m_arrayValueMapInfo[ nSlot ].m_nLastSortIdx = nSortIdx;
	
	return nSortIdx;
}

/**********************************************************************/

bool CCustomFieldManager::FindMapValue( int nSlot, int nValue, int& nPos )
{
	if ( ( nSlot < 0 ) || ( nSlot >= m_nSlotCount ) )
		return FALSE;

	int nStart = 0;
	int nEnd = m_arrayValueMap[ nSlot ].GetSize() - 1;

	while ( nStart <= nEnd )
	{
		nPos = ( nStart + nEnd ) / 2;

		if ( m_arrayValueMap[ nSlot ][ nPos ].m_nValueNo > nValue )
			nEnd = nPos - 1;
		else if ( m_arrayValueMap[ nSlot ][ nPos ].m_nValueNo < nValue )
			nStart = nPos + 1;
		else
			return TRUE;
	}

	nPos = nStart;
	return FALSE;
}

/**********************************************************************/
//DROP DOWN LIST SELECTOR FOR CUSTOM FIELD VALUES AND SETS
/**********************************************************************/

void CCustomFieldManager::SetComboSelector( int nSlot, CSSComboBox* p )
{
	if ( ( nSlot >= 0 ) && ( nSlot < m_nSlotCount ) )
		m_pComboSelector[ nSlot ] = p;
}

/**********************************************************************/

void CCustomFieldManager::FillComboSelectorDatabase( int nSlot )
{
	int nFieldNo = GetFieldNoBySlot( nSlot );
	if ( ( nFieldNo < 1 ) || ( nFieldNo > MAX_ITEM_CUSTOM_FIELDS ) )
		return;

	CSSComboBox* pCombo = m_pComboSelector[ nSlot ];
	if ( pCombo == NULL )
		return;

	pCombo -> ResetContent();
	
	for ( int nIdx = 0; nIdx < DataManager.CustomFieldValue[ nFieldNo ].GetSize(); nIdx++ )
	{
		CString strText = DataManager.CustomFieldValue[ nFieldNo ].GetName(nIdx);
		int nInsertionPoint = pCombo -> InsertString ( -1, strText );
		pCombo -> SetItemData ( nInsertionPoint, nIdx + 2 );
	}

	if ( m_bIncludeSets == TRUE )
	{
		for ( int nIndex = 0; nIndex < DataManager.CustomFieldSet[ nFieldNo ].GetSize(); nIndex++ )
		{
			pCombo -> InsertString ( nIndex, DataManager.CustomFieldSet[ nFieldNo ].GetName ( nIndex ) );
			pCombo -> SetItemData ( nIndex, FIRST_CATEGORYSET_FILTER + nIndex );
		}
	}

	if ( m_bIncludeNA == TRUE )
	{
		pCombo -> InsertString ( 0, "N/A" );
		pCombo -> SetItemData ( 0, 1 );
	}

	if ( m_bIncludeAll == TRUE )
	{
		pCombo -> InsertString ( 0, "All" );
		pCombo -> SetItemData ( 0, 0 );
	}

	int nCount = pCombo -> GetCount();
	if ( nCount >= 1 ) pCombo -> SetCurSel(0);
	pCombo -> EnableWindow ( nCount >= 2 );
}

/**********************************************************************/

void CCustomFieldManager::FillComboSelectorSystem( int nSlot )
{
	int nFieldNo = GetFieldNoBySlot( nSlot );
	if ( ( nFieldNo < 1 ) || ( nFieldNo > MAX_ITEM_CUSTOM_FIELDS ) )
		return;

	CSSComboBox* pCombo = m_pComboSelector[ nSlot ];
	if ( pCombo == NULL )
		return;

	pCombo -> ResetContent();

	for ( int nIdx = 0; nIdx < DataManagerNonDb.SystemCustomFieldSet[ nFieldNo ].GetSize(); nIdx++ )
	{
		CString strText = DataManagerNonDb.SystemCustomFieldSet[ nFieldNo ].GetText(nIdx);
		int nInsertionPoint = pCombo -> InsertString ( -1, strText );
		pCombo -> SetItemData ( nInsertionPoint, nIdx + 2 );
	}

	if ( m_bIncludeNA == TRUE )
	{
		pCombo -> InsertString ( 0, "N/A" );
		pCombo -> SetItemData ( 0, 1 );
	}

	if ( m_bIncludeAll == TRUE )
	{
		pCombo -> InsertString ( 0, "All" );
		pCombo -> SetItemData ( 0, 0 );
	}

	int nCount = pCombo -> GetCount();
	if ( nCount >= 1 ) pCombo -> SetCurSel(0);
	pCombo -> EnableWindow ( nCount >= 2 );
}

/**********************************************************************/

int CCustomFieldManager::GetComboSelection( int nSlot )
{
	int nSelection = 0;
	
	if ( ( nSlot >= 0 ) && ( nSlot < m_nSlotCount ) )
	{
		CSSComboBox* pCombo = m_pComboSelector[ nSlot ];
		if ( pCombo != NULL )
		{
			int nPos = pCombo -> GetCurSel();
			if ( nPos >= 0 && nPos < pCombo -> GetCount() )
				nSelection = pCombo -> GetItemData ( nPos );
		}
	}

	return nSelection;
}

/**********************************************************************/

const char* CCustomFieldManager::GetComboText( int nSlot )
{
	m_strComboText = "Unknown Value";

	if ( ( nSlot >= 0 ) && ( nSlot < m_nSlotCount ) )
	{
		CSSComboBox* pCombo = m_pComboSelector[ nSlot ];
		if ( pCombo != NULL )
		{
			int nPos = pCombo -> GetCurSel();

			if ( nPos >= 0 && nPos < pCombo -> GetCount() )
				pCombo -> GetLBText ( nPos, m_strComboText );
		}
	}
	
	return m_strComboText;
}

/**********************************************************************/

void CCustomFieldManager::ForceComboSelection ( int nSlot, int nFilter )
{
	if ( ( nSlot >= 0 ) && ( nSlot < m_nSlotCount ) )
	{
		CSSComboBox* pCombo = m_pComboSelector[ nSlot ];
		if ( pCombo != NULL )
		{
			for ( int nPos = 0; nPos < pCombo -> GetCount(); nPos++ )
			{
				if ( pCombo -> GetItemData ( nPos ) == (DWORD) nFilter )
				{
					pCombo -> SetCurSel ( nPos );
					break;
				}
			}
		}
	}
}

/**********************************************************************/

void CCustomFieldManager::ForceComboSelection ( int nSlot, const char* szSelect )
{
	if ( ( nSlot >= 0 ) && ( nSlot < m_nSlotCount ) )
	{
		CSSComboBox* pCombo = m_pComboSelector[ nSlot ];
		if ( pCombo != NULL )
		{
			int nSel = pCombo -> FindStringExact( 0, szSelect );

			if ( ( nSel >= 0 ) && ( nSel < pCombo -> GetCount() ) )
				pCombo -> SetCurSel( nSel );
			else
				pCombo -> SetCurSel( 0 );
		}
	}
}

/**********************************************************************/

bool CCustomFieldManager::UpdateValueMapsFromComboSelectorsDatabase()
{
	bool bRebuiltMap = FALSE;

	for ( int nSlot = 0; nSlot < m_nSlotCount; nSlot++ )
	{
		int nOldFilter = m_arrayValueMapInfo[nSlot].m_nCurrentFilter;
		int nNewFilter = GetComboSelection(nSlot);

		if ( nNewFilter != nOldFilter )
		{
			CreateValueMap( nSlot, nNewFilter );
			bRebuiltMap = TRUE;
		}
	}

	return bRebuiltMap;
}

/**********************************************************************/

bool CCustomFieldManager::UpdateValueMapsFromComboSelectorsSystem()
{
	bool bRebuiltMap = FALSE;

	for ( int nSlot = 0; nSlot < m_nSlotCount; nSlot++ )
	{
		int nFieldNo = GetFieldNoBySlot(nSlot);

		int nOldFilter = m_arrayValueMapInfo[nSlot].m_nCurrentFilter;
		int nNewFilter = -1;

		int nOldDbIdx = m_arrayValueMapInfo[nSlot].m_nCurrentDbIdx;
		int nNewDbIdx = DataManager.GetActiveDbIdx();
		
		CString strMatch = GetComboText(nSlot);
		strMatch.MakeUpper();

		if ( ( TRUE == m_bIncludeNA ) && ( strMatch == "N/A" ) )
		{
			nNewFilter = 1;
		}
		else if ( ( TRUE == m_bIncludeAll ) && ( strMatch == "ALL" ) )
		{
			nNewFilter = 0;
		}
		else
		{
			for ( int nIdx = 0; nIdx < DataManager.CustomFieldValue[ nFieldNo ].GetSize(); nIdx++ )
			{
				CString strText = DataManager.CustomFieldValue[ nFieldNo ].GetName(nIdx);
				strText.MakeUpper();

				if ( strMatch == strText )
				{
					nNewFilter = nIdx + 2;
					break;
				}
			}

			if ( ( -1 == nNewFilter ) && ( m_bIncludeSets == TRUE ) )
			{
				for ( int nIndex = 0; nIndex < DataManager.CustomFieldSet[ nFieldNo ].GetSize(); nIndex++ )
				{
					CString strText = DataManager.CustomFieldSet[ nFieldNo ].GetName ( nIndex );
					strText.MakeUpper();
					
					if ( strMatch == strText )
					{
						nNewFilter = FIRST_CATEGORYSET_FILTER + nIndex;
						break;
					}
				}
			}
		}

		if ( ( nNewFilter != nOldFilter ) || ( nOldDbIdx != nNewDbIdx ) )
		{
			CreateValueMap( nSlot, nNewFilter );
			m_arrayValueMapInfo[nSlot].m_nCurrentDbIdx = nNewDbIdx;
			bRebuiltMap = TRUE;
		}
	}

	return bRebuiltMap;
}

/**********************************************************************/
