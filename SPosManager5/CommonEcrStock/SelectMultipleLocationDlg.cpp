/**********************************************************************/
#include "LocationCSVArray.h"
#include "NetworkCSVArray.h"
/**********************************************************************/
#include "SelectMultipleLocationDlg.h"
/**********************************************************************/

void CSelectMultipleLocationDlg::CreateLocInfoArray( CArray<int,int>& arrayLocIdx, CArray<CSelectMultipleEntityInfo,CSelectMultipleEntityInfo>& arrayLocInfo )
{
	for ( int n = 0; n < arrayLocIdx.GetSize(); n++ )
	{
		int nLocIdx = arrayLocIdx.GetAt(n);

		CSelectMultipleEntityInfo infoNode;

		if ( dbNetwork.FindNetworkByNumber( dbLocation.GetNetworkNo( nLocIdx ), infoNode.m_nNwkIdx ) == FALSE )
			infoNode.m_nNwkIdx = -1;

		infoNode.m_nLocIdx = nLocIdx;
		infoNode.m_bSelected = FALSE;
		arrayLocInfo.Add( infoNode );
	}
}

/**********************************************************************/

bool CSelectMultipleLocationDlg::FilterLocListForDownload( CArray<int,int>& arrayLocIdx, const char* szTitle, CWnd* pParent )
{
	if ( arrayLocIdx.GetSize() > 1 )
	{
		if ( EcrmanOptions.GetReportsLocationSortType() != 0 )
			dbLocation.SortLocListByName( arrayLocIdx );

		CArray<CSelectMultipleEntityInfo,CSelectMultipleEntityInfo> arrayLocInfo;
		CreateLocInfoArray( arrayLocIdx, arrayLocInfo );
		arrayLocIdx.RemoveAll();

		CSelectMultipleLocationDlg dlgSelect( szTitle, arrayLocInfo, FALSE, pParent );
		
		if ( dlgSelect.DoModal() == IDOK )
		{
			for ( int n = 0; n < arrayLocInfo.GetSize(); n++ )
				if ( TRUE == arrayLocInfo[n].m_bSelected )
					arrayLocIdx.Add( arrayLocInfo[n].m_nLocIdx );
		}
	}

	return ( arrayLocIdx.GetSize() != 0 );
}

/**********************************************************************/

CSelectMultipleLocationDlg::CSelectMultipleLocationDlg( const char* szTitle, CArray<CSelectMultipleEntityInfo,CSelectMultipleEntityInfo>& arrayEntityInfo, bool bAutoSelectSingle, CWnd* pParent )
	: CSelectMultipleEntityDlg( szTitle, arrayEntityInfo, TRUE, bAutoSelectSingle, pParent )
{
}

/**********************************************************************/

void CSelectMultipleLocationDlg::SetListColumnns()
{
	m_listEntities.InsertColumn ( 0, "Network", LVCFMT_LEFT, 160 );
	m_listEntities.InsertColumn ( 1, "Version", LVCFMT_LEFT, 80 );
	m_listEntities.InsertColumn ( 2, "Location", LVCFMT_LEFT, 200 );
}

/**********************************************************************/

void CSelectMultipleLocationDlg::GetListData()
{
	m_nColumnCount = 3;

	for ( int nIndex = 0; nIndex < m_arrayEntityInfo.GetSize(); nIndex++ )
	{
		CSelectMultipleEntityInfo infoNode = m_arrayEntityInfo[nIndex];
		
		if ( ( infoNode.m_nNwkIdx >= 0 ) && ( infoNode.m_nNwkIdx < dbNetwork.GetSize() ) )
			infoNode.m_strText[0] = dbNetwork.GetName ( infoNode.m_nNwkIdx );
		else
			infoNode.m_strText[0] = "Unknown";
		
		{
			int nSPOSVersion;
			int nConnectionType;
			dbLocation.GetLocSPOSVersion( infoNode.m_nLocIdx, nSPOSVersion, nConnectionType );
			CNetworkCSVRecord::GetVersionNumberExternal( nConnectionType, nSPOSVersion, infoNode.m_strText[1] );
		}
			
		infoNode.m_strText[2] = dbLocation.GetName( infoNode.m_nLocIdx );	
		
		m_arrayEntityInfo[nIndex] = infoNode;
	}
}

/**********************************************************************/