/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "FileRemover.h"
#include "SysInfo.h"
#include "StatusProgress.h"
/**********************************************************************/
#include "ListKeyImageWrapperPlu.h"
/**********************************************************************/

CListKeyImageStatusPlu::CListKeyImageStatusPlu()
{
	Reset();
}

/**********************************************************************/

void CListKeyImageStatusPlu::Reset()
{
	m_nPluNo = 0;
	m_nStatusKeyImage = PLUINFO_STATUS_KEEP;
	m_strFileNameKeyOld = "";
	m_strFileNameKeyNew = "";
	m_bOriginalItemDeleted = FALSE;
}

/**********************************************************************/

void CListKeyImageStatusPlu::Add( CListKeyImageStatusPlu& source )
{
}

/**********************************************************************/

int CListKeyImageStatusPlu::Compare( CListKeyImageStatusPlu& source, int nHint )
{
	if ( m_nPluNo != source.m_nPluNo )
		return ( m_nPluNo > source.m_nPluNo ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CListKeyImageFoldersPlu::CListKeyImageFoldersPlu()
{
	m_strFolderKeyImage = "";
}

/**********************************************************************/

void CListKeyImageFoldersPlu::CreateFolderKeyImage()
{
	if ( ::ExistSubdirectory( m_strFolderKeyImage ) == FALSE )
		::CreateSubdirectory( m_strFolderKeyImage );
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CListKeyImageWorkingPlu::CListKeyImageWorkingPlu( CListKeyImageStatusPlu& status, CListKeyImageFoldersPlu& Folders ) : m_currentItemStatus( status ), m_Folders( Folders )
{
	m_strPluNo = "";
	m_bSavedAsBlank = FALSE;
	m_bGotCurrentFile = FALSE;
	m_strFileNameKeyOld = "";
	m_strFileNameKeyNew = "";
	m_strSuffixCurrent = "";
	m_strSuffixEdit001 = ".xx1";
	m_strSuffixEdit002 = ".xx2";
}

/**********************************************************************/

void CListKeyImageWorkingPlu::ResetForNewPluNo( int nMode, const char* szFileNameKeyOld, const char* szFileNameKeyNew )
{
	m_strFileNameKeyOld = szFileNameKeyOld;
	m_strFileNameKeyNew = szFileNameKeyNew;
	m_strPluNo.Format( "P%14.14I64d", m_currentItemStatus.m_nPluNo );
	
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

const char* CListKeyImageWorkingPlu::GetEditFilenamePrefix()
{
	m_strEditFilenamePrefix.Format( "%3.3d_", dbDatabase.GetDbNo( DataManager.GetActiveDbIdx() ) );
	return m_strEditFilenamePrefix;
}

/**********************************************************************/

const char* CListKeyImageWorkingPlu::GetFileNameCurrent()
{
	return m_strFileNameKeyOld;
}

/**********************************************************************/

const char* CListKeyImageWorkingPlu::GetFilePathCurrent()
{
	m_strFilePathCurrent = m_Folders.m_strFolderKeyImage;
	m_strFilePathCurrent += "\\";
	m_strFilePathCurrent += GetFileNameCurrent();
	return m_strFilePathCurrent;
}

/**********************************************************************/

bool CListKeyImageWorkingPlu::FileExistsCurrent()
{
	bool bResult = FALSE;
	
	CString strFileName = GetFileNameCurrent();
	
	if ( strFileName != "" )
	{
		bResult = ::FileExists( GetFilePathCurrent() );
	}

	return bResult;
}

/**********************************************************************/

const char* CListKeyImageWorkingPlu::GetFilePathReplace( const char* szReplace )
{
	m_strFilePathReplace = m_Folders.m_strFolderKeyImage;
	m_strFilePathReplace += "\\";
	m_strFilePathReplace += szReplace;
	return m_strFilePathReplace;
}

/**********************************************************************/

const char* CListKeyImageWorkingPlu::GetFileNameEdit001()
{
	m_strFileNameEdit001 = GetEditFilenamePrefix();
	m_strFileNameEdit001 += m_strPluNo;
	m_strFileNameEdit001 += m_strSuffixEdit001;
	return m_strFileNameEdit001;
}

/**********************************************************************/

const char* CListKeyImageWorkingPlu::GetFilePathEdit001()
{
	m_strFilePathEdit001 = m_Folders.m_strFolderKeyImage;
	m_strFilePathEdit001 += "\\";
	m_strFilePathEdit001 += GetFileNameEdit001();
	return m_strFilePathEdit001;
}

/**********************************************************************/

const char* CListKeyImageWorkingPlu::GetFileNameEdit002()
{
	m_strFileNameEdit002 = GetEditFilenamePrefix();
	m_strFileNameEdit002 += m_strPluNo;
	m_strFileNameEdit002 += m_strSuffixEdit002;
	return m_strFileNameEdit002;
}

/**********************************************************************/

const char* CListKeyImageWorkingPlu::GetFilePathEdit002()
{
	m_strFilePathEdit002 = m_Folders.m_strFolderKeyImage;
	m_strFilePathEdit002 += "\\";
	m_strFilePathEdit002 += GetFileNameEdit002();
	return m_strFilePathEdit002;
}

/**********************************************************************/

void CListKeyImageWorkingPlu::DeleteFileCurrent()
{
	if ( m_currentItemStatus.m_nPluNo != 0 )
		CFileRemover FileRemover( GetFilePathCurrent() );
}

/**********************************************************************/

void CListKeyImageWorkingPlu::DeleteFileEdit001()
{
	if ( m_currentItemStatus.m_nPluNo != 0 )
		CFileRemover FileRemover( GetFilePathEdit001() );
}

/**********************************************************************/

void CListKeyImageWorkingPlu::DeleteFileEdit002()
{
	if ( m_currentItemStatus.m_nPluNo != 0 )
		CFileRemover FileRemover( GetFilePathEdit002() );
}

/**********************************************************************/

void CListKeyImageWorkingPlu::ConfirmEditChange( const char* szReplace )
{
	if ( m_currentItemStatus.m_nPluNo != 0 )
		::CopyFile( GetFilePathEdit001(), GetFilePathReplace( szReplace ), FALSE );
}

/**********************************************************************/

int CListKeyImageWorkingPlu::GetItemStatusBeforeEditing()
{
	return m_currentItemStatus.m_nStatusKeyImage;
}

/**********************************************************************/

const char* CListKeyImageWorkingPlu::GetFilePathVisible()
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

void CListKeyImageWorkingPlu::UpdateFileEdit002( const char* szFilename, int nFileLen )
{
	CString strFilename = szFilename;
	m_strFileNameKeyNew = strFilename.Right( nFileLen );
	m_Folders.CreateFolderKeyImage();
	::CopyFile( strFilename, GetFilePathEdit002(), FALSE );
	SetSavedAsBlankFlag( FALSE );
}

/**********************************************************************/

void CListKeyImageWorkingPlu::SaveChanges( int& nChangeFlag )
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
		nChangeFlag = PLUINFO_STATUS_REPLACE;
		CopyFile( m_strFilePathEdit002, m_strFilePathEdit001, FALSE );
	}
	
	CFileRemover FileRemover( m_strFilePathEdit002 );
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CListKeyImageWrapperPlu::CListKeyImageWrapperPlu() : 
	m_WorkingPlu( m_currentItemStatus, m_Folders )
{
}

/**********************************************************************/

void CListKeyImageWrapperPlu::Reset()
{
	m_currentItemStatus.Reset();
	m_Folders.m_strFolderKeyImage = "Images";
	GetDataProgramPath( m_Folders.m_strFolderKeyImage );
	m_arrayItemStatus.RemoveAll();
}

/**********************************************************************/

void CListKeyImageWrapperPlu::FlagDeletedPlu( __int64 nPluNo )
{
	SetCurrentPluNo( PLUINFO_MODE_MINIMAL, nPluNo );
	m_currentItemStatus.m_bOriginalItemDeleted = TRUE;

	int nPos;
	if ( m_arrayItemStatus.Find( m_currentItemStatus, nPos ) == TRUE )
		m_arrayItemStatus.SetAt( nPos, m_currentItemStatus );
	else
		m_arrayItemStatus.InsertAt( nPos, m_currentItemStatus );

	m_WorkingPlu.DeleteFileEdit001();

	SetCurrentPluNo( PLUINFO_MODE_MINIMAL, 0 );

	ShowStatusForDebug();
}

/**********************************************************************/

void CListKeyImageWrapperPlu::SetCurrentPluNo( int nMode, __int64 nPluNo, const char* szFileNameKeyCurrent )
{
	if ( nPluNo != m_currentItemStatus.m_nPluNo )
	{
		m_currentItemStatus.m_nPluNo = nPluNo;
		
		int nPos;
		if ( m_arrayItemStatus.Find( m_currentItemStatus, nPos ) == TRUE )
		{
			m_arrayItemStatus.GetAt( nPos, m_currentItemStatus );
		}
		else
		{
			m_currentItemStatus.Reset();
			m_currentItemStatus.m_nPluNo = nPluNo;
			m_currentItemStatus.m_strFileNameKeyOld = szFileNameKeyCurrent;
		}
		
		m_WorkingPlu.ResetForNewPluNo( nMode, m_currentItemStatus.m_strFileNameKeyOld, m_currentItemStatus.m_strFileNameKeyNew );
	}
}

/**********************************************************************/

bool CListKeyImageWrapperPlu::SavedAsBlank()
{
	return m_WorkingPlu.SavedAsBlank();
}

/**********************************************************************/

const char* CListKeyImageWrapperPlu::GetFilePathCurrent()
{
	m_strFilePathCurrent = m_WorkingPlu.GetFilePathCurrent();
	return m_strFilePathCurrent;
}

/**********************************************************************/

const char* CListKeyImageWrapperPlu::GetFilePathEdit001()
{
	m_strFilePathEdit001 = m_WorkingPlu.GetFilePathEdit001();
	return m_strFilePathEdit001;
}

/**********************************************************************/

const char* CListKeyImageWrapperPlu::GetFilePathEdit002()
{
	m_strFilePathEdit002 = m_WorkingPlu.GetFilePathEdit002();
	return m_strFilePathEdit002;
}

/**********************************************************************/

const char* CListKeyImageWrapperPlu::GetFilePathVisible()
{
	m_strFilePathVisible = m_WorkingPlu.GetFilePathVisible();
	return m_strFilePathVisible;
}

/**********************************************************************/

void CListKeyImageWrapperPlu::DeleteFileCurrent()
{ 
	m_WorkingPlu.DeleteFileCurrent();
}

/**********************************************************************/

void CListKeyImageWrapperPlu::DeleteFileEdit001()
{ 
	m_WorkingPlu.DeleteFileEdit001();
}

/**********************************************************************/

void CListKeyImageWrapperPlu::DeleteFileEdit002()
{ 
	m_WorkingPlu.DeleteFileEdit002();
}

/**********************************************************************/

void CListKeyImageWrapperPlu::UpdateFileEdit002( const char* szFilename, int nFileLen )
{ 
	m_WorkingPlu.UpdateFileEdit002( szFilename, nFileLen );
}

/**********************************************************************/

void CListKeyImageWrapperPlu::SetSavedAsBlankFlag( bool b )
{ 
	m_WorkingPlu.SetSavedAsBlankFlag(b);
}

/**********************************************************************/

void CListKeyImageWrapperPlu::SaveRecordChanges()
{
	if ( m_currentItemStatus.m_nPluNo != 0 )
	{
		m_WorkingPlu.SaveChanges( m_currentItemStatus.m_nStatusKeyImage );
		
		//CURRENT ITEM STATUS
		{
			bool bCanSave = FALSE;
			bCanSave |= ( TRUE == m_currentItemStatus.m_bOriginalItemDeleted );
			bCanSave |= ( m_currentItemStatus.m_nStatusKeyImage != PLUINFO_STATUS_KEEP );
			
			if ( TRUE == bCanSave )
			{
				if ( m_currentItemStatus.m_nStatusKeyImage != PLUINFO_STATUS_KEEP )
					m_currentItemStatus.m_strFileNameKeyNew = m_WorkingPlu.GetFileNameKeyNew();

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
		SetCurrentPluNo( PLUINFO_MODE_MINIMAL, 0 );
	}
}

/**********************************************************************/

void CListKeyImageWrapperPlu::SaveDatabaseChanges()
{
	__int64 nTarget = m_arrayItemStatus.GetSize();
	__int64 nProgress = 0;

	StatusProgress.Lock( TRUE, "Processing Plu Info Changes" );
	
	for ( int n = 0; n < m_arrayItemStatus.GetSize(); n++ )
	{
		StatusProgress.SetPos( nProgress++, nTarget );

		CListKeyImageStatusPlu status;
		m_arrayItemStatus.GetAt( n, status );
		
		SetCurrentPluNo( PLUINFO_MODE_EDIT, status.m_nPluNo );

		switch( status.m_nStatusKeyImage )
		{
		//DON'T DELETE CURRENT FILE, AS MAYBE REQUIRED BY OTHER ITEM
		case PLUINFO_STATUS_REPLACE:
			m_WorkingPlu.ConfirmEditChange( status.m_strFileNameKeyNew );
			break;
		}
	
		m_WorkingPlu.DeleteFileEdit001();
		m_WorkingPlu.DeleteFileEdit002();
	}

	StatusProgress.Unlock();
}

/**********************************************************************/

void CListKeyImageWrapperPlu::UndoDatabaseChanges()
{
	__int64 nTarget = m_arrayItemStatus.GetSize();
	__int64 nProgress = 0;

	StatusProgress.Lock( TRUE, "Canceling Plu Info Changes" );

	for ( int n = 0; n < nTarget; n++ )
	{
		StatusProgress.SetPos( nProgress++, nTarget );

		CListKeyImageStatusPlu status;
		m_arrayItemStatus.GetAt( n, status );
		
		SetCurrentPluNo( PLUINFO_MODE_EDIT, status.m_nPluNo );

		m_WorkingPlu.DeleteFileEdit001();
		m_WorkingPlu.DeleteFileEdit002();
	}

	StatusProgress.Unlock();
}

/**********************************************************************/

void CListKeyImageWrapperPlu::UndoRecordChanges()
{
	m_WorkingPlu.DeleteFileEdit002();
}

/**********************************************************************/

void CListKeyImageWrapperPlu::ShowStatusForDebug()
{
#ifdef _DEBUG
	/*
	int nCount = 0;
	CString strTrace = "\n\nSave Current Item Status\n";

	for ( int n = 0; n < m_arrayItemStatus.GetSize(); n++ )
	{
		CListKeyImageStatusPlu status;
		m_arrayItemStatus.GetAt( n, status );
		
		CString str;
		str.Format ( "%I64d %d %d OLD:%s NEW:%s\n",
			status.m_nPluNo,
			status.m_nStatusKeyImage,
			status.m_bOriginalItemDeleted ? 1 : 0,
			(const char*) status.m_strFileNameKeyOld,
			(const char*) status.m_strFileNameKeyNew );
		
		strTrace += str;
		
		nCount++;
		if ( nCount >= 20 )
		{
			TRACE(strTrace);
			strTrace = "";
			nCount == 0;
		}
	}

	if ( nCount > 0 )
		TRACE(strTrace);
	*/
#endif
}

/**********************************************************************/
