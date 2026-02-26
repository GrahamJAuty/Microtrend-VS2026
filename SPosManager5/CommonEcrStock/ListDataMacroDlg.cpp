/**********************************************************************/
#include "CCSVReportLine.h"
#include "DefEditMacroLocalDlg.h"
#include "DefEditMacroSystemDlg.h"
#include "LocationCSVArray.h"
#include "NewRecordNumberDlg.h"
#include "Password.h"
#include "RepSpawn.h"
/**********************************************************************/
#include "ListDataMacroDlg.h"
/**********************************************************************/

CSingleListContentHelpersMacro::CSingleListContentHelpersMacro( CLocationSelectorEntity& LocSelEntity ) : m_LocSelEntity( LocSelEntity )
{
	switch( LocSelEntity.GetConType() )
	{
	case NODE_DBKEYBOARDSET:	
		m_pMacroArray = &DataManagerNonDb.CCMacroKeyboard;	
		m_strLocalType = "Keyboard";
		break;

	case NODE_LOCATION:	
		m_pMacroArray = &DataManagerNonDb.CCMacroLocation;
		m_strLocalType = "Location";
		break;

	case NODE_DATABASE:	
		m_pMacroArray = &DataManagerNonDb.CCMacroDatabase;	
		m_strLocalType = "Database";
		break;

	case NODE_SYSTEM:
	default:			
		m_pMacroArray = &DataManagerNonDb.CCMacroSystem;	
		m_strLocalType = "";
		break;
	}
}

/**********************************************************************/

int CSingleListContentHelpersMacro::GetArraySize()
{
	return m_pMacroArray -> GetSize();
}

/**********************************************************************/

void CSingleListContentHelpersMacro::GetMacroInfo( CCascadingMacroCSVRecord& Macro, CString& strSource, CString& strName, CString& strData )
{
	strSource = "";

	switch( m_LocSelEntity.GetConType() )
	{
	case NODE_DATABASE:
	case NODE_LOCATION:
	case NODE_DBKEYBOARDSET:
		{
			if ( Macro.GetLocalMacroFlag() == FALSE )
			{
				switch( Macro.GetParentMacroTypeExternal() )
				{
				case NODE_SYSTEM:
					strSource = "System";
					break;

				case NODE_DATABASE:
					strSource = "Database";
					break;
				
				case NODE_LOCATION:
					strSource = "Location";
					break;
				}
			}

			if ( strSource != "" )
			{
				strName = Macro.GetParentMacroName();
				strData = Macro.GetParentMacroData();
			}
			else
			{
				strSource = m_strLocalType;
				strName = Macro.GetLocalMacroName();
				strData = Macro.GetLocalMacroData();
				
			}
		}
		break;

	case NODE_SYSTEM:
	default:
		strSource = "System";
		strName = Macro.GetLocalMacroName();
		strData = Macro.GetLocalMacroData();
		break;
	}	
}

/**********************************************************************/

bool CSingleListContentHelpersMacro::PrepareLine( int nIndex, CString& strLine )
{
	strLine = "";

	bool bResult = FALSE;
	if ( ( nIndex >= 0 ) && ( nIndex < GetArraySize() ) )
	{
		CCascadingMacroCSVRecord Macro;
		m_pMacroArray -> GetAt ( nIndex, Macro );

		CCSV csvLine( '\t' );
		csvLine.Add( Macro.GetMacroNo() );

		CString strSource, strName, strData;
		GetMacroInfo( Macro, strSource, strName, strData );

		if ( m_LocSelEntity.GetConType() != NODE_SYSTEM )
		{
			csvLine.Add( strSource );
		}

		csvLine.Add( strName );
		csvLine.Add( strData );
		strLine = csvLine.GetLine();
		
		bResult = TRUE;
	}
	
	return bResult;
}

/**********************************************************************/

void CSingleListContentHelpersMacro::DeleteDatabaseEntry( int nIndex )
{
	if ( ( nIndex >= 0 ) && ( nIndex < m_pMacroArray -> GetSize() ) )
	{
		if ( m_LocSelEntity.GetConType() == NODE_SYSTEM )
		{
			m_pMacroArray -> RemoveAt( nIndex );
			m_bDeletionEffect = TRUE;
		}
		else
		{
			CCascadingMacroCSVRecord Macro;
			m_pMacroArray -> GetAt ( nIndex, Macro );

			switch( Macro.GetParentMacroTypeExternal() )
			{
			case NODE_SYSTEM:
			case NODE_DATABASE:
			case NODE_LOCATION:
				if ( Macro.GetLocalMacroFlag() == TRUE )
				{
					Macro.SetLocalMacroFlag( FALSE );
					Macro.SetLocalMacroName( "" );
					Macro.SetLocalMacroData( "" );
					m_pMacroArray -> SetAt( nIndex, Macro );
					m_bDeletionEffect = TRUE;
				}
				break;

			case NODE_PLACEHOLDER:
			default:
				m_pMacroArray -> RemoveAt( nIndex );
				m_bDeletionEffect = TRUE;
				break;
			}	
		}
	}
}

/**********************************************************************/

void CSingleListContentHelpersMacro::SetColumns( CSSListCtrlVirtual& list, CWnd* pParent )
{
	list.SubclassDlgItem ( IDC_LIST, pParent );
	list.InsertColumn ( 0, "No", LVCFMT_LEFT, 50 );

	if ( m_LocSelEntity.GetConType() == NODE_SYSTEM )
	{
		list.InsertColumn ( 1, "Name", LVCFMT_LEFT, 215 );
		list.InsertColumn ( 2, "Data", LVCFMT_LEFT, 215 );
	}
	else
	{
		list.InsertColumn ( 1, "Source", LVCFMT_LEFT, 80 );
		list.InsertColumn ( 2, "Name", LVCFMT_LEFT, 175 );
		list.InsertColumn ( 3, "Data", LVCFMT_LEFT, 175 );
	}
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CListDataMacroDlg::CListDataMacroDlg( CLocationSelectorEntity& LocSelEntity, CWnd* pParent) : CListDataDlg ( pParent ), m_LocSelEntity( LocSelEntity )
{
	m_pMacroHelpDlg = NULL;

	switch( LocSelEntity.GetConType() )
	{
	case NODE_DBKEYBOARDSET:	
		m_pMacroArray = &DataManagerNonDb.CCMacroKeyboard;
		break;

	case NODE_LOCATION:	
		m_pMacroArray = &DataManagerNonDb.CCMacroLocation;
		break;

	case NODE_DATABASE:	
		m_pMacroArray = &DataManagerNonDb.CCMacroDatabase;	
		break;

	case NODE_SYSTEM:
	default:			
		m_pMacroArray = &DataManagerNonDb.CCMacroSystem;	
		break;
	}

	m_pContentHelpers = new CSingleListContentHelpersMacro( LocSelEntity );
	m_pDisplayHelpers = new CSingleListDisplayHelpers( m_listData, *m_pContentHelpers );

	m_pDisplayHelpers -> SetTopSingleText( "Macro" );
	m_pDisplayHelpers -> SetTopPluralText( "Macros" );
	m_pDisplayHelpers -> SetBottomSingleText( "Macro" );
	m_pDisplayHelpers -> SetBottomPluralText( "Macros" );
}

/**********************************************************************/

CListDataMacroDlg::~CListDataMacroDlg()
{
	delete m_pDisplayHelpers;
	delete m_pContentHelpers;

	if (m_pMacroHelpDlg != NULL)
	{
		delete (m_pMacroHelpDlg);
	}
}

/**********************************************************************/

void CListDataMacroDlg::SetTitle()
{
	switch( m_LocSelEntity.GetConType() )
	{
	case NODE_DATABASE:
		{
			CString strTitle;
			strTitle.Format( "Setup Database Macros (%s)", dbDatabase.GetName( m_LocSelEntity.GetDbIdx() ) );
			SetWindowText( strTitle );
		}
		break;

	case NODE_LOCATION:
		{
			CString strTitle;
			strTitle.Format( "Setup Location Macros (%s)", dbLocation.GetName( m_LocSelEntity.GetEntityIdx() ) );
			SetWindowText( strTitle );
		}
		break;

	case NODE_DBKEYBOARDSET:
		SetWindowText( "Setup Keyboard Macros" );
		break;
	
	case NODE_SYSTEM:
	default:
		SetWindowText ( "Setup System Macros" );
		break;
	}
}

/**********************************************************************/

void CListDataMacroDlg::SetButtonTexts()
{
	m_button1.SetWindowText("&Add");
	m_button2.SetWindowText("&Edit");
	m_button3.SetWindowText("&Delete");
	m_button4.SetWindowText("&Help");
	m_button5.SetWindowText("&Print");
	m_button6.ShowWindow(SW_HIDE);

	MoveControl(&m_button1, 368, 7);
	MoveControl(&m_button2, 368, 47);
	MoveControl(&m_button3, 368, 87);
	MoveControl(&m_button4, 368, 137);
	MoveControl(&m_button5, 368, 187);
}

/**********************************************************************/

void CListDataMacroDlg::SetColumns()
{
	m_pContentHelpers -> SetColumns( m_listData, this );
}

/**********************************************************************/

void CListDataMacroDlg::HandleButton1()
{
	int nMacroNo = m_pMacroArray -> FindFirstFreeNumber();

	if ( nMacroNo == 0 )
	{
		Prompter.Error ( "All macro numbers are already in use." );
		return;
	}

	CNewRecordNumberDlg dlg ( NODE_CCMACRO_SYSTEM, this );
	dlg.m_nNumber = nMacroNo;
	if ( dlg.DoModal() != IDOK )
		return;

	int nMacroIdx = 0;
	nMacroNo = dlg.m_nNumber;
	if ( m_pMacroArray -> FindMacroByNumber ( nMacroNo, nMacroIdx ) )
	{
		Prompter.Error ( "The selected macro is already in use." );
		return;
	}

	CCascadingMacroCSVRecord Macro;
	Macro.SetMacroNo( nMacroNo );
	Macro.SetParentMacroTypeExternal( NODE_PLACEHOLDER );
	Macro.SetParentMacroName( "" );
	Macro.SetParentMacroData( "" );

	CString strName;
	strName.Format( "Macro %d", nMacroNo );

	Macro.SetLocalMacroFlag( TRUE );
	Macro.SetLocalMacroName( strName );
	Macro.SetLocalMacroData( "" );

	m_pMacroArray -> InsertAt ( nMacroIdx, Macro );
	m_listData.SetItemCountEx ( GetArraySize() );
	SelectLine ( nMacroIdx );

	if ( m_LocSelEntity.GetConType() == NODE_SYSTEM )
	{
		CDefEditMacroSystemDlg dlgEdit( nMacroIdx, m_WarnCancelAfterChange, this );
		dlgEdit.SetNoHelpButtonFlag(m_pMacroHelpDlg != NULL);
		dlgEdit.SetListDataDlg( this );
		dlgEdit.DoModal();
		SelectLine ( dlgEdit.GetIndex() );
	}
	else
	{
		CDefEditMacroLocalDlg dlgEdit( nMacroIdx, m_LocSelEntity.GetConType(), TRUE, m_WarnCancelAfterChange, this );
		dlgEdit.SetNoHelpButtonFlag(m_pMacroHelpDlg != NULL);
		dlgEdit.SetListDataDlg( this );
		dlgEdit.DoModal();
		SelectLine ( dlgEdit.GetIndex() );
	}

	m_WarnCancelAfterChange.SetFlag();
}

/**********************************************************************/

void CListDataMacroDlg::HandleButton2() 
{
	int nMacroIdx = GetSafeSelection();
	if ( nMacroIdx == -1 )
		return;
	
	if ( m_LocSelEntity.GetConType() == NODE_SYSTEM )
	{
		CDefEditMacroSystemDlg dlgEdit( nMacroIdx, m_WarnCancelAfterChange, this );
		dlgEdit.SetNoHelpButtonFlag(m_pMacroHelpDlg != NULL);
		dlgEdit.SetListDataDlg( this );
		dlgEdit.DoModal();
		SelectLine ( dlgEdit.GetIndex() );
	}
	else
	{
		CDefEditMacroLocalDlg dlgEdit( nMacroIdx, m_LocSelEntity.GetConType(), TRUE, m_WarnCancelAfterChange, this );
		dlgEdit.SetNoHelpButtonFlag(m_pMacroHelpDlg != NULL);
		dlgEdit.SetListDataDlg( this );
		dlgEdit.DoModal();
		SelectLine ( dlgEdit.GetIndex() );
	}
}

/**********************************************************************/

void CListDataMacroDlg::HandleButton3()
{
	m_pDisplayHelpers -> HandleDelete( this );
	m_WarnCancelAfterChange.SetFlag( m_pContentHelpers -> GetDeletionEffectFlag() );
}

/**********************************************************************/

void CListDataMacroDlg::HandleButton4()
{
	if (m_pMacroHelpDlg == NULL)
	{
		m_pMacroHelpDlg = new CMacroHelpDlg(&m_pMacroHelpDlg, this);
		m_pMacroHelpDlg->Create(IDD_MACRO_HELP, this);
	}

	m_pMacroHelpDlg->SetFocus();
}

/**********************************************************************/

void CListDataMacroDlg::HandleButton5() 
{
	bool bSystem = ( m_LocSelEntity.GetConType() == NODE_SYSTEM );

	if ( m_pMacroArray -> GetSize() == 0 )
	{
		Prompter.Error ( "You must create some Macros\nbefore you can print this report" );
		return;
	}
	else
	{
		CReportFile ReportFile;
		if ( ReportFile.Open ( Super.ReportFilename() ) == FALSE )
			return;

		CString strTitle = "";

		switch( m_LocSelEntity.GetConType() )
		{
		case NODE_DATABASE:
			strTitle.Format( "Database Macros (%s)", dbDatabase.GetName( m_LocSelEntity.GetDbIdx() ) );
			break;

		case NODE_LOCATION:
			strTitle.Format( "Location Macros (%s)", dbLocation.GetName( m_LocSelEntity.GetEntityIdx() ) );
			break;

		case NODE_DBKEYBOARDSET:
			strTitle = "Keyboard Macros";
			break;

		case NODE_SYSTEM:
		default:
			strTitle = "System Macros";
			break;
		}
		
		ReportFile.SetStyle1 ( strTitle );
		
		ReportFile.AddColumn ( "No", TA_LEFT, 100 );

		if ( FALSE == bSystem )
		{
			ReportFile.AddColumn ( "Source", TA_LEFT, 300 );
		}

		ReportFile.AddColumn ( "Name", TA_LEFT, 500 );
		ReportFile.AddColumn ( "Data", TA_LEFT, 800 );
		
		for ( int nMacroIdx = 0; nMacroIdx < m_pMacroArray -> GetSize(); nMacroIdx++ )
		{
			CCascadingMacroCSVRecord Macro;
			m_pMacroArray -> GetAt ( nMacroIdx, Macro );

			CCSVReportLine csvOut;
			csvOut.AppendInt( Macro.GetMacroNo() );

			CString strSource, strName, strData;
			((CSingleListContentHelpersMacro*)m_pContentHelpers) -> GetMacroInfo( Macro, strSource, strName, strData );
			
			if ( FALSE == bSystem )
			{
				csvOut.AppendString( strSource );
			}

			csvOut.AppendString( strName );
			csvOut.AppendString( strData );
			
			ReportFile.WriteLine ( csvOut.GetLine() );
		}
	
		ReportFile.Close();

		CReportManager ReportManager;
		CFilenameUpdater FnUp ( SysFiles::MacroListPrm );
		ReportManager.DisplayReport ( Super.ReportFilename(), FnUp.GetFilenameToUse(), bSystem ? "1" : "0", this );
	}
}
		
/**********************************************************************/
