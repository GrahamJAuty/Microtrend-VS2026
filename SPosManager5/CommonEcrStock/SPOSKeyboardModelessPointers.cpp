/**********************************************************************/
#include "SPOSKeyboardModelessPointers.h"
/**********************************************************************/

CSPOSKeyboardModelessPointers::CSPOSKeyboardModelessPointers()
{
	m_pColourDropper = NULL;
	m_pView = NULL;
	m_bFilter = TRUE;
	m_nDepartmentFilter = DEPARTMENTSET_ALL;
	m_nCategoryFilter = CATEGORYSET_ALL;
}

/**********************************************************************/

void CSPOSKeyboardModelessPointers::OpenColourDropper( int nMode )
{
	if ( m_pColourDropper != NULL )
	{
		if ( m_pColourDropper -> GetMode() == nMode )
		{
			m_pColourDropper->SetFocus();
			return;
		}
		CloseColourDropper();
	}
	m_pColourDropper = new CSPOSKeyboardColourDropperDlg ( nMode, m_pView );
	m_pColourDropper -> Create ( IDD_SPOS_KEYBOARD_COLOUR_DROPPER, m_pView );

	if ( nMode == COLOURDROPPER_LISTKEY )
	{
		m_pColourDropper -> SetKeyType( SPOSKeyboardManager.GetLayoutOptions() -> GetLastKeyType() );
		m_pColourDropper -> SetKeyValue( SPOSKeyboardManager.GetLayoutOptions() -> GetLastKeyValue() );
		m_pColourDropper -> SetColoursAndTextToDefault();
	}
}

/**********************************************************************/

bool CSPOSKeyboardModelessPointers::IsColourDropperActive()
{
	return ( m_pColourDropper != NULL );
}

/**********************************************************************/

int CSPOSKeyboardModelessPointers::GetColourDropperMode()
{
	if ( m_pColourDropper == NULL )
		return COLOURDROPPER_CLOSED;
	else
		return m_pColourDropper -> GetMode();
}

/**********************************************************************/

void CSPOSKeyboardModelessPointers::CloseColourDropper()
{
	if ( m_pColourDropper != NULL )
	{
		m_pColourDropper -> DestroyWindow();
		m_pColourDropper = NULL;
	}
}

/**********************************************************************/

void CSPOSKeyboardModelessPointers::SetForeground( int n )
{
	if ( m_pColourDropper != NULL )
		m_pColourDropper -> SetForeground( n );
}

/**********************************************************************/

void CSPOSKeyboardModelessPointers::SetBackground( int n )
{
	if ( m_pColourDropper != NULL )
		m_pColourDropper -> SetBackground( n );
}

/**********************************************************************/

void CSPOSKeyboardModelessPointers::SetFontSize( int n )
{
	if ( m_pColourDropper != NULL )
		m_pColourDropper -> SetFontSize( n );
}

/**********************************************************************/

void CSPOSKeyboardModelessPointers::SetHAlign( int n )
{
	if ( m_pColourDropper != NULL )
		m_pColourDropper -> SetHAlign( n );
}

/**********************************************************************/

void CSPOSKeyboardModelessPointers::SetVAlign( int n )
{
	if ( m_pColourDropper != NULL )
		m_pColourDropper -> SetVAlign( n );
}

/**********************************************************************/

void CSPOSKeyboardModelessPointers::SetBoldStatus( int n )
{
	if ( m_pColourDropper != NULL )
		m_pColourDropper -> SetBold( n );
}

/**********************************************************************/

void CSPOSKeyboardModelessPointers::SetDefaultTextAlwaysStatus( int n )
{
	if ( m_pColourDropper != NULL )
		m_pColourDropper -> SetDefTextAlways( n );
}

/**********************************************************************/

void CSPOSKeyboardModelessPointers::SetColoursToNoChange()
{
	if ( m_pColourDropper != NULL )
		m_pColourDropper -> SetColoursToNoChange();
}

/**********************************************************************/

void CSPOSKeyboardModelessPointers::SetColoursToDropAsShown()
{
	if ( m_pColourDropper != NULL )
		m_pColourDropper -> SetColoursToDropAsShown();
}

/**********************************************************************/

void CSPOSKeyboardModelessPointers::SelectImageTab()
{
	if ( m_pColourDropper != NULL )
		m_pColourDropper -> SelectImageTab();
}

/**********************************************************************/

void CSPOSKeyboardModelessPointers::SetImageFlag( bool b )
{
	if ( m_pColourDropper != NULL )
		m_pColourDropper -> SetImageFlag( b );
}

/**********************************************************************/

void CSPOSKeyboardModelessPointers::SetImageFilename( const char* sz )
{
	if ( m_pColourDropper != NULL )
		m_pColourDropper -> SetImageFilename(sz);
}

/**********************************************************************/

void CSPOSKeyboardModelessPointers::SetImageScale( int n )
{
	if ( m_pColourDropper != NULL )
		m_pColourDropper -> SetImageScale(n);
}

/**********************************************************************/

void CSPOSKeyboardModelessPointers::UpdateTitle()
{
	if ( m_pColourDropper != NULL )
		m_pColourDropper -> UpdateTitle();
}

/**********************************************************************/

void CSPOSKeyboardModelessPointers::SetCopyImageTitle()
{
	if ( m_pColourDropper != NULL )
		m_pColourDropper -> SetCopyImageTitle();
}

/**********************************************************************/

void CSPOSKeyboardModelessPointers::UpdateImage()
{
	if ( m_pColourDropper != NULL )
		m_pColourDropper -> UpdateImage();
}

/**********************************************************************/

bool CSPOSKeyboardModelessPointers::CanDropKeyNow()
{
	switch( GetColourDropperMode() )
	{
	case COLOURDROPPER_COPYKEY:
	case COLOURDROPPER_GROUPKEY:
	case COLOURDROPPER_COPYSTYLE:
		return TRUE;

	case COLOURDROPPER_LISTKEY:
		return m_pColourDropper -> CanDropKeyNow();

	case COLOURDROPPER_CLOSED:
	default:
		return FALSE;
	}
}

/**********************************************************************/

void CSPOSKeyboardModelessPointers::UpdateKeyRecord( CSPOSKeyboardKeyRecord& KeyRecord )
{
	if ( m_pColourDropper != NULL )
		m_pColourDropper -> UpdateKeyRecord( KeyRecord );
}

/**********************************************************************/

void CSPOSKeyboardModelessPointers::SetKeyToCopy( CSPOSKeyboardKeyRecord& KeyRecord )
{
	if ( m_pColourDropper != NULL )
		m_pColourDropper -> SetKeyToCopy( KeyRecord );
}

/**********************************************************************/

void CSPOSKeyboardModelessPointers::RedrawView()
{
	if ( m_pView != NULL )
	{
		m_pView -> Invalidate();
		m_pView -> UpdateWindow();
	}
}

/**********************************************************************/

void CSPOSKeyboardModelessPointers::NullifyColourDropper()
{
	if ( (m_pView != NULL) && (GetColourDropperMode() == COLOURDROPPER_GROUPKEY) )
	{
		m_pView -> m_nEditSharpKeyPos = 0;
		m_pView -> Invalidate();
		m_pView -> UpdateWindow();
	}

	m_pColourDropper = NULL;
}

/**********************************************************************/
