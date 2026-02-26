/**********************************************************************/
#include "LocationMasterMap.h"
#include "NetworkCSVArray.h"
/**********************************************************************/
#include "SelectMultipleMasterDlg.h"
/**********************************************************************/

CSelectMultipleMasterDlg::CSelectMultipleMasterDlg( CArray<CSelectMultipleEntityInfo,CSelectMultipleEntityInfo>& arrayEntityInfo, CWnd* pParent )
	: CSelectMultipleEntityDlg( "Select Master Terminals for Download", arrayEntityInfo, TRUE, FALSE, pParent )
{
}

/**********************************************************************/

void CSelectMultipleMasterDlg::SetListColumnns()
{
	m_listEntities.InsertColumn ( 0, "Network", LVCFMT_LEFT, 160 );
	m_listEntities.InsertColumn ( 1, "Version", LVCFMT_LEFT, 80 );
	m_listEntities.InsertColumn ( 2, "Location", LVCFMT_LEFT, 200 );
}

/**********************************************************************/

void CSelectMultipleMasterDlg::GetListData()
{
	m_nColumnCount = 3;

	for ( int nIndex = m_arrayEntityInfo.GetSize() - 1; nIndex >= 0; nIndex-- )
	{
		CSelectMultipleEntityInfo infoNode = m_arrayEntityInfo[nIndex];

		if ( infoNode.m_nLocIdx < 0 || infoNode.m_nLocIdx >= dbLocation.GetSize() )
		{
			m_arrayEntityInfo.RemoveAt(nIndex);
			continue;
		}

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

		{
			if ( 0 == infoNode.m_nTNo )
				infoNode.m_strText[2] = dbLocation.GetName( infoNode.m_nLocIdx );
			else
			{
				infoNode.m_strText[2] = "==> ";
				infoNode.m_strText[2] += LocationMasterMap.GetMasterName( infoNode.m_nLocIdx, infoNode.m_nTNo );
			}
		}
				
		m_arrayEntityInfo[nIndex] = infoNode;
	}
}

/**********************************************************************/
