/**********************************************************************/
#include "MyMessageBoxDlg.h"
/**********************************************************************/

CMyMessageBoxDlg::CMyMessageBoxDlg(CMyMessageBoxDlg** pParentToThis, CMyMessageBoxInfo& MessageBoxInfo, CWnd* pParent)
	: CSSDialog(CMyMessageBoxDlg::IDD, pParent), m_MessageBoxInfo( MessageBoxInfo ), m_staticMessage( MessageBoxInfo )
{
	m_pParentToThis = pParentToThis;
	*m_pParentToThis = this;
	m_bIsModeless = TRUE;
}

/**********************************************************************/

CMyMessageBoxDlg::CMyMessageBoxDlg(CMyMessageBoxInfo& MessageBoxInfo, CWnd* pParent)
	: CSSDialog(CMyMessageBoxDlg::IDD, pParent), m_MessageBoxInfo(MessageBoxInfo), m_staticMessage(MessageBoxInfo)
{
	m_bIsModeless = FALSE;
}

/**********************************************************************/

void CMyMessageBoxDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyMessageBoxDlg)
	DDX_Control(pDX, IDC_STATIC_MESSAGE, m_staticMessage);
	DDX_Control(pDX, IDC_BUTTON_ACTION1, m_buttonAction1);
	DDX_Control(pDX, IDC_BUTTON_ACTION2, m_buttonAction2);
	DDX_Control(pDX, IDC_BUTTON_ACTION3, m_buttonAction3);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CMyMessageBoxDlg, CDialog)
	//{{AFX_MSG_MAP(CMyMessageBoxDlg)
	ON_BN_CLICKED( IDC_BUTTON_ACTION1, OnButtonAction1)
	ON_BN_CLICKED( IDC_BUTTON_ACTION2, OnButtonAction2)
	ON_BN_CLICKED( IDC_BUTTON_ACTION3, OnButtonAction3)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

int CMyMessageBoxDlg::PixelsToDBUs( int nPixels, bool bHeight )
{
	static const int nMultiplier = 1000;
	CRect r ( 0, 0, nMultiplier, nMultiplier );
	MapDialogRect(&r);

	if ( TRUE == bHeight )
		return nPixels * nMultiplier/ r.Height();
	else
		return nPixels * nMultiplier/ r.Width();
}

/**********************************************************************/

BOOL CMyMessageBoxDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	{
		CString strTitle = m_MessageBoxInfo.GetTitle();

		if ( strTitle != "" )
			SetWindowText( strTitle );
		else
			SetWindowText( AfxGetAppName() );
	}

	m_staticMessage.CalculateTextExtents();
	int nTextWidth = PixelsToDBUs( m_staticMessage.GetTextWidth(), FALSE );
	int nTextHeight = PixelsToDBUs( m_staticMessage.GetTextHeight(), TRUE );

	MoveControl( &m_staticMessage, 0, 0, nTextWidth + 80, nTextHeight );
	MoveControl( &m_buttonAction1, nTextWidth - 87, nTextHeight + 5 );
	MoveControl( &m_buttonAction2, nTextWidth - 32, nTextHeight + 5 );
	MoveControl( &m_buttonAction3, nTextWidth + 23, nTextHeight + 5 );
	ResizeDialog( nTextWidth + 80, nTextHeight + 26 );
	m_staticMessage.AllowPaint();

	int nFirstButtonOffset = 0;
	switch( m_MessageBoxInfo.GetButtonMode() )
	{
	case MB_YESNO:
		ShowAndEnableWindow( &m_buttonAction1, FALSE );
		m_buttonAction2.SetWindowText( "Yes" );
		m_buttonAction3.SetWindowText( "No" );
		nFirstButtonOffset = 1;
		break;

	case MB_YESNOCANCEL:
		m_buttonAction1.SetWindowText( "Yes" );
		m_buttonAction2.SetWindowText( "No" );
		m_buttonAction3.SetWindowText( "Cancel" );
		nFirstButtonOffset = 0;
		break;

	case MB_OKCANCEL:
		ShowAndEnableWindow( &m_buttonAction1, FALSE );
		m_buttonAction2.SetWindowText( "OK" );
		m_buttonAction3.SetWindowText( "Cancel" );
		nFirstButtonOffset = 1;
		break;

	case MB_OK:
	default:
		ShowAndEnableWindow( &m_buttonAction1, FALSE );
		ShowAndEnableWindow( &m_buttonAction2, FALSE );
		m_buttonAction3.SetWindowText( "OK" );
		nFirstButtonOffset = 2;
		break;
	}

	switch ( m_MessageBoxInfo.GetDefButton() + nFirstButtonOffset )
	{
	case 1:
		SetDefID( IDC_BUTTON_ACTION1 );
		m_buttonAction1.SetFocus();
		break;

	case 2:
		SetDefID( IDC_BUTTON_ACTION2 );
		m_buttonAction2.SetFocus();
		break;

	case 3:
	default:
		SetDefID( IDC_BUTTON_ACTION3 );
		m_buttonAction3.SetFocus();
		break;
	}

	return FALSE;  
}

/**********************************************************************/

void CMyMessageBoxDlg::OnButtonAction1()
{
	if (TRUE == m_bIsModeless)
	{
		DestroyWindow();
	}
	else
	{
		switch (m_MessageBoxInfo.GetButtonMode())
		{
		case MB_YESNOCANCEL:
			EndDialog(IDYES);
			break;
		}
	}
}

/**********************************************************************/

void CMyMessageBoxDlg::OnButtonAction2()
{
	if (TRUE == m_bIsModeless)
	{
		DestroyWindow();
	}
	else
	{
		switch (m_MessageBoxInfo.GetButtonMode())
		{
		case MB_YESNO:
			EndDialog(IDYES);
			break;

		case MB_YESNOCANCEL:
			EndDialog(IDNO);
			break;

		case MB_OKCANCEL:
			EndDialog(IDOK);
			break;
		}
	}
}

/**********************************************************************/

void CMyMessageBoxDlg::OnButtonAction3()
{
	if (TRUE == m_bIsModeless)
	{
		DestroyWindow();
	}
	else
	{
		switch (m_MessageBoxInfo.GetButtonMode())
		{
		case MB_YESNO:
			EndDialog(IDNO);
			break;

		case MB_YESNOCANCEL:
		case MB_OKCANCEL:
			EndDialog(IDCANCEL);
			break;

		case MB_OK:
			EndDialog(IDOK);
			break;
		}
	}
}

/**********************************************************************/

void CMyMessageBoxDlg::OnOK() 
{
	if (TRUE == m_bIsModeless)
	{
		DestroyWindow();
	}
	else
	{
		switch (m_MessageBoxInfo.GetButtonMode())
		{
		case MB_OK:
		case MB_OKCANCEL:
			EndDialog(IDOK);
			break;
		}
	}
}

/**********************************************************************/

void CMyMessageBoxDlg::OnCancel() 
{
	if (TRUE == m_bIsModeless)
	{
		DestroyWindow();
	}
	else
	{
		switch (m_MessageBoxInfo.GetButtonMode())
		{
		case MB_YESNOCANCEL:
		case MB_OKCANCEL:
			EndDialog(IDCANCEL);
			break;

		case MB_OK:
			EndDialog(IDOK);
			break;

		case MB_YESNO:
		default:
			break;
		}
	}
}

/**********************************************************************/

void CMyMessageBoxDlg::PostNcDestroy()
{
	if (TRUE == m_bIsModeless)
	{
		*m_pParentToThis = NULL;
		delete this;
	}
}

/**********************************************************************/