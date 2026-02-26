#pragma once
/**********************************************************************/
#include "SPOSKeyboardScreenRecord.h"
#include "SPOSKeyboardPropPageEditScreenLoggedOff.h"
#include "SPOSKeyboardPropPageEditScreenPayment.h"
#include "SPOSKeyboardPropPageEditScreenTrading.h"
/**********************************************************************/

class CSPOSKeyboardPropertySheetEditScreen : public CSSPropertySheet
{
	DECLARE_DYNAMIC(CSPOSKeyboardPropertySheetEditScreen)

public:
	CSPOSKeyboardPropertySheetEditScreen( int nScreenIdx, CWnd* pWndParent = NULL);
	virtual ~CSPOSKeyboardPropertySheetEditScreen();
		
public:
	//{{AFX_VIRTUAL(CSPOSKeyboardPropertySheetEditScreen)
	public:
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

public:
	void RefreshListCombo( CSSComboBox& combo, CArray<__int64,__int64>& arrayListNo, __int64 nListNoToSelect, int nListType );
	__int64 GetComboListNo( CSSComboBox& combo, CArray<__int64,__int64>& arrayListNo );

	void SaveRecord();
	
private:
	
protected:
	//{{AFX_MSG(CSPOSKeyboardPropertySheetEditScreen)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	int m_nScreenIdx;
	CSPOSKeyboardScreenRecord m_ScreenRecord;

private:
	CSPOSKeyboardPropPageEditScreenLoggedOff m_Page1;
	CSPOSKeyboardPropPageEditScreenTrading m_Page2;
	CSPOSKeyboardPropPageEditScreenPayment m_Page3;
};

/**********************************************************************/


