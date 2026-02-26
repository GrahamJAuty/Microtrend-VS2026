#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#define DEPTINFO_STATUS_KEEP 0
#define DEPTINFO_STATUS_DELETE 1
#define DEPTINFO_STATUS_REPLACE 2
/**********************************************************************/
#define DEPTINFO_MODE_EDIT 0
#define DEPTINFO_MODE_DOWNLOAD 1
#define DEPTINFO_MODE_MINIMAL 2
/**********************************************************************/
#include "DepartmentCSVArray.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/

struct CListKeyImageStatusDept
{
public:
	CListKeyImageStatusDept();
	void Reset();
	
public:
	int Compare ( CListKeyImageStatusDept& source, int nHint = 0 );
	void Add	( CListKeyImageStatusDept& source );

public:
	int m_nDeptNo;
	int m_nStatusKeyImage;
	CString m_strFileNameKeyOld;
	CString m_strFileNameKeyNew;
	bool m_bOriginalItemDeleted;
};

/**********************************************************************/

struct CListKeyImageFoldersDept
{
public:
	CListKeyImageFoldersDept();

public:
	void CreateFolderKeyImage();
		
public:
	CString m_strFolderKeyImage;
};

/**********************************************************************/

class CListKeyImageWorkingDept
{
public:
	CListKeyImageWorkingDept( CListKeyImageStatusDept& status, CListKeyImageFoldersDept& Folders );
	void ResetForNewDeptNo( int nMode, const char* szFileNameKeyOld, const char* szFileNameKeyNew );
	
public:
	bool SavedAsBlank(){ return m_bSavedAsBlank; }
	bool FileExistsCurrent();
	bool FileExistsEdit001() { return ::FileExists( GetFilePathEdit001() ); }
	bool FileExistsEdit002() { return ::FileExists( GetFilePathEdit002() ); }

private:
	int GetItemStatusBeforeEditing();

public:
	void DeleteFileCurrent();
	void DeleteFileEdit001();
	void DeleteFileEdit002();

public:
	void UpdateFileEdit002( const char* szFilename, int nFileLen = 0 );
	void SetSavedAsBlankFlag( bool b ){ m_bSavedAsBlank = b; }
	void SaveChanges( int& nChangeFlag );
	void ConfirmEditChange( const char* szReplace = "" );

public:
	const char* GetEditFilenamePrefix();
	const char* GetFileNameCurrent();
	const char* GetFileNameEdit001();
	const char* GetFileNameEdit002();
	const char* GetFileNameKeyNew(){ return m_strFileNameKeyNew; }

public:
	const char* GetFilePathCurrent();
	const char* GetFilePathReplace( const char* szReplace = "" );
	const char* GetFilePathEdit001();
	const char* GetFilePathEdit002();
	const char* GetFilePathVisible();
	
private:
	CListKeyImageFoldersDept& m_Folders;
	CListKeyImageStatusDept& m_currentItemStatus;

private:
	CString m_strSuffixCurrent;
	CString m_strSuffixEdit001;
	CString m_strSuffixEdit002;
	bool m_bSavedAsBlank;
	bool m_bGotCurrentFile;

private:
	CString m_strEditFilenamePrefix;
	CString m_strFileNameDest;
	CString m_strFileNameEdit001;
	CString m_strFileNameEdit002;
	CString m_strFilePathCurrent;
	CString m_strFilePathReplace;
	CString m_strFilePathEdit001;
	CString m_strFilePathEdit002;
	CString m_strFilePathVisible;

private:
	CString m_strFileNameKeyOld;
	CString m_strFileNameKeyNew;
	
private:
	CString m_strDeptNo;
};

/**********************************************************************/

class CListKeyImageWrapperDept
{
public:
	CListKeyImageWrapperDept();
	void Reset();

public:
	void SetCurrentDeptNo( int nMode, int nDeptNo, const char* szFilenameKeyCurrent = "" );
	void FlagDeletedDept( int nDeptNo );
	
public:
	void SaveRecordChanges();
	void UndoRecordChanges();
	
public:
	int GetDefaultTabType();
	
public:
	bool SavedAsBlank();
	const char* GetFilePathCurrent();
	const char* GetFilePathEdit001();
	const char* GetFilePathEdit002();
	const char* GetFilePathVisible();
	const char* GetNewKeyImageFileName(){ return m_WorkingDept.GetFileNameKeyNew(); } 

public:
	void DeleteFileCurrent();
	void DeleteFileEdit001();
	void DeleteFileEdit002();

public:
	void UpdateFileEdit002( const char* szFilename, int nFileLen = 0 );
	void SetSavedAsBlankFlag( bool b );
					
public:
	bool IsModified(){ return ( m_arrayItemStatus.GetSize() != 0 ); }
	void SaveDatabaseChanges();
	void UndoDatabaseChanges();
	
private:
	void ShowStatusForDebug();
	
private:
	CListKeyImageFoldersDept m_Folders;
	CListKeyImageWorkingDept m_WorkingDept;
	
private:
	CReportConsolidationArray<CListKeyImageStatusDept> m_arrayItemStatus;
	
private:
	CListKeyImageStatusDept m_currentItemStatus;
	
private:
	CString m_strFilePathCurrent;
	CString m_strFilePathEdit001;
	CString m_strFilePathEdit002;
	CString m_strFilePathVisible;
};

/**********************************************************************/
#endif
/**********************************************************************/