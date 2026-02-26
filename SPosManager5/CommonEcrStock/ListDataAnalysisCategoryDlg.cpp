/**********************************************************************/
#include "CommsBatchDlg.h"
#include "DefEditAnalysisCategoryDlg.h"
#include "DownloadDateDlg.h"
#include "NetworkCSVArray.h"
#include "Password.h"
#include "SelectMultipleLocationDlg.h"
/**********************************************************************/
#include "ListDataAnalysisCategoryDlg.h"
/**********************************************************************/

CSingleListContentHelpersAnalysisCategory::CSingleListContentHelpersAnalysisCategory()
{
}

/**********************************************************************/

int CSingleListContentHelpersAnalysisCategory::GetArraySize()
{
	return DataManager.AnalysisCategory.GetSize();
}

/**********************************************************************/

bool CSingleListContentHelpersAnalysisCategory::PrepareLine( int nIndex, CString& strLine )
{
	strLine = "";

	bool bResult = FALSE;
	if ( ( nIndex >= 0 ) && ( nIndex < GetArraySize() ) )
	{
		CAnalysisCategoryCSVRecord Record;
		DataManager.AnalysisCategory.GetDisplayCategory( nIndex + 1, Record );

		CString strType;
		switch( Record.GetType() )
		{
		case 1:	strType = "Food";		break;
		case 2:	strType = "Drinks";		break;
		case 3:	strType = "Other";		break;
		default:	strType = "Unknown";	break;
		}

		strLine.Format ( "%d\t%s\t%s",
			nIndex + 1,
			Record.GetEposText(),
			(const char*) strType );

		bResult = TRUE;
	}
	
	return bResult;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CListDataAnalysisCategoryDlg::CListDataAnalysisCategoryDlg( CWnd* pParent) : CListDataDlg ( pParent )
{
	m_bMultiSelect = FALSE;
	m_pContentHelpers = new CSingleListContentHelpersAnalysisCategory();
	m_pDisplayHelpers = new CSingleListDisplayHelpers( m_listData, *m_pContentHelpers );
}

/**********************************************************************/

CListDataAnalysisCategoryDlg::~CListDataAnalysisCategoryDlg()
{
	delete m_pDisplayHelpers;
	delete m_pContentHelpers;
}

/**********************************************************************/

void CListDataAnalysisCategoryDlg::SetTitle()
{
	CString strTitle = "Setup Analysis Categories ";
	strTitle += dbDatabase.GetDatabaseTitle( DataManager.GetActiveDbIdx(), EcrmanOptions.GetGlobalAnalysisCategoryFlag() ); 
	SetWindowText ( strTitle );
}

/**********************************************************************/

void CListDataAnalysisCategoryDlg::SetButtonTexts()
{
	m_button1.SetWindowText ( "&Edit" );
	m_button2.ShowWindow ( SW_HIDE );
	m_button3.ShowWindow ( SW_HIDE );
	m_button4.SetWindowText ( "Do&wnload" );
	m_button5.ShowWindow ( SW_HIDE );
	m_button6.ShowWindow ( SW_HIDE );

	if ( PasswordArray.GetEnable( PasswordTicks::CommsDownloads ) == FALSE )
	{
		m_button4.EnableWindow( FALSE );
		m_button4.ShowWindow( SW_HIDE );
	}

	MoveControl( &m_button1, 368, 7 );
	MoveControl( &m_button4, 368, 107 );
}

/**********************************************************************/

void CListDataAnalysisCategoryDlg::SetColumns()
{
	m_listData.SubclassDlgItem ( IDC_LIST, this );
	m_listData.ModifyStyle( 0, LVS_SINGLESEL, 0 );
	m_listData.InsertColumn ( 0, "No", LVCFMT_LEFT, 40 );
	m_listData.InsertColumn ( 1, "Text", LVCFMT_LEFT, 360 );
	m_listData.InsertColumn ( 2, "Type", LVCFMT_LEFT, 80 );
}

/**********************************************************************/

void CListDataAnalysisCategoryDlg::HandleButton1() 
{
	int nCatNo = GetSafeSelection() + 1;
	if ( nCatNo == 0 )
		return;
	
	CDefEditAnalysisCategoryDlg dlg ( nCatNo, m_WarnCancelAfterChange, this );
	dlg.SetListDataDlg( this );
	dlg.DoModal();

	SelectLine ( dlg.GetCatNo() - 1 );
}

/**********************************************************************/

void CListDataAnalysisCategoryDlg::HandleButton4() 
{
	int nDbIdx;
	if ( EcrmanOptions.GetGlobalAnalysisCategoryFlag() )
		nDbIdx = -1;
	else
		nDbIdx = DataManager.GetActiveDbIdx();

	bool bBlocked = FALSE;
	CArray<int,int> arrayLocIdx;
	dbNetwork.GetDatabaseCommsLocArray( nDbIdx, arrayLocIdx, bBlocked );

	if ( arrayLocIdx.GetSize() == 0 )
	{
		if ( FALSE == bBlocked )
			Prompter.Error ( "There are no locations available for\ndownload on this database." );
		else
			Prompter.Error ( "You are not authorised to download files\nfrom this database." );

		return;
	}

	HandleDownload( arrayLocIdx );
}

/**********************************************************************/

void CListDataAnalysisCategoryDlg::HandleDownload( CArray<int,int>& arrayLocIdx )
{
	if ( CSelectMultipleLocationDlg::FilterLocListForDownload( arrayLocIdx, "Download Analysis Categories", this ) == TRUE )
	{
		CEposTaskDatabase TaskDatabase;
		TaskDatabase.CreateSimpleTask( DN_ACAT, arrayLocIdx );
		CString strDate = "";

		{
			CEposTaskHeader TaskHeader;
			TaskDatabase.GetEditableTaskHeader( TaskHeader );

			CString strDate = "";
			TaskHeader.SetName( "Download Analysis Categories" );

			CString strTitle;
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
}

/**********************************************************************/
