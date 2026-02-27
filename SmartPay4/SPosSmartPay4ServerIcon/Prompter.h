#pragma once
/**********************************************************************/

class CPrompter  
{
public:
	CPrompter();
	void SetPreventParentEnableFlag( bool b ){ m_bPreventParentEnable = b; }

public:
	int YesNo ( const char* szPrompt, const char* szTitle = "", bool bYes = FALSE );
	void Info ( const char* szInfo, const char* szTitle = "" );
	void Error ( const char* szError, const char* szTitle = "" );
	void FatalError ( const char* szError, const char* szTitle = "" );
	int DisplayMessage( const char* szMsg, const char* szTitle, UINT nType );
	
private:
	void OKMessageBox( const char* szTitle, const char* szMsg, int nIconType );
	
private:
	bool m_bPreventParentEnable;
};

/**********************************************************************/
extern CPrompter Prompter;
/**********************************************************************/
