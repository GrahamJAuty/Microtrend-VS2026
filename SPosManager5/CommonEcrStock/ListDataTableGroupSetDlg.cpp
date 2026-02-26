/**********************************************************************/
#include "CCSVReportLine.h"
#include "DefEditTableGroupSetDlg.h"
#include "LocationCSVArray.h"
#include "RepSpawn.h"
/**********************************************************************/
#include "ListDataTableGroupSetDlg.h"
/**********************************************************************/

CSingleListContentHelpersTableGroupSet::CSingleListContentHelpersTableGroupSet( int nLevel )
{
	m_nLevel = nLevel;
}

/**********************************************************************/

int CSingleListContentHelpersTableGroupSet::GetArraySize()
{
	return DataManagerNonDb.TableGroupSets.GetSize();
}

/**********************************************************************/

bool CSingleListContentHelpersTableGroupSet::PrepareLine( int nIndex, CString& strLine )
{
	strLine = "";

	bool bResult = FALSE;
	if ( ( nIndex >= 0 ) && ( nIndex < GetArraySize() ) )
	{
		CTableGroupSetCSVRecord TableGroupSetRecord;
		DataManagerNonDb.TableGroupSets.GetAt ( nIndex, TableGroupSetRecord );
		
		strLine.Format( "%s\t%s", 
			TableGroupSetRecord.GetGroupSetName(m_nLevel ),
			TableGroupSetRecord.GetShowAsFilterFlag( m_nLevel ) ? "Yes" : "No" );
		
		bResult = TRUE;
	}
	
	return bResult;
}

/**********************************************************************/

void CSingleListContentHelpersTableGroupSet::SetColumns( CSSListCtrlVirtual& list, CWnd* pParent )
{
	list.SubclassDlgItem ( IDC_LIST, pParent );
	list.InsertColumn ( 0, "Name", LVCFMT_LEFT, 360 );
	list.InsertColumn ( 1, "Report Filter", LVCFMT_LEFT, 120 );
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CListDataTableGroupSetDlg::CListDataTableGroupSetDlg( int nLocIdx, int nLevel, CWnd* pParent) : CListDataDlg ( pParent )
{
	m_nLocIdx = nLocIdx;
	m_nLevel = nLevel;

	m_pContentHelpers = new CSingleListContentHelpersTableGroupSet( m_nLevel );
	m_pDisplayHelpers = new CSingleListDisplayHelpers( m_listData, *m_pContentHelpers );

	m_pDisplayHelpers -> SetTopSingleText( "Table Group Set" );
	m_pDisplayHelpers -> SetTopPluralText( "Table Group Sets" );
	m_pDisplayHelpers -> SetBottomSingleText( "Table Group Set" );
	m_pDisplayHelpers -> SetBottomPluralText( "Table Group Sets" );
}

/**********************************************************************/

CListDataTableGroupSetDlg::~CListDataTableGroupSetDlg()
{
	delete m_pDisplayHelpers;
	delete m_pContentHelpers;
}

/**********************************************************************/

void CListDataTableGroupSetDlg::SetTitle()
{
	CString strTitle;
	strTitle.Format( "Setup Table Group Sets (%s, Batch %d)", dbLocation.GetName(m_nLocIdx), m_nLevel );
	SetWindowText ( strTitle );
}

/**********************************************************************/

void CListDataTableGroupSetDlg::SetButtonTexts()
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

void CListDataTableGroupSetDlg::SetColumns()
{
	m_pContentHelpers -> SetColumns( m_listData, this );
}

/**********************************************************************/

void CListDataTableGroupSetDlg::HandleButton1() 
{
	int nGroupSetIdx = GetSafeSelection();
	if ( nGroupSetIdx == -1 )
		return;
	
	CDefEditTableGroupSetDlg dlgEdit( nGroupSetIdx, m_nLevel, m_WarnCancelAfterChange, this );
	dlgEdit.SetListDataDlg( this );
	dlgEdit.DoModal();

	SelectLine ( dlgEdit.GetIndex() );
}

/**********************************************************************/

void CListDataTableGroupSetDlg::HandleButton3() 
{
	//SORT GROUPS BY SET
	{
		bool bGotActive = FALSE;
		for ( int nSetIdx = 0; ( nSetIdx < DataManagerNonDb.TableGroupSets.GetSize() ) && ( nSetIdx < MAX_TABLEGROUPSET ); nSetIdx++ )
		{
			CTableGroupSetCSVRecord GroupSet;
			DataManagerNonDb.TableGroupSets.GetAt ( nSetIdx, GroupSet );
			if ( GroupSet.IsEmpty(m_nLevel) == FALSE )
			{
				bGotActive = TRUE;
				break;
			}
		}

		if ( FALSE == bGotActive )
		{
			Prompter.Error ( "You must link some groups to sets\nbefore you can print this report" );
			return;
		}
	}

	{
		CReportFile ReportFile;
		if ( ReportFile.Open ( Super.ReportFilename() ) == FALSE )
			return;

		{
			CString strTitle;
			strTitle.Format( "Table Group Sets (%s, Batch %d)",
				dbLocation.GetName( m_nLocIdx ),
				m_nLevel );

			ReportFile.SetStyle1 ( strTitle );
		}
		
		ReportFile.AddColumn ( "No", TA_LEFT, 100 );
		ReportFile.AddColumn ( "Name", TA_LEFT, 500 );
		ReportFile.AddColumn ( "Report Filter", TA_LEFT, 250 );
		ReportFile.AddColumn ( "Unknown Tables", TA_LEFT, 320);
		ReportFile.AddColumn ( "Non Table Sales", TA_LEFT, 300 );
		ReportFile.AddColumn ( "Groups", TA_LEFT, 500 );

		for ( int nSetIdx = 0; ( nSetIdx < DataManagerNonDb.TableGroupSets.GetSize() ) && ( nSetIdx < MAX_TABLEGROUPSET ); nSetIdx++ )
		{
			CTableGroupSetCSVRecord GroupSet;
			DataManagerNonDb.TableGroupSets.GetAt ( nSetIdx, GroupSet );

			if ( GroupSet.IsEmpty(m_nLevel) == FALSE )
			{
				CCSVReportLine csvOut;
				csvOut.AppendInt( nSetIdx + 1 );
				csvOut.AppendString( GroupSet.GetGroupSetName(m_nLevel) );
				csvOut.AppendYesOrBlank( GroupSet.GetShowAsFilterFlag(m_nLevel) );
				csvOut.AppendYesOrBlank( GroupSet.GetIncludeUnknownFlag(m_nLevel) );
				csvOut.AppendYesOrBlank( GroupSet.GetIncludeNonTableFlag(m_nLevel) );

				int nGroupMask = 1;
				int nSetMask = GroupSet.GetGroupMask(m_nLevel);
				
				CStringArray arrayGroups;
				for( int nGroupNo = 1; ( nGroupNo <= MAX_TABLEGROUP ) && ( nGroupNo < DataManagerNonDb.TableGroups.GetSize() ); nGroupNo++ )
				{
					if ( ( nGroupMask & nSetMask ) != 0 )
					{
						CTableGroupCSVRecord Group;
						DataManagerNonDb.TableGroups.GetAt( nGroupNo - 1, Group );
						arrayGroups.Add( Group.GetPrintReportText( nGroupNo, m_nLevel) );
					}

					nGroupMask *= 2;
				}

				if ( arrayGroups.GetSize() > 0 )
				{
					csvOut.AppendString( arrayGroups.GetAt(0) );
				}

				ReportFile.WriteLine ( csvOut.GetLine() );

				for ( int n = 1; n < arrayGroups.GetSize(); n++ )
				{
					CString strLine = "\t\t\t\t\t";
					strLine += arrayGroups.GetAt(n);
					ReportFile.WriteLine( strLine );
				}

				ReportFile.WriteLine ( "<LI>" );
			}
		}
	
		ReportFile.Close();

		CReportManager ReportManager;
		CFilenameUpdater FnUp ( SysFiles::TableGroupSetPrm );
		ReportManager.DisplayReport ( Super.ReportFilename(), FnUp.GetFilenameToUse(), "", this );
	}
}
		
/**********************************************************************/

