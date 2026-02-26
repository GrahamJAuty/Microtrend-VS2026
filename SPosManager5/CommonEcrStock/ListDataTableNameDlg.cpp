/**********************************************************************/
#include "CCSVReportLine.h"
#include "CommsBatchDlg.h"
#include "DefEditTableDlg.h"
#include "DefEditTableCopyDlg.h"
#include "DownloadDateDlg.h"
#include "EposTaskDatabase.h"
#include "LocationCSVArray.h"
#include "NewRecordNumberDlg.h"
#include "Password.h"
#include "RepSpawn.h"
/**********************************************************************/
#include "ListDataTableNameDlg.h"
/**********************************************************************/

CSingleListContentHelpersTableName::CSingleListContentHelpersTableName()
{
	m_nGroupLevel = 1;
}

/**********************************************************************/

int CSingleListContentHelpersTableName::GetArraySize()
{
	return DataManagerNonDb.TableNames.GetSize();
}

/**********************************************************************/

void CSingleListContentHelpersTableName::ChangeGroupLevel()
{
	m_nGroupLevel++;
	if ( m_nGroupLevel >= 4 )
	{
		m_nGroupLevel = 1;
	}
}

/**********************************************************************/

bool CSingleListContentHelpersTableName::PrepareLine( int nIndex, CString& strLine )
{
	strLine = "";

	bool bResult = FALSE;
	if ( ( nIndex >= 0 ) && ( nIndex < GetArraySize() ) )
	{
		CTableNameCSVRecord TableNameRecord;
		DataManagerNonDb.TableNames.GetAt ( nIndex, TableNameRecord );
	
		strLine.Format ( "%d\t%s\t%s",
			TableNameRecord.GetTableNo(),
			TableNameRecord.GetTableName(),
			GetGroupText( TableNameRecord, m_nGroupLevel ) );
		
		bResult = TRUE;
	}
	
	return bResult;
}

/**********************************************************************/

const char* CSingleListContentHelpersTableName::GetGroupText( CTableNameCSVRecord& Table, int nLevel )
{
	int nGroupNo = Table.GetReportGroup(nLevel);

	if ( ( nGroupNo >= 1 )  && ( nGroupNo <= DataManagerNonDb.TableGroups.GetSize() ) )
	{
		CTableGroupCSVRecord Group;
		DataManagerNonDb.TableGroups.GetAt( nGroupNo - 1, Group );
		m_strGroupText = Group.GetPrintReportText( nGroupNo, nLevel ); 
	}
	else
	{
		m_strGroupText.Format ( "Group %d", nGroupNo );
	}

	return m_strGroupText;
}

/**********************************************************************/

void CSingleListContentHelpersTableName::DeleteDatabaseEntry( int nIndex )
{
	if ( ( nIndex >= 0 ) && ( nIndex < DataManagerNonDb.TableNames.GetSize() ) )
	{
		DataManagerNonDb.TableNames.RemoveAt( nIndex );
		m_bDeletionEffect = TRUE;
	}
}

/**********************************************************************/

void CSingleListContentHelpersTableName::SetColumns( CSSListCtrlVirtual& list, CWnd* pParent )
{
	list.SubclassDlgItem ( IDC_LIST, pParent );
	list.InsertColumn ( 0, "No", LVCFMT_LEFT, 50 );
	list.InsertColumn ( 1, "Name", LVCFMT_LEFT, 280 );
	list.InsertColumn ( 2, "Group", LVCFMT_LEFT, 150 );
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CListDataTableNameDlg::CListDataTableNameDlg( int nLocIdx, CWnd* pParent) : CListDataDlg ( pParent )
{
	m_nLocIdx = nLocIdx;

	m_pContentHelpers = new CSingleListContentHelpersTableName();
	m_pDisplayHelpers = new CSingleListDisplayHelpers( m_listData, *m_pContentHelpers );

	m_pDisplayHelpers -> SetTopSingleText( "Table Name" );
	m_pDisplayHelpers -> SetTopPluralText( "Table Names" );
	m_pDisplayHelpers -> SetBottomSingleText( "Table Name" );
	m_pDisplayHelpers -> SetBottomPluralText( "Table Names" );
}

/**********************************************************************/

CListDataTableNameDlg::~CListDataTableNameDlg()
{
	delete m_pDisplayHelpers;
	delete m_pContentHelpers;
}

/**********************************************************************/

void CListDataTableNameDlg::SetTitle()
{
	CString strTitle;
	strTitle.Format( "Setup Table Names (%s)", dbLocation.GetName(m_nLocIdx) );
	SetWindowText ( strTitle );
}

/**********************************************************************/

void CListDataTableNameDlg::SetButtonTexts()
{
	m_button1.SetWindowText("&Add");
	m_button2.SetWindowText("&Edit");
	m_button3.SetWindowText("&Copy");
	m_button4.SetWindowText("&Delete");
	m_button5.SetWindowText("&Print");
	m_button6.SetWindowText("Do&wnload");

	if (PasswordArray.GetEnable(PasswordTicks::CommsDownloads) == FALSE)
	{
		m_button6.EnableWindow(FALSE);
		m_button6.ShowWindow(SW_HIDE);
	}

	MoveControl(&m_button1, 368, 7);
	MoveControl(&m_button2, 368, 42);
	MoveControl(&m_button3, 368, 77);
	MoveControl(&m_button4, 368, 112);
	MoveControl(&m_button5, 368, 162);
	MoveControl(&m_button6, 368, 197);
}

/**********************************************************************/

void CListDataTableNameDlg::SetColumns()
{
	m_pContentHelpers -> SetColumns( m_listData, this );
}

/**********************************************************************/

void CListDataTableNameDlg::HandleButton1()
{
	int nTableNo = DataManagerNonDb.TableNames.FindFirstFreeNumber();

	if ( nTableNo == 0 )
	{
		Prompter.Error ( "All table numbers are already in use." );
		return;
	}

	CNewRecordNumberDlg dlg ( NODE_TABLE_NAME, this );
	dlg.m_nNumber = nTableNo;
	if ( dlg.DoModal() != IDOK )
		return;

	int nTableIdx = 0;
	nTableNo = dlg.m_nNumber;
	if ( DataManagerNonDb.TableNames.FindTableByNumber ( nTableNo, nTableIdx ) )
	{
		Prompter.Error ( "The selected table number is already in use." );
		return;
	}

	CTableNameCSVRecord Table;
	Table.SetTableNo( nTableNo );

	CString strName;
	strName.Format( "Table %d", nTableNo );
	Table.SetTableName( strName );

	DataManagerNonDb.TableNames.InsertAt ( nTableIdx, Table );
	m_listData.SetItemCountEx ( GetArraySize() );
	SelectLine ( nTableIdx );

	CDefEditTableDlg dlgEdit( nTableIdx, m_WarnCancelAfterChange, this );
	dlgEdit.SetListDataDlg( this );
	dlgEdit.DoModal();

	SelectLine ( dlgEdit.GetIndex() );

	m_WarnCancelAfterChange.SetFlag();
}

/**********************************************************************/

void CListDataTableNameDlg::HandleButton2() 
{
	int nTableIdx = GetSafeSelection();
	if ( nTableIdx == -1 )
		return;
	
	CDefEditTableDlg dlgEdit( nTableIdx, m_WarnCancelAfterChange, this );
	dlgEdit.SetListDataDlg( this );
	dlgEdit.DoModal();

	SelectLine ( dlgEdit.GetIndex() );
}

/**********************************************************************/

void CListDataTableNameDlg::HandleButton3()
{
	int nTableIdx = GetSafeSelection();
	if (nTableIdx == -1)
	{
		return;
	}

	int nTableNo = DataManagerNonDb.TableNames.GetTableNo(nTableIdx);

	CDefEditTableCopyDlg dlgCopy(nTableIdx, this);
	if (dlgCopy.DoModal() == IDOK)
	{
		if ((dlgCopy.m_nTablesCreated != 0) || (dlgCopy.m_nTablesModified != 0))
		{
			CString strLine1 = "";
			if (dlgCopy.m_nTablesCreated != 0)
			{
				strLine1.Format("New tables created : %d", dlgCopy.m_nTablesCreated);
			}

			CString strLine2 = "";
			if (dlgCopy.m_nTablesModified != 0)
			{
				strLine2.Format("Existing tables modified : %d", dlgCopy.m_nTablesModified);
			}

			CString strInfo = "Copy Table Results\n\n";

			if (strLine1 != "")
			{
				strInfo += strLine1;
			}

			if (strLine2 != "")
			{
				if (strLine1 != "")
				{
					strInfo += "\n\n";
				}
				strInfo += strLine2;
			}

			m_WarnCancelAfterChange.SetFlag();
			DataManagerNonDb.TableNames.FindTableByNumber(nTableNo, nTableIdx);

			m_listData.SetItemCountEx(GetArraySize());
			SelectLine(nTableIdx);

			Prompter.Info(strInfo);
		}
	}
}

/**********************************************************************/

void CListDataTableNameDlg::HandleButton4()
{
	m_pDisplayHelpers -> HandleDelete( this );
	m_WarnCancelAfterChange.SetFlag( m_pContentHelpers -> GetDeletionEffectFlag() );
}

/**********************************************************************/

void CListDataTableNameDlg::HandleButton5() 
{
	if ( DataManagerNonDb.TableNames.GetSize() == 0 )
	{
		Prompter.Error ( "You must create some Table Names\nbefore you can print this report" );
		return;
	}
	else
	{
		CReportFile ReportFile;
		if ( ReportFile.Open ( Super.ReportFilename() ) == FALSE )
			return;

		{
			CString strTitle;
			strTitle.Format( "Table Names (%s)",
				dbLocation.GetName( m_nLocIdx ) );

			ReportFile.SetStyle1 ( strTitle );
		}
		
		ReportFile.AddColumn ( "No", TA_LEFT, 100 );
		ReportFile.AddColumn ( "Name", TA_LEFT, 500 );
		ReportFile.AddColumn ( "Account", TA_LEFT, 200 );
		ReportFile.AddColumn ( "Enable Limit", TA_LEFT, 250 );
		ReportFile.AddColumn ( "Limit(£)", TA_LEFT, 200 );
		ReportFile.AddColumn ( "AutoEnable", TA_LEFT, 250 );
		ReportFile.AddColumn ( "UnusedAfterPay", TA_LEFT, 350 );
		ReportFile.AddColumn ( "Report Filter", TA_LEFT, 250 );
		ReportFile.AddColumn ( "Report Group 1", TA_LEFT, 500 );
		ReportFile.AddColumn ( "Report Group 2", TA_LEFT, 500 );
		ReportFile.AddColumn ( "Report Group 3", TA_LEFT, 500 );
		
		for ( int nTableIdx = 0; nTableIdx < DataManagerNonDb.TableNames.GetSize(); nTableIdx++ )
		{
			CTableNameCSVRecord Table;
			DataManagerNonDb.TableNames.GetAt ( nTableIdx, Table );

			CCSVReportLine csvOut;
			csvOut.AppendInt( Table.GetTableNo() );
			csvOut.AppendString( Table.GetTableName() );
			csvOut.AppendYesOrBlank( Table.GetAccountTableFlag() );
			csvOut.AppendYesOrBlank( Table.GetEnableLimitFlag() );
			csvOut.AppendInt( Table.GetLimitPounds() );
			csvOut.AppendYesOrBlank( Table.GetAutoStatusEnableFlag() );
			csvOut.AppendYesOrBlank( Table.GetAutoStatusUnusedAfterPaymentFlag() );
			csvOut.AppendYesOrBlank( Table.GetShowAsFilterFlag() );
			csvOut.AppendString( ( (CSingleListContentHelpersTableName*) m_pContentHelpers) -> GetGroupText( Table, 1 ) );
			csvOut.AppendString( ( (CSingleListContentHelpersTableName*) m_pContentHelpers) -> GetGroupText( Table, 2 ) );
			csvOut.AppendString( ( (CSingleListContentHelpersTableName*) m_pContentHelpers) -> GetGroupText( Table, 3 ) );
			ReportFile.WriteLine ( csvOut.GetLine() );
		}
	
		ReportFile.Close();

		CReportManager ReportManager;
		CFilenameUpdater FnUp ( SysFiles::TableNamePrm );
		ReportManager.DisplayReport ( Super.ReportFilename(), FnUp.GetFilenameToUse(), "", this );
	}
}
		
/**********************************************************************/

void CListDataTableNameDlg::HandleButton6() 
{
	CArray<int,int> arrayLocIdx;
	arrayLocIdx.Add( m_nLocIdx );
	
	CEposTaskDatabase TaskDatabase;
	TaskDatabase.CreateSimpleTask( DN_TABLE_NAME, arrayLocIdx );

	CString strDate = "";
	{
		CEposTaskHeader TaskHeader;
		TaskDatabase.GetEditableTaskHeader( TaskHeader );

		CString strTitle;
		TaskHeader.SetName( "Download Table Names" );
		strTitle.Format ( "Epos Import Date (%s)", TaskHeader.GetName() );

		CDownloadDateDlg dlgDate( strTitle, this );
		if ( dlgDate.RunIfNeeded() == FALSE )
			return;

		if ( dlgDate.m_bImmediate == TRUE )
			TaskHeader.SetImportType( IMPORT_IMMEDIATE );
		else
		{
			TaskHeader.SetImportType( IMPORT_SPECIFIED );
			strDate = dlgDate.m_strDate;
		}

		TaskDatabase.SetEditableTaskHeader( TaskHeader );
	}

	CKeyboardMap KeyboardMap;
	CCommsBatchDlg dlgComms ( TaskDatabase, strDate, TRUE, KeyboardMap, -1, 0, 0, this );
	dlgComms.DoModal();
	m_bWarnCancelAfterDownload = TRUE;
}

/**********************************************************************/

void CListDataTableNameDlg::HandleClickHeader( int nColumn )
{
	if ( 2 == nColumn )
	{
		((CSingleListContentHelpersTableName*) m_pContentHelpers) -> ChangeGroupLevel();
		m_listData.Invalidate();
	}
}

/**********************************************************************/
