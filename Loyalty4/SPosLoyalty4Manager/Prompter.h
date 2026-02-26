#pragma once
/**********************************************************************/

class CPrompter  
{
public:
	CPrompter();
	void SetPreventParentEnableFlag( bool b ){ m_bPreventParentEnable = b; }

public:
	int Warning ( CStringArray& array, bool bYesNoCancel = FALSE );
	int OkCancel ( const char* szPrompt, const char* szTitle = "" );
	int YesNo ( const char* szPrompt, const char* szTitle = "", bool bYes = FALSE );
	int YesNoCancel ( const char* szPrompt, const char* szTitle = "", bool bYes = FALSE );
	void Info ( const char* szInfo, const char* szTitle = "" );
	void Diagnostic ( const char* szInfo );
	void Error ( const char* szError, const char* szTitle = "" );
	void FatalError ( const char* szError, const char* szTitle = "" );
	void ContactDealer( const char* szError, bool bFatal = FALSE );
	void LockError( const char* szType );
	int DisplayMessage( const char* szMsg, const char* szTitle, UINT nType );
	void ShareError( const char* szFile, int nError );

public:
	void DatabaseAccessError() { Error("Unable to access database.\n\nPlease try again later."); }
	void DatabaseUpdateError(const char* szReason);
	
private:
	int WarningMessageBox( CStringArray& array, bool bYesNoCancel );
	void OKMessageBox( const char* szTitle, const char* szMsg, int nIconType );
	
private:
	CString m_strError;
	CString m_strNodeString;
	bool m_bPreventParentEnable;
};

/**********************************************************************/
extern CPrompter Prompter;
/**********************************************************************/
