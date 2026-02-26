#pragma once
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CSPOSKeyboardPluSortPromptDlg : public CSSDialog
{
public:
	CSPOSKeyboardPluSortPromptDlg( bool bOnePageFromList, bool bMultiPageList, CWnd* pParent = NULL);
	virtual ~CSPOSKeyboardPluSortPromptDlg();

	enum { IDD = IDD_SPOS_KEYBOARD_PLUSORT_PROMPT };
	CStatic m_staticLine1;
	CStatic m_staticLine2;
	CStatic m_staticLine3;
	CStatic m_staticLine4;
	CStatic m_staticLine5;
	CStatic m_staticLine6;
	CButton m_buttonPrevious;
	CButton m_buttonNext;
	CButton m_buttonOK;
	CButton m_buttonCancel;

protected:
	virtual void DoDataExchange(CDataExchange* pDX); 
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonPrevious();
	afx_msg void OnButtonNext();
	DECLARE_MESSAGE_MAP()

private:
	bool m_bOnePageFromList;
	bool m_bMultiPageList;
	CWnd* m_pParent;
};

/**********************************************************************/


