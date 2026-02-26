#pragma once
/**********************************************************************/
#include "SPOSKeyboardPropPageListSelect.h"
/**********************************************************************/

class CSPOSKeyboardPropertySheetListSelect : public CSSPropertySheet
{
	DECLARE_DYNAMIC(CSPOSKeyboardPropertySheetListSelect)

public:
	CSPOSKeyboardPropertySheetListSelect( bool bSI, bool bLI, bool bSF, bool bLF, bool bBB, CWnd* pWndParent = NULL);
	virtual ~CSPOSKeyboardPropertySheetListSelect();
		
public:
	//{{AFX_VIRTUAL(CSPOSKeyboardPropertySheetListSelect)
	public:
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

public:
	void SaveRecord();
	
protected:
	//{{AFX_MSG(CSPOSKeyboardPropertySheetListSelect)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CSPOSKeyboardPropPageListSelect m_Page1;
	CSPOSKeyboardPropPageListSelect m_Page2;
	CSPOSKeyboardPropPageListSelect m_Page3;
	CSPOSKeyboardPropPageListSelect m_Page4;
	CSPOSKeyboardPropPageListSelect m_Page5;
};

/**********************************************************************/

