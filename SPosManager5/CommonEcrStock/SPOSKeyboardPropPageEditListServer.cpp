/**********************************************************************/
#include "SPOSKeyboardColourServerDlg.h"
#include "SPOSKeyboardPropPageEditListServer.h"
/**********************************************************************/

CSPOSKeyboardPropPageEditListServer::CSPOSKeyboardPropPageEditListServer() : CSSPropertyPage(CSPOSKeyboardPropPageEditListServer::IDD)
{
	//{{AFX_DATA_INIT(CSPOSKeyboardPropPageEditListServer)
	//}}AFX_DATA_INIT
	m_nButtonID[0] = IDC_BUTTON_COLOUR1;
	m_nButtonID[1] = IDC_BUTTON_COLOUR2;
	m_nButtonID[2] = IDC_BUTTON_COLOUR3;
	m_nButtonID[3] = IDC_BUTTON_COLOUR4;
	m_nButtonID[4] = IDC_BUTTON_COLOUR5;
	m_nButtonID[5] = IDC_BUTTON_COLOUR6;
	m_nButtonID[6] = IDC_BUTTON_COLOUR7;

	for ( int n = 0; n < 7; n++ )
		m_buttonColour[n].m_nButtonNo = n;

	m_nBackground = 0;
	m_nStartKeyIdx = 0;
	m_nEndKeyIdx = -1;
}

/**********************************************************************/

void CSPOSKeyboardPropPageEditListServer::SetListRecord( CSPOSKeyboardListRecord* pRecord )
{
	m_pListRecord = pRecord;

	for ( int n = 0; n < 7; n++ )
		m_buttonColour[n].m_ColourRefBack = SPOSKeyboardManager.GetColourTable() -> GetCurrentRGB( 0 );

	SPOSKeyboardManager.GetListManager() -> GetListKeyRange( pRecord -> GetListNo(), m_nStartKeyIdx, m_nEndKeyIdx );
	m_nEndKeyIdx--;

	for ( int n = 0; n < 4; n++ )
	{
		if ( m_nStartKeyIdx + n <= m_nEndKeyIdx )
			SPOSKeyboardManager.GetListManager() -> GetKeyRecord( m_nStartKeyIdx + n, m_KeyRecord[n] );
	}
}

/**********************************************************************/

CSPOSKeyboardPropPageEditListServer::~CSPOSKeyboardPropPageEditListServer()
{
}

/**********************************************************************/

void CSPOSKeyboardPropPageEditListServer::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSPOSKeyboardPropPageEditListServer)
	//}}AFX_DATA_MAP
	for ( int n = 0; n < 7; n++ )
		DDX_Control(pDX, m_nButtonID[n], m_buttonColour[n]);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CSPOSKeyboardPropPageEditListServer, CPropertyPage)
	//{{AFX_MSG_MAP(CSPOSKeyboardPropPageEditListServer)
	ON_BN_CLICKED(IDC_BUTTON_BACKGROUND, OnSwitchBackground)
	//}}AFX_MSG_MAP
	ON_MESSAGE ( WM_APP, OnColourButtonClick)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CSPOSKeyboardPropPageEditListServer::OnInitDialog() 
{
	CPropertyPage::OnInitDialog(); 

	for ( int n = 0; n < 7; n++ )
		RedrawButton(n);

	return TRUE;
}

/**********************************************************************/

void CSPOSKeyboardPropPageEditListServer::RedrawButton( int n )
{
	switch( n )
	{
	case 0:
		m_buttonColour[0].m_ColourRefBack = SPOSKeyboardManager.GetColourTable() -> GetCurrentRGB( m_KeyRecord[0].GetBackground() );
		break;

	case 1:
		m_buttonColour[1].m_ColourRefBack = SPOSKeyboardManager.GetColourTable() -> GetCurrentRGB( m_KeyRecord[1].GetBackground() );
		break;

	case 2:
		m_buttonColour[2].m_ColourRefBack = SPOSKeyboardManager.GetColourTable() -> GetCurrentRGB( m_KeyRecord[2].GetBackground() );
		break;

	case 3:
		m_buttonColour[3].m_ColourRefBack = SPOSKeyboardManager.GetColourTable() -> GetCurrentRGB( m_KeyRecord[m_nBackground].GetBackground() );
		m_buttonColour[3].m_ColourRefText = SPOSKeyboardManager.GetColourTable() -> GetCurrentRGB( m_KeyRecord[0].GetForeground() );
		m_buttonColour[3].SetFontSize ( m_KeyRecord[0].GetFontSize() );
		m_buttonColour[3].SetBoldFlag ( m_KeyRecord[0].GetBoldFlag() );
		m_buttonColour[3].SetHAlign ( m_KeyRecord[0].GetAlignmentH() );
		m_buttonColour[3].SetVAlign  ( m_KeyRecord[0].GetAlignmentV() );
		m_buttonColour[3].SetWindowText( "name" );
		break;

	case 4:
		m_buttonColour[4].m_ColourRefBack = SPOSKeyboardManager.GetColourTable() -> GetCurrentRGB( m_KeyRecord[m_nBackground].GetBackground() );
		m_buttonColour[4].m_ColourRefText = SPOSKeyboardManager.GetColourTable() -> GetCurrentRGB( m_KeyRecord[1].GetForeground() );
		m_buttonColour[4].SetFontSize ( m_KeyRecord[1].GetFontSize() );
		m_buttonColour[4].SetBoldFlag ( m_KeyRecord[1].GetBoldFlag() );
		m_buttonColour[4].SetHAlign ( m_KeyRecord[1].GetAlignmentH() );
		m_buttonColour[4].SetVAlign  ( m_KeyRecord[1].GetAlignmentV() );
		m_buttonColour[4].SetWindowText( "number" );
		break;

	case 5:
		m_buttonColour[5].m_ColourRefBack = SPOSKeyboardManager.GetColourTable() -> GetCurrentRGB( m_KeyRecord[m_nBackground].GetBackground() );
		m_buttonColour[5].m_ColourRefText = SPOSKeyboardManager.GetColourTable() -> GetCurrentRGB( m_KeyRecord[2].GetForeground() );
		m_buttonColour[5].SetFontSize ( m_KeyRecord[2].GetFontSize() );
		m_buttonColour[5].SetBoldFlag ( m_KeyRecord[2].GetBoldFlag() );
		m_buttonColour[5].SetHAlign ( m_KeyRecord[2].GetAlignmentH() );
		m_buttonColour[5].SetVAlign  ( m_KeyRecord[2].GetAlignmentV() );
		m_buttonColour[5].SetWindowText( "till" );
		break;

	case 6:
		m_buttonColour[6].m_ColourRefBack = SPOSKeyboardManager.GetColourTable() -> GetCurrentRGB( m_KeyRecord[m_nBackground].GetBackground() );
		m_buttonColour[6].m_ColourRefText = SPOSKeyboardManager.GetColourTable() -> GetCurrentRGB( m_KeyRecord[3].GetForeground() );
		m_buttonColour[6].SetFontSize ( m_KeyRecord[3].GetFontSize() );
		m_buttonColour[6].SetBoldFlag ( m_KeyRecord[3].GetBoldFlag() );
		m_buttonColour[6].SetHAlign ( m_KeyRecord[3].GetAlignmentH() );
		m_buttonColour[6].SetVAlign  ( m_KeyRecord[3].GetAlignmentV() );
		m_buttonColour[6].SetWindowText( "amount" );
		break;
	}

	m_buttonColour[n].Invalidate();
}

/**********************************************************************/

void CSPOSKeyboardPropPageEditListServer::OnSwitchBackground()
{
	m_nBackground = ( 0 == m_nBackground ) ? 1 : 0;

	for ( int n = 3; n < 7; n++ )
		RedrawButton(n);
}

/**********************************************************************/

BOOL CSPOSKeyboardPropPageEditListServer::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	SaveRecord();
	return bResult;
}

/**********************************************************************/

BOOL CSPOSKeyboardPropPageEditListServer::OnSetActive() 
{
	bool bResult = ( CPropertyPage::OnSetActive() != 0 );
	return bResult;
}

/**********************************************************************/

long CSPOSKeyboardPropPageEditListServer::OnColourButtonClick( WPARAM wIndex, LPARAM lParam )
{
	if ( wIndex >= 0 && wIndex <= 6 )
	{
		if ( 0 == lParam )
		{
			switch( wIndex )
			{
			case 0:
			case 1:
			case 2:
				{
					if ( m_nEndKeyIdx >= m_nStartKeyIdx + (int) wIndex )
					{
						CSPOSKeyboardColourServerDlg dlg( m_KeyRecord[wIndex], wIndex, this );
						dlg.DoModal();

						RedrawButton(wIndex);
						RedrawButton(3);
						RedrawButton(4);
						RedrawButton(5);
						RedrawButton(6);
					}
				}
				break;

			case 3:
			case 4:
			case 5:
			case 6:
				{
					int nKeyIdx = wIndex - 3;
					if ( m_nEndKeyIdx >= m_nStartKeyIdx + nKeyIdx )
					{
						int nBgnd = m_KeyRecord[nKeyIdx].GetBackground();
						m_KeyRecord[nKeyIdx].SetBackground( m_KeyRecord[m_nBackground].GetBackground() );
						
						CSPOSKeyboardColourServerDlg dlg( m_KeyRecord[nKeyIdx], wIndex, this );
						dlg.DoModal();
						
						m_KeyRecord[nKeyIdx].SetBackground( nBgnd );
						RedrawButton( wIndex );
					}
				}
				break;
			}
		}
	}

	return 0l;
}

/**********************************************************************/

void CSPOSKeyboardPropPageEditListServer::SaveRecord()
{		
	for ( int n = 0; n < 4; n++ )
	{
		if ( m_nStartKeyIdx + n <= m_nEndKeyIdx )
			SPOSKeyboardManager.GetListManager() -> SetKeyRecord( m_nStartKeyIdx + n, m_KeyRecord[n] );
	}
}

/**********************************************************************/


