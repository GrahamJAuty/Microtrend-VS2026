/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "FileRemover.h"
#include "SysInfo.h"
#include "StatusProgress.h"
/**********************************************************************/
#include "ListKeyImageWrapperDept.h"
/**********************************************************************/

CListKeyImageStatusDept::CListKeyImageStatusDept()
{
	Reset();
}

/**********************************************************************/

void CListKeyImageStatusDept::Reset()
{
	m_nDeptNo = 0;
	m_nStatusKeyImage = DEPTINFO_STATUS_KEEP;
	m_strFileNameKeyOld = "";
	m_strFileNameKeyNew = "";
	m_bOriginalItemDeleted = FALSE;
}

/**********************************************************************/

void CListKeyImageStatusDept::Add( CListKeyImageStatusDept& source )
{
}

/**********************************************************************/

int CListKeyImageStatusDept::Compare( CListKeyImageStatusDept& source, int nHint )
{
	if ( m_nDeptNo != source.m_nDeptNo )
		return ( m_nDeptNo > source.m_nDeptNo ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CListKeyImageFoldersDept::CListKeyImageFoldersDept()
{
	m_strFolderKeyImage = "";
}

/**********************************************************************/

void CListKeyImageFoldersDept::CreateFolderKeyImage()
{
	if ( ::ExistSubdirectory( m_strFolderKeyImage ) == FALSE )
		::CreateSubdirectory( m_strFolderKeyImage );
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CListKeyImageWorkingDept::CListKeyImageWorkingDept( CListKeyImageStatusDept& status, CListKeyImageFoldersDept& Folders ) : m_currentItemStatus( status ), m_Folders( Folders )
{
	m_strDeptNo = "";
	m_bSavedAsBlank = FALSE;
	m_bGotCurrentFile = FALSE;
	m_strFileNameKeyOld = "";
	m_strFileNameKeyNew = "";
	m_strSuffixCurrent = "";
	m_strSuffixEdit001 = ".xx1";
	m_strSuffixEdit002 = ".xx2";
}

/**********************************************************************/

void CListKeyImageWorkingDept::ResetForNewDeptNo( int nMode, const char* szFileNameKeyOld, const char* szFileNameKeyNew )
{
	m_strFileNameKeyOld = szFileNameKeyOld;
	m_strFileNameKeyNew = szFileNameKeyNew;
	m_strDeptNo.Format( "D%4.4d", m_currentItemStatus.m_nDeptNo );
	
	m_bGotCurrentFile = FileExistsCurrent();
	m_bSavedAsBlank = FALSE;
	
	if ( DEPTINFO_MODE_EDIT == nMode )
	{
		DeleteFileEdit002();

		if ( GetItemStatusBeforeEditing() != DEPTINFO_STATUS_REPLACE )
			DeleteFileEdit001();
	}
}

/**********************************************************************/

const char* CListKeyImageWorkingDept::GetEditFilenamePrefix()
{
	if ( EcrmanOptions.GetGlobalDepartmentFlag() == FALSE )
		m_strEditFilenamePrefix.Format( "%3.3d_", dbDatabase.GetDbNo( DataManager.GetActiveDbIdx() ) );
	else
		m_strEditFilenamePrefix = "";

	return m_strEditFilenamePrefix;
}

/**********************************************************************/

const char* CListKeyImageWorkingDept::GetFileNameCurrent()
{
	return m_strFileNameKeyOld;
}

/**********************************************************************/

const char* CListKeyImageWorkingDept::GetFilePathCurrent()
{
	m_strFilePathCurrent = m_Folders.m_strFolderKeyImage;
	m_strFilePathCurrent += "\\";
	m_strFilePathCurrent += GetFileNameCurrent();
	return m_strFilePathCurrent;
}

/**********************************************************************/

bool CListKeyImageWorkingDept::FileExistsCurrent()
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

const char* CListKeyImageWorkingDept::GetFilePathReplace( const char* szReplace )
{
	m_strFilePathReplace = m_Folders.m_strFolderKeyImage;
	m_strFilePathReplace += "\\";
	m_strFilePathReplace += szReplace;
	return m_strFilePathReplace;
}

/**********************************************************************/

const char* CListKeyImageWorkingDept::GetFileNameEdit001()
{
	m_strFileNameEdit001 = GetEditFilenamePrefix();
	m_strFileNameEdit001 += m_strDeptNo;
	m_strFileNameEdit001 += m_strSuffixEdit001;
	return m_strFileNameEdit001;
}

/**********************************************************************/

const char* CListKeyImageWorkingDept::GetFilePathEdit001()
{
	m_strFilePathEdit001 = m_Folders.m_strFolderKeyImage;
	m_strFilePathEdit001 += "\\";
	m_strFilePathEdit001 += GetFileNameEdit001();
	return m_strFilePathEdit001;
}

/**********************************************************************/

const char* CListKeyImageWorkingDept::GetFileNameEdit002()
{
	m_strFileNameEdit002 = GetEditFilenamePrefix();
	m_strFileNameEdit002 += m_strDeptNo;
	m_strFileNameEdit002 += m_strSuffixEdit002;
	return m_strFileNameEdit002;
}

/**********************************************************************/

const char* CListKeyImageWorkingDept::GetFilePathEdit002()
{
	m_strFilePathEdit002 = m_Folders.m_strFolderKeyImage;
	m_strFilePathEdit002 += "\\";
	m_strFilePathEdit002 += GetFileNameEdit002();
	return m_strFilePathEdit002;
}

/**********************************************************************/

void CListKeyImageWorkingDept::DeleteFileCurrent()
{
	if ( m_currentItemStatus.m_nDeptNo != 0 )
		CFileRemover FileRemover( GetFilePathCurrent() );
}

/**********************************************************************/

void CListKeyImageWorkingDept::DeleteFileEdit001()
{
	if ( m_currentItemStatus.m_nDeptNo != 0 )
		CFileRemover FileRemover( GetFilePathEdit001() );
}

/**********************************************************************/

void CListKeyImageWorkingDept::DeleteFileEdit002()
{
	if ( m_currentItemStatus.m_nDeptNo != 0 )
		CFileRemover FileRemover( GetFilePathEdit002() );
}

/**********************************************************************/

void CListKeyImageWorkingDept::ConfirmEditChange( const char* szReplace )
{
	if ( m_currentItemStatus.m_nDeptNo != 0 )
		::CopyFile( GetFilePathEdit001(), GetFilePathReplace( szReplace ), FALSE );
}

/**********************************************************************/

int CListKeyImageWorkingDept::GetItemStatusBeforeEditing()
{
	return m_currentItemStatus.m_nStatusKeyImage;
}

/**********************************************************************/

const char* CListKeyImageWorkingDept::GetFilePathVisible()
{
	GetFilePathCurrent();
	GetFilePathEdit001();
	GetFilePathEdit002();

	if ( ::FileExists( m_strFilePathEdit002 ) == TRUE )
		m_strFilePathVisible = m_strFilePathEdit002;
	else if ( ::FileExists( m_strFilePathEdit001 ) == TRUE )
		m_strFilePathVisible = m_strFilePathEdit001;
	else if ( ( DEPTINFO_STATUS_KEEP == GetItemStatusBeforeEditing() ) && ( FALSE == m_currentItemStatus.m_bOriginalItemDeleted ) )
		m_strFilePathVisible = m_strFilePathCurrent;
	else
		m_strFilePathVisible = m_strFilePathEdit002;

	return m_strFilePathVisible;
}

/**********************************************************************/

void CListKeyImageWorkingDept::UpdateFileEdit002( const char* szFilename, int nFileLen )
{
	CString strFilename = szFilename;
	m_strFileNameKeyNew = strFilename.Right( nFileLen );
	m_Folders.CreateFolderKeyImage();
	::CopyFile( strFilename, GetFilePathEdit002(), FALSE );
	SetSavedAsBlankFlag( FALSE );
}

/**********************************************************************/

void CListKeyImageWorkingDept::SaveChanges( int& nChangeFlag )
{
	GetFilePathCurrent();
	GetFilePathEdit001();
	GetFilePathEdit002();

	if ( TRUE == m_bSavedAsBlank )
	{
		DeleteFileEdit001();

		if ( TRUE == m_bGotCurrentFile )
			nChangeFlag = DEPTINFO_STATUS_DELETE;
		else
			nChangeFlag = DEPTINFO_STATUS_KEEP;
	}
	else if ( FileExistsEdit002() )
	{
		nChangeFlag = DEPTINFO_STATUS_REPLACE;
		CopyFile( m_strFilePathEdit002, m_strFilePathEdit001, FALSE );
	}
	
	CFileRemover FileRemover( m_strFilePathEdit002 );
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CListKeyImageWrapperDept::CListKeyImageWrapperDept() : 
	m_WorkingDept( m_currentItemStatus, m_Folders )
{
}

/**********************************************************************/

void CListKeyImageWrapperDept::Reset()
{
	m_currentItemStatus.Reset();
	m_Folders.m_strFolderKeyImage = "Images";
	GetDataProgramPath( m_Folders.m_strFolderKeyImage );
	m_arrayItemStatus.RemoveAll();
}

/**********************************************************************/

void CListKeyImageWrapperDept::FlagDeletedDept( int nDeptNo )
{
	SetCurrentDeptNo( DEPTINFO_MODE_MINIMAL, nDeptNo );
	m_currentItemStatus.m_bOriginalItemDeleted = TRUE;

	int nPos;
	if ( m_arrayItemStatus.Find( m_currentItemStatus, nPos ) == TRUE )
		m_arrayItemStatus.SetAt( nPos, m_currentItemStatus );
	else
		m_arrayItemStatus.InsertAt( nPos, m_currentItemStatus );

	m_WorkingDept.DeleteFileEdit001();

	SetCurrentDeptNo( DEPTINFO_MODE_MINIMAL, 0 );

	ShowStatusForDebug();
}

/**********************************************************************/

void CListKeyImageWrapperDept::SetCurrentDeptNo( int nMode, int nDeptNo, const char* szFileNameKeyCurrent )
{
	if ( nDeptNo != m_currentItemStatus.m_nDeptNo )
	{
		m_currentItemStatus.m_nDeptNo = nDeptNo;
		
		int nPos;
		if ( m_arrayItemStatus.Find( m_currentItemStatus, nPos ) == TRUE )
		{
			m_arrayItemStatus.GetAt( nPos, m_currentItemStatus );
		}
		else
		{
			m_currentItemStatus.Reset();
			m_currentItemStatus.m_nDeptNo = nDeptNo;
			m_currentItemStatus.m_strFileNameKeyOld = szFileNameKeyCurrent;
		}
		
		m_WorkingDept.ResetForNewDeptNo( nMode, m_currentItemStatus.m_strFileNameKeyOld, m_currentItemStatus.m_strFileNameKeyNew );
	}
}

/**********************************************************************/

bool CListKeyImageWrapperDept::SavedAsBlank()
{
	return m_WorkingDept.SavedAsBlank();
}

/**********************************************************************/

const char* CListKeyImageWrapperDept::GetFilePathCurrent()
{
	m_strFilePathCurrent = m_WorkingDept.GetFilePathCurrent();
	return m_strFilePathCurrent;
}

/**********************************************************************/

const char* CListKeyImageWrapperDept::GetFilePathEdit001()
{
	m_strFilePathEdit001 = m_WorkingDept.GetFilePathEdit001();
	return m_strFilePathEdit001;
}

/**********************************************************************/

const char* CListKeyImageWrapperDept::GetFilePathEdit002()
{
	m_strFilePathEdit002 = m_WorkingDept.GetFilePathEdit002();
	return m_strFilePathEdit002;
}

/**********************************************************************/

const char* CListKeyImageWrapperDept::GetFilePathVisible()
{
	m_strFilePathVisible = m_WorkingDept.GetFilePathVisible();
	return m_strFilePathVisible;
}

/**********************************************************************/

void CListKeyImageWrapperDept::DeleteFileCurrent()
{ 
	m_WorkingDept.DeleteFileCurrent();
}

/**********************************************************************/

void CListKeyImageWrapperDept::DeleteFileEdit001()
{ 
	m_WorkingDept.DeleteFileEdit001();
}

/**********************************************************************/

void CListKeyImageWrapperDept::DeleteFileEdit002()
{ 
	m_WorkingDept.DeleteFileEdit002();
}

/**********************************************************************/

void CListKeyImageWrapperDept::UpdateFileEdit002( const char* szFilename, int nFileLen )
{ 
	m_WorkingDept.UpdateFileEdit002( szFilename, nFileLen );
}

/**********************************************************************/

void CListKeyImageWrapperDept::SetSavedAsBlankFlag( bool b )
{ 
	m_WorkingDept.SetSavedAsBlankFlag(b);
}

/**********************************************************************/

void CListKeyImageWrapperDept::SaveRecordChanges()
{
	if ( m_currentItemStatus.m_nDeptNo != 0 )
	{
		m_WorkingDept.SaveChanges( m_currentItemStatus.m_nStatusKeyImage );
		
		//CURRENT ITEM STATUS
		{
			bool bCanSave = FALSE;
			bCanSave |= ( TRUE == m_currentItemStatus.m_bOriginalItemDeleted );
			bCanSave |= ( m_currentItemStatus.m_nStatusKeyImage != DEPTINFO_STATUS_KEEP );
			
			if ( TRUE == bCanSave )
			{
				if ( m_currentItemStatus.m_nStatusKeyImage != DEPTINFO_STATUS_KEEP )
					m_currentItemStatus.m_strFileNameKeyNew = m_WorkingDept.GetFileNameKeyNew();

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
		SetCurrentDeptNo( DEPTINFO_MODE_MINIMAL, 0 );
	}
}

/**********************************************************************/

void CListKeyImageWrapperDept::SaveDatabaseChanges()
{
	__int64 nTarget = m_arrayItemStatus.GetSize();
	__int64 nProgress = 0;

	StatusProgress.Lock( TRUE, "Processing Dept Info Changes" );
	
	for ( int n = 0; n < m_arrayItemStatus.GetSize(); n++ )
	{
		StatusProgress.SetPos( nProgress++, nTarget );

		CListKeyImageStatusDept status;
		m_arrayItemStatus.GetAt( n, status );
		
		SetCurrentDeptNo( DEPTINFO_MODE_EDIT, status.m_nDeptNo );

		switch( status.m_nStatusKeyImage )
		{
		//DON'T DELETE CURRENT FILE, AS MAYBE REQUIRED BY OTHER ITEM
		case DEPTINFO_STATUS_REPLACE:
			m_WorkingDept.ConfirmEditChange( status.m_strFileNameKeyNew );
			break;
		}
	
		m_WorkingDept.DeleteFileEdit001();
		m_WorkingDept.DeleteFileEdit002();
	}

	StatusProgress.Unlock();
}

/**********************************************************************/

void CListKeyImageWrapperDept::UndoDatabaseChanges()
{
	__int64 nTarget = m_arrayItemStatus.GetSize();
	__int64 nProgress = 0;

	StatusProgress.Lock( TRUE, "Canceling Dept Info Changes" );

	for ( int n = 0; n < nTarget; n++ )
	{
		StatusProgress.SetPos( nProgress++, nTarget );

		CListKeyImageStatusDept status;
		m_arrayItemStatus.GetAt( n, status );
		
		SetCurrentDeptNo( DEPTINFO_MODE_EDIT, status.m_nDeptNo );

		m_WorkingDept.DeleteFileEdit001();
		m_WorkingDept.DeleteFileEdit002();
	}

	StatusProgress.Unlock();
}

/**********************************************************************/

void CListKeyImageWrapperDept::UndoRecordChanges()
{
	m_WorkingDept.DeleteFileEdit002();
}

/**********************************************************************/

void CListKeyImageWrapperDept::ShowStatusForDebug()
{
#ifdef _DEBUG

	TRACE ( "\n\nSave Current Item Status\n" );
	
	for ( int n = 0; n < m_arrayItemStatus.GetSize(); n++ )
	{
		CListKeyImageStatusDept status;
		m_arrayItemStatus.GetAt( n, status );
		
		CString str;
		str.Format ( "%d %d %d OLD:%s NEW:%s\n",
			status.m_nDeptNo,
			status.m_nStatusKeyImage,
			status.m_bOriginalItemDeleted ? 1 : 0,
			(const char*) status.m_strFileNameKeyOld,
			(const char*) status.m_strFileNameKeyNew );
		TRACE ( str );
	}
#endif
}

/**********************************************************************/
