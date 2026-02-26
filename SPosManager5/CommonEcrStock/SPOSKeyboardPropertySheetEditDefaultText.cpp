/**********************************************************************/
#include "SPOSKeyboardKeyHelpers.h"
/**********************************************************************/
#include "SPOSKeyboardPropertySheetEditDefaultText.h"
/**********************************************************************/
IMPLEMENT_DYNAMIC(CSPOSKeyboardPropertySheetEditDefaultText, CPropertySheet)
/**********************************************************************/

CSPOSKeyboardPropertySheetEditDefaultText::CSPOSKeyboardPropertySheetEditDefaultText( int nType, CSPOSKeyboardDefaultTextRecord& TextRecord, CCascadingMacroCSVRecord& MacroRecord, bool bWantMacroTab, CWnd* pWndParent)
	 : CSSPropertySheet("", pWndParent), m_DefaultTextRecord ( TextRecord ), m_MacroRecord( MacroRecord )
{	
	m_psh.dwFlags |= PSH_NOAPPLYNOW;
	
	switch( nType )
	{
	case KEY_TYPE_LIST:
		AddPage( &m_Page2 );
		AddPage( &m_Page3 );
		break;

	case KEY_TYPE_MACRO:
		AddPage( &m_Page2 );
		AddPage( &m_Page3 );

		if ( TRUE == bWantMacroTab )
		{
			AddPage( &m_Page4 );
		}
		break;

	default:
		AddPage( &m_Page1 );
		AddPage( &m_Page2 );
		AddPage( &m_Page3 );
		break;
	}
		
	m_Page1.SetDefaultTextMode();

	m_KeyRecord.SetType( nType );
	m_KeyRecord.SetValue( m_DefaultTextRecord.GetValue() );
	m_KeyRecord.SetForeground( m_DefaultTextRecord.GetForeground() );
	m_KeyRecord.SetBackground( m_DefaultTextRecord.GetBackground() );
	m_KeyRecord.SetFontSize( m_DefaultTextRecord.GetFontSize() );
	m_KeyRecord.SetAlignmentH( m_DefaultTextRecord.GetHAlign() );
	m_KeyRecord.SetAlignmentV( m_DefaultTextRecord.GetVAlign() );
	m_KeyRecord.SetBoldFlag( m_DefaultTextRecord.GetBoldFlag() );
	m_KeyRecord.SetImageFilename( m_DefaultTextRecord.GetImageFilename() );
	m_KeyRecord.SetImageScale( m_DefaultTextRecord.GetImageScale() );

	CString strText;
	m_DefaultTextRecord.GetUserDefaultText( strText );
	m_KeyRecord.SetKeyText( strText );

	m_Page1.SetKeyRecord ( &m_KeyRecord );
	m_Page2.SetKeyRecord ( &m_KeyRecord );
	m_Page3.SetKeyRecord ( &m_KeyRecord );
	m_Page4.SetMacroRecord( &m_MacroRecord );
	
	CString strTitle = "";
	CSPOSKeyboardKeyHelpers KeyHelpers;
	strTitle.Format ( "Key Defaults (%s)", KeyHelpers.GetCombiText( m_KeyRecord.GetType(), m_KeyRecord.GetValue() ) );
	SetTitle ( strTitle );
}

/**********************************************************************/

CSPOSKeyboardPropertySheetEditDefaultText::~CSPOSKeyboardPropertySheetEditDefaultText()
{
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CSPOSKeyboardPropertySheetEditDefaultText, CPropertySheet)
	//{{AFX_MSG_MAP(CSPOSKeyboardPropertySheetEditDefaultText)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CSPOSKeyboardPropertySheetEditDefaultText::OnInitDialog() 
{
	return CPropertySheet::OnInitDialog(); 
}

/**********************************************************************/

void CSPOSKeyboardPropertySheetEditDefaultText::SaveRecord()
{
	m_DefaultTextRecord.SetForeground( m_KeyRecord.GetForeground() );
	m_DefaultTextRecord.SetBackground( m_KeyRecord.GetBackground() );
	m_DefaultTextRecord.SetFontSize( m_KeyRecord.GetFontSize() );
	m_DefaultTextRecord.SetHAlign( m_KeyRecord.GetAlignmentH() );
	m_DefaultTextRecord.SetVAlign( m_KeyRecord.GetAlignmentV() );
	m_DefaultTextRecord.SetBoldFlag( m_KeyRecord.GetBoldFlag() );
	m_DefaultTextRecord.SetImageFilename( m_KeyRecord.GetImageFilename() );
	m_DefaultTextRecord.SetImageScale( m_KeyRecord.GetImageScale() );

	CString strText = m_KeyRecord.GetKeyText();
	m_DefaultTextRecord.SetUserDefaultText( strText );
}

/**********************************************************************/
