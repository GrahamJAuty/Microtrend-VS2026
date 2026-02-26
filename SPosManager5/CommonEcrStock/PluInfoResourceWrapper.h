#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#define PLUINFO_TYPE_TEXT 0
#define PLUINFO_TYPE_IMAGE 1
#define PLUINFO_TYPE_HTML 2
#define PLUINFO_TYPE_RESOURCE 3
#define PLUINFO_TYPE_KPIMAGE 4
/**********************************************************************/
#define PLUINFO_STATUS_KEEP 0
#define PLUINFO_STATUS_DELETE 1
#define PLUINFO_STATUS_REPLACE 2
/**********************************************************************/
#define PLUINFO_MODE_EDIT 0
#define PLUINFO_MODE_DOWNLOAD 1
#define PLUINFO_MODE_MINIMAL 2
/**********************************************************************/
#include "PluCSVArray.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/

struct CPluInfoItemStatus
{
public:
	CPluInfoItemStatus();
	void Reset();
	
public:
	int Compare ( CPluInfoItemStatus& source, int nHint = 0 );
	void Add	( CPluInfoItemStatus& source );

public:
	__int64 m_nPluNo;
	int m_nStatusText;
	int m_nStatusImage;
	int m_nStatusHTML;
	int m_nStatusKPImage;
	bool m_bNewResourceFile;
	bool m_bOriginalItemDeleted;
};

/**********************************************************************/

struct CPluInfoDownloadHelper
{
public:
	CPluInfoDownloadHelper();

public:
	__int64 m_nPluNo;
	int m_nFileType;
	CString m_strFilenames;
	bool m_bModifiable;
	int m_nModifiers;
};

/**********************************************************************/

struct CPluInfoResourceFolders
{
public:
	CPluInfoResourceFolders();

public:
	void CreateFolderPluInfo();
	void CreateFolderKPImage();
	void CreateFolderResourceCurrent();
	void CreateFolderResourceEdit001();
	void CreateFolderResourceEdit002();

public:
	void ClearFolderResourceCurrent();
	void ClearFolderResourceEdit001();
	void ClearFolderResourceEdit002();

private:
	void CreateFolder( const char* szFolder );
	void ClearFolder( const char* szFolder );
	
public:
	CString m_strFolderPluInfo;
	CString m_strFolderKPImage;
	CString m_strFolderResourceCurrent;
	CString m_strFolderResourceEdit001;
	CString m_strFolderResourceEdit002;
};

/**********************************************************************/

class CPluInfoResourceFileType
{
public:
	CPluInfoResourceFileType( int nType, CPluInfoItemStatus& status, CPluInfoResourceFolders& Folders );
	void ResetForNewPluNo( int nMode );
	
public:
	bool SavedAsBlank(){ return m_bSavedAsBlank; }
	bool CheckExistingDataBeforeEditing();
	bool FileExistsCurrent() { return ::FileExists( GetFilePathCurrent() ); }
	bool FileExistsEdit001() { return ::FileExists( GetFilePathEdit001() ); }
	bool FileExistsEdit002() { return ::FileExists( GetFilePathEdit002() ); }

private:
	int GetItemStatusBeforeEditing();

public:
	void DeleteFileCurrent();
	void DeleteFileEdit001();
	void DeleteFileEdit002();

public:
	void UpdateFileEdit002( const char* szFilename );
	void SetSavedAsBlankFlag( bool b ){ m_bSavedAsBlank = b; }
	void SaveChanges( int& nChangeFlag );
	void ConfirmEditChange();

public:
	void GetFileNameDestArray( CPluInfoDownloadHelper& item, CStringArray& arrayDest );
	void GetFolderDestArray( CPluInfoDownloadHelper& item, CStringArray& arrayDest );

public:
	const char* GetFileNameCurrent();
	const char* GetFileNameEdit001();
	const char* GetFileNameEdit002();

public:
	const char* GetFilePathCurrent();
	const char* GetFilePathEdit001();
	const char* GetFilePathEdit002();
	const char* GetFilePathVisible();

private:
	void GetModifierStringArray( CPluInfoDownloadHelper& item, CStringArray& arrayMod );
	const char* GetWorkingFolder();
	
private:
	CPluInfoResourceFolders& m_Folders;
	CPluInfoItemStatus &m_currentItemStatus;

private:
	CString m_strSuffixCurrent;
	CString m_strSuffixEdit001;
	CString m_strSuffixEdit002;
	bool m_bSavedAsBlank;
	bool m_bGotCurrentFile;

private:
	CString m_strWorkingFolder;
	CString m_strFileNameDest;
	CString m_strFileNameCurrent;
	CString m_strFileNameEdit001;
	CString m_strFileNameEdit002;
	CString m_strFilePathCurrent;
	CString m_strFilePathEdit001;
	CString m_strFilePathEdit002;
	CString m_strFilePathVisible;
	
private:
	int m_nType;	
	CString m_strPluNo;
};

/**********************************************************************/

class CPluInfoResourceWrapper
{
public:
	CPluInfoResourceWrapper();
	void Reset();

public:
	void SetCurrentPluNo( int nMode, __int64 nPluNo );
	void FlagDeletedPlu( __int64 nPluNo );
	
public:
	void SaveRecordChanges( bool& bChangeInfo, bool& bChangePlu );
	void UndoRecordChanges();
	
public:
	int GetDefaultTabType();
	
public:
	bool SavedAsBlank( int nType );
	const char* GetFilePathCurrent( int nType );
	const char* GetFilePathEdit001( int nType );
	const char* GetFilePathEdit002( int nType );
	const char* GetFilePathVisible( int nType );

public:
	void DeleteFileCurrent( int nType );
	void DeleteFileEdit001( int nType );
	void DeleteFileEdit002( int nType );

public:
	void UpdateFileEdit002( int nType, const char* szFilename );
	void UpdateFileEdit002( int nType, CEdit& edit );
	void SetSavedAsBlankFlag( int nType, bool b );
	
public:
	const char* GetFolderPathPluInfo(){ return m_Folders.m_strFolderPluInfo; }
	
public:
	void InitialiseResourceFileList( int nMode );
	int GetResourceFileCount(){ return m_arrayResourceFiles002.GetSize(); }
	const char* GetResourceFileName( int nIndex );
	const char* GetResourceFilePath( int nIndex );
	int AddResourceFile( const char* szFullPath, int nNameLen );
	void DeleteResourceFile( int nSel );
			
public:
	bool IsModified(){ return ( m_arrayItemStatus.GetSize() != 0 ); }
	void SaveDatabaseChanges();
	void UndoDatabaseChanges();
	
public:
	void GetSourceFileInfoList( CArray<CPluInfoDownloadHelper>& arrayInfo, CArray<CPluInfoDownloadHelper>& arrayResource, bool bModifiable, int nModifiers );
	void GetDownloadFileList( CPluInfoDownloadHelper& item, CString& strSourceFile, CStringArray& strDestFiles );
	void GetDownloadFolderList( CPluInfoDownloadHelper& item, CString& strSourceFolder, CStringArray& strDestFolders );

private:
	void ShowStatusForDebug();
	CPluInfoResourceFileType* GetPluInfoResourceFileType( int nType );

private:
	CPluInfoResourceFolders m_Folders;
	CPluInfoResourceFileType m_ResourceText;
	CPluInfoResourceFileType m_ResourceImage;
	CPluInfoResourceFileType m_ResourceHTML;
	CPluInfoResourceFileType m_ResourceKPImage;
	int m_nDefaultTabType;

private:
	CReportConsolidationArray<CPluInfoItemStatus> m_arrayItemStatus;
	CReportConsolidationArray<CSortedInt64AndStringItem> m_arrayDeletedFiles001;
	
private:
	CReportConsolidationArray<CSortedIntAndStringItem> m_arrayResourceFiles002;
	CReportConsolidationArray<CSortedIntAndStringItem> m_arrayDeletedFiles002;
	CPluInfoItemStatus m_currentItemStatus;
	bool m_bPluInfoModifierTick;

private:
	CString m_strFilePathCurrent;
	CString m_strFilePathEdit001;
	CString m_strFilePathEdit002;
	CString m_strFilePathVisible;
	CString m_strFileNameResource;
	CString m_strFilePathResource;
};

/**********************************************************************/
#endif
/**********************************************************************/