/**********************************************************************/
#include "SPOSKeyboardKeyHelpers.h"
#include "SPOSKeyboardView.h"
/**********************************************************************/
#include "SPOSKeyboardPropertySheetEditKey.h"
/**********************************************************************/
IMPLEMENT_DYNAMIC(CSPOSKeyboardPropertySheetEditKey, CPropertySheet)
/**********************************************************************/

CSPOSKeyboardPropertySheetEditKey::CSPOSKeyboardPropertySheetEditKey( int nKeyIdx, CRect rect, CWnd* pWndParent)
	 : CSSPropertySheet("", pWndParent)
{	
	m_rectParent = rect;
	m_pParentView = pWndParent;

	m_psh.dwFlags |= PSH_NOAPPLYNOW;
	
	AddPage( &m_Page1 );
	AddPage( &m_Page2 );
	AddPage( &m_Page3 );

	m_nKeyIdx = nKeyIdx;
	SPOSKeyboardManager.GetListManager() -> GetKeyRecord( m_nKeyIdx, m_KeyRecordOld );
	SPOSKeyboardManager.GetListManager() -> GetKeyRecord( m_nKeyIdx, m_KeyRecordNew );

	m_Page1.SetKeyRecord ( &m_KeyRecordNew );
	m_Page2.SetKeyRecord ( &m_KeyRecordNew );
	m_Page3.SetKeyRecord ( &m_KeyRecordNew );

	m_Page2.SetView ( (CSPOSKeyboardView*) m_pParentView );

	m_Page1.SetParentSheet( this );
	m_Page2.SetParentSheet( this );
	m_Page3.SetParentSheet( this );

	CString strTitle;
	CSPOSKeyboardKeyHelpers KeyHelpers;
	strTitle.Format ( "Edit Key (%s)", KeyHelpers.GetCombiText( m_KeyRecordNew.GetType(), m_KeyRecordNew.GetValue() ) );
	SetTitle ( strTitle );
}

/**********************************************************************/

CSPOSKeyboardPropertySheetEditKey::~CSPOSKeyboardPropertySheetEditKey()
{
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CSPOSKeyboardPropertySheetEditKey, CPropertySheet)
	//{{AFX_MSG_MAP(CSPOSKeyboardPropertySheetEditKey)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

void CSPOSKeyboardPropertySheetEditKey::RedrawKey()
{
	SaveRecord();
	( (CSPOSKeyboardView*) m_pParentView ) -> RedrawEditedKey();
}

/**********************************************************************/

void CSPOSKeyboardPropertySheetEditKey::SaveRecord()
{
	//AUTOMATIC KEY TEXT UPDATE IF USER HAS CHANGED TYPE OR VALUE BUT NOT TEXT
	/*
	int nNewType = m_KeyRecordNew.GetType();
	__int64 nNewValue = m_KeyRecordNew.GetValue();
	
	{
		CString strInitialText = m_KeyRecordEdit.GetKeyText();
		if ( strInitialText == m_KeyRecordNew.GetKeyText() ) 
		{
			CString strText;
			
			CKeyHelpers KeyHelpers;
			KeyHelpers.GetDefaultText( nNewType, nNewValue, strText, FALSE );
			m_KeyRecordNew.SetKeyText( strText );
		}
	
		CString strInitialImage = m_KeyRecordEdit.GetImageFilename();
		if ( strInitialImage == m_KeyRecordNew.GetImageFilename() )
		{
			CKeyHelpers KeyHelpers;
			CDefaultTextRecord TextRecord;
			KeyHelpers.GetDefaultTextRecord( m_KeyRecordNew, TextRecord );
			m_KeyRecordNew.SetImageFilename( TextRecord.GetImageFilename() );
			m_KeyRecordNew.SetImageScale( TextRecord.GetImageScale() );
		}
	}
	*/
	SPOSKeyboardManager.GetListManager() -> SetKeyRecord( m_nKeyIdx, m_KeyRecordNew );
}

/**********************************************************************/

void CSPOSKeyboardPropertySheetEditKey::RestoreRecord()
{
	SPOSKeyboardManager.GetListManager() -> SetKeyRecord( m_nKeyIdx, m_KeyRecordOld );
}

/**********************************************************************/

BOOL CSPOSKeyboardPropertySheetEditKey::OnInitDialog() 
{
	BOOL bResult = CPropertySheet::OnInitDialog();
	
	if ( SPOSKeyboardManager.GetLayoutOptions() -> GetEditKeyTopLeftFlag() == TRUE )
		SetWindowPos( GetParent(), m_rectParent.left, m_rectParent.top, 0, 0, SWP_NOSIZE );
	
	return bResult;
}

/**********************************************************************/
