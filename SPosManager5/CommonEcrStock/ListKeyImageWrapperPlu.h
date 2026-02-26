#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#define PLUINFO_STATUS_KEEP 0
#define PLUINFO_STATUS_DELETE 1
#define PLUINFO_STATUS_REPLACE 2
/**********************************************************************/
#define PLUINFO_MODE_EDIT 0
#define PLUINFO_MODE_DOWNLOAD 1
#define PLUINFO_MODE_MINIMAL 2
/**********************************************************************/
#include "DepartmentCSVArray.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/

struct CListKeyImageStatusPlu
{
public:
	CListKeyImageStatusPlu();
	void Reset();
	
public:
	int Compare ( CListKeyImageStatusPlu& source, int nHint = 0 );
	void Add	( CListKeyImageStatusPlu& source );

public:
	__int64 m_nPluNo;
	int m_nStatusKeyImage;
	CString m_strFileNameKeyOld;
	CString m_strFileNameKeyNew;
	bool m_bOriginalItemDeleted;
};

/**********************************************************************/

struct CListKeyImageFoldersPlu
{
public:
	CListKeyImageFoldersPlu();

public:
	void CreateFolderKeyImage();
		
public:
	CString m_strFolderKeyImage;
};

/**********************************************************************/

class CListKeyImageWorkingPlu
{
public:
	CListKeyImageWorkingPlu( CListKeyImageStatusPlu& status, CListKeyImageFoldersPlu& Folders );
	void ResetForNewPluNo( int nMode, const char* szFileNameKeyOld, const char* szFileNameKeyNew );
	
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
	CListKeyImageFoldersPlu& m_Folders;
	CListKeyImageStatusPlu& m_currentItemStatus;

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
	CString m_strPluNo;
};

/**********************************************************************/

class CListKeyImageWrapperPlu
{
public:
	CListKeyImageWrapperPlu();
	void Reset();

public:
	void SetCurrentPluNo( int nMode, __int64 nPluNo, const char* szFilenameKeyCurrent = "" );
	void FlagDeletedPlu( __int64 nPluNo );
	
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
	const char* GetNewKeyImageFileName(){ return m_WorkingPlu.GetFileNameKeyNew(); } 

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
	CListKeyImageFoldersPlu m_Folders;
	CListKeyImageWorkingPlu m_WorkingPlu;
	
private:
	CReportConsolidationArray<CListKeyImageStatusPlu> m_arrayItemStatus;
	
private:
	CListKeyImageStatusPlu m_currentItemStatus;
	
private:
	CString m_strFilePathCurrent;
	CString m_strFilePathEdit001;
	CString m_strFilePathEdit002;
	CString m_strFilePathVisible;
};

/**********************************************************************/
#endif
/**********************************************************************/