/**********************************************************************/
#include "LocationCSVArray.h"
/**********************************************************************/
#include "TableReportHelpers.h"
/**********************************************************************/

CTableFilter::CTableFilter()
{
	Reset();
}

/**********************************************************************/

void CTableFilter::Reset()
{
	m_nType = TABLEFILTER_ALL;
	m_nLevel = 1;
	m_nEntityNo = 1;
	m_strName = "";
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CTableReportHelpers::CTableReportHelpers(void)
{
	m_nLocIdx = -2;
	m_bIncludeUnknown = FALSE;
	m_bIncludeNonTable = FALSE;
	m_nLastLookupTableNo = -1;
	m_nLastLookupGroupNo = 0;
	m_bLastLookupResponse = FALSE;
}

/**********************************************************************/

CTableReportHelpers::~CTableReportHelpers(void)
{
}

/**********************************************************************/

void CTableReportHelpers::SetLocIdx( int nLocIdx )
{
	if ( m_nLocIdx != nLocIdx )
	{
		m_nLocIdx = nLocIdx;
		m_arrayTableFilters.RemoveAll();
		m_GroupMap.RemoveAll();
		m_arrayTableNames.RemoveAll();
		m_arrayGroupNames.RemoveAll();
		m_nLastLookupTableNo = -1;
		m_nLastLookupGroupNo = 0;
		m_bLastLookupResponse = FALSE;
	}
}

/**********************************************************************/

void CTableReportHelpers::BuildTableFilterList()
{
	if ( ( m_nLocIdx > -2 ) && ( m_arrayTableFilters.GetSize() == 0 ) )
	{
		CTableFilter info;
		info.m_nType = TABLEFILTER_ALL;
		info.m_nLevel = 1;
		info.m_nEntityNo = 1;
		info.m_strName = "All";
		m_arrayTableFilters.Add( info );
		
		if ( ( m_nLocIdx >= 0 ) && ( m_nLocIdx < dbLocation.GetSize() ) )
		{
			OpenTableData();

			for ( int n = 0; n < DataManagerNonDb.TableNames.GetSize(); n++ )
			{
				CTableNameCSVRecord Table;
				DataManagerNonDb.TableNames.GetAt( n, Table );

				if ( Table.GetShowAsFilterFlag() == TRUE )
				{
					CTableFilter TableFilter;
					TableFilter.m_nType = TABLEFILTER_TABLE;
					TableFilter.m_nLevel = 1;
					TableFilter.m_nEntityNo = Table.GetTableNo();
					TableFilter.m_strName = Table.GetTableName();
					m_arrayTableFilters.Add( TableFilter );
				}
			}

			for ( int x = 1; x <= 3; x++ )
			{
				for ( int n = 0; n < DataManagerNonDb.TableGroups.GetSize(); n++ )
				{
					CTableGroupCSVRecord TableGroup;
					DataManagerNonDb.TableGroups.GetAt( n, TableGroup );

					if ( TableGroup.GetShowAsFilterFlag(x) == TRUE )
					{
						CTableFilter TableFilter;
						TableFilter.m_nType = TABLEFILTER_GROUP;
						TableFilter.m_nLevel = x;
						TableFilter.m_nEntityNo = n + 1;
						TableFilter.m_strName = TableGroup.GetGroupName(x);
						m_arrayTableFilters.Add( TableFilter );
					}
				}
			}

			for ( int x = 1; x <= 3; x++ )
			{
				for ( int n = 0; n < DataManagerNonDb.TableGroupSets.GetSize(); n++ )
				{
					CTableGroupSetCSVRecord TableGroupSet;
					DataManagerNonDb.TableGroupSets.GetAt( n, TableGroupSet );

					if ( TableGroupSet.GetShowAsFilterFlag(x) == TRUE )
					{
						CTableFilter TableFilter;
						TableFilter.m_nType = TABLEFILTER_SET;
						TableFilter.m_nLevel = x;
						TableFilter.m_nEntityNo = n + 1;
						TableFilter.m_strName = TableGroupSet.GetGroupSetName(x);
						m_arrayTableFilters.Add( TableFilter );
					}
				}
			}

			CloseTableData();
		}
	}
}

/**********************************************************************/

void CTableReportHelpers::GetTableFilter( int n, CTableFilter& Filter )
{
	Filter.Reset();
	if ( ( n >= 0 ) && ( n < m_arrayTableFilters.GetSize() ) )
		Filter = m_arrayTableFilters.GetAt(n);
}

/**********************************************************************/

void CTableReportHelpers::BuildGroupMap( CTableFilter& TableFilter )
{
	if ( ( m_nLocIdx > -2 ) && ( m_GroupMap.GetSize() == 0 ) )
	{
		OpenTableData();

		switch( TableFilter.m_nType )
		{
		case TABLEFILTER_ALL:
			{
				m_bIncludeUnknown = TRUE;
				m_bIncludeNonTable = TRUE;
			
				for ( int nTableIdx = 0; nTableIdx < DataManagerNonDb.TableNames.GetSize(); nTableIdx++ )
				{
					CTableNameCSVRecord Table;
					DataManagerNonDb.TableNames.GetAt( nTableIdx, Table );
					AddTableToGroupMap( Table, 1 );
				}
			}
			break;

		case TABLEFILTER_TABLE:
			{
				m_bIncludeUnknown = FALSE;
				m_bIncludeNonTable = FALSE;

				int nTableIdx;
				if ( DataManagerNonDb.TableNames.FindTableByNumber( TableFilter.m_nEntityNo, nTableIdx ) == TRUE )
				{
					CTableNameCSVRecord Table;
					DataManagerNonDb.TableNames.GetAt( nTableIdx, Table );
					AddTableToGroupMap( Table, TableFilter.m_nLevel );
				}
			}
			break;

		case TABLEFILTER_GROUP:
			{
				m_bIncludeUnknown = FALSE;
				m_bIncludeNonTable = FALSE;

				for ( int nTableIdx = 0; nTableIdx < DataManagerNonDb.TableNames.GetSize(); nTableIdx++ )
				{
					CTableNameCSVRecord Table;
					DataManagerNonDb.TableNames.GetAt( nTableIdx, Table );

					if ( Table.GetReportGroup( TableFilter.m_nLevel ) == TableFilter.m_nEntityNo )
					{
						AddTableToGroupMap( Table, TableFilter.m_nLevel );
					}
				}
			}
			break;

		case TABLEFILTER_SET:
			{
				m_bIncludeUnknown = FALSE;
				m_bIncludeNonTable = FALSE;

				int nMask = 0;
				if ( ( TableFilter.m_nEntityNo >= 1 ) && ( TableFilter.m_nEntityNo <= DataManagerNonDb.TableGroupSets.GetSize() ) )
				{
					CTableGroupSetCSVRecord GroupSet;
					DataManagerNonDb.TableGroupSets.GetAt( TableFilter.m_nEntityNo - 1, GroupSet );
					m_bIncludeUnknown = GroupSet.GetIncludeUnknownFlag( TableFilter.m_nLevel );
					m_bIncludeNonTable = GroupSet.GetIncludeNonTableFlag( TableFilter.m_nLevel );
					nMask = GroupSet.GetGroupMask( TableFilter.m_nLevel );
				}

				for ( int nTableIdx = 0; nTableIdx < DataManagerNonDb.TableNames.GetSize(); nTableIdx++ )
				{
					CTableNameCSVRecord Table;
					DataManagerNonDb.TableNames.GetAt( nTableIdx, Table );
					
					int nGroupMask = 1 << ( Table.GetReportGroup( TableFilter.m_nLevel ) - 1 );

					if ( ( nGroupMask & nMask ) != 0 )
					{
						AddTableToGroupMap( Table, TableFilter.m_nLevel );
					}
				}
			}
			break;
		}

		for ( int n = 0; n < DataManagerNonDb.TableGroups.GetSize(); n++ )
		{
			CTableGroupCSVRecord TableGroup;
			DataManagerNonDb.TableGroups.GetAt( n, TableGroup );
			m_arrayGroupNames.Add( TableGroup.GetGroupName(TableFilter.m_nLevel) );
		}

		CloseTableData();
	}
}

/**********************************************************************/

void CTableReportHelpers::OpenTableData()
{
	CDataManagerInfo infoDM;
	DataManagerNonDb.OpenTableNames( DB_READONLY, m_nLocIdx, infoDM );
	DataManagerNonDb.OpenTableGroups( DB_READONLY, m_nLocIdx, infoDM );
	DataManagerNonDb.OpenTableGroupSets( DB_READONLY, m_nLocIdx, infoDM );
}

/**********************************************************************/

void CTableReportHelpers::CloseTableData()
{
	CDataManagerInfo infoDM;
	DataManagerNonDb.CloseTableNames( infoDM );
	DataManagerNonDb.CloseTableGroups( infoDM );
	DataManagerNonDb.CloseTableGroupSets( infoDM );
}

/**********************************************************************/

void CTableReportHelpers::AddTableToGroupMap( CTableNameCSVRecord& Table, int nLevel )
{
	{
		CSortedIntByInt item;
		item.m_nKey = Table.GetTableNo();
		item.m_nVal = Table.GetReportGroup(nLevel);
		m_GroupMap.DirectAdd( item );
	}

	{
		CSortedStringByInt item;
		item.m_nItem = Table.GetTableNo();
		item.m_strItem = Table.GetTableName();
		m_arrayTableNames.DirectAdd( item );
	}
}

/**********************************************************************/

bool CTableReportHelpers::CheckTableGroup( int nTableNo, int& nGroupNo )
{
	//NON TABLE SALES
	if ( nTableNo <= 0 )
	{		
		nGroupNo = -1;				
		return m_bIncludeNonTable;
	}

	//SAME TABLE NUMBER AS BEFORE
	if ( nTableNo == m_nLastLookupTableNo )
	{
		nGroupNo = m_nLastLookupGroupNo;
		return m_bLastLookupResponse;
	}

	CSortedIntByInt item;
	item.m_nKey = nTableNo;

	int nItemIdx;
	if ( m_GroupMap.Find( item, nItemIdx ) == TRUE )
	{
		m_GroupMap.GetAt( nItemIdx, item );

		nGroupNo = item.m_nVal;
		m_bLastLookupResponse = TRUE;
	}
	else
	{
		nGroupNo = 0;
		m_bLastLookupResponse = m_bIncludeUnknown;
	}

	m_nLastLookupTableNo = nTableNo;
	m_nLastLookupGroupNo = nGroupNo;

	return m_bLastLookupResponse;
}

/**********************************************************************/

const char* CTableReportHelpers::GetTableName( int nTableNo )
{
	if ( nTableNo <= 0 )
		m_strTableName = "N/A";
	else
	{
		CSortedStringByInt item;
		item.m_nItem = nTableNo;

		int nTableIdx;
		if ( m_arrayTableNames.Find( item, nTableIdx ) == TRUE )
		{
			m_arrayTableNames.GetAt( nTableIdx, item );
			m_strTableName = item.m_strItem;
		}
		else
		{
			m_strTableName.Format ( "Table %d", nTableNo );
		}
	}

	return m_strTableName;
}

/**********************************************************************/

const char* CTableReportHelpers::GetGroupName( int nGroupNo )
{
	switch( nGroupNo )
	{
	case -1:
		m_strGroupName = "Non Table Sales";
		break;

	case 0:
		m_strGroupName = "Unknown Tables";
		break;

	default:
		if ( ( nGroupNo >= 1 ) && ( nGroupNo <= m_arrayGroupNames.GetSize() ) )
		{
			m_strGroupName = m_arrayGroupNames.GetAt( nGroupNo - 1 );
		}
		else
		{
			m_strGroupName.Format( "Unknown (%d)", nGroupNo );
		}
		break;
	}

	return m_strGroupName;
}

/**********************************************************************/
