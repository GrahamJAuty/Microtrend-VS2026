/**********************************************************************/
 
/**********************************************************************/
#include "NetworkCSVArray.h"
/**********************************************************************/
#include "SelectMultipleNetworkDlg.h"
/**********************************************************************/

CSelectMultipleNetworkDlg::CSelectMultipleNetworkDlg( const char* szTitle, CArray<CSelectMultipleEntityInfo,CSelectMultipleEntityInfo>& arrayEntityInfo, CWnd* pParent )
	: CSelectMultipleEntityDlg( szTitle, arrayEntityInfo, TRUE, FALSE, pParent )
{
}

/**********************************************************************/

void CSelectMultipleNetworkDlg::SetListColumnns()
{
	m_listEntities.InsertColumn ( 0, "Network", LVCFMT_LEFT, 440 );
}

/**********************************************************************/

void CSelectMultipleNetworkDlg::GetListData()
{
	//FIDDLE : SORT NETWORK NAMES BEFORE CALLING THIS FUNCTION
	m_nColumnCount = 1;
	
	for ( int nIndex = 0; nIndex < m_arrayEntityInfo.GetSize(); nIndex++ )
		m_arrayEntityInfo[nIndex].m_strText[0] = dbNetwork.GetName( m_arrayEntityInfo[nIndex].m_nNwkIdx );		  
}

/**********************************************************************/
