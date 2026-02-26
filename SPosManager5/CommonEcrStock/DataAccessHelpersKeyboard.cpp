/**********************************************************************/
#include "CommsBatchDlg.h"
#include "DownloadDateDlg.h"
#include "SPOSVersions.h"
#include "FileRemover.h"
#include "KeyboardHelpers.h"
#include "KeyboardInfoDlg.h"
#include "KeyboardJobInfo.h"
#include "KeyboardNumberDlg.h"
#include "KeyboardSetCSVArray.h"
#include "LocationCSVArray.h"
#include "NetworkCSVArray.h"
#include "Password.h"
#include "SelectMultipleKbdCloneTargetDlg.h"
#include "SPOSKeyboardManager.h"
#include "SysInfo.h"
/**********************************************************************/
#include "DataAccessHelpersKeyboard.h"
/**********************************************************************/

CDataAccessHelpersKeyboard::CDataAccessHelpersKeyboard()
{
	BuildKeyboardMap();
}

/**********************************************************************/

void CDataAccessHelpersKeyboard::BuildKeyboardMap()
{
	m_KeyboardNameArray.ReadNames();
	m_KeyboardMap.ClearMap();
	m_KeyboardMap.BuildMap();
}

/**********************************************************************/

void CDataAccessHelpersKeyboard::ConvertMapInfoToNodeInfo( CKeyboardMapInfo& infoMap, CDataNodeInfoKeyboard& infoNode )
{
	infoNode.m_nSPOSVersion = infoMap.m_nSPOSVersion;
	infoNode.m_nNodeType = ( infoMap.m_nEntryType == KBD_MAP_LOC ) ? NODE_LOCATION : NODE_DBKEYBOARDSET;
	infoNode.m_nDbIdx = infoMap.m_nDbIdx;
	infoNode.m_nKbsIdx = infoMap.m_nKbsIdx;
	infoNode.m_nLocIdx = infoMap.m_nLocIdx;		
	infoNode.m_nKbNo = 0;
}

/**********************************************************************/

int CDataAccessHelpersKeyboard::GetKbNoByNodeType( int nId )
{
	if ( ( nId >= NODE_KEYBOARD1 ) && ( nId < NODE_KEYBOARD1 + MAX_KEYBOARDS_PER_LOCATION ) )
		return nId + 1 - NODE_KEYBOARD1;

	if ( ( nId >= NODE_DBKEYBOARD1 ) && ( nId < NODE_DBKEYBOARD1 + MAX_KEYBOARDS_PER_LOCATION ) )
		return nId + 1 - NODE_DBKEYBOARD1;

	return 0;
}

/**********************************************************************/

const char* CDataAccessHelpersKeyboard::GetKeyboardFolder( CDataNodeInfoKeyboard& infoNode )
{
	if ( NODE_LOCATION == infoNode.m_nNodeType )
		m_strKeyboardFolder = dbLocation.GetFolderPathDataKeyboard( infoNode.m_nLocIdx, infoNode.m_nKbNo );
	else
		m_strKeyboardFolder = dbKeyboardSet.GetFolderPathKeyboard( infoNode.m_nKbsIdx, infoNode.m_nKbNo );

	return m_strKeyboardFolder;
}

/**********************************************************************/

const char* CDataAccessHelpersKeyboard::GetKeyboardLabel ( CDataNodeInfoKeyboard& infoNode )
{
	m_strKeyboardLabel.Format ( "Kbd %2.2d", infoNode.m_nKbNo );

	if ( ( infoNode.m_nKbNo < 1 ) || ( infoNode.m_nKbNo > MAX_KEYBOARDS_PER_LOCATION ) )
		return m_strKeyboardLabel;

	CString strExtra = "";
	
	if ( NODE_DBKEYBOARDSET == infoNode.m_nNodeType )
		m_KeyboardNameArray.GetKbSetKeyboardName( infoNode.m_nKbsIdx, infoNode.m_nKbNo, strExtra );
	else
		m_KeyboardNameArray.GetLocKeyboardName( infoNode.m_nLocIdx, infoNode.m_nKbNo, strExtra );
	
	if ( strExtra.GetLength() > 20 )
		strExtra = "";

	CString strPath = GetKeyboardFolder( infoNode );

	if ( ::ExistSubdirectory ( strPath ) == FALSE )
		strExtra = "unused";
	else
	{
		CCSV csvFiles;

		if ( CSPOSVersions::CheckSPOSVersion( infoNode.m_nSPOSVersion, SPOS_V4 ) == TRUE )
		{
			csvFiles.Add( "\\KbdV4.sql" );
			csvFiles.Add( "\\KbdV3_4.sql" );
			csvFiles.Add( "\\KbdV3.sql" );
		}
		
		bool bFound = FALSE;
		for ( int n = 0; n < csvFiles.GetSize(); n++ )
		{
			CString strFilename = strPath + csvFiles.GetString(n);

			if ( ::FileExists ( strFilename ) == TRUE )	
			{
				if ( strExtra == "" )
				{
					CFileStatus FileStatus;
					CFile::GetStatus ( strFilename, FileStatus );
					CTime time = FileStatus.m_mtime;				
					strExtra = time.Format ( "%d/%m/%Y at %H:%M:%S" );
				}

				bFound = TRUE;
				break;
			}
		}

		if ( FALSE == bFound )
			if ( strExtra == "" )
				strExtra = "in use";
	}

	m_strKeyboardLabel += " (";
	m_strKeyboardLabel += strExtra;
	m_strKeyboardLabel += ")";

	return m_strKeyboardLabel;
}

/**********************************************************************/

bool CDataAccessHelpersKeyboard::KeyboardInUse( CDataNodeInfoKeyboard& infoNode )
{
	switch( infoNode.m_nNodeType )
	{
	case NODE_LOCATION:
		return LocKeyboardInUse( infoNode );

	case NODE_DBKEYBOARDSET:
		return DbKeyboardInUse( infoNode );

	default:
		return FALSE;
	}
}

/**********************************************************************/

bool CDataAccessHelpersKeyboard::LocKeyboardInUse( CDataNodeInfoKeyboard& infoNode )
{
	if ( infoNode.m_nLocIdx < 0 || infoNode.m_nLocIdx >= dbLocation.GetSize() )
		return FALSE;
		
	if ( infoNode.m_nKbNo < 1 || infoNode.m_nKbNo > EcrmanOptions.GetFeaturesKeyboardsPerLocation() )
		return FALSE;

	return ::ExistSubdirectory ( GetKeyboardFolder( infoNode ) );
}

/**********************************************************************/

bool CDataAccessHelpersKeyboard::DbKeyboardInUse( CDataNodeInfoKeyboard& infoNode )
{
	if ( infoNode.m_nDbIdx < 0 || infoNode.m_nDbIdx >= dbDatabase.GetSize() )
		return FALSE;

	if ( infoNode.m_nKbsIdx < 0 || infoNode.m_nKbsIdx >= dbKeyboardSet.GetSize() )
		return FALSE;
		
	if ( infoNode.m_nKbNo < 1 || infoNode.m_nKbNo > EcrmanOptions.GetFeaturesKeyboardsPerLocation() )
		return FALSE;

	return ::ExistSubdirectory( GetKeyboardFolder( infoNode ) );
}

/**********************************************************************/

bool CDataAccessHelpersKeyboard::IsLocKeyboardNode( CDataNodeInfoKeyboard& infoNode )
{
	return( ( NODE_LOCATION == infoNode.m_nNodeType ) && ( infoNode.m_nKbNo != 0 ) );
}

/**********************************************************************/

bool CDataAccessHelpersKeyboard::IsDbKeyboardNode( CDataNodeInfoKeyboard& infoNode )
{
	return ( ( NODE_DBKEYBOARDSET == infoNode.m_nNodeType ) && ( infoNode.m_nKbNo != 0 ) );
}

/**********************************************************************/

bool CDataAccessHelpersKeyboard::IsKeyboardNode( CDataNodeInfoKeyboard& infoNode )
{
	return ( IsLocKeyboardNode( infoNode ) || IsDbKeyboardNode( infoNode ) );
}

/**********************************************************************/

bool CDataAccessHelpersKeyboard::EditKeyboard( CDataNodeInfoKeyboard& infoNode )
{
	bool bUpdateLabels = FALSE;

	CString strFilename = "";
	switch( infoNode.m_nNodeType )
	{
	case NODE_LOCATION:
		{
			CFilenameUpdater FnUp( SysFiles::KeyboardLock, infoNode.m_nLocIdx, infoNode.m_nKbNo );
			strFilename = FnUp.GetFilenameToUse();
		}
		break;

	case NODE_DBKEYBOARDSET:
		{
			CFilenameUpdater FnUp( SysFiles::KbSetLock, infoNode.m_nKbsIdx, infoNode.m_nKbNo );
			strFilename = FnUp.GetFilenameToUse();
		}
		break;
	}

	switch( infoNode.m_nNodeType )
	{
	case NODE_LOCATION:
	case NODE_DBKEYBOARDSET:
		{
			CSSFile fileLock;
			if ( fileLock.Open( strFilename, "wb" ) == TRUE )
			{
				EditKeyboardInternal ( infoNode );
				fileLock.Close();
				bUpdateLabels = TRUE;
			}
			else
			{
				CDataManagerInfo info;
				info.m_nNodeType = NODE_KEYBOARD1;
				info.m_nErrorType = DB_ERR_SHARE;
				Prompter.ShareError( info );
			}
		}
		break;
	}

	return bUpdateLabels;
}

/**********************************************************************/

void CDataAccessHelpersKeyboard::EditKeyboardInternal( CDataNodeInfoKeyboard& infoNode )
{
	CDataManagerInfo info;
	DataManager.OpenDatabaseReadOnly( infoNode.m_nDbIdx, info, FALSE );

	CCSV csvSQL;
	csvSQL.Add( "kbdv4.sql" );
	csvSQL.Add( "kbdv3_4.sql" );
	csvSQL.Add( "kbdv3.sql" );

	CSPOSKeyboardManagerParams Params;

	if ( NODE_LOCATION == infoNode.m_nNodeType )
	{
		Params.m_nDbIdx = infoNode.m_nDbIdx;
		Params.m_nLocIdx = infoNode.m_nLocIdx;
		Params.m_bKbSet = FALSE;

		if ( EcrmanOptions.GetPluPriceSetsFlag() == TRUE )
			Params.m_nPriceSetCount = dbLocation.GetPluPriceSetCount( infoNode.m_nLocIdx );
		else
			Params.m_nPriceSetCount = 0;
	}
	else
	{
		Params.m_nDbIdx = infoNode.m_nDbIdx;
		Params.m_nLocIdx = 0;
		Params.m_bKbSet = TRUE;
	}

	Params.m_strDataPath = GetKeyboardFolder( infoNode );
	Params.m_strKeyboardName = m_KeyboardNameArray.GetKeyboardDisplayName( infoNode );
	Params.m_strSQLFileIn = csvSQL.GetLine();
	Params.m_strSQLFileOut = csvSQL.GetString(0);
	Params.m_nSPOSVersion = infoNode.m_nSPOSVersion;
	
	Params.m_strImageFolder = "Images";
	GetDataProgramPath( Params.m_strImageFolder );
	::CreateSubdirectory( Params.m_strImageFolder );

	Params.m_strSysFilesFolder = "SysFiles";
	GetDataProgramPath( Params.m_strSysFilesFolder );

	Params.m_strListNameFolder = dbDatabase.GetFolderPathData( infoNode.m_nDbIdx );

	Params.m_nPasswordIdx = 0;
	if ( PasswordArray.IsMasterPassword() == FALSE )
		Params.m_nPasswordIdx = PasswordArray.GetPasswordIdx();

	SPOSKeyboardManager.ProcessKeyboard( m_pParentWnd, Params );
}

/**********************************************************************/

void CDataAccessHelpersKeyboard::GetKeyboardListForCloning( CDataNodeInfoKeyboard& infoNode, CArray<CSelectMultipleEntityInfo,CSelectMultipleEntityInfo>& arraySets, CArray<int,int>& arrayLocs, bool bSingle )
{
	arraySets.RemoveAll();
	arrayLocs.RemoveAll();

	for ( int nMapIdx = 0; nMapIdx < m_KeyboardMap.GetMapInfoCount(); nMapIdx++ )
	{
		CKeyboardMapInfo arrayInfo;
		m_KeyboardMap.GetMapInfo( nMapIdx, arrayInfo );

		switch( arrayInfo.m_nEntryType )
		{
		case KBD_MAP_SET:
			if ( ( TRUE == bSingle ) || ( infoNode.m_nNodeType != NODE_DBKEYBOARDSET ) || ( infoNode.m_nDbIdx != arrayInfo.m_nDbIdx ) || ( infoNode.m_nKbsIdx != arrayInfo.m_nKbsIdx ) )
			{
				int nBaseSPOSVersion1 = CSPOSVersions::GetSPOSBaseVersion( infoNode.m_nSPOSVersion );
				int nBaseSPOSVersion2 = CSPOSVersions::GetSPOSBaseVersion( arrayInfo.m_nSPOSVersion );

				if ( ( nBaseSPOSVersion1 == nBaseSPOSVersion2 ) && ( nBaseSPOSVersion1 != -1 ) )
				{
					CSelectMultipleEntityInfo info;
					info.m_nNodeType = NODE_DATABASE;
					info.m_nDbIdx = arrayInfo.m_nDbIdx;
					info.m_nKbsIdx = arrayInfo.m_nKbsIdx;;
					info.m_bSelected = FALSE;
					arraySets.Add( info );
				}
			}
			break;

		case KBD_MAP_LOC:
			if ( ( TRUE == bSingle ) || ( infoNode.m_nNodeType != NODE_LOCATION ) || ( infoNode.m_nLocIdx != arrayInfo.m_nLocIdx ) )
			{
				int nBaseSPOSVersion1 = CSPOSVersions::GetSPOSBaseVersion( infoNode.m_nSPOSVersion );
				int nBaseSPOSVersion2 = CSPOSVersions::GetSPOSBaseVersion( arrayInfo.m_nSPOSVersion );

				if ( ( nBaseSPOSVersion1 == nBaseSPOSVersion2 ) && ( nBaseSPOSVersion1 != -1 ) )
					arrayLocs.Add( arrayInfo.m_nLocIdx );
			}
			break;
		}
	}
}

/**********************************************************************/

int CDataAccessHelpersKeyboard::CloneKeyboards( CDataNodeInfoKeyboard& infoNode )
{
	switch( infoNode.m_nNodeType )
	{
	case NODE_LOCATION:
	case NODE_DBKEYBOARDSET:
		break;

	default:
		return 0;
	}

	if ( 0 == infoNode.m_nKbNo )
	{
		bool bFoundKeyboard = FALSE;
		for ( int nLocKbNo = 1; nLocKbNo <= EcrmanOptions.GetFeaturesKeyboardsPerLocation(); nLocKbNo++ )
		{
			infoNode.m_nKbNo = nLocKbNo;
			if ( KeyboardInUse ( infoNode ) == TRUE )
			{
				bFoundKeyboard = TRUE;
				break;
			}
		}
			
		if ( bFoundKeyboard == FALSE )
			return 1;

		CArray<CSelectMultipleEntityInfo,CSelectMultipleEntityInfo> arraySets;
		
		{
			CArray<int,int> arrayLocs;
			GetKeyboardListForCloning( infoNode, arraySets, arrayLocs, FALSE );

			if ( EcrmanOptions.GetReportsLocationSortType() != 0 )
				dbLocation.SortLocListByName( arrayLocs );

			for ( int n = 0; n < arrayLocs.GetSize(); n++ )
			{
				CSelectMultipleEntityInfo infoKbSet;
				infoKbSet.m_nNodeType = NODE_LOCATION;
				infoKbSet.m_nLocIdx = arrayLocs.GetAt(n);
				infoKbSet.m_nKbsIdx = 0;
				infoKbSet.m_bSelected = FALSE;
				arraySets.Add( infoKbSet );
			}
		}

		if ( arraySets.GetSize() < 1 )
			return 2;

		CString strTitle;

		if ( NODE_LOCATION == infoNode.m_nNodeType )
		{
			strTitle.Format ( "Clone Location Keyboards (%s)", 
				dbLocation.GetName( infoNode.m_nLocIdx ) );
		}
		else
		{
			strTitle.Format ( "Clone Keyboard Set (%s, %s)", 
				dbDatabase.GetName( infoNode.m_nDbIdx ),
				dbKeyboardSet.GetName( infoNode.m_nKbsIdx ) );
		}

		CSelectMultipleKbdCloneTargetDlg dlg ( strTitle, arraySets, FALSE, m_pParentWnd );
		
		if ( dlg.DoModal() != IDOK )
			return 0;

		if ( arraySets.GetSize() == 0 )
			return 0;

		for ( int nPos = 0; nPos < arraySets.GetSize(); nPos++ )
		{
			if ( TRUE == arraySets[nPos].m_bSelected )
			{
				CDataNodeInfoKeyboard infoDest;
				infoDest.m_nNodeType = arraySets[nPos].m_nNodeType;
				infoDest.m_nKbsIdx = arraySets[nPos].m_nKbsIdx;

				if ( NODE_LOCATION == infoDest.m_nNodeType )
				{
					infoDest.m_nLocIdx = arraySets[nPos].m_nLocIdx;
					infoDest.m_nDbIdx = 0;
				}
		 		else
				{
					infoDest.m_nLocIdx = 0;
					infoDest.m_nDbIdx = arraySets[nPos].m_nDbIdx;
				}

				m_KeyboardNameArray.CopyKeyboardNames( infoNode, infoDest );
				m_KeyboardNameArray.WriteKeyboardNames( infoDest );

				for ( int nKbNo = 1; nKbNo <= EcrmanOptions.GetFeaturesKeyboardsPerLocation(); nKbNo++ )
				{
					infoNode.m_nKbNo = nKbNo;
					infoDest.m_nKbNo = nKbNo;
					if ( KeyboardInUse( infoNode ) == TRUE )
					{
						CString strSourcePath = GetKeyboardFolder( infoNode );	
						CString strDestPath = GetKeyboardFolder( infoDest );	
						CKeyboardHelpers::CloneFolder( strSourcePath, strDestPath );
					}			
				}
			}
		}

		return 3;
	}
	else
	{
		if ( KeyboardInUse ( infoNode ) == FALSE )
			return 4;

		CArray<CSelectMultipleEntityInfo,CSelectMultipleEntityInfo> arraySets;
		
		{
			CArray<int,int> arrayLocs;
			GetKeyboardListForCloning( infoNode, arraySets, arrayLocs, TRUE );

			if ( EcrmanOptions.GetReportsLocationSortType() != 0 )
				dbLocation.SortLocListByName( arrayLocs );

			for ( int n = 0; n < arrayLocs.GetSize(); n++ )
			{
				CSelectMultipleEntityInfo infoKbSet;
				infoKbSet.m_nNodeType = NODE_LOCATION;
				infoKbSet.m_nLocIdx = arrayLocs.GetAt(n);
				infoKbSet.m_nKbsIdx = 0;
				infoKbSet.m_bSelected = FALSE;
				arraySets.Add( infoKbSet );
			}
		}
	
		if ( ( arraySets.GetSize() <= 1 ) && ( EcrmanOptions.GetFeaturesKeyboardsPerLocation() == 1 ) )
			return 2;

		for ( int n = arraySets.GetSize() - 1; n >= 0; n-- )
		{
			CSelectMultipleEntityInfo infoKbSet = arraySets[n];
			arraySets.RemoveAt(n);
			int nInsertPos = n;
			
			for ( int k = 1; k <= EcrmanOptions.GetFeaturesKeyboardsPerLocation(); k++ )
			{
				bool bAllow = TRUE;

				if ( k == infoNode.m_nKbNo ) 
				{
					if ( NODE_LOCATION == infoNode.m_nNodeType )
					{
						if ( NODE_LOCATION == infoKbSet.m_nNodeType ) 
							if ( infoKbSet.m_nLocIdx == infoNode.m_nLocIdx ) 
									bAllow = FALSE;
					}
					else if ( NODE_DBKEYBOARDSET == infoNode.m_nNodeType  )
					{
						if ( NODE_DATABASE == infoKbSet.m_nNodeType  ) 
							if ( infoKbSet.m_nDbIdx == infoNode.m_nDbIdx )
								if ( infoKbSet.m_nKbsIdx == infoNode.m_nKbsIdx )
									bAllow = FALSE;
					}
				}
			
				if ( TRUE == bAllow )
				{
					infoKbSet.m_nKbNo = k;
					arraySets.InsertAt( nInsertPos++, infoKbSet );
				}
			}
		}

		CString strTitle;
		strTitle.Format ( "Clone (%s)",
			m_KeyboardNameArray.GetKeyboardDisplayName( infoNode ) );

		CSelectMultipleKbdCloneTargetDlg dlg ( strTitle, arraySets, TRUE, m_pParentWnd );
		
		if ( dlg.DoModal() != IDOK )
			return 0;

		if ( arraySets.GetSize() == 0 )
			return 0;

		CDataNodeInfoKeyboard infoDest, infoDestCurrent;
		infoDestCurrent.m_nNodeType = -1;
		
		for ( int nPos = 0; nPos < arraySets.GetSize(); nPos++ )
		{
			if ( TRUE == arraySets[nPos].m_bSelected )
			{
				infoDest.m_nNodeType = arraySets[nPos].m_nNodeType;
				infoDest.m_nKbsIdx = arraySets[nPos].m_nKbsIdx;

				if ( NODE_LOCATION == infoDest.m_nNodeType )
				{
					infoDest.m_nLocIdx = arraySets[nPos].m_nLocIdx;
					infoDest.m_nDbIdx = 0;
				}
				else
				{
					infoDest.m_nLocIdx = 0;
					infoDest.m_nDbIdx = arraySets[nPos].m_nDbIdx;
				}

				infoDest.m_nKbNo = arraySets[nPos].m_nKbNo;

				CString strSourcePath = GetKeyboardFolder( infoNode );	
				CString strDestPath = GetKeyboardFolder( infoDest );	
				CKeyboardHelpers::CloneFolder ( strSourcePath, strDestPath );

				if ( dlg.GetCopyNameFlag() == TRUE )
				{
					CString strName;
					m_KeyboardNameArray.GetKeyboardName( infoNode, strName );
					m_KeyboardNameArray.SetKeyboardName( infoDest, strName );

					if ( infoDestCurrent.Compare( infoDest ) == FALSE )
					{
						if ( infoDestCurrent.m_nNodeType != -1 )
							m_KeyboardNameArray.WriteKeyboardNames( infoDestCurrent );

						infoDestCurrent = infoDest;
					}
				}
			}
		}

		if ( dlg.GetCopyNameFlag() == TRUE )
			if ( infoDestCurrent.m_nNodeType != -1 )
				m_KeyboardNameArray.WriteKeyboardNames( infoDestCurrent );

		return 3;
	}
}

/**********************************************************************/

void CDataAccessHelpersKeyboard::DownloadKeyboard( CDataNodeInfoKeyboard& infoNode )
{
	if ( 0 == infoNode.m_nKbNo )
	{
		infoNode.m_nKbNo = 0;

		switch ( infoNode.m_nNodeType )
		{	
		case NODE_SYSTEM:
		case NODE_DATABASE:
		case NODE_LOCATION:
		case NODE_DBKEYBOARDSET:
			{
				CKeyboardNumberDlg dlg ( m_pParentWnd );
				if ( dlg.DoModal() == IDOK )
					infoNode.m_nKbNo = dlg.m_nKbNo;
			}
			break;
		}
	}

	if ( 0 == infoNode.m_nKbNo )
		return;

	CString strLocName = "";
	switch( infoNode.m_nNodeType )
	{
	case NODE_SYSTEM:
		strLocName = "System";
		break;

	case NODE_DATABASE:
		strLocName = dbDatabase.GetName( infoNode.m_nDbIdx );
		break;

	case NODE_LOCATION:
		strLocName = dbLocation.GetName( infoNode.m_nLocIdx );
		break;

	case NODE_DBKEYBOARDSET:
		strLocName.Format( "%s, %s", dbDatabase.GetName( infoNode.m_nDbIdx ), dbKeyboardSet.GetName( infoNode.m_nKbsIdx ) );
		break;
	}

	CArray<int,int> arrayLocIdx;

	for ( int n = 0; n < m_KeyboardMap.GetLocInfoCount(); n++ )
	{
		CKeyboardMapInfo infoLoc;
		m_KeyboardMap.GetLocInfo( n, infoLoc );

		bool bInclude = FALSE;

		switch( infoLoc.m_nEntryType )
		{
		case KBD_MAP_LOC:
		case KBD_MAP_SET:
			switch( infoNode.m_nNodeType )
			{
			case NODE_SYSTEM:
				bInclude = TRUE;
				break;

			case NODE_DATABASE:
				bInclude = ( infoLoc.m_nDbIdx == infoNode.m_nDbIdx );
				break;

			case NODE_LOCATION:
				if ( KBD_MAP_LOC == infoLoc.m_nEntryType )
					bInclude = ( infoNode.m_nLocIdx == infoLoc.m_nLocIdx );
				break;

			case NODE_DBKEYBOARDSET:
				if ( KBD_MAP_SET == infoLoc.m_nEntryType )
					bInclude = ( infoNode.m_nDbIdx == infoLoc.m_nDbIdx ) && ( infoNode.m_nKbsIdx == infoLoc.m_nKbsIdx );
				break;
			}
			break;
		}

		if ( TRUE == bInclude )
			arrayLocIdx.Add( infoLoc.m_nLocIdx );		
	}

	if ( arrayLocIdx.GetSize() == 0 )
		return;

	CWordArray arrayJobType;

	CKeyboardJobInfo infoKb;
	infoKb.m_nKbNo = infoNode.m_nKbNo;
	int nJobNo = infoKb.GetJobNo();

	if ( 0 == nJobNo )
		return;

	arrayJobType.Add( nJobNo );

	CString strDlgTitle, strTaskName;
	
	if ( arrayLocIdx.GetSize() == 1 )
	{
		strDlgTitle.Format ( "Epos Import Date (%s, Keyboard %2.2d)", 
			dbLocation.GetName( arrayLocIdx.GetAt(0) ),		
			infoNode.m_nKbNo );
	}
	else
	{
		strDlgTitle.Format ( "Epos Import Date (%s, Keyboard %2.2d)", 
			(const char*) strLocName,
			infoNode.m_nKbNo );
	}

	strTaskName.Format ( "Download Keyboard %2.2d", infoNode.m_nKbNo );

	CString strDate = "";
	CDownloadDateDlg dlgDate( strDlgTitle, m_pParentWnd );

	dlgDate.SetV3KeyboardFlag();
	if ( dlgDate.DoModal() != IDOK )
		return;

	if ( dlgDate.m_bImages != 0 )
		arrayJobType.Add( DN_IMAGES );

	CEposTaskDatabase TaskDatabase;
	TaskDatabase.CreateSimpleTask( arrayJobType, arrayLocIdx );
	
	CEposTaskHeader TaskHeader;
	TaskDatabase.GetEditableTaskHeader( TaskHeader );
	
	if ( dlgDate.m_bImmediate == TRUE )
		TaskHeader.SetImportType( IMPORT_IMMEDIATE );
	else
	{
		strDate = dlgDate.m_strDate;
		TaskHeader.SetImportType( IMPORT_SPECIFIED );
	}

	TaskHeader.SetName( strTaskName );
	TaskDatabase.SetEditableTaskHeader( TaskHeader );

	CCommsBatchDlg dlg( TaskDatabase, strDate, FALSE, m_KeyboardMap, -1, 0, 0, m_pParentWnd );
	dlg.DoModal();
}

/**********************************************************************/

void CDataAccessHelpersKeyboard::ShowDeletionResult( int nResult )
{
	CString strMsg = "";

	switch( nResult )
	{
	case 0:
		strMsg = "The keyboard files have been deleted.";
		Prompter.Info( strMsg );
		break;

	case 1:
		strMsg += "There is no keyboard folder at\n";       
		strMsg += "the selected position.";
		Prompter.Error ( strMsg );
		break;

	case 2:
		strMsg += "There are no keyboard data files at\n";       
		strMsg += "the selected position.";
		Prompter.Error ( strMsg );
		break;
	}
}

/**********************************************************************/

void CDataAccessHelpersKeyboard::ShowCloneResult( int nResult )
{
	CString strMsg = "";

	switch( nResult )
	{
	case 1:
		strMsg += "You cannot clone a location for which\n";
		strMsg += "no keyboards have been created.";
		Prompter.Error ( strMsg );
		break;

	case 2:
		strMsg += "There are no other locations available that\n";
		strMsg += "are linked to the same EPOS family.";
		Prompter.Error ( strMsg );
		break;

	case 4:
		strMsg += "You cannot clone an empty keyboard folder";
		Prompter.Error ( strMsg );
		break;
	}
}

/**********************************************************************/

int CDataAccessHelpersKeyboard::DeleteKeyboards( CDataNodeInfoKeyboard& infoNode )
{
	int nResult = -1;

	switch( infoNode.m_nNodeType )
	{
	case NODE_LOCATION:
	case NODE_DBKEYBOARDSET:
		
		if ( infoNode.m_nKbNo != 0 )
		{
			if ( KeyboardInUse ( infoNode ) == FALSE )
				nResult = 1;
			else
			{
				CString strLockFilename = "";
				if ( NODE_LOCATION == infoNode.m_nNodeType )
				{
					CFilenameUpdater FnUp( SysFiles::KeyboardLock, infoNode.m_nLocIdx, infoNode.m_nKbNo );
					strLockFilename = FnUp.GetFilenameToUse();
				}
				else
				{
					CFilenameUpdater FnUp( SysFiles::KbSetLock, infoNode.m_nKbsIdx, infoNode.m_nKbNo );
					strLockFilename = FnUp.GetFilenameToUse();
				}

				CSSFile fileLock;
				if ( fileLock.Open( strLockFilename, "wb" ) == FALSE )
				{
					CDataManagerInfo info;
					info.m_nNodeType = NODE_KEYBOARD1;
					info.m_nErrorType = DB_ERR_SHARE;
					Prompter.ShareError( info );
				}
				else
				{
					nResult = DeleteSingleKeyboard( infoNode );
					fileLock.Close();
				}
			}
		}
		else
		{
			bool bFoundKeyboard = FALSE;
			for ( int nKbNo = 1; nKbNo <= EcrmanOptions.GetFeaturesKeyboardsPerLocation(); nKbNo++ )
			{
				infoNode.m_nKbNo = nKbNo;
				if ( KeyboardInUse ( infoNode ) == TRUE )
				{
					bFoundKeyboard = TRUE;
					break;
				}
			}
				
			if ( bFoundKeyboard == FALSE )
				nResult = 2;
			else
			{
				if ( DataManager.LockAllUsers() == FALSE )
					Prompter.LockError( "delete multiple keyboards" );
				else
				{
					nResult = DeleteMultipleKeyboards( infoNode );
					DataManager.UnlockAllUsers();
				}
			}
		}
		break;
	}

	return nResult;
}

/**********************************************************************/

int CDataAccessHelpersKeyboard::DeleteSingleKeyboard( CDataNodeInfoKeyboard& infoNode )
{
	int nResult = -1;

	CStringArray WarningArray;
	
	CString strTitle;
	strTitle.Format ( "Delete Keyboard (%s)", 
		m_KeyboardNameArray.GetKeyboardDisplayName( infoNode ) );
	
	WarningArray.Add ( strTitle );
	WarningArray.Add ( "The keyboard file for this position will be deleted from your computer." );
	WarningArray.Add ( "" );
	WarningArray.Add ( "The keyboard layouts on your EPOS network will not be affected." );

	if ( Prompter.Warning( WarningArray ) == IDYES )
	{
		CString strFolder = GetKeyboardFolder( infoNode );
		CFileRemover FileRemover;
		FileRemover.RemoveFolder( strFolder );
		nResult = 0;
	}
			
	return nResult;
}

/**********************************************************************/

int CDataAccessHelpersKeyboard::DeleteMultipleKeyboards( CDataNodeInfoKeyboard& infoNode )
{
	int nResult = -1;

	CStringArray WarningArray;
	CString strTitle;

	if ( NODE_LOCATION == infoNode.m_nNodeType )
	{
		strTitle.Format ( "Delete Keyboards (%s)", 
			dbLocation.GetName( infoNode.m_nLocIdx ) );
	}
	else
	{
		strTitle.Format ( "Delete Keyboards (%s, %s)",
			dbDatabase.GetName( infoNode.m_nDbIdx ),
			dbKeyboardSet.GetName( infoNode.m_nKbsIdx ) );
	}

	WarningArray.Add ( strTitle );

	CString strLine = "The keyboard files for this ";
	strLine += ( NODE_LOCATION == infoNode.m_nNodeType ) ? "location" : "set";
	strLine += " will be deleted from your computer.";
	WarningArray.Add( strLine );

	WarningArray.Add ( "" );
	WarningArray.Add ( "The keyboard layouts on your EPOS network will not be affected." );

	if ( Prompter.Warning( WarningArray ) == IDYES )
	{
		for ( int nKbNo = 1; nKbNo <= EcrmanOptions.GetFeaturesKeyboardsPerLocation(); nKbNo++ )
		{
			CFileRemover FileRemover;
			infoNode.m_nKbNo = nKbNo;
			FileRemover.RemoveFolder( GetKeyboardFolder( infoNode ) );
		}
		nResult = 0;
	}

	return nResult;
}

/**********************************************************************/

bool CDataAccessHelpersKeyboard::EditNames( CDataNodeInfoKeyboard& infoNode )
{
	bool bResult = FALSE;

	if ( infoNode.m_nKbNo != 0 )
	{
		CString strLockFile = "";
		if ( NODE_LOCATION == infoNode.m_nNodeType )
		{
			CFilenameUpdater FnUp ( SysFiles::KeyboardNameLock, infoNode.m_nLocIdx );
			strLockFile = FnUp.GetFilenameToUse();
		}
		else
		{
			CFilenameUpdater FnUp ( SysFiles::KbSetNameLock, infoNode.m_nKbsIdx );
			strLockFile = FnUp.GetFilenameToUse();
		}

		CString strName = "";
		m_KeyboardNameArray.GetKeyboardName( infoNode, strName );

		CSSFile fileToken;
		if ( fileToken.Open( strLockFile, "wb" ) == FALSE )
		{
			CDataManagerInfo infoDM;
			infoDM.m_nNodeType = NODE_KEYBOARD_NAME;
			infoDM.m_nErrorType = DB_ERR_SHARE;
			Prompter.ShareError( infoDM );
		}
		else
		{
			CKeyboardInfoDlg dlg( infoNode, strName, m_pParentWnd );
		
			int nResult = dlg.DoModal();
			if ( nResult == IDOK )
			{
				m_KeyboardNameArray.SetKeyboardName( infoNode, strName );
				m_KeyboardNameArray.WriteKeyboardNames( infoNode );
			}

			fileToken.Close();
			CFileRemover FileRemover( strLockFile );

			bResult = TRUE;
		}	
	}

	return bResult;
}

/**********************************************************************/
