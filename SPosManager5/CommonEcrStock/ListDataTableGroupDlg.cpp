/**********************************************************************/
#include "CCSVReportLine.h"
#include "DefEditTableGroupDlg.h"
#include "LocationCSVArray.h"
#include "Repspawn.h"
/**********************************************************************/
#include "ListDataTableGroupDlg.h"
/**********************************************************************/

CSingleListContentHelpersTableGroup::CSingleListContentHelpersTableGroup( int nLevel )
{
	m_nLevel = nLevel;
}

/**********************************************************************/

int CSingleListContentHelpersTableGroup::GetArraySize()
{
	return DataManagerNonDb.TableGroups.GetSize();
}

/**********************************************************************/

bool CSingleListContentHelpersTableGroup::PrepareLine( int nIndex, CString& strLine )
{
	strLine = "";

	bool bResult = FALSE;
	if ( ( nIndex >= 0 ) && ( nIndex < GetArraySize() ) )
	{
		CTableGroupCSVRecord TableGroupRecord;
		DataManagerNonDb.TableGroups.GetAt ( nIndex, TableGroupRecord );
		
		strLine.Format( "%s\t%s", 
			TableGroupRecord.GetGroupName(m_nLevel),
			TableGroupRecord.GetShowAsFilterFlag(m_nLevel) ? "Yes" : "No" );
		
		bResult = TRUE;
	}
	
	return bResult;
}

/**********************************************************************/

void CSingleListContentHelpersTableGroup::SetColumns( CSSListCtrlVirtual& list, CWnd* pParent )
{
	list.SubclassDlgItem ( IDC_LIST, pParent );
	list.InsertColumn ( 0, "Name", LVCFMT_LEFT, 360 );
	list.InsertColumn ( 1, "Report Filter", LVCFMT_LEFT, 120 );
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CListDataTableGroupDlg::CListDataTableGroupDlg( int nLocIdx, int nLevel, CWnd* pParent) : CListDataDlg ( pParent )
{
	m_nLocIdx = nLocIdx;
	m_nLevel = nLevel;

	m_pContentHelpers = new CSingleListContentHelpersTableGroup( m_nLevel );
	m_pDisplayHelpers = new CSingleListDisplayHelpers( m_listData, *m_pContentHelpers );

	m_pDisplayHelpers -> SetTopSingleText( "Table Group" );
	m_pDisplayHelpers -> SetTopPluralText( "Table Groups" );
	m_pDisplayHelpers -> SetBottomSingleText( "Table Group" );
	m_pDisplayHelpers -> SetBottomPluralText( "Table Groups" );
}

/**********************************************************************/

CListDataTableGroupDlg::~CListDataTableGroupDlg()
{
	delete m_pDisplayHelpers;
	delete m_pContentHelpers;
}

/**********************************************************************/

void CListDataTableGroupDlg::SetTitle()
{
	CString strTitle;
	strTitle.Format( "Setup Table Groups (%s, Batch %d)", dbLocation.GetName(m_nLocIdx), m_nLevel );
	SetWindowText ( strTitle );
}

/**********************************************************************/

void CListDataTableGroupDlg::SetButtonTexts()
{
	m_button1.SetWindowText ( "&Edit" );
	m_button2.ShowWindow ( SW_HIDE );
	m_button3.SetWindowText ( "&Print" );
	m_button4.ShowWindow ( SW_HIDE );
	m_button5.ShowWindow ( SW_HIDE );
	m_button6.ShowWindow ( SW_HIDE );

	MoveControl( &m_button1, 368, 7 );
	MoveControl( &m_button2, 368, 47 );
	MoveControl( &m_button3, 368, 87 );
	MoveControl( &m_button4, 368, 147 );
	MoveControl( &m_button5, 368, 187 );
}

/**********************************************************************/

void CListDataTableGroupDlg::SetColumns()
{
	m_pContentHelpers -> SetColumns( m_listData, this );
}

/**********************************************************************/

void CListDataTableGroupDlg::HandleButton1() 
{
	int nGroupIdx = GetSafeSelection();
	if ( nGroupIdx == -1 )
		return;
	
	CDefEditTableGroupDlg dlgEdit( nGroupIdx, m_nLevel, m_WarnCancelAfterChange, this );
	dlgEdit.SetListDataDlg( this );
	dlgEdit.DoModal();

	SelectLine ( dlgEdit.GetIndex() );
}

/**********************************************************************/

void CListDataTableGroupDlg::HandleButton3() 
{
	//SORT TABLES BY GROUP
	CStringArray arrayTables;
	for ( int n = 0; n < MAX_TABLEGROUP; n++ )
	{
		arrayTables.Add( "" );
	}

	{
		bool bGotActive = FALSE;
		for ( int nTableIdx = 0; nTableIdx < DataManagerNonDb.TableNames.GetSize(); nTableIdx++ )
		{
			CTableNameCSVRecord Table;
			DataManagerNonDb.TableNames.GetAt( nTableIdx, Table );
			int nGroupIdx = Table.GetReportGroup( m_nLevel ) - 1;

			if ( ( nGroupIdx >= 0 ) && ( nGroupIdx < arrayTables.GetSize() ) )
			{
				CString strTableList = arrayTables.GetAt( nGroupIdx );
				
				CString strTableInfo;
				strTableInfo.Format( "%s%3.3d %s", 
					( strTableList == "" ) ? "" : ",",
					Table.GetTableNo(),
					Table.GetTableName() );
				
				arrayTables.SetAt( nGroupIdx, strTableList + strTableInfo );
				bGotActive = TRUE;
			}
		}

		if ( FALSE == bGotActive )
		{
			Prompter.Error ( "You must link some tables to groups\nbefore you can print this report" );
			return;
		}
	}

	{
		CReportFile ReportFile;
		if ( ReportFile.Open ( Super.ReportFilename() ) == FALSE )
			return;

		{
			CString strTitle;
			strTitle.Format( "Table Groups (%s, Batch %d)",
				dbLocation.GetName( m_nLocIdx ),
				m_nLevel );

			ReportFile.SetStyle1 ( strTitle );
		}
		
		ReportFile.AddColumn ( "No", TA_LEFT, 100 );
		ReportFile.AddColumn ( "Name", TA_LEFT, 500 );
		ReportFile.AddColumn ( "Report Filter", TA_LEFT, 250 );
		ReportFile.AddColumn ( "Tables", TA_LEFT, 500 );

		for ( int nGroupIdx = 0; ( nGroupIdx < DataManagerNonDb.TableGroups.GetSize() ) && ( nGroupIdx < MAX_TABLEGROUP ); nGroupIdx++ )
		{
			CCSV csvTables( arrayTables.GetAt( nGroupIdx ) );
			if ( csvTables.GetSize() == 0 )
				continue;
			
			CTableGroupCSVRecord Group;
			DataManagerNonDb.TableGroups.GetAt ( nGroupIdx, Group );

			CCSVReportLine csvOut;
			csvOut.AppendInt( nGroupIdx + 1 );
			csvOut.AppendString( Group.GetGroupName(m_nLevel) );
			csvOut.AppendYesOrBlank( Group.GetShowAsFilterFlag(m_nLevel) );
			csvOut.AppendString( csvTables.GetString(0) );
			ReportFile.WriteLine ( csvOut.GetLine() );
			
			for ( int t = 1; t < csvTables.GetSize(); t++ )
			{
				CString strLine = "\t\t\t";
				strLine += csvTables.GetString(t);
				ReportFile.WriteLine( strLine );
			}
			
			ReportFile.WriteLine ( "<LI>" );
		}
	
		ReportFile.Close();

		CReportManager ReportManager;
		CFilenameUpdater FnUp ( SysFiles::TableGroupPrm );
		ReportManager.DisplayReport ( Super.ReportFilename(), FnUp.GetFilenameToUse(), "", this );
	}
}
		
/**********************************************************************/

