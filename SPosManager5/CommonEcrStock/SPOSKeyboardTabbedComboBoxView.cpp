/**********************************************************************/
#include "SPOSKeyboardView.h"
/**********************************************************************/
#include "SPOSKeyboardTabbedComboBoxView.h"
/**********************************************************************/

CSPOSKeyboardTabbedComboBoxView::CSPOSKeyboardTabbedComboBoxView() : CSSTabbedComboBox()
{
}

/**********************************************************************/

void CSPOSKeyboardTabbedComboBoxView::SetViewMode( int nMode )
{
	if ( VIEWMODE_SCREEN == nMode )
		SetMaxColumns(99);
	else
		SetMaxColumns(2);
}

/**********************************************************************/
