#pragma once
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#define PLUINACTIVE_CONFIRM 0
#define PLUINACTIVE_CANCEL 1
#define PLUINACTIVE_CONTINUE 2
/**********************************************************************/

class CPluInactiveCloseDlg : public CDialog
{
public:
	CPluInactiveCloseDlg( int nCount, CWnd* pParent = NULL);   
	virtual ~CPluInactiveCloseDlg();

	enum { IDD = IDD_PLU_INACTIVE_CLOSE };

protected:
	virtual void OnOK();
	virtual void OnCancel();

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonConfirm();
	afx_msg void OnButtonCancel();
	afx_msg void OnButtonContinue();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	DECLARE_MESSAGE_MAP()

private:
	CStatic m_static1;
	CStatic m_static2;
	CButton m_buttonConfirm;
	CButton m_buttonCancel;

private:
	int m_nCount;

public:
	int m_nAction;
};

/**********************************************************************/

