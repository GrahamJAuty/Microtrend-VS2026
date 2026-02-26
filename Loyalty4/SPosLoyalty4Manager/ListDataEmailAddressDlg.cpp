/**********************************************************************/
#include "DefEditEmailDlg.h"
#include "DefTextEmailAddress.h"
/**********************************************************************/
#include "ListDataEmailAddressDlg.h"
/**********************************************************************/

CSingleListContentHelpersEmailAddress::CSingleListContentHelpersEmailAddress() : CSingleListContentHelpers()
{
}

/**********************************************************************/

int CSingleListContentHelpersEmailAddress::GetArraySize()
{
	return EmailAddresses.GetSize();
}

/**********************************************************************/

bool CSingleListContentHelpersEmailAddress::PrepareLine( int nIndex, CString& strLine )
{
	strLine = "";

	bool bResult = FALSE;
	if ( ( nIndex >= 0 ) && ( nIndex < GetArraySize() ) )
	{
		strLine.Format ( "%s\t%s", 
			EmailAddresses.GetNickname( nIndex ),
			EmailAddresses.GetDisplayEmailAddress( nIndex ) );

		bResult = TRUE;
	}
	
	return bResult;
}

/**********************************************************************/

void CSingleListContentHelpersEmailAddress::DeleteDatabaseEntry( int nIndex )
{
	if ( ( nIndex >= 0 ) && ( nIndex < EmailAddresses.GetSize() ) )
	{
		EmailAddresses.RemoveAddressBookEntry ( nIndex );
		m_bDeletionEffect = TRUE;
	}
}

/**********************************************************************/

void CSingleListContentHelpersEmailAddress::SetColumns( CSSListCtrlVirtual& list, CWnd* pParent )
{
	list.SubclassDlgItem ( IDC_LIST, pParent );
	list.InsertColumn ( 0, "Nickname", LVCFMT_LEFT, 140 );
	list.InsertColumn ( 1, "Email", LVCFMT_LEFT, 340 );
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CListDataEmailAddressDlg::CListDataEmailAddressDlg( const char* szName, CWnd* pParent) : CListDataDlg ( pParent )
{
	m_strInitialName = szName;
	::TrimSpacesFromString( m_strInitialName, FALSE );

	m_pContentHelpers = new CSingleListContentHelpersEmailAddress();
	m_pDisplayHelpers = new CSingleListDisplayHelpers( m_listData, *m_pContentHelpers );

	m_pDisplayHelpers -> SetTopSingleText( "email address" );
	m_pDisplayHelpers -> SetTopPluralText( "email addresses" );
	m_pDisplayHelpers -> SetBottomSingleText( "address" );
	m_pDisplayHelpers -> SetBottomPluralText( "addresses" );
}

/**********************************************************************/

CListDataEmailAddressDlg::~CListDataEmailAddressDlg()
{
	delete m_pDisplayHelpers;
	delete m_pContentHelpers;
}

/**********************************************************************/

void CListDataEmailAddressDlg::SetTitle()
{
	SetWindowText ( "Setup Email Address Book" );
}

/**********************************************************************/

void CListDataEmailAddressDlg::SetButtonTexts()
{
	m_button1.SetWindowText ( "&Add" );
	m_button2.SetWindowText ( "&Edit" );
	m_button3.SetWindowText ( "&Delete" );
	m_button4.SetWindowText ( "Move &Up" );
	m_button5.SetWindowText ( "Move D&own" );
	m_button6.ShowWindow ( SW_HIDE );

	MoveControl( &m_button1, 368, 7 );
	MoveControl( &m_button2, 368, 47 );
	MoveControl( &m_button3, 368, 87 );
	MoveControl( &m_button4, 368, 147 );
	MoveControl( &m_button5, 368, 187 );
}

/**********************************************************************/

void CListDataEmailAddressDlg::SetColumns()
{
	m_pContentHelpers -> SetColumns( m_listData, this );
}

/**********************************************************************/

void CListDataEmailAddressDlg::PrepareList()
{
	CListDataDlg::PrepareList();

	if ( m_strInitialName != "" )
	{
		CString strName = m_strInitialName;
		strName.MakeUpper();
	
		int nPos = EmailAddresses.FindNameOrFirstAddress( strName );
		
		if ( nPos != -1 )
			SelectLine( nPos );
		else
		{
			CStringArray arrayEmail;
			arrayEmail.Add( m_strInitialName );
			EmailAddresses.AddAddressBookEntry( "", arrayEmail );

			m_listData.SetItemCountEx( GetArraySize() );
			SelectLine( GetArraySize() - 1 );
		}
	}
}
/**********************************************************************/

void CListDataEmailAddressDlg::HandleButton1() 
{
	CStringArray arrayEmail;
	CDefEditEmailDlg dlg ( "Add Email Address", arrayEmail, this );

	if ( dlg.DoModal() == IDOK )
	{		
		EmailAddresses.AddAddressBookEntry ( dlg.m_strName, arrayEmail );	
		m_listData.SetItemCountEx ( GetArraySize() );
		SelectLine ( EmailAddresses.GetSize() - 1 );

		m_WarnCancelAfterChange.SetFlag();
	}
}

/**********************************************************************/

void CListDataEmailAddressDlg::HandleButton2() 
{
	int nIndex = GetSafeSelection();
	if ( nIndex == -1 ) 
		return;

	CStringArray arrayEmail;
	for ( int n = 0; n < EmailAddresses.GetEmailAddressCount( nIndex ); n++ )
		arrayEmail.Add( EmailAddresses.GetEmailAddress( nIndex, n ) );

	CDefEditEmailDlg dlg ( "Edit Email Address", arrayEmail, this );
	dlg.m_strName = EmailAddresses.GetNickname ( nIndex );
	
	if ( dlg.DoModal() == IDOK )
	{
		EmailAddresses.SetAddressBookEntry ( nIndex, dlg.m_strName, arrayEmail );
		SelectLine ( nIndex );

		m_WarnCancelAfterChange.SetFlag();
	}
}

/**********************************************************************/

void CListDataEmailAddressDlg::HandleButton3() 
{
	m_pDisplayHelpers -> HandleDelete( this );
	m_WarnCancelAfterChange.SetFlag( m_pContentHelpers -> GetDeletionEffectFlag() );
}

/**********************************************************************/

void CListDataEmailAddressDlg::HandleButton4()
{
	int nIndex = GetSafeSelection();
	if ( nIndex <= 0 ) 
		return;

	EmailAddresses.SwapWithNext( nIndex - 1 );
	SelectLine( nIndex - 1 );

	m_WarnCancelAfterChange.SetFlag();
}

/**********************************************************************/

void CListDataEmailAddressDlg::HandleButton5()
{
	int nIndex = GetSafeSelection();
	if ( nIndex >= GetArraySize() - 1 ) 
		return;

	EmailAddresses.SwapWithNext( nIndex );
	SelectLine( nIndex + 1 );

	m_WarnCancelAfterChange.SetFlag();
}

/**********************************************************************/

const char* CListDataEmailAddressDlg::GetSelectedText()
{
	if ( m_nListSelection >= 0 && m_nListSelection < EmailAddresses.GetSize() )
		m_strSelectedText = EmailAddresses.GetComboText( m_nListSelection );
	else
		m_strSelectedText = "";

	return m_strSelectedText;
}

/**********************************************************************/