/**********************************************************************/
 
/**********************************************************************/
//#include "EcrmanOptionsIni.h"
#include "LocationCSVArray.h"
#include "KeyboardSetCSVArray.h"
#include "NetworkCSVArray.h"
#include "NodeTypes.h"
/**********************************************************************/
#include "SelectMultipleKbdCloneTargetDlg.h"
/**********************************************************************/

CSelectMultipleKbdCloneTargetDlg::CSelectMultipleKbdCloneTargetDlg( const char* szTitle, CArray<CSelectMultipleEntityInfo,CSelectMultipleEntityInfo>& arrayEntityInfo, bool bSingle, CWnd* pParent )
	: CSelectMultipleEntityDlg( szTitle, arrayEntityInfo, TRUE, FALSE, pParent )
{
	m_bSingle = bSingle;
	m_strOption = ( m_bSingle ) ? "Copy keyboard name to destination keyboards" : "";
}

/**********************************************************************/

void CSelectMultipleKbdCloneTargetDlg::SetListColumnns()
{
	if ( FALSE == m_bSingle )
	{
		m_listEntities.InsertColumn ( 0, "Network", LVCFMT_LEFT, 220 );
		m_listEntities.InsertColumn ( 1, "Location", LVCFMT_LEFT, 220 );
	}
	else
	{
		m_listEntities.InsertColumn ( 0, "Network", LVCFMT_LEFT, 195 );
		m_listEntities.InsertColumn ( 1, "Location", LVCFMT_LEFT, 195 );
		m_listEntities.InsertColumn ( 2, "KbNo", LVCFMT_LEFT, 50 );
	}
}

/**********************************************************************/

void CSelectMultipleKbdCloneTargetDlg::GetListData()
{
	m_nColumnCount = ( m_bSingle ) ? 3 : 2;

	int nLastNodeType = -1;
	int nLastLocIdx = -1;
	int nLastDbIdx = -1;
	int nLastKbsIdx = -1;

	for ( int nIndex = 0; nIndex < m_arrayEntityInfo.GetSize(); nIndex++ )
	{
		CSelectMultipleEntityInfo infoNode = m_arrayEntityInfo[nIndex];

		int nThisNodeType = infoNode.m_nNodeType;
		int nThisLocIdx = infoNode.m_nLocIdx;
		int nThisDbIdx = infoNode.m_nDbIdx;
		int nThisKbsIdx = infoNode.m_nKbsIdx;

		if ( NODE_LOCATION == nThisNodeType )
		{
			if ( nThisLocIdx < 0 || nThisLocIdx >= dbLocation.GetSize() )
			{
				m_arrayEntityInfo.RemoveAt(nIndex--);
				continue;
			}

			int nNetworkIdx;
			int nNetworkNo = dbLocation.GetNetworkNo ( nThisLocIdx );
			
			if ( dbNetwork.FindNetworkByNumber ( nNetworkNo, nNetworkIdx ) )
				infoNode.m_strText[0] = dbNetwork.GetName ( nNetworkIdx );
			else
				infoNode.m_strText[0] = "Unknown";

			infoNode.m_strText[1] = dbLocation.GetName( nThisLocIdx );
		}
		else 
		{
			if ( nThisDbIdx < 0 || nThisDbIdx >= dbDatabase.GetSize() )
			{
				m_arrayEntityInfo.RemoveAt(nIndex--);
				continue;
			}

			infoNode.m_strText[0] = dbDatabase.GetName( nThisDbIdx );
			infoNode.m_strText[1] = dbKeyboardSet.GetName( nThisKbsIdx );
		}

		if ( TRUE == m_bSingle )
		{
			infoNode.m_strText[2].Format( "%2.2d", m_arrayEntityInfo[nIndex].m_nKbNo );
			if ( ( nThisNodeType == nLastNodeType ) && ( nThisLocIdx == nLastLocIdx ) && ( nThisDbIdx == nLastDbIdx ) && ( nThisKbsIdx == nLastKbsIdx ) )
				infoNode.m_strText[0] = "";
		}
		
		infoNode.m_bSelected = FALSE;
		m_arrayEntityInfo[nIndex] = infoNode;

		nLastNodeType = nThisNodeType;
		nLastDbIdx = nThisDbIdx;
		nLastLocIdx = nThisLocIdx;
		nLastKbsIdx = nThisKbsIdx;
	}
}

/**********************************************************************/

bool CSelectMultipleKbdCloneTargetDlg::HandleOK() 
{
	CSelectMultipleEntityDlg::HandleOK();
	m_bCopyNames = ( m_checkOption.GetCheck() != 0 );
	return TRUE;
}

/**********************************************************************/