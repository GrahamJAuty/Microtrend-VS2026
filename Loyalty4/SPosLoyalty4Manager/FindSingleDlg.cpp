//$$******************************************************************
#include "LoyaltyManager.h"
#include "FindDlg.h"
//$$******************************************************************
#include "FindSingleDlg.h"
//$$******************************************************************

CFindSingleBrowseDlg::CFindSingleBrowseDlg( CWnd* pParent ) : CFindSingleDlg( "", pParent )
{
	m_strTitle = "Find Account";
	m_bShowMiFare = System.IsMiFareSystem();
	m_bShowBrowse = TRUE;
	m_bShowCount = FALSE;
}

//$$******************************************************************

CFindSingleHotReplaceDlg::CFindSingleHotReplaceDlg( const char* szCardNo, CWnd* pParent ) : CFindSingleDlg( "", pParent )
{
	m_strTitle.Format ( "Specify the replacement for AccountNo %s", szCardNo );
	m_bShowMiFare = System.IsMiFareSystem();
	m_bShowBrowse = FALSE;
	m_bShowCount = FALSE;
}

//$$******************************************************************

CFindSingleImportDlg::CFindSingleImportDlg( CWnd* pParent ) : CFindSingleDlg( "", pParent )
{
	m_strTitle = "Enter Starting Point for New Cards";
	m_bShowMiFare = ( System.GetInterfaceType() == nINTERFACE_MIFAREv1 );
	m_bShowBrowse = TRUE;
	m_bShowCount = FALSE;
}

//$$******************************************************************

CFindSingleTransferDlg::CFindSingleTransferDlg( const char* szCardNo, CWnd* pParent ) : CFindSingleDlg( "", pParent )
{
	m_strTitle.Format ( "Enter recipient for account %s data", szCardNo );
	m_bShowMiFare = System.IsMiFareSystem();
	m_bShowBrowse = TRUE;
	m_bShowCount = FALSE;
}

//$$******************************************************************

CFindSingleAddOneDlg::CFindSingleAddOneDlg( const char* szCardNo, CWnd* pParent ) : CFindSingleDlg( szCardNo, pParent )
{
	m_strTitle = "Add Account Record";
	m_bShowMiFare = ( System.GetInterfaceType() == nINTERFACE_MIFAREv1 );
	m_bShowBrowse = FALSE;
	m_bShowCount = FALSE;
}

//$$******************************************************************

CFindSingleAddBatchDlg::CFindSingleAddBatchDlg( const char* szCardNo, CWnd* pParent ) : CFindSingleDlg( szCardNo, pParent )
{
	m_strTitle = "Batch Add Accounts";
	m_bShowMiFare = FALSE;
	m_bShowBrowse = FALSE;
	m_bShowCount = TRUE;
}

//$$******************************************************************

CFindSingleDlg::CFindSingleDlg( const char* szCardNo, CWnd* pParent /*=NULL*/)
	: CSSDialog(CFindSingleDlg::IDD, pParent)
{
	m_strInputCardNo = szCardNo;
	m_strInputMiFareNo = "";
	m_nRecordCount = 1;
	m_bReaderMode = FALSE;
	m_bShowBrowse = TRUE;
	m_bShowMiFare = FALSE;
	m_bShowCount = FALSE;

	if ( m_strInputCardNo.GetLength() > System.GetUserIDLength() )
		m_strInputCardNo = "";

	m_terminal.Read();												
	ClearInput();
}

//$$******************************************************************

void CFindSingleDlg::ClearInput()
{
	m_strOutputCardNo		= "";
	m_strOutputMiFareNo		= "";
}

//$$******************************************************************

void CFindSingleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_PROMPT1, m_staticPrompt1);
	DDX_Control(pDX, IDC_STATIC_PROMPT2, m_staticPrompt2);
	DDX_Control(pDX, IDC_BUTTON_READER, m_buttonReader);
	DDX_Control(pDX, IDC_EDIT_MIFARE, m_editMiFareNo );
	DDX_Control(pDX, IDC_BUTTON_BROWSE, m_buttonBrowse );
	DDX_Control(pDX, IDOK, m_buttonOK );
	DDX_Control(pDX, IDCANCEL, m_buttonCancel );
	DDX_Text(pDX, IDC_EDIT_CARDNO, m_strInputCardNo);
	DDX_Text(pDX, IDC_EDIT_MIFARE, m_strInputMiFareNo);
	DDX_Text(pDX, IDC_EDIT_COUNT, m_nRecordCount);

	if ( TRUE == m_bShowCount )
	{
		DDV_MinMaxInt(pDX, m_nRecordCount, 1, 999);
	}
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CFindSingleDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, OnBnClickedButtonBrowse)
	ON_BN_CLICKED(IDC_BUTTON_READER, OnBnClickedButtonReader)
END_MESSAGE_MAP()

//$$******************************************************************

BOOL CFindSingleDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	if ( TRUE == m_bShowCount )
		m_bShowMiFare = FALSE;

	SetWindowText ( m_strTitle );

	SubclassEdit( IDC_EDIT_CARDNO, SS_NUM, System.GetMaxCardNoDlgLen(), "%s" );
	SubclassEdit( IDC_EDIT_COUNT, SS_NUM, 3, "%s" );
	m_staticPrompt1.SetWindowText( m_bShowCount ? "First new account" : "AccountNo" );
	m_staticPrompt2.SetWindowText( m_bShowCount ? "Accounts to add" : "MiFareNo" );

	MoveControl( GetEditCount(), 74, 28 );

	ShowAndEnableWindow( &m_staticPrompt2, m_bShowMiFare || m_bShowCount );
	ShowAndEnableWindow( &m_editMiFareNo, m_bShowMiFare );
	ShowAndEnableWindow( &m_buttonReader, m_bShowMiFare, ( m_terminal.m_strTerminal != szTERMINAL_NONE ) );
	ShowAndEnableWindow( &m_buttonBrowse, m_bShowBrowse );
	ShowAndEnableWindow( GetEditCount(), m_bShowCount );

	if ( ( FALSE == m_bShowMiFare ) && ( FALSE == m_bShowCount ) )
	{
		MoveControl( &m_buttonOK, 241, 20 );
		MoveControl( &m_buttonCancel, 241, 36 );
		ResizeDialog( 300, 60 );
	}
	else
	{
		MoveControl( &m_buttonOK, 241, 39 );
		MoveControl( &m_buttonCancel, 241, 55 );
		ResizeDialog( 300, 79 );
	}
		
	return TRUE;
}

//$$******************************************************************
CEdit* CFindSingleDlg::GetEditCardNo(){ return GetEdit( IDC_EDIT_CARDNO ); }
CEdit* CFindSingleDlg::GetEditCount(){ return GetEdit( IDC_EDIT_COUNT ); }
//$$******************************************************************

void CFindSingleDlg::OnBnClickedButtonBrowse()
{
	CFindDlg dlg{};
	if ( dlg.DoModal() == IDOK )
	{
		m_strInputCardNo = SimpleFormatInt64Value(dlg.m_nRequestedCardNo);
		UpdateData(FALSE);
	}
}

//$$******************************************************************

void CFindSingleDlg::SetReaderMode ( bool bMode )
{
	m_bReaderMode = bMode;

	bool bAllowed = ( m_bReaderMode == TRUE ) ? FALSE : TRUE;

	GetEditCardNo() -> EnableWindow(bAllowed);
	m_buttonBrowse.EnableWindow( bAllowed & m_bShowBrowse );
	m_buttonReader.EnableWindow( bAllowed );
	m_editMiFareNo.EnableWindow(bAllowed);
	m_buttonOK.EnableWindow( bAllowed );
	
	if ( TRUE == bAllowed )
	{
		GetEditCardNo() -> SetFocus();
		GetEditCardNo() -> SetSel(0,-1);
	}
}

//$$******************************************************************

void CFindSingleDlg::OnBnClickedButtonReader()
{
	m_editMiFareNo.SetWindowText( "" );

	CString strReaderMiFareNo = "";

	SetReaderMode ( TRUE );				// set dialog in reader mode / inhibit other keys

	if ( m_reader.SelectReader ( m_terminal.m_strTerminal, m_terminal.m_strPort, m_terminal.m_strBaud ) == TRUE )
	{
		m_reader.SetUIDFormat ( m_terminal.m_nUIDFormat1 );
		m_reader.SetDataFormat( m_terminal.GetDataFormat1() );
		m_reader.m_pWnd = &m_editMiFareNo;
		m_reader.m_bWaitForCard = TRUE;

		while ( TRUE )
		{
			int nReply = m_reader.GetUID ( TRUE, Server.GetLogCardReadsFlag());
			m_editMiFareNo.SetWindowText ( "" );

			if ( nReply == 0 )		// 0=no error, 1=error, -1 = terminate
			{
				strReaderMiFareNo = m_reader.GetCardNo();
				m_editMiFareNo.SetWindowText( strReaderMiFareNo );
				break;
			}

			else if ( nReply == -1 )
				break;

			else
				Prompter.Error( m_reader.GetError(), "Reader Error" );
		}

		m_reader.DeselectReader();
	}
	else
	{
		CString strError = m_reader.GetError();
		Prompter.Error( strError, "Hardware Error" );
	}

	SetReaderMode ( FALSE );						// enable dialog input

	if ( strReaderMiFareNo != "" )
		if ( ValidateCardNumber( "", strReaderMiFareNo ) == TRUE )
			EndDialog( IDOK );
}

//$$******************************************************************

bool CFindSingleDlg::ValidateCardNumber( const char* szCardNo, const char* szMiFareNo )
{
	CString strTestCardNo = szCardNo;
	CString strTestMiFareNo = szMiFareNo;

	CString strErrorMsg = "";									// assume no error
	m_strOutputCardNo = "";
	m_strOutputMiFareNo = "";

	switch ( System.GetInterfaceType() )
	{
	case nINTERFACE_SWIPEv1:
	case nINTERFACE_SWIPEv2:
	case nINTERFACE_GENERALv1:
		{
			CAccountIdentifier AccountIdentifier( strTestCardNo, System.GetInterfaceType() );
			strErrorMsg = System.TranslateCardNo ( AccountIdentifier, m_strOutputCardNo );
		}
		break;
		
	case nINTERFACE_MIFAREv1:

		if ( strTestMiFareNo != "" )														// see if mifare card read
		{
			CAccountIdentifier AccountIdentifier( strTestMiFareNo, nINTERFACE_MIFAREv1 );
			strErrorMsg = System.TranslateCardNo ( AccountIdentifier, m_strOutputCardNo );	// translate MifareNo to CardNo
		}
		else
		{
			CAccountIdentifier AccountIdentifier( strTestCardNo, nINTERFACE_MIFAREv1 );
			strErrorMsg = System.TranslateCardNo ( AccountIdentifier, m_strOutputCardNo );	// translate MifareNo to CardNo
		}
			
		break;

	case nINTERFACE_MIFAREv2:
	case nINTERFACE_MIFAREv3:
		{
			m_strOutputMiFareNo = strTestMiFareNo;

			if ( strTestMiFareNo != "" )																// see if mifare card read
			{
				CAccountIdentifier AccountIdentifier( strTestMiFareNo, nINTERFACE_MIFAREv2 );
				strErrorMsg = System.TranslateCardNo ( AccountIdentifier, m_strOutputCardNo );		// translate MifareNo to CardNo
			}
			else
				m_strOutputCardNo = strTestCardNo;
		}
		break;
	}

	if ( strErrorMsg != "" )
	{
		Prompter.Error( strErrorMsg, "Validation Error" );
		
		if ( ( strTestMiFareNo != "" ) && ( m_editMiFareNo.IsWindowEnabled() ) )
		{
			m_editMiFareNo.SetFocus();
			m_editMiFareNo.SetSel(0,-1);
		}
		else							
		{
			GetEditCardNo() -> SetFocus();
			GetEditCardNo() -> SetSel(0,-1);
		}

		return FALSE;
	}

	return ( m_strOutputCardNo != "" || m_strOutputMiFareNo != "" );
}

//$$******************************************************************

void CFindSingleDlg::OnOK()
{
	if ( UpdateData() == TRUE )
		if ( ValidateCardNumber( m_strInputCardNo, m_strInputMiFareNo ) == TRUE )
			EndDialog(IDOK);
}

//$$******************************************************************

void CFindSingleDlg::OnCancel()
{
	if ( m_bReaderMode == TRUE )	
		m_reader.Terminate();
	else							
		EndDialog(IDCANCEL);
}

//$$******************************************************************
