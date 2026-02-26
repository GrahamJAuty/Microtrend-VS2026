#pragma once
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CSPOSKeyboardPluSortOptionDlg : public CDialog
{
	DECLARE_DYNAMIC(CSPOSKeyboardPluSortOptionDlg)

public:
	CSPOSKeyboardPluSortOptionDlg(CWnd* pParent = NULL);
	virtual ~CSPOSKeyboardPluSortOptionDlg();

	enum { IDD = IDD_SPOS_KEYBOARD_PLUSORT_OPTION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX); 
	afx_msg void OnButtonCurrent();
	afx_msg void OnButtonAll();
	virtual void OnOK();
	virtual void OnCancel();
	DECLARE_MESSAGE_MAP()

public:
	bool GetAllFlag(){ return m_bAll; }

private:
	bool m_bAll;

};

/**********************************************************************/


