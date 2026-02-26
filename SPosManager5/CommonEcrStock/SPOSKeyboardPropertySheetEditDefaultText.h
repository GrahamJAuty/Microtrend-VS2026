#pragma once
/**********************************************************************/
#include "SPOSKeyboardDefaultTextArray.h"
#include "SPOSKeyboardPropPageEditKeyType.h"
#include "SPOSKeyboardPropPageEditKeyColour.h"
#include "SPOSKeyboardPropPageEditKeyImage.h"
#include "SPOSKeyboardPropPageEditKeyMacro.h"
/**********************************************************************/

class CSPOSKeyboardPropertySheetEditDefaultText : public CSSPropertySheet
{
	DECLARE_DYNAMIC(CSPOSKeyboardPropertySheetEditDefaultText)

public:
	CSPOSKeyboardPropertySheetEditDefaultText( int nType, CSPOSKeyboardDefaultTextRecord& TextRecord, CCascadingMacroCSVRecord& MacroRecord, bool bWantMacroTab, CWnd* pWndParent = NULL);
	virtual ~CSPOSKeyboardPropertySheetEditDefaultText();
		
public:
	//{{AFX_VIRTUAL(CSPOSKeyboardPropertySheetEditDefaultText)
	public:
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL
	
protected:
	//{{AFX_MSG(CSPOSKeyboardPropertySheetEditDefaultText)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void SaveRecord();

private:
	CSPOSKeyboardPropPageEditKeyType m_Page1;
	CSPOSKeyboardPropPageEditKeyColour m_Page2;
	CSPOSKeyboardPropPageEditKeyImage m_Page3;
	CSPOSKeyboardPropPageEditKeyMacro m_Page4;

private:
	CSPOSKeyboardDefaultTextRecord& m_DefaultTextRecord;
	CCascadingMacroCSVRecord& m_MacroRecord;
	CSPOSKeyboardKeyRecord m_KeyRecord;
	CString m_strInitialText1;
	CString m_strInitialText2;
	CString m_strInitialText3;
};

/**********************************************************************/

