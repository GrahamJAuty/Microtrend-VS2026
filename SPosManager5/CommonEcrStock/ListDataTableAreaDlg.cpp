/**********************************************************************/
#include "CCSVReportLine.h"
#include "CommsBatchDlg.h"
#include "DefEditTableAreaDlg.h"
#include "DownloadDateDlg.h"
#include "EposTaskDatabase.h"
#include "LocationCSVArray.h"
#include "NewRecordNumberDlg.h"
#include "Password.h"
#include "RepSpawn.h"
/**********************************************************************/
#include "ListDataTableAreaDlg.h"
/**********************************************************************/

CSingleListContentHelpersTableArea::CSingleListContentHelpersTableArea()
{
}

/**********************************************************************/

int CSingleListContentHelpersTableArea::GetArraySize()
{
	return DataManagerNonDb.TableAreas.GetSize();
}

/**********************************************************************/

bool CSingleListContentHelpersTableArea::PrepareLine( int nIndex, CString& strLine )
{
	strLine = "";

	bool bResult = FALSE;
	if ( ( nIndex >= 0 ) && ( nIndex < GetArraySize() ) )
	{
		CTableAreaCSVRecord TableAreaRecord;
		DataManagerNonDb.TableAreas.GetAt ( nIndex, TableAreaRecord );
	
		strLine.Format ( "%d\t%s",
			TableAreaRecord.GetAreaNo(),
			TableAreaRecord.GetAreaName() );
		
		bResult = TRUE;
	}
	
	return bResult;
}

/**********************************************************************/

void CSingleListContentHelpersTableArea::DeleteDatabaseEntry( int nIndex )
{
	if ( ( nIndex >= 0 ) && ( nIndex < DataManagerNonDb.TableAreas.GetSize() ) )
	{
		DataManagerNonDb.TableAreas.RemoveAt( nIndex );
		m_bDeletionEffect = TRUE;
	}
}

/**********************************************************************/

void CSingleListContentHelpersTableArea::SetColumns( CSSListCtrlVirtual& list, CWnd* pParent )
{
	list.SubclassDlgItem ( IDC_LIST, pParent );
	list.InsertColumn ( 0, "No", LVCFMT_LEFT, 50 );
	list.InsertColumn ( 1, "Name", LVCFMT_LEFT, 430 );
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CListDataTableAreaDlg::CListDataTableAreaDlg( int nLocIdx, CWnd* pParent) : CListDataDlg ( pParent )
{
	m_nLocIdx = nLocIdx;

	DataManagerNonDb.CCMacroActive.Reset();
	DataManagerNonDb.CCMacroActive.LoadParentMacrosLocation( m_nLocIdx );

	int nDbIdx;
	if ( dbDatabase.FindDatabaseByNumber( dbLocation.GetDbNo ( m_nLocIdx ), nDbIdx ) == TRUE )
	{
		DataManagerNonDb.CCMacroActive.LoadParentMacrosDatabase( nDbIdx );
	}

	DataManagerNonDb.CCMacroActive.LoadParentMacrosSystem();

	m_pContentHelpers = new CSingleListContentHelpersTableArea();
	m_pDisplayHelpers = new CSingleListDisplayHelpers( m_listData, *m_pContentHelpers );

	m_pDisplayHelpers -> SetTopSingleText( "Table Area" );
	m_pDisplayHelpers -> SetTopPluralText( "Table Areas" );
	m_pDisplayHelpers -> SetBottomSingleText( "Table Area" );
	m_pDisplayHelpers -> SetBottomPluralText( "Table Areas" );
}

/**********************************************************************/

CListDataTableAreaDlg::~CListDataTableAreaDlg()
{
	delete m_pDisplayHelpers;
	delete m_pContentHelpers;
}

/**********************************************************************/

void CListDataTableAreaDlg::SetTitle()
{
	CString strTitle;
	strTitle.Format( "Setup Table Areas (%s)", dbLocation.GetName(m_nLocIdx) );
	SetWindowText ( strTitle );
}

/**********************************************************************/

void CListDataTableAreaDlg::SetButtonTexts()
{
	m_button1.SetWindowText ( "&Add" );
	m_button2.SetWindowText ( "&Edit" );
	m_button3.SetWindowText ( "&Delete" );
	m_button4.SetWindowText ( "&Print" );
	m_button5.SetWindowText ( "Do&wnload" );
	m_button6.ShowWindow ( SW_HIDE );

	if ( PasswordArray.GetEnable( PasswordTicks::CommsDownloads ) == FALSE ) 
	{
		m_button5.EnableWindow( FALSE );
		m_button5.ShowWindow( SW_HIDE );
	}

	MoveControl( &m_button1, 368, 7 );
	MoveControl( &m_button2, 368, 47 );
	MoveControl( &m_button3, 368, 87 );
	MoveControl( &m_button4, 368, 147 );
	MoveControl( &m_button5, 368, 187 );
}

/**********************************************************************/

void CListDataTableAreaDlg::SetColumns()
{
	m_pContentHelpers -> SetColumns( m_listData, this );
}

/**********************************************************************/

void CListDataTableAreaDlg::HandleButton1()
{
	int nAreaNo = DataManagerNonDb.TableAreas.FindFirstFreeNumber();

	if ( nAreaNo == 0 )
	{
		Prompter.Error ( "All table area numbers are already in use." );
		return;
	}

	CNewRecordNumberDlg dlg ( NODE_TABLE_AREA, this );
	dlg.m_nNumber = nAreaNo;
	if ( dlg.DoModal() != IDOK )
		return;

	int nAreaIdx = 0;
	nAreaNo = dlg.m_nNumber;
	if ( DataManagerNonDb.TableAreas.FindAreaByNumber ( nAreaNo, nAreaIdx ) )
	{
		Prompter.Error ( "The selected area number is already in use." );
		return;
	}

	CTableAreaCSVRecord Area;
	Area.SetAreaNo( nAreaNo );

	CString strName;
	strName.Format( "Area %d", nAreaNo );
	Area.SetAreaName( strName );

	DataManagerNonDb.TableAreas.InsertAt ( nAreaIdx, Area );
	m_listData.SetItemCountEx ( GetArraySize() );
	SelectLine ( nAreaIdx );

	CDefEditTableAreaDlg dlgEdit( nAreaIdx, m_WarnCancelAfterChange, this );
	dlgEdit.SetListDataDlg( this );
	dlgEdit.DoModal();

	SelectLine ( dlgEdit.GetIndex() );

	m_WarnCancelAfterChange.SetFlag();
}

/**********************************************************************/

void CListDataTableAreaDlg::HandleButton2() 
{
	int nAreaIdx = GetSafeSelection();
	if ( nAreaIdx == -1 )
		return;
	
	CDefEditTableAreaDlg dlgEdit( nAreaIdx, m_WarnCancelAfterChange, this );
	dlgEdit.SetListDataDlg( this );
	dlgEdit.DoModal();

	SelectLine ( dlgEdit.GetIndex() );
}

/**********************************************************************/

void CListDataTableAreaDlg::HandleButton3()
{
	m_pDisplayHelpers -> HandleDelete( this );
	m_WarnCancelAfterChange.SetFlag( m_pContentHelpers -> GetDeletionEffectFlag() );
}

/**********************************************************************/

void CListDataTableAreaDlg::HandleButton4() 
{
	if ( DataManagerNonDb.TableAreas.GetSize() == 0 )
	{
		Prompter.Error ( "You must create some Table Areas\nbefore you can print this report" );
		return;
	}
	else
	{
		CReportFile ReportFile;
		if ( ReportFile.Open ( Super.ReportFilename() ) == FALSE )
			return;

		{
			CString strTitle;
			strTitle.Format( "Table Areas (%s)",
				dbLocation.GetName( m_nLocIdx ) );

			ReportFile.SetStyle1 ( strTitle );
		}
		
		ReportFile.AddColumn ( "No", TA_LEFT, 100 );
		ReportFile.AddColumn ( "Name", TA_LEFT, 500 );
		ReportFile.AddColumn ( "Auto-Tabs Only", TA_LEFT, 320 );
		ReportFile.AddColumn ( "Suppress Covers", TA_LEFT, 320 );
		ReportFile.AddColumn ( "Start Table", TA_LEFT, 300 );
		ReportFile.AddColumn ( "End Table", TA_LEFT, 300 );
		ReportFile.AddColumn ( "KP Enables", TA_LEFT, 400 );
		ReportFile.AddColumn ( "Open Table Macro", TA_LEFT, 500 );
		
		for ( int nAreaIdx = 0; nAreaIdx < DataManagerNonDb.TableAreas.GetSize(); nAreaIdx++ )
		{
			CTableAreaCSVRecord Area;
			DataManagerNonDb.TableAreas.GetAt ( nAreaIdx, Area );

			CCSVReportLine csvOut;
			csvOut.AppendInt( Area.GetAreaNo() );
			csvOut.AppendString( Area.GetAreaName() );
			csvOut.AppendYesOrBlank( Area.GetAutoTabsOnlyFlag() );
			csvOut.AppendYesOrBlank( Area.GetSuppressCoversFlag() );
			csvOut.AppendInt( Area.GetTableNoLo() );
			csvOut.AppendInt( Area.GetTableNoHi() );

			{
				CCSV csvFlags( ',' );
				for ( int n = 1; n <= 16; n++ )
				{
					if ( Area.GetKPFlag(n) == TRUE )
					{
						csvFlags.Add(n);
					}
				}
				csvOut.AppendString( csvFlags.GetLine() );
			}

			{
				CString str;
				int nMacroNo = Area.GetTableOpenMacro();

				if ( 0 == nMacroNo )
				{
					str = "None";
				}
				else
				{
					str.Format( "M%4.4d %s", nMacroNo, DataManagerNonDb.CCMacroActive.GetMacroNameFromNumber( nMacroNo ) );
				}

				csvOut.AppendString( str );
			}
			
			ReportFile.WriteLine ( csvOut.GetLine() );
		}
	
		ReportFile.Close();

		CReportManager ReportManager;
		CFilenameUpdater FnUp ( SysFiles::TableAreaPrm );
		ReportManager.DisplayReport ( Super.ReportFilename(), FnUp.GetFilenameToUse(), "", this );
	}
}
		
/**********************************************************************/

void CListDataTableAreaDlg::HandleButton5() 
{
	CArray<int,int> arrayLocIdx;
	arrayLocIdx.Add( m_nLocIdx );
	
	CEposTaskDatabase TaskDatabase;
	TaskDatabase.CreateSimpleTask( DN_TABLE_AREA, arrayLocIdx );

	CString strDate = "";
	{
		CEposTaskHeader TaskHeader;
		TaskDatabase.GetEditableTaskHeader( TaskHeader );

		CString strTitle;
		TaskHeader.SetName( "Download Table Areas" );
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
