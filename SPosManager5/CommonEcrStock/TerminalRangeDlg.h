#pragma once
/**********************************************************************/
 
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CTerminalRangeDlg : public CSSDialog
{
public:
	CTerminalRangeDlg( bool bRemove, CWnd* pParent = NULL);   

	//{{AFX_DATA(CTerminalRangeDlg)
	enum { IDD = IDD_TERMINAL_RANGE };
	CString m_strFrom;
	CString m_strTo;
	CStatic m_staticFrom;
	CStatic m_staticTo;
	//}}AFX_DATA
	
	//{{AFX_VIRTUAL(CTerminalRangeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CTerminalRangeDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CEdit* GetEditFrom();
	CEdit* GetEditTo();

public:
	int m_nFrom;
	int m_nTo;

private:
	bool m_bRemove;
};

/**********************************************************************/
