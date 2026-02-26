/**********************************************************************/
 
/**********************************************************************/
#include "StockpointCSVArray.h"
/**********************************************************************/
#include "SelectMultipleStockpointDlg.h"
/**********************************************************************/

CSelectMultipleStockpointDlg::CSelectMultipleStockpointDlg( const char* szTitle, CArray<CSelectMultipleEntityInfo,CSelectMultipleEntityInfo>& arrayEntityInfo, CWnd* pParent )
	: CSelectMultipleEntityDlg( szTitle, arrayEntityInfo, TRUE, FALSE, pParent)
{
}

/**********************************************************************/

void CSelectMultipleStockpointDlg::SetListColumnns()
{
	m_listEntities.InsertColumn ( 0, "Stockpoint", LVCFMT_LEFT, 440 );
}

/**********************************************************************/

void CSelectMultipleStockpointDlg::GetListData() 
{
	m_nColumnCount = 1;

	for ( int nIndex = 0; nIndex < m_arrayEntityInfo.GetSize(); nIndex++ )
	{
		m_arrayEntityInfo[nIndex].m_strText[0] = dbStockpoint.GetName( m_arrayEntityInfo[nIndex].m_nSpIdx );
		m_arrayEntityInfo[nIndex].m_bSelected = FALSE;
	}
}

/**********************************************************************/
