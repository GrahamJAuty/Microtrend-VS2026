/**********************************************************************/
#include "ListDataTableGroupDlg.h"
/**********************************************************************/
#include "SelectTableGroupDlg.h"
/**********************************************************************/

CSelectTableGroupDlg::CSelectTableGroupDlg( int nLocIdx, CWnd* pParent /*=NULL*/)
	: CReportDataTableDlg( pParent)
{
	m_strTitle = "Edit Table Groups";
	m_arrayReports.Add( "Table Groups 1" );
	m_arrayReports.Add( "Table Groups 2" );
	m_arrayReports.Add( "Table Groups 3" );
	m_nLocIdx = nLocIdx;
	m_bKeepOpen = TRUE;
}

/**********************************************************************/

void CSelectTableGroupDlg::HandleInitDialog()
{
	m_buttonOK.SetWindowText( "Continue" );
	m_buttonCancel.SetWindowText( "Close" );
}

/**********************************************************************/

void CSelectTableGroupDlg::HandleReport1()
{
	EditGroups(1);
}

/**********************************************************************/

void CSelectTableGroupDlg::HandleReport2()
{
	EditGroups(2);
}

/**********************************************************************/

void CSelectTableGroupDlg::HandleReport3()
{
	EditGroups(3);
}

/**********************************************************************/

void CSelectTableGroupDlg::EditGroups( int nLevel )
{
	CDataManagerInfo infoDM;
	if ( DataManagerNonDb.OpenTableGroups( DB_READWRITE, m_nLocIdx, infoDM ) == TRUE )
	{
		DataManagerNonDb.OpenTableNames( DB_READONLY, m_nLocIdx, infoDM );

		CListDataTableGroupDlg dlgEdit( m_nLocIdx, nLevel, this );
		if ( dlgEdit.DoModal() == IDOK )
		{
			if ( DataManagerNonDb.WriteTableGroups( infoDM ) == FALSE )
			{
				Prompter.WriteError( infoDM );
			}
		}

		DataManagerNonDb.CloseTableNames( infoDM );
		DataManagerNonDb.CloseTableGroups( infoDM );
	}
	else
	{
		Prompter.ShareError( infoDM );
	}
}

/**********************************************************************/
