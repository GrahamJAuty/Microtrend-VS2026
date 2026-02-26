/**********************************************************************/
#include "ListDataTableGroupSetDlg.h"
/**********************************************************************/
#include "SelectTableGroupSetDlg.h"
/**********************************************************************/

CSelectTableGroupSetDlg::CSelectTableGroupSetDlg( int nLocIdx, CWnd* pParent /*=NULL*/)
	: CReportDataTableDlg( pParent)
{
	m_strTitle = "Edit Table Group Sets";
	m_arrayReports.Add( "Table Group Sets 1" );
	m_arrayReports.Add( "Table Group Sets 2" );
	m_arrayReports.Add( "Table Group Sets 3" );
	m_nLocIdx = nLocIdx;
	m_bKeepOpen = TRUE;
}

/**********************************************************************/

void CSelectTableGroupSetDlg::HandleInitDialog()
{
	m_buttonOK.SetWindowText( "Continue" );
	m_buttonCancel.SetWindowText( "Close" );
}

/**********************************************************************/

void CSelectTableGroupSetDlg::HandleReport1()
{
	EditGroupSets(1);
}

/**********************************************************************/

void CSelectTableGroupSetDlg::HandleReport2()
{
	EditGroupSets(2);
}

/**********************************************************************/

void CSelectTableGroupSetDlg::HandleReport3()
{
	EditGroupSets(3);
}

/**********************************************************************/

void CSelectTableGroupSetDlg::EditGroupSets( int nLevel )
{
	CDataManagerInfo infoDM;
	if ( DataManagerNonDb.OpenTableGroupSets( DB_READWRITE, m_nLocIdx, infoDM ) == TRUE )
	{
		DataManagerNonDb.OpenTableGroups( DB_READONLY, m_nLocIdx, infoDM );

		CListDataTableGroupSetDlg dlgEdit( m_nLocIdx, nLevel, this );
		if ( dlgEdit.DoModal() == IDOK )
		{
			if ( DataManagerNonDb.WriteTableGroupSets( infoDM ) == FALSE )
			{
				Prompter.WriteError( infoDM );
			}
		}

		DataManagerNonDb.CloseTableGroups( infoDM );
		DataManagerNonDb.CloseTableGroupSets( infoDM );
	}
	else
	{
		Prompter.ShareError( infoDM );
	}
}

/**********************************************************************/
