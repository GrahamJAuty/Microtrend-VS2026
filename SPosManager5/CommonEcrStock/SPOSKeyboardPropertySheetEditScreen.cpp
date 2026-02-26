/**********************************************************************/
#include "SPOSKeyboardKeyHelpers.h"
/**********************************************************************/
#include "SPOSKeyboardPropertySheetEditScreen.h"
/**********************************************************************/
IMPLEMENT_DYNAMIC(CSPOSKeyboardPropertySheetEditScreen, CPropertySheet)
/**********************************************************************/

CSPOSKeyboardPropertySheetEditScreen::CSPOSKeyboardPropertySheetEditScreen( int nScreenIdx, CWnd* pWndParent)
	 : CSSPropertySheet("", pWndParent)
{	
	m_psh.dwFlags |= PSH_NOAPPLYNOW;
	
	AddPage( &m_Page1 );
	AddPage( &m_Page2 );
	AddPage( &m_Page3 );
	
	m_nScreenIdx = nScreenIdx;
	SPOSKeyboardManager.GetListManager() -> GetScreenRecord( m_nScreenIdx, m_ScreenRecord );

	m_Page1.SetScreenRecord ( &m_ScreenRecord );
	m_Page2.SetScreenRecord ( &m_ScreenRecord );
	m_Page3.SetScreenRecord ( &m_ScreenRecord );
	
	m_Page1.SetPropertySheet(this);
	m_Page2.SetPropertySheet(this);
	m_Page3.SetPropertySheet(this);

	CString strTitle;
	strTitle.Format ( "Screen Settings (%s)",
		m_ScreenRecord.GetComboText() );

	SetTitle ( strTitle );
}

/**********************************************************************/

CSPOSKeyboardPropertySheetEditScreen::~CSPOSKeyboardPropertySheetEditScreen()
{
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CSPOSKeyboardPropertySheetEditScreen, CPropertySheet)
	//{{AFX_MSG_MAP(CSPOSKeyboardPropertySheetEditScreen)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CSPOSKeyboardPropertySheetEditScreen::OnInitDialog() 
{
	BOOL bResult = CPropertySheet::OnInitDialog();
	return bResult;
}

/**********************************************************************/

void CSPOSKeyboardPropertySheetEditScreen::RefreshListCombo( CSSComboBox& combo, CArray<__int64,__int64>& arrayListNo, __int64 nListNoToSelect, int nListType )
{
	if ( nListNoToSelect < 0 )
		nListNoToSelect = GetComboListNo( combo, arrayListNo );

	arrayListNo.RemoveAll();

	combo.ResetContent();
	combo.AddString( "none" );
	arrayListNo.Add(0);

	int nSel = 0;

	for ( int n = 0; n < SPOSKeyboardManager.GetListManager() -> GetListCount(); n++ )
	{
		CSPOSKeyboardListRecord ListRecord;
		SPOSKeyboardManager.GetListManager() -> GetListRecord( n, ListRecord );

		if ( ListRecord.GetListType() == nListType )
		{
			CSPOSKeyboardKeyHelpers KeyHelpers;
			__int64 nListNo = ListRecord.GetListNo();
			CString strCombiText = KeyHelpers.GetCombiText( KEY_TYPE_LIST, nListNo );

			int nPos = combo.AddString( strCombiText );
			arrayListNo.InsertAt( nPos, nListNo );

			if ( nListNo == nListNoToSelect )
				nSel = nPos;
		}
	}

	combo.SetCurSel( nSel );
	combo.EnableWindow( combo.GetCount() >= 2 );
}

/**********************************************************************/

__int64 CSPOSKeyboardPropertySheetEditScreen::GetComboListNo( CSSComboBox& combo, CArray<__int64,__int64>& arrayListNo )
{
	int nSel = combo.GetCurSel();
	if ( ( nSel < 0 ) || ( nSel >= arrayListNo.GetSize() ) )
		return 0;
	else
		return arrayListNo.GetAt( nSel );
}

/**********************************************************************/
	
void CSPOSKeyboardPropertySheetEditScreen::SaveRecord()
{
	SPOSKeyboardManager.GetListManager() -> SetScreenRecord( m_nScreenIdx, m_ScreenRecord );	
}

/**********************************************************************/
