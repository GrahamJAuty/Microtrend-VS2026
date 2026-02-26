#pragma once
/**********************************************************************/
#include "MyMessageBoxDlg.h"
/**********************************************************************/

class CPrompter  
{
public:
	CPrompter();
	void SetPreventParentEnableFlag( bool b ){ m_bPreventParentEnable = b; }

public:
	int Warning ( CStringArray& array, bool bYesNoCancel = FALSE );
	int OkCancel ( const char* szPrompt );
	int YesNo ( const char* szPrompt, bool bYes = FALSE );
	int YesNoCancel ( const char* szPrompt, bool bYes = TRUE);
	void Info ( const char* szInfo );
	void Diagnostic ( const char* szInfo );
	void Error ( const char* szError, const char* szTitle = "");
	void FatalError ( const char* szError );
	void ContactDealer( const char* szError, bool bFatal = FALSE );
	void ShareError ( CDataManagerInfo& info );
	void WriteError ( CDataManagerInfo& info );
	int SuggestReadOnly( CDataManagerInfo& info );
	void LockError( const char* szType );
	void InContextCommsError();
	void FatalReadError( SysFileInfo info );
	void SupplierError();
	void CustomFieldError();

	void LocationAccessError();
	void StockpointAccessError();

	void ShowStartupMessage(CMyMessageBoxDlg** ppMessageBox, CWnd* pParent);

private:
	int WarningMessageBox( CStringArray& array, bool bYesNoCancel );
	void OKMessageBox( const char* szTitle, const char* szMsg, int nIconType );
	
private:
	const char* GetNodeString( int nNodeType );
	const char* GetErrorMessage( CDataManagerInfo& info );

private:
	CString m_strError;
	CString m_strNodeString;
	bool m_bPreventParentEnable;
};

/**********************************************************************/
extern CPrompter Prompter;
/**********************************************************************/
