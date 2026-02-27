#pragma once
/**********************************************************************/

//******************************************************************
#ifndef SYSTEMTYPE_MANAGER
//******************************************************************

class CPrompter  
{
public:
	CPrompter(){};

public:
	void Error ( const char* szError, const char* szTitle = "" ){}
	int YesNo ( const char* szPrompt, const char* szTitle = "", bool bDefaultYes = FALSE, bool bBackgroundYes = FALSE ){ return bBackgroundYes ? IDYES : IDNO; }
	void DatabaseUpdateError(const char* szReason){}
};

//******************************************************************
#else
//******************************************************************

class CPrompter  
{
public:
	CPrompter();
	void SetPreventParentEnableFlag( bool b ){ m_bPreventParentEnable = b; }

public:
	int Warning ( CStringArray& array, bool bYesNoCancel = FALSE );
	int OkCancel ( const char* szPrompt, const char* szTitle = "" );
	int YesNo ( const char* szPrompt, const char* szTitle = "", bool bDefaultYes = FALSE, bool bBackgroundYes = FALSE );
	int YesNoCancel ( const char* szPrompt, const char* szTitle = "", bool bYes = FALSE );
	void Info ( const char* szInfo, const char* szTitle = "" );
	void Diagnostic ( const char* szInfo );
	void Error ( const char* szError, const char* szTitle = "" );
	void FatalError ( const char* szError, const char* szTitle = "" );
	void ContactDealer( const char* szError, bool bFatal = FALSE );
	void LockError( const char* szType );
	int DisplayMessage( const char* szMsg, const char* szTitle, UINT nType );
	void ShareError( const char* szFile, int nError );
	void NotImplemented(int n);

public:
	void DatabaseUpdateError(const char* szReason);
	void DatabaseAccessError() { Error("Unable to access database.\n\nPlease try again later."); }

private:
	int WarningMessageBox( CStringArray& array, bool bYesNoCancel );
	void OKMessageBox( const char* szTitle, const char* szMsg, int nIconType );
	
private:
	CString m_strError;
	CString m_strNodeString;
	bool m_bPreventParentEnable;
};

//******************************************************************
#endif
//******************************************************************

/**********************************************************************/
extern CPrompter Prompter;
/**********************************************************************/
