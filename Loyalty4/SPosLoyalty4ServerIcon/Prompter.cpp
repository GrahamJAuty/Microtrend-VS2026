/**********************************************************************/
#include "PresentationOptions.h"
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

void CPrompter::Error ( const char* szError, const char* szTitle )
{
	CString strTitle = szTitle;
	if ( strTitle == "" ) strTitle = ::GetTidyAppName(FALSE);
	OKMessageBox( strTitle, szError, MB_ICONEXCLAMATION );
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

