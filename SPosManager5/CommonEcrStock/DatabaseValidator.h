#pragma once
/**********************************************************************/
#include "ProgressInfoDlg.h"
/**********************************************************************/

class CDatabaseValidator  
{
public:
	CDatabaseValidator();

public:
	bool Run( bool bFullValidation, CWnd* pParentWnd );
	
public:
	bool GetBusyFlag(){ return m_bBusy; }
	bool GetErrorFlag(){ return m_bError; }
	bool GetDoingUpdateFlag(){ return m_bDoingUpdate; }
	void SetBusyFlag( bool b ){ m_bBusy = b; }
	void SetErrorFlag( bool b ){ m_bError = b; }
	void SetDoingUpdateFlag( bool b ){ m_bDoingUpdate = b; }

public:
	void WriteQuitToken();
	void DeleteQuitToken();

public:
	void LogError( int nDbNo, int nSpNo, int nFileType, int nError, int nIndex, const char* szFilename );
	void ShowErrorMessage();
	bool ValidAtStartup() { return m_bValidAtStartup; }
	bool CancelAtStartup() { return m_bCancelAtStartup; }

private:
	bool m_bValidAtStartup;
	bool m_bCancelAtStartup;
	bool m_bBusy;
	bool m_bDoingUpdate;
	bool m_bError;
	CString m_strError;
};

/**********************************************************************/
extern CDatabaseValidator DatabaseValidator;
/**********************************************************************/
