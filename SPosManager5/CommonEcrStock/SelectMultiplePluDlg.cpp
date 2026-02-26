/**********************************************************************/
 
/**********************************************************************/
//#include "DataManager.h"
#include "ReportHelpers.h"
/**********************************************************************/
#include "..\CommonEcrStock\SelectMultiplePluDlg.h"
/**********************************************************************/

CSelectMultiplePluDlg::CSelectMultiplePluDlg( CArray<CSelectMultipleEntityInfo,CSelectMultipleEntityInfo>& arrayEntityInfo, const char* szStockCode, CWnd* pParent )
	: CSelectMultipleEntityDlg( "", arrayEntityInfo, TRUE, FALSE, pParent )
{
	m_strTitle.Format ( "Enable Plu Items at Linked Locations (Stock Code: %s)", szStockCode );
}

/**********************************************************************/

void CSelectMultiplePluDlg::SetListColumnns()
{
	m_listEntities.InsertColumn ( 0, "Plu Number", LVCFMT_LEFT, 140 );
	m_listEntities.InsertColumn ( 1, "Description", LVCFMT_LEFT, 300 );
	ShowAndEnableWindow( &m_buttonCancel, FALSE );
	MoveControl( &m_buttonOK, 343, 264 );
}

/**********************************************************************/

void CSelectMultiplePluDlg::GetListData() 
{
	m_nColumnCount = 2;
	
	for ( int nIndex = 0; nIndex < m_arrayEntityInfo.GetSize(); nIndex++ )
	{
		CSelectMultipleEntityInfo infoNode = m_arrayEntityInfo[nIndex];

		::FormatPluNo( infoNode.m_nPluNo, infoNode.m_strText[0] );
		
		int nPluIdx;
		if ( DataManager.Plu.FindPluByNumber ( infoNode.m_nPluNo, nPluIdx ) == TRUE )
		{
			CPluCSVRecord PluRecord;
			DataManager.Plu.GetAt ( nPluIdx, PluRecord );
			infoNode.m_strText[1] = ReportHelpers.GetReportText( PluRecord );
		}
		else
		{
			infoNode.m_strText[1] = "Unknown";
		}

		infoNode.m_bSelected = TRUE;
		m_arrayEntityInfo[nIndex] = infoNode;
	}  
}

/**********************************************************************/

void CSelectMultiplePluDlg::HandleCancel()
{
	HandleOK();
	m_bEndAsCancel = FALSE;
}

/**********************************************************************/
