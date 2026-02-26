/**********************************************************************/
#include "CommsBatchDlg.h"
#include "CSVDBImportStatsDlg.h"
#include "CSVDBImportTypeDlg.h"
#include "DatabaseImportHandlerCommon.h"
#include "DefEditDlg.h"
#include "NewRecordNumberDlg.h"
#include "DownloadDateDlg.h"
#include "ImportDefines.h"
#include "LocationCSVArray.h"
#include "NetworkCSVArray.h"
#include "Password.h"
#include "PropertySheetServer.h"
#include "ReportDataTableServerDlg.h"
#include "SelectMultipleLocationDlg.h"
#include "ServerDatabaseHeader.h"
#include "ServerExportDlg.h"
#include "StatusProgress.h"
/**********************************************************************/
#include "ListDataServerDlg.h"
/**********************************************************************/

CSingleListContentHelpersServer::CSingleListContentHelpersServer()
{
}

/**********************************************************************/

int CSingleListContentHelpersServer::GetArraySize()
{
	return DataManagerNonDb.Server.GetSortArraySize();
}

/**********************************************************************/

bool CSingleListContentHelpersServer::PrepareLine( int nIndex, CString& strLine )
{
	strLine = "";

	bool bResult = FALSE;
	if ( ( nIndex >= 0 ) && ( nIndex < DataManagerNonDb.Server.GetSortArraySize() ) )
	{
		int nArrayIdx = DataManagerNonDb.Server.GetArrayIdxFromSortIdx(nIndex);

		CServerCSVRecord ServerRecord;
		DataManagerNonDb.Server.GetAt ( nArrayIdx, ServerRecord );
	
		strLine.Format("%d\t%s",
			ServerRecord.GetServerNo(),
			ServerRecord.GetReportName());
		
		if ( TRUE == DataManagerNonDb.Server.GetShowHomeLocFlag() )
		{
			CString strHomeLoc = ServerRecord.GetHomeLocName();

			if (strHomeLoc != "")
			{
				strLine += "\t";
				strLine += strHomeLoc;
			}
		}
			
		bResult = TRUE;
	}
	
	return bResult;
}

/**********************************************************************/

void CSingleListContentHelpersServer::DeleteDatabaseEntry( int nIndex )
{
	if ( ( nIndex >= 0 ) && ( nIndex < DataManagerNonDb.Server.GetSortArraySize() ) )
	{
		int nArrayIdx = DataManagerNonDb.Server.GetArrayIdxFromSortIdx(nIndex);
		DataManagerNonDb.Server.RemoveAt( nArrayIdx );
		m_bDeletionEffect = TRUE;
	}
}

/**********************************************************************/

void CSingleListContentHelpersServer::SetColumns( CSSListCtrlVirtual& list, CWnd* pParent )
{
	list.SubclassDlgItem ( IDC_LIST, pParent );
	list.InsertColumn ( 0, "No", LVCFMT_LEFT, 60 );

	if (DataManagerNonDb.Server.GetShowHomeLocFlag() == FALSE )
	{
		list.InsertColumn(1, "Name", LVCFMT_LEFT, 400);
		list.InsertColumn(2, "", LVCFMT_LEFT, 20);
	}
	else
	{
		list.InsertColumn(1, "Name", LVCFMT_LEFT, 200);
		list.InsertColumn(2, "", LVCFMT_LEFT, 20);
		list.InsertColumn(3, "Default Location", LVCFMT_LEFT, 200);
	}
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CListDataServerDlg::CListDataServerDlg(int nServerDbIdx, int nServerLocIdx, int nTerminalLocIdx, CWnd* pParent) : CListDataDlg(pParent)
{
	m_nServerDbIdx = nServerDbIdx;
	m_nServerLocIdx = nServerLocIdx;
	m_nTerminalLocIdx = nTerminalLocIdx;

	{
		bool bShowHomeLoc = TRUE;
		bShowHomeLoc &= DealerFlags.GetServerHomeLocationFlag();
		bShowHomeLoc &= PasswordArray.GetEnable(PasswordTicks::EditServerDetails);
		bShowHomeLoc &= (m_nServerLocIdx < 0);
		DataManagerNonDb.Server.SetShowHomeLocFlag(bShowHomeLoc);
	}

	{
		int nDbNo = 0;
		if ((m_nServerDbIdx > 0) && (m_nServerDbIdx < dbDatabase.GetSize()))
		{
			nDbNo = dbDatabase.GetDbNo(m_nServerDbIdx);
		}
		DataManagerNonDb.Server.SetServerDbNo(nDbNo);
	}

	DataManagerNonDb.Server.SetSortMode(SERVER_SORT_NUMBER);

	m_pContentHelpers = new CSingleListContentHelpersServer();
	m_pDisplayHelpers = new CSingleListDisplayHelpers(m_listData, *m_pContentHelpers);


	m_pDisplayHelpers->SetTopSingleText("server");
	m_pDisplayHelpers->SetTopPluralText("servers");
	m_pDisplayHelpers->SetBottomSingleText("server");
	m_pDisplayHelpers->SetBottomPluralText("servers");

	m_bSearchPrevious = FALSE;
	m_strSearchText = "";
	m_strSearchPrompt = "Find Server by Name";
	m_nSearchColumn = 2;
}

/**********************************************************************/

CListDataServerDlg::~CListDataServerDlg()
{
	delete m_pDisplayHelpers;
	delete m_pContentHelpers;
}

/**********************************************************************/

void CListDataServerDlg::SetTitle()
{
	CString strTitle = "Edit Servers";

	CString strExtra = "";

	if (m_nServerLocIdx >= 0)
	{
		strExtra.Format(" (%s)", dbLocation.GetName(m_nServerLocIdx));
	}
	else if (m_nServerDbIdx >= 0)
	{
		strExtra.Format(" (%s)", dbDatabase.GetName(m_nServerDbIdx));
	}
	else
	{
		strExtra = " (Global)";
	}

	if ((EcrmanOptions.GetFeaturesServerRestrictionFlag() == TRUE) && (EcrmanOptions.GetFeaturesServerSettingsType() != SERVER_LOC))
	{
		strExtra += " (Terminals for ";
		strExtra += dbLocation.GetName(m_nTerminalLocIdx);
		strExtra += ")";
	}

	SetWindowText(strTitle + strExtra);
}

/**********************************************************************/

void CListDataServerDlg::SetButtonTexts()
{
	m_button1.SetWindowText ( "&Add" );
	m_button2.SetWindowText ( "&Edit" );
	m_button3.SetWindowText ( "&Delete" );
	m_button4.SetWindowText ( "&Print" );
	m_button5.SetWindowText ( "Do&wnload" );
	m_button6.ShowWindow ( SW_HIDE );

	if ( PasswordArray.GetEnable( PasswordTicks::EditServerDetails ) == FALSE )
	{
		m_button1.ShowWindow( SW_HIDE );
		m_button3.ShowWindow( SW_HIDE );
	}

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

	if (DealerFlags.GetServerImportExportFlag() == TRUE)
	{
		EnableHiddenButton1("X");
		EnableHiddenButton2("I");
	}
}

/**********************************************************************/

void CListDataServerDlg::SetColumns()
{
	m_pContentHelpers -> SetColumns( m_listData, this );
}

/**********************************************************************/

void CListDataServerDlg::HandleButton1()
{
	int nServerNo = DataManagerNonDb.Server.FindFirstFreeNumber();

	if ( nServerNo == 0 )
	{
		Prompter.Error ( "All available servers are already in use." );
		return;
	}

	CNewRecordNumberDlg dlg ( NODE_SERVER, this );
	dlg.m_nNumber = nServerNo;
	if ( dlg.DoModal() != IDOK )
		return;

	int nServerIdx = 0;
	nServerNo = dlg.m_nNumber;
	if ( DataManagerNonDb.Server.FindServerByNumber ( nServerNo, nServerIdx ) )
	{
		Prompter.Error ( "The selected server number is already in use." );
		return;
	}

	CString strTimeStamp;
	::GetSimpleHexTimeStamp( strTimeStamp );

	CServerCSVRecord Server;
	Server.SetServerNo( nServerNo );
	Server.SetTimeStamp( strTimeStamp );
	Server.SetReceiptName ( Server.GetReportName() );
	DataManagerNonDb.Server.InsertAt ( nServerIdx, Server );
	
	DataManagerNonDb.Server.RefreshSort();
	m_listData.SetItemCountEx ( GetArraySize() );
	SelectLine(DataManagerNonDb.Server.GetSortIdxFromArrayIdx(nServerIdx));

	CPropertySheetServer propSheet( IDC_SPIN, TRUE, m_nServerDbIdx, m_nServerLocIdx, m_nTerminalLocIdx, nServerIdx, m_WarnCancelAfterChange, this );
	propSheet.SetListDataDlg( this );
	
	if ( propSheet.DoModal() == IDOK )
	{
		propSheet.SaveRecord();
	}

	DataManagerNonDb.Server.RefreshSort();
	SelectLine( DataManagerNonDb.Server.GetSortIdxFromArrayIdx(propSheet.GetIndex()));
	m_WarnCancelAfterChange.SetFlag();
}

/**********************************************************************/

void CListDataServerDlg::HandleButton2() 
{
	int nIndex = GetSafeSelection();
	if ( nIndex != -1 )
	{
		int nArrayIdx = DataManagerNonDb.Server.GetArrayIdxFromSortIdx(nIndex);

		bool bFullEdit = PasswordArray.GetEnable( PasswordTicks::EditServerDetails );
		
		CPropertySheetServer propSheet( IDC_SPIN, bFullEdit, m_nServerDbIdx, m_nServerLocIdx, m_nTerminalLocIdx, nArrayIdx, m_WarnCancelAfterChange, this );
		propSheet.SetListDataDlg( this );
	
		if ( propSheet.DoModal() == IDOK )
		{
			propSheet.SaveRecord();
		}

		DataManagerNonDb.Server.RefreshSort();
		SelectLine(DataManagerNonDb.Server.GetSortIdxFromArrayIdx(propSheet.GetIndex()));
	}
}

/**********************************************************************/

void CListDataServerDlg::HandleButton3()
{
	int nOldSize = DataManagerNonDb.Server.GetSize();
	
	m_pDisplayHelpers -> HandleDelete( this );
	m_WarnCancelAfterChange.SetFlag( m_pContentHelpers -> GetDeletionEffectFlag() );

	if (nOldSize != DataManagerNonDb.Server.GetSize())
	{
		DataManagerNonDb.Server.RefreshSort();
		m_listData.SetItemCountEx(GetArraySize());
		SelectLine(m_pDisplayHelpers->GetFirstDeletedItem());
	}
}

/**********************************************************************/

void CListDataServerDlg::HandleButton4() 
{
	if ( DataManagerNonDb.Server.GetSize() == 0 )
	{
		Prompter.Error ( "You must create some servers\nbefore you can print this report" );
		return;
	}
	else
	{
		CReportDataTableServerDlg dlg( m_nServerDbIdx, m_nServerLocIdx, m_nTerminalLocIdx, this );
		dlg.DoModal();
	}
}

/**********************************************************************/	

void CListDataServerDlg::HandleButton5() 
{
	bool bBlocked = FALSE;
	CArray<int,int> arrayLocIdx;

	if ( m_nServerLocIdx >= 0 )
	{
		int nNetworkNo = dbLocation.GetNetworkNo( m_nServerLocIdx );

		int nNetworkIdx;
		if ( dbNetwork.FindNetworkByNumber( nNetworkNo, nNetworkIdx ) == TRUE )
		{
			switch( dbNetwork.GetConnectionType( nNetworkIdx ) )
			{
			case CONNECTION_TYPE_STANDARD_REALTIME:
			case CONNECTION_TYPE_STANDARD_BATCH:
				{
					if ( PasswordArray.CheckLocationAccessLocIdx( m_nServerLocIdx ) == TRUE )
						arrayLocIdx.Add( m_nServerLocIdx );
					else
						bBlocked = TRUE;
				}
				break;
			}
		}		
	}
	else if ( m_nServerDbIdx >= 0 )
		dbNetwork.GetDatabaseCommsLocArray( m_nServerDbIdx, arrayLocIdx, bBlocked );
	else 
		dbNetwork.GetDatabaseCommsLocArray( -1, arrayLocIdx, bBlocked );

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

void CListDataServerDlg::HandleButtonHidden1()
{
	if (DataManagerNonDb.Server.GetSize() == 0)
	{
		return;
	}

	int nSel = GetSafeSelection();
	if (nSel == -1)
	{
		return;
	}

	CServerExportDlg dlgFields(m_nServerDbIdx, m_nServerLocIdx, this);
	if (dlgFields.DoModal() == IDCANCEL)
	{
		return;
	}

	bool bGotServer = FALSE;
	
	int nStartIdx = 0;
	DataManagerNonDb.Server.FindServerByNumber(atoi(dlgFields.m_strServerFrom), nStartIdx);
	
	int nEndIdx = 0;
	if (DataManagerNonDb.Server.FindServerByNumber(atoi(dlgFields.m_strServerTo), nEndIdx) == FALSE)
	{
		nEndIdx--;
	}

	if ( nEndIdx < nStartIdx )
	{ 
		Prompter.Error("There are no Server in the selected range");
		return;
	}

	CString strPathName = "";
	{
		CSSFile fileExportFilename;
		if (fileExportFilename.Open(DataManagerNonDb.GetFilePathServerExport(m_nServerDbIdx, m_nServerLocIdx), "rb" ) == TRUE)
		{
			fileExportFilename.ReadString(strPathName);
		}
	}

	if (strPathName == "")
	{
		strPathName = DataManagerNonDb.GetDefaultServerExportPath(m_nServerDbIdx, m_nServerLocIdx);
	}

	CString strDatabaseHeader, strExportHeaderToUse;
	strDatabaseHeader = ServerDatabaseHeader.GetDatabaseHeader();
	strExportHeaderToUse = dlgFields.GetServerHeaderLine();

	CSSExportFilterFile fileExportServer(strDatabaseHeader, strExportHeaderToUse);
	if (fileExportServer.Open(strPathName, "wb") == FALSE)
	{
		CString strMsg;
		strMsg.Format("Unable to create export file %s", (const char*)strPathName);
		Prompter.Error(strMsg);
		return;
	}

	int nServerExported = 0;
	int nCount = (nEndIdx - nStartIdx) + 1;

	//LOOP THROUGH SELECTED RANGE OF PLU RECORDS
	StatusProgress.Lock(TRUE, "Exporting Server records");
	for (int nProgress = 0; nProgress < nCount; nProgress++)
	{
		StatusProgress.SetPos(nProgress, nCount);

		CServerCSVRecord ServerRecord;
		DataManagerNonDb.Server.GetAt(nStartIdx + nProgress, ServerRecord);

		//EXPORT THE SERVER RECORD
		CCSV csvServer(',', '"', FALSE, FALSE, TRUE);
		ServerRecord.ConvertToCSV(csvServer);

		CString strExportLine = csvServer.GetLine();
		
		fileExportServer.WriteData(strExportLine);
		nServerExported++;
	}

	StatusProgress.Unlock();
	fileExportServer.Close();

	//SHOW THE RESULTS
	CString strMsg;
	switch (nServerExported)
	{
	case 0:	
		strMsg = "No Servers were exported.";	
		break;

	case 1:	
		strMsg = "1 Server was exported.";	
		break;

	default:	
		strMsg.Format("%d Servers were exported.", nServerExported);	
		break;
	}

	Prompter.Info(strMsg);
}

/**********************************************************************/

void CListDataServerDlg::HandleButtonHidden2()
{
	CString strImportPath = "";
	{
		CSSFile fileImportPath;
		if (fileImportPath.Open(DataManagerNonDb.GetFilePathServerImport(m_nServerDbIdx, m_nServerLocIdx), "rb") == TRUE)
		{
			fileImportPath.ReadLine(strImportPath);
		}
	}

	CString strTitle = "Import Server CSV File";
	CFileDialog dlgFilename(TRUE, "csv", "plu", OFN_HIDEREADONLY | OFN_NOCHANGEDIR, "CSV Files (*.csv)|*.csv|All Files (*.*)|*.*||", this);
	dlgFilename.m_ofn.lpstrTitle = strTitle;
	dlgFilename.m_ofn.lpstrInitialDir = strImportPath;

	if (dlgFilename.DoModal() != IDOK)
	{
		return;
	}

	CString strFilename = dlgFilename.GetPathName();

	if (HandleCSVImport(strFilename) == TRUE)
	{
		CString strPath = dlgFilename.GetPathName();
		CString strFile = dlgFilename.GetFileName();

		if (strPath.GetLength() >= strFile.GetLength())
		{
			strPath = strPath.Left(strPath.GetLength() - strFile.GetLength());

			CSSFile fileImportPath;
			if (fileImportPath.Open(DataManagerNonDb.GetFilePathServerImport(m_nServerDbIdx, m_nServerLocIdx), "wb") == TRUE)
			{
				fileImportPath.WriteLine(strPath);
			}
		}
	}
}

/**********************************************************************/

bool CListDataServerDlg::HandleCSVImport(CString& strFilename)
{
	CCSVDBImportTypeDlg dlgMethod(NODE_SERVER, this);
	dlgMethod.m_nImportMethod = IMPORT_METHOD_ALL;

	if (dlgMethod.DoModal() != IDOK)
	{
		return FALSE;
	}

	CDatabaseImportHandlerCommon ImportHandler;
	ImportHandler.SetImportMethod(dlgMethod.m_nImportMethod);
	int nResult = ImportHandler.ImportServerFile(strFilename);

	if (nResult != IMPORTERROR_NONE)
	{
		CString strMsg;
		strMsg.Format("Unable to process import file\n(error code %d)", nResult);
		Prompter.Error(strMsg);
		return FALSE;
	}

	/*
	CDWordArray dw;
	dw.Add(ImportHandler.GetImportTotalLines());
	dw.Add(ImportHandler.GetImportIgnoredLines());
	dw.Add(ImportHandler.GetImportPluAdded());
	dw.Add(ImportHandler.GetImportPluModified());
	MessageLogger.LogSystem(2, 3, FALSE, dw);
	*/

	DataManagerNonDb.Server.SetSortMode(SERVER_SORT_NUMBER);
	m_listData.SetItemCountEx(GetArraySize());
	m_listData.Invalidate();
	SelectLine(0);

	CCSVDBImportStatsDlg dlgInfo(NODE_SERVER, this);
	dlgInfo.m_nTotalLines = ImportHandler.GetImportTotalLines();
	dlgInfo.m_nIgnoredLines = ImportHandler.GetImportIgnoredLines();
	dlgInfo.m_nRecordsAdded = ImportHandler.GetImportRecordsAdded();
	dlgInfo.m_nRecordsModified = ImportHandler.GetImportRecordsModified();
	dlgInfo.DoModal();

	if ((dlgInfo.m_nRecordsAdded > 0) || (dlgInfo.m_nRecordsModified > 0))
	{
		m_WarnCancelAfterChange.SetFlag();
	}

	return TRUE;
}

/**********************************************************************/

void CListDataServerDlg::HandleDownload( CArray<int,int>& arrayLocIdx )
{
	if ( CSelectMultipleLocationDlg::FilterLocListForDownload( arrayLocIdx, "Download Servers", this ) == TRUE )
	{
		CEposTaskDatabase TaskDatabase;
		TaskDatabase.CreateSimpleTask( DN_SERVER_NAME, arrayLocIdx );

		CString strDate = "";
		{
			CEposTaskHeader TaskHeader;
			TaskDatabase.GetEditableTaskHeader( TaskHeader );

			CString strTitle;
			TaskHeader.SetName( "Download Servers" );
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

void CListDataServerDlg::HandleClickHeader(int nColumn)
{
	int nDbNo = 0;
	if (m_nServerDbIdx >= 0)
	{
		nDbNo = dbDatabase.GetDbNo(m_nServerDbIdx);
	}

	if (m_nSearchColumn == nColumn)
	{
		if ((nColumn >= 0) && (GetArraySize() > 1))
		{
			if (m_pSearchDlg == NULL)
			{
				m_pSearchDlg = new CListDataSearchDlg(&m_pSearchDlg, m_strSearchPrompt, this);
				m_pSearchDlg->Create(IDD_DEF_EDIT, this);
			}

			m_pSearchDlg->PostMessage(WM_SETFOCUS);
			m_pSearchDlg->LoadSettings(m_bSearchPrevious, m_strSearchText);
		}
	}
	else if (0 == nColumn)
	{
		if (DataManagerNonDb.Server.GetSortMode() != SERVER_SORT_NUMBER)
		{
			DataManagerNonDb.Server.SetSortMode(SERVER_SORT_NUMBER);
			m_listData.Invalidate();
			SelectLine(0);
		}
	}
	else if (1 == nColumn)
	{
		if (DataManagerNonDb.Server.GetSortMode() == SERVER_SORT_NAME_UP )
		{
			DataManagerNonDb.Server.SetSortMode(SERVER_SORT_NAME_DOWN);
		}
		else
		{
			DataManagerNonDb.Server.SetSortMode(SERVER_SORT_NAME_UP);
		}

		m_listData.Invalidate();
		SelectLine(0);
	}
	else if (3 == nColumn)
	{
		if (DataManagerNonDb.Server.GetSortMode() == SERVER_SORT_HOMELOC_UP )
		{
			DataManagerNonDb.Server.SetSortMode(SERVER_SORT_HOMELOC_DOWN);
		}
		else
		{
			DataManagerNonDb.Server.SetSortMode(SERVER_SORT_HOMELOC_UP);
		}

		m_listData.Invalidate();
		SelectLine(0);
	}
}

/**********************************************************************/

bool CListDataServerDlg::SearchText(int nIndex, CString strSearchText)
{
	bool bResult = FALSE;

	CServerCSVRecord Server{};
	int nArrayIdx = DataManagerNonDb.Server.GetArrayIdxFromSortIdx(nIndex);
	DataManagerNonDb.Server.GetAt(nArrayIdx, Server);

	CString strText = Server.GetReportName();
	strText.MakeUpper();

	if (strText.Find(strSearchText) != -1)
	{
		SelectLine(nIndex);
		bResult = TRUE;
	}

	return bResult;
}

/**********************************************************************/
