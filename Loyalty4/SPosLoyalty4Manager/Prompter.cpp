/**********************************************************************/
#include "..\SPosLoyalty4Shared\PresentationOptions.h"
/**********************************************************************/
#include "MyMessageBoxDlg.h"
/**********************************************************************/
#include "Prompter.h"
/**********************************************************************/

CPrompter::CPrompter()
{
	m_bPreventParentEnable = FALSE;
}

/**********************************************************************/
/**********************************************************************/

int CPrompter::DisplayMessage( const char* szMsg, const char* szTitle, UINT nType )
{
	int nResult = IDYES;

	switch( nType )
	{
	case MB_ICONEXCLAMATION:	
		Error( szMsg, szTitle );		
		break;
	
	case MB_ICONSTOP:			
		FatalError( szMsg, szTitle );	
		break;
	
	case MB_YESNO:
		nResult = YesNo( szMsg, szTitle );
		break;

	case MB_ICONINFORMATION:	
	default:					
		Info( szMsg, szTitle );			
		break;
	}

	return nResult;
}

/**********************************************************************/

void CPrompter::DatabaseUpdateError(const char* szReason)
{
	CString strMsg = "";
	strMsg += "Unable to save changes to database\n\n";
	strMsg += szReason;
	Error(strMsg);
}

/**********************************************************************/

int CPrompter::WarningMessageBox( CStringArray& array, bool bYesNoCancel )
{
	CWnd* pParent = CWnd::FromHandle( CWnd::GetSafeOwner_( NULL, NULL ) );

	BOOL bParentEnabled = FALSE;
	if ( pParent != NULL )
		bParentEnabled = pParent -> IsWindowEnabled();
	
	CString strMsg = "";
	strMsg += array.GetAt(1);
	strMsg += "\n\n";

	int n = 1;
	for ( int i = 3 ; i < array.GetSize() ; i++ )
	{
		CString strTmp;

		if ( array.GetAt(i) == "" )
			strTmp = "\n";
		else if ( array.GetAt(i).GetAt(0) != '+' )
			strTmp.Format ( "%d )   %s\n", n++, array.GetAt(i) );
		else
			strTmp.Format ( "        %s\n", array.GetAt(i).Mid(1) );

		strMsg += strTmp;
	}

	strMsg += "\n";
	strMsg += array.GetAt(2);

	int nResult;
	if ( NULL == pParent )
	{
		if ( bYesNoCancel == FALSE )
			nResult = AfxMessageBox ( strMsg, MB_ICONEXCLAMATION | MB_YESNO | MB_DEFBUTTON2 );
		else
			nResult = AfxMessageBox ( strMsg, MB_ICONEXCLAMATION | MB_YESNOCANCEL | MB_DEFBUTTON2 );
	}
	else if ( PresentationOptions.GetCustomMessageBoxFlag() == FALSE  )
	{
		if ( bYesNoCancel == FALSE )
			nResult = pParent->MessageBox ( strMsg, array.GetAt(0), MB_ICONEXCLAMATION | MB_YESNO | MB_DEFBUTTON2 );
		else
			nResult = pParent->MessageBox ( strMsg, array.GetAt(0), MB_ICONEXCLAMATION | MB_YESNOCANCEL | MB_DEFBUTTON2 );
	}
	else
	{
		CMyMessageBoxInfo infoMessage;
		infoMessage.SetIconType( MB_ICONEXCLAMATION );
		infoMessage.SetButtonMode( bYesNoCancel ? MB_YESNOCANCEL : MB_YESNO );
		infoMessage.SetLineGap( PresentationOptions.GetCustomMessageBoxLineGap() );
		infoMessage.SetFontSize( PresentationOptions.GetCustomMessageBoxFont() );
		infoMessage.SetBackColour( PresentationOptions.GetCustomMessageBoxColourBackInt() );
		infoMessage.SetTextColour( PresentationOptions.GetCustomMessageBoxColourTextInt() );
		infoMessage.SetDefButton(2);
		infoMessage.SetTitle( array.GetAt(0) );
		infoMessage.SetTexts( strMsg );

		CMyMessageBoxDlg dlg( infoMessage, pParent );
		nResult = dlg.DoModal();
	}

	if ( ( FALSE == bParentEnabled ) && ( TRUE == m_bPreventParentEnable ) && ( pParent != NULL ) )
		pParent -> EnableWindow( FALSE );
	
	return nResult;
}

/**********************************************************************/

int CPrompter::Warning( CStringArray& array, bool bYesNoCancel )
{
	array.InsertAt( 1, "On selecting to continue the following irreversible actions take place" );
	array.InsertAt( 2, "Are you sure you wish to continue ?" );
	
	int nResult = WarningMessageBox( array, bYesNoCancel );
	
	array.RemoveAt( 1 );
	array.RemoveAt( 1 );
	
	return nResult;
}

/**********************************************************************/
/**********************************************************************/

void CPrompter::OKMessageBox( const char* szTitle, const char* szMsg, int nIconType )
{
	CWnd* pParent = CWnd::FromHandle( CWnd::GetSafeOwner_( NULL, NULL ) );

	BOOL bParentEnabled = FALSE;
	if ( pParent != NULL )
		bParentEnabled = pParent -> IsWindowEnabled();

	if ( NULL == pParent )
		AfxMessageBox ( szMsg, nIconType | MB_OK );
	else if ( PresentationOptions.GetCustomMessageBoxFlag() == FALSE )
		pParent -> MessageBox ( szMsg, szTitle, nIconType | MB_OK );
	else
	{
		CMyMessageBoxInfo infoMessage;
		infoMessage.SetIconType( nIconType );
		infoMessage.SetButtonMode( MB_OK );
		infoMessage.SetLineGap( PresentationOptions.GetCustomMessageBoxLineGap() );
		infoMessage.SetFontSize( PresentationOptions.GetCustomMessageBoxFont() );
		infoMessage.SetBackColour( PresentationOptions.GetCustomMessageBoxColourBackInt() );
		infoMessage.SetTextColour( PresentationOptions.GetCustomMessageBoxColourTextInt() );
		infoMessage.SetTitle( szTitle );
		infoMessage.SetTexts( szMsg );
		
		CMyMessageBoxDlg dlg( infoMessage, pParent );
		dlg.DoModal();
	}

	if ( ( FALSE == bParentEnabled ) && ( TRUE == m_bPreventParentEnable ) && ( pParent != NULL ) )
		pParent -> EnableWindow( FALSE );
}

/**********************************************************************/

void CPrompter::Info ( const char* szInfo, const char* szTitle )
{
	CString strTitle = szTitle;
	if ( strTitle == "" ) strTitle = ::GetTidyAppName(FALSE);
	OKMessageBox( strTitle, szInfo, MB_ICONINFORMATION );
}

/**********************************************************************/

void CPrompter::FatalError ( const char* szError, const char* szTitle )
{
	CString strTitle = szTitle;
	if ( strTitle == "" ) strTitle =::GetTidyAppName(FALSE);
	OKMessageBox( strTitle, szError, MB_ICONSTOP );
}

/**********************************************************************/

void CPrompter::Diagnostic ( const char* szInfo )
{
	OKMessageBox( "Diagnostic", szInfo, 0 );
}

/**********************************************************************/

void CPrompter::Error ( const char* szError, const char* szTitle )
{
	CString strTitle = szTitle;
	if ( strTitle == "" ) strTitle = ::GetTidyAppName(FALSE);
	OKMessageBox( strTitle, szError, MB_ICONEXCLAMATION );
}

/**********************************************************************/

void CPrompter::LockError( const char* szType )
{
	CString strMsg = "";
	strMsg.Format ( "You cannot %s while another user is logged on", szType );
	OKMessageBox( "Access Denied", strMsg, MB_ICONEXCLAMATION );
}

/**********************************************************************/

void CPrompter::ContactDealer( const char* szError, bool bFatal )
{
	CString strError = szError;
	strError += "\n\nPlease contact your dealer for further advice.";

	if ( bFatal == FALSE )
		Error( strError );
	else
		FatalError( strError );
}

/**********************************************************************/
/**********************************************************************/

int CPrompter::OkCancel ( const char* szPrompt, const char* szTitle )
{
	CWnd* pParent = CWnd::FromHandle( CWnd::GetSafeOwner_( NULL, NULL ) );

	CString strTitle = szTitle;
	if ( strTitle == "" ) strTitle = ::GetTidyAppName(FALSE);

	BOOL bParentEnabled = FALSE;
	if ( pParent != NULL )
		bParentEnabled = pParent -> IsWindowEnabled();

	int nResult;
	CString strPrompt = szPrompt;

	if ( ( PresentationOptions.GetCustomMessageBoxFlag() == FALSE ) || ( NULL == pParent ) )
	{
		nResult = AfxMessageBox ( strPrompt, MB_ICONEXCLAMATION | MB_OKCANCEL );
	}
	else
	{
		CMyMessageBoxInfo infoMessage;
		infoMessage.SetIconType( MB_ICONEXCLAMATION );
		infoMessage.SetButtonMode( MB_OKCANCEL );
		infoMessage.SetLineGap( PresentationOptions.GetCustomMessageBoxLineGap() );
		infoMessage.SetFontSize( PresentationOptions.GetCustomMessageBoxFont() );
		infoMessage.SetBackColour( PresentationOptions.GetCustomMessageBoxColourBackInt() );
		infoMessage.SetTextColour( PresentationOptions.GetCustomMessageBoxColourTextInt() );
		infoMessage.SetTexts( strPrompt );
		infoMessage.SetTitle( strTitle );

		CMyMessageBoxDlg dlg( infoMessage, pParent );
		nResult = dlg.DoModal();
	}

	if ( ( FALSE == bParentEnabled ) && ( TRUE == m_bPreventParentEnable ) && ( pParent != NULL ) )
		pParent -> EnableWindow( FALSE );

	return nResult;
}

/**********************************************************************/
/**********************************************************************/

int CPrompter::YesNo ( const char* szPrompt, const char* szTitle, bool bYes )
{
	CWnd* pParent = CWnd::FromHandle( CWnd::GetSafeOwner_( NULL, NULL ) );

	CString strTitle = szTitle;
	if ( strTitle == "" ) strTitle = ::GetTidyAppName(FALSE);

	BOOL bParentEnabled = FALSE;
	if ( pParent != NULL )
		bParentEnabled = pParent -> IsWindowEnabled();

	int nResult;
	CString strPrompt = szPrompt;
	
	if ( ( PresentationOptions.GetCustomMessageBoxFlag() == FALSE ) || ( NULL == pParent ) )
	{
		if ( bYes == FALSE )
			nResult = AfxMessageBox ( strPrompt, MB_ICONQUESTION | MB_YESNO | MB_DEFBUTTON2 );
		else
			nResult = AfxMessageBox ( strPrompt, MB_ICONQUESTION | MB_YESNO );
	}
	else
	{
		CMyMessageBoxInfo infoMessage;
		infoMessage.SetIconType( MB_ICONQUESTION );
		infoMessage.SetButtonMode( MB_YESNO );
		infoMessage.SetDefButton( bYes ? 1 : 2 );
		infoMessage.SetLineGap( PresentationOptions.GetCustomMessageBoxLineGap() );
		infoMessage.SetFontSize( PresentationOptions.GetCustomMessageBoxFont() );
		infoMessage.SetBackColour( PresentationOptions.GetCustomMessageBoxColourBackInt() );
		infoMessage.SetTextColour( PresentationOptions.GetCustomMessageBoxColourTextInt() );
		infoMessage.SetTitle( strTitle );
		infoMessage.SetTexts( strPrompt );

		CMyMessageBoxDlg dlg( infoMessage, pParent );
		nResult = dlg.DoModal();
	}

	if ( ( FALSE == bParentEnabled ) && ( TRUE == m_bPreventParentEnable ) && ( pParent != NULL ) )
		pParent -> EnableWindow( FALSE );

	return nResult;
}

/**********************************************************************/

int CPrompter::YesNoCancel ( const char* szPrompt, const char* szTitle, bool bYes )
{
	CWnd* pParent = CWnd::FromHandle( CWnd::GetSafeOwner_( NULL, NULL ) );

	CString strTitle = szTitle;
	if ( strTitle == "" ) strTitle = ::GetTidyAppName(FALSE);

	BOOL bParentEnabled = FALSE;
	if ( pParent != NULL )
		bParentEnabled = pParent -> IsWindowEnabled();

	int nResult;
	CString strPrompt = szPrompt;
	
	if ( ( PresentationOptions.GetCustomMessageBoxFlag() == FALSE ) || ( NULL == pParent ) )
	{
		if ( TRUE == bYes )
			nResult = AfxMessageBox ( strPrompt, MB_ICONQUESTION | MB_YESNOCANCEL | MB_DEFBUTTON1 );
		else
			nResult = AfxMessageBox ( strPrompt, MB_ICONQUESTION | MB_YESNOCANCEL | MB_DEFBUTTON2 );
	}
	else
	{
		CMyMessageBoxInfo infoMessage;
		infoMessage.SetIconType( MB_ICONQUESTION );
		infoMessage.SetButtonMode( MB_YESNOCANCEL );
		infoMessage.SetDefButton( bYes ? 1 : 2 );
		infoMessage.SetLineGap( PresentationOptions.GetCustomMessageBoxLineGap() );
		infoMessage.SetFontSize( PresentationOptions.GetCustomMessageBoxFont() );
		infoMessage.SetBackColour( PresentationOptions.GetCustomMessageBoxColourBackInt() );
		infoMessage.SetTextColour( PresentationOptions.GetCustomMessageBoxColourTextInt() );
		infoMessage.SetTexts( strPrompt );
		infoMessage.SetTitle( szTitle );

		CMyMessageBoxDlg dlg( infoMessage, pParent );
		nResult = dlg.DoModal();
	}

	if ( ( FALSE == bParentEnabled ) && ( TRUE == m_bPreventParentEnable ) && ( pParent != NULL ) )
		pParent -> EnableWindow( FALSE );

	return nResult;
}

/**********************************************************************/

void CPrompter::ShareError( const char* szFile, int nError )
{
	CString strMsg = "";

	if ( 2 == nError )
		strMsg.Format ( "The %s file is currently being modified by another user.\n\nPlease try again later.", szFile );
	else
		strMsg.Format ( "Unable to open the %s file.", szFile );
	
	OKMessageBox( "Access Denied", strMsg, MB_ICONEXCLAMATION );
}

/**********************************************************************/
