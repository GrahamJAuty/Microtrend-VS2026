/**********************************************************************/
#include "CCSVReportLine.h"
#include "DefEditReasonLocalDlg.h"
#include "DefEditReasonSystemDlg.h"
#include "LocationCSVArray.h"
#include "NewRecordNumberDlg.h"
#include "Password.h"
#include "RepSpawn.h"
/**********************************************************************/
#include "ListDataReasonDlg.h"
/**********************************************************************/

CSingleListContentHelpersReason::CSingleListContentHelpersReason( CLocationSelectorEntity& LocSelEntity ) : m_LocSelEntity( LocSelEntity )
{
	switch( LocSelEntity.GetConType() )
	{
	case NODE_LOCATION:	
		m_pReasonArray = &DataManagerNonDb.CCReasonLocation;
		m_strLocalType = "Location";
		break;

	case NODE_DATABASE:	
		m_pReasonArray = &DataManagerNonDb.CCReasonDatabase;	
		m_strLocalType = "Database";
		break;

	case NODE_SYSTEM:
	default:			
		m_pReasonArray = &DataManagerNonDb.CCReasonSystem;	
		m_strLocalType = "";
		break;
	}
}

/**********************************************************************/

int CSingleListContentHelpersReason::GetArraySize()
{
	return m_pReasonArray -> GetSize();
}

/**********************************************************************/

void CSingleListContentHelpersReason::GetReasonInfo( CCascadingReasonCSVRecord& Reason, CString& strSource, CString& strText, bool& bManualEntry )
{
	strSource = "";

	switch( m_LocSelEntity.GetConType() )
	{
	case NODE_DATABASE:
	case NODE_LOCATION:
	case NODE_DBKEYBOARDSET:
		{
			if ( Reason.GetLocalReasonFlag() == FALSE )
			{
				switch( Reason.GetParentReasonConLevelExternal() )
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
				strText = Reason.GetParentReasonText();
				bManualEntry = Reason.GetParentManualEntryFlag();
			}
			else
			{
				strSource = m_strLocalType;
				strText = Reason.GetLocalReasonText();
				bManualEntry = Reason.GetLocalManualEntryFlag();
				
			}
		}
		break;

	case NODE_SYSTEM:
	default:
		strSource = "System";
		strText = Reason.GetLocalReasonText();
		bManualEntry = Reason.GetLocalManualEntryFlag();
		break;
	}	
}

/**********************************************************************/

bool CSingleListContentHelpersReason::PrepareLine( int nIndex, CString& strLine )
{
	strLine = "";

	bool bResult = FALSE;
	if ( ( nIndex >= 0 ) && ( nIndex < GetArraySize() ) )
	{
		CCascadingReasonCSVRecord Reason;
		m_pReasonArray -> GetAt ( nIndex, Reason );

		CCSV csvLine( '\t' );
		csvLine.Add( Reason.GetReasonNo() );

		CString strSource, strText;
		bool bManualEntry;
		GetReasonInfo( Reason, strSource, strText, bManualEntry );

		if ( m_LocSelEntity.GetConType() != NODE_SYSTEM )
		{
			csvLine.Add( strSource );
		}

		csvLine.Add( strText );
		csvLine.Add( bManualEntry ? "Yes" : "No" );
		strLine = csvLine.GetLine();
		
		bResult = TRUE;
	}
	
	return bResult;
}

/**********************************************************************/

void CSingleListContentHelpersReason::DeleteDatabaseEntry( int nIndex )
{
	if ( ( nIndex >= 0 ) && ( nIndex < m_pReasonArray -> GetSize() ) )
	{
		if ( m_LocSelEntity.GetConType() == NODE_SYSTEM )
		{
			m_pReasonArray -> RemoveAt( nIndex );
			m_bDeletionEffect = TRUE;
		}
		else
		{
			CCascadingReasonCSVRecord Reason;
			m_pReasonArray -> GetAt ( nIndex, Reason );

			switch( Reason.GetParentReasonConLevelExternal() )
			{
			case NODE_SYSTEM:
			case NODE_DATABASE:
			case NODE_LOCATION:
				if ( Reason.GetLocalReasonFlag() == TRUE )
				{
					Reason.SetLocalReasonFlag( FALSE );
					Reason.SetLocalReasonText( "" );
					Reason.SetLocalManualEntryFlag( FALSE );
					m_pReasonArray -> SetAt( nIndex, Reason );
					m_bDeletionEffect = TRUE;
				}
				break;

			case NODE_PLACEHOLDER:
			default:
				m_pReasonArray -> RemoveAt( nIndex );
				m_bDeletionEffect = TRUE;
				break;
			}	
		}
	}
}

/**********************************************************************/

void CSingleListContentHelpersReason::SetColumns( CSSListCtrlVirtual& list, CWnd* pParent )
{
	list.SubclassDlgItem ( IDC_LIST, pParent );
	list.InsertColumn ( 0, "No", LVCFMT_LEFT, 50 );

	if ( m_LocSelEntity.GetConType() == NODE_SYSTEM )
	{
		list.InsertColumn ( 1, "Name", LVCFMT_LEFT, 330 );
		list.InsertColumn ( 2, "Manual Entry", LVCFMT_LEFT, 100 );
	}
	else
	{
		list.InsertColumn ( 1, "Source", LVCFMT_LEFT, 80 );
		list.InsertColumn ( 2, "Name", LVCFMT_LEFT, 250 );
		list.InsertColumn ( 3, "Manual Entry", LVCFMT_LEFT, 100 );
	}
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CListDataReasonDlg::CListDataReasonDlg( CLocationSelectorEntity& LocSelEntity, CWnd* pParent) : CListDataDlg ( pParent ), m_LocSelEntity( LocSelEntity )
{
	switch( LocSelEntity.GetConType() )
	{
	case NODE_LOCATION:	
		m_pReasonArray = &DataManagerNonDb.CCReasonLocation;
		break;

	case NODE_DATABASE:	
		m_pReasonArray = &DataManagerNonDb.CCReasonDatabase;	
		break;

	case NODE_SYSTEM:
	default:			
		m_pReasonArray = &DataManagerNonDb.CCReasonSystem;	
		break;
	}

	m_pContentHelpers = new CSingleListContentHelpersReason( LocSelEntity );
	m_pDisplayHelpers = new CSingleListDisplayHelpers( m_listData, *m_pContentHelpers );

	m_pDisplayHelpers -> SetTopSingleText( "Reason Text" );
	m_pDisplayHelpers -> SetTopPluralText( "Reason Texts" );
	m_pDisplayHelpers -> SetBottomSingleText( "Reason Text" );
	m_pDisplayHelpers -> SetBottomPluralText( "Reason Texts" );
}

/**********************************************************************/

CListDataReasonDlg::~CListDataReasonDlg()
{
	delete m_pDisplayHelpers;
	delete m_pContentHelpers;
}

/**********************************************************************/

void CListDataReasonDlg::SetTitle()
{
	CString strType = CCascadingReasonCSVArray::GetReasonTypeText(m_pReasonArray->GetReasonType());
	
	switch( m_LocSelEntity.GetConType() )
	{
	case NODE_DATABASE:
		{
			CString strTitle;
			strTitle.Format( "Setup Database %s Reason Texts (%s)",
				(const char*) strType,
				dbDatabase.GetName( m_LocSelEntity.GetDbIdx() ) );
			SetWindowText( strTitle );

		}
		break;

	case NODE_LOCATION:
		{
			CString strTitle;
			strTitle.Format( "Setup Location %s Reason Texts (%s)",
				(const char*)strType,
				dbLocation.GetName( m_LocSelEntity.GetEntityIdx() ) );
			
			SetWindowText( strTitle );
		}
		break;

	case NODE_SYSTEM:
	default:
		{
			CString strTitle;
			strTitle.Format("Setup System %s Reason Texts",
				(const char*)strType);
			
			SetWindowText(strTitle);
		}	
		break;
	}
}

/**********************************************************************/

void CListDataReasonDlg::SetButtonTexts()
{
	m_button1.SetWindowText ( "&Add" );
	m_button2.SetWindowText ( "&Edit" );
	m_button3.SetWindowText ( "&Delete" );
	m_button4.SetWindowText ( "&Print" );
	m_button5.ShowWindow ( SW_HIDE );
	m_button6.ShowWindow ( SW_HIDE );

	MoveControl( &m_button1, 368, 7 );
	MoveControl( &m_button2, 368, 47 );
	MoveControl( &m_button3, 368, 87 );
	MoveControl( &m_button4, 368, 147 );
	MoveControl( &m_button5, 368, 187 );
}

/**********************************************************************/

void CListDataReasonDlg::SetColumns()
{
	m_pContentHelpers -> SetColumns( m_listData, this );
}

/**********************************************************************/

void CListDataReasonDlg::HandleButton1()
{
	int nReasonNo = m_pReasonArray -> FindFirstFreeNumber();

	if ( nReasonNo == 0 )
	{
		Prompter.Error ( "All reason numbers are already in use." );
		return;
	}

	CNewRecordNumberDlg dlg ( NODE_CCREASON_SYSTEM, this );
	dlg.m_nNumber = nReasonNo;
	if ( dlg.DoModal() != IDOK )
		return;

	int nReasonIdx = 0;
	nReasonNo = dlg.m_nNumber;
	if ( m_pReasonArray -> FindReasonByNumber ( nReasonNo, nReasonIdx ) )
	{
		Prompter.Error ( "The selected reason text is already in use." );
		return;
	}

	CCascadingReasonCSVRecord Reason;
	Reason.SetReasonNo( nReasonNo );
	Reason.SetParentReasonConLevelExternal( NODE_PLACEHOLDER );
	Reason.SetParentReasonText( "" );
	Reason.SetParentManualEntryFlag(FALSE);
	
	CString strName;
	strName.Format( "Reason %d", nReasonNo );

	Reason.SetLocalReasonFlag( TRUE );
	Reason.SetLocalReasonText( strName );
	Reason.SetLocalManualEntryFlag( FALSE );

	m_pReasonArray -> InsertAt ( nReasonIdx, Reason );
	m_listData.SetItemCountEx ( GetArraySize() );
	SelectLine ( nReasonIdx );

	if ( m_LocSelEntity.GetConType() == NODE_SYSTEM )
	{
		CDefEditReasonSystemDlg dlgEdit( nReasonIdx, m_WarnCancelAfterChange, this );
		dlgEdit.SetListDataDlg( this );
		dlgEdit.DoModal();
		SelectLine ( dlgEdit.GetIndex() );
	}
	else
	{
		CDefEditReasonLocalDlg dlgEdit( nReasonIdx, m_LocSelEntity.GetConType(), TRUE, m_WarnCancelAfterChange, this );
		dlgEdit.SetListDataDlg( this );
		dlgEdit.DoModal();
		SelectLine ( dlgEdit.GetIndex() );
	}

	m_WarnCancelAfterChange.SetFlag();
}

/**********************************************************************/

void CListDataReasonDlg::HandleButton2() 
{
	int nReasonIdx = GetSafeSelection();
	if ( nReasonIdx == -1 )
		return;
	
	if ( m_LocSelEntity.GetConType() == NODE_SYSTEM )
	{
		CDefEditReasonSystemDlg dlgEdit( nReasonIdx, m_WarnCancelAfterChange, this );
		dlgEdit.SetListDataDlg( this );
		dlgEdit.DoModal();
		SelectLine ( dlgEdit.GetIndex() );
	}
	else
	{
		CDefEditReasonLocalDlg dlgEdit( nReasonIdx, m_LocSelEntity.GetConType(), TRUE, m_WarnCancelAfterChange, this );
		dlgEdit.SetListDataDlg( this );
		dlgEdit.DoModal();
		SelectLine ( dlgEdit.GetIndex() );
	}
}

/**********************************************************************/

void CListDataReasonDlg::HandleButton3()
{
	m_pDisplayHelpers -> HandleDelete( this );
	m_WarnCancelAfterChange.SetFlag( m_pContentHelpers -> GetDeletionEffectFlag() );
}

/**********************************************************************/

void CListDataReasonDlg::HandleButton4() 
{
	bool bSystem = ( m_LocSelEntity.GetConType() == NODE_SYSTEM );

	if ( m_pReasonArray -> GetSize() == 0 )
	{
		Prompter.Error ( "You must create some Reason Texts\nbefore you can print this report" );
		return;
	}
	else
	{
		CReportFile ReportFile;
		if ( ReportFile.Open ( Super.ReportFilename() ) == FALSE )
			return;

		CString strTitle = "";

		CString strType = CCascadingReasonCSVArray::GetReasonTypeText(m_pReasonArray->GetReasonType());

		switch( m_LocSelEntity.GetConType() )
		{
		case NODE_DATABASE:
			strTitle.Format( "Database %s Reason Texts (%s)", 
				(const char*) strType,
				dbDatabase.GetName( m_LocSelEntity.GetDbIdx() ) );
			break;

		case NODE_LOCATION:
			strTitle.Format( "Location %s Reason Texts (%s)", 
				(const char*)strType,
				dbLocation.GetName( m_LocSelEntity.GetEntityIdx() ) );
			break;

		case NODE_SYSTEM:
		default:
			strTitle.Format("System %s Reason Texts",
				(const char*)strType);
			break;

			strTitle = "System Reason Texts";
			break;
		}
		
		ReportFile.SetStyle1 ( strTitle );
		
		ReportFile.AddColumn ( "No", TA_LEFT, 100 );

		if ( FALSE == bSystem )
		{
			ReportFile.AddColumn ( "Source", TA_LEFT, 300 );
		}

		ReportFile.AddColumn ( "Manual Entry", TA_LEFT, 300 );
		ReportFile.AddColumn ( "Reason Text", TA_LEFT, 1500 );
		
		for ( int nReasonIdx = 0; nReasonIdx < m_pReasonArray -> GetSize(); nReasonIdx++ )
		{
			CCascadingReasonCSVRecord Reason;
			m_pReasonArray -> GetAt ( nReasonIdx, Reason );

			CCSVReportLine csvOut;
			csvOut.AppendInt( Reason.GetReasonNo() );

			CString strSource = "";
			CString strText = "";
			bool bManualEntry = FALSE;
			((CSingleListContentHelpersReason*)m_pContentHelpers) -> GetReasonInfo( Reason, strSource, strText, bManualEntry );
			
			if ( FALSE == bSystem )
			{
				csvOut.AppendString( strSource );
			}

			csvOut.AppendString( bManualEntry ? "Yes" : "No" );
			csvOut.AppendString( strText );
			
			ReportFile.WriteLine ( csvOut.GetLine() );
		}
	
		ReportFile.Close();

		CReportManager ReportManager;
		CFilenameUpdater FnUp ( SysFiles::MacroListPrm );
		ReportManager.DisplayReport ( Super.ReportFilename(), FnUp.GetFilenameToUse(), bSystem ? "1" : "0", this );
	}
}
		
/**********************************************************************/

