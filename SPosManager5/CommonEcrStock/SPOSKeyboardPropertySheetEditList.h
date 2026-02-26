#pragma once
/**********************************************************************/
#include "SPOSKeyboardListRecord.h"
#include "SPOSKeyboardPropPageEditListGeneral.h"
#include "SPOSKeyboardPropPageEditList86Items.h"
#include "SPOSKeyboardPropPageEditListServer.h"
#include "SPOSKeyboardPropPageEditListTable.h"
/**********************************************************************/

class CSPOSKeyboardPropertySheetEditList : public CSSPropertySheet
{
	DECLARE_DYNAMIC(CSPOSKeyboardPropertySheetEditList)

public:
	CSPOSKeyboardPropertySheetEditList( int nListIdx, CWnd* pWndParent = NULL);
	virtual ~CSPOSKeyboardPropertySheetEditList();
		
public:
	//{{AFX_VIRTUAL(CSPOSKeyboardPropertySheetEditList)
	public:
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

public:
	void SaveRecord();
	bool CheckKeyCount();
	void SetSecondTabType( int nType );
	
protected:
	//{{AFX_MSG(CSPOSKeyboardPropertySheetEditList)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	int m_nListIdx;
	CSPOSKeyboardListRecord m_ListRecord;

private:
	CSPOSKeyboardPropPageEditListGeneral m_Page1;
	CSPOSKeyboardPropPageEditList86Items m_Page2;
	CSPOSKeyboardPropPageEditListServer m_Page3;
	CSPOSKeyboardPropPageEditListTable m_Page4;

private:
	int m_nSecondTabType;
};

/**********************************************************************/


