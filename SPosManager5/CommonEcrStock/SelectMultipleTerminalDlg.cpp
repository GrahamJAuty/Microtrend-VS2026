/**********************************************************************/
 
/**********************************************************************/
#include "LocationCSVArray.h"
/**********************************************************************/
#include "SelectMultipleTerminalDlg.h"
/**********************************************************************/

CSelectMultipleTerminalDlg::CSelectMultipleTerminalDlg( const char* szTitle, CArray<CSelectMultipleEntityInfo,CSelectMultipleEntityInfo>& arrayEntityInfo, CWnd* pParent )
	: CSelectMultipleEntityDlg( szTitle, arrayEntityInfo, TRUE, TRUE, pParent )
{
}

/**********************************************************************/

void CSelectMultipleTerminalDlg::SetListColumnns()
{
	m_listEntities.InsertColumn ( 0, "Location", LVCFMT_LEFT, 220 );
	m_listEntities.InsertColumn ( 1, "Terminal", LVCFMT_LEFT, 220 );
}

/**********************************************************************/

void CSelectMultipleTerminalDlg::GetListData()
{
	m_nColumnCount = 2;

	for ( int nIndex = 0; nIndex < m_arrayEntityInfo.GetSize(); nIndex++ )
	{		
		CSelectMultipleEntityInfo infoNode = m_arrayEntityInfo[nIndex];

		infoNode.m_strText[0] = dbLocation.GetName ( infoNode.m_nLocIdx );
		infoNode.m_strText[1].Format( "TNo %3.3d (%s)",
			dbLocation.GetTNo( infoNode.m_nLocIdx, infoNode.m_nTermIdx ),
			dbLocation.GetTerminalName( infoNode.m_nLocIdx, infoNode.m_nTermIdx ) );
		
		m_arrayEntityInfo[nIndex] = infoNode;
	}
}

/**********************************************************************/
