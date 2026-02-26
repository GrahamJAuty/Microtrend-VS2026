#pragma once
/**********************************************************************/
 
/**********************************************************************/
#define EXPORT_ERROR_NONE 0
/**********************************************************************/
#define EXPORT_ERROR_FILENAME 1
#define EXPORT_ERROR_INFOFILE_FIND 2
#define EXPORT_ERROR_INFOFILE_READ 3
#define EXPORT_ERROR_INFOFILE_TARGETFOLDER 4
#define EXPORT_ERROR_INFOFILE_TASKFILENAME 5
#define EXPORT_ERROR_COPYFILE1 6
#define EXPORT_ERROR_COPYFILE2 7
#define EXPORT_ERROR_STATUS_SOURCE 8
#define EXPORT_ERROR_STATUS_DEST 9
#define EXPORT_ERROR_TARGET_FOLDER 10
/**********************************************************************/
#ifdef POSTRAY_UTILITY
/**********************************************************************/

class CPosTrayExportSender  
{
public:
	CPosTrayExportSender();

public:
	void LookForReports();
	
private:
	void ExportReports( const char* szSourceFolder, const char* szParent, bool bRoot );
	int ExportReport( CString& strSourceFolder, CString& strSourceFilename, CString& strTargetFolder, CString& strTargetFilename, CString& strErrorText );
	
private:
	void LogExportJobSystem( const char* szType, const char* szTargetFolder, const char* szTargetFilename, const char* szResult );
	const char* GetExportErrorText( int nError );
	
private:
	CString m_strExportErrorText;
	CString m_strChangeExt;
	CString m_strSlash;
};

/**********************************************************************/
#endif
/**********************************************************************/
