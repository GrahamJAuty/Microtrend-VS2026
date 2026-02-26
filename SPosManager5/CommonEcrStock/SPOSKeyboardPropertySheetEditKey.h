#pragma once
/**********************************************************************/
#include "SPOSKeyboardPropPageEditKeyType.h"
#include "SPOSKeyboardPropPageEditKeyColour.h"
#include "SPOSKeyboardPropPageEditKeyImage.h"
/**********************************************************************/

class CSPOSKeyboardPropertySheetEditKey : public CSSPropertySheet
{
	DECLARE_DYNAMIC(CSPOSKeyboardPropertySheetEditKey)

public:
	CSPOSKeyboardPropertySheetEditKey( int nKeyIdx, CRect rect, CWnd* pWndParent = NULL);
	virtual ~CSPOSKeyboardPropertySheetEditKey();
		
public:
	//{{AFX_VIRTUAL(CSPOSKeyboardPropertySheetEditKey)
	public:
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

public:
	void SaveRecord();
	void RestoreRecord();

public:
	void RedrawKey();
	
protected:
	//{{AFX_MSG(CSPOSKeyboardPropertySheetEditKey)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CSPOSKeyboardKeyRecord m_KeyRecordOld;
	CSPOSKeyboardKeyRecord m_KeyRecordNew;
	CSPOSKeyboardPropPageEditKeyType m_Page1;
	CSPOSKeyboardPropPageEditKeyColour m_Page2;
	CSPOSKeyboardPropPageEditKeyImage m_Page3;

private:
	void* m_pParentView;
	CRect m_rectParent;
	int m_nKeyIdx;
};

/**********************************************************************/

