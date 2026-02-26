/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "FileRemover.h"
#include "SysInfo.h"
#include "StatusProgress.h"
/**********************************************************************/
#include "PluInfoResourceWrapper.h"
/**********************************************************************/

CPluInfoItemStatus::CPluInfoItemStatus()
{
	Reset();
}

/**********************************************************************/

void CPluInfoItemStatus::Reset()
{
	m_nPluNo = 0;
	m_nStatusText = PLUINFO_STATUS_KEEP;
	m_nStatusImage = PLUINFO_STATUS_KEEP;
	m_nStatusHTML = PLUINFO_STATUS_KEEP;
	m_nStatusKPImage = PLUINFO_STATUS_KEEP;
	m_bNewResourceFile = FALSE;
	m_bOriginalItemDeleted = FALSE;
}

/**********************************************************************/

void CPluInfoItemStatus::Add( CPluInfoItemStatus& source )
{
}

/**********************************************************************/

int CPluInfoItemStatus::Compare( CPluInfoItemStatus& source, int nHint )
{
	if ( m_nPluNo != source.m_nPluNo )
		return ( m_nPluNo > source.m_nPluNo ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CPluInfoDownloadHelper::CPluInfoDownloadHelper()
{
	m_nPluNo = 0;
	m_nFileType = PLUINFO_TYPE_TEXT;
	m_strFilenames = "";
	m_bModifiable = FALSE;
	m_nModifiers = 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CPluInfoResourceFolders::CPluInfoResourceFolders()
{
	m_strFolderPluInfo = "";
	m_strFolderKPImage = "";
	m_strFolderResourceCurrent = "";
	m_strFolderResourceEdit001 = "";
	m_strFolderResourceEdit002 = "";
}

/**********************************************************************/

void CPluInfoResourceFolders::CreateFolder( const char* szFolder )
{
	if ( ::ExistSubdirectory( szFolder ) == FALSE )
		::CreateSubdirectory( szFolder );
}

/**********************************************************************/

void CPluInfoResourceFolders::CreateFolderPluInfo()
{
	CreateFolder( m_strFolderPluInfo );
}

/**********************************************************************/

void CPluInfoResourceFolders::CreateFolderKPImage()
{
	CreateFolder( m_strFolderKPImage );
}

/**********************************************************************/

void CPluInfoResourceFolders::CreateFolderResourceCurrent()
{
	CreateFolderPluInfo();
	CreateFolder( m_strFolderResourceCurrent );
}

/**********************************************************************/

void CPluInfoResourceFolders::CreateFolderResourceEdit001()
{
	CreateFolderResourceCurrent();
	CreateFolder( m_strFolderResourceEdit001 );
}

/**********************************************************************/

void CPluInfoResourceFolders::CreateFolderResourceEdit002()
{
	CreateFolderResourceCurrent();
	CreateFolder( m_strFolderResourceEdit002 );
}

/**********************************************************************/

void CPluInfoResourceFolders::ClearFolder( const char* szFolder )
{
	int nDummy = 1;
	CFileRemover FileRemover;
	FileRemover.RemoveFolderContents( szFolder, nDummy );
}

/**********************************************************************/

void CPluInfoResourceFolders::ClearFolderResourceCurrent()
{
	ClearFolder( m_strFolderResourceCurrent );
}

/**********************************************************************/

void CPluInfoResourceFolders::ClearFolderResourceEdit001()
{
	ClearFolder( m_strFolderResourceEdit001 );
}

/**********************************************************************/

void CPluInfoResourceFolders::ClearFolderResourceEdit002()
{
	ClearFolder( m_strFolderResourceEdit002 );
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CPluInfoResourceFileType::CPluInfoResourceFileType( int nType, CPluInfoItemStatus& status, CPluInfoResourceFolders& Folders ) : m_currentItemStatus( status ), m_Folders( Folders )
{
	m_strPluNo = "";
	m_bSavedAsBlank = FALSE;
	m_bGotCurrentFile = FALSE;
	
	m_nType = nType;
	
	switch( m_nType )
	{
	case PLUINFO_TYPE_IMAGE:
	case PLUINFO_TYPE_KPIMAGE:
		m_strSuffixCurrent = ".dat";
		break;

	case PLUINFO_TYPE_HTML:
		m_strSuffixCurrent = ".htm";
		break;

	case PLUINFO_TYPE_TEXT:
	default:
		m_strSuffixCurrent = ".txt";
		m_nType = PLUINFO_TYPE_TEXT;
		break;
	}

	m_strSuffixEdit001 = m_strSuffixCurrent + ".xx1";
	m_strSuffixEdit002 = m_strSuffixCurrent + ".xx2";
}

/**********************************************************************/

void CPluInfoResourceFileType::ResetForNewPluNo( int nMode )
{
	if ( PLUINFO_TYPE_KPIMAGE == m_nType )
	{
		if ( SysInfo.IsModifiableSSPluNo( m_currentItemStatus.m_nPluNo ) == TRUE )
			m_strPluNo.Format( "%7.7d0", m_currentItemStatus.m_nPluNo );
		else
			m_strPluNo.Format( "%14.14I64d", m_currentItemStatus.m_nPluNo );
	}
	else
	{
		m_strPluNo.Format( "%I64d", m_currentItemStatus.m_nPluNo );
	}

	m_bGotCurrentFile = FileExistsCurrent();
	m_bSavedAsBlank = FALSE;
	
	if ( PLUINFO_MODE_EDIT == nMode )
	{
		DeleteFileEdit002();

		if ( GetItemStatusBeforeEditing() != PLUINFO_STATUS_REPLACE )
			DeleteFileEdit001();
	}
}

/**********************************************************************/

void CPluInfoResourceFileType::GetModifierStringArray( CPluInfoDownloadHelper& item, CStringArray& arrayMod )
{
	arrayMod.RemoveAll();

	if ( FALSE == item.m_bModifiable )
		arrayMod.Add( "" );
	else
	{
		arrayMod.Add( "0" );

		int nMask = 1;
		CString str = "0";
		for ( int n = 1; n <= 9; n++ )
		{
			if ( ( item.m_nModifiers & nMask ) != 0 )
			{
				str.SetAt( 0, '0' + n );
				arrayMod.Add( str );
			}
			nMask *= 2;
		}
	}
}

/**********************************************************************/

void CPluInfoResourceFileType::GetFileNameDestArray( CPluInfoDownloadHelper& item, CStringArray& arrayDest )
{
	CStringArray arrayMod;
	GetModifierStringArray( item, arrayMod );	

	for ( int nMod = 0; nMod < arrayMod.GetSize(); nMod++ )
	{
		CString strFileName = "";
		strFileName += "VTX_PLU";
		strFileName += m_strPluNo;
		strFileName += arrayMod.GetAt(nMod);

		switch( m_nType )
		{
		case PLUINFO_TYPE_IMAGE:
			{
				CString strImageData = "";
				switch ( ::GetImageType( GetFilePathCurrent(), strImageData, FALSE ) )
				{
				case IMAGE_TYPE_BMP:	strFileName += ".bmp";	break;
				case IMAGE_TYPE_GIF:	strFileName += ".gif";	break;
				case IMAGE_TYPE_JPG:	strFileName += ".jpg";	break;
				default:				strFileName = "";		break;
				}
				break;
			}
			break;

		case PLUINFO_TYPE_TEXT:
		case PLUINFO_TYPE_HTML:
		default:
			strFileName += m_strSuffixCurrent;
			break;
		}

		if ( strFileName != "" )
			arrayDest.Add( strFileName );
	}
}

/**********************************************************************/

void CPluInfoResourceFileType::GetFolderDestArray( CPluInfoDownloadHelper& item, CStringArray& arrayDest )
{
	CStringArray arrayMod;
	GetModifierStringArray( item, arrayMod );	

	for ( int nMod = 0; nMod < arrayMod.GetSize(); nMod++ )
	{
		CString strFolder = "";
		strFolder += "VTX_PLU";
		strFolder += m_strPluNo;
		strFolder += arrayMod.GetAt(nMod);
		strFolder += "_FILES";
		arrayDest.Add( strFolder );
	}
}

/**********************************************************************/

const char* CPluInfoResourceFileType::GetFileNameCurrent()
{
	m_strFileNameCurrent = "";
	m_strFileNameCurrent += m_strPluNo;
	m_strFileNameCurrent += m_strSuffixCurrent;
	return m_strFileNameCurrent;
}

/**********************************************************************/

const char* CPluInfoResourceFileType::GetWorkingFolder()
{
	m_strWorkingFolder = "";

	switch( m_nType )
	{
	case PLUINFO_TYPE_KPIMAGE:
		m_strWorkingFolder = m_Folders.m_strFolderKPImage;
		break;

	default:
		m_strWorkingFolder = m_Folders.m_strFolderPluInfo;
		break;
	}

	return m_strWorkingFolder;
}

/**********************************************************************/

const char* CPluInfoResourceFileType::GetFilePathCurrent()
{
	m_strFilePathCurrent = GetWorkingFolder();
	m_strFilePathCurrent += "\\";
	m_strFilePathCurrent += GetFileNameCurrent();
	return m_strFilePathCurrent;
}

/**********************************************************************/

const char* CPluInfoResourceFileType::GetFileNameEdit001()
{
	m_strFileNameEdit001 = "";
	m_strFileNameEdit001 += m_strPluNo;
	m_strFileNameEdit001 += m_strSuffixEdit001;
	return m_strFileNameEdit001;
}

/**********************************************************************/

const char* CPluInfoResourceFileType::GetFilePathEdit001()
{
	m_strFilePathEdit001 = GetWorkingFolder();
	m_strFilePathEdit001 += "\\";
	m_strFilePathEdit001 += GetFileNameEdit001();
	return m_strFilePathEdit001;
}

/**********************************************************************/

const char* CPluInfoResourceFileType::GetFileNameEdit002()
{
	m_strFileNameEdit002 = "";
	m_strFileNameEdit002 += m_strPluNo;
	m_strFileNameEdit002 += m_strSuffixEdit002;
	return m_strFileNameEdit002;
}

/**********************************************************************/

const char* CPluInfoResourceFileType::GetFilePathEdit002()
{
	m_strFilePathEdit002 = GetWorkingFolder();	
	m_strFilePathEdit002 += "\\";
	m_strFilePathEdit002 += GetFileNameEdit002();
	return m_strFilePathEdit002;
}

/**********************************************************************/

void CPluInfoResourceFileType::DeleteFileCurrent()
{
	if ( m_currentItemStatus.m_nPluNo != 0 )
		CFileRemover FileRemover( GetFilePathCurrent() );
}

/**********************************************************************/

void CPluInfoResourceFileType::DeleteFileEdit001()
{
	if ( m_currentItemStatus.m_nPluNo != 0 )
		CFileRemover FileRemover( GetFilePathEdit001() );
}

/**********************************************************************/

void CPluInfoResourceFileType::DeleteFileEdit002()
{
	if ( m_currentItemStatus.m_nPluNo != 0 )
		CFileRemover FileRemover( GetFilePathEdit002() );
}

/**********************************************************************/

void CPluInfoResourceFileType::ConfirmEditChange()
{
	if ( m_currentItemStatus.m_nPluNo != 0 )
		::CopyFile( GetFilePathEdit001(), GetFilePathCurrent(), FALSE );
}

/**********************************************************************/

int CPluInfoResourceFileType::GetItemStatusBeforeEditing()
{
	int nStatus = -1;
	switch( m_nType )
	{
	case PLUINFO_TYPE_TEXT:			nStatus = m_currentItemStatus.m_nStatusText;		break;
	case PLUINFO_TYPE_IMAGE:		nStatus = m_currentItemStatus.m_nStatusImage;		break;
	case PLUINFO_TYPE_HTML:			nStatus = m_currentItemStatus.m_nStatusHTML;		break;
	case PLUINFO_TYPE_KPIMAGE:		nStatus = m_currentItemStatus.m_nStatusKPImage;		break;
	}
	return nStatus;
}

/**********************************************************************/

const char* CPluInfoResourceFileType::GetFilePathVisible()
{
	GetFilePathCurrent();
	GetFilePathEdit001();
	GetFilePathEdit002();

	if ( ::FileExists( m_strFilePathEdit002 ) == TRUE )
		m_strFilePathVisible = m_strFilePathEdit002;
	else if ( ::FileExists( m_strFilePathEdit001 ) == TRUE )
		m_strFilePathVisible = m_strFilePathEdit001;
	else if ( ( PLUINFO_STATUS_KEEP == GetItemStatusBeforeEditing() ) && ( FALSE == m_currentItemStatus.m_bOriginalItemDeleted ) )
		m_strFilePathVisible = m_strFilePathCurrent;
	else
		m_strFilePathVisible = m_strFilePathEdit002;

	return m_strFilePathVisible;
}

/**********************************************************************/

void CPluInfoResourceFileType::UpdateFileEdit002( const char* szFilename )
{
	m_Folders.CreateFolderPluInfo();
	m_Folders.CreateFolderKPImage();
	::CopyFile( szFilename, GetFilePathEdit002(), FALSE );
	SetSavedAsBlankFlag( FALSE );
}

/**********************************************************************/

bool CPluInfoResourceFileType::CheckExistingDataBeforeEditing()
{
	bool bResult = FALSE;

	switch( GetItemStatusBeforeEditing() )
	{
	case PLUINFO_STATUS_REPLACE:
		bResult = FileExistsEdit001();
		break;

	case PLUINFO_STATUS_KEEP:
		bResult = FileExistsCurrent() && ( FALSE == m_currentItemStatus.m_bOriginalItemDeleted );
		break;
	}

	return bResult;
}

/**********************************************************************/

void CPluInfoResourceFileType::SaveChanges( int& nChangeFlag )
{
	GetFilePathCurrent();
	GetFilePathEdit001();
	GetFilePathEdit002();

	if ( TRUE == m_bSavedAsBlank )
	{
		DeleteFileEdit001();

		if ( TRUE == m_bGotCurrentFile )
			nChangeFlag = PLUINFO_STATUS_DELETE;
		else
			nChangeFlag = PLUINFO_STATUS_KEEP;
	}
	else if ( FileExistsEdit002() )
	{
		//CREATING A FILE IF NONE EXISTED BEFORE IS A CHANGE
		if ( FALSE == m_bGotCurrentFile )
			nChangeFlag = PLUINFO_STATUS_REPLACE;

		//COMPARE CURRENT AND PREVIOUS FILES
		if ( PLUINFO_STATUS_KEEP == nChangeFlag )
		{
			if ( ::CompareFiles( m_strFilePathCurrent, m_strFilePathEdit002 ) == FALSE )
				nChangeFlag =  PLUINFO_STATUS_REPLACE;
		}
			
		CopyFile( m_strFilePathEdit002, m_strFilePathEdit001, FALSE );
	}
	
	CFileRemover FileRemover( m_strFilePathEdit002 );
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CPluInfoResourceWrapper::CPluInfoResourceWrapper() : 
	m_ResourceText		( PLUINFO_TYPE_TEXT,		m_currentItemStatus, m_Folders ), 
	m_ResourceImage		( PLUINFO_TYPE_IMAGE,		m_currentItemStatus, m_Folders ), 
	m_ResourceHTML		( PLUINFO_TYPE_HTML,		m_currentItemStatus, m_Folders ),
	m_ResourceKPImage	( PLUINFO_TYPE_KPIMAGE,		m_currentItemStatus, m_Folders )
{
	Reset();
}

/**********************************************************************/

void CPluInfoResourceWrapper::Reset()
{
	m_currentItemStatus.Reset();

	m_Folders.m_strFolderPluInfo =	dbDatabase.GetFolderPathDataPluInfo( DataManager.GetActiveDbIdx() );
	m_Folders.m_strFolderKPImage =	dbDatabase.GetFolderPathDataPluImage( DataManager.GetActiveDbIdx() );
	m_Folders.m_strFolderResourceCurrent = "";
	m_Folders.m_strFolderResourceEdit001 = "";
	m_Folders.m_strFolderResourceEdit002 = "";

	m_nDefaultTabType = -1;

	m_arrayItemStatus.RemoveAll();
	m_arrayDeletedFiles001.RemoveAll();
	m_arrayDeletedFiles002.RemoveAll();
	m_arrayResourceFiles002.RemoveAll();
}

/**********************************************************************/

void CPluInfoResourceWrapper::FlagDeletedPlu( __int64 nPluNo )
{
	SetCurrentPluNo( PLUINFO_MODE_MINIMAL, nPluNo );
	m_currentItemStatus.m_bOriginalItemDeleted = TRUE;

	int nPos;
	if ( m_arrayItemStatus.Find( m_currentItemStatus, nPos ) == TRUE )
		m_arrayItemStatus.SetAt( nPos, m_currentItemStatus );
	else
		m_arrayItemStatus.InsertAt( nPos, m_currentItemStatus );

	m_Folders.ClearFolderResourceEdit001();
	m_ResourceText.DeleteFileEdit001();
	m_ResourceImage.DeleteFileEdit001();
	m_ResourceHTML.DeleteFileEdit001();
	m_ResourceKPImage.DeleteFileEdit001();

	SetCurrentPluNo( PLUINFO_MODE_MINIMAL, 0 );

	ShowStatusForDebug();
}

/**********************************************************************/

void CPluInfoResourceWrapper::SetCurrentPluNo( int nMode, __int64 nPluNo )
{
	CString strSSPluNo;
	strSSPluNo.Format( "%I64d", nPluNo );
	
	{
		CString strTemp = "";
		strTemp += m_Folders.m_strFolderPluInfo;
		strTemp += "\\";
		strTemp += strSSPluNo;
		
		m_Folders.m_strFolderResourceCurrent = "";
		m_Folders.m_strFolderResourceCurrent += strTemp;
		m_Folders.m_strFolderResourceCurrent += "_files";

		m_Folders.m_strFolderResourceEdit001 = m_Folders.m_strFolderResourceCurrent + "\\Pending1";
		m_Folders.m_strFolderResourceEdit002 = m_Folders.m_strFolderResourceCurrent + "\\Pending2";
	}

	if ( nPluNo != m_currentItemStatus.m_nPluNo )
	{
		m_currentItemStatus.m_nPluNo = nPluNo;
	
		int nPos;
		if ( m_arrayItemStatus.Find( m_currentItemStatus, nPos ) == TRUE )
			m_arrayItemStatus.GetAt( nPos, m_currentItemStatus );
		else
		{
			m_currentItemStatus.Reset();
			m_currentItemStatus.m_nPluNo = nPluNo;
		}
		
		m_ResourceText.ResetForNewPluNo( nMode );
		m_ResourceImage.ResetForNewPluNo( nMode );
		m_ResourceHTML.ResetForNewPluNo( nMode );
		m_ResourceKPImage.ResetForNewPluNo( nMode );

		if ( PLUINFO_MODE_MINIMAL != nMode )
			InitialiseResourceFileList( nMode );

		if ( PLUINFO_MODE_EDIT == nMode )
		{
			m_Folders.ClearFolderResourceEdit002();
			
			//WORK OUT DEFAULT PLU INFO TAB FOR EDITING PLU RECORD
			m_nDefaultTabType = PLUINFO_TYPE_TEXT;

			if ( m_ResourceImage.CheckExistingDataBeforeEditing() == TRUE )
				m_nDefaultTabType = PLUINFO_TYPE_IMAGE;

			if ( m_ResourceHTML.CheckExistingDataBeforeEditing() == TRUE )
				m_nDefaultTabType = PLUINFO_TYPE_HTML;
		}
	}
}

/**********************************************************************/

int CPluInfoResourceWrapper::GetDefaultTabType()
{
	int nTabType = m_nDefaultTabType;
	m_nDefaultTabType = -1;
	return nTabType;
}

/**********************************************************************/

void CPluInfoResourceWrapper::InitialiseResourceFileList( int nMode )
{
	m_arrayResourceFiles002.RemoveAll();
	m_arrayDeletedFiles002.RemoveAll();

	if ( FALSE == m_currentItemStatus.m_bOriginalItemDeleted )
	{
		CFileFind finder;
		BOOL bWorking = finder.FindFile ( m_Folders.m_strFolderResourceCurrent + "\\*.*" );

		while ( bWorking == TRUE )
		{
			bWorking = finder.FindNextFile();

			if ( finder.IsDots() == TRUE )
				continue;

			if ( finder.IsDirectory() == TRUE )
				continue;

			CSortedIntAndStringItem item;
			item.m_nItem = 0;
			item.m_strItem = finder.GetFileName();
			m_arrayResourceFiles002.Consolidate( item );
		}
	}

	if ( PLUINFO_MODE_EDIT == nMode )
	{
		CFileFind finder;
		BOOL bWorking = finder.FindFile ( m_Folders.m_strFolderResourceEdit001 + "\\*.*" );

		while ( bWorking == TRUE )
		{
			bWorking = finder.FindNextFile();

			if ( finder.IsDots() == TRUE )
				continue;

			if ( finder.IsDirectory() == TRUE )
				continue;

			CSortedIntAndStringItem item;
			item.m_nItem = 1;
			item.m_strItem = finder.GetFileName();
			m_arrayResourceFiles002.Consolidate( item );

			int nPos;
			item.m_nItem = 0;
			if ( m_arrayResourceFiles002.Find( item, nPos ) == TRUE )
				m_arrayResourceFiles002.RemoveAt( nPos );
		}
	}
}

/**********************************************************************/

const char* CPluInfoResourceWrapper::GetResourceFileName( int nIndex )
{
	m_strFileNameResource = "";

	if ( ( nIndex >= 0 ) && ( nIndex < m_arrayResourceFiles002.GetSize() ) )
	{
		CSortedIntAndStringItem item;
		m_arrayResourceFiles002.GetAt( nIndex, item );
		m_strFileNameResource = item.m_strItem;
	}

	return m_strFileNameResource;
}

/**********************************************************************/

const char* CPluInfoResourceWrapper::GetResourceFilePath( int nIndex )
{
	m_strFilePathResource = "";

	if ( ( nIndex >= 0 ) && ( nIndex < m_arrayResourceFiles002.GetSize() ) )
	{
		CSortedIntAndStringItem item;
		m_arrayResourceFiles002.GetAt( nIndex, item );
		
		switch ( item.m_nItem )
		{
		case 0:		m_strFilePathResource = m_Folders.m_strFolderResourceCurrent;	break;
		case 1:		m_strFilePathResource = m_Folders.m_strFolderResourceEdit001;	break;
		case 2:	
		default:	m_strFilePathResource = m_Folders.m_strFolderResourceEdit002;	break;
		}
		
		m_strFilePathResource += "\\";
		m_strFilePathResource += item.m_strItem;
	}

	return m_strFilePathResource;
}

/**********************************************************************/

int CPluInfoResourceWrapper::AddResourceFile( const char* szFilePath, int nNameLen )
{
	m_Folders.CreateFolderResourceEdit002();
	
	CString strFilePath = szFilePath;
	CString strFileName = strFilePath.Right( nNameLen );

	CString strDest = "";
	strDest += m_Folders.m_strFolderResourceEdit002;
	strDest += "\\";
	strDest += strFileName;

	::CopyFile( strFilePath, strDest, FALSE );

	CSortedIntAndStringItem item;
	item.m_nItem = 2;
	item.m_strItem = strFileName;
	m_arrayResourceFiles002.Consolidate( item );

	int nPos;
	
	item.m_nItem = 0;
	if ( m_arrayResourceFiles002.Find( item, nPos ) == TRUE )
		m_arrayResourceFiles002.RemoveAt( nPos );
	
	item.m_nItem = 1;
	if ( m_arrayResourceFiles002.Find( item, nPos ) == TRUE )
		m_arrayResourceFiles002.RemoveAt( nPos );
	
	item.m_nItem = 2;
	m_arrayResourceFiles002.Find( item, nPos );
	return nPos;
}

/**********************************************************************/

void CPluInfoResourceWrapper::DeleteResourceFile( int nSel )
{
	if ( ( nSel >= 0 ) && ( nSel < m_arrayResourceFiles002.GetSize() ) )
	{
		CSortedIntAndStringItem item;
		m_arrayResourceFiles002.GetAt( nSel, item );

		if ( 2 != item.m_nItem )
			m_arrayDeletedFiles002.Consolidate( item );
		
		m_arrayResourceFiles002.RemoveAt( nSel );
	}
}

/**********************************************************************/

void CPluInfoResourceWrapper::UpdateFileEdit002( int nType, CEdit& edit )
{
	CString strFilePath = "";

	switch( nType )
	{
	case PLUINFO_TYPE_TEXT:	
	case PLUINFO_TYPE_HTML:
		strFilePath = GetFilePathEdit002( nType );
		break;
	}

	if ( strFilePath != "" )
	{
		m_Folders.CreateFolderPluInfo();
	
		CFileRemover FileRemover( strFilePath );

		CSSFile fileOut;
		bool bIsOpen = FALSE;
		
		CString strText;
		edit.GetWindowText( strText );

		int nLength = strText.GetLength();
			
		CString strLine = "";
		for ( int nPos = 0; nPos < nLength; nPos++ )
		{
			if ( nPos == nLength - 1 )
				strLine += strText.GetAt(nPos);
			else
			{
				if ( strText.Mid( nPos, 2 ) == "\r\n" )
				{
					if ( FALSE == bIsOpen )
						bIsOpen = fileOut.Open( strFilePath, "wb" );

					if ( TRUE == bIsOpen )
						fileOut.WriteLine( strLine );	
					
					strLine = "";
					nPos++;
				}
				else
					strLine += strText.GetAt(nPos);
			}
		}

		if ( strLine != "" )
		{
			if ( FALSE == bIsOpen )
				bIsOpen = fileOut.Open( strFilePath, "wb" );

			if ( TRUE == bIsOpen )
				fileOut.WriteLine( strLine );
		}

		SetSavedAsBlankFlag( nType, bIsOpen == FALSE );
	}
}

/**********************************************************************/

CPluInfoResourceFileType* CPluInfoResourceWrapper::GetPluInfoResourceFileType( int nType )
{
	switch( nType )
	{
	case PLUINFO_TYPE_TEXT:			return &m_ResourceText;
	case PLUINFO_TYPE_IMAGE:		return &m_ResourceImage;
	case PLUINFO_TYPE_HTML:			return &m_ResourceHTML;
	case PLUINFO_TYPE_KPIMAGE:		return &m_ResourceKPImage;
	default:						return NULL;
	}
}

/**********************************************************************/

bool CPluInfoResourceWrapper::SavedAsBlank( int nType )
{
	CPluInfoResourceFileType* pInfo = GetPluInfoResourceFileType(nType);
	return ( NULL == pInfo ) ? FALSE : pInfo -> SavedAsBlank();
}

/**********************************************************************/

const char* CPluInfoResourceWrapper::GetFilePathCurrent( int nType )
{
	CPluInfoResourceFileType* pInfo = GetPluInfoResourceFileType(nType);
	m_strFilePathCurrent = ( NULL == pInfo ) ? "" : pInfo -> GetFilePathCurrent();
	return m_strFilePathCurrent;
}

/**********************************************************************/

const char* CPluInfoResourceWrapper::GetFilePathEdit001( int nType )
{
	CPluInfoResourceFileType* pInfo = GetPluInfoResourceFileType(nType);
	m_strFilePathEdit001 = ( NULL == pInfo ) ? "" : pInfo -> GetFilePathEdit001();
	return m_strFilePathEdit001;
}

/**********************************************************************/

const char* CPluInfoResourceWrapper::GetFilePathEdit002( int nType )
{
	CPluInfoResourceFileType* pInfo = GetPluInfoResourceFileType(nType);
	m_strFilePathEdit002 = ( NULL == pInfo ) ? "" : pInfo -> GetFilePathEdit002();
	return m_strFilePathEdit002;
}

/**********************************************************************/

const char* CPluInfoResourceWrapper::GetFilePathVisible( int nType )
{
	CPluInfoResourceFileType* pInfo = GetPluInfoResourceFileType(nType);
	m_strFilePathVisible = ( NULL == pInfo ) ? "" : pInfo -> GetFilePathVisible();
	return m_strFilePathVisible;
}

/**********************************************************************/

void CPluInfoResourceWrapper::DeleteFileCurrent( int nType )
{ 
	CPluInfoResourceFileType* pInfo = GetPluInfoResourceFileType(nType);
	if ( pInfo != NULL ) pInfo -> DeleteFileCurrent();
}

/**********************************************************************/

void CPluInfoResourceWrapper::DeleteFileEdit001( int nType )
{ 
	CPluInfoResourceFileType* pInfo = GetPluInfoResourceFileType(nType);
	if ( pInfo != NULL ) pInfo -> DeleteFileEdit001();
}

/**********************************************************************/

void CPluInfoResourceWrapper::DeleteFileEdit002( int nType )
{ 
	CPluInfoResourceFileType* pInfo = GetPluInfoResourceFileType(nType);
	if ( pInfo != NULL ) pInfo -> DeleteFileEdit002();
}

/**********************************************************************/

void CPluInfoResourceWrapper::UpdateFileEdit002( int nType, const char* szFilename )
{ 
	CPluInfoResourceFileType* pInfo = GetPluInfoResourceFileType(nType);
	if ( pInfo != NULL ) pInfo -> UpdateFileEdit002( szFilename );
}

/**********************************************************************/

void CPluInfoResourceWrapper::SetSavedAsBlankFlag( int nType, bool b )
{ 
	CPluInfoResourceFileType* pInfo = GetPluInfoResourceFileType(nType);
	if ( pInfo != NULL ) pInfo -> SetSavedAsBlankFlag(b);
}

/**********************************************************************/

void CPluInfoResourceWrapper::SaveRecordChanges( bool& bChangeInfo, bool& bChangePlu )
{
	bChangeInfo = FALSE;
	bChangePlu = FALSE;

	if ( m_currentItemStatus.m_nPluNo != 0 )
	{
		m_ResourceText.SaveChanges( m_currentItemStatus.m_nStatusText );
		m_ResourceImage.SaveChanges( m_currentItemStatus.m_nStatusImage );
		m_ResourceHTML.SaveChanges( m_currentItemStatus.m_nStatusHTML );
		m_ResourceKPImage.SaveChanges( m_currentItemStatus.m_nStatusKPImage );
		
		//DELETED RESOURCE FILES
		for ( int n = 0; n < m_arrayDeletedFiles002.GetSize(); n++ )
		{
			CSortedIntAndStringItem item;
			m_arrayDeletedFiles002.GetAt( n, item );

			switch( item.m_nItem )
			{
			case 0:
				{
					CSortedInt64AndStringItem itemDel;
					itemDel.m_nItem = m_currentItemStatus.m_nPluNo;
					itemDel.m_strItem = item.m_strItem;
					m_arrayDeletedFiles001.Consolidate( itemDel );
				}
				break;

			case 1:
				{
					CString strPath = "";
					strPath += m_Folders.m_strFolderResourceEdit001;
					strPath += "\\";
					strPath += item.m_strItem;
					CFileRemover FileRemover( strPath );
				}
				break;
			}
		}

		//NEW RESOURCE FILES
		for ( int n = 0; n < m_arrayResourceFiles002.GetSize(); n++ )
		{
			CSortedIntAndStringItem item;
			m_arrayResourceFiles002.GetAt( n, item );

			if ( 2 == item.m_nItem )
			{
				m_Folders.CreateFolderResourceEdit001();

				CString strSource = "";
				strSource += m_Folders.m_strFolderResourceEdit002;
				strSource += "\\";
				strSource += item.m_strItem;

				CString strDest = "";
				strDest += m_Folders.m_strFolderResourceEdit001;
				strDest += "\\";
				strDest += item.m_strItem;
				
				::CopyFile( strSource, strDest, FALSE );

				m_currentItemStatus.m_bNewResourceFile = TRUE;
			}
		}
		
		//CURRENT ITEM STATUS
		{
			bool bCanSave = FALSE;
			bCanSave |= ( TRUE == m_currentItemStatus.m_bNewResourceFile );
			bCanSave |= ( TRUE == m_currentItemStatus.m_bOriginalItemDeleted );
			bCanSave |= ( m_currentItemStatus.m_nStatusText != PLUINFO_STATUS_KEEP );
			bCanSave |= ( m_currentItemStatus.m_nStatusImage != PLUINFO_STATUS_KEEP );
			bCanSave |= ( m_currentItemStatus.m_nStatusHTML != PLUINFO_STATUS_KEEP );
			bChangeInfo = bCanSave;

			bChangePlu = ( m_currentItemStatus.m_nStatusKPImage != PLUINFO_STATUS_KEEP );
			bCanSave |= bChangePlu;

			if ( TRUE == bCanSave )
			{
				int nPos;
				if ( m_arrayItemStatus.Find( m_currentItemStatus, nPos ) == TRUE )
					m_arrayItemStatus.SetAt( nPos, m_currentItemStatus );
				else
					m_arrayItemStatus.InsertAt( nPos, m_currentItemStatus );
			}
			else
			{
				int nPos;
				if ( m_arrayItemStatus.Find( m_currentItemStatus, nPos ) == TRUE )
					m_arrayItemStatus.RemoveAt( nPos );
			}
		}

		ShowStatusForDebug();

		m_Folders.ClearFolderResourceEdit002();
		SetCurrentPluNo( PLUINFO_MODE_MINIMAL, 0 );
	}
}

/**********************************************************************/

void CPluInfoResourceWrapper::SaveDatabaseChanges()
{
	__int64 nTarget = m_arrayDeletedFiles001.GetSize() + m_arrayItemStatus.GetSize();
	__int64 nProgress = 0;

	StatusProgress.Lock( TRUE, "Processing Plu Info Changes" );
	
	for ( int n = 0; n < m_arrayDeletedFiles001.GetSize(); n++ )
	{
		StatusProgress.SetPos( nProgress++, nTarget );

		CSortedInt64AndStringItem item;
		m_arrayDeletedFiles001.GetAt( n, item );
		SetCurrentPluNo( PLUINFO_MODE_EDIT, item.m_nItem );
		
		CString strPath = "";
		strPath += m_Folders.m_strFolderResourceCurrent;
		strPath += "\\";
		strPath += item.m_strItem;
		CFileRemover FileRemover( strPath );
	}

	for ( int n = 0; n < m_arrayItemStatus.GetSize(); n++ )
	{
		StatusProgress.SetPos( nProgress++, nTarget );

		CPluInfoItemStatus status;
		m_arrayItemStatus.GetAt( n, status );
		
		SetCurrentPluNo( PLUINFO_MODE_EDIT, status.m_nPluNo );

		if ( TRUE == status.m_bOriginalItemDeleted )
		{
			m_ResourceText.DeleteFileCurrent();
			m_ResourceImage.DeleteFileCurrent();
			m_ResourceHTML.DeleteFileCurrent();
			m_ResourceKPImage.DeleteFileCurrent();
			m_Folders.ClearFolderResourceCurrent();
		}

		switch( status.m_nStatusText )
		{
		case PLUINFO_STATUS_DELETE:
			m_ResourceText.DeleteFileCurrent();
			break;

		case PLUINFO_STATUS_REPLACE:
			m_ResourceText.ConfirmEditChange();
			break;
		}

		switch( status.m_nStatusImage )
		{
		case PLUINFO_STATUS_DELETE:
			m_ResourceImage.DeleteFileCurrent();
			break;

		case PLUINFO_STATUS_REPLACE:
			m_ResourceImage.ConfirmEditChange();
			break;
		}

		switch( status.m_nStatusHTML )
		{
		case PLUINFO_STATUS_DELETE:
			m_ResourceHTML.DeleteFileCurrent();
			break;

		case PLUINFO_STATUS_REPLACE:
			m_ResourceHTML.ConfirmEditChange();
			break;
		}

		switch( status.m_nStatusKPImage )
		{
		case PLUINFO_STATUS_DELETE:
			m_ResourceKPImage.DeleteFileCurrent();
			break;

		case PLUINFO_STATUS_REPLACE:
			m_ResourceKPImage.ConfirmEditChange();
			break;
		}

		{
			CFileFind finder;
			BOOL bWorking = finder.FindFile ( m_Folders.m_strFolderResourceEdit001 + "\\*.*" );

			while ( bWorking == TRUE )
			{
				bWorking = finder.FindNextFile();

				if ( finder.IsDots() == TRUE )
					continue;

				if ( finder.IsDirectory() == TRUE )
					continue;

				CString strSourceFolder = "";
				strSourceFolder += m_Folders.m_strFolderResourceEdit001;
				strSourceFolder += "\\";

				CString strDestFolder = "";
				strDestFolder += m_Folders.m_strFolderResourceCurrent;
				strDestFolder += "\\";

				CString strFileName = finder.GetFileName();
				::CopyFile( strSourceFolder + strFileName, strDestFolder + strFileName, FALSE );
			}
		}
	
		m_ResourceText.DeleteFileEdit001();
		m_ResourceText.DeleteFileEdit002();
		m_ResourceImage.DeleteFileEdit001();
		m_ResourceImage.DeleteFileEdit002();
		m_ResourceHTML.DeleteFileEdit001();
		m_ResourceHTML.DeleteFileEdit002();
		m_ResourceKPImage.DeleteFileEdit001();
		m_ResourceKPImage.DeleteFileEdit002();
		m_Folders.ClearFolderResourceEdit001();
		m_Folders.ClearFolderResourceEdit002();
	}

	StatusProgress.Unlock();
}

/**********************************************************************/

void CPluInfoResourceWrapper::UndoDatabaseChanges()
{
	__int64 nTarget = m_arrayItemStatus.GetSize();
	__int64 nProgress = 0;

	StatusProgress.Lock( TRUE, "Canceling Plu Info Changes" );

	for ( int n = 0; n < nTarget; n++ )
	{
		StatusProgress.SetPos( nProgress++, nTarget );

		CPluInfoItemStatus status;
		m_arrayItemStatus.GetAt( n, status );
		
		SetCurrentPluNo( PLUINFO_MODE_EDIT, status.m_nPluNo );

		m_ResourceText.DeleteFileEdit001();
		m_ResourceText.DeleteFileEdit002();
		m_ResourceImage.DeleteFileEdit001();
		m_ResourceImage.DeleteFileEdit002();
		m_ResourceHTML.DeleteFileEdit001();
		m_ResourceHTML.DeleteFileEdit002();
		m_ResourceKPImage.DeleteFileEdit001();
		m_ResourceKPImage.DeleteFileEdit002();
		m_Folders.ClearFolderResourceEdit001();
		m_Folders.ClearFolderResourceEdit002();
	}

	StatusProgress.Unlock();
}

/**********************************************************************/

void CPluInfoResourceWrapper::UndoRecordChanges()
{
	m_ResourceText.DeleteFileEdit002();
	m_ResourceImage.DeleteFileEdit002();
	m_ResourceHTML.DeleteFileEdit002();
	m_ResourceKPImage.DeleteFileEdit002();
	m_Folders.ClearFolderResourceEdit002();
}

/**********************************************************************/

void CPluInfoResourceWrapper::GetSourceFileInfoList( CArray<CPluInfoDownloadHelper>& arrayInfo, CArray<CPluInfoDownloadHelper>& arrayResource, bool bModifiable, int nModifiers )
{
	//INFO ARRAY IS CREATED FROM SEVERAL PLU RECORDS, SO DO NOT CLEAR 

	CPluInfoDownloadHelper item;
	item.m_nPluNo = m_currentItemStatus.m_nPluNo;
	item.m_bModifiable = bModifiable;
	item.m_nModifiers = nModifiers;

	if ( m_ResourceText.FileExistsCurrent() == TRUE )
	{
		item.m_nFileType = PLUINFO_TYPE_TEXT;
		arrayInfo.Add( item );
	}

	if ( m_ResourceImage.FileExistsCurrent() == TRUE )
	{
		item.m_nFileType = PLUINFO_TYPE_IMAGE;
		arrayInfo.Add( item );
	}

	if ( m_ResourceHTML.FileExistsCurrent() == TRUE )
	{
		item.m_nFileType = PLUINFO_TYPE_HTML;
		arrayInfo.Add( item );
	}

	item.m_nFileType = PLUINFO_TYPE_RESOURCE;
	for ( int n = 0; n < GetResourceFileCount(); n++ )
	{
		if ( n != 0 )
			item.m_strFilenames += ",";

		item.m_strFilenames += GetResourceFileName(n);
	}

	if ( item.m_strFilenames != "" )
		arrayResource.Add( item );
}

/**********************************************************************/

void CPluInfoResourceWrapper::GetDownloadFileList( CPluInfoDownloadHelper& item, CString& strSourceFile, CStringArray& arrayDestFiles )
{
	SetCurrentPluNo( PLUINFO_MODE_DOWNLOAD, item.m_nPluNo );

	strSourceFile = "";

	switch( item.m_nFileType )
	{
	case PLUINFO_TYPE_TEXT:
		strSourceFile = m_ResourceText.GetFileNameCurrent();
		break;

	case PLUINFO_TYPE_IMAGE:
		strSourceFile = m_ResourceImage.GetFileNameCurrent();
		break;

	case PLUINFO_TYPE_HTML:
		strSourceFile = m_ResourceHTML.GetFileNameCurrent();
		break;
	}

	arrayDestFiles.RemoveAll();

	switch( item.m_nFileType )
	{
	case PLUINFO_TYPE_TEXT:
		m_ResourceText.GetFileNameDestArray( item, arrayDestFiles );
		break;

	case PLUINFO_TYPE_IMAGE:
		m_ResourceImage.GetFileNameDestArray( item, arrayDestFiles );
		break;

	case PLUINFO_TYPE_HTML:
		m_ResourceHTML.GetFileNameDestArray( item, arrayDestFiles );
		break;
	}
}

/**********************************************************************/

void CPluInfoResourceWrapper::GetDownloadFolderList( CPluInfoDownloadHelper& item, CString& strSourceFolder, CStringArray& arrayDestFolders )
{
	strSourceFolder = "";
	arrayDestFolders.RemoveAll();

	if ( PLUINFO_TYPE_RESOURCE == item.m_nFileType )
	{
		SetCurrentPluNo( PLUINFO_MODE_DOWNLOAD, item.m_nPluNo );
		strSourceFolder = m_Folders.m_strFolderResourceCurrent + "\\";
		m_ResourceHTML.GetFolderDestArray( item, arrayDestFolders );
	}
}

/**********************************************************************/

void CPluInfoResourceWrapper::ShowStatusForDebug()
{
#ifdef _DEBUG
	
	CString strTrace = "\n\nSave Current Item Status\n";
	TRACE(strTrace);
	
	for ( int n = 0; n < m_arrayItemStatus.GetSize(); n++ )
	{
		CPluInfoItemStatus status;
		m_arrayItemStatus.GetAt( n, status );
		
		CString str;
		str.Format ( "%I64d   %d %d %d %d   %d %d\n",
			status.m_nPluNo,
			status.m_nStatusText,
			status.m_nStatusImage,
			status.m_nStatusHTML,
			status.m_nStatusKPImage,
			status.m_bNewResourceFile ? 1 : 0,
			status.m_bOriginalItemDeleted ? 1 : 0 );
		
		TRACE(str);
	}

#endif
}

/**********************************************************************/
